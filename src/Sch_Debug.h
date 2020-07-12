#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct DebugRegisters {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top);
  SignalHash commit();

private:
  friend SchematicTop;

  /*p25.SOTO*/ Reg17 SOTO_DBG;

  CpuPinOut CPU_PIN_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  CpuPinOut CPU_PIN_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
};

//-----------------------------------------------------------------------------

}; // namespace Schematics