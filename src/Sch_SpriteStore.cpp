#include "Sch_SpriteStore.h"

#include "Sch_Top.h"

using namespace Schematics;

void SpriteStore::dump(Dumper& d) const {
  d("----------SpriteStore---------\n");
  d("DEZY_STORE_ENn %c\n", DEZY_STORE_ENn.c());
  d("FEPO_STORE_MATCHp %d\n", wire(FEPO_STORE_MATCHp));

  d("SPRITE INDEX %02d\n", 
    pack_p(
      XADU_SPRITE_IDX0n.qn(),
      XEDY_SPRITE_IDX1n.qn(),
      ZUZE_SPRITE_IDX2n.qn(),
      XOBE_SPRITE_IDX3n.qn(),
      YDUF_SPRITE_IDX4n.qn(),
      XECU_SPRITE_IDX5n.qn(),
      0,
      0
    )
  );

  d("STORE MATCH %c%c%c%c%c%c%c%c%c%c\n",
    STORE0_MATCHn.c(), STORE1_MATCHn.c(), STORE2_MATCHn.c(), STORE3_MATCHn.c(), STORE4_MATCHn.c(),
    STORE5_MATCHn.c(), STORE6_MATCHn.c(), STORE7_MATCHn.c(), STORE8_MATCHn.c(), STORE9_MATCHn.c());

  d("SPRITE COUNT %02d\n", pack_p(SPRITE_COUNT0.qp(), SPRITE_COUNT1.qp(), SPRITE_COUNT2.qp(), SPRITE_COUNT3.qp()));

  int spr_tri_idx = pack_p(SPR_TRI_I0.qp(), SPR_TRI_I1.qp(), SPR_TRI_I2.qp(), SPR_TRI_I3.qp(), SPR_TRI_I4.qp(), SPR_TRI_I5.qp(), 0, 0);
  d("SPR_TRI_IDX  = %2d %c%c%c%c%c%c\n",
    spr_tri_idx,
    SPR_TRI_I5.c(), SPR_TRI_I4.c(), SPR_TRI_I3.c(), SPR_TRI_I2.c(),
    SPR_TRI_I1.c(), SPR_TRI_I0.c());

  int spr_tri_line = pack_p(SPR_TRI_L0.qp(), SPR_TRI_L1.qp(), SPR_TRI_L2.qp(), SPR_TRI_L3.qp()); 
  d("SPR_TRI_LINE = %2d %c%c%c%c\n",
    spr_tri_line,
    SPR_TRI_L0.c(), SPR_TRI_L1.c(), SPR_TRI_L2.c(), SPR_TRI_L3.c());

  d("STORE0 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE0_MATCHn,
    EBOJ_STORE0_RSTp.qp(),
    pack_p(YGUS_STORE0_I0.qp(), YSOK_STORE0_I1.qp(), YZEP_STORE0_I2.qp(), WYTE_STORE0_I3.qp(), ZONY_STORE0_I4.qp(), YWAK_STORE0_I5.qp(), 0, 0),
    pack_p(GYHO_STORE0_L0.qp(), CUFO_STORE0_L1.qp(), BOZU_STORE0_L2.qp(), FYHY_STORE0_L3.qp()),
    pack_p(XEPE_STORE0_X0.qn(), YLAH_STORE0_X1.qn(), ZOLA_STORE0_X2.qn(), ZULU_STORE0_X3.qn(), WELO_STORE0_X4.qn(), XUNY_STORE0_X5.qn(), WOTE_STORE0_X6.qn(), XAKO_STORE0_X7.qn())
  );

  d("STORE1 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE1_MATCHn,
    CEDY_STORE1_RSTp.qp(),
    pack_p(CADU_STORE1_I0.qp(), CEBO_STORE1_I1.qp(), CUFA_STORE1_I2.qp(), COMA_STORE1_I3.qp(), CUZA_STORE1_I4.qp(), CAJY_STORE1_I5.qp(), 0, 0),
    pack_p(AMES_STORE1_L0.qp(), AROF_STORE1_L1.qp(), ABOP_STORE1_L2.qp(), ABUG_STORE1_L3.qp()),
    pack_p(DANY_STORE1_X0.qn(), DUKO_STORE1_X1.qn(), DESU_STORE1_X2.qn(), DAZO_STORE1_X3.qn(), DAKE_STORE1_X4.qn(), CESO_STORE1_X5.qn(), DYFU_STORE1_X6.qn(), CUSY_STORE1_X7.qn())
  );

  d("STORE2 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE2_MATCHn,
    EGAV_STORE2_RSTp.qp(),
    pack_p(BUHE_STORE2_I0.qp(), BYHU_STORE2_I1.qp(), BECA_STORE2_I2.qp(), BULU_STORE2_I3.qp(), BUNA_STORE2_I4.qp(), BOXA_STORE2_I5.qp(), 0, 0),
    pack_p(YLOV_STORE2_L0.qp(), XOSY_STORE2_L1.qp(), XAZY_STORE2_L2.qp(), YKUK_STORE2_L3.qp()),
    pack_p(FOKA_STORE2_X0.qn(), FYTY_STORE2_X1.qn(), FUBY_STORE2_X2.qn(), GOXU_STORE2_X3.qn(), DUHY_STORE2_X4.qn(), EJUF_STORE2_X5.qn(), ENOR_STORE2_X6.qn(), DEPY_STORE2_X7.qn())
  );

  d("STORE3 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE3_MATCHn,
    GOTA_STORE3_RSTp.qp(),
    pack_p(DEVY_STORE3_I0.qp(), DESE_STORE3_I1.qp(), DUNY_STORE3_I2.qp(), DUHA_STORE3_I3.qp(), DEBA_STORE3_I4.qp(), DAFU_STORE3_I5.qp(), 0, 0),
    pack_p(ZURO_STORE3_L0.qp(), ZYLU_STORE3_L1.qp(), ZENE_STORE3_L2.qp(), ZURY_STORE3_L3.qp()),
    pack_p(XOLY_STORE3_X0.qn(), XYBA_STORE3_X1.qn(), XABE_STORE3_X2.qn(), XEKA_STORE3_X3.qn(), XOMY_STORE3_X4.qn(), WUHA_STORE3_X5.qn(), WYNA_STORE3_X6.qn(), WECO_STORE3_X7.qn())
  );

  d("STORE4 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE4_MATCHn,
    XUDY_STORE4_RSTp.qp(),
    pack_p(XAVE_STORE4_I0.qp(), XEFE_STORE4_I1.qp(), WANU_STORE4_I2.qp(), XABO_STORE4_I3.qp(), XEGE_STORE4_I4.qp(), XYNU_STORE4_I5.qp(), 0, 0),
    pack_p(CAPO_STORE4_L0.qp(), CAJU_STORE4_L1.qp(), CONO_STORE4_L2.qp(), CUMU_STORE4_L3.qp()),
    pack_p(WEDU_STORE4_X0.qn(), YGAJ_STORE4_X1.qn(), ZYJO_STORE4_X2.qn(), XURY_STORE4_X3.qn(), YBED_STORE4_X4.qn(), ZALA_STORE4_X5.qn(), WYDE_STORE4_X6.qn(), XEPA_STORE4_X7.qn())
  );

  d("STORE5 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE5_MATCHn,
    WAFY_STORE5_RSTp.qp(),
    pack_p(EKOP_STORE5_I0.qp(), ETYM_STORE5_I1.qp(), GORU_STORE5_I2.qp(), EBEX_STORE5_I3.qp(), ETAV_STORE5_I4.qp(), EKAP_STORE5_I5.qp(), 0, 0),
    pack_p(ACEP_STORE5_L0.qp(), ABEG_STORE5_L1.qp(), ABUX_STORE5_L2.qp(), ANED_STORE5_L3.qp()),
    pack_p(FUSA_STORE5_X0.qn(), FAXA_STORE5_X1.qn(), FOZY_STORE5_X2.qn(), FESY_STORE5_X3.qn(), CYWE_STORE5_X4.qn(), DYBY_STORE5_X5.qn(), DURY_STORE5_X6.qn(), CUVY_STORE5_X7.qn())
  );

  d("STORE6 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE6_MATCHn,
    WOMY_STORE6_RSTp.qp(),
    pack_p(GABO_STORE6_I0.qp(), GACY_STORE6_I1.qp(), FOGO_STORE6_I2.qp(), GOHU_STORE6_I3.qp(), FOXY_STORE6_I4.qp(), GECU_STORE6_I5.qp(), 0, 0),
    pack_p(ZUMY_STORE6_L0.qp(), ZAFU_STORE6_L1.qp(), ZEXO_STORE6_L2.qp(), ZUBE_STORE6_L3.qp()),
    pack_p(YCOL_STORE6_X0.qn(), YRAC_STORE6_X1.qn(), YMEM_STORE6_X2.qn(), YVAG_STORE6_X3.qn(), ZOLY_STORE6_X4.qn(), ZOGO_STORE6_X5.qn(), ZECU_STORE6_X6.qn(), ZESA_STORE6_X7.qn())
  );

  d("STORE7 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE7_MATCHn,
    WAPO_STORE7_RSTp.qp(),
    pack_p(GULE_STORE7_I0.qp(), GYNO_STORE7_I1.qp(), FEFA_STORE7_I2.qp(), FYSU_STORE7_I3.qp(), GESY_STORE7_I4.qp(), FUZO_STORE7_I5.qp(), 0, 0),
    pack_p(XYNA_STORE7_L0.qp(), YGUM_STORE7_L1.qp(), XAKU_STORE7_L2.qp(), XYGO_STORE7_L3.qp()),
    pack_p(ERAZ_STORE7_X0.qn(), EPUM_STORE7_X1.qn(), EROL_STORE7_X2.qn(), EHYN_STORE7_X3.qn(), FAZU_STORE7_X4.qn(), FAXE_STORE7_X5.qn(), EXUK_STORE7_X6.qn(), FEDE_STORE7_X7.qn())
  );

  d("STORE8 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE8_MATCHn,
    EXUQ_STORE8_RSTp.qp(),
    pack_p(AXUV_STORE8_I0.qp(), BADA_STORE8_I1.qp(), APEV_STORE8_I2.qp(), BADO_STORE8_I3.qp(), BEXY_STORE8_I4.qp(), BYHE_STORE8_I5.qp(), 0, 0),
    pack_p(AZAP_STORE8_L0.qp(), AFYX_STORE8_L1.qp(), AFUT_STORE8_L2.qp(), AFYM_STORE8_L3.qp()),
    pack_p(EZUF_STORE8_X0.qn(), ENAD_STORE8_X1.qn(), EBOW_STORE8_X2.qn(), FYCA_STORE8_X3.qn(), GAVY_STORE8_X4.qn(), GYPU_STORE8_X5.qn(), GADY_STORE8_X6.qn(), GAZA_STORE8_X7.qn())
  );

  d("STORE9 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE9_MATCHn,
    FONO_STORE9_RSTp.qp(),
    pack_p(YBER_STORE9_I0.qp(), YZOR_STORE9_I1.qp(), XYFE_STORE9_I2.qp(), XOTU_STORE9_I3.qp(), XUTE_STORE9_I4.qp(), XUFO_STORE9_I5.qp(), 0, 0),
    pack_p(CANA_STORE9_L0.qp(), FOFO_STORE9_L1.qp(), DYSY_STORE9_L2.qp(), DEWU_STORE9_L3.qp()),
    pack_p(XUVY_STORE9_X0.qn(), XERE_STORE9_X1.qn(), XUZO_STORE9_X2.qn(), XEXA_STORE9_X3.qn(), YPOD_STORE9_X4.qn(), YROP_STORE9_X5.qn(), YNEP_STORE9_X6.qn(), YZOF_STORE9_X7.qn())
  );

  d("\n");
}


//------------------------------------------------------------------------------

