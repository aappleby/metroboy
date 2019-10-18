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

inline uint16_t pack4(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3) {
  return (n0 << 0) | (n1 << 4) | (n2 << 8) | (n3 << 12);
}

inline uint8_t pack(bool d0, bool d1) {
  return (d0 << 0) | (d1 << 1);
}

inline uint8_t pack(bool d0, bool d1, bool d2, bool d3) {
  return (d0 << 0) | (d1 << 1) | (d2 << 2) | (d3 << 3);
}

inline void pack(uint8_t& x, bool d0, bool d1, bool d2, bool d3, bool d4, bool d5, bool d6, bool d7) {
  x = (d0 << 0) | (d1 << 1) | (d2 << 2) | (d3 << 3) | (d4 << 4) | (d5 << 5) | (d6 << 6) | (d7 << 7);
}

inline void unpack4(uint16_t x, uint8_t& n0, uint8_t& n1, uint8_t& n2, uint8_t& n3) {
  n0 = (x >> 0) & 0x0F;
  n1 = (x >> 4) & 0x0F;
  n2 = (x >> 8) & 0x0F;
  n3 = (x >> 12) & 0x0F;
}

inline void unpack(uint8_t x, bool& d0, bool& d1) {
  d0 = x & 0x01;
  d1 = x & 0x02;
}

inline void unpack(uint8_t x, bool& d0, bool& d1, bool& d2, bool& d3) {
  d0 = x & 0x01;
  d1 = x & 0x02;
  d2 = x & 0x04;
  d3 = x & 0x08;
}

inline void unpack(uint8_t x, bool& d0, bool& d1, bool& d2, bool& d3, bool& d4, bool& d5, bool& d6, bool& d7) {
  d0 = x & 0x01;
  d1 = x & 0x02;
  d2 = x & 0x04;
  d3 = x & 0x08;
  d4 = x & 0x10;
  d5 = x & 0x20;
  d6 = x & 0x40;
  d7 = x & 0x80;
}

