#include "Sch_LCD.h"
#include "Sch_Top.h"

using namespace Schematics;

void LcdRegisters::dump(Dumper& d, const SchematicTop& top) const {
  d("----------   LCD    ----------\n");
  d("LCD X : %03d\n", get_x());
  d("LCD Y : %03d\n", get_y());
  d("LYC   : %03d\n", get_lyc());

  d("PIX COUNT     %03d\n", top.pix_pipe.get_pix_count());
  d("!LCD_PIN_CLOCK %c\n", top.LCD_PIN_CLOCK.cn());
  d("!LCD_PIN_HSYNC %c\n", top.LCD_PIN_HSYNC.cn());
  d("!LCD_PIN_VSYNC %c\n", top.LCD_PIN_VSYNC.cn());
  d("\n");
  d("!LCD_PIN_DATA1 %c\n", top.LCD_PIN_DATA1.cn());
  d("!LCD_PIN_DATA0 %c\n", top.LCD_PIN_DATA0.cn());
  d("\n");
  d("!LCD_PIN_CNTRL %c\n", top.LCD_PIN_CNTRL.cn());
  d("!LCD_PIN_LATCH %c\n", top.LCD_PIN_LATCH.cn());
  d("!LCD_PIN_ALTSG %c\n", top.LCD_PIN_ALTSG.cn());
  d("\n");

  d("NYPE_LINE_P000          %c\n", _NYPE_LINE_P000.c());
  d("ANEL_LINE_P000          %c\n", _ANEL_LINE_P000.c());
  d("RUTU_LINE_P908          %c\n", _RUTU_LINE_P908.c());
  d("CATU_LINE_P910          %c\n", _CATU_LINE_P910.c());
  d("BYHA_VID_LINE_END_TRIGn %d\n", BYHA_VID_LINE_END_TRIGn());
  d("ATEJ_VID_LINE_END_TRIGp %d\n", ATEJ_VID_LINE_END_TRIGp());
  d("\n");

  d("MYTA_LINE_153p      %c\n", _MYTA_LINE_153p     .c());
  d("POPU_IN_VBLANKp     %c\n", _POPU_IN_VBLANKp    .c());
  d("SYGU_LINE_STROBE    %c\n", _SYGU_LINE_STROBE   .c());
  d("MEDA_VSYNC_OUTn     %c\n", _MEDA_VSYNC_OUTn    .c());
  d("LUCA_LINE_EVEN      %c\n", _LUCA_LINE_EVEN     .c());
  d("NAPO_FRAME_EVEN     %c\n", _NAPO_FRAME_EVEN    .c());
  d("ROPO_LY_MATCH_SYNCp %c\n", _ROPO_LY_MATCH_SYNCp.c());
  d("\n");

}


//------------------------------------------------------------------------------

void LcdRegisters::tick(const SchematicTop& top) {
  _ABEZ_VID_RSTn = top.clk_reg.ABEZ_VID_RSTn();
}

//------------------------------------------------------------------------------

