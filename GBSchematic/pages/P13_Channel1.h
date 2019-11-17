#pragma once

struct Gameboy;

struct P13_Channel1 {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

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

  union { bool GEXU; };
  union { bool FEKU; };
  union { bool BUSO; };
  union { bool BOJE; };
  union { bool ADAD; };
  union { bool KYLY; };
  union { bool KALA; };
  union { bool EGOR; };

  /*p13.BACY*/ bool NR11_LEN0;
  /*p13.CAVY*/ bool NR11_LEN1;
  /*p13.BOVY*/ bool NR11_LEN2;
  /*p13.CUNO*/ bool NR11_LEN3;
  /*p13.CUSO*/ bool NR11_LEN3n;
  /*p13.CURA*/ bool NR11_LEN4;
  /*p13.ERAM*/ bool NR11_LEN5;
    
  /*p13.CERO*/ bool CH1_LEN_DONE;

  bool CAXY,CYPU,CUPO,BEXA;

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
  bool COPA,CAJA,BYRA,BYTE;

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

