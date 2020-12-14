#include "GateBoyLib/GateBoyLCD.h"

//------------------------------------------------------------------------------------------------------------------------

void RegLX::tock(wire XODO_VID_RSTp, wire TALU_xxCDEFxx_clkevn) {
  /*#p21.SONO*/ wire _SONO_ABxxxxGH_clknew = not1(TALU_xxCDEFxx_clkevn);

  /*#p21.SANU*/ wire _SANU_x113p_old_evn = and4(TYRY_LX6p_evn.qp_old(), TAHA_LX5p_evn.qp_old(), SUDE_LX4p_evn.qp_old(), SAXO_LX0p_evn.qp_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*#p21.NYPE*/ NYPE_x113p_c.dff17( TALU_xxCDEFxx_clkevn, LYFE_VID_RSTn(XODO_VID_RSTp), RUTU_x113p_g.qp_old());
  /*#p21.RUTU*/ RUTU_x113p_g.dff17(_SONO_ABxxxxGH_clknew, LYFE_VID_RSTn(XODO_VID_RSTp), _SANU_x113p_old_evn);

  /*#p21.MUDE*/ wire _MUDE_X_RSTn_new_evn = nor2(RUTU_x113p_g.qp_new(), LYHA_VID_RSTp(XODO_VID_RSTp));
  /*#p21.SAXO*/ SAXO_LX0p_evn.dff17(TALU_xxCDEFxx_clkevn,   _MUDE_X_RSTn_new_evn, SAXO_LX0p_evn.qn_any());
  /*#p21.TYPO*/ TYPO_LX1p_evn.dff17(SAXO_LX0p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TYPO_LX1p_evn.qn_any());
  /*#p21.VYZO*/ VYZO_LX2p_evn.dff17(TYPO_LX1p_evn.qn_new(), _MUDE_X_RSTn_new_evn, VYZO_LX2p_evn.qn_any());
  /*#p21.TELU*/ TELU_LX3p_evn.dff17(VYZO_LX2p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TELU_LX3p_evn.qn_any());
  /*#p21.SUDE*/ SUDE_LX4p_evn.dff17(TELU_LX3p_evn.qn_new(), _MUDE_X_RSTn_new_evn, SUDE_LX4p_evn.qn_any());
  /*#p21.TAHA*/ TAHA_LX5p_evn.dff17(SUDE_LX4p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TAHA_LX5p_evn.qn_any());
  /*#p21.TYRY*/ TYRY_LX6p_evn.dff17(TAHA_LX5p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TYRY_LX6p_evn.qn_any());
}

//------------------------------------------------------------------------------------------------------------------------

void RegLY::tock(wire BUS_CPU_A[16],
                 wire XODO_VID_RSTp,
                 wire TEDO_CPU_RDp,
                 wire NYPE_x113p_c_new,
                 wire RUTU_x113p_g_new,
                 BusOut BUS_CPU_D_out[8])
{
  /*#p21.NOKO*/ wire _NOKO_y153p_old_evn = and4(LAFO_LY7p_evn.qp_old(), LOVU_LY4p_evn.qp_old(), LYDO_LY3p_evn.qp_old(), MUWY_LY0p_evn.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
  /*#p21.MYTA*/ MYTA_y153p_evn.dff17(NYPE_x113p_c_new, LYFE_VID_RSTn(XODO_VID_RSTp), _NOKO_y153p_old_evn);

  /*#p21.LAMA*/ wire _LAMA_Y_RSTn_new_evn = nor2(MYTA_y153p_evn.qp_new(), LYHA_VID_RSTp(XODO_VID_RSTp));
  /*#p21.MUWY*/ MUWY_LY0p_evn.dff17(RUTU_x113p_g_new,       _LAMA_Y_RSTn_new_evn, MUWY_LY0p_evn.qn_any());
  /*#p21.MYRO*/ MYRO_LY1p_evn.dff17(MUWY_LY0p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, MYRO_LY1p_evn.qn_any());
  /*#p21.LEXA*/ LEXA_LY2p_evn.dff17(MYRO_LY1p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LEXA_LY2p_evn.qn_any());
  /*#p21.LYDO*/ LYDO_LY3p_evn.dff17(LEXA_LY2p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LYDO_LY3p_evn.qn_any());
  /*#p21.LOVU*/ LOVU_LY4p_evn.dff17(LYDO_LY3p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LOVU_LY4p_evn.qn_any());
  /*#p21.LEMA*/ LEMA_LY5p_evn.dff17(LOVU_LY4p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LEMA_LY5p_evn.qn_any());
  /*#p21.MATO*/ MATO_LY6p_evn.dff17(LEMA_LY5p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, MATO_LY6p_evn.qn_any());
  /*#p21.LAFO*/ LAFO_LY7p_evn.dff17(MATO_LY6p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LAFO_LY7p_evn.qn_any());

  /* p23.WAFU*/ wire _WAFU_FF44_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), XOGY_FF44p_ext(BUS_CPU_A));
  /* p23.VARO*/ wire _VARO_FF44_RDn_ext = not1(_WAFU_FF44_RDp_ext);

  /*#p23.WURY*/ wire _WURY_LY0n_new = not1(MUWY_LY0p_evn.qp_new());
  /* p23.XEPO*/ wire _XEPO_LY1n_new = not1(MYRO_LY1p_evn.qp_new());
  /* p23.MYFA*/ wire _MYFA_LY2n_new = not1(LEXA_LY2p_evn.qp_new());
  /* p23.XUHY*/ wire _XUHY_LY3n_new = not1(LYDO_LY3p_evn.qp_new());
  /* p23.WATA*/ wire _WATA_LY4n_new = not1(LOVU_LY4p_evn.qp_new());
  /* p23.XAGA*/ wire _XAGA_LY5n_new = not1(LEMA_LY5p_evn.qp_new());
  /* p23.XUCE*/ wire _XUCE_LY6n_new = not1(MATO_LY6p_evn.qp_new());
  /* p23.XOWO*/ wire _XOWO_LY7n_new = not1(LAFO_LY7p_evn.qp_new());

  /* p23.VEGA*/ BUS_CPU_D_out[0].tri6_nn(_VARO_FF44_RDn_ext, _WURY_LY0n_new);
  /* p23.WUVA*/ BUS_CPU_D_out[1].tri6_nn(_VARO_FF44_RDn_ext, _XEPO_LY1n_new);
  /* p23.LYCO*/ BUS_CPU_D_out[2].tri6_nn(_VARO_FF44_RDn_ext, _MYFA_LY2n_new);
  /* p23.WOJY*/ BUS_CPU_D_out[3].tri6_nn(_VARO_FF44_RDn_ext, _XUHY_LY3n_new);
  /* p23.VYNE*/ BUS_CPU_D_out[4].tri6_nn(_VARO_FF44_RDn_ext, _WATA_LY4n_new);
  /* p23.WAMA*/ BUS_CPU_D_out[5].tri6_nn(_VARO_FF44_RDn_ext, _XAGA_LY5n_new);
  /* p23.WAVO*/ BUS_CPU_D_out[6].tri6_nn(_VARO_FF44_RDn_ext, _XUCE_LY6n_new);
  /* p23.WEZE*/ BUS_CPU_D_out[7].tri6_nn(_VARO_FF44_RDn_ext, _XOWO_LY7n_new);
}

