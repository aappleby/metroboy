#pragma once

#include <stdint.h>
#include "Logic.h"
#include <string>

//----------------------------------------
// Trivial support for Verilog's "+foo=bar" test arg syntax.
// This only works for $value$plusargs("NAME=%s", s);

void metron_init(int argc, char** argv);
void metron_reset();
void value_plusargs(const char* fmt, std::string& out);

//----------------------------------------
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

void parse_hex(const char* src_filename, void* dst_data, int dst_size);

//----------------------------------------
// 'end' is INCLUSIVE

void readmemh(const char* path, void* mem, int begin, int end);
void readmemh(const std::string& path, void* mem, int begin, int end);
void readmemh(const char* path, void* mem);
void readmemh(const std::string& path, void* mem);

//----------------------------------------

int display(const char* fmt, ...);
int write(const char* fmt, ...);

//----------------------------------------
// Verilog's signed right shift doesn't work quite the same as C++'s, so we
// patch around it here.

template<int WIDTH>
inline logic<WIDTH> sra(logic<WIDTH> x, int s) {
  return x.as_signed() >> s;
}

