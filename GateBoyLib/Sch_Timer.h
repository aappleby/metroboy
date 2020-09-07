#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Timer {

  void dump(Dumper& d) const;
  void tick(const SchematicTop& top);
  void tock(wire RST, const SchematicTop& top, CpuBus& cpu_bus);

  int get_div() const {
    return pack_p(!UKUP_DIV_00.qn(), UFOR_DIV_01.qp(), !UNER_DIV_02.qn(), TERO_DIV_03.qp(),
                  !UNYK_DIV_04.qn(), TAMA_DIV_05.qp(),  UGOT_DIV_06.qp(), TULU_DIV_07.qp(),
                   TUGO_DIV_08.qp(), TOFE_DIV_09.qp(),  TERU_DIV_10.qp(), SOLA_DIV_11.qp(),
                   SUBU_DIV_12.qp(), TEKA_DIV_13.qp(), !UKET_DIV_14.qn(), UPOF_DIV_15.qp());
  }

  int get_tima() const {
    return pack_p(REGA_TIMA_D0.qp(), POVY_TIMA_D1.qp(), PERU_TIMA_D2.qp(), RATE_TIMA_D3.qp(),
                  RUBY_TIMA_D4.qp(), RAGE_TIMA_D5.qp(), PEDA_TIMA_D6.qp(), NUGA_TIMA_D7.qp());
  }

  int get_tma() const {
    return pack_p(SABU_TMA_D0.qp(), NYKE_TMA_D1.qp(), MURU_TMA_D2.qp(), TYVA_TMA_D3.qp(),
                  TYRU_TMA_D4.qp(), SUFY_TMA_D5.qp(), PETO_TMA_D6.qp(), SETA_TMA_D7.qp());
  }

  int get_tac() const {
    return pack_p(SOPU_TAC_D0.qp(), SAMY_TAC_D1.qp(), SABO_TAC_D2.qp(), 0);
  }

  /*p03.MOBA*/ Reg MOBA_INT_TIMERp = REG_D0C0; // -> interrupts

  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg UKUP_DIV_00 = REG_D0C0;
  /*p01.UFOR*/ Reg UFOR_DIV_01 = REG_D0C0;
  /*p01.UNER*/ Reg UNER_DIV_02 = REG_D0C0;
  /*p01.TERO*/ Reg TERO_DIV_03 = REG_D0C0; // -> UNUT_POR_TRIGn (fast boot)
  /*p01.UNYK*/ Reg UNYK_DIV_04 = REG_D0C0;
  /*p01.TAMA*/ Reg TAMA_DIV_05 = REG_D0C0; // -> Serial
  /*p01.UGOT*/ Reg UGOT_DIV_06 = REG_D0C0; // -> LCD
  /*p01.TULU*/ Reg TULU_DIV_07 = REG_D0C0; // -> LCD
  /*p01.TUGO*/ Reg TUGO_DIV_08 = REG_D0C0;
  /*p01.TOFE*/ Reg TOFE_DIV_09 = REG_D0C0;
  /*p01.TERU*/ Reg TERU_DIV_10 = REG_D0C0;
  /*p01.SOLA*/ Reg SOLA_DIV_11 = REG_D0C0;
  /*p01.SUBU*/ Reg SUBU_DIV_12 = REG_D0C0;
  /*p01.TEKA*/ Reg TEKA_DIV_13 = REG_D0C0;
  /*p01.UKET*/ Reg UKET_DIV_14 = REG_D0C0;
  /*p01.UPOF*/ Reg UPOF_DIV_15 = REG_D0C0; // -> UNUT_POR_TRIGn

private:

  /*p03.NYDU*/ Reg NYDU_TIMA_D7_DELAY = REG_D0C0;

  //----------
  // FF05 TIMA

  /*p03.REGA*/ Reg REGA_TIMA_D0 = REG_D0C0;
  /*p03.POVY*/ Reg POVY_TIMA_D1 = REG_D0C0;
  /*p03.PERU*/ Reg PERU_TIMA_D2 = REG_D0C0;
  /*p03.RATE*/ Reg RATE_TIMA_D3 = REG_D0C0;
  /*p03.RUBY*/ Reg RUBY_TIMA_D4 = REG_D0C0;
  /*p03.RAGE*/ Reg RAGE_TIMA_D5 = REG_D0C0;
  /*p03.PEDA*/ Reg PEDA_TIMA_D6 = REG_D0C0;
  /*p03.NUGA*/ Reg NUGA_TIMA_D7 = REG_D0C0;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg SABU_TMA_D0 = REG_D0C0;
  /*p03.NYKE*/ Reg NYKE_TMA_D1 = REG_D0C0;
  /*p03.MURU*/ Reg MURU_TMA_D2 = REG_D0C0;
  /*p03.TYVA*/ Reg TYVA_TMA_D3 = REG_D0C0;
  /*p03.TYRU*/ Reg TYRU_TMA_D4 = REG_D0C0;
  /*p03.SUFY*/ Reg SUFY_TMA_D5 = REG_D0C0;
  /*p03.PETO*/ Reg PETO_TMA_D6 = REG_D0C0;
  /*p03.SETA*/ Reg SETA_TMA_D7 = REG_D0C0;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg SOPU_TAC_D0 = REG_D0C0;
  /*p03.SAMY*/ Reg SAMY_TAC_D1 = REG_D0C0;
  /*p03.SABO*/ Reg SABO_TAC_D2 = REG_D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics