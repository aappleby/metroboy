#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct LcdSignals {
  /*p29.CATU*/ Signal CATU_VID_LINE_d4;
  /*p28.BYHA*/ Signal BYHA_VID_LINE_TRIG_d4n;
  /*p28.ATEJ*/ Signal ATEJ_VID_LINE_TRIG_d4p;
  /*p27.XAHY*/ Signal XAHY_VID_LINE_TRIG_d4n;
  /*p28.BYVA*/ Signal BYVA_VID_LINE_TRIG_d4n;
  /*p29.DYBA*/ Signal DYBA_VID_LINE_TRIG_d4p;
  /*p21.PURE*/ Signal PURE_NEW_LINE_d0n;
  /*p21.SELA*/ Signal SELA_NEW_LINE_d0p;
  /*p21.POPU*/ Signal POPU_VBLANK_d4;
  /*p21.PARU*/ Signal PARU_VBLANKp;
  /*p21.TOLU*/ Signal TOLU_VBLANKn;
  /*p21.VYPU*/ Signal VYPU_VBLANKp;
  /*p21.ROPO*/ Signal ROPO_LY_MATCH_SYNCp;
  /*p21.PALY*/ Signal PALY_LY_MATCHa;
  /*p28.ANOM*/ Signal ANOM_SCAN_RSTn;


  /*p21.MUWY*/ Signal MUWY_Y0;
  /*p21.MYRO*/ Signal MYRO_Y1;
  /*p21.LEXA*/ Signal LEXA_Y2;
  /*p21.LYDO*/ Signal LYDO_Y3;
  /*p21.LOVU*/ Signal LOVU_Y4;
  /*p21.LEMA*/ Signal LEMA_Y5;
  /*p21.MATO*/ Signal MATO_Y6;
  /*p21.LAFO*/ Signal LAFO_Y7;
};

//-----------------------------------------------------------------------------

struct LcdRegisters {

  LcdSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

private:

  // 17-rung regs. Output order QN/Q
  /*p21.SAXO*/ Reg XEHO_X0; // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ Reg SAVY_X1;
  /*p21.VYZO*/ Reg XODU_X2;
  /*p21.TELU*/ Reg XYDO_X3;
  /*p21.SUDE*/ Reg TUHU_X4;
  /*p21.TAHA*/ Reg TUKY_X5;
  /*p21.TYRY*/ Reg TAKO_X6;

  // 17-rung regs. Output order QN/Q
  /*p21.MUWY*/ Reg MUWY_Y0; // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ Reg MYRO_Y1;
  /*p21.LEXA*/ Reg LEXA_Y2;
  /*p21.LYDO*/ Reg LYDO_Y3;
  /*p21.LOVU*/ Reg LOVU_Y4;
  /*p21.LEMA*/ Reg LEMA_Y5;
  /*p21.MATO*/ Reg MATO_Y6;
  /*p21.LAFO*/ Reg LAFO_Y7;

  /*p21.RUTU*/ Reg RUTU_NEW_LINE_d0; // p909+8
  /*p29.CATU*/ Reg CATU_VID_LINE_d4; // p001+8
  /*p21.NYPE*/ Reg NYPE_NEW_LINE_d4; // p001+8
  /*p28.ANEL*/ Reg ANEL_VID_LINE_d6; // p003+8

  /*p21.MYTA*/ Reg MYTA_LINE_153_d4;  // p153:001 - p000:001
  /*p21.POPU*/ Reg POPU_VBLANK_d4; // p144:001 - p000:001

  /*p21.SYGU*/ Reg SYGU_LINE_STROBE;

  /*p24.MEDA*/ Reg MEDA_VSYNC_OUTn;
  /*p24.LUCA*/ Reg LUCA_LINE_EVEN;
  /*p21.NAPO*/ Reg NAPO_FRAME_EVEN;

  // FF45 - LYC
  /*p23.SYRY*/ Reg SYRY_LYC0;
  /*p23.VUCE*/ Reg VUCE_LYC1;
  /*p23.SEDY*/ Reg SEDY_LYC2;
  /*p23.SALO*/ Reg SALO_LYC3;
  /*p23.SOTA*/ Reg SOTA_LYC4;
  /*p23.VAFA*/ Reg VAFA_LYC5;
  /*p23.VEVO*/ Reg VEVO_LYC6;
  /*p23.RAHA*/ Reg RAHA_LYC7;

  /*p21.ROPO*/ Reg ROPO_LY_MATCH_SYNCp;

  /* PIN_50 */ PinOut LD1;
  /* PIN_51 */ PinOut LD0;
  /* PIN_52 */ PinOut CPG;
  /* PIN_55 */ PinOut CPL;
  /* PIN_56 */ PinOut FR;
  /* PIN_57 */ PinOut S;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics