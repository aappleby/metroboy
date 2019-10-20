#pragma once
#include "Schematics.h"

struct MemBus {
  bool A0;
  bool A1;
  bool A2;
  bool A3;
  bool A4;
  bool A5;
  bool A6;
  bool A7;
  bool A8;
  bool A9;
  bool A10;
  bool A11;
  bool A12;
  bool A13;
  bool A14;
  bool A15;

  uint16_t get_addr() {
    return (uint16_t)pack(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15);
  }

  void set_addr(uint16_t x) {
    unpack(x,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15);
  }

  void set_lo(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h) {
    A0 = a; A1 = b; A2 = c; A3 = d; A4 = e; A5 = f; A6 = g; A7 = h;
  }

  void set_hi(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h) {
    A8 = a; A9 = b; A10 = c; A11 = d; A12 = e; A13 = f; A14 = g; A15 = h;
  }
  
  void set_d(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h) {
    D0 = a; D1 = b; D2 = c; D3 = d; D4 = e; D5 = f; D6 = g; D7 = h;
  }

  bool match(uint16_t x) {
    return x == get_addr();
  }

  bool match(uint16_t mask, uint16_t x) {
    return (x & mask) == (get_addr() & mask);
  }

  uint8_t get_d() {
    return (uint8_t)pack(D0, D1, D2, D3, D4, D5, D6, D7);
  }

  bool D0;
  bool D1;
  bool D2;
  bool D3;
  bool D4;
  bool D5;
  bool D6;
  bool D7;

  bool TOVY_A0n;
  bool TOLA_A1n;
};

extern MemBus mem;
