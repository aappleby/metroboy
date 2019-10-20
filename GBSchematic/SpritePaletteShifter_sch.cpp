// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "Clocks.h"
#include "ExtBus.h"
#include "PPU.h"
#include "Sprites.h"

//----------
// inputs

//----------
// outputs

//----------
// registers

reg RUGO;
reg SATA;
reg ROSA;
reg SOMY;
reg PALU;
reg NUKE;
reg MODA;
reg LYME;

//-----------------------------------------------------------------------------

void tick_spritepaletteshifter() {
  bool MEFU = or(spr.XEFY, ppu.SPR_PIX_A_0, ppu.SPR_PIX_B_0);
  bool MEVE = or(spr.XEFY, ppu.SPR_PIX_A_1, ppu.SPR_PIX_B_1);
  bool MYZO = or(spr.XEFY, ppu.SPR_PIX_A_2, ppu.SPR_PIX_B_2);
  bool RUDA = or(spr.XEFY, ppu.SPR_PIX_A_3, ppu.SPR_PIX_B_3);
  bool VOTO = or(spr.XEFY, ppu.SPR_PIX_A_4, ppu.SPR_PIX_B_4);
  bool VYSA = or(spr.XEFY, ppu.SPR_PIX_A_5, ppu.SPR_PIX_B_5);
  bool TORY = or(spr.XEFY, ppu.SPR_PIX_A_6, ppu.SPR_PIX_B_6);
  bool WOPE = or(spr.XEFY, ppu.SPR_PIX_A_7, ppu.SPR_PIX_B_7);

  ppu.LESY = not(MEFU);
  ppu.LOTA = not(MEVE);
  ppu.LYKU = not(MYZO);
  ppu.ROBY = not(RUDA);
  ppu.TYTA = not(VOTO);
  ppu.TYCO = not(VYSA);
  ppu.SOKA = not(TORY);
  ppu.XOVU = not(WOPE);

  bool GOMO_Q = spr.GOMO.q();

  bool SYPY = not(!GOMO_Q);
  bool TOTU = not(!GOMO_Q);
  bool NARO = not(!GOMO_Q);
  bool WEXY = not(!GOMO_Q);
  bool RYZY = not(!GOMO_Q);
  bool RYFE = not(!GOMO_Q);
  bool LADY = not(!GOMO_Q);
  bool LAFY = not(!GOMO_Q);

  bool PUME = nand(ppu.LESY, !GOMO_Q);
  bool SUCO = nand(ppu.LESY, SYPY);

  bool SORO = nand(ppu.LOTA, !GOMO_Q);
  bool TAFA = nand(ppu.LOTA, TOTU);

  bool PAMO = nand(ppu.LYKU, !GOMO_Q);
  bool PYZY = nand(ppu.LYKU, NARO);

  bool SUKY = nand(ppu.ROBY, !GOMO_Q);
  bool TOWA = nand(ppu.ROBY, WEXY);

  bool RORA = nand(ppu.TYTA, !GOMO_Q);
  bool RUDU = nand(ppu.TYTA, RYZY);

  bool MENE = nand(ppu.TYCO, !GOMO_Q);
  bool PAZO = nand(ppu.TYCO, RYFE);

  bool LUKE = nand(ppu.SOKA, !GOMO_Q);
  bool LOWA = nand(ppu.SOKA, LADY);

  bool LAMY = nand(ppu.XOVU, !GOMO_Q);
  bool LUNU = nand(ppu.XOVU, LAFY);

  bool RUGO_Q = RUGO.srtock(clk.CLKPIPE, PUME, SUCO, ext.P10_B);
  bool SATA_Q = SATA.srtock(clk.CLKPIPE, SORO, TAFA, RUGO_Q);
  bool ROSA_Q = ROSA.srtock(clk.CLKPIPE, PAMO, PYZY, SATA_Q);
  bool SOMY_Q = SOMY.srtock(clk.CLKPIPE, SUKY, TOWA, ROSA_Q);
  bool PALU_Q = PALU.srtock(clk.CLKPIPE, RORA, RUDU, SOMY_Q);
  bool NUKE_Q = NUKE.srtock(clk.CLKPIPE, MENE, PAZO, PALU_Q);
  bool MODA_Q = MODA.srtock(clk.CLKPIPE, LUKE, LOWA, NUKE_Q);
  bool LYME_Q = LYME.srtock(clk.CLKPIPE, LAMY, LUNU, MODA_Q);

  bool LOME = not(LYME_Q);
  bool LAFU = nand(LOME, ppu.BGPIXELn);
  bool LEKA = nand(LYME_Q, ppu.BGPIXELn);

  ppu.OBP0PIXELn = LAFU;
  ppu.OBP1PIXELn = LEKA;
}