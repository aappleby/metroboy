#pragma once
#include "Types.h"

namespace Schematics {

//typedef const bool wire;
typedef bool wire;

//-----------------------------------------------------------------------------

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

inline bool not (bool a) { return !a; }
inline bool and (bool a, bool b) { return a & b; }
inline bool and (bool a, bool b, bool c) { return  (a & b & c); }
inline bool and (bool a, bool b, bool c, bool d) { return  (a & b & c & d); }
inline bool and (bool a, bool b, bool c, bool d, bool e, bool f) { return  (a & b & c & d & e & f); }
inline bool or  (bool a, bool b) { return a | b; }
inline bool or  (bool a, bool b, bool c) { return  (a | b | c); }
inline bool or  (bool a, bool b, bool c, bool d) { return  (a | b | c | d); }
inline bool or  (bool a, bool b, bool c, bool d, bool e) { return  (a | b | c | d | e); }
inline bool xor (bool a, bool b) { return a ^ b; }
inline bool xnor(bool a, bool b) { return a == b; }
inline bool nor (bool a, bool b) { return !(a | b); }
inline bool nor (bool a, bool b, bool c) { return !(a | b | c); }
inline bool nor (bool a, bool b, bool c, bool d) { return !(a | b | c | d); }
inline bool nor (bool a, bool b, bool c, bool d, bool e) { return !(a | b | c | d | e); }
inline bool nor (bool a, bool b, bool c, bool d, bool e, bool f) { return !(a | b | c | d | e | f); }
inline bool nor (bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h) { return !(a | b | c | d | e | f | g | h); }
inline bool nand(bool a, bool b) { return !(a & b); }
inline bool nand(bool a, bool b, bool c) { return !(a & b & c); }
inline bool nand(bool a, bool b, bool c, bool d) { return !(a & b & c & d); }
inline bool nand(bool a, bool b, bool c, bool d, bool e) { return !(a & b & c & d & e); }
inline bool nand(bool a, bool b, bool c, bool d, bool e, bool f) { return !(a & b & c & d & e & f); }
inline bool nand(bool a, bool b, bool c, bool d, bool e, bool f, bool g) { return !(a & b & c & d & e & f & g); }

/*
template<typename... Args> const bool and (const bool first, Args... args) { return first & and(args...); }
template<typename... Args> const bool or  (const bool first, Args... args) { return first | or(args...); }
template<typename... Args> const bool xor (const bool first, Args... args) { return first ^ xor(args...); }
template<typename... Args> const bool nor (const bool first, Args... args) { return !or(first, args...); }
template<typename... Args> const bool nand(const bool first, Args... args) { return !and(first, args...); }
*/

// Six-rung mux cells are _non_inverting_
// m = 1 selects input _ZERO_
inline const bool mux2 (bool a, bool b, bool m) {
  return m ? a : b;
}

// Five-rung mux cells are _inverting_
// m = 1 selects input _ZERO_
inline const bool mux2n (bool a, bool b, bool m) {
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


inline bool amux2(bool a0, bool b0, bool a1, bool b1) {
  return (b0 ? a0 : 0) | (b1 ? a1 : 0);
}

template<typename T>
inline T amux3(T a0, bool b0, T a1, bool b1, T a2, bool b2) {
  return (b0 ? a0 : 0) | (b1 ? a1 : 0) | (b2 ? a2 : 0);
}

inline bool amux4(bool a0, bool b0, bool a1, bool b1, bool a2, bool b2, bool a3, bool b3) {
  return (b0 ? a0 : 0) | (b1 ? a1 : 0) | (b2 ? a2 : 0) | (b3 ? a3 : 0);
}

template<typename T>
inline T amux6(T a0, bool b0, T a1, bool b1, T a2, bool b2, T a3, bool b3, bool a4, bool b4, bool a5, bool b5) {
  return (b0 ? a0 : 0) | (b1 ? a1 : 0) | (b2 ? a2 : 0) | (b3 ? a3 : 0) | (b4 ? a4 : 0) | (b5 ? a5 : 0);
}

inline bool weird1(bool a, bool b) {
  return (a | b);
}

inline bool weird2(bool a, bool b) {
  return (a | b);
}

//-----------------------------------------------------------------------------

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

};