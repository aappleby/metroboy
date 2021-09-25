#include "GateBoyLib/GateBoySpriteStore.h"

#include "GateBoyLib/GateBoy.h"

#pragma warning(disable:4458)

//-----------------------------------------------------------------------------




























//-----------------------------------------------------------------------------
// Turn the sprite counter into a one-hot clock signal.

void GateBoy::update_sprite_store_flags_gates(
  SpriteCounter& sprite_counter,
  wire DYTY_COUNT_CLKp,
  SpriteStoreFlags& sprite_store_flags)
{
  /*_p29.EDEN*/ wire EDEN_SPRITE_COUNT0n = not1(sprite_counter.BESE_SPRITE_COUNT0_odd.qp_new());
  /*_p29.CYPY*/ wire CYPY_SPRITE_COUNT1n = not1(sprite_counter.CUXY_SPRITE_COUNT1_odd.qp_new());
  /*_p29.CAPE*/ wire CAPE_SPRITE_COUNT2n = not1(sprite_counter.BEGO_SPRITE_COUNT2_odd.qp_new());
  /*_p29.CAXU*/ wire CAXU_SPRITE_COUNT3n = not1(sprite_counter.DYBE_SPRITE_COUNT3_odd.qp_new());

  /*_p29.FYCU*/ wire FYCU_SPRITE_COUNT0p = not1(EDEN_SPRITE_COUNT0n);
  /*_p29.FONE*/ wire FONE_SPRITE_COUNT1p = not1(CYPY_SPRITE_COUNT1n);
  /*_p29.EKUD*/ wire EKUD_SPRITE_COUNT2p = not1(CAPE_SPRITE_COUNT2n);
  /*_p29.ELYG*/ wire ELYG_SPRITE_COUNT3p = not1(CAXU_SPRITE_COUNT3n);

  /*_p29.DEZO*/ wire DEZO_STORE0_SELn = nand4(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
  /*_p29.CUVA*/ wire CUVA_STORE1_SELn = nand4(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
  /*_p29.GEBU*/ wire GEBU_STORE2_SELn = nand4(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
  /*_p29.FOCO*/ wire FOCO_STORE3_SELn = nand4(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
  /*_p29.CUPE*/ wire CUPE_STORE4_SELn = nand4(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
  /*_p29.CUGU*/ wire CUGU_STORE5_SELn = nand4(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
  /*_p29.WOMU*/ wire WOMU_STORE6_SELn = nand4(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
  /*_p29.GUNA*/ wire GUNA_STORE7_SELn = nand4(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
  /*_p29.DEWY*/ wire DEWY_STORE8_SELn = nand4(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);
  /*_p29.DOGU*/ wire DOGU_STORE9_SELn = nand4(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);

  /*_p29.CEMY*/ wire CEMY_STORE0_CLKp = or2(DYTY_COUNT_CLKp, DEZO_STORE0_SELn);
  /*_p29.BYBY*/ wire BYBY_STORE1_CLKp = or2(DYTY_COUNT_CLKp, CUVA_STORE1_SELn);
  /*_p29.WYXO*/ wire WYXO_STORE2_CLKp = or2(DYTY_COUNT_CLKp, GEBU_STORE2_SELn);
  /*_p29.GUVE*/ wire GUVE_STORE3_CLKp = or2(DYTY_COUNT_CLKp, FOCO_STORE3_SELn);
  /*_p29.CECU*/ wire CECU_STORE4_CLKp = or2(DYTY_COUNT_CLKp, CUPE_STORE4_SELn);
  /*_p29.CADO*/ wire CADO_STORE5_CLKp = or2(DYTY_COUNT_CLKp, CUGU_STORE5_SELn);
  /*_p29.XUJO*/ wire XUJO_STORE6_CLKp = or2(DYTY_COUNT_CLKp, WOMU_STORE6_SELn);
  /*_p29.GAPE*/ wire GAPE_STORE7_CLKp = or2(DYTY_COUNT_CLKp, GUNA_STORE7_SELn);
  /*_p29.CAHO*/ wire CAHO_STORE8_CLKp = or2(DYTY_COUNT_CLKp, DEWY_STORE8_SELn);
  /*_p29.CATO*/ wire CATO_STORE9_CLKp = or2(DYTY_COUNT_CLKp, DOGU_STORE9_SELn);

  /*_p29.DYHU*/ sprite_store_flags.DYHU_STORE0_CLKn = not1(CEMY_STORE0_CLKp);
  /*_p29.BUCO*/ sprite_store_flags.BUCO_STORE1_CLKn = not1(BYBY_STORE1_CLKp);
  /*_p29.GYFO*/ sprite_store_flags.GYFO_STORE2_CLKn = not1(WYXO_STORE2_CLKp);
  /*_p29.GUSA*/ sprite_store_flags.GUSA_STORE3_CLKn = not1(GUVE_STORE3_CLKp);
  /*_p29.DUKE*/ sprite_store_flags.DUKE_STORE4_CLKn = not1(CECU_STORE4_CLKp);
  /*_p29.BEDE*/ sprite_store_flags.BEDE_STORE5_CLKn = not1(CADO_STORE5_CLKp);
  /*_p29.WEKA*/ sprite_store_flags.WEKA_STORE6_CLKn = not1(XUJO_STORE6_CLKp);
  /*_p29.GYVO*/ sprite_store_flags.GYVO_STORE7_CLKn = not1(GAPE_STORE7_CLKp);
  /*_p29.BUKA*/ sprite_store_flags.BUKA_STORE8_CLKn = not1(CAHO_STORE8_CLKp);
  /*_p29.DECU*/ sprite_store_flags.DECU_STORE9_CLKn = not1(CATO_STORE9_CLKp);
}































































//-----------------------------------------------------------------------------

void GateBoy::store_sprite_gates(
  SpriteStoreFlags& /*sprite_store_flags_old*/,
  SpriteStoreFlags& sprite_store_flags_new,
  SpriteResetFlags& sprite_reset_flags,
  wire BYVA_LINE_RSTn,
  OamTempB& oam_temp_b_old)
{

  /*_p29.FUXU*/ wire FUXU_STORE0_CLKp = not1(sprite_store_flags_new.DYHU_STORE0_CLKn.out_new());
  /*_p29.ASYS*/ wire ASYS_STORE1_CLKp = not1(sprite_store_flags_new.BUCO_STORE1_CLKn.out_new());
  /*_p29.CACU*/ wire CACU_STORE2_CLKp = not1(sprite_store_flags_new.GYFO_STORE2_CLKn.out_new());
  /*_p29.YFAG*/ wire YFAG_STORE3_CLKp = not1(sprite_store_flags_new.GUSA_STORE3_CLKn.out_new());
  /*_p29.WOFO*/ wire WOFO_STORE4_CLKp = not1(sprite_store_flags_new.DUKE_STORE4_CLKn.out_new());
  /*_p29.CYLA*/ wire CYLA_STORE5_CLKp = not1(sprite_store_flags_new.BEDE_STORE5_CLKn.out_new());
  /*_p29.ZAPE*/ wire ZAPE_STORE6_CLKp = not1(sprite_store_flags_new.WEKA_STORE6_CLKn.out_new());
  /*_p29.GECY*/ wire GECY_STORE7_CLKp = not1(sprite_store_flags_new.GYVO_STORE7_CLKn.out_new());
  /*_p29.CEXU*/ wire CEXU_STORE8_CLKp = not1(sprite_store_flags_new.BUKA_STORE8_CLKn.out_new());
  /*_p29.WEME*/ wire WEME_STORE9_CLKp = not1(sprite_store_flags_new.DECU_STORE9_CLKn.out_new());

  /*_p29.DYBA*/ wire DYBA_LINE_RSTp = not1(BYVA_LINE_RSTn);
  /*_p29.DYWE*/ wire DYWE_STORE0_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.EBOJ_STORE0_RSTp.qp_new());
  /*_p29.EFEV*/ wire EFEV_STORE1_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.CEDY_STORE1_RSTp.qp_new());
  /*_p29.FOKO*/ wire FOKO_STORE2_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.EGAV_STORE2_RSTp.qp_new());
  /*_p29.GAKE*/ wire GAKE_STORE3_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.GOTA_STORE3_RSTp.qp_new());
  /*_p29.WOHU*/ wire WOHU_STORE4_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.XUDY_STORE4_RSTp.qp_new());
  /*_p29.FEVE*/ wire FEVE_STORE5_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.WAFY_STORE5_RSTp.qp_new());
  /*_p29.WACY*/ wire WACY_STORE6_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.WOMY_STORE6_RSTp.qp_new());
  /*_p29.GUKY*/ wire GUKY_STORE7_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.WAPO_STORE7_RSTp.qp_new());
  /*_p29.GORO*/ wire GORO_STORE8_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.EXUQ_STORE8_RSTp.qp_new());
  /*_p29.DUBU*/ wire DUBU_STORE9_RSTp = or2(DYBA_LINE_RSTp, sprite_reset_flags.FONO_STORE9_RSTp.qp_new());

  /*_p29.DYNA*/ wire DYNA_STORE0_RSTn = not1(DYWE_STORE0_RSTp);
  /*_p29.DOKU*/ wire DOKU_STORE1_RSTn = not1(EFEV_STORE1_RSTp);
  /*_p29.GAMY*/ wire GAMY_STORE2_RSTn = not1(FOKO_STORE2_RSTp);
  /*_p29.WUPA*/ wire WUPA_STORE3_RSTn = not1(GAKE_STORE3_RSTp);
  /*_p29.WUNU*/ wire WUNU_STORE4_RSTn = not1(WOHU_STORE4_RSTp);
  /*_p29.EJAD*/ wire EJAD_STORE5_RSTn = not1(FEVE_STORE5_RSTp);
  /*_p29.XAHO*/ wire XAHO_STORE6_RSTn = not1(WACY_STORE6_RSTp);
  /*_p29.GAFY*/ wire GAFY_STORE7_RSTn = not1(GUKY_STORE7_RSTp);
  /*_p29.WUZO*/ wire WUZO_STORE8_RSTn = not1(GORO_STORE8_RSTp);
  /*_p29.DOSY*/ wire DOSY_STORE9_RSTn = not1(DUBU_STORE9_RSTp);

  // Clock the scanned sprite's X coordinate into the selected sprite store.

  /*#p31.ZAGO*/ wire ZAGO_SPX0n_old = not1(oam_temp_b_old.YLOR_OAM_DB0p.qp_old());
  /*_p31.ZOCY*/ wire ZOCY_SPX1n_old = not1(oam_temp_b_old.ZYTY_OAM_DB1p.qp_old());
  /*_p31.YPUR*/ wire YPUR_SPX2n_old = not1(oam_temp_b_old.ZYVE_OAM_DB2p.qp_old());
  /*_p31.YVOK*/ wire YVOK_SPX3n_old = not1(oam_temp_b_old.ZEZY_OAM_DB3p.qp_old());
  /*_p31.COSE*/ wire COSE_SPX4n_old = not1(oam_temp_b_old.GOMO_OAM_DB4p.qp_old());
  /*_p31.AROP*/ wire AROP_SPX5n_old = not1(oam_temp_b_old.BAXO_OAM_DB5p.qp_old());
  /*_p31.XATU*/ wire XATU_SPX6n_old = not1(oam_temp_b_old.YZOS_OAM_DB6p.qp_old());
  /*_p31.BADY*/ wire BADY_SPX7n_old = not1(oam_temp_b_old.DEPO_OAM_DB7p.qp_old());

  /*#p31.XEPE*/ gb_state.store_x0.XEPE_STORE0_X0p.dff9(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZAGO_SPX0n_old);
  /*_p31.YLAH*/ gb_state.store_x0.YLAH_STORE0_X1p.dff9(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZOCY_SPX1n_old);
  /*_p31.ZOLA*/ gb_state.store_x0.ZOLA_STORE0_X2p.dff9(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YPUR_SPX2n_old);
  /*_p31.ZULU*/ gb_state.store_x0.ZULU_STORE0_X3p.dff9(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YVOK_SPX3n_old);
  /*_p31.WELO*/ gb_state.store_x0.WELO_STORE0_X4p.dff9(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, COSE_SPX4n_old);
  /*_p31.XUNY*/ gb_state.store_x0.XUNY_STORE0_X5p.dff9(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, AROP_SPX5n_old);
  /*_p31.WOTE*/ gb_state.store_x0.WOTE_STORE0_X6p.dff9(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, XATU_SPX6n_old);
  /*_p31.XAKO*/ gb_state.store_x0.XAKO_STORE0_X7p.dff9(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, BADY_SPX7n_old);

  /*_p31.DANY*/ gb_state.store_x1.DANY_STORE1_X0p.dff9(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZAGO_SPX0n_old);
  /*_p31.DUKO*/ gb_state.store_x1.DUKO_STORE1_X1p.dff9(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZOCY_SPX1n_old);
  /*_p31.DESU*/ gb_state.store_x1.DESU_STORE1_X2p.dff9(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YPUR_SPX2n_old);
  /*_p31.DAZO*/ gb_state.store_x1.DAZO_STORE1_X3p.dff9(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YVOK_SPX3n_old);
  /*_p31.DAKE*/ gb_state.store_x1.DAKE_STORE1_X4p.dff9(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, COSE_SPX4n_old);
  /*_p31.CESO*/ gb_state.store_x1.CESO_STORE1_X5p.dff9(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, AROP_SPX5n_old);
  /*_p31.DYFU*/ gb_state.store_x1.DYFU_STORE1_X6p.dff9(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, XATU_SPX6n_old);
  /*_p31.CUSY*/ gb_state.store_x1.CUSY_STORE1_X7p.dff9(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, BADY_SPX7n_old);

  /*_p31.FOKA*/ gb_state.store_x2.FOKA_STORE2_X0p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZAGO_SPX0n_old);
  /*_p31.FYTY*/ gb_state.store_x2.FYTY_STORE2_X1p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZOCY_SPX1n_old);
  /*_p31.FUBY*/ gb_state.store_x2.FUBY_STORE2_X2p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YPUR_SPX2n_old);
  /*_p31.GOXU*/ gb_state.store_x2.GOXU_STORE2_X3p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YVOK_SPX3n_old);
  /*_p31.DUHY*/ gb_state.store_x2.DUHY_STORE2_X4p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, COSE_SPX4n_old);
  /*_p31.EJUF*/ gb_state.store_x2.EJUF_STORE2_X5p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, AROP_SPX5n_old);
  /*_p31.ENOR*/ gb_state.store_x2.ENOR_STORE2_X6p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, XATU_SPX6n_old);
  /*_p31.DEPY*/ gb_state.store_x2.DEPY_STORE2_X7p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, BADY_SPX7n_old);

  /*_p31.XOLY*/ gb_state.store_x3.XOLY_STORE3_X0p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, ZAGO_SPX0n_old);
  /*_p31.XYBA*/ gb_state.store_x3.XYBA_STORE3_X1p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, ZOCY_SPX1n_old);
  /*_p31.XABE*/ gb_state.store_x3.XABE_STORE3_X2p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, YPUR_SPX2n_old);
  /*_p31.XEKA*/ gb_state.store_x3.XEKA_STORE3_X3p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, YVOK_SPX3n_old);
  /*_p31.XOMY*/ gb_state.store_x3.XOMY_STORE3_X4p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, COSE_SPX4n_old);
  /*_p31.WUHA*/ gb_state.store_x3.WUHA_STORE3_X5p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, AROP_SPX5n_old);
  /*_p31.WYNA*/ gb_state.store_x3.WYNA_STORE3_X6p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, XATU_SPX6n_old);
  /*_p31.WECO*/ gb_state.store_x3.WECO_STORE3_X7p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, BADY_SPX7n_old);

  /*_p31.WEDU*/ gb_state.store_x4.WEDU_STORE4_X0p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, ZAGO_SPX0n_old);
  /*_p31.YGAJ*/ gb_state.store_x4.YGAJ_STORE4_X1p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, ZOCY_SPX1n_old);
  /*_p31.ZYJO*/ gb_state.store_x4.ZYJO_STORE4_X2p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, YPUR_SPX2n_old);
  /*_p31.XURY*/ gb_state.store_x4.XURY_STORE4_X3p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, YVOK_SPX3n_old);
  /*_p31.YBED*/ gb_state.store_x4.YBED_STORE4_X4p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, COSE_SPX4n_old);
  /*_p31.ZALA*/ gb_state.store_x4.ZALA_STORE4_X5p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, AROP_SPX5n_old);
  /*_p31.WYDE*/ gb_state.store_x4.WYDE_STORE4_X6p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, XATU_SPX6n_old);
  /*_p31.XEPA*/ gb_state.store_x4.XEPA_STORE4_X7p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, BADY_SPX7n_old);

  /*_p31.FUSA*/ gb_state.store_x5.FUSA_STORE5_X0p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, ZAGO_SPX0n_old);
  /*_p31.FAXA*/ gb_state.store_x5.FAXA_STORE5_X1p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, ZOCY_SPX1n_old);
  /*_p31.FOZY*/ gb_state.store_x5.FOZY_STORE5_X2p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, YPUR_SPX2n_old);
  /*_p31.FESY*/ gb_state.store_x5.FESY_STORE5_X3p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, YVOK_SPX3n_old);
  /*_p31.CYWE*/ gb_state.store_x5.CYWE_STORE5_X4p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, COSE_SPX4n_old);
  /*_p31.DYBY*/ gb_state.store_x5.DYBY_STORE5_X5p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, AROP_SPX5n_old);
  /*_p31.DURY*/ gb_state.store_x5.DURY_STORE5_X6p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, XATU_SPX6n_old);
  /*_p31.CUVY*/ gb_state.store_x5.CUVY_STORE5_X7p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, BADY_SPX7n_old);

  /*_p31.YCOL*/ gb_state.store_x6.YCOL_STORE6_X0p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, ZAGO_SPX0n_old);
  /*_p31.YRAC*/ gb_state.store_x6.YRAC_STORE6_X1p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, ZOCY_SPX1n_old);
  /*_p31.YMEM*/ gb_state.store_x6.YMEM_STORE6_X2p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, YPUR_SPX2n_old);
  /*_p31.YVAG*/ gb_state.store_x6.YVAG_STORE6_X3p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, YVOK_SPX3n_old);
  /*_p31.ZOLY*/ gb_state.store_x6.ZOLY_STORE6_X4p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, COSE_SPX4n_old);
  /*_p31.ZOGO*/ gb_state.store_x6.ZOGO_STORE6_X5p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, AROP_SPX5n_old);
  /*_p31.ZECU*/ gb_state.store_x6.ZECU_STORE6_X6p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, XATU_SPX6n_old);
  /*_p31.ZESA*/ gb_state.store_x6.ZESA_STORE6_X7p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, BADY_SPX7n_old);

  /*_p31.ERAZ*/ gb_state.store_x7.ERAZ_STORE7_X0p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, ZAGO_SPX0n_old);
  /*_p31.EPUM*/ gb_state.store_x7.EPUM_STORE7_X1p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, ZOCY_SPX1n_old);
  /*_p31.EROL*/ gb_state.store_x7.EROL_STORE7_X2p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, YPUR_SPX2n_old);
  /*_p31.EHYN*/ gb_state.store_x7.EHYN_STORE7_X3p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, YVOK_SPX3n_old);
  /*_p31.FAZU*/ gb_state.store_x7.FAZU_STORE7_X4p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, COSE_SPX4n_old);
  /*_p31.FAXE*/ gb_state.store_x7.FAXE_STORE7_X5p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, AROP_SPX5n_old);
  /*_p31.EXUK*/ gb_state.store_x7.EXUK_STORE7_X6p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, XATU_SPX6n_old);
  /*_p31.FEDE*/ gb_state.store_x7.FEDE_STORE7_X7p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, BADY_SPX7n_old);

  /*_p31.EZUF*/ gb_state.store_x8.EZUF_STORE8_X0p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, ZAGO_SPX0n_old);
  /*_p31.ENAD*/ gb_state.store_x8.ENAD_STORE8_X1p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, ZOCY_SPX1n_old);
  /*_p31.EBOW*/ gb_state.store_x8.EBOW_STORE8_X2p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, YPUR_SPX2n_old);
  /*_p31.FYCA*/ gb_state.store_x8.FYCA_STORE8_X3p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, YVOK_SPX3n_old);
  /*_p31.GAVY*/ gb_state.store_x8.GAVY_STORE8_X4p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, COSE_SPX4n_old);
  /*_p31.GYPU*/ gb_state.store_x8.GYPU_STORE8_X5p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, AROP_SPX5n_old);
  /*_p31.GADY*/ gb_state.store_x8.GADY_STORE8_X6p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, XATU_SPX6n_old);
  /*_p31.GAZA*/ gb_state.store_x8.GAZA_STORE8_X7p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, BADY_SPX7n_old);

  /*_p31.XUVY*/ gb_state.store_x9.XUVY_STORE9_X0p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, ZAGO_SPX0n_old);
  /*_p31.XERE*/ gb_state.store_x9.XERE_STORE9_X1p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, ZOCY_SPX1n_old);
  /*_p31.XUZO*/ gb_state.store_x9.XUZO_STORE9_X2p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, YPUR_SPX2n_old);
  /*_p31.XEXA*/ gb_state.store_x9.XEXA_STORE9_X3p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, YVOK_SPX3n_old);
  /*_p31.YPOD*/ gb_state.store_x9.YPOD_STORE9_X4p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, COSE_SPX4n_old);
  /*_p31.YROP*/ gb_state.store_x9.YROP_STORE9_X5p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, AROP_SPX5n_old);
  /*_p31.YNEP*/ gb_state.store_x9.YNEP_STORE9_X6p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, XATU_SPX6n_old);
  /*_p31.YZOF*/ gb_state.store_x9.YZOF_STORE9_X7p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, BADY_SPX7n_old);

  // Clock the scanned sprite's index into the selected sprite store.

  /*_p29.GENY*/ wire GENY_STORE0_CLKp = not1(sprite_store_flags_new.DYHU_STORE0_CLKn.out_new());
  /*_p29.BYVY*/ wire BYVY_STORE1_CLKp = not1(sprite_store_flags_new.BUCO_STORE1_CLKn.out_new());
  /*_p29.BUZY*/ wire BUZY_STORE2_CLKp = not1(sprite_store_flags_new.GYFO_STORE2_CLKn.out_new());
  /*_p29.FEKA*/ wire FEKA_STORE3_CLKp = not1(sprite_store_flags_new.GUSA_STORE3_CLKn.out_new());
  /*_p29.WYLU*/ wire WYLU_STORE4_CLKp = not1(sprite_store_flags_new.DUKE_STORE4_CLKn.out_new());
  /*_p29.DYMO*/ wire DYMO_STORE5_CLKp = not1(sprite_store_flags_new.BEDE_STORE5_CLKn.out_new());
  /*_p29.WUSE*/ wire WUSE_STORE6_CLKp = not1(sprite_store_flags_new.WEKA_STORE6_CLKn.out_new());
  /*_p29.FEFO*/ wire FEFO_STORE7_CLKp = not1(sprite_store_flags_new.GYVO_STORE7_CLKn.out_new());
  /*_p29.AKOL*/ wire AKOL_STORE8_CLKp = not1(sprite_store_flags_new.BUKA_STORE8_CLKn.out_new());
  /*_p29.WUFA*/ wire WUFA_STORE9_CLKp = not1(sprite_store_flags_new.DECU_STORE9_CLKn.out_new());

  /*_p30.YSOK*/ gb_state.store_i0.YSOK_STORE0_I1n.dff8n(GENY_STORE0_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.YGUS*/ gb_state.store_i0.YGUS_STORE0_I0n.dff8n(GENY_STORE0_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.YZEP*/ gb_state.store_i0.YZEP_STORE0_I2n.dff8n(GENY_STORE0_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.WYTE*/ gb_state.store_i0.WYTE_STORE0_I3n.dff8n(GENY_STORE0_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.ZONY*/ gb_state.store_i0.ZONY_STORE0_I4n.dff8n(GENY_STORE0_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.YWAK*/ gb_state.store_i0.YWAK_STORE0_I5n.dff8n(GENY_STORE0_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  /*_p30.CADU*/ gb_state.store_i1.CADU_STORE1_I0n.dff8n(BYVY_STORE1_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.CEBO*/ gb_state.store_i1.CEBO_STORE1_I1n.dff8n(BYVY_STORE1_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.CUFA*/ gb_state.store_i1.CUFA_STORE1_I2n.dff8n(BYVY_STORE1_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.COMA*/ gb_state.store_i1.COMA_STORE1_I3n.dff8n(BYVY_STORE1_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.CUZA*/ gb_state.store_i1.CUZA_STORE1_I4n.dff8n(BYVY_STORE1_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.CAJY*/ gb_state.store_i1.CAJY_STORE1_I5n.dff8n(BYVY_STORE1_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  /*_p30.BUHE*/ gb_state.store_i2.BUHE_STORE2_I0n.dff8n(BUZY_STORE2_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.BYHU*/ gb_state.store_i2.BYHU_STORE2_I1n.dff8n(BUZY_STORE2_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.BECA*/ gb_state.store_i2.BECA_STORE2_I2n.dff8n(BUZY_STORE2_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.BULU*/ gb_state.store_i2.BULU_STORE2_I3n.dff8n(BUZY_STORE2_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.BUNA*/ gb_state.store_i2.BUNA_STORE2_I4n.dff8n(BUZY_STORE2_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.BOXA*/ gb_state.store_i2.BOXA_STORE2_I5n.dff8n(BUZY_STORE2_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  /*_p30.DEVY*/ gb_state.store_i3.DEVY_STORE3_I0n.dff8n(FEKA_STORE3_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.DESE*/ gb_state.store_i3.DESE_STORE3_I1n.dff8n(FEKA_STORE3_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.DUNY*/ gb_state.store_i3.DUNY_STORE3_I2n.dff8n(FEKA_STORE3_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.DUHA*/ gb_state.store_i3.DUHA_STORE3_I3n.dff8n(FEKA_STORE3_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.DEBA*/ gb_state.store_i3.DEBA_STORE3_I4n.dff8n(FEKA_STORE3_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.DAFU*/ gb_state.store_i3.DAFU_STORE3_I5n.dff8n(FEKA_STORE3_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  /*_p30.XAVE*/ gb_state.store_i4.XAVE_STORE4_I0n.dff8n(WYLU_STORE4_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.XEFE*/ gb_state.store_i4.XEFE_STORE4_I1n.dff8n(WYLU_STORE4_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.WANU*/ gb_state.store_i4.WANU_STORE4_I2n.dff8n(WYLU_STORE4_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.XABO*/ gb_state.store_i4.XABO_STORE4_I3n.dff8n(WYLU_STORE4_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.XEGE*/ gb_state.store_i4.XEGE_STORE4_I4n.dff8n(WYLU_STORE4_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.XYNU*/ gb_state.store_i4.XYNU_STORE4_I5n.dff8n(WYLU_STORE4_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  /*_p30.EKOP*/ gb_state.store_i5.EKOP_STORE5_I0n.dff8n(DYMO_STORE5_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.ETYM*/ gb_state.store_i5.ETYM_STORE5_I1n.dff8n(DYMO_STORE5_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.GORU*/ gb_state.store_i5.GORU_STORE5_I2n.dff8n(DYMO_STORE5_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.EBEX*/ gb_state.store_i5.EBEX_STORE5_I3n.dff8n(DYMO_STORE5_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.ETAV*/ gb_state.store_i5.ETAV_STORE5_I4n.dff8n(DYMO_STORE5_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.EKAP*/ gb_state.store_i5.EKAP_STORE5_I5n.dff8n(DYMO_STORE5_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  /*_p30.GABO*/ gb_state.store_i6.GABO_STORE6_I0n.dff8n(WUSE_STORE6_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.GACY*/ gb_state.store_i6.GACY_STORE6_I1n.dff8n(WUSE_STORE6_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.FOGO*/ gb_state.store_i6.FOGO_STORE6_I2n.dff8n(WUSE_STORE6_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.GOHU*/ gb_state.store_i6.GOHU_STORE6_I3n.dff8n(WUSE_STORE6_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.FOXY*/ gb_state.store_i6.FOXY_STORE6_I4n.dff8n(WUSE_STORE6_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.GECU*/ gb_state.store_i6.GECU_STORE6_I5n.dff8n(WUSE_STORE6_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  /*_p30.GULE*/ gb_state.store_i7.GULE_STORE7_I0n.dff8n(FEFO_STORE7_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.GYNO*/ gb_state.store_i7.GYNO_STORE7_I1n.dff8n(FEFO_STORE7_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.FEFA*/ gb_state.store_i7.FEFA_STORE7_I2n.dff8n(FEFO_STORE7_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.FYSU*/ gb_state.store_i7.FYSU_STORE7_I3n.dff8n(FEFO_STORE7_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.GESY*/ gb_state.store_i7.GESY_STORE7_I4n.dff8n(FEFO_STORE7_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.FUZO*/ gb_state.store_i7.FUZO_STORE7_I5n.dff8n(FEFO_STORE7_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  /*_p30.AXUV*/ gb_state.store_i8.AXUV_STORE8_I0n.dff8n(AKOL_STORE8_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.BADA*/ gb_state.store_i8.BADA_STORE8_I1n.dff8n(AKOL_STORE8_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.APEV*/ gb_state.store_i8.APEV_STORE8_I2n.dff8n(AKOL_STORE8_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.BADO*/ gb_state.store_i8.BADO_STORE8_I3n.dff8n(AKOL_STORE8_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.BEXY*/ gb_state.store_i8.BEXY_STORE8_I4n.dff8n(AKOL_STORE8_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.BYHE*/ gb_state.store_i8.BYHE_STORE8_I5n.dff8n(AKOL_STORE8_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  /*_p30.YBER*/ gb_state.store_i9.YBER_STORE9_I0n.dff8n(WUFA_STORE9_CLKp, gb_state.sprite_ibus.BUS_SPR_I0.out_old());
  /*_p30.YZOR*/ gb_state.store_i9.YZOR_STORE9_I1n.dff8n(WUFA_STORE9_CLKp, gb_state.sprite_ibus.BUS_SPR_I1.out_old());
  /*_p30.XYFE*/ gb_state.store_i9.XYFE_STORE9_I2n.dff8n(WUFA_STORE9_CLKp, gb_state.sprite_ibus.BUS_SPR_I2.out_old());
  /*_p30.XOTU*/ gb_state.store_i9.XOTU_STORE9_I3n.dff8n(WUFA_STORE9_CLKp, gb_state.sprite_ibus.BUS_SPR_I3.out_old());
  /*_p30.XUTE*/ gb_state.store_i9.XUTE_STORE9_I4n.dff8n(WUFA_STORE9_CLKp, gb_state.sprite_ibus.BUS_SPR_I4.out_old());
  /*_p30.XUFO*/ gb_state.store_i9.XUFO_STORE9_I5n.dff8n(WUFA_STORE9_CLKp, gb_state.sprite_ibus.BUS_SPR_I5.out_old());

  // Clock the scanned sprite's current line into the selected sprite store.

  /*_p29.ENOB*/ wire ENOB_STORE0_CLKp = not1(sprite_store_flags_new.DYHU_STORE0_CLKn.out_new());
  /*_p29.AHOF*/ wire AHOF_STORE1_CLKp = not1(sprite_store_flags_new.BUCO_STORE1_CLKn.out_new());
  /*_p29.FUKE*/ wire FUKE_STORE2_CLKp = not1(sprite_store_flags_new.GYFO_STORE2_CLKn.out_new());
  /*_p29.XYHA*/ wire XYHA_STORE3_CLKp = not1(sprite_store_flags_new.GUSA_STORE3_CLKn.out_new());
  /*_p29.EWOT*/ wire EWOT_STORE4_CLKp = not1(sprite_store_flags_new.DUKE_STORE4_CLKn.out_new());
  /*_p29.BUCY*/ wire BUCY_STORE5_CLKp = not1(sprite_store_flags_new.BEDE_STORE5_CLKn.out_new());
  /*_p29.ZURU*/ wire ZURU_STORE6_CLKp = not1(sprite_store_flags_new.WEKA_STORE6_CLKn.out_new());
  /*_p29.WABE*/ wire WABE_STORE7_CLKp = not1(sprite_store_flags_new.GYVO_STORE7_CLKn.out_new());
  /*_p29.BYMY*/ wire BYMY_STORE8_CLKp = not1(sprite_store_flags_new.BUKA_STORE8_CLKn.out_new());
  /*_p29.FAKA*/ wire FAKA_STORE9_CLKp = not1(sprite_store_flags_new.DECU_STORE9_CLKn.out_new());

  /*_p30.GYHO*/ gb_state.store_l0.GYHO_STORE0_L0n.dff8n(ENOB_STORE0_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.CUFO*/ gb_state.store_l0.CUFO_STORE0_L1n.dff8n(ENOB_STORE0_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.BOZU*/ gb_state.store_l0.BOZU_STORE0_L2n.dff8n(ENOB_STORE0_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.FYHY*/ gb_state.store_l0.FYHY_STORE0_L3n.dff8n(ENOB_STORE0_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());

  /*_p30.AMES*/ gb_state.store_l1.AMES_STORE1_L0n.dff8n(AHOF_STORE1_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.AROF*/ gb_state.store_l1.AROF_STORE1_L1n.dff8n(AHOF_STORE1_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.ABOP*/ gb_state.store_l1.ABOP_STORE1_L2n.dff8n(AHOF_STORE1_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.ABUG*/ gb_state.store_l1.ABUG_STORE1_L3n.dff8n(AHOF_STORE1_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());

  /*_p30.YLOV*/ gb_state.store_l2.YLOV_STORE2_L0n.dff8n(FUKE_STORE2_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.XOSY*/ gb_state.store_l2.XOSY_STORE2_L1n.dff8n(FUKE_STORE2_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.XAZY*/ gb_state.store_l2.XAZY_STORE2_L2n.dff8n(FUKE_STORE2_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.YKUK*/ gb_state.store_l2.YKUK_STORE2_L3n.dff8n(FUKE_STORE2_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());

  /*_p30.ZURO*/ gb_state.store_l3.ZURO_STORE3_L0n.dff8n(XYHA_STORE3_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.ZYLU*/ gb_state.store_l3.ZYLU_STORE3_L1n.dff8n(XYHA_STORE3_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.ZENE*/ gb_state.store_l3.ZENE_STORE3_L2n.dff8n(XYHA_STORE3_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.ZURY*/ gb_state.store_l3.ZURY_STORE3_L3n.dff8n(XYHA_STORE3_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());

  /*_p30.CAPO*/ gb_state.store_l4.CAPO_STORE4_L0n.dff8n(EWOT_STORE4_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.CAJU*/ gb_state.store_l4.CAJU_STORE4_L1n.dff8n(EWOT_STORE4_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.CONO*/ gb_state.store_l4.CONO_STORE4_L2n.dff8n(EWOT_STORE4_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.CUMU*/ gb_state.store_l4.CUMU_STORE4_L3n.dff8n(EWOT_STORE4_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());

  /*_p30.ACEP*/ gb_state.store_l5.ACEP_STORE5_L0n.dff8n(BUCY_STORE5_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.ABEG*/ gb_state.store_l5.ABEG_STORE5_L1n.dff8n(BUCY_STORE5_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.ABUX*/ gb_state.store_l5.ABUX_STORE5_L2n.dff8n(BUCY_STORE5_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.ANED*/ gb_state.store_l5.ANED_STORE5_L3n.dff8n(BUCY_STORE5_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());

  /*_p30.ZUMY*/ gb_state.store_l6.ZUMY_STORE6_L0n.dff8n(ZURU_STORE6_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.ZAFU*/ gb_state.store_l6.ZAFU_STORE6_L1n.dff8n(ZURU_STORE6_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.ZEXO*/ gb_state.store_l6.ZEXO_STORE6_L2n.dff8n(ZURU_STORE6_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.ZUBE*/ gb_state.store_l6.ZUBE_STORE6_L3n.dff8n(ZURU_STORE6_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());

  /*_p30.XYNA*/ gb_state.store_l7.XYNA_STORE7_L0n.dff8n(WABE_STORE7_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.YGUM*/ gb_state.store_l7.YGUM_STORE7_L1n.dff8n(WABE_STORE7_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.XAKU*/ gb_state.store_l7.XAKU_STORE7_L2n.dff8n(WABE_STORE7_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.XYGO*/ gb_state.store_l7.XYGO_STORE7_L3n.dff8n(WABE_STORE7_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());

  /*_p30.AZAP*/ gb_state.store_l8.AZAP_STORE8_L0n.dff8n(BYMY_STORE8_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.AFYX*/ gb_state.store_l8.AFYX_STORE8_L1n.dff8n(BYMY_STORE8_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.AFUT*/ gb_state.store_l8.AFUT_STORE8_L2n.dff8n(BYMY_STORE8_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.AFYM*/ gb_state.store_l8.AFYM_STORE8_L3n.dff8n(BYMY_STORE8_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());

  /*_p30.CANA*/ gb_state.store_l9.CANA_STORE9_L0n.dff8n(FAKA_STORE9_CLKp, gb_state.sprite_lbus.BUS_SPR_L0.out_old());
  /*_p30.FOFO*/ gb_state.store_l9.FOFO_STORE9_L1n.dff8n(FAKA_STORE9_CLKp, gb_state.sprite_lbus.BUS_SPR_L1.out_old());
  /*_p30.DYSY*/ gb_state.store_l9.DYSY_STORE9_L2n.dff8n(FAKA_STORE9_CLKp, gb_state.sprite_lbus.BUS_SPR_L2.out_old());
  /*_p30.DEWU*/ gb_state.store_l9.DEWU_STORE9_L3n.dff8n(FAKA_STORE9_CLKp, gb_state.sprite_lbus.BUS_SPR_L3.out_old());
}


//-----------------------------------------------------------------------------



















































































//-----------------------------------------------------------------------------
// Check the whole sprite store for a sprite at the current pixel coordinate
// and update the match flags and FEPO_STORE_MATCHp.

void GateBoy::get_sprite_match_flags_gates(wire AROR_MATCH_ENp, SpriteMatchFlags& sprite_match_flags) {
  /*#p21.ACAM*/ wire ACAM_PX0n_odd = not1(gb_state.pix_count.XEHO_PX0p_odd.qp_new());
  /*_p21.AZUB*/ wire AZUB_PX1n_odd = not1(gb_state.pix_count.SAVY_PX1p_odd.qp_new());
  /*_p21.AMEL*/ wire AMEL_PX2n_odd = not1(gb_state.pix_count.XODU_PX2p_odd.qp_new());
  /*_p21.AHAL*/ wire AHAL_PX3n_odd = not1(gb_state.pix_count.XYDO_PX3p_odd.qp_new());
  /*_p21.APUX*/ wire APUX_PX4n_odd = not1(gb_state.pix_count.TUHU_PX4p_odd.qp_new());
  /*_p21.ABEF*/ wire ABEF_PX5n_odd = not1(gb_state.pix_count.TUKY_PX5p_odd.qp_new());
  /*_p21.ADAZ*/ wire ADAZ_PX6n_odd = not1(gb_state.pix_count.TAKO_PX6p_odd.qp_new());
  /*_p21.ASAH*/ wire ASAH_PX7n_odd = not1(gb_state.pix_count.SYBE_PX7p_odd.qp_new());

  /*#p31.ZOGY*/ wire ZOGY_STORE0_MATCH0n = xor2(gb_state.store_x0.XEPE_STORE0_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.ZEBA*/ wire ZEBA_STORE0_MATCH1n = xor2(gb_state.store_x0.YLAH_STORE0_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.ZAHA*/ wire ZAHA_STORE0_MATCH2n = xor2(gb_state.store_x0.ZOLA_STORE0_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.ZOKY*/ wire ZOKY_STORE0_MATCH3n = xor2(gb_state.store_x0.ZULU_STORE0_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.WOJU*/ wire WOJU_STORE0_MATCH4n = xor2(gb_state.store_x0.WELO_STORE0_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.YFUN*/ wire YFUN_STORE0_MATCH5n = xor2(gb_state.store_x0.XUNY_STORE0_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.WYZA*/ wire WYZA_STORE0_MATCH6n = xor2(gb_state.store_x0.WOTE_STORE0_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.YPUK*/ wire YPUK_STORE0_MATCH7n = xor2(gb_state.store_x0.XAKO_STORE0_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.EDYM*/ wire EDYM_STORE1_MATCH0n = xor2(gb_state.store_x1.DANY_STORE1_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.EMYB*/ wire EMYB_STORE1_MATCH1n = xor2(gb_state.store_x1.DUKO_STORE1_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.EBEF*/ wire EBEF_STORE1_MATCH2n = xor2(gb_state.store_x1.DESU_STORE1_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.EWOK*/ wire EWOK_STORE1_MATCH3n = xor2(gb_state.store_x1.DAZO_STORE1_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.COLA*/ wire COLA_STORE1_MATCH4n = xor2(gb_state.store_x1.DAKE_STORE1_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.BOBA*/ wire BOBA_STORE1_MATCH5n = xor2(gb_state.store_x1.CESO_STORE1_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.COLU*/ wire COLU_STORE1_MATCH6n = xor2(gb_state.store_x1.DYFU_STORE1_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.BAHU*/ wire BAHU_STORE1_MATCH7n = xor2(gb_state.store_x1.CUSY_STORE1_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.FUZU*/ wire FUZU_STORE2_MATCH0n = xor2(gb_state.store_x2.FOKA_STORE2_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.FESO*/ wire FESO_STORE2_MATCH1n = xor2(gb_state.store_x2.FYTY_STORE2_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.FOKY*/ wire FOKY_STORE2_MATCH2n = xor2(gb_state.store_x2.FUBY_STORE2_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.FYVA*/ wire FYVA_STORE2_MATCH3n = xor2(gb_state.store_x2.GOXU_STORE2_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.CEKO*/ wire CEKO_STORE2_MATCH4n = xor2(gb_state.store_x2.DUHY_STORE2_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.DETY*/ wire DETY_STORE2_MATCH5n = xor2(gb_state.store_x2.EJUF_STORE2_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.DOZO*/ wire DOZO_STORE2_MATCH6n = xor2(gb_state.store_x2.ENOR_STORE2_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.CONY*/ wire CONY_STORE2_MATCH7n = xor2(gb_state.store_x2.DEPY_STORE2_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.YHOK*/ wire YHOK_STORE3_MATCH0n = xor2(gb_state.store_x3.XOLY_STORE3_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.YCAH*/ wire YCAH_STORE3_MATCH1n = xor2(gb_state.store_x3.XYBA_STORE3_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.YDAJ*/ wire YDAJ_STORE3_MATCH2n = xor2(gb_state.store_x3.XABE_STORE3_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.YVUZ*/ wire YVUZ_STORE3_MATCH3n = xor2(gb_state.store_x3.XEKA_STORE3_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.YVAP*/ wire YVAP_STORE3_MATCH4n = xor2(gb_state.store_x3.XOMY_STORE3_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.XENY*/ wire XENY_STORE3_MATCH5n = xor2(gb_state.store_x3.WUHA_STORE3_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.XAVU*/ wire XAVU_STORE3_MATCH6n = xor2(gb_state.store_x3.WYNA_STORE3_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.XEVA*/ wire XEVA_STORE3_MATCH7n = xor2(gb_state.store_x3.WECO_STORE3_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.XEJU*/ wire XEJU_STORE4_MATCH0n = xor2(gb_state.store_x4.WEDU_STORE4_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.ZATE*/ wire ZATE_STORE4_MATCH1n = xor2(gb_state.store_x4.YGAJ_STORE4_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.ZAKU*/ wire ZAKU_STORE4_MATCH2n = xor2(gb_state.store_x4.ZYJO_STORE4_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.YBOX*/ wire YBOX_STORE4_MATCH3n = xor2(gb_state.store_x4.XURY_STORE4_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.ZYKU*/ wire ZYKU_STORE4_MATCH4n = xor2(gb_state.store_x4.YBED_STORE4_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.ZYPU*/ wire ZYPU_STORE4_MATCH5n = xor2(gb_state.store_x4.ZALA_STORE4_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.XAHA*/ wire XAHA_STORE4_MATCH6n = xor2(gb_state.store_x4.WYDE_STORE4_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.ZEFE*/ wire ZEFE_STORE4_MATCH7n = xor2(gb_state.store_x4.XEPA_STORE4_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.GUZO*/ wire GUZO_STORE5_MATCH0n = xor2(gb_state.store_x5.FUSA_STORE5_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.GOLA*/ wire GOLA_STORE5_MATCH1n = xor2(gb_state.store_x5.FAXA_STORE5_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.GEVE*/ wire GEVE_STORE5_MATCH2n = xor2(gb_state.store_x5.FOZY_STORE5_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.GUDE*/ wire GUDE_STORE5_MATCH3n = xor2(gb_state.store_x5.FESY_STORE5_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.BAZY*/ wire BAZY_STORE5_MATCH4n = xor2(gb_state.store_x5.CYWE_STORE5_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.CYLE*/ wire CYLE_STORE5_MATCH5n = xor2(gb_state.store_x5.DYBY_STORE5_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.CEVA*/ wire CEVA_STORE5_MATCH6n = xor2(gb_state.store_x5.DURY_STORE5_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.BUMY*/ wire BUMY_STORE5_MATCH7n = xor2(gb_state.store_x5.CUVY_STORE5_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.XOSU*/ wire XOSU_STORE6_MATCH0n = xor2(gb_state.store_x6.YCOL_STORE6_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.ZUVU*/ wire ZUVU_STORE6_MATCH1n = xor2(gb_state.store_x6.YRAC_STORE6_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.XUCO*/ wire XUCO_STORE6_MATCH2n = xor2(gb_state.store_x6.YMEM_STORE6_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.ZULO*/ wire ZULO_STORE6_MATCH3n = xor2(gb_state.store_x6.YVAG_STORE6_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.ZARE*/ wire ZARE_STORE6_MATCH4n = xor2(gb_state.store_x6.ZOLY_STORE6_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.ZEMU*/ wire ZEMU_STORE6_MATCH5n = xor2(gb_state.store_x6.ZOGO_STORE6_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.ZYGO*/ wire ZYGO_STORE6_MATCH6n = xor2(gb_state.store_x6.ZECU_STORE6_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.ZUZY*/ wire ZUZY_STORE6_MATCH7n = xor2(gb_state.store_x6.ZESA_STORE6_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.DUSE*/ wire DUSE_STORE7_MATCH0n = xor2(gb_state.store_x7.ERAZ_STORE7_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.DAGU*/ wire DAGU_STORE7_MATCH1n = xor2(gb_state.store_x7.EPUM_STORE7_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.DYZE*/ wire DYZE_STORE7_MATCH2n = xor2(gb_state.store_x7.EROL_STORE7_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.DESO*/ wire DESO_STORE7_MATCH3n = xor2(gb_state.store_x7.EHYN_STORE7_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.EJOT*/ wire EJOT_STORE7_MATCH4n = xor2(gb_state.store_x7.FAZU_STORE7_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.ESAJ*/ wire ESAJ_STORE7_MATCH5n = xor2(gb_state.store_x7.FAXE_STORE7_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.DUCU*/ wire DUCU_STORE7_MATCH6n = xor2(gb_state.store_x7.EXUK_STORE7_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.EWUD*/ wire EWUD_STORE7_MATCH7n = xor2(gb_state.store_x7.FEDE_STORE7_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.DUZE*/ wire DUZE_STORE8_MATCH0n = xor2(gb_state.store_x8.EZUF_STORE8_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.DAGA*/ wire DAGA_STORE8_MATCH1n = xor2(gb_state.store_x8.ENAD_STORE8_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.DAWU*/ wire DAWU_STORE8_MATCH2n = xor2(gb_state.store_x8.EBOW_STORE8_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.EJAW*/ wire EJAW_STORE8_MATCH3n = xor2(gb_state.store_x8.FYCA_STORE8_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.GOHO*/ wire GOHO_STORE8_MATCH4n = xor2(gb_state.store_x8.GAVY_STORE8_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.GASU*/ wire GASU_STORE8_MATCH5n = xor2(gb_state.store_x8.GYPU_STORE8_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.GABU*/ wire GABU_STORE8_MATCH6n = xor2(gb_state.store_x8.GADY_STORE8_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.GAFE*/ wire GAFE_STORE8_MATCH7n = xor2(gb_state.store_x8.GAZA_STORE8_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.YMAM*/ wire YMAM_STORE9_MATCH0n = xor2(gb_state.store_x9.XUVY_STORE9_X0p.qn_new(), ACAM_PX0n_odd);
  /*_p31.YTYP*/ wire YTYP_STORE9_MATCH1n = xor2(gb_state.store_x9.XERE_STORE9_X1p.qn_new(), AZUB_PX1n_odd);
  /*_p31.YFOP*/ wire YFOP_STORE9_MATCH2n = xor2(gb_state.store_x9.XUZO_STORE9_X2p.qn_new(), AMEL_PX2n_odd);
  /*_p31.YVAC*/ wire YVAC_STORE9_MATCH3n = xor2(gb_state.store_x9.XEXA_STORE9_X3p.qn_new(), AHAL_PX3n_odd);
  /*_p31.ZYWU*/ wire ZYWU_STORE9_MATCH4n = xor2(gb_state.store_x9.YPOD_STORE9_X4p.qn_new(), APUX_PX4n_odd);
  /*_p31.ZUZA*/ wire ZUZA_STORE9_MATCH5n = xor2(gb_state.store_x9.YROP_STORE9_X5p.qn_new(), ABEF_PX5n_odd);
  /*_p31.ZEJO*/ wire ZEJO_STORE9_MATCH6n = xor2(gb_state.store_x9.YNEP_STORE9_X6p.qn_new(), ADAZ_PX6n_odd);
  /*_p31.ZEDA*/ wire ZEDA_STORE9_MATCH7n = xor2(gb_state.store_x9.YZOF_STORE9_X7p.qn_new(), ASAH_PX7n_odd);

  /*_p31.ZAKO*/ wire ZAKO_STORE0_MATCHAp = nor4(ZOGY_STORE0_MATCH0n, ZEBA_STORE0_MATCH1n, ZAHA_STORE0_MATCH2n, ZOKY_STORE0_MATCH3n);
  /*_p31.XEBA*/ wire XEBA_STORE0_MATCHBp = nor4(WOJU_STORE0_MATCH4n, YFUN_STORE0_MATCH5n, WYZA_STORE0_MATCH6n, YPUK_STORE0_MATCH7n);
  /*_p31.CYVY*/ wire CYVY_STORE1_MATCHBp = nor4(COLA_STORE1_MATCH4n, BOBA_STORE1_MATCH5n, COLU_STORE1_MATCH6n, BAHU_STORE1_MATCH7n);
  /*_p31.EWAM*/ wire EWAM_STORE1_MATCHAp = nor4(EDYM_STORE1_MATCH0n, EMYB_STORE1_MATCH1n, EBEF_STORE1_MATCH2n, EWOK_STORE1_MATCH3n);
  /*_p31.CEHU*/ wire CEHU_STORE2_MATCHAp = nor4(CEKO_STORE2_MATCH4n, DETY_STORE2_MATCH5n, DOZO_STORE2_MATCH6n, CONY_STORE2_MATCH7n);
  /*_p31.EKES*/ wire EKES_STORE2_MATCHBp = nor4(FUZU_STORE2_MATCH0n, FESO_STORE2_MATCH1n, FOKY_STORE2_MATCH2n, FYVA_STORE2_MATCH3n);
  /*_p31.ZURE*/ wire ZURE_STORE3_MATCHAp = nor4(YHOK_STORE3_MATCH0n, YCAH_STORE3_MATCH1n, YDAJ_STORE3_MATCH2n, YVUZ_STORE3_MATCH3n);
  /*_p31.YWOS*/ wire YWOS_STORE3_MATCHBp = nor4(YVAP_STORE3_MATCH4n, XENY_STORE3_MATCH5n, XAVU_STORE3_MATCH6n, XEVA_STORE3_MATCH7n);
  /*_p31.YKOK*/ wire YKOK_STORE4_MATCHAp = nor4(XEJU_STORE4_MATCH0n, ZATE_STORE4_MATCH1n, ZAKU_STORE4_MATCH2n, YBOX_STORE4_MATCH3n);
  /*_p31.YNAZ*/ wire YNAZ_STORE4_MATCHBp = nor4(ZYKU_STORE4_MATCH4n, ZYPU_STORE4_MATCH5n, XAHA_STORE4_MATCH6n, ZEFE_STORE4_MATCH7n);
  /*_p31.COGY*/ wire COGY_STORE5_MATCHAp = nor4(BAZY_STORE5_MATCH4n, CYLE_STORE5_MATCH5n, CEVA_STORE5_MATCH6n, BUMY_STORE5_MATCH7n);
  /*_p31.FYMA*/ wire FYMA_STORE5_MATCHBp = nor4(GUZO_STORE5_MATCH0n, GOLA_STORE5_MATCH1n, GEVE_STORE5_MATCH2n, GUDE_STORE5_MATCH3n);
  /*_p31.YWAP*/ wire YWAP_STORE6_MATCHAp = nor4(ZARE_STORE6_MATCH4n, ZEMU_STORE6_MATCH5n, ZYGO_STORE6_MATCH6n, ZUZY_STORE6_MATCH7n);
  /*_p31.YDOT*/ wire YDOT_STORE6_MATCHBp = nor4(XOSU_STORE6_MATCH0n, ZUVU_STORE6_MATCH1n, XUCO_STORE6_MATCH2n, ZULO_STORE6_MATCH3n);
  /*_p31.CYCO*/ wire CYCO_STORE7_MATCHAp = nor4(DUSE_STORE7_MATCH0n, DAGU_STORE7_MATCH1n, DYZE_STORE7_MATCH2n, DESO_STORE7_MATCH3n);
  /*_p31.DAJE*/ wire DAJE_STORE7_MATCHBp = nor4(EJOT_STORE7_MATCH4n, ESAJ_STORE7_MATCH5n, DUCU_STORE7_MATCH6n, EWUD_STORE7_MATCH7n);
  /*_p31.DAMA*/ wire DAMA_STORE8_MATCHAp = nor4(DUZE_STORE8_MATCH0n, DAGA_STORE8_MATCH1n, DAWU_STORE8_MATCH2n, EJAW_STORE8_MATCH3n);
  /*_p31.FEHA*/ wire FEHA_STORE8_MATCHBp = nor4(GOHO_STORE8_MATCH4n, GASU_STORE8_MATCH5n, GABU_STORE8_MATCH6n, GAFE_STORE8_MATCH7n);
  /*_p31.YLEV*/ wire YLEV_STORE9_MATCHAp = nor4(YMAM_STORE9_MATCH0n, YTYP_STORE9_MATCH1n, YFOP_STORE9_MATCH2n, YVAC_STORE9_MATCH3n);
  /*_p31.YTUB*/ wire YTUB_STORE9_MATCHBp = nor4(ZYWU_STORE9_MATCH4n, ZUZA_STORE9_MATCH5n, ZEJO_STORE9_MATCH6n, ZEDA_STORE9_MATCH7n);

  /*_p29.YDUG*/ wire YDUG_STORE0_MATCHn = nand3(AROR_MATCH_ENp, ZAKO_STORE0_MATCHAp, XEBA_STORE0_MATCHBp);
  /*_p29.DYDU*/ wire DYDU_STORE1_MATCHn = nand3(AROR_MATCH_ENp, EWAM_STORE1_MATCHAp, CYVY_STORE1_MATCHBp);
  /*_p29.DEGO*/ wire DEGO_STORE2_MATCHn = nand3(AROR_MATCH_ENp, CEHU_STORE2_MATCHAp, EKES_STORE2_MATCHBp);
  /*_p29.YLOZ*/ wire YLOZ_STORE3_MATCHn = nand3(AROR_MATCH_ENp, ZURE_STORE3_MATCHAp, YWOS_STORE3_MATCHBp);
  /*_p29.XAGE*/ wire XAGE_STORE4_MATCHn = nand3(AROR_MATCH_ENp, YKOK_STORE4_MATCHAp, YNAZ_STORE4_MATCHBp);
  /*_p29.EGOM*/ wire EGOM_STORE5_MATCHn = nand3(AROR_MATCH_ENp, COGY_STORE5_MATCHAp, FYMA_STORE5_MATCHBp);
  /*_p29.YBEZ*/ wire YBEZ_STORE6_MATCHn = nand3(AROR_MATCH_ENp, YWAP_STORE6_MATCHAp, YDOT_STORE6_MATCHBp);
  /*_p29.DYKA*/ wire DYKA_STORE7_MATCHn = nand3(AROR_MATCH_ENp, CYCO_STORE7_MATCHAp, DAJE_STORE7_MATCHBp);
  /*_p29.EFYL*/ wire EFYL_STORE8_MATCHn = nand3(AROR_MATCH_ENp, DAMA_STORE8_MATCHAp, FEHA_STORE8_MATCHBp);
  /*_p29.YGEM*/ wire YGEM_STORE9_MATCHn = nand3(AROR_MATCH_ENp, YLEV_STORE9_MATCHAp, YTUB_STORE9_MATCHBp);

  /*_p29.FEFY*/ wire FEFY_STORE_MATCHp = nand5(XAGE_STORE4_MATCHn, YLOZ_STORE3_MATCHn, DEGO_STORE2_MATCHn, DYDU_STORE1_MATCHn, YDUG_STORE0_MATCHn);
  /*_p29.FOVE*/ wire FOVE_STORE_MATCHp = nand5(YGEM_STORE9_MATCHn, EFYL_STORE8_MATCHn, DYKA_STORE7_MATCHn, YBEZ_STORE6_MATCHn, EGOM_STORE5_MATCHn);
  /*_p29.FEPO*/ gb_state.FEPO_STORE_MATCHp_odd = or2(FEFY_STORE_MATCHp, FOVE_STORE_MATCHp);

  // Update one-hot priority encoder to flag the first matching sprite.

  /*_p29.WEFU*/ wire WEFU_STORE0_MATCH = not1(YDUG_STORE0_MATCHn);
  /*_p29.GAJA*/ wire GAJA_STORE1_MATCH = not1(DYDU_STORE1_MATCHn);
  /*_p29.GUPO*/ wire GUPO_STORE2_MATCH = not1(DEGO_STORE2_MATCHn);
  /*_p29.WEBO*/ wire WEBO_STORE3_MATCH = not1(YLOZ_STORE3_MATCHn);
  /*_p29.WUNA*/ wire WUNA_STORE4_MATCH = not1(XAGE_STORE4_MATCHn);
  /*_p29.GABA*/ wire GABA_STORE5_MATCH = not1(EGOM_STORE5_MATCHn);
  /*_p29.WASE*/ wire WASE_STORE6_MATCH = not1(YBEZ_STORE6_MATCHn);
  /*_p29.GYTE*/ wire GYTE_STORE7_MATCH = not1(DYKA_STORE7_MATCHn);
  /*_p29.GEKE*/ wire GEKE_STORE8_MATCH = not1(EFYL_STORE8_MATCHn);

  /*_p29.GEZE*/ wire GEZE_STORE0_MATCH = or2(WEFU_STORE0_MATCH, gb_state.SIG_GND.out_new());
  /*_p29.FUMA*/ wire FUMA_STORE1_MATCH = or2(GAJA_STORE1_MATCH, GEZE_STORE0_MATCH);
  /*_p29.GEDE*/ wire GEDE_STORE2_MATCH = or2(GUPO_STORE2_MATCH, FUMA_STORE1_MATCH);
  /*_p29.WUTO*/ wire WUTO_STORE3_MATCH = or2(WEBO_STORE3_MATCH, GEDE_STORE2_MATCH);
  /*_p29.XYLA*/ wire XYLA_STORE4_MATCH = or2(WUNA_STORE4_MATCH, WUTO_STORE3_MATCH);
  /*_p29.WEJA*/ wire WEJA_STORE5_MATCH = or2(GABA_STORE5_MATCH, XYLA_STORE4_MATCH);
  /*_p29.WYLA*/ wire WYLA_STORE6_MATCH = or2(WASE_STORE6_MATCH, WEJA_STORE5_MATCH);
  /*_p29.FAVO*/ wire FAVO_STORE7_MATCH = or2(GYTE_STORE7_MATCH, WYLA_STORE6_MATCH);
  /*_p29.GYGA*/ wire GYGA_STORE8_MATCH = or2(GEKE_STORE8_MATCH, FAVO_STORE7_MATCH);

  /*_p29.GUVA*/ sprite_match_flags.GUVA_SPRITE0_GETp = nor2(YDUG_STORE0_MATCHn, gb_state.SIG_GND.out_new());
  /*_p29.ENUT*/ sprite_match_flags.ENUT_SPRITE1_GETp = nor2(DYDU_STORE1_MATCHn, GEZE_STORE0_MATCH);
  /*_p29.EMOL*/ sprite_match_flags.EMOL_SPRITE2_GETp = nor2(DEGO_STORE2_MATCHn, FUMA_STORE1_MATCH);
  /*_p29.GYFY*/ sprite_match_flags.GYFY_SPRITE3_GETp = nor2(YLOZ_STORE3_MATCHn, GEDE_STORE2_MATCH);
  /*_p29.GONO*/ sprite_match_flags.GONO_SPRITE4_GETp = nor2(XAGE_STORE4_MATCHn, WUTO_STORE3_MATCH);
  /*_p29.GEGA*/ sprite_match_flags.GEGA_SPRITE5_GETp = nor2(EGOM_STORE5_MATCHn, XYLA_STORE4_MATCH);
  /*_p29.XOJA*/ sprite_match_flags.XOJA_SPRITE6_GETp = nor2(YBEZ_STORE6_MATCHn, WEJA_STORE5_MATCH);
  /*_p29.GUTU*/ sprite_match_flags.GUTU_SPRITE7_GETp = nor2(DYKA_STORE7_MATCHn, WYLA_STORE6_MATCH);
  /*_p29.FOXA*/ sprite_match_flags.FOXA_SPRITE8_GETp = nor2(EFYL_STORE8_MATCHn, FAVO_STORE7_MATCH);
  /*_p29.GUZE*/ sprite_match_flags.GUZE_SPRITE9_GETp = nor2(YGEM_STORE9_MATCHn, GYGA_STORE8_MATCH);
}

//-----------------------------------------------------------------------------






















































































//-----------------------------------------------------------------------------

void GateBoy::sprite_match_to_bus_gates(SpriteMatchFlags& sprite_get_flag)
{
  // Push sprite index and line for the matching sprite onto the i/l bus.

  /*_p29.FURO*/ wire FURO_SPRITE0_GETn = not1(sprite_get_flag.GUVA_SPRITE0_GETp.out_new());
  /*_p29.DYDO*/ wire DYDO_SPRITE1_GETn = not1(sprite_get_flag.ENUT_SPRITE1_GETp.out_new());
  /*_p29.FAME*/ wire FAME_SPRITE2_GETn = not1(sprite_get_flag.EMOL_SPRITE2_GETp.out_new());
  /*_p29.GYMA*/ wire GYMA_SPRITE3_GETn = not1(sprite_get_flag.GYFY_SPRITE3_GETp.out_new());
  /*_p29.GOWO*/ wire GOWO_SPRITE4_GETn = not1(sprite_get_flag.GONO_SPRITE4_GETp.out_new());
  /*_p29.GYGY*/ wire GYGY_SPRITE5_GETn = not1(sprite_get_flag.GEGA_SPRITE5_GETp.out_new());
  /*_p29.XYME*/ wire XYME_SPRITE6_GETn = not1(sprite_get_flag.XOJA_SPRITE6_GETp.out_new());
  /*_p29.GUGY*/ wire GUGY_SPRITE7_GETn = not1(sprite_get_flag.GUTU_SPRITE7_GETp.out_new());
  /*_p29.DENY*/ wire DENY_SPRITE8_GETn = not1(sprite_get_flag.FOXA_SPRITE8_GETp.out_new());
  /*_p29.FADO*/ wire FADO_SPRITE9_GETn = not1(sprite_get_flag.GUZE_SPRITE9_GETp.out_new());

  /*_p30.ZETU*/ triwire ZETU_STORE0_GET_I0 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_i0.YGUS_STORE0_I0n.qp_new());
  /*_p30.ZECE*/ triwire ZECE_STORE0_GET_I1 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_i0.YSOK_STORE0_I1n.qp_new());
  /*_p30.ZAVE*/ triwire ZAVE_STORE0_GET_I2 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_i0.YZEP_STORE0_I2n.qp_new());
  /*_p30.WOKO*/ triwire WOKO_STORE0_GET_I3 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_i0.WYTE_STORE0_I3n.qp_new());
  /*_p30.ZUMU*/ triwire ZUMU_STORE0_GET_L0 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_i0.ZONY_STORE0_I4n.qp_new());
  /*#p30.ZEDY*/ triwire ZEDY_STORE0_GET_L1 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_i0.YWAK_STORE0_I5n.qp_new());
  /*_p30.WEHE*/ triwire WEHE_STORE0_GET_L2 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_l0.GYHO_STORE0_L0n.qp_new());
  /*_p30.BUKY*/ triwire BUKY_STORE0_GET_L3 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_l0.CUFO_STORE0_L1n.qp_new());
  /*_p30.AJAL*/ triwire AJAL_STORE0_GET_L4 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_l0.BOZU_STORE0_L2n.qp_new());
  /*_p30.GOFO*/ triwire GOFO_STORE0_GET_L5 = tri6_nn(FURO_SPRITE0_GETn, gb_state.store_l0.FYHY_STORE0_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(ZETU_STORE0_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(ZECE_STORE0_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(ZAVE_STORE0_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(WOKO_STORE0_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(ZUMU_STORE0_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(ZEDY_STORE0_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(WEHE_STORE0_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(BUKY_STORE0_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(AJAL_STORE0_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(GOFO_STORE0_GET_L5);

  /*#p30.CUBO*/ triwire CUBO_STORE1_GET_I0 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_i1.CADU_STORE1_I0n.qp_new());
  /*_p30.CELU*/ triwire CELU_STORE1_GET_I1 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_i1.CEBO_STORE1_I1n.qp_new());
  /*_p30.CEGY*/ triwire CEGY_STORE1_GET_I2 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_i1.CUFA_STORE1_I2n.qp_new());
  /*_p30.BETY*/ triwire BETY_STORE1_GET_I3 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_i1.COMA_STORE1_I3n.qp_new());
  /*_p30.CYBY*/ triwire CYBY_STORE1_GET_L0 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_i1.CUZA_STORE1_I4n.qp_new());
  /*_p30.BEMO*/ triwire BEMO_STORE1_GET_L1 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_i1.CAJY_STORE1_I5n.qp_new());
  /*_p30.BYRO*/ triwire BYRO_STORE1_GET_L2 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_l1.AMES_STORE1_L0n.qp_new());
  /*_p30.AHUM*/ triwire AHUM_STORE1_GET_L3 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_l1.AROF_STORE1_L1n.qp_new());
  /*_p30.BACO*/ triwire BACO_STORE1_GET_L4 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_l1.ABOP_STORE1_L2n.qp_new());
  /*_p30.BEFE*/ triwire BEFE_STORE1_GET_L5 = tri6_nn(DYDO_SPRITE1_GETn, gb_state.store_l1.ABUG_STORE1_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(CUBO_STORE1_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(CELU_STORE1_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(CEGY_STORE1_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(BETY_STORE1_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(CYBY_STORE1_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(BEMO_STORE1_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(BYRO_STORE1_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(AHUM_STORE1_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(BACO_STORE1_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(BEFE_STORE1_GET_L5);

  /*_p30.CUBE*/ triwire CUBE_STORE2_GET_I0 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_i2.BUHE_STORE2_I0n.qp_new());
  /*_p30.AFOZ*/ triwire AFOZ_STORE2_GET_I1 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_i2.BYHU_STORE2_I1n.qp_new());
  /*_p30.APON*/ triwire APON_STORE2_GET_I2 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_i2.BECA_STORE2_I2n.qp_new());
  /*_p30.CUVU*/ triwire CUVU_STORE2_GET_I3 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_i2.BULU_STORE2_I3n.qp_new());
  /*_p30.CYRO*/ triwire CYRO_STORE2_GET_L0 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_i2.BUNA_STORE2_I4n.qp_new());
  /*_p30.AXEC*/ triwire AXEC_STORE2_GET_L1 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_i2.BOXA_STORE2_I5n.qp_new());
  /*_p30.ZUKE*/ triwire ZUKE_STORE2_GET_L2 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_l2.YLOV_STORE2_L0n.qp_new());
  /*_p30.WERE*/ triwire WERE_STORE2_GET_L3 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_l2.XOSY_STORE2_L1n.qp_new());
  /*_p30.WUXE*/ triwire WUXE_STORE2_GET_L4 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_l2.XAZY_STORE2_L2n.qp_new());
  /*_p30.ZABY*/ triwire ZABY_STORE2_GET_L5 = tri6_nn(FAME_SPRITE2_GETn, gb_state.store_l2.YKUK_STORE2_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(CUBE_STORE2_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(AFOZ_STORE2_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(APON_STORE2_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(CUVU_STORE2_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(CYRO_STORE2_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(AXEC_STORE2_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(ZUKE_STORE2_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(WERE_STORE2_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(WUXE_STORE2_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(ZABY_STORE2_GET_L5);

  /*_p30.ENAP*/ triwire ENAP_STORE3_GET_I0 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_i3.DEVY_STORE3_I0n.qp_new());
  /*_p30.DYGO*/ triwire DYGO_STORE3_GET_I1 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_i3.DESE_STORE3_I1n.qp_new());
  /*_p30.DOWA*/ triwire DOWA_STORE3_GET_I2 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_i3.DUNY_STORE3_I2n.qp_new());
  /*_p30.DONY*/ triwire DONY_STORE3_GET_I3 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_i3.DUHA_STORE3_I3n.qp_new());
  /*_p30.EFUD*/ triwire EFUD_STORE3_GET_L0 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_i3.DEBA_STORE3_I4n.qp_new());
  /*_p30.DEZU*/ triwire DEZU_STORE3_GET_L1 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_i3.DAFU_STORE3_I5n.qp_new());
  /*_p30.ZEXE*/ triwire ZEXE_STORE3_GET_L2 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_l3.ZURO_STORE3_L0n.qp_new());
  /*_p30.YWAV*/ triwire YWAV_STORE3_GET_L3 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_l3.ZYLU_STORE3_L1n.qp_new());
  /*_p30.YJEM*/ triwire YJEM_STORE3_GET_L4 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_l3.ZENE_STORE3_L2n.qp_new());
  /*_p30.ZYPO*/ triwire ZYPO_STORE3_GET_L5 = tri6_nn(GYMA_SPRITE3_GETn, gb_state.store_l3.ZURY_STORE3_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(ENAP_STORE3_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(DYGO_STORE3_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(DOWA_STORE3_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(DONY_STORE3_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(EFUD_STORE3_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(DEZU_STORE3_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(ZEXE_STORE3_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(YWAV_STORE3_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(YJEM_STORE3_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(ZYPO_STORE3_GET_L5);

  /*_p30.WUXU*/ triwire WUXU_STORE4_GET_I0 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_i4.XAVE_STORE4_I0n.qp_new());
  /*_p30.WEPY*/ triwire WEPY_STORE4_GET_I1 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_i4.XEFE_STORE4_I1n.qp_new());
  /*_p30.WERU*/ triwire WERU_STORE4_GET_I2 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_i4.WANU_STORE4_I2n.qp_new());
  /*_p30.XYRE*/ triwire XYRE_STORE4_GET_I3 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_i4.XABO_STORE4_I3n.qp_new());
  /*_p30.WOXY*/ triwire WOXY_STORE4_GET_L0 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_i4.XEGE_STORE4_I4n.qp_new());
  /*_p30.WAJA*/ triwire WAJA_STORE4_GET_L1 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_i4.XYNU_STORE4_I5n.qp_new());
  /*_p30.BUCE*/ triwire BUCE_STORE4_GET_L2 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_l4.CAPO_STORE4_L0n.qp_new());
  /*_p30.BEVY*/ triwire BEVY_STORE4_GET_L3 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_l4.CAJU_STORE4_L1n.qp_new());
  /*_p30.BOVE*/ triwire BOVE_STORE4_GET_L4 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_l4.CONO_STORE4_L2n.qp_new());
  /*_p30.BYDO*/ triwire BYDO_STORE4_GET_L5 = tri6_nn(GOWO_SPRITE4_GETn, gb_state.store_l4.CUMU_STORE4_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(WUXU_STORE4_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(WEPY_STORE4_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(WERU_STORE4_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(XYRE_STORE4_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(WOXY_STORE4_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(WAJA_STORE4_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(BUCE_STORE4_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(BEVY_STORE4_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(BOVE_STORE4_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(BYDO_STORE4_GET_L5);

  /*_p30.DOBO*/ triwire DOBO_STORE5_GET_I0 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_i5.EKOP_STORE5_I0n.qp_new());
  /*_p30.DYNY*/ triwire DYNY_STORE5_GET_I1 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_i5.ETYM_STORE5_I1n.qp_new());
  /*_p30.WAGA*/ triwire WAGA_STORE5_GET_I2 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_i5.GORU_STORE5_I2n.qp_new());
  /*_p30.DUZA*/ triwire DUZA_STORE5_GET_I3 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_i5.EBEX_STORE5_I3n.qp_new());
  /*_p30.DALY*/ triwire DALY_STORE5_GET_L0 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_i5.ETAV_STORE5_I4n.qp_new());
  /*_p30.DALO*/ triwire DALO_STORE5_GET_L1 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_i5.EKAP_STORE5_I5n.qp_new());
  /*_p30.BACE*/ triwire BACE_STORE5_GET_L2 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_l5.ACEP_STORE5_L0n.qp_new());
  /*_p30.BUJA*/ triwire BUJA_STORE5_GET_L3 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_l5.ABEG_STORE5_L1n.qp_new());
  /*_p30.BODU*/ triwire BODU_STORE5_GET_L4 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_l5.ABUX_STORE5_L2n.qp_new());
  /*_p30.AWAT*/ triwire AWAT_STORE5_GET_L5 = tri6_nn(GYGY_SPRITE5_GETn, gb_state.store_l5.ANED_STORE5_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(DOBO_STORE5_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(DYNY_STORE5_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(WAGA_STORE5_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(DUZA_STORE5_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(DALY_STORE5_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(DALO_STORE5_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(BACE_STORE5_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(BUJA_STORE5_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(BODU_STORE5_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(AWAT_STORE5_GET_L5);

  /*_p30.WATO*/ triwire WATO_STORE6_GET_I0 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_i6.GABO_STORE6_I0n.qp_new());
  /*_p30.WYWY*/ triwire WYWY_STORE6_GET_I1 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_i6.GACY_STORE6_I1n.qp_new());
  /*_p30.EZOC*/ triwire EZOC_STORE6_GET_I2 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_i6.FOGO_STORE6_I2n.qp_new());
  /*_p30.WABO*/ triwire WABO_STORE6_GET_I3 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_i6.GOHU_STORE6_I3n.qp_new());
  /*_p30.ELYC*/ triwire ELYC_STORE6_GET_L0 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_i6.FOXY_STORE6_I4n.qp_new());
  /*_p30.WOCY*/ triwire WOCY_STORE6_GET_L1 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_i6.GECU_STORE6_I5n.qp_new());
  /*_p30.YBUK*/ triwire YBUK_STORE6_GET_L2 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_l6.ZUMY_STORE6_L0n.qp_new());
  /*_p30.YKOZ*/ triwire YKOZ_STORE6_GET_L3 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_l6.ZAFU_STORE6_L1n.qp_new());
  /*_p30.ZYTO*/ triwire ZYTO_STORE6_GET_L4 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_l6.ZEXO_STORE6_L2n.qp_new());
  /*_p30.ZUDO*/ triwire ZUDO_STORE6_GET_L5 = tri6_nn(XYME_SPRITE6_GETn, gb_state.store_l6.ZUBE_STORE6_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(WATO_STORE6_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(WYWY_STORE6_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(EZOC_STORE6_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(WABO_STORE6_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(ELYC_STORE6_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(WOCY_STORE6_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(YBUK_STORE6_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(YKOZ_STORE6_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(ZYTO_STORE6_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(ZUDO_STORE6_GET_L5);

  /*_p30.WAKO*/ triwire WAKO_STORE7_GET_I0 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_i7.GULE_STORE7_I0n.qp_new());
  /*_p30.WYGO*/ triwire WYGO_STORE7_GET_I1 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_i7.GYNO_STORE7_I1n.qp_new());
  /*_p30.ELEP*/ triwire ELEP_STORE7_GET_I2 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_i7.FEFA_STORE7_I2n.qp_new());
  /*_p30.ETAD*/ triwire ETAD_STORE7_GET_I3 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_i7.FYSU_STORE7_I3n.qp_new());
  /*_p30.WABA*/ triwire WABA_STORE7_GET_L0 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_i7.GESY_STORE7_I4n.qp_new());
  /*_p30.EVYT*/ triwire EVYT_STORE7_GET_L1 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_i7.FUZO_STORE7_I5n.qp_new());
  /*_p30.WAXE*/ triwire WAXE_STORE7_GET_L2 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_l7.XYNA_STORE7_L0n.qp_new());
  /*_p30.YPOZ*/ triwire YPOZ_STORE7_GET_L3 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_l7.YGUM_STORE7_L1n.qp_new());
  /*_p30.WABU*/ triwire WABU_STORE7_GET_L4 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_l7.XAKU_STORE7_L2n.qp_new());
  /*_p30.WANA*/ triwire WANA_STORE7_GET_L5 = tri6_nn(GUGY_SPRITE7_GETn, gb_state.store_l7.XYGO_STORE7_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(WAKO_STORE7_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(WYGO_STORE7_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(ELEP_STORE7_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(ETAD_STORE7_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(WABA_STORE7_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(EVYT_STORE7_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(WAXE_STORE7_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(YPOZ_STORE7_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(WABU_STORE7_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(WANA_STORE7_GET_L5);

  /*_p30.APOC*/ triwire APOC_STORE8_GET_I0 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_i8.AXUV_STORE8_I0n.qp_new());
  /*_p30.AKYH*/ triwire AKYH_STORE8_GET_I1 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_i8.BADA_STORE8_I1n.qp_new());
  /*_p30.AFEN*/ triwire AFEN_STORE8_GET_I2 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_i8.APEV_STORE8_I2n.qp_new());
  /*_p30.APYV*/ triwire APYV_STORE8_GET_I3 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_i8.BADO_STORE8_I3n.qp_new());
  /*_p30.APOB*/ triwire APOB_STORE8_GET_L0 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_i8.BEXY_STORE8_I4n.qp_new());
  /*_p30.ADYB*/ triwire ADYB_STORE8_GET_L1 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_i8.BYHE_STORE8_I5n.qp_new());
  /*_p30.BOSO*/ triwire BOSO_STORE8_GET_L2 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_l8.AZAP_STORE8_L0n.qp_new());
  /*_p30.BAZU*/ triwire BAZU_STORE8_GET_L3 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_l8.AFYX_STORE8_L1n.qp_new());
  /*_p30.AHAC*/ triwire AHAC_STORE8_GET_L4 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_l8.AFUT_STORE8_L2n.qp_new());
  /*_p30.BUJY*/ triwire BUJY_STORE8_GET_L5 = tri6_nn(DENY_SPRITE8_GETn, gb_state.store_l8.AFYM_STORE8_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(APOC_STORE8_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(AKYH_STORE8_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(AFEN_STORE8_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(APYV_STORE8_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(APOB_STORE8_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(ADYB_STORE8_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(BOSO_STORE8_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(BAZU_STORE8_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(AHAC_STORE8_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(BUJY_STORE8_GET_L5);

  /*_p30.ZARO*/ triwire ZARO_STORE9_GET_I0 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_i9.YBER_STORE9_I0n.qp_new());
  /*_p30.ZOJY*/ triwire ZOJY_STORE9_GET_I1 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_i9.YZOR_STORE9_I1n.qp_new());
  /*_p30.YNEV*/ triwire YNEV_STORE9_GET_I2 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_i9.XYFE_STORE9_I2n.qp_new());
  /*_p30.XYRA*/ triwire XYRA_STORE9_GET_I3 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_i9.XOTU_STORE9_I3n.qp_new());
  /*_p30.YRAD*/ triwire YRAD_STORE9_GET_L0 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_i9.XUTE_STORE9_I4n.qp_new());
  /*_p30.YHAL*/ triwire YHAL_STORE9_GET_L1 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_i9.XUFO_STORE9_I5n.qp_new());
  /*_p30.BYME*/ triwire BYME_STORE9_GET_L2 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_l9.CANA_STORE9_L0n.qp_new());
  /*_p30.GATE*/ triwire GATE_STORE9_GET_L3 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_l9.FOFO_STORE9_L1n.qp_new());
  /*_p30.COHO*/ triwire COHO_STORE9_GET_L4 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_l9.DYSY_STORE9_L2n.qp_new());
  /*_p30.CAWO*/ triwire CAWO_STORE9_GET_L5 = tri6_nn(FADO_SPRITE9_GETn, gb_state.store_l9.DEWU_STORE9_L3n.qp_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(ZARO_STORE9_GET_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(ZOJY_STORE9_GET_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(YNEV_STORE9_GET_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(XYRA_STORE9_GET_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(YRAD_STORE9_GET_L0);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(YHAL_STORE9_GET_L1);
  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(BYME_STORE9_GET_L2);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(GATE_STORE9_GET_L3);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(COHO_STORE9_GET_L4);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(CAWO_STORE9_GET_L5);
}

//-----------------------------------------------------------------------------





















































//-----------------------------------------------------------------------------

void GateBoy::sprite_scan_to_bus_gates(SpriteDeltaY sprite_delta_y, NorLatch XYMU_RENDERINGn, Gate FEPO_STORE_MATCHp)
{
  /*#p29.BUZA*/ wire BUZA_STORE_SPRITE_INDXn_new = and2(gb_state.sprite_scanner.CENO_SCAN_DONEn_odd.qn_new(), XYMU_RENDERINGn.qn_new());
  /*#p30.WUZY*/ triwire WUZY_STORE_I0 = tri6_nn(BUZA_STORE_SPRITE_INDXn_new, gb_state.sprite_index.XADU_SPRITE_IDX0p_odd.qn_new());
  /*_p30.WYSE*/ triwire WYSE_STORE_I1 = tri6_nn(BUZA_STORE_SPRITE_INDXn_new, gb_state.sprite_index.XEDY_SPRITE_IDX1p_odd.qn_new());
  /*_p30.ZYSU*/ triwire ZYSU_STORE_I2 = tri6_nn(BUZA_STORE_SPRITE_INDXn_new, gb_state.sprite_index.ZUZE_SPRITE_IDX2p_odd.qn_new());
  /*_p30.WYDA*/ triwire WYDA_STORE_I3 = tri6_nn(BUZA_STORE_SPRITE_INDXn_new, gb_state.sprite_index.XOBE_SPRITE_IDX3p_odd.qn_new());
  /*_p30.WUCO*/ triwire WUCO_STORE_I4 = tri6_nn(BUZA_STORE_SPRITE_INDXn_new, gb_state.sprite_index.YDUF_SPRITE_IDX4p_odd.qn_new());
  /*_p30.WEZA*/ triwire WEZA_STORE_I5 = tri6_nn(BUZA_STORE_SPRITE_INDXn_new, gb_state.sprite_index.XECU_SPRITE_IDX5p_odd.qn_new());

  /*_BUS_SPR_I0*/ gb_state.sprite_ibus.BUS_SPR_I0.tri_bus(WUZY_STORE_I0);
  /*_BUS_SPR_I1*/ gb_state.sprite_ibus.BUS_SPR_I1.tri_bus(WYSE_STORE_I1);
  /*_BUS_SPR_I2*/ gb_state.sprite_ibus.BUS_SPR_I2.tri_bus(ZYSU_STORE_I2);
  /*_BUS_SPR_I3*/ gb_state.sprite_ibus.BUS_SPR_I3.tri_bus(WYDA_STORE_I3);
  /*_BUS_SPR_I4*/ gb_state.sprite_ibus.BUS_SPR_I4.tri_bus(WUCO_STORE_I4);
  /*_BUS_SPR_I5*/ gb_state.sprite_ibus.BUS_SPR_I5.tri_bus(WEZA_STORE_I5);

  /*_p29.DEGE*/ wire DEGE_SPRITE_DELTA0 = not1(sprite_delta_y.ERUC_YDIFF0.sum);
  /*_p29.DABY*/ wire DABY_SPRITE_DELTA1 = not1(sprite_delta_y.ENEF_YDIFF1.sum);
  /*_p29.DABU*/ wire DABU_SPRITE_DELTA2 = not1(sprite_delta_y.FECO_YDIFF2.sum);
  /*_p29.GYSA*/ wire GYSA_SPRITE_DELTA3 = not1(sprite_delta_y.GYKY_YDIFF3.sum);

  /*#p30.CUCU*/ triwire CUCU_STORE_L0 = tri6_nn(FEPO_STORE_MATCHp.out_new(), DEGE_SPRITE_DELTA0);
  /*#p30.CUCA*/ triwire CUCA_STORE_L1 = tri6_nn(FEPO_STORE_MATCHp.out_new(), DABY_SPRITE_DELTA1);
  /*#p30.CEGA*/ triwire CEGA_STORE_L2 = tri6_nn(FEPO_STORE_MATCHp.out_new(), DABU_SPRITE_DELTA2);
  /*#p30.WENU*/ triwire WENU_STORE_L3 = tri6_nn(FEPO_STORE_MATCHp.out_new(), GYSA_SPRITE_DELTA3);

  /*_BUS_SPR_L0*/ gb_state.sprite_lbus.BUS_SPR_L0.tri_bus(CUCU_STORE_L0);
  /*_BUS_SPR_L1*/ gb_state.sprite_lbus.BUS_SPR_L1.tri_bus(CUCA_STORE_L1);
  /*_BUS_SPR_L2*/ gb_state.sprite_lbus.BUS_SPR_L2.tri_bus(CEGA_STORE_L2);
  /*_BUS_SPR_L3*/ gb_state.sprite_lbus.BUS_SPR_L3.tri_bus(WENU_STORE_L3);
}

//-----------------------------------------------------------------------------

