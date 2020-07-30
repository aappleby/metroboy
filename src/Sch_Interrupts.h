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

  void dump(Dumper& d) {
    d("----------   Ints   ----------\n");
    d("LOPE_FF0F_0        %c\n", LOPE_FF0F_0        .c());
    d("UBUL_FF0F_3        %c\n", UBUL_FF0F_3        .c());
    d("ULAK_FF0F_4        %c\n", ULAK_FF0F_4        .c());
    d("LALU_FF0F_1        %c\n", LALU_FF0F_1        .c());
    d("NYBO_FF0F_2        %c\n", NYBO_FF0F_2        .c());
    d("MATY_FF0F_L0       %c\n", MATY_FF0F_L0       .c());
    d("NEJY_FF0F_L1       %c\n", NEJY_FF0F_L1       .c());
    d("NUTY_FF0F_L2       %c\n", NUTY_FF0F_L2       .c());
    d("MOPO_FF0F_L3       %c\n", MOPO_FF0F_L3       .c());
    d("PAVY_FF0F_L4       %c\n", PAVY_FF0F_L4       .c());
    d("CPU_PIN_INT_VBLANK %c\n", CPU_PIN_INT_VBLANK .c());
    d("CPU_PIN_INT_STAT   %c\n", CPU_PIN_INT_STAT   .c());
    d("CPU_PIN_INT_TIMER  %c\n", CPU_PIN_INT_TIMER  .c());
    d("CPU_PIN_INT_SERIAL %c\n", CPU_PIN_INT_SERIAL .c());
    d("CPU_PIN_INT_JOYPAD %c\n", CPU_PIN_INT_JOYPAD .c());
    d("CPU_PIN_ACK_VBLANK %c\n", CPU_PIN_ACK_VBLANK .c());
    d("CPU_PIN_ACK_STAT   %c\n", CPU_PIN_ACK_STAT   .c());
    d("CPU_PIN_ACK_TIMER  %c\n", CPU_PIN_ACK_TIMER  .c());
    d("CPU_PIN_ACK_SERIAL %c\n", CPU_PIN_ACK_SERIAL .c());
    d("CPU_PIN_ACK_JOYPAD %c\n", CPU_PIN_ACK_JOYPAD .c());
    d("\n");
  }

//private:
  friend SchematicTop;

  /*p02.LOPE*/ Reg LOPE_FF0F_0 = REG_D0C0;
  /*p02.UBUL*/ Reg UBUL_FF0F_3 = REG_D0C0;
  /*p02.ULAK*/ Reg ULAK_FF0F_4 = REG_D0C0;
  /*p02.LALU*/ Reg LALU_FF0F_1 = REG_D0C0;
  /*p02.NYBO*/ Reg NYBO_FF0F_2 = REG_D0C0;

  /*p02.MATY*/ Tri MATY_FF0F_L0 = TRI_D0NP;
  /*p02.NEJY*/ Tri NEJY_FF0F_L1 = TRI_D0NP;
  /*p02.NUTY*/ Tri NUTY_FF0F_L2 = TRI_D0NP;
  /*p02.MOPO*/ Tri MOPO_FF0F_L3 = TRI_D0NP;
  /*p02.PAVY*/ Tri PAVY_FF0F_L4 = TRI_D0NP;

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