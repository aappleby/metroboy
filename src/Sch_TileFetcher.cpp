#include "Sch_TileFetcher.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void TileFetcher::tick(const SchematicTop& top) {
  _XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGp();
  _NYXU_TILE_FETCHER_RSTn = top.NYXU_TILE_FETCHER_RSTn();
}

//------------------------------------------------------------------------------

void TileFetcher::tock(SchematicTop& top, CpuBus& cpu_bus) {
  {
    /*p27.LURY*/ wire _LURY_BG_READ_VRAM_LATCH_RSTn = and(_LOVY_FETCH_DONEp.qn(), top.pix_pipe.XYMU_RENDERINGp());
    /*p27.LONY*/ _LONY_BG_READ_VRAM_LATCHp.nand_latch(top.NYXU_TILE_FETCHER_RSTn(), _LURY_BG_READ_VRAM_LATCH_RSTn);
  }

  {
    /*p27.LOVY*/ _LOVY_FETCH_DONEp.set(top.clk_reg.MYVO_AxCxExGx(), top.NYXU_TILE_FETCHER_RSTn(), LYRY_BFETCH_DONEp());

    /*p27.LEBO*/ wire _LEBO_AxCxExGx = nand(top.clk_reg.ALET_xBxDxFxH(), MOCE_BFETCH_DONEn());
    /*p27.LAXU*/ _LAXU_BFETCH_S0.set(_LEBO_AxCxExGx,       top.NYXU_TILE_FETCHER_RSTn(), _LAXU_BFETCH_S0.qn());
    /*p27.MESU*/ _MESU_BFETCH_S1.set(_LAXU_BFETCH_S0.qn(), top.NYXU_TILE_FETCHER_RSTn(), _MESU_BFETCH_S1.qn());
    /*p27.NYVA*/ _NYVA_BFETCH_S2.set(_MESU_BFETCH_S1.qn(), top.NYXU_TILE_FETCHER_RSTn(), _NYVA_BFETCH_S2.qn());

    /*p27.LYZU*/ _LYZU_BFETCH_S0_DELAY.set (top.clk_reg.ALET_xBxDxFxH(), top.pix_pipe.XYMU_RENDERINGp(), _LAXU_BFETCH_S0.q());
  }

  {
    /*p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG = nor(top.pix_pipe.MOSU_TILE_FETCHER_RSTp(), top.pix_pipe.LOBY_RENDERINGn());

    /*p24.NYKA*/ _NYKA_FETCH_DONE_Ap.set(top.clk_reg.ALET_xBxDxFxH(), _NAFY_RENDERING_AND_NOT_WIN_TRIG, LYRY_BFETCH_DONEp());
    /*p24.PORY*/ _PORY_FETCH_DONE_Bp.set(top.clk_reg.MYVO_AxCxExGx(), _NAFY_RENDERING_AND_NOT_WIN_TRIG, _NYKA_FETCH_DONE_Ap.q());
    /*p24.PYGO*/ _PYGO_FETCH_DONE_Cp.set(top.clk_reg.ALET_xBxDxFxH(), _XYMU_RENDERINGp,                 _PORY_FETCH_DONE_Bp.q());

    /*p24.POKY*/ _POKY_PORCH_DONEp.nor_latch(_PYGO_FETCH_DONE_Cp.q(), top.pix_pipe.LOBY_RENDERINGn());
  }

  {
    /*p27.MYSO*/ wire _MYSO_BG_TRIGp        = nor(top.pix_pipe.LOBY_RENDERINGn(), LAXE_BFETCH_S0n(), _LYZU_BFETCH_S0_DELAY.q());
    /*p27.NYDY*/ _NYDY_LATCH_TILE_DAp = nand(_MYSO_BG_TRIGp, _MESU_BFETCH_S1.q(), NOFU_BFETCH_S2n());
    /*p27.MOFU*/ _MOFU_LATCH_TILE_DBn = and(_MYSO_BG_TRIGp, NAKO_BFETCH_S1n());
  }


  //----------------------------------------


  // FF42 SCY
  {
    /*p22.WEBU*/ wire _WEBU_FF42n = nand(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.XERA_A03n());
    /*p22.XARO*/ wire _XARO_FF42p = not(_WEBU_FF42n);

    /*p23.ANYP*/ wire _ANYP_FF42_RDp = and(_XARO_FF42p, top.ASOT_CPU_RDp());
    /*p23.BUWY*/ wire _BUWY_FF42_RDn = not(_ANYP_FF42_RDp);

    /*p23.BEDY*/ wire _BEDY_FF42_WRp = and(_XARO_FF42p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.CAVO*/ wire _CAVO_FF42_WRn = not(_BEDY_FF42_WRp);

    /*p23.GAVE*/ GAVE_SCY0.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D0.q());
    /*p23.FYMO*/ FYMO_SCY1.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D1.q());
    /*p23.FEZU*/ FEZU_SCY2.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D2.q());
    /*p23.FUJO*/ FUJO_SCY3.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D3.q());
    /*p23.DEDE*/ DEDE_SCY4.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D4.q());
    /*p23.FOTY*/ FOTY_SCY5.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D5.q());
    /*p23.FOHA*/ FOHA_SCY6.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D6.q());
    /*p23.FUNY*/ FUNY_SCY7.set(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D7.q());

    /*p23.WARE*/ cpu_bus.CPU_TRI_D0.set_tribuf_6n(_BUWY_FF42_RDn, GAVE_SCY0.q());
    /*p23.GOBA*/ cpu_bus.CPU_TRI_D1.set_tribuf_6n(_BUWY_FF42_RDn, FYMO_SCY1.q());
    /*p23.GONU*/ cpu_bus.CPU_TRI_D2.set_tribuf_6n(_BUWY_FF42_RDn, FEZU_SCY2.q());
    /*p23.GODO*/ cpu_bus.CPU_TRI_D3.set_tribuf_6n(_BUWY_FF42_RDn, FUJO_SCY3.q());
    /*p23.CUSA*/ cpu_bus.CPU_TRI_D4.set_tribuf_6n(_BUWY_FF42_RDn, DEDE_SCY4.q());
    /*p23.GYZO*/ cpu_bus.CPU_TRI_D5.set_tribuf_6n(_BUWY_FF42_RDn, FOTY_SCY5.q());
    /*p23.GUNE*/ cpu_bus.CPU_TRI_D6.set_tribuf_6n(_BUWY_FF42_RDn, FOHA_SCY6.q());
    /*p23.GYZA*/ cpu_bus.CPU_TRI_D7.set_tribuf_6n(_BUWY_FF42_RDn, FUNY_SCY7.q());
  }

  // FF43 SCX
  {
    /*p22.WAVU*/ wire _WAVU_FF43n = nand(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.XERA_A03n());
    /*p22.XAVY*/ wire _XAVY_FF43p = not(_WAVU_FF43n);

    /*p23.AVOG*/ wire _AVOG_FF43_RDp = and (_XAVY_FF43p, top.ASOT_CPU_RDp());
    /*p23.BEBA*/ wire _BEBA_FF43_RDn = not(_AVOG_FF43_RDp);

    /*p23.ARUR*/ wire _ARUR_FF43_WRp = and (_XAVY_FF43p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p23.AMUN*/ wire _AMUN_FF43_WRn = not(_ARUR_FF43_WRp);

    /*p23.DATY*/ DATY_SCX0.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D0.q());
    /*p23.DUZU*/ DUZU_SCX1.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D1.q());
    /*p23.CYXU*/ CYXU_SCX2.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D2.q());
    /*p23.GUBO*/ GUBO_SCX3.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D3.q());
    /*p23.BEMY*/ BEMY_SCX4.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D4.q());
    /*p23.CUZY*/ CUZY_SCX5.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D5.q());
    /*p23.CABU*/ CABU_SCX6.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D6.q());
    /*p23.BAKE*/ BAKE_SCX7.set(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_TRI_D7.q());

    /*p23.EDOS*/ cpu_bus.CPU_TRI_D0.set_tribuf_6n(_BEBA_FF43_RDn, DATY_SCX0.q());
    /*p23.EKOB*/ cpu_bus.CPU_TRI_D1.set_tribuf_6n(_BEBA_FF43_RDn, DUZU_SCX1.q());
    /*p23.CUGA*/ cpu_bus.CPU_TRI_D2.set_tribuf_6n(_BEBA_FF43_RDn, CYXU_SCX2.q());
    /*p23.WONY*/ cpu_bus.CPU_TRI_D3.set_tribuf_6n(_BEBA_FF43_RDn, GUBO_SCX3.q());
    /*p23.CEDU*/ cpu_bus.CPU_TRI_D4.set_tribuf_6n(_BEBA_FF43_RDn, BEMY_SCX4.q());
    /*p23.CATA*/ cpu_bus.CPU_TRI_D5.set_tribuf_6n(_BEBA_FF43_RDn, CUZY_SCX5.q());
    /*p23.DOXE*/ cpu_bus.CPU_TRI_D6.set_tribuf_6n(_BEBA_FF43_RDn, CABU_SCX6.q());
    /*p23.CASY*/ cpu_bus.CPU_TRI_D7.set_tribuf_6n(_BEBA_FF43_RDn, BAKE_SCX7.q());
  }
}

