#include "Sch_Decoder.h"

#include "Sch_Bootrom.h"
#include "Sch_BusControl.h"
#include "Sch_CpuSignals.h"
#include "Sch_Clocks.h"
#include "Sch_Debug.h"
#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

Decoder Decoder::tick(const SysSignals&   sys_sig,
                      const ClkSignals&   clk_sig,
                      const BusTristates& bus_tri)
{
  /*p10.AMUS*/ wire ADDR_0xx00000 = nor(bus_tri.A00, bus_tri.A01, bus_tri.A02, bus_tri.A03, bus_tri.A04, bus_tri.A07);
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(bus_tri.A15, bus_tri.A14, bus_tri.A13, bus_tri.A12, bus_tri.A11, bus_tri.A10, bus_tri.A09);
  /*p07.TONA*/ wire ADDR_08n = not(bus_tri.A08);
  /*p07.SYKE*/ wire ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);
  /*p10.ANAP*/ wire ADDR_111111110xx00000 = and(ADDR_0xx00000, ADDR_FFXX);

  /*p22.XOLA*/ wire A00n = not(bus_tri.A00);
  /*p22.XENO*/ wire A01n = not(bus_tri.A01);
  /*p22.XUSY*/ wire A02n = not(bus_tri.A02);
  /*p22.XERA*/ wire A03n = not(bus_tri.A03);
  /*p22.XALY*/ wire ADDR_0x00xxxx  = nor(bus_tri.A07, bus_tri.A05, bus_tri.A04);
  /*p22.WUTU*/ wire FF4Xn          = nand(ADDR_FFXX, bus_tri.A06, ADDR_0x00xxxx);
  /*p22.WERO*/ wire FF4X           = not(FF4Xn);

  /*p07.SEMY*/ wire ADDR_XX0X = nor(bus_tri.A07, bus_tri.A06, bus_tri.A05, bus_tri.A04);
  /*p07.SAPA*/ wire ADDR_XXXF = and(bus_tri.A00, bus_tri.A01, bus_tri.A02, bus_tri.A03);

  /*p22.WADO*/ wire WADO_A00 = not(A00n);
  /*p22.WESA*/ wire WESA_A01 = not(A01n);
  /*p22.WALO*/ wire WALO_A02 = not(A02n);
  /*p22.WEPO*/ wire WEPO_A03 = not(A03n);

  /*p22.WORU*/ wire FF40n = nand(FF4X, A00n, A01n, A02n, A03n);
  /*p22.WOFA*/ wire FF41n = nand(FF4X, WADO_A00, A01n, A02n, A03n);
  /*p22.WEBU*/ wire FF42n = nand(FF4X, A00n, bus_tri.A01, A02n, A03n);
  /*p22.WAVU*/ wire FF43n = nand(FF4X, bus_tri.A00, bus_tri.A01, A02n, A03n);
  /*p22.WYLE*/ wire FF44n = nand(FF4X, A00n, A01n, WALO_A02,  A03n);
  /*p22.WETY*/ wire FF45n = nand(FF4X, bus_tri.A00, A01n, bus_tri.A02, A03n);
  /*p22.WATE*/ wire FF46n = nand(FF4X, A00n, WESA_A01, WALO_A02, A03n);
  /*p22.WYBO*/ wire FF47n = nand(FF4X, bus_tri.A00, bus_tri.A01, bus_tri.A02, A03n);
  /*p22.WETA*/ wire FF48n = nand(FF4X, A00n, A01n, A02n, bus_tri.A03);
  /*p22.VAMA*/ wire FF49n = nand(FF4X, bus_tri.A00, A01n, A02n, bus_tri.A03);
  /*p22.WYVO*/ wire FF4An = nand(FF4X, A00n, bus_tri.A01, A02n, bus_tri.A03);
  /*p22.WAGE*/ wire FF4Bn = nand(FF4X, bus_tri.A00, bus_tri.A01, A02n, bus_tri.A03);

  /*p22.VOCA*/ wire FF40 = not(FF40n);
  /*p22.VARY*/ wire FF41 = not(FF41n);
  /*p22.XARO*/ wire FF42 = not(FF42n);
  /*p22.XAVY*/ wire FF43 = not(FF43n);
  /*p22.XOGY*/ wire FF44 = not(FF44n);
  /*p22.XAYU*/ wire FF45 = not(FF45n);
  /*p22.XEDA*/ wire FF46 = not(FF46n);
  /*p22.WERA*/ wire FF47 = not(FF47n);
  /*p22.XAYO*/ wire FF48 = not(FF48n);
  /*p22.TEGO*/ wire FF49 = not(FF49n);
  /*p22.VYGA*/ wire FF4A = not(FF4An);
  /*p22.VUMY*/ wire FF4B = not(FF4Bn);

  /*p07.TULO*/ wire ADDR_00XX  = nor(bus_tri.A15, bus_tri.A14, bus_tri.A13, bus_tri.A12, bus_tri.A11, bus_tri.A10, bus_tri.A09, bus_tri.A08);
  /*p07.TERA*/ wire BOOT_BITn  = not(sys_sig.BOOT_BIT);
  /*p07.TUTU*/ wire ADDR_BOOT  = and(BOOT_BITn, ADDR_00XX);

  /*p07.RYCU*/ wire ADDR_FE00_FFFF = not(ADDR_0000_FE00);
  /*p07.SOHA*/ wire ADDR_FFXXn2 = not(ADDR_FFXX);
  /*p07.ROPE*/ wire ADDR_FEXXn = nand(ADDR_FE00_FFFF, ADDR_FFXXn2);
  /*p07.SARO*/ wire ADDR_OAM = not(ADDR_FEXXn);

  /*p01.AGUT*/ wire AGUT_xxCDEFGH = and(or(clk_sig.AJAX_xxxxEFGH, clk_sig.AROV_xxCDEFxx), sys_sig.CPU_ADDR_VALID);
  /*p01.AWOD*/ wire AWOD = or(sys_sig.PIN_MODE_DBG2, AGUT_xxCDEFGH);
  /*p01.ABUZ*/ wire ADDR_VALID_xBCxxxxx = not(AWOD);

  /*p03.TOVY*/ wire TOVY_A00n = not(bus_tri.A00);
  /*p08.TOLA*/ wire TOLA_A01n = not(bus_tri.A01);
  /*p06.SEFY*/ wire SEFY_A02n = not(bus_tri.A02);

  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(bus_tri.A07, bus_tri.A06, bus_tri.A05, bus_tri.A04, bus_tri.A03);
  /*p06.SANO*/ wire ADDR_FF00_FF03 = and(ADDR_XX00_XX07, SEFY_A02n, ADDR_FFXX);

  /*p03.RYFO*/ wire FF04_FF07 = and(bus_tri.A02, ADDR_XX00_XX07, ADDR_FFXX);

#if 0
  bool HRAM_CS;
  // addr >= FF80 and not XXFF
  /*p07.WALE*/ wire ADDR_x1111111n = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
  /*p07.WOLY*/ wire WOLY = nand(b.dec.ADDR_FFXX, b.A07, ADDR_x1111111n);
  /*p07.WUTA*/ c.hrm.HRAM_CS = not(WOLY);
#endif


  return {
    /*p10.AMUS*/ .ADDR_0xx00000 = ADDR_0xx00000,
    /*p07.TUNA*/ .ADDR_0000_FE00 = ADDR_0000_FE00,
    /*p07.TONA*/ .ADDR_08n = ADDR_08n,
    /*p10.ANAP*/ .ADDR_111111110xx00000 = ADDR_111111110xx00000,
    /*p07.SYKE*/ .ADDR_FFXX = ADDR_FFXX,

    /*p07.SEMY*/ .ADDR_XX0X = ADDR_XX0X,
    /*p07.SAPA*/ .ADDR_XXXF = ADDR_XXXF,

    /*p22.VOCA*/ .FF40 = FF40,
    /*p22.VARY*/ .FF41 = FF41,
    /*p22.XARO*/ .FF42 = FF42,
    /*p22.XAVY*/ .FF43 = FF43,
    /*p22.XOGY*/ .FF44 = FF44,
    /*p22.XAYU*/ .FF45 = FF45,
    /*p22.XAYU*/ .FF46 = FF46,
    /*p22.WERA*/ .FF47 = FF47,
    /*p22.XAYO*/ .FF48 = FF48,
    /*p22.TEGO*/ .FF49 = FF49,
    /*p22.VYGA*/ .FF4A = FF4A,
    /*p22.VUMY*/ .FF4B = FF4B,

    /*p07.TUTU*/ .ADDR_BOOT = ADDR_BOOT,
    /*p01.ABUZ*/ .ADDR_VALID_xBCxxxxx = ADDR_VALID_xBCxxxxx,

    /*p03.TOVY*/ .TOVY_A00n = TOVY_A00n,
    /*p08.TOLA*/ .TOLA_A01n = TOLA_A01n,
    /*p06.SEFY*/ .SEFY_A02n = SEFY_A02n,

    /*p06.SANO*/ .ADDR_FF00_FF03 = ADDR_FF00_FF03,

    /*p03.RYFO*/ .FF04_FF07 = FF04_FF07,

    /*p07.SARO*/ .ADDR_OAM = ADDR_OAM,
  };


}

//-----------------------------------------------------------------------------

};