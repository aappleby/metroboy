#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct LcdRegisters {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  SignalHash commit();

  /*p28.BYHA*/ wire LcdRegisters::BYHA_VID_LINE_TRIG_d4() const {
    /*p28.ABAF*/ wire _ABAF_LINE_END_Bn = not(_CATU_VID_LINE_ENDp.q());
    return and (or (_ANEL_VID_LINE_ENDp.q(), _ABAF_LINE_END_Bn), _ABEZ_VID_RSTn);
  }

  // -> lcd, window
  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p() const { return not(BYHA_VID_LINE_TRIG_d4()); }

  // -> sprite scanner
  /*p28.ANOM*/ wire ANOM_LINE_RSTn()         const { return nor(ATEJ_VID_LINE_TRIG_d4p(), _ATAR_VID_RSTp); }

  // -> interrupts, ppu
  /*p21.PARU*/ wire PARU_VBLANKp_d4()     const { return not(POPU_IN_VBLANKp.qn()); }

  // -> lcd
  /*p21.PURE*/ wire PURE_LINE_ENDn()   const { return not(_RUTU_LINE_ENDp.q()); }

  // -> interrupts, lcd
  /*p21.SELA*/ wire SELA_LINE_ENDp()    const { return not(PURE_LINE_ENDn()); }

  // -> sprite scanner
  /*p29.CATU*/ wire CATU_VID_LINE_ENDp()       const { return _CATU_VID_LINE_ENDp.q(); }

  // -> sprite store, lcd
  /*p28.BYVA*/ wire BYVA_VID_LINE_TRIG_d4n() const {
    /*p28.ABAK*/ wire _ABAK_VID_LINE_TRIG_d4p = or (ATEJ_VID_LINE_TRIG_d4p(), _AMYG_VID_RSTp);
    return not(_ABAK_VID_LINE_TRIG_d4p);
  }

  // -> interrupts, lcd, ppu
  /*p21.ROPO*/ wire ROPO_LY_MATCH_SYNCp() const { return _ROPO_LY_MATCH_SYNCp.q(); }
  

  // -> sprite store
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

private:

  Signal _AMYG_VID_RSTp;
  Signal _ATAR_VID_RSTp;
  Signal _ABEZ_VID_RSTn;

  /*p21.RUTU*/ Reg17 _RUTU_LINE_ENDp; // p909+8
  /*p29.CATU*/ Reg17 _CATU_VID_LINE_ENDp;     // p001+8
  /*p21.NYPE*/ Reg17 NYPE_LINE_STARTp;    // p001+8
  /*p28.ANEL*/ Reg17 _ANEL_VID_LINE_ENDp;    // p003+8

  /*p21.MYTA*/ Reg17 MYTA_LINE_153p;   // p153:001 - p000:001
  /*p21.POPU*/ Reg17 POPU_IN_VBLANKp;    // p144:001 - p000:001

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

  /*p21.ROPO*/ Reg17 _ROPO_LY_MATCH_SYNCp;

  ExtPinOut LCD_PIN_CPG;  // PIN_52 
  ExtPinOut LCD_PIN_CPL;  // PIN_55 
  ExtPinOut LCD_PIN_FR;   // PIN_56 
  ExtPinOut LCD_PIN_S;    // PIN_57 
};

//-----------------------------------------------------------------------------

}; // namespace Schematics