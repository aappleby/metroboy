#pragma once
#include "Schematics.h"

//-----------------------------------------------------------------------------

struct TimerInput {

  bool RST;
  bool SYS_RESETn;

  bool CPUCLK_REQn;
  bool CLK_BAD1;
  bool CLK_BAD2;

  bool PHASE_ABCDxxxx1;
  bool PHASE_xBCDExxx1;
  bool PHASE_xxCDEFxx1;

  bool CPU_RD;
  bool CPU_WR;
  bool FROM_CPU5;

  bool FF60_1;

  union {
    uint16_t A;
    struct {
      bool A00 : 1;
      bool A01 : 1;
      bool A02 : 1;
      bool A03 : 1;
      bool A04 : 1;
      bool A05 : 1;
      bool A06 : 1;
      bool A07 : 1;
      bool A08 : 1;
      bool A09 : 1;
      bool A10 : 1;
      bool A11 : 1;
      bool A12 : 1;
      bool A13 : 1;
      bool A14 : 1;
      bool A15 : 1;
    };
  };

  union { 
    uint8_t D;
    struct {
      bool D0 : 1;
      bool D1 : 1;
      bool D2 : 1;
      bool D3 : 1;
      bool D4 : 1;
      bool D5 : 1;
      bool D6 : 1;
      bool D7 : 1;
    };
  };
};

//-----------------------------------------------------------------------------

struct TimerOutput {
  union { 
    uint8_t D;
    struct {
      bool D0 : 1;
      bool D1 : 1;
      bool D2 : 1;
      bool D3 : 1;
      bool D4 : 1;
      bool D5 : 1;
      bool D6 : 1;
      bool D7 : 1;
    };
  };
};

//-----------------------------------------------------------------------------

struct Timer {
  /*p01.UKUP*/ bool DIV_00;
  /*p01.UFOR*/ bool DIV_01;
  /*p01.UNER*/ bool DIV_02;
  /*p01.TERO*/ bool DIV_03;
  /*p01.UNYK*/ bool DIV_04;
  /*p01.TAMA*/ bool DIV_05;
  /*p01.UGOT*/ bool DIV_06;
  /*p01.TULU*/ bool DIV_07;
  /*p01.TUGO*/ bool DIV_08;
  /*p01.TOFE*/ bool DIV_09;
  /*p01.TERU*/ bool DIV_10;
  /*p01.SOLA*/ bool DIV_11;
  /*p01.SUBU*/ bool DIV_12;
  /*p01.TEKA*/ bool DIV_13;
  /*p01.UKET*/ bool DIV_14;
  /*p01.UPOF*/ bool DIV_15;

  /*p01.UMEK*/ bool DIV_06n;
  /*p01.UREK*/ bool DIV_07n;
  /*p01.UTOK*/ bool DIV_08n;
  /*p01.SAPY*/ bool DIV_09n;
  /*p01.UMER*/ bool DIV_10n;
  /*p01.RAVE*/ bool DIV_11n;
  /*p01.RYSO*/ bool DIV_12n;
  /*p01.UDOR*/ bool DIV_13n;

  /*p01.BOGA*/ bool BOGA_xBCDEFGH;

  /*p01.ULUR*/ bool DIV_06_CLK;

  //----------
  // FF07 TAC

  /*p03.SARA*/ bool FF07_WRn;
  /*p03.SOPU*/ bool TAC_0;
  /*p03.SAMY*/ bool TAC_1;
  /*p03.SABO*/ bool TAC_2;

  //----------
  // FF06 TMA

  /*p03.TYJU*/ bool FF06_WRn;

  /*p03.SABU*/ bool TMA_0;
  /*p03.NYKE*/ bool TMA_1;
  /*p03.MURU*/ bool TMA_2;
  /*p03.TYVA*/ bool TMA_3;
  /*p03.TYRU*/ bool TMA_4;
  /*p03.SUFY*/ bool TMA_5;
  /*p03.PETO*/ bool TMA_6;
  /*p03.SETA*/ bool TMA_7;

  //----------
  // FF05 TIMA

  /*p03.SOGU*/ bool TIMA_CLK;
  /*p03.NYDU*/ bool TIMA_MAX;
  /*p03.MOBA*/ bool INT_TIMER;

  /*p03.TOPE*/ bool FF05_WRn;
  /*p03.REGA*/ bool TIMA_0;
  /*p03.POVY*/ bool TIMA_1;
  /*p03.PERU*/ bool TIMA_2;
  /*p03.RATE*/ bool TIMA_3;
  /*p03.RUBY*/ bool TIMA_4;
  /*p03.RAGE*/ bool TIMA_5;
  /*p03.PEDA*/ bool TIMA_6;
  /*p03.NUGA*/ bool TIMA_7;
};

//-----------------------------------------------------------------------------
