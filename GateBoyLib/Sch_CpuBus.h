#pragma once
#include "GateBoyLib/Signals.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct CpuBus {
  void dump(Dumper& d) const;

  uint16_t get_bus_addr() const {
    return (uint16_t)pack_p(BUS_CPU_A00.tp(), BUS_CPU_A01.tp(), BUS_CPU_A02.tp(), BUS_CPU_A03.tp(),
                          BUS_CPU_A04.tp(), BUS_CPU_A05.tp(), BUS_CPU_A06.tp(), BUS_CPU_A07.tp(),
                          BUS_CPU_A08.tp(), BUS_CPU_A09.tp(), BUS_CPU_A10.tp(), BUS_CPU_A11.tp(),
                          BUS_CPU_A12.tp(), BUS_CPU_A13.tp(), BUS_CPU_A14.tp(), BUS_CPU_A15.tp());
  }

  uint8_t get_bus_data() const {
    return (uint8_t)pack_p(BUS_CPU_D0p.tp(), BUS_CPU_D1p.tp(), BUS_CPU_D2p.tp(), BUS_CPU_D3p.tp(),
                           BUS_CPU_D4p.tp(), BUS_CPU_D5p.tp(), BUS_CPU_D6p.tp(), BUS_CPU_D7p.tp());
  }

  void set_data(uint8_t data) {
    BUS_CPU_D0p.preset(wire(data & 0x01));
    BUS_CPU_D1p.preset(wire(data & 0x02));
    BUS_CPU_D2p.preset(wire(data & 0x04));
    BUS_CPU_D3p.preset(wire(data & 0x08));
    BUS_CPU_D4p.preset(wire(data & 0x10));
    BUS_CPU_D5p.preset(wire(data & 0x20));
    BUS_CPU_D6p.preset(wire(data & 0x40));
    BUS_CPU_D7p.preset(wire(data & 0x80));
  }

  //-----------------------------------------------------------------------------
  // Address decoders

  /*p25.SYRO*/ wire SYRO_FE00_FFFFp() const { return not1(TUNA_0000_FDFFp()); }
  /*p07.RYCU*/ wire RYCU_0000_FDFFn() const { return not1(TUNA_0000_FDFFp()); }
  /*p07.SYKE*/ wire SYKE_FF00_FFFFp() const { return nor2(TUNA_0000_FDFFp(), TONA_A08n()); }
  /*p07.SOHA*/ wire SOHA_FF00_FFFFn() const { return not1(SYKE_FF00_FFFFp()); }
  /*p07.ROPE*/ wire ROPE_FE00_FEFFn() const { return nand2(RYCU_0000_FDFFn(), SOHA_FF00_FFFFn()); }
  /*p07.SARO*/ wire SARO_FE00_FEFFp() const { return not1(ROPE_FE00_FEFFn()); }
  /*p28.ADAH*/ wire ADAH_FE00_FEFFn() const { return not1(SARO_FE00_FEFFp()); }
  /*p08.SORE*/ wire SORE_0000_7FFFp() const { return not1(BUS_CPU_A15.tp()); }
  /*p08.TEVY*/ wire TEVY_8000_9FFFn() const { return or3(BUS_CPU_A13.tp(), BUS_CPU_A14.tp(), SORE_0000_7FFFp()); }

  /*p22.XOLA*/ wire XOLA_A00n() const { return not1(BUS_CPU_A00.tp()); }
  /*p22.XENO*/ wire XENO_A01n() const { return not1(BUS_CPU_A01.tp()); }
  /*p22.XUSY*/ wire XUSY_A02n() const { return not1(BUS_CPU_A02.tp()); }
  /*p22.XERA*/ wire XERA_A03n() const { return not1(BUS_CPU_A03.tp()); }
  /*p07.TONA*/ wire TONA_A08n() const { return not1(BUS_CPU_A08.tp()); }

  /*p22.WADO*/ wire WADO_A00p() const { return not1(XOLA_A00n()); }
  /*p22.WESA*/ wire WESA_A01p() const { return not1(XENO_A01n()); }
  /*p22.WALO*/ wire WALO_A02p() const { return not1(XUSY_A02n()); }
  /*p22.WEPO*/ wire WEPO_A03p() const { return not1(XERA_A03n()); }

  /*p03.TOVY*/ wire TOVY_A00n() const { return not1(BUS_CPU_A00.tp()); }
  /*p08.TOLA*/ wire TOLA_A01n() const { return not1(BUS_CPU_A01.tp()); }
  /*p06.SEFY*/ wire SEFY_A02n() const { return not1(BUS_CPU_A02.tp()); }

  /*p07.TUNA*/ wire TUNA_0000_FDFFp() const {
    return nand7(BUS_CPU_A15.tp(), BUS_CPU_A14.tp(), BUS_CPU_A13.tp(),
                 BUS_CPU_A12.tp(), BUS_CPU_A11.tp(), BUS_CPU_A10.tp(),
                 BUS_CPU_A09.tp());
  }

  /*p22.WERO*/ wire WERO_FF4Xp() const {
    /*p22.XALY*/ wire XALY_0x00xxxxp  = nor3(BUS_CPU_A07.tp(), BUS_CPU_A05.tp(), BUS_CPU_A04.tp());
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor2(TUNA_0000_FDFFp(), TONA_A08n());
    /*p22.WUTU*/ wire WUTU_FF4Xn = nand3(SYKE_FF00_FFFFp, BUS_CPU_A06.tp(), XALY_0x00xxxxp);
    return not1(WUTU_FF4Xn);
  }

  /*p06.SARE*/ wire SARE_XX00_XX07p() const { return nor5(BUS_CPU_A07.tp(), BUS_CPU_A06.tp(), BUS_CPU_A05.tp(), BUS_CPU_A04.tp(), BUS_CPU_A03.tp()); }


  /*p07.TULO*/ wire TULO_ADDR_00XXp() const {
    return nor8(BUS_CPU_A15.tp(), BUS_CPU_A14.tp(), BUS_CPU_A13.tp(), BUS_CPU_A12.tp(),
                BUS_CPU_A11.tp(), BUS_CPU_A10.tp(), BUS_CPU_A09.tp(), BUS_CPU_A08.tp());
  }

  /*p07.ZUFA*/ wire ZUFA_ADDR_00XX() const {
    /*p07.ZORO*/ wire ZORO_ADDR_0XXX = nor4(BUS_CPU_A15.tp(), BUS_CPU_A14.tp(), BUS_CPU_A13.tp(), BUS_CPU_A12.tp());
    /*p07.ZADU*/ wire ZADU_ADDR_X0XX = nor4(BUS_CPU_A11.tp(), BUS_CPU_A10.tp(), BUS_CPU_A09.tp(), BUS_CPU_A08.tp());
    return and2(ZORO_ADDR_0XXX, ZADU_ADDR_X0XX);
  }

  //-----------------------------------------------------------------------------

  Bus BUS_CPU_A00 = TRI_D0NP;  // bottom right port PORTB_00: -> A00
  Bus BUS_CPU_A01 = TRI_D0NP;  // bottom right port PORTB_04: -> A01
  Bus BUS_CPU_A02 = TRI_D0NP;  // bottom right port PORTB_08: -> A02
  Bus BUS_CPU_A03 = TRI_D0NP;  // bottom right port PORTB_12: -> A03
  Bus BUS_CPU_A04 = TRI_D0NP;  // bottom right port PORTB_16: -> A04
  Bus BUS_CPU_A05 = TRI_D0NP;  // bottom right port PORTB_20: -> A05
  Bus BUS_CPU_A06 = TRI_D0NP;  // bottom right port PORTB_24: -> A06
  Bus BUS_CPU_A07 = TRI_D0NP;  // bottom right port PORTB_28: -> A07
  Bus BUS_CPU_A08 = TRI_D0NP;  // bottom right port PORTB_02: -> A08
  Bus BUS_CPU_A09 = TRI_D0NP;  // bottom right port PORTB_06: -> A09
  Bus BUS_CPU_A10 = TRI_D0NP;  // bottom right port PORTB_10: -> A10
  Bus BUS_CPU_A11 = TRI_D0NP;  // bottom right port PORTB_14: -> A11
  Bus BUS_CPU_A12 = TRI_D0NP;  // bottom right port PORTB_18: -> A12
  Bus BUS_CPU_A13 = TRI_D0NP;  // bottom right port PORTB_22: -> A13
  Bus BUS_CPU_A14 = TRI_D0NP;  // bottom right port PORTB_26: -> A14
  Bus BUS_CPU_A15 = TRI_D0NP;  // bottom right port PORTB_30: -> A15                               

  Bus BUS_CPU_D0p = TRI_HZPU;  // bottom left port: <>
  Bus BUS_CPU_D1p = TRI_HZPU;  // bottom left port: <>
  Bus BUS_CPU_D2p = TRI_HZPU;  // bottom left port: <>
  Bus BUS_CPU_D3p = TRI_HZPU;  // bottom left port: <>
  Bus BUS_CPU_D4p = TRI_HZPU;  // bottom left port: <>
  Bus BUS_CPU_D5p = TRI_HZPU;  // bottom left port: <>
  Bus BUS_CPU_D6p = TRI_HZPU;  // bottom left port: <>
  Bus BUS_CPU_D7p = TRI_HZPU;  // bottom left port: <>

  //-----------------------------------------------------------------------------
  // CPU-to-SOC control signals

  Tri PIN_CPU_6          = TRI_HZNP; // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  Tri PIN_CPU_LATCH_EXT  = TRI_HZNP; // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  Tri PIN_CPU_RDp        = TRI_HZNP; // top right port PORTA_00: -> LAGU, LAVO, TEDO
  Tri PIN_CPU_WRp        = TRI_HZNP; // top right port PORTA_01: ->
  Tri PIN_CPU_ADDR_EXTp  = TRI_HZNP; // top right port PORTA_06: -> TEXO, APAP

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  Tri PIN_CPU_BOOTp         = TRI_HZNP; // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  Tri PIN_CPU_ADDR_HI       = TRI_HZNP; // top right port PORTA_03: <- P25.SYRO // Not really sure why this is here

  Tri PIN_CPU_STARTp        = TRI_HZNP; // top center port PORTC_04: <- P01.CPU_RESET
  Tri PIN_CPU_SYS_RSTp      = TRI_HZNP; // top center port PORTC_01: <- P01.AFER , reset related state
  Tri PIN_CPU_EXT_RST       = TRI_HZNP; // top center port PORTC_02: <- PIN_RESET directly connected to the pad 
  Tri PIN_CPU_UNOR_DBG      = TRI_HZNP; // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  Tri PIN_CPU_UMUT_DBG      = TRI_HZNP; // top right port PORTA_05: <- P07.UMUT_MODE_DBG1

  Tri PIN_CPU_EXT_CLKGOOD   = TRI_HZNP; // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  
  Tri PIN_CPU_BOWA_xBCDEFGH = TRI_HZNP; // top left port PORTD_01: // this is the "put address on bus" clock
  Tri PIN_CPU_BEDO_Axxxxxxx = TRI_HZNP; // top left port PORTD_02:

  Tri PIN_CPU_BEKO_ABCDxxxx = TRI_HZNP; // top left port PORTD_03: // this is the "reset for next cycle" clock
  Tri PIN_CPU_BUDE_xxxxEFGH = TRI_HZNP; // top left port PORTD_04: // this is the "put write data on bus" clock

  Tri PIN_CPU_BOLO_ABCDEFxx = TRI_HZNP; // top left port PORTD_05:
  Tri PIN_CPU_BUKE_AxxxxxGH = TRI_HZNP; // top left port PORTD_07: // this is probably the "latch bus data" clock

  // These two clocks are the only ones that run before PIN_CPU_READYp is asserted.
  Tri PIN_CPU_BOMA_Axxxxxxx = TRI_HZNP; // top left port PORTD_08: (RESET_CLK)
  Tri PIN_CPU_BOGA_xBCDEFGH = TRI_HZNP; // top left port PORTD_09: - test pad 3
};

//-----------------------------------------------------------------------------

}; // namespace Schematics