#pragma once

struct Channel3 {

  // resets
  /*p16.GOVE*/ bool APU_RESETn;
  /*p16.GAZE*/ bool APU_RESETo;
  /*p16.GOMA*/ bool APU_RESETp;
  /*p16.KUHA*/ bool APU_RESETq;
  /*p16.HEKY*/ bool APU_RESETr;
  /*p16.KOPY*/ bool APU_RESETs;
  /*p16.GURO*/ bool APU_RESETt;
  /*p17.BAMA*/ bool APU_RESETu;
  /*p17.ACOR*/ bool APU_RESETv;
  /*p18.CALU*/ bool APU_RESETw;

  //----------
  // FF1A NR30

  /*p16.GUXE*/ bool CH3_AMP_ENna;

  //----------
  // FF1B NR31

  //----------
  // FF1C NR32

  /*p16.HODY*/ bool NR32_VOL0;
  /*p16.HUKY*/ bool NR32_VOL1;

  //----------
  // FF1D NR33

  /*p16.KOTA*/ bool FF1D_WRn;
  /*p16.KYHO*/ bool FF1D_WRa;
  /*p16.JAFA*/ bool FF1D_WRo;
  /*p16.KULY*/ bool FF1D_WRb;

  /*p16.KOGA*/ bool NR33_FREQ0;
  /*p16.JOVY*/ bool NR33_FREQ1;
  /*p16.JAXA*/ bool NR33_FREQ2;
  /*p16.JEFE*/ bool NR33_FREQ3;
  /*p16.JYPO*/ bool NR33_FREQ4;
  /*p16.JOVE*/ bool NR33_FREQ5;
  /*p16.KANA*/ bool NR33_FREQ6;
  /*p16.KOGU*/ bool NR33_FREQ7;

  /*p16.DOVO*/ bool DOVO;
  /*p16.EGAD*/ bool EGAD;
  /*p16.HOXA*/ bool HOXA;
  /*p16.GUTE*/ bool GUTE;
  /*p16.HOVO*/ bool HOVO;

  //----------
  // FF1E NR34

  /*p16.JEMO*/ bool NR34_FREQ8;
  /*p16.JETY*/ bool NR34_FREQ9;
  /*p16.JACY*/ bool NR34_FREQ10;
  /*p16.HOTO*/ bool NR34_STOP;

  //----------

  /*p16.GEJO*/ bool FF1A_WR;
  /*p16.GUCY*/ bool FF1A_WRn;
  
  /*p16.FASY*/ bool FASY;
  /*p16.FEVO*/ bool CH3_AMP_ENa;
  /*p16.FUVO*/ bool FUVO;
  /*p16.FAJU*/ bool FAJU;
  /*p16.GUGU*/ bool GUGU;

  /*p16.FABO*/ bool CLK_xxCDxxGH;
  /*p16.GAVU*/ bool CH3_RESTART;
  /*p16.EPYX*/ bool FF1E_WRn;

  /*p16.GARA*/ bool GARA;
  /*p16.GYTA*/ bool GYTA;
  /*p16.GYRA*/ bool GYRA;
  /*p16.FURY*/ bool FURY;
  /*p16.FOBA*/ bool FOBA;
  /*p16.GULO*/ bool GULO;
  /*p16.GOFY*/ bool GOFY;
  /*p16.FAKO*/ bool FAKO;



  /*p16.DERY*/ bool DERY;
  /*p16.GETO*/ bool GETO;
  /*p16.EMUT*/ bool EMUT;
  /*p16.GAJY*/ bool GAJY;

  /*p16.FOVO*/ bool FOVO;
  /*p16.GORY*/ bool GORY;
  /*p16.GAWA*/ bool GAWA;
  /*p16.HACA*/ bool HACA;
  /*p16.HUDA*/ bool HUDA;
  /*p16.JUZO*/ bool JUZO;
  /*p16.GUNU*/ bool GUNU;
  /*p16.FUVA*/ bool FUVA;

