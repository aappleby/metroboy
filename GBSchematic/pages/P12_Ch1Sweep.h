#pragma once

struct Gameboy;

struct P12_Ch1Sweep {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  /*p12.DEPU*/ bool FREQ_WRa;
  /*p12.DEBY*/ bool FREQ_WRb;
  /*p12.DYLA*/ bool FREQ_WRc;

  // presumably this is the freq out
  /*p12.AXAN*/ bool FREQ_SUM_OUT_10;
  /*p12.EVAB*/ bool FREQ_SUM_OUT_09;
  /*p12.DYGY*/ bool FREQ_SUM_OUT_08;
  /*p12.HOPO*/ bool FREQ_SUM_OUT_07;
  /*p12.HYXU*/ bool FREQ_SUM_OUT_06;
  /*p12.HOLU*/ bool FREQ_SUM_OUT_05;
  /*p12.FELY*/ bool FREQ_SUM_OUT_04;
  /*p12.EDUL*/ bool FREQ_SUM_OUT_03;
  /*p12.HAVO*/ bool FREQ_SUM_OUT_02;
  /*p12.JYKA*/ bool FREQ_SUM_OUT_01;
  /*p12.HYKA*/ bool FREQ_SUM_OUT_00;

  /*p12.BOJO*/ bool FREQ_SUM_OUT_10n;
  /*p12.APAT*/ bool FREQ_SUM_OUT_09n;
  /*p12.BYRU*/ bool FREQ_SUM_OUT_08n;
  /*p12.CYKY*/ bool FREQ_SUM_OUT_07n;
  /*p12.DEBO*/ bool FREQ_SUM_OUT_06n;
  /*p12.FOHY*/ bool FREQ_SUM_OUT_05n;
  /*p12.KOVO*/ bool FREQ_SUM_OUT_04n;
  /*p12.KEKE*/ bool FREQ_SUM_OUT_03n;
  /*p12.HUNY*/ bool FREQ_SUM_OUT_02n;
  /*p12.HOXE*/ bool FREQ_SUM_OUT_01n;
  /*p12.JUTA*/ bool FREQ_SUM_OUT_00n;

  /*p12.BYFU*/ bool FREQ_D10n;
  /*p12.BOFU*/ bool FREQ_D09n;
  /*p12.BYSU*/ bool FREQ_D08n;
  /*p12.DULO*/ bool FREQ_D07n;
  /*p12.DYLU*/ bool FREQ_D06n;
  /*p12.JULO*/ bool FREQ_D05n;
  /*p12.KOPU*/ bool FREQ_D04n;
  /*p12.ETUV*/ bool FREQ_D03n;
  /*p12.FULE*/ bool FREQ_D02n;
  /*p12.GULU*/ bool FREQ_D01n;
  /*p12.DEKE*/ bool FREQ_D00n;

  /*p12.AFEG*/ bool FREQ_SUM_SET_10;
  /*p12.BUDO*/ bool FREQ_SUM_SET_09;
  /*p12.BUGU*/ bool FREQ_SUM_SET_08;
  /*p12.ETOL*/ bool FREQ_SUM_SET_07;
  /*p12.ELER*/ bool FREQ_SUM_SET_06;
  /*p12.KYPA*/ bool FREQ_SUM_SET_05;
  /*p12.KOVU*/ bool FREQ_SUM_SET_04;
  /*p12.GOPE*/ bool FREQ_SUM_SET_03;
  /*p12.GOLO*/ bool FREQ_SUM_SET_02;
  /*p12.GETA*/ bool FREQ_SUM_SET_01;
  /*p12.GYLU*/ bool FREQ_SUM_SET_00;

  /*p12.APAJ*/ bool FREQ_SUM_RST_10;
  /*p12.BOVU*/ bool FREQ_SUM_RST_09;
  /*p12.BOXU*/ bool FREQ_SUM_RST_08;
  /*p12.ESEL*/ bool FREQ_SUM_RST_07;
  /*p12.ELUF*/ bool FREQ_SUM_RST_06;
  /*p12.KAJU*/ bool FREQ_SUM_RST_05;
  /*p12.KAPO*/ bool FREQ_SUM_RST_04;
  /*p12.GAMO*/ bool FREQ_SUM_RST_03;
  /*p12.GYFU*/ bool FREQ_SUM_RST_02;
  /*p12.GATO*/ bool FREQ_SUM_RST_01;
  /*p12.EFOR*/ bool FREQ_SUM_RST_00;

  /*p12.DOLY*/ bool FREQ_SUM_L_10;
  /*p12.DOFY*/ bool FREQ_SUM_L_09;
  /*p12.DEXE*/ bool FREQ_SUM_L_08;
  /*p12.DELE*/ bool FREQ_SUM_L_07;
  /*p12.EXAP*/ bool FREQ_SUM_L_06;
  /*p12.FAXO*/ bool FREQ_SUM_L_05;
  /*p12.GYME*/ bool FREQ_SUM_L_04;
  /*p12.JYME*/ bool FREQ_SUM_L_03;
  /*p12.KARE*/ bool FREQ_SUM_L_02;
  /*p12.JODE*/ bool FREQ_SUM_L_01;
  /*p12.GALO*/ bool FREQ_SUM_L_00;

