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

  union { bool DONE; bool CH2_FREQ_00; };
  union { bool DYNU; bool CH2_FREQ_01; };
  union { bool EZOF; bool CH2_FREQ_02; };
  union { bool CYVO; bool CH2_FREQ_03; };
  union { bool FUXO; bool CH2_FREQ_04; };
  union { bool GANO; bool CH2_FREQ_05; };
  union { bool GOCA; bool CH2_FREQ_06; };
  union { bool GANE; bool CH2_FREQ_07; };
  union { bool HEVY; bool CH2_FREQ_08; };
  union { bool HEPU; bool CH2_FREQ_09; };
  union { bool HERO; bool CH2_FREQ_10; bool CH2_FTICK; };

  union { bool AGYN; bool FF16_WR; };

  union { bool HYFU; bool FF17_D0; };
  union { bool HAVA; bool FF17_D1; };
  union { bool HORE; bool FF17_D2; };
  union { bool FORE; bool FF17_D3; };
  union { bool GATA; bool FF17_D4; };
  union { bool GUFE; bool FF17_D5; };
  union { bool GURA; bool FF17_D6; };
  union { bool GAGE; bool FF17_D7; };

  union { bool FOFE; bool FF18_D0; };
  union { bool FOVA; bool FF18_D1; };
  union { bool FEDY; bool FF18_D2; };
  union { bool FOME; bool FF18_D3; };
  union { bool FORA; bool FF18_D4; };
  union { bool GODA; bool FF18_D5; };
  union { bool GUMY; bool FF18_D6; };
  union { bool GUPU; bool FF18_D7; };

private:

  bool FOGE,FAPE;
  bool ASYP;
  bool BACU,BUDU;
  bool BYGO,CORO,CEKA,CECY;

  bool ENUF,JYBU,FYRY,GURU;
  bool GURE,GEXA,GERE,JEDE;
  bool HUPE,GENE,HYRY,HORO,HERE,HAVU,HYRE,HUVU;

  bool DOSA,HUDE,ESUR,EXUC,FYXO,FERY,GUZA,FUTY;
  bool EDEP;

  bool FAVA,FAJY,FEGU,FOSE,GERO,GAKY,GADU,GAZO;

  bool KYPU,JENU,KYSA;
  bool DETA,GOTE,HYPO;
  bool GALA;
  bool JEKE,JARO,HUNA;
  bool GADO,EVYF,FAZO,HUMA,GOJY;
};



