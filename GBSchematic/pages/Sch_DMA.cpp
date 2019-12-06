#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void DMA_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  const System& pa = a.sys;
  const System& pb = b.sys;
  System& pc = c.sys;

  /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn1);
  /*p01.CUNU*/ c.sys.SYS_RESETn2 = not(DULA_RESET); // video, sprites, dma

  //----------
  // FF46 DMA

  /*p04.MOLU*/ wire FF46_RDn1 = nand(b.vid.FF46, pb.CPU_RD2);
  /*p04.NYGO*/ wire FF46_RD   = not(FF46_RDn1);
  /*p04.PUSY*/ wire FF46_RDn2 = not(FF46_RD);
  /*p04.LAVY*/ wire FF46_WRn  = nand(b.vid.FF46, pb.CPU_WR2);
  /*p04.LORU*/ pc.FF46_WR   = not(FF46_WRn);

  /*p04.NAFA*/ pc.DMA_A08 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A08, b.D0);
  /*p04.PYNE*/ pc.DMA_A09 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A09, b.D1);
  /*p04.PARA*/ pc.DMA_A10 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A10, b.D2);
  /*p04.NYDO*/ pc.DMA_A11 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A11, b.D3);
  /*p04.NYGY*/ pc.DMA_A12 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A12, b.D4);
  /*p04.PULA*/ pc.DMA_A13 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A13, b.D5);
  /*p04.POKU*/ pc.DMA_A14 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A14, b.D6);
  /*p04.MARU*/ pc.DMA_A15 = tock_pos(pa.FF46_WR, pb.FF46_WR, 0, pb.DMA_A15, b.D7);

  /*p04.POLY*/ if (FF46_RDn2) c.D0 = pb.DMA_A08; // polarity looks backwards
  /*p04.ROFO*/ if (FF46_RDn2) c.D1 = pb.DMA_A09;
  /*p04.REMA*/ if (FF46_RDn2) c.D2 = pb.DMA_A10;
  /*p04.PANE*/ if (FF46_RDn2) c.D3 = pb.DMA_A11;
  /*p04.PARE*/ if (FF46_RDn2) c.D4 = pb.DMA_A12;
  /*p04.RALY*/ if (FF46_RDn2) c.D5 = pb.DMA_A13;
  /*p04.RESU*/ if (FF46_RDn2) c.D6 = pb.DMA_A14;
  /*p04.NUVY*/ if (FF46_RDn2) c.D7 = pb.DMA_A15;

  // what's this about? seems wrong. schematic incorrect
  /*p04.LYXE*/ pc.LYXE = or(pb.FF46_WR, pb.DMA_RST);
  /*p04.LUPA*/ pc.LUPA = nor(pb.FF46_WR, pb.LYXE); // this is FF46_WRn on the schematic
  /*p04.LUVY*/ pc.LUVY = tock_pos(pa.PHASE_ABCDxxxx7b, pb.PHASE_ABCDxxxx7b, pb.SYS_RESETn2, pb.LUVY, pb.LUPA);
  /*p04.LENE*/ pc.LENE = tock_pos(pa.PHASE_xxxxEFGH8b, pb.PHASE_xxxxEFGH8b, pb.SYS_RESETn2, pb.LENE, pb.LUVY);

  /*p04.LOKO*/ pc.DMA_RST = nand(pb.SYS_RESETn2, !pb.LENE);
  /*p04.LAPA*/ pc.DMA_RSTn = not(pb.DMA_RST);

  // glitch filter here
  /*p04.NAVO*/ pc.DMA_DONEn = nand(pb.DMA_A00, pb.DMA_A01, pb.DMA_A02, pb.DMA_A03, pb.DMA_A04, pb.DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
  /*p04.NOLO*/ pc.DMA_DONE = not(pb.DMA_DONEn);
  /*p04.MYTE*/ pc.DMA_DONE_SYNC = tock_pos(pa.PHASE_xxxxEFGH8b, pb.PHASE_xxxxEFGH8b, pb.DMA_RSTn, pb.DMA_DONE_SYNC, pb.DMA_DONE);
  /*p04.LARA*/ pc.LARA = nand(pb.DMA_RUNNING, !pb.DMA_DONE_SYNC, pb.SYS_RESETn2);
  /*p04.LOKY*/ pc.DMA_RUNNING = nand(pb.LARA, !pb.LENE);
    
  /*p04.MATU*/ pc.DMA_RUNNING_SYNCn = tock_pos(pa.PHASE_ABCDxxxx7b, pb.PHASE_ABCDxxxx7b, pb.SYS_RESETn2, pb.DMA_RUNNING_SYNCn, pb.DMA_RUNNING);

  /*p04.META*/ pc.DMA_CLK = and(pb.PHASE_ABCDxxxx7b, pb.DMA_RUNNING);


  /*p04.NAKY*/ pc.DMA_A00 = tock_pos( pa.DMA_CLK,  pb.DMA_CLK, pb.DMA_RSTn, pb.DMA_A00, !pb.DMA_A00);
  /*p04.PYRO*/ pc.DMA_A01 = tock_pos(!pa.DMA_A00, !pb.DMA_A00, pb.DMA_RSTn, pb.DMA_A01, !pb.DMA_A01);
  /*p04.NEFY*/ pc.DMA_A02 = tock_pos(!pa.DMA_A01, !pb.DMA_A01, pb.DMA_RSTn, pb.DMA_A02, !pb.DMA_A02);
  /*p04.MUTY*/ pc.DMA_A03 = tock_pos(!pa.DMA_A02, !pb.DMA_A02, pb.DMA_RSTn, pb.DMA_A03, !pb.DMA_A03);
  /*p04.NYKO*/ pc.DMA_A04 = tock_pos(!pa.DMA_A03, !pb.DMA_A03, pb.DMA_RSTn, pb.DMA_A04, !pb.DMA_A04);
  /*p04.PYLO*/ pc.DMA_A05 = tock_pos(!pa.DMA_A04, !pb.DMA_A04, pb.DMA_RSTn, pb.DMA_A05, !pb.DMA_A05);
  /*p04.NUTO*/ pc.DMA_A06 = tock_pos(!pa.DMA_A05, !pb.DMA_A05, pb.DMA_RSTn, pb.DMA_A06, !pb.DMA_A06);
  /*p04.MUGU*/ pc.DMA_A07 = tock_pos(!pa.DMA_A06, !pb.DMA_A06, pb.DMA_RSTn, pb.DMA_A07, !pb.DMA_A07);


  /*p04.LEBU*/ pc.DMA_A15n  = not(pb.DMA_A15);
  /*p04.MUDA*/ pc.DMA_VRAM  = nor(pb.DMA_A13, pb.DMA_A14, pb.DMA_A15n);
  /*p04.LOGO*/ pc.DMA_VRAMn = not(pb.DMA_VRAM);

  /*p04.MORY*/ pc.DO_DMAn   = nand(pb.DMA_RUNNING_SYNCn, pb.DMA_VRAMn);
  /*p04.LUMA*/ pc.DO_DMA    = not(pb.DO_DMAn);

  // polarity?
  /*p04.MUHO*/     pc.VRAM_TO_OAMb = nand(pb.DMA_RUNNING_SYNCn, pb.DMA_VRAM);
  /*p04.LUFA*/   pc.VRAM_TO_OAMn = not(pb.VRAM_TO_OAMb);
  /*p04.AHOC*/ pc.VRAM_TO_OAMa = not(pb.VRAM_TO_OAMn);

  /*p04.ECAL*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA00 = pb.DMA_A00;
  /*p04.EGEZ*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA01 = pb.DMA_A01;
  /*p04.FUHE*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA02 = pb.DMA_A02;
  /*p04.FYZY*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA03 = pb.DMA_A03;
  /*p04.DAMU*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA04 = pb.DMA_A04;
  /*p04.DAVA*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA05 = pb.DMA_A05;
  /*p04.ETEG*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA06 = pb.DMA_A06;
  /*p04.EREW*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA07 = pb.DMA_A07;
  /*p04.EVAX*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA08 = pb.DMA_A08;
  /*p04.DUVE*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA09 = pb.DMA_A09;
  /*p04.ERAF*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA10 = pb.DMA_A10;
  /*p04.FUSY*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA11 = pb.DMA_A11;
  /*p04.EXYF*/ if (pb.VRAM_TO_OAMa) c.chip_out.MA12 = pb.DMA_A12;
}

//-----------------------------------------------------------------------------

};