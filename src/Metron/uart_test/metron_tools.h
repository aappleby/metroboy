#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <cstdlib>
#include <cstring>

#include "Logic.h"

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

void parse_hex(const char* src_filename, void* dst_data, int dst_size);

//----------------------------------------
// 'end' is INCLUSIVE

void readmemh(const char* path, void* mem, int begin, int end);