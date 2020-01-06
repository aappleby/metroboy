#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct LcdSignals {
  /*p21.POPU*/ wire VBLANK_d4;
  /*p21.PARU*/ wire VBLANK_d4b;
  /*p21.VYPU*/ wire INT_VBL;
  /*p28.BYHA*/ wire VID_LINE_TRIG_d4n; // p001+2
  /*p28.ATEJ*/ wire VID_LINE_TRIG_d4a; // p001+2
  /*p28.BYVA*/ wire VID_LINE_TRIG_d4p; // p001+2
  /*p29.DYBA*/ wire VID_LINE_TRIG_d4c; // p001+2
  /*p27.XAHY*/ wire VID_LINE_TRIG_d4o; // p001+2
  /*p29.CATU*/ wire VID_LINE_d4;       // p001+8

  /*p21.MUWY*/ wire Y0; // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ wire Y1;
  /*p21.LEXA*/ wire Y2;
  /*p21.LYDO*/ wire Y3;
  /*p21.LOVU*/ wire Y4;
  /*p21.LEMA*/ wire Y5;
  /*p21.MATO*/ wire Y6;
  /*p21.LAFO*/ wire Y7;
};

//-----------------------------------------------------------------------------

struct LcdRegisters {

  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  LcdSignals tock_slow(const VclkSignals& vid_clk, const VrstSignals& vid_rst);
  LcdSignals signals(const VrstSignals& vid_rst);

  uint32_t x() const {
    return (X0 << 0) | (X1 << 1) | (X2 << 2) | (X3 << 3) | (X4 << 4) | (X5 << 5) | (X6 << 6);
  }

  uint32_t y() const {
    return (Y0 << 0) | (Y1 << 1) | (Y2 << 2) | (Y3 << 3) | (Y4 << 4) | (Y5 << 5) | (Y6 << 6) | (Y7 << 7);
  }


  /*p21.SAXO*/ Reg2 X0; // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ Reg2 X1;
  /*p21.VYZO*/ Reg2 X2;
  /*p21.TELU*/ Reg2 X3;
  /*p21.SUDE*/ Reg2 X4;
  /*p21.TAHA*/ Reg2 X5;
  /*p21.TYRY*/ Reg2 X6;

  /*p21.MUWY*/ Reg2 Y0; // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ Reg2 Y1;
  /*p21.LEXA*/ Reg2 Y2;
  /*p21.LYDO*/ Reg2 Y3;
  /*p21.LOVU*/ Reg2 Y4;
  /*p21.LEMA*/ Reg2 Y5;
  /*p21.MATO*/ Reg2 Y6;
  /*p21.LAFO*/ Reg2 Y7;

  /*p21.RUTU*/ Reg2 NEW_LINE_d0a; // p909+8
  /*p29.CATU*/ Reg2 VID_LINE_d4;  // p001+8
  /*p21.NYPE*/ Reg2 NEW_LINE_d4a; // p001+8
  /*p28.ANEL*/ Reg2 VID_LINE_d6;  // p003+8
                                 
  /*p21.MYTA*/ Reg2 LINE_153_d4;  // p153:001 - p000:001
  /*p21.POPU*/ Reg2 VBLANK_d4;    // p144:001 - p000:001

};

//-----------------------------------------------------------------------------

};





#if 0
    X_8_SYNC.val = false; X_8_SYNC.clk = false;
    LINE_STROBE.val = false; LINE_STROBE.clk = false;

    /*p21.WUSA*/ CPEN_LATCH = 0;

    /*p24.POME*/ POME = 0;
    /*p24.RUJU*/ RUJU = 0;

    /*p24.MEDA*/ VSYNC_OUTn.val = false; VSYNC_OUTn.clk = false;
    /*p24.LUCA*/ LINE_EVEN.val = true;  LINE_EVEN.clk = true;
    /*p21.NAPO*/ FRAME_EVEN.val = true; FRAME_EVEN.clk = true;

    /* PIN_50 */ PIN_LD1 = 0;
    /* PIN_51 */ PIN_LD0 = 0;
    /* PIN_52 */ PIN_CPG = 1;
    /* PIN_53 */ PIN_CP = 1;
    /* PIN_54 */ PIN_ST = 0;
    /* PIN_55 */ PIN_CPL = 1;
    /* PIN_56 */ PIN_FR = 1;
    /* PIN_57 */ PIN_S = 1;
#endif



#if 0
  /*p21.SYGU*/ Reg LINE_STROBE;
  /*p24.PAHO*/ Reg X_8_SYNC;

  /*p21.WUSA*/ bool CPEN_LATCH;

  /*p24.POME*/ bool POME; 
  /*p24.RUJU*/ bool RUJU;

  /*p24.MEDA*/ Reg VSYNC_OUTn;
  /*p24.LUCA*/ Reg LINE_EVEN;
  /*p21.NAPO*/ Reg FRAME_EVEN;

  //----------
  // LCD pins

  /* PIN_50 */ bool PIN_LD1;
  /* PIN_51 */ bool PIN_LD0;
  /* PIN_52 */ bool PIN_CPG;
  /* PIN_53 */ bool PIN_CP;
  /* PIN_54 */ bool PIN_ST;
  /* PIN_55 */ bool PIN_CPL;
  /* PIN_56 */ bool PIN_FR;
  /* PIN_57 */ bool PIN_S;
#endif
