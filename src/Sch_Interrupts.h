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

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- INT REG -----\n");
    LOPE_FF0F_0.dump(text_painter, "LOPE_FF0F_0  ");
    UBUL_FF0F_3.dump(text_painter, "UBUL_FF0F_3  ");
    ULAK_FF0F_4.dump(text_painter, "ULAK_FF0F_4  ");
    LALU_FF0F_1.dump(text_painter, "LALU_FF0F_1  ");
    NYBO_FF0F_2.dump(text_painter, "NYBO_FF0F_2  ");
    FF0F_L0.dump(text_painter, "FF0F_L0 ");
    FF0F_L1.dump(text_painter, "FF0F_L1 ");
    FF0F_L2.dump(text_painter, "FF0F_L2 ");
    FF0F_L3.dump(text_painter, "FF0F_L3 ");
    FF0F_L4.dump(text_painter, "FF0F_L4 ");
    text_painter.newline();
  }

  /*p02.LOPE*/ Reg LOPE_FF0F_0;
  /*p02.UBUL*/ Reg UBUL_FF0F_3;   // 22-rung?
  /*p02.ULAK*/ Reg ULAK_FF0F_4;
  /*p02.LALU*/ Reg LALU_FF0F_1;
  /*p02.NYBO*/ Reg NYBO_FF0F_2;

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