/// plait_noparse

#pragma once

//-----------------------------------------------------------------------------

struct Gameboy;

struct Channel2 {

  /*_p14.HUDE*/ union { bool HUDE; bool APU_RESETn1; };

  //----------
  // FF16 NR21

  /*_p14.AGYN*/ bool FF16_WR;
  /*_p14.ASYP*/ bool ASYP;
  /*_p14.BENY*/ bool BENY;

  /*_p14.BACU*/ bool BACU;
  /*_p14.BUDU*/ bool BUDU;
  /*_p14.BERA*/ bool NR21_DUTY0;
  /*_p14.BAMY*/ bool NR21_DUTY1;

  /*_p14.BYGO*/ bool BYGO;
  /*_p14.CORO*/ bool CORO;

  //----------
  // FF17 NR22

  /*_p14.HYFU*/ bool NR22_ENV_TIMER0;
  /*_p14.HAVA*/ bool NR22_ENV_TIMER1;
  /*_p14.HORE*/ bool NR22_ENV_TIMER2;
  /*_p14.FORE*/ bool FF17_D3;
  /*_p14.GATA*/ bool FF17_D4;
  /*_p14.GUFE*/ bool FF17_D5;
  /*_p14.GURA*/ bool FF17_D6;
  /*_p14.GAGE*/ bool FF17_D7;

  //----------
  // FF18 NR23

  /*_p14.DOSA*/ bool NR23_WR1;
  /*_p14.EXUC*/ bool NR23_WR2;
  /*_p14.ESUR*/ bool NR23_WRn1;
  /*_p14.FYXO*/ bool NR23_WRn2;

  /*_p14.FOFE*/ bool NR23_FREQ0;
  /*_p14.FOVA*/ bool NR23_FREQ1;
  /*_p14.FEDY*/ bool NR23_FREQ2;
  /*_p14.FOME*/ bool NR23_FREQ3;
  /*_p14.FORA*/ bool NR23_FREQ4;
  /*_p14.GODA*/ bool NR23_FREQ5;
  /*_p14.GUMY*/ bool NR23_FREQ6;
  /*_p14.GUPU*/ bool NR23_FREQ7;

  /*_p14.DONE*/ bool CH2_FREQ_00;
  /*_p14.DYNU*/ bool CH2_FREQ_01;
  /*_p14.EZOF*/ bool CH2_FREQ_02;
  /*_p14.CYVO*/ bool CH2_FREQ_03;
  /*_p14.EDEP*/ bool CH2_FREQ_03b;
  /*_p14.FUXO*/ bool CH2_FREQ_04;
  /*_p14.GANO*/ bool CH2_FREQ_05;
  /*_p14.GOCA*/ bool CH2_FREQ_06;
  /*_p14.GANE*/ bool CH2_FREQ_07;
  /*_p14.GALA*/ bool CH2_FREQ_07b;
  /*_p14.HEVY*/ bool CH2_FREQ_08;
  /*_p14.HEPU*/ bool CH2_FREQ_09;
  /*_p14.HERO*/ bool CH2_FREQ_10;

  //----------
  // FF19 NR24

  /*_p14.JEFU*/ bool NR24_FREQ8;
  /*_p14.JANY*/ bool NR24_FREQ9;
  /*_p14.JUPY*/ bool NR24_FREQ10;
  /*_p14.EMER*/ bool NR24_STOP;
  /*_p14.ETAP*/ bool NR24_START;

  //----------




  /*_p14.CECY*/ bool CECY;
  /*_p14.DETA*/ bool DETA;
  /*_p14.ENUF*/ bool ENUF;
  /*_p14.EVYF*/ bool EVYF;
  /*_p14.FAPE*/ bool FAPE;
  /*_p14.FAZO*/ bool FAZO;
  /*_p14.FERY*/ bool FERY;
  /*_p14.FOGE*/ bool FOGE;
  /*_p14.FUTY*/ bool FUTY;
  /*_p14.FYRY*/ bool FYRY;
  /*_p14.GADO*/ bool GADO;
  /*_p14.GERE*/ bool GERE;
  /*_p14.GEXA*/ bool GEXA;
  /*_p14.GOJY*/ bool GOJY;
  /*_p14.GOTE*/ bool GOTE;
  /*_p14.GURE*/ bool GURE;
  /*_p14.GURU*/ bool GURU;
  /*_p14.GUZA*/ bool GUZA;
  /*_p14.HUMA*/ bool HUMA;
  /*_p14.HYPO*/ bool HYPO;
  /*_p14.JEDE*/ bool JEDE;
  /*_p14.JENU*/ bool JENU;
  /*_p14.JYBU*/ bool JYBU;
  /*_p14.KYPU*/ bool KYPU;
  /*_p14.KYSA*/ bool KYSA;

  /*_p15.DEFU*/ bool CH2_ACTIVEn;
  /*_p15.FUTE*/ bool CH2_AMP_ENn;

