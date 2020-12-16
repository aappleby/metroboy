#include "GateBoyLib/GateBoyTileFetcher.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyCpuBus.h"

void TileTempA::tock(const GateBoyVramBus& vram_bus, wire LOMA_LATCH_TILE_DAn)
{
  /* p32.LEGU*/ LEGU_TILE_DA0n.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[0].qp_old());
  /* p32.NUDU*/ NUDU_TILE_DA1n.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[1].qp_old());
  /* p32.MUKU*/ MUKU_TILE_DA2n.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[2].qp_old());
  /* p32.LUZO*/ LUZO_TILE_DA3n.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[3].qp_old());
  /* p32.MEGU*/ MEGU_TILE_DA4n.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[4].qp_old());
  /* p32.MYJY*/ MYJY_TILE_DA5n.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[5].qp_old());
  /* p32.NASA*/ NASA_TILE_DA6n.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[6].qp_old());
  /* p32.NEFO*/ NEFO_TILE_DA7n.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[7].qp_old());
}

void TileTempB::tock(const GateBoyVramBus& vram_bus, wire _LABU_LATCH_TILE_DBn)
{
  wire _VYPO_VCC = 1;
  // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
  /* p32.RAWU*/ RAWU_TILE_DB0p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[0].qp_old());
  /* p32.POZO*/ POZO_TILE_DB1p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[1].qp_old());
  /* p32.PYZO*/ PYZO_TILE_DB2p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[2].qp_old());
  /* p32.POXA*/ POXA_TILE_DB3p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[3].qp_old());
  /* p32.PULO*/ PULO_TILE_DB4p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[4].qp_old());
  /* p32.POJU*/ POJU_TILE_DB5p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[5].qp_old());
  /* p32.POWY*/ POWY_TILE_DB6p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[6].qp_old());
  /* p32.PYJU*/ PYJU_TILE_DB7p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[7].qp_old());
}


void RegSCX::tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  // FF43 SCX
  /* p23.AVOG*/ wire _AVOG_FF43_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.XAVY_FF43p());
  /* p23.ARUR*/ wire _ARUR_FF43_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.XAVY_FF43p());

  /* p23.AMUN*/ wire _AMUN_FF43_WRn = not1(_ARUR_FF43_WRp);
  /* p23.DATY*/ DATY_SCX0n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp());
  /* p23.DUZU*/ DUZU_SCX1n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp());
  /* p23.CYXU*/ CYXU_SCX2n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp());
  /* p23.GUBO*/ GUBO_SCX3n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp());
  /* p23.BEMY*/ BEMY_SCX4n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp());
  /* p23.CUZY*/ CUZY_SCX5n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp());
  /* p23.CABU*/ CABU_SCX6n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp());
  /* p23.BAKE*/ BAKE_SCX7n.dff9(_AMUN_FF43_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp());

  // FF43 SCX
  /* p23.BEBA*/ wire _BEBA_FF43_RDn = not1(_AVOG_FF43_RDp);
  /*#p23.EDOS*/ cpu_bus.BUS_CPU_D_out[0].tri6_nn(_BEBA_FF43_RDn, DATY_SCX0n.qp_new());
  /* p23.EKOB*/ cpu_bus.BUS_CPU_D_out[1].tri6_nn(_BEBA_FF43_RDn, DUZU_SCX1n.qp_new());
  /* p23.CUGA*/ cpu_bus.BUS_CPU_D_out[2].tri6_nn(_BEBA_FF43_RDn, CYXU_SCX2n.qp_new());
  /* p23.WONY*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_BEBA_FF43_RDn, GUBO_SCX3n.qp_new());
  /* p23.CEDU*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_BEBA_FF43_RDn, BEMY_SCX4n.qp_new());
  /* p23.CATA*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_BEBA_FF43_RDn, CUZY_SCX5n.qp_new());
  /* p23.DOXE*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_BEBA_FF43_RDn, CABU_SCX6n.qp_new());
  /* p23.CASY*/ cpu_bus.BUS_CPU_D_out[7].tri6_nn(_BEBA_FF43_RDn, BAKE_SCX7n.qp_new());
}


void RegSCY::tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  // FF42 SCY

  /* p23.ANYP*/ wire _ANYP_FF42_RDp = and2(cpu_bus.ASOT_CPU_RDp(), cpu_bus.XARO_FF42p());
  /* p23.BEDY*/ wire _BEDY_FF42_WRp = and2(cpu_bus.CUPA_CPU_WRp(), cpu_bus.XARO_FF42p());

  /* FF42 SCY */
  /* p23.CAVO*/ wire _CAVO_FF42_WRn = not1(_BEDY_FF42_WRp);
  /* p23.GAVE*/ GAVE_SCY0n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp());
  /* p23.FYMO*/ FYMO_SCY1n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp());
  /* p23.FEZU*/ FEZU_SCY2n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp());
  /* p23.FUJO*/ FUJO_SCY3n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp());
  /* p23.DEDE*/ DEDE_SCY4n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp());
  /* p23.FOTY*/ FOTY_SCY5n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp());
  /* p23.FOHA*/ FOHA_SCY6n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp());
  /* p23.FUNY*/ FUNY_SCY7n.dff9(_CAVO_FF42_WRn, rst.CUNU_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp());

  /* FF42 SCY */
  /* p23.BUWY*/ wire _BUWY_FF42_RDn = not1(_ANYP_FF42_RDp);
  /*#p23.WARE*/ cpu_bus.BUS_CPU_D_out[0].tri6_nn(_BUWY_FF42_RDn, GAVE_SCY0n.qp_new());
  /* p23.GOBA*/ cpu_bus.BUS_CPU_D_out[1].tri6_nn(_BUWY_FF42_RDn, FYMO_SCY1n.qp_new());
  /* p23.GONU*/ cpu_bus.BUS_CPU_D_out[2].tri6_nn(_BUWY_FF42_RDn, FEZU_SCY2n.qp_new());
  /* p23.GODO*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_BUWY_FF42_RDn, FUJO_SCY3n.qp_new());
  /* p23.CUSA*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_BUWY_FF42_RDn, DEDE_SCY4n.qp_new());
  /* p23.GYZO*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_BUWY_FF42_RDn, FOTY_SCY5n.qp_new());
  /* p23.GUNE*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_BUWY_FF42_RDn, FOHA_SCY6n.qp_new());
  /* p23.GYZA*/ cpu_bus.BUS_CPU_D_out[7].tri6_nn(_BUWY_FF42_RDn, FUNY_SCY7n.qp_new());
}


void WinMapX::tock(const GateBoyResetDebug& rst, wire TEVO_FETCH_TRIGp, wire PORE_WIN_MODEp, wire WYMO_LCDC_WINENp, wire XAHY_LINE_RSTn) {
  /* p27.VETU*/ wire _VETU_WIN_MAPp = and2(TEVO_FETCH_TRIGp, PORE_WIN_MODEp);
  /*#p27.XOFO*/ wire _XOFO_WIN_RSTp = nand3(WYMO_LCDC_WINENp, XAHY_LINE_RSTn, rst.XAPO_VID_RSTn());
  /* p27.XACO*/ wire _XACO_WIN_RSTn = not1(_XOFO_WIN_RSTp);
  /* p27.WYKA*/ WYKA_WIN_X3.dff17(_VETU_WIN_MAPp,       _XACO_WIN_RSTn, WYKA_WIN_X3.qn());
  /* p27.WODY*/ WODY_WIN_X4.dff17(WYKA_WIN_X3.qn_new(), _XACO_WIN_RSTn, WODY_WIN_X4.qn());
  /* p27.WOBO*/ WOBO_WIN_X5.dff17(WODY_WIN_X4.qn_new(), _XACO_WIN_RSTn, WOBO_WIN_X5.qn());
  /* p27.WYKO*/ WYKO_WIN_X6.dff17(WOBO_WIN_X5.qn_new(), _XACO_WIN_RSTn, WYKO_WIN_X6.qn());
  /* p27.XOLO*/ XOLO_WIN_X7.dff17(WYKO_WIN_X6.qn_new(), _XACO_WIN_RSTn, XOLO_WIN_X7.qn());

}

void WinLineY::tock(const GateBoyResetDebug& rst, wire PORE_WIN_MODEp, wire PARU_VBLANKp) {
  // Every time we leave win mode we increment win_y
  /* p27.WAZY*/ wire _WAZY_WIN_MODEn = not1(PORE_WIN_MODEp);
  /* p27.REPU*/ wire _REPU_VBLANKp   = or2(PARU_VBLANKp, rst.PYRY_VID_RSTp());
  /* p27.SYNY*/ wire _SYNY_VBLANKn   = not1(_REPU_VBLANKp);
  /* p27.VYNO*/ VYNO_WIN_Y0.dff17(_WAZY_WIN_MODEn,      _SYNY_VBLANKn, VYNO_WIN_Y0.qn());
  /* p27.VUJO*/ VUJO_WIN_Y1.dff17(VYNO_WIN_Y0.qn_new(), _SYNY_VBLANKn, VUJO_WIN_Y1.qn());
  /* p27.VYMU*/ VYMU_WIN_Y2.dff17(VUJO_WIN_Y1.qn_new(), _SYNY_VBLANKn, VYMU_WIN_Y2.qn());
  /* p27.TUFU*/ TUFU_WIN_Y3.dff17(VYMU_WIN_Y2.qn_new(), _SYNY_VBLANKn, TUFU_WIN_Y3.qn());
  /* p27.TAXA*/ TAXA_WIN_Y4.dff17(TUFU_WIN_Y3.qn_new(), _SYNY_VBLANKn, TAXA_WIN_Y4.qn());
  /* p27.TOZO*/ TOZO_WIN_Y5.dff17(TAXA_WIN_Y4.qn_new(), _SYNY_VBLANKn, TOZO_WIN_Y5.qn());
  /* p27.TATE*/ TATE_WIN_Y6.dff17(TOZO_WIN_Y5.qn_new(), _SYNY_VBLANKn, TATE_WIN_Y6.qn());
  /* p27.TEKE*/ TEKE_WIN_Y7.dff17(TATE_WIN_Y6.qn_new(), _SYNY_VBLANKn, TEKE_WIN_Y7.qn());
}

