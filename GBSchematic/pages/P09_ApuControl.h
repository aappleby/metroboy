#pragma once
#include "Common.h"

struct P09_ApuControl {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool AJER; bool AJER_2M; };
  union { bool DYFA; bool DYFA_1M; };

  union { bool JYRO; bool APU_RESET2; };
  union { bool KEBA; bool APU_RESET1; };

  union { bool AGUR; bool APU_RESETn; };
  union { bool AFAT; bool APU_RESETn2; };
  union { bool ATYV; bool APU_RESETn3; };
  union { bool DAPA; bool APU_RESETn4; };
  union { bool KAME; bool APU_RESETn5; };
  union { bool KEPY; bool APU_RESETn6; };
  union { bool KUBY; bool APU_RESETn7; };


  union { bool AGUZ; bool CPU_RDn; };
  union { bool EDEK; bool NET03; };
  union { bool FERO; bool FERO_Q; };

  union { bool BATA; }; // some 2m clock
  union { bool CALO; }; // some 2m clock

  bool CATE;

//private:

  // FF24 NR50
  union {
    uint8_t NR50;

    // APEG,BYGA,AGER,APOS,BYRE,BUMO,COZU,BEDU;
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

  union {
    uint8_t NR50_D;

    // AKOD,AWED,AVUD,AXEM,AMAD,ARUX,BOCY,ATUM
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
  union {
    uint8_t NR51;

    // ANEV,BOGU,BAFO,ATUF,BUME,BOFA,BEFO,BEPU
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

  union { 
    uint8_t NR51_D;

    // CAPU,CAGA,BOCA,BUZU,CERE,CADA,CAVU,CUDU
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

  union { bool BOSU; bool NR50_WRn1; };
  union { bool BUBU; bool NR50_WRn2; };
  union { bool ATAF; bool NR50_WRn3; };

  union { bool BAXY; bool NR50_WR1; };
  union { bool BOWE; bool NR50_WR2; };

  union { bool JURE; bool NR52_RDn1; };

  union { bool HAWU; bool NR52_WRn1; };
  union { bool BOPY; bool NR52_WRn2; };
  union { bool FOKU; bool NR52_WRn3; };
  union { bool ETUC; bool NR52_WR1; };

  union { bool HADA; bool NR52_7; bool ALL_SOUND_ON; };

  union { bool KYDU; bool CPU_RD1; };
  union { bool BOWY; bool NR52_5; };  // secret bit?

  union { bool HOPE; bool FF26_D7; };

  union { bool BYMA; bool FF24n; };
  union { bool BEFU; bool NR50_RDn1; };
  union { bool ADAK; bool NR50_RD1; };



  //----------

  bool EFOP,BAZA,CELY,CONE;
  bool BUPO,BONO,BYFA,GEPA,HEFA,GUMU;

  bool CETO,KAZO,CURU,GAXO;
  bool DOLE,KAMU,DURU,FEWA;
  bool COTO,KOGE,EFUS,FATE;
};

