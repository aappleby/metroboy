#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#pragma warning(disable:4201)


inline void r(int i, int y, const char* name, int v) {
  if (i == 0) {
    printf("\033[%d;%dH%s", y, 0, name);
  }

  printf("\033[%d;%dH%c", y, i + 15 + (i/8), v ? 219 : 176);
}

inline void print_at(int x, int y, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\033[%d;%dH", y, x);
  vprintf(format, args);
  va_end(args);
}

template<typename T> const T and(const T a, const T b) { return a & b; }
template<typename T> const T or (const T a, const T b) { return a | b; }
template<typename T> const T xor(const T a, const T b) { return a ^ b; }

template<typename T, typename... Args> const T and (const T first, Args... args) { return first & and(args...); }
template<typename T, typename... Args> const T or  (const T first, Args... args) { return first | or(args...); }
template<typename T, typename... Args> const T xor (const T first, Args... args) { return first ^ xor(args...); }
template<typename T, typename... Args> const T nor (const T first, Args... args) { return !or(first, args...); }
template<typename T, typename... Args> const T nand(const T first, Args... args) { return !and(first, args...); }

inline const bool not(bool a) { return !a; }

template<typename T>
inline const T not(T a) { return ~a; }

// Six-rung mux cells are _non_inverting_
// m = 1 selects input _ZERO_
template<typename T>
inline const T mux2 (T a, T b, bool m) {
  return m ? a : b;
}

// Five-rung mux cells are _inverting_
// m = 1 selects input _ZERO_
template<typename T>
inline const T mux2n (T a, T b, bool m) {
  return m ? not(a) : not(b);
}

// this is def or
template<typename T>
inline T unk2 (T a, T b) { return a | b; }

// probably not right...
template<typename T>
inline T unk3 (T a, T b, T c) { return (a & b) | c; }

// definitely not right...
template<typename T>
inline T unk1 (T a, T b, T c, T d) { return a ^ b ^ c ^ d; }

template<typename T>
inline T amux2(T a0, bool b0, T a1, bool b1) {
  return (b0 ? a0 : 0) | (b1 ? a1 : 0);
}

template<typename T>
inline T amux3(T a0, bool b0, T a1, bool b1, T a2, bool b2) {
  return (b0 ? a0 : 0) | (b1 ? a1 : 0) | (b2 ? a2 : 0);
}

template<typename T>
inline T amux4(T a0, bool b0, T a1, bool b1, T a2, bool b2, T a3, bool b3) {
  return (b0 ? a0 : 0) | (b1 ? a1 : 0) | (b2 ? a2 : 0) | (b3 ? a3 : 0);
}

template<typename T>
inline T amux6(T a0, bool b0, T a1, bool b1, T a2, bool b2, T a3, bool b3, bool a4, bool b4, bool a5, bool b5) {
  return (b0 ? a0 : 0) | (b1 ? a1 : 0) | (b2 ? a2 : 0) | (b3 ? a3 : 0) | (b4 ? a4 : 0) | (b5 ? a5 : 0);
}

//-----------------------------------------------------------------------------

typedef const bool wire;

inline wire add_c(wire a, wire b, wire c) {
  return (a + b + c) & 2;
}

inline wire add_s(wire a, wire b, wire c) {
  return (a + b + c) & 1;
}

//-----------------------------------------------------------------------------

inline const uint32_t pack(bool b) {
  return (uint32_t)b;
}

template<typename... Args> const uint32_t pack(const bool first, Args... args) {
  return (pack(args...) << 1) | (uint32_t)first;
}

inline void unpack(uint32_t x, bool& b) {
  b = (bool)x;
}

template<typename... Args> void unpack(uint32_t x, bool& first, Args&... args) {
  first = bool(x & 1);
  unpack(x >> 1, args...);
}

//-----------------------------------------------------------------------------

bool latch_pos (bool clk, bool d1, bool d2);

bool tock_pos  (bool clk1, bool clk2, bool rst,  bool d1, bool d2);
bool tock_duo  (bool clk1, bool clk2, bool rst,  bool d1, bool d2);
bool count_pos (bool clk1, bool clk2, bool load, bool d1, bool d2);
bool srtock_pos(bool clk1, bool clk2, bool set, bool rst, bool d1, bool d2);

//-----------------------------------------------------------------------------

struct Resets;
struct AddressDecoder;
struct Clocks;
struct CpuBus;
struct Debug;
struct DmaBus;
struct ExtBus;
struct MemBus;
struct APU;
struct Background;
struct Joypad;
struct LCD;
struct OAM;
struct PPU;
struct Serial;
struct Sprites;
struct System;
struct Timer;
struct Vram;
struct Window;

