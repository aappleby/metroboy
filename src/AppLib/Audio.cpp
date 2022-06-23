#include "AppLib/Audio.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Log.h"

#include <list>
#include <mutex>
#include <condition_variable>
#include <string.h>
#include <semaphore>
#include <atomic>

#include "SDL/include/SDL.h"

#define AVERAGE
#define HIGHPASS
#define LOWPASS

std::binary_semaphore audio_full{0};
std::binary_semaphore audio_empty{0};

SDL_AudioDeviceID dev;

uint16_t spu_write_cursor = 0;
sample_t spu_buffer[samples_per_frame * 2];

static std::atomic<bool> quit;

//-----------------------------------------------------------------------------

void audio_callback(void* /*userdata*/, Uint8* stream, int len) {
  if (quit) return;
  audio_full.acquire();
  memcpy(stream, spu_buffer, len);
  audio_empty.release();
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
  }
  else {
    LOG_R("Could not open audio device!\n");
  }
}

//-------------------------------------

void audio_stop() {
  if (!dev) return;
  quit = true;
  audio_full.release();
  SDL_CloseAudioDevice(dev);
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

  static int32_t time_bucket = 0;
  time_bucket += output_hz;

  if (time_bucket >= input_hz) {
    time_bucket -= input_hz;

    float out_l = 0;
    float out_r = 0;

    if (sample_count) {
      // Input samples are in the range [0,480]
      // - 4 channels * 15 max intensity * 8 global volume.
      out_l = float(in_l_accum) / float(sample_count * 240);
      out_r = float(in_r_accum) / float(sample_count * 240);
      in_l_accum = 0;
      in_r_accum = 0;
      sample_count = 0;
    }

    // high pass to remove dc bias
    const float a = 0.99633663348f;
    static float highpass_l = 0;
    static float highpass_r = 0;
    highpass_l = highpass_l * a + out_l * (1 - a);
    highpass_r = highpass_r * a + out_r * (1 - a);
    out_l -= highpass_l;
    out_r -= highpass_r;

    static float lowpass_l = 0;
    static float lowpass_r = 0;
    lowpass_l = lowpass_l * 0.4f + out_l * 0.6f;
    lowpass_r = lowpass_r * 0.4f + out_r * 0.6f;
    out_l = lowpass_l;
    out_r = lowpass_r;

    spu_buffer[spu_write_cursor++] = int16_t(out_l * 16383);
    spu_buffer[spu_write_cursor++] = int16_t(out_r * 16383);

    if (spu_write_cursor == samples_per_frame * 2) {
      audio_full.release();
      spu_write_cursor = 0;
      audio_empty.acquire();
    }
  }
}

//-----------------------------------------------------------------------------
