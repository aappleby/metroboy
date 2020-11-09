#pragma once

//-----------------------------------------------------------------------------

struct Gameboy;

struct Channel2 {

  /*p14.HUDE*/ union { bool HUDE; bool APU_RESETn1; };

  //----------
  // FF16 NR21

  /*p14.AGYN*/ bool FF16_WR;
  /*p14.ASYP*/ bool ASYP;
  /*p14.BENY*/ bool BENY;

  /*p14.BACU*/ bool BACU;
  /*p14.BUDU*/ bool BUDU;
  /*p14.BERA*/ bool NR21_DUTY0;
  /*p14.BAMY*/ bool NR21_DUTY1;

  /*p14.BYGO*/ bool BYGO;
  /*p14.CORO*/ bool CORO;

  //----------
  // FF17 NR22

  /*p14.HYFU*/ bool NR22_ENV_TIMER0;
  /*p14.HAVA*/ bool NR22_ENV_TIMER1;
  /*p14.HORE*/ bool NR22_ENV_TIMER2;
  /*p14.FORE*/ bool FF17_D3;
  /*p14.GATA*/ bool FF17_D4;
  /*p14.GUFE*/ bool FF17_D5;
  /*p14.GURA*/ bool FF17_D6;
  /*p14.GAGE*/ bool FF17_D7;

  //----------
  // FF18 NR23

  /*p14.DOSA*/ bool NR23_WR1;
  /*p14.EXUC*/ bool NR23_WR2;
  /*p14.ESUR*/ bool NR23_WRn1;
  /*p14.FYXO*/ bool NR23_WRn2;

  /*p14.FOFE*/ bool NR23_FREQ0;
  /*p14.FOVA*/ bool NR23_FREQ1;
  /*p14.FEDY*/ bool NR23_FREQ2;
  /*p14.FOME*/ bool NR23_FREQ3;
  /*p14.FORA*/ bool NR23_FREQ4;
  /*p14.GODA*/ bool NR23_FREQ5;
  /*p14.GUMY*/ bool NR23_FREQ6;
  /*p14.GUPU*/ bool NR23_FREQ7;

  /*p14.DONE*/ bool CH2_FREQ_00;
  /*p14.DYNU*/ bool CH2_FREQ_01;
  /*p14.EZOF*/ bool CH2_FREQ_02;
  /*p14.CYVO*/ bool CH2_FREQ_03;
  /*p14.EDEP*/ bool CH2_FREQ_03b;
  /*p14.FUXO*/ bool CH2_FREQ_04;
  /*p14.GANO*/ bool CH2_FREQ_05;
  /*p14.GOCA*/ bool CH2_FREQ_06;
  /*p14.GANE*/ bool CH2_FREQ_07;
  /*p14.GALA*/ bool CH2_FREQ_07b;
  /*p14.HEVY*/ bool CH2_FREQ_08;
  /*p14.HEPU*/ bool CH2_FREQ_09;
  /*p14.HERO*/ bool CH2_FREQ_10;

  //----------
  // FF19 NR24

  /*p14.JEFU*/ bool NR24_FREQ8;
  /*p14.JANY*/ bool NR24_FREQ9;
  /*p14.JUPY*/ bool NR24_FREQ10;
  /*p14.EMER*/ bool NR24_STOP;
  /*p14.ETAP*/ bool NR24_START;

  //----------




  /*p14.CECY*/ bool CECY;
  /*p14.DETA*/ bool DETA;
  /*p14.ENUF*/ bool ENUF;
  /*p14.EVYF*/ bool EVYF;
  /*p14.FAPE*/ bool FAPE;
  /*p14.FAZO*/ bool FAZO;
  /*p14.FERY*/ bool FERY;
  /*p14.FOGE*/ bool FOGE;
  /*p14.FUTY*/ bool FUTY;
  /*p14.FYRY*/ bool FYRY;
  /*p14.GADO*/ bool GADO;
  /*p14.GERE*/ bool GERE;
  /*p14.GEXA*/ bool GEXA;
  /*p14.GOJY*/ bool GOJY;
  /*p14.GOTE*/ bool GOTE;
  /*p14.GURE*/ bool GURE;
  /*p14.GURU*/ bool GURU;
  /*p14.GUZA*/ bool GUZA;
  /*p14.HUMA*/ bool HUMA;
  /*p14.HYPO*/ bool HYPO;
  /*p14.JEDE*/ bool JEDE;
  /*p14.JENU*/ bool JENU;
  /*p14.JYBU*/ bool JYBU;
  /*p14.KYPU*/ bool KYPU;
  /*p14.KYSA*/ bool KYSA;

  /*p15.DEFU*/ bool CH2_ACTIVEn;
  /*p15.FUTE*/ bool CH2_AMP_ENn;

