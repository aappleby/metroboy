#pragma once

struct Channel1 {

  // resets
  /*p11.HATO*/ bool FF12_RSTn;
  /*p11.CAMY*/ bool NR14_STOP_RST;
  /*p13.KADO*/ bool CH1_RSTd;
  /*p13.DUKA*/ bool START_RST;

  //----------
  // FF10

  /*p11.CENU*/ bool FF10_WR;
  /*p11.ATYN*/ bool FF10_RDn;
  /*p11.ASOP*/ bool FF10_RD;

  /*p11.BANY*/ bool NR10_SHIFT0;
  /*p11.ARAX*/ bool NR10_SHIFT1;
  /*p11.ANAZ*/ bool NR10_SHIFT2;
  /*p11.AVAF*/ bool NR10_SWEEP_DIR;
  /*p11.ADEK*/ bool NR10_DELAY0;
  /*p11.BANA*/ bool NR10_DELAY1;
  /*p11.BOTU*/ bool NR10_DELAY2;

  /*p13.BAVE*/ bool NR10_NO_SWEEP;
  /*p12.ARYL*/ bool NR10_SWEEP_DIRn;

  //----------
  // FF11

  /*p11.COVU*/ bool FF11_WR;
  /*p11.DAFO*/ bool FF11_WRn;
  /*p11.BEXU*/ bool FF11_RD;

  /*p11.CENA*/ bool NR11_DUTY_0;
  /*p11.DYCA*/ bool NR11_DUTY_1;

  //----------
  // FF12

  /*p11.KAGY*/ bool FF12_WRb;
  /*p11.HAFU*/ bool FF12_WRa;
  /*p11.KYGY*/ bool FF12_WRo;
  /*p11.GAXU*/ bool FF12_WRn;
  /*p11.HAMY*/ bool FF12_RDn;
  /*p11.HOCU*/ bool FF12_RDo;

  /*p11.JUSA*/ bool NR12_DELAY0;
  /*p11.JUZY*/ bool NR12_DELAY1;
  /*p11.JOMA*/ bool NR12_DELAY2;
  /*p11.JAFY*/ bool NR12_ENV_DIR;
  /*p11.JATY*/ bool NR12_VOL0;
  /*p11.JAXO*/ bool NR12_VOL1;
  /*p11.JENA*/ bool NR12_VOL2;
  /*p11.JOPU*/ bool NR12_VOL3;

  //----------
  // FF13 NR13 - ch1 freq

  /*p11.CEGE*/ bool CPU_RDb;
  /*p11.DAXA*/ bool CPU_RDnb;
  /*p11.DYPU*/ bool FF13_RDn;
  /*p11.EVAJ*/ bool FF13_RDa;

  /*p11.GEKU*/ bool CH1_FREQ_CLKa;
  /*p11.KYPE*/ bool CH1_FREQ_CLKb;
  /*p11.DERU*/ bool CH1_FREQ_CLKc;

  /*p11.EPYK*/ bool CH1_FREQ_RSTn;
  /*p11.FUME*/ bool CH1_FREQ_RSTa;
  /*p11.DEGA*/ bool CH1_FREQ_RSTb;
  /*p11.DAKO*/ bool CH1_FREQ_RSTc;

  /*p11.GAXE*/ bool FREQ_CNT_00;
  /*p11.HYFE*/ bool FREQ_CNT_01;
  /*p11.JYTY*/ bool FREQ_CNT_02;
  /*p11.KYNA*/ bool FREQ_CNT_03;
  /*p11.JEMA*/ bool FREQ_CNT_04;
  /*p11.HYKE*/ bool FREQ_CNT_05;
  /*p11.FEVA*/ bool FREQ_CNT_06;
  /*p11.EKOV*/ bool FREQ_CNT_07;
  /*p11.EMUS*/ bool FREQ_CNT_08;
  /*p11.EVAK*/ bool FREQ_CNT_09;
  /*p11.COPU*/ bool FREQ_CNT_10;

  //----------
  // FF14

  /*p11.BAGE*/ bool FF14_WRn;
  /*p11.BOKO*/ bool NR14_STOP;
  /*p11.DUPY*/ bool FF14_RDa;
  /*p11.BALE*/ bool FF14_RDb;

  /*p12.DEBY*/ bool FREQ_WRb;
  /*p12.DYLA*/ bool FREQ_WRc;

  /*p12.HORA*/ bool SUM_B00;
  /*p12.HOPA*/ bool SUM_B01;
  /*p12.HELE*/ bool SUM_B02;
  /*p12.JAPE*/ bool SUM_B03;
  /*p12.JETE*/ bool SUM_B04;
  /*p12.GELE*/ bool SUM_B05;
  /*p12.EPYR*/ bool SUM_B06;
  /*p12.EDOK*/ bool SUM_B07;
  /*p12.DEFA*/ bool SUM_B08;
  /*p12.ETER*/ bool SUM_B09;
  /*p12.DEVA*/ bool SUM_B10;

  /*p12.GALO*/ bool SUM_A00;
  /*p12.JODE*/ bool SUM_A01;
  /*p12.KARE*/ bool SUM_A02;
  /*p12.JYME*/ bool SUM_A03;
  /*p12.GYME*/ bool SUM_A04;
  /*p12.FAXO*/ bool SUM_A05;
  /*p12.EXAP*/ bool SUM_A06;
  /*p12.DELE*/ bool SUM_A07;
  /*p12.DEXE*/ bool SUM_A08;
  /*p12.DOFY*/ bool SUM_A09;
  /*p12.DOLY*/ bool SUM_A10;

  /*p12.HYKA*/ bool SUMMER00;
  /*p12.JYKA*/ bool SUMMER01;
  /*p12.HAVO*/ bool SUMMER02;
  /*p12.EDUL*/ bool SUMMER03;
  /*p12.FELY*/ bool SUMMER04;
  /*p12.HOLU*/ bool SUMMER05;
  /*p12.HYXU*/ bool SUMMER06;
  /*p12.HOPO*/ bool SUMMER07;
  /*p12.DYGY*/ bool SUMMER08;
  /*p12.EVAB*/ bool SUMMER09;
  /*p12.AXAN*/ bool SUMMER10;

  /*p12.EFOR*/ bool SUMMER00_RST;
  /*p12.GATO*/ bool SUMMER01_RST;
  /*p12.GYFU*/ bool SUMMER02_RST;
  /*p12.GAMO*/ bool SUMMER03_RST;
  /*p12.KAPO*/ bool SUMMER04_RST;
  /*p12.KAJU*/ bool SUMMER05_RST;
  /*p12.ELUF*/ bool SUMMER06_RST;
  /*p12.ESEL*/ bool SUMMER07_RST;
  /*p12.BOXU*/ bool SUMMER08_RST;
  /*p12.BOVU*/ bool SUMMER09_RST;
  /*p12.APAJ*/ bool SUMMER10_RST;

  /*p12.GYLU*/ bool SUMMER00_SET;
  /*p12.GETA*/ bool SUMMER01_SET;
  /*p12.GOLO*/ bool SUMMER02_SET;
  /*p12.GOPE*/ bool SUMMER03_SET;
  /*p12.KOVU*/ bool SUMMER04_SET;
  /*p12.KYPA*/ bool SUMMER05_SET;
  /*p12.ELER*/ bool SUMMER06_SET;
  /*p12.ETOL*/ bool SUMMER07_SET;
  /*p12.BUGU*/ bool SUMMER08_SET;
  /*p12.BUDO*/ bool SUMMER09_SET;
  /*p12.AFEG*/ bool SUMMER10_SET;

  /*p12.ATYS*/ bool FREQ_OVERFLOW;

  //----------

  /*p12.DEKE*/ bool FREQ_D00n;
  /*p12.GULU*/ bool FREQ_D01n;
  /*p12.FULE*/ bool FREQ_D02n;
  /*p12.ETUV*/ bool FREQ_D03n;
  /*p12.KOPU*/ bool FREQ_D04n;
  /*p12.JULO*/ bool FREQ_D05n;
  /*p12.DYLU*/ bool FREQ_D06n;
  /*p12.DULO*/ bool FREQ_D07n;
  /*p12.BYSU*/ bool FREQ_D08n;
  /*p12.BOFU*/ bool FREQ_D09n;
  /*p12.BYFU*/ bool FREQ_D10n;

  /*p12.EJYF*/ bool FREQ_WR_00;
  /*p12.FOPU*/ bool FREQ_WR_01;
  /*p12.FOKE*/ bool FREQ_WR_02;
  /*p12.FEGA*/ bool FREQ_WR_03;
  /*p12.KAVO*/ bool FREQ_WR_04;
  /*p12.KYFU*/ bool FREQ_WR_05;
  /*p12.ETOK*/ bool FREQ_WR_06;
  /*p12.EMAR*/ bool FREQ_WR_07;
  /*p12.BASO*/ bool FREQ_WR_08;
  /*p12.AMAC*/ bool FREQ_WR_09;
  /*p12.AJUX*/ bool FREQ_WR_10;

  //----------

  /*p13.BUGE*/ bool SHIFTER_EN;

