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
// Silly template to convert size-in-bits to a primitive type.

template<int N> struct bitsize_to_basetype {};
#define DECLARE_SIZE(T, N) template<> struct bitsize_to_basetype<N> { typedef T basetype; };

DECLARE_SIZE(uint8_t, 1);
DECLARE_SIZE(uint8_t, 2);
DECLARE_SIZE(uint8_t, 3);
DECLARE_SIZE(uint8_t, 4);
DECLARE_SIZE(uint8_t, 5);
DECLARE_SIZE(uint8_t, 6);
DECLARE_SIZE(uint8_t, 7);
DECLARE_SIZE(uint8_t, 8);

DECLARE_SIZE(uint16_t, 9);
DECLARE_SIZE(uint16_t, 10);
DECLARE_SIZE(uint16_t, 11);
DECLARE_SIZE(uint16_t, 12);
DECLARE_SIZE(uint16_t, 13);
DECLARE_SIZE(uint16_t, 14);
DECLARE_SIZE(uint16_t, 15);
DECLARE_SIZE(uint16_t, 16);

DECLARE_SIZE(uint32_t, 17);
DECLARE_SIZE(uint32_t, 18);
DECLARE_SIZE(uint32_t, 19);
DECLARE_SIZE(uint32_t, 20);
DECLARE_SIZE(uint32_t, 21);
DECLARE_SIZE(uint32_t, 22);
DECLARE_SIZE(uint32_t, 23);
DECLARE_SIZE(uint32_t, 24);
DECLARE_SIZE(uint32_t, 25);
DECLARE_SIZE(uint32_t, 26);
DECLARE_SIZE(uint32_t, 27);
DECLARE_SIZE(uint32_t, 28);
DECLARE_SIZE(uint32_t, 29);
DECLARE_SIZE(uint32_t, 30);
DECLARE_SIZE(uint32_t, 31);
DECLARE_SIZE(uint32_t, 32);

DECLARE_SIZE(uint64_t, 33);
DECLARE_SIZE(uint64_t, 34);
DECLARE_SIZE(uint64_t, 35);
DECLARE_SIZE(uint64_t, 36);
DECLARE_SIZE(uint64_t, 37);
DECLARE_SIZE(uint64_t, 38);
DECLARE_SIZE(uint64_t, 39);
DECLARE_SIZE(uint64_t, 40);
DECLARE_SIZE(uint64_t, 41);
DECLARE_SIZE(uint64_t, 42);
DECLARE_SIZE(uint64_t, 43);
DECLARE_SIZE(uint64_t, 44);
DECLARE_SIZE(uint64_t, 45);
DECLARE_SIZE(uint64_t, 46);
DECLARE_SIZE(uint64_t, 47);
DECLARE_SIZE(uint64_t, 48);
DECLARE_SIZE(uint64_t, 49);
DECLARE_SIZE(uint64_t, 50);
DECLARE_SIZE(uint64_t, 51);
DECLARE_SIZE(uint64_t, 52);
DECLARE_SIZE(uint64_t, 53);
DECLARE_SIZE(uint64_t, 54);
DECLARE_SIZE(uint64_t, 55);
DECLARE_SIZE(uint64_t, 56);
DECLARE_SIZE(uint64_t, 57);
DECLARE_SIZE(uint64_t, 58);
DECLARE_SIZE(uint64_t, 59);
DECLARE_SIZE(uint64_t, 60);
DECLARE_SIZE(uint64_t, 61);
DECLARE_SIZE(uint64_t, 62);
DECLARE_SIZE(uint64_t, 63);
DECLARE_SIZE(uint64_t, 64);

//----------------------------------------
// Statically sized chunk of bits.

template<int N>
struct logic {
  typedef typename bitsize_to_basetype<N>::basetype basetype;
  static const int width = N;
  static const uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> (64 - N);
  
  logic() { x = 0; }
  logic(const basetype& y) { x = y; }

  template<int M>
  logic(logic<M> b) {
    static_assert(M <= N);
    x = b.x;
  }

  template<int M>
  logic<M> truncate() const {
    static_assert(M < N);
    return (typename logic<M>::basetype)(x);
  }

  operator basetype() const { return x; }

  basetype x : N;
};

typedef logic<1> bit;

//----------------------------------------
// BitExtract helper methods

template<int N>
inline logic<N> bx(uint64_t a) {
  return (typename logic<N>::basetype)(a);
}

template<int N>
inline logic<N> bx(uint64_t a, int e) {
  int w = N;
  return (typename logic<N>::basetype)((a >> e) & ((1 << w) - 1));
}

template<int N>
inline logic<N> bx(uint64_t a, int b, int e) {
  int w = (b - e + 1);
  return (typename logic<N>::basetype)((a >> e) & ((1 << w) - 1));
}

template<int N, int M>
inline logic<N> bx(logic<M> a) {
  static_assert(N <= M);
  return (typename logic<N>::basetype)(a);
}

template<int N, int M>
inline logic<N> bx(logic<M> a, int e) {
  static_assert(N <= M);
  int w = N;
  return (typename logic<N>::basetype)((a.x >> e) & ((1 << w) - 1));
}

template<int N, int M>
inline logic<N> bx(logic<M> a, int b, int e) {
  static_assert(N <= M);
  int w = (b - e + 1);
  return (typename logic<N>::basetype)((a.x >> e) & ((1 << w) - 1));
}

/*
template<int M>
inline logic<5> bx5(logic<M> a, int b, int e) { return bx<5>(a, b, e); }
*/

//----------------------------------------
// Concatenate any number of logic<>s into one logic<>.

template<int NA, int NB>
inline logic<NA + NB> cat(logic<NA> a, logic<NB> b) {
  return (a.x << NB) | b.x;
}

template<int N, typename... Args>
inline auto cat(logic<N> a, Args... args) -> logic<N + decltype(cat(args...))::width> {
  return cat(a, cat(args...));
}

//----------------------------------------
// Duplicate a logic<>
//
// logic<3> boop = 0b101;
// logic<9> moop = dup<3>(boop);

template<int D>
struct duper {
  template<int N>
  static logic<N* D> dup(logic<N> a) {
    return cat(a, duper<D-1>::dup(a));
  }
};

template<>
struct duper<1> {
  template<int N>
  static logic<N> dup(logic<N> a) { return a; }
};

template<int D, int N>
logic<D*N> dup(logic<N> a) { return duper<D>::dup(a); }

//----------------------------------------
// Dynamically sized chunk of bits, where N <= 64.

struct slice {
  slice(uint64_t _x, int _w) : x(_x), w(_w) {}
  operator uint64_t() const { return x; }
  const uint64_t x;
  const int w;
};

/*
#define B1(x)       slice((x >> 0) & 0b00000001, 1)
#define B2(x)       slice((x >> 0) & 0b00000011, 2)
#define B3(x)       slice((x >> 0) & 0b00000111, 3)
#define B4(x)       slice((x >> 0) & 0b00001111, 4)
#define B5(x)       slice((x >> 0) & 0b00011111, 5)
#define B6(x)       slice((x >> 0) & 0b00111111, 6)
#define B7(x)       slice((x >> 0) & 0b01111111, 7)
#define B8(x)       slice((x >> 0) & 0b11111111, 8)
#define BX(x, w)    slice((x >> 0) & (1 << w) - 1, w)

#define S1(x, e)    slice((x >> e) & 0b00000001, 1)
#define S2(x, e)    slice((x >> e) & 0b00000011, 2)
#define S3(x, e)    slice((x >> e) & 0b00000111, 3)
#define S4(x, e)    slice((x >> e) & 0b00001111, 4)
#define S5(x, e)    slice((x >> e) & 0b00011111, 5)
#define S6(x, e)    slice((x >> e) & 0b00111111, 6)
#define S7(x, e)    slice((x >> e) & 0b01111111, 7)
#define S8(x, e)    slice((x >> e) & 0b11111111, 8)
#define SX(x, b, e) slice((x >> e) & (1 << (b - e + 1)) - 1, b - e + 1)
*/

//----------------------------------------
// Concatenate any number of slices into one slice.

inline slice cat(slice a, slice b) { return { (a.x << b.w) | b.x, a.w + b.w }; }

template<typename... Args>
inline slice cat(slice a, Args... args) { return cat(a, cat(args...)); }

//----------------------------------------
// 'end' is INCLUSIVE

void readmemh(const char* path, logic<8>* mem, int begin, int end);