void SpriteStore::tick(const SchematicTop& top) {
  {
    int pix_count = top.pix_pipe.get_pix_count();
    (void)pix_count;

    // So the matcher only makes sense if the xor gates are reading STOREN_XN.qn(), not qp().
    // Which means rung 8 of dff9 must be qn
    
    // ACAM := not1(XEHO_17);
    // ZOGY_01 >> ZAKO_01
    // ZOGY_02 << XEPE_08
    // ZOGY_03 << ACAM_03

    // FIXME polarity hack

    /*p21.ACAM*/ wire _ACAM_X0n = not1(top.pix_pipe.XEHO_X0p.qp());
    /*p21.AZUB*/ wire _AZUB_X1n = not1(top.pix_pipe.SAVY_X1p.qp());
    /*p21.AMEL*/ wire _AMEL_X2n = not1(top.pix_pipe.XODU_X2p.qp());
    /*p21.AHAL*/ wire _AHAL_X3n = not1(top.pix_pipe.XYDO_X3p.qp());
    /*p21.APUX*/ wire _APUX_X4n = not1(top.pix_pipe.TUHU_X4p.qp());
    /*p21.ABEF*/ wire _ABEF_X5n = not1(top.pix_pipe.TUKY_X5p.qp());
    /*p21.ADAZ*/ wire _ADAZ_X6n = not1(top.pix_pipe.TAKO_X6p.qp());
    /*p21.ASAH*/ wire _ASAH_X7n = not1(top.pix_pipe.SYBE_X7p.qp());

    /*p31.ZOGY*/ wire STORE0_MATCH0n = xnor2(XEPE_STORE0_X0.qn(), _ACAM_X0n);
    /*p31.ZEBA*/ wire STORE0_MATCH1n = xnor2(YLAH_STORE0_X1.qn(), _AZUB_X1n);
    /*p31.ZAHA*/ wire STORE0_MATCH2n = xnor2(ZOLA_STORE0_X2.qn(), _AMEL_X2n);
    /*p31.ZOKY*/ wire STORE0_MATCH3n = xnor2(ZULU_STORE0_X3.qn(), _AHAL_X3n);
    /*p31.WOJU*/ wire STORE0_MATCH4n = xnor2(WELO_STORE0_X4.qn(), _APUX_X4n);
    /*p31.YFUN*/ wire STORE0_MATCH5n = xnor2(XUNY_STORE0_X5.qn(), _ABEF_X5n);
    /*p31.WYZA*/ wire STORE0_MATCH6n = xnor2(WOTE_STORE0_X6.qn(), _ADAZ_X6n);
    /*p31.YPUK*/ wire STORE0_MATCH7n = xnor2(XAKO_STORE0_X7.qn(), _ASAH_X7n);

    /*p31.EDYM*/ wire STORE1_MATCH0n = xnor2(DANY_STORE1_X0.qn(), _ACAM_X0n);
    /*p31.EMYB*/ wire STORE1_MATCH1n = xnor2(DUKO_STORE1_X1.qn(), _AZUB_X1n);
    /*p31.EBEF*/ wire STORE1_MATCH2n = xnor2(DESU_STORE1_X2.qn(), _AMEL_X2n);
    /*p31.EWOK*/ wire STORE1_MATCH3n = xnor2(DAZO_STORE1_X3.qn(), _AHAL_X3n);
    /*p31.COLA*/ wire STORE1_MATCH4n = xnor2(DAKE_STORE1_X4.qn(), _APUX_X4n);
    /*p31.BOBA*/ wire STORE1_MATCH5n = xnor2(CESO_STORE1_X5.qn(), _ABEF_X5n);
    /*p31.COLU*/ wire STORE1_MATCH6n = xnor2(DYFU_STORE1_X6.qn(), _ADAZ_X6n);
    /*p31.BAHU*/ wire STORE1_MATCH7n = xnor2(CUSY_STORE1_X7.qn(), _ASAH_X7n);

    /*p31.FUZU*/ wire STORE2_MATCH0n = xnor2(FOKA_STORE2_X0.qn(), _ACAM_X0n);
    /*p31.FESO*/ wire STORE2_MATCH1n = xnor2(FYTY_STORE2_X1.qn(), _AZUB_X1n);
    /*p31.FOKY*/ wire STORE2_MATCH2n = xnor2(FUBY_STORE2_X2.qn(), _AMEL_X2n);
    /*p31.FYVA*/ wire STORE2_MATCH3n = xnor2(GOXU_STORE2_X3.qn(), _AHAL_X3n);
    /*p31.CEKO*/ wire STORE2_MATCH4n = xnor2(DUHY_STORE2_X4.qn(), _APUX_X4n);
    /*p31.DETY*/ wire STORE2_MATCH5n = xnor2(EJUF_STORE2_X5.qn(), _ABEF_X5n);
    /*p31.DOZO*/ wire STORE2_MATCH6n = xnor2(ENOR_STORE2_X6.qn(), _ADAZ_X6n);
    /*p31.CONY*/ wire STORE2_MATCH7n = xnor2(DEPY_STORE2_X7.qn(), _ASAH_X7n);

    /*p31.YHOK*/ wire STORE3_MATCH0n = xnor2(XOLY_STORE3_X0.qn(), _ACAM_X0n);
    /*p31.YCAH*/ wire STORE3_MATCH1n = xnor2(XYBA_STORE3_X1.qn(), _AZUB_X1n);
    /*p31.YDAJ*/ wire STORE3_MATCH2n = xnor2(XABE_STORE3_X2.qn(), _AMEL_X2n);
    /*p31.YVUZ*/ wire STORE3_MATCH3n = xnor2(XEKA_STORE3_X3.qn(), _AHAL_X3n);
    /*p31.YVAP*/ wire STORE3_MATCH4n = xnor2(XOMY_STORE3_X4.qn(), _APUX_X4n);
    /*p31.XENY*/ wire STORE3_MATCH5n = xnor2(WUHA_STORE3_X5.qn(), _ABEF_X5n);
    /*p31.XAVU*/ wire STORE3_MATCH6n = xnor2(WYNA_STORE3_X6.qn(), _ADAZ_X6n);
    /*p31.XEVA*/ wire STORE3_MATCH7n = xnor2(WECO_STORE3_X7.qn(), _ASAH_X7n);

    /*p31.XEJU*/ wire STORE4_MATCH0n = xnor2(WEDU_STORE4_X0.qn(), _ACAM_X0n);
    /*p31.ZATE*/ wire STORE4_MATCH1n = xnor2(YGAJ_STORE4_X1.qn(), _AZUB_X1n);
    /*p31.ZAKU*/ wire STORE4_MATCH2n = xnor2(ZYJO_STORE4_X2.qn(), _AMEL_X2n);
    /*p31.YBOX*/ wire STORE4_MATCH3n = xnor2(XURY_STORE4_X3.qn(), _AHAL_X3n);
    /*p31.ZYKU*/ wire STORE4_MATCH4n = xnor2(YBED_STORE4_X4.qn(), _APUX_X4n);
    /*p31.ZYPU*/ wire STORE4_MATCH5n = xnor2(ZALA_STORE4_X5.qn(), _ABEF_X5n);
    /*p31.XAHA*/ wire STORE4_MATCH6n = xnor2(WYDE_STORE4_X6.qn(), _ADAZ_X6n);
    /*p31.ZEFE*/ wire STORE4_MATCH7n = xnor2(XEPA_STORE4_X7.qn(), _ASAH_X7n);

    /*p31.GUZO*/ wire STORE5_MATCH0n = xnor2(FUSA_STORE5_X0.qn(), _ACAM_X0n);
    /*p31.GOLA*/ wire STORE5_MATCH1n = xnor2(FAXA_STORE5_X1.qn(), _AZUB_X1n);
    /*p31.GEVE*/ wire STORE5_MATCH2n = xnor2(FOZY_STORE5_X2.qn(), _AMEL_X2n);
    /*p31.GUDE*/ wire STORE5_MATCH3n = xnor2(FESY_STORE5_X3.qn(), _AHAL_X3n);
    /*p31.BAZY*/ wire STORE5_MATCH4n = xnor2(CYWE_STORE5_X4.qn(), _APUX_X4n);
    /*p31.CYLE*/ wire STORE5_MATCH5n = xnor2(DYBY_STORE5_X5.qn(), _ABEF_X5n);
    /*p31.CEVA*/ wire STORE5_MATCH6n = xnor2(DURY_STORE5_X6.qn(), _ADAZ_X6n);
    /*p31.BUMY*/ wire STORE5_MATCH7n = xnor2(CUVY_STORE5_X7.qn(), _ASAH_X7n);

    /*p31.XOSU*/ wire STORE6_MATCH0n = xnor2(YCOL_STORE6_X0.qn(), _ACAM_X0n);
    /*p31.ZUVU*/ wire STORE6_MATCH1n = xnor2(YRAC_STORE6_X1.qn(), _AZUB_X1n);
    /*p31.XUCO*/ wire STORE6_MATCH2n = xnor2(YMEM_STORE6_X2.qn(), _AMEL_X2n);
    /*p31.ZULO*/ wire STORE6_MATCH3n = xnor2(YVAG_STORE6_X3.qn(), _AHAL_X3n);
    /*p31.ZARE*/ wire STORE6_MATCH4n = xnor2(ZOLY_STORE6_X4.qn(), _APUX_X4n);
    /*p31.ZEMU*/ wire STORE6_MATCH5n = xnor2(ZOGO_STORE6_X5.qn(), _ABEF_X5n);
    /*p31.ZYGO*/ wire STORE6_MATCH6n = xnor2(ZECU_STORE6_X6.qn(), _ADAZ_X6n);
    /*p31.ZUZY*/ wire STORE6_MATCH7n = xnor2(ZESA_STORE6_X7.qn(), _ASAH_X7n);

    /*p31.DUSE*/ wire STORE7_MATCH0n = xnor2(ERAZ_STORE7_X0.qn(), _ACAM_X0n);
    /*p31.DAGU*/ wire STORE7_MATCH1n = xnor2(EPUM_STORE7_X1.qn(), _AZUB_X1n);
    /*p31.DYZE*/ wire STORE7_MATCH2n = xnor2(EROL_STORE7_X2.qn(), _AMEL_X2n);
    /*p31.DESO*/ wire STORE7_MATCH3n = xnor2(EHYN_STORE7_X3.qn(), _AHAL_X3n);
    /*p31.EJOT*/ wire STORE7_MATCH4n = xnor2(FAZU_STORE7_X4.qn(), _APUX_X4n);
    /*p31.ESAJ*/ wire STORE7_MATCH5n = xnor2(FAXE_STORE7_X5.qn(), _ABEF_X5n);
    /*p31.DUCU*/ wire STORE7_MATCH6n = xnor2(EXUK_STORE7_X6.qn(), _ADAZ_X6n);
    /*p31.EWUD*/ wire STORE7_MATCH7n = xnor2(FEDE_STORE7_X7.qn(), _ASAH_X7n);

    /*p31.DUZE*/ wire STORE8_MATCH0n = xnor2(EZUF_STORE8_X0.qn(), _APUX_X4n);
    /*p31.DAGA*/ wire STORE8_MATCH1n = xnor2(ENAD_STORE8_X1.qn(), _ABEF_X5n);
    /*p31.DAWU*/ wire STORE8_MATCH2n = xnor2(EBOW_STORE8_X2.qn(), _ADAZ_X6n);
    /*p31.EJAW*/ wire STORE8_MATCH3n = xnor2(FYCA_STORE8_X3.qn(), _ASAH_X7n);
    /*p31.GOHO*/ wire STORE8_MATCH4n = xnor2(GAVY_STORE8_X4.qn(), _ACAM_X0n);
    /*p31.GASU*/ wire STORE8_MATCH5n = xnor2(GYPU_STORE8_X5.qn(), _AZUB_X1n);
    /*p31.GABU*/ wire STORE8_MATCH6n = xnor2(GADY_STORE8_X6.qn(), _AMEL_X2n);
    /*p31.GAFE*/ wire STORE8_MATCH7n = xnor2(GAZA_STORE8_X7.qn(), _AHAL_X3n);

    /*p31.YMAM*/ wire STORE9_MATCH0n = xnor2(XUVY_STORE9_X0.qn(), _ACAM_X0n);
    /*p31.YTYP*/ wire STORE9_MATCH1n = xnor2(XERE_STORE9_X1.qn(), _AZUB_X1n);
    /*p31.YFOP*/ wire STORE9_MATCH2n = xnor2(XUZO_STORE9_X2.qn(), _AMEL_X2n);
    /*p31.YVAC*/ wire STORE9_MATCH3n = xnor2(XEXA_STORE9_X3.qn(), _AHAL_X3n);
    /*p31.ZYWU*/ wire STORE9_MATCH4n = xnor2(YPOD_STORE9_X4.qn(), _APUX_X4n);
    /*p31.ZUZA*/ wire STORE9_MATCH5n = xnor2(YROP_STORE9_X5.qn(), _ABEF_X5n);
    /*p31.ZEJO*/ wire STORE9_MATCH6n = xnor2(YNEP_STORE9_X6.qn(), _ADAZ_X6n);
    /*p31.ZEDA*/ wire STORE9_MATCH7n = xnor2(YZOF_STORE9_X7.qn(), _ASAH_X7n);

    /*p31.ZAKO*/ wire STORE0_MATCHA = nor4(STORE0_MATCH0n, STORE0_MATCH1n, STORE0_MATCH2n, STORE0_MATCH3n);
    /*p31.XEBA*/ wire STORE0_MATCHB = nor4(STORE0_MATCH4n, STORE0_MATCH5n, STORE0_MATCH6n, STORE0_MATCH7n);
    /*p31.CYVY*/ wire STORE1_MATCHB = nor4(STORE1_MATCH4n, STORE1_MATCH5n, STORE1_MATCH6n, STORE1_MATCH7n);
    /*p31.EWAM*/ wire STORE1_MATCHA = nor4(STORE1_MATCH0n, STORE1_MATCH1n, STORE1_MATCH2n, STORE1_MATCH3n);
    /*p31.CEHU*/ wire STORE2_MATCHA = nor4(STORE2_MATCH4n, STORE2_MATCH5n, STORE2_MATCH6n, STORE2_MATCH7n);
    /*p31.EKES*/ wire STORE2_MATCHB = nor4(STORE2_MATCH0n, STORE2_MATCH1n, STORE2_MATCH2n, STORE2_MATCH3n);
    /*p31.ZURE*/ wire STORE3_MATCHA = nor4(STORE3_MATCH0n, STORE3_MATCH1n, STORE3_MATCH2n, STORE3_MATCH3n);
    /*p31.YWOS*/ wire STORE3_MATCHB = nor4(STORE3_MATCH4n, STORE3_MATCH5n, STORE3_MATCH6n, STORE3_MATCH7n);
    /*p31.YKOK*/ wire STORE4_MATCHA = nor4(STORE4_MATCH0n, STORE4_MATCH1n, STORE4_MATCH2n, STORE4_MATCH3n);
    /*p31.YNAZ*/ wire STORE4_MATCHB = nor4(STORE4_MATCH4n, STORE4_MATCH5n, STORE4_MATCH6n, STORE4_MATCH7n);
    /*p31.COGY*/ wire STORE5_MATCHA = nor4(STORE5_MATCH4n, STORE5_MATCH5n, STORE5_MATCH6n, STORE5_MATCH7n);
    /*p31.FYMA*/ wire STORE5_MATCHB = nor4(STORE5_MATCH0n, STORE5_MATCH1n, STORE5_MATCH2n, STORE5_MATCH3n);
    /*p31.YWAP*/ wire STORE6_MATCHA = nor4(STORE6_MATCH4n, STORE6_MATCH5n, STORE6_MATCH6n, STORE6_MATCH7n);
    /*p31.YDOT*/ wire STORE6_MATCHB = nor4(STORE6_MATCH0n, STORE6_MATCH1n, STORE6_MATCH2n, STORE6_MATCH3n);
    /*p31.CYCO*/ wire STORE7_MATCHA = nor4(STORE7_MATCH0n, STORE7_MATCH1n, STORE7_MATCH2n, STORE7_MATCH3n);
    /*p31.DAJE*/ wire STORE7_MATCHB = nor4(STORE7_MATCH4n, STORE7_MATCH5n, STORE7_MATCH6n, STORE7_MATCH7n);
    /*p31.DAMA*/ wire STORE8_MATCHA = nor4(STORE8_MATCH0n, STORE8_MATCH1n, STORE8_MATCH2n, STORE8_MATCH3n);
    /*p31.FEHA*/ wire STORE8_MATCHB = nor4(STORE8_MATCH4n, STORE8_MATCH5n, STORE8_MATCH6n, STORE8_MATCH7n);
    /*p31.YLEV*/ wire STORE9_MATCHA = nor4(STORE9_MATCH0n, STORE9_MATCH1n, STORE9_MATCH2n, STORE9_MATCH3n);
    /*p31.YTUB*/ wire STORE9_MATCHB = nor4(STORE9_MATCH4n, STORE9_MATCH5n, STORE9_MATCH6n, STORE9_MATCH7n);

    // CEHA := not(CENO_16.qn)
    // BYHO := not(CEHA);
    // AZEM := and2(BYJO, XYMU)
    // AROR := and2(AZEM, XYLO_08 (qn?) );

    /*p29.CEHA*/ wire CEHA_SCANNINGp = not1(top.sprite_scanner.CENO_SCANNINGp.qn());
    /*p29.BYJO*/ wire BYJO_SCANNINGn = not1(CEHA_SCANNINGp);
    /*p29.AZEM*/ wire AZEM_RENDERINGp = and2(BYJO_SCANNINGn, top.pix_pipe._XYMU_RENDERINGp.qp());
    /*p29.AROR*/ wire AROR_MATCH_ENp = and2(AZEM_RENDERINGp, top.pix_pipe.XYLO_LCDC_SPEN.qp());

    /*p29.YDUG*/ STORE0_MATCHn = nand3(AROR_MATCH_ENp, STORE0_MATCHA, STORE0_MATCHB);
    /*p29.DYDU*/ STORE1_MATCHn = nand3(AROR_MATCH_ENp, STORE1_MATCHA, STORE1_MATCHB);
    /*p29.DEGO*/ STORE2_MATCHn = nand3(AROR_MATCH_ENp, STORE2_MATCHA, STORE2_MATCHB);
    /*p29.YLOZ*/ STORE3_MATCHn = nand3(AROR_MATCH_ENp, STORE3_MATCHA, STORE3_MATCHB);
    /*p29.XAGE*/ STORE4_MATCHn = nand3(AROR_MATCH_ENp, STORE4_MATCHA, STORE4_MATCHB);
    /*p29.EGOM*/ STORE5_MATCHn = nand3(AROR_MATCH_ENp, STORE5_MATCHA, STORE5_MATCHB);
    /*p29.YBEZ*/ STORE6_MATCHn = nand3(AROR_MATCH_ENp, STORE6_MATCHA, STORE6_MATCHB);
    /*p29.DYKA*/ STORE7_MATCHn = nand3(AROR_MATCH_ENp, STORE7_MATCHA, STORE7_MATCHB);
    /*p29.EFYL*/ STORE8_MATCHn = nand3(AROR_MATCH_ENp, STORE8_MATCHA, STORE8_MATCHB);
    /*p29.YGEM*/ STORE9_MATCHn = nand3(AROR_MATCH_ENp, STORE9_MATCHA, STORE9_MATCHB);

    /*p29.FEFY*/ wire _FEFY = nand5(STORE4_MATCHn, STORE3_MATCHn, STORE2_MATCHn, STORE1_MATCHn, STORE0_MATCHn);
    /*p29.FOVE*/ wire _FOVE = nand5(STORE9_MATCHn, STORE8_MATCHn, STORE7_MATCHn, STORE6_MATCHn, STORE5_MATCHn);
    /*p29.FEPO*/ FEPO_STORE_MATCHp = or2(_FEFY, _FOVE);
  }
}

  //------------------------------------------------------------------------------

