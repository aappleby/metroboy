#include "Sch_DMA.h"

#include "Sch_BusControl.h"
#include "Sch_Clocks.h"
#include "Sch_Decoder.h"
#include "Sch_Resets.h"
#include "Sch_CpuSignals.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void DMA_tick(const Bus& bus,
              const BusControl& ctl,
              const Cpu& cpu,
              const Decoder& dec,
              const Clocks& clk,
              const Resets& rst,
              const DMA& dma,

              DMA& next,
              Bus& bus_out) {

  //----------
  // FF46 DMA

  {
    /*p04.LAVY*/ wire FF46_WRn  = nand(dec.FF46, ctl.CPU_WR2);
    /*p04.MOLU*/ wire FF46_RDn1 = nand(dec.FF46, ctl.CPU_RD2);
    /*p04.NYGO*/ wire FF46_RD   = not(FF46_RDn1);
    /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);

    /*p04.LORU*/ wire CLK_DMA_HI = not(FF46_WRn);
    /*p04.NAFA*/ next.DMA_A08.tock(CLK_DMA_HI, 1, bus.D0);
    /*p04.PYNE*/ next.DMA_A09.tock(CLK_DMA_HI, 1, bus.D1);
    /*p04.PARA*/ next.DMA_A10.tock(CLK_DMA_HI, 1, bus.D2);
    /*p04.NYDO*/ next.DMA_A11.tock(CLK_DMA_HI, 1, bus.D3);
    /*p04.NYGY*/ next.DMA_A12.tock(CLK_DMA_HI, 1, bus.D4);
    /*p04.PULA*/ next.DMA_A13.tock(CLK_DMA_HI, 1, bus.D5);
    /*p04.POKU*/ next.DMA_A14.tock(CLK_DMA_HI, 1, bus.D6);
    /*p04.MARU*/ next.DMA_A15.tock(CLK_DMA_HI, 1, bus.D7);

    /*p04.POLY*/ if (FF46_RDn2) bus_out.D0 = dma.DMA_A08;
    /*p04.ROFO*/ if (FF46_RDn2) bus_out.D1 = dma.DMA_A09;
    /*p04.REMA*/ if (FF46_RDn2) bus_out.D2 = dma.DMA_A10;
    /*p04.PANE*/ if (FF46_RDn2) bus_out.D3 = dma.DMA_A11;
    /*p04.PARE*/ if (FF46_RDn2) bus_out.D4 = dma.DMA_A12;
    /*p04.RALY*/ if (FF46_RDn2) bus_out.D5 = dma.DMA_A13;
    /*p04.RESU*/ if (FF46_RDn2) bus_out.D6 = dma.DMA_A14;
    /*p04.NUVY*/ if (FF46_RDn2) bus_out.D7 = dma.DMA_A15;
  }

  // schematic incorrect.
  // lyxe - weird gate - lavy, loko
  // lupa - nor - lavy, lyxe

  {
    /*p04.LAVY*/ wire FF46_WRn = nand(dec.FF46, ctl.CPU_WR2);
    /*p04.LOKO*/ wire DMA_RST  = nand(rst.CUNU_RESETn, !dma.DMA_ENb);

    // Latch
    /*p04.LYXE*/ if (FF46_WRn) next.DMA_EN_LATCHn = 0;
    /*p04.LYXE*/ if (DMA_RST)  next.DMA_EN_LATCHn = 1;

    /*p04.LUPA*/ wire DMA_EN = nor(FF46_WRn, dma.DMA_EN_LATCHn); // this seems redundant
    /*p04.LUVY*/ next.DMA_ENa.tock(clk.UVYT_xBCDExxx, rst.CUNU_RESETn, DMA_EN);
    /*p04.LENE*/ next.DMA_ENb.tock(clk.MOPA_AxxxxFGH, rst.CUNU_RESETn, dma.DMA_ENa);
  }

  {
    /*p04.LOKO*/ wire DMA_RST   = nand(rst.CUNU_RESETn, !dma.DMA_ENb);
    /*p04.LAPA*/ wire DMA_RSTn  = not(DMA_RST);

    /*p04.NAVO*/ wire DMA_DONEn = nand(dma.DMA_A00, dma.DMA_A01, dma.DMA_A02, dma.DMA_A03, dma.DMA_A04, dma.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*p04.NOLO*/ wire DMA_DONE  = not(DMA_DONEn);

    /*p04.MYTE*/ next.DMA_DONE_SYNC.tock(clk.MOPA_AxxxxFGH, DMA_RSTn, DMA_DONE);
  }

  {
    // NAND latch
    ///*p04.LARA*/ next.LARA = nand(dma.LOKY, !dma.DMA_DONE_SYNC, rst.CUNU_RESETn);
    ///*p04.LOKY*/ next.LOKY = nand(dma.LARA, !dma.DMA_ENb);

    if (dma.DMA_ENb) {
      next.DMA_CLKEN_LATCHn = 0;
      next.DMA_CLKEN_LATCH  = 1;
    }
    if (dma.DMA_DONE_SYNC || !rst.CUNU_RESETn) {
      next.DMA_CLKEN_LATCHn = 1;
      next.DMA_CLKEN_LATCH  = 0;
    }
  }


  /*p04.MATU*/ next.DMA_CLKEN.tock(clk.UVYT_xBCDExxx, rst.CUNU_RESETn, dma.DMA_CLKEN_LATCH);

  {
    /*p04.LEBU*/ wire DMA_A15n  = not(dma.DMA_A15);
    /*p04.MUDA*/ wire DMA_VRAM  = nor(dma.DMA_A13, dma.DMA_A14, DMA_A15n);
    /*p04.LOGO*/ wire DMA_VRAMn = not(DMA_VRAM);

    /*p04.MORY*/ next.DMA_READ_CARTn = nand(dma.DMA_CLKEN, DMA_VRAMn);
    /*p04.MUHO*/ next.DMA_READ_VRAMn = nand(dma.DMA_CLKEN, DMA_VRAM);

    /*p04.LUMA*/ next.DMA_READ_CART = not(dma.DMA_READ_CARTn);
    /*p04.LUFA*/ next.DMA_READ_VRAM = not(dma.DMA_READ_VRAMn);
  }

  {
    /*p04.MAKA*/ next.FROM_CPU5_SYNC.tock(clk.ZEME_AxCxExGx, rst.CUNU_RESETn, cpu.FROM_CPU5);
    /*p04.NAXY*/ wire NAXY = nor(dma.FROM_CPU5_SYNC, dma.DMA_ENa);
    /*p04.POWU*/ next.DMA_WRITE_OAM = and(dma.DMA_CLKEN, NAXY);
  }


  {
    /*p04.META*/ wire CLK_DMA_LO = and(clk.UVYT_xBCDExxx, dma.DMA_CLKEN_LATCH);
    /*p04.LOKO*/ wire DMA_RST    = nand(rst.CUNU_RESETn, !dma.DMA_ENb);
    /*p04.LAPA*/ wire DMA_RSTn   = not(DMA_RST);

    /*p04.NAKY*/ next.DMA_A00.tock(CLK_DMA_LO,   DMA_RSTn, !dma.DMA_A00);
    /*p04.PYRO*/ next.DMA_A01.tock(!dma.DMA_A00, DMA_RSTn, !dma.DMA_A01);
    /*p04.NEFY*/ next.DMA_A02.tock(!dma.DMA_A01, DMA_RSTn, !dma.DMA_A02);
    /*p04.MUTY*/ next.DMA_A03.tock(!dma.DMA_A02, DMA_RSTn, !dma.DMA_A03);
    /*p04.NYKO*/ next.DMA_A04.tock(!dma.DMA_A03, DMA_RSTn, !dma.DMA_A04);
    /*p04.PYLO*/ next.DMA_A05.tock(!dma.DMA_A04, DMA_RSTn, !dma.DMA_A05);
    /*p04.NUTO*/ next.DMA_A06.tock(!dma.DMA_A05, DMA_RSTn, !dma.DMA_A06);
    /*p04.MUGU*/ next.DMA_A07.tock(!dma.DMA_A06, DMA_RSTn, !dma.DMA_A07);
  }
}

//-----------------------------------------------------------------------------

};