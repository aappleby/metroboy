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

  /*p02.LOPE*/ Reg2 LOPE_FF0F_0 = Reg2::D0C0;
  /*p02.UBUL*/ Reg2 UBUL_FF0F_3 = Reg2::D0C0;
  /*p02.ULAK*/ Reg2 ULAK_FF0F_4 = Reg2::D0C0;
  /*p02.LALU*/ Reg2 LALU_FF0F_1 = Reg2::D0C0;
  /*p02.NYBO*/ Reg2 NYBO_FF0F_2 = Reg2::D0C0;

  /*p02.MATY*/ Reg2 MATY_FF0F_L0 = Reg2::D0C0;
  /*p02.NEJY*/ Reg2 NEJY_FF0F_L1 = Reg2::D0C0;
  /*p02.NUTY*/ Reg2 NUTY_FF0F_L2 = Reg2::D0C0;
  /*p02.MOPO*/ Reg2 MOPO_FF0F_L3 = Reg2::D0C0;
  /*p02.PAVY*/ Reg2 PAVY_FF0F_L4 = Reg2::D0C0;

  // Interrupts
  Pin2 CPU_PIN_INT_VBLANK  = Pin2::HIZ_NP;    // bottom right port PORTB_03: <-        P02.LOPE, vblank int
  Pin2 CPU_PIN_INT_STAT    = Pin2::HIZ_NP;    // bottom right port PORTB_07: <-        P02.LALU, stat int
  Pin2 CPU_PIN_INT_TIMER   = Pin2::HIZ_NP;    // bottom right port PORTB_11: <-        P02.NYBO, timer int
  Pin2 CPU_PIN_INT_SERIAL  = Pin2::HIZ_NP;    // bottom right port PORTB_15: <-        P02.UBUL, serial int
  Pin2 CPU_PIN_INT_JOYPAD  = Pin2::HIZ_NP;    // bottom right port PORTB_19: <-        P02.ULAK, joypad int

  // Interrupt acks
  Pin2  CPU_PIN_ACK_VBLANK = Pin2::HOLD_0;    // bottom right port PORTB_01: ->        P02.LETY, vblank int ack
  Pin2  CPU_PIN_ACK_STAT   = Pin2::HOLD_0;    // bottom right port PORTB_05: ->        P02.LEJA, stat int ack
  Pin2  CPU_PIN_ACK_TIMER  = Pin2::HOLD_0;    // bottom right port PORTB_09: ->        P02.LESA, timer int ack
  Pin2  CPU_PIN_ACK_SERIAL = Pin2::HOLD_0;    // bottom right port PORTB_13: ->        P02.LUFE, serial int ack
  Pin2  CPU_PIN_ACK_JOYPAD = Pin2::HOLD_0;    // bottom right port PORTB_17: ->        P02.LAMO, joypad int ack
};

//-----------------------------------------------------------------------------

}; // namespace Schematics