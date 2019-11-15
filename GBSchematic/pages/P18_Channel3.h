#pragma once

struct P18_Channel3 {

  union { bool COKA; bool CH3_ACTIVE; };
  union { bool ERED; bool CH3_ACTIVEn; };

  union { bool BOLE; bool WAVE_A0; };
  union { bool AGYL; bool WAVE_A1; };
  union { bool AFUM; bool WAVE_A2; };
  union { bool AXOL; bool WAVE_A3; };

  union { bool BARY; bool WAVE_DAC_D0; };
  union { bool BYKA; bool WAVE_DAC_D1; };
  union { bool BOPA; bool WAVE_DAC_D2; };
  union { bool BELY; bool WAVE_DAC_D3; };

  union { bool GASE; };

  /*p18.DERO*/ bool WAVE_CLK;
  /*p18.EFAR*/ bool WAVE_IDX0;
  /*p18.ERUS*/ bool WAVE_IDX1;
  /*p18.EFUZ*/ bool WAVE_IDX2;
  /*p18.EXEL*/ bool WAVE_IDX3;
  /*p18.EFAL*/ bool WAVE_IDX4;
  /*p18.FETY*/ bool WAVE_LOOP;
  /*p18.ETAN*/ bool WAVE_RST;

  /*p18.FAPY*/ bool WAVE_IDX_D0;
  /*p18.FARO*/ bool WAVE_IDX_D1;
  /*p18.FOTE*/ bool WAVE_IDX_D2;
  /*p18.FANA*/ bool WAVE_IDX_D3;
  /*p18.FERA*/ bool WAVE_IDX_D4;

//private:

  bool JYFO,HUNO,HEMA,HUPA,GAFU,HEFO,JUTY;
  bool HERA,KYKO,KASO,KYRU,JERA,KESE;

  bool KUTU_00,KUPE_01,KUNU_02,KEMU_03,KYGU_04,KEPA_05,KAFO_06,KENO_07,KEJU_08,KEZA_09,JAPU_10;

  bool FOTO,GYRY;
  bool JECO,HONY,GENO;
  bool GEDO,FYGO,FOZU,EZAS,CALU,DORU,DAVO;

  bool GOKA,GEMY,GEGE,EZAG,EVUG,DOKY,DORE;
  bool GUDA,FEXU,GEPY,GENU,FALU;

  bool BENO;

  /*p18.ATOK*/ bool WAVE_RAM_CTRL3;

  bool GEVO_00;
  bool FORY_01;
  bool GATU_02;
  bool GAPO_03;
  bool GEMO_04;
  bool FORO_05;
  bool FAVE_06;
  bool FYRU_07;

  //----------

  bool FF1A_D7n;     // P16.GUXE

  bool FF1B_WR1;     // P16.EMUT
  bool FF1B_WR2;     // P16.GAJY
  bool FF1B_WR3;     // P16.GETO

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

  bool FF1E_D6;
  bool FF1E_D6n;

  bool FF1C;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};

