#pragma once
#include "Schematics.h"

namespace Schematics {

struct Debug {

  /*p07.UMUT*/ bool MODE_DBG1;
  /*p07.UNOR*/ bool MODE_DBG2;
  /*p07.UPOJ*/ bool MODE_PROD;

  /*p05.KUKO*/ bool DBG_FF00_D6;
  /*p05.KERU*/ bool DBG_FF00_D7;
  /*p05.KORE*/ bool P05_NC0;
  /*p05.KYWE*/ bool P05_NC1;

  /*p07.APER*/ bool FF60_WRn;
  /*p07.BURO*/ bool FF60_0;
  /*p07.AMUT*/ bool FF60_1;

  /*p07.ROMY*/ bool DBG_0;
  /*p07.RYNE*/ bool DBG_1;
  /*p07.REJY*/ bool DBG_2;
  /*p07.RASE*/ bool DBG_3;
  /*p07.REKA*/ bool DBG_4;
  /*p07.ROWE*/ bool DBG_5;
  /*p07.RYKE*/ bool DBG_6;
  /*p07.RARU*/ bool DBG_7;

  /*p08.KOVA*/ bool A00_Cn; // Debug, drives external address onto internal address
  /*p08.CAMU*/ bool A01_Cn;
  /*p08.BUXU*/ bool A02_Cn;
  /*p08.BASE*/ bool A03_Cn;
  /*p08.AFEC*/ bool A04_Cn;
  /*p08.ABUP*/ bool A05_Cn;
  /*p08.CYGU*/ bool A06_Cn;
  /*p08.COGO*/ bool A07_Cn;
  /*p08.MUJY*/ bool A08_Cn;
  /*p08.NENA*/ bool A09_Cn;
  /*p08.SURA*/ bool A10_Cn;
  /*p08.MADY*/ bool A11_Cn;
  /*p08.LAHE*/ bool A12_Cn;
  /*p08.LURA*/ bool A13_Cn;
  /*p08.PEVO*/ bool A14_Cn;
  /*p08.RAZA*/ bool A15_Cn;

  /*p08.KEJO*/ bool A00_C;
  /*p08.BYXE*/ bool A01_C;
  /*p08.AKAN*/ bool A02_C;
  /*p08.ANAR*/ bool A03_C;
  /*p08.AZUV*/ bool A04_C;
  /*p08.AJOV*/ bool A05_C;
  /*p08.BYNE*/ bool A06_C;
  /*p08.BYNA*/ bool A07_C;
  /*p08.LOFA*/ bool A08_C;
  /*p08.MAPU*/ bool A09_C;
  /*p08.RALA*/ bool A10_C;
  /*p08.LORA*/ bool A11_C;
  /*p08.LYNA*/ bool A12_C;
  /*p08.LEFY*/ bool A13_C;
  /*p08.NEFE*/ bool A14_C;
  /*p08.SYZU*/ bool A15_C;
};

};