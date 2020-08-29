#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------
// Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
// Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
// Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
// Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
// Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)

struct InterruptRegisters {
  void dump(Dumper& d) const;
  void tick(const SchematicTop& gb);
  void tock(const SchematicTop& gb, CpuBus& cpu_bus);


//private:
  friend SchematicTop;

  /*p02.LOPE*/ Reg LOPE_FF0F_0p = REG_D0C0;
  /*p02.UBUL*/ Reg UBUL_FF0F_3p = REG_D0C0;
  /*p02.ULAK*/ Reg ULAK_FF0F_4p = REG_D0C0;
  /*p02.LALU*/ Reg LALU_FF0F_1p = REG_D0C0;
  /*p02.NYBO*/ Reg NYBO_FF0F_2p = REG_D0C0;

  /*p02.MATY*/ Latch MATY_FF0F_L0p = TRI_D0NP;
  /*p02.NEJY*/ Latch NEJY_FF0F_L3p = TRI_D0NP;
  /*p02.NUTY*/ Latch NUTY_FF0F_L4p = TRI_D0NP;
  /*p02.MOPO*/ Latch MOPO_FF0F_L1p = TRI_D0NP;
  /*p02.PAVY*/ Latch PAVY_FF0F_L2p = TRI_D0NP;

  // Interrupts
  Tri CPU_PIN_INT_VBLANK  = TRI_HZNP;    // bottom right port PORTB_03: <-        P02.LOPE, vblank int
  Tri CPU_PIN_INT_STAT    = TRI_HZNP;    // bottom right port PORTB_07: <-        P02.LALU, stat int
  Tri CPU_PIN_INT_TIMER   = TRI_HZNP;    // bottom right port PORTB_11: <-        P02.NYBO, timer int
  Tri CPU_PIN_INT_SERIAL  = TRI_HZNP;    // bottom right port PORTB_15: <-        P02.UBUL, serial int
  Tri CPU_PIN_INT_JOYPAD  = TRI_HZNP;    // bottom right port PORTB_19: <-        P02.ULAK, joypad int

  // Interrupt acks
  Tri CPU_PIN_ACK_VBLANK = TRI_D0NP;    // bottom right port PORTB_01: ->        P02.LETY, vblank int ack
  Tri CPU_PIN_ACK_STAT   = TRI_D0NP;    // bottom right port PORTB_05: ->        P02.LEJA, stat int ack
  Tri CPU_PIN_ACK_TIMER  = TRI_D0NP;    // bottom right port PORTB_09: ->        P02.LESA, timer int ack
  Tri CPU_PIN_ACK_SERIAL = TRI_D0NP;    // bottom right port PORTB_13: ->        P02.LUFE, serial int ack
  Tri CPU_PIN_ACK_JOYPAD = TRI_D0NP;    // bottom right port PORTB_17: ->        P02.LAMO, joypad int ack
};

//-----------------------------------------------------------------------------

}; // namespace Schematics