#include "CoreLib/Types.h"

#include <string>
#include <stdlib.h>

#include "SDL/include/SDL.h"

#ifdef __GNUC__

void QueryPerformanceCounter(LARGE_INTEGER* result) {
  *result = 1;
}

uint32_t swap(uint32_t x) { return __builtin_bswap32(x); }
uint64_t swap(uint64_t x) { return __builtin_bswap64(x); }


void QueryPerformanceFrequency(LARGE_INTEGER* result) {
  *result = 1;
}

#else

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

uint32_t swap(uint32_t x) { return _byteswap_ulong(x); }
uint64_t swap(uint64_t x) { return _byteswap_uint64(x); }


#endif


uint32_t mix(uint32_t h) {
  return swap(h * 0xed558ccd);
}

uint64_t mix(uint64_t h) {
  return swap(h * 0xff51afd7ed558ccd);
}

uint64_t hash_blob(uint64_t h, void* blob, size_t len) {
  uint8_t* base = (uint8_t*)blob;
  for (auto i = 0; i < len; i++) {
    h = mix(h ^ base[i]);
  }
  return h;
}

uint64_t hash_blob(void* blob, size_t len) {
  return hash_blob(HASH_INIT, blob, len);
}

static const uint64_t app_start = []() {
  uint64_t ret = SDL_GetPerformanceCounter();
  return ret;
}();

static const double inv_perf_freq = []() {
  uint64_t ret = SDL_GetPerformanceFrequency();
  return 1.0 / double(ret);
}();

double timestamp() {
  uint64_t ret = SDL_GetPerformanceCounter();
  return double(ret - app_start) * inv_perf_freq;
}

//-----------------------------------------------------------------------------

