#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------
// FF04 DIV

struct DivRegister {
  int get_div() const  { return pack_u16p(16, &UKUP_DIV00p_evn); }

  void force_set_div(uint16_t div) {
    uint16_t div_a = div;
    uint16_t div_b = ((~div) << 2);

    UKUP_DIV00p_evn.reset(((div_a >>  0) & 1) | ((div_b >>  0) & 2));
    UFOR_DIV01p_evn.reset(((div_a >>  1) & 1) | ((div_b >>  1) & 2));
    UNER_DIV02p_evn.reset(((div_a >>  2) & 1) | ((div_b >>  2) & 2));
    TERO_DIV03p_evn.reset(((div_a >>  3) & 1) | ((div_b >>  3) & 2));
    UNYK_DIV04p_evn.reset(((div_a >>  4) & 1) | ((div_b >>  4) & 2));
    TAMA_DIV05p_evn.reset(((div_a >>  5) & 1) | ((div_b >>  5) & 2));
    UGOT_DIV06p_evn.reset(((div_a >>  6) & 1) | ((div_b >>  6) & 2));
    TULU_DIV07p_evn.reset(((div_a >>  7) & 1) | ((div_b >>  7) & 2));
    TUGO_DIV08p_evn.reset(((div_a >>  8) & 1) | ((div_b >>  8) & 2));
    TOFE_DIV09p_evn.reset(((div_a >>  9) & 1) | ((div_b >>  9) & 2));
    TERU_DIV10p_evn.reset(((div_a >> 10) & 1) | ((div_b >> 10) & 2));
    SOLA_DIV11p_evn.reset(((div_a >> 11) & 1) | ((div_b >> 11) & 2));
    SUBU_DIV12p_evn.reset(((div_a >> 12) & 1) | ((div_b >> 12) & 2));
    TEKA_DIV13p_evn.reset(((div_a >> 13) & 1) | ((div_b >> 13) & 2));
    UKET_DIV14p_evn.reset(((div_a >> 14) & 1) | ((div_b >> 14) & 2));
    UPOF_DIV15p_evn.reset(((div_a >> 15) & 1) | ((div_b >> 15) & 2));
  }

