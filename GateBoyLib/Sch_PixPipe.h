#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------
// FF4A - WY

struct RegWY {
  void set(uint8_t wy) {
    NESO_WY0n_h.reset((wy & 0x01) ? REG_D0C1 : REG_D1C1);
    NYRO_WY1n_h.reset((wy & 0x02) ? REG_D0C1 : REG_D1C1);
    NAGA_WY2n_h.reset((wy & 0x04) ? REG_D0C1 : REG_D1C1);
    MELA_WY3n_h.reset((wy & 0x08) ? REG_D0C1 : REG_D1C1);
    NULO_WY4n_h.reset((wy & 0x10) ? REG_D0C1 : REG_D1C1);
    NENE_WY5n_h.reset((wy & 0x20) ? REG_D0C1 : REG_D1C1);
    NUKA_WY6n_h.reset((wy & 0x40) ? REG_D0C1 : REG_D1C1);
    NAFU_WY7n_h.reset((wy & 0x80) ? REG_D0C1 : REG_D1C1);
  }

  int get() const { return pack_u8n(8, &NESO_WY0n_h); }

  void tock(const wire BUS_CPU_A[16], const wire BUS_CPU_D[8], wire AVOR_SYS_RSTp, wire TEDO_CPU_RDp, wire TAPU_CPU_WRp, BusOut BUS_CPU_D_out[8]) {
    // FF4A WY
    /* p23.WEKO*/ wire _WEKO_FF4A_WRp_clk_evn = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), VYGA_FF4Ap_ext(BUS_CPU_A));
    /* p23.VEFU*/ wire _VEFU_FF4A_WRn_clk_evn = not1(_WEKO_FF4A_WRp_clk_evn);
    /* p23.NESO*/ NESO_WY0n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[0]);
    /* p23.NYRO*/ NYRO_WY1n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[1]);
    /* p23.NAGA*/ NAGA_WY2n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[2]);
    /* p23.MELA*/ MELA_WY3n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[3]);
    /* p23.NULO*/ NULO_WY4n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[4]);
    /* p23.NENE*/ NENE_WY5n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[5]);
    /* p23.NUKA*/ NUKA_WY6n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[6]);
    /* p23.NAFU*/ NAFU_WY7n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[7]);

    // FF4A WY
    /* p23.WAXU*/ wire _WAXU_FF4A_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), VYGA_FF4Ap_ext(BUS_CPU_A));
    /* p23.VOMY*/ wire _VOMY_FF4A_RDn_ext = not1(_WAXU_FF4A_RDp_ext);
    /*#p23.PUNU*/ BUS_CPU_D_out[0].tri6_nn(_VOMY_FF4A_RDn_ext, NESO_WY0n_h.qp_new());
    /* p23.PODA*/ BUS_CPU_D_out[1].tri6_nn(_VOMY_FF4A_RDn_ext, NYRO_WY1n_h.qp_new());
    /* p23.PYGU*/ BUS_CPU_D_out[2].tri6_nn(_VOMY_FF4A_RDn_ext, NAGA_WY2n_h.qp_new());
    /* p23.LOKA*/ BUS_CPU_D_out[3].tri6_nn(_VOMY_FF4A_RDn_ext, MELA_WY3n_h.qp_new());
    /* p23.MEGA*/ BUS_CPU_D_out[4].tri6_nn(_VOMY_FF4A_RDn_ext, NULO_WY4n_h.qp_new());
    /* p23.PELA*/ BUS_CPU_D_out[5].tri6_nn(_VOMY_FF4A_RDn_ext, NENE_WY5n_h.qp_new());
    /* p23.POLO*/ BUS_CPU_D_out[6].tri6_nn(_VOMY_FF4A_RDn_ext, NUKA_WY6n_h.qp_new());
    /* p23.MERA*/ BUS_CPU_D_out[7].tri6_nn(_VOMY_FF4A_RDn_ext, NAFU_WY7n_h.qp_new());
  }

  /*p23.NESO*/ DFF9 NESO_WY0n_h; // xxxxxxxH
  /*p23.NYRO*/ DFF9 NYRO_WY1n_h; // xxxxxxxH
  /*p23.NAGA*/ DFF9 NAGA_WY2n_h; // xxxxxxxH
  /*p23.MELA*/ DFF9 MELA_WY3n_h; // xxxxxxxH
  /*p23.NULO*/ DFF9 NULO_WY4n_h; // xxxxxxxH
  /*p23.NENE*/ DFF9 NENE_WY5n_h; // xxxxxxxH
  /*p23.NUKA*/ DFF9 NUKA_WY6n_h; // xxxxxxxH
  /*p23.NAFU*/ DFF9 NAFU_WY7n_h; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF4B - WX

struct RegWX {
  void set(uint8_t wx) {
    MYPA_WX0n_h.reset((wx & 0x01) ? REG_D0C1 : REG_D1C1);
    NOFE_WX1n_h.reset((wx & 0x02) ? REG_D0C1 : REG_D1C1);
    NOKE_WX2n_h.reset((wx & 0x04) ? REG_D0C1 : REG_D1C1);
    MEBY_WX3n_h.reset((wx & 0x08) ? REG_D0C1 : REG_D1C1);
    MYPU_WX4n_h.reset((wx & 0x10) ? REG_D0C1 : REG_D1C1);
    MYCE_WX5n_h.reset((wx & 0x20) ? REG_D0C1 : REG_D1C1);
    MUVO_WX6n_h.reset((wx & 0x40) ? REG_D0C1 : REG_D1C1);
    NUKU_WX7n_h.reset((wx & 0x80) ? REG_D0C1 : REG_D1C1);
  }

  int get() const        { return pack_u8n(8, &MYPA_WX0n_h); }

  void tock(const wire BUS_CPU_A[16], const wire BUS_CPU_D[8], wire AVOR_SYS_RSTp, wire TEDO_CPU_RDp, wire TAPU_CPU_WRp, BusOut BUS_CPU_D_out[8]) {
    // FF4B WX
    /* p23.WUZA*/ wire _WUZA_FF4B_WRp_clk_evn = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), VUMY_FF4Bp_ext(BUS_CPU_A));
    /* p23.VOXU*/ wire _VOXU_FF4B_WRn_clk_evn = not1(_WUZA_FF4B_WRp_clk_evn);
    /* p23.MYPA*/ MYPA_WX0n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[0]);
    /* p23.NOFE*/ NOFE_WX1n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[1]);
    /* p23.NOKE*/ NOKE_WX2n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[2]);
    /* p23.MEBY*/ MEBY_WX3n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[3]);
    /* p23.MYPU*/ MYPU_WX4n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[4]);
    /* p23.MYCE*/ MYCE_WX5n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[5]);
    /* p23.MUVO*/ MUVO_WX6n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[6]);
    /* p23.NUKU*/ NUKU_WX7n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[7]);

    // FF4B WX
    /* p23.WYZE*/ wire _WYZE_FF4B_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), VUMY_FF4Bp_ext(BUS_CPU_A));
    /* p23.VYCU*/ wire _VYCU_FF4B_RDn_ext = not1(_WYZE_FF4B_RDp_ext);
    /*#p23.LOVA*/ BUS_CPU_D_out[0].tri6_nn(_VYCU_FF4B_RDn_ext, MYPA_WX0n_h.qp_new());
    /* p23.MUKA*/ BUS_CPU_D_out[1].tri6_nn(_VYCU_FF4B_RDn_ext, NOFE_WX1n_h.qp_new());
    /* p23.MOKO*/ BUS_CPU_D_out[2].tri6_nn(_VYCU_FF4B_RDn_ext, NOKE_WX2n_h.qp_new());
    /* p23.LOLE*/ BUS_CPU_D_out[3].tri6_nn(_VYCU_FF4B_RDn_ext, MEBY_WX3n_h.qp_new());
    /* p23.MELE*/ BUS_CPU_D_out[4].tri6_nn(_VYCU_FF4B_RDn_ext, MYPU_WX4n_h.qp_new());
    /* p23.MUFE*/ BUS_CPU_D_out[5].tri6_nn(_VYCU_FF4B_RDn_ext, MYCE_WX5n_h.qp_new());
    /* p23.MULY*/ BUS_CPU_D_out[6].tri6_nn(_VYCU_FF4B_RDn_ext, MUVO_WX6n_h.qp_new());
    /* p23.MARA*/ BUS_CPU_D_out[7].tri6_nn(_VYCU_FF4B_RDn_ext, NUKU_WX7n_h.qp_new());
  }

  /*p23.MYPA*/ DFF9 MYPA_WX0n_h; // xxxxxxxH
  /*p23.NOFE*/ DFF9 NOFE_WX1n_h; // xxxxxxxH
  /*p23.NOKE*/ DFF9 NOKE_WX2n_h; // xxxxxxxH
  /*p23.MEBY*/ DFF9 MEBY_WX3n_h; // xxxxxxxH
  /*p23.MYPU*/ DFF9 MYPU_WX4n_h; // xxxxxxxH
  /*p23.MYCE*/ DFF9 MYCE_WX5n_h; // xxxxxxxH
  /*p23.MUVO*/ DFF9 MUVO_WX6n_h; // xxxxxxxH
  /*p23.NUKU*/ DFF9 NUKU_WX7n_h; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// Pixel counter

struct PixCounter {

  void tock(wire TADY_LINE_RSTn, wire SACU_CLKPIPE_evn) {
    // Pixel counter, has carry lookahead because this can increment every tcycle
    /* p21.RYBO*/ wire RYBO_old_evn = xor2(XEHO_PX0p.qp_old(), SAVY_PX1p.qp_old()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /* p21.XUKE*/ wire XUKE_old_evn = and2(XEHO_PX0p.qp_old(), SAVY_PX1p.qp_old());
    /* p21.XYLE*/ wire XYLE_old_evn = and2(XODU_PX2p.qp_old(), XUKE_old_evn);
    /* p21.XEGY*/ wire XEGY_old_evn = xor2(XODU_PX2p.qp_old(), XUKE_old_evn); // feet facing gnd
    /* p21.XORA*/ wire XORA_old_evn = xor2(XYDO_PX3p.qp_old(), XYLE_old_evn); // feet facing gnd

    /* p21.XEHO*/ XEHO_PX0p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEHO_PX0p.qn_old());
    /* p21.SAVY*/ SAVY_PX1p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, RYBO_old_evn);
    /* p21.XODU*/ XODU_PX2p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEGY_old_evn);
    /* p21.XYDO*/ XYDO_PX3p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XORA_old_evn);

    /* p24.TOCA*/ wire TOCA_new_evn = not1(XYDO_PX3p.qp_new());
    /* p21.SAKE*/ wire SAKE_old_evn = xor2(TUHU_PX4p.qp_old(), TUKY_PX5p.qp_old());
    /* p21.TYBA*/ wire TYBA_old_evn = and2(TUHU_PX4p.qp_old(), TUKY_PX5p.qp_old());
    /* p21.SURY*/ wire SURY_old_evn = and2(TAKO_PX6p.qp_old(), TYBA_old_evn);
    /* p21.TYGE*/ wire TYGE_old_evn = xor2(TAKO_PX6p.qp_old(), TYBA_old_evn);
    /* p21.ROKU*/ wire ROKU_old_evn = xor2(SYBE_PX7p.qp_old(), SURY_old_evn);

    /* p21.TUHU*/ TUHU_PX4p.dff17(TOCA_new_evn, TADY_LINE_RSTn, TUHU_PX4p.qn_any());
    /* p21.TUKY*/ TUKY_PX5p.dff17(TOCA_new_evn, TADY_LINE_RSTn, SAKE_old_evn);
    /* p21.TAKO*/ TAKO_PX6p.dff17(TOCA_new_evn, TADY_LINE_RSTn, TYGE_old_evn);
    /* p21.SYBE*/ SYBE_PX7p.dff17(TOCA_new_evn, TADY_LINE_RSTn, ROKU_old_evn);
  }

  /*#p21.XUGU*/ wire XANO_PX167p() const {
    /*#p21.XUGU*/ wire _XUGU_PX167n = nand5(XEHO_PX0p.qp_any(), SAVY_PX1p.qp_any(), XODU_PX2p.qp_any(), TUKY_PX5p.qp_any(), SYBE_PX7p.qp_any()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire _XANO_PX167p = not1(_XUGU_PX167n);
    return _XANO_PX167p;
  }

  /*p21.XEHO*/ DFF17 XEHO_PX0p; // AxCxExGx
  /*p21.SAVY*/ DFF17 SAVY_PX1p; // AxCxExGx
  /*p21.XODU*/ DFF17 XODU_PX2p; // AxCxExGx
  /*p21.XYDO*/ DFF17 XYDO_PX3p; // AxCxExGx
  /*p21.TUHU*/ DFF17 TUHU_PX4p; // AxCxExGx
  /*p21.TUKY*/ DFF17 TUKY_PX5p; // AxCxExGx
  /*p21.TAKO*/ DFF17 TAKO_PX6p; // AxCxExGx
  /*p21.SYBE*/ DFF17 SYBE_PX7p; // AxCxExGx
};

//-----------------------------------------------------------------------------
// FF41 - STAT

struct RegStat {
  /*p21.ROXE*/ DFF9 ROXE_STAT_HBI_ENn_h; // xxxxxxxH
  /*p21.RUFO*/ DFF9 RUFO_STAT_VBI_ENn_h; // xxxxxxxH
  /*p21.REFE*/ DFF9 REFE_STAT_OAI_ENn_h; // xxxxxxxH
  /*p21.RUGU*/ DFF9 RUGU_STAT_LYI_ENn_h; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF47 - BGP

struct RegBGP {
  /*p36.PAVO*/ DFF8p PAVO_BGP_D0n_h; // xxxxxxxH
  /*p36.NUSY*/ DFF8p NUSY_BGP_D1n_h; // xxxxxxxH
  /*p36.PYLU*/ DFF8p PYLU_BGP_D2n_h; // xxxxxxxH
  /*p36.MAXY*/ DFF8p MAXY_BGP_D3n_h; // xxxxxxxH
  /*p36.MUKE*/ DFF8p MUKE_BGP_D4n_h; // xxxxxxxH
  /*p36.MORU*/ DFF8p MORU_BGP_D5n_h; // xxxxxxxH
  /*p36.MOGY*/ DFF8p MOGY_BGP_D6n_h; // xxxxxxxH
  /*p36.MENA*/ DFF8p MENA_BGP_D7n_h; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF48 - OBP0

struct RegOBP0 {
  /*p36.XUFU*/ DFF8p XUFU_OBP0_D0n_h; // xxxxxxxH
  /*p36.XUKY*/ DFF8p XUKY_OBP0_D1n_h; // xxxxxxxH
  /*p36.XOVA*/ DFF8p XOVA_OBP0_D2n_h; // xxxxxxxH
  /*p36.XALO*/ DFF8p XALO_OBP0_D3n_h; // xxxxxxxH
  /*p36.XERU*/ DFF8p XERU_OBP0_D4n_h; // xxxxxxxH
  /*p36.XYZE*/ DFF8p XYZE_OBP0_D5n_h; // xxxxxxxH
  /*p36.XUPO*/ DFF8p XUPO_OBP0_D6n_h; // xxxxxxxH
  /*p36.XANA*/ DFF8p XANA_OBP0_D7n_h; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF49 - OBP1

struct RegOBP1 {
  /*p36.MOXY*/ DFF8p MOXY_OBP1_D0n_h; // xxxxxxxH
  /*p36.LAWO*/ DFF8p LAWO_OBP1_D1n_h; // xxxxxxxH
  /*p36.MOSA*/ DFF8p MOSA_OBP1_D2n_h; // xxxxxxxH
  /*p36.LOSE*/ DFF8p LOSE_OBP1_D3n_h; // xxxxxxxH
  /*p36.LUNE*/ DFF8p LUNE_OBP1_D4n_h; // xxxxxxxH
  /*p36.LUGU*/ DFF8p LUGU_OBP1_D5n_h; // xxxxxxxH
  /*p36.LEPU*/ DFF8p LEPU_OBP1_D6n_h; // xxxxxxxH
  /*p36.LUXO*/ DFF8p LUXO_OBP1_D7n_h; // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct WindowRegisters {

  void tock(wire XODO_VID_RSTp, wire ATAL_xBxDxFxH) {
    /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp);
    /* p27.SOVY*/ SOVY_WIN_HITp.dff17(ALET_xBxDxFxH(ATAL_xBxDxFxH), _XAPO_VID_RSTn_new_evn, RYDY_WIN_HITp_evn.qp_old());
  }

  wire NUNY_WIN_MODE_TRIGp() const {
    /*#p27.NUNY*/ wire _NUNY_WIN_MODE_TRIGp = and2(PYNU_WIN_MODE_Ap_evn.qp_any(), NOPA_WIN_MODE_Bp_odd.qn_any());
    return _NUNY_WIN_MODE_TRIGp;
  }
  /* p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn() const { return not1(NUNY_WIN_MODE_TRIGp()); }
  /* p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp() const { return not1(NYFO_WIN_MODE_TRIGn()); }


  wire NAFY_WIN_MODE_TRIGn(wire XYMU_RENDERINGp) const {
    /* p24.LOBY*/ wire _LOBY_RENDERINGn = not1(XYMU_RENDERINGp);
    /* p24.NAFY*/ wire _NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp(), _LOBY_RENDERINGn);
    return _NAFY_WIN_MODE_TRIGn;
  }

  wire SUZU_WIN_FIRST_TILEne() const {
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn = not1(RYDY_WIN_HITp_evn.qp_any());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne = nand2(_SYLO_WIN_HITn, SOVY_WIN_HITp.qp_any());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne = not1(_TUXY_WIN_FIRST_TILEne);
    return _SUZU_WIN_FIRST_TILEne;
  }

  wire SEKO_WIN_FETCH_TRIGp() const {
    /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp = nor2(RYFA_WIN_FETCHn_A_evn.qn_any(), RENE_WIN_FETCHn_B_odd.qp_any());
    return _SEKO_WIN_FETCH_TRIGp;
  }

  /*#p27.SYLO*/ wire SYLO_WIN_HITn() const { return not1(RYDY_WIN_HITp_evn.qp_any()); }
  /*#p24.TOMU*/ wire TOMU_WIN_HITp() const { return not1(SYLO_WIN_HITn()); }
  /* p27.TUKU*/ wire TUKU_WIN_HITn() const { return not1(TOMU_WIN_HITp()); }
  /*#p24.SOCY*/ wire SOCY_WIN_HITn() const { return not1(TOMU_WIN_HITp()); }

  /*#p27.NOCU*/ wire NOCU_WIN_MODEn() const { return not1(PYNU_WIN_MODE_Ap_evn.qp_new()); }
  /* p27.PORE*/ wire PORE_WIN_MODEp() const { return not1(NOCU_WIN_MODEn()); }
  /* p26.AXAD*/ wire AXAD_WIN_MODEn() const { return not1(PORE_WIN_MODEp()); }


  /*p27.PYNU*/ NorLatch PYNU_WIN_MODE_Ap_evn;            // AxxxxxGx
  /*p27.PUKU*/ Gate PUKU_WIN_HITn_evn;                   // xxCxxxGx
  /*p27.RYDY*/ Gate RYDY_WIN_HITp_evn;                   // xxCxxxGx
  /*p27.SOVY*/ DFF17 SOVY_WIN_HITp;                      // xxxDxxxH
  /*p27.NOPA*/ DFF17 NOPA_WIN_MODE_Bp_odd;               // xBxDxFxH
  /*p27.PYCO*/ DFF17 PYCO_WIN_MATCHp_odd;                // xxxDxxxH
  /*p27.NUNU*/ DFF17 NUNU_WIN_MATCHp_evn;                // xxxxExGx
  /*p27.REJO*/ NorLatch REJO_WY_MATCH_LATCHp_evn;        // xxCxxxxx
  /*p27.SARY*/ DFF17 SARY_WY_MATCHp_evn;                 // xxCxxxxx
  /*p27.RYFA*/ DFF17 RYFA_WIN_FETCHn_A_evn;              // AxCxExGx
  /*p27.RENE*/ DFF17 RENE_WIN_FETCHn_B_odd;              // xBxDxFxH
};

