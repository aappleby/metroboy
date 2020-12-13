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


void SpriteStore::reset_matched_store(wire _ABAK_LINE_RSTp, wire WUTY_SFETCH_DONE_TRIGp, SpriteFlag sprite_flag) {
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


void SpriteStore::get_sprite(SpriteFlag sprite_flag, BusIO SPR_TRI_I[6], BusIO SPR_TRI_L[4]) {
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