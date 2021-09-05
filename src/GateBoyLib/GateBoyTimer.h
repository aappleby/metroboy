#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------
// FF04 DIV

struct GateBoyDiv {
  void reset_to_cart() {
    UKUP_DIV00p.state = 0b00011011;
    UFOR_DIV01p.state = 0b00011001;
    UNER_DIV02p.state = 0b00011000;
    TERO_DIV03p.state = 0b00011010;
    UNYK_DIV04p.state = 0b00011011;
    TAMA_DIV05p.state = 0b00011001;
    UGOT_DIV06p.state = 0b00011001;
    TULU_DIV07p.state = 0b00011001;
    TUGO_DIV08p.state = 0b00011000;
    TOFE_DIV09p.state = 0b00011011;
    TERU_DIV10p.state = 0b00011000;
    SOLA_DIV11p.state = 0b00011011;
    SUBU_DIV12p.state = 0b00011000;
    TEKA_DIV13p.state = 0b00011011;
    UKET_DIV14p.state = 0b00011001;
    UPOF_DIV15p.state = 0b00011001;
  }

  int get_div() const  { return pack(16, &UKUP_DIV00p); }

  void force_set_div(uint16_t div) {
    uint16_t div_a = div;
    uint16_t div_b = ((~div) << 2);

    UKUP_DIV00p.state = ((div_a >>  0) & 1) | ((div_b >>  0) & 2);
    UFOR_DIV01p.state = ((div_a >>  1) & 1) | ((div_b >>  1) & 2);
    UNER_DIV02p.state = ((div_a >>  2) & 1) | ((div_b >>  2) & 2);
    TERO_DIV03p.state = ((div_a >>  3) & 1) | ((div_b >>  3) & 2);
    UNYK_DIV04p.state = ((div_a >>  4) & 1) | ((div_b >>  4) & 2);
    TAMA_DIV05p.state = ((div_a >>  5) & 1) | ((div_b >>  5) & 2);
    UGOT_DIV06p.state = ((div_a >>  6) & 1) | ((div_b >>  6) & 2);
    TULU_DIV07p.state = ((div_a >>  7) & 1) | ((div_b >>  7) & 2);
    TUGO_DIV08p.state = ((div_a >>  8) & 1) | ((div_b >>  8) & 2);
    TOFE_DIV09p.state = ((div_a >>  9) & 1) | ((div_b >>  9) & 2);
    TERU_DIV10p.state = ((div_a >> 10) & 1) | ((div_b >> 10) & 2);
    SOLA_DIV11p.state = ((div_a >> 11) & 1) | ((div_b >> 11) & 2);
    SUBU_DIV12p.state = ((div_a >> 12) & 1) | ((div_b >> 12) & 2);
    TEKA_DIV13p.state = ((div_a >> 13) & 1) | ((div_b >> 13) & 2);
    UKET_DIV14p.state = ((div_a >> 14) & 1) | ((div_b >> 14) & 2);
    UPOF_DIV15p.state = ((div_a >> 15) & 1) | ((div_b >> 15) & 2);
  }

  /*_p01.UKUP*/ DFF17 UKUP_DIV00p; // AxxxExxx
  /*_p01.UFOR*/ DFF17 UFOR_DIV01p; // AxxxExxx
  /*_p01.UNER*/ DFF17 UNER_DIV02p; // AxxxExxx
  /*#p01.TERO*/ DFF17 TERO_DIV03p; // AxxxExxx
  /*_p01.UNYK*/ DFF17 UNYK_DIV04p; // AxxxExxx
  /*_p01.TAMA*/ DFF17 TAMA_DIV05p; // AxxxExxx
  /*_p01.UGOT*/ DFF17 UGOT_DIV06p; // AxxxExxx
  /*_p01.TULU*/ DFF17 TULU_DIV07p; // AxxxExxx
  /*_p01.TUGO*/ DFF17 TUGO_DIV08p; // AxxxExxx
  /*_p01.TOFE*/ DFF17 TOFE_DIV09p; // AxxxExxx
  /*_p01.TERU*/ DFF17 TERU_DIV10p; // AxxxExxx
  /*_p01.SOLA*/ DFF17 SOLA_DIV11p; // AxxxExxx
  /*_p01.SUBU*/ DFF17 SUBU_DIV12p; // AxxxExxx
  /*_p01.TEKA*/ DFF17 TEKA_DIV13p; // AxxxExxx
  /*_p01.UKET*/ DFF17 UKET_DIV14p; // AxxxExxx
  /*_p01.UPOF*/ DFF17 UPOF_DIV15p; // AxxxExxx
};

//-----------------------------------------------------------------------------

struct GateBoyTimer {

  int get_tima() const { return pack(8,  &REGA_TIMA0p); }
  int get_tma() const  { return pack(8,  &SABU_TMA0p); }
  int get_tac() const  { return pack(3,  &SOPU_TAC0p); }

  void force_set_tima(uint8_t tima) {
    uint16_t tima_a = tima;
    uint16_t tima_b = ((~tima) << 2);

    REGA_TIMA0p.state = ((tima_a >>  0) & 1) | ((tima_b >>  0) & 2);
    POVY_TIMA1p.state = ((tima_a >>  1) & 1) | ((tima_b >>  1) & 2);
    PERU_TIMA2p.state = ((tima_a >>  2) & 1) | ((tima_b >>  2) & 2);
    RATE_TIMA3p.state = ((tima_a >>  3) & 1) | ((tima_b >>  3) & 2);
    RUBY_TIMA4p.state = ((tima_a >>  4) & 1) | ((tima_b >>  4) & 2);
    RAGE_TIMA5p.state = ((tima_a >>  5) & 1) | ((tima_b >>  5) & 2);
    PEDA_TIMA6p.state = ((tima_a >>  6) & 1) | ((tima_b >>  6) & 2);
    NUGA_TIMA7p.state = ((tima_a >>  7) & 1) | ((tima_b >>  7) & 2);
  }

  /*#p03.NYDU*/ DFF17 NYDU_TIMA7p_DELAY;    // Axxxxxxx
  /*#p03.MOBA*/ DFF17 MOBA_TIMER_OVERFLOWp; // AxxxExxx

  //----------
  // FF05 TIMA

  /*#p03.REGA*/ DFF20 REGA_TIMA0p; // AxxxExxH
  /*#p03.POVY*/ DFF20 POVY_TIMA1p; // AxxxExxH
  /*#p03.PERU*/ DFF20 PERU_TIMA2p; // AxxxExxH
  /*#p03.RATE*/ DFF20 RATE_TIMA3p; // AxxxExxH
  /*#p03.RUBY*/ DFF20 RUBY_TIMA4p; // AxxxExxH
  /*#p03.RAGE*/ DFF20 RAGE_TIMA5p; // AxxxExxH
  /*#p03.PEDA*/ DFF20 PEDA_TIMA6p; // AxxxExxH
  /*#p03.NUGA*/ DFF20 NUGA_TIMA7p; // AxxxExxH

  //----------
  // FF06 TMA

  /*_p03.SABU*/ DFF17 SABU_TMA0p; // xxxxxxxH
  /*_p03.NYKE*/ DFF17 NYKE_TMA1p; // xxxxxxxH
  /*_p03.MURU*/ DFF17 MURU_TMA2p; // xxxxxxxH
  /*_p03.TYVA*/ DFF17 TYVA_TMA3p; // xxxxxxxH
  /*_p03.TYRU*/ DFF17 TYRU_TMA4p; // xxxxxxxH
  /*_p03.SUFY*/ DFF17 SUFY_TMA5p; // xxxxxxxH
  /*_p03.PETO*/ DFF17 PETO_TMA6p; // xxxxxxxH
  /*_p03.SETA*/ DFF17 SETA_TMA7p; // xxxxxxxH

  //----------
  // FF07 TAC

  /*_p03.SOPU*/ DFF17 SOPU_TAC0p; // xxxxxxxH
  /*_p03.SAMY*/ DFF17 SAMY_TAC1p; // xxxxxxxH
  /*_p03.SABO*/ DFF17 SABO_TAC2p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
