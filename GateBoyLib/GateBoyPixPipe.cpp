#include "GateBoyLib/GateBoyPixPipe.h"
#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyCpuBus.h"

//------------------------------------------------------------------------------------------------------------------------

void RegWY::tock(GateBoyCpuBus& cpu_bus, wire AVOR_SYS_RSTp, wire TEDO_CPU_RDp, wire TAPU_CPU_WRp) {
  // FF4A WY
  /* p23.WEKO*/ wire _WEKO_FF4A_WRp_clk_evn = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), cpu_bus.VYGA_FF4Ap());
  /* p23.VEFU*/ wire _VEFU_FF4A_WRn_clk_evn = not1(_WEKO_FF4A_WRp_clk_evn);
  /* p23.NESO*/ NESO_WY0n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[0]);
  /* p23.NYRO*/ NYRO_WY1n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[1]);
  /* p23.NAGA*/ NAGA_WY2n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[2]);
  /* p23.MELA*/ MELA_WY3n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[3]);
  /* p23.NULO*/ NULO_WY4n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[4]);
  /* p23.NENE*/ NENE_WY5n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[5]);
  /* p23.NUKA*/ NUKA_WY6n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[6]);
  /* p23.NAFU*/ NAFU_WY7n_h.dff9(_VEFU_FF4A_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[7]);

  // FF4A WY
  /* p23.WAXU*/ wire _WAXU_FF4A_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), cpu_bus.VYGA_FF4Ap());
  /* p23.VOMY*/ wire _VOMY_FF4A_RDn_ext = not1(_WAXU_FF4A_RDp_ext);
  /*#p23.PUNU*/ cpu_bus.BUS_CPU_D_out[0].tri6_nn(_VOMY_FF4A_RDn_ext, NESO_WY0n_h.qp_new());
  /* p23.PODA*/ cpu_bus.BUS_CPU_D_out[1].tri6_nn(_VOMY_FF4A_RDn_ext, NYRO_WY1n_h.qp_new());
  /* p23.PYGU*/ cpu_bus.BUS_CPU_D_out[2].tri6_nn(_VOMY_FF4A_RDn_ext, NAGA_WY2n_h.qp_new());
  /* p23.LOKA*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_VOMY_FF4A_RDn_ext, MELA_WY3n_h.qp_new());
  /* p23.MEGA*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_VOMY_FF4A_RDn_ext, NULO_WY4n_h.qp_new());
  /* p23.PELA*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_VOMY_FF4A_RDn_ext, NENE_WY5n_h.qp_new());
  /* p23.POLO*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_VOMY_FF4A_RDn_ext, NUKA_WY6n_h.qp_new());
  /* p23.MERA*/ cpu_bus.BUS_CPU_D_out[7].tri6_nn(_VOMY_FF4A_RDn_ext, NAFU_WY7n_h.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void RegWX::tock(GateBoyCpuBus& cpu_bus, wire AVOR_SYS_RSTp, wire TEDO_CPU_RDp, wire TAPU_CPU_WRp) {
  // FF4B WX
  /* p23.WUZA*/ wire _WUZA_FF4B_WRp_clk_evn = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), cpu_bus.VUMY_FF4Bp());
  /* p23.VOXU*/ wire _VOXU_FF4B_WRn_clk_evn = not1(_WUZA_FF4B_WRp_clk_evn);
  /* p23.MYPA*/ MYPA_WX0n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[0]);
  /* p23.NOFE*/ NOFE_WX1n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[1]);
  /* p23.NOKE*/ NOKE_WX2n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[2]);
  /* p23.MEBY*/ MEBY_WX3n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[3]);
  /* p23.MYPU*/ MYPU_WX4n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[4]);
  /* p23.MYCE*/ MYCE_WX5n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[5]);
  /* p23.MUVO*/ MUVO_WX6n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[6]);
  /* p23.NUKU*/ NUKU_WX7n_h.dff9(_VOXU_FF4B_WRn_clk_evn, WALU_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[7]);

  // FF4B WX
  /* p23.WYZE*/ wire _WYZE_FF4B_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), cpu_bus.VUMY_FF4Bp());
  /* p23.VYCU*/ wire _VYCU_FF4B_RDn_ext = not1(_WYZE_FF4B_RDp_ext);
  /*#p23.LOVA*/ cpu_bus.BUS_CPU_D_out[0].tri6_nn(_VYCU_FF4B_RDn_ext, MYPA_WX0n_h.qp_new());
  /* p23.MUKA*/ cpu_bus.BUS_CPU_D_out[1].tri6_nn(_VYCU_FF4B_RDn_ext, NOFE_WX1n_h.qp_new());
  /* p23.MOKO*/ cpu_bus.BUS_CPU_D_out[2].tri6_nn(_VYCU_FF4B_RDn_ext, NOKE_WX2n_h.qp_new());
  /* p23.LOLE*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_VYCU_FF4B_RDn_ext, MEBY_WX3n_h.qp_new());
  /* p23.MELE*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_VYCU_FF4B_RDn_ext, MYPU_WX4n_h.qp_new());
  /* p23.MUFE*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_VYCU_FF4B_RDn_ext, MYCE_WX5n_h.qp_new());
  /* p23.MULY*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_VYCU_FF4B_RDn_ext, MUVO_WX6n_h.qp_new());
  /* p23.MARA*/ cpu_bus.BUS_CPU_D_out[7].tri6_nn(_VYCU_FF4B_RDn_ext, NUKU_WX7n_h.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void RegStat::tock(
  GateBoyCpuBus& cpu_bus,
  wire AVOR_SYS_RSTp,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,

  wire ACYL_SCANNINGp,
  wire XYMU_RENDERINGp,
  wire PARU_VBLANKp,
  const RegLYC& reg_lyc)
{
  // FF41 STAT
  /* p21.SEPA*/ wire _SEPA_FF41_WRp_clk = and2(CUPA_CPU_WRp(TAPU_CPU_WRp), cpu_bus.VARY_FF41p());
  /* p21.RYVE*/ wire _RYVE_FF41_WRn_clk = not1(_SEPA_FF41_WRp_clk);
  /* p21.ROXE*/ ROXE_STAT_HBI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[3]);
  /* p21.RUFO*/ RUFO_STAT_VBI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[4]);
  /* p21.REFE*/ REFE_STAT_OAI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[5]);
  /* p21.RUGU*/ RUGU_STAT_LYI_ENn_h.dff9(_RYVE_FF41_WRn_clk, WESY_SYS_RSTn(AVOR_SYS_RSTp), cpu_bus.BUS_CPU_D[6]);

  /* p21.RYJU*/ wire _RYJU_FF41_WRn_clk_evn = not1(_SEPA_FF41_WRp_clk);
  /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST_new_evn = or2(WESY_SYS_RSTn(AVOR_SYS_RSTp), _RYJU_FF41_WRn_clk_evn);
  /* p21.RUPO*/ RUPO_STAT_LYC_MATCHn.nor_latch(_PAGO_LYC_MATCH_RST_new_evn, reg_lyc.ROPO_LY_MATCH_SYNCp.qp_new());

  // FF41 STAT
  /* p21.SADU*/ wire _SADU_STAT_MODE0n_new  = nor2(XYMU_RENDERINGp, PARU_VBLANKp); // die NOR
  /* p21.XATY*/ wire _XATY_STAT_MODE1n_new  = nor2(ACYL_SCANNINGp, XYMU_RENDERINGp); // die NOR

  /* p21.TOBE*/ wire _TOBE_FF41_RDp_ext = and2(ASOT_CPU_RDp(TEDO_CPU_RDp), cpu_bus.VARY_FF41p());
  /* p21.VAVE*/ wire _VAVE_FF41_RDn_ext = not1(_TOBE_FF41_RDp_ext);
  /*#p21.TEBY*/ cpu_bus.BUS_CPU_D_out[0].tri6_pn(_TOBE_FF41_RDp_ext, _SADU_STAT_MODE0n_new);
  /*#p21.WUGA*/ cpu_bus.BUS_CPU_D_out[1].tri6_pn(_TOBE_FF41_RDp_ext, _XATY_STAT_MODE1n_new);
  /*#p21.SEGO*/ cpu_bus.BUS_CPU_D_out[2].tri6_pn(_TOBE_FF41_RDp_ext, RUPO_STAT_LYC_MATCHn.qp_new());
  /* p21.PUZO*/ cpu_bus.BUS_CPU_D_out[3].tri6_nn(_VAVE_FF41_RDn_ext, ROXE_STAT_HBI_ENn_h.qp_new());
  /* p21.POFO*/ cpu_bus.BUS_CPU_D_out[4].tri6_nn(_VAVE_FF41_RDn_ext, RUFO_STAT_VBI_ENn_h.qp_new());
  /* p21.SASY*/ cpu_bus.BUS_CPU_D_out[5].tri6_nn(_VAVE_FF41_RDn_ext, REFE_STAT_OAI_ENn_h.qp_new());
  /* p21.POTE*/ cpu_bus.BUS_CPU_D_out[6].tri6_nn(_VAVE_FF41_RDn_ext, RUGU_STAT_LYI_ENn_h.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void WindowRegisters::tock(
  wire XODO_VID_RSTp,
  wire ALET_xBxDxFxH,
  wire TYFA_CLKPIPE_odd,
  wire XAPO_VID_RSTn,
  wire NUKO_WX_MATCHp,
  wire XYMU_RENDERINGp,
  wire ROZE_FINE_COUNT_7n)
{
  /*#p24.SEGU*/ wire _SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /* p27.ROCO*/ wire _ROCO_CLKPIPE_odd = not1(_SEGU_CLKPIPE_evn);
  /* p27.PYCO*/ PYCO_WIN_MATCHp_odd.dff17(_ROCO_CLKPIPE_odd, XAPO_VID_RSTn, NUKO_WX_MATCHp);

  /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp);
  /* p27.SOVY*/ SOVY_WIN_HITp.dff17(ALET_xBxDxFxH, _XAPO_VID_RSTn_new_evn, RYDY_WIN_HITp_evn.qp_old());

  /* p27.PANY*/ wire _PANY_WIN_FETCHn_old = nor2(NUKO_WX_MATCHp, ROZE_FINE_COUNT_7n);
  /* p27.RYFA*/ RYFA_WIN_FETCHn_A_evn.dff17(_SEGU_CLKPIPE_evn, XYMU_RENDERINGp, _PANY_WIN_FETCHn_old);
}

//------------------------------------------------------------------------------------------------------------------------

#if 0
  [
    this,

    XODO_VID_RSTp,
    &NUKO_WX_MATCHp_old_evn
  ](){

    /*#p27.MYLO*/ wire _MYLO_WX_MATCH0p_old_evn = xnor2(pix_count.XEHO_PX0p.qp_old(), reg_wx.MYPA_WX0n_h.qn_old());
    /* p27.PUWU*/ wire _PUWU_WX_MATCH1p_old_evn = xnor2(pix_count.SAVY_PX1p.qp_old(), reg_wx.NOFE_WX1n_h.qn_old());
    /* p27.PUHO*/ wire _PUHO_WX_MATCH2p_old_evn = xnor2(pix_count.XODU_PX2p.qp_old(), reg_wx.NOKE_WX2n_h.qn_old());
    /* p27.NYTU*/ wire _NYTU_WX_MATCH3p_old_evn = xnor2(pix_count.XYDO_PX3p.qp_old(), reg_wx.MEBY_WX3n_h.qn_old());
    /* p27.NEZO*/ wire _NEZO_WX_MATCH4p_old_evn = xnor2(pix_count.TUHU_PX4p.qp_old(), reg_wx.MYPU_WX4n_h.qn_old());
    /* p27.NORY*/ wire _NORY_WX_MATCH5p_old_evn = xnor2(pix_count.TUKY_PX5p.qp_old(), reg_wx.MYCE_WX5n_h.qn_old());
    /* p27.NONO*/ wire _NONO_WX_MATCH6p_old_evn = xnor2(pix_count.TAKO_PX6p.qp_old(), reg_wx.MUVO_WX6n_h.qn_old());
    /* p27.PASE*/ wire _PASE_WX_MATCH7p_old_evn = xnor2(pix_count.SYBE_PX7p.qp_old(), reg_wx.NUKU_WX7n_h.qn_old());

    /* p27.PUKY*/ wire _PUKY_WX_MATCHn_old_evn  = nand5(win_reg.REJO_WY_MATCH_LATCHp_evn.qp_old(), _NEZO_WX_MATCH4p_old_evn, _NORY_WX_MATCH5p_old_evn, _NONO_WX_MATCH6p_old_evn, _PASE_WX_MATCH7p_old_evn);
    /* p27.NUFA*/ wire _NUFA_WX_MATCHp_old_evn  = not1(_PUKY_WX_MATCHn_old_evn);
    /* p27.NOGY*/ wire _NOGY_WX_MATCHn_old_evn  = nand5(_NUFA_WX_MATCHp_old_evn, _MYLO_WX_MATCH0p_old_evn, _PUWU_WX_MATCH1p_old_evn, _PUHO_WX_MATCH2p_old_evn, _NYTU_WX_MATCH3p_old_evn);
    /* p27.NUKO*/ NUKO_WX_MATCHp_old_evn  = not1(_NOGY_WX_MATCHn_old_evn);

    /*#p27.NAZE*/ wire _NAZE_WY_MATCH0p_old_evn = xnor2(reg_wy.NESO_WY0n_h.qn_old(), reg_ly.MUWY_LY0p_evn.qp_old());
    /* p27.PEBO*/ wire _PEBO_WY_MATCH1p_old_evn = xnor2(reg_wy.NYRO_WY1n_h.qn_old(), reg_ly.MYRO_LY1p_evn.qp_old());
    /* p27.POMO*/ wire _POMO_WY_MATCH2p_old_evn = xnor2(reg_wy.NAGA_WY2n_h.qn_old(), reg_ly.LEXA_LY2p_evn.qp_old());
    /* p27.NEVU*/ wire _NEVU_WY_MATCH3p_old_evn = xnor2(reg_wy.MELA_WY3n_h.qn_old(), reg_ly.LYDO_LY3p_evn.qp_old());
    /* p27.NOJO*/ wire _NOJO_WY_MATCH4p_old_evn = xnor2(reg_wy.NULO_WY4n_h.qn_old(), reg_ly.LOVU_LY4p_evn.qp_old());
    /* p27.PAGA*/ wire _PAGA_WY_MATCH5p_old_evn = xnor2(reg_wy.NENE_WY5n_h.qn_old(), reg_ly.LEMA_LY5p_evn.qp_old());
    /* p27.PEZO*/ wire _PEZO_WY_MATCH6p_old_evn = xnor2(reg_wy.NUKA_WY6n_h.qn_old(), reg_ly.MATO_LY6p_evn.qp_old());
    /* p27.NUPA*/ wire _NUPA_WY_MATCH7p_old_evn = xnor2(reg_wy.NAFU_WY7n_h.qn_old(), reg_ly.LAFO_LY7p_evn.qp_old());

    /*#p27.PALO*/ wire _PALO_WY_MATCH_HIn_old_evn = nand5(reg_lcdc.WYMO_LCDC_WINENn_h.qn_old(), _NOJO_WY_MATCH4p_old_evn, _PAGA_WY_MATCH5p_old_evn, _PEZO_WY_MATCH6p_old_evn, _NUPA_WY_MATCH7p_old_evn);
    /* p27.NELE*/ wire _NELE_WY_MATCH_HIp_old_evn = not1(_PALO_WY_MATCH_HIn_old_evn);
    /* p27.PAFU*/ wire _PAFU_WY_MATCHn_old_evn    = nand5(_NELE_WY_MATCH_HIp_old_evn, _NAZE_WY_MATCH0p_old_evn, _PEBO_WY_MATCH1p_old_evn, _POMO_WY_MATCH2p_old_evn, _NEVU_WY_MATCH3p_old_evn);
    /* p27.ROGE*/ wire _ROGE_WY_MATCHp_old_evn    = not1(_PAFU_WY_MATCHn_old_evn);

    {
      /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp);
      /* p01.PYRY*/ wire _PYRY_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
      /* p27.SARY*/ win_reg.SARY_WY_MATCHp_evn.dff17(vclk.TALU_xxCDEFxx(), _XAPO_VID_RSTn_new_evn, _ROGE_WY_MATCHp_old_evn);
      /* p27.REPU*/ wire _REPU_VBLANKp_new_evn = or2(lcd.PARU_VBLANKp(), _PYRY_VID_RSTp_new_evn);
      /* p27.REJO*/ win_reg.REJO_WY_MATCH_LATCHp_evn.nor_latch(win_reg.SARY_WY_MATCHp_evn.qp_new(), _REPU_VBLANKp_new_evn);
    }
  }();
#endif