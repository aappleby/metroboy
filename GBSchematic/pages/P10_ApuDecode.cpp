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

  pc.AMUS = nor(gb.cpu.A00, gb.cpu.A01, gb.cpu.A02, gb.cpu.A03, gb.cpu.A04, gb.cpu.A07);
  pc.ANAP = and(pb.AMUS, gb.FFXX);
  pc.BYKO = not(gb.cpu.A05);
  pc.AKUG = not(gb.cpu.A06);
  pc.ATOZ = nand(pb.BYKO, pb.AKUG, gb.CPU_WR, pb.ANAP);
  pc.ACAT = and(pb.ANAP, gb.CPU_RD, pb.AKUG, pb.BYKO);

  pc.BOXY = not(gb.cpu.A05);
  pc.AWET = or(gb.cpu.A04, pb.BOXY, gb.cpu.A06, gb.cpu.A07);
  pc.BEZY = or(pb.AWET, gb.FFXXn);

  pc.AVUN = not(gb.cpu.A07);
  pc.ASAD = not(gb.cpu.A06);
  pc.ACOM = nand(pb.AVUN, pb.ASAD, gb.cpu.A05, gb.cpu.A04);
  pc.BARO = nor(pb.ACOM, gb.FFXXn);

  pc.ATUP = not(gb.cpu.A04);
  pc.ATEG = or(pb.ATUP, gb.cpu.A05, gb.cpu.A06, gb.cpu.A07);
  pc.BUNO = nor(gb.FFXXn, pb.ATEG);
  pc.BANU = not(pb.BUNO);

  pc.CONA = not(pb.FF2Xn);
  pc.DOXY = and(pb.CONA, pb.XXX6);

  pc.BAFU = not(gb.CPU_WR);
  pc.BOGY = not(pb.BAFU);

  pc.TACE = and(gb.CH1_AMP_ENn, gb.CH2_AMP_ENn, gb.FF1A_D7n, gb.CH4_AMP_ENn);

  pc.FF00WR  = pb.ATOZ;
  pc.FF00RD  = pb.ACAT;
  pc.FF2Xn   = pb.BEZY;
  pc.FF3X    = pb.BARO;
  pc.FF1Xn   = pb.BANU;
  pc.FF2X    = pb.CONA;
  pc.FF26    = pb.DOXY;
  pc.APU_WR  = pb.BOGY;
  pc.AMP_ENn = pb.TACE;

  // Biiiig decoder

  pc.DYTE = not(gb.cpu.A00);
  pc.DOSO = not(pb.DYTE);
  pc.AFOB = not(gb.cpu.A01);
  pc.DUPA = not(pb.AFOB);
  pc.ABUB = not(gb.cpu.A02);
  pc.DENO = not(pb.ABUB);
  pc.ACOL = not(gb.cpu.A03);
  pc.DUCE = not(pb.ACOL);

  // 0 pb.ACOL pb.ABUB pb.AFOB pb.DYTE
  // 1 DUCE pb.DENO pb.DUPA pb.DOSO

  // FF10 - 0000
  pc.DUPO = nand(pb.ACOL, pb.ABUB, pb.AFOB, pb.DYTE);
  pc.DYVA = nor(pb.DUPO, pb.FF1Xn);
  pc.FF10 = pb.DYVA;

  // FF24 - 0100
  pc.DATU = nand(pb.ACOL, pb.DENO, pb.AFOB, pb.DYTE);
  pc.CAFY = nor(pb.DATU, pb.FF2Xn);
  pc.FF24 = pb.CAFY;

  // FF16 - 0110
  pc.DAZA = nand(pb.ACOL, pb.DENO, pb.DUPA, pb.DYTE);
  pc.COVY = nor(pb.DAZA, pb.FF1Xn);
  pc.FF16 = pb.COVY;

  // FF25 - 0101
  pc.DURA = nand(pb.ACOL, pb.DENO, pb.AFOB, pb.DOSO);
  pc.CORA = nor(pb.DURA, pb.FF2Xn);
  pc.FF25 = pb.CORA;

  // FF17 - 0111
  pc.DUVU = nand(pb.ACOL, pb.DENO, pb.DUPA, pb.DOSO);
  pc.DUTU = nor(pb.DUVU, pb.FF1Xn);
  pc.FF17 = pb.DUTU;

  // FF22 - 0010
  pc.DOFA = nand(pb.ACOL, pb.ABUB, pb.DUPA, pb.DYTE);
  pc.EKEZ = nor(pb.DOFA, pb.FF2Xn);
  pc.FF22 = pb.EKEZ;

  // FF12 - 0010
  pc.DAMY = nand(pb.ACOL, pb.ABUB, pb.DUPA, pb.DYTE);
  pc.EDAF = nor(pb.DAMY, pb.FF1Xn);
  pc.FF12 = pb.EDAF;

  // FF23 - 0011
  pc.DUFE = nand(pb.ACOL, pb.ABUB, pb.DUPA, pb.DOSO);
  pc.CUGE = nor(pb.DUFE, pb.FF2Xn);
  pc.FF23 = pb.CUGE;

  // FF11 - 0001
  pc.DUNO = nand(pb.ACOL, pb.ABUB, pb.AFOB, pb.DOSO);
  pc.CAXE = nor(pb.DUNO, pb.FF1Xn);
  pc.FF11 = pb.CAXE;

  // FF21 - 0001
  pc.DEWA = nand(pb.ACOL, pb.ABUB, pb.AFOB, pb.DOSO);
  pc.COVO = nor(pb.DEWA, pb.FF1Xn);
  pc.FF21 = pb.COVO;

  // FF19 - 1001
  pc.DEJY = nand(pb.DUCE, pb.ABUB, pb.AFOB, pb.DOSO);
  pc.DOZA = nor(pb.DEJY, pb.FF1Xn);
  pc.FF19 = pb.DOZA;

  // FF20 - 0000
  pc.DONA = nand(pb.ACOL, pb.ABUB, pb.AFOB, pb.DYTE);
  pc.DANU = nor(pb.DONA, pb.FF2Xn);
  pc.FF20 = pb.DANU;

  // FF18 - 1000
  pc.DAFY = nand(pb.DUCE, pb.ABUB, pb.AFOB, pb.DYTE);
  pc.DARA = nor(pb.DAFY, pb.FF1Xn);
  pc.FF18 = pb.DARA;

  // FF1D - 1101
  pc.EMOS = nand(pb.DUCE, pb.DENO, pb.AFOB, pb.DOSO);
  pc.FENY = nor(pb.EMOS, pb.FF1Xn);
  pc.FF1D = pb.FENY;

  // XXX6 - 0110
  pc.EKAG = nand(pb.ACOL, pb.DENO, pb.DUPA, pb.DYTE);
  pc.XXX6 = pb.EKAG;

  // FF14 - 0100
  pc.ESOT = nand(pb.ACOL, pb.DENO, pb.AFOB, pb.DYTE);
  pc.DUJA = nor(pb.ESOT, pb.FF1Xn);
  pc.FF14 = pb.DUJA;

  // FF1E - 1110
  pc.EGEN = nand(pb.DUCE, pb.DENO, pb.DUPA, pb.DYTE);
  pc.DUGO = nor(pb.EGEN, pb.FF1Xn);
  pc.FF1E = pb.DUGO;

  // FF1A - 1010
  pc.EXAT = nand(pb.DUCE, pb.ABUB, pb.DUPA, pb.DYTE);
  pc.EMOR = nor(pb.EXAT, pb.FF1Xn);
  pc.FF1A = pb.EMOR;

  // FF1B - 1011
  pc.EMAX = nand(pb.DUCE, pb.ABUB, pb.DUPA, pb.DOSO);
  pc.DUSA = nor(pb.EMAX, pb.FF1Xn);
  pc.FF1B = pb.DUSA;

  // FF13 - 0011
  pc.ETUF = nand(pb.ACOL, pb.ABUB, pb.DUPA, pb.DOSO);
  pc.DECO = nor(pb.ETUF, pb.FF1Xn);
  pc.FF13 = pb.DECO;

  // FF1C - 1100
  pc.GANY = nand(pb.DUCE, pb.DENO, pb.AFOB, pb.DYTE);
  pc.GEFO = nor(pb.GANY, pb.FF1Xn);
  pc.FF1C = pb.GEFO;
}
