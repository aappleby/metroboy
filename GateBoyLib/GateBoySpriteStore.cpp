#include "GateBoyLib/GateBoySpriteStore.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoySpriteStore::reset_to_cart() {
  DEZY_COUNT_CLKp.state = 0b00011011;
  BESE_SPRITE_COUNT0.state = 0b00011010;
  CUXY_SPRITE_COUNT1.state = 0b00011010;
  BEGO_SPRITE_COUNT2.state = 0b00011010;
  DYBE_SPRITE_COUNT3.state = 0b00011010;
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::update_count(wire _DYTY_STORE_CLKp) {
  (void)_DYTY_STORE_CLKp;
}

//------------------------------------------------------------------------------------------------------------------------

SpriteStoreFlag GateBoy::get_store_flags(wire _DYTY_STORE_CLKp) {
  /* p29.EDEN*/ wire _EDEN_SPRITE_COUNT0n_new = not1(sprite_store.BESE_SPRITE_COUNT0.qp_new());
  /* p29.CYPY*/ wire _CYPY_SPRITE_COUNT1n_new = not1(sprite_store.CUXY_SPRITE_COUNT1.qp_new());
  /* p29.CAPE*/ wire _CAPE_SPRITE_COUNT2n_new = not1(sprite_store.BEGO_SPRITE_COUNT2.qp_new());
  /* p29.CAXU*/ wire _CAXU_SPRITE_COUNT3n_new = not1(sprite_store.DYBE_SPRITE_COUNT3.qp_new());

  /* p29.FYCU*/ wire _FYCU_SPRITE_COUNT0p_new = not1(_EDEN_SPRITE_COUNT0n_new);
  /* p29.FONE*/ wire _FONE_SPRITE_COUNT1p_new = not1(_CYPY_SPRITE_COUNT1n_new);
  /* p29.EKUD*/ wire _EKUD_SPRITE_COUNT2p_new = not1(_CAPE_SPRITE_COUNT2n_new);
  /* p29.ELYG*/ wire _ELYG_SPRITE_COUNT3p_new = not1(_CAXU_SPRITE_COUNT3n_new);

  /* p29.DEZO*/ wire _DEZO_STORE0_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.CUVA*/ wire _CUVA_STORE1_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.GEBU*/ wire _GEBU_STORE2_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _FONE_SPRITE_COUNT1p_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.FOCO*/ wire _FOCO_STORE3_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _FONE_SPRITE_COUNT1p_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.CUPE*/ wire _CUPE_STORE4_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _CYPY_SPRITE_COUNT1n_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.CUGU*/ wire _CUGU_STORE5_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _CYPY_SPRITE_COUNT1n_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.WOMU*/ wire _WOMU_STORE6_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _FONE_SPRITE_COUNT1p_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.GUNA*/ wire _GUNA_STORE7_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _FONE_SPRITE_COUNT1p_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.DEWY*/ wire _DEWY_STORE8_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _ELYG_SPRITE_COUNT3p_new);
  /* p29.DOGU*/ wire _DOGU_STORE9_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _ELYG_SPRITE_COUNT3p_new);

  /* p29.CEMY*/ wire _CEMY_STORE0_CLKp = or2(_DYTY_STORE_CLKp, _DEZO_STORE0_SELn_new);
  /* p29.BYBY*/ wire _BYBY_STORE1_CLKp = or2(_DYTY_STORE_CLKp, _CUVA_STORE1_SELn_new);
  /* p29.WYXO*/ wire _WYXO_STORE2_CLKp = or2(_DYTY_STORE_CLKp, _GEBU_STORE2_SELn_new);
  /* p29.GUVE*/ wire _GUVE_STORE3_CLKp = or2(_DYTY_STORE_CLKp, _FOCO_STORE3_SELn_new);
  /* p29.CECU*/ wire _CECU_STORE4_CLKp = or2(_DYTY_STORE_CLKp, _CUPE_STORE4_SELn_new);
  /* p29.CADO*/ wire _CADO_STORE5_CLKp = or2(_DYTY_STORE_CLKp, _CUGU_STORE5_SELn_new);
  /* p29.XUJO*/ wire _XUJO_STORE6_CLKp = or2(_DYTY_STORE_CLKp, _WOMU_STORE6_SELn_new);
  /* p29.GAPE*/ wire _GAPE_STORE7_CLKp = or2(_DYTY_STORE_CLKp, _GUNA_STORE7_SELn_new);
  /* p29.CAHO*/ wire _CAHO_STORE8_CLKp = or2(_DYTY_STORE_CLKp, _DEWY_STORE8_SELn_new);
  /* p29.CATO*/ wire _CATO_STORE9_CLKp = or2(_DYTY_STORE_CLKp, _DOGU_STORE9_SELn_new);

  /* p29.DYHU*/ wire _DYHU_STORE0_CLKn = not1(_CEMY_STORE0_CLKp);
  /* p29.BUCO*/ wire _BUCO_STORE1_CLKn = not1(_BYBY_STORE1_CLKp);
  /* p29.GYFO*/ wire _GYFO_STORE2_CLKn = not1(_WYXO_STORE2_CLKp);
  /* p29.GUSA*/ wire _GUSA_STORE3_CLKn = not1(_GUVE_STORE3_CLKp);
  /* p29.DUKE*/ wire _DUKE_STORE4_CLKn = not1(_CECU_STORE4_CLKp);
  /* p29.BEDE*/ wire _BEDE_STORE5_CLKn = not1(_CADO_STORE5_CLKp);
  /* p29.WEKA*/ wire _WEKA_STORE6_CLKn = not1(_XUJO_STORE6_CLKp);
  /* p29.GYVO*/ wire _GYVO_STORE7_CLKn = not1(_GAPE_STORE7_CLKp);
  /* p29.BUKA*/ wire _BUKA_STORE8_CLKn = not1(_CAHO_STORE8_CLKp);
  /* p29.DECU*/ wire _DECU_STORE9_CLKn = not1(_CATO_STORE9_CLKp);

  return {
    _DYHU_STORE0_CLKn,
    _BUCO_STORE1_CLKn,
    _GYFO_STORE2_CLKn,
    _GUSA_STORE3_CLKn,
    _DUKE_STORE4_CLKn,
    _BEDE_STORE5_CLKn,
    _WEKA_STORE6_CLKn,
    _GYVO_STORE7_CLKn,
    _BUKA_STORE8_CLKn,
    _DECU_STORE9_CLKn
  };
}

//------------------------------------------------------------------------------------------------------------------------

