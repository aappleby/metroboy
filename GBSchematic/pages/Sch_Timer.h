#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TimerSignals {
  /*p01.UMEK*/ bool DIV_06n;
  /*p01.UREK*/ bool DIV_07n;
  /*p01.UTOK*/ bool DIV_08n;
  /*p01.SAPY*/ bool DIV_09n;
  /*p01.UMER*/ bool DIV_10n;
  /*p01.RAVE*/ bool DIV_11n;
  /*p01.RYSO*/ bool DIV_12n;
  /*p01.UDOR*/ bool DIV_13n;

  /*p03.MOBA*/ bool INT_TIMER;
};

//-----------------------------------------------------------------------------

struct Timer {

  void pwron();
  void reset();

  void tickDIV(const SystemRegisters& sys_reg,
               const ClockSignals1& clk_sig1,
               const BusControl& ctl,
               const Decoder& dec);

  void tickTIMA(const SystemRegisters& sys_reg,
                const Bus& bus,
                const BusControl& ctl,
                const Decoder& dec,
                const ClockSignals1& clk,
                const ResetSignals1& rst_sig);

  void tickTMA(const Bus& bus,
               const BusControl& ctl,
               const Decoder& dec,
               const ResetSignals1& rst_sig);

  void tickTAC(const Bus& bus,
               const BusControl& ctl,
               const Decoder& dec,
               const ResetSignals1& rst_sig);

  void tickBusRead(const Bus& bus,
                   const BusControl& ctl,
                   const Decoder& dec,
                   Bus& bus_out) const;

  TimerSignals signals() const;

  void commit();

private:

  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg2 DIV_00;
  /*p01.UFOR*/ Reg2 DIV_01;
  /*p01.UNER*/ Reg2 DIV_02;
  /*p01.TERO*/ Reg2 DIV_03;
  /*p01.UNYK*/ Reg2 DIV_04;
  /*p01.TAMA*/ Reg2 DIV_05;
  /*p01.UGOT*/ Reg2 DIV_06;
  /*p01.TULU*/ Reg2 DIV_07;
  /*p01.TUGO*/ Reg2 DIV_08;
  /*p01.TOFE*/ Reg2 DIV_09;
  /*p01.TERU*/ Reg2 DIV_10;
  /*p01.SOLA*/ Reg2 DIV_11;
  /*p01.SUBU*/ Reg2 DIV_12;
  /*p01.TEKA*/ Reg2 DIV_13;
  /*p01.UKET*/ Reg2 DIV_14;
  /*p01.UPOF*/ Reg2 DIV_15;

  //----------
  // FF05 TIMA

  /*p03.NYDU*/ Reg2 TIMA_MAX;
  /*p03.MOBA*/ Reg2 INT_TIMER;

  /*p03.REGA*/ Counter TIMA_0;
  /*p03.POVY*/ Counter TIMA_1;
  /*p03.PERU*/ Counter TIMA_2;
  /*p03.RATE*/ Counter TIMA_3;
  /*p03.RUBY*/ Counter TIMA_4;
  /*p03.RAGE*/ Counter TIMA_5;
  /*p03.PEDA*/ Counter TIMA_6;
  /*p03.NUGA*/ Counter TIMA_7;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg2 TMA_0;
  /*p03.NYKE*/ Reg2 TMA_1;
  /*p03.MURU*/ Reg2 TMA_2;
  /*p03.TYVA*/ Reg2 TMA_3;
  /*p03.TYRU*/ Reg2 TMA_4;
  /*p03.SUFY*/ Reg2 TMA_5;
  /*p03.PETO*/ Reg2 TMA_6;
  /*p03.SETA*/ Reg2 TMA_7;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg2 TAC_0;
  /*p03.SAMY*/ Reg2 TAC_1;
  /*p03.SABO*/ Reg2 TAC_2;
};

//-----------------------------------------------------------------------------

};