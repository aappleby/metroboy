#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct LCDC {
  void reset_cart() {
    VYXE_LCDC_BGENn_h  .reset(REG_D0C1);
    XYLO_LCDC_SPENn_h  .reset(REG_D1C1);
    XYMO_LCDC_SPSIZEn_h.reset(REG_D1C1);
    XONA_LCDC_LCDENn_h .reset(REG_D0C1);
    XAFO_LCDC_BGMAPn_h .reset(REG_D1C1);
    WEXU_LCDC_BGTILEn_h.reset(REG_D0C1);
    WYMO_LCDC_WINENn_h .reset(REG_D1C1);
    WOKY_LCDC_WINMAPn_h.reset(REG_D1C1);
  }

  void reset_boot() {
    VYXE_LCDC_BGENn_h  .reset(REG_D0C0);
    XYLO_LCDC_SPENn_h  .reset(REG_D0C0);
    XYMO_LCDC_SPSIZEn_h.reset(REG_D0C0);
    XONA_LCDC_LCDENn_h .reset(REG_D0C0);
    XAFO_LCDC_BGMAPn_h .reset(REG_D0C0);
    WEXU_LCDC_BGTILEn_h.reset(REG_D0C0);
    WYMO_LCDC_WINENn_h .reset(REG_D0C0);
    WOKY_LCDC_WINMAPn_h.reset(REG_D0C0);
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

struct LYC {
  void reset_cart() {
    SYRY_LYC0n.reset(REG_D1C1);
    VUCE_LYC1n.reset(REG_D1C1);
    SEDY_LYC2n.reset(REG_D1C1);
    SALO_LYC3n.reset(REG_D1C1);
    SOTA_LYC4n.reset(REG_D1C1);
    VAFA_LYC5n.reset(REG_D1C1);
    VEVO_LYC6n.reset(REG_D1C1);
    RAHA_LYC7n.reset(REG_D1C1);
  }

  void reset_boot() {
    SYRY_LYC0n.reset(REG_D0C0);
    VUCE_LYC1n.reset(REG_D0C0);
    SEDY_LYC2n.reset(REG_D0C0);
    SALO_LYC3n.reset(REG_D0C0);
    SOTA_LYC4n.reset(REG_D0C0);
    VAFA_LYC5n.reset(REG_D0C0);
    VEVO_LYC6n.reset(REG_D0C0);
    RAHA_LYC7n.reset(REG_D0C0);
  }

  uint8_t get_lyc() const { return pack_u8n(8, &SYRY_LYC0n); }

  void tock_reg_lyc(
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp_new,
    wire TEDO_CPU_RDp_ext,
    wire TAPU_CPU_WRp_clkevn,

    wire WERO_ADDR_PPUp_ext,
    wire WADO_A00p_ext,
    wire XENO_A01n_ext,
    wire WALO_A02p_ext,
    wire XERA_A03n_ext,
    BusOut BUS_CPU_D_out[8])
  {
    /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp_new);
    /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
    /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);
    /*#p01.XORE*/ wire _XORE_SYS_RSTp_new = not1(_CUNU_SYS_RSTn_new);
    /* p01.WESY*/ wire _WESY_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);

    /* p07.AJAS*/ wire _AJAS_CPU_RDn_ext = not1(TEDO_CPU_RDp_ext);
    /* p07.ASOT*/ wire _ASOT_CPU_RDp_ext = not1(_AJAS_CPU_RDn_ext);

    /* p07.DYKY*/ wire _DYKY_CPU_WRn_clk_evn = not1(TAPU_CPU_WRp_clkevn);
    /* p07.CUPA*/ wire _CUPA_CPU_WRp_clk_evn = not1(_DYKY_CPU_WRn_clk_evn);

    /* p22.WETY*/ wire _WETY_FF45n_ext = nand5(WERO_ADDR_PPUp_ext, WADO_A00p_ext, XENO_A01n_ext, WALO_A02p_ext, XERA_A03n_ext);
    /* p22.XAYU*/ wire _XAYU_FF45p_ext = not1(_WETY_FF45n_ext);
    /* p23.XYLY*/ wire _XYLY_FF45_RDp_ext = and2(_ASOT_CPU_RDp_ext, _XAYU_FF45p_ext);
    /* p23.XUFA*/ wire _XUFA_FF45_WRn_clk_evn = and2(_CUPA_CPU_WRp_clk_evn, _XAYU_FF45p_ext);
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

  /*p23.SYRY*/ DFF9 SYRY_LYC0n; // xxxxxxxH
  /*p23.VUCE*/ DFF9 VUCE_LYC1n; // xxxxxxxH
  /*p23.SEDY*/ DFF9 SEDY_LYC2n; // xxxxxxxH
  /*p23.SALO*/ DFF9 SALO_LYC3n; // xxxxxxxH
  /*p23.SOTA*/ DFF9 SOTA_LYC4n; // xxxxxxxH
  /*p23.VAFA*/ DFF9 VAFA_LYC5n; // xxxxxxxH
  /*p23.VEVO*/ DFF9 VEVO_LYC6n; // xxxxxxxH
  /*p23.RAHA*/ DFF9 RAHA_LYC7n; // xxxxxxxH
};

