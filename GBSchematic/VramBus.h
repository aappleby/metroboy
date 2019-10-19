#pragma once

struct Vram {
  bool MA0;
  bool MA1;
  bool MA2;
  bool MA3;
  bool MA4;
  bool MA5;
  bool MA7;
  bool MA8;
  bool MA9;
  bool MA6;
  bool MA10;
  bool MA11;
  bool MA12;

  bool MD0;
  bool MD1;
  bool MD2;
  bool MD3;
  bool MD4;
  bool MD5;
  bool MD6;
  bool MD7;
};

extern Vram vram;

extern bool MCS_A, MCS_D, MCS_IN;
extern bool MOE_A, MOE_D, MOE_IN;
extern bool MWR_A, MWR_D, MWR_IN;

// chip-to-vram data bus
extern bool MD0_A, MD0_OUT, MD0_IN;
extern bool MD1_A, MD1_OUT, MD1_IN;
extern bool MD2_A, MD2_OUT, MD2_IN;
extern bool MD3_A, MD3_OUT, MD3_IN;
extern bool MD4_A, MD4_OUT, MD4_IN;
extern bool MD5_A, MD5_OUT, MD5_IN;
extern bool MD6_A, MD6_OUT, MD6_IN;
extern bool MD7_A, MD7_OUT, MD7_IN;

