#include "GateBoyLib/Sch_CpuBus.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void CpuBus::dump(Dumper& d) const {
  d("---------- CPU Bus  ----------\n");
  d("BOOT      : %d\n", CPU_PIN_BOOTp.tp());
  d("ADDR_HI   : %d\n", CPU_PIN_ADDR_HI.tp());
  d("RDp       : %d\n", CPU_PIN_RDp.tp());
  d("WRp       : %d\n", CPU_PIN_WRp.tp());
  d("ADDR_EXT  : %d\n", CPU_PIN_ADDR_EXTp.tp());
  d("LATCH_EXT : %d\n", CPU_PIN_LATCH_EXT.tp());

  d("BUS ADDR  : 0x%04x %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    get_bus_addr(),
    CPU_BUS_A15.c(), CPU_BUS_A14.c(), CPU_BUS_A13.c(), CPU_BUS_A12.c(),
    CPU_BUS_A11.c(), CPU_BUS_A10.c(), CPU_BUS_A09.c(), CPU_BUS_A08.c(),
    CPU_BUS_A07.c(), CPU_BUS_A06.c(), CPU_BUS_A05.c(), CPU_BUS_A04.c(),
    CPU_BUS_A03.c(), CPU_BUS_A02.c(), CPU_BUS_A01.c(), CPU_BUS_A00.c());

  d("BUS DATA  : 0x%02x   %c%c%c%c%c%c%c%c\n",
    get_bus_data(),
    CPU_BUS_D7p.c(), CPU_BUS_D6p.c(), CPU_BUS_D5p.c(), CPU_BUS_D4p.c(),
    CPU_BUS_D3p.c(), CPU_BUS_D2p.c(), CPU_BUS_D1p.c(), CPU_BUS_D0p.c());

  d("\n");

  d("PIN STARTp        : %d\n", CPU_PIN_STARTp.tp());
  d("PIN SYS_RSTp      : %d\n", CPU_PIN_SYS_RSTp.tp());
  d("PIN EXT_RST       : %d\n", CPU_PIN_EXT_RST.tp());
  d("PIN UNOR_DBG      : %d\n", CPU_PIN_UNOR_DBG.tp());
  d("PIN UMUT_DBG      : %d\n", CPU_PIN_UMUT_DBG.tp());
  d("PIN EXT_CLKGOOD   : %d\n", CPU_PIN_EXT_CLKGOOD.tp());
  d("PIN BOWA_xBCDEFGH : %d\n", CPU_PIN_BOWA_xBCDEFGH.tp());
  d("PIN BEDO_Axxxxxxx : %d\n", CPU_PIN_BEDO_Axxxxxxx.tp());
  d("PIN BEKO_ABCDxxxx : %d\n", CPU_PIN_BEKO_ABCDxxxx.tp());
  d("PIN BUDE_xxxxEFGH : %d\n", CPU_PIN_BUDE_xxxxEFGH.tp());
  d("PIN BOLO_ABCDEFxx : %d\n", CPU_PIN_BOLO_ABCDEFxx.tp());
  d("PIN BUKE_AxxxxxGH : %d\n", CPU_PIN_BUKE_AxxxxxGH.tp());
  d("PIN BOMA_Axxxxxxx : %d\n", CPU_PIN_BOMA_Axxxxxxx.tp());
  d("PIN BOGA_xBCDEFGH : %d\n", CPU_PIN_BOGA_xBCDEFGH.tp());
  d("\n");
}


//------------------------------------------------------------------------------