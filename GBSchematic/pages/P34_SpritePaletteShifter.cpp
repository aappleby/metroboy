#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P34_SpritePaletteShifter::tick(const Gameboy& a, const Gameboy& b, Gameboy& c ) {
  c.p34.MEFU = or(b.p29.XEFY, b.p33.SPR_PIX_A_0, b.p33.SPR_PIX_B_0);
  c.p34.MEVE = or(b.p29.XEFY, b.p33.SPR_PIX_A_1, b.p33.SPR_PIX_B_1);
  c.p34.MYZO = or(b.p29.XEFY, b.p33.SPR_PIX_A_2, b.p33.SPR_PIX_B_2);
  c.p34.RUDA = or(b.p29.XEFY, b.p33.SPR_PIX_A_3, b.p33.SPR_PIX_B_3);
  c.p34.VOTO = or(b.p29.XEFY, b.p33.SPR_PIX_A_4, b.p33.SPR_PIX_B_4);
  c.p34.VYSA = or(b.p29.XEFY, b.p33.SPR_PIX_A_5, b.p33.SPR_PIX_B_5);
  c.p34.TORY = or(b.p29.XEFY, b.p33.SPR_PIX_A_6, b.p33.SPR_PIX_B_6);
  c.p34.WOPE = or(b.p29.XEFY, b.p33.SPR_PIX_A_7, b.p33.SPR_PIX_B_7);

  c.p34.LESY = not(b.p34.MEFU);
  c.p34.LOTA = not(b.p34.MEVE);
  c.p34.LYKU = not(b.p34.MYZO);
  c.p34.ROBY = not(b.p34.RUDA);
  c.p34.TYTA = not(b.p34.VOTO);
  c.p34.TYCO = not(b.p34.VYSA);
  c.p34.SOKA = not(b.p34.TORY);
  c.p34.XOVU = not(b.p34.WOPE);

  c.p34.SYPY = not(b.p31.OAM_A_D4);
  c.p34.TOTU = not(b.p31.OAM_A_D4);
  c.p34.NARO = not(b.p31.OAM_A_D4);
  c.p34.WEXY = not(b.p31.OAM_A_D4);
  c.p34.RYZY = not(b.p31.OAM_A_D4);
  c.p34.RYFE = not(b.p31.OAM_A_D4);
  c.p34.LADY = not(b.p31.OAM_A_D4);
  c.p34.LAFY = not(b.p31.OAM_A_D4);

  c.p34.PUME = nand(b.p34.LESY, b.p31.OAM_A_D4);
  c.p34.SUCO = nand(b.p34.LESY, b.p34.SYPY);

  c.p34.SORO = nand(b.p34.LOTA, b.p31.OAM_A_D4);
  c.p34.TAFA = nand(b.p34.LOTA, b.p34.TOTU);

  c.p34.PAMO = nand(b.p34.LYKU, b.p31.OAM_A_D4);
  c.p34.PYZY = nand(b.p34.LYKU, b.p34.NARO);

  c.p34.SUKY = nand(b.p34.ROBY, b.p31.OAM_A_D4);
  c.p34.TOWA = nand(b.p34.ROBY, b.p34.WEXY);

  c.p34.RORA = nand(b.p34.TYTA, b.p31.OAM_A_D4);
  c.p34.RUDU = nand(b.p34.TYTA, b.p34.RYZY);

  c.p34.MENE = nand(b.p34.TYCO, b.p31.OAM_A_D4);
  c.p34.PAZO = nand(b.p34.TYCO, b.p34.RYFE);

  c.p34.LUKE = nand(b.p34.SOKA, b.p31.OAM_A_D4);
  c.p34.LOWA = nand(b.p34.SOKA, b.p34.LADY);

  c.p34.LAMY = nand(b.p34.XOVU, b.p31.OAM_A_D4);
  c.p34.LUNU = nand(b.p34.XOVU, b.p34.LAFY);

  c.p34.RUGO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.PUME, b.p34.SUCO, b.p34.RUGO, b.chip.P10_B);
  c.p34.SATA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SORO, b.p34.TAFA, b.p34.SATA, b.p34.RUGO);
  c.p34.ROSA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.PAMO, b.p34.PYZY, b.p34.ROSA, b.p34.SATA);
  c.p34.SOMY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.SUKY, b.p34.TOWA, b.p34.SOMY, b.p34.ROSA);
  c.p34.PALU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.RORA, b.p34.RUDU, b.p34.PALU, b.p34.SOMY);
  c.p34.NUKE = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.MENE, b.p34.PAZO, b.p34.NUKE, b.p34.PALU);
  c.p34.MODA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.LUKE, b.p34.LOWA, b.p34.MODA, b.p34.NUKE);
  c.p34.LYME = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p34.LAMY, b.p34.LUNU, b.p34.LYME, b.p34.MODA);

  c.p34.LOME = not(b.p34.LYME);
  c.p34.LAFU = nand(b.p34.LOME, b.p35.BGPIXELn);
  c.p34.LEKA = nand(b.p34.LYME, b.p35.BGPIXELn);

  c.p34.LESY = not(b.p34.MEFU);
  c.p34.LOTA = not(b.p34.MEVE);
  c.p34.LYKU = not(b.p34.MYZO);
  c.p34.ROBY = not(b.p34.RUDA);
  c.p34.TYTA = not(b.p34.VOTO);
  c.p34.TYCO = not(b.p34.VYSA);
  c.p34.SOKA = not(b.p34.TORY);
  c.p34.XOVU = not(b.p34.WOPE);
}