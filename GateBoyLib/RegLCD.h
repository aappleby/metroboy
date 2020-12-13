#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct RegLX {
  uint8_t get() const  { return pack_u8p(7, &SAXO_LX0p_evn); }

  wire PURE_LINE_ENDn_new_evn() const {
    /*#p21.PURE*/ wire _PURE_LINE_ENDn_new_evn = not1(RUTU_x113p_g.qp_new());
    return _PURE_LINE_ENDn_new_evn;
  }

  void tock(wire XODO_VID_RSTp_new_h,
            wire TALU_xxCDEFxx_clkevn) {
    /*#p21.SONO*/ wire _SONO_ABxxxxGH_clknew = not1(TALU_xxCDEFxx_clkevn);

    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp_new_h);
    /* p01.LYHA*/ wire _LYHA_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
    /* p01.LYFE*/ wire _LYFE_VID_RSTn_new_evn = not1(_LYHA_VID_RSTp_new_evn);

    /*#p21.SANU*/ wire _SANU_x113p_old_evn = and4(TYRY_LX6p_evn.qp_old(), TAHA_LX5p_evn.qp_old(), SUDE_LX4p_evn.qp_old(), SAXO_LX0p_evn.qp_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*#p21.NYPE*/ NYPE_x113p_c.dff17( TALU_xxCDEFxx_clkevn, _LYFE_VID_RSTn_new_evn, RUTU_x113p_g.qp_old());
    /*#p21.RUTU*/ RUTU_x113p_g.dff17(_SONO_ABxxxxGH_clknew, _LYFE_VID_RSTn_new_evn, _SANU_x113p_old_evn);

    /*#p21.MUDE*/ wire _MUDE_X_RSTn_new_evn = nor2(RUTU_x113p_g.qp_new(), _LYHA_VID_RSTp_new_evn);
    /*#p21.SAXO*/ SAXO_LX0p_evn.dff17(TALU_xxCDEFxx_clkevn,   _MUDE_X_RSTn_new_evn, SAXO_LX0p_evn.qn_any());
    /*#p21.TYPO*/ TYPO_LX1p_evn.dff17(SAXO_LX0p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TYPO_LX1p_evn.qn_any());
    /*#p21.VYZO*/ VYZO_LX2p_evn.dff17(TYPO_LX1p_evn.qn_new(), _MUDE_X_RSTn_new_evn, VYZO_LX2p_evn.qn_any());
    /*#p21.TELU*/ TELU_LX3p_evn.dff17(VYZO_LX2p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TELU_LX3p_evn.qn_any());
    /*#p21.SUDE*/ SUDE_LX4p_evn.dff17(TELU_LX3p_evn.qn_new(), _MUDE_X_RSTn_new_evn, SUDE_LX4p_evn.qn_any());
    /*#p21.TAHA*/ TAHA_LX5p_evn.dff17(SUDE_LX4p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TAHA_LX5p_evn.qn_any());
    /*#p21.TYRY*/ TYRY_LX6p_evn.dff17(TAHA_LX5p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TYRY_LX6p_evn.qn_any());
  }

  /*p21.RUTU*/ DFF17 RUTU_x113p_g; // xxxxxxGx
  /*p21.NYPE*/ DFF17 NYPE_x113p_c; // xxCxxxxx

  /*p21.SAXO*/ DFF17 SAXO_LX0p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYPO*/ DFF17 TYPO_LX1p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.VYZO*/ DFF17 VYZO_LX2p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TELU*/ DFF17 TELU_LX3p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.SUDE*/ DFF17 SUDE_LX4p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TAHA*/ DFF17 TAHA_LX5p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYRY*/ DFF17 TYRY_LX6p_evn; // xxCxxxGx Ticks on C, reset on G
};

//-----------------------------------------------------------------------------

struct RegLY {
  uint8_t get() const  { return pack_u8p(8, &MUWY_LY0p_evn); }

  void tock(wire BUS_CPU_A[16],
            wire XODO_VID_RSTp_new_h,
            wire TEDO_CPU_RDp,
            wire NYPE_x113p_c_new,
            wire RUTU_x113p_g_new,
            BusOut BUS_CPU_D_out[8])
  {
    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp_new_h);
    /* p01.LYHA*/ wire _LYHA_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
    /* p01.LYFE*/ wire _LYFE_VID_RSTn_new_evn = not1(_LYHA_VID_RSTp_new_evn);

    /*#p21.NOKO*/ wire _NOKO_y153p_old_evn = and4(LAFO_LY7p_evn.qp_old(), LOVU_LY4p_evn.qp_old(), LYDO_LY3p_evn.qp_old(), MUWY_LY0p_evn.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*#p21.MYTA*/ MYTA_y153p_evn.dff17(NYPE_x113p_c_new, _LYFE_VID_RSTn_new_evn, _NOKO_y153p_old_evn);

    /*#p21.LAMA*/ wire _LAMA_Y_RSTn_new_evn = nor2(MYTA_y153p_evn.qp_new(), _LYHA_VID_RSTp_new_evn);
    /*#p21.MUWY*/ MUWY_LY0p_evn.dff17(RUTU_x113p_g_new,       _LAMA_Y_RSTn_new_evn, MUWY_LY0p_evn.qn_any());
    /*#p21.MYRO*/ MYRO_LY1p_evn.dff17(MUWY_LY0p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, MYRO_LY1p_evn.qn_any());
    /*#p21.LEXA*/ LEXA_LY2p_evn.dff17(MYRO_LY1p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LEXA_LY2p_evn.qn_any());
    /*#p21.LYDO*/ LYDO_LY3p_evn.dff17(LEXA_LY2p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LYDO_LY3p_evn.qn_any());
    /*#p21.LOVU*/ LOVU_LY4p_evn.dff17(LYDO_LY3p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LOVU_LY4p_evn.qn_any());
    /*#p21.LEMA*/ LEMA_LY5p_evn.dff17(LOVU_LY4p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LEMA_LY5p_evn.qn_any());
    /*#p21.MATO*/ MATO_LY6p_evn.dff17(LEMA_LY5p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, MATO_LY6p_evn.qn_any());
    /*#p21.LAFO*/ LAFO_LY7p_evn.dff17(MATO_LY6p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LAFO_LY7p_evn.qn_any());

    /* p07.AJAS*/ wire _AJAS_CPU_RDn_ext = not1(TEDO_CPU_RDp);
    /* p07.ASOT*/ wire _ASOT_CPU_RDp_ext = not1(_AJAS_CPU_RDn_ext);

    /* p23.WAFU*/ wire _WAFU_FF44_RDp_ext = and2(_ASOT_CPU_RDp_ext, XOGY_FF44p_ext(BUS_CPU_A));
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

  /*p21.MYTA*/ DFF17 MYTA_y153p_evn; // xxCxxxxH

  /*p21.MUWY*/ DFF17 MUWY_LY0p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.MYRO*/ DFF17 MYRO_LY1p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LEXA*/ DFF17 LEXA_LY2p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LYDO*/ DFF17 LYDO_LY3p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LOVU*/ DFF17 LOVU_LY4p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LEMA*/ DFF17 LEMA_LY5p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.MATO*/ DFF17 MATO_LY6p_evn;  // xxCxxxGx Ticks on G, reset on C
  /*p21.LAFO*/ DFF17 LAFO_LY7p_evn;  // xxCxxxGx Ticks on G, reset on C
};

//-----------------------------------------------------------------------------

struct RegLCDC {
  void tock(wire BUS_CPU_A[16],
            wire BUS_CPU_D[8],
            wire AVOR_SYS_RSTp,
            wire TEDO_CPU_RDp,
            wire TAPU_CPU_WRp,
            BusOut BUS_CPU_D_out[8])
  {
    /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);
    /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
    /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);
    /*#p01.XORE*/ wire _XORE_SYS_RSTp_new = not1(_CUNU_SYS_RSTn_new);
    /* p01.XARE*/ wire _XARE_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);

    /* p07.AJAS*/ wire _AJAS_CPU_RDn_ext = not1(TEDO_CPU_RDp);
    /* p07.ASOT*/ wire _ASOT_CPU_RDp_ext = not1(_AJAS_CPU_RDn_ext);

    /* p07.DYKY*/ wire _DYKY_CPU_WRn_clk = not1(TAPU_CPU_WRp);
    /* p07.CUPA*/ wire _CUPA_CPU_WRp_clk = not1(_DYKY_CPU_WRn_clk);

    /* p23.VYRE*/ wire _VYRE_FF40_RDp_ext = and2(_ASOT_CPU_RDp_ext, VOCA_FF40p_ext(BUS_CPU_A));
    /* p23.WARU*/ wire _WARU_FF40_WRp_clk = and2(_CUPA_CPU_WRp_clk, VOCA_FF40p_ext(BUS_CPU_A));

    // FF40 LCDC
    /* p23.XUBO*/ wire _XUBO_FF40_WRn_clk = not1(_WARU_FF40_WRp_clk);
    /*#p23.VYXE*/ VYXE_LCDC_BGENn_h  .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_D[0]);
    /* p23.XYLO*/ XYLO_LCDC_SPENn_h  .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_D[1]);
    /* p23.XYMO*/ XYMO_LCDC_SPSIZEn_h.dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_D[2]);
    /* p23.XAFO*/ XAFO_LCDC_BGMAPn_h .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_D[3]);
    /* p23.WEXU*/ WEXU_LCDC_BGTILEn_h.dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_D[4]);
    /* p23.WYMO*/ WYMO_LCDC_WINENn_h .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_D[5]);
    /* p23.WOKY*/ WOKY_LCDC_WINMAPn_h.dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_D[6]);
    /* p23.XONA*/ XONA_LCDC_LCDENn_h .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_D[7]);

    // FF40 LCDC
    /* p23.WYCE*/ wire _WYCE_FF40_RDn_ext = not1(_VYRE_FF40_RDp_ext);
    /*#p23.WYPO*/ BUS_CPU_D_out[0].tri6_nn(_WYCE_FF40_RDn_ext, VYXE_LCDC_BGENn_h.qp_new());
    /*#p23.XERO*/ BUS_CPU_D_out[1].tri6_nn(_WYCE_FF40_RDn_ext, XYLO_LCDC_SPENn_h.qp_new());
    /* p23.WYJU*/ BUS_CPU_D_out[2].tri6_nn(_WYCE_FF40_RDn_ext, XYMO_LCDC_SPSIZEn_h.qp_new());
    /* p23.WUKA*/ BUS_CPU_D_out[3].tri6_nn(_WYCE_FF40_RDn_ext, XAFO_LCDC_BGMAPn_h.qp_new());
    /* p23.VOKE*/ BUS_CPU_D_out[4].tri6_nn(_WYCE_FF40_RDn_ext, WEXU_LCDC_BGTILEn_h.qp_new());
    /* p23.VATO*/ BUS_CPU_D_out[5].tri6_nn(_WYCE_FF40_RDn_ext, WYMO_LCDC_WINENn_h.qp_new());
    /*#p23.VAHA*/ BUS_CPU_D_out[6].tri6_nn(_WYCE_FF40_RDn_ext, WOKY_LCDC_WINMAPn_h.qp_new());
    /*#p23.XEBU*/ BUS_CPU_D_out[7].tri6_nn(_WYCE_FF40_RDn_ext, XONA_LCDC_LCDENn_h.qp_new());
  }

  /*p23.VYXE*/ DFF9 VYXE_LCDC_BGENn_h;   // xxxxxxxH
  /*p23.XYLO*/ DFF9 XYLO_LCDC_SPENn_h;   // xxxxxxxH
  /*p23.XYMO*/ DFF9 XYMO_LCDC_SPSIZEn_h; // xxxxxxxH
  /*p23.XONA*/ DFF9 XONA_LCDC_LCDENn_h;  // xxxxxxxH
  /*p23.XAFO*/ DFF9 XAFO_LCDC_BGMAPn_h;  // xxxxxxxH
  /*p23.WEXU*/ DFF9 WEXU_LCDC_BGTILEn_h; // xxxxxxxH
  /*p23.WYMO*/ DFF9 WYMO_LCDC_WINENn_h;  // xxxxxxxH
  /*p23.WOKY*/ DFF9 WOKY_LCDC_WINMAPn_h; // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct RegLYC {
  uint8_t get() const { return pack_u8n(8, &SYRY_LYC0n); }

  void tock(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire TALU_xxCDEFxx_clkevn,

    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,

    const RegLY& reg_ly,

    BusOut BUS_CPU_D_out[8])
  {
    /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);
    /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
    /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);
    /*#p01.XORE*/ wire _XORE_SYS_RSTp_new = not1(_CUNU_SYS_RSTn_new);
    /* p01.WESY*/ wire _WESY_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);

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

    /*#p21.ROPO*/ ROPO_LY_MATCH_SYNCp_c.dff17(TALU_xxCDEFxx_clkevn,  _WESY_SYS_RSTn_new, _PALY_LY_MATCHa_old_evn);

    /* p07.AJAS*/ wire _AJAS_CPU_RDn_ext = not1(TEDO_CPU_RDp);
    /* p07.ASOT*/ wire _ASOT_CPU_RDp_ext = not1(_AJAS_CPU_RDn_ext);

    /* p07.DYKY*/ wire _DYKY_CPU_WRn_clk_evn = not1(TAPU_CPU_WRp);
    /* p07.CUPA*/ wire _CUPA_CPU_WRp_clk_evn = not1(_DYKY_CPU_WRn_clk_evn);

    /* p23.XYLY*/ wire _XYLY_FF45_RDp_ext = and2(_ASOT_CPU_RDp_ext,         XAYU_FF45p_ext(BUS_CPU_A));
    /* p23.XUFA*/ wire _XUFA_FF45_WRn_clk_evn = and2(_CUPA_CPU_WRp_clk_evn, XAYU_FF45p_ext(BUS_CPU_A));
    /* p23.WEKU*/ wire _WEKU_FF45_RDn_ext = not1(_XYLY_FF45_RDp_ext);
    /* p23.WANE*/ wire _WANE_FF45_WRp_clk_evn = not1(_XUFA_FF45_WRn_clk_evn);

    /* p23.SYRY*/ SYRY_LYC0n.dff9(_WANE_FF45_WRp_clk_evn, _WESY_SYS_RSTn_new, BUS_CPU_D[0]);
    /* p23.VUCE*/ VUCE_LYC1n.dff9(_WANE_FF45_WRp_clk_evn, _WESY_SYS_RSTn_new, BUS_CPU_D[1]);
    /* p23.SEDY*/ SEDY_LYC2n.dff9(_WANE_FF45_WRp_clk_evn, _WESY_SYS_RSTn_new, BUS_CPU_D[2]);
    /* p23.SALO*/ SALO_LYC3n.dff9(_WANE_FF45_WRp_clk_evn, _WESY_SYS_RSTn_new, BUS_CPU_D[3]);
    /* p23.SOTA*/ SOTA_LYC4n.dff9(_WANE_FF45_WRp_clk_evn, _WESY_SYS_RSTn_new, BUS_CPU_D[4]);
    /* p23.VAFA*/ VAFA_LYC5n.dff9(_WANE_FF45_WRp_clk_evn, _WESY_SYS_RSTn_new, BUS_CPU_D[5]);
    /* p23.VEVO*/ VEVO_LYC6n.dff9(_WANE_FF45_WRp_clk_evn, _WESY_SYS_RSTn_new, BUS_CPU_D[6]);
    /* p23.RAHA*/ RAHA_LYC7n.dff9(_WANE_FF45_WRp_clk_evn, _WESY_SYS_RSTn_new, BUS_CPU_D[7]);

    /*#p23.RETU*/ BUS_CPU_D_out[0].tri6_nn(_WEKU_FF45_RDn_ext, SYRY_LYC0n.qp_new());
    /* p23.VOJO*/ BUS_CPU_D_out[1].tri6_nn(_WEKU_FF45_RDn_ext, VUCE_LYC1n.qp_new());
    /* p23.RAZU*/ BUS_CPU_D_out[2].tri6_nn(_WEKU_FF45_RDn_ext, SEDY_LYC2n.qp_new());
    /* p23.REDY*/ BUS_CPU_D_out[3].tri6_nn(_WEKU_FF45_RDn_ext, SALO_LYC3n.qp_new());
    /* p23.RACE*/ BUS_CPU_D_out[4].tri6_nn(_WEKU_FF45_RDn_ext, SOTA_LYC4n.qp_new());
    /*#p23.VAZU*/ BUS_CPU_D_out[5].tri6_nn(_WEKU_FF45_RDn_ext, VAFA_LYC5n.qp_new());
    /* p23.VAFE*/ BUS_CPU_D_out[6].tri6_nn(_WEKU_FF45_RDn_ext, VEVO_LYC6n.qp_new());
    /* p23.PUFY*/ BUS_CPU_D_out[7].tri6_nn(_WEKU_FF45_RDn_ext, RAHA_LYC7n.qp_new());
  }

  /*p21.ROPO*/ DFF17 ROPO_LY_MATCH_SYNCp_c;   // xxCxxxxx

  /*p23.SYRY*/ DFF9 SYRY_LYC0n; // xxxxxxxH
  /*p23.VUCE*/ DFF9 VUCE_LYC1n; // xxxxxxxH
  /*p23.SEDY*/ DFF9 SEDY_LYC2n; // xxxxxxxH
  /*p23.SALO*/ DFF9 SALO_LYC3n; // xxxxxxxH
  /*p23.SOTA*/ DFF9 SOTA_LYC4n; // xxxxxxxH
  /*p23.VAFA*/ DFF9 VAFA_LYC5n; // xxxxxxxH
  /*p23.VEVO*/ DFF9 VEVO_LYC6n; // xxxxxxxH
  /*p23.RAHA*/ DFF9 RAHA_LYC7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct LcdRegisters {

  wire ATEJ_LINE_TRIGp_old(wire XODO_VID_RSTp_old) const;
  wire ATEJ_LINE_TRIGp_new_evn(wire XODO_VID_RSTp_new_h) const;
  void tock(
    wire XODO_VID_RSTp_new_h,
    wire XUPY_ABxxEFxx_clk_evn,
    const RegLX& reg_lx,
    const RegLY& reg_ly);

#pragma warning(push)
#pragma warning(disable : 4100)
  void tock_pins(
    wire XODO_VID_RSTp_new_h,
    wire TALU_xxCDEFxx_clkevn,
    wire TYFA_CLKPIPE_xBxDxFxH_clknew_odd,
    wire XONA_LCDC_LCDENp_h_new,
    wire XYMU_RENDERINGp_new_xxx,
    wire AVAP_SCAN_DONE_TRIGp_new_xxx,
    wire XYDO_PX3p_evn_old,
    wire TULU_DIV07p_evn,
    wire REMY_LD0n_new,
    wire RAVO_LD1n_new,
    const RegLX& reg_lx,
    const RegLY& reg_ly
  ) {
#pragma warning(pop)
    //lcd_data0_delay.set(_REMY_LD0n);
    //lcd_data1_delay.set(_RAVO_LD1n);
    PIN_LCD_DATA0.pin_out(/*lcd_data0_delay.q1_new()*/ REMY_LD0n_new, /*lcd_data0_delay.q1_new()*/ REMY_LD0n_new);
    PIN_LCD_DATA1.pin_out(/*lcd_data1_delay.q1_new()*/ RAVO_LD1n_new, /*lcd_data1_delay.q1_new()*/ RAVO_LD1n_new);

    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp_new_h);
    /* p01.LYHA*/ wire _LYHA_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
    /* p01.LYFE*/ wire _LYFE_VID_RSTn_new_evn = not1(_LYHA_VID_RSTp_new_evn);

    /*#p21.TOCU*/ wire _TOCU_LX0n_evn_new = not1(reg_lx.SAXO_LX0p_evn.qp_new());
    /*#p21.VEPE*/ wire _VEPE_LX1n_evn_new = not1(reg_lx.TYPO_LX1p_evn.qp_new());
    /* p21.VUTY*/ wire _VUTY_LX2n_evn_new = not1(reg_lx.VYZO_LX2p_evn.qp_new());
    /* p21.VATE*/ wire _VATE_LX3n_evn_new = not1(reg_lx.TELU_LX3p_evn.qp_new());
    /* p21.TUDA*/ wire _TUDA_LX4n_evn_new = not1(reg_lx.SUDE_LX4p_evn.qp_new());
    /* p21.TAFY*/ wire _TAFY_LX5n_evn_new = not1(reg_lx.TAHA_LX5p_evn.qp_new());
    /* p21.TUJU*/ wire _TUJU_LX6n_evn_new = not1(reg_lx.TYRY_LX6p_evn.qp_new());

    /*#p21.SONO*/ wire _SONO_ABxxxxGH_clkevn = not1(TALU_xxCDEFxx_clkevn);

    /* p21.VOKU*/ wire _VOKU_LX000n_new = nand7(_TUJU_LX6n_evn_new,            _TAFY_LX5n_evn_new,            _TUDA_LX4n_evn_new,            _VATE_LX3n_evn_new,            _VUTY_LX2n_evn_new,            _VEPE_LX1n_evn_new,            _TOCU_LX0n_evn_new           ); // 0000000 == 0
    /* p21.TOZU*/ wire _TOZU_LX007n_new = nand7(_TUJU_LX6n_evn_new,            _TAFY_LX5n_evn_new,            _TUDA_LX4n_evn_new,            _VATE_LX3n_evn_new,            reg_lx.VYZO_LX2p_evn.qp_new(), reg_lx.TYPO_LX1p_evn.qp_new(), reg_lx.SAXO_LX0p_evn.qp_new()); // 0000111 == 7
    /* p21.TECE*/ wire _TECE_LX045n_new = nand7(_TUJU_LX6n_evn_new,            reg_lx.TAHA_LX5p_evn.qp_new(), _TUDA_LX4n_evn_new,            reg_lx.TELU_LX3p_evn.qp_new(), reg_lx.VYZO_LX2p_evn.qp_new(), _VEPE_LX1n_evn_new,            reg_lx.SAXO_LX0p_evn.qp_new()); // 0101101 == 45
    /*#p21.TEBO*/ wire _TEBO_LX083n_new = nand7(reg_lx.TYRY_LX6p_evn.qp_new(), _TAFY_LX5n_evn_new,            reg_lx.SUDE_LX4p_evn.qp_new(), _VATE_LX3n_evn_new,            _VUTY_LX2n_evn_new,            reg_lx.TYPO_LX1p_evn.qp_new(), reg_lx.SAXO_LX0p_evn.qp_new()); // 1010011 == 83
    /*#p21.TEGY*/ wire _TEGY_STROBE_new = nand4(_VOKU_LX000n_new, _TOZU_LX007n_new, _TECE_LX045n_new, _TEBO_LX083n_new);
    /*#p21.SYGU*/ SYGU_LINE_STROBE_evn.dff17(_SONO_ABxxxxGH_clkevn,  _LYFE_VID_RSTn_new_evn, _TEGY_STROBE_new);

    /*#p21.RYNO*/ wire RYNO_new_evn = or2(SYGU_LINE_STROBE_evn.qp_new(), reg_lx.RUTU_x113p_g.qp_new());
    /*#p21.POGU*/ wire _POGU_new_evn = not1(RYNO_new_evn);
    PIN_LCD_CNTRL_evn.pin_out(_POGU_new_evn, _POGU_new_evn);

    // if LCDC_ENn, PIN_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)
    /*#p24.LOFU*/ wire _LOFU_LINE_ENDn_evn_new  = not1(reg_lx.RUTU_x113p_g.qp_new());
    /*#p24.LUCA*/ LUCA_LINE_EVENp_evn .dff17(_LOFU_LINE_ENDn_evn_new,   _LYFE_VID_RSTn_new_evn, LUCA_LINE_EVENp_evn.qn_any());
    /*#p21.NAPO*/ NAPO_FRAME_EVENp_evn.dff17(POPU_VBLANKp_evn.qp_new(), _LYFE_VID_RSTn_new_evn, NAPO_FRAME_EVENp_evn.qn_any());
    /*#p24.MAGU*/ wire _MAGU_new_evn = xor2(NAPO_FRAME_EVENp_evn.qp_new(), LUCA_LINE_EVENp_evn.qn_new());
    /*#p24.MECO*/ wire _MECO_new_evn = not1(_MAGU_new_evn);
    /*#p24.KEBO*/ wire _KEBO_new_evn = not1(_MECO_new_evn);
    /* p01.UREK*/ wire _UREK_DIV07n_new_evn = not1(TULU_DIV07p_evn);
    /*#p24.USEC*/ wire _USEC_DIV07p_new_evn = not1(_UREK_DIV07n_new_evn);
    /*#p24.KEDY*/ wire _KEDY_LCDC_ENn_new_evn = not1(XONA_LCDC_LCDENp_h_new);
    /*#p24.KUPA*/ wire _KUPA_new_evn = amux2(XONA_LCDC_LCDENp_h_new, _KEBO_new_evn, _KEDY_LCDC_ENn_new_evn, _USEC_DIV07p_new_evn);
    /*#p24.KOFO*/ wire _KOFO_new_evn = not1(_KUPA_new_evn);
    PIN_LCD_FLIPS_evn.pin_out(_KOFO_new_evn, _KOFO_new_evn);

    /*#p24.NERU*/ wire NERU_y000p_new_evn = nor8(reg_ly.LAFO_LY7p_evn.qp_new(), reg_ly.LOVU_LY4p_evn.qp_new(), reg_ly.LYDO_LY3p_evn.qp_new(), reg_ly.MUWY_LY0p_evn.qp_new(),
                                                 reg_ly.MYRO_LY1p_evn.qp_new(), reg_ly.LEXA_LY2p_evn.qp_new(), reg_ly.LEMA_LY5p_evn.qp_new(), reg_ly.MATO_LY6p_evn.qp_new());
    /*#p24.MEDA*/ MEDA_VSYNC_OUTn_evn.dff17(reg_lx.NYPE_x113p_c.qn_new(), _LYFE_VID_RSTn_new_evn, NERU_y000p_new_evn);
    /*#p24.MURE*/ wire _MURE_VSYNC_new_evn = not1(MEDA_VSYNC_OUTn_evn.qp_new());
    PIN_LCD_VSYNC_evn.pin_out(_MURE_VSYNC_new_evn, _MURE_VSYNC_new_evn);

    //----------------------------------------
    // FIXME inversion
    // I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
    // seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
    // Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
    // That would be a loooot of gate delay.
    // Could we possibly be incrementing X3p one phase early?

    [
      this,
      XODO_VID_RSTp_new_h,
      XYMU_RENDERINGp_new_xxx,
      AVAP_SCAN_DONE_TRIGp_new_xxx,
      TYFA_CLKPIPE_xBxDxFxH_clknew_odd,
      XYDO_PX3p_evn_old
    ](){
      /* p01.XAPO*/ wire _XAPO_VID_RSTn_new = not1(XODO_VID_RSTp_new_h);
      /* p01.TOFU*/ wire _TOFU_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);

      /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx_clknew_evn = not1(TYFA_CLKPIPE_xBxDxFxH_clknew_odd);
      /*#p24.ROXO*/ wire _ROXO_CLKPIPE_xBxDxFxH_clknew_odd = not1(_SEGU_CLKPIPE_AxCxExGx_clknew_evn);
      /* p24.PAHO*/ PAHO_X_8_SYNC_odd.dff17(!_ROXO_CLKPIPE_xBxDxFxH_clknew_odd, XYMU_RENDERINGp_new_xxx, XYDO_PX3p_evn_old);

      // LCD horizontal sync pin latch
      /*#p24.POME*/ POME_xxx.set(nor2(AVAP_SCAN_DONE_TRIGp_new_xxx, POFY_xxx.qp_old()));
      /*#p24.RUJU*/ RUJU_xxx.set(or3(PAHO_X_8_SYNC_odd.qp_new(), _TOFU_VID_RSTp_new, POME_xxx.qp_new()));
      /*#p24.POFY*/ POFY_xxx.set(not1(RUJU_xxx.qp_new()));
      /*#p24.POME*/ POME_xxx.set(nor2(AVAP_SCAN_DONE_TRIGp_new_xxx, POFY_xxx.qp_new()));
      /*#p24.RUJU*/ RUJU_xxx.set(or3(PAHO_X_8_SYNC_odd.qp_new(), _TOFU_VID_RSTp_new, POME_xxx.qp_new()));
      /*#p24.POFY*/ POFY_xxx.set(not1(RUJU_xxx.qp_new()));

      /*#p24.RUZE*/ wire _RUZE_HSYNCn_new = not1(POFY_xxx.qp_new());
      PIN_LCD_HSYNC_evn.pin_out(_RUZE_HSYNCn_new, _RUZE_HSYNCn_new);
    }();
  }

  // H deltas are due to reg writes
  /*p29.CATU*/ DFF17 CATU_LINE_P000p_a;       // Axxxxxxx
  /*p28.ANEL*/ DFF17 ANEL_LINE_P002p_c;       // xxCxxxxx
  /*p21.POPU*/ DFF17 POPU_VBLANKp_evn;        // xxCxxxxH

  /*p21.SYGU*/ DFF17 SYGU_LINE_STROBE_evn;    // xxxxxxGH
  /*p24.MEDA*/ DFF17 MEDA_VSYNC_OUTn_evn;     // xxCxxxxH
  /*p24.LUCA*/ DFF17 LUCA_LINE_EVENp_evn;     // xxxxxxGH
  /*p21.NAPO*/ DFF17 NAPO_FRAME_EVENp_evn;    // xxCxxxxx

  // RUJU+POFY+POME form a nor latch
  /*p24.RUJU*/ Gate RUJU_xxx; // AxxxxFxx
  /*p24.POFY*/ Gate POFY_xxx; // AxxxxFxx
  /*p24.POME*/ Gate POME_xxx; // AxxxxFxx
  /*p24.PAHO*/ DFF17 PAHO_X_8_SYNC_odd;                 // xBxDxFxH
  /*p21.WUSA*/ NorLatch WUSA_LCD_CLOCK_GATE_xxx;         // xBxDxFGH High on G at beginning of line, low on H at end of line. Not sure what's up with the others. Scroll/sprite count?

  /*PIN_50*/ PinOut PIN_LCD_DATA1;
  /*PIN_51*/ PinOut PIN_LCD_DATA0;
  /*PIN_54*/ PinOut PIN_LCD_HSYNC_evn;
  /*PIN_56*/ PinOut PIN_LCD_FLIPS_evn;
  /*PIN_52*/ PinOut PIN_LCD_CNTRL_evn;
  /*PIN_55*/ PinOut PIN_LCD_LATCH_evn;
  /*PIN_53*/ PinOut PIN_LCD_CLOCK_xxx;
  /*PIN_57*/ PinOut PIN_LCD_VSYNC_evn;

  NorLatch lcd_pix_lo;
  NorLatch lcd_pix_hi;

  DFF lcd_pipe_lo[160];
  DFF lcd_pipe_hi[160];
};

//-----------------------------------------------------------------------------
