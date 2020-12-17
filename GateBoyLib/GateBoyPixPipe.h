#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

struct SpriteFetcher;
struct TileTempA;
struct TileTempB;
struct SpriteTempA;
struct SpriteTempB;
struct OamTempB;

//-----------------------------------------------------------------------------

struct WindowRegisters {

  void tock(
    GateBoyResetDebug& rst,
    GateBoyClock& clk,
    wire XYMU_RENDERINGp,
    wire TYFA_CLKPIPE_odd,
    wire NUKO_WX_MATCHp_old,
    wire ROZE_FINE_COUNT_7n_old,
    wire RYDY_WIN_HITp_old);

  wire NUNY_WIN_MODE_TRIGp() const {
    /*#p27.NUNY*/ wire _NUNY_WIN_MODE_TRIGp = and2(PYNU_WIN_MODE_Ap.qp(), NOPA_WIN_MODE_Bp.qn());
    return _NUNY_WIN_MODE_TRIGp;
  }
  /* p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn() const { return not1(NUNY_WIN_MODE_TRIGp()); }
  /* p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp() const { return not1(NYFO_WIN_MODE_TRIGn()); }


  wire NAFY_WIN_MODE_TRIGn() const {
    /* p24.LOBY*/ wire _LOBY_RENDERINGn = not1(_XYMU_RENDERINGp.qp());
    /* p24.NAFY*/ wire _NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp(), _LOBY_RENDERINGn);
    return _NAFY_WIN_MODE_TRIGn;
  }

  wire SUZU_WIN_FIRST_TILEne() const {
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn = not1(RYDY_WIN_HITp.qp());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne = nand2(_SYLO_WIN_HITn, SOVY_WIN_HITp.qp());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne = not1(_TUXY_WIN_FIRST_TILEne);
    return _SUZU_WIN_FIRST_TILEne;
  }

  wire SEKO_WIN_FETCH_TRIGp() const {
    /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp = nor2(RYFA_WIN_FETCHn_A.qn(), RENE_WIN_FETCHn_B.qp());
    return _SEKO_WIN_FETCH_TRIGp;
  }

  /*#p27.SYLO*/ wire SYLO_WIN_HITn() const { return not1(RYDY_WIN_HITp.qp()); }
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
// FIXME this is kinda useless now

struct PPURegisters {
  void reset_app() {
    VOGA_HBLANKp.reset(REG_D1C0);
  }

  wire XYMU_RENDERINGp() const { return XYMU_RENDERINGn.qn(); }

  /*p21.XYMU*/ NorLatch XYMU_RENDERINGn;             // ABxDxFxH Cleared on A, set on BDFH
  /*p21.VOGA*/ DFF17 VOGA_HBLANKp;                   // ABxDxFxH Clocked on odd, reset on A
};

//-----------------------------------------------------------------------------

struct PixelPipes {

  void reset_app() {
    VEZO_MASK_PIPE_0.reset(REG_D1C1);
    WURU_MASK_PIPE_1.reset(REG_D1C1);
    VOSA_MASK_PIPE_2.reset(REG_D1C1);
    WYFU_MASK_PIPE_3.reset(REG_D1C1);
    XETE_MASK_PIPE_4.reset(REG_D1C1);
    WODA_MASK_PIPE_5.reset(REG_D1C1);
    VUMO_MASK_PIPE_6.reset(REG_D1C1);
    VAVA_MASK_PIPE_7.reset(REG_D1C1);
  }

  void tock_bgw_pipe(
    TileTempA& tile_temp_a,
    TileTempB& tile_temp_b,
    wire SACU_CLKPIPE_evn,
    wire NYXU_BFETCH_RSTn);

  void tock_sprite_pipe(
    SpriteFetcher& sprite_fetcher,
    SpriteTempA& sprite_temp_a,
    SpriteTempB& sprite_temp_b,
    wire SACU_CLKPIPE_evn);

  void tock_mask_pipe(
    SpriteFetcher& sprite_fetcher,
    OamTempB& oam_temp_b,
    wire SACU_CLKPIPE_evn);

  void tock_pal_pipe(
    SpriteFetcher& sprite_fetcher,
    OamTempB& oam_temp_b,
    wire SACU_CLKPIPE_evn);

  void tock_pix_output(RegLCDC& reg_lcdc);

  RegBGP  reg_bgp;
  RegOBP0 reg_obp0;
  RegOBP1 reg_obp1;

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
