#include "Sch_Decoder.h"

#include "Sch_Bootrom.h"
#include "Sch_BusControl.h"
#include "Sch_CpuSignals.h"
#include "Sch_Clocks.h"
#include "Sch_Debug.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Decoder::tick(const Bus& bus,
                   const ClockRegisters& clocks,
                   wire BOOT_BIT,
                   wire MODE_DBG2,
                   wire ADDR_VALID) {
  Decoder& next = *this;

  /*p10.AMUS*/ next.ADDR_xxxxxxxx0xx00000 = nor(bus.A00, bus.A01, bus.A02, bus.A03, bus.A04, bus.A07);
  /*p07.TUNA*/ next.ADDR_0000_FE00 = nand(bus.A15, bus.A14, bus.A13, bus.A12, bus.A11, bus.A10, bus.A09);
  /*p07.TONA*/ next.ADDR_08n = not(bus.A08);
  /*p07.SYKE*/ next.ADDR_FFXX = nor(next.ADDR_0000_FE00, next.ADDR_08n);
  /*p10.ANAP*/ next.ADDR_111111110xx00000 = and(next.ADDR_xxxxxxxx0xx00000, next.ADDR_FFXX);

  /*p22.XOLA*/ wire A00n = not(bus.A00);
  /*p22.XENO*/ wire A01n = not(bus.A01);
  /*p22.XUSY*/ wire A02n = not(bus.A02);
  /*p22.XERA*/ wire A03n = not(bus.A03);
  /*p22.XALY*/ wire ADDR_0x00xxxx  = nor(bus.A07, bus.A05, bus.A04);
  /*p22.WUTU*/ wire FF4Xn          = nand(next.ADDR_FFXX, bus.A06, ADDR_0x00xxxx);
  /*p22.WERO*/ wire FF4X           = not(FF4Xn);

  /*p07.SEMY*/ next.ADDR_XX0X = nor(bus.A07, bus.A06, bus.A05, bus.A04);
  /*p07.SAPA*/ next.ADDR_XXXF = and(bus.A00, bus.A01, bus.A02, bus.A03);

  /*p22.WADO*/ wire WADO_A00 = not(A00n);
  /*p22.WESA*/ wire WESA_A01 = not(A01n);
  /*p22.WALO*/ wire WALO_A02 = not(A02n);
  /*p22.WEPO*/ wire WEPO_A03 = not(A03n);

  /*p22.WORU*/ wire FF40n = nand(FF4X, A00n, A01n, A02n, A03n);
  /*p22.WOFA*/ wire FF41n = nand(FF4X, WADO_A00, A01n, A02n, A03n);
  /*p22.WEBU*/ wire FF42n = nand(FF4X, A00n, bus.A01, A02n, A03n);
  /*p22.WAVU*/ wire FF43n = nand(FF4X, bus.A00, bus.A01, A02n, A03n);
  /*p22.WYLE*/ wire FF44n = nand(FF4X, A00n, A01n, WALO_A02,  A03n);
  /*p22.WETY*/ wire FF45n = nand(FF4X, bus.A00, A01n, bus.A02, A03n);
  /*p22.WATE*/ wire FF46n = nand(FF4X, A00n, WESA_A01, WALO_A02, A03n);
  /*p22.WYBO*/ wire FF47n = nand(FF4X, bus.A00, bus.A01, bus.A02, A03n);
  /*p22.WETA*/ wire FF48n = nand(FF4X, A00n, A01n, A02n, bus.A03);
  /*p22.VAMA*/ wire FF49n = nand(FF4X, bus.A00, A01n, A02n, bus.A03);
  /*p22.WYVO*/ wire FF4An = nand(FF4X, A00n, bus.A01, A02n, bus.A03);
  /*p22.WAGE*/ wire FF4Bn = nand(FF4X, bus.A00, bus.A01, A02n, bus.A03);

  /*p22.VOCA*/ next.FF40 = not(FF40n);
  /*p22.VARY*/ next.FF41 = not(FF41n);
  /*p22.XARO*/ next.FF42 = not(FF42n);
  /*p22.XAVY*/ next.FF43 = not(FF43n);
  /*p22.XOGY*/ next.FF44 = not(FF44n);
  /*p22.XAYU*/ next.FF45 = not(FF45n);
  /*p22.XEDA*/ next.FF46 = not(FF46n);
  /*p22.WERA*/ next.FF47 = not(FF47n);
  /*p22.XAYO*/ next.FF48 = not(FF48n);
  /*p22.TEGO*/ next.FF49 = not(FF49n);
  /*p22.VYGA*/ next.FF4A = not(FF4An);
  /*p22.VUMY*/ next.FF4B = not(FF4Bn);

  /*p07.TULO*/ wire ADDR_00XX  = nor(bus.A15, bus.A14, bus.A13, bus.A12, bus.A11, bus.A10, bus.A09, bus.A08);
  /*p07.TERA*/ wire BOOT_BITn  = not(BOOT_BIT);
  /*p07.TUTU*/ next.ADDR_BOOT  = and(BOOT_BITn, ADDR_00XX);

  /*p07.RYCU*/ wire ADDR_FE00_FFFF = not(next.ADDR_0000_FE00);
  /*p07.SOHA*/ wire ADDR_FFXXn2 = not(next.ADDR_FFXX);
  /*p07.ROPE*/ wire ADDR_FEXXn = nand(ADDR_FE00_FFFF, ADDR_FFXXn2);
  /*p07.SARO*/ next.ADDR_OAM = not(ADDR_FEXXn);


  /*p08.SORE*/ wire SORE = not(bus.A15);
  /*p08.TEVY*/ next.ADDR_NOT_VRAM = or(bus.A13, bus.A14, SORE);
  /*p08.TEXO*/ next.ADDR_VALID_AND_NOT_VRAM = and(ADDR_VALID, next.ADDR_NOT_VRAM);
  /*p08.LEVO*/ next.ADDR_VALID_AND_NOT_VRAMn = not(next.ADDR_VALID_AND_NOT_VRAM);

  /*p25.TEFA*/ wire TEFA = nor(ADDR_FE00_FFFF, next.ADDR_VALID_AND_NOT_VRAM);
  /*p25.SOSE*/ next.ADDR_VRAM = and(bus.A15, TEFA);

  /*p01.ATYP*/ wire PHASE_xBCDExxx2 = not(!clocks.PHAZ_xBCDExxx);  
  /*p01.AROV*/ wire PHASE_xxxDEFGx2 = not(!clocks.PHAZ_xxxDEFGx);
  /*p01.AJAX*/ wire PHASE_AxxxxFGH3 = not(PHASE_xBCDExxx2);
  /*p01.AGUT*/ wire AGUT_AxxDEFGH = and(or(PHASE_AxxxxFGH3, PHASE_xxxDEFGx2), ADDR_VALID);
  /*p01.AWOD*/ wire AWOD = or(MODE_DBG2, AGUT_AxxDEFGH);
  /*p01.ABUZ*/ next.ADDR_VALID_xBCxxxxx = not(AWOD);

  /*p03.TOVY*/ next.TOVY_A00n = not(bus.A00);
  /*p08.TOLA*/ next.TOLA_A01n = not(bus.A01);
  /*p06.SEFY*/ next.SEFY_A02n = not(bus.A02);

  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(bus.A07, bus.A06, bus.A05, bus.A04, bus.A03);
  /*p06.SANO*/ next.ADDR_FF00_FF03 = and(ADDR_XX00_XX07, next.SEFY_A02n, next.ADDR_FFXX);

  /*p03.RYFO*/ next.FF04_FF07 = and(bus.A02, ADDR_XX00_XX07, next.ADDR_FFXX);

#if 0
  bool HRAM_CS;
  // addr >= FF80 and not XXFF
  /*p07.WALE*/ wire ADDR_x1111111n = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
  /*p07.WOLY*/ wire WOLY = nand(b.dec.ADDR_FFXX, b.A07, ADDR_x1111111n);
  /*p07.WUTA*/ c.hrm.HRAM_CS = not(WOLY);
#endif
}

//-----------------------------------------------------------------------------

};