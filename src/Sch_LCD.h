#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct LcdSignals {
  /*p29.CATU*/ bool CATU_VID_LINE_d4;
  /*p28.BYHA*/ bool BYHA_VID_LINE_TRIG_d4n;
  /*p28.ATEJ*/ bool ATEJ_VID_LINE_TRIG_d4p;
  /*p27.XAHY*/ bool XAHY_VID_LINE_TRIG_d4n;
  /*p28.BYVA*/ bool BYVA_VID_LINE_TRIG_d4n;
  /*p29.DYBA*/ bool DYBA_VID_LINE_TRIG_d4p;
  /*p21.PURE*/ bool PURE_NEW_LINE_d0n;
  /*p21.SELA*/ bool SELA_NEW_LINE_d0p;
  /*p21.POPU*/ bool POPU_VBLANK_d4;
  /*p21.PARU*/ bool PARU_VBLANKp;
  /*p21.TOLU*/ bool TOLU_VBLANKn;
  /*p21.VYPU*/ bool VYPU_VBLANKp;
  /*p21.ROPO*/ bool ROPO_LY_MATCH_SYNCp;

  /*p21.MUWY*/ bool MUWY_Y0;
  /*p21.MYRO*/ bool MYRO_Y1;
  /*p21.LEXA*/ bool LEXA_Y2;
  /*p21.LYDO*/ bool LYDO_Y3;
  /*p21.LOVU*/ bool LOVU_Y4;
  /*p21.LEMA*/ bool LEMA_Y5;
  /*p21.MATO*/ bool MATO_Y6;
  /*p21.LAFO*/ bool LAFO_Y7;
};

//-----------------------------------------------------------------------------

struct LcdRegisters {

  LcdSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  uint32_t x() const {
    return (XEHO_X0.q() << 0) | (SAVY_X1.q() << 1) | (XODU_X2.q() << 2) | (XYDO_X3.q() << 3) | (TUHU_X4.q() << 4) | (TUKY_X5.q() << 5) | (TAKO_X6.q() << 6);
  }

  uint32_t y() const {
    return (MUWY_Y0.q() << 0) | (MYRO_Y1.q() << 1) | (LEXA_Y2.q() << 2) | (LYDO_Y3.q() << 3) | (LOVU_Y4.q() << 4) | (LEMA_Y5.q() << 5) | (MATO_Y6.q() << 6) | (LAFO_Y7.q() << 7);
  }


  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- LCD REG -----\n");

    dump(text_painter, "LCD X ", XEHO_X0, SAVY_X1, XODU_X2, XYDO_X3, TUHU_X4, TUKY_X5, TAKO_X6);
    dump(text_painter, "LCD Y ", MUWY_Y0, MYRO_Y1, LEXA_Y2, LYDO_Y3, LOVU_Y4, LEMA_Y5, MATO_Y6, LAFO_Y7);

    dump_long(text_painter, "RUTU_NEW_LINE_d0   ", RUTU_NEW_LINE_d0.a);
    dump_long(text_painter, "CATU_VID_LINE_d4   ", CATU_VID_LINE_d4.a);
    dump_long(text_painter, "NYPE_NEW_LINE_d4   ", NYPE_NEW_LINE_d4.a);
    dump_long(text_painter, "ANEL_VID_LINE_d6   ", ANEL_VID_LINE_d6.a);
    dump_long(text_painter, "MYTA_LINE_153_d4   ", MYTA_LINE_153_d4.a);
    dump_long(text_painter, "POPU_VBLANK_d4     ", POPU_VBLANK_d4.a);
    dump_long(text_painter, "SYGU_LINE_STROBE   ", SYGU_LINE_STROBE.a);
    dump_long(text_painter, "MEDA_VSYNC_OUTn    ", MEDA_VSYNC_OUTn.a);
    dump_long(text_painter, "LUCA_LINE_EVEN     ", LUCA_LINE_EVEN.a);
    dump_long(text_painter, "NAPO_FRAME_EVEN    ", NAPO_FRAME_EVEN.a);
    text_painter.newline();
  }

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


  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- LCD_PINS -----\n");
    //LD1.dump(text_painter, "LD1 ");
    //LD0.dump(text_painter, "LD0 ");
    CPG.dump(text_painter, "CPG ");
    CPL.dump(text_painter, "CPL ");
    FR.dump(text_painter, "FR  ");
    S.dump(text_painter, "S   ");
    text_painter.newline();
  }

  /* PIN_52 */ PinOut CPG;
  /* PIN_55 */ PinOut CPL;
  /* PIN_56 */ PinOut FR;
  /* PIN_57 */ PinOut S;

  bool commit_pins() {
    bool changed = false;
    /* PIN_52 */ changed |= CPG.commit_pinout();
    /* PIN_55 */ changed |= CPL.commit_pinout();
    /* PIN_56 */ changed |= FR.commit_pinout();
    /* PIN_57 */ changed |= S.commit_pinout();
    return changed;
  }
};

//-----------------------------------------------------------------------------

}; // namespace Schematics