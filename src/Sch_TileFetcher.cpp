#include "Sch_TileFetcher.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void TileFetcher::dump(Dumper& d, const SchematicTop& top) const {
  d("----------TileFetcher---------\n");

  // high for 10 phases during fetch, low for 6 phases
  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(_LAXU_BFETCH_S0.qp(), _NYVA_BFETCH_S2.qp(), top.NYXU_TILE_FETCHER_RSTn());

  /*p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(top.clk_reg.ALET_xBxDxFxH(), MOCE_BFETCH_DONEn);
  d("_LEBO_AxCxExGx           %d\n", _LEBO_AxCxExGx);
  d("\n");

  int scx = pack_p(BAKE_SCX7.qp(), CABU_SCX6.qp(), CUZY_SCX5.qp(), BEMY_SCX4.qp(), GUBO_SCX3.qp(), CYXU_SCX2.qp(), DUZU_SCX1.qp(), DATY_SCX0.qp());
  int scy = pack_p(FUNY_SCY7.qp(), FOHA_SCY6.qp(), FOTY_SCY5.qp(), DEDE_SCY4.qp(), FUJO_SCY3.qp(), FEZU_SCY2.qp(), FYMO_SCY1.qp(), GAVE_SCY0.qp());
  d("SCX  %03d\n", scx);
  d("SCY  %03d\n", scy);

  /*p27.TEVO*/ wire TEVO_FINE_RSTp = or3(top.pix_pipe.SEKO_WX_MATCHne(), top.pix_pipe.SUZU_WIN_FIRST_TILEne(), top.TAVE_PRELOAD_DONE_TRIGp()); // Schematic wrong, this is OR
  /*p27.NYXU*/ wire NYXU_TILE_FETCHER_RSTn = nor3(top.sprite_scanner.AVAP_RENDER_START_TRIGp(), top.pix_pipe.MOSU_TILE_FETCHER_RSTp(), TEVO_FINE_RSTp);

  d("POKY_PRELOAD_DONEp         %c\n", _POKY_PRELOAD_DONEp        .c());
  d("TAVE_PRELOAD_DONE_TRIGp  %d\n", top.TAVE_PRELOAD_DONE_TRIGp());
  d("\n");
  d("SEKO_WX_MATCHne          %d\n", top.pix_pipe.SEKO_WX_MATCHne());
  d("SUZU_WIN_FIRST_TILEne    %d\n", top.pix_pipe.SUZU_WIN_FIRST_TILEne());

  d("TEVO_FINE_RSTp           %d\n", TEVO_FINE_RSTp);

  d("AVAP_RENDER_START_TRIGp  %d\n", top.sprite_scanner.AVAP_RENDER_START_TRIGp());
  d("MOSU_TILE_FETCHER_RSTp   %d\n", top.pix_pipe.MOSU_TILE_FETCHER_RSTp());
  d("NYXU_TILE_FETCHER_RSTn   %d\n", NYXU_TILE_FETCHER_RSTn);

  d("LAXU_BFETCH_S0           %c\n", _LAXU_BFETCH_S0          .c());
  d("MESU_BFETCH_S1           %c\n", _MESU_BFETCH_S1          .c());
  d("NYVA_BFETCH_S2           %c\n", _NYVA_BFETCH_S2          .c());
  d("MOCE_BFETCH_DONEn        %d\n", MOCE_BFETCH_DONEn);
  d("LOVY_FETCH_DONEp         %c\n", _LOVY_FETCH_DONEp        .c());
  d("NYKA_FETCH_DONE_Ap       %c\n", _NYKA_FETCH_DONE_Ap      .c());
  d("PORY_FETCH_DONE_Bp       %c\n", _PORY_FETCH_DONE_Bp      .c());
  d("LYZU_BFETCH_S0_DELAY     %c\n", _LYZU_BFETCH_S0_DELAY    .c());
  d("PYGO_FETCH_DONE_Cp       %c\n", _PYGO_FETCH_DONE_Cp      .c());
  d("LONY_BG_READ_VRAM_LATCHp %c\n", _LONY_BG_READ_VRAM_LATCHp.c());

  d("\n");
}