  /*p12.DEVA*/ bool FREQ_SUM_R_10;
  /*p12.ETER*/ bool FREQ_SUM_R_09;
  /*p12.DEFA*/ bool FREQ_SUM_R_08;
  /*p12.EDOK*/ bool FREQ_SUM_R_07;
  /*p12.EPYR*/ bool FREQ_SUM_R_06;
  /*p12.GELE*/ bool FREQ_SUM_R_05;
  /*p12.JETE*/ bool FREQ_SUM_R_04;
  /*p12.JAPE*/ bool FREQ_SUM_R_03;
  /*p12.HELE*/ bool FREQ_SUM_R_02;
  /*p12.HOPA*/ bool FREQ_SUM_R_01;
  /*p12.HORA*/ bool FREQ_SUM_R_00;

  bool ATYS;

  /*p12.BEKU*/ bool FREQ_SHIFT_10;
  /*p12.AGEZ*/ bool FREQ_SHIFT_09;
  /*p12.ELUX*/ bool FREQ_SHIFT_08;
  /*p12.EXAC*/ bool FREQ_SHIFT_07;
  /*p12.FEDO*/ bool FREQ_SHIFT_06;
  /*p12.FUDE*/ bool FREQ_SHIFT_05;
  /*p12.JOTA*/ bool FREQ_SHIFT_04;
  /*p12.JOLU*/ bool FREQ_SHIFT_03;
  /*p12.GOGA*/ bool FREQ_SHIFT_02;
  /*p12.JEFA*/ bool FREQ_SHIFT_01;
  /*p12.FABU*/ bool FREQ_SHIFT_00;

  /*p12.CULU*/ bool FREQ_SHIFT_FLIP_10;
  /*p12.DOZY*/ bool FREQ_SHIFT_FLIP_09;
  /*p12.CALE*/ bool FREQ_SHIFT_FLIP_08;
  /*p12.DYME*/ bool FREQ_SHIFT_FLIP_07;
  /*p12.FURE*/ bool FREQ_SHIFT_FLIP_06;
  /*p12.GOLY*/ bool FREQ_SHIFT_FLIP_05;
  /*p12.KEFE*/ bool FREQ_SHIFT_FLIP_04;
  /*p12.HEFY*/ bool FREQ_SHIFT_FLIP_03;
  /*p12.GOPO*/ bool FREQ_SHIFT_FLIP_02;
  /*p12.GELA*/ bool FREQ_SHIFT_FLIP_01;
  /*p12.GYLO*/ bool FREQ_SHIFT_FLIP_00;

  /*p12.BEJU*/ bool FREQ_SHIFT_SET_10;
  /*p12.BESO*/ bool FREQ_SHIFT_SET_09;
  /*p12.BEGE*/ bool FREQ_SHIFT_SET_08;
  /*p12.DACE*/ bool FREQ_SHIFT_SET_07;
  /*p12.EKEM*/ bool FREQ_SHIFT_SET_06;
  /*p12.GOVO*/ bool FREQ_SHIFT_SET_05;
  /*p12.KOLA*/ bool FREQ_SHIFT_SET_04;
  /*p12.KYRY*/ bool FREQ_SHIFT_SET_03;
  /*p12.HAWY*/ bool FREQ_SHIFT_SET_02;
  /*p12.HOLA*/ bool FREQ_SHIFT_SET_01;
  /*p12.HOZU*/ bool FREQ_SHIFT_SET_00;

  /*p12.AVUF*/ bool FREQ_SHIFT_RST_10;
  /*p12.AFUX*/ bool FREQ_SHIFT_RST_09;
  /*p12.AGOR*/ bool FREQ_SHIFT_RST_08;
  /*p12.BEWO*/ bool FREQ_SHIFT_RST_07;
  /*p12.ENOK*/ bool FREQ_SHIFT_RST_06;
  /*p12.EZUK*/ bool FREQ_SHIFT_RST_05;
  /*p12.KYBO*/ bool FREQ_SHIFT_RST_04;
  /*p12.KETO*/ bool FREQ_SHIFT_RST_03;
  /*p12.HYVU*/ bool FREQ_SHIFT_RST_02;
  /*p12.HOBU*/ bool FREQ_SHIFT_RST_01;
  /*p12.JADO*/ bool FREQ_SHIFT_RST_00;


private:



  bool AJUX,AMAC,BASO,EMAR,ETOK,KYFU,KAVO,FEGA,FOKE,FOPU,EJYF;
  bool ARYL,BYLE;


  bool KEDO,JUJU,KAPE;
  bool AFYR,BUVO,AFUG,BAPU,EREG,EVOF,KEVY,KAXY,JEHY,JOCY,KOKO;
  bool FAJA,EJYB,CYBE,BECY;
};

