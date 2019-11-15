#pragma once

struct Gameboy;

struct P11_Ch1Regs {

  /*p11.BANY*/ bool CH1_SWEEP_SHIFT_0;
  /*p11.ARAX*/ bool CH1_SWEEP_SHIFT_1;
  /*p11.ANAZ*/ bool CH1_SWEEP_SHIFT_2;
  /*p11.AVAF*/ bool CH1_SWEEP_DIR;
  /*p11.ADEK*/ bool CH1_SWEEP_TIME_0;
  /*p11.BANA*/ bool CH1_SWEEP_TIME_1;
  /*p11.BOTU*/ bool CH1_SWEEP_TIME_2;
  /*p11.AMYD*/ bool FF10_D0;
  /*p11.ATAX*/ bool FF10_D1;
  /*p11.AZYP*/ bool FF10_D2;
  /*p11.AFOX*/ bool FF10_D3;
  /*p11.AVEK*/ bool FF10_D4;
  /*p11.AKUX*/ bool FF10_D5;
  /*p11.AWOS*/ bool FF10_D6;

  /*p11.COVU*/ bool FF11_WR;
  /*p11.DAFO*/ bool FF11_WRn;
  /*p11.BUWA*/ bool CPU_RD2;
  /*p11.BEXU*/ bool FF11_RD;

  /*p11.CENA*/ bool CH1_DUTY_0;
  /*p11.DYCA*/ bool CH1_DUTY_1;
  /*p11.BOWO*/ bool FF11_D6;
  /*p11.CUDA*/ bool FF11_D7;

  /*p11.BAGE*/ bool BAGE;
  /*p11.BALE*/ bool FF14_RD;
  /*p11.BOKO*/ bool CH1_USE_LENGTH;
  /*p11.BYTU*/ bool FF14_D6;

  bool CEPO;

  union { bool JUSA; bool FF12_D0; };
  union { bool JUZY; bool FF12_D1; };
  union { bool JOMA; bool FF12_D2; };
  union { bool JAFY; bool FF12_D3; bool CH1_ENV_DIR; };
  union { bool JATY; bool FF12_D4; };
  union { bool JAXO; bool FF12_D5; };
  union { bool JENA; bool FF12_D6; };
  union { bool JOPU; bool FF12_D7; };

  // FF13 NR13 - ch1 freq
  union { bool GAXE; bool CH1_FREQ_00; };
  union { bool HYFE; bool CH1_FREQ_01; };
  union { bool JYTY; bool CH1_FREQ_02; };
  union { bool KYNA; bool CH1_FREQ_03; };
  union { bool JEMA; bool CH1_FREQ_04; };
  union { bool HYKE; bool CH1_FREQ_05; };
  union { bool FEVA; bool CH1_FREQ_06; };
  union { bool EKOV; bool CH1_FREQ_07; };
  union { bool EMUS; bool CH1_FREQ_08; };
  union { bool EVAK; bool CH1_FREQ_09; };
  union { bool COPU; bool CH1_FREQ_10; };

  bool CENU,BUZE,ATYN,ASOP;

  bool GAXU,HATO,GAGO,KAGY,HOCU;
  bool JYSE,HEVE,HEWA,HOWU,HONO;
  bool HAXE,HAFU,HAMY,KYGY;
  bool JYNE,JACA,JOKU;

  bool FULO,EPYK,GEKU,FUME;
  bool DEGA,KYPE,DAKO,DERU;
  bool FORU,GEFU,KYVU,KUMO,KARY,GODE,GOJE,FOZE;
  bool CEGE,DAXA,CACA,DYPU,EVAJ;
  bool CURE,DUPY,DOPA,DEMU,DEXO;

  bool CAMY;

  union { /*bool BUDA;*/ bool CPU_RDn; };
};



