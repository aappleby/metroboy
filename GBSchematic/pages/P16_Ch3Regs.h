#pragma once

struct P16_Ch3Regs {
  union { bool GUXE; bool FF1A_D7n; };
  union { bool ANUJ; };

private:

  bool GEJO,GUCY,GOVE,FUVO,GUGU,FEVO,GEKO,FASY;
  bool FABO,GARA,GYTA,GYRA,FAJU,GAZE,FURY,GOMA,FOBA;
  bool GULO,GOFY,EPYX,FAKO,GAVU;

  bool KOTA,KUHA,KYHO,JAFA,KULY;
  bool KOGA_00,JOVY_01,JAXA_02,JEFE_03,JYPO_04,JOVE_05,KANA_06,KOGU_07;

  bool DOVO,EGAD,HOXA,GUTE,HOVO;
  bool JOFO_00,KAFU_01,KESY_02,JUDE_03,JUKE_04,JEZA_05,KORA_06,KAMY_07;
  bool KUTU_00,KUPE_01,KUNU_02,KEMU_03,KYGU_04,KEPA_05,KAFO_06,KENO_07;

  bool DERY,GETO,EMUT,GAJY;

  bool FOVO,HEKY,HOTO,GORY,GAWA,HACA;
  bool HUDA,JUZO,KOPY;
  bool JEMO_00,JETY_01,JACY_02;
  bool GUNU,FUVA;

  bool HUFO_02,JURA_01,JUVY_00;
  bool JAPU_02,KEZA_01,KEJU_00;

  bool JOTU,HENU,GURO,GUZU,HAGA,HUCO,HAMU;
  bool HUKY_06,HODY_05;

  bool GAXO;

  bool FF1B_WR1;
  bool FF1B_WR2;
  bool FF1B_WR3;

  bool FF1C_D5;
  bool FF1C_D6;
  bool FF1C_D5n;
  bool FF1C_D6n;

  bool FF1D_D0;
  bool FF1D_D1;
  bool FF1D_D2;
  bool FF1D_D3;
  bool FF1D_D4;
  bool FF1D_D5;
  bool FF1D_D6;
  bool FF1D_D7;

  bool FF1E_D0;
  bool FF1E_D1;
  bool FF1E_D2;


  bool FF1A,FF1B,FF1C,FF1D,FF1E;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};

