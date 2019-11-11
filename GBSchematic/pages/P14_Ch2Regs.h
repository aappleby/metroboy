#pragma once

struct Gameboy;

struct P14_Ch2Regs {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool BENY; };
  union { bool BERA; bool FF16_D6; };
  union { bool BAMY; bool FF16_D7; };

  union { bool JEFU; bool FF19_D0; };
  union { bool JANY; bool FF19_D1; };
  union { bool JUPY; bool FF19_D2; };
  union { bool EMER; bool FF19_D6; };
  union { bool ETAP; bool FF19_D7; };

  union { bool HERO; bool CH2_FTICK; };
  union { bool AGYN; bool FF16_WR; };

private:

  bool FOGE,FAPE;
  bool ASYP;
  bool BACU,BUDU;
  bool BYGO,CORO,CEKA,CECY;

  bool ENUF,JYBU,FYRY,GURU;
  bool GURE,GEXA,GERE,JEDE;
  bool GATA,FORE,GAGE,GURA,GUFE,HAVA,HORE,HYFU;
  bool HUPE,GENE,HYRY,HORO,HERE,HAVU,HYRE,HUVU;

  bool DOSA,HUDE,ESUR,EXUC,FYXO,FERY,GUZA,FUTY;
  bool FOFE,FOVA,FEDY,FOME,FORA,GODA,GUMY,GUPU;
  bool DONE,DYNU,EZOF,CYVO;
  bool EDEP;
  bool FUXO,GANO,GOCA,GANE;
  bool FAVA,FAJY,FEGU,FOSE,GERO,GAKY,GADU,GAZO;

  bool KYPU,JENU,KYSA;
  bool DETA,GOTE,HYPO;
  bool GALA,HEVY,HEPU;
  bool JEKE,JARO,HUNA;
  bool GADO,EVYF,FAZO,HUMA,GOJY;
};



