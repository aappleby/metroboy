#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct TimerSignals {
  /*p01.UVYN*/ Signal UVYN_DIV_05n;
  /*p01.UMEK*/ Signal UMEK_DIV_06n;
  /*p01.UREK*/ Signal UREK_DIV_07n;
  /*p01.UPOF*/ Signal UPOF_DIV_15;
  /*p03.MOBA*/ Signal MOBA_INT_TIMERp;
};

//-----------------------------------------------------------------------------

struct TimerRegisters {

  TimerSignals sig() const;

  void tick(SchematicTop& gb);
  void tick(
    const ClockSignals& clk_sig,
    const ResetSignals& rst_sig,
    const CpuBusSignals& cpu_sig,
    CpuBus& cpu_bus,
    bool EXT_PIN_CLK_GOOD,
    bool EXT_PIN_RST);
  
  SignalHash commit();

private:

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
  /*p03.MOBA*/ Reg MOBA_INT_TIMERp;

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