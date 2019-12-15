#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct OAM {


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

  /*p28.BODE*/ bool OAM_LATCH;
  /*p28.ZODO*/ bool OAM_CLK;
  /*p28.ZONE*/ bool OAM_A_CS;
  /*p28.ZOFE*/ bool OAM_B_CS;


  /*p31.XYKY*/ bool OAM_A_LATCH0;
  /*p31.YRUM*/ bool OAM_A_LATCH1;
  /*p31.YSEX*/ bool OAM_A_LATCH2;
  /*p31.YVEL*/ bool OAM_A_LATCH3;
  /*p31.WYNO*/ bool OAM_A_LATCH4;
  /*p31.CYRA*/ bool OAM_A_LATCH5;
  /*p31.ZUVE*/ bool OAM_A_LATCH6;
  /*p31.ECED*/ bool OAM_A_LATCH7;

  /*p29.YDYV*/ bool OAM_B_LATCH0;
  /*p29.YCEB*/ bool OAM_B_LATCH1;
  /*p29.ZUCA*/ bool OAM_B_LATCH2;
  /*p29.WONE*/ bool OAM_B_LATCH3;
  /*p29.ZAXE*/ bool OAM_B_LATCH4;
  /*p29.XAFU*/ bool OAM_B_LATCH5;
  /*p29.YSES*/ bool OAM_B_LATCH6;
  /*p29.ZECA*/ bool OAM_B_LATCH7;

  /*p31.YLOR*/ Reg OAM_A_D0;
  /*p31.ZYTY*/ Reg OAM_A_D1;
  /*p31.ZYVE*/ Reg OAM_A_D2;
  /*p31.ZEZY*/ Reg OAM_A_D3;
  /*p31.GOMO*/ Reg OAM_A_D4;
  /*p31.BAXO*/ Reg OAM_A_D5;
  /*p31.YZOS*/ Reg OAM_A_D6;
  /*p31.DEPO*/ Reg OAM_A_D7;

  /*p29.XUSO*/ Reg OAM_B_D0;
  /*p29.XEGU*/ Reg OAM_B_D1;
  /*p29.YJEX*/ Reg OAM_B_D2;
  /*p29.XYJU*/ Reg OAM_B_D3;
  /*p29.YBOG*/ Reg OAM_B_D4;
  /*p29.WYSO*/ Reg OAM_B_D5;
  /*p29.XOTE*/ Reg OAM_B_D6;
  /*p29.YZAB*/ Reg OAM_B_D7;


  /*p31.ZAGO*/ bool OAM_A_D0b;
  /*p31.ZOCY*/ bool OAM_A_D1b;
  /*p31.YPUR*/ bool OAM_A_D2b;
  /*p31.YVOK*/ bool OAM_A_D3b;
  /*p31.COSE*/ bool OAM_A_D4b;
  /*p31.AROP*/ bool OAM_A_D5b;
  /*p31.XATU*/ bool OAM_A_D6b;
  /*p31.BADY*/ bool OAM_A_D7b;
};

//-----------------------------------------------------------------------------

};