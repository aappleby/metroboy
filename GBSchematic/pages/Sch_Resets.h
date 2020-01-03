#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct VrstSignals {
  /*p01.XAPO*/ bool VID_RESETn;
  /*p01.TOFU*/ bool VID_RESET3;
  /*p01.PYRY*/ bool VID_RESET4;
  /*p01.ROSY*/ bool VID_RESET5;
  /*p01.ATAR*/ bool VID_RESET6;
  /*p01.ABEZ*/ bool VID_RESETn3;
};

//-----------------------------------------------------------------------------

struct RstSignals {
  /*p01.BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07
  /*p01.ALUR*/ bool SYS_RESETn;
  /*p01.DULA*/ bool SYS_RESET;
  /*p01.CUNU*/ bool CUNU_RESETn; // dma
  /*p01.WESY*/ bool WESY_RESET; // video, registers
  /*p01.WALU*/ bool WALU_RESET; // registers
  /*p01.XARE*/ bool XARE_RESET; // registers
};

//-----------------------------------------------------------------------------

struct RstRegisters {
  void pwron();
  void reset();
  RstSignals tick_slow(const SysSignals& sys_sig, const ClkSignals& clk_sig1);
  void commit();

  RstSignals  rst_signals (const SysSignals& sys_sig, const ClkSignals& clk_sig1);
  VrstSignals vrst_signals(const SysSignals& sys_sig, const RstSignals& rst_sig);

  /*p01.TUBO*/ bool WAITING_FOR_CLKREQ;
  /*p01.AFER*/ Reg2 RESET_REG;
};

//-----------------------------------------------------------------------------

};