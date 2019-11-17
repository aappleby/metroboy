#pragma once

// Noise
// Length counter
// Env vol / dir / period
// Clock shift
// LFSR mode
// LFSR divisor
// START
// STOP

struct Channel4 {
  void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  //----------
  // Clock/reset

  /*p20.ALOP*/ bool CLK_128n;

  /*p09.KUBY*/ bool RSTn;
  /*p09.KEBA*/ bool RSTa;
  /*p09.DAPA*/ bool RSTo;
  /*p09.KAME*/ bool RSTp;
  /*p19.FEXO*/ bool RSTq;
  /*p19.HYNE*/ bool RSTr;
  /*p19.CABE*/ bool RSTs;
  /*p20.FEBY*/ bool RSTt;
  /*p20.GASO*/ bool RSTu;

  /*p19.KAGE*/ bool CPU_RDa;
  /*p19.BYLO*/ bool CPU_RDb;
  /*p20.COSA*/ bool CPU_RDc;

  //----------
  // FF20 NR41 ch4 length

  /*p10.DANU*/ bool FF20a;
  /*p19.FURU*/ bool FF20_WRa;
  /*p19.DOTU*/ bool FF20_WRb;
  /*p19.EPEK*/ bool FF20_WRc;
  /*p19.CAZE*/ bool FF20_WRn;

  /*p19.DANO*/ bool NR41_LEN0;
  /*p19.FAVY*/ bool NR41_LEN1;
  /*p19.DENA*/ bool NR41_LEN2;
  /*p19.CEDO*/ bool NR41_LEN3;
  /*p19.DOPU*/ bool NR41_LEN3b;
  /*p19.FYLO*/ bool NR41_LEN4;
  /*p19.EDOP*/ bool NR41_LEN5;

  //----------
  // FF21 NR42 ch4 envelope

  /*p10.COVO*/ bool FF21a;
  /*p19.BOFY*/ bool FF21n;
  /*p19.GONY*/ bool FF21o;
  /*p19.BOXE*/ bool FF21_RDa;
  /*p19.HASU*/ bool FF21_RDb;
  /*p19.DACO*/ bool FF21_WRa;
  /*p19.GOKO*/ bool FF21_WRb;
  /*p19.DYKE*/ bool FF21_WRn;
  /*p19.FUPA*/ bool FF21_WRo;

  /*p19.EMOK*/ bool NR42_ENV_TIMER0;
  /*p19.ETYJ*/ bool NR42_ENV_TIMER1;
  /*p19.EZYK*/ bool NR42_ENV_TIMER2;
  /*p19.GEKY*/ bool NR42_ENV_DIR;
  /*p19.GARU*/ bool NR42_ENV_VOL0;
  /*p19.GOKY*/ bool NR42_ENV_VOL1;
  /*p19.GOZO*/ bool NR42_ENV_VOL2;
  /*p19.GEDU*/ bool NR42_ENV_VOL3;

  //----------
  // FF22 NR43 ch4 freq

  /*p10.EKEZ*/ bool FF22a;
  /*p19.KOKU*/ bool FF22n;
  /*p19.GUGO*/ bool FF22o;
  /*p19.KEKA*/ bool FF22_RDa;
  /*p19.JORA*/ bool FF22_RDb;
  /*p19.HEZE*/ bool FF22_RDc;
  /*p19.HUMO*/ bool FF22_WRa;
  /*p19.GETU*/ bool FF22_WRn;
  /*p19.HOVA*/ bool FF22_WRo;
  /*p19.HOSO*/ bool FF22_WRp;
  /*p19.EFUG*/ bool FF22_WRq;

  /*p19.JARE*/ bool NR43_DIV0;
  /*p19.JERO*/ bool NR43_DIV1;
  /*p19.JAKY*/ bool NR43_DIV2;
  /*p19.JAMY*/ bool NR43_MODE;
  /*p19.FETA*/ bool NR43_FREQ0;
  /*p19.FYTO*/ bool NR43_FREQ1;
  /*p19.GOGO*/ bool NR43_FREQ2;
  /*p19.GAFO*/ bool NR43_FREQ3;

  //---------
  // FF23

  /*p10.CUGE*/ bool FF23a;
  /*p19.BARE*/ bool FF23_RDa;
  /*p19.DULU*/ bool FF23_WRn;
  /*p19.FOXE*/ bool FF23_WRo;

  /*p19.CUNY*/ bool NR44_STOP;
  /*p19.HOGA*/ bool NR44_START;
  /*p20.CEPY*/ bool NR44_STOPn;
  /*p20.GUZY*/ bool NR44_START_RST;

  //----------
  // Control

  /*p20.GYSU*/ bool CH4_START;
  /*p20.EFOT*/ bool CH4_STOP;
  /*p20.FEGY*/ bool CH4_OFF;
  /*p20.GENA*/ bool CH4_ACTIVE;
  /*p20.JUWA*/ bool CH4_ACTIVEn;
  
  /*p20.FALE*/ bool RESTART_RSTn;
  /*p20.HELU*/ bool RESTART_RST;
  /*p20.HAZO*/ bool RESTART_IN;

  /*p20.GONE*/ bool RESTART1;
  /*p20.GORA*/ bool RESTART2;

  /*p20.GATY*/ bool RESTART3;
  /*p20.HERY*/ bool DIV_GATE1;
  /*p20.HAPU*/ bool DIV_GATE2;
  /*p20.JERY*/ bool DIV_GATE3;
  /*p20.KYKU*/ bool DIV_CLKb;

  /*p20.KONY*/ bool DIV_CLKn;
  /*p20.KANU*/ bool DIV_CLKa;
  /*p20.GOFU*/ bool DIV_LOADn;
  /*p20.HUCE*/ bool DIV_LOAD;

  /*p20.JYCO*/ bool DIV0;
  /*p20.JYRE*/ bool DIV1;
  /*p20.JYFU*/ bool DIV2;
  /*p20.HYNO*/ bool DIV_MAX;

  //----------
  // Length

  /*p19.GAPY*/ bool LEN_RSTn;
  /*p19.FUGO*/ bool LEN_STOP;
  /*p19.DODA*/ bool LEN_CLKn;
  /*p19.CUWA*/ bool LEN_CLKa;

  //----------
  // Frequency timer

  /*p20.GYBA*/ bool FREQ_GATE_CLK;
  /*p20.GUNY*/ bool FREQ_GATE_RSTn;
  /*p20.GARY*/ bool FREQ_GATEn;
  /*p20.CARY*/ bool FREQ_CLK;
  /*p20.CEXO*/ bool FREQ_00;
  /*p20.DEKO*/ bool FREQ_01;
  /*p20.EZEF*/ bool FREQ_02;
  /*p20.EPOR*/ bool FREQ_03;
  /*p20.DURE*/ bool FREQ_04;
  /*p20.DALE*/ bool FREQ_05;
  /*p20.DOKE*/ bool FREQ_06;
  /*p20.DEMO*/ bool FREQ_07;
  /*p20.DOSE*/ bool FREQ_08;
  /*p20.DETE*/ bool FREQ_09;
  /*p20.ERUT*/ bool FREQ_10;
  /*p20.DOTA*/ bool FREQ_11;
  /*p20.DERE*/ bool FREQ_12;
  /*p20.ESEP*/ bool FREQ_13;

  //----------
  // 14-1 mux

  /*p20.ETOV*/ bool LFSR_CLK_MUX_0;
  /*p20.EVER*/ bool LFSR_CLK_MUX_1;
  /*p20.ETAR*/ bool LFSR_CLK_MUX_2;
  /*p20.FURA*/ bool LFSR_CLK_MUX_3;
  /*p20.ETAT*/ bool LFSR_CLK_MUX_4;
  /*p20.DUDU*/ bool LFSR_CLK_MUX_5;
  /*p20.ELAR*/ bool LFSR_CLK_MUX_6;
  /*p20.EMOF*/ bool LFSR_CLK_MUX_7;
  /*p20.ETYR*/ bool LFSR_CLK_MUX_A;
  /*p20.ELYX*/ bool LFSR_CLK_MUX_B;
  /*p20.DARY*/ bool LFSR_CLK_MUX_C;
  /*p20.ERYF*/ bool LFSR_CLK_MUX_D;

  //----------
  // LFSR

  /*p20.FEME*/ bool LFSR_CLKa;
  /*p20.GYVE*/ bool LFSR_CLKb;
  /*p20.KOPA*/ bool LFSR_CLKc;
  /*p20.JYJA*/ bool LFSR_CLKn;
  /*p20.GUFA*/ bool LFSR_CLKo;
  /*p20.KARA*/ bool LFSR_CLKp;

  /*p20.GEPO*/ bool LFSR_RSTa;
  /*p20.GOGE*/ bool LFSR_RSTn;

  /*p20.HURA*/ bool LFSR_IN;
  /*p20.JOTO*/ bool LFSR_00;
  /*p20.KOMU*/ bool LFSR_01;
  /*p20.KETU*/ bool LFSR_02;
  /*p20.KUTA*/ bool LFSR_03;
  /*p20.KUZY*/ bool LFSR_04;
  /*p20.KYWY*/ bool LFSR_05;
  /*p20.JAJU*/ bool LFSR_06;
  /*p20.HAPE*/ bool LFSR_07;
  /*p20.JUXE*/ bool LFSR_08;

  /*p20.KAVU*/ bool LFSR_FB;
  /*p20.JEPE*/ bool LFSR_09;
  /*p20.JAVO*/ bool LFSR_10;
  /*p20.HEPA*/ bool LFSR_11;
  /*p20.HORY*/ bool LFSR_12;
  /*p20.HENO*/ bool LFSR_13;
  /*p20.HYRO*/ bool LFSR_14;
  /*p20.HEZU*/ bool LFSR_15;

  //----------
  // Env

  /*p20.FOWA*/ bool ENV_OFF;

  /*p20.BOKY*/ bool ENV_TIMER_CLK_RST;
  /*p20.ABEL*/ bool ENV_TIMER_CLKa;
  /*p20.BAWA*/ bool ENV_TIMER_CLKn;
  /*p20.BUXO*/ bool ENV_TIMER_CLKb;
  /*p20.ENEC*/ bool ENV_TIMER_LOADn;
  /*p20.DAPY*/ bool ENV_TIMER_LOAD;
  /*p20.CUNA*/ bool ENV_TIMER0;
  /*p20.COFE*/ bool ENV_TIMER1;
  /*p20.DOGO*/ bool ENV_TIMER2;
  /*p20.EJEX*/ bool ENV_TIMER_MAX;

  /*p20.HURY*/ bool ENV_PULSE_RST1;
  /*p20.GOPA*/ bool ENV_PULSE_RST2;

  /*p20.FOSY*/ bool ENV_PULSE;
  /*p20.GEXE*/ bool ENV_PULSEn;

  /*p20.EMET*/ bool ENV_STOP_RST;
  /*p20.FYNO*/ bool ENV_STOP;

  /*p20.ENUR*/ bool ENV_CLK1;
  /*p20.EROX*/ bool ENV_CLK2;
  /*p20.FELO*/ bool ENV_CLK;

  /*p20.FOLE*/ bool VOL_CLK0;
  /*p20.ETEF*/ bool VOL_CLK1;
  /*p20.EDYF*/ bool VOL_CLK2;
  /*p20.ELAF*/ bool VOL_CLK3;
  
  /*p20.FEKO*/ bool VOL0;
  /*p20.FATY*/ bool VOL1;
  /*p20.FERU*/ bool VOL2;
  /*p20.FYRO*/ bool VOL3;
  
  /*p20.CUTY*/ bool ENV_TOPn;
  /*p20.DARO*/ bool ENV_BOT;
  /*p20.DUBO*/ bool ENV_TOPa;
  /*p20.EVUR*/ bool ENV_MAX;

  //----------
  // Output

  /*p20.GAME*/ bool LFSR_OUT;
  /*p20.EZUL*/ bool CH4_BIT_MUX;
  /*p20.COTE*/ bool DBG_CH4_MUTE;
  /*p20.DATO*/ bool CH4_RAW_BIT;

  /*p20.GEVY*/ bool CH4_AMP_ENn;
  /*p20.AKOF*/ bool CH4_DAC0;
  /*p20.BYZY*/ bool CH4_DAC1;
  /*p20.APYR*/ bool CH4_DAC2;
  /*p20.BOZA*/ bool CH4_DAC3;

  //----------
  // Debug

  /*p20.DYRY*/ bool DBG_CH4;
  /*p20.COMO*/ bool DBG_COMO;
  /*p20.BAGU*/ bool DBG_BAGU;
  /*p20.BEFA*/ bool DBG_BEFA;
};

