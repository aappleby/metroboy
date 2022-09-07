#include "AppLib/Audio.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Log.h"

#include <list>
#include <mutex>
#include <condition_variable>
#include <string.h>
#include <semaphore>
#include <atomic>
#include <math.h>

#include "SDL/include/SDL.h"

SDL_AudioDeviceID dev;

constexpr uint16_t samples_per_channel = 256;
constexpr uint16_t samples_per_buffer = samples_per_channel * 2;

sample_t spu_ring_buffer[samples_per_buffer];
uint16_t spu_ring_cursor = 0;

bool play_audio = true;
//bool play_audio = false;

//const float master_volume = 100;
const float master_volume = 8000;

//-----------------------------------------------------------------------------

struct AudioQueue {
  AudioQueue() : queue(), mut(), cv(), closed(false) {
  }

  std::list<sample_t*> queue;
  std::mutex mut;
  std::condition_variable cv;
  bool closed;

  sample_t* get() {
    std::unique_lock<std::mutex> lock(mut);
    cv.wait(lock, [&] { return closed || !queue.empty(); });

    if (closed || queue.empty()) {
      return nullptr;
    }
    else {
      sample_t* buf = queue.front();
      queue.pop_front();
      return buf;
    }
  }

  void put(sample_t* buf) {
    std::unique_lock<std::mutex> lock(mut);
    queue.push_back(buf);
    cv.notify_one();
  }

  void close() {
    std::unique_lock<std::mutex> lock(mut);
    closed = true;
    // FIXME is this ok? how do we want to handle if the sdl buf is in the queue?
    //for (auto a : queue) delete [] a;
    queue.clear();
    cv.notify_all();
  }

  int count() {
    std::unique_lock<std::mutex> lock(mut);
    return (int)queue.size();
  }
};

AudioQueue audio_queue_out;
AudioQueue audio_queue_in;

int audio_queue_size() {
  return audio_queue_out.count();
}

//-----------------------------------------------------------------------------

struct BiquadLP {
  double b0, b1, b2, a1, a2;
  double z1, z2;

  BiquadLP(double Fc) {
    double Q = sqrt(2.0) / 2.0;
    double s = sin(2.0 * M_PI * Fc);
    double c = cos(2.0 * M_PI * Fc);
    double a = s / (2.0 * Q);

    b0 = (1 - c) / 2.0;
    b1 = 1 - c;
    b2 = (1 - c) / 2.0;
    double a0 = 1 + a;
    a1 = -2.0 * c;
    a2 = 1 - a;

    b2 /= a0;
    b1 /= a0;
    b0 /= a0;
    a2 /= a0;
    a1 /= a0;
    a0 /= a0;
  }

  double operator()(double in) {
    double out = in * b0 + z1;
    z1 = in * b1 + z2 - a1 * out;
    z2 = in * b2 - a2 * out;
    return out;
  }
};

//-----------------------------------------------------------------------------

void audio_callback(void* userdata, Uint8* stream, int len) {
  if (!dev) return;
  sample_t* dst = (sample_t*)stream;
  audio_queue_in.put(dst);
  sample_t* src = audio_queue_out.get();
  if (src && (dst != src)) printf("??? ");
}

//-------------------------------------

void audio_init() {
  LOG_G("audio_init()\n");

  static SDL_AudioSpec want, have;
  memset(&want, 0, sizeof(want));
  want.freq = output_hz;
  want.format = AUDIO_S16SYS;
  want.channels = 2;
  want.samples = samples_per_channel;
  want.callback = audio_callback;
  want.userdata = nullptr;

  if (play_audio) {
    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
  }

  memset(spu_ring_buffer, 0, sizeof(spu_ring_buffer));
  spu_ring_cursor = 0;

  if (dev) SDL_PauseAudioDevice(dev, 0);
}

//-------------------------------------

void audio_stop() {
  LOG_G("audio_stop()\n");
  audio_queue_in.close();
  audio_queue_out.close();
  if (dev) SDL_CloseAudioDevice(dev);
}

//-------------------------------------

void audio_post(sample_t in_l_i, sample_t in_r_i) {
  static uint32_t in_l_accum = 0;
  static uint32_t in_r_accum = 0;
  static int sample_count = 0;

  in_l_accum += in_l_i;
  in_r_accum += in_r_i;
  sample_count++;

  static int32_t time_bucket = 0;
  time_bucket += output_hz;

  if (time_bucket >= input_hz) {
    time_bucket -= input_hz;

    double out_l = 0;
    double out_r = 0;

    if (sample_count) {
      out_l = double(in_l_accum) / double(sample_count);
      out_r = double(in_r_accum) / double(sample_count);
      in_l_accum = 0;
      in_r_accum = 0;
      sample_count = 0;
    }

    // high pass to remove dc bias
    const double a = 0.99633663348f;
    static double highpass_l = 0;
    static double highpass_r = 0;
    highpass_l = highpass_l * a + out_l * (1 - a);
    highpass_r = highpass_r * a + out_r * (1 - a);
    out_l -= highpass_l;
    out_r -= highpass_r;

    // adjustable low pass to remove aliasing

    static BiquadLP lo_l1(16000.0 / 48000.0);
    static BiquadLP lo_r1(16000.0 / 48000.0);

    out_l = lo_l1(out_l);
    out_r = lo_r1(out_r);

    /*
    static BiquadLP lo_l1(16000.0 / 48000.0);
    static BiquadLP lo_l2(16000.0 / 48000.0);
    static BiquadLP lo_l3(16000.0 / 48000.0);

    static BiquadLP lo_r1(16000.0 / 48000.0);
    static BiquadLP lo_r2(16000.0 / 48000.0);
    static BiquadLP lo_r3(16000.0 / 48000.0);

    out_l = lo_l3(lo_l2(lo_l1(out_l)));
    out_r = lo_r3(lo_r2(lo_r1(out_r)));
    */

    spu_ring_buffer[spu_ring_cursor++] = int16_t(out_l);
    spu_ring_buffer[spu_ring_cursor++] = int16_t(out_r);

    if (spu_ring_cursor == samples_per_buffer) {
      if (dev) {
        auto temp = audio_queue_in.get();
        for (int i = 0; i < samples_per_buffer; i++) {
          // Audio range is 0-480, or -240-240 after removing DC bias. Low-pass filtering can cause
          // overshoot due to ringing, so account for that when rescaling so we don't clip.
          constexpr double gibbs = 1.089489872236;
          constexpr double max_possible = 240 * gibbs;

          temp[i] = (sample_t)remap_clamp<double>(spu_ring_buffer[i], -max_possible, max_possible, -master_volume, master_volume);
        }
        audio_queue_out.put(temp);
      }
      spu_ring_cursor = 0;
    }
  }
}

//-----------------------------------------------------------------------------
