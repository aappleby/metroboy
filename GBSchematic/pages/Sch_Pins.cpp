#include "Sch_Pins.h"

#include "Sch_DMA.h"
#include "Sch_Debug.h"
#include "Sch_Clocks.h"
#include "Sch_BusControl.h"
#include "Sch_CpuSignals.h"
#include "Sch_Decoder.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Pins::tick(Cpu& cpu,
                const Bus& bus,
                const BusControl& ctl,
                const Debug& dbg,
                const ClockSignals1& clk,
                const Decoder& dec,
                const DMA& dma) {

  Pins& next = *this;

  {
    /*p08.SOGY*/ wire SOGY = not(bus.A14);
    /*p08.TUMA*/ wire A000_BFFF = and(bus.A13, SOGY, bus.A15);
    /*p08.TYNU*/ wire A000_FFFF = or(and(bus.A15, bus.A14), A000_BFFF);
    /*p08.TOZA*/ wire A000_FDFF_ABxxxxxx = and(dec.ADDR_VALID_xBCxxxxx, A000_FFFF, dec.ADDR_0000_FE00);
    /*p08.TYHO*/ next.CS_A = mux2(dma.DMA_A15, A000_FDFF_ABxxxxxx, dma.DMA_READ_CART); // ABxxxxxx
  }

  {
    /*p08.MOCA*/ wire DBG_EXT_RDn = nor(dec.ADDR_VALID_AND_NOT_VRAM, dbg.MODE_DBG1);
    /*p08.LAGU*/ wire LAGU = or(and(cpu.CPU_RAW_RD, dec.ADDR_VALID_AND_NOT_VRAMn), cpu.CPU_RAW_WR);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
    /*p08.TYMU*/ wire RD_OUT = nor(dma.DMA_READ_CART, CPU_EXT_RD);

    /*p08.UGAC*/ next.RD_A = nand(RD_OUT, dbg.MODE_DBG2n1);
    /*p08.URUN*/ next.RD_D = nor (RD_OUT, dbg.MODE_DBG2);
  }

  {
    /*p01.AREV*/ wire AREV = nand(cpu.CPU_RAW_WR, clk.AFAS_xxxxxFGH);
    /*p01.APOV*/ wire CPU_WR_xxxxEFGx  = not(AREV);
    /*p08.MOCA*/ wire DBG_EXT_RDn = nor(dec.ADDR_VALID_AND_NOT_VRAM, dbg.MODE_DBG1);
    /*p08.MEXO*/ wire MEXO_ABCDxxxH = not(CPU_WR_xxxxEFGx);
    /*p08.NEVY*/ wire NEVY = or(MEXO_ABCDxxxH, DBG_EXT_RDn);
    /*p08.PUVA*/ wire WR_OUT = or(NEVY, dma.DMA_READ_CART);

    /*p08.UVER*/ next.WR_A = nand(WR_OUT, dbg.MODE_DBG2n1);
    /*p08.USUF*/ next.WR_D = nor (WR_OUT, dbg.MODE_DBG2);
  }

  //----------
  // Address pin driver

  {
    /*p08.KUPO*/ next.A00_A = nand(ctl.ADDR_OUT_00, dbg.MODE_DBG2n1);
    /*p08.CABA*/ next.A01_A = nand(ctl.ADDR_OUT_01, dbg.MODE_DBG2n1);
    /*p08.BOKU*/ next.A02_A = nand(ctl.ADDR_OUT_02, dbg.MODE_DBG2n1);
    /*p08.BOTY*/ next.A03_A = nand(ctl.ADDR_OUT_03, dbg.MODE_DBG2n1);
    /*p08.BYLA*/ next.A04_A = nand(ctl.ADDR_OUT_04, dbg.MODE_DBG2n1);
    /*p08.BADU*/ next.A05_A = nand(ctl.ADDR_OUT_05, dbg.MODE_DBG2n1);
    /*p08.CEPU*/ next.A06_A = nand(ctl.ADDR_OUT_06, dbg.MODE_DBG2n1);
    /*p08.DEFY*/ next.A07_A = nand(ctl.ADDR_OUT_07, dbg.MODE_DBG2n1);
    /*p08.MYNY*/ next.A08_A = nand(ctl.ADDR_OUT_08, dbg.MODE_DBG2n1);
    /*p08.MUNE*/ next.A09_A = nand(ctl.ADDR_OUT_09, dbg.MODE_DBG2n1);
    /*p08.ROXU*/ next.A10_A = nand(ctl.ADDR_OUT_10, dbg.MODE_DBG2n1);
    /*p08.LEPY*/ next.A11_A = nand(ctl.ADDR_OUT_11, dbg.MODE_DBG2n1);
    /*p08.LUCE*/ next.A12_A = nand(ctl.ADDR_OUT_12, dbg.MODE_DBG2n1);
    /*p08.LABE*/ next.A13_A = nand(ctl.ADDR_OUT_13, dbg.MODE_DBG2n1);
    /*p08.PUHE*/ next.A14_A = nand(ctl.ADDR_OUT_14, dbg.MODE_DBG2n1);
    /*p08.SUZE*/ next.A15_A = nand(ctl.ADDR_OUT_15, dbg.MODE_DBG2n2);

    /*p08.KOTY*/ next.A00_D = nor (ctl.ADDR_OUT_00, dbg.MODE_DBG2);
    /*p08.COTU*/ next.A01_D = nor (ctl.ADDR_OUT_01, dbg.MODE_DBG2);
    /*p08.BAJO*/ next.A02_D = nor (ctl.ADDR_OUT_02, dbg.MODE_DBG2);
    /*p08.BOLA*/ next.A03_D = nor (ctl.ADDR_OUT_03, dbg.MODE_DBG2);
    /*p08.BEVO*/ next.A04_D = nor (ctl.ADDR_OUT_04, dbg.MODE_DBG2);
    /*p08.AJAV*/ next.A05_D = nor (ctl.ADDR_OUT_05, dbg.MODE_DBG2);
    /*p08.CYKA*/ next.A06_D = nor (ctl.ADDR_OUT_06, dbg.MODE_DBG2);
    /*p08.COLO*/ next.A07_D = nor (ctl.ADDR_OUT_07, dbg.MODE_DBG2);
    /*p08.MEGO*/ next.A08_D = nor (ctl.ADDR_OUT_08, dbg.MODE_DBG2);
    /*p08.MENY*/ next.A09_D = nor (ctl.ADDR_OUT_09, dbg.MODE_DBG2);
    /*p08.RORE*/ next.A10_D = nor (ctl.ADDR_OUT_10, dbg.MODE_DBG2);
    /*p08.LYNY*/ next.A11_D = nor (ctl.ADDR_OUT_11, dbg.MODE_DBG2);
    /*p08.LOSO*/ next.A12_D = nor (ctl.ADDR_OUT_12, dbg.MODE_DBG2);
    /*p08.LEVA*/ next.A13_D = nor (ctl.ADDR_OUT_13, dbg.MODE_DBG2);
    /*p08.PAHY*/ next.A14_D = nor (ctl.ADDR_OUT_14, dbg.MODE_DBG2);
    /*p08.RULO*/ next.A15_D = nor (ctl.ADDR_OUT_15, dbg.MODE_DBG2);
  }

  //----------
  // Data pin driver.

  {
    /*p08.MOCA*/ wire DBG_EXT_RDn = nor(dec.ADDR_VALID_AND_NOT_VRAM, dbg.MODE_DBG1);
    /*p08.LAGU*/ wire LAGU = or(and(cpu.CPU_RAW_RD, dec.ADDR_VALID_AND_NOT_VRAMn), cpu.CPU_RAW_WR);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
    /*p08.REDU*/ wire CPU_RDo = not(ctl.CPU_RD);

    /*p08.LULA*/ next.D0_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D1_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D2_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D3_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D4_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D5_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D6_B = ctl.CBUS_TO_CEXT;
    /*p08.LULA*/ next.D7_B = ctl.CBUS_TO_CEXT;

    /*p25.RUXA*/ next.D0_A = nand(bus.D0, ctl.CBUS_TO_CEXT);
    /*p25.RUJA*/ next.D1_A = nand(bus.D1, ctl.CBUS_TO_CEXT);
    /*p25.RABY*/ next.D2_A = nand(bus.D2, ctl.CBUS_TO_CEXT);
    /*p25.RERA*/ next.D3_A = nand(bus.D3, ctl.CBUS_TO_CEXT);
    /*p25.RORY*/ next.D4_A = nand(bus.D4, ctl.CBUS_TO_CEXT);
    /*p25.RYVO*/ next.D5_A = nand(bus.D5, ctl.CBUS_TO_CEXT);
    /*p25.RAFY*/ next.D7_A = nand(bus.D6, ctl.CBUS_TO_CEXT);
    /*p25.RAVU*/ next.D6_A = nand(bus.D7, ctl.CBUS_TO_CEXT);

    /*p08.RUNE*/ next.D0_D = nor (bus.D0, ctl.CBUS_TO_CEXTn);
    /*p08.RYPU*/ next.D1_D = nor (bus.D1, ctl.CBUS_TO_CEXTn);
    /*p08.SULY*/ next.D2_D = nor (bus.D2, ctl.CBUS_TO_CEXTn);
    /*p08.SEZE*/ next.D3_D = nor (bus.D3, ctl.CBUS_TO_CEXTn);
    /*p08.RESY*/ next.D4_D = nor (bus.D4, ctl.CBUS_TO_CEXTn);
    /*p08.TAMU*/ next.D5_D = nor (bus.D5, ctl.CBUS_TO_CEXTn);
    /*p08.ROGY*/ next.D6_D = nor (bus.D6, ctl.CBUS_TO_CEXTn);
    /*p08.RYDA*/ next.D7_D = nor (bus.D7, ctl.CBUS_TO_CEXTn);
  }
}

//-----------------------------------------------------------------------------

};