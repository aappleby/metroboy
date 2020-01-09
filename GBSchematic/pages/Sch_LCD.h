#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct LcdSignals {
  /*p21.POPU*/ wire POPU_VBLANK_d4;
  /*p21.PARU*/ wire PARU_VBLANK_d4;
  /*p21.VYPU*/ wire VYPU_INT_VBL;
  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n; // p001+2
  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4;  // p001+2
  /*p28.BYVA*/ wire BYVA_VID_LINE_TRIG_d4n; // p001+2
  /*p29.DYBA*/ wire DYBA_VID_LINE_TRIG_d4;  // p001+2
  /*p27.XAHY*/ wire XAHY_VID_LINE_TRIG_d4n; // p001+2
  /*p29.CATU*/ wire CATU_VID_LINE_d4;       // p001+8
};

//-----------------------------------------------------------------------------

struct LcdRegisters {

  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  LcdSignals tock_slow(const VclkSignals& vid_clk, const RstSignals& rst_sig);
  LcdSignals signals(const RstSignals& rst_sig);

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

