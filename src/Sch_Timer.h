#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Timer {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);

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

  wire UVYN_DIV_05n() const { return _TAMA_DIV_05.qn(); }
  wire UMEK_DIV_06n() const { return _UGOT_DIV_06.qn(); }
  wire UREK_DIV_07n() const { return _TULU_DIV_07.qn(); }

  wire UKUP_DIV_00()  const { return _UKUP_DIV_00.q(); }
  wire UFOR_DIV_01()  const { return _UFOR_DIV_01.q(); }
  wire UNER_DIV_02()  const { return _UNER_DIV_02.q(); }
  wire TERO_DIV_03()  const { return _TERO_DIV_03.q(); }
  wire UNYK_DIV_04()  const { return _UNYK_DIV_04.q(); }
  wire TAMA_DIV_05()  const { return _TAMA_DIV_05.q(); }
  wire UGOT_DIV_06()  const { return _UGOT_DIV_06.q(); }
  wire TULU_DIV_07()  const { return _TULU_DIV_07.q(); }
  wire TUGO_DIV_08()  const { return _TUGO_DIV_08.q(); }
  wire TOFE_DIV_09()  const { return _TOFE_DIV_09.q(); }
  wire TERU_DIV_10()  const { return _TERU_DIV_10.q(); }
  wire SOLA_DIV_11()  const { return _SOLA_DIV_11.q(); }
  wire SUBU_DIV_12()  const { return _SUBU_DIV_12.q(); }
  wire TEKA_DIV_13()  const { return _TEKA_DIV_13.q(); }
  wire UKET_DIV_14()  const { return _UKET_DIV_14.q(); }
  wire UPOF_DIV_15()  const { return _UPOF_DIV_15.q(); }

  wire MOBA_INT_TIMERp() const { return _MOBA_INT_TIMERp.q(); }

  void dump(Dumper& d) {
    d("----------  Timer   ----------\n");
    d("DIV        %05d\n", get_div());
    d("TIMA       %03d\n", get_tima());
    d("TMA        %03d\n", get_tma());
    d("TAC        %03d\n", get_tac());
    d("TIMA_MAX   %c\n", NYDU_TIMA_MAX.c());
    d("INT_TIMERp %c\n", _MOBA_INT_TIMERp.c());
    d("\n");
  }

private:
  friend SchematicTop;

  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg _UKUP_DIV_00 = REG_D0C0;
  /*p01.UFOR*/ Reg _UFOR_DIV_01 = REG_D0C0;
  /*p01.UNER*/ Reg _UNER_DIV_02 = REG_D0C0;
  /*p01.TERO*/ Reg _TERO_DIV_03 = REG_D0C0;
  /*p01.UNYK*/ Reg _UNYK_DIV_04 = REG_D0C0;
  /*p01.TAMA*/ Reg _TAMA_DIV_05 = REG_D0C0;
  /*p01.UGOT*/ Reg _UGOT_DIV_06 = REG_D0C0;
  /*p01.TULU*/ Reg _TULU_DIV_07 = REG_D0C0;
  /*p01.TUGO*/ Reg _TUGO_DIV_08 = REG_D0C0;
  /*p01.TOFE*/ Reg _TOFE_DIV_09 = REG_D0C0;
  /*p01.TERU*/ Reg _TERU_DIV_10 = REG_D0C0;
  /*p01.SOLA*/ Reg _SOLA_DIV_11 = REG_D0C0;
  /*p01.SUBU*/ Reg _SUBU_DIV_12 = REG_D0C0;
  /*p01.TEKA*/ Reg _TEKA_DIV_13 = REG_D0C0;
  /*p01.UKET*/ Reg _UKET_DIV_14 = REG_D0C0;
  /*p01.UPOF*/ Reg _UPOF_DIV_15 = REG_D0C0;

  //----------
  // FF05 TIMA

  /*p03.REGA*/ Reg REGA_TIMA_0 = REG_D0C0;
  /*p03.POVY*/ Reg POVY_TIMA_1 = REG_D0C0;
  /*p03.PERU*/ Reg PERU_TIMA_2 = REG_D0C0;
  /*p03.RATE*/ Reg RATE_TIMA_3 = REG_D0C0;
  /*p03.RUBY*/ Reg RUBY_TIMA_4 = REG_D0C0;
  /*p03.RAGE*/ Reg RAGE_TIMA_5 = REG_D0C0;
  /*p03.PEDA*/ Reg PEDA_TIMA_6 = REG_D0C0;
  /*p03.NUGA*/ Reg NUGA_TIMA_7 = REG_D0C0;

  /*p03.NYDU*/ Reg NYDU_TIMA_MAX = REG_D0C0;
  /*p03.MOBA*/ Reg _MOBA_INT_TIMERp = REG_D0C0;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg SABU_TMA_0 = REG_D0C0;
  /*p03.NYKE*/ Reg NYKE_TMA_1 = REG_D0C0;
  /*p03.MURU*/ Reg MURU_TMA_2 = REG_D0C0;
  /*p03.TYVA*/ Reg TYVA_TMA_3 = REG_D0C0;
  /*p03.TYRU*/ Reg TYRU_TMA_4 = REG_D0C0;
  /*p03.SUFY*/ Reg SUFY_TMA_5 = REG_D0C0;
  /*p03.PETO*/ Reg PETO_TMA_6 = REG_D0C0;
  /*p03.SETA*/ Reg SETA_TMA_7 = REG_D0C0;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg SOPU_TAC_0 = REG_D0C0;
  /*p03.SAMY*/ Reg SAMY_TAC_1 = REG_D0C0;
  /*p03.SABO*/ Reg SABO_TAC_2 = REG_D0C0;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics