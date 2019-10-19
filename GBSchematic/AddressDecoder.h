#pragma once

struct AddressDecoder {
  bool FF10;
  bool FF11;
  bool FF12;
  bool FF13;
  bool FF14;
  bool FF16;
  bool FF17;
  bool FF18;
  bool FF19;
  bool FF1A;
  bool FF1B;
  bool FF1C;
  bool FF1D;
  bool FF1E;
  bool FF1F;

  bool FF20;
  bool FF21;
  bool FF22;
  bool FF23;
  bool FF24;
  bool FF25;
  bool FF26;
  bool FF27;
};

extern AddressDecoder dec;

extern bool A00_07;
extern bool XXX6;
extern bool FEXXFFXXn;
extern bool FF00RD;
extern bool FF00WR;
extern bool FF04_FF07;
extern bool FF0F;
extern bool FF0F_RD;
extern bool FF0F_WR;

extern bool FF1Xn;
extern bool FF2X;
extern bool FF2Xn;
extern bool FF3X;
extern bool FF40;
extern bool FF41;
extern bool FF42;
extern bool FF43;
extern bool FF44;
extern bool FF45;
extern bool FF46;
extern bool FF47;
extern bool FF48;
extern bool FF49;
extern bool FF4A;
extern bool FF4B;
extern bool FFXX;
extern bool FFXXn;

