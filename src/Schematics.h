#pragma once

#include <stdint.h>

template<typename T> T and(T a, T b) { return a & b; }
template<typename T> T or (T a, T b) { return a & b; }
template<typename T> T xor(T a, T b) { return a & b; }

template<typename T, typename... Args> T and (T first, Args... args) { return first & and(args...); }
template<typename T, typename... Args> T or  (T first, Args... args) { return first & and(args...); }
template<typename T, typename... Args> T xor (T first, Args... args) { return first & and(args...); }
template<typename T, typename... Args> T nor (T first, Args... args) { return !or(first, args...); }
template<typename T, typename... Args> T nand(T first, Args... args) { return !and(first, args...); }

inline bool not  (bool a)                         { return !a; }
inline bool mux2 (bool m, bool a, bool b)         { return m ? a : b; }

// definitely not right...
inline bool unk2 (bool a, bool b)                 { return a ^ b; }
inline bool unk3 (bool a, bool b, bool c)         { return a ^ b ^ c; }
inline bool unk1 (bool a, bool b, bool c, bool d) { return a ^ b ^ c ^ d; }

// maybe not right
inline bool amux2(bool a0, bool b0, bool a1, bool b1) {
  return (a0 & b0) | (a1 & b1);
}

inline bool amux4(bool a0, bool b0, bool a1, bool b1, bool a2, bool b2, bool a3, bool b3) {
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

extern bool CPU_RD;
extern bool CPU_WR;

extern bool A0;
extern bool A1;
extern bool A2;
extern bool A3;
extern bool A4;
extern bool A5;
extern bool A6;
extern bool A7;
extern bool A8;
extern bool A9;
extern bool A10;
extern bool A11;
extern bool A12;
extern bool A13;
extern bool A14;
extern bool A15;

extern bool D0;
extern bool D1;
extern bool D2;
extern bool D3;
extern bool D4;
extern bool D5;
extern bool D6;
extern bool D7;

extern bool D0_A;
extern bool D1_A;
extern bool D2_A;
extern bool D3_A;
extern bool D4_A;
extern bool D5_A;
extern bool D6_A;
extern bool D7_A;

extern bool D0_IN;
extern bool D1_IN;
extern bool D2_IN;
extern bool D3_IN;
extern bool D4_IN;
extern bool D5_IN;
extern bool D6_IN;
extern bool D7_IN;

extern bool MD0_A;
extern bool MD3_A;
extern bool MD4_A;
extern bool MD7_A;
extern bool MD6_A;
extern bool MD1_A;
extern bool MD5_A;
extern bool MD2_A;

extern bool MD0_IN;
extern bool MD1_IN;
extern bool MD2_IN;
extern bool MD3_IN;
extern bool MD4_IN;
extern bool MD5_IN;
extern bool MD6_IN;
extern bool MD7_IN;

extern bool MD0_OUT;
extern bool MD1_OUT;
extern bool MD2_OUT;
extern bool MD3_OUT;
extern bool MD4_OUT;
extern bool MD5_OUT;
extern bool MD6_OUT;
extern bool MD7_OUT;

extern bool MD0;
extern bool MD1;
extern bool MD2;
extern bool MD3;
extern bool MD4;
extern bool MD5;
extern bool MD6;
extern bool MD7;

extern bool MA0;
extern bool MA1;
extern bool MA2;
extern bool MA3;
extern bool MA4;
extern bool MA5;
extern bool MA7;
extern bool MA8;
extern bool MA9;
extern bool MA6;
extern bool MA10;
extern bool MA11;
extern bool MA12;

extern bool OAM_A_D0;
extern bool OAM_A_D1;
extern bool OAM_A_D2;
extern bool OAM_A_D3;
extern bool OAM_A_D4;
extern bool OAM_A_D5;
extern bool OAM_A_D6;
extern bool OAM_A_D7;

extern bool OAM_B_D0;
extern bool OAM_B_D1;
extern bool OAM_B_D2;
extern bool OAM_B_D3;
extern bool OAM_B_D4;
extern bool OAM_B_D5;
extern bool OAM_B_D6;
extern bool OAM_B_D7;

extern bool DMA_A0;
extern bool DMA_A1;
extern bool DMA_A2;
extern bool DMA_A3;
extern bool DMA_A4;
extern bool DMA_A5;
extern bool DMA_A6;
extern bool DMA_A7;

extern bool OAM_A1;
extern bool OAM_A2;
extern bool OAM_A3;
extern bool OAM_A4;
extern bool OAM_A5;
extern bool OAM_A6;
extern bool OAM_A7;

extern bool V0;
extern bool V1;
extern bool V2;
extern bool V3;
extern bool V4;
extern bool V5;
extern bool V6;
extern bool V7;
