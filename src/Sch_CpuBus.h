#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct OamBus;
struct VramBus;

//-----------------------------------------------------------------------------

struct CpuBus {
  CpuBus();
  void tock(const SchematicTop& top);
  SignalHash commit();

  void set_cpu_req(Req req);

  int get_addr() const {
    return pack(CPU_PIN_A00, CPU_PIN_A01, CPU_PIN_A02, CPU_PIN_A03,
                CPU_PIN_A04, CPU_PIN_A05, CPU_PIN_A06, CPU_PIN_A07,
                CPU_PIN_A08, CPU_PIN_A09, CPU_PIN_A10, CPU_PIN_A11,
                CPU_PIN_A12, CPU_PIN_A13, CPU_PIN_A14, CPU_PIN_A15);
  }

  int get_data() const {
    return pack(CPU_TRI_D0, CPU_TRI_D1, CPU_TRI_D2, CPU_TRI_D3,
                CPU_TRI_D4, CPU_TRI_D5, CPU_TRI_D6, CPU_TRI_D7);
  }

  //-----------------------------------------------------------------------------
  // Address decoders

  /*p25.SYRO*/ wire SYRO_FE00_FFFFp() const { return not(TUNA_0000_FDFFp()); }
  /*p07.RYCU*/ wire RYCU_0000_FDFFn() const { return not(TUNA_0000_FDFFp()); }
  /*p07.SOHA*/ wire SOHA_FF00_FFFFn() const { return not(SYKE_FF00_FFFFp()); }
  /*p07.ROPE*/ wire ROPE_FE00_FEFFn() const { return nand(RYCU_0000_FDFFn(), SOHA_FF00_FFFFn()); }
  /*p07.SARO*/ wire SARO_FE00_FEFFp() const { return not(ROPE_FE00_FEFFn()); }
  /*p28.ADAH*/ wire ADAH_FE00_FEFFn() const { return not(SARO_FE00_FEFFp()); }
  /*p08.SORE*/ wire SORE_0000_7FFFp() const { return not(CPU_PIN_A15); }
  /*p08.TEVY*/ wire TEVY_8000_9FFFn() const { return or(CPU_PIN_A13, CPU_PIN_A14, SORE_0000_7FFFp()); }
  /*p08.TEXO*/ wire TEXO_8000_9FFFn() const { return and(_CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn()); }
  /*p08.LEVO*/ wire LEVO_8000_9FFFp() const { return not(TEXO_8000_9FFFn()); }

  /*p22.XOLA*/ wire XOLA_A00n() const { return not(CPU_PIN_A00); }
  /*p22.XENO*/ wire XENO_A01n() const { return not(CPU_PIN_A01); }
  /*p22.XUSY*/ wire XUSY_A02n() const { return not(CPU_PIN_A02); }
  /*p22.XERA*/ wire XERA_A03n() const { return not(CPU_PIN_A03); }
  /*p07.TONA*/ wire TONA_A08n() const { return not(CPU_PIN_A08); }

  /*p22.WADO*/ wire WADO_A00p() const { return not(XOLA_A00n()); }
  /*p22.WESA*/ wire WESA_A01p() const { return not(XENO_A01n()); }
  /*p22.WALO*/ wire WALO_A02p() const { return not(XUSY_A02n()); }
  /*p22.WEPO*/ wire WEPO_A03p() const { return not(XERA_A03n()); }

  /*p03.TOVY*/ wire TOVY_A00n() const { return not(CPU_PIN_A00); }
  /*p08.TOLA*/ wire TOLA_A01n() const { return not(CPU_PIN_A01); }
  /*p06.SEFY*/ wire SEFY_A02n() const { return not(CPU_PIN_A02); }

  /*p07.TUNA*/ wire TUNA_0000_FDFFp() const {
    return nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13,
                CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10,
                CPU_PIN_A09);
  }

  /*p22.WERO*/ wire WERO_FF4Xp() const {
    /*p22.XALY*/ wire XALY_0x00xxxxp  = nor(CPU_PIN_A07, CPU_PIN_A05, CPU_PIN_A04);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp(), TONA_A08n());
    /*p22.WUTU*/ wire WUTU_FF4Xn = nand(SYKE_FF00_FFFFp, CPU_PIN_A06, XALY_0x00xxxxp);
    return not(WUTU_FF4Xn);
  }

  /*p07.SYKE*/ wire SYKE_FF00_FFFFp() const { return nor(TUNA_0000_FDFFp(), TONA_A08n()); }
  /*p06.SARE*/ wire SARE_XX00_XX07p() const { return nor(CPU_PIN_A07, CPU_PIN_A06, CPU_PIN_A05, CPU_PIN_A04, CPU_PIN_A03); }


  /*p25.SOSE*/ wire SOSE_8000_9FFFp() const {
    /*p25.TEFA*/ wire _TEFA_8000_9FFFp = nor(SYRO_FE00_FFFFp(), TEXO_8000_9FFFn());
    return and (CPU_PIN_A15, _TEFA_8000_9FFFp);
  }

  /*p07.TULO*/ wire TULO_ADDR_00XXp() const {
    return nor(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12,
               CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09, CPU_PIN_A08);
  }

  /*p07.ZUFA*/ wire ZUFA_ADDR_00XX() const {
    /*p07.ZORO*/ wire _ZORO_ADDR_0XXX = nor(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12);
    /*p07.ZADU*/ wire _ZADU_ADDR_X0XX = nor(CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09, CPU_PIN_A08);
    return and(_ZORO_ADDR_0XXX, _ZADU_ADDR_X0XX);
  }

  //-----------------------------------------------------------------------------
  // Bus control

  // -> ext bus
  /*p08.LAVO*/ wire LAVO_LATCH_CPU_DATAn() const { return nand(_CPU_PIN_RDp, TEXO_8000_9FFFn(), _CPU_PIN5); }

  // -> top.SALE
  /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn() const { return nand(SOSE_8000_9FFFp(), _CPU_PIN_WRp); } // Schematic wrong, second input is CPU_RAW_WR

  /*p08.TYNU*/ wire TYNU_ADDR_RAM() const { 
    /*p08.SOGY*/ wire _SOGY_A14n = not(CPU_PIN_A14);
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and(CPU_PIN_A13, _SOGY_A14n, CPU_PIN_A15);
    return or(and(CPU_PIN_A15, CPU_PIN_A14), _TUMA_CART_RAM);
  }

  /*p??.APAP*/ wire APAP_CPU_ADDR_VALIDp() const { return not(_CPU_PIN_ADDR_VALID); } // Missing from schematic

  /*p04.MAKA*/ wire MAKA_FROM_CPU5_SYNC() const { return _MAKA_FROM_CPU5_SYNC.q(); }

  /*p04.DECY*/ wire DECY_FROM_CPU5n() const { return not(_CPU_PIN5); }

  /*p08.LYWE*/ wire LYWE() const {
    /*p08.LAGU*/ wire _LAGU = or(and(_CPU_PIN_RDp, LEVO_8000_9FFFp()), _CPU_PIN_WRp);
    return not(_LAGU);
  }

  wire CPU_PIN_RDp() const { return _CPU_PIN_RDp; }
  wire CPU_PIN_WRp() const { return _CPU_PIN_WRp; }
  wire CPU_PIN5() const { return _CPU_PIN5; }

  //-----------------------------------------------------------------------------

  Pin2 CPU_PIN_A00 = Pin2::HOLD_0;  // bottom right port PORTB_00: -> A00
  Pin2 CPU_PIN_A01 = Pin2::HOLD_0;  // bottom right port PORTB_04: -> A01
  Pin2 CPU_PIN_A02 = Pin2::HOLD_0;  // bottom right port PORTB_08: -> A02
  Pin2 CPU_PIN_A03 = Pin2::HOLD_0;  // bottom right port PORTB_12: -> A03
  Pin2 CPU_PIN_A04 = Pin2::HOLD_0;  // bottom right port PORTB_16: -> A04
  Pin2 CPU_PIN_A05 = Pin2::HOLD_0;  // bottom right port PORTB_20: -> A05
  Pin2 CPU_PIN_A06 = Pin2::HOLD_0;  // bottom right port PORTB_24: -> A06
  Pin2 CPU_PIN_A07 = Pin2::HOLD_0;  // bottom right port PORTB_28: -> A07
  Pin2 CPU_PIN_A08 = Pin2::HOLD_0;  // bottom right port PORTB_02: -> A08
  Pin2 CPU_PIN_A09 = Pin2::HOLD_0;  // bottom right port PORTB_06: -> A09
  Pin2 CPU_PIN_A10 = Pin2::HOLD_0;  // bottom right port PORTB_10: -> A10
  Pin2 CPU_PIN_A11 = Pin2::HOLD_0;  // bottom right port PORTB_14: -> A11
  Pin2 CPU_PIN_A12 = Pin2::HOLD_0;  // bottom right port PORTB_18: -> A12
  Pin2 CPU_PIN_A13 = Pin2::HOLD_0;  // bottom right port PORTB_22: -> A13
  Pin2 CPU_PIN_A14 = Pin2::HOLD_0;  // bottom right port PORTB_26: -> A14
  Pin2 CPU_PIN_A15 = Pin2::HOLD_0;  // bottom right port PORTB_30: -> A15                               

  Pin2 CPU_TRI_D0 = Pin2::HIZ_PU;   // bottom left port: <>
  Pin2 CPU_TRI_D1 = Pin2::HIZ_PU;   // bottom left port: <>
  Pin2 CPU_TRI_D2 = Pin2::HIZ_PU;   // bottom left port: <>
  Pin2 CPU_TRI_D3 = Pin2::HIZ_PU;   // bottom left port: <>
  Pin2 CPU_TRI_D4 = Pin2::HIZ_PU;   // bottom left port: <>
  Pin2 CPU_TRI_D5 = Pin2::HIZ_PU;   // bottom left port: <>
  Pin2 CPU_TRI_D6 = Pin2::HIZ_PU;   // bottom left port: <>
  Pin2 CPU_TRI_D7 = Pin2::HIZ_PU;   // bottom left port: <>

private:

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  Reg _CPU_PIN_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  Reg _CPU_PIN_ADDR_HI;       // top right port PORTA_03: <- P25.SYRO // Not really sure why this is here

  //-----------------------------------------------------------------------------
  // CPU-to-SOC control signals

  CpuPinIn  _CPU_PIN6;              // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  CpuPinIn  _CPU_PIN5;              // top left port PORTD_06: -> ANUJ (FROM_CPU5). Probably "DATA_VALIDn"

  /*p04.MAKA*/ Reg _MAKA_FROM_CPU5_SYNC;

  // Main bus
  CpuPinIn  _CPU_PIN_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  CpuPinIn  _CPU_PIN_WRp;           // top right port PORTA_01: ->
  CpuPinIn  _CPU_PIN_ADDR_VALID;    // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.

};

//-----------------------------------------------------------------------------

}; // namespace Schematics