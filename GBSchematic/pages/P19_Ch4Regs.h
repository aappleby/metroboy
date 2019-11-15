#pragma once

struct P19_Ch4Regs {
public:

  /*p19.FEXO*/ bool FEXO; // reset
  /*p19.HYNE*/ bool HYNE; // reset
  /*p19.CABE*/ bool CABE; // reset

  //----------
  // FF20 NR41 ch4 length

  /*p19.DANO*/ bool CH4_LEN_0;
  /*p19.FAVY*/ bool CH4_LEN_1;
  /*p19.DENA*/ bool CH4_LEN_2;
  /*p19.CEDO*/ bool CH4_LEN_3;
  /*p19.FYLO*/ bool CH4_LEN_4;
  /*p19.EDOP*/ bool CH4_LEN_5;

  /*p19.DOPU*/ bool CH4_LEN_3b;

  /*p19.GAPY*/ bool CH4_LEN_RST;
  /*p19.FUGO*/ bool CH4_DONE;

  /*p19.CAZE*/ bool FF20_WRn;
  
  /*p19.FURU*/ bool FF20_WRa;
  /*p19.DOTU*/ bool FF20_WRb;
  /*p19.EPEK*/ bool FF20_WRc;

  /*p19.DODA*/ bool CH4_LEN_CLKn;
  /*p19.CUWA*/ bool CH4_LEN_CLK;

  //----------
  // FF21 NR42 ch4 envelope

  /*p19.DACO*/ bool FF21_WRa;
  /*p19.GOKO*/ bool FF21_WRb;
  /*p19.DYKE*/ bool FF21_WRna;
  /*p19.FUPA*/ bool FF21_WRnb;

  /*p19.BOFY*/ bool ADDR_FF21na;
  /*p19.GONY*/ bool ADDR_FF21nb;
  /*p19.BOXE*/ bool FF21_RDa;
  /*p19.HASU*/ bool FF21_RDb;

  /*p19.EMOK*/ bool CH4_ENV_SWEEP_0;
  /*p19.ETYJ*/ bool CH4_ENV_SWEEP_1;
  /*p19.EZYK*/ bool CH4_ENV_SWEEP_2;
  /*p19.GEKY*/ bool CH4_ENV_DIR;
  /*p19.GARU*/ bool CH4_ENV_VOL_0;
  /*p19.GOKY*/ bool CH4_ENV_VOL_1;
  /*p19.GOZO*/ bool CH4_ENV_VOL_2;
  /*p19.GEDU*/ bool CH4_ENV_VOL_3;

  //----------
  // FF22 NR43 ch4 freq

  /*p19.HUMO*/ bool FF22_WRa;
  /*p19.GETU*/ bool FF22_WRna;

  /*p19.HOVA*/ bool FF22_WRnb;
  /*p19.HOSO*/ bool FF22_WRnc;
  /*p19.EFUG*/ bool FF22_WRnd;

  /*p19.KOKU*/ bool ADDR_FF22na;
  /*p19.KAGE*/ bool CPU_RDa;
  /*p19.GUGO*/ bool ADDR_FF22nb;

  /*p19.KEKA*/ bool FF22_RDa;
  /*p19.JORA*/ bool FF22_RDb;
  /*p19.HEZE*/ bool FF22_RDc;

  /*p19.JARE*/ bool CH4_DIV_0;
  /*p19.JERO*/ bool CH4_DIV_1;
  /*p19.JAKY*/ bool CH4_DIV_2;
  /*p19.JAMY*/ bool CH4_MODE;
  /*p19.FETA*/ bool CH4_FREQ_0;
  /*p19.FYTO*/ bool CH4_FREQ_1;
  /*p19.GOGO*/ bool CH4_FREQ_2;
  /*p19.GAFO*/ bool CH4_FREQ_3;

  //---------
  // FF23

  /*p19.DULU*/ bool FF23_WRna;
  /*p19.FOXE*/ bool FF23_WRnb;

  /*p19.BYLO*/ bool CPU_RDb;
  /*p19.BARE*/ bool FF23_RD;

  /*p19.CUNY*/ bool CH4_STOP;
  /*p19.HOGA*/ bool CH4_START;
};

