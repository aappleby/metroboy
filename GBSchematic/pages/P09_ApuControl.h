#pragma once
#include "Common.h"

struct P09_ApuControl {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  /*p09.CETO*/ bool CPU_RDa;
  /*p09.KAZO*/ bool CPU_RDb;
  /*p09.CURU*/ bool CPU_RDc;
  /*p09.GAXO*/ bool CPU_RDd;
  /*p09.KYDU*/ bool CPU_RD1;
  /*p09.AGUZ*/ bool CPU_RDn;

  /*p09.DOLE*/ bool FF26_RDna;
  /*p09.KAMU*/ bool FF26_RDnb;
  /*p09.DURU*/ bool FF26_RDnd;
  /*p09.FEWA*/ bool FF26_RDnc;

  /*p09.COTO*/ bool FF26_D0;
  /*p09.KOGE*/ bool FF26_D1;
  /*p09.EFUS*/ bool FF26_D2;
  /*p09.FATE*/ bool FF26_D3;

  /*p09.KEBA*/ bool APU_RESET1;
  /*p09.AGUR*/ bool APU_RESETn;
  /*p09.AFAT*/ bool APU_RESETn2;
  /*p09.ATYV*/ bool APU_RESETn3;
  /*p09.DAPA*/ bool APU_RESETn4;
  /*p09.KAME*/ bool APU_RESETn5;
  /*p09.KEPY*/ bool APU_RESETn6;
  /*p09.KUBY*/ bool APU_RESETn7;
  
  /*p09.EDEK*/ bool DBG_APU;

  /*p09.BATA*/ bool BATA; // some 2m clock
  /*p09.CALO*/ bool CALO; // some 2m clock

  /*p09.CATE*/ bool CATE;

  union {
    uint8_t NR50;
    struct {
      /*p09.APEG*/ bool VOL_L0 : 1;
      /*p09.BYGA*/ bool VOL_L1 : 1;
      /*p09.AGER*/ bool VOL_L2 : 1;
      /*p09.APOS*/ bool VIN_TO_L : 1;
      /*p09.BYRE*/ bool VOL_R0 : 1;
      /*p09.BUMO*/ bool VOL_R1 : 1;
      /*p09.COZU*/ bool VOL_R2 : 1;
      /*p09.BEDU*/ bool VIN_TO_R : 1;
    };
  };

  union {
    uint8_t NR50_D;
    struct {
      /*p09.AKOD*/ bool NR50_D0 : 1;
      /*p09.AWED*/ bool NR50_D1 : 1;
      /*p09.AVUD*/ bool NR50_D2 : 1;
      /*p09.AXEM*/ bool NR50_D3 : 1;
      /*p09.AMAD*/ bool NR50_D4 : 1;
      /*p09.ARUX*/ bool NR50_D5 : 1;
      /*p09.BOCY*/ bool NR50_D6 : 1;
      /*p09.ATUM*/ bool NR50_D7 : 1;
    };
  };

  // FF25 NR51
  /*ANEV,BOGU,BAFO,ATUF,BUME,BOFA,BEFO,BEPU*/
  union {
    uint8_t NR51;
    struct {
      bool NR51_0 : 1;
      bool NR51_1 : 1;
      bool NR51_2 : 1;
      bool NR51_3 : 1;
      bool NR51_4 : 1;
      bool NR51_5 : 1;
      bool NR51_6 : 1;
      bool NR51_7 : 1;
    };
  };

  /*CAPU,CAGA,BOCA,BUZU,CERE,CADA,CAVU,CUDU*/
  union { 
    uint8_t NR51_D;
    struct {
      bool NR51_D0 : 1;
      bool NR51_D1 : 1;
      bool NR51_D2 : 1;
      bool NR51_D3 : 1;
      bool NR51_D4 : 1;
      bool NR51_D5 : 1;
      bool NR51_D6 : 1;
      bool NR51_D7 : 1;
    };
  };

  // FF26 NR52

  /*p09.HADA*/ bool ALL_SOUND_ON; //FF26_7

  /*p09.BEFU*/ bool NR50_RDn1;
  /*p09.ADAK*/ bool NR50_RD1;

  /*p09.BOSU*/ bool NR50_WRn1;
  /*p09.BUBU*/ bool NR50_WRn2;
  /*p09.ATAF*/ bool NR50_WRn3;
  /*p09.BAXY*/ bool NR50_WR1;
  /*p09.BOWE*/ bool NR50_WR2;
  /*p09.JURE*/ bool NR52_RDn1;
  /*p09.HAWU*/ bool NR52_WRn1;
  /*p09.BOPY*/ bool NR52_WRn2;
  /*p09.FOKU*/ bool NR52_WRn3;
  /*p09.ETUC*/ bool NR52_WR1;

  /*p09.FERO*/ bool NR52_4_SECRET; // secret debug bit
  /*p09.BOWY*/ bool NR52_5_SECRET; // secret debug bit

  /*p09.HOPE*/ bool FF26_D7;
  /*p09.BYMA*/ bool FF24n;

  //----------

  /*p09.EFOP*/ bool EFOP;
  /*p09.BAZA*/ bool BAZA;
  /*p09.CELY*/ bool CELY;
  /*p09.CONE*/ bool CONE;

  /*p09.BUPO*/ bool FF25_WRn;
  /*p09.BONO*/ bool FF25_WRa;
  /*p09.BYFA*/ bool FF25_WRb;
  /*p09.GEPA*/ bool ADDR_FF25n;
  /*p09.HEFA*/ bool FF25_RDn;
  /*p09.GUMU*/ bool FF25_RD;

};

