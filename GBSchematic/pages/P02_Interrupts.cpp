#include "P02_Interrupts.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P02_Interrupts::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p02.KERY = or(b.chip.P13_C, b.chip.P12_C, b.chip.P11_C, b.chip.P10_C);

  c.p02.AWOB = latch_pos(b.p01.CLK_ABCDExxx1, b.p02.AWOB, b.p02.KERY);

  c.p02.BATU = tock_pos(a.p01.CLK_ABCDExxx1, b.p01.CLK_ABCDExxx1, b.p01.SYS_RESETn1, b.p02.BATU, b.p02.KERY);
  c.p02.ACEF = tock_pos(a.p01.CLK_ABCDExxx1, b.p01.CLK_ABCDExxx1, b.p01.SYS_RESETn1, b.p02.ACEF, b.p02.BATU);
  c.p02.AGEM = tock_pos(a.p01.CLK_ABCDExxx1, b.p01.CLK_ABCDExxx1, b.p01.SYS_RESETn1, b.p02.AGEM, b.p02.ACEF);
  c.p02.APUG = tock_pos(a.p01.CLK_ABCDExxx1, b.p01.CLK_ABCDExxx1, b.p01.SYS_RESETn1, b.p02.APUG, c.p02.AGEM);
  
  c.p02.ASOK = and(b.p02.APUG, b.p02.BATU);

  c.p02.ROTU = not(b.p07.FF0F_WR);
  c.p02.LETY = not(b.cpu.FROM_CPU9);
  c.p02.MUXE = or(b.D0, b.p07.FF0F_WR);
  c.p02.LUFE = not(b.cpu.FROM_CPU7);
  c.p02.SULO = or(b.D3, b.p07.FF0F_WR);
  c.p02.LAMO = not(b.cpu.FROM_CPU11); 
  c.p02.SEME = or(b.D4, b.p07.FF0F_WR);
  c.p02.LEJA = not(b.cpu.FROM_CPU8);
  c.p02.NABE = or(b.D1, b.p07.FF0F_WR);
  c.p02.LESA = not(b.cpu.FROM_CPU10);
  c.p02.RAKE = or(b.D2, b.p07.FF0F_WR);

  c.p02.MYZU = nand(b.p02.ROTU, b.p02.LETY, b.D0);
  c.p02.LYTA = and(b.p02.MUXE, b.p02.LETY, b.p01.SYS_RESETn1);

  c.p02.TOME = nand(b.p02.ROTU, b.p02.LUFE, b.D3);
  c.p02.TUNY = and(b.p02.SULO, b.p02.LUFE, b.p01.SYS_RESETn1);

  c.p02.TOGA = nand(b.p02.ROTU, b.p02.LAMO, b.D4);
  c.p02.TYME = and(b.p02.SEME, b.p02.LAMO, b.p01.SYS_RESETn1);

  c.p02.MODY = nand(b.p02.ROTU, b.p02.LEJA, b.D1);
  c.p02.MOVU = and(b.p02.NABE, b.p02.LEJA, b.p01.SYS_RESETn1);

  c.p02.PYHU = nand(b.p02.ROTU, b.p02.LESA, b.D2);
  c.p02.PYGA = and(b.p02.RAKE, b.p02.LESA, b.p01.SYS_RESETn1);

  c.p02.PESU = not(b.chip.P10_B);

  c.p02.LOPE = srtock_pos(a.p21.INT_VBL_BUF, b.p21.INT_VBL_BUF, b.p02.MYZU, b.p02.LYTA, b.p02.LOPE, b.p02.PESU);
  c.p02.UBUL = srtock_pos(a.p06.INT_SERIAL,  b.p06.INT_SERIAL,  b.p02.TOME, b.p02.TUNY, b.p02.UBUL, b.p02.PESU);
  c.p02.ULAK = srtock_pos(a.p02.INT_JP,      b.p02.INT_JP,      b.p02.TOGA, b.p02.TYME, b.p02.ULAK, b.p02.PESU);
  c.p02.LALU = srtock_pos(a.p21.INT_STAT,    b.p21.INT_STAT,    b.p02.MODY, b.p02.MOVU, b.p02.LALU, b.p02.PESU);
  c.p02.NYBO = srtock_pos(a.p03.INT_TIMER,   b.p03.INT_TIMER,   b.p02.PYHU, b.p02.PYGA, b.p02.NYBO, b.p02.PESU);

  c.p02.POLA = not(b.p07.FF0F_RD);

  c.p02.MATY = latch_pos(b.p07.FF0F_RD, b.p02.MATY, b.p02.LOPE);
  c.p02.NEJY = latch_pos(b.p07.FF0F_RD, b.p02.NEJY, b.p02.UBUL);
  c.p02.NUTY = latch_pos(b.p07.FF0F_RD, b.p02.NUTY, b.p02.ULAK);
  c.p02.MOPO = latch_pos(b.p07.FF0F_RD, b.p02.MOPO, b.p02.LALU);
  c.p02.PAVY = latch_pos(b.p07.FF0F_RD, b.p02.PAVY, b.p02.NYBO);

  c.p02.NELA = not(b.p02.MATY);
  c.p02.PADO = not(b.p02.NEJY);
  c.p02.PEGY = not(b.p02.NUTY);
  c.p02.NABO = not(b.p02.MOPO);
  c.p02.ROVA = not(b.p02.PAVY);

  if (b.p02.POLA) {
    c.D0 = b.p02.NELA;
    c.D1 = b.p02.NABO;
    c.D2 = b.p02.ROVA;
    c.D3 = b.p02.PADO;
    c.D4 = b.p02.PEGY;
  }
}

