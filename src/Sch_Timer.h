#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct TimerSignals {
  /*p01.UVYN*/ wire UVYN_DIV_05n;
  /*p01.UMEK*/ wire UMEK_DIV_06n;
  /*p01.UREK*/ wire UREK_DIV_07n;
};

//-----------------------------------------------------------------------------

struct TimerRegisters {

  TimerSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- TIM_REG -----\n");
    text_painter.dprintf("DIV       %d\n", get_div());
    text_painter.dprintf("TIMA      %d\n", get_tima());
    text_painter.dprintf("TMA       %d\n", get_tma());
    text_painter.newline();

    UKUP_DIV_00.dump(text_painter, "UKUP_DIV_00    ");
    UFOR_DIV_01.dump(text_painter, "UFOR_DIV_01    ");
    UNER_DIV_02.dump(text_painter, "UNER_DIV_02    ");
    TERO_DIV_03.dump(text_painter, "TERO_DIV_03    ");
    UNYK_DIV_04.dump(text_painter, "UNYK_DIV_04    ");
    TAMA_DIV_05.dump(text_painter, "TAMA_DIV_05    ");
    UGOT_DIV_06.dump(text_painter, "UGOT_DIV_06    ");
    TULU_DIV_07.dump(text_painter, "TULU_DIV_07    ");
    TUGO_DIV_08.dump(text_painter, "TUGO_DIV_08    ");
    TOFE_DIV_09.dump(text_painter, "TOFE_DIV_09    ");
    TERU_DIV_10.dump(text_painter, "TERU_DIV_10    ");
    SOLA_DIV_11.dump(text_painter, "SOLA_DIV_11    ");
    SUBU_DIV_12.dump(text_painter, "SUBU_DIV_12    ");
    TEKA_DIV_13.dump(text_painter, "TEKA_DIV_13    ");
    UKET_DIV_14.dump(text_painter, "UKET_DIV_14    ");
    UPOF_DIV_15.dump(text_painter, "UPOF_DIV_15    ");
    text_painter.newline();

    TIMA_0.dump(text_painter, "TIMA_0    ");
    TIMA_1.dump(text_painter, "TIMA_1    ");
    TIMA_2.dump(text_painter, "TIMA_2    ");
    TIMA_3.dump(text_painter, "TIMA_3    ");
    TIMA_4.dump(text_painter, "TIMA_4    ");
    TIMA_5.dump(text_painter, "TIMA_5    ");
    TIMA_6.dump(text_painter, "TIMA_6    ");
    TIMA_7.dump(text_painter, "TIMA_7    ");
    text_painter.newline();

    TMA_0.dump(text_painter, "TMA_0     ");
    TMA_1.dump(text_painter, "TMA_1     ");
    TMA_2.dump(text_painter, "TMA_2     ");
    TMA_3.dump(text_painter, "TMA_3     ");
    TMA_4.dump(text_painter, "TMA_4     ");
    TMA_5.dump(text_painter, "TMA_5     ");
    TMA_6.dump(text_painter, "TMA_6     ");
    TMA_7.dump(text_painter, "TMA_7     ");
    text_painter.newline();

    TAC_0.dump(text_painter, "TAC_0     ");
    TAC_1.dump(text_painter, "TAC_1     ");
    TAC_2.dump(text_painter, "TAC_2     ");
    text_painter.newline();

