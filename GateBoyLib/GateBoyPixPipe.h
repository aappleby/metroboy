#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

struct SpriteFetcher;
struct TileTempA;
struct TileTempB;
struct SpritePixA;
struct SpritePixB;
struct OamTempB;

//------------------------------------------------------------------------------------------------------------------------

struct WindowRegisters {

  void tock(
    GateBoyResetDebug& rst,
    GateBoyClock& clk,
    wire2 XYMU_RENDERINGp,
    wire2 TYFA_CLKPIPE_odd,
    wire2 NUKO_WX_MATCHp_old,
    wire2 ROZE_FINE_COUNT_7n_old,
    wire2 RYDY_WIN_HITp_old);

  wire2 NUNY_WIN_MODE_TRIGp_old() const {
    /*#p27.NUNY*/ wire2 _NUNY_WIN_MODE_TRIGp_old = and2(PYNU_WIN_MODE_Ap.qp_old(), NOPA_WIN_MODE_Bp.qn_old2());
    return _NUNY_WIN_MODE_TRIGp_old;
  }
  /* p27.NYFO*/ wire2 NYFO_WIN_MODE_TRIGn_old() const { return not1b(NUNY_WIN_MODE_TRIGp_old()); }
  /* p27.MOSU*/ wire2 MOSU_WIN_MODE_TRIGp_old() const { return not1b(NYFO_WIN_MODE_TRIGn_old()); }

  wire2 NUNY_WIN_MODE_TRIGp_new() const {
    /*#p27.NUNY*/ wire2 _NUNY_WIN_MODE_TRIGp_new = and2(PYNU_WIN_MODE_Ap.qp_new2(), NOPA_WIN_MODE_Bp.qn_new2());
    return _NUNY_WIN_MODE_TRIGp_new;
  }
  /* p27.NYFO*/ wire2 NYFO_WIN_MODE_TRIGn_new() const { return not1b(NUNY_WIN_MODE_TRIGp_new()); }
  /* p27.MOSU*/ wire2 MOSU_WIN_MODE_TRIGp_new() const { return not1b(NYFO_WIN_MODE_TRIGn_new()); }


  wire2 NAFY_WIN_MODE_TRIGn() const {
    /* p24.LOBY*/ wire2 _LOBY_RENDERINGn = not1b(_XYMU_RENDERINGp.qp_new2());
    /* p24.NAFY*/ wire2 _NAFY_WIN_MODE_TRIGn = nor2b(MOSU_WIN_MODE_TRIGp_new(), _LOBY_RENDERINGn);
    return _NAFY_WIN_MODE_TRIGn;
  }

  wire2 SUZU_WIN_FIRST_TILEne_old() const {
    /*#p27.SYLO*/ wire2 _SYLO_WIN_HITn_old = not1b(RYDY_WIN_HITp.qp_old());
    /* p27.TUXY*/ wire2 _TUXY_WIN_FIRST_TILEne_old = nand2b(_SYLO_WIN_HITn_old, SOVY_WIN_HITp.qp_old());
    /* p27.SUZU*/ wire2 _SUZU_WIN_FIRST_TILEne_old = not1b(_TUXY_WIN_FIRST_TILEne_old);
    return _SUZU_WIN_FIRST_TILEne_old;
  }

  wire2 SUZU_WIN_FIRST_TILEne_new() const {
    /*#p27.SYLO*/ wire2 _SYLO_WIN_HITn_new = not1b(RYDY_WIN_HITp.qp_new2());
    /* p27.TUXY*/ wire2 _TUXY_WIN_FIRST_TILEne_new = nand2b(_SYLO_WIN_HITn_new, SOVY_WIN_HITp.qp_new2());
    /* p27.SUZU*/ wire2 _SUZU_WIN_FIRST_TILEne_new = not1b(_TUXY_WIN_FIRST_TILEne_new);
    return _SUZU_WIN_FIRST_TILEne_new;
  }

  wire2 SEKO_WIN_FETCH_TRIGp_old() const {
    /* p27.SEKO*/ wire2 _SEKO_WIN_FETCH_TRIGp_old = nor2b(RYFA_WIN_FETCHn_A.qn_old2(), RENE_WIN_FETCHn_B.qp_old());
    return _SEKO_WIN_FETCH_TRIGp_old;
  }

  wire2 SEKO_WIN_FETCH_TRIGp_new() const {
    /* p27.SEKO*/ wire2 _SEKO_WIN_FETCH_TRIGp_new = nor2b(RYFA_WIN_FETCHn_A.qn_new2(), RENE_WIN_FETCHn_B.qp_new2());
    return _SEKO_WIN_FETCH_TRIGp_new;
  }

  /*#p27.SYLO*/ wire2 SYLO_WIN_HITn_old() const { return not1b(RYDY_WIN_HITp.qp_old()); }
  /*#p24.TOMU*/ wire2 TOMU_WIN_HITp_old() const { return not1b(SYLO_WIN_HITn_old()); }
  /* p27.TUKU*/ wire2 TUKU_WIN_HITn_old() const { return not1b(TOMU_WIN_HITp_old()); }
  /*#p24.SOCY*/ wire2 SOCY_WIN_HITn_old() const { return not1b(TOMU_WIN_HITp_old()); }

  /*#p27.SYLO*/ wire2 SYLO_WIN_HITn_new() const { return not1b(RYDY_WIN_HITp.qp_new2()); }
  /*#p24.TOMU*/ wire2 TOMU_WIN_HITp_new() const { return not1b(SYLO_WIN_HITn_new()); }
  /* p27.TUKU*/ wire2 TUKU_WIN_HITn_new() const { return not1b(TOMU_WIN_HITp_new()); }
  /*#p24.SOCY*/ wire2 SOCY_WIN_HITn_new() const { return not1b(TOMU_WIN_HITp_new()); }

  /*#p27.NOCU*/ wire2 NOCU_WIN_MODEn() const { return not1b(PYNU_WIN_MODE_Ap.qp_new2()); }
  /* p27.PORE*/ wire2 PORE_WIN_MODEp() const { return not1b(NOCU_WIN_MODEn()); }
  /* p26.AXAD*/ wire2 AXAD_WIN_MODEn() const { return not1b(PORE_WIN_MODEp()); }

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

//------------------------------------------------------------------------------------------------------------------------

struct FineScroll {

  void tock(wire2 XYMU_RENDERINGp, wire2 TYFA_CLKPIPE_odd, wire2 TEVO_FETCH_TRIGp) {
    // Fine match counter. Registers are only read as old, so this can go down as far in the list as needed.

    /*#p24.SEGU*/ wire2 _SEGU_CLKPIPE_evn = not1b(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire2 _ROXO_CLKPIPE_odd = not1b(_SEGU_CLKPIPE_evn);

    /*#p27.PAHA*/ wire2 _PAHA_RENDERINGn = not1b(XYMU_RENDERINGp);
    /*#p27.PASO*/ wire2 _PASO_FINE_RST = nor2b(_PAHA_RENDERINGn, TEVO_FETCH_TRIGp);

    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p27.ROZE*/ wire2 _ROZE_FINE_COUNT_7n = nand3b(RUBU_FINE_CNT2.qp_any2(), ROGA_FINE_CNT1.qp_any2(), RYKU_FINE_CNT0.qp_any2());
      /*#p27.PECU*/ wire2 _PECU_FINE_CLK = nand2b(_ROXO_CLKPIPE_odd, _ROZE_FINE_COUNT_7n);
      /*#p27.RYKU*/ RYKU_FINE_CNT0.dff17_any(_PECU_FINE_CLK,           _PASO_FINE_RST, RYKU_FINE_CNT0.qn_any2());
      /*#p27.ROGA*/ ROGA_FINE_CNT1.dff17_any(RYKU_FINE_CNT0.qn_any2(), _PASO_FINE_RST, ROGA_FINE_CNT1.qn_any2());
      /*#p27.RUBU*/ RUBU_FINE_CNT2.dff17_any(ROGA_FINE_CNT1.qn_any2(), _PASO_FINE_RST, RUBU_FINE_CNT2.qn_any2());
    }
  }

