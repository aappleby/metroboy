#include "Sch_Address.h"

#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void AddressRegisters::tick(const TestGB& /*gb*/) {
  //changed |= cpu_pins.SYRO.commit_pinout();          // PORTA_03: <- P25.SYRO
}

bool AddressRegisters::commit() {
  bool changed = false;
  return changed;
}

//-----------------------------------------------------------------------------

AddressSignals AddressRegisters::sig(const CpuBus& cpu_bus) const {

  /*p03.TOVY*/ wire _TOVY_A00n = not(cpu_bus.A00);
  /*p08.TOLA*/ wire _TOLA_A01n = not(cpu_bus.A01);
  /*p06.SEFY*/ wire _SEFY_A02n = not(cpu_bus.A02);
  /*p07.TONA*/ wire _TONA_A08n = not(cpu_bus.A08);

  /*p22.XOLA*/ wire _XOLA_A00n = not(cpu_bus.A00);
  /*p22.XENO*/ wire _XENO_A01n = not(cpu_bus.A01);
  /*p22.XUSY*/ wire _XUSY_A02n = not(cpu_bus.A02);
  /*p22.XERA*/ wire _XERA_A03n = not(cpu_bus.A03);
  /*p10.BYKO*/ wire _BYKO_A05n = not(cpu_bus.A05);
  /*p10.AKUG*/ wire _AKUG_A06n = not(cpu_bus.A06);

  /*p22.WADO*/ wire _WADO_A00 = not(_XOLA_A00n);
  /*p22.WESA*/ wire _WESA_A01p = not(_XENO_A01n);
  /*p22.WALO*/ wire _WALO_A02p = not(_XUSY_A02n);
  /*p22.WEPO*/ wire _WEPO_A03p = not(_XERA_A03n);

  /*p07.TUNA*/ wire _TUNA_0000_FDFFp = nand(cpu_bus.A15, cpu_bus.A14, cpu_bus.A13, cpu_bus.A12, cpu_bus.A11, cpu_bus.A10, cpu_bus.A09);
  /*p06.SARE*/ wire _SARE_XX00_XX07p = nor(cpu_bus.A07, cpu_bus.A06, cpu_bus.A05, cpu_bus.A04, cpu_bus.A03);
  /*p07.SYKE*/ wire _SYKE_FF00_FFFFp = nor(_TUNA_0000_FDFFp, _TONA_A08n);
  /*p25.SYRO*/ wire _SYRO_FE00_FFFFp = not(_TUNA_0000_FDFFp);

  /*p22.XALY*/ wire _XALY_0x00xxxxp = nor(cpu_bus.A07, cpu_bus.A05, cpu_bus.A04);
  /*p22.WUTU*/ wire _WUTU_FF40_FF4Fn = nand(_SYKE_FF00_FFFFp, cpu_bus.A06, _XALY_0x00xxxxp);
  /*p22.WERO*/ wire _WERO_FF40_FF4Fp = not(_WUTU_FF40_FF4Fn);

  /*p22.WATE*/ wire _WATE_FF46n = nand(_WERO_FF40_FF4Fp, _XOLA_A00n, _WESA_A01p, _WALO_A02p, _XERA_A03n);
  /*p22.XEDA*/ wire _XEDA_FF46p = not(_WATE_FF46n);
  /*p03.RYFO*/ wire _RYFO_FF04_FF07p = and (cpu_bus.A02, _SARE_XX00_XX07p, _SYKE_FF00_FFFFp);

  /*p07.RYCU*/ wire _RYCU_FE00_FFFFp = not(_TUNA_0000_FDFFp);
  /*p07.SOHA*/ wire _SOHA_FF00_FFFFn = not(_SYKE_FF00_FFFFp);
  /*p07.ROPE*/ wire _ROPE_FE00_FEFFn = nand(_RYCU_FE00_FFFFp, _SOHA_FF00_FFFFn);
  /*p07.SARO*/ wire _SARO_FE00_FEFFp = not(_ROPE_FE00_FEFFn);

  // TEVY box color wrong on die trace, but schematic correct.

  // Die trace:
  // SORE = not(A15)
  // TEVY = or(A13, A13, SORE) // A13 line not fully drawn
  // TEXO = and(ADDR_VALIDx?, TEVY)

  /*p08.SORE*/ wire _SORE_0000_7FFFp = not(cpu_bus.A15);
  /*p08.TEVY*/ wire _TEVY_8000_9FFFn = or(cpu_bus.A13, cpu_bus.A14, _SORE_0000_7FFFp);
  /*p08.TEXO*/ wire _TEXO_8000_9FFFn = and (cpu_bus.PIN_ADDR_VALID, _TEVY_8000_9FFFn);
  /*p08.LEVO*/ wire _LEVO_8000_9FFFp = not(_TEXO_8000_9FFFn);

  // the logic here is kinda weird, still seems to select vram.
  /*p25.TEFA*/ wire _TEFA_8000_9FFFp = nor(_SYRO_FE00_FFFFp, _TEXO_8000_9FFFn);
  /*p25.SOSE*/ wire _SOSE_8000_9FFFp = and (cpu_bus.A15, _TEFA_8000_9FFFp);

  return {
    .TOVY_A00n = _TOVY_A00n,
    .TOLA_A01n = _TOLA_A01n,
    .SEFY_A02n = _SEFY_A02n,
    .TONA_A08n = _TONA_A08n,

    .XOLA_A00n = _XOLA_A00n,
    .XENO_A01n = _XENO_A01n,
    .XUSY_A02n = _XUSY_A02n,
    .XERA_A03n = _XERA_A03n,
    .BYKO_A05n = _BYKO_A05n,
    .AKUG_A06n = _AKUG_A06n,

    .WADO_A00  = _WADO_A00,
    .WESA_A01  = _WESA_A01p,
    .WALO_A02  = _WALO_A02p,
    .WEPO_A03  = _WEPO_A03p,

    .TUNA_0000_FDFFp = _TUNA_0000_FDFFp,
    .SARE_XX00_XX07p = _SARE_XX00_XX07p,
    .SYKE_FF00_FFFFp = _SYKE_FF00_FFFFp,
    .SYRO_FE00_FFFFp = _SYRO_FE00_FFFFp,
    .RYFO_FF04_FF07p = _RYFO_FF04_FF07p,
    .SARO_FE00_FEFFp = _SARO_FE00_FEFFp,
    .WERO_FF40_FF4Fp = _WERO_FF40_FF4Fp,
    .SOSE_8000_9FFFp = _SOSE_8000_9FFFp,
    .TEXO_8000_9FFFn = _TEXO_8000_9FFFn,
    .LEVO_8000_9FFFp = _LEVO_8000_9FFFp,

    .XEDA_FF46p = _XEDA_FF46p,
  };
}

//-----------------------------------------------------------------------------