void SpriteStore::tock(const SchematicTop& top) {
  wire WEFE_VCC = 1;

  //----------------------------------------
  // Sprite scan Y matcher

  {
    // Sprite store counter.
    // The sprite count clock stops ticking once we have 10 sprites.

    /*p29.BAKY*/ wire _BAKY_SPRITES_FULL = and2(SPRITE_COUNT1.qp(), SPRITE_COUNT3.qp());
    /*p29.CAKE*/ wire _CAKE_CLKp = or2(_BAKY_SPRITES_FULL, DEZY_STORE_ENn.qp());
    
    // AZYB := not1(ATEJ)
    /*p28.AZYB*/ wire _AZYB_RST = not1(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp());
    
    /*p29.BESE*/ SPRITE_COUNT0 = dff17_AB(_CAKE_CLKp,         _AZYB_RST, SPRITE_COUNT0.qn());
    /*p29.CUXY*/ SPRITE_COUNT1 = dff17_AB(SPRITE_COUNT0.qn(), _AZYB_RST, SPRITE_COUNT1.qn());
    /*p29.BEGO*/ SPRITE_COUNT2 = dff17_AB(SPRITE_COUNT1.qn(), _AZYB_RST, SPRITE_COUNT2.qn());
    /*p29.DYBE*/ SPRITE_COUNT3 = dff17_AB(SPRITE_COUNT2.qn(), _AZYB_RST, SPRITE_COUNT3.qn());

    /*
    if (SPRITE_COUNT0.posedge()) {
      printf("??????\n");
    }
    */
  }

  //----------------------------------------

  {
    // FEPO_STORE_MATCHp here is weird, I guess it's just an easy signal to use to mux the bus?

    wire DEGE_SPRITE_DELTA0 = not1(top.sprite_scanner._ERUC_YDIFF_S0);
    wire DABY_SPRITE_DELTA1 = not1(top.sprite_scanner._ENEF_YDIFF_S1);
    wire DABU_SPRITE_DELTA2 = not1(top.sprite_scanner._FECO_YDIFF_S2);
    wire GYSA_SPRITE_DELTA3 = not1(top.sprite_scanner._GYKY_YDIFF_S3);

    /*p30.CUCU*/ SPR_TRI_L0 = tribuf_6n(FEPO_STORE_MATCHp, DEGE_SPRITE_DELTA0);
    /*p30.CUCA*/ SPR_TRI_L1 = tribuf_6n(FEPO_STORE_MATCHp, DABY_SPRITE_DELTA1);
    /*p30.CEGA*/ SPR_TRI_L2 = tribuf_6n(FEPO_STORE_MATCHp, DABU_SPRITE_DELTA2);
    /*p30.WENU*/ SPR_TRI_L3 = tribuf_6n(FEPO_STORE_MATCHp, GYSA_SPRITE_DELTA3);

    /*p29.DEZY*/ DEZY_STORE_ENn = dff17_B(top.clk_reg.ZEME_AxCxExGx(), top.clk_reg.XAPO_VID_RSTn(), top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH());
  }

  //----------------------------------------
  // Sprite store input driver.

  {
    /*p30.CYKE*/ wire _CYKE_ABxxEFxx = not1(top.clk_reg.XUPY_xxCDxxGH());
    /*p30.WUDA*/ wire _WUDA_xxCDxxGH = not1(_CYKE_ABxxEFxx);

    /*p28.YFOT*/ wire YFOT_OAM_A2n = not1(top.oam_bus.OAM_TRI_A2.qp());
    /*p28.YFOC*/ wire YFOC_OAM_A3n = not1(top.oam_bus.OAM_TRI_A3.qp());
    /*p28.YVOM*/ wire YVOM_OAM_A4n = not1(top.oam_bus.OAM_TRI_A4.qp());
    /*p28.YMEV*/ wire YMEV_OAM_A5n = not1(top.oam_bus.OAM_TRI_A5.qp());
    /*p28.XEMU*/ wire XEMU_OAM_A6n = not1(top.oam_bus.OAM_TRI_A6.qp());
    /*p28.YZET*/ wire YZET_OAM_A7n = not1(top.oam_bus.OAM_TRI_A7.qp());

    /*p30.XADU*/ XADU_SPRITE_IDX0n = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YFOT_OAM_A2n);
    /*p30.XEDY*/ XEDY_SPRITE_IDX1n = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YFOC_OAM_A3n);
    /*p30.ZUZE*/ ZUZE_SPRITE_IDX2n = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YVOM_OAM_A4n);
    /*p30.XOBE*/ XOBE_SPRITE_IDX3n = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YMEV_OAM_A5n);
    /*p30.YDUF*/ YDUF_SPRITE_IDX4n = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, XEMU_OAM_A6n);
    /*p30.XECU*/ XECU_SPRITE_IDX5n = dff13_A(_WUDA_xxCDxxGH, _CYKE_ABxxEFxx, WEFE_VCC, YZET_OAM_A7n);

    /*p30.WUZY*/ SPR_TRI_I0 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XADU_SPRITE_IDX0n.qn());
    /*p30.WYSE*/ SPR_TRI_I1 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XEDY_SPRITE_IDX1n.qn());
    /*p30.ZYSU*/ SPR_TRI_I2 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), ZUZE_SPRITE_IDX2n.qn());
    /*p30.WYDA*/ SPR_TRI_I3 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XOBE_SPRITE_IDX3n.qn());
    /*p30.WUCO*/ SPR_TRI_I4 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), YDUF_SPRITE_IDX4n.qn());
    /*p30.WEZA*/ SPR_TRI_I5 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XECU_SPRITE_IDX5n.qn());
  }

  //----------------------------------------
  // Sprite store getter

  {
    wire GND = 0;

    /*p29.WEFU*/ wire STORE0_MATCH = not1(STORE0_MATCHn);
    /*p29.GAJA*/ wire STORE1_MATCH = not1(STORE1_MATCHn);
    /*p29.GUPO*/ wire STORE2_MATCH = not1(STORE2_MATCHn);
    /*p29.WEBO*/ wire STORE3_MATCH = not1(STORE3_MATCHn);
    /*p29.WUNA*/ wire STORE4_MATCH = not1(STORE4_MATCHn);
    /*p29.GABA*/ wire STORE5_MATCH = not1(STORE5_MATCHn);
    /*p29.WASE*/ wire STORE6_MATCH = not1(STORE6_MATCHn);
    /*p29.GYTE*/ wire STORE7_MATCH = not1(STORE7_MATCHn);
    /*p29.GEKE*/ wire STORE8_MATCH = not1(STORE8_MATCHn);

    // Priority encoder so we fetch the first sprite that matches

    /*p29.GEZE*/ wire STORE0_MATCH_OUT = or2(STORE0_MATCH, GND);
    /*p29.FUMA*/ wire STORE1_MATCH_OUT = or2(STORE1_MATCH, STORE0_MATCH_OUT);
    /*p29.GEDE*/ wire STORE2_MATCH_OUT = or2(STORE2_MATCH, STORE1_MATCH_OUT);
    /*p29.WUTO*/ wire STORE3_MATCH_OUT = or2(STORE3_MATCH, STORE2_MATCH_OUT);
    /*p29.XYLA*/ wire STORE4_MATCH_OUT = or2(STORE4_MATCH, STORE3_MATCH_OUT);
    /*p29.WEJA*/ wire STORE5_MATCH_OUT = or2(STORE5_MATCH, STORE4_MATCH_OUT);
    /*p29.WYLA*/ wire STORE6_MATCH_OUT = or2(STORE6_MATCH, STORE5_MATCH_OUT);
    /*p29.FAVO*/ wire STORE7_MATCH_OUT = or2(STORE7_MATCH, STORE6_MATCH_OUT);
    /*p29.GYGA*/ wire STORE8_MATCH_OUT = or2(STORE8_MATCH, STORE7_MATCH_OUT);

    /*p29.GUVA*/ wire _GUVA_SPRITE0_GETp = nor2(STORE0_MATCHn, GND);
    /*p29.ENUT*/ wire _ENUT_SPRITE1_GETp = nor2(STORE1_MATCHn, STORE0_MATCH_OUT);
    /*p29.EMOL*/ wire _EMOL_SPRITE2_GETp = nor2(STORE2_MATCHn, STORE1_MATCH_OUT);
    /*p29.GYFY*/ wire _GYFY_SPRITE3_GETp = nor2(STORE3_MATCHn, STORE2_MATCH_OUT);
    /*p29.GONO*/ wire _GONO_SPRITE4_GETp = nor2(STORE4_MATCHn, STORE3_MATCH_OUT);
    /*p29.GEGA*/ wire _GEGA_SPRITE5_GETp = nor2(STORE5_MATCHn, STORE4_MATCH_OUT);
    /*p29.XOJA*/ wire _XOJA_SPRITE6_GETp = nor2(STORE6_MATCHn, STORE5_MATCH_OUT);
    /*p29.GUTU*/ wire _GUTU_SPRITE7_GETp = nor2(STORE7_MATCHn, STORE6_MATCH_OUT);
    /*p29.FOXA*/ wire _FOXA_SPRITE8_GETp = nor2(STORE8_MATCHn, STORE7_MATCH_OUT);
    /*p29.GUZE*/ wire _GUZE_SPRITE9_GETp = nor2(STORE9_MATCHn, STORE8_MATCH_OUT);

    // Delayed reset signal for the selected store

    /*p28.ABAK*/ wire ABAK_VID_LINE_TRIGp = or2(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp(), top.clk_reg.AMYG_VID_RSTp());
    /*p28.BYVA*/ wire BYVA_VID_LINE_TRIGn = not1(ABAK_VID_LINE_TRIGp);

    /*p29.TYNO*/ wire TYNO = nand3(top.sprite_fetcher._TOXE_SFETCH_S0.qp(), top.sprite_fetcher._SEBA_SFETCH_S1_D5.qp(), top.sprite_fetcher._VONU_SFETCH_S1_D4.qp());
    /*p29.VUSA*/ wire VUSA_SPRITE_DONEn  = or2(top.sprite_fetcher._TYFO_SFETCH_S0_D1.qn(), TYNO);
    /*p29.WUTY*/ wire WUTY_SPRITE_DONEp  = not1(VUSA_SPRITE_DONEn);

    /*p29.EBOJ*/ EBOJ_STORE0_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _GUVA_SPRITE0_GETp);
    /*p29.CEDY*/ CEDY_STORE1_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _ENUT_SPRITE1_GETp);
    /*p29.EGAV*/ EGAV_STORE2_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _EMOL_SPRITE2_GETp);
    /*p29.GOTA*/ GOTA_STORE3_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _GYFY_SPRITE3_GETp);
    /*p29.XUDY*/ XUDY_STORE4_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _GONO_SPRITE4_GETp);
    /*p29.WAFY*/ WAFY_STORE5_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _GEGA_SPRITE5_GETp);
    /*p29.WOMY*/ WOMY_STORE6_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _XOJA_SPRITE6_GETp);
    /*p29.WAPO*/ WAPO_STORE7_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _GUTU_SPRITE7_GETp);
    /*p29.EXUQ*/ EXUQ_STORE8_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _FOXA_SPRITE8_GETp);
    /*p29.FONO*/ FONO_STORE9_RSTp = dff17_B(WUTY_SPRITE_DONEp, BYVA_VID_LINE_TRIGn, _GUZE_SPRITE9_GETp);

    /*p29.FURO*/ wire FURO_SPRITE0_GETn = not1(_GUVA_SPRITE0_GETp);
    /*p29.DYDO*/ wire DYDO_SPRITE1_GETn = not1(_ENUT_SPRITE1_GETp);
    /*p29.FAME*/ wire FAME_SPRITE2_GETn = not1(_EMOL_SPRITE2_GETp);
    /*p29.GYMA*/ wire GYMA_SPRITE3_GETn = not1(_GYFY_SPRITE3_GETp);
    /*p29.GOWO*/ wire GOWO_SPRITE4_GETn = not1(_GONO_SPRITE4_GETp);
    /*p29.GYGY*/ wire GYGY_SPRITE5_GETn = not1(_GEGA_SPRITE5_GETp);
    /*p29.XYME*/ wire XYME_SPRITE6_GETn = not1(_XOJA_SPRITE6_GETp);
    /*p29.GUGY*/ wire GUGY_SPRITE7_GETn = not1(_GUTU_SPRITE7_GETp);
    /*p29.DENY*/ wire DENY_SPRITE8_GETn = not1(_FOXA_SPRITE8_GETp);
    /*p29.FADO*/ wire FADO_SPRITE9_GETn = not1(_GUZE_SPRITE9_GETp);

