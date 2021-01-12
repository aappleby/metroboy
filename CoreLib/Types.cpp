#include "CoreLib/Types.h"

#include <string>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

uint32_t mix(uint32_t h) {
  return _byteswap_ulong(h * 0xed558ccd);
}

uint64_t mix(uint64_t h) {
  return _byteswap_uint64(h * 0xff51afd7ed558ccd);
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
  uint64_t ret = 0;
  QueryPerformanceCounter((LARGE_INTEGER*)&ret);
  return ret;
}();

static const double inv_perf_freq = []() {
  uint64_t ret = 0;
  QueryPerformanceFrequency((LARGE_INTEGER*)&ret);
  return 1.0 / double(ret);
}();

double timestamp() {
  uint64_t ret = 0;
  QueryPerformanceCounter((LARGE_INTEGER*)&ret);
  return double(ret - app_start) * inv_perf_freq;
}

//-----------------------------------------------------------------------------

