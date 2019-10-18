// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "Clocks.h"
#include "ExtBus.h"
#include "PpuRegs.h"

//----------
// inputs

extern bool XEFY;
extern reg GOMO;

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
  bool MEFU = or(XEFY, SPR_PIX_A_0, SPR_PIX_B_0);
  bool MEVE = or(XEFY, SPR_PIX_A_1, SPR_PIX_B_1);
  bool MYZO = or(XEFY, SPR_PIX_A_2, SPR_PIX_B_2);
  bool RUDA = or(XEFY, SPR_PIX_A_3, SPR_PIX_B_3);
  bool VOTO = or(XEFY, SPR_PIX_A_4, SPR_PIX_B_4);
  bool VYSA = or(XEFY, SPR_PIX_A_5, SPR_PIX_B_5);
  bool TORY = or(XEFY, SPR_PIX_A_6, SPR_PIX_B_6);
  bool WOPE = or(XEFY, SPR_PIX_A_7, SPR_PIX_B_7);

  LESY = not(MEFU);
  LOTA = not(MEVE);
  LYKU = not(MYZO);
  ROBY = not(RUDA);
  TYTA = not(VOTO);
  TYCO = not(VYSA);
  SOKA = not(TORY);
  XOVU = not(WOPE);

  bool GOMO_Q = GOMO.q();

  bool SYPY = not(!GOMO_Q);
  bool TOTU = not(!GOMO_Q);
  bool NARO = not(!GOMO_Q);
  bool WEXY = not(!GOMO_Q);
  bool RYZY = not(!GOMO_Q);
  bool RYFE = not(!GOMO_Q);
  bool LADY = not(!GOMO_Q);
  bool LAFY = not(!GOMO_Q);

  bool PUME = nand(LESY, !GOMO_Q);
  bool SUCO = nand(LESY, SYPY);

  bool SORO = nand(LOTA, !GOMO_Q);
  bool TAFA = nand(LOTA, TOTU);

  bool PAMO = nand(LYKU, !GOMO_Q);
  bool PYZY = nand(LYKU, NARO);

  bool SUKY = nand(ROBY, !GOMO_Q);
  bool TOWA = nand(ROBY, WEXY);

  bool RORA = nand(TYTA, !GOMO_Q);
  bool RUDU = nand(TYTA, RYZY);

  bool MENE = nand(TYCO, !GOMO_Q);
  bool PAZO = nand(TYCO, RYFE);

  bool LUKE = nand(SOKA, !GOMO_Q);
  bool LOWA = nand(SOKA, LADY);

  bool LAMY = nand(XOVU, !GOMO_Q);
  bool LUNU = nand(XOVU, LAFY);

  bool RUGO_Q = RUGO.srtock(CLKPIPE, PUME, SUCO, P10_B);
  bool SATA_Q = SATA.srtock(CLKPIPE, SORO, TAFA, RUGO_Q);
  bool ROSA_Q = ROSA.srtock(CLKPIPE, PAMO, PYZY, SATA_Q);
  bool SOMY_Q = SOMY.srtock(CLKPIPE, SUKY, TOWA, ROSA_Q);
  bool PALU_Q = PALU.srtock(CLKPIPE, RORA, RUDU, SOMY_Q);
  bool NUKE_Q = NUKE.srtock(CLKPIPE, MENE, PAZO, PALU_Q);
  bool MODA_Q = MODA.srtock(CLKPIPE, LUKE, LOWA, NUKE_Q);
  bool LYME_Q = LYME.srtock(CLKPIPE, LAMY, LUNU, MODA_Q);

  bool LOME = not(LYME_Q);
  bool LAFU = nand(LOME, BGPIXELn);
  bool LEKA = nand(LYME_Q, BGPIXELn);

  OBP0PIXELn = LAFU;
  OBP1PIXELn = LEKA;


}