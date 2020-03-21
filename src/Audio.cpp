#include "Audio.h"

#include "Constants.h"

#include <list>
#include <mutex>
#include <condition_variable>

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#define AVERAGE
#define HIGHPASS
#define LOWPASS

SDL_AudioDeviceID dev;

struct AudioQueue {
  AudioQueue() : queue(), mux(), cv(), closed(false) {
  }

  std::list<sample_t*> queue;
  std::mutex mux;
  std::condition_variable cv;
  bool closed;

  sample_t* get() {
    if (!dev) return nullptr;

    std::unique_lock<std::mutex> lock(mux);
    cv.wait(lock, [&] { return closed || !queue.empty(); });

    printf("get: queue size %zd\n", queue.size());

    if (queue.empty()) {
      return nullptr;
    }
    else {
      sample_t* buf = queue.front();
      queue.pop_front();
      return buf;
    }
  }

  void put(sample_t* buf) {
    if (!dev) return;

    std::unique_lock<std::mutex> lock(mux);

    printf("put: queue size %zd\n", queue.size());

    queue.push_back(buf);
    cv.notify_one();
  }

  void close() {
    closed = true;
    cv.notify_all();
  }
};


AudioQueue audio_queue_out;
AudioQueue audio_queue_in;

uint16_t spu_write_cursor = 0;
sample_t* spu_buffer = nullptr;

//-----------------------------------------------------------------------------

void audio_callback(void* /*userdata*/, Uint8* stream, int len) {
  if (!dev) return;

  sample_t* buf = audio_queue_out.get();
  if (buf) {
    memcpy(stream, buf, len);
    audio_queue_in.put(buf);
  }
}

//-------------------------------------

void audio_init() {
  static SDL_AudioSpec want, have;
  memset(&want, 0, sizeof(want));
  want.freq = output_hz;
  want.format = AUDIO_S16SYS;
  want.channels = 2;
  want.samples = samples_per_frame;
  want.callback = audio_callback;
  want.userdata = nullptr;

  dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

  if (dev) {
    SDL_PauseAudioDevice(dev, 0);

    audio_queue_in.put(new sample_t[samples_per_frame * 2]);
    audio_queue_in.put(new sample_t[samples_per_frame * 2]);
  }
  else {
    printf("Could not open audio device!\n");
  }
}

//-------------------------------------

void audio_stop() {
  if (!dev) return;

  audio_queue_out.close();
  SDL_CloseAudioDevice(dev);
}

//-------------------------------------

void audio_begin() {
  if (!dev) return;

  spu_buffer = audio_queue_in.get();
  spu_write_cursor = 0;
  memset(spu_buffer, 0, samples_per_frame * 2 * sizeof(sample_t));
}

//-------------------------------------

void audio_post(sample_t in_l_i, sample_t in_r_i) {
  if (!dev) return;

  static uint32_t in_l_accum = 0;
  static uint32_t in_r_accum = 0;
  static int sample_count = 0;

  in_l_accum += in_l_i;
  in_r_accum += in_r_i;
  sample_count++;

  static int32_t accum = 0;
  accum += output_hz;

  if (accum >= input_hz) {
    accum -= input_hz;

    float out_l = 0;
    float out_r = 0;

#ifdef AVERAGE
    if (sample_count) {
      // Input samples are in the range [0,480]
      // - 4 channels * 15 max intensity * 8 global volume.
      out_l = float(in_l_accum) / float(sample_count * 240);
      out_r = float(in_r_accum) / float(sample_count * 240);
      in_l_accum = 0;
      in_r_accum = 0;
      sample_count = 0;
    }
#else
    out_l = float(in_l_i) / float(240);
    out_r = float(in_r_i) / float(240);
#endif

#ifdef HIGHPASS
    // high pass to remove dc bias
    const float a = 0.99633663348f;
    static float highpass_l = 0;
    static float highpass_r = 0;
    highpass_l = highpass_l * a + out_l * (1 - a);
    highpass_r = highpass_r * a + out_r * (1 - a);
    out_l -= highpass_l;
    out_r -= highpass_r;
#endif

#ifdef LOWPASS
    static float lowpass_l = 0;
    static float lowpass_r = 0;
    lowpass_l = lowpass_l * 0.4f + out_l * 0.6f;
    lowpass_r = lowpass_r * 0.4f + out_r * 0.6f;
    out_l = lowpass_l;
    out_r = lowpass_r;
#endif

    spu_buffer[spu_write_cursor++] = int16_t(out_l * 16383);
    spu_buffer[spu_write_cursor++] = int16_t(out_r * 16383);
  }
}

//-------------------------------------

void audio_end() {
  if (!dev) return;

  audio_queue_out.put(spu_buffer);
  spu_buffer = nullptr;
  spu_write_cursor = 0;
}

//-----------------------------------------------------------------------------