  void tock(wire sys_rst,
            wire BUS_CPU_A[16],
            wire UCOB_CLKBADp_ext,
            wire BOGA_Axxxxxxx_clkevn,
            wire TEDO_CPU_RDp,
            wire TAPU_CPU_WRp,
            BusOut BUS_CPU_D_out[8])
  {

    /* p01.TAGY*/ wire _TAGY_FF04_RDp_ext    = and4(TEDO_CPU_RDp,    RYFO_FF04_FF07p_ext(BUS_CPU_A), TOLA_A01n_ext(BUS_CPU_A), TOVY_A00n_ext(BUS_CPU_A));
    /* p01.TAPE*/ wire _TAPE_FF04_WRp_clkevn = and4(TAPU_CPU_WRp, RYFO_FF04_FF07p_ext(BUS_CPU_A), TOLA_A01n_ext(BUS_CPU_A), TOVY_A00n_ext(BUS_CPU_A));

    /* p01.UFOL*/ wire _UFOL_DIV_RSTn_evn_ext = nor3(UCOB_CLKBADp_ext, sys_rst, _TAPE_FF04_WRp_clkevn);

    /* p01.UKUP*/ UKUP_DIV00p_evn.dff17(BOGA_Axxxxxxx_clkevn,     _UFOL_DIV_RSTn_evn_ext, UKUP_DIV00p_evn.qn_any());
    /* p01.UFOR*/ UFOR_DIV01p_evn.dff17(UKUP_DIV00p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, UFOR_DIV01p_evn.qn_any());
    /* p01.UNER*/ UNER_DIV02p_evn.dff17(UFOR_DIV01p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, UNER_DIV02p_evn.qn_any());
    /*#p01.TERO*/ TERO_DIV03p_evn.dff17(UNER_DIV02p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, TERO_DIV03p_evn.qn_any());
    /* p01.UNYK*/ UNYK_DIV04p_evn.dff17(TERO_DIV03p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, UNYK_DIV04p_evn.qn_any());
    /* p01.TAMA*/ TAMA_DIV05p_evn.dff17(UNYK_DIV04p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, TAMA_DIV05p_evn.qn_any());

    // this is hacked up because we're ignoring the debug reg for the moment
    /* p01.ULUR*/ wire _ULUR_DIV_06_clknew = /*mux2p(FF60_1, _BOGA_Axxxxxxx,*/ TAMA_DIV05p_evn.qn_new() /*)*/;

    /* p01.UGOT*/ UGOT_DIV06p_evn.dff17(_ULUR_DIV_06_clknew,      _UFOL_DIV_RSTn_evn_ext, UGOT_DIV06p_evn.qn_any());
    /* p01.TULU*/ TULU_DIV07p_evn.dff17(UGOT_DIV06p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, TULU_DIV07p_evn.qn_any());
    /* p01.TUGO*/ TUGO_DIV08p_evn.dff17(TULU_DIV07p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, TUGO_DIV08p_evn.qn_any());
    /* p01.TOFE*/ TOFE_DIV09p_evn.dff17(TUGO_DIV08p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, TOFE_DIV09p_evn.qn_any());
    /* p01.TERU*/ TERU_DIV10p_evn.dff17(TOFE_DIV09p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, TERU_DIV10p_evn.qn_any());
    /* p01.SOLA*/ SOLA_DIV11p_evn.dff17(TERU_DIV10p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, SOLA_DIV11p_evn.qn_any());
    /* p01.SUBU*/ SUBU_DIV12p_evn.dff17(SOLA_DIV11p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, SUBU_DIV12p_evn.qn_any());
    /* p01.TEKA*/ TEKA_DIV13p_evn.dff17(SUBU_DIV12p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, TEKA_DIV13p_evn.qn_any());
    /* p01.UKET*/ UKET_DIV14p_evn.dff17(TEKA_DIV13p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, UKET_DIV14p_evn.qn_any());
    /* p01.UPOF*/ UPOF_DIV15p_evn.dff17(UKET_DIV14p_evn.qn_new(), _UFOL_DIV_RSTn_evn_ext, UPOF_DIV15p_evn.qn_any());

    /* FF04 DIV */
    /* p01.UMEK*/ wire _UMEK_DIV06n_evn = not1(UGOT_DIV06p_evn.qp_new());
    /* p01.UREK*/ wire _UREK_DIV07n_evn = not1(TULU_DIV07p_evn.qp_new());
    /* p01.UTOK*/ wire _UTOK_DIV08n_evn = not1(TUGO_DIV08p_evn.qp_new());
    /* p01.SAPY*/ wire _SAPY_DIV09n_evn = not1(TOFE_DIV09p_evn.qp_new());
    /* p01.UMER*/ wire _UMER_DIV10n_evn = not1(TERU_DIV10p_evn.qp_new());
    /* p01.RAVE*/ wire _RAVE_DIV11n_evn = not1(SOLA_DIV11p_evn.qp_new());
    /* p01.RYSO*/ wire _RYSO_DIV12n_evn = not1(SUBU_DIV12p_evn.qp_new());
    /* p01.UDOR*/ wire _UDOR_DIV13n_evn = not1(TEKA_DIV13p_evn.qp_new());

    /* p01.TAWU*/ BUS_CPU_D_out[0].tri6_pn(_TAGY_FF04_RDp_ext, _UMEK_DIV06n_evn);
    /* p01.TAKU*/ BUS_CPU_D_out[1].tri6_pn(_TAGY_FF04_RDp_ext, _UREK_DIV07n_evn);
    /* p01.TEMU*/ BUS_CPU_D_out[2].tri6_pn(_TAGY_FF04_RDp_ext, _UTOK_DIV08n_evn);
    /* p01.TUSE*/ BUS_CPU_D_out[3].tri6_pn(_TAGY_FF04_RDp_ext, _SAPY_DIV09n_evn);
    /* p01.UPUG*/ BUS_CPU_D_out[4].tri6_pn(_TAGY_FF04_RDp_ext, _UMER_DIV10n_evn); // Schematic wrong, UPUG/SEPU driving D5/D4
    /* p01.SEPU*/ BUS_CPU_D_out[5].tri6_pn(_TAGY_FF04_RDp_ext, _RAVE_DIV11n_evn);
    /* p01.SAWA*/ BUS_CPU_D_out[6].tri6_pn(_TAGY_FF04_RDp_ext, _RYSO_DIV12n_evn);
    /* p01.TATU*/ BUS_CPU_D_out[7].tri6_pn(_TAGY_FF04_RDp_ext, _UDOR_DIV13n_evn);
  }

