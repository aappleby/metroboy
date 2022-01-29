#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <cstdlib>
#include <cstring>

#pragma warning(disable:4996)

constexpr int clog2(uint64_t x) {
  if (x == 0) return 0;
  x--;
  for (int i = 63; i >= 0; i--) if (x & (uint64_t(1) << i)) return i + 1;
  return 1;
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

template<int N> struct logic {};

#define DECLARE_LOGIC(T, N) \
template<> \
struct logic<N> { \
static const int width = N; \
  logic<N>() { *(T*)this = 0; } \
  logic<N>(const T& y) { x =  y; } \
  void operator = (const T& y) { x = y; } \
  operator T() const { return x; } \
  T x : N; \
}

DECLARE_LOGIC(uint8_t, 1);
DECLARE_LOGIC(uint8_t, 2);
DECLARE_LOGIC(uint8_t, 3);
DECLARE_LOGIC(uint8_t, 4);
DECLARE_LOGIC(uint8_t, 5);
DECLARE_LOGIC(uint8_t, 6);
DECLARE_LOGIC(uint8_t, 7);
DECLARE_LOGIC(uint8_t, 8);

DECLARE_LOGIC(uint16_t, 9);
DECLARE_LOGIC(uint16_t, 10);
DECLARE_LOGIC(uint16_t, 11);
DECLARE_LOGIC(uint16_t, 12);
DECLARE_LOGIC(uint16_t, 13);
DECLARE_LOGIC(uint16_t, 14);
DECLARE_LOGIC(uint16_t, 15);
DECLARE_LOGIC(uint16_t, 16);

DECLARE_LOGIC(uint32_t, 17);
DECLARE_LOGIC(uint32_t, 18);
DECLARE_LOGIC(uint32_t, 19);
DECLARE_LOGIC(uint32_t, 20);
DECLARE_LOGIC(uint32_t, 21);
DECLARE_LOGIC(uint32_t, 22);
DECLARE_LOGIC(uint32_t, 23);
DECLARE_LOGIC(uint32_t, 24);
DECLARE_LOGIC(uint32_t, 25);
DECLARE_LOGIC(uint32_t, 26);
DECLARE_LOGIC(uint32_t, 27);
DECLARE_LOGIC(uint32_t, 28);
DECLARE_LOGIC(uint32_t, 29);
DECLARE_LOGIC(uint32_t, 30);
DECLARE_LOGIC(uint32_t, 31);
DECLARE_LOGIC(uint32_t, 32);

DECLARE_LOGIC(uint64_t, 33);
DECLARE_LOGIC(uint64_t, 34);
DECLARE_LOGIC(uint64_t, 35);
DECLARE_LOGIC(uint64_t, 36);
DECLARE_LOGIC(uint64_t, 37);
DECLARE_LOGIC(uint64_t, 38);
DECLARE_LOGIC(uint64_t, 39);
DECLARE_LOGIC(uint64_t, 40);
DECLARE_LOGIC(uint64_t, 41);
DECLARE_LOGIC(uint64_t, 42);
DECLARE_LOGIC(uint64_t, 43);
DECLARE_LOGIC(uint64_t, 44);
DECLARE_LOGIC(uint64_t, 45);
DECLARE_LOGIC(uint64_t, 46);
DECLARE_LOGIC(uint64_t, 47);
DECLARE_LOGIC(uint64_t, 48);
DECLARE_LOGIC(uint64_t, 49);
DECLARE_LOGIC(uint64_t, 50);
DECLARE_LOGIC(uint64_t, 51);
DECLARE_LOGIC(uint64_t, 52);
DECLARE_LOGIC(uint64_t, 53);
DECLARE_LOGIC(uint64_t, 54);
DECLARE_LOGIC(uint64_t, 55);
DECLARE_LOGIC(uint64_t, 56);
DECLARE_LOGIC(uint64_t, 57);
DECLARE_LOGIC(uint64_t, 58);
DECLARE_LOGIC(uint64_t, 59);
DECLARE_LOGIC(uint64_t, 60);
DECLARE_LOGIC(uint64_t, 61);
DECLARE_LOGIC(uint64_t, 62);
DECLARE_LOGIC(uint64_t, 63);
DECLARE_LOGIC(uint64_t, 64);

typedef logic<1> bit;

//----------------------------------------
// 'end' is INCLUSIVE

void readmemh(const char* path, logic<8>* mem, int begin, int end);