#pragma once

struct Gameboy;

struct P15_Channel2 {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool DEFU; bool CH2_ACTIVEn; };
  union { bool FUTE; bool CH2_AMP_ENn; };

  bool DOCA;
  bool ELOX;
  bool COGU;
  bool DERA;
  bool GYPA;
  bool EROG;

private:

  bool HOTA,KATY,JYNA,KYLO,KENE;

  // EG timer
  bool JORE,JONA,JEVY;

  bool KYVO;

  bool JOPA;
  bool HEPO,DOPE;
  bool JAKE;

  bool FF16_D6;
  bool FF16_D7;

  bool FF16_D6n;
  bool FF16_D7n;

  bool FF17_D0n;
  bool FF17_D1n;
  bool FF17_D2n;
  bool FF17_D3n;

  bool FF17_D0;
  bool FF17_D1;
  bool FF17_D2;
  bool FF17_D3;
  bool FF17_D4;
  bool FF17_D5;
  bool FF17_D6;
  bool FF17_D7;

  bool HAFE,HYPA;
  bool CYWU,DALA,CELO,DOXA,DORY,CEXE;
  bool HYLE,HYLY,JEME,GALU,GYKO,ETUK,DAVU,FUJY,GYRE,DUJU;
  bool BUWE,AZEG,ATEP,CAZA,BYHO,BUFO,BODO,CEMO,BUTA,CAMA;
  bool FOPY,FENA,ETUP,FOMY,FARU,FETE,GAFA,FENO,EMYR,ERAT,FYRE,GUFY;

  bool CYRE,DEME,DORA,DYRO,ESYK,ARES,DANE;
  bool ERYC_00,CERA_01,CONU_02,CAME_03,BUVA_04,AKYD_05;
  bool BYMO,AGET,BUKO;

  bool GADE,HOLY,JUPU,HOFO;

  bool CULE,DYMU,DUGE,DARE,EGOG,DOMO,DYTA,DOJU,DOVE,EXES;
  bool DOME;

  bool CYSE,BONU,ANYV,ANAN,AMOV,ASOG;

  bool CANO_00,CAGY_01,DYVE_02;
};



