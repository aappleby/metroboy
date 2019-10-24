#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P10_ApuDecode {
  struct Input {
    bool CPU_WR;
    bool CPU_RD;
    bool FFXX;
    bool FFXXn;
    bool XXX6;

    bool CH1_AMP_ENn;
    bool CH2_AMP_ENn;
    bool CH4_AMP_ENn;
    bool FF1A_D7n;

    bool A0,A1,A2,A3,A4,A5,A6,A7;
  };

  struct Output {
    bool DUCE;

    bool APU_WR;
    bool AMP_ENn;

    bool FF00RD;
    bool FF00WR;
    bool FF1Xn;
    bool FF2X;
    bool FF3X;
    bool FF2Xn;
    bool XXX6;

    bool FF10;
    bool FF11;
    bool FF12;
    bool FF13;
    bool FF14;
    bool FF16;
    bool FF17;
    bool FF18;
    bool FF19;
    bool FF1A;
    bool FF1B;
    bool FF1C;
    bool FF1D;
    bool FF1E;

    bool FF20;
    bool FF21;
    bool FF22;
    bool FF23;
    bool FF24;
    bool FF25;
    bool FF26;
  };

  void tick_apudecode(const Input& in, Output& out) {
    wire AMUS = nor(in.A0, in.A1, in.A2, in.A3, in.A4, in.A7);
    wire ANAP = and(AMUS, in.FFXX);
    wire BYKO = not(in.A5);
    wire AKUG = not(in.A6);
    wire ATOZ = nand(BYKO, AKUG, in.CPU_WR, ANAP);
    wire ACAT = and(ANAP, in.CPU_RD, AKUG, BYKO);
    out.FF00WR = ATOZ;
    out.FF00RD = ACAT;

    wire BOXY = not(in.A5);
    wire AWET = or(in.A4, BOXY, in.A6, in.A7);
    wire BEZY = or(AWET, in.FFXXn);
    out.FF2Xn = BEZY;

    wire AVUN = not(in.A7);
    wire ASAD = not(in.A6);
    wire ACOM = nand(AVUN, ASAD, in.A5, in.A4);
    wire BARO = nor(ACOM, in.FFXXn);
    out.FF3X = BARO;

    wire ATUP = not(in.A4);
    wire ATEG = or(ATUP, in.A5, in.A6, in.A7);
    wire BUNO = nor(in.FFXXn, ATEG);
    wire BANU = not(BUNO);
    out.FF1Xn = BANU;

    wire CONA = not(out.FF2Xn);
    out.FF2X = CONA;
    wire DOXY = and(CONA, in.XXX6);
    out.FF26 = DOXY;

    wire BAFU = not(in.CPU_WR);
    wire BOGY = not(BAFU);
    out.APU_WR = BOGY;

    wire TACE = and(in.CH1_AMP_ENn, in.CH2_AMP_ENn, in.FF1A_D7n, in.CH4_AMP_ENn);
    out.AMP_ENn = TACE;

    // Biiiig decoder

    wire DYTE = not(in.A0);
    wire DOSO = not(DYTE);
    wire AFOB = not(in.A1);
    wire DUPA = not(AFOB);
    wire ABUB = not(in.A2);
    wire DENO = not(ABUB);
    wire ACOL = not(in.A3);
    wire DUCE = not(ACOL);
    out.DUCE = DUCE;

    // 0 ACOL ABUB AFOB DYTE
    // 1 DUCE DENO DUPA DOSO

    // FF10 - 0000
    wire DUPO = nand(ACOL, ABUB, AFOB, DYTE);
    wire DYVA = nor(DUPO, out.FF1Xn);
    out.FF10 = DYVA;

    // FF24 - 0100
    wire DATU = nand(ACOL, DENO, AFOB, DYTE);
    wire CAFY = nor(DATU, out.FF2Xn);
    out.FF24 = CAFY;

    // FF16 - 0110
    wire DAZA = nand(ACOL, DENO, DUPA, DYTE);
    wire COVY = nor(DAZA, out.FF1Xn);
    out.FF16 = COVY;

    // FF25 - 0101
    wire DURA = nand(ACOL, DENO, AFOB, DOSO);
    wire CORA = nor(DURA, out.FF2Xn);
    out.FF25 = CORA;

    // FF17 - 0111
    wire DUVU = nand(ACOL, DENO, DUPA, DOSO);
    wire DUTU = nor(DUVU, out.FF1Xn);
    out.FF17 = DUTU;

    // FF22 - 0010
    wire DOFA = nand(ACOL, ABUB, DUPA, DYTE);
    wire EKEZ = nor(DOFA, out.FF2Xn);
    out.FF22 = EKEZ;

    // FF12 - 0010
    wire DAMY = nand(ACOL, ABUB, DUPA, DYTE);
    wire EDAF = nor(DAMY, out.FF1Xn);
    out.FF12 = EDAF;

    // FF23 - 0011
    wire DUFE = nand(ACOL, ABUB, DUPA, DOSO);
    wire CUGE = nor(DUFE, out.FF2Xn);
    out.FF23 = CUGE;

    // FF11 - 0001
    wire DUNO = nand(ACOL, ABUB, AFOB, DOSO);
    wire CAXE = nor(DUNO, out.FF1Xn);
    out.FF11 = CAXE;

    // FF21 - 0001
    wire DEWA = nand(ACOL, ABUB, AFOB, DOSO);
    wire COVO = nor(DEWA, out.FF1Xn);
    out.FF21 = COVO;

    // FF19 - 1001
    wire DEJY = nand(DUCE, ABUB, AFOB, DOSO);
    wire DOZA = nor(DEJY, out.FF1Xn);
    out.FF19 = DOZA;

    // FF20 - 0000
    wire DONA = nand(ACOL, ABUB, AFOB, DYTE);
    wire DANU = nor(DONA, out.FF2Xn);
    out.FF20 = DANU;

    // FF18 - 1000
    wire DAFY = nand(DUCE, ABUB, AFOB, DYTE);
    wire DARA = nor(DAFY, out.FF1Xn);
    out.FF18 = DARA;

    // FF1D - 1101
    wire EMOS = nand(DUCE, DENO, AFOB, DOSO);
    wire FENY = nor(EMOS, out.FF1Xn);
    out.FF1D = FENY;

    // XXX6 - 0110
    wire EKAG = nand(ACOL, DENO, DUPA, DYTE);
    out.XXX6 = EKAG;

    // FF14 - 0100
    wire ESOT = nand(ACOL, DENO, AFOB, DYTE);
    wire DUJA = nor(ESOT, out.FF1Xn);
    out.FF14 = DUJA;

    // FF1E - 1110
    wire EGEN = nand(DUCE, DENO, DUPA, DYTE);
    wire DUGO = nor(EGEN, out.FF1Xn);
    out.FF1E = DUGO;

    // FF1A - 1010
    wire EXAT = nand(DUCE, ABUB, DUPA, DYTE);
    wire EMOR = nor(EXAT, out.FF1Xn);
    out.FF1A = EMOR;

    // FF1B - 1011
    wire EMAX = nand(DUCE, ABUB, DUPA, DOSO);
    wire DUSA = nor(EMAX, out.FF1Xn);
    out.FF1B = DUSA;

    // FF13 - 0011
    wire ETUF = nand(ACOL, ABUB, DUPA, DOSO);
    wire DECO = nor(ETUF, out.FF1Xn);
    out.FF13 = DECO;

    // FF1C - 1100
    wire GANY = nand(DUCE, DENO, AFOB, DYTE);
    wire GEFO = nor(GANY, out.FF1Xn);
    out.FF1C = GEFO;
  }
};

