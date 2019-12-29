#include "Sch_Debug.h"

#include "Sch_BusControl.h"
#include "Sch_Decoder.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"
#include "Sch_Resets.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Debug::tick(const Bus& bus,
                 const BusControl& ctl,
                 const ClockSignals1& clk,
                 const Pins& pins,
                 const Decoder& dec,
                 const ResetSignals1& rst_sig,
                 const Debug& prev,
                 Bus& bus_out) {

  Debug& next = *this;

  /*p27.VYPO*/ wire VYPO = not(pins.P10_B);
  /*p07.UBET*/ next.T1n = not(pins.T1);
  /*p07.UVAR*/ next.T2n = not(pins.T2);

  /*p07.UPOJ*/ next.MODE_PROD   = nand(prev.T1n, prev.T2n, pins.RST);
  /*p07.UMUT*/ next.MODE_DBG1   = and(pins.T1, prev.T2n);
  /*p07.UNOR*/ next.MODE_DBG2   = and(pins.T2, prev.T1n);
  /*p08.TOVA*/ next.MODE_DBG2n1 = not(prev.MODE_DBG2);
  /*p08.RYCA*/ next.MODE_DBG2n2 = not(prev.MODE_DBG2);

  // so is one of the debug pins a clock, and this is a clock divider?
  /*p25.SYCY*/ wire CLK_SOTO = not(prev.MODE_DBG2);
  /*p25.SOTO*/ next.DBG_SOTO.tock(CLK_SOTO, rst_sig.SOTO_RESET, !prev.DBG_SOTO);

  /*p25.TUTO*/ next.DBG_VRAM  = and(prev.MODE_DBG2, !prev.DBG_SOTO);
  /*p25.RACO*/ next.DBG_VRAMn = not(prev.DBG_VRAM);

  //----------
  // weird debug things, probably not right

  /*p05.AXYN*/ wire AXYN_xBCDEFGH = not(clk.BEDO_xBxxxxxx);
  /*p05.ADYR*/ wire ADYR_Axxxxxxx = not(AXYN_xBCDEFGH);
  /*p05.APYS*/ wire APYS_xBCDEFGH = nor(prev.MODE_DBG2, ADYR_Axxxxxxx);
  /*p05.AFOP*/ wire AFOP_Axxxxxxx = not(APYS_xBCDEFGH);
  /*p07.LECO*/ wire LECO_xBCDEFGH = nor(clk.BEDO_xBxxxxxx, prev.MODE_DBG2);

  /*p05.ANOC*/ if (AFOP_Axxxxxxx) bus_out.D0 = not(pins.P10_B);
  /*p05.ATAJ*/ if (AFOP_Axxxxxxx) bus_out.D1 = not(pins.P10_B);
  /*p05.AJEC*/ if (AFOP_Axxxxxxx) bus_out.D2 = not(pins.P10_B);
  /*p05.ASUZ*/ if (AFOP_Axxxxxxx) bus_out.D3 = not(pins.P10_B);
  /*p05.BENU*/ if (AFOP_Axxxxxxx) bus_out.D4 = not(pins.P10_B);
  /*p05.AKAJ*/ if (AFOP_Axxxxxxx) bus_out.D5 = not(pins.P10_B);
  /*p05.ARAR*/ if (AFOP_Axxxxxxx) bus_out.D6 = not(pins.P10_B);
  /*p05.BEDA*/ if (AFOP_Axxxxxxx) bus_out.D7 = not(pins.P10_B);

  /*p07.ROMY*/ if (LECO_xBCDEFGH) bus_out.D0 = pins.P10_B;
  /*p07.RYNE*/ if (LECO_xBCDEFGH) bus_out.D1 = pins.P10_B;
  /*p07.REJY*/ if (LECO_xBCDEFGH) bus_out.D2 = pins.P10_B;
  /*p07.RASE*/ if (LECO_xBCDEFGH) bus_out.D3 = pins.P10_B;
  /*p07.REKA*/ if (LECO_xBCDEFGH) bus_out.D4 = pins.P10_B;
  /*p07.ROWE*/ if (LECO_xBCDEFGH) bus_out.D5 = pins.P10_B;
  /*p07.RYKE*/ if (LECO_xBCDEFGH) bus_out.D6 = pins.P10_B;
  /*p07.RARU*/ if (LECO_xBCDEFGH) bus_out.D7 = pins.P10_B;

  // FF60 debug reg

  /*p07.APET*/ wire MODE_DEBUG = or(prev.MODE_DBG1, prev.MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand(MODE_DEBUG, bus.A05, bus.A06, ctl.CPU_WR, dec.ADDR_111111110xx00000);
  /*p07.BURO*/ next.FF60_0.tock(FF60_WRn, rst_sig.SYS_RESETn, bus.D0);
  /*p07.AMUT*/ next.FF60_1.tock(FF60_WRn, rst_sig.SYS_RESETn, bus.D1);

  /*p05.KURA*/ next.FF60_0n = not(prev.FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not(prev.FF60_0);
  /*p05.KORE*/ next.P05_NC0 = nand(prev.DBG_FF00_D7, prev.FF60_0);
  /*p05.KYWE*/ next.P05_NC1 = nor (prev.DBG_FF00_D7, FF60_0o);

  /*p08.LYRA*/ wire DBG_D_RDn = nand(prev.MODE_DBG2, ctl.CBUS_TO_CEXTn);
  /*p08.TUTY*/ if (!DBG_D_RDn) bus_out.D0 = not(/*p08.TOVO*/ not(pins.D0_C));
  /*p08.SYWA*/ if (!DBG_D_RDn) bus_out.D1 = not(/*p08.RUZY*/ not(pins.D1_C));
  /*p08.SUGU*/ if (!DBG_D_RDn) bus_out.D2 = not(/*p08.ROME*/ not(pins.D2_C));
  /*p08.TAWO*/ if (!DBG_D_RDn) bus_out.D3 = not(/*p08.SAZA*/ not(pins.D3_C));
  /*p08.TUTE*/ if (!DBG_D_RDn) bus_out.D4 = not(/*p08.TEHE*/ not(pins.D4_C));
  /*p08.SAJO*/ if (!DBG_D_RDn) bus_out.D5 = not(/*p08.RATU*/ not(pins.D5_C));
  /*p08.TEMY*/ if (!DBG_D_RDn) bus_out.D6 = not(/*p08.SOCA*/ not(pins.D6_C));
  /*p08.ROPA*/ if (!DBG_D_RDn) bus_out.D7 = not(/*p08.RYBA*/ not(pins.D7_C));

  //----------
  // if NET01 high, drive external address bus onto internal address
  // these may be backwards, probably don't want to drive external address onto bus normally...

  /*p08.KOVA*/ wire A00_Cn = not(pins.A00_C);
  /*p08.CAMU*/ wire A01_Cn = not(pins.A01_C);
  /*p08.BUXU*/ wire A02_Cn = not(pins.A02_C);
  /*p08.BASE*/ wire A03_Cn = not(pins.A03_C);
  /*p08.AFEC*/ wire A04_Cn = not(pins.A04_C);
  /*p08.ABUP*/ wire A05_Cn = not(pins.A05_C);
  /*p08.CYGU*/ wire A06_Cn = not(pins.A06_C);
  /*p08.COGO*/ wire A07_Cn = not(pins.A07_C);
  /*p08.MUJY*/ wire A08_Cn = not(pins.A08_C);
  /*p08.NENA*/ wire A09_Cn = not(pins.A09_C);
  /*p08.SURA*/ wire A10_Cn = not(pins.A10_C);
  /*p08.MADY*/ wire A11_Cn = not(pins.A11_C);
  /*p08.LAHE*/ wire A12_Cn = not(pins.A12_C);
  /*p08.LURA*/ wire A13_Cn = not(pins.A13_C);
  /*p08.PEVO*/ wire A14_Cn = not(pins.A14_C);
  /*p08.RAZA*/ wire A15_Cn = not(pins.A15_C);

  /*p08.KEJO*/ if (prev.MODE_DBG2n1) bus_out.A00 = not(A00_Cn);
  /*p08.BYXE*/ if (prev.MODE_DBG2n1) bus_out.A01 = not(A01_Cn);
  /*p08.AKAN*/ if (prev.MODE_DBG2n1) bus_out.A02 = not(A02_Cn);
  /*p08.ANAR*/ if (prev.MODE_DBG2n1) bus_out.A03 = not(A03_Cn);
  /*p08.AZUV*/ if (prev.MODE_DBG2n1) bus_out.A04 = not(A04_Cn);
  /*p08.AJOV*/ if (prev.MODE_DBG2n1) bus_out.A05 = not(A05_Cn);
  /*p08.BYNE*/ if (prev.MODE_DBG2n1) bus_out.A06 = not(A06_Cn);
  /*p08.BYNA*/ if (prev.MODE_DBG2n1) bus_out.A07 = not(A07_Cn);
  /*p08.LOFA*/ if (prev.MODE_DBG2n1) bus_out.A08 = not(A08_Cn);
  /*p08.MAPU*/ if (prev.MODE_DBG2n1) bus_out.A09 = not(A09_Cn);
  /*p08.RALA*/ if (prev.MODE_DBG2n1) bus_out.A10 = not(A10_Cn);
  /*p08.LORA*/ if (prev.MODE_DBG2n1) bus_out.A11 = not(A11_Cn);
  /*p08.LYNA*/ if (prev.MODE_DBG2n1) bus_out.A12 = not(A12_Cn);
  /*p08.LEFY*/ if (prev.MODE_DBG2n1) bus_out.A13 = not(A13_Cn);
  /*p08.NEFE*/ if (prev.MODE_DBG2n1) bus_out.A14 = not(A14_Cn);
  /*p08.SYZU*/ if (prev.MODE_DBG2n2) bus_out.A15 = not(A15_Cn);

  //----------
  // more debug stuff

  /*p25.TUSO*/ wire TUSO = nor(prev.MODE_DBG2, clk.BOGA_AxCDEFGH);
  /*p25.SOLE*/ wire SOLE = not(TUSO);

  /*p25.TOVU*/ if (VYPO) bus_out.D0 = SOLE;
  /*p25.SOSA*/ if (VYPO) bus_out.D1 = SOLE;
  /*p25.SEDU*/ if (VYPO) bus_out.D2 = SOLE;
  /*p25.TAXO*/ if (VYPO) bus_out.D3 = SOLE;
  /*p25.TAHY*/ if (VYPO) bus_out.D4 = SOLE;
  /*p25.TESU*/ if (VYPO) bus_out.D5 = SOLE;
  /*p25.TAZU*/ if (VYPO) bus_out.D6 = SOLE;
  /*p25.TEWA*/ if (VYPO) bus_out.D7 = SOLE;
}

//-----------------------------------------------------------------------------

};