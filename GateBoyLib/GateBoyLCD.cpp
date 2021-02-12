#include "GateBoyLib/GateBoyLCD.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_lyc() {
  /* p23.XUFA*/ wire _XUFA_FF45_WRn = and2(CUPA_CPU_WRp(), new_bus.XAYU_FF45p());
  /* p23.WANE*/ wire _WANE_FF45_WRp = not1(_XUFA_FF45_WRn);
  /* p23.SYRY*/ reg_lyc.SYRY_LYC0n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D00p.qp_old());
  /* p23.VUCE*/ reg_lyc.VUCE_LYC1n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D01p.qp_old());
  /* p23.SEDY*/ reg_lyc.SEDY_LYC2n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D02p.qp_old());
  /* p23.SALO*/ reg_lyc.SALO_LYC3n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p23.SOTA*/ reg_lyc.SOTA_LYC4n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p23.VAFA*/ reg_lyc.VAFA_LYC5n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p23.VEVO*/ reg_lyc.VEVO_LYC6n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
  /* p23.RAHA*/ reg_lyc.RAHA_LYC7n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D07p.qp_old());

  /* p23.XYLY*/ wire _XYLY_FF45_RDp = and2(ASOT_CPU_RDp(), new_bus.XAYU_FF45p());
  /* p23.WEKU*/ wire _WEKU_FF45_RDn = not1(_XYLY_FF45_RDp);
  /*#p23.RETU_LYC0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.SYRY_LYC0n.qp_new());
  /* p23.VOJO_LYC1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.VUCE_LYC1n.qp_new());
  /* p23.RAZU_LYC2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.SEDY_LYC2n.qp_new());
  /* p23.REDY_LYC3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.SALO_LYC3n.qp_new());
  /* p23.RACE_LYC4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.SOTA_LYC4n.qp_new());
  /*#p23.VAZU_LYC5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.VAFA_LYC5n.qp_new());
  /* p23.VAFE_LYC6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.VEVO_LYC6n.qp_new());
  /* p23.PUFY_LYC7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.RAHA_LYC7n.qp_new());
}

void GateBoy::tock_lcd() {
  /*#p21.PURE*/ wire _PURE_LINE_ENDn_old = not1(reg_lx.RUTU_x113p.qp_old());
  /*#p21.SELA*/ wire _SELA_LINE_P908p_old = not1(_PURE_LINE_ENDn_old);
  /*#p21.XYVO*/ wire XYVO_y144p_old = and2(reg_ly.LOVU_LY4p.qp_old(), reg_ly.LAFO_LY7p.qp_old()); // 128 + 16 = 144

  /*#p29.ALES*/ wire _ALES_y144n_old = not1(XYVO_y144p_old);
  /*#p29.ABOV*/ wire ABOV_LINE_P908p_old = and2(_SELA_LINE_P908p_old, _ALES_y144n_old);


  /*#p28.ANEL*/ lcd.ANEL_LINE_P002p.dff17(AWOH_xxCDxxGH(), ABEZ_VID_RSTn(),  lcd.CATU_START_SCANNING.qp_old());
  /*#p29.CATU*/ lcd.CATU_START_SCANNING.dff17(XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), ABOV_LINE_P908p_old);

  /*#p21.NYPE*/ reg_lx.NYPE_x113p.dff17(TALU_xxCDEFxx(), LYFE_VID_RSTn(), reg_lx.RUTU_x113p.qp_old());

  /*#p21.SANU*/ wire SANU_x113p_old = and4(reg_lx.TYRY_LX6p.qp_old(), reg_lx.TAHA_LX5p.qp_old(), reg_lx.SUDE_LX4p.qp_old(), reg_lx.SAXO_LX0p.qp_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*#p21.RUTU*/ reg_lx.RUTU_x113p.dff17(SONO_ABxxxxGH(), LYFE_VID_RSTn(), SANU_x113p_old);

  /*#p21.POPU*/ lcd.POPU_VBLANKp.dff17(reg_lx.NYPE_x113p.qp_new(), LYFE_VID_RSTn(), XYVO_y144p_old);

  /*#p21.NOKO*/ wire NOKO_y153p = and4(reg_ly.LAFO_LY7p.qp_old(), reg_ly.LOVU_LY4p.qp_old(), reg_ly.LYDO_LY3p.qp_old(), reg_ly.MUWY_LY0p.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
  /*#p21.MYTA*/ reg_ly.MYTA_y153p.dff17(reg_lx.NYPE_x113p.qp_new(), LYFE_VID_RSTn(), NOKO_y153p);

  /* p28.ABAF*/ wire _ABAF_LINE_P000n = not1(lcd.CATU_START_SCANNING.qp_new());
  /* p28.BYHA*/ wire _BYHA_LINE_RSTn = or_and3(lcd.ANEL_LINE_P002p.qp_new(), _ABAF_LINE_P000n, ABEZ_VID_RSTn()); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ ATEJ_LINE_RSTp = not1(_BYHA_LINE_RSTn);
}

void GateBoy::tock_lx() {
  /*#p21.MUDE*/ wire _MUDE_X_RSTn = nor2(reg_lx.RUTU_x113p.qp_new(), LYHA_VID_RSTp());
  /*#p21.SAXO*/ reg_lx.SAXO_LX0p.dff17(TALU_xxCDEFxx(),           _MUDE_X_RSTn, reg_lx.SAXO_LX0p.qn_old());
  /*#p21.TYPO*/ reg_lx.TYPO_LX1p.dff17(reg_lx.SAXO_LX0p.qn_new(), _MUDE_X_RSTn, reg_lx.TYPO_LX1p.qn_old());
  /*#p21.VYZO*/ reg_lx.VYZO_LX2p.dff17(reg_lx.TYPO_LX1p.qn_new(), _MUDE_X_RSTn, reg_lx.VYZO_LX2p.qn_old());
  /*#p21.TELU*/ reg_lx.TELU_LX3p.dff17(reg_lx.VYZO_LX2p.qn_new(), _MUDE_X_RSTn, reg_lx.TELU_LX3p.qn_old());
  /*#p21.SUDE*/ reg_lx.SUDE_LX4p.dff17(reg_lx.TELU_LX3p.qn_new(), _MUDE_X_RSTn, reg_lx.SUDE_LX4p.qn_old());
  /*#p21.TAHA*/ reg_lx.TAHA_LX5p.dff17(reg_lx.SUDE_LX4p.qn_new(), _MUDE_X_RSTn, reg_lx.TAHA_LX5p.qn_old());
  /*#p21.TYRY*/ reg_lx.TYRY_LX6p.dff17(reg_lx.TAHA_LX5p.qn_new(), _MUDE_X_RSTn, reg_lx.TYRY_LX6p.qn_old());
}

void GateBoy::tock_ly() {
  /*#p21.LAMA*/ wire _LAMA_Y_RSTn = nor2(reg_ly.MYTA_y153p.qp_new(), LYHA_VID_RSTp());
  /*#p21.MUWY*/ reg_ly.MUWY_LY0p.dff17(reg_lx.RUTU_x113p.qp_new(), _LAMA_Y_RSTn, reg_ly.MUWY_LY0p.qn_old());
  /*#p21.MYRO*/ reg_ly.MYRO_LY1p.dff17(reg_ly.MUWY_LY0p.qn_new(),  _LAMA_Y_RSTn, reg_ly.MYRO_LY1p.qn_old());
  /*#p21.LEXA*/ reg_ly.LEXA_LY2p.dff17(reg_ly.MYRO_LY1p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LEXA_LY2p.qn_old());
  /*#p21.LYDO*/ reg_ly.LYDO_LY3p.dff17(reg_ly.LEXA_LY2p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LYDO_LY3p.qn_old());
  /*#p21.LOVU*/ reg_ly.LOVU_LY4p.dff17(reg_ly.LYDO_LY3p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LOVU_LY4p.qn_old());
  /*#p21.LEMA*/ reg_ly.LEMA_LY5p.dff17(reg_ly.LOVU_LY4p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LEMA_LY5p.qn_old());
  /*#p21.MATO*/ reg_ly.MATO_LY6p.dff17(reg_ly.LEMA_LY5p.qn_new(),  _LAMA_Y_RSTn, reg_ly.MATO_LY6p.qn_old());
  /*#p21.LAFO*/ reg_ly.LAFO_LY7p.dff17(reg_ly.MATO_LY6p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LAFO_LY7p.qn_old());

  /* p23.WAFU*/ wire _WAFU_FF44_RDp = and2(ASOT_CPU_RDp(), new_bus.XOGY_FF44p());
  /* p23.VARO*/ wire _VARO_FF44_RDn = not1(_WAFU_FF44_RDp);

  /*#p23.WURY*/ wire _WURY_LY0n = not1(reg_ly.MUWY_LY0p.qp_new());
  /* p23.XEPO*/ wire _XEPO_LY1n = not1(reg_ly.MYRO_LY1p.qp_new());
  /* p23.MYFA*/ wire _MYFA_LY2n = not1(reg_ly.LEXA_LY2p.qp_new());
  /* p23.XUHY*/ wire _XUHY_LY3n = not1(reg_ly.LYDO_LY3p.qp_new());
  /* p23.WATA*/ wire _WATA_LY4n = not1(reg_ly.LOVU_LY4p.qp_new());
  /* p23.XAGA*/ wire _XAGA_LY5n = not1(reg_ly.LEMA_LY5p.qp_new());
  /* p23.XUCE*/ wire _XUCE_LY6n = not1(reg_ly.MATO_LY6p.qp_new());
  /* p23.XOWO*/ wire _XOWO_LY7n = not1(reg_ly.LAFO_LY7p.qp_new());

  /* p23.VEGA_LY0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_VARO_FF44_RDn, _WURY_LY0n);
  /* p23.WUVA_LY1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_VARO_FF44_RDn, _XEPO_LY1n);
  /* p23.LYCO_LY2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_VARO_FF44_RDn, _MYFA_LY2n);
  /* p23.WOJY_LY3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_VARO_FF44_RDn, _XUHY_LY3n);
  /* p23.VYNE_LY4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_VARO_FF44_RDn, _WATA_LY4n);
  /* p23.WAMA_LY5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_VARO_FF44_RDn, _XAGA_LY5n);
  /* p23.WAVO_LY6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_VARO_FF44_RDn, _XUCE_LY6n);
  /* p23.WEZE_LY7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_VARO_FF44_RDn, _XOWO_LY7n);
}

void GateBoy::tock_lyc_match() {
  /* p21.RYME*/ wire _RYME_LY_MATCH0n_old = xor2(reg_ly.MUWY_LY0p.qp_old(), reg_lyc.SYRY_LYC0n.qn_old());
  /* p21.TYDE*/ wire _TYDE_LY_MATCH1n_old = xor2(reg_ly.MYRO_LY1p.qp_old(), reg_lyc.VUCE_LYC1n.qn_old());
  /* p21.REDA*/ wire _REDA_LY_MATCH2n_old = xor2(reg_ly.LEXA_LY2p.qp_old(), reg_lyc.SEDY_LYC2n.qn_old());
  /* p21.RASY*/ wire _RASY_LY_MATCH3n_old = xor2(reg_ly.LYDO_LY3p.qp_old(), reg_lyc.SALO_LYC3n.qn_old());
  /* p21.TYKU*/ wire _TYKU_LY_MATCH4n_old = xor2(reg_ly.LOVU_LY4p.qp_old(), reg_lyc.SOTA_LYC4n.qn_old());
  /* p21.TUCY*/ wire _TUCY_LY_MATCH5n_old = xor2(reg_ly.LEMA_LY5p.qp_old(), reg_lyc.VAFA_LYC5n.qn_old());
  /* p21.TERY*/ wire _TERY_LY_MATCH6n_old = xor2(reg_ly.MATO_LY6p.qp_old(), reg_lyc.VEVO_LYC6n.qn_old());
  /* p21.SYFU*/ wire _SYFU_LY_MATCH7n_old = xor2(reg_ly.LAFO_LY7p.qp_old(), reg_lyc.RAHA_LYC7n.qn_old());

  /*#p21.SOVU*/ wire _SOVU_LY_MATCHA_old = nor4 (_SYFU_LY_MATCH7n_old, _TERY_LY_MATCH6n_old, _TUCY_LY_MATCH5n_old, _TYKU_LY_MATCH4n_old);
  /*#p21.SUBO*/ wire _SUBO_LY_MATCHB_old = nor4 (_RASY_LY_MATCH3n_old, _REDA_LY_MATCH2n_old, _TYDE_LY_MATCH1n_old, _RYME_LY_MATCH0n_old);
  /*#p21.RAPE*/ wire _RAPE_LY_MATCHn_old = nand2(_SOVU_LY_MATCHA_old,  _SUBO_LY_MATCHB_old);
  /*#p21.PALY*/ wire _PALY_LY_MATCHa_old = not1 (_RAPE_LY_MATCHn_old);

  /*#p21.ROPO*/ reg_lyc.ROPO_LY_MATCH_SYNCp.dff17(TALU_xxCDEFxx(), WESY_SYS_RSTn(), _PALY_LY_MATCHa_old);

  /* p21.SEPA*/ wire _SEPA_FF41_WRp = and2(CUPA_CPU_WRp(), new_bus.VARY_FF41p());
  /* p21.RYJU*/ wire _RYJU_FF41_WRn = not1(_SEPA_FF41_WRp);
  /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = or2(WESY_SYS_RSTn(), _RYJU_FF41_WRn);
  /* p21.RUPO*/ reg_stat.RUPO_LYC_MATCHn.nor_latch(_PAGO_LYC_MATCH_RST, reg_lyc.ROPO_LY_MATCH_SYNCp.qp_new());
}

void GateBoy::tock_lcd2() {
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_lcd_pins(wire SACU_CLKPIPE_evn) {
  lcd.PIN_50_LCD_DATA1.reset_for_pass();
  lcd.PIN_51_LCD_DATA0.reset_for_pass();
  lcd.PIN_54_LCD_HSYNC.reset_for_pass();
  lcd.PIN_56_LCD_FLIPS.reset_for_pass();
  lcd.PIN_52_LCD_CNTRL.reset_for_pass();
  lcd.PIN_55_LCD_LATCH.reset_for_pass();
  lcd.PIN_53_LCD_CLOCK.reset_for_pass();
  lcd.PIN_57_LCD_VSYNC.reset_for_pass();

  /*#p21.TOCU*/ wire _TOCU_LX0n = not1(reg_lx.SAXO_LX0p.qp_new());
  /*#p21.VEPE*/ wire _VEPE_LX1n = not1(reg_lx.TYPO_LX1p.qp_new());
  /* p21.VUTY*/ wire _VUTY_LX2n = not1(reg_lx.VYZO_LX2p.qp_new());
  /* p21.VATE*/ wire _VATE_LX3n = not1(reg_lx.TELU_LX3p.qp_new());
  /* p21.TUDA*/ wire _TUDA_LX4n = not1(reg_lx.SUDE_LX4p.qp_new());
  /* p21.TAFY*/ wire _TAFY_LX5n = not1(reg_lx.TAHA_LX5p.qp_new());
  /* p21.TUJU*/ wire _TUJU_LX6n = not1(reg_lx.TYRY_LX6p.qp_new());

  /* p21.VOKU*/ wire _VOKU_LX000n = nand7(_TUJU_LX6n,         _TAFY_LX5n,         _TUDA_LX4n,         _VATE_LX3n,         _VUTY_LX2n,         _VEPE_LX1n,         _TOCU_LX0n); // 0000000 == 0
  /* p21.TOZU*/ wire _TOZU_LX007n = nand7(_TUJU_LX6n,         _TAFY_LX5n,         _TUDA_LX4n,         _VATE_LX3n,         reg_lx.VYZO_LX2p.qp_new(), reg_lx.TYPO_LX1p.qp_new(), reg_lx.SAXO_LX0p.qp_new()); // 0000111 == 7
  /* p21.TECE*/ wire _TECE_LX045n = nand7(_TUJU_LX6n,         reg_lx.TAHA_LX5p.qp_new(), _TUDA_LX4n,         reg_lx.TELU_LX3p.qp_new(), reg_lx.VYZO_LX2p.qp_new(), _VEPE_LX1n,         reg_lx.SAXO_LX0p.qp_new()); // 0101101 == 45
  /*#p21.TEBO*/ wire _TEBO_LX083n = nand7(reg_lx.TYRY_LX6p.qp_new(), _TAFY_LX5n,         reg_lx.SUDE_LX4p.qp_new(), _VATE_LX3n,         _VUTY_LX2n,         reg_lx.TYPO_LX1p.qp_new(), reg_lx.SAXO_LX0p.qp_new()); // 1010011 == 83

  /*#p21.TEGY*/ wire _TEGY_STROBE = nand4(_VOKU_LX000n,       _TOZU_LX007n,       _TECE_LX045n, _TEBO_LX083n);

  // FIXME old/new - tegy should be old
  /*#p21.SYGU*/ lcd.SYGU_LINE_STROBE.dff17(SONO_ABxxxxGH(), LYFE_VID_RSTn(), _TEGY_STROBE);
  /*#p21.RYNO*/ wire _RYNO = or2(lcd.SYGU_LINE_STROBE.qp_new(), reg_lx.RUTU_x113p.qp_new());
  /*#p21.POGU*/ wire _POGU = not1(_RYNO);
  /*PIN_52*/ lcd.PIN_52_LCD_CNTRL.pin_out_dp(_POGU);

  // if LCDC_ENn, PIN_56_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)

  /*#p24.LOFU*/ wire LOFU_x113n = not1(reg_lx.RUTU_x113p.qp_new());
  /*#p24.LUCA*/ lcd.LUCA_LINE_EVENp .dff17(LOFU_x113n, LYFE_VID_RSTn(), lcd.LUCA_LINE_EVENp.qn_old());
  /*#p21.NAPO*/ lcd.NAPO_FRAME_EVENp.dff17(lcd.POPU_VBLANKp.qp_new(),   LYFE_VID_RSTn(), lcd.NAPO_FRAME_EVENp.qn_old());

  /*#p24.MAGU*/ wire _MAGU = xor2(lcd.NAPO_FRAME_EVENp.qp_new(), lcd.LUCA_LINE_EVENp.qn_new());
  /*#p24.MECO*/ wire _MECO = not1(_MAGU);
  /*#p24.KEBO*/ wire _KEBO = not1(_MECO);
  /* p01.UREK*/ wire _UREK_DIV07n = not1(div.TULU_DIV07p.qp_new());
  /*#p24.USEC*/ wire _USEC_DIV07p = not1(_UREK_DIV07n);
  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(reg_lcdc.XONA_LCDC_LCDENn.qn_new());
  /*#p24.KUPA*/ wire _KUPA = amux2(reg_lcdc.XONA_LCDC_LCDENn.qn_new(), _KEBO, _KEDY_LCDC_ENn, _USEC_DIV07p);
  /*#p24.KOFO*/ wire _KOFO = not1(_KUPA);
  /*PIN_56*/ lcd.PIN_56_LCD_FLIPS.pin_out_dp(_KOFO);

  /*#p24.NERU*/ wire _NERU_VSYNCp = nor8(reg_ly.LAFO_LY7p.qp_new(), reg_ly.LOVU_LY4p.qp_new(), reg_ly.LYDO_LY3p.qp_new(), reg_ly.MUWY_LY0p.qp_new(), reg_ly.MYRO_LY1p.qp_new(), reg_ly.LEXA_LY2p.qp_new(), reg_ly.LEMA_LY5p.qp_new(), reg_ly.MATO_LY6p.qp_new());
  /*#p24.MEDA*/ lcd.MEDA_VSYNC_OUTn.dff17(reg_lx.NYPE_x113p.qn_new(), LYFE_VID_RSTn(), _NERU_VSYNCp);
  /*#p24.MURE*/ wire _MURE_VSYNC = not1(lcd.MEDA_VSYNC_OUTn.qp_new());
  /*PIN_57*/ lcd.PIN_57_LCD_VSYNC.pin_out_dp(_MURE_VSYNC);

  // FIXME inversion
  // I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
  // seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
  // Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
  // That would be a loooot of gate delay.
  // Could we possibly be incrementing X3p one phase early?

  // LCD horizontal sync pin latch
  /*#p24.POME*/ lcd.POME = nor2(sprite_scanner.AVAP_SCAN_DONE_TRIGp.qp_new(), lcd.POFY.qp_mid());
  /*#p24.RUJU*/ lcd.RUJU = or3(lcd.PAHO_X_8_SYNC.qp_new(), TOFU_VID_RSTp(), lcd.POME.qp_mid());
  /*#p24.POFY*/ lcd.POFY = not1(lcd.RUJU.qp_mid());
  /*#p24.POME*/ lcd.POME = nor2(sprite_scanner.AVAP_SCAN_DONE_TRIGp.qp_new(), lcd.POFY.qp_mid());
  /*#p24.RUJU*/ lcd.RUJU = or3(lcd.PAHO_X_8_SYNC.qp_new(), TOFU_VID_RSTp(), lcd.POME.qp_mid());
  /*#p24.POFY*/ lcd.POFY = not1(lcd.RUJU.qp_mid());

  /*#p24.RUZE*/ wire _RUZE_HSYNCn = not1(lcd.POFY.qp_mid());
  /*PIN_54*/ lcd.PIN_54_LCD_HSYNC.pin_out_dp( _RUZE_HSYNCn);

  /*PIN_51*/ lcd.PIN_51_LCD_DATA0.pin_out_dp(pix_pipes.REMY_LD0n.qp_new());
  /*PIN_50*/ lcd.PIN_50_LCD_DATA1.pin_out_dp(pix_pipes.RAVO_LD1n.qp_new());

  /* p01.UMEK*/ wire _UMEK_DIV06n = not1(div.UGOT_DIV06p.qp_new());
  /*#p21.PURE*/ wire _PURE_LINE_ENDn = not1(reg_lx.RUTU_x113p.qp_new());
  /*#p24.KASA*/ wire _KASA_LINE_ENDp = not1(_PURE_LINE_ENDn);
  /*#p24.UMOB*/ wire _UMOB_DIV_06p = not1(_UMEK_DIV06n);
  /*#p24.KAHE*/ wire _KAHE_LINE_ENDp = amux2(reg_lcdc.XONA_LCDC_LCDENn.qn_new(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, _UMOB_DIV_06p);
  /*#p24.KYMO*/ wire _KYMO_LINE_ENDn = not1(_KAHE_LINE_ENDp);
  /*PIN_55*/ lcd.PIN_55_LCD_LATCH.pin_out_dp(_KYMO_LINE_ENDn);

  /*#p21.XAJO*/ wire _XAJO_X_009p = and2(pix_count.XEHO_PX0p.qp_new(), pix_count.XYDO_PX3p.qp_new());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), VOGA_HBLANKp.qp_new());
  /*#p21.WUSA*/ lcd.WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p, WEGO_HBLANKp);
  /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK = and2(lcd.WUSA_LCD_CLOCK_GATE.qp_new(), SACU_CLKPIPE_evn);
  /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_new(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_new());
  /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, _POVA_FINE_MATCH_TRIGp);
  /*#p21.RYPO*/ wire _RYPO_LCD_CLOCK = not1(_SEMU_LCD_CLOCK);
  /*PIN_53*/ lcd.PIN_53_LCD_CLOCK.pin_out_dp(_RYPO_LCD_CLOCK);

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

//------------------------------------------------------------------------------------------------------------------------