struct LcdRegisters {

  void reset_cart() {
    ROPO_LY_MATCH_SYNCp_c.reset(REG_D1C0);

    CATU_LINE_P000p_a.reset(REG_D0C1);
    NYPE_x113p_c.reset(REG_D0C0);
    ANEL_LINE_P002p_c.reset(REG_D0C0);
    RUTU_x113p_g.reset(REG_D0C1);

    MUWY_LY0p_evn.reset(REG_D0C0);
    MYRO_LY1p_evn.reset(REG_D0C1);
    LEXA_LY2p_evn.reset(REG_D0C1);
    LYDO_LY3p_evn.reset(REG_D0C1);
    LOVU_LY4p_evn.reset(REG_D0C1);
    LEMA_LY5p_evn.reset(REG_D0C1);
    MATO_LY6p_evn.reset(REG_D0C1);
    LAFO_LY7p_evn.reset(REG_D0C1);

    SAXO_LX0p_evn.reset(REG_D0C0);
    TYPO_LX1p_evn.reset(REG_D1C1);
    VYZO_LX2p_evn.reset(REG_D0C0);
    TELU_LX3p_evn.reset(REG_D0C1);
    SUDE_LX4p_evn.reset(REG_D0C1);
    TAHA_LX5p_evn.reset(REG_D1C1);
    TYRY_LX6p_evn.reset(REG_D1C0);

    POPU_VBLANKp_evn .reset(REG_D1C0);
    MYTA_y153p_evn  .reset(REG_D1C1);
    SYGU_LINE_STROBE_evn.reset(REG_D0C1);
    MEDA_VSYNC_OUTn_evn .reset(REG_D0C1);
    LUCA_LINE_EVENp_evn  .reset(REG_D1C1);
    NAPO_FRAME_EVENp_evn .reset(REG_D0C1);
  }

  void reset_boot() {
    ROPO_LY_MATCH_SYNCp_c.reset(REG_D0C0);

    CATU_LINE_P000p_a.reset(REG_D0C0);
    NYPE_x113p_c.reset(REG_D0C0);
    ANEL_LINE_P002p_c.reset(REG_D0C0);
    RUTU_x113p_g.reset(REG_D0C0);

    MUWY_LY0p_evn.reset(REG_D0C0);
    MYRO_LY1p_evn.reset(REG_D0C0);
    LEXA_LY2p_evn.reset(REG_D0C0);
    LYDO_LY3p_evn.reset(REG_D0C0);
    LOVU_LY4p_evn.reset(REG_D0C0);
    LEMA_LY5p_evn.reset(REG_D0C0);
    MATO_LY6p_evn.reset(REG_D0C0);
    LAFO_LY7p_evn.reset(REG_D0C0);

    SAXO_LX0p_evn.reset(REG_D0C0);
    TYPO_LX1p_evn.reset(REG_D0C0);
    VYZO_LX2p_evn.reset(REG_D0C0);
    TELU_LX3p_evn.reset(REG_D0C0);
    SUDE_LX4p_evn.reset(REG_D0C0);
    TAHA_LX5p_evn.reset(REG_D0C0);
    TYRY_LX6p_evn.reset(REG_D0C0);

    POPU_VBLANKp_evn.reset(REG_D0C0);
    MYTA_y153p_evn.reset(REG_D0C0);
    SYGU_LINE_STROBE_evn.reset(REG_D0C0);
    MEDA_VSYNC_OUTn_evn.reset(REG_D0C0);
    LUCA_LINE_EVENp_evn.reset(REG_D0C0);
    NAPO_FRAME_EVENp_evn.reset(REG_D0C0);
  }

  uint8_t get_lx() const  { return pack_u8p(7, &SAXO_LX0p_evn); }
  uint8_t get_ly() const  { return pack_u8p(8, &MUWY_LY0p_evn); }

  wire PURE_LINE_ENDn_new_evn() const {
    /*#p21.PURE*/ wire _PURE_LINE_ENDn_new_evn = not1(RUTU_x113p_g.qp_new());
    return _PURE_LINE_ENDn_new_evn;
  }

  wire ATEJ_LINE_TRIGp_old(wire XODO_VID_RSTp_old) const {
    /* p01.XAPO*/ wire _XAPO_VID_RSTn_old = not1(XODO_VID_RSTp_old);
    /*#p01.ATAR*/ wire _ATAR_VID_RSTp_old = not1(_XAPO_VID_RSTn_old);
    /*#p01.ABEZ*/ wire _ABEZ_VID_RSTn_old = not1(_ATAR_VID_RSTp_old);
    /* p28.ABAF*/ wire _ABAF_LINE_P000n_old = not1(CATU_LINE_P000p_a.qp_old());
    /* p28.BYHA*/ wire _BYHA_LINE_TRIGn_old = or_and3(ANEL_LINE_P002p_c.qp_old(), _ABAF_LINE_P000n_old, _ABEZ_VID_RSTn_old); // so if this is or_and, BYHA should go low on 910 and 911
    /* p28.ATEJ*/ wire _ATEJ_LINE_TRIGp_old = not1(_BYHA_LINE_TRIGn_old);
    return _ATEJ_LINE_TRIGp_old;
  }

  wire ATEJ_LINE_TRIGp_new_evn(wire XODO_VID_RSTp_new_h) const {
    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp_new_h);
    /*#p01.ATAR*/ wire _ATAR_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
    /*#p01.ABEZ*/ wire _ABEZ_VID_RSTn_new_evn = not1(_ATAR_VID_RSTp_new_evn);
    /* p28.ABAF*/ wire _ABAF_LINE_P000n_new_evn = not1(CATU_LINE_P000p_a.qp_new());
    /* p28.BYHA*/ wire _BYHA_LINE_TRIGn_new_evn = or_and3(ANEL_LINE_P002p_c.qp_new(), _ABAF_LINE_P000n_new_evn, _ABEZ_VID_RSTn_new_evn); // so if this is or_and, BYHA should go low on 910 and 911
    /* p28.ATEJ*/ wire _ATEJ_LINE_TRIGp_new_evn = not1(_BYHA_LINE_TRIGn_new_evn);
    return _ATEJ_LINE_TRIGp_new_evn;
  }

  void tock_counters(wire XODO_VID_RSTp_new_h, wire TALU_xxCDEFxx_clkevn) {
    [this, XODO_VID_RSTp_new_h, TALU_xxCDEFxx_clkevn](){
      /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp_new_h);
      /* p01.LYHA*/ wire _LYHA_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
      /*#p21.MUDE*/ wire _MUDE_X_RSTn_new_evn = nor2(RUTU_x113p_g.qp_new(), _LYHA_VID_RSTp_new_evn);
      /*#p21.SAXO*/ SAXO_LX0p_evn.dff17(TALU_xxCDEFxx_clkevn,           _MUDE_X_RSTn_new_evn, SAXO_LX0p_evn.qn_any());
      /*#p21.TYPO*/ TYPO_LX1p_evn.dff17(SAXO_LX0p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TYPO_LX1p_evn.qn_any());
      /*#p21.VYZO*/ VYZO_LX2p_evn.dff17(TYPO_LX1p_evn.qn_new(), _MUDE_X_RSTn_new_evn, VYZO_LX2p_evn.qn_any());
      /*#p21.TELU*/ TELU_LX3p_evn.dff17(VYZO_LX2p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TELU_LX3p_evn.qn_any());
      /*#p21.SUDE*/ SUDE_LX4p_evn.dff17(TELU_LX3p_evn.qn_new(), _MUDE_X_RSTn_new_evn, SUDE_LX4p_evn.qn_any());
      /*#p21.TAHA*/ TAHA_LX5p_evn.dff17(SUDE_LX4p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TAHA_LX5p_evn.qn_any());
      /*#p21.TYRY*/ TYRY_LX6p_evn.dff17(TAHA_LX5p_evn.qn_new(), _MUDE_X_RSTn_new_evn, TYRY_LX6p_evn.qn_any());
    }();

    [this, XODO_VID_RSTp_new_h](){
      /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp_new_h);
      /* p01.LYHA*/ wire _LYHA_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
      /* p01.LYFE*/ wire _LYFE_VID_RSTn_new_evn = not1(_LYHA_VID_RSTp_new_evn);
      /*#p21.NOKO*/ wire _NOKO_y153p_old_evn = and4(LAFO_LY7p_evn.qp_old(), LOVU_LY4p_evn.qp_old(), LYDO_LY3p_evn.qp_old(), MUWY_LY0p_evn.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
      /*#p21.MYTA*/ MYTA_y153p_evn.dff17(NYPE_x113p_c.qn_new(), _LYFE_VID_RSTn_new_evn, _NOKO_y153p_old_evn);

      /*#p21.LAMA*/ wire _LAMA_Y_RSTn_new_evn = nor2(MYTA_y153p_evn.qp_new(), _LYHA_VID_RSTp_new_evn);
      /*#p21.MUWY*/ MUWY_LY0p_evn.dff17(RUTU_x113p_g.qp_new(),  _LAMA_Y_RSTn_new_evn, MUWY_LY0p_evn.qn_any());
      /*#p21.MYRO*/ MYRO_LY1p_evn.dff17(MUWY_LY0p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, MYRO_LY1p_evn.qn_any());
      /*#p21.LEXA*/ LEXA_LY2p_evn.dff17(MYRO_LY1p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LEXA_LY2p_evn.qn_any());
      /*#p21.LYDO*/ LYDO_LY3p_evn.dff17(LEXA_LY2p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LYDO_LY3p_evn.qn_any());
      /*#p21.LOVU*/ LOVU_LY4p_evn.dff17(LYDO_LY3p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LOVU_LY4p_evn.qn_any());
      /*#p21.LEMA*/ LEMA_LY5p_evn.dff17(LOVU_LY4p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LEMA_LY5p_evn.qn_any());
      /*#p21.MATO*/ MATO_LY6p_evn.dff17(LEMA_LY5p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, MATO_LY6p_evn.qn_any());
      /*#p21.LAFO*/ LAFO_LY7p_evn.dff17(MATO_LY6p_evn.qn_new(), _LAMA_Y_RSTn_new_evn, LAFO_LY7p_evn.qn_any());
    }();
  }

  void tock_ly(
    wire TEDO_CPU_RDp_ext,
    wire WERO_ADDR_PPUp_ext,
    wire XOLA_A00n_ext,
    wire XENO_A01n_ext,
    wire WALO_A02p_ext,
    wire XERA_A03n_ext,
    BusOut BUS_CPU_D_out[8])
  {
    /* p07.AJAS*/ wire _AJAS_CPU_RDn_ext = not1(TEDO_CPU_RDp_ext);
    /* p07.ASOT*/ wire _ASOT_CPU_RDp_ext = not1(_AJAS_CPU_RDn_ext);

    /* FF44 LY */
    /* p22.WYLE*/ wire _WYLE_FF44n_ext = nand5(WERO_ADDR_PPUp_ext, XOLA_A00n_ext, XENO_A01n_ext, WALO_A02p_ext, XERA_A03n_ext);
    /* p22.XOGY*/ wire _XOGY_FF44p_ext = not1(_WYLE_FF44n_ext);
    /* p23.WAFU*/ wire _WAFU_FF44_RDp_ext = and2(_ASOT_CPU_RDp_ext, _XOGY_FF44p_ext);
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

  void tock_lyc_match(
    wire AVOR_SYS_RSTp_new,
    wire TALU_xxCDEFxx_clkevn,
    const LYC& lyc_old
  )
  {
    /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp_new);
    /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
    /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);
    /*#p01.XORE*/ wire _XORE_SYS_RSTp_new = not1(_CUNU_SYS_RSTn_new);
    /* p01.WESY*/ wire _WESY_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);

    // LYC matcher
    /* p21.RYME*/ wire _RYME_LY_MATCH0n_old_evn = xor2(MUWY_LY0p_evn.qp_old(), lyc_old.SYRY_LYC0n.qn_old());
    /* p21.TYDE*/ wire _TYDE_LY_MATCH1n_old_evn = xor2(MYRO_LY1p_evn.qp_old(), lyc_old.VUCE_LYC1n.qn_old());
    /* p21.REDA*/ wire _REDA_LY_MATCH2n_old_evn = xor2(LEXA_LY2p_evn.qp_old(), lyc_old.SEDY_LYC2n.qn_old());
    /* p21.RASY*/ wire _RASY_LY_MATCH3n_old_evn = xor2(LYDO_LY3p_evn.qp_old(), lyc_old.SALO_LYC3n.qn_old());
    /* p21.TYKU*/ wire _TYKU_LY_MATCH4n_old_evn = xor2(LOVU_LY4p_evn.qp_old(), lyc_old.SOTA_LYC4n.qn_old());
    /* p21.TUCY*/ wire _TUCY_LY_MATCH5n_old_evn = xor2(LEMA_LY5p_evn.qp_old(), lyc_old.VAFA_LYC5n.qn_old());
    /* p21.TERY*/ wire _TERY_LY_MATCH6n_old_evn = xor2(MATO_LY6p_evn.qp_old(), lyc_old.VEVO_LYC6n.qn_old());
    /* p21.SYFU*/ wire _SYFU_LY_MATCH7n_old_evn = xor2(LAFO_LY7p_evn.qp_old(), lyc_old.RAHA_LYC7n.qn_old());

    /*#p21.SOVU*/ wire _SOVU_LY_MATCHA_old_evn = nor4 (_SYFU_LY_MATCH7n_old_evn, _TERY_LY_MATCH6n_old_evn, _TUCY_LY_MATCH5n_old_evn, _TYKU_LY_MATCH4n_old_evn);
    /*#p21.SUBO*/ wire _SUBO_LY_MATCHB_old_evn = nor4 (_RASY_LY_MATCH3n_old_evn, _REDA_LY_MATCH2n_old_evn, _TYDE_LY_MATCH1n_old_evn, _RYME_LY_MATCH0n_old_evn);
    /*#p21.RAPE*/ wire _RAPE_LY_MATCHn_old_evn = nand2(_SOVU_LY_MATCHA_old_evn,  _SUBO_LY_MATCHB_old_evn);
    /*#p21.PALY*/ wire _PALY_LY_MATCHa_old_evn = not1 (_RAPE_LY_MATCHn_old_evn);

    /*#p21.ROPO*/ ROPO_LY_MATCH_SYNCp_c.dff17(TALU_xxCDEFxx_clkevn,  _WESY_SYS_RSTn_new, _PALY_LY_MATCHa_old_evn);
  }

  // H deltas are due to reg writes
  /*p21.ROPO*/ DFF17 ROPO_LY_MATCH_SYNCp_c;   // xxCxxxxx
  /*p29.CATU*/ DFF17 CATU_LINE_P000p_a;       // Axxxxxxx
  /*p21.NYPE*/ DFF17 NYPE_x113p_c;            // xxCxxxxx
  /*p28.ANEL*/ DFF17 ANEL_LINE_P002p_c;       // xxCxxxxx
  /*p21.RUTU*/ DFF17 RUTU_x113p_g;            // xxxxxxGx

  /*p21.POPU*/ DFF17 POPU_VBLANKp_evn;        // xxCxxxxH
  /*p21.MYTA*/ DFF17 MYTA_y153p_evn;          // xxCxxxxH
  /*p21.SYGU*/ DFF17 SYGU_LINE_STROBE_evn;    // xxxxxxGH
  /*p24.MEDA*/ DFF17 MEDA_VSYNC_OUTn_evn;     // xxCxxxxH
  /*p24.LUCA*/ DFF17 LUCA_LINE_EVENp_evn;     // xxxxxxGH
  /*p21.NAPO*/ DFF17 NAPO_FRAME_EVENp_evn;    // xxCxxxxx

  // Increments at P010 (because of RUTU holding it in reset) and then at every A phase.
  /*p21.SAXO*/ DFF17 SAXO_LX0p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYPO*/ DFF17 TYPO_LX1p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.VYZO*/ DFF17 VYZO_LX2p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TELU*/ DFF17 TELU_LX3p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.SUDE*/ DFF17 SUDE_LX4p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TAHA*/ DFF17 TAHA_LX5p_evn; // xxCxxxGx Ticks on C, reset on G
  /*p21.TYRY*/ DFF17 TYRY_LX6p_evn; // xxCxxxGx Ticks on C, reset on G

  // -> pix pipe, sprite scanner, vram bus. Increments at P910
  /*p21.MUWY*/ DFF17 MUWY_LY0p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.MYRO*/ DFF17 MYRO_LY1p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LEXA*/ DFF17 LEXA_LY2p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LYDO*/ DFF17 LYDO_LY3p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LOVU*/ DFF17 LOVU_LY4p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LEMA*/ DFF17 LEMA_LY5p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.MATO*/ DFF17 MATO_LY6p_evn; // xxCxxxGx Ticks on G, reset on C
  /*p21.LAFO*/ DFF17 LAFO_LY7p_evn; // xxCxxxGx Ticks on G, reset on C
};

//-----------------------------------------------------------------------------
