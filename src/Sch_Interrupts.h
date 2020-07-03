#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------
// Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
// Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
// Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
// Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
// Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)

struct InterruptRegisters {
  void tick(SchematicTop& gb);
  SignalHash commit();

private:
  friend SchematicTop;

  /*p02.LOPE*/ Reg22 LOPE_FF0F_0;
  /*p02.UBUL*/ Reg22 UBUL_FF0F_3;   // 22-rung?
  /*p02.ULAK*/ Reg22 ULAK_FF0F_4;
  /*p02.LALU*/ Reg22 LALU_FF0F_1;
  /*p02.NYBO*/ Reg22 NYBO_FF0F_2;

  /*p02.MATY*/ TpLatch FF0F_L0;
  /*p02.NEJY*/ TpLatch FF0F_L1;
  /*p02.NUTY*/ TpLatch FF0F_L2;
  /*p02.MOPO*/ TpLatch FF0F_L3;
  /*p02.PAVY*/ TpLatch FF0F_L4;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics