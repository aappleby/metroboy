#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct WindowRegisters {

  void tick(SchematicTop& gb);
  void tock(SchematicTop& gb);
  SignalHash commit();

  wire MOSU_WIN_MODE_TRIGp() const {
    /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(NUNY_WX_MATCHpe());
    /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not(NYFO_WIN_MODE_TRIGn);
    return MOSU_WIN_MODE_TRIGp;
  }

  /*p27.NOCU*/ wire NOCU_WIN_MODEn() const { return not(_PYNU_WIN_MODE_A.q()); }
  /*p27.PORE*/ wire PORE_WIN_MODEp() const { return not(NOCU_WIN_MODEn()); }
  /*p24.TOMU*/ wire TOMU_WIN_HITp()  const { return not(SYLO_WIN_HITn()); }

  /*p27.WYKA*/ wire WYKA_WIN_X3() const { return _WYKA_WIN_X3.q(); }
  /*p27.WODY*/ wire WODY_WIN_X4() const { return _WODY_WIN_X4.q(); }
  /*p27.WOBO*/ wire WOBO_WIN_X5() const { return _WOBO_WIN_X5.q(); }
  /*p27.WYKO*/ wire WYKO_WIN_X6() const { return _WYKO_WIN_X6.q(); }
  /*p27.XOLO*/ wire XOLO_WIN_X7() const { return _XOLO_WIN_X7.q(); }

  /*p27.VYNO*/ wire VYNO_WIN_Y0() const { return _VYNO_WIN_Y0.q(); }
  /*p27.VUJO*/ wire VUJO_WIN_Y1() const { return _VUJO_WIN_Y1.q(); }
  /*p27.VYMU*/ wire VYMU_WIN_Y2() const { return _VYMU_WIN_Y2.q(); }
  /*p27.TUFU*/ wire TUFU_WIN_Y3() const { return _TUFU_WIN_Y3.q(); }
  /*p27.TAXA*/ wire TAXA_WIN_Y4() const { return _TAXA_WIN_Y4.q(); }
  /*p27.TOZO*/ wire TOZO_WIN_Y5() const { return _TOZO_WIN_Y5.q(); }
  /*p27.TATE*/ wire TATE_WIN_Y6() const { return _TATE_WIN_Y6.q(); }
  /*p27.TEKE*/ wire TEKE_WIN_Y7() const { return _TEKE_WIN_Y7.q(); }

  /*p27.SEKO*/ Signal SEKO_WX_MATCHne;
  /*p27.SUZU*/ Signal SUZU_WIN_FIRST_TILEne;

private:

  /*p27.NUNY*/ wire NUNY_WX_MATCHpe() const { return and(_PYNU_WIN_MODE_A.q(), _NOPA_WIN_MODE_B.qn()); }
  /*p27.SYLO*/ wire SYLO_WIN_HITn()   const { return not(_RYDY_WIN_FIRST_TILE_A.q()); }

  wire ROGE_WY_MATCHp(const SchematicTop& top) const;
  wire NUKO_WX_MATCHp(const SchematicTop& top) const;

  /*p27.PYNU*/ NorLatch _PYNU_WIN_MODE_A;
  /*p27.RYDY*/ NorLatch _RYDY_WIN_FIRST_TILE_A;

  /*p27.NOPA*/ Reg17 _NOPA_WIN_MODE_B;
  /*p27.SOVY*/ Reg17 _SOVY_WIN_FIRST_TILE_B;
  /*p27.REJO*/ NorLatch _REJO_WY_MATCH_LATCH;
  /*p27.SARY*/ Reg17 _SARY_WY_MATCH;
  /*p27.RYFA*/ Reg17 _RYFA_WX_MATCHn_A;
  /*p27.RENE*/ Reg17 _RENE_WX_MATCHn_B;
  /*p27.PYCO*/ Reg17 _PYCO_WX_MATCH_A;
  /*p27.NUNU*/ Reg17 _NUNU_WX_MATCH_B;

  /*p27.WYKA*/ Reg17 _WYKA_WIN_X3;
  /*p27.WODY*/ Reg17 _WODY_WIN_X4;
  /*p27.WOBO*/ Reg17 _WOBO_WIN_X5;
  /*p27.WYKO*/ Reg17 _WYKO_WIN_X6;
  /*p27.XOLO*/ Reg17 _XOLO_WIN_X7;

  /*p27.VYNO*/ Reg17 _VYNO_WIN_Y0;
  /*p27.VUJO*/ Reg17 _VUJO_WIN_Y1;
  /*p27.VYMU*/ Reg17 _VYMU_WIN_Y2;
  /*p27.TUFU*/ Reg17 _TUFU_WIN_Y3;
  /*p27.TAXA*/ Reg17 _TAXA_WIN_Y4;
  /*p27.TOZO*/ Reg17 _TOZO_WIN_Y5;
  /*p27.TATE*/ Reg17 _TATE_WIN_Y6;
  /*p27.TEKE*/ Reg17 _TEKE_WIN_Y7;

  // FF4A - WY
  /*p23.NESO*/ Reg9 _NESO_WY0;
  /*p23.NYRO*/ Reg9 _NYRO_WY1;
  /*p23.NAGA*/ Reg9 _NAGA_WY2;
  /*p23.MELA*/ Reg9 _MELA_WY3;
  /*p23.NULO*/ Reg9 _NULO_WY4;
  /*p23.NENE*/ Reg9 _NENE_WY5;
  /*p23.NUKA*/ Reg9 _NUKA_WY6;
  /*p23.NAFU*/ Reg9 _NAFU_WY7;

  // FF4B - WX
  /*p23.MYPA*/ Reg9 _MYPA_WX0;
  /*p23.NOFE*/ Reg9 _NOFE_WX1;
  /*p23.NOKE*/ Reg9 _NOKE_WX2;
  /*p23.MEBY*/ Reg9 _MEBY_WX3;
  /*p23.MYPU*/ Reg9 _MYPU_WX4;
  /*p23.MYCE*/ Reg9 _MYCE_WX5;
  /*p23.MUVO*/ Reg9 _MUVO_WX6;
  /*p23.NUKU*/ Reg9 _NUKU_WX7;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics