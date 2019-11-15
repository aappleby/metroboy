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

  /*p12.BOJO*/ bool FREQ_SUM_10n;
  /*p12.APAT*/ bool FREQ_SUM_09n;
  /*p12.BYRU*/ bool FREQ_SUM_08n;
  /*p12.CYKY*/ bool FREQ_SUM_07n;
  /*p12.DEBO*/ bool FREQ_SUM_06n;
  /*p12.FOHY*/ bool FREQ_SUM_05n;
  /*p12.KOVO*/ bool FREQ_SUM_04n;
  /*p12.KEKE*/ bool FREQ_SUM_03n;
  /*p12.HUNY*/ bool FREQ_SUM_02n;
  /*p12.HOXE*/ bool FREQ_SUM_01n;
  /*p12.JUTA*/ bool FREQ_SUM_00n;

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

  bool DOLY,DOFY,DEXE,DELE,EXAP,FAXO,GYME,JYME,KARE,JODE,GALO;
  bool DEVA,ETER,DEFA,EDOK,EPYR,GELE,JETE,JAPE,HELE,HOPA,HORA;

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

