#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct TimerRegisters {

  void tick(SchematicTop& gb);
  void tock(SchematicTop& gb);

  SignalHash commit();

  int get_div() {
    return pack(UKUP_DIV_00.q(), UFOR_DIV_01.q(), UNER_DIV_02.q(), TERO_DIV_03.q(),
                UNYK_DIV_04.q(), TAMA_DIV_05.q(), UGOT_DIV_06.q(), TULU_DIV_07.q(),
                TUGO_DIV_08.q(), TOFE_DIV_09.q(), TERU_DIV_10.q(), SOLA_DIV_11.q(),
                SUBU_DIV_12.q(), TEKA_DIV_13.q(), UKET_DIV_14.q(), UPOF_DIV_15.q());
  }

  int get_tima() {
    return pack(REGA_TIMA_0.q(), POVY_TIMA_1.q(), PERU_TIMA_2.q(), RATE_TIMA_3.q(),
                RUBY_TIMA_4.q(), RAGE_TIMA_5.q(), PEDA_TIMA_6.q(), NUGA_TIMA_7.q());
  }

  int get_tma() {
    return pack(SABU_TMA_0.q(), NYKE_TMA_1.q(), MURU_TMA_2.q(), TYVA_TMA_3.q(),
                TYRU_TMA_4.q(), SUFY_TMA_5.q(), PETO_TMA_6.q(), SETA_TMA_7.q());
  }

  int get_tac() {
    return pack(SOPU_TAC_0.q(), SAMY_TAC_1.q(), SABO_TAC_2.q(), 0);
  }

private:
  friend SchematicTop;

  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg17 UKUP_DIV_00;
  /*p01.UFOR*/ Reg17 UFOR_DIV_01;
  /*p01.UNER*/ Reg17 UNER_DIV_02;
  /*p01.TERO*/ Reg17 TERO_DIV_03;
  /*p01.UNYK*/ Reg17 UNYK_DIV_04;
  /*p01.TAMA*/ Reg17 TAMA_DIV_05;
  /*p01.UGOT*/ Reg17 UGOT_DIV_06;
  /*p01.TULU*/ Reg17 TULU_DIV_07;
  /*p01.TUGO*/ Reg17 TUGO_DIV_08;
  /*p01.TOFE*/ Reg17 TOFE_DIV_09;
  /*p01.TERU*/ Reg17 TERU_DIV_10;
  /*p01.SOLA*/ Reg17 SOLA_DIV_11;
  /*p01.SUBU*/ Reg17 SUBU_DIV_12;
  /*p01.TEKA*/ Reg17 TEKA_DIV_13;
  /*p01.UKET*/ Reg17 UKET_DIV_14;
  /*p01.UPOF*/ Reg17 UPOF_DIV_15;

  //----------
  // FF05 TIMA

  /*p03.REGA*/ Counter REGA_TIMA_0;
  /*p03.POVY*/ Counter POVY_TIMA_1;
  /*p03.PERU*/ Counter PERU_TIMA_2;
  /*p03.RATE*/ Counter RATE_TIMA_3;
  /*p03.RUBY*/ Counter RUBY_TIMA_4;
  /*p03.RAGE*/ Counter RAGE_TIMA_5;
  /*p03.PEDA*/ Counter PEDA_TIMA_6;
  /*p03.NUGA*/ Counter NUGA_TIMA_7;

  /*p03.NYDU*/ Reg17 NYDU_TIMA_MAX;
  /*p03.MOBA*/ Reg17 MOBA_INT_TIMERp;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg17 SABU_TMA_0;
  /*p03.NYKE*/ Reg17 NYKE_TMA_1;
  /*p03.MURU*/ Reg17 MURU_TMA_2;
  /*p03.TYVA*/ Reg17 TYVA_TMA_3;
  /*p03.TYRU*/ Reg17 TYRU_TMA_4;
  /*p03.SUFY*/ Reg17 SUFY_TMA_5;
  /*p03.PETO*/ Reg17 PETO_TMA_6;
  /*p03.SETA*/ Reg17 SETA_TMA_7;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg17 SOPU_TAC_0;
  /*p03.SAMY*/ Reg17 SAMY_TAC_1;
  /*p03.SABO*/ Reg17 SABO_TAC_2;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics