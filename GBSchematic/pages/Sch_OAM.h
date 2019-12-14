#pragma once
#include "Schematics.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct OAM {

  bool OAM_CLK;

  /*p28.GEKA*/ bool OAM_A0;
  /*p28.ZYFO*/ bool OAM_A1;
  /*p28.YFOT*/ bool OAM_A2;
  /*p28.YFOC*/ bool OAM_A3;
  /*p28.YVOM*/ bool OAM_A4;
  /*p28.YMEV*/ bool OAM_A5;
  /*p28.XEMU*/ bool OAM_A6;
  /*p28.YZET*/ bool OAM_A7;

  bool TS_OAM_A_D0;
  bool TS_OAM_A_D1;
  bool TS_OAM_A_D2;
  bool TS_OAM_A_D3;
  bool TS_OAM_A_D4;
  bool TS_OAM_A_D5;
  bool TS_OAM_A_D6;
  bool TS_OAM_A_D7;

  bool TS_OAM_B_D0;
  bool TS_OAM_B_D1;
  bool TS_OAM_B_D2;
  bool TS_OAM_B_D3;
  bool TS_OAM_B_D4;
  bool TS_OAM_B_D5;
  bool TS_OAM_B_D6;
  bool TS_OAM_B_D7;

  /*p25.COTA*/ bool OAM_IN_CLKn;
};

//-----------------------------------------------------------------------------

};