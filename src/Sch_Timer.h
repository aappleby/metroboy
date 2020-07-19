#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Timer {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  uint64_t commit();

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

  /*p01.UKUP*/ Reg2 _UKUP_DIV_00 = Reg2::D0C0;
  /*p01.UFOR*/ Reg2 _UFOR_DIV_01 = Reg2::D0C0;
  /*p01.UNER*/ Reg2 _UNER_DIV_02 = Reg2::D0C0;
  /*p01.TERO*/ Reg2 _TERO_DIV_03 = Reg2::D0C0;
  /*p01.UNYK*/ Reg2 _UNYK_DIV_04 = Reg2::D0C0;
  /*p01.TAMA*/ Reg2 _TAMA_DIV_05 = Reg2::D0C0;
  /*p01.UGOT*/ Reg2 _UGOT_DIV_06 = Reg2::D0C0;
  /*p01.TULU*/ Reg2 _TULU_DIV_07 = Reg2::D0C0;
  /*p01.TUGO*/ Reg2 _TUGO_DIV_08 = Reg2::D0C0;
  /*p01.TOFE*/ Reg2 _TOFE_DIV_09 = Reg2::D0C0;
  /*p01.TERU*/ Reg2 _TERU_DIV_10 = Reg2::D0C0;
  /*p01.SOLA*/ Reg2 _SOLA_DIV_11 = Reg2::D0C0;
  /*p01.SUBU*/ Reg2 _SUBU_DIV_12 = Reg2::D0C0;
  /*p01.TEKA*/ Reg2 _TEKA_DIV_13 = Reg2::D0C0;
  /*p01.UKET*/ Reg2 _UKET_DIV_14 = Reg2::D0C0;
  /*p01.UPOF*/ Reg2 _UPOF_DIV_15 = Reg2::D0C0;

  //----------
  // FF05 TIMA

  /*p03.REGA*/ Reg2 REGA_TIMA_0 = Reg2::D0C0;
  /*p03.POVY*/ Reg2 POVY_TIMA_1 = Reg2::D0C0;
  /*p03.PERU*/ Reg2 PERU_TIMA_2 = Reg2::D0C0;
  /*p03.RATE*/ Reg2 RATE_TIMA_3 = Reg2::D0C0;
  /*p03.RUBY*/ Reg2 RUBY_TIMA_4 = Reg2::D0C0;
  /*p03.RAGE*/ Reg2 RAGE_TIMA_5 = Reg2::D0C0;
  /*p03.PEDA*/ Reg2 PEDA_TIMA_6 = Reg2::D0C0;
  /*p03.NUGA*/ Reg2 NUGA_TIMA_7 = Reg2::D0C0;

  /*p03.NYDU*/ Reg2 NYDU_TIMA_MAX = Reg2::D0C0;
  /*p03.MOBA*/ Reg2 _MOBA_INT_TIMERp = Reg2::D0C0;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg2 SABU_TMA_0 = Reg2::D0C0;
  /*p03.NYKE*/ Reg2 NYKE_TMA_1 = Reg2::D0C0;
  /*p03.MURU*/ Reg2 MURU_TMA_2 = Reg2::D0C0;
  /*p03.TYVA*/ Reg2 TYVA_TMA_3 = Reg2::D0C0;
  /*p03.TYRU*/ Reg2 TYRU_TMA_4 = Reg2::D0C0;
  /*p03.SUFY*/ Reg2 SUFY_TMA_5 = Reg2::D0C0;
  /*p03.PETO*/ Reg2 PETO_TMA_6 = Reg2::D0C0;
  /*p03.SETA*/ Reg2 SETA_TMA_7 = Reg2::D0C0;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg2 SOPU_TAC_0 = Reg2::D0C0;
  /*p03.SAMY*/ Reg2 SAMY_TAC_1 = Reg2::D0C0;
  /*p03.SABO*/ Reg2 SABO_TAC_2 = Reg2::D0C0;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics