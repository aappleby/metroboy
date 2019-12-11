#include "Sch_System.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void DMA_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // FF46 DMA

  {
    /*p22.WATE*/ wire FF46n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01,  b.vid.A02,  b.vid.A03n);
    /*p22.XEDA*/ wire FF46  = not(FF46n);

    /*p04.LAVY*/ wire FF46_WRn  = nand(FF46, b.sys.CPU_WR2);

    /*p04.LORU*/ c.dma.CLK_DMA_HI = not(FF46_WRn);
    /*p04.NAFA*/ c.dma.DMA_A08 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A08, b.D0);
    /*p04.PYNE*/ c.dma.DMA_A09 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A09, b.D1);
    /*p04.PARA*/ c.dma.DMA_A10 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A10, b.D2);
    /*p04.NYDO*/ c.dma.DMA_A11 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A11, b.D3);
    /*p04.NYGY*/ c.dma.DMA_A12 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A12, b.D4);
    /*p04.PULA*/ c.dma.DMA_A13 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A13, b.D5);
    /*p04.POKU*/ c.dma.DMA_A14 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A14, b.D6);
    /*p04.MARU*/ c.dma.DMA_A15 = tock_pos(a.dma.CLK_DMA_HI, b.dma.CLK_DMA_HI, 0, b.dma.DMA_A15, b.D7);

    /*p04.MOLU*/ wire FF46_RDn1 = nand(FF46, b.sys.CPU_RD2);
    /*p04.NYGO*/ wire FF46_RD   = not(FF46_RDn1);
    /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);
    /*p04.POLY*/ if (FF46_RDn2) c.D0 = b.dma.DMA_A08;
    /*p04.ROFO*/ if (FF46_RDn2) c.D1 = b.dma.DMA_A09;
    /*p04.REMA*/ if (FF46_RDn2) c.D2 = b.dma.DMA_A10;
    /*p04.PANE*/ if (FF46_RDn2) c.D3 = b.dma.DMA_A11;
    /*p04.PARE*/ if (FF46_RDn2) c.D4 = b.dma.DMA_A12;
    /*p04.RALY*/ if (FF46_RDn2) c.D5 = b.dma.DMA_A13;
    /*p04.RESU*/ if (FF46_RDn2) c.D6 = b.dma.DMA_A14;
    /*p04.NUVY*/ if (FF46_RDn2) c.D7 = b.dma.DMA_A15;
  }

  // schematic incorrect.
  // lyxe - weird gate - lavy, loko
  // lupa - nor - lavy, lyxe

  /*p22.WATE*/ wire FF46n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01,  b.vid.A02,  b.vid.A03n);
  /*p22.XEDA*/ wire FF46  = not(FF46n);
  /*p04.LAVY*/ wire FF46_WRn  = nand(FF46, b.sys.CPU_WR2);

  /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
  /*p01.CUNU*/ wire SYS_RESETn = not(DULA_RESET);
  /*p04.LOKO*/ wire DMA_RST = nand(SYS_RESETn, !b.dma.DMA_STARTb);
  /*p04.LAPA*/ wire DMA_RSTn = not(DMA_RST);

  /*p04.LYXE*/ wire LYXE = or(FF46_WRn, DMA_RST);
  /*p04.LUPA*/ wire DMA_START = nor(FF46_WRn, LYXE);
  /*p04.LUVY*/ c.dma.DMA_STARTa = tock_pos(a.sys.PHASE_ABCDxxxx7b, b.sys.PHASE_ABCDxxxx7b, SYS_RESETn, b.dma.DMA_STARTa, DMA_START);
  /*p04.LENE*/ c.dma.DMA_STARTb = tock_pos(a.sys.PHASE_xxxxEFGH8b, b.sys.PHASE_xxxxEFGH8b, SYS_RESETn, b.dma.DMA_STARTb, b.dma.DMA_STARTa);


  /*p04.NAVO*/ wire DMA_DONEn = nand(b.dma.DMA_A00, b.dma.DMA_A01, b.dma.DMA_A02, b.dma.DMA_A03, b.dma.DMA_A04, b.dma.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
  /*p04.NOLO*/ wire DMA_DONE = not(DMA_DONEn);
  /*p04.MYTE*/ c.dma.DMA_DONE_SYNC = tock_pos(a.sys.PHASE_xxxxEFGH8b, b.sys.PHASE_xxxxEFGH8b, DMA_RSTn, b.dma.DMA_DONE_SYNC, DMA_DONE);

  // glitch filter here, or it could be some sort of latch or flipflop...
  /*p04.LARA*/ wire LARA = nand(b.dma.DMA_RUNNING, !b.dma.DMA_DONE_SYNC, SYS_RESETn);
  /*p04.LOKY*/ c.dma.DMA_RUNNING = nand(LARA, !b.dma.DMA_STARTb);
  /*p04.MATU*/ c.dma.DMA_RUNNING_SYNC = tock_pos(a.sys.PHASE_ABCDxxxx7b, b.sys.PHASE_ABCDxxxx7b, SYS_RESETn, b.dma.DMA_RUNNING_SYNC, b.dma.DMA_RUNNING);

  /*p04.META*/ c.dma.CLK_DMA_LO = and(b.sys.PHASE_ABCDxxxx7b, b.dma.DMA_RUNNING);
  /*p04.NAKY*/ c.dma.DMA_A00 = tock_pos( a.dma.CLK_DMA_LO,  b.dma.CLK_DMA_LO, DMA_RSTn, b.dma.DMA_A00, !b.dma.DMA_A00);
  /*p04.PYRO*/ c.dma.DMA_A01 = tock_pos(!a.dma.DMA_A00,    !b.dma.DMA_A00,    DMA_RSTn, b.dma.DMA_A01, !b.dma.DMA_A01);
  /*p04.NEFY*/ c.dma.DMA_A02 = tock_pos(!a.dma.DMA_A01,    !b.dma.DMA_A01,    DMA_RSTn, b.dma.DMA_A02, !b.dma.DMA_A02);
  /*p04.MUTY*/ c.dma.DMA_A03 = tock_pos(!a.dma.DMA_A02,    !b.dma.DMA_A02,    DMA_RSTn, b.dma.DMA_A03, !b.dma.DMA_A03);
  /*p04.NYKO*/ c.dma.DMA_A04 = tock_pos(!a.dma.DMA_A03,    !b.dma.DMA_A03,    DMA_RSTn, b.dma.DMA_A04, !b.dma.DMA_A04);
  /*p04.PYLO*/ c.dma.DMA_A05 = tock_pos(!a.dma.DMA_A04,    !b.dma.DMA_A04,    DMA_RSTn, b.dma.DMA_A05, !b.dma.DMA_A05);
  /*p04.NUTO*/ c.dma.DMA_A06 = tock_pos(!a.dma.DMA_A05,    !b.dma.DMA_A05,    DMA_RSTn, b.dma.DMA_A06, !b.dma.DMA_A06);
  /*p04.MUGU*/ c.dma.DMA_A07 = tock_pos(!a.dma.DMA_A06,    !b.dma.DMA_A06,    DMA_RSTn, b.dma.DMA_A07, !b.dma.DMA_A07);

  /*p04.LEBU*/ wire DMA_A15n  = not(b.dma.DMA_A15);
  /*p04.MUDA*/ wire DMA_VRAM  = nor(b.dma.DMA_A13, b.dma.DMA_A14, DMA_A15n);
  /*p04.LOGO*/ wire DMA_VRAMn = not(DMA_VRAM);

  /*p04.MORY*/ wire DMA_READ_CARTn = nand(b.dma.DMA_RUNNING_SYNC, DMA_VRAMn);
  /*p04.MUHO*/ wire DMA_READ_VRAMn = nand(b.dma.DMA_RUNNING_SYNC, DMA_VRAM);

  /*p04.LUMA*/ c.dma.DMA_READ_CART = not(DMA_READ_CARTn);
  /*p04.LUFA*/ c.dma.DMA_READ_VRAM = not(DMA_READ_VRAMn);

  /*p04.AHOC*/ wire DMA_READ_VRAMo = not(c.dma.DMA_READ_VRAM);
  /*p04.ECAL*/ if (!DMA_READ_VRAMo) c.pin.MA00 = b.dma.DMA_A00;
  /*p04.EGEZ*/ if (!DMA_READ_VRAMo) c.pin.MA01 = b.dma.DMA_A01;
  /*p04.FUHE*/ if (!DMA_READ_VRAMo) c.pin.MA02 = b.dma.DMA_A02;
  /*p04.FYZY*/ if (!DMA_READ_VRAMo) c.pin.MA03 = b.dma.DMA_A03;
  /*p04.DAMU*/ if (!DMA_READ_VRAMo) c.pin.MA04 = b.dma.DMA_A04;
  /*p04.DAVA*/ if (!DMA_READ_VRAMo) c.pin.MA05 = b.dma.DMA_A05;
  /*p04.ETEG*/ if (!DMA_READ_VRAMo) c.pin.MA06 = b.dma.DMA_A06;
  /*p04.EREW*/ if (!DMA_READ_VRAMo) c.pin.MA07 = b.dma.DMA_A07;
  /*p04.EVAX*/ if (!DMA_READ_VRAMo) c.pin.MA08 = b.dma.DMA_A08;
  /*p04.DUVE*/ if (!DMA_READ_VRAMo) c.pin.MA09 = b.dma.DMA_A09;
  /*p04.ERAF*/ if (!DMA_READ_VRAMo) c.pin.MA10 = b.dma.DMA_A10;
  /*p04.FUSY*/ if (!DMA_READ_VRAMo) c.pin.MA11 = b.dma.DMA_A11;
  /*p04.EXYF*/ if (!DMA_READ_VRAMo) c.pin.MA12 = b.dma.DMA_A12;

  /*p28.AZAR*/ wire DMA_READ_VRAMp = not(c.dma.DMA_READ_VRAM);
  /*p28.WUZU*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_A_D0 = b.MD0;
  /*p28.AXER*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_A_D1 = b.MD1;
  /*p28.ASOX*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_A_D2 = b.MD2;
  /*p28.CETU*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_A_D3 = b.MD3;
  /*p28.ARYN*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_A_D4 = b.MD4;
  /*p28.ACOT*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_A_D5 = b.MD5;
  /*p28.CUJE*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_A_D6 = b.MD6;
  /*p28.ATER*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_A_D7 = b.MD7;
  /*p28.WOWA*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_B_D0 = b.MD0;
  /*p28.AVEB*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_B_D1 = b.MD1;
  /*p28.AMUH*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_B_D2 = b.MD2;
  /*p28.COFO*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_B_D3 = b.MD3;
  /*p28.AZOZ*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_B_D4 = b.MD4;
  /*p28.AGYK*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_B_D5 = b.MD5;
  /*p28.BUSE*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_B_D6 = b.MD6;
  /*p28.ANUM*/ if (!DMA_READ_VRAMp) c.oam.TS_OAM_B_D7 = b.MD7;

  /*p25.WEJO*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_A_D0 = not(/*p25.RALO*/ not(b.pin.D0_C));
  /*p25.BUBO*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_A_D1 = not(/*p25.TUNE*/ not(b.pin.D1_C));
  /*p25.BETU*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_A_D2 = not(/*p25.SERA*/ not(b.pin.D2_C));
  /*p25.CYME*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_A_D3 = not(/*p25.TENU*/ not(b.pin.D3_C));
  /*p25.BAXU*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_A_D4 = not(/*p25.SYSA*/ not(b.pin.D4_C));
  /*p25.BUHU*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_A_D5 = not(/*p25.SUGY*/ not(b.pin.D5_C));
  /*p25.BYNY*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_A_D6 = not(/*p25.TUBE*/ not(b.pin.D6_C));
  /*p25.BYPY*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_A_D7 = not(/*p25.SYZO*/ not(b.pin.D7_C));
  /*p25.WASA*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_B_D0 = not(/*p25.RALO*/ not(b.pin.D0_C));
  /*p25.BOMO*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_B_D1 = not(/*p25.TUNE*/ not(b.pin.D1_C));
  /*p25.BASA*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_B_D2 = not(/*p25.SERA*/ not(b.pin.D2_C));
  /*p25.CAKO*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_B_D3 = not(/*p25.TENU*/ not(b.pin.D3_C));
  /*p25.BUMA*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_B_D4 = not(/*p25.SYSA*/ not(b.pin.D4_C));
  /*p25.BUPY*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_B_D5 = not(/*p25.SUGY*/ not(b.pin.D5_C));
  /*p25.BASY*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_B_D6 = not(/*p25.TUBE*/ not(b.pin.D6_C));
  /*p25.BAPE*/ if (!DMA_READ_CARTn) c.oam.TS_OAM_B_D7 = not(/*p25.SYZO*/ not(b.pin.D7_C));
}

//-----------------------------------------------------------------------------

};