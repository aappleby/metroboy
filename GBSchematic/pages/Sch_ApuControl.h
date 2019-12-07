#pragma once
#include <stdint.h>
#pragma warning(disable:4201)

namespace Schematics {

//-----------------------------------------------------------------------------

struct ApuControl {

  /*p16.ANUJ*/ bool CPU_WR_WEIRD;

  /*p09.JYRO*/ bool APU_RST; // This is the root of the APU reset tree
    
  /*p09.KEBA*/ bool APU_RESET1;

  //----------
  // clocks for the apu

  /*p01.AZOF*/ bool CLK_xBxDxFxH4; // apu
  /*p01.APUV*/ bool CLK_AxCxExGx1; // apu
  /*p01.ARYF*/ bool CLK_AxCxExGx2; // apu
  /*p01.CYBO*/ bool CLK_AxCxExGx7; // apu
  /*p01.AMUK*/ bool CLK_xBxDxFxH1; // apu
  /*p01.CERY*/ bool CLK_ABxxEFxx1; // apu
  /*p01.ATAG*/ bool CLK_AxCxExGx8; // apu

  /*p01.ATYK*/ bool ATYK;
  /*p01.AVOK*/ bool AVOK;

  /*p09.AJER*/ bool AJER_2M;
  /*p01.COKE*/ bool AJER_2Mn;
  /*p01.BAVU*/ bool BAVU_1M;

  /*p01.JESO*/ bool CLK_512Ka;
  /*p01.HAMA*/ bool CLK_512Kn;

  /*p01.HORU*/ bool CLK_512a;
  /*p01.BUFY*/ bool CLK_256a;
  /*p01.BYFE*/ bool CLK_128a;

  /*p01.BURE*/ bool CLK_512n;

  /*p01.BARA*/ bool CLK_512;
  /*p01.CARU*/ bool CLK_256;
  /*p01.BYLU*/ bool CLK_128;

  //----------
  // FF24 NR50

  /*p09.BEFU*/ bool NR50_RDn1;
  /*p09.ADAK*/ bool NR50_RD1;
  /*p09.BUBU*/ bool NR50_WRn2;
  /*p09.ATAF*/ bool NR50_WRn3;
  /*p09.BAXY*/ bool NR50_WR1;
  /*p09.BOWE*/ bool NR50_WR2;

  union {
    uint8_t NR50;
    struct {
      /*p09.APEG*/ bool NR50_VOL_L0 : 1;
      /*p09.BYGA*/ bool NR50_VOL_L1 : 1;
      /*p09.AGER*/ bool NR50_VOL_L2 : 1;
      /*p09.APOS*/ bool NR50_VIN_TO_L : 1;
      /*p09.BYRE*/ bool NR50_VOL_R0 : 1;
      /*p09.BUMO*/ bool NR50_VOL_R1 : 1;
      /*p09.COZU*/ bool NR50_VOL_R2 : 1;
      /*p09.BEDU*/ bool NR50_VIN_TO_R : 1;
    };
  };

  //----------
  // FF25 NR51

  /*p09.GUMU*/ bool NR51_RD;
  /*p09.HEFA*/ bool NR51_RDn;
  /*p09.BUPO*/ bool NR51_WRn;
  /*p09.BONO*/ bool NR51_WRa;
  /*p09.BYFA*/ bool NR51_WRb;

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
  
  /*p09.ETUC*/ bool NR52_WR1;
  /*p09.HAWU*/ bool NR52_WRn1;
  /*p09.BOPY*/ bool NR52_WRn2;
  /*p09.FOKU*/ bool NR52_WRn3;

  /*p09.FERO*/ bool NR52_DBG_APUn; // secret debug bit
  /*p09.BOWY*/ bool NR52_DBG_SWEEP; // secret debug bit
  /*p09.HADA*/ bool NR52_ALL_SOUND_ON; //FF26_7
  /*p09.EDEK*/ bool NR52_DBG_APU;

  //----------

  /*p10.TACE*/ bool AMP_ENn;
  /*p10.BOGY*/ bool APU_WR;

  //----------

  /*p07.BAKO*/ bool ADDR_FFXXn1;

  /*p09.GEPA*/ bool ADDR_FF25n;

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

  /*p10.CORA*/ bool ADDR_FF25;
  /*p10.DOXY*/ bool ADDR_FF26;
};

//-----------------------------------------------------------------------------

};