//------------------------------------------------------------------------------

void TileFetcher::tick(const SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void TileFetcher::tock(SchematicTop& top, CpuBus& cpu_bus) {
  {
    /*p27.LURY*/ wire _LURY_BG_READ_VRAM_LATCH_RSTn = and2(_LOVY_FETCH_DONEp.qn(), top.pix_pipe.XYMU_RENDERINGp());
    /*p27.LONY*/ _LONY_BG_READ_VRAM_LATCHp = nand_latch(top.NYXU_TILE_FETCHER_RSTn(), _LURY_BG_READ_VRAM_LATCH_RSTn);
  }

  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(_LAXU_BFETCH_S0.qp(), _NYVA_BFETCH_S2.qp(), top.NYXU_TILE_FETCHER_RSTn());
  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  {
    /*p27.LOVY*/ _LOVY_FETCH_DONEp = dff17_A(top.clk_reg.MYVO_AxCxExGx(), top.NYXU_TILE_FETCHER_RSTn(), LYRY_BFETCH_DONEp);

    // high for 10 phases during fetch, low for 6 phases
    /*p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(top.clk_reg.ALET_xBxDxFxH(), MOCE_BFETCH_DONEn);

    /*p27.LAXU*/ _LAXU_BFETCH_S0 = dff17_AB(_LEBO_AxCxExGx,       top.NYXU_TILE_FETCHER_RSTn(), _LAXU_BFETCH_S0.qn());
    /*p27.MESU*/ _MESU_BFETCH_S1 = dff17_AB(_LAXU_BFETCH_S0.qn(), top.NYXU_TILE_FETCHER_RSTn(), _MESU_BFETCH_S1.qn());
    /*p27.NYVA*/ _NYVA_BFETCH_S2 = dff17_AB(_MESU_BFETCH_S1.qn(), top.NYXU_TILE_FETCHER_RSTn(), _NYVA_BFETCH_S2.qn());

    /*p27.LYZU*/ _LYZU_BFETCH_S0_DELAY = dff17_B(top.clk_reg.ALET_xBxDxFxH(), top.pix_pipe.XYMU_RENDERINGp(), _LAXU_BFETCH_S0.qp());
  }

  {
    /*p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG = nor2(top.pix_pipe.MOSU_TILE_FETCHER_RSTp(), top.pix_pipe.LOBY_RENDERINGn());

    /*p24.NYKA*/ _NYKA_FETCH_DONE_Ap = dff17_B(top.clk_reg.ALET_xBxDxFxH(), _NAFY_RENDERING_AND_NOT_WIN_TRIG, LYRY_BFETCH_DONEp);
    /*p24.PORY*/ _PORY_FETCH_DONE_Bp = dff17_B(top.clk_reg.MYVO_AxCxExGx(), _NAFY_RENDERING_AND_NOT_WIN_TRIG, _NYKA_FETCH_DONE_Ap.qp());
    /*p24.PYGO*/ _PYGO_FETCH_DONE_Cp = dff17_B(top.clk_reg.ALET_xBxDxFxH(), top.pix_pipe.XYMU_RENDERINGp(),   _PORY_FETCH_DONE_Bp.qp());

    /*p24.POKY*/ _POKY_PRELOAD_DONEp = nor_latch(_PYGO_FETCH_DONE_Cp.qp(), top.pix_pipe.LOBY_RENDERINGn());
  }

  //----------------------------------------

  // FF42 SCY
  {
    /*p22.WEBU*/ wire _WEBU_FF42n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.XERA_A03n());
    /*p22.XARO*/ wire _XARO_FF42p = not1(_WEBU_FF42n);

    /*p23.ANYP*/ wire _ANYP_FF42_RDp = and2(_XARO_FF42p, top.ASOT_CPU_RDp());
    /*p23.BUWY*/ wire _BUWY_FF42_RDn = not1(_ANYP_FF42_RDp);

    /*p23.BEDY*/ wire _BEDY_FF42_WRp = and2(_XARO_FF42p, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.CAVO*/ wire _CAVO_FF42_WRn = not1(_BEDY_FF42_WRp);

    /*p23.GAVE*/ GAVE_SCY0 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p23.FYMO*/ FYMO_SCY1 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p23.FEZU*/ FEZU_SCY2 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p23.FUJO*/ FUJO_SCY3 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p23.DEDE*/ DEDE_SCY4 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p23.FOTY*/ FOTY_SCY5 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p23.FOHA*/ FOHA_SCY6 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p23.FUNY*/ FUNY_SCY7 = dff9(_CAVO_FF42_WRn, !_CAVO_FF42_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());

    /*p23.WARE*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_BUWY_FF42_RDn, GAVE_SCY0.qp());
    /*p23.GOBA*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_BUWY_FF42_RDn, FYMO_SCY1.qp());
    /*p23.GONU*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_BUWY_FF42_RDn, FEZU_SCY2.qp());
    /*p23.GODO*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_BUWY_FF42_RDn, FUJO_SCY3.qp());
    /*p23.CUSA*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_BUWY_FF42_RDn, DEDE_SCY4.qp());
    /*p23.GYZO*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_BUWY_FF42_RDn, FOTY_SCY5.qp());
    /*p23.GUNE*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_BUWY_FF42_RDn, FOHA_SCY6.qp());
    /*p23.GYZA*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_BUWY_FF42_RDn, FUNY_SCY7.qp());
  }

  // FF43 SCX
  {
    /*p22.WAVU*/ wire _WAVU_FF43n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.WESA_A01p(), top.cpu_bus.XUSY_A02n(), top.cpu_bus.XERA_A03n());
    /*p22.XAVY*/ wire _XAVY_FF43p = not1(_WAVU_FF43n);

    /*p23.AVOG*/ wire _AVOG_FF43_RDp = and2(_XAVY_FF43p, top.ASOT_CPU_RDp());
    /*p23.BEBA*/ wire _BEBA_FF43_RDn = not1(_AVOG_FF43_RDp);

    /*p23.ARUR*/ wire _ARUR_FF43_WRp = and2(_XAVY_FF43p, top.CUPA_CPU_WRp_xxxDxxxx());
    /*p23.AMUN*/ wire _AMUN_FF43_WRn = not1(_ARUR_FF43_WRp);

    /*p23.DATY*/ DATY_SCX0 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p23.DUZU*/ DUZU_SCX1 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p23.CYXU*/ CYXU_SCX2 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p23.GUBO*/ GUBO_SCX3 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p23.BEMY*/ BEMY_SCX4 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p23.CUZY*/ CUZY_SCX5 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p23.CABU*/ CABU_SCX6 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p23.BAKE*/ BAKE_SCX7 = dff9(_AMUN_FF43_WRn, !_AMUN_FF43_WRn, top.clk_reg.CUNU_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());

    /*p23.EDOS*/ cpu_bus.CPU_BUS_D0 = tribuf_6n(_BEBA_FF43_RDn, DATY_SCX0.qp());
    /*p23.EKOB*/ cpu_bus.CPU_BUS_D1 = tribuf_6n(_BEBA_FF43_RDn, DUZU_SCX1.qp());
    /*p23.CUGA*/ cpu_bus.CPU_BUS_D2 = tribuf_6n(_BEBA_FF43_RDn, CYXU_SCX2.qp());
    /*p23.WONY*/ cpu_bus.CPU_BUS_D3 = tribuf_6n(_BEBA_FF43_RDn, GUBO_SCX3.qp());
    /*p23.CEDU*/ cpu_bus.CPU_BUS_D4 = tribuf_6n(_BEBA_FF43_RDn, BEMY_SCX4.qp());
    /*p23.CATA*/ cpu_bus.CPU_BUS_D5 = tribuf_6n(_BEBA_FF43_RDn, CUZY_SCX5.qp());
    /*p23.DOXE*/ cpu_bus.CPU_BUS_D6 = tribuf_6n(_BEBA_FF43_RDn, CABU_SCX6.qp());
    /*p23.CASY*/ cpu_bus.CPU_BUS_D7 = tribuf_6n(_BEBA_FF43_RDn, BAKE_SCX7.qp());
  }
}

//------------------------------------------------------------------------------
