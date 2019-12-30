#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct LCDSignals {
  static LCDSignals tick_slow(const ClockSignals2& clk_sig2,
                        const ResetSignals2& rst_sig2,
                        const LCDRegisters& lcd);

  static LCDSignals tick_fast(const ClockSignals2& clk_sig2,
                             const ResetSignals2& rst_sig2,
                             const LCDRegisters& lcd);

  // ResetRegisters to immediately before the first phase on the first line
  void reset() {
    VBLANK_d4b = true;
		VID_LINE_TRIG_d4n = 1;
		VID_LINE_TRIG_d4a = 0;
		VID_LINE_TRIG_d4p = 1;
		VID_LINE_TRIG_d4c = 0;
		VID_LINE_TRIG_d4o = 1;
  }

  /*p21.PARU*/ bool VBLANK_d4b;
  /*p28.BYHA*/ bool VID_LINE_TRIG_d4n;
  /*p28.ATEJ*/ bool VID_LINE_TRIG_d4a;
  /*p28.BYVA*/ bool VID_LINE_TRIG_d4p;
  /*p29.DYBA*/ bool VID_LINE_TRIG_d4c;
  /*p27.XAHY*/ bool VID_LINE_TRIG_d4o;
};

//-----------------------------------------------------------------------------

struct LCDRegisters {

  static void tock_slow(const ClockSignals2& clk_sig2,
                        const ResetSignals2& rst_sig2,
                        const LCDSignals& /*lcd_sig*/,
                        const LCDRegisters& lcd,
                        LCDRegisters& next);

  static void tock_fast(const ClockSignals2& clk_sig2,
                        const ResetSignals2& rst_sig2,
                        const LCDSignals& /*lcd_sig*/,
                        const LCDRegisters& lcd,
                        LCDRegisters& next);

  // ResetRegisters to immediately before the first phase on the first line
  void reset() {
    X0.val=false; X0.clk=false;
    X1.val=false; X1.clk=true;
    X2.val=false; X2.clk=true;
    X3.val=false; X3.clk=true;
    X4.val=false; X4.clk=true;
    X5.val=false; X5.clk=true;
    X6.val=false; X6.clk=true;
    Y0.val=false; Y0.clk=true;
    Y1.val=false; Y1.clk=true;
    Y2.val=false; Y2.clk=true;
    Y3.val=false; Y3.clk=true;
    Y4.val=false; Y4.clk=true;
    Y5.val=false; Y5.clk=true;
    Y6.val=false; Y6.clk=true;
    Y7.val=false; Y7.clk=true;

    VBLANK_d4b = true;
		VID_LINE_TRIG_d4n = 1;
		VID_LINE_TRIG_d4a = 0;
		VID_LINE_TRIG_d4p = 1;
		VID_LINE_TRIG_d4c = 0;
		VID_LINE_TRIG_d4o = 1;

    VID_LINE_d4.val  = false; VID_LINE_d4.clk  = false; 
    VID_LINE_d6.val  = false; VID_LINE_d6.clk  = true;
    NEW_LINE_d0a.val = true;  NEW_LINE_d0a.clk = true;
    NEW_LINE_d4a.val = false; NEW_LINE_d4a.clk = false;
    LINE_153_d4.val  = true;  LINE_153_d4.clk  = false;
    VBLANK_d4.val    = true;  VBLANK_d4.clk    = false;

  }

  uint32_t x() const {
    return (X0 << 0) | (X1 << 1) | (X2 << 2) | (X3 << 3) | (X4 << 4) | (X5 << 5) | (X6 << 6);
  }

  uint32_t y() const {
    return (Y0 << 0) | (Y1 << 1) | (Y2 << 2) | (Y3 << 3) | (Y4 << 4) | (Y5 << 5) | (Y6 << 6) | (Y7 << 7);
  }

  /*p21.SAXO*/ Reg X0;
  /*p21.TYPO*/ Reg X1;
  /*p21.VYZO*/ Reg X2;
  /*p21.TELU*/ Reg X3;
  /*p21.SUDE*/ Reg X4;
  /*p21.TAHA*/ Reg X5;
  /*p21.TYRY*/ Reg X6;

  /*p21.MUWY*/ Reg Y0;
  /*p21.MYRO*/ Reg Y1;
  /*p21.LEXA*/ Reg Y2;
  /*p21.LYDO*/ Reg Y3;
  /*p21.LOVU*/ Reg Y4;
  /*p21.LEMA*/ Reg Y5;
  /*p21.MATO*/ Reg Y6;
  /*p21.LAFO*/ Reg Y7;

  /*p21.PARU*/ bool VBLANK_d4b;
  /*p28.BYHA*/ bool VID_LINE_TRIG_d4n;
  /*p28.ATEJ*/ bool VID_LINE_TRIG_d4a;
  /*p28.BYVA*/ bool VID_LINE_TRIG_d4p;
  /*p29.DYBA*/ bool VID_LINE_TRIG_d4c;
  /*p27.XAHY*/ bool VID_LINE_TRIG_d4o;

  /*p29.CATU*/ Reg VID_LINE_d4;
  /*p28.ANEL*/ Reg VID_LINE_d6;
  /*p21.RUTU*/ Reg NEW_LINE_d0a;
  /*p21.NYPE*/ Reg NEW_LINE_d4a;
  /*p21.MYTA*/ Reg LINE_153_d4;
  /*p21.POPU*/ Reg VBLANK_d4;

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
