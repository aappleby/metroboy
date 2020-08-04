#include "Sch_SpriteStore.h"

#include "Sch_Top.h"

using namespace Schematics;

void SpriteStore::dump(Dumper& d) const {
  d("----------SpriteStore---------\n");
  d("DEZY_STORE_ENn %c\n", DEZY_STORE_ENn.c());
  d("FEPO_STORE_MATCHp %d\n", wire(FEPO_STORE_MATCHp));

  d("SPRITE INDEX %02d\n", 
    pack(
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

  d("SPRITE COUNT %02d\n", pack(SPRITE_COUNT0.qp(), SPRITE_COUNT1.qp(), SPRITE_COUNT2.qp(), SPRITE_COUNT3.qp()));

  int spr_tri_idx = pack(SPR_TRI_IDX0n.qn(), SPR_TRI_IDX1n.qn(), SPR_TRI_IDX2n.qn(), SPR_TRI_IDX3n.qn(),
                         SPR_TRI_IDX4n.qn(), SPR_TRI_IDX5n.qn(), 0, 0);
  d("SPR_TRI_IDX  = %2d %c%c%c%c%c%c\n",
    spr_tri_idx,
    SPR_TRI_IDX5n.cn(), SPR_TRI_IDX4n.cn(), SPR_TRI_IDX3n.cn(), SPR_TRI_IDX2n.cn(),
    SPR_TRI_IDX1n.cn(), SPR_TRI_IDX0n.cn());

  int spr_tri_line = pack(SPR_TRI_LINE_0.qp(), SPR_TRI_LINE_1.qp(), SPR_TRI_LINE_2.qp(), SPR_TRI_LINE_3.qp()); 
  d("SPR_TRI_LINE = %2d %c%c%c%c\n",
    spr_tri_line,
    SPR_TRI_LINE_0.c(), SPR_TRI_LINE_1.c(), SPR_TRI_LINE_2.c(), SPR_TRI_LINE_3.c());

  d("STORE0 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE0_MATCHn,
    STORE0_RSTp.qp(),
    pack(STORE0_IDX0n.qp(), STORE0_IDX1n.qp(), STORE0_IDX2n.qp(), STORE0_IDX3n.qp(), STORE0_IDX4n.qp(), STORE0_IDX5n.qp(), 0, 0),
    pack(STORE0_LINE0.qp(), STORE0_LINE1.qp(), STORE0_LINE2.qp(), STORE0_LINE3.qp()),
    pack(STORE0_X0.qp(),    STORE0_X1.qp(),    STORE0_X2.qp(),    STORE0_X3.qp(),    STORE0_X4.qp(),    STORE0_X5.qp(),    STORE0_X6.qp(),    STORE0_X7.qp())
  );

  d("STORE1 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE1_MATCHn,
    STORE1_RSTp.qp(),
    pack(STORE1_IDX0n.qp(), STORE1_IDX1n.qp(), STORE1_IDX2n.qp(), STORE1_IDX3n.qp(), STORE1_IDX4n.qp(), STORE1_IDX5n.qp(), 0, 0),
    pack(STORE1_LINE0.qp(), STORE1_LINE1.qp(), STORE1_LINE2.qp(), STORE1_LINE3.qp()),
    pack(STORE1_X0.qp(),    STORE1_X1.qp(),    STORE1_X2.qp(),    STORE1_X3.qp(),    STORE1_X4.qp(),    STORE1_X5.qp(),    STORE1_X6.qp(),    STORE1_X7.qp())
  );

  d("STORE2 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE2_MATCHn,
    STORE2_RSTp.qp(),
    pack(STORE2_IDX0n.qp(), STORE2_IDX1n.qp(), STORE2_IDX2n.qp(), STORE2_IDX3n.qp(), STORE2_IDX4n.qp(), STORE2_IDX5n.qp(), 0, 0),
    pack(STORE2_LINE0.qp(), STORE2_LINE1.qp(), STORE2_LINE2.qp(), STORE2_LINE3.qp()),
    pack(STORE2_X0.qp(),    STORE2_X1.qp(),    STORE2_X2.qp(),    STORE2_X3.qp(),    STORE2_X4.qp(),    STORE2_X5.qp(),    STORE2_X6.qp(),    STORE2_X7.qp())
  );

  d("STORE3 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE3_MATCHn,
    STORE3_RSTp.qp(),
    pack(STORE3_INDX5.qp(), STORE3_INDX4.qp(), STORE3_INDX3.qp(), STORE3_INDX2.qp(), STORE3_INDX1.qp(), STORE3_INDX0.qp(), 0, 0),
    pack(STORE3_LINE0.qp(), STORE3_LINE1.qp(), STORE3_LINE2.qp(), STORE3_LINE3.qp()),
    pack(STORE3_X0.qp(),    STORE3_X1.qp(),    STORE3_X2.qp(),    STORE3_X3.qp(),    STORE3_X4.qp(),    STORE3_X5.qp(),    STORE3_X6.qp(),    STORE3_X7.qp())
  );

  d("STORE4 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE4_MATCHn,
    STORE4_RSTp.qp(),
    pack(STORE4_INDX0.qp(), STORE4_INDX1.qp(), STORE4_INDX2.qp(), STORE4_INDX3.qp(), STORE4_INDX4.qp(), STORE4_INDX5.qp(), 0, 0),
    pack(STORE4_LINE0.qp(), STORE4_LINE1.qp(), STORE4_LINE2.qp(), STORE4_LINE3.qp()),
    pack(STORE4_X0.qp(),    STORE4_X1.qp(),    STORE4_X2.qp(),    STORE4_X3.qp(),    STORE4_X4.qp(),    STORE4_X5.qp(),    STORE4_X6.qp(),    STORE4_X7.qp())
  );

  d("STORE5 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE5_MATCHn,
    STORE5_RSTp.qp(),
    pack(STORE5_INDX0.qp(), STORE5_INDX1.qp(), STORE5_INDX2.qp(), STORE5_INDX3.qp(), STORE5_INDX4.qp(), STORE5_INDX5.qp(), 0, 0),
    pack(STORE5_LINE0.qp(), STORE5_LINE1.qp(), STORE5_LINE2.qp(), STORE5_LINE3.qp()),
    pack(STORE5_X0.qp(),    STORE5_X1.qp(),    STORE5_X2.qp(),    STORE5_X3.qp(),    STORE5_X4.qp(),    STORE5_X5.qp(),    STORE5_X6.qp(),    STORE5_X7.qp())
  );

  d("STORE6 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE6_MATCHn,
    STORE6_RSTp.qp(),
    pack(STORE6_INDX0.qp(), STORE6_INDX1.qp(), STORE6_INDX2.qp(), STORE6_INDX3.qp(), STORE6_INDX4.qp(), STORE6_INDX5.qp(), 0, 0),
    pack(STORE6_LINE0.qp(), STORE6_LINE1.qp(), STORE6_LINE2.qp(), STORE6_LINE3.qp()),
    pack(STORE6_X0.qp(),    STORE6_X1.qp(),    STORE6_X2.qp(),    STORE6_X3.qp(),    STORE6_X4.qp(),    STORE6_X5.qp(),    STORE6_X6.qp(),    STORE6_X7.qp())
  );

  d("STORE7 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE7_MATCHn,
    STORE7_RSTp.qp(),
    pack(STORE7_INDX0.qp(), STORE7_INDX1.qp(), STORE7_INDX2.qp(), STORE7_INDX3.qp(), STORE7_INDX4.qp(), STORE7_INDX5.qp(), 0, 0),
    pack(STORE7_LINE0.qp(), STORE7_LINE1.qp(), STORE7_LINE2.qp(), STORE7_LINE3.qp()),
    pack(STORE7_X0.qp(),    STORE7_X1.qp(),    STORE7_X2.qp(),    STORE7_X3.qp(),    STORE7_X4.qp(),    STORE7_X5.qp(),    STORE7_X6.qp(),    STORE7_X7.qp())
  );

  d("STORE8 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE8_MATCHn,
    STORE8_RSTp.qp(),
    pack(STORE8_INDX0.qp(), STORE8_INDX1.qp(), STORE8_INDX2.qp(), STORE8_INDX3.qp(), STORE8_INDX4.qp(), STORE8_INDX5.qp(), 0, 0),
    pack(STORE8_LINE0.qp(), STORE8_LINE1.qp(), STORE8_LINE2.qp(), STORE8_LINE3.qp()),
    pack(STORE8_X0.qp(),    STORE8_X1.qp(),    STORE8_X2.qp(),    STORE8_X3.qp(),    STORE8_X4.qp(),    STORE8_X5.qp(),    STORE8_X6.qp(),    STORE8_X7.qp())
  );

  d("STORE9 M%d R%d I%02d L%02d X%03d\n",
    (wire)STORE9_MATCHn,
    STORE9_RSTp.qp(),
    pack(STORE9_INDX0.qp(), STORE9_INDX1.qp(), STORE9_INDX2.qp(), STORE9_INDX3.qp(), STORE9_INDX4.qp(), STORE9_INDX5.qp(), 0, 0),
    pack(STORE9_LINE0.qp(), STORE9_LINE1.qp(), STORE9_LINE2.qp(), STORE9_LINE3.qp()),
    pack(STORE9_X0.qp(),    STORE9_X1.qp(),    STORE9_X2.qp(),    STORE9_X3.qp(),    STORE9_X4.qp(),    STORE9_X5.qp(),    STORE9_X6.qp(),    STORE9_X7.qp())
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

    /*p21.ACAM*/ wire _ACAM_X0n = not1(top.pix_pipe.XEHO_X0.qp());
    /*p21.AZUB*/ wire _AZUB_X1n = not1(top.pix_pipe.SAVY_X1.qp());
    /*p21.AMEL*/ wire _AMEL_X2n = not1(top.pix_pipe.XODU_X2.qp());
    /*p21.AHAL*/ wire _AHAL_X3n = not1(top.pix_pipe.XYDO_X3.qp());
    /*p21.APUX*/ wire _APUX_X4n = not1(top.pix_pipe.TUHU_X4.qp());
    /*p21.ABEF*/ wire _ABEF_X5n = not1(top.pix_pipe.TUKY_X5.qp());
    /*p21.ADAZ*/ wire _ADAZ_X6n = not1(top.pix_pipe.TAKO_X6.qp());
    /*p21.ASAH*/ wire _ASAH_X7n = not1(top.pix_pipe.SYBE_X7.qp());

    /*p31.ZOGY*/ wire STORE0_MATCH0n = xor2(STORE0_X0.qn(), _ACAM_X0n);
    /*p31.ZEBA*/ wire STORE0_MATCH1n = xor2(STORE0_X1.qn(), _AZUB_X1n);
    /*p31.ZAHA*/ wire STORE0_MATCH2n = xor2(STORE0_X2.qn(), _AMEL_X2n);
    /*p31.ZOKY*/ wire STORE0_MATCH3n = xor2(STORE0_X3.qn(), _AHAL_X3n);
    /*p31.WOJU*/ wire STORE0_MATCH4n = xor2(STORE0_X4.qn(), _APUX_X4n);
    /*p31.YFUN*/ wire STORE0_MATCH5n = xor2(STORE0_X5.qn(), _ABEF_X5n);
    /*p31.WYZA*/ wire STORE0_MATCH6n = xor2(STORE0_X6.qn(), _ADAZ_X6n);
    /*p31.YPUK*/ wire STORE0_MATCH7n = xor2(STORE0_X7.qn(), _ASAH_X7n);

    /*p31.EDYM*/ wire STORE1_MATCH0n = xor2(STORE1_X0.qn(), _ACAM_X0n);
    /*p31.EMYB*/ wire STORE1_MATCH1n = xor2(STORE1_X1.qn(), _AZUB_X1n);
    /*p31.EBEF*/ wire STORE1_MATCH2n = xor2(STORE1_X2.qn(), _AMEL_X2n);
    /*p31.EWOK*/ wire STORE1_MATCH3n = xor2(STORE1_X3.qn(), _AHAL_X3n);
    /*p31.COLA*/ wire STORE1_MATCH4n = xor2(STORE1_X4.qn(), _APUX_X4n);
    /*p31.BOBA*/ wire STORE1_MATCH5n = xor2(STORE1_X5.qn(), _ABEF_X5n);
    /*p31.COLU*/ wire STORE1_MATCH6n = xor2(STORE1_X6.qn(), _ADAZ_X6n);
    /*p31.BAHU*/ wire STORE1_MATCH7n = xor2(STORE1_X7.qn(), _ASAH_X7n);

    /*p31.FUZU*/ wire STORE2_MATCH0n = xor2(STORE2_X0.qn(), _ACAM_X0n);
    /*p31.FESO*/ wire STORE2_MATCH1n = xor2(STORE2_X1.qn(), _AZUB_X1n);
    /*p31.FOKY*/ wire STORE2_MATCH2n = xor2(STORE2_X2.qn(), _AMEL_X2n);
    /*p31.FYVA*/ wire STORE2_MATCH3n = xor2(STORE2_X3.qn(), _AHAL_X3n);
    /*p31.CEKO*/ wire STORE2_MATCH4n = xor2(STORE2_X4.qn(), _APUX_X4n);
    /*p31.DETY*/ wire STORE2_MATCH5n = xor2(STORE2_X5.qn(), _ABEF_X5n);
    /*p31.DOZO*/ wire STORE2_MATCH6n = xor2(STORE2_X6.qn(), _ADAZ_X6n);
    /*p31.CONY*/ wire STORE2_MATCH7n = xor2(STORE2_X7.qn(), _ASAH_X7n);

    /*p31.YHOK*/ wire STORE3_MATCH0n = xor2(STORE3_X0.qn(), _ACAM_X0n);
    /*p31.YCAH*/ wire STORE3_MATCH1n = xor2(STORE3_X1.qn(), _AZUB_X1n);
    /*p31.YDAJ*/ wire STORE3_MATCH2n = xor2(STORE3_X2.qn(), _AMEL_X2n);
    /*p31.YVUZ*/ wire STORE3_MATCH3n = xor2(STORE3_X3.qn(), _AHAL_X3n);
    /*p31.YVAP*/ wire STORE3_MATCH4n = xor2(STORE3_X4.qn(), _APUX_X4n);
    /*p31.XENY*/ wire STORE3_MATCH5n = xor2(STORE3_X5.qn(), _ABEF_X5n);
    /*p31.XAVU*/ wire STORE3_MATCH6n = xor2(STORE3_X6.qn(), _ADAZ_X6n);
    /*p31.XEVA*/ wire STORE3_MATCH7n = xor2(STORE3_X7.qn(), _ASAH_X7n);

    /*p31.XEJU*/ wire STORE4_MATCH0n = xor2(STORE4_X0.qn(), _ACAM_X0n);
    /*p31.ZATE*/ wire STORE4_MATCH1n = xor2(STORE4_X1.qn(), _AZUB_X1n);
    /*p31.ZAKU*/ wire STORE4_MATCH2n = xor2(STORE4_X2.qn(), _AMEL_X2n);
    /*p31.YBOX*/ wire STORE4_MATCH3n = xor2(STORE4_X3.qn(), _AHAL_X3n);
    /*p31.ZYKU*/ wire STORE4_MATCH4n = xor2(STORE4_X4.qn(), _APUX_X4n);
    /*p31.ZYPU*/ wire STORE4_MATCH5n = xor2(STORE4_X5.qn(), _ABEF_X5n);
    /*p31.XAHA*/ wire STORE4_MATCH6n = xor2(STORE4_X6.qn(), _ADAZ_X6n);
    /*p31.ZEFE*/ wire STORE4_MATCH7n = xor2(STORE4_X7.qn(), _ASAH_X7n);

    /*p31.GUZO*/ wire STORE5_MATCH0n = xor2(STORE5_X0.qn(), _ACAM_X0n);
    /*p31.GOLA*/ wire STORE5_MATCH1n = xor2(STORE5_X1.qn(), _AZUB_X1n);
    /*p31.GEVE*/ wire STORE5_MATCH2n = xor2(STORE5_X2.qn(), _AMEL_X2n);
    /*p31.GUDE*/ wire STORE5_MATCH3n = xor2(STORE5_X3.qn(), _AHAL_X3n);
    /*p31.BAZY*/ wire STORE5_MATCH4n = xor2(STORE5_X4.qn(), _APUX_X4n);
    /*p31.CYLE*/ wire STORE5_MATCH5n = xor2(STORE5_X5.qn(), _ABEF_X5n);
    /*p31.CEVA*/ wire STORE5_MATCH6n = xor2(STORE5_X6.qn(), _ADAZ_X6n);
    /*p31.BUMY*/ wire STORE5_MATCH7n = xor2(STORE5_X7.qn(), _ASAH_X7n);

    /*p31.XOSU*/ wire STORE6_MATCH0n = xor2(STORE6_X0.qn(), _ACAM_X0n);
    /*p31.ZUVU*/ wire STORE6_MATCH1n = xor2(STORE6_X1.qn(), _AZUB_X1n);
    /*p31.XUCO*/ wire STORE6_MATCH2n = xor2(STORE6_X2.qn(), _AMEL_X2n);
    /*p31.ZULO*/ wire STORE6_MATCH3n = xor2(STORE6_X3.qn(), _AHAL_X3n);
    /*p31.ZARE*/ wire STORE6_MATCH4n = xor2(STORE6_X4.qn(), _APUX_X4n);
    /*p31.ZEMU*/ wire STORE6_MATCH5n = xor2(STORE6_X5.qn(), _ABEF_X5n);
    /*p31.ZYGO*/ wire STORE6_MATCH6n = xor2(STORE6_X6.qn(), _ADAZ_X6n);
    /*p31.ZUZY*/ wire STORE6_MATCH7n = xor2(STORE6_X7.qn(), _ASAH_X7n);

    /*p31.DUSE*/ wire STORE7_MATCH0n = xor2(STORE7_X0.qn(), _ACAM_X0n);
    /*p31.DAGU*/ wire STORE7_MATCH1n = xor2(STORE7_X1.qn(), _AZUB_X1n);
    /*p31.DYZE*/ wire STORE7_MATCH2n = xor2(STORE7_X2.qn(), _AMEL_X2n);
    /*p31.DESO*/ wire STORE7_MATCH3n = xor2(STORE7_X3.qn(), _AHAL_X3n);
    /*p31.EJOT*/ wire STORE7_MATCH4n = xor2(STORE7_X4.qn(), _APUX_X4n);
    /*p31.ESAJ*/ wire STORE7_MATCH5n = xor2(STORE7_X5.qn(), _ABEF_X5n);
    /*p31.DUCU*/ wire STORE7_MATCH6n = xor2(STORE7_X6.qn(), _ADAZ_X6n);
    /*p31.EWUD*/ wire STORE7_MATCH7n = xor2(STORE7_X7.qn(), _ASAH_X7n);

    /*p31.DUZE*/ wire STORE8_MATCH0n = xor2(STORE8_X0.qn(), _APUX_X4n);
    /*p31.DAGA*/ wire STORE8_MATCH1n = xor2(STORE8_X1.qn(), _ABEF_X5n);
    /*p31.DAWU*/ wire STORE8_MATCH2n = xor2(STORE8_X2.qn(), _ADAZ_X6n);
    /*p31.EJAW*/ wire STORE8_MATCH3n = xor2(STORE8_X3.qn(), _ASAH_X7n);
    /*p31.GOHO*/ wire STORE8_MATCH4n = xor2(STORE8_X4.qn(), _ACAM_X0n);
    /*p31.GASU*/ wire STORE8_MATCH5n = xor2(STORE8_X5.qn(), _AZUB_X1n);
    /*p31.GABU*/ wire STORE8_MATCH6n = xor2(STORE8_X6.qn(), _AMEL_X2n);
    /*p31.GAFE*/ wire STORE8_MATCH7n = xor2(STORE8_X7.qn(), _AHAL_X3n);

    /*p31.YMAM*/ wire STORE9_MATCH0n = xor2(STORE9_X0.qn(), _ACAM_X0n);
    /*p31.YTYP*/ wire STORE9_MATCH1n = xor2(STORE9_X1.qn(), _AZUB_X1n);
    /*p31.YFOP*/ wire STORE9_MATCH2n = xor2(STORE9_X2.qn(), _AMEL_X2n);
    /*p31.YVAC*/ wire STORE9_MATCH3n = xor2(STORE9_X3.qn(), _AHAL_X3n);
    /*p31.ZYWU*/ wire STORE9_MATCH4n = xor2(STORE9_X4.qn(), _APUX_X4n);
    /*p31.ZUZA*/ wire STORE9_MATCH5n = xor2(STORE9_X5.qn(), _ABEF_X5n);
    /*p31.ZEJO*/ wire STORE9_MATCH6n = xor2(STORE9_X6.qn(), _ADAZ_X6n);
    /*p31.ZEDA*/ wire STORE9_MATCH7n = xor2(STORE9_X7.qn(), _ASAH_X7n);

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

    /*p29.YDUG*/ STORE0_MATCHn = nand3(AROR_MATCH_ENp, STORE0_MATCHB, STORE0_MATCHA);
    /*p29.DYDU*/ STORE1_MATCHn = nand3(AROR_MATCH_ENp, STORE1_MATCHA, STORE1_MATCHB);
    /*p29.DEGO*/ STORE2_MATCHn = nand3(AROR_MATCH_ENp, STORE2_MATCHB, STORE2_MATCHA);
    /*p29.YLOZ*/ STORE3_MATCHn = nand3(AROR_MATCH_ENp, STORE3_MATCHA, STORE3_MATCHB);
    /*p29.XAGE*/ STORE4_MATCHn = nand3(AROR_MATCH_ENp, STORE4_MATCHB, STORE4_MATCHA);
    /*p29.EGOM*/ STORE5_MATCHn = nand3(AROR_MATCH_ENp, STORE5_MATCHB, STORE5_MATCHA);
    /*p29.YBEZ*/ STORE6_MATCHn = nand3(AROR_MATCH_ENp, STORE6_MATCHB, STORE6_MATCHA);
    /*p29.DYKA*/ STORE7_MATCHn = nand3(AROR_MATCH_ENp, STORE7_MATCHA, STORE7_MATCHB);
    /*p29.EFYL*/ STORE8_MATCHn = nand3(AROR_MATCH_ENp, STORE8_MATCHB, STORE8_MATCHA);
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
    /*p28.AZYB*/ wire _AZYB_RST = not1(top.lcd_reg.ATEJ_VID_LINE_TRIGp());
    
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

    /*p30.WENU*/ SPR_TRI_LINE_0 = tribuf_6n(FEPO_STORE_MATCHp, top.sprite_scanner.DEGE_SPRITE_DELTA0());
    /*p30.CUCU*/ SPR_TRI_LINE_1 = tribuf_6n(FEPO_STORE_MATCHp, top.sprite_scanner.DABY_SPRITE_DELTA1());
    /*p30.CUCA*/ SPR_TRI_LINE_2 = tribuf_6n(FEPO_STORE_MATCHp, top.sprite_scanner.DABU_SPRITE_DELTA2());
    /*p30.CEGA*/ SPR_TRI_LINE_3 = tribuf_6n(FEPO_STORE_MATCHp, top.sprite_scanner.GYSA_SPRITE_DELTA3());

    /*p29.DEZY*/ DEZY_STORE_ENn = dff17_B(top.clk_reg.ZEME_AxCxExGx(), top.clk_reg.XAPO_VID_RSTn(), top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH());
  }

  //----------------------------------------
  // Sprite store input driver.

  {
    /*p30.CYKE*/ wire _CYKE_AxxDExxH = not1(top.clk_reg.XUPY_xxCDxxGH());
    /*p30.WUDA*/ wire _WUDA_xBCxxFGx = not1(_CYKE_AxxDExxH);

    // XADU := dff(WUDA, CYKE, WEFE, YFOT);
    // WUZY := tri(BUZA, XADU)

    /*p30.XADU*/ XADU_SPRITE_IDX0n = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YFOT_OAM_A2n());
    /*p30.XEDY*/ XEDY_SPRITE_IDX1n = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YFOC_OAM_A3n());
    /*p30.ZUZE*/ ZUZE_SPRITE_IDX2n = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YVOM_OAM_A4n());
    /*p30.XOBE*/ XOBE_SPRITE_IDX3n = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YMEV_OAM_A5n());
    /*p30.YDUF*/ YDUF_SPRITE_IDX4n = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.XEMU_OAM_A6n());
    /*p30.XECU*/ XECU_SPRITE_IDX5n = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YZET_OAM_A7n());


    /*p30.WUZY*/ SPR_TRI_IDX0n = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XADU_SPRITE_IDX0n.qp());
    /*p30.WYSE*/ SPR_TRI_IDX1n = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XEDY_SPRITE_IDX1n.qp());
    /*p30.ZYSU*/ SPR_TRI_IDX2n = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), ZUZE_SPRITE_IDX2n.qp());
    /*p30.WYDA*/ SPR_TRI_IDX3n = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XOBE_SPRITE_IDX3n.qp());
    /*p30.WUCO*/ SPR_TRI_IDX4n = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), YDUF_SPRITE_IDX4n.qp());
    /*p30.WEZA*/ SPR_TRI_IDX5n = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XECU_SPRITE_IDX5n.qp());
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

    /*p28.ABAK*/ wire _ABAK_VID_LINE_TRIGp = or2(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.AMYG_VID_RSTp());
    /*p28.BYVA*/ wire BYVA_VID_LINE_TRIGn = not1(_ABAK_VID_LINE_TRIGp);

    /*p29.EBOJ*/ STORE0_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GUVA_SPRITE0_GETp);
    /*p29.CEDY*/ STORE1_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _ENUT_SPRITE1_GETp);
    /*p29.EGAV*/ STORE2_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _EMOL_SPRITE2_GETp);
    /*p29.GOTA*/ STORE3_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GYFY_SPRITE3_GETp);
    /*p29.XUDY*/ STORE4_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GONO_SPRITE4_GETp);
    /*p29.WAFY*/ STORE5_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GEGA_SPRITE5_GETp);
    /*p29.WOMY*/ STORE6_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _XOJA_SPRITE6_GETp);
    /*p29.WAPO*/ STORE7_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GUTU_SPRITE7_GETp);
    /*p29.EXUQ*/ STORE8_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _FOXA_SPRITE8_GETp);
    /*p29.FONO*/ STORE9_RSTp = dff17_B(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GUZE_SPRITE9_GETp);

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
      /*p29.EBOJ*/ STORE0_RSTp = dff17(WUTY_SPRITE_DONEp(), 0, _GUVA_SPRITE0_GETp);
      /*p31.XEPE*/ STORE0_X0   = dff9(FUXU_STORE0_CLKp,     0, ZAGO_OAM_DB0);
    }


    /*p28.ABAK*/ wire ABAK_VID_LINE_TRIGp = or2(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.AMYG_VID_RSTp());
    /*p28.BYVA*/ wire BYVA_VID_LINE_TRIGn = not1(ABAK_VID_LINE_TRIGp);
    /*p29.DYBA*/ wire DYBA_VID_LINE_TRIGp = not1(BYVA_VID_LINE_TRIGn);
    /*p29.EBOJ*/ STORE0_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GUVA_SPRITE0_GETp);
    /*p29.DYWE*/ wire DYWE_STORE0_RSTp = or2(DYBA_VID_LINE_TRIGp, STORE0_RSTp.qp());
    /*p29.DYNA*/ wire DYNA_STORE0_RSTn = not1(DYWE_STORE0_RSTp);
    /*p31.XEPE*/ STORE0_X0    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZAGO_OAM_DB0);
#endif

    // ZEDY := tri(FURO, YWAK)
    // CUBO := tri(DYDO, CADU)

    /*p30.ZEDY*/ SPR_TRI_IDX5n = tribuf_6n(FURO_SPRITE0_GETn, STORE0_IDX5n.qp());
    /*p30.ZUMU*/ SPR_TRI_IDX4n = tribuf_6n(FURO_SPRITE0_GETn, STORE0_IDX4n.qp());
    /*p30.WOKO*/ SPR_TRI_IDX3n = tribuf_6n(FURO_SPRITE0_GETn, STORE0_IDX3n.qp());
    /*p30.ZAVE*/ SPR_TRI_IDX2n = tribuf_6n(FURO_SPRITE0_GETn, STORE0_IDX2n.qp());
    /*p30.ZECE*/ SPR_TRI_IDX1n = tribuf_6n(FURO_SPRITE0_GETn, STORE0_IDX1n.qp());
    /*p30.ZETU*/ SPR_TRI_IDX0n = tribuf_6n(FURO_SPRITE0_GETn, STORE0_IDX0n.qp());
    /*p30.GOFO*/ SPR_TRI_LINE_0 = tribuf_6n(FURO_SPRITE0_GETn, STORE0_LINE0.qp());
    /*p30.WEHE*/ SPR_TRI_LINE_1 = tribuf_6n(FURO_SPRITE0_GETn, STORE0_LINE1.qp());
    /*p30.AJAL*/ SPR_TRI_LINE_2 = tribuf_6n(FURO_SPRITE0_GETn, STORE0_LINE2.qp());
    /*p30.BUKY*/ SPR_TRI_LINE_3 = tribuf_6n(FURO_SPRITE0_GETn, STORE0_LINE3.qp());

    /*p30.BEMO*/ SPR_TRI_IDX5n = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_IDX5n.qp());
    /*p30.CYBY*/ SPR_TRI_IDX4n = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_IDX4n.qp());
    /*p30.BETY*/ SPR_TRI_IDX3n = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_IDX3n.qp());
    /*p30.CEGY*/ SPR_TRI_IDX2n = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_IDX2n.qp());
    /*p30.CELU*/ SPR_TRI_IDX1n = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_IDX1n.qp());
    /*p30.CUBO*/ SPR_TRI_IDX0n = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_IDX0n.qp());
    /*p30.BEFE*/ SPR_TRI_LINE_0 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_LINE0.qp());
    /*p30.BYRO*/ SPR_TRI_LINE_1 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_LINE1.qp());
    /*p30.BACO*/ SPR_TRI_LINE_2 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_LINE2.qp());
    /*p30.AHUM*/ SPR_TRI_LINE_3 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_LINE3.qp());

    /*p30.AXEC*/ SPR_TRI_IDX5n = tribuf_6n(FAME_SPRITE2_GETn, STORE2_IDX5n.qp());
    /*p30.CYRO*/ SPR_TRI_IDX4n = tribuf_6n(FAME_SPRITE2_GETn, STORE2_IDX4n.qp());
    /*p30.CUVU*/ SPR_TRI_IDX3n = tribuf_6n(FAME_SPRITE2_GETn, STORE2_IDX3n.qp());
    /*p30.APON*/ SPR_TRI_IDX2n = tribuf_6n(FAME_SPRITE2_GETn, STORE2_IDX2n.qp());
    /*p30.AFOZ*/ SPR_TRI_IDX1n = tribuf_6n(FAME_SPRITE2_GETn, STORE2_IDX1n.qp());
    /*p30.CUBE*/ SPR_TRI_IDX0n = tribuf_6n(FAME_SPRITE2_GETn, STORE2_IDX0n.qp());
    /*p30.ZABY*/ SPR_TRI_LINE_0 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_LINE0.qp());
    /*p30.ZUKE*/ SPR_TRI_LINE_1 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_LINE1.qp());
    /*p30.WUXE*/ SPR_TRI_LINE_2 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_LINE2.qp());
    /*p30.WERE*/ SPR_TRI_LINE_3 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_LINE3.qp());

    /*p30.DEZU*/ SPR_TRI_IDX5n = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX0.qp());
    /*p30.EFUD*/ SPR_TRI_IDX4n = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX1.qp());
    /*p30.DONY*/ SPR_TRI_IDX3n = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX2.qp());
    /*p30.DOWA*/ SPR_TRI_IDX2n = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX3.qp());
    /*p30.DYGO*/ SPR_TRI_IDX1n = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX4.qp());
    /*p30.ENAP*/ SPR_TRI_IDX0n = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX5.qp());
    /*p30.ZYPO*/ SPR_TRI_LINE_0 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_LINE0.qp());
    /*p30.ZEXE*/ SPR_TRI_LINE_1 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_LINE1.qp());
    /*p30.YJEM*/ SPR_TRI_LINE_2 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_LINE2.qp());
    /*p30.YWAV*/ SPR_TRI_LINE_3 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_LINE3.qp());

    /*p30.WAJA*/ SPR_TRI_IDX5n = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_INDX0.qp());
    /*p30.WOXY*/ SPR_TRI_IDX4n = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_INDX1.qp());
    /*p30.XYRE*/ SPR_TRI_IDX3n = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_INDX2.qp());
    /*p30.WERU*/ SPR_TRI_IDX2n = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_INDX3.qp());
    /*p30.WEPY*/ SPR_TRI_IDX1n = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_INDX4.qp());
    /*p30.WUXU*/ SPR_TRI_IDX0n = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_INDX5.qp());
    /*p30.BYDO*/ SPR_TRI_LINE_0 = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_LINE0.qp());
    /*p30.BUCE*/ SPR_TRI_LINE_1 = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_LINE1.qp());
    /*p30.BOVE*/ SPR_TRI_LINE_2 = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_LINE2.qp());
    /*p30.BEVY*/ SPR_TRI_LINE_3 = tribuf_6n(GOWO_SPRITE4_GETn, STORE4_LINE3.qp());

    /*p30.DALO*/ SPR_TRI_IDX5n = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_INDX0.qp());
    /*p30.DALY*/ SPR_TRI_IDX4n = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_INDX1.qp());
    /*p30.DUZA*/ SPR_TRI_IDX3n = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_INDX2.qp());
    /*p30.WAGA*/ SPR_TRI_IDX2n = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_INDX3.qp());
    /*p30.DYNY*/ SPR_TRI_IDX1n = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_INDX4.qp());
    /*p30.DOBO*/ SPR_TRI_IDX0n = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_INDX5.qp());
    /*p30.AWAT*/ SPR_TRI_LINE_0 = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_LINE0.qp());
    /*p30.BACE*/ SPR_TRI_LINE_1 = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_LINE1.qp());
    /*p30.BODU*/ SPR_TRI_LINE_2 = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_LINE2.qp());
    /*p30.BUJA*/ SPR_TRI_LINE_3 = tribuf_6n(GYGY_SPRITE5_GETn, STORE5_LINE3.qp());

    /*p30.WOCY*/ SPR_TRI_IDX5n = tribuf_6n(XYME_SPRITE6_GETn, STORE6_INDX0.qp());
    /*p30.ELYC*/ SPR_TRI_IDX4n = tribuf_6n(XYME_SPRITE6_GETn, STORE6_INDX1.qp());
    /*p30.WABO*/ SPR_TRI_IDX3n = tribuf_6n(XYME_SPRITE6_GETn, STORE6_INDX2.qp());
    /*p30.EZOC*/ SPR_TRI_IDX2n = tribuf_6n(XYME_SPRITE6_GETn, STORE6_INDX3.qp());
    /*p30.WYWY*/ SPR_TRI_IDX1n = tribuf_6n(XYME_SPRITE6_GETn, STORE6_INDX4.qp());
    /*p30.WATO*/ SPR_TRI_IDX0n = tribuf_6n(XYME_SPRITE6_GETn, STORE6_INDX5.qp());
    /*p30.ZUDO*/ SPR_TRI_LINE_0 = tribuf_6n(XYME_SPRITE6_GETn, STORE6_LINE0.qp());
    /*p30.YBUK*/ SPR_TRI_LINE_1 = tribuf_6n(XYME_SPRITE6_GETn, STORE6_LINE1.qp());
    /*p30.ZYTO*/ SPR_TRI_LINE_2 = tribuf_6n(XYME_SPRITE6_GETn, STORE6_LINE2.qp());
    /*p30.YKOZ*/ SPR_TRI_LINE_3 = tribuf_6n(XYME_SPRITE6_GETn, STORE6_LINE3.qp());

    /*p30.EVYT*/ SPR_TRI_IDX5n = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_INDX0.qp());
    /*p30.WABA*/ SPR_TRI_IDX4n = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_INDX1.qp());
    /*p30.ETAD*/ SPR_TRI_IDX3n = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_INDX2.qp());
    /*p30.ELEP*/ SPR_TRI_IDX2n = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_INDX3.qp());
    /*p30.WYGO*/ SPR_TRI_IDX1n = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_INDX4.qp());
    /*p30.WAKO*/ SPR_TRI_IDX0n = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_INDX5.qp());
    /*p30.WANA*/ SPR_TRI_LINE_0 = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_LINE0.qp());
    /*p30.WAXE*/ SPR_TRI_LINE_1 = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_LINE1.qp());
    /*p30.WABU*/ SPR_TRI_LINE_2 = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_LINE2.qp());
    /*p30.YPOZ*/ SPR_TRI_LINE_3 = tribuf_6n(GUGY_SPRITE7_GETn, STORE7_LINE3.qp());

    /*p30.ADYB*/ SPR_TRI_IDX5n = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX0.qp());
    /*p30.APOB*/ SPR_TRI_IDX4n = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX1.qp());
    /*p30.APYV*/ SPR_TRI_IDX3n = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX2.qp());
    /*p30.AFEN*/ SPR_TRI_IDX2n = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX3.qp());
    /*p30.AKYH*/ SPR_TRI_IDX1n = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX4.qp());
    /*p30.APOC*/ SPR_TRI_IDX0n = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX5.qp());
    /*p30.BUJY*/ SPR_TRI_LINE_0 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_LINE0.qp());
    /*p30.BOSO*/ SPR_TRI_LINE_1 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_LINE1.qp());
    /*p30.AHAC*/ SPR_TRI_LINE_2 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_LINE2.qp());
    /*p30.BAZU*/ SPR_TRI_LINE_3 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_LINE3.qp());

    /*p30.YHAL*/ SPR_TRI_IDX5n = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX0.qp());
    /*p30.YRAD*/ SPR_TRI_IDX4n = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX1.qp());
    /*p30.XYRA*/ SPR_TRI_IDX3n = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX2.qp());
    /*p30.YNEV*/ SPR_TRI_IDX2n = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX3.qp());
    /*p30.ZOJY*/ SPR_TRI_IDX1n = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX4.qp());
    /*p30.ZARO*/ SPR_TRI_IDX0n = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX5.qp());
    /*p30.CAWO*/ SPR_TRI_LINE_0 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_LINE0.qp());
    /*p30.BYME*/ SPR_TRI_LINE_1 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_LINE1.qp());
    /*p30.COHO*/ SPR_TRI_LINE_2 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_LINE2.qp());
    /*p30.GATE*/ SPR_TRI_LINE_3 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_LINE3.qp());
  }

  //----------------------------------------
  // Sprite store setter

  {
    // ABAK := or(ATEJ, AMYG)
    // BYVA := not(ABAK)
    // DYBA := not(BYVA)

    /*p28.ABAK*/ wire _ABAK_VID_LINE_TRIGp = or2(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.AMYG_VID_RSTp());
    /*p28.BYVA*/ wire _BYVA_VID_LINE_TRIGn = not1(_ABAK_VID_LINE_TRIGp);
    /*p29.DYBA*/ wire _DYBA_VID_LINE_TRIGp = not1(_BYVA_VID_LINE_TRIGn);

    /*p29.DYWE*/ wire DYWE_STORE0_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE0_RSTp.qp());
    /*p29.EFEV*/ wire EFEV_STORE1_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE1_RSTp.qp());
    /*p29.FOKO*/ wire FOKO_STORE2_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE2_RSTp.qp());
    /*p29.GAKE*/ wire GAKE_STORE3_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE3_RSTp.qp());
    /*p29.WOHU*/ wire WOHU_STORE4_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE4_RSTp.qp());
    /*p29.FEVE*/ wire FEVE_STORE5_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE5_RSTp.qp());
    /*p29.WACY*/ wire WACY_STORE6_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE6_RSTp.qp());
    /*p29.GUKY*/ wire GUKY_STORE7_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE7_RSTp.qp());
    /*p29.GORO*/ wire GORO_STORE8_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE8_RSTp.qp());
    /*p29.DUBU*/ wire DUBU_STORE9_RSTp = or2(_DYBA_VID_LINE_TRIGp, STORE9_RSTp.qp());

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

    // ZAGO := not(YLOR_08)

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
    /*p30.YWAK*/ STORE0_IDX5n = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_IDX5n.qp());
    /*p30.ZONY*/ STORE0_IDX4n = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_IDX4n.qp());
    /*p30.WYTE*/ STORE0_IDX3n = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_IDX3n.qp());
    /*p30.YZEP*/ STORE0_IDX2n = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_IDX2n.qp());
    /*p30.YSOK*/ STORE0_IDX1n = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_IDX1n.qp());
    /*p30.YGUS*/ STORE0_IDX0n = dff8_B(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_IDX0n.qp());
    /*p30.FYHY*/ STORE0_LINE0 = dff8_B(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_LINE_0.qp());
    /*p30.GYHO*/ STORE0_LINE1 = dff8_B(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_LINE_1.qp());
    /*p30.BOZU*/ STORE0_LINE2 = dff8_B(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_LINE_2.qp());
    /*p30.CUFO*/ STORE0_LINE3 = dff8_B(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_LINE_3.qp());

    // XEPE_01 nc
    // XEPE_02 << FUXU_02  (clk)
    // XEPE_03 << ZAGO_02  (d)
    // XEPE_04 << COMP_CLK (clkn)
    // XEPE_05 nc
    // XEPE_06 << DYNA02   (rst)
    // XEPE_07 nc
    // XEPE_08 >> ZOGY_02  (qn)
    // XEPE_09 >> nc

    /*p31.XEPE*/ STORE0_X0    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZAGO_OAM_DB0);
    /*p31.YLAH*/ STORE0_X1    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZOCY_OAM_DB1);
    /*p31.ZOLA*/ STORE0_X2    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YPUR_OAM_DB2);
    /*p31.ZULU*/ STORE0_X3    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YVOK_OAM_DB3);
    /*p31.WELO*/ STORE0_X4    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, COSE_OAM_DB4);
    /*p31.XUNY*/ STORE0_X5    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, AROP_OAM_DB5);
    /*p31.WOTE*/ STORE0_X6    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, XATU_OAM_DB6);
    /*p31.XAKO*/ STORE0_X7    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, BADY_OAM_DB7);

    /*p29.BYVY*/ wire BYVU_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
    /*p29.AHOF*/ wire AHOF_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
    /*p29.ASYS*/ wire ASYS_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
    /*p30.CAJY*/ STORE1_IDX5n = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_IDX5n.qp());
    /*p30.CUZA*/ STORE1_IDX4n = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_IDX4n.qp());
    /*p30.COMA*/ STORE1_IDX3n = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_IDX3n.qp());
    /*p30.CUFA*/ STORE1_IDX2n = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_IDX2n.qp());
    /*p30.CEBO*/ STORE1_IDX1n = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_IDX1n.qp());
    /*p30.CADU*/ STORE1_IDX0n = dff8_B(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_IDX0n.qp());
    /*p30.ABUG*/ STORE1_LINE0 = dff8_B(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_LINE_0.qp());
    /*p30.AMES*/ STORE1_LINE1 = dff8_B(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_LINE_1.qp());
    /*p30.ABOP*/ STORE1_LINE2 = dff8_B(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_LINE_2.qp());
    /*p30.AROF*/ STORE1_LINE3 = dff8_B(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_LINE_3.qp());
    /*p31.DANY*/ STORE1_X0    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZAGO_OAM_DB0);
    /*p31.DUKO*/ STORE1_X1    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZOCY_OAM_DB1);
    /*p31.DESU*/ STORE1_X2    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YPUR_OAM_DB2);
    /*p31.DAZO*/ STORE1_X3    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YVOK_OAM_DB3);
    /*p31.DAKE*/ STORE1_X4    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, COSE_OAM_DB4);
    /*p31.CESO*/ STORE1_X5    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, AROP_OAM_DB5);
    /*p31.DYFU*/ STORE1_X6    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, XATU_OAM_DB6);
    /*p31.CUSY*/ STORE1_X7    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, BADY_OAM_DB7);

    /*p29.BUZY*/ wire BUZY_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
    /*p29.FUKE*/ wire FUKE_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
    /*p29.CACU*/ wire CACU_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
    /*p30.BOXA*/ STORE2_IDX5n = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_IDX5n.qp());
    /*p30.BUNA*/ STORE2_IDX4n = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_IDX4n.qp());
    /*p30.BULU*/ STORE2_IDX3n = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_IDX3n.qp());
    /*p30.BECA*/ STORE2_IDX2n = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_IDX2n.qp());
    /*p30.BYHU*/ STORE2_IDX1n = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_IDX1n.qp());
    /*p30.BUHE*/ STORE2_IDX0n = dff8_B(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_IDX0n.qp());
    /*p30.YKUK*/ STORE2_LINE0 = dff8_B(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_LINE_0.qp());
    /*p30.YLOV*/ STORE2_LINE1 = dff8_B(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_LINE_1.qp());
    /*p30.XAZY*/ STORE2_LINE2 = dff8_B(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_LINE_2.qp());
    /*p30.XOSY*/ STORE2_LINE3 = dff8_B(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_LINE_3.qp());
    /*p31.FOKA*/ STORE2_X0    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZAGO_OAM_DB0);
    /*p31.FYTY*/ STORE2_X1    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZOCY_OAM_DB1);
    /*p31.FUBY*/ STORE2_X2    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YPUR_OAM_DB2);
    /*p31.GOXU*/ STORE2_X3    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YVOK_OAM_DB3);
    /*p31.DUHY*/ STORE2_X4    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, COSE_OAM_DB4);
    /*p31.EJUF*/ STORE2_X5    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, AROP_OAM_DB5);
    /*p31.ENOR*/ STORE2_X6    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, XATU_OAM_DB6);
    /*p31.DEPY*/ STORE2_X7    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, BADY_OAM_DB7);

    /*p29.FEKA*/ wire FEKA = not1(GUSA_STORE3_CLKn);
    /*p29.XYHA*/ wire XYHA = not1(GUSA_STORE3_CLKn);
    /*p29.YFAG*/ wire YFAG = not1(GUSA_STORE3_CLKn);
    /*p30.DAFU*/ STORE3_INDX0 = dff8_B(FEKA, !FEKA, SPR_TRI_IDX5n.qp());
    /*p30.DEBA*/ STORE3_INDX1 = dff8_B(FEKA, !FEKA, SPR_TRI_IDX4n.qp());
    /*p30.DUHA*/ STORE3_INDX2 = dff8_B(FEKA, !FEKA, SPR_TRI_IDX3n.qp());
    /*p30.DUNY*/ STORE3_INDX3 = dff8_B(FEKA, !FEKA, SPR_TRI_IDX2n.qp());
    /*p30.DESE*/ STORE3_INDX4 = dff8_B(FEKA, !FEKA, SPR_TRI_IDX1n.qp());
    /*p30.DEVY*/ STORE3_INDX5 = dff8_B(FEKA, !FEKA, SPR_TRI_IDX0n.qp());
    /*p30.ZURY*/ STORE3_LINE0 = dff8_B(XYHA, !XYHA, SPR_TRI_LINE_0.qp());
    /*p30.ZURO*/ STORE3_LINE1 = dff8_B(XYHA, !XYHA, SPR_TRI_LINE_1.qp());
    /*p30.ZENE*/ STORE3_LINE2 = dff8_B(XYHA, !XYHA, SPR_TRI_LINE_2.qp());
    /*p30.ZYLU*/ STORE3_LINE3 = dff8_B(XYHA, !XYHA, SPR_TRI_LINE_3.qp());
    /*p31.XOLY*/ STORE3_X0    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, ZAGO_OAM_DB0);
    /*p31.XYBA*/ STORE3_X1    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, ZOCY_OAM_DB1);
    /*p31.XABE*/ STORE3_X2    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, YPUR_OAM_DB2);
    /*p31.XEKA*/ STORE3_X3    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, YVOK_OAM_DB3);
    /*p31.XOMY*/ STORE3_X4    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, COSE_OAM_DB4);
    /*p31.WUHA*/ STORE3_X5    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, AROP_OAM_DB5);
    /*p31.WYNA*/ STORE3_X6    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, XATU_OAM_DB6);
    /*p31.WECO*/ STORE3_X7    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, BADY_OAM_DB7);

    /*p29.WOFO*/ wire WOFO = not1(DUKE_STORE4_CLKn);
    /*p29.WYLU*/ wire WYLU = not1(DUKE_STORE4_CLKn);
    /*p29.EWOT*/ wire EWOT = not1(DUKE_STORE4_CLKn);
    /*p30.????*/ STORE4_INDX0 = dff8_B(WYLU, !WYLU, SPR_TRI_IDX5n.qp());
    /*p30.????*/ STORE4_INDX1 = dff8_B(WYLU, !WYLU, SPR_TRI_IDX4n.qp());
    /*p30.????*/ STORE4_INDX2 = dff8_B(WYLU, !WYLU, SPR_TRI_IDX3n.qp());
    /*p30.????*/ STORE4_INDX3 = dff8_B(WYLU, !WYLU, SPR_TRI_IDX2n.qp());
    /*p30.????*/ STORE4_INDX4 = dff8_B(WYLU, !WYLU, SPR_TRI_IDX1n.qp());
    /*p30.????*/ STORE4_INDX5 = dff8_B(WYLU, !WYLU, SPR_TRI_IDX0n.qp());
    /*p30.????*/ STORE4_LINE0 = dff8_B(EWOT, !EWOT, SPR_TRI_LINE_0.qp());
    /*p30.????*/ STORE4_LINE1 = dff8_B(EWOT, !EWOT, SPR_TRI_LINE_1.qp());
    /*p30.????*/ STORE4_LINE2 = dff8_B(EWOT, !EWOT, SPR_TRI_LINE_2.qp());
    /*p30.????*/ STORE4_LINE3 = dff8_B(EWOT, !EWOT, SPR_TRI_LINE_3.qp());
    /*p31.WEDU*/ STORE4_X0    = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, ZAGO_OAM_DB0);
    /*p31.YGAJ*/ STORE4_X1    = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, ZOCY_OAM_DB1);
    /*p31.ZYJO*/ STORE4_X2    = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, YPUR_OAM_DB2);
    /*p31.XURY*/ STORE4_X3    = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, YVOK_OAM_DB3);
    /*p31.YBED*/ STORE4_X4    = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, COSE_OAM_DB4);
    /*p31.ZALA*/ STORE4_X5    = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, AROP_OAM_DB5);
    /*p31.WYDE*/ STORE4_X6    = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, XATU_OAM_DB6);
    /*p31.XEPA*/ STORE4_X7    = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, BADY_OAM_DB7);

    /*p29.CYLA*/ wire CYLA = not1(BEDE_STORE5_CLKn);
    /*p29.DYMO*/ wire DYMO = not1(BEDE_STORE5_CLKn);
    /*p29.BUCY*/ wire BUCY = not1(BEDE_STORE5_CLKn);
    /*p30.????*/ STORE5_INDX0 = dff8_B(DYMO, !DYMO, SPR_TRI_IDX5n.qp());
    /*p30.????*/ STORE5_INDX1 = dff8_B(DYMO, !DYMO, SPR_TRI_IDX4n.qp());
    /*p30.????*/ STORE5_INDX2 = dff8_B(DYMO, !DYMO, SPR_TRI_IDX3n.qp());
    /*p30.????*/ STORE5_INDX3 = dff8_B(DYMO, !DYMO, SPR_TRI_IDX2n.qp());
    /*p30.????*/ STORE5_INDX4 = dff8_B(DYMO, !DYMO, SPR_TRI_IDX1n.qp());
    /*p30.????*/ STORE5_INDX5 = dff8_B(DYMO, !DYMO, SPR_TRI_IDX0n.qp());
    /*p30.????*/ STORE5_LINE0 = dff8_B(BUCY, !BUCY, SPR_TRI_LINE_0.qp());
    /*p30.????*/ STORE5_LINE1 = dff8_B(BUCY, !BUCY, SPR_TRI_LINE_1.qp());
    /*p30.????*/ STORE5_LINE2 = dff8_B(BUCY, !BUCY, SPR_TRI_LINE_2.qp());
    /*p30.????*/ STORE5_LINE3 = dff8_B(BUCY, !BUCY, SPR_TRI_LINE_3.qp());
    /*p31.FUSA*/ STORE5_X0    = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, ZAGO_OAM_DB0);
    /*p31.FAXA*/ STORE5_X1    = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, ZOCY_OAM_DB1);
    /*p31.FOZY*/ STORE5_X2    = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, YPUR_OAM_DB2);
    /*p31.FESY*/ STORE5_X3    = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, YVOK_OAM_DB3);
    /*p31.CYWE*/ STORE5_X4    = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, COSE_OAM_DB4);
    /*p31.DYBY*/ STORE5_X5    = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, AROP_OAM_DB5);
    /*p31.DURY*/ STORE5_X6    = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, XATU_OAM_DB6);
    /*p31.CUVY*/ STORE5_X7    = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, BADY_OAM_DB7);

    /*p29.ZAPE*/ wire ZAPE = not1(WEKA_STORE6_CLKn);
    /*p29.WUSE*/ wire WUSE = not1(WEKA_STORE6_CLKn);
    /*p29.ZURU*/ wire ZURU = not1(WEKA_STORE6_CLKn);
    /*p30.????*/ STORE6_INDX0 = dff8_B(WUSE, !WUSE, SPR_TRI_IDX5n.qp());
    /*p30.????*/ STORE6_INDX1 = dff8_B(WUSE, !WUSE, SPR_TRI_IDX4n.qp());
    /*p30.????*/ STORE6_INDX2 = dff8_B(WUSE, !WUSE, SPR_TRI_IDX3n.qp());
    /*p30.????*/ STORE6_INDX3 = dff8_B(WUSE, !WUSE, SPR_TRI_IDX2n.qp());
    /*p30.????*/ STORE6_INDX4 = dff8_B(WUSE, !WUSE, SPR_TRI_IDX1n.qp());
    /*p30.????*/ STORE6_INDX5 = dff8_B(WUSE, !WUSE, SPR_TRI_IDX0n.qp());
    /*p30.????*/ STORE6_LINE0 = dff8_B(ZURU, !ZURU, SPR_TRI_LINE_0.qp());
    /*p30.????*/ STORE6_LINE1 = dff8_B(ZURU, !ZURU, SPR_TRI_LINE_1.qp());
    /*p30.????*/ STORE6_LINE2 = dff8_B(ZURU, !ZURU, SPR_TRI_LINE_2.qp());
    /*p30.????*/ STORE6_LINE3 = dff8_B(ZURU, !ZURU, SPR_TRI_LINE_3.qp());
    /*p31.YCOL*/ STORE6_X0    = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, ZAGO_OAM_DB0);
    /*p31.YRAC*/ STORE6_X1    = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, ZOCY_OAM_DB1);
    /*p31.YMEM*/ STORE6_X2    = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, YPUR_OAM_DB2);
    /*p31.YVAG*/ STORE6_X3    = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, YVOK_OAM_DB3);
    /*p31.ZOLY*/ STORE6_X4    = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, COSE_OAM_DB4);
    /*p31.ZOGO*/ STORE6_X5    = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, AROP_OAM_DB5);
    /*p31.ZECU*/ STORE6_X6    = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, XATU_OAM_DB6);
    /*p31.ZESA*/ STORE6_X7    = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, BADY_OAM_DB7);

    /*p29.GECY*/ wire GECY = not1(GYVO_STORE7_CLKn);
    /*p29.FEFO*/ wire FEFO = not1(GYVO_STORE7_CLKn);
    /*p29.WABE*/ wire WABE = not1(GYVO_STORE7_CLKn);
    /*p30.????*/ STORE7_INDX0 = dff8_B(FEFO, !FEFO, SPR_TRI_IDX5n.qp());
    /*p30.????*/ STORE7_INDX1 = dff8_B(FEFO, !FEFO, SPR_TRI_IDX4n.qp());
    /*p30.????*/ STORE7_INDX2 = dff8_B(FEFO, !FEFO, SPR_TRI_IDX3n.qp());
    /*p30.????*/ STORE7_INDX3 = dff8_B(FEFO, !FEFO, SPR_TRI_IDX2n.qp());
    /*p30.????*/ STORE7_INDX4 = dff8_B(FEFO, !FEFO, SPR_TRI_IDX1n.qp());
    /*p30.????*/ STORE7_INDX5 = dff8_B(FEFO, !FEFO, SPR_TRI_IDX0n.qp());
    /*p30.????*/ STORE7_LINE0 = dff8_B(WABE, !WABE, SPR_TRI_LINE_0.qp());
    /*p30.????*/ STORE7_LINE1 = dff8_B(WABE, !WABE, SPR_TRI_LINE_1.qp());
    /*p30.????*/ STORE7_LINE2 = dff8_B(WABE, !WABE, SPR_TRI_LINE_2.qp());
    /*p30.????*/ STORE7_LINE3 = dff8_B(WABE, !WABE, SPR_TRI_LINE_3.qp());
    /*p31.ERAZ*/ STORE7_X0    = dff9(GECY, !GECY, GAFY_STORE7_RSTn, ZAGO_OAM_DB0);
    /*p31.EPUM*/ STORE7_X1    = dff9(GECY, !GECY, GAFY_STORE7_RSTn, ZOCY_OAM_DB1);
    /*p31.EROL*/ STORE7_X2    = dff9(GECY, !GECY, GAFY_STORE7_RSTn, YPUR_OAM_DB2);
    /*p31.EHYN*/ STORE7_X3    = dff9(GECY, !GECY, GAFY_STORE7_RSTn, YVOK_OAM_DB3);
    /*p31.FAZU*/ STORE7_X4    = dff9(GECY, !GECY, GAFY_STORE7_RSTn, COSE_OAM_DB4);
    /*p31.FAXE*/ STORE7_X5    = dff9(GECY, !GECY, GAFY_STORE7_RSTn, AROP_OAM_DB5);
    /*p31.EXUK*/ STORE7_X6    = dff9(GECY, !GECY, GAFY_STORE7_RSTn, XATU_OAM_DB6);
    /*p31.FEDE*/ STORE7_X7    = dff9(GECY, !GECY, GAFY_STORE7_RSTn, BADY_OAM_DB7);

    /*p29.CEXU*/ wire CEXU = not1(BUKA_STORE8_CLKn);
    /*p29.AKOL*/ wire AKOL = not1(BUKA_STORE8_CLKn);
    /*p29.BYMY*/ wire BYMY = not1(BUKA_STORE8_CLKn);
    /*p30.AXUV*/ STORE8_INDX5 = dff8_B(AKOL, !AKOL, SPR_TRI_IDX5n.qp());
    /*p30.BADA*/ STORE8_INDX4 = dff8_B(AKOL, !AKOL, SPR_TRI_IDX4n.qp());
    /*p30.APEV*/ STORE8_INDX3 = dff8_B(AKOL, !AKOL, SPR_TRI_IDX3n.qp());
    /*p30.BADO*/ STORE8_INDX2 = dff8_B(AKOL, !AKOL, SPR_TRI_IDX2n.qp());
    /*p30.BEXY*/ STORE8_INDX1 = dff8_B(AKOL, !AKOL, SPR_TRI_IDX1n.qp());
    /*p30.BYHE*/ STORE8_INDX0 = dff8_B(AKOL, !AKOL, SPR_TRI_IDX0n.qp());
    /*p30.AFYM*/ STORE8_LINE0 = dff8_B(BYMY, !BYMY, SPR_TRI_LINE_0.qp());
    /*p30.AZAP*/ STORE8_LINE1 = dff8_B(BYMY, !BYMY, SPR_TRI_LINE_1.qp());
    /*p30.AFUT*/ STORE8_LINE2 = dff8_B(BYMY, !BYMY, SPR_TRI_LINE_2.qp());
    /*p30.AFYX*/ STORE8_LINE3 = dff8_B(BYMY, !BYMY, SPR_TRI_LINE_3.qp());
    /*p31.GAVY*/ STORE8_X4    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, ZAGO_OAM_DB0);
    /*p31.GYPU*/ STORE8_X5    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, ZOCY_OAM_DB1);
    /*p31.GADY*/ STORE8_X6    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, YPUR_OAM_DB2);
    /*p31.GAZA*/ STORE8_X7    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, YVOK_OAM_DB3);
    /*p31.EZUF*/ STORE8_X0    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, COSE_OAM_DB4);
    /*p31.ENAD*/ STORE8_X1    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, AROP_OAM_DB5);
    /*p31.EBOW*/ STORE8_X2    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, XATU_OAM_DB6);
    /*p31.FYCA*/ STORE8_X3    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, BADY_OAM_DB7);

    /*p29.WEME*/ wire STORE9_CLKa = not1(DECU_STORE9_CLKn);
    /*p29.WUFA*/ wire STORE9_CLKb = not1(DECU_STORE9_CLKn);
    /*p29.FAKA*/ wire STORE9_CLKc = not1(DECU_STORE9_CLKn);
    /*p30.XUFO*/ STORE9_INDX0 = dff8_B(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_IDX5n.qp());
    /*p30.XUTE*/ STORE9_INDX1 = dff8_B(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_IDX4n.qp());
    /*p30.XOTU*/ STORE9_INDX2 = dff8_B(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_IDX3n.qp());
    /*p30.XYFE*/ STORE9_INDX3 = dff8_B(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_IDX2n.qp());
    /*p30.YZOR*/ STORE9_INDX4 = dff8_B(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_IDX1n.qp());
    /*p30.YBER*/ STORE9_INDX5 = dff8_B(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_IDX0n.qp());
    /*p30.DEWU*/ STORE9_LINE0 = dff8_B(STORE9_CLKc, !STORE9_CLKc, SPR_TRI_LINE_0.qp());
    /*p30.CANA*/ STORE9_LINE1 = dff8_B(STORE9_CLKc, !STORE9_CLKc, SPR_TRI_LINE_1.qp());
    /*p30.DYSY*/ STORE9_LINE2 = dff8_B(STORE9_CLKc, !STORE9_CLKc, SPR_TRI_LINE_2.qp());
    /*p30.FOFO*/ STORE9_LINE3 = dff8_B(STORE9_CLKc, !STORE9_CLKc, SPR_TRI_LINE_3.qp());
    /*p31.XUVY*/ STORE9_X0    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, ZAGO_OAM_DB0);
    /*p31.XERE*/ STORE9_X1    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, ZOCY_OAM_DB1);
    /*p31.XUZO*/ STORE9_X2    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, YPUR_OAM_DB2);
    /*p31.XEXA*/ STORE9_X3    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, YVOK_OAM_DB3);
    /*p31.YPOD*/ STORE9_X4    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, COSE_OAM_DB4);
    /*p31.YROP*/ STORE9_X5    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, AROP_OAM_DB5);
    /*p31.YNEP*/ STORE9_X6    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, XATU_OAM_DB6);
    /*p31.YZOF*/ STORE9_X7    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, BADY_OAM_DB7);
  }
}

//------------------------------------------------------------------------------