  /*p16.JOTU*/ bool JOTU;
  /*p16.HENU*/ bool HENU;
  /*p16.GUZU*/ bool GUZU;
  /*p16.HAGA*/ bool HAGA;
  /*p16.HUCO*/ bool HUCO;
  /*p16.HAMU*/ bool HAMU;

  /*p16.GAXO*/ bool GAXO;

  /*p17.BYZA*/ bool WAVE_RAM_WR;
  /*p17.CAZU*/ bool WAVE_RAM_RDn;
  /*p17.BUTU*/ bool BUTU;
  /*p17.ALER*/ bool WAVE_RAM_CTRL1;
  /*p17.AMYT*/ bool WAVE_RAM_WRn;
  /*p17.DATE*/ bool WAVE_PLAY_D0;
  /*p17.DAZY*/ bool WAVE_PLAY_D1;
  /*p17.COPO*/ bool WAVE_PLAY_D3;
  /*p17.CUZO*/ bool WAVE_PLAY_D2;

  /*p17.BOKE*/ bool BOKE;
  /*p17.BENA*/ bool BENA;
  /*p17.ARUC*/ bool ARUC;
  /*p17.COZY*/ bool COZY;
  /*p17.ABUR*/ bool ABUR;
  /*p17.BORY*/ bool BORY;
  /*p17.BETA*/ bool BETA;
  /*p17.AZOR*/ bool AZOR;
  /*p17.BUKU*/ bool BUKU;
  /*p17.BUSA*/ bool BUSA;
  /*p17.BANO*/ bool BANO;
  /*p17.AZUS*/ bool AZUS;
  /*p17.AZET*/ bool AZET;
  /*p17.BOXO*/ bool BOXO;
  /*p17.BORU*/ bool BORU;
  /*p17.ATUR*/ bool ATUR;
  /*p17.BEKA*/ bool BEKA;
  /*p17.COJU*/ bool COJU;
  /*p17.BAJA*/ bool BAJA;
  /*p17.BUFE*/ bool BUFE;

  /*p17.CUGO*/ bool WAVE_D0n;
  /*p17.CEGU*/ bool WAVE_D1n;
  /*p17.ATEC*/ bool WAVE_D2n;
  /*p17.ADOK*/ bool WAVE_D3n;
  /*p17.BACA*/ bool WAVE_D4n;
  /*p17.BERO*/ bool WAVE_D5n;
  /*p17.CUTO*/ bool WAVE_D6n;
  /*p17.AKAF*/ bool WAVE_D7n;

  /*p17.DUGU*/ bool WAVE_D0;
  /*p17.DESY*/ bool WAVE_D1;
  /*p17.BATY*/ bool WAVE_D2;
  /*p17.BADE*/ bool WAVE_D3;
  /*p17.BUNE*/ bool WAVE_D4;
  /*p17.BAVA*/ bool WAVE_D5;
  /*p17.DESA*/ bool WAVE_D6;
  /*p17.BEZU*/ bool WAVE_D7;


  /*p17.CYFO*/ bool SAMPLE_0;
  /*p17.CESY*/ bool SAMPLE_1;
  /*p17.BUDY*/ bool SAMPLE_2;
  /*p17.BEGU*/ bool SAMPLE_3;
  /*p17.CUVO*/ bool SAMPLE_4;
  /*p17.CEVO*/ bool SAMPLE_5;
  /*p17.BORA*/ bool SAMPLE_6;
  /*p17.BEPA*/ bool SAMPLE_7;

  /*p18.COKA*/ union { bool CH3_ACTIVE; };
  /*p18.ERED*/ union { bool CH3_ACTIVEn; };
  /*p18.BOLE*/ union { bool WAVE_A0; };
  /*p18.AGYL*/ union { bool WAVE_A1; };
  /*p18.AFUM*/ union { bool WAVE_A2; };
  /*p18.AXOL*/ union { bool WAVE_A3; };

