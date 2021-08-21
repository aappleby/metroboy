#include "GateBoyLib/GateBoySpriteStore.h"

#include "GateBoyLib/GateBoy.h"

#pragma warning(disable:4458)

//-----------------------------------------------------------------------------

void GateBoy::reset_sprite_store() {
  sprite_counter.DEZY_COUNT_CLKp.state    = 0b00011011;
  sprite_counter.BESE_SPRITE_COUNT0.state = 0b00011010;
  sprite_counter.CUXY_SPRITE_COUNT1.state = 0b00011010;
  sprite_counter.BEGO_SPRITE_COUNT2.state = 0b00011010;
  sprite_counter.DYBE_SPRITE_COUNT3.state = 0b00011010;
}

//-----------------------------------------------------------------------------
// Update sprite store reset signals to clear the first matched sprite when the
// sprite fetch is done.

void GateBoy::update_sprite_reset_flags(
  const Gate WUTY_SFETCH_DONE_TRIGp,
  const wire BYVA_LINE_RSTn,
  const SpriteMatchFlags& sprite_get_flag,
  SpriteResetFlags& sprite_reset_flags)
{
  ///* p28.ABAK*/ wire _ABAK_LINE_RSTp = or2(ATEJ_LINE_RSTp.qp_new(), AMYG_VID_RSTp);
  ///* p28.BYVA*/ wire _BYVA_LINE_RSTn = not1(_ABAK_LINE_RSTp);
  ///* p29.DYBA*/ wire _DYBA_LINE_RSTp = not1(_BYVA_LINE_RSTn);

  /* p29.EBOJ*/ sprite_reset_flags.EBOJ_STORE0_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.GUVA_SPRITE0_GETp.qp_old());
  /* p29.CEDY*/ sprite_reset_flags.CEDY_STORE1_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.ENUT_SPRITE1_GETp.qp_old());
  /* p29.EGAV*/ sprite_reset_flags.EGAV_STORE2_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.EMOL_SPRITE2_GETp.qp_old());
  /* p29.GOTA*/ sprite_reset_flags.GOTA_STORE3_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.GYFY_SPRITE3_GETp.qp_old());
  /* p29.XUDY*/ sprite_reset_flags.XUDY_STORE4_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.GONO_SPRITE4_GETp.qp_old());
  /* p29.WAFY*/ sprite_reset_flags.WAFY_STORE5_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.GEGA_SPRITE5_GETp.qp_old());
  /* p29.WOMY*/ sprite_reset_flags.WOMY_STORE6_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.XOJA_SPRITE6_GETp.qp_old());
  /* p29.WAPO*/ sprite_reset_flags.WAPO_STORE7_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.GUTU_SPRITE7_GETp.qp_old());
  /* p29.EXUQ*/ sprite_reset_flags.EXUQ_STORE8_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.FOXA_SPRITE8_GETp.qp_old());
  /* p29.FONO*/ sprite_reset_flags.FONO_STORE9_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), BYVA_LINE_RSTn, sprite_get_flag.GUZE_SPRITE9_GETp.qp_old());
}

//-----------------------------------------------------------------------------
// Turn the sprite counter into a one-hot clock signal.

void GateBoy::update_sprite_store_flags(
  const SpriteCounter& sprite_counter,
  const wire _DYTY_COUNT_CLKp,
  SpriteStoreFlags& sprite_store_flags)
{
  /* p29.EDEN*/ wire _EDEN_SPRITE_COUNT0n = not1(sprite_counter.BESE_SPRITE_COUNT0.qp_new());
  /* p29.CYPY*/ wire _CYPY_SPRITE_COUNT1n = not1(sprite_counter.CUXY_SPRITE_COUNT1.qp_new());
  /* p29.CAPE*/ wire _CAPE_SPRITE_COUNT2n = not1(sprite_counter.BEGO_SPRITE_COUNT2.qp_new());
  /* p29.CAXU*/ wire _CAXU_SPRITE_COUNT3n = not1(sprite_counter.DYBE_SPRITE_COUNT3.qp_new());

  /* p29.FYCU*/ wire _FYCU_SPRITE_COUNT0p = not1(_EDEN_SPRITE_COUNT0n);
  /* p29.FONE*/ wire _FONE_SPRITE_COUNT1p = not1(_CYPY_SPRITE_COUNT1n);
  /* p29.EKUD*/ wire _EKUD_SPRITE_COUNT2p = not1(_CAPE_SPRITE_COUNT2n);
  /* p29.ELYG*/ wire _ELYG_SPRITE_COUNT3p = not1(_CAXU_SPRITE_COUNT3n);

  /* p29.DEZO*/ wire _DEZO_STORE0_SELn = nand4(_EDEN_SPRITE_COUNT0n, _CYPY_SPRITE_COUNT1n, _CAPE_SPRITE_COUNT2n, _CAXU_SPRITE_COUNT3n);
  /* p29.CUVA*/ wire _CUVA_STORE1_SELn = nand4(_FYCU_SPRITE_COUNT0p, _CYPY_SPRITE_COUNT1n, _CAPE_SPRITE_COUNT2n, _CAXU_SPRITE_COUNT3n);
  /* p29.GEBU*/ wire _GEBU_STORE2_SELn = nand4(_EDEN_SPRITE_COUNT0n, _FONE_SPRITE_COUNT1p, _CAPE_SPRITE_COUNT2n, _CAXU_SPRITE_COUNT3n);
  /* p29.FOCO*/ wire _FOCO_STORE3_SELn = nand4(_FYCU_SPRITE_COUNT0p, _FONE_SPRITE_COUNT1p, _CAPE_SPRITE_COUNT2n, _CAXU_SPRITE_COUNT3n);
  /* p29.CUPE*/ wire _CUPE_STORE4_SELn = nand4(_EDEN_SPRITE_COUNT0n, _CYPY_SPRITE_COUNT1n, _EKUD_SPRITE_COUNT2p, _CAXU_SPRITE_COUNT3n);
  /* p29.CUGU*/ wire _CUGU_STORE5_SELn = nand4(_FYCU_SPRITE_COUNT0p, _CYPY_SPRITE_COUNT1n, _EKUD_SPRITE_COUNT2p, _CAXU_SPRITE_COUNT3n);
  /* p29.WOMU*/ wire _WOMU_STORE6_SELn = nand4(_EDEN_SPRITE_COUNT0n, _FONE_SPRITE_COUNT1p, _EKUD_SPRITE_COUNT2p, _CAXU_SPRITE_COUNT3n);
  /* p29.GUNA*/ wire _GUNA_STORE7_SELn = nand4(_FYCU_SPRITE_COUNT0p, _FONE_SPRITE_COUNT1p, _EKUD_SPRITE_COUNT2p, _CAXU_SPRITE_COUNT3n);
  /* p29.DEWY*/ wire _DEWY_STORE8_SELn = nand4(_EDEN_SPRITE_COUNT0n, _CYPY_SPRITE_COUNT1n, _CAPE_SPRITE_COUNT2n, _ELYG_SPRITE_COUNT3p);
  /* p29.DOGU*/ wire _DOGU_STORE9_SELn = nand4(_FYCU_SPRITE_COUNT0p, _CYPY_SPRITE_COUNT1n, _CAPE_SPRITE_COUNT2n, _ELYG_SPRITE_COUNT3p);

  /* p29.CEMY*/ wire _CEMY_STORE0_CLKp = or2(_DYTY_COUNT_CLKp, _DEZO_STORE0_SELn);
  /* p29.BYBY*/ wire _BYBY_STORE1_CLKp = or2(_DYTY_COUNT_CLKp, _CUVA_STORE1_SELn);
  /* p29.WYXO*/ wire _WYXO_STORE2_CLKp = or2(_DYTY_COUNT_CLKp, _GEBU_STORE2_SELn);
  /* p29.GUVE*/ wire _GUVE_STORE3_CLKp = or2(_DYTY_COUNT_CLKp, _FOCO_STORE3_SELn);
  /* p29.CECU*/ wire _CECU_STORE4_CLKp = or2(_DYTY_COUNT_CLKp, _CUPE_STORE4_SELn);
  /* p29.CADO*/ wire _CADO_STORE5_CLKp = or2(_DYTY_COUNT_CLKp, _CUGU_STORE5_SELn);
  /* p29.XUJO*/ wire _XUJO_STORE6_CLKp = or2(_DYTY_COUNT_CLKp, _WOMU_STORE6_SELn);
  /* p29.GAPE*/ wire _GAPE_STORE7_CLKp = or2(_DYTY_COUNT_CLKp, _GUNA_STORE7_SELn);
  /* p29.CAHO*/ wire _CAHO_STORE8_CLKp = or2(_DYTY_COUNT_CLKp, _DEWY_STORE8_SELn);
  /* p29.CATO*/ wire _CATO_STORE9_CLKp = or2(_DYTY_COUNT_CLKp, _DOGU_STORE9_SELn);

  /* p29.DYHU*/ sprite_store_flags.DYHU_STORE0_CLKn = not1(_CEMY_STORE0_CLKp);
  /* p29.BUCO*/ sprite_store_flags.BUCO_STORE1_CLKn = not1(_BYBY_STORE1_CLKp);
  /* p29.GYFO*/ sprite_store_flags.GYFO_STORE2_CLKn = not1(_WYXO_STORE2_CLKp);
  /* p29.GUSA*/ sprite_store_flags.GUSA_STORE3_CLKn = not1(_GUVE_STORE3_CLKp);
  /* p29.DUKE*/ sprite_store_flags.DUKE_STORE4_CLKn = not1(_CECU_STORE4_CLKp);
  /* p29.BEDE*/ sprite_store_flags.BEDE_STORE5_CLKn = not1(_CADO_STORE5_CLKp);
  /* p29.WEKA*/ sprite_store_flags.WEKA_STORE6_CLKn = not1(_XUJO_STORE6_CLKp);
  /* p29.GYVO*/ sprite_store_flags.GYVO_STORE7_CLKn = not1(_GAPE_STORE7_CLKp);
  /* p29.BUKA*/ sprite_store_flags.BUKA_STORE8_CLKn = not1(_CAHO_STORE8_CLKp);
  /* p29.DECU*/ sprite_store_flags.DECU_STORE9_CLKn = not1(_CATO_STORE9_CLKp);
}

//-----------------------------------------------------------------------------

void GateBoy::store_sprite(
  const SpriteStoreFlags& sprite_store_flags,
  const SpriteResetFlags& sprite_reset_flags,
  const wire BYVA_LINE_RSTn,
  const SpriteBus& sprite_bus,
  const OamTempB& oam_temp_b,
  GateBoySpriteStore& sprite_store)
{

  /* p29.FUXU*/ wire _FUXU_STORE0_CLKp = not1(sprite_store_flags.DYHU_STORE0_CLKn.qp_new());
  /* p29.ASYS*/ wire _ASYS_STORE1_CLKp = not1(sprite_store_flags.BUCO_STORE1_CLKn.qp_new());
  /* p29.CACU*/ wire _CACU_STORE2_CLKp = not1(sprite_store_flags.GYFO_STORE2_CLKn.qp_new());
  /* p29.YFAG*/ wire _YFAG_STORE3_CLKp = not1(sprite_store_flags.GUSA_STORE3_CLKn.qp_new());
  /* p29.WOFO*/ wire _WOFO_STORE4_CLKp = not1(sprite_store_flags.DUKE_STORE4_CLKn.qp_new());
  /* p29.CYLA*/ wire _CYLA_STORE5_CLKp = not1(sprite_store_flags.BEDE_STORE5_CLKn.qp_new());
  /* p29.ZAPE*/ wire _ZAPE_STORE6_CLKp = not1(sprite_store_flags.WEKA_STORE6_CLKn.qp_new());
  /* p29.GECY*/ wire _GECY_STORE7_CLKp = not1(sprite_store_flags.GYVO_STORE7_CLKn.qp_new());
  /* p29.CEXU*/ wire _CEXU_STORE8_CLKp = not1(sprite_store_flags.BUKA_STORE8_CLKn.qp_new());
  /* p29.WEME*/ wire _WEME_STORE9_CLKp = not1(sprite_store_flags.DECU_STORE9_CLKn.qp_new());

  /* p29.DYBA*/ wire _DYBA_LINE_RSTp = not1(BYVA_LINE_RSTn);
  /* p29.DYWE*/ wire _DYWE_STORE0_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.EBOJ_STORE0_RSTp_evn.qp_new());
  /* p29.EFEV*/ wire _EFEV_STORE1_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.CEDY_STORE1_RSTp_evn.qp_new());
  /* p29.FOKO*/ wire _FOKO_STORE2_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.EGAV_STORE2_RSTp_evn.qp_new());
  /* p29.GAKE*/ wire _GAKE_STORE3_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.GOTA_STORE3_RSTp_evn.qp_new());
  /* p29.WOHU*/ wire _WOHU_STORE4_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.XUDY_STORE4_RSTp_evn.qp_new());
  /* p29.FEVE*/ wire _FEVE_STORE5_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.WAFY_STORE5_RSTp_evn.qp_new());
  /* p29.WACY*/ wire _WACY_STORE6_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.WOMY_STORE6_RSTp_evn.qp_new());
  /* p29.GUKY*/ wire _GUKY_STORE7_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.WAPO_STORE7_RSTp_evn.qp_new());
  /* p29.GORO*/ wire _GORO_STORE8_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.EXUQ_STORE8_RSTp_evn.qp_new());
  /* p29.DUBU*/ wire _DUBU_STORE9_RSTp = or2(_DYBA_LINE_RSTp, sprite_reset_flags.FONO_STORE9_RSTp_evn.qp_new());

  /* p29.DYNA*/ wire _DYNA_STORE0_RSTn = not1(_DYWE_STORE0_RSTp);
  /* p29.DOKU*/ wire _DOKU_STORE1_RSTn = not1(_EFEV_STORE1_RSTp);
  /* p29.GAMY*/ wire _GAMY_STORE2_RSTn = not1(_FOKO_STORE2_RSTp);
  /* p29.WUPA*/ wire _WUPA_STORE3_RSTn = not1(_GAKE_STORE3_RSTp);
  /* p29.WUNU*/ wire _WUNU_STORE4_RSTn = not1(_WOHU_STORE4_RSTp);
  /* p29.EJAD*/ wire _EJAD_STORE5_RSTn = not1(_FEVE_STORE5_RSTp);
  /* p29.XAHO*/ wire _XAHO_STORE6_RSTn = not1(_WACY_STORE6_RSTp);
  /* p29.GAFY*/ wire _GAFY_STORE7_RSTn = not1(_GUKY_STORE7_RSTp);
  /* p29.WUZO*/ wire _WUZO_STORE8_RSTn = not1(_GORO_STORE8_RSTp);
  /* p29.DOSY*/ wire _DOSY_STORE9_RSTn = not1(_DUBU_STORE9_RSTp);

  // Clock the scanned sprite's X coordinate into the selected sprite store.

  /*#p31.ZAGO*/ wire _ZAGO_SPX0n_old = not1(oam_temp_b.YLOR_OAM_DB0p.qp_old());
  /* p31.ZOCY*/ wire _ZOCY_SPX1n_old = not1(oam_temp_b.ZYTY_OAM_DB1p.qp_old());
  /* p31.YPUR*/ wire _YPUR_SPX2n_old = not1(oam_temp_b.ZYVE_OAM_DB2p.qp_old());
  /* p31.YVOK*/ wire _YVOK_SPX3n_old = not1(oam_temp_b.ZEZY_OAM_DB3p.qp_old());
  /* p31.COSE*/ wire _COSE_SPX4n_old = not1(oam_temp_b.GOMO_OAM_DB4p.qp_old());
  /* p31.AROP*/ wire _AROP_SPX5n_old = not1(oam_temp_b.BAXO_OAM_DB5p.qp_old());
  /* p31.XATU*/ wire _XATU_SPX6n_old = not1(oam_temp_b.YZOS_OAM_DB6p.qp_old());
  /* p31.BADY*/ wire _BADY_SPX7n_old = not1(oam_temp_b.DEPO_OAM_DB7p.qp_old());

  /*#p31.XEPE*/ sprite_store.XEPE_STORE0_X0p_odd.dff9(_FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _ZAGO_SPX0n_old);
  /* p31.YLAH*/ sprite_store.YLAH_STORE0_X1p_odd.dff9(_FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _ZOCY_SPX1n_old);
  /* p31.ZOLA*/ sprite_store.ZOLA_STORE0_X2p_odd.dff9(_FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _YPUR_SPX2n_old);
  /* p31.ZULU*/ sprite_store.ZULU_STORE0_X3p_odd.dff9(_FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _YVOK_SPX3n_old);
  /* p31.WELO*/ sprite_store.WELO_STORE0_X4p_odd.dff9(_FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _COSE_SPX4n_old);
  /* p31.XUNY*/ sprite_store.XUNY_STORE0_X5p_odd.dff9(_FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _AROP_SPX5n_old);
  /* p31.WOTE*/ sprite_store.WOTE_STORE0_X6p_odd.dff9(_FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _XATU_SPX6n_old);
  /* p31.XAKO*/ sprite_store.XAKO_STORE0_X7p_odd.dff9(_FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _BADY_SPX7n_old);

  /* p31.DANY*/ sprite_store.DANY_STORE1_X0p_odd.dff9(_ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _ZAGO_SPX0n_old);
  /* p31.DUKO*/ sprite_store.DUKO_STORE1_X1p_odd.dff9(_ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _ZOCY_SPX1n_old);
  /* p31.DESU*/ sprite_store.DESU_STORE1_X2p_odd.dff9(_ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _YPUR_SPX2n_old);
  /* p31.DAZO*/ sprite_store.DAZO_STORE1_X3p_odd.dff9(_ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _YVOK_SPX3n_old);
  /* p31.DAKE*/ sprite_store.DAKE_STORE1_X4p_odd.dff9(_ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _COSE_SPX4n_old);
  /* p31.CESO*/ sprite_store.CESO_STORE1_X5p_odd.dff9(_ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _AROP_SPX5n_old);
  /* p31.DYFU*/ sprite_store.DYFU_STORE1_X6p_odd.dff9(_ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _XATU_SPX6n_old);
  /* p31.CUSY*/ sprite_store.CUSY_STORE1_X7p_odd.dff9(_ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _BADY_SPX7n_old);

  /* p31.FOKA*/ sprite_store.FOKA_STORE2_X0p_odd.dff9(_CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _ZAGO_SPX0n_old);
  /* p31.FYTY*/ sprite_store.FYTY_STORE2_X1p_odd.dff9(_CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _ZOCY_SPX1n_old);
  /* p31.FUBY*/ sprite_store.FUBY_STORE2_X2p_odd.dff9(_CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _YPUR_SPX2n_old);
  /* p31.GOXU*/ sprite_store.GOXU_STORE2_X3p_odd.dff9(_CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _YVOK_SPX3n_old);
  /* p31.DUHY*/ sprite_store.DUHY_STORE2_X4p_odd.dff9(_CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _COSE_SPX4n_old);
  /* p31.EJUF*/ sprite_store.EJUF_STORE2_X5p_odd.dff9(_CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _AROP_SPX5n_old);
  /* p31.ENOR*/ sprite_store.ENOR_STORE2_X6p_odd.dff9(_CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _XATU_SPX6n_old);
  /* p31.DEPY*/ sprite_store.DEPY_STORE2_X7p_odd.dff9(_CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _BADY_SPX7n_old);

  /* p31.XOLY*/ sprite_store.XOLY_STORE3_X0p_odd.dff9(_YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _ZAGO_SPX0n_old);
  /* p31.XYBA*/ sprite_store.XYBA_STORE3_X1p_odd.dff9(_YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _ZOCY_SPX1n_old);
  /* p31.XABE*/ sprite_store.XABE_STORE3_X2p_odd.dff9(_YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _YPUR_SPX2n_old);
  /* p31.XEKA*/ sprite_store.XEKA_STORE3_X3p_odd.dff9(_YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _YVOK_SPX3n_old);
  /* p31.XOMY*/ sprite_store.XOMY_STORE3_X4p_odd.dff9(_YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _COSE_SPX4n_old);
  /* p31.WUHA*/ sprite_store.WUHA_STORE3_X5p_odd.dff9(_YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _AROP_SPX5n_old);
  /* p31.WYNA*/ sprite_store.WYNA_STORE3_X6p_odd.dff9(_YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _XATU_SPX6n_old);
  /* p31.WECO*/ sprite_store.WECO_STORE3_X7p_odd.dff9(_YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _BADY_SPX7n_old);

  /* p31.WEDU*/ sprite_store.WEDU_STORE4_X0p_odd.dff9(_WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _ZAGO_SPX0n_old);
  /* p31.YGAJ*/ sprite_store.YGAJ_STORE4_X1p_odd.dff9(_WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _ZOCY_SPX1n_old);
  /* p31.ZYJO*/ sprite_store.ZYJO_STORE4_X2p_odd.dff9(_WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _YPUR_SPX2n_old);
  /* p31.XURY*/ sprite_store.XURY_STORE4_X3p_odd.dff9(_WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _YVOK_SPX3n_old);
  /* p31.YBED*/ sprite_store.YBED_STORE4_X4p_odd.dff9(_WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _COSE_SPX4n_old);
  /* p31.ZALA*/ sprite_store.ZALA_STORE4_X5p_odd.dff9(_WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _AROP_SPX5n_old);
  /* p31.WYDE*/ sprite_store.WYDE_STORE4_X6p_odd.dff9(_WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _XATU_SPX6n_old);
  /* p31.XEPA*/ sprite_store.XEPA_STORE4_X7p_odd.dff9(_WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _BADY_SPX7n_old);

  /* p31.FUSA*/ sprite_store.FUSA_STORE5_X0p_odd.dff9(_CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _ZAGO_SPX0n_old);
  /* p31.FAXA*/ sprite_store.FAXA_STORE5_X1p_odd.dff9(_CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _ZOCY_SPX1n_old);
  /* p31.FOZY*/ sprite_store.FOZY_STORE5_X2p_odd.dff9(_CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _YPUR_SPX2n_old);
  /* p31.FESY*/ sprite_store.FESY_STORE5_X3p_odd.dff9(_CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _YVOK_SPX3n_old);
  /* p31.CYWE*/ sprite_store.CYWE_STORE5_X4p_odd.dff9(_CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _COSE_SPX4n_old);
  /* p31.DYBY*/ sprite_store.DYBY_STORE5_X5p_odd.dff9(_CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _AROP_SPX5n_old);
  /* p31.DURY*/ sprite_store.DURY_STORE5_X6p_odd.dff9(_CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _XATU_SPX6n_old);
  /* p31.CUVY*/ sprite_store.CUVY_STORE5_X7p_odd.dff9(_CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _BADY_SPX7n_old);

  /* p31.YCOL*/ sprite_store.YCOL_STORE6_X0p_odd.dff9(_ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _ZAGO_SPX0n_old);
  /* p31.YRAC*/ sprite_store.YRAC_STORE6_X1p_odd.dff9(_ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _ZOCY_SPX1n_old);
  /* p31.YMEM*/ sprite_store.YMEM_STORE6_X2p_odd.dff9(_ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _YPUR_SPX2n_old);
  /* p31.YVAG*/ sprite_store.YVAG_STORE6_X3p_odd.dff9(_ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _YVOK_SPX3n_old);
  /* p31.ZOLY*/ sprite_store.ZOLY_STORE6_X4p_odd.dff9(_ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _COSE_SPX4n_old);
  /* p31.ZOGO*/ sprite_store.ZOGO_STORE6_X5p_odd.dff9(_ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _AROP_SPX5n_old);
  /* p31.ZECU*/ sprite_store.ZECU_STORE6_X6p_odd.dff9(_ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _XATU_SPX6n_old);
  /* p31.ZESA*/ sprite_store.ZESA_STORE6_X7p_odd.dff9(_ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _BADY_SPX7n_old);

  /* p31.ERAZ*/ sprite_store.ERAZ_STORE7_X0p_odd.dff9(_GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _ZAGO_SPX0n_old);
  /* p31.EPUM*/ sprite_store.EPUM_STORE7_X1p_odd.dff9(_GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _ZOCY_SPX1n_old);
  /* p31.EROL*/ sprite_store.EROL_STORE7_X2p_odd.dff9(_GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _YPUR_SPX2n_old);
  /* p31.EHYN*/ sprite_store.EHYN_STORE7_X3p_odd.dff9(_GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _YVOK_SPX3n_old);
  /* p31.FAZU*/ sprite_store.FAZU_STORE7_X4p_odd.dff9(_GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _COSE_SPX4n_old);
  /* p31.FAXE*/ sprite_store.FAXE_STORE7_X5p_odd.dff9(_GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _AROP_SPX5n_old);
  /* p31.EXUK*/ sprite_store.EXUK_STORE7_X6p_odd.dff9(_GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _XATU_SPX6n_old);
  /* p31.FEDE*/ sprite_store.FEDE_STORE7_X7p_odd.dff9(_GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _BADY_SPX7n_old);

  /* p31.EZUF*/ sprite_store.EZUF_STORE8_X0p_odd.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _ZAGO_SPX0n_old);
  /* p31.ENAD*/ sprite_store.ENAD_STORE8_X1p_odd.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _ZOCY_SPX1n_old);
  /* p31.EBOW*/ sprite_store.EBOW_STORE8_X2p_odd.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _YPUR_SPX2n_old);
  /* p31.FYCA*/ sprite_store.FYCA_STORE8_X3p_odd.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _YVOK_SPX3n_old);
  /* p31.GAVY*/ sprite_store.GAVY_STORE8_X4p_odd.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _COSE_SPX4n_old);
  /* p31.GYPU*/ sprite_store.GYPU_STORE8_X5p_odd.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _AROP_SPX5n_old);
  /* p31.GADY*/ sprite_store.GADY_STORE8_X6p_odd.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _XATU_SPX6n_old);
  /* p31.GAZA*/ sprite_store.GAZA_STORE8_X7p_odd.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _BADY_SPX7n_old);

  /* p31.XUVY*/ sprite_store.XUVY_STORE9_X0p_odd.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _ZAGO_SPX0n_old);
  /* p31.XERE*/ sprite_store.XERE_STORE9_X1p_odd.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _ZOCY_SPX1n_old);
  /* p31.XUZO*/ sprite_store.XUZO_STORE9_X2p_odd.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _YPUR_SPX2n_old);
  /* p31.XEXA*/ sprite_store.XEXA_STORE9_X3p_odd.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _YVOK_SPX3n_old);
  /* p31.YPOD*/ sprite_store.YPOD_STORE9_X4p_odd.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _COSE_SPX4n_old);
  /* p31.YROP*/ sprite_store.YROP_STORE9_X5p_odd.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _AROP_SPX5n_old);
  /* p31.YNEP*/ sprite_store.YNEP_STORE9_X6p_odd.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _XATU_SPX6n_old);
  /* p31.YZOF*/ sprite_store.YZOF_STORE9_X7p_odd.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _BADY_SPX7n_old);

  // Clock the scanned sprite's index into the selected sprite store.

  /* p29.GENY*/ wire _GENY_STORE0_CLKp = not1(sprite_store_flags.DYHU_STORE0_CLKn.qp_new());
  /* p29.BYVY*/ wire _BYVY_STORE1_CLKp = not1(sprite_store_flags.BUCO_STORE1_CLKn.qp_new());
  /* p29.BUZY*/ wire _BUZY_STORE2_CLKp = not1(sprite_store_flags.GYFO_STORE2_CLKn.qp_new());
  /* p29.FEKA*/ wire _FEKA_STORE3_CLKp = not1(sprite_store_flags.GUSA_STORE3_CLKn.qp_new());
  /* p29.WYLU*/ wire _WYLU_STORE4_CLKp = not1(sprite_store_flags.DUKE_STORE4_CLKn.qp_new());
  /* p29.DYMO*/ wire _DYMO_STORE5_CLKp = not1(sprite_store_flags.BEDE_STORE5_CLKn.qp_new());
  /* p29.WUSE*/ wire _WUSE_STORE6_CLKp = not1(sprite_store_flags.WEKA_STORE6_CLKn.qp_new());
  /* p29.FEFO*/ wire _FEFO_STORE7_CLKp = not1(sprite_store_flags.GYVO_STORE7_CLKn.qp_new());
  /* p29.AKOL*/ wire _AKOL_STORE8_CLKp = not1(sprite_store_flags.BUKA_STORE8_CLKn.qp_new());
  /* p29.WUFA*/ wire _WUFA_STORE9_CLKp = not1(sprite_store_flags.DECU_STORE9_CLKn.qp_new());

  /* p30.YGUS*/ sprite_store.YGUS_STORE0_I0n_odd.dff8n(_GENY_STORE0_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.YSOK*/ sprite_store.YSOK_STORE0_I1n_odd.dff8n(_GENY_STORE0_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.YZEP*/ sprite_store.YZEP_STORE0_I2n_odd.dff8n(_GENY_STORE0_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.WYTE*/ sprite_store.WYTE_STORE0_I3n_odd.dff8n(_GENY_STORE0_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.ZONY*/ sprite_store.ZONY_STORE0_I4n_odd.dff8n(_GENY_STORE0_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.YWAK*/ sprite_store.YWAK_STORE0_I5n_odd.dff8n(_GENY_STORE0_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  /* p30.CADU*/ sprite_store.CADU_STORE1_I0n_odd.dff8n(_BYVY_STORE1_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.CEBO*/ sprite_store.CEBO_STORE1_I1n_odd.dff8n(_BYVY_STORE1_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.CUFA*/ sprite_store.CUFA_STORE1_I2n_odd.dff8n(_BYVY_STORE1_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.COMA*/ sprite_store.COMA_STORE1_I3n_odd.dff8n(_BYVY_STORE1_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.CUZA*/ sprite_store.CUZA_STORE1_I4n_odd.dff8n(_BYVY_STORE1_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.CAJY*/ sprite_store.CAJY_STORE1_I5n_odd.dff8n(_BYVY_STORE1_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  /* p30.BUHE*/ sprite_store.BUHE_STORE2_I0n_odd.dff8n(_BUZY_STORE2_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.BYHU*/ sprite_store.BYHU_STORE2_I1n_odd.dff8n(_BUZY_STORE2_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.BECA*/ sprite_store.BECA_STORE2_I2n_odd.dff8n(_BUZY_STORE2_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.BULU*/ sprite_store.BULU_STORE2_I3n_odd.dff8n(_BUZY_STORE2_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.BUNA*/ sprite_store.BUNA_STORE2_I4n_odd.dff8n(_BUZY_STORE2_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.BOXA*/ sprite_store.BOXA_STORE2_I5n_odd.dff8n(_BUZY_STORE2_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  /* p30.DEVY*/ sprite_store.DEVY_STORE3_I0n_odd.dff8n(_FEKA_STORE3_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.DESE*/ sprite_store.DESE_STORE3_I1n_odd.dff8n(_FEKA_STORE3_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.DUNY*/ sprite_store.DUNY_STORE3_I2n_odd.dff8n(_FEKA_STORE3_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.DUHA*/ sprite_store.DUHA_STORE3_I3n_odd.dff8n(_FEKA_STORE3_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.DEBA*/ sprite_store.DEBA_STORE3_I4n_odd.dff8n(_FEKA_STORE3_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.DAFU*/ sprite_store.DAFU_STORE3_I5n_odd.dff8n(_FEKA_STORE3_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  /* p30.XAVE*/ sprite_store.XAVE_STORE4_I0n_odd.dff8n(_WYLU_STORE4_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.XEFE*/ sprite_store.XEFE_STORE4_I1n_odd.dff8n(_WYLU_STORE4_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.WANU*/ sprite_store.WANU_STORE4_I2n_odd.dff8n(_WYLU_STORE4_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.XABO*/ sprite_store.XABO_STORE4_I3n_odd.dff8n(_WYLU_STORE4_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.XEGE*/ sprite_store.XEGE_STORE4_I4n_odd.dff8n(_WYLU_STORE4_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.XYNU*/ sprite_store.XYNU_STORE4_I5n_odd.dff8n(_WYLU_STORE4_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  /* p30.EKOP*/ sprite_store.EKOP_STORE5_I0n_odd.dff8n(_DYMO_STORE5_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.ETYM*/ sprite_store.ETYM_STORE5_I1n_odd.dff8n(_DYMO_STORE5_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.GORU*/ sprite_store.GORU_STORE5_I2n_odd.dff8n(_DYMO_STORE5_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.EBEX*/ sprite_store.EBEX_STORE5_I3n_odd.dff8n(_DYMO_STORE5_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.ETAV*/ sprite_store.ETAV_STORE5_I4n_odd.dff8n(_DYMO_STORE5_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.EKAP*/ sprite_store.EKAP_STORE5_I5n_odd.dff8n(_DYMO_STORE5_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  /* p30.GABO*/ sprite_store.GABO_STORE6_I0n_odd.dff8n(_WUSE_STORE6_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.GACY*/ sprite_store.GACY_STORE6_I1n_odd.dff8n(_WUSE_STORE6_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.FOGO*/ sprite_store.FOGO_STORE6_I2n_odd.dff8n(_WUSE_STORE6_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.GOHU*/ sprite_store.GOHU_STORE6_I3n_odd.dff8n(_WUSE_STORE6_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.FOXY*/ sprite_store.FOXY_STORE6_I4n_odd.dff8n(_WUSE_STORE6_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.GECU*/ sprite_store.GECU_STORE6_I5n_odd.dff8n(_WUSE_STORE6_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  /* p30.GULE*/ sprite_store.GULE_STORE7_I0n_odd.dff8n(_FEFO_STORE7_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.GYNO*/ sprite_store.GYNO_STORE7_I1n_odd.dff8n(_FEFO_STORE7_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.FEFA*/ sprite_store.FEFA_STORE7_I2n_odd.dff8n(_FEFO_STORE7_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.FYSU*/ sprite_store.FYSU_STORE7_I3n_odd.dff8n(_FEFO_STORE7_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.GESY*/ sprite_store.GESY_STORE7_I4n_odd.dff8n(_FEFO_STORE7_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.FUZO*/ sprite_store.FUZO_STORE7_I5n_odd.dff8n(_FEFO_STORE7_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  /* p30.AXUV*/ sprite_store.AXUV_STORE8_I0n_odd.dff8n(_AKOL_STORE8_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.BADA*/ sprite_store.BADA_STORE8_I1n_odd.dff8n(_AKOL_STORE8_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.APEV*/ sprite_store.APEV_STORE8_I2n_odd.dff8n(_AKOL_STORE8_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.BADO*/ sprite_store.BADO_STORE8_I3n_odd.dff8n(_AKOL_STORE8_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.BEXY*/ sprite_store.BEXY_STORE8_I4n_odd.dff8n(_AKOL_STORE8_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.BYHE*/ sprite_store.BYHE_STORE8_I5n_odd.dff8n(_AKOL_STORE8_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  /* p30.YBER*/ sprite_store.YBER_STORE9_I0n_odd.dff8n(_WUFA_STORE9_CLKp, sprite_bus.BUS_SPR_I0.qp_old());
  /* p30.YZOR*/ sprite_store.YZOR_STORE9_I1n_odd.dff8n(_WUFA_STORE9_CLKp, sprite_bus.BUS_SPR_I1.qp_old());
  /* p30.XYFE*/ sprite_store.XYFE_STORE9_I2n_odd.dff8n(_WUFA_STORE9_CLKp, sprite_bus.BUS_SPR_I2.qp_old());
  /* p30.XOTU*/ sprite_store.XOTU_STORE9_I3n_odd.dff8n(_WUFA_STORE9_CLKp, sprite_bus.BUS_SPR_I3.qp_old());
  /* p30.XUTE*/ sprite_store.XUTE_STORE9_I4n_odd.dff8n(_WUFA_STORE9_CLKp, sprite_bus.BUS_SPR_I4.qp_old());
  /* p30.XUFO*/ sprite_store.XUFO_STORE9_I5n_odd.dff8n(_WUFA_STORE9_CLKp, sprite_bus.BUS_SPR_I5.qp_old());

  // Clock the scanned sprite's current line into the selected sprite store.

  /* p29.ENOB*/ wire _ENOB_STORE0_CLKp = not1(sprite_store_flags.DYHU_STORE0_CLKn.qp_new());
  /* p29.AHOF*/ wire _AHOF_STORE1_CLKp = not1(sprite_store_flags.BUCO_STORE1_CLKn.qp_new());
  /* p29.FUKE*/ wire _FUKE_STORE2_CLKp = not1(sprite_store_flags.GYFO_STORE2_CLKn.qp_new());
  /* p29.XYHA*/ wire _XYHA_STORE3_CLKp = not1(sprite_store_flags.GUSA_STORE3_CLKn.qp_new());
  /* p29.EWOT*/ wire _EWOT_STORE4_CLKp = not1(sprite_store_flags.DUKE_STORE4_CLKn.qp_new());
  /* p29.BUCY*/ wire _BUCY_STORE5_CLKp = not1(sprite_store_flags.BEDE_STORE5_CLKn.qp_new());
  /* p29.ZURU*/ wire _ZURU_STORE6_CLKp = not1(sprite_store_flags.WEKA_STORE6_CLKn.qp_new());
  /* p29.WABE*/ wire _WABE_STORE7_CLKp = not1(sprite_store_flags.GYVO_STORE7_CLKn.qp_new());
  /* p29.BYMY*/ wire _BYMY_STORE8_CLKp = not1(sprite_store_flags.BUKA_STORE8_CLKn.qp_new());
  /* p29.FAKA*/ wire _FAKA_STORE9_CLKp = not1(sprite_store_flags.DECU_STORE9_CLKn.qp_new());

  /* p30.GYHO*/ sprite_store.GYHO_STORE0_L0n_odd.dff8n(_ENOB_STORE0_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.CUFO*/ sprite_store.CUFO_STORE0_L1n_odd.dff8n(_ENOB_STORE0_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.BOZU*/ sprite_store.BOZU_STORE0_L2n_odd.dff8n(_ENOB_STORE0_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.FYHY*/ sprite_store.FYHY_STORE0_L3n_odd.dff8n(_ENOB_STORE0_CLKp, sprite_bus.BUS_SPR_L3.qp_old());

  /* p30.AMES*/ sprite_store.AMES_STORE1_L0n_odd.dff8n(_AHOF_STORE1_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.AROF*/ sprite_store.AROF_STORE1_L1n_odd.dff8n(_AHOF_STORE1_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.ABOP*/ sprite_store.ABOP_STORE1_L2n_odd.dff8n(_AHOF_STORE1_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.ABUG*/ sprite_store.ABUG_STORE1_L3n_odd.dff8n(_AHOF_STORE1_CLKp, sprite_bus.BUS_SPR_L3.qp_old());

  /* p30.YLOV*/ sprite_store.YLOV_STORE2_L0n_odd.dff8n(_FUKE_STORE2_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.XOSY*/ sprite_store.XOSY_STORE2_L1n_odd.dff8n(_FUKE_STORE2_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.XAZY*/ sprite_store.XAZY_STORE2_L2n_odd.dff8n(_FUKE_STORE2_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.YKUK*/ sprite_store.YKUK_STORE2_L3n_odd.dff8n(_FUKE_STORE2_CLKp, sprite_bus.BUS_SPR_L3.qp_old());

  /* p30.ZURO*/ sprite_store.ZURO_STORE3_L0n_odd.dff8n(_XYHA_STORE3_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.ZYLU*/ sprite_store.ZYLU_STORE3_L1n_odd.dff8n(_XYHA_STORE3_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.ZENE*/ sprite_store.ZENE_STORE3_L2n_odd.dff8n(_XYHA_STORE3_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.ZURY*/ sprite_store.ZURY_STORE3_L3n_odd.dff8n(_XYHA_STORE3_CLKp, sprite_bus.BUS_SPR_L3.qp_old());

  /* p30.CAPO*/ sprite_store.CAPO_STORE4_L0n_odd.dff8n(_EWOT_STORE4_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.CAJU*/ sprite_store.CAJU_STORE4_L1n_odd.dff8n(_EWOT_STORE4_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.CONO*/ sprite_store.CONO_STORE4_L2n_odd.dff8n(_EWOT_STORE4_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.CUMU*/ sprite_store.CUMU_STORE4_L3n_odd.dff8n(_EWOT_STORE4_CLKp, sprite_bus.BUS_SPR_L3.qp_old());

  /* p30.ACEP*/ sprite_store.ACEP_STORE5_L0n_odd.dff8n(_BUCY_STORE5_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.ABEG*/ sprite_store.ABEG_STORE5_L1n_odd.dff8n(_BUCY_STORE5_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.ABUX*/ sprite_store.ABUX_STORE5_L2n_odd.dff8n(_BUCY_STORE5_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.ANED*/ sprite_store.ANED_STORE5_L3n_odd.dff8n(_BUCY_STORE5_CLKp, sprite_bus.BUS_SPR_L3.qp_old());

  /* p30.ZUMY*/ sprite_store.ZUMY_STORE6_L0n_odd.dff8n(_ZURU_STORE6_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.ZAFU*/ sprite_store.ZAFU_STORE6_L1n_odd.dff8n(_ZURU_STORE6_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.ZEXO*/ sprite_store.ZEXO_STORE6_L2n_odd.dff8n(_ZURU_STORE6_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.ZUBE*/ sprite_store.ZUBE_STORE6_L3n_odd.dff8n(_ZURU_STORE6_CLKp, sprite_bus.BUS_SPR_L3.qp_old());

  /* p30.XYNA*/ sprite_store.XYNA_STORE7_L0n_odd.dff8n(_WABE_STORE7_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.YGUM*/ sprite_store.YGUM_STORE7_L1n_odd.dff8n(_WABE_STORE7_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.XAKU*/ sprite_store.XAKU_STORE7_L2n_odd.dff8n(_WABE_STORE7_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.XYGO*/ sprite_store.XYGO_STORE7_L3n_odd.dff8n(_WABE_STORE7_CLKp, sprite_bus.BUS_SPR_L3.qp_old());

  /* p30.AZAP*/ sprite_store.AZAP_STORE8_L0n_odd.dff8n(_BYMY_STORE8_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.AFYX*/ sprite_store.AFYX_STORE8_L1n_odd.dff8n(_BYMY_STORE8_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.AFUT*/ sprite_store.AFUT_STORE8_L2n_odd.dff8n(_BYMY_STORE8_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.AFYM*/ sprite_store.AFYM_STORE8_L3n_odd.dff8n(_BYMY_STORE8_CLKp, sprite_bus.BUS_SPR_L3.qp_old());

  /* p30.CANA*/ sprite_store.CANA_STORE9_L0n_odd.dff8n(_FAKA_STORE9_CLKp, sprite_bus.BUS_SPR_L0.qp_old());
  /* p30.FOFO*/ sprite_store.FOFO_STORE9_L1n_odd.dff8n(_FAKA_STORE9_CLKp, sprite_bus.BUS_SPR_L1.qp_old());
  /* p30.DYSY*/ sprite_store.DYSY_STORE9_L2n_odd.dff8n(_FAKA_STORE9_CLKp, sprite_bus.BUS_SPR_L2.qp_old());
  /* p30.DEWU*/ sprite_store.DEWU_STORE9_L3n_odd.dff8n(_FAKA_STORE9_CLKp, sprite_bus.BUS_SPR_L3.qp_old());
}

//-----------------------------------------------------------------------------
// Check the whole sprite store for a sprite at the current pixel coordinate
// and update the match flags and FEPO_STORE_MATCHp.

void GateBoy::get_sprite_match_flags(
  const PixCount& pix_count,
  const wire AROR_MATCH_ENp,
  GateBoySpriteStore& sprite_store,
  SpriteMatchFlags& sprite_match_flags,
  SigIn SIG_GND) {
  /*#p21.ACAM*/ wire ACAM_PX0n = not1(pix_count.XEHO_PX0p.qp_new());
  /* p21.AZUB*/ wire AZUB_PX1n = not1(pix_count.SAVY_PX1p.qp_new());
  /* p21.AMEL*/ wire AMEL_PX2n = not1(pix_count.XODU_PX2p.qp_new());
  /* p21.AHAL*/ wire AHAL_PX3n = not1(pix_count.XYDO_PX3p.qp_new());
  /* p21.APUX*/ wire APUX_PX4n = not1(pix_count.TUHU_PX4p.qp_new());
  /* p21.ABEF*/ wire ABEF_PX5n = not1(pix_count.TUKY_PX5p.qp_new());
  /* p21.ADAZ*/ wire ADAZ_PX6n = not1(pix_count.TAKO_PX6p.qp_new());
  /* p21.ASAH*/ wire ASAH_PX7n = not1(pix_count.SYBE_PX7p.qp_new());

  /*#p31.ZOGY*/ wire _ZOGY_STORE0_MATCH0n = xor2(sprite_store.XEPE_STORE0_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.ZEBA*/ wire _ZEBA_STORE0_MATCH1n = xor2(sprite_store.YLAH_STORE0_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.ZAHA*/ wire _ZAHA_STORE0_MATCH2n = xor2(sprite_store.ZOLA_STORE0_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.ZOKY*/ wire _ZOKY_STORE0_MATCH3n = xor2(sprite_store.ZULU_STORE0_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.WOJU*/ wire _WOJU_STORE0_MATCH4n = xor2(sprite_store.WELO_STORE0_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.YFUN*/ wire _YFUN_STORE0_MATCH5n = xor2(sprite_store.XUNY_STORE0_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.WYZA*/ wire _WYZA_STORE0_MATCH6n = xor2(sprite_store.WOTE_STORE0_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.YPUK*/ wire _YPUK_STORE0_MATCH7n = xor2(sprite_store.XAKO_STORE0_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.EDYM*/ wire _EDYM_STORE1_MATCH0n = xor2(sprite_store.DANY_STORE1_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.EMYB*/ wire _EMYB_STORE1_MATCH1n = xor2(sprite_store.DUKO_STORE1_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.EBEF*/ wire _EBEF_STORE1_MATCH2n = xor2(sprite_store.DESU_STORE1_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.EWOK*/ wire _EWOK_STORE1_MATCH3n = xor2(sprite_store.DAZO_STORE1_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.COLA*/ wire _COLA_STORE1_MATCH4n = xor2(sprite_store.DAKE_STORE1_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.BOBA*/ wire _BOBA_STORE1_MATCH5n = xor2(sprite_store.CESO_STORE1_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.COLU*/ wire _COLU_STORE1_MATCH6n = xor2(sprite_store.DYFU_STORE1_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.BAHU*/ wire _BAHU_STORE1_MATCH7n = xor2(sprite_store.CUSY_STORE1_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.FUZU*/ wire _FUZU_STORE2_MATCH0n = xor2(sprite_store.FOKA_STORE2_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.FESO*/ wire _FESO_STORE2_MATCH1n = xor2(sprite_store.FYTY_STORE2_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.FOKY*/ wire _FOKY_STORE2_MATCH2n = xor2(sprite_store.FUBY_STORE2_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.FYVA*/ wire _FYVA_STORE2_MATCH3n = xor2(sprite_store.GOXU_STORE2_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.CEKO*/ wire _CEKO_STORE2_MATCH4n = xor2(sprite_store.DUHY_STORE2_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.DETY*/ wire _DETY_STORE2_MATCH5n = xor2(sprite_store.EJUF_STORE2_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.DOZO*/ wire _DOZO_STORE2_MATCH6n = xor2(sprite_store.ENOR_STORE2_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.CONY*/ wire _CONY_STORE2_MATCH7n = xor2(sprite_store.DEPY_STORE2_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.YHOK*/ wire _YHOK_STORE3_MATCH0n = xor2(sprite_store.XOLY_STORE3_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.YCAH*/ wire _YCAH_STORE3_MATCH1n = xor2(sprite_store.XYBA_STORE3_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.YDAJ*/ wire _YDAJ_STORE3_MATCH2n = xor2(sprite_store.XABE_STORE3_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.YVUZ*/ wire _YVUZ_STORE3_MATCH3n = xor2(sprite_store.XEKA_STORE3_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.YVAP*/ wire _YVAP_STORE3_MATCH4n = xor2(sprite_store.XOMY_STORE3_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.XENY*/ wire _XENY_STORE3_MATCH5n = xor2(sprite_store.WUHA_STORE3_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.XAVU*/ wire _XAVU_STORE3_MATCH6n = xor2(sprite_store.WYNA_STORE3_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.XEVA*/ wire _XEVA_STORE3_MATCH7n = xor2(sprite_store.WECO_STORE3_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.XEJU*/ wire _XEJU_STORE4_MATCH0n = xor2(sprite_store.WEDU_STORE4_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.ZATE*/ wire _ZATE_STORE4_MATCH1n = xor2(sprite_store.YGAJ_STORE4_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.ZAKU*/ wire _ZAKU_STORE4_MATCH2n = xor2(sprite_store.ZYJO_STORE4_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.YBOX*/ wire _YBOX_STORE4_MATCH3n = xor2(sprite_store.XURY_STORE4_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.ZYKU*/ wire _ZYKU_STORE4_MATCH4n = xor2(sprite_store.YBED_STORE4_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ZYPU*/ wire _ZYPU_STORE4_MATCH5n = xor2(sprite_store.ZALA_STORE4_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.XAHA*/ wire _XAHA_STORE4_MATCH6n = xor2(sprite_store.WYDE_STORE4_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.ZEFE*/ wire _ZEFE_STORE4_MATCH7n = xor2(sprite_store.XEPA_STORE4_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.GUZO*/ wire _GUZO_STORE5_MATCH0n = xor2(sprite_store.FUSA_STORE5_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.GOLA*/ wire _GOLA_STORE5_MATCH1n = xor2(sprite_store.FAXA_STORE5_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.GEVE*/ wire _GEVE_STORE5_MATCH2n = xor2(sprite_store.FOZY_STORE5_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.GUDE*/ wire _GUDE_STORE5_MATCH3n = xor2(sprite_store.FESY_STORE5_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.BAZY*/ wire _BAZY_STORE5_MATCH4n = xor2(sprite_store.CYWE_STORE5_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.CYLE*/ wire _CYLE_STORE5_MATCH5n = xor2(sprite_store.DYBY_STORE5_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.CEVA*/ wire _CEVA_STORE5_MATCH6n = xor2(sprite_store.DURY_STORE5_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.BUMY*/ wire _BUMY_STORE5_MATCH7n = xor2(sprite_store.CUVY_STORE5_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.XOSU*/ wire _XOSU_STORE6_MATCH0n = xor2(sprite_store.YCOL_STORE6_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.ZUVU*/ wire _ZUVU_STORE6_MATCH1n = xor2(sprite_store.YRAC_STORE6_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.XUCO*/ wire _XUCO_STORE6_MATCH2n = xor2(sprite_store.YMEM_STORE6_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.ZULO*/ wire _ZULO_STORE6_MATCH3n = xor2(sprite_store.YVAG_STORE6_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.ZARE*/ wire _ZARE_STORE6_MATCH4n = xor2(sprite_store.ZOLY_STORE6_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ZEMU*/ wire _ZEMU_STORE6_MATCH5n = xor2(sprite_store.ZOGO_STORE6_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.ZYGO*/ wire _ZYGO_STORE6_MATCH6n = xor2(sprite_store.ZECU_STORE6_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.ZUZY*/ wire _ZUZY_STORE6_MATCH7n = xor2(sprite_store.ZESA_STORE6_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.DUSE*/ wire _DUSE_STORE7_MATCH0n = xor2(sprite_store.ERAZ_STORE7_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.DAGU*/ wire _DAGU_STORE7_MATCH1n = xor2(sprite_store.EPUM_STORE7_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.DYZE*/ wire _DYZE_STORE7_MATCH2n = xor2(sprite_store.EROL_STORE7_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.DESO*/ wire _DESO_STORE7_MATCH3n = xor2(sprite_store.EHYN_STORE7_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.EJOT*/ wire _EJOT_STORE7_MATCH4n = xor2(sprite_store.FAZU_STORE7_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ESAJ*/ wire _ESAJ_STORE7_MATCH5n = xor2(sprite_store.FAXE_STORE7_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.DUCU*/ wire _DUCU_STORE7_MATCH6n = xor2(sprite_store.EXUK_STORE7_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.EWUD*/ wire _EWUD_STORE7_MATCH7n = xor2(sprite_store.FEDE_STORE7_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.DUZE*/ wire _DUZE_STORE8_MATCH0n = xor2(sprite_store.EZUF_STORE8_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.DAGA*/ wire _DAGA_STORE8_MATCH1n = xor2(sprite_store.ENAD_STORE8_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.DAWU*/ wire _DAWU_STORE8_MATCH2n = xor2(sprite_store.EBOW_STORE8_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.EJAW*/ wire _EJAW_STORE8_MATCH3n = xor2(sprite_store.FYCA_STORE8_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.GOHO*/ wire _GOHO_STORE8_MATCH4n = xor2(sprite_store.GAVY_STORE8_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.GASU*/ wire _GASU_STORE8_MATCH5n = xor2(sprite_store.GYPU_STORE8_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.GABU*/ wire _GABU_STORE8_MATCH6n = xor2(sprite_store.GADY_STORE8_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.GAFE*/ wire _GAFE_STORE8_MATCH7n = xor2(sprite_store.GAZA_STORE8_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.YMAM*/ wire _YMAM_STORE9_MATCH0n = xor2(sprite_store.XUVY_STORE9_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.YTYP*/ wire _YTYP_STORE9_MATCH1n = xor2(sprite_store.XERE_STORE9_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.YFOP*/ wire _YFOP_STORE9_MATCH2n = xor2(sprite_store.XUZO_STORE9_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.YVAC*/ wire _YVAC_STORE9_MATCH3n = xor2(sprite_store.XEXA_STORE9_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.ZYWU*/ wire _ZYWU_STORE9_MATCH4n = xor2(sprite_store.YPOD_STORE9_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ZUZA*/ wire _ZUZA_STORE9_MATCH5n = xor2(sprite_store.YROP_STORE9_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.ZEJO*/ wire _ZEJO_STORE9_MATCH6n = xor2(sprite_store.YNEP_STORE9_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.ZEDA*/ wire _ZEDA_STORE9_MATCH7n = xor2(sprite_store.YZOF_STORE9_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.ZAKO*/ wire _ZAKO_STORE0_MATCHAp = nor4(_ZOGY_STORE0_MATCH0n, _ZEBA_STORE0_MATCH1n, _ZAHA_STORE0_MATCH2n, _ZOKY_STORE0_MATCH3n);
  /* p31.XEBA*/ wire _XEBA_STORE0_MATCHBp = nor4(_WOJU_STORE0_MATCH4n, _YFUN_STORE0_MATCH5n, _WYZA_STORE0_MATCH6n, _YPUK_STORE0_MATCH7n);
  /* p31.CYVY*/ wire _CYVY_STORE1_MATCHBp = nor4(_COLA_STORE1_MATCH4n, _BOBA_STORE1_MATCH5n, _COLU_STORE1_MATCH6n, _BAHU_STORE1_MATCH7n);
  /* p31.EWAM*/ wire _EWAM_STORE1_MATCHAp = nor4(_EDYM_STORE1_MATCH0n, _EMYB_STORE1_MATCH1n, _EBEF_STORE1_MATCH2n, _EWOK_STORE1_MATCH3n);
  /* p31.CEHU*/ wire _CEHU_STORE2_MATCHAp = nor4(_CEKO_STORE2_MATCH4n, _DETY_STORE2_MATCH5n, _DOZO_STORE2_MATCH6n, _CONY_STORE2_MATCH7n);
  /* p31.EKES*/ wire _EKES_STORE2_MATCHBp = nor4(_FUZU_STORE2_MATCH0n, _FESO_STORE2_MATCH1n, _FOKY_STORE2_MATCH2n, _FYVA_STORE2_MATCH3n);
  /* p31.ZURE*/ wire _ZURE_STORE3_MATCHAp = nor4(_YHOK_STORE3_MATCH0n, _YCAH_STORE3_MATCH1n, _YDAJ_STORE3_MATCH2n, _YVUZ_STORE3_MATCH3n);
  /* p31.YWOS*/ wire _YWOS_STORE3_MATCHBp = nor4(_YVAP_STORE3_MATCH4n, _XENY_STORE3_MATCH5n, _XAVU_STORE3_MATCH6n, _XEVA_STORE3_MATCH7n);
  /* p31.YKOK*/ wire _YKOK_STORE4_MATCHAp = nor4(_XEJU_STORE4_MATCH0n, _ZATE_STORE4_MATCH1n, _ZAKU_STORE4_MATCH2n, _YBOX_STORE4_MATCH3n);
  /* p31.YNAZ*/ wire _YNAZ_STORE4_MATCHBp = nor4(_ZYKU_STORE4_MATCH4n, _ZYPU_STORE4_MATCH5n, _XAHA_STORE4_MATCH6n, _ZEFE_STORE4_MATCH7n);
  /* p31.COGY*/ wire _COGY_STORE5_MATCHAp = nor4(_BAZY_STORE5_MATCH4n, _CYLE_STORE5_MATCH5n, _CEVA_STORE5_MATCH6n, _BUMY_STORE5_MATCH7n);
  /* p31.FYMA*/ wire _FYMA_STORE5_MATCHBp = nor4(_GUZO_STORE5_MATCH0n, _GOLA_STORE5_MATCH1n, _GEVE_STORE5_MATCH2n, _GUDE_STORE5_MATCH3n);
  /* p31.YWAP*/ wire _YWAP_STORE6_MATCHAp = nor4(_ZARE_STORE6_MATCH4n, _ZEMU_STORE6_MATCH5n, _ZYGO_STORE6_MATCH6n, _ZUZY_STORE6_MATCH7n);
  /* p31.YDOT*/ wire _YDOT_STORE6_MATCHBp = nor4(_XOSU_STORE6_MATCH0n, _ZUVU_STORE6_MATCH1n, _XUCO_STORE6_MATCH2n, _ZULO_STORE6_MATCH3n);
  /* p31.CYCO*/ wire _CYCO_STORE7_MATCHAp = nor4(_DUSE_STORE7_MATCH0n, _DAGU_STORE7_MATCH1n, _DYZE_STORE7_MATCH2n, _DESO_STORE7_MATCH3n);
  /* p31.DAJE*/ wire _DAJE_STORE7_MATCHBp = nor4(_EJOT_STORE7_MATCH4n, _ESAJ_STORE7_MATCH5n, _DUCU_STORE7_MATCH6n, _EWUD_STORE7_MATCH7n);
  /* p31.DAMA*/ wire _DAMA_STORE8_MATCHAp = nor4(_DUZE_STORE8_MATCH0n, _DAGA_STORE8_MATCH1n, _DAWU_STORE8_MATCH2n, _EJAW_STORE8_MATCH3n);
  /* p31.FEHA*/ wire _FEHA_STORE8_MATCHBp = nor4(_GOHO_STORE8_MATCH4n, _GASU_STORE8_MATCH5n, _GABU_STORE8_MATCH6n, _GAFE_STORE8_MATCH7n);
  /* p31.YLEV*/ wire _YLEV_STORE9_MATCHAp = nor4(_YMAM_STORE9_MATCH0n, _YTYP_STORE9_MATCH1n, _YFOP_STORE9_MATCH2n, _YVAC_STORE9_MATCH3n);
  /* p31.YTUB*/ wire _YTUB_STORE9_MATCHBp = nor4(_ZYWU_STORE9_MATCH4n, _ZUZA_STORE9_MATCH5n, _ZEJO_STORE9_MATCH6n, _ZEDA_STORE9_MATCH7n);

  /* p29.YDUG*/ wire YDUG_STORE0_MATCHn = nand3(AROR_MATCH_ENp, _ZAKO_STORE0_MATCHAp, _XEBA_STORE0_MATCHBp);
  /* p29.DYDU*/ wire DYDU_STORE1_MATCHn = nand3(AROR_MATCH_ENp, _EWAM_STORE1_MATCHAp, _CYVY_STORE1_MATCHBp);
  /* p29.DEGO*/ wire DEGO_STORE2_MATCHn = nand3(AROR_MATCH_ENp, _CEHU_STORE2_MATCHAp, _EKES_STORE2_MATCHBp);
  /* p29.YLOZ*/ wire YLOZ_STORE3_MATCHn = nand3(AROR_MATCH_ENp, _ZURE_STORE3_MATCHAp, _YWOS_STORE3_MATCHBp);
  /* p29.XAGE*/ wire XAGE_STORE4_MATCHn = nand3(AROR_MATCH_ENp, _YKOK_STORE4_MATCHAp, _YNAZ_STORE4_MATCHBp);
  /* p29.EGOM*/ wire EGOM_STORE5_MATCHn = nand3(AROR_MATCH_ENp, _COGY_STORE5_MATCHAp, _FYMA_STORE5_MATCHBp);
  /* p29.YBEZ*/ wire YBEZ_STORE6_MATCHn = nand3(AROR_MATCH_ENp, _YWAP_STORE6_MATCHAp, _YDOT_STORE6_MATCHBp);
  /* p29.DYKA*/ wire DYKA_STORE7_MATCHn = nand3(AROR_MATCH_ENp, _CYCO_STORE7_MATCHAp, _DAJE_STORE7_MATCHBp);
  /* p29.EFYL*/ wire EFYL_STORE8_MATCHn = nand3(AROR_MATCH_ENp, _DAMA_STORE8_MATCHAp, _FEHA_STORE8_MATCHBp);
  /* p29.YGEM*/ wire YGEM_STORE9_MATCHn = nand3(AROR_MATCH_ENp, _YLEV_STORE9_MATCHAp, _YTUB_STORE9_MATCHBp);

  /* p29.FEFY*/ wire FEFY_STORE_MATCHp = nand5(XAGE_STORE4_MATCHn, YLOZ_STORE3_MATCHn, DEGO_STORE2_MATCHn, DYDU_STORE1_MATCHn, YDUG_STORE0_MATCHn);
  /* p29.FOVE*/ wire FOVE_STORE_MATCHp = nand5(YGEM_STORE9_MATCHn, EFYL_STORE8_MATCHn, DYKA_STORE7_MATCHn, YBEZ_STORE6_MATCHn, EGOM_STORE5_MATCHn);
  /* p29.FEPO*/ sprite_match_flags.FEPO_STORE_MATCHp = or2(FEFY_STORE_MATCHp, FOVE_STORE_MATCHp);

  // Update one-hot priority encoder to flag the first matching sprite.

  /* p29.WEFU*/ wire _WEFU_STORE0_MATCH = not1(YDUG_STORE0_MATCHn);
  /* p29.GAJA*/ wire _GAJA_STORE1_MATCH = not1(DYDU_STORE1_MATCHn);
  /* p29.GUPO*/ wire _GUPO_STORE2_MATCH = not1(DEGO_STORE2_MATCHn);
  /* p29.WEBO*/ wire _WEBO_STORE3_MATCH = not1(YLOZ_STORE3_MATCHn);
  /* p29.WUNA*/ wire _WUNA_STORE4_MATCH = not1(XAGE_STORE4_MATCHn);
  /* p29.GABA*/ wire _GABA_STORE5_MATCH = not1(EGOM_STORE5_MATCHn);
  /* p29.WASE*/ wire _WASE_STORE6_MATCH = not1(YBEZ_STORE6_MATCHn);
  /* p29.GYTE*/ wire _GYTE_STORE7_MATCH = not1(DYKA_STORE7_MATCHn);
  /* p29.GEKE*/ wire _GEKE_STORE8_MATCH = not1(EFYL_STORE8_MATCHn);

  /* p29.GEZE*/ wire _GEZE_STORE0_MATCH = or2(_WEFU_STORE0_MATCH, SIG_GND.qp_new());
  /* p29.FUMA*/ wire _FUMA_STORE1_MATCH = or2(_GAJA_STORE1_MATCH, _GEZE_STORE0_MATCH);
  /* p29.GEDE*/ wire _GEDE_STORE2_MATCH = or2(_GUPO_STORE2_MATCH, _FUMA_STORE1_MATCH);
  /* p29.WUTO*/ wire _WUTO_STORE3_MATCH = or2(_WEBO_STORE3_MATCH, _GEDE_STORE2_MATCH);
  /* p29.XYLA*/ wire _XYLA_STORE4_MATCH = or2(_WUNA_STORE4_MATCH, _WUTO_STORE3_MATCH);
  /* p29.WEJA*/ wire _WEJA_STORE5_MATCH = or2(_GABA_STORE5_MATCH, _XYLA_STORE4_MATCH);
  /* p29.WYLA*/ wire _WYLA_STORE6_MATCH = or2(_WASE_STORE6_MATCH, _WEJA_STORE5_MATCH);
  /* p29.FAVO*/ wire _FAVO_STORE7_MATCH = or2(_GYTE_STORE7_MATCH, _WYLA_STORE6_MATCH);
  /* p29.GYGA*/ wire _GYGA_STORE8_MATCH = or2(_GEKE_STORE8_MATCH, _FAVO_STORE7_MATCH);

  /* p29.GUVA*/ sprite_match_flags.GUVA_SPRITE0_GETp = nor2(YDUG_STORE0_MATCHn, SIG_GND.qp_new());
  /* p29.ENUT*/ sprite_match_flags.ENUT_SPRITE1_GETp = nor2(DYDU_STORE1_MATCHn, _GEZE_STORE0_MATCH);
  /* p29.EMOL*/ sprite_match_flags.EMOL_SPRITE2_GETp = nor2(DEGO_STORE2_MATCHn, _FUMA_STORE1_MATCH);
  /* p29.GYFY*/ sprite_match_flags.GYFY_SPRITE3_GETp = nor2(YLOZ_STORE3_MATCHn, _GEDE_STORE2_MATCH);
  /* p29.GONO*/ sprite_match_flags.GONO_SPRITE4_GETp = nor2(XAGE_STORE4_MATCHn, _WUTO_STORE3_MATCH);
  /* p29.GEGA*/ sprite_match_flags.GEGA_SPRITE5_GETp = nor2(EGOM_STORE5_MATCHn, _XYLA_STORE4_MATCH);
  /* p29.XOJA*/ sprite_match_flags.XOJA_SPRITE6_GETp = nor2(YBEZ_STORE6_MATCHn, _WEJA_STORE5_MATCH);
  /* p29.GUTU*/ sprite_match_flags.GUTU_SPRITE7_GETp = nor2(DYKA_STORE7_MATCHn, _WYLA_STORE6_MATCH);
  /* p29.FOXA*/ sprite_match_flags.FOXA_SPRITE8_GETp = nor2(EFYL_STORE8_MATCHn, _FAVO_STORE7_MATCH);
  /* p29.GUZE*/ sprite_match_flags.GUZE_SPRITE9_GETp = nor2(YGEM_STORE9_MATCHn, _GYGA_STORE8_MATCH);
}

//-----------------------------------------------------------------------------

void GateBoy::sprite_match_to_bus(
  const GateBoySpriteStore& sprite_store,
  const SpriteMatchFlags& sprite_get_flag,
  SpriteBus& sprite_bus)
{
  // Push sprite index and line for the matching sprite onto the i/l bus.

  /* p29.FURO*/ wire _FURO_SPRITE0_GETn = not1(sprite_get_flag.GUVA_SPRITE0_GETp.qp_new());
  /* p29.DYDO*/ wire _DYDO_SPRITE1_GETn = not1(sprite_get_flag.ENUT_SPRITE1_GETp.qp_new());
  /* p29.FAME*/ wire _FAME_SPRITE2_GETn = not1(sprite_get_flag.EMOL_SPRITE2_GETp.qp_new());
  /* p29.GYMA*/ wire _GYMA_SPRITE3_GETn = not1(sprite_get_flag.GYFY_SPRITE3_GETp.qp_new());
  /* p29.GOWO*/ wire _GOWO_SPRITE4_GETn = not1(sprite_get_flag.GONO_SPRITE4_GETp.qp_new());
  /* p29.GYGY*/ wire _GYGY_SPRITE5_GETn = not1(sprite_get_flag.GEGA_SPRITE5_GETp.qp_new());
  /* p29.XYME*/ wire _XYME_SPRITE6_GETn = not1(sprite_get_flag.XOJA_SPRITE6_GETp.qp_new());
  /* p29.GUGY*/ wire _GUGY_SPRITE7_GETn = not1(sprite_get_flag.GUTU_SPRITE7_GETp.qp_new());
  /* p29.DENY*/ wire _DENY_SPRITE8_GETn = not1(sprite_get_flag.FOXA_SPRITE8_GETp.qp_new());
  /* p29.FADO*/ wire _FADO_SPRITE9_GETn = not1(sprite_get_flag.GUZE_SPRITE9_GETp.qp_new());

  /* p30.ZETU_STORE0_GET_I0*/ wire ZETU_STORE0_GET_I0 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.YGUS_STORE0_I0n_odd.qp_new());
  /* p30.ZECE_STORE0_GET_I1*/ wire ZECE_STORE0_GET_I1 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.YSOK_STORE0_I1n_odd.qp_new());
  /* p30.ZAVE_STORE0_GET_I2*/ wire ZAVE_STORE0_GET_I2 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.YZEP_STORE0_I2n_odd.qp_new());
  /* p30.WOKO_STORE0_GET_I3*/ wire WOKO_STORE0_GET_I3 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.WYTE_STORE0_I3n_odd.qp_new());
  /* p30.ZUMU_STORE0_GET_L0*/ wire ZUMU_STORE0_GET_L0 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.ZONY_STORE0_I4n_odd.qp_new());
  /*#p30.ZEDY_STORE0_GET_L1*/ wire ZEDY_STORE0_GET_L1 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.YWAK_STORE0_I5n_odd.qp_new());
  /* p30.WEHE_STORE0_GET_L2*/ wire WEHE_STORE0_GET_L2 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.GYHO_STORE0_L0n_odd.qp_new());
  /* p30.BUKY_STORE0_GET_L3*/ wire BUKY_STORE0_GET_L3 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.CUFO_STORE0_L1n_odd.qp_new());
  /* p30.AJAL_STORE0_GET_L4*/ wire AJAL_STORE0_GET_L4 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.BOZU_STORE0_L2n_odd.qp_new());
  /* p30.GOFO_STORE0_GET_L5*/ wire GOFO_STORE0_GET_L5 = tri6_nn(_FURO_SPRITE0_GETn, sprite_store.FYHY_STORE0_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(ZETU_STORE0_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(ZECE_STORE0_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(ZAVE_STORE0_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(WOKO_STORE0_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(ZUMU_STORE0_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(ZEDY_STORE0_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(WEHE_STORE0_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(BUKY_STORE0_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(AJAL_STORE0_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(GOFO_STORE0_GET_L5);

  /*#p30.CUBO_STORE1_GET_I0*/ wire CUBO_STORE1_GET_I0 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CADU_STORE1_I0n_odd.qp_new());
  /* p30.CELU_STORE1_GET_I1*/ wire CELU_STORE1_GET_I1 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CEBO_STORE1_I1n_odd.qp_new());
  /* p30.CEGY_STORE1_GET_I2*/ wire CEGY_STORE1_GET_I2 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CUFA_STORE1_I2n_odd.qp_new());
  /* p30.BETY_STORE1_GET_I3*/ wire BETY_STORE1_GET_I3 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.COMA_STORE1_I3n_odd.qp_new());
  /* p30.CYBY_STORE1_GET_L0*/ wire CYBY_STORE1_GET_L0 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CUZA_STORE1_I4n_odd.qp_new());
  /* p30.BEMO_STORE1_GET_L1*/ wire BEMO_STORE1_GET_L1 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CAJY_STORE1_I5n_odd.qp_new());
  /* p30.BYRO_STORE1_GET_L2*/ wire BYRO_STORE1_GET_L2 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.AMES_STORE1_L0n_odd.qp_new());
  /* p30.AHUM_STORE1_GET_L3*/ wire AHUM_STORE1_GET_L3 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.AROF_STORE1_L1n_odd.qp_new());
  /* p30.BACO_STORE1_GET_L4*/ wire BACO_STORE1_GET_L4 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.ABOP_STORE1_L2n_odd.qp_new());
  /* p30.BEFE_STORE1_GET_L5*/ wire BEFE_STORE1_GET_L5 = tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.ABUG_STORE1_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(CUBO_STORE1_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(CELU_STORE1_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(CEGY_STORE1_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(BETY_STORE1_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(CYBY_STORE1_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(BEMO_STORE1_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(BYRO_STORE1_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(AHUM_STORE1_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(BACO_STORE1_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(BEFE_STORE1_GET_L5);

  /* p30.CUBE_STORE2_GET_I0*/ wire CUBE_STORE2_GET_I0 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BUHE_STORE2_I0n_odd.qp_new());
  /* p30.AFOZ_STORE2_GET_I1*/ wire AFOZ_STORE2_GET_I1 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BYHU_STORE2_I1n_odd.qp_new());
  /* p30.APON_STORE2_GET_I2*/ wire APON_STORE2_GET_I2 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BECA_STORE2_I2n_odd.qp_new());
  /* p30.CUVU_STORE2_GET_I3*/ wire CUVU_STORE2_GET_I3 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BULU_STORE2_I3n_odd.qp_new());
  /* p30.CYRO_STORE2_GET_L0*/ wire CYRO_STORE2_GET_L0 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BUNA_STORE2_I4n_odd.qp_new());
  /* p30.AXEC_STORE2_GET_L1*/ wire AXEC_STORE2_GET_L1 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BOXA_STORE2_I5n_odd.qp_new());
  /* p30.ZUKE_STORE2_GET_L2*/ wire ZUKE_STORE2_GET_L2 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.YLOV_STORE2_L0n_odd.qp_new());
  /* p30.WERE_STORE2_GET_L3*/ wire WERE_STORE2_GET_L3 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.XOSY_STORE2_L1n_odd.qp_new());
  /* p30.WUXE_STORE2_GET_L4*/ wire WUXE_STORE2_GET_L4 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.XAZY_STORE2_L2n_odd.qp_new());
  /* p30.ZABY_STORE2_GET_L5*/ wire ZABY_STORE2_GET_L5 = tri6_nn(_FAME_SPRITE2_GETn, sprite_store.YKUK_STORE2_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(CUBE_STORE2_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(AFOZ_STORE2_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(APON_STORE2_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(CUVU_STORE2_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(CYRO_STORE2_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(AXEC_STORE2_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(ZUKE_STORE2_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(WERE_STORE2_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(WUXE_STORE2_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(ZABY_STORE2_GET_L5);

  /* p30.ENAP_STORE3_GET_I0*/ wire ENAP_STORE3_GET_I0 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DEVY_STORE3_I0n_odd.qp_new());
  /* p30.DYGO_STORE3_GET_I1*/ wire DYGO_STORE3_GET_I1 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DESE_STORE3_I1n_odd.qp_new());
  /* p30.DOWA_STORE3_GET_I2*/ wire DOWA_STORE3_GET_I2 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DUNY_STORE3_I2n_odd.qp_new());
  /* p30.DONY_STORE3_GET_I3*/ wire DONY_STORE3_GET_I3 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DUHA_STORE3_I3n_odd.qp_new());
  /* p30.EFUD_STORE3_GET_L0*/ wire EFUD_STORE3_GET_L0 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DEBA_STORE3_I4n_odd.qp_new());
  /* p30.DEZU_STORE3_GET_L1*/ wire DEZU_STORE3_GET_L1 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DAFU_STORE3_I5n_odd.qp_new());
  /* p30.ZEXE_STORE3_GET_L2*/ wire ZEXE_STORE3_GET_L2 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.ZURO_STORE3_L0n_odd.qp_new());
  /* p30.YWAV_STORE3_GET_L3*/ wire YWAV_STORE3_GET_L3 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.ZYLU_STORE3_L1n_odd.qp_new());
  /* p30.YJEM_STORE3_GET_L4*/ wire YJEM_STORE3_GET_L4 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.ZENE_STORE3_L2n_odd.qp_new());
  /* p30.ZYPO_STORE3_GET_L5*/ wire ZYPO_STORE3_GET_L5 = tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.ZURY_STORE3_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(ENAP_STORE3_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(DYGO_STORE3_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(DOWA_STORE3_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(DONY_STORE3_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(EFUD_STORE3_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(DEZU_STORE3_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(ZEXE_STORE3_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(YWAV_STORE3_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(YJEM_STORE3_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(ZYPO_STORE3_GET_L5);

  /* p30.WUXU_STORE4_GET_I0*/ wire WUXU_STORE4_GET_I0 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XAVE_STORE4_I0n_odd.qp_new());
  /* p30.WEPY_STORE4_GET_I1*/ wire WEPY_STORE4_GET_I1 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XEFE_STORE4_I1n_odd.qp_new());
  /* p30.WERU_STORE4_GET_I2*/ wire WERU_STORE4_GET_I2 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.WANU_STORE4_I2n_odd.qp_new());
  /* p30.XYRE_STORE4_GET_I3*/ wire XYRE_STORE4_GET_I3 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XABO_STORE4_I3n_odd.qp_new());
  /* p30.WOXY_STORE4_GET_L0*/ wire WOXY_STORE4_GET_L0 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XEGE_STORE4_I4n_odd.qp_new());
  /* p30.WAJA_STORE4_GET_L1*/ wire WAJA_STORE4_GET_L1 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XYNU_STORE4_I5n_odd.qp_new());
  /* p30.BUCE_STORE4_GET_L2*/ wire BUCE_STORE4_GET_L2 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.CAPO_STORE4_L0n_odd.qp_new());
  /* p30.BEVY_STORE4_GET_L3*/ wire BEVY_STORE4_GET_L3 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.CAJU_STORE4_L1n_odd.qp_new());
  /* p30.BOVE_STORE4_GET_L4*/ wire BOVE_STORE4_GET_L4 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.CONO_STORE4_L2n_odd.qp_new());
  /* p30.BYDO_STORE4_GET_L5*/ wire BYDO_STORE4_GET_L5 = tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.CUMU_STORE4_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(WUXU_STORE4_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(WEPY_STORE4_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(WERU_STORE4_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(XYRE_STORE4_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(WOXY_STORE4_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(WAJA_STORE4_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(BUCE_STORE4_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(BEVY_STORE4_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(BOVE_STORE4_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(BYDO_STORE4_GET_L5);

  /* p30.DOBO_STORE5_GET_I0*/ wire DOBO_STORE5_GET_I0 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.EKOP_STORE5_I0n_odd.qp_new());
  /* p30.DYNY_STORE5_GET_I1*/ wire DYNY_STORE5_GET_I1 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ETYM_STORE5_I1n_odd.qp_new());
  /* p30.WAGA_STORE5_GET_I2*/ wire WAGA_STORE5_GET_I2 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.GORU_STORE5_I2n_odd.qp_new());
  /* p30.DUZA_STORE5_GET_I3*/ wire DUZA_STORE5_GET_I3 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.EBEX_STORE5_I3n_odd.qp_new());
  /* p30.DALY_STORE5_GET_L0*/ wire DALY_STORE5_GET_L0 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ETAV_STORE5_I4n_odd.qp_new());
  /* p30.DALO_STORE5_GET_L1*/ wire DALO_STORE5_GET_L1 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.EKAP_STORE5_I5n_odd.qp_new());
  /* p30.BACE_STORE5_GET_L2*/ wire BACE_STORE5_GET_L2 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ACEP_STORE5_L0n_odd.qp_new());
  /* p30.BUJA_STORE5_GET_L3*/ wire BUJA_STORE5_GET_L3 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ABEG_STORE5_L1n_odd.qp_new());
  /* p30.BODU_STORE5_GET_L4*/ wire BODU_STORE5_GET_L4 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ABUX_STORE5_L2n_odd.qp_new());
  /* p30.AWAT_STORE5_GET_L5*/ wire AWAT_STORE5_GET_L5 = tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ANED_STORE5_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(DOBO_STORE5_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(DYNY_STORE5_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(WAGA_STORE5_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(DUZA_STORE5_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(DALY_STORE5_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(DALO_STORE5_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(BACE_STORE5_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(BUJA_STORE5_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(BODU_STORE5_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(AWAT_STORE5_GET_L5);

  /* p30.WATO_STORE6_GET_I0*/ wire WATO_STORE6_GET_I0 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.GABO_STORE6_I0n_odd.qp_new());
  /* p30.WYWY_STORE6_GET_I1*/ wire WYWY_STORE6_GET_I1 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.GACY_STORE6_I1n_odd.qp_new());
  /* p30.EZOC_STORE6_GET_I2*/ wire EZOC_STORE6_GET_I2 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.FOGO_STORE6_I2n_odd.qp_new());
  /* p30.WABO_STORE6_GET_I3*/ wire WABO_STORE6_GET_I3 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.GOHU_STORE6_I3n_odd.qp_new());
  /* p30.ELYC_STORE6_GET_L0*/ wire ELYC_STORE6_GET_L0 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.FOXY_STORE6_I4n_odd.qp_new());
  /* p30.WOCY_STORE6_GET_L1*/ wire WOCY_STORE6_GET_L1 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.GECU_STORE6_I5n_odd.qp_new());
  /* p30.YBUK_STORE6_GET_L2*/ wire YBUK_STORE6_GET_L2 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.ZUMY_STORE6_L0n_odd.qp_new());
  /* p30.YKOZ_STORE6_GET_L3*/ wire YKOZ_STORE6_GET_L3 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.ZAFU_STORE6_L1n_odd.qp_new());
  /* p30.ZYTO_STORE6_GET_L4*/ wire ZYTO_STORE6_GET_L4 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.ZEXO_STORE6_L2n_odd.qp_new());
  /* p30.ZUDO_STORE6_GET_L5*/ wire ZUDO_STORE6_GET_L5 = tri6_nn(_XYME_SPRITE6_GETn, sprite_store.ZUBE_STORE6_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(WATO_STORE6_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(WYWY_STORE6_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(EZOC_STORE6_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(WABO_STORE6_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(ELYC_STORE6_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(WOCY_STORE6_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(YBUK_STORE6_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(YKOZ_STORE6_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(ZYTO_STORE6_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(ZUDO_STORE6_GET_L5);

  /* p30.WAKO_STORE7_GET_I0*/ wire WAKO_STORE7_GET_I0 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.GULE_STORE7_I0n_odd.qp_new());
  /* p30.WYGO_STORE7_GET_I1*/ wire WYGO_STORE7_GET_I1 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.GYNO_STORE7_I1n_odd.qp_new());
  /* p30.ELEP_STORE7_GET_I2*/ wire ELEP_STORE7_GET_I2 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.FEFA_STORE7_I2n_odd.qp_new());
  /* p30.ETAD_STORE7_GET_I3*/ wire ETAD_STORE7_GET_I3 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.FYSU_STORE7_I3n_odd.qp_new());
  /* p30.WABA_STORE7_GET_L0*/ wire WABA_STORE7_GET_L0 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.GESY_STORE7_I4n_odd.qp_new());
  /* p30.EVYT_STORE7_GET_L1*/ wire EVYT_STORE7_GET_L1 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.FUZO_STORE7_I5n_odd.qp_new());
  /* p30.WAXE_STORE7_GET_L2*/ wire WAXE_STORE7_GET_L2 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.XYNA_STORE7_L0n_odd.qp_new());
  /* p30.YPOZ_STORE7_GET_L3*/ wire YPOZ_STORE7_GET_L3 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.YGUM_STORE7_L1n_odd.qp_new());
  /* p30.WABU_STORE7_GET_L4*/ wire WABU_STORE7_GET_L4 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.XAKU_STORE7_L2n_odd.qp_new());
  /* p30.WANA_STORE7_GET_L5*/ wire WANA_STORE7_GET_L5 = tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.XYGO_STORE7_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(WAKO_STORE7_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(WYGO_STORE7_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(ELEP_STORE7_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(ETAD_STORE7_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(WABA_STORE7_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(EVYT_STORE7_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(WAXE_STORE7_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(YPOZ_STORE7_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(WABU_STORE7_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(WANA_STORE7_GET_L5);

  /* p30.APOC_STORE8_GET_I0*/ wire APOC_STORE8_GET_I0 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AXUV_STORE8_I0n_odd.qp_new());
  /* p30.AKYH_STORE8_GET_I1*/ wire AKYH_STORE8_GET_I1 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.BADA_STORE8_I1n_odd.qp_new());
  /* p30.AFEN_STORE8_GET_I2*/ wire AFEN_STORE8_GET_I2 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.APEV_STORE8_I2n_odd.qp_new());
  /* p30.APYV_STORE8_GET_I3*/ wire APYV_STORE8_GET_I3 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.BADO_STORE8_I3n_odd.qp_new());
  /* p30.APOB_STORE8_GET_L0*/ wire APOB_STORE8_GET_L0 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.BEXY_STORE8_I4n_odd.qp_new());
  /* p30.ADYB_STORE8_GET_L1*/ wire ADYB_STORE8_GET_L1 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.BYHE_STORE8_I5n_odd.qp_new());
  /* p30.BOSO_STORE8_GET_L2*/ wire BOSO_STORE8_GET_L2 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AZAP_STORE8_L0n_odd.qp_new());
  /* p30.BAZU_STORE8_GET_L3*/ wire BAZU_STORE8_GET_L3 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AFYX_STORE8_L1n_odd.qp_new());
  /* p30.AHAC_STORE8_GET_L4*/ wire AHAC_STORE8_GET_L4 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AFUT_STORE8_L2n_odd.qp_new());
  /* p30.BUJY_STORE8_GET_L5*/ wire BUJY_STORE8_GET_L5 = tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AFYM_STORE8_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(APOC_STORE8_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(AKYH_STORE8_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(AFEN_STORE8_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(APYV_STORE8_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(APOB_STORE8_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(ADYB_STORE8_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(BOSO_STORE8_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(BAZU_STORE8_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(AHAC_STORE8_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(BUJY_STORE8_GET_L5);

  /* p30.ZARO_STORE9_GET_I0*/ wire ZARO_STORE9_GET_I0 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.YBER_STORE9_I0n_odd.qp_new());
  /* p30.ZOJY_STORE9_GET_I1*/ wire ZOJY_STORE9_GET_I1 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.YZOR_STORE9_I1n_odd.qp_new());
  /* p30.YNEV_STORE9_GET_I2*/ wire YNEV_STORE9_GET_I2 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.XYFE_STORE9_I2n_odd.qp_new());
  /* p30.XYRA_STORE9_GET_I3*/ wire XYRA_STORE9_GET_I3 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.XOTU_STORE9_I3n_odd.qp_new());
  /* p30.YRAD_STORE9_GET_L0*/ wire YRAD_STORE9_GET_L0 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.XUTE_STORE9_I4n_odd.qp_new());
  /* p30.YHAL_STORE9_GET_L1*/ wire YHAL_STORE9_GET_L1 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.XUFO_STORE9_I5n_odd.qp_new());
  /* p30.BYME_STORE9_GET_L2*/ wire BYME_STORE9_GET_L2 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.CANA_STORE9_L0n_odd.qp_new());
  /* p30.GATE_STORE9_GET_L3*/ wire GATE_STORE9_GET_L3 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.FOFO_STORE9_L1n_odd.qp_new());
  /* p30.COHO_STORE9_GET_L4*/ wire COHO_STORE9_GET_L4 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.DYSY_STORE9_L2n_odd.qp_new());
  /* p30.CAWO_STORE9_GET_L5*/ wire CAWO_STORE9_GET_L5 = tri6_nn(_FADO_SPRITE9_GETn, sprite_store.DEWU_STORE9_L3n_odd.qp_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(ZARO_STORE9_GET_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(ZOJY_STORE9_GET_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(YNEV_STORE9_GET_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(XYRA_STORE9_GET_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(YRAD_STORE9_GET_L0);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(YHAL_STORE9_GET_L1);
  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(BYME_STORE9_GET_L2);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(GATE_STORE9_GET_L3);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(COHO_STORE9_GET_L4);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(CAWO_STORE9_GET_L5);
}

void GateBoy::sprite_scan_to_bus(SpriteDeltaY sprite_delta_y, NorLatch XYMU_RENDERINGn, Gate FEPO_STORE_MATCHp)
{
  /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn_new = and2(sprite_scanner.CENO_SCANNINGn.qn_new(), XYMU_RENDERINGn.qn_new());
  /*#p30.WUZY_STORE_I0*/ wire WUZY_STORE_I0 = tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XADU_SPRITE_IDX0p.qn_new());
  /* p30.WYSE_STORE_I1*/ wire WYSE_STORE_I1 = tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XEDY_SPRITE_IDX1p.qn_new());
  /* p30.ZYSU_STORE_I2*/ wire ZYSU_STORE_I2 = tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.ZUZE_SPRITE_IDX2p.qn_new());
  /* p30.WYDA_STORE_I3*/ wire WYDA_STORE_I3 = tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XOBE_SPRITE_IDX3p.qn_new());
  /* p30.WUCO_STORE_I4*/ wire WUCO_STORE_I4 = tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.YDUF_SPRITE_IDX4p.qn_new());
  /* p30.WEZA_STORE_I5*/ wire WEZA_STORE_I5 = tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XECU_SPRITE_IDX5p.qn_new());

  /* BUS_SPR_I0*/ sprite_bus.BUS_SPR_I0.tri_bus(WUZY_STORE_I0);
  /* BUS_SPR_I1*/ sprite_bus.BUS_SPR_I1.tri_bus(WYSE_STORE_I1);
  /* BUS_SPR_I2*/ sprite_bus.BUS_SPR_I2.tri_bus(ZYSU_STORE_I2);
  /* BUS_SPR_I3*/ sprite_bus.BUS_SPR_I3.tri_bus(WYDA_STORE_I3);
  /* BUS_SPR_I4*/ sprite_bus.BUS_SPR_I4.tri_bus(WUCO_STORE_I4);
  /* BUS_SPR_I5*/ sprite_bus.BUS_SPR_I5.tri_bus(WEZA_STORE_I5);

  /*#p30.CUCU_STORE_L0*/ wire CUCU_STORE_L0 = tri6_nn(FEPO_STORE_MATCHp.qp_new(), sprite_delta_y.DEGE_SPRITE_DELTA0);
  /*#p30.CUCA_STORE_L1*/ wire CUCA_STORE_L1 = tri6_nn(FEPO_STORE_MATCHp.qp_new(), sprite_delta_y.DABY_SPRITE_DELTA1);
  /*#p30.CEGA_STORE_L2*/ wire CEGA_STORE_L2 = tri6_nn(FEPO_STORE_MATCHp.qp_new(), sprite_delta_y.DABU_SPRITE_DELTA2);
  /*#p30.WENU_STORE_L3*/ wire WENU_STORE_L3 = tri6_nn(FEPO_STORE_MATCHp.qp_new(), sprite_delta_y.GYSA_SPRITE_DELTA3);

  /* BUS_SPR_L0*/ sprite_bus.BUS_SPR_L0.tri_bus(CUCU_STORE_L0);
  /* BUS_SPR_L1*/ sprite_bus.BUS_SPR_L1.tri_bus(CUCA_STORE_L1);
  /* BUS_SPR_L2*/ sprite_bus.BUS_SPR_L2.tri_bus(CEGA_STORE_L2);
  /* BUS_SPR_L3*/ sprite_bus.BUS_SPR_L3.tri_bus(WENU_STORE_L3);
}

//-----------------------------------------------------------------------------
