#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct CpuBus {
  void reset_cart() {
    BUS_CPU_A00.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A01.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A02.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A03.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A04.reset(TRI_D1PU | (DELTA_LOCK << 4));
    BUS_CPU_A05.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A06.reset(TRI_D1PU | (DELTA_LOCK << 4));
    BUS_CPU_A07.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A08.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A09.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A10.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A11.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A12.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A13.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A14.reset(TRI_D0PU | (DELTA_LOCK << 4));
    BUS_CPU_A15.reset(TRI_D0PU | (DELTA_LOCK << 4));

    BUS_CPU_D0p.reset(TRI_HZPU);
    BUS_CPU_D1p.reset(TRI_HZPU);
    BUS_CPU_D2p.reset(TRI_HZPU);
    BUS_CPU_D3p.reset(TRI_HZPU);
    BUS_CPU_D4p.reset(TRI_HZPU);
    BUS_CPU_D5p.reset(TRI_HZPU);
    BUS_CPU_D6p.reset(TRI_HZPU);
    BUS_CPU_D7p.reset(TRI_HZPU);

    //PIN_CPU_6.reset(0x3E, 0x0F); // ?
    PIN_CPU_6.reset(TRI_HZNP | (DELTA_LOCK << 4));

    PIN_CPU_LATCH_EXT.reset(TRI_HZNP, TRI_D0NP | (DELTA_LOCK << 4));
    PIN_CPU_RDp      .reset(TRI_HZNP, TRI_D0NP | (DELTA_LOCK << 4));
    PIN_CPU_WRp      .reset(TRI_HZNP, TRI_D0NP | (DELTA_LOCK << 4));
    PIN_CPU_EXT_BUSp.reset(TRI_HZNP, TRI_D0NP | (DELTA_LOCK << 4));

    PIN_CPU_BOOTp         .reset(TRI_D0NP);
    PIN_CPU_ADDR_HIp      .reset(TRI_D0NP);
    PIN_CPU_STARTp        .reset(TRI_D0NP);
    PIN_CPU_SYS_RSTp      .reset(TRI_D0NP);
    PIN_CPU_EXT_RST       .reset(TRI_D0NP);
    PIN_CPU_UNOR_DBG      .reset(TRI_D0NP);
    PIN_CPU_UMUT_DBG      .reset(TRI_D0NP);
    PIN_CPU_EXT_CLKGOOD   .reset(TRI_D1NP);

    PIN_CPU_BOWA_Axxxxxxx .reset(TRI_D1NP);
    PIN_CPU_BEDO_xBCDEFGH .reset(TRI_D0NP);
    PIN_CPU_BEKO_ABCDxxxx .reset(TRI_D1NP);
    PIN_CPU_BUDE_xxxxEFGH .reset(TRI_D0NP);
    PIN_CPU_BOLO_ABCDEFxx .reset(TRI_D1NP);
    PIN_CPU_BUKE_AxxxxxGH .reset(TRI_D1NP);
    PIN_CPU_BOMA_xBCDEFGH .reset(TRI_D0NP);
    PIN_CPU_BOGA_Axxxxxxx .reset(TRI_D1NP);
  }

  void reset_boot() {
    BUS_CPU_A00.reset(TRI_HZPU);
    BUS_CPU_A01.reset(TRI_HZPU);
    BUS_CPU_A02.reset(TRI_HZPU);
    BUS_CPU_A03.reset(TRI_HZPU);
    BUS_CPU_A04.reset(TRI_HZPU);
    BUS_CPU_A05.reset(TRI_HZPU);
    BUS_CPU_A06.reset(TRI_HZPU);
    BUS_CPU_A07.reset(TRI_HZPU);
    BUS_CPU_A08.reset(TRI_HZPU);
    BUS_CPU_A09.reset(TRI_HZPU);
    BUS_CPU_A10.reset(TRI_HZPU);
    BUS_CPU_A11.reset(TRI_HZPU);
    BUS_CPU_A12.reset(TRI_HZPU);
    BUS_CPU_A13.reset(TRI_HZPU);
    BUS_CPU_A14.reset(TRI_HZPU);
    BUS_CPU_A15.reset(TRI_HZPU);

    BUS_CPU_D0p.reset(TRI_HZPU);
    BUS_CPU_D1p.reset(TRI_HZPU);
    BUS_CPU_D2p.reset(TRI_HZPU);
    BUS_CPU_D3p.reset(TRI_HZPU);
    BUS_CPU_D4p.reset(TRI_HZPU);
    BUS_CPU_D5p.reset(TRI_HZPU);
    BUS_CPU_D6p.reset(TRI_HZPU);
    BUS_CPU_D7p.reset(TRI_HZPU);

    PIN_CPU_6.reset(TRI_HZNP | (DELTA_LOCK << 4));
    PIN_CPU_LATCH_EXT.reset(TRI_HZNP);
    PIN_CPU_RDp.reset(TRI_HZNP);
    PIN_CPU_WRp.reset(TRI_HZNP);
    PIN_CPU_EXT_BUSp.reset(TRI_HZNP);

    PIN_CPU_BOOTp         .reset(TRI_HZNP);
    PIN_CPU_ADDR_HIp      .reset(TRI_HZNP);
    PIN_CPU_STARTp        .reset(TRI_HZNP);
    PIN_CPU_SYS_RSTp      .reset(TRI_HZNP);
    PIN_CPU_EXT_RST       .reset(TRI_HZNP);
    PIN_CPU_UNOR_DBG      .reset(TRI_HZNP);
    PIN_CPU_UMUT_DBG      .reset(TRI_HZNP);
    PIN_CPU_EXT_CLKGOOD   .reset(TRI_HZNP);
    PIN_CPU_BOWA_Axxxxxxx .reset(TRI_HZNP);
    PIN_CPU_BEDO_xBCDEFGH .reset(TRI_HZNP);
    PIN_CPU_BEKO_ABCDxxxx .reset(TRI_HZNP);
    PIN_CPU_BUDE_xxxxEFGH .reset(TRI_HZNP);
    PIN_CPU_BOLO_ABCDEFxx .reset(TRI_HZNP);
    PIN_CPU_BUKE_AxxxxxGH .reset(TRI_HZNP);
    PIN_CPU_BOMA_xBCDEFGH .reset(TRI_HZNP);
    PIN_CPU_BOGA_Axxxxxxx .reset(TRI_HZNP);
  }

  void dump(Dumper& d) const;

  //-----------------------------------------------------------------------------

  Bus BUS_CPU_A00;  // bottom right port PORTB_00: -> A00
  Bus BUS_CPU_A01;  // bottom right port PORTB_04: -> A01
  Bus BUS_CPU_A02;  // bottom right port PORTB_08: -> A02
  Bus BUS_CPU_A03;  // bottom right port PORTB_12: -> A03
  Bus BUS_CPU_A04;  // bottom right port PORTB_16: -> A04
  Bus BUS_CPU_A05;  // bottom right port PORTB_20: -> A05
  Bus BUS_CPU_A06;  // bottom right port PORTB_24: -> A06
  Bus BUS_CPU_A07;  // bottom right port PORTB_28: -> A07
  Bus BUS_CPU_A08;  // bottom right port PORTB_02: -> A08
  Bus BUS_CPU_A09;  // bottom right port PORTB_06: -> A09
  Bus BUS_CPU_A10;  // bottom right port PORTB_10: -> A10
  Bus BUS_CPU_A11;  // bottom right port PORTB_14: -> A11
  Bus BUS_CPU_A12;  // bottom right port PORTB_18: -> A12
  Bus BUS_CPU_A13;  // bottom right port PORTB_22: -> A13
  Bus BUS_CPU_A14;  // bottom right port PORTB_26: -> A14
  Bus BUS_CPU_A15;  // bottom right port PORTB_30: -> A15

  Bus BUS_CPU_D0p;  // bottom left port: <>
  Bus BUS_CPU_D1p;  // bottom left port: <>
  Bus BUS_CPU_D2p;  // bottom left port: <>
  Bus BUS_CPU_D3p;  // bottom left port: <>
  Bus BUS_CPU_D4p;  // bottom left port: <>
  Bus BUS_CPU_D5p;  // bottom left port: <>
  Bus BUS_CPU_D6p;  // bottom left port: <>
  Bus BUS_CPU_D7p;  // bottom left port: <>

  //-----------------------------------------------------------------------------
  // CPU-to-SOC control signals

  Pin PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  Pin PIN_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  Pin PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  Pin PIN_CPU_WRp;           // top right port PORTA_01: ->
  Pin PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  Pin PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  Pin PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp

  Pin PIN_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  Pin PIN_CPU_SYS_RSTp;      // top center port PORTC_01: <- P01.AFER , reset related state
  Pin PIN_CPU_EXT_RST;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  Pin PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  Pin PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1

  Pin PIN_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,

  Pin PIN_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: // this is the "put address on bus" clock
  Pin PIN_CPU_BEDO_xBCDEFGH; // top left port PORTD_02:

  Pin PIN_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: // this is the "reset for next cycle" clock
  Pin PIN_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: // this is the "put write data on bus" clock

  Pin PIN_CPU_BOLO_ABCDEFxx; // top left port PORTD_05:
  Pin PIN_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: // this is probably the "latch bus data" clock

  // These two clocks are the only ones that run before PIN_CPU_READYp is asserted.
  Pin PIN_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: (RESET_CLK)
  Pin PIN_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: - test pad 3
};

//-----------------------------------------------------------------------------

}; // namespace Schematics