  /*p18.JECO*/ bool JECO;
  /*p18.HONY*/ bool HONY;
  /*p18.GENO*/ bool GENO;

  /*p18.BARY*/ union { bool WAVE_DAC0; };
  /*p18.BYKA*/ union { bool WAVE_DAC1; };
  /*p18.BOPA*/ union { bool WAVE_DAC2; };
  /*p18.BELY*/ union { bool WAVE_DAC3; };
  /*p18.GASE*/ union { bool GASE; };

  /*p18.DERO*/ bool WAVE_CLK;
  /*p18.EFAR*/ bool WAVE_IDX0;
  /*p18.ERUS*/ bool WAVE_IDX1;
  /*p18.EFUZ*/ bool WAVE_IDX2;
  /*p18.EXEL*/ bool WAVE_IDX3;
  /*p18.EFAL*/ bool WAVE_IDX4;
  /*p18.FETY*/ bool WAVE_LOOP;
  /*p18.ETAN*/ bool WAVE_RST;

  /*p18.JYFO*/ bool JYFO;
  /*p18.HUNO*/ bool HUNO;
  /*p18.HEMA*/ bool HEMA;
  /*p18.HUPA*/ bool HUPA;
  /*p18.GAFU*/ bool GAFU;
  /*p18.HEFO*/ bool HEFO;
  /*p18.JUTY*/ bool JUTY;
  /*p18.HERA*/ bool HERA;
  /*p18.KYKO*/ bool KYKO;
  /*p18.KASO*/ bool KASO;
  /*p18.KYRU*/ bool KYRU;
  /*p18.JERA*/ bool JERA;
  /*p18.KESE*/ bool KESE;

  /*p18.KUTU*/ bool KUTU_00;
  /*p18.KUPE*/ bool KUPE_01;
  /*p18.KUNU*/ bool KUNU_02;
  /*p18.KEMU*/ bool KEMU_03;
  /*p18.KYGU*/ bool KYGU_04;
  /*p18.KEPA*/ bool KEPA_05;
  /*p18.KAFO*/ bool KAFO_06;
  /*p18.KENO*/ bool KENO_07;
  /*p18.KEJU*/ bool KEJU_08;
  /*p18.KEZA*/ bool KEZA_09;
  /*p18.JAPU*/ bool JAPU_10;

  /*p18.FOTO*/ bool FOTO;
  /*p18.GYRY*/ bool GYRY;
  /*p18.GEDO*/ bool GEDO;
  /*p18.FYGO*/ bool FYGO;
  /*p18.FOZU*/ bool FOZU;
  /*p18.EZAS*/ bool EZAS;
  /*p18.DORU*/ bool DORU;
  /*p18.DAVO*/ bool DAVO;

  /*p18.GOKA*/ bool GOKA;
  /*p18.GEMY*/ bool GEMY;
  /*p18.GEGE*/ bool GEGE;
  /*p18.EZAG*/ bool EZAG;
  /*p18.EVUG*/ bool EVUG;
  /*p18.DOKY*/ bool DOKY;
  /*p18.DORE*/ bool DORE;
  /*p18.GUDA*/ bool GUDA;
  /*p18.FEXU*/ bool FEXU;
  /*p18.GEPY*/ bool GEPY;
  /*p18.GENU*/ bool GENU;
  /*p18.FALU*/ bool FALU;

  /*p18.BENO*/ bool BENO;

  /*p18.ATOK*/ bool WAVE_RAM_CTRL3;

  /*p18.GEVO*/ bool GEVO_00;
  /*p18.FORY*/ bool FORY_01;
  /*p18.GATU*/ bool GATU_02;
  /*p18.GAPO*/ bool GAPO_03;
  /*p18.GEMO*/ bool GEMO_04;
  /*p18.FORO*/ bool FORO_05;
  /*p18.FAVE*/ bool FAVE_06;
  /*p18.FYRU*/ bool FYRU_07;
};

