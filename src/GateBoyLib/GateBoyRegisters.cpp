#include "GateBoyLib/GateBoyRegisters.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

SpriteDeltaY GateBoy::sub_sprite_y_gates() {
  /*#p29.EBOS*/ wire EBOS_LY0n = not1(reg.reg_ly.MUWY_LY0p.qp_new());
  /*_p29.DASA*/ wire DASA_LY1n = not1(reg.reg_ly.MYRO_LY1p.qp_new());
  /*_p29.FUKY*/ wire FUKY_LY2n = not1(reg.reg_ly.LEXA_LY2p.qp_new());
  /*_p29.FUVE*/ wire FUVE_LY3n = not1(reg.reg_ly.LYDO_LY3p.qp_new());
  /*_p29.FEPU*/ wire FEPU_LY4n = not1(reg.reg_ly.LOVU_LY4p.qp_new());
  /*_p29.FOFA*/ wire FOFA_LY5n = not1(reg.reg_ly.LEMA_LY5p.qp_new());
  /*_p29.FEMO*/ wire FEMO_LY6n = not1(reg.reg_ly.MATO_LY6p.qp_new());
  /*_p29.GUSU*/ wire GUSU_LY7n = not1(reg.reg_ly.LAFO_LY7p.qp_new());

  /*_p29.ERUC*/ Adder ERUC_YDIFF0 = add3(EBOS_LY0n, reg.oam_temp_a.XUSO_OAM_DA0p.qp_new(), reg.SIG_GND.out_new());
  /*_p29.ENEF*/ Adder ENEF_YDIFF1 = add3(DASA_LY1n, reg.oam_temp_a.XEGU_OAM_DA1p.qp_new(), ERUC_YDIFF0.carry);
  /*_p29.FECO*/ Adder FECO_YDIFF2 = add3(FUKY_LY2n, reg.oam_temp_a.YJEX_OAM_DA2p.qp_new(), ENEF_YDIFF1.carry);
  /*_p29.GYKY*/ Adder GYKY_YDIFF3 = add3(FUVE_LY3n, reg.oam_temp_a.XYJU_OAM_DA3p.qp_new(), FECO_YDIFF2.carry);
  /*_p29.GOPU*/ Adder GOPU_YDIFF4 = add3(FEPU_LY4n, reg.oam_temp_a.YBOG_OAM_DA4p.qp_new(), GYKY_YDIFF3.carry);
  /*_p29.FUWA*/ Adder FUWA_YDIFF5 = add3(FOFA_LY5n, reg.oam_temp_a.WYSO_OAM_DA5p.qp_new(), GOPU_YDIFF4.carry);
  /*_p29.GOJU*/ Adder GOJU_YDIFF6 = add3(FEMO_LY6n, reg.oam_temp_a.XOTE_OAM_DA6p.qp_new(), FUWA_YDIFF5.carry);
  /*_p29.WUHU*/ Adder WUHU_YDIFF7 = add3(GUSU_LY7n, reg.oam_temp_a.YZAB_OAM_DA7p.qp_new(), GOJU_YDIFF6.carry);

  return {
    ERUC_YDIFF0,
    ENEF_YDIFF1,
    FECO_YDIFF2,
    GYKY_YDIFF3,
    GOPU_YDIFF4,
    FUWA_YDIFF5,
    GOJU_YDIFF6,
    WUHU_YDIFF7,
  };
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_lcdc_gates(const GateBoyReg& reg_old)
{
  /*_p23.WARU*/ wire WARU_FF40_WRp = and2(CUPA_CPU_WRp(), reg.cpu_abus.VOCA_FF40p());
  /*_p23.XUBO*/ wire XUBO_FF40_WRn = not1(WARU_FF40_WRp);
  /*#p23.VYXE*/ reg.reg_lcdc.VYXE_LCDC_BGENn  .dff9(XUBO_FF40_WRn, XARE_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p23.XYLO*/ reg.reg_lcdc.XYLO_LCDC_SPENn  .dff9(XUBO_FF40_WRn, XARE_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p23.XYMO*/ reg.reg_lcdc.XYMO_LCDC_SPSIZEn.dff9(XUBO_FF40_WRn, XARE_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p23.XAFO*/ reg.reg_lcdc.XAFO_LCDC_BGMAPn .dff9(XUBO_FF40_WRn, XARE_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p23.WEXU*/ reg.reg_lcdc.WEXU_LCDC_BGTILEn.dff9(XUBO_FF40_WRn, XARE_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p23.WYMO*/ reg.reg_lcdc.WYMO_LCDC_WINENn .dff9(XUBO_FF40_WRn, XARE_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p23.WOKY*/ reg.reg_lcdc.WOKY_LCDC_WINMAPn.dff9(XUBO_FF40_WRn, XARE_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p23.XONA*/ reg.reg_lcdc.XONA_LCDC_LCDENn. dff9(XUBO_FF40_WRn, XARE_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p23.VYRE*/ wire VYRE_FF40_RDp = and2(ASOT_CPU_RDp(), reg.cpu_abus.VOCA_FF40p());
  /*_p23.WYCE*/ wire WYCE_FF40_RDn = not1(VYRE_FF40_RDp);
  /*#p23.WYPO*/ triwire WYPO_LCDC0_TO_CD0 = tri6_nn(WYCE_FF40_RDn, reg.reg_lcdc.VYXE_LCDC_BGENn.qp_new());
  /*#p23.XERO*/ triwire XERO_LCDC1_TO_CD1 = tri6_nn(WYCE_FF40_RDn, reg.reg_lcdc.XYLO_LCDC_SPENn.qp_new());
  /*_p23.WYJU*/ triwire WYJU_LCDC2_TO_CD2 = tri6_nn(WYCE_FF40_RDn, reg.reg_lcdc.XYMO_LCDC_SPSIZEn.qp_new());
  /*_p23.WUKA*/ triwire WUKA_LCDC3_TO_CD3 = tri6_nn(WYCE_FF40_RDn, reg.reg_lcdc.XAFO_LCDC_BGMAPn.qp_new());
  /*_p23.VOKE*/ triwire VOKE_LCDC4_TO_CD4 = tri6_nn(WYCE_FF40_RDn, reg.reg_lcdc.WEXU_LCDC_BGTILEn.qp_new());
  /*_p23.VATO*/ triwire VATO_LCDC5_TO_CD5 = tri6_nn(WYCE_FF40_RDn, reg.reg_lcdc.WYMO_LCDC_WINENn.qp_new());
  /*#p23.VAHA*/ triwire VAHA_LCDC6_TO_CD6 = tri6_nn(WYCE_FF40_RDn, reg.reg_lcdc.WOKY_LCDC_WINMAPn.qp_new());
  /*#p23.XEBU*/ triwire XEBU_LCDC7_TO_CD7 = tri6_nn(WYCE_FF40_RDn, reg.reg_lcdc.XONA_LCDC_LCDENn.qp_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus.BUS_CPU_D00p.tri_bus(WYPO_LCDC0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus.BUS_CPU_D01p.tri_bus(XERO_LCDC1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus.BUS_CPU_D02p.tri_bus(WYJU_LCDC2_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus.BUS_CPU_D03p.tri_bus(WUKA_LCDC3_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus.BUS_CPU_D04p.tri_bus(VOKE_LCDC4_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus.BUS_CPU_D05p.tri_bus(VATO_LCDC5_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus.BUS_CPU_D06p.tri_bus(VAHA_LCDC6_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus.BUS_CPU_D07p.tri_bus(XEBU_LCDC7_TO_CD7);
}

//------------------------------------------------------------------------------------------------------------------------


