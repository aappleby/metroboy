#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Timer {
  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg DIV_00;
  /*p01.UFOR*/ Reg DIV_01;
  /*p01.UNER*/ Reg DIV_02;
  /*p01.TERO*/ Reg DIV_03;
  /*p01.UNYK*/ Reg DIV_04;
  /*p01.TAMA*/ Reg DIV_05;
  /*p01.UGOT*/ Reg DIV_06;
  /*p01.TULU*/ Reg DIV_07;
  /*p01.TUGO*/ Reg DIV_08;
  /*p01.TOFE*/ Reg DIV_09;
  /*p01.TERU*/ Reg DIV_10;
  /*p01.SOLA*/ Reg DIV_11;
  /*p01.SUBU*/ Reg DIV_12;
  /*p01.TEKA*/ Reg DIV_13;
  /*p01.UKET*/ Reg DIV_14;
  /*p01.UPOF*/ Reg DIV_15;

  /*p01.UMEK*/ bool DIV_06n;
  /*p01.UREK*/ bool DIV_07n;
  /*p01.UTOK*/ bool DIV_08n;
  /*p01.SAPY*/ bool DIV_09n;
  /*p01.UMER*/ bool DIV_10n;
  /*p01.RAVE*/ bool DIV_11n;
  /*p01.RYSO*/ bool DIV_12n;
  /*p01.UDOR*/ bool DIV_13n;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg TAC_0;
  /*p03.SAMY*/ Reg TAC_1;
  /*p03.SABO*/ Reg TAC_2;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg TMA_0;
  /*p03.NYKE*/ Reg TMA_1;
  /*p03.MURU*/ Reg TMA_2;
  /*p03.TYVA*/ Reg TMA_3;
  /*p03.TYRU*/ Reg TMA_4;
  /*p03.SUFY*/ Reg TMA_5;
  /*p03.PETO*/ Reg TMA_6;
  /*p03.SETA*/ Reg TMA_7;

  //----------
  // FF05 TIMA

  /*p03.NYDU*/ Reg TIMA_MAX;
  /*p03.MOBA*/ Reg INT_TIMER;

  /*p03.REGA*/ Reg TIMA_0;
  /*p03.POVY*/ Reg TIMA_1;
  /*p03.PERU*/ Reg TIMA_2;
  /*p03.RATE*/ Reg TIMA_3;
  /*p03.RUBY*/ Reg TIMA_4;
  /*p03.RAGE*/ Reg TIMA_5;
  /*p03.PEDA*/ Reg TIMA_6;
  /*p03.NUGA*/ Reg TIMA_7;
};

//-----------------------------------------------------------------------------

};