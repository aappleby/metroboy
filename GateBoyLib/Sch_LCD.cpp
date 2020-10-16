#include "GateBoyLib/Sch_LCD.h"
#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

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
  d("PIN_LCD_DATALCH : "); top.PIN_LCD_LATCH.dump(d); d("\n");
  d("PIN_LCD_ALTSIGL   : "); top.PIN_LCD_FLIPS.dump(d); d("\n");
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
  // fires on P910 and P911
  /*p28.ABAF*/ wire ABAF_LINE_P910n = not1(CATU_LINE_P000.qp17());

  // so if this is or_and, BYHA should go low on 910 and 911
  /*p28.BYHA*/ wire BYHA_VID_LINE_END_TRIGn = or_and3(ANEL_LINE_P002.qp17(), ABAF_LINE_P910n, top.clk_reg.ABEZ_VID_RSTn);

  // fires on P910 and P911
  /*p28.ATEJ*/ ATEJ_LINE_TRIGp = not1(BYHA_VID_LINE_END_TRIGn);

  // -> interrupts, ppu
  /*#p21.PARU*/ PARU_VBLANKp_d4 = not1(POPU_IN_VBLANKp.qn16());

  /*#p21.TOLU*/ wire TOLU_VBLANKn = not1(PARU_VBLANKp_d4);
  /*#p21.VYPU*/ VYPU_INT_VBLANKp = not1(TOLU_VBLANKn);

  /*#p21.PURE*/ PURE_LINE_P908n = not1(RUTU_LINE_P910.qp17());
  /*#p21.SELA*/ SELA_LINE_P908p = not1(PURE_LINE_P908n);
}

//------------------------------------------------------------------------------

void LcdRegisters::tock(SchematicTop& top, CpuBus& cpu_bus) {
  /*#p01.XORE*/ wire XORE_SYS_RSTp = not1(top.clk_reg.CUNU_SYS_RSTn);
  /*#p01.WESY*/ wire WESY_SYS_RSTn = not1(XORE_SYS_RSTp);
  /*#p01.ATAR*/ wire ATAR_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn = not1(ATAR_VID_RSTp);
  /* p01.LYHA*/ wire LYHA_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);

  /*#p29.XUPY*/ wire XUPY_ABxxEFxx = top.clk_reg.XUPY_ABxxEFxx;
  /*#p21.TALU*/ wire TALU_xxCDEFxx = top.clk_reg.TALU_xxCDEFxx;
  /*#p28.AWOH*/ wire AWOH_xxCDxxGH = not1(XUPY_ABxxEFxx);
  /*#p21.SONO*/ wire SONO_ABxxxxGH = not1(TALU_xxCDEFxx);

  /*#p21.XYVO*/ wire _XYVO_IN_VBLANKp = and2(LOVU_Y4p.qp17(), LAFO_Y7p.qp17()); // 128 + 16 = 144
  /*#p29.ALES*/ wire _ALES_IN_VBLANKn = not1(_XYVO_IN_VBLANKp);

  /* p01.LYFE*/ wire _LYFE_LCD_RSTn = not1(LYHA_VID_RSTp);
  /*#p21.POPU*/ POPU_IN_VBLANKp.dff17(NYPE_LINE_P002.qp17(), _LYFE_LCD_RSTn, _XYVO_IN_VBLANKp);

  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(top.pix_pipe.XONA_LCDC_LCDENn.qn08());

  {
    // LCD main timer, 912 phases per line

    /*#p21.MUDE*/ wire _MUDE_X_RSTn = nor2(RUTU_LINE_P910.qp17(), LYHA_VID_RSTp);
    /*#p21.SAXO*/ SAXO_X0p.dff17(TALU_xxCDEFxx,   _MUDE_X_RSTn, SAXO_X0p.qn16());
    /*#p21.TYPO*/ TYPO_X1p.dff17(SAXO_X0p.qn16(), _MUDE_X_RSTn, TYPO_X1p.qn16());
    /*#p21.VYZO*/ VYZO_X2p.dff17(TYPO_X1p.qn16(), _MUDE_X_RSTn, VYZO_X2p.qn16());
    /*#p21.TELU*/ TELU_X3p.dff17(VYZO_X2p.qn16(), _MUDE_X_RSTn, TELU_X3p.qn16());
    /*#p21.SUDE*/ SUDE_X4p.dff17(TELU_X3p.qn16(), _MUDE_X_RSTn, SUDE_X4p.qn16());
    /*#p21.TAHA*/ TAHA_X5p.dff17(SUDE_X4p.qn16(), _MUDE_X_RSTn, TAHA_X5p.qn16());
    /*#p21.TYRY*/ TYRY_X6p.dff17(TAHA_X5p.qn16(), _MUDE_X_RSTn, TYRY_X6p.qn16());

    /*#p21.LAMA*/ wire _LAMA_FRAME_RSTn = nor2(MYTA_LINE_153p.qp17(), LYHA_VID_RSTp);
    /*#p21.MUWY*/ MUWY_Y0p.dff17(RUTU_LINE_P910.qp17(), _LAMA_FRAME_RSTn, MUWY_Y0p.qn16());
    /*#p21.MYRO*/ MYRO_Y1p.dff17(MUWY_Y0p.qn16(),       _LAMA_FRAME_RSTn, MYRO_Y1p.qn16());
    /*#p21.LEXA*/ LEXA_Y2p.dff17(MYRO_Y1p.qn16(),       _LAMA_FRAME_RSTn, LEXA_Y2p.qn16());
    /*#p21.LYDO*/ LYDO_Y3p.dff17(LEXA_Y2p.qn16(),       _LAMA_FRAME_RSTn, LYDO_Y3p.qn16());
    /*#p21.LOVU*/ LOVU_Y4p.dff17(LYDO_Y3p.qn16(),       _LAMA_FRAME_RSTn, LOVU_Y4p.qn16());
    /*#p21.LEMA*/ LEMA_Y5p.dff17(LOVU_Y4p.qn16(),       _LAMA_FRAME_RSTn, LEMA_Y5p.qn16());
    /*#p21.MATO*/ MATO_Y6p.dff17(LEMA_Y5p.qn16(),       _LAMA_FRAME_RSTn, MATO_Y6p.qn16());
    /*#p21.LAFO*/ LAFO_Y7p.dff17(MATO_Y6p.qn16(),       _LAMA_FRAME_RSTn, LAFO_Y7p.qn16());
  }

  {
    /*#p21.SANU*/ wire _SANU_x113p = and4(TYRY_X6p.qp17(), TAHA_X5p.qp17(), SUDE_X4p.qp17(), SAXO_X0p.qp17()); // 113 = 64 + 32 + 16 + 1, schematic is wrong

    /*#p21.RUTU*/ RUTU_LINE_P910.dff17(SONO_ABxxxxGH, _LYFE_LCD_RSTn, _SANU_x113p);
    /*#p21.NYPE*/ NYPE_LINE_P002.dff17(TALU_xxCDEFxx, _LYFE_LCD_RSTn, RUTU_LINE_P910.qp17());

    /*#p29.ABOV*/ wire _ABOV_VID_LINE_P908p = and2(SELA_LINE_P908p, _ALES_IN_VBLANKn);

    /*#p29.CATU*/ CATU_LINE_P000.dff17(XUPY_ABxxEFxx, ABEZ_VID_RSTn, _ABOV_VID_LINE_P908p);
    /*#p28.ANEL*/ ANEL_LINE_P002.dff17(AWOH_xxCDxxGH, ABEZ_VID_RSTn, CATU_LINE_P000.qp17());

    /*#p21.NOKO*/ wire _NOKO_LINE_153 = and4(LAFO_Y7p.qp17(), LOVU_Y4p.qp17(), LYDO_Y3p.qp17(), MUWY_Y0p.qp17()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*#p21.MYTA*/ MYTA_LINE_153p.dff17(NYPE_LINE_P002.qn16(), _LYFE_LCD_RSTn, _NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, LCD_PIN_ALTSG = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)

    /*#p24.LOFU*/ wire _LOFU_LINE_ENDn = not1(RUTU_LINE_P910.qp17());
    /*#p24.LUCA*/ LUCA_LINE_EVEN .dff17(_LOFU_LINE_ENDn,        _LYFE_LCD_RSTn, LUCA_LINE_EVEN.qn16());
    /*#p21.NAPO*/ NAPO_FRAME_EVEN.dff17(POPU_IN_VBLANKp.qp17(), _LYFE_LCD_RSTn, NAPO_FRAME_EVEN.qn16());

    /*#p24.MAGU*/ wire _MAGU = xor2(NAPO_FRAME_EVEN.qp17(), LUCA_LINE_EVEN.qn16());
    /*#p24.MECO*/ wire _MECO = not1(_MAGU);
    /*#p24.KEBO*/ wire _KEBO = not1(_MECO);

    /*#p24.USEC*/ wire _USEC_DIV_07p = not1(top.tim_reg.UREK_DIV_07n);
    /*#p24.KUPA*/ wire _KUPA = amux2(top.pix_pipe.XONA_LCDC_LCDENn.qn08(), _KEBO, _KEDY_LCDC_ENn, _USEC_DIV_07p);
    /*#p24.KOFO*/ wire _KOFO = not1(_KUPA);

    top.PIN_LCD_FLIPS.io_pin(_KOFO, _KOFO);
  }

  {
    /*#p21.TOCU*/ wire _TOCU_C0n = not1(SAXO_X0p.qp17());
    /*#p21.VEPE*/ wire _VEPE_C1n = not1(TYPO_X1p.qp17());
    /* p21.VUTY*/ wire _VUTY_C2n = not1(VYZO_X2p.qp17());
    /* p21.VATE*/ wire _VATE_C3n = not1(TELU_X3p.qp17());
    /* p21.TUDA*/ wire _TUDA_C4n = not1(SUDE_X4p.qp17());
    /* p21.TAFY*/ wire _TAFY_C5n = not1(TAHA_X5p.qp17());
    /* p21.TUJU*/ wire _TUJU_C6n = not1(TYRY_X6p.qp17());

    /* p21.VOKU*/ wire _VOKU_000n = nand7(_TUJU_C6n,       _TAFY_C5n,       _TUDA_C4n,       _VATE_C3n,       _VUTY_C2n,       _VEPE_C1n,       _TOCU_C0n);       // 0000000 == 0
    /* p21.TOZU*/ wire _TOZU_007n = nand7(_TUJU_C6n,       _TAFY_C5n,       _TUDA_C4n,       _VATE_C3n,       VYZO_X2p.qp17(), TYPO_X1p.qp17(), SAXO_X0p.qp17()); // 0000111 == 7
    /* p21.TECE*/ wire _TECE_045n = nand7(_TUJU_C6n,       TAHA_X5p.qp17(), _TUDA_C4n,       TELU_X3p.qp17(), VYZO_X2p.qp17(), _VEPE_C1n,       SAXO_X0p.qp17()); // 0101101 == 45
    /*#p21.TEBO*/ wire _TEBO_083n = nand7(TYRY_X6p.qp17(), _TAFY_C5n,       SUDE_X4p.qp17(), _VATE_C3n,       _VUTY_C2n,       TYPO_X1p.qp17(), SAXO_X0p.qp17()); // 1010011 == 83

    /*#p21.TEGY*/ wire _TEGY_LINE_STROBE = nand4(_VOKU_000n, _TOZU_007n, _TECE_045n, _TEBO_083n);
    /*#p21.SYGU*/ SYGU_LINE_STROBE.dff17(SONO_ABxxxxGH, _LYFE_LCD_RSTn, _TEGY_LINE_STROBE);

    /*#p21.RYNO*/ wire _RYNO = or2(SYGU_LINE_STROBE.qp17(), RUTU_LINE_P910.qp17());
    /*#p21.POGU*/ wire _POGU = not1(_RYNO);

    top.PIN_LCD_CNTRL.io_pin(_POGU, _POGU);
  }

  {

    /*#p24.KASA*/ wire _KASA_LINE_ENDp = not1(PURE_LINE_P908n);

    /*#p24.UMOB*/ wire UMOB_DIV_06p = not1(top.tim_reg.UMEK_DIV_06n);
    /*#p24.KAHE*/ wire _KAHE_LINE_ENDp = amux2(top.pix_pipe.XONA_LCDC_LCDENn.qn08(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, UMOB_DIV_06p);
    /*#p24.KYMO*/ wire _KYMO_LINE_ENDn = not1(_KAHE_LINE_ENDp);
    top.PIN_LCD_LATCH.io_pin(_KYMO_LINE_ENDn, _KYMO_LINE_ENDn);
  }

  // LCD vertical sync pin
  {
    // NERU looks a little odd, not 100% positive it's a big nor but it does make sense as one
    /*#p24.NERU*/ wire _NERU_LINE_000p = nor8(LAFO_Y7p.qp17(), LOVU_Y4p.qp17(), LYDO_Y3p.qp17(), MUWY_Y0p.qp17(),
                                              MYRO_Y1p.qp17(), LEXA_Y2p.qp17(), LEMA_Y5p.qp17(), MATO_Y6p.qp17());

    /*#p24.MEDA*/ MEDA_VSYNC_OUTn.dff17(NYPE_LINE_P002.qn16(), _LYFE_LCD_RSTn, _NERU_LINE_000p);
    /*#p24.MURE*/ wire _MURE_VSYNC = not1(MEDA_VSYNC_OUTn.qp17());

    /*#*/ top.PIN_LCD_VSYNC.io_pin(_MURE_VSYNC, _MURE_VSYNC);
  }

  // ly match
  {
    /* p21.RYME*/ wire _RYME_LY_MATCH0n = xor2(MUWY_Y0p.qp17(), SYRY_LYC0n.qn08());
    /* p21.TYDE*/ wire _TYDE_LY_MATCH1n = xor2(MYRO_Y1p.qp17(), VUCE_LYC1n.qn08());
    /* p21.REDA*/ wire _REDA_LY_MATCH2n = xor2(LEXA_Y2p.qp17(), SEDY_LYC2n.qn08());
    /* p21.RASY*/ wire _RASY_LY_MATCH3n = xor2(LYDO_Y3p.qp17(), SALO_LYC3n.qn08());
    /* p21.TYKU*/ wire _TYKU_LY_MATCH4n = xor2(LOVU_Y4p.qp17(), SOTA_LYC4n.qn08());
    /* p21.TUCY*/ wire _TUCY_LY_MATCH5n = xor2(LEMA_Y5p.qp17(), VAFA_LYC5n.qn08());
    /* p21.TERY*/ wire _TERY_LY_MATCH6n = xor2(MATO_Y6p.qp17(), VEVO_LYC6n.qn08());
    /* p21.SYFU*/ wire _SYFU_LY_MATCH7n = xor2(LAFO_Y7p.qp17(), RAHA_LYC7n.qn08());

    /* p21.SOVU*/ wire _SOVU_LY_MATCHA = nor4 (_SYFU_LY_MATCH7n, _TERY_LY_MATCH6n, _TUCY_LY_MATCH5n, _TYKU_LY_MATCH4n); // def nor4
    /* p21.SUBO*/ wire _SUBO_LY_MATCHB = nor4 (_RASY_LY_MATCH3n, _REDA_LY_MATCH2n, _TYDE_LY_MATCH1n, _RYME_LY_MATCH0n); // def nor4
    /* p21.RAPE*/ wire _RAPE_LY_MATCHn = nand2(_SOVU_LY_MATCHA,  _SUBO_LY_MATCHB); // def nand2
    /* p21.PALY*/ wire _PALY_LY_MATCHa = not1(_RAPE_LY_MATCHn); // def not

    /*#p21.ROPO*/ ROPO_LY_MATCH_SYNCp.dff17(TALU_xxCDEFxx, WESY_SYS_RSTn, _PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /* p22.WYLE*/ wire _WYLE_FF44n    = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.XENO_A01n(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /* p22.XOGY*/ wire _XOGY_FF44p    = not1(_WYLE_FF44n);
    /* p23.WAFU*/ wire _WAFU_FF44_RDp = and2(top.ASOT_CPU_RDp, _XOGY_FF44p);
    /* p23.VARO*/ wire _VARO_FF44_RDn = not1(_WAFU_FF44_RDp);

    /*#p23.WURY*/ wire _WURY_LY0n = not1(MUWY_Y0p.qp17());
    /* p23.XEPO*/ wire _XEPO_LY1n = not1(MYRO_Y1p.qp17());
    /* p23.MYFA*/ wire _MYFA_LY2n = not1(LEXA_Y2p.qp17());
    /* p23.XUHY*/ wire _XUHY_LY3n = not1(LYDO_Y3p.qp17());
    /* p23.WATA*/ wire _WATA_LY4n = not1(LOVU_Y4p.qp17());
    /* p23.XAGA*/ wire _XAGA_LY5n = not1(LEMA_Y5p.qp17());
    /* p23.XUCE*/ wire _XUCE_LY6n = not1(MATO_Y6p.qp17());
    /* p23.XOWO*/ wire _XOWO_LY7n = not1(LAFO_Y7p.qp17());

    /* p23.VEGA*/ cpu_bus.BUS_CPU_D0p.tri_6nn(_VARO_FF44_RDn, _WURY_LY0n);
    /* p23.WUVA*/ cpu_bus.BUS_CPU_D1p.tri_6nn(_VARO_FF44_RDn, _XEPO_LY1n);
    /* p23.LYCO*/ cpu_bus.BUS_CPU_D2p.tri_6nn(_VARO_FF44_RDn, _MYFA_LY2n);
    /* p23.WOJY*/ cpu_bus.BUS_CPU_D3p.tri_6nn(_VARO_FF44_RDn, _XUHY_LY3n);
    /* p23.VYNE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(_VARO_FF44_RDn, _WATA_LY4n);
    /* p23.WAMA*/ cpu_bus.BUS_CPU_D5p.tri_6nn(_VARO_FF44_RDn, _XAGA_LY5n);
    /* p23.WAVO*/ cpu_bus.BUS_CPU_D6p.tri_6nn(_VARO_FF44_RDn, _XUCE_LY6n);
    /* p23.WEZE*/ cpu_bus.BUS_CPU_D7p.tri_6nn(_VARO_FF44_RDn, _XOWO_LY7n);
  }

  // FF45 LYC
  {
    /* p22.WETY*/ wire _WETY_FF45n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.XENO_A01n(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /* p22.XAYU*/ wire _XAYU_FF45p = not1(_WETY_FF45n);

    /* p23.XYLY*/ wire _XYLY_FF45_RDp = and2(top.ASOT_CPU_RDp, _XAYU_FF45p);
    /* p23.WEKU*/ wire _WEKU_FF45_RDn = not1(_XYLY_FF45_RDp);

    /* p23.XUFA*/ wire _XUFA_FF45_WRn = and2(top.CUPA_CPU_WRp_xxxxEFGx, _XAYU_FF45p);
    /* p23.WANE*/ wire _WANE_FF45_WRp = not1(_XUFA_FF45_WRn);

    /* p23.SYRY*/ SYRY_LYC0n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D0p.qp());
    /* p23.VUCE*/ VUCE_LYC1n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D1p.qp());
    /* p23.SEDY*/ SEDY_LYC2n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D2p.qp());
    /* p23.SALO*/ SALO_LYC3n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D3p.qp());
    /* p23.SOTA*/ SOTA_LYC4n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D4p.qp());
    /* p23.VAFA*/ VAFA_LYC5n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D5p.qp());
    /* p23.VEVO*/ VEVO_LYC6n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D6p.qp());
    /* p23.RAHA*/ RAHA_LYC7n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*#p23.RETU*/ cpu_bus.BUS_CPU_D0p.tri_6nn(_WEKU_FF45_RDn, SYRY_LYC0n.qp09());
    /* p23.VOJO*/ cpu_bus.BUS_CPU_D1p.tri_6nn(_WEKU_FF45_RDn, VUCE_LYC1n.qp09());
    /* p23.RAZU*/ cpu_bus.BUS_CPU_D2p.tri_6nn(_WEKU_FF45_RDn, SEDY_LYC2n.qp09());
    /* p23.REDY*/ cpu_bus.BUS_CPU_D3p.tri_6nn(_WEKU_FF45_RDn, SALO_LYC3n.qp09());
    /* p23.RACE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(_WEKU_FF45_RDn, SOTA_LYC4n.qp09());
    /*#p23.VAZU*/ cpu_bus.BUS_CPU_D5p.tri_6nn(_WEKU_FF45_RDn, VAFA_LYC5n.qp09());
    /* p23.VAFE*/ cpu_bus.BUS_CPU_D6p.tri_6nn(_WEKU_FF45_RDn, VEVO_LYC6n.qp09());
    /* p23.PUFY*/ cpu_bus.BUS_CPU_D7p.tri_6nn(_WEKU_FF45_RDn, RAHA_LYC7n.qp09());
  }
}

//------------------------------------------------------------------------------
