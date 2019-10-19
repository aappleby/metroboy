#pragma once

struct AddressDecoder {
  bool A00_07;
  bool XXX6;
  bool FEXXFFXXn;
  bool FF04_FF07;

  bool FFXXn;
  bool FF1Xn;
  bool FF2X;
  bool FF3X;
  bool FFXX;

  bool FF2Xn;

  //----------

  bool FF0F;

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

  bool FF40;
  bool FF41;
  bool FF42;
  bool FF43;
  bool FF44;
  bool FF45;
  bool FF46;
  bool FF47;
  bool FF48;
  bool FF49;
  bool FF4A;
  bool FF4B;
};

extern AddressDecoder dec;

extern bool FF00RD;
extern bool FF00WR;
extern bool FF0F_RD;
extern bool FF0F_WR;
