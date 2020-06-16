#pragma once
#include "Types.h"

// lcd x/y, lyc match, stat mode & int

struct LCD {
  void reset();
  void tick(const Req& req, Ack& ack);
  void tock(int phase, const Req& req, bool LCDC_EN);

  void dump(std::string& d);

  uint8_t X;
  uint8_t Y;

  uint8_t LY;
  uint8_t LYC;
  uint8_t STAT;

  /*p21.RUTU*/ bool NEW_LINE_d0 = 0;
  /*p21.NYPE*/ bool NEW_LINE_d4 = 0;
  /*p21.MYTA*/ bool LINE_153_d4 = 0;
  /*p21.POPU*/ bool IN_VBLANK_d4 = 0;
  /*p24.MEDA*/ bool VSYNC_OUTn = 0;

  /*p24.LUCA*/ bool LINE_EVEN = 0;
  /*p21.NAPO*/ bool FRAME_EVEN = 0;

#if 0
  /* PIN_50 */ PinOut LD1;
  /* PIN_51 */ PinOut LD0;
  /* PIN_52 */ PinOut CPG;
  /* PIN_53 */ PinOut CP;
  /* PIN_54 */ PinOut ST;
  /* PIN_55 */ PinOut CPL;
  /* PIN_56 */ PinOut FR;
  /* PIN_57 */ PinOut S;
#endif


  /*p21.POGU*/ bool PIN_CPG = 0;
  /*p24.KYMO*/ bool PIN_CPL = 0;
  /*p24.MURE*/ bool PIN_S = 0;
};