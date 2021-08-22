/// plait_noparse

#pragma once

//-----------------------------------------------------------------------------

struct Channel1 {

  //----------
  // FF10

  /*_p11.CENU*/ bool NR10_CLK;
  /*_p11.BANY*/ bool NR10_SHIFT0;
  /*_p11.ARAX*/ bool NR10_SHIFT1;
  /*_p11.ANAZ*/ bool NR10_SHIFT2;
  /*_p11.AVAF*/ bool NR10_SWEEP_DIR;
  /*_p11.ADEK*/ bool NR10_DELAY0;
  /*_p11.BANA*/ bool NR10_DELAY1;
  /*_p11.BOTU*/ bool NR10_DELAY2;

  //----------
  // FF11

  /*_p11.DAFO*/ bool NR11_CLK;
  /*_p11.CENA*/ bool NR11_DUTY_0;
  /*_p11.DYCA*/ bool NR11_DUTY_1;

  //----------
  // FF12

  /*_p11.KYGY*/ bool NR12_CLKa;
  /*_p11.GAXU*/ bool NR12_CLKb;

  /*_p11.JUSA*/ bool NR12_DELAY0;
  /*_p11.JUZY*/ bool NR12_DELAY1;
  /*_p11.JOMA*/ bool NR12_DELAY2;
  /*_p11.JAFY*/ bool NR12_ENV_DIR;
  /*_p11.JATY*/ bool NR12_VOL0;
  /*_p11.JAXO*/ bool NR12_VOL1;
  /*_p11.JENA*/ bool NR12_VOL2;
  /*_p11.JOPU*/ bool NR12_VOL3;

  //----------
  // FF13 NR13 - ch1 freq

  /*_p11.GEKU*/ bool FREQ_CLKa;
  /*_p11.KYPE*/ bool FREQ_CLKb;
  /*_p11.DERU*/ bool FREQ_CLKc;

  /*_p11.GAXE*/ bool FREQ_CNT_00;
  /*_p11.HYFE*/ bool FREQ_CNT_01;
  /*_p11.JYTY*/ bool FREQ_CNT_02;
  /*_p11.KYNA*/ bool FREQ_CNT_03;
  /*_p11.JEMA*/ bool FREQ_CNT_04;
  /*_p11.HYKE*/ bool FREQ_CNT_05;
  /*_p11.FEVA*/ bool FREQ_CNT_06;
  /*_p11.EKOV*/ bool FREQ_CNT_07;
  /*_p11.EMUS*/ bool FREQ_CNT_08;
  /*_p11.EVAK*/ bool FREQ_CNT_09;
  /*_p11.COPU*/ bool FREQ_CNT_10;

  //----------
  // FF14

  /*_p11.BAGE*/ bool NR14_CLKa;
  /*_p11.BOKO*/ bool NR14_STOP;

  /*_p12.HORA*/ bool SUM_B00;
  /*_p12.HOPA*/ bool SUM_B01;
  /*_p12.HELE*/ bool SUM_B02;
  /*_p12.JAPE*/ bool SUM_B03;
  /*_p12.JETE*/ bool SUM_B04;
  /*_p12.GELE*/ bool SUM_B05;
  /*_p12.EPYR*/ bool SUM_B06;
  /*_p12.EDOK*/ bool SUM_B07;
  /*_p12.DEFA*/ bool SUM_B08;
  /*_p12.ETER*/ bool SUM_B09;
  /*_p12.DEVA*/ bool SUM_B10;

  /*_p12.GALO*/ bool SUM_A00;
  /*_p12.JODE*/ bool SUM_A01;
  /*_p12.KARE*/ bool SUM_A02;
  /*_p12.JYME*/ bool SUM_A03;
  /*_p12.GYME*/ bool SUM_A04;
  /*_p12.FAXO*/ bool SUM_A05;
  /*_p12.EXAP*/ bool SUM_A06;
  /*_p12.DELE*/ bool SUM_A07;
  /*_p12.DEXE*/ bool SUM_A08;
  /*_p12.DOFY*/ bool SUM_A09;
  /*_p12.DOLY*/ bool SUM_A10;

  /*_p12.HYKA*/ bool SUMMER00;
  /*_p12.JYKA*/ bool SUMMER01;
  /*_p12.HAVO*/ bool SUMMER02;
  /*_p12.EDUL*/ bool SUMMER03;
  /*_p12.FELY*/ bool SUMMER04;
  /*_p12.HOLU*/ bool SUMMER05;
  /*_p12.HYXU*/ bool SUMMER06;
  /*_p12.HOPO*/ bool SUMMER07;
  /*_p12.DYGY*/ bool SUMMER08;
  /*_p12.EVAB*/ bool SUMMER09;
  /*_p12.AXAN*/ bool SUMMER10;

  /*_p12.ATYS*/ bool FREQ_OVERFLOW;

  //----------

  /*_p13.BUGE*/ bool SHIFTER_EN;

  /*_p12.FABU*/ bool SHIFTER_00;
  /*_p12.JEFA*/ bool SHIFTER_01;
  /*_p12.GOGA*/ bool SHIFTER_02;
  /*_p12.JOLU*/ bool SHIFTER_03;
  /*_p12.JOTA*/ bool SHIFTER_04;
  /*_p12.FUDE*/ bool SHIFTER_05;
  /*_p12.FEDO*/ bool SHIFTER_06;
  /*_p12.EXAC*/ bool SHIFTER_07;
  /*_p12.ELUX*/ bool SHIFTER_08;
  /*_p12.AGEZ*/ bool SHIFTER_09;
  /*_p12.BEKU*/ bool SHIFTER_10;

  //----------

  /*_p13.CARA*/ bool CH1_ACTIVEn;


  /*_p13.AMOP*/ bool CH1_OUT0;
  /*_p13.ASON*/ bool CH1_OUT1;
  /*_p13.AGOF*/ bool CH1_OUT2;
  /*_p13.ACEG*/ bool CH1_OUT3;
  /*_p13.HOCA*/ bool CH1_AMP_ENn;

  //----------

  /*_p09.CALO*/ bool RESTART_CLK;
  /*_p09.DYFA*/ bool RESTART_CLKn;

  /*_p13.GEFE*/ bool RESTART0_RSTn;
  /*_p13.ERUM*/ bool RESTART_RSTo;

  /*_p13.FEKU*/ bool RESTART0;
  /*_p13.FARE*/ bool RESTART1;
  /*_p13.FYTE*/ bool RESTART2;


  //----------

  /*_p13.GEXU*/ bool FREQ_GATE;

  /*_p13.BYTE*/ bool SHIFT_DONE_SYNC;
  /*_p13.ATAT*/ bool SHIFT_DONE_SYNC_RST;
  /*_p13.ADAD*/ bool SHIFT_DONE_SYNCn;

  /*_p13.EGOR*/ bool SHIFTER_CLKn;

  /*_p13.BACY*/ bool NR11_LEN0;
  /*_p13.CAVY*/ bool NR11_LEN1;
  /*_p13.BOVY*/ bool NR11_LEN2;
  /*_p13.CUNO*/ bool NR11_LEN3;
  /*_p13.CUSO*/ bool NR11_LEN3n;
  /*_p13.CURA*/ bool NR11_LEN4;
  /*_p13.ERAM*/ bool NR11_LEN5;

  /*_p13.CERO*/ bool CH1_LEN_DONE;

  /*_p13.CUPO*/ bool SWEEP_DELAY0;
  /*_p13.CYPU*/ bool SWEEP_DELAY1;
  /*_p13.CAXY*/ bool SWEEP_DELAY2;
  /*_p13.BEXA*/ bool SWEEP_TRIGGER;

  /*_p09.CATE*/ bool SWEEP_DELAY_CLK;





  /*_p13.CALA*/ bool PHASE_CLK_CLK;

  /*_p13.COMY*/ bool PHASE_CLKnb;
  /*_p13.COPE*/ bool PHASE_CLKna;
  /*_p13.DAJO*/ bool PHASE_CLKa;

  /*_p13.ESUT*/ bool PHASE_xBxDxFxH;
  /*_p13.EROS*/ bool PHASE_xxCDxxGH;
  /*_p13.DAPE*/ bool PHASE_xxxxEFGH;

  // Sweep shift counter
  /*_p13.CYLU*/ bool SHIFTER_CNT_LD;
  /*_p13.DACU*/ bool SHIFTER_CNT_LDn;
  /*_p13.DAPU*/ bool SHIFTER_CNT_CLK;
  /*_p13.COPA*/ bool SHIFTER_CNT0;
  /*_p13.CAJA*/ bool SHIFTER_CNT1;
  /*_p13.BYRA*/ bool SHIFTER_CNT2;
  /*_p12.EJYB*/ bool SHIFTER_CLKo;
  /*_p12.BECY*/ bool SHIFTER_CLKp;

  //----------

  /*_p13.HESU*/ bool ENV_DELTA0;
  /*_p13.HETO*/ bool ENV_DELTA1;
  /*_p13.HYTO*/ bool ENV_DELTA2;
  /*_p13.JUFY*/ bool ENV_DELTA3;
  /*_p13.HEVO*/ bool ENV0;
  /*_p13.HOKO*/ bool ENV1;
  /*_p13.HEMY*/ bool ENV2;
  /*_p13.HAFO*/ bool ENV3;
  /*_p13.KOZY*/ bool ENV_CLK;
  /*_p13.KYNO*/ bool ENV_STOP;
  /*_p13.JOVA*/ bool ENV_DELAY0;
  /*_p13.KENU*/ bool ENV_DELAY1;
  /*_p13.KERA*/ bool ENV_DELAY2;


  //----------

  /*_p13.DUWO*/ bool RAW_BIT_SYNC;
  /*_p13.EZEC*/ bool START;

  /*_p13.DUPE*/ bool NR14_START;

  /*_p13.JONE*/ bool CLK_128n;
  /*_p13.KALY*/ bool CLK_64a;

  /*_p13.CANU*/ bool CH1_LEN_CLK;
  /*_p13.CAPY*/ bool CH1_LEN_CLKn;
  /*_p13.CYTO*/ bool CH1_ACTIVE;


  /*_p13.BORO*/ bool FF11_WRna;
  /*_p13.BEPE*/ bool FF11_WRa;
  /*_p13.BOKA*/ bool FF11_WRb;
  /*_p13.BUGY*/ bool FF11_WRc;

  /*_p13.DOGE*/ bool FF14_WRnb;

  /*_p13.CORY*/ bool CH1_LEN_DONE_RST;

  /*_p09.BAZA*/ bool DBG_SWEEP_CLK;
};

//-----------------------------------------------------------------------------