  /*#p27.ROZE*/ wire2 ROZE_FINE_COUNT_7n_old() {
    wire2 _ROZE_FINE_COUNT_7n_old = nand3b(RUBU_FINE_CNT2.qp_old(), ROGA_FINE_CNT1.qp_old(), RYKU_FINE_CNT0.qp_old());
    return _ROZE_FINE_COUNT_7n_old;
  }


  /*#p27.POVA*/ wire2 POVA_FINE_MATCH_TRIGp() {
    wire2 _POVA_FINE_MATCH_TRIGp = and2(PUXA_SCX_FINE_MATCH_A.qp_new2(), NYZE_SCX_FINE_MATCH_B.qn_new2());
    return _POVA_FINE_MATCH_TRIGp;
  }

  /*p27.RYKU*/ DFF17 RYKU_FINE_CNT0;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.
  /*p27.ROGA*/ DFF17 ROGA_FINE_CNT1;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.
  /*p27.RUBU*/ DFF17 RUBU_FINE_CNT2;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.

  /*p??.PUXA*/ DFF17 PUXA_SCX_FINE_MATCH_A;        // xxxxxFxH
  /*p27.NYZE*/ DFF17 NYZE_SCX_FINE_MATCH_B;        // AxxxxxGx
  /*p??.ROXY*/ NorLatch ROXY_FINE_SCROLL_DONEn;    // xBxDxFxH
};

//------------------------------------------------------------------------------------------------------------------------
// FIXME this is kinda useless now

struct PPURegisters {
  void reset_to_cart() {
    VOGA_HBLANKp.reset(0, 1);
  }

  wire2 XYMU_RENDERINGp() const { return XYMU_RENDERINGn.qn_new2(); }

  /*p21.XYMU*/ NorLatch XYMU_RENDERINGn;             // ABxDxFxH Cleared on A, set on BDFH
  /*p21.VOGA*/ DFF17 VOGA_HBLANKp;                   // ABxDxFxH Clocked on odd, reset on A
};

//------------------------------------------------------------------------------------------------------------------------

struct PixelPipes {

  void reset_to_cart() {
    VEZO_MASK_PIPE_0.reset(1, 1);
    WURU_MASK_PIPE_1.reset(1, 1);
    VOSA_MASK_PIPE_2.reset(1, 1);
    WYFU_MASK_PIPE_3.reset(1, 1);
    XETE_MASK_PIPE_4.reset(1, 1);
    WODA_MASK_PIPE_5.reset(1, 1);
    VUMO_MASK_PIPE_6.reset(1, 1);
    VAVA_MASK_PIPE_7.reset(1, 1);
    REMY_LD0n.reset(1);
    RAVO_LD1n.reset(1);
  }

  void tock_bgw_pipe(
    TileTempA& tile_temp_a,
    TileTempB& tile_temp_b,
    wire2 SACU_CLKPIPE_evn,
    wire2 NYXU_BFETCH_RSTn);

  void tock_sprite_pipe(
    SpritePixA& sprite_pix_a,
    SpritePixB& sprite_pix_b,
    wire2 WUTY_SFETCH_DONE_TRIGp,
    wire2 SACU_CLKPIPE_evn);

  void tock_mask_pipe(
    OamTempB& oam_temp_b,
    wire2 WUTY_SFETCH_DONE_TRIGp,
    wire2 SACU_CLKPIPE_evn);

  void tock_pal_pipe(
    OamTempB& oam_temp_b,
    wire2 WUTY_SFETCH_DONE_TRIGp,
    wire2 SACU_CLKPIPE_evn);

  void tock_pix_output(
    RegLCDC& reg_lcdc,
    RegBGP  _reg_bgp,
    RegOBP0 _reg_obp0,
    RegOBP1 _reg_obp1);

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
