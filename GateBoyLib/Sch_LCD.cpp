#include "GateBoyLib/Sch_LCD.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void LcdRegisters::dump(Dumper& d, const SchematicTop& top) const {
  d("\002===== LCD =====\001\n");
  d("PIX COUNT : %03d\n", top.pix_pipe.get_pix_count());
  d("LCD X     : %03d\n", get_lx());
  d("LCD Y     : %03d\n", get_ly());
  d("LYC       : %03d\n", get_lyc());
  d("\n");

  d("PIN_LCD_CLOCK   : "); top.PIN_LCD_CLOCK.dump(d); d("\n");
  d("PIN_LCD_HSYNC   : "); top.PIN_LCD_HSYNC.dump(d); d("\n");
  d("PIN_LCD_VSYNC   : "); top.PIN_LCD_VSYNC.dump(d); d("\n");
  d("PIN_LCD_DATA1   : "); top.PIN_LCD_DATA1.dump(d); d("\n");
  d("PIN_LCD_DATA0   : "); top.PIN_LCD_DATA0.dump(d); d("\n");
  d("PIN_LCD_CNTRL   : "); top.PIN_LCD_CNTRL.dump(d); d("\n");
  d("PIN_LCD_DATALCH : "); top.PIN_LCD_DATALCH.dump(d); d("\n");
  d("PIN_LCD_ALTSIGL   : "); top.PIN_LCD_ALTSIGL.dump(d); d("\n");
  d("\n");

  d("CATU_LINE_P000      %c\n", CATU_LINE_P000.c());
  d("NYPE_LINE_P002      %c\n", NYPE_LINE_P002.c());
  d("ANEL_LINE_P002      %c\n", ANEL_LINE_P002.c());
  d("RUTU_LINE_P910      %c\n", RUTU_LINE_P910.c());
  d("ATEJ_LINE_TRIGp     %c\n", ATEJ_LINE_TRIGp.c());
  d("MYTA_LINE_153p      %c\n", MYTA_LINE_153p     .c());
  d("POPU_IN_VBLANKp     %c\n", POPU_IN_VBLANKp    .c());
  d("ROPO_LY_MATCH_SYNCp %c\n", ROPO_LY_MATCH_SYNCp.c());
  d("PARU_VBLANKp_d4     %c\n", PARU_VBLANKp_d4.c());
  d("VYPU_INT_VBLANKp    %c\n", VYPU_INT_VBLANKp.c());
  d("\n");
}

//------------------------------------------------------------------------------

void LcdRegisters::tick(const SchematicTop& top) {
  /*p01.ATAR*/ wire ATAR_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not1(ATAR_VID_RSTp);

  // fires on P910 and P911
  /*p28.ABAF*/ wire ABAF_LINE_P910n = not1(CATU_LINE_P000.qp());

  // so if this is or_and, BYHA should go low on 910 and 911
  /*p28.BYHA*/ wire BYHA_VID_LINE_END_TRIGn = or_and3(ANEL_LINE_P002.qp(), ABAF_LINE_P910n, ABEZ_VID_RSTn);

  // fires on P910 and P911
  /*p28.ATEJ*/ ATEJ_LINE_TRIGp = not1(BYHA_VID_LINE_END_TRIGn);

  // -> interrupts, ppu
  /*p21.PARU*/ PARU_VBLANKp_d4 = not1(POPU_IN_VBLANKp.qn());

  /*p21.TOLU*/ wire TOLU_VBLANKn = not1(PARU_VBLANKp_d4);
  /*p21.VYPU*/ VYPU_INT_VBLANKp = not1(TOLU_VBLANKn);
}

//------------------------------------------------------------------------------

void LcdRegisters::tock(SchematicTop& top, CpuBus& cpu_bus) {
  /*p01.ALUR*/ wire ALUR_SYS_RSTn = top.clk_reg.ALUR_SYS_RSTn;
  /*p01.XAPO*/ wire XAPO_VID_RSTn = top.clk_reg.XAPO_VID_RSTn;
  /*p01.DULA*/ wire DULA_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
  /*p01.XORE*/ wire XORE_SYS_RSTp = not1(CUNU_SYS_RSTn);
  /*p01.WESY*/ wire WESY_SYS_RSTn = not1(XORE_SYS_RSTp);
  /*p01.ATAR*/ wire ATAR_VID_RSTp = not1(XAPO_VID_RSTn);
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not1(ATAR_VID_RSTp);
  /*p01.LYHA*/ wire LYHA_VID_RSTp = not1(XAPO_VID_RSTn);
  /*p01.LYFE*/ wire LYFE_VID_RSTn = not1(LYHA_VID_RSTp);

  /* p29.XUPY*/ wire XUPY_ABxxEFxx = top.clk_reg.XUPY_ABxxEFxx;
  /* p21.TALU*/ wire TALU_xxCDEFxx = top.clk_reg.TALU_xxCDEFxx;
  /* p28.AWOH*/ wire AWOH_xxCDxxGH = not1(XUPY_ABxxEFxx);
  /* p21.SONO*/ wire SONO_ABxxxxGH = not1(TALU_xxCDEFxx);

  /* p21.XYVO*/ wire XYVO_IN_VBLANKp = and2(LOVU_Y4p.qp(), LAFO_Y7p.qp()); // 128 + 16 = 144
  /* p29.ALES*/ wire ALES_IN_VBLANKn = not1(XYVO_IN_VBLANKp);
  /* p21.POPU*/ POPU_IN_VBLANKp.dff17(NYPE_LINE_P002.q17(), LYFE_VID_RSTn, XYVO_IN_VBLANKp);

  /*#p24.KEDY*/ wire KEDY_LCDC_ENn = not1(top.pix_pipe.XONA_LCDC_LCDENn.q08());

  {
    // LCD main timer, 912 phases per line

    /*p21.MUDE*/ wire MUDE_X_RSTn = nor2(RUTU_LINE_P910.qp(), LYHA_VID_RSTp);
    /*p21.SAXO*/ SAXO_X0p.dff17(TALU_xxCDEFxx,  MUDE_X_RSTn, SAXO_X0p.qn());
    /*p21.TYPO*/ TYPO_X1p.dff17(SAXO_X0p.qn(),  MUDE_X_RSTn, TYPO_X1p.qn());
    /*p21.VYZO*/ VYZO_X2p.dff17(TYPO_X1p.qn(),  MUDE_X_RSTn, VYZO_X2p.qn());
    /*p21.TELU*/ TELU_X3p.dff17(VYZO_X2p.qn(),  MUDE_X_RSTn, TELU_X3p.qn());
    /*p21.SUDE*/ SUDE_X4p.dff17(TELU_X3p.qn(),  MUDE_X_RSTn, SUDE_X4p.qn());
    /*p21.TAHA*/ TAHA_X5p.dff17(SUDE_X4p.qn(),  MUDE_X_RSTn, TAHA_X5p.qn());
    /*p21.TYRY*/ TYRY_X6p.dff17(TAHA_X5p.qn(),  MUDE_X_RSTn, TYRY_X6p.qn());

    /*p21.LAMA*/ wire LAMA_FRAME_RSTn = nor2(MYTA_LINE_153p.qp(), LYHA_VID_RSTp);
    /*p21.MUWY*/ MUWY_Y0p.dff17(RUTU_LINE_P910.qp(), LAMA_FRAME_RSTn, MUWY_Y0p.qn());
    /*p21.MYRO*/ MYRO_Y1p.dff17(MUWY_Y0p.qn(),       LAMA_FRAME_RSTn, MYRO_Y1p.qn());
    /*p21.LEXA*/ LEXA_Y2p.dff17(MYRO_Y1p.qn(),       LAMA_FRAME_RSTn, LEXA_Y2p.qn());
    /*p21.LYDO*/ LYDO_Y3p.dff17(LEXA_Y2p.qn(),       LAMA_FRAME_RSTn, LYDO_Y3p.qn());
    /*p21.LOVU*/ LOVU_Y4p.dff17(LYDO_Y3p.qn(),       LAMA_FRAME_RSTn, LOVU_Y4p.qn());
    /*p21.LEMA*/ LEMA_Y5p.dff17(LOVU_Y4p.qn(),       LAMA_FRAME_RSTn, LEMA_Y5p.qn());
    /*p21.MATO*/ MATO_Y6p.dff17(LEMA_Y5p.qn(),       LAMA_FRAME_RSTn, MATO_Y6p.qn());
    /*p21.LAFO*/ LAFO_Y7p.dff17(MATO_Y6p.qn(),       LAMA_FRAME_RSTn, LAFO_Y7p.qn());
  }

  {
    /*#p21.SANU*/ wire SANU_x113p = and4(TYRY_X6p.q17(), TAHA_X5p.q17(), SUDE_X4p.q17(), SAXO_X0p.q17()); // 113 = 64 + 32 + 16 + 1, schematic is wrong

    /*#p21.RUTU*/ RUTU_LINE_P910.dff17(SONO_ABxxxxGH, LYFE_VID_RSTn, SANU_x113p);
    /*#p21.NYPE*/ NYPE_LINE_P002.dff17(TALU_xxCDEFxx, LYFE_VID_RSTn, RUTU_LINE_P910.q17());

    /*#p21.PURE*/ wire PURE_LINE_P908n = not1(RUTU_LINE_P910.qp());
    /*#p21.SELA*/ wire SELA_LINE_P908p = not1(PURE_LINE_P908n);
    /*#p29.ABOV*/ wire ABOV_VID_LINE_P908p = and2(SELA_LINE_P908p, ALES_IN_VBLANKn);

    /*#p29.CATU*/ CATU_LINE_P000.dff17(XUPY_ABxxEFxx, ABEZ_VID_RSTn, ABOV_VID_LINE_P908p);
    /*#p28.ANEL*/ ANEL_LINE_P002.dff17(AWOH_xxCDxxGH, ABEZ_VID_RSTn, CATU_LINE_P000.q17());

    /*#p21.NOKO*/ wire NOKO_LINE_153 = and4(LAFO_Y7p.qp(), LOVU_Y4p.qp(), LYDO_Y3p.qp(), MUWY_Y0p.qp()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*#p21.MYTA*/ MYTA_LINE_153p.dff17(NYPE_LINE_P002.q16(), LYFE_VID_RSTn, NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, LCD_PIN_ALTSG = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)

    /*#p24.LOFU*/ wire LOFU_LINE_ENDn = not1(RUTU_LINE_P910.q17());
    /*#p24.LUCA*/ LUCA_LINE_EVEN .dff17(LOFU_LINE_ENDn,        LYFE_VID_RSTn, LUCA_LINE_EVEN.q16());
    /*#p21.NAPO*/ NAPO_FRAME_EVEN.dff17(POPU_IN_VBLANKp.q17(), LYFE_VID_RSTn, NAPO_FRAME_EVEN.q16());

    /*#p24.MAGU*/ wire MAGU = xor2(NAPO_FRAME_EVEN.q17(), LUCA_LINE_EVEN.q16());
    /*#p24.MECO*/ wire MECO = not1(MAGU);
    /*#p24.KEBO*/ wire KEBO = not1(MECO);
    /*#p01.UREK*/ wire UREK_DIV_07n = not1(top.tim_reg.TULU_DIV_07.q17());
    /*#p24.USEC*/ wire USEC_DIV_07p = not1(UREK_DIV_07n);

    /*#p24.KUPA*/ wire KUPA = amux2(top.pix_pipe.XONA_LCDC_LCDENn.q08(), KEBO, KEDY_LCDC_ENn, USEC_DIV_07p);
    /*#p24.KOFO*/ wire KOFO = not1(KUPA);
    top.PIN_LCD_ALTSIGL.io_pin(KOFO, KOFO);
  }

  {
    /*#p21.TOCU*/ wire TOCU_C0n = not1(SAXO_X0p.qp());
    /*#p21.VEPE*/ wire VEPE_C1n = not1(TYPO_X1p.qp());
    /* p21.VUTY*/ wire VUTY_C2n = not1(VYZO_X2p.qp());
    /* p21.VATE*/ wire VATE_C3n = not1(TELU_X3p.qp());
    /* p21.TUDA*/ wire TUDA_C4n = not1(SUDE_X4p.qp());
    /* p21.TAFY*/ wire TAFY_C5n = not1(TAHA_X5p.qp());
    /* p21.TUJU*/ wire TUJU_C6n = not1(TYRY_X6p.qp());

    /* p21.VOKU*/ wire VOKU_000n = nand7(TUJU_C6n,      TAFY_C5n,      TUDA_C4n,      VATE_C3n,      VUTY_C2n,      VEPE_C1n,      TOCU_C0n);      // 0000000 == 0
    /* p21.TOZU*/ wire TOZU_007n = nand7(TUJU_C6n,      TAFY_C5n,      TUDA_C4n,      VATE_C3n,      VYZO_X2p.qp(), TYPO_X1p.qp(), SAXO_X0p.qp()); // 0000111 == 7
    /* p21.TECE*/ wire TECE_045n = nand7(TUJU_C6n,      TAHA_X5p.qp(), TUDA_C4n,      TELU_X3p.qp(), VYZO_X2p.qp(), VEPE_C1n,      SAXO_X0p.qp()); // 0101101 == 45
    /*#p21.TEBO*/ wire TEBO_083n = nand7(TYRY_X6p.q17(), TAFY_C5n,      SUDE_X4p.q17(), VATE_C3n,      VUTY_C2n,      TYPO_X1p.q17(), SAXO_X0p.q17()); // 1010011 == 83

    /*#p21.TEGY*/ wire TEGY_LINE_STROBE = nand4(VOKU_000n, TOZU_007n, TECE_045n, TEBO_083n);
    /*#p21.SYGU*/ SYGU_LINE_STROBE.dff17(SONO_ABxxxxGH, LYFE_VID_RSTn, TEGY_LINE_STROBE);
    /*#p21.RYNO*/ wire RYNO = or2(SYGU_LINE_STROBE.q17(), RUTU_LINE_P910.q17());
    /*#p21.POGU*/ wire POGU = not1(RYNO);
    top.PIN_LCD_CNTRL.io_pin(POGU, POGU);
  }

  {
    /*#p21.PURE*/ wire PURE_LINE_ENDn = not1(RUTU_LINE_P910.q17());
    /*#p24.KASA*/ wire KASA_LINE_ENDp = not1(PURE_LINE_ENDn);
    /*#p01.UTOK*/ wire UMEK_DIV_06n   = not1(top.tim_reg.UGOT_DIV_06.q17());
    /*#p24.UMOB*/ wire UMOB_DIV_06p   = not1(UMEK_DIV_06n);

    /*#p24.KAHE*/ wire KAHE_LINE_ENDp = amux2(top.pix_pipe.XONA_LCDC_LCDENn.q08(), KASA_LINE_ENDp, KEDY_LCDC_ENn, UMOB_DIV_06p);
    /*#p24.KYMO*/ wire KYMO_LINE_ENDn = not1(KAHE_LINE_ENDp);
    top.PIN_LCD_DATALCH.io_pin(KYMO_LINE_ENDn, KYMO_LINE_ENDn);
  }

  // LCD vertical sync pin
  {
    // NERU looks a little odd, not 100% positive it's a big nor but it does make sense as one
    /*#p24.NERU*/ wire NERU_LINE_000p = nor8(LAFO_Y7p.q17(), LOVU_Y4p.q17(), LYDO_Y3p.q17(), MUWY_Y0p.q17(),
                                             MYRO_Y1p.q17(), LEXA_Y2p.q17(), LEMA_Y5p.q17(), MATO_Y6p.q17());

    /*#p24.MEDA*/ MEDA_VSYNC_OUTn.dff17(NYPE_LINE_P002.q16(), LYFE_VID_RSTn, NERU_LINE_000p);
    /*#p24.MURE*/ wire MURE_VSYNC = not1(MEDA_VSYNC_OUTn.q17());
    /*#*/ top.PIN_LCD_VSYNC.io_pin(MURE_VSYNC, MURE_VSYNC);
  }

  // ly match
  {
    /* p21.RYME*/ wire RYME_LY_MATCH0n = xor2(MUWY_Y0p.qp(), SYRY_LYC0n.q08());
    /* p21.TYDE*/ wire TYDE_LY_MATCH1n = xor2(MYRO_Y1p.qp(), VUCE_LYC1n.q08());
    /* p21.REDA*/ wire REDA_LY_MATCH2n = xor2(LEXA_Y2p.qp(), SEDY_LYC2n.q08());
    /* p21.RASY*/ wire RASY_LY_MATCH3n = xor2(LYDO_Y3p.qp(), SALO_LYC3n.q08());
    /* p21.TYKU*/ wire TYKU_LY_MATCH4n = xor2(LOVU_Y4p.qp(), SOTA_LYC4n.q08());
    /* p21.TUCY*/ wire TUCY_LY_MATCH5n = xor2(LEMA_Y5p.qp(), VAFA_LYC5n.q08());
    /* p21.TERY*/ wire TERY_LY_MATCH6n = xor2(MATO_Y6p.qp(), VEVO_LYC6n.q08());
    /* p21.SYFU*/ wire SYFU_LY_MATCH7n = xor2(LAFO_Y7p.qp(), RAHA_LYC7n.q08());

    /* p21.SOVU*/ wire SOVU_LY_MATCHA = nor4 (SYFU_LY_MATCH7n, TERY_LY_MATCH6n, TUCY_LY_MATCH5n, TYKU_LY_MATCH4n); // def nor4
    /* p21.SUBO*/ wire SUBO_LY_MATCHB = nor4 (RASY_LY_MATCH3n, REDA_LY_MATCH2n, TYDE_LY_MATCH1n, RYME_LY_MATCH0n); // def nor4
    /* p21.RAPE*/ wire RAPE_LY_MATCHn = nand2(SOVU_LY_MATCHA,  SUBO_LY_MATCHB); // def nand2
    /* p21.PALY*/ wire PALY_LY_MATCHa = not1(RAPE_LY_MATCHn); // def not

    /* p21.ROPO*/ ROPO_LY_MATCH_SYNCp.dff17(TALU_xxCDEFxx, WESY_SYS_RSTn, PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /* p07.UJYV*/ wire UJYV_CPU_RDn  = mux2n(top.UNOR_MODE_DBG2p, top.ext_bus.PIN_EXT_RDn.qn(), top.cpu_bus.PIN_CPU_RDp.qp());
    /* p07.TEDO*/ wire TEDO_CPU_RDp  = not1(UJYV_CPU_RDn);
    /* p07.AJAS*/ wire AJAS_CPU_RDn  = not1(TEDO_CPU_RDp);
    /* p07.ASOT*/ wire ASOT_CPU_RDp  = not1(AJAS_CPU_RDn);

    /* p22.WYLE*/ wire WYLE_FF44n    = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.XENO_A01n(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /* p22.XOGY*/ wire XOGY_FF44p    = not1(WYLE_FF44n);
    /* p23.WAFU*/ wire WAFU_FF44_RDp = and2(ASOT_CPU_RDp, XOGY_FF44p);
    /* p23.VARO*/ wire VARO_FF44_RDn = not1(WAFU_FF44_RDp);

    /*#p23.WURY*/ wire WURY_LY0n = not1(MUWY_Y0p.qp());
    /* p23.XEPO*/ wire XEPO_LY1n = not1(MYRO_Y1p.qp());
    /* p23.MYFA*/ wire MYFA_LY2n = not1(LEXA_Y2p.qp());
    /* p23.XUHY*/ wire XUHY_LY3n = not1(LYDO_Y3p.qp());
    /* p23.WATA*/ wire WATA_LY4n = not1(LOVU_Y4p.qp());
    /* p23.XAGA*/ wire XAGA_LY5n = not1(LEMA_Y5p.qp());
    /* p23.XUCE*/ wire XUCE_LY6n = not1(MATO_Y6p.qp());
    /* p23.XOWO*/ wire XOWO_LY7n = not1(LAFO_Y7p.qp());

    /* p23.VEGA*/ cpu_bus.BUS_CPU_D0p.tri_6nn(VARO_FF44_RDn, WURY_LY0n);
    /* p23.WUVA*/ cpu_bus.BUS_CPU_D1p.tri_6nn(VARO_FF44_RDn, XEPO_LY1n);
    /* p23.LYCO*/ cpu_bus.BUS_CPU_D2p.tri_6nn(VARO_FF44_RDn, MYFA_LY2n);
    /* p23.WOJY*/ cpu_bus.BUS_CPU_D3p.tri_6nn(VARO_FF44_RDn, XUHY_LY3n);
    /* p23.VYNE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(VARO_FF44_RDn, WATA_LY4n);
    /* p23.WAMA*/ cpu_bus.BUS_CPU_D5p.tri_6nn(VARO_FF44_RDn, XAGA_LY5n);
    /* p23.WAVO*/ cpu_bus.BUS_CPU_D6p.tri_6nn(VARO_FF44_RDn, XUCE_LY6n);
    /* p23.WEZE*/ cpu_bus.BUS_CPU_D7p.tri_6nn(VARO_FF44_RDn, XOWO_LY7n);
  }

  // FF45 LYC
  {
    /* p22.WETY*/ wire WETY_FF45n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.XENO_A01n(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /* p22.XAYU*/ wire XAYU_FF45p = not1(WETY_FF45n);

    /*p07.AJAS*/ wire AJAS_CPU_RDn = not1(top.TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);
    /* p23.XYLY*/ wire XYLY_FF45_RDp = and2(ASOT_CPU_RDp, XAYU_FF45p);
    /* p23.WEKU*/ wire WEKU_FF45_RDn = not1(XYLY_FF45_RDp);

    /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH = not1(top.TAPU_CPU_WRp_xxxxEFGx);
    /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx = not1(DYKY_CPU_WRn_ABCDxxxH);
    /* p23.XUFA*/ wire XUFA_FF45_WRn = and2(CUPA_CPU_WRp_xxxxEFGx, XAYU_FF45p);
    /* p23.WANE*/ wire WANE_FF45_WRp = not1(XUFA_FF45_WRn);

    /* p23.SYRY*/ SYRY_LYC0n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D0p.qp());
    /* p23.VUCE*/ VUCE_LYC1n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D1p.qp());
    /* p23.SEDY*/ SEDY_LYC2n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D2p.qp());
    /* p23.SALO*/ SALO_LYC3n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D3p.qp());
    /* p23.SOTA*/ SOTA_LYC4n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D4p.qp());
    /* p23.VAFA*/ VAFA_LYC5n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D5p.qp());
    /* p23.VEVO*/ VEVO_LYC6n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D6p.qp());
    /* p23.RAHA*/ RAHA_LYC7n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*#p23.RETU*/ cpu_bus.BUS_CPU_D0p.tri_6nn(WEKU_FF45_RDn, SYRY_LYC0n.q09p());
    /* p23.VOJO*/ cpu_bus.BUS_CPU_D1p.tri_6nn(WEKU_FF45_RDn, VUCE_LYC1n.q09p());
    /* p23.RAZU*/ cpu_bus.BUS_CPU_D2p.tri_6nn(WEKU_FF45_RDn, SEDY_LYC2n.q09p());
    /* p23.REDY*/ cpu_bus.BUS_CPU_D3p.tri_6nn(WEKU_FF45_RDn, SALO_LYC3n.q09p());
    /* p23.RACE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(WEKU_FF45_RDn, SOTA_LYC4n.q09p());
    /*#p23.VAZU*/ cpu_bus.BUS_CPU_D5p.tri_6nn(WEKU_FF45_RDn, VAFA_LYC5n.q09p());
    /* p23.VAFE*/ cpu_bus.BUS_CPU_D6p.tri_6nn(WEKU_FF45_RDn, VEVO_LYC6n.q09p());
    /* p23.PUFY*/ cpu_bus.BUS_CPU_D7p.tri_6nn(WEKU_FF45_RDn, RAHA_LYC7n.q09p());
  }
}

//------------------------------------------------------------------------------
