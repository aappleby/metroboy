#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SysPins;

//-----------------------------------------------------------------------------

struct DebugRegisters {

  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  void tick(const SysSignals& sys_sig,
            const BusSignals& bus_sig,
            const RstSignals& rst_sig,
            const ClkSignals& clk_sig,
            const DecoderSignals& dec_sig,
            BusTristates& bus_tri);

  /*p07.BURO*/ Reg2 FF60_0;
  /*p07.AMUT*/ Reg2 FF60_1;
};

//-----------------------------------------------------------------------------

};