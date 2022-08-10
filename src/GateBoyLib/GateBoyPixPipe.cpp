#include "GateBoyLib/GateBoyPixPipe.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

// Sprite temp loads  on AB/CD/EF/GH
// Tile   temp loads  on AB/CD/EF/GH
// Pipes       shifts on HA/BC/DE/FG
// WUTY        fires  on HA/BC/DE/FG
// NYXU        fires  on HA/BC/DE/FG

//-----------------------------------------------------------------------------

void GateBoy::tock_window_gates(const GateBoyState& reg_old, wire SEGU_CLKPIPE_odd_new, wire REPU_VBLANKp) {
  auto& reg_new = gb_state;

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(reg_new.sys_rst.AFER_SYS_RSTp.qp_new(), reg_new.sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(ALUR_SYS_RSTn);  /*#p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
  /*#p01.XORE*/ wire XORE_SYS_RSTp = not1(CUNU_SYS_RSTn);
  /*#p01.WALU*/ wire WALU_SYS_RSTn = not1(XORE_SYS_RSTp);

  /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(reg_new.sys_clk.AVET_AxCxExGx.out_new());
  /*_p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
  /*_p01.ZAXY*/ wire ZAXY_xBxDxFxH = not1(AZOF_AxCxExGx);
  /*#p01.ZEME*/ wire ZEME_AxCxExGx = not1(ZAXY_xBxDxFxH);
  /*#p01.ALET*/ wire ALET_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*#p21.TALU*/ wire TALU_xxCDEFxx = not1(reg_new.sys_clk.VENA_xxCDEFxx.qn_new());

  /*#p27.ROZE*/ wire ROZE_FINE_COUNT_7n_odd_old = nand3(reg_old.fine_count_odd.RUBU_FINE_CNT2_odd.qp_old(), reg_old.fine_count_odd.ROGA_FINE_CNT1_odd.qp_old(), reg_old.fine_count_odd.RYKU_FINE_CNT0_odd.qp_old());
  /*_p27.PANY*/ wire PANY_WIN_FETCHn_old = nor2(reg_old.win_ctrl.NUKO_WX_MATCHp_odd.out_old(), ROZE_FINE_COUNT_7n_odd_old);

  /*_p27.RENE*/ reg_new.win_ctrl.RENE_WIN_FETCHn_B_evn.dff17(ALET_xBxDxFxH,  reg_new.XYMU_RENDERING_LATCHn.qn_new(), reg_old.win_ctrl.RYFA_WIN_FETCHn_A_evn.qp_old());
  /*_p27.RYFA*/ reg_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.dff17(SEGU_CLKPIPE_odd_new,            reg_new.XYMU_RENDERING_LATCHn.qn_new(), PANY_WIN_FETCHn_old);

  /*_p07.DYKY*/ wire DYKY_CPU_WRn_new = not1(reg_new.cpu_signals.TAPU_CPU_WRp.out_new());
  /*_p07.CUPA*/ wire CUPA_CPU_WRp_new = not1(DYKY_CPU_WRn_new);

  /*_p23.WEKO*/ wire WEKO_FF4A_WRp_new = and2(CUPA_CPU_WRp_new, reg_new.cpu_abus.VYGA_FF4Ap_new());
  /*_p23.VEFU*/ wire VEFU_FF4A_WRn_new = not1(WEKO_FF4A_WRp_new);
  /*_p23.NESO*/ reg_new.reg_wy.NESO_WY0p.dff9(VEFU_FF4A_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p23.NYRO*/ reg_new.reg_wy.NYRO_WY1p.dff9(VEFU_FF4A_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p23.NAGA*/ reg_new.reg_wy.NAGA_WY2p.dff9(VEFU_FF4A_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p23.MELA*/ reg_new.reg_wy.MELA_WY3p.dff9(VEFU_FF4A_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p23.NULO*/ reg_new.reg_wy.NULO_WY4p.dff9(VEFU_FF4A_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p23.NENE*/ reg_new.reg_wy.NENE_WY5p.dff9(VEFU_FF4A_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p23.NUKA*/ reg_new.reg_wy.NUKA_WY6p.dff9(VEFU_FF4A_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p23.NAFU*/ reg_new.reg_wy.NAFU_WY7p.dff9(VEFU_FF4A_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p23.WUZA*/ wire WUZA_FF4B_WRp_new = and2(CUPA_CPU_WRp_new, reg_new.cpu_abus.VUMY_FF4Bp_new());
  /*_p23.VOXU*/ wire VOXU_FF4B_WRn_new = not1(WUZA_FF4B_WRp_new);
  /*_p23.MYPA*/ reg_new.reg_wx.MYPA_WX0p.dff9(VOXU_FF4B_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p23.NOFE*/ reg_new.reg_wx.NOFE_WX1p.dff9(VOXU_FF4B_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p23.NOKE*/ reg_new.reg_wx.NOKE_WX2p.dff9(VOXU_FF4B_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p23.MEBY*/ reg_new.reg_wx.MEBY_WX3p.dff9(VOXU_FF4B_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p23.MYPU*/ reg_new.reg_wx.MYPU_WX4p.dff9(VOXU_FF4B_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p23.MYCE*/ reg_new.reg_wx.MYCE_WX5p.dff9(VOXU_FF4B_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p23.MUVO*/ reg_new.reg_wx.MUVO_WX6p.dff9(VOXU_FF4B_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p23.NUKU*/ reg_new.reg_wx.NUKU_WX7p.dff9(VOXU_FF4B_WRn_new, WALU_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*#p27.NAZE*/ wire NAZE_WY_MATCH0p_odd_new = xnor2(reg_new.reg_wy.NESO_WY0p.qp_new(), reg_new.reg_ly.MUWY_LY0p_odd.qp_new());
  /*_p27.PEBO*/ wire PEBO_WY_MATCH1p_odd_new = xnor2(reg_new.reg_wy.NYRO_WY1p.qp_new(), reg_new.reg_ly.MYRO_LY1p_odd.qp_new());
  /*_p27.POMO*/ wire POMO_WY_MATCH2p_odd_new = xnor2(reg_new.reg_wy.NAGA_WY2p.qp_new(), reg_new.reg_ly.LEXA_LY2p_odd.qp_new());
  /*_p27.NEVU*/ wire NEVU_WY_MATCH3p_odd_new = xnor2(reg_new.reg_wy.MELA_WY3p.qp_new(), reg_new.reg_ly.LYDO_LY3p_odd.qp_new()); 
  /*_p27.NOJO*/ wire NOJO_WY_MATCH4p_odd_new = xnor2(reg_new.reg_wy.NULO_WY4p.qp_new(), reg_new.reg_ly.LOVU_LY4p_odd.qp_new());
  /*_p27.PAGA*/ wire PAGA_WY_MATCH5p_odd_new = xnor2(reg_new.reg_wy.NENE_WY5p.qp_new(), reg_new.reg_ly.LEMA_LY5p_odd.qp_new());
  /*_p27.PEZO*/ wire PEZO_WY_MATCH6p_odd_new = xnor2(reg_new.reg_wy.NUKA_WY6p.qp_new(), reg_new.reg_ly.MATO_LY6p_odd.qp_new());
  /*_p27.NUPA*/ wire NUPA_WY_MATCH7p_odd_new = xnor2(reg_new.reg_wy.NAFU_WY7p.qp_new(), reg_new.reg_ly.LAFO_LY7p_odd.qp_new());

  /*#p27.MYLO*/ wire MYLO_WX_MATCH0p_odd_new = xnor2(reg_new.pix_count.XEHO_PX0p_odd.qp_new(), reg_new.reg_wx.MYPA_WX0p.qp_new());
  /*_p27.PUWU*/ wire PUWU_WX_MATCH1p_odd_new = xnor2(reg_new.pix_count.SAVY_PX1p_odd.qp_new(), reg_new.reg_wx.NOFE_WX1p.qp_new());
  /*_p27.PUHO*/ wire PUHO_WX_MATCH2p_odd_new = xnor2(reg_new.pix_count.XODU_PX2p_odd.qp_new(), reg_new.reg_wx.NOKE_WX2p.qp_new());
  /*_p27.NYTU*/ wire NYTU_WX_MATCH3p_odd_new = xnor2(reg_new.pix_count.XYDO_PX3p_odd.qp_new(), reg_new.reg_wx.MEBY_WX3p.qp_new());
  /*_p27.NEZO*/ wire NEZO_WX_MATCH4p_odd_new = xnor2(reg_new.pix_count.TUHU_PX4p_odd.qp_new(), reg_new.reg_wx.MYPU_WX4p.qp_new());
  /*_p27.NORY*/ wire NORY_WX_MATCH5p_odd_new = xnor2(reg_new.pix_count.TUKY_PX5p_odd.qp_new(), reg_new.reg_wx.MYCE_WX5p.qp_new());
  /*_p27.NONO*/ wire NONO_WX_MATCH6p_odd_new = xnor2(reg_new.pix_count.TAKO_PX6p_odd.qp_new(), reg_new.reg_wx.MUVO_WX6p.qp_new());
  /*_p27.PASE*/ wire PASE_WX_MATCH7p_odd_new = xnor2(reg_new.pix_count.SYBE_PX7p_odd.qp_new(), reg_new.reg_wx.NUKU_WX7p.qp_new());

  /*#p27.PALO*/ wire PALO_WY_MATCHn_odd_new = nand5(reg_new.reg_lcdc.WYMO_LCDC_WINENp.qp_new(), NOJO_WY_MATCH4p_odd_new, PAGA_WY_MATCH5p_odd_new, PEZO_WY_MATCH6p_odd_new, NUPA_WY_MATCH7p_odd_new);
  /*_p27.NELE*/ wire NELE_WY_MATCHp_odd_new = not1(PALO_WY_MATCHn_odd_new);
  /*_p27.PAFU*/ wire PAFU_WY_MATCHn_odd_new = nand5(NELE_WY_MATCHp_odd_new, NAZE_WY_MATCH0p_odd_new, PEBO_WY_MATCH1p_odd_new, POMO_WY_MATCH2p_odd_new, NEVU_WY_MATCH3p_odd_new);

  /*_p27.SARY*/ reg_new.win_ctrl.SARY_WY_MATCHp_odd.dff17(TALU_xxCDEFxx, reg_new.XAPO_VID_RSTn_new(), reg_old.win_ctrl.ROGE_WY_MATCHp_odd.out_old());
  /*_p27.ROGE*/ reg_new.win_ctrl.ROGE_WY_MATCHp_odd <<= not1(PAFU_WY_MATCHn_odd_new);
  /*_p27.REJO*/ reg_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.nor_latch(reg_new.win_ctrl.SARY_WY_MATCHp_odd.qp_new(), REPU_VBLANKp);

  /*_p27.PUKY*/ wire PUKY_WX_MATCHn_odd_new  = nand5(reg_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.qp_new(), NEZO_WX_MATCH4p_odd_new, NORY_WX_MATCH5p_odd_new, NONO_WX_MATCH6p_odd_new, PASE_WX_MATCH7p_odd_new);
  /*_p27.NUFA*/ wire NUFA_WX_MATCHp_odd_new  = not1(PUKY_WX_MATCHn_odd_new);
  /*_p27.NOGY*/ wire NOGY_WX_MATCHn_odd_new  = nand5(NUFA_WX_MATCHp_odd_new, MYLO_WX_MATCH0p_odd_new, PUWU_WX_MATCH1p_odd_new, PUHO_WX_MATCH2p_odd_new, NYTU_WX_MATCH3p_odd_new);
  /*_p27.NUKO*/ reg_new.win_ctrl.NUKO_WX_MATCHp_odd  <<= not1(NOGY_WX_MATCHn_odd_new);

  /*_p07.AJAS*/ wire AJAS_CPU_RDn_new = not1(reg_new.cpu_signals.TEDO_CPU_RDp.out_new());
  /*_p07.ASOT*/ wire ASOT_CPU_RDp_new = not1(AJAS_CPU_RDn_new);
  /*_p23.WAXU*/ wire WAXU_FF4A_RDp_new = and2(ASOT_CPU_RDp_new, reg_new.cpu_abus.VYGA_FF4Ap_new());
  /*_p23.VOMY*/ wire VOMY_FF4A_RDn_new = not1(WAXU_FF4A_RDp_new);
  /*#p23.PUNU*/ triwire PUNU_WY0_TO_CD0_new = tri6_nn(VOMY_FF4A_RDn_new, reg_new.reg_wy.NESO_WY0p.qn_new());
  /*_p23.PODA*/ triwire PODA_WY1_TO_CD1_new = tri6_nn(VOMY_FF4A_RDn_new, reg_new.reg_wy.NYRO_WY1p.qn_new());
  /*_p23.PYGU*/ triwire PYGU_WY2_TO_CD2_new = tri6_nn(VOMY_FF4A_RDn_new, reg_new.reg_wy.NAGA_WY2p.qn_new());
  /*_p23.LOKA*/ triwire LOKA_WY3_TO_CD3_new = tri6_nn(VOMY_FF4A_RDn_new, reg_new.reg_wy.MELA_WY3p.qn_new());
  /*_p23.MEGA*/ triwire MEGA_WY4_TO_CD4_new = tri6_nn(VOMY_FF4A_RDn_new, reg_new.reg_wy.NULO_WY4p.qn_new());
  /*_p23.PELA*/ triwire PELA_WY5_TO_CD5_new = tri6_nn(VOMY_FF4A_RDn_new, reg_new.reg_wy.NENE_WY5p.qn_new());
  /*_p23.POLO*/ triwire POLO_WY6_TO_CD6_new = tri6_nn(VOMY_FF4A_RDn_new, reg_new.reg_wy.NUKA_WY6p.qn_new());
  /*_p23.MERA*/ triwire MERA_WY7_TO_CD7_new = tri6_nn(VOMY_FF4A_RDn_new, reg_new.reg_wy.NAFU_WY7p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(PUNU_WY0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(PODA_WY1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(PYGU_WY2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(LOKA_WY3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(MEGA_WY4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(PELA_WY5_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(POLO_WY6_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(MERA_WY7_TO_CD7_new);

  /*_p23.WYZE*/ wire WYZE_FF4B_RDp_new = and2(ASOT_CPU_RDp_new, reg_new.cpu_abus.VUMY_FF4Bp_new());
  /*_p23.VYCU*/ wire VYCU_FF4B_RDn_new = not1(WYZE_FF4B_RDp_new);
  /*#p23.LOVA*/ triwire LOVA_WX0_TO_CD0_new = tri6_nn(VYCU_FF4B_RDn_new, reg_new.reg_wx.MYPA_WX0p.qn_new());
  /*_p23.MUKA*/ triwire MUKA_WX1_TO_CD1_new = tri6_nn(VYCU_FF4B_RDn_new, reg_new.reg_wx.NOFE_WX1p.qn_new());
  /*_p23.MOKO*/ triwire MOKO_WX2_TO_CD2_new = tri6_nn(VYCU_FF4B_RDn_new, reg_new.reg_wx.NOKE_WX2p.qn_new());
  /*_p23.LOLE*/ triwire LOLE_WX3_TO_CD3_new = tri6_nn(VYCU_FF4B_RDn_new, reg_new.reg_wx.MEBY_WX3p.qn_new());
  /*_p23.MELE*/ triwire MELE_WX4_TO_CD4_new = tri6_nn(VYCU_FF4B_RDn_new, reg_new.reg_wx.MYPU_WX4p.qn_new());
  /*_p23.MUFE*/ triwire MUFE_WX5_TO_CD5_new = tri6_nn(VYCU_FF4B_RDn_new, reg_new.reg_wx.MYCE_WX5p.qn_new());
  /*_p23.MULY*/ triwire MULY_WX6_TO_CD6_new = tri6_nn(VYCU_FF4B_RDn_new, reg_new.reg_wx.MUVO_WX6p.qn_new());
  /*_p23.MARA*/ triwire MARA_WX7_TO_CD7_new = tri6_nn(VYCU_FF4B_RDn_new, reg_new.reg_wx.NUKU_WX7p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(LOVA_WX0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(MUKA_WX1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(MOKO_WX2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(LOLE_WX3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(MELE_WX4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(MUFE_WX5_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(MULY_WX6_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(MARA_WX7_TO_CD7_new);
}

//-----------------------------------------------------------------------------

void GateBoy::tock_pix_pipes_gates(const GateBoyState& reg_old, wire SACU_CLKPIPE_odd_new, wire NYXU_BFETCH_RSTn_new)
{
  auto& reg_new = gb_state;

  /*_p07.DYKY*/ wire DYKY_CPU_WRn_new = not1(reg_new.cpu_signals.TAPU_CPU_WRp.out_new());
  /*_p07.CUPA*/ wire CUPA_CPU_WRp_new = not1(DYKY_CPU_WRn_new);

  /*_p24.LOBY*/ wire LOBY_RENDERINGn_new = not1(reg_new.XYMU_RENDERING_LATCHn.qn_new());

  /*#p27.LAXE*/ wire LAXE_BFETCH_S0n_new = not1(reg_new.tfetch_counter.LAXU_BFETCH_S0p_odd.qp_new());
  /*#p27.NAKO*/ wire NAKO_BFETCH_S1n_new = not1(reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.qp_new());
  /*#p27.NOFU*/ wire NOFU_BFETCH_S2n_new = not1(reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.qp_new());
  /*_p29.TYTU*/ wire TYTU_SFETCH_S0n_new = not1(reg_new.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.qp_new());
  /*_p29.SYCU*/ wire SYCU_SFETCH_S0pe_new = nor3(TYTU_SFETCH_S0n_new, LOBY_RENDERINGn_new, reg_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.qp_new());

  /*#p27.MYSO*/ wire MYSO_STORE_VRAM_DATA_TRIGp_new = nor3(LOBY_RENDERINGn_new, LAXE_BFETCH_S0n_new, reg_new.tfetch_control.LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /*#p27.NYDY*/ wire NYDY_LATCH_TILE_DAn_new = nand3(MYSO_STORE_VRAM_DATA_TRIGp_new, reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.qp_new(), NOFU_BFETCH_S2n_new); // NYDY on fetch phase 6
  /*_p27.MOFU*/ wire MOFU_LATCH_TILE_DBp_new = and2(MYSO_STORE_VRAM_DATA_TRIGp_new, NAKO_BFETCH_S1n_new); // MOFU fires on fetch phase 2 and 10
  /*#p29.TOPU*/ wire TOPU_STORE_SPRITE_Ap_new = and2(reg_new.sfetch_counter_evn.TULY_SFETCH_S1p_evn.qp_new(), SYCU_SFETCH_S0pe_new);
  /*#p29.RACA*/ wire RACA_STORE_SPRITE_Bp_new = and2(reg_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.qp_new(), SYCU_SFETCH_S0pe_new);

  {
    /*#p32.METE*/ wire METE_LATCH_TILE_DAp_new = not1(NYDY_LATCH_TILE_DAn_new);
    /*#p32.LOMA*/ wire LOMA_LATCH_TILE_DAn_new = not1(METE_LATCH_TILE_DAp_new);
    /*_p32.LEGU*/ reg_new.tile_temp_a.LEGU_TILE_DA0p.dff8(LOMA_LATCH_TILE_DAn_new, reg_old.vram_dbus.BUS_VRAM_D00p.out_old());
    /*_p32.NUDU*/ reg_new.tile_temp_a.NUDU_TILE_DA1p.dff8(LOMA_LATCH_TILE_DAn_new, reg_old.vram_dbus.BUS_VRAM_D01p.out_old());
    /*_p32.MUKU*/ reg_new.tile_temp_a.MUKU_TILE_DA2p.dff8(LOMA_LATCH_TILE_DAn_new, reg_old.vram_dbus.BUS_VRAM_D02p.out_old());
    /*_p32.LUZO*/ reg_new.tile_temp_a.LUZO_TILE_DA3p.dff8(LOMA_LATCH_TILE_DAn_new, reg_old.vram_dbus.BUS_VRAM_D03p.out_old());
    /*_p32.MEGU*/ reg_new.tile_temp_a.MEGU_TILE_DA4p.dff8(LOMA_LATCH_TILE_DAn_new, reg_old.vram_dbus.BUS_VRAM_D04p.out_old());
    /*_p32.MYJY*/ reg_new.tile_temp_a.MYJY_TILE_DA5p.dff8(LOMA_LATCH_TILE_DAn_new, reg_old.vram_dbus.BUS_VRAM_D05p.out_old());
    /*_p32.NASA*/ reg_new.tile_temp_a.NASA_TILE_DA6p.dff8(LOMA_LATCH_TILE_DAn_new, reg_old.vram_dbus.BUS_VRAM_D06p.out_old());
    /*_p32.NEFO*/ reg_new.tile_temp_a.NEFO_TILE_DA7p.dff8(LOMA_LATCH_TILE_DAn_new, reg_old.vram_dbus.BUS_VRAM_D07p.out_old());
  }

  {
    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.

    /*_p32.LESO*/ wire LESO_LATCH_TILE_DBn_new = not1(MOFU_LATCH_TILE_DBp_new);
    /*_p32.LUVE*/ wire LUVE_LATCH_TILE_DBp_new = not1(LESO_LATCH_TILE_DBn_new); // Schematic wrong, was labeled AJAR
    /*_p32.LABU*/ wire LABU_LATCH_TILE_DBn_new = not1(LUVE_LATCH_TILE_DBp_new);
    /*_p32.RAWU*/ reg_new.tile_temp_b.RAWU_TILE_DB0p.dff11(LABU_LATCH_TILE_DBn_new, reg_new.SIG_VCC.out_new(), reg_old.vram_dbus.BUS_VRAM_D00p.out_old());
    /*_p32.POZO*/ reg_new.tile_temp_b.POZO_TILE_DB1p.dff11(LABU_LATCH_TILE_DBn_new, reg_new.SIG_VCC.out_new(), reg_old.vram_dbus.BUS_VRAM_D01p.out_old());
    /*_p32.PYZO*/ reg_new.tile_temp_b.PYZO_TILE_DB2p.dff11(LABU_LATCH_TILE_DBn_new, reg_new.SIG_VCC.out_new(), reg_old.vram_dbus.BUS_VRAM_D02p.out_old());
    /*_p32.POXA*/ reg_new.tile_temp_b.POXA_TILE_DB3p.dff11(LABU_LATCH_TILE_DBn_new, reg_new.SIG_VCC.out_new(), reg_old.vram_dbus.BUS_VRAM_D03p.out_old());
    /*_p32.PULO*/ reg_new.tile_temp_b.PULO_TILE_DB4p.dff11(LABU_LATCH_TILE_DBn_new, reg_new.SIG_VCC.out_new(), reg_old.vram_dbus.BUS_VRAM_D04p.out_old());
    /*_p32.POJU*/ reg_new.tile_temp_b.POJU_TILE_DB5p.dff11(LABU_LATCH_TILE_DBn_new, reg_new.SIG_VCC.out_new(), reg_old.vram_dbus.BUS_VRAM_D05p.out_old());
    /*_p32.POWY*/ reg_new.tile_temp_b.POWY_TILE_DB6p.dff11(LABU_LATCH_TILE_DBn_new, reg_new.SIG_VCC.out_new(), reg_old.vram_dbus.BUS_VRAM_D06p.out_old());
    /*_p32.PYJU*/ reg_new.tile_temp_b.PYJU_TILE_DB7p.dff11(LABU_LATCH_TILE_DBn_new, reg_new.SIG_VCC.out_new(), reg_old.vram_dbus.BUS_VRAM_D07p.out_old());
  }

  {
    /*#p29.VYWA*/ wire VYWA_STORE_SPRITE_An_new = not1(TOPU_STORE_SPRITE_Ap_new);
    /*#p29.WENY*/ wire WENY_STORE_SPRITE_Ap_new = not1(VYWA_STORE_SPRITE_An_new);
    /*#p29.XADO*/ wire XADO_STORE_SPRITE_An_new = not1(WENY_STORE_SPRITE_Ap_new);
    /*_p33.REWO*/ reg_new.sprite_pix_a.REWO_SPRITE_DA0p.dff8(XADO_STORE_SPRITE_An_new, reg_old.flipped_sprite.PUTE_FLIP0p.out_old());
    /*_p33.PEBA*/ reg_new.sprite_pix_a.PEBA_SPRITE_DA1p.dff8(XADO_STORE_SPRITE_An_new, reg_old.flipped_sprite.PELO_FLIP1p.out_old());
    /*_p33.MOFO*/ reg_new.sprite_pix_a.MOFO_SPRITE_DA2p.dff8(XADO_STORE_SPRITE_An_new, reg_old.flipped_sprite.PONO_FLIP2p.out_old());
    /*_p33.PUDU*/ reg_new.sprite_pix_a.PUDU_SPRITE_DA3p.dff8(XADO_STORE_SPRITE_An_new, reg_old.flipped_sprite.POBE_FLIP3p.out_old());
    /*_p33.SAJA*/ reg_new.sprite_pix_a.SAJA_SPRITE_DA4p.dff8(XADO_STORE_SPRITE_An_new, reg_old.flipped_sprite.PACY_FLIP4p.out_old());
    /*_p33.SUNY*/ reg_new.sprite_pix_a.SUNY_SPRITE_DA5p.dff8(XADO_STORE_SPRITE_An_new, reg_old.flipped_sprite.PUGU_FLIP5p.out_old());
    /*_p33.SEMO*/ reg_new.sprite_pix_a.SEMO_SPRITE_DA6p.dff8(XADO_STORE_SPRITE_An_new, reg_old.flipped_sprite.PAWE_FLIP6p.out_old());
    /*_p33.SEGA*/ reg_new.sprite_pix_a.SEGA_SPRITE_DA7p.dff8(XADO_STORE_SPRITE_An_new, reg_old.flipped_sprite.PULY_FLIP7p.out_old());
  }

  {
    /*#p29.PEBY*/ wire PEBY_STORE_SPRITE_Bn_new = not1(RACA_STORE_SPRITE_Bp_new);
    /*#p29.NYBE*/ wire NYBE_STORE_SPRITE_Bp_new = not1(PEBY_STORE_SPRITE_Bn_new);
    /*#p29.PUCO*/ wire PUCO_STORE_SPRITE_Bn_new = not1(NYBE_STORE_SPRITE_Bp_new);
    /*_p33.PEFO*/ reg_new.sprite_pix_b.PEFO_SPRITE_DB0p.dff8(PUCO_STORE_SPRITE_Bn_new, reg_old.flipped_sprite.PUTE_FLIP0p.out_old());
    /*_p33.ROKA*/ reg_new.sprite_pix_b.ROKA_SPRITE_DB1p.dff8(PUCO_STORE_SPRITE_Bn_new, reg_old.flipped_sprite.PELO_FLIP1p.out_old());
    /*_p33.MYTU*/ reg_new.sprite_pix_b.MYTU_SPRITE_DB2p.dff8(PUCO_STORE_SPRITE_Bn_new, reg_old.flipped_sprite.PONO_FLIP2p.out_old());
    /*_p33.RAMU*/ reg_new.sprite_pix_b.RAMU_SPRITE_DB3p.dff8(PUCO_STORE_SPRITE_Bn_new, reg_old.flipped_sprite.POBE_FLIP3p.out_old());
    /*_p33.SELE*/ reg_new.sprite_pix_b.SELE_SPRITE_DB4p.dff8(PUCO_STORE_SPRITE_Bn_new, reg_old.flipped_sprite.PACY_FLIP4p.out_old());
    /*_p33.SUTO*/ reg_new.sprite_pix_b.SUTO_SPRITE_DB5p.dff8(PUCO_STORE_SPRITE_Bn_new, reg_old.flipped_sprite.PUGU_FLIP5p.out_old());
    /*_p33.RAMA*/ reg_new.sprite_pix_b.RAMA_SPRITE_DB6p.dff8(PUCO_STORE_SPRITE_Bn_new, reg_old.flipped_sprite.PAWE_FLIP6p.out_old());
    /*_p33.RYDU*/ reg_new.sprite_pix_b.RYDU_SPRITE_DB7p.dff8(PUCO_STORE_SPRITE_Bn_new, reg_old.flipped_sprite.PULY_FLIP7p.out_old());
  }

  //----------------------------------------
  // Sprite pipe A
  // FIXME - old? well,i guess there's another feedback loop here...

  /*_p33.NURO*/ reg_new.spr_pipe_a.NURO_SPR_PIPE_A0.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.SIG_GND.out_old());
  /*_p33.MASO*/ reg_new.spr_pipe_a.MASO_SPR_PIPE_A1.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_a.NURO_SPR_PIPE_A0.qp_old());
  /*_p33.LEFE*/ reg_new.spr_pipe_a.LEFE_SPR_PIPE_A2.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_a.MASO_SPR_PIPE_A1.qp_old());
  /*_p33.LESU*/ reg_new.spr_pipe_a.LESU_SPR_PIPE_A3.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_a.LEFE_SPR_PIPE_A2.qp_old());
  /*_p33.WYHO*/ reg_new.spr_pipe_a.WYHO_SPR_PIPE_A4.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_a.LESU_SPR_PIPE_A3.qp_old());
  /*_p33.WORA*/ reg_new.spr_pipe_a.WORA_SPR_PIPE_A5.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_a.WYHO_SPR_PIPE_A4.qp_old());
  /*_p33.VAFO*/ reg_new.spr_pipe_a.VAFO_SPR_PIPE_A6.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_a.WORA_SPR_PIPE_A5.qp_old());
  /*_p33.WUFY*/ reg_new.spr_pipe_a.WUFY_SPR_PIPE_A7.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_a.VAFO_SPR_PIPE_A6.qp_old());

  /*_p33.NYLU*/ reg_new.spr_pipe_b.NYLU_SPR_PIPE_B0.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.SIG_GND.out_old());
  /*_p33.PEFU*/ reg_new.spr_pipe_b.PEFU_SPR_PIPE_B1.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_b.NYLU_SPR_PIPE_B0.qp_old());
  /*_p33.NATY*/ reg_new.spr_pipe_b.NATY_SPR_PIPE_B2.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_b.PEFU_SPR_PIPE_B1.qp_old());
  /*_p33.PYJO*/ reg_new.spr_pipe_b.PYJO_SPR_PIPE_B3.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_b.NATY_SPR_PIPE_B2.qp_old());
  /*_p33.VARE*/ reg_new.spr_pipe_b.VARE_SPR_PIPE_B4.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_b.PYJO_SPR_PIPE_B3.qp_old());
  /*_p33.WEBA*/ reg_new.spr_pipe_b.WEBA_SPR_PIPE_B5.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_b.VARE_SPR_PIPE_B4.qp_old());
  /*_p33.VANU*/ reg_new.spr_pipe_b.VANU_SPR_PIPE_B6.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_b.WEBA_SPR_PIPE_B5.qp_old());
  /*_p33.VUPY*/ reg_new.spr_pipe_b.VUPY_SPR_PIPE_B7.dff22_sync(SACU_CLKPIPE_odd_new, reg_old.spr_pipe_b.VANU_SPR_PIPE_B6.qp_old());


  /*_p29.XEFY*/ wire XEFY_SPRITE_DONEn_odd_new = not1(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new());

  /*_p34.MEFU*/ wire MEFU_SPRITE_MASK0n_new = or3(XEFY_SPRITE_DONEn_odd_new, reg_new.spr_pipe_b.NYLU_SPR_PIPE_B0.qp_mid(), reg_new.spr_pipe_a.NURO_SPR_PIPE_A0.qp_mid());
  /*_p34.MEVE*/ wire MEVE_SPRITE_MASK1n_new = or3(XEFY_SPRITE_DONEn_odd_new, reg_new.spr_pipe_b.PEFU_SPR_PIPE_B1.qp_mid(), reg_new.spr_pipe_a.MASO_SPR_PIPE_A1.qp_mid());
  /*_p34.MYZO*/ wire MYZO_SPRITE_MASK2n_new = or3(XEFY_SPRITE_DONEn_odd_new, reg_new.spr_pipe_b.NATY_SPR_PIPE_B2.qp_mid(), reg_new.spr_pipe_a.LEFE_SPR_PIPE_A2.qp_mid());
  /*_p34.RUDA*/ wire RUDA_SPRITE_MASK3n_new = or3(XEFY_SPRITE_DONEn_odd_new, reg_new.spr_pipe_b.PYJO_SPR_PIPE_B3.qp_mid(), reg_new.spr_pipe_a.LESU_SPR_PIPE_A3.qp_mid());
  /*_p34.VOTO*/ wire VOTO_SPRITE_MASK4n_new = or3(XEFY_SPRITE_DONEn_odd_new, reg_new.spr_pipe_b.VARE_SPR_PIPE_B4.qp_mid(), reg_new.spr_pipe_a.WYHO_SPR_PIPE_A4.qp_mid());
  /*_p34.VYSA*/ wire VYSA_SPRITE_MASK5n_new = or3(XEFY_SPRITE_DONEn_odd_new, reg_new.spr_pipe_b.WEBA_SPR_PIPE_B5.qp_mid(), reg_new.spr_pipe_a.WORA_SPR_PIPE_A5.qp_mid());
  /*_p34.TORY*/ wire TORY_SPRITE_MASK6n_new = or3(XEFY_SPRITE_DONEn_odd_new, reg_new.spr_pipe_b.VANU_SPR_PIPE_B6.qp_mid(), reg_new.spr_pipe_a.VAFO_SPR_PIPE_A6.qp_mid());
  /*_p34.WOPE*/ wire WOPE_SPRITE_MASK7n_new = or3(XEFY_SPRITE_DONEn_odd_new, reg_new.spr_pipe_b.VUPY_SPR_PIPE_B7.qp_mid(), reg_new.spr_pipe_a.WUFY_SPR_PIPE_A7.qp_mid());

  /*_p34.LESY*/ wire LESY_SPRITE_MASK0p_new = not1(MEFU_SPRITE_MASK0n_new);
  /*_p34.LOTA*/ wire LOTA_SPRITE_MASK1p_new = not1(MEVE_SPRITE_MASK1n_new);
  /*_p34.LYKU*/ wire LYKU_SPRITE_MASK2p_new = not1(MYZO_SPRITE_MASK2n_new);
  /*_p34.ROBY*/ wire ROBY_SPRITE_MASK3p_new = not1(RUDA_SPRITE_MASK3n_new);
  /*_p34.TYTA*/ wire TYTA_SPRITE_MASK4p_new = not1(VOTO_SPRITE_MASK4n_new);
  /*_p34.TYCO*/ wire TYCO_SPRITE_MASK5p_new = not1(VYSA_SPRITE_MASK5n_new);
  /*_p34.SOKA*/ wire SOKA_SPRITE_MASK6p_new = not1(TORY_SPRITE_MASK6n_new);
  /*_p34.XOVU*/ wire XOVU_SPRITE_MASK7p_new = not1(WOPE_SPRITE_MASK7n_new);

  {
    /*_p33.PABE*/ wire PABE_SPR_PIX_SET0_new = nand2(LESY_SPRITE_MASK0p_new, reg_new.sprite_pix_a.REWO_SPRITE_DA0p.qp_new());
    /*_p33.MYTO*/ wire MYTO_SPR_PIX_SET1_new = nand2(LOTA_SPRITE_MASK1p_new, reg_new.sprite_pix_a.PEBA_SPRITE_DA1p.qp_new());
    /*_p33.LELA*/ wire LELA_SPR_PIX_SET2_new = nand2(LYKU_SPRITE_MASK2p_new, reg_new.sprite_pix_a.MOFO_SPRITE_DA2p.qp_new());
    /*_p33.MAME*/ wire MAME_SPR_PIX_SET3_new = nand2(ROBY_SPRITE_MASK3p_new, reg_new.sprite_pix_a.PUDU_SPRITE_DA3p.qp_new());
    /*_p33.VEXU*/ wire VEXU_SPR_PIX_SET4_new = nand2(TYTA_SPRITE_MASK4p_new, reg_new.sprite_pix_a.SAJA_SPRITE_DA4p.qp_new());
    /*_p33.VABY*/ wire VABY_SPR_PIX_SET5_new = nand2(TYCO_SPRITE_MASK5p_new, reg_new.sprite_pix_a.SUNY_SPRITE_DA5p.qp_new());
    /*_p33.TUXA*/ wire TUXA_SPR_PIX_SET6_new = nand2(SOKA_SPRITE_MASK6p_new, reg_new.sprite_pix_a.SEMO_SPRITE_DA6p.qp_new());
    /*_p33.VUNE*/ wire VUNE_SPR_PIX_SET7_new = nand2(XOVU_SPRITE_MASK7p_new, reg_new.sprite_pix_a.SEGA_SPRITE_DA7p.qp_new());

    /*_p33.RATA*/ wire RATA_SPR_PIX_DA0n_new = not1(reg_new.sprite_pix_a.REWO_SPRITE_DA0p.qp_new());
    /*_p33.NUCA*/ wire NUCA_SPR_PIX_DA1n_new = not1(reg_new.sprite_pix_a.PEBA_SPRITE_DA1p.qp_new());
    /*_p33.LASE*/ wire LASE_SPR_PIX_DA2n_new = not1(reg_new.sprite_pix_a.MOFO_SPRITE_DA2p.qp_new());
    /*_p33.LUBO*/ wire LUBO_SPR_PIX_DA3n_new = not1(reg_new.sprite_pix_a.PUDU_SPRITE_DA3p.qp_new());
    /*_p33.WERY*/ wire WERY_SPR_PIX_DA4n_new = not1(reg_new.sprite_pix_a.SAJA_SPRITE_DA4p.qp_new());
    /*_p33.WURA*/ wire WURA_SPR_PIX_DA5n_new = not1(reg_new.sprite_pix_a.SUNY_SPRITE_DA5p.qp_new());
    /*_p33.SULU*/ wire SULU_SPR_PIX_DA6n_new = not1(reg_new.sprite_pix_a.SEMO_SPRITE_DA6p.qp_new());
    /*_p33.WAMY*/ wire WAMY_SPR_PIX_DA7n_new = not1(reg_new.sprite_pix_a.SEGA_SPRITE_DA7p.qp_new());

    /*_p33.PYZU*/ wire PYZU_SPR_PIX_RST0_new = nand2(LESY_SPRITE_MASK0p_new, RATA_SPR_PIX_DA0n_new);
    /*_p33.MADA*/ wire MADA_SPR_PIX_RST1_new = nand2(LOTA_SPRITE_MASK1p_new, NUCA_SPR_PIX_DA1n_new);
    /*_p33.LYDE*/ wire LYDE_SPR_PIX_RST2_new = nand2(LYKU_SPRITE_MASK2p_new, LASE_SPR_PIX_DA2n_new);
    /*_p33.LUFY*/ wire LUFY_SPR_PIX_RST3_new = nand2(ROBY_SPRITE_MASK3p_new, LUBO_SPR_PIX_DA3n_new);
    /*_p33.XATO*/ wire XATO_SPR_PIX_RST4_new = nand2(TYTA_SPRITE_MASK4p_new, WERY_SPR_PIX_DA4n_new);
    /*_p33.XEXU*/ wire XEXU_SPR_PIX_RST5_new = nand2(TYCO_SPRITE_MASK5p_new, WURA_SPR_PIX_DA5n_new);
    /*_p33.TUPE*/ wire TUPE_SPR_PIX_RST6_new = nand2(SOKA_SPRITE_MASK6p_new, SULU_SPR_PIX_DA6n_new);
    /*_p33.XYVE*/ wire XYVE_SPR_PIX_RST7_new = nand2(XOVU_SPRITE_MASK7p_new, WAMY_SPR_PIX_DA7n_new);

    /*_p33.NURO*/ reg_new.spr_pipe_a.NURO_SPR_PIPE_A0.dff22_async(PABE_SPR_PIX_SET0_new, PYZU_SPR_PIX_RST0_new);
    /*_p33.MASO*/ reg_new.spr_pipe_a.MASO_SPR_PIPE_A1.dff22_async(MYTO_SPR_PIX_SET1_new, MADA_SPR_PIX_RST1_new);
    /*_p33.LEFE*/ reg_new.spr_pipe_a.LEFE_SPR_PIPE_A2.dff22_async(LELA_SPR_PIX_SET2_new, LYDE_SPR_PIX_RST2_new);
    /*_p33.LESU*/ reg_new.spr_pipe_a.LESU_SPR_PIPE_A3.dff22_async(MAME_SPR_PIX_SET3_new, LUFY_SPR_PIX_RST3_new);
    /*_p33.WYHO*/ reg_new.spr_pipe_a.WYHO_SPR_PIPE_A4.dff22_async(VEXU_SPR_PIX_SET4_new, XATO_SPR_PIX_RST4_new);
    /*_p33.WORA*/ reg_new.spr_pipe_a.WORA_SPR_PIPE_A5.dff22_async(VABY_SPR_PIX_SET5_new, XEXU_SPR_PIX_RST5_new);
    /*_p33.VAFO*/ reg_new.spr_pipe_a.VAFO_SPR_PIPE_A6.dff22_async(TUXA_SPR_PIX_SET6_new, TUPE_SPR_PIX_RST6_new);
    /*_p33.WUFY*/ reg_new.spr_pipe_a.WUFY_SPR_PIPE_A7.dff22_async(VUNE_SPR_PIX_SET7_new, XYVE_SPR_PIX_RST7_new);
  }

  {
    /*_p33.MEZU*/ wire MEZU_SPR_PIX_SET0_new = nand2(LESY_SPRITE_MASK0p_new, reg_new.sprite_pix_b.PEFO_SPRITE_DB0p.qp_new());
    /*_p33.RUSY*/ wire RUSY_SPR_PIX_SET1_new = nand2(LOTA_SPRITE_MASK1p_new, reg_new.sprite_pix_b.ROKA_SPRITE_DB1p.qp_new());
    /*_p33.MYXA*/ wire MYXA_SPR_PIX_SET2_new = nand2(LYKU_SPRITE_MASK2p_new, reg_new.sprite_pix_b.MYTU_SPRITE_DB2p.qp_new());
    /*_p33.RANO*/ wire RANO_SPR_PIX_SET3_new = nand2(ROBY_SPRITE_MASK3p_new, reg_new.sprite_pix_b.RAMU_SPRITE_DB3p.qp_new());
    /*_p33.TYGA*/ wire TYGA_SPR_PIX_SET4_new = nand2(TYTA_SPRITE_MASK4p_new, reg_new.sprite_pix_b.SELE_SPRITE_DB4p.qp_new());
    /*_p33.VUME*/ wire VUME_SPR_PIX_SET5_new = nand2(TYCO_SPRITE_MASK5p_new, reg_new.sprite_pix_b.SUTO_SPRITE_DB5p.qp_new());
    /*_p33.TAPO*/ wire TAPO_SPR_PIX_SET6_new = nand2(SOKA_SPRITE_MASK6p_new, reg_new.sprite_pix_b.RAMA_SPRITE_DB6p.qp_new());
    /*_p33.TESO*/ wire TESO_SPR_PIX_SET7_new = nand2(XOVU_SPRITE_MASK7p_new, reg_new.sprite_pix_b.RYDU_SPRITE_DB7p.qp_new());

    /*_p33.LOZA*/ wire LOZA_SPR_PIX_DB0n_new = not1(reg_new.sprite_pix_b.PEFO_SPRITE_DB0p.qp_new());
    /*_p33.SYBO*/ wire SYBO_SPR_PIX_DB1n_new = not1(reg_new.sprite_pix_b.ROKA_SPRITE_DB1p.qp_new());
    /*_p33.LUMO*/ wire LUMO_SPR_PIX_DB2n_new = not1(reg_new.sprite_pix_b.MYTU_SPRITE_DB2p.qp_new());
    /*_p33.SOLO*/ wire SOLO_SPR_PIX_DB3n_new = not1(reg_new.sprite_pix_b.RAMU_SPRITE_DB3p.qp_new());
    /*_p33.VOBY*/ wire VOBY_SPR_PIX_DB4n_new = not1(reg_new.sprite_pix_b.SELE_SPRITE_DB4p.qp_new());
    /*_p33.WYCO*/ wire WYCO_SPR_PIX_DB5n_new = not1(reg_new.sprite_pix_b.SUTO_SPRITE_DB5p.qp_new());
    /*_p33.SERY*/ wire SERY_SPR_PIX_DB6n_new = not1(reg_new.sprite_pix_b.RAMA_SPRITE_DB6p.qp_new());
    /*_p33.SELU*/ wire SELU_SPR_PIX_DB7n_new = not1(reg_new.sprite_pix_b.RYDU_SPRITE_DB7p.qp_new());

    /*_p33.MOFY*/ wire MOFY_SPR_PIX_RST0_new = nand2(LESY_SPRITE_MASK0p_new, LOZA_SPR_PIX_DB0n_new);
    /*_p33.RUCA*/ wire RUCA_SPR_PIX_RST1_new = nand2(LOTA_SPRITE_MASK1p_new, SYBO_SPR_PIX_DB1n_new);
    /*_p33.MAJO*/ wire MAJO_SPR_PIX_RST2_new = nand2(LYKU_SPRITE_MASK2p_new, LUMO_SPR_PIX_DB2n_new);
    /*_p33.REHU*/ wire REHU_SPR_PIX_RST3_new = nand2(ROBY_SPRITE_MASK3p_new, SOLO_SPR_PIX_DB3n_new);
    /*_p33.WAXO*/ wire WAXO_SPR_PIX_RST4_new = nand2(TYTA_SPRITE_MASK4p_new, VOBY_SPR_PIX_DB4n_new);
    /*_p33.XOLE*/ wire XOLE_SPR_PIX_RST5_new = nand2(TYCO_SPRITE_MASK5p_new, WYCO_SPR_PIX_DB5n_new);
    /*_p33.TABY*/ wire TABY_SPR_PIX_RST6_new = nand2(SOKA_SPRITE_MASK6p_new, SERY_SPR_PIX_DB6n_new);
    /*_p33.TULA*/ wire TULA_SPR_PIX_RST7_new = nand2(XOVU_SPRITE_MASK7p_new, SELU_SPR_PIX_DB7n_new);

    /*_p33.NYLU*/ reg_new.spr_pipe_b.NYLU_SPR_PIPE_B0.dff22_async(MEZU_SPR_PIX_SET0_new, MOFY_SPR_PIX_RST0_new);
    /*_p33.PEFU*/ reg_new.spr_pipe_b.PEFU_SPR_PIPE_B1.dff22_async(RUSY_SPR_PIX_SET1_new, RUCA_SPR_PIX_RST1_new);
    /*_p33.NATY*/ reg_new.spr_pipe_b.NATY_SPR_PIPE_B2.dff22_async(MYXA_SPR_PIX_SET2_new, MAJO_SPR_PIX_RST2_new);
    /*_p33.PYJO*/ reg_new.spr_pipe_b.PYJO_SPR_PIPE_B3.dff22_async(RANO_SPR_PIX_SET3_new, REHU_SPR_PIX_RST3_new);
    /*_p33.VARE*/ reg_new.spr_pipe_b.VARE_SPR_PIPE_B4.dff22_async(TYGA_SPR_PIX_SET4_new, WAXO_SPR_PIX_RST4_new);
    /*_p33.WEBA*/ reg_new.spr_pipe_b.WEBA_SPR_PIPE_B5.dff22_async(VUME_SPR_PIX_SET5_new, XOLE_SPR_PIX_RST5_new);
    /*_p33.VANU*/ reg_new.spr_pipe_b.VANU_SPR_PIPE_B6.dff22_async(TAPO_SPR_PIX_SET6_new, TABY_SPR_PIX_RST6_new);
    /*_p33.VUPY*/ reg_new.spr_pipe_b.VUPY_SPR_PIPE_B7.dff22_async(TESO_SPR_PIX_SET7_new, TULA_SPR_PIX_RST7_new);
  }

  //----------------------------------------
  // Background pipe A
  {
    /*_p32.LOZE*/ wire LOZE_PIPE_A_LOADp_new = not1(NYXU_BFETCH_RSTn_new);
    /*_p32.LAKY*/ wire LAKY_BG_PIX_SET0_new = nand2(LOZE_PIPE_A_LOADp_new, reg_new.tile_temp_a.LEGU_TILE_DA0p.qp_new());
    /*_p32.NYXO*/ wire NYXO_BG_PIX_SET1_new = nand2(LOZE_PIPE_A_LOADp_new, reg_new.tile_temp_a.NUDU_TILE_DA1p.qp_new());
    /*_p32.LOTO*/ wire LOTO_BG_PIX_SET2_new = nand2(LOZE_PIPE_A_LOADp_new, reg_new.tile_temp_a.MUKU_TILE_DA2p.qp_new());
    /*_p32.LYDU*/ wire LYDU_BG_PIX_SET3_new = nand2(LOZE_PIPE_A_LOADp_new, reg_new.tile_temp_a.LUZO_TILE_DA3p.qp_new());
    /*_p32.MYVY*/ wire MYVY_BG_PIX_SET4_new = nand2(LOZE_PIPE_A_LOADp_new, reg_new.tile_temp_a.MEGU_TILE_DA4p.qp_new());
    /*_p32.LODO*/ wire LODO_BG_PIX_SET5_new = nand2(LOZE_PIPE_A_LOADp_new, reg_new.tile_temp_a.MYJY_TILE_DA5p.qp_new());
    /*_p32.NUTE*/ wire NUTE_BG_PIX_SET6_new = nand2(LOZE_PIPE_A_LOADp_new, reg_new.tile_temp_a.NASA_TILE_DA6p.qp_new());
    /*_p32.NAJA*/ wire NAJA_BG_PIX_SET7_new = nand2(LOZE_PIPE_A_LOADp_new, reg_new.tile_temp_a.NEFO_TILE_DA7p.qp_new());

    /*_p32.LUHE*/ wire LUHE_BG_PIX_DA0n_new = not1(reg_new.tile_temp_a.LEGU_TILE_DA0p.qp_new());
    /*_p32.NOLY*/ wire NOLY_BG_PIX_DA1n_new = not1(reg_new.tile_temp_a.NUDU_TILE_DA1p.qp_new());
    /*_p32.LEKE*/ wire LEKE_BG_PIX_DA2n_new = not1(reg_new.tile_temp_a.MUKU_TILE_DA2p.qp_new());
    /*_p32.LOMY*/ wire LOMY_BG_PIX_DA3n_new = not1(reg_new.tile_temp_a.LUZO_TILE_DA3p.qp_new());
    /*_p32.LALA*/ wire LALA_BG_PIX_DA4n_new = not1(reg_new.tile_temp_a.MEGU_TILE_DA4p.qp_new());
    /*_p32.LOXA*/ wire LOXA_BG_PIX_DA5n_new = not1(reg_new.tile_temp_a.MYJY_TILE_DA5p.qp_new());
    /*_p32.NEZE*/ wire NEZE_BG_PIX_DA6n_new = not1(reg_new.tile_temp_a.NASA_TILE_DA6p.qp_new());
    /*_p32.NOBO*/ wire NOBO_BG_PIX_DA7n_new = not1(reg_new.tile_temp_a.NEFO_TILE_DA7p.qp_new());

    /*_p32.LOTY*/ wire LOTY_BG_PIX_RST0_new = nand2(LOZE_PIPE_A_LOADp_new, LUHE_BG_PIX_DA0n_new);
    /*_p32.NEXA*/ wire NEXA_BG_PIX_RST1_new = nand2(LOZE_PIPE_A_LOADp_new, NOLY_BG_PIX_DA1n_new);
    /*_p32.LUTU*/ wire LUTU_BG_PIX_RST2_new = nand2(LOZE_PIPE_A_LOADp_new, LEKE_BG_PIX_DA2n_new);
    /*_p32.LUJA*/ wire LUJA_BG_PIX_RST3_new = nand2(LOZE_PIPE_A_LOADp_new, LOMY_BG_PIX_DA3n_new);
    /*_p32.MOSY*/ wire MOSY_BG_PIX_RST4_new = nand2(LOZE_PIPE_A_LOADp_new, LALA_BG_PIX_DA4n_new);
    /*_p32.LERU*/ wire LERU_BG_PIX_RST5_new = nand2(LOZE_PIPE_A_LOADp_new, LOXA_BG_PIX_DA5n_new);
    /*_p32.NYHA*/ wire NYHA_BG_PIX_RST6_new = nand2(LOZE_PIPE_A_LOADp_new, NEZE_BG_PIX_DA6n_new);
    /*_p32.NADY*/ wire NADY_BG_PIX_RST7_new = nand2(LOZE_PIPE_A_LOADp_new, NOBO_BG_PIX_DA7n_new);

    /*_p32.MYDE*/ reg_new.bgw_pipe_a.MYDE_BGW_PIPE_A0.dff22(SACU_CLKPIPE_odd_new, LAKY_BG_PIX_SET0_new, LOTY_BG_PIX_RST0_new, reg_old.SIG_GND.out_old());
    /*_p32.NOZO*/ reg_new.bgw_pipe_a.NOZO_BGW_PIPE_A1.dff22(SACU_CLKPIPE_odd_new, NYXO_BG_PIX_SET1_new, NEXA_BG_PIX_RST1_new, reg_old.bgw_pipe_a.MYDE_BGW_PIPE_A0.qp_old());
    /*_p32.MOJU*/ reg_new.bgw_pipe_a.MOJU_BGW_PIPE_A2.dff22(SACU_CLKPIPE_odd_new, LOTO_BG_PIX_SET2_new, LUTU_BG_PIX_RST2_new, reg_old.bgw_pipe_a.NOZO_BGW_PIPE_A1.qp_old());
    /*_p32.MACU*/ reg_new.bgw_pipe_a.MACU_BGW_PIPE_A3.dff22(SACU_CLKPIPE_odd_new, LYDU_BG_PIX_SET3_new, LUJA_BG_PIX_RST3_new, reg_old.bgw_pipe_a.MOJU_BGW_PIPE_A2.qp_old());
    /*_p32.NEPO*/ reg_new.bgw_pipe_a.NEPO_BGW_PIPE_A4.dff22(SACU_CLKPIPE_odd_new, MYVY_BG_PIX_SET4_new, MOSY_BG_PIX_RST4_new, reg_old.bgw_pipe_a.MACU_BGW_PIPE_A3.qp_old());
    /*_p32.MODU*/ reg_new.bgw_pipe_a.MODU_BGW_PIPE_A5.dff22(SACU_CLKPIPE_odd_new, LODO_BG_PIX_SET5_new, LERU_BG_PIX_RST5_new, reg_old.bgw_pipe_a.NEPO_BGW_PIPE_A4.qp_old());
    /*_p32.NEDA*/ reg_new.bgw_pipe_a.NEDA_BGW_PIPE_A6.dff22(SACU_CLKPIPE_odd_new, NUTE_BG_PIX_SET6_new, NYHA_BG_PIX_RST6_new, reg_old.bgw_pipe_a.MODU_BGW_PIPE_A5.qp_old());
    /*_p32.PYBO*/ reg_new.bgw_pipe_a.PYBO_BGW_PIPE_A7.dff22(SACU_CLKPIPE_odd_new, NAJA_BG_PIX_SET7_new, NADY_BG_PIX_RST7_new, reg_old.bgw_pipe_a.NEDA_BGW_PIPE_A6.qp_old());
  }

  //----------------------------------------
  // Background pipe B
  {
    /*_p32.LUXA*/ wire LUXA_PIPE_B_LOADp = not1(NYXU_BFETCH_RSTn_new);
    /*_p32.TUXE*/ wire TUXE_BG_PIX_SET0_new = nand2(LUXA_PIPE_B_LOADp, reg_new.tile_temp_b.RAWU_TILE_DB0p.qp_new());
    /*_p32.SOLY*/ wire SOLY_BG_PIX_SET1_new = nand2(LUXA_PIPE_B_LOADp, reg_new.tile_temp_b.POZO_TILE_DB1p.qp_new());
    /*_p32.RUCE*/ wire RUCE_BG_PIX_SET2_new = nand2(LUXA_PIPE_B_LOADp, reg_new.tile_temp_b.PYZO_TILE_DB2p.qp_new());
    /*_p32.RYJA*/ wire RYJA_BG_PIX_SET3_new = nand2(LUXA_PIPE_B_LOADp, reg_new.tile_temp_b.POXA_TILE_DB3p.qp_new());
    /*_p32.RUTO*/ wire RUTO_BG_PIX_SET4_new = nand2(LUXA_PIPE_B_LOADp, reg_new.tile_temp_b.PULO_TILE_DB4p.qp_new());
    /*_p32.RAJA*/ wire RAJA_BG_PIX_SET5_new = nand2(LUXA_PIPE_B_LOADp, reg_new.tile_temp_b.POJU_TILE_DB5p.qp_new());
    /*_p32.RAJO*/ wire RAJO_BG_PIX_SET6_new = nand2(LUXA_PIPE_B_LOADp, reg_new.tile_temp_b.POWY_TILE_DB6p.qp_new());
    /*_p32.RAGA*/ wire RAGA_BG_PIX_SET7_new = nand2(LUXA_PIPE_B_LOADp, reg_new.tile_temp_b.PYJU_TILE_DB7p.qp_new());

    /*_p32.TOSA*/ wire TOSA_BG_PIX_DB0n_new = not1(reg_new.tile_temp_b.RAWU_TILE_DB0p.qp_new());
    /*_p32.RUCO*/ wire RUCO_BG_PIX_DB1n_new = not1(reg_new.tile_temp_b.POZO_TILE_DB1p.qp_new());
    /*_p32.TYCE*/ wire TYCE_BG_PIX_DB2n_new = not1(reg_new.tile_temp_b.PYZO_TILE_DB2p.qp_new());
    /*_p32.REVY*/ wire REVY_BG_PIX_DB3n_new = not1(reg_new.tile_temp_b.POXA_TILE_DB3p.qp_new());
    /*_p32.RYGA*/ wire RYGA_BG_PIX_DB4n_new = not1(reg_new.tile_temp_b.PULO_TILE_DB4p.qp_new());
    /*_p32.RYLE*/ wire RYLE_BG_PIX_DB5n_new = not1(reg_new.tile_temp_b.POJU_TILE_DB5p.qp_new());
    /*_p32.RAPU*/ wire RAPU_BG_PIX_DB6n_new = not1(reg_new.tile_temp_b.POWY_TILE_DB6p.qp_new());
    /*_p32.SOJA*/ wire SOJA_BG_PIX_DB7n_new = not1(reg_new.tile_temp_b.PYJU_TILE_DB7p.qp_new());

    /*_p32.SEJA*/ wire SEJA_BG_PIX_RST0_new = nand2(LUXA_PIPE_B_LOADp, TOSA_BG_PIX_DB0n_new);
    /*_p32.SENO*/ wire SENO_BG_PIX_RST1_new = nand2(LUXA_PIPE_B_LOADp, RUCO_BG_PIX_DB1n_new);
    /*_p32.SURE*/ wire SURE_BG_PIX_RST2_new = nand2(LUXA_PIPE_B_LOADp, TYCE_BG_PIX_DB2n_new);
    /*_p32.SEBO*/ wire SEBO_BG_PIX_RST3_new = nand2(LUXA_PIPE_B_LOADp, REVY_BG_PIX_DB3n_new);
    /*_p32.SUCA*/ wire SUCA_BG_PIX_RST4_new = nand2(LUXA_PIPE_B_LOADp, RYGA_BG_PIX_DB4n_new);
    /*_p32.SYWE*/ wire SYWE_BG_PIX_RST5_new = nand2(LUXA_PIPE_B_LOADp, RYLE_BG_PIX_DB5n_new);
    /*_p32.SUPU*/ wire SUPU_BG_PIX_RST6_new = nand2(LUXA_PIPE_B_LOADp, RAPU_BG_PIX_DB6n_new);
    /*_p32.RYJY*/ wire RYJY_BG_PIX_RST7_new = nand2(LUXA_PIPE_B_LOADp, SOJA_BG_PIX_DB7n_new);

    /*_p32.TOMY*/ reg_new.bgw_pipe_b.TOMY_BGW_PIPE_B0.dff22(SACU_CLKPIPE_odd_new, TUXE_BG_PIX_SET0_new, SEJA_BG_PIX_RST0_new, reg_old.SIG_GND.out_old());
    /*_p32.TACA*/ reg_new.bgw_pipe_b.TACA_BGW_PIPE_B1.dff22(SACU_CLKPIPE_odd_new, SOLY_BG_PIX_SET1_new, SENO_BG_PIX_RST1_new, reg_old.bgw_pipe_b.TOMY_BGW_PIPE_B0.qp_old());
    /*_p32.SADY*/ reg_new.bgw_pipe_b.SADY_BGW_PIPE_B2.dff22(SACU_CLKPIPE_odd_new, RUCE_BG_PIX_SET2_new, SURE_BG_PIX_RST2_new, reg_old.bgw_pipe_b.TACA_BGW_PIPE_B1.qp_old());
    /*_p32.RYSA*/ reg_new.bgw_pipe_b.RYSA_BGW_PIPE_B3.dff22(SACU_CLKPIPE_odd_new, RYJA_BG_PIX_SET3_new, SEBO_BG_PIX_RST3_new, reg_old.bgw_pipe_b.SADY_BGW_PIPE_B2.qp_old());
    /*_p32.SOBO*/ reg_new.bgw_pipe_b.SOBO_BGW_PIPE_B4.dff22(SACU_CLKPIPE_odd_new, RUTO_BG_PIX_SET4_new, SUCA_BG_PIX_RST4_new, reg_old.bgw_pipe_b.RYSA_BGW_PIPE_B3.qp_old());
    /*_p32.SETU*/ reg_new.bgw_pipe_b.SETU_BGW_PIPE_B5.dff22(SACU_CLKPIPE_odd_new, RAJA_BG_PIX_SET5_new, SYWE_BG_PIX_RST5_new, reg_old.bgw_pipe_b.SOBO_BGW_PIPE_B4.qp_old());
    /*_p32.RALU*/ reg_new.bgw_pipe_b.RALU_BGW_PIPE_B6.dff22(SACU_CLKPIPE_odd_new, RAJO_BG_PIX_SET6_new, SUPU_BG_PIX_RST6_new, reg_old.bgw_pipe_b.SETU_BGW_PIPE_B5.qp_old());
    /*_p32.SOHU*/ reg_new.bgw_pipe_b.SOHU_BGW_PIPE_B7.dff22(SACU_CLKPIPE_odd_new, RAGA_BG_PIX_SET7_new, RYJY_BG_PIX_RST7_new, reg_old.bgw_pipe_b.RALU_BGW_PIPE_B6.qp_old());
  }

  //----------------------------------------
  // Mask pipe
  {
    /*_p26.TEDE*/ wire TEDE_MASK_PIPE_SET0_new = nand2(LESY_SPRITE_MASK0p_new, reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.XALA*/ wire XALA_MASK_PIPE_SET1_new = nand2(LOTA_SPRITE_MASK1p_new, reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.TYRA*/ wire TYRA_MASK_PIPE_SET2_new = nand2(LYKU_SPRITE_MASK2p_new, reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.XYRU*/ wire XYRU_MASK_PIPE_SET3_new = nand2(ROBY_SPRITE_MASK3p_new, reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.XUKU*/ wire XUKU_MASK_PIPE_SET4_new = nand2(TYTA_SPRITE_MASK4p_new, reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.XELY*/ wire XELY_MASK_PIPE_SET5_new = nand2(TYCO_SPRITE_MASK5p_new, reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.TYKO*/ wire TYKO_MASK_PIPE_SET6_new = nand2(SOKA_SPRITE_MASK6p_new, reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.TUWU*/ wire TUWU_MASK_PIPE_SET7_new = nand2(XOVU_SPRITE_MASK7p_new, reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());

    /*_p26.XOGA*/ wire XOGA_MASK_PIPE_DB7n_new = not1(reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.XURA*/ wire XURA_MASK_PIPE_DB7n_new = not1(reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.TAJO*/ wire TAJO_MASK_PIPE_DB7n_new = not1(reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.XENU*/ wire XENU_MASK_PIPE_DB7n_new = not1(reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.XYKE*/ wire XYKE_MASK_PIPE_DB7n_new = not1(reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.XABA*/ wire XABA_MASK_PIPE_DB7n_new = not1(reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.TAFU*/ wire TAFU_MASK_PIPE_DB7n_new = not1(reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());
    /*_p26.XUHO*/ wire XUHO_MASK_PIPE_DB7n_new = not1(reg_new.oam_temp_b.DEPO_OAM_DB7n.qn_new());

    /*_p26.WOKA*/ wire WOKA_MASK_PIPE_RST0_new = nand2(LESY_SPRITE_MASK0p_new, XOGA_MASK_PIPE_DB7n_new);
    /*_p26.WEDE*/ wire WEDE_MASK_PIPE_RST1_new = nand2(LOTA_SPRITE_MASK1p_new, XURA_MASK_PIPE_DB7n_new);
    /*_p26.TUFO*/ wire TUFO_MASK_PIPE_RST2_new = nand2(LYKU_SPRITE_MASK2p_new, TAJO_MASK_PIPE_DB7n_new);
    /*_p26.WEVO*/ wire WEVO_MASK_PIPE_RST3_new = nand2(ROBY_SPRITE_MASK3p_new, XENU_MASK_PIPE_DB7n_new);
    /*_p26.WEDY*/ wire WEDY_MASK_PIPE_RST4_new = nand2(TYTA_SPRITE_MASK4p_new, XYKE_MASK_PIPE_DB7n_new);
    /*_p26.WUJA*/ wire WUJA_MASK_PIPE_RST5_new = nand2(TYCO_SPRITE_MASK5p_new, XABA_MASK_PIPE_DB7n_new);
    /*_p26.TENA*/ wire TENA_MASK_PIPE_RST6_new = nand2(SOKA_SPRITE_MASK6p_new, TAFU_MASK_PIPE_DB7n_new);
    /*_p26.WUBU*/ wire WUBU_MASK_PIPE_RST7_new = nand2(XOVU_SPRITE_MASK7p_new, XUHO_MASK_PIPE_DB7n_new);

    /*_p26.VEZO*/ reg_new.mask_pipe.VEZO_MASK_PIPE_0.dff22(SACU_CLKPIPE_odd_new, TEDE_MASK_PIPE_SET0_new, WOKA_MASK_PIPE_RST0_new, reg_old.SIG_VCC.out_old());
    /*_p26.WURU*/ reg_new.mask_pipe.WURU_MASK_PIPE_1.dff22(SACU_CLKPIPE_odd_new, XALA_MASK_PIPE_SET1_new, WEDE_MASK_PIPE_RST1_new, reg_old.mask_pipe.VEZO_MASK_PIPE_0.qp_old());
    /*_p26.VOSA*/ reg_new.mask_pipe.VOSA_MASK_PIPE_2.dff22(SACU_CLKPIPE_odd_new, TYRA_MASK_PIPE_SET2_new, TUFO_MASK_PIPE_RST2_new, reg_old.mask_pipe.WURU_MASK_PIPE_1.qp_old());
    /*_p26.WYFU*/ reg_new.mask_pipe.WYFU_MASK_PIPE_3.dff22(SACU_CLKPIPE_odd_new, XYRU_MASK_PIPE_SET3_new, WEVO_MASK_PIPE_RST3_new, reg_old.mask_pipe.VOSA_MASK_PIPE_2.qp_old());
    /*_p26.XETE*/ reg_new.mask_pipe.XETE_MASK_PIPE_4.dff22(SACU_CLKPIPE_odd_new, XUKU_MASK_PIPE_SET4_new, WEDY_MASK_PIPE_RST4_new, reg_old.mask_pipe.WYFU_MASK_PIPE_3.qp_old());
    /*_p26.WODA*/ reg_new.mask_pipe.WODA_MASK_PIPE_5.dff22(SACU_CLKPIPE_odd_new, XELY_MASK_PIPE_SET5_new, WUJA_MASK_PIPE_RST5_new, reg_old.mask_pipe.XETE_MASK_PIPE_4.qp_old());
    /*_p26.VUMO*/ reg_new.mask_pipe.VUMO_MASK_PIPE_6.dff22(SACU_CLKPIPE_odd_new, TYKO_MASK_PIPE_SET6_new, TENA_MASK_PIPE_RST6_new, reg_old.mask_pipe.WODA_MASK_PIPE_5.qp_old());
    /*_p26.VAVA*/ reg_new.mask_pipe.VAVA_MASK_PIPE_7.dff22(SACU_CLKPIPE_odd_new, TUWU_MASK_PIPE_SET7_new, WUBU_MASK_PIPE_RST7_new, reg_old.mask_pipe.VUMO_MASK_PIPE_6.qp_old());
  }

  //----------------------------------------
  // Pal pipe
  {
    /*_p34.PUME*/ wire PUME_PAL_PIPE_SET0_new = nand2(LESY_SPRITE_MASK0p_new, reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.SORO*/ wire SORO_PAL_PIPE_SET1_new = nand2(LOTA_SPRITE_MASK1p_new, reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.PAMO*/ wire PAMO_PAL_PIPE_SET2_new = nand2(LYKU_SPRITE_MASK2p_new, reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.SUKY*/ wire SUKY_PAL_PIPE_SET3_new = nand2(ROBY_SPRITE_MASK3p_new, reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.RORA*/ wire RORA_PAL_PIPE_SET4_new = nand2(TYTA_SPRITE_MASK4p_new, reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.MENE*/ wire MENE_PAL_PIPE_SET5_new = nand2(TYCO_SPRITE_MASK5p_new, reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.LUKE*/ wire LUKE_PAL_PIPE_SET6_new = nand2(SOKA_SPRITE_MASK6p_new, reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.LAMY*/ wire LAMY_PAL_PIPE_SET7_new = nand2(XOVU_SPRITE_MASK7p_new, reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());

    /*_p34.SYPY*/ wire SYPY_PAL_PIPE_DB4n_new = not1(reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.TOTU*/ wire TOTU_PAL_PIPE_DB4n_new = not1(reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.NARO*/ wire NARO_PAL_PIPE_DB4n_new = not1(reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.WEXY*/ wire WEXY_PAL_PIPE_DB4n_new = not1(reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.RYZY*/ wire RYZY_PAL_PIPE_DB4n_new = not1(reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.RYFE*/ wire RYFE_PAL_PIPE_DB4n_new = not1(reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.LADY*/ wire LADY_PAL_PIPE_DB4n_new = not1(reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());
    /*_p34.LAFY*/ wire LAFY_PAL_PIPE_DB4n_new = not1(reg_new.oam_temp_b.GOMO_OAM_DB4n.qn_new());

    /*_p34.SUCO*/ wire SUCO_PAL_PIPE_RST0_new = nand2(LESY_SPRITE_MASK0p_new, SYPY_PAL_PIPE_DB4n_new);
    /*_p34.TAFA*/ wire TAFA_PAL_PIPE_RST1_new = nand2(LOTA_SPRITE_MASK1p_new, TOTU_PAL_PIPE_DB4n_new);
    /*_p34.PYZY*/ wire PYZY_PAL_PIPE_RST2_new = nand2(LYKU_SPRITE_MASK2p_new, NARO_PAL_PIPE_DB4n_new);
    /*_p34.TOWA*/ wire TOWA_PAL_PIPE_RST3_new = nand2(ROBY_SPRITE_MASK3p_new, WEXY_PAL_PIPE_DB4n_new);
    /*_p34.RUDU*/ wire RUDU_PAL_PIPE_RST4_new = nand2(TYTA_SPRITE_MASK4p_new, RYZY_PAL_PIPE_DB4n_new);
    /*_p34.PAZO*/ wire PAZO_PAL_PIPE_RST5_new = nand2(TYCO_SPRITE_MASK5p_new, RYFE_PAL_PIPE_DB4n_new);
    /*_p34.LOWA*/ wire LOWA_PAL_PIPE_RST6_new = nand2(SOKA_SPRITE_MASK6p_new, LADY_PAL_PIPE_DB4n_new);
    /*_p34.LUNU*/ wire LUNU_PAL_PIPE_RST7_new = nand2(XOVU_SPRITE_MASK7p_new, LAFY_PAL_PIPE_DB4n_new);

    /*_p34.RUGO*/ reg_new.pal_pipe.RUGO_PAL_PIPE_D0.dff22(SACU_CLKPIPE_odd_new, PUME_PAL_PIPE_SET0_new, SUCO_PAL_PIPE_RST0_new, reg_old.SIG_GND.out_old());
    /*_p34.SATA*/ reg_new.pal_pipe.SATA_PAL_PIPE_D1.dff22(SACU_CLKPIPE_odd_new, SORO_PAL_PIPE_SET1_new, TAFA_PAL_PIPE_RST1_new, reg_old.pal_pipe.RUGO_PAL_PIPE_D0.qp_old());
    /*_p34.ROSA*/ reg_new.pal_pipe.ROSA_PAL_PIPE_D2.dff22(SACU_CLKPIPE_odd_new, PAMO_PAL_PIPE_SET2_new, PYZY_PAL_PIPE_RST2_new, reg_old.pal_pipe.SATA_PAL_PIPE_D1.qp_old());
    /*_p34.SOMY*/ reg_new.pal_pipe.SOMY_PAL_PIPE_D3.dff22(SACU_CLKPIPE_odd_new, SUKY_PAL_PIPE_SET3_new, TOWA_PAL_PIPE_RST3_new, reg_old.pal_pipe.ROSA_PAL_PIPE_D2.qp_old());
    /*_p34.PALU*/ reg_new.pal_pipe.PALU_PAL_PIPE_D4.dff22(SACU_CLKPIPE_odd_new, RORA_PAL_PIPE_SET4_new, RUDU_PAL_PIPE_RST4_new, reg_old.pal_pipe.SOMY_PAL_PIPE_D3.qp_old());
    /*_p34.NUKE*/ reg_new.pal_pipe.NUKE_PAL_PIPE_D5.dff22(SACU_CLKPIPE_odd_new, MENE_PAL_PIPE_SET5_new, PAZO_PAL_PIPE_RST5_new, reg_old.pal_pipe.PALU_PAL_PIPE_D4.qp_old());
    /*_p34.MODA*/ reg_new.pal_pipe.MODA_PAL_PIPE_D6.dff22(SACU_CLKPIPE_odd_new, LUKE_PAL_PIPE_SET6_new, LOWA_PAL_PIPE_RST6_new, reg_old.pal_pipe.NUKE_PAL_PIPE_D5.qp_old());
    /*_p34.LYME*/ reg_new.pal_pipe.LYME_PAL_PIPE_D7.dff22(SACU_CLKPIPE_odd_new, LAMY_PAL_PIPE_SET7_new, LUNU_PAL_PIPE_RST7_new, reg_old.pal_pipe.MODA_PAL_PIPE_D6.qp_old());
  }

  //----------------------------------------
  // Pipe merge

  /*#p35.RAJY*/ wire RAJY_PIX_BG_LOp_new = and2(reg_new.bgw_pipe_a.PYBO_BGW_PIPE_A7.qp_new(), reg_new.reg_lcdc.VYXE_LCDC_BGENp.qp_new());
  /*#p35.TADE*/ wire TADE_PIX_BG_HIp_new = and2(reg_new.bgw_pipe_b.SOHU_BGW_PIPE_B7.qp_new(), reg_new.reg_lcdc.VYXE_LCDC_BGENp.qp_new());
  /*#p35.XULA*/ wire XULA_PIX_SP_LOp_new = and2(reg_new.reg_lcdc.XYLO_LCDC_SPENp.qp_new(), reg_new.spr_pipe_a.WUFY_SPR_PIPE_A7.qp_new());
  /*#p35.WOXA*/ wire WOXA_PIX_SP_HIp_new = and2(reg_new.reg_lcdc.XYLO_LCDC_SPENp.qp_new(), reg_new.spr_pipe_b.VUPY_SPR_PIPE_B7.qp_new());

  /*#p35.NULY*/ wire NULY_PIX_SP_MASKn_new = nor2(WOXA_PIX_SP_HIp_new, XULA_PIX_SP_LOp_new);

  /*#p35.RYFU*/ wire RYFU_MASK_BG0_new = and2(RAJY_PIX_BG_LOp_new, reg_new.mask_pipe.VAVA_MASK_PIPE_7.qp_new());
  /*#p35.RUTA*/ wire RUTA_MASK_BG1_new = and2(TADE_PIX_BG_HIp_new, reg_new.mask_pipe.VAVA_MASK_PIPE_7.qp_new());
  /*#p35.POKA*/ wire POKA_BGPIXELn_new = nor3(NULY_PIX_SP_MASKn_new, RUTA_MASK_BG1_new, RYFU_MASK_BG0_new);

  /*#p34.LOME*/ wire LOME_PAL_PIPE_7n_new = not1(reg_new.pal_pipe.LYME_PAL_PIPE_D7.qp_new());
  /*#p34.LAFU*/ wire LAFU_OBP0PIXELn_new = nand2(LOME_PAL_PIPE_7n_new, POKA_BGPIXELn_new);
  /*#p34.LEKA*/ wire LEKA_OBP1PIXELn_new = nand2(reg_new.pal_pipe.LYME_PAL_PIPE_D7.qp_new(), POKA_BGPIXELn_new);

  //----------------------------------------
  // Pal reg read/write

  /*_p36.VELY*/ wire VELY_FF47_WRp_new = and2(CUPA_CPU_WRp_new, reg_new.cpu_abus.WERA_FF47p_new());
  /*_p36.TEPO*/ wire TEPO_FF47_WRp_new = not1(VELY_FF47_WRp_new);

  /*#p36.PAVO*/ reg_new.reg_bgp.PAVO_BGP_D0p.dff8(TEPO_FF47_WRp_new, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p36.NUSY*/ reg_new.reg_bgp.NUSY_BGP_D1p.dff8(TEPO_FF47_WRp_new, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p36.PYLU*/ reg_new.reg_bgp.PYLU_BGP_D2p.dff8(TEPO_FF47_WRp_new, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p36.MAXY*/ reg_new.reg_bgp.MAXY_BGP_D3p.dff8(TEPO_FF47_WRp_new, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p36.MUKE*/ reg_new.reg_bgp.MUKE_BGP_D4p.dff8(TEPO_FF47_WRp_new, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p36.MORU*/ reg_new.reg_bgp.MORU_BGP_D5p.dff8(TEPO_FF47_WRp_new, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p36.MOGY*/ reg_new.reg_bgp.MOGY_BGP_D6p.dff8(TEPO_FF47_WRp_new, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p36.MENA*/ reg_new.reg_bgp.MENA_BGP_D7p.dff8(TEPO_FF47_WRp_new, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p36.XOMA*/ wire XOMA_FF48_WRp_new = and2(CUPA_CPU_WRp_new, reg_new.cpu_abus.XAYO_FF48p_new());
  /*_p36.XELO*/ wire XELO_FF48_WRn_new = not1(XOMA_FF48_WRp_new);
  /*_p36.XUFU*/ reg_new.reg_obp0.XUFU_OBP0_D0p.dff8(XELO_FF48_WRn_new, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p36.XUKY*/ reg_new.reg_obp0.XUKY_OBP0_D1p.dff8(XELO_FF48_WRn_new, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p36.XOVA*/ reg_new.reg_obp0.XOVA_OBP0_D2p.dff8(XELO_FF48_WRn_new, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p36.XALO*/ reg_new.reg_obp0.XALO_OBP0_D3p.dff8(XELO_FF48_WRn_new, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p36.XERU*/ reg_new.reg_obp0.XERU_OBP0_D4p.dff8(XELO_FF48_WRn_new, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p36.XYZE*/ reg_new.reg_obp0.XYZE_OBP0_D5p.dff8(XELO_FF48_WRn_new, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p36.XUPO*/ reg_new.reg_obp0.XUPO_OBP0_D6p.dff8(XELO_FF48_WRn_new, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p36.XANA*/ reg_new.reg_obp0.XANA_OBP0_D7p.dff8(XELO_FF48_WRn_new, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p36.MYXE*/ wire MYXE_FF49_WRp_new = and2(CUPA_CPU_WRp_new, reg_new.cpu_abus.TEGO_FF49p_new());
  /*_p36.LEHO*/ wire LEHO_FF49_WRn_new = not1(MYXE_FF49_WRp_new);
  /*_p36.MOXY*/ reg_new.reg_obp1.MOXY_OBP1_D0p.dff8(LEHO_FF49_WRn_new, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p36.LAWO*/ reg_new.reg_obp1.LAWO_OBP1_D1p.dff8(LEHO_FF49_WRn_new, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p36.MOSA*/ reg_new.reg_obp1.MOSA_OBP1_D2p.dff8(LEHO_FF49_WRn_new, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p36.LOSE*/ reg_new.reg_obp1.LOSE_OBP1_D3p.dff8(LEHO_FF49_WRn_new, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p36.LUNE*/ reg_new.reg_obp1.LUNE_OBP1_D4p.dff8(LEHO_FF49_WRn_new, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p36.LUGU*/ reg_new.reg_obp1.LUGU_OBP1_D5p.dff8(LEHO_FF49_WRn_new, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p36.LEPU*/ reg_new.reg_obp1.LEPU_OBP1_D6p.dff8(LEHO_FF49_WRn_new, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p36.LUXO*/ reg_new.reg_obp1.LUXO_OBP1_D7p.dff8(LEHO_FF49_WRn_new, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p07.AJAS*/ wire AJAS_CPU_RDn_new = not1(reg_new.cpu_signals.TEDO_CPU_RDp.out_new());
  /*_p07.ASOT*/ wire ASOT_CPU_RDp_new = not1(AJAS_CPU_RDn_new);
  /*_p36.VUSO*/ wire VUSO_FF47_RDp_new = and2(ASOT_CPU_RDp_new, reg_new.cpu_abus.WERA_FF47p_new());
  /*_p36.TEPY*/ wire TEPY_FF47_RDn_new = not1(VUSO_FF47_RDp_new);
  /*#p36.RARO*/ triwire RARO_BGP0_TO_CD0_new = tri6_nn(TEPY_FF47_RDn_new, reg_new.reg_bgp.PAVO_BGP_D0p.qn_new());
  /*_p36.PABA*/ triwire PABA_BGP1_TO_CD1_new = tri6_nn(TEPY_FF47_RDn_new, reg_new.reg_bgp.NUSY_BGP_D1p.qn_new());
  /*_p36.REDO*/ triwire REDO_BGP2_TO_CD2_new = tri6_nn(TEPY_FF47_RDn_new, reg_new.reg_bgp.PYLU_BGP_D2p.qn_new());
  /*_p36.LOBE*/ triwire LOBE_BGP3_TO_CD3_new = tri6_nn(TEPY_FF47_RDn_new, reg_new.reg_bgp.MAXY_BGP_D3p.qn_new());
  /*_p36.LACE*/ triwire LACE_BGP4_TO_CD4_new = tri6_nn(TEPY_FF47_RDn_new, reg_new.reg_bgp.MUKE_BGP_D4p.qn_new());
  /*_p36.LYKA*/ triwire LYKA_BGP5_TO_CD5_new = tri6_nn(TEPY_FF47_RDn_new, reg_new.reg_bgp.MORU_BGP_D5p.qn_new());
  /*_p36.LODY*/ triwire LODY_BGP6_TO_CD6_new = tri6_nn(TEPY_FF47_RDn_new, reg_new.reg_bgp.MOGY_BGP_D6p.qn_new());
  /*_p36.LARY*/ triwire LARY_BGP7_TO_CD7_new = tri6_nn(TEPY_FF47_RDn_new, reg_new.reg_bgp.MENA_BGP_D7p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(RARO_BGP0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(PABA_BGP1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(REDO_BGP2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(LOBE_BGP3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(LACE_BGP4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(LYKA_BGP5_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(LODY_BGP6_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(LARY_BGP7_TO_CD7_new);

  /*_p36.XUFY*/ wire XUFY_FF48_RDp_new = and2(ASOT_CPU_RDp_new, reg_new.cpu_abus.XAYO_FF48p_new());
  /*_p36.XOZY*/ wire XOZY_FF48_RDn_new = not1(XUFY_FF48_RDp_new);
  /*#p36.XARY*/ triwire XARY_OBP00_TO_CD0_new = tri6_nn(XOZY_FF48_RDn_new, reg_new.reg_obp0.XUFU_OBP0_D0p.qn_new());
  /*_p36.XOKE*/ triwire XOKE_OBP01_TO_CD1_new = tri6_nn(XOZY_FF48_RDn_new, reg_new.reg_obp0.XUKY_OBP0_D1p.qn_new());
  /*_p36.XUNO*/ triwire XUNO_OBP02_TO_CD2_new = tri6_nn(XOZY_FF48_RDn_new, reg_new.reg_obp0.XOVA_OBP0_D2p.qn_new());
  /*_p36.XUBY*/ triwire XUBY_OBP03_TO_CD3_new = tri6_nn(XOZY_FF48_RDn_new, reg_new.reg_obp0.XALO_OBP0_D3p.qn_new());
  /*_p36.XAJU*/ triwire XAJU_OBP04_TO_CD4_new = tri6_nn(XOZY_FF48_RDn_new, reg_new.reg_obp0.XERU_OBP0_D4p.qn_new());
  /*_p36.XOBO*/ triwire XOBO_OBP05_TO_CD5_new = tri6_nn(XOZY_FF48_RDn_new, reg_new.reg_obp0.XYZE_OBP0_D5p.qn_new());
  /*_p36.XAXA*/ triwire XAXA_OBP06_TO_CD6_new = tri6_nn(XOZY_FF48_RDn_new, reg_new.reg_obp0.XUPO_OBP0_D6p.qn_new());
  /*_p36.XAWO*/ triwire XAWO_OBP07_TO_CD7_new = tri6_nn(XOZY_FF48_RDn_new, reg_new.reg_obp0.XANA_OBP0_D7p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(XARY_OBP00_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(XOKE_OBP01_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(XUNO_OBP02_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(XUBY_OBP03_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(XAJU_OBP04_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(XOBO_OBP05_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(XAXA_OBP06_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(XAWO_OBP07_TO_CD7_new);

  /*_p36.MUMY*/ wire MUMY_FF49_RDp_new = and2(ASOT_CPU_RDp_new, reg_new.cpu_abus.TEGO_FF49p_new());
  /*_p36.LOTE*/ wire LOTE_FF49_RDn_new = not1(MUMY_FF49_RDp_new);
  /*#p36.LAJU*/ triwire LAJU_OBP10_TO_CD0_new = tri6_nn(LOTE_FF49_RDn_new, reg_new.reg_obp1.MOXY_OBP1_D0p.qn_new());
  /*_p36.LEPA*/ triwire LEPA_OBP11_TO_CD1_new = tri6_nn(LOTE_FF49_RDn_new, reg_new.reg_obp1.LAWO_OBP1_D1p.qn_new());
  /*_p36.LODE*/ triwire LODE_OBP12_TO_CD2_new = tri6_nn(LOTE_FF49_RDn_new, reg_new.reg_obp1.MOSA_OBP1_D2p.qn_new());
  /*_p36.LYZA*/ triwire LYZA_OBP13_TO_CD3_new = tri6_nn(LOTE_FF49_RDn_new, reg_new.reg_obp1.LOSE_OBP1_D3p.qn_new());
  /*_p36.LUKY*/ triwire LUKY_OBP14_TO_CD4_new = tri6_nn(LOTE_FF49_RDn_new, reg_new.reg_obp1.LUNE_OBP1_D4p.qn_new());
  /*_p36.LUGA*/ triwire LUGA_OBP15_TO_CD5_new = tri6_nn(LOTE_FF49_RDn_new, reg_new.reg_obp1.LUGU_OBP1_D5p.qn_new());
  /*_p36.LEBA*/ triwire LEBA_OBP16_TO_CD6_new = tri6_nn(LOTE_FF49_RDn_new, reg_new.reg_obp1.LEPU_OBP1_D6p.qn_new());
  /*_p36.LELU*/ triwire LELU_OBP17_TO_CD7_new = tri6_nn(LOTE_FF49_RDn_new, reg_new.reg_obp1.LUXO_OBP1_D7p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(LAJU_OBP10_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(LEPA_OBP11_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(LODE_OBP12_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(LYZA_OBP13_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(LUKY_OBP14_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(LUGA_OBP15_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(LEBA_OBP16_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(LELU_OBP17_TO_CD7_new);

  //----------------------------------------
  // Sprite palette 0 lookup

  /*#p35.WELE*/ wire WELE_PIX_SP_LOn_new = not1(XULA_PIX_SP_LOp_new);
  /*#p35.WOLO*/ wire WOLO_PIX_SP_LOp_new = not1(WELE_PIX_SP_LOn_new);
  /*#p35.VUMU*/ wire VUMU_PIX_SP_HIn_new = not1(WOXA_PIX_SP_HIp_new);
  /*#p35.WYRU*/ wire WYRU_PIX_SP_HIp_new = not1(VUMU_PIX_SP_HIn_new);

  /*#p35.LAVA*/ wire LAVA_MASK_OPB0_new = not1(LAFU_OBP0PIXELn_new);

  /*#p35.VUGO*/ wire VUGO_PAL_OBP0A_new = and3(VUMU_PIX_SP_HIn_new, WELE_PIX_SP_LOn_new, LAVA_MASK_OPB0_new); // does not have vcc arm
  /*#p35.VOLO*/ wire VOLO_PAL_OBP0B_new = and3(VUMU_PIX_SP_HIn_new, WOLO_PIX_SP_LOp_new, LAVA_MASK_OPB0_new); // does not have vcc arm
  /*#p35.VATA*/ wire VATA_PAL_OBP0C_new = and3(WYRU_PIX_SP_HIp_new, WELE_PIX_SP_LOn_new, LAVA_MASK_OPB0_new); // does not have vcc arm
  /*#p35.VYRO*/ wire VYRO_PAL_OBP0D_new = and3(WYRU_PIX_SP_HIp_new, WOLO_PIX_SP_LOp_new, LAVA_MASK_OPB0_new); // does not have vcc arm

  /*#p35.WUFU*/ wire WUFU_COL_OBP0_HI_new = amux4(
    reg_new.reg_obp0.XANA_OBP0_D7p.qn_any(), VYRO_PAL_OBP0D_new,
    reg_new.reg_obp0.XYZE_OBP0_D5p.qn_any(), VATA_PAL_OBP0C_new,
    reg_new.reg_obp0.XALO_OBP0_D3p.qn_any(), VOLO_PAL_OBP0B_new,
    reg_new.reg_obp0.XUKY_OBP0_D1p.qn_any(), VUGO_PAL_OBP0A_new);

  /*#p35.WALY*/ wire WALY_COL_OBP0_LO_new = amux4(
    reg_new.reg_obp0.XUPO_OBP0_D6p.qn_any(), VYRO_PAL_OBP0D_new,
    reg_new.reg_obp0.XERU_OBP0_D4p.qn_any(), VATA_PAL_OBP0C_new,
    reg_new.reg_obp0.XOVA_OBP0_D2p.qn_any(), VOLO_PAL_OBP0B_new,
    reg_new.reg_obp0.XUFU_OBP0_D0p.qn_any(), VUGO_PAL_OBP0A_new);

  //----------------------------------------
  // Sprite palette 1 lookup

  /*#p35.MABY*/ wire MABY_PIX_SP_LOn_new = not1(XULA_PIX_SP_LOp_new);
  /*#p35.LYLE*/ wire LYLE_PIX_SP_LOp_new = not1(MABY_PIX_SP_LOn_new);
  /*#p35.MEXA*/ wire MEXA_PIX_SP_HIn_new = not1(WOXA_PIX_SP_HIp_new);
  /*#p35.LOZO*/ wire LOZO_PIX_SP_HIp_new = not1(MEXA_PIX_SP_HIn_new);

  /*#p35.LUKU*/ wire LUKU_MASK_OBP1_new = not1(LEKA_OBP1PIXELn_new);

  /*#p35.LOPU*/ wire LOPU_PAL_OBP1A_new = and3(MEXA_PIX_SP_HIn_new, MABY_PIX_SP_LOn_new, LUKU_MASK_OBP1_new); // does not have vcc arm
  /*#p35.LYKY*/ wire LYKY_PAL_OBP1B_new = and3(MEXA_PIX_SP_HIn_new, LYLE_PIX_SP_LOp_new, LUKU_MASK_OBP1_new); // does not have vcc arm
  /*#p35.LARU*/ wire LARU_PAL_OBP1C_new = and3(LOZO_PIX_SP_HIp_new, MABY_PIX_SP_LOn_new, LUKU_MASK_OBP1_new); // does not have vcc arm
  /*#p35.LEDO*/ wire LEDO_PAL_OBP1D_new = and3(LOZO_PIX_SP_HIp_new, LYLE_PIX_SP_LOp_new, LUKU_MASK_OBP1_new); // does not have vcc arm

  /*#p35.MOKA*/ wire MOKA_COL_OBP1_HI_new = amux4(
    reg_new.reg_obp1.LUXO_OBP1_D7p.qn_any(), LEDO_PAL_OBP1D_new,
    reg_new.reg_obp1.LUGU_OBP1_D5p.qn_any(), LARU_PAL_OBP1C_new,
    reg_new.reg_obp1.LOSE_OBP1_D3p.qn_any(), LYKY_PAL_OBP1B_new,
    reg_new.reg_obp1.LAWO_OBP1_D1p.qn_any(), LOPU_PAL_OBP1A_new);

  /*#p35.MUFA*/ wire MUFA_COL_OBP1_LO_new = amux4(
    LEDO_PAL_OBP1D_new, reg_new.reg_obp1.LEPU_OBP1_D6p.qn_any(),
    LARU_PAL_OBP1C_new, reg_new.reg_obp1.LUNE_OBP1_D4p.qn_any(),
    LYKY_PAL_OBP1B_new, reg_new.reg_obp1.MOSA_OBP1_D2p.qn_any(),
    LOPU_PAL_OBP1A_new, reg_new.reg_obp1.MOXY_OBP1_D0p.qn_any());

  //----------------------------------------
  // Background/window palette lookup

  /*_p35.SOBA*/ wire SOBA_PIX_BG_LOn_new = not1(RAJY_PIX_BG_LOp_new);
  /*_p35.NUPO*/ wire NUPO_PIX_BG_LOp_new = not1(SOBA_PIX_BG_LOn_new);
  /*_p35.VYCO*/ wire VYCO_PIX_BG_HIn_new = not1(TADE_PIX_BG_HIp_new);
  /*_p35.NALE*/ wire NALE_PIX_BG_HIp_new = not1(VYCO_PIX_BG_HIn_new);

  /*_p35.MUVE*/ wire MUVE_MASK_BGP_new = not1(POKA_BGPIXELn_new);

  /*_p35.POBU*/ wire POBU_PAL_BGPA_new = and3(VYCO_PIX_BG_HIn_new, SOBA_PIX_BG_LOn_new, MUVE_MASK_BGP_new); // does not have vcc arm
  /*_p35.NUXO*/ wire NUXO_PAL_BGPB_new = and3(VYCO_PIX_BG_HIn_new, NUPO_PIX_BG_LOp_new, MUVE_MASK_BGP_new); // does not have vcc arm
  /*_p35.NUMA*/ wire NUMA_PAL_BGPC_new = and3(NALE_PIX_BG_HIp_new, SOBA_PIX_BG_LOn_new, MUVE_MASK_BGP_new); // does not have vcc arm
  /*_p35.NYPO*/ wire NYPO_PAL_BGPD_new = and3(NALE_PIX_BG_HIp_new, NUPO_PIX_BG_LOp_new, MUVE_MASK_BGP_new); // does not have vcc arm

  /*#p35.NELO*/ wire NELO_COL_BG_LO_new = amux4(
    NYPO_PAL_BGPD_new, reg_new.reg_bgp.MOGY_BGP_D6p.qn_any(),
    NUMA_PAL_BGPC_new, reg_new.reg_bgp.MUKE_BGP_D4p.qn_any(),
    NUXO_PAL_BGPB_new, reg_new.reg_bgp.PYLU_BGP_D2p.qn_any(),
    POBU_PAL_BGPA_new, reg_new.reg_bgp.PAVO_BGP_D0p.qn_any());

  /*#p35.NURA*/ wire NURA_COL_BG_HI_new = amux4(
    reg_new.reg_bgp.MENA_BGP_D7p.qn_any(), NYPO_PAL_BGPD_new,
    reg_new.reg_bgp.MORU_BGP_D5p.qn_any(), NUMA_PAL_BGPC_new,
    reg_new.reg_bgp.MAXY_BGP_D3p.qn_any(), NUXO_PAL_BGPB_new,
    reg_new.reg_bgp.NUSY_BGP_D1p.qn_any(), POBU_PAL_BGPA_new);

  //----------------------------------------
  // Pixel merge and send

  /*#p35.PERO*/ wire PERO_COL_LO_new = or3(NELO_COL_BG_LO_new, WALY_COL_OBP0_LO_new, MUFA_COL_OBP1_LO_new);
  /*#p35.PATY*/ wire PATY_COL_HI_new = or3(NURA_COL_BG_HI_new, WUFU_COL_OBP0_HI_new, MOKA_COL_OBP1_HI_new);

  /*#p35.REMY*/ reg_new.lcd.REMY_LD0n <<= not1(PERO_COL_LO_new);
  /*#p35.RAVO*/ reg_new.lcd.RAVO_LD1n <<= not1(PATY_COL_HI_new);
}

//-----------------------------------------------------------------------------

void WinControl::reset() {
  NUKO_WX_MATCHp_odd.state       = 0b00011000;
  ROGE_WY_MATCHp_odd.state       = 0b00011000;
  PYNU_WIN_MODE_Ap_odd.state     = 0b00011000;
  PUKU_WIN_HITn_odd.state        = 0b00011001;
  RYDY_WIN_HITp.state        = 0b00011000;
  SOVY_WIN_HITp_evn.state        = 0b00011010;
  NOPA_WIN_MODE_Bp_evn.state     = 0b00011010;
  PYCO_WIN_MATCHp_evn.state      = 0b00011000;
  NUNU_WIN_MATCHp_odd.state      = 0b00011000;
  REJO_WY_MATCH_LATCHp_odd.state = 0b00011000;
  SARY_WY_MATCHp_odd.state       = 0b00011000;
  RYFA_WIN_FETCHn_A_evn.state    = 0b00011010;
  RENE_WIN_FETCHn_B_evn.state    = 0b00011010;
}

//-----------------------------------------------------------------------------

void FineCount::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }

//-----------------------------------------------------------------------------

void FineScroll::reset() {
  PUXA_SCX_FINE_MATCH_evn.state    = 0b00011000;
  NYZE_SCX_FINE_MATCH_odd.state    = 0b00011000;
  ROXY_FINE_SCROLL_DONEn.state = 0b00011001;
}

//-----------------------------------------------------------------------------

void MaskPipe::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0xFF); }

//-----------------------------------------------------------------------------

void BgwPipeA::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }

//-----------------------------------------------------------------------------

void BgwPipeB::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }

//-----------------------------------------------------------------------------

void SprPipeA::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }

//-----------------------------------------------------------------------------

void SprPipeB::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }

//-----------------------------------------------------------------------------

void PalPipe::reset()    { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }

//-----------------------------------------------------------------------------