void LcdRegisters::tock(int phase, SchematicTop& top, CpuBus& cpu_bus) {
  (void)phase;

  /*p21.XYVO*/ wire _XYVO_IN_VBLANKp = and2(LOVU_Y4p.qp(), LAFO_Y7p.qp()); // 128 + 16 = 144
  /*p29.ALES*/ wire _ALES_IN_VBLANKn = not1(_XYVO_IN_VBLANKp);
  /*p21.POPU*/ _POPU_IN_VBLANKp = dff17_AB(_NYPE_LINE_P000.qp(), top.clk_reg.LYFE_VID_RSTn(), _XYVO_IN_VBLANKp);

  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(top.pix_pipe.XONA_LCDC_ENn.qn());

  // LCD main timer

  {
    // 912 phases per line

    /*p21.MUDE*/ wire _MUDE_X_RSTn = nor2(_RUTU_LINE_P908.qp(), top.clk_reg.LYHA_VID_RSTp());
    /*p21.SAXO*/ SAXO_X0p = dff17_AB(top.clk_reg.TALU_ABCDxxxx(),  _MUDE_X_RSTn, SAXO_X0p.qn());
    /*p21.TYPO*/ TYPO_X1p = dff17_AB(SAXO_X0p.qn(),                _MUDE_X_RSTn, TYPO_X1p.qn());
    /*p21.VYZO*/ VYZO_X2p = dff17_AB(TYPO_X1p.qn(),                _MUDE_X_RSTn, VYZO_X2p.qn());
    /*p21.TELU*/ TELU_X3p = dff17_AB(VYZO_X2p.qn(),                _MUDE_X_RSTn, TELU_X3p.qn());
    /*p21.SUDE*/ SUDE_X4p = dff17_AB(TELU_X3p.qn(),                _MUDE_X_RSTn, SUDE_X4p.qn());
    /*p21.TAHA*/ TAHA_X5p = dff17_AB(SUDE_X4p.qn(),                _MUDE_X_RSTn, TAHA_X5p.qn());
    /*p21.TYRY*/ TYRY_X6p = dff17_AB(TAHA_X5p.qn(),                _MUDE_X_RSTn, TYRY_X6p.qn());
  }

  {
    /*p21.LAMA*/ wire _LAMA_FRAME_RSTn = nor2(_MYTA_LINE_153p.qp(), top.clk_reg.LYHA_VID_RSTp());
    /*p21.MUWY*/ MUWY_Y0p = dff17_AB(_RUTU_LINE_P908.qp(),  _LAMA_FRAME_RSTn, MUWY_Y0p.qn());
    /*p21.MYRO*/ MYRO_Y1p = dff17_AB(MUWY_Y0p.qn(),         _LAMA_FRAME_RSTn, MYRO_Y1p.qn());
    /*p21.LEXA*/ LEXA_Y2p = dff17_AB(MYRO_Y1p.qn(),         _LAMA_FRAME_RSTn, LEXA_Y2p.qn());
    /*p21.LYDO*/ LYDO_Y3p = dff17_AB(LEXA_Y2p.qn(),         _LAMA_FRAME_RSTn, LYDO_Y3p.qn());
    /*p21.LOVU*/ LOVU_Y4p = dff17_AB(LYDO_Y3p.qn(),         _LAMA_FRAME_RSTn, LOVU_Y4p.qn());
    /*p21.LEMA*/ LEMA_Y5p = dff17_AB(LOVU_Y4p.qn(),         _LAMA_FRAME_RSTn, LEMA_Y5p.qn());
    /*p21.MATO*/ MATO_Y6p = dff17_AB(LEMA_Y5p.qn(),         _LAMA_FRAME_RSTn, MATO_Y6p.qn());
    /*p21.LAFO*/ LAFO_Y7p = dff17_AB(MATO_Y6p.qn(),         _LAMA_FRAME_RSTn, LAFO_Y7p.qn());
  }

  {
    /*#p21.SANU*/ wire _SANU_x113p = and4(TYRY_X6p.qp(), TAHA_X5p.qp(), SUDE_X4p.qp(), SAXO_X0p.qp()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*p21.RUTU*/ _RUTU_LINE_P908 = dff17_B (top.clk_reg.SONO_xxxxEFGH(), top.clk_reg.LYFE_VID_RSTn(), _SANU_x113p);
    /*p21.NYPE*/ _NYPE_LINE_P000 = dff17_AB(top.clk_reg.TALU_ABCDxxxx(), top.clk_reg.LYFE_VID_RSTn(), _RUTU_LINE_P908.qp());

    /*#p21.PURE*/ wire PURE_LINE_P908n = not1(_RUTU_LINE_P908.qp());
    /*#p21.SELA*/ wire SELA_LINE_P908p = not1(PURE_LINE_P908n);
    /*#p29.ABOV*/ wire ABOV_LINE_P908p = and2(SELA_LINE_P908p, _ALES_IN_VBLANKn);
    /*p29.CATU*/ _CATU_LINE_P910 = dff17_B(top.clk_reg.XUPY_xxCDxxGH(), top.clk_reg.ABEZ_VID_RSTn(), ABOV_LINE_P908p);
    /*p28.ANEL*/ _ANEL_LINE_P000 = dff17_B(top.clk_reg.AWOH_ABxxEFxx(), top.clk_reg.ABEZ_VID_RSTn(), _CATU_LINE_P910.qp());

    /*#p21.NOKO*/ wire NOKO_LINE_153 = and4(LAFO_Y7p.qp(), LOVU_Y4p.qp(), LYDO_Y3p.qp(), MUWY_Y0p.qp()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*p21.MYTA*/ _MYTA_LINE_153p = dff17_B(_NYPE_LINE_P000.qp(), top.clk_reg.LYFE_VID_RSTn(), NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, LCD_PIN_ALTSG = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)
    
    /*p24.LOFU*/ wire _LOFU_LINE_ENDn = not1(_RUTU_LINE_P908.qp());
    /*p24.LUCA*/ _LUCA_LINE_EVEN = dff17_A(_LOFU_LINE_ENDn,      top.clk_reg.LYFE_VID_RSTn(), _LUCA_LINE_EVEN.qn());
    /*p21.NAPO*/ _NAPO_FRAME_EVEN = dff17_AB(_POPU_IN_VBLANKp.qp(), top.clk_reg.LYFE_VID_RSTn(), !_NAPO_FRAME_EVEN.qp());

    // xor or xnor? does it matter?
    /*p24.MAGU*/ wire _MAGU = xnor2(_NAPO_FRAME_EVEN.qp(), _LUCA_LINE_EVEN.qn());
    /*p24.MECO*/ wire _MECO = not1(_MAGU);
    /*p24.KEBO*/ wire _KEBO = not1(_MECO);
    /*p24.USEC*/ wire _USEC = not1(top.tim_reg.UREK_DIV_07n());

    /*#p24.KUPA*/ wire _KUPA = amux2(top.pix_pipe.XONA_LCDC_ENn.qn(), _KEBO, _KEDY_LCDC_ENn, _USEC);
    /*p24.KOFO*/ wire _KOFO = not1(_KUPA);
    top.LCD_PIN_ALTSG = _KOFO;
  }

  {
    /*p21.TOCU*/ wire _TOCU_C0n = not1(SAXO_X0p.qp());
    /*p21.VEPE*/ wire _VEPE_C1n = not1(TYPO_X1p.qp());
    /*p21.VUTY*/ wire _VUTY_C2n = not1(VYZO_X2p.qp());
    /*p21.VATE*/ wire _VATE_C3n = not1(TELU_X3p.qp());
    /*p21.TUDA*/ wire _TUDA_C4n = not1(SUDE_X4p.qp());
    /*p21.TAFY*/ wire _TAFY_C5n = not1(TAHA_X5p.qp());
    /*p21.TUJU*/ wire _TUJU_C6n = not1(TYRY_X6p.qp());

    /*p21.VOKU*/ wire _VOKU_000n = nand7(  _TUJU_C6n,   _TAFY_C5n,   _TUDA_C4n,   _VATE_C3n,   _VUTY_C2n,   _VEPE_C1n,   _TOCU_C0n); // 0000000 == 0
    /*p21.TOZU*/ wire _TOZU_007n = nand7(  _TUJU_C6n,   _TAFY_C5n,   _TUDA_C4n,   _VATE_C3n, VYZO_X2p.qp(), TYPO_X1p.qp(), SAXO_X0p.qp()); // 0000111 == 7
    /*p21.TECE*/ wire _TECE_045n = nand7(  _TUJU_C6n, TAHA_X5p.qp(),   _TUDA_C4n, TELU_X3p.qp(), VYZO_X2p.qp(),   _VEPE_C1n, SAXO_X0p.qp()); // 0101101 == 45
    /*p21.TEBO*/ wire _TEBO_083n = nand7(TYRY_X6p.qp(),   _TAFY_C5n, SUDE_X4p.qp(),   _VATE_C3n,   _VUTY_C2n, TYPO_X1p.qp(), SAXO_X0p.qp()); // 1010011 == 83

    /*p21.TEGY*/ wire _TEGY_LINE_STROBE = nand4(_VOKU_000n, _TOZU_007n, _TECE_045n, _TEBO_083n);
    
    /*p21.SYGU*/ _SYGU_LINE_STROBE = dff17_B(top.clk_reg.SONO_xxxxEFGH(), top.clk_reg.LYFE_VID_RSTn(), _TEGY_LINE_STROBE);
    /*p21.RYNO*/ wire _RYNO = or2(_TEGY_LINE_STROBE, _RUTU_LINE_P908.qp());
    /*p21.POGU*/ wire _POGU = not1(_RYNO);
    top.LCD_PIN_CNTRL = _POGU;
  }

  {
#if 0
    LCD_PIN_LATCH = !_RUTU_LINE_P908.qp();
#endif
    /*p21.PURE*/ wire PURE_LINE_ENDn = not1(_RUTU_LINE_P908.qp());
    /*p24.KASA*/ wire KASA_LINE_ENDp = not1(PURE_LINE_ENDn);
    /*p24.UMOB*/ wire UMOB_DIV_06p   = not1(top.tim_reg.UMEK_DIV_06n());

    /*#p24.KAHE*/ wire KAHE_LINE_ENDp = amux2(top.pix_pipe.XONA_LCDC_ENn.qn(), KASA_LINE_ENDp, _KEDY_LCDC_ENn, UMOB_DIV_06p);
    /*p24.KYMO*/ wire KYMO_LINE_ENDn = not1(KAHE_LINE_ENDp);
    top.LCD_PIN_LATCH = KYMO_LINE_ENDn;
  }

  // LCD vertical sync pin
  {
#if 0
    /*p24.MEDA*/ _MEDA_VSYNC_OUTn = dff17_B(_NYPE_LINE_P000.qp(), top.clk_reg.LYFE_VID_RSTn(), !LINE_000);
    /*p24.MURE*/ wire _MURE_VSYNC = not1(_MEDA_VSYNC_OUTn.qp());
    LCD_PIN_VSYNC = _MURE_VSYNC;
#endif
    /*p24.NERU*/ wire _LINE_000n = nor8(MUWY_Y0p.qp(), MYRO_Y1p.qp(), LEXA_Y2p.qp(), LYDO_Y3p.qp(), LOVU_Y4p.qp(), LEMA_Y5p.qp(), MATO_Y6p.qp(), LAFO_Y7p.qp());
    /*p24.MEDA*/ _MEDA_VSYNC_OUTn = dff17_B(_NYPE_LINE_P000.qp(), top.clk_reg.LYFE_VID_RSTn(), _LINE_000n);
    /*p24.MURE*/ wire _MURE_VSYNC = not1(_MEDA_VSYNC_OUTn.qp());
    top.LCD_PIN_VSYNC = _MURE_VSYNC;
  }

  // ly match
  {
    // these make sense as xor and not xnor
    // SYFU := xor(LAFO_Q, RAHA_QN)

    /*p21.SYFU*/ wire _SYFU_LY_MATCH7n = xor2(LAFO_Y7p.qp(), _RAHA_LYC7.qn());
    /*p21.TERY*/ wire _TERY_LY_MATCH6n = xor2(MATO_Y6p.qp(), _VEVO_LYC6.qn());
    /*p21.TUCY*/ wire _TUCY_LY_MATCH5n = xor2(LEMA_Y5p.qp(), _VAFA_LYC5.qn());
    /*p21.TYKU*/ wire _TYKU_LY_MATCH4n = xor2(LOVU_Y4p.qp(), _SOTA_LYC4.qn());
    /*p21.RASY*/ wire _RASY_LY_MATCH3n = xor2(LYDO_Y3p.qp(), _SALO_LYC3.qn());
    /*p21.REDA*/ wire _REDA_LY_MATCH2n = xor2(LEXA_Y2p.qp(), _SEDY_LYC2.qn());
    /*p21.TYDE*/ wire _TYDE_LY_MATCH1n = xor2(MYRO_Y1p.qp(), _VUCE_LYC1.qn());
    /*p21.RYME*/ wire _RYME_LY_MATCH0n = xor2(MUWY_Y0p.qp(), _SYRY_LYC0.qn());
    /*p21.SOVU*/ wire _SOVU_LY_MATCHA  = nor4 (_SYFU_LY_MATCH7n, _TERY_LY_MATCH6n, _TUCY_LY_MATCH5n, _TYKU_LY_MATCH4n); // def nor4
    /*p21.SUBO*/ wire _SUBO_LY_MATCHB  = nor4 (_RASY_LY_MATCH3n, _REDA_LY_MATCH2n, _TYDE_LY_MATCH1n, _RYME_LY_MATCH0n); // def nor4
    /*p21.RAPE*/ wire _RAPE_LY_MATCHn  = nand2(_SOVU_LY_MATCHA,  _SUBO_LY_MATCHB); // def nand2
    /*p21.PALY*/ wire _PALY_LY_MATCHa  = not1(_RAPE_LY_MATCHn); // def not

    /*p21.ROPO*/ _ROPO_LY_MATCH_SYNCp = dff17_B(top.clk_reg.TALU_ABCDxxxx(), top.clk_reg.WESY_SYS_RSTn(), _PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /*p22.WYLE*/ wire _WYLE_FF44n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.XENO_A01n(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /*p22.XOGY*/ wire _XOGY_FF44p = not1(_WYLE_FF44n);

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(top.ext_bus.EXT_PIN_RD_C.qp(), top.cpu_bus.CPU_PIN_RDp.qp(), top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /*p07.AJAS*/ wire AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);

    /*p23.WAFU*/ wire _WAFU_FF44_RDp = and2(ASOT_CPU_RDp, _XOGY_FF44p);
    /*p23.VARO*/ wire _VARO_FF44_RDn = not1(_WAFU_FF44_RDp);

    /*#p23.WURY*/ wire WURY_LY0n = not1(MUWY_Y0p.qp());
    /*p23.XEPO*/ wire XEPO_LY1n = not1(MYRO_Y1p.qp());
    /*p23.MYFA*/ wire MYFA_LY2n = not1(LEXA_Y2p.qp());
    /*p23.XUHY*/ wire XUHY_LY3n = not1(LYDO_Y3p.qp());
    /*p23.WATA*/ wire WATA_LY4n = not1(LOVU_Y4p.qp());
    /*p23.XAGA*/ wire XAGA_LY5n = not1(LEMA_Y5p.qp());
    /*p23.XUCE*/ wire XUCE_LY6n = not1(MATO_Y6p.qp());
    /*p23.XOWO*/ wire XOWO_LY7n = not1(LAFO_Y7p.qp());

    // FIXME ok wat. is tri6n inverting also?

    /*p23.VEGA*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_VARO_FF44_RDn, WURY_LY0n);
    /*p23.WUVA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_VARO_FF44_RDn, XEPO_LY1n);
    /*p23.LYCO*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_VARO_FF44_RDn, MYFA_LY2n);
    /*p23.WOJY*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_VARO_FF44_RDn, XUHY_LY3n);
    /*p23.VYNE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_VARO_FF44_RDn, WATA_LY4n);
    /*p23.WAMA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_VARO_FF44_RDn, XAGA_LY5n);
    /*p23.WAVO*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_VARO_FF44_RDn, XUCE_LY6n);
    /*p23.WEZE*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_VARO_FF44_RDn, XOWO_LY7n);
  }

  // FF45 LYC
  {
    /*p22.WETY*/ wire _WETY_FF45n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.XENO_A01n(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /*p22.XAYU*/ wire _XAYU_FF45p = not1(_WETY_FF45n);

    /*p23.XYLY*/ wire _XYLY_FF45_RDp = and2(top.ASOT_CPU_RDp(), _XAYU_FF45p);
    /*p23.WEKU*/ wire _WEKU_FF45_RDn = not1(_XYLY_FF45_RDp);

    /*p23.XUFA*/ wire _XUFA_FF45_WRn = and2(top.CUPA_CPU_WRp_xxxDxxxx(), _XAYU_FF45p);
    /*p23.WANE*/ wire _WANE_FF45_WRp = not1(_XUFA_FF45_WRn);

    /*p23.SYRY*/ _SYRY_LYC0 = dff9(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p23.VUCE*/ _VUCE_LYC1 = dff9(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p23.SEDY*/ _SEDY_LYC2 = dff9(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p23.SALO*/ _SALO_LYC3 = dff9(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p23.SOTA*/ _SOTA_LYC4 = dff9(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p23.VAFA*/ _VAFA_LYC5 = dff9(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p23.VEVO*/ _VEVO_LYC6 = dff9(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p23.RAHA*/ _RAHA_LYC7 = dff9(_WANE_FF45_WRp, !_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());

    /*#p23.RETU*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_WEKU_FF45_RDn, _SYRY_LYC0.qp());
    /*p23.VOJO*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_WEKU_FF45_RDn, _VUCE_LYC1.qp());
    /*p23.RAZU*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_WEKU_FF45_RDn, _SEDY_LYC2.qp());
    /*p23.REDY*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_WEKU_FF45_RDn, _SALO_LYC3.qp());
    /*p23.RACE*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_WEKU_FF45_RDn, _SOTA_LYC4.qp());
    /*#p23.VAZU*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_WEKU_FF45_RDn, _VAFA_LYC5.qp());
    /*p23.VAFE*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_WEKU_FF45_RDn, _VEVO_LYC6.qp());
    /*p23.PUFY*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_WEKU_FF45_RDn, _RAHA_LYC7.qp());
  }
}

//------------------------------------------------------------------------------
