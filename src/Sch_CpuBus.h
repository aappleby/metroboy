#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct OamBus;
struct VramBus;

//-----------------------------------------------------------------------------

struct CpuBus {
  void tock(const SchematicTop& top);

  uint16_t get_bus_addr() const {
    return (uint16_t)pack(CPU_BUS_A00, CPU_BUS_A01, CPU_BUS_A02, CPU_BUS_A03,
                          CPU_BUS_A04, CPU_BUS_A05, CPU_BUS_A06, CPU_BUS_A07,
                          CPU_BUS_A08, CPU_BUS_A09, CPU_BUS_A10, CPU_BUS_A11,
                          CPU_BUS_A12, CPU_BUS_A13, CPU_BUS_A14, CPU_BUS_A15);
  }

  uint8_t get_bus_data() const {
    return (uint8_t)pack(CPU_BUS_D0, CPU_BUS_D1, CPU_BUS_D2, CPU_BUS_D3,
                         CPU_BUS_D4, CPU_BUS_D5, CPU_BUS_D6, CPU_BUS_D7);
  }

  void preset_addr(uint16_t addr) {
    CPU_BUS_A00.preset(addr & 0x0001);
    CPU_BUS_A01.preset(addr & 0x0002);
    CPU_BUS_A02.preset(addr & 0x0004);
    CPU_BUS_A03.preset(addr & 0x0008);
    CPU_BUS_A04.preset(addr & 0x0010);
    CPU_BUS_A05.preset(addr & 0x0020);
    CPU_BUS_A06.preset(addr & 0x0040);
    CPU_BUS_A07.preset(addr & 0x0080);
    CPU_BUS_A08.preset(addr & 0x0100);
    CPU_BUS_A09.preset(addr & 0x0200);
    CPU_BUS_A10.preset(addr & 0x0400);
    CPU_BUS_A11.preset(addr & 0x0800);
    CPU_BUS_A12.preset(addr & 0x1000);
    CPU_BUS_A13.preset(addr & 0x2000);
    CPU_BUS_A14.preset(addr & 0x4000);
    CPU_BUS_A15.preset(addr & 0x8000);
  }

  void preset_addr_z() {
    CPU_BUS_A00.preset(DELTA_TRIZ);
    CPU_BUS_A01.preset(DELTA_TRIZ);
    CPU_BUS_A02.preset(DELTA_TRIZ);
    CPU_BUS_A03.preset(DELTA_TRIZ);
    CPU_BUS_A04.preset(DELTA_TRIZ);
    CPU_BUS_A05.preset(DELTA_TRIZ);
    CPU_BUS_A06.preset(DELTA_TRIZ);
    CPU_BUS_A07.preset(DELTA_TRIZ);
    CPU_BUS_A08.preset(DELTA_TRIZ);
    CPU_BUS_A09.preset(DELTA_TRIZ);
    CPU_BUS_A10.preset(DELTA_TRIZ);
    CPU_BUS_A11.preset(DELTA_TRIZ);
    CPU_BUS_A12.preset(DELTA_TRIZ);
    CPU_BUS_A13.preset(DELTA_TRIZ);
    CPU_BUS_A14.preset(DELTA_TRIZ);
    CPU_BUS_A15.preset(DELTA_TRIZ);
  }

  void preset_data(bool oe, uint8_t data) {
    if (oe) {
      CPU_BUS_D0.preset(data & 0x01);
      CPU_BUS_D1.preset(data & 0x02);
      CPU_BUS_D2.preset(data & 0x04);
      CPU_BUS_D3.preset(data & 0x08);
      CPU_BUS_D4.preset(data & 0x10);
      CPU_BUS_D5.preset(data & 0x20);
      CPU_BUS_D6.preset(data & 0x40);
      CPU_BUS_D7.preset(data & 0x80);
    }
    else {
      CPU_BUS_D0.preset(DELTA_TRIZ);
      CPU_BUS_D1.preset(DELTA_TRIZ);
      CPU_BUS_D2.preset(DELTA_TRIZ);
      CPU_BUS_D3.preset(DELTA_TRIZ);
      CPU_BUS_D4.preset(DELTA_TRIZ);
      CPU_BUS_D5.preset(DELTA_TRIZ);
      CPU_BUS_D6.preset(DELTA_TRIZ);
      CPU_BUS_D7.preset(DELTA_TRIZ);
    }
  }

  void preset_data_z() {
    CPU_BUS_D0.preset(DELTA_TRIZ);
    CPU_BUS_D1.preset(DELTA_TRIZ);
    CPU_BUS_D2.preset(DELTA_TRIZ);
    CPU_BUS_D3.preset(DELTA_TRIZ);
    CPU_BUS_D4.preset(DELTA_TRIZ);
    CPU_BUS_D5.preset(DELTA_TRIZ);
    CPU_BUS_D6.preset(DELTA_TRIZ);
    CPU_BUS_D7.preset(DELTA_TRIZ);
  }