#if 0

    // Resetting the store X coords to 0 doesn't make sense, as they'd fire during a line even if we never stored any sprites.
    // I guess it must store inverted X, so that when reset X = 0xFF?

    if (AMYG_VID_RSTp()) {
      /*p29.EBOJ*/ EBOJ_STORE0_RSTp = dff17(WUTY_SPRITE_DONEp(), 0, _GUVA_SPRITE0_GETp);
      /*p31.XEPE*/ XEPE_STORE0_X0   = dff9(FUXU_STORE0_CLKp,  0, ZAGO_OAM_DB0);
    }


    /*p28.ABAK*/ wire ABAK_VID_LINE_TRIGp = or2(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp(), top.clk_reg.AMYG_VID_RSTp());
    /*p28.BYVA*/ wire BYVA_VID_LINE_TRIGn = not1(ABAK_VID_LINE_TRIGp);
    /*p29.DYBA*/ wire DYBA_VID_LINE_TRIGp = not1(BYVA_VID_LINE_TRIGn);
    /*p29.EBOJ*/ EBOJ_STORE0_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GUVA_SPRITE0_GETp);
    /*p29.DYWE*/ wire DYWE_STORE0_RSTp = or2(DYBA_VID_LINE_TRIGp, EBOJ_STORE0_RSTp.qp());
    /*p29.DYNA*/ wire DYNA_STORE0_RSTn = not1(DYWE_STORE0_RSTp);
    /*p31.XEPE*/ XEPE_STORE0_X0    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZAGO_OAM_DB0);
