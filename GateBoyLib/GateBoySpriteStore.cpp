#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoyPixPipe.h"
#include "GateBoyLib/GateBoyOamBus.h"
#include "GateBoyLib/GateBoyLCD.h"

//----------------------------------------

void SpriteCounter::update_count(wire XAPO_VID_RSTn, wire ZEME_AxCxExGx, wire ATEJ_LINE_RSTp, wire _DYTY_STORE_CLKp_new_odd) {
  // Sprite store counter. The sprite count clock stops ticking once we have 10 sprites.
  // Sprite Y matcher. This is using an adder as a subtracter by inverting the first input.

  /*#p28.AZYB*/ wire _AZYB_LINE_TRIGn = not1(ATEJ_LINE_RSTp);
  /* p29.BESE*/ BESE_SPRITE_COUNT0.RSTn(_AZYB_LINE_TRIGn);
  /* p29.CUXY*/ CUXY_SPRITE_COUNT1.RSTn(_AZYB_LINE_TRIGn);
  /* p29.BEGO*/ BEGO_SPRITE_COUNT2.RSTn(_AZYB_LINE_TRIGn);
  /* p29.DYBE*/ DYBE_SPRITE_COUNT3.RSTn(_AZYB_LINE_TRIGn);

  /*#p29.BAKY*/ wire _BAKY_SPRITES_FULL_new = and2(CUXY_SPRITE_COUNT1.qp_new(), DYBE_SPRITE_COUNT3.qp_new());

  /* p29.DEZY*/ DEZY_COUNT_CLKp.dff17(ZEME_AxCxExGx, XAPO_VID_RSTn, _DYTY_STORE_CLKp_new_odd);
  /*#p29.CAKE*/ wire _CAKE_COUNT_CLKp_new = or2(_BAKY_SPRITES_FULL_new, DEZY_COUNT_CLKp.qp_new());

  /* p29.BESE*/ BESE_SPRITE_COUNT0.dff17(_CAKE_COUNT_CLKp_new,            _AZYB_LINE_TRIGn, BESE_SPRITE_COUNT0.qn_new());
  /* p29.CUXY*/ CUXY_SPRITE_COUNT1.dff17(BESE_SPRITE_COUNT0.qn_new(), _AZYB_LINE_TRIGn, CUXY_SPRITE_COUNT1.qn_new());
  /* p29.BEGO*/ BEGO_SPRITE_COUNT2.dff17(CUXY_SPRITE_COUNT1.qn_new(), _AZYB_LINE_TRIGn, BEGO_SPRITE_COUNT2.qn_new());
  /* p29.DYBE*/ DYBE_SPRITE_COUNT3.dff17(BEGO_SPRITE_COUNT2.qn_new(), _AZYB_LINE_TRIGn, DYBE_SPRITE_COUNT3.qn_new());
}

//----------------------------------------

SpriteStoreFlag SpriteCounter::get_store_flag(wire _DYTY_STORE_CLKp_new_odd) {
  /* p29.EDEN*/ wire _EDEN_SPRITE_COUNT0n_new = not1(BESE_SPRITE_COUNT0.qp_any());
  /* p29.CYPY*/ wire _CYPY_SPRITE_COUNT1n_new = not1(CUXY_SPRITE_COUNT1.qp_any());
  /* p29.CAPE*/ wire _CAPE_SPRITE_COUNT2n_new = not1(BEGO_SPRITE_COUNT2.qp_any());
  /* p29.CAXU*/ wire _CAXU_SPRITE_COUNT3n_new = not1(DYBE_SPRITE_COUNT3.qp_any());

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

  /* p29.CEMY*/ wire _CEMY_STORE0_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _DEZO_STORE0_SELn_new);
  /* p29.BYBY*/ wire _BYBY_STORE1_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _CUVA_STORE1_SELn_new);
  /* p29.WYXO*/ wire _WYXO_STORE2_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _GEBU_STORE2_SELn_new);
  /* p29.GUVE*/ wire _GUVE_STORE3_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _FOCO_STORE3_SELn_new);
  /* p29.CECU*/ wire _CECU_STORE4_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _CUPE_STORE4_SELn_new);
  /* p29.CADO*/ wire _CADO_STORE5_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _CUGU_STORE5_SELn_new);
  /* p29.XUJO*/ wire _XUJO_STORE6_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _WOMU_STORE6_SELn_new);
  /* p29.GAPE*/ wire _GAPE_STORE7_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _GUNA_STORE7_SELn_new);
  /* p29.CAHO*/ wire _CAHO_STORE8_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _DEWY_STORE8_SELn_new);
  /* p29.CATO*/ wire _CATO_STORE9_CLKp_new_odd = or2(_DYTY_STORE_CLKp_new_odd, _DOGU_STORE9_SELn_new);

  /* p29.DYHU*/ wire _DYHU_STORE0_CLKn_new_odd = not1(_CEMY_STORE0_CLKp_new_odd);
  /* p29.BUCO*/ wire _BUCO_STORE1_CLKn_new_odd = not1(_BYBY_STORE1_CLKp_new_odd);
  /* p29.GYFO*/ wire _GYFO_STORE2_CLKn_new_odd = not1(_WYXO_STORE2_CLKp_new_odd);
  /* p29.GUSA*/ wire _GUSA_STORE3_CLKn_new_odd = not1(_GUVE_STORE3_CLKp_new_odd);
  /* p29.DUKE*/ wire _DUKE_STORE4_CLKn_new_odd = not1(_CECU_STORE4_CLKp_new_odd);
  /* p29.BEDE*/ wire _BEDE_STORE5_CLKn_new_odd = not1(_CADO_STORE5_CLKp_new_odd);
  /* p29.WEKA*/ wire _WEKA_STORE6_CLKn_new_odd = not1(_XUJO_STORE6_CLKp_new_odd);
  /* p29.GYVO*/ wire _GYVO_STORE7_CLKn_new_odd = not1(_GAPE_STORE7_CLKp_new_odd);
  /* p29.BUKA*/ wire _BUKA_STORE8_CLKn_new_odd = not1(_CAHO_STORE8_CLKp_new_odd);
  /* p29.DECU*/ wire _DECU_STORE9_CLKn_new_odd = not1(_CATO_STORE9_CLKp_new_odd);

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

//----------------------------------------

SpriteMatch SpriteStore::get_match(wire _AROR_MATCH_ENp_new, const PixCounter& _pix_count) {
  /*#p21.ACAM*/ wire ACAM_PX0n_new_evn = not1(_pix_count.XEHO_PX0p.qp_any());
  /* p21.AZUB*/ wire AZUB_PX1n_new_evn = not1(_pix_count.SAVY_PX1p.qp_any());
  /* p21.AMEL*/ wire AMEL_PX2n_new_evn = not1(_pix_count.XODU_PX2p.qp_any());
  /* p21.AHAL*/ wire AHAL_PX3n_new_evn = not1(_pix_count.XYDO_PX3p.qp_any());
  /* p21.APUX*/ wire APUX_PX4n_new_evn = not1(_pix_count.TUHU_PX4p.qp_any());
  /* p21.ABEF*/ wire ABEF_PX5n_new_evn = not1(_pix_count.TUKY_PX5p.qp_any());
  /* p21.ADAZ*/ wire ADAZ_PX6n_new_evn = not1(_pix_count.TAKO_PX6p.qp_any());
  /* p21.ASAH*/ wire ASAH_PX7n_new_evn = not1(_pix_count.SYBE_PX7p.qp_any());

  /*#p31.ZOGY*/ wire _ZOGY_STORE0_MATCH0n_new = xor2(XEPE_STORE0_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.ZEBA*/ wire _ZEBA_STORE0_MATCH1n_new = xor2(YLAH_STORE0_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.ZAHA*/ wire _ZAHA_STORE0_MATCH2n_new = xor2(ZOLA_STORE0_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.ZOKY*/ wire _ZOKY_STORE0_MATCH3n_new = xor2(ZULU_STORE0_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.WOJU*/ wire _WOJU_STORE0_MATCH4n_new = xor2(WELO_STORE0_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.YFUN*/ wire _YFUN_STORE0_MATCH5n_new = xor2(XUNY_STORE0_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.WYZA*/ wire _WYZA_STORE0_MATCH6n_new = xor2(WOTE_STORE0_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.YPUK*/ wire _YPUK_STORE0_MATCH7n_new = xor2(XAKO_STORE0_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

  /* p31.EDYM*/ wire _EDYM_STORE1_MATCH0n_new = xor2(DANY_STORE1_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.EMYB*/ wire _EMYB_STORE1_MATCH1n_new = xor2(DUKO_STORE1_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.EBEF*/ wire _EBEF_STORE1_MATCH2n_new = xor2(DESU_STORE1_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.EWOK*/ wire _EWOK_STORE1_MATCH3n_new = xor2(DAZO_STORE1_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.COLA*/ wire _COLA_STORE1_MATCH4n_new = xor2(DAKE_STORE1_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.BOBA*/ wire _BOBA_STORE1_MATCH5n_new = xor2(CESO_STORE1_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.COLU*/ wire _COLU_STORE1_MATCH6n_new = xor2(DYFU_STORE1_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.BAHU*/ wire _BAHU_STORE1_MATCH7n_new = xor2(CUSY_STORE1_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

  /* p31.FUZU*/ wire _FUZU_STORE2_MATCH0n_new = xor2(FOKA_STORE2_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.FESO*/ wire _FESO_STORE2_MATCH1n_new = xor2(FYTY_STORE2_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.FOKY*/ wire _FOKY_STORE2_MATCH2n_new = xor2(FUBY_STORE2_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.FYVA*/ wire _FYVA_STORE2_MATCH3n_new = xor2(GOXU_STORE2_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.CEKO*/ wire _CEKO_STORE2_MATCH4n_new = xor2(DUHY_STORE2_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.DETY*/ wire _DETY_STORE2_MATCH5n_new = xor2(EJUF_STORE2_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.DOZO*/ wire _DOZO_STORE2_MATCH6n_new = xor2(ENOR_STORE2_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.CONY*/ wire _CONY_STORE2_MATCH7n_new = xor2(DEPY_STORE2_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

  /* p31.YHOK*/ wire _YHOK_STORE3_MATCH0n_new = xor2(XOLY_STORE3_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.YCAH*/ wire _YCAH_STORE3_MATCH1n_new = xor2(XYBA_STORE3_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.YDAJ*/ wire _YDAJ_STORE3_MATCH2n_new = xor2(XABE_STORE3_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.YVUZ*/ wire _YVUZ_STORE3_MATCH3n_new = xor2(XEKA_STORE3_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.YVAP*/ wire _YVAP_STORE3_MATCH4n_new = xor2(XOMY_STORE3_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.XENY*/ wire _XENY_STORE3_MATCH5n_new = xor2(WUHA_STORE3_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.XAVU*/ wire _XAVU_STORE3_MATCH6n_new = xor2(WYNA_STORE3_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.XEVA*/ wire _XEVA_STORE3_MATCH7n_new = xor2(WECO_STORE3_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

  /* p31.XEJU*/ wire _XEJU_STORE4_MATCH0n_new = xor2(WEDU_STORE4_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.ZATE*/ wire _ZATE_STORE4_MATCH1n_new = xor2(YGAJ_STORE4_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.ZAKU*/ wire _ZAKU_STORE4_MATCH2n_new = xor2(ZYJO_STORE4_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.YBOX*/ wire _YBOX_STORE4_MATCH3n_new = xor2(XURY_STORE4_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.ZYKU*/ wire _ZYKU_STORE4_MATCH4n_new = xor2(YBED_STORE4_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.ZYPU*/ wire _ZYPU_STORE4_MATCH5n_new = xor2(ZALA_STORE4_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.XAHA*/ wire _XAHA_STORE4_MATCH6n_new = xor2(WYDE_STORE4_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.ZEFE*/ wire _ZEFE_STORE4_MATCH7n_new = xor2(XEPA_STORE4_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

  /* p31.GUZO*/ wire _GUZO_STORE5_MATCH0n_new = xor2(FUSA_STORE5_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.GOLA*/ wire _GOLA_STORE5_MATCH1n_new = xor2(FAXA_STORE5_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.GEVE*/ wire _GEVE_STORE5_MATCH2n_new = xor2(FOZY_STORE5_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.GUDE*/ wire _GUDE_STORE5_MATCH3n_new = xor2(FESY_STORE5_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.BAZY*/ wire _BAZY_STORE5_MATCH4n_new = xor2(CYWE_STORE5_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.CYLE*/ wire _CYLE_STORE5_MATCH5n_new = xor2(DYBY_STORE5_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.CEVA*/ wire _CEVA_STORE5_MATCH6n_new = xor2(DURY_STORE5_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.BUMY*/ wire _BUMY_STORE5_MATCH7n_new = xor2(CUVY_STORE5_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

  /* p31.XOSU*/ wire _XOSU_STORE6_MATCH0n_new = xor2(YCOL_STORE6_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.ZUVU*/ wire _ZUVU_STORE6_MATCH1n_new = xor2(YRAC_STORE6_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.XUCO*/ wire _XUCO_STORE6_MATCH2n_new = xor2(YMEM_STORE6_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.ZULO*/ wire _ZULO_STORE6_MATCH3n_new = xor2(YVAG_STORE6_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.ZARE*/ wire _ZARE_STORE6_MATCH4n_new = xor2(ZOLY_STORE6_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.ZEMU*/ wire _ZEMU_STORE6_MATCH5n_new = xor2(ZOGO_STORE6_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.ZYGO*/ wire _ZYGO_STORE6_MATCH6n_new = xor2(ZECU_STORE6_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.ZUZY*/ wire _ZUZY_STORE6_MATCH7n_new = xor2(ZESA_STORE6_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

  /* p31.DUSE*/ wire _DUSE_STORE7_MATCH0n_new = xor2(ERAZ_STORE7_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.DAGU*/ wire _DAGU_STORE7_MATCH1n_new = xor2(EPUM_STORE7_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.DYZE*/ wire _DYZE_STORE7_MATCH2n_new = xor2(EROL_STORE7_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.DESO*/ wire _DESO_STORE7_MATCH3n_new = xor2(EHYN_STORE7_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.EJOT*/ wire _EJOT_STORE7_MATCH4n_new = xor2(FAZU_STORE7_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.ESAJ*/ wire _ESAJ_STORE7_MATCH5n_new = xor2(FAXE_STORE7_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.DUCU*/ wire _DUCU_STORE7_MATCH6n_new = xor2(EXUK_STORE7_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.EWUD*/ wire _EWUD_STORE7_MATCH7n_new = xor2(FEDE_STORE7_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

  /* p31.DUZE*/ wire _DUZE_STORE8_MATCH0n_new = xor2(EZUF_STORE8_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.DAGA*/ wire _DAGA_STORE8_MATCH1n_new = xor2(ENAD_STORE8_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.DAWU*/ wire _DAWU_STORE8_MATCH2n_new = xor2(EBOW_STORE8_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.EJAW*/ wire _EJAW_STORE8_MATCH3n_new = xor2(FYCA_STORE8_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.GOHO*/ wire _GOHO_STORE8_MATCH4n_new = xor2(GAVY_STORE8_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.GASU*/ wire _GASU_STORE8_MATCH5n_new = xor2(GYPU_STORE8_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.GABU*/ wire _GABU_STORE8_MATCH6n_new = xor2(GADY_STORE8_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.GAFE*/ wire _GAFE_STORE8_MATCH7n_new = xor2(GAZA_STORE8_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

  /* p31.YMAM*/ wire _YMAM_STORE9_MATCH0n_new = xor2(XUVY_STORE9_X0p_odd.qn_any(), ACAM_PX0n_new_evn);
  /* p31.YTYP*/ wire _YTYP_STORE9_MATCH1n_new = xor2(XERE_STORE9_X1p_odd.qn_any(), AZUB_PX1n_new_evn);
  /* p31.YFOP*/ wire _YFOP_STORE9_MATCH2n_new = xor2(XUZO_STORE9_X2p_odd.qn_any(), AMEL_PX2n_new_evn);
  /* p31.YVAC*/ wire _YVAC_STORE9_MATCH3n_new = xor2(XEXA_STORE9_X3p_odd.qn_any(), AHAL_PX3n_new_evn);
  /* p31.ZYWU*/ wire _ZYWU_STORE9_MATCH4n_new = xor2(YPOD_STORE9_X4p_odd.qn_any(), APUX_PX4n_new_evn);
  /* p31.ZUZA*/ wire _ZUZA_STORE9_MATCH5n_new = xor2(YROP_STORE9_X5p_odd.qn_any(), ABEF_PX5n_new_evn);
  /* p31.ZEJO*/ wire _ZEJO_STORE9_MATCH6n_new = xor2(YNEP_STORE9_X6p_odd.qn_any(), ADAZ_PX6n_new_evn);
  /* p31.ZEDA*/ wire _ZEDA_STORE9_MATCH7n_new = xor2(YZOF_STORE9_X7p_odd.qn_any(), ASAH_PX7n_new_evn);

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

  /* p29.YDUG*/ wire _YDUG_STORE0_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _ZAKO_STORE0_MATCHAp_new, _XEBA_STORE0_MATCHBp_new);
  /* p29.DYDU*/ wire _DYDU_STORE1_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _EWAM_STORE1_MATCHAp_new, _CYVY_STORE1_MATCHBp_new);
  /* p29.DEGO*/ wire _DEGO_STORE2_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _CEHU_STORE2_MATCHAp_new, _EKES_STORE2_MATCHBp_new);
  /* p29.YLOZ*/ wire _YLOZ_STORE3_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _ZURE_STORE3_MATCHAp_new, _YWOS_STORE3_MATCHBp_new);
  /* p29.XAGE*/ wire _XAGE_STORE4_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _YKOK_STORE4_MATCHAp_new, _YNAZ_STORE4_MATCHBp_new);
  /* p29.EGOM*/ wire _EGOM_STORE5_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _COGY_STORE5_MATCHAp_new, _FYMA_STORE5_MATCHBp_new);
  /* p29.YBEZ*/ wire _YBEZ_STORE6_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _YWAP_STORE6_MATCHAp_new, _YDOT_STORE6_MATCHBp_new);
  /* p29.DYKA*/ wire _DYKA_STORE7_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _CYCO_STORE7_MATCHAp_new, _DAJE_STORE7_MATCHBp_new);
  /* p29.EFYL*/ wire _EFYL_STORE8_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _DAMA_STORE8_MATCHAp_new, _FEHA_STORE8_MATCHBp_new);
  /* p29.YGEM*/ wire _YGEM_STORE9_MATCHn_new_evn = nand3(_AROR_MATCH_ENp_new, _YLEV_STORE9_MATCHAp_new, _YTUB_STORE9_MATCHBp_new);

  return {
    _YDUG_STORE0_MATCHn_new_evn,
    _DYDU_STORE1_MATCHn_new_evn,
    _DEGO_STORE2_MATCHn_new_evn,
    _YLOZ_STORE3_MATCHn_new_evn,
    _XAGE_STORE4_MATCHn_new_evn,
    _EGOM_STORE5_MATCHn_new_evn,
    _YBEZ_STORE6_MATCHn_new_evn,
    _DYKA_STORE7_MATCHn_new_evn,
    _EFYL_STORE8_MATCHn_new_evn,
    _YGEM_STORE9_MATCHn_new_evn
  };
}

void SpriteStore::get_sprite(SpriteMatch sprite_match) {
  SpriteGetFlag sprite_flag = sprite_match.get_flag();

  /* p29.FURO*/ wire _FURO_SPRITE0_GETn_new_evn = not1(sprite_flag.GUVA_SPRITE0_GETp);
  /* p29.DYDO*/ wire _DYDO_SPRITE1_GETn_new_evn = not1(sprite_flag.ENUT_SPRITE1_GETp);
  /* p29.FAME*/ wire _FAME_SPRITE2_GETn_new_evn = not1(sprite_flag.EMOL_SPRITE2_GETp);
  /* p29.GYMA*/ wire _GYMA_SPRITE3_GETn_new_evn = not1(sprite_flag.GYFY_SPRITE3_GETp);
  /* p29.GOWO*/ wire _GOWO_SPRITE4_GETn_new_evn = not1(sprite_flag.GONO_SPRITE4_GETp);
  /* p29.GYGY*/ wire _GYGY_SPRITE5_GETn_new_evn = not1(sprite_flag.GEGA_SPRITE5_GETp);
  /* p29.XYME*/ wire _XYME_SPRITE6_GETn_new_evn = not1(sprite_flag.XOJA_SPRITE6_GETp);
  /* p29.GUGY*/ wire _GUGY_SPRITE7_GETn_new_evn = not1(sprite_flag.GUTU_SPRITE7_GETp);
  /* p29.DENY*/ wire _DENY_SPRITE8_GETn_new_evn = not1(sprite_flag.FOXA_SPRITE8_GETp);
  /* p29.FADO*/ wire _FADO_SPRITE9_GETn_new_evn = not1(sprite_flag.GUZE_SPRITE9_GETp);

  // Sprite store output tribuf driver. The store regs are set during scan, so it's safe to read the old values here.

  /* p30.ZETU*/ SPR_TRI_I[0].tri6_nn(_FURO_SPRITE0_GETn_new_evn, YGUS_STORE0_I0n_odd.qp_old());
  /* p30.ZECE*/ SPR_TRI_I[1].tri6_nn(_FURO_SPRITE0_GETn_new_evn, YSOK_STORE0_I1n_odd.qp_old());
  /* p30.ZAVE*/ SPR_TRI_I[2].tri6_nn(_FURO_SPRITE0_GETn_new_evn, YZEP_STORE0_I2n_odd.qp_old());
  /* p30.WOKO*/ SPR_TRI_I[3].tri6_nn(_FURO_SPRITE0_GETn_new_evn, WYTE_STORE0_I3n_odd.qp_old());
  /* p30.ZUMU*/ SPR_TRI_I[4].tri6_nn(_FURO_SPRITE0_GETn_new_evn, ZONY_STORE0_I4n_odd.qp_old());
  /*#p30.ZEDY*/ SPR_TRI_I[5].tri6_nn(_FURO_SPRITE0_GETn_new_evn, YWAK_STORE0_I5n_odd.qp_old());
  /* p30.WEHE*/ SPR_TRI_L[0].tri6_nn(_FURO_SPRITE0_GETn_new_evn, GYHO_STORE0_L0n_odd.qp_old());
  /* p30.BUKY*/ SPR_TRI_L[1].tri6_nn(_FURO_SPRITE0_GETn_new_evn, CUFO_STORE0_L1n_odd.qp_old());
  /* p30.AJAL*/ SPR_TRI_L[2].tri6_nn(_FURO_SPRITE0_GETn_new_evn, BOZU_STORE0_L2n_odd.qp_old());
  /* p30.GOFO*/ SPR_TRI_L[3].tri6_nn(_FURO_SPRITE0_GETn_new_evn, FYHY_STORE0_L3n_odd.qp_old());

  /*#p30.CUBO*/ SPR_TRI_I[0].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CADU_STORE1_I0n_odd.qp_old());
  /* p30.CELU*/ SPR_TRI_I[1].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CEBO_STORE1_I1n_odd.qp_old());
  /* p30.CEGY*/ SPR_TRI_I[2].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CUFA_STORE1_I2n_odd.qp_old());
  /* p30.BETY*/ SPR_TRI_I[3].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, COMA_STORE1_I3n_odd.qp_old());
  /* p30.CYBY*/ SPR_TRI_I[4].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CUZA_STORE1_I4n_odd.qp_old());
  /* p30.BEMO*/ SPR_TRI_I[5].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, CAJY_STORE1_I5n_odd.qp_old());
  /* p30.BYRO*/ SPR_TRI_L[0].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, AMES_STORE1_L0n_odd.qp_old());
  /* p30.AHUM*/ SPR_TRI_L[1].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, AROF_STORE1_L1n_odd.qp_old());
  /* p30.BACO*/ SPR_TRI_L[2].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, ABOP_STORE1_L2n_odd.qp_old());
  /* p30.BEFE*/ SPR_TRI_L[3].tri6_nn(_DYDO_SPRITE1_GETn_new_evn, ABUG_STORE1_L3n_odd.qp_old());

  /* p30.CUBE*/ SPR_TRI_I[0].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BUHE_STORE2_I0n_odd.qp_old());
  /* p30.AFOZ*/ SPR_TRI_I[1].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BYHU_STORE2_I1n_odd.qp_old());
  /* p30.APON*/ SPR_TRI_I[2].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BECA_STORE2_I2n_odd.qp_old());
  /* p30.CUVU*/ SPR_TRI_I[3].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BULU_STORE2_I3n_odd.qp_old());
  /* p30.CYRO*/ SPR_TRI_I[4].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BUNA_STORE2_I4n_odd.qp_old());
  /* p30.AXEC*/ SPR_TRI_I[5].tri6_nn(_FAME_SPRITE2_GETn_new_evn, BOXA_STORE2_I5n_odd.qp_old());
  /* p30.ZUKE*/ SPR_TRI_L[0].tri6_nn(_FAME_SPRITE2_GETn_new_evn, YLOV_STORE2_L0n_odd.qp_old());
  /* p30.WERE*/ SPR_TRI_L[1].tri6_nn(_FAME_SPRITE2_GETn_new_evn, XOSY_STORE2_L1n_odd.qp_old());
  /* p30.WUXE*/ SPR_TRI_L[2].tri6_nn(_FAME_SPRITE2_GETn_new_evn, XAZY_STORE2_L2n_odd.qp_old());
  /* p30.ZABY*/ SPR_TRI_L[3].tri6_nn(_FAME_SPRITE2_GETn_new_evn, YKUK_STORE2_L3n_odd.qp_old());

  /* p30.ENAP*/ SPR_TRI_I[0].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DEVY_STORE3_I0n_odd.qp_old());
  /* p30.DYGO*/ SPR_TRI_I[1].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DESE_STORE3_I1n_odd.qp_old());
  /* p30.DOWA*/ SPR_TRI_I[2].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DUNY_STORE3_I2n_odd.qp_old());
  /* p30.DONY*/ SPR_TRI_I[3].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DUHA_STORE3_I3n_odd.qp_old());
  /* p30.EFUD*/ SPR_TRI_I[4].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DEBA_STORE3_I4n_odd.qp_old());
  /* p30.DEZU*/ SPR_TRI_I[5].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, DAFU_STORE3_I5n_odd.qp_old());
  /* p30.ZEXE*/ SPR_TRI_L[0].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, ZURO_STORE3_L0n_odd.qp_old());
  /* p30.YWAV*/ SPR_TRI_L[1].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, ZYLU_STORE3_L1n_odd.qp_old());
  /* p30.YJEM*/ SPR_TRI_L[2].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, ZENE_STORE3_L2n_odd.qp_old());
  /* p30.ZYPO*/ SPR_TRI_L[3].tri6_nn(_GYMA_SPRITE3_GETn_new_evn, ZURY_STORE3_L3n_odd.qp_old());

  /* p30.WUXU*/ SPR_TRI_I[0].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XAVE_STORE4_I0n_odd.qp_old());
  /* p30.WEPY*/ SPR_TRI_I[1].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XEFE_STORE4_I1n_odd.qp_old());
  /* p30.WERU*/ SPR_TRI_I[2].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, WANU_STORE4_I2n_odd.qp_old());
  /* p30.XYRE*/ SPR_TRI_I[3].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XABO_STORE4_I3n_odd.qp_old());
  /* p30.WOXY*/ SPR_TRI_I[4].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XEGE_STORE4_I4n_odd.qp_old());
  /* p30.WAJA*/ SPR_TRI_I[5].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, XYNU_STORE4_I5n_odd.qp_old());
  /* p30.BUCE*/ SPR_TRI_L[0].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, CAPO_STORE4_L0n_odd.qp_old());
  /* p30.BEVY*/ SPR_TRI_L[1].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, CAJU_STORE4_L1n_odd.qp_old());
  /* p30.BOVE*/ SPR_TRI_L[2].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, CONO_STORE4_L2n_odd.qp_old());
  /* p30.BYDO*/ SPR_TRI_L[3].tri6_nn(_GOWO_SPRITE4_GETn_new_evn, CUMU_STORE4_L3n_odd.qp_old());

  /* p30.DOBO*/ SPR_TRI_I[0].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, EKOP_STORE5_I0n_odd.qp_old());
  /* p30.DYNY*/ SPR_TRI_I[1].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ETYM_STORE5_I1n_odd.qp_old());
  /* p30.WAGA*/ SPR_TRI_I[2].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, GORU_STORE5_I2n_odd.qp_old());
  /* p30.DUZA*/ SPR_TRI_I[3].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, EBEX_STORE5_I3n_odd.qp_old());
  /* p30.DALY*/ SPR_TRI_I[4].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ETAV_STORE5_I4n_odd.qp_old());
  /* p30.DALO*/ SPR_TRI_I[5].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, EKAP_STORE5_I5n_odd.qp_old());
  /* p30.BACE*/ SPR_TRI_L[0].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ACEP_STORE5_L0n_odd.qp_old());
  /* p30.BUJA*/ SPR_TRI_L[1].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ABEG_STORE5_L1n_odd.qp_old());
  /* p30.BODU*/ SPR_TRI_L[2].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ABUX_STORE5_L2n_odd.qp_old());
  /* p30.AWAT*/ SPR_TRI_L[3].tri6_nn(_GYGY_SPRITE5_GETn_new_evn, ANED_STORE5_L3n_odd.qp_old());

  /* p30.WATO*/ SPR_TRI_I[0].tri6_nn(_XYME_SPRITE6_GETn_new_evn, GABO_STORE6_I0n_odd.qp_old());
  /* p30.WYWY*/ SPR_TRI_I[1].tri6_nn(_XYME_SPRITE6_GETn_new_evn, GACY_STORE6_I1n_odd.qp_old());
  /* p30.EZOC*/ SPR_TRI_I[2].tri6_nn(_XYME_SPRITE6_GETn_new_evn, FOGO_STORE6_I2n_odd.qp_old());
  /* p30.WABO*/ SPR_TRI_I[3].tri6_nn(_XYME_SPRITE6_GETn_new_evn, GOHU_STORE6_I3n_odd.qp_old());
  /* p30.ELYC*/ SPR_TRI_I[4].tri6_nn(_XYME_SPRITE6_GETn_new_evn, FOXY_STORE6_I4n_odd.qp_old());
  /* p30.WOCY*/ SPR_TRI_I[5].tri6_nn(_XYME_SPRITE6_GETn_new_evn, GECU_STORE6_I5n_odd.qp_old());
  /* p30.YBUK*/ SPR_TRI_L[0].tri6_nn(_XYME_SPRITE6_GETn_new_evn, ZUMY_STORE6_L0n_odd.qp_old());
  /* p30.YKOZ*/ SPR_TRI_L[1].tri6_nn(_XYME_SPRITE6_GETn_new_evn, ZAFU_STORE6_L1n_odd.qp_old());
  /* p30.ZYTO*/ SPR_TRI_L[2].tri6_nn(_XYME_SPRITE6_GETn_new_evn, ZEXO_STORE6_L2n_odd.qp_old());
  /* p30.ZUDO*/ SPR_TRI_L[3].tri6_nn(_XYME_SPRITE6_GETn_new_evn, ZUBE_STORE6_L3n_odd.qp_old());

  /* p30.WAKO*/ SPR_TRI_I[0].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, GULE_STORE7_I0n_odd.qp_old());
  /* p30.WYGO*/ SPR_TRI_I[1].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, GYNO_STORE7_I1n_odd.qp_old());
  /* p30.ELEP*/ SPR_TRI_I[2].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, FEFA_STORE7_I2n_odd.qp_old());
  /* p30.ETAD*/ SPR_TRI_I[3].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, FYSU_STORE7_I3n_odd.qp_old());
  /* p30.WABA*/ SPR_TRI_I[4].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, GESY_STORE7_I4n_odd.qp_old());
  /* p30.EVYT*/ SPR_TRI_I[5].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, FUZO_STORE7_I5n_odd.qp_old());
  /* p30.WAXE*/ SPR_TRI_L[0].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, XYNA_STORE7_L0n_odd.qp_old());
  /* p30.YPOZ*/ SPR_TRI_L[1].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, YGUM_STORE7_L1n_odd.qp_old());
  /* p30.WABU*/ SPR_TRI_L[2].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, XAKU_STORE7_L2n_odd.qp_old());
  /* p30.WANA*/ SPR_TRI_L[3].tri6_nn(_GUGY_SPRITE7_GETn_new_evn, XYGO_STORE7_L3n_odd.qp_old());

  /* p30.APOC*/ SPR_TRI_I[0].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AXUV_STORE8_I0n_odd.qp_old());
  /* p30.AKYH*/ SPR_TRI_I[1].tri6_nn(_DENY_SPRITE8_GETn_new_evn, BADA_STORE8_I1n_odd.qp_old());
  /* p30.AFEN*/ SPR_TRI_I[2].tri6_nn(_DENY_SPRITE8_GETn_new_evn, APEV_STORE8_I2n_odd.qp_old());
  /* p30.APYV*/ SPR_TRI_I[3].tri6_nn(_DENY_SPRITE8_GETn_new_evn, BADO_STORE8_I3n_odd.qp_old());
  /* p30.APOB*/ SPR_TRI_I[4].tri6_nn(_DENY_SPRITE8_GETn_new_evn, BEXY_STORE8_I4n_odd.qp_old());
  /* p30.ADYB*/ SPR_TRI_I[5].tri6_nn(_DENY_SPRITE8_GETn_new_evn, BYHE_STORE8_I5n_odd.qp_old());
  /* p30.BOSO*/ SPR_TRI_L[0].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AZAP_STORE8_L0n_odd.qp_old());
  /* p30.BAZU*/ SPR_TRI_L[1].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AFYX_STORE8_L1n_odd.qp_old());
  /* p30.AHAC*/ SPR_TRI_L[2].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AFUT_STORE8_L2n_odd.qp_old());
  /* p30.BUJY*/ SPR_TRI_L[3].tri6_nn(_DENY_SPRITE8_GETn_new_evn, AFYM_STORE8_L3n_odd.qp_old());

  /* p30.ZARO*/ SPR_TRI_I[0].tri6_nn(_FADO_SPRITE9_GETn_new_evn, YBER_STORE9_I0n_odd.qp_old());
  /* p30.ZOJY*/ SPR_TRI_I[1].tri6_nn(_FADO_SPRITE9_GETn_new_evn, YZOR_STORE9_I1n_odd.qp_old());
  /* p30.YNEV*/ SPR_TRI_I[2].tri6_nn(_FADO_SPRITE9_GETn_new_evn, XYFE_STORE9_I2n_odd.qp_old());
  /* p30.XYRA*/ SPR_TRI_I[3].tri6_nn(_FADO_SPRITE9_GETn_new_evn, XOTU_STORE9_I3n_odd.qp_old());
  /* p30.YRAD*/ SPR_TRI_I[4].tri6_nn(_FADO_SPRITE9_GETn_new_evn, XUTE_STORE9_I4n_odd.qp_old());
  /* p30.YHAL*/ SPR_TRI_I[5].tri6_nn(_FADO_SPRITE9_GETn_new_evn, XUFO_STORE9_I5n_odd.qp_old());
  /* p30.BYME*/ SPR_TRI_L[0].tri6_nn(_FADO_SPRITE9_GETn_new_evn, CANA_STORE9_L0n_odd.qp_old());
  /* p30.GATE*/ SPR_TRI_L[1].tri6_nn(_FADO_SPRITE9_GETn_new_evn, FOFO_STORE9_L1n_odd.qp_old());
  /* p30.COHO*/ SPR_TRI_L[2].tri6_nn(_FADO_SPRITE9_GETn_new_evn, DYSY_STORE9_L2n_odd.qp_old());
  /* p30.CAWO*/ SPR_TRI_L[3].tri6_nn(_FADO_SPRITE9_GETn_new_evn, DEWU_STORE9_L3n_odd.qp_old());
}

void SpriteStore::reset_matched_store(wire _ABAK_LINE_RSTp, wire WUTY_SFETCH_DONE_TRIGp, SpriteGetFlag sprite_flag) {
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

  /* p29.DYWE*/ wire _DYWE_STORE0_RSTp_new_evn = or2(_DYBA_LINE_RSTp, EBOJ_STORE0_RSTp_evn.qp_new());
  /* p29.EFEV*/ wire _EFEV_STORE1_RSTp_new_evn = or2(_DYBA_LINE_RSTp, CEDY_STORE1_RSTp_evn.qp_new());
  /* p29.FOKO*/ wire _FOKO_STORE2_RSTp_new_evn = or2(_DYBA_LINE_RSTp, EGAV_STORE2_RSTp_evn.qp_new());
  /* p29.GAKE*/ wire _GAKE_STORE3_RSTp_new_evn = or2(_DYBA_LINE_RSTp, GOTA_STORE3_RSTp_evn.qp_new());
  /* p29.WOHU*/ wire _WOHU_STORE4_RSTp_new_evn = or2(_DYBA_LINE_RSTp, XUDY_STORE4_RSTp_evn.qp_new());
  /* p29.FEVE*/ wire _FEVE_STORE5_RSTp_new_evn = or2(_DYBA_LINE_RSTp, WAFY_STORE5_RSTp_evn.qp_new());
  /* p29.WACY*/ wire _WACY_STORE6_RSTp_new_evn = or2(_DYBA_LINE_RSTp, WOMY_STORE6_RSTp_evn.qp_new());
  /* p29.GUKY*/ wire _GUKY_STORE7_RSTp_new_evn = or2(_DYBA_LINE_RSTp, WAPO_STORE7_RSTp_evn.qp_new());
  /* p29.GORO*/ wire _GORO_STORE8_RSTp_new_evn = or2(_DYBA_LINE_RSTp, EXUQ_STORE8_RSTp_evn.qp_new());
  /* p29.DUBU*/ wire _DUBU_STORE9_RSTp_new_evn = or2(_DYBA_LINE_RSTp, FONO_STORE9_RSTp_evn.qp_new());

  /* p29.DYNA*/ wire _DYNA_STORE0_RSTn_new_evn = not1(_DYWE_STORE0_RSTp_new_evn);
  /* p29.DOKU*/ wire _DOKU_STORE1_RSTn_new_evn = not1(_EFEV_STORE1_RSTp_new_evn);
  /* p29.GAMY*/ wire _GAMY_STORE2_RSTn_new_evn = not1(_FOKO_STORE2_RSTp_new_evn);
  /* p29.WUPA*/ wire _WUPA_STORE3_RSTn_new_evn = not1(_GAKE_STORE3_RSTp_new_evn);
  /* p29.WUNU*/ wire _WUNU_STORE4_RSTn_new_evn = not1(_WOHU_STORE4_RSTp_new_evn);
  /* p29.EJAD*/ wire _EJAD_STORE5_RSTn_new_evn = not1(_FEVE_STORE5_RSTp_new_evn);
  /* p29.XAHO*/ wire _XAHO_STORE6_RSTn_new_evn = not1(_WACY_STORE6_RSTp_new_evn);
  /* p29.GAFY*/ wire _GAFY_STORE7_RSTn_new_evn = not1(_GUKY_STORE7_RSTp_new_evn);
  /* p29.WUZO*/ wire _WUZO_STORE8_RSTn_new_evn = not1(_GORO_STORE8_RSTp_new_evn);
  /* p29.DOSY*/ wire _DOSY_STORE9_RSTn_new_evn = not1(_DUBU_STORE9_RSTp_new_evn);

  /*#p31.XEPE*/ XEPE_STORE0_X0p_odd.SETn(_DYNA_STORE0_RSTn_new_evn);
  /* p31.YLAH*/ YLAH_STORE0_X1p_odd.SETn(_DYNA_STORE0_RSTn_new_evn);
  /* p31.ZOLA*/ ZOLA_STORE0_X2p_odd.SETn(_DYNA_STORE0_RSTn_new_evn);
  /* p31.ZULU*/ ZULU_STORE0_X3p_odd.SETn(_DYNA_STORE0_RSTn_new_evn);
  /* p31.WELO*/ WELO_STORE0_X4p_odd.SETn(_DYNA_STORE0_RSTn_new_evn);
  /* p31.XUNY*/ XUNY_STORE0_X5p_odd.SETn(_DYNA_STORE0_RSTn_new_evn);
  /* p31.WOTE*/ WOTE_STORE0_X6p_odd.SETn(_DYNA_STORE0_RSTn_new_evn);
  /* p31.XAKO*/ XAKO_STORE0_X7p_odd.SETn(_DYNA_STORE0_RSTn_new_evn);

  /* p31.DANY*/ DANY_STORE1_X0p_odd.SETn(_DOKU_STORE1_RSTn_new_evn);
  /* p31.DUKO*/ DUKO_STORE1_X1p_odd.SETn(_DOKU_STORE1_RSTn_new_evn);
  /* p31.DESU*/ DESU_STORE1_X2p_odd.SETn(_DOKU_STORE1_RSTn_new_evn);
  /* p31.DAZO*/ DAZO_STORE1_X3p_odd.SETn(_DOKU_STORE1_RSTn_new_evn);
  /* p31.DAKE*/ DAKE_STORE1_X4p_odd.SETn(_DOKU_STORE1_RSTn_new_evn);
  /* p31.CESO*/ CESO_STORE1_X5p_odd.SETn(_DOKU_STORE1_RSTn_new_evn);
  /* p31.DYFU*/ DYFU_STORE1_X6p_odd.SETn(_DOKU_STORE1_RSTn_new_evn);
  /* p31.CUSY*/ CUSY_STORE1_X7p_odd.SETn(_DOKU_STORE1_RSTn_new_evn);

  /* p31.FOKA*/ FOKA_STORE2_X0p_odd.SETn(_GAMY_STORE2_RSTn_new_evn);
  /* p31.FYTY*/ FYTY_STORE2_X1p_odd.SETn(_GAMY_STORE2_RSTn_new_evn);
  /* p31.FUBY*/ FUBY_STORE2_X2p_odd.SETn(_GAMY_STORE2_RSTn_new_evn);
  /* p31.GOXU*/ GOXU_STORE2_X3p_odd.SETn(_GAMY_STORE2_RSTn_new_evn);
  /* p31.DUHY*/ DUHY_STORE2_X4p_odd.SETn(_GAMY_STORE2_RSTn_new_evn);
  /* p31.EJUF*/ EJUF_STORE2_X5p_odd.SETn(_GAMY_STORE2_RSTn_new_evn);
  /* p31.ENOR*/ ENOR_STORE2_X6p_odd.SETn(_GAMY_STORE2_RSTn_new_evn);
  /* p31.DEPY*/ DEPY_STORE2_X7p_odd.SETn(_GAMY_STORE2_RSTn_new_evn);

  /* p31.XOLY*/ XOLY_STORE3_X0p_odd.SETn(_WUPA_STORE3_RSTn_new_evn);
  /* p31.XYBA*/ XYBA_STORE3_X1p_odd.SETn(_WUPA_STORE3_RSTn_new_evn);
  /* p31.XABE*/ XABE_STORE3_X2p_odd.SETn(_WUPA_STORE3_RSTn_new_evn);
  /* p31.XEKA*/ XEKA_STORE3_X3p_odd.SETn(_WUPA_STORE3_RSTn_new_evn);
  /* p31.XOMY*/ XOMY_STORE3_X4p_odd.SETn(_WUPA_STORE3_RSTn_new_evn);
  /* p31.WUHA*/ WUHA_STORE3_X5p_odd.SETn(_WUPA_STORE3_RSTn_new_evn);
  /* p31.WYNA*/ WYNA_STORE3_X6p_odd.SETn(_WUPA_STORE3_RSTn_new_evn);
  /* p31.WECO*/ WECO_STORE3_X7p_odd.SETn(_WUPA_STORE3_RSTn_new_evn);

  /* p31.WEDU*/ WEDU_STORE4_X0p_odd.SETn(_WUNU_STORE4_RSTn_new_evn);
  /* p31.YGAJ*/ YGAJ_STORE4_X1p_odd.SETn(_WUNU_STORE4_RSTn_new_evn);
  /* p31.ZYJO*/ ZYJO_STORE4_X2p_odd.SETn(_WUNU_STORE4_RSTn_new_evn);
  /* p31.XURY*/ XURY_STORE4_X3p_odd.SETn(_WUNU_STORE4_RSTn_new_evn);
  /* p31.YBED*/ YBED_STORE4_X4p_odd.SETn(_WUNU_STORE4_RSTn_new_evn);
  /* p31.ZALA*/ ZALA_STORE4_X5p_odd.SETn(_WUNU_STORE4_RSTn_new_evn);
  /* p31.WYDE*/ WYDE_STORE4_X6p_odd.SETn(_WUNU_STORE4_RSTn_new_evn);
  /* p31.XEPA*/ XEPA_STORE4_X7p_odd.SETn(_WUNU_STORE4_RSTn_new_evn);

  /* p31.FUSA*/ FUSA_STORE5_X0p_odd.SETn(_EJAD_STORE5_RSTn_new_evn);
  /* p31.FAXA*/ FAXA_STORE5_X1p_odd.SETn(_EJAD_STORE5_RSTn_new_evn);
  /* p31.FOZY*/ FOZY_STORE5_X2p_odd.SETn(_EJAD_STORE5_RSTn_new_evn);
  /* p31.FESY*/ FESY_STORE5_X3p_odd.SETn(_EJAD_STORE5_RSTn_new_evn);
  /* p31.CYWE*/ CYWE_STORE5_X4p_odd.SETn(_EJAD_STORE5_RSTn_new_evn);
  /* p31.DYBY*/ DYBY_STORE5_X5p_odd.SETn(_EJAD_STORE5_RSTn_new_evn);
  /* p31.DURY*/ DURY_STORE5_X6p_odd.SETn(_EJAD_STORE5_RSTn_new_evn);
  /* p31.CUVY*/ CUVY_STORE5_X7p_odd.SETn(_EJAD_STORE5_RSTn_new_evn);

  /* p31.YCOL*/ YCOL_STORE6_X0p_odd.SETn(_XAHO_STORE6_RSTn_new_evn);
  /* p31.YRAC*/ YRAC_STORE6_X1p_odd.SETn(_XAHO_STORE6_RSTn_new_evn);
  /* p31.YMEM*/ YMEM_STORE6_X2p_odd.SETn(_XAHO_STORE6_RSTn_new_evn);
  /* p31.YVAG*/ YVAG_STORE6_X3p_odd.SETn(_XAHO_STORE6_RSTn_new_evn);
  /* p31.ZOLY*/ ZOLY_STORE6_X4p_odd.SETn(_XAHO_STORE6_RSTn_new_evn);
  /* p31.ZOGO*/ ZOGO_STORE6_X5p_odd.SETn(_XAHO_STORE6_RSTn_new_evn);
  /* p31.ZECU*/ ZECU_STORE6_X6p_odd.SETn(_XAHO_STORE6_RSTn_new_evn);
  /* p31.ZESA*/ ZESA_STORE6_X7p_odd.SETn(_XAHO_STORE6_RSTn_new_evn);

  /* p31.ERAZ*/ ERAZ_STORE7_X0p_odd.SETn(_GAFY_STORE7_RSTn_new_evn);
  /* p31.EPUM*/ EPUM_STORE7_X1p_odd.SETn(_GAFY_STORE7_RSTn_new_evn);
  /* p31.EROL*/ EROL_STORE7_X2p_odd.SETn(_GAFY_STORE7_RSTn_new_evn);
  /* p31.EHYN*/ EHYN_STORE7_X3p_odd.SETn(_GAFY_STORE7_RSTn_new_evn);
  /* p31.FAZU*/ FAZU_STORE7_X4p_odd.SETn(_GAFY_STORE7_RSTn_new_evn);
  /* p31.FAXE*/ FAXE_STORE7_X5p_odd.SETn(_GAFY_STORE7_RSTn_new_evn);
  /* p31.EXUK*/ EXUK_STORE7_X6p_odd.SETn(_GAFY_STORE7_RSTn_new_evn);
  /* p31.FEDE*/ FEDE_STORE7_X7p_odd.SETn(_GAFY_STORE7_RSTn_new_evn);

  /* p31.EZUF*/ EZUF_STORE8_X0p_odd.SETn(_WUZO_STORE8_RSTn_new_evn);
  /* p31.ENAD*/ ENAD_STORE8_X1p_odd.SETn(_WUZO_STORE8_RSTn_new_evn);
  /* p31.EBOW*/ EBOW_STORE8_X2p_odd.SETn(_WUZO_STORE8_RSTn_new_evn);
  /* p31.FYCA*/ FYCA_STORE8_X3p_odd.SETn(_WUZO_STORE8_RSTn_new_evn);
  /* p31.GAVY*/ GAVY_STORE8_X4p_odd.SETn(_WUZO_STORE8_RSTn_new_evn);
  /* p31.GYPU*/ GYPU_STORE8_X5p_odd.SETn(_WUZO_STORE8_RSTn_new_evn);
  /* p31.GADY*/ GADY_STORE8_X6p_odd.SETn(_WUZO_STORE8_RSTn_new_evn);
  /* p31.GAZA*/ GAZA_STORE8_X7p_odd.SETn(_WUZO_STORE8_RSTn_new_evn);

  /* p31.XUVY*/ XUVY_STORE9_X0p_odd.SETn(_DOSY_STORE9_RSTn_new_evn);
  /* p31.XERE*/ XERE_STORE9_X1p_odd.SETn(_DOSY_STORE9_RSTn_new_evn);
  /* p31.XUZO*/ XUZO_STORE9_X2p_odd.SETn(_DOSY_STORE9_RSTn_new_evn);
  /* p31.XEXA*/ XEXA_STORE9_X3p_odd.SETn(_DOSY_STORE9_RSTn_new_evn);
  /* p31.YPOD*/ YPOD_STORE9_X4p_odd.SETn(_DOSY_STORE9_RSTn_new_evn);
  /* p31.YROP*/ YROP_STORE9_X5p_odd.SETn(_DOSY_STORE9_RSTn_new_evn);
  /* p31.YNEP*/ YNEP_STORE9_X6p_odd.SETn(_DOSY_STORE9_RSTn_new_evn);
  /* p31.YZOF*/ YZOF_STORE9_X7p_odd.SETn(_DOSY_STORE9_RSTn_new_evn);
}

void SpriteStore::store_sprite_index(SpriteStoreFlag store_flag) {
  /* p29.GENY*/ wire _GENY_STORE0_CLKp_new_odd = not1(store_flag.DYHU_STORE0_CLKn);
  /* p29.BYVY*/ wire _BYVY_STORE1_CLKp_new_odd = not1(store_flag.BUCO_STORE1_CLKn);
  /* p29.BUZY*/ wire _BUZY_STORE2_CLKp_new_odd = not1(store_flag.GYFO_STORE2_CLKn);
  /* p29.FEKA*/ wire _FEKA_STORE3_CLKp_new_odd = not1(store_flag.GUSA_STORE3_CLKn);
  /* p29.WYLU*/ wire _WYLU_STORE4_CLKp_new_odd = not1(store_flag.DUKE_STORE4_CLKn);
  /* p29.DYMO*/ wire _DYMO_STORE5_CLKp_new_odd = not1(store_flag.BEDE_STORE5_CLKn);
  /* p29.WUSE*/ wire _WUSE_STORE6_CLKp_new_odd = not1(store_flag.WEKA_STORE6_CLKn);
  /* p29.FEFO*/ wire _FEFO_STORE7_CLKp_new_odd = not1(store_flag.GYVO_STORE7_CLKn);
  /* p29.AKOL*/ wire _AKOL_STORE8_CLKp_new_odd = not1(store_flag.BUKA_STORE8_CLKn);
  /* p29.WUFA*/ wire _WUFA_STORE9_CLKp_new_odd = not1(store_flag.DECU_STORE9_CLKn);

  /* p30.YGUS*/ YGUS_STORE0_I0n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.YSOK*/ YSOK_STORE0_I1n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.YZEP*/ YZEP_STORE0_I2n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.WYTE*/ WYTE_STORE0_I3n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.ZONY*/ ZONY_STORE0_I4n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.YWAK*/ YWAK_STORE0_I5n_odd.dff8n(_GENY_STORE0_CLKp_new_odd, SPR_TRI_I[5].qp_any());

  /* p30.CADU*/ CADU_STORE1_I0n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.CEBO*/ CEBO_STORE1_I1n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.CUFA*/ CUFA_STORE1_I2n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.COMA*/ COMA_STORE1_I3n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.CUZA*/ CUZA_STORE1_I4n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.CAJY*/ CAJY_STORE1_I5n_odd.dff8n(_BYVY_STORE1_CLKp_new_odd, SPR_TRI_I[5].qp_any());

  /* p30.BUHE*/ BUHE_STORE2_I0n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.BYHU*/ BYHU_STORE2_I1n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.BECA*/ BECA_STORE2_I2n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.BULU*/ BULU_STORE2_I3n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.BUNA*/ BUNA_STORE2_I4n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.BOXA*/ BOXA_STORE2_I5n_odd.dff8n(_BUZY_STORE2_CLKp_new_odd, SPR_TRI_I[5].qp_any());

  /* p30.DEVY*/ DEVY_STORE3_I0n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.DESE*/ DESE_STORE3_I1n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.DUNY*/ DUNY_STORE3_I2n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.DUHA*/ DUHA_STORE3_I3n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.DEBA*/ DEBA_STORE3_I4n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.DAFU*/ DAFU_STORE3_I5n_odd.dff8n(_FEKA_STORE3_CLKp_new_odd, SPR_TRI_I[5].qp_any());

  /* p30.XAVE*/ XAVE_STORE4_I0n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.XEFE*/ XEFE_STORE4_I1n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.WANU*/ WANU_STORE4_I2n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.XABO*/ XABO_STORE4_I3n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.XEGE*/ XEGE_STORE4_I4n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.XYNU*/ XYNU_STORE4_I5n_odd.dff8n(_WYLU_STORE4_CLKp_new_odd, SPR_TRI_I[5].qp_any());

  /* p30.EKOP*/ EKOP_STORE5_I0n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.ETYM*/ ETYM_STORE5_I1n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.GORU*/ GORU_STORE5_I2n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.EBEX*/ EBEX_STORE5_I3n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.ETAV*/ ETAV_STORE5_I4n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.EKAP*/ EKAP_STORE5_I5n_odd.dff8n(_DYMO_STORE5_CLKp_new_odd, SPR_TRI_I[5].qp_any());

  /* p30.GABO*/ GABO_STORE6_I0n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.GACY*/ GACY_STORE6_I1n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.FOGO*/ FOGO_STORE6_I2n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.GOHU*/ GOHU_STORE6_I3n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.FOXY*/ FOXY_STORE6_I4n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.GECU*/ GECU_STORE6_I5n_odd.dff8n(_WUSE_STORE6_CLKp_new_odd, SPR_TRI_I[5].qp_any());

  /* p30.GULE*/ GULE_STORE7_I0n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.GYNO*/ GYNO_STORE7_I1n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.FEFA*/ FEFA_STORE7_I2n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.FYSU*/ FYSU_STORE7_I3n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.GESY*/ GESY_STORE7_I4n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.FUZO*/ FUZO_STORE7_I5n_odd.dff8n(_FEFO_STORE7_CLKp_new_odd, SPR_TRI_I[5].qp_any());

  /* p30.AXUV*/ AXUV_STORE8_I0n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.BADA*/ BADA_STORE8_I1n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.APEV*/ APEV_STORE8_I2n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.BADO*/ BADO_STORE8_I3n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.BEXY*/ BEXY_STORE8_I4n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.BYHE*/ BYHE_STORE8_I5n_odd.dff8n(_AKOL_STORE8_CLKp_new_odd, SPR_TRI_I[5].qp_any());

  /* p30.YBER*/ YBER_STORE9_I0n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[0].qp_any());
  /* p30.YZOR*/ YZOR_STORE9_I1n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[1].qp_any());
  /* p30.XYFE*/ XYFE_STORE9_I2n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[2].qp_any());
  /* p30.XOTU*/ XOTU_STORE9_I3n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[3].qp_any());
  /* p30.XUTE*/ XUTE_STORE9_I4n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[4].qp_any());
  /* p30.XUFO*/ XUFO_STORE9_I5n_odd.dff8n(_WUFA_STORE9_CLKp_new_odd, SPR_TRI_I[5].qp_any());
}

void SpriteStore::store_sprite_line(SpriteStoreFlag store_flag) {
  /* p29.ENOB*/ wire _ENOB_STORE0_CLKp_new_odd = not1(store_flag.DYHU_STORE0_CLKn);
  /* p29.AHOF*/ wire _AHOF_STORE1_CLKp_new_odd = not1(store_flag.BUCO_STORE1_CLKn);
  /* p29.FUKE*/ wire _FUKE_STORE2_CLKp_new_odd = not1(store_flag.GYFO_STORE2_CLKn);
  /* p29.XYHA*/ wire _XYHA_STORE3_CLKp_new_odd = not1(store_flag.GUSA_STORE3_CLKn);
  /* p29.EWOT*/ wire _EWOT_STORE4_CLKp_new_odd = not1(store_flag.DUKE_STORE4_CLKn);
  /* p29.BUCY*/ wire _BUCY_STORE5_CLKp_new_odd = not1(store_flag.BEDE_STORE5_CLKn);
  /* p29.ZURU*/ wire _ZURU_STORE6_CLKp_new_odd = not1(store_flag.WEKA_STORE6_CLKn);
  /* p29.WABE*/ wire _WABE_STORE7_CLKp_new_odd = not1(store_flag.GYVO_STORE7_CLKn);
  /* p29.BYMY*/ wire _BYMY_STORE8_CLKp_new_odd = not1(store_flag.BUKA_STORE8_CLKn);
  /* p29.FAKA*/ wire _FAKA_STORE9_CLKp_new_odd = not1(store_flag.DECU_STORE9_CLKn);

  /* p30.GYHO*/ GYHO_STORE0_L0n_odd.dff8n(_ENOB_STORE0_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.CUFO*/ CUFO_STORE0_L1n_odd.dff8n(_ENOB_STORE0_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.BOZU*/ BOZU_STORE0_L2n_odd.dff8n(_ENOB_STORE0_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.FYHY*/ FYHY_STORE0_L3n_odd.dff8n(_ENOB_STORE0_CLKp_new_odd, SPR_TRI_L[3].qp_any());

  /* p30.AMES*/ AMES_STORE1_L0n_odd.dff8n(_AHOF_STORE1_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.AROF*/ AROF_STORE1_L1n_odd.dff8n(_AHOF_STORE1_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.ABOP*/ ABOP_STORE1_L2n_odd.dff8n(_AHOF_STORE1_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.ABUG*/ ABUG_STORE1_L3n_odd.dff8n(_AHOF_STORE1_CLKp_new_odd, SPR_TRI_L[3].qp_any());

  /* p30.YLOV*/ YLOV_STORE2_L0n_odd.dff8n(_FUKE_STORE2_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.XOSY*/ XOSY_STORE2_L1n_odd.dff8n(_FUKE_STORE2_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.XAZY*/ XAZY_STORE2_L2n_odd.dff8n(_FUKE_STORE2_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.YKUK*/ YKUK_STORE2_L3n_odd.dff8n(_FUKE_STORE2_CLKp_new_odd, SPR_TRI_L[3].qp_any());

  /* p30.ZURO*/ ZURO_STORE3_L0n_odd.dff8n(_XYHA_STORE3_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.ZYLU*/ ZYLU_STORE3_L1n_odd.dff8n(_XYHA_STORE3_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.ZENE*/ ZENE_STORE3_L2n_odd.dff8n(_XYHA_STORE3_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.ZURY*/ ZURY_STORE3_L3n_odd.dff8n(_XYHA_STORE3_CLKp_new_odd, SPR_TRI_L[3].qp_any());

  /* p30.CAPO*/ CAPO_STORE4_L0n_odd.dff8n(_EWOT_STORE4_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.CAJU*/ CAJU_STORE4_L1n_odd.dff8n(_EWOT_STORE4_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.CONO*/ CONO_STORE4_L2n_odd.dff8n(_EWOT_STORE4_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.CUMU*/ CUMU_STORE4_L3n_odd.dff8n(_EWOT_STORE4_CLKp_new_odd, SPR_TRI_L[3].qp_any());

  /* p30.ACEP*/ ACEP_STORE5_L0n_odd.dff8n(_BUCY_STORE5_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.ABEG*/ ABEG_STORE5_L1n_odd.dff8n(_BUCY_STORE5_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.ABUX*/ ABUX_STORE5_L2n_odd.dff8n(_BUCY_STORE5_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.ANED*/ ANED_STORE5_L3n_odd.dff8n(_BUCY_STORE5_CLKp_new_odd, SPR_TRI_L[3].qp_any());

  /* p30.ZUMY*/ ZUMY_STORE6_L0n_odd.dff8n(_ZURU_STORE6_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.ZAFU*/ ZAFU_STORE6_L1n_odd.dff8n(_ZURU_STORE6_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.ZEXO*/ ZEXO_STORE6_L2n_odd.dff8n(_ZURU_STORE6_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.ZUBE*/ ZUBE_STORE6_L3n_odd.dff8n(_ZURU_STORE6_CLKp_new_odd, SPR_TRI_L[3].qp_any());

  /* p30.XYNA*/ XYNA_STORE7_L0n_odd.dff8n(_WABE_STORE7_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.YGUM*/ YGUM_STORE7_L1n_odd.dff8n(_WABE_STORE7_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.XAKU*/ XAKU_STORE7_L2n_odd.dff8n(_WABE_STORE7_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.XYGO*/ XYGO_STORE7_L3n_odd.dff8n(_WABE_STORE7_CLKp_new_odd, SPR_TRI_L[3].qp_any());

  /* p30.AZAP*/ AZAP_STORE8_L0n_odd.dff8n(_BYMY_STORE8_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.AFYX*/ AFYX_STORE8_L1n_odd.dff8n(_BYMY_STORE8_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.AFUT*/ AFUT_STORE8_L2n_odd.dff8n(_BYMY_STORE8_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.AFYM*/ AFYM_STORE8_L3n_odd.dff8n(_BYMY_STORE8_CLKp_new_odd, SPR_TRI_L[3].qp_any());

  /* p30.CANA*/ CANA_STORE9_L0n_odd.dff8n(_FAKA_STORE9_CLKp_new_odd, SPR_TRI_L[0].qp_any());
  /* p30.FOFO*/ FOFO_STORE9_L1n_odd.dff8n(_FAKA_STORE9_CLKp_new_odd, SPR_TRI_L[1].qp_any());
  /* p30.DYSY*/ DYSY_STORE9_L2n_odd.dff8n(_FAKA_STORE9_CLKp_new_odd, SPR_TRI_L[2].qp_any());
  /* p30.DEWU*/ DEWU_STORE9_L3n_odd.dff8n(_FAKA_STORE9_CLKp_new_odd, SPR_TRI_L[3].qp_any());
}

void SpriteStore::store_sprite_x(SpriteStoreFlag store_flag, const OamTempB& oam_temp_b) {
  /* p29.FUXU*/ wire _FUXU_STORE0_CLKp_new_odd = not1(store_flag.DYHU_STORE0_CLKn);
  /* p29.ASYS*/ wire _ASYS_STORE1_CLKp_new_odd = not1(store_flag.BUCO_STORE1_CLKn);
  /* p29.CACU*/ wire _CACU_STORE2_CLKp_new_odd = not1(store_flag.GYFO_STORE2_CLKn);
  /* p29.YFAG*/ wire _YFAG_STORE3_CLKp_new_odd = not1(store_flag.GUSA_STORE3_CLKn);
  /* p29.WOFO*/ wire _WOFO_STORE4_CLKp_new_odd = not1(store_flag.DUKE_STORE4_CLKn);
  /* p29.CYLA*/ wire _CYLA_STORE5_CLKp_new_odd = not1(store_flag.BEDE_STORE5_CLKn);
  /* p29.ZAPE*/ wire _ZAPE_STORE6_CLKp_new_odd = not1(store_flag.WEKA_STORE6_CLKn);
  /* p29.GECY*/ wire _GECY_STORE7_CLKp_new_odd = not1(store_flag.GYVO_STORE7_CLKn);
  /* p29.CEXU*/ wire _CEXU_STORE8_CLKp_new_odd = not1(store_flag.BUKA_STORE8_CLKn);
  /* p29.WEME*/ wire _WEME_STORE9_CLKp_new_odd = not1(store_flag.DECU_STORE9_CLKn);

  /*#p31.ZAGO*/ wire _ZAGO_SPX0n_new_any = not1(oam_temp_b.YLOR_OAM_DB0p.qp_old());
  /* p31.ZOCY*/ wire _ZOCY_SPX1n_new_any = not1(oam_temp_b.ZYTY_OAM_DB1p.qp_old());
  /* p31.YPUR*/ wire _YPUR_SPX2n_new_any = not1(oam_temp_b.ZYVE_OAM_DB2p.qp_old());
  /* p31.YVOK*/ wire _YVOK_SPX3n_new_any = not1(oam_temp_b.ZEZY_OAM_DB3p.qp_old());
  /* p31.COSE*/ wire _COSE_SPX4n_new_any = not1(oam_temp_b.GOMO_OAM_DB4p.qp_old());
  /* p31.AROP*/ wire _AROP_SPX5n_new_any = not1(oam_temp_b.BAXO_OAM_DB5p.qp_old());
  /* p31.XATU*/ wire _XATU_SPX6n_new_any = not1(oam_temp_b.YZOS_OAM_DB6p.qp_old());
  /* p31.BADY*/ wire _BADY_SPX7n_new_any = not1(oam_temp_b.DEPO_OAM_DB7p.qp_old());

  /*#p31.XEPE*/ XEPE_STORE0_X0p_odd.dff9(_FUXU_STORE0_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.YLAH*/ YLAH_STORE0_X1p_odd.dff9(_FUXU_STORE0_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.ZOLA*/ ZOLA_STORE0_X2p_odd.dff9(_FUXU_STORE0_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.ZULU*/ ZULU_STORE0_X3p_odd.dff9(_FUXU_STORE0_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.WELO*/ WELO_STORE0_X4p_odd.dff9(_FUXU_STORE0_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.XUNY*/ XUNY_STORE0_X5p_odd.dff9(_FUXU_STORE0_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.WOTE*/ WOTE_STORE0_X6p_odd.dff9(_FUXU_STORE0_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.XAKO*/ XAKO_STORE0_X7p_odd.dff9(_FUXU_STORE0_CLKp_new_odd, _BADY_SPX7n_new_any);

  /* p31.DANY*/ DANY_STORE1_X0p_odd.dff9(_ASYS_STORE1_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.DUKO*/ DUKO_STORE1_X1p_odd.dff9(_ASYS_STORE1_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.DESU*/ DESU_STORE1_X2p_odd.dff9(_ASYS_STORE1_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.DAZO*/ DAZO_STORE1_X3p_odd.dff9(_ASYS_STORE1_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.DAKE*/ DAKE_STORE1_X4p_odd.dff9(_ASYS_STORE1_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.CESO*/ CESO_STORE1_X5p_odd.dff9(_ASYS_STORE1_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.DYFU*/ DYFU_STORE1_X6p_odd.dff9(_ASYS_STORE1_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.CUSY*/ CUSY_STORE1_X7p_odd.dff9(_ASYS_STORE1_CLKp_new_odd, _BADY_SPX7n_new_any);

  /* p31.FOKA*/ FOKA_STORE2_X0p_odd.dff9(_CACU_STORE2_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.FYTY*/ FYTY_STORE2_X1p_odd.dff9(_CACU_STORE2_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.FUBY*/ FUBY_STORE2_X2p_odd.dff9(_CACU_STORE2_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.GOXU*/ GOXU_STORE2_X3p_odd.dff9(_CACU_STORE2_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.DUHY*/ DUHY_STORE2_X4p_odd.dff9(_CACU_STORE2_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.EJUF*/ EJUF_STORE2_X5p_odd.dff9(_CACU_STORE2_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.ENOR*/ ENOR_STORE2_X6p_odd.dff9(_CACU_STORE2_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.DEPY*/ DEPY_STORE2_X7p_odd.dff9(_CACU_STORE2_CLKp_new_odd, _BADY_SPX7n_new_any);

  /* p31.XOLY*/ XOLY_STORE3_X0p_odd.dff9(_YFAG_STORE3_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.XYBA*/ XYBA_STORE3_X1p_odd.dff9(_YFAG_STORE3_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.XABE*/ XABE_STORE3_X2p_odd.dff9(_YFAG_STORE3_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.XEKA*/ XEKA_STORE3_X3p_odd.dff9(_YFAG_STORE3_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.XOMY*/ XOMY_STORE3_X4p_odd.dff9(_YFAG_STORE3_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.WUHA*/ WUHA_STORE3_X5p_odd.dff9(_YFAG_STORE3_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.WYNA*/ WYNA_STORE3_X6p_odd.dff9(_YFAG_STORE3_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.WECO*/ WECO_STORE3_X7p_odd.dff9(_YFAG_STORE3_CLKp_new_odd, _BADY_SPX7n_new_any);

  /* p31.WEDU*/ WEDU_STORE4_X0p_odd.dff9(_WOFO_STORE4_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.YGAJ*/ YGAJ_STORE4_X1p_odd.dff9(_WOFO_STORE4_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.ZYJO*/ ZYJO_STORE4_X2p_odd.dff9(_WOFO_STORE4_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.XURY*/ XURY_STORE4_X3p_odd.dff9(_WOFO_STORE4_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.YBED*/ YBED_STORE4_X4p_odd.dff9(_WOFO_STORE4_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.ZALA*/ ZALA_STORE4_X5p_odd.dff9(_WOFO_STORE4_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.WYDE*/ WYDE_STORE4_X6p_odd.dff9(_WOFO_STORE4_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.XEPA*/ XEPA_STORE4_X7p_odd.dff9(_WOFO_STORE4_CLKp_new_odd, _BADY_SPX7n_new_any);

  /* p31.FUSA*/ FUSA_STORE5_X0p_odd.dff9(_CYLA_STORE5_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.FAXA*/ FAXA_STORE5_X1p_odd.dff9(_CYLA_STORE5_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.FOZY*/ FOZY_STORE5_X2p_odd.dff9(_CYLA_STORE5_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.FESY*/ FESY_STORE5_X3p_odd.dff9(_CYLA_STORE5_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.CYWE*/ CYWE_STORE5_X4p_odd.dff9(_CYLA_STORE5_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.DYBY*/ DYBY_STORE5_X5p_odd.dff9(_CYLA_STORE5_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.DURY*/ DURY_STORE5_X6p_odd.dff9(_CYLA_STORE5_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.CUVY*/ CUVY_STORE5_X7p_odd.dff9(_CYLA_STORE5_CLKp_new_odd, _BADY_SPX7n_new_any);

  /* p31.YCOL*/ YCOL_STORE6_X0p_odd.dff9(_ZAPE_STORE6_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.YRAC*/ YRAC_STORE6_X1p_odd.dff9(_ZAPE_STORE6_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.YMEM*/ YMEM_STORE6_X2p_odd.dff9(_ZAPE_STORE6_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.YVAG*/ YVAG_STORE6_X3p_odd.dff9(_ZAPE_STORE6_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.ZOLY*/ ZOLY_STORE6_X4p_odd.dff9(_ZAPE_STORE6_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.ZOGO*/ ZOGO_STORE6_X5p_odd.dff9(_ZAPE_STORE6_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.ZECU*/ ZECU_STORE6_X6p_odd.dff9(_ZAPE_STORE6_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.ZESA*/ ZESA_STORE6_X7p_odd.dff9(_ZAPE_STORE6_CLKp_new_odd, _BADY_SPX7n_new_any);

  /* p31.ERAZ*/ ERAZ_STORE7_X0p_odd.dff9(_GECY_STORE7_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.EPUM*/ EPUM_STORE7_X1p_odd.dff9(_GECY_STORE7_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.EROL*/ EROL_STORE7_X2p_odd.dff9(_GECY_STORE7_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.EHYN*/ EHYN_STORE7_X3p_odd.dff9(_GECY_STORE7_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.FAZU*/ FAZU_STORE7_X4p_odd.dff9(_GECY_STORE7_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.FAXE*/ FAXE_STORE7_X5p_odd.dff9(_GECY_STORE7_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.EXUK*/ EXUK_STORE7_X6p_odd.dff9(_GECY_STORE7_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.FEDE*/ FEDE_STORE7_X7p_odd.dff9(_GECY_STORE7_CLKp_new_odd, _BADY_SPX7n_new_any);

  /* p31.EZUF*/ EZUF_STORE8_X0p_odd.dff9(_CEXU_STORE8_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.ENAD*/ ENAD_STORE8_X1p_odd.dff9(_CEXU_STORE8_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.EBOW*/ EBOW_STORE8_X2p_odd.dff9(_CEXU_STORE8_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.FYCA*/ FYCA_STORE8_X3p_odd.dff9(_CEXU_STORE8_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.GAVY*/ GAVY_STORE8_X4p_odd.dff9(_CEXU_STORE8_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.GYPU*/ GYPU_STORE8_X5p_odd.dff9(_CEXU_STORE8_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.GADY*/ GADY_STORE8_X6p_odd.dff9(_CEXU_STORE8_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.GAZA*/ GAZA_STORE8_X7p_odd.dff9(_CEXU_STORE8_CLKp_new_odd, _BADY_SPX7n_new_any);

  /* p31.XUVY*/ XUVY_STORE9_X0p_odd.dff9(_WEME_STORE9_CLKp_new_odd, _ZAGO_SPX0n_new_any);
  /* p31.XERE*/ XERE_STORE9_X1p_odd.dff9(_WEME_STORE9_CLKp_new_odd, _ZOCY_SPX1n_new_any);
  /* p31.XUZO*/ XUZO_STORE9_X2p_odd.dff9(_WEME_STORE9_CLKp_new_odd, _YPUR_SPX2n_new_any);
  /* p31.XEXA*/ XEXA_STORE9_X3p_odd.dff9(_WEME_STORE9_CLKp_new_odd, _YVOK_SPX3n_new_any);
  /* p31.YPOD*/ YPOD_STORE9_X4p_odd.dff9(_WEME_STORE9_CLKp_new_odd, _COSE_SPX4n_new_any);
  /* p31.YROP*/ YROP_STORE9_X5p_odd.dff9(_WEME_STORE9_CLKp_new_odd, _AROP_SPX5n_new_any);
  /* p31.YNEP*/ YNEP_STORE9_X6p_odd.dff9(_WEME_STORE9_CLKp_new_odd, _XATU_SPX6n_new_any);
  /* p31.YZOF*/ YZOF_STORE9_X7p_odd.dff9(_WEME_STORE9_CLKp_new_odd, _BADY_SPX7n_new_any);
}

void SpriteStore::get_sprite_index(wire WUDA_xxCDxxGH, wire XYMU_RENDERINGp, wire CENO_SCANNINGn, BusOut BUS_OAM_An[8]) {
  wire VCC = 1;
  /* p28.YFOT*/ wire _YFOT_OAM_A2p_old = not1(BUS_OAM_An[2].qp_old());
  /* p28.YFOC*/ wire _YFOC_OAM_A3p_old = not1(BUS_OAM_An[3].qp_old());
  /* p28.YVOM*/ wire _YVOM_OAM_A4p_old = not1(BUS_OAM_An[4].qp_old());
  /* p28.YMEV*/ wire _YMEV_OAM_A5p_old = not1(BUS_OAM_An[5].qp_old());
  /* p28.XEMU*/ wire _XEMU_OAM_A6p_old = not1(BUS_OAM_An[6].qp_old());
  /* p28.YZET*/ wire _YZET_OAM_A7p_old = not1(BUS_OAM_An[7].qp_old());

  // Sprite store grabs the sprite index off the _old_ oam address bus
  /* p30.XADU*/ XADU_SPRITE_IDX0p.dff13(WUDA_xxCDxxGH, VCC, _YFOT_OAM_A2p_old);
  /* p30.XEDY*/ XEDY_SPRITE_IDX1p.dff13(WUDA_xxCDxxGH, VCC, _YFOC_OAM_A3p_old);
  /* p30.ZUZE*/ ZUZE_SPRITE_IDX2p.dff13(WUDA_xxCDxxGH, VCC, _YVOM_OAM_A4p_old);
  /* p30.XOBE*/ XOBE_SPRITE_IDX3p.dff13(WUDA_xxCDxxGH, VCC, _YMEV_OAM_A5p_old);
  /* p30.YDUF*/ YDUF_SPRITE_IDX4p.dff13(WUDA_xxCDxxGH, VCC, _XEMU_OAM_A6p_old);
  /* p30.XECU*/ XECU_SPRITE_IDX5p.dff13(WUDA_xxCDxxGH, VCC, _YZET_OAM_A7p_old);

  /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn_new = and2(CENO_SCANNINGn, XYMU_RENDERINGp);
  /*#p30.WUZY*/ SPR_TRI_I[0].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XADU_SPRITE_IDX0p.qn_new());
  /* p30.WYSE*/ SPR_TRI_I[1].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XEDY_SPRITE_IDX1p.qn_new());
  /* p30.ZYSU*/ SPR_TRI_I[2].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, ZUZE_SPRITE_IDX2p.qn_new());
  /* p30.WYDA*/ SPR_TRI_I[3].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XOBE_SPRITE_IDX3p.qn_new());
  /* p30.WUCO*/ SPR_TRI_I[4].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, YDUF_SPRITE_IDX4p.qn_new());
  /* p30.WEZA*/ SPR_TRI_I[5].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XECU_SPRITE_IDX5p.qn_new());
}

void SpriteStore::get_sprite_line(wire FEPO_STORE_MATCHp_new_evn, const RegLY& reg_ly, const OamTempA& oam_temp_a) {
  wire GND = 0;

  /*#p29.EBOS*/ wire EBOS_LY0n_new_evn = not1(reg_ly.MUWY_LY0p.qp_new());
  /* p29.DASA*/ wire DASA_LY1n_new_evn = not1(reg_ly.MYRO_LY1p.qp_new());
  /* p29.FUKY*/ wire FUKY_LY2n_new_evn = not1(reg_ly.LEXA_LY2p.qp_new());
  /* p29.FUVE*/ wire FUVE_LY3n_new_evn = not1(reg_ly.LYDO_LY3p.qp_new());

  /* p29.ERUC*/ wire _ERUC_YDIFF_S0_new = add_s(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p.qp_new(), GND);
  /* p29.ERUC*/ wire _ERUC_YDIFF_C0_new = add_c(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p.qp_new(), GND);
  /* p29.ENEF*/ wire _ENEF_YDIFF_S1_new = add_s(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
  /* p29.ENEF*/ wire _ENEF_YDIFF_C1_new = add_c(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
  /* p29.FECO*/ wire _FECO_YDIFF_S2_new = add_s(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
  /* p29.FECO*/ wire _FECO_YDIFF_C2_new = add_c(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
  /* p29.GYKY*/ wire _GYKY_YDIFF_S3_new = add_s(FUVE_LY3n_new_evn, oam_temp_a.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF_C2_new);

  /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0_new = not1(_ERUC_YDIFF_S0_new);
  /* p29.DABY*/ wire _DABY_SPRITE_DELTA1_new = not1(_ENEF_YDIFF_S1_new);
  /* p29.DABU*/ wire _DABU_SPRITE_DELTA2_new = not1(_FECO_YDIFF_S2_new);
  /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3_new = not1(_GYKY_YDIFF_S3_new);

  /*#p30.CUCU*/ SPR_TRI_L[0].tri6_nn(FEPO_STORE_MATCHp_new_evn, _DEGE_SPRITE_DELTA0_new);
  /*#p30.CUCA*/ SPR_TRI_L[1].tri6_nn(FEPO_STORE_MATCHp_new_evn, _DABY_SPRITE_DELTA1_new);
  /*#p30.CEGA*/ SPR_TRI_L[2].tri6_nn(FEPO_STORE_MATCHp_new_evn, _DABU_SPRITE_DELTA2_new);
  /*#p30.WENU*/ SPR_TRI_L[3].tri6_nn(FEPO_STORE_MATCHp_new_evn, _GYSA_SPRITE_DELTA3_new);
}