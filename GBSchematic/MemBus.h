#pragma once

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