#endif

    // ZEDY := tri(FURO, YWAK)
    // CUBO := tri(DYDO, CADU)

    /*p30.ZETU*/ SPR_TRI_I0 = tribuf_6n(FURO_SPRITE0_GETn, YGUS_STORE0_I0.qp());
    /*p30.ZECE*/ SPR_TRI_I1 = tribuf_6n(FURO_SPRITE0_GETn, YSOK_STORE0_I1.qp());
    /*p30.ZAVE*/ SPR_TRI_I2 = tribuf_6n(FURO_SPRITE0_GETn, YZEP_STORE0_I2.qp());
    /*p30.WOKO*/ SPR_TRI_I3 = tribuf_6n(FURO_SPRITE0_GETn, WYTE_STORE0_I3.qp());
    /*p30.ZUMU*/ SPR_TRI_I4 = tribuf_6n(FURO_SPRITE0_GETn, ZONY_STORE0_I4.qp());
    /*p30.ZEDY*/ SPR_TRI_I5 = tribuf_6n(FURO_SPRITE0_GETn, YWAK_STORE0_I5.qp());

    /*p30.CUBO*/ SPR_TRI_I0 = tribuf_6n(DYDO_SPRITE1_GETn, CADU_STORE1_I0.qp());
    /*p30.CELU*/ SPR_TRI_I1 = tribuf_6n(DYDO_SPRITE1_GETn, CEBO_STORE1_I1.qp());
    /*p30.CEGY*/ SPR_TRI_I2 = tribuf_6n(DYDO_SPRITE1_GETn, CUFA_STORE1_I2.qp());
    /*p30.BETY*/ SPR_TRI_I3 = tribuf_6n(DYDO_SPRITE1_GETn, COMA_STORE1_I3.qp());
    /*p30.CYBY*/ SPR_TRI_I4 = tribuf_6n(DYDO_SPRITE1_GETn, CUZA_STORE1_I4.qp());
    /*p30.BEMO*/ SPR_TRI_I5 = tribuf_6n(DYDO_SPRITE1_GETn, CAJY_STORE1_I5.qp());

    /*p30.CUBE*/ SPR_TRI_I0 = tribuf_6n(FAME_SPRITE2_GETn, BUHE_STORE2_I0.qp());
    /*p30.AFOZ*/ SPR_TRI_I1 = tribuf_6n(FAME_SPRITE2_GETn, BYHU_STORE2_I1.qp());
    /*p30.APON*/ SPR_TRI_I2 = tribuf_6n(FAME_SPRITE2_GETn, BECA_STORE2_I2.qp());
    /*p30.CUVU*/ SPR_TRI_I3 = tribuf_6n(FAME_SPRITE2_GETn, BULU_STORE2_I3.qp());
    /*p30.CYRO*/ SPR_TRI_I4 = tribuf_6n(FAME_SPRITE2_GETn, BUNA_STORE2_I4.qp());
    /*p30.AXEC*/ SPR_TRI_I5 = tribuf_6n(FAME_SPRITE2_GETn, BOXA_STORE2_I5.qp());

    /*p30.ENAP*/ SPR_TRI_I0 = tribuf_6n(GYMA_SPRITE3_GETn, DEVY_STORE3_I0.qp());
    /*p30.DYGO*/ SPR_TRI_I1 = tribuf_6n(GYMA_SPRITE3_GETn, DESE_STORE3_I1.qp());
    /*p30.DOWA*/ SPR_TRI_I2 = tribuf_6n(GYMA_SPRITE3_GETn, DUNY_STORE3_I2.qp());
    /*p30.DONY*/ SPR_TRI_I3 = tribuf_6n(GYMA_SPRITE3_GETn, DUHA_STORE3_I3.qp());
    /*p30.EFUD*/ SPR_TRI_I4 = tribuf_6n(GYMA_SPRITE3_GETn, DEBA_STORE3_I4.qp());
    /*p30.DEZU*/ SPR_TRI_I5 = tribuf_6n(GYMA_SPRITE3_GETn, DAFU_STORE3_I5.qp());

    /*p30.WUXU*/ SPR_TRI_I0 = tribuf_6n(GOWO_SPRITE4_GETn, XAVE_STORE4_I0.qp());
    /*p30.WEPY*/ SPR_TRI_I1 = tribuf_6n(GOWO_SPRITE4_GETn, XEFE_STORE4_I1.qp());
    /*p30.WERU*/ SPR_TRI_I2 = tribuf_6n(GOWO_SPRITE4_GETn, WANU_STORE4_I2.qp());
    /*p30.XYRE*/ SPR_TRI_I3 = tribuf_6n(GOWO_SPRITE4_GETn, XABO_STORE4_I3.qp());
    /*p30.WOXY*/ SPR_TRI_I4 = tribuf_6n(GOWO_SPRITE4_GETn, XEGE_STORE4_I4.qp());
    /*p30.WAJA*/ SPR_TRI_I5 = tribuf_6n(GOWO_SPRITE4_GETn, XYNU_STORE4_I5.qp());

    /*p30.DOBO*/ SPR_TRI_I0 = tribuf_6n(GYGY_SPRITE5_GETn, EKOP_STORE5_I0.qp());
    /*p30.DYNY*/ SPR_TRI_I1 = tribuf_6n(GYGY_SPRITE5_GETn, ETYM_STORE5_I1.qp());
    /*p30.WAGA*/ SPR_TRI_I2 = tribuf_6n(GYGY_SPRITE5_GETn, GORU_STORE5_I2.qp());
    /*p30.DUZA*/ SPR_TRI_I3 = tribuf_6n(GYGY_SPRITE5_GETn, EBEX_STORE5_I3.qp());
    /*p30.DALY*/ SPR_TRI_I4 = tribuf_6n(GYGY_SPRITE5_GETn, ETAV_STORE5_I4.qp());
    /*p30.DALO*/ SPR_TRI_I5 = tribuf_6n(GYGY_SPRITE5_GETn, EKAP_STORE5_I5.qp());

    /*p30.WATO*/ SPR_TRI_I0 = tribuf_6n(XYME_SPRITE6_GETn, GABO_STORE6_I0.qp());
    /*p30.WYWY*/ SPR_TRI_I1 = tribuf_6n(XYME_SPRITE6_GETn, GACY_STORE6_I1.qp());
    /*p30.EZOC*/ SPR_TRI_I2 = tribuf_6n(XYME_SPRITE6_GETn, FOGO_STORE6_I2.qp());
    /*p30.WABO*/ SPR_TRI_I3 = tribuf_6n(XYME_SPRITE6_GETn, GOHU_STORE6_I3.qp());
    /*p30.ELYC*/ SPR_TRI_I4 = tribuf_6n(XYME_SPRITE6_GETn, FOXY_STORE6_I4.qp());
    /*p30.WOCY*/ SPR_TRI_I5 = tribuf_6n(XYME_SPRITE6_GETn, GECU_STORE6_I5.qp());

    /*p30.WAKO*/ SPR_TRI_I0 = tribuf_6n(GUGY_SPRITE7_GETn, GULE_STORE7_I0.qp());
    /*p30.WYGO*/ SPR_TRI_I1 = tribuf_6n(GUGY_SPRITE7_GETn, GYNO_STORE7_I1.qp());
    /*p30.ELEP*/ SPR_TRI_I2 = tribuf_6n(GUGY_SPRITE7_GETn, FEFA_STORE7_I2.qp());
    /*p30.ETAD*/ SPR_TRI_I3 = tribuf_6n(GUGY_SPRITE7_GETn, FYSU_STORE7_I3.qp());
    /*p30.WABA*/ SPR_TRI_I4 = tribuf_6n(GUGY_SPRITE7_GETn, GESY_STORE7_I4.qp());
    /*p30.EVYT*/ SPR_TRI_I5 = tribuf_6n(GUGY_SPRITE7_GETn, FUZO_STORE7_I5.qp());

    /*p30.APOC*/ SPR_TRI_I0 = tribuf_6n(DENY_SPRITE8_GETn, AXUV_STORE8_I0.qp());
    /*p30.AKYH*/ SPR_TRI_I1 = tribuf_6n(DENY_SPRITE8_GETn, BADA_STORE8_I1.qp());
    /*p30.AFEN*/ SPR_TRI_I2 = tribuf_6n(DENY_SPRITE8_GETn, APEV_STORE8_I2.qp());
    /*p30.APYV*/ SPR_TRI_I3 = tribuf_6n(DENY_SPRITE8_GETn, BADO_STORE8_I3.qp());
    /*p30.APOB*/ SPR_TRI_I4 = tribuf_6n(DENY_SPRITE8_GETn, BEXY_STORE8_I4.qp());
    /*p30.ADYB*/ SPR_TRI_I5 = tribuf_6n(DENY_SPRITE8_GETn, BYHE_STORE8_I5.qp());

    /*p30.ZARO*/ SPR_TRI_I0 = tribuf_6n(FADO_SPRITE9_GETn, YBER_STORE9_I0.qp());
    /*p30.ZOJY*/ SPR_TRI_I1 = tribuf_6n(FADO_SPRITE9_GETn, YZOR_STORE9_I1.qp());
    /*p30.YNEV*/ SPR_TRI_I2 = tribuf_6n(FADO_SPRITE9_GETn, XYFE_STORE9_I2.qp());
    /*p30.XYRA*/ SPR_TRI_I3 = tribuf_6n(FADO_SPRITE9_GETn, XOTU_STORE9_I3.qp());
    /*p30.YRAD*/ SPR_TRI_I4 = tribuf_6n(FADO_SPRITE9_GETn, XUTE_STORE9_I4.qp());
    /*p30.YHAL*/ SPR_TRI_I5 = tribuf_6n(FADO_SPRITE9_GETn, XUFO_STORE9_I5.qp());


    /*p30.WEHE*/ SPR_TRI_L0 = tribuf_6n(FURO_SPRITE0_GETn, GYHO_STORE0_L0.qp());
    /*p30.BUKY*/ SPR_TRI_L1 = tribuf_6n(FURO_SPRITE0_GETn, CUFO_STORE0_L1.qp());
    /*p30.AJAL*/ SPR_TRI_L2 = tribuf_6n(FURO_SPRITE0_GETn, BOZU_STORE0_L2.qp());
    /*p30.GOFO*/ SPR_TRI_L3 = tribuf_6n(FURO_SPRITE0_GETn, FYHY_STORE0_L3.qp());

    /*p30.BYRO*/ SPR_TRI_L0 = tribuf_6n(DYDO_SPRITE1_GETn, AMES_STORE1_L0.qp());
    /*p30.AHUM*/ SPR_TRI_L1 = tribuf_6n(DYDO_SPRITE1_GETn, AROF_STORE1_L1.qp());
    /*p30.BACO*/ SPR_TRI_L2 = tribuf_6n(DYDO_SPRITE1_GETn, ABOP_STORE1_L2.qp());
    /*p30.BEFE*/ SPR_TRI_L3 = tribuf_6n(DYDO_SPRITE1_GETn, ABUG_STORE1_L3.qp());

    /*p30.ZUKE*/ SPR_TRI_L0 = tribuf_6n(FAME_SPRITE2_GETn, YLOV_STORE2_L0.qp());
    /*p30.WERE*/ SPR_TRI_L1 = tribuf_6n(FAME_SPRITE2_GETn, XOSY_STORE2_L1.qp());
    /*p30.WUXE*/ SPR_TRI_L2 = tribuf_6n(FAME_SPRITE2_GETn, XAZY_STORE2_L2.qp());
    /*p30.ZABY*/ SPR_TRI_L3 = tribuf_6n(FAME_SPRITE2_GETn, YKUK_STORE2_L3.qp());

    /*p30.ZEXE*/ SPR_TRI_L0 = tribuf_6n(GYMA_SPRITE3_GETn, ZURO_STORE3_L0.qp());
    /*p30.YWAV*/ SPR_TRI_L1 = tribuf_6n(GYMA_SPRITE3_GETn, ZYLU_STORE3_L1.qp());
    /*p30.YJEM*/ SPR_TRI_L2 = tribuf_6n(GYMA_SPRITE3_GETn, ZENE_STORE3_L2.qp());
    /*p30.ZYPO*/ SPR_TRI_L3 = tribuf_6n(GYMA_SPRITE3_GETn, ZURY_STORE3_L3.qp());

    /*p30.BUCE*/ SPR_TRI_L0 = tribuf_6n(GOWO_SPRITE4_GETn, CAPO_STORE4_L0.qp());
    /*p30.BEVY*/ SPR_TRI_L1 = tribuf_6n(GOWO_SPRITE4_GETn, CAJU_STORE4_L1.qp());
    /*p30.BOVE*/ SPR_TRI_L2 = tribuf_6n(GOWO_SPRITE4_GETn, CONO_STORE4_L2.qp());
    /*p30.BYDO*/ SPR_TRI_L3 = tribuf_6n(GOWO_SPRITE4_GETn, CUMU_STORE4_L3.qp());

    /*p30.BACE*/ SPR_TRI_L0 = tribuf_6n(GYGY_SPRITE5_GETn, ACEP_STORE5_L0.qp());
    /*p30.BUJA*/ SPR_TRI_L1 = tribuf_6n(GYGY_SPRITE5_GETn, ABEG_STORE5_L1.qp());
    /*p30.BODU*/ SPR_TRI_L2 = tribuf_6n(GYGY_SPRITE5_GETn, ABUX_STORE5_L2.qp());
    /*p30.AWAT*/ SPR_TRI_L3 = tribuf_6n(GYGY_SPRITE5_GETn, ANED_STORE5_L3.qp());

    /*p30.YBUK*/ SPR_TRI_L0 = tribuf_6n(XYME_SPRITE6_GETn, ZUMY_STORE6_L0.qp());
    /*p30.YKOZ*/ SPR_TRI_L1 = tribuf_6n(XYME_SPRITE6_GETn, ZAFU_STORE6_L1.qp());
    /*p30.ZYTO*/ SPR_TRI_L2 = tribuf_6n(XYME_SPRITE6_GETn, ZEXO_STORE6_L2.qp());
    /*p30.ZUDO*/ SPR_TRI_L3 = tribuf_6n(XYME_SPRITE6_GETn, ZUBE_STORE6_L3.qp());

    /*p30.WAXE*/ SPR_TRI_L0 = tribuf_6n(GUGY_SPRITE7_GETn, XYNA_STORE7_L0.qp());
    /*p30.YPOZ*/ SPR_TRI_L1 = tribuf_6n(GUGY_SPRITE7_GETn, YGUM_STORE7_L1.qp());
    /*p30.WABU*/ SPR_TRI_L2 = tribuf_6n(GUGY_SPRITE7_GETn, XAKU_STORE7_L2.qp());
    /*p30.WANA*/ SPR_TRI_L3 = tribuf_6n(GUGY_SPRITE7_GETn, XYGO_STORE7_L3.qp());

    /*p30.BOSO*/ SPR_TRI_L0 = tribuf_6n(DENY_SPRITE8_GETn, AZAP_STORE8_L0.qp());
    /*p30.BAZU*/ SPR_TRI_L1 = tribuf_6n(DENY_SPRITE8_GETn, AFYX_STORE8_L1.qp());
    /*p30.AHAC*/ SPR_TRI_L2 = tribuf_6n(DENY_SPRITE8_GETn, AFUT_STORE8_L2.qp());
    /*p30.BUJY*/ SPR_TRI_L3 = tribuf_6n(DENY_SPRITE8_GETn, AFYM_STORE8_L3.qp());

    /*p30.BYME*/ SPR_TRI_L0 = tribuf_6n(FADO_SPRITE9_GETn, CANA_STORE9_L0.qp());
    /*p30.GATE*/ SPR_TRI_L1 = tribuf_6n(FADO_SPRITE9_GETn, FOFO_STORE9_L1.qp());
    /*p30.COHO*/ SPR_TRI_L2 = tribuf_6n(FADO_SPRITE9_GETn, DYSY_STORE9_L2.qp());
    /*p30.CAWO*/ SPR_TRI_L3 = tribuf_6n(FADO_SPRITE9_GETn, DEWU_STORE9_L3.qp());

  }

  //----------------------------------------
  // Sprite store setter

  {
    // ABAK := or(ATEJ, AMYG)
    // BYVA := not(ABAK)
    // DYBA := not(BYVA)

    /*p28.ABAK*/ wire _ABAK_VID_LINE_TRIGp = or2(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp(), top.clk_reg.AMYG_VID_RSTp());
    /*p28.BYVA*/ wire _BYVA_VID_LINE_TRIGn = not1(_ABAK_VID_LINE_TRIGp);
    /*p29.DYBA*/ wire _DYBA_VID_LINE_TRIGp = not1(_BYVA_VID_LINE_TRIGn);

    /*p29.DYWE*/ wire DYWE_STORE0_RSTp = or2(_DYBA_VID_LINE_TRIGp, EBOJ_STORE0_RSTp.qp());
    /*p29.EFEV*/ wire EFEV_STORE1_RSTp = or2(_DYBA_VID_LINE_TRIGp, CEDY_STORE1_RSTp.qp());
    /*p29.FOKO*/ wire FOKO_STORE2_RSTp = or2(_DYBA_VID_LINE_TRIGp, EGAV_STORE2_RSTp.qp());
    /*p29.GAKE*/ wire GAKE_STORE3_RSTp = or2(_DYBA_VID_LINE_TRIGp, GOTA_STORE3_RSTp.qp());
    /*p29.WOHU*/ wire WOHU_STORE4_RSTp = or2(_DYBA_VID_LINE_TRIGp, XUDY_STORE4_RSTp.qp());
    /*p29.FEVE*/ wire FEVE_STORE5_RSTp = or2(_DYBA_VID_LINE_TRIGp, WAFY_STORE5_RSTp.qp());
    /*p29.WACY*/ wire WACY_STORE6_RSTp = or2(_DYBA_VID_LINE_TRIGp, WOMY_STORE6_RSTp.qp());
    /*p29.GUKY*/ wire GUKY_STORE7_RSTp = or2(_DYBA_VID_LINE_TRIGp, WAPO_STORE7_RSTp.qp());
    /*p29.GORO*/ wire GORO_STORE8_RSTp = or2(_DYBA_VID_LINE_TRIGp, EXUQ_STORE8_RSTp.qp());
    /*p29.DUBU*/ wire DUBU_STORE9_RSTp = or2(_DYBA_VID_LINE_TRIGp, FONO_STORE9_RSTp.qp());

    /*p29.DYNA*/ wire DYNA_STORE0_RSTn = not1(DYWE_STORE0_RSTp);
    /*p29.DOKU*/ wire DOKU_STORE1_RSTn = not1(EFEV_STORE1_RSTp);
    /*p29.GAMY*/ wire GAMY_STORE2_RSTn = not1(FOKO_STORE2_RSTp);
    /*p29.WUPA*/ wire WUPA_STORE3_RSTn = not1(GAKE_STORE3_RSTp);
    /*p29.WUNU*/ wire WUNU_STORE4_RSTn = not1(WOHU_STORE4_RSTp);
    /*p29.EJAD*/ wire EJAD_STORE5_RSTn = not1(FEVE_STORE5_RSTp);
    /*p29.XAHO*/ wire XAHO_STORE6_RSTn = not1(WACY_STORE6_RSTp);
    /*p29.GAFY*/ wire GAFY_STORE7_RSTn = not1(GUKY_STORE7_RSTp);
    /*p29.WUZO*/ wire WUZO_STORE8_RSTn = not1(GORO_STORE8_RSTp);
    /*p29.DOSY*/ wire DOSY_STORE9_RSTn = not1(DUBU_STORE9_RSTp);

    /*p29.EDEN*/ wire EDEN_SPRITE_COUNT0n = not1(SPRITE_COUNT0.qp());
    /*p29.CYPY*/ wire CYPY_SPRITE_COUNT1n = not1(SPRITE_COUNT1.qp());
    /*p29.CAPE*/ wire CAPE_SPRITE_COUNT2n = not1(SPRITE_COUNT2.qp());
    /*p29.CAXU*/ wire CAXU_SPRITE_COUNT3n = not1(SPRITE_COUNT3.qp());

    /*p29.FYCU*/ wire FYCU_SPRITE_COUNT0p = not1(EDEN_SPRITE_COUNT0n);
    /*p29.FONE*/ wire FONE_SPRITE_COUNT1p = not1(CYPY_SPRITE_COUNT1n);
    /*p29.EKUD*/ wire EKUD_SPRITE_COUNT2p = not1(CAPE_SPRITE_COUNT2n);
    /*p29.ELYG*/ wire ELYG_SPRITE_COUNT3p = not1(CAXU_SPRITE_COUNT3n);

    /*p29.DEZO*/ wire DEZO_STORE0_SELn = nand4(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.CUVA*/ wire CUVA_STORE1_SELn = nand4(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.GEBU*/ wire GEBU_STORE2_SELn = nand4(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.FOCO*/ wire FOCO_STORE3_SELn = nand4(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.CUPE*/ wire CUPE_STORE4_SELn = nand4(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.CUGU*/ wire CUGU_STORE5_SELn = nand4(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.WOMU*/ wire WOMU_STORE6_SELn = nand4(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.GUNA*/ wire GUNA_STORE7_SELn = nand4(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.DEWY*/ wire DEWY_STORE8_SELn = nand4(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);
    /*p29.DOGU*/ wire DOGU_STORE9_SELn = nand4(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);

    // Sprite stores latch their input when their SELn signal goes _high_
    /*p29.CEMY*/ wire CEMY_STORE0_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), DEZO_STORE0_SELn);
    /*p29.BYBY*/ wire BYBY_STORE1_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), CUVA_STORE1_SELn);
    /*p29.WYXO*/ wire WYXO_STORE2_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), GEBU_STORE2_SELn);
    /*p29.GUVE*/ wire GUVE_STORE3_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), FOCO_STORE3_SELn);
    /*p29.CECU*/ wire CECU_STORE4_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), CUPE_STORE4_SELn);
    /*p29.CADO*/ wire CADO_STORE5_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), CUGU_STORE5_SELn);
    /*p29.XUJO*/ wire XUJO_STORE6_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), WOMU_STORE6_SELn);
    /*p29.GAPE*/ wire GAPE_STORE7_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), GUNA_STORE7_SELn);
    /*p29.CAHO*/ wire CAHO_STORE8_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), DEWY_STORE8_SELn);
    /*p29.CATO*/ wire CATO_STORE9_CLKp = or2(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), DOGU_STORE9_SELn);

    /*p29.DYHU*/ wire DYHU_STORE0_CLKn = not1(CEMY_STORE0_CLKp);
    /*p29.BUCO*/ wire BUCO_STORE1_CLKn = not1(BYBY_STORE1_CLKp);
    /*p29.GYFO*/ wire GYFO_STORE2_CLKn = not1(WYXO_STORE2_CLKp);
    /*p29.GUSA*/ wire GUSA_STORE3_CLKn = not1(GUVE_STORE3_CLKp);
    /*p29.DUKE*/ wire DUKE_STORE4_CLKn = not1(CECU_STORE4_CLKp);
    /*p29.BEDE*/ wire BEDE_STORE5_CLKn = not1(CADO_STORE5_CLKp);
    /*p29.WEKA*/ wire WEKA_STORE6_CLKn = not1(XUJO_STORE6_CLKp);
    /*p29.GYVO*/ wire GYVO_STORE7_CLKn = not1(GAPE_STORE7_CLKp);
    /*p29.BUKA*/ wire BUKA_STORE8_CLKn = not1(CAHO_STORE8_CLKp);
    /*p29.DECU*/ wire DECU_STORE9_CLKn = not1(CATO_STORE9_CLKp);

    // 10 sprite stores

    // ZAGO := not(YLOR_Q)

    /*p31.ZAGO*/ wire ZAGO_OAM_DB0 = not1(top.oam_bus.YLOR_OAM_DB0.qp());
    /*p31.ZOCY*/ wire ZOCY_OAM_DB1 = not1(top.oam_bus.ZYTY_OAM_DB1.qp());
    /*p31.YPUR*/ wire YPUR_OAM_DB2 = not1(top.oam_bus.ZYVE_OAM_DB2.qp());
    /*p31.YVOK*/ wire YVOK_OAM_DB3 = not1(top.oam_bus.ZEZY_OAM_DB3.qp());
    /*p31.COSE*/ wire COSE_OAM_DB4 = not1(top.oam_bus.GOMO_OAM_DB4.qp());
    /*p31.AROP*/ wire AROP_OAM_DB5 = not1(top.oam_bus.BAXO_OAM_DB5.qp());
    /*p31.XATU*/ wire XATU_OAM_DB6 = not1(top.oam_bus.YZOS_OAM_DB6.qp());
    /*p31.BADY*/ wire BADY_OAM_DB7 = not1(top.oam_bus.DEPO_OAM_DB7.qp());

    /*p29.GENY*/ wire GENY_STORE0_CLKp = not1(DYHU_STORE0_CLKn);
    /*p29.ENOB*/ wire ENOB_STORE0_CLKp = not1(DYHU_STORE0_CLKn);
    /*p29.FUXU*/ wire FUXU_STORE0_CLKp = not1(DYHU_STORE0_CLKn);

    /*p30.YGUS*/ YGUS_STORE0_I0 = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_I0.qp());
    /*p30.YSOK*/ YSOK_STORE0_I1 = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_I1.qp());
    /*p30.YZEP*/ YZEP_STORE0_I2 = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_I2.qp());
    /*p30.WYTE*/ WYTE_STORE0_I3 = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_I3.qp());
    /*p30.ZONY*/ ZONY_STORE0_I4 = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_I4.qp());
    /*p30.YWAK*/ YWAK_STORE0_I5 = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_I5.qp());

    /*p30.GYHO*/ GYHO_STORE0_L0 = dff8_B(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_L0.qp());
    /*p30.CUFO*/ CUFO_STORE0_L1 = dff8_B(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_L1.qp());
    /*p30.BOZU*/ BOZU_STORE0_L2 = dff8_B(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_L2.qp());
    /*p30.FYHY*/ FYHY_STORE0_L3 = dff8_B(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_L3.qp());

    /*p31.XEPE*/ XEPE_STORE0_X0 = dff9_A(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZAGO_OAM_DB0);
    /*p31.YLAH*/ YLAH_STORE0_X1 = dff9_A(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZOCY_OAM_DB1);
    /*p31.ZOLA*/ ZOLA_STORE0_X2 = dff9_A(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YPUR_OAM_DB2);
    /*p31.ZULU*/ ZULU_STORE0_X3 = dff9_A(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YVOK_OAM_DB3);
    /*p31.WELO*/ WELO_STORE0_X4 = dff9_A(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, COSE_OAM_DB4);
    /*p31.XUNY*/ XUNY_STORE0_X5 = dff9_A(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, AROP_OAM_DB5);
    /*p31.WOTE*/ WOTE_STORE0_X6 = dff9_A(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, XATU_OAM_DB6);
    /*p31.XAKO*/ XAKO_STORE0_X7 = dff9_A(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, BADY_OAM_DB7);

    /*p29.BYVY*/ wire BYVU_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
    /*p29.AHOF*/ wire AHOF_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
    /*p29.ASYS*/ wire ASYS_STORE1_CLKp = not1(BUCO_STORE1_CLKn);

    /*p30.CADU*/ CADU_STORE1_I0 = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_I0.qp());
    /*p30.CEBO*/ CEBO_STORE1_I1 = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_I1.qp());
    /*p30.CUFA*/ CUFA_STORE1_I2 = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_I2.qp());
    /*p30.COMA*/ COMA_STORE1_I3 = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_I3.qp());
    /*p30.CUZA*/ CUZA_STORE1_I4 = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_I4.qp());
    /*p30.CAJY*/ CAJY_STORE1_I5 = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_I5.qp());

    /*p30.AMES*/ AMES_STORE1_L0 = dff8_B(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_L0.qp());
    /*p30.AROF*/ AROF_STORE1_L1 = dff8_B(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_L1.qp());
    /*p30.ABOP*/ ABOP_STORE1_L2 = dff8_B(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_L2.qp());
    /*p30.ABUG*/ ABUG_STORE1_L3 = dff8_B(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_L3.qp());

    /*p31.DANY*/ DANY_STORE1_X0 = dff9_A(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZAGO_OAM_DB0);
    /*p31.DUKO*/ DUKO_STORE1_X1 = dff9_A(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZOCY_OAM_DB1);
    /*p31.DESU*/ DESU_STORE1_X2 = dff9_A(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YPUR_OAM_DB2);
    /*p31.DAZO*/ DAZO_STORE1_X3 = dff9_A(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YVOK_OAM_DB3);
    /*p31.DAKE*/ DAKE_STORE1_X4 = dff9_A(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, COSE_OAM_DB4);
    /*p31.CESO*/ CESO_STORE1_X5 = dff9_A(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, AROP_OAM_DB5);
    /*p31.DYFU*/ DYFU_STORE1_X6 = dff9_A(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, XATU_OAM_DB6);
    /*p31.CUSY*/ CUSY_STORE1_X7 = dff9_A(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, BADY_OAM_DB7);

    /*p29.BUZY*/ wire BUZY_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
    /*p29.FUKE*/ wire FUKE_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
    /*p29.CACU*/ wire CACU_STORE2_CLKp = not1(GYFO_STORE2_CLKn);

    /*p30.BUHE*/ BUHE_STORE2_I0 = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_I0.qp());
    /*p30.BYHU*/ BYHU_STORE2_I1 = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_I1.qp());
    /*p30.BECA*/ BECA_STORE2_I2 = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_I2.qp());
    /*p30.BULU*/ BULU_STORE2_I3 = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_I3.qp());
    /*p30.BUNA*/ BUNA_STORE2_I4 = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_I4.qp());
    /*p30.BOXA*/ BOXA_STORE2_I5 = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_I5.qp());

    /*p30.YLOV*/ YLOV_STORE2_L0 = dff8_B(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_L0.qp());
    /*p30.XOSY*/ XOSY_STORE2_L1 = dff8_B(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_L1.qp());
    /*p30.XAZY*/ XAZY_STORE2_L2 = dff8_B(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_L2.qp());
    /*p30.YKUK*/ YKUK_STORE2_L3 = dff8_B(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_L3.qp());

    /*p31.FOKA*/ FOKA_STORE2_X0 = dff9_A(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZAGO_OAM_DB0);
    /*p31.FYTY*/ FYTY_STORE2_X1 = dff9_A(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZOCY_OAM_DB1);
    /*p31.FUBY*/ FUBY_STORE2_X2 = dff9_A(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YPUR_OAM_DB2);
    /*p31.GOXU*/ GOXU_STORE2_X3 = dff9_A(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YVOK_OAM_DB3);
    /*p31.DUHY*/ DUHY_STORE2_X4 = dff9_A(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, COSE_OAM_DB4);
    /*p31.EJUF*/ EJUF_STORE2_X5 = dff9_A(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, AROP_OAM_DB5);
    /*p31.ENOR*/ ENOR_STORE2_X6 = dff9_A(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, XATU_OAM_DB6);
    /*p31.DEPY*/ DEPY_STORE2_X7 = dff9_A(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, BADY_OAM_DB7);

    /*p29.FEKA*/ wire FEKA_STORE3_CLKp = not1(GUSA_STORE3_CLKn);
    /*p29.XYHA*/ wire XYHA_STORE3_CLKp = not1(GUSA_STORE3_CLKn);
    /*p29.YFAG*/ wire YFAG_STORE3_CLKp = not1(GUSA_STORE3_CLKn);

    /*p30.DEVY*/ DEVY_STORE3_I0 = dff8_B(FEKA_STORE3_CLKp, !FEKA_STORE3_CLKp, SPR_TRI_I0.qp());
    /*p30.DESE*/ DESE_STORE3_I1 = dff8_B(FEKA_STORE3_CLKp, !FEKA_STORE3_CLKp, SPR_TRI_I1.qp());
    /*p30.DUNY*/ DUNY_STORE3_I2 = dff8_B(FEKA_STORE3_CLKp, !FEKA_STORE3_CLKp, SPR_TRI_I2.qp());
    /*p30.DUHA*/ DUHA_STORE3_I3 = dff8_B(FEKA_STORE3_CLKp, !FEKA_STORE3_CLKp, SPR_TRI_I3.qp());
    /*p30.DEBA*/ DEBA_STORE3_I4 = dff8_B(FEKA_STORE3_CLKp, !FEKA_STORE3_CLKp, SPR_TRI_I4.qp());
    /*p30.DAFU*/ DAFU_STORE3_I5 = dff8_B(FEKA_STORE3_CLKp, !FEKA_STORE3_CLKp, SPR_TRI_I5.qp());
    
    /*p30.ZURO*/ ZURO_STORE3_L0 = dff8_B(XYHA_STORE3_CLKp, !XYHA_STORE3_CLKp, SPR_TRI_L0.qp());
    /*p30.ZYLU*/ ZYLU_STORE3_L1 = dff8_B(XYHA_STORE3_CLKp, !XYHA_STORE3_CLKp, SPR_TRI_L1.qp());
    /*p30.ZENE*/ ZENE_STORE3_L2 = dff8_B(XYHA_STORE3_CLKp, !XYHA_STORE3_CLKp, SPR_TRI_L2.qp());
    /*p30.ZURY*/ ZURY_STORE3_L3 = dff8_B(XYHA_STORE3_CLKp, !XYHA_STORE3_CLKp, SPR_TRI_L3.qp());

    /*p31.XOLY*/ XOLY_STORE3_X0 = dff9_A(YFAG_STORE3_CLKp, !YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, ZAGO_OAM_DB0);
    /*p31.XYBA*/ XYBA_STORE3_X1 = dff9_A(YFAG_STORE3_CLKp, !YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, ZOCY_OAM_DB1);
    /*p31.XABE*/ XABE_STORE3_X2 = dff9_A(YFAG_STORE3_CLKp, !YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, YPUR_OAM_DB2);
    /*p31.XEKA*/ XEKA_STORE3_X3 = dff9_A(YFAG_STORE3_CLKp, !YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, YVOK_OAM_DB3);
    /*p31.XOMY*/ XOMY_STORE3_X4 = dff9_A(YFAG_STORE3_CLKp, !YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, COSE_OAM_DB4);
    /*p31.WUHA*/ WUHA_STORE3_X5 = dff9_A(YFAG_STORE3_CLKp, !YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, AROP_OAM_DB5);
    /*p31.WYNA*/ WYNA_STORE3_X6 = dff9_A(YFAG_STORE3_CLKp, !YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, XATU_OAM_DB6);
    /*p31.WECO*/ WECO_STORE3_X7 = dff9_A(YFAG_STORE3_CLKp, !YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, BADY_OAM_DB7);

    /*p29.WOFO*/ wire WOFO_STORE4_CLKp = not1(DUKE_STORE4_CLKn);
    /*p29.WYLU*/ wire WYLU_STORE4_CLKp = not1(DUKE_STORE4_CLKn);
    /*p29.EWOT*/ wire EWOT_STORE4_CLKp = not1(DUKE_STORE4_CLKn);

    /*p30.XAVE*/ XAVE_STORE4_I0 = dff8_B(WYLU_STORE4_CLKp, !WYLU_STORE4_CLKp, SPR_TRI_I0.qp());
    /*p30.XEFE*/ XEFE_STORE4_I1 = dff8_B(WYLU_STORE4_CLKp, !WYLU_STORE4_CLKp, SPR_TRI_I1.qp());
    /*p30.WANU*/ WANU_STORE4_I2 = dff8_B(WYLU_STORE4_CLKp, !WYLU_STORE4_CLKp, SPR_TRI_I2.qp());
    /*p30.XABO*/ XABO_STORE4_I3 = dff8_B(WYLU_STORE4_CLKp, !WYLU_STORE4_CLKp, SPR_TRI_I3.qp());
    /*p30.XEGE*/ XEGE_STORE4_I4 = dff8_B(WYLU_STORE4_CLKp, !WYLU_STORE4_CLKp, SPR_TRI_I4.qp());
    /*p30.XYNU*/ XYNU_STORE4_I5 = dff8_B(WYLU_STORE4_CLKp, !WYLU_STORE4_CLKp, SPR_TRI_I5.qp());

    /*p30.CAPO*/ CAPO_STORE4_L0 = dff8_B(EWOT_STORE4_CLKp, !EWOT_STORE4_CLKp, SPR_TRI_L0.qp());
    /*p30.CAJU*/ CAJU_STORE4_L1 = dff8_B(EWOT_STORE4_CLKp, !EWOT_STORE4_CLKp, SPR_TRI_L1.qp());
    /*p30.CONO*/ CONO_STORE4_L2 = dff8_B(EWOT_STORE4_CLKp, !EWOT_STORE4_CLKp, SPR_TRI_L2.qp());
    /*p30.CUMU*/ CUMU_STORE4_L3 = dff8_B(EWOT_STORE4_CLKp, !EWOT_STORE4_CLKp, SPR_TRI_L3.qp());

    /*p31.WEDU*/ WEDU_STORE4_X0 = dff9_A(WOFO_STORE4_CLKp, !WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, ZAGO_OAM_DB0);
    /*p31.YGAJ*/ YGAJ_STORE4_X1 = dff9_A(WOFO_STORE4_CLKp, !WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, ZOCY_OAM_DB1);
    /*p31.ZYJO*/ ZYJO_STORE4_X2 = dff9_A(WOFO_STORE4_CLKp, !WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, YPUR_OAM_DB2);
    /*p31.XURY*/ XURY_STORE4_X3 = dff9_A(WOFO_STORE4_CLKp, !WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, YVOK_OAM_DB3);
    /*p31.YBED*/ YBED_STORE4_X4 = dff9_A(WOFO_STORE4_CLKp, !WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, COSE_OAM_DB4);
    /*p31.ZALA*/ ZALA_STORE4_X5 = dff9_A(WOFO_STORE4_CLKp, !WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, AROP_OAM_DB5);
    /*p31.WYDE*/ WYDE_STORE4_X6 = dff9_A(WOFO_STORE4_CLKp, !WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, XATU_OAM_DB6);
    /*p31.XEPA*/ XEPA_STORE4_X7 = dff9_A(WOFO_STORE4_CLKp, !WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, BADY_OAM_DB7);

    /*p29.CYLA*/ wire CYLA_STORE5_CLKp = not1(BEDE_STORE5_CLKn);
    /*p29.DYMO*/ wire DYMO_STORE5_CLKp = not1(BEDE_STORE5_CLKn);
    /*p29.BUCY*/ wire BUCY_STORE5_CLKp = not1(BEDE_STORE5_CLKn);

    /*p30.EKOP*/ EKOP_STORE5_I0 = dff8_B(DYMO_STORE5_CLKp, !DYMO_STORE5_CLKp, SPR_TRI_I0.qp());
    /*p30.ETYM*/ ETYM_STORE5_I1 = dff8_B(DYMO_STORE5_CLKp, !DYMO_STORE5_CLKp, SPR_TRI_I1.qp());
    /*p30.GORU*/ GORU_STORE5_I2 = dff8_B(DYMO_STORE5_CLKp, !DYMO_STORE5_CLKp, SPR_TRI_I2.qp());
    /*p30.EBEX*/ EBEX_STORE5_I3 = dff8_B(DYMO_STORE5_CLKp, !DYMO_STORE5_CLKp, SPR_TRI_I3.qp());
    /*p30.ETAV*/ ETAV_STORE5_I4 = dff8_B(DYMO_STORE5_CLKp, !DYMO_STORE5_CLKp, SPR_TRI_I4.qp());
    /*p30.EKAP*/ EKAP_STORE5_I5 = dff8_B(DYMO_STORE5_CLKp, !DYMO_STORE5_CLKp, SPR_TRI_I5.qp());

    /*p30.ACEP*/ ACEP_STORE5_L0 = dff8_B(BUCY_STORE5_CLKp, !BUCY_STORE5_CLKp, SPR_TRI_L0.qp());
    /*p30.ABEG*/ ABEG_STORE5_L1 = dff8_B(BUCY_STORE5_CLKp, !BUCY_STORE5_CLKp, SPR_TRI_L1.qp());
    /*p30.ABUX*/ ABUX_STORE5_L2 = dff8_B(BUCY_STORE5_CLKp, !BUCY_STORE5_CLKp, SPR_TRI_L2.qp());
    /*p30.ANED*/ ANED_STORE5_L3 = dff8_B(BUCY_STORE5_CLKp, !BUCY_STORE5_CLKp, SPR_TRI_L3.qp());

    /*p31.FUSA*/ FUSA_STORE5_X0 = dff9_A(CYLA_STORE5_CLKp, !CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, ZAGO_OAM_DB0);
    /*p31.FAXA*/ FAXA_STORE5_X1 = dff9_A(CYLA_STORE5_CLKp, !CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, ZOCY_OAM_DB1);
    /*p31.FOZY*/ FOZY_STORE5_X2 = dff9_A(CYLA_STORE5_CLKp, !CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, YPUR_OAM_DB2);
    /*p31.FESY*/ FESY_STORE5_X3 = dff9_A(CYLA_STORE5_CLKp, !CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, YVOK_OAM_DB3);
    /*p31.CYWE*/ CYWE_STORE5_X4 = dff9_A(CYLA_STORE5_CLKp, !CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, COSE_OAM_DB4);
    /*p31.DYBY*/ DYBY_STORE5_X5 = dff9_A(CYLA_STORE5_CLKp, !CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, AROP_OAM_DB5);
    /*p31.DURY*/ DURY_STORE5_X6 = dff9_A(CYLA_STORE5_CLKp, !CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, XATU_OAM_DB6);
    /*p31.CUVY*/ CUVY_STORE5_X7 = dff9_A(CYLA_STORE5_CLKp, !CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, BADY_OAM_DB7);

    /*p29.ZAPE*/ wire ZAPE_STORE6_CLKp = not1(WEKA_STORE6_CLKn);
    /*p29.WUSE*/ wire WUSE_STORE6_CLKp = not1(WEKA_STORE6_CLKn);
    /*p29.ZURU*/ wire ZURU_STORE6_CLKp = not1(WEKA_STORE6_CLKn);

    /*p30.GABO*/ GABO_STORE6_I0 = dff8_B(WUSE_STORE6_CLKp, !WUSE_STORE6_CLKp, SPR_TRI_I0.qp());
    /*p30.GACY*/ GACY_STORE6_I1 = dff8_B(WUSE_STORE6_CLKp, !WUSE_STORE6_CLKp, SPR_TRI_I1.qp());
    /*p30.FOGO*/ FOGO_STORE6_I2 = dff8_B(WUSE_STORE6_CLKp, !WUSE_STORE6_CLKp, SPR_TRI_I2.qp());
    /*p30.GOHU*/ GOHU_STORE6_I3 = dff8_B(WUSE_STORE6_CLKp, !WUSE_STORE6_CLKp, SPR_TRI_I3.qp());
    /*p30.FOXY*/ FOXY_STORE6_I4 = dff8_B(WUSE_STORE6_CLKp, !WUSE_STORE6_CLKp, SPR_TRI_I4.qp());
    /*p30.GECU*/ GECU_STORE6_I5 = dff8_B(WUSE_STORE6_CLKp, !WUSE_STORE6_CLKp, SPR_TRI_I5.qp());

    /*p30.ZUMY*/ ZUMY_STORE6_L0 = dff8_B(ZURU_STORE6_CLKp, !ZURU_STORE6_CLKp, SPR_TRI_L0.qp());
    /*p30.ZAFU*/ ZAFU_STORE6_L1 = dff8_B(ZURU_STORE6_CLKp, !ZURU_STORE6_CLKp, SPR_TRI_L1.qp());
    /*p30.ZEXO*/ ZEXO_STORE6_L2 = dff8_B(ZURU_STORE6_CLKp, !ZURU_STORE6_CLKp, SPR_TRI_L2.qp());
    /*p30.ZUBE*/ ZUBE_STORE6_L3 = dff8_B(ZURU_STORE6_CLKp, !ZURU_STORE6_CLKp, SPR_TRI_L3.qp());

    /*p31.YCOL*/ YCOL_STORE6_X0 = dff9_A(ZAPE_STORE6_CLKp, !ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, ZAGO_OAM_DB0);
    /*p31.YRAC*/ YRAC_STORE6_X1 = dff9_A(ZAPE_STORE6_CLKp, !ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, ZOCY_OAM_DB1);
    /*p31.YMEM*/ YMEM_STORE6_X2 = dff9_A(ZAPE_STORE6_CLKp, !ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, YPUR_OAM_DB2);
    /*p31.YVAG*/ YVAG_STORE6_X3 = dff9_A(ZAPE_STORE6_CLKp, !ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, YVOK_OAM_DB3);
    /*p31.ZOLY*/ ZOLY_STORE6_X4 = dff9_A(ZAPE_STORE6_CLKp, !ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, COSE_OAM_DB4);
    /*p31.ZOGO*/ ZOGO_STORE6_X5 = dff9_A(ZAPE_STORE6_CLKp, !ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, AROP_OAM_DB5);
    /*p31.ZECU*/ ZECU_STORE6_X6 = dff9_A(ZAPE_STORE6_CLKp, !ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, XATU_OAM_DB6);
    /*p31.ZESA*/ ZESA_STORE6_X7 = dff9_A(ZAPE_STORE6_CLKp, !ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, BADY_OAM_DB7);

    /*p29.GECY*/ wire GECY_STORE7_CLKp = not1(GYVO_STORE7_CLKn);
    /*p29.FEFO*/ wire FEFO_STORE7_CLKp = not1(GYVO_STORE7_CLKn);
    /*p29.WABE*/ wire WABE_STORE7_CLKp = not1(GYVO_STORE7_CLKn);

    /*p30.GULE*/ GULE_STORE7_I0 = dff8_B(FEFO_STORE7_CLKp, !FEFO_STORE7_CLKp, SPR_TRI_I0.qp());
    /*p30.GYNO*/ GYNO_STORE7_I1 = dff8_B(FEFO_STORE7_CLKp, !FEFO_STORE7_CLKp, SPR_TRI_I1.qp());
    /*p30.FEFA*/ FEFA_STORE7_I2 = dff8_B(FEFO_STORE7_CLKp, !FEFO_STORE7_CLKp, SPR_TRI_I2.qp());
    /*p30.FYSU*/ FYSU_STORE7_I3 = dff8_B(FEFO_STORE7_CLKp, !FEFO_STORE7_CLKp, SPR_TRI_I3.qp());
    /*p30.GESY*/ GESY_STORE7_I4 = dff8_B(FEFO_STORE7_CLKp, !FEFO_STORE7_CLKp, SPR_TRI_I4.qp());
    /*p30.FUZO*/ FUZO_STORE7_I5 = dff8_B(FEFO_STORE7_CLKp, !FEFO_STORE7_CLKp, SPR_TRI_I5.qp());

    /*p30.XYNA*/ XYNA_STORE7_L0 = dff8_B(WABE_STORE7_CLKp, !WABE_STORE7_CLKp, SPR_TRI_L0.qp());
    /*p30.YGUM*/ YGUM_STORE7_L1 = dff8_B(WABE_STORE7_CLKp, !WABE_STORE7_CLKp, SPR_TRI_L1.qp());
    /*p30.XAKU*/ XAKU_STORE7_L2 = dff8_B(WABE_STORE7_CLKp, !WABE_STORE7_CLKp, SPR_TRI_L2.qp());
    /*p30.XYGO*/ XYGO_STORE7_L3 = dff8_B(WABE_STORE7_CLKp, !WABE_STORE7_CLKp, SPR_TRI_L3.qp());

    /*p31.ERAZ*/ ERAZ_STORE7_X0 = dff9_A(GECY_STORE7_CLKp, !GECY_STORE7_CLKp, GAFY_STORE7_RSTn, ZAGO_OAM_DB0);
    /*p31.EPUM*/ EPUM_STORE7_X1 = dff9_A(GECY_STORE7_CLKp, !GECY_STORE7_CLKp, GAFY_STORE7_RSTn, ZOCY_OAM_DB1);
    /*p31.EROL*/ EROL_STORE7_X2 = dff9_A(GECY_STORE7_CLKp, !GECY_STORE7_CLKp, GAFY_STORE7_RSTn, YPUR_OAM_DB2);
    /*p31.EHYN*/ EHYN_STORE7_X3 = dff9_A(GECY_STORE7_CLKp, !GECY_STORE7_CLKp, GAFY_STORE7_RSTn, YVOK_OAM_DB3);
    /*p31.FAZU*/ FAZU_STORE7_X4 = dff9_A(GECY_STORE7_CLKp, !GECY_STORE7_CLKp, GAFY_STORE7_RSTn, COSE_OAM_DB4);
    /*p31.FAXE*/ FAXE_STORE7_X5 = dff9_A(GECY_STORE7_CLKp, !GECY_STORE7_CLKp, GAFY_STORE7_RSTn, AROP_OAM_DB5);
    /*p31.EXUK*/ EXUK_STORE7_X6 = dff9_A(GECY_STORE7_CLKp, !GECY_STORE7_CLKp, GAFY_STORE7_RSTn, XATU_OAM_DB6);
    /*p31.FEDE*/ FEDE_STORE7_X7 = dff9_A(GECY_STORE7_CLKp, !GECY_STORE7_CLKp, GAFY_STORE7_RSTn, BADY_OAM_DB7);

    /*p29.CEXU*/ wire CEXU_STORE8_CLKp = not1(BUKA_STORE8_CLKn);
    /*p29.AKOL*/ wire AKOL_STORE8_CLKp = not1(BUKA_STORE8_CLKn);
    /*p29.BYMY*/ wire BYMY_STORE8_CLKp = not1(BUKA_STORE8_CLKn);

    /*p30.AXUV*/ AXUV_STORE8_I0 = dff8_B(AKOL_STORE8_CLKp, !AKOL_STORE8_CLKp, SPR_TRI_I0.qp());
    /*p30.BADA*/ BADA_STORE8_I1 = dff8_B(AKOL_STORE8_CLKp, !AKOL_STORE8_CLKp, SPR_TRI_I1.qp());
    /*p30.APEV*/ APEV_STORE8_I2 = dff8_B(AKOL_STORE8_CLKp, !AKOL_STORE8_CLKp, SPR_TRI_I2.qp());
    /*p30.BADO*/ BADO_STORE8_I3 = dff8_B(AKOL_STORE8_CLKp, !AKOL_STORE8_CLKp, SPR_TRI_I3.qp());
    /*p30.BEXY*/ BEXY_STORE8_I4 = dff8_B(AKOL_STORE8_CLKp, !AKOL_STORE8_CLKp, SPR_TRI_I4.qp());
    /*p30.BYHE*/ BYHE_STORE8_I5 = dff8_B(AKOL_STORE8_CLKp, !AKOL_STORE8_CLKp, SPR_TRI_I5.qp());

    /*p30.AZAP*/ AZAP_STORE8_L0 = dff8_B(BYMY_STORE8_CLKp, !BYMY_STORE8_CLKp, SPR_TRI_L0.qp());
    /*p30.AFYX*/ AFYX_STORE8_L1 = dff8_B(BYMY_STORE8_CLKp, !BYMY_STORE8_CLKp, SPR_TRI_L1.qp());
    /*p30.AFUT*/ AFUT_STORE8_L2 = dff8_B(BYMY_STORE8_CLKp, !BYMY_STORE8_CLKp, SPR_TRI_L2.qp());
    /*p30.AFYM*/ AFYM_STORE8_L3 = dff8_B(BYMY_STORE8_CLKp, !BYMY_STORE8_CLKp, SPR_TRI_L3.qp());

    /*p31.GAVY*/ GAVY_STORE8_X4 = dff9_A(CEXU_STORE8_CLKp, !CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, ZAGO_OAM_DB0);
    /*p31.GYPU*/ GYPU_STORE8_X5 = dff9_A(CEXU_STORE8_CLKp, !CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, ZOCY_OAM_DB1);
    /*p31.GADY*/ GADY_STORE8_X6 = dff9_A(CEXU_STORE8_CLKp, !CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, YPUR_OAM_DB2);
    /*p31.GAZA*/ GAZA_STORE8_X7 = dff9_A(CEXU_STORE8_CLKp, !CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, YVOK_OAM_DB3);
    /*p31.EZUF*/ EZUF_STORE8_X0 = dff9_A(CEXU_STORE8_CLKp, !CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, COSE_OAM_DB4);
    /*p31.ENAD*/ ENAD_STORE8_X1 = dff9_A(CEXU_STORE8_CLKp, !CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, AROP_OAM_DB5);
    /*p31.EBOW*/ EBOW_STORE8_X2 = dff9_A(CEXU_STORE8_CLKp, !CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, XATU_OAM_DB6);
    /*p31.FYCA*/ FYCA_STORE8_X3 = dff9_A(CEXU_STORE8_CLKp, !CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, BADY_OAM_DB7);

    /*p29.WEME*/ wire WEME_STORE9_CLKp = not1(DECU_STORE9_CLKn);
    /*p29.WUFA*/ wire WUFA_STORE9_CLKp = not1(DECU_STORE9_CLKn);
    /*p29.FAKA*/ wire FAKA_STORE9_CLKp = not1(DECU_STORE9_CLKn);

    /*p30.YBER*/ YBER_STORE9_I0 = dff8_B(WUFA_STORE9_CLKp, !WUFA_STORE9_CLKp, SPR_TRI_I0.qp());
    /*p30.YZOR*/ YZOR_STORE9_I1 = dff8_B(WUFA_STORE9_CLKp, !WUFA_STORE9_CLKp, SPR_TRI_I1.qp());
    /*p30.XYFE*/ XYFE_STORE9_I2 = dff8_B(WUFA_STORE9_CLKp, !WUFA_STORE9_CLKp, SPR_TRI_I2.qp());
    /*p30.XOTU*/ XOTU_STORE9_I3 = dff8_B(WUFA_STORE9_CLKp, !WUFA_STORE9_CLKp, SPR_TRI_I3.qp());
    /*p30.XUTE*/ XUTE_STORE9_I4 = dff8_B(WUFA_STORE9_CLKp, !WUFA_STORE9_CLKp, SPR_TRI_I4.qp());
    /*p30.XUFO*/ XUFO_STORE9_I5 = dff8_B(WUFA_STORE9_CLKp, !WUFA_STORE9_CLKp, SPR_TRI_I5.qp());

    /*p30.CANA*/ CANA_STORE9_L0 = dff8_B(FAKA_STORE9_CLKp, !FAKA_STORE9_CLKp, SPR_TRI_L0.qp());
    /*p30.FOFO*/ FOFO_STORE9_L1 = dff8_B(FAKA_STORE9_CLKp, !FAKA_STORE9_CLKp, SPR_TRI_L1.qp());
    /*p30.DYSY*/ DYSY_STORE9_L2 = dff8_B(FAKA_STORE9_CLKp, !FAKA_STORE9_CLKp, SPR_TRI_L2.qp());
    /*p30.DEWU*/ DEWU_STORE9_L3 = dff8_B(FAKA_STORE9_CLKp, !FAKA_STORE9_CLKp, SPR_TRI_L3.qp());

    /*p31.XUVY*/ XUVY_STORE9_X0 = dff9_A(WEME_STORE9_CLKp, !WEME_STORE9_CLKp, DOSY_STORE9_RSTn, ZAGO_OAM_DB0);
    /*p31.XERE*/ XERE_STORE9_X1 = dff9_A(WEME_STORE9_CLKp, !WEME_STORE9_CLKp, DOSY_STORE9_RSTn, ZOCY_OAM_DB1);
    /*p31.XUZO*/ XUZO_STORE9_X2 = dff9_A(WEME_STORE9_CLKp, !WEME_STORE9_CLKp, DOSY_STORE9_RSTn, YPUR_OAM_DB2);
    /*p31.XEXA*/ XEXA_STORE9_X3 = dff9_A(WEME_STORE9_CLKp, !WEME_STORE9_CLKp, DOSY_STORE9_RSTn, YVOK_OAM_DB3);
    /*p31.YPOD*/ YPOD_STORE9_X4 = dff9_A(WEME_STORE9_CLKp, !WEME_STORE9_CLKp, DOSY_STORE9_RSTn, COSE_OAM_DB4);
    /*p31.YROP*/ YROP_STORE9_X5 = dff9_A(WEME_STORE9_CLKp, !WEME_STORE9_CLKp, DOSY_STORE9_RSTn, AROP_OAM_DB5);
    /*p31.YNEP*/ YNEP_STORE9_X6 = dff9_A(WEME_STORE9_CLKp, !WEME_STORE9_CLKp, DOSY_STORE9_RSTn, XATU_OAM_DB6);
    /*p31.YZOF*/ YZOF_STORE9_X7 = dff9_A(WEME_STORE9_CLKp, !WEME_STORE9_CLKp, DOSY_STORE9_RSTn, BADY_OAM_DB7);
  }
}

//------------------------------------------------------------------------------