  /*p01.UKUP*/ DFF17 UKUP_DIV00p_evn; // AxxxExxx
  /*p01.UFOR*/ DFF17 UFOR_DIV01p_evn; // AxxxExxx
  /*p01.UNER*/ DFF17 UNER_DIV02p_evn; // AxxxExxx
  /*p01.TERO*/ DFF17 TERO_DIV03p_evn; // AxxxExxx
  /*p01.UNYK*/ DFF17 UNYK_DIV04p_evn; // AxxxExxx
  /*p01.TAMA*/ DFF17 TAMA_DIV05p_evn; // AxxxExxx
  /*p01.UGOT*/ DFF17 UGOT_DIV06p_evn; // AxxxExxx
  /*p01.TULU*/ DFF17 TULU_DIV07p_evn; // AxxxExxx
  /*p01.TUGO*/ DFF17 TUGO_DIV08p_evn; // AxxxExxx
  /*p01.TOFE*/ DFF17 TOFE_DIV09p_evn; // AxxxExxx
  /*p01.TERU*/ DFF17 TERU_DIV10p_evn; // AxxxExxx
  /*p01.SOLA*/ DFF17 SOLA_DIV11p_evn; // AxxxExxx
  /*p01.SUBU*/ DFF17 SUBU_DIV12p_evn; // AxxxExxx
  /*p01.TEKA*/ DFF17 TEKA_DIV13p_evn; // AxxxExxx
  /*p01.UKET*/ DFF17 UKET_DIV14p_evn; // AxxxExxx
  /*p01.UPOF*/ DFF17 UPOF_DIV15p_evn; // AxxxExxx
};

//-----------------------------------------------------------------------------

struct TimerRegisters {
  int get_tima() const { return pack_u8p (8,  &REGA_TIMA0p_evn); }
  int get_tma() const  { return pack_u8p (8,  &SABU_TMA0p_h); }
  int get_tac() const  { return pack_u8p (3,  &SOPU_TAC0p_h); }

  void force_set_tima(uint8_t tima) {
    uint16_t tima_a = tima;
    uint16_t tima_b = ((~tima) << 2);

    REGA_TIMA0p_evn.reset(((tima_a >>  0) & 1) | ((tima_b >>  0) & 2));
    POVY_TIMA1p_evn.reset(((tima_a >>  1) & 1) | ((tima_b >>  1) & 2));
    PERU_TIMA2p_evn.reset(((tima_a >>  2) & 1) | ((tima_b >>  2) & 2));
    RATE_TIMA3p_evn.reset(((tima_a >>  3) & 1) | ((tima_b >>  3) & 2));
    RUBY_TIMA4p_evn.reset(((tima_a >>  4) & 1) | ((tima_b >>  4) & 2));
    RAGE_TIMA5p_evn.reset(((tima_a >>  5) & 1) | ((tima_b >>  5) & 2));
    PEDA_TIMA6p_evn.reset(((tima_a >>  6) & 1) | ((tima_b >>  6) & 2));
    NUGA_TIMA7p_evn.reset(((tima_a >>  7) & 1) | ((tima_b >>  7) & 2));
  }

  void tock(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire BOGA_Axxxxxxx_clkevn,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    wire PIN_CPU_LATCH_EXT,
    BusOut BUS_CPU_D_out[8],
    const DivRegister& div_reg)
  {
    /*#p01.ALUR*/ wire ALUR_SYS_RSTn_new = not1(AVOR_SYS_RSTp);

    /* p03.TOVY*/ wire TOVY_A00n_ext = not1(BUS_CPU_A[ 0]);
    /* p08.TOLA*/ wire TOLA_A01n_ext = not1(BUS_CPU_A[ 1]);

    /*#p03.TEDA*/ wire _TEDA_FF05_RDp_ext =  and4(TEDO_CPU_RDp,    RYFO_FF04_FF07p_ext(BUS_CPU_A),  TOLA_A01n_ext,          BUS_CPU_A[ 0]);
    /*#p03.TOPE*/ wire _TOPE_FF05_WRn_clk = nand4(TAPU_CPU_WRp, RYFO_FF04_FF07p_ext(BUS_CPU_A),  TOLA_A01n_ext,          BUS_CPU_A[ 0]);

    /* p03.TUBY*/ wire _TUBY_FF06_RDp_ext =  and4(TEDO_CPU_RDp,    RYFO_FF04_FF07p_ext(BUS_CPU_A),  BUS_CPU_A[ 1], TOVY_A00n_ext);
    /* p03.TYJU*/ wire _TYJU_FF06_WRn_clk = nand4(TAPU_CPU_WRp, RYFO_FF04_FF07p_ext(BUS_CPU_A),  BUS_CPU_A[ 1], TOVY_A00n_ext);

    /* p03.SORA*/ wire _SORA_FF07_RDp_ext =  and4(TEDO_CPU_RDp,        RYFO_FF04_FF07p_ext(BUS_CPU_A),  BUS_CPU_A[ 1], BUS_CPU_A[ 0]);
    /* p03.SARA*/ wire _SARA_FF07_WRn_clk_evn = nand4(TAPU_CPU_WRp, RYFO_FF04_FF07p_ext(BUS_CPU_A),  BUS_CPU_A[ 1], BUS_CPU_A[ 0]);

    /*#p03.MERY*/ wire _MERY_TIMER_OVERFLOWp_old = nor2(NUGA_TIMA7p_evn.qp_old(), NYDU_TIMA7p_DELAY_evn.qn_old());
    /*#p03.MOBA*/ MOBA_TIMER_OVERFLOWp_evn.dff17(BOGA_Axxxxxxx_clkevn, ALUR_SYS_RSTn_new, _MERY_TIMER_OVERFLOWp_old);

    /*#p03.MEKE*/ wire _MEKE_TIMER_OVERFLOWn_new = not1(MOBA_TIMER_OVERFLOWp_evn.qp_new());
    /*#p03.MUZU*/ wire _MUZU_CPU_LOAD_TIMAn_ext  = or2(PIN_CPU_LATCH_EXT, _TOPE_FF05_WRn_clk);
    /*#p03.MEXU*/ wire _MEXU_TIMA_LOADp_new      = nand3(_MUZU_CPU_LOAD_TIMAn_ext, ALUR_SYS_RSTn_new, _MEKE_TIMER_OVERFLOWn_new);
    /*#p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn_new   = not1(_MEXU_TIMA_LOADp_new);
    /*#p03.NYDU*/ NYDU_TIMA7p_DELAY_evn   .dff17(BOGA_Axxxxxxx_clkevn, _MUGY_TIMA_MAX_RSTn_new, NUGA_TIMA7p_evn.qp_old());

    // FF06 TMA
    /* p03.SABU*/ SABU_TMA0p_h.dff17(_TYJU_FF06_WRn_clk, ALUR_SYS_RSTn_new, BUS_CPU_D[0]);
    /* p03.NYKE*/ NYKE_TMA1p_h.dff17(_TYJU_FF06_WRn_clk, ALUR_SYS_RSTn_new, BUS_CPU_D[1]);
    /* p03.MURU*/ MURU_TMA2p_h.dff17(_TYJU_FF06_WRn_clk, ALUR_SYS_RSTn_new, BUS_CPU_D[2]);
    /* p03.TYVA*/ TYVA_TMA3p_h.dff17(_TYJU_FF06_WRn_clk, ALUR_SYS_RSTn_new, BUS_CPU_D[3]);
    /* p03.TYRU*/ TYRU_TMA4p_h.dff17(_TYJU_FF06_WRn_clk, ALUR_SYS_RSTn_new, BUS_CPU_D[4]);
    /* p03.SUFY*/ SUFY_TMA5p_h.dff17(_TYJU_FF06_WRn_clk, ALUR_SYS_RSTn_new, BUS_CPU_D[5]);
    /* p03.PETO*/ PETO_TMA6p_h.dff17(_TYJU_FF06_WRn_clk, ALUR_SYS_RSTn_new, BUS_CPU_D[6]);
    /* p03.SETA*/ SETA_TMA7p_h.dff17(_TYJU_FF06_WRn_clk, ALUR_SYS_RSTn_new, BUS_CPU_D[7]);

    // FF07 TAC
    /* p03.SOPU*/ SOPU_TAC0p_h.dff17(_SARA_FF07_WRn_clk_evn, ALUR_SYS_RSTn_new, BUS_CPU_D[0]);
    /* p03.SAMY*/ SAMY_TAC1p_h.dff17(_SARA_FF07_WRn_clk_evn, ALUR_SYS_RSTn_new, BUS_CPU_D[1]);
    /* p03.SABO*/ SABO_TAC2p_h.dff17(_SARA_FF07_WRn_clk_evn, ALUR_SYS_RSTn_new, BUS_CPU_D[2]);

    /*#p03.ROKE*/ wire _ROKE_TIMA_D0_new = mux2n(_TOPE_FF05_WRn_clk, SABU_TMA0p_h.qp_new(), BUS_CPU_D[0]);
    /*#p03.PETU*/ wire _PETU_TIMA_D1_new = mux2n(_TOPE_FF05_WRn_clk, NYKE_TMA1p_h.qp_new(), BUS_CPU_D[1]);
    /*#p03.NYKU*/ wire _NYKU_TIMA_D2_new = mux2n(_TOPE_FF05_WRn_clk, MURU_TMA2p_h.qp_new(), BUS_CPU_D[2]);
    /*#p03.SOCE*/ wire _SOCE_TIMA_D3_new = mux2n(_TOPE_FF05_WRn_clk, TYVA_TMA3p_h.qp_new(), BUS_CPU_D[3]);
    /*#p03.SALA*/ wire _SALA_TIMA_D4_new = mux2n(_TOPE_FF05_WRn_clk, TYRU_TMA4p_h.qp_new(), BUS_CPU_D[4]);
    /*#p03.SYRU*/ wire _SYRU_TIMA_D5_new = mux2n(_TOPE_FF05_WRn_clk, SUFY_TMA5p_h.qp_new(), BUS_CPU_D[5]);
    /*#p03.REFU*/ wire _REFU_TIMA_D6_new = mux2n(_TOPE_FF05_WRn_clk, PETO_TMA6p_h.qp_new(), BUS_CPU_D[6]);
    /*#p03.RATO*/ wire _RATO_TIMA_D7_new = mux2n(_TOPE_FF05_WRn_clk, SETA_TMA7p_h.qp_new(), BUS_CPU_D[7]);

    /* p03.MULO*/ wire _MULO_SYS_RSTn_new = not1(ALUR_SYS_RSTn_new);
    /*#p03.PUXY*/ wire _PUXY_TIMA_D0_new = nor2(_MULO_SYS_RSTn_new, _ROKE_TIMA_D0_new);
    /*#p03.NERO*/ wire _NERO_TIMA_D1_new = nor2(_MULO_SYS_RSTn_new, _PETU_TIMA_D1_new);
    /*#p03.NADA*/ wire _NADA_TIMA_D2_new = nor2(_MULO_SYS_RSTn_new, _NYKU_TIMA_D2_new);
    /*#p03.REPA*/ wire _REPA_TIMA_D3_new = nor2(_MULO_SYS_RSTn_new, _SOCE_TIMA_D3_new);
    /*#p03.ROLU*/ wire _ROLU_TIMA_D4_new = nor2(_MULO_SYS_RSTn_new, _SALA_TIMA_D4_new);
    /*#p03.RUGY*/ wire _RUGY_TIMA_D5_new = nor2(_MULO_SYS_RSTn_new, _SYRU_TIMA_D5_new);
    /*#p03.PYMA*/ wire _PYMA_TIMA_D6_new = nor2(_MULO_SYS_RSTn_new, _REFU_TIMA_D6_new);
    /*#p03.PAGU*/ wire _PAGU_TIMA_D7_new = nor2(_MULO_SYS_RSTn_new, _RATO_TIMA_D7_new);

    /*#p03.UBOT*/ wire _UBOT_DIV01n_new_evn = not1(div_reg.UFOR_DIV01p_evn.qp_new());
    /*#p03.UVYR*/ wire _UVYR_DIV03n_new_evn = not1(div_reg.TERO_DIV03p_evn.qp_new());
    /* p01.UVYN*/ wire _UVYN_DIV05n_new_evn = not1(div_reg.TAMA_DIV05p_evn.qp_new());
    /* p01.UREK*/ wire _UREK_DIV07n_new_evn = not1(div_reg.TULU_DIV07p_evn.qp_new());

    /*#p03.UKAP*/ wire _UKAP_CLK_MUXa_new_evn  = mux2n(SOPU_TAC0p_h.qp_new(), _UVYN_DIV05n_new_evn, _UVYR_DIV03n_new_evn);
    /*#p03.TEKO*/ wire _TEKO_CLK_MUXb_new_evn  = mux2n(SOPU_TAC0p_h.qp_new(), _UBOT_DIV01n_new_evn, _UREK_DIV07n_new_evn);
    /*#p03.TECY*/ wire _TECY_CLK_MUXc_new_evn  = mux2n(SAMY_TAC1p_h.qp_new(), _UKAP_CLK_MUXa_new_evn, _TEKO_CLK_MUXb_new_evn);
    /*#p03.SOGU*/ wire _SOGU_TIMA_CLKn_new_evn = nor2(_TECY_CLK_MUXc_new_evn, SABO_TAC2p_h.qn_new());

    // note the data input here is async because of LOADp, so it has to be a new signal
    /*#p03.REGA*/ REGA_TIMA0p_evn.dff20(_SOGU_TIMA_CLKn_new_evn,          _MEXU_TIMA_LOADp_new, _PUXY_TIMA_D0_new);
    /*#p03.POVY*/ POVY_TIMA1p_evn.dff20(REGA_TIMA0p_evn.qp_new(), _MEXU_TIMA_LOADp_new, _NERO_TIMA_D1_new);
    /*#p03.PERU*/ PERU_TIMA2p_evn.dff20(POVY_TIMA1p_evn.qp_new(), _MEXU_TIMA_LOADp_new, _NADA_TIMA_D2_new);
    /*#p03.RATE*/ RATE_TIMA3p_evn.dff20(PERU_TIMA2p_evn.qp_new(), _MEXU_TIMA_LOADp_new, _REPA_TIMA_D3_new);
    /*#p03.RUBY*/ RUBY_TIMA4p_evn.dff20(RATE_TIMA3p_evn.qp_new(), _MEXU_TIMA_LOADp_new, _ROLU_TIMA_D4_new);
    /*#p03.RAGE*/ RAGE_TIMA5p_evn.dff20(RUBY_TIMA4p_evn.qp_new(), _MEXU_TIMA_LOADp_new, _RUGY_TIMA_D5_new);
    /*#p03.PEDA*/ PEDA_TIMA6p_evn.dff20(RAGE_TIMA5p_evn.qp_new(), _MEXU_TIMA_LOADp_new, _PYMA_TIMA_D6_new);
    /*#p03.NUGA*/ NUGA_TIMA7p_evn.dff20(PEDA_TIMA6p_evn.qp_new(), _MEXU_TIMA_LOADp_new, _PAGU_TIMA_D7_new);

    /* FF05 TIMA */
    /*#p03.SOKU*/ BUS_CPU_D_out[0].tri6_pn(_TEDA_FF05_RDp_ext, REGA_TIMA0p_evn.qn_new());
    /*#p03.RACY*/ BUS_CPU_D_out[1].tri6_pn(_TEDA_FF05_RDp_ext, POVY_TIMA1p_evn.qn_new());
    /*#p03.RAVY*/ BUS_CPU_D_out[2].tri6_pn(_TEDA_FF05_RDp_ext, PERU_TIMA2p_evn.qn_new());
    /*#p03.SOSY*/ BUS_CPU_D_out[3].tri6_pn(_TEDA_FF05_RDp_ext, RATE_TIMA3p_evn.qn_new());
    /*#p03.SOMU*/ BUS_CPU_D_out[4].tri6_pn(_TEDA_FF05_RDp_ext, RUBY_TIMA4p_evn.qn_new());
    /*#p03.SURO*/ BUS_CPU_D_out[5].tri6_pn(_TEDA_FF05_RDp_ext, RAGE_TIMA5p_evn.qn_new());
    /*#p03.ROWU*/ BUS_CPU_D_out[6].tri6_pn(_TEDA_FF05_RDp_ext, PEDA_TIMA6p_evn.qn_new());
    /*#p03.PUSO*/ BUS_CPU_D_out[7].tri6_pn(_TEDA_FF05_RDp_ext, NUGA_TIMA7p_evn.qn_new());

    /* FF06 TMA */
    /*#p03.SETE*/ BUS_CPU_D_out[0].tri6_pn(_TUBY_FF06_RDp_ext, SABU_TMA0p_h.qn_new());
    /*#p03.PYRE*/ BUS_CPU_D_out[1].tri6_pn(_TUBY_FF06_RDp_ext, NYKE_TMA1p_h.qn_new());
    /*#p03.NOLA*/ BUS_CPU_D_out[2].tri6_pn(_TUBY_FF06_RDp_ext, MURU_TMA2p_h.qn_new());
    /*#p03.SALU*/ BUS_CPU_D_out[3].tri6_pn(_TUBY_FF06_RDp_ext, TYVA_TMA3p_h.qn_new());
    /*#p03.SUPO*/ BUS_CPU_D_out[4].tri6_pn(_TUBY_FF06_RDp_ext, TYRU_TMA4p_h.qn_new());
    /*#p03.SOTU*/ BUS_CPU_D_out[5].tri6_pn(_TUBY_FF06_RDp_ext, SUFY_TMA5p_h.qn_new());
    /*#p03.REVA*/ BUS_CPU_D_out[6].tri6_pn(_TUBY_FF06_RDp_ext, PETO_TMA6p_h.qn_new());
    /*#p03.SAPU*/ BUS_CPU_D_out[7].tri6_pn(_TUBY_FF06_RDp_ext, SETA_TMA7p_h.qn_new());

    /* FF07 TAC */
    /*#p03.RYLA*/ BUS_CPU_D_out[0].tri6_pn(_SORA_FF07_RDp_ext, SOPU_TAC0p_h.qn_new());
    /*#p03.ROTE*/ BUS_CPU_D_out[1].tri6_pn(_SORA_FF07_RDp_ext, SAMY_TAC1p_h.qn_new());
    /*#p03.SUPE*/ BUS_CPU_D_out[2].tri6_pn(_SORA_FF07_RDp_ext, SABO_TAC2p_h.qn_new());
  }

  /*p03.NYDU*/ DFF17 NYDU_TIMA7p_DELAY_evn;    // Axxxxxxx
  /*p03.MOBA*/ DFF17 MOBA_TIMER_OVERFLOWp_evn; // AxxxExxx

  //----------
  // FF05 TIMA

  /*p03.REGA*/ DFF20 REGA_TIMA0p_evn; // AxxxExxH
  /*p03.POVY*/ DFF20 POVY_TIMA1p_evn; // AxxxExxH
  /*p03.PERU*/ DFF20 PERU_TIMA2p_evn; // AxxxExxH
  /*p03.RATE*/ DFF20 RATE_TIMA3p_evn; // AxxxExxH
  /*p03.RUBY*/ DFF20 RUBY_TIMA4p_evn; // AxxxExxH
  /*p03.RAGE*/ DFF20 RAGE_TIMA5p_evn; // AxxxExxH
  /*p03.PEDA*/ DFF20 PEDA_TIMA6p_evn; // AxxxExxH
  /*p03.NUGA*/ DFF20 NUGA_TIMA7p_evn; // AxxxExxH

  //----------
  // FF06 TMA

  /*p03.SABU*/ DFF17 SABU_TMA0p_h; // xxxxxxxH
  /*p03.NYKE*/ DFF17 NYKE_TMA1p_h; // xxxxxxxH
  /*p03.MURU*/ DFF17 MURU_TMA2p_h; // xxxxxxxH
  /*p03.TYVA*/ DFF17 TYVA_TMA3p_h; // xxxxxxxH
  /*p03.TYRU*/ DFF17 TYRU_TMA4p_h; // xxxxxxxH
  /*p03.SUFY*/ DFF17 SUFY_TMA5p_h; // xxxxxxxH
  /*p03.PETO*/ DFF17 PETO_TMA6p_h; // xxxxxxxH
  /*p03.SETA*/ DFF17 SETA_TMA7p_h; // xxxxxxxH

  //----------
  // FF07 TAC

  /*p03.SOPU*/ DFF17 SOPU_TAC0p_h; // xxxxxxxH
  /*p03.SAMY*/ DFF17 SAMY_TAC1p_h; // xxxxxxxH
  /*p03.SABO*/ DFF17 SABO_TAC2p_h; // xxxxxxxH
};

//-----------------------------------------------------------------------------
