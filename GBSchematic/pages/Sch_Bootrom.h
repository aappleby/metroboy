#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Bootrom {
  void tick(const BusSignals& bus_sig,
            const RstSignals& rst_sig,
            const DebugSignals& dbg_sig,
            const DecoderSignals& dec_sig,
            BusTristates& bus_tri);

  /*p07.TEPU*/ Reg2 BOOT_BIT;
};

//-----------------------------------------------------------------------------

};