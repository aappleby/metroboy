#include "RegSpriteStore.h"
#include "Sch_PixPipe.h"

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