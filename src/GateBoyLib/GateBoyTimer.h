#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct RegDIV {
  void reset_to_bootrom();
  void reset_to_cart();

  void force_set_div(uint16_t div);

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

struct RegTIMA {
  void reset_to_bootrom();
  void reset_to_cart();

  void force_set_tima(uint8_t tima);

  /*#p03.REGA*/ DFF20 REGA_TIMA0p; // AxxxExxH
  /*#p03.POVY*/ DFF20 POVY_TIMA1p; // AxxxExxH
  /*#p03.PERU*/ DFF20 PERU_TIMA2p; // AxxxExxH
  /*#p03.RATE*/ DFF20 RATE_TIMA3p; // AxxxExxH
  /*#p03.RUBY*/ DFF20 RUBY_TIMA4p; // AxxxExxH
  /*#p03.RAGE*/ DFF20 RAGE_TIMA5p; // AxxxExxH
  /*#p03.PEDA*/ DFF20 PEDA_TIMA6p; // AxxxExxH
  /*#p03.NUGA*/ DFF20 NUGA_TIMA7p; // AxxxExxH
};

//-----------------------------------------------------------------------------

struct RegTMA {
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p03.SABU*/ DFF17 SABU_TMA0p; // xxxxxxxH
  /*_p03.NYKE*/ DFF17 NYKE_TMA1p; // xxxxxxxH
  /*_p03.MURU*/ DFF17 MURU_TMA2p; // xxxxxxxH
  /*_p03.TYVA*/ DFF17 TYVA_TMA3p; // xxxxxxxH
  /*_p03.TYRU*/ DFF17 TYRU_TMA4p; // xxxxxxxH
  /*_p03.SUFY*/ DFF17 SUFY_TMA5p; // xxxxxxxH
  /*_p03.PETO*/ DFF17 PETO_TMA6p; // xxxxxxxH
  /*_p03.SETA*/ DFF17 SETA_TMA7p; // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct RegTAC {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p03.SOPU*/ DFF17 SOPU_TAC0p; // xxxxxxxH
  /*_p03.SAMY*/ DFF17 SAMY_TAC1p; // xxxxxxxH
  /*_p03.SABO*/ DFF17 SABO_TAC2p; // xxxxxxxH
};

//-----------------------------------------------------------------------------
