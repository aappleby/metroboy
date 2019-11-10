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
  c.p10.ATOZ = nand(b.p10.BYKO, b.p10.AKUG, b.p07.CPU_WR, b.p10.ANAP);
  c.p10.ACAT = and(b.p10.ANAP, b.p07.CPU_RD, b.p10.AKUG, b.p10.BYKO);

  c.p10.BOXY = not(b.A05);
  c.p10.AWET = or(b.A04, b.p10.BOXY, b.A06, b.A07);
  c.p10.BEZY = or(b.p10.AWET, b.p07.FFXXn);

  c.p10.AVUN = not(b.A07);
  c.p10.ASAD = not(b.A06);
  c.p10.ACOM = nand(b.p10.AVUN, b.p10.ASAD, b.A05, b.A04);
  c.p10.BARO = nor(b.p10.ACOM, b.p07.FFXXn);

  c.p10.ATUP = not(b.A04);
  c.p10.ATEG = or(b.p10.ATUP, b.A05, b.A06, b.A07);
  c.p10.BUNO = nor(b.p07.FFXXn, b.p10.ATEG);
  c.p10.BANU = not(b.p10.BUNO);

  c.p10.CONA = not(b.p10.FF2Xn);
  c.p10.DOXY = and(b.p10.CONA, b.p10.XXX6);

  c.p10.BAFU = not(b.p07.CPU_WR);
  c.p10.BOGY = not(b.p10.BAFU);

  c.p10.TACE = and(b.p13.CH1_AMP_ENn, b.p15.CH2_AMP_ENn, b.p16.FF1A_D7n, b.p20.CH4_AMP_ENn);

  // Biiiig decoder

  c.p10.DYTE = not(b.A00);
  c.p10.DOSO = not(b.p10.DYTE);
  c.p10.AFOB = not(b.A01);
  c.p10.DUPA = not(b.p10.AFOB);
  c.p10.ABUB = not(b.A02);
  c.p10.DENO = not(b.p10.ABUB);
  c.p10.ACOL = not(b.A03);
  c.p10.DUCE = not(b.p10.ACOL);

  // FF10 - 0000
  c.p10.DUPO = nand(b.p10.ACOL, b.p10.ABUB, b.p10.AFOB, b.p10.DYTE);
  c.p10.DYVA = nor(b.p10.DUPO, b.p10.FF1Xn);

  // FF24 - 0100
  c.p10.DATU = nand(b.p10.ACOL, b.p10.DENO, b.p10.AFOB, b.p10.DYTE);
  c.p10.CAFY = nor(b.p10.DATU, b.p10.FF2Xn);

  // FF16 - 0110
  c.p10.DAZA = nand(b.p10.ACOL, b.p10.DENO, b.p10.DUPA, b.p10.DYTE);
  c.p10.COVY = nor(b.p10.DAZA, b.p10.FF1Xn);

  // FF25 - 0101
  c.p10.DURA = nand(b.p10.ACOL, b.p10.DENO, b.p10.AFOB, b.p10.DOSO);
  c.p10.CORA = nor(b.p10.DURA, b.p10.FF2Xn);

  // FF17 - 0111
  c.p10.DUVU = nand(b.p10.ACOL, b.p10.DENO, b.p10.DUPA, b.p10.DOSO);
  c.p10.DUTU = nor(b.p10.DUVU, b.p10.FF1Xn);

  // FF22 - 0010
  c.p10.DOFA = nand(b.p10.ACOL, b.p10.ABUB, b.p10.DUPA, b.p10.DYTE);
  c.p10.EKEZ = nor(b.p10.DOFA, b.p10.FF2Xn);

  // FF12 - 0010
  c.p10.DAMY = nand(b.p10.ACOL, b.p10.ABUB, b.p10.DUPA, b.p10.DYTE);
  c.p10.EDAF = nor(b.p10.DAMY, b.p10.FF1Xn);

  // FF23 - 0011
  c.p10.DUFE = nand(b.p10.ACOL, b.p10.ABUB, b.p10.DUPA, b.p10.DOSO);
  c.p10.CUGE = nor(b.p10.DUFE, b.p10.FF2Xn);

  // FF11 - 0001
  c.p10.DUNO = nand(b.p10.ACOL, b.p10.ABUB, b.p10.AFOB, b.p10.DOSO);
  c.p10.CAXE = nor(b.p10.DUNO, b.p10.FF1Xn);

  // FF21 - 0001
  c.p10.DEWA = nand(b.p10.ACOL, b.p10.ABUB, b.p10.AFOB, b.p10.DOSO);
  c.p10.COVO = nor(b.p10.DEWA, b.p10.FF1Xn);

  // FF19 - 1001
  c.p10.DEJY = nand(b.p10.DUCE, b.p10.ABUB, b.p10.AFOB, b.p10.DOSO);
  c.p10.DOZA = nor(b.p10.DEJY, b.p10.FF1Xn);

  // FF20 - 0000
  c.p10.DONA = nand(b.p10.ACOL, b.p10.ABUB, b.p10.AFOB, b.p10.DYTE);
  c.p10.DANU = nor(b.p10.DONA, b.p10.FF2Xn);

  // FF18 - 1000
  c.p10.DAFY = nand(b.p10.DUCE, b.p10.ABUB, b.p10.AFOB, b.p10.DYTE);
  c.p10.DARA = nor(b.p10.DAFY, b.p10.FF1Xn);

  // FF1D - 1101
  c.p10.EMOS = nand(b.p10.DUCE, b.p10.DENO, b.p10.AFOB, b.p10.DOSO);
  c.p10.FENY = nor(b.p10.EMOS, b.p10.FF1Xn);

  // XXX6 - 0110
  c.p10.EKAG = nand(b.p10.ACOL, b.p10.DENO, b.p10.DUPA, b.p10.DYTE);

  // FF14 - 0100
  c.p10.ESOT = nand(b.p10.ACOL, b.p10.DENO, b.p10.AFOB, b.p10.DYTE);
  c.p10.DUJA = nor(b.p10.ESOT, b.p10.FF1Xn);

  // FF1E - 1110
  c.p10.EGEN = nand(b.p10.DUCE, b.p10.DENO, b.p10.DUPA, b.p10.DYTE);
  c.p10.DUGO = nor(b.p10.EGEN, b.p10.FF1Xn);

  // FF1A - 1010
  c.p10.EXAT = nand(b.p10.DUCE, b.p10.ABUB, b.p10.DUPA, b.p10.DYTE);
  c.p10.EMOR = nor(b.p10.EXAT, b.p10.FF1Xn);

  // FF1B - 1011
  c.p10.EMAX = nand(b.p10.DUCE, b.p10.ABUB, b.p10.DUPA, b.p10.DOSO);
  c.p10.DUSA = nor(b.p10.EMAX, b.p10.FF1Xn);

  // FF13 - 0011
  c.p10.ETUF = nand(b.p10.ACOL, b.p10.ABUB, b.p10.DUPA, b.p10.DOSO);
  c.p10.DECO = nor(b.p10.ETUF, b.p10.FF1Xn);

  // FF1C - 1100
  c.p10.GANY = nand(b.p10.DUCE, b.p10.DENO, b.p10.AFOB, b.p10.DYTE);
  c.p10.GEFO = nor(b.p10.GANY, b.p10.FF1Xn);
}
