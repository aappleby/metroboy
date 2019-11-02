#pragma once

struct CpuSignals {

  //----------
  // top right wire by itself

  // CPU_? - AWOB - something about interrupts. is this the master interrupt line?
  // probably "unstop"?
  // joypad, P13_C|P12|P11_C|P10
  // NMIn?
  // TO_CPU2 = AWOB_Q;
  bool TO_CPU2;

  // there's another dangly bit right next to this wire, it goes to another unconnected pad

  //----------
  // top right port

  bool CPU_RAW_RD; // PORTA_00: -> CPU_RAW_RD
  bool FROM_CPU3;  // PORTA_01: -> FROM_CPU3
  bool T1nT2;      // PORTA_02: <- T1nT2
  bool SYRO;       // PORTA_03: <- SYRO
  bool TUTU;       // PORTA_04: <- TUTU
  bool T1T2n;      // PORTA_05: <- T1T2n
  bool FROM_CPU4;  // PORTA_06: -> FROM_CPU4

  // UJYV = mux2(RD_B, CPU_RAW_RD, T1nT2);
  // LAGU = unk3(CPU_RAW_RD, LEVO, FROM_CPU3);
  // LAVO = nand(CPU_RAW_RD, TEXO, FROM_CPU5);
  // LAGU = unk3(CPU_RAW_RD, LEVO, FROM_CPU3);
  // AREV = nand(FROM_CPU3, AFAS);
  // wire SYRO = not(FEXXFFXXn);
  // wire TEFA = nor(SYRO, TEXO);
  // tutu is the use bootrom signal... but that doesn't make any sense
  // FROM_CPU4 controls driving the external data pins onto the internal data bus
  // FROM_CPU4 can trigger tima reload? weird
  // drives CATY
  // CATY -> BOFE -> BOTA -> ASYT -> BODE -> CLK3
  // CATY -> MYNU -> LEKO -> TYVY -> SEBY, something about putting vram data bus on internal data bus
  // CATY -> MYNU -> LEKO -> GUKO/WUKU -> WUME/WEWU
  // stored in MAKA

  //----------
  // 32 signals coming off the bottom right, some are ground

  bool A0;             // PORTB_00: A0
  bool FROM_CPU9;      // PORTB_01: ->LETY, vblank int ack
  bool A8;             // PORTB_02: A8
  bool TO_CPU3;        // PORTB_03: <-LOPE_Q, vblank int
  bool A1;             // PORTB_04: A1
  bool FROM_CPU8;      // PORTB_05: ->LEJA, stat int ack
  bool A9;             // PORTB_06: A9
  bool TO_CPU6;        // PORTB_07: <-LALU_Q, stat int
  bool A2;             // PORTB_08: A2
  bool FROM_CPU10;     // PORTB_09: ->LESA, timer int ack
  bool A10;            // PORTB_10: A10
  bool TO_CPU7;        // PORTB_11: <-NYBO_Q, timer int
  bool A3;             // PORTB_12: A3
  bool FROM_CPU7;      // PORTB_13: ->LUFE, serial int ack
  bool A11;            // PORTB_14: A11
  bool TO_CPU4;        // PORTB_15: <-UBUL_Q, serial int
  bool A4;             // PORTB_16: A4
  bool FROM_CPU11;     // PORTB_17: ->LAMO, joypad int ack
  bool A12;            // PORTB_18: A12
  bool TO_CPU5;        // PORTB_19: <-ULAK_Q, joypad int
  bool A5;             // PORTB_20: A5
  /*GND*/              // PORTB_21: GND
  bool A13;            // PORTB_22: A13
  /*GND*/              // PORTB_23: GND
  bool A6;             // PORTB_24: A6
  /*GND*/              // PORTB_25: GND
  bool A14;            // PORTB_26: A14
  /*GND*/              // PORTB_27: GND
  bool A7;             // PORTB_28: A7
  /*GND*/              // PORTB_29: GND
  bool A15;            // PORTB_30: A15
  /*GND*/              // PORTB_31: GND

  //----------
  // top center, 5 signals, numbered left to right

  bool TO_ABOL;        // PORTC_00: ->ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  bool AFER;           // PORTC_01: <-AFER , something reset related
  bool PIN_RESET;      // PORTC_02: <-PIN_RESET directly connected to the pad
  bool CLKIN_A;        // PORTC_03: <-CLKIN_A top wire on PAD_XI,
  bool TABA;           // PORTC_04: <-TABA, more reset stuff

  // I think all the clocks that go to the CPU are downstream of ABOL, so yeah ABOL could be
  // clock request

  //----------
  // top left, 10? signals... i think i screwed this up, my counting was off
  // these are mostly the clocks

  bool PIN_NC;         // PORTD_00: -> LEXY, FROM_CPU6 -> goes to an unconnected pad between LD0 and CPG
  bool TO_CPU;         // PORTD_01: <- BOWA
  bool BEDO;           // PORTD_02: <- BEDO
  bool BEKO;           // PORTD_03: <- BEKO, connection not indicated on P01
  bool PHI_OUT;        // PORTD_04: <- BUDE, PHI_A, PHI_OUT
  bool FROM_CPU5;      // PORTD_05: -> FROM_CPU5 - controls driving the external data pins onto the internal data bus and other stuff. is this actually a clock?
  bool BUKE;           // PORTD_06: <- BUKE
  bool BOMA;           // PORTD_07: <- BOMA, connection not indicated on P01
  bool BOGA1MHZ;       // PORTD_08: <- BOGA1MHZ

  //----------
  // bottom left, 8 signals, data bus

  bool D0,D1,D2,D3,D4,D5,D6,D7;
};