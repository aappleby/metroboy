#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoyPixPipe.h"
#include "GateBoyLib/GateBoyOamBus.h"
#include "GateBoyLib/GateBoyLCD.h"

//------------------------------------------------------------------------------------------------------------------------

void SpriteStore::reset_to_cart() {
  XADU_SPRITE_IDX0p.reset(1, 0);
  XEDY_SPRITE_IDX1p.reset(1, 0);
  ZUZE_SPRITE_IDX2p.reset(1, 1);
  XOBE_SPRITE_IDX3p.reset(1, 0);
  YDUF_SPRITE_IDX4p.reset(1, 1);
  XECU_SPRITE_IDX5p.reset(1, 0);

  DEZY_COUNT_CLKp.reset(1, 1);
  BESE_SPRITE_COUNT0.reset(1, 0);
  CUXY_SPRITE_COUNT1.reset(1, 0);
  BEGO_SPRITE_COUNT2.reset(1, 0);
  DYBE_SPRITE_COUNT3.reset(1, 0);
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteStore::update_count(wire2 XAPO_VID_RSTn, wire2 ZEME_AxCxExGx, wire2 ATEJ_LINE_RSTp, wire2 _DYTY_STORE_CLKp_new_odd) {
  // Sprite store counter. The sprite count clock stops ticking once we have 10 sprites.
  // Sprite Y matcher. This is using an adder as a subtracter by inverting the first input.

  /*#p28.AZYB*/ wire2 _AZYB_LINE_TRIGn = not1b(ATEJ_LINE_RSTp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p29.BAKY*/ wire2 _BAKY_SPRITES_FULL_new = and2(CUXY_SPRITE_COUNT1.qp_any2(), DYBE_SPRITE_COUNT3.qp_any2());

    /* p29.DEZY*/ DEZY_COUNT_CLKp.dff17_any(ZEME_AxCxExGx, XAPO_VID_RSTn, _DYTY_STORE_CLKp_new_odd);
    /*#p29.CAKE*/ wire2 _CAKE_COUNT_CLKp_new = or2(_BAKY_SPRITES_FULL_new, DEZY_COUNT_CLKp.qp_any2());

    /* p29.BESE*/ BESE_SPRITE_COUNT0.dff17_any(_CAKE_COUNT_CLKp_new,         _AZYB_LINE_TRIGn, BESE_SPRITE_COUNT0.qn_any2());
    /* p29.CUXY*/ CUXY_SPRITE_COUNT1.dff17_any(BESE_SPRITE_COUNT0.qn_any2(), _AZYB_LINE_TRIGn, CUXY_SPRITE_COUNT1.qn_any2());
    /* p29.BEGO*/ BEGO_SPRITE_COUNT2.dff17_any(CUXY_SPRITE_COUNT1.qn_any2(), _AZYB_LINE_TRIGn, BEGO_SPRITE_COUNT2.qn_any2());
    /* p29.DYBE*/ DYBE_SPRITE_COUNT3.dff17_any(BEGO_SPRITE_COUNT2.qn_any2(), _AZYB_LINE_TRIGn, DYBE_SPRITE_COUNT3.qn_any2());
  }
}

//------------------------------------------------------------------------------------------------------------------------

SpriteStoreFlag SpriteStore::get_store_flags(wire2 _DYTY_STORE_CLKp_new_odd) {
  /* p29.EDEN*/ wire2 _EDEN_SPRITE_COUNT0n_new = not1b(BESE_SPRITE_COUNT0.qp_new2());
  /* p29.CYPY*/ wire2 _CYPY_SPRITE_COUNT1n_new = not1b(CUXY_SPRITE_COUNT1.qp_new2());
  /* p29.CAPE*/ wire2 _CAPE_SPRITE_COUNT2n_new = not1b(BEGO_SPRITE_COUNT2.qp_new2());
  /* p29.CAXU*/ wire2 _CAXU_SPRITE_COUNT3n_new = not1b(DYBE_SPRITE_COUNT3.qp_new2());

  /* p29.FYCU*/ wire2 _FYCU_SPRITE_COUNT0p_new = not1b(_EDEN_SPRITE_COUNT0n_new);
  /* p29.FONE*/ wire2 _FONE_SPRITE_COUNT1p_new = not1b(_CYPY_SPRITE_COUNT1n_new);
  /* p29.EKUD*/ wire2 _EKUD_SPRITE_COUNT2p_new = not1b(_CAPE_SPRITE_COUNT2n_new);
  /* p29.ELYG*/ wire2 _ELYG_SPRITE_COUNT3p_new = not1b(_CAXU_SPRITE_COUNT3n_new);

  /* p29.DEZO*/ wire2 _DEZO_STORE0_SELn_new = nand4b(_EDEN_SPRITE_COUNT0n_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.CUVA*/ wire2 _CUVA_STORE1_SELn_new = nand4b(_FYCU_SPRITE_COUNT0p_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.GEBU*/ wire2 _GEBU_STORE2_SELn_new = nand4b(_EDEN_SPRITE_COUNT0n_new, _FONE_SPRITE_COUNT1p_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.FOCO*/ wire2 _FOCO_STORE3_SELn_new = nand4b(_FYCU_SPRITE_COUNT0p_new, _FONE_SPRITE_COUNT1p_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.CUPE*/ wire2 _CUPE_STORE4_SELn_new = nand4b(_EDEN_SPRITE_COUNT0n_new, _CYPY_SPRITE_COUNT1n_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.CUGU*/ wire2 _CUGU_STORE5_SELn_new = nand4b(_FYCU_SPRITE_COUNT0p_new, _CYPY_SPRITE_COUNT1n_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.WOMU*/ wire2 _WOMU_STORE6_SELn_new = nand4b(_EDEN_SPRITE_COUNT0n_new, _FONE_SPRITE_COUNT1p_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.GUNA*/ wire2 _GUNA_STORE7_SELn_new = nand4b(_FYCU_SPRITE_COUNT0p_new, _FONE_SPRITE_COUNT1p_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.DEWY*/ wire2 _DEWY_STORE8_SELn_new = nand4b(_EDEN_SPRITE_COUNT0n_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _ELYG_SPRITE_COUNT3p_new);
  /* p29.DOGU*/ wire2 _DOGU_STORE9_SELn_new = nand4b(_FYCU_SPRITE_COUNT0p_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _ELYG_SPRITE_COUNT3p_new);

  /* p29.CEMY*/ wire2 _CEMY_STORE0_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _DEZO_STORE0_SELn_new);
  /* p29.BYBY*/ wire2 _BYBY_STORE1_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _CUVA_STORE1_SELn_new);
  /* p29.WYXO*/ wire2 _WYXO_STORE2_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _GEBU_STORE2_SELn_new);
  /* p29.GUVE*/ wire2 _GUVE_STORE3_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _FOCO_STORE3_SELn_new);
  /* p29.CECU*/ wire2 _CECU_STORE4_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _CUPE_STORE4_SELn_new);
  /* p29.CADO*/ wire2 _CADO_STORE5_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _CUGU_STORE5_SELn_new);
  /* p29.XUJO*/ wire2 _XUJO_STORE6_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _WOMU_STORE6_SELn_new);
  /* p29.GAPE*/ wire2 _GAPE_STORE7_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _GUNA_STORE7_SELn_new);
  /* p29.CAHO*/ wire2 _CAHO_STORE8_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _DEWY_STORE8_SELn_new);
  /* p29.CATO*/ wire2 _CATO_STORE9_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _DOGU_STORE9_SELn_new);

  /* p29.DYHU*/ wire2 _DYHU_STORE0_CLKn_new_odd = not1b(_CEMY_STORE0_CLKp_new_odd);
  /* p29.BUCO*/ wire2 _BUCO_STORE1_CLKn_new_odd = not1b(_BYBY_STORE1_CLKp_new_odd);
  /* p29.GYFO*/ wire2 _GYFO_STORE2_CLKn_new_odd = not1b(_WYXO_STORE2_CLKp_new_odd);
  /* p29.GUSA*/ wire2 _GUSA_STORE3_CLKn_new_odd = not1b(_GUVE_STORE3_CLKp_new_odd);
  /* p29.DUKE*/ wire2 _DUKE_STORE4_CLKn_new_odd = not1b(_CECU_STORE4_CLKp_new_odd);
  /* p29.BEDE*/ wire2 _BEDE_STORE5_CLKn_new_odd = not1b(_CADO_STORE5_CLKp_new_odd);
  /* p29.WEKA*/ wire2 _WEKA_STORE6_CLKn_new_odd = not1b(_XUJO_STORE6_CLKp_new_odd);
  /* p29.GYVO*/ wire2 _GYVO_STORE7_CLKn_new_odd = not1b(_GAPE_STORE7_CLKp_new_odd);
  /* p29.BUKA*/ wire2 _BUKA_STORE8_CLKn_new_odd = not1b(_CAHO_STORE8_CLKp_new_odd);
  /* p29.DECU*/ wire2 _DECU_STORE9_CLKn_new_odd = not1b(_CATO_STORE9_CLKp_new_odd);

  return {
    _DYHU_STORE0_CLKn_new_odd,
    _BUCO_STORE1_CLKn_new_odd,
    _GYFO_STORE2_CLKn_new_odd,
    _GUSA_STORE3_CLKn_new_odd,
    _DUKE_STORE4_CLKn_new_odd,
    _BEDE_STORE5_CLKn_new_odd,
    _WEKA_STORE6_CLKn_new_odd,
    _GYVO_STORE7_CLKn_new_odd,
    _BUKA_STORE8_CLKn_new_odd,
    _DECU_STORE9_CLKn_new_odd
  };
}

//------------------------------------------------------------------------------------------------------------------------

