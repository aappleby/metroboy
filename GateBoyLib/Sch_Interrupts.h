#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct InterruptRegisters {
  void reset() {}
  void reset_bootrom() {
    PIN_CPU_ACK_VBLANK.lock(0);
    PIN_CPU_ACK_STAT  .lock(0);
    PIN_CPU_ACK_TIMER .lock(0);
    PIN_CPU_ACK_SERIAL.lock(0);
    PIN_CPU_ACK_JOYPAD.lock(0);
  }
  void dump(Dumper& d, const SchematicTop& top) const;
  void tock(const SchematicTop& gb, CpuBus& cpu_bus);

  void force_set_if(uint8_t _if) {
    LOPE_FF0F_D0p.reset(_if & 0x01);
    LALU_FF0F_D1p.reset(_if & 0x02);
    NYBO_FF0F_D2p.reset(_if & 0x04);
    UBUL_FF0F_D3p.reset(_if & 0x08);
    ULAK_FF0F_D4p.reset(_if & 0x10);
  }

//private:
  friend SchematicTop;

  /*p02.LOPE*/ DFF22 LOPE_FF0F_D0p;
  /*p02.UBUL*/ DFF22 UBUL_FF0F_D3p;
  /*p02.ULAK*/ DFF22 ULAK_FF0F_D4p;
  /*p02.LALU*/ DFF22 LALU_FF0F_D1p;
  /*p02.NYBO*/ DFF22 NYBO_FF0F_D2p;

  /*p02.MATY*/ TpLatch MATY_FF0F_L0p = TRI_D0NP;
  /*p02.NEJY*/ TpLatch NEJY_FF0F_L3p = TRI_D0NP;
  /*p02.NUTY*/ TpLatch NUTY_FF0F_L4p = TRI_D0NP;
  /*p02.MOPO*/ TpLatch MOPO_FF0F_L1p = TRI_D0NP;
  /*p02.PAVY*/ TpLatch PAVY_FF0F_L2p = TRI_D0NP;

  // Interrupts
  Pin PIN_CPU_INT_VBLANK  = TRI_HZNP; // bottom right port PORTB_03: <- P02.LOPE, vblank int
  Pin PIN_CPU_INT_STAT    = TRI_HZNP; // bottom right port PORTB_07: <- P02.LALU, stat int
  Pin PIN_CPU_INT_TIMER   = TRI_HZNP; // bottom right port PORTB_11: <- P02.NYBO, timer int
  Pin PIN_CPU_INT_SERIAL  = TRI_HZNP; // bottom right port PORTB_15: <- P02.UBUL, serial int
  Pin PIN_CPU_INT_JOYPAD  = TRI_HZNP; // bottom right port PORTB_19: <- P02.ULAK, joypad int

  // Interrupt acks
  Pin PIN_CPU_ACK_VBLANK = TRI_D0NP;  // bottom right port PORTB_01: -> P02.LETY, vblank int ack
  Pin PIN_CPU_ACK_STAT   = TRI_D0NP;  // bottom right port PORTB_05: -> P02.LEJA, stat int ack
  Pin PIN_CPU_ACK_TIMER  = TRI_D0NP;  // bottom right port PORTB_09: -> P02.LESA, timer int ack
  Pin PIN_CPU_ACK_SERIAL = TRI_D0NP;  // bottom right port PORTB_13: -> P02.LUFE, serial int ack
  Pin PIN_CPU_ACK_JOYPAD = TRI_D0NP;  // bottom right port PORTB_17: -> P02.LAMO, joypad int ack
};

//-----------------------------------------------------------------------------

}; // namespace Schematics