#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct CpuBus {
  void reset() {
  }

  void reset_bootrom() {
    printf("CpuBus::reset_bootrom\n");
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

    PIN_CPU_6.reset(TRI_HZNP);
    PIN_CPU_LATCH_EXT.reset(TRI_HZNP);
    PIN_CPU_RDp.reset(TRI_HZNP);
    PIN_CPU_WRp.reset(TRI_HZNP);
    PIN_CPU_ADDR_EXTp.reset(TRI_HZNP);

    /*
    PIN_CPU_RDp.lock(1);
    PIN_CPU_WRp.lock(0);
    PIN_CPU_6.lock(0);
    PIN_CPU_ADDR_EXTp.lock(1);
    PIN_CPU_LATCH_EXT.lock(0);
    */

    PIN_CPU_RDp.reset(TRI_HZNP);
    PIN_CPU_WRp.reset(TRI_HZNP);
    PIN_CPU_6.reset(TRI_HZNP);
    PIN_CPU_ADDR_EXTp.reset(TRI_HZNP);
    PIN_CPU_LATCH_EXT.reset(TRI_HZNP);

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

  uint16_t get_bus_addr() const {
    return (uint16_t)pack_p(BUS_CPU_A00.qp(), BUS_CPU_A01.qp(), BUS_CPU_A02.qp(), BUS_CPU_A03.qp(),
                          BUS_CPU_A04.qp(), BUS_CPU_A05.qp(), BUS_CPU_A06.qp(), BUS_CPU_A07.qp(),
                          BUS_CPU_A08.qp(), BUS_CPU_A09.qp(), BUS_CPU_A10.qp(), BUS_CPU_A11.qp(),
                          BUS_CPU_A12.qp(), BUS_CPU_A13.qp(), BUS_CPU_A14.qp(), BUS_CPU_A15.qp());
  }

  uint8_t get_bus_data() const {
    return (uint8_t)pack_p(BUS_CPU_D0p.qp(), BUS_CPU_D1p.qp(), BUS_CPU_D2p.qp(), BUS_CPU_D3p.qp(),
                           BUS_CPU_D4p.qp(), BUS_CPU_D5p.qp(), BUS_CPU_D6p.qp(), BUS_CPU_D7p.qp());
  }

  void set_data(uint8_t data) {
    BUS_CPU_D0p.set(data & 0x01);
    BUS_CPU_D1p.set(data & 0x02);
    BUS_CPU_D2p.set(data & 0x04);
    BUS_CPU_D3p.set(data & 0x08);
    BUS_CPU_D4p.set(data & 0x10);
    BUS_CPU_D5p.set(data & 0x20);
    BUS_CPU_D6p.set(data & 0x40);
    BUS_CPU_D7p.set(data & 0x80);
  }

  void lock_data(uint8_t data) {
    BUS_CPU_D0p.lock(data & 0x01);
    BUS_CPU_D1p.lock(data & 0x02);
    BUS_CPU_D2p.lock(data & 0x04);
    BUS_CPU_D3p.lock(data & 0x08);
    BUS_CPU_D4p.lock(data & 0x10);
    BUS_CPU_D5p.lock(data & 0x20);
    BUS_CPU_D6p.lock(data & 0x40);
    BUS_CPU_D7p.lock(data & 0x80);
  }

  void unlock_data() {
    BUS_CPU_D0p.unlock();
    BUS_CPU_D1p.unlock();
    BUS_CPU_D2p.unlock();
    BUS_CPU_D3p.unlock();
    BUS_CPU_D4p.unlock();
    BUS_CPU_D5p.unlock();
    BUS_CPU_D6p.unlock();
    BUS_CPU_D7p.unlock();
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
  /*p08.SORE*/ wire SORE_0000_7FFFp() const { return not1(BUS_CPU_A15.qp()); }
  /*p08.TEVY*/ wire TEVY_8000_9FFFn() const { return or3(BUS_CPU_A13.qp(), BUS_CPU_A14.qp(), SORE_0000_7FFFp()); }

  /*p22.XOLA*/ wire XOLA_A00n() const { return not1(BUS_CPU_A00.qp()); }
  /*p22.XENO*/ wire XENO_A01n() const { return not1(BUS_CPU_A01.qp()); }
  /*p22.XUSY*/ wire XUSY_A02n() const { return not1(BUS_CPU_A02.qp()); }
  /*p22.XERA*/ wire XERA_A03n() const { return not1(BUS_CPU_A03.qp()); }
  /*p07.TONA*/ wire TONA_A08n() const { return not1(BUS_CPU_A08.qp()); }

  /*p22.WADO*/ wire WADO_A00p() const { return not1(XOLA_A00n()); }
  /*p22.WESA*/ wire WESA_A01p() const { return not1(XENO_A01n()); }
  /*p22.WALO*/ wire WALO_A02p() const { return not1(XUSY_A02n()); }
  /*p22.WEPO*/ wire WEPO_A03p() const { return not1(XERA_A03n()); }

  /*p03.TOVY*/ wire TOVY_A00n() const { return not1(BUS_CPU_A00.qp()); }
  /*p08.TOLA*/ wire TOLA_A01n() const { return not1(BUS_CPU_A01.qp()); }
  /*p06.SEFY*/ wire SEFY_A02n() const { return not1(BUS_CPU_A02.qp()); }

  /*p07.TUNA*/ wire TUNA_0000_FDFFp() const {
    return nand7(BUS_CPU_A15.qp(), BUS_CPU_A14.qp(), BUS_CPU_A13.qp(),
                 BUS_CPU_A12.qp(), BUS_CPU_A11.qp(), BUS_CPU_A10.qp(),
                 BUS_CPU_A09.qp());
  }

  /*p22.WERO*/ wire WERO_FF4Xp() const {
    /*p22.XALY*/ wire XALY_0x00xxxxp  = nor3(BUS_CPU_A07.qp(), BUS_CPU_A05.qp(), BUS_CPU_A04.qp());
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor2(TUNA_0000_FDFFp(), TONA_A08n());
    /*p22.WUTU*/ wire WUTU_FF4Xn = nand3(SYKE_FF00_FFFFp, BUS_CPU_A06.qp(), XALY_0x00xxxxp);
    return not1(WUTU_FF4Xn);
  }

  /*p06.SARE*/ wire SARE_XX00_XX07p() const { return nor5(BUS_CPU_A07.qp(), BUS_CPU_A06.qp(), BUS_CPU_A05.qp(), BUS_CPU_A04.qp(), BUS_CPU_A03.qp()); }


  /*p07.TULO*/ wire TULO_ADDR_00XXp() const {
    return nor8(BUS_CPU_A15.qp(), BUS_CPU_A14.qp(), BUS_CPU_A13.qp(), BUS_CPU_A12.qp(),
                BUS_CPU_A11.qp(), BUS_CPU_A10.qp(), BUS_CPU_A09.qp(), BUS_CPU_A08.qp());
  }

  /*p07.ZUFA*/ wire ZUFA_ADDR_00XX() const {
    /*p07.ZORO*/ wire ZORO_ADDR_0XXX = nor4(BUS_CPU_A15.qp(), BUS_CPU_A14.qp(), BUS_CPU_A13.qp(), BUS_CPU_A12.qp());
    /*p07.ZADU*/ wire ZADU_ADDR_X0XX = nor4(BUS_CPU_A11.qp(), BUS_CPU_A10.qp(), BUS_CPU_A09.qp(), BUS_CPU_A08.qp());
    return and2(ZORO_ADDR_0XXX, ZADU_ADDR_X0XX);
  }

  //-----------------------------------------------------------------------------

  Bus BUS_CPU_A00 = ERR_XXXX;  // bottom right port PORTB_00: -> A00
  Bus BUS_CPU_A01 = ERR_XXXX;  // bottom right port PORTB_04: -> A01
  Bus BUS_CPU_A02 = ERR_XXXX;  // bottom right port PORTB_08: -> A02
  Bus BUS_CPU_A03 = ERR_XXXX;  // bottom right port PORTB_12: -> A03
  Bus BUS_CPU_A04 = ERR_XXXX;  // bottom right port PORTB_16: -> A04
  Bus BUS_CPU_A05 = ERR_XXXX;  // bottom right port PORTB_20: -> A05
  Bus BUS_CPU_A06 = ERR_XXXX;  // bottom right port PORTB_24: -> A06
  Bus BUS_CPU_A07 = ERR_XXXX;  // bottom right port PORTB_28: -> A07
  Bus BUS_CPU_A08 = ERR_XXXX;  // bottom right port PORTB_02: -> A08
  Bus BUS_CPU_A09 = ERR_XXXX;  // bottom right port PORTB_06: -> A09
  Bus BUS_CPU_A10 = ERR_XXXX;  // bottom right port PORTB_10: -> A10
  Bus BUS_CPU_A11 = ERR_XXXX;  // bottom right port PORTB_14: -> A11
  Bus BUS_CPU_A12 = ERR_XXXX;  // bottom right port PORTB_18: -> A12
  Bus BUS_CPU_A13 = ERR_XXXX;  // bottom right port PORTB_22: -> A13
  Bus BUS_CPU_A14 = ERR_XXXX;  // bottom right port PORTB_26: -> A14
  Bus BUS_CPU_A15 = ERR_XXXX;  // bottom right port PORTB_30: -> A15

  Bus BUS_CPU_D0p = ERR_XXXX;  // bottom left port: <>
  Bus BUS_CPU_D1p = ERR_XXXX;  // bottom left port: <>
  Bus BUS_CPU_D2p = ERR_XXXX;  // bottom left port: <>
  Bus BUS_CPU_D3p = ERR_XXXX;  // bottom left port: <>
  Bus BUS_CPU_D4p = ERR_XXXX;  // bottom left port: <>
  Bus BUS_CPU_D5p = ERR_XXXX;  // bottom left port: <>
  Bus BUS_CPU_D6p = ERR_XXXX;  // bottom left port: <>
  Bus BUS_CPU_D7p = ERR_XXXX;  // bottom left port: <>

  //-----------------------------------------------------------------------------
  // CPU-to-SOC control signals

  Pin PIN_CPU_6 = ERR_XXXX;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  Pin PIN_CPU_LATCH_EXT = ERR_XXXX;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  Pin PIN_CPU_RDp = ERR_XXXX;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  Pin PIN_CPU_WRp = ERR_XXXX;           // top right port PORTA_01: ->
  Pin PIN_CPU_ADDR_EXTp = ERR_XXXX;     // top right port PORTA_06: -> TEXO, APAP

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  Pin PIN_CPU_BOOTp = ERR_XXXX;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  Pin PIN_CPU_ADDR_HIp = ERR_XXXX;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp

  Pin PIN_CPU_STARTp = ERR_XXXX;        // top center port PORTC_04: <- P01.CPU_RESET
  Pin PIN_CPU_SYS_RSTp = ERR_XXXX;      // top center port PORTC_01: <- P01.AFER , reset related state
  Pin PIN_CPU_EXT_RST = ERR_XXXX;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  Pin PIN_CPU_UNOR_DBG = ERR_XXXX;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  Pin PIN_CPU_UMUT_DBG = ERR_XXXX;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1

  Pin PIN_CPU_EXT_CLKGOOD = ERR_XXXX;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,

  Pin PIN_CPU_BOWA_Axxxxxxx = ERR_XXXX; // top left port PORTD_01: // this is the "put address on bus" clock
  Pin PIN_CPU_BEDO_xBCDEFGH = ERR_XXXX; // top left port PORTD_02:

  Pin PIN_CPU_BEKO_ABCDxxxx = ERR_XXXX; // top left port PORTD_03: // this is the "reset for next cycle" clock
  Pin PIN_CPU_BUDE_xxxxEFGH = ERR_XXXX; // top left port PORTD_04: // this is the "put write data on bus" clock

  Pin PIN_CPU_BOLO_ABCDEFxx = ERR_XXXX; // top left port PORTD_05:
  Pin PIN_CPU_BUKE_AxxxxxGH = ERR_XXXX; // top left port PORTD_07: // this is probably the "latch bus data" clock

  // These two clocks are the only ones that run before PIN_CPU_READYp is asserted.
  Pin PIN_CPU_BOMA_xBCDEFGH = ERR_XXXX; // top left port PORTD_08: (RESET_CLK)
  Pin PIN_CPU_BOGA_Axxxxxxx = ERR_XXXX; // top left port PORTD_09: - test pad 3
};

//-----------------------------------------------------------------------------

}; // namespace Schematics