#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoyPixPipe.h"
#include "GateBoyLib/GateBoyOamBus.h"
#include "GateBoyLib/GateBoyLCD.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoySpriteStore::reset_to_cart() {
  BUS_SPR_I0.reset(0);
  BUS_SPR_I1.reset(0);
  BUS_SPR_I2.reset(1);
  BUS_SPR_I3.reset(0);
  BUS_SPR_I4.reset(1);
  BUS_SPR_I5.reset(0);

  BUS_SPR_L0.reset(1);
  BUS_SPR_L1.reset(1);
  BUS_SPR_L2.reset(1);
  BUS_SPR_L3.reset(1);

  DEZY_COUNT_CLKp.reset(1, 1);
  BESE_SPRITE_COUNT0.reset(1, 0);
  CUXY_SPRITE_COUNT1.reset(1, 0);
  BEGO_SPRITE_COUNT2.reset(1, 0);
  DYBE_SPRITE_COUNT3.reset(1, 0);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySpriteStore::update_count(wire XAPO_VID_RSTn, wire ZEME_AxCxExGx, wire ATEJ_LINE_RSTp, wire _DYTY_STORE_CLKp) {
  // Sprite store counter. The sprite count clock stops ticking once we have 10 sprites.
  // Sprite Y matcher. This is using an adder as a subtracter by inverting the first input.

  /*#p28.AZYB*/ wire _AZYB_LINE_TRIGn = not1(ATEJ_LINE_RSTp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p29.BAKY*/ wire _BAKY_SPRITES_FULL_new = and2(CUXY_SPRITE_COUNT1.qp_any(), DYBE_SPRITE_COUNT3.qp_any());

    /* p29.DEZY*/ DEZY_COUNT_CLKp.dff17_any(ZEME_AxCxExGx, XAPO_VID_RSTn, _DYTY_STORE_CLKp);
    /*#p29.CAKE*/ wire _CAKE_COUNT_CLKp_new = or2(_BAKY_SPRITES_FULL_new, DEZY_COUNT_CLKp.qp_any());

    /* p29.BESE*/ BESE_SPRITE_COUNT0.dff17_any(_CAKE_COUNT_CLKp_new,         _AZYB_LINE_TRIGn, BESE_SPRITE_COUNT0.qn_any());
    /* p29.CUXY*/ CUXY_SPRITE_COUNT1.dff17_any(BESE_SPRITE_COUNT0.qn_any(), _AZYB_LINE_TRIGn, CUXY_SPRITE_COUNT1.qn_any());
    /* p29.BEGO*/ BEGO_SPRITE_COUNT2.dff17_any(CUXY_SPRITE_COUNT1.qn_any(), _AZYB_LINE_TRIGn, BEGO_SPRITE_COUNT2.qn_any());
    /* p29.DYBE*/ DYBE_SPRITE_COUNT3.dff17_any(BEGO_SPRITE_COUNT2.qn_any(), _AZYB_LINE_TRIGn, DYBE_SPRITE_COUNT3.qn_any());
  }
}

//------------------------------------------------------------------------------------------------------------------------

