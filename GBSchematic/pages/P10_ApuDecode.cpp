#include "P10_ApuDecode.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P10_ApuDecode::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P10_ApuDecode pa = ga.p10;
  const P10_ApuDecode pb = gb.p10;
  P10_ApuDecode pc = gc.p10;

  pc.AMUS = nor(gb.A00, gb.A01, gb.A02, gb.A03, gb.A04, gb.A07);
  pc.ANAP = and(pb.AMUS, gb.p07.FFXX);
  pc.BYKO = not(gb.A05);
  pc.AKUG = not(gb.A06);
  pc.ATOZ = nand(pb.BYKO, pb.AKUG, gb.p07.CPU_WR, pb.ANAP);
  pc.ACAT = and(pb.ANAP, gb.p07.CPU_RD, pb.AKUG, pb.BYKO);

  pc.BOXY = not(gb.A05);
  pc.AWET = or(gb.A04, pb.BOXY, gb.A06, gb.A07);
  pc.BEZY = or(pb.AWET, gb.p07.FFXXn);

  pc.AVUN = not(gb.A07);
  pc.ASAD = not(gb.A06);
  pc.ACOM = nand(pb.AVUN, pb.ASAD, gb.A05, gb.A04);
  pc.BARO = nor(pb.ACOM, gb.p07.FFXXn);

  pc.ATUP = not(gb.A04);
  pc.ATEG = or(pb.ATUP, gb.A05, gb.A06, gb.A07);
  pc.BUNO = nor(gb.p07.FFXXn, pb.ATEG);
  pc.BANU = not(pb.BUNO);

  pc.CONA = not(pb.FF2Xn);
  pc.DOXY = and(pb.CONA, pb.XXX6);

  pc.BAFU = not(gb.p07.CPU_WR);
  pc.BOGY = not(pb.BAFU);

  pc.TACE = and(gb.p13.CH1_AMP_ENn, gb.p15.CH2_AMP_ENn, gb.p16.FF1A_D7n, gb.p20.CH4_AMP_ENn);

  // Biiiig decoder

  pc.DYTE = not(gb.A00);
  pc.DOSO = not(pb.DYTE);
  pc.AFOB = not(gb.A01);
  pc.DUPA = not(pb.AFOB);
  pc.ABUB = not(gb.A02);
  pc.DENO = not(pb.ABUB);
  pc.ACOL = not(gb.A03);
  pc.DUCE = not(pb.ACOL);

  // FF10 - 0000
  pc.DUPO = nand(pb.ACOL, pb.ABUB, pb.AFOB, pb.DYTE);
  pc.DYVA = nor(pb.DUPO, pb.FF1Xn);

  // FF24 - 0100
  pc.DATU = nand(pb.ACOL, pb.DENO, pb.AFOB, pb.DYTE);
  pc.CAFY = nor(pb.DATU, pb.FF2Xn);

  // FF16 - 0110
  pc.DAZA = nand(pb.ACOL, pb.DENO, pb.DUPA, pb.DYTE);
  pc.COVY = nor(pb.DAZA, pb.FF1Xn);

  // FF25 - 0101
  pc.DURA = nand(pb.ACOL, pb.DENO, pb.AFOB, pb.DOSO);
  pc.CORA = nor(pb.DURA, pb.FF2Xn);

  // FF17 - 0111
  pc.DUVU = nand(pb.ACOL, pb.DENO, pb.DUPA, pb.DOSO);
  pc.DUTU = nor(pb.DUVU, pb.FF1Xn);

  // FF22 - 0010
  pc.DOFA = nand(pb.ACOL, pb.ABUB, pb.DUPA, pb.DYTE);
  pc.EKEZ = nor(pb.DOFA, pb.FF2Xn);

  // FF12 - 0010
  pc.DAMY = nand(pb.ACOL, pb.ABUB, pb.DUPA, pb.DYTE);
  pc.EDAF = nor(pb.DAMY, pb.FF1Xn);

  // FF23 - 0011
  pc.DUFE = nand(pb.ACOL, pb.ABUB, pb.DUPA, pb.DOSO);
  pc.CUGE = nor(pb.DUFE, pb.FF2Xn);

  // FF11 - 0001
  pc.DUNO = nand(pb.ACOL, pb.ABUB, pb.AFOB, pb.DOSO);
  pc.CAXE = nor(pb.DUNO, pb.FF1Xn);

  // FF21 - 0001
  pc.DEWA = nand(pb.ACOL, pb.ABUB, pb.AFOB, pb.DOSO);
  pc.COVO = nor(pb.DEWA, pb.FF1Xn);

  // FF19 - 1001
  pc.DEJY = nand(pb.DUCE, pb.ABUB, pb.AFOB, pb.DOSO);
  pc.DOZA = nor(pb.DEJY, pb.FF1Xn);

  // FF20 - 0000
  pc.DONA = nand(pb.ACOL, pb.ABUB, pb.AFOB, pb.DYTE);
  pc.DANU = nor(pb.DONA, pb.FF2Xn);

  // FF18 - 1000
  pc.DAFY = nand(pb.DUCE, pb.ABUB, pb.AFOB, pb.DYTE);
  pc.DARA = nor(pb.DAFY, pb.FF1Xn);

  // FF1D - 1101
  pc.EMOS = nand(pb.DUCE, pb.DENO, pb.AFOB, pb.DOSO);
  pc.FENY = nor(pb.EMOS, pb.FF1Xn);

  // XXX6 - 0110
  pc.EKAG = nand(pb.ACOL, pb.DENO, pb.DUPA, pb.DYTE);

  // FF14 - 0100
  pc.ESOT = nand(pb.ACOL, pb.DENO, pb.AFOB, pb.DYTE);
  pc.DUJA = nor(pb.ESOT, pb.FF1Xn);

  // FF1E - 1110
  pc.EGEN = nand(pb.DUCE, pb.DENO, pb.DUPA, pb.DYTE);
  pc.DUGO = nor(pb.EGEN, pb.FF1Xn);

  // FF1A - 1010
  pc.EXAT = nand(pb.DUCE, pb.ABUB, pb.DUPA, pb.DYTE);
  pc.EMOR = nor(pb.EXAT, pb.FF1Xn);

  // FF1B - 1011
  pc.EMAX = nand(pb.DUCE, pb.ABUB, pb.DUPA, pb.DOSO);
  pc.DUSA = nor(pb.EMAX, pb.FF1Xn);

  // FF13 - 0011
  pc.ETUF = nand(pb.ACOL, pb.ABUB, pb.DUPA, pb.DOSO);
  pc.DECO = nor(pb.ETUF, pb.FF1Xn);

  // FF1C - 1100
  pc.GANY = nand(pb.DUCE, pb.DENO, pb.AFOB, pb.DYTE);
  pc.GEFO = nor(pb.GANY, pb.FF1Xn);
}
