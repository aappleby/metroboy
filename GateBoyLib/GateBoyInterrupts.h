#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyInterrupts {
  void reset_to_cart() {
    LOPE_FF0F_D0p.reset(REG_D1C1);
    PIN_CPU_INT_VBLANK.reset(REG_D1C0);
  }

  void tock(
    const GateBoyResetDebug& rst,
      GateBoyCpuBus& cpu_bus,
    const GateBoyJoypad& joypad,
    const RegStat& reg_stat,
    const RegLYC& reg_lyc,
    const GateBoySerial& serial,
    wire PARU_VBLANKp,
    wire PURE_LINE_ENDn,
    wire MOBA_TIMER_OVERFLOWp,
    wire WODU_HBLANKp);

  void dump(Dumper& d) {
    d.dump_bitp("IE_D0        ", IE_D0.state);
    d.dump_bitp("IE_D1        ", IE_D1.state);
    d.dump_bitp("IE_D2        ", IE_D2.state);
    d.dump_bitp("IE_D3        ", IE_D3.state);
    d.dump_bitp("IE_D4        ", IE_D4.state);
    d("\n");
    d.dump_bitp("LOPE_FF0F_0  ", LOPE_FF0F_D0p.state);
    d.dump_bitp("LALU_FF0F_1  ", LALU_FF0F_D1p.state);
    d.dump_bitp("NYBO_FF0F_2  ", NYBO_FF0F_D2p.state);
    d.dump_bitp("UBUL_FF0F_3  ", UBUL_FF0F_D3p.state);
    d.dump_bitp("ULAK_FF0F_4  ", ULAK_FF0F_D4p.state);
    d("\n");
    d.dump_bitp("MATY_FF0F_L0p", MATY_FF0F_L0p.state);
    d.dump_bitp("MOPO_FF0F_L1p", MOPO_FF0F_L1p.state);
    d.dump_bitp("PAVY_FF0F_L2p", PAVY_FF0F_L2p.state);
    d.dump_bitp("NEJY_FF0F_L3p", NEJY_FF0F_L3p.state);
    d.dump_bitp("NUTY_FF0F_L4p", NUTY_FF0F_L4p.state);
  }

  /*p02.LOPE*/ DFF22 LOPE_FF0F_D0p;
  /*p02.UBUL*/ DFF22 UBUL_FF0F_D3p;
  /*p02.ULAK*/ DFF22 ULAK_FF0F_D4p;
  /*p02.LALU*/ DFF22 LALU_FF0F_D1p;
  /*p02.NYBO*/ DFF22 NYBO_FF0F_D2p;

  /*p02.MATY*/ TpLatch MATY_FF0F_L0p;
  /*p02.NEJY*/ TpLatch NEJY_FF0F_L3p;
  /*p02.NUTY*/ TpLatch NUTY_FF0F_L4p;
  /*p02.MOPO*/ TpLatch MOPO_FF0F_L1p;
  /*p02.PAVY*/ TpLatch PAVY_FF0F_L2p;

  DFF IE_D0;
  DFF IE_D1;
  DFF IE_D2;
  DFF IE_D3;
  DFF IE_D4;

  PinIn  PIN_CPU_ACK_VBLANK;    // bottom right port PORTB_01: -> P02.LETY, vblank int ack
  PinOut PIN_CPU_INT_VBLANK;    // bottom right port PORTB_03: <- P02.LOPE, vblank int
  PinIn  PIN_CPU_ACK_STAT  ;    // bottom right port PORTB_05: -> P02.LEJA, stat int ack
  PinOut PIN_CPU_INT_STAT  ;    // bottom right port PORTB_07: <- P02.LALU, stat int
  PinIn  PIN_CPU_ACK_TIMER ;    // bottom right port PORTB_09: -> P02.LESA, timer int ack
  PinOut PIN_CPU_INT_TIMER ;    // bottom right port PORTB_11: <- P02.NYBO, timer int
  PinIn  PIN_CPU_ACK_SERIAL;    // bottom right port PORTB_13: -> P02.LUFE, serial int ack
  PinOut PIN_CPU_INT_SERIAL;    // bottom right port PORTB_15: <- P02.UBUL, serial int
  PinIn  PIN_CPU_ACK_JOYPAD;    // bottom right port PORTB_17: -> P02.LAMO, joypad int ack
  PinOut PIN_CPU_INT_JOYPAD;    // bottom right port PORTB_19: <- P02.ULAK, joypad int
};

//------------------------------------------------------------------------------------------------------------------------
