#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Timer {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);

  int get_div() const {
    return pack(!_UKUP_DIV_00.qn(), _UFOR_DIV_01.qp(), !_UNER_DIV_02.qn(), _TERO_DIV_03.qp(),
                !_UNYK_DIV_04.qn(), _TAMA_DIV_05.qp(),  _UGOT_DIV_06.qp(), _TULU_DIV_07.qp(),
                 _TUGO_DIV_08.qp(), _TOFE_DIV_09.qp(),  _TERU_DIV_10.qp(), _SOLA_DIV_11.qp(),
                 _SUBU_DIV_12.qp(), _TEKA_DIV_13.qp(), !_UKET_DIV_14.qn(), _UPOF_DIV_15.qp());
  }

  int get_tima() const {
    return pack(REGA_TIMA_0.qp(), POVY_TIMA_1.qp(), PERU_TIMA_2.qp(), RATE_TIMA_3.qp(),
                RUBY_TIMA_4.qp(), RAGE_TIMA_5.qp(), PEDA_TIMA_6.qp(), NUGA_TIMA_7.qp());
  }

  int get_tma() const {
    return pack(SABU_TMA_0.qp(), NYKE_TMA_1.qp(), MURU_TMA_2.qp(), TYVA_TMA_3.qp(),
                TYRU_TMA_4.qp(), SUFY_TMA_5.qp(), PETO_TMA_6.qp(), SETA_TMA_7.qp());
  }

  int get_tac() const {
    return pack(SOPU_TAC_0.qp(), SAMY_TAC_1.qp(), SABO_TAC_2.qp(), 0);
  }

  wire UVYN_DIV_05n() const { return _TAMA_DIV_05.qn(); }
  wire UMEK_DIV_06n() const { return _UGOT_DIV_06.qn(); }
  wire UREK_DIV_07n() const { return _TULU_DIV_07.qn(); }

  wire UKUP_DIV_00()  const { return !_UKUP_DIV_00.qn(); }
  wire UFOR_DIV_01()  const { return  _UFOR_DIV_01.qp(); }
  wire UNER_DIV_02()  const { return !_UNER_DIV_02.qn(); }
  wire TERO_DIV_03()  const { return  _TERO_DIV_03.qp(); }
  wire UNYK_DIV_04()  const { return !_UNYK_DIV_04.qn(); }
  wire TAMA_DIV_05()  const { return  _TAMA_DIV_05.qp(); }
  wire UGOT_DIV_06()  const { return  _UGOT_DIV_06.qp(); }
  wire TULU_DIV_07()  const { return  _TULU_DIV_07.qp(); }
  wire TUGO_DIV_08()  const { return  _TUGO_DIV_08.qp(); }
  wire TOFE_DIV_09()  const { return  _TOFE_DIV_09.qp(); }
  wire TERU_DIV_10()  const { return  _TERU_DIV_10.qp(); }
  wire SOLA_DIV_11()  const { return  _SOLA_DIV_11.qp(); }
  wire SUBU_DIV_12()  const { return  _SUBU_DIV_12.qp(); }
  wire TEKA_DIV_13()  const { return  _TEKA_DIV_13.qp(); }
  wire UKET_DIV_14()  const { return !_UKET_DIV_14.qn(); }
  wire UPOF_DIV_15()  const { return  _UPOF_DIV_15.qp(); }

  void dump(Dumper& d) const {
    d("----------  Timer   ----------\n");
    d("DIV        %05d\n", get_div());
    d("TIMA       %03d\n", get_tima());
    d("TMA        %03d\n", get_tma());
    d("TAC        %03d\n", get_tac());
    d("TIMA_MAX   %c\n", NYDU_TIMA_MAX.c());
    d("INT_TIMERp %c\n", MOBA_INT_TIMERp.c());
    d("\n");
  }

//private:
  friend SchematicTop;

  //----------
  // FF04 DIV

  /*p01.UKUP*/ RegQN  _UKUP_DIV_00 = REG_D0C0;
  /*p01.UFOR*/ RegQPN _UFOR_DIV_01 = REG_D0C0;
  /*p01.UNER*/ RegQN  _UNER_DIV_02 = REG_D0C0;
  /*p01.TERO*/ RegQPN _TERO_DIV_03 = REG_D0C0;
  /*p01.UNYK*/ RegQN  _UNYK_DIV_04 = REG_D0C0;
  /*p01.TAMA*/ RegQPN _TAMA_DIV_05 = REG_D0C0;
  /*p01.UGOT*/ RegQPN _UGOT_DIV_06 = REG_D0C0;
  /*p01.TULU*/ RegQPN _TULU_DIV_07 = REG_D0C0;
  /*p01.TUGO*/ RegQPN _TUGO_DIV_08 = REG_D0C0;
  /*p01.TOFE*/ RegQPN _TOFE_DIV_09 = REG_D0C0;
  /*p01.TERU*/ RegQPN _TERU_DIV_10 = REG_D0C0;
  /*p01.SOLA*/ RegQPN _SOLA_DIV_11 = REG_D0C0;
  /*p01.SUBU*/ RegQPN _SUBU_DIV_12 = REG_D0C0;
  /*p01.TEKA*/ RegQPN _TEKA_DIV_13 = REG_D0C0;
  /*p01.UKET*/ RegQN  _UKET_DIV_14 = REG_D0C0;
  /*p01.UPOF*/ RegQPN _UPOF_DIV_15 = REG_D0C0;

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

  /*p03.NYDU*/ RegQN NYDU_TIMA_MAX = REG_D0C0;
  /*p03.MOBA*/ RegQP MOBA_INT_TIMERp = REG_D0C0;

  //----------
  // FF06 TMA

  /*p03.SABU*/ RegQPN SABU_TMA_0 = REG_D0C0;
  /*p03.NYKE*/ RegQPN NYKE_TMA_1 = REG_D0C0;
  /*p03.MURU*/ RegQPN MURU_TMA_2 = REG_D0C0;
  /*p03.TYVA*/ RegQPN TYVA_TMA_3 = REG_D0C0;
  /*p03.TYRU*/ RegQPN TYRU_TMA_4 = REG_D0C0;
  /*p03.SUFY*/ RegQPN SUFY_TMA_5 = REG_D0C0;
  /*p03.PETO*/ RegQPN PETO_TMA_6 = REG_D0C0;
  /*p03.SETA*/ RegQPN SETA_TMA_7 = REG_D0C0;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ RegQPN SOPU_TAC_0 = REG_D0C0;
  /*p03.SAMY*/ RegQPN SAMY_TAC_1 = REG_D0C0;
  /*p03.SABO*/ RegQPN SABO_TAC_2 = REG_D0C0;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics