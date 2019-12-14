#include "Sch_System.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void DMA_tick(const Gameboy& a, const Gameboy& b, Gameboy& next) {

  //----------
  // FF46 DMA

  {
    /*p22.WATE*/ wire FF46n = nand(b.vid.FF4X, b.vid.A00n, b.vid.WESA_A01,  b.vid.WALO_A02,  b.vid.A03n);
    /*p22.XEDA*/ wire FF46  = not(FF46n);

    /*p04.LAVY*/ wire FF46_WRn  = nand(FF46, b.ctl.CPU_WR2);

    /*p04.LORU*/ next.dma.CLK_DMA_HI = not(FF46_WRn);
    /*p04.NAFA*/ next.dma.DMA_A08 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A08, b.bus.D0);
    /*p04.PYNE*/ next.dma.DMA_A09 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A09, b.bus.D1);
    /*p04.PARA*/ next.dma.DMA_A10 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A10, b.bus.D2);
    /*p04.NYDO*/ next.dma.DMA_A11 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A11, b.bus.D3);
    /*p04.NYGY*/ next.dma.DMA_A12 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A12, b.bus.D4);
    /*p04.PULA*/ next.dma.DMA_A13 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A13, b.bus.D5);
    /*p04.POKU*/ next.dma.DMA_A14 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A14, b.bus.D6);
    /*p04.MARU*/ next.dma.DMA_A15 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A15, b.bus.D7);

    /*p04.MOLU*/ wire FF46_RDn1 = nand(FF46, b.ctl.CPU_RD2);
    /*p04.NYGO*/ wire FF46_RD   = not(FF46_RDn1);
    /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);
    /*p04.POLY*/ if (FF46_RDn2) next.bus.D0 = b.dma.DMA_A08;
    /*p04.ROFO*/ if (FF46_RDn2) next.bus.D1 = b.dma.DMA_A09;
    /*p04.REMA*/ if (FF46_RDn2) next.bus.D2 = b.dma.DMA_A10;
    /*p04.PANE*/ if (FF46_RDn2) next.bus.D3 = b.dma.DMA_A11;
    /*p04.PARE*/ if (FF46_RDn2) next.bus.D4 = b.dma.DMA_A12;
    /*p04.RALY*/ if (FF46_RDn2) next.bus.D5 = b.dma.DMA_A13;
    /*p04.RESU*/ if (FF46_RDn2) next.bus.D6 = b.dma.DMA_A14;
    /*p04.NUVY*/ if (FF46_RDn2) next.bus.D7 = b.dma.DMA_A15;
  }

  // schematic incorrect.
  // lyxe - weird gate - lavy, loko
  // lupa - nor - lavy, lyxe

  /*p22.WATE*/ wire FF46n = nand(b.vid.FF4X, b.vid.A00n, b.vid.WESA_A01,  b.vid.WALO_A02,  b.vid.A03n);
  /*p22.XEDA*/ wire FF46  = not(FF46n);
  /*p04.LAVY*/ wire FF46_WRn  = nand(FF46, b.ctl.CPU_WR2);

  /*p01.DULA*/ wire DULA_RESET = not(b.rst.SYS_RESETn);
  /*p01.CUNU*/ wire SYS_RESETn = not(DULA_RESET);
  /*p04.LOKO*/ wire DMA_RST = nand(SYS_RESETn, !b.dma.DMA_STARTb);
  /*p04.LAPA*/ wire DMA_RSTn = not(DMA_RST);

  /*p04.LYXE*/ wire LYXE = or(FF46_WRn, DMA_RST);
  /*p04.LUPA*/ wire DMA_START = nor(FF46_WRn, LYXE);
  /*p04.LUVY*/ next.dma.DMA_STARTa = tock_pos(a.clk.UVYT_ABCDxxxx, b.clk.UVYT_ABCDxxxx, SYS_RESETn, b.dma.DMA_STARTa, DMA_START);
  /*p04.LENE*/ next.dma.DMA_STARTb = tock_pos(a.clk.MOPA_xxxxEFGH, b.clk.MOPA_xxxxEFGH, SYS_RESETn, b.dma.DMA_STARTb, b.dma.DMA_STARTa);


  /*p04.NAVO*/ wire DMA_DONEn = nand(b.dma.DMA_A00, b.dma.DMA_A01, b.dma.DMA_A02, b.dma.DMA_A03, b.dma.DMA_A04, b.dma.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
  /*p04.NOLO*/ wire DMA_DONE = not(DMA_DONEn);
  /*p04.MYTE*/ next.dma.DMA_DONE_SYNC = tock_pos(a.clk.MOPA_xxxxEFGH, b.clk.MOPA_xxxxEFGH, DMA_RSTn, b.dma.DMA_DONE_SYNC, DMA_DONE);

  // glitch filter here, or it could be some sort of latch or flipflop...
  /*p04.LARA*/ wire LARA = nand(b.dma.DMA_RUNNING, !b.dma.DMA_DONE_SYNC, SYS_RESETn);
  /*p04.LOKY*/ next.dma.DMA_RUNNING = nand(LARA, !b.dma.DMA_STARTb);
  /*p04.MATU*/ next.dma.DMA_RUNNING_SYNC = tock_pos(a.clk.UVYT_ABCDxxxx, b.clk.UVYT_ABCDxxxx, SYS_RESETn, b.dma.DMA_RUNNING_SYNC, b.dma.DMA_RUNNING);

  /*p04.META*/ next.dma.CLK_DMA_LO = and(b.clk.UVYT_ABCDxxxx, b.dma.DMA_RUNNING);
  /*p04.NAKY*/ next.dma.DMA_A00 = tock_pos( a.dma.CLK_DMA_LO,  b.dma.CLK_DMA_LO, DMA_RSTn, b.dma.DMA_A00, !b.dma.DMA_A00);
  /*p04.PYRO*/ next.dma.DMA_A01 = tock_pos(!a.dma.DMA_A00,    !b.dma.DMA_A00,    DMA_RSTn, b.dma.DMA_A01, !b.dma.DMA_A01);
  /*p04.NEFY*/ next.dma.DMA_A02 = tock_pos(!a.dma.DMA_A01,    !b.dma.DMA_A01,    DMA_RSTn, b.dma.DMA_A02, !b.dma.DMA_A02);
  /*p04.MUTY*/ next.dma.DMA_A03 = tock_pos(!a.dma.DMA_A02,    !b.dma.DMA_A02,    DMA_RSTn, b.dma.DMA_A03, !b.dma.DMA_A03);
  /*p04.NYKO*/ next.dma.DMA_A04 = tock_pos(!a.dma.DMA_A03,    !b.dma.DMA_A03,    DMA_RSTn, b.dma.DMA_A04, !b.dma.DMA_A04);
  /*p04.PYLO*/ next.dma.DMA_A05 = tock_pos(!a.dma.DMA_A04,    !b.dma.DMA_A04,    DMA_RSTn, b.dma.DMA_A05, !b.dma.DMA_A05);
  /*p04.NUTO*/ next.dma.DMA_A06 = tock_pos(!a.dma.DMA_A05,    !b.dma.DMA_A05,    DMA_RSTn, b.dma.DMA_A06, !b.dma.DMA_A06);
  /*p04.MUGU*/ next.dma.DMA_A07 = tock_pos(!a.dma.DMA_A06,    !b.dma.DMA_A06,    DMA_RSTn, b.dma.DMA_A07, !b.dma.DMA_A07);

  /*p04.LEBU*/ wire DMA_A15n  = not(b.dma.DMA_A15);
  /*p04.MUDA*/ wire DMA_VRAM  = nor(b.dma.DMA_A13, b.dma.DMA_A14, DMA_A15n);
  /*p04.LOGO*/ wire DMA_VRAMn = not(DMA_VRAM);

  /*p04.MORY*/ wire DMA_READ_CARTn = nand(b.dma.DMA_RUNNING_SYNC, DMA_VRAMn);
  /*p04.MUHO*/ wire DMA_READ_VRAMn = nand(b.dma.DMA_RUNNING_SYNC, DMA_VRAM);

  /*p04.LUMA*/ next.dma.DMA_READ_CART = not(DMA_READ_CARTn);
  /*p04.LUFA*/ next.dma.DMA_READ_VRAM = not(DMA_READ_VRAMn);

  /*p04.AHOC*/ wire DMA_READ_VRAMo = not(next.dma.DMA_READ_VRAM);
  /*p04.ECAL*/ if (!DMA_READ_VRAMo) next.vrm.MA00 = b.dma.DMA_A00;
  /*p04.EGEZ*/ if (!DMA_READ_VRAMo) next.vrm.MA01 = b.dma.DMA_A01;
  /*p04.FUHE*/ if (!DMA_READ_VRAMo) next.vrm.MA02 = b.dma.DMA_A02;
  /*p04.FYZY*/ if (!DMA_READ_VRAMo) next.vrm.MA03 = b.dma.DMA_A03;
  /*p04.DAMU*/ if (!DMA_READ_VRAMo) next.vrm.MA04 = b.dma.DMA_A04;
  /*p04.DAVA*/ if (!DMA_READ_VRAMo) next.vrm.MA05 = b.dma.DMA_A05;
  /*p04.ETEG*/ if (!DMA_READ_VRAMo) next.vrm.MA06 = b.dma.DMA_A06;
  /*p04.EREW*/ if (!DMA_READ_VRAMo) next.vrm.MA07 = b.dma.DMA_A07;
  /*p04.EVAX*/ if (!DMA_READ_VRAMo) next.vrm.MA08 = b.dma.DMA_A08;
  /*p04.DUVE*/ if (!DMA_READ_VRAMo) next.vrm.MA09 = b.dma.DMA_A09;
  /*p04.ERAF*/ if (!DMA_READ_VRAMo) next.vrm.MA10 = b.dma.DMA_A10;
  /*p04.FUSY*/ if (!DMA_READ_VRAMo) next.vrm.MA11 = b.dma.DMA_A11;
  /*p04.EXYF*/ if (!DMA_READ_VRAMo) next.vrm.MA12 = b.dma.DMA_A12;
}

//-----------------------------------------------------------------------------

};