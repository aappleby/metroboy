/// plait_noparse

#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

struct ApuControl {

  /*_p09.JYRO*/ bool JYRO_APU_RST; // This is the root of the APU reset tree

  /*_p09.KEBA*/ bool APU_RESET1;
  /*_p10.TACE*/ bool AMP_ENn;
  /*_p10.BOGY*/ bool APU_WR;

  //----------
  // Address decoders

  /*_p07.BAKO*/ bool ADDR_FFXXn1;

  /*_p10.BANU*/ bool ADDR_FF1Xn;
  /*_p10.BEZY*/ bool ADDR_FF2Xn;

  /*_p10.DYTE*/ bool ADDR_xxx0;
  /*_p10.AFOB*/ bool ADDR_xx0x;
  /*_p10.ABUB*/ bool ADDR_x0xx;
  /*_p10.ACOL*/ bool ADDR_0xxx;
  /*_p10.DOSO*/ bool ADDR_xxx1;
  /*_p10.DUPA*/ bool ADDR_xx1x;
  /*_p10.DENO*/ bool ADDR_x1xx;
  /*_p10.DUCE*/ bool ADDR_1xxx;

  //----------
  // clocks for the apu

  /*_p01.CERY*/ bool CERY; // apu
  /*_p01.ATYK*/ bool ATYK;
  /*_p01.AVOK*/ bool AVOK;
  /*_p09.AJER*/ bool AJER_2M;
  /*_p01.COKE*/ bool COKE;
  /*_p01.BAVU*/ bool BAVU_1M;

  /*_p01.JESO*/ bool CLK_512Ka;
  /*_p01.HAMA*/ bool CLK_512Kn;
  /*_p01.HORU*/ bool CLK_512a;
  /*_p01.BUFY*/ bool CLK_256a;
  /*_p01.BYFE*/ bool CLK_128a;
  /*_p01.BURE*/ bool CLK_512n;
  /*_p01.BARA*/ bool CLK_512;
  /*_p01.CARU*/ bool CLK_256;
  /*_p01.BYLU*/ bool CLK_128;

  //----------
  // FF24 NR50

  /*_p09.BUBU*/ bool NR50_WRn2;
  /*_p09.ATAF*/ bool NR50_WRn3;

#pragma warning(push)
#pragma warning(disable:4201)

  union {
    uint8_t NR50;
    struct {
      /*_p09.APEG*/ bool NR50_VOL_L0 : 1;
      /*_p09.BYGA*/ bool NR50_VOL_L1 : 1;
      /*_p09.AGER*/ bool NR50_VOL_L2 : 1;
      /*_p09.APOS*/ bool NR50_VIN_TO_L : 1;
      /*_p09.BYRE*/ bool NR50_VOL_R0 : 1;
      /*_p09.BUMO*/ bool NR50_VOL_R1 : 1;
      /*_p09.COZU*/ bool NR50_VOL_R2 : 1;
      /*_p09.BEDU*/ bool NR50_VIN_TO_R : 1;
    };
  };

  //----------
  // FF25 NR51

  /*_p09.BUPO*/ bool NR51_WRn;
  /*_p09.BONO*/ bool NR51_WRa;
  /*_p09.BYFA*/ bool NR51_WRb;

  union {
    uint8_t NR51;
    struct {
      /*_p09.ANEV*/ bool NR51_0 : 1;
      /*_p09.BOGU*/ bool NR51_1 : 1;
      /*_p09.BAFO*/ bool NR51_2 : 1;
      /*_p09.ATUF*/ bool NR51_3 : 1;
      /*_p09.BUME*/ bool NR51_4 : 1;
      /*_p09.BOFA*/ bool NR51_5 : 1;
      /*_p09.BEFO*/ bool NR51_6 : 1;
      /*_p09.BEPU*/ bool NR51_7 : 1;
    };
  };

#pragma warning(pop)

  //----------
  // FF26 NR52

  /*_p09.HAWU*/ bool NR52_WRn1;
  /*_p09.BOPY*/ bool NR52_WRn2;
  /*_p09.FOKU*/ bool NR52_WRn3;

  /*_p09.FERO*/ bool NR52_DBG_APUn; // secret debug bit
  /*_p09.BOWY*/ bool NR52_DBG_SWEEP; // secret debug bit
  /*_p09.HADA*/ bool NR52_ALL_SOUND_ON; //FF26_7
  /*_p09.EDEK*/ bool NR52_DBG_APU;
};

//-----------------------------------------------------------------------------
