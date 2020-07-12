#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct LcdRegisters {

  void tick(SchematicTop& top);
  void tock(SchematicTop& top);
  SignalHash commit(SchematicTop& top);

private:
  friend SchematicTop;

  /*p21.SAXO*/ Reg17 XEHO_X0; // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ Reg17 SAVY_X1;
  /*p21.VYZO*/ Reg17 XODU_X2;
  /*p21.TELU*/ Reg17 XYDO_X3;
  /*p21.SUDE*/ Reg17 TUHU_X4;
  /*p21.TAHA*/ Reg17 TUKY_X5;
  /*p21.TYRY*/ Reg17 TAKO_X6;

  /*p21.MUWY*/ Reg17 MUWY_Y0; // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ Reg17 MYRO_Y1;
  /*p21.LEXA*/ Reg17 LEXA_Y2;
  /*p21.LYDO*/ Reg17 LYDO_Y3;
  /*p21.LOVU*/ Reg17 LOVU_Y4;
  /*p21.LEMA*/ Reg17 LEMA_Y5;
  /*p21.MATO*/ Reg17 MATO_Y6;
  /*p21.LAFO*/ Reg17 LAFO_Y7;

  /*p21.RUTU*/ Reg17 RUTU_LINE_END; // p909+8
  /*p29.CATU*/ Reg17 CATU_LINE_END; // p001+8
  /*p21.NYPE*/ Reg17 NYPE_LINE_END_B; // p001+8
  /*p28.ANEL*/ Reg17 ANEL_LINE_END_D; // p003+8

  /*p21.MYTA*/ Reg17 MYTA_LINE_153_d4;  // p153:001 - p000:001
  /*p21.POPU*/ Reg17 POPU_VBLANKp_d4; // p144:001 - p000:001

  /*p21.SYGU*/ Reg17 SYGU_LINE_STROBE;

  /*p24.MEDA*/ Reg17 MEDA_VSYNC_OUTn;
  /*p24.LUCA*/ Reg17 LUCA_LINE_EVEN;
  /*p21.NAPO*/ Reg17 NAPO_FRAME_EVEN;

  // FF45 - LYC
  /*p23.SYRY*/ Reg9 SYRY_LYC0;
  /*p23.VUCE*/ Reg9 VUCE_LYC1;
  /*p23.SEDY*/ Reg9 SEDY_LYC2;
  /*p23.SALO*/ Reg9 SALO_LYC3;
  /*p23.SOTA*/ Reg9 SOTA_LYC4;
  /*p23.VAFA*/ Reg9 VAFA_LYC5;
  /*p23.VEVO*/ Reg9 VEVO_LYC6;
  /*p23.RAHA*/ Reg9 RAHA_LYC7;

  /*p21.ROPO*/ Reg17 ROPO_LY_MATCH_SYNCp;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics