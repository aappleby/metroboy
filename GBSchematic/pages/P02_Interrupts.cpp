#include "P02_Interrupts.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P02_Interrupts::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P02_Interrupts& pb = gb.p02;
  P02_Interrupts& pc = gc.p02;

  pc.KERY = or(gb.chip.P13_C, gb.chip.P12_C, gb.chip.P11_C, gb.chip.P10_C);

  pc.AWOB = latch_pos(gb.p01.BOGA_1M, pb.AWOB, pb.KERY);

  pc.BATU = tock_pos(ga.p01.BOGA_1M, gb.p01.BOGA_1M, gb.p01.RESET2, pb.BATU, pb.KERY);
  pc.ACEF = tock_pos(ga.p01.BOGA_1M, gb.p01.BOGA_1M, gb.p01.RESET2, pb.ACEF, pb.BATU);
  pc.AGEM = tock_pos(ga.p01.BOGA_1M, gb.p01.BOGA_1M, gb.p01.RESET2, pb.AGEM, pb.ACEF);
  pc.APUG = tock_pos(ga.p01.BOGA_1M, gb.p01.BOGA_1M, gb.p01.RESET2, pb.APUG, pc.AGEM);
  
  pc.ASOK = and(pb.APUG, pb.BATU);

  pc.ROTU = not(gb.p07.FF0F_WR);
  pc.LETY = not(gb.cpu.FROM_CPU9);
  pc.MUXE = or(gb.D0, gb.p07.FF0F_WR);
  pc.LUFE = not(gb.cpu.FROM_CPU7);
  pc.SULO = or(gb.D3, gb.p07.FF0F_WR);
  pc.LAMO = not(gb.cpu.FROM_CPU11); 
  pc.SEME = or(gb.D4, gb.p07.FF0F_WR);
  pc.LEJA = not(gb.cpu.FROM_CPU8);
  pc.NABE = or(gb.D1, gb.p07.FF0F_WR);
  pc.LESA = not(gb.cpu.FROM_CPU10);
  pc.RAKE = or(gb.D2, gb.p07.FF0F_WR);

  pc.MYZU = nand(pb.ROTU, pb.LETY, gb.D0);
  pc.LYTA = and(pb.MUXE, pb.LETY, gb.p01.RESET2);

  pc.TOME = nand(pb.ROTU, pb.LUFE, gb.D3);
  pc.TUNY = and(pb.SULO, pb.LUFE, gb.p01.RESET2);

  pc.TOGA = nand(pb.ROTU, pb.LAMO, gb.D4);
  pc.TYME = and(pb.SEME, pb.LAMO, gb.p01.RESET2);

  pc.MODY = nand(pb.ROTU, pb.LEJA, gb.D1);
  pc.MOVU = and(pb.NABE, pb.LEJA, gb.p01.RESET2);

  pc.PYHU = nand(pb.ROTU, pb.LESA, gb.D2);
  pc.PYGA = and(pb.RAKE, pb.LESA, gb.p01.RESET2);

  pc.PESU = not(gb.chip.P10_B);

  pc.LOPE = srtock_pos(ga.p21.INT_VBL_BUF, gb.p21.INT_VBL_BUF, pb.MYZU, pb.LYTA, pb.LOPE, pb.PESU);
  pc.UBUL = srtock_pos(ga.p06.INT_SERIAL,  gb.p06.INT_SERIAL,  pb.TOME, pb.TUNY, pb.UBUL, pb.PESU);
  pc.ULAK = srtock_pos(ga.p02.INT_JP,      gb.p02.INT_JP,      pb.TOGA, pb.TYME, pb.ULAK, pb.PESU);
  pc.LALU = srtock_pos(ga.p21.INT_STAT,    gb.p21.INT_STAT,    pb.MODY, pb.MOVU, pb.LALU, pb.PESU);
  pc.NYBO = srtock_pos(ga.p03.INT_TIMER,   gb.p03.INT_TIMER,   pb.PYHU, pb.PYGA, pb.NYBO, pb.PESU);

  pc.POLA = not(gb.p07.FF0F_RD);

  pc.MATY = latch_pos(gb.p07.FF0F_RD, pb.MATY, pb.LOPE);
  pc.NEJY = latch_pos(gb.p07.FF0F_RD, pb.NEJY, pb.UBUL);
  pc.NUTY = latch_pos(gb.p07.FF0F_RD, pb.NUTY, pb.ULAK);
  pc.MOPO = latch_pos(gb.p07.FF0F_RD, pb.MOPO, pb.LALU);
  pc.PAVY = latch_pos(gb.p07.FF0F_RD, pb.PAVY, pb.NYBO);

  pc.NELA = not(pb.MATY);
  pc.PADO = not(pb.NEJY);
  pc.PEGY = not(pb.NUTY);
  pc.NABO = not(pb.MOPO);
  pc.ROVA = not(pb.PAVY);

  gc.cpu.TO_CPU2 = pb.AWOB;
  gc.cpu.TO_CPU3 = pb.LOPE;
  gc.cpu.TO_CPU4 = pb.UBUL;
  gc.cpu.TO_CPU5 = pb.ULAK;
  gc.cpu.TO_CPU6 = pb.LALU;
  gc.cpu.TO_CPU7 = pb.NYBO;

  if (pb.POLA) {
    gc.D0 = pb.NELA;
    gc.D1 = pb.NABO;
    gc.D2 = pb.ROVA;
    gc.D3 = pb.PADO;
    gc.D4 = pb.PEGY;
  }
}

