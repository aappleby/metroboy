#include "GateBoyLib/Sch_Interrupts.h"

#include "GateBoyLib/Probe.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void InterruptRegisters::dump(Dumper& d, const SchematicTop& top) const {
  d("\002===== Ints =====\001\n");
  d("IE_D0              %c\n", top.IE_D0.c());
  d("IE_D1              %c\n", top.IE_D1.c());
  d("IE_D2              %c\n", top.IE_D2.c());
  d("IE_D3              %c\n", top.IE_D3.c());
  d("IE_D4              %c\n", top.IE_D4.c());
  d("\n");

  d("LOPE_FF0F_0        %c\n", LOPE_FF0F_D0p        .c());
  d("LALU_FF0F_1        %c\n", LALU_FF0F_D1p        .c());
  d("NYBO_FF0F_2        %c\n", NYBO_FF0F_D2p        .c());
  d("UBUL_FF0F_3        %c\n", UBUL_FF0F_D3p        .c());
  d("ULAK_FF0F_4        %c\n", ULAK_FF0F_D4p        .c());
  d("\n");
  d("MATY_FF0F_L0p      %c\n", MATY_FF0F_L0p       .c());
  d("MOPO_FF0F_L1p      %c\n", MOPO_FF0F_L1p       .c());
  d("PAVY_FF0F_L2p      %c\n", PAVY_FF0F_L2p       .c());
  d("NEJY_FF0F_L3p      %c\n", NEJY_FF0F_L3p       .c());
  d("NUTY_FF0F_L4p      %c\n", NUTY_FF0F_L4p       .c());
  d("\n");
  d("PIN_CPU_INT_VBLANK %c\n", PIN_CPU_INT_VBLANK .c());
  d("PIN_CPU_INT_STAT   %c\n", PIN_CPU_INT_STAT   .c());
  d("PIN_CPU_INT_TIMER  %c\n", PIN_CPU_INT_TIMER  .c());
  d("PIN_CPU_INT_SERIAL %c\n", PIN_CPU_INT_SERIAL .c());
  d("PIN_CPU_INT_JOYPAD %c\n", PIN_CPU_INT_JOYPAD .c());
  d("\n");
  d("PIN_CPU_ACK_VBLANK %c\n", PIN_CPU_ACK_VBLANK .c());
  d("PIN_CPU_ACK_STAT   %c\n", PIN_CPU_ACK_STAT   .c());
  d("PIN_CPU_ACK_TIMER  %c\n", PIN_CPU_ACK_TIMER  .c());
  d("PIN_CPU_ACK_SERIAL %c\n", PIN_CPU_ACK_SERIAL .c());
  d("PIN_CPU_ACK_JOYPAD %c\n", PIN_CPU_ACK_JOYPAD .c());
  d("\n");
}

//------------------------------------------------------------------------------
