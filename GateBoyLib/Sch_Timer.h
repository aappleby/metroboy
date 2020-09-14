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

  int get_tima_a() const {
    return pack_p(REGA_TIMA_D0.qp(), POVY_TIMA_D1.qp(), PERU_TIMA_D2.qp(), RATE_TIMA_D3.qp(),
                  RUBY_TIMA_D4.qp(), RAGE_TIMA_D5.qp(), PEDA_TIMA_D6.qp(), NUGA_TIMA_D7.qp());
  }

  int get_tima_b() const {
    return pack_p(REGA_TIMA_D0.qn(), POVY_TIMA_D1.qn(), PERU_TIMA_D2.qn(), RATE_TIMA_D3.qn(),
                  RUBY_TIMA_D4.qn(), RAGE_TIMA_D5.qn(), PEDA_TIMA_D6.qn(), NUGA_TIMA_D7.qn());
  }

  int get_tma() const {
    return pack_p(SABU_TMA_D0.qp(), NYKE_TMA_D1.qp(), MURU_TMA_D2.qp(), TYVA_TMA_D3.qp(),
                  TYRU_TMA_D4.qp(), SUFY_TMA_D5.qp(), PETO_TMA_D6.qp(), SETA_TMA_D7.qp());
  }

  int get_tac() const {
    return pack_p(SOPU_TAC_D0.qp(), SAMY_TAC_D1.qp(), SABO_TAC_D2.qp(), 0);
  }

  void force_set_tima(uint8_t tima) {
    uint16_t tima_a = tima;
    uint16_t tima_b = ((~tima) << 2);

    REGA_TIMA_D0.force_state(((tima_a >>  0) & 1) | ((tima_b >>  0) & 2));
    POVY_TIMA_D1.force_state(((tima_a >>  1) & 1) | ((tima_b >>  1) & 2));
    PERU_TIMA_D2.force_state(((tima_a >>  2) & 1) | ((tima_b >>  2) & 2));
    RATE_TIMA_D3.force_state(((tima_a >>  3) & 1) | ((tima_b >>  3) & 2));
    RUBY_TIMA_D4.force_state(((tima_a >>  4) & 1) | ((tima_b >>  4) & 2));
    RAGE_TIMA_D5.force_state(((tima_a >>  5) & 1) | ((tima_b >>  5) & 2));
    PEDA_TIMA_D6.force_state(((tima_a >>  6) & 1) | ((tima_b >>  6) & 2));
    NUGA_TIMA_D7.force_state(((tima_a >>  7) & 1) | ((tima_b >>  7) & 2));
  }

  void force_set_div(uint16_t div) {

    // 0b1111001111011011

    //REG_D0C0 = 0b0000, // 00: state 0 + clock 0
    //REG_D1C0 = 0b0001, // 01: state 1 + clock 0
    //REG_D0C1 = 0b0010, // 02: state 0 + clock 1
    //REG_D1C1 = 0b0011, // 03: state 1 + clock 1

    uint16_t div_a = div;
    uint16_t div_b = ((~div) << 2);

    UKUP_DIV_00.force_state(((div_a >>  0) & 1) | ((div_b >>  0) & 2));
    UFOR_DIV_01.force_state(((div_a >>  1) & 1) | ((div_b >>  1) & 2));
    UNER_DIV_02.force_state(((div_a >>  2) & 1) | ((div_b >>  2) & 2));
    TERO_DIV_03.force_state(((div_a >>  3) & 1) | ((div_b >>  3) & 2));
    UNYK_DIV_04.force_state(((div_a >>  4) & 1) | ((div_b >>  4) & 2));
    TAMA_DIV_05.force_state(((div_a >>  5) & 1) | ((div_b >>  5) & 2));
    UGOT_DIV_06.force_state(((div_a >>  6) & 1) | ((div_b >>  6) & 2));
    TULU_DIV_07.force_state(((div_a >>  7) & 1) | ((div_b >>  7) & 2));
    TUGO_DIV_08.force_state(((div_a >>  8) & 1) | ((div_b >>  8) & 2));
    TOFE_DIV_09.force_state(((div_a >>  9) & 1) | ((div_b >>  9) & 2));
    TERU_DIV_10.force_state(((div_a >> 10) & 1) | ((div_b >> 10) & 2));
    SOLA_DIV_11.force_state(((div_a >> 11) & 1) | ((div_b >> 11) & 2));
    SUBU_DIV_12.force_state(((div_a >> 12) & 1) | ((div_b >> 12) & 2));
    TEKA_DIV_13.force_state(((div_a >> 13) & 1) | ((div_b >> 13) & 2));
    UKET_DIV_14.force_state(((div_a >> 14) & 1) | ((div_b >> 14) & 2));
    UPOF_DIV_15.force_state(((div_a >> 15) & 1) | ((div_b >> 15) & 2));

    /*
    UKUP_DIV_00.force_state(REG_D1C0);
    UFOR_DIV_01.force_state(REG_D1C0);
    UNER_DIV_02.force_state(REG_D0C0);
    TERO_DIV_03.force_state(REG_D1C1);
    UNYK_DIV_04.force_state(REG_D1C0);
    TAMA_DIV_05.force_state(REG_D0C0);
    UGOT_DIV_06.force_state(REG_D1C1);
    TULU_DIV_07.force_state(REG_D1C0);
    TUGO_DIV_08.force_state(REG_D1C0);
    TOFE_DIV_09.force_state(REG_D1C0);
    TERU_DIV_10.force_state(REG_D0C0);
    SOLA_DIV_11.force_state(REG_D0C1);
    SUBU_DIV_12.force_state(REG_D1C1);
    TEKA_DIV_13.force_state(REG_D1C0);
    UKET_DIV_14.force_state(REG_D1C0);
    UPOF_DIV_15.force_state(REG_D1C0);
    */
  }

  /*p03.MOBA*/ DFF17 MOBA_TIMER_OVERFLOWp; // -> interrupts

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

  /*p03.REGA*/ DFF20 REGA_TIMA_D0;
  /*p03.POVY*/ DFF20 POVY_TIMA_D1;
  /*p03.PERU*/ DFF20 PERU_TIMA_D2;
  /*p03.RATE*/ DFF20 RATE_TIMA_D3;
  /*p03.RUBY*/ DFF20 RUBY_TIMA_D4;
  /*p03.RAGE*/ DFF20 RAGE_TIMA_D5;
  /*p03.PEDA*/ DFF20 PEDA_TIMA_D6;
  /*p03.NUGA*/ DFF20 NUGA_TIMA_D7;

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