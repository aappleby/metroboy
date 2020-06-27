#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct WindowSignals {
  /*p27.NOCU*/ bool NOCU_WIN_MODEn;
  /*p27.PORE*/ bool PORE_WIN_MODE;
  /*p27.NUNY*/ bool NUNY_WIN_MODE_TRIGp;
  /*p27.NYFO*/ bool NYFO_WIN_MODE_TRIGn;
  /*p27.MOSU*/ bool MOSU_WIN_MODE_TRIGp;

  /*p27.SYLO*/ bool SYLO_WIN_HITn;
  /*p24.TOMU*/ bool TOMU_WIN_HITp;
  /*p24.SOCY*/ bool SOCY_WIN_HITn;
  /*p27.TUKU*/ bool TUKU_WIN_HITn;
  /*p27.XOFO*/ bool XOFO_WIN_RSTp;

  /*p27.SEKO*/ bool SEKO_WIN_TRIGGER;

  /*p27.TUXY*/ bool TUXY;
  /*p27.SUZU*/ bool SUZU;

  /*p27.WYKA*/ bool WIN_X3;
  /*p27.WODY*/ bool WIN_X4;
  /*p27.WOBO*/ bool WIN_X5;
  /*p27.WYKO*/ bool WIN_X6;
  /*p27.XOLO*/ bool WIN_X7;

  /*p27.VYNO*/ bool WIN_Y0;
  /*p27.VUJO*/ bool WIN_Y1;
  /*p27.VYMU*/ bool WIN_Y2;
  /*p27.TUFU*/ bool WIN_Y3;
  /*p27.TAXA*/ bool WIN_Y4;
  /*p27.TOZO*/ bool WIN_Y5;
  /*p27.TATE*/ bool WIN_Y6;
  /*p27.TEKE*/ bool WIN_Y7;

};

//-----------------------------------------------------------------------------

struct WindowRegisters {

  WindowSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  int get_wy()   const { return pack(WY0, WY1, WY2, WY3, WY4, WY5, WY6, WY7); }
  int get_wx()   const { return pack(WX0, WX1, WX2, WX3, WX4, WX5, WX6, WX7); }

private:

  /*p27.PYNU*/ NorLatch PYNU_WIN_MODE_LATCH;
  /*p27.RYDY*/ NorLatch RYDY_WIN_HIT_LATCH;

  /*p27.WYKA*/ Reg WIN_X3;
  /*p27.WODY*/ Reg WIN_X4;
  /*p27.WOBO*/ Reg WIN_X5;
  /*p27.WYKO*/ Reg WIN_X6;
  /*p27.XOLO*/ Reg WIN_X7;

  /*p27.VYNO*/ Reg WIN_Y0;
  /*p27.VUJO*/ Reg WIN_Y1;
  /*p27.VYMU*/ Reg WIN_Y2;
  /*p27.TUFU*/ Reg WIN_Y3;
  /*p27.TAXA*/ Reg WIN_Y4;
  /*p27.TOZO*/ Reg WIN_Y5;
  /*p27.TATE*/ Reg WIN_Y6;
  /*p27.TEKE*/ Reg WIN_Y7;


  /*p27.NOPA*/ Reg NOPA_WIN_MODE_SYNC;
  /*p27.SOVY*/ Reg SOVY_WIN_HIT_SYNC;
  /*p27.REJO*/ NorLatch REJO_WY_MATCH_LATCH;
  /*p27.SARY*/ Reg SARY_WIN_MATCH_Y_SYNC;
  /*p27.RYFA*/ Reg RYFA_WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ Reg RENE_WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.PYCO*/ Reg PYCO_WIN_MATCH_SYNC1;
  /*p27.NUNU*/ Reg NUNU_WIN_MATCH_SYNC2;


  // FF4A - WY
  /*p23.NESO*/ Reg WY0;
  /*p23.NYRO*/ Reg WY1;
  /*p23.NAGA*/ Reg WY2;
  /*p23.MELA*/ Reg WY3;
  /*p23.NULO*/ Reg WY4;
  /*p23.NENE*/ Reg WY5;
  /*p23.NUKA*/ Reg WY6;
  /*p23.NAFU*/ Reg WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg WX0;
  /*p23.NOFE*/ Reg WX1;
  /*p23.NOKE*/ Reg WX2;
  /*p23.MEBY*/ Reg WX3;
  /*p23.MYPU*/ Reg WX4;
  /*p23.MYCE*/ Reg WX5;
  /*p23.MUVO*/ Reg WX6;
  /*p23.NUKU*/ Reg WX7;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics