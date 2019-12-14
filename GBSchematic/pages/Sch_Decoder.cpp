#include "Sch_Decoder.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Decoders_tick(const Bus& bus,
                   Decoder& next) {
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
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(bus.A15, bus.A14, bus.A13, bus.A12, bus.A11, bus.A10, bus.A09);
  /*p07.TONA*/ wire ADDR_08n       = not(bus.A08);
  /*p07.SYKE*/ wire ADDR_FFXX      = nor(ADDR_0000_FE00, ADDR_08n);
  /*p22.WUTU*/ wire FF4Xn          = nand(ADDR_FFXX, bus.A06, ADDR_0x00xxxx);
  /*p22.WERO*/ wire FF4X           = not(FF4Xn);

  /*p22.WORU*/ wire FF40n = nand(FF4X, A00n, A01n, A02n, A03n);
  /*p22.WEBU*/ wire FF42n = nand(FF4X, A00n, bus.A01, A02n, A03n);
  /*p22.WAVU*/ wire FF43n = nand(FF4X, bus.A00, bus.A01, A02n, A03n);
  /*p22.WETY*/ wire FF45n = nand(FF4X, bus.A00, A01n, bus.A02, A03n);
  /*p22.WYBO*/ wire FF47n = nand(FF4X, bus.A00, bus.A01, bus.A02, A03n);
  /*p22.WETA*/ wire FF48n = nand(FF4X, A00n, A01n, A02n, bus.A03);
  /*p22.VAMA*/ wire FF49n = nand(FF4X, bus.A00, A01n, A02n, bus.A03);
  /*p22.WYVO*/ wire FF4An = nand(FF4X, A00n, bus.A01, A02n, bus.A03);
  /*p22.WAGE*/ wire FF4Bn = nand(FF4X, bus.A00, bus.A01, A02n, bus.A03);

  /*p22.VOCA*/ next.FF40 = not(FF40n);
  /*p22.XARO*/ next.FF42 = not(FF42n);
  /*p22.XAVY*/ next.FF43 = not(FF43n);
  /*p22.XAYU*/ next.FF45 = not(FF45n);
  /*p22.WERA*/ next.FF47 = not(FF47n);
  /*p22.XAYO*/ next.FF48 = not(FF48n);
  /*p22.TEGO*/ next.FF49 = not(FF49n);
  /*p22.VYGA*/ next.FF4A = not(FF4An);
  /*p22.VUMY*/ next.FF4B = not(FF4Bn);
}

//-----------------------------------------------------------------------------

};