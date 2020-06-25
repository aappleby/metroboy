#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct AddressSignals {
  /*p03.TOVY*/ wire TOVY_A00n;
  /*p08.TOLA*/ wire TOLA_A01n;
  /*p06.SEFY*/ wire SEFY_A02n;
  /*p07.TONA*/ wire TONA_A08n;

  /*p22.XOLA*/ wire XOLA_A00n;
  /*p22.XENO*/ wire XENO_A01n;
  /*p22.XUSY*/ wire XUSY_A02n;
  /*p22.XERA*/ wire XERA_A03n;
  /*p10.BYKO*/ wire BYKO_A05n;
  /*p10.AKUG*/ wire AKUG_A06n;

  /*p22.WADO*/ wire WADO_A00;
  /*p22.WESA*/ wire WESA_A01;
  /*p22.WALO*/ wire WALO_A02;
  /*p22.WEPO*/ wire WEPO_A03;

  /*p07.TUNA*/ wire TUNA_0000_FDFFp;
  /*p06.SARE*/ wire SARE_XX00_XX07p;
  /*p07.SYKE*/ wire SYKE_FF00_FFFFp;
  /*p25.SYRO*/ wire SYRO_FE00_FFFFp;
  /*p03.RYFO*/ wire RYFO_FF04_FF07p;
  /*p07.SARO*/ wire SARO_FE00_FEFFp;
  /*p22.WERO*/ wire WERO_FF40_FF4Fp;
  /*p25.SOSE*/ wire SOSE_8000_9FFFp;
  /*p08.TEXO*/ wire TEXO_8000_9FFFn;
  /*p08.LEVO*/ wire LEVO_8000_9FFFp;

  /*p22.XEDA*/ wire XEDA_FF46p;
};


struct AddressRegisters {
  void tick(const TestGB& gb);
  AddressSignals sig(const CpuPins& cpu_pins) const;
  bool commit();
};

//-----------------------------------------------------------------------------

}; // namespace Schematics