  //-----------------------------------------------------------------------------

  void dump(Dumper& d) const {
    d("---------- CPU Bus  ----------\n");
    d("CPU BOOT : %d\n", (wire)CPU_PIN_BOOTp);
    d("CPU AHI  : %d\n", (wire)CPU_PIN_ADDR_HI);
    d("CPU RDp  : %d\n", (wire)CPU_PIN_RDp);
    d("CPU WRp  : %d\n", (wire)CPU_PIN_WRp);
    d("CPU AVp  : %d\n", (wire)CPU_PIN_ADDR_EXT);
    d("CPU DVp  : %d\n", (wire)CPU_PIN_HOLD_MEM);

    d("CPU ADDR : %04x\n", get_bus_addr());

    d("CPU DATA : %02x\n", get_bus_data());

    d("CPU PIN ADDR    : %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
      CPU_BUS_A15.c(), CPU_BUS_A14.c(), CPU_BUS_A13.c(), CPU_BUS_A12.c(),
      CPU_BUS_A11.c(), CPU_BUS_A10.c(), CPU_BUS_A09.c(), CPU_BUS_A08.c(),
      CPU_BUS_A07.c(), CPU_BUS_A06.c(), CPU_BUS_A05.c(), CPU_BUS_A04.c(),
      CPU_BUS_A03.c(), CPU_BUS_A02.c(), CPU_BUS_A01.c(), CPU_BUS_A00.c());

    d("CPU PIN DATA    : %c%c%c%c%c%c%c%c\n",
      CPU_BUS_D7.c(), CPU_BUS_D6.c(), CPU_BUS_D5.c(), CPU_BUS_D4.c(),
      CPU_BUS_D3.c(), CPU_BUS_D2.c(), CPU_BUS_D1.c(), CPU_BUS_D0.c());

    d("\n");
  }

  //-----------------------------------------------------------------------------
  // Address decoders

  /*p25.SYRO*/ wire SYRO_FE00_FFFFp() const { return not(TUNA_0000_FDFFp()); }
  /*p07.RYCU*/ wire RYCU_0000_FDFFn() const { return not(TUNA_0000_FDFFp()); }
  /*p07.SYKE*/ wire SYKE_FF00_FFFFp() const { return nor(TUNA_0000_FDFFp(), TONA_A08n()); }
  /*p07.SOHA*/ wire SOHA_FF00_FFFFn() const { return not(SYKE_FF00_FFFFp()); }
  /*p07.ROPE*/ wire ROPE_FE00_FEFFn() const { return nand(RYCU_0000_FDFFn(), SOHA_FF00_FFFFn()); }
  /*p07.SARO*/ wire SARO_FE00_FEFFp() const { return not(ROPE_FE00_FEFFn()); }
  /*p28.ADAH*/ wire ADAH_FE00_FEFFn() const { return not(SARO_FE00_FEFFp()); }
  /*p08.SORE*/ wire SORE_0000_7FFFp() const { return not(CPU_BUS_A15); }
  /*p08.TEVY*/ wire TEVY_8000_9FFFn() const { return or(CPU_BUS_A13, CPU_BUS_A14, SORE_0000_7FFFp()); }

  /*p22.XOLA*/ wire XOLA_A00n() const { return not(CPU_BUS_A00); }
  /*p22.XENO*/ wire XENO_A01n() const { return not(CPU_BUS_A01); }
  /*p22.XUSY*/ wire XUSY_A02n() const { return not(CPU_BUS_A02); }
  /*p22.XERA*/ wire XERA_A03n() const { return not(CPU_BUS_A03); }
  /*p07.TONA*/ wire TONA_A08n() const { return not(CPU_BUS_A08); }

  /*p22.WADO*/ wire WADO_A00p() const { return not(XOLA_A00n()); }
  /*p22.WESA*/ wire WESA_A01p() const { return not(XENO_A01n()); }
  /*p22.WALO*/ wire WALO_A02p() const { return not(XUSY_A02n()); }
  /*p22.WEPO*/ wire WEPO_A03p() const { return not(XERA_A03n()); }

  /*p03.TOVY*/ wire TOVY_A00n() const { return not(CPU_BUS_A00); }
  /*p08.TOLA*/ wire TOLA_A01n() const { return not(CPU_BUS_A01); }
  /*p06.SEFY*/ wire SEFY_A02n() const { return not(CPU_BUS_A02); }

  /*p07.TUNA*/ wire TUNA_0000_FDFFp() const {
    return nand(CPU_BUS_A15, CPU_BUS_A14, CPU_BUS_A13,
                CPU_BUS_A12, CPU_BUS_A11, CPU_BUS_A10,
                CPU_BUS_A09);
  }

  /*p22.WERO*/ wire WERO_FF4Xp() const {
    /*p22.XALY*/ wire XALY_0x00xxxxp  = nor(CPU_BUS_A07, CPU_BUS_A05, CPU_BUS_A04);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp(), TONA_A08n());
    /*p22.WUTU*/ wire WUTU_FF4Xn = nand(SYKE_FF00_FFFFp, CPU_BUS_A06, XALY_0x00xxxxp);
    return not(WUTU_FF4Xn);
  }

  /*p06.SARE*/ wire SARE_XX00_XX07p() const { return nor(CPU_BUS_A07, CPU_BUS_A06, CPU_BUS_A05, CPU_BUS_A04, CPU_BUS_A03); }


  /*p07.TULO*/ wire TULO_ADDR_00XXp() const {
    return nor(CPU_BUS_A15, CPU_BUS_A14, CPU_BUS_A13, CPU_BUS_A12,
               CPU_BUS_A11, CPU_BUS_A10, CPU_BUS_A09, CPU_BUS_A08);
  }

  /*p07.ZUFA*/ wire ZUFA_ADDR_00XX() const {
    /*p07.ZORO*/ wire _ZORO_ADDR_0XXX = nor(CPU_BUS_A15, CPU_BUS_A14, CPU_BUS_A13, CPU_BUS_A12);
    /*p07.ZADU*/ wire _ZADU_ADDR_X0XX = nor(CPU_BUS_A11, CPU_BUS_A10, CPU_BUS_A09, CPU_BUS_A08);
    return and(_ZORO_ADDR_0XXX, _ZADU_ADDR_X0XX);
  }

  //-----------------------------------------------------------------------------

  Tri CPU_BUS_A00 = TRI_D0NP;  // bottom right port PORTB_00: -> A00
  Tri CPU_BUS_A01 = TRI_D0NP;  // bottom right port PORTB_04: -> A01
  Tri CPU_BUS_A02 = TRI_D0NP;  // bottom right port PORTB_08: -> A02
  Tri CPU_BUS_A03 = TRI_D0NP;  // bottom right port PORTB_12: -> A03
  Tri CPU_BUS_A04 = TRI_D0NP;  // bottom right port PORTB_16: -> A04
  Tri CPU_BUS_A05 = TRI_D0NP;  // bottom right port PORTB_20: -> A05
  Tri CPU_BUS_A06 = TRI_D0NP;  // bottom right port PORTB_24: -> A06
  Tri CPU_BUS_A07 = TRI_D0NP;  // bottom right port PORTB_28: -> A07
  Tri CPU_BUS_A08 = TRI_D0NP;  // bottom right port PORTB_02: -> A08
  Tri CPU_BUS_A09 = TRI_D0NP;  // bottom right port PORTB_06: -> A09
  Tri CPU_BUS_A10 = TRI_D0NP;  // bottom right port PORTB_10: -> A10
  Tri CPU_BUS_A11 = TRI_D0NP;  // bottom right port PORTB_14: -> A11
  Tri CPU_BUS_A12 = TRI_D0NP;  // bottom right port PORTB_18: -> A12
  Tri CPU_BUS_A13 = TRI_D0NP;  // bottom right port PORTB_22: -> A13
  Tri CPU_BUS_A14 = TRI_D0NP;  // bottom right port PORTB_26: -> A14
  Tri CPU_BUS_A15 = TRI_D0NP;  // bottom right port PORTB_30: -> A15                               

  Tri CPU_BUS_D0  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D1  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D2  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D3  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D4  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D5  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D6  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D7  = TRI_HZPU;   // bottom left port: <>

//private:

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  Tri CPU_PIN_BOOTp   = TRI_HZNP;   // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  Tri CPU_PIN_ADDR_HI = TRI_HZNP;   // top right port PORTA_03: <- P25.SYRO // Not really sure why this is here

  //-----------------------------------------------------------------------------
  // CPU-to-SOC control signals

  Tri CPU_PIN6          = TRI_HZNP; // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  Tri CPU_PIN_HOLD_MEM  = TRI_HZNP; // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  Tri CPU_PIN_RDp       = TRI_HZNP; // top right port PORTA_00: -> LAGU, LAVO, TEDO
  Tri CPU_PIN_WRp       = TRI_HZNP; // top right port PORTA_01: ->
  Tri CPU_PIN_ADDR_EXT  = TRI_HZNP; // top right port PORTA_06: -> TEXO, APAP
};

//-----------------------------------------------------------------------------

}; // namespace Schematics