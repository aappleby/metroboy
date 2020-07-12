#include "Sch_LCD.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

wire LcdRegisters::BYHA_VID_LINE_TRIG_d4(const SchematicTop& top) const {
  /*p28.ABAF*/ wire ABAF_LINE_END_Bn = not(CATU_LINE_END.q());
  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4 = and (or (ANEL_LINE_END_D.q(), ABAF_LINE_END_Bn), top.ABEZ_VID_RSTn());
  return BYHA_VID_LINE_TRIG_d4;
}

//------------------------------------------------------------------------------

void LcdRegisters::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void LcdRegisters::tock(SchematicTop& top) {

  /*p21.XYVO*/ wire _XYVO_IN_VBLANKp = and(LOVU_Y4.q(), LAFO_Y7.q()); // 128 + 16 = 144
  /*p29.ALES*/ wire _ALES_IN_VBLANKn = not(_XYVO_IN_VBLANKp);
  /*p24.KEDY*/ wire _KEDY_LCDC_ENn = not(top.XONA_LCDC_EN.q());

  /*p01.LYHA*/ wire LYHA_VID_RSTp = not(top.XAPO_VID_RSTn());
  /*p01.LYFE*/ wire LYFE_VID_RSTn = not(LYHA_VID_RSTp);

  // LCD main timer
  {
    /*p21.MUDE*/ wire _MUDE_X_RSTn = nor(RUTU_LINE_END.q(), LYHA_VID_RSTp);
    /*p21.SAXO*/ XEHO_X0.set(top.clk_reg.TALU_ABCDxxxx(), _MUDE_X_RSTn, !XEHO_X0.q());
    /*p21.TYPO*/ SAVY_X1.set(!XEHO_X0.q(),        _MUDE_X_RSTn, !SAVY_X1.q());
    /*p21.VYZO*/ XODU_X2.set(!SAVY_X1.q(),        _MUDE_X_RSTn, !XODU_X2.q());
    /*p21.TELU*/ XYDO_X3.set(!XODU_X2.q(),        _MUDE_X_RSTn, !XYDO_X3.q());
    /*p21.SUDE*/ TUHU_X4.set(!XYDO_X3.q(),        _MUDE_X_RSTn, !TUHU_X4.q());
    /*p21.TAHA*/ TUKY_X5.set(!TUHU_X4.q(),        _MUDE_X_RSTn, !TUKY_X5.q());
    /*p21.TYRY*/ TAKO_X6.set(!TUKY_X5.q(),        _MUDE_X_RSTn, !TAKO_X6.q());
  }

  {
    /*p21.LAMA*/ wire _LAMA_FRAME_RSTn = nor(MYTA_LINE_153_d4.q(), LYHA_VID_RSTp);
    /*p21.MUWY*/ MUWY_Y0.set(RUTU_LINE_END.q(), _LAMA_FRAME_RSTn, !MUWY_Y0.q());
    /*p21.MYRO*/ MYRO_Y1.set(!MUWY_Y0.q(),      _LAMA_FRAME_RSTn, !MYRO_Y1.q());
    /*p21.LEXA*/ LEXA_Y2.set(!MYRO_Y1.q(),      _LAMA_FRAME_RSTn, !LEXA_Y2.q());
    /*p21.LYDO*/ LYDO_Y3.set(!LEXA_Y2.q(),      _LAMA_FRAME_RSTn, !LYDO_Y3.q());
    /*p21.LOVU*/ LOVU_Y4.set(!LYDO_Y3.q(),      _LAMA_FRAME_RSTn, !LOVU_Y4.q());
    /*p21.LEMA*/ LEMA_Y5.set(!LOVU_Y4.q(),      _LAMA_FRAME_RSTn, !LEMA_Y5.q());
    /*p21.MATO*/ MATO_Y6.set(!LEMA_Y5.q(),      _LAMA_FRAME_RSTn, !MATO_Y6.q());
    /*p21.LAFO*/ LAFO_Y7.set(!MATO_Y6.q(),      _LAMA_FRAME_RSTn, !LAFO_Y7.q());
  }

  {
    /*p21.POPU*/ POPU_VBLANKp_d4.set(NYPE_LINE_END_B.q(), LYFE_VID_RSTn, _XYVO_IN_VBLANKp);
  }

  {
    /*p21.SANU*/ wire _SANU_LINE_END = and(TAKO_X6.q(), TUKY_X5.q(), TUHU_X4.q(), XEHO_X0.q()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*p21.RUTU*/ RUTU_LINE_END.set(top.clk_reg.SONO_xxxxEFGH(), LYFE_VID_RSTn, _SANU_LINE_END);
    /*p21.NYPE*/ NYPE_LINE_END_B.set(top.clk_reg.TALU_ABCDxxxx(), LYFE_VID_RSTn, RUTU_LINE_END.q());
  }

  {
    /*p29.ABOV*/ wire _ABOV_LINE_END_Fp = and(top.lcd_reg.SELA_LINE_END_Fp(), _ALES_IN_VBLANKn);
    /*p29.CATU*/ CATU_LINE_END.set(top.clk_reg.XUPY_ABxxEFxx(), top.ABEZ_VID_RSTn(), _ABOV_LINE_END_Fp);
    /*p28.ANEL*/ ANEL_LINE_END_D.set(top.clk_reg.AWOH_xxCDxxGH(), top.ABEZ_VID_RSTn(), CATU_LINE_END.q());
  }

  {
    /*p21.NOKO*/ wire _NOKO_LINE_153 = and(LAFO_Y7.q(), LOVU_Y4.q(), LYDO_Y3.q(), MUWY_Y0.q()); // Schematic wrong: NOKO = and(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*p21.MYTA*/ MYTA_LINE_153_d4.set(NYPE_LINE_END_B.q(), LYFE_VID_RSTn, _NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, LCD_PIN_FR = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)
    
    /*p24.LOFU*/ wire _LOFU_CLKn = not(RUTU_LINE_END.q());
    /*p24.LUCA*/ LUCA_LINE_EVEN.set(_LOFU_CLKn,          LYFE_VID_RSTn, !LUCA_LINE_EVEN.q());
    /*p21.NAPO*/ NAPO_FRAME_EVEN.set(POPU_VBLANKp_d4.q(), LYFE_VID_RSTn, !NAPO_FRAME_EVEN.q());
    /*p24.MAGU*/ wire _MAGU = xor(NAPO_FRAME_EVEN.q(), LUCA_LINE_EVEN.q());
    /*p24.MECO*/ wire _MECO = not(_MAGU);
    /*p24.KEBO*/ wire _KEBO = not(_MECO);
    /*p24.USEC*/ wire _USEC = not(top.tim_reg.UREK_DIV_07n());
    /*p24.KUPA*/ wire _KUPA = amux2(top.XONA_LCDC_EN.q(), _KEBO, _KEDY_LCDC_ENn, _USEC);
    /*p24.KOFO*/ wire _KOFO = not(_KUPA);
    top.LCD_PIN_FR.set(_KOFO);
  }

  // LCD line strobe
  {
    /*p21.TOCU*/ wire _TOCU_C0n = not(XEHO_X0.q());
    /*p21.VEPE*/ wire _VEPE_C1n = not(SAVY_X1.q());
    /*p21.VUTY*/ wire _VUTY_C2n = not(XODU_X2.q());
    /*p21.VATE*/ wire _VATE_C3n = not(XYDO_X3.q());
    /*p21.TUDA*/ wire _TUDA_C4n = not(TUHU_X4.q());
    /*p21.TAFY*/ wire _TAFY_C5n = not(TUKY_X5.q());
    /*p21.TUJU*/ wire _TUJU_C6n = not(TAKO_X6.q());

    /*p21.VOKU*/ wire _VOKU_000n = nand(  _TUJU_C6n,   _TAFY_C5n,   _TUDA_C4n,   _VATE_C3n,   _VUTY_C2n,   _VEPE_C1n,   _TOCU_C0n); // 0000000 == 0
    /*p21.TOZU*/ wire _TOZU_007n = nand(  _TUJU_C6n,   _TAFY_C5n,   _TUDA_C4n,   _VATE_C3n, XODU_X2.q(), SAVY_X1.q(), XEHO_X0.q()); // 0000111 == 7
    /*p21.TECE*/ wire _TECE_045n = nand(  _TUJU_C6n, TUKY_X5.q(),   _TUDA_C4n, XYDO_X3.q(), XODU_X2.q(),   _VEPE_C1n, XEHO_X0.q()); // 0101101 == 45
    /*p21.TEBO*/ wire _TEBO_083n = nand(TAKO_X6.q(),   _TAFY_C5n, TUHU_X4.q(),   _VATE_C3n,   _VUTY_C2n, SAVY_X1.q(), XEHO_X0.q()); // 1010011 == 83

    /*p21.TEGY*/ wire _TEGY_LINE_STROBE = nand(_VOKU_000n, _TOZU_007n, _TECE_045n, _TEBO_083n);
    
    /*p21.SYGU*/ SYGU_LINE_STROBE.set(top.clk_reg.SONO_xxxxEFGH(), LYFE_VID_RSTn, _TEGY_LINE_STROBE);
    /*p21.RYNO*/ wire _RYNO = or(_TEGY_LINE_STROBE, RUTU_LINE_END.q());
    /*p21.POGU*/ wire _POGU = not(_RYNO);
    top.LCD_PIN_CPG.set(_POGU);
  }

  // LCD LCD_PIN_CPL pin
  {
    /*p24.KASA*/ wire _KASA_LINE_DONE = not(top.lcd_reg.PURE_NEW_LINE_d0n());
    /*p24.UMOB*/ wire _UMOB_DIV_06p = not(top.tim_reg.UMEK_DIV_06n());
    /*p24.KAHE*/ wire _KAHE = amux2(top.XONA_LCDC_EN.q(), _KASA_LINE_DONE, _KEDY_LCDC_ENn, _UMOB_DIV_06p);
    /*p24.KYMO*/ wire _KYMO = not(_KAHE);
    top.LCD_PIN_CPL.set(_KYMO);
  }

  // LCD vertical sync pin
  {
    /*p24.NERU*/ wire _LINE_000n = nor(MUWY_Y0.q(), MYRO_Y1.q(), LEXA_Y2.q(), LYDO_Y3.q(), LOVU_Y4.q(), LEMA_Y5.q(), MATO_Y6.q(), LAFO_Y7.q());
    
    /*p24.MEDA*/ MEDA_VSYNC_OUTn.set(wire(NYPE_LINE_END_B.q()), LYFE_VID_RSTn, _LINE_000n);
    /*p24.MURE*/ wire _MURE_PIN_S = not(MEDA_VSYNC_OUTn.q());
    top.LCD_PIN_S.set(_MURE_PIN_S);
  }

  // ly match
  {
    /*p21.SYFU*/ wire _SYFU_LY_MATCH7n = xor (LAFO_Y7.q(), RAHA_LYC7.qn());
    /*p21.TERY*/ wire _TERY_LY_MATCH6n = xor (MATO_Y6.q(), VEVO_LYC6.qn());
    /*p21.TUCY*/ wire _TUCY_LY_MATCH5n = xor (LEMA_Y5.q(), VAFA_LYC5.qn());
    /*p21.TYKU*/ wire _TYKU_LY_MATCH4n = xor (LOVU_Y4.q(), SOTA_LYC4.qn());
    /*p21.RASY*/ wire _RASY_LY_MATCH3n = xor (LYDO_Y3.q(), SALO_LYC3.qn());
    /*p21.REDA*/ wire _REDA_LY_MATCH2n = xor (LEXA_Y2.q(), SEDY_LYC2.qn());
    /*p21.TYDE*/ wire _TYDE_LY_MATCH1n = xor (MYRO_Y1.q(), VUCE_LYC1.qn());
    /*p21.RYME*/ wire _RYME_LY_MATCH0n = xor (MUWY_Y0.q(), SYRY_LYC0.qn());
    /*p21.SOVU*/ wire _SOVU_LY_MATCHA  = nor (_SYFU_LY_MATCH7n, _TERY_LY_MATCH6n, _TUCY_LY_MATCH5n, _TYKU_LY_MATCH4n); // def nor
    /*p21.SUBO*/ wire _SUBO_LY_MATCHB  = nor (_RASY_LY_MATCH3n, _REDA_LY_MATCH2n, _TYDE_LY_MATCH1n, _RYME_LY_MATCH0n); // def nor
    /*p21.RAPE*/ wire _RAPE_LY_MATCHn  = nand(_SOVU_LY_MATCHA,  _SUBO_LY_MATCHB); // def nand
    /*p21.PALY*/ wire _PALY_LY_MATCHa  = not (_RAPE_LY_MATCHn); // def not

    /*p21.ROPO*/ ROPO_LY_MATCH_SYNCp.set(top.clk_reg.TALU_ABCDxxxx(), top.rst_reg.WESY_SYS_RSTn(), _PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /*p22.WYLE*/ wire _WYLE_FF44n = nand(top.WERO_FF4Xp(), top.XOLA_A00n(), top.XENO_A01n(), top.WALO_A02p(), top.XERA_A03n());
    /*p22.XOGY*/ wire _XOGY_FF44p = not(_WYLE_FF44n);

    /*p23.WAFU*/ wire _WAFU_FF44_RDp = and (top.ASOT_CPU_RDp(), _XOGY_FF44p);
    /*p23.VARO*/ wire _VARO_FF44_RDn = not(_WAFU_FF44_RDp);

    /*p23.WURY*/ wire _WURY_LY0n = not(MUWY_Y0.q());
    /*p23.XEPO*/ wire _XEPO_LY1n = not(MYRO_Y1.q());
    /*p23.MYFA*/ wire _MYFA_LY2n = not(LEXA_Y2.q());
    /*p23.XUHY*/ wire _XUHY_LY3n = not(LYDO_Y3.q());
    /*p23.WATA*/ wire _WATA_LY4n = not(LOVU_Y4.q());
    /*p23.XAGA*/ wire _XAGA_LY5n = not(LEMA_Y5.q());
    /*p23.XUCE*/ wire _XUCE_LY6n = not(MATO_Y6.q());
    /*p23.XOWO*/ wire _XOWO_LY7n = not(LAFO_Y7.q());

    /*p23.VEGA*/ top.CPU_TRI_D0.set_tribuf_6n(_VARO_FF44_RDn, not(_WURY_LY0n));
    /*p23.WUVA*/ top.CPU_TRI_D1.set_tribuf_6n(_VARO_FF44_RDn, not(_XEPO_LY1n));
    /*p23.LYCO*/ top.CPU_TRI_D2.set_tribuf_6n(_VARO_FF44_RDn, not(_MYFA_LY2n));
    /*p23.WOJY*/ top.CPU_TRI_D3.set_tribuf_6n(_VARO_FF44_RDn, not(_XUHY_LY3n));
    /*p23.VYNE*/ top.CPU_TRI_D4.set_tribuf_6n(_VARO_FF44_RDn, not(_WATA_LY4n));
    /*p23.WAMA*/ top.CPU_TRI_D5.set_tribuf_6n(_VARO_FF44_RDn, not(_XAGA_LY5n));
    /*p23.WAVO*/ top.CPU_TRI_D6.set_tribuf_6n(_VARO_FF44_RDn, not(_XUCE_LY6n));
    /*p23.WEZE*/ top.CPU_TRI_D7.set_tribuf_6n(_VARO_FF44_RDn, not(_XOWO_LY7n));
  }

  // FF45 LYC
  {
    /*p22.WETY*/ wire _WETY_FF45n = nand(top.WERO_FF4Xp(), top.WADO_A00p(), top.XENO_A01n(), top.WALO_A02p(), top.XERA_A03n());
    /*p22.XAYU*/ wire _XAYU_FF45p = not(_WETY_FF45n);

    /*p23.XYLY*/ wire _XYLY_FF45_RDp = and(top.ASOT_CPU_RDp(), _XAYU_FF45p);
    /*p23.WEKU*/ wire _WEKU_FF45_RDn = not(_XYLY_FF45_RDp);

    /*p23.RETU*/ top.CPU_TRI_D0.set_tribuf_6n(_WEKU_FF45_RDn, SYRY_LYC0.q());
    /*p23.VOJO*/ top.CPU_TRI_D1.set_tribuf_6n(_WEKU_FF45_RDn, VUCE_LYC1.q());
    /*p23.RAZU*/ top.CPU_TRI_D2.set_tribuf_6n(_WEKU_FF45_RDn, SEDY_LYC2.q());
    /*p23.REDY*/ top.CPU_TRI_D3.set_tribuf_6n(_WEKU_FF45_RDn, SALO_LYC3.q());
    /*p23.RACE*/ top.CPU_TRI_D4.set_tribuf_6n(_WEKU_FF45_RDn, SOTA_LYC4.q());
    /*p23.VAZU*/ top.CPU_TRI_D5.set_tribuf_6n(_WEKU_FF45_RDn, VAFA_LYC5.q());
    /*p23.VAFE*/ top.CPU_TRI_D6.set_tribuf_6n(_WEKU_FF45_RDn, VEVO_LYC6.q());
    /*p23.PUFY*/ top.CPU_TRI_D7.set_tribuf_6n(_WEKU_FF45_RDn, RAHA_LYC7.q());

    /*p23.XUFA*/ wire _XUFA_FF45_WRn = and (top.CUPA_CPU_WRp_xxxxEFGx(), _XAYU_FF45p);
    /*p23.WANE*/ wire _WANE_FF45_WRp = not(_XUFA_FF45_WRn);

    /*p23.SYRY*/ SYRY_LYC0.set(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D0.q());
    /*p23.VUCE*/ VUCE_LYC1.set(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D1.q());
    /*p23.SEDY*/ SEDY_LYC2.set(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D2.q());
    /*p23.SALO*/ SALO_LYC3.set(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D3.q());
    /*p23.SOTA*/ SOTA_LYC4.set(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D4.q());
    /*p23.VAFA*/ VAFA_LYC5.set(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D5.q());
    /*p23.VEVO*/ VEVO_LYC6.set(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D6.q());
    /*p23.RAHA*/ RAHA_LYC7.set(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D7.q());
  }
}

//------------------------------------------------------------------------------

SignalHash LcdRegisters::commit(SchematicTop& top) {
  SignalHash hash;

  /*p21.SAXO*/ hash << XEHO_X0.commit(); // increments at phase 1, reset to 0 at p909.
  /*p21.TYPO*/ hash << SAVY_X1.commit();
  /*p21.VYZO*/ hash << XODU_X2.commit();
  /*p21.TELU*/ hash << XYDO_X3.commit();
  /*p21.SUDE*/ hash << TUHU_X4.commit();
  /*p21.TAHA*/ hash << TUKY_X5.commit();
  /*p21.TYRY*/ hash << TAKO_X6.commit();
  /*p21.MUWY*/ hash << MUWY_Y0.commit(); // increments at p909, reset to 0 at p153:001
  /*p21.MYRO*/ hash << MYRO_Y1.commit();
  /*p21.LEXA*/ hash << LEXA_Y2.commit();
  /*p21.LYDO*/ hash << LYDO_Y3.commit();
  /*p21.LOVU*/ hash << LOVU_Y4.commit();
  /*p21.LEMA*/ hash << LEMA_Y5.commit();
  /*p21.MATO*/ hash << MATO_Y6.commit();
  /*p21.LAFO*/ hash << LAFO_Y7.commit();
  /*p21.RUTU*/ hash << RUTU_LINE_END.commit(); // p909+8
  /*p29.CATU*/ hash << CATU_LINE_END.commit();  // p001+8
  /*p21.NYPE*/ hash << NYPE_LINE_END_B.commit(); // p001+8
  /*p28.ANEL*/ hash << ANEL_LINE_END_D.commit();  // p003+8
  /*p21.MYTA*/ hash << MYTA_LINE_153_d4.commit();  // p153:001 - p000:001
  /*p21.POPU*/ hash << POPU_VBLANKp_d4.commit();    // p144:001 - p000:001
  /*p21.SYGU*/ hash << SYGU_LINE_STROBE.commit();
  /*p24.MEDA*/ hash << MEDA_VSYNC_OUTn.commit();
  /*p24.LUCA*/ hash << LUCA_LINE_EVEN.commit();
  /*p21.NAPO*/ hash << NAPO_FRAME_EVEN.commit();

  /*p23.SYRY*/ hash << SYRY_LYC0.commit();
  /*p23.VUCE*/ hash << VUCE_LYC1.commit();
  /*p23.SEDY*/ hash << SEDY_LYC2.commit();
  /*p23.SALO*/ hash << SALO_LYC3.commit();
  /*p23.SOTA*/ hash << SOTA_LYC4.commit();
  /*p23.VAFA*/ hash << VAFA_LYC5.commit();
  /*p23.VEVO*/ hash << VEVO_LYC6.commit();
  /*p23.RAHA*/ hash << RAHA_LYC7.commit();

  /*p21.ROPO*/ hash << ROPO_LY_MATCH_SYNCp.commit();

  hash << top.LCD_PIN_CPG.commit();
  hash << top.LCD_PIN_CPL.commit();
  hash << top.LCD_PIN_FR.commit();
  hash << top.LCD_PIN_S.commit();

  return hash;
}

//------------------------------------------------------------------------------

#if 0
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

  dump_long(text_painter, "RUTU_LINE_END   ", RUTU_LINE_END.a);
  dump_long(text_painter, "CATU_LINE_END   ", CATU_LINE_END.a);
  dump_long(text_painter, "NYPE_LINE_END_B   ", NYPE_LINE_END_B.a);
  dump_long(text_painter, "ANEL_LINE_END_D   ", ANEL_LINE_END_D.a);
  dump_long(text_painter, "MYTA_LINE_153_d4   ", MYTA_LINE_153_d4.a);
  dump_long(text_painter, "POPU_VBLANKp_d4     ", POPU_VBLANKp_d4.a);
  dump_long(text_painter, "SYGU_LINE_STROBE   ", SYGU_LINE_STROBE.a);
  dump_long(text_painter, "MEDA_VSYNC_OUTn    ", MEDA_VSYNC_OUTn.a);
  dump_long(text_painter, "LUCA_LINE_EVEN     ", LUCA_LINE_EVEN.a);
  dump_long(text_painter, "NAPO_FRAME_EVEN    ", NAPO_FRAME_EVEN.a);
  text_painter.newline();
}



void dump_pins(TextPainter& text_painter) {
  text_painter.dprintf("----- LCD_PINS -----\n");
  //LD1.dump(text_painter, "LD1 ");
  //LD0.dump(text_painter, "LD0 ");
  LCD_PIN_CPG.dump(text_painter, "LCD_PIN_CPG ");
  LCD_PIN_CPL.dump(text_painter, "LCD_PIN_CPL ");
  LCD_PIN_FR.dump(text_painter, "LCD_PIN_FR  ");
  S.dump(text_painter, "S   ");
  text_painter.newline();
}

#endif