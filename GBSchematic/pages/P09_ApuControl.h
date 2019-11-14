#pragma once
#include "Common.h"

struct P09_ApuControl {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  /*P09.KEBA*/ bool APU_RESET1;
  /*P09.AGUR*/ bool APU_RESETn;
  /*P09.AFAT*/ bool APU_RESETn2;
  /*P09.ATYV*/ bool APU_RESETn3;
  /*P09.DAPA*/ bool APU_RESETn4;
  /*P09.KAME*/ bool APU_RESETn5;
  /*P09.KEPY*/ bool APU_RESETn6;
  /*P09.KUBY*/ bool APU_RESETn7;
  
  /*P09.AGUZ*/ bool CPU_RDn;
  /*P09.EDEK*/ bool NET03;
  /*P09.FERO*/ bool FERO;

  /*P09.BATA*/ bool BATA; // some 2m clock
  /*P09.CALO*/ bool CALO; // some 2m clock

  /*P09.CATE*/ bool CATE;

//private:

  // FF24 NR50
  /*APEG,BYGA,AGER,APOS,BYRE,BUMO,COZU,BEDU*/
  union {
    uint8_t NR50;
    struct {
      bool NR50_0 : 1;
      bool NR50_1 : 1;
      bool NR50_2 : 1;
      bool NR50_3 : 1;
      bool NR50_4 : 1;
      bool NR50_5 : 1;
      bool NR50_6 : 1;
      bool NR50_7 : 1;
    };
  };

  /*AKOD,AWED,AVUD,AXEM,AMAD,ARUX,BOCY,ATUM*/
  union {
    uint8_t NR50_D;
    struct {
      bool NR50_D0 : 1;
      bool NR50_D1 : 1;
      bool NR50_D2 : 1;
      bool NR50_D3 : 1;
      bool NR50_D4 : 1;
      bool NR50_D5 : 1;
      bool NR50_D6 : 1;
      bool NR50_D7 : 1;
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

  /*P09.HADA*/ union { bool NR52_7; bool ALL_SOUND_ON; };
  /*P09.KYDU*/ bool CPU_RD1;

  /*P09.BEFU*/ bool NR50_RDn1;
  /*P09.ADAK*/ bool NR50_RD1;

  /*P09.BOSU*/ bool NR50_WRn1;
  /*P09.BUBU*/ bool NR50_WRn2;
  /*P09.ATAF*/ bool NR50_WRn3;
  /*P09.BAXY*/ bool NR50_WR1;
  /*P09.BOWE*/ bool NR50_WR2;
  /*P09.JURE*/ bool NR52_RDn1;
  /*P09.HAWU*/ bool NR52_WRn1;
  /*P09.BOPY*/ bool NR52_WRn2;
  /*P09.FOKU*/ bool NR52_WRn3;
  /*P09.ETUC*/ bool NR52_WR1;
  /*P09.BOWY*/ bool NR52_5; // secret bit?
  /*P09.HOPE*/ bool FF26_D7;
  /*P09.BYMA*/ bool FF24n;

  //----------

  bool EFOP,BAZA,CELY,CONE;
  bool BUPO,BONO,BYFA,GEPA,HEFA,GUMU;

  bool CETO,KAZO,CURU,GAXO;
  bool DOLE,KAMU,DURU,FEWA;
  bool COTO,KOGE,EFUS,FATE;
};

