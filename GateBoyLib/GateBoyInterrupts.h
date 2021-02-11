#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyInterrupts {
  void reset_to_cart() {
    LOPE_FF0F_D0p.state = 0b00011011;
    UBUL_FF0F_D3p.state = 0b00011000;
    ULAK_FF0F_D4p.state = 0b00011000;
    LALU_FF0F_D1p.state = 0b00011000;
    NYBO_FF0F_D2p.state = 0b00011000;
    MATY_FF0F_L0p.state = 0b00011000;
    NEJY_FF0F_L3p.state = 0b00011000;
    NUTY_FF0F_L4p.state = 0b00011000;
    MOPO_FF0F_L1p.state = 0b00011000;
    PAVY_FF0F_L2p.state = 0b00011000;
    IE_D0.state = 0b00011010;
    IE_D1.state = 0b00011010;
    IE_D2.state = 0b00011010;
    IE_D3.state = 0b00011010;
    IE_D4.state = 0b00011010;
    SIG_CPU_INT_VBLANK.state = 0b00011001;
    SIG_CPU_INT_STAT.state = 0b00011000;
    SIG_CPU_INT_TIMER.state = 0b00011000;
    SIG_CPU_INT_SERIAL.state = 0b00011000;
    SIG_CPU_INT_JOYPAD.state = 0b00011000;
    SIG_CPU_ACK_VBLANK.state = 0b00011000;
    SIG_CPU_ACK_STAT.state = 0b00011000;
    SIG_CPU_ACK_TIMER.state = 0b00011000;
    SIG_CPU_ACK_SERIAL.state = 0b00011000;
    SIG_CPU_ACK_JOYPAD.state = 0b00011000;
  }

  /*p02.LOPE*/ DFF22 LOPE_FF0F_D0p;
  /*p02.LALU*/ DFF22 LALU_FF0F_D1p;
  /*p02.NYBO*/ DFF22 NYBO_FF0F_D2p;
  /*p02.UBUL*/ DFF22 UBUL_FF0F_D3p;
  /*p02.ULAK*/ DFF22 ULAK_FF0F_D4p;

  /*p02.MATY*/ TpLatch MATY_FF0F_L0p;
  /*p02.MOPO*/ TpLatch MOPO_FF0F_L1p;
  /*p02.PAVY*/ TpLatch PAVY_FF0F_L2p;
  /*p02.NEJY*/ TpLatch NEJY_FF0F_L3p;
  /*p02.NUTY*/ TpLatch NUTY_FF0F_L4p;

  // This is technically in the CPU, but we're going to implement it here for now.
  DFF IE_D0;
  DFF IE_D1;
  DFF IE_D2;
  DFF IE_D3;
  DFF IE_D4;

  /*SIG_CPU_INT_VBLANK*/ SigOut SIG_CPU_INT_VBLANK;    // bottom right port PORTB_03: <- P02.LOPE, vblank int
  /*SIG_CPU_INT_STAT  */ SigOut SIG_CPU_INT_STAT  ;    // bottom right port PORTB_07: <- P02.LALU, stat int
  /*SIG_CPU_INT_TIMER */ SigOut SIG_CPU_INT_TIMER ;    // bottom right port PORTB_11: <- P02.NYBO, timer int
  /*SIG_CPU_INT_SERIAL*/ SigOut SIG_CPU_INT_SERIAL;    // bottom right port PORTB_15: <- P02.UBUL, serial int
  /*SIG_CPU_INT_JOYPAD*/ SigOut SIG_CPU_INT_JOYPAD;    // bottom right port PORTB_19: <- P02.ULAK, joy int

  /*SIG_CPU_ACK_VBLANK*/ SigIn  SIG_CPU_ACK_VBLANK;    // bottom right port PORTB_01: -> P02.LETY, vblank int ack
  /*SIG_CPU_ACK_STAT  */ SigIn  SIG_CPU_ACK_STAT  ;    // bottom right port PORTB_05: -> P02.LEJA, stat int ack
  /*SIG_CPU_ACK_TIMER */ SigIn  SIG_CPU_ACK_TIMER ;    // bottom right port PORTB_09: -> P02.LESA, timer int ack
  /*SIG_CPU_ACK_SERIAL*/ SigIn  SIG_CPU_ACK_SERIAL;    // bottom right port PORTB_13: -> P02.LUFE, serial int ack
  /*SIG_CPU_ACK_JOYPAD*/ SigIn  SIG_CPU_ACK_JOYPAD;    // bottom right port PORTB_17: -> P02.LAMO, joy int ack
};

//------------------------------------------------------------------------------------------------------------------------
