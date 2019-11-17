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

  /*p20.COSA*/ bool CPU_RD;
  /*p19.KAGE*/ bool CPU_RDa;
  /*p19.BYLO*/ bool CPU_RDb;
  /*p20.ALOP*/ bool CLK_128n;

  //----------
  // FF20 NR41 ch4 length

  /*p19.FURU*/ bool FF20_WRa;
  /*p19.DOTU*/ bool FF20_WRb;
  /*p19.EPEK*/ bool FF20_WRc;
  /*p19.CAZE*/ bool FF20_WRn;

  /*p19.DANO*/ bool NR41_LEN_0;
  /*p19.FAVY*/ bool NR41_LEN_1;
  /*p19.DENA*/ bool NR41_LEN_2;
  /*p19.CEDO*/ bool NR41_LEN_3;
  /*p19.DOPU*/ bool NR41_LEN_3b;
  /*p19.FYLO*/ bool NR41_LEN_4;
  /*p19.EDOP*/ bool NR41_LEN_5;

  /*p19.GAPY*/ bool CH4_LEN_RST;
  /*p19.FUGO*/ bool CH4_LEN_STOP;

  /*p19.DODA*/ bool CH4_LEN_CLKn;
  /*p19.CUWA*/ bool CH4_LEN_CLKa;

  //----------
  // FF21 NR42 ch4 envelope

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
  /*p19.GARU*/ bool NR42_ENV_VOL_0;
  /*p19.GOKY*/ bool NR42_ENV_VOL_1;
  /*p19.GOZO*/ bool NR42_ENV_VOL_2;
  /*p19.GEDU*/ bool NR42_ENV_VOL_3;

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

  /*p19.JARE*/ bool NR43_DIV_0;
  /*p19.JERO*/ bool NR43_DIV_1;
  /*p19.JAKY*/ bool NR43_DIV_2;
  /*p19.JAMY*/ bool NR43_MODE;
  /*p19.FETA*/ bool NR43_FREQ_0;
  /*p19.FYTO*/ bool NR43_FREQ_1;
  /*p19.GOGO*/ bool NR43_FREQ_2;
  /*p19.GAFO*/ bool NR43_FREQ_3;

  //---------
  // FF23

  /*p19.BARE*/ bool FF23_RD;
  /*p19.DULU*/ bool FF23_WRn;
  /*p19.FOXE*/ bool FF23_WRo;

  /*p19.CUNY*/ bool NR44_STOP;
  /*p19.HOGA*/ bool NR44_START;
  /*p20.CEPY*/ bool NR44_STOPn;
  /*p20.GUZY*/ bool NR44_START_RST;

  //----------
  // Control

  /*p20.JUWA*/ bool CH4_ACTIVEn;
  /*p20.GEVY*/ bool CH4_AMP_ENn;
  /*p20.DATO*/ bool CH4_RAW_BIT;
  /*p20.FOWA*/ bool CH4_ENV_OFF;
  /*p20.FOSY*/ bool CH4_EG_TICK;
  /*p20.GEXE*/ bool CH4_EG_TICKn;
  /*p20.GONE*/ bool CH4_RESTART;
  /*p20.GYSU*/ bool CH4_START;
  /*p19.FEXO*/ bool FEXO; // reset
  /*p19.HYNE*/ bool HYNE; // reset
  /*p19.CABE*/ bool CABE; // reset
  /*p20.EFOT*/ bool CH4_STOP;
  /*p20.FEGY*/ bool CH4_OFF;
  /*p20.GENA*/ bool CH4_ACTIVE;

  //----------
  // LFSR

  /*p20.FEME*/ bool LFSR_CLKa;
  /*p20.GYVE*/ bool LFSR_CLKb;
  /*p20.KOPA*/ bool LFSR_CLKc;
  /*p20.JYJA*/ bool LFSR_CLKn;
  /*p20.GUFA*/ bool LFSR_CLKo;
  /*p20.KARA*/ bool LFSR_CLKp;

  /*p20.HURA*/ bool LFSR_IN;
  /*p20.KAVU*/ bool LFSR_FB;

  /*p20.GEPO*/ bool LFSR_RSTa;
  /*p20.GOGE*/ bool LFSR_RSTn;

  /*p20.JOTO*/ bool LFSR_00;
  /*p20.KOMU*/ bool LFSR_01;
  /*p20.KETU*/ bool LFSR_02;
  /*p20.KUTA*/ bool LFSR_03;
  /*p20.KUZY*/ bool LFSR_04;
  /*p20.KYWY*/ bool LFSR_05;
  /*p20.JAJU*/ bool LFSR_06;
  /*p20.HAPE*/ bool LFSR_07;
  /*p20.JUXE*/ bool LFSR_08;

  /*p20.JEPE*/ bool LFSR_09;
  /*p20.JAVO*/ bool LFSR_10;
  /*p20.HEPA*/ bool LFSR_11;
  /*p20.HORY*/ bool LFSR_12;
  /*p20.HENO*/ bool LFSR_13;
  /*p20.HYRO*/ bool LFSR_14;
  /*p20.HEZU*/ bool LFSR_15;

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


  /*p20.GAME*/ bool LFSR_OUT;

  //----------
  // Env

  /*p20.EZUL*/ bool CH4_BIT_MUX;
  
  /*p20.FELO*/ bool CH4_ENV_CLK;
  /*p20.FOLE*/ bool CH4_ENV_CLK0;
  /*p20.ETEF*/ bool CH4_ENV_CLK1;
  /*p20.EDYF*/ bool CH4_ENV_CLK2;
  /*p20.ELAF*/ bool CH4_ENV_CLK3;
  
  /*p20.FEKO*/ bool CH4_ENV0;
  /*p20.FATY*/ bool CH4_ENV1;
  /*p20.FERU*/ bool CH4_ENV2;
  /*p20.FYRO*/ bool CH4_ENV3;
  
  /*p20.CARY*/ bool CH4_FREQ_CLK;
  /*p20.CUTY*/ bool CH4_ENV_TOPn;
  /*p20.DARO*/ bool CH4_ENV_BOT;
  /*p20.DUBO*/ bool CH4_ENV_TOPa;
  /*p20.EVUR*/ bool CH4_ENV_MAX;

  /*p20.ABEL*/ bool CH4_ENV_TIMER_CLKa;
  /*p20.BAWA*/ bool CH4_ENV_TIMER_CLKn;
  /*p20.BOKY*/ bool CH4_ENV_TIMER_CLK_RST;
  /*p20.BUXO*/ bool CH4_ENV_TIMER_CLKb;
  /*p20.ENEC*/ bool CH4_ENV_TIMER_RST;
  /*p20.DAPY*/ bool CH4_ENV_TIMER_RSTn;
  /*p20.CUNA*/ bool CH4_ENV_TIMER0;
  /*p20.COFE*/ bool CH4_ENV_TIMER1;
  /*p20.DOGO*/ bool CH4_ENV_TIMER2;
  /*p20.EJEX*/ bool CH4_ENV_TIMER_TICK;

  //----------
  // Output

  /*p20.AKOF*/ bool CH4_DAC0;
  /*p20.BYZY*/ bool CH4_DAC1;
  /*p20.APYR*/ bool CH4_DAC2;
  /*p20.BOZA*/ bool CH4_DAC3;

  //----------
  // Debug

  /*p20.DYRY*/ bool DBG_CH4;
  /*p20.COTE*/ bool DBG_CH4_MUTE;
  /*p20.COMO*/ bool DBG_COMO;
  /*p20.BAGU*/ bool DBG_BAGU;
  /*p20.BEFA*/ bool DBG_BEFA;

  //----------
  // Unsorted signals

  /*p20.EMET*/ bool EMET;
  /*p20.ENUR*/ bool ENUR;
  /*p20.EROX*/ bool EROX;
  /*p20.FALE*/ bool FALE;
  /*p20.FEBY*/ bool FEBY;
  /*p20.FYNO*/ bool FYNO;
  /*p20.GARY*/ bool GARY;
  /*p20.GASO*/ bool GASO;
  /*p20.GATY*/ bool GATY;
  /*p20.GOFU*/ bool GOFU;
  /*p20.GOPA*/ bool GOPA;
  /*p20.GORA*/ bool GORA;
  /*p20.GUNY*/ bool GUNY;
  /*p20.GYBA*/ bool GYBA;
  /*p20.HAPU*/ bool HAPU;
  /*p20.HAZO*/ bool HAZO;
  /*p20.HELU*/ bool HELU;
  /*p20.HERY*/ bool HERY;
  /*p20.HUCE*/ bool HUCE;
  /*p20.HURY*/ bool HURY;
  /*p20.HYNO*/ bool HYNO;
  /*p20.JERY*/ bool JERY;
  /*p20.JYCO*/ bool JYCO;
  /*p20.JYFU*/ bool JYFU;
  /*p20.JYRE*/ bool JYRE;
  /*p20.KANU*/ bool KANU;
  /*p20.KONY*/ bool KONY;
  /*p20.KYKU*/ bool KYKU;
};

