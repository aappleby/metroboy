#pragma once

struct P20_Channel4 {
  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  union { bool CEPY; bool FF23_D6n; };

  union { bool JUWA; bool CH4_ACTIVEn; };
  union { bool GEVY; bool CH4_AMP_ENn; };
  union { bool DATO; bool CH4_BIT; };
  union { bool FEME; bool CH4_LFSR_CLK1; };
  union { bool GYVE; bool CH4_LFSR_CLK2; };
  union { bool KOPA; bool CH4_LFSR_CLK3; };
  union { bool FOWA; bool CH4_EG_DISABLE; };
  union { bool FOSY; bool CH4_EG_TICK; };
  union { bool GONE; bool CH4_RESTART; };
  union { bool GUZY; bool RST_FF23_D7; };
  union { bool GAME; bool LFSR_OUT; };

  //----------

  bool FF23;           // P10.KUGE

  bool FF21_D0;        // P19.EMOK
  bool FF21_D0n;       // P19.EMOK
  bool FF21_D1;        // P19.ETYJ
  bool FF21_D1n;       // P19.ETYJ
  bool FF21_D2;
  bool FF21_D2n;
  bool FF21_D3;
  bool FF21_D3n;
  bool FF21_D4;
  bool FF21_D5;
  bool FF21_D6;
  bool FF21_D7;

  bool FF22_D0n;       // P19.JARE
  bool FF22_D1n;       // P19.JERO
  bool FF22_D2n;       // P19.JAKY
  bool FF22_D3;        // P19.JAMY
  bool FF22_D3n;       // P19.JAMY
  bool FF22_D4;        // P19.FETA
  bool FF22_D4n;       // P19.FETA
  bool FF22_D5;        // P19.FYTO
  bool FF22_D5n;       // P19.FYTO
  bool FF22_D6;        // P19.GOGO
  bool FF22_D6n;       // P19.GOGO
  bool FF22_D7;        // P19.GAFO
  bool FF23_D6;        // P19.CUNY
  bool FF23_D7;        // P19.HOGA
  bool FUGO;           // P19.FUGO

//private:

  bool EMOF,ELAR,DUDU,ETAT,FURA,ETAR,EVER,ETOV;

  bool CEXO_00;
  bool DEKO_01;
  bool EZEF_02;
  bool EPOR_03;
  bool DURE_04;
  bool DALE_05;
  bool DOKE_06;
  bool DEMO_07;
  bool DOSE_08;
  bool DETE_09;
  bool ERUT_10;
  bool DOTA_11;
  bool DERE_12;
  bool ESEP_13;

  bool ETYR,ELYX,DARY,ERYF;

  bool COSA,DYRY,COMO,COTE,EZUL,BAGU,BEFA,ATEL,GUFA,KARA;

  //----------

  bool FELO;
  bool FOLE,ETEF,EDYF,ELAF;
  bool FEKO,FATY,FERU,FYRO;
  bool AKOF,BYZY,APYR,BOZA;
  bool DARO,CUTY;
  bool DUBO,EVUR,EMET,FYNO,ENUR,EROX;

  //----------

  bool ALOP,BOKY,ABEL,BAWA,BUXO,DOGO,COFE,CUNA,EJEX,GEXE,HURY,ENEC,DAPY,GOPA;

  //----------

  bool GASO,GYSU,FALE,HELU,HAZO,FEBY,GORA,GATY,HAPU,EFOT;
  bool HERY,FEGY,JERY,GENA,KYKU,KONY,KANU,JYCO,JYRE,JYFU;
  bool HYNO,GYBA,GARY,CARY,GOFU,GUNY,HUCE;

  bool GEPO,GOGE,HURA,KAVU,JYJA;

  bool JEPE,JAVO,HEPA,HORY,HENO,HYRO,HEZU;
  bool JOTO,KOMU,KETU,KUTA,KUZY,KYWY,JAJU,HAPE,JUXE;
};