SpriteStoreFlag GateBoySpriteStore::get_store_flags(wire _DYTY_STORE_CLKp) {
  /* p29.EDEN*/ wire _EDEN_SPRITE_COUNT0n_new = not1(BESE_SPRITE_COUNT0.qp_new());
  /* p29.CYPY*/ wire _CYPY_SPRITE_COUNT1n_new = not1(CUXY_SPRITE_COUNT1.qp_new());
  /* p29.CAPE*/ wire _CAPE_SPRITE_COUNT2n_new = not1(BEGO_SPRITE_COUNT2.qp_new());
  /* p29.CAXU*/ wire _CAXU_SPRITE_COUNT3n_new = not1(DYBE_SPRITE_COUNT3.qp_new());

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

SpriteMatchFlag GateBoySpriteStore::get_match_flags_old(wire _AROR_MATCH_ENp_old, const PixCount& _pix_count) {
  /*#p21.ACAM*/ wire ACAM_PX0n_old = not1(_pix_count.XEHO_PX0p.qp_old());
  /* p21.AZUB*/ wire AZUB_PX1n_old = not1(_pix_count.SAVY_PX1p.qp_old());
  /* p21.AMEL*/ wire AMEL_PX2n_old = not1(_pix_count.XODU_PX2p.qp_old());
  /* p21.AHAL*/ wire AHAL_PX3n_old = not1(_pix_count.XYDO_PX3p.qp_old());
  /* p21.APUX*/ wire APUX_PX4n_old = not1(_pix_count.TUHU_PX4p.qp_old());
  /* p21.ABEF*/ wire ABEF_PX5n_old = not1(_pix_count.TUKY_PX5p.qp_old());
  /* p21.ADAZ*/ wire ADAZ_PX6n_old = not1(_pix_count.TAKO_PX6p.qp_old());
  /* p21.ASAH*/ wire ASAH_PX7n_old = not1(_pix_count.SYBE_PX7p.qp_old());

  /*#p31.ZOGY*/ wire _ZOGY_STORE0_MATCH0n_old = xor2(XEPE_STORE0_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.ZEBA*/ wire _ZEBA_STORE0_MATCH1n_old = xor2(YLAH_STORE0_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.ZAHA*/ wire _ZAHA_STORE0_MATCH2n_old = xor2(ZOLA_STORE0_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.ZOKY*/ wire _ZOKY_STORE0_MATCH3n_old = xor2(ZULU_STORE0_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.WOJU*/ wire _WOJU_STORE0_MATCH4n_old = xor2(WELO_STORE0_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.YFUN*/ wire _YFUN_STORE0_MATCH5n_old = xor2(XUNY_STORE0_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.WYZA*/ wire _WYZA_STORE0_MATCH6n_old = xor2(WOTE_STORE0_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.YPUK*/ wire _YPUK_STORE0_MATCH7n_old = xor2(XAKO_STORE0_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.EDYM*/ wire _EDYM_STORE1_MATCH0n_old = xor2(DANY_STORE1_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.EMYB*/ wire _EMYB_STORE1_MATCH1n_old = xor2(DUKO_STORE1_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.EBEF*/ wire _EBEF_STORE1_MATCH2n_old = xor2(DESU_STORE1_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.EWOK*/ wire _EWOK_STORE1_MATCH3n_old = xor2(DAZO_STORE1_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.COLA*/ wire _COLA_STORE1_MATCH4n_old = xor2(DAKE_STORE1_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.BOBA*/ wire _BOBA_STORE1_MATCH5n_old = xor2(CESO_STORE1_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.COLU*/ wire _COLU_STORE1_MATCH6n_old = xor2(DYFU_STORE1_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.BAHU*/ wire _BAHU_STORE1_MATCH7n_old = xor2(CUSY_STORE1_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.FUZU*/ wire _FUZU_STORE2_MATCH0n_old = xor2(FOKA_STORE2_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.FESO*/ wire _FESO_STORE2_MATCH1n_old = xor2(FYTY_STORE2_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.FOKY*/ wire _FOKY_STORE2_MATCH2n_old = xor2(FUBY_STORE2_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.FYVA*/ wire _FYVA_STORE2_MATCH3n_old = xor2(GOXU_STORE2_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.CEKO*/ wire _CEKO_STORE2_MATCH4n_old = xor2(DUHY_STORE2_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.DETY*/ wire _DETY_STORE2_MATCH5n_old = xor2(EJUF_STORE2_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.DOZO*/ wire _DOZO_STORE2_MATCH6n_old = xor2(ENOR_STORE2_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.CONY*/ wire _CONY_STORE2_MATCH7n_old = xor2(DEPY_STORE2_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.YHOK*/ wire _YHOK_STORE3_MATCH0n_old = xor2(XOLY_STORE3_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.YCAH*/ wire _YCAH_STORE3_MATCH1n_old = xor2(XYBA_STORE3_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.YDAJ*/ wire _YDAJ_STORE3_MATCH2n_old = xor2(XABE_STORE3_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.YVUZ*/ wire _YVUZ_STORE3_MATCH3n_old = xor2(XEKA_STORE3_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.YVAP*/ wire _YVAP_STORE3_MATCH4n_old = xor2(XOMY_STORE3_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.XENY*/ wire _XENY_STORE3_MATCH5n_old = xor2(WUHA_STORE3_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.XAVU*/ wire _XAVU_STORE3_MATCH6n_old = xor2(WYNA_STORE3_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.XEVA*/ wire _XEVA_STORE3_MATCH7n_old = xor2(WECO_STORE3_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.XEJU*/ wire _XEJU_STORE4_MATCH0n_old = xor2(WEDU_STORE4_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.ZATE*/ wire _ZATE_STORE4_MATCH1n_old = xor2(YGAJ_STORE4_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.ZAKU*/ wire _ZAKU_STORE4_MATCH2n_old = xor2(ZYJO_STORE4_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.YBOX*/ wire _YBOX_STORE4_MATCH3n_old = xor2(XURY_STORE4_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.ZYKU*/ wire _ZYKU_STORE4_MATCH4n_old = xor2(YBED_STORE4_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.ZYPU*/ wire _ZYPU_STORE4_MATCH5n_old = xor2(ZALA_STORE4_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.XAHA*/ wire _XAHA_STORE4_MATCH6n_old = xor2(WYDE_STORE4_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.ZEFE*/ wire _ZEFE_STORE4_MATCH7n_old = xor2(XEPA_STORE4_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.GUZO*/ wire _GUZO_STORE5_MATCH0n_old = xor2(FUSA_STORE5_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.GOLA*/ wire _GOLA_STORE5_MATCH1n_old = xor2(FAXA_STORE5_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.GEVE*/ wire _GEVE_STORE5_MATCH2n_old = xor2(FOZY_STORE5_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.GUDE*/ wire _GUDE_STORE5_MATCH3n_old = xor2(FESY_STORE5_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.BAZY*/ wire _BAZY_STORE5_MATCH4n_old = xor2(CYWE_STORE5_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.CYLE*/ wire _CYLE_STORE5_MATCH5n_old = xor2(DYBY_STORE5_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.CEVA*/ wire _CEVA_STORE5_MATCH6n_old = xor2(DURY_STORE5_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.BUMY*/ wire _BUMY_STORE5_MATCH7n_old = xor2(CUVY_STORE5_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.XOSU*/ wire _XOSU_STORE6_MATCH0n_old = xor2(YCOL_STORE6_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.ZUVU*/ wire _ZUVU_STORE6_MATCH1n_old = xor2(YRAC_STORE6_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.XUCO*/ wire _XUCO_STORE6_MATCH2n_old = xor2(YMEM_STORE6_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.ZULO*/ wire _ZULO_STORE6_MATCH3n_old = xor2(YVAG_STORE6_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.ZARE*/ wire _ZARE_STORE6_MATCH4n_old = xor2(ZOLY_STORE6_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.ZEMU*/ wire _ZEMU_STORE6_MATCH5n_old = xor2(ZOGO_STORE6_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.ZYGO*/ wire _ZYGO_STORE6_MATCH6n_old = xor2(ZECU_STORE6_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.ZUZY*/ wire _ZUZY_STORE6_MATCH7n_old = xor2(ZESA_STORE6_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.DUSE*/ wire _DUSE_STORE7_MATCH0n_old = xor2(ERAZ_STORE7_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.DAGU*/ wire _DAGU_STORE7_MATCH1n_old = xor2(EPUM_STORE7_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.DYZE*/ wire _DYZE_STORE7_MATCH2n_old = xor2(EROL_STORE7_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.DESO*/ wire _DESO_STORE7_MATCH3n_old = xor2(EHYN_STORE7_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.EJOT*/ wire _EJOT_STORE7_MATCH4n_old = xor2(FAZU_STORE7_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.ESAJ*/ wire _ESAJ_STORE7_MATCH5n_old = xor2(FAXE_STORE7_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.DUCU*/ wire _DUCU_STORE7_MATCH6n_old = xor2(EXUK_STORE7_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.EWUD*/ wire _EWUD_STORE7_MATCH7n_old = xor2(FEDE_STORE7_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.DUZE*/ wire _DUZE_STORE8_MATCH0n_old = xor2(EZUF_STORE8_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.DAGA*/ wire _DAGA_STORE8_MATCH1n_old = xor2(ENAD_STORE8_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.DAWU*/ wire _DAWU_STORE8_MATCH2n_old = xor2(EBOW_STORE8_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.EJAW*/ wire _EJAW_STORE8_MATCH3n_old = xor2(FYCA_STORE8_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.GOHO*/ wire _GOHO_STORE8_MATCH4n_old = xor2(GAVY_STORE8_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.GASU*/ wire _GASU_STORE8_MATCH5n_old = xor2(GYPU_STORE8_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.GABU*/ wire _GABU_STORE8_MATCH6n_old = xor2(GADY_STORE8_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.GAFE*/ wire _GAFE_STORE8_MATCH7n_old = xor2(GAZA_STORE8_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.YMAM*/ wire _YMAM_STORE9_MATCH0n_old = xor2(XUVY_STORE9_X0p_odd.qn_old(), ACAM_PX0n_old);
  /* p31.YTYP*/ wire _YTYP_STORE9_MATCH1n_old = xor2(XERE_STORE9_X1p_odd.qn_old(), AZUB_PX1n_old);
  /* p31.YFOP*/ wire _YFOP_STORE9_MATCH2n_old = xor2(XUZO_STORE9_X2p_odd.qn_old(), AMEL_PX2n_old);
  /* p31.YVAC*/ wire _YVAC_STORE9_MATCH3n_old = xor2(XEXA_STORE9_X3p_odd.qn_old(), AHAL_PX3n_old);
  /* p31.ZYWU*/ wire _ZYWU_STORE9_MATCH4n_old = xor2(YPOD_STORE9_X4p_odd.qn_old(), APUX_PX4n_old);
  /* p31.ZUZA*/ wire _ZUZA_STORE9_MATCH5n_old = xor2(YROP_STORE9_X5p_odd.qn_old(), ABEF_PX5n_old);
  /* p31.ZEJO*/ wire _ZEJO_STORE9_MATCH6n_old = xor2(YNEP_STORE9_X6p_odd.qn_old(), ADAZ_PX6n_old);
  /* p31.ZEDA*/ wire _ZEDA_STORE9_MATCH7n_old = xor2(YZOF_STORE9_X7p_odd.qn_old(), ASAH_PX7n_old);

  /* p31.ZAKO*/ wire _ZAKO_STORE0_MATCHAp_old = nor4(_ZOGY_STORE0_MATCH0n_old, _ZEBA_STORE0_MATCH1n_old, _ZAHA_STORE0_MATCH2n_old, _ZOKY_STORE0_MATCH3n_old);
  /* p31.XEBA*/ wire _XEBA_STORE0_MATCHBp_old = nor4(_WOJU_STORE0_MATCH4n_old, _YFUN_STORE0_MATCH5n_old, _WYZA_STORE0_MATCH6n_old, _YPUK_STORE0_MATCH7n_old);
  /* p31.CYVY*/ wire _CYVY_STORE1_MATCHBp_old = nor4(_COLA_STORE1_MATCH4n_old, _BOBA_STORE1_MATCH5n_old, _COLU_STORE1_MATCH6n_old, _BAHU_STORE1_MATCH7n_old);
  /* p31.EWAM*/ wire _EWAM_STORE1_MATCHAp_old = nor4(_EDYM_STORE1_MATCH0n_old, _EMYB_STORE1_MATCH1n_old, _EBEF_STORE1_MATCH2n_old, _EWOK_STORE1_MATCH3n_old);
  /* p31.CEHU*/ wire _CEHU_STORE2_MATCHAp_old = nor4(_CEKO_STORE2_MATCH4n_old, _DETY_STORE2_MATCH5n_old, _DOZO_STORE2_MATCH6n_old, _CONY_STORE2_MATCH7n_old);
  /* p31.EKES*/ wire _EKES_STORE2_MATCHBp_old = nor4(_FUZU_STORE2_MATCH0n_old, _FESO_STORE2_MATCH1n_old, _FOKY_STORE2_MATCH2n_old, _FYVA_STORE2_MATCH3n_old);
  /* p31.ZURE*/ wire _ZURE_STORE3_MATCHAp_old = nor4(_YHOK_STORE3_MATCH0n_old, _YCAH_STORE3_MATCH1n_old, _YDAJ_STORE3_MATCH2n_old, _YVUZ_STORE3_MATCH3n_old);
  /* p31.YWOS*/ wire _YWOS_STORE3_MATCHBp_old = nor4(_YVAP_STORE3_MATCH4n_old, _XENY_STORE3_MATCH5n_old, _XAVU_STORE3_MATCH6n_old, _XEVA_STORE3_MATCH7n_old);
  /* p31.YKOK*/ wire _YKOK_STORE4_MATCHAp_old = nor4(_XEJU_STORE4_MATCH0n_old, _ZATE_STORE4_MATCH1n_old, _ZAKU_STORE4_MATCH2n_old, _YBOX_STORE4_MATCH3n_old);
  /* p31.YNAZ*/ wire _YNAZ_STORE4_MATCHBp_old = nor4(_ZYKU_STORE4_MATCH4n_old, _ZYPU_STORE4_MATCH5n_old, _XAHA_STORE4_MATCH6n_old, _ZEFE_STORE4_MATCH7n_old);
  /* p31.COGY*/ wire _COGY_STORE5_MATCHAp_old = nor4(_BAZY_STORE5_MATCH4n_old, _CYLE_STORE5_MATCH5n_old, _CEVA_STORE5_MATCH6n_old, _BUMY_STORE5_MATCH7n_old);
  /* p31.FYMA*/ wire _FYMA_STORE5_MATCHBp_old = nor4(_GUZO_STORE5_MATCH0n_old, _GOLA_STORE5_MATCH1n_old, _GEVE_STORE5_MATCH2n_old, _GUDE_STORE5_MATCH3n_old);
  /* p31.YWAP*/ wire _YWAP_STORE6_MATCHAp_old = nor4(_ZARE_STORE6_MATCH4n_old, _ZEMU_STORE6_MATCH5n_old, _ZYGO_STORE6_MATCH6n_old, _ZUZY_STORE6_MATCH7n_old);
  /* p31.YDOT*/ wire _YDOT_STORE6_MATCHBp_old = nor4(_XOSU_STORE6_MATCH0n_old, _ZUVU_STORE6_MATCH1n_old, _XUCO_STORE6_MATCH2n_old, _ZULO_STORE6_MATCH3n_old);
  /* p31.CYCO*/ wire _CYCO_STORE7_MATCHAp_old = nor4(_DUSE_STORE7_MATCH0n_old, _DAGU_STORE7_MATCH1n_old, _DYZE_STORE7_MATCH2n_old, _DESO_STORE7_MATCH3n_old);
  /* p31.DAJE*/ wire _DAJE_STORE7_MATCHBp_old = nor4(_EJOT_STORE7_MATCH4n_old, _ESAJ_STORE7_MATCH5n_old, _DUCU_STORE7_MATCH6n_old, _EWUD_STORE7_MATCH7n_old);
  /* p31.DAMA*/ wire _DAMA_STORE8_MATCHAp_old = nor4(_DUZE_STORE8_MATCH0n_old, _DAGA_STORE8_MATCH1n_old, _DAWU_STORE8_MATCH2n_old, _EJAW_STORE8_MATCH3n_old);
  /* p31.FEHA*/ wire _FEHA_STORE8_MATCHBp_old = nor4(_GOHO_STORE8_MATCH4n_old, _GASU_STORE8_MATCH5n_old, _GABU_STORE8_MATCH6n_old, _GAFE_STORE8_MATCH7n_old);
  /* p31.YLEV*/ wire _YLEV_STORE9_MATCHAp_old = nor4(_YMAM_STORE9_MATCH0n_old, _YTYP_STORE9_MATCH1n_old, _YFOP_STORE9_MATCH2n_old, _YVAC_STORE9_MATCH3n_old);
  /* p31.YTUB*/ wire _YTUB_STORE9_MATCHBp_old = nor4(_ZYWU_STORE9_MATCH4n_old, _ZUZA_STORE9_MATCH5n_old, _ZEJO_STORE9_MATCH6n_old, _ZEDA_STORE9_MATCH7n_old);

  /* p29.YDUG*/ wire _YDUG_STORE0_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _ZAKO_STORE0_MATCHAp_old, _XEBA_STORE0_MATCHBp_old);
  /* p29.DYDU*/ wire _DYDU_STORE1_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _EWAM_STORE1_MATCHAp_old, _CYVY_STORE1_MATCHBp_old);
  /* p29.DEGO*/ wire _DEGO_STORE2_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _CEHU_STORE2_MATCHAp_old, _EKES_STORE2_MATCHBp_old);
  /* p29.YLOZ*/ wire _YLOZ_STORE3_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _ZURE_STORE3_MATCHAp_old, _YWOS_STORE3_MATCHBp_old);
  /* p29.XAGE*/ wire _XAGE_STORE4_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _YKOK_STORE4_MATCHAp_old, _YNAZ_STORE4_MATCHBp_old);
  /* p29.EGOM*/ wire _EGOM_STORE5_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _COGY_STORE5_MATCHAp_old, _FYMA_STORE5_MATCHBp_old);
  /* p29.YBEZ*/ wire _YBEZ_STORE6_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _YWAP_STORE6_MATCHAp_old, _YDOT_STORE6_MATCHBp_old);
  /* p29.DYKA*/ wire _DYKA_STORE7_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _CYCO_STORE7_MATCHAp_old, _DAJE_STORE7_MATCHBp_old);
  /* p29.EFYL*/ wire _EFYL_STORE8_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _DAMA_STORE8_MATCHAp_old, _FEHA_STORE8_MATCHBp_old);
  /* p29.YGEM*/ wire _YGEM_STORE9_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _YLEV_STORE9_MATCHAp_old, _YTUB_STORE9_MATCHBp_old);

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

SpriteMatchFlag GateBoySpriteStore::get_match_flags_new(wire _AROR_MATCH_ENp_new, const PixCount& _pix_count) {
  /*#p21.ACAM*/ wire ACAM_PX0n = not1(_pix_count.XEHO_PX0p.qp_new());
  /* p21.AZUB*/ wire AZUB_PX1n = not1(_pix_count.SAVY_PX1p.qp_new());
  /* p21.AMEL*/ wire AMEL_PX2n = not1(_pix_count.XODU_PX2p.qp_new());
  /* p21.AHAL*/ wire AHAL_PX3n = not1(_pix_count.XYDO_PX3p.qp_new());
  /* p21.APUX*/ wire APUX_PX4n = not1(_pix_count.TUHU_PX4p.qp_new());
  /* p21.ABEF*/ wire ABEF_PX5n = not1(_pix_count.TUKY_PX5p.qp_new());
  /* p21.ADAZ*/ wire ADAZ_PX6n = not1(_pix_count.TAKO_PX6p.qp_new());
  /* p21.ASAH*/ wire ASAH_PX7n = not1(_pix_count.SYBE_PX7p.qp_new());

  /*#p31.ZOGY*/ wire _ZOGY_STORE0_MATCH0n_new = xor2(XEPE_STORE0_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.ZEBA*/ wire _ZEBA_STORE0_MATCH1n_new = xor2(YLAH_STORE0_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.ZAHA*/ wire _ZAHA_STORE0_MATCH2n_new = xor2(ZOLA_STORE0_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.ZOKY*/ wire _ZOKY_STORE0_MATCH3n_new = xor2(ZULU_STORE0_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.WOJU*/ wire _WOJU_STORE0_MATCH4n_new = xor2(WELO_STORE0_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.YFUN*/ wire _YFUN_STORE0_MATCH5n_new = xor2(XUNY_STORE0_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.WYZA*/ wire _WYZA_STORE0_MATCH6n_new = xor2(WOTE_STORE0_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.YPUK*/ wire _YPUK_STORE0_MATCH7n_new = xor2(XAKO_STORE0_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.EDYM*/ wire _EDYM_STORE1_MATCH0n_new = xor2(DANY_STORE1_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.EMYB*/ wire _EMYB_STORE1_MATCH1n_new = xor2(DUKO_STORE1_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.EBEF*/ wire _EBEF_STORE1_MATCH2n_new = xor2(DESU_STORE1_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.EWOK*/ wire _EWOK_STORE1_MATCH3n_new = xor2(DAZO_STORE1_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.COLA*/ wire _COLA_STORE1_MATCH4n_new = xor2(DAKE_STORE1_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.BOBA*/ wire _BOBA_STORE1_MATCH5n_new = xor2(CESO_STORE1_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.COLU*/ wire _COLU_STORE1_MATCH6n_new = xor2(DYFU_STORE1_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.BAHU*/ wire _BAHU_STORE1_MATCH7n_new = xor2(CUSY_STORE1_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.FUZU*/ wire _FUZU_STORE2_MATCH0n_new = xor2(FOKA_STORE2_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.FESO*/ wire _FESO_STORE2_MATCH1n_new = xor2(FYTY_STORE2_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.FOKY*/ wire _FOKY_STORE2_MATCH2n_new = xor2(FUBY_STORE2_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.FYVA*/ wire _FYVA_STORE2_MATCH3n_new = xor2(GOXU_STORE2_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.CEKO*/ wire _CEKO_STORE2_MATCH4n_new = xor2(DUHY_STORE2_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.DETY*/ wire _DETY_STORE2_MATCH5n_new = xor2(EJUF_STORE2_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.DOZO*/ wire _DOZO_STORE2_MATCH6n_new = xor2(ENOR_STORE2_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.CONY*/ wire _CONY_STORE2_MATCH7n_new = xor2(DEPY_STORE2_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.YHOK*/ wire _YHOK_STORE3_MATCH0n_new = xor2(XOLY_STORE3_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.YCAH*/ wire _YCAH_STORE3_MATCH1n_new = xor2(XYBA_STORE3_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.YDAJ*/ wire _YDAJ_STORE3_MATCH2n_new = xor2(XABE_STORE3_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.YVUZ*/ wire _YVUZ_STORE3_MATCH3n_new = xor2(XEKA_STORE3_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.YVAP*/ wire _YVAP_STORE3_MATCH4n_new = xor2(XOMY_STORE3_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.XENY*/ wire _XENY_STORE3_MATCH5n_new = xor2(WUHA_STORE3_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.XAVU*/ wire _XAVU_STORE3_MATCH6n_new = xor2(WYNA_STORE3_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.XEVA*/ wire _XEVA_STORE3_MATCH7n_new = xor2(WECO_STORE3_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.XEJU*/ wire _XEJU_STORE4_MATCH0n_new = xor2(WEDU_STORE4_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.ZATE*/ wire _ZATE_STORE4_MATCH1n_new = xor2(YGAJ_STORE4_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.ZAKU*/ wire _ZAKU_STORE4_MATCH2n_new = xor2(ZYJO_STORE4_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.YBOX*/ wire _YBOX_STORE4_MATCH3n_new = xor2(XURY_STORE4_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.ZYKU*/ wire _ZYKU_STORE4_MATCH4n_new = xor2(YBED_STORE4_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ZYPU*/ wire _ZYPU_STORE4_MATCH5n_new = xor2(ZALA_STORE4_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.XAHA*/ wire _XAHA_STORE4_MATCH6n_new = xor2(WYDE_STORE4_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.ZEFE*/ wire _ZEFE_STORE4_MATCH7n_new = xor2(XEPA_STORE4_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.GUZO*/ wire _GUZO_STORE5_MATCH0n_new = xor2(FUSA_STORE5_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.GOLA*/ wire _GOLA_STORE5_MATCH1n_new = xor2(FAXA_STORE5_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.GEVE*/ wire _GEVE_STORE5_MATCH2n_new = xor2(FOZY_STORE5_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.GUDE*/ wire _GUDE_STORE5_MATCH3n_new = xor2(FESY_STORE5_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.BAZY*/ wire _BAZY_STORE5_MATCH4n_new = xor2(CYWE_STORE5_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.CYLE*/ wire _CYLE_STORE5_MATCH5n_new = xor2(DYBY_STORE5_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.CEVA*/ wire _CEVA_STORE5_MATCH6n_new = xor2(DURY_STORE5_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.BUMY*/ wire _BUMY_STORE5_MATCH7n_new = xor2(CUVY_STORE5_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.XOSU*/ wire _XOSU_STORE6_MATCH0n_new = xor2(YCOL_STORE6_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.ZUVU*/ wire _ZUVU_STORE6_MATCH1n_new = xor2(YRAC_STORE6_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.XUCO*/ wire _XUCO_STORE6_MATCH2n_new = xor2(YMEM_STORE6_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.ZULO*/ wire _ZULO_STORE6_MATCH3n_new = xor2(YVAG_STORE6_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.ZARE*/ wire _ZARE_STORE6_MATCH4n_new = xor2(ZOLY_STORE6_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ZEMU*/ wire _ZEMU_STORE6_MATCH5n_new = xor2(ZOGO_STORE6_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.ZYGO*/ wire _ZYGO_STORE6_MATCH6n_new = xor2(ZECU_STORE6_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.ZUZY*/ wire _ZUZY_STORE6_MATCH7n_new = xor2(ZESA_STORE6_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.DUSE*/ wire _DUSE_STORE7_MATCH0n_new = xor2(ERAZ_STORE7_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.DAGU*/ wire _DAGU_STORE7_MATCH1n_new = xor2(EPUM_STORE7_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.DYZE*/ wire _DYZE_STORE7_MATCH2n_new = xor2(EROL_STORE7_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.DESO*/ wire _DESO_STORE7_MATCH3n_new = xor2(EHYN_STORE7_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.EJOT*/ wire _EJOT_STORE7_MATCH4n_new = xor2(FAZU_STORE7_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ESAJ*/ wire _ESAJ_STORE7_MATCH5n_new = xor2(FAXE_STORE7_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.DUCU*/ wire _DUCU_STORE7_MATCH6n_new = xor2(EXUK_STORE7_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.EWUD*/ wire _EWUD_STORE7_MATCH7n_new = xor2(FEDE_STORE7_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.DUZE*/ wire _DUZE_STORE8_MATCH0n_new = xor2(EZUF_STORE8_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.DAGA*/ wire _DAGA_STORE8_MATCH1n_new = xor2(ENAD_STORE8_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.DAWU*/ wire _DAWU_STORE8_MATCH2n_new = xor2(EBOW_STORE8_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.EJAW*/ wire _EJAW_STORE8_MATCH3n_new = xor2(FYCA_STORE8_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.GOHO*/ wire _GOHO_STORE8_MATCH4n_new = xor2(GAVY_STORE8_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.GASU*/ wire _GASU_STORE8_MATCH5n_new = xor2(GYPU_STORE8_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.GABU*/ wire _GABU_STORE8_MATCH6n_new = xor2(GADY_STORE8_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.GAFE*/ wire _GAFE_STORE8_MATCH7n_new = xor2(GAZA_STORE8_X7p_odd.qn_new(), ASAH_PX7n);

  /* p31.YMAM*/ wire _YMAM_STORE9_MATCH0n_new = xor2(XUVY_STORE9_X0p_odd.qn_new(), ACAM_PX0n);
  /* p31.YTYP*/ wire _YTYP_STORE9_MATCH1n_new = xor2(XERE_STORE9_X1p_odd.qn_new(), AZUB_PX1n);
  /* p31.YFOP*/ wire _YFOP_STORE9_MATCH2n_new = xor2(XUZO_STORE9_X2p_odd.qn_new(), AMEL_PX2n);
  /* p31.YVAC*/ wire _YVAC_STORE9_MATCH3n_new = xor2(XEXA_STORE9_X3p_odd.qn_new(), AHAL_PX3n);
  /* p31.ZYWU*/ wire _ZYWU_STORE9_MATCH4n_new = xor2(YPOD_STORE9_X4p_odd.qn_new(), APUX_PX4n);
  /* p31.ZUZA*/ wire _ZUZA_STORE9_MATCH5n_new = xor2(YROP_STORE9_X5p_odd.qn_new(), ABEF_PX5n);
  /* p31.ZEJO*/ wire _ZEJO_STORE9_MATCH6n_new = xor2(YNEP_STORE9_X6p_odd.qn_new(), ADAZ_PX6n);
  /* p31.ZEDA*/ wire _ZEDA_STORE9_MATCH7n_new = xor2(YZOF_STORE9_X7p_odd.qn_new(), ASAH_PX7n);

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

SpriteFirstMatch GateBoySpriteStore::get_first_match(SpriteMatchFlag match_flag) const {
  SigIn SIG_GND = 0;
  /* p29.WEFU*/ wire _WEFU_STORE0_MATCH = not1(match_flag.YDUG_STORE0_MATCHn);
  /* p29.GAJA*/ wire _GAJA_STORE1_MATCH = not1(match_flag.DYDU_STORE1_MATCHn);
  /* p29.GUPO*/ wire _GUPO_STORE2_MATCH = not1(match_flag.DEGO_STORE2_MATCHn);
  /* p29.WEBO*/ wire _WEBO_STORE3_MATCH = not1(match_flag.YLOZ_STORE3_MATCHn);
  /* p29.WUNA*/ wire _WUNA_STORE4_MATCH = not1(match_flag.XAGE_STORE4_MATCHn);
  /* p29.GABA*/ wire _GABA_STORE5_MATCH = not1(match_flag.EGOM_STORE5_MATCHn);
  /* p29.WASE*/ wire _WASE_STORE6_MATCH = not1(match_flag.YBEZ_STORE6_MATCHn);
  /* p29.GYTE*/ wire _GYTE_STORE7_MATCH = not1(match_flag.DYKA_STORE7_MATCHn);
  /* p29.GEKE*/ wire _GEKE_STORE8_MATCH = not1(match_flag.EFYL_STORE8_MATCHn);

  /* p29.GEZE*/ wire _GEZE_STORE0_MATCH = or2(_WEFU_STORE0_MATCH, SIG_GND);
  /* p29.FUMA*/ wire _FUMA_STORE1_MATCH = or2(_GAJA_STORE1_MATCH, _GEZE_STORE0_MATCH);
  /* p29.GEDE*/ wire _GEDE_STORE2_MATCH = or2(_GUPO_STORE2_MATCH, _FUMA_STORE1_MATCH);
  /* p29.WUTO*/ wire _WUTO_STORE3_MATCH = or2(_WEBO_STORE3_MATCH, _GEDE_STORE2_MATCH);
  /* p29.XYLA*/ wire _XYLA_STORE4_MATCH = or2(_WUNA_STORE4_MATCH, _WUTO_STORE3_MATCH);
  /* p29.WEJA*/ wire _WEJA_STORE5_MATCH = or2(_GABA_STORE5_MATCH, _XYLA_STORE4_MATCH);
  /* p29.WYLA*/ wire _WYLA_STORE6_MATCH = or2(_WASE_STORE6_MATCH, _WEJA_STORE5_MATCH);
  /* p29.FAVO*/ wire _FAVO_STORE7_MATCH = or2(_GYTE_STORE7_MATCH, _WYLA_STORE6_MATCH);
  /* p29.GYGA*/ wire _GYGA_STORE8_MATCH = or2(_GEKE_STORE8_MATCH, _FAVO_STORE7_MATCH);

  /* p29.GUVA*/ wire _GUVA_SPRITE0_GETp = nor2(match_flag.YDUG_STORE0_MATCHn, SIG_GND);
  /* p29.ENUT*/ wire _ENUT_SPRITE1_GETp = nor2(match_flag.DYDU_STORE1_MATCHn, _GEZE_STORE0_MATCH);
  /* p29.EMOL*/ wire _EMOL_SPRITE2_GETp = nor2(match_flag.DEGO_STORE2_MATCHn, _FUMA_STORE1_MATCH);
  /* p29.GYFY*/ wire _GYFY_SPRITE3_GETp = nor2(match_flag.YLOZ_STORE3_MATCHn, _GEDE_STORE2_MATCH);
  /* p29.GONO*/ wire _GONO_SPRITE4_GETp = nor2(match_flag.XAGE_STORE4_MATCHn, _WUTO_STORE3_MATCH);
  /* p29.GEGA*/ wire _GEGA_SPRITE5_GETp = nor2(match_flag.EGOM_STORE5_MATCHn, _XYLA_STORE4_MATCH);
  /* p29.XOJA*/ wire _XOJA_SPRITE6_GETp = nor2(match_flag.YBEZ_STORE6_MATCHn, _WEJA_STORE5_MATCH);
  /* p29.GUTU*/ wire _GUTU_SPRITE7_GETp = nor2(match_flag.DYKA_STORE7_MATCHn, _WYLA_STORE6_MATCH);
  /* p29.FOXA*/ wire _FOXA_SPRITE8_GETp = nor2(match_flag.EFYL_STORE8_MATCHn, _FAVO_STORE7_MATCH);
  /* p29.GUZE*/ wire _GUZE_SPRITE9_GETp = nor2(match_flag.YGEM_STORE9_MATCHn, _GYGA_STORE8_MATCH);

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

void GateBoySpriteStore::get_sprite(SpriteFirstMatch first_match) {

  /* p29.FURO*/ wire _FURO_SPRITE0_GETn = not1(first_match.GUVA_SPRITE0_GETp);
  /* p29.DYDO*/ wire _DYDO_SPRITE1_GETn = not1(first_match.ENUT_SPRITE1_GETp);
  /* p29.FAME*/ wire _FAME_SPRITE2_GETn = not1(first_match.EMOL_SPRITE2_GETp);
  /* p29.GYMA*/ wire _GYMA_SPRITE3_GETn = not1(first_match.GYFY_SPRITE3_GETp);
  /* p29.GOWO*/ wire _GOWO_SPRITE4_GETn = not1(first_match.GONO_SPRITE4_GETp);
  /* p29.GYGY*/ wire _GYGY_SPRITE5_GETn = not1(first_match.GEGA_SPRITE5_GETp);
  /* p29.XYME*/ wire _XYME_SPRITE6_GETn = not1(first_match.XOJA_SPRITE6_GETp);
  /* p29.GUGY*/ wire _GUGY_SPRITE7_GETn = not1(first_match.GUTU_SPRITE7_GETp);
  /* p29.DENY*/ wire _DENY_SPRITE8_GETn = not1(first_match.FOXA_SPRITE8_GETp);
  /* p29.FADO*/ wire _FADO_SPRITE9_GETn = not1(first_match.GUZE_SPRITE9_GETp);

  /* p30.ZETU_STORE0_GET_I0*/ BUS_SPR_I0.tri6_nn(_FURO_SPRITE0_GETn, YGUS_STORE0_I0n_odd.qp_new());
  /* p30.ZECE_STORE0_GET_I1*/ BUS_SPR_I1.tri6_nn(_FURO_SPRITE0_GETn, YSOK_STORE0_I1n_odd.qp_new());
  /* p30.ZAVE_STORE0_GET_I2*/ BUS_SPR_I2.tri6_nn(_FURO_SPRITE0_GETn, YZEP_STORE0_I2n_odd.qp_new());
  /* p30.WOKO_STORE0_GET_I3*/ BUS_SPR_I3.tri6_nn(_FURO_SPRITE0_GETn, WYTE_STORE0_I3n_odd.qp_new());
  /* p30.ZUMU_STORE0_GET_L0*/ BUS_SPR_I4.tri6_nn(_FURO_SPRITE0_GETn, ZONY_STORE0_I4n_odd.qp_new());
  /*#p30.ZEDY_STORE0_GET_L1*/ BUS_SPR_I5.tri6_nn(_FURO_SPRITE0_GETn, YWAK_STORE0_I5n_odd.qp_new());
  /* p30.WEHE_STORE0_GET_L2*/ BUS_SPR_L0.tri6_nn(_FURO_SPRITE0_GETn, GYHO_STORE0_L0n_odd.qp_new());
  /* p30.BUKY_STORE0_GET_L3*/ BUS_SPR_L1.tri6_nn(_FURO_SPRITE0_GETn, CUFO_STORE0_L1n_odd.qp_new());
  /* p30.AJAL_STORE0_GET_L4*/ BUS_SPR_L2.tri6_nn(_FURO_SPRITE0_GETn, BOZU_STORE0_L2n_odd.qp_new());
  /* p30.GOFO_STORE0_GET_L5*/ BUS_SPR_L3.tri6_nn(_FURO_SPRITE0_GETn, FYHY_STORE0_L3n_odd.qp_new());

  /*#p30.CUBO_STORE1_GET_I0*/ BUS_SPR_I0.tri6_nn(_DYDO_SPRITE1_GETn, CADU_STORE1_I0n_odd.qp_new());
  /* p30.CELU_STORE1_GET_I1*/ BUS_SPR_I1.tri6_nn(_DYDO_SPRITE1_GETn, CEBO_STORE1_I1n_odd.qp_new());
  /* p30.CEGY_STORE1_GET_I2*/ BUS_SPR_I2.tri6_nn(_DYDO_SPRITE1_GETn, CUFA_STORE1_I2n_odd.qp_new());
  /* p30.BETY_STORE1_GET_I3*/ BUS_SPR_I3.tri6_nn(_DYDO_SPRITE1_GETn, COMA_STORE1_I3n_odd.qp_new());
  /* p30.CYBY_STORE1_GET_L0*/ BUS_SPR_I4.tri6_nn(_DYDO_SPRITE1_GETn, CUZA_STORE1_I4n_odd.qp_new());
  /* p30.BEMO_STORE1_GET_L1*/ BUS_SPR_I5.tri6_nn(_DYDO_SPRITE1_GETn, CAJY_STORE1_I5n_odd.qp_new());
  /* p30.BYRO_STORE1_GET_L2*/ BUS_SPR_L0.tri6_nn(_DYDO_SPRITE1_GETn, AMES_STORE1_L0n_odd.qp_new());
  /* p30.AHUM_STORE1_GET_L3*/ BUS_SPR_L1.tri6_nn(_DYDO_SPRITE1_GETn, AROF_STORE1_L1n_odd.qp_new());
  /* p30.BACO_STORE1_GET_L4*/ BUS_SPR_L2.tri6_nn(_DYDO_SPRITE1_GETn, ABOP_STORE1_L2n_odd.qp_new());
  /* p30.BEFE_STORE1_GET_L5*/ BUS_SPR_L3.tri6_nn(_DYDO_SPRITE1_GETn, ABUG_STORE1_L3n_odd.qp_new());

  /* p30.CUBE_STORE2_GET_I0*/ BUS_SPR_I0.tri6_nn(_FAME_SPRITE2_GETn, BUHE_STORE2_I0n_odd.qp_new());
  /* p30.AFOZ_STORE2_GET_I1*/ BUS_SPR_I1.tri6_nn(_FAME_SPRITE2_GETn, BYHU_STORE2_I1n_odd.qp_new());
  /* p30.APON_STORE2_GET_I2*/ BUS_SPR_I2.tri6_nn(_FAME_SPRITE2_GETn, BECA_STORE2_I2n_odd.qp_new());
  /* p30.CUVU_STORE2_GET_I3*/ BUS_SPR_I3.tri6_nn(_FAME_SPRITE2_GETn, BULU_STORE2_I3n_odd.qp_new());
  /* p30.CYRO_STORE2_GET_L0*/ BUS_SPR_I4.tri6_nn(_FAME_SPRITE2_GETn, BUNA_STORE2_I4n_odd.qp_new());
  /* p30.AXEC_STORE2_GET_L1*/ BUS_SPR_I5.tri6_nn(_FAME_SPRITE2_GETn, BOXA_STORE2_I5n_odd.qp_new());
  /* p30.ZUKE_STORE2_GET_L2*/ BUS_SPR_L0.tri6_nn(_FAME_SPRITE2_GETn, YLOV_STORE2_L0n_odd.qp_new());
  /* p30.WERE_STORE2_GET_L3*/ BUS_SPR_L1.tri6_nn(_FAME_SPRITE2_GETn, XOSY_STORE2_L1n_odd.qp_new());
  /* p30.WUXE_STORE2_GET_L4*/ BUS_SPR_L2.tri6_nn(_FAME_SPRITE2_GETn, XAZY_STORE2_L2n_odd.qp_new());
  /* p30.ZABY_STORE2_GET_L5*/ BUS_SPR_L3.tri6_nn(_FAME_SPRITE2_GETn, YKUK_STORE2_L3n_odd.qp_new());

  /* p30.ENAP_STORE3_GET_I0*/ BUS_SPR_I0.tri6_nn(_GYMA_SPRITE3_GETn, DEVY_STORE3_I0n_odd.qp_new());
  /* p30.DYGO_STORE3_GET_I1*/ BUS_SPR_I1.tri6_nn(_GYMA_SPRITE3_GETn, DESE_STORE3_I1n_odd.qp_new());
  /* p30.DOWA_STORE3_GET_I2*/ BUS_SPR_I2.tri6_nn(_GYMA_SPRITE3_GETn, DUNY_STORE3_I2n_odd.qp_new());
  /* p30.DONY_STORE3_GET_I3*/ BUS_SPR_I3.tri6_nn(_GYMA_SPRITE3_GETn, DUHA_STORE3_I3n_odd.qp_new());
  /* p30.EFUD_STORE3_GET_L0*/ BUS_SPR_I4.tri6_nn(_GYMA_SPRITE3_GETn, DEBA_STORE3_I4n_odd.qp_new());
  /* p30.DEZU_STORE3_GET_L1*/ BUS_SPR_I5.tri6_nn(_GYMA_SPRITE3_GETn, DAFU_STORE3_I5n_odd.qp_new());
  /* p30.ZEXE_STORE3_GET_L2*/ BUS_SPR_L0.tri6_nn(_GYMA_SPRITE3_GETn, ZURO_STORE3_L0n_odd.qp_new());
  /* p30.YWAV_STORE3_GET_L3*/ BUS_SPR_L1.tri6_nn(_GYMA_SPRITE3_GETn, ZYLU_STORE3_L1n_odd.qp_new());
  /* p30.YJEM_STORE3_GET_L4*/ BUS_SPR_L2.tri6_nn(_GYMA_SPRITE3_GETn, ZENE_STORE3_L2n_odd.qp_new());
  /* p30.ZYPO_STORE3_GET_L5*/ BUS_SPR_L3.tri6_nn(_GYMA_SPRITE3_GETn, ZURY_STORE3_L3n_odd.qp_new());

  /* p30.WUXU_STORE4_GET_I0*/ BUS_SPR_I0.tri6_nn(_GOWO_SPRITE4_GETn, XAVE_STORE4_I0n_odd.qp_new());
  /* p30.WEPY_STORE4_GET_I1*/ BUS_SPR_I1.tri6_nn(_GOWO_SPRITE4_GETn, XEFE_STORE4_I1n_odd.qp_new());
  /* p30.WERU_STORE4_GET_I2*/ BUS_SPR_I2.tri6_nn(_GOWO_SPRITE4_GETn, WANU_STORE4_I2n_odd.qp_new());
  /* p30.XYRE_STORE4_GET_I3*/ BUS_SPR_I3.tri6_nn(_GOWO_SPRITE4_GETn, XABO_STORE4_I3n_odd.qp_new());
  /* p30.WOXY_STORE4_GET_L0*/ BUS_SPR_I4.tri6_nn(_GOWO_SPRITE4_GETn, XEGE_STORE4_I4n_odd.qp_new());
  /* p30.WAJA_STORE4_GET_L1*/ BUS_SPR_I5.tri6_nn(_GOWO_SPRITE4_GETn, XYNU_STORE4_I5n_odd.qp_new());
  /* p30.BUCE_STORE4_GET_L2*/ BUS_SPR_L0.tri6_nn(_GOWO_SPRITE4_GETn, CAPO_STORE4_L0n_odd.qp_new());
  /* p30.BEVY_STORE4_GET_L3*/ BUS_SPR_L1.tri6_nn(_GOWO_SPRITE4_GETn, CAJU_STORE4_L1n_odd.qp_new());
  /* p30.BOVE_STORE4_GET_L4*/ BUS_SPR_L2.tri6_nn(_GOWO_SPRITE4_GETn, CONO_STORE4_L2n_odd.qp_new());
  /* p30.BYDO_STORE4_GET_L5*/ BUS_SPR_L3.tri6_nn(_GOWO_SPRITE4_GETn, CUMU_STORE4_L3n_odd.qp_new());

  /* p30.DOBO_STORE5_GET_I0*/ BUS_SPR_I0.tri6_nn(_GYGY_SPRITE5_GETn, EKOP_STORE5_I0n_odd.qp_new());
  /* p30.DYNY_STORE5_GET_I1*/ BUS_SPR_I1.tri6_nn(_GYGY_SPRITE5_GETn, ETYM_STORE5_I1n_odd.qp_new());
  /* p30.WAGA_STORE5_GET_I2*/ BUS_SPR_I2.tri6_nn(_GYGY_SPRITE5_GETn, GORU_STORE5_I2n_odd.qp_new());
  /* p30.DUZA_STORE5_GET_I3*/ BUS_SPR_I3.tri6_nn(_GYGY_SPRITE5_GETn, EBEX_STORE5_I3n_odd.qp_new());
  /* p30.DALY_STORE5_GET_L0*/ BUS_SPR_I4.tri6_nn(_GYGY_SPRITE5_GETn, ETAV_STORE5_I4n_odd.qp_new());
  /* p30.DALO_STORE5_GET_L1*/ BUS_SPR_I5.tri6_nn(_GYGY_SPRITE5_GETn, EKAP_STORE5_I5n_odd.qp_new());
  /* p30.BACE_STORE5_GET_L2*/ BUS_SPR_L0.tri6_nn(_GYGY_SPRITE5_GETn, ACEP_STORE5_L0n_odd.qp_new());
  /* p30.BUJA_STORE5_GET_L3*/ BUS_SPR_L1.tri6_nn(_GYGY_SPRITE5_GETn, ABEG_STORE5_L1n_odd.qp_new());
  /* p30.BODU_STORE5_GET_L4*/ BUS_SPR_L2.tri6_nn(_GYGY_SPRITE5_GETn, ABUX_STORE5_L2n_odd.qp_new());
  /* p30.AWAT_STORE5_GET_L5*/ BUS_SPR_L3.tri6_nn(_GYGY_SPRITE5_GETn, ANED_STORE5_L3n_odd.qp_new());

  /* p30.WATO_STORE6_GET_I0*/ BUS_SPR_I0.tri6_nn(_XYME_SPRITE6_GETn, GABO_STORE6_I0n_odd.qp_new());
  /* p30.WYWY_STORE6_GET_I1*/ BUS_SPR_I1.tri6_nn(_XYME_SPRITE6_GETn, GACY_STORE6_I1n_odd.qp_new());
  /* p30.EZOC_STORE6_GET_I2*/ BUS_SPR_I2.tri6_nn(_XYME_SPRITE6_GETn, FOGO_STORE6_I2n_odd.qp_new());
  /* p30.WABO_STORE6_GET_I3*/ BUS_SPR_I3.tri6_nn(_XYME_SPRITE6_GETn, GOHU_STORE6_I3n_odd.qp_new());
  /* p30.ELYC_STORE6_GET_L0*/ BUS_SPR_I4.tri6_nn(_XYME_SPRITE6_GETn, FOXY_STORE6_I4n_odd.qp_new());
  /* p30.WOCY_STORE6_GET_L1*/ BUS_SPR_I5.tri6_nn(_XYME_SPRITE6_GETn, GECU_STORE6_I5n_odd.qp_new());
  /* p30.YBUK_STORE6_GET_L2*/ BUS_SPR_L0.tri6_nn(_XYME_SPRITE6_GETn, ZUMY_STORE6_L0n_odd.qp_new());
  /* p30.YKOZ_STORE6_GET_L3*/ BUS_SPR_L1.tri6_nn(_XYME_SPRITE6_GETn, ZAFU_STORE6_L1n_odd.qp_new());
  /* p30.ZYTO_STORE6_GET_L4*/ BUS_SPR_L2.tri6_nn(_XYME_SPRITE6_GETn, ZEXO_STORE6_L2n_odd.qp_new());
  /* p30.ZUDO_STORE6_GET_L5*/ BUS_SPR_L3.tri6_nn(_XYME_SPRITE6_GETn, ZUBE_STORE6_L3n_odd.qp_new());

  /* p30.WAKO_STORE7_GET_I0*/ BUS_SPR_I0.tri6_nn(_GUGY_SPRITE7_GETn, GULE_STORE7_I0n_odd.qp_new());
  /* p30.WYGO_STORE7_GET_I1*/ BUS_SPR_I1.tri6_nn(_GUGY_SPRITE7_GETn, GYNO_STORE7_I1n_odd.qp_new());
  /* p30.ELEP_STORE7_GET_I2*/ BUS_SPR_I2.tri6_nn(_GUGY_SPRITE7_GETn, FEFA_STORE7_I2n_odd.qp_new());
  /* p30.ETAD_STORE7_GET_I3*/ BUS_SPR_I3.tri6_nn(_GUGY_SPRITE7_GETn, FYSU_STORE7_I3n_odd.qp_new());
  /* p30.WABA_STORE7_GET_L0*/ BUS_SPR_I4.tri6_nn(_GUGY_SPRITE7_GETn, GESY_STORE7_I4n_odd.qp_new());
  /* p30.EVYT_STORE7_GET_L1*/ BUS_SPR_I5.tri6_nn(_GUGY_SPRITE7_GETn, FUZO_STORE7_I5n_odd.qp_new());
  /* p30.WAXE_STORE7_GET_L2*/ BUS_SPR_L0.tri6_nn(_GUGY_SPRITE7_GETn, XYNA_STORE7_L0n_odd.qp_new());
  /* p30.YPOZ_STORE7_GET_L3*/ BUS_SPR_L1.tri6_nn(_GUGY_SPRITE7_GETn, YGUM_STORE7_L1n_odd.qp_new());
  /* p30.WABU_STORE7_GET_L4*/ BUS_SPR_L2.tri6_nn(_GUGY_SPRITE7_GETn, XAKU_STORE7_L2n_odd.qp_new());
  /* p30.WANA_STORE7_GET_L5*/ BUS_SPR_L3.tri6_nn(_GUGY_SPRITE7_GETn, XYGO_STORE7_L3n_odd.qp_new());

  /* p30.APOC_STORE8_GET_I0*/ BUS_SPR_I0.tri6_nn(_DENY_SPRITE8_GETn, AXUV_STORE8_I0n_odd.qp_new());
  /* p30.AKYH_STORE8_GET_I1*/ BUS_SPR_I1.tri6_nn(_DENY_SPRITE8_GETn, BADA_STORE8_I1n_odd.qp_new());
  /* p30.AFEN_STORE8_GET_I2*/ BUS_SPR_I2.tri6_nn(_DENY_SPRITE8_GETn, APEV_STORE8_I2n_odd.qp_new());
  /* p30.APYV_STORE8_GET_I3*/ BUS_SPR_I3.tri6_nn(_DENY_SPRITE8_GETn, BADO_STORE8_I3n_odd.qp_new());
  /* p30.APOB_STORE8_GET_L0*/ BUS_SPR_I4.tri6_nn(_DENY_SPRITE8_GETn, BEXY_STORE8_I4n_odd.qp_new());
  /* p30.ADYB_STORE8_GET_L1*/ BUS_SPR_I5.tri6_nn(_DENY_SPRITE8_GETn, BYHE_STORE8_I5n_odd.qp_new());
  /* p30.BOSO_STORE8_GET_L2*/ BUS_SPR_L0.tri6_nn(_DENY_SPRITE8_GETn, AZAP_STORE8_L0n_odd.qp_new());
  /* p30.BAZU_STORE8_GET_L3*/ BUS_SPR_L1.tri6_nn(_DENY_SPRITE8_GETn, AFYX_STORE8_L1n_odd.qp_new());
  /* p30.AHAC_STORE8_GET_L4*/ BUS_SPR_L2.tri6_nn(_DENY_SPRITE8_GETn, AFUT_STORE8_L2n_odd.qp_new());
  /* p30.BUJY_STORE8_GET_L5*/ BUS_SPR_L3.tri6_nn(_DENY_SPRITE8_GETn, AFYM_STORE8_L3n_odd.qp_new());

  /* p30.ZARO_STORE9_GET_I0*/ BUS_SPR_I0.tri6_nn(_FADO_SPRITE9_GETn, YBER_STORE9_I0n_odd.qp_new());
  /* p30.ZOJY_STORE9_GET_I1*/ BUS_SPR_I1.tri6_nn(_FADO_SPRITE9_GETn, YZOR_STORE9_I1n_odd.qp_new());
  /* p30.YNEV_STORE9_GET_I2*/ BUS_SPR_I2.tri6_nn(_FADO_SPRITE9_GETn, XYFE_STORE9_I2n_odd.qp_new());
  /* p30.XYRA_STORE9_GET_I3*/ BUS_SPR_I3.tri6_nn(_FADO_SPRITE9_GETn, XOTU_STORE9_I3n_odd.qp_new());
  /* p30.YRAD_STORE9_GET_L0*/ BUS_SPR_I4.tri6_nn(_FADO_SPRITE9_GETn, XUTE_STORE9_I4n_odd.qp_new());
  /* p30.YHAL_STORE9_GET_L1*/ BUS_SPR_I5.tri6_nn(_FADO_SPRITE9_GETn, XUFO_STORE9_I5n_odd.qp_new());
  /* p30.BYME_STORE9_GET_L2*/ BUS_SPR_L0.tri6_nn(_FADO_SPRITE9_GETn, CANA_STORE9_L0n_odd.qp_new());
  /* p30.GATE_STORE9_GET_L3*/ BUS_SPR_L1.tri6_nn(_FADO_SPRITE9_GETn, FOFO_STORE9_L1n_odd.qp_new());
  /* p30.COHO_STORE9_GET_L4*/ BUS_SPR_L2.tri6_nn(_FADO_SPRITE9_GETn, DYSY_STORE9_L2n_odd.qp_new());
  /* p30.CAWO_STORE9_GET_L5*/ BUS_SPR_L3.tri6_nn(_FADO_SPRITE9_GETn, DEWU_STORE9_L3n_odd.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySpriteStore::store_sprite_x(SpriteStoreFlag store_flag, const OamTempB& oam_temp_b, wire _ABAK_LINE_RSTp, wire WUTY_SFETCH_DONE_TRIGp, SpriteFirstMatch sprite_flag) {
  // Sprite store X resetter. This has to happen before the match check.
  /* p28.BYVA*/ wire _BYVA_LINE_RSTn = not1(_ABAK_LINE_RSTp);
  /* p29.DYBA*/ wire _DYBA_LINE_RSTp = not1(_BYVA_LINE_RSTn);

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

  /* p29.DYWE*/ wire _DYWE_STORE0_RSTp = or2(_DYBA_LINE_RSTp, EBOJ_STORE0_RSTp_evn.qp_new());
  /* p29.EFEV*/ wire _EFEV_STORE1_RSTp = or2(_DYBA_LINE_RSTp, CEDY_STORE1_RSTp_evn.qp_new());
  /* p29.FOKO*/ wire _FOKO_STORE2_RSTp = or2(_DYBA_LINE_RSTp, EGAV_STORE2_RSTp_evn.qp_new());
  /* p29.GAKE*/ wire _GAKE_STORE3_RSTp = or2(_DYBA_LINE_RSTp, GOTA_STORE3_RSTp_evn.qp_new());
  /* p29.WOHU*/ wire _WOHU_STORE4_RSTp = or2(_DYBA_LINE_RSTp, XUDY_STORE4_RSTp_evn.qp_new());
  /* p29.FEVE*/ wire _FEVE_STORE5_RSTp = or2(_DYBA_LINE_RSTp, WAFY_STORE5_RSTp_evn.qp_new());
  /* p29.WACY*/ wire _WACY_STORE6_RSTp = or2(_DYBA_LINE_RSTp, WOMY_STORE6_RSTp_evn.qp_new());
  /* p29.GUKY*/ wire _GUKY_STORE7_RSTp = or2(_DYBA_LINE_RSTp, WAPO_STORE7_RSTp_evn.qp_new());
  /* p29.GORO*/ wire _GORO_STORE8_RSTp = or2(_DYBA_LINE_RSTp, EXUQ_STORE8_RSTp_evn.qp_new());
  /* p29.DUBU*/ wire _DUBU_STORE9_RSTp = or2(_DYBA_LINE_RSTp, FONO_STORE9_RSTp_evn.qp_new());

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


  /* p29.FUXU*/ wire _FUXU_STORE0_clk = not1(store_flag.DYHU_STORE0_CLKn);
  /* p29.ASYS*/ wire _ASYS_STORE1_clk = not1(store_flag.BUCO_STORE1_CLKn);
  /* p29.CACU*/ wire _CACU_STORE2_clk = not1(store_flag.GYFO_STORE2_CLKn);
  /* p29.YFAG*/ wire _YFAG_STORE3_clk = not1(store_flag.GUSA_STORE3_CLKn);
  /* p29.WOFO*/ wire _WOFO_STORE4_clk = not1(store_flag.DUKE_STORE4_CLKn);
  /* p29.CYLA*/ wire _CYLA_STORE5_clk = not1(store_flag.BEDE_STORE5_CLKn);
  /* p29.ZAPE*/ wire _ZAPE_STORE6_clk = not1(store_flag.WEKA_STORE6_CLKn);
  /* p29.GECY*/ wire _GECY_STORE7_clk = not1(store_flag.GYVO_STORE7_CLKn);
  /* p29.CEXU*/ wire _CEXU_STORE8_clk = not1(store_flag.BUKA_STORE8_CLKn);
  /* p29.WEME*/ wire _WEME_STORE9_clk = not1(store_flag.DECU_STORE9_CLKn);

  /*#p31.ZAGO*/ wire _ZAGO_SPX0n_new_any = not1(oam_temp_b.YLOR_OAM_DB0p.qp_old());
  /* p31.ZOCY*/ wire _ZOCY_SPX1n_new_any = not1(oam_temp_b.ZYTY_OAM_DB1p.qp_old());
  /* p31.YPUR*/ wire _YPUR_SPX2n_new_any = not1(oam_temp_b.ZYVE_OAM_DB2p.qp_old());
  /* p31.YVOK*/ wire _YVOK_SPX3n_new_any = not1(oam_temp_b.ZEZY_OAM_DB3p.qp_old());
  /* p31.COSE*/ wire _COSE_SPX4n_new_any = not1(oam_temp_b.GOMO_OAM_DB4p.qp_old());
  /* p31.AROP*/ wire _AROP_SPX5n_new_any = not1(oam_temp_b.BAXO_OAM_DB5p.qp_old());
  /* p31.XATU*/ wire _XATU_SPX6n_new_any = not1(oam_temp_b.YZOS_OAM_DB6p.qp_old());
  /* p31.BADY*/ wire _BADY_SPX7n_new_any = not1(oam_temp_b.DEPO_OAM_DB7p.qp_old());


  /*#p31.XEPE*/ XEPE_STORE0_X0p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.YLAH*/ YLAH_STORE0_X1p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.ZOLA*/ ZOLA_STORE0_X2p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _YPUR_SPX2n_new_any);
  /* p31.ZULU*/ ZULU_STORE0_X3p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _YVOK_SPX3n_new_any);
  /* p31.WELO*/ WELO_STORE0_X4p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _COSE_SPX4n_new_any);
  /* p31.XUNY*/ XUNY_STORE0_X5p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _AROP_SPX5n_new_any);
  /* p31.WOTE*/ WOTE_STORE0_X6p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _XATU_SPX6n_new_any);
  /* p31.XAKO*/ XAKO_STORE0_X7p_odd.dff9(_FUXU_STORE0_clk, _DYNA_STORE0_RSTn, _BADY_SPX7n_new_any);

  /* p31.DANY*/ DANY_STORE1_X0p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.DUKO*/ DUKO_STORE1_X1p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.DESU*/ DESU_STORE1_X2p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _YPUR_SPX2n_new_any);
  /* p31.DAZO*/ DAZO_STORE1_X3p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _YVOK_SPX3n_new_any);
  /* p31.DAKE*/ DAKE_STORE1_X4p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _COSE_SPX4n_new_any);
  /* p31.CESO*/ CESO_STORE1_X5p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _AROP_SPX5n_new_any);
  /* p31.DYFU*/ DYFU_STORE1_X6p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _XATU_SPX6n_new_any);
  /* p31.CUSY*/ CUSY_STORE1_X7p_odd.dff9(_ASYS_STORE1_clk, _DOKU_STORE1_RSTn, _BADY_SPX7n_new_any);

  /* p31.FOKA*/ FOKA_STORE2_X0p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.FYTY*/ FYTY_STORE2_X1p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.FUBY*/ FUBY_STORE2_X2p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _YPUR_SPX2n_new_any);
  /* p31.GOXU*/ GOXU_STORE2_X3p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _YVOK_SPX3n_new_any);
  /* p31.DUHY*/ DUHY_STORE2_X4p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _COSE_SPX4n_new_any);
  /* p31.EJUF*/ EJUF_STORE2_X5p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _AROP_SPX5n_new_any);
  /* p31.ENOR*/ ENOR_STORE2_X6p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _XATU_SPX6n_new_any);
  /* p31.DEPY*/ DEPY_STORE2_X7p_odd.dff9(_CACU_STORE2_clk, _GAMY_STORE2_RSTn, _BADY_SPX7n_new_any);

  /* p31.XOLY*/ XOLY_STORE3_X0p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.XYBA*/ XYBA_STORE3_X1p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.XABE*/ XABE_STORE3_X2p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _YPUR_SPX2n_new_any);
  /* p31.XEKA*/ XEKA_STORE3_X3p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _YVOK_SPX3n_new_any);
  /* p31.XOMY*/ XOMY_STORE3_X4p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _COSE_SPX4n_new_any);
  /* p31.WUHA*/ WUHA_STORE3_X5p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _AROP_SPX5n_new_any);
  /* p31.WYNA*/ WYNA_STORE3_X6p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _XATU_SPX6n_new_any);
  /* p31.WECO*/ WECO_STORE3_X7p_odd.dff9(_YFAG_STORE3_clk, _WUPA_STORE3_RSTn, _BADY_SPX7n_new_any);

  /* p31.WEDU*/ WEDU_STORE4_X0p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.YGAJ*/ YGAJ_STORE4_X1p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.ZYJO*/ ZYJO_STORE4_X2p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _YPUR_SPX2n_new_any);
  /* p31.XURY*/ XURY_STORE4_X3p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _YVOK_SPX3n_new_any);
  /* p31.YBED*/ YBED_STORE4_X4p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _COSE_SPX4n_new_any);
  /* p31.ZALA*/ ZALA_STORE4_X5p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _AROP_SPX5n_new_any);
  /* p31.WYDE*/ WYDE_STORE4_X6p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _XATU_SPX6n_new_any);
  /* p31.XEPA*/ XEPA_STORE4_X7p_odd.dff9(_WOFO_STORE4_clk, _WUNU_STORE4_RSTn, _BADY_SPX7n_new_any);

  /* p31.FUSA*/ FUSA_STORE5_X0p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.FAXA*/ FAXA_STORE5_X1p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.FOZY*/ FOZY_STORE5_X2p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _YPUR_SPX2n_new_any);
  /* p31.FESY*/ FESY_STORE5_X3p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _YVOK_SPX3n_new_any);
  /* p31.CYWE*/ CYWE_STORE5_X4p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _COSE_SPX4n_new_any);
  /* p31.DYBY*/ DYBY_STORE5_X5p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _AROP_SPX5n_new_any);
  /* p31.DURY*/ DURY_STORE5_X6p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _XATU_SPX6n_new_any);
  /* p31.CUVY*/ CUVY_STORE5_X7p_odd.dff9(_CYLA_STORE5_clk, _EJAD_STORE5_RSTn, _BADY_SPX7n_new_any);

  /* p31.YCOL*/ YCOL_STORE6_X0p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.YRAC*/ YRAC_STORE6_X1p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.YMEM*/ YMEM_STORE6_X2p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _YPUR_SPX2n_new_any);
  /* p31.YVAG*/ YVAG_STORE6_X3p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _YVOK_SPX3n_new_any);
  /* p31.ZOLY*/ ZOLY_STORE6_X4p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _COSE_SPX4n_new_any);
  /* p31.ZOGO*/ ZOGO_STORE6_X5p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _AROP_SPX5n_new_any);
  /* p31.ZECU*/ ZECU_STORE6_X6p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _XATU_SPX6n_new_any);
  /* p31.ZESA*/ ZESA_STORE6_X7p_odd.dff9(_ZAPE_STORE6_clk, _XAHO_STORE6_RSTn, _BADY_SPX7n_new_any);

  /* p31.ERAZ*/ ERAZ_STORE7_X0p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.EPUM*/ EPUM_STORE7_X1p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.EROL*/ EROL_STORE7_X2p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _YPUR_SPX2n_new_any);
  /* p31.EHYN*/ EHYN_STORE7_X3p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _YVOK_SPX3n_new_any);
  /* p31.FAZU*/ FAZU_STORE7_X4p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _COSE_SPX4n_new_any);
  /* p31.FAXE*/ FAXE_STORE7_X5p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _AROP_SPX5n_new_any);
  /* p31.EXUK*/ EXUK_STORE7_X6p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _XATU_SPX6n_new_any);
  /* p31.FEDE*/ FEDE_STORE7_X7p_odd.dff9(_GECY_STORE7_clk, _GAFY_STORE7_RSTn, _BADY_SPX7n_new_any);

  /* p31.EZUF*/ EZUF_STORE8_X0p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.ENAD*/ ENAD_STORE8_X1p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.EBOW*/ EBOW_STORE8_X2p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _YPUR_SPX2n_new_any);
  /* p31.FYCA*/ FYCA_STORE8_X3p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _YVOK_SPX3n_new_any);
  /* p31.GAVY*/ GAVY_STORE8_X4p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _COSE_SPX4n_new_any);
  /* p31.GYPU*/ GYPU_STORE8_X5p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _AROP_SPX5n_new_any);
  /* p31.GADY*/ GADY_STORE8_X6p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _XATU_SPX6n_new_any);
  /* p31.GAZA*/ GAZA_STORE8_X7p_odd.dff9(_CEXU_STORE8_clk, _WUZO_STORE8_RSTn, _BADY_SPX7n_new_any);

  /* p31.XUVY*/ XUVY_STORE9_X0p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _ZAGO_SPX0n_new_any);
  /* p31.XERE*/ XERE_STORE9_X1p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _ZOCY_SPX1n_new_any);
  /* p31.XUZO*/ XUZO_STORE9_X2p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _YPUR_SPX2n_new_any);
  /* p31.XEXA*/ XEXA_STORE9_X3p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _YVOK_SPX3n_new_any);
  /* p31.YPOD*/ YPOD_STORE9_X4p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _COSE_SPX4n_new_any);
  /* p31.YROP*/ YROP_STORE9_X5p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _AROP_SPX5n_new_any);
  /* p31.YNEP*/ YNEP_STORE9_X6p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _XATU_SPX6n_new_any);
  /* p31.YZOF*/ YZOF_STORE9_X7p_odd.dff9(_WEME_STORE9_clk, _DOSY_STORE9_RSTn, _BADY_SPX7n_new_any);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySpriteStore::store_sprite_index(SpriteStoreFlag store_flag) {
  /* p29.GENY*/ wire _GENY_STORE0_CLKp = not1(store_flag.DYHU_STORE0_CLKn);
  /* p29.BYVY*/ wire _BYVY_STORE1_CLKp = not1(store_flag.BUCO_STORE1_CLKn);
  /* p29.BUZY*/ wire _BUZY_STORE2_CLKp = not1(store_flag.GYFO_STORE2_CLKn);
  /* p29.FEKA*/ wire _FEKA_STORE3_CLKp = not1(store_flag.GUSA_STORE3_CLKn);
  /* p29.WYLU*/ wire _WYLU_STORE4_CLKp = not1(store_flag.DUKE_STORE4_CLKn);
  /* p29.DYMO*/ wire _DYMO_STORE5_CLKp = not1(store_flag.BEDE_STORE5_CLKn);
  /* p29.WUSE*/ wire _WUSE_STORE6_CLKp = not1(store_flag.WEKA_STORE6_CLKn);
  /* p29.FEFO*/ wire _FEFO_STORE7_CLKp = not1(store_flag.GYVO_STORE7_CLKn);
  /* p29.AKOL*/ wire _AKOL_STORE8_CLKp = not1(store_flag.BUKA_STORE8_CLKn);
  /* p29.WUFA*/ wire _WUFA_STORE9_CLKp = not1(store_flag.DECU_STORE9_CLKn);

  /* p30.YGUS*/ YGUS_STORE0_I0n_odd.dff8n(_GENY_STORE0_CLKp, BUS_SPR_I0.qp_old());
  /* p30.YSOK*/ YSOK_STORE0_I1n_odd.dff8n(_GENY_STORE0_CLKp, BUS_SPR_I1.qp_old());
  /* p30.YZEP*/ YZEP_STORE0_I2n_odd.dff8n(_GENY_STORE0_CLKp, BUS_SPR_I2.qp_old());
  /* p30.WYTE*/ WYTE_STORE0_I3n_odd.dff8n(_GENY_STORE0_CLKp, BUS_SPR_I3.qp_old());
  /* p30.ZONY*/ ZONY_STORE0_I4n_odd.dff8n(_GENY_STORE0_CLKp, BUS_SPR_I4.qp_old());
  /* p30.YWAK*/ YWAK_STORE0_I5n_odd.dff8n(_GENY_STORE0_CLKp, BUS_SPR_I5.qp_old());

  /* p30.CADU*/ CADU_STORE1_I0n_odd.dff8n(_BYVY_STORE1_CLKp, BUS_SPR_I0.qp_old());
  /* p30.CEBO*/ CEBO_STORE1_I1n_odd.dff8n(_BYVY_STORE1_CLKp, BUS_SPR_I1.qp_old());
  /* p30.CUFA*/ CUFA_STORE1_I2n_odd.dff8n(_BYVY_STORE1_CLKp, BUS_SPR_I2.qp_old());
  /* p30.COMA*/ COMA_STORE1_I3n_odd.dff8n(_BYVY_STORE1_CLKp, BUS_SPR_I3.qp_old());
  /* p30.CUZA*/ CUZA_STORE1_I4n_odd.dff8n(_BYVY_STORE1_CLKp, BUS_SPR_I4.qp_old());
  /* p30.CAJY*/ CAJY_STORE1_I5n_odd.dff8n(_BYVY_STORE1_CLKp, BUS_SPR_I5.qp_old());

  /* p30.BUHE*/ BUHE_STORE2_I0n_odd.dff8n(_BUZY_STORE2_CLKp, BUS_SPR_I0.qp_old());
  /* p30.BYHU*/ BYHU_STORE2_I1n_odd.dff8n(_BUZY_STORE2_CLKp, BUS_SPR_I1.qp_old());
  /* p30.BECA*/ BECA_STORE2_I2n_odd.dff8n(_BUZY_STORE2_CLKp, BUS_SPR_I2.qp_old());
  /* p30.BULU*/ BULU_STORE2_I3n_odd.dff8n(_BUZY_STORE2_CLKp, BUS_SPR_I3.qp_old());
  /* p30.BUNA*/ BUNA_STORE2_I4n_odd.dff8n(_BUZY_STORE2_CLKp, BUS_SPR_I4.qp_old());
  /* p30.BOXA*/ BOXA_STORE2_I5n_odd.dff8n(_BUZY_STORE2_CLKp, BUS_SPR_I5.qp_old());

  /* p30.DEVY*/ DEVY_STORE3_I0n_odd.dff8n(_FEKA_STORE3_CLKp, BUS_SPR_I0.qp_old());
  /* p30.DESE*/ DESE_STORE3_I1n_odd.dff8n(_FEKA_STORE3_CLKp, BUS_SPR_I1.qp_old());
  /* p30.DUNY*/ DUNY_STORE3_I2n_odd.dff8n(_FEKA_STORE3_CLKp, BUS_SPR_I2.qp_old());
  /* p30.DUHA*/ DUHA_STORE3_I3n_odd.dff8n(_FEKA_STORE3_CLKp, BUS_SPR_I3.qp_old());
  /* p30.DEBA*/ DEBA_STORE3_I4n_odd.dff8n(_FEKA_STORE3_CLKp, BUS_SPR_I4.qp_old());
  /* p30.DAFU*/ DAFU_STORE3_I5n_odd.dff8n(_FEKA_STORE3_CLKp, BUS_SPR_I5.qp_old());

  /* p30.XAVE*/ XAVE_STORE4_I0n_odd.dff8n(_WYLU_STORE4_CLKp, BUS_SPR_I0.qp_old());
  /* p30.XEFE*/ XEFE_STORE4_I1n_odd.dff8n(_WYLU_STORE4_CLKp, BUS_SPR_I1.qp_old());
  /* p30.WANU*/ WANU_STORE4_I2n_odd.dff8n(_WYLU_STORE4_CLKp, BUS_SPR_I2.qp_old());
  /* p30.XABO*/ XABO_STORE4_I3n_odd.dff8n(_WYLU_STORE4_CLKp, BUS_SPR_I3.qp_old());
  /* p30.XEGE*/ XEGE_STORE4_I4n_odd.dff8n(_WYLU_STORE4_CLKp, BUS_SPR_I4.qp_old());
  /* p30.XYNU*/ XYNU_STORE4_I5n_odd.dff8n(_WYLU_STORE4_CLKp, BUS_SPR_I5.qp_old());

  /* p30.EKOP*/ EKOP_STORE5_I0n_odd.dff8n(_DYMO_STORE5_CLKp, BUS_SPR_I0.qp_old());
  /* p30.ETYM*/ ETYM_STORE5_I1n_odd.dff8n(_DYMO_STORE5_CLKp, BUS_SPR_I1.qp_old());
  /* p30.GORU*/ GORU_STORE5_I2n_odd.dff8n(_DYMO_STORE5_CLKp, BUS_SPR_I2.qp_old());
  /* p30.EBEX*/ EBEX_STORE5_I3n_odd.dff8n(_DYMO_STORE5_CLKp, BUS_SPR_I3.qp_old());
  /* p30.ETAV*/ ETAV_STORE5_I4n_odd.dff8n(_DYMO_STORE5_CLKp, BUS_SPR_I4.qp_old());
  /* p30.EKAP*/ EKAP_STORE5_I5n_odd.dff8n(_DYMO_STORE5_CLKp, BUS_SPR_I5.qp_old());

  /* p30.GABO*/ GABO_STORE6_I0n_odd.dff8n(_WUSE_STORE6_CLKp, BUS_SPR_I0.qp_old());
  /* p30.GACY*/ GACY_STORE6_I1n_odd.dff8n(_WUSE_STORE6_CLKp, BUS_SPR_I1.qp_old());
  /* p30.FOGO*/ FOGO_STORE6_I2n_odd.dff8n(_WUSE_STORE6_CLKp, BUS_SPR_I2.qp_old());
  /* p30.GOHU*/ GOHU_STORE6_I3n_odd.dff8n(_WUSE_STORE6_CLKp, BUS_SPR_I3.qp_old());
  /* p30.FOXY*/ FOXY_STORE6_I4n_odd.dff8n(_WUSE_STORE6_CLKp, BUS_SPR_I4.qp_old());
  /* p30.GECU*/ GECU_STORE6_I5n_odd.dff8n(_WUSE_STORE6_CLKp, BUS_SPR_I5.qp_old());

  /* p30.GULE*/ GULE_STORE7_I0n_odd.dff8n(_FEFO_STORE7_CLKp, BUS_SPR_I0.qp_old());
  /* p30.GYNO*/ GYNO_STORE7_I1n_odd.dff8n(_FEFO_STORE7_CLKp, BUS_SPR_I1.qp_old());
  /* p30.FEFA*/ FEFA_STORE7_I2n_odd.dff8n(_FEFO_STORE7_CLKp, BUS_SPR_I2.qp_old());
  /* p30.FYSU*/ FYSU_STORE7_I3n_odd.dff8n(_FEFO_STORE7_CLKp, BUS_SPR_I3.qp_old());
  /* p30.GESY*/ GESY_STORE7_I4n_odd.dff8n(_FEFO_STORE7_CLKp, BUS_SPR_I4.qp_old());
  /* p30.FUZO*/ FUZO_STORE7_I5n_odd.dff8n(_FEFO_STORE7_CLKp, BUS_SPR_I5.qp_old());

  /* p30.AXUV*/ AXUV_STORE8_I0n_odd.dff8n(_AKOL_STORE8_CLKp, BUS_SPR_I0.qp_old());
  /* p30.BADA*/ BADA_STORE8_I1n_odd.dff8n(_AKOL_STORE8_CLKp, BUS_SPR_I1.qp_old());
  /* p30.APEV*/ APEV_STORE8_I2n_odd.dff8n(_AKOL_STORE8_CLKp, BUS_SPR_I2.qp_old());
  /* p30.BADO*/ BADO_STORE8_I3n_odd.dff8n(_AKOL_STORE8_CLKp, BUS_SPR_I3.qp_old());
  /* p30.BEXY*/ BEXY_STORE8_I4n_odd.dff8n(_AKOL_STORE8_CLKp, BUS_SPR_I4.qp_old());
  /* p30.BYHE*/ BYHE_STORE8_I5n_odd.dff8n(_AKOL_STORE8_CLKp, BUS_SPR_I5.qp_old());

  /* p30.YBER*/ YBER_STORE9_I0n_odd.dff8n(_WUFA_STORE9_CLKp, BUS_SPR_I0.qp_old());
  /* p30.YZOR*/ YZOR_STORE9_I1n_odd.dff8n(_WUFA_STORE9_CLKp, BUS_SPR_I1.qp_old());
  /* p30.XYFE*/ XYFE_STORE9_I2n_odd.dff8n(_WUFA_STORE9_CLKp, BUS_SPR_I2.qp_old());
  /* p30.XOTU*/ XOTU_STORE9_I3n_odd.dff8n(_WUFA_STORE9_CLKp, BUS_SPR_I3.qp_old());
  /* p30.XUTE*/ XUTE_STORE9_I4n_odd.dff8n(_WUFA_STORE9_CLKp, BUS_SPR_I4.qp_old());
  /* p30.XUFO*/ XUFO_STORE9_I5n_odd.dff8n(_WUFA_STORE9_CLKp, BUS_SPR_I5.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySpriteStore::store_sprite_line(SpriteStoreFlag store_flag) {
  /* p29.ENOB*/ wire _ENOB_STORE0_CLKp = not1(store_flag.DYHU_STORE0_CLKn);
  /* p29.AHOF*/ wire _AHOF_STORE1_CLKp = not1(store_flag.BUCO_STORE1_CLKn);
  /* p29.FUKE*/ wire _FUKE_STORE2_CLKp = not1(store_flag.GYFO_STORE2_CLKn);
  /* p29.XYHA*/ wire _XYHA_STORE3_CLKp = not1(store_flag.GUSA_STORE3_CLKn);
  /* p29.EWOT*/ wire _EWOT_STORE4_CLKp = not1(store_flag.DUKE_STORE4_CLKn);
  /* p29.BUCY*/ wire _BUCY_STORE5_CLKp = not1(store_flag.BEDE_STORE5_CLKn);
  /* p29.ZURU*/ wire _ZURU_STORE6_CLKp = not1(store_flag.WEKA_STORE6_CLKn);
  /* p29.WABE*/ wire _WABE_STORE7_CLKp = not1(store_flag.GYVO_STORE7_CLKn);
  /* p29.BYMY*/ wire _BYMY_STORE8_CLKp = not1(store_flag.BUKA_STORE8_CLKn);
  /* p29.FAKA*/ wire _FAKA_STORE9_CLKp = not1(store_flag.DECU_STORE9_CLKn);

  /* p30.GYHO*/ GYHO_STORE0_L0n_odd.dff8n(_ENOB_STORE0_CLKp, BUS_SPR_L0.qp_old());
  /* p30.CUFO*/ CUFO_STORE0_L1n_odd.dff8n(_ENOB_STORE0_CLKp, BUS_SPR_L1.qp_old());
  /* p30.BOZU*/ BOZU_STORE0_L2n_odd.dff8n(_ENOB_STORE0_CLKp, BUS_SPR_L2.qp_old());
  /* p30.FYHY*/ FYHY_STORE0_L3n_odd.dff8n(_ENOB_STORE0_CLKp, BUS_SPR_L3.qp_old());

  /* p30.AMES*/ AMES_STORE1_L0n_odd.dff8n(_AHOF_STORE1_CLKp, BUS_SPR_L0.qp_old());
  /* p30.AROF*/ AROF_STORE1_L1n_odd.dff8n(_AHOF_STORE1_CLKp, BUS_SPR_L1.qp_old());
  /* p30.ABOP*/ ABOP_STORE1_L2n_odd.dff8n(_AHOF_STORE1_CLKp, BUS_SPR_L2.qp_old());
  /* p30.ABUG*/ ABUG_STORE1_L3n_odd.dff8n(_AHOF_STORE1_CLKp, BUS_SPR_L3.qp_old());

  /* p30.YLOV*/ YLOV_STORE2_L0n_odd.dff8n(_FUKE_STORE2_CLKp, BUS_SPR_L0.qp_old());
  /* p30.XOSY*/ XOSY_STORE2_L1n_odd.dff8n(_FUKE_STORE2_CLKp, BUS_SPR_L1.qp_old());
  /* p30.XAZY*/ XAZY_STORE2_L2n_odd.dff8n(_FUKE_STORE2_CLKp, BUS_SPR_L2.qp_old());
  /* p30.YKUK*/ YKUK_STORE2_L3n_odd.dff8n(_FUKE_STORE2_CLKp, BUS_SPR_L3.qp_old());

  /* p30.ZURO*/ ZURO_STORE3_L0n_odd.dff8n(_XYHA_STORE3_CLKp, BUS_SPR_L0.qp_old());
  /* p30.ZYLU*/ ZYLU_STORE3_L1n_odd.dff8n(_XYHA_STORE3_CLKp, BUS_SPR_L1.qp_old());
  /* p30.ZENE*/ ZENE_STORE3_L2n_odd.dff8n(_XYHA_STORE3_CLKp, BUS_SPR_L2.qp_old());
  /* p30.ZURY*/ ZURY_STORE3_L3n_odd.dff8n(_XYHA_STORE3_CLKp, BUS_SPR_L3.qp_old());

  /* p30.CAPO*/ CAPO_STORE4_L0n_odd.dff8n(_EWOT_STORE4_CLKp, BUS_SPR_L0.qp_old());
  /* p30.CAJU*/ CAJU_STORE4_L1n_odd.dff8n(_EWOT_STORE4_CLKp, BUS_SPR_L1.qp_old());
  /* p30.CONO*/ CONO_STORE4_L2n_odd.dff8n(_EWOT_STORE4_CLKp, BUS_SPR_L2.qp_old());
  /* p30.CUMU*/ CUMU_STORE4_L3n_odd.dff8n(_EWOT_STORE4_CLKp, BUS_SPR_L3.qp_old());

  /* p30.ACEP*/ ACEP_STORE5_L0n_odd.dff8n(_BUCY_STORE5_CLKp, BUS_SPR_L0.qp_old());
  /* p30.ABEG*/ ABEG_STORE5_L1n_odd.dff8n(_BUCY_STORE5_CLKp, BUS_SPR_L1.qp_old());
  /* p30.ABUX*/ ABUX_STORE5_L2n_odd.dff8n(_BUCY_STORE5_CLKp, BUS_SPR_L2.qp_old());
  /* p30.ANED*/ ANED_STORE5_L3n_odd.dff8n(_BUCY_STORE5_CLKp, BUS_SPR_L3.qp_old());

  /* p30.ZUMY*/ ZUMY_STORE6_L0n_odd.dff8n(_ZURU_STORE6_CLKp, BUS_SPR_L0.qp_old());
  /* p30.ZAFU*/ ZAFU_STORE6_L1n_odd.dff8n(_ZURU_STORE6_CLKp, BUS_SPR_L1.qp_old());
  /* p30.ZEXO*/ ZEXO_STORE6_L2n_odd.dff8n(_ZURU_STORE6_CLKp, BUS_SPR_L2.qp_old());
  /* p30.ZUBE*/ ZUBE_STORE6_L3n_odd.dff8n(_ZURU_STORE6_CLKp, BUS_SPR_L3.qp_old());

  /* p30.XYNA*/ XYNA_STORE7_L0n_odd.dff8n(_WABE_STORE7_CLKp, BUS_SPR_L0.qp_old());
  /* p30.YGUM*/ YGUM_STORE7_L1n_odd.dff8n(_WABE_STORE7_CLKp, BUS_SPR_L1.qp_old());
  /* p30.XAKU*/ XAKU_STORE7_L2n_odd.dff8n(_WABE_STORE7_CLKp, BUS_SPR_L2.qp_old());
  /* p30.XYGO*/ XYGO_STORE7_L3n_odd.dff8n(_WABE_STORE7_CLKp, BUS_SPR_L3.qp_old());

  /* p30.AZAP*/ AZAP_STORE8_L0n_odd.dff8n(_BYMY_STORE8_CLKp, BUS_SPR_L0.qp_old());
  /* p30.AFYX*/ AFYX_STORE8_L1n_odd.dff8n(_BYMY_STORE8_CLKp, BUS_SPR_L1.qp_old());
  /* p30.AFUT*/ AFUT_STORE8_L2n_odd.dff8n(_BYMY_STORE8_CLKp, BUS_SPR_L2.qp_old());
  /* p30.AFYM*/ AFYM_STORE8_L3n_odd.dff8n(_BYMY_STORE8_CLKp, BUS_SPR_L3.qp_old());

  /* p30.CANA*/ CANA_STORE9_L0n_odd.dff8n(_FAKA_STORE9_CLKp, BUS_SPR_L0.qp_old());
  /* p30.FOFO*/ FOFO_STORE9_L1n_odd.dff8n(_FAKA_STORE9_CLKp, BUS_SPR_L1.qp_old());
  /* p30.DYSY*/ DYSY_STORE9_L2n_odd.dff8n(_FAKA_STORE9_CLKp, BUS_SPR_L2.qp_old());
  /* p30.DEWU*/ DEWU_STORE9_L3n_odd.dff8n(_FAKA_STORE9_CLKp, BUS_SPR_L3.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySpriteStore::ly_to_sprite_line(wire FEPO_STORE_MATCHp, const RegLY& reg_ly, const OamTempA& oam_temp_a) {
  SigIn SIG_GND = 0;

  /*#p29.EBOS*/ wire EBOS_LY0n = not1(reg_ly.MUWY_LY0p.qp_new());
  /* p29.DASA*/ wire DASA_LY1n = not1(reg_ly.MYRO_LY1p.qp_new());
  /* p29.FUKY*/ wire FUKY_LY2n = not1(reg_ly.LEXA_LY2p.qp_new());
  /* p29.FUVE*/ wire FUVE_LY3n = not1(reg_ly.LYDO_LY3p.qp_new());

  /* p29.ERUC*/ auto _ERUC_YDIFF0 = add3(EBOS_LY0n, oam_temp_a.XUSO_OAM_DA0p.qp_new(), SIG_GND);
  /* p29.ENEF*/ auto _ENEF_YDIFF1 = add3(DASA_LY1n, oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF0.carry);
  /* p29.FECO*/ auto _FECO_YDIFF2 = add3(FUKY_LY2n, oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF1.carry);
  /* p29.GYKY*/ auto _GYKY_YDIFF3 = add3(FUVE_LY3n, oam_temp_a.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF2.carry);

  /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0_new = not1(_ERUC_YDIFF0.sum);
  /* p29.DABY*/ wire _DABY_SPRITE_DELTA1_new = not1(_ENEF_YDIFF1.sum);
  /* p29.DABU*/ wire _DABU_SPRITE_DELTA2_new = not1(_FECO_YDIFF2.sum);
  /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3_new = not1(_GYKY_YDIFF3.sum);

  /*#p30.CUCU_STORE_L0*/ BUS_SPR_L0.tri6_nn(FEPO_STORE_MATCHp, _DEGE_SPRITE_DELTA0_new);
  /*#p30.CUCA_STORE_L1*/ BUS_SPR_L1.tri6_nn(FEPO_STORE_MATCHp, _DABY_SPRITE_DELTA1_new);
  /*#p30.CEGA_STORE_L2*/ BUS_SPR_L2.tri6_nn(FEPO_STORE_MATCHp, _DABU_SPRITE_DELTA2_new);
  /*#p30.WENU_STORE_L3*/ BUS_SPR_L3.tri6_nn(FEPO_STORE_MATCHp, _GYSA_SPRITE_DELTA3_new);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySpriteStore::dump(Dumper& d) {
  //d.dump_slice2p("SPRITE INDEX   : ", &XADU_SPRITE_IDX0p.state, 6);
  d.dump_slice2p("SPRITE COUNT   : ", &BESE_SPRITE_COUNT0, 4);
  d.dump_bitp   ("DEZY_STORE_ENn : ", DEZY_COUNT_CLKp.state);
  d("\n");
  d             ("STORE0 R%d I%02d L%02d X%03d\n", EBOJ_STORE0_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &YGUS_STORE0_I0n_odd), BitBase::pack_oldn(4, &GYHO_STORE0_L0n_odd), BitBase::pack_oldn(8, &XEPE_STORE0_X0p_odd));
  d             ("STORE1 R%d I%02d L%02d X%03d\n", CEDY_STORE1_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &CADU_STORE1_I0n_odd), BitBase::pack_oldn(4, &AMES_STORE1_L0n_odd), BitBase::pack_oldn(8, &DANY_STORE1_X0p_odd));
  d             ("STORE2 R%d I%02d L%02d X%03d\n", EGAV_STORE2_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &BUHE_STORE2_I0n_odd), BitBase::pack_oldn(4, &YLOV_STORE2_L0n_odd), BitBase::pack_oldn(8, &FOKA_STORE2_X0p_odd));
  d             ("STORE3 R%d I%02d L%02d X%03d\n", GOTA_STORE3_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &DEVY_STORE3_I0n_odd), BitBase::pack_oldn(4, &ZURO_STORE3_L0n_odd), BitBase::pack_oldn(8, &XOLY_STORE3_X0p_odd));
  d             ("STORE4 R%d I%02d L%02d X%03d\n", XUDY_STORE4_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &XAVE_STORE4_I0n_odd), BitBase::pack_oldn(4, &CAPO_STORE4_L0n_odd), BitBase::pack_oldn(8, &WEDU_STORE4_X0p_odd));
  d             ("STORE5 R%d I%02d L%02d X%03d\n", WAFY_STORE5_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &EKOP_STORE5_I0n_odd), BitBase::pack_oldn(4, &ACEP_STORE5_L0n_odd), BitBase::pack_oldn(8, &FUSA_STORE5_X0p_odd));
  d             ("STORE6 R%d I%02d L%02d X%03d\n", WOMY_STORE6_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &GABO_STORE6_I0n_odd), BitBase::pack_oldn(4, &ZUMY_STORE6_L0n_odd), BitBase::pack_oldn(8, &YCOL_STORE6_X0p_odd));
  d             ("STORE7 R%d I%02d L%02d X%03d\n", WAPO_STORE7_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &GULE_STORE7_I0n_odd), BitBase::pack_oldn(4, &XYNA_STORE7_L0n_odd), BitBase::pack_oldn(8, &ERAZ_STORE7_X0p_odd));
  d             ("STORE8 R%d I%02d L%02d X%03d\n", EXUQ_STORE8_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &AXUV_STORE8_I0n_odd), BitBase::pack_oldn(4, &AZAP_STORE8_L0n_odd), BitBase::pack_oldn(8, &EZUF_STORE8_X0p_odd));
  d             ("STORE9 R%d I%02d L%02d X%03d\n", FONO_STORE9_RSTp_evn.qp_old(), BitBase::pack_oldn(6, &YBER_STORE9_I0n_odd), BitBase::pack_oldn(4, &CANA_STORE9_L0n_odd), BitBase::pack_oldn(8, &XUVY_STORE9_X0p_odd));
}

//------------------------------------------------------------------------------------------------------------------------
