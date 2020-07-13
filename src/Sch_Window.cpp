#include "Sch_Window.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void WindowRegisters::tick(SchematicTop& top) {
  _SUVU_PORCH_ENDn = top.tile_fetcher.SUVU_PORCH_ENDn();

  {
    // This trigger fires on the pixel _after_ WX. Not sure what the fine count is about.
    /*p27.PANY*/ wire _PANY_WX_MATCHn = nor(NUKO_WX_MATCHp(top), top.ppu_reg.ROZE_FINE_COUNT_7n());
    /*p27.RYFA*/ _RYFA_WX_MATCHn_A.set(top.SEGU_CLKPIPEn(), top.ppu_reg.XYMU_RENDERINGp(), _PANY_WX_MATCHn);
    /*p27.RENE*/ _RENE_WX_MATCHn_B.set(top.clk_reg.ALET_xBxDxFxH(), top.ppu_reg.XYMU_RENDERINGp(), _RYFA_WX_MATCHn_A.q());
    /*p27.SEKO*/ SEKO_WX_MATCHne = nor(_RYFA_WX_MATCHn_A.qn(), _RENE_WX_MATCHn_B.q());
  }

  {
    /*p27.TUXY*/ wire _TUXY_WIN_FIRST_TILE_NE = nand(SYLO_WIN_HITn(), _SOVY_WIN_FIRST_TILE_B.q());
    /*p27.SUZU*/ SUZU_WIN_FIRST_TILEne = not(_TUXY_WIN_FIRST_TILE_NE);
  }
}

//------------------------------------------------------------------------------

void WindowRegisters::tock(SchematicTop& top) {

  /*p27.XAHY*/ wire _XAHY_VID_LINE_TRIG_d4n = not(top.lcd_reg.ATEJ_VID_LINE_TRIG_d4p());
  /*p27.XOFO*/ wire _XOFO_WIN_RSTp = nand(top.WYMO_LCDC_WINEN.q(), _XAHY_VID_LINE_TRIG_d4n, top.rst_reg.XAPO_VID_RSTn());

  /*p27.REPU*/ wire _REPU_VBLANK_RSTp = or(top.lcd_reg.PARU_VBLANKp_d4(), top.rst_reg.PYRY_VID_RSTp());

  {
    /*p27.SARY*/ _SARY_WY_MATCH.set(top.clk_reg.TALU_ABCDxxxx(), top.rst_reg.XAPO_VID_RSTn(), ROGE_WY_MATCHp(top));
    /*p27.REJO*/ _REJO_WY_MATCH_LATCH.nor_latch(_SARY_WY_MATCH.q(), _REPU_VBLANK_RSTp);
  }

  {
    // This trigger fires on the pixel _at_ WX
    /*p27.ROCO*/ wire _ROCO_CLKPIPEp = not(top.SEGU_CLKPIPEn());
    /*p27.PYCO*/ _PYCO_WX_MATCH_A.set(_ROCO_CLKPIPEp, top.rst_reg.XAPO_VID_RSTn(), NUKO_WX_MATCHp(top));
    /*p27.NUNU*/ _NUNU_WX_MATCH_B.set(top.clk_reg.MEHE_AxCxExGx(), top.rst_reg.XAPO_VID_RSTn(), _PYCO_WX_MATCH_A.q());

    /*p27.PYNU*/ _PYNU_WIN_MODE_A.nor_latch(_NUNU_WX_MATCH_B.q(), _XOFO_WIN_RSTp);
    /*p27.NOPA*/ _NOPA_WIN_MODE_B.set(top.clk_reg.ALET_xBxDxFxH(), top.rst_reg.XAPO_VID_RSTn(), _PYNU_WIN_MODE_A.q());
  }

  {
    // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BFETCH_DONE_SYNC_DELAY) is RESET.
    ///*p27.PUKU*/ PUKU = nor(RYDY, WIN_MODE_TRIG);
    ///*p27.RYDY*/ RYDY = nor(PUKU, rst_reg.VID_RESET4, BFETCH_DONE_SYNC_DELAY);

    /*p27.RYDY*/ _RYDY_WIN_FIRST_TILE_A.nor_latch(NUNY_WX_MATCHpe(), top.rst_reg.PYRY_VID_RSTp() || top.tile_fetcher.PORY_TILE_FETCH_DONE_Bp());
    /*p27.SOVY*/ _SOVY_WIN_FIRST_TILE_B.set(top.clk_reg.ALET_xBxDxFxH(), top.rst_reg.XAPO_VID_RSTn(), _RYDY_WIN_FIRST_TILE_A.q());
  }

  // window x coordinate
  {
    // something weird here, PORE doesn't look like a clock

    /*p27.VETU*/ wire _VETU_WIN_MAP_CLK = and (TEVO_FINE_RSTp(), PORE_WIN_MODEp());
    /*p27.XACO*/ wire _XACO_WIN_RSTn = not(_XOFO_WIN_RSTp);

    /*p27.WYKA*/ _WYKA_WIN_X3.set(_VETU_WIN_MAP_CLK,  _XACO_WIN_RSTn, _WYKA_WIN_X3.qn());
    /*p27.WODY*/ _WODY_WIN_X4.set(_WYKA_WIN_X3.qn(),  _XACO_WIN_RSTn, _WODY_WIN_X4.qn());
    /*p27.WOBO*/ _WOBO_WIN_X5.set(_WODY_WIN_X4.qn(),  _XACO_WIN_RSTn, _WOBO_WIN_X5.qn());
    /*p27.WYKO*/ _WYKO_WIN_X6.set(_WOBO_WIN_X5.qn(),  _XACO_WIN_RSTn, _WYKO_WIN_X6.qn());
    /*p27.XOLO*/ _XOLO_WIN_X7.set(_WYKO_WIN_X6.qn(),  _XACO_WIN_RSTn, _XOLO_WIN_X7.qn());
  }

  // window y coordinate
  // every time we leave win mode we increment win_y
  {
    /*p27.WAZY*/ wire _WAZY_WIN_Y_CLK = not(PORE_WIN_MODEp());
    /*p27.SYNY*/ wire _SYNY_VBLANK_RSTn = not(_REPU_VBLANK_RSTp);

    /*p27.VYNO*/ _VYNO_WIN_Y0.set(_WAZY_WIN_Y_CLK,   _SYNY_VBLANK_RSTn, _VYNO_WIN_Y0.qn());
    /*p27.VUJO*/ _VUJO_WIN_Y1.set(_VYNO_WIN_Y0.qn(), _SYNY_VBLANK_RSTn, _VUJO_WIN_Y1.qn());
    /*p27.VYMU*/ _VYMU_WIN_Y2.set(_VUJO_WIN_Y1.qn(), _SYNY_VBLANK_RSTn, _VYMU_WIN_Y2.qn());
    /*p27.TUFU*/ _TUFU_WIN_Y3.set(_VYMU_WIN_Y2.qn(), _SYNY_VBLANK_RSTn, _TUFU_WIN_Y3.qn());
    /*p27.TAXA*/ _TAXA_WIN_Y4.set(_TUFU_WIN_Y3.qn(), _SYNY_VBLANK_RSTn, _TAXA_WIN_Y4.qn());
    /*p27.TOZO*/ _TOZO_WIN_Y5.set(_TAXA_WIN_Y4.qn(), _SYNY_VBLANK_RSTn, _TOZO_WIN_Y5.qn());
    /*p27.TATE*/ _TATE_WIN_Y6.set(_TOZO_WIN_Y5.qn(), _SYNY_VBLANK_RSTn, _TATE_WIN_Y6.qn());
    /*p27.TEKE*/ _TEKE_WIN_Y7.set(_TATE_WIN_Y6.qn(), _SYNY_VBLANK_RSTn, _TEKE_WIN_Y7.qn());
  }

  // FF4A
  {
    /*p22.WYVO*/ wire _FF4An = nand(top.WERO_FF4Xp(), top.XOLA_A00n(), top.WESA_A01p(), top.XUSY_A02n(), top.WEPO_A03p());
    /*p22.VYGA*/ wire _FF4Ap = not(_FF4An);

    /*p23.WAXU*/ wire _FF4A_RD = and (_FF4Ap, top.ASOT_CPU_RDp());
    /*p23.VOMY*/ wire _FF4A_RDn = not(_FF4A_RD);

    /*p23.WEKO*/ wire _WEKO_FF4A_WRn = and (_FF4Ap, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.VEFU*/ wire _VEFU_FF4A_WRp = not(_WEKO_FF4A_WRn);

    /*p23.NESO*/ _NESO_WY0.set(_VEFU_FF4A_WRp, !_VEFU_FF4A_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D0.q());
    /*p23.NYRO*/ _NYRO_WY1.set(_VEFU_FF4A_WRp, !_VEFU_FF4A_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D1.q());
    /*p23.NAGA*/ _NAGA_WY2.set(_VEFU_FF4A_WRp, !_VEFU_FF4A_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D2.q());
    /*p23.MELA*/ _MELA_WY3.set(_VEFU_FF4A_WRp, !_VEFU_FF4A_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D3.q());
    /*p23.NULO*/ _NULO_WY4.set(_VEFU_FF4A_WRp, !_VEFU_FF4A_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D4.q());
    /*p23.NENE*/ _NENE_WY5.set(_VEFU_FF4A_WRp, !_VEFU_FF4A_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D5.q());
    /*p23.NUKA*/ _NUKA_WY6.set(_VEFU_FF4A_WRp, !_VEFU_FF4A_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D6.q());
    /*p23.NAFU*/ _NAFU_WY7.set(_VEFU_FF4A_WRp, !_VEFU_FF4A_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D7.q());


    /*p23.PUNU*/ top.int_bus.INT_TRI_D0.set_tribuf_6n(_FF4A_RDn, _NESO_WY0.q());
    /*p23.PODA*/ top.int_bus.INT_TRI_D1.set_tribuf_6n(_FF4A_RDn, _NYRO_WY1.q());
    /*p23.PYGU*/ top.int_bus.INT_TRI_D2.set_tribuf_6n(_FF4A_RDn, _NAGA_WY2.q());
    /*p23.LOKA*/ top.int_bus.INT_TRI_D3.set_tribuf_6n(_FF4A_RDn, _MELA_WY3.q());
    /*p23.MEGA*/ top.int_bus.INT_TRI_D4.set_tribuf_6n(_FF4A_RDn, _NULO_WY4.q());
    /*p23.PELA*/ top.int_bus.INT_TRI_D5.set_tribuf_6n(_FF4A_RDn, _NENE_WY5.q());
    /*p23.POLO*/ top.int_bus.INT_TRI_D6.set_tribuf_6n(_FF4A_RDn, _NUKA_WY6.q());
    /*p23.MERA*/ top.int_bus.INT_TRI_D7.set_tribuf_6n(_FF4A_RDn, _NAFU_WY7.q());
  }

  // FF4B
  {
    /*p22.WAGE*/ wire _FF4Bn = nand(top.WERO_FF4Xp(), top.WADO_A00p(), top.WESA_A01p(), top.XUSY_A02n(), top.WEPO_A03p());
    /*p22.VUMY*/ wire _FF4Bp = not(_FF4Bn);

    /*p23.WYZE*/ wire _FF4B_RDp = and (_FF4Bp, top.ASOT_CPU_RDp());
    /*p23.VYCU*/ wire _FF4B_RDn = not(_FF4B_RDp);

    /*p23.WUZA*/ wire _WUZA_FF4B_WRn = and (_FF4Bp, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.VOXU*/ wire _VOXU_FF4B_WRp = not(_WUZA_FF4B_WRn);

    /*p23.MYPA*/ _MYPA_WX0.set(_VOXU_FF4B_WRp, !_VOXU_FF4B_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D0.q());
    /*p23.NOFE*/ _NOFE_WX1.set(_VOXU_FF4B_WRp, !_VOXU_FF4B_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D1.q());
    /*p23.NOKE*/ _NOKE_WX2.set(_VOXU_FF4B_WRp, !_VOXU_FF4B_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D2.q());
    /*p23.MEBY*/ _MEBY_WX3.set(_VOXU_FF4B_WRp, !_VOXU_FF4B_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D3.q());
    /*p23.MYPU*/ _MYPU_WX4.set(_VOXU_FF4B_WRp, !_VOXU_FF4B_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D4.q());
    /*p23.MYCE*/ _MYCE_WX5.set(_VOXU_FF4B_WRp, !_VOXU_FF4B_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D5.q());
    /*p23.MUVO*/ _MUVO_WX6.set(_VOXU_FF4B_WRp, !_VOXU_FF4B_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D6.q());
    /*p23.NUKU*/ _NUKU_WX7.set(_VOXU_FF4B_WRp, !_VOXU_FF4B_WRp, top.rst_reg.WALU_SYS_RSTn(), top.int_bus.INT_TRI_D7.q());


    /*p23.LOVA*/ top.int_bus.INT_TRI_D0.set_tribuf_6n(_FF4B_RDn, _MYPA_WX0.q());
    /*p23.MUKA*/ top.int_bus.INT_TRI_D1.set_tribuf_6n(_FF4B_RDn, _NOFE_WX1.q());
    /*p23.MOKO*/ top.int_bus.INT_TRI_D2.set_tribuf_6n(_FF4B_RDn, _NOKE_WX2.q());
    /*p23.LOLE*/ top.int_bus.INT_TRI_D3.set_tribuf_6n(_FF4B_RDn, _MEBY_WX3.q());
    /*p23.MELE*/ top.int_bus.INT_TRI_D4.set_tribuf_6n(_FF4B_RDn, _MYPU_WX4.q());
    /*p23.MUFE*/ top.int_bus.INT_TRI_D5.set_tribuf_6n(_FF4B_RDn, _MYCE_WX5.q());
    /*p23.MULY*/ top.int_bus.INT_TRI_D6.set_tribuf_6n(_FF4B_RDn, _MUVO_WX6.q());
    /*p23.MARA*/ top.int_bus.INT_TRI_D7.set_tribuf_6n(_FF4B_RDn, _NUKU_WX7.q());
  }
}

//------------------------------------------------------------------------------

wire WindowRegisters::ROGE_WY_MATCHp(const SchematicTop& top) const {
  /*p27.NAZE*/ wire _WY_MATCH0 = xnor(top.lcd_reg.MUWY_Y0.q(), _NESO_WY0.q());
  /*p27.PEBO*/ wire _WY_MATCH1 = xnor(top.lcd_reg.MYRO_Y1.q(), _NYRO_WY1.q());
  /*p27.POMO*/ wire _WY_MATCH2 = xnor(top.lcd_reg.LEXA_Y2.q(), _NAGA_WY2.q());
  /*p27.NEVU*/ wire _WY_MATCH3 = xnor(top.lcd_reg.LYDO_Y3.q(), _MELA_WY3.q());
  /*p27.NOJO*/ wire _WY_MATCH4 = xnor(top.lcd_reg.LOVU_Y4.q(), _NULO_WY4.q());
  /*p27.PAGA*/ wire _WY_MATCH5 = xnor(top.lcd_reg.LEMA_Y5.q(), _NENE_WY5.q());
  /*p27.PEZO*/ wire _WY_MATCH6 = xnor(top.lcd_reg.MATO_Y6.q(), _NUKA_WY6.q());
  /*p27.NUPA*/ wire _WY_MATCH7 = xnor(top.lcd_reg.LAFO_Y7.q(), _NAFU_WY7.q());

  /*p27.PALO*/ wire _WY_MATCH_HIn  = nand(top.WYMO_LCDC_WINEN.q(), _WY_MATCH4, _WY_MATCH5, _WY_MATCH6, _WY_MATCH7);
  /*p27.NELE*/ wire _WY_MATCH_HI   = not(_WY_MATCH_HIn);
  /*p27.PAFU*/ wire _WY_MATCHn     = nand(_WY_MATCH_HI, _WY_MATCH0, _WY_MATCH1, _WY_MATCH2, _WY_MATCH3);
  /*p27.ROGE*/ wire _ROGE_WY_MATCHp = not(_WY_MATCHn);
  return _ROGE_WY_MATCHp;
}

wire WindowRegisters::NUKO_WX_MATCHp(const SchematicTop& top) const {
  /*p27.MYLO*/ wire _WX_MATCH0 = xnor(top.ppu_reg.XEHO_X0.q(), _MYPA_WX0.q());
  /*p27.PUWU*/ wire _WX_MATCH1 = xnor(top.ppu_reg.SAVY_X1.q(), _NOFE_WX1.q());
  /*p27.PUHO*/ wire _WX_MATCH2 = xnor(top.ppu_reg.XODU_X2.q(), _NOKE_WX2.q());
  /*p27.NYTU*/ wire _WX_MATCH3 = xnor(top.ppu_reg.XYDO_X3.q(), _MEBY_WX3.q());
  /*p27.NEZO*/ wire _WX_MATCH4 = xnor(top.ppu_reg.TUHU_X4.q(), _MYPU_WX4.q());
  /*p27.NORY*/ wire _WX_MATCH5 = xnor(top.ppu_reg.TUKY_X5.q(), _MYCE_WX5.q());
  /*p27.NONO*/ wire _WX_MATCH6 = xnor(top.ppu_reg.TAKO_X6.q(), _MUVO_WX6.q());
  /*p27.PASE*/ wire _WX_MATCH7 = xnor(top.ppu_reg.SYBE_X7.q(), _NUKU_WX7.q());

  /*p27.PUKY*/ wire _WX_MATCH_HIn  = nand(_REJO_WY_MATCH_LATCH.q(), _WX_MATCH4, _WX_MATCH5, _WX_MATCH6, _WX_MATCH7);
  /*p27.NUFA*/ wire _WX_MATCH_HI   = not (_WX_MATCH_HIn);
  /*p27.NOGY*/ wire _WX_MATCHn     = nand(_WX_MATCH_HI, _WX_MATCH0, _WX_MATCH1, _WX_MATCH2, _WX_MATCH3);
  /*p27.NUKO*/ wire _NUKO_WX_MATCHp = not(_WX_MATCHn);
  return _NUKO_WX_MATCHp;
}

//------------------------------------------------------------------------------

SignalHash WindowRegisters::commit() {
  SignalHash hash;

  hash << _SUVU_PORCH_ENDn.commit();

  /*p27.SEKO*/ hash << SEKO_WX_MATCHne.commit();
  /*p27.SUZU*/ hash << SUZU_WIN_FIRST_TILEne.commit();

  /*p27.PYNU*/ hash << _PYNU_WIN_MODE_A.commit();
  /*p27.RYDY*/ hash << _RYDY_WIN_FIRST_TILE_A.commit();

  /*p27.NOPA*/ hash << _NOPA_WIN_MODE_B.commit();
  /*p27.SOVY*/ hash << _SOVY_WIN_FIRST_TILE_B.commit();
  /*p27.REJO*/ hash << _REJO_WY_MATCH_LATCH.commit();
  /*p27.SARY*/ hash << _SARY_WY_MATCH.commit();
  /*p27.RYFA*/ hash << _RYFA_WX_MATCHn_A.commit();
  /*p27.RENE*/ hash << _RENE_WX_MATCHn_B.commit();
  /*p27.PYCO*/ hash << _PYCO_WX_MATCH_A.commit();
  /*p27.NUNU*/ hash << _NUNU_WX_MATCH_B.commit();

  /*p27.WYKA*/ hash << _WYKA_WIN_X3.commit();
  /*p27.WODY*/ hash << _WODY_WIN_X4.commit();
  /*p27.WOBO*/ hash << _WOBO_WIN_X5.commit();
  /*p27.WYKO*/ hash << _WYKO_WIN_X6.commit();
  /*p27.XOLO*/ hash << _XOLO_WIN_X7.commit();

  /*p27.VYNO*/ hash << _VYNO_WIN_Y0.commit();
  /*p27.VUJO*/ hash << _VUJO_WIN_Y1.commit();
  /*p27.VYMU*/ hash << _VYMU_WIN_Y2.commit();
  /*p27.TUFU*/ hash << _TUFU_WIN_Y3.commit();
  /*p27.TAXA*/ hash << _TAXA_WIN_Y4.commit();
  /*p27.TOZO*/ hash << _TOZO_WIN_Y5.commit();
  /*p27.TATE*/ hash << _TATE_WIN_Y6.commit();
  /*p27.TEKE*/ hash << _TEKE_WIN_Y7.commit();

  /*p23.NESO*/ hash << _NESO_WY0.commit();
  /*p23.NYRO*/ hash << _NYRO_WY1.commit();
  /*p23.NAGA*/ hash << _NAGA_WY2.commit();
  /*p23.MELA*/ hash << _MELA_WY3.commit();
  /*p23.NULO*/ hash << _NULO_WY4.commit();
  /*p23.NENE*/ hash << _NENE_WY5.commit();
  /*p23.NUKA*/ hash << _NUKA_WY6.commit();
  /*p23.NAFU*/ hash << _NAFU_WY7.commit();

  /*p23.MYPA*/ hash << _MYPA_WX0.commit();
  /*p23.NOFE*/ hash << _NOFE_WX1.commit();
  /*p23.NOKE*/ hash << _NOKE_WX2.commit();
  /*p23.MEBY*/ hash << _MEBY_WX3.commit();
  /*p23.MYPU*/ hash << _MYPU_WX4.commit();
  /*p23.MYCE*/ hash << _MYCE_WX5.commit();
  /*p23.MUVO*/ hash << _MUVO_WX6.commit();
  /*p23.NUKU*/ hash << _NUKU_WX7.commit();

  return hash;
}

//------------------------------------------------------------------------------


// Die trace:
// VETU = and(TEVO, PORE);
// XACO = not(XOFO)
// WYKA =

// WYKA01 <> WYKA12
// WYKA02 << VETU
// WYKA03 <> WYKA09
// WYKA04 nc
// WYKA05 nc
// WYKA06 << XACO
// WYKA07 << WYKA16
// WYKA08 nc
// WYKA09 <> WYKA03
// WYKA10 nc
// WYKA11 nc
// WYKA12 <> WYKA01
// WYKA13 << XACO
// WYKA14 nc
// WYKA15 nc
// WYKA16 >> WYKA07, WODY02
// WYKA17 >> XEJA

