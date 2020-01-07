#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SysPins;

//-----------------------------------------------------------------------------

struct DebugSignals {
  /*p05.KUKO*/ bool DBG_FF00_D6;
  /*p05.KERU*/ bool DBG_FF00_D7;
  /*p05.KORE*/ bool P05_NC0;
  /*p05.KYWE*/ bool P05_NC1;

  /*p07.BURO*/ bool FF60_0;
  /*p05.KURA*/ bool FF60_0n;
  /*p07.AMUT*/ bool FF60_1;
};

struct DebugRegisters {

  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  DebugSignals tick(const SysSignals& sys_sig,
                    const RstSignals& rst_sig1);

  /*p07.BURO*/ Reg2 FF60_0;
  /*p07.AMUT*/ Reg2 FF60_1;
};

//-----------------------------------------------------------------------------

};