#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyResetDebug;
struct GateBoyPhaseClock;
struct GateBoyCpuBus;

//-----------------------------------------------------------------------------
// FF04 DIV

struct GateBoyDiv {
  int get_div() const  { return pack_u16p(16, &UKUP_DIV00p); }

  void force_set_div(uint16_t div) {
    uint16_t div_a = div;
    uint16_t div_b = ((~div) << 2);

    UKUP_DIV00p.reset(((div_a >>  0) & 1) | ((div_b >>  0) & 2));
    UFOR_DIV01p.reset(((div_a >>  1) & 1) | ((div_b >>  1) & 2));
    UNER_DIV02p.reset(((div_a >>  2) & 1) | ((div_b >>  2) & 2));
    TERO_DIV03p.reset(((div_a >>  3) & 1) | ((div_b >>  3) & 2));
    UNYK_DIV04p.reset(((div_a >>  4) & 1) | ((div_b >>  4) & 2));
    TAMA_DIV05p.reset(((div_a >>  5) & 1) | ((div_b >>  5) & 2));
    UGOT_DIV06p.reset(((div_a >>  6) & 1) | ((div_b >>  6) & 2));
    TULU_DIV07p.reset(((div_a >>  7) & 1) | ((div_b >>  7) & 2));
    TUGO_DIV08p.reset(((div_a >>  8) & 1) | ((div_b >>  8) & 2));
    TOFE_DIV09p.reset(((div_a >>  9) & 1) | ((div_b >>  9) & 2));
    TERU_DIV10p.reset(((div_a >> 10) & 1) | ((div_b >> 10) & 2));
    SOLA_DIV11p.reset(((div_a >> 11) & 1) | ((div_b >> 11) & 2));
    SUBU_DIV12p.reset(((div_a >> 12) & 1) | ((div_b >> 12) & 2));
    TEKA_DIV13p.reset(((div_a >> 13) & 1) | ((div_b >> 13) & 2));
    UKET_DIV14p.reset(((div_a >> 14) & 1) | ((div_b >> 14) & 2));
    UPOF_DIV15p.reset(((div_a >> 15) & 1) | ((div_b >> 15) & 2));
  }

  void tock(
    GateBoyResetDebug& rst,
    GateBoyPhaseClock& pclk,
    GateBoyCpuBus& cpu_bus
  );

  /*p01.UKUP*/ DFF17 UKUP_DIV00p; // AxxxExxx
  /*p01.UFOR*/ DFF17 UFOR_DIV01p; // AxxxExxx
  /*p01.UNER*/ DFF17 UNER_DIV02p; // AxxxExxx
  /*p01.TERO*/ DFF17 TERO_DIV03p; // AxxxExxx
  /*p01.UNYK*/ DFF17 UNYK_DIV04p; // AxxxExxx
  /*p01.TAMA*/ DFF17 TAMA_DIV05p; // AxxxExxx
  /*p01.UGOT*/ DFF17 UGOT_DIV06p; // AxxxExxx
  /*p01.TULU*/ DFF17 TULU_DIV07p; // AxxxExxx
  /*p01.TUGO*/ DFF17 TUGO_DIV08p; // AxxxExxx
  /*p01.TOFE*/ DFF17 TOFE_DIV09p; // AxxxExxx
  /*p01.TERU*/ DFF17 TERU_DIV10p; // AxxxExxx
  /*p01.SOLA*/ DFF17 SOLA_DIV11p; // AxxxExxx
  /*p01.SUBU*/ DFF17 SUBU_DIV12p; // AxxxExxx
  /*p01.TEKA*/ DFF17 TEKA_DIV13p; // AxxxExxx
  /*p01.UKET*/ DFF17 UKET_DIV14p; // AxxxExxx
  /*p01.UPOF*/ DFF17 UPOF_DIV15p; // AxxxExxx
};

//-----------------------------------------------------------------------------

struct GateBoyTimer {
  int get_tima() const { return pack_u8p (8,  &REGA_TIMA0p); }
  int get_tma() const  { return pack_u8p (8,  &SABU_TMA0p); }
  int get_tac() const  { return pack_u8p (3,  &SOPU_TAC0p); }