SpriteMatchFlag GateBoy::get_match_flags(wire _AROR_MATCH_ENp_new) {
  /*#p21.ACAM*/ wire ACAM_PX0n = not1(pix_count.XEHO_PX0p.qp_new());
  /* p21.AZUB*/ wire AZUB_PX1n = not1(pix_count.SAVY_PX1p.qp_new());
  /* p21.AMEL*/ wire AMEL_PX2n = not1(pix_count.XODU_PX2p.qp_new());
  /* p21.AHAL*/ wire AHAL_PX3n = not1(pix_count.XYDO_PX3p.qp_new());
  /* p21.APUX*/ wire APUX_PX4n = not1(pix_count.TUHU_PX4p.qp_new());
  /* p21.ABEF*/ wire ABEF_PX5n = not1(pix_count.TUKY_PX5p.qp_new());
  /* p21.ADAZ*/ wire ADAZ_PX6n = not1(pix_count.TAKO_PX6p.qp_new());
  /* p21.ASAH*/ wire ASAH_PX7n = not1(pix_count.SYBE_PX7p.qp_new());

  /*#p31.ZOGY*/ wire _ZOGY_STORE0_MATCH0n_new = xor2(sprite_store.XEPE_STORE0_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.ZEBA*/ wire _ZEBA_STORE0_MATCH1n_new = xor2(sprite_store.YLAH_STORE0_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.ZAHA*/ wire _ZAHA_STORE0_MATCH2n_new = xor2(sprite_store.ZOLA_STORE0_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.ZOKY*/ wire _ZOKY_STORE0_MATCH3n_new = xor2(sprite_store.ZULU_STORE0_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.WOJU*/ wire _WOJU_STORE0_MATCH4n_new = xor2(sprite_store.WELO_STORE0_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.YFUN*/ wire _YFUN_STORE0_MATCH5n_new = xor2(sprite_store.XUNY_STORE0_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.WYZA*/ wire _WYZA_STORE0_MATCH6n_new = xor2(sprite_store.WOTE_STORE0_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.YPUK*/ wire _YPUK_STORE0_MATCH7n_new = xor2(sprite_store.XAKO_STORE0_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.EDYM*/ wire _EDYM_STORE1_MATCH0n_new = xor2(sprite_store.DANY_STORE1_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.EMYB*/ wire _EMYB_STORE1_MATCH1n_new = xor2(sprite_store.DUKO_STORE1_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.EBEF*/ wire _EBEF_STORE1_MATCH2n_new = xor2(sprite_store.DESU_STORE1_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.EWOK*/ wire _EWOK_STORE1_MATCH3n_new = xor2(sprite_store.DAZO_STORE1_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.COLA*/ wire _COLA_STORE1_MATCH4n_new = xor2(sprite_store.DAKE_STORE1_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.BOBA*/ wire _BOBA_STORE1_MATCH5n_new = xor2(sprite_store.CESO_STORE1_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.COLU*/ wire _COLU_STORE1_MATCH6n_new = xor2(sprite_store.DYFU_STORE1_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.BAHU*/ wire _BAHU_STORE1_MATCH7n_new = xor2(sprite_store.CUSY_STORE1_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.FUZU*/ wire _FUZU_STORE2_MATCH0n_new = xor2(sprite_store.FOKA_STORE2_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.FESO*/ wire _FESO_STORE2_MATCH1n_new = xor2(sprite_store.FYTY_STORE2_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.FOKY*/ wire _FOKY_STORE2_MATCH2n_new = xor2(sprite_store.FUBY_STORE2_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.FYVA*/ wire _FYVA_STORE2_MATCH3n_new = xor2(sprite_store.GOXU_STORE2_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.CEKO*/ wire _CEKO_STORE2_MATCH4n_new = xor2(sprite_store.DUHY_STORE2_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.DETY*/ wire _DETY_STORE2_MATCH5n_new = xor2(sprite_store.EJUF_STORE2_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.DOZO*/ wire _DOZO_STORE2_MATCH6n_new = xor2(sprite_store.ENOR_STORE2_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.CONY*/ wire _CONY_STORE2_MATCH7n_new = xor2(sprite_store.DEPY_STORE2_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.YHOK*/ wire _YHOK_STORE3_MATCH0n_new = xor2(sprite_store.XOLY_STORE3_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.YCAH*/ wire _YCAH_STORE3_MATCH1n_new = xor2(sprite_store.XYBA_STORE3_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.YDAJ*/ wire _YDAJ_STORE3_MATCH2n_new = xor2(sprite_store.XABE_STORE3_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.YVUZ*/ wire _YVUZ_STORE3_MATCH3n_new = xor2(sprite_store.XEKA_STORE3_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.YVAP*/ wire _YVAP_STORE3_MATCH4n_new = xor2(sprite_store.XOMY_STORE3_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.XENY*/ wire _XENY_STORE3_MATCH5n_new = xor2(sprite_store.WUHA_STORE3_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.XAVU*/ wire _XAVU_STORE3_MATCH6n_new = xor2(sprite_store.WYNA_STORE3_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.XEVA*/ wire _XEVA_STORE3_MATCH7n_new = xor2(sprite_store.WECO_STORE3_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.XEJU*/ wire _XEJU_STORE4_MATCH0n_new = xor2(sprite_store.WEDU_STORE4_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.ZATE*/ wire _ZATE_STORE4_MATCH1n_new = xor2(sprite_store.YGAJ_STORE4_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.ZAKU*/ wire _ZAKU_STORE4_MATCH2n_new = xor2(sprite_store.ZYJO_STORE4_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.YBOX*/ wire _YBOX_STORE4_MATCH3n_new = xor2(sprite_store.XURY_STORE4_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.ZYKU*/ wire _ZYKU_STORE4_MATCH4n_new = xor2(sprite_store.YBED_STORE4_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ZYPU*/ wire _ZYPU_STORE4_MATCH5n_new = xor2(sprite_store.ZALA_STORE4_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.XAHA*/ wire _XAHA_STORE4_MATCH6n_new = xor2(sprite_store.WYDE_STORE4_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.ZEFE*/ wire _ZEFE_STORE4_MATCH7n_new = xor2(sprite_store.XEPA_STORE4_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.GUZO*/ wire _GUZO_STORE5_MATCH0n_new = xor2(sprite_store.FUSA_STORE5_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.GOLA*/ wire _GOLA_STORE5_MATCH1n_new = xor2(sprite_store.FAXA_STORE5_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.GEVE*/ wire _GEVE_STORE5_MATCH2n_new = xor2(sprite_store.FOZY_STORE5_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.GUDE*/ wire _GUDE_STORE5_MATCH3n_new = xor2(sprite_store.FESY_STORE5_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.BAZY*/ wire _BAZY_STORE5_MATCH4n_new = xor2(sprite_store.CYWE_STORE5_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.CYLE*/ wire _CYLE_STORE5_MATCH5n_new = xor2(sprite_store.DYBY_STORE5_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.CEVA*/ wire _CEVA_STORE5_MATCH6n_new = xor2(sprite_store.DURY_STORE5_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.BUMY*/ wire _BUMY_STORE5_MATCH7n_new = xor2(sprite_store.CUVY_STORE5_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.XOSU*/ wire _XOSU_STORE6_MATCH0n_new = xor2(sprite_store.YCOL_STORE6_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.ZUVU*/ wire _ZUVU_STORE6_MATCH1n_new = xor2(sprite_store.YRAC_STORE6_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.XUCO*/ wire _XUCO_STORE6_MATCH2n_new = xor2(sprite_store.YMEM_STORE6_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.ZULO*/ wire _ZULO_STORE6_MATCH3n_new = xor2(sprite_store.YVAG_STORE6_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.ZARE*/ wire _ZARE_STORE6_MATCH4n_new = xor2(sprite_store.ZOLY_STORE6_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ZEMU*/ wire _ZEMU_STORE6_MATCH5n_new = xor2(sprite_store.ZOGO_STORE6_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.ZYGO*/ wire _ZYGO_STORE6_MATCH6n_new = xor2(sprite_store.ZECU_STORE6_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.ZUZY*/ wire _ZUZY_STORE6_MATCH7n_new = xor2(sprite_store.ZESA_STORE6_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.DUSE*/ wire _DUSE_STORE7_MATCH0n_new = xor2(sprite_store.ERAZ_STORE7_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.DAGU*/ wire _DAGU_STORE7_MATCH1n_new = xor2(sprite_store.EPUM_STORE7_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.DYZE*/ wire _DYZE_STORE7_MATCH2n_new = xor2(sprite_store.EROL_STORE7_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.DESO*/ wire _DESO_STORE7_MATCH3n_new = xor2(sprite_store.EHYN_STORE7_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.EJOT*/ wire _EJOT_STORE7_MATCH4n_new = xor2(sprite_store.FAZU_STORE7_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ESAJ*/ wire _ESAJ_STORE7_MATCH5n_new = xor2(sprite_store.FAXE_STORE7_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.DUCU*/ wire _DUCU_STORE7_MATCH6n_new = xor2(sprite_store.EXUK_STORE7_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.EWUD*/ wire _EWUD_STORE7_MATCH7n_new = xor2(sprite_store.FEDE_STORE7_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.DUZE*/ wire _DUZE_STORE8_MATCH0n_new = xor2(sprite_store.EZUF_STORE8_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.DAGA*/ wire _DAGA_STORE8_MATCH1n_new = xor2(sprite_store.ENAD_STORE8_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.DAWU*/ wire _DAWU_STORE8_MATCH2n_new = xor2(sprite_store.EBOW_STORE8_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.EJAW*/ wire _EJAW_STORE8_MATCH3n_new = xor2(sprite_store.FYCA_STORE8_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.GOHO*/ wire _GOHO_STORE8_MATCH4n_new = xor2(sprite_store.GAVY_STORE8_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.GASU*/ wire _GASU_STORE8_MATCH5n_new = xor2(sprite_store.GYPU_STORE8_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.GABU*/ wire _GABU_STORE8_MATCH6n_new = xor2(sprite_store.GADY_STORE8_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.GAFE*/ wire _GAFE_STORE8_MATCH7n_new = xor2(sprite_store.GAZA_STORE8_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.YMAM*/ wire _YMAM_STORE9_MATCH0n_new = xor2(sprite_store.XUVY_STORE9_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.YTYP*/ wire _YTYP_STORE9_MATCH1n_new = xor2(sprite_store.XERE_STORE9_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.YFOP*/ wire _YFOP_STORE9_MATCH2n_new = xor2(sprite_store.XUZO_STORE9_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.YVAC*/ wire _YVAC_STORE9_MATCH3n_new = xor2(sprite_store.XEXA_STORE9_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.ZYWU*/ wire _ZYWU_STORE9_MATCH4n_new = xor2(sprite_store.YPOD_STORE9_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ZUZA*/ wire _ZUZA_STORE9_MATCH5n_new = xor2(sprite_store.YROP_STORE9_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.ZEJO*/ wire _ZEJO_STORE9_MATCH6n_new = xor2(sprite_store.YNEP_STORE9_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.ZEDA*/ wire _ZEDA_STORE9_MATCH7n_new = xor2(sprite_store.YZOF_STORE9_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.ZAKO*/ wire _ZAKO_STORE0_MATCHAp_new = nor4(_ZOGY_STORE0_MATCH0n_new, _ZEBA_STORE0_MATCH1n_new, _ZAHA_STORE0_MATCH2n_new, _ZOKY_STORE0_MATCH3n_new);
  /* p31.XEBA*/ wire _XEBA_STORE0_MATCHBp_new = nor4(_WOJU_STORE0_MATCH4n_new, _YFUN_STORE0_MATCH5n_new, _WYZA_STORE0_MATCH6n_new, _YPUK_STORE0_MATCH7n_new);
  /* p31.CYVY*/ wire _CYVY_STORE1_MATCHBp_new = nor4(_COLA_STORE1_MATCH4n_new, _BOBA_STORE1_MATCH5n_new, _COLU_STORE1_MATCH6n_new, _BAHU_STORE1_MATCH7n_new);
  /* p31.EWAM*/ wire _EWAM_STORE1_MATCHAp_new = nor4(_EDYM_STORE1_MATCH0n_new, _EMYB_STORE1_MATCH1n_new, _EBEF_STORE1_MATCH2n_new, _EWOK_STORE1_MATCH3n_new);
  /* p31.CEHU*/ wire _CEHU_STORE2_MATCHAp_new = nor4(_CEKO_STORE2_MATCH4n_new, _DETY_STORE2_MATCH5n_new, _DOZO_STORE2_MATCH6n_new, _CONY_STORE2_MATCH7n_new);
  /* p31.EKES*/ wire _EKES_STORE2_MATCHBp_new = nor4(_FUZU_STORE2_MATCH0n_new, _FESO_STORE2_MATCH1n_new, _FOKY_STORE2_MATCH2n_new, _FYVA_STORE2_MATCH3n_new);
  /* p31.ZURE*/ wire _ZURE_STORE3_MATCHAp_new = nor4(_YHOK_STORE3_MATCH0n_new, _YCAH_STORE3_MATCH1n_new, _YDAJ_STORE3_MATCH2n_new, _YVUZ_STORE3_MATCH3n_new);
  /* p31.YWOS*/ wire _YWOS_STORE3_MATCHBp_new = nor4(_YVAP_STORE3_MATCH4n_new, _XENY_STORE3_MATCH5n_new, _XAVU_STORE3_MATCH6n_new, _XEVA_STORE3_MATCH7n_new);
  /* p31.YKOK*/ wire _YKOK_STORE4_MATCHAp_new = nor4(_XEJU_STORE4_MATCH0n_new, _ZATE_STORE4_MATCH1n_new, _ZAKU_STORE4_MATCH2n_new, _YBOX_STORE4_MATCH3n_new);
  /* p31.YNAZ*/ wire _YNAZ_STORE4_MATCHBp_new = nor4(_ZYKU_STORE4_MATCH4n_new, _ZYPU_STORE4_MATCH5n_new, _XAHA_STORE4_MATCH6n_new, _ZEFE_STORE4_MATCH7n_new);
  /* p31.COGY*/ wire _COGY_STORE5_MATCHAp_new = nor4(_BAZY_STORE5_MATCH4n_new, _CYLE_STORE5_MATCH5n_new, _CEVA_STORE5_MATCH6n_new, _BUMY_STORE5_MATCH7n_new);
  /* p31.FYMA*/ wire _FYMA_STORE5_MATCHBp_new = nor4(_GUZO_STORE5_MATCH0n_new, _GOLA_STORE5_MATCH1n_new, _GEVE_STORE5_MATCH2n_new, _GUDE_STORE5_MATCH3n_new);
  /* p31.YWAP*/ wire _YWAP_STORE6_MATCHAp_new = nor4(_ZARE_STORE6_MATCH4n_new, _ZEMU_STORE6_MATCH5n_new, _ZYGO_STORE6_MATCH6n_new, _ZUZY_STORE6_MATCH7n_new);
  /* p31.YDOT*/ wire _YDOT_STORE6_MATCHBp_new = nor4(_XOSU_STORE6_MATCH0n_new, _ZUVU_STORE6_MATCH1n_new, _XUCO_STORE6_MATCH2n_new, _ZULO_STORE6_MATCH3n_new);
  /* p31.CYCO*/ wire _CYCO_STORE7_MATCHAp_new = nor4(_DUSE_STORE7_MATCH0n_new, _DAGU_STORE7_MATCH1n_new, _DYZE_STORE7_MATCH2n_new, _DESO_STORE7_MATCH3n_new);
  /* p31.DAJE*/ wire _DAJE_STORE7_MATCHBp_new = nor4(_EJOT_STORE7_MATCH4n_new, _ESAJ_STORE7_MATCH5n_new, _DUCU_STORE7_MATCH6n_new, _EWUD_STORE7_MATCH7n_new);
  /* p31.DAMA*/ wire _DAMA_STORE8_MATCHAp_new = nor4(_DUZE_STORE8_MATCH0n_new, _DAGA_STORE8_MATCH1n_new, _DAWU_STORE8_MATCH2n_new, _EJAW_STORE8_MATCH3n_new);
  /* p31.FEHA*/ wire _FEHA_STORE8_MATCHBp_new = nor4(_GOHO_STORE8_MATCH4n_new, _GASU_STORE8_MATCH5n_new, _GABU_STORE8_MATCH6n_new, _GAFE_STORE8_MATCH7n_new);
  /* p31.YLEV*/ wire _YLEV_STORE9_MATCHAp_new = nor4(_YMAM_STORE9_MATCH0n_new, _YTYP_STORE9_MATCH1n_new, _YFOP_STORE9_MATCH2n_new, _YVAC_STORE9_MATCH3n_new);
  /* p31.YTUB*/ wire _YTUB_STORE9_MATCHBp_new = nor4(_ZYWU_STORE9_MATCH4n_new, _ZUZA_STORE9_MATCH5n_new, _ZEJO_STORE9_MATCH6n_new, _ZEDA_STORE9_MATCH7n_new);

  /* p29.YDUG*/ wire _YDUG_STORE0_MATCHn = nand3(_AROR_MATCH_ENp_new, _ZAKO_STORE0_MATCHAp_new, _XEBA_STORE0_MATCHBp_new);
  /* p29.DYDU*/ wire _DYDU_STORE1_MATCHn = nand3(_AROR_MATCH_ENp_new, _EWAM_STORE1_MATCHAp_new, _CYVY_STORE1_MATCHBp_new);
  /* p29.DEGO*/ wire _DEGO_STORE2_MATCHn = nand3(_AROR_MATCH_ENp_new, _CEHU_STORE2_MATCHAp_new, _EKES_STORE2_MATCHBp_new);
  /* p29.YLOZ*/ wire _YLOZ_STORE3_MATCHn = nand3(_AROR_MATCH_ENp_new, _ZURE_STORE3_MATCHAp_new, _YWOS_STORE3_MATCHBp_new);
  /* p29.XAGE*/ wire _XAGE_STORE4_MATCHn = nand3(_AROR_MATCH_ENp_new, _YKOK_STORE4_MATCHAp_new, _YNAZ_STORE4_MATCHBp_new);
  /* p29.EGOM*/ wire _EGOM_STORE5_MATCHn = nand3(_AROR_MATCH_ENp_new, _COGY_STORE5_MATCHAp_new, _FYMA_STORE5_MATCHBp_new);
  /* p29.YBEZ*/ wire _YBEZ_STORE6_MATCHn = nand3(_AROR_MATCH_ENp_new, _YWAP_STORE6_MATCHAp_new, _YDOT_STORE6_MATCHBp_new);
  /* p29.DYKA*/ wire _DYKA_STORE7_MATCHn = nand3(_AROR_MATCH_ENp_new, _CYCO_STORE7_MATCHAp_new, _DAJE_STORE7_MATCHBp_new);
  /* p29.EFYL*/ wire _EFYL_STORE8_MATCHn = nand3(_AROR_MATCH_ENp_new, _DAMA_STORE8_MATCHAp_new, _FEHA_STORE8_MATCHBp_new);
  /* p29.YGEM*/ wire _YGEM_STORE9_MATCHn = nand3(_AROR_MATCH_ENp_new, _YLEV_STORE9_MATCHAp_new, _YTUB_STORE9_MATCHBp_new);

  return {
    bit(_YDUG_STORE0_MATCHn),
    bit(_DYDU_STORE1_MATCHn),
    bit(_DEGO_STORE2_MATCHn),
    bit(_YLOZ_STORE3_MATCHn),
    bit(_XAGE_STORE4_MATCHn),
    bit(_EGOM_STORE5_MATCHn),
    bit(_YBEZ_STORE6_MATCHn),
    bit(_DYKA_STORE7_MATCHn),
    bit(_EFYL_STORE8_MATCHn),
    bit(_YGEM_STORE9_MATCHn)
  };
}

//------------------------------------------------------------------------------------------------------------------------

SpriteFirstMatch GateBoy::get_first_match(SpriteMatchFlag match_flag) const {
  /* p29.WEFU*/ wire _WEFU_STORE0_MATCH = not1(match_flag.YDUG_STORE0_MATCHn.qp_new());
  /* p29.GAJA*/ wire _GAJA_STORE1_MATCH = not1(match_flag.DYDU_STORE1_MATCHn.qp_new());
  /* p29.GUPO*/ wire _GUPO_STORE2_MATCH = not1(match_flag.DEGO_STORE2_MATCHn.qp_new());
  /* p29.WEBO*/ wire _WEBO_STORE3_MATCH = not1(match_flag.YLOZ_STORE3_MATCHn.qp_new());
  /* p29.WUNA*/ wire _WUNA_STORE4_MATCH = not1(match_flag.XAGE_STORE4_MATCHn.qp_new());
  /* p29.GABA*/ wire _GABA_STORE5_MATCH = not1(match_flag.EGOM_STORE5_MATCHn.qp_new());
  /* p29.WASE*/ wire _WASE_STORE6_MATCH = not1(match_flag.YBEZ_STORE6_MATCHn.qp_new());
  /* p29.GYTE*/ wire _GYTE_STORE7_MATCH = not1(match_flag.DYKA_STORE7_MATCHn.qp_new());
  /* p29.GEKE*/ wire _GEKE_STORE8_MATCH = not1(match_flag.EFYL_STORE8_MATCHn.qp_new());

  /* p29.GEZE*/ wire _GEZE_STORE0_MATCH = or2(_WEFU_STORE0_MATCH, SIG_GND.qp_new());
  /* p29.FUMA*/ wire _FUMA_STORE1_MATCH = or2(_GAJA_STORE1_MATCH, _GEZE_STORE0_MATCH);
  /* p29.GEDE*/ wire _GEDE_STORE2_MATCH = or2(_GUPO_STORE2_MATCH, _FUMA_STORE1_MATCH);
  /* p29.WUTO*/ wire _WUTO_STORE3_MATCH = or2(_WEBO_STORE3_MATCH, _GEDE_STORE2_MATCH);
  /* p29.XYLA*/ wire _XYLA_STORE4_MATCH = or2(_WUNA_STORE4_MATCH, _WUTO_STORE3_MATCH);
  /* p29.WEJA*/ wire _WEJA_STORE5_MATCH = or2(_GABA_STORE5_MATCH, _XYLA_STORE4_MATCH);
  /* p29.WYLA*/ wire _WYLA_STORE6_MATCH = or2(_WASE_STORE6_MATCH, _WEJA_STORE5_MATCH);
  /* p29.FAVO*/ wire _FAVO_STORE7_MATCH = or2(_GYTE_STORE7_MATCH, _WYLA_STORE6_MATCH);
  /* p29.GYGA*/ wire _GYGA_STORE8_MATCH = or2(_GEKE_STORE8_MATCH, _FAVO_STORE7_MATCH);

  /* p29.GUVA*/ wire _GUVA_SPRITE0_GETp = nor2(match_flag.YDUG_STORE0_MATCHn.qp_new(), SIG_GND.qp_new());
  /* p29.ENUT*/ wire _ENUT_SPRITE1_GETp = nor2(match_flag.DYDU_STORE1_MATCHn.qp_new(), _GEZE_STORE0_MATCH);
  /* p29.EMOL*/ wire _EMOL_SPRITE2_GETp = nor2(match_flag.DEGO_STORE2_MATCHn.qp_new(), _FUMA_STORE1_MATCH);
  /* p29.GYFY*/ wire _GYFY_SPRITE3_GETp = nor2(match_flag.YLOZ_STORE3_MATCHn.qp_new(), _GEDE_STORE2_MATCH);
  /* p29.GONO*/ wire _GONO_SPRITE4_GETp = nor2(match_flag.XAGE_STORE4_MATCHn.qp_new(), _WUTO_STORE3_MATCH);
  /* p29.GEGA*/ wire _GEGA_SPRITE5_GETp = nor2(match_flag.EGOM_STORE5_MATCHn.qp_new(), _XYLA_STORE4_MATCH);
  /* p29.XOJA*/ wire _XOJA_SPRITE6_GETp = nor2(match_flag.YBEZ_STORE6_MATCHn.qp_new(), _WEJA_STORE5_MATCH);
  /* p29.GUTU*/ wire _GUTU_SPRITE7_GETp = nor2(match_flag.DYKA_STORE7_MATCHn.qp_new(), _WYLA_STORE6_MATCH);
  /* p29.FOXA*/ wire _FOXA_SPRITE8_GETp = nor2(match_flag.EFYL_STORE8_MATCHn.qp_new(), _FAVO_STORE7_MATCH);
  /* p29.GUZE*/ wire _GUZE_SPRITE9_GETp = nor2(match_flag.YGEM_STORE9_MATCHn.qp_new(), _GYGA_STORE8_MATCH);

  return {
    bit(_GUVA_SPRITE0_GETp),
    bit(_ENUT_SPRITE1_GETp),
    bit(_EMOL_SPRITE2_GETp),
    bit(_GYFY_SPRITE3_GETp),
    bit(_GONO_SPRITE4_GETp),
    bit(_GEGA_SPRITE5_GETp),
    bit(_XOJA_SPRITE6_GETp),
    bit(_GUTU_SPRITE7_GETp),
    bit(_FOXA_SPRITE8_GETp),
    bit(_GUZE_SPRITE9_GETp)
  };
}

//------------------------------------------------------------------------------------------------------------------------
// Sprite store output tribuf driver. The store regs are set during scan, so it's safe to read the old values here.

void GateBoy::get_sprite() {

  /* p29.FURO*/ wire _FURO_SPRITE0_GETn = not1(first_match.GUVA_SPRITE0_GETp.qp_new());
  /* p29.DYDO*/ wire _DYDO_SPRITE1_GETn = not1(first_match.ENUT_SPRITE1_GETp.qp_new());
  /* p29.FAME*/ wire _FAME_SPRITE2_GETn = not1(first_match.EMOL_SPRITE2_GETp.qp_new());
  /* p29.GYMA*/ wire _GYMA_SPRITE3_GETn = not1(first_match.GYFY_SPRITE3_GETp.qp_new());
  /* p29.GOWO*/ wire _GOWO_SPRITE4_GETn = not1(first_match.GONO_SPRITE4_GETp.qp_new());
  /* p29.GYGY*/ wire _GYGY_SPRITE5_GETn = not1(first_match.GEGA_SPRITE5_GETp.qp_new());
  /* p29.XYME*/ wire _XYME_SPRITE6_GETn = not1(first_match.XOJA_SPRITE6_GETp.qp_new());
  /* p29.GUGY*/ wire _GUGY_SPRITE7_GETn = not1(first_match.GUTU_SPRITE7_GETp.qp_new());
  /* p29.DENY*/ wire _DENY_SPRITE8_GETn = not1(first_match.FOXA_SPRITE8_GETp.qp_new());
  /* p29.FADO*/ wire _FADO_SPRITE9_GETn = not1(first_match.GUZE_SPRITE9_GETp.qp_new());

  /* p30.ZETU_STORE0_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.YGUS_STORE0_I0n_odd.qp_new());
  /* p30.ZECE_STORE0_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.YSOK_STORE0_I1n_odd.qp_new());
  /* p30.ZAVE_STORE0_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.YZEP_STORE0_I2n_odd.qp_new());
  /* p30.WOKO_STORE0_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.WYTE_STORE0_I3n_odd.qp_new());
  /* p30.ZUMU_STORE0_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.ZONY_STORE0_I4n_odd.qp_new());
  /*#p30.ZEDY_STORE0_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.YWAK_STORE0_I5n_odd.qp_new());
  /* p30.WEHE_STORE0_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.GYHO_STORE0_L0n_odd.qp_new());
  /* p30.BUKY_STORE0_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.CUFO_STORE0_L1n_odd.qp_new());
  /* p30.AJAL_STORE0_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.BOZU_STORE0_L2n_odd.qp_new());
  /* p30.GOFO_STORE0_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_FURO_SPRITE0_GETn, sprite_store.FYHY_STORE0_L3n_odd.qp_new());

  /*#p30.CUBO_STORE1_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CADU_STORE1_I0n_odd.qp_new());
  /* p30.CELU_STORE1_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CEBO_STORE1_I1n_odd.qp_new());
  /* p30.CEGY_STORE1_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CUFA_STORE1_I2n_odd.qp_new());
  /* p30.BETY_STORE1_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.COMA_STORE1_I3n_odd.qp_new());
  /* p30.CYBY_STORE1_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CUZA_STORE1_I4n_odd.qp_new());
  /* p30.BEMO_STORE1_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.CAJY_STORE1_I5n_odd.qp_new());
  /* p30.BYRO_STORE1_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.AMES_STORE1_L0n_odd.qp_new());
  /* p30.AHUM_STORE1_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.AROF_STORE1_L1n_odd.qp_new());
  /* p30.BACO_STORE1_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.ABOP_STORE1_L2n_odd.qp_new());
  /* p30.BEFE_STORE1_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_DYDO_SPRITE1_GETn, sprite_store.ABUG_STORE1_L3n_odd.qp_new());

  /* p30.CUBE_STORE2_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BUHE_STORE2_I0n_odd.qp_new());
  /* p30.AFOZ_STORE2_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BYHU_STORE2_I1n_odd.qp_new());
  /* p30.APON_STORE2_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BECA_STORE2_I2n_odd.qp_new());
  /* p30.CUVU_STORE2_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BULU_STORE2_I3n_odd.qp_new());
  /* p30.CYRO_STORE2_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BUNA_STORE2_I4n_odd.qp_new());
  /* p30.AXEC_STORE2_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.BOXA_STORE2_I5n_odd.qp_new());
  /* p30.ZUKE_STORE2_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.YLOV_STORE2_L0n_odd.qp_new());
  /* p30.WERE_STORE2_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.XOSY_STORE2_L1n_odd.qp_new());
  /* p30.WUXE_STORE2_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.XAZY_STORE2_L2n_odd.qp_new());
  /* p30.ZABY_STORE2_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_FAME_SPRITE2_GETn, sprite_store.YKUK_STORE2_L3n_odd.qp_new());

  /* p30.ENAP_STORE3_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DEVY_STORE3_I0n_odd.qp_new());
  /* p30.DYGO_STORE3_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DESE_STORE3_I1n_odd.qp_new());
  /* p30.DOWA_STORE3_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DUNY_STORE3_I2n_odd.qp_new());
  /* p30.DONY_STORE3_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DUHA_STORE3_I3n_odd.qp_new());
  /* p30.EFUD_STORE3_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DEBA_STORE3_I4n_odd.qp_new());
  /* p30.DEZU_STORE3_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.DAFU_STORE3_I5n_odd.qp_new());
  /* p30.ZEXE_STORE3_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.ZURO_STORE3_L0n_odd.qp_new());
  /* p30.YWAV_STORE3_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.ZYLU_STORE3_L1n_odd.qp_new());
  /* p30.YJEM_STORE3_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.ZENE_STORE3_L2n_odd.qp_new());
  /* p30.ZYPO_STORE3_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_GYMA_SPRITE3_GETn, sprite_store.ZURY_STORE3_L3n_odd.qp_new());

  /* p30.WUXU_STORE4_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XAVE_STORE4_I0n_odd.qp_new());
  /* p30.WEPY_STORE4_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XEFE_STORE4_I1n_odd.qp_new());
  /* p30.WERU_STORE4_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.WANU_STORE4_I2n_odd.qp_new());
  /* p30.XYRE_STORE4_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XABO_STORE4_I3n_odd.qp_new());
  /* p30.WOXY_STORE4_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XEGE_STORE4_I4n_odd.qp_new());
  /* p30.WAJA_STORE4_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.XYNU_STORE4_I5n_odd.qp_new());
  /* p30.BUCE_STORE4_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.CAPO_STORE4_L0n_odd.qp_new());
  /* p30.BEVY_STORE4_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.CAJU_STORE4_L1n_odd.qp_new());
  /* p30.BOVE_STORE4_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.CONO_STORE4_L2n_odd.qp_new());
  /* p30.BYDO_STORE4_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_GOWO_SPRITE4_GETn, sprite_store.CUMU_STORE4_L3n_odd.qp_new());

  /* p30.DOBO_STORE5_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.EKOP_STORE5_I0n_odd.qp_new());
  /* p30.DYNY_STORE5_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ETYM_STORE5_I1n_odd.qp_new());
  /* p30.WAGA_STORE5_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.GORU_STORE5_I2n_odd.qp_new());
  /* p30.DUZA_STORE5_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.EBEX_STORE5_I3n_odd.qp_new());
  /* p30.DALY_STORE5_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ETAV_STORE5_I4n_odd.qp_new());
  /* p30.DALO_STORE5_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.EKAP_STORE5_I5n_odd.qp_new());
  /* p30.BACE_STORE5_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ACEP_STORE5_L0n_odd.qp_new());
  /* p30.BUJA_STORE5_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ABEG_STORE5_L1n_odd.qp_new());
  /* p30.BODU_STORE5_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ABUX_STORE5_L2n_odd.qp_new());
  /* p30.AWAT_STORE5_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_GYGY_SPRITE5_GETn, sprite_store.ANED_STORE5_L3n_odd.qp_new());

  /* p30.WATO_STORE6_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.GABO_STORE6_I0n_odd.qp_new());
  /* p30.WYWY_STORE6_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.GACY_STORE6_I1n_odd.qp_new());
  /* p30.EZOC_STORE6_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.FOGO_STORE6_I2n_odd.qp_new());
  /* p30.WABO_STORE6_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.GOHU_STORE6_I3n_odd.qp_new());
  /* p30.ELYC_STORE6_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.FOXY_STORE6_I4n_odd.qp_new());
  /* p30.WOCY_STORE6_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.GECU_STORE6_I5n_odd.qp_new());
  /* p30.YBUK_STORE6_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.ZUMY_STORE6_L0n_odd.qp_new());
  /* p30.YKOZ_STORE6_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.ZAFU_STORE6_L1n_odd.qp_new());
  /* p30.ZYTO_STORE6_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.ZEXO_STORE6_L2n_odd.qp_new());
  /* p30.ZUDO_STORE6_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_XYME_SPRITE6_GETn, sprite_store.ZUBE_STORE6_L3n_odd.qp_new());

  /* p30.WAKO_STORE7_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.GULE_STORE7_I0n_odd.qp_new());
  /* p30.WYGO_STORE7_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.GYNO_STORE7_I1n_odd.qp_new());
  /* p30.ELEP_STORE7_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.FEFA_STORE7_I2n_odd.qp_new());
  /* p30.ETAD_STORE7_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.FYSU_STORE7_I3n_odd.qp_new());
  /* p30.WABA_STORE7_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.GESY_STORE7_I4n_odd.qp_new());
  /* p30.EVYT_STORE7_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.FUZO_STORE7_I5n_odd.qp_new());
  /* p30.WAXE_STORE7_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.XYNA_STORE7_L0n_odd.qp_new());
  /* p30.YPOZ_STORE7_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.YGUM_STORE7_L1n_odd.qp_new());
  /* p30.WABU_STORE7_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.XAKU_STORE7_L2n_odd.qp_new());
  /* p30.WANA_STORE7_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_GUGY_SPRITE7_GETn, sprite_store.XYGO_STORE7_L3n_odd.qp_new());

  /* p30.APOC_STORE8_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AXUV_STORE8_I0n_odd.qp_new());
  /* p30.AKYH_STORE8_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.BADA_STORE8_I1n_odd.qp_new());
  /* p30.AFEN_STORE8_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.APEV_STORE8_I2n_odd.qp_new());
  /* p30.APYV_STORE8_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.BADO_STORE8_I3n_odd.qp_new());
  /* p30.APOB_STORE8_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.BEXY_STORE8_I4n_odd.qp_new());
  /* p30.ADYB_STORE8_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.BYHE_STORE8_I5n_odd.qp_new());
  /* p30.BOSO_STORE8_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AZAP_STORE8_L0n_odd.qp_new());
  /* p30.BAZU_STORE8_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AFYX_STORE8_L1n_odd.qp_new());
  /* p30.AHAC_STORE8_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AFUT_STORE8_L2n_odd.qp_new());
  /* p30.BUJY_STORE8_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_DENY_SPRITE8_GETn, sprite_store.AFYM_STORE8_L3n_odd.qp_new());

  /* p30.ZARO_STORE9_GET_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.YBER_STORE9_I0n_odd.qp_new());
  /* p30.ZOJY_STORE9_GET_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.YZOR_STORE9_I1n_odd.qp_new());
  /* p30.YNEV_STORE9_GET_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.XYFE_STORE9_I2n_odd.qp_new());
  /* p30.XYRA_STORE9_GET_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.XOTU_STORE9_I3n_odd.qp_new());
  /* p30.YRAD_STORE9_GET_L0*/ new_bus.BUS_SPR_I4.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.XUTE_STORE9_I4n_odd.qp_new());
  /* p30.YHAL_STORE9_GET_L1*/ new_bus.BUS_SPR_I5.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.XUFO_STORE9_I5n_odd.qp_new());
  /* p30.BYME_STORE9_GET_L2*/ new_bus.BUS_SPR_L0.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.CANA_STORE9_L0n_odd.qp_new());
  /* p30.GATE_STORE9_GET_L3*/ new_bus.BUS_SPR_L1.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.FOFO_STORE9_L1n_odd.qp_new());
  /* p30.COHO_STORE9_GET_L4*/ new_bus.BUS_SPR_L2.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.DYSY_STORE9_L2n_odd.qp_new());
  /* p30.CAWO_STORE9_GET_L5*/ new_bus.BUS_SPR_L3.tri6_nn(_FADO_SPRITE9_GETn, sprite_store.DEWU_STORE9_L3n_odd.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::store_sprite(wire _DYTY_COUNT_CLKp) {
  SpriteStoreFlag store_flag = get_store_flags(_DYTY_COUNT_CLKp);

  // Sprite store X resetter. This has to happen before the match check.
  /* p28.ABAK*/ wire _ABAK_LINE_RSTp = or2(ATEJ_LINE_RSTp.qp_new(), AMYG_VID_RSTp());
  /* p28.BYVA*/ wire _BYVA_LINE_RSTn = not1(_ABAK_LINE_RSTp);
  /* p29.DYBA*/ wire _DYBA_LINE_RSTp = not1(_BYVA_LINE_RSTn);

  /* p29.EBOJ*/ sprite_store.EBOJ_STORE0_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.GUVA_SPRITE0_GETp.qp_old());
  /* p29.CEDY*/ sprite_store.CEDY_STORE1_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.ENUT_SPRITE1_GETp.qp_old());
  /* p29.EGAV*/ sprite_store.EGAV_STORE2_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.EMOL_SPRITE2_GETp.qp_old());
  /* p29.GOTA*/ sprite_store.GOTA_STORE3_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.GYFY_SPRITE3_GETp.qp_old());
  /* p29.XUDY*/ sprite_store.XUDY_STORE4_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.GONO_SPRITE4_GETp.qp_old());
  /* p29.WAFY*/ sprite_store.WAFY_STORE5_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.GEGA_SPRITE5_GETp.qp_old());
  /* p29.WOMY*/ sprite_store.WOMY_STORE6_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.XOJA_SPRITE6_GETp.qp_old());
  /* p29.WAPO*/ sprite_store.WAPO_STORE7_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.GUTU_SPRITE7_GETp.qp_old());
  /* p29.EXUQ*/ sprite_store.EXUQ_STORE8_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.FOXA_SPRITE8_GETp.qp_old());
  /* p29.FONO*/ sprite_store.FONO_STORE9_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp.qp_new(), _BYVA_LINE_RSTn, first_match.GUZE_SPRITE9_GETp.qp_old());

  /* p29.DYWE*/ wire _DYWE_STORE0_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.EBOJ_STORE0_RSTp_evn.qp_new());
  /* p29.EFEV*/ wire _EFEV_STORE1_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.CEDY_STORE1_RSTp_evn.qp_new());
  /* p29.FOKO*/ wire _FOKO_STORE2_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.EGAV_STORE2_RSTp_evn.qp_new());
  /* p29.GAKE*/ wire _GAKE_STORE3_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.GOTA_STORE3_RSTp_evn.qp_new());
  /* p29.WOHU*/ wire _WOHU_STORE4_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.XUDY_STORE4_RSTp_evn.qp_new());
  /* p29.FEVE*/ wire _FEVE_STORE5_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.WAFY_STORE5_RSTp_evn.qp_new());
  /* p29.WACY*/ wire _WACY_STORE6_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.WOMY_STORE6_RSTp_evn.qp_new());
  /* p29.GUKY*/ wire _GUKY_STORE7_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.WAPO_STORE7_RSTp_evn.qp_new());
  /* p29.GORO*/ wire _GORO_STORE8_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.EXUQ_STORE8_RSTp_evn.qp_new());
  /* p29.DUBU*/ wire _DUBU_STORE9_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.FONO_STORE9_RSTp_evn.qp_new());

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


  /* p29.FUXU*/ wire _FUXU_STORE0_clk = not1(store_flag.DYHU_STORE0_CLKn.qp_new());
  /* p29.ASYS*/ wire _ASYS_STORE1_clk = not1(store_flag.BUCO_STORE1_CLKn.qp_new());
  /* p29.CACU*/ wire _CACU_STORE2_clk = not1(store_flag.GYFO_STORE2_CLKn.qp_new());
  /* p29.YFAG*/ wire _YFAG_STORE3_clk = not1(store_flag.GUSA_STORE3_CLKn.qp_new());
  /* p29.WOFO*/ wire _WOFO_STORE4_clk = not1(store_flag.DUKE_STORE4_CLKn.qp_new());
  /* p29.CYLA*/ wire _CYLA_STORE5_clk = not1(store_flag.BEDE_STORE5_CLKn.qp_new());
  /* p29.ZAPE*/ wire _ZAPE_STORE6_clk = not1(store_flag.WEKA_STORE6_CLKn.qp_new());
  /* p29.GECY*/ wire _GECY_STORE7_clk = not1(store_flag.GYVO_STORE7_CLKn.qp_new());
  /* p29.CEXU*/ wire _CEXU_STORE8_clk = not1(store_flag.BUKA_STORE8_CLKn.qp_new());
  /* p29.WEME*/ wire _WEME_STORE9_clk = not1(store_flag.DECU_STORE9_CLKn.qp_new());

  /*#p31.ZAGO*/ wire _ZAGO_SPX0n_old = not1(oam_temp_b.YLOR_OAM_DB0p.qp_old());
  /* p31.ZOCY*/ wire _ZOCY_SPX1n_old = not1(oam_temp_b.ZYTY_OAM_DB1p.qp_old());
  /* p31.YPUR*/ wire _YPUR_SPX2n_old = not1(oam_temp_b.ZYVE_OAM_DB2p.qp_old());
  /* p31.YVOK*/ wire _YVOK_SPX3n_old = not1(oam_temp_b.ZEZY_OAM_DB3p.qp_old());
  /* p31.COSE*/ wire _COSE_SPX4n_old = not1(oam_temp_b.GOMO_OAM_DB4p.qp_old());
  /* p31.AROP*/ wire _AROP_SPX5n_old = not1(oam_temp_b.BAXO_OAM_DB5p.qp_old());
  /* p31.XATU*/ wire _XATU_SPX6n_old = not1(oam_temp_b.YZOS_OAM_DB6p.qp_old());
  /* p31.BADY*/ wire _BADY_SPX7n_old = not1(oam_temp_b.DEPO_OAM_DB7p.qp_old());


  /*#p31.XEPE*/ sprite_store.XEPE_STORE0_X0p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _ZAGO_SPX0n_old);
  /* p31.YLAH*/ sprite_store.YLAH_STORE0_X1p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _ZOCY_SPX1n_old);
  /* p31.ZOLA*/ sprite_store.ZOLA_STORE0_X2p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _YPUR_SPX2n_old);
  /* p31.ZULU*/ sprite_store.ZULU_STORE0_X3p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _YVOK_SPX3n_old);
  /* p31.WELO*/ sprite_store.WELO_STORE0_X4p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _COSE_SPX4n_old);
  /* p31.XUNY*/ sprite_store.XUNY_STORE0_X5p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _AROP_SPX5n_old);
  /* p31.WOTE*/ sprite_store.WOTE_STORE0_X6p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _XATU_SPX6n_old);
  /* p31.XAKO*/ sprite_store.XAKO_STORE0_X7p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _BADY_SPX7n_old);

  /* p31.DANY*/ sprite_store.DANY_STORE1_X0p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _ZAGO_SPX0n_old);
  /* p31.DUKO*/ sprite_store.DUKO_STORE1_X1p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _ZOCY_SPX1n_old);
  /* p31.DESU*/ sprite_store.DESU_STORE1_X2p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _YPUR_SPX2n_old);
  /* p31.DAZO*/ sprite_store.DAZO_STORE1_X3p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _YVOK_SPX3n_old);
  /* p31.DAKE*/ sprite_store.DAKE_STORE1_X4p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _COSE_SPX4n_old);
  /* p31.CESO*/ sprite_store.CESO_STORE1_X5p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _AROP_SPX5n_old);
  /* p31.DYFU*/ sprite_store.DYFU_STORE1_X6p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _XATU_SPX6n_old);
  /* p31.CUSY*/ sprite_store.CUSY_STORE1_X7p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _BADY_SPX7n_old);

  /* p31.FOKA*/ sprite_store.FOKA_STORE2_X0p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _ZAGO_SPX0n_old);
  /* p31.FYTY*/ sprite_store.FYTY_STORE2_X1p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _ZOCY_SPX1n_old);
  /* p31.FUBY*/ sprite_store.FUBY_STORE2_X2p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _YPUR_SPX2n_old);
  /* p31.GOXU*/ sprite_store.GOXU_STORE2_X3p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _YVOK_SPX3n_old);
  /* p31.DUHY*/ sprite_store.DUHY_STORE2_X4p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _COSE_SPX4n_old);
  /* p31.EJUF*/ sprite_store.EJUF_STORE2_X5p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _AROP_SPX5n_old);
  /* p31.ENOR*/ sprite_store.ENOR_STORE2_X6p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _XATU_SPX6n_old);
  /* p31.DEPY*/ sprite_store.DEPY_STORE2_X7p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _BADY_SPX7n_old);

  /* p31.XOLY*/ sprite_store.XOLY_STORE3_X0p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _ZAGO_SPX0n_old);
  /* p31.XYBA*/ sprite_store.XYBA_STORE3_X1p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _ZOCY_SPX1n_old);
  /* p31.XABE*/ sprite_store.XABE_STORE3_X2p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _YPUR_SPX2n_old);
  /* p31.XEKA*/ sprite_store.XEKA_STORE3_X3p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _YVOK_SPX3n_old);
  /* p31.XOMY*/ sprite_store.XOMY_STORE3_X4p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _COSE_SPX4n_old);
  /* p31.WUHA*/ sprite_store.WUHA_STORE3_X5p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _AROP_SPX5n_old);
  /* p31.WYNA*/ sprite_store.WYNA_STORE3_X6p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _XATU_SPX6n_old);
  /* p31.WECO*/ sprite_store.WECO_STORE3_X7p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _BADY_SPX7n_old);

  /* p31.WEDU*/ sprite_store.WEDU_STORE4_X0p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _ZAGO_SPX0n_old);
  /* p31.YGAJ*/ sprite_store.YGAJ_STORE4_X1p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _ZOCY_SPX1n_old);
  /* p31.ZYJO*/ sprite_store.ZYJO_STORE4_X2p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _YPUR_SPX2n_old);
  /* p31.XURY*/ sprite_store.XURY_STORE4_X3p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _YVOK_SPX3n_old);
  /* p31.YBED*/ sprite_store.YBED_STORE4_X4p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _COSE_SPX4n_old);
  /* p31.ZALA*/ sprite_store.ZALA_STORE4_X5p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _AROP_SPX5n_old);
  /* p31.WYDE*/ sprite_store.WYDE_STORE4_X6p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _XATU_SPX6n_old);
  /* p31.XEPA*/ sprite_store.XEPA_STORE4_X7p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _BADY_SPX7n_old);

  /* p31.FUSA*/ sprite_store.FUSA_STORE5_X0p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _ZAGO_SPX0n_old);
  /* p31.FAXA*/ sprite_store.FAXA_STORE5_X1p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _ZOCY_SPX1n_old);
  /* p31.FOZY*/ sprite_store.FOZY_STORE5_X2p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _YPUR_SPX2n_old);
  /* p31.FESY*/ sprite_store.FESY_STORE5_X3p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _YVOK_SPX3n_old);
  /* p31.CYWE*/ sprite_store.CYWE_STORE5_X4p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _COSE_SPX4n_old);
  /* p31.DYBY*/ sprite_store.DYBY_STORE5_X5p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _AROP_SPX5n_old);
  /* p31.DURY*/ sprite_store.DURY_STORE5_X6p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _XATU_SPX6n_old);
  /* p31.CUVY*/ sprite_store.CUVY_STORE5_X7p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _BADY_SPX7n_old);

  /* p31.YCOL*/ sprite_store.YCOL_STORE6_X0p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _ZAGO_SPX0n_old);
  /* p31.YRAC*/ sprite_store.YRAC_STORE6_X1p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _ZOCY_SPX1n_old);
  /* p31.YMEM*/ sprite_store.YMEM_STORE6_X2p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _YPUR_SPX2n_old);
  /* p31.YVAG*/ sprite_store.YVAG_STORE6_X3p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _YVOK_SPX3n_old);
  /* p31.ZOLY*/ sprite_store.ZOLY_STORE6_X4p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _COSE_SPX4n_old);
  /* p31.ZOGO*/ sprite_store.ZOGO_STORE6_X5p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _AROP_SPX5n_old);
  /* p31.ZECU*/ sprite_store.ZECU_STORE6_X6p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _XATU_SPX6n_old);
  /* p31.ZESA*/ sprite_store.ZESA_STORE6_X7p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _BADY_SPX7n_old);

  /* p31.ERAZ*/ sprite_store.ERAZ_STORE7_X0p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _ZAGO_SPX0n_old);
  /* p31.EPUM*/ sprite_store.EPUM_STORE7_X1p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _ZOCY_SPX1n_old);
  /* p31.EROL*/ sprite_store.EROL_STORE7_X2p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _YPUR_SPX2n_old);
  /* p31.EHYN*/ sprite_store.EHYN_STORE7_X3p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _YVOK_SPX3n_old);
  /* p31.FAZU*/ sprite_store.FAZU_STORE7_X4p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _COSE_SPX4n_old);
  /* p31.FAXE*/ sprite_store.FAXE_STORE7_X5p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _AROP_SPX5n_old);
  /* p31.EXUK*/ sprite_store.EXUK_STORE7_X6p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _XATU_SPX6n_old);
  /* p31.FEDE*/ sprite_store.FEDE_STORE7_X7p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _BADY_SPX7n_old);

  /* p31.EZUF*/ sprite_store.EZUF_STORE8_X0p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _ZAGO_SPX0n_old);
  /* p31.ENAD*/ sprite_store.ENAD_STORE8_X1p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _ZOCY_SPX1n_old);
  /* p31.EBOW*/ sprite_store.EBOW_STORE8_X2p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _YPUR_SPX2n_old);
  /* p31.FYCA*/ sprite_store.FYCA_STORE8_X3p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _YVOK_SPX3n_old);
  /* p31.GAVY*/ sprite_store.GAVY_STORE8_X4p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _COSE_SPX4n_old);
  /* p31.GYPU*/ sprite_store.GYPU_STORE8_X5p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _AROP_SPX5n_old);
  /* p31.GADY*/ sprite_store.GADY_STORE8_X6p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _XATU_SPX6n_old);
  /* p31.GAZA*/ sprite_store.GAZA_STORE8_X7p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _BADY_SPX7n_old);

  /* p31.XUVY*/ sprite_store.XUVY_STORE9_X0p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _ZAGO_SPX0n_old);
  /* p31.XERE*/ sprite_store.XERE_STORE9_X1p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _ZOCY_SPX1n_old);
  /* p31.XUZO*/ sprite_store.XUZO_STORE9_X2p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _YPUR_SPX2n_old);
  /* p31.XEXA*/ sprite_store.XEXA_STORE9_X3p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _YVOK_SPX3n_old);
  /* p31.YPOD*/ sprite_store.YPOD_STORE9_X4p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _COSE_SPX4n_old);
  /* p31.YROP*/ sprite_store.YROP_STORE9_X5p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _AROP_SPX5n_old);
  /* p31.YNEP*/ sprite_store.YNEP_STORE9_X6p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _XATU_SPX6n_old);
  /* p31.YZOF*/ sprite_store.YZOF_STORE9_X7p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _BADY_SPX7n_old);

  /* p29.GENY*/ wire _GENY_STORE0_CLKp = not1(store_flag.DYHU_STORE0_CLKn.qp_new());
  /* p29.BYVY*/ wire _BYVY_STORE1_CLKp = not1(store_flag.BUCO_STORE1_CLKn.qp_new());
  /* p29.BUZY*/ wire _BUZY_STORE2_CLKp = not1(store_flag.GYFO_STORE2_CLKn.qp_new());
  /* p29.FEKA*/ wire _FEKA_STORE3_CLKp = not1(store_flag.GUSA_STORE3_CLKn.qp_new());
  /* p29.WYLU*/ wire _WYLU_STORE4_CLKp = not1(store_flag.DUKE_STORE4_CLKn.qp_new());
  /* p29.DYMO*/ wire _DYMO_STORE5_CLKp = not1(store_flag.BEDE_STORE5_CLKn.qp_new());
  /* p29.WUSE*/ wire _WUSE_STORE6_CLKp = not1(store_flag.WEKA_STORE6_CLKn.qp_new());
  /* p29.FEFO*/ wire _FEFO_STORE7_CLKp = not1(store_flag.GYVO_STORE7_CLKn.qp_new());
  /* p29.AKOL*/ wire _AKOL_STORE8_CLKp = not1(store_flag.BUKA_STORE8_CLKn.qp_new());
  /* p29.WUFA*/ wire _WUFA_STORE9_CLKp = not1(store_flag.DECU_STORE9_CLKn.qp_new());

  /* p30.YGUS*/ sprite_store.YGUS_STORE0_I0n_odd.dff8n(_GENY_STORE0_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.YSOK*/ sprite_store.YSOK_STORE0_I1n_odd.dff8n(_GENY_STORE0_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.YZEP*/ sprite_store.YZEP_STORE0_I2n_odd.dff8n(_GENY_STORE0_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.WYTE*/ sprite_store.WYTE_STORE0_I3n_odd.dff8n(_GENY_STORE0_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.ZONY*/ sprite_store.ZONY_STORE0_I4n_odd.dff8n(_GENY_STORE0_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.YWAK*/ sprite_store.YWAK_STORE0_I5n_odd.dff8n(_GENY_STORE0_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p30.CADU*/ sprite_store.CADU_STORE1_I0n_odd.dff8n(_BYVY_STORE1_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.CEBO*/ sprite_store.CEBO_STORE1_I1n_odd.dff8n(_BYVY_STORE1_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.CUFA*/ sprite_store.CUFA_STORE1_I2n_odd.dff8n(_BYVY_STORE1_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.COMA*/ sprite_store.COMA_STORE1_I3n_odd.dff8n(_BYVY_STORE1_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.CUZA*/ sprite_store.CUZA_STORE1_I4n_odd.dff8n(_BYVY_STORE1_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.CAJY*/ sprite_store.CAJY_STORE1_I5n_odd.dff8n(_BYVY_STORE1_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p30.BUHE*/ sprite_store.BUHE_STORE2_I0n_odd.dff8n(_BUZY_STORE2_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.BYHU*/ sprite_store.BYHU_STORE2_I1n_odd.dff8n(_BUZY_STORE2_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.BECA*/ sprite_store.BECA_STORE2_I2n_odd.dff8n(_BUZY_STORE2_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.BULU*/ sprite_store.BULU_STORE2_I3n_odd.dff8n(_BUZY_STORE2_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.BUNA*/ sprite_store.BUNA_STORE2_I4n_odd.dff8n(_BUZY_STORE2_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.BOXA*/ sprite_store.BOXA_STORE2_I5n_odd.dff8n(_BUZY_STORE2_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p30.DEVY*/ sprite_store.DEVY_STORE3_I0n_odd.dff8n(_FEKA_STORE3_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.DESE*/ sprite_store.DESE_STORE3_I1n_odd.dff8n(_FEKA_STORE3_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.DUNY*/ sprite_store.DUNY_STORE3_I2n_odd.dff8n(_FEKA_STORE3_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.DUHA*/ sprite_store.DUHA_STORE3_I3n_odd.dff8n(_FEKA_STORE3_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.DEBA*/ sprite_store.DEBA_STORE3_I4n_odd.dff8n(_FEKA_STORE3_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.DAFU*/ sprite_store.DAFU_STORE3_I5n_odd.dff8n(_FEKA_STORE3_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p30.XAVE*/ sprite_store.XAVE_STORE4_I0n_odd.dff8n(_WYLU_STORE4_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.XEFE*/ sprite_store.XEFE_STORE4_I1n_odd.dff8n(_WYLU_STORE4_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.WANU*/ sprite_store.WANU_STORE4_I2n_odd.dff8n(_WYLU_STORE4_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.XABO*/ sprite_store.XABO_STORE4_I3n_odd.dff8n(_WYLU_STORE4_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.XEGE*/ sprite_store.XEGE_STORE4_I4n_odd.dff8n(_WYLU_STORE4_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.XYNU*/ sprite_store.XYNU_STORE4_I5n_odd.dff8n(_WYLU_STORE4_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p30.EKOP*/ sprite_store.EKOP_STORE5_I0n_odd.dff8n(_DYMO_STORE5_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.ETYM*/ sprite_store.ETYM_STORE5_I1n_odd.dff8n(_DYMO_STORE5_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.GORU*/ sprite_store.GORU_STORE5_I2n_odd.dff8n(_DYMO_STORE5_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.EBEX*/ sprite_store.EBEX_STORE5_I3n_odd.dff8n(_DYMO_STORE5_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.ETAV*/ sprite_store.ETAV_STORE5_I4n_odd.dff8n(_DYMO_STORE5_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.EKAP*/ sprite_store.EKAP_STORE5_I5n_odd.dff8n(_DYMO_STORE5_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p30.GABO*/ sprite_store.GABO_STORE6_I0n_odd.dff8n(_WUSE_STORE6_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.GACY*/ sprite_store.GACY_STORE6_I1n_odd.dff8n(_WUSE_STORE6_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.FOGO*/ sprite_store.FOGO_STORE6_I2n_odd.dff8n(_WUSE_STORE6_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.GOHU*/ sprite_store.GOHU_STORE6_I3n_odd.dff8n(_WUSE_STORE6_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.FOXY*/ sprite_store.FOXY_STORE6_I4n_odd.dff8n(_WUSE_STORE6_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.GECU*/ sprite_store.GECU_STORE6_I5n_odd.dff8n(_WUSE_STORE6_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p30.GULE*/ sprite_store.GULE_STORE7_I0n_odd.dff8n(_FEFO_STORE7_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.GYNO*/ sprite_store.GYNO_STORE7_I1n_odd.dff8n(_FEFO_STORE7_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.FEFA*/ sprite_store.FEFA_STORE7_I2n_odd.dff8n(_FEFO_STORE7_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.FYSU*/ sprite_store.FYSU_STORE7_I3n_odd.dff8n(_FEFO_STORE7_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.GESY*/ sprite_store.GESY_STORE7_I4n_odd.dff8n(_FEFO_STORE7_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.FUZO*/ sprite_store.FUZO_STORE7_I5n_odd.dff8n(_FEFO_STORE7_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p30.AXUV*/ sprite_store.AXUV_STORE8_I0n_odd.dff8n(_AKOL_STORE8_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.BADA*/ sprite_store.BADA_STORE8_I1n_odd.dff8n(_AKOL_STORE8_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.APEV*/ sprite_store.APEV_STORE8_I2n_odd.dff8n(_AKOL_STORE8_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.BADO*/ sprite_store.BADO_STORE8_I3n_odd.dff8n(_AKOL_STORE8_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.BEXY*/ sprite_store.BEXY_STORE8_I4n_odd.dff8n(_AKOL_STORE8_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.BYHE*/ sprite_store.BYHE_STORE8_I5n_odd.dff8n(_AKOL_STORE8_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p30.YBER*/ sprite_store.YBER_STORE9_I0n_odd.dff8n(_WUFA_STORE9_CLKp, old_bus.BUS_SPR_I0.qp_old());
  /* p30.YZOR*/ sprite_store.YZOR_STORE9_I1n_odd.dff8n(_WUFA_STORE9_CLKp, old_bus.BUS_SPR_I1.qp_old());
  /* p30.XYFE*/ sprite_store.XYFE_STORE9_I2n_odd.dff8n(_WUFA_STORE9_CLKp, old_bus.BUS_SPR_I2.qp_old());
  /* p30.XOTU*/ sprite_store.XOTU_STORE9_I3n_odd.dff8n(_WUFA_STORE9_CLKp, old_bus.BUS_SPR_I3.qp_old());
  /* p30.XUTE*/ sprite_store.XUTE_STORE9_I4n_odd.dff8n(_WUFA_STORE9_CLKp, old_bus.BUS_SPR_I4.qp_old());
  /* p30.XUFO*/ sprite_store.XUFO_STORE9_I5n_odd.dff8n(_WUFA_STORE9_CLKp, old_bus.BUS_SPR_I5.qp_old());

  /* p29.ENOB*/ wire _ENOB_STORE0_CLKp = not1(store_flag.DYHU_STORE0_CLKn.qp_new());
  /* p29.AHOF*/ wire _AHOF_STORE1_CLKp = not1(store_flag.BUCO_STORE1_CLKn.qp_new());
  /* p29.FUKE*/ wire _FUKE_STORE2_CLKp = not1(store_flag.GYFO_STORE2_CLKn.qp_new());
  /* p29.XYHA*/ wire _XYHA_STORE3_CLKp = not1(store_flag.GUSA_STORE3_CLKn.qp_new());
  /* p29.EWOT*/ wire _EWOT_STORE4_CLKp = not1(store_flag.DUKE_STORE4_CLKn.qp_new());
  /* p29.BUCY*/ wire _BUCY_STORE5_CLKp = not1(store_flag.BEDE_STORE5_CLKn.qp_new());
  /* p29.ZURU*/ wire _ZURU_STORE6_CLKp = not1(store_flag.WEKA_STORE6_CLKn.qp_new());
  /* p29.WABE*/ wire _WABE_STORE7_CLKp = not1(store_flag.GYVO_STORE7_CLKn.qp_new());
  /* p29.BYMY*/ wire _BYMY_STORE8_CLKp = not1(store_flag.BUKA_STORE8_CLKn.qp_new());
  /* p29.FAKA*/ wire _FAKA_STORE9_CLKp = not1(store_flag.DECU_STORE9_CLKn.qp_new());

  /* p30.GYHO*/ sprite_store.GYHO_STORE0_L0n_odd.dff8n(_ENOB_STORE0_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.CUFO*/ sprite_store.CUFO_STORE0_L1n_odd.dff8n(_ENOB_STORE0_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.BOZU*/ sprite_store.BOZU_STORE0_L2n_odd.dff8n(_ENOB_STORE0_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.FYHY*/ sprite_store.FYHY_STORE0_L3n_odd.dff8n(_ENOB_STORE0_CLKp, old_bus.BUS_SPR_L3.qp_old());

  /* p30.AMES*/ sprite_store.AMES_STORE1_L0n_odd.dff8n(_AHOF_STORE1_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.AROF*/ sprite_store.AROF_STORE1_L1n_odd.dff8n(_AHOF_STORE1_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.ABOP*/ sprite_store.ABOP_STORE1_L2n_odd.dff8n(_AHOF_STORE1_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.ABUG*/ sprite_store.ABUG_STORE1_L3n_odd.dff8n(_AHOF_STORE1_CLKp, old_bus.BUS_SPR_L3.qp_old());

  /* p30.YLOV*/ sprite_store.YLOV_STORE2_L0n_odd.dff8n(_FUKE_STORE2_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.XOSY*/ sprite_store.XOSY_STORE2_L1n_odd.dff8n(_FUKE_STORE2_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.XAZY*/ sprite_store.XAZY_STORE2_L2n_odd.dff8n(_FUKE_STORE2_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.YKUK*/ sprite_store.YKUK_STORE2_L3n_odd.dff8n(_FUKE_STORE2_CLKp, old_bus.BUS_SPR_L3.qp_old());

  /* p30.ZURO*/ sprite_store.ZURO_STORE3_L0n_odd.dff8n(_XYHA_STORE3_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.ZYLU*/ sprite_store.ZYLU_STORE3_L1n_odd.dff8n(_XYHA_STORE3_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.ZENE*/ sprite_store.ZENE_STORE3_L2n_odd.dff8n(_XYHA_STORE3_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.ZURY*/ sprite_store.ZURY_STORE3_L3n_odd.dff8n(_XYHA_STORE3_CLKp, old_bus.BUS_SPR_L3.qp_old());

  /* p30.CAPO*/ sprite_store.CAPO_STORE4_L0n_odd.dff8n(_EWOT_STORE4_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.CAJU*/ sprite_store.CAJU_STORE4_L1n_odd.dff8n(_EWOT_STORE4_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.CONO*/ sprite_store.CONO_STORE4_L2n_odd.dff8n(_EWOT_STORE4_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.CUMU*/ sprite_store.CUMU_STORE4_L3n_odd.dff8n(_EWOT_STORE4_CLKp, old_bus.BUS_SPR_L3.qp_old());

  /* p30.ACEP*/ sprite_store.ACEP_STORE5_L0n_odd.dff8n(_BUCY_STORE5_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.ABEG*/ sprite_store.ABEG_STORE5_L1n_odd.dff8n(_BUCY_STORE5_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.ABUX*/ sprite_store.ABUX_STORE5_L2n_odd.dff8n(_BUCY_STORE5_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.ANED*/ sprite_store.ANED_STORE5_L3n_odd.dff8n(_BUCY_STORE5_CLKp, old_bus.BUS_SPR_L3.qp_old());

  /* p30.ZUMY*/ sprite_store.ZUMY_STORE6_L0n_odd.dff8n(_ZURU_STORE6_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.ZAFU*/ sprite_store.ZAFU_STORE6_L1n_odd.dff8n(_ZURU_STORE6_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.ZEXO*/ sprite_store.ZEXO_STORE6_L2n_odd.dff8n(_ZURU_STORE6_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.ZUBE*/ sprite_store.ZUBE_STORE6_L3n_odd.dff8n(_ZURU_STORE6_CLKp, old_bus.BUS_SPR_L3.qp_old());

  /* p30.XYNA*/ sprite_store.XYNA_STORE7_L0n_odd.dff8n(_WABE_STORE7_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.YGUM*/ sprite_store.YGUM_STORE7_L1n_odd.dff8n(_WABE_STORE7_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.XAKU*/ sprite_store.XAKU_STORE7_L2n_odd.dff8n(_WABE_STORE7_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.XYGO*/ sprite_store.XYGO_STORE7_L3n_odd.dff8n(_WABE_STORE7_CLKp, old_bus.BUS_SPR_L3.qp_old());

  /* p30.AZAP*/ sprite_store.AZAP_STORE8_L0n_odd.dff8n(_BYMY_STORE8_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.AFYX*/ sprite_store.AFYX_STORE8_L1n_odd.dff8n(_BYMY_STORE8_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.AFUT*/ sprite_store.AFUT_STORE8_L2n_odd.dff8n(_BYMY_STORE8_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.AFYM*/ sprite_store.AFYM_STORE8_L3n_odd.dff8n(_BYMY_STORE8_CLKp, old_bus.BUS_SPR_L3.qp_old());

  /* p30.CANA*/ sprite_store.CANA_STORE9_L0n_odd.dff8n(_FAKA_STORE9_CLKp, old_bus.BUS_SPR_L0.qp_old());
  /* p30.FOFO*/ sprite_store.FOFO_STORE9_L1n_odd.dff8n(_FAKA_STORE9_CLKp, old_bus.BUS_SPR_L1.qp_old());
  /* p30.DYSY*/ sprite_store.DYSY_STORE9_L2n_odd.dff8n(_FAKA_STORE9_CLKp, old_bus.BUS_SPR_L2.qp_old());
  /* p30.DEWU*/ sprite_store.DEWU_STORE9_L3n_odd.dff8n(_FAKA_STORE9_CLKp, old_bus.BUS_SPR_L3.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::ly_to_sprite_line() {
  /*#p29.EBOS*/ wire EBOS_LY0n = not1(reg_ly.MUWY_LY0p.qp_new());
  /* p29.DASA*/ wire DASA_LY1n = not1(reg_ly.MYRO_LY1p.qp_new());
  /* p29.FUKY*/ wire FUKY_LY2n = not1(reg_ly.LEXA_LY2p.qp_new());
  /* p29.FUVE*/ wire FUVE_LY3n = not1(reg_ly.LYDO_LY3p.qp_new());

  /* p29.ERUC*/ auto _ERUC_YDIFF0 = add3(EBOS_LY0n, oam_temp_a.XUSO_OAM_DA0p.qp_new(), SIG_GND.qp_new());
  /* p29.ENEF*/ auto _ENEF_YDIFF1 = add3(DASA_LY1n, oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF0.carry);
  /* p29.FECO*/ auto _FECO_YDIFF2 = add3(FUKY_LY2n, oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF1.carry);
  /* p29.GYKY*/ auto _GYKY_YDIFF3 = add3(FUVE_LY3n, oam_temp_a.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF2.carry);

  /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0_new = not1(_ERUC_YDIFF0.sum);
  /* p29.DABY*/ wire _DABY_SPRITE_DELTA1_new = not1(_ENEF_YDIFF1.sum);
  /* p29.DABU*/ wire _DABU_SPRITE_DELTA2_new = not1(_FECO_YDIFF2.sum);
  /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3_new = not1(_GYKY_YDIFF3.sum);

  /*#p30.CUCU_STORE_L0*/ new_bus.BUS_SPR_L0.tri6_nn(FEPO_STORE_MATCHp.qp_new(), _DEGE_SPRITE_DELTA0_new);
  /*#p30.CUCA_STORE_L1*/ new_bus.BUS_SPR_L1.tri6_nn(FEPO_STORE_MATCHp.qp_new(), _DABY_SPRITE_DELTA1_new);
  /*#p30.CEGA_STORE_L2*/ new_bus.BUS_SPR_L2.tri6_nn(FEPO_STORE_MATCHp.qp_new(), _DABU_SPRITE_DELTA2_new);
  /*#p30.WENU_STORE_L3*/ new_bus.BUS_SPR_L3.tri6_nn(FEPO_STORE_MATCHp.qp_new(), _GYSA_SPRITE_DELTA3_new);
}

//------------------------------------------------------------------------------------------------------------------------
