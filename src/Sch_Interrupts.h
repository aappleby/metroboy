#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------
// Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
// Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
// Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
// Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
// Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)

struct InterruptRegisters {
  void tick(TestGB& gb);
  bool commit();

private:

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

  PinOut PIN_INT_VBLANK;    // bottom right port PORTB_03: <-        P02.LOPE, vblank int
  PinOut PIN_INT_STAT;      // bottom right port PORTB_07: <-        P02.LALU, stat int
  PinOut PIN_INT_TIMER;     // bottom right port PORTB_11: <-        P02.NYBO, timer int
  PinOut PIN_INT_SERIAL;    // bottom right port PORTB_15: <-        P02.UBUL, serial int
  PinOut PIN_INT_JOYPAD;    // bottom right port PORTB_19: <-        P02.ULAK, joypad int

  PinIn  PIN_ACK_VBLANK;    // bottom right port PORTB_01: ->        P02.LETY, vblank int ack
  PinIn  PIN_ACK_STAT;      // bottom right port PORTB_05: ->        P02.LEJA, stat int ack
  PinIn  PIN_ACK_TIMER;     // bottom right port PORTB_09: ->        P02.LESA, timer int ack
  PinIn  PIN_ACK_SERIAL;    // bottom right port PORTB_13: ->        P02.LUFE, serial int ack
  PinIn  PIN_ACK_JOYPAD;    // bottom right port PORTB_17: ->        P02.LAMO, joypad int ack
};

//-----------------------------------------------------------------------------

}; // namespace Schematics