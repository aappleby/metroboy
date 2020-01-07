#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct RstSignals {
  /*p01.BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07
  /*p01.TABA*/ bool CPU_RESET;
  /*p01.ALYP*/ bool CPU_RESETn;
  /*p01.DULA*/ bool SYS_RESET;
  /*p01.ALUR*/ bool SYS_RESETn;
  /*p01.XODO*/ bool VID_RESET;
  /*p01.XAPO*/ bool VID_RESETn;
};

//-----------------------------------------------------------------------------

struct RstRegisters {
  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  RstSignals tick_slow(const SysSignals& sys_sig, const ClkSignals& clk_sig);

  /*p01.TUBO*/ Reg2 WAITING_FOR_CLKREQ;
  /*p01.AFER*/ Reg2 RESET_REG;

  bool changed;
};

//-----------------------------------------------------------------------------

};