  /*p15.AGET*/ bool AGET;
  /*p15.AKYD*/ bool AKYD_05;
  /*p15.AMOV*/ bool AMOV;
  /*p15.ANAN*/ bool ANAN;
  /*p15.ANYV*/ bool ANYV;
  /*p15.ARES*/ bool ARES;
  /*p15.ASOG*/ bool ASOG;
  /*p15.ATEP*/ bool ATEP;
  /*p15.AZEG*/ bool AZEG;
  /*p15.BODO*/ bool BODO;
  /*p15.BONU*/ bool BONU;
  /*p15.BUFO*/ bool BUFO;
  /*p15.BUKO*/ bool BUKO;
  /*p15.BUTA*/ bool BUTA;
  /*p15.BUVA*/ bool BUVA_04;
  /*p15.BUWE*/ bool BUWE;
  /*p15.BYHO*/ bool BYHO;
  /*p15.BYMO*/ bool BYMO;
  /*p15.CAGY*/ bool CAGY_01;
  /*p15.CAMA*/ bool CAMA;
  /*p15.CAME*/ bool CAME_03;
  /*p15.CANO*/ bool CANO_00;
  /*p15.CAZA*/ bool CAZA;
  /*p15.CELO*/ bool CELO;
  /*p15.CEMO*/ bool CEMO;
  /*p15.CERA*/ bool CERA_01;
  /*p15.CEXE*/ bool CEXE;
  /*p15.COGU*/ bool COGU;
  /*p15.CONU*/ bool CONU_02;
  /*p15.CULE*/ bool CULE;
  /*p15.CYRE*/ bool CYRE;
  /*p15.CYSE*/ bool CYSE;
  /*p15.CYWU*/ bool CYWU;
  /*p15.DALA*/ bool DALA;
  /*p15.DANE*/ bool DANE;
  /*p15.DARE*/ bool DARE;
  /*p15.DAVU*/ bool DAVU;
  /*p15.DEME*/ bool DEME;
  /*p15.DERA*/ bool DERA;
  /*p15.DOCA*/ bool DOCA;
  /*p15.DOJU*/ bool DOJU;
  /*p15.DOME*/ bool DOME;
  /*p15.DOMO*/ bool DOMO;
  /*p15.DOPE*/ bool DOPE;
  /*p15.DORA*/ bool DORA;
  /*p15.DORY*/ bool DORY;
  /*p15.DOVE*/ bool DOVE;
  /*p15.DOXA*/ bool DOXA;
  /*p15.DUGE*/ bool DUGE;
  /*p15.DUJU*/ bool DUJU;
  /*p15.DYMU*/ bool DYMU;
  /*p15.DYRO*/ bool DYRO;
  /*p15.DYTA*/ bool DYTA;
  /*p15.DYVE*/ bool DYVE_02;
  /*p15.EGOG*/ bool EGOG;
  /*p15.ELOX*/ bool ELOX;
  /*p15.EMYR*/ bool EMYR;
  /*p15.ERAT*/ bool ERAT;
  /*p15.EROG*/ bool EROG;
  /*p15.ERYC*/ bool ERYC_00;
  /*p15.ESYK*/ bool ESYK;
  /*p15.ETUK*/ bool ETUK;
  /*p15.ETUP*/ bool ETUP;
  /*p15.EXES*/ bool EXES;
  /*p15.FARU*/ bool FARU;
  /*p15.FENA*/ bool FENA;
  /*p15.FENO*/ bool FENO;
  /*p15.FETE*/ bool FETE;
  /*p15.FOMY*/ bool FOMY;
  /*p15.FOPY*/ bool FOPY;
  /*p15.FUJY*/ bool FUJY;
  /*p15.FYRE*/ bool FYRE;
  /*p15.GADE*/ bool GADE;
  /*p15.GAFA*/ bool GAFA;
  /*p15.GALU*/ bool GALU;
  /*p15.GUFY*/ bool GUFY;
  /*p15.GYKO*/ bool GYKO;
  /*p15.GYPA*/ bool GYPA;
  /*p15.GYRE*/ bool GYRE;
  /*p15.HAFE*/ bool HAFE;
  /*p15.HEPO*/ bool HEPO;
  /*p15.HOFO*/ bool HOFO;
  /*p15.HOLY*/ bool HOLY;
  /*p15.HOTA*/ bool HOTA;
  /*p15.HYLE*/ bool HYLE;
  /*p15.HYLY*/ bool HYLY;
  /*p15.HYPA*/ bool HYPA;
  /*p15.JAKE*/ bool JAKE;
  /*p15.JEME*/ bool JEME;
  /*p15.JEVY*/ bool JEVY;
  /*p15.JONA*/ bool JONA;
  /*p15.JOPA*/ bool JOPA;
  /*p15.JORE*/ bool JORE;
  /*p15.JUPU*/ bool JUPU;
  /*p15.JYNA*/ bool JYNA;
  /*p15.KATY*/ bool KATY;
  /*p15.KENE*/ bool KENE;
  /*p15.KYLO*/ bool KYLO;
  /*p15.KYVO*/ bool KYVO;
};

//-----------------------------------------------------------------------------