    TIMA_MAX.dump(text_painter, "TIMA_MAX  ");
    PIN_INT_TIMER.dump(text_painter, "PIN_INT_TIMER ");
    text_painter.newline();
  }

  int get_div() const {
    return pack(UKUP_DIV_00.q(), UFOR_DIV_01.q(), UNER_DIV_02.q(), TERO_DIV_03.q(), UNYK_DIV_04.q(), TAMA_DIV_05.q(), UGOT_DIV_06.q(), TULU_DIV_07.q(),
      TUGO_DIV_08.q(), TOFE_DIV_09.q(), TERU_DIV_10.q(), SOLA_DIV_11.q(), SUBU_DIV_12.q(), TEKA_DIV_13.q(), UKET_DIV_14.q(), UPOF_DIV_15.q());
  }

  /*
  void set_div(uint16_t x) {
    DIV_00.set2(x & 0x0001); DIV_01.set2(x & 0x0002); DIV_02.set2(x & 0x0004); DIV_03.set2(x & 0x0008);
    DIV_04.set2(x & 0x0010); DIV_05.set2(x & 0x0020); DIV_06.set2(x & 0x0040); DIV_07.set2(x & 0x0080);
    DIV_08.set2(x & 0x0100); DIV_09.set2(x & 0x0200); DIV_10.set2(x & 0x0400); DIV_11.set2(x & 0x0800);
    DIV_12.set2(x & 0x1000); DIV_13.set2(x & 0x2000); DIV_14.set2(x & 0x4000); DIV_15.set2(x & 0x8000);
  }
  */

  int get_tima() const {
    return pack(TIMA_0.q(), TIMA_1.q(), TIMA_2.q(), TIMA_3.q(), TIMA_4.q(), TIMA_5.q(), TIMA_6.q(), TIMA_7.q());
  }

  /*
  void set_tima(uint8_t x) {
    TIMA_0.set2(x & 0x01); TIMA_1.set2(x & 0x02); TIMA_2.set2(x & 0x04); TIMA_3.set2(x & 0x08);
    TIMA_4.set2(x & 0x10); TIMA_5.set2(x & 0x20); TIMA_6.set2(x & 0x40); TIMA_7.set2(x & 0x80);
  }
  */

  wire get_tima_max() { return TIMA_MAX.q(); }
  wire get_int_timer() { return PIN_INT_TIMER.q(); }

  int get_tma() const {
    return pack(TMA_0.q(), TMA_1.q(), TMA_2.q(), TMA_3.q(), TMA_4.q(), TMA_5.q(), TMA_6.q(), TMA_7.q());
  }

  /*
  void set_tma(uint8_t x) {
    TMA_0.set2(x & 0x01); TMA_1.set2(x & 0x02); TMA_2.set2(x & 0x04); TMA_3.set2(x & 0x08);
    TMA_4.set2(x & 0x10); TMA_5.set2(x & 0x20); TMA_6.set2(x & 0x40); TMA_7.set2(x & 0x80);
  }
  */

  int get_tac() const {
    return pack(TAC_0.q(), TAC_1.q(), TAC_2.q());
  }

  /*
  void set_tac(uint8_t x) {
    TAC_0.set2(x & 1);
    TAC_1.set2(x & 2);
    TAC_2.set2(x & 4);
  }
  */

  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg UKUP_DIV_00;
  /*p01.UFOR*/ Reg UFOR_DIV_01;
  /*p01.UNER*/ Reg UNER_DIV_02;
  /*p01.TERO*/ Reg TERO_DIV_03;
  /*p01.UNYK*/ Reg UNYK_DIV_04;
  /*p01.TAMA*/ Reg TAMA_DIV_05;
  /*p01.UGOT*/ Reg UGOT_DIV_06;
  /*p01.TULU*/ Reg TULU_DIV_07;
  /*p01.TUGO*/ Reg TUGO_DIV_08;
  /*p01.TOFE*/ Reg TOFE_DIV_09;
  /*p01.TERU*/ Reg TERU_DIV_10;
  /*p01.SOLA*/ Reg SOLA_DIV_11;
  /*p01.SUBU*/ Reg SUBU_DIV_12;
  /*p01.TEKA*/ Reg TEKA_DIV_13;
  /*p01.UKET*/ Reg UKET_DIV_14;
  /*p01.UPOF*/ Reg UPOF_DIV_15;

  //----------
  // FF05 TIMA

  /*p03.REGA*/ Counter TIMA_0;
  /*p03.POVY*/ Counter TIMA_1;
  /*p03.PERU*/ Counter TIMA_2;
  /*p03.RATE*/ Counter TIMA_3;
  /*p03.RUBY*/ Counter TIMA_4;
  /*p03.RAGE*/ Counter TIMA_5;
  /*p03.PEDA*/ Counter TIMA_6;
  /*p03.NUGA*/ Counter TIMA_7;

  /*p03.NYDU*/ Reg TIMA_MAX;
  /*p03.MOBA*/ Reg PIN_INT_TIMER;

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
  // FF07 TAC

  /*p03.SOPU*/ Reg TAC_0;
  /*p03.SAMY*/ Reg TAC_1;
  /*p03.SABO*/ Reg TAC_2;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics