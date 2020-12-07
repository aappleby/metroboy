#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct TimerRegisters {
  void reset_cart() {
    NYDU_TIMA7p_DELAY_evn.reset(REG_D0C1);
    MOBA_TIMER_OVERFLOWp_evn.reset(REG_D0C1);

    UKUP_DIV00p_evn.reset(REG_D1C1);
    UFOR_DIV01p_evn.reset(REG_D1C0);
    UNER_DIV02p_evn.reset(REG_D0C0);
    TERO_DIV03p_evn.reset(REG_D0C1);
    UNYK_DIV04p_evn.reset(REG_D1C1);
    TAMA_DIV05p_evn.reset(REG_D1C0);
    UGOT_DIV06p_evn.reset(REG_D1C0);
    TULU_DIV07p_evn.reset(REG_D1C0);
    TUGO_DIV08p_evn.reset(REG_D0C0);
    TOFE_DIV09p_evn.reset(REG_D1C1);
    TERU_DIV10p_evn.reset(REG_D0C0);
    SOLA_DIV11p_evn.reset(REG_D1C1);
    SUBU_DIV12p_evn.reset(REG_D0C0);
    TEKA_DIV13p_evn.reset(REG_D1C1);
    UKET_DIV14p_evn.reset(REG_D1C0);
    UPOF_DIV15p_evn.reset(REG_D1C0);

    REGA_TIMA0p_evn.reset(REG_D0C1);
    POVY_TIMA1p_evn.reset(REG_D0C1);
    PERU_TIMA2p_evn.reset(REG_D0C1);
    RATE_TIMA3p_evn.reset(REG_D0C1);
    RUBY_TIMA4p_evn.reset(REG_D0C1);
    RAGE_TIMA5p_evn.reset(REG_D0C1);
    PEDA_TIMA6p_evn.reset(REG_D0C1);
    NUGA_TIMA7p_evn.reset(REG_D0C1);

    SABU_TMA0p_h.reset(REG_D0C1);
    NYKE_TMA1p_h.reset(REG_D0C1);
    MURU_TMA2p_h.reset(REG_D0C1);
    TYVA_TMA3p_h.reset(REG_D0C1);
    TYRU_TMA4p_h.reset(REG_D0C1);
    SUFY_TMA5p_h.reset(REG_D0C1);
    PETO_TMA6p_h.reset(REG_D0C1);
    SETA_TMA7p_h.reset(REG_D0C1);

    SOPU_TAC0p_h.reset(REG_D0C1);
    SAMY_TAC1p_h.reset(REG_D0C1);
    SABO_TAC2p_h.reset(REG_D0C1);
  }

  void reset_boot() {
    NYDU_TIMA7p_DELAY_evn.reset(REG_D0C0);
    MOBA_TIMER_OVERFLOWp_evn.reset(REG_D0C0);

    UKUP_DIV00p_evn.reset(REG_D0C0);
    UFOR_DIV01p_evn.reset(REG_D0C0);
    UNER_DIV02p_evn.reset(REG_D0C0);
    TERO_DIV03p_evn.reset(REG_D0C0);
    UNYK_DIV04p_evn.reset(REG_D0C0);
    TAMA_DIV05p_evn.reset(REG_D0C0);
    UGOT_DIV06p_evn.reset(REG_D0C0);
    TULU_DIV07p_evn.reset(REG_D0C0);
    TUGO_DIV08p_evn.reset(REG_D0C0);
    TOFE_DIV09p_evn.reset(REG_D0C0);
    TERU_DIV10p_evn.reset(REG_D0C0);
    SOLA_DIV11p_evn.reset(REG_D0C0);
    SUBU_DIV12p_evn.reset(REG_D0C0);
    TEKA_DIV13p_evn.reset(REG_D0C0);
    UKET_DIV14p_evn.reset(REG_D0C0);
    UPOF_DIV15p_evn.reset(REG_D0C0);

    REGA_TIMA0p_evn.reset(REG_D0C0);
    POVY_TIMA1p_evn.reset(REG_D0C0);
    PERU_TIMA2p_evn.reset(REG_D0C0);
    RATE_TIMA3p_evn.reset(REG_D0C0);
    RUBY_TIMA4p_evn.reset(REG_D0C0);
    RAGE_TIMA5p_evn.reset(REG_D0C0);
    PEDA_TIMA6p_evn.reset(REG_D0C0);
    NUGA_TIMA7p_evn.reset(REG_D0C0);

    SABU_TMA0p_h.reset(REG_D0C0);
    NYKE_TMA1p_h.reset(REG_D0C0);
    MURU_TMA2p_h.reset(REG_D0C0);
    TYVA_TMA3p_h.reset(REG_D0C0);
    TYRU_TMA4p_h.reset(REG_D0C0);
    SUFY_TMA5p_h.reset(REG_D0C0);
    PETO_TMA6p_h.reset(REG_D0C0);
    SETA_TMA7p_h.reset(REG_D0C0);

    SOPU_TAC0p_h.reset(REG_D0C0);
    SAMY_TAC1p_h.reset(REG_D0C0);
    SABO_TAC2p_h.reset(REG_D0C0);
  }

  int get_div() const  { return pack_u16p(16, &UKUP_DIV00p_evn); }
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

  /*p03.NYDU*/ DFF17 NYDU_TIMA7p_DELAY_evn;    // Axxxxxxx
  /*p03.MOBA*/ DFF17 MOBA_TIMER_OVERFLOWp_evn; // AxxxExxx

  //----------
  // FF04 DIV

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
