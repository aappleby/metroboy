#include "Sch_CpuBus.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void CpuBus::tock(const SchematicTop& top) {
  _CPU_PIN_ADDR_HI = SYRO_FE00_FFFFp();
  _CPU_PIN_BOOTp = top.TUTU_ADDR_BOOTp();
}

//------------------------------------------------------------------------------