//------------------------------------------------------------------------------------------------------------------------

void RegLCDC::tock(wire BUS_CPU_A[16],
                   wire BUS_CPU_D[8],
                   wire AVOR_SYS_RSTp,
                   wire TEDO_CPU_RDp,
                   wire TAPU_CPU_WRp,
                   BusOut BUS_CPU_D_out[8])
{
  // FF40 LCDC
  /* p23.WARU*/ wire _WARU_FF40_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), VOCA_FF40p_ext(BUS_CPU_A));
  /* p23.XUBO*/ wire _XUBO_FF40_WRn_clk = not1(_WARU_FF40_WRp_clk);
  /*#p23.VYXE*/ VYXE_LCDC_BGENn_h  .dff9(_XUBO_FF40_WRn_clk, XARE_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[0]);
  /* p23.XYLO*/ XYLO_LCDC_SPENn_h  .dff9(_XUBO_FF40_WRn_clk, XARE_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[1]);
  /* p23.XYMO*/ XYMO_LCDC_SPSIZEn_h.dff9(_XUBO_FF40_WRn_clk, XARE_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[2]);
  /* p23.XAFO*/ XAFO_LCDC_BGMAPn_h .dff9(_XUBO_FF40_WRn_clk, XARE_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[3]);
  /* p23.WEXU*/ WEXU_LCDC_BGTILEn_h.dff9(_XUBO_FF40_WRn_clk, XARE_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[4]);
  /* p23.WYMO*/ WYMO_LCDC_WINENn_h .dff9(_XUBO_FF40_WRn_clk, XARE_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[5]);
  /* p23.WOKY*/ WOKY_LCDC_WINMAPn_h.dff9(_XUBO_FF40_WRn_clk, XARE_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[6]);

  // FF40 LCDC
  /* p23.VYRE*/ wire _VYRE_FF40_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), VOCA_FF40p_ext(BUS_CPU_A));
  /* p23.WYCE*/ wire _WYCE_FF40_RDn_ext = not1(_VYRE_FF40_RDp_ext);
  /*#p23.WYPO*/ BUS_CPU_D_out[0].tri6_nn(_WYCE_FF40_RDn_ext, VYXE_LCDC_BGENn_h.qp_new());
  /*#p23.XERO*/ BUS_CPU_D_out[1].tri6_nn(_WYCE_FF40_RDn_ext, XYLO_LCDC_SPENn_h.qp_new());
  /* p23.WYJU*/ BUS_CPU_D_out[2].tri6_nn(_WYCE_FF40_RDn_ext, XYMO_LCDC_SPSIZEn_h.qp_new());
  /* p23.WUKA*/ BUS_CPU_D_out[3].tri6_nn(_WYCE_FF40_RDn_ext, XAFO_LCDC_BGMAPn_h.qp_new());
  /* p23.VOKE*/ BUS_CPU_D_out[4].tri6_nn(_WYCE_FF40_RDn_ext, WEXU_LCDC_BGTILEn_h.qp_new());
  /* p23.VATO*/ BUS_CPU_D_out[5].tri6_nn(_WYCE_FF40_RDn_ext, WYMO_LCDC_WINENn_h.qp_new());
  /*#p23.VAHA*/ BUS_CPU_D_out[6].tri6_nn(_WYCE_FF40_RDn_ext, WOKY_LCDC_WINMAPn_h.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void RegLYC::tock(
  wire BUS_CPU_A[16],
  wire BUS_CPU_D[8],
  wire AVOR_SYS_RSTp,
  wire TALU_xxCDEFxx_clkevn,

  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,

  const RegLY& reg_ly,

  BusOut BUS_CPU_D_out[8])
{
  // LYC matcher
  /* p21.RYME*/ wire _RYME_LY_MATCH0n_old_evn = xor2(reg_ly.MUWY_LY0p_evn.qp_old(), SYRY_LYC0n.qn_old());
  /* p21.TYDE*/ wire _TYDE_LY_MATCH1n_old_evn = xor2(reg_ly.MYRO_LY1p_evn.qp_old(), VUCE_LYC1n.qn_old());
  /* p21.REDA*/ wire _REDA_LY_MATCH2n_old_evn = xor2(reg_ly.LEXA_LY2p_evn.qp_old(), SEDY_LYC2n.qn_old());
  /* p21.RASY*/ wire _RASY_LY_MATCH3n_old_evn = xor2(reg_ly.LYDO_LY3p_evn.qp_old(), SALO_LYC3n.qn_old());
  /* p21.TYKU*/ wire _TYKU_LY_MATCH4n_old_evn = xor2(reg_ly.LOVU_LY4p_evn.qp_old(), SOTA_LYC4n.qn_old());
  /* p21.TUCY*/ wire _TUCY_LY_MATCH5n_old_evn = xor2(reg_ly.LEMA_LY5p_evn.qp_old(), VAFA_LYC5n.qn_old());
  /* p21.TERY*/ wire _TERY_LY_MATCH6n_old_evn = xor2(reg_ly.MATO_LY6p_evn.qp_old(), VEVO_LYC6n.qn_old());
  /* p21.SYFU*/ wire _SYFU_LY_MATCH7n_old_evn = xor2(reg_ly.LAFO_LY7p_evn.qp_old(), RAHA_LYC7n.qn_old());

  /*#p21.SOVU*/ wire _SOVU_LY_MATCHA_old_evn = nor4 (_SYFU_LY_MATCH7n_old_evn, _TERY_LY_MATCH6n_old_evn, _TUCY_LY_MATCH5n_old_evn, _TYKU_LY_MATCH4n_old_evn);
  /*#p21.SUBO*/ wire _SUBO_LY_MATCHB_old_evn = nor4 (_RASY_LY_MATCH3n_old_evn, _REDA_LY_MATCH2n_old_evn, _TYDE_LY_MATCH1n_old_evn, _RYME_LY_MATCH0n_old_evn);
  /*#p21.RAPE*/ wire _RAPE_LY_MATCHn_old_evn = nand2(_SOVU_LY_MATCHA_old_evn,  _SUBO_LY_MATCHB_old_evn);
  /*#p21.PALY*/ wire _PALY_LY_MATCHa_old_evn = not1 (_RAPE_LY_MATCHn_old_evn);

  /*#p21.ROPO*/ ROPO_LY_MATCH_SYNCp_c.dff17(TALU_xxCDEFxx_clkevn, WESY_SYS_RSTn(AVOR_SYS_RSTp), _PALY_LY_MATCHa_old_evn);

  /* p23.XUFA*/ wire _XUFA_FF45_WRn_clk_evn = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), XAYU_FF45p_ext(BUS_CPU_A));
  /* p23.WANE*/ wire _WANE_FF45_WRp_clk_evn = not1(_XUFA_FF45_WRn_clk_evn);
  /* p23.SYRY*/ SYRY_LYC0n.dff9(_WANE_FF45_WRp_clk_evn, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[0]);
  /* p23.VUCE*/ VUCE_LYC1n.dff9(_WANE_FF45_WRp_clk_evn, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[1]);
  /* p23.SEDY*/ SEDY_LYC2n.dff9(_WANE_FF45_WRp_clk_evn, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[2]);
  /* p23.SALO*/ SALO_LYC3n.dff9(_WANE_FF45_WRp_clk_evn, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[3]);
  /* p23.SOTA*/ SOTA_LYC4n.dff9(_WANE_FF45_WRp_clk_evn, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[4]);
  /* p23.VAFA*/ VAFA_LYC5n.dff9(_WANE_FF45_WRp_clk_evn, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[5]);
  /* p23.VEVO*/ VEVO_LYC6n.dff9(_WANE_FF45_WRp_clk_evn, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[6]);
  /* p23.RAHA*/ RAHA_LYC7n.dff9(_WANE_FF45_WRp_clk_evn, WESY_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[7]);

  /* p23.XYLY*/ wire _XYLY_FF45_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), XAYU_FF45p_ext(BUS_CPU_A));
  /* p23.WEKU*/ wire _WEKU_FF45_RDn_ext = not1(_XYLY_FF45_RDp_ext);
  /*#p23.RETU*/ BUS_CPU_D_out[0].tri6_nn(_WEKU_FF45_RDn_ext, SYRY_LYC0n.qp_new());
  /* p23.VOJO*/ BUS_CPU_D_out[1].tri6_nn(_WEKU_FF45_RDn_ext, VUCE_LYC1n.qp_new());
  /* p23.RAZU*/ BUS_CPU_D_out[2].tri6_nn(_WEKU_FF45_RDn_ext, SEDY_LYC2n.qp_new());
  /* p23.REDY*/ BUS_CPU_D_out[3].tri6_nn(_WEKU_FF45_RDn_ext, SALO_LYC3n.qp_new());
  /* p23.RACE*/ BUS_CPU_D_out[4].tri6_nn(_WEKU_FF45_RDn_ext, SOTA_LYC4n.qp_new());
  /*#p23.VAZU*/ BUS_CPU_D_out[5].tri6_nn(_WEKU_FF45_RDn_ext, VAFA_LYC5n.qp_new());
  /* p23.VAFE*/ BUS_CPU_D_out[6].tri6_nn(_WEKU_FF45_RDn_ext, VEVO_LYC6n.qp_new());
  /* p23.PUFY*/ BUS_CPU_D_out[7].tri6_nn(_WEKU_FF45_RDn_ext, RAHA_LYC7n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

wire GateBoyLCD::ATEJ_LINE_RSTp(wire XODO_VID_RSTp) const {
  /* p28.ABAF*/ wire _ABAF_LINE_P000n = not1(CATU_LINE_P000p_a.qp_any());
  /* p28.BYHA*/ wire _BYHA_LINE_TRIGn = or_and3(ANEL_LINE_P002p_c.qp_any(), _ABAF_LINE_P000n, ABEZ_VID_RSTn(XODO_VID_RSTp)); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire _ATEJ_LINE_RSTp = not1(_BYHA_LINE_TRIGn);
  return _ATEJ_LINE_RSTp;
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::tock(
  wire XODO_VID_RSTp_new_h,
  wire XUPY_ABxxEFxx_clk_evn,
  const RegLX& reg_lx,
  const RegLY& reg_ly)
{
  /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp_new_h);
  /* p01.LYHA*/ wire _LYHA_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
  /* p01.LYFE*/ wire _LYFE_VID_RSTn_new_evn = not1(_LYHA_VID_RSTp_new_evn);
  /*#p01.ATAR*/ wire _ATAR_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
  /*#p01.ABEZ*/ wire _ABEZ_VID_RSTn_new_evn = not1(_ATAR_VID_RSTp_new_evn);

  /*#p28.AWOH*/ wire _AWOH_xxCDxxGH_clknew = not1(XUPY_ABxxEFxx_clk_evn);

  /*#p21.XYVO*/ wire _XYVO_y144p_old_evn = and2(reg_ly.LOVU_LY4p_evn.qp_old(), reg_ly.LAFO_LY7p_evn.qp_old()); // 128 + 16 = 144

  {
    /*#p21.POPU*/ POPU_VBLANKp_evn.dff17(reg_lx.NYPE_x113p_c.qp_new(), _LYFE_VID_RSTn_new_evn, _XYVO_y144p_old_evn);
  }

  {
    /*#p21.PURE*/ wire _PURE_LINE_ENDn_old_evn = not1(reg_lx.RUTU_x113p_g.qp_old());
    /*#p21.SELA*/ wire _SELA_LINE_P908p_old_evn = not1(_PURE_LINE_ENDn_old_evn);
    /*#p29.ALES*/ wire _ALES_y144n_old_evn = not1(_XYVO_y144p_old_evn);
    /*#p29.ABOV*/ wire _ABOV_LINE_P908p_old_evn = and2(_SELA_LINE_P908p_old_evn, _ALES_y144n_old_evn);
    /*#p28.ANEL*/ ANEL_LINE_P002p_c.dff17(_AWOH_xxCDxxGH_clknew,  _ABEZ_VID_RSTn_new_evn,  CATU_LINE_P000p_a.qp_old());
    /*#p29.CATU*/ CATU_LINE_P000p_a.dff17( XUPY_ABxxEFxx_clk_evn,  _ABEZ_VID_RSTn_new_evn, _ABOV_LINE_P908p_old_evn);
  }
}

//--------------------------------------------------------------------------------

void GateBoyLCD::tock_pins(
  wire XODO_VID_RSTp,
  wire TALU_xxCDEFxx,
  wire TYFA_CLKPIPE_odd,
  wire XONA_LCDC_LCDENp,
  wire XYMU_RENDERINGp,
  wire AVAP_SCAN_DONE_TRIGp,
  wire XYDO_PX3p,
  wire TULU_DIV07p,
  wire REMY_LD0n,
  wire RAVO_LD1n,
  const RegLX& reg_lx,
  const RegLY& reg_ly
) {
  //lcd_data0_delay.set(_REMY_LD0n);
  //lcd_data1_delay.set(_RAVO_LD1n);
  PIN_LCD_DATA0.pin_out(/*lcd_data0_delay.q1_new()*/ REMY_LD0n, /*lcd_data0_delay.q1_new()*/ REMY_LD0n);
  PIN_LCD_DATA1.pin_out(/*lcd_data1_delay.q1_new()*/ RAVO_LD1n, /*lcd_data1_delay.q1_new()*/ RAVO_LD1n);

  {
    /*#p21.TOCU*/ wire _TOCU_LX0n_evn_new = not1(reg_lx.SAXO_LX0p_evn.qp_new());
    /*#p21.VEPE*/ wire _VEPE_LX1n_evn_new = not1(reg_lx.TYPO_LX1p_evn.qp_new());
    /* p21.VUTY*/ wire _VUTY_LX2n_evn_new = not1(reg_lx.VYZO_LX2p_evn.qp_new());
    /* p21.VATE*/ wire _VATE_LX3n_evn_new = not1(reg_lx.TELU_LX3p_evn.qp_new());
    /* p21.TUDA*/ wire _TUDA_LX4n_evn_new = not1(reg_lx.SUDE_LX4p_evn.qp_new());
    /* p21.TAFY*/ wire _TAFY_LX5n_evn_new = not1(reg_lx.TAHA_LX5p_evn.qp_new());
    /* p21.TUJU*/ wire _TUJU_LX6n_evn_new = not1(reg_lx.TYRY_LX6p_evn.qp_new());

    /*#p21.SONO*/ wire _SONO_ABxxxxGH_clkevn = not1(TALU_xxCDEFxx);

    /* p21.VOKU*/ wire _VOKU_LX000n_new = nand7(_TUJU_LX6n_evn_new,            _TAFY_LX5n_evn_new,            _TUDA_LX4n_evn_new,            _VATE_LX3n_evn_new,            _VUTY_LX2n_evn_new,            _VEPE_LX1n_evn_new,            _TOCU_LX0n_evn_new           ); // 0000000 == 0
    /* p21.TOZU*/ wire _TOZU_LX007n_new = nand7(_TUJU_LX6n_evn_new,            _TAFY_LX5n_evn_new,            _TUDA_LX4n_evn_new,            _VATE_LX3n_evn_new,            reg_lx.VYZO_LX2p_evn.qp_new(), reg_lx.TYPO_LX1p_evn.qp_new(), reg_lx.SAXO_LX0p_evn.qp_new()); // 0000111 == 7
    /* p21.TECE*/ wire _TECE_LX045n_new = nand7(_TUJU_LX6n_evn_new,            reg_lx.TAHA_LX5p_evn.qp_new(), _TUDA_LX4n_evn_new,            reg_lx.TELU_LX3p_evn.qp_new(), reg_lx.VYZO_LX2p_evn.qp_new(), _VEPE_LX1n_evn_new,            reg_lx.SAXO_LX0p_evn.qp_new()); // 0101101 == 45
    /*#p21.TEBO*/ wire _TEBO_LX083n_new = nand7(reg_lx.TYRY_LX6p_evn.qp_new(), _TAFY_LX5n_evn_new,            reg_lx.SUDE_LX4p_evn.qp_new(), _VATE_LX3n_evn_new,            _VUTY_LX2n_evn_new,            reg_lx.TYPO_LX1p_evn.qp_new(), reg_lx.SAXO_LX0p_evn.qp_new()); // 1010011 == 83
    /*#p21.TEGY*/ wire _TEGY_STROBE_new = nand4(_VOKU_LX000n_new, _TOZU_LX007n_new, _TECE_LX045n_new, _TEBO_LX083n_new);
    /*#p21.SYGU*/ SYGU_LINE_STROBE_evn.dff17(_SONO_ABxxxxGH_clkevn, LYFE_VID_RSTn(XODO_VID_RSTp), _TEGY_STROBE_new);
  }

  {
    /*#p21.RYNO*/ wire RYNO_new_evn = or2(SYGU_LINE_STROBE_evn.qp_new(), reg_lx.RUTU_x113p_g.qp_new());
    /*#p21.POGU*/ wire _POGU_new_evn = not1(RYNO_new_evn);
    PIN_LCD_CNTRL_evn.pin_out(_POGU_new_evn, _POGU_new_evn);
  }

  {
    // if LCDC_ENn, PIN_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)
    /*#p24.LOFU*/ wire _LOFU_LINE_ENDn_evn_new  = not1(reg_lx.RUTU_x113p_g.qp_new());
    /*#p24.LUCA*/ LUCA_LINE_EVENp_evn .dff17(_LOFU_LINE_ENDn_evn_new,   LYFE_VID_RSTn(XODO_VID_RSTp), LUCA_LINE_EVENp_evn.qn_any());
    /*#p21.NAPO*/ NAPO_FRAME_EVENp_evn.dff17(POPU_VBLANKp_evn.qp_new(), LYFE_VID_RSTn(XODO_VID_RSTp), NAPO_FRAME_EVENp_evn.qn_any());
    /*#p24.MAGU*/ wire _MAGU_new_evn = xor2(NAPO_FRAME_EVENp_evn.qp_new(), LUCA_LINE_EVENp_evn.qn_new());
    /*#p24.MECO*/ wire _MECO_new_evn = not1(_MAGU_new_evn);
    /*#p24.KEBO*/ wire _KEBO_new_evn = not1(_MECO_new_evn);
    /* p01.UREK*/ wire _UREK_DIV07n_new_evn = not1(TULU_DIV07p);
    /*#p24.USEC*/ wire _USEC_DIV07p_new_evn = not1(_UREK_DIV07n_new_evn);
    /*#p24.KEDY*/ wire _KEDY_LCDC_ENn_new_evn = not1(XONA_LCDC_LCDENp);
    /*#p24.KUPA*/ wire _KUPA_new_evn = amux2(XONA_LCDC_LCDENp, _KEBO_new_evn, _KEDY_LCDC_ENn_new_evn, _USEC_DIV07p_new_evn);
    /*#p24.KOFO*/ wire _KOFO_new_evn = not1(_KUPA_new_evn);
    PIN_LCD_FLIPS_evn.pin_out(_KOFO_new_evn, _KOFO_new_evn);
  }

  {
    /*#p24.NERU*/ wire NERU_y000p_new_evn = nor8(reg_ly.LAFO_LY7p_evn.qp_new(), reg_ly.LOVU_LY4p_evn.qp_new(), reg_ly.LYDO_LY3p_evn.qp_new(), reg_ly.MUWY_LY0p_evn.qp_new(),
                                                  reg_ly.MYRO_LY1p_evn.qp_new(), reg_ly.LEXA_LY2p_evn.qp_new(), reg_ly.LEMA_LY5p_evn.qp_new(), reg_ly.MATO_LY6p_evn.qp_new());
    /*#p24.MEDA*/ MEDA_VSYNC_OUTn_evn.dff17(reg_lx.NYPE_x113p_c.qn_new(), LYFE_VID_RSTn(XODO_VID_RSTp), NERU_y000p_new_evn);
    /*#p24.MURE*/ wire _MURE_VSYNC_new_evn = not1(MEDA_VSYNC_OUTn_evn.qp_new());
    PIN_LCD_VSYNC_evn.pin_out(_MURE_VSYNC_new_evn, _MURE_VSYNC_new_evn);
  }

  //----------------------------------------
  // FIXME inversion
  // I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
  // seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
  // Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
  // That would be a loooot of gate delay.
  // Could we possibly be incrementing X3p one phase early?

  {
    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx_clknew_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire _ROXO_CLKPIPE_xBxDxFxH_clknew_odd = not1(_SEGU_CLKPIPE_AxCxExGx_clknew_evn);
    /* p24.PAHO*/ PAHO_X_8_SYNC_odd.dff17(!_ROXO_CLKPIPE_xBxDxFxH_clknew_odd, XYMU_RENDERINGp, XYDO_PX3p);

    // LCD horizontal sync pin latch
    /*#p24.POME*/ POME.set(nor2(AVAP_SCAN_DONE_TRIGp, POFY.qp_old()));
    /*#p24.RUJU*/ RUJU.set(or3(PAHO_X_8_SYNC_odd.qp_new(), TOFU_VID_RSTp(XODO_VID_RSTp), POME.qp_new()));
    /*#p24.POFY*/ POFY.set(not1(RUJU.qp_new()));
    /*#p24.POME*/ POME.set(nor2(AVAP_SCAN_DONE_TRIGp, POFY.qp_new()));
    /*#p24.RUJU*/ RUJU.set(or3(PAHO_X_8_SYNC_odd.qp_new(), TOFU_VID_RSTp(XODO_VID_RSTp), POME.qp_new()));
    /*#p24.POFY*/ POFY.set(not1(RUJU.qp_new()));

    /*#p24.RUZE*/ wire _RUZE_HSYNCn_new = not1(POFY.qp_new());
    PIN_LCD_HSYNC_evn.pin_out(_RUZE_HSYNCn_new, _RUZE_HSYNCn_new);
  }
}

//------------------------------------------------------------------------------------------------------------------------