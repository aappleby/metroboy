#include "GateBoyLib/GateBoyLCD.h"

#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_lyc_gates(const GateBoyState& reg_old) {
  {
    // Inverting ROPO's clock and making it store the new match instead of the old match fixes lcdon_to_stat2_a but breaks other things
    // Just making it store the new match doesn't break anything.
    /*_p21.RYME*/ wire RYME_LY_MATCH0n_old = xor2(gb_state.reg_ly.MUWY_LY0p_odd.qp_old(), gb_state.reg_lyc.SYRY_LYC0n.qn_old());
    /*_p21.TYDE*/ wire TYDE_LY_MATCH1n_old = xor2(gb_state.reg_ly.MYRO_LY1p_odd.qp_old(), gb_state.reg_lyc.VUCE_LYC1n.qn_old());
    /*_p21.REDA*/ wire REDA_LY_MATCH2n_old = xor2(gb_state.reg_ly.LEXA_LY2p_odd.qp_old(), gb_state.reg_lyc.SEDY_LYC2n.qn_old());
    /*_p21.RASY*/ wire RASY_LY_MATCH3n_old = xor2(gb_state.reg_ly.LYDO_LY3p_odd.qp_old(), gb_state.reg_lyc.SALO_LYC3n.qn_old());
    /*_p21.TYKU*/ wire TYKU_LY_MATCH4n_old = xor2(gb_state.reg_ly.LOVU_LY4p_odd.qp_old(), gb_state.reg_lyc.SOTA_LYC4n.qn_old());
    /*_p21.TUCY*/ wire TUCY_LY_MATCH5n_old = xor2(gb_state.reg_ly.LEMA_LY5p_odd.qp_old(), gb_state.reg_lyc.VAFA_LYC5n.qn_old());
    /*_p21.TERY*/ wire TERY_LY_MATCH6n_old = xor2(gb_state.reg_ly.MATO_LY6p_odd.qp_old(), gb_state.reg_lyc.VEVO_LYC6n.qn_old());
    /*_p21.SYFU*/ wire SYFU_LY_MATCH7n_old = xor2(gb_state.reg_ly.LAFO_LY7p_odd.qp_old(), gb_state.reg_lyc.RAHA_LYC7n.qn_old());

    /*#p21.SOVU*/ wire SOVU_LY_MATCHA_old = nor4 (SYFU_LY_MATCH7n_old, TERY_LY_MATCH6n_old, TUCY_LY_MATCH5n_old, TYKU_LY_MATCH4n_old);
    /*#p21.SUBO*/ wire SUBO_LY_MATCHB_old = nor4 (RASY_LY_MATCH3n_old, REDA_LY_MATCH2n_old, TYDE_LY_MATCH1n_old, RYME_LY_MATCH0n_old);
    /*#p21.RAPE*/ wire RAPE_LY_MATCHn_old = nand2(SOVU_LY_MATCHA_old,  SUBO_LY_MATCHB_old);
    /*#p21.PALY*/ wire PALY_LY_MATCHa_old = not1 (RAPE_LY_MATCHn_old);

    /*#p21.ROPO*/ gb_state.int_ctrl.ROPO_LY_MATCH_SYNCp.dff17(gb_state.sys_clk.TALU_xxCDEFxx(), gb_state.sys_rst.WESY_SYS_RSTn(), PALY_LY_MATCHa_old);
  }

  {
    /*_p23.XUFA*/ wire XUFA_FF45_WRn = and2(gb_state.cpu_signals.CUPA_CPU_WRp(), gb_state.cpu_abus.XAYU_FF45p());
    /*_p23.WANE*/ wire WANE_FF45_WRp = not1(XUFA_FF45_WRn);
    /*_p23.SYRY*/ gb_state.reg_lyc.SYRY_LYC0n.dff9(WANE_FF45_WRp, gb_state.sys_rst.WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p23.VUCE*/ gb_state.reg_lyc.VUCE_LYC1n.dff9(WANE_FF45_WRp, gb_state.sys_rst.WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p23.SEDY*/ gb_state.reg_lyc.SEDY_LYC2n.dff9(WANE_FF45_WRp, gb_state.sys_rst.WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p23.SALO*/ gb_state.reg_lyc.SALO_LYC3n.dff9(WANE_FF45_WRp, gb_state.sys_rst.WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p23.SOTA*/ gb_state.reg_lyc.SOTA_LYC4n.dff9(WANE_FF45_WRp, gb_state.sys_rst.WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p23.VAFA*/ gb_state.reg_lyc.VAFA_LYC5n.dff9(WANE_FF45_WRp, gb_state.sys_rst.WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p23.VEVO*/ gb_state.reg_lyc.VEVO_LYC6n.dff9(WANE_FF45_WRp, gb_state.sys_rst.WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p23.RAHA*/ gb_state.reg_lyc.RAHA_LYC7n.dff9(WANE_FF45_WRp, gb_state.sys_rst.WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

    /*_p23.XYLY*/ wire XYLY_FF45_RDp = and2(gb_state.cpu_signals.ASOT_CPU_RDp(), gb_state.cpu_abus.XAYU_FF45p());
    /*_p23.WEKU*/ wire WEKU_FF45_RDn = not1(XYLY_FF45_RDp);

    /*#p23.RETU*/ triwire RETU_LYC0_TO_CD0 = tri6_nn(WEKU_FF45_RDn, gb_state.reg_lyc.SYRY_LYC0n.qp_new());
    /*_p23.VOJO*/ triwire VOJO_LYC1_TO_CD1 = tri6_nn(WEKU_FF45_RDn, gb_state.reg_lyc.VUCE_LYC1n.qp_new());
    /*_p23.RAZU*/ triwire RAZU_LYC2_TO_CD2 = tri6_nn(WEKU_FF45_RDn, gb_state.reg_lyc.SEDY_LYC2n.qp_new());
    /*_p23.REDY*/ triwire REDY_LYC3_TO_CD3 = tri6_nn(WEKU_FF45_RDn, gb_state.reg_lyc.SALO_LYC3n.qp_new());
    /*_p23.RACE*/ triwire RACE_LYC4_TO_CD4 = tri6_nn(WEKU_FF45_RDn, gb_state.reg_lyc.SOTA_LYC4n.qp_new());
    /*#p23.VAZU*/ triwire VAZU_LYC5_TO_CD5 = tri6_nn(WEKU_FF45_RDn, gb_state.reg_lyc.VAFA_LYC5n.qp_new());
    /*_p23.VAFE*/ triwire VAFE_LYC6_TO_CD6 = tri6_nn(WEKU_FF45_RDn, gb_state.reg_lyc.VEVO_LYC6n.qp_new());
    /*_p23.PUFY*/ triwire PUFY_LYC7_TO_CD7 = tri6_nn(WEKU_FF45_RDn, gb_state.reg_lyc.RAHA_LYC7n.qp_new());

    /*_BUS_CPU_D00p*/ gb_state.cpu_dbus.BUS_CPU_D00p.tri_bus(RETU_LYC0_TO_CD0);
    /*_BUS_CPU_D01p*/ gb_state.cpu_dbus.BUS_CPU_D01p.tri_bus(VOJO_LYC1_TO_CD1);
    /*_BUS_CPU_D02p*/ gb_state.cpu_dbus.BUS_CPU_D02p.tri_bus(RAZU_LYC2_TO_CD2);
    /*_BUS_CPU_D03p*/ gb_state.cpu_dbus.BUS_CPU_D03p.tri_bus(REDY_LYC3_TO_CD3);
    /*_BUS_CPU_D04p*/ gb_state.cpu_dbus.BUS_CPU_D04p.tri_bus(RACE_LYC4_TO_CD4);
    /*_BUS_CPU_D05p*/ gb_state.cpu_dbus.BUS_CPU_D05p.tri_bus(VAZU_LYC5_TO_CD5);
    /*_BUS_CPU_D06p*/ gb_state.cpu_dbus.BUS_CPU_D06p.tri_bus(VAFE_LYC6_TO_CD6);
    /*_BUS_CPU_D07p*/ gb_state.cpu_dbus.BUS_CPU_D07p.tri_bus(PUFY_LYC7_TO_CD7);
  }

  {
    /*_p21.SEPA*/ wire SEPA_FF41_WRp = and2(gb_state.cpu_signals.CUPA_CPU_WRp(), gb_state.cpu_abus.VARY_FF41p());
    /*_p21.RYJU*/ wire RYJU_FF41_WRn = not1(SEPA_FF41_WRp);
    /*_p21.PAGO*/ wire PAGO_LYC_MATCH_RST = or2(gb_state.sys_rst.WESY_SYS_RSTn(), RYJU_FF41_WRn);
    /*_p21.RUPO*/ gb_state.int_ctrl.RUPO_LYC_MATCHn.nor_latch(PAGO_LYC_MATCH_RST, gb_state.int_ctrl.ROPO_LY_MATCH_SYNCp.qp_new());
  }
}

//-----------------------------------------------------------------------------

void GateBoy::tock_lcd_gates() {
  {
    /*#p21.XYVO*/ wire XYVO_y144p = and2(gb_state.reg_ly.LOVU_LY4p_odd.qp_old(), gb_state.reg_ly.LAFO_LY7p_odd.qp_old()); // 128 + 16 = 144
    /*#p21.NOKO*/ wire NOKO_y153p = and4(gb_state.reg_ly.LAFO_LY7p_odd.qp_old(), gb_state.reg_ly.LOVU_LY4p_odd.qp_old(), gb_state.reg_ly.LYDO_LY3p_odd.qp_old(), gb_state.reg_ly.MUWY_LY0p_odd.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153

    /*#p28.ANEL*/ gb_state.lcd.ANEL_x113p_odd.dff17(gb_state.sys_clk.AWOH_xxCDxxGH(), ABEZ_VID_RSTn(), gb_state.lcd.CATU_x113p_odd.qp_old());

    /*#p21.PURE*/ wire PURE_x113n = not1(gb_state.lcd.RUTU_LINE_ENDp_odd.qp_old());
    /*#p29.ALES*/ wire ALES_y144n = not1(XYVO_y144p);
    /*#p21.SELA*/ wire SELA_x113p = not1(PURE_x113n);
    /*#p29.ABOV*/ wire ABOV_x113p = and2(SELA_x113p, ALES_y144n);
    /*#p29.CATU*/ gb_state.lcd.CATU_x113p_odd.dff17(gb_state.sys_clk.XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), ABOV_x113p);

    /*#p21.NYPE*/ gb_state.lcd.NYPE_LINE_ENDp_odd.dff17(gb_state.sys_clk.TALU_xxCDEFxx(),         LYFE_VID_RSTn(), gb_state.lcd.RUTU_LINE_ENDp_odd.qp_old());
    /*#p21.SANU*/ wire SANU_x113p_odd_old = and4(gb_state.reg_lx.TYRY_LX6p_odd.qp_old(), gb_state.reg_lx.TAHA_LX5p_odd.qp_old(), gb_state.reg_lx.SUDE_LX4p_odd.qp_old(), gb_state.reg_lx.SAXO_LX0p_odd.qp_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*#p21.RUTU*/ gb_state.lcd.RUTU_LINE_ENDp_odd.dff17(gb_state.sys_clk.SONO_ABxxxxGH(),         LYFE_VID_RSTn(), SANU_x113p_odd_old);

    /*_p28.ABAF*/ wire ABAF_x113n_odd = not1(gb_state.lcd.CATU_x113p_odd.qp_new());
    /*_p28.BYHA*/ wire BYHA_LINE_RSTn_odd = or_and3(gb_state.lcd.ANEL_x113p_odd.qp_new(), ABAF_x113n_odd, ABEZ_VID_RSTn()); // so if this is or_and, BYHA should go low on 910 and 911
    /*_p28.ATEJ*/ gb_state.ATEJ_LINE_RSTp_odd = not1(BYHA_LINE_RSTn_odd);

    /*#p21.POPU*/ gb_state.lcd.POPU_VBLANKp_odd.dff17(gb_state.lcd.NYPE_LINE_ENDp_odd.qp_new(), LYFE_VID_RSTn(), XYVO_y144p);
    /*#p21.MYTA*/ gb_state.lcd.MYTA_FRAME_ENDp_odd.dff17(gb_state.lcd.NYPE_LINE_ENDp_odd.qp_new(), LYFE_VID_RSTn(), NOKO_y153p);
  }

  {
    /*#p21.TOCU*/ wire TOCU_LX0n_old = not1(gb_state.reg_lx.SAXO_LX0p_odd.qp_old());
    /*#p21.VEPE*/ wire VEPE_LX1n_old = not1(gb_state.reg_lx.TYPO_LX1p_odd.qp_old());
    /*_p21.VUTY*/ wire VUTY_LX2n_old = not1(gb_state.reg_lx.VYZO_LX2p_odd.qp_old());
    /*_p21.VATE*/ wire VATE_LX3n_old = not1(gb_state.reg_lx.TELU_LX3p_odd.qp_old());
    /*_p21.TUDA*/ wire TUDA_LX4n_old = not1(gb_state.reg_lx.SUDE_LX4p_odd.qp_old());
    /*_p21.TAFY*/ wire TAFY_LX5n_old = not1(gb_state.reg_lx.TAHA_LX5p_odd.qp_old());
    /*_p21.TUJU*/ wire TUJU_LX6n_old = not1(gb_state.reg_lx.TYRY_LX6p_odd.qp_old());

    /*_p21.VOKU*/ wire VOKU_LX000n_old = nand7(TUJU_LX6n_old,                          TAFY_LX5n_old,                          TUDA_LX4n_old,                          VATE_LX3n_old,                          VUTY_LX2n_old,                          VEPE_LX1n_old,                          TOCU_LX0n_old);                          // 0000000 == 0
    /*_p21.TOZU*/ wire TOZU_LX007n_old = nand7(TUJU_LX6n_old,                          TAFY_LX5n_old,                          TUDA_LX4n_old,                          VATE_LX3n_old,                          gb_state.reg_lx.VYZO_LX2p_odd.qp_old(), gb_state.reg_lx.TYPO_LX1p_odd.qp_old(), gb_state.reg_lx.SAXO_LX0p_odd.qp_old()); // 0000111 == 7
    /*_p21.TECE*/ wire TECE_LX045n_old = nand7(TUJU_LX6n_old,                          gb_state.reg_lx.TAHA_LX5p_odd.qp_old(), TUDA_LX4n_old,                          gb_state.reg_lx.TELU_LX3p_odd.qp_old(), gb_state.reg_lx.VYZO_LX2p_odd.qp_old(), VEPE_LX1n_old,                          gb_state.reg_lx.SAXO_LX0p_odd.qp_old()); // 0101101 == 45
    /*#p21.TEBO*/ wire TEBO_LX083n_old = nand7(gb_state.reg_lx.TYRY_LX6p_odd.qp_old(), TAFY_LX5n_old,                          gb_state.reg_lx.SUDE_LX4p_odd.qp_old(), VATE_LX3n_old,                          VUTY_LX2n_old,                          gb_state.reg_lx.TYPO_LX1p_odd.qp_old(), gb_state.reg_lx.SAXO_LX0p_odd.qp_old()); // 1010011 == 83

    /*#p21.TEGY*/ wire TEGY_STROBE_old = nand4(VOKU_LX000n_old, TOZU_LX007n_old, TECE_LX045n_old, TEBO_LX083n_old);

    /*#p21.SYGU*/ gb_state.lcd.SYGU_LINE_STROBE.dff17(gb_state.sys_clk.SONO_ABxxxxGH(), LYFE_VID_RSTn(), TEGY_STROBE_old);
  }

  {
    /*#p21.MUDE*/ wire MUDE_X_RSTn = nor2(gb_state.lcd.RUTU_LINE_ENDp_odd.qp_new(), LYHA_VID_RSTp());
    /*#p21.SAXO*/ gb_state.reg_lx.SAXO_LX0p_odd.dff17(gb_state.sys_clk.TALU_xxCDEFxx(),   MUDE_X_RSTn, gb_state.reg_lx.SAXO_LX0p_odd.qn_old());
    /*#p21.TYPO*/ gb_state.reg_lx.TYPO_LX1p_odd.dff17(gb_state.reg_lx.SAXO_LX0p_odd.qn_new(), MUDE_X_RSTn, gb_state.reg_lx.TYPO_LX1p_odd.qn_old());
    /*#p21.VYZO*/ gb_state.reg_lx.VYZO_LX2p_odd.dff17(gb_state.reg_lx.TYPO_LX1p_odd.qn_new(), MUDE_X_RSTn, gb_state.reg_lx.VYZO_LX2p_odd.qn_old());
    /*#p21.TELU*/ gb_state.reg_lx.TELU_LX3p_odd.dff17(gb_state.reg_lx.VYZO_LX2p_odd.qn_new(), MUDE_X_RSTn, gb_state.reg_lx.TELU_LX3p_odd.qn_old());
    /*#p21.SUDE*/ gb_state.reg_lx.SUDE_LX4p_odd.dff17(gb_state.reg_lx.TELU_LX3p_odd.qn_new(), MUDE_X_RSTn, gb_state.reg_lx.SUDE_LX4p_odd.qn_old());
    /*#p21.TAHA*/ gb_state.reg_lx.TAHA_LX5p_odd.dff17(gb_state.reg_lx.SUDE_LX4p_odd.qn_new(), MUDE_X_RSTn, gb_state.reg_lx.TAHA_LX5p_odd.qn_old());
    /*#p21.TYRY*/ gb_state.reg_lx.TYRY_LX6p_odd.dff17(gb_state.reg_lx.TAHA_LX5p_odd.qn_new(), MUDE_X_RSTn, gb_state.reg_lx.TYRY_LX6p_odd.qn_old());
  }

  {
    /*#p21.LAMA*/ wire LAMA_Y_RSTn = nor2(gb_state.lcd.MYTA_FRAME_ENDp_odd.qp_new(), LYHA_VID_RSTp());
    /*#p21.MUWY*/ gb_state.reg_ly.MUWY_LY0p_odd.dff17(gb_state.lcd.RUTU_LINE_ENDp_odd.qp_new(), LAMA_Y_RSTn, gb_state.reg_ly.MUWY_LY0p_odd.qn_old());
    /*#p21.MYRO*/ gb_state.reg_ly.MYRO_LY1p_odd.dff17(gb_state.reg_ly.MUWY_LY0p_odd.qn_new(),   LAMA_Y_RSTn, gb_state.reg_ly.MYRO_LY1p_odd.qn_old());
    /*#p21.LEXA*/ gb_state.reg_ly.LEXA_LY2p_odd.dff17(gb_state.reg_ly.MYRO_LY1p_odd.qn_new(),   LAMA_Y_RSTn, gb_state.reg_ly.LEXA_LY2p_odd.qn_old());
    /*#p21.LYDO*/ gb_state.reg_ly.LYDO_LY3p_odd.dff17(gb_state.reg_ly.LEXA_LY2p_odd.qn_new(),   LAMA_Y_RSTn, gb_state.reg_ly.LYDO_LY3p_odd.qn_old());
    /*#p21.LOVU*/ gb_state.reg_ly.LOVU_LY4p_odd.dff17(gb_state.reg_ly.LYDO_LY3p_odd.qn_new(),   LAMA_Y_RSTn, gb_state.reg_ly.LOVU_LY4p_odd.qn_old());
    /*#p21.LEMA*/ gb_state.reg_ly.LEMA_LY5p_odd.dff17(gb_state.reg_ly.LOVU_LY4p_odd.qn_new(),   LAMA_Y_RSTn, gb_state.reg_ly.LEMA_LY5p_odd.qn_old());
    /*#p21.MATO*/ gb_state.reg_ly.MATO_LY6p_odd.dff17(gb_state.reg_ly.LEMA_LY5p_odd.qn_new(),   LAMA_Y_RSTn, gb_state.reg_ly.MATO_LY6p_odd.qn_old());
    /*#p21.LAFO*/ gb_state.reg_ly.LAFO_LY7p_odd.dff17(gb_state.reg_ly.MATO_LY6p_odd.qn_new(),   LAMA_Y_RSTn, gb_state.reg_ly.LAFO_LY7p_odd.qn_old());
  }

  {
    /*_p23.WAFU*/ wire WAFU_FF44_RDp = and2(gb_state.cpu_signals.ASOT_CPU_RDp(), gb_state.cpu_abus.XOGY_FF44p());
    /*_p23.VARO*/ wire VARO_FF44_RDn = not1(WAFU_FF44_RDp);

    /*#p23.WURY*/ wire WURY_LY0n = not1(gb_state.reg_ly.MUWY_LY0p_odd.qp_new());
    /*_p23.XEPO*/ wire XEPO_LY1n = not1(gb_state.reg_ly.MYRO_LY1p_odd.qp_new());
    /*_p23.MYFA*/ wire MYFA_LY2n = not1(gb_state.reg_ly.LEXA_LY2p_odd.qp_new());
    /*_p23.XUHY*/ wire XUHY_LY3n = not1(gb_state.reg_ly.LYDO_LY3p_odd.qp_new());
    /*_p23.WATA*/ wire WATA_LY4n = not1(gb_state.reg_ly.LOVU_LY4p_odd.qp_new());
    /*_p23.XAGA*/ wire XAGA_LY5n = not1(gb_state.reg_ly.LEMA_LY5p_odd.qp_new());
    /*_p23.XUCE*/ wire XUCE_LY6n = not1(gb_state.reg_ly.MATO_LY6p_odd.qp_new());
    /*_p23.XOWO*/ wire XOWO_LY7n = not1(gb_state.reg_ly.LAFO_LY7p_odd.qp_new());

    /*_p23.VEGA*/ triwire VEGA_LY0_TO_CD0 = tri6_nn(VARO_FF44_RDn, WURY_LY0n);
    /*_p23.WUVA*/ triwire WUVA_LY1_TO_CD1 = tri6_nn(VARO_FF44_RDn, XEPO_LY1n);
    /*_p23.LYCO*/ triwire LYCO_LY2_TO_CD2 = tri6_nn(VARO_FF44_RDn, MYFA_LY2n);
    /*_p23.WOJY*/ triwire WOJY_LY3_TO_CD3 = tri6_nn(VARO_FF44_RDn, XUHY_LY3n);
    /*_p23.VYNE*/ triwire VYNE_LY4_TO_CD4 = tri6_nn(VARO_FF44_RDn, WATA_LY4n);
    /*_p23.WAMA*/ triwire WAMA_LY5_TO_CD5 = tri6_nn(VARO_FF44_RDn, XAGA_LY5n);
    /*_p23.WAVO*/ triwire WAVO_LY6_TO_CD6 = tri6_nn(VARO_FF44_RDn, XUCE_LY6n);
    /*_p23.WEZE*/ triwire WEZE_LY7_TO_CD7 = tri6_nn(VARO_FF44_RDn, XOWO_LY7n);

    /*_BUS_CPU_D00p*/ gb_state.cpu_dbus.BUS_CPU_D00p.tri_bus(VEGA_LY0_TO_CD0);
    /*_BUS_CPU_D01p*/ gb_state.cpu_dbus.BUS_CPU_D01p.tri_bus(WUVA_LY1_TO_CD1);
    /*_BUS_CPU_D02p*/ gb_state.cpu_dbus.BUS_CPU_D02p.tri_bus(LYCO_LY2_TO_CD2);
    /*_BUS_CPU_D03p*/ gb_state.cpu_dbus.BUS_CPU_D03p.tri_bus(WOJY_LY3_TO_CD3);
    /*_BUS_CPU_D04p*/ gb_state.cpu_dbus.BUS_CPU_D04p.tri_bus(VYNE_LY4_TO_CD4);
    /*_BUS_CPU_D05p*/ gb_state.cpu_dbus.BUS_CPU_D05p.tri_bus(WAMA_LY5_TO_CD5);
    /*_BUS_CPU_D06p*/ gb_state.cpu_dbus.BUS_CPU_D06p.tri_bus(WAVO_LY6_TO_CD6);
    /*_BUS_CPU_D07p*/ gb_state.cpu_dbus.BUS_CPU_D07p.tri_bus(WEZE_LY7_TO_CD7);
  }
}

//-----------------------------------------------------------------------------

void GateBoy::set_lcd_pins_gates(wire SACU_CLKPIPE_evn) {
  /*#p21.RYNO*/ wire RYNO = or2(gb_state.lcd.SYGU_LINE_STROBE.qp_new(), gb_state.lcd.RUTU_LINE_ENDp_odd.qp_new());
  /*#p21.POGU*/ wire POGU = not1(RYNO);
  /*_PIN_52*/ pins.lcd.PIN_52_LCD_CNTRL.pin_out(POGU, POGU);

  // if LCDC_ENn, PIN_56_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)

  /*#p24.LOFU*/ wire LOFU_x113n = not1(gb_state.lcd.RUTU_LINE_ENDp_odd.qp_new());
  /*#p24.LUCA*/ gb_state.lcd.LUCA_LINE_EVENp .dff17(LOFU_x113n, LYFE_VID_RSTn(), gb_state.lcd.LUCA_LINE_EVENp.qn_old());
  /*#p21.NAPO*/ gb_state.lcd.NAPO_FRAME_EVENp.dff17(gb_state.lcd.POPU_VBLANKp_odd.qp_new(),   LYFE_VID_RSTn(), gb_state.lcd.NAPO_FRAME_EVENp.qn_old());

  /*#p24.MAGU*/ wire MAGU = xor2(gb_state.lcd.NAPO_FRAME_EVENp.qp_new(), gb_state.lcd.LUCA_LINE_EVENp.qn_new());
  /*#p24.MECO*/ wire MECO = not1(MAGU);
  /*#p24.KEBO*/ wire KEBO = not1(MECO);
  /*_p01.UREK*/ wire UREK_DIV07n = not1(gb_state.reg_div.TULU_DIV07p.qp_new());
  /*#p24.USEC*/ wire USEC_DIV07p = not1(UREK_DIV07n);
  /*#p24.KEDY*/ wire KEDY_LCDC_ENn = not1(gb_state.reg_lcdc.XONA_LCDC_LCDENn.qn_new());
  /*#p24.KUPA*/ wire KUPA = amux2(gb_state.reg_lcdc.XONA_LCDC_LCDENn.qn_new(), KEBO, KEDY_LCDC_ENn, USEC_DIV07p);
  /*#p24.KOFO*/ wire KOFO = not1(KUPA);
  /*_PIN_56*/ pins.lcd.PIN_56_LCD_FLIPS.pin_out(KOFO, KOFO);

  /*#p24.NERU*/ wire NERU_VSYNCp = nor8(gb_state.reg_ly.LAFO_LY7p_odd.qp_new(), gb_state.reg_ly.LOVU_LY4p_odd.qp_new(), gb_state.reg_ly.LYDO_LY3p_odd.qp_new(), gb_state.reg_ly.MUWY_LY0p_odd.qp_new(), gb_state.reg_ly.MYRO_LY1p_odd.qp_new(), gb_state.reg_ly.LEXA_LY2p_odd.qp_new(), gb_state.reg_ly.LEMA_LY5p_odd.qp_new(), gb_state.reg_ly.MATO_LY6p_odd.qp_new());
  /*#p24.MEDA*/ gb_state.lcd.MEDA_VSYNC_OUTn.dff17(gb_state.lcd.NYPE_LINE_ENDp_odd.qn_new(), LYFE_VID_RSTn(), NERU_VSYNCp);
  /*#p24.MURE*/ wire MURE_VSYNC = not1(gb_state.lcd.MEDA_VSYNC_OUTn.qp_new());
  /*_PIN_57*/ pins.lcd.PIN_57_LCD_VSYNC.pin_out(MURE_VSYNC, MURE_VSYNC);

  // FIXME inversion
  // I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
  // seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
  // Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
  // That would be a loooot of gate delay.
  // Could we possibly be incrementing X3p one phase early?

  // LCD horizontal sync pin latch
  /*#p24.POME*/ gb_state.lcd.POME_X8_LATCH = nor2(gb_state.sprite_scanner.AVAP_SCAN_DONE_tp_odd.out_new(), gb_state.lcd.POFY.out_mid());
  /*#p24.RUJU*/ gb_state.lcd.RUJU = or3(gb_state.lcd.PAHO_X8_SYNC.qp_new(), TOFU_VID_RSTp(), gb_state.lcd.POME_X8_LATCH.out_mid());
  /*#p24.POFY*/ gb_state.lcd.POFY = not1(gb_state.lcd.RUJU.out_mid());
  /*#p24.POME*/ gb_state.lcd.POME_X8_LATCH = nor2(gb_state.sprite_scanner.AVAP_SCAN_DONE_tp_odd.out_new(), gb_state.lcd.POFY.out_mid());
  /*#p24.RUJU*/ gb_state.lcd.RUJU = or3(gb_state.lcd.PAHO_X8_SYNC.qp_new(), TOFU_VID_RSTp(), gb_state.lcd.POME_X8_LATCH.out_mid());
  /*#p24.POFY*/ gb_state.lcd.POFY = not1(gb_state.lcd.RUJU.out_mid());

  /*#p24.RUZE*/ wire RUZE_HSYNCn = not1(gb_state.lcd.POFY.out_mid());
  /*_PIN_54*/ pins.lcd.PIN_54_LCD_HSYNC.pin_out(RUZE_HSYNCn, RUZE_HSYNCn);

  /*_PIN_51*/ pins.lcd.PIN_51_LCD_DATA0.pin_out(gb_state.lcd.REMY_LD0n.out_new(), gb_state.lcd.REMY_LD0n.out_new());
  /*_PIN_50*/ pins.lcd.PIN_50_LCD_DATA1.pin_out(gb_state.lcd.RAVO_LD1n.out_new(), gb_state.lcd.RAVO_LD1n.out_new());

  /*_p01.UMEK*/ wire UMEK_DIV06n = not1(gb_state.reg_div.UGOT_DIV06p.qp_new());
  /*#p21.PURE*/ wire PURE_x113n = not1(gb_state.lcd.RUTU_LINE_ENDp_odd.qp_new());
  /*#p24.KASA*/ wire KASA_LINE_ENDp = not1(PURE_x113n);
  /*#p24.UMOB*/ wire UMOB_DIV_06p = not1(UMEK_DIV06n);
  /*#p24.KAHE*/ wire KAHE_LINE_ENDp = amux2(gb_state.reg_lcdc.XONA_LCDC_LCDENn.qn_new(), KASA_LINE_ENDp, KEDY_LCDC_ENn, UMOB_DIV_06p);
  /*#p24.KYMO*/ wire KYMO_LINE_ENDn = not1(KAHE_LINE_ENDp);
  /*_PIN_55*/ pins.lcd.PIN_55_LCD_LATCH.pin_out(KYMO_LINE_ENDn, KYMO_LINE_ENDn);

  /*#p21.XAJO*/ wire XAJO_X_009p_odd = and2(gb_state.pix_count.XEHO_PX0p_odd.qp_new(), gb_state.pix_count.XYDO_PX3p_odd.qp_new());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), gb_state.VOGA_HBLANKp_evn.qp_new());
  /*#p21.WUSA*/ gb_state.lcd.WUSA_LCD_CLOCK_GATE.nor_latch(XAJO_X_009p_odd, WEGO_HBLANKp);
  /*#p21.TOBA*/ wire TOBA_LCD_CLOCK = and2(gb_state.lcd.WUSA_LCD_CLOCK_GATE.qp_new(), SACU_CLKPIPE_evn);
  /*#p27.POVA*/ wire POVA_FINE_MATCH_TRIGp = and2(gb_state.fine_scroll.PUXA_SCX_FINE_MATCH_evn.qp_new(), gb_state.fine_scroll.NYZE_SCX_FINE_MATCH_odd.qn_new());
  /*#p21.SEMU*/ wire SEMU_LCD_CLOCK = or2(TOBA_LCD_CLOCK, POVA_FINE_MATCH_TRIGp);
  /*#p21.RYPO*/ wire RYPO_LCD_CLOCK = not1(SEMU_LCD_CLOCK);
  /*_PIN_53*/ pins.lcd.PIN_53_LCD_CLOCK.pin_out(RYPO_LCD_CLOCK, RYPO_LCD_CLOCK);

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
