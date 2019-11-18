#pragma once

struct Channel1 {

  // resets
  /*p11.CEPO*/ bool CEPO;
  /*p11.HATO*/ bool HATO;
  /*p11.CAMY*/ bool CAMY;

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

  /*p12.DEPU*/ bool FREQ_WRa;
  /*p12.DEBY*/ bool FREQ_WRb;
  /*p12.DYLA*/ bool FREQ_WRc;

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

  /*p12.ATYS*/ bool CH1_SWEEP_STOPn;

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

  /*p12.ARYL*/ bool NR10_SWEEP_DIRn;
  /*p12.BYLE*/ bool CH1_SWEEP_MAX;

  /*p12.AJUX*/ bool AJUX;
  /*p12.AMAC*/ bool AMAC;
  /*p12.BASO*/ bool BASO;
  /*p12.EMAR*/ bool EMAR;
  /*p12.ETOK*/ bool ETOK;
  /*p12.KYFU*/ bool KYFU;
  /*p12.KAVO*/ bool KAVO;
  /*p12.FEGA*/ bool FEGA;
  /*p12.FOKE*/ bool FOKE;
  /*p12.FOPU*/ bool FOPU;
  /*p12.EJYF*/ bool EJYF;

  /*p12.KEDO*/ bool KEDO;
  /*p12.JUJU*/ bool JUJU;
  /*p12.KAPE*/ bool KAPE;
  /*p12.AFYR*/ bool AFYR;
  /*p12.BUVO*/ bool BUVO;
  /*p12.AFUG*/ bool AFUG;
  /*p12.BAPU*/ bool BAPU;
  /*p12.EREG*/ bool EREG;
  /*p12.EVOF*/ bool EVOF;
  /*p12.KEVY*/ bool KEVY;
  /*p12.KAXY*/ bool KAXY;
  /*p12.JEHY*/ bool JEHY;
  /*p12.JOCY*/ bool JOCY;
  /*p12.KOKO*/ bool KOKO;
  /*p12.FAJA*/ bool FAJA;
  /*p12.EJYB*/ bool EJYB;
  /*p12.CYBE*/ bool CYBE;
  /*p12.BECY*/ bool BECY;

  /*p13.DUNA*/ bool CH1_RAW_BIT;
  /*p13.CARA*/ bool CH1_ACTIVEn;

  /*p13.HESU*/ bool CH1_ENV_DELTA0;
  /*p13.HETO*/ bool CH1_ENV_DELTA1;
  /*p13.HYTO*/ bool CH1_ENV_DELTA2;
  /*p13.JUFY*/ bool CH1_ENV_DELTA3;

  /*p13.HEVO*/ bool CH1_ENV0;
  /*p13.HOKO*/ bool CH1_ENV1;
  /*p13.HEMY*/ bool CH1_ENV2;
  /*p13.HAFO*/ bool CH1_ENV3;

  /*p13.AMOP*/ bool CH1_OUT0;
  /*p13.ASON*/ bool CH1_OUT1;
  /*p13.AGOF*/ bool CH1_OUT2;
  /*p13.ACEG*/ bool CH1_OUT3;
  /*p13.HOCA*/ bool CH1_AMP_ENn;

  /*p13.GEXU*/ bool GEXU;
  /*p13.FEKU*/ bool FEKU;
  /*p13.BUSO*/ bool BUSO;
  /*p13.BOJE*/ bool BOJE;
  /*p13.ADAD*/ bool ADAD;
  /*p13.KYLY*/ bool KYLY;
  /*p13.KALA*/ bool KALA;
  /*p13.EGOR*/ bool EGOR;

  /*p13.BACY*/ bool NR11_LEN0;
  /*p13.CAVY*/ bool NR11_LEN1;
  /*p13.BOVY*/ bool NR11_LEN2;
  /*p13.CUNO*/ bool NR11_LEN3;
  /*p13.CUSO*/ bool NR11_LEN3n;
  /*p13.CURA*/ bool NR11_LEN4;
  /*p13.ERAM*/ bool NR11_LEN5;
    
  /*p13.CERO*/ bool CH1_LEN_DONE;

  /*p13.CAXY*/ bool CAXY;
  /*p13.CYPU*/ bool CYPU;
  /*p13.CUPO*/ bool CUPO;
  /*p13.BEXA*/ bool BEXA;

  /*p13.CALA*/ bool CALA;
  /*p13.DOKA*/ bool DOKA;
  /*p13.DYRU*/ bool DYRU;
  /*p13.COMY*/ bool CH1_PHASE_CLKnb;
  /*p13.CYTE*/ bool CH1_PHASE_CLKb;
  /*p13.COPE*/ bool CH1_PHASE_CLKna;
  /*p13.DAJO*/ bool CH1_PHASE_CLKa;

