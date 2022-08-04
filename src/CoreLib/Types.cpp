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

uint64_t hash_blob(uint64_t h, const void* blob, size_t len) {
  const uint8_t* base = (uint8_t*)blob;
  for (auto i = 0; i < len; i++) {
    h = mix(h ^ base[i]);
  }
  return h;
}

uint64_t hash_blob(const void* blob, size_t len) {
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

void print_field_at(int offset, const FieldInfo* fields) {
  if (fields == nullptr) return;

  const FieldInfo* field = nullptr;
  int min_delta = 1000000;

  for (auto cursor = fields; cursor->name != nullptr; cursor++) {
    int delta = offset - cursor->offset;
    if (delta >= 0 && delta < cursor->size && delta < min_delta) {
      field = cursor;
      min_delta = delta;
    }
  }

  if (field != nullptr) {
    LOG_R("%s + %d", field->name, min_delta);
  }
  else {
    LOG_R("<could not find field at offset %d>", offset);
  }
}

//-----------------------------------------------------------------------------

bool diff_blobs(const void* blob_a, const void* blob_b, size_t size, uint8_t mask, const FieldInfo* fields) {
  bool result = true;

  uint8_t* bytes_a = (uint8_t*)blob_a;
  uint8_t* bytes_b = (uint8_t*)blob_b;

  for (int i = 0; i < size; i++) {
    int byte_a = bytes_a[i] & mask;
    int byte_b = bytes_b[i] & mask;

    if (byte_a != byte_b) {
      LOG_R("MISMATCH @ %5d - ", i);
      print_field_at(i, fields);
      LOG_R(": 0x%02x 0x%02x\n", byte_a, byte_b);
      result = false;
    }
  }

  return result;
}

//-----------------------------------------------------------------------------

