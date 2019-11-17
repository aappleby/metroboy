#pragma once

struct Gameboy;

struct P11_Ch1Regs {

  /*p11.CEPO*/ bool CEPO; // reset
  /*p11.HATO*/ bool HATO; // reset
  /*p11.CAMY*/ bool CAMY; // reset

  //----------
  // FF10
  
  /*p11.CENU*/ bool CENU;
  /*p11.BUZE*/ bool BUZE;
  /*p11.ATYN*/ bool ATYN;
  /*p11.ASOP*/ bool ASOP;

  /*p11.BANY*/ bool NR10_SWEEP_SHIFT_0;
  /*p11.ARAX*/ bool NR10_SWEEP_SHIFT_1;
  /*p11.ANAZ*/ bool NR10_SWEEP_SHIFT_2;
  /*p11.AVAF*/ bool NR10_SWEEP_DIR;
  /*p11.ADEK*/ bool NR10_SWEEP_TIME_0;
  /*p11.BANA*/ bool NR10_SWEEP_TIME_1;
  /*p11.BOTU*/ bool NR10_SWEEP_TIME_2;

  //----------
  // FF11

  /*p11.COVU*/ bool FF11_WR;
  /*p11.DAFO*/ bool FF11_WRn;
  /*p11.BUWA*/ bool CPU_RDa;
  /*p11.BEXU*/ bool FF11_RD;

  /*p11.CENA*/ bool CH1_DUTY_0;
  /*p11.DYCA*/ bool CH1_DUTY_1;

  //----------
  // FF12

  /*p11.KAGY*/ bool KAGY;
  /*p11.HAFU*/ bool HAFU;
  /*p11.KYGY*/ bool KYGY;
  /*p11.GAXU*/ bool GAXU;
  /*p11.HAXE*/ bool HAXE;
  /*p11.HAMY*/ bool HAMY;
  /*p11.GAGO*/ bool GAGO;
  /*p11.HOCU*/ bool HOCU;

  /*p11.JUSA*/ bool CH1_SWEEP_0;
  /*p11.JUZY*/ bool CH1_SWEEP_1;
  /*p11.JOMA*/ bool CH1_SWEEP_2;
  /*p11.JAFY*/ bool CH1_ENV_DIR;
  /*p11.JATY*/ bool CH1_VOL_0;
  /*p11.JAXO*/ bool CH1_VOL_1;
  /*p11.JENA*/ bool CH1_VOL_2;
  /*p11.JOPU*/ bool CH1_VOL_3;

  //----------
  // FF13 NR13 - ch1 freq

  /*p11.CEGE*/ bool CPU_RDb;
  /*p11.DAXA*/ bool CPU_RDnb;
  /*p11.CACA*/ bool ADDR_FF13n;
  /*p11.DYPU*/ bool FF13_RDn;
  /*p11.EVAJ*/ bool FF13_RDa;

  /*p11.FULO*/ bool CH1_FREQ_CLKn;
  /*p11.GEKU*/ bool CH1_FREQ_CLKa;
  /*p11.KYPE*/ bool CH1_FREQ_CLKb;
  /*p11.DERU*/ bool CH1_FREQ_CLKc;

  /*p11.EPYK*/ bool CH1_FREQ_RSTn;
  /*p11.FUME*/ bool CH1_FREQ_RSTa;
  /*p11.DEGA*/ bool CH1_FREQ_RSTb;
  /*p11.DAKO*/ bool CH1_FREQ_RSTc;

  /*p11.GAXE*/ bool CH1_FREQ_00;
  /*p11.HYFE*/ bool CH1_FREQ_01;
  /*p11.JYTY*/ bool CH1_FREQ_02;
  /*p11.KYNA*/ bool CH1_FREQ_03;
  /*p11.JEMA*/ bool CH1_FREQ_04;
  /*p11.HYKE*/ bool CH1_FREQ_05;
  /*p11.FEVA*/ bool CH1_FREQ_06;
  /*p11.EKOV*/ bool CH1_FREQ_07;
  /*p11.EMUS*/ bool CH1_FREQ_08;
  /*p11.EVAK*/ bool CH1_FREQ_09;
  /*p11.COPU*/ bool CH1_FREQ_10;

  //----------
  // FF14

  /*p11.BAGE*/ bool FF14_WRn;
  /*p11.BOKO*/ bool NR14_STOP;
  /*p11.CURE*/ bool ADDR_FF14n;
  /*p11.DUPY*/ bool FF14_RDa;
  /*p11.BUDA*/ bool CPU_RDna;
  /*p11.BALE*/ bool FF14_RDb;

};



