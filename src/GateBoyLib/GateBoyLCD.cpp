#include "GateBoyLib/GateBoyLCD.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_lyc_gates() {
  {
    // Inverting ROPO's clock and making it store the new match instead of the old match fixes lcdon_to_stat2_a but breaks other things
    // Just making it store the new match doesn't break anything.
    /*_p21.RYME*/ wire RYME_LY_MATCH0n_old = xor2(reg_ly.MUWY_LY0p.qp_old(), reg_lyc.SYRY_LYC0n.qn_old());
    /*_p21.TYDE*/ wire TYDE_LY_MATCH1n_old = xor2(reg_ly.MYRO_LY1p.qp_old(), reg_lyc.VUCE_LYC1n.qn_old());
    /*_p21.REDA*/ wire REDA_LY_MATCH2n_old = xor2(reg_ly.LEXA_LY2p.qp_old(), reg_lyc.SEDY_LYC2n.qn_old());
    /*_p21.RASY*/ wire RASY_LY_MATCH3n_old = xor2(reg_ly.LYDO_LY3p.qp_old(), reg_lyc.SALO_LYC3n.qn_old());
    /*_p21.TYKU*/ wire TYKU_LY_MATCH4n_old = xor2(reg_ly.LOVU_LY4p.qp_old(), reg_lyc.SOTA_LYC4n.qn_old());
    /*_p21.TUCY*/ wire TUCY_LY_MATCH5n_old = xor2(reg_ly.LEMA_LY5p.qp_old(), reg_lyc.VAFA_LYC5n.qn_old());
    /*_p21.TERY*/ wire TERY_LY_MATCH6n_old = xor2(reg_ly.MATO_LY6p.qp_old(), reg_lyc.VEVO_LYC6n.qn_old());
    /*_p21.SYFU*/ wire SYFU_LY_MATCH7n_old = xor2(reg_ly.LAFO_LY7p.qp_old(), reg_lyc.RAHA_LYC7n.qn_old());

    /*#p21.SOVU*/ wire SOVU_LY_MATCHA_old = nor4 (SYFU_LY_MATCH7n_old, TERY_LY_MATCH6n_old, TUCY_LY_MATCH5n_old, TYKU_LY_MATCH4n_old);
    /*#p21.SUBO*/ wire SUBO_LY_MATCHB_old = nor4 (RASY_LY_MATCH3n_old, REDA_LY_MATCH2n_old, TYDE_LY_MATCH1n_old, RYME_LY_MATCH0n_old);
    /*#p21.RAPE*/ wire RAPE_LY_MATCHn_old = nand2(SOVU_LY_MATCHA_old,  SUBO_LY_MATCHB_old);
    /*#p21.PALY*/ wire PALY_LY_MATCHa_old = not1 (RAPE_LY_MATCHn_old);

    /*#p21.ROPO*/ ROPO_LY_MATCH_SYNCp.dff17(TALU_xxCDEFxx(), WESY_SYS_RSTn(), PALY_LY_MATCHa_old);
  }

  {
    /*_p23.XUFA*/ wire XUFA_FF45_WRn = and2(CUPA_CPU_WRp(), cpu_abus_new.XAYU_FF45p());
    /*_p23.WANE*/ wire WANE_FF45_WRp = not1(XUFA_FF45_WRn);
    /*_p23.SYRY*/ reg_lyc.SYRY_LYC0n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn(), cpu_dbus_old.BUS_CPU_D00p.out_old());
    /*_p23.VUCE*/ reg_lyc.VUCE_LYC1n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn(), cpu_dbus_old.BUS_CPU_D01p.out_old());
    /*_p23.SEDY*/ reg_lyc.SEDY_LYC2n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn(), cpu_dbus_old.BUS_CPU_D02p.out_old());
    /*_p23.SALO*/ reg_lyc.SALO_LYC3n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn(), cpu_dbus_old.BUS_CPU_D03p.out_old());
    /*_p23.SOTA*/ reg_lyc.SOTA_LYC4n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn(), cpu_dbus_old.BUS_CPU_D04p.out_old());
    /*_p23.VAFA*/ reg_lyc.VAFA_LYC5n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn(), cpu_dbus_old.BUS_CPU_D05p.out_old());
    /*_p23.VEVO*/ reg_lyc.VEVO_LYC6n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn(), cpu_dbus_old.BUS_CPU_D06p.out_old());
    /*_p23.RAHA*/ reg_lyc.RAHA_LYC7n.dff9(WANE_FF45_WRp, WESY_SYS_RSTn(), cpu_dbus_old.BUS_CPU_D07p.out_old());

    /*_p23.XYLY*/ wire XYLY_FF45_RDp = and2(ASOT_CPU_RDp(), cpu_abus_new.XAYU_FF45p());
    /*_p23.WEKU*/ wire WEKU_FF45_RDn = not1(XYLY_FF45_RDp);

    /*#p23.RETU*/ triwire RETU_LYC0_TO_CD0 = tri6_nn(WEKU_FF45_RDn, reg_lyc.SYRY_LYC0n.qp_new());
    /*_p23.VOJO*/ triwire VOJO_LYC1_TO_CD1 = tri6_nn(WEKU_FF45_RDn, reg_lyc.VUCE_LYC1n.qp_new());
    /*_p23.RAZU*/ triwire RAZU_LYC2_TO_CD2 = tri6_nn(WEKU_FF45_RDn, reg_lyc.SEDY_LYC2n.qp_new());
    /*_p23.REDY*/ triwire REDY_LYC3_TO_CD3 = tri6_nn(WEKU_FF45_RDn, reg_lyc.SALO_LYC3n.qp_new());
    /*_p23.RACE*/ triwire RACE_LYC4_TO_CD4 = tri6_nn(WEKU_FF45_RDn, reg_lyc.SOTA_LYC4n.qp_new());
    /*#p23.VAZU*/ triwire VAZU_LYC5_TO_CD5 = tri6_nn(WEKU_FF45_RDn, reg_lyc.VAFA_LYC5n.qp_new());
    /*_p23.VAFE*/ triwire VAFE_LYC6_TO_CD6 = tri6_nn(WEKU_FF45_RDn, reg_lyc.VEVO_LYC6n.qp_new());
    /*_p23.PUFY*/ triwire PUFY_LYC7_TO_CD7 = tri6_nn(WEKU_FF45_RDn, reg_lyc.RAHA_LYC7n.qp_new());

    /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(RETU_LYC0_TO_CD0);
    /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(VOJO_LYC1_TO_CD1);
    /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(RAZU_LYC2_TO_CD2);
    /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(REDY_LYC3_TO_CD3);
    /*_BUS_CPU_D04p*/ cpu_dbus_new.BUS_CPU_D04p.tri_bus(RACE_LYC4_TO_CD4);
    /*_BUS_CPU_D05p*/ cpu_dbus_new.BUS_CPU_D05p.tri_bus(VAZU_LYC5_TO_CD5);
    /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(VAFE_LYC6_TO_CD6);
    /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(PUFY_LYC7_TO_CD7);
  }

  {
    /*_p21.SEPA*/ wire SEPA_FF41_WRp = and2(CUPA_CPU_WRp(), cpu_abus_new.VARY_FF41p());
    /*_p21.RYJU*/ wire RYJU_FF41_WRn = not1(SEPA_FF41_WRp);
    /*_p21.PAGO*/ wire PAGO_LYC_MATCH_RST = or2(WESY_SYS_RSTn(), RYJU_FF41_WRn);
    /*_p21.RUPO*/ RUPO_LYC_MATCHn.nor_latch(PAGO_LYC_MATCH_RST, ROPO_LY_MATCH_SYNCp.qp_new());
  }
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_lcd_gates() {
  {
    /*#p21.XYVO*/ wire XYVO_y144p = and2(reg_ly.LOVU_LY4p.qp_old(), reg_ly.LAFO_LY7p.qp_old()); // 128 + 16 = 144
    /*#p21.NOKO*/ wire NOKO_y153p = and4(reg_ly.LAFO_LY7p.qp_old(), reg_ly.LOVU_LY4p.qp_old(), reg_ly.LYDO_LY3p.qp_old(), reg_ly.MUWY_LY0p.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153

    /*#p28.ANEL*/ lcd.ANEL_x113p.dff17(AWOH_xxCDxxGH(), ABEZ_VID_RSTn(), lcd.CATU_x113p.qp_old());

    /*#p21.PURE*/ wire PURE_x113n = not1(lcd.RUTU_x113p.qp_old());
    /*#p29.ALES*/ wire ALES_y144n = not1(XYVO_y144p);
    /*#p21.SELA*/ wire SELA_x113p = not1(PURE_x113n);
    /*#p29.ABOV*/ wire ABOV_x113p = and2(SELA_x113p, ALES_y144n);
    /*#p29.CATU*/ lcd.CATU_x113p.dff17(XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), ABOV_x113p);

    /*#p21.NYPE*/ lcd.NYPE_x113p.dff17(TALU_xxCDEFxx(),         LYFE_VID_RSTn(), lcd.RUTU_x113p.qp_old());
    /*#p21.SANU*/ wire SANU_x113p = and4(reg_lx.TYRY_LX6p.qp_old(), reg_lx.TAHA_LX5p.qp_old(), reg_lx.SUDE_LX4p.qp_old(), reg_lx.SAXO_LX0p.qp_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*#p21.RUTU*/ lcd.RUTU_x113p.dff17(SONO_ABxxxxGH(),         LYFE_VID_RSTn(), SANU_x113p);

    /*_p28.ABAF*/ wire ABAF_x113n = not1(lcd.CATU_x113p.qp_new());
    /*_p28.BYHA*/ wire BYHA_LINE_RSTn = or_and3(lcd.ANEL_x113p.qp_new(), ABAF_x113n, ABEZ_VID_RSTn()); // so if this is or_and, BYHA should go low on 910 and 911
    /*_p28.ATEJ*/ ATEJ_LINE_RSTp = not1(BYHA_LINE_RSTn);

    /*#p21.POPU*/ lcd.POPU_y144p.dff17(lcd.NYPE_x113p.qp_new(), LYFE_VID_RSTn(), XYVO_y144p);
    /*#p21.MYTA*/ lcd.MYTA_y153p.dff17(lcd.NYPE_x113p.qp_new(), LYFE_VID_RSTn(), NOKO_y153p);
  }

  {
    /*#p21.TOCU*/ wire TOCU_LX0n_old = not1(reg_lx.SAXO_LX0p.qp_old());
    /*#p21.VEPE*/ wire VEPE_LX1n_old = not1(reg_lx.TYPO_LX1p.qp_old());
    /*_p21.VUTY*/ wire VUTY_LX2n_old = not1(reg_lx.VYZO_LX2p.qp_old());
    /*_p21.VATE*/ wire VATE_LX3n_old = not1(reg_lx.TELU_LX3p.qp_old());
    /*_p21.TUDA*/ wire TUDA_LX4n_old = not1(reg_lx.SUDE_LX4p.qp_old());
    /*_p21.TAFY*/ wire TAFY_LX5n_old = not1(reg_lx.TAHA_LX5p.qp_old());
    /*_p21.TUJU*/ wire TUJU_LX6n_old = not1(reg_lx.TYRY_LX6p.qp_old());

    /*_p21.VOKU*/ wire VOKU_LX000n_old = nand7(TUJU_LX6n_old,            TAFY_LX5n_old,            TUDA_LX4n_old,            VATE_LX3n_old,            VUTY_LX2n_old,            VEPE_LX1n_old,            TOCU_LX0n_old);            // 0000000 == 0
    /*_p21.TOZU*/ wire TOZU_LX007n_old = nand7(TUJU_LX6n_old,            TAFY_LX5n_old,            TUDA_LX4n_old,            VATE_LX3n_old,            reg_lx.VYZO_LX2p.qp_old(), reg_lx.TYPO_LX1p.qp_old(), reg_lx.SAXO_LX0p.qp_old()); // 0000111 == 7
    /*_p21.TECE*/ wire TECE_LX045n_old = nand7(TUJU_LX6n_old,            reg_lx.TAHA_LX5p.qp_old(), TUDA_LX4n_old,            reg_lx.TELU_LX3p.qp_old(), reg_lx.VYZO_LX2p.qp_old(), VEPE_LX1n_old,            reg_lx.SAXO_LX0p.qp_old()); // 0101101 == 45
    /*#p21.TEBO*/ wire TEBO_LX083n_old = nand7(reg_lx.TYRY_LX6p.qp_old(), TAFY_LX5n_old,            reg_lx.SUDE_LX4p.qp_old(), VATE_LX3n_old,            VUTY_LX2n_old,            reg_lx.TYPO_LX1p.qp_old(), reg_lx.SAXO_LX0p.qp_old()); // 1010011 == 83

    /*#p21.TEGY*/ wire TEGY_STROBE_old = nand4(VOKU_LX000n_old, TOZU_LX007n_old, TECE_LX045n_old, TEBO_LX083n_old);

    /*#p21.SYGU*/ lcd.SYGU_LINE_STROBE.dff17(SONO_ABxxxxGH(), LYFE_VID_RSTn(), TEGY_STROBE_old);
  }

  {
    /*#p21.MUDE*/ wire MUDE_X_RSTn = nor2(lcd.RUTU_x113p.qp_new(), LYHA_VID_RSTp());
    /*#p21.SAXO*/ reg_lx.SAXO_LX0p.dff17(TALU_xxCDEFxx(),           MUDE_X_RSTn, reg_lx.SAXO_LX0p.qn_old());
    /*#p21.TYPO*/ reg_lx.TYPO_LX1p.dff17(reg_lx.SAXO_LX0p.qn_new(), MUDE_X_RSTn, reg_lx.TYPO_LX1p.qn_old());
    /*#p21.VYZO*/ reg_lx.VYZO_LX2p.dff17(reg_lx.TYPO_LX1p.qn_new(), MUDE_X_RSTn, reg_lx.VYZO_LX2p.qn_old());
    /*#p21.TELU*/ reg_lx.TELU_LX3p.dff17(reg_lx.VYZO_LX2p.qn_new(), MUDE_X_RSTn, reg_lx.TELU_LX3p.qn_old());
    /*#p21.SUDE*/ reg_lx.SUDE_LX4p.dff17(reg_lx.TELU_LX3p.qn_new(), MUDE_X_RSTn, reg_lx.SUDE_LX4p.qn_old());
    /*#p21.TAHA*/ reg_lx.TAHA_LX5p.dff17(reg_lx.SUDE_LX4p.qn_new(), MUDE_X_RSTn, reg_lx.TAHA_LX5p.qn_old());
    /*#p21.TYRY*/ reg_lx.TYRY_LX6p.dff17(reg_lx.TAHA_LX5p.qn_new(), MUDE_X_RSTn, reg_lx.TYRY_LX6p.qn_old());
  }

  {
    /*#p21.LAMA*/ wire LAMA_Y_RSTn = nor2(lcd.MYTA_y153p.qp_new(), LYHA_VID_RSTp());
    /*#p21.MUWY*/ reg_ly.MUWY_LY0p.dff17(lcd.RUTU_x113p.qp_new(), LAMA_Y_RSTn, reg_ly.MUWY_LY0p.qn_old());
    /*#p21.MYRO*/ reg_ly.MYRO_LY1p.dff17(reg_ly.MUWY_LY0p.qn_new(),  LAMA_Y_RSTn, reg_ly.MYRO_LY1p.qn_old());
    /*#p21.LEXA*/ reg_ly.LEXA_LY2p.dff17(reg_ly.MYRO_LY1p.qn_new(),  LAMA_Y_RSTn, reg_ly.LEXA_LY2p.qn_old());
    /*#p21.LYDO*/ reg_ly.LYDO_LY3p.dff17(reg_ly.LEXA_LY2p.qn_new(),  LAMA_Y_RSTn, reg_ly.LYDO_LY3p.qn_old());
    /*#p21.LOVU*/ reg_ly.LOVU_LY4p.dff17(reg_ly.LYDO_LY3p.qn_new(),  LAMA_Y_RSTn, reg_ly.LOVU_LY4p.qn_old());
    /*#p21.LEMA*/ reg_ly.LEMA_LY5p.dff17(reg_ly.LOVU_LY4p.qn_new(),  LAMA_Y_RSTn, reg_ly.LEMA_LY5p.qn_old());
    /*#p21.MATO*/ reg_ly.MATO_LY6p.dff17(reg_ly.LEMA_LY5p.qn_new(),  LAMA_Y_RSTn, reg_ly.MATO_LY6p.qn_old());
    /*#p21.LAFO*/ reg_ly.LAFO_LY7p.dff17(reg_ly.MATO_LY6p.qn_new(),  LAMA_Y_RSTn, reg_ly.LAFO_LY7p.qn_old());
  }

  {
    /*_p23.WAFU*/ wire WAFU_FF44_RDp = and2(ASOT_CPU_RDp(), cpu_abus_new.XOGY_FF44p());
    /*_p23.VARO*/ wire VARO_FF44_RDn = not1(WAFU_FF44_RDp);

    /*#p23.WURY*/ wire WURY_LY0n = not1(reg_ly.MUWY_LY0p.qp_new());
    /*_p23.XEPO*/ wire XEPO_LY1n = not1(reg_ly.MYRO_LY1p.qp_new());
    /*_p23.MYFA*/ wire MYFA_LY2n = not1(reg_ly.LEXA_LY2p.qp_new());
    /*_p23.XUHY*/ wire XUHY_LY3n = not1(reg_ly.LYDO_LY3p.qp_new());
    /*_p23.WATA*/ wire WATA_LY4n = not1(reg_ly.LOVU_LY4p.qp_new());
    /*_p23.XAGA*/ wire XAGA_LY5n = not1(reg_ly.LEMA_LY5p.qp_new());
    /*_p23.XUCE*/ wire XUCE_LY6n = not1(reg_ly.MATO_LY6p.qp_new());
    /*_p23.XOWO*/ wire XOWO_LY7n = not1(reg_ly.LAFO_LY7p.qp_new());

    /*_p23.VEGA*/ triwire VEGA_LY0_TO_CD0 = tri6_nn(VARO_FF44_RDn, WURY_LY0n);
    /*_p23.WUVA*/ triwire WUVA_LY1_TO_CD1 = tri6_nn(VARO_FF44_RDn, XEPO_LY1n);
    /*_p23.LYCO*/ triwire LYCO_LY2_TO_CD2 = tri6_nn(VARO_FF44_RDn, MYFA_LY2n);
    /*_p23.WOJY*/ triwire WOJY_LY3_TO_CD3 = tri6_nn(VARO_FF44_RDn, XUHY_LY3n);
    /*_p23.VYNE*/ triwire VYNE_LY4_TO_CD4 = tri6_nn(VARO_FF44_RDn, WATA_LY4n);
    /*_p23.WAMA*/ triwire WAMA_LY5_TO_CD5 = tri6_nn(VARO_FF44_RDn, XAGA_LY5n);
    /*_p23.WAVO*/ triwire WAVO_LY6_TO_CD6 = tri6_nn(VARO_FF44_RDn, XUCE_LY6n);
    /*_p23.WEZE*/ triwire WEZE_LY7_TO_CD7 = tri6_nn(VARO_FF44_RDn, XOWO_LY7n);

    /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(VEGA_LY0_TO_CD0);
    /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(WUVA_LY1_TO_CD1);
    /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(LYCO_LY2_TO_CD2);
    /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(WOJY_LY3_TO_CD3);
    /*_BUS_CPU_D04p*/ cpu_dbus_new.BUS_CPU_D04p.tri_bus(VYNE_LY4_TO_CD4);
    /*_BUS_CPU_D05p*/ cpu_dbus_new.BUS_CPU_D05p.tri_bus(WAMA_LY5_TO_CD5);
    /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(WAVO_LY6_TO_CD6);
    /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(WEZE_LY7_TO_CD7);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_lcd_pins_gates(wire SACU_CLKPIPE_evn) {
  /*#p21.RYNO*/ wire RYNO = or2(lcd.SYGU_LINE_STROBE.qp_new(), lcd.RUTU_x113p.qp_new());
  /*#p21.POGU*/ wire POGU = not1(RYNO);
  /*_PIN_52*/ lcd.PIN_52_LCD_CNTRL.pin_out(POGU, POGU);

  // if LCDC_ENn, PIN_56_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)

  /*#p24.LOFU*/ wire LOFU_x113n = not1(lcd.RUTU_x113p.qp_new());
  /*#p24.LUCA*/ lcd.LUCA_LINE_EVENp .dff17(LOFU_x113n, LYFE_VID_RSTn(), lcd.LUCA_LINE_EVENp.qn_old());
  /*#p21.NAPO*/ lcd.NAPO_FRAME_EVENp.dff17(lcd.POPU_y144p.qp_new(),   LYFE_VID_RSTn(), lcd.NAPO_FRAME_EVENp.qn_old());

  /*#p24.MAGU*/ wire MAGU = xor2(lcd.NAPO_FRAME_EVENp.qp_new(), lcd.LUCA_LINE_EVENp.qn_new());
  /*#p24.MECO*/ wire MECO = not1(MAGU);
  /*#p24.KEBO*/ wire KEBO = not1(MECO);
  /*_p01.UREK*/ wire UREK_DIV07n = not1(div.TULU_DIV07p.qp_new());
  /*#p24.USEC*/ wire USEC_DIV07p = not1(UREK_DIV07n);
  /*#p24.KEDY*/ wire KEDY_LCDC_ENn = not1(reg_lcdc.XONA_LCDC_LCDENn.qn_new());
  /*#p24.KUPA*/ wire KUPA = amux2(reg_lcdc.XONA_LCDC_LCDENn.qn_new(), KEBO, KEDY_LCDC_ENn, USEC_DIV07p);
  /*#p24.KOFO*/ wire KOFO = not1(KUPA);
  /*_PIN_56*/ lcd.PIN_56_LCD_FLIPS.pin_out(KOFO, KOFO);

  /*#p24.NERU*/ wire NERU_VSYNCp = nor8(reg_ly.LAFO_LY7p.qp_new(), reg_ly.LOVU_LY4p.qp_new(), reg_ly.LYDO_LY3p.qp_new(), reg_ly.MUWY_LY0p.qp_new(), reg_ly.MYRO_LY1p.qp_new(), reg_ly.LEXA_LY2p.qp_new(), reg_ly.LEMA_LY5p.qp_new(), reg_ly.MATO_LY6p.qp_new());
  /*#p24.MEDA*/ lcd.MEDA_VSYNC_OUTn.dff17(lcd.NYPE_x113p.qn_new(), LYFE_VID_RSTn(), NERU_VSYNCp);
  /*#p24.MURE*/ wire MURE_VSYNC = not1(lcd.MEDA_VSYNC_OUTn.qp_new());
  /*_PIN_57*/ lcd.PIN_57_LCD_VSYNC.pin_out(MURE_VSYNC, MURE_VSYNC);

  // FIXME inversion
  // I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
  // seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
  // Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
  // That would be a loooot of gate delay.
  // Could we possibly be incrementing X3p one phase early?

  // LCD horizontal sync pin latch
  /*#p24.POME*/ lcd.POME = nor2(sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new(), lcd.POFY.out_mid());
  /*#p24.RUJU*/ lcd.RUJU = or3(lcd.PAHO_X_8_SYNC.qp_new(), TOFU_VID_RSTp(), lcd.POME.out_mid());
  /*#p24.POFY*/ lcd.POFY = not1(lcd.RUJU.out_mid());
  /*#p24.POME*/ lcd.POME = nor2(sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new(), lcd.POFY.out_mid());
  /*#p24.RUJU*/ lcd.RUJU = or3(lcd.PAHO_X_8_SYNC.qp_new(), TOFU_VID_RSTp(), lcd.POME.out_mid());
  /*#p24.POFY*/ lcd.POFY = not1(lcd.RUJU.out_mid());

  /*#p24.RUZE*/ wire RUZE_HSYNCn = not1(lcd.POFY.out_mid());
  /*_PIN_54*/ lcd.PIN_54_LCD_HSYNC.pin_out(RUZE_HSYNCn, RUZE_HSYNCn);

  /*_PIN_51*/ lcd.PIN_51_LCD_DATA0.pin_out(REMY_LD0n.out_new(), REMY_LD0n.out_new());
  /*_PIN_50*/ lcd.PIN_50_LCD_DATA1.pin_out(RAVO_LD1n.out_new(), RAVO_LD1n.out_new());

  /*_p01.UMEK*/ wire UMEK_DIV06n = not1(div.UGOT_DIV06p.qp_new());
  /*#p21.PURE*/ wire PURE_x113n = not1(lcd.RUTU_x113p.qp_new());
  /*#p24.KASA*/ wire KASA_LINE_ENDp = not1(PURE_x113n);
  /*#p24.UMOB*/ wire UMOB_DIV_06p = not1(UMEK_DIV06n);
  /*#p24.KAHE*/ wire KAHE_LINE_ENDp = amux2(reg_lcdc.XONA_LCDC_LCDENn.qn_new(), KASA_LINE_ENDp, KEDY_LCDC_ENn, UMOB_DIV_06p);
  /*#p24.KYMO*/ wire KYMO_LINE_ENDn = not1(KAHE_LINE_ENDp);
  /*_PIN_55*/ lcd.PIN_55_LCD_LATCH.pin_out(KYMO_LINE_ENDn, KYMO_LINE_ENDn);

  /*#p21.XAJO*/ wire XAJO_X_009p = and2(pix_count.XEHO_PX0p.qp_new(), pix_count.XYDO_PX3p.qp_new());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), VOGA_HBLANKp.qp_new());
  /*#p21.WUSA*/ lcd.WUSA_LCD_CLOCK_GATE.nor_latch(XAJO_X_009p, WEGO_HBLANKp);
  /*#p21.TOBA*/ wire TOBA_LCD_CLOCK = and2(lcd.WUSA_LCD_CLOCK_GATE.qp_new(), SACU_CLKPIPE_evn);
  /*#p27.POVA*/ wire POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_new(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_new());
  /*#p21.SEMU*/ wire SEMU_LCD_CLOCK = or2(TOBA_LCD_CLOCK, POVA_FINE_MATCH_TRIGp);
  /*#p21.RYPO*/ wire RYPO_LCD_CLOCK = not1(SEMU_LCD_CLOCK);
  /*_PIN_53*/ lcd.PIN_53_LCD_CLOCK.pin_out(RYPO_LCD_CLOCK, RYPO_LCD_CLOCK);

  /*
  lcd_pix_lo.nor_latch(PIN_51_LCD_DATA0.qp_new(), PIN_53_LCD_CLOCK.qp_new() | PIN_54_LCD_HSYNC.qp_new());
  lcd_pix_hi.nor_latch(PIN_50_LCD_DATA1.qp_new(), PIN_53_LCD_CLOCK.qp_new() | PIN_54_LCD_HSYNC.qp_new());

  for (int i = 0; i < 159; i++) {
    lcd_pipe_lo[i].dff(bit(~PIN_53_LCD_CLOCK.qp_new()), 1, 1, lcd_pipe_lo[i + 1].qp_old());
    lcd_pipe_hi[i].dff(bit(~PIN_53_LCD_CLOCK.qp_new()), 1, 1, lcd_pipe_hi[i + 1].qp_old());
  }

  lcd_pipe_lo[159].dff(bit(~PIN_53_LCD_CLOCK.qp_new()), 1, 1, lcd_pix_lo.qp_new());
  lcd_pipe_hi[159].dff(bit(~PIN_53_LCD_CLOCK.qp_new()), 1, 1, lcd_pix_hi.qp_new());
  */
}
