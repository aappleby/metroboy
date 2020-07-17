#pragma once
#include "Cells.h"

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
  void tick(const SchematicTop& gb);
  void tock(const SchematicTop& gb, CpuBus& cpu_bus);
  SignalHash commit();

private:
  friend SchematicTop;

  /*p02.LOPE*/ Reg LOPE_FF0F_0;
  /*p02.UBUL*/ Reg UBUL_FF0F_3;
  /*p02.ULAK*/ Reg ULAK_FF0F_4;
  /*p02.LALU*/ Reg LALU_FF0F_1;
  /*p02.NYBO*/ Reg NYBO_FF0F_2;

  /*p02.MATY*/ Reg MATY_FF0F_L0;
  /*p02.NEJY*/ Reg NEJY_FF0F_L1;
  /*p02.NUTY*/ Reg NUTY_FF0F_L2;
  /*p02.MOPO*/ Reg MOPO_FF0F_L3;
  /*p02.PAVY*/ Reg PAVY_FF0F_L4;

  // Interrupts
  Reg CPU_PIN_INT_VBLANK;    // bottom right port PORTB_03: <-        P02.LOPE, vblank int
  Reg CPU_PIN_INT_STAT;      // bottom right port PORTB_07: <-        P02.LALU, stat int
  Reg CPU_PIN_INT_TIMER;     // bottom right port PORTB_11: <-        P02.NYBO, timer int
  Reg CPU_PIN_INT_SERIAL;    // bottom right port PORTB_15: <-        P02.UBUL, serial int
  Reg CPU_PIN_INT_JOYPAD;    // bottom right port PORTB_19: <-        P02.ULAK, joypad int

  // Interrupt acks
  CpuPinIn  CPU_PIN_ACK_VBLANK;    // bottom right port PORTB_01: ->        P02.LETY, vblank int ack
  CpuPinIn  CPU_PIN_ACK_STAT;      // bottom right port PORTB_05: ->        P02.LEJA, stat int ack
  CpuPinIn  CPU_PIN_ACK_TIMER;     // bottom right port PORTB_09: ->        P02.LESA, timer int ack
  CpuPinIn  CPU_PIN_ACK_SERIAL;    // bottom right port PORTB_13: ->        P02.LUFE, serial int ack
  CpuPinIn  CPU_PIN_ACK_JOYPAD;    // bottom right port PORTB_17: ->        P02.LAMO, joypad int ack
};

//-----------------------------------------------------------------------------

}; // namespace Schematics