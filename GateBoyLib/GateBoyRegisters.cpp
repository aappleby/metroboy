#include "GateBoyLib/GateBoyRegisters.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

SpriteDeltaY GateBoy::sub_sprite_y(const RegLY& reg_ly, const OamTempA& oam_temp_a) {
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