//------------------------------------------------------------------------------

SignalHash TileFetcher::commit() {
  SignalHash hash;

  hash << _XYMU_RENDERINGp.commit();
  hash << _NYXU_TILE_FETCHER_RSTn.commit();

  hash << _NYDY_LATCH_TILE_DAp.commit();
  hash << _MOFU_LATCH_TILE_DBn.commit();

  hash << _PYGO_FETCH_DONE_Cp.commit();
  hash << _POKY_PORCH_DONEp.commit();

  hash << _LAXU_BFETCH_S0.commit();
  hash << _MESU_BFETCH_S1.commit();
  hash << _NYVA_BFETCH_S2.commit();

  hash << _LOVY_FETCH_DONEp.commit();
  hash << _LONY_BG_READ_VRAM_LATCHp.commit();

  hash << _NYKA_FETCH_DONE_Ap.commit();
  hash << _PORY_FETCH_DONE_Bp.commit();
  hash << _LYZU_BFETCH_S0_DELAY.commit();

  hash << GAVE_SCY0.commit();
  hash << FYMO_SCY1.commit();
  hash << FEZU_SCY2.commit();
  hash << FUJO_SCY3.commit();
  hash << DEDE_SCY4.commit();
  hash << FOTY_SCY5.commit();
  hash << FOHA_SCY6.commit();
  hash << FUNY_SCY7.commit();

  hash << DATY_SCX0.commit();
  hash << DUZU_SCX1.commit();
  hash << CYXU_SCX2.commit();
  hash << GUBO_SCX3.commit();
  hash << BEMY_SCX4.commit();
  hash << CUZY_SCX5.commit();
  hash << CABU_SCX6.commit();
  hash << BAKE_SCX7.commit();

  return hash;
}

//------------------------------------------------------------------------------
