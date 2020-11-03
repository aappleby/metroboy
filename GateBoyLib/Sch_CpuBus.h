#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct CpuBus {

  //------------------------------------------------------------------------------

  void dump(Dumper& d) const {
    d("\002===== CPU Bus =====\001\n");
    d("PIN_CPU_BOOTp     %c\n", PIN_CPU_BOOTp.c());
    d("PIN_CPU_ADDR_HIp  %c\n", PIN_CPU_ADDR_HIp.c());
    d("PIN_CPU_RDp       %c\n", PIN_CPU_RDp.c());
    d("PIN_CPU_WRp       %c\n", PIN_CPU_WRp.c());
    d("PIN_CPU_ADDR_EXT  %c\n", PIN_CPU_EXT_BUSp.c());
    d("PIN_CPU_LATCH_EXT %c\n", PIN_CPU_LATCH_EXT.c());

    /*
    d("BUS ADDR  0x%04x  %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
      get_bus_addr(),
      BUS_CPU_A15.c(), BUS_CPU_A14.c(), BUS_CPU_A13.c(), BUS_CPU_A12.c(),
      BUS_CPU_A11.c(), BUS_CPU_A10.c(), BUS_CPU_A09.c(), BUS_CPU_A08.c(),
      BUS_CPU_A07.c(), BUS_CPU_A06.c(), BUS_CPU_A05.c(), BUS_CPU_A04.c(),
      BUS_CPU_A03.c(), BUS_CPU_A02.c(), BUS_CPU_A01.c(), BUS_CPU_A00.c());

    d("BUS DATA  0x%02x   %c%c%c%c%c%c%c%c\n",
      get_bus_data(),
      BUS_CPU_D7p.c(), BUS_CPU_D6p.c(), BUS_CPU_D5p.c(), BUS_CPU_D4p.c(),
      BUS_CPU_D3p.c(), BUS_CPU_D2p.c(), BUS_CPU_D1p.c(), BUS_CPU_D0p.c());
    */

    d("\n");

    d("PIN STARTp        %d\n", PIN_CPU_STARTp.qp());
    d("PIN SYS_RSTp      %d\n", PIN_CPU_SYS_RSTp.qp());
    d("PIN EXT_RST       %d\n", PIN_CPU_EXT_RST.qp());
    d("PIN UNOR_DBG      %d\n", PIN_CPU_UNOR_DBG.qp());
    d("PIN UMUT_DBG      %d\n", PIN_CPU_UMUT_DBG.qp());
    d("PIN EXT_CLKGOOD   %d\n", PIN_CPU_EXT_CLKGOOD.qp());
    d("PIN BOWA_xBCDEFGH %d\n", PIN_CPU_BOWA_Axxxxxxx.qp());
    d("PIN BEDO_Axxxxxxx %d\n", PIN_CPU_BEDO_xBCDEFGH.qp());
    d("PIN BEKO_ABCDxxxx %d\n", PIN_CPU_BEKO_ABCDxxxx.qp());
    d("PIN BUDE_xxxxEFGH %d\n", PIN_CPU_BUDE_xxxxEFGH.qp());
    d("PIN BOLO_ABCDEFxx %d\n", PIN_CPU_BOLO_ABCDEFxx.qp());
    d("PIN BUKE_AxxxxxGH %d\n", PIN_CPU_BUKE_AxxxxxGH.qp());
    d("PIN BOMA_Axxxxxxx %d\n", PIN_CPU_BOMA_xBCDEFGH.qp());
    d("PIN BOGA_xBCDEFGH %d\n", PIN_CPU_BOGA_Axxxxxxx.qp());
    d("\n");
  }

  //-----------------------------------------------------------------------------

  Signal BUS_CPU_A00;  // bottom right port PORTB_00: -> A00
  Signal BUS_CPU_A01;  // bottom right port PORTB_04: -> A01
  Signal BUS_CPU_A02;  // bottom right port PORTB_08: -> A02
  Signal BUS_CPU_A03;  // bottom right port PORTB_12: -> A03
  Signal BUS_CPU_A04;  // bottom right port PORTB_16: -> A04
  Signal BUS_CPU_A05;  // bottom right port PORTB_20: -> A05
  Signal BUS_CPU_A06;  // bottom right port PORTB_24: -> A06
  Signal BUS_CPU_A07;  // bottom right port PORTB_28: -> A07
  Signal BUS_CPU_A08;  // bottom right port PORTB_02: -> A08
  Signal BUS_CPU_A09;  // bottom right port PORTB_06: -> A09
  Signal BUS_CPU_A10;  // bottom right port PORTB_10: -> A10
  Signal BUS_CPU_A11;  // bottom right port PORTB_14: -> A11
  Signal BUS_CPU_A12;  // bottom right port PORTB_18: -> A12
  Signal BUS_CPU_A13;  // bottom right port PORTB_22: -> A13
  Signal BUS_CPU_A14;  // bottom right port PORTB_26: -> A14
  Signal BUS_CPU_A15;  // bottom right port PORTB_30: -> A15

  BusPU BUS_CPU_D0p;  // bottom left port: <>
  BusPU BUS_CPU_D1p;  // bottom left port: <>
  BusPU BUS_CPU_D2p;  // bottom left port: <>
  BusPU BUS_CPU_D3p;  // bottom left port: <>
  BusPU BUS_CPU_D4p;  // bottom left port: <>
  BusPU BUS_CPU_D5p;  // bottom left port: <>
  BusPU BUS_CPU_D6p;  // bottom left port: <>
  BusPU BUS_CPU_D7p;  // bottom left port: <>

  //-----------------------------------------------------------------------------
  // CPU-to-SOC control signals

  Signal PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  Signal PIN_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU
  Signal PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  Signal PIN_CPU_WRp;           // top right port PORTA_01: ->
  Signal PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  PinNP PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinNP PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  PinNP PIN_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  PinNP PIN_CPU_SYS_RSTp;      // top center port PORTC_01: <- P01.AFER , reset related state
  PinNP PIN_CPU_EXT_RST;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  PinNP PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinNP PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  PinNP PIN_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  PinNP PIN_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: // this is the "put address on bus" clock
  PinNP PIN_CPU_BEDO_xBCDEFGH; // top left port PORTD_02:
  PinNP PIN_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: // this is the "reset for next cycle" clock
  PinNP PIN_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: // this is the "put write data on bus" clock
  PinNP PIN_CPU_BOLO_ABCDEFxx; // top left port PORTD_05:
  PinNP PIN_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: // this is probably the "latch bus data" clock
  // These two clocks are the only ones that run before PIN_CPU_READYp is asserted.
  PinNP PIN_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: (RESET_CLK)
  PinNP PIN_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: - test pad 3
};

//-----------------------------------------------------------------------------

}; // namespace Schematics