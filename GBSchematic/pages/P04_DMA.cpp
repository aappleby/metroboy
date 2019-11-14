#include "P04_DMA.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P04_DMA::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // this logic loop is annoying.

  c.p04.FROM_CPU5n = not(b.cpu.FROM_CPU5);
  c.p04.FROM_CPU5  = not(b.p04.FROM_CPU5n);

  c.p04.FROM_CPU5_SYNC = tock_pos(a.p01.CLK_xBxDxFxH2,
                                  b.p01.CLK_xBxDxFxH2,
                                  b.p01.SYS_RESETn2,
                                  b.p04.FROM_CPU5_SYNC,
                                  b.p04.FROM_CPU5);

  c.p04.NAXY = nor(b.p04.FROM_CPU5_SYNC, b.p04.LUVY);
  c.p04.POWU = and(b.p04.MATU, b.p04.NAXY);
  c.p04.WYJA = unk3(b.p28.AMAB, b.p07.CPU_WR2, b.p04.POWU);

  c.p04.LUPA = nor(b.p04.FF46_WRn, b.p04.LYXE);
  c.p04.LUVY = tock_pos(a.p01.PHI_OUTn,
                        b.p01.PHI_OUTn,
                        b.p01.SYS_RESETn2,
                        b.p04.LUVY,
                        b.p04.LUPA);

  c.p04.PHI_OUT = not(b.p01.PHI_OUTn);
  
  // 128+16+8+4+2+1 = 159, this must be "dma done"
  c.p04.DMA_DONE = nand(b.p04.DMA_A00,
                        b.p04.DMA_A01,
                        b.p04.DMA_A02,
                        b.p04.DMA_A03,
                        b.p04.DMA_A04,
                        b.p04.DMA_A07);

  c.p04.MUDA = nor(b.p04.DMA_A13, b.p04.DMA_A14, b.p04.DMA_A15n);
  c.p04.DMA_DONEn = not(b.p04.DMA_DONE);
  
  c.p04.MYTE = tock_pos(a.p04.PHI_OUT, b.p04.PHI_OUT, b.p04.LAPA,   b.p04.MYTE, b.p04.DMA_DONEn);
  c.p04.LENE = tock_pos(a.p04.PHI_OUT, b.p04.PHI_OUT, b.p01.SYS_RESETn2, b.p04.LENE, b.p04.LUVY);

  c.p04.LARA = nand(b.p04.DMA_RUNNING, !b.p04.MYTE, b.p01.SYS_RESETn2);
  c.p04.MATU = tock_pos(a.p01.PHI_OUTn, b.p01.PHI_OUTn, b.p01.SYS_RESETn2, b.p04.MATU, b.p04.DMA_RUNNING);
  c.p04.MORY = nand(b.p04.MATU, b.p04.LOGO);

  c.p04.DO_DMA = not(b.p04.MORY);
  c.p04.LOGO = not(b.p04.MUDA);
  
  c.p04.DMA_A15n = not(b.p04.DMA_A15);

  c.p04.MUHO = nand(b.p04.MATU, b.p04.MUDA);

  c.p04.OAM_ADDR_DMA = not(b.p04.MATU);

  c.p04.FF46_WRn = nand(b.p22.FF46, b.p07.CPU_WR2);
  c.p04.FF46_WR = not(b.p04.FF46_WRn);

  c.p04.LYXE = or(b.p04.FF46_WR, b.p04.LOKO);

  c.p04.LOKO = nand(b.p01.SYS_RESETn2, !b.p04.LENE);
  c.p04.LAPA = not(b.p04.LOKO);

  c.p04.DMA_RUNNING = nand(b.p04.LARA, !b.p04.LENE);
  c.p04.DMA_CLK = and(b.p01.PHI_OUTn, b.p04.DMA_RUNNING);

  //----------
  // FF46 DMA

  c.p04.DMA_A00 = tock_pos( a.p04.DMA_CLK,  b.p04.DMA_CLK, b.p04.LAPA, b.p04.DMA_A00, !b.p04.DMA_A00);
  c.p04.DMA_A01 = tock_pos(!a.p04.DMA_A00, !b.p04.DMA_A00, b.p04.LAPA, b.p04.DMA_A01, !b.p04.DMA_A01);
  c.p04.DMA_A02 = tock_pos(!a.p04.DMA_A01, !b.p04.DMA_A01, b.p04.LAPA, b.p04.DMA_A02, !b.p04.DMA_A02);
  c.p04.DMA_A03 = tock_pos(!a.p04.DMA_A02, !b.p04.DMA_A02, b.p04.LAPA, b.p04.DMA_A03, !b.p04.DMA_A03);
  c.p04.DMA_A04 = tock_pos(!a.p04.DMA_A03, !b.p04.DMA_A03, b.p04.LAPA, b.p04.DMA_A04, !b.p04.DMA_A04);
  c.p04.DMA_A05 = tock_pos(!a.p04.DMA_A04, !b.p04.DMA_A04, b.p04.LAPA, b.p04.DMA_A05, !b.p04.DMA_A05);
  c.p04.DMA_A06 = tock_pos(!a.p04.DMA_A05, !b.p04.DMA_A05, b.p04.LAPA, b.p04.DMA_A06, !b.p04.DMA_A06);
  c.p04.DMA_A07 = tock_pos(!a.p04.DMA_A06, !b.p04.DMA_A06, b.p04.LAPA, b.p04.DMA_A07, !b.p04.DMA_A07);

  c.p04.DMA_A08 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A08, b.D0);
  c.p04.DMA_A09 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A09, b.D1);
  c.p04.DMA_A10 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A10, b.D2);
  c.p04.DMA_A11 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A11, b.D3);
  c.p04.DMA_A12 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A12, b.D4);
  c.p04.DMA_A13 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A13, b.D5);
  c.p04.DMA_A14 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A14, b.D6);
  c.p04.DMA_A15 = tock_pos(a.p04.FF46_WR, b.p04.FF46_WR, 0, b.p04.DMA_A15, b.D7);

  //----------
  // FF46 read
  c.p04.FF46_D0 = not(!b.p04.DMA_A08);
  c.p04.FF46_D1 = not(!b.p04.DMA_A09);
  c.p04.FF46_D2 = not(!b.p04.DMA_A10);
  c.p04.FF46_D3 = not(!b.p04.DMA_A11);
  c.p04.FF46_D4 = not(!b.p04.DMA_A12);
  c.p04.FF46_D5 = not(!b.p04.DMA_A13);
  c.p04.FF46_D6 = not(!b.p04.DMA_A14);
  c.p04.FF46_D7 = not(!b.p04.DMA_A15);

  c.p04.FF46_RDn1 = nand(b.p22.FF46, b.p07.CPU_RD2);
  c.p04.FF46_RD   = not(b.p04.FF46_RDn1);
  c.p04.FF46_RDn2 = not(b.p04.FF46_RD);

  // polarity looks backwards
  if (b.p04.FF46_RDn2) {
    c.D0 = b.p04.FF46_D0;
    c.D1 = b.p04.FF46_D1;
    c.D2 = b.p04.FF46_D2;
    c.D3 = b.p04.FF46_D3;
    c.D4 = b.p04.FF46_D4;
    c.D5 = b.p04.FF46_D5;
    c.D6 = b.p04.FF46_D6;
    c.D7 = b.p04.FF46_D7;
  }

  //----------

  c.p04.MA00 = b.p04.DMA_A00;
  c.p04.MA01 = b.p04.DMA_A01;
  c.p04.MA02 = b.p04.DMA_A02;
  c.p04.MA03 = b.p04.DMA_A03;
  c.p04.MA04 = b.p04.DMA_A04;
  c.p04.MA05 = b.p04.DMA_A05;
  c.p04.MA06 = b.p04.DMA_A06;
  c.p04.MA07 = b.p04.DMA_A07;
  c.p04.MA08 = b.p04.DMA_A08;
  c.p04.MA09 = b.p04.DMA_A09;
  c.p04.MA10 = b.p04.DMA_A10;
  c.p04.MA11 = b.p04.DMA_A11;
  c.p04.MA12 = b.p04.DMA_A12;

  // polarity looks backwards
  c.p04.VRAM_TO_OAM  = not(b.p04.MUHO);
  c.p04.VRAM_TO_OAMn = not(b.p04.VRAM_TO_OAM);
  if (b.p04.VRAM_TO_OAMn) {
    c.chip.MA00 = b.p04.MA00;
    c.chip.MA01 = b.p04.MA01;
    c.chip.MA02 = b.p04.MA02;
    c.chip.MA03 = b.p04.MA03;
    c.chip.MA04 = b.p04.MA04;
    c.chip.MA05 = b.p04.MA05;
    c.chip.MA06 = b.p04.MA06;
    c.chip.MA07 = b.p04.MA07;
    c.chip.MA08 = b.p04.MA08;
    c.chip.MA09 = b.p04.MA09;
    c.chip.MA10 = b.p04.MA10;
    c.chip.MA11 = b.p04.MA11;
    c.chip.MA12 = b.p04.MA12;
  }
}
