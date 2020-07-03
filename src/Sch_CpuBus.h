#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct CpuBus {

  void tick(SchematicTop& gb);
  SignalHash commit();

  //----------
  // bottom left port, tristate data bus

private:

  //----------
  // Ext-to-cpu "latch" - looks more like a pass gate really

  /*p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_00;
  /*p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_01;
  /*p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_02;
  /*p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_03;
  /*p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_04;
  /*p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_05;
  /*p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_06;
  /*p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_07;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics