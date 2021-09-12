#include "GateBoyLib/GateBoyPixPipe.h"

#include "GateBoyLib/GateBoy.h"

// Sprite temp loads  on AB/CD/EF/GH
// Tile   temp loads  on AB/CD/EF/GH
// Pipes       shifts on HA/BC/DE/FG
// WUTY        fires  on HA/BC/DE/FG
// NYXU        fires  on HA/BC/DE/FG

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_window_gates(wire SEGU_CLKPIPE_evn, wire REPU_VBLANKp) {
  /*_p27.ROCO*/ wire ROCO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);

  /*_p27.PYCO*/ reg.win_ctrl.PYCO_WIN_MATCHp.dff17(ROCO_CLKPIPE_odd, XAPO_VID_RSTn(), reg.win_ctrl.NUKO_WX_MATCHp.out_old());

  /*#p27.ROZE*/ wire ROZE_FINE_COUNT_7n_old = nand3(reg.fine_count.RUBU_FINE_CNT2.qp_old(), reg.fine_count.ROGA_FINE_CNT1.qp_old(), reg.fine_count.RYKU_FINE_CNT0.qp_old());
  /*_p27.PANY*/ wire PANY_WIN_FETCHn_old = nor2(reg.win_ctrl.NUKO_WX_MATCHp.out_old(), ROZE_FINE_COUNT_7n_old);

  /*_p27.RENE*/ reg.win_ctrl.RENE_WIN_FETCHn_B.dff17(ALET_xBxDxFxH(),  reg.XYMU_RENDERINGn.qn_new(), reg.win_ctrl.RYFA_WIN_FETCHn_A.qp_old());
  /*_p27.RYFA*/ reg.win_ctrl.RYFA_WIN_FETCHn_A.dff17(SEGU_CLKPIPE_evn, reg.XYMU_RENDERINGn.qn_new(), PANY_WIN_FETCHn_old);

  /*_p23.WEKO*/ wire WEKO_FF4A_WRp = and2(CUPA_CPU_WRp(), reg.cpu_abus_new.VYGA_FF4Ap());
  /*_p23.VEFU*/ wire VEFU_FF4A_WRn = not1(WEKO_FF4A_WRp);
  /*_p23.NESO*/ reg.reg_wy.NESO_WY0n.dff9(VEFU_FF4A_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D00p.out_old());
  /*_p23.NYRO*/ reg.reg_wy.NYRO_WY1n.dff9(VEFU_FF4A_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D01p.out_old());
  /*_p23.NAGA*/ reg.reg_wy.NAGA_WY2n.dff9(VEFU_FF4A_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D02p.out_old());
  /*_p23.MELA*/ reg.reg_wy.MELA_WY3n.dff9(VEFU_FF4A_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D03p.out_old());
  /*_p23.NULO*/ reg.reg_wy.NULO_WY4n.dff9(VEFU_FF4A_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D04p.out_old());
  /*_p23.NENE*/ reg.reg_wy.NENE_WY5n.dff9(VEFU_FF4A_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D05p.out_old());
  /*_p23.NUKA*/ reg.reg_wy.NUKA_WY6n.dff9(VEFU_FF4A_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D06p.out_old());
  /*_p23.NAFU*/ reg.reg_wy.NAFU_WY7n.dff9(VEFU_FF4A_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D07p.out_old());

  /*_p23.WUZA*/ wire WUZA_FF4B_WRp = and2(CUPA_CPU_WRp(), reg.cpu_abus_new.VUMY_FF4Bp());
  /*_p23.VOXU*/ wire VOXU_FF4B_WRn = not1(WUZA_FF4B_WRp);
  /*_p23.MYPA*/ reg.reg_wx.MYPA_WX0n.dff9(VOXU_FF4B_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D00p.out_old());
  /*_p23.NOFE*/ reg.reg_wx.NOFE_WX1n.dff9(VOXU_FF4B_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D01p.out_old());
  /*_p23.NOKE*/ reg.reg_wx.NOKE_WX2n.dff9(VOXU_FF4B_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D02p.out_old());
  /*_p23.MEBY*/ reg.reg_wx.MEBY_WX3n.dff9(VOXU_FF4B_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D03p.out_old());
  /*_p23.MYPU*/ reg.reg_wx.MYPU_WX4n.dff9(VOXU_FF4B_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D04p.out_old());
  /*_p23.MYCE*/ reg.reg_wx.MYCE_WX5n.dff9(VOXU_FF4B_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D05p.out_old());
  /*_p23.MUVO*/ reg.reg_wx.MUVO_WX6n.dff9(VOXU_FF4B_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D06p.out_old());
  /*_p23.NUKU*/ reg.reg_wx.NUKU_WX7n.dff9(VOXU_FF4B_WRn, WALU_SYS_RSTn(), reg.cpu_dbus_old.BUS_CPU_D07p.out_old());

  /*#p27.NAZE*/ wire NAZE_WY_MATCH0p = xnor2(reg.reg_wy.NESO_WY0n.qn_new(), reg.reg_ly.MUWY_LY0p.qp_new());
  /*_p27.PEBO*/ wire PEBO_WY_MATCH1p = xnor2(reg.reg_wy.NYRO_WY1n.qn_new(), reg.reg_ly.MYRO_LY1p.qp_new());
  /*_p27.POMO*/ wire POMO_WY_MATCH2p = xnor2(reg.reg_wy.NAGA_WY2n.qn_new(), reg.reg_ly.LEXA_LY2p.qp_new());
  /*_p27.NEVU*/ wire NEVU_WY_MATCH3p = xnor2(reg.reg_wy.MELA_WY3n.qn_new(), reg.reg_ly.LYDO_LY3p.qp_new());
  /*_p27.NOJO*/ wire NOJO_WY_MATCH4p = xnor2(reg.reg_wy.NULO_WY4n.qn_new(), reg.reg_ly.LOVU_LY4p.qp_new());
  /*_p27.PAGA*/ wire PAGA_WY_MATCH5p = xnor2(reg.reg_wy.NENE_WY5n.qn_new(), reg.reg_ly.LEMA_LY5p.qp_new());
  /*_p27.PEZO*/ wire PEZO_WY_MATCH6p = xnor2(reg.reg_wy.NUKA_WY6n.qn_new(), reg.reg_ly.MATO_LY6p.qp_new());
  /*_p27.NUPA*/ wire NUPA_WY_MATCH7p = xnor2(reg.reg_wy.NAFU_WY7n.qn_new(), reg.reg_ly.LAFO_LY7p.qp_new());

  /*#p27.MYLO*/ wire MYLO_WX_MATCH0p = xnor2(reg.pix_count.XEHO_PX0p.qp_new(), reg.reg_wx.MYPA_WX0n.qn_new());
  /*_p27.PUWU*/ wire PUWU_WX_MATCH1p = xnor2(reg.pix_count.SAVY_PX1p.qp_new(), reg.reg_wx.NOFE_WX1n.qn_new());
  /*_p27.PUHO*/ wire PUHO_WX_MATCH2p = xnor2(reg.pix_count.XODU_PX2p.qp_new(), reg.reg_wx.NOKE_WX2n.qn_new());
  /*_p27.NYTU*/ wire NYTU_WX_MATCH3p = xnor2(reg.pix_count.XYDO_PX3p.qp_new(), reg.reg_wx.MEBY_WX3n.qn_new());
  /*_p27.NEZO*/ wire NEZO_WX_MATCH4p = xnor2(reg.pix_count.TUHU_PX4p.qp_new(), reg.reg_wx.MYPU_WX4n.qn_new());
  /*_p27.NORY*/ wire NORY_WX_MATCH5p = xnor2(reg.pix_count.TUKY_PX5p.qp_new(), reg.reg_wx.MYCE_WX5n.qn_new());
  /*_p27.NONO*/ wire NONO_WX_MATCH6p = xnor2(reg.pix_count.TAKO_PX6p.qp_new(), reg.reg_wx.MUVO_WX6n.qn_new());
  /*_p27.PASE*/ wire PASE_WX_MATCH7p = xnor2(reg.pix_count.SYBE_PX7p.qp_new(), reg.reg_wx.NUKU_WX7n.qn_new());

  /*#p27.PALO*/ wire PALO_WY_MATCHn  = nand5(reg.reg_lcdc.WYMO_LCDC_WINENn.qn_new(), NOJO_WY_MATCH4p, PAGA_WY_MATCH5p, PEZO_WY_MATCH6p, NUPA_WY_MATCH7p);
  /*_p27.NELE*/ wire NELE_WY_MATCHp  = not1(PALO_WY_MATCHn);
  /*_p27.PAFU*/ wire PAFU_WY_MATCHn  = nand5(NELE_WY_MATCHp, NAZE_WY_MATCH0p, PEBO_WY_MATCH1p, POMO_WY_MATCH2p, NEVU_WY_MATCH3p);

  /*_p27.SARY*/ reg.win_ctrl.SARY_WY_MATCHp.dff17(TALU_xxCDEFxx(), XAPO_VID_RSTn(), reg.win_ctrl.ROGE_WY_MATCHp.out_old());
  /*_p27.ROGE*/ reg.win_ctrl.ROGE_WY_MATCHp = not1(PAFU_WY_MATCHn);
  /*_p27.REJO*/ reg.win_ctrl.REJO_WY_MATCH_LATCHp.nor_latch(reg.win_ctrl.SARY_WY_MATCHp.qp_new(), REPU_VBLANKp);

  /*_p27.PUKY*/ wire PUKY_WX_MATCHn  = nand5(reg.win_ctrl.REJO_WY_MATCH_LATCHp.qp_new(), NEZO_WX_MATCH4p, NORY_WX_MATCH5p, NONO_WX_MATCH6p, PASE_WX_MATCH7p);
  /*_p27.NUFA*/ wire NUFA_WX_MATCHp  = not1(PUKY_WX_MATCHn);
  /*_p27.NOGY*/ wire NOGY_WX_MATCHn  = nand5(NUFA_WX_MATCHp, MYLO_WX_MATCH0p, PUWU_WX_MATCH1p, PUHO_WX_MATCH2p, NYTU_WX_MATCH3p);
  /*_p27.NUKO*/ reg.win_ctrl.NUKO_WX_MATCHp  = not1(NOGY_WX_MATCHn);

  /*_p23.WAXU*/ wire WAXU_FF4A_RDp = and2(ASOT_CPU_RDp(), reg.cpu_abus_new.VYGA_FF4Ap());
  /*_p23.VOMY*/ wire VOMY_FF4A_RDn = not1(WAXU_FF4A_RDp);
  /*#p23.PUNU*/ triwire PUNU_WY0_TO_CD0 = tri6_nn(VOMY_FF4A_RDn, reg.reg_wy.NESO_WY0n.qp_new());
  /*_p23.PODA*/ triwire PODA_WY1_TO_CD1 = tri6_nn(VOMY_FF4A_RDn, reg.reg_wy.NYRO_WY1n.qp_new());
  /*_p23.PYGU*/ triwire PYGU_WY2_TO_CD2 = tri6_nn(VOMY_FF4A_RDn, reg.reg_wy.NAGA_WY2n.qp_new());
  /*_p23.LOKA*/ triwire LOKA_WY3_TO_CD3 = tri6_nn(VOMY_FF4A_RDn, reg.reg_wy.MELA_WY3n.qp_new());
  /*_p23.MEGA*/ triwire MEGA_WY4_TO_CD4 = tri6_nn(VOMY_FF4A_RDn, reg.reg_wy.NULO_WY4n.qp_new());
  /*_p23.PELA*/ triwire PELA_WY5_TO_CD5 = tri6_nn(VOMY_FF4A_RDn, reg.reg_wy.NENE_WY5n.qp_new());
  /*_p23.POLO*/ triwire POLO_WY6_TO_CD6 = tri6_nn(VOMY_FF4A_RDn, reg.reg_wy.NUKA_WY6n.qp_new());
  /*_p23.MERA*/ triwire MERA_WY7_TO_CD7 = tri6_nn(VOMY_FF4A_RDn, reg.reg_wy.NAFU_WY7n.qp_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus_new.BUS_CPU_D00p.tri_bus(PUNU_WY0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus_new.BUS_CPU_D01p.tri_bus(PODA_WY1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus_new.BUS_CPU_D02p.tri_bus(PYGU_WY2_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus_new.BUS_CPU_D03p.tri_bus(LOKA_WY3_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus_new.BUS_CPU_D04p.tri_bus(MEGA_WY4_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus_new.BUS_CPU_D05p.tri_bus(PELA_WY5_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus_new.BUS_CPU_D06p.tri_bus(POLO_WY6_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus_new.BUS_CPU_D07p.tri_bus(MERA_WY7_TO_CD7);

  /*_p23.WYZE*/ wire WYZE_FF4B_RDp = and2(ASOT_CPU_RDp(), reg.cpu_abus_new.VUMY_FF4Bp());
  /*_p23.VYCU*/ wire VYCU_FF4B_RDn = not1(WYZE_FF4B_RDp);
  /*#p23.LOVA*/ triwire LOVA_WX0_TO_CD0 = tri6_nn(VYCU_FF4B_RDn, reg.reg_wx.MYPA_WX0n.qp_new());
  /*_p23.MUKA*/ triwire MUKA_WX1_TO_CD1 = tri6_nn(VYCU_FF4B_RDn, reg.reg_wx.NOFE_WX1n.qp_new());
  /*_p23.MOKO*/ triwire MOKO_WX2_TO_CD2 = tri6_nn(VYCU_FF4B_RDn, reg.reg_wx.NOKE_WX2n.qp_new());
  /*_p23.LOLE*/ triwire LOLE_WX3_TO_CD3 = tri6_nn(VYCU_FF4B_RDn, reg.reg_wx.MEBY_WX3n.qp_new());
  /*_p23.MELE*/ triwire MELE_WX4_TO_CD4 = tri6_nn(VYCU_FF4B_RDn, reg.reg_wx.MYPU_WX4n.qp_new());
  /*_p23.MUFE*/ triwire MUFE_WX5_TO_CD5 = tri6_nn(VYCU_FF4B_RDn, reg.reg_wx.MYCE_WX5n.qp_new());
  /*_p23.MULY*/ triwire MULY_WX6_TO_CD6 = tri6_nn(VYCU_FF4B_RDn, reg.reg_wx.MUVO_WX6n.qp_new());
  /*_p23.MARA*/ triwire MARA_WX7_TO_CD7 = tri6_nn(VYCU_FF4B_RDn, reg.reg_wx.NUKU_WX7n.qp_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus_new.BUS_CPU_D00p.tri_bus(LOVA_WX0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus_new.BUS_CPU_D01p.tri_bus(MUKA_WX1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus_new.BUS_CPU_D02p.tri_bus(MOKO_WX2_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus_new.BUS_CPU_D03p.tri_bus(LOLE_WX3_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus_new.BUS_CPU_D04p.tri_bus(MELE_WX4_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus_new.BUS_CPU_D05p.tri_bus(MUFE_WX5_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus_new.BUS_CPU_D06p.tri_bus(MULY_WX6_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus_new.BUS_CPU_D07p.tri_bus(MARA_WX7_TO_CD7);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_pix_pipes_gates(wire SACU_CLKPIPE_new, wire NYXU_BFETCH_RSTn)
{
  /*_p24.LOBY*/ wire LOBY_RENDERINGn = not1(reg.XYMU_RENDERINGn.qn_new());

  /*#p27.LAXE*/ wire LAXE_BFETCH_S0n = not1(reg.tile_fetcher.LAXU_BFETCH_S0p.qp_new());
  /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(reg.tile_fetcher.MESU_BFETCH_S1p.qp_new());
  /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(reg.tile_fetcher.NYVA_BFETCH_S2p.qp_new());
  /*_p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(reg.sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
  /*_p29.SYCU*/ wire SYCU_SFETCH_S0pe = nor3(TYTU_SFETCH_S0n, LOBY_RENDERINGn, reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new());

  /*#p27.MYSO*/ wire MYSO_STORE_VRAM_DATA_TRIGp = nor3(LOBY_RENDERINGn, LAXE_BFETCH_S0n, reg.tile_fetcher.LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /*#p27.NYDY*/ wire NYDY_LATCH_TILE_DAn = nand3(MYSO_STORE_VRAM_DATA_TRIGp, reg.tile_fetcher.MESU_BFETCH_S1p.qp_new(), NOFU_BFETCH_S2n); // NYDY on fetch phase 6
  /*_p27.MOFU*/ wire MOFU_LATCH_TILE_DBp = and2(MYSO_STORE_VRAM_DATA_TRIGp, NAKO_BFETCH_S1n); // MOFU fires on fetch phase 2 and 10
  /*#p29.TOPU*/ wire TOPU_STORE_SPRITE_Ap = and2(reg.sprite_fetcher.TULY_SFETCH_S1p.qp_new(), SYCU_SFETCH_S0pe);
  /*#p29.RACA*/ wire RACA_STORE_SPRITE_Bp = and2(reg.sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new(), SYCU_SFETCH_S0pe);

  {
    /*#p32.METE*/ wire METE_LATCH_TILE_DAp = not1(NYDY_LATCH_TILE_DAn);
    /*#p32.LOMA*/ wire LOMA_LATCH_TILE_DAn = not1(METE_LATCH_TILE_DAp);
    /*_p32.LEGU*/ reg.tile_temp_a.LEGU_TILE_DA0n.dff8p(LOMA_LATCH_TILE_DAn, reg.vram_dbus.BUS_VRAM_D00p.out_old());
    /*_p32.NUDU*/ reg.tile_temp_a.NUDU_TILE_DA1n.dff8p(LOMA_LATCH_TILE_DAn, reg.vram_dbus.BUS_VRAM_D01p.out_old());
    /*_p32.MUKU*/ reg.tile_temp_a.MUKU_TILE_DA2n.dff8p(LOMA_LATCH_TILE_DAn, reg.vram_dbus.BUS_VRAM_D02p.out_old());
    /*_p32.LUZO*/ reg.tile_temp_a.LUZO_TILE_DA3n.dff8p(LOMA_LATCH_TILE_DAn, reg.vram_dbus.BUS_VRAM_D03p.out_old());
    /*_p32.MEGU*/ reg.tile_temp_a.MEGU_TILE_DA4n.dff8p(LOMA_LATCH_TILE_DAn, reg.vram_dbus.BUS_VRAM_D04p.out_old());
    /*_p32.MYJY*/ reg.tile_temp_a.MYJY_TILE_DA5n.dff8p(LOMA_LATCH_TILE_DAn, reg.vram_dbus.BUS_VRAM_D05p.out_old());
    /*_p32.NASA*/ reg.tile_temp_a.NASA_TILE_DA6n.dff8p(LOMA_LATCH_TILE_DAn, reg.vram_dbus.BUS_VRAM_D06p.out_old());
    /*_p32.NEFO*/ reg.tile_temp_a.NEFO_TILE_DA7n.dff8p(LOMA_LATCH_TILE_DAn, reg.vram_dbus.BUS_VRAM_D07p.out_old());
  }

  {
    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.

    /*_p32.LESO*/ wire LESO_LATCH_TILE_DBn = not1(MOFU_LATCH_TILE_DBp);
    /*_p32.LUVE*/ wire LUVE_LATCH_TILE_DBp = not1(LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /*_p32.LABU*/ wire LABU_LATCH_TILE_DBn = not1(LUVE_LATCH_TILE_DBp);
    /*_p32.RAWU*/ reg.tile_temp_b.RAWU_TILE_DB0p.dff11(LABU_LATCH_TILE_DBn, reg.SIG_VCC.out_new(), reg.vram_dbus.BUS_VRAM_D00p.out_old());
    /*_p32.POZO*/ reg.tile_temp_b.POZO_TILE_DB1p.dff11(LABU_LATCH_TILE_DBn, reg.SIG_VCC.out_new(), reg.vram_dbus.BUS_VRAM_D01p.out_old());
    /*_p32.PYZO*/ reg.tile_temp_b.PYZO_TILE_DB2p.dff11(LABU_LATCH_TILE_DBn, reg.SIG_VCC.out_new(), reg.vram_dbus.BUS_VRAM_D02p.out_old());
    /*_p32.POXA*/ reg.tile_temp_b.POXA_TILE_DB3p.dff11(LABU_LATCH_TILE_DBn, reg.SIG_VCC.out_new(), reg.vram_dbus.BUS_VRAM_D03p.out_old());
    /*_p32.PULO*/ reg.tile_temp_b.PULO_TILE_DB4p.dff11(LABU_LATCH_TILE_DBn, reg.SIG_VCC.out_new(), reg.vram_dbus.BUS_VRAM_D04p.out_old());
    /*_p32.POJU*/ reg.tile_temp_b.POJU_TILE_DB5p.dff11(LABU_LATCH_TILE_DBn, reg.SIG_VCC.out_new(), reg.vram_dbus.BUS_VRAM_D05p.out_old());
    /*_p32.POWY*/ reg.tile_temp_b.POWY_TILE_DB6p.dff11(LABU_LATCH_TILE_DBn, reg.SIG_VCC.out_new(), reg.vram_dbus.BUS_VRAM_D06p.out_old());
    /*_p32.PYJU*/ reg.tile_temp_b.PYJU_TILE_DB7p.dff11(LABU_LATCH_TILE_DBn, reg.SIG_VCC.out_new(), reg.vram_dbus.BUS_VRAM_D07p.out_old());
  }

  {
    /*#p29.VYWA*/ wire VYWA_STORE_SPRITE_An = not1(TOPU_STORE_SPRITE_Ap);
    /*#p29.WENY*/ wire WENY_STORE_SPRITE_Ap = not1(VYWA_STORE_SPRITE_An);
    /*#p29.XADO*/ wire XADO_STORE_SPRITE_An = not1(WENY_STORE_SPRITE_Ap);
    /*_p33.REWO*/ reg.sprite_pix_a.REWO_SPRITE_DA0n.dff8n(XADO_STORE_SPRITE_An, reg.flipped_sprite.PUTE_FLIP0p.out_old());
    /*_p33.PEBA*/ reg.sprite_pix_a.PEBA_SPRITE_DA1n.dff8n(XADO_STORE_SPRITE_An, reg.flipped_sprite.PELO_FLIP1p.out_old());
    /*_p33.MOFO*/ reg.sprite_pix_a.MOFO_SPRITE_DA2n.dff8n(XADO_STORE_SPRITE_An, reg.flipped_sprite.PONO_FLIP2p.out_old());
    /*_p33.PUDU*/ reg.sprite_pix_a.PUDU_SPRITE_DA3n.dff8n(XADO_STORE_SPRITE_An, reg.flipped_sprite.POBE_FLIP3p.out_old());
    /*_p33.SAJA*/ reg.sprite_pix_a.SAJA_SPRITE_DA4n.dff8n(XADO_STORE_SPRITE_An, reg.flipped_sprite.PACY_FLIP4p.out_old());
    /*_p33.SUNY*/ reg.sprite_pix_a.SUNY_SPRITE_DA5n.dff8n(XADO_STORE_SPRITE_An, reg.flipped_sprite.PUGU_FLIP5p.out_old());
    /*_p33.SEMO*/ reg.sprite_pix_a.SEMO_SPRITE_DA6n.dff8n(XADO_STORE_SPRITE_An, reg.flipped_sprite.PAWE_FLIP6p.out_old());
    /*_p33.SEGA*/ reg.sprite_pix_a.SEGA_SPRITE_DA7n.dff8n(XADO_STORE_SPRITE_An, reg.flipped_sprite.PULY_FLIP7p.out_old());
  }

  {
    /*#p29.PEBY*/ wire PEBY_STORE_SPRITE_Bn = not1(RACA_STORE_SPRITE_Bp);
    /*#p29.NYBE*/ wire NYBE_STORE_SPRITE_Bp = not1(PEBY_STORE_SPRITE_Bn);
    /*#p29.PUCO*/ wire PUCO_STORE_SPRITE_Bn = not1(NYBE_STORE_SPRITE_Bp);
    /*_p33.PEFO*/ reg.sprite_pix_b.PEFO_SPRITE_DB0n.dff8n(PUCO_STORE_SPRITE_Bn, reg.flipped_sprite.PUTE_FLIP0p.out_old());
    /*_p33.ROKA*/ reg.sprite_pix_b.ROKA_SPRITE_DB1n.dff8n(PUCO_STORE_SPRITE_Bn, reg.flipped_sprite.PELO_FLIP1p.out_old());
    /*_p33.MYTU*/ reg.sprite_pix_b.MYTU_SPRITE_DB2n.dff8n(PUCO_STORE_SPRITE_Bn, reg.flipped_sprite.PONO_FLIP2p.out_old());
    /*_p33.RAMU*/ reg.sprite_pix_b.RAMU_SPRITE_DB3n.dff8n(PUCO_STORE_SPRITE_Bn, reg.flipped_sprite.POBE_FLIP3p.out_old());
    /*_p33.SELE*/ reg.sprite_pix_b.SELE_SPRITE_DB4n.dff8n(PUCO_STORE_SPRITE_Bn, reg.flipped_sprite.PACY_FLIP4p.out_old());
    /*_p33.SUTO*/ reg.sprite_pix_b.SUTO_SPRITE_DB5n.dff8n(PUCO_STORE_SPRITE_Bn, reg.flipped_sprite.PUGU_FLIP5p.out_old());
    /*_p33.RAMA*/ reg.sprite_pix_b.RAMA_SPRITE_DB6n.dff8n(PUCO_STORE_SPRITE_Bn, reg.flipped_sprite.PAWE_FLIP6p.out_old());
    /*_p33.RYDU*/ reg.sprite_pix_b.RYDU_SPRITE_DB7n.dff8n(PUCO_STORE_SPRITE_Bn, reg.flipped_sprite.PULY_FLIP7p.out_old());
  }

  /*_p29.XEFY*/ wire XEFY_SPRITE_DONEn = not1(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new());

  // FIXME - old? well,i guess there's another feedback loop here...

  /*_p34.MEFU*/ wire MEFU_SPRITE_MASK0n = or3(XEFY_SPRITE_DONEn, reg.spr_pipe_b.NYLU_SPR_PIPE_B0.qp_old(), reg.spr_pipe_a.NURO_SPR_PIPE_A0.qp_old());
  /*_p34.MEVE*/ wire MEVE_SPRITE_MASK1n = or3(XEFY_SPRITE_DONEn, reg.spr_pipe_b.PEFU_SPR_PIPE_B1.qp_old(), reg.spr_pipe_a.MASO_SPR_PIPE_A1.qp_old());
  /*_p34.MYZO*/ wire MYZO_SPRITE_MASK2n = or3(XEFY_SPRITE_DONEn, reg.spr_pipe_b.NATY_SPR_PIPE_B2.qp_old(), reg.spr_pipe_a.LEFE_SPR_PIPE_A2.qp_old());
  /*_p34.RUDA*/ wire RUDA_SPRITE_MASK3n = or3(XEFY_SPRITE_DONEn, reg.spr_pipe_b.PYJO_SPR_PIPE_B3.qp_old(), reg.spr_pipe_a.LESU_SPR_PIPE_A3.qp_old());
  /*_p34.VOTO*/ wire VOTO_SPRITE_MASK4n = or3(XEFY_SPRITE_DONEn, reg.spr_pipe_b.VARE_SPR_PIPE_B4.qp_old(), reg.spr_pipe_a.WYHO_SPR_PIPE_A4.qp_old());
  /*_p34.VYSA*/ wire VYSA_SPRITE_MASK5n = or3(XEFY_SPRITE_DONEn, reg.spr_pipe_b.WEBA_SPR_PIPE_B5.qp_old(), reg.spr_pipe_a.WORA_SPR_PIPE_A5.qp_old());
  /*_p34.TORY*/ wire TORY_SPRITE_MASK6n = or3(XEFY_SPRITE_DONEn, reg.spr_pipe_b.VANU_SPR_PIPE_B6.qp_old(), reg.spr_pipe_a.VAFO_SPR_PIPE_A6.qp_old());
  /*_p34.WOPE*/ wire WOPE_SPRITE_MASK7n = or3(XEFY_SPRITE_DONEn, reg.spr_pipe_b.VUPY_SPR_PIPE_B7.qp_old(), reg.spr_pipe_a.WUFY_SPR_PIPE_A7.qp_old());

  /*_p34.LESY*/ wire LESY_SPRITE_MASK0p = not1(MEFU_SPRITE_MASK0n);
  /*_p34.LOTA*/ wire LOTA_SPRITE_MASK1p = not1(MEVE_SPRITE_MASK1n);
  /*_p34.LYKU*/ wire LYKU_SPRITE_MASK2p = not1(MYZO_SPRITE_MASK2n);
  /*_p34.ROBY*/ wire ROBY_SPRITE_MASK3p = not1(RUDA_SPRITE_MASK3n);
  /*_p34.TYTA*/ wire TYTA_SPRITE_MASK4p = not1(VOTO_SPRITE_MASK4n);
  /*_p34.TYCO*/ wire TYCO_SPRITE_MASK5p = not1(VYSA_SPRITE_MASK5n);
  /*_p34.SOKA*/ wire SOKA_SPRITE_MASK6p = not1(TORY_SPRITE_MASK6n);
  /*_p34.XOVU*/ wire XOVU_SPRITE_MASK7p = not1(WOPE_SPRITE_MASK7n);

  //----------------------------------------
  // Sprite pipe A
  {
    /*_p33.PABE*/ wire PABE_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, reg.sprite_pix_a.REWO_SPRITE_DA0n.qn_new());
    /*_p33.MYTO*/ wire MYTO_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, reg.sprite_pix_a.PEBA_SPRITE_DA1n.qn_new());
    /*_p33.LELA*/ wire LELA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, reg.sprite_pix_a.MOFO_SPRITE_DA2n.qn_new());
    /*_p33.MAME*/ wire MAME_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, reg.sprite_pix_a.PUDU_SPRITE_DA3n.qn_new());
    /*_p33.VEXU*/ wire VEXU_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, reg.sprite_pix_a.SAJA_SPRITE_DA4n.qn_new());
    /*_p33.VABY*/ wire VABY_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, reg.sprite_pix_a.SUNY_SPRITE_DA5n.qn_new());
    /*_p33.TUXA*/ wire TUXA_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, reg.sprite_pix_a.SEMO_SPRITE_DA6n.qn_new());
    /*_p33.VUNE*/ wire VUNE_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, reg.sprite_pix_a.SEGA_SPRITE_DA7n.qn_new());

    /*_p33.RATA*/ wire RATA_SPR_PIX_DA0n = not1(reg.sprite_pix_a.REWO_SPRITE_DA0n.qn_new());
    /*_p33.NUCA*/ wire NUCA_SPR_PIX_DA1n = not1(reg.sprite_pix_a.PEBA_SPRITE_DA1n.qn_new());
    /*_p33.LASE*/ wire LASE_SPR_PIX_DA2n = not1(reg.sprite_pix_a.MOFO_SPRITE_DA2n.qn_new());
    /*_p33.LUBO*/ wire LUBO_SPR_PIX_DA3n = not1(reg.sprite_pix_a.PUDU_SPRITE_DA3n.qn_new());
    /*_p33.WERY*/ wire WERY_SPR_PIX_DA4n = not1(reg.sprite_pix_a.SAJA_SPRITE_DA4n.qn_new());
    /*_p33.WURA*/ wire WURA_SPR_PIX_DA5n = not1(reg.sprite_pix_a.SUNY_SPRITE_DA5n.qn_new());
    /*_p33.SULU*/ wire SULU_SPR_PIX_DA6n = not1(reg.sprite_pix_a.SEMO_SPRITE_DA6n.qn_new());
    /*_p33.WAMY*/ wire WAMY_SPR_PIX_DA7n = not1(reg.sprite_pix_a.SEGA_SPRITE_DA7n.qn_new());

    /*_p33.PYZU*/ wire PYZU_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, RATA_SPR_PIX_DA0n);
    /*_p33.MADA*/ wire MADA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, NUCA_SPR_PIX_DA1n);
    /*_p33.LYDE*/ wire LYDE_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LASE_SPR_PIX_DA2n);
    /*_p33.LUFY*/ wire LUFY_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, LUBO_SPR_PIX_DA3n);
    /*_p33.XATO*/ wire XATO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, WERY_SPR_PIX_DA4n);
    /*_p33.XEXU*/ wire XEXU_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WURA_SPR_PIX_DA5n);
    /*_p33.TUPE*/ wire TUPE_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SULU_SPR_PIX_DA6n);
    /*_p33.XYVE*/ wire XYVE_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, WAMY_SPR_PIX_DA7n);

    /*_p33.WUFY*/ reg.spr_pipe_a.WUFY_SPR_PIPE_A7.dff22(SACU_CLKPIPE_new, VUNE_SPR_PIX_SET7, XYVE_SPR_PIX_RST7, reg.spr_pipe_a.VAFO_SPR_PIPE_A6.qp_old());
    /*_p33.VAFO*/ reg.spr_pipe_a.VAFO_SPR_PIPE_A6.dff22(SACU_CLKPIPE_new, TUXA_SPR_PIX_SET6, TUPE_SPR_PIX_RST6, reg.spr_pipe_a.WORA_SPR_PIPE_A5.qp_old());
    /*_p33.WORA*/ reg.spr_pipe_a.WORA_SPR_PIPE_A5.dff22(SACU_CLKPIPE_new, VABY_SPR_PIX_SET5, XEXU_SPR_PIX_RST5, reg.spr_pipe_a.WYHO_SPR_PIPE_A4.qp_old());
    /*_p33.WYHO*/ reg.spr_pipe_a.WYHO_SPR_PIPE_A4.dff22(SACU_CLKPIPE_new, VEXU_SPR_PIX_SET4, XATO_SPR_PIX_RST4, reg.spr_pipe_a.LESU_SPR_PIPE_A3.qp_old());
    /*_p33.LESU*/ reg.spr_pipe_a.LESU_SPR_PIPE_A3.dff22(SACU_CLKPIPE_new, MAME_SPR_PIX_SET3, LUFY_SPR_PIX_RST3, reg.spr_pipe_a.LEFE_SPR_PIPE_A2.qp_old());
    /*_p33.LEFE*/ reg.spr_pipe_a.LEFE_SPR_PIPE_A2.dff22(SACU_CLKPIPE_new, LELA_SPR_PIX_SET2, LYDE_SPR_PIX_RST2, reg.spr_pipe_a.MASO_SPR_PIPE_A1.qp_old());
    /*_p33.MASO*/ reg.spr_pipe_a.MASO_SPR_PIPE_A1.dff22(SACU_CLKPIPE_new, MYTO_SPR_PIX_SET1, MADA_SPR_PIX_RST1, reg.spr_pipe_a.NURO_SPR_PIPE_A0.qp_old());
    /*_p33.NURO*/ reg.spr_pipe_a.NURO_SPR_PIPE_A0.dff22(SACU_CLKPIPE_new, PABE_SPR_PIX_SET0, PYZU_SPR_PIX_RST0, reg.SIG_GND.out_new());
  }

  //----------------------------------------
  // Sprite pipe B
  {
    /*_p33.MEZU*/ wire MEZU_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, reg.sprite_pix_b.PEFO_SPRITE_DB0n.qn_new());
    /*_p33.RUSY*/ wire RUSY_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, reg.sprite_pix_b.ROKA_SPRITE_DB1n.qn_new());
    /*_p33.MYXA*/ wire MYXA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, reg.sprite_pix_b.MYTU_SPRITE_DB2n.qn_new());
    /*_p33.RANO*/ wire RANO_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, reg.sprite_pix_b.RAMU_SPRITE_DB3n.qn_new());
    /*_p33.TYGA*/ wire TYGA_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, reg.sprite_pix_b.SELE_SPRITE_DB4n.qn_new());
    /*_p33.VUME*/ wire VUME_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, reg.sprite_pix_b.SUTO_SPRITE_DB5n.qn_new());
    /*_p33.TAPO*/ wire TAPO_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, reg.sprite_pix_b.RAMA_SPRITE_DB6n.qn_new());
    /*_p33.TESO*/ wire TESO_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, reg.sprite_pix_b.RYDU_SPRITE_DB7n.qn_new());

    /*_p33.LOZA*/ wire LOZA_SPR_PIX_DB0n = not1(reg.sprite_pix_b.PEFO_SPRITE_DB0n.qn_new());
    /*_p33.SYBO*/ wire SYBO_SPR_PIX_DB1n = not1(reg.sprite_pix_b.ROKA_SPRITE_DB1n.qn_new());
    /*_p33.LUMO*/ wire LUMO_SPR_PIX_DB2n = not1(reg.sprite_pix_b.MYTU_SPRITE_DB2n.qn_new());
    /*_p33.SOLO*/ wire SOLO_SPR_PIX_DB3n = not1(reg.sprite_pix_b.RAMU_SPRITE_DB3n.qn_new());
    /*_p33.VOBY*/ wire VOBY_SPR_PIX_DB4n = not1(reg.sprite_pix_b.SELE_SPRITE_DB4n.qn_new());
    /*_p33.WYCO*/ wire WYCO_SPR_PIX_DB5n = not1(reg.sprite_pix_b.SUTO_SPRITE_DB5n.qn_new());
    /*_p33.SERY*/ wire SERY_SPR_PIX_DB6n = not1(reg.sprite_pix_b.RAMA_SPRITE_DB6n.qn_new());
    /*_p33.SELU*/ wire SELU_SPR_PIX_DB7n = not1(reg.sprite_pix_b.RYDU_SPRITE_DB7n.qn_new());

    /*_p33.MOFY*/ wire MOFY_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, LOZA_SPR_PIX_DB0n);
    /*_p33.RUCA*/ wire RUCA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, SYBO_SPR_PIX_DB1n);
    /*_p33.MAJO*/ wire MAJO_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LUMO_SPR_PIX_DB2n);
    /*_p33.REHU*/ wire REHU_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, SOLO_SPR_PIX_DB3n);
    /*_p33.WAXO*/ wire WAXO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, VOBY_SPR_PIX_DB4n);
    /*_p33.XOLE*/ wire XOLE_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WYCO_SPR_PIX_DB5n);
    /*_p33.TABY*/ wire TABY_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SERY_SPR_PIX_DB6n);
    /*_p33.TULA*/ wire TULA_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, SELU_SPR_PIX_DB7n);

    /*_p33.VUPY*/ reg.spr_pipe_b.VUPY_SPR_PIPE_B7.dff22(SACU_CLKPIPE_new, TESO_SPR_PIX_SET7, TULA_SPR_PIX_RST7, reg.spr_pipe_b.VANU_SPR_PIPE_B6.qp_old());
    /*_p33.VANU*/ reg.spr_pipe_b.VANU_SPR_PIPE_B6.dff22(SACU_CLKPIPE_new, TAPO_SPR_PIX_SET6, TABY_SPR_PIX_RST6, reg.spr_pipe_b.WEBA_SPR_PIPE_B5.qp_old());
    /*_p33.WEBA*/ reg.spr_pipe_b.WEBA_SPR_PIPE_B5.dff22(SACU_CLKPIPE_new, VUME_SPR_PIX_SET5, XOLE_SPR_PIX_RST5, reg.spr_pipe_b.VARE_SPR_PIPE_B4.qp_old());
    /*_p33.VARE*/ reg.spr_pipe_b.VARE_SPR_PIPE_B4.dff22(SACU_CLKPIPE_new, TYGA_SPR_PIX_SET4, WAXO_SPR_PIX_RST4, reg.spr_pipe_b.PYJO_SPR_PIPE_B3.qp_old());
    /*_p33.PYJO*/ reg.spr_pipe_b.PYJO_SPR_PIPE_B3.dff22(SACU_CLKPIPE_new, RANO_SPR_PIX_SET3, REHU_SPR_PIX_RST3, reg.spr_pipe_b.NATY_SPR_PIPE_B2.qp_old());
    /*_p33.NATY*/ reg.spr_pipe_b.NATY_SPR_PIPE_B2.dff22(SACU_CLKPIPE_new, MYXA_SPR_PIX_SET2, MAJO_SPR_PIX_RST2, reg.spr_pipe_b.PEFU_SPR_PIPE_B1.qp_old());
    /*_p33.PEFU*/ reg.spr_pipe_b.PEFU_SPR_PIPE_B1.dff22(SACU_CLKPIPE_new, RUSY_SPR_PIX_SET1, RUCA_SPR_PIX_RST1, reg.spr_pipe_b.NYLU_SPR_PIPE_B0.qp_old());
    /*_p33.NYLU*/ reg.spr_pipe_b.NYLU_SPR_PIPE_B0.dff22(SACU_CLKPIPE_new, MEZU_SPR_PIX_SET0, MOFY_SPR_PIX_RST0, reg.SIG_GND.out_new());
  }

  //----------------------------------------
  // Background pipe A
  {
    /*_p32.LOZE*/ wire LOZE_PIPE_A_LOADp = not1(NYXU_BFETCH_RSTn);
    /*_p32.LAKY*/ wire LAKY_BG_PIX_SET0 = nand2(LOZE_PIPE_A_LOADp, reg.tile_temp_a.LEGU_TILE_DA0n.qn_new());
    /*_p32.NYXO*/ wire NYXO_BG_PIX_SET1 = nand2(LOZE_PIPE_A_LOADp, reg.tile_temp_a.NUDU_TILE_DA1n.qn_new());
    /*_p32.LOTO*/ wire LOTO_BG_PIX_SET2 = nand2(LOZE_PIPE_A_LOADp, reg.tile_temp_a.MUKU_TILE_DA2n.qn_new());
    /*_p32.LYDU*/ wire LYDU_BG_PIX_SET3 = nand2(LOZE_PIPE_A_LOADp, reg.tile_temp_a.LUZO_TILE_DA3n.qn_new());
    /*_p32.MYVY*/ wire MYVY_BG_PIX_SET4 = nand2(LOZE_PIPE_A_LOADp, reg.tile_temp_a.MEGU_TILE_DA4n.qn_new());
    /*_p32.LODO*/ wire LODO_BG_PIX_SET5 = nand2(LOZE_PIPE_A_LOADp, reg.tile_temp_a.MYJY_TILE_DA5n.qn_new());
    /*_p32.NUTE*/ wire NUTE_BG_PIX_SET6 = nand2(LOZE_PIPE_A_LOADp, reg.tile_temp_a.NASA_TILE_DA6n.qn_new());
    /*_p32.NAJA*/ wire NAJA_BG_PIX_SET7 = nand2(LOZE_PIPE_A_LOADp, reg.tile_temp_a.NEFO_TILE_DA7n.qn_new());

    /*_p32.LUHE*/ wire LUHE_BG_PIX_DA0n = not1(reg.tile_temp_a.LEGU_TILE_DA0n.qn_new());
    /*_p32.NOLY*/ wire NOLY_BG_PIX_DA1n = not1(reg.tile_temp_a.NUDU_TILE_DA1n.qn_new());
    /*_p32.LEKE*/ wire LEKE_BG_PIX_DA2n = not1(reg.tile_temp_a.MUKU_TILE_DA2n.qn_new());
    /*_p32.LOMY*/ wire LOMY_BG_PIX_DA3n = not1(reg.tile_temp_a.LUZO_TILE_DA3n.qn_new());
    /*_p32.LALA*/ wire LALA_BG_PIX_DA4n = not1(reg.tile_temp_a.MEGU_TILE_DA4n.qn_new());
    /*_p32.LOXA*/ wire LOXA_BG_PIX_DA5n = not1(reg.tile_temp_a.MYJY_TILE_DA5n.qn_new());
    /*_p32.NEZE*/ wire NEZE_BG_PIX_DA6n = not1(reg.tile_temp_a.NASA_TILE_DA6n.qn_new());
    /*_p32.NOBO*/ wire NOBO_BG_PIX_DA7n = not1(reg.tile_temp_a.NEFO_TILE_DA7n.qn_new());

    /*_p32.LOTY*/ wire LOTY_BG_PIX_RST0 = nand2(LOZE_PIPE_A_LOADp, LUHE_BG_PIX_DA0n);
    /*_p32.NEXA*/ wire NEXA_BG_PIX_RST1 = nand2(LOZE_PIPE_A_LOADp, NOLY_BG_PIX_DA1n);
    /*_p32.LUTU*/ wire LUTU_BG_PIX_RST2 = nand2(LOZE_PIPE_A_LOADp, LEKE_BG_PIX_DA2n);
    /*_p32.LUJA*/ wire LUJA_BG_PIX_RST3 = nand2(LOZE_PIPE_A_LOADp, LOMY_BG_PIX_DA3n);
    /*_p32.MOSY*/ wire MOSY_BG_PIX_RST4 = nand2(LOZE_PIPE_A_LOADp, LALA_BG_PIX_DA4n);
    /*_p32.LERU*/ wire LERU_BG_PIX_RST5 = nand2(LOZE_PIPE_A_LOADp, LOXA_BG_PIX_DA5n);
    /*_p32.NYHA*/ wire NYHA_BG_PIX_RST6 = nand2(LOZE_PIPE_A_LOADp, NEZE_BG_PIX_DA6n);
    /*_p32.NADY*/ wire NADY_BG_PIX_RST7 = nand2(LOZE_PIPE_A_LOADp, NOBO_BG_PIX_DA7n);

    /*_p32.PYBO*/ reg.bgw_pipe_a.PYBO_BGW_PIPE_A7.dff22(SACU_CLKPIPE_new, NAJA_BG_PIX_SET7, NADY_BG_PIX_RST7, reg.bgw_pipe_a.NEDA_BGW_PIPE_A6.qp_old());
    /*_p32.NEDA*/ reg.bgw_pipe_a.NEDA_BGW_PIPE_A6.dff22(SACU_CLKPIPE_new, NUTE_BG_PIX_SET6, NYHA_BG_PIX_RST6, reg.bgw_pipe_a.MODU_BGW_PIPE_A5.qp_old());
    /*_p32.MODU*/ reg.bgw_pipe_a.MODU_BGW_PIPE_A5.dff22(SACU_CLKPIPE_new, LODO_BG_PIX_SET5, LERU_BG_PIX_RST5, reg.bgw_pipe_a.NEPO_BGW_PIPE_A4.qp_old());
    /*_p32.NEPO*/ reg.bgw_pipe_a.NEPO_BGW_PIPE_A4.dff22(SACU_CLKPIPE_new, MYVY_BG_PIX_SET4, MOSY_BG_PIX_RST4, reg.bgw_pipe_a.MACU_BGW_PIPE_A3.qp_old());
    /*_p32.MACU*/ reg.bgw_pipe_a.MACU_BGW_PIPE_A3.dff22(SACU_CLKPIPE_new, LYDU_BG_PIX_SET3, LUJA_BG_PIX_RST3, reg.bgw_pipe_a.MOJU_BGW_PIPE_A2.qp_old());
    /*_p32.MOJU*/ reg.bgw_pipe_a.MOJU_BGW_PIPE_A2.dff22(SACU_CLKPIPE_new, LOTO_BG_PIX_SET2, LUTU_BG_PIX_RST2, reg.bgw_pipe_a.NOZO_BGW_PIPE_A1.qp_old());
    /*_p32.NOZO*/ reg.bgw_pipe_a.NOZO_BGW_PIPE_A1.dff22(SACU_CLKPIPE_new, NYXO_BG_PIX_SET1, NEXA_BG_PIX_RST1, reg.bgw_pipe_a.MYDE_BGW_PIPE_A0.qp_old());
    /*_p32.MYDE*/ reg.bgw_pipe_a.MYDE_BGW_PIPE_A0.dff22(SACU_CLKPIPE_new, LAKY_BG_PIX_SET0, LOTY_BG_PIX_RST0, reg.SIG_GND.out_new());
  }

  //----------------------------------------
  // Background pipe B
  {
    /*_p32.LUXA*/ wire LUXA_PIPE_B_LOADp = not1(NYXU_BFETCH_RSTn);
    /*_p32.TUXE*/ wire TUXE_BG_PIX_SET0 = nand2(LUXA_PIPE_B_LOADp, reg.tile_temp_b.RAWU_TILE_DB0p.qp_new());
    /*_p32.SOLY*/ wire SOLY_BG_PIX_SET1 = nand2(LUXA_PIPE_B_LOADp, reg.tile_temp_b.POZO_TILE_DB1p.qp_new());
    /*_p32.RUCE*/ wire RUCE_BG_PIX_SET2 = nand2(LUXA_PIPE_B_LOADp, reg.tile_temp_b.PYZO_TILE_DB2p.qp_new());
    /*_p32.RYJA*/ wire RYJA_BG_PIX_SET3 = nand2(LUXA_PIPE_B_LOADp, reg.tile_temp_b.POXA_TILE_DB3p.qp_new());
    /*_p32.RUTO*/ wire RUTO_BG_PIX_SET4 = nand2(LUXA_PIPE_B_LOADp, reg.tile_temp_b.PULO_TILE_DB4p.qp_new());
    /*_p32.RAJA*/ wire RAJA_BG_PIX_SET5 = nand2(LUXA_PIPE_B_LOADp, reg.tile_temp_b.POJU_TILE_DB5p.qp_new());
    /*_p32.RAJO*/ wire RAJO_BG_PIX_SET6 = nand2(LUXA_PIPE_B_LOADp, reg.tile_temp_b.POWY_TILE_DB6p.qp_new());
    /*_p32.RAGA*/ wire RAGA_BG_PIX_SET7 = nand2(LUXA_PIPE_B_LOADp, reg.tile_temp_b.PYJU_TILE_DB7p.qp_new());

    /*_p32.TOSA*/ wire TOSA_BG_PIX_DB0n = not1(reg.tile_temp_b.RAWU_TILE_DB0p.qp_new());
    /*_p32.RUCO*/ wire RUCO_BG_PIX_DB1n = not1(reg.tile_temp_b.POZO_TILE_DB1p.qp_new());
    /*_p32.TYCE*/ wire TYCE_BG_PIX_DB2n = not1(reg.tile_temp_b.PYZO_TILE_DB2p.qp_new());
    /*_p32.REVY*/ wire REVY_BG_PIX_DB3n = not1(reg.tile_temp_b.POXA_TILE_DB3p.qp_new());
    /*_p32.RYGA*/ wire RYGA_BG_PIX_DB4n = not1(reg.tile_temp_b.PULO_TILE_DB4p.qp_new());
    /*_p32.RYLE*/ wire RYLE_BG_PIX_DB5n = not1(reg.tile_temp_b.POJU_TILE_DB5p.qp_new());
    /*_p32.RAPU*/ wire RAPU_BG_PIX_DB6n = not1(reg.tile_temp_b.POWY_TILE_DB6p.qp_new());
    /*_p32.SOJA*/ wire SOJA_BG_PIX_DB7n = not1(reg.tile_temp_b.PYJU_TILE_DB7p.qp_new());

    /*_p32.SEJA*/ wire SEJA_BG_PIX_RST0 = nand2(LUXA_PIPE_B_LOADp, TOSA_BG_PIX_DB0n);
    /*_p32.SENO*/ wire SENO_BG_PIX_RST1 = nand2(LUXA_PIPE_B_LOADp, RUCO_BG_PIX_DB1n);
    /*_p32.SURE*/ wire SURE_BG_PIX_RST2 = nand2(LUXA_PIPE_B_LOADp, TYCE_BG_PIX_DB2n);
    /*_p32.SEBO*/ wire SEBO_BG_PIX_RST3 = nand2(LUXA_PIPE_B_LOADp, REVY_BG_PIX_DB3n);
    /*_p32.SUCA*/ wire SUCA_BG_PIX_RST4 = nand2(LUXA_PIPE_B_LOADp, RYGA_BG_PIX_DB4n);
    /*_p32.SYWE*/ wire SYWE_BG_PIX_RST5 = nand2(LUXA_PIPE_B_LOADp, RYLE_BG_PIX_DB5n);
    /*_p32.SUPU*/ wire SUPU_BG_PIX_RST6 = nand2(LUXA_PIPE_B_LOADp, RAPU_BG_PIX_DB6n);
    /*_p32.RYJY*/ wire RYJY_BG_PIX_RST7 = nand2(LUXA_PIPE_B_LOADp, SOJA_BG_PIX_DB7n);

    /*_p32.SOHU*/ reg.bgw_pipe_b.SOHU_BGW_PIPE_B7.dff22(SACU_CLKPIPE_new, RAGA_BG_PIX_SET7, RYJY_BG_PIX_RST7, reg.bgw_pipe_b.RALU_BGW_PIPE_B6.qp_old());
    /*_p32.RALU*/ reg.bgw_pipe_b.RALU_BGW_PIPE_B6.dff22(SACU_CLKPIPE_new, RAJO_BG_PIX_SET6, SUPU_BG_PIX_RST6, reg.bgw_pipe_b.SETU_BGW_PIPE_B5.qp_old());
    /*_p32.SETU*/ reg.bgw_pipe_b.SETU_BGW_PIPE_B5.dff22(SACU_CLKPIPE_new, RAJA_BG_PIX_SET5, SYWE_BG_PIX_RST5, reg.bgw_pipe_b.SOBO_BGW_PIPE_B4.qp_old());
    /*_p32.SOBO*/ reg.bgw_pipe_b.SOBO_BGW_PIPE_B4.dff22(SACU_CLKPIPE_new, RUTO_BG_PIX_SET4, SUCA_BG_PIX_RST4, reg.bgw_pipe_b.RYSA_BGW_PIPE_B3.qp_old());
    /*_p32.RYSA*/ reg.bgw_pipe_b.RYSA_BGW_PIPE_B3.dff22(SACU_CLKPIPE_new, RYJA_BG_PIX_SET3, SEBO_BG_PIX_RST3, reg.bgw_pipe_b.SADY_BGW_PIPE_B2.qp_old());
    /*_p32.SADY*/ reg.bgw_pipe_b.SADY_BGW_PIPE_B2.dff22(SACU_CLKPIPE_new, RUCE_BG_PIX_SET2, SURE_BG_PIX_RST2, reg.bgw_pipe_b.TACA_BGW_PIPE_B1.qp_old());
    /*_p32.TACA*/ reg.bgw_pipe_b.TACA_BGW_PIPE_B1.dff22(SACU_CLKPIPE_new, SOLY_BG_PIX_SET1, SENO_BG_PIX_RST1, reg.bgw_pipe_b.TOMY_BGW_PIPE_B0.qp_old());
    /*_p32.TOMY*/ reg.bgw_pipe_b.TOMY_BGW_PIPE_B0.dff22(SACU_CLKPIPE_new, TUXE_BG_PIX_SET0, SEJA_BG_PIX_RST0, reg.SIG_GND.out_new());
  }

  //----------------------------------------
  // Mask pipe
  {
    /*_p26.TEDE*/ wire TEDE_MASK_PIPE_SET0 = nand2(LESY_SPRITE_MASK0p, reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.XALA*/ wire XALA_MASK_PIPE_SET1 = nand2(LOTA_SPRITE_MASK1p, reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.TYRA*/ wire TYRA_MASK_PIPE_SET2 = nand2(LYKU_SPRITE_MASK2p, reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.XYRU*/ wire XYRU_MASK_PIPE_SET3 = nand2(ROBY_SPRITE_MASK3p, reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.XUKU*/ wire XUKU_MASK_PIPE_SET4 = nand2(TYTA_SPRITE_MASK4p, reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.XELY*/ wire XELY_MASK_PIPE_SET5 = nand2(TYCO_SPRITE_MASK5p, reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.TYKO*/ wire TYKO_MASK_PIPE_SET6 = nand2(SOKA_SPRITE_MASK6p, reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.TUWU*/ wire TUWU_MASK_PIPE_SET7 = nand2(XOVU_SPRITE_MASK7p, reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());

    /*_p26.XOGA*/ wire XOGA_MASK_PIPE_DB7n = not1(reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.XURA*/ wire XURA_MASK_PIPE_DB7n = not1(reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.TAJO*/ wire TAJO_MASK_PIPE_DB7n = not1(reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.XENU*/ wire XENU_MASK_PIPE_DB7n = not1(reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.XYKE*/ wire XYKE_MASK_PIPE_DB7n = not1(reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.XABA*/ wire XABA_MASK_PIPE_DB7n = not1(reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.TAFU*/ wire TAFU_MASK_PIPE_DB7n = not1(reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());
    /*_p26.XUHO*/ wire XUHO_MASK_PIPE_DB7n = not1(reg.oam_temp_b.DEPO_OAM_DB7p.qp_new());

    /*_p26.WOKA*/ wire WOKA_MASK_PIPE_RST0 = nand2(LESY_SPRITE_MASK0p, XOGA_MASK_PIPE_DB7n);
    /*_p26.WEDE*/ wire WEDE_MASK_PIPE_RST1 = nand2(LOTA_SPRITE_MASK1p, XURA_MASK_PIPE_DB7n);
    /*_p26.TUFO*/ wire TUFO_MASK_PIPE_RST2 = nand2(LYKU_SPRITE_MASK2p, TAJO_MASK_PIPE_DB7n);
    /*_p26.WEVO*/ wire WEVO_MASK_PIPE_RST3 = nand2(ROBY_SPRITE_MASK3p, XENU_MASK_PIPE_DB7n);
    /*_p26.WEDY*/ wire WEDY_MASK_PIPE_RST4 = nand2(TYTA_SPRITE_MASK4p, XYKE_MASK_PIPE_DB7n);
    /*_p26.WUJA*/ wire WUJA_MASK_PIPE_RST5 = nand2(TYCO_SPRITE_MASK5p, XABA_MASK_PIPE_DB7n);
    /*_p26.TENA*/ wire TENA_MASK_PIPE_RST6 = nand2(SOKA_SPRITE_MASK6p, TAFU_MASK_PIPE_DB7n);
    /*_p26.WUBU*/ wire WUBU_MASK_PIPE_RST7 = nand2(XOVU_SPRITE_MASK7p, XUHO_MASK_PIPE_DB7n);

    /*_p26.VAVA*/ reg.mask_pipe.VAVA_MASK_PIPE_7.dff22(SACU_CLKPIPE_new, TUWU_MASK_PIPE_SET7, WUBU_MASK_PIPE_RST7, reg.mask_pipe.VUMO_MASK_PIPE_6.qp_old());
    /*_p26.VUMO*/ reg.mask_pipe.VUMO_MASK_PIPE_6.dff22(SACU_CLKPIPE_new, TYKO_MASK_PIPE_SET6, TENA_MASK_PIPE_RST6, reg.mask_pipe.WODA_MASK_PIPE_5.qp_old());
    /*_p26.WODA*/ reg.mask_pipe.WODA_MASK_PIPE_5.dff22(SACU_CLKPIPE_new, XELY_MASK_PIPE_SET5, WUJA_MASK_PIPE_RST5, reg.mask_pipe.XETE_MASK_PIPE_4.qp_old());
    /*_p26.XETE*/ reg.mask_pipe.XETE_MASK_PIPE_4.dff22(SACU_CLKPIPE_new, XUKU_MASK_PIPE_SET4, WEDY_MASK_PIPE_RST4, reg.mask_pipe.WYFU_MASK_PIPE_3.qp_old());
    /*_p26.WYFU*/ reg.mask_pipe.WYFU_MASK_PIPE_3.dff22(SACU_CLKPIPE_new, XYRU_MASK_PIPE_SET3, WEVO_MASK_PIPE_RST3, reg.mask_pipe.VOSA_MASK_PIPE_2.qp_old());
    /*_p26.VOSA*/ reg.mask_pipe.VOSA_MASK_PIPE_2.dff22(SACU_CLKPIPE_new, TYRA_MASK_PIPE_SET2, TUFO_MASK_PIPE_RST2, reg.mask_pipe.WURU_MASK_PIPE_1.qp_old());
    /*_p26.WURU*/ reg.mask_pipe.WURU_MASK_PIPE_1.dff22(SACU_CLKPIPE_new, XALA_MASK_PIPE_SET1, WEDE_MASK_PIPE_RST1, reg.mask_pipe.VEZO_MASK_PIPE_0.qp_old());
    /*_p26.VEZO*/ reg.mask_pipe.VEZO_MASK_PIPE_0.dff22(SACU_CLKPIPE_new, TEDE_MASK_PIPE_SET0, WOKA_MASK_PIPE_RST0, reg.SIG_VCC.out_new());
  }

  //----------------------------------------
  // Pal pipe
  {
    /*_p34.PUME*/ wire PUME_PAL_PIPE_SET0 = nand2(LESY_SPRITE_MASK0p, reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.SORO*/ wire SORO_PAL_PIPE_SET1 = nand2(LOTA_SPRITE_MASK1p, reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.PAMO*/ wire PAMO_PAL_PIPE_SET2 = nand2(LYKU_SPRITE_MASK2p, reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.SUKY*/ wire SUKY_PAL_PIPE_SET3 = nand2(ROBY_SPRITE_MASK3p, reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.RORA*/ wire RORA_PAL_PIPE_SET4 = nand2(TYTA_SPRITE_MASK4p, reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.MENE*/ wire MENE_PAL_PIPE_SET5 = nand2(TYCO_SPRITE_MASK5p, reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.LUKE*/ wire LUKE_PAL_PIPE_SET6 = nand2(SOKA_SPRITE_MASK6p, reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.LAMY*/ wire LAMY_PAL_PIPE_SET7 = nand2(XOVU_SPRITE_MASK7p, reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());

    /*_p34.SYPY*/ wire SYPY_PAL_PIPE_DB4n = not1(reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.TOTU*/ wire TOTU_PAL_PIPE_DB4n = not1(reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.NARO*/ wire NARO_PAL_PIPE_DB4n = not1(reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.WEXY*/ wire WEXY_PAL_PIPE_DB4n = not1(reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.RYZY*/ wire RYZY_PAL_PIPE_DB4n = not1(reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.RYFE*/ wire RYFE_PAL_PIPE_DB4n = not1(reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.LADY*/ wire LADY_PAL_PIPE_DB4n = not1(reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());
    /*_p34.LAFY*/ wire LAFY_PAL_PIPE_DB4n = not1(reg.oam_temp_b.GOMO_OAM_DB4p.qp_new());

    /*_p34.SUCO*/ wire SUCO_PAL_PIPE_RST0 = nand2(LESY_SPRITE_MASK0p, SYPY_PAL_PIPE_DB4n);
    /*_p34.TAFA*/ wire TAFA_PAL_PIPE_RST1 = nand2(LOTA_SPRITE_MASK1p, TOTU_PAL_PIPE_DB4n);
    /*_p34.PYZY*/ wire PYZY_PAL_PIPE_RST2 = nand2(LYKU_SPRITE_MASK2p, NARO_PAL_PIPE_DB4n);
    /*_p34.TOWA*/ wire TOWA_PAL_PIPE_RST3 = nand2(ROBY_SPRITE_MASK3p, WEXY_PAL_PIPE_DB4n);
    /*_p34.RUDU*/ wire RUDU_PAL_PIPE_RST4 = nand2(TYTA_SPRITE_MASK4p, RYZY_PAL_PIPE_DB4n);
    /*_p34.PAZO*/ wire PAZO_PAL_PIPE_RST5 = nand2(TYCO_SPRITE_MASK5p, RYFE_PAL_PIPE_DB4n);
    /*_p34.LOWA*/ wire LOWA_PAL_PIPE_RST6 = nand2(SOKA_SPRITE_MASK6p, LADY_PAL_PIPE_DB4n);
    /*_p34.LUNU*/ wire LUNU_PAL_PIPE_RST7 = nand2(XOVU_SPRITE_MASK7p, LAFY_PAL_PIPE_DB4n);

    /*_p34.LYME*/ reg.pal_pipe.LYME_PAL_PIPE_D7.dff22(SACU_CLKPIPE_new, LAMY_PAL_PIPE_SET7, LUNU_PAL_PIPE_RST7, reg.pal_pipe.MODA_PAL_PIPE_D6.qp_old());
    /*_p34.MODA*/ reg.pal_pipe.MODA_PAL_PIPE_D6.dff22(SACU_CLKPIPE_new, LUKE_PAL_PIPE_SET6, LOWA_PAL_PIPE_RST6, reg.pal_pipe.NUKE_PAL_PIPE_D5.qp_old());
    /*_p34.NUKE*/ reg.pal_pipe.NUKE_PAL_PIPE_D5.dff22(SACU_CLKPIPE_new, MENE_PAL_PIPE_SET5, PAZO_PAL_PIPE_RST5, reg.pal_pipe.PALU_PAL_PIPE_D4.qp_old());
    /*_p34.PALU*/ reg.pal_pipe.PALU_PAL_PIPE_D4.dff22(SACU_CLKPIPE_new, RORA_PAL_PIPE_SET4, RUDU_PAL_PIPE_RST4, reg.pal_pipe.SOMY_PAL_PIPE_D3.qp_old());
    /*_p34.SOMY*/ reg.pal_pipe.SOMY_PAL_PIPE_D3.dff22(SACU_CLKPIPE_new, SUKY_PAL_PIPE_SET3, TOWA_PAL_PIPE_RST3, reg.pal_pipe.ROSA_PAL_PIPE_D2.qp_old());
    /*_p34.ROSA*/ reg.pal_pipe.ROSA_PAL_PIPE_D2.dff22(SACU_CLKPIPE_new, PAMO_PAL_PIPE_SET2, PYZY_PAL_PIPE_RST2, reg.pal_pipe.SATA_PAL_PIPE_D1.qp_old());
    /*_p34.SATA*/ reg.pal_pipe.SATA_PAL_PIPE_D1.dff22(SACU_CLKPIPE_new, SORO_PAL_PIPE_SET1, TAFA_PAL_PIPE_RST1, reg.pal_pipe.RUGO_PAL_PIPE_D0.qp_old());
    /*_p34.RUGO*/ reg.pal_pipe.RUGO_PAL_PIPE_D0.dff22(SACU_CLKPIPE_new, PUME_PAL_PIPE_SET0, SUCO_PAL_PIPE_RST0, reg.SIG_GND.out_new());
  }

  //----------------------------------------
  // Pipe merge

  /*#p35.RAJY*/ wire RAJY_PIX_BG_LOp = and2(reg.bgw_pipe_a.PYBO_BGW_PIPE_A7.qp_new(), reg.reg_lcdc.VYXE_LCDC_BGENn.qn_new());
  /*#p35.TADE*/ wire TADE_PIX_BG_HIp = and2(reg.bgw_pipe_b.SOHU_BGW_PIPE_B7.qp_new(), reg.reg_lcdc.VYXE_LCDC_BGENn.qn_new());
  /*#p35.XULA*/ wire XULA_PIX_SP_LOp = and2(reg.reg_lcdc.XYLO_LCDC_SPENn.qn_new(), reg.spr_pipe_a.WUFY_SPR_PIPE_A7.qp_new());
  /*#p35.WOXA*/ wire WOXA_PIX_SP_HIp = and2(reg.reg_lcdc.XYLO_LCDC_SPENn.qn_new(), reg.spr_pipe_b.VUPY_SPR_PIPE_B7.qp_new());

  /*#p35.NULY*/ wire NULY_PIX_SP_MASKn = nor2(WOXA_PIX_SP_HIp, XULA_PIX_SP_LOp);

  /*#p35.RYFU*/ wire RYFU_MASK_BG0 = and2(RAJY_PIX_BG_LOp, reg.mask_pipe.VAVA_MASK_PIPE_7.qp_new());
  /*#p35.RUTA*/ wire RUTA_MASK_BG1 = and2(TADE_PIX_BG_HIp, reg.mask_pipe.VAVA_MASK_PIPE_7.qp_new());
  /*#p35.POKA*/ wire POKA_BGPIXELn = nor3(NULY_PIX_SP_MASKn, RUTA_MASK_BG1, RYFU_MASK_BG0);

  /*#p34.LOME*/ wire LOME_PAL_PIPE_7n = not1(reg.pal_pipe.LYME_PAL_PIPE_D7.qp_new());
  /*#p34.LAFU*/ wire LAFU_OBP0PIXELn = nand2(LOME_PAL_PIPE_7n, POKA_BGPIXELn);
  /*#p34.LEKA*/ wire LEKA_OBP1PIXELn = nand2(reg.pal_pipe.LYME_PAL_PIPE_D7.qp_new(), POKA_BGPIXELn);

  //----------------------------------------
  // Pal reg read/write

  /*_p36.VELY*/ wire VELY_FF47_WRp = and2(CUPA_CPU_WRp(), reg.cpu_abus_new.WERA_FF47p());
  /*_p36.TEPO*/ wire TEPO_FF47_WRp = not1(VELY_FF47_WRp);
  /*#p36.PAVO*/ reg.reg_bgp.PAVO_BGP_D0n.dff8p(TEPO_FF47_WRp, reg.cpu_dbus_old.BUS_CPU_D00p.out_old());
  /*_p36.NUSY*/ reg.reg_bgp.NUSY_BGP_D1n.dff8p(TEPO_FF47_WRp, reg.cpu_dbus_old.BUS_CPU_D01p.out_old());
  /*_p36.PYLU*/ reg.reg_bgp.PYLU_BGP_D2n.dff8p(TEPO_FF47_WRp, reg.cpu_dbus_old.BUS_CPU_D02p.out_old());
  /*_p36.MAXY*/ reg.reg_bgp.MAXY_BGP_D3n.dff8p(TEPO_FF47_WRp, reg.cpu_dbus_old.BUS_CPU_D03p.out_old());
  /*_p36.MUKE*/ reg.reg_bgp.MUKE_BGP_D4n.dff8p(TEPO_FF47_WRp, reg.cpu_dbus_old.BUS_CPU_D04p.out_old());
  /*_p36.MORU*/ reg.reg_bgp.MORU_BGP_D5n.dff8p(TEPO_FF47_WRp, reg.cpu_dbus_old.BUS_CPU_D05p.out_old());
  /*_p36.MOGY*/ reg.reg_bgp.MOGY_BGP_D6n.dff8p(TEPO_FF47_WRp, reg.cpu_dbus_old.BUS_CPU_D06p.out_old());
  /*_p36.MENA*/ reg.reg_bgp.MENA_BGP_D7n.dff8p(TEPO_FF47_WRp, reg.cpu_dbus_old.BUS_CPU_D07p.out_old());

  /*_p36.XOMA*/ wire XOMA_FF48_WRp = and2(CUPA_CPU_WRp(), reg.cpu_abus_new.XAYO_FF48p());
  /*_p36.XELO*/ wire XELO_FF48_WRn = not1(XOMA_FF48_WRp);
  /*_p36.XUFU*/ reg.reg_obp0.XUFU_OBP0_D0n.dff8p(XELO_FF48_WRn, reg.cpu_dbus_old.BUS_CPU_D00p.out_old());
  /*_p36.XUKY*/ reg.reg_obp0.XUKY_OBP0_D1n.dff8p(XELO_FF48_WRn, reg.cpu_dbus_old.BUS_CPU_D01p.out_old());
  /*_p36.XOVA*/ reg.reg_obp0.XOVA_OBP0_D2n.dff8p(XELO_FF48_WRn, reg.cpu_dbus_old.BUS_CPU_D02p.out_old());
  /*_p36.XALO*/ reg.reg_obp0.XALO_OBP0_D3n.dff8p(XELO_FF48_WRn, reg.cpu_dbus_old.BUS_CPU_D03p.out_old());
  /*_p36.XERU*/ reg.reg_obp0.XERU_OBP0_D4n.dff8p(XELO_FF48_WRn, reg.cpu_dbus_old.BUS_CPU_D04p.out_old());
  /*_p36.XYZE*/ reg.reg_obp0.XYZE_OBP0_D5n.dff8p(XELO_FF48_WRn, reg.cpu_dbus_old.BUS_CPU_D05p.out_old());
  /*_p36.XUPO*/ reg.reg_obp0.XUPO_OBP0_D6n.dff8p(XELO_FF48_WRn, reg.cpu_dbus_old.BUS_CPU_D06p.out_old());
  /*_p36.XANA*/ reg.reg_obp0.XANA_OBP0_D7n.dff8p(XELO_FF48_WRn, reg.cpu_dbus_old.BUS_CPU_D07p.out_old());

  /*_p36.MYXE*/ wire MYXE_FF49_WRp = and2(CUPA_CPU_WRp(), reg.cpu_abus_new.TEGO_FF49p());
  /*_p36.LEHO*/ wire LEHO_FF49_WRn = not1(MYXE_FF49_WRp);
  /*_p36.MOXY*/ reg.reg_obp1.MOXY_OBP1_D0n.dff8p(LEHO_FF49_WRn, reg.cpu_dbus_old.BUS_CPU_D00p.out_old());
  /*_p36.LAWO*/ reg.reg_obp1.LAWO_OBP1_D1n.dff8p(LEHO_FF49_WRn, reg.cpu_dbus_old.BUS_CPU_D01p.out_old());
  /*_p36.MOSA*/ reg.reg_obp1.MOSA_OBP1_D2n.dff8p(LEHO_FF49_WRn, reg.cpu_dbus_old.BUS_CPU_D02p.out_old());
  /*_p36.LOSE*/ reg.reg_obp1.LOSE_OBP1_D3n.dff8p(LEHO_FF49_WRn, reg.cpu_dbus_old.BUS_CPU_D03p.out_old());
  /*_p36.LUNE*/ reg.reg_obp1.LUNE_OBP1_D4n.dff8p(LEHO_FF49_WRn, reg.cpu_dbus_old.BUS_CPU_D04p.out_old());
  /*_p36.LUGU*/ reg.reg_obp1.LUGU_OBP1_D5n.dff8p(LEHO_FF49_WRn, reg.cpu_dbus_old.BUS_CPU_D05p.out_old());
  /*_p36.LEPU*/ reg.reg_obp1.LEPU_OBP1_D6n.dff8p(LEHO_FF49_WRn, reg.cpu_dbus_old.BUS_CPU_D06p.out_old());
  /*_p36.LUXO*/ reg.reg_obp1.LUXO_OBP1_D7n.dff8p(LEHO_FF49_WRn, reg.cpu_dbus_old.BUS_CPU_D07p.out_old());

  /*_p36.VUSO*/ wire VUSO_FF47_RDp = and2(ASOT_CPU_RDp(), reg.cpu_abus_new.WERA_FF47p());
  /*_p36.TEPY*/ wire TEPY_FF47_RDn = not1(VUSO_FF47_RDp);
  /*#p36.RARO*/ triwire RARO_BGP0_TO_CD0 = tri6_nn(TEPY_FF47_RDn, reg.reg_bgp.PAVO_BGP_D0n.qp_new());
  /*_p36.PABA*/ triwire PABA_BGP1_TO_CD1 = tri6_nn(TEPY_FF47_RDn, reg.reg_bgp.NUSY_BGP_D1n.qp_new());
  /*_p36.REDO*/ triwire REDO_BGP2_TO_CD2 = tri6_nn(TEPY_FF47_RDn, reg.reg_bgp.PYLU_BGP_D2n.qp_new());
  /*_p36.LOBE*/ triwire LOBE_BGP3_TO_CD3 = tri6_nn(TEPY_FF47_RDn, reg.reg_bgp.MAXY_BGP_D3n.qp_new());
  /*_p36.LACE*/ triwire LACE_BGP4_TO_CD4 = tri6_nn(TEPY_FF47_RDn, reg.reg_bgp.MUKE_BGP_D4n.qp_new());
  /*_p36.LYKA*/ triwire LYKA_BGP5_TO_CD5 = tri6_nn(TEPY_FF47_RDn, reg.reg_bgp.MORU_BGP_D5n.qp_new());
  /*_p36.LODY*/ triwire LODY_BGP6_TO_CD6 = tri6_nn(TEPY_FF47_RDn, reg.reg_bgp.MOGY_BGP_D6n.qp_new());
  /*_p36.LARY*/ triwire LARY_BGP7_TO_CD7 = tri6_nn(TEPY_FF47_RDn, reg.reg_bgp.MENA_BGP_D7n.qp_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus_new.BUS_CPU_D00p.tri_bus(RARO_BGP0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus_new.BUS_CPU_D01p.tri_bus(PABA_BGP1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus_new.BUS_CPU_D02p.tri_bus(REDO_BGP2_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus_new.BUS_CPU_D03p.tri_bus(LOBE_BGP3_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus_new.BUS_CPU_D04p.tri_bus(LACE_BGP4_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus_new.BUS_CPU_D05p.tri_bus(LYKA_BGP5_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus_new.BUS_CPU_D06p.tri_bus(LODY_BGP6_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus_new.BUS_CPU_D07p.tri_bus(LARY_BGP7_TO_CD7);

  /*_p36.XUFY*/ wire XUFY_FF48_RDp = and2(ASOT_CPU_RDp(), reg.cpu_abus_new.XAYO_FF48p());
  /*_p36.XOZY*/ wire XOZY_FF48_RDn = not1(XUFY_FF48_RDp);
  /*#p36.XARY*/ triwire XARY_OBP00_TO_CD0 = tri6_nn(XOZY_FF48_RDn, reg.reg_obp0.XUFU_OBP0_D0n.qp_new());
  /*_p36.XOKE*/ triwire XOKE_OBP01_TO_CD1 = tri6_nn(XOZY_FF48_RDn, reg.reg_obp0.XUKY_OBP0_D1n.qp_new());
  /*_p36.XUNO*/ triwire XUNO_OBP02_TO_CD2 = tri6_nn(XOZY_FF48_RDn, reg.reg_obp0.XOVA_OBP0_D2n.qp_new());
  /*_p36.XUBY*/ triwire XUBY_OBP03_TO_CD3 = tri6_nn(XOZY_FF48_RDn, reg.reg_obp0.XALO_OBP0_D3n.qp_new());
  /*_p36.XAJU*/ triwire XAJU_OBP04_TO_CD4 = tri6_nn(XOZY_FF48_RDn, reg.reg_obp0.XERU_OBP0_D4n.qp_new());
  /*_p36.XOBO*/ triwire XOBO_OBP05_TO_CD5 = tri6_nn(XOZY_FF48_RDn, reg.reg_obp0.XYZE_OBP0_D5n.qp_new());
  /*_p36.XAXA*/ triwire XAXA_OBP06_TO_CD6 = tri6_nn(XOZY_FF48_RDn, reg.reg_obp0.XUPO_OBP0_D6n.qp_new());
  /*_p36.XAWO*/ triwire XAWO_OBP07_TO_CD7 = tri6_nn(XOZY_FF48_RDn, reg.reg_obp0.XANA_OBP0_D7n.qp_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus_new.BUS_CPU_D00p.tri_bus(XARY_OBP00_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus_new.BUS_CPU_D01p.tri_bus(XOKE_OBP01_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus_new.BUS_CPU_D02p.tri_bus(XUNO_OBP02_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus_new.BUS_CPU_D03p.tri_bus(XUBY_OBP03_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus_new.BUS_CPU_D04p.tri_bus(XAJU_OBP04_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus_new.BUS_CPU_D05p.tri_bus(XOBO_OBP05_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus_new.BUS_CPU_D06p.tri_bus(XAXA_OBP06_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus_new.BUS_CPU_D07p.tri_bus(XAWO_OBP07_TO_CD7);

  /*_p36.MUMY*/ wire MUMY_FF49_RDp = and2(ASOT_CPU_RDp(), reg.cpu_abus_new.TEGO_FF49p());
  /*_p36.LOTE*/ wire LOTE_FF49_RDn = not1(MUMY_FF49_RDp);
  /*#p36.LAJU*/ triwire LAJU_OBP10_TO_CD0 = tri6_nn(LOTE_FF49_RDn, reg.reg_obp1.MOXY_OBP1_D0n.qp_new());
  /*_p36.LEPA*/ triwire LEPA_OBP11_TO_CD1 = tri6_nn(LOTE_FF49_RDn, reg.reg_obp1.LAWO_OBP1_D1n.qp_new());
  /*_p36.LODE*/ triwire LODE_OBP12_TO_CD2 = tri6_nn(LOTE_FF49_RDn, reg.reg_obp1.MOSA_OBP1_D2n.qp_new());
  /*_p36.LYZA*/ triwire LYZA_OBP13_TO_CD3 = tri6_nn(LOTE_FF49_RDn, reg.reg_obp1.LOSE_OBP1_D3n.qp_new());
  /*_p36.LUKY*/ triwire LUKY_OBP14_TO_CD4 = tri6_nn(LOTE_FF49_RDn, reg.reg_obp1.LUNE_OBP1_D4n.qp_new());
  /*_p36.LUGA*/ triwire LUGA_OBP15_TO_CD5 = tri6_nn(LOTE_FF49_RDn, reg.reg_obp1.LUGU_OBP1_D5n.qp_new());
  /*_p36.LEBA*/ triwire LEBA_OBP16_TO_CD6 = tri6_nn(LOTE_FF49_RDn, reg.reg_obp1.LEPU_OBP1_D6n.qp_new());
  /*_p36.LELU*/ triwire LELU_OBP17_TO_CD7 = tri6_nn(LOTE_FF49_RDn, reg.reg_obp1.LUXO_OBP1_D7n.qp_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus_new.BUS_CPU_D00p.tri_bus(LAJU_OBP10_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus_new.BUS_CPU_D01p.tri_bus(LEPA_OBP11_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus_new.BUS_CPU_D02p.tri_bus(LODE_OBP12_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus_new.BUS_CPU_D03p.tri_bus(LYZA_OBP13_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus_new.BUS_CPU_D04p.tri_bus(LUKY_OBP14_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus_new.BUS_CPU_D05p.tri_bus(LUGA_OBP15_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus_new.BUS_CPU_D06p.tri_bus(LEBA_OBP16_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus_new.BUS_CPU_D07p.tri_bus(LELU_OBP17_TO_CD7);

  //----------------------------------------
  // Sprite palette 0 lookup

  /*#p35.WELE*/ wire WELE_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
  /*#p35.WOLO*/ wire WOLO_PIX_SP_LOp = not1(WELE_PIX_SP_LOn);
  /*#p35.VUMU*/ wire VUMU_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
  /*#p35.WYRU*/ wire WYRU_PIX_SP_HIp = not1(VUMU_PIX_SP_HIn);

  /*#p35.LAVA*/ wire LAVA_MASK_OPB0 = not1(LAFU_OBP0PIXELn);

  /*#p35.VUGO*/ wire VUGO_PAL_OBP0A = and3(VUMU_PIX_SP_HIn, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
  /*#p35.VOLO*/ wire VOLO_PAL_OBP0B = and3(VUMU_PIX_SP_HIn, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm
  /*#p35.VATA*/ wire VATA_PAL_OBP0C = and3(WYRU_PIX_SP_HIp, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
  /*#p35.VYRO*/ wire VYRO_PAL_OBP0D = and3(WYRU_PIX_SP_HIp, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm

  /*#p35.WUFU*/ wire WUFU_COL_OBP0_HI = amux4(reg.reg_obp0.XANA_OBP0_D7n.qp_any(), VYRO_PAL_OBP0D, reg.reg_obp0.XYZE_OBP0_D5n.qp_any(), VATA_PAL_OBP0C, reg.reg_obp0.XALO_OBP0_D3n.qp_any(), VOLO_PAL_OBP0B, reg.reg_obp0.XUKY_OBP0_D1n.qp_any(), VUGO_PAL_OBP0A);
  /*#p35.WALY*/ wire WALY_COL_OBP0_LO = amux4(reg.reg_obp0.XUPO_OBP0_D6n.qp_any(), VYRO_PAL_OBP0D, reg.reg_obp0.XERU_OBP0_D4n.qp_any(), VATA_PAL_OBP0C, reg.reg_obp0.XOVA_OBP0_D2n.qp_any(), VOLO_PAL_OBP0B, reg.reg_obp0.XUFU_OBP0_D0n.qp_any(), VUGO_PAL_OBP0A);

  //----------------------------------------
  // Sprite palette 1 lookup

  /*#p35.MABY*/ wire MABY_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
  /*#p35.LYLE*/ wire LYLE_PIX_SP_LOp = not1(MABY_PIX_SP_LOn);
  /*#p35.MEXA*/ wire MEXA_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
  /*#p35.LOZO*/ wire LOZO_PIX_SP_HIp = not1(MEXA_PIX_SP_HIn);

  /*#p35.LUKU*/ wire LUKU_MASK_OBP1 = not1(LEKA_OBP1PIXELn);

  /*#p35.LOPU*/ wire LOPU_PAL_OBP1A = and3(MEXA_PIX_SP_HIn, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
  /*#p35.LYKY*/ wire LYKY_PAL_OBP1B = and3(MEXA_PIX_SP_HIn, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm
  /*#p35.LARU*/ wire LARU_PAL_OBP1C = and3(LOZO_PIX_SP_HIp, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
  /*#p35.LEDO*/ wire LEDO_PAL_OBP1D = and3(LOZO_PIX_SP_HIp, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm

  /*#p35.MOKA*/ wire MOKA_COL_OBP1_HI = amux4(reg.reg_obp1.LUXO_OBP1_D7n.qp_any(), LEDO_PAL_OBP1D, reg.reg_obp1.LUGU_OBP1_D5n.qp_any(), LARU_PAL_OBP1C, reg.reg_obp1.LOSE_OBP1_D3n.qp_any(), LYKY_PAL_OBP1B, reg.reg_obp1.LAWO_OBP1_D1n.qp_any(), LOPU_PAL_OBP1A);
  /*#p35.MUFA*/ wire MUFA_COL_OBP1_LO = amux4(LEDO_PAL_OBP1D, reg.reg_obp1.LEPU_OBP1_D6n.qp_any(), LARU_PAL_OBP1C, reg.reg_obp1.LUNE_OBP1_D4n.qp_any(), LYKY_PAL_OBP1B, reg.reg_obp1.MOSA_OBP1_D2n.qp_any(), LOPU_PAL_OBP1A, reg.reg_obp1.MOXY_OBP1_D0n.qp_any());

  //----------------------------------------
  // Background/window palette lookup

  /*_p35.SOBA*/ wire SOBA_PIX_BG_LOn = not1(RAJY_PIX_BG_LOp);
  /*_p35.NUPO*/ wire NUPO_PIX_BG_LOp = not1(SOBA_PIX_BG_LOn);
  /*_p35.VYCO*/ wire VYCO_PIX_BG_HIn = not1(TADE_PIX_BG_HIp);
  /*_p35.NALE*/ wire NALE_PIX_BG_HIp = not1(VYCO_PIX_BG_HIn);

  /*_p35.MUVE*/ wire MUVE_MASK_BGP = not1(POKA_BGPIXELn);

  /*_p35.POBU*/ wire POBU_PAL_BGPA = and3(VYCO_PIX_BG_HIn, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
  /*_p35.NUXO*/ wire NUXO_PAL_BGPB = and3(VYCO_PIX_BG_HIn, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm
  /*_p35.NUMA*/ wire NUMA_PAL_BGPC = and3(NALE_PIX_BG_HIp, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
  /*_p35.NYPO*/ wire NYPO_PAL_BGPD = and3(NALE_PIX_BG_HIp, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm

  /*#p35.NELO*/ wire NELO_COL_BG_LO = amux4(NYPO_PAL_BGPD, reg.reg_bgp.MOGY_BGP_D6n.qp_any(), NUMA_PAL_BGPC, reg.reg_bgp.MUKE_BGP_D4n.qp_any(), NUXO_PAL_BGPB, reg.reg_bgp.PYLU_BGP_D2n.qp_any(), POBU_PAL_BGPA, reg.reg_bgp.PAVO_BGP_D0n.qp_any());
  /*#p35.NURA*/ wire NURA_COL_BG_HI = amux4(reg.reg_bgp.MENA_BGP_D7n.qp_any(), NYPO_PAL_BGPD, reg.reg_bgp.MORU_BGP_D5n.qp_any(), NUMA_PAL_BGPC, reg.reg_bgp.MAXY_BGP_D3n.qp_any(), NUXO_PAL_BGPB, reg.reg_bgp.NUSY_BGP_D1n.qp_any(), POBU_PAL_BGPA);

  //----------------------------------------
  // Pixel merge and send

  /*#p35.PERO*/ wire PERO_COL_LO = or3(NELO_COL_BG_LO, WALY_COL_OBP0_LO, MUFA_COL_OBP1_LO);
  /*#p35.PATY*/ wire PATY_COL_HI = or3(NURA_COL_BG_HI, WUFU_COL_OBP0_HI, MOKA_COL_OBP1_HI);

  /*#p35.REMY*/ reg.REMY_LD0n = not1(PERO_COL_LO);
  /*#p35.RAVO*/ reg.RAVO_LD1n = not1(PATY_COL_HI);
}