  void force_set_tima(uint8_t tima) {
    uint16_t tima_a = tima;
    uint16_t tima_b = ((~tima) << 2);

    REGA_TIMA0p.reset(((tima_a >>  0) & 1) | ((tima_b >>  0) & 2));
    POVY_TIMA1p.reset(((tima_a >>  1) & 1) | ((tima_b >>  1) & 2));
    PERU_TIMA2p.reset(((tima_a >>  2) & 1) | ((tima_b >>  2) & 2));
    RATE_TIMA3p.reset(((tima_a >>  3) & 1) | ((tima_b >>  3) & 2));
    RUBY_TIMA4p.reset(((tima_a >>  4) & 1) | ((tima_b >>  4) & 2));
    RAGE_TIMA5p.reset(((tima_a >>  5) & 1) | ((tima_b >>  5) & 2));
    PEDA_TIMA6p.reset(((tima_a >>  6) & 1) | ((tima_b >>  6) & 2));
    NUGA_TIMA7p.reset(((tima_a >>  7) & 1) | ((tima_b >>  7) & 2));
  }

  void tock(
    Signal BUS_CPU_A[16],
    Signal BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire BOGA_Axxxxxxx,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    wire PIN_CPU_LATCH_EXT,
    BusOut BUS_CPU_D_out[8],
    const GateBoyDiv& div)
  {
    /* p03.TOVY*/ wire TOVY_A00n = not1(BUS_CPU_A[ 0]);
    /* p08.TOLA*/ wire TOLA_A01n = not1(BUS_CPU_A[ 1]);

    /*#p03.TEDA*/ wire _TEDA_FF05_RDp =  and4(TEDO_CPU_RDp, RYFO_FF04_FF07p(BUS_CPU_A),  TOLA_A01n, BUS_CPU_A[ 0]);
    /*#p03.TOPE*/ wire _TOPE_FF05_WRn = nand4(TAPU_CPU_WRp, RYFO_FF04_FF07p(BUS_CPU_A),  TOLA_A01n, BUS_CPU_A[ 0]);

    /* p03.TUBY*/ wire _TUBY_FF06_RDp =  and4(TEDO_CPU_RDp, RYFO_FF04_FF07p(BUS_CPU_A),  BUS_CPU_A[ 1], TOVY_A00n);
    /* p03.TYJU*/ wire _TYJU_FF06_WRn = nand4(TAPU_CPU_WRp, RYFO_FF04_FF07p(BUS_CPU_A),  BUS_CPU_A[ 1], TOVY_A00n);

    /* p03.SORA*/ wire _SORA_FF07_RDp =  and4(TEDO_CPU_RDp, RYFO_FF04_FF07p(BUS_CPU_A),  BUS_CPU_A[ 1], BUS_CPU_A[ 0]);
    /* p03.SARA*/ wire _SARA_FF07_WRn = nand4(TAPU_CPU_WRp, RYFO_FF04_FF07p(BUS_CPU_A),  BUS_CPU_A[ 1], BUS_CPU_A[ 0]);

    /*#p03.MERY*/ wire _MERY_TIMER_OVERFLOWp_old = nor2(NUGA_TIMA7p.qp_old(), NYDU_TIMA7p_DELAY.qn_old());
    /*#p03.MOBA*/ MOBA_TIMER_OVERFLOWp.dff17(BOGA_Axxxxxxx, ALUR_SYS_RSTn(AVOR_SYS_RSTp), _MERY_TIMER_OVERFLOWp_old);

    /*#p03.MEKE*/ wire _MEKE_TIMER_OVERFLOWn = not1(MOBA_TIMER_OVERFLOWp.qp_new());
    /*#p03.MUZU*/ wire _MUZU_CPU_LOAD_TIMAn  = or2(PIN_CPU_LATCH_EXT, _TOPE_FF05_WRn);
    /*#p03.MEXU*/ wire _MEXU_TIMA_LOADp      = nand3(_MUZU_CPU_LOAD_TIMAn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), _MEKE_TIMER_OVERFLOWn);
    /*#p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn   = not1(_MEXU_TIMA_LOADp);
    /*#p03.NYDU*/ NYDU_TIMA7p_DELAY   .dff17(BOGA_Axxxxxxx, _MUGY_TIMA_MAX_RSTn, NUGA_TIMA7p.qp_old());

    // FF06 TMA
    /* p03.SABU*/ SABU_TMA0p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[0]);
    /* p03.NYKE*/ NYKE_TMA1p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[1]);
    /* p03.MURU*/ MURU_TMA2p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[2]);
    /* p03.TYVA*/ TYVA_TMA3p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[3]);
    /* p03.TYRU*/ TYRU_TMA4p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[4]);
    /* p03.SUFY*/ SUFY_TMA5p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[5]);
    /* p03.PETO*/ PETO_TMA6p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[6]);
    /* p03.SETA*/ SETA_TMA7p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[7]);

    // FF07 TAC
    /* p03.SOPU*/ SOPU_TAC0p.dff17(_SARA_FF07_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[0]);
    /* p03.SAMY*/ SAMY_TAC1p.dff17(_SARA_FF07_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[1]);
    /* p03.SABO*/ SABO_TAC2p.dff17(_SARA_FF07_WRn, ALUR_SYS_RSTn(AVOR_SYS_RSTp), BUS_CPU_D[2]);

    /*#p03.ROKE*/ wire _ROKE_TIMA_D0 = mux2n(_TOPE_FF05_WRn, SABU_TMA0p.qp_new(), BUS_CPU_D[0]);
    /*#p03.PETU*/ wire _PETU_TIMA_D1 = mux2n(_TOPE_FF05_WRn, NYKE_TMA1p.qp_new(), BUS_CPU_D[1]);
    /*#p03.NYKU*/ wire _NYKU_TIMA_D2 = mux2n(_TOPE_FF05_WRn, MURU_TMA2p.qp_new(), BUS_CPU_D[2]);
    /*#p03.SOCE*/ wire _SOCE_TIMA_D3 = mux2n(_TOPE_FF05_WRn, TYVA_TMA3p.qp_new(), BUS_CPU_D[3]);
    /*#p03.SALA*/ wire _SALA_TIMA_D4 = mux2n(_TOPE_FF05_WRn, TYRU_TMA4p.qp_new(), BUS_CPU_D[4]);
    /*#p03.SYRU*/ wire _SYRU_TIMA_D5 = mux2n(_TOPE_FF05_WRn, SUFY_TMA5p.qp_new(), BUS_CPU_D[5]);
    /*#p03.REFU*/ wire _REFU_TIMA_D6 = mux2n(_TOPE_FF05_WRn, PETO_TMA6p.qp_new(), BUS_CPU_D[6]);
    /*#p03.RATO*/ wire _RATO_TIMA_D7 = mux2n(_TOPE_FF05_WRn, SETA_TMA7p.qp_new(), BUS_CPU_D[7]);

    /*#p03.PUXY*/ wire _PUXY_TIMA_D0 = nor2(MULO_SYS_RSTn(AVOR_SYS_RSTp), _ROKE_TIMA_D0);
    /*#p03.NERO*/ wire _NERO_TIMA_D1 = nor2(MULO_SYS_RSTn(AVOR_SYS_RSTp), _PETU_TIMA_D1);
    /*#p03.NADA*/ wire _NADA_TIMA_D2 = nor2(MULO_SYS_RSTn(AVOR_SYS_RSTp), _NYKU_TIMA_D2);
    /*#p03.REPA*/ wire _REPA_TIMA_D3 = nor2(MULO_SYS_RSTn(AVOR_SYS_RSTp), _SOCE_TIMA_D3);
    /*#p03.ROLU*/ wire _ROLU_TIMA_D4 = nor2(MULO_SYS_RSTn(AVOR_SYS_RSTp), _SALA_TIMA_D4);
    /*#p03.RUGY*/ wire _RUGY_TIMA_D5 = nor2(MULO_SYS_RSTn(AVOR_SYS_RSTp), _SYRU_TIMA_D5);
    /*#p03.PYMA*/ wire _PYMA_TIMA_D6 = nor2(MULO_SYS_RSTn(AVOR_SYS_RSTp), _REFU_TIMA_D6);
    /*#p03.PAGU*/ wire _PAGU_TIMA_D7 = nor2(MULO_SYS_RSTn(AVOR_SYS_RSTp), _RATO_TIMA_D7);

    /*#p03.UBOT*/ wire _UBOT_DIV01n = not1(div.UFOR_DIV01p.qp_new());
    /*#p03.UVYR*/ wire _UVYR_DIV03n = not1(div.TERO_DIV03p.qp_new());
    /* p01.UVYN*/ wire _UVYN_DIV05n = not1(div.TAMA_DIV05p.qp_new());
    /* p01.UREK*/ wire _UREK_DIV07n = not1(div.TULU_DIV07p.qp_new());

    /*#p03.UKAP*/ wire _UKAP_CLK_MUXa  = mux2n(SOPU_TAC0p.qp_new(), _UVYN_DIV05n, _UVYR_DIV03n);
    /*#p03.TEKO*/ wire _TEKO_CLK_MUXb  = mux2n(SOPU_TAC0p.qp_new(), _UBOT_DIV01n, _UREK_DIV07n);
    /*#p03.TECY*/ wire _TECY_CLK_MUXc  = mux2n(SAMY_TAC1p.qp_new(), _UKAP_CLK_MUXa, _TEKO_CLK_MUXb);
    /*#p03.SOGU*/ wire _SOGU_TIMA_CLKn = nor2(_TECY_CLK_MUXc, SABO_TAC2p.qn_new());

    // note the data input here is async because of LOADp, so it has to be a new signal
    /*#p03.REGA*/ REGA_TIMA0p.dff20(_SOGU_TIMA_CLKn,      _MEXU_TIMA_LOADp, _PUXY_TIMA_D0);
    /*#p03.POVY*/ POVY_TIMA1p.dff20(REGA_TIMA0p.qp_new(), _MEXU_TIMA_LOADp, _NERO_TIMA_D1);
    /*#p03.PERU*/ PERU_TIMA2p.dff20(POVY_TIMA1p.qp_new(), _MEXU_TIMA_LOADp, _NADA_TIMA_D2);
    /*#p03.RATE*/ RATE_TIMA3p.dff20(PERU_TIMA2p.qp_new(), _MEXU_TIMA_LOADp, _REPA_TIMA_D3);
    /*#p03.RUBY*/ RUBY_TIMA4p.dff20(RATE_TIMA3p.qp_new(), _MEXU_TIMA_LOADp, _ROLU_TIMA_D4);
    /*#p03.RAGE*/ RAGE_TIMA5p.dff20(RUBY_TIMA4p.qp_new(), _MEXU_TIMA_LOADp, _RUGY_TIMA_D5);
    /*#p03.PEDA*/ PEDA_TIMA6p.dff20(RAGE_TIMA5p.qp_new(), _MEXU_TIMA_LOADp, _PYMA_TIMA_D6);
    /*#p03.NUGA*/ NUGA_TIMA7p.dff20(PEDA_TIMA6p.qp_new(), _MEXU_TIMA_LOADp, _PAGU_TIMA_D7);

    /* FF05 TIMA */
    /*#p03.SOKU*/ BUS_CPU_D_out[0].tri6_pn(_TEDA_FF05_RDp, REGA_TIMA0p.qn_new());
    /*#p03.RACY*/ BUS_CPU_D_out[1].tri6_pn(_TEDA_FF05_RDp, POVY_TIMA1p.qn_new());
    /*#p03.RAVY*/ BUS_CPU_D_out[2].tri6_pn(_TEDA_FF05_RDp, PERU_TIMA2p.qn_new());
    /*#p03.SOSY*/ BUS_CPU_D_out[3].tri6_pn(_TEDA_FF05_RDp, RATE_TIMA3p.qn_new());
    /*#p03.SOMU*/ BUS_CPU_D_out[4].tri6_pn(_TEDA_FF05_RDp, RUBY_TIMA4p.qn_new());
    /*#p03.SURO*/ BUS_CPU_D_out[5].tri6_pn(_TEDA_FF05_RDp, RAGE_TIMA5p.qn_new());
    /*#p03.ROWU*/ BUS_CPU_D_out[6].tri6_pn(_TEDA_FF05_RDp, PEDA_TIMA6p.qn_new());
    /*#p03.PUSO*/ BUS_CPU_D_out[7].tri6_pn(_TEDA_FF05_RDp, NUGA_TIMA7p.qn_new());

    /* FF06 TMA */
    /*#p03.SETE*/ BUS_CPU_D_out[0].tri6_pn(_TUBY_FF06_RDp, SABU_TMA0p.qn_new());
    /*#p03.PYRE*/ BUS_CPU_D_out[1].tri6_pn(_TUBY_FF06_RDp, NYKE_TMA1p.qn_new());
    /*#p03.NOLA*/ BUS_CPU_D_out[2].tri6_pn(_TUBY_FF06_RDp, MURU_TMA2p.qn_new());
    /*#p03.SALU*/ BUS_CPU_D_out[3].tri6_pn(_TUBY_FF06_RDp, TYVA_TMA3p.qn_new());
    /*#p03.SUPO*/ BUS_CPU_D_out[4].tri6_pn(_TUBY_FF06_RDp, TYRU_TMA4p.qn_new());
    /*#p03.SOTU*/ BUS_CPU_D_out[5].tri6_pn(_TUBY_FF06_RDp, SUFY_TMA5p.qn_new());
    /*#p03.REVA*/ BUS_CPU_D_out[6].tri6_pn(_TUBY_FF06_RDp, PETO_TMA6p.qn_new());
    /*#p03.SAPU*/ BUS_CPU_D_out[7].tri6_pn(_TUBY_FF06_RDp, SETA_TMA7p.qn_new());

    /* FF07 TAC */
    /*#p03.RYLA*/ BUS_CPU_D_out[0].tri6_pn(_SORA_FF07_RDp, SOPU_TAC0p.qn_new());
    /*#p03.ROTE*/ BUS_CPU_D_out[1].tri6_pn(_SORA_FF07_RDp, SAMY_TAC1p.qn_new());
    /*#p03.SUPE*/ BUS_CPU_D_out[2].tri6_pn(_SORA_FF07_RDp, SABO_TAC2p.qn_new());
  }

  /*p03.NYDU*/ DFF17 NYDU_TIMA7p_DELAY;    // Axxxxxxx
  /*p03.MOBA*/ DFF17 MOBA_TIMER_OVERFLOWp; // AxxxExxx

  //----------
  // FF05 TIMA

  /*p03.REGA*/ DFF20 REGA_TIMA0p; // AxxxExxH
  /*p03.POVY*/ DFF20 POVY_TIMA1p; // AxxxExxH
  /*p03.PERU*/ DFF20 PERU_TIMA2p; // AxxxExxH
  /*p03.RATE*/ DFF20 RATE_TIMA3p; // AxxxExxH
  /*p03.RUBY*/ DFF20 RUBY_TIMA4p; // AxxxExxH
  /*p03.RAGE*/ DFF20 RAGE_TIMA5p; // AxxxExxH
  /*p03.PEDA*/ DFF20 PEDA_TIMA6p; // AxxxExxH
  /*p03.NUGA*/ DFF20 NUGA_TIMA7p; // AxxxExxH

  //----------
  // FF06 TMA

  /*p03.SABU*/ DFF17 SABU_TMA0p; // xxxxxxxH
  /*p03.NYKE*/ DFF17 NYKE_TMA1p; // xxxxxxxH
  /*p03.MURU*/ DFF17 MURU_TMA2p; // xxxxxxxH
  /*p03.TYVA*/ DFF17 TYVA_TMA3p; // xxxxxxxH
  /*p03.TYRU*/ DFF17 TYRU_TMA4p; // xxxxxxxH
  /*p03.SUFY*/ DFF17 SUFY_TMA5p; // xxxxxxxH
  /*p03.PETO*/ DFF17 PETO_TMA6p; // xxxxxxxH
  /*p03.SETA*/ DFF17 SETA_TMA7p; // xxxxxxxH

  //----------
  // FF07 TAC

  /*p03.SOPU*/ DFF17 SOPU_TAC0p; // xxxxxxxH
  /*p03.SAMY*/ DFF17 SAMY_TAC1p; // xxxxxxxH
  /*p03.SABO*/ DFF17 SABO_TAC2p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
