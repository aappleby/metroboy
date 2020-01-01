#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ResetSignals1 {

  static ResetSignals1 tick_slow(const SystemRegisters& sys_reg,
                                 const ClockSignals1& clk_sig1,
                                 const ResetRegisters& rst_reg);
  static ResetSignals1 tick_fast(const SystemRegisters& sys_reg,
                                 const ResetRegisters& rst_reg);

  /*p01.BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07
  /*p01.ALUR*/ bool SYS_RESETn;
  /*p01.DULA*/ bool SYS_RESET;

  /*p01.CUNU*/ bool CUNU_RESETn;

  /*p01.WESY*/ bool WESY_RESET;
  /*p01.WALU*/ bool WALU_RESET;
  /*p01.XARE*/ bool XARE_RESET;
  /*p01.SOTO*/ bool SOTO_RESET;
};

//-----------------------------------------------------------------------------

struct ResetSignals2 {

  static ResetSignals2 tick_slow(const SystemRegisters& sys_reg, const ResetSignals1& rst_sig1);
  static ResetSignals2 tick_fast(const SystemRegisters& sys_reg, const ResetSignals1& rst_sig1);

  /*p01.XAPO*/ bool VID_RESETn;
  /*p01.TOFU*/ bool VID_RESET3;
  /*p01.PYRY*/ bool VID_RESET4;
  /*p01.ROSY*/ bool VID_RESET5;
  /*p01.ATAR*/ bool VID_RESET6;
  /*p01.ABEZ*/ bool VID_RESETn3;
};

//-----------------------------------------------------------------------------

struct ResetRegisters {

  void pwron();
  void reset();

  void tick_slow(const SystemRegisters& sys_reg,
                 const ClockSignals1& clk_sig1);
  void tick_fast(const SystemRegisters& sys_reg);
  void commit();

  /*p01.TUBO*/ bool WAITING_FOR_CLKREQ;
  /*p01.AFER*/ Reg2 RESET_REG;
};

//-----------------------------------------------------------------------------

};