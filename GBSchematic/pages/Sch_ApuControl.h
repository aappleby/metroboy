#pragma once
#include <stdint.h>
#pragma warning(disable:4201)

struct ApuControl {

  /*p16.ANUJ*/ bool CPU_WR_WEIRD;

  /*p09.CETO*/ bool CPU_RDa;
  /*p09.KAZO*/ bool CPU_RDb;
  /*p09.CURU*/ bool CPU_RDc;
  /*p09.GAXO*/ bool CPU_RDd;
  /*p09.KYDU*/ bool CPU_RD1;
  /*p09.AGUZ*/ bool CPU_RDn;

  /*p09.KEBA*/ bool APU_RESET1;
  /*p09.AGUR*/ bool APU_RESETn;
  /*p09.AFAT*/ bool APU_RESETn2;
  /*p09.ATYV*/ bool APU_RESETn3;
  /*p09.KEPY*/ bool APU_RESETn6;
  /*p09.KUBY*/ bool APU_RESETn7;
  
  //----------
  // FF24 NR50

  /*p09.BYMA*/ bool FF24n;

  /*p09.BEFU*/ bool NR50_RDn1;
  /*p09.ADAK*/ bool NR50_RD1;

  /*p09.BOSU*/ bool NR50_WRn1;
  /*p09.BUBU*/ bool NR50_WRn2;
  /*p09.ATAF*/ bool NR50_WRn3;
  /*p09.BAXY*/ bool NR50_WR1;
  /*p09.BOWE*/ bool NR50_WR2;

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

  //----------
  // FF25 NR51

  /*p09.GEPA*/ bool ADDR_FF25n;

  /*p09.GUMU*/ bool FF25_RD;
  /*p09.HEFA*/ bool FF25_RDn;

  /*p09.BUPO*/ bool FF25_WRn;
  /*p09.BONO*/ bool FF25_WRa;
  /*p09.BYFA*/ bool FF25_WRb;

  union {
    uint8_t NR51;
    struct {
      /*p09.ANEV*/ bool NR51_0 : 1;
      /*p09.BOGU*/ bool NR51_1 : 1;
      /*p09.BAFO*/ bool NR51_2 : 1;
      /*p09.ATUF*/ bool NR51_3 : 1;
      /*p09.BUME*/ bool NR51_4 : 1;
      /*p09.BOFA*/ bool NR51_5 : 1;
      /*p09.BEFO*/ bool NR51_6 : 1;
      /*p09.BEPU*/ bool NR51_7 : 1;
    };
  };

  //----------
  // FF26 NR52
  
  /*p09.DOLE*/ bool FF26_RDna;
  /*p09.KAMU*/ bool FF26_RDnb;
  /*p09.DURU*/ bool FF26_RDnd;
  /*p09.FEWA*/ bool FF26_RDnc;
  /*p09.JURE*/ bool NR52_RDn1;

  /*p09.ETUC*/ bool NR52_WR1;
  /*p09.HAWU*/ bool NR52_WRn1;
  /*p09.BOPY*/ bool NR52_WRn2;
  /*p09.FOKU*/ bool NR52_WRn3;

  /*p09.EFOP*/ bool DBG_APU_IN;
  /*p09.FERO*/ bool DBG_APUn; // secret debug bit
  /*p09.BOWY*/ bool DBG_SWEEP; // secret debug bit
  /*p09.HADA*/ bool ALL_SOUND_ON; //FF26_7
  /*p09.EDEK*/ bool DBG_APU;

  /*p10.TACE*/ bool AMP_ENn;
  /*p10.BAFU*/ bool CPU_WRn;
  /*p10.BOGY*/ bool APU_WR;

  /*p10.ATUP*/ bool ADDR_xxx0xxxx;
  /*p10.BOXY*/ bool ADDR_xx0xxxxx;
  /*p10.ASAD*/ bool ADDR_x0xxxxxx;
  /*p10.AVUN*/ bool ADDR_0xxxxxxx;

  /*p10.ATEG*/ bool ADDR_XX1Xn;
  /*p10.AWET*/ bool ADDR_XX2Xn;
  /*p10.ACOM*/ bool ADDR_XX3Xn;

  /*p10.BUNO*/ bool ADDR_FF1X;
  /*p10.CONA*/ bool ADDR_FF2X;
  /*p10.BARO*/ bool ADDR_FF3X;

  /*p10.BANU*/ bool ADDR_FF1Xn;
  /*p10.BEZY*/ bool ADDR_FF2Xn;

  /*p10.DYTE*/ bool ADDR_xxx0;
  /*p10.AFOB*/ bool ADDR_xx0x;
  /*p10.ABUB*/ bool ADDR_x0xx;
  /*p10.ACOL*/ bool ADDR_0xxx;
  /*p10.DOSO*/ bool ADDR_xxx1;
  /*p10.DUPA*/ bool ADDR_xx1x;
  /*p10.DENO*/ bool ADDR_x1xx;
  /*p10.DUCE*/ bool ADDR_1xxx;

  /*p10.DUPO*/ bool ADDR_0000an;
  /*p10.DUNO*/ bool ADDR_0001an;
  /*p10.DAMY*/ bool ADDR_0010an;
  /*p10.ETUF*/ bool ADDR_0011an;
  /*p10.ESOT*/ bool ADDR_0100an;
  /*p10.DAZA*/ bool ADDR_0110an;
  /*p10.DUVU*/ bool ADDR_0111an;
  /*p10.DAFY*/ bool ADDR_1000an;
  /*p10.DEJY*/ bool ADDR_1001an;
  /*p10.EXAT*/ bool ADDR_1010an;
  /*p10.EMAX*/ bool ADDR_1011an;
  /*p10.GANY*/ bool ADDR_1100an;
  /*p10.EMOS*/ bool ADDR_1101an;
  /*p10.EGEN*/ bool ADDR_1110an;

  /*p10.DONA*/ bool ADDR_0000bn;
  /*p10.DEWA*/ bool ADDR_0001bn;
  /*p10.DOFA*/ bool ADDR_0010bn;
  /*p10.DUFE*/ bool ADDR_0011bn;
  /*p10.DATU*/ bool ADDR_0100bn;
  /*p10.DURA*/ bool ADDR_0101bn;
  /*p10.EKAG*/ bool ADDR_0110bn;

  /*p10.DYVA*/ bool ADDR_FF10;
  /*p10.CAXE*/ bool ADDR_FF11;
  /*p10.EDAF*/ bool ADDR_FF12;
  /*p10.DECO*/ bool ADDR_FF13;
  /*p10.DUJA*/ bool ADDR_FF14;
  /*p10.COVY*/ bool ADDR_FF16;
  /*p10.DUTU*/ bool ADDR_FF17;
  /*p10.DARA*/ bool ADDR_FF18;
  /*p10.DOZA*/ bool ADDR_FF19;
  /*p10.EMOR*/ bool ADDR_FF1A;
  /*p10.DUSA*/ bool ADDR_FF1B;
  /*p10.GEFO*/ bool ADDR_FF1C;
  /*p10.FENY*/ bool ADDR_FF1D;
  /*p10.DUGO*/ bool ADDR_FF1E;

  /*p10.CAFY*/ bool ADDR_FF24;
  /*p10.CORA*/ bool ADDR_FF25;
  /*p10.DOXY*/ bool ADDR_FF26;
};
