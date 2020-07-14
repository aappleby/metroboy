#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Timer {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  SignalHash commit();

  int get_div() {
    return pack(_UKUP_DIV_00.q(), _UFOR_DIV_01.q(), _UNER_DIV_02.q(), _TERO_DIV_03.q(),
                _UNYK_DIV_04.q(), _TAMA_DIV_05.q(), _UGOT_DIV_06.q(), _TULU_DIV_07.q(),
                _TUGO_DIV_08.q(), _TOFE_DIV_09.q(), _TERU_DIV_10.q(), _SOLA_DIV_11.q(),
                _SUBU_DIV_12.q(), _TEKA_DIV_13.q(), _UKET_DIV_14.q(), _UPOF_DIV_15.q());
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

  wire UVYN_DIV_05n() const { return not(_TAMA_DIV_05.q()); }
  wire UMEK_DIV_06n() const { return not(_UGOT_DIV_06.q()); }
  wire UREK_DIV_07n() const { return not(_TULU_DIV_07.q()); }

  wire UKUP_DIV_00()  const { return _UKUP_DIV_00.q(); }
  wire UFOR_DIV_01()  const { return _UFOR_DIV_01.q(); }
  wire UNER_DIV_02()  const { return _UNER_DIV_02.q(); }
  wire TERO_DIV_03()  const { return _TERO_DIV_03.q(); }
  wire UNYK_DIV_04()  const { return _UNYK_DIV_04.q(); }
  wire TERU_DIV_10()  const { return _TERU_DIV_10.q(); }
  wire UPOF_DIV_15()  const { return _UPOF_DIV_15.q(); }

  wire MOBA_INT_TIMERp() const { return _MOBA_INT_TIMERp.q(); }

private:
  friend SchematicTop;

  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg17 _UKUP_DIV_00;
  /*p01.UFOR*/ Reg17 _UFOR_DIV_01;
  /*p01.UNER*/ Reg17 _UNER_DIV_02;
  /*p01.TERO*/ Reg17 _TERO_DIV_03;
  /*p01.UNYK*/ Reg17 _UNYK_DIV_04;
  /*p01.TAMA*/ Reg17 _TAMA_DIV_05;
  /*p01.UGOT*/ Reg17 _UGOT_DIV_06;
  /*p01.TULU*/ Reg17 _TULU_DIV_07;
  /*p01.TUGO*/ Reg17 _TUGO_DIV_08;
  /*p01.TOFE*/ Reg17 _TOFE_DIV_09;
  /*p01.TERU*/ Reg17 _TERU_DIV_10;
  /*p01.SOLA*/ Reg17 _SOLA_DIV_11;
  /*p01.SUBU*/ Reg17 _SUBU_DIV_12;
  /*p01.TEKA*/ Reg17 _TEKA_DIV_13;
  /*p01.UKET*/ Reg17 _UKET_DIV_14;
  /*p01.UPOF*/ Reg17 _UPOF_DIV_15;

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
  /*p03.MOBA*/ Reg17 _MOBA_INT_TIMERp;

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