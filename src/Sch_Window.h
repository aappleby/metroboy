#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct WindowRegisters {

  void tick(SchematicTop& gb);
  SignalHash commit();

  int get_wy()   const { return pack(WY0, WY1, WY2, WY3, WY4, WY5, WY6, WY7); }
  int get_wx()   const { return pack(WX0, WX1, WX2, WX3, WX4, WX5, WX6, WX7); }

private:
  friend SchematicTop;

  /*p27.PYNU*/ NorLatch PYNU_WIN_MODE_TRIGA;
  /*p27.RYDY*/ NorLatch RYDY_WIN_HIT_LATCHp;

  /*p27.WYKA*/ Reg17 WIN_X3;
  /*p27.WODY*/ Reg17 WIN_X4;
  /*p27.WOBO*/ Reg17 WIN_X5;
  /*p27.WYKO*/ Reg17 WIN_X6;
  /*p27.XOLO*/ Reg17 WIN_X7;

  /*p27.VYNO*/ Reg17 WIN_Y0;
  /*p27.VUJO*/ Reg17 WIN_Y1;
  /*p27.VYMU*/ Reg17 WIN_Y2;
  /*p27.TUFU*/ Reg17 WIN_Y3;
  /*p27.TAXA*/ Reg17 WIN_Y4;
  /*p27.TOZO*/ Reg17 WIN_Y5;
  /*p27.TATE*/ Reg17 WIN_Y6;
  /*p27.TEKE*/ Reg17 WIN_Y7;


  /*p27.NOPA*/ Reg17 NOPA_WIN_MODE_TRIGB;
  /*p27.SOVY*/ Reg17 SOVY_WIN_HIT_SYNC;
  /*p27.REJO*/ NorLatch REJO_WY_MATCH_LATCH;
  /*p27.SARY*/ Reg17 SARY_WIN_MATCH_Y_SYNC;
  /*p27.RYFA*/ Reg17 RYFA_WIN_MATCH_TRIGA;
  /*p27.RENE*/ Reg17 RENE_WIN_MATCH_TRIGB;
  /*p27.PYCO*/ Reg17 PYCO_WIN_MATCH_SYNC1;
  /*p27.NUNU*/ Reg17 NUNU_WIN_MATCH_SYNC2;

  // FF4A - WY
  /*p23.NESO*/ Reg9 WY0;
  /*p23.NYRO*/ Reg9 WY1;
  /*p23.NAGA*/ Reg9 WY2;
  /*p23.MELA*/ Reg9 WY3;
  /*p23.NULO*/ Reg9 WY4;
  /*p23.NENE*/ Reg9 WY5;
  /*p23.NUKA*/ Reg9 WY6;
  /*p23.NAFU*/ Reg9 WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg9 WX0;
  /*p23.NOFE*/ Reg9 WX1;
  /*p23.NOKE*/ Reg9 WX2;
  /*p23.MEBY*/ Reg9 WX3;
  /*p23.MYPU*/ Reg9 WX4;
  /*p23.MYCE*/ Reg9 WX5;
  /*p23.MUVO*/ Reg9 WX6;
  /*p23.NUKU*/ Reg9 WX7;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics