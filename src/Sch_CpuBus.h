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
    return (uint16_t)pack_p(CPU_BUS_A00.tp(), CPU_BUS_A01.tp(), CPU_BUS_A02.tp(), CPU_BUS_A03.tp(),
                          CPU_BUS_A04.tp(), CPU_BUS_A05.tp(), CPU_BUS_A06.tp(), CPU_BUS_A07.tp(),
                          CPU_BUS_A08.tp(), CPU_BUS_A09.tp(), CPU_BUS_A10.tp(), CPU_BUS_A11.tp(),
                          CPU_BUS_A12.tp(), CPU_BUS_A13.tp(), CPU_BUS_A14.tp(), CPU_BUS_A15.tp());
  }

  uint8_t get_bus_data() const {
    return (uint8_t)pack_p(CPU_BUS_D0p.tp(), CPU_BUS_D1p.tp(), CPU_BUS_D2p.tp(), CPU_BUS_D3p.tp(),
                           CPU_BUS_D4p.tp(), CPU_BUS_D5p.tp(), CPU_BUS_D6p.tp(), CPU_BUS_D7p.tp());
  }

  void set_addr(uint16_t addr) {
    CPU_BUS_A00 = (addr & 0x0001);
    CPU_BUS_A01 = (addr & 0x0002);
    CPU_BUS_A02 = (addr & 0x0004);
    CPU_BUS_A03 = (addr & 0x0008);
    CPU_BUS_A04 = (addr & 0x0010);
    CPU_BUS_A05 = (addr & 0x0020);
    CPU_BUS_A06 = (addr & 0x0040);
    CPU_BUS_A07 = (addr & 0x0080);
    CPU_BUS_A08 = (addr & 0x0100);
    CPU_BUS_A09 = (addr & 0x0200);
    CPU_BUS_A10 = (addr & 0x0400);
    CPU_BUS_A11 = (addr & 0x0800);
    CPU_BUS_A12 = (addr & 0x1000);
    CPU_BUS_A13 = (addr & 0x2000);
    CPU_BUS_A14 = (addr & 0x4000);
    CPU_BUS_A15 = (addr & 0x8000);
  }

  void set_addr_z() {
    CPU_BUS_A00 = DELTA_TRIZ;
    CPU_BUS_A01 = DELTA_TRIZ;
    CPU_BUS_A02 = DELTA_TRIZ;
    CPU_BUS_A03 = DELTA_TRIZ;
    CPU_BUS_A04 = DELTA_TRIZ;
    CPU_BUS_A05 = DELTA_TRIZ;
    CPU_BUS_A06 = DELTA_TRIZ;
    CPU_BUS_A07 = DELTA_TRIZ;
    CPU_BUS_A08 = DELTA_TRIZ;
    CPU_BUS_A09 = DELTA_TRIZ;
    CPU_BUS_A10 = DELTA_TRIZ;
    CPU_BUS_A11 = DELTA_TRIZ;
    CPU_BUS_A12 = DELTA_TRIZ;
    CPU_BUS_A13 = DELTA_TRIZ;
    CPU_BUS_A14 = DELTA_TRIZ;
    CPU_BUS_A15 = DELTA_TRIZ;
  }

  void set_data(bool oe, uint8_t data) {
    if (oe) {
      CPU_BUS_D0p = (data & 0x01);
      CPU_BUS_D1p = (data & 0x02);
      CPU_BUS_D2p = (data & 0x04);
      CPU_BUS_D3p = (data & 0x08);
      CPU_BUS_D4p = (data & 0x10);
      CPU_BUS_D5p = (data & 0x20);
      CPU_BUS_D6p = (data & 0x40);
      CPU_BUS_D7p = (data & 0x80);
    }
    else {
      CPU_BUS_D0p = DELTA_TRIZ;
      CPU_BUS_D1p = DELTA_TRIZ;
      CPU_BUS_D2p = DELTA_TRIZ;
      CPU_BUS_D3p = DELTA_TRIZ;
      CPU_BUS_D4p = DELTA_TRIZ;
      CPU_BUS_D5p = DELTA_TRIZ;
      CPU_BUS_D6p = DELTA_TRIZ;
      CPU_BUS_D7p = DELTA_TRIZ;
    }
  }

  void set_data_z() {
    CPU_BUS_D0p = DELTA_TRIZ;
    CPU_BUS_D1p = DELTA_TRIZ;
    CPU_BUS_D2p = DELTA_TRIZ;
    CPU_BUS_D3p = DELTA_TRIZ;
    CPU_BUS_D4p = DELTA_TRIZ;
    CPU_BUS_D5p = DELTA_TRIZ;
    CPU_BUS_D6p = DELTA_TRIZ;
    CPU_BUS_D7p = DELTA_TRIZ;
  }

  //-----------------------------------------------------------------------------

  void dump(Dumper& d) const {
    d("---------- CPU Bus  ----------\n");
    d("CPU BOOT : %d\n", CPU_PIN_BOOTp.tp());
    d("CPU AHI  : %d\n", CPU_PIN_ADDR_HI.tp());
    d("CPU RDp  : %d\n", CPU_PIN_RDp.tp());
    d("CPU WRp  : %d\n", CPU_PIN_WRp.tp());
    d("CPU AVp  : %d\n", CPU_PIN_ADDR_EXTp.tp());
    d("CPU DVp  : %d\n", CPU_PIN_HOLD_MEM.tp());

    d("CPU ADDR : %04x\n", get_bus_addr());

    d("CPU DATA : %02x\n", get_bus_data());

    d("CPU PIN ADDR    : %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
      CPU_BUS_A15.c(), CPU_BUS_A14.c(), CPU_BUS_A13.c(), CPU_BUS_A12.c(),
      CPU_BUS_A11.c(), CPU_BUS_A10.c(), CPU_BUS_A09.c(), CPU_BUS_A08.c(),
      CPU_BUS_A07.c(), CPU_BUS_A06.c(), CPU_BUS_A05.c(), CPU_BUS_A04.c(),
      CPU_BUS_A03.c(), CPU_BUS_A02.c(), CPU_BUS_A01.c(), CPU_BUS_A00.c());

    d("CPU PIN DATA    : %c%c%c%c%c%c%c%c\n",
      CPU_BUS_D7p.c(), CPU_BUS_D6p.c(), CPU_BUS_D5p.c(), CPU_BUS_D4p.c(),
      CPU_BUS_D3p.c(), CPU_BUS_D2p.c(), CPU_BUS_D1p.c(), CPU_BUS_D0p.c());

    d("\n");
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
  /*p08.SORE*/ wire SORE_0000_7FFFp() const { return not1(CPU_BUS_A15.tp()); }
  /*p08.TEVY*/ wire TEVY_8000_9FFFn() const { return or3(CPU_BUS_A13.tp(), CPU_BUS_A14.tp(), SORE_0000_7FFFp()); }

  /*p22.XOLA*/ wire XOLA_A00n() const { return not1(CPU_BUS_A00.tp()); }
  /*p22.XENO*/ wire XENO_A01n() const { return not1(CPU_BUS_A01.tp()); }
  /*p22.XUSY*/ wire XUSY_A02n() const { return not1(CPU_BUS_A02.tp()); }
  /*p22.XERA*/ wire XERA_A03n() const { return not1(CPU_BUS_A03.tp()); }
  /*p07.TONA*/ wire TONA_A08n() const { return not1(CPU_BUS_A08.tp()); }

  /*p22.WADO*/ wire WADO_A00p() const { return not1(XOLA_A00n()); }
  /*p22.WESA*/ wire WESA_A01p() const { return not1(XENO_A01n()); }
  /*p22.WALO*/ wire WALO_A02p() const { return not1(XUSY_A02n()); }
  /*p22.WEPO*/ wire WEPO_A03p() const { return not1(XERA_A03n()); }

  /*p03.TOVY*/ wire TOVY_A00n() const { return not1(CPU_BUS_A00.tp()); }
  /*p08.TOLA*/ wire TOLA_A01n() const { return not1(CPU_BUS_A01.tp()); }
  /*p06.SEFY*/ wire SEFY_A02n() const { return not1(CPU_BUS_A02.tp()); }

  /*p07.TUNA*/ wire TUNA_0000_FDFFp() const {
    return nand7(CPU_BUS_A15.tp(), CPU_BUS_A14.tp(), CPU_BUS_A13.tp(),
                 CPU_BUS_A12.tp(), CPU_BUS_A11.tp(), CPU_BUS_A10.tp(),
                 CPU_BUS_A09.tp());
  }

  /*p22.WERO*/ wire WERO_FF4Xp() const {
    /*p22.XALY*/ wire XALY_0x00xxxxp  = nor3(CPU_BUS_A07.tp(), CPU_BUS_A05.tp(), CPU_BUS_A04.tp());
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor2(TUNA_0000_FDFFp(), TONA_A08n());
    /*p22.WUTU*/ wire WUTU_FF4Xn = nand3(SYKE_FF00_FFFFp, CPU_BUS_A06.tp(), XALY_0x00xxxxp);
    return not1(WUTU_FF4Xn);
  }

  /*p06.SARE*/ wire SARE_XX00_XX07p() const { return nor5(CPU_BUS_A07.tp(), CPU_BUS_A06.tp(), CPU_BUS_A05.tp(), CPU_BUS_A04.tp(), CPU_BUS_A03.tp()); }


  /*p07.TULO*/ wire TULO_ADDR_00XXp() const {
    return nor8(CPU_BUS_A15.tp(), CPU_BUS_A14.tp(), CPU_BUS_A13.tp(), CPU_BUS_A12.tp(),
                CPU_BUS_A11.tp(), CPU_BUS_A10.tp(), CPU_BUS_A09.tp(), CPU_BUS_A08.tp());
  }

  /*p07.ZUFA*/ wire ZUFA_ADDR_00XX() const {
    /*p07.ZORO*/ wire _ZORO_ADDR_0XXX = nor4(CPU_BUS_A15.tp(), CPU_BUS_A14.tp(), CPU_BUS_A13.tp(), CPU_BUS_A12.tp());
    /*p07.ZADU*/ wire _ZADU_ADDR_X0XX = nor4(CPU_BUS_A11.tp(), CPU_BUS_A10.tp(), CPU_BUS_A09.tp(), CPU_BUS_A08.tp());
    return and2(_ZORO_ADDR_0XXX, _ZADU_ADDR_X0XX);
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

  Tri CPU_BUS_D0p  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D1p  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D2p  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D3p  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D4p  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D5p  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D6p  = TRI_HZPU;   // bottom left port: <>
  Tri CPU_BUS_D7p  = TRI_HZPU;   // bottom left port: <>

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
  Tri CPU_PIN_ADDR_EXTp  = TRI_HZNP; // top right port PORTA_06: -> TEXO, APAP
};

//-----------------------------------------------------------------------------

}; // namespace Schematics