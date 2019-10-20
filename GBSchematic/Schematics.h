#pragma once

#include <stdint.h>
#include "Register.h"

template<typename T> const T and(const T a, const T b) { return a & b; }
template<typename T> const T or (const T a, const T b) { return a & b; }
template<typename T> const T xor(const T a, const T b) { return a & b; }

template<typename T, typename... Args> const T and (const T first, Args... args) { return first & and(args...); }
template<typename T, typename... Args> const T or  (const T first, Args... args) { return first & and(args...); }
template<typename T, typename... Args> const T xor (const T first, Args... args) { return first & and(args...); }
template<typename T, typename... Args> const T nor (const T first, Args... args) { return !or(first, args...); }
template<typename T, typename... Args> const T nand(const T first, Args... args) { return !and(first, args...); }

inline const wire not  (wire a)                   { return !a; }
inline const wire mux2 (wire m, wire a, wire b)   { return m ? a : b; }

// definitely not right...
inline wire unk2 (wire a, wire b)                 { return a ^ b; }
inline wire unk3 (wire a, wire b, wire c)         { return a ^ b ^ c; }
inline wire unk1 (wire a, wire b, wire c, wire d) { return a ^ b ^ c ^ d; }

inline wire add_c(wire a, wire b, wire c) {
  return (a + b + c) & 2;
}

inline wire add_s(wire a, wire b, wire c) {
  return (a + b + c) & 1;
}

inline wire amux2(wire a0, wire b0, wire a1, wire b1) {
  return (a0 & b0) | (a1 & b1);
}

inline wire amux4(wire a0, wire b0, wire a1, wire b1, wire a2, wire b2, wire a3, wire b3) {
  return (a0 & b0) | (a1 & b1) | (a2 & b2) | (a3 & b3);
}

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

