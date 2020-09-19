#include "CoreLib/Types.h"

#include <string>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

uint64_t mix(uint64_t h) {
  return _byteswap_uint64(h * 0xff51afd7ed558ccd);
}

uint64_t hash_states(void* blob, int len) {
  uint8_t* base = (uint8_t*)blob;
  uint64_t h = HASH_INIT;
  for (int i = 0; i < len; i++) {
    h = mix(h ^ (base[i] & 0x0F));
  }
  return h;
}

static const uint64_t app_start = []() {
  uint64_t ret;
  QueryPerformanceCounter((LARGE_INTEGER*)&ret);
  return ret;
}();

static const double inv_perf_freq = []() {
  uint64_t ret;
  QueryPerformanceFrequency((LARGE_INTEGER*)&ret);
  return 1.0 / double(ret);
}();

double timestamp() {
  uint64_t ret;
  QueryPerformanceCounter((LARGE_INTEGER*)&ret);
  return double(ret - app_start) * inv_perf_freq;
}

//-----------------------------------------------------------------------------

/*
template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}
*/

//-----------------------------------------------------------------------------

void dump_req(Dumper& d, const Req& req) {
  d("%04x:%04x %s%s\n", req.addr, req.data,
    req.read  ? "\003R \001" : "- ",
    req.write ? "\002W \001" : "- ");
}

void dump_ack(Dumper& d, const Ack& ack) {
  d("%04x:%04x %s%s\n", ack.addr, ack.data,
    ack.read  ? "\003R \001" : "- ",
    "- ");
}

