#include "GateBoyLib/GateBoyLCD.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_lyc_gates(const GateBoyState& reg_old) {
  auto& reg_new = gb_state;

  {
    // Inverting ROPO's clock and making it store the new match instead of the old match fixes lcdon_to_stat2_a but breaks other things
    // Just making it store the new match doesn't break anything.
    /*_p21.RYME*/ wire RYME_LY_MATCH0n_old = xor2(reg_old.reg_ly.MUWY_LY0p_odd.qp_old(), reg_old.reg_lyc.SYRY_LYC0n.qn_old());
    /*_p21.TYDE*/ wire TYDE_LY_MATCH1n_old = xor2(reg_old.reg_ly.MYRO_LY1p_odd.qp_old(), reg_old.reg_lyc.VUCE_LYC1n.qn_old());
    /*_p21.REDA*/ wire REDA_LY_MATCH2n_old = xor2(reg_old.reg_ly.LEXA_LY2p_odd.qp_old(), reg_old.reg_lyc.SEDY_LYC2n.qn_old());
    /*_p21.RASY*/ wire RASY_LY_MATCH3n_old = xor2(reg_old.reg_ly.LYDO_LY3p_odd.qp_old(), reg_old.reg_lyc.SALO_LYC3n.qn_old());
    /*_p21.TYKU*/ wire TYKU_LY_MATCH4n_old = xor2(reg_old.reg_ly.LOVU_LY4p_odd.qp_old(), reg_old.reg_lyc.SOTA_LYC4n.qn_old());
    /*_p21.TUCY*/ wire TUCY_LY_MATCH5n_old = xor2(reg_old.reg_ly.LEMA_LY5p_odd.qp_old(), reg_old.reg_lyc.VAFA_LYC5n.qn_old());
    /*_p21.TERY*/ wire TERY_LY_MATCH6n_old = xor2(reg_old.reg_ly.MATO_LY6p_odd.qp_old(), reg_old.reg_lyc.VEVO_LYC6n.qn_old());
    /*_p21.SYFU*/ wire SYFU_LY_MATCH7n_old = xor2(reg_old.reg_ly.LAFO_LY7p_odd.qp_old(), reg_old.reg_lyc.RAHA_LYC7n.qn_old());

    /*#p21.SOVU*/ wire SOVU_LY_MATCHA_old = nor4 (SYFU_LY_MATCH7n_old, TERY_LY_MATCH6n_old, TUCY_LY_MATCH5n_old, TYKU_LY_MATCH4n_old);
    /*#p21.SUBO*/ wire SUBO_LY_MATCHB_old = nor4 (RASY_LY_MATCH3n_old, REDA_LY_MATCH2n_old, TYDE_LY_MATCH1n_old, RYME_LY_MATCH0n_old);
    /*#p21.RAPE*/ wire RAPE_LY_MATCHn_old = nand2(SOVU_LY_MATCHA_old,  SUBO_LY_MATCHB_old);
    /*#p21.PALY*/ wire PALY_LY_MATCHa_old = not1 (RAPE_LY_MATCHn_old);

    /*#p21.ROPO*/ reg_new.int_ctrl.ROPO_LY_MATCH_SYNCp.dff17(reg_new.sys_clk.TALU_xxCDEFxx_new(), reg_new.sys_rst.WESY_SYS_RSTn_new(), PALY_LY_MATCHa_old);
  }

  {
    /*_p23.XUFA*/ wire XUFA_FF45_WRn_new = and2(reg_new.cpu_signals.CUPA_CPU_WRp_new(), reg_new.cpu_abus.XAYU_FF45p_new());
    /*_p23.WANE*/ wire WANE_FF45_WRp_new = not1(XUFA_FF45_WRn_new);
    /*_p23.SYRY*/ reg_new.reg_lyc.SYRY_LYC0n.dff9(WANE_FF45_WRp_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p23.VUCE*/ reg_new.reg_lyc.VUCE_LYC1n.dff9(WANE_FF45_WRp_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p23.SEDY*/ reg_new.reg_lyc.SEDY_LYC2n.dff9(WANE_FF45_WRp_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p23.SALO*/ reg_new.reg_lyc.SALO_LYC3n.dff9(WANE_FF45_WRp_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p23.SOTA*/ reg_new.reg_lyc.SOTA_LYC4n.dff9(WANE_FF45_WRp_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p23.VAFA*/ reg_new.reg_lyc.VAFA_LYC5n.dff9(WANE_FF45_WRp_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p23.VEVO*/ reg_new.reg_lyc.VEVO_LYC6n.dff9(WANE_FF45_WRp_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p23.RAHA*/ reg_new.reg_lyc.RAHA_LYC7n.dff9(WANE_FF45_WRp_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

    /*_p23.XYLY*/ wire XYLY_FF45_RDp_new = and2(reg_new.cpu_signals.ASOT_CPU_RDp_new(), reg_new.cpu_abus.XAYU_FF45p_new());
    /*_p23.WEKU*/ wire WEKU_FF45_RDn_new = not1(XYLY_FF45_RDp_new);

    /*#p23.RETU*/ triwire RETU_LYC0_TO_CD0_new = tri6_nn(WEKU_FF45_RDn_new, reg_new.reg_lyc.SYRY_LYC0n.qp_new());
    /*_p23.VOJO*/ triwire VOJO_LYC1_TO_CD1_new = tri6_nn(WEKU_FF45_RDn_new, reg_new.reg_lyc.VUCE_LYC1n.qp_new());
    /*_p23.RAZU*/ triwire RAZU_LYC2_TO_CD2_new = tri6_nn(WEKU_FF45_RDn_new, reg_new.reg_lyc.SEDY_LYC2n.qp_new());
    /*_p23.REDY*/ triwire REDY_LYC3_TO_CD3_new = tri6_nn(WEKU_FF45_RDn_new, reg_new.reg_lyc.SALO_LYC3n.qp_new());
    /*_p23.RACE*/ triwire RACE_LYC4_TO_CD4_new = tri6_nn(WEKU_FF45_RDn_new, reg_new.reg_lyc.SOTA_LYC4n.qp_new());
    /*#p23.VAZU*/ triwire VAZU_LYC5_TO_CD5_new = tri6_nn(WEKU_FF45_RDn_new, reg_new.reg_lyc.VAFA_LYC5n.qp_new());
    /*_p23.VAFE*/ triwire VAFE_LYC6_TO_CD6_new = tri6_nn(WEKU_FF45_RDn_new, reg_new.reg_lyc.VEVO_LYC6n.qp_new());
    /*_p23.PUFY*/ triwire PUFY_LYC7_TO_CD7_new = tri6_nn(WEKU_FF45_RDn_new, reg_new.reg_lyc.RAHA_LYC7n.qp_new());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(RETU_LYC0_TO_CD0_new);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(VOJO_LYC1_TO_CD1_new);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(RAZU_LYC2_TO_CD2_new);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(REDY_LYC3_TO_CD3_new);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(RACE_LYC4_TO_CD4_new);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(VAZU_LYC5_TO_CD5_new);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(VAFE_LYC6_TO_CD6_new);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(PUFY_LYC7_TO_CD7_new);
  }

  {
    /*_p21.SEPA*/ wire SEPA_FF41_WRp_new = and2(reg_new.cpu_signals.CUPA_CPU_WRp_new(), reg_new.cpu_abus.VARY_FF41p_new());
    /*_p21.RYJU*/ wire RYJU_FF41_WRn_new = not1(SEPA_FF41_WRp_new);
    /*_p21.PAGO*/ wire PAGO_LYC_MATCH_RST_new = or2(reg_new.sys_rst.WESY_SYS_RSTn_new(), RYJU_FF41_WRn_new);
    /*_p21.RUPO*/ reg_new.int_ctrl.RUPO_LYC_MATCHn.nor_latch(PAGO_LYC_MATCH_RST_new, reg_new.int_ctrl.ROPO_LY_MATCH_SYNCp.qp_new());
  }
}

//-----------------------------------------------------------------------------

void GateBoy::tock_lcd_gates(const GateBoyState& reg_old) {
  auto& reg_new = gb_state;

  {
    /*#p21.XYVO*/ wire XYVO_y144p_old = and2(reg_old.reg_ly.LOVU_LY4p_odd.qp_old(), reg_old.reg_ly.LAFO_LY7p_odd.qp_old()); // 128 + 16 = 144
    /*#p21.NOKO*/ wire NOKO_y153p_old = and4(
      reg_old.reg_ly.LAFO_LY7p_odd.qp_old(),
      reg_old.reg_ly.LOVU_LY4p_odd.qp_old(),
      reg_old.reg_ly.LYDO_LY3p_odd.qp_old(),
      reg_old.reg_ly.MUWY_LY0p_odd.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153

    /*#p28.ANEL*/ reg_new.lcd.ANEL_LINE_ENDp_odd.dff17(reg_new.sys_clk.AWOH_xxCDxxGH_new(), reg_new.ABEZ_VID_RSTn_new(), reg_old.lcd.CATU_LINE_ENDp_odd.qp_old());

    /*#p21.PURE*/ wire PURE_LINE_ENDn_old  = not1(reg_old.lcd.RUTU_LINE_ENDp_odd.qp_old());
    /*#p29.ALES*/ wire ALES_FRAME_ENDn_old = not1(XYVO_y144p_old);
    /*#p21.SELA*/ wire SELA_LINE_ENDp_old  = not1(PURE_LINE_ENDn_old);
    /*#p29.ABOV*/ wire ABOV_LINE_ENDp_old  = and2(SELA_LINE_ENDp_old, ALES_FRAME_ENDn_old);
    /*#p29.CATU*/ reg_new.lcd.CATU_LINE_ENDp_odd.dff17(reg_new.sys_clk.XUPY_ABxxEFxx_new(), reg_new.ABEZ_VID_RSTn_new(), ABOV_LINE_ENDp_old);

    // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*#p21.SANU*/ wire SANU_x113p_odd_old = and4(
      reg_old.reg_lx.TYRY_LX6p_odd.qp_old(),
      reg_old.reg_lx.TAHA_LX5p_odd.qp_old(),
      reg_old.reg_lx.SUDE_LX4p_odd.qp_old(),
      reg_old.reg_lx.SAXO_LX0p_odd.qp_old());

    /*_p28.ABAF*/ wire ABAF_LINE_ENDn_odd = not1(reg_new.lcd.CATU_LINE_ENDp_odd.qp_new());
    /*_p28.BYHA*/ wire BYHA_LINE_RST_TRIGn_odd = or_and3(reg_new.lcd.ANEL_LINE_ENDp_odd.qp_new(), ABAF_LINE_ENDn_odd, reg_new.ABEZ_VID_RSTn_new()); // so if this is or_and, BYHA should go low on 910 and 911
    /*_p28.ATEJ*/ reg_new.ATEJ_LINE_RST_TRIGp_odd <<= not1(BYHA_LINE_RST_TRIGn_odd);

    /*#p21.RUTU*/ reg_new.lcd.RUTU_LINE_ENDp_odd .dff17(reg_new.sys_clk.SONO_ABxxxxGH_new(),     reg_new.LYFE_VID_RSTn_new(), SANU_x113p_odd_old);
    /*#p21.NYPE*/ reg_new.lcd.NYPE_LINE_ENDp_odd .dff17(reg_new.sys_clk.TALU_xxCDEFxx_new(),     reg_new.LYFE_VID_RSTn_new(), reg_old.lcd.RUTU_LINE_ENDp_odd.qp_old());
    /*#p21.POPU*/ reg_new.lcd.POPU_VBLANKp_odd   .dff17(reg_new.lcd.NYPE_LINE_ENDp_odd.qp_new(), reg_new.LYFE_VID_RSTn_new(), XYVO_y144p_old);
    /*#p21.MYTA*/ reg_new.lcd.MYTA_FRAME_ENDp_odd.dff17(reg_new.lcd.NYPE_LINE_ENDp_odd.qp_new(), reg_new.LYFE_VID_RSTn_new(), NOKO_y153p_old);
  }

  {
    /*#p21.TOCU*/ wire TOCU_LX0n_old = not1(reg_old.reg_lx.SAXO_LX0p_odd.qp_old());
    /*#p21.VEPE*/ wire VEPE_LX1n_old = not1(reg_old.reg_lx.TYPO_LX1p_odd.qp_old());
    /*_p21.VUTY*/ wire VUTY_LX2n_old = not1(reg_old.reg_lx.VYZO_LX2p_odd.qp_old());
    /*_p21.VATE*/ wire VATE_LX3n_old = not1(reg_old.reg_lx.TELU_LX3p_odd.qp_old());
    /*_p21.TUDA*/ wire TUDA_LX4n_old = not1(reg_old.reg_lx.SUDE_LX4p_odd.qp_old());
    /*_p21.TAFY*/ wire TAFY_LX5n_old = not1(reg_old.reg_lx.TAHA_LX5p_odd.qp_old());
    /*_p21.TUJU*/ wire TUJU_LX6n_old = not1(reg_old.reg_lx.TYRY_LX6p_odd.qp_old());

    /*_p21.VOKU*/ wire VOKU_LX000n_old = nand7(TUJU_LX6n_old,                         TAFY_LX5n_old,                         TUDA_LX4n_old,                         VATE_LX3n_old,                         VUTY_LX2n_old,                         VEPE_LX1n_old,                         TOCU_LX0n_old);                         // 0000000 == 0
    /*_p21.TOZU*/ wire TOZU_LX007n_old = nand7(TUJU_LX6n_old,                         TAFY_LX5n_old,                         TUDA_LX4n_old,                         VATE_LX3n_old,                         reg_old.reg_lx.VYZO_LX2p_odd.qp_old(), reg_old.reg_lx.TYPO_LX1p_odd.qp_old(), reg_old.reg_lx.SAXO_LX0p_odd.qp_old()); // 0000111 == 7
    /*_p21.TECE*/ wire TECE_LX045n_old = nand7(TUJU_LX6n_old,                         reg_old.reg_lx.TAHA_LX5p_odd.qp_old(), TUDA_LX4n_old,                         reg_old.reg_lx.TELU_LX3p_odd.qp_old(), reg_old.reg_lx.VYZO_LX2p_odd.qp_old(), VEPE_LX1n_old,                         reg_old.reg_lx.SAXO_LX0p_odd.qp_old()); // 0101101 == 45
    /*#p21.TEBO*/ wire TEBO_LX083n_old = nand7(reg_old.reg_lx.TYRY_LX6p_odd.qp_old(), TAFY_LX5n_old,                         reg_old.reg_lx.SUDE_LX4p_odd.qp_old(), VATE_LX3n_old,                         VUTY_LX2n_old,                         reg_old.reg_lx.TYPO_LX1p_odd.qp_old(), reg_old.reg_lx.SAXO_LX0p_odd.qp_old()); // 1010011 == 83

    /*#p21.TEGY*/ wire TEGY_STROBE_old = nand4(VOKU_LX000n_old, TOZU_LX007n_old, TECE_LX045n_old, TEBO_LX083n_old);

    /*#p21.SYGU*/ reg_new.lcd.SYGU_LINE_STROBE.dff17(reg_new.sys_clk.SONO_ABxxxxGH_new(), reg_new.LYFE_VID_RSTn_new(), TEGY_STROBE_old);
  }

  {
    /*#p21.MUDE*/ wire MUDE_X_RSTn_new = nor2(reg_new.lcd.RUTU_LINE_ENDp_odd.qp_new(), reg_new.LYHA_VID_RSTp_new());
    /*#p21.SAXO*/ reg_new.reg_lx.SAXO_LX0p_odd.dff17(reg_new.sys_clk.TALU_xxCDEFxx_new(),   MUDE_X_RSTn_new, reg_old.reg_lx.SAXO_LX0p_odd.qn_old());
    /*#p21.TYPO*/ reg_new.reg_lx.TYPO_LX1p_odd.dff17(reg_new.reg_lx.SAXO_LX0p_odd.qn_new(), MUDE_X_RSTn_new, reg_old.reg_lx.TYPO_LX1p_odd.qn_old());
    /*#p21.VYZO*/ reg_new.reg_lx.VYZO_LX2p_odd.dff17(reg_new.reg_lx.TYPO_LX1p_odd.qn_new(), MUDE_X_RSTn_new, reg_old.reg_lx.VYZO_LX2p_odd.qn_old());
    /*#p21.TELU*/ reg_new.reg_lx.TELU_LX3p_odd.dff17(reg_new.reg_lx.VYZO_LX2p_odd.qn_new(), MUDE_X_RSTn_new, reg_old.reg_lx.TELU_LX3p_odd.qn_old());
    /*#p21.SUDE*/ reg_new.reg_lx.SUDE_LX4p_odd.dff17(reg_new.reg_lx.TELU_LX3p_odd.qn_new(), MUDE_X_RSTn_new, reg_old.reg_lx.SUDE_LX4p_odd.qn_old());
    /*#p21.TAHA*/ reg_new.reg_lx.TAHA_LX5p_odd.dff17(reg_new.reg_lx.SUDE_LX4p_odd.qn_new(), MUDE_X_RSTn_new, reg_old.reg_lx.TAHA_LX5p_odd.qn_old());
    /*#p21.TYRY*/ reg_new.reg_lx.TYRY_LX6p_odd.dff17(reg_new.reg_lx.TAHA_LX5p_odd.qn_new(), MUDE_X_RSTn_new, reg_old.reg_lx.TYRY_LX6p_odd.qn_old());
  }

  {
    /*#p21.LAMA*/ wire LAMA_Y_RSTn_new = nor2(reg_new.lcd.MYTA_FRAME_ENDp_odd.qp_new(), reg_new.LYHA_VID_RSTp_new());
    /*#p21.MUWY*/ reg_new.reg_ly.MUWY_LY0p_odd.dff17(reg_new.lcd.RUTU_LINE_ENDp_odd.qp_new(), LAMA_Y_RSTn_new, reg_old.reg_ly.MUWY_LY0p_odd.qn_old());
    /*#p21.MYRO*/ reg_new.reg_ly.MYRO_LY1p_odd.dff17(reg_new.reg_ly.MUWY_LY0p_odd.qn_new(),   LAMA_Y_RSTn_new, reg_old.reg_ly.MYRO_LY1p_odd.qn_old());
    /*#p21.LEXA*/ reg_new.reg_ly.LEXA_LY2p_odd.dff17(reg_new.reg_ly.MYRO_LY1p_odd.qn_new(),   LAMA_Y_RSTn_new, reg_old.reg_ly.LEXA_LY2p_odd.qn_old());
    /*#p21.LYDO*/ reg_new.reg_ly.LYDO_LY3p_odd.dff17(reg_new.reg_ly.LEXA_LY2p_odd.qn_new(),   LAMA_Y_RSTn_new, reg_old.reg_ly.LYDO_LY3p_odd.qn_old());
    /*#p21.LOVU*/ reg_new.reg_ly.LOVU_LY4p_odd.dff17(reg_new.reg_ly.LYDO_LY3p_odd.qn_new(),   LAMA_Y_RSTn_new, reg_old.reg_ly.LOVU_LY4p_odd.qn_old());
    /*#p21.LEMA*/ reg_new.reg_ly.LEMA_LY5p_odd.dff17(reg_new.reg_ly.LOVU_LY4p_odd.qn_new(),   LAMA_Y_RSTn_new, reg_old.reg_ly.LEMA_LY5p_odd.qn_old());
    /*#p21.MATO*/ reg_new.reg_ly.MATO_LY6p_odd.dff17(reg_new.reg_ly.LEMA_LY5p_odd.qn_new(),   LAMA_Y_RSTn_new, reg_old.reg_ly.MATO_LY6p_odd.qn_old());
    /*#p21.LAFO*/ reg_new.reg_ly.LAFO_LY7p_odd.dff17(reg_new.reg_ly.MATO_LY6p_odd.qn_new(),   LAMA_Y_RSTn_new, reg_old.reg_ly.LAFO_LY7p_odd.qn_old());
  }

  {
    /*_p23.WAFU*/ wire WAFU_FF44_RDp_new = and2(reg_new.cpu_signals.ASOT_CPU_RDp_new(), reg_new.cpu_abus.XOGY_FF44p_new());
    /*_p23.VARO*/ wire VARO_FF44_RDn_new = not1(WAFU_FF44_RDp_new);

    /*#p23.WURY*/ wire WURY_LY0n_new = not1(reg_new.reg_ly.MUWY_LY0p_odd.qp_new());
    /*_p23.XEPO*/ wire XEPO_LY1n_new = not1(reg_new.reg_ly.MYRO_LY1p_odd.qp_new());
    /*_p23.MYFA*/ wire MYFA_LY2n_new = not1(reg_new.reg_ly.LEXA_LY2p_odd.qp_new());
    /*_p23.XUHY*/ wire XUHY_LY3n_new = not1(reg_new.reg_ly.LYDO_LY3p_odd.qp_new());
    /*_p23.WATA*/ wire WATA_LY4n_new = not1(reg_new.reg_ly.LOVU_LY4p_odd.qp_new());
    /*_p23.XAGA*/ wire XAGA_LY5n_new = not1(reg_new.reg_ly.LEMA_LY5p_odd.qp_new());
    /*_p23.XUCE*/ wire XUCE_LY6n_new = not1(reg_new.reg_ly.MATO_LY6p_odd.qp_new());
    /*_p23.XOWO*/ wire XOWO_LY7n_new = not1(reg_new.reg_ly.LAFO_LY7p_odd.qp_new());

    /*_p23.VEGA*/ triwire VEGA_LY0_TO_CD0_new = tri6_nn(VARO_FF44_RDn_new, WURY_LY0n_new);
    /*_p23.WUVA*/ triwire WUVA_LY1_TO_CD1_new = tri6_nn(VARO_FF44_RDn_new, XEPO_LY1n_new);
    /*_p23.LYCO*/ triwire LYCO_LY2_TO_CD2_new = tri6_nn(VARO_FF44_RDn_new, MYFA_LY2n_new);
    /*_p23.WOJY*/ triwire WOJY_LY3_TO_CD3_new = tri6_nn(VARO_FF44_RDn_new, XUHY_LY3n_new);
    /*_p23.VYNE*/ triwire VYNE_LY4_TO_CD4_new = tri6_nn(VARO_FF44_RDn_new, WATA_LY4n_new);
    /*_p23.WAMA*/ triwire WAMA_LY5_TO_CD5_new = tri6_nn(VARO_FF44_RDn_new, XAGA_LY5n_new);
    /*_p23.WAVO*/ triwire WAVO_LY6_TO_CD6_new = tri6_nn(VARO_FF44_RDn_new, XUCE_LY6n_new);
    /*_p23.WEZE*/ triwire WEZE_LY7_TO_CD7_new = tri6_nn(VARO_FF44_RDn_new, XOWO_LY7n_new);

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(VEGA_LY0_TO_CD0_new);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(WUVA_LY1_TO_CD1_new);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(LYCO_LY2_TO_CD2_new);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(WOJY_LY3_TO_CD3_new);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(VYNE_LY4_TO_CD4_new);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(WAMA_LY5_TO_CD5_new);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(WAVO_LY6_TO_CD6_new);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(WEZE_LY7_TO_CD7_new);
  }
}

//-----------------------------------------------------------------------------

void GateBoy::set_lcd_pins_gates(const GateBoyState& reg_old, wire SACU_CLKPIPE_evn) {
  auto& reg_new = gb_state;

  /*#p21.RYNO*/ wire RYNO = or2(reg_new.lcd.SYGU_LINE_STROBE.qp_new(), reg_new.lcd.RUTU_LINE_ENDp_odd.qp_new());
  /*#p21.POGU*/ wire POGU = not1(RYNO);
  /*_PIN_52*/ pins.lcd.PIN_52_LCD_CNTRL.pin_out(POGU, POGU);

  // if LCDC_ENn, PIN_56_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)

  /*#p24.LOFU*/ wire LOFU_x113n = not1(reg_new.lcd.RUTU_LINE_ENDp_odd.qp_new());
  /*#p24.LUCA*/ reg_new.lcd.LUCA_LINE_EVENp .dff17(LOFU_x113n,                            reg_new.LYFE_VID_RSTn_new(), reg_old.lcd.LUCA_LINE_EVENp.qn_old());
  /*#p21.NAPO*/ reg_new.lcd.NAPO_FRAME_EVENp.dff17(reg_new.lcd.POPU_VBLANKp_odd.qp_new(), reg_new.LYFE_VID_RSTn_new(), reg_old.lcd.NAPO_FRAME_EVENp.qn_old());

  /*#p24.MAGU*/ wire MAGU = xor2(reg_new.lcd.NAPO_FRAME_EVENp.qp_new(), reg_new.lcd.LUCA_LINE_EVENp.qn_new());
  /*#p24.MECO*/ wire MECO = not1(MAGU);
  /*#p24.KEBO*/ wire KEBO = not1(MECO);
  /*_p01.UREK*/ wire UREK_DIV07n = not1(reg_new.reg_div.TULU_DIV07p.qp_new());
  /*#p24.USEC*/ wire USEC_DIV07p = not1(UREK_DIV07n);
  /*#p24.KEDY*/ wire KEDY_LCDC_ENn = not1(reg_new.reg_lcdc.XONA_LCDC_LCDENn.qn_new());
  /*#p24.KUPA*/ wire KUPA = amux2(reg_new.reg_lcdc.XONA_LCDC_LCDENn.qn_new(), KEBO, KEDY_LCDC_ENn, USEC_DIV07p);
  /*#p24.KOFO*/ wire KOFO = not1(KUPA);
  /*_PIN_56*/ pins.lcd.PIN_56_LCD_FLIPS.pin_out(KOFO, KOFO);

  /*#p24.NERU*/ wire NERU_VSYNCp_old = nor8(
    reg_old.reg_ly.LAFO_LY7p_odd.qp_old(),
    reg_old.reg_ly.LOVU_LY4p_odd.qp_old(),
    reg_old.reg_ly.LYDO_LY3p_odd.qp_old(),
    reg_old.reg_ly.MUWY_LY0p_odd.qp_old(),
    reg_old.reg_ly.MYRO_LY1p_odd.qp_old(),
    reg_old.reg_ly.LEXA_LY2p_odd.qp_old(),
    reg_old.reg_ly.LEMA_LY5p_odd.qp_old(),
    reg_old.reg_ly.MATO_LY6p_odd.qp_old());

  /*#p24.MEDA*/ reg_new.lcd.MEDA_VSYNC_OUTn.dff17(reg_new.lcd.NYPE_LINE_ENDp_odd.qn_new(), reg_new.LYFE_VID_RSTn_new(), NERU_VSYNCp_old);
  /*#p24.MURE*/ wire MURE_VSYNC = not1(reg_new.lcd.MEDA_VSYNC_OUTn.qp_new());
  /*_PIN_57*/ pins.lcd.PIN_57_LCD_VSYNC.pin_out(MURE_VSYNC, MURE_VSYNC);

  // FIXME inversion
  // I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
  // seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
  // Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
  // That would be a loooot of gate delay.
  // Could we possibly be incrementing X3p one phase early?

  // LCD horizontal sync pin latch
  /*#p24.POME*/ reg_new.lcd.POME_X8_LATCH <<= nor2(reg_new.sprite_scanner.AVAP_SCAN_DONE_tp_odd.out_new(), reg_new.lcd.POFY.out_mid());
  /*#p24.RUJU*/ reg_new.lcd.RUJU <<= or3(reg_new.lcd.PAHO_X8_SYNC.qp_new(), reg_new.TOFU_VID_RSTp_new(), reg_new.lcd.POME_X8_LATCH.out_mid());
  /*#p24.POFY*/ reg_new.lcd.POFY <<= not1(reg_new.lcd.RUJU.out_mid());
  /*#p24.POME*/ reg_new.lcd.POME_X8_LATCH <<= nor2(reg_new.sprite_scanner.AVAP_SCAN_DONE_tp_odd.out_new(), reg_new.lcd.POFY.out_mid());
  /*#p24.RUJU*/ reg_new.lcd.RUJU <<= or3(reg_new.lcd.PAHO_X8_SYNC.qp_new(), reg_new.TOFU_VID_RSTp_new(), reg_new.lcd.POME_X8_LATCH.out_mid());
  /*#p24.POFY*/ reg_new.lcd.POFY <<= not1(reg_new.lcd.RUJU.out_mid());

  /*#p24.RUZE*/ wire RUZE_HSYNCn_new = not1(reg_new.lcd.POFY.out_new());
  /*_PIN_54*/ pins.lcd.PIN_54_LCD_HSYNC.pin_out(RUZE_HSYNCn_new, RUZE_HSYNCn_new);

  /*_PIN_51*/ pins.lcd.PIN_51_LCD_DATA0.pin_out(reg_new.lcd.REMY_LD0n.out_new(), reg_new.lcd.REMY_LD0n.out_new());
  /*_PIN_50*/ pins.lcd.PIN_50_LCD_DATA1.pin_out(reg_new.lcd.RAVO_LD1n.out_new(), reg_new.lcd.RAVO_LD1n.out_new());

  /*_p01.UMEK*/ wire UMEK_DIV06n = not1(reg_new.reg_div.UGOT_DIV06p.qp_new());
  /*#p21.PURE*/ wire PURE_x113n = not1(reg_new.lcd.RUTU_LINE_ENDp_odd.qp_new());
  /*#p24.KASA*/ wire KASA_LINE_ENDp = not1(PURE_x113n);
  /*#p24.UMOB*/ wire UMOB_DIV_06p = not1(UMEK_DIV06n);
  /*#p24.KAHE*/ wire KAHE_LINE_ENDp = amux2(reg_new.reg_lcdc.XONA_LCDC_LCDENn.qn_new(), KASA_LINE_ENDp, KEDY_LCDC_ENn, UMOB_DIV_06p);
  /*#p24.KYMO*/ wire KYMO_LINE_ENDn = not1(KAHE_LINE_ENDp);
  /*_PIN_55*/ pins.lcd.PIN_55_LCD_LATCH.pin_out(KYMO_LINE_ENDn, KYMO_LINE_ENDn);

  /*#p21.XAJO*/ wire XAJO_X_009p_odd = and2(reg_new.pix_count.XEHO_PX0p_odd.qp_new(), reg_new.pix_count.XYDO_PX3p_odd.qp_new());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(reg_new.TOFU_VID_RSTp_new(), reg_new.VOGA_HBLANKp_evn.qp_new());
  /*#p21.WUSA*/ reg_new.lcd.WUSA_LCD_CLOCK_GATE.nor_latch(XAJO_X_009p_odd, WEGO_HBLANKp);
  /*#p21.TOBA*/ wire TOBA_LCD_CLOCK = and2(reg_new.lcd.WUSA_LCD_CLOCK_GATE.qp_new(), SACU_CLKPIPE_evn);
  /*#p27.POVA*/ wire POVA_FINE_MATCH_TRIGp = and2(reg_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.qp_new(), reg_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.qn_new());
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

//-----------------------------------------------------------------------------

void LCDControl::reset_to_poweron() {
  CATU_LINE_ENDp_odd.state  = BIT_OLD | BIT_DRIVEN;
  ANEL_LINE_ENDp_odd.state  = BIT_OLD | BIT_DRIVEN;
  POPU_VBLANKp_odd.state    = BIT_OLD | BIT_DRIVEN;
  MYTA_FRAME_ENDp_odd.state = BIT_OLD | BIT_DRIVEN;
  RUTU_LINE_ENDp_odd.state  = BIT_OLD | BIT_DRIVEN;
  NYPE_LINE_ENDp_odd.state  = BIT_OLD | BIT_DRIVEN;
  SYGU_LINE_STROBE.state    = BIT_OLD | BIT_DRIVEN;
  MEDA_VSYNC_OUTn.state     = BIT_OLD | BIT_DRIVEN;
  LUCA_LINE_EVENp.state     = BIT_OLD | BIT_DRIVEN;
  NAPO_FRAME_EVENp.state    = BIT_OLD | BIT_DRIVEN;
  RUJU.state                = BIT_OLD | BIT_DRIVEN;
  POFY.state                = BIT_OLD | BIT_DRIVEN;
  POME_X8_LATCH.state       = BIT_OLD | BIT_DRIVEN;
  PAHO_X8_SYNC.state        = BIT_OLD | BIT_DRIVEN;
  WUSA_LCD_CLOCK_GATE.state = BIT_OLD | BIT_DRIVEN;
  REMY_LD0n.state           = BIT_OLD | BIT_DRIVEN;
  RAVO_LD1n.state           = BIT_OLD | BIT_DRIVEN;
}

void LCDControl::reset_to_bootrom() {
  CATU_LINE_ENDp_odd.state  = BIT_OLD | BIT_DRIVEN;
  ANEL_LINE_ENDp_odd.state  = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  POPU_VBLANKp_odd.state    = BIT_OLD | BIT_DRIVEN;
  MYTA_FRAME_ENDp_odd.state = BIT_OLD | BIT_DRIVEN;
  RUTU_LINE_ENDp_odd.state  = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  NYPE_LINE_ENDp_odd.state  = BIT_OLD | BIT_DRIVEN;
  SYGU_LINE_STROBE.state    = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  MEDA_VSYNC_OUTn.state     = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  LUCA_LINE_EVENp.state     = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  NAPO_FRAME_EVENp.state    = BIT_OLD | BIT_DRIVEN;
  RUJU.state                = BIT_OLD | BIT_DRIVEN | 1;
  POFY.state                = BIT_OLD | BIT_DRIVEN | 0;
  POME_X8_LATCH.state       = BIT_OLD | BIT_DRIVEN | 1;
  PAHO_X8_SYNC.state        = BIT_OLD | BIT_DRIVEN;
  WUSA_LCD_CLOCK_GATE.state = BIT_OLD | BIT_DRIVEN;
  REMY_LD0n.state           = BIT_OLD | BIT_DRIVEN | 1;
  RAVO_LD1n.state           = BIT_OLD | BIT_DRIVEN | 1;
}

void LCDControl::reset_to_cart() {
  CATU_LINE_ENDp_odd.state  = 0b00011000;
  ANEL_LINE_ENDp_odd.state  = 0b00011010;
  POPU_VBLANKp_odd.state    = 0b00011001;
  MYTA_FRAME_ENDp_odd.state = 0b00011001;
  RUTU_LINE_ENDp_odd.state  = 0b00011010;
  NYPE_LINE_ENDp_odd.state  = 0b00011000;
  SYGU_LINE_STROBE.state    = 0b00011010;
  MEDA_VSYNC_OUTn.state     = 0b00011011;
  LUCA_LINE_EVENp.state     = 0b00011011;
  NAPO_FRAME_EVENp.state    = 0b00011010;
  RUJU.state                = 0b00011001;
  POFY.state                = 0b00011000;
  POME_X8_LATCH.state       = 0b00011001;
  PAHO_X8_SYNC.state        = 0b00011000;
  WUSA_LCD_CLOCK_GATE.state = 0b00011000;
  REMY_LD0n.state           = 0b00011000;
  RAVO_LD1n.state           = 0b00011000;
}

//-----------------------------------------------------------------------------
