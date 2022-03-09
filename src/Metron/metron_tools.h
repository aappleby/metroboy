#pragma once

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>

#include "Logic.h"
//#include "../CoreLib/Log.h"

#pragma warning(disable:4996)

// "Magic" constant that gets translated to 'x' in Verilog
static const uint64_t DONTCARE = 0;

constexpr int clog2(uint64_t x) {
  if (x == 0) return 0;
  x--;
  for (int i = 63; i >= 0; i--) if (x & (uint64_t(1) << i)) return i + 1;
  return 1;
}

constexpr uint64_t pow2(int x) {
  return (1ull << x);
}

static_assert(clog2(0) == 0);
static_assert(clog2(1) == 1);
static_assert(clog2(2) == 1);
static_assert(clog2(3) == 2);
static_assert(clog2(255) == 8);
static_assert(clog2(256) == 8);
static_assert(clog2(257) == 9);

inline void parse_hex(const char* src_filename, void* dst_data, int dst_size) {

  FILE* f = fopen(src_filename, "rb");
  fseek(f, 0, SEEK_END);
  int src_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  uint8_t* src_data = (uint8_t*)malloc(src_size);
  size_t read = fread(src_data, 1, src_size, f);

  memset(dst_data, 0, dst_size);
  uint8_t* dst_bytes = (uint8_t*)dst_data;

  int dst_cursor = 0;
  int hi_lo = 1;
  for (int src_cursor = 0; src_cursor < src_size; src_cursor++) {
    auto c = src_data[src_cursor];
    int d = -1;
    if (c >= '0' && c <= '9') d = c - '0';
    if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
    if (c >= 'A' && c <= 'F') d = c - 'A' + 10;

    if (d >= 0) {
      if (hi_lo == 1) {
        dst_bytes[dst_cursor] |= d << 4;
        hi_lo = 0;
      }
      else {
        dst_bytes[dst_cursor] |= d << 0;
        dst_cursor++;
        hi_lo = 1;
      }
    }

    if (dst_cursor == dst_size) break;
  }

  fclose(f);
  free(src_data);
}

/*
void print_hex(const char* buf_name, void* src_data, int src_size) {
  char buf_name[512];
  strcpy(buf_name, src_filename);
  for (char* cursor = buf_name; *cursor; cursor++) {
    if (*cursor == '.') *cursor = '_';
  }

  printf("uint8_t %s[%d] = {\n", buf_name, src_size);
  int dst_cursor = 0;
  for (int y = 0; y < dst_size / 16; y++) {
    printf("  ");
    for (int x = 0; x < 16; x++) {
      printf("%02x, ", dst_data[dst_cursor++]);
    }
    printf("\n");
  }
  printf("};\n");
}
*/

//----------------------------------------
// 'end' is INCLUSIVE

inline void readmemh(const char* path, void* mem, int begin, int end) {
  parse_hex(path, (uint8_t*)mem + begin, end - begin + 1);
}

//----------------------------------------

inline int display(const char* fmt, ...) {
  return 0;
}

inline int write(const char* fmt, ...) {
  char buffer[256];
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(buffer, 256, fmt, args);
  va_end(args);
  log_print(0xFFFFFF, buffer, len);
  return len;
}