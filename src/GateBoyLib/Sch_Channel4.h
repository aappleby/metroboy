/// plait_noparse

#pragma once

//-----------------------------------------------------------------------------

// Noise
// Length counter
// Env vol / dir / period
// Clock shift
// LFSR mode
// LFSR divisor
// START
// STOP

struct Channel4 {

  //----------
  // Clock/reset

  //----------
  // FF20 NR41 ch4 length

  /*_p19.DANO*/ bool NR41_LEN0;
  /*_p19.FAVY*/ bool NR41_LEN1;
  /*_p19.DENA*/ bool NR41_LEN2;
  /*_p19.CEDO*/ bool NR41_LEN3;
  /*_p19.DOPU*/ bool NR41_LEN3b;
  /*_p19.FYLO*/ bool NR41_LEN4;
  /*_p19.EDOP*/ bool NR41_LEN5;

  //----------
  // FF21 NR42 ch4 envelope

  /*_p19.DYKE*/ bool FF21_WRn;
  /*_p19.FUPA*/ bool FF21_WRo;

  /*_p19.EMOK*/ bool NR42_ENV_TIMER0;
  /*_p19.ETYJ*/ bool NR42_ENV_TIMER1;
  /*_p19.EZYK*/ bool NR42_ENV_TIMER2;
  /*_p19.GEKY*/ bool NR42_ENV_DIR;
  /*_p19.GARU*/ bool NR42_ENV_VOL0;
  /*_p19.GOKY*/ bool NR42_ENV_VOL1;
  /*_p19.GOZO*/ bool NR42_ENV_VOL2;
  /*_p19.GEDU*/ bool NR42_ENV_VOL3;

  //----------
  // FF22 NR43 ch4 freq

  /*_p19.HOVA*/ bool FF22_WRo;
  /*_p19.HOSO*/ bool FF22_WRp;
  /*_p19.EFUG*/ bool FF22_WRq;

  /*_p19.JARE*/ bool NR43_DIV0;
  /*_p19.JERO*/ bool NR43_DIV1;
  /*_p19.JAKY*/ bool NR43_DIV2;
  /*_p19.JAMY*/ bool NR43_MODE;
  /*_p19.FETA*/ bool NR43_FREQ0;
  /*_p19.FYTO*/ bool NR43_FREQ1;
  /*_p19.GOGO*/ bool NR43_FREQ2;
  /*_p19.GAFO*/ bool NR43_FREQ3;

  //---------
  // FF23

  /*_p19.DULU*/ bool FF23_WRn;
  /*_p19.FOXE*/ bool FF23_WRo;

  /*_p19.CUNY*/ bool NR44_STOP;
  /*_p19.HOGA*/ bool NR44_START;

  //----------
  // Control

  /*_p20.GYSU*/ bool CH4_START;


  /*_p20.GENA*/ bool CH4_ACTIVE;


  /*_p20.GONE*/ bool RESTART1;
  /*_p20.GORA*/ bool RESTART2;

  /*_p20.GATY*/ bool RESTART3;






  /*_p20.KANU*/ bool DIV_CLKa;

  /*_p20.JYCO*/ bool DIV0;
  /*_p20.JYRE*/ bool DIV1;
  /*_p20.JYFU*/ bool DIV2;
  /*_p20.HYNO*/ bool DIV_MAX;

  //----------
  // Length


  /*_p19.FUGO*/ bool LEN_STOP;

  /*_p19.CUWA*/ bool LEN_CLKa;

  //----------
  // Frequency timer

  /*_p20.GYBA*/ bool FREQ_GATE_CLK;
  /*_p20.GARY*/ bool FREQ_GATEn;
  /*_p20.CARY*/ bool FREQ_CLK;
  /*_p20.CEXO*/ bool FREQ_00;
  /*_p20.DEKO*/ bool FREQ_01;
  /*_p20.EZEF*/ bool FREQ_02;
  /*_p20.EPOR*/ bool FREQ_03;
  /*_p20.DURE*/ bool FREQ_04;
  /*_p20.DALE*/ bool FREQ_05;
  /*_p20.DOKE*/ bool FREQ_06;
  /*_p20.DEMO*/ bool FREQ_07;
  /*_p20.DOSE*/ bool FREQ_08;
  /*_p20.DETE*/ bool FREQ_09;
  /*_p20.ERUT*/ bool FREQ_10;
  /*_p20.DOTA*/ bool FREQ_11;
  /*_p20.DERE*/ bool FREQ_12;
  /*_p20.ESEP*/ bool FREQ_13;

  //----------
  // LFSR

  /*_p20.FEME*/ bool LFSR_CLKa;
  /*_p20.GYVE*/ bool LFSR_CLKb;
  /*_p20.KOPA*/ bool LFSR_CLKc;
  /*_p20.JYJA*/ bool LFSR_CLKn;
  /*_p20.GUFA*/ bool LFSR_CLKo;
  /*_p20.KARA*/ bool LFSR_CLKp;


  /*_p20.JOTO*/ bool LFSR_00;
  /*_p20.KOMU*/ bool LFSR_01;
  /*_p20.KETU*/ bool LFSR_02;
  /*_p20.KUTA*/ bool LFSR_03;
  /*_p20.KUZY*/ bool LFSR_04;
  /*_p20.KYWY*/ bool LFSR_05;
  /*_p20.JAJU*/ bool LFSR_06;
  /*_p20.HAPE*/ bool LFSR_07;
  /*_p20.JUXE*/ bool LFSR_08;
  /*_p20.JEPE*/ bool LFSR_09;
  /*_p20.JAVO*/ bool LFSR_10;
  /*_p20.HEPA*/ bool LFSR_11;
  /*_p20.HORY*/ bool LFSR_12;
  /*_p20.HENO*/ bool LFSR_13;
  /*_p20.HYRO*/ bool LFSR_14;
  /*_p20.HEZU*/ bool LFSR_15;

  //----------
  // Env

  /*_p20.ALOP*/ bool CLK_128n;

  /*_p20.ABEL*/ bool ENV_TIMER_CLKa;

  /*_p20.BUXO*/ bool ENV_TIMER_CLKb;


  /*_p20.CUNA*/ bool ENV_TIMER0;
  /*_p20.COFE*/ bool ENV_TIMER1;
  /*_p20.DOGO*/ bool ENV_TIMER2;


  /*_p20.FOSY*/ bool ENV_PULSE;



  /*_p20.FYNO*/ bool ENV_STOP;

  /*_p20.FOLE*/ bool VOL_CLK0;
  /*_p20.ETEF*/ bool VOL_CLK1;
  /*_p20.EDYF*/ bool VOL_CLK2;
  /*_p20.ELAF*/ bool VOL_CLK3;

  /*_p20.FEKO*/ bool VOL0;
  /*_p20.FATY*/ bool VOL1;
  /*_p20.FERU*/ bool VOL2;
  /*_p20.FYRO*/ bool VOL3;


  //----------
  // Output


  /*_p20.GEVY*/ bool CH4_AMP_ENn;
  /*_p20.AKOF*/ bool CH4_DAC0;
  /*_p20.BYZY*/ bool CH4_DAC1;
  /*_p20.APYR*/ bool CH4_DAC2;
  /*_p20.BOZA*/ bool CH4_DAC3;

  //----------
  // Debug

  /*_p20.DYRY*/ bool DBG_CH4;
  /*_p20.COMO*/ bool DBG_COMO;
  /*_p20.BAGU*/ bool DBG_BAGU;
  /*_p20.BEFA*/ bool DBG_BEFA;
};

//-----------------------------------------------------------------------------
