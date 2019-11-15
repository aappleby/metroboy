#include "P08_ExtCpuBuses.h"
#include "../Schematics.h"
#include "Gameboy.h"

/*
void blah(const Gameboy& b, Gameboy& c) {

  c.p08.ADDR_0000_7FFF = not(b.A15);
  c.p08.ADDR_NOT_VRAM = or(b.A13, b.A14, b.p08.ADDR_0000_7FFF);

  c.p08.TEXO = and(b.cpu.ADDR_VALID, b.p08.ADDR_NOT_VRAM);

  c.p08.LEVO = not(b.p08.TEXO);

  c.p08.LAGU = unk3(b.cpu.CPU_RAW_RD, b.p08.LEVO, b.cpu.CPU_RAW_WR);

  c.p08.LYWE = not(b.p08.LAGU);

  c.p08.MOCA = nor(b.p08.TEXO, b.p07.MODE_DBG1);
  c.p08.MEXO = not(b.p01.CPU_WR_SYNC);
  c.p08.NEVY = or(b.p08.MEXO, b.p08.MOCA);
  c.p08.MOTY = or(b.p08.MOCA, b.p08.LYWE);
  c.p08.PUVA = or(b.p08.NEVY, b.p04.DO_DMA);
  c.p08.TYMU = nor(b.p04.DO_DMA, b.p08.MOTY);

  c.p08.MODE_DBG1n = not(b.p07.MODE_DBG1);
  c.p08.A1n = not(b.A01);
  c.p08.LOXO = unk3(b.p08.MODE_DBG1n, b.p08.TEXO, b.p07.MODE_DBG1);
  c.p08.LASY = not(b.p08.LOXO);
  c.p08.MATE = not(b.p08.LASY);

  c.p08.SOGY = not(b.A14);
  c.p08.CART_RAM = and(b.A13, b.p08.SOGY, b.A15);

  c.p08.TYNU = unk3(b.A15, b.A14, b.p08.CART_RAM);

  c.p08.TOZA = and(b.p01.CPU_RD_SYNC, b.p08.TYNU, b.p07.ADDR_0000_FE00);

  c.p08.CS_A = mux2(b.p04.DMA_A15, b.p08.TOZA, b.p04.DO_DMA); // polarity?
  c.p08.WR_A = nand(b.p08.PUVA, b.p08.MODE_DBG2n1);
  c.p08.WR_D = nor (b.p08.PUVA, b.p07.MODE_DBG2);
  c.p08.RD_A = nand(b.p08.TYMU, b.p08.MODE_DBG2n1);
  c.p08.RD_D = nor (b.p08.TYMU, b.p07.MODE_DBG2);

  c.chip.CS_A = b.p08.CS_A;
  c.chip.WR_A = b.p08.WR_A;
  c.chip.WR_D = b.p08.WR_D;
  c.chip.RD_A = b.p08.RD_A;
  c.chip.RD_D = b.p08.RD_D;
}
*/

