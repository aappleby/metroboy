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

  /*p03.MOBA*/ DFF17 MOBA_INT_TIMERp; // -> interrupts

  //----------
  // FF04 DIV

  /*p01.UKUP*/ DFF17 UKUP_DIV_00;
  /*p01.UFOR*/ DFF17 UFOR_DIV_01;
  /*p01.UNER*/ DFF17 UNER_DIV_02;
  /*p01.TERO*/ DFF17 TERO_DIV_03; // -> UNUT_POR_TRIGn (fast boot)
  /*p01.UNYK*/ DFF17 UNYK_DIV_04;
  /*p01.TAMA*/ DFF17 TAMA_DIV_05; // -> Serial
  /*p01.UGOT*/ DFF17 UGOT_DIV_06; // -> LCD
  /*p01.TULU*/ DFF17 TULU_DIV_07; // -> LCD
  /*p01.TUGO*/ DFF17 TUGO_DIV_08;
  /*p01.TOFE*/ DFF17 TOFE_DIV_09;
  /*p01.TERU*/ DFF17 TERU_DIV_10;
  /*p01.SOLA*/ DFF17 SOLA_DIV_11;
  /*p01.SUBU*/ DFF17 SUBU_DIV_12;
  /*p01.TEKA*/ DFF17 TEKA_DIV_13;
  /*p01.UKET*/ DFF17 UKET_DIV_14;
  /*p01.UPOF*/ DFF17 UPOF_DIV_15; // -> UNUT_POR_TRIGn

private:

  /*p03.NYDU*/ DFF17 NYDU_TIMA_D7_DELAY;

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

  /*p03.SABU*/ DFF17 SABU_TMA_D0;
  /*p03.NYKE*/ DFF17 NYKE_TMA_D1;
  /*p03.MURU*/ DFF17 MURU_TMA_D2;
  /*p03.TYVA*/ DFF17 TYVA_TMA_D3;
  /*p03.TYRU*/ DFF17 TYRU_TMA_D4;
  /*p03.SUFY*/ DFF17 SUFY_TMA_D5;
  /*p03.PETO*/ DFF17 PETO_TMA_D6;
  /*p03.SETA*/ DFF17 SETA_TMA_D7;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ DFF17 SOPU_TAC_D0;
  /*p03.SAMY*/ DFF17 SAMY_TAC_D1;
  /*p03.SABO*/ DFF17 SABO_TAC_D2;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics