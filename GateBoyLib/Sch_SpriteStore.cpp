#include "GateBoyLib/Sch_SpriteStore.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void SpriteStore::dump(Dumper& d, const SchematicTop& top) const {
  d("\002===== SpriteStore =====\001\n");
  d("DEZY_STORE_ENn %c\n", DEZY_STORE_ENn.c());
  d("FEPO_STORE_MATCHp %d\n", wire(FEPO_STORE_MATCHp));

  d("STORE MATCH %c%c%c%c%c%c%c%c%c%c\n",
    YDUG_STORE0_MATCHn.c(), DYDU_STORE1_MATCHn.c(), DEGO_STORE2_MATCHn.c(), YLOZ_STORE3_MATCHn.c(), XAGE_STORE4_MATCHn.c(),
    EGOM_STORE5_MATCHn.c(), YBEZ_STORE6_MATCHn.c(), DYKA_STORE7_MATCHn.c(), EFYL_STORE8_MATCHn.c(), YGEM_STORE9_MATCHn.c());

  d("SPRITE COUNT %02d\n", pack_p(BESE_SPRITE_COUNT0.qp17(), CUXY_SPRITE_COUNT1.qp17(), BEGO_SPRITE_COUNT2.qp17(), DYBE_SPRITE_COUNT3.qp17()));

  int spr_tri_idx = pack_p(SPR_TRI_I0p.qp(), top.sprite_store.SPR_TRI_I1p.qp(), top.sprite_store.SPR_TRI_I2p.qp(), top.sprite_store.SPR_TRI_I3p.qp(), top.sprite_store.SPR_TRI_I4p.qp(), top.sprite_store.SPR_TRI_I5p.qp(), 0, 0);
  d("SPR_TRI_IDX  = %2d %c%c%c%c%c%c\n",
    spr_tri_idx,
    SPR_TRI_I5p.c(), top.sprite_store.SPR_TRI_I4p.c(), top.sprite_store.SPR_TRI_I3p.c(), top.sprite_store.SPR_TRI_I2p.c(),
    SPR_TRI_I1p.c(), top.sprite_store.SPR_TRI_I0p.c());

  int spr_tri_line = pack_p(SPR_TRI_L0.qp(), top.sprite_store.SPR_TRI_L1.qp(), top.sprite_store.SPR_TRI_L2.qp(), top.sprite_store.SPR_TRI_L3.qp());
  d("SPR_TRI_LINE = %2d %c%c%c%c\n",
    spr_tri_line,
    SPR_TRI_L0.c(), top.sprite_store.SPR_TRI_L1.c(), top.sprite_store.SPR_TRI_L2.c(), top.sprite_store.SPR_TRI_L3.c());

  d("STORE0 M%d R%d I%02d L%02d X%03d\n",
    !YDUG_STORE0_MATCHn,
    EBOJ_STORE0_RSTp.qp17(),
    pack_n(YGUS_STORE0_I0n.qp08(), YSOK_STORE0_I1n.qp08(), YZEP_STORE0_I2n.qp08(), WYTE_STORE0_I3n.qp08(), ZONY_STORE0_I4n.qp08(), YWAK_STORE0_I5n.qp08(), 1, 1),
    pack_n(GYHO_STORE0_L0n.qp08(), CUFO_STORE0_L1n.qp08(), BOZU_STORE0_L2n.qp08(), FYHY_STORE0_L3n.qp08()),
    pack_n(XEPE_STORE0_X0p.qn08(), YLAH_STORE0_X1p.qn08(), ZOLA_STORE0_X2p.qn08(), ZULU_STORE0_X3p.qn08(), WELO_STORE0_X4p.qn08(), XUNY_STORE0_X5p.qn08(), WOTE_STORE0_X6p.qn08(), XAKO_STORE0_X7p.qn08())
  );

  d("STORE1 M%d R%d I%02d L%02d X%03d\n",
    !DYDU_STORE1_MATCHn,
    CEDY_STORE1_RSTp.qp17(),
    pack_n(CADU_STORE1_I0n.qp08(), CEBO_STORE1_I1n.qp08(), CUFA_STORE1_I2n.qp08(), COMA_STORE1_I3n.qp08(), CUZA_STORE1_I4n.qp08(), CAJY_STORE1_I5n.qp08(), 1, 1),
    pack_n(AMES_STORE1_L0n.qp08(), AROF_STORE1_L1n.qp08(), ABOP_STORE1_L2n.qp08(), ABUG_STORE1_L3n.qp08()),
    pack_n(DANY_STORE1_X0p.qn08(), DUKO_STORE1_X1p.qn08(), DESU_STORE1_X2p.qn08(), DAZO_STORE1_X3p.qn08(), DAKE_STORE1_X4p.qn08(), CESO_STORE1_X5p.qn08(), DYFU_STORE1_X6p.qn08(), CUSY_STORE1_X7p.qn08())
  );

  d("STORE2 M%d R%d I%02d L%02d X%03d\n",
    !DEGO_STORE2_MATCHn,
    EGAV_STORE2_RSTp.qp17(),
    pack_n(BUHE_STORE2_I0n.qp08(), BYHU_STORE2_I1n.qp08(), BECA_STORE2_I2n.qp08(), BULU_STORE2_I3n.qp08(), BUNA_STORE2_I4n.qp08(), BOXA_STORE2_I5n.qp08(), 1, 1),
    pack_n(YLOV_STORE2_L0n.qp08(), XOSY_STORE2_L1n.qp08(), XAZY_STORE2_L2n.qp08(), YKUK_STORE2_L3n.qp08()),
    pack_n(FOKA_STORE2_X0p.qn08(), FYTY_STORE2_X1p.qn08(), FUBY_STORE2_X2p.qn08(), GOXU_STORE2_X3p.qn08(), DUHY_STORE2_X4p.qn08(), EJUF_STORE2_X5p.qn08(), ENOR_STORE2_X6p.qn08(), DEPY_STORE2_X7p.qn08())
  );

  d("STORE3 M%d R%d I%02d L%02d X%03d\n",
    !YLOZ_STORE3_MATCHn,
    GOTA_STORE3_RSTp.qp17(),
    pack_n(DEVY_STORE3_I0n.qp08(), DESE_STORE3_I1n.qp08(), DUNY_STORE3_I2n.qp08(), DUHA_STORE3_I3n.qp08(), DEBA_STORE3_I4n.qp08(), DAFU_STORE3_I5n.qp08(), 1, 1),
    pack_n(ZURO_STORE3_L0n.qp08(), ZYLU_STORE3_L1n.qp08(), ZENE_STORE3_L2n.qp08(), ZURY_STORE3_L3n.qp08()),
    pack_n(XOLY_STORE3_X0p.qn08(), XYBA_STORE3_X1p.qn08(), XABE_STORE3_X2p.qn08(), XEKA_STORE3_X3p.qn08(), XOMY_STORE3_X4p.qn08(), WUHA_STORE3_X5p.qn08(), WYNA_STORE3_X6p.qn08(), WECO_STORE3_X7p.qn08())
  );

  d("STORE4 M%d R%d I%02d L%02d X%03d\n",
    !XAGE_STORE4_MATCHn,
    XUDY_STORE4_RSTp.qp17(),
    pack_n(XAVE_STORE4_I0n.qp08(), XEFE_STORE4_I1n.qp08(), WANU_STORE4_I2n.qp08(), XABO_STORE4_I3n.qp08(), XEGE_STORE4_I4n.qp08(), XYNU_STORE4_I5n.qp08(), 1, 1),
    pack_n(CAPO_STORE4_L0n.qp08(), CAJU_STORE4_L1n.qp08(), CONO_STORE4_L2n.qp08(), CUMU_STORE4_L3n.qp08()),
    pack_n(WEDU_STORE4_X0p.qn08(), YGAJ_STORE4_X1p.qn08(), ZYJO_STORE4_X2p.qn08(), XURY_STORE4_X3p.qn08(), YBED_STORE4_X4p.qn08(), ZALA_STORE4_X5p.qn08(), WYDE_STORE4_X6p.qn08(), XEPA_STORE4_X7p.qn08())
  );

  d("STORE5 M%d R%d I%02d L%02d X%03d\n",
    !EGOM_STORE5_MATCHn,
    WAFY_STORE5_RSTp.qp17(),
    pack_n(EKOP_STORE5_I0n.qp08(), ETYM_STORE5_I1n.qp08(), GORU_STORE5_I2n.qp08(), EBEX_STORE5_I3n.qp08(), ETAV_STORE5_I4n.qp08(), EKAP_STORE5_I5n.qp08(), 1, 1),
    pack_n(ACEP_STORE5_L0n.qp08(), ABEG_STORE5_L1n.qp08(), ABUX_STORE5_L2n.qp08(), ANED_STORE5_L3n.qp08()),
    pack_n(FUSA_STORE5_X0p.qn08(), FAXA_STORE5_X1p.qn08(), FOZY_STORE5_X2p.qn08(), FESY_STORE5_X3p.qn08(), CYWE_STORE5_X4p.qn08(), DYBY_STORE5_X5p.qn08(), DURY_STORE5_X6p.qn08(), CUVY_STORE5_X7p.qn08())
  );

  d("STORE6 M%d R%d I%02d L%02d X%03d\n",
    !YBEZ_STORE6_MATCHn,
    WOMY_STORE6_RSTp.qp17(),
    pack_n(GABO_STORE6_I0n.qp08(), GACY_STORE6_I1n.qp08(), FOGO_STORE6_I2n.qp08(), GOHU_STORE6_I3n.qp08(), FOXY_STORE6_I4n.qp08(), GECU_STORE6_I5n.qp08(), 1, 1),
    pack_n(ZUMY_STORE6_L0n.qp08(), ZAFU_STORE6_L1n.qp08(), ZEXO_STORE6_L2n.qp08(), ZUBE_STORE6_L3n.qp08()),
    pack_n(YCOL_STORE6_X0p.qn08(), YRAC_STORE6_X1p.qn08(), YMEM_STORE6_X2p.qn08(), YVAG_STORE6_X3p.qn08(), ZOLY_STORE6_X4p.qn08(), ZOGO_STORE6_X5p.qn08(), ZECU_STORE6_X6p.qn08(), ZESA_STORE6_X7p.qn08())
  );

  d("STORE7 M%d R%d I%02d L%02d X%03d\n",
    !DYKA_STORE7_MATCHn,
    WAPO_STORE7_RSTp.qp17(),
    pack_n(GULE_STORE7_I0n.qp08(), GYNO_STORE7_I1n.qp08(), FEFA_STORE7_I2n.qp08(), FYSU_STORE7_I3n.qp08(), GESY_STORE7_I4n.qp08(), FUZO_STORE7_I5n.qp08(), 1, 1),
    pack_n(XYNA_STORE7_L0n.qp08(), YGUM_STORE7_L1n.qp08(), XAKU_STORE7_L2n.qp08(), XYGO_STORE7_L3n.qp08()),
    pack_n(ERAZ_STORE7_X0p.qn08(), EPUM_STORE7_X1p.qn08(), EROL_STORE7_X2p.qn08(), EHYN_STORE7_X3p.qn08(), FAZU_STORE7_X4p.qn08(), FAXE_STORE7_X5p.qn08(), EXUK_STORE7_X6p.qn08(), FEDE_STORE7_X7p.qn08())
  );

  d("STORE8 M%d R%d I%02d L%02d X%03d\n",
    !EFYL_STORE8_MATCHn,
    EXUQ_STORE8_RSTp.qp17(),
    pack_n(AXUV_STORE8_I0n.qp08(), BADA_STORE8_I1n.qp08(), APEV_STORE8_I2n.qp08(), BADO_STORE8_I3n.qp08(), BEXY_STORE8_I4n.qp08(), BYHE_STORE8_I5n.qp08(), 1, 1),
    pack_n(AZAP_STORE8_L0n.qp08(), AFYX_STORE8_L1n.qp08(), AFUT_STORE8_L2n.qp08(), AFYM_STORE8_L3n.qp08()),
    pack_n(EZUF_STORE8_X0p.qn08(), ENAD_STORE8_X1p.qn08(), EBOW_STORE8_X2p.qn08(), FYCA_STORE8_X3p.qn08(), GAVY_STORE8_X4p.qn08(), GYPU_STORE8_X5p.qn08(), GADY_STORE8_X6p.qn08(), GAZA_STORE8_X7p.qn08())
  );

  d("STORE9 M%d R%d I%02d L%02d X%03d\n",
    !YGEM_STORE9_MATCHn,
    FONO_STORE9_RSTp.qp17(),
    pack_n(YBER_STORE9_I0n.qp08(), YZOR_STORE9_I1n.qp08(), XYFE_STORE9_I2n.qp08(), XOTU_STORE9_I3n.qp08(), XUTE_STORE9_I4n.qp08(), XUFO_STORE9_I5n.qp08(), 1, 1),
    pack_n(CANA_STORE9_L0n.qp08(), FOFO_STORE9_L1n.qp08(), DYSY_STORE9_L2n.qp08(), DEWU_STORE9_L3n.qp08()),
    pack_n(XUVY_STORE9_X0p.qn08(), XERE_STORE9_X1p.qn08(), XUZO_STORE9_X2p.qn08(), XEXA_STORE9_X3p.qn08(), YPOD_STORE9_X4p.qn08(), YROP_STORE9_X5p.qn08(), YNEP_STORE9_X6p.qn08(), YZOF_STORE9_X7p.qn08())
  );

  d("\n");
}