  /*_p15.AGET*/ bool AGET;
  /*_p15.AKYD*/ bool AKYD_05;
  /*_p15.AMOV*/ bool AMOV;
  /*_p15.ANAN*/ bool ANAN;
  /*_p15.ANYV*/ bool ANYV;
  /*_p15.ARES*/ bool ARES;
  /*_p15.ASOG*/ bool ASOG;
  /*_p15.ATEP*/ bool ATEP;
  /*_p15.AZEG*/ bool AZEG;
  /*_p15.BODO*/ bool BODO;
  /*_p15.BONU*/ bool BONU;
  /*_p15.BUFO*/ bool BUFO;
  /*_p15.BUKO*/ bool BUKO;
  /*_p15.BUTA*/ bool BUTA;
  /*_p15.BUVA*/ bool BUVA_04;
  /*_p15.BUWE*/ bool BUWE;
  /*_p15.BYHO*/ bool BYHO;
  /*_p15.BYMO*/ bool BYMO;
  /*_p15.CAGY*/ bool CAGY_01;
  /*_p15.CAMA*/ bool CAMA;
  /*_p15.CAME*/ bool CAME_03;
  /*_p15.CANO*/ bool CANO_00;
  /*_p15.CAZA*/ bool CAZA;
  /*_p15.CELO*/ bool CELO;
  /*_p15.CEMO*/ bool CEMO;
  /*_p15.CERA*/ bool CERA_01;
  /*_p15.CEXE*/ bool CEXE;
  /*_p15.COGU*/ bool COGU;
  /*_p15.CONU*/ bool CONU_02;
  /*_p15.CULE*/ bool CULE;
  /*_p15.CYRE*/ bool CYRE;
  /*_p15.CYSE*/ bool CYSE;
  /*_p15.CYWU*/ bool CYWU;
  /*_p15.DALA*/ bool DALA;
  /*_p15.DANE*/ bool DANE;
  /*_p15.DARE*/ bool DARE;
  /*_p15.DAVU*/ bool DAVU;
  /*_p15.DEME*/ bool DEME;
  /*_p15.DERA*/ bool DERA;
  /*_p15.DOCA*/ bool DOCA;
  /*_p15.DOJU*/ bool DOJU;
  /*_p15.DOME*/ bool DOME;
  /*_p15.DOMO*/ bool DOMO;
  /*_p15.DOPE*/ bool DOPE;
  /*_p15.DORA*/ bool DORA;
  /*_p15.DORY*/ bool DORY;
  /*_p15.DOVE*/ bool DOVE;
  /*_p15.DOXA*/ bool DOXA;
  /*_p15.DUGE*/ bool DUGE;
  /*_p15.DUJU*/ bool DUJU;
  /*_p15.DYMU*/ bool DYMU;
  /*_p15.DYRO*/ bool DYRO;
  /*_p15.DYTA*/ bool DYTA;
  /*_p15.DYVE*/ bool DYVE_02;
  /*_p15.EGOG*/ bool EGOG;
  /*_p15.ELOX*/ bool ELOX;
  /*_p15.EMYR*/ bool EMYR;
  /*_p15.ERAT*/ bool ERAT;
  /*_p15.EROG*/ bool EROG;
  /*_p15.ERYC*/ bool ERYC_00;
  /*_p15.ESYK*/ bool ESYK;
  /*_p15.ETUK*/ bool ETUK;
  /*_p15.ETUP*/ bool ETUP;
  /*_p15.EXES*/ bool EXES;
  /*_p15.FARU*/ bool FARU;
  /*_p15.FENA*/ bool FENA;
  /*_p15.FENO*/ bool FENO;
  /*_p15.FETE*/ bool FETE;
  /*_p15.FOMY*/ bool FOMY;
  /*_p15.FOPY*/ bool FOPY;
  /*_p15.FUJY*/ bool FUJY;
  /*_p15.FYRE*/ bool FYRE;
  /*_p15.GADE*/ bool GADE;
  /*_p15.GAFA*/ bool GAFA;
  /*_p15.GALU*/ bool GALU;
  /*_p15.GUFY*/ bool GUFY;
  /*_p15.GYKO*/ bool GYKO;
  /*_p15.GYPA*/ bool GYPA;
  /*_p15.GYRE*/ bool GYRE;
  /*_p15.HAFE*/ bool HAFE;
  /*_p15.HEPO*/ bool HEPO;
  /*_p15.HOFO*/ bool HOFO;
  /*_p15.HOLY*/ bool HOLY;
  /*_p15.HOTA*/ bool HOTA;
  /*_p15.HYLE*/ bool HYLE;
  /*_p15.HYLY*/ bool HYLY;
  /*_p15.HYPA*/ bool HYPA;
  /*_p15.JAKE*/ bool JAKE;
  /*_p15.JEME*/ bool JEME;
  /*_p15.JEVY*/ bool JEVY;
  /*_p15.JONA*/ bool JONA;
  /*_p15.JOPA*/ bool JOPA;
  /*_p15.JORE*/ bool JORE;
  /*_p15.JUPU*/ bool JUPU;
  /*_p15.JYNA*/ bool JYNA;
  /*_p15.KATY*/ bool KATY;
  /*_p15.KENE*/ bool KENE;
  /*_p15.KYLO*/ bool KYLO;
  /*_p15.KYVO*/ bool KYVO;
};

//-----------------------------------------------------------------------------