  /*p13.ESUT*/ bool CH1_PHASE_0;
  /*p13.EROS*/ bool CH1_PHASE_1;
  /*p13.DAPE*/ bool CH1_PHASE_2;
  /*p13.COSO*/ bool DUTY_0;
  /*p13.CAVA*/ bool DUTY_1;
  /*p13.CEVU*/ bool DUTY_2;
  /*p13.CAXO*/ bool DUTY_3;

  // Sweep shift counter
  /*p13.COPA*/ bool COPA;
  /*p13.CAJA*/ bool CAJA;
  /*p13.BYRA*/ bool BYRA;
  /*p13.BYTE*/ bool BYTE;

  // EG timer
  /*p13.KOZY*/ bool KOZY;
  /*p13.DUWO*/ bool DUWO;
  /*p13.FARE*/ bool FARE;
  /*p13.FYTE*/ bool FYTE;
  /*p13.EZEC*/ bool CH1_RUNNING;
  /*p13.KYNO*/ bool KYNO;

  /*p13.DUPE*/ bool NR14_START;

  /*p13.JONE*/ bool CLK_128n;
  /*p13.KALY*/ bool CLK_64a;
  /*p13.KERE*/ bool CLK_64nb;
  /*p13.JOLA*/ bool CLK_64b;

  /*p13.JOVA*/ bool SWEEP_CNT0;
  /*p13.KENU*/ bool SWEEP_CNT1;
  /*p13.KERA*/ bool SWEEP_CNT2;



  /*p13.KOMA*/ bool CH1_NO_SWEEP;
  /*p13.CANU*/ bool CH1_LEN_CLK;
  /*p13.CAPY*/ bool CH1_LEN_CLKn;
  /*p13.CYTO*/ bool CH1_ACTIVE;

  /*p13.KUXU*/ bool SWEEP_RST;

  /*p13.BORO*/ bool FF11_WRna;
  /*p13.BEPE*/ bool FF11_WRa;
  /*p13.BOKA*/ bool FF11_WRb;
  /*p13.BUGY*/ bool FF11_WRc;

  /*p13.DADO*/ bool NR14_START_RST;
  /*p13.DOGE*/ bool FF14_WRnb;

  /*p13.ATAT*/ bool ATAT;
  /*p13.ATUV*/ bool ATUV;
  /*p13.BAVE*/ bool BAVE;
  /*p13.BERY*/ bool CH1_STOP;
  /*p13.BONE*/ bool CH1_SWEEP_STOP;
  /*p13.BOTO*/ bool CH1_BIT;
  /*p13.BUGE*/ bool BUGE;
  /*p13.BURY*/ bool BURY;
  /*p13.CELE*/ bool CELE;
  /*p13.CODO*/ bool CODO;
  /*p13.COPY*/ bool COPY;
  /*p13.CORY*/ bool CORY;
  /*p13.COWE*/ bool COWE;
  /*p13.COZE*/ bool COZE;
  /*p13.CYFA*/ bool CH1_LEN_STOP;
  /*p13.CYLU*/ bool CYLU;
  /*p13.CYMU*/ bool CYMU;
  /*p13.DACU*/ bool DACU;
  /*p13.DAFA*/ bool DAFA;
  /*p13.DAPU*/ bool DAPU;
  /*p13.DODY*/ bool DODY;
  /*p13.DUKA*/ bool DUKA;
  /*p13.DUVO*/ bool DUVO;
  /*p13.EGET*/ bool EGET;
  /*p13.EGYP*/ bool EGYP;
  /*p13.ENEK*/ bool ENEK;
  /*p13.EPUK*/ bool EPUK;
  /*p13.ERUM*/ bool ERUM;
  /*p13.EVOL*/ bool EVOL;
  /*p13.EZOZ*/ bool EZOZ;
  /*p13.FEMU*/ bool FEMU;
  /*p13.FEMY*/ bool FEMY;
  /*p13.FYFO*/ bool FYFO;
  /*p13.GEFE*/ bool GEFE;
  /*p13.GEPU*/ bool GEPU;
  /*p13.HAKE*/ bool CH1_ENV_TOP;
  /*p13.HANO*/ bool CH1_ENV_BOT;
  /*p13.HUFU*/ bool CH1_ENV_TOPn;
  /*p13.JADE*/ bool CH1_ENV_MAX;
  /*p13.KABA*/ bool KABA;
  /*p13.KADO*/ bool KADO;
  /*p13.KAKE*/ bool KAKE;
  /*p13.KAZA*/ bool KAZA;
  /*p13.KEKO*/ bool KEKO;
  /*p13.KEZU*/ bool KEZU;
  /*p13.KORO*/ bool KORO;
  /*p13.KORU*/ bool KORU;
  /*p13.KOTE*/ bool KOTE;
  /*p13.KUKU*/ bool KUKU;
  /*p13.KURY*/ bool KURY;
};