SpriteMatchFlag SpriteStore::get_match_flags_old(wire2 _AROR_MATCH_ENp_old, const PixCount& _pix_count) {
  /*#p21.ACAM*/ wire2 ACAM_PX0n_old = not1b(_pix_count.XEHO_PX0p.qp_old2());
  /* p21.AZUB*/ wire2 AZUB_PX1n_old = not1b(_pix_count.SAVY_PX1p.qp_old2());
  /* p21.AMEL*/ wire2 AMEL_PX2n_old = not1b(_pix_count.XODU_PX2p.qp_old2());
  /* p21.AHAL*/ wire2 AHAL_PX3n_old = not1b(_pix_count.XYDO_PX3p.qp_old2());
  /* p21.APUX*/ wire2 APUX_PX4n_old = not1b(_pix_count.TUHU_PX4p.qp_old2());
  /* p21.ABEF*/ wire2 ABEF_PX5n_old = not1b(_pix_count.TUKY_PX5p.qp_old2());
  /* p21.ADAZ*/ wire2 ADAZ_PX6n_old = not1b(_pix_count.TAKO_PX6p.qp_old2());
  /* p21.ASAH*/ wire2 ASAH_PX7n_old = not1b(_pix_count.SYBE_PX7p.qp_old2());

  /*#p31.ZOGY*/ wire2 _ZOGY_STORE0_MATCH0n_old = xor2(XEPE_STORE0_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.ZEBA*/ wire2 _ZEBA_STORE0_MATCH1n_old = xor2(YLAH_STORE0_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.ZAHA*/ wire2 _ZAHA_STORE0_MATCH2n_old = xor2(ZOLA_STORE0_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.ZOKY*/ wire2 _ZOKY_STORE0_MATCH3n_old = xor2(ZULU_STORE0_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.WOJU*/ wire2 _WOJU_STORE0_MATCH4n_old = xor2(WELO_STORE0_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.YFUN*/ wire2 _YFUN_STORE0_MATCH5n_old = xor2(XUNY_STORE0_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.WYZA*/ wire2 _WYZA_STORE0_MATCH6n_old = xor2(WOTE_STORE0_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.YPUK*/ wire2 _YPUK_STORE0_MATCH7n_old = xor2(XAKO_STORE0_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.EDYM*/ wire2 _EDYM_STORE1_MATCH0n_old = xor2(DANY_STORE1_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.EMYB*/ wire2 _EMYB_STORE1_MATCH1n_old = xor2(DUKO_STORE1_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.EBEF*/ wire2 _EBEF_STORE1_MATCH2n_old = xor2(DESU_STORE1_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.EWOK*/ wire2 _EWOK_STORE1_MATCH3n_old = xor2(DAZO_STORE1_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.COLA*/ wire2 _COLA_STORE1_MATCH4n_old = xor2(DAKE_STORE1_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.BOBA*/ wire2 _BOBA_STORE1_MATCH5n_old = xor2(CESO_STORE1_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.COLU*/ wire2 _COLU_STORE1_MATCH6n_old = xor2(DYFU_STORE1_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.BAHU*/ wire2 _BAHU_STORE1_MATCH7n_old = xor2(CUSY_STORE1_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.FUZU*/ wire2 _FUZU_STORE2_MATCH0n_old = xor2(FOKA_STORE2_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.FESO*/ wire2 _FESO_STORE2_MATCH1n_old = xor2(FYTY_STORE2_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.FOKY*/ wire2 _FOKY_STORE2_MATCH2n_old = xor2(FUBY_STORE2_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.FYVA*/ wire2 _FYVA_STORE2_MATCH3n_old = xor2(GOXU_STORE2_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.CEKO*/ wire2 _CEKO_STORE2_MATCH4n_old = xor2(DUHY_STORE2_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.DETY*/ wire2 _DETY_STORE2_MATCH5n_old = xor2(EJUF_STORE2_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.DOZO*/ wire2 _DOZO_STORE2_MATCH6n_old = xor2(ENOR_STORE2_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.CONY*/ wire2 _CONY_STORE2_MATCH7n_old = xor2(DEPY_STORE2_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.YHOK*/ wire2 _YHOK_STORE3_MATCH0n_old = xor2(XOLY_STORE3_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.YCAH*/ wire2 _YCAH_STORE3_MATCH1n_old = xor2(XYBA_STORE3_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.YDAJ*/ wire2 _YDAJ_STORE3_MATCH2n_old = xor2(XABE_STORE3_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.YVUZ*/ wire2 _YVUZ_STORE3_MATCH3n_old = xor2(XEKA_STORE3_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.YVAP*/ wire2 _YVAP_STORE3_MATCH4n_old = xor2(XOMY_STORE3_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.XENY*/ wire2 _XENY_STORE3_MATCH5n_old = xor2(WUHA_STORE3_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.XAVU*/ wire2 _XAVU_STORE3_MATCH6n_old = xor2(WYNA_STORE3_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.XEVA*/ wire2 _XEVA_STORE3_MATCH7n_old = xor2(WECO_STORE3_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.XEJU*/ wire2 _XEJU_STORE4_MATCH0n_old = xor2(WEDU_STORE4_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.ZATE*/ wire2 _ZATE_STORE4_MATCH1n_old = xor2(YGAJ_STORE4_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.ZAKU*/ wire2 _ZAKU_STORE4_MATCH2n_old = xor2(ZYJO_STORE4_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.YBOX*/ wire2 _YBOX_STORE4_MATCH3n_old = xor2(XURY_STORE4_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.ZYKU*/ wire2 _ZYKU_STORE4_MATCH4n_old = xor2(YBED_STORE4_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.ZYPU*/ wire2 _ZYPU_STORE4_MATCH5n_old = xor2(ZALA_STORE4_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.XAHA*/ wire2 _XAHA_STORE4_MATCH6n_old = xor2(WYDE_STORE4_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.ZEFE*/ wire2 _ZEFE_STORE4_MATCH7n_old = xor2(XEPA_STORE4_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.GUZO*/ wire2 _GUZO_STORE5_MATCH0n_old = xor2(FUSA_STORE5_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.GOLA*/ wire2 _GOLA_STORE5_MATCH1n_old = xor2(FAXA_STORE5_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.GEVE*/ wire2 _GEVE_STORE5_MATCH2n_old = xor2(FOZY_STORE5_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.GUDE*/ wire2 _GUDE_STORE5_MATCH3n_old = xor2(FESY_STORE5_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.BAZY*/ wire2 _BAZY_STORE5_MATCH4n_old = xor2(CYWE_STORE5_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.CYLE*/ wire2 _CYLE_STORE5_MATCH5n_old = xor2(DYBY_STORE5_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.CEVA*/ wire2 _CEVA_STORE5_MATCH6n_old = xor2(DURY_STORE5_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.BUMY*/ wire2 _BUMY_STORE5_MATCH7n_old = xor2(CUVY_STORE5_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.XOSU*/ wire2 _XOSU_STORE6_MATCH0n_old = xor2(YCOL_STORE6_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.ZUVU*/ wire2 _ZUVU_STORE6_MATCH1n_old = xor2(YRAC_STORE6_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.XUCO*/ wire2 _XUCO_STORE6_MATCH2n_old = xor2(YMEM_STORE6_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.ZULO*/ wire2 _ZULO_STORE6_MATCH3n_old = xor2(YVAG_STORE6_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.ZARE*/ wire2 _ZARE_STORE6_MATCH4n_old = xor2(ZOLY_STORE6_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.ZEMU*/ wire2 _ZEMU_STORE6_MATCH5n_old = xor2(ZOGO_STORE6_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.ZYGO*/ wire2 _ZYGO_STORE6_MATCH6n_old = xor2(ZECU_STORE6_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.ZUZY*/ wire2 _ZUZY_STORE6_MATCH7n_old = xor2(ZESA_STORE6_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.DUSE*/ wire2 _DUSE_STORE7_MATCH0n_old = xor2(ERAZ_STORE7_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.DAGU*/ wire2 _DAGU_STORE7_MATCH1n_old = xor2(EPUM_STORE7_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.DYZE*/ wire2 _DYZE_STORE7_MATCH2n_old = xor2(EROL_STORE7_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.DESO*/ wire2 _DESO_STORE7_MATCH3n_old = xor2(EHYN_STORE7_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.EJOT*/ wire2 _EJOT_STORE7_MATCH4n_old = xor2(FAZU_STORE7_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.ESAJ*/ wire2 _ESAJ_STORE7_MATCH5n_old = xor2(FAXE_STORE7_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.DUCU*/ wire2 _DUCU_STORE7_MATCH6n_old = xor2(EXUK_STORE7_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.EWUD*/ wire2 _EWUD_STORE7_MATCH7n_old = xor2(FEDE_STORE7_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.DUZE*/ wire2 _DUZE_STORE8_MATCH0n_old = xor2(EZUF_STORE8_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.DAGA*/ wire2 _DAGA_STORE8_MATCH1n_old = xor2(ENAD_STORE8_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.DAWU*/ wire2 _DAWU_STORE8_MATCH2n_old = xor2(EBOW_STORE8_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.EJAW*/ wire2 _EJAW_STORE8_MATCH3n_old = xor2(FYCA_STORE8_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.GOHO*/ wire2 _GOHO_STORE8_MATCH4n_old = xor2(GAVY_STORE8_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.GASU*/ wire2 _GASU_STORE8_MATCH5n_old = xor2(GYPU_STORE8_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.GABU*/ wire2 _GABU_STORE8_MATCH6n_old = xor2(GADY_STORE8_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.GAFE*/ wire2 _GAFE_STORE8_MATCH7n_old = xor2(GAZA_STORE8_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.YMAM*/ wire2 _YMAM_STORE9_MATCH0n_old = xor2(XUVY_STORE9_X0p_odd.qn_old2(), ACAM_PX0n_old);
  /* p31.YTYP*/ wire2 _YTYP_STORE9_MATCH1n_old = xor2(XERE_STORE9_X1p_odd.qn_old2(), AZUB_PX1n_old);
  /* p31.YFOP*/ wire2 _YFOP_STORE9_MATCH2n_old = xor2(XUZO_STORE9_X2p_odd.qn_old2(), AMEL_PX2n_old);
  /* p31.YVAC*/ wire2 _YVAC_STORE9_MATCH3n_old = xor2(XEXA_STORE9_X3p_odd.qn_old2(), AHAL_PX3n_old);
  /* p31.ZYWU*/ wire2 _ZYWU_STORE9_MATCH4n_old = xor2(YPOD_STORE9_X4p_odd.qn_old2(), APUX_PX4n_old);
  /* p31.ZUZA*/ wire2 _ZUZA_STORE9_MATCH5n_old = xor2(YROP_STORE9_X5p_odd.qn_old2(), ABEF_PX5n_old);
  /* p31.ZEJO*/ wire2 _ZEJO_STORE9_MATCH6n_old = xor2(YNEP_STORE9_X6p_odd.qn_old2(), ADAZ_PX6n_old);
  /* p31.ZEDA*/ wire2 _ZEDA_STORE9_MATCH7n_old = xor2(YZOF_STORE9_X7p_odd.qn_old2(), ASAH_PX7n_old);

  /* p31.ZAKO*/ wire2 _ZAKO_STORE0_MATCHAp_old = nor4b(_ZOGY_STORE0_MATCH0n_old, _ZEBA_STORE0_MATCH1n_old, _ZAHA_STORE0_MATCH2n_old, _ZOKY_STORE0_MATCH3n_old);
  /* p31.XEBA*/ wire2 _XEBA_STORE0_MATCHBp_old = nor4b(_WOJU_STORE0_MATCH4n_old, _YFUN_STORE0_MATCH5n_old, _WYZA_STORE0_MATCH6n_old, _YPUK_STORE0_MATCH7n_old);
  /* p31.CYVY*/ wire2 _CYVY_STORE1_MATCHBp_old = nor4b(_COLA_STORE1_MATCH4n_old, _BOBA_STORE1_MATCH5n_old, _COLU_STORE1_MATCH6n_old, _BAHU_STORE1_MATCH7n_old);
  /* p31.EWAM*/ wire2 _EWAM_STORE1_MATCHAp_old = nor4b(_EDYM_STORE1_MATCH0n_old, _EMYB_STORE1_MATCH1n_old, _EBEF_STORE1_MATCH2n_old, _EWOK_STORE1_MATCH3n_old);
  /* p31.CEHU*/ wire2 _CEHU_STORE2_MATCHAp_old = nor4b(_CEKO_STORE2_MATCH4n_old, _DETY_STORE2_MATCH5n_old, _DOZO_STORE2_MATCH6n_old, _CONY_STORE2_MATCH7n_old);
  /* p31.EKES*/ wire2 _EKES_STORE2_MATCHBp_old = nor4b(_FUZU_STORE2_MATCH0n_old, _FESO_STORE2_MATCH1n_old, _FOKY_STORE2_MATCH2n_old, _FYVA_STORE2_MATCH3n_old);
  /* p31.ZURE*/ wire2 _ZURE_STORE3_MATCHAp_old = nor4b(_YHOK_STORE3_MATCH0n_old, _YCAH_STORE3_MATCH1n_old, _YDAJ_STORE3_MATCH2n_old, _YVUZ_STORE3_MATCH3n_old);
  /* p31.YWOS*/ wire2 _YWOS_STORE3_MATCHBp_old = nor4b(_YVAP_STORE3_MATCH4n_old, _XENY_STORE3_MATCH5n_old, _XAVU_STORE3_MATCH6n_old, _XEVA_STORE3_MATCH7n_old);
  /* p31.YKOK*/ wire2 _YKOK_STORE4_MATCHAp_old = nor4b(_XEJU_STORE4_MATCH0n_old, _ZATE_STORE4_MATCH1n_old, _ZAKU_STORE4_MATCH2n_old, _YBOX_STORE4_MATCH3n_old);
  /* p31.YNAZ*/ wire2 _YNAZ_STORE4_MATCHBp_old = nor4b(_ZYKU_STORE4_MATCH4n_old, _ZYPU_STORE4_MATCH5n_old, _XAHA_STORE4_MATCH6n_old, _ZEFE_STORE4_MATCH7n_old);
  /* p31.COGY*/ wire2 _COGY_STORE5_MATCHAp_old = nor4b(_BAZY_STORE5_MATCH4n_old, _CYLE_STORE5_MATCH5n_old, _CEVA_STORE5_MATCH6n_old, _BUMY_STORE5_MATCH7n_old);
  /* p31.FYMA*/ wire2 _FYMA_STORE5_MATCHBp_old = nor4b(_GUZO_STORE5_MATCH0n_old, _GOLA_STORE5_MATCH1n_old, _GEVE_STORE5_MATCH2n_old, _GUDE_STORE5_MATCH3n_old);
  /* p31.YWAP*/ wire2 _YWAP_STORE6_MATCHAp_old = nor4b(_ZARE_STORE6_MATCH4n_old, _ZEMU_STORE6_MATCH5n_old, _ZYGO_STORE6_MATCH6n_old, _ZUZY_STORE6_MATCH7n_old);
  /* p31.YDOT*/ wire2 _YDOT_STORE6_MATCHBp_old = nor4b(_XOSU_STORE6_MATCH0n_old, _ZUVU_STORE6_MATCH1n_old, _XUCO_STORE6_MATCH2n_old, _ZULO_STORE6_MATCH3n_old);
  /* p31.CYCO*/ wire2 _CYCO_STORE7_MATCHAp_old = nor4b(_DUSE_STORE7_MATCH0n_old, _DAGU_STORE7_MATCH1n_old, _DYZE_STORE7_MATCH2n_old, _DESO_STORE7_MATCH3n_old);
  /* p31.DAJE*/ wire2 _DAJE_STORE7_MATCHBp_old = nor4b(_EJOT_STORE7_MATCH4n_old, _ESAJ_STORE7_MATCH5n_old, _DUCU_STORE7_MATCH6n_old, _EWUD_STORE7_MATCH7n_old);
  /* p31.DAMA*/ wire2 _DAMA_STORE8_MATCHAp_old = nor4b(_DUZE_STORE8_MATCH0n_old, _DAGA_STORE8_MATCH1n_old, _DAWU_STORE8_MATCH2n_old, _EJAW_STORE8_MATCH3n_old);
  /* p31.FEHA*/ wire2 _FEHA_STORE8_MATCHBp_old = nor4b(_GOHO_STORE8_MATCH4n_old, _GASU_STORE8_MATCH5n_old, _GABU_STORE8_MATCH6n_old, _GAFE_STORE8_MATCH7n_old);
  /* p31.YLEV*/ wire2 _YLEV_STORE9_MATCHAp_old = nor4b(_YMAM_STORE9_MATCH0n_old, _YTYP_STORE9_MATCH1n_old, _YFOP_STORE9_MATCH2n_old, _YVAC_STORE9_MATCH3n_old);
  /* p31.YTUB*/ wire2 _YTUB_STORE9_MATCHBp_old = nor4b(_ZYWU_STORE9_MATCH4n_old, _ZUZA_STORE9_MATCH5n_old, _ZEJO_STORE9_MATCH6n_old, _ZEDA_STORE9_MATCH7n_old);

  /* p29.YDUG*/ wire2 _YDUG_STORE0_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _ZAKO_STORE0_MATCHAp_old, _XEBA_STORE0_MATCHBp_old);
  /* p29.DYDU*/ wire2 _DYDU_STORE1_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _EWAM_STORE1_MATCHAp_old, _CYVY_STORE1_MATCHBp_old);
  /* p29.DEGO*/ wire2 _DEGO_STORE2_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _CEHU_STORE2_MATCHAp_old, _EKES_STORE2_MATCHBp_old);
  /* p29.YLOZ*/ wire2 _YLOZ_STORE3_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _ZURE_STORE3_MATCHAp_old, _YWOS_STORE3_MATCHBp_old);
  /* p29.XAGE*/ wire2 _XAGE_STORE4_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _YKOK_STORE4_MATCHAp_old, _YNAZ_STORE4_MATCHBp_old);
  /* p29.EGOM*/ wire2 _EGOM_STORE5_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _COGY_STORE5_MATCHAp_old, _FYMA_STORE5_MATCHBp_old);
  /* p29.YBEZ*/ wire2 _YBEZ_STORE6_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _YWAP_STORE6_MATCHAp_old, _YDOT_STORE6_MATCHBp_old);
  /* p29.DYKA*/ wire2 _DYKA_STORE7_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _CYCO_STORE7_MATCHAp_old, _DAJE_STORE7_MATCHBp_old);
  /* p29.EFYL*/ wire2 _EFYL_STORE8_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _DAMA_STORE8_MATCHAp_old, _FEHA_STORE8_MATCHBp_old);
  /* p29.YGEM*/ wire2 _YGEM_STORE9_MATCHn_old = nand3b(_AROR_MATCH_ENp_old, _YLEV_STORE9_MATCHAp_old, _YTUB_STORE9_MATCHBp_old);

  return {
    bit(_YDUG_STORE0_MATCHn_old),
    bit(_DYDU_STORE1_MATCHn_old),
    bit(_DEGO_STORE2_MATCHn_old),
    bit(_YLOZ_STORE3_MATCHn_old),
    bit(_XAGE_STORE4_MATCHn_old),
    bit(_EGOM_STORE5_MATCHn_old),
    bit(_YBEZ_STORE6_MATCHn_old),
    bit(_DYKA_STORE7_MATCHn_old),
    bit(_EFYL_STORE8_MATCHn_old),
    bit(_YGEM_STORE9_MATCHn_old)
  };
}

//------------------------------------------------------------------------------------------------------------------------

SpriteMatchFlag SpriteStore::get_match_flags_new(wire2 _AROR_MATCH_ENp_new, const PixCount& _pix_count) {
  /*#p21.ACAM*/ wire2 ACAM_PX0n_new_evn = not1b(_pix_count.XEHO_PX0p.qp_new2());
  /* p21.AZUB*/ wire2 AZUB_PX1n_new_evn = not1b(_pix_count.SAVY_PX1p.qp_new2());
  /* p21.AMEL*/ wire2 AMEL_PX2n_new_evn = not1b(_pix_count.XODU_PX2p.qp_new2());
  /* p21.AHAL*/ wire2 AHAL_PX3n_new_evn = not1b(_pix_count.XYDO_PX3p.qp_new2());
  /* p21.APUX*/ wire2 APUX_PX4n_new_evn = not1b(_pix_count.TUHU_PX4p.qp_new2());
  /* p21.ABEF*/ wire2 ABEF_PX5n_new_evn = not1b(_pix_count.TUKY_PX5p.qp_new2());
  /* p21.ADAZ*/ wire2 ADAZ_PX6n_new_evn = not1b(_pix_count.TAKO_PX6p.qp_new2());
  /* p21.ASAH*/ wire2 ASAH_PX7n_new_evn = not1b(_pix_count.SYBE_PX7p.qp_new2());

  /*#p31.ZOGY*/ wire2 _ZOGY_STORE0_MATCH0n_new = xor2(XEPE_STORE0_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.ZEBA*/ wire2 _ZEBA_STORE0_MATCH1n_new = xor2(YLAH_STORE0_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.ZAHA*/ wire2 _ZAHA_STORE0_MATCH2n_new = xor2(ZOLA_STORE0_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.ZOKY*/ wire2 _ZOKY_STORE0_MATCH3n_new = xor2(ZULU_STORE0_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.WOJU*/ wire2 _WOJU_STORE0_MATCH4n_new = xor2(WELO_STORE0_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.YFUN*/ wire2 _YFUN_STORE0_MATCH5n_new = xor2(XUNY_STORE0_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.WYZA*/ wire2 _WYZA_STORE0_MATCH6n_new = xor2(WOTE_STORE0_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.YPUK*/ wire2 _YPUK_STORE0_MATCH7n_new = xor2(XAKO_STORE0_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.EDYM*/ wire2 _EDYM_STORE1_MATCH0n_new = xor2(DANY_STORE1_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.EMYB*/ wire2 _EMYB_STORE1_MATCH1n_new = xor2(DUKO_STORE1_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.EBEF*/ wire2 _EBEF_STORE1_MATCH2n_new = xor2(DESU_STORE1_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.EWOK*/ wire2 _EWOK_STORE1_MATCH3n_new = xor2(DAZO_STORE1_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.COLA*/ wire2 _COLA_STORE1_MATCH4n_new = xor2(DAKE_STORE1_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.BOBA*/ wire2 _BOBA_STORE1_MATCH5n_new = xor2(CESO_STORE1_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.COLU*/ wire2 _COLU_STORE1_MATCH6n_new = xor2(DYFU_STORE1_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.BAHU*/ wire2 _BAHU_STORE1_MATCH7n_new = xor2(CUSY_STORE1_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.FUZU*/ wire2 _FUZU_STORE2_MATCH0n_new = xor2(FOKA_STORE2_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.FESO*/ wire2 _FESO_STORE2_MATCH1n_new = xor2(FYTY_STORE2_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.FOKY*/ wire2 _FOKY_STORE2_MATCH2n_new = xor2(FUBY_STORE2_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.FYVA*/ wire2 _FYVA_STORE2_MATCH3n_new = xor2(GOXU_STORE2_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.CEKO*/ wire2 _CEKO_STORE2_MATCH4n_new = xor2(DUHY_STORE2_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.DETY*/ wire2 _DETY_STORE2_MATCH5n_new = xor2(EJUF_STORE2_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.DOZO*/ wire2 _DOZO_STORE2_MATCH6n_new = xor2(ENOR_STORE2_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.CONY*/ wire2 _CONY_STORE2_MATCH7n_new = xor2(DEPY_STORE2_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.YHOK*/ wire2 _YHOK_STORE3_MATCH0n_new = xor2(XOLY_STORE3_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.YCAH*/ wire2 _YCAH_STORE3_MATCH1n_new = xor2(XYBA_STORE3_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.YDAJ*/ wire2 _YDAJ_STORE3_MATCH2n_new = xor2(XABE_STORE3_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.YVUZ*/ wire2 _YVUZ_STORE3_MATCH3n_new = xor2(XEKA_STORE3_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.YVAP*/ wire2 _YVAP_STORE3_MATCH4n_new = xor2(XOMY_STORE3_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.XENY*/ wire2 _XENY_STORE3_MATCH5n_new = xor2(WUHA_STORE3_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.XAVU*/ wire2 _XAVU_STORE3_MATCH6n_new = xor2(WYNA_STORE3_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.XEVA*/ wire2 _XEVA_STORE3_MATCH7n_new = xor2(WECO_STORE3_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.XEJU*/ wire2 _XEJU_STORE4_MATCH0n_new = xor2(WEDU_STORE4_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.ZATE*/ wire2 _ZATE_STORE4_MATCH1n_new = xor2(YGAJ_STORE4_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.ZAKU*/ wire2 _ZAKU_STORE4_MATCH2n_new = xor2(ZYJO_STORE4_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.YBOX*/ wire2 _YBOX_STORE4_MATCH3n_new = xor2(XURY_STORE4_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.ZYKU*/ wire2 _ZYKU_STORE4_MATCH4n_new = xor2(YBED_STORE4_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.ZYPU*/ wire2 _ZYPU_STORE4_MATCH5n_new = xor2(ZALA_STORE4_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.XAHA*/ wire2 _XAHA_STORE4_MATCH6n_new = xor2(WYDE_STORE4_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.ZEFE*/ wire2 _ZEFE_STORE4_MATCH7n_new = xor2(XEPA_STORE4_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.GUZO*/ wire2 _GUZO_STORE5_MATCH0n_new = xor2(FUSA_STORE5_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.GOLA*/ wire2 _GOLA_STORE5_MATCH1n_new = xor2(FAXA_STORE5_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.GEVE*/ wire2 _GEVE_STORE5_MATCH2n_new = xor2(FOZY_STORE5_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.GUDE*/ wire2 _GUDE_STORE5_MATCH3n_new = xor2(FESY_STORE5_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.BAZY*/ wire2 _BAZY_STORE5_MATCH4n_new = xor2(CYWE_STORE5_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.CYLE*/ wire2 _CYLE_STORE5_MATCH5n_new = xor2(DYBY_STORE5_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.CEVA*/ wire2 _CEVA_STORE5_MATCH6n_new = xor2(DURY_STORE5_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.BUMY*/ wire2 _BUMY_STORE5_MATCH7n_new = xor2(CUVY_STORE5_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.XOSU*/ wire2 _XOSU_STORE6_MATCH0n_new = xor2(YCOL_STORE6_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.ZUVU*/ wire2 _ZUVU_STORE6_MATCH1n_new = xor2(YRAC_STORE6_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.XUCO*/ wire2 _XUCO_STORE6_MATCH2n_new = xor2(YMEM_STORE6_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.ZULO*/ wire2 _ZULO_STORE6_MATCH3n_new = xor2(YVAG_STORE6_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.ZARE*/ wire2 _ZARE_STORE6_MATCH4n_new = xor2(ZOLY_STORE6_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.ZEMU*/ wire2 _ZEMU_STORE6_MATCH5n_new = xor2(ZOGO_STORE6_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.ZYGO*/ wire2 _ZYGO_STORE6_MATCH6n_new = xor2(ZECU_STORE6_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.ZUZY*/ wire2 _ZUZY_STORE6_MATCH7n_new = xor2(ZESA_STORE6_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.DUSE*/ wire2 _DUSE_STORE7_MATCH0n_new = xor2(ERAZ_STORE7_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.DAGU*/ wire2 _DAGU_STORE7_MATCH1n_new = xor2(EPUM_STORE7_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.DYZE*/ wire2 _DYZE_STORE7_MATCH2n_new = xor2(EROL_STORE7_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.DESO*/ wire2 _DESO_STORE7_MATCH3n_new = xor2(EHYN_STORE7_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.EJOT*/ wire2 _EJOT_STORE7_MATCH4n_new = xor2(FAZU_STORE7_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.ESAJ*/ wire2 _ESAJ_STORE7_MATCH5n_new = xor2(FAXE_STORE7_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.DUCU*/ wire2 _DUCU_STORE7_MATCH6n_new = xor2(EXUK_STORE7_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.EWUD*/ wire2 _EWUD_STORE7_MATCH7n_new = xor2(FEDE_STORE7_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.DUZE*/ wire2 _DUZE_STORE8_MATCH0n_new = xor2(EZUF_STORE8_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.DAGA*/ wire2 _DAGA_STORE8_MATCH1n_new = xor2(ENAD_STORE8_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.DAWU*/ wire2 _DAWU_STORE8_MATCH2n_new = xor2(EBOW_STORE8_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.EJAW*/ wire2 _EJAW_STORE8_MATCH3n_new = xor2(FYCA_STORE8_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.GOHO*/ wire2 _GOHO_STORE8_MATCH4n_new = xor2(GAVY_STORE8_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.GASU*/ wire2 _GASU_STORE8_MATCH5n_new = xor2(GYPU_STORE8_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.GABU*/ wire2 _GABU_STORE8_MATCH6n_new = xor2(GADY_STORE8_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.GAFE*/ wire2 _GAFE_STORE8_MATCH7n_new = xor2(GAZA_STORE8_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.YMAM*/ wire2 _YMAM_STORE9_MATCH0n_new = xor2(XUVY_STORE9_X0p_odd.qn_new2(), ACAM_PX0n_new_evn);
  /* p31.YTYP*/ wire2 _YTYP_STORE9_MATCH1n_new = xor2(XERE_STORE9_X1p_odd.qn_new2(), AZUB_PX1n_new_evn);
  /* p31.YFOP*/ wire2 _YFOP_STORE9_MATCH2n_new = xor2(XUZO_STORE9_X2p_odd.qn_new2(), AMEL_PX2n_new_evn);
  /* p31.YVAC*/ wire2 _YVAC_STORE9_MATCH3n_new = xor2(XEXA_STORE9_X3p_odd.qn_new2(), AHAL_PX3n_new_evn);
  /* p31.ZYWU*/ wire2 _ZYWU_STORE9_MATCH4n_new = xor2(YPOD_STORE9_X4p_odd.qn_new2(), APUX_PX4n_new_evn);
  /* p31.ZUZA*/ wire2 _ZUZA_STORE9_MATCH5n_new = xor2(YROP_STORE9_X5p_odd.qn_new2(), ABEF_PX5n_new_evn);
  /* p31.ZEJO*/ wire2 _ZEJO_STORE9_MATCH6n_new = xor2(YNEP_STORE9_X6p_odd.qn_new2(), ADAZ_PX6n_new_evn);
  /* p31.ZEDA*/ wire2 _ZEDA_STORE9_MATCH7n_new = xor2(YZOF_STORE9_X7p_odd.qn_new2(), ASAH_PX7n_new_evn);

  /* p31.ZAKO*/ wire2 _ZAKO_STORE0_MATCHAp_new = nor4b(_ZOGY_STORE0_MATCH0n_new, _ZEBA_STORE0_MATCH1n_new, _ZAHA_STORE0_MATCH2n_new, _ZOKY_STORE0_MATCH3n_new);
  /* p31.XEBA*/ wire2 _XEBA_STORE0_MATCHBp_new = nor4b(_WOJU_STORE0_MATCH4n_new, _YFUN_STORE0_MATCH5n_new, _WYZA_STORE0_MATCH6n_new, _YPUK_STORE0_MATCH7n_new);
  /* p31.CYVY*/ wire2 _CYVY_STORE1_MATCHBp_new = nor4b(_COLA_STORE1_MATCH4n_new, _BOBA_STORE1_MATCH5n_new, _COLU_STORE1_MATCH6n_new, _BAHU_STORE1_MATCH7n_new);
  /* p31.EWAM*/ wire2 _EWAM_STORE1_MATCHAp_new = nor4b(_EDYM_STORE1_MATCH0n_new, _EMYB_STORE1_MATCH1n_new, _EBEF_STORE1_MATCH2n_new, _EWOK_STORE1_MATCH3n_new);
  /* p31.CEHU*/ wire2 _CEHU_STORE2_MATCHAp_new = nor4b(_CEKO_STORE2_MATCH4n_new, _DETY_STORE2_MATCH5n_new, _DOZO_STORE2_MATCH6n_new, _CONY_STORE2_MATCH7n_new);
  /* p31.EKES*/ wire2 _EKES_STORE2_MATCHBp_new = nor4b(_FUZU_STORE2_MATCH0n_new, _FESO_STORE2_MATCH1n_new, _FOKY_STORE2_MATCH2n_new, _FYVA_STORE2_MATCH3n_new);
  /* p31.ZURE*/ wire2 _ZURE_STORE3_MATCHAp_new = nor4b(_YHOK_STORE3_MATCH0n_new, _YCAH_STORE3_MATCH1n_new, _YDAJ_STORE3_MATCH2n_new, _YVUZ_STORE3_MATCH3n_new);
  /* p31.YWOS*/ wire2 _YWOS_STORE3_MATCHBp_new = nor4b(_YVAP_STORE3_MATCH4n_new, _XENY_STORE3_MATCH5n_new, _XAVU_STORE3_MATCH6n_new, _XEVA_STORE3_MATCH7n_new);
  /* p31.YKOK*/ wire2 _YKOK_STORE4_MATCHAp_new = nor4b(_XEJU_STORE4_MATCH0n_new, _ZATE_STORE4_MATCH1n_new, _ZAKU_STORE4_MATCH2n_new, _YBOX_STORE4_MATCH3n_new);
  /* p31.YNAZ*/ wire2 _YNAZ_STORE4_MATCHBp_new = nor4b(_ZYKU_STORE4_MATCH4n_new, _ZYPU_STORE4_MATCH5n_new, _XAHA_STORE4_MATCH6n_new, _ZEFE_STORE4_MATCH7n_new);
  /* p31.COGY*/ wire2 _COGY_STORE5_MATCHAp_new = nor4b(_BAZY_STORE5_MATCH4n_new, _CYLE_STORE5_MATCH5n_new, _CEVA_STORE5_MATCH6n_new, _BUMY_STORE5_MATCH7n_new);
  /* p31.FYMA*/ wire2 _FYMA_STORE5_MATCHBp_new = nor4b(_GUZO_STORE5_MATCH0n_new, _GOLA_STORE5_MATCH1n_new, _GEVE_STORE5_MATCH2n_new, _GUDE_STORE5_MATCH3n_new);
  /* p31.YWAP*/ wire2 _YWAP_STORE6_MATCHAp_new = nor4b(_ZARE_STORE6_MATCH4n_new, _ZEMU_STORE6_MATCH5n_new, _ZYGO_STORE6_MATCH6n_new, _ZUZY_STORE6_MATCH7n_new);
  /* p31.YDOT*/ wire2 _YDOT_STORE6_MATCHBp_new = nor4b(_XOSU_STORE6_MATCH0n_new, _ZUVU_STORE6_MATCH1n_new, _XUCO_STORE6_MATCH2n_new, _ZULO_STORE6_MATCH3n_new);
  /* p31.CYCO*/ wire2 _CYCO_STORE7_MATCHAp_new = nor4b(_DUSE_STORE7_MATCH0n_new, _DAGU_STORE7_MATCH1n_new, _DYZE_STORE7_MATCH2n_new, _DESO_STORE7_MATCH3n_new);
  /* p31.DAJE*/ wire2 _DAJE_STORE7_MATCHBp_new = nor4b(_EJOT_STORE7_MATCH4n_new, _ESAJ_STORE7_MATCH5n_new, _DUCU_STORE7_MATCH6n_new, _EWUD_STORE7_MATCH7n_new);
  /* p31.DAMA*/ wire2 _DAMA_STORE8_MATCHAp_new = nor4b(_DUZE_STORE8_MATCH0n_new, _DAGA_STORE8_MATCH1n_new, _DAWU_STORE8_MATCH2n_new, _EJAW_STORE8_MATCH3n_new);
  /* p31.FEHA*/ wire2 _FEHA_STORE8_MATCHBp_new = nor4b(_GOHO_STORE8_MATCH4n_new, _GASU_STORE8_MATCH5n_new, _GABU_STORE8_MATCH6n_new, _GAFE_STORE8_MATCH7n_new);
  /* p31.YLEV*/ wire2 _YLEV_STORE9_MATCHAp_new = nor4b(_YMAM_STORE9_MATCH0n_new, _YTYP_STORE9_MATCH1n_new, _YFOP_STORE9_MATCH2n_new, _YVAC_STORE9_MATCH3n_new);
  /* p31.YTUB*/ wire2 _YTUB_STORE9_MATCHBp_new = nor4b(_ZYWU_STORE9_MATCH4n_new, _ZUZA_STORE9_MATCH5n_new, _ZEJO_STORE9_MATCH6n_new, _ZEDA_STORE9_MATCH7n_new);

  /* p29.YDUG*/ wire2 _YDUG_STORE0_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _ZAKO_STORE0_MATCHAp_new, _XEBA_STORE0_MATCHBp_new);
  /* p29.DYDU*/ wire2 _DYDU_STORE1_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _EWAM_STORE1_MATCHAp_new, _CYVY_STORE1_MATCHBp_new);
  /* p29.DEGO*/ wire2 _DEGO_STORE2_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _CEHU_STORE2_MATCHAp_new, _EKES_STORE2_MATCHBp_new);
  /* p29.YLOZ*/ wire2 _YLOZ_STORE3_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _ZURE_STORE3_MATCHAp_new, _YWOS_STORE3_MATCHBp_new);
  /* p29.XAGE*/ wire2 _XAGE_STORE4_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _YKOK_STORE4_MATCHAp_new, _YNAZ_STORE4_MATCHBp_new);
  /* p29.EGOM*/ wire2 _EGOM_STORE5_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _COGY_STORE5_MATCHAp_new, _FYMA_STORE5_MATCHBp_new);
  /* p29.YBEZ*/ wire2 _YBEZ_STORE6_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _YWAP_STORE6_MATCHAp_new, _YDOT_STORE6_MATCHBp_new);
  /* p29.DYKA*/ wire2 _DYKA_STORE7_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _CYCO_STORE7_MATCHAp_new, _DAJE_STORE7_MATCHBp_new);
  /* p29.EFYL*/ wire2 _EFYL_STORE8_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _DAMA_STORE8_MATCHAp_new, _FEHA_STORE8_MATCHBp_new);
  /* p29.YGEM*/ wire2 _YGEM_STORE9_MATCHn_new_evn = nand3b(_AROR_MATCH_ENp_new, _YLEV_STORE9_MATCHAp_new, _YTUB_STORE9_MATCHBp_new);

  return {
    bit(_YDUG_STORE0_MATCHn_new_evn),
    bit(_DYDU_STORE1_MATCHn_new_evn),
    bit(_DEGO_STORE2_MATCHn_new_evn),
    bit(_YLOZ_STORE3_MATCHn_new_evn),
    bit(_XAGE_STORE4_MATCHn_new_evn),
    bit(_EGOM_STORE5_MATCHn_new_evn),
    bit(_YBEZ_STORE6_MATCHn_new_evn),
    bit(_DYKA_STORE7_MATCHn_new_evn),
    bit(_EFYL_STORE8_MATCHn_new_evn),
    bit(_YGEM_STORE9_MATCHn_new_evn)
  };
}

//------------------------------------------------------------------------------------------------------------------------

SpriteFirstMatch SpriteStore::get_first_match(SpriteMatchFlag match_flag) const {
  wire2 GND = 0;
  /* p29.WEFU*/ wire2 _WEFU_STORE0_MATCH = not1b(match_flag.YDUG_STORE0_MATCHn);
  /* p29.GAJA*/ wire2 _GAJA_STORE1_MATCH = not1b(match_flag.DYDU_STORE1_MATCHn);
  /* p29.GUPO*/ wire2 _GUPO_STORE2_MATCH = not1b(match_flag.DEGO_STORE2_MATCHn);
  /* p29.WEBO*/ wire2 _WEBO_STORE3_MATCH = not1b(match_flag.YLOZ_STORE3_MATCHn);
  /* p29.WUNA*/ wire2 _WUNA_STORE4_MATCH = not1b(match_flag.XAGE_STORE4_MATCHn);
  /* p29.GABA*/ wire2 _GABA_STORE5_MATCH = not1b(match_flag.EGOM_STORE5_MATCHn);
  /* p29.WASE*/ wire2 _WASE_STORE6_MATCH = not1b(match_flag.YBEZ_STORE6_MATCHn);
  /* p29.GYTE*/ wire2 _GYTE_STORE7_MATCH = not1b(match_flag.DYKA_STORE7_MATCHn);
  /* p29.GEKE*/ wire2 _GEKE_STORE8_MATCH = not1b(match_flag.EFYL_STORE8_MATCHn);

  /* p29.GEZE*/ wire2 _GEZE_STORE0_MATCH = or2(_WEFU_STORE0_MATCH, GND);
  /* p29.FUMA*/ wire2 _FUMA_STORE1_MATCH = or2(_GAJA_STORE1_MATCH, _GEZE_STORE0_MATCH);
  /* p29.GEDE*/ wire2 _GEDE_STORE2_MATCH = or2(_GUPO_STORE2_MATCH, _FUMA_STORE1_MATCH);
  /* p29.WUTO*/ wire2 _WUTO_STORE3_MATCH = or2(_WEBO_STORE3_MATCH, _GEDE_STORE2_MATCH);
  /* p29.XYLA*/ wire2 _XYLA_STORE4_MATCH = or2(_WUNA_STORE4_MATCH, _WUTO_STORE3_MATCH);
  /* p29.WEJA*/ wire2 _WEJA_STORE5_MATCH = or2(_GABA_STORE5_MATCH, _XYLA_STORE4_MATCH);
  /* p29.WYLA*/ wire2 _WYLA_STORE6_MATCH = or2(_WASE_STORE6_MATCH, _WEJA_STORE5_MATCH);
  /* p29.FAVO*/ wire2 _FAVO_STORE7_MATCH = or2(_GYTE_STORE7_MATCH, _WYLA_STORE6_MATCH);
  /* p29.GYGA*/ wire2 _GYGA_STORE8_MATCH = or2(_GEKE_STORE8_MATCH, _FAVO_STORE7_MATCH);

  /* p29.GUVA*/ wire2 _GUVA_SPRITE0_GETp = nor2b(match_flag.YDUG_STORE0_MATCHn, GND);
  /* p29.ENUT*/ wire2 _ENUT_SPRITE1_GETp = nor2b(match_flag.DYDU_STORE1_MATCHn, _GEZE_STORE0_MATCH);
  /* p29.EMOL*/ wire2 _EMOL_SPRITE2_GETp = nor2b(match_flag.DEGO_STORE2_MATCHn, _FUMA_STORE1_MATCH);
  /* p29.GYFY*/ wire2 _GYFY_SPRITE3_GETp = nor2b(match_flag.YLOZ_STORE3_MATCHn, _GEDE_STORE2_MATCH);
  /* p29.GONO*/ wire2 _GONO_SPRITE4_GETp = nor2b(match_flag.XAGE_STORE4_MATCHn, _WUTO_STORE3_MATCH);
  /* p29.GEGA*/ wire2 _GEGA_SPRITE5_GETp = nor2b(match_flag.EGOM_STORE5_MATCHn, _XYLA_STORE4_MATCH);
  /* p29.XOJA*/ wire2 _XOJA_SPRITE6_GETp = nor2b(match_flag.YBEZ_STORE6_MATCHn, _WEJA_STORE5_MATCH);
  /* p29.GUTU*/ wire2 _GUTU_SPRITE7_GETp = nor2b(match_flag.DYKA_STORE7_MATCHn, _WYLA_STORE6_MATCH);
  /* p29.FOXA*/ wire2 _FOXA_SPRITE8_GETp = nor2b(match_flag.EFYL_STORE8_MATCHn, _FAVO_STORE7_MATCH);
  /* p29.GUZE*/ wire2 _GUZE_SPRITE9_GETp = nor2b(match_flag.YGEM_STORE9_MATCHn, _GYGA_STORE8_MATCH);

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

void SpriteStore::get_sprite(SpriteFirstMatch first_match, Bus SPR_TRI_I[6], Bus SPR_TRI_L[4]) {

  /* p29.FURO*/ wire2 _FURO_SPRITE0_GETn_new_evn = not1b(first_match.GUVA_SPRITE0_GETp);
  /* p29.DYDO*/ wire2 _DYDO_SPRITE1_GETn_new_evn = not1b(first_match.ENUT_SPRITE1_GETp);
  /* p29.FAME*/ wire2 _FAME_SPRITE2_GETn_new_evn = not1b(first_match.EMOL_SPRITE2_GETp);
  /* p29.GYMA*/ wire2 _GYMA_SPRITE3_GETn_new_evn = not1b(first_match.GYFY_SPRITE3_GETp);
  /* p29.GOWO*/ wire2 _GOWO_SPRITE4_GETn_new_evn = not1b(first_match.GONO_SPRITE4_GETp);
  /* p29.GYGY*/ wire2 _GYGY_SPRITE5_GETn_new_evn = not1b(first_match.GEGA_SPRITE5_GETp);
  /* p29.XYME*/ wire2 _XYME_SPRITE6_GETn_new_evn = not1b(first_match.XOJA_SPRITE6_GETp);
  /* p29.GUGY*/ wire2 _GUGY_SPRITE7_GETn_new_evn = not1b(first_match.GUTU_SPRITE7_GETp);
  /* p29.DENY*/ wire2 _DENY_SPRITE8_GETn_new_evn = not1b(first_match.FOXA_SPRITE8_GETp);
  /* p29.FADO*/ wire2 _FADO_SPRITE9_GETn_new_evn = not1b(first_match.GUZE_SPRITE9_GETp);

  /* p30.ZETU*/ SPR_TRI_I[0].tri6_nn(_FURO_SPRITE0_GETn_new_evn, YGUS_STORE0_I0n_odd.qp_new2());
  /* p30.ZECE*/ SPR_TRI_I[1].tri6_nn(_FURO_SPRITE0_GETn_new_evn, YSOK_STORE0_I1n_odd.qp_new2());
  /* p30.ZAVE*/ SPR_TRI_I[2].tri6_nn(_FURO_SPRITE0_GETn_new_evn, YZEP_STORE0_I2n_odd.qp_new2());
  /* p30.WOKO*/ SPR_TRI_I[3].tri6_nn(_FURO_SPRITE0_GETn_new_evn, WYTE_STORE0_I3n_odd.qp_new2());
  /* p30.ZUMU*/ SPR_TRI_I[4].tri6_nn(_FURO_SPRITE0_GETn_new_evn, ZONY_STORE0_I4n_odd.qp_new2());
  /*#p30.ZEDY*/ SPR_TRI_I[5].tri6_nn(_FURO_SPRITE0_GETn_new_evn, YWAK_STORE0_I5n_odd.qp_new2());
  /* p30.WEHE*/ SPR_TRI_L[0].tri6_nn(_FURO_SPRITE0_GETn_new_evn, GYHO_STORE0_L0n_odd.qp_new2());
  /* p30.BUKY*/ SPR_TRI_L[1].tri6_nn(_FURO_SPRITE0_GETn_new_evn, CUFO_STORE0_L1n_odd.qp_new2());
  /* p30.AJAL*/ SPR_TRI_L[2].tri6_nn(_FURO_SPRITE0_GETn_new_evn, BOZU_STORE0_L2n_odd.qp_new2());
  /* p30.GOFO*/ SPR_TRI_L[3].tri6_nn(_FURO_SPRITE0_GETn_new_evn, FYHY_STORE0_L3n_odd.qp_new2());

  /*#p30.CUBO*/ SPR_TRI_I[0].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CADU_STORE1_I0n_odd.qp_new2());
  /* p30.CELU*/ SPR_TRI_I[1].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CEBO_STORE1_I1n_odd.qp_new2());
  /* p30.CEGY*/ SPR_TRI_I[2].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CUFA_STORE1_I2n_odd.qp_new2());
  /* p30.BETY*/ SPR_TRI_I[3].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, COMA_STORE1_I3n_odd.qp_new2());
  /* p30.CYBY*/ SPR_TRI_I[4].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CUZA_STORE1_I4n_odd.qp_new2());
  /* p30.BEMO*/ SPR_TRI_I[5].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CAJY_STORE1_I5n_odd.qp_new2());
  /* p30.BYRO*/ SPR_TRI_L[0].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, AMES_STORE1_L0n_odd.qp_new2());
  /* p30.AHUM*/ SPR_TRI_L[1].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, AROF_STORE1_L1n_odd.qp_new2());
  /* p30.BACO*/ SPR_TRI_L[2].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, ABOP_STORE1_L2n_odd.qp_new2());
  /* p30.BEFE*/ SPR_TRI_L[3].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, ABUG_STORE1_L3n_odd.qp_new2());

  /* p30.CUBE*/ SPR_TRI_I[0].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BUHE_STORE2_I0n_odd.qp_new2());
  /* p30.AFOZ*/ SPR_TRI_I[1].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BYHU_STORE2_I1n_odd.qp_new2());
  /* p30.APON*/ SPR_TRI_I[2].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BECA_STORE2_I2n_odd.qp_new2());
  /* p30.CUVU*/ SPR_TRI_I[3].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BULU_STORE2_I3n_odd.qp_new2());
  /* p30.CYRO*/ SPR_TRI_I[4].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BUNA_STORE2_I4n_odd.qp_new2());
  /* p30.AXEC*/ SPR_TRI_I[5].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BOXA_STORE2_I5n_odd.qp_new2());
  /* p30.ZUKE*/ SPR_TRI_L[0].tri6_nn(_FAME_SPRITE2_GETn_new_evn, YLOV_STORE2_L0n_odd.qp_new2());
  /* p30.WERE*/ SPR_TRI_L[1].tri6_nn(_FAME_SPRITE2_GETn_new_evn, XOSY_STORE2_L1n_odd.qp_new2());
  /* p30.WUXE*/ SPR_TRI_L[2].tri6_nn(_FAME_SPRITE2_GETn_new_evn, XAZY_STORE2_L2n_odd.qp_new2());
  /* p30.ZABY*/ SPR_TRI_L[3].tri6_nn(_FAME_SPRITE2_GETn_new_evn, YKUK_STORE2_L3n_odd.qp_new2());

  /* p30.ENAP*/ SPR_TRI_I[0].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DEVY_STORE3_I0n_odd.qp_new2());
  /* p30.DYGO*/ SPR_TRI_I[1].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DESE_STORE3_I1n_odd.qp_new2());
  /* p30.DOWA*/ SPR_TRI_I[2].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DUNY_STORE3_I2n_odd.qp_new2());
  /* p30.DONY*/ SPR_TRI_I[3].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DUHA_STORE3_I3n_odd.qp_new2());
  /* p30.EFUD*/ SPR_TRI_I[4].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DEBA_STORE3_I4n_odd.qp_new2());
  /* p30.DEZU*/ SPR_TRI_I[5].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DAFU_STORE3_I5n_odd.qp_new2());
  /* p30.ZEXE*/ SPR_TRI_L[0].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, ZURO_STORE3_L0n_odd.qp_new2());
  /* p30.YWAV*/ SPR_TRI_L[1].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, ZYLU_STORE3_L1n_odd.qp_new2());
  /* p30.YJEM*/ SPR_TRI_L[2].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, ZENE_STORE3_L2n_odd.qp_new2());
  /* p30.ZYPO*/ SPR_TRI_L[3].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, ZURY_STORE3_L3n_odd.qp_new2());

  /* p30.WUXU*/ SPR_TRI_I[0].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XAVE_STORE4_I0n_odd.qp_new2());
  /* p30.WEPY*/ SPR_TRI_I[1].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XEFE_STORE4_I1n_odd.qp_new2());
  /* p30.WERU*/ SPR_TRI_I[2].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, WANU_STORE4_I2n_odd.qp_new2());
  /* p30.XYRE*/ SPR_TRI_I[3].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XABO_STORE4_I3n_odd.qp_new2());
  /* p30.WOXY*/ SPR_TRI_I[4].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XEGE_STORE4_I4n_odd.qp_new2());
  /* p30.WAJA*/ SPR_TRI_I[5].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XYNU_STORE4_I5n_odd.qp_new2());
  /* p30.BUCE*/ SPR_TRI_L[0].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, CAPO_STORE4_L0n_odd.qp_new2());
  /* p30.BEVY*/ SPR_TRI_L[1].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, CAJU_STORE4_L1n_odd.qp_new2());
  /* p30.BOVE*/ SPR_TRI_L[2].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, CONO_STORE4_L2n_odd.qp_new2());
  /* p30.BYDO*/ SPR_TRI_L[3].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, CUMU_STORE4_L3n_odd.qp_new2());

  /* p30.DOBO*/ SPR_TRI_I[0].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, EKOP_STORE5_I0n_odd.qp_new2());
  /* p30.DYNY*/ SPR_TRI_I[1].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ETYM_STORE5_I1n_odd.qp_new2());
  /* p30.WAGA*/ SPR_TRI_I[2].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, GORU_STORE5_I2n_odd.qp_new2());
  /* p30.DUZA*/ SPR_TRI_I[3].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, EBEX_STORE5_I3n_odd.qp_new2());
  /* p30.DALY*/ SPR_TRI_I[4].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ETAV_STORE5_I4n_odd.qp_new2());
  /* p30.DALO*/ SPR_TRI_I[5].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, EKAP_STORE5_I5n_odd.qp_new2());
  /* p30.BACE*/ SPR_TRI_L[0].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ACEP_STORE5_L0n_odd.qp_new2());
  /* p30.BUJA*/ SPR_TRI_L[1].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ABEG_STORE5_L1n_odd.qp_new2());
  /* p30.BODU*/ SPR_TRI_L[2].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ABUX_STORE5_L2n_odd.qp_new2());
  /* p30.AWAT*/ SPR_TRI_L[3].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ANED_STORE5_L3n_odd.qp_new2());

  /* p30.WATO*/ SPR_TRI_I[0].tri6_nn(_XYME_SPRITE6_GETn_new_evn, GABO_STORE6_I0n_odd.qp_new2());
  /* p30.WYWY*/ SPR_TRI_I[1].tri6_nn(_XYME_SPRITE6_GETn_new_evn, GACY_STORE6_I1n_odd.qp_new2());
  /* p30.EZOC*/ SPR_TRI_I[2].tri6_nn(_XYME_SPRITE6_GETn_new_evn, FOGO_STORE6_I2n_odd.qp_new2());
  /* p30.WABO*/ SPR_TRI_I[3].tri6_nn(_XYME_SPRITE6_GETn_new_evn, GOHU_STORE6_I3n_odd.qp_new2());
  /* p30.ELYC*/ SPR_TRI_I[4].tri6_nn(_XYME_SPRITE6_GETn_new_evn, FOXY_STORE6_I4n_odd.qp_new2());
  /* p30.WOCY*/ SPR_TRI_I[5].tri6_nn(_XYME_SPRITE6_GETn_new_evn, GECU_STORE6_I5n_odd.qp_new2());
  /* p30.YBUK*/ SPR_TRI_L[0].tri6_nn(_XYME_SPRITE6_GETn_new_evn, ZUMY_STORE6_L0n_odd.qp_new2());
  /* p30.YKOZ*/ SPR_TRI_L[1].tri6_nn(_XYME_SPRITE6_GETn_new_evn, ZAFU_STORE6_L1n_odd.qp_new2());
  /* p30.ZYTO*/ SPR_TRI_L[2].tri6_nn(_XYME_SPRITE6_GETn_new_evn, ZEXO_STORE6_L2n_odd.qp_new2());
  /* p30.ZUDO*/ SPR_TRI_L[3].tri6_nn(_XYME_SPRITE6_GETn_new_evn, ZUBE_STORE6_L3n_odd.qp_new2());

  /* p30.WAKO*/ SPR_TRI_I[0].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, GULE_STORE7_I0n_odd.qp_new2());
  /* p30.WYGO*/ SPR_TRI_I[1].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, GYNO_STORE7_I1n_odd.qp_new2());
  /* p30.ELEP*/ SPR_TRI_I[2].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, FEFA_STORE7_I2n_odd.qp_new2());
  /* p30.ETAD*/ SPR_TRI_I[3].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, FYSU_STORE7_I3n_odd.qp_new2());
  /* p30.WABA*/ SPR_TRI_I[4].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, GESY_STORE7_I4n_odd.qp_new2());
  /* p30.EVYT*/ SPR_TRI_I[5].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, FUZO_STORE7_I5n_odd.qp_new2());
  /* p30.WAXE*/ SPR_TRI_L[0].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, XYNA_STORE7_L0n_odd.qp_new2());
  /* p30.YPOZ*/ SPR_TRI_L[1].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, YGUM_STORE7_L1n_odd.qp_new2());
  /* p30.WABU*/ SPR_TRI_L[2].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, XAKU_STORE7_L2n_odd.qp_new2());
  /* p30.WANA*/ SPR_TRI_L[3].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, XYGO_STORE7_L3n_odd.qp_new2());

  /* p30.APOC*/ SPR_TRI_I[0].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AXUV_STORE8_I0n_odd.qp_new2());
  /* p30.AKYH*/ SPR_TRI_I[1].tri6_nn(_DENY_SPRITE8_GETn_new_evn, BADA_STORE8_I1n_odd.qp_new2());
  /* p30.AFEN*/ SPR_TRI_I[2].tri6_nn(_DENY_SPRITE8_GETn_new_evn, APEV_STORE8_I2n_odd.qp_new2());
  /* p30.APYV*/ SPR_TRI_I[3].tri6_nn(_DENY_SPRITE8_GETn_new_evn, BADO_STORE8_I3n_odd.qp_new2());
  /* p30.APOB*/ SPR_TRI_I[4].tri6_nn(_DENY_SPRITE8_GETn_new_evn, BEXY_STORE8_I4n_odd.qp_new2());
  /* p30.ADYB*/ SPR_TRI_I[5].tri6_nn(_DENY_SPRITE8_GETn_new_evn, BYHE_STORE8_I5n_odd.qp_new2());
  /* p30.BOSO*/ SPR_TRI_L[0].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AZAP_STORE8_L0n_odd.qp_new2());
  /* p30.BAZU*/ SPR_TRI_L[1].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AFYX_STORE8_L1n_odd.qp_new2());
  /* p30.AHAC*/ SPR_TRI_L[2].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AFUT_STORE8_L2n_odd.qp_new2());
  /* p30.BUJY*/ SPR_TRI_L[3].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AFYM_STORE8_L3n_odd.qp_new2());

  /* p30.ZARO*/ SPR_TRI_I[0].tri6_nn(_FADO_SPRITE9_GETn_new_evn, YBER_STORE9_I0n_odd.qp_new2());
  /* p30.ZOJY*/ SPR_TRI_I[1].tri6_nn(_FADO_SPRITE9_GETn_new_evn, YZOR_STORE9_I1n_odd.qp_new2());
  /* p30.YNEV*/ SPR_TRI_I[2].tri6_nn(_FADO_SPRITE9_GETn_new_evn, XYFE_STORE9_I2n_odd.qp_new2());
  /* p30.XYRA*/ SPR_TRI_I[3].tri6_nn(_FADO_SPRITE9_GETn_new_evn, XOTU_STORE9_I3n_odd.qp_new2());
  /* p30.YRAD*/ SPR_TRI_I[4].tri6_nn(_FADO_SPRITE9_GETn_new_evn, XUTE_STORE9_I4n_odd.qp_new2());
  /* p30.YHAL*/ SPR_TRI_I[5].tri6_nn(_FADO_SPRITE9_GETn_new_evn, XUFO_STORE9_I5n_odd.qp_new2());
  /* p30.BYME*/ SPR_TRI_L[0].tri6_nn(_FADO_SPRITE9_GETn_new_evn, CANA_STORE9_L0n_odd.qp_new2());
  /* p30.GATE*/ SPR_TRI_L[1].tri6_nn(_FADO_SPRITE9_GETn_new_evn, FOFO_STORE9_L1n_odd.qp_new2());
  /* p30.COHO*/ SPR_TRI_L[2].tri6_nn(_FADO_SPRITE9_GETn_new_evn, DYSY_STORE9_L2n_odd.qp_new2());
  /* p30.CAWO*/ SPR_TRI_L[3].tri6_nn(_FADO_SPRITE9_GETn_new_evn, DEWU_STORE9_L3n_odd.qp_new2());
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteStore::store_sprite_x(SpriteStoreFlag store_flag, const OamTempB& oam_temp_b, wire2 _ABAK_LINE_RSTp, wire2 WUTY_SFETCH_DONE_TRIGp, SpriteFirstMatch sprite_flag) {
  // Sprite store X resetter. This has to happen before the match check.
  /* p28.BYVA*/ wire2 _BYVA_LINE_RSTn = not1b(_ABAK_LINE_RSTp);
  /* p29.DYBA*/ wire2 _DYBA_LINE_RSTp = not1b(_BYVA_LINE_RSTn);

  /* p29.EBOJ*/ EBOJ_STORE0_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.GUVA_SPRITE0_GETp);
  /* p29.CEDY*/ CEDY_STORE1_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.ENUT_SPRITE1_GETp);
  /* p29.EGAV*/ EGAV_STORE2_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.EMOL_SPRITE2_GETp);
  /* p29.GOTA*/ GOTA_STORE3_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.GYFY_SPRITE3_GETp);
  /* p29.XUDY*/ XUDY_STORE4_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.GONO_SPRITE4_GETp);
  /* p29.WAFY*/ WAFY_STORE5_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.GEGA_SPRITE5_GETp);
  /* p29.WOMY*/ WOMY_STORE6_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.XOJA_SPRITE6_GETp);
  /* p29.WAPO*/ WAPO_STORE7_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.GUTU_SPRITE7_GETp);
  /* p29.EXUQ*/ EXUQ_STORE8_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.FOXA_SPRITE8_GETp);
  /* p29.FONO*/ FONO_STORE9_RSTp_evn.dff17(WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_flag.GUZE_SPRITE9_GETp);

  /* p29.DYWE*/ wire2 _DYWE_STORE0_RSTp_new_evn = or2(_DYBA_LINE_RSTp, EBOJ_STORE0_RSTp_evn.qp_new2());
  /* p29.EFEV*/ wire2 _EFEV_STORE1_RSTp_new_evn = or2(_DYBA_LINE_RSTp, CEDY_STORE1_RSTp_evn.qp_new2());
  /* p29.FOKO*/ wire2 _FOKO_STORE2_RSTp_new_evn = or2(_DYBA_LINE_RSTp, EGAV_STORE2_RSTp_evn.qp_new2());
  /* p29.GAKE*/ wire2 _GAKE_STORE3_RSTp_new_evn = or2(_DYBA_LINE_RSTp, GOTA_STORE3_RSTp_evn.qp_new2());
  /* p29.WOHU*/ wire2 _WOHU_STORE4_RSTp_new_evn = or2(_DYBA_LINE_RSTp, XUDY_STORE4_RSTp_evn.qp_new2());
  /* p29.FEVE*/ wire2 _FEVE_STORE5_RSTp_new_evn = or2(_DYBA_LINE_RSTp, WAFY_STORE5_RSTp_evn.qp_new2());
  /* p29.WACY*/ wire2 _WACY_STORE6_RSTp_new_evn = or2(_DYBA_LINE_RSTp, WOMY_STORE6_RSTp_evn.qp_new2());
  /* p29.GUKY*/ wire2 _GUKY_STORE7_RSTp_new_evn = or2(_DYBA_LINE_RSTp, WAPO_STORE7_RSTp_evn.qp_new2());
  /* p29.GORO*/ wire2 _GORO_STORE8_RSTp_new_evn = or2(_DYBA_LINE_RSTp, EXUQ_STORE8_RSTp_evn.qp_new2());
  /* p29.DUBU*/ wire2 _DUBU_STORE9_RSTp_new_evn = or2(_DYBA_LINE_RSTp, FONO_STORE9_RSTp_evn.qp_new2());

  /* p29.DYNA*/ wire2 _DYNA_STORE0_RSTn_new_evn = not1b(_DYWE_STORE0_RSTp_new_evn);
  /* p29.DOKU*/ wire2 _DOKU_STORE1_RSTn_new_evn = not1b(_EFEV_STORE1_RSTp_new_evn);
  /* p29.GAMY*/ wire2 _GAMY_STORE2_RSTn_new_evn = not1b(_FOKO_STORE2_RSTp_new_evn);
  /* p29.WUPA*/ wire2 _WUPA_STORE3_RSTn_new_evn = not1b(_GAKE_STORE3_RSTp_new_evn);
  /* p29.WUNU*/ wire2 _WUNU_STORE4_RSTn_new_evn = not1b(_WOHU_STORE4_RSTp_new_evn);
  /* p29.EJAD*/ wire2 _EJAD_STORE5_RSTn_new_evn = not1b(_FEVE_STORE5_RSTp_new_evn);
  /* p29.XAHO*/ wire2 _XAHO_STORE6_RSTn_new_evn = not1b(_WACY_STORE6_RSTp_new_evn);
  /* p29.GAFY*/ wire2 _GAFY_STORE7_RSTn_new_evn = not1b(_GUKY_STORE7_RSTp_new_evn);
  /* p29.WUZO*/ wire2 _WUZO_STORE8_RSTn_new_evn = not1b(_GORO_STORE8_RSTp_new_evn);
  /* p29.DOSY*/ wire2 _DOSY_STORE9_RSTn_new_evn = not1b(_DUBU_STORE9_RSTp_new_evn);


  /* p29.FUXU*/ wire2 _FUXU_STORE0_clk = not1b(store_flag.DYHU_STORE0_CLKn);
  /* p29.ASYS*/ wire2 _ASYS_STORE1_clk = not1b(store_flag.BUCO_STORE1_CLKn);
  /* p29.CACU*/ wire2 _CACU_STORE2_clk = not1b(store_flag.GYFO_STORE2_CLKn);
  /* p29.YFAG*/ wire2 _YFAG_STORE3_clk = not1b(store_flag.GUSA_STORE3_CLKn);
  /* p29.WOFO*/ wire2 _WOFO_STORE4_clk = not1b(store_flag.DUKE_STORE4_CLKn);
  /* p29.CYLA*/ wire2 _CYLA_STORE5_clk = not1b(store_flag.BEDE_STORE5_CLKn);
  /* p29.ZAPE*/ wire2 _ZAPE_STORE6_clk = not1b(store_flag.WEKA_STORE6_CLKn);
  /* p29.GECY*/ wire2 _GECY_STORE7_clk = not1b(store_flag.GYVO_STORE7_CLKn);
  /* p29.CEXU*/ wire2 _CEXU_STORE8_clk = not1b(store_flag.BUKA_STORE8_CLKn);
  /* p29.WEME*/ wire2 _WEME_STORE9_clk = not1b(store_flag.DECU_STORE9_CLKn);

  /*#p31.ZAGO*/ wire2 _ZAGO_SPX0n_new_any = not1b(oam_temp_b.YLOR_OAM_DB0p.qp_old2());
  /* p31.ZOCY*/ wire2 _ZOCY_SPX1n_new_any = not1b(oam_temp_b.ZYTY_OAM_DB1p.qp_old2());
  /* p31.YPUR*/ wire2 _YPUR_SPX2n_new_any = not1b(oam_temp_b.ZYVE_OAM_DB2p.qp_old2());
  /* p31.YVOK*/ wire2 _YVOK_SPX3n_new_any = not1b(oam_temp_b.ZEZY_OAM_DB3p.qp_old2());
  /* p31.COSE*/ wire2 _COSE_SPX4n_new_any = not1b(oam_temp_b.GOMO_OAM_DB4p.qp_old2());
  /* p31.AROP*/ wire2 _AROP_SPX5n_new_any = not1b(oam_temp_b.BAXO_OAM_DB5p.qp_old2());
  /* p31.XATU*/ wire2 _XATU_SPX6n_new_any = not1b(oam_temp_b.YZOS_OAM_DB6p.qp_old2());
  /* p31.BADY*/ wire2 _BADY_SPX7n_new_any = not1b(oam_temp_b.DEPO_OAM_DB7p.qp_old2());

  /*#p31.XEPE*/ XEPE_STORE0_X0p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.YLAH*/ YLAH_STORE0_X1p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.ZOLA*/ ZOLA_STORE0_X2p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.ZULU*/ ZULU_STORE0_X3p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.WELO*/ WELO_STORE0_X4p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.XUNY*/ XUNY_STORE0_X5p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.WOTE*/ WOTE_STORE0_X6p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.XAKO*/ XAKO_STORE0_X7p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn_new_evn, _BADY_SPX7n_new_any);

  /* p31.DANY*/ DANY_STORE1_X0p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.DUKO*/ DUKO_STORE1_X1p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.DESU*/ DESU_STORE1_X2p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.DAZO*/ DAZO_STORE1_X3p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.DAKE*/ DAKE_STORE1_X4p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.CESO*/ CESO_STORE1_X5p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.DYFU*/ DYFU_STORE1_X6p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.CUSY*/ CUSY_STORE1_X7p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn_new_evn, _BADY_SPX7n_new_any);

  /* p31.FOKA*/ FOKA_STORE2_X0p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.FYTY*/ FYTY_STORE2_X1p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.FUBY*/ FUBY_STORE2_X2p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.GOXU*/ GOXU_STORE2_X3p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.DUHY*/ DUHY_STORE2_X4p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.EJUF*/ EJUF_STORE2_X5p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.ENOR*/ ENOR_STORE2_X6p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.DEPY*/ DEPY_STORE2_X7p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn_new_evn, _BADY_SPX7n_new_any);

  /* p31.XOLY*/ XOLY_STORE3_X0p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.XYBA*/ XYBA_STORE3_X1p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.XABE*/ XABE_STORE3_X2p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.XEKA*/ XEKA_STORE3_X3p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.XOMY*/ XOMY_STORE3_X4p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.WUHA*/ WUHA_STORE3_X5p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.WYNA*/ WYNA_STORE3_X6p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.WECO*/ WECO_STORE3_X7p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn_new_evn, _BADY_SPX7n_new_any);

  /* p31.WEDU*/ WEDU_STORE4_X0p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.YGAJ*/ YGAJ_STORE4_X1p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.ZYJO*/ ZYJO_STORE4_X2p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.XURY*/ XURY_STORE4_X3p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.YBED*/ YBED_STORE4_X4p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.ZALA*/ ZALA_STORE4_X5p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.WYDE*/ WYDE_STORE4_X6p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.XEPA*/ XEPA_STORE4_X7p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn_new_evn, _BADY_SPX7n_new_any);

  /* p31.FUSA*/ FUSA_STORE5_X0p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.FAXA*/ FAXA_STORE5_X1p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.FOZY*/ FOZY_STORE5_X2p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.FESY*/ FESY_STORE5_X3p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.CYWE*/ CYWE_STORE5_X4p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.DYBY*/ DYBY_STORE5_X5p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.DURY*/ DURY_STORE5_X6p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.CUVY*/ CUVY_STORE5_X7p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn_new_evn, _BADY_SPX7n_new_any);

  /* p31.YCOL*/ YCOL_STORE6_X0p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.YRAC*/ YRAC_STORE6_X1p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.YMEM*/ YMEM_STORE6_X2p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.YVAG*/ YVAG_STORE6_X3p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.ZOLY*/ ZOLY_STORE6_X4p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.ZOGO*/ ZOGO_STORE6_X5p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.ZECU*/ ZECU_STORE6_X6p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.ZESA*/ ZESA_STORE6_X7p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn_new_evn, _BADY_SPX7n_new_any);

  /* p31.ERAZ*/ ERAZ_STORE7_X0p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.EPUM*/ EPUM_STORE7_X1p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.EROL*/ EROL_STORE7_X2p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.EHYN*/ EHYN_STORE7_X3p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.FAZU*/ FAZU_STORE7_X4p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.FAXE*/ FAXE_STORE7_X5p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.EXUK*/ EXUK_STORE7_X6p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.FEDE*/ FEDE_STORE7_X7p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn_new_evn, _BADY_SPX7n_new_any);

  /* p31.EZUF*/ EZUF_STORE8_X0p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.ENAD*/ ENAD_STORE8_X1p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.EBOW*/ EBOW_STORE8_X2p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.FYCA*/ FYCA_STORE8_X3p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.GAVY*/ GAVY_STORE8_X4p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.GYPU*/ GYPU_STORE8_X5p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.GADY*/ GADY_STORE8_X6p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.GAZA*/ GAZA_STORE8_X7p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn_new_evn, _BADY_SPX7n_new_any);

  /* p31.XUVY*/ XUVY_STORE9_X0p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn_new_evn, _ZAGO_SPX0n_new_any);
  /* p31.XERE*/ XERE_STORE9_X1p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn_new_evn, _ZOCY_SPX1n_new_any);
  /* p31.XUZO*/ XUZO_STORE9_X2p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn_new_evn, _YPUR_SPX2n_new_any);
  /* p31.XEXA*/ XEXA_STORE9_X3p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn_new_evn, _YVOK_SPX3n_new_any);
  /* p31.YPOD*/ YPOD_STORE9_X4p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn_new_evn, _COSE_SPX4n_new_any);
  /* p31.YROP*/ YROP_STORE9_X5p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn_new_evn, _AROP_SPX5n_new_any);
  /* p31.YNEP*/ YNEP_STORE9_X6p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn_new_evn, _XATU_SPX6n_new_any);
  /* p31.YZOF*/ YZOF_STORE9_X7p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn_new_evn, _BADY_SPX7n_new_any);
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteStore::store_sprite_index(SpriteStoreFlag store_flag, Bus SPR_TRI_I[6]) {
  /* p29.GENY*/ wire2 _GENY_STORE0_CLKp_new_odd = not1b(store_flag.DYHU_STORE0_CLKn);
  /* p29.BYVY*/ wire2 _BYVY_STORE1_CLKp_new_odd = not1b(store_flag.BUCO_STORE1_CLKn);
  /* p29.BUZY*/ wire2 _BUZY_STORE2_CLKp_new_odd = not1b(store_flag.GYFO_STORE2_CLKn);
  /* p29.FEKA*/ wire2 _FEKA_STORE3_CLKp_new_odd = not1b(store_flag.GUSA_STORE3_CLKn);
  /* p29.WYLU*/ wire2 _WYLU_STORE4_CLKp_new_odd = not1b(store_flag.DUKE_STORE4_CLKn);
  /* p29.DYMO*/ wire2 _DYMO_STORE5_CLKp_new_odd = not1b(store_flag.BEDE_STORE5_CLKn);
  /* p29.WUSE*/ wire2 _WUSE_STORE6_CLKp_new_odd = not1b(store_flag.WEKA_STORE6_CLKn);
  /* p29.FEFO*/ wire2 _FEFO_STORE7_CLKp_new_odd = not1b(store_flag.GYVO_STORE7_CLKn);
  /* p29.AKOL*/ wire2 _AKOL_STORE8_CLKp_new_odd = not1b(store_flag.BUKA_STORE8_CLKn);
  /* p29.WUFA*/ wire2 _WUFA_STORE9_CLKp_new_odd = not1b(store_flag.DECU_STORE9_CLKn);

  /* p30.YGUS*/ YGUS_STORE0_I0n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.YSOK*/ YSOK_STORE0_I1n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.YZEP*/ YZEP_STORE0_I2n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.WYTE*/ WYTE_STORE0_I3n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.ZONY*/ ZONY_STORE0_I4n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.YWAK*/ YWAK_STORE0_I5n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[5].qp_old2());

  /* p30.CADU*/ CADU_STORE1_I0n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.CEBO*/ CEBO_STORE1_I1n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.CUFA*/ CUFA_STORE1_I2n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.COMA*/ COMA_STORE1_I3n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.CUZA*/ CUZA_STORE1_I4n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.CAJY*/ CAJY_STORE1_I5n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[5].qp_old2());

  /* p30.BUHE*/ BUHE_STORE2_I0n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.BYHU*/ BYHU_STORE2_I1n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.BECA*/ BECA_STORE2_I2n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.BULU*/ BULU_STORE2_I3n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.BUNA*/ BUNA_STORE2_I4n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.BOXA*/ BOXA_STORE2_I5n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[5].qp_old2());

  /* p30.DEVY*/ DEVY_STORE3_I0n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.DESE*/ DESE_STORE3_I1n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.DUNY*/ DUNY_STORE3_I2n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.DUHA*/ DUHA_STORE3_I3n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.DEBA*/ DEBA_STORE3_I4n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.DAFU*/ DAFU_STORE3_I5n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[5].qp_old2());

  /* p30.XAVE*/ XAVE_STORE4_I0n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.XEFE*/ XEFE_STORE4_I1n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.WANU*/ WANU_STORE4_I2n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.XABO*/ XABO_STORE4_I3n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.XEGE*/ XEGE_STORE4_I4n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.XYNU*/ XYNU_STORE4_I5n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[5].qp_old2());

  /* p30.EKOP*/ EKOP_STORE5_I0n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.ETYM*/ ETYM_STORE5_I1n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.GORU*/ GORU_STORE5_I2n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.EBEX*/ EBEX_STORE5_I3n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.ETAV*/ ETAV_STORE5_I4n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.EKAP*/ EKAP_STORE5_I5n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[5].qp_old2());

  /* p30.GABO*/ GABO_STORE6_I0n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.GACY*/ GACY_STORE6_I1n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.FOGO*/ FOGO_STORE6_I2n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.GOHU*/ GOHU_STORE6_I3n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.FOXY*/ FOXY_STORE6_I4n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.GECU*/ GECU_STORE6_I5n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[5].qp_old2());

  /* p30.GULE*/ GULE_STORE7_I0n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.GYNO*/ GYNO_STORE7_I1n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.FEFA*/ FEFA_STORE7_I2n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.FYSU*/ FYSU_STORE7_I3n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.GESY*/ GESY_STORE7_I4n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.FUZO*/ FUZO_STORE7_I5n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[5].qp_old2());

  /* p30.AXUV*/ AXUV_STORE8_I0n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.BADA*/ BADA_STORE8_I1n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.APEV*/ APEV_STORE8_I2n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.BADO*/ BADO_STORE8_I3n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.BEXY*/ BEXY_STORE8_I4n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.BYHE*/ BYHE_STORE8_I5n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[5].qp_old2());

  /* p30.YBER*/ YBER_STORE9_I0n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[0].qp_old2());
  /* p30.YZOR*/ YZOR_STORE9_I1n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[1].qp_old2());
  /* p30.XYFE*/ XYFE_STORE9_I2n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[2].qp_old2());
  /* p30.XOTU*/ XOTU_STORE9_I3n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[3].qp_old2());
  /* p30.XUTE*/ XUTE_STORE9_I4n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[4].qp_old2());
  /* p30.XUFO*/ XUFO_STORE9_I5n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[5].qp_old2());
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteStore::store_sprite_line(SpriteStoreFlag store_flag, Bus SPR_TRI_L[4]) {
  /* p29.ENOB*/ wire2 _ENOB_STORE0_CLKp_new_odd = not1b(store_flag.DYHU_STORE0_CLKn);
  /* p29.AHOF*/ wire2 _AHOF_STORE1_CLKp_new_odd = not1b(store_flag.BUCO_STORE1_CLKn);
  /* p29.FUKE*/ wire2 _FUKE_STORE2_CLKp_new_odd = not1b(store_flag.GYFO_STORE2_CLKn);
  /* p29.XYHA*/ wire2 _XYHA_STORE3_CLKp_new_odd = not1b(store_flag.GUSA_STORE3_CLKn);
  /* p29.EWOT*/ wire2 _EWOT_STORE4_CLKp_new_odd = not1b(store_flag.DUKE_STORE4_CLKn);
  /* p29.BUCY*/ wire2 _BUCY_STORE5_CLKp_new_odd = not1b(store_flag.BEDE_STORE5_CLKn);
  /* p29.ZURU*/ wire2 _ZURU_STORE6_CLKp_new_odd = not1b(store_flag.WEKA_STORE6_CLKn);
  /* p29.WABE*/ wire2 _WABE_STORE7_CLKp_new_odd = not1b(store_flag.GYVO_STORE7_CLKn);
  /* p29.BYMY*/ wire2 _BYMY_STORE8_CLKp_new_odd = not1b(store_flag.BUKA_STORE8_CLKn);
  /* p29.FAKA*/ wire2 _FAKA_STORE9_CLKp_new_odd = not1b(store_flag.DECU_STORE9_CLKn);

  /* p30.GYHO*/ GYHO_STORE0_L0n_odd.dff8n(_ENOB_STORE0_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.CUFO*/ CUFO_STORE0_L1n_odd.dff8n(_ENOB_STORE0_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.BOZU*/ BOZU_STORE0_L2n_odd.dff8n(_ENOB_STORE0_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.FYHY*/ FYHY_STORE0_L3n_odd.dff8n(_ENOB_STORE0_CLKp_new_odd, SPR_TRI_L[3].qp_old2());

  /* p30.AMES*/ AMES_STORE1_L0n_odd.dff8n(_AHOF_STORE1_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.AROF*/ AROF_STORE1_L1n_odd.dff8n(_AHOF_STORE1_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.ABOP*/ ABOP_STORE1_L2n_odd.dff8n(_AHOF_STORE1_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.ABUG*/ ABUG_STORE1_L3n_odd.dff8n(_AHOF_STORE1_CLKp_new_odd, SPR_TRI_L[3].qp_old2());

  /* p30.YLOV*/ YLOV_STORE2_L0n_odd.dff8n(_FUKE_STORE2_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.XOSY*/ XOSY_STORE2_L1n_odd.dff8n(_FUKE_STORE2_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.XAZY*/ XAZY_STORE2_L2n_odd.dff8n(_FUKE_STORE2_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.YKUK*/ YKUK_STORE2_L3n_odd.dff8n(_FUKE_STORE2_CLKp_new_odd, SPR_TRI_L[3].qp_old2());

  /* p30.ZURO*/ ZURO_STORE3_L0n_odd.dff8n(_XYHA_STORE3_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.ZYLU*/ ZYLU_STORE3_L1n_odd.dff8n(_XYHA_STORE3_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.ZENE*/ ZENE_STORE3_L2n_odd.dff8n(_XYHA_STORE3_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.ZURY*/ ZURY_STORE3_L3n_odd.dff8n(_XYHA_STORE3_CLKp_new_odd, SPR_TRI_L[3].qp_old2());

  /* p30.CAPO*/ CAPO_STORE4_L0n_odd.dff8n(_EWOT_STORE4_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.CAJU*/ CAJU_STORE4_L1n_odd.dff8n(_EWOT_STORE4_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.CONO*/ CONO_STORE4_L2n_odd.dff8n(_EWOT_STORE4_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.CUMU*/ CUMU_STORE4_L3n_odd.dff8n(_EWOT_STORE4_CLKp_new_odd, SPR_TRI_L[3].qp_old2());

  /* p30.ACEP*/ ACEP_STORE5_L0n_odd.dff8n(_BUCY_STORE5_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.ABEG*/ ABEG_STORE5_L1n_odd.dff8n(_BUCY_STORE5_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.ABUX*/ ABUX_STORE5_L2n_odd.dff8n(_BUCY_STORE5_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.ANED*/ ANED_STORE5_L3n_odd.dff8n(_BUCY_STORE5_CLKp_new_odd, SPR_TRI_L[3].qp_old2());

  /* p30.ZUMY*/ ZUMY_STORE6_L0n_odd.dff8n(_ZURU_STORE6_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.ZAFU*/ ZAFU_STORE6_L1n_odd.dff8n(_ZURU_STORE6_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.ZEXO*/ ZEXO_STORE6_L2n_odd.dff8n(_ZURU_STORE6_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.ZUBE*/ ZUBE_STORE6_L3n_odd.dff8n(_ZURU_STORE6_CLKp_new_odd, SPR_TRI_L[3].qp_old2());

  /* p30.XYNA*/ XYNA_STORE7_L0n_odd.dff8n(_WABE_STORE7_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.YGUM*/ YGUM_STORE7_L1n_odd.dff8n(_WABE_STORE7_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.XAKU*/ XAKU_STORE7_L2n_odd.dff8n(_WABE_STORE7_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.XYGO*/ XYGO_STORE7_L3n_odd.dff8n(_WABE_STORE7_CLKp_new_odd, SPR_TRI_L[3].qp_old2());

  /* p30.AZAP*/ AZAP_STORE8_L0n_odd.dff8n(_BYMY_STORE8_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.AFYX*/ AFYX_STORE8_L1n_odd.dff8n(_BYMY_STORE8_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.AFUT*/ AFUT_STORE8_L2n_odd.dff8n(_BYMY_STORE8_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.AFYM*/ AFYM_STORE8_L3n_odd.dff8n(_BYMY_STORE8_CLKp_new_odd, SPR_TRI_L[3].qp_old2());

  /* p30.CANA*/ CANA_STORE9_L0n_odd.dff8n(_FAKA_STORE9_CLKp_new_odd, SPR_TRI_L[0].qp_old2());
  /* p30.FOFO*/ FOFO_STORE9_L1n_odd.dff8n(_FAKA_STORE9_CLKp_new_odd, SPR_TRI_L[1].qp_old2());
  /* p30.DYSY*/ DYSY_STORE9_L2n_odd.dff8n(_FAKA_STORE9_CLKp_new_odd, SPR_TRI_L[2].qp_old2());
  /* p30.DEWU*/ DEWU_STORE9_L3n_odd.dff8n(_FAKA_STORE9_CLKp_new_odd, SPR_TRI_L[3].qp_old2());
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteStore::oam_addr_to_sprite_index(wire2 WUDA_xxCDxxGH, wire2 XYMU_RENDERINGp, wire2 CENO_SCANNINGn, Bus BUS_OAM_An[8], Bus SPR_TRI_I[6]) {
  wire2 VCC = 1;
  /* p28.YFOT*/ wire2 _YFOT_OAM_A2p_old = not1b(BUS_OAM_An[2].qp_old2());
  /* p28.YFOC*/ wire2 _YFOC_OAM_A3p_old = not1b(BUS_OAM_An[3].qp_old2());
  /* p28.YVOM*/ wire2 _YVOM_OAM_A4p_old = not1b(BUS_OAM_An[4].qp_old2());
  /* p28.YMEV*/ wire2 _YMEV_OAM_A5p_old = not1b(BUS_OAM_An[5].qp_old2());
  /* p28.XEMU*/ wire2 _XEMU_OAM_A6p_old = not1b(BUS_OAM_An[6].qp_old2());
  /* p28.YZET*/ wire2 _YZET_OAM_A7p_old = not1b(BUS_OAM_An[7].qp_old2());

  // Sprite store grabs the sprite index off the _old_ oam address bus
  /* p30.XADU*/ XADU_SPRITE_IDX0p.dff13(WUDA_xxCDxxGH, VCC, _YFOT_OAM_A2p_old);
  /* p30.XEDY*/ XEDY_SPRITE_IDX1p.dff13(WUDA_xxCDxxGH, VCC, _YFOC_OAM_A3p_old);
  /* p30.ZUZE*/ ZUZE_SPRITE_IDX2p.dff13(WUDA_xxCDxxGH, VCC, _YVOM_OAM_A4p_old);
  /* p30.XOBE*/ XOBE_SPRITE_IDX3p.dff13(WUDA_xxCDxxGH, VCC, _YMEV_OAM_A5p_old);
  /* p30.YDUF*/ YDUF_SPRITE_IDX4p.dff13(WUDA_xxCDxxGH, VCC, _XEMU_OAM_A6p_old);
  /* p30.XECU*/ XECU_SPRITE_IDX5p.dff13(WUDA_xxCDxxGH, VCC, _YZET_OAM_A7p_old);

  /*#p29.BUZA*/ wire2 _BUZA_STORE_SPRITE_INDXn_new = and2(CENO_SCANNINGn, XYMU_RENDERINGp);
  /*#p30.WUZY*/ SPR_TRI_I[0].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XADU_SPRITE_IDX0p.qn_new2());
  /* p30.WYSE*/ SPR_TRI_I[1].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XEDY_SPRITE_IDX1p.qn_new2());
  /* p30.ZYSU*/ SPR_TRI_I[2].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, ZUZE_SPRITE_IDX2p.qn_new2());
  /* p30.WYDA*/ SPR_TRI_I[3].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XOBE_SPRITE_IDX3p.qn_new2());
  /* p30.WUCO*/ SPR_TRI_I[4].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, YDUF_SPRITE_IDX4p.qn_new2());
  /* p30.WEZA*/ SPR_TRI_I[5].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XECU_SPRITE_IDX5p.qn_new2());
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteStore::ly_to_sprite_line(wire2 FEPO_STORE_MATCHp_new_evn, const RegLY& reg_ly, const OamTempA& oam_temp_a, Bus SPR_TRI_L[4]) {
  wire2 GND = 0;

  /*#p29.EBOS*/ wire2 EBOS_LY0n_new_evn = not1b(reg_ly.MUWY_LY0p.qp_new2());
  /* p29.DASA*/ wire2 DASA_LY1n_new_evn = not1b(reg_ly.MYRO_LY1p.qp_new2());
  /* p29.FUKY*/ wire2 FUKY_LY2n_new_evn = not1b(reg_ly.LEXA_LY2p.qp_new2());
  /* p29.FUVE*/ wire2 FUVE_LY3n_new_evn = not1b(reg_ly.LYDO_LY3p.qp_new2());

  /* p29.ERUC*/ wire2 _ERUC_YDIFF_S0_new = add_s(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p.qp_new2(), GND);
  /* p29.ERUC*/ wire2 _ERUC_YDIFF_C0_new = add_c(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p.qp_new2(), GND);
  /* p29.ENEF*/ wire2 _ENEF_YDIFF_S1_new = add_s(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p.qp_new2(), _ERUC_YDIFF_C0_new);
  /* p29.ENEF*/ wire2 _ENEF_YDIFF_C1_new = add_c(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p.qp_new2(), _ERUC_YDIFF_C0_new);
  /* p29.FECO*/ wire2 _FECO_YDIFF_S2_new = add_s(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p.qp_new2(), _ENEF_YDIFF_C1_new);
  /* p29.FECO*/ wire2 _FECO_YDIFF_C2_new = add_c(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p.qp_new2(), _ENEF_YDIFF_C1_new);
  /* p29.GYKY*/ wire2 _GYKY_YDIFF_S3_new = add_s(FUVE_LY3n_new_evn, oam_temp_a.XYJU_OAM_DA3p.qp_new2(), _FECO_YDIFF_C2_new);

  /* p29.DEGE*/ wire2 _DEGE_SPRITE_DELTA0_new = not1b(_ERUC_YDIFF_S0_new);
  /* p29.DABY*/ wire2 _DABY_SPRITE_DELTA1_new = not1b(_ENEF_YDIFF_S1_new);
  /* p29.DABU*/ wire2 _DABU_SPRITE_DELTA2_new = not1b(_FECO_YDIFF_S2_new);
  /* p29.GYSA*/ wire2 _GYSA_SPRITE_DELTA3_new = not1b(_GYKY_YDIFF_S3_new);

  /*#p30.CUCU*/ SPR_TRI_L[0].tri6_nn(FEPO_STORE_MATCHp_new_evn, _DEGE_SPRITE_DELTA0_new);
  /*#p30.CUCA*/ SPR_TRI_L[1].tri6_nn(FEPO_STORE_MATCHp_new_evn, _DABY_SPRITE_DELTA1_new);
  /*#p30.CEGA*/ SPR_TRI_L[2].tri6_nn(FEPO_STORE_MATCHp_new_evn, _DABU_SPRITE_DELTA2_new);
  /*#p30.WENU*/ SPR_TRI_L[3].tri6_nn(FEPO_STORE_MATCHp_new_evn, _GYSA_SPRITE_DELTA3_new);
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteStore::dump(Dumper& d) {
  d.dump_slice2p("SPRITE INDEX   : ", &XADU_SPRITE_IDX0p.state, 6);
  d.dump_slice2p("SPRITE COUNT   : ", &BESE_SPRITE_COUNT0, 4);
  d.dump_bitp   ("DEZY_STORE_ENn : ", DEZY_COUNT_CLKp.state);
  d("\n");
  d             ("STORE0 R%d I%02d L%02d X%03d\n", EBOJ_STORE0_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &YGUS_STORE0_I0n_odd), BitBase::pack_oldn(4, &GYHO_STORE0_L0n_odd), BitBase::pack_oldn(8, &XEPE_STORE0_X0p_odd));
  d             ("STORE1 R%d I%02d L%02d X%03d\n", CEDY_STORE1_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &CADU_STORE1_I0n_odd), BitBase::pack_oldn(4, &AMES_STORE1_L0n_odd), BitBase::pack_oldn(8, &DANY_STORE1_X0p_odd));
  d             ("STORE2 R%d I%02d L%02d X%03d\n", EGAV_STORE2_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &BUHE_STORE2_I0n_odd), BitBase::pack_oldn(4, &YLOV_STORE2_L0n_odd), BitBase::pack_oldn(8, &FOKA_STORE2_X0p_odd));
  d             ("STORE3 R%d I%02d L%02d X%03d\n", GOTA_STORE3_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &DEVY_STORE3_I0n_odd), BitBase::pack_oldn(4, &ZURO_STORE3_L0n_odd), BitBase::pack_oldn(8, &XOLY_STORE3_X0p_odd));
  d             ("STORE4 R%d I%02d L%02d X%03d\n", XUDY_STORE4_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &XAVE_STORE4_I0n_odd), BitBase::pack_oldn(4, &CAPO_STORE4_L0n_odd), BitBase::pack_oldn(8, &WEDU_STORE4_X0p_odd));
  d             ("STORE5 R%d I%02d L%02d X%03d\n", WAFY_STORE5_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &EKOP_STORE5_I0n_odd), BitBase::pack_oldn(4, &ACEP_STORE5_L0n_odd), BitBase::pack_oldn(8, &FUSA_STORE5_X0p_odd));
  d             ("STORE6 R%d I%02d L%02d X%03d\n", WOMY_STORE6_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &GABO_STORE6_I0n_odd), BitBase::pack_oldn(4, &ZUMY_STORE6_L0n_odd), BitBase::pack_oldn(8, &YCOL_STORE6_X0p_odd));
  d             ("STORE7 R%d I%02d L%02d X%03d\n", WAPO_STORE7_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &GULE_STORE7_I0n_odd), BitBase::pack_oldn(4, &XYNA_STORE7_L0n_odd), BitBase::pack_oldn(8, &ERAZ_STORE7_X0p_odd));
  d             ("STORE8 R%d I%02d L%02d X%03d\n", EXUQ_STORE8_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &AXUV_STORE8_I0n_odd), BitBase::pack_oldn(4, &AZAP_STORE8_L0n_odd), BitBase::pack_oldn(8, &EZUF_STORE8_X0p_odd));
  d             ("STORE9 R%d I%02d L%02d X%03d\n", FONO_STORE9_RSTp_evn.qp_old2(), BitBase::pack_oldn(6, &YBER_STORE9_I0n_odd), BitBase::pack_oldn(4, &CANA_STORE9_L0n_odd), BitBase::pack_oldn(8, &XUVY_STORE9_X0p_odd));
}

//------------------------------------------------------------------------------------------------------------------------
