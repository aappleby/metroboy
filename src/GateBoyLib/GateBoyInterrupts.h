#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct RegIF {
  void reset_to_cart() {
    LOPE_FF0F_D0p.state = 0b00011011;
    LALU_FF0F_D1p.state = 0b00011000;
    NYBO_FF0F_D2p.state = 0b00011000;
    UBUL_FF0F_D3p.state = 0b00011000;
    ULAK_FF0F_D4p.state = 0b00011000;
  }

  /*_p02.LOPE*/ DFF22 LOPE_FF0F_D0p;
  /*_p02.LALU*/ DFF22 LALU_FF0F_D1p;
  /*_p02.NYBO*/ DFF22 NYBO_FF0F_D2p;
  /*_p02.UBUL*/ DFF22 UBUL_FF0F_D3p;
  /*_p02.ULAK*/ DFF22 ULAK_FF0F_D4p;
};

// This is technically in the CPU, but we're going to implement it here for now.
struct RegIE {
  void reset_to_cart() {
    IE_D0.state = 0b00011010;
    IE_D1.state = 0b00011010;
    IE_D2.state = 0b00011010;
    IE_D3.state = 0b00011010;
    IE_D4.state = 0b00011010;
  }

  DFF IE_D0;
  DFF IE_D1;
  DFF IE_D2;
  DFF IE_D3;
  DFF IE_D4;
};

struct InterruptLatch {
  void reset_to_cart() {
    MATY_FF0F_L0p.state = 0b00011000;
    MOPO_FF0F_L1p.state = 0b00011000;
    PAVY_FF0F_L2p.state = 0b00011000;
    NEJY_FF0F_L3p.state = 0b00011000;
    NUTY_FF0F_L4p.state = 0b00011000;
  }

  /*_p02.MATY*/ TpLatch MATY_FF0F_L0p;
  /*_p02.MOPO*/ TpLatch MOPO_FF0F_L1p;
  /*_p02.PAVY*/ TpLatch PAVY_FF0F_L2p;
  /*_p02.NEJY*/ TpLatch NEJY_FF0F_L3p;
  /*_p02.NUTY*/ TpLatch NUTY_FF0F_L4p;
};

struct CpuInt {
  void reset_to_cart() {
    SIG_CPU_INT_VBLANK.state = 0b00011001;
    SIG_CPU_INT_STAT.state   = 0b00011000;
    SIG_CPU_INT_TIMER.state  = 0b00011000;
    SIG_CPU_INT_SERIAL.state = 0b00011000;
    SIG_CPU_INT_JOYPAD.state = 0b00011000;
  }

  /*_SIG_CPU_INT_VBLANK*/ SigOut SIG_CPU_INT_VBLANK;    // bottom right port PORTB_03: <- P02.LOPE, vblank int
  /*_SIG_CPU_INT_STAT  */ SigOut SIG_CPU_INT_STAT  ;    // bottom right port PORTB_07: <- P02.LALU, stat int
  /*_SIG_CPU_INT_TIMER */ SigOut SIG_CPU_INT_TIMER ;    // bottom right port PORTB_11: <- P02.NYBO, timer int
  /*_SIG_CPU_INT_SERIAL*/ SigOut SIG_CPU_INT_SERIAL;    // bottom right port PORTB_15: <- P02.UBUL, serial int
  /*_SIG_CPU_INT_JOYPAD*/ SigOut SIG_CPU_INT_JOYPAD;    // bottom right port PORTB_19: <- P02.ULAK, joy int
};

struct CpuAck {
  void reset_to_cart() {
    SIG_CPU_ACK_VBLANK.state = 0b00011000;
    SIG_CPU_ACK_STAT.state   = 0b00011000;
    SIG_CPU_ACK_TIMER.state  = 0b00011000;
    SIG_CPU_ACK_SERIAL.state = 0b00011000;
    SIG_CPU_ACK_JOYPAD.state = 0b00011000;
  }

  /*_SIG_CPU_ACK_VBLANK*/ SigIn  SIG_CPU_ACK_VBLANK;    // bottom right port PORTB_01: -> P02.LETY, vblank int ack
  /*_SIG_CPU_ACK_STAT  */ SigIn  SIG_CPU_ACK_STAT  ;    // bottom right port PORTB_05: -> P02.LEJA, stat int ack
  /*_SIG_CPU_ACK_TIMER */ SigIn  SIG_CPU_ACK_TIMER ;    // bottom right port PORTB_09: -> P02.LESA, timer int ack
  /*_SIG_CPU_ACK_SERIAL*/ SigIn  SIG_CPU_ACK_SERIAL;    // bottom right port PORTB_13: -> P02.LUFE, serial int ack
  /*_SIG_CPU_ACK_JOYPAD*/ SigIn  SIG_CPU_ACK_JOYPAD;    // bottom right port PORTB_17: -> P02.LAMO, joy int ack
};

struct InterruptControl {
  // This is driven by what we think is a latch and it goes straight to the CPU - maybe there's a pull-down?
  /*_p02.AWOB*/ TpLatch AWOB_WAKE_CPU;
  /*_SIG_CPU_WAKE*/ SigOut SIG_CPU_WAKE;  // top right wire by itself <- P02.AWOB

  /*#p03.NYDU*/ DFF17 NYDU_TIMA7p_DELAY;    // Axxxxxxx
  /*#p03.MOBA*/ DFF17 MOBA_TIMER_OVERFLOWp; // AxxxExxx

  /*_p21.RUPO*/ NorLatch RUPO_LYC_MATCHn;
  /*#p21.ROPO*/ DFF17 ROPO_LY_MATCH_SYNCp;   // xxCxxxxx
};

//-----------------------------------------------------------------------------