struct FineScroll {

  /*#p27.ROZE*/ wire ROZE_FINE_COUNT_7n_old() {
    wire _ROZE_FINE_COUNT_7n_old = nand3(RUBU_FINE_CNT2.qp_old(), ROGA_FINE_CNT1.qp_old(), RYKU_FINE_CNT0.qp_old());
    return _ROZE_FINE_COUNT_7n_old;
  }


  /*#p27.POVA*/ wire POVA_FINE_MATCH_TRIGp() {
    wire _POVA_FINE_MATCH_TRIGp = and2(PUXA_SCX_FINE_MATCH_A.qp_new(), NYZE_SCX_FINE_MATCH_B.qn_new());
    return _POVA_FINE_MATCH_TRIGp;
  }

  /*p27.RYKU*/ DFF17 RYKU_FINE_CNT0;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.
  /*p27.ROGA*/ DFF17 ROGA_FINE_CNT1;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.
  /*p27.RUBU*/ DFF17 RUBU_FINE_CNT2;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.

  /*p??.PUXA*/ DFF17 PUXA_SCX_FINE_MATCH_A;        // xxxxxFxH
  /*p27.NYZE*/ DFF17 NYZE_SCX_FINE_MATCH_B;        // AxxxxxGx
  /*p??.ROXY*/ NorLatch ROXY_FINE_SCROLL_DONEn;    // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct PPURegisters {
  wire XYMU_RENDERINGp() const { return XYMU_RENDERINGn.qn_any(); }

  /*p21.XYMU*/ NorLatch XYMU_RENDERINGn;             // ABxDxFxH Cleared on A, set on BDFH
  /*p21.RUPO*/ NorLatch RUPO_STAT_LYC_MATCHn_evn;       // xxCxxxxx
  /*p21.VOGA*/ DFF17 VOGA_HBLANKp_xxx;                   // ABxDxFxH Clocked on odd, reset on A
};

//-----------------------------------------------------------------------------

struct PixelPipes {

  /*p32.MYDE*/ DFF22 MYDE_BGW_PIPE_A0_evn; // AxCxExGx
  /*p32.NOZO*/ DFF22 NOZO_BGW_PIPE_A1_evn; // AxCxExGx
  /*p32.MOJU*/ DFF22 MOJU_BGW_PIPE_A2_evn; // AxCxExGx
  /*p32.MACU*/ DFF22 MACU_BGW_PIPE_A3_evn; // AxCxExGx
  /*p32.NEPO*/ DFF22 NEPO_BGW_PIPE_A4_evn; // AxCxExGx
  /*p32.MODU*/ DFF22 MODU_BGW_PIPE_A5_evn; // AxCxExGx
  /*p32.NEDA*/ DFF22 NEDA_BGW_PIPE_A6_evn; // AxCxExGx
  /*p32.PYBO*/ DFF22 PYBO_BGW_PIPE_A7_evn; // AxCxExGx

  /*p32.TOMY*/ DFF22 TOMY_BGW_PIPE_B0_evn; // AxCxExGx
  /*p32.TACA*/ DFF22 TACA_BGW_PIPE_B1_evn; // AxCxExGx
  /*p32.SADY*/ DFF22 SADY_BGW_PIPE_B2_evn; // AxCxExGx
  /*p32.RYSA*/ DFF22 RYSA_BGW_PIPE_B3_evn; // AxCxExGx
  /*p32.SOBO*/ DFF22 SOBO_BGW_PIPE_B4_evn; // AxCxExGx
  /*p32.SETU*/ DFF22 SETU_BGW_PIPE_B5_evn; // AxCxExGx
  /*p32.RALU*/ DFF22 RALU_BGW_PIPE_B6_evn; // AxCxExGx
  /*p32.SOHU*/ DFF22 SOHU_BGW_PIPE_B7_evn; // AxCxExGx

