#include "P02_Interrupts.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P02_Interrupts::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P02_Interrupts& pb = gb.p02;
  P02_Interrupts& pc = gc.p02;

  pc.KERY = or(gb.chip.P13_C, gb.chip.P12_C, gb.chip.P11_C, gb.chip.P10_C);

  pc.AWOB = latch_pos(ga.BOGA_1M, gb.BOGA_1M, pb.AWOB, pb.KERY);

  pc.BATU = tock_pos(ga.BOGA_1M, gb.BOGA_1M, gb.RESET2, pb.BATU, pb.KERY);
  pc.ACEF = tock_pos(ga.BOGA_1M, gb.BOGA_1M, gb.RESET2, pb.ACEF, pb.BATU);
  pc.AGEM = tock_pos(ga.BOGA_1M, gb.BOGA_1M, gb.RESET2, pb.AGEM, pb.ACEF);
  pc.APUG = tock_pos(ga.BOGA_1M, gb.BOGA_1M, gb.RESET2, pb.APUG, pc.AGEM);
  
  pc.ASOK = and(pb.APUG, pb.BATU);

  pc.ROTU = not(gb.FF0F_WR);
  pc.LETY = not(gb.cpu.FROM_CPU9);
  pc.MUXE = or(gb.cpu.D0, gb.FF0F);
  pc.LUFE = not(gb.cpu.FROM_CPU7);
  pc.SULO = or(gb.cpu.D3, gb.FF0F_WR);
  pc.LAMO = not(gb.cpu.FROM_CPU11); 
  pc.SEME = or(gb.cpu.D4, gb.FF0F);
  pc.LEJA = not(gb.cpu.FROM_CPU8);
  pc.NABE = or(gb.cpu.D1, gb.FF0F);
  pc.LESA = not(gb.cpu.FROM_CPU10);
  pc.RAKE = or(gb.cpu.D2, gb.FF0F);

  pc.MYZU = nand(pb.ROTU, pb.LETY, gb.cpu.D0);
  pc.LYTA = and(pb.MUXE, pb.LETY, gb.RESET2);

  pc.TOME = nand(pb.ROTU, pb.LUFE, gb.cpu.D3);
  pc.TUNY = and(pb.SULO, pb.LUFE, gb.RESET2);

  pc.TOGA = nand(pb.ROTU, pb.LAMO, gb.cpu.D4);
  pc.TYME = and(pb.SEME, pb.LAMO, gb.RESET2);

  pc.MODY = nand(pb.ROTU, pb.LEJA, gb.cpu.D1);
  pc.MOVU = and(pb.NABE, pb.LEJA, gb.RESET2);

  pc.PYHU = nand(pb.ROTU, pb.LESA, gb.cpu.D2);
  pc.PYGA = and(pb.RAKE, pb.LESA, gb.RESET2);

  pc.PESU = not(gb.chip.P10_B);

  pc.LOPE = srtock_pos(ga.INT_VBL_BUF, gb.INT_VBL_BUF, pb.MYZU, pb.LYTA, pb.LOPE, pb.PESU);
  pc.UBUL = srtock_pos(ga.INT_SERIAL,  gb.INT_SERIAL,  pb.TOME, pb.TUNY, pb.UBUL, pb.PESU);
  pc.ULAK = srtock_pos(ga.INT_JP,      gb.INT_JP,      pb.TOGA, pb.TYME, pb.ULAK, pb.PESU);
  pc.LALU = srtock_pos(ga.INT_STAT,    gb.INT_STAT,    pb.MODY, pb.MOVU, pb.LALU, pb.PESU);
  pc.NYBO = srtock_pos(ga.INT_TIMER,   gb.INT_TIMER,   pb.PYHU, pb.PYGA, pb.NYBO, pb.PESU);

  pc.POLA = not(gb.FF0F_RD);

  pc.MATY = latch_pos(ga.FF0F_RD, gb.FF0F_RD, pb.MATY, pb.LOPE);
  pc.NEJY = latch_pos(ga.FF0F_RD, gb.FF0F_RD, pb.NEJY, pb.UBUL);
  pc.NUTY = latch_pos(ga.FF0F_RD, gb.FF0F_RD, pb.NUTY, pb.ULAK);
  pc.MOPO = latch_pos(ga.FF0F_RD, gb.FF0F_RD, pb.MOPO, pb.LALU);
  pc.PAVY = latch_pos(ga.FF0F_RD, gb.FF0F_RD, pb.PAVY, pb.NYBO);

  pc.NELA = not(pb.MATY);
  pc.PADO = not(pb.NEJY);
  pc.PEGY = not(pb.NUTY);
  pc.NABO = not(pb.MOPO);
  pc.ROVA = not(pb.PAVY);

  gc.INT_JP  = pb.ASOK;

  gc.cpu.TO_CPU2 = pb.AWOB;
  gc.cpu.TO_CPU3 = pb.LOPE;
  gc.cpu.TO_CPU4 = pb.UBUL;
  gc.cpu.TO_CPU5 = pb.ULAK;
  gc.cpu.TO_CPU6 = pb.LALU;
  gc.cpu.TO_CPU7 = pb.NYBO;

  if (pb.POLA) {
    gc.cpu.D0 = pb.NELA;
    gc.cpu.D1 = pb.NABO;
    gc.cpu.D2 = pb.ROVA;
    gc.cpu.D3 = pb.PADO;
    gc.cpu.D4 = pb.PEGY;
  }
}

