#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct WindowSignals {
  /*p27.NOCU*/ Signal NOCU_WIN_MODEn;       // to tilefetcher map read, 
  /*p27.NUNY*/ Signal NUNY_WIN_MODE_TRIGp;  // to NYXU_TILE_FETCHER_RSTn
  /*p27.SYLO*/ Signal SYLO_WIN_HITn;        // to ppu.TYFA_CLKPIPEp_xBxDxFxH, ppu.TEKY_SPRITE_FETCH
  /*p27.SEKO*/ Signal SEKO_WIN_MATCH_TRIGp; // to TEVO_FINE_RSTp, which I don't completely understand
  /*p27.SUZU*/ Signal SUZU;                 // to TEVO_FINE_RSTp

  /*p27.WYKA*/ Signal WIN_X3;
  /*p27.WODY*/ Signal WIN_X4;
  /*p27.WOBO*/ Signal WIN_X5;
  /*p27.WYKO*/ Signal WIN_X6;
  /*p27.XOLO*/ Signal WIN_X7;

  /*p27.VYNO*/ Signal WIN_Y0;
  /*p27.VUJO*/ Signal WIN_Y1;
  /*p27.VYMU*/ Signal WIN_Y2;
  /*p27.TUFU*/ Signal WIN_Y3;
  /*p27.TAXA*/ Signal WIN_Y4;
  /*p27.TOZO*/ Signal WIN_Y5;
  /*p27.TATE*/ Signal WIN_Y6;
  /*p27.TEKE*/ Signal WIN_Y7;
};

//-----------------------------------------------------------------------------

struct WindowRegisters {

  WindowSignals sig(const SchematicTop& gb) const;
  void tick(SchematicTop& gb);
  SignalHash commit();

  int get_wy()   const { return pack(WY0, WY1, WY2, WY3, WY4, WY5, WY6, WY7); }
  int get_wx()   const { return pack(WX0, WX1, WX2, WX3, WX4, WX5, WX6, WX7); }

private:

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


  /*p27.NOPA*/ Reg NOPA_WIN_MODE_TRIGB;
  /*p27.SOVY*/ Reg SOVY_WIN_HIT_SYNC;
  /*p27.REJO*/ NorLatch REJO_WY_MATCH_LATCH;
  /*p27.SARY*/ Reg SARY_WIN_MATCH_Y_SYNC;
  /*p27.RYFA*/ Reg RYFA_WIN_MATCH_TRIGA;
  /*p27.RENE*/ Reg RENE_WIN_MATCH_TRIGB;
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