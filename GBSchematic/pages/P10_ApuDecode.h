#pragma once

struct Gameboy;

struct P10_ApuDecode {
  bool APU_WR;
  bool AMP_ENn;

  bool FF00RD;
  bool FF00WR;
  bool FF1Xn;
  bool FF2X;
  bool FF3X;
  bool FF2Xn;
  bool XXX6;

  bool FF10,FF11,FF12,FF13,FF14,FF15,FF16,FF17;
  bool FF18,FF19,FF1A,FF1B,FF1C,FF1D,FF1E,FF1F;
  bool FF20,FF21,FF22,FF23,FF24,FF25,FF26,FF27;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  bool AMUS,ANAP,BYKO,AKUG,ATOZ,ACAT;
  bool BOXY,AWET,BEZY;
  bool AVUN,ASAD,ACOM,BARO;
  bool ATUP,ATEG,BUNO,BANU;
  bool CONA,DOXY,BAFU,BOGY,TACE;
  bool DYTE,DOSO,AFOB,DUPA,ABUB,DENO,ACOL,DUCE;

  bool CAFY,CAXE,CORA,COVO,COVY,CUGE,DAFY,DAMY;
  bool DANU,DARA,DATU,DAZA,DECO,DEJY,DEWA,DOFA;
  bool DONA,DOZA,DUFE,DUGO,DUJA,DUNO,DUPO,DURA;
  bool DUSA,DUTU,DUVU,DYVA,EDAF,EGEN,EKAG,EKEZ;
  bool EMAX,EMOR,EMOS,ESOT,ETUF,EXAT,FENY,GANY;
  bool GEFO;
};

