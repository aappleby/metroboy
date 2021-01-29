#include "GateBoyLib/GateBoyRegisters.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

SpriteDeltaY GateBoy::sub_sprite_y() {
  /*#p29.EBOS*/ wire EBOS_LY0n = not1(reg_ly.MUWY_LY0p.qp_new());
  /* p29.DASA*/ wire DASA_LY1n = not1(reg_ly.MYRO_LY1p.qp_new());
  /* p29.FUKY*/ wire FUKY_LY2n = not1(reg_ly.LEXA_LY2p.qp_new());
  /* p29.FUVE*/ wire FUVE_LY3n = not1(reg_ly.LYDO_LY3p.qp_new());
  /* p29.FEPU*/ wire FEPU_LY4n = not1(reg_ly.LOVU_LY4p.qp_new());
  /* p29.FOFA*/ wire FOFA_LY5n = not1(reg_ly.LEMA_LY5p.qp_new());
  /* p29.FEMO*/ wire FEMO_LY6n = not1(reg_ly.MATO_LY6p.qp_new());
  /* p29.GUSU*/ wire GUSU_LY7n = not1(reg_ly.LAFO_LY7p.qp_new());

  /* p29.ERUC*/ auto _ERUC_YDIFF0 = add3(EBOS_LY0n, oam_temp_a.XUSO_OAM_DA0p.qp_new(), SIG_GND.qp_new());
  /* p29.ENEF*/ auto _ENEF_YDIFF1 = add3(DASA_LY1n, oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF0.carry);
  /* p29.FECO*/ auto _FECO_YDIFF2 = add3(FUKY_LY2n, oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF1.carry);
  /* p29.GYKY*/ auto _GYKY_YDIFF3 = add3(FUVE_LY3n, oam_temp_a.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF2.carry);
  /* p29.GOPU*/ auto _GOPU_YDIFF4 = add3(FEPU_LY4n, oam_temp_a.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF3.carry);
  /* p29.FUWA*/ auto _FUWA_YDIFF5 = add3(FOFA_LY5n, oam_temp_a.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF4.carry);
  /* p29.GOJU*/ auto _GOJU_YDIFF6 = add3(FEMO_LY6n, oam_temp_a.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF5.carry);
  /* p29.WUHU*/ auto _WUHU_YDIFF7 = add3(GUSU_LY7n, oam_temp_a.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF6.carry);

  /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0_new = not1(_ERUC_YDIFF0.sum);
  /* p29.DABY*/ wire _DABY_SPRITE_DELTA1_new = not1(_ENEF_YDIFF1.sum);
  /* p29.DABU*/ wire _DABU_SPRITE_DELTA2_new = not1(_FECO_YDIFF2.sum);
  /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3_new = not1(_GYKY_YDIFF3.sum);
  /* p29.GACE*/ wire _GACE_SPRITE_DELTA4_new = not1(_GOPU_YDIFF4.sum);
  /* p29.GUVU*/ wire _GUVU_SPRITE_DELTA5_new = not1(_FUWA_YDIFF5.sum);
  /* p29.GYDA*/ wire _GYDA_SPRITE_DELTA6_new = not1(_GOJU_YDIFF6.sum);
  /* p29.GEWY*/ wire _GEWY_SPRITE_DELTA7_new = not1(_WUHU_YDIFF7.sum);

  return {
    _DEGE_SPRITE_DELTA0_new,
    _DABY_SPRITE_DELTA1_new,
    _DABU_SPRITE_DELTA2_new,
    _GYSA_SPRITE_DELTA3_new,
    _GACE_SPRITE_DELTA4_new,
    _GUVU_SPRITE_DELTA5_new,
    _GYDA_SPRITE_DELTA6_new,
    _GEWY_SPRITE_DELTA7_new,
    _GYKY_YDIFF3,
    _WUHU_YDIFF7
  };
}

//------------------------------------------------------------------------------------------------------------------------

SpritePix GateBoy::flip_sprite_pix(DFF8n BAXO_OAM_DB5p) {
  /*#p29.XONO*/ wire _XONO_FLIP_X = and2(BAXO_OAM_DB5p.qp_new(), TEXY_SFETCHINGp.qp_new());
  /* p33.PUTE*/ wire _PUTE_FLIP0p = mux2p(_XONO_FLIP_X, new_bus.BUS_VRAM_D07p.qp_new(), new_bus.BUS_VRAM_D00p.qp_new());
  /* p33.PELO*/ wire _PELO_FLIP1p = mux2p(_XONO_FLIP_X, new_bus.BUS_VRAM_D06p.qp_new(), new_bus.BUS_VRAM_D01p.qp_new());
  /* p33.PONO*/ wire _PONO_FLIP2p = mux2p(_XONO_FLIP_X, new_bus.BUS_VRAM_D05p.qp_new(), new_bus.BUS_VRAM_D02p.qp_new());
  /* p33.POBE*/ wire _POBE_FLIP3p = mux2p(_XONO_FLIP_X, new_bus.BUS_VRAM_D04p.qp_new(), new_bus.BUS_VRAM_D03p.qp_new());
  /* p33.PACY*/ wire _PACY_FLIP4p = mux2p(_XONO_FLIP_X, new_bus.BUS_VRAM_D03p.qp_new(), new_bus.BUS_VRAM_D04p.qp_new());
  /* p33.PUGU*/ wire _PUGU_FLIP5p = mux2p(_XONO_FLIP_X, new_bus.BUS_VRAM_D02p.qp_new(), new_bus.BUS_VRAM_D05p.qp_new());
  /* p33.PAWE*/ wire _PAWE_FLIP6p = mux2p(_XONO_FLIP_X, new_bus.BUS_VRAM_D01p.qp_new(), new_bus.BUS_VRAM_D06p.qp_new());
  /* p33.PULY*/ wire _PULY_FLIP7p = mux2p(_XONO_FLIP_X, new_bus.BUS_VRAM_D00p.qp_new(), new_bus.BUS_VRAM_D07p.qp_new());

  return {
    _PUTE_FLIP0p,
    _PELO_FLIP1p,
    _PONO_FLIP2p,
    _POBE_FLIP3p,
    _PACY_FLIP4p,
    _PUGU_FLIP5p,
    _PAWE_FLIP6p,
    _PULY_FLIP7p
  };
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_lcdc_read()
{
  /* p23.VYRE*/ wire _VYRE_FF40_RDp = and2(ASOT_CPU_RDp(), new_bus.VOCA_FF40p());
  /* p23.WYCE*/ wire _WYCE_FF40_RDn = not1(_VYRE_FF40_RDp);
  /*#p23.WYPO_LCDC0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_WYCE_FF40_RDn, reg_lcdc.VYXE_LCDC_BGENn.qp_new());
  /*#p23.XERO_LCDC1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_WYCE_FF40_RDn, reg_lcdc.XYLO_LCDC_SPENn.qp_new());
  /* p23.WYJU_LCDC2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_WYCE_FF40_RDn, reg_lcdc.XYMO_LCDC_SPSIZEn.qp_new());
  /* p23.WUKA_LCDC3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_WYCE_FF40_RDn, reg_lcdc.XAFO_LCDC_BGMAPn.qp_new());
  /* p23.VOKE_LCDC4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_WYCE_FF40_RDn, reg_lcdc.WEXU_LCDC_BGTILEn.qp_new());
  /* p23.VATO_LCDC5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_WYCE_FF40_RDn, reg_lcdc.WYMO_LCDC_WINENn.qp_new());
  /*#p23.VAHA_LCDC6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_WYCE_FF40_RDn, reg_lcdc.WOKY_LCDC_WINMAPn.qp_new());
  /*#p23.XEBU_LCDC7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_WYCE_FF40_RDn, reg_lcdc.XONA_LCDC_LCDENn.qp_new());
}

void GateBoy::reg_lcdc_write()
{
  /* p23.WARU*/ wire _WARU_FF40_WRp = and2(CUPA_CPU_WRp(), old_bus.VOCA_FF40p());
  /* p23.XUBO*/ wire _XUBO_FF40_WRn = not1(_WARU_FF40_WRp);
  /*#p23.VYXE*/ reg_lcdc.VYXE_LCDC_BGENn  .dff9(_XUBO_FF40_WRn, XARE_SYS_RSTn(), old_bus.BUS_CPU_D00p.qp_old());
  /* p23.XYLO*/ reg_lcdc.XYLO_LCDC_SPENn  .dff9(_XUBO_FF40_WRn, XARE_SYS_RSTn(), old_bus.BUS_CPU_D01p.qp_old());
  /* p23.XYMO*/ reg_lcdc.XYMO_LCDC_SPSIZEn.dff9(_XUBO_FF40_WRn, XARE_SYS_RSTn(), old_bus.BUS_CPU_D02p.qp_old());
  /* p23.XAFO*/ reg_lcdc.XAFO_LCDC_BGMAPn .dff9(_XUBO_FF40_WRn, XARE_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p23.WEXU*/ reg_lcdc.WEXU_LCDC_BGTILEn.dff9(_XUBO_FF40_WRn, XARE_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p23.WYMO*/ reg_lcdc.WYMO_LCDC_WINENn .dff9(_XUBO_FF40_WRn, XARE_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p23.WOKY*/ reg_lcdc.WOKY_LCDC_WINMAPn.dff9(_XUBO_FF40_WRn, XARE_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
  /* p23.XONA*/ reg_lcdc.XONA_LCDC_LCDENn. dff9(_XUBO_FF40_WRn, XARE_SYS_RSTn(), old_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_stat_read()
{
  /*#p21.PARU*/ wire PARU_VBLANKp = not1(lcd.POPU_VBLANKp.qn_new());
  /*#p21.SADU*/ wire _SADU_STAT_MODE0n = nor2(XYMU_RENDERINGn.qn_new(), PARU_VBLANKp);   // die NOR
  /*#p21.XATY*/ wire _XATY_STAT_MODE1n = nor2(ACYL_SCANNINGp.qp_new(), XYMU_RENDERINGn.qn_new()); // die NOR
  /* p21.TOBE*/ wire _TOBE_FF41_RDp = and2(ASOT_CPU_RDp(), new_bus.VARY_FF41p());
  /* p21.VAVE*/ wire _VAVE_FF41_RDn = not1(_TOBE_FF41_RDp);
  /*#p21.TEBY_STAT0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_TOBE_FF41_RDp, _SADU_STAT_MODE0n);
  /*#p21.WUGA_STAT1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_pn(_TOBE_FF41_RDp, _XATY_STAT_MODE1n);
  /*#p21.SEGO_STAT2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_pn(_TOBE_FF41_RDp, reg_stat.RUPO_LYC_MATCHn.qp_new());
  /* p21.PUZO_STAT3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_VAVE_FF41_RDn, reg_stat.ROXE_STAT_HBI_ENn.qp_new());
  /* p21.POFO_STAT4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_VAVE_FF41_RDn, reg_stat.RUFO_STAT_VBI_ENn.qp_new());
  /* p21.SASY_STAT5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_VAVE_FF41_RDn, reg_stat.REFE_STAT_OAI_ENn.qp_new());
  /* p21.POTE_STAT6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_VAVE_FF41_RDn, reg_stat.RUGU_STAT_LYI_ENn.qp_new());
}

void GateBoy::reg_stat_write()
{
  // FF41 STAT
  /* p21.SEPA*/ wire _SEPA_FF41_WRp = and2(CUPA_CPU_WRp(), new_bus.VARY_FF41p());
  /* p21.RYVE*/ wire _RYVE_FF41_WRn = not1(_SEPA_FF41_WRp);

  // FIXME polarity doesn't seem right, are we sure that RUPO is a nor latch and these signals are correct?
  /* p21.RYJU*/ wire _RYJU_FF41_WRn = not1(_SEPA_FF41_WRp);
  /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = or2(WESY_SYS_RSTn(), _RYJU_FF41_WRn);
  /* p21.RUPO*/ reg_stat.RUPO_LYC_MATCHn.nor_latch(_PAGO_LYC_MATCH_RST, reg_lyc.ROPO_LY_MATCH_SYNCp.qp_new());

  /* p21.ROXE*/ reg_stat.ROXE_STAT_HBI_ENn.dff9(_RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p21.RUFO*/ reg_stat.RUFO_STAT_VBI_ENn.dff9(_RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p21.REFE*/ reg_stat.REFE_STAT_OAI_ENn.dff9(_RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p21.RUGU*/ reg_stat.RUGU_STAT_LYI_ENn.dff9(_RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_scy_read() {
  /* p23.ANYP*/ wire _ANYP_FF42_RDp = and2(ASOT_CPU_RDp(), new_bus.XARO_FF42p());
  /* p23.BUWY*/ wire _BUWY_FF42_RDn = not1(_ANYP_FF42_RDp);
  /*#p23.WARE_SCY0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_BUWY_FF42_RDn, reg_scy.GAVE_SCY0n.qp_new());
  /* p23.GOBA_SCY1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_BUWY_FF42_RDn, reg_scy.FYMO_SCY1n.qp_new());
  /* p23.GONU_SCY2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_BUWY_FF42_RDn, reg_scy.FEZU_SCY2n.qp_new());
  /* p23.GODO_SCY3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_BUWY_FF42_RDn, reg_scy.FUJO_SCY3n.qp_new());
  /* p23.CUSA_SCY4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_BUWY_FF42_RDn, reg_scy.DEDE_SCY4n.qp_new());
  /* p23.GYZO_SCY5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_BUWY_FF42_RDn, reg_scy.FOTY_SCY5n.qp_new());
  /* p23.GUNE_SCY6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_BUWY_FF42_RDn, reg_scy.FOHA_SCY6n.qp_new());
  /* p23.GYZA_SCY7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_BUWY_FF42_RDn, reg_scy.FUNY_SCY7n.qp_new());
}

void GateBoy::reg_scy_write() {
  /* p23.BEDY*/ wire _BEDY_FF42_WRp = and2(CUPA_CPU_WRp(), new_bus.XARO_FF42p());
  /* p23.CAVO*/ wire _CAVO_FF42_WRn = not1(_BEDY_FF42_WRp);
  /* p23.GAVE*/ reg_scy.GAVE_SCY0n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D00p.qp_old());
  /* p23.FYMO*/ reg_scy.FYMO_SCY1n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D01p.qp_old());
  /* p23.FEZU*/ reg_scy.FEZU_SCY2n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D02p.qp_old());
  /* p23.FUJO*/ reg_scy.FUJO_SCY3n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p23.DEDE*/ reg_scy.DEDE_SCY4n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p23.FOTY*/ reg_scy.FOTY_SCY5n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p23.FOHA*/ reg_scy.FOHA_SCY6n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
  /* p23.FUNY*/ reg_scy.FUNY_SCY7n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_scx_read() {
  /* p23.AVOG*/ wire _AVOG_FF43_RDp = and2(ASOT_CPU_RDp(), new_bus.XAVY_FF43p());
  /* p23.BEBA*/ wire _BEBA_FF43_RDn = not1(_AVOG_FF43_RDp);
  /*#p23.EDOS_SCX0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_BEBA_FF43_RDn, reg_scx.DATY_SCX0n.qp_new());
  /* p23.EKOB_SCX1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_BEBA_FF43_RDn, reg_scx.DUZU_SCX1n.qp_new());
  /* p23.CUGA_SCX2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_BEBA_FF43_RDn, reg_scx.CYXU_SCX2n.qp_new());
  /* p23.WONY_SCX3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_BEBA_FF43_RDn, reg_scx.GUBO_SCX3n.qp_new());
  /* p23.CEDU_SCX4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_BEBA_FF43_RDn, reg_scx.BEMY_SCX4n.qp_new());
  /* p23.CATA_SCX5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_BEBA_FF43_RDn, reg_scx.CUZY_SCX5n.qp_new());
  /* p23.DOXE_SCX6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_BEBA_FF43_RDn, reg_scx.CABU_SCX6n.qp_new());
  /* p23.CASY_SCX7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_BEBA_FF43_RDn, reg_scx.BAKE_SCX7n.qp_new());
}

void GateBoy::reg_scx_write() {
  /* p23.ARUR*/ wire _ARUR_FF43_WRp = and2(CUPA_CPU_WRp(), new_bus.XAVY_FF43p());
  /* p23.AMUN*/ wire _AMUN_FF43_WRn = not1(_ARUR_FF43_WRp);
  /* p23.DATY*/ reg_scx.DATY_SCX0n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D00p.qp_old());
  /* p23.DUZU*/ reg_scx.DUZU_SCX1n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D01p.qp_old());
  /* p23.CYXU*/ reg_scx.CYXU_SCX2n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D02p.qp_old());
  /* p23.GUBO*/ reg_scx.GUBO_SCX3n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p23.BEMY*/ reg_scx.BEMY_SCX4n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p23.CUZY*/ reg_scx.CUZY_SCX5n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p23.CABU*/ reg_scx.CABU_SCX6n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
  /* p23.BAKE*/ reg_scx.BAKE_SCX7n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), old_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_ly_read()
{
}

void GateBoy::reg_ly_tock2()
{
}

//------------------------------------------------------------------------------------------------------------------------

wire GateBoy::TEGY_STROBE() const {

  /*#p21.TOCU*/ wire _TOCU_LX0n = not1(reg_lx.SAXO_LX0p.qp_new());
  /*#p21.VEPE*/ wire _VEPE_LX1n = not1(reg_lx.TYPO_LX1p.qp_new());
  /* p21.VUTY*/ wire _VUTY_LX2n = not1(reg_lx.VYZO_LX2p.qp_new());
  /* p21.VATE*/ wire _VATE_LX3n = not1(reg_lx.TELU_LX3p.qp_new());
  /* p21.TUDA*/ wire _TUDA_LX4n = not1(reg_lx.SUDE_LX4p.qp_new());
  /* p21.TAFY*/ wire _TAFY_LX5n = not1(reg_lx.TAHA_LX5p.qp_new());
  /* p21.TUJU*/ wire _TUJU_LX6n = not1(reg_lx.TYRY_LX6p.qp_new());

  /* p21.VOKU*/ wire _VOKU_LX000n = nand7(_TUJU_LX6n,         _TAFY_LX5n,         _TUDA_LX4n,         _VATE_LX3n,         _VUTY_LX2n,         _VEPE_LX1n,         _TOCU_LX0n); // 0000000 == 0
  /* p21.TOZU*/ wire _TOZU_LX007n = nand7(_TUJU_LX6n,         _TAFY_LX5n,         _TUDA_LX4n,         _VATE_LX3n,         reg_lx.VYZO_LX2p.qp_new(), reg_lx.TYPO_LX1p.qp_new(), reg_lx.SAXO_LX0p.qp_new()); // 0000111 == 7
  /* p21.TECE*/ wire _TECE_LX045n = nand7(_TUJU_LX6n,         reg_lx.TAHA_LX5p.qp_new(), _TUDA_LX4n,         reg_lx.TELU_LX3p.qp_new(), reg_lx.VYZO_LX2p.qp_new(), _VEPE_LX1n,         reg_lx.SAXO_LX0p.qp_new()); // 0101101 == 45
  /*#p21.TEBO*/ wire _TEBO_LX083n = nand7(reg_lx.TYRY_LX6p.qp_new(), _TAFY_LX5n,         reg_lx.SUDE_LX4p.qp_new(), _VATE_LX3n,         _VUTY_LX2n,         reg_lx.TYPO_LX1p.qp_new(), reg_lx.SAXO_LX0p.qp_new()); // 1010011 == 83

  /*#p21.TEGY*/ wire _TEGY_STROBE = nand4(_VOKU_LX000n,       _TOZU_LX007n,       _TECE_LX045n, _TEBO_LX083n);

  return _TEGY_STROBE;
}

void GateBoy::reg_lx_tock() {
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_lyc_read()
{
  /* p23.XYLY*/ wire _XYLY_FF45_RDp = and2(ASOT_CPU_RDp(), new_bus.XAYU_FF45p());
  /* p23.WEKU*/ wire _WEKU_FF45_RDn = not1(_XYLY_FF45_RDp);
  /*#p23.RETU_LYC0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.SYRY_LYC0n.qp_new());
  /* p23.VOJO_LYC1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.VUCE_LYC1n.qp_new());
  /* p23.RAZU_LYC2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.SEDY_LYC2n.qp_new());
  /* p23.REDY_LYC3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.SALO_LYC3n.qp_new());
  /* p23.RACE_LYC4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.SOTA_LYC4n.qp_new());
  /*#p23.VAZU_LYC5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.VAFA_LYC5n.qp_new());
  /* p23.VAFE_LYC6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.VEVO_LYC6n.qp_new());
  /* p23.PUFY_LYC7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_WEKU_FF45_RDn, reg_lyc.RAHA_LYC7n.qp_new());
}

void GateBoy::reg_lyc_write()
{
  /* p23.XUFA*/ wire _XUFA_FF45_WRn = and2(CUPA_CPU_WRp(), new_bus.XAYU_FF45p());
  /* p23.WANE*/ wire _WANE_FF45_WRp = not1(_XUFA_FF45_WRn);
  /* p23.SYRY*/ reg_lyc.SYRY_LYC0n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D00p.qp_old());
  /* p23.VUCE*/ reg_lyc.VUCE_LYC1n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D01p.qp_old());
  /* p23.SEDY*/ reg_lyc.SEDY_LYC2n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D02p.qp_old());
  /* p23.SALO*/ reg_lyc.SALO_LYC3n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p23.SOTA*/ reg_lyc.SOTA_LYC4n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p23.VAFA*/ reg_lyc.VAFA_LYC5n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p23.VEVO*/ reg_lyc.VEVO_LYC6n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
  /* p23.RAHA*/ reg_lyc.RAHA_LYC7n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), old_bus.BUS_CPU_D07p.qp_old());
}

void GateBoy::reg_lyc_tock2()
{
  // LYC matcher
  /* p21.RYME*/ wire _RYME_LY_MATCH0n_old = xor2(reg_ly.MUWY_LY0p.qp_old(), reg_lyc.SYRY_LYC0n.qn_old());
  /* p21.TYDE*/ wire _TYDE_LY_MATCH1n_old = xor2(reg_ly.MYRO_LY1p.qp_old(), reg_lyc.VUCE_LYC1n.qn_old());
  /* p21.REDA*/ wire _REDA_LY_MATCH2n_old = xor2(reg_ly.LEXA_LY2p.qp_old(), reg_lyc.SEDY_LYC2n.qn_old());
  /* p21.RASY*/ wire _RASY_LY_MATCH3n_old = xor2(reg_ly.LYDO_LY3p.qp_old(), reg_lyc.SALO_LYC3n.qn_old());
  /* p21.TYKU*/ wire _TYKU_LY_MATCH4n_old = xor2(reg_ly.LOVU_LY4p.qp_old(), reg_lyc.SOTA_LYC4n.qn_old());
  /* p21.TUCY*/ wire _TUCY_LY_MATCH5n_old = xor2(reg_ly.LEMA_LY5p.qp_old(), reg_lyc.VAFA_LYC5n.qn_old());
  /* p21.TERY*/ wire _TERY_LY_MATCH6n_old = xor2(reg_ly.MATO_LY6p.qp_old(), reg_lyc.VEVO_LYC6n.qn_old());
  /* p21.SYFU*/ wire _SYFU_LY_MATCH7n_old = xor2(reg_ly.LAFO_LY7p.qp_old(), reg_lyc.RAHA_LYC7n.qn_old());

  /*#p21.SOVU*/ wire _SOVU_LY_MATCHA_old = nor4 (_SYFU_LY_MATCH7n_old, _TERY_LY_MATCH6n_old, _TUCY_LY_MATCH5n_old, _TYKU_LY_MATCH4n_old);
  /*#p21.SUBO*/ wire _SUBO_LY_MATCHB_old = nor4 (_RASY_LY_MATCH3n_old, _REDA_LY_MATCH2n_old, _TYDE_LY_MATCH1n_old, _RYME_LY_MATCH0n_old);
  /*#p21.RAPE*/ wire _RAPE_LY_MATCHn_old = nand2(_SOVU_LY_MATCHA_old,  _SUBO_LY_MATCHB_old);
  /*#p21.PALY*/ wire _PALY_LY_MATCHa_old = not1 (_RAPE_LY_MATCHn_old);

  /*#p21.ROPO*/ reg_lyc.ROPO_LY_MATCH_SYNCp.dff17(TALU_xxCDEFxx(), WESY_SYS_RSTn(), _PALY_LY_MATCHa_old);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_wy_write()
{
  /* p23.WEKO*/ wire _WEKO_FF4A_WRp = and2(CUPA_CPU_WRp(), new_bus.VYGA_FF4Ap());
  /* p23.VEFU*/ wire _VEFU_FF4A_WRn = not1(_WEKO_FF4A_WRp);
  /* p23.NESO*/ reg_wy.NESO_WY0n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D00p.qp_old());
  /* p23.NYRO*/ reg_wy.NYRO_WY1n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D01p.qp_old());
  /* p23.NAGA*/ reg_wy.NAGA_WY2n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D02p.qp_old());
  /* p23.MELA*/ reg_wy.MELA_WY3n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p23.NULO*/ reg_wy.NULO_WY4n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p23.NENE*/ reg_wy.NENE_WY5n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p23.NUKA*/ reg_wy.NUKA_WY6n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
  /* p23.NAFU*/ reg_wy.NAFU_WY7n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D07p.qp_old());
}

void GateBoy::reg_wy_read()
{
  /* p23.WAXU*/ wire _WAXU_FF4A_RDp = and2(ASOT_CPU_RDp(), new_bus.VYGA_FF4Ap());
  /* p23.VOMY*/ wire _VOMY_FF4A_RDn = not1(_WAXU_FF4A_RDp);
  /*#p23.PUNU_WY0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_VOMY_FF4A_RDn, reg_wy.NESO_WY0n.qp_new());
  /* p23.PODA_WY1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_VOMY_FF4A_RDn, reg_wy.NYRO_WY1n.qp_new());
  /* p23.PYGU_WY2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_VOMY_FF4A_RDn, reg_wy.NAGA_WY2n.qp_new());
  /* p23.LOKA_WY3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_VOMY_FF4A_RDn, reg_wy.MELA_WY3n.qp_new());
  /* p23.MEGA_WY4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_VOMY_FF4A_RDn, reg_wy.NULO_WY4n.qp_new());
  /* p23.PELA_WY5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_VOMY_FF4A_RDn, reg_wy.NENE_WY5n.qp_new());
  /* p23.POLO_WY6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_VOMY_FF4A_RDn, reg_wy.NUKA_WY6n.qp_new());
  /* p23.MERA_WY7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_VOMY_FF4A_RDn, reg_wy.NAFU_WY7n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_wx_read() {
  /* p23.WYZE*/ wire _WYZE_FF4B_RDp = and2(ASOT_CPU_RDp(), new_bus.VUMY_FF4Bp());
  /* p23.VYCU*/ wire _VYCU_FF4B_RDn = not1(_WYZE_FF4B_RDp);
  /*#p23.LOVA_WX0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_VYCU_FF4B_RDn, reg_wx.MYPA_WX0n.qp_new());
  /* p23.MUKA_WX1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_VYCU_FF4B_RDn, reg_wx.NOFE_WX1n.qp_new());
  /* p23.MOKO_WX2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_VYCU_FF4B_RDn, reg_wx.NOKE_WX2n.qp_new());
  /* p23.LOLE_WX3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_VYCU_FF4B_RDn, reg_wx.MEBY_WX3n.qp_new());
  /* p23.MELE_WX4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_VYCU_FF4B_RDn, reg_wx.MYPU_WX4n.qp_new());
  /* p23.MUFE_WX5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_VYCU_FF4B_RDn, reg_wx.MYCE_WX5n.qp_new());
  /* p23.MULY_WX6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_VYCU_FF4B_RDn, reg_wx.MUVO_WX6n.qp_new());
  /* p23.MARA_WX7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_VYCU_FF4B_RDn, reg_wx.NUKU_WX7n.qp_new());
}

void GateBoy::reg_wx_write() {
  /* p23.WUZA*/ wire _WUZA_FF4B_WRp = and2(CUPA_CPU_WRp(), new_bus.VUMY_FF4Bp());
  /* p23.VOXU*/ wire _VOXU_FF4B_WRn = not1(_WUZA_FF4B_WRp);
  /* p23.MYPA*/ reg_wx.MYPA_WX0n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D00p.qp_old());
  /* p23.NOFE*/ reg_wx.NOFE_WX1n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D01p.qp_old());
  /* p23.NOKE*/ reg_wx.NOKE_WX2n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D02p.qp_old());
  /* p23.MEBY*/ reg_wx.MEBY_WX3n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p23.MYPU*/ reg_wx.MYPU_WX4n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p23.MYCE*/ reg_wx.MYCE_WX5n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p23.MUVO*/ reg_wx.MUVO_WX6n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
  /* p23.NUKU*/ reg_wx.NUKU_WX7n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), old_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::store_vram_data()
{
  /* p24.LOBY*/ wire _LOBY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n = not1(tile_fetcher._LAXU_BFETCH_S0p.qp_new());
  /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n = not1(tile_fetcher._MESU_BFETCH_S1p.qp_new());
  /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n = not1(tile_fetcher._NYVA_BFETCH_S2p.qp_new());
  /* p29.TYTU*/ wire _TYTU_SFETCH_S0n = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
  /* p29.SYCU*/ wire _SYCU_SFETCH_S0pe = nor3(_TYTU_SFETCH_S0n, _LOBY_RENDERINGn, sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new());

  /*#p27.MYSO*/ wire _MYSO_STORE_VRAM_DATA_TRIGp = nor3(_LOBY_RENDERINGn, _LAXE_BFETCH_S0n, tile_fetcher._LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn = nand3(_MYSO_STORE_VRAM_DATA_TRIGp, tile_fetcher._MESU_BFETCH_S1p.qp_new(), _NOFU_BFETCH_S2n); // NYDY on fetch phase 6
  /* p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp = and2(_MYSO_STORE_VRAM_DATA_TRIGp, _NAKO_BFETCH_S1n); // MOFU fires on fetch phase 2 and 10
  /*#p29.TOPU*/ wire _TOPU_STORE_SPRITE_Ap = and2(sprite_fetcher.TULY_SFETCH_S1p.qp_new(), _SYCU_SFETCH_S0pe);
  /*#p29.RACA*/ wire _RACA_STORE_SPRITE_Bp = and2(sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new(), _SYCU_SFETCH_S0pe);

  {
    /*#p32.METE*/ wire _METE_LATCH_TILE_DAp = not1(_NYDY_LATCH_TILE_DAn);
    /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn = not1(_METE_LATCH_TILE_DAp);
    /* p32.LEGU*/ tile_temp_a.LEGU_TILE_DA0n.dff8p(_LOMA_LATCH_TILE_DAn, old_bus.BUS_VRAM_D00p.qp_old());
    /* p32.NUDU*/ tile_temp_a.NUDU_TILE_DA1n.dff8p(_LOMA_LATCH_TILE_DAn, old_bus.BUS_VRAM_D01p.qp_old());
    /* p32.MUKU*/ tile_temp_a.MUKU_TILE_DA2n.dff8p(_LOMA_LATCH_TILE_DAn, old_bus.BUS_VRAM_D02p.qp_old());
    /* p32.LUZO*/ tile_temp_a.LUZO_TILE_DA3n.dff8p(_LOMA_LATCH_TILE_DAn, old_bus.BUS_VRAM_D03p.qp_old());
    /* p32.MEGU*/ tile_temp_a.MEGU_TILE_DA4n.dff8p(_LOMA_LATCH_TILE_DAn, old_bus.BUS_VRAM_D04p.qp_old());
    /* p32.MYJY*/ tile_temp_a.MYJY_TILE_DA5n.dff8p(_LOMA_LATCH_TILE_DAn, old_bus.BUS_VRAM_D05p.qp_old());
    /* p32.NASA*/ tile_temp_a.NASA_TILE_DA6n.dff8p(_LOMA_LATCH_TILE_DAn, old_bus.BUS_VRAM_D06p.qp_old());
    /* p32.NEFO*/ tile_temp_a.NEFO_TILE_DA7n.dff8p(_LOMA_LATCH_TILE_DAn, old_bus.BUS_VRAM_D07p.qp_old());
  }

  {
    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.

    /* p32.LESO*/ wire _LESO_LATCH_TILE_DBn = not1(_MOFU_LATCH_TILE_DBp);
    /* p32.LUVE*/ wire _LUVE_LATCH_TILE_DBp = not1(_LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /* p32.LABU*/ wire _LABU_LATCH_TILE_DBn = not1(_LUVE_LATCH_TILE_DBp);
    /* p32.RAWU*/ tile_temp_b.RAWU_TILE_DB0p.dff11(_LABU_LATCH_TILE_DBn, SIG_VCC.qp_new(), old_bus.BUS_VRAM_D00p.qp_old());
    /* p32.POZO*/ tile_temp_b.POZO_TILE_DB1p.dff11(_LABU_LATCH_TILE_DBn, SIG_VCC.qp_new(), old_bus.BUS_VRAM_D01p.qp_old());
    /* p32.PYZO*/ tile_temp_b.PYZO_TILE_DB2p.dff11(_LABU_LATCH_TILE_DBn, SIG_VCC.qp_new(), old_bus.BUS_VRAM_D02p.qp_old());
    /* p32.POXA*/ tile_temp_b.POXA_TILE_DB3p.dff11(_LABU_LATCH_TILE_DBn, SIG_VCC.qp_new(), old_bus.BUS_VRAM_D03p.qp_old());
    /* p32.PULO*/ tile_temp_b.PULO_TILE_DB4p.dff11(_LABU_LATCH_TILE_DBn, SIG_VCC.qp_new(), old_bus.BUS_VRAM_D04p.qp_old());
    /* p32.POJU*/ tile_temp_b.POJU_TILE_DB5p.dff11(_LABU_LATCH_TILE_DBn, SIG_VCC.qp_new(), old_bus.BUS_VRAM_D05p.qp_old());
    /* p32.POWY*/ tile_temp_b.POWY_TILE_DB6p.dff11(_LABU_LATCH_TILE_DBn, SIG_VCC.qp_new(), old_bus.BUS_VRAM_D06p.qp_old());
    /* p32.PYJU*/ tile_temp_b.PYJU_TILE_DB7p.dff11(_LABU_LATCH_TILE_DBn, SIG_VCC.qp_new(), old_bus.BUS_VRAM_D07p.qp_old());
  }

  {
    /*#p29.VYWA*/ wire _VYWA_STORE_SPRITE_An = not1(_TOPU_STORE_SPRITE_Ap);
    /*#p29.WENY*/ wire _WENY_STORE_SPRITE_Ap = not1(_VYWA_STORE_SPRITE_An);
    /*#p29.XADO*/ wire _XADO_STORE_SPRITE_An = not1(_WENY_STORE_SPRITE_Ap);
    /* p33.REWO*/ sprite_pix_a.REWO_SPRITE_DA0n.dff8n(_XADO_STORE_SPRITE_An, flipped_sprite.PUTE_FLIP0p.qp_old());
    /* p33.PEBA*/ sprite_pix_a.PEBA_SPRITE_DA1n.dff8n(_XADO_STORE_SPRITE_An, flipped_sprite.PELO_FLIP1p.qp_old());
    /* p33.MOFO*/ sprite_pix_a.MOFO_SPRITE_DA2n.dff8n(_XADO_STORE_SPRITE_An, flipped_sprite.PONO_FLIP2p.qp_old());
    /* p33.PUDU*/ sprite_pix_a.PUDU_SPRITE_DA3n.dff8n(_XADO_STORE_SPRITE_An, flipped_sprite.POBE_FLIP3p.qp_old());
    /* p33.SAJA*/ sprite_pix_a.SAJA_SPRITE_DA4n.dff8n(_XADO_STORE_SPRITE_An, flipped_sprite.PACY_FLIP4p.qp_old());
    /* p33.SUNY*/ sprite_pix_a.SUNY_SPRITE_DA5n.dff8n(_XADO_STORE_SPRITE_An, flipped_sprite.PUGU_FLIP5p.qp_old());
    /* p33.SEMO*/ sprite_pix_a.SEMO_SPRITE_DA6n.dff8n(_XADO_STORE_SPRITE_An, flipped_sprite.PAWE_FLIP6p.qp_old());
    /* p33.SEGA*/ sprite_pix_a.SEGA_SPRITE_DA7n.dff8n(_XADO_STORE_SPRITE_An, flipped_sprite.PULY_FLIP7p.qp_old());
  }

  {
    /*#p29.PEBY*/ wire _PEBY_STORE_SPRITE_Bn = not1(_RACA_STORE_SPRITE_Bp);
    /*#p29.NYBE*/ wire _NYBE_STORE_SPRITE_Bp = not1(_PEBY_STORE_SPRITE_Bn);
    /*#p29.PUCO*/ wire _PUCO_STORE_SPRITE_Bn = not1(_NYBE_STORE_SPRITE_Bp);
    /* p33.PEFO*/ sprite_pix_b.PEFO_SPRITE_DB0n.dff8n(_PUCO_STORE_SPRITE_Bn, flipped_sprite.PUTE_FLIP0p.qp_old());
    /* p33.ROKA*/ sprite_pix_b.ROKA_SPRITE_DB1n.dff8n(_PUCO_STORE_SPRITE_Bn, flipped_sprite.PELO_FLIP1p.qp_old());
    /* p33.MYTU*/ sprite_pix_b.MYTU_SPRITE_DB2n.dff8n(_PUCO_STORE_SPRITE_Bn, flipped_sprite.PONO_FLIP2p.qp_old());
    /* p33.RAMU*/ sprite_pix_b.RAMU_SPRITE_DB3n.dff8n(_PUCO_STORE_SPRITE_Bn, flipped_sprite.POBE_FLIP3p.qp_old());
    /* p33.SELE*/ sprite_pix_b.SELE_SPRITE_DB4n.dff8n(_PUCO_STORE_SPRITE_Bn, flipped_sprite.PACY_FLIP4p.qp_old());
    /* p33.SUTO*/ sprite_pix_b.SUTO_SPRITE_DB5n.dff8n(_PUCO_STORE_SPRITE_Bn, flipped_sprite.PUGU_FLIP5p.qp_old());
    /* p33.RAMA*/ sprite_pix_b.RAMA_SPRITE_DB6n.dff8n(_PUCO_STORE_SPRITE_Bn, flipped_sprite.PAWE_FLIP6p.qp_old());
    /* p33.RYDU*/ sprite_pix_b.RYDU_SPRITE_DB7n.dff8n(_PUCO_STORE_SPRITE_Bn, flipped_sprite.PULY_FLIP7p.qp_old());
  }
}

//------------------------------------------------------------------------------------------------------------------------

