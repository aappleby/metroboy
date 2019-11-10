#pragma once

struct Gameboy;

struct P10_ApuDecode {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool ATOZ; bool FF00WR; };
  union { bool ACAT; bool FF00RD; };
  union { bool BEZY; bool FF2Xn; };
  union { bool BARO; bool FF3X; };
  union { bool BANU; bool FF1Xn; };
  union { bool CONA; bool FF2X; };
  union { bool DOXY; bool FF26; };
  union { bool BOGY; bool APU_WR; };
  union { bool TACE; bool AMP_ENn; };

  union { bool DYVA; bool FF10; };
  union { bool CAFY; bool FF24; };
  union { bool COVY; bool FF16; };
  union { bool CORA; bool FF25; };
  union { bool DUTU; bool FF17; };
  union { bool EKEZ; bool FF22; };
  union { bool EDAF; bool FF12; };
  union { bool CUGE; bool FF23; };
  union { bool CAXE; bool FF11; };
  union { bool COVO; bool FF21; };
  union { bool DOZA; bool FF19; };
  union { bool DANU; bool FF20; };
  union { bool DARA; bool FF18; };
  union { bool FENY; bool FF1D; };
  union { bool DUJA; bool FF14; };
  union { bool DUGO; bool FF1E; };
  union { bool EMOR; bool FF1A; };
  union { bool DUSA; bool FF1B; };
  union { bool DECO; bool FF13; };
  union { bool GEFO; bool FF1C; };

  union { bool EKAG; bool XXX6; };

  union { bool ANAP; }; // -> p07.APER

private:

  bool AMUS,BYKO,AKUG;
  bool BOXY,AWET;
  bool AVUN,ASAD,ACOM;
  bool ATUP,ATEG,BUNO;
  bool BAFU;
  bool DYTE,DOSO,AFOB,DUPA,ABUB,DENO,ACOL,DUCE;

  bool DAFY,DAMY;
  bool DATU,DAZA,DEJY,DEWA,DOFA;
  bool DONA,DUFE,DUNO,DUPO,DURA;
  bool DUVU,EGEN;
  bool EMAX,EMOS,ESOT,ETUF,EXAT,GANY;
};

