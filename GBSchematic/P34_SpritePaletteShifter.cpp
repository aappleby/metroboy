#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P34_SpritePaletteShifter {
  struct Input {
    bool CLKPIPE;
    bool XEFY;
    bool GOMO;
    bool BGPIXELn;
    bool P10_B;

    bool SPR_PIX_A_0;
    bool SPR_PIX_A_1;
    bool SPR_PIX_A_2;
    bool SPR_PIX_A_3;
    bool SPR_PIX_A_4;
    bool SPR_PIX_A_5;
    bool SPR_PIX_A_6;
    bool SPR_PIX_A_7;

    bool SPR_PIX_B_0;
    bool SPR_PIX_B_1;
    bool SPR_PIX_B_2;
    bool SPR_PIX_B_3;
    bool SPR_PIX_B_4;
    bool SPR_PIX_B_5;
    bool SPR_PIX_B_6;
    bool SPR_PIX_B_7;
  };

  struct Output {
    bool LESY,LOTA,LYKU,ROBY,TYTA,TYCO,SOKA,XOVU; // P34 or of spr_pix_A/B

    bool OBP0PIXELn;
    bool OBP1PIXELn;
  };

  // P34 Sprite palette shift register;
  reg RUGO,SATA,ROSA,SOMY,PALU,NUKE,MODA,LYME;

  void tick(const Input& in, Output& out) {
    bool MEFU = or(in.XEFY, in.SPR_PIX_A_0, in.SPR_PIX_B_0);
    bool MEVE = or(in.XEFY, in.SPR_PIX_A_1, in.SPR_PIX_B_1);
    bool MYZO = or(in.XEFY, in.SPR_PIX_A_2, in.SPR_PIX_B_2);
    bool RUDA = or(in.XEFY, in.SPR_PIX_A_3, in.SPR_PIX_B_3);
    bool VOTO = or(in.XEFY, in.SPR_PIX_A_4, in.SPR_PIX_B_4);
    bool VYSA = or(in.XEFY, in.SPR_PIX_A_5, in.SPR_PIX_B_5);
    bool TORY = or(in.XEFY, in.SPR_PIX_A_6, in.SPR_PIX_B_6);
    bool WOPE = or(in.XEFY, in.SPR_PIX_A_7, in.SPR_PIX_B_7);

    wire LESY = not(MEFU);
    wire LOTA = not(MEVE);
    wire LYKU = not(MYZO);
    wire ROBY = not(RUDA);
    wire TYTA = not(VOTO);
    wire TYCO = not(VYSA);
    wire SOKA = not(TORY);
    wire XOVU = not(WOPE);

    bool SYPY = not(in.GOMO);
    bool TOTU = not(in.GOMO);
    bool NARO = not(in.GOMO);
    bool WEXY = not(in.GOMO);
    bool RYZY = not(in.GOMO);
    bool RYFE = not(in.GOMO);
    bool LADY = not(in.GOMO);
    bool LAFY = not(in.GOMO);

    bool PUME = nand(LESY, in.GOMO);
    bool SUCO = nand(LESY, SYPY);

    bool SORO = nand(LOTA, in.GOMO);
    bool TAFA = nand(LOTA, TOTU);

    bool PAMO = nand(LYKU, in.GOMO);
    bool PYZY = nand(LYKU, NARO);

    bool SUKY = nand(ROBY, in.GOMO);
    bool TOWA = nand(ROBY, WEXY);

    bool RORA = nand(TYTA, in.GOMO);
    bool RUDU = nand(TYTA, RYZY);

    bool MENE = nand(TYCO, in.GOMO);
    bool PAZO = nand(TYCO, RYFE);

    bool LUKE = nand(SOKA, in.GOMO);
    bool LOWA = nand(SOKA, LADY);

    bool LAMY = nand(XOVU, in.GOMO);
    bool LUNU = nand(XOVU, LAFY);

    bool RUGO_Q = RUGO.srtock(in.CLKPIPE, PUME, SUCO, in.P10_B);
    bool SATA_Q = SATA.srtock(in.CLKPIPE, SORO, TAFA, RUGO_Q);
    bool ROSA_Q = ROSA.srtock(in.CLKPIPE, PAMO, PYZY, SATA_Q);
    bool SOMY_Q = SOMY.srtock(in.CLKPIPE, SUKY, TOWA, ROSA_Q);
    bool PALU_Q = PALU.srtock(in.CLKPIPE, RORA, RUDU, SOMY_Q);
    bool NUKE_Q = NUKE.srtock(in.CLKPIPE, MENE, PAZO, PALU_Q);
    bool MODA_Q = MODA.srtock(in.CLKPIPE, LUKE, LOWA, NUKE_Q);
    bool LYME_Q = LYME.srtock(in.CLKPIPE, LAMY, LUNU, MODA_Q);

    bool LOME = not(LYME_Q);
    bool LAFU = nand(LOME, in.BGPIXELn);
    bool LEKA = nand(LYME_Q, in.BGPIXELn);

    out.LESY = not(MEFU);
    out.LOTA = not(MEVE);
    out.LYKU = not(MYZO);
    out.ROBY = not(RUDA);
    out.TYTA = not(VOTO);
    out.TYCO = not(VYSA);
    out.SOKA = not(TORY);
    out.XOVU = not(WOPE);

    out.OBP0PIXELn = LAFU;
    out.OBP1PIXELn = LEKA;
  }};