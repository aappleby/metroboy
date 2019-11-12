#include "P10_ApuDecode.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P10_ApuDecode::tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {

  c.p10.AMUS = nor(b.A00, b.A01, b.A02, b.A03, b.A04, b.A07);
  c.p10.ANAP = and(b.p10.AMUS, b.p07.FFXX);
  c.p10.BYKO = not(b.A05);
  c.p10.AKUG = not(b.A06);
  c.p10.FF00WR = nand(b.p10.BYKO, b.p10.AKUG, b.p07.CPU_WR, b.p10.ANAP);
  c.p10.FF00RD = and(b.p10.ANAP, b.p07.CPU_RD, b.p10.AKUG, b.p10.BYKO);

  c.p10.BOXY = not(b.A05);
  c.p10.AWET = or(b.A04, b.p10.BOXY, b.A06, b.A07);
  c.p10.FF2Xn = or(b.p10.AWET, b.p07.FFXXn);

  c.p10.AVUN = not(b.A07);
  c.p10.ASAD = not(b.A06);
  c.p10.ACOM = nand(b.p10.AVUN, b.p10.ASAD, b.A05, b.A04);
  c.p10.FF3X = nor(b.p10.ACOM, b.p07.FFXXn);

  c.p10.ATUP = not(b.A04);
  c.p10.ATEG = or(b.p10.ATUP, b.A05, b.A06, b.A07);
  c.p10.BUNO = nor(b.p07.FFXXn, b.p10.ATEG);
  c.p10.FF1Xn = not(b.p10.BUNO);

  c.p10.FF2X = not(b.p10.FF2Xn);
  c.p10.FF26 = and(b.p10.FF2X, b.p10.XXX6);

  c.p10.BAFU = not(b.p07.CPU_WR);
  c.p10.APU_WR = not(b.p10.BAFU);

  c.p10.AMP_ENn = and(b.p13.CH1_AMP_ENn, b.p15.CH2_AMP_ENn, b.p16.FF1A_D7n, b.p20.CH4_AMP_ENn);

  // Biiiig FF13der

  c.p10.A00_n1 = not(b.A00);
  c.p10.A00_1  = not(b.p10.A00_n1);
  c.p10.A01_n1 = not(b.A01);
  c.p10.A01_1  = not(b.p10.A01_n1);
  c.p10.A02_n1 = not(b.A02);
  c.p10.A02_1  = not(b.p10.A02_n1);
  c.p10.A03_n1 = not(b.A03);
  c.p10.A03_1  = not(b.p10.A03_n1);

  // FF10 - 0000
  c.p10.DUPO = nand(b.p10.A03_n1, b.p10.A02_n1, b.p10.A01_n1, b.p10.A00_n1);
  c.p10.FF10 = nor(b.p10.DUPO, b.p10.FF1Xn);

  // FF24 - 0100
  c.p10.DATU = nand(b.p10.A03_n1, b.p10.A02_1, b.p10.A01_n1, b.p10.A00_n1);
  c.p10.FF24 = nor(b.p10.DATU, b.p10.FF2Xn);

  // FF16 - 0110
  c.p10.DAZA = nand(b.p10.A03_n1, b.p10.A02_1, b.p10.A01_1, b.p10.A00_n1);
  c.p10.FF16 = nor(b.p10.DAZA, b.p10.FF1Xn);

  // FF25 - 0101
  c.p10.DURA = nand(b.p10.A03_n1, b.p10.A02_1, b.p10.A01_n1, b.p10.A00_1);
  c.p10.FF25 = nor(b.p10.DURA, b.p10.FF2Xn);

  // FF17 - 0111
  c.p10.DUVU = nand(b.p10.A03_n1, b.p10.A02_1, b.p10.A01_1, b.p10.A00_1);
  c.p10.FF17 = nor(b.p10.DUVU, b.p10.FF1Xn);

  // FF22 - 0010
  c.p10.DOFA = nand(b.p10.A03_n1, b.p10.A02_n1, b.p10.A01_1, b.p10.A00_n1);
  c.p10.FF22 = nor(b.p10.DOFA, b.p10.FF2Xn);

  // FF12 - 0010
  c.p10.DAMY = nand(b.p10.A03_n1, b.p10.A02_n1, b.p10.A01_1, b.p10.A00_n1);
  c.p10.FF12 = nor(b.p10.DAMY, b.p10.FF1Xn);

  // FF23 - 0011
  c.p10.DUFE = nand(b.p10.A03_n1, b.p10.A02_n1, b.p10.A01_1, b.p10.A00_1);
  c.p10.FF23 = nor(b.p10.DUFE, b.p10.FF2Xn);

  // FF11 - 0001
  c.p10.DUNO = nand(b.p10.A03_n1, b.p10.A02_n1, b.p10.A01_n1, b.p10.A00_1);
  c.p10.FF11 = nor(b.p10.DUNO, b.p10.FF1Xn);

  // FF21 - 0001
  c.p10.DEWA = nand(b.p10.A03_n1, b.p10.A02_n1, b.p10.A01_n1, b.p10.A00_1);
  c.p10.FF21 = nor(b.p10.DEWA, b.p10.FF1Xn);

  // FF19 - 1001
  c.p10.DEJY = nand(b.p10.A03_1, b.p10.A02_n1, b.p10.A01_n1, b.p10.A00_1);
  c.p10.FF19 = nor(b.p10.DEJY, b.p10.FF1Xn);

  // FF20 - 0000
  c.p10.DONA = nand(b.p10.A03_n1, b.p10.A02_n1, b.p10.A01_n1, b.p10.A00_n1);
  c.p10.FF20 = nor(b.p10.DONA, b.p10.FF2Xn);

  // FF18 - 1000
  c.p10.DAFY = nand(b.p10.A03_1, b.p10.A02_n1, b.p10.A01_n1, b.p10.A00_n1);
  c.p10.FF18 = nor(b.p10.DAFY, b.p10.FF1Xn);

  // FF1D - 1101
  c.p10.EMOS = nand(b.p10.A03_1, b.p10.A02_1, b.p10.A01_n1, b.p10.A00_1);
  c.p10.FF1D = nor(b.p10.EMOS, b.p10.FF1Xn);

  // XXX6 - 0110
  c.p10.XXX6 = nand(b.p10.A03_n1, b.p10.A02_1, b.p10.A01_1, b.p10.A00_n1);

  // FF14 - 0100
  c.p10.ESOT = nand(b.p10.A03_n1, b.p10.A02_1, b.p10.A01_n1, b.p10.A00_n1);
  c.p10.FF14 = nor(b.p10.ESOT, b.p10.FF1Xn);

  // FF1E - 1110
  c.p10.EGEN = nand(b.p10.A03_1, b.p10.A02_1, b.p10.A01_1, b.p10.A00_n1);
  c.p10.FF1E = nor(b.p10.EGEN, b.p10.FF1Xn);

  // FF1A - 1010
  c.p10.EXAT = nand(b.p10.A03_1, b.p10.A02_n1, b.p10.A01_1, b.p10.A00_n1);
  c.p10.FF1A = nor(b.p10.EXAT, b.p10.FF1Xn);

  // FF1B - 1011
  c.p10.EMAX = nand(b.p10.A03_1, b.p10.A02_n1, b.p10.A01_1, b.p10.A00_1);
  c.p10.FF1B = nor(b.p10.EMAX, b.p10.FF1Xn);

  // FF13 - 0011
  c.p10.ETUF = nand(b.p10.A03_n1, b.p10.A02_n1, b.p10.A01_1, b.p10.A00_1);
  c.p10.FF13 = nor(b.p10.ETUF, b.p10.FF1Xn);

  // FF1C - 1100
  c.p10.GANY = nand(b.p10.A03_1, b.p10.A02_1, b.p10.A01_n1, b.p10.A00_n1);
  c.p10.FF1C = nor(b.p10.GANY, b.p10.FF1Xn);
}
