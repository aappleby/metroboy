#include "GateBoyLib/Sch_CpuBus.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void CpuBus::dump(Dumper& d) const {
  d("---------- CPU Bus  ----------\n");
  d("CPU BOOT : %d\n", CPU_PIN_BOOTp.tp());
  d("CPU AHI  : %d\n", CPU_PIN_ADDR_HI.tp());
  d("CPU RDp  : %d\n", CPU_PIN_RDp.tp());
  d("CPU WRp  : %d\n", CPU_PIN_WRp.tp());
  d("CPU AXp  : %d\n", CPU_PIN_ADDR_EXTp.tp());
  d("CPU HMp  : %d\n", CPU_PIN_LATCH_EXT.tp());

  d("CPU ADDR : %04x\n", get_bus_addr());

  d("CPU DATA : %02x\n", get_bus_data());

  d("CPU PIN ADDR    : %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    CPU_BUS_A15.c(), CPU_BUS_A14.c(), CPU_BUS_A13.c(), CPU_BUS_A12.c(),
    CPU_BUS_A11.c(), CPU_BUS_A10.c(), CPU_BUS_A09.c(), CPU_BUS_A08.c(),
    CPU_BUS_A07.c(), CPU_BUS_A06.c(), CPU_BUS_A05.c(), CPU_BUS_A04.c(),
    CPU_BUS_A03.c(), CPU_BUS_A02.c(), CPU_BUS_A01.c(), CPU_BUS_A00.c());

  d("CPU PIN DATA    : %c%c%c%c%c%c%c%c\n",
    CPU_BUS_D7p.c(), CPU_BUS_D6p.c(), CPU_BUS_D5p.c(), CPU_BUS_D4p.c(),
    CPU_BUS_D3p.c(), CPU_BUS_D2p.c(), CPU_BUS_D1p.c(), CPU_BUS_D0p.c());

  d("\n");
}


//------------------------------------------------------------------------------