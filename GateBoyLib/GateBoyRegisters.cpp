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

  /* p29.ERUC*/ Adder ERUC_YDIFF0 = add3(EBOS_LY0n, oam_temp_a.XUSO_OAM_DA0p.qp_new(), SIG_GND.qp_new());
  /* p29.ENEF*/ Adder ENEF_YDIFF1 = add3(DASA_LY1n, oam_temp_a.XEGU_OAM_DA1p.qp_new(), ERUC_YDIFF0.carry);
  /* p29.FECO*/ Adder FECO_YDIFF2 = add3(FUKY_LY2n, oam_temp_a.YJEX_OAM_DA2p.qp_new(), ENEF_YDIFF1.carry);
  /* p29.GYKY*/ Adder GYKY_YDIFF3 = add3(FUVE_LY3n, oam_temp_a.XYJU_OAM_DA3p.qp_new(), FECO_YDIFF2.carry);
  /* p29.GOPU*/ Adder GOPU_YDIFF4 = add3(FEPU_LY4n, oam_temp_a.YBOG_OAM_DA4p.qp_new(), GYKY_YDIFF3.carry);
  /* p29.FUWA*/ Adder FUWA_YDIFF5 = add3(FOFA_LY5n, oam_temp_a.WYSO_OAM_DA5p.qp_new(), GOPU_YDIFF4.carry);
  /* p29.GOJU*/ Adder GOJU_YDIFF6 = add3(FEMO_LY6n, oam_temp_a.XOTE_OAM_DA6p.qp_new(), FUWA_YDIFF5.carry);
  /* p29.WUHU*/ Adder WUHU_YDIFF7 = add3(GUSU_LY7n, oam_temp_a.YZAB_OAM_DA7p.qp_new(), GOJU_YDIFF6.carry);

  /* p29.DEGE*/ wire DEGE_SPRITE_DELTA0 = not1(ERUC_YDIFF0.sum);
  /* p29.DABY*/ wire DABY_SPRITE_DELTA1 = not1(ENEF_YDIFF1.sum);
  /* p29.DABU*/ wire DABU_SPRITE_DELTA2 = not1(FECO_YDIFF2.sum);
  /* p29.GYSA*/ wire GYSA_SPRITE_DELTA3 = not1(GYKY_YDIFF3.sum);
  /* p29.GACE*/ wire GACE_SPRITE_DELTA4 = not1(GOPU_YDIFF4.sum);
  /* p29.GUVU*/ wire GUVU_SPRITE_DELTA5 = not1(FUWA_YDIFF5.sum);
  /* p29.GYDA*/ wire GYDA_SPRITE_DELTA6 = not1(GOJU_YDIFF6.sum);
  /* p29.GEWY*/ wire GEWY_SPRITE_DELTA7 = not1(WUHU_YDIFF7.sum);

  return {
    ERUC_YDIFF0,
    ENEF_YDIFF1,
    FECO_YDIFF2,
    GYKY_YDIFF3,
    GOPU_YDIFF4,
    FUWA_YDIFF5,
    GOJU_YDIFF6,
    WUHU_YDIFF7,
    DEGE_SPRITE_DELTA0,
    DABY_SPRITE_DELTA1,
    DABU_SPRITE_DELTA2,
    GYSA_SPRITE_DELTA3,
    GACE_SPRITE_DELTA4,
    GUVU_SPRITE_DELTA5,
    GYDA_SPRITE_DELTA6,
    GEWY_SPRITE_DELTA7,
  };
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_lcdc()
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

  /* p23.VYRE*/ wire _VYRE_FF40_RDp = and2(ASOT_CPU_RDp(), new_bus.VOCA_FF40p());
  /* p23.WYCE*/ wire _WYCE_FF40_RDn = not1(_VYRE_FF40_RDp);
  /*#p23.WYPO_LCDC0_TO_CD0*/ wire WYPO_LCDC0_TO_CD0 = tri6_nn(_WYCE_FF40_RDn, reg_lcdc.VYXE_LCDC_BGENn.qp_new());
  /*#p23.XERO_LCDC1_TO_CD1*/ wire XERO_LCDC1_TO_CD1 = tri6_nn(_WYCE_FF40_RDn, reg_lcdc.XYLO_LCDC_SPENn.qp_new());
  /* p23.WYJU_LCDC2_TO_CD2*/ wire WYJU_LCDC2_TO_CD2 = tri6_nn(_WYCE_FF40_RDn, reg_lcdc.XYMO_LCDC_SPSIZEn.qp_new());
  /* p23.WUKA_LCDC3_TO_CD3*/ wire WUKA_LCDC3_TO_CD3 = tri6_nn(_WYCE_FF40_RDn, reg_lcdc.XAFO_LCDC_BGMAPn.qp_new());
  /* p23.VOKE_LCDC4_TO_CD4*/ wire VOKE_LCDC4_TO_CD4 = tri6_nn(_WYCE_FF40_RDn, reg_lcdc.WEXU_LCDC_BGTILEn.qp_new());
  /* p23.VATO_LCDC5_TO_CD5*/ wire VATO_LCDC5_TO_CD5 = tri6_nn(_WYCE_FF40_RDn, reg_lcdc.WYMO_LCDC_WINENn.qp_new());
  /*#p23.VAHA_LCDC6_TO_CD6*/ wire VAHA_LCDC6_TO_CD6 = tri6_nn(_WYCE_FF40_RDn, reg_lcdc.WOKY_LCDC_WINMAPn.qp_new());
  /*#p23.XEBU_LCDC7_TO_CD7*/ wire XEBU_LCDC7_TO_CD7 = tri6_nn(_WYCE_FF40_RDn, reg_lcdc.XONA_LCDC_LCDENn.qp_new());

  /* BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(WYPO_LCDC0_TO_CD0);
  /* BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(XERO_LCDC1_TO_CD1);
  /* BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(WYJU_LCDC2_TO_CD2);
  /* BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(WUKA_LCDC3_TO_CD3);
  /* BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(VOKE_LCDC4_TO_CD4);
  /* BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(VATO_LCDC5_TO_CD5);
  /* BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(VAHA_LCDC6_TO_CD6);
  /* BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(XEBU_LCDC7_TO_CD7);
}

//------------------------------------------------------------------------------------------------------------------------

