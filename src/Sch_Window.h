#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct WindowRegisters {

  void tick(SchematicTop& gb);
  void tock(SchematicTop& gb);
  SignalHash commit();

private:
  friend SchematicTop;

  wire ROGE_WY_MATCHp(const SchematicTop& top) const;
  wire NUKO_WX_MATCHp(const SchematicTop& top) const;

  /*p27.NUNY*/ Signal NUNY_WX_MATCHpe;
  /*p27.SEKO*/ Signal SEKO_WX_MATCHne;
  /*p27.SUZU*/ Signal SUZU_WIN_FIRST_TILEne;

  /*p27.PYNU*/ NorLatch PYNU_WIN_MODE_A;
  /*p27.RYDY*/ NorLatch RYDY_WIN_FIRST_TILE_A;

  /*p27.NOPA*/ Reg17 _NOPA_WIN_MODE_B;
  /*p27.SOVY*/ Reg17 _SOVY_WIN_FIRST_TILE_B;
  /*p27.REJO*/ NorLatch _REJO_WY_MATCH_LATCH;
  /*p27.SARY*/ Reg17 _SARY_WY_MATCH;
  /*p27.RYFA*/ Reg17 _RYFA_WX_MATCHn_A;
  /*p27.RENE*/ Reg17 _RENE_WX_MATCHn_B;
  /*p27.PYCO*/ Reg17 _PYCO_WX_MATCH_A;
  /*p27.NUNU*/ Reg17 _NUNU_WX_MATCH_B;

  /*p27.WYKA*/ Reg17 WYKA_WIN_X3;
  /*p27.WODY*/ Reg17 WODY_WIN_X4;
  /*p27.WOBO*/ Reg17 WOBO_WIN_X5;
  /*p27.WYKO*/ Reg17 WYKO_WIN_X6;
  /*p27.XOLO*/ Reg17 XOLO_WIN_X7;

  /*p27.VYNO*/ Reg17 VYNO_WIN_Y0;
  /*p27.VUJO*/ Reg17 VUJO_WIN_Y1;
  /*p27.VYMU*/ Reg17 VYMU_WIN_Y2;
  /*p27.TUFU*/ Reg17 TUFU_WIN_Y3;
  /*p27.TAXA*/ Reg17 TAXA_WIN_Y4;
  /*p27.TOZO*/ Reg17 TOZO_WIN_Y5;
  /*p27.TATE*/ Reg17 TATE_WIN_Y6;
  /*p27.TEKE*/ Reg17 TEKE_WIN_Y7;

  // FF4A - WY
  /*p23.NESO*/ Reg9 NESO_WY0;
  /*p23.NYRO*/ Reg9 NYRO_WY1;
  /*p23.NAGA*/ Reg9 NAGA_WY2;
  /*p23.MELA*/ Reg9 MELA_WY3;
  /*p23.NULO*/ Reg9 NULO_WY4;
  /*p23.NENE*/ Reg9 NENE_WY5;
  /*p23.NUKA*/ Reg9 NUKA_WY6;
  /*p23.NAFU*/ Reg9 NAFU_WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg9 MYPA_WX0;
  /*p23.NOFE*/ Reg9 NOFE_WX1;
  /*p23.NOKE*/ Reg9 NOKE_WX2;
  /*p23.MEBY*/ Reg9 MEBY_WX3;
  /*p23.MYPU*/ Reg9 MYPU_WX4;
  /*p23.MYCE*/ Reg9 MYCE_WX5;
  /*p23.MUVO*/ Reg9 MUVO_WX6;
  /*p23.NUKU*/ Reg9 NUKU_WX7;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics