#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <cstdlib>
#include <cstring>

#pragma warning(disable:4996)

constexpr int clog2(uint32_t x) {
  for (int i = 31; i >= 0; i--) if (x & (1 << i)) return i + 1;
  return 0;
}

inline void parse_hex(const char* src_filename, uint8_t* dst_data, int dst_size) {

  FILE* f = fopen(src_filename, "rb");
  fseek(f, 0, SEEK_END);
  int src_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  uint8_t* src_data = (uint8_t*)malloc(src_size);
  fread(src_data, 1, src_size, f);

  memset(dst_data, 0, dst_size);

  char buf_name[512];
  strcpy(buf_name, src_filename);
  for (char* cursor = buf_name; *cursor; cursor++) {
    if(*cursor == '.') *cursor = '_';
  }

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
        dst_data[dst_cursor] |= d << 4;
        hi_lo = 0;
      } else {
        dst_data[dst_cursor] |= d << 0;
        dst_cursor++;
        hi_lo = 1;
      }
    }

    if (dst_cursor == dst_size) break;
  }

  printf("uint8_t %s[512] = {\n", buf_name);
  dst_cursor = 0;
  for (int y = 0; y < dst_size / 16; y++) {
    printf("  ");
    for (int x = 0; x < 16; x++) {
      printf("%02x, ", dst_data[dst_cursor++]);
    }
    printf("\n");
  }
  printf("};\n");

  fclose(f);
  free(src_data);
}

template<int N = 1>
struct logic {
  typedef uint32_t base_type;
  static constexpr int width = N;
  static constexpr base_type mask = (1 << width) - 1;

  logic() : x(0) {}
  logic(base_type x) : x(x & mask) {}
  operator base_type() const { return x; }
  base_type x;
};

typedef logic<1> bit;

// the specializations just save ram, but not sure if it's worth it...

/*
template<>
struct bits<1> {
  static constexpr int width = 1;
  typedef uint8_t base_type;
  static constexpr base_type mask = (1 << width) - 1;
  bits() : x(0) {}
  bits(base_type x) : x(x& mask) {}
  operator base_type() const { return x; }
  base_type x;
};

template<>
struct bits<8> {
  static constexpr int width = 8;
  typedef uint16_t base_type;
  static constexpr base_type mask = (1 << width) - 1;
  bits() : x(0) {}
  bits(base_type x) : x(x& mask) {}
  operator base_type() const { return x; }
  base_type x;
};

template<>
struct bits<9> {
  static constexpr int width = 9;
  typedef uint16_t base_type;
  static constexpr base_type mask = (1 << width) - 1;
  bits() : x(0) {}
  bits(base_type x) : x(x& mask) {}
  operator base_type() const { return x; }
  base_type x;
};
*/