  /*p12.GYLO*/ bool SHIFTER_FLIP_00;
  /*p12.GELA*/ bool SHIFTER_FLIP_01;
  /*p12.GOPO*/ bool SHIFTER_FLIP_02;
  /*p12.HEFY*/ bool SHIFTER_FLIP_03;
  /*p12.KEFE*/ bool SHIFTER_FLIP_04;
  /*p12.GOLY*/ bool SHIFTER_FLIP_05;
  /*p12.FURE*/ bool SHIFTER_FLIP_06;
  /*p12.DYME*/ bool SHIFTER_FLIP_07;
  /*p12.CALE*/ bool SHIFTER_FLIP_08;
  /*p12.DOZY*/ bool SHIFTER_FLIP_09;
  /*p12.CULU*/ bool SHIFTER_FLIP_10;

  /*p12.FABU*/ bool SHIFTER_00;
  /*p12.JEFA*/ bool SHIFTER_01;
  /*p12.GOGA*/ bool SHIFTER_02;
  /*p12.JOLU*/ bool SHIFTER_03;
  /*p12.JOTA*/ bool SHIFTER_04;
  /*p12.FUDE*/ bool SHIFTER_05;
  /*p12.FEDO*/ bool SHIFTER_06;
  /*p12.EXAC*/ bool SHIFTER_07;
  /*p12.ELUX*/ bool SHIFTER_08;
  /*p12.AGEZ*/ bool SHIFTER_09;
  /*p12.BEKU*/ bool SHIFTER_10;

  //----------

  /*p13.DUNA*/ bool RAW_BIT;
  /*p13.CARA*/ bool CH1_ACTIVEn;


  /*p13.AMOP*/ bool CH1_OUT0;
  /*p13.ASON*/ bool CH1_OUT1;
  /*p13.AGOF*/ bool CH1_OUT2;
  /*p13.ACEG*/ bool CH1_OUT3;
  /*p13.HOCA*/ bool CH1_AMP_ENn;

  //----------

  /*p09.CALO*/ bool SEQ_CLK;
  /*p09.DYFA*/ bool SEQ_CLKn;

  /*p13.EGET*/ bool SEQ0_RST;
  /*p13.GEFE*/ bool SEQ0_RSTn;
  /*p13.ERUM*/ bool SEQ_RSTo;

  /*p13.FYFO*/ bool SEQ_D;

  /*p13.FEKU*/ bool SEQ0;
  /*p13.FARE*/ bool SEQ1;
  /*p13.FYTE*/ bool SEQ2;
  /*p13.GEPU*/ bool SEQ2n;

  //----------

  /*p13.GEXU*/ bool FREQ_GATE;
  /*p13.BUSO*/ bool SUMMER_CLKb;
  /*p13.BOJE*/ bool SUMMER_CLKa;

  /*p13.BYTE*/ bool SHIFT_DONE_SYNC;
  /*p13.ATAT*/ bool SHIFT_DONE_SYNC_RST;
  /*p13.ADAD*/ bool SHIFT_DONE_SYNCn;
  /*p13.KYLY*/ bool SUMMER_RSTn;
  
  /*p13.EGOR*/ bool SHIFTER_CLKn;

  /*p13.BACY*/ bool NR11_LEN0;
  /*p13.CAVY*/ bool NR11_LEN1;
  /*p13.BOVY*/ bool NR11_LEN2;
  /*p13.CUNO*/ bool NR11_LEN3;
  /*p13.CUSO*/ bool NR11_LEN3n;
  /*p13.CURA*/ bool NR11_LEN4;
  /*p13.ERAM*/ bool NR11_LEN5;
    
  /*p13.CERO*/ bool CH1_LEN_DONE;


  /*p13.CYMU*/ bool SWEEP_DELAY_RSTn;
  /*p13.DAFA*/ bool SWEEP_DELAY_RSTb;

  /*p13.CUPO*/ bool SWEEP_DELAY0;
  /*p13.CYPU*/ bool SWEEP_DELAY1;
  /*p13.CAXY*/ bool SWEEP_DELAY2;
  /*p13.BEXA*/ bool SWEEP_TRIGGER;
  /*p13.BURY*/ bool SWEEP_TRIGGER_RSTn;
  /*p13.COZE*/ bool SWEEP_MAX;
  /*p09.CATE*/ bool SWEEP_DELAY_CLK;
  /*p09.CONE*/ bool SWEEP_CLKn;
  /*p09.CELY*/ bool SWEEP_CLKb;




  /*p13.CALA*/ bool PHASE_CLK_CLK;
  /*p13.DOKA*/ bool DOKA;
  /*p13.DYRU*/ bool PHASE_CLK_RST;
  /*p13.COMY*/ bool PHASE_CLKnb;
  /*p13.CYTE*/ bool PHASE_CLKb;
  /*p13.COPE*/ bool PHASE_CLKna;
  /*p13.DAJO*/ bool PHASE_CLKa;

  /*p13.ESUT*/ bool PHASE_xBxDxFxH;
  /*p13.EROS*/ bool PHASE_xxCDxxGH;
  /*p13.DAPE*/ bool PHASE_xxxxEFGH;

  // Sweep shift counter
  /*p13.CYLU*/ bool SHIFTER_CNT_LD;
  /*p13.DACU*/ bool SHIFTER_CNT_LDn;
  /*p13.DAPU*/ bool SHIFTER_CNT_CLK;
  /*p13.COPA*/ bool SHIFTER_CNT0;
  /*p13.CAJA*/ bool SHIFTER_CNT1;
  /*p13.BYRA*/ bool SHIFTER_CNT2;
  /*p12.EJYB*/ bool SHIFTER_CLKo;
  /*p12.BECY*/ bool SHIFTER_CLKp;

  //----------

  /*p13.JADE*/ bool ENV_MAX;
  /*p13.KOMA*/ bool ENV_EN;
  /*p13.HAKE*/ bool ENV_TOP;
  /*p13.HANO*/ bool ENV_BOT;
  /*p13.HUFU*/ bool ENV_TOPn;
  /*p13.KOTE*/ bool ENV_TRIGGER;
  /*p13.HESU*/ bool ENV_DELTA0;
  /*p13.HETO*/ bool ENV_DELTA1;
  /*p13.HYTO*/ bool ENV_DELTA2;
  /*p13.JUFY*/ bool ENV_DELTA3;
  /*p13.HEVO*/ bool ENV0;
  /*p13.HOKO*/ bool ENV1;
  /*p13.HEMY*/ bool ENV2;
  /*p13.HAFO*/ bool ENV3;
  /*p13.KOZY*/ bool ENV_CLK;
  /*p13.KYNO*/ bool ENV_STOP;
  /*p13.KORU*/ bool ENV_STOP_RST;
  /*p13.JOVA*/ bool ENV_DELAY0;
  /*p13.KENU*/ bool ENV_DELAY1;
  /*p13.KERA*/ bool ENV_DELAY2;
  /*p13.KUXU*/ bool ENV_DELAY_LOAD;
  /*p13.KAZA*/ bool ENV_DELAY_LOADn;

  //----------

  /*p13.DUWO*/ bool RAW_BIT_SYNC;
  /*p13.EZEC*/ bool START;

  /*p13.DUPE*/ bool NR14_START;

  /*p13.JONE*/ bool CLK_128n;
  /*p13.KALY*/ bool CLK_64a;
  /*p13.KERE*/ bool CLK_64nb;
  /*p13.JOLA*/ bool CLK_64b;
   
  /*p13.CANU*/ bool CH1_LEN_CLK;
  /*p13.CAPY*/ bool CH1_LEN_CLKn;
  /*p13.CYTO*/ bool CH1_ACTIVE;


  /*p13.BORO*/ bool FF11_WRna;
  /*p13.BEPE*/ bool FF11_WRa;
  /*p13.BOKA*/ bool FF11_WRb;
  /*p13.BUGY*/ bool FF11_WRc;

  /*p13.DADO*/ bool NR14_START_RST;
  /*p13.DOGE*/ bool FF14_WRnb;

  /*p13.BERY*/ bool CH1_STOP;
  /*p13.BOTO*/ bool BIT_OUT;
  /*p13.CODO*/ bool PHASE_ABCDEFxx;
  /*p13.COPY*/ bool SHIFT_DONE;
  /*p13.CORY*/ bool CH1_LEN_DONE_RST;



  /*p13.DODY*/ bool DODY;
  /*p13.DUVO*/ bool PHASE_AxCxExGx;
  /*p13.EGYP*/ bool EGYP;
  /*p13.ENEK*/ bool PHASE_xxxxxxGx;
  /*p13.EPUK*/ bool EPUK;
  /*p13.EVOL*/ bool EVOL;
  /*p13.EZOZ*/ bool PHASE_xxxxxxGH;
  /*p13.FEMU*/ bool FEMU;
  /*p13.KABA*/ bool SUMMER_RST;
  /*p13.KAKE*/ bool KAKE;
  /*p13.KEKO*/ bool KEKO;
  /*p13.KEZU*/ bool KEZU;
  /*p13.KORO*/ bool SWEEP_CNT_MAX_SYNC_RST;
  /*p13.KUKU*/ bool KUKU;
  /*p13.KURY*/ bool SWEEP_CNT_MAX_SYNCn;

  /*p09.BAZA*/ bool DBG_SWEEP_CLK;
};