void TileFetcher::tock2(GateBoyClock& clk, wire XYMU_RENDERINGp, wire NYXU_BFETCH_RSTn, wire MOCE_BFETCH_DONEn_old)
{
  /* p27.LAXU*/ _LAXU_BFETCH_S0p.RSTn(NYXU_BFETCH_RSTn);
  /* p27.MESU*/ _MESU_BFETCH_S1p.RSTn(NYXU_BFETCH_RSTn);
  /* p27.NYVA*/ _NYVA_BFETCH_S2p.RSTn(NYXU_BFETCH_RSTn);

  /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn_mid = MOCE_BFETCH_DONEn(NYXU_BFETCH_RSTn);
  /* p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(clk.ALET_xBxDxFxH(), _MOCE_BFETCH_DONEn_mid);

  /* p27.LAXU*/ _LAXU_BFETCH_S0p.dff17(_LEBO_AxCxExGx,               NYXU_BFETCH_RSTn, _LAXU_BFETCH_S0p.qn_new());
  /* p27.MESU*/ _MESU_BFETCH_S1p.dff17(_LAXU_BFETCH_S0p.qn_new(), NYXU_BFETCH_RSTn, _MESU_BFETCH_S1p.qn_new());
  /* p27.NYVA*/ _NYVA_BFETCH_S2p.dff17(_MESU_BFETCH_S1p.qn_new(), NYXU_BFETCH_RSTn, _NYVA_BFETCH_S2p.qn_new());

  /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp_old = not1(MOCE_BFETCH_DONEn_old);
  /* p27.LOVY*/ LOVY_FETCH_DONEp.dff17(clk.MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, _LYRY_BFETCH_DONEp_old);
  /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn = and2(LOVY_FETCH_DONEp.qn_new(), XYMU_RENDERINGp);
  /* p27.LONY*/ LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, _LURY_BG_FETCH_DONEn);
  /* p27.LYZU*/ _LYZU_BFETCH_S0p_D1.dff17(clk.ALET_xBxDxFxH(), XYMU_RENDERINGp, _LAXU_BFETCH_S0p.qp_new());
}

wire TileFetcher::LOMA_LATCH_TILE_DAn() const {
    /* p24.LOBY*/ wire _LOBY_RENDERINGn_new_xxx = not1(_XYMU_RENDERINGp.qp());

  /* p27.LAXU*/ wire LAXU_BFETCH_S0p = _LAXU_BFETCH_S0p.qp_new();
  /* p27.MESU*/ wire MESU_BFETCH_S1p = _MESU_BFETCH_S1p.qp_new();
  /* p27.NYVA*/ wire NYVA_BFETCH_S2p = _NYVA_BFETCH_S2p.qp_new();

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n = not1(LAXU_BFETCH_S0p);
  /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n = not1(NYVA_BFETCH_S2p);

  /*#p27.MYSO*/ wire _MYSO_STORE_VRAM_DATA_TRIGp = nor3(_LOBY_RENDERINGn_new_xxx, _LAXE_BFETCH_S0n, _LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn = nand3(_MYSO_STORE_VRAM_DATA_TRIGp, MESU_BFETCH_S1p, _NOFU_BFETCH_S2n); // NYDY on fetch phase 6
  /*#p32.METE*/ wire _METE_LATCH_TILE_DAp = not1(_NYDY_LATCH_TILE_DAn);
  /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn = not1(_METE_LATCH_TILE_DAp);
  return _LOMA_LATCH_TILE_DAn;
}

wire TileFetcher::LABU_LATCH_TILE_DBn() const {
  /* p24.LOBY*/ wire _LOBY_RENDERINGn = not1(_XYMU_RENDERINGp.qp());

  /* p27.LAXU*/ wire LAXU_BFETCH_S0p = _LAXU_BFETCH_S0p.qp_new();
  /* p27.MESU*/ wire MESU_BFETCH_S1p = _MESU_BFETCH_S1p.qp_new();

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n = not1(LAXU_BFETCH_S0p);
  /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n = not1(MESU_BFETCH_S1p);

  /*#p27.MYSO*/ wire _MYSO_STORE_VRAM_DATA_TRIGp = nor3(_LOBY_RENDERINGn, _LAXE_BFETCH_S0n, _LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /* p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp = and2(_MYSO_STORE_VRAM_DATA_TRIGp, _NAKO_BFETCH_S1n); // MOFU fires on fetch phase 2 and 10
  /* p32.LESO*/ wire _LESO_LATCH_TILE_DBn = not1(_MOFU_LATCH_TILE_DBp);
  /* p??.LUVE*/ wire _LUVE_LATCH_TILE_DBp = not1(_LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
  /* p32.LABU*/ wire _LABU_LATCH_TILE_DBn = not1(_LUVE_LATCH_TILE_DBp);
  return _LABU_LATCH_TILE_DBn;
}