  /*p33.NURO*/ DFF22 NURO_SPR_PIPE_A0_evn; // AxCxExGx
  /*p33.MASO*/ DFF22 MASO_SPR_PIPE_A1_evn; // AxCxExGx
  /*p33.LEFE*/ DFF22 LEFE_SPR_PIPE_A2_evn; // AxCxExGx
  /*p33.LESU*/ DFF22 LESU_SPR_PIPE_A3_evn; // AxCxExGx
  /*p33.WYHO*/ DFF22 WYHO_SPR_PIPE_A4_evn; // AxCxExGx
  /*p33.WORA*/ DFF22 WORA_SPR_PIPE_A5_evn; // AxCxExGx
  /*p33.VAFO*/ DFF22 VAFO_SPR_PIPE_A6_evn; // AxCxExGx
  /*p33.WUFY*/ DFF22 WUFY_SPR_PIPE_A7_evn; // AxCxExGx

  /*p33.NYLU*/ DFF22 NYLU_SPR_PIPE_B0_evn; // AxCxExGx
  /*p33.PEFU*/ DFF22 PEFU_SPR_PIPE_B1_evn; // AxCxExGx
  /*p33.NATY*/ DFF22 NATY_SPR_PIPE_B2_evn; // AxCxExGx
  /*p33.PYJO*/ DFF22 PYJO_SPR_PIPE_B3_evn; // AxCxExGx
  /*p33.VARE*/ DFF22 VARE_SPR_PIPE_B4_evn; // AxCxExGx
  /*p33.WEBA*/ DFF22 WEBA_SPR_PIPE_B5_evn; // AxCxExGx
  /*p33.VANU*/ DFF22 VANU_SPR_PIPE_B6_evn; // AxCxExGx
  /*p33.VUPY*/ DFF22 VUPY_SPR_PIPE_B7_evn; // AxCxExGx

  /*p34.RUGO*/ DFF22 RUGO_PAL_PIPE_D0_evn;
  /*p34.SATA*/ DFF22 SATA_PAL_PIPE_D1_evn;
  /*p34.ROSA*/ DFF22 ROSA_PAL_PIPE_D2_evn;
  /*p34.SOMY*/ DFF22 SOMY_PAL_PIPE_D3_evn;
  /*p34.PALU*/ DFF22 PALU_PAL_PIPE_D4_evn;
  /*p34.NUKE*/ DFF22 NUKE_PAL_PIPE_D5_evn;
  /*p34.MODA*/ DFF22 MODA_PAL_PIPE_D6_evn;
  /*p34.LYME*/ DFF22 LYME_PAL_PIPE_D7_evn;

  /*p26.VEZO*/ DFF22 VEZO_MASK_PIPE_0_evn; // AxCxExGx
  /*p26.WURU*/ DFF22 WURU_MASK_PIPE_1_evn; // AxCxExGx
  /*p26.VOSA*/ DFF22 VOSA_MASK_PIPE_2_evn; // AxCxExGx
  /*p26.WYFU*/ DFF22 WYFU_MASK_PIPE_3_evn; // AxCxExGx
  /*p26.XETE*/ DFF22 XETE_MASK_PIPE_4_evn; // AxCxExGx
  /*p26.WODA*/ DFF22 WODA_MASK_PIPE_5_evn; // AxCxExGx
  /*p26.VUMO*/ DFF22 VUMO_MASK_PIPE_6_evn; // AxCxExGx
  /*p26.VAVA*/ DFF22 VAVA_MASK_PIPE_7_evn; // AxCxExGx
};

//-----------------------------------------------------------------------------
