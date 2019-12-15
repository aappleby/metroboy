#include "Sch_DMA.h"

#include "Sch_BusControl.h"
#include "Sch_Clocks.h"
#include "Sch_Decoder.h"
#include "Sch_Resets.h"
#include "Sch_CpuSignals.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void DMA_tick(const BusControl& bus,
              const Cpu& cpu,
              const Decoder& dec,
              const Clocks& clk,
              const Resets& rst,
              const DMA& b,
              DMA& next,
              BusControl& bus_out) {

  //----------
  // FF46 DMA

  {
    /*p04.LAVY*/ wire FF46_WRn  = nand(dec.FF46, bus.CPU_WR2);
    /*p04.MOLU*/ wire FF46_RDn1 = nand(dec.FF46, bus.CPU_RD2);
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

    /*p04.POLY*/ if (FF46_RDn2) bus_out.D0 = b.DMA_A08;
    /*p04.ROFO*/ if (FF46_RDn2) bus_out.D1 = b.DMA_A09;
    /*p04.REMA*/ if (FF46_RDn2) bus_out.D2 = b.DMA_A10;
    /*p04.PANE*/ if (FF46_RDn2) bus_out.D3 = b.DMA_A11;
    /*p04.PARE*/ if (FF46_RDn2) bus_out.D4 = b.DMA_A12;
    /*p04.RALY*/ if (FF46_RDn2) bus_out.D5 = b.DMA_A13;
    /*p04.RESU*/ if (FF46_RDn2) bus_out.D6 = b.DMA_A14;
    /*p04.NUVY*/ if (FF46_RDn2) bus_out.D7 = b.DMA_A15;
  }

  // schematic incorrect.
  // lyxe - weird gate - lavy, loko
  // lupa - nor - lavy, lyxe

  /*p04.LAVY*/ wire FF46_WRn  = nand(dec.FF46, bus.CPU_WR2);

  /*p04.LOKO*/ wire DMA_RST = nand(rst.CUNU_RESETn, !b.DMA_STARTb);
  /*p04.LAPA*/ wire DMA_RSTn = not(DMA_RST);

  /*p04.LYXE*/ wire LYXE = or(FF46_WRn, DMA_RST);
  /*p04.LUPA*/ wire DMA_START = nor(FF46_WRn, LYXE);
  /*p04.LUVY*/ next.DMA_STARTa.tock(clk.UVYT_ABCDxxxx, rst.CUNU_RESETn, DMA_START);
  /*p04.LENE*/ next.DMA_STARTb.tock(clk.MOPA_xxxxEFGH, rst.CUNU_RESETn, b.DMA_STARTa);


  /*p04.NAVO*/ wire DMA_DONEn = nand(b.DMA_A00, b.DMA_A01, b.DMA_A02, b.DMA_A03, b.DMA_A04, b.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
  /*p04.NOLO*/ wire DMA_DONE = not(DMA_DONEn);
  /*p04.MYTE*/ next.DMA_DONE_SYNC.tock(clk.MOPA_xxxxEFGH, DMA_RSTn, DMA_DONE);

  // glitch filter here, or it could be some sort of latch or flipflop...
  /*p04.LARA*/ wire LARA        = nand(b.DMA_RUNNING, !b.DMA_DONE_SYNC, rst.CUNU_RESETn);
  /*p04.LOKY*/ next.DMA_RUNNING = nand(LARA, !b.DMA_STARTb);

  /*p04.MATU*/ next.DMA_RUNNING_SYNC.tock(clk.UVYT_ABCDxxxx, rst.CUNU_RESETn, b.DMA_RUNNING);

  /*p04.META*/ wire CLK_DMA_LO = and(clk.UVYT_ABCDxxxx, b.DMA_RUNNING);
  /*p04.NAKY*/ next.DMA_A00.tock(CLK_DMA_LO, DMA_RSTn, !b.DMA_A00);
  /*p04.PYRO*/ next.DMA_A01.tock(!b.DMA_A00, DMA_RSTn, !b.DMA_A01);
  /*p04.NEFY*/ next.DMA_A02.tock(!b.DMA_A01, DMA_RSTn, !b.DMA_A02);
  /*p04.MUTY*/ next.DMA_A03.tock(!b.DMA_A02, DMA_RSTn, !b.DMA_A03);
  /*p04.NYKO*/ next.DMA_A04.tock(!b.DMA_A03, DMA_RSTn, !b.DMA_A04);
  /*p04.PYLO*/ next.DMA_A05.tock(!b.DMA_A04, DMA_RSTn, !b.DMA_A05);
  /*p04.NUTO*/ next.DMA_A06.tock(!b.DMA_A05, DMA_RSTn, !b.DMA_A06);
  /*p04.MUGU*/ next.DMA_A07.tock(!b.DMA_A06, DMA_RSTn, !b.DMA_A07);

  /*p04.LEBU*/ wire DMA_A15n  = not(b.DMA_A15);
  /*p04.MUDA*/ wire DMA_VRAM  = nor(b.DMA_A13, b.DMA_A14, DMA_A15n);
  /*p04.LOGO*/ wire DMA_VRAMn = not(DMA_VRAM);

  /*p04.MORY*/ next.DMA_READ_CARTn = nand(b.DMA_RUNNING_SYNC, DMA_VRAMn);
  /*p04.MUHO*/ next.DMA_READ_VRAMn = nand(b.DMA_RUNNING_SYNC, DMA_VRAM);

  /*p04.LUMA*/ next.DMA_READ_CART = not(b.DMA_READ_CARTn);
  /*p04.LUFA*/ next.DMA_READ_VRAM = not(b.DMA_READ_VRAMn);

  /*p04.MAKA*/ next.FROM_CPU5_SYNC.tock(clk.ZEME_xBxDxFxH, rst.CUNU_RESETn, cpu.FROM_CPU5);
  /*p04.NAXY*/ wire NAXY = nor(b.FROM_CPU5_SYNC, b.DMA_STARTa);
  /*p04.POWU*/ next.DMA_WRITE_OAM = and(b.DMA_RUNNING_SYNC, NAXY);
}

//-----------------------------------------------------------------------------

};