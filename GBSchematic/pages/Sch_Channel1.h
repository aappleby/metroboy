#pragma once

namespace Schematics {

//-----------------------------------------------------------------------------

struct Channel1 {

  //----------
  // FF10

  /*p11.CENU*/ bool NR10_CLK;
  /*p11.BANY*/ bool NR10_SHIFT0;
  /*p11.ARAX*/ bool NR10_SHIFT1;
  /*p11.ANAZ*/ bool NR10_SHIFT2;
  /*p11.AVAF*/ bool NR10_SWEEP_DIR;
  /*p11.ADEK*/ bool NR10_DELAY0;
  /*p11.BANA*/ bool NR10_DELAY1;
  /*p11.BOTU*/ bool NR10_DELAY2;

  //----------
  // FF11

  /*p11.DAFO*/ bool NR11_CLK;
  /*p11.CENA*/ bool NR11_DUTY_0;
  /*p11.DYCA*/ bool NR11_DUTY_1;

  //----------
  // FF12

  /*p11.KYGY*/ bool NR12_CLKa;
  /*p11.GAXU*/ bool NR12_CLKb;

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

  /*p11.GEKU*/ bool FREQ_CLKa;
  /*p11.KYPE*/ bool FREQ_CLKb;
  /*p11.DERU*/ bool FREQ_CLKc;

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

  /*p11.BAGE*/ bool NR14_CLKa;
  /*p11.BOKO*/ bool NR14_STOP;

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

  /*p12.ATYS*/ bool FREQ_OVERFLOW;

  //----------

  /*p13.BUGE*/ bool SHIFTER_EN;

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

  /*p13.CARA*/ bool CH1_ACTIVEn;


  /*p13.AMOP*/ bool CH1_OUT0;
  /*p13.ASON*/ bool CH1_OUT1;
  /*p13.AGOF*/ bool CH1_OUT2;
  /*p13.ACEG*/ bool CH1_OUT3;
  /*p13.HOCA*/ bool CH1_AMP_ENn;

  //----------

  /*p09.CALO*/ bool RESTART_CLK;
  /*p09.DYFA*/ bool RESTART_CLKn;

  /*p13.GEFE*/ bool RESTART0_RSTn;
  /*p13.ERUM*/ bool RESTART_RSTo;

  /*p13.FEKU*/ bool RESTART0;
  /*p13.FARE*/ bool RESTART1;
  /*p13.FYTE*/ bool RESTART2;


  //----------

  /*p13.GEXU*/ bool FREQ_GATE;

  /*p13.BYTE*/ bool SHIFT_DONE_SYNC;
  /*p13.ATAT*/ bool SHIFT_DONE_SYNC_RST;
  /*p13.ADAD*/ bool SHIFT_DONE_SYNCn;
  
  /*p13.EGOR*/ bool SHIFTER_CLKn;

  /*p13.BACY*/ bool NR11_LEN0;
  /*p13.CAVY*/ bool NR11_LEN1;
  /*p13.BOVY*/ bool NR11_LEN2;
  /*p13.CUNO*/ bool NR11_LEN3;
  /*p13.CUSO*/ bool NR11_LEN3n;
  /*p13.CURA*/ bool NR11_LEN4;
  /*p13.ERAM*/ bool NR11_LEN5;
    
  /*p13.CERO*/ bool CH1_LEN_DONE;

  /*p13.CUPO*/ bool SWEEP_DELAY0;
  /*p13.CYPU*/ bool SWEEP_DELAY1;
  /*p13.CAXY*/ bool SWEEP_DELAY2;
  /*p13.BEXA*/ bool SWEEP_TRIGGER;

  /*p09.CATE*/ bool SWEEP_DELAY_CLK;





  /*p13.CALA*/ bool PHASE_CLK_CLK;

  /*p13.COMY*/ bool PHASE_CLKnb;
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
  /*p13.JOVA*/ bool ENV_DELAY0;
  /*p13.KENU*/ bool ENV_DELAY1;
  /*p13.KERA*/ bool ENV_DELAY2;


  //----------

  /*p13.DUWO*/ bool RAW_BIT_SYNC;
  /*p13.EZEC*/ bool START;

  /*p13.DUPE*/ bool NR14_START;

  /*p13.JONE*/ bool CLK_128n;
  /*p13.KALY*/ bool CLK_64a;
   
  /*p13.CANU*/ bool CH1_LEN_CLK;
  /*p13.CAPY*/ bool CH1_LEN_CLKn;
  /*p13.CYTO*/ bool CH1_ACTIVE;


  /*p13.BORO*/ bool FF11_WRna;
  /*p13.BEPE*/ bool FF11_WRa;
  /*p13.BOKA*/ bool FF11_WRb;
  /*p13.BUGY*/ bool FF11_WRc;

  /*p13.DOGE*/ bool FF14_WRnb;

  /*p13.CORY*/ bool CH1_LEN_DONE_RST;

  /*p09.BAZA*/ bool DBG_SWEEP_CLK;
};

//-----------------------------------------------------------------------------

};