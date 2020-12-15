#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------
// FF4A - WY

struct RegWY {
  void set(uint8_t wy) {
    NESO_WY0n.reset((wy & 0x01) ? REG_D0C1 : REG_D1C1);
    NYRO_WY1n.reset((wy & 0x02) ? REG_D0C1 : REG_D1C1);
    NAGA_WY2n.reset((wy & 0x04) ? REG_D0C1 : REG_D1C1);
    MELA_WY3n.reset((wy & 0x08) ? REG_D0C1 : REG_D1C1);
    NULO_WY4n.reset((wy & 0x10) ? REG_D0C1 : REG_D1C1);
    NENE_WY5n.reset((wy & 0x20) ? REG_D0C1 : REG_D1C1);
    NUKA_WY6n.reset((wy & 0x40) ? REG_D0C1 : REG_D1C1);
    NAFU_WY7n.reset((wy & 0x80) ? REG_D0C1 : REG_D1C1);
  }

  int get() const { return pack_u8n(8, &NESO_WY0n); }

  void tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  /*p23.NESO*/ DFF9 NESO_WY0n; // xxxxxxxH
  /*p23.NYRO*/ DFF9 NYRO_WY1n; // xxxxxxxH
  /*p23.NAGA*/ DFF9 NAGA_WY2n; // xxxxxxxH
  /*p23.MELA*/ DFF9 MELA_WY3n; // xxxxxxxH
  /*p23.NULO*/ DFF9 NULO_WY4n; // xxxxxxxH
  /*p23.NENE*/ DFF9 NENE_WY5n; // xxxxxxxH
  /*p23.NUKA*/ DFF9 NUKA_WY6n; // xxxxxxxH
  /*p23.NAFU*/ DFF9 NAFU_WY7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF4B - WX

struct RegWX {
  void set(uint8_t wx) {
    MYPA_WX0n.reset((wx & 0x01) ? REG_D0C1 : REG_D1C1);
    NOFE_WX1n.reset((wx & 0x02) ? REG_D0C1 : REG_D1C1);
    NOKE_WX2n.reset((wx & 0x04) ? REG_D0C1 : REG_D1C1);
    MEBY_WX3n.reset((wx & 0x08) ? REG_D0C1 : REG_D1C1);
    MYPU_WX4n.reset((wx & 0x10) ? REG_D0C1 : REG_D1C1);
    MYCE_WX5n.reset((wx & 0x20) ? REG_D0C1 : REG_D1C1);
    MUVO_WX6n.reset((wx & 0x40) ? REG_D0C1 : REG_D1C1);
    NUKU_WX7n.reset((wx & 0x80) ? REG_D0C1 : REG_D1C1);
  }

  int get() const { return pack_u8n(8, &MYPA_WX0n); }

  void tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  /*p23.MYPA*/ DFF9 MYPA_WX0n; // xxxxxxxH
  /*p23.NOFE*/ DFF9 NOFE_WX1n; // xxxxxxxH
  /*p23.NOKE*/ DFF9 NOKE_WX2n; // xxxxxxxH
  /*p23.MEBY*/ DFF9 MEBY_WX3n; // xxxxxxxH
  /*p23.MYPU*/ DFF9 MYPU_WX4n; // xxxxxxxH
  /*p23.MYCE*/ DFF9 MYCE_WX5n; // xxxxxxxH
  /*p23.MUVO*/ DFF9 MUVO_WX6n; // xxxxxxxH
  /*p23.NUKU*/ DFF9 NUKU_WX7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// Pixel counter

struct PixCounter {

  void tock(wire TADY_LINE_RSTn, wire SACU_CLKPIPE_evn) {
    // Pixel counter, has carry lookahead because this can increment every tcycle
    /* p21.RYBO*/ wire RYBO_old = xor2(XEHO_PX0p.qp_old(), SAVY_PX1p.qp_old()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /* p21.XUKE*/ wire XUKE_old = and2(XEHO_PX0p.qp_old(), SAVY_PX1p.qp_old());
    /* p21.XYLE*/ wire XYLE_old = and2(XODU_PX2p.qp_old(), XUKE_old);
    /* p21.XEGY*/ wire XEGY_old = xor2(XODU_PX2p.qp_old(), XUKE_old); // feet facing gnd
    /* p21.XORA*/ wire XORA_old = xor2(XYDO_PX3p.qp_old(), XYLE_old); // feet facing gnd

    /* p21.XEHO*/ XEHO_PX0p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEHO_PX0p.qn_old());
    /* p21.SAVY*/ SAVY_PX1p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, RYBO_old);
    /* p21.XODU*/ XODU_PX2p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEGY_old);
    /* p21.XYDO*/ XYDO_PX3p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XORA_old);

    /* p24.TOCA*/ wire TOCA_new = not1(XYDO_PX3p.qp_new());
    /* p21.SAKE*/ wire SAKE_old = xor2(TUHU_PX4p.qp_old(), TUKY_PX5p.qp_old());
    /* p21.TYBA*/ wire TYBA_old = and2(TUHU_PX4p.qp_old(), TUKY_PX5p.qp_old());
    /* p21.SURY*/ wire SURY_old = and2(TAKO_PX6p.qp_old(), TYBA_old);
    /* p21.TYGE*/ wire TYGE_old = xor2(TAKO_PX6p.qp_old(), TYBA_old);
    /* p21.ROKU*/ wire ROKU_old = xor2(SYBE_PX7p.qp_old(), SURY_old);

    /* p21.TUHU*/ TUHU_PX4p.dff17(TOCA_new, TADY_LINE_RSTn, TUHU_PX4p.qn_any());
    /* p21.TUKY*/ TUKY_PX5p.dff17(TOCA_new, TADY_LINE_RSTn, SAKE_old);
    /* p21.TAKO*/ TAKO_PX6p.dff17(TOCA_new, TADY_LINE_RSTn, TYGE_old);
    /* p21.SYBE*/ SYBE_PX7p.dff17(TOCA_new, TADY_LINE_RSTn, ROKU_old);
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

  void tock(
    GateBoyResetDebug& rst,
    GateBoyCpuBus& cpu_bus,
    wire ACYL_SCANNINGp,
    wire XYMU_RENDERINGp,
    wire PARU_VBLANKp,
    const RegLYC& reg_lyc);

  /*p21.RUPO*/ NorLatch RUPO_STAT_LYC_MATCHn;       // xxCxxxxx

  /*p21.ROXE*/ DFF9 ROXE_STAT_HBI_ENn; // xxxxxxxH
  /*p21.RUFO*/ DFF9 RUFO_STAT_VBI_ENn; // xxxxxxxH
  /*p21.REFE*/ DFF9 REFE_STAT_OAI_ENn; // xxxxxxxH
  /*p21.RUGU*/ DFF9 RUGU_STAT_LYI_ENn; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF47 - BGP

struct RegBGP {
  /*p36.PAVO*/ DFF8p PAVO_BGP_D0n; // xxxxxxxH
  /*p36.NUSY*/ DFF8p NUSY_BGP_D1n; // xxxxxxxH
  /*p36.PYLU*/ DFF8p PYLU_BGP_D2n; // xxxxxxxH
  /*p36.MAXY*/ DFF8p MAXY_BGP_D3n; // xxxxxxxH
  /*p36.MUKE*/ DFF8p MUKE_BGP_D4n; // xxxxxxxH
  /*p36.MORU*/ DFF8p MORU_BGP_D5n; // xxxxxxxH
  /*p36.MOGY*/ DFF8p MOGY_BGP_D6n; // xxxxxxxH
  /*p36.MENA*/ DFF8p MENA_BGP_D7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF48 - OBP0

struct RegOBP0 {
  /*p36.XUFU*/ DFF8p XUFU_OBP0_D0n; // xxxxxxxH
  /*p36.XUKY*/ DFF8p XUKY_OBP0_D1n; // xxxxxxxH
  /*p36.XOVA*/ DFF8p XOVA_OBP0_D2n; // xxxxxxxH
  /*p36.XALO*/ DFF8p XALO_OBP0_D3n; // xxxxxxxH
  /*p36.XERU*/ DFF8p XERU_OBP0_D4n; // xxxxxxxH
  /*p36.XYZE*/ DFF8p XYZE_OBP0_D5n; // xxxxxxxH
  /*p36.XUPO*/ DFF8p XUPO_OBP0_D6n; // xxxxxxxH
  /*p36.XANA*/ DFF8p XANA_OBP0_D7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------
// FF49 - OBP1

struct RegOBP1 {
  /*p36.MOXY*/ DFF8p MOXY_OBP1_D0n; // xxxxxxxH
  /*p36.LAWO*/ DFF8p LAWO_OBP1_D1n; // xxxxxxxH
  /*p36.MOSA*/ DFF8p MOSA_OBP1_D2n; // xxxxxxxH
  /*p36.LOSE*/ DFF8p LOSE_OBP1_D3n; // xxxxxxxH
  /*p36.LUNE*/ DFF8p LUNE_OBP1_D4n; // xxxxxxxH
  /*p36.LUGU*/ DFF8p LUGU_OBP1_D5n; // xxxxxxxH
  /*p36.LEPU*/ DFF8p LEPU_OBP1_D6n; // xxxxxxxH
  /*p36.LUXO*/ DFF8p LUXO_OBP1_D7n; // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct WindowRegisters {

  void tock(
    GateBoyResetDebug& rst,
    GateBoyClock& clk,
    wire TYFA_CLKPIPE_odd,
    wire NUKO_WX_MATCHp,
    wire XYMU_RENDERINGp,
    wire ROZE_FINE_COUNT_7n);

  wire NUNY_WIN_MODE_TRIGp() const {
    /*#p27.NUNY*/ wire _NUNY_WIN_MODE_TRIGp = and2(PYNU_WIN_MODE_Ap.qp_any(), NOPA_WIN_MODE_Bp.qn_any());
    return _NUNY_WIN_MODE_TRIGp;
  }
  /* p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn() const { return not1(NUNY_WIN_MODE_TRIGp()); }
  /* p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp() const { return not1(NYFO_WIN_MODE_TRIGn()); }


  wire NAFY_WIN_MODE_TRIGn() const {
    /* p24.LOBY*/ wire _LOBY_RENDERINGn = not1(_XYMU_RENDERINGp);
    /* p24.NAFY*/ wire _NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp(), _LOBY_RENDERINGn);
    return _NAFY_WIN_MODE_TRIGn;
  }

  wire SUZU_WIN_FIRST_TILEne() const {
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn = not1(RYDY_WIN_HITp.qp_any());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne = nand2(_SYLO_WIN_HITn, SOVY_WIN_HITp.qp_any());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne = not1(_TUXY_WIN_FIRST_TILEne);
    return _SUZU_WIN_FIRST_TILEne;
  }

  wire SEKO_WIN_FETCH_TRIGp() const {
    /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp = nor2(RYFA_WIN_FETCHn_A.qn_any(), RENE_WIN_FETCHn_B.qp_any());
    return _SEKO_WIN_FETCH_TRIGp;
  }

  /*#p27.SYLO*/ wire SYLO_WIN_HITn() const { return not1(RYDY_WIN_HITp.qp_any()); }
  /*#p24.TOMU*/ wire TOMU_WIN_HITp() const { return not1(SYLO_WIN_HITn()); }
  /* p27.TUKU*/ wire TUKU_WIN_HITn() const { return not1(TOMU_WIN_HITp()); }
  /*#p24.SOCY*/ wire SOCY_WIN_HITn() const { return not1(TOMU_WIN_HITp()); }

  /*#p27.NOCU*/ wire NOCU_WIN_MODEn() const { return not1(PYNU_WIN_MODE_Ap.qp_new()); }
  /* p27.PORE*/ wire PORE_WIN_MODEp() const { return not1(NOCU_WIN_MODEn()); }
  /* p26.AXAD*/ wire AXAD_WIN_MODEn() const { return not1(PORE_WIN_MODEp()); }

  Signal _XYMU_RENDERINGp;

  /*p27.PYNU*/ NorLatch PYNU_WIN_MODE_Ap;      // AxxxxxGx
  /*p27.PUKU*/ Gate PUKU_WIN_HITn;             // xxCxxxGx
  /*p27.RYDY*/ Gate RYDY_WIN_HITp;             // xxCxxxGx
  /*p27.SOVY*/ DFF17 SOVY_WIN_HITp;            // xxxDxxxH
  /*p27.NOPA*/ DFF17 NOPA_WIN_MODE_Bp;         // xBxDxFxH
  /*p27.PYCO*/ DFF17 PYCO_WIN_MATCHp;          // xxxDxxxH
  /*p27.NUNU*/ DFF17 NUNU_WIN_MATCHp;          // xxxxExGx
  /*p27.REJO*/ NorLatch REJO_WY_MATCH_LATCHp;  // xxCxxxxx
  /*p27.SARY*/ DFF17 SARY_WY_MATCHp;           // xxCxxxxx
  /*p27.RYFA*/ DFF17 RYFA_WIN_FETCHn_A;        // AxCxExGx
  /*p27.RENE*/ DFF17 RENE_WIN_FETCHn_B;        // xBxDxFxH
};

struct FineScroll {

  void tock(wire XYMU_RENDERINGp, wire TYFA_CLKPIPE_odd, wire TEVO_FETCH_TRIGp) {
    // Fine match counter. Registers are only read as old, so this can go down as far in the list as needed.

    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire _ROXO_CLKPIPE_odd = not1(_SEGU_CLKPIPE_evn);

    /*#p27.PAHA*/ wire _PAHA_RENDERINGn = not1(XYMU_RENDERINGp);
    /*#p27.PASO*/ wire _PASO_FINE_RST = nor2(_PAHA_RENDERINGn, TEVO_FETCH_TRIGp);
    /*#p27.RYKU*/ RYKU_FINE_CNT0.RSTn(_PASO_FINE_RST);
    /*#p27.ROGA*/ ROGA_FINE_CNT1.RSTn(_PASO_FINE_RST);
    /*#p27.RUBU*/ RUBU_FINE_CNT2.RSTn(_PASO_FINE_RST);

    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n = nand3(RUBU_FINE_CNT2.qp_new(), ROGA_FINE_CNT1.qp_new(), RYKU_FINE_CNT0.qp_new());
    /*#p27.PECU*/ wire _PECU_FINE_CLK = nand2(_ROXO_CLKPIPE_odd, _ROZE_FINE_COUNT_7n);
    /*#p27.RYKU*/ RYKU_FINE_CNT0.dff17(_PECU_FINE_CLK,          _PASO_FINE_RST, RYKU_FINE_CNT0.qn_new());
    /*#p27.ROGA*/ ROGA_FINE_CNT1.dff17(RYKU_FINE_CNT0.qn_new(), _PASO_FINE_RST, ROGA_FINE_CNT1.qn_new());
    /*#p27.RUBU*/ RUBU_FINE_CNT2.dff17(ROGA_FINE_CNT1.qn_new(), _PASO_FINE_RST, RUBU_FINE_CNT2.qn_new());
  }

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
  /*p21.VOGA*/ DFF17 VOGA_HBLANKp;                   // ABxDxFxH Clocked on odd, reset on A
};

//-----------------------------------------------------------------------------

struct PixelPipes {

  /*p32.MYDE*/ DFF22 MYDE_BGW_PIPE_A0; // AxCxExGx
  /*p32.NOZO*/ DFF22 NOZO_BGW_PIPE_A1; // AxCxExGx
  /*p32.MOJU*/ DFF22 MOJU_BGW_PIPE_A2; // AxCxExGx
  /*p32.MACU*/ DFF22 MACU_BGW_PIPE_A3; // AxCxExGx
  /*p32.NEPO*/ DFF22 NEPO_BGW_PIPE_A4; // AxCxExGx
  /*p32.MODU*/ DFF22 MODU_BGW_PIPE_A5; // AxCxExGx
  /*p32.NEDA*/ DFF22 NEDA_BGW_PIPE_A6; // AxCxExGx
  /*p32.PYBO*/ DFF22 PYBO_BGW_PIPE_A7; // AxCxExGx

  /*p32.TOMY*/ DFF22 TOMY_BGW_PIPE_B0; // AxCxExGx
  /*p32.TACA*/ DFF22 TACA_BGW_PIPE_B1; // AxCxExGx
  /*p32.SADY*/ DFF22 SADY_BGW_PIPE_B2; // AxCxExGx
  /*p32.RYSA*/ DFF22 RYSA_BGW_PIPE_B3; // AxCxExGx
  /*p32.SOBO*/ DFF22 SOBO_BGW_PIPE_B4; // AxCxExGx
  /*p32.SETU*/ DFF22 SETU_BGW_PIPE_B5; // AxCxExGx
  /*p32.RALU*/ DFF22 RALU_BGW_PIPE_B6; // AxCxExGx
  /*p32.SOHU*/ DFF22 SOHU_BGW_PIPE_B7; // AxCxExGx

  /*p33.NURO*/ DFF22 NURO_SPR_PIPE_A0; // AxCxExGx
  /*p33.MASO*/ DFF22 MASO_SPR_PIPE_A1; // AxCxExGx
  /*p33.LEFE*/ DFF22 LEFE_SPR_PIPE_A2; // AxCxExGx
  /*p33.LESU*/ DFF22 LESU_SPR_PIPE_A3; // AxCxExGx
  /*p33.WYHO*/ DFF22 WYHO_SPR_PIPE_A4; // AxCxExGx
  /*p33.WORA*/ DFF22 WORA_SPR_PIPE_A5; // AxCxExGx
  /*p33.VAFO*/ DFF22 VAFO_SPR_PIPE_A6; // AxCxExGx
  /*p33.WUFY*/ DFF22 WUFY_SPR_PIPE_A7; // AxCxExGx

  /*p33.NYLU*/ DFF22 NYLU_SPR_PIPE_B0; // AxCxExGx
  /*p33.PEFU*/ DFF22 PEFU_SPR_PIPE_B1; // AxCxExGx
  /*p33.NATY*/ DFF22 NATY_SPR_PIPE_B2; // AxCxExGx
  /*p33.PYJO*/ DFF22 PYJO_SPR_PIPE_B3; // AxCxExGx
  /*p33.VARE*/ DFF22 VARE_SPR_PIPE_B4; // AxCxExGx
  /*p33.WEBA*/ DFF22 WEBA_SPR_PIPE_B5; // AxCxExGx
  /*p33.VANU*/ DFF22 VANU_SPR_PIPE_B6; // AxCxExGx
  /*p33.VUPY*/ DFF22 VUPY_SPR_PIPE_B7; // AxCxExGx

  /*p34.RUGO*/ DFF22 RUGO_PAL_PIPE_D0;
  /*p34.SATA*/ DFF22 SATA_PAL_PIPE_D1;
  /*p34.ROSA*/ DFF22 ROSA_PAL_PIPE_D2;
  /*p34.SOMY*/ DFF22 SOMY_PAL_PIPE_D3;
  /*p34.PALU*/ DFF22 PALU_PAL_PIPE_D4;
  /*p34.NUKE*/ DFF22 NUKE_PAL_PIPE_D5;
  /*p34.MODA*/ DFF22 MODA_PAL_PIPE_D6;
  /*p34.LYME*/ DFF22 LYME_PAL_PIPE_D7;

  /*p26.VEZO*/ DFF22 VEZO_MASK_PIPE_0; // AxCxExGx
  /*p26.WURU*/ DFF22 WURU_MASK_PIPE_1; // AxCxExGx
  /*p26.VOSA*/ DFF22 VOSA_MASK_PIPE_2; // AxCxExGx
  /*p26.WYFU*/ DFF22 WYFU_MASK_PIPE_3; // AxCxExGx
  /*p26.XETE*/ DFF22 XETE_MASK_PIPE_4; // AxCxExGx
  /*p26.WODA*/ DFF22 WODA_MASK_PIPE_5; // AxCxExGx
  /*p26.VUMO*/ DFF22 VUMO_MASK_PIPE_6; // AxCxExGx
  /*p26.VAVA*/ DFF22 VAVA_MASK_PIPE_7; // AxCxExGx

  /*#p35.REMY*/ Signal REMY_LD0n;
  /*#p35.RAVO*/ Signal RAVO_LD1n;
};

//-----------------------------------------------------------------------------
