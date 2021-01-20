#include "GateBoyLib/GateBoyRegisters.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

SpriteDeltaY GateBoy::sub_sprite_y() {
  SigIn SIG_GND = 0;

  /*#p29.EBOS*/ wire EBOS_LY0n = not1(lcd.reg_ly.MUWY_LY0p.qp_new());
  /* p29.DASA*/ wire DASA_LY1n = not1(lcd.reg_ly.MYRO_LY1p.qp_new());
  /* p29.FUKY*/ wire FUKY_LY2n = not1(lcd.reg_ly.LEXA_LY2p.qp_new());
  /* p29.FUVE*/ wire FUVE_LY3n = not1(lcd.reg_ly.LYDO_LY3p.qp_new());
  /* p29.FEPU*/ wire FEPU_LY4n = not1(lcd.reg_ly.LOVU_LY4p.qp_new());
  /* p29.FOFA*/ wire FOFA_LY5n = not1(lcd.reg_ly.LEMA_LY5p.qp_new());
  /* p29.FEMO*/ wire FEMO_LY6n = not1(lcd.reg_ly.MATO_LY6p.qp_new());
  /* p29.GUSU*/ wire GUSU_LY7n = not1(lcd.reg_ly.LAFO_LY7p.qp_new());

  /* p29.ERUC*/ auto _ERUC_YDIFF0 = add3(EBOS_LY0n, oam_bus.oam_temp_a.XUSO_OAM_DA0p.qp_new(), SIG_GND);
  /* p29.ENEF*/ auto _ENEF_YDIFF1 = add3(DASA_LY1n, oam_bus.oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF0.carry);
  /* p29.FECO*/ auto _FECO_YDIFF2 = add3(FUKY_LY2n, oam_bus.oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF1.carry);
  /* p29.GYKY*/ auto _GYKY_YDIFF3 = add3(FUVE_LY3n, oam_bus.oam_temp_a.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF2.carry);
  /* p29.GOPU*/ auto _GOPU_YDIFF4 = add3(FEPU_LY4n, oam_bus.oam_temp_a.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF3.carry);
  /* p29.FUWA*/ auto _FUWA_YDIFF5 = add3(FOFA_LY5n, oam_bus.oam_temp_a.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF4.carry);
  /* p29.GOJU*/ auto _GOJU_YDIFF6 = add3(FEMO_LY6n, oam_bus.oam_temp_a.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF5.carry);
  /* p29.WUHU*/ auto _WUHU_YDIFF7 = add3(GUSU_LY7n, oam_bus.oam_temp_a.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF6.carry);

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

//-----------------------------------------------------------------------------
// Pixel counter

void GateBoy::tock_pix_counter(wire SACU_CLKPIPE_evn) {
  // Pixel counter, has carry lookahead because this can increment every tcycle
  /* p21.RYBO*/ wire RYBO_old = xor2(pix_count.XEHO_PX0p.qp_old(), pix_count.SAVY_PX1p.qp_old()); // XOR layout 1, feet facing gnd, this should def be regular xor
  /* p21.XUKE*/ wire XUKE_old = and2(pix_count.XEHO_PX0p.qp_old(), pix_count.SAVY_PX1p.qp_old());
  /* p21.XYLE*/ wire XYLE_old = and2(pix_count.XODU_PX2p.qp_old(), XUKE_old);
  /* p21.XEGY*/ wire XEGY_old = xor2(pix_count.XODU_PX2p.qp_old(), XUKE_old); // feet facing gnd
  /* p21.XORA*/ wire XORA_old = xor2(pix_count.XYDO_PX3p.qp_old(), XYLE_old); // feet facing gnd

  /* p21.XEHO*/ pix_count.XEHO_PX0p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn_new(), pix_count.XEHO_PX0p.qn_old());
  /* p21.SAVY*/ pix_count.SAVY_PX1p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn_new(), RYBO_old);
  /* p21.XODU*/ pix_count.XODU_PX2p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn_new(), XEGY_old);
  /* p21.XYDO*/ pix_count.XYDO_PX3p.dff17(SACU_CLKPIPE_evn, TADY_LINE_RSTn_new(), XORA_old);

  /* p24.TOCA*/ wire TOCA_new = not1(pix_count.XYDO_PX3p.qp_new());
  /* p21.SAKE*/ wire SAKE_old = xor2(pix_count.TUHU_PX4p.qp_old(), pix_count.TUKY_PX5p.qp_old());
  /* p21.TYBA*/ wire TYBA_old = and2(pix_count.TUHU_PX4p.qp_old(), pix_count.TUKY_PX5p.qp_old());
  /* p21.SURY*/ wire SURY_old = and2(pix_count.TAKO_PX6p.qp_old(), TYBA_old);
  /* p21.TYGE*/ wire TYGE_old = xor2(pix_count.TAKO_PX6p.qp_old(), TYBA_old);
  /* p21.ROKU*/ wire ROKU_old = xor2(pix_count.SYBE_PX7p.qp_old(), SURY_old); // derp

  /* p21.TUHU*/ pix_count.TUHU_PX4p.dff17(TOCA_new, TADY_LINE_RSTn_new(), pix_count.TUHU_PX4p.qn_old());
  /* p21.TUKY*/ pix_count.TUKY_PX5p.dff17(TOCA_new, TADY_LINE_RSTn_new(), SAKE_old); // this is a doc
  /* p21.TAKO*/ pix_count.TAKO_PX6p.dff17(TOCA_new, TADY_LINE_RSTn_new(), TYGE_old);
  /* p21.SYBE*/ pix_count.SYBE_PX7p.dff17(TOCA_new, TADY_LINE_RSTn_new(), ROKU_old);
}

//------------------------------------------------------------------------------------------------------------------------

BGScrollX GateBoy::add_scx() {
  SigIn SIG_GND = 0;

  /*#p26.ATAD*/ auto _ATAD_TILE_X0 = add3(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n.qn_new(), SIG_GND);
  /* p26.BEHU*/ auto _BEHU_TILE_X1 = add3(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n.qn_new(), _ATAD_TILE_X0.carry);
  /* p26.APYH*/ auto _APYH_TILE_X2 = add3(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n.qn_new(), _BEHU_TILE_X1.carry);
  /* p26.BABE*/ auto _BABE_MAP_X0  = add3(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n.qn_new(), _APYH_TILE_X2.carry);
  /* p26.ABOD*/ auto _ABOD_MAP_X1  = add3(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n.qn_new(), _BABE_MAP_X0.carry);
  /* p26.BEWY*/ auto _BEWY_MAP_X2  = add3(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n.qn_new(), _ABOD_MAP_X1.carry);
  /* p26.BYCA*/ auto _BYCA_MAP_X3  = add3(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n.qn_new(), _BEWY_MAP_X2.carry);
  /* p26.ACUL*/ auto _ACUL_MAP_X4  = add3(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n.qn_new(), _BYCA_MAP_X3.carry);

  return {
    _ATAD_TILE_X0,
    _BEHU_TILE_X1,
    _APYH_TILE_X2,
    _BABE_MAP_X0,
    _ABOD_MAP_X1,
    _BEWY_MAP_X2,
    _BYCA_MAP_X3,
    _ACUL_MAP_X4
  };
}

BGScrollY GateBoy::add_scy() {
  SigIn SIG_GND = 0;

  /*#p26.FAFO*/ auto _FAFO_TILE_Y0 = add3(lcd.reg_ly.MUWY_LY0p.qp_new(), reg_scy.GAVE_SCY0n.qn_new(), SIG_GND);
  /* p26.EMUX*/ auto _EMUX_TILE_Y1 = add3(lcd.reg_ly.MYRO_LY1p.qp_new(), reg_scy.FYMO_SCY1n.qn_new(), _FAFO_TILE_Y0.carry);
  /* p26.ECAB*/ auto _ECAB_TILE_Y2 = add3(lcd.reg_ly.LEXA_LY2p.qp_new(), reg_scy.FEZU_SCY2n.qn_new(), _EMUX_TILE_Y1.carry);
  /* p26.ETAM*/ auto _ETAM_MAP_Y0  = add3(lcd.reg_ly.LYDO_LY3p.qp_new(), reg_scy.FUJO_SCY3n.qn_new(), _ECAB_TILE_Y2.carry);
  /* p26.DOTO*/ auto _DOTO_MAP_Y1  = add3(lcd.reg_ly.LOVU_LY4p.qp_new(), reg_scy.DEDE_SCY4n.qn_new(), _ETAM_MAP_Y0.carry);
  /* p26.DABA*/ auto _DABA_MAP_Y2  = add3(lcd.reg_ly.LEMA_LY5p.qp_new(), reg_scy.FOTY_SCY5n.qn_new(), _DOTO_MAP_Y1.carry);
  /* p26.EFYK*/ auto _EFYK_MAP_Y3  = add3(lcd.reg_ly.MATO_LY6p.qp_new(), reg_scy.FOHA_SCY6n.qn_new(), _DABA_MAP_Y2.carry);
  /* p26.EJOK*/ auto _EJOK_MAP_Y4  = add3(lcd.reg_ly.LAFO_LY7p.qp_new(), reg_scy.FUNY_SCY7n.qn_new(), _EFYK_MAP_Y3.carry);

  return {
    _FAFO_TILE_Y0,
    _EMUX_TILE_Y1,
    _ECAB_TILE_Y2,
    _ETAM_MAP_Y0,
    _DOTO_MAP_Y1,
    _DABA_MAP_Y2,
    _EFYK_MAP_Y3,
    _EJOK_MAP_Y4
  };
}

//------------------------------------------------------------------------------------------------------------------------

SpritePix GateBoy::flip_sprite_pix(wire TEXY_SFETCHINGp, DFF8n BAXO_OAM_DB5p) {
  /*#p29.XONO*/ wire _XONO_FLIP_X_old = and2(BAXO_OAM_DB5p.qp_old(), TEXY_SFETCHINGp);
  /* p33.PUTE*/ wire _PUTE_FLIP0p = mux2p(_XONO_FLIP_X_old, new_bus.BUS_VRAM_D07p.qp_old(), new_bus.BUS_VRAM_D00p.qp_old());
  /* p33.PELO*/ wire _PELO_FLIP1p = mux2p(_XONO_FLIP_X_old, new_bus.BUS_VRAM_D06p.qp_old(), new_bus.BUS_VRAM_D01p.qp_old());
  /* p33.PONO*/ wire _PONO_FLIP2p = mux2p(_XONO_FLIP_X_old, new_bus.BUS_VRAM_D05p.qp_old(), new_bus.BUS_VRAM_D02p.qp_old());
  /* p33.POBE*/ wire _POBE_FLIP3p = mux2p(_XONO_FLIP_X_old, new_bus.BUS_VRAM_D04p.qp_old(), new_bus.BUS_VRAM_D03p.qp_old());
  /* p33.PACY*/ wire _PACY_FLIP4p = mux2p(_XONO_FLIP_X_old, new_bus.BUS_VRAM_D03p.qp_old(), new_bus.BUS_VRAM_D04p.qp_old());
  /* p33.PUGU*/ wire _PUGU_FLIP5p = mux2p(_XONO_FLIP_X_old, new_bus.BUS_VRAM_D02p.qp_old(), new_bus.BUS_VRAM_D05p.qp_old());
  /* p33.PAWE*/ wire _PAWE_FLIP6p = mux2p(_XONO_FLIP_X_old, new_bus.BUS_VRAM_D01p.qp_old(), new_bus.BUS_VRAM_D06p.qp_old());
  /* p33.PULY*/ wire _PULY_FLIP7p = mux2p(_XONO_FLIP_X_old, new_bus.BUS_VRAM_D00p.qp_old(), new_bus.BUS_VRAM_D07p.qp_old());

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
  /*#p21.SADU*/ wire _SADU_STAT_MODE0n = nor2(XYMU_RENDERINGn.qn_new(), PARU_VBLANKp());   // die NOR
  /*#p21.XATY*/ wire _XATY_STAT_MODE1n = nor2(ACYL_SCANNINGp(), XYMU_RENDERINGn.qn_new()); // die NOR
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
  /* p21.ROXE*/ reg_stat.ROXE_STAT_HBI_ENn.dff9(_RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p21.RUFO*/ reg_stat.RUFO_STAT_VBI_ENn.dff9(_RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p21.REFE*/ reg_stat.REFE_STAT_OAI_ENn.dff9(_RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p21.RUGU*/ reg_stat.RUGU_STAT_LYI_ENn.dff9(_RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
}

void GateBoy::reg_stat_tock()
{
  // FIXME polarity doesn't seem right, are we sure that RUPO is a nor latch and these signals are correct?
  /* p21.SEPA*/ wire _SEPA_FF41_WRp = and2(CUPA_CPU_WRp(), new_bus.VARY_FF41p());
  /* p21.RYJU*/ wire _RYJU_FF41_WRn = not1(_SEPA_FF41_WRp);
  /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = or2(WESY_SYS_RSTn(), _RYJU_FF41_WRn);
  /* p21.RUPO*/ reg_stat.RUPO_LYC_MATCHn.nor_latch(_PAGO_LYC_MATCH_RST, lcd.reg_lyc.ROPO_LY_MATCH_SYNCp.qp_new());
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
  /* p23.GAVE*/ reg_scy.GAVE_SCY0n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D00p.qp_old());
  /* p23.FYMO*/ reg_scy.FYMO_SCY1n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D01p.qp_old());
  /* p23.FEZU*/ reg_scy.FEZU_SCY2n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D02p.qp_old());
  /* p23.FUJO*/ reg_scy.FUJO_SCY3n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D03p.qp_old());
  /* p23.DEDE*/ reg_scy.DEDE_SCY4n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D04p.qp_old());
  /* p23.FOTY*/ reg_scy.FOTY_SCY5n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D05p.qp_old());
  /* p23.FOHA*/ reg_scy.FOHA_SCY6n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D06p.qp_old());
  /* p23.FUNY*/ reg_scy.FUNY_SCY7n.dff9(_CAVO_FF42_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D07p.qp_old());
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
  /* p23.DATY*/ reg_scx.DATY_SCX0n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D00p.qp_old());
  /* p23.DUZU*/ reg_scx.DUZU_SCX1n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D01p.qp_old());
  /* p23.CYXU*/ reg_scx.CYXU_SCX2n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D02p.qp_old());
  /* p23.GUBO*/ reg_scx.GUBO_SCX3n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D03p.qp_old());
  /* p23.BEMY*/ reg_scx.BEMY_SCX4n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D04p.qp_old());
  /* p23.CUZY*/ reg_scx.CUZY_SCX5n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D05p.qp_old());
  /* p23.CABU*/ reg_scx.CABU_SCX6n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D06p.qp_old());
  /* p23.BAKE*/ reg_scx.BAKE_SCX7n.dff9(_AMUN_FF43_WRn, CUNU_SYS_RSTn(), new_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_ly_read()
{
  /* p23.WAFU*/ wire _WAFU_FF44_RDp = and2(ASOT_CPU_RDp(), new_bus.XOGY_FF44p());
  /* p23.VARO*/ wire _VARO_FF44_RDn = not1(_WAFU_FF44_RDp);

  /*#p23.WURY*/ wire _WURY_LY0n = not1(lcd.reg_ly.MUWY_LY0p.qp_new());
  /* p23.XEPO*/ wire _XEPO_LY1n = not1(lcd.reg_ly.MYRO_LY1p.qp_new());
  /* p23.MYFA*/ wire _MYFA_LY2n = not1(lcd.reg_ly.LEXA_LY2p.qp_new());
  /* p23.XUHY*/ wire _XUHY_LY3n = not1(lcd.reg_ly.LYDO_LY3p.qp_new());
  /* p23.WATA*/ wire _WATA_LY4n = not1(lcd.reg_ly.LOVU_LY4p.qp_new());
  /* p23.XAGA*/ wire _XAGA_LY5n = not1(lcd.reg_ly.LEMA_LY5p.qp_new());
  /* p23.XUCE*/ wire _XUCE_LY6n = not1(lcd.reg_ly.MATO_LY6p.qp_new());
  /* p23.XOWO*/ wire _XOWO_LY7n = not1(lcd.reg_ly.LAFO_LY7p.qp_new());

  /* p23.VEGA_LY0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_VARO_FF44_RDn, _WURY_LY0n);
  /* p23.WUVA_LY1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_VARO_FF44_RDn, _XEPO_LY1n);
  /* p23.LYCO_LY2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_VARO_FF44_RDn, _MYFA_LY2n);
  /* p23.WOJY_LY3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_VARO_FF44_RDn, _XUHY_LY3n);
  /* p23.VYNE_LY4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_VARO_FF44_RDn, _WATA_LY4n);
  /* p23.WAMA_LY5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_VARO_FF44_RDn, _XAGA_LY5n);
  /* p23.WAVO_LY6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_VARO_FF44_RDn, _XUCE_LY6n);
  /* p23.WEZE_LY7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_VARO_FF44_RDn, _XOWO_LY7n);
}

void GateBoy::reg_ly_tock2()
{
  (void)new_bus;
  /*#p21.NOKO*/ wire _NOKO_y153p_old = and4(lcd.reg_ly.LAFO_LY7p.qp_old(), lcd.reg_ly.LOVU_LY4p.qp_old(), lcd.reg_ly.LYDO_LY3p.qp_old(), lcd.reg_ly.MUWY_LY0p.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
  /*#p21.MYTA*/ lcd.reg_ly.MYTA_y153p.dff17(lcd.reg_lx.NYPE_x113p.qp_new(), LYFE_VID_RSTn(), _NOKO_y153p_old);

  /*#p21.LAMA*/ wire _LAMA_Y_RSTn = nor2(lcd.reg_ly.MYTA_y153p.qp_new(), LYHA_VID_RSTp());
  /*#p21.MUWY*/ lcd.reg_ly.MUWY_LY0p.dff17(lcd.reg_lx.RUTU_x113p.qp_new(), _LAMA_Y_RSTn, lcd.reg_ly.MUWY_LY0p.qn_old());
  /*#p21.MYRO*/ lcd.reg_ly.MYRO_LY1p.dff17(lcd.reg_ly.MUWY_LY0p.qn_new(),  _LAMA_Y_RSTn, lcd.reg_ly.MYRO_LY1p.qn_old());
  /*#p21.LEXA*/ lcd.reg_ly.LEXA_LY2p.dff17(lcd.reg_ly.MYRO_LY1p.qn_new(),  _LAMA_Y_RSTn, lcd.reg_ly.LEXA_LY2p.qn_old());
  /*#p21.LYDO*/ lcd.reg_ly.LYDO_LY3p.dff17(lcd.reg_ly.LEXA_LY2p.qn_new(),  _LAMA_Y_RSTn, lcd.reg_ly.LYDO_LY3p.qn_old());
  /*#p21.LOVU*/ lcd.reg_ly.LOVU_LY4p.dff17(lcd.reg_ly.LYDO_LY3p.qn_new(),  _LAMA_Y_RSTn, lcd.reg_ly.LOVU_LY4p.qn_old());
  /*#p21.LEMA*/ lcd.reg_ly.LEMA_LY5p.dff17(lcd.reg_ly.LOVU_LY4p.qn_new(),  _LAMA_Y_RSTn, lcd.reg_ly.LEMA_LY5p.qn_old());
  /*#p21.MATO*/ lcd.reg_ly.MATO_LY6p.dff17(lcd.reg_ly.LEMA_LY5p.qn_new(),  _LAMA_Y_RSTn, lcd.reg_ly.MATO_LY6p.qn_old());
  /*#p21.LAFO*/ lcd.reg_ly.LAFO_LY7p.dff17(lcd.reg_ly.MATO_LY6p.qn_new(),  _LAMA_Y_RSTn, lcd.reg_ly.LAFO_LY7p.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------

wire GateBoy::TEGY_STROBE() const {

  /*#p21.TOCU*/ wire _TOCU_LX0n = not1(lcd.reg_lx.SAXO_LX0p.qp_new());
  /*#p21.VEPE*/ wire _VEPE_LX1n = not1(lcd.reg_lx.TYPO_LX1p.qp_new());
  /* p21.VUTY*/ wire _VUTY_LX2n = not1(lcd.reg_lx.VYZO_LX2p.qp_new());
  /* p21.VATE*/ wire _VATE_LX3n = not1(lcd.reg_lx.TELU_LX3p.qp_new());
  /* p21.TUDA*/ wire _TUDA_LX4n = not1(lcd.reg_lx.SUDE_LX4p.qp_new());
  /* p21.TAFY*/ wire _TAFY_LX5n = not1(lcd.reg_lx.TAHA_LX5p.qp_new());
  /* p21.TUJU*/ wire _TUJU_LX6n = not1(lcd.reg_lx.TYRY_LX6p.qp_new());

  /* p21.VOKU*/ wire _VOKU_LX000n = nand7(_TUJU_LX6n,         _TAFY_LX5n,         _TUDA_LX4n,         _VATE_LX3n,         _VUTY_LX2n,         _VEPE_LX1n,         _TOCU_LX0n); // 0000000 == 0
  /* p21.TOZU*/ wire _TOZU_LX007n = nand7(_TUJU_LX6n,         _TAFY_LX5n,         _TUDA_LX4n,         _VATE_LX3n,         lcd.reg_lx.VYZO_LX2p.qp_new(), lcd.reg_lx.TYPO_LX1p.qp_new(), lcd.reg_lx.SAXO_LX0p.qp_new()); // 0000111 == 7
  /* p21.TECE*/ wire _TECE_LX045n = nand7(_TUJU_LX6n,         lcd.reg_lx.TAHA_LX5p.qp_new(), _TUDA_LX4n,         lcd.reg_lx.TELU_LX3p.qp_new(), lcd.reg_lx.VYZO_LX2p.qp_new(), _VEPE_LX1n,         lcd.reg_lx.SAXO_LX0p.qp_new()); // 0101101 == 45
  /*#p21.TEBO*/ wire _TEBO_LX083n = nand7(lcd.reg_lx.TYRY_LX6p.qp_new(), _TAFY_LX5n,         lcd.reg_lx.SUDE_LX4p.qp_new(), _VATE_LX3n,         _VUTY_LX2n,         lcd.reg_lx.TYPO_LX1p.qp_new(), lcd.reg_lx.SAXO_LX0p.qp_new()); // 1010011 == 83

  /*#p21.TEGY*/ wire _TEGY_STROBE = nand4(_VOKU_LX000n,       _TOZU_LX007n,       _TECE_LX045n, _TEBO_LX083n);

  return _TEGY_STROBE;
}

void GateBoy::reg_lx_tock() {
  /*#p21.SANU*/ wire _SANU_x113p_old = and4(lcd.reg_lx.TYRY_LX6p.qp_old(), lcd.reg_lx.TAHA_LX5p.qp_old(), lcd.reg_lx.SUDE_LX4p.qp_old(), lcd.reg_lx.SAXO_LX0p.qp_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
  /*#p21.NYPE*/ lcd.reg_lx.NYPE_x113p.dff17(TALU_xxCDEFxx(), LYFE_VID_RSTn(), lcd.reg_lx.RUTU_x113p.qp_old());
  /*#p21.RUTU*/ lcd.reg_lx.RUTU_x113p.dff17(SONO_ABxxxxGH(), LYFE_VID_RSTn(), _SANU_x113p_old);

  /*#p21.MUDE*/ wire _MUDE_X_RSTn = nor2(lcd.reg_lx.RUTU_x113p.qp_new(), LYHA_VID_RSTp());
  /*#p21.SAXO*/ lcd.reg_lx.SAXO_LX0p.dff17(TALU_xxCDEFxx(),           _MUDE_X_RSTn, lcd.reg_lx.SAXO_LX0p.qn_old());
  /*#p21.TYPO*/ lcd.reg_lx.TYPO_LX1p.dff17(lcd.reg_lx.SAXO_LX0p.qn_new(), _MUDE_X_RSTn, lcd.reg_lx.TYPO_LX1p.qn_old());
  /*#p21.VYZO*/ lcd.reg_lx.VYZO_LX2p.dff17(lcd.reg_lx.TYPO_LX1p.qn_new(), _MUDE_X_RSTn, lcd.reg_lx.VYZO_LX2p.qn_old());
  /*#p21.TELU*/ lcd.reg_lx.TELU_LX3p.dff17(lcd.reg_lx.VYZO_LX2p.qn_new(), _MUDE_X_RSTn, lcd.reg_lx.TELU_LX3p.qn_old());
  /*#p21.SUDE*/ lcd.reg_lx.SUDE_LX4p.dff17(lcd.reg_lx.TELU_LX3p.qn_new(), _MUDE_X_RSTn, lcd.reg_lx.SUDE_LX4p.qn_old());
  /*#p21.TAHA*/ lcd.reg_lx.TAHA_LX5p.dff17(lcd.reg_lx.SUDE_LX4p.qn_new(), _MUDE_X_RSTn, lcd.reg_lx.TAHA_LX5p.qn_old());
  /*#p21.TYRY*/ lcd.reg_lx.TYRY_LX6p.dff17(lcd.reg_lx.TAHA_LX5p.qn_new(), _MUDE_X_RSTn, lcd.reg_lx.TYRY_LX6p.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_lyc_read()
{
  /* p23.XYLY*/ wire _XYLY_FF45_RDp = and2(ASOT_CPU_RDp(), new_bus.XAYU_FF45p());
  /* p23.WEKU*/ wire _WEKU_FF45_RDn = not1(_XYLY_FF45_RDp);
  /*#p23.RETU_LYC0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_WEKU_FF45_RDn, lcd.reg_lyc.SYRY_LYC0n.qp_new());
  /* p23.VOJO_LYC1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_WEKU_FF45_RDn, lcd.reg_lyc.VUCE_LYC1n.qp_new());
  /* p23.RAZU_LYC2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_WEKU_FF45_RDn, lcd.reg_lyc.SEDY_LYC2n.qp_new());
  /* p23.REDY_LYC3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_WEKU_FF45_RDn, lcd.reg_lyc.SALO_LYC3n.qp_new());
  /* p23.RACE_LYC4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_WEKU_FF45_RDn, lcd.reg_lyc.SOTA_LYC4n.qp_new());
  /*#p23.VAZU_LYC5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_WEKU_FF45_RDn, lcd.reg_lyc.VAFA_LYC5n.qp_new());
  /* p23.VAFE_LYC6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_WEKU_FF45_RDn, lcd.reg_lyc.VEVO_LYC6n.qp_new());
  /* p23.PUFY_LYC7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_WEKU_FF45_RDn, lcd.reg_lyc.RAHA_LYC7n.qp_new());
}

void GateBoy::reg_lyc_write()
{
  /* p23.XUFA*/ wire _XUFA_FF45_WRn = and2(CUPA_CPU_WRp(), new_bus.XAYU_FF45p());
  /* p23.WANE*/ wire _WANE_FF45_WRp = not1(_XUFA_FF45_WRn);
  /* p23.SYRY*/ lcd.reg_lyc.SYRY_LYC0n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), new_bus.BUS_CPU_D00p.qp_old());
  /* p23.VUCE*/ lcd.reg_lyc.VUCE_LYC1n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), new_bus.BUS_CPU_D01p.qp_old());
  /* p23.SEDY*/ lcd.reg_lyc.SEDY_LYC2n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), new_bus.BUS_CPU_D02p.qp_old());
  /* p23.SALO*/ lcd.reg_lyc.SALO_LYC3n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), new_bus.BUS_CPU_D03p.qp_old());
  /* p23.SOTA*/ lcd.reg_lyc.SOTA_LYC4n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), new_bus.BUS_CPU_D04p.qp_old());
  /* p23.VAFA*/ lcd.reg_lyc.VAFA_LYC5n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), new_bus.BUS_CPU_D05p.qp_old());
  /* p23.VEVO*/ lcd.reg_lyc.VEVO_LYC6n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), new_bus.BUS_CPU_D06p.qp_old());
  /* p23.RAHA*/ lcd.reg_lyc.RAHA_LYC7n.dff9(_WANE_FF45_WRp, WESY_SYS_RSTn(), new_bus.BUS_CPU_D07p.qp_old());
}

void GateBoy::reg_lyc_tock2()
{
  // LYC matcher
  /* p21.RYME*/ wire _RYME_LY_MATCH0n_old = xor2(lcd.reg_ly.MUWY_LY0p.qp_old(), lcd.reg_lyc.SYRY_LYC0n.qn_old());
  /* p21.TYDE*/ wire _TYDE_LY_MATCH1n_old = xor2(lcd.reg_ly.MYRO_LY1p.qp_old(), lcd.reg_lyc.VUCE_LYC1n.qn_old());
  /* p21.REDA*/ wire _REDA_LY_MATCH2n_old = xor2(lcd.reg_ly.LEXA_LY2p.qp_old(), lcd.reg_lyc.SEDY_LYC2n.qn_old());
  /* p21.RASY*/ wire _RASY_LY_MATCH3n_old = xor2(lcd.reg_ly.LYDO_LY3p.qp_old(), lcd.reg_lyc.SALO_LYC3n.qn_old());
  /* p21.TYKU*/ wire _TYKU_LY_MATCH4n_old = xor2(lcd.reg_ly.LOVU_LY4p.qp_old(), lcd.reg_lyc.SOTA_LYC4n.qn_old());
  /* p21.TUCY*/ wire _TUCY_LY_MATCH5n_old = xor2(lcd.reg_ly.LEMA_LY5p.qp_old(), lcd.reg_lyc.VAFA_LYC5n.qn_old());
  /* p21.TERY*/ wire _TERY_LY_MATCH6n_old = xor2(lcd.reg_ly.MATO_LY6p.qp_old(), lcd.reg_lyc.VEVO_LYC6n.qn_old());
  /* p21.SYFU*/ wire _SYFU_LY_MATCH7n_old = xor2(lcd.reg_ly.LAFO_LY7p.qp_old(), lcd.reg_lyc.RAHA_LYC7n.qn_old());

  /*#p21.SOVU*/ wire _SOVU_LY_MATCHA_old = nor4 (_SYFU_LY_MATCH7n_old, _TERY_LY_MATCH6n_old, _TUCY_LY_MATCH5n_old, _TYKU_LY_MATCH4n_old);
  /*#p21.SUBO*/ wire _SUBO_LY_MATCHB_old = nor4 (_RASY_LY_MATCH3n_old, _REDA_LY_MATCH2n_old, _TYDE_LY_MATCH1n_old, _RYME_LY_MATCH0n_old);
  /*#p21.RAPE*/ wire _RAPE_LY_MATCHn_old = nand2(_SOVU_LY_MATCHA_old,  _SUBO_LY_MATCHB_old);
  /*#p21.PALY*/ wire _PALY_LY_MATCHa_old = not1 (_RAPE_LY_MATCHn_old);

  /*#p21.ROPO*/ lcd.reg_lyc.ROPO_LY_MATCH_SYNCp.dff17(TALU_xxCDEFxx(), WESY_SYS_RSTn(), _PALY_LY_MATCHa_old);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_bgp_read()
{
  /* p36.VUSO*/ wire _VUSO_FF47_RDp = and2(ASOT_CPU_RDp(), new_bus.WERA_FF47p());
  /* p36.TEPY*/ wire _TEPY_FF47_RDn = not1(_VUSO_FF47_RDp);

  /*#p36.RARO_BGP0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_TEPY_FF47_RDn, pix_pipes.reg_bgp.PAVO_BGP_D0n.qp_new());
  /* p36.PABA_BGP1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_TEPY_FF47_RDn, pix_pipes.reg_bgp.NUSY_BGP_D1n.qp_new());
  /* p36.REDO_BGP2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_TEPY_FF47_RDn, pix_pipes.reg_bgp.PYLU_BGP_D2n.qp_new());
  /* p36.LOBE_BGP3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_TEPY_FF47_RDn, pix_pipes.reg_bgp.MAXY_BGP_D3n.qp_new());
  /* p36.LACE_BGP4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_TEPY_FF47_RDn, pix_pipes.reg_bgp.MUKE_BGP_D4n.qp_new());
  /* p36.LYKA_BGP5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_TEPY_FF47_RDn, pix_pipes.reg_bgp.MORU_BGP_D5n.qp_new());
  /* p36.LODY_BGP6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_TEPY_FF47_RDn, pix_pipes.reg_bgp.MOGY_BGP_D6n.qp_new());
  /* p36.LARY_BGP7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_TEPY_FF47_RDn, pix_pipes.reg_bgp.MENA_BGP_D7n.qp_new());
}

void GateBoy::reg_bgp_write()
{
  /* p36.VELY*/ wire _VELY_FF47_WRp = and2(CUPA_CPU_WRp(), new_bus.WERA_FF47p());
  /* p36.TEPO*/ wire _TEPO_FF47_WRp = not1(_VELY_FF47_WRp);

  /*#p36.PAVO*/ pix_pipes.reg_bgp.PAVO_BGP_D0n.dff8p(_TEPO_FF47_WRp, new_bus.BUS_CPU_D00p.qp_old());
  /* p36.NUSY*/ pix_pipes.reg_bgp.NUSY_BGP_D1n.dff8p(_TEPO_FF47_WRp, new_bus.BUS_CPU_D01p.qp_old());
  /* p36.PYLU*/ pix_pipes.reg_bgp.PYLU_BGP_D2n.dff8p(_TEPO_FF47_WRp, new_bus.BUS_CPU_D02p.qp_old());
  /* p36.MAXY*/ pix_pipes.reg_bgp.MAXY_BGP_D3n.dff8p(_TEPO_FF47_WRp, new_bus.BUS_CPU_D03p.qp_old());
  /* p36.MUKE*/ pix_pipes.reg_bgp.MUKE_BGP_D4n.dff8p(_TEPO_FF47_WRp, new_bus.BUS_CPU_D04p.qp_old());
  /* p36.MORU*/ pix_pipes.reg_bgp.MORU_BGP_D5n.dff8p(_TEPO_FF47_WRp, new_bus.BUS_CPU_D05p.qp_old());
  /* p36.MOGY*/ pix_pipes.reg_bgp.MOGY_BGP_D6n.dff8p(_TEPO_FF47_WRp, new_bus.BUS_CPU_D06p.qp_old());
  /* p36.MENA*/ pix_pipes.reg_bgp.MENA_BGP_D7n.dff8p(_TEPO_FF47_WRp, new_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_obp0_read()
{
  // FF48 OBP0
  /* p36.XUFY*/ wire _XUFY_FF48_RDp = and2(ASOT_CPU_RDp(), new_bus.XAYO_FF48p());
  /* p36.XOZY*/ wire _XOZY_FF48_RDn = not1(_XUFY_FF48_RDp);

  /*#p36.XARY_OBP00_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_XOZY_FF48_RDn, pix_pipes.reg_obp0.XUFU_OBP0_D0n.qp_new());
  /* p36.XOKE_OBP01_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_XOZY_FF48_RDn, pix_pipes.reg_obp0.XUKY_OBP0_D1n.qp_new());
  /* p36.XUNO_OBP02_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_XOZY_FF48_RDn, pix_pipes.reg_obp0.XOVA_OBP0_D2n.qp_new());
  /* p36.XUBY_OBP03_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_XOZY_FF48_RDn, pix_pipes.reg_obp0.XALO_OBP0_D3n.qp_new());
  /* p36.XAJU_OBP04_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_XOZY_FF48_RDn, pix_pipes.reg_obp0.XERU_OBP0_D4n.qp_new());
  /* p36.XOBO_OBP05_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_XOZY_FF48_RDn, pix_pipes.reg_obp0.XYZE_OBP0_D5n.qp_new());
  /* p36.XAXA_OBP06_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_XOZY_FF48_RDn, pix_pipes.reg_obp0.XUPO_OBP0_D6n.qp_new());
  /* p36.XAWO_OBP07_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_XOZY_FF48_RDn, pix_pipes.reg_obp0.XANA_OBP0_D7n.qp_new());
}

void GateBoy::reg_obp0_write()
{
  // FF48 OBP0
  /* p36.XOMA*/ wire _XOMA_FF48_WRp = and2(CUPA_CPU_WRp(), new_bus.XAYO_FF48p());
  /* p36.XELO*/ wire _XELO_FF48_WRn = not1(_XOMA_FF48_WRp);

  /* p36.XUFU*/ pix_pipes.reg_obp0.XUFU_OBP0_D0n.dff8p(_XELO_FF48_WRn, new_bus.BUS_CPU_D00p.qp_old());
  /* p36.XUKY*/ pix_pipes.reg_obp0.XUKY_OBP0_D1n.dff8p(_XELO_FF48_WRn, new_bus.BUS_CPU_D01p.qp_old());
  /* p36.XOVA*/ pix_pipes.reg_obp0.XOVA_OBP0_D2n.dff8p(_XELO_FF48_WRn, new_bus.BUS_CPU_D02p.qp_old());
  /* p36.XALO*/ pix_pipes.reg_obp0.XALO_OBP0_D3n.dff8p(_XELO_FF48_WRn, new_bus.BUS_CPU_D03p.qp_old());
  /* p36.XERU*/ pix_pipes.reg_obp0.XERU_OBP0_D4n.dff8p(_XELO_FF48_WRn, new_bus.BUS_CPU_D04p.qp_old());
  /* p36.XYZE*/ pix_pipes.reg_obp0.XYZE_OBP0_D5n.dff8p(_XELO_FF48_WRn, new_bus.BUS_CPU_D05p.qp_old());
  /* p36.XUPO*/ pix_pipes.reg_obp0.XUPO_OBP0_D6n.dff8p(_XELO_FF48_WRn, new_bus.BUS_CPU_D06p.qp_old());
  /* p36.XANA*/ pix_pipes.reg_obp0.XANA_OBP0_D7n.dff8p(_XELO_FF48_WRn, new_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_obp1_read()
{
  /* p36.MUMY*/ wire _MUMY_FF49_RDp = and2(ASOT_CPU_RDp(), new_bus.TEGO_FF49p());
  /* p36.LOTE*/ wire _LOTE_FF49_RDn = not1(_MUMY_FF49_RDp);

  /*#p36.LAJU_OBP10_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_LOTE_FF49_RDn, pix_pipes.reg_obp1.MOXY_OBP1_D0n.qp_new());
  /* p36.LEPA_OBP11_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_LOTE_FF49_RDn, pix_pipes.reg_obp1.LAWO_OBP1_D1n.qp_new());
  /* p36.LODE_OBP12_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_LOTE_FF49_RDn, pix_pipes.reg_obp1.MOSA_OBP1_D2n.qp_new());
  /* p36.LYZA_OBP13_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_LOTE_FF49_RDn, pix_pipes.reg_obp1.LOSE_OBP1_D3n.qp_new());
  /* p36.LUKY_OBP14_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_LOTE_FF49_RDn, pix_pipes.reg_obp1.LUNE_OBP1_D4n.qp_new());
  /* p36.LUGA_OBP15_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_LOTE_FF49_RDn, pix_pipes.reg_obp1.LUGU_OBP1_D5n.qp_new());
  /* p36.LEBA_OBP16_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_LOTE_FF49_RDn, pix_pipes.reg_obp1.LEPU_OBP1_D6n.qp_new());
  /* p36.LELU_OBP17_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_LOTE_FF49_RDn, pix_pipes.reg_obp1.LUXO_OBP1_D7n.qp_new());
}

void GateBoy::reg_obp1_write()
{
  /* p36.MYXE*/ wire _MYXE_FF49_WRp = and2(CUPA_CPU_WRp(), new_bus.TEGO_FF49p());
  /* p36.LEHO*/ wire _LEHO_FF49_WRn = not1(_MYXE_FF49_WRp);

  /* p36.MOXY*/ pix_pipes.reg_obp1.MOXY_OBP1_D0n.dff8p(_LEHO_FF49_WRn, new_bus.BUS_CPU_D00p.qp_old());
  /* p36.LAWO*/ pix_pipes.reg_obp1.LAWO_OBP1_D1n.dff8p(_LEHO_FF49_WRn, new_bus.BUS_CPU_D01p.qp_old());
  /* p36.MOSA*/ pix_pipes.reg_obp1.MOSA_OBP1_D2n.dff8p(_LEHO_FF49_WRn, new_bus.BUS_CPU_D02p.qp_old());
  /* p36.LOSE*/ pix_pipes.reg_obp1.LOSE_OBP1_D3n.dff8p(_LEHO_FF49_WRn, new_bus.BUS_CPU_D03p.qp_old());
  /* p36.LUNE*/ pix_pipes.reg_obp1.LUNE_OBP1_D4n.dff8p(_LEHO_FF49_WRn, new_bus.BUS_CPU_D04p.qp_old());
  /* p36.LUGU*/ pix_pipes.reg_obp1.LUGU_OBP1_D5n.dff8p(_LEHO_FF49_WRn, new_bus.BUS_CPU_D05p.qp_old());
  /* p36.LEPU*/ pix_pipes.reg_obp1.LEPU_OBP1_D6n.dff8p(_LEHO_FF49_WRn, new_bus.BUS_CPU_D06p.qp_old());
  /* p36.LUXO*/ pix_pipes.reg_obp1.LUXO_OBP1_D7n.dff8p(_LEHO_FF49_WRn, new_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_wy_write()
{
  /* p23.WEKO*/ wire _WEKO_FF4A_WRp = and2(CUPA_CPU_WRp(), new_bus.VYGA_FF4Ap());
  /* p23.VEFU*/ wire _VEFU_FF4A_WRn = not1(_WEKO_FF4A_WRp);
  /* p23.NESO*/ reg_wy.NESO_WY0n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D00p.qp_old());
  /* p23.NYRO*/ reg_wy.NYRO_WY1n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D01p.qp_old());
  /* p23.NAGA*/ reg_wy.NAGA_WY2n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D02p.qp_old());
  /* p23.MELA*/ reg_wy.MELA_WY3n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D03p.qp_old());
  /* p23.NULO*/ reg_wy.NULO_WY4n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D04p.qp_old());
  /* p23.NENE*/ reg_wy.NENE_WY5n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D05p.qp_old());
  /* p23.NUKA*/ reg_wy.NUKA_WY6n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D06p.qp_old());
  /* p23.NAFU*/ reg_wy.NAFU_WY7n.dff9(_VEFU_FF4A_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D07p.qp_old());
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
  /* p23.MYPA*/ reg_wx.MYPA_WX0n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D00p.qp_old());
  /* p23.NOFE*/ reg_wx.NOFE_WX1n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D01p.qp_old());
  /* p23.NOKE*/ reg_wx.NOKE_WX2n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D02p.qp_old());
  /* p23.MEBY*/ reg_wx.MEBY_WX3n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D03p.qp_old());
  /* p23.MYPU*/ reg_wx.MYPU_WX4n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D04p.qp_old());
  /* p23.MYCE*/ reg_wx.MYCE_WX5n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D05p.qp_old());
  /* p23.MUVO*/ reg_wx.MUVO_WX6n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D06p.qp_old());
  /* p23.NUKU*/ reg_wx.NUKU_WX7n.dff9(_VOXU_FF4B_WRn, WALU_SYS_RSTn(), new_bus.BUS_CPU_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::store_tile_temp_a()
{
  /* p32.LEGU*/ tile_fetcher.tile_temp_a.LEGU_TILE_DA0n.dff8p(LOMA_LATCH_TILE_DAn(), new_bus.BUS_VRAM_D00p.qp_old());
  /* p32.NUDU*/ tile_fetcher.tile_temp_a.NUDU_TILE_DA1n.dff8p(LOMA_LATCH_TILE_DAn(), new_bus.BUS_VRAM_D01p.qp_old());
  /* p32.MUKU*/ tile_fetcher.tile_temp_a.MUKU_TILE_DA2n.dff8p(LOMA_LATCH_TILE_DAn(), new_bus.BUS_VRAM_D02p.qp_old());
  /* p32.LUZO*/ tile_fetcher.tile_temp_a.LUZO_TILE_DA3n.dff8p(LOMA_LATCH_TILE_DAn(), new_bus.BUS_VRAM_D03p.qp_old());
  /* p32.MEGU*/ tile_fetcher.tile_temp_a.MEGU_TILE_DA4n.dff8p(LOMA_LATCH_TILE_DAn(), new_bus.BUS_VRAM_D04p.qp_old());
  /* p32.MYJY*/ tile_fetcher.tile_temp_a.MYJY_TILE_DA5n.dff8p(LOMA_LATCH_TILE_DAn(), new_bus.BUS_VRAM_D05p.qp_old());
  /* p32.NASA*/ tile_fetcher.tile_temp_a.NASA_TILE_DA6n.dff8p(LOMA_LATCH_TILE_DAn(), new_bus.BUS_VRAM_D06p.qp_old());
  /* p32.NEFO*/ tile_fetcher.tile_temp_a.NEFO_TILE_DA7n.dff8p(LOMA_LATCH_TILE_DAn(), new_bus.BUS_VRAM_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::store_tile_temp_b()
{
  SigIn SIG_VCC = 1;
  // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
  /* p32.RAWU*/ tile_fetcher.tile_temp_b.RAWU_TILE_DB0p.dff11(LABU_LATCH_TILE_DBn(), SIG_VCC, new_bus.BUS_VRAM_D00p.qp_old());
  /* p32.POZO*/ tile_fetcher.tile_temp_b.POZO_TILE_DB1p.dff11(LABU_LATCH_TILE_DBn(), SIG_VCC, new_bus.BUS_VRAM_D01p.qp_old());
  /* p32.PYZO*/ tile_fetcher.tile_temp_b.PYZO_TILE_DB2p.dff11(LABU_LATCH_TILE_DBn(), SIG_VCC, new_bus.BUS_VRAM_D02p.qp_old());
  /* p32.POXA*/ tile_fetcher.tile_temp_b.POXA_TILE_DB3p.dff11(LABU_LATCH_TILE_DBn(), SIG_VCC, new_bus.BUS_VRAM_D03p.qp_old());
  /* p32.PULO*/ tile_fetcher.tile_temp_b.PULO_TILE_DB4p.dff11(LABU_LATCH_TILE_DBn(), SIG_VCC, new_bus.BUS_VRAM_D04p.qp_old());
  /* p32.POJU*/ tile_fetcher.tile_temp_b.POJU_TILE_DB5p.dff11(LABU_LATCH_TILE_DBn(), SIG_VCC, new_bus.BUS_VRAM_D05p.qp_old());
  /* p32.POWY*/ tile_fetcher.tile_temp_b.POWY_TILE_DB6p.dff11(LABU_LATCH_TILE_DBn(), SIG_VCC, new_bus.BUS_VRAM_D06p.qp_old());
  /* p32.PYJU*/ tile_fetcher.tile_temp_b.PYJU_TILE_DB7p.dff11(LABU_LATCH_TILE_DBn(), SIG_VCC, new_bus.BUS_VRAM_D07p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::store_sprite_pix_a(SpritePix sprite_pix_old) {
  /* p33.REWO*/ sprite_fetcher.sprite_pix_a.REWO_SPRITE_DA0n.dff8n(XADO_STORE_SPRITE_An(), sprite_pix_old.PUTE_FLIP0p);
  /* p33.PEBA*/ sprite_fetcher.sprite_pix_a.PEBA_SPRITE_DA1n.dff8n(XADO_STORE_SPRITE_An(), sprite_pix_old.PELO_FLIP1p);
  /* p33.MOFO*/ sprite_fetcher.sprite_pix_a.MOFO_SPRITE_DA2n.dff8n(XADO_STORE_SPRITE_An(), sprite_pix_old.PONO_FLIP2p);
  /* p33.PUDU*/ sprite_fetcher.sprite_pix_a.PUDU_SPRITE_DA3n.dff8n(XADO_STORE_SPRITE_An(), sprite_pix_old.POBE_FLIP3p);
  /* p33.SAJA*/ sprite_fetcher.sprite_pix_a.SAJA_SPRITE_DA4n.dff8n(XADO_STORE_SPRITE_An(), sprite_pix_old.PACY_FLIP4p);
  /* p33.SUNY*/ sprite_fetcher.sprite_pix_a.SUNY_SPRITE_DA5n.dff8n(XADO_STORE_SPRITE_An(), sprite_pix_old.PUGU_FLIP5p);
  /* p33.SEMO*/ sprite_fetcher.sprite_pix_a.SEMO_SPRITE_DA6n.dff8n(XADO_STORE_SPRITE_An(), sprite_pix_old.PAWE_FLIP6p);
  /* p33.SEGA*/ sprite_fetcher.sprite_pix_a.SEGA_SPRITE_DA7n.dff8n(XADO_STORE_SPRITE_An(), sprite_pix_old.PULY_FLIP7p);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::store_sprite_pix_b(SpritePix sprite_pix_old) {
  /* p33.PEFO*/ sprite_fetcher.sprite_pix_b.PEFO_SPRITE_DB0n.dff8n(PUCO_STORE_SPRITE_Bn(), sprite_pix_old.PUTE_FLIP0p);
  /* p33.ROKA*/ sprite_fetcher.sprite_pix_b.ROKA_SPRITE_DB1n.dff8n(PUCO_STORE_SPRITE_Bn(), sprite_pix_old.PELO_FLIP1p);
  /* p33.MYTU*/ sprite_fetcher.sprite_pix_b.MYTU_SPRITE_DB2n.dff8n(PUCO_STORE_SPRITE_Bn(), sprite_pix_old.PONO_FLIP2p);
  /* p33.RAMU*/ sprite_fetcher.sprite_pix_b.RAMU_SPRITE_DB3n.dff8n(PUCO_STORE_SPRITE_Bn(), sprite_pix_old.POBE_FLIP3p);
  /* p33.SELE*/ sprite_fetcher.sprite_pix_b.SELE_SPRITE_DB4n.dff8n(PUCO_STORE_SPRITE_Bn(), sprite_pix_old.PACY_FLIP4p);
  /* p33.SUTO*/ sprite_fetcher.sprite_pix_b.SUTO_SPRITE_DB5n.dff8n(PUCO_STORE_SPRITE_Bn(), sprite_pix_old.PUGU_FLIP5p);
  /* p33.RAMA*/ sprite_fetcher.sprite_pix_b.RAMA_SPRITE_DB6n.dff8n(PUCO_STORE_SPRITE_Bn(), sprite_pix_old.PAWE_FLIP6p);
  /* p33.RYDU*/ sprite_fetcher.sprite_pix_b.RYDU_SPRITE_DB7n.dff8n(PUCO_STORE_SPRITE_Bn(), sprite_pix_old.PULY_FLIP7p);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_win_map_x(wire TEVO_WIN_FETCH_TRIGp) {
  /* p27.VETU*/ wire _VETU_WIN_MAPp = and2(TEVO_WIN_FETCH_TRIGp, PORE_WIN_MODEp());
  /*#p27.XOFO*/ wire _XOFO_WIN_RSTp = nand3(reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn_new(), XAPO_VID_RSTn());
  /* p27.XACO*/ wire _XACO_WIN_RSTn = not1(_XOFO_WIN_RSTp);
  /* p27.WYKA*/ win_map_x.WYKA_WIN_X3.dff17(_VETU_WIN_MAPp,                 _XACO_WIN_RSTn, win_map_x.WYKA_WIN_X3.qn_old());
  /* p27.WODY*/ win_map_x.WODY_WIN_X4.dff17(win_map_x.WYKA_WIN_X3.qn_new(), _XACO_WIN_RSTn, win_map_x.WODY_WIN_X4.qn_old());
  /* p27.WOBO*/ win_map_x.WOBO_WIN_X5.dff17(win_map_x.WODY_WIN_X4.qn_new(), _XACO_WIN_RSTn, win_map_x.WOBO_WIN_X5.qn_old());
  /* p27.WYKO*/ win_map_x.WYKO_WIN_X6.dff17(win_map_x.WOBO_WIN_X5.qn_new(), _XACO_WIN_RSTn, win_map_x.WYKO_WIN_X6.qn_old());
  /* p27.XOLO*/ win_map_x.XOLO_WIN_X7.dff17(win_map_x.WYKO_WIN_X6.qn_new(), _XACO_WIN_RSTn, win_map_x.XOLO_WIN_X7.qn_old());

}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_win_map_y() {
  // Every time we leave win mode we increment win_y
  /* p27.WAZY*/ wire _WAZY_WIN_MODEn = not1(PORE_WIN_MODEp());
  /* p27.REPU*/ wire _REPU_VBLANKp   = or2(PARU_VBLANKp(), PYRY_VID_RSTp());
  /* p27.SYNY*/ wire _SYNY_VBLANKn   = not1(_REPU_VBLANKp);
  /* p27.VYNO*/ win_map_y.VYNO_WIN_Y0.dff17(_WAZY_WIN_MODEn,                _SYNY_VBLANKn, win_map_y.VYNO_WIN_Y0.qn_old());
  /* p27.VUJO*/ win_map_y.VUJO_WIN_Y1.dff17(win_map_y.VYNO_WIN_Y0.qn_new(), _SYNY_VBLANKn, win_map_y.VUJO_WIN_Y1.qn_old());
  /* p27.VYMU*/ win_map_y.VYMU_WIN_Y2.dff17(win_map_y.VUJO_WIN_Y1.qn_new(), _SYNY_VBLANKn, win_map_y.VYMU_WIN_Y2.qn_old());
  /* p27.TUFU*/ win_map_y.TUFU_WIN_Y3.dff17(win_map_y.VYMU_WIN_Y2.qn_new(), _SYNY_VBLANKn, win_map_y.TUFU_WIN_Y3.qn_old());
  /* p27.TAXA*/ win_map_y.TAXA_WIN_Y4.dff17(win_map_y.TUFU_WIN_Y3.qn_new(), _SYNY_VBLANKn, win_map_y.TAXA_WIN_Y4.qn_old());
  /* p27.TOZO*/ win_map_y.TOZO_WIN_Y5.dff17(win_map_y.TAXA_WIN_Y4.qn_new(), _SYNY_VBLANKn, win_map_y.TOZO_WIN_Y5.qn_old());
  /* p27.TATE*/ win_map_y.TATE_WIN_Y6.dff17(win_map_y.TOZO_WIN_Y5.qn_new(), _SYNY_VBLANKn, win_map_y.TATE_WIN_Y6.qn_old());
  /* p27.TEKE*/ win_map_y.TEKE_WIN_Y7.dff17(win_map_y.TATE_WIN_Y6.qn_new(), _SYNY_VBLANKn, win_map_y.TEKE_WIN_Y7.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------
