#pragma once

struct CpuSignals {

  //----------
  // top right wire by itself

  // CPU_? - AWOB - something about interrupts. is this the master interrupt line?
  // probably "unstop"?
  // joypad, P13_C|P12|P11_C|P10
  // NMIn?
  // TO_CPU2 = AWOB_Q;
  // there's another dangly bit right next to this wire, it goes to another unconnected pad
  bool TO_CPU2;        // <- P02.ASOK

  //----------
  // top right port

  bool CPU_RAW_RD;     // PORTA_00: -> P07.UJYV, P08.LAGU, P08.LAVO
  bool FROM_CPU3;      // PORTA_01: -> P01.AREV, P08.LAGU. This is almost definitely "raw write"
  bool T1nT2;          // PORTA_02: <- P07.T1nT2
  bool SYRO;           // PORTA_03: <- P25.SYRO
  bool TUTU;           // PORTA_04: <- P07.TUTU
  bool T1T2n;          // PORTA_05: <- P07.T1T2n
  bool FROM_CPU4;      // PORTA_06: -> P01.AGUT, P08.TEX0. This is almost definitely "address valid"

  //----------
  // 32 signals coming off the bottom right, some are ground

  bool A00;            // PORTB_00: -> A00
  bool FROM_CPU9;      // PORTB_01: -> P02.LETY, vblank int ack
  bool A08;            // PORTB_02: -> A08
  bool TO_CPU3;        // PORTB_03: <- P02.LOPE, vblank int
  bool A01;            // PORTB_04: -> A01
  bool FROM_CPU8;      // PORTB_05: -> P02.LEJA, stat int ack
  bool A09;            // PORTB_06: -> A09
  bool TO_CPU6;        // PORTB_07: <- P02.LALU, stat int
  bool A02;            // PORTB_08: -> A02
  bool FROM_CPU10;     // PORTB_09: -> P02.LESA, timer int ack
  bool A10;            // PORTB_10: -> A10
  bool TO_CPU7;        // PORTB_11: <- P02.NYBO, timer int
  bool A03;            // PORTB_12: -> A03
  bool FROM_CPU7;      // PORTB_13: -> P02.LUFE, serial int ack
  bool A11;            // PORTB_14: -> A11
  bool TO_CPU4;        // PORTB_15: <- P02.UBUL, serial int
  bool A04;            // PORTB_16: -> A04
  bool FROM_CPU11;     // PORTB_17: -> P02.LAMO, joypad int ack
  bool A12;            // PORTB_18: -> A12
  bool TO_CPU5;        // PORTB_19: <- P02.ULAK, joypad int
  bool A05;            // PORTB_20: -> A05
  /*GND*/              // PORTB_21: <- GND
  bool A13;            // PORTB_22: -> A13
  /*GND*/              // PORTB_23: <- GND
  bool A06;            // PORTB_24: -> A06
  /*GND*/              // PORTB_25: <- GND
  bool A14;            // PORTB_26: -> A14
  /*GND*/              // PORTB_27: <- GND
  bool A07;            // PORTB_28: -> A07
  /*GND*/              // PORTB_29: <- GND
  bool A15;            // PORTB_30: -> A15
  /*GND*/              // PORTB_31: <- GND

  //----------
  // top center, 5 signals, numbered left to right

  bool TO_ABOL;        // PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  bool AFER;           // PORTC_01: <- P01.AFER , something reset related
  bool PIN_RESET;      // PORTC_02: <- PIN_RESET directly connected to the pad
  bool CLKIN_A;        // PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  bool TABA;           // PORTC_04: <- P01.TABA, more reset stuff

  // I think all the clocks that go to the CPU are downstream of ABOL, so yeah ABOL could be
  // clock request

  //----------
  // top left, 10? signals... i think i screwed this up, my counting was off
  // these are mostly the clocks

  bool FROM_CPU6;      // PORTD_00: -> P07.LEXY, doesn't do anything
  bool TO_CPU;         // PORTD_01: <- P01.BOWA, this is a 1 mhz clock
  bool BEDO;           // PORTD_02: <- P01.BEDO, this is a 1 mhz clock
  bool BEKO;           // PORTD_03: <- P01.BEKO, connection not indicated on P01. Clock but not sure what phase
  bool PHI_OUT;        // PORTD_04: <- P01.BUDE, PHI_A, PHI_OUT. More clocks
  bool FROM_CPU5;      // PORTD_05: -> FROM_CPU5 - controls driving the external data pins onto the internal data bus and other stuff. is this actually a clock, or like OE?
  bool BUKE;           // PORTD_06: <- P01.BUKE, 4 mhz clock
  bool BOMA;           // PORTD_07: <- P01.BOMA, connection not indicated on P01, 1 mhz clock
  bool BOGA1MHZ;       // PORTD_08: <- P01.BOGA1MHZ, clock

  //----------
  // bottom left, 8 signals, data bus

  bool D0,D1,D2,D3,D4,D5,D6,D7;
};