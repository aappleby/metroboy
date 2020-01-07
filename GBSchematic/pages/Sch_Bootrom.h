#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Bootrom {
  void tick(const SysSignals& sys_sig,
            const BusSignals& bus_sig,
            const RstSignals& rst_sig,
            const DecoderSignals& dec_sig,
            BusTristates& bus_tri);

  /*p07.TEPU*/ Reg2 BOOT_BIT;
};

//-----------------------------------------------------------------------------

};