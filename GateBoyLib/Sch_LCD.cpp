#include "GateBoyLib/Sch_LCD.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

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
  d("!LCD_PIN_DATA1n %c\n", top.LCD_PIN_DATA1n.cn());
  d("!LCD_PIN_DATA0n %c\n", top.LCD_PIN_DATA0n.cn());
  d("\n");
  d("!LCD_PIN_CNTRL %c\n", top.LCD_PIN_CNTRL.cn());
  d("!LCD_PIN_LATCH %c\n", top.LCD_PIN_LATCH.cn());
  d("!LCD_PIN_ALTSG %c\n", top.LCD_PIN_ALTSG.cn());
  d("\n");

  d("NYPE_LINE_P000          %c\n", NYPE_LINE_P000.c());
  d("ANEL_LINE_P000          %c\n", ANEL_LINE_P000.c());
  d("RUTU_LINE_P908          %c\n", RUTU_LINE_P908.c());
  d("CATU_LINE_P910          %c\n", CATU_LINE_P910.c());
  d("BYHA_VID_LINE_END_TRIGn %d\n", BYHA_VID_LINE_END_TRIGn);
  d("ATEJ_VID_LINE_END_TRIGp %d\n", ATEJ_VID_LINE_END_TRIGp);
  d("\n");

  d("MYTA_LINE_153p      %c\n", MYTA_LINE_153p     .c());
  d("POPU_IN_VBLANKp     %c\n", POPU_IN_VBLANKp    .c());
  d("SYGU_LINE_STROBE    %c\n", SYGU_LINE_STROBE   .c());
  d("MEDA_VSYNC_OUTn     %c\n", MEDA_VSYNC_OUTn    .c());
  d("LUCA_LINE_EVEN      %c\n", LUCA_LINE_EVEN     .c());
  d("NAPO_FRAME_EVEN     %c\n", NAPO_FRAME_EVEN    .c());
  d("ROPO_LY_MATCH_SYNCp %c\n", ROPO_LY_MATCH_SYNCp.c());
  d("\n");

  d("PARU_VBLANKp_d4         %c\n", PARU_VBLANKp_d4.c());
  d("BYHA_VID_LINE_END_TRIGn %c\n", BYHA_VID_LINE_END_TRIGn.c());
  d("ATEJ_VID_LINE_END_TRIGp %c\n", ATEJ_VID_LINE_END_TRIGp.c());
  d("VYPU_INT_VBLANKp        %c\n", VYPU_INT_VBLANKp.c());
  d("\n");
}

//------------------------------------------------------------------------------

void LcdRegisters::tick(const SchematicTop& top) {
  /*p01.ATAR*/ wire ATAR_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not1(ATAR_VID_RSTp);

  // fires on P910 and P911
  /*p28.ABAF*/ wire _ABAF_LINE_P910n = not1(CATU_LINE_P910.qp());
    
  // so if this is or_and, BYHA should go low on 910 and 911
  /*p28.BYHA*/ BYHA_VID_LINE_END_TRIGn = or_and3(ANEL_LINE_P000.qp(), _ABAF_LINE_P910n, ABEZ_VID_RSTn);

  // fires on P910 and P911
  /*p28.ATEJ*/ ATEJ_VID_LINE_END_TRIGp = not1(BYHA_VID_LINE_END_TRIGn);

  // -> interrupts, ppu
  /*p21.PARU*/ PARU_VBLANKp_d4 = not1(POPU_IN_VBLANKp.qn());

  /*p21.TOLU*/ wire TOLU_VBLANKn = not1(PARU_VBLANKp_d4);
  /*p21.VYPU*/ VYPU_INT_VBLANKp = not1(TOLU_VBLANKn);
}

//------------------------------------------------------------------------------

void LcdRegisters::tock(SchematicTop& top, CpuBus& cpu_bus) {
  /*p01.DULA*/ wire DULA_SYS_RSTp = not1(top.clk_reg.ALUR_SYS_RSTn);
  /*p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
  /*p01.XORE*/ wire XORE_SYS_RSTp = not1(CUNU_SYS_RSTn);
  /*p01.WESY*/ wire WESY_SYS_RSTn = not1(XORE_SYS_RSTp);
  /*p01.ATAR*/ wire ATAR_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not1(ATAR_VID_RSTp);
  /*p01.LYHA*/ wire LYHA_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /*p01.LYFE*/ wire LYFE_VID_RSTn = not1(LYHA_VID_RSTp);

  /* p21.XYVO*/ wire XYVO_IN_VBLANKp = and2(LOVU_Y4p.qp(), LAFO_Y7p.qp()); // 128 + 16 = 144
  /* p29.ALES*/ wire ALES_IN_VBLANKn = not1(XYVO_IN_VBLANKp);
  /* p21.POPU*/ POPU_IN_VBLANKp = dff17_AB(NYPE_LINE_P000.qp(), LYFE_VID_RSTn, XYVO_IN_VBLANKp);

  /*#p24.KEDY*/ wire KEDY_LCDC_ENn = not1(top.pix_pipe.XONA_LCDC_ENn.qn());

  /*p28.AWOH*/ wire AWOH_ABxxEFxx = not1(top.clk_reg.XUPY_xxCDxxGH);
  /*p21.SONO*/ wire SONO_xxxxEFGH = not1(top.clk_reg.TALU_ABCDxxxx);

  {
    // LCD main timer, 912 phases per line

    /*p 21.MUDE*/ wire MUDE_X_RSTn = nor2(RUTU_LINE_P908.qp(), LYHA_VID_RSTp);
    /*p 21.SAXO*/ SAXO_X0p = dff17_AB(top.clk_reg.TALU_ABCDxxxx,  MUDE_X_RSTn, SAXO_X0p.qn());
    /*p 21.TYPO*/ TYPO_X1p = dff17_AB(SAXO_X0p.qn(),                MUDE_X_RSTn, TYPO_X1p.qn());
    /*p 21.VYZO*/ VYZO_X2p = dff17_AB(TYPO_X1p.qn(),                MUDE_X_RSTn, VYZO_X2p.qn());
    /*p 21.TELU*/ TELU_X3p = dff17_AB(VYZO_X2p.qn(),                MUDE_X_RSTn, TELU_X3p.qn());
    /*p 21.SUDE*/ SUDE_X4p = dff17_AB(TELU_X3p.qn(),                MUDE_X_RSTn, SUDE_X4p.qn());
    /*p 21.TAHA*/ TAHA_X5p = dff17_AB(SUDE_X4p.qn(),                MUDE_X_RSTn, TAHA_X5p.qn());
    /*p 21.TYRY*/ TYRY_X6p = dff17_AB(TAHA_X5p.qn(),                MUDE_X_RSTn, TYRY_X6p.qn());
        
    /*p 21.LAMA*/ wire LAMA_FRAME_RSTn = nor2(MYTA_LINE_153p.qp(), LYHA_VID_RSTp);
    /*p 21.MUWY*/ MUWY_Y0p = dff17_AB(RUTU_LINE_P908.qp(),  LAMA_FRAME_RSTn, MUWY_Y0p.qn());
    /*p 21.MYRO*/ MYRO_Y1p = dff17_AB(MUWY_Y0p.qn(),         LAMA_FRAME_RSTn, MYRO_Y1p.qn());
    /*p 21.LEXA*/ LEXA_Y2p = dff17_AB(MYRO_Y1p.qn(),         LAMA_FRAME_RSTn, LEXA_Y2p.qn());
    /*p 21.LYDO*/ LYDO_Y3p = dff17_AB(LEXA_Y2p.qn(),         LAMA_FRAME_RSTn, LYDO_Y3p.qn());
    /*p 21.LOVU*/ LOVU_Y4p = dff17_AB(LYDO_Y3p.qn(),         LAMA_FRAME_RSTn, LOVU_Y4p.qn());
    /*p 21.LEMA*/ LEMA_Y5p = dff17_AB(LOVU_Y4p.qn(),         LAMA_FRAME_RSTn, LEMA_Y5p.qn());
    /*p 21.MATO*/ MATO_Y6p = dff17_AB(LEMA_Y5p.qn(),         LAMA_FRAME_RSTn, MATO_Y6p.qn());
    /*p 21.LAFO*/ LAFO_Y7p = dff17_AB(MATO_Y6p.qn(),         LAMA_FRAME_RSTn, LAFO_Y7p.qn());
  }

  {
    /*#p21.SANU*/ wire SANU_x113p = and4(TYRY_X6p.qp(), TAHA_X5p.qp(), SUDE_X4p.qp(), SAXO_X0p.qp()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /* p21.RUTU*/ RUTU_LINE_P908 = dff17_B (SONO_xxxxEFGH, LYFE_VID_RSTn, SANU_x113p);
    /* p21.NYPE*/ NYPE_LINE_P000 = dff17_AB(top.clk_reg.TALU_ABCDxxxx, LYFE_VID_RSTn, RUTU_LINE_P908.qp());

    /*#p21.PURE*/ wire PURE_LINE_P908n = not1(RUTU_LINE_P908.qp());
    /*#p21.SELA*/ wire SELA_LINE_P908p = not1(PURE_LINE_P908n);
    /*#p29.ABOV*/ wire ABOV_LINE_P908p = and2(SELA_LINE_P908p, ALES_IN_VBLANKn);
    /* p29.CATU*/ CATU_LINE_P910 = dff17_B(top.clk_reg.XUPY_xxCDxxGH, ABEZ_VID_RSTn, ABOV_LINE_P908p);
    /* p28.ANEL*/ ANEL_LINE_P000 = dff17_B(AWOH_ABxxEFxx, ABEZ_VID_RSTn, CATU_LINE_P910.qp());

    /*#p21.NOKO*/ wire NOKO_LINE_153 = and4(LAFO_Y7p.qp(), LOVU_Y4p.qp(), LYDO_Y3p.qp(), MUWY_Y0p.qp()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /* p21.MYTA*/ MYTA_LINE_153p = dff17_B(NYPE_LINE_P000.qp(), LYFE_VID_RSTn, NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, LCD_PIN_ALTSG = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)
    
    /* p24.LOFU*/ wire LOFU_LINE_ENDn = not1(RUTU_LINE_P908.qp());
    /* p24.LUCA*/ LUCA_LINE_EVEN = dff17_A(LOFU_LINE_ENDn,      LYFE_VID_RSTn, LUCA_LINE_EVEN.qn());
    /* p21.NAPO*/ NAPO_FRAME_EVEN = dff17_AB(POPU_IN_VBLANKp.qp(), LYFE_VID_RSTn, !NAPO_FRAME_EVEN.qp());

    /* p24.MAGU*/ wire MAGU = xor2(NAPO_FRAME_EVEN.qp(), LUCA_LINE_EVEN.qn());
    /* p24.MECO*/ wire MECO = not1(MAGU);
    /* p24.KEBO*/ wire KEBO = not1(MECO);
    /* p01.SAPY*/ wire UREK_DIV_07n = not1(top.tim_reg.TULU_DIV_07.qp());
    /* p24.USEC*/ wire USEC = not1(UREK_DIV_07n);

    /*#p24.KUPA*/ wire KUPA = amux2(top.pix_pipe.XONA_LCDC_ENn.qn(), KEBO, KEDY_LCDC_ENn, USEC);
    /* p24.KOFO*/ wire KOFO = not1(KUPA);
    top.LCD_PIN_ALTSG = KOFO;
  }

  {
    /* p21.TOCU*/ wire TOCU_C0n = not1(SAXO_X0p.qp());
    /* p21.VEPE*/ wire VEPE_C1n = not1(TYPO_X1p.qp());
    /* p21.VUTY*/ wire VUTY_C2n = not1(VYZO_X2p.qp());
    /* p21.VATE*/ wire VATE_C3n = not1(TELU_X3p.qp());
    /* p21.TUDA*/ wire TUDA_C4n = not1(SUDE_X4p.qp());
    /* p21.TAFY*/ wire TAFY_C5n = not1(TAHA_X5p.qp());
    /* p21.TUJU*/ wire TUJU_C6n = not1(TYRY_X6p.qp());
       
    /* p21.VOKU*/ wire VOKU_000n = nand7(  TUJU_C6n,   TAFY_C5n,   TUDA_C4n,   VATE_C3n,   VUTY_C2n,   VEPE_C1n,   TOCU_C0n); // 0000000 == 0
    /* p21.TOZU*/ wire TOZU_007n = nand7(  TUJU_C6n,   TAFY_C5n,   TUDA_C4n,   VATE_C3n, VYZO_X2p.qp(), TYPO_X1p.qp(), SAXO_X0p.qp()); // 0000111 == 7
    /* p21.TECE*/ wire TECE_045n = nand7(  TUJU_C6n, TAHA_X5p.qp(),   TUDA_C4n, TELU_X3p.qp(), VYZO_X2p.qp(),   VEPE_C1n, SAXO_X0p.qp()); // 0101101 == 45
    /* p21.TEBO*/ wire TEBO_083n = nand7(TYRY_X6p.qp(),   TAFY_C5n, SUDE_X4p.qp(),   VATE_C3n,   VUTY_C2n, TYPO_X1p.qp(), SAXO_X0p.qp()); // 1010011 == 83
       
    /* p21.TEGY*/ wire TEGY_LINE_STROBE = nand4(VOKU_000n, TOZU_007n, TECE_045n, TEBO_083n);
       
    /* p21.SYGU*/ SYGU_LINE_STROBE = dff17_B(SONO_xxxxEFGH, LYFE_VID_RSTn, TEGY_LINE_STROBE);
    /* p21.RYNO*/ wire RYNO = or2(TEGY_LINE_STROBE, RUTU_LINE_P908.qp());
    /* p21.POGU*/ wire POGU = not1(RYNO);
    top.LCD_PIN_CNTRL = POGU;
  }

  {
    /* p21.PURE*/ wire PURE_LINE_ENDn = not1(RUTU_LINE_P908.qp());
    /* p24.KASA*/ wire KASA_LINE_ENDp = not1(PURE_LINE_ENDn);
    /* p01.UTOK*/ wire UMEK_DIV_06n   = not1(top.tim_reg.UGOT_DIV_06.qp());
    /* p24.UMOB*/ wire UMOB_DIV_06p   = not1(UMEK_DIV_06n);

    /*#p24.KAHE*/ wire KAHE_LINE_ENDp = amux2(top.pix_pipe.XONA_LCDC_ENn.qn(), KASA_LINE_ENDp, KEDY_LCDC_ENn, UMOB_DIV_06p);
    /* p24.KYMO*/ wire KYMO_LINE_ENDn = not1(KAHE_LINE_ENDp);
    top.LCD_PIN_LATCH = KYMO_LINE_ENDn;
  }

  // LCD vertical sync pin
  {
    /* p24.NERU*/ wire NERU_LINE_000n = nor8(MUWY_Y0p.qp(), MYRO_Y1p.qp(), LEXA_Y2p.qp(), LYDO_Y3p.qp(), LOVU_Y4p.qp(), LEMA_Y5p.qp(), MATO_Y6p.qp(), LAFO_Y7p.qp());
    /* p24.MEDA*/ MEDA_VSYNC_OUTn = dff17_B(NYPE_LINE_P000.qp(), LYFE_VID_RSTn, NERU_LINE_000n);
    /* p24.MURE*/ wire MURE_VSYNC = not1(MEDA_VSYNC_OUTn.qp());
    top.LCD_PIN_VSYNC = MURE_VSYNC;
  }

  // ly match
  {
    /* p21.RYME*/ wire RYME_LY_MATCH0n = xor2(MUWY_Y0p.qp(), SYRY_LYC0n.qn());
    /* p21.TYDE*/ wire TYDE_LY_MATCH1n = xor2(MYRO_Y1p.qp(), VUCE_LYC1n.qn());
    /* p21.REDA*/ wire REDA_LY_MATCH2n = xor2(LEXA_Y2p.qp(), SEDY_LYC2n.qn());
    /* p21.RASY*/ wire RASY_LY_MATCH3n = xor2(LYDO_Y3p.qp(), SALO_LYC3n.qn());
    /* p21.TYKU*/ wire TYKU_LY_MATCH4n = xor2(LOVU_Y4p.qp(), SOTA_LYC4n.qn());
    /* p21.TUCY*/ wire TUCY_LY_MATCH5n = xor2(LEMA_Y5p.qp(), VAFA_LYC5n.qn());
    /* p21.TERY*/ wire TERY_LY_MATCH6n = xor2(MATO_Y6p.qp(), VEVO_LYC6n.qn());
    /* p21.SYFU*/ wire SYFU_LY_MATCH7n = xor2(LAFO_Y7p.qp(), RAHA_LYC7n.qn());
       
    /* p21.SOVU*/ wire SOVU_LY_MATCHA = nor4 (SYFU_LY_MATCH7n, TERY_LY_MATCH6n, TUCY_LY_MATCH5n, TYKU_LY_MATCH4n); // def nor4
    /* p21.SUBO*/ wire SUBO_LY_MATCHB = nor4 (RASY_LY_MATCH3n, REDA_LY_MATCH2n, TYDE_LY_MATCH1n, RYME_LY_MATCH0n); // def nor4
    /* p21.RAPE*/ wire RAPE_LY_MATCHn = nand2(SOVU_LY_MATCHA,  SUBO_LY_MATCHB); // def nand2
    /* p21.PALY*/ wire PALY_LY_MATCHa = not1(RAPE_LY_MATCHn); // def not
       
    /* p21.ROPO*/ ROPO_LY_MATCH_SYNCp = dff17_B(top.clk_reg.TALU_ABCDxxxx, WESY_SYS_RSTn, PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /* p07.UJYV*/ wire UJYV_CPU_RDn  = mux2_n(!top.ext_bus.EXT_PIN_RDn.qp(), top.cpu_bus.CPU_PIN_RDp.tp(), top.UNOR_MODE_DBG2p);
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

    /* p23.VEGA*/ cpu_bus.CPU_BUS_D0p = tribuf_6nn(VARO_FF44_RDn, WURY_LY0n);
    /* p23.WUVA*/ cpu_bus.CPU_BUS_D1p = tribuf_6nn(VARO_FF44_RDn, XEPO_LY1n);
    /* p23.LYCO*/ cpu_bus.CPU_BUS_D2p = tribuf_6nn(VARO_FF44_RDn, MYFA_LY2n);
    /* p23.WOJY*/ cpu_bus.CPU_BUS_D3p = tribuf_6nn(VARO_FF44_RDn, XUHY_LY3n);
    /* p23.VYNE*/ cpu_bus.CPU_BUS_D4p = tribuf_6nn(VARO_FF44_RDn, WATA_LY4n);
    /* p23.WAMA*/ cpu_bus.CPU_BUS_D5p = tribuf_6nn(VARO_FF44_RDn, XAGA_LY5n);
    /* p23.WAVO*/ cpu_bus.CPU_BUS_D6p = tribuf_6nn(VARO_FF44_RDn, XUCE_LY6n);
    /* p23.WEZE*/ cpu_bus.CPU_BUS_D7p = tribuf_6nn(VARO_FF44_RDn, XOWO_LY7n);
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
       
    /* p23.SYRY*/ SYRY_LYC0n = dff9_inv(WANE_FF45_WRp, !WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.CPU_BUS_D0p.tp());
    /* p23.VUCE*/ VUCE_LYC1n = dff9_inv(WANE_FF45_WRp, !WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.CPU_BUS_D1p.tp());
    /* p23.SEDY*/ SEDY_LYC2n = dff9_inv(WANE_FF45_WRp, !WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.CPU_BUS_D2p.tp());
    /* p23.SALO*/ SALO_LYC3n = dff9_inv(WANE_FF45_WRp, !WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.CPU_BUS_D3p.tp());
    /* p23.SOTA*/ SOTA_LYC4n = dff9_inv(WANE_FF45_WRp, !WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.CPU_BUS_D4p.tp());
    /* p23.VAFA*/ VAFA_LYC5n = dff9_inv(WANE_FF45_WRp, !WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.CPU_BUS_D5p.tp());
    /* p23.VEVO*/ VEVO_LYC6n = dff9_inv(WANE_FF45_WRp, !WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.CPU_BUS_D6p.tp());
    /* p23.RAHA*/ RAHA_LYC7n = dff9_inv(WANE_FF45_WRp, !WANE_FF45_WRp, WESY_SYS_RSTn, top.cpu_bus.CPU_BUS_D7p.tp());

    /*#p23.RETU*/ cpu_bus.CPU_BUS_D0p = tribuf_6nn(WEKU_FF45_RDn, SYRY_LYC0n.qp());
    /* p23.VOJO*/ cpu_bus.CPU_BUS_D1p = tribuf_6nn(WEKU_FF45_RDn, VUCE_LYC1n.qp());
    /* p23.RAZU*/ cpu_bus.CPU_BUS_D2p = tribuf_6nn(WEKU_FF45_RDn, SEDY_LYC2n.qp());
    /* p23.REDY*/ cpu_bus.CPU_BUS_D3p = tribuf_6nn(WEKU_FF45_RDn, SALO_LYC3n.qp());
    /* p23.RACE*/ cpu_bus.CPU_BUS_D4p = tribuf_6nn(WEKU_FF45_RDn, SOTA_LYC4n.qp());
    /*#p23.VAZU*/ cpu_bus.CPU_BUS_D5p = tribuf_6nn(WEKU_FF45_RDn, VAFA_LYC5n.qp());
    /* p23.VAFE*/ cpu_bus.CPU_BUS_D6p = tribuf_6nn(WEKU_FF45_RDn, VEVO_LYC6n.qp());
    /* p23.PUFY*/ cpu_bus.CPU_BUS_D7p = tribuf_6nn(WEKU_FF45_RDn, RAHA_LYC7n.qp());
  }
}

//------------------------------------------------------------------------------
