#include "Sch_SpriteScanner.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void SpriteScanner::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void SpriteScanner::tock(SchematicTop& top) {

  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(top.BYHA_VID_LINE_TRIG_d4());
  /*p28.ANOM*/ wire ANOM_LINE_RSTn = nor(ATEJ_VID_LINE_TRIG_d4p, top.ATAR_VID_RSTp());
  /*p29.BALU*/ wire BALU_LINE_RSTp = not(ANOM_LINE_RSTn);

  /*p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and (YFEL_SCAN0.q(), WEWY_SCAN1.q(), GOSO_SCAN2.q(), FONY_SCAN5.q()); // die AND. 32 + 4 + 2 + 1 = 39

  //----------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  {
    /*p29.BAGY*/ wire BAGY_LINE_RSTn = not(BALU_LINE_RSTp);
    /*p29.BYBA*/ BYBA_SCAN_DONE_A.set(top.XUPY_ABxxEFxx(), BAGY_LINE_RSTn, _FETO_SCAN_DONE_d0);
    /*p29.DOBA*/ DOBA_SCAN_DONE_B.set(top.ALET_xBxDxFxH(), BAGY_LINE_RSTn, BYBA_SCAN_DONE_A.q());

    // BEBU_01 << DOBA_17
    // BEBU_02 << BALU_02
    // BEBU_03 << BYBA_16
    // BEBU_04 nc
    // BEBU_05 >> AVAP_01


    /*p29.BEBU*/ wire BEBU_SCAN_DONE_PEn = or (BALU_LINE_RSTp, DOBA_SCAN_DONE_B.q(), !BYBA_SCAN_DONE_A.q());
    /*p29.AVAP*/ wire AVAP_SCAN_DONE_PE = not(BEBU_SCAN_DONE_PEn);
    /*p28.ASEN*/ wire ASEN_SCAN_DONE_PE = or (top.ATAR_VID_RSTp(), AVAP_SCAN_DONE_PE);

    // Arms on ground, nor latch
    // BESU01 << CATU17
    // BESU02 nc
    // BESU03 >> nc
    // BESU04 >> ACYL02
    // BESU05 nc
    // BESU06 << ASEN04

    // When CATU goes high, BESU goes high
    // When ASEN goes high, BESU goes low.

    /*p28.BESU*/ BESU_SCANNINGp.nor_latch(top.CATU_LINE_END(), ASEN_SCAN_DONE_PE);
    /*p29.CENO*/ CENO_SCANNINGp.set(top.XUPY_ABxxEFxx(), top.ABEZ_VID_RSTn(), BESU_SCANNINGp.q());
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or(_FETO_SCAN_DONE_d0,   top.XUPY_ABxxEFxx());
    /*p28.YFEL*/ YFEL_SCAN0.set(_GAVA_SCAN_CLK,  ANOM_LINE_RSTn, YFEL_SCAN0.qn());
    /*p28.WEWY*/ WEWY_SCAN1.set(YFEL_SCAN0.qn(), ANOM_LINE_RSTn, WEWY_SCAN1.qn());
    /*p28.GOSO*/ GOSO_SCAN2.set(WEWY_SCAN1.qn(), ANOM_LINE_RSTn, GOSO_SCAN2.qn());
    /*p28.ELYN*/ ELYN_SCAN3.set(GOSO_SCAN2.qn(), ANOM_LINE_RSTn, ELYN_SCAN3.qn());
    /*p28.FAHA*/ FAHA_SCAN4.set(ELYN_SCAN3.qn(), ANOM_LINE_RSTn, FAHA_SCAN4.qn());
    /*p28.FONY*/ FONY_SCAN5.set(FAHA_SCAN4.qn(), ANOM_LINE_RSTn, FONY_SCAN5.qn());
  }

  //----------------------------------------
  // Sprite Y match and store enable

  {
    /*p29.EBOS*/ wire Y0n = not(top.MUWY_Y0());
    /*p29.DASA*/ wire Y1n = not(top.MYRO_Y1());
    /*p29.FUKY*/ wire Y2n = not(top.LEXA_Y2());
    /*p29.FUVE*/ wire Y3n = not(top.LYDO_Y3());
    /*p29.FEPU*/ wire Y4n = not(top.LOVU_Y4());
    /*p29.FOFA*/ wire Y5n = not(top.LEMA_Y5());
    /*p29.FEMO*/ wire Y6n = not(top.MATO_Y6());
    /*p29.GUSU*/ wire Y7n = not(top.LAFO_Y7());

    /*p29.ERUC*/ wire YDIFF_S0 = add_c(Y0n, top.XUSO_SPRITE_Y0(), top.GND);
    /*p29.ERUC*/ wire YDIFF_C0 = add_s(Y0n, top.XUSO_SPRITE_Y0(), top.GND);
    /*p29.ENEF*/ wire YDIFF_S1 = add_s(Y1n, top.XEGU_SPRITE_Y1(), YDIFF_C0);
    /*p29.ENEF*/ wire YDIFF_C1 = add_c(Y1n, top.XEGU_SPRITE_Y1(), YDIFF_C0);
    /*p29.FECO*/ wire YDIFF_S2 = add_s(Y2n, top.YJEX_SPRITE_Y2(), YDIFF_C1);
    /*p29.FECO*/ wire YDIFF_C2 = add_c(Y2n, top.YJEX_SPRITE_Y2(), YDIFF_C1);
    /*p29.GYKY*/ wire YDIFF_S3 = add_s(Y3n, top.XYJU_SPRITE_Y3(), YDIFF_C2);
    /*p29.GYKY*/ wire YDIFF_C3 = add_c(Y3n, top.XYJU_SPRITE_Y3(), YDIFF_C2);
    /*p29.GOPU*/ wire YDIFF_S4 = add_s(Y4n, top.YBOG_SPRITE_Y4(), YDIFF_C3);
    /*p29.GOPU*/ wire YDIFF_C4 = add_c(Y4n, top.YBOG_SPRITE_Y4(), YDIFF_C3);
    /*p29.FUWA*/ wire YDIFF_S5 = add_s(Y5n, top.WYSO_SPRITE_Y5(), YDIFF_C4);
    /*p29.FUWA*/ wire YDIFF_C5 = add_c(Y5n, top.WYSO_SPRITE_Y5(), YDIFF_C4);
    /*p29.GOJU*/ wire YDIFF_S6 = add_s(Y6n, top.XOTE_SPRITE_Y6(), YDIFF_C5);
    /*p29.GOJU*/ wire YDIFF_C6 = add_c(Y6n, top.XOTE_SPRITE_Y6(), YDIFF_C5);
    /*p29.WUHU*/ wire YDIFF_S7 = add_s(Y7n, top.YZAB_SPRITE_Y7(), YDIFF_C6);
    /*p29.WUHU*/ wire YDIFF_C7 = add_c(Y7n, top.YZAB_SPRITE_Y7(), YDIFF_C6);

    /*p29.DEGE*/ DEGE_SPRITE_DELTA0 = not(YDIFF_S0);
    /*p29.DABY*/ DABY_SPRITE_DELTA1 = not(YDIFF_S1);
    /*p29.DABU*/ DABU_SPRITE_DELTA2 = not(YDIFF_S2);
    /*p29.GYSA*/ GYSA_SPRITE_DELTA3 = not(YDIFF_S3);
    /*p29.GACE*/ wire GACE_SPRITE_DELTA4 = not(YDIFF_S4);
    /*p29.GUVU*/ wire GUVU_SPRITE_DELTA5 = not(YDIFF_S5);
    /*p29.GYDA*/ wire GYDA_SPRITE_DELTA6 = not(YDIFF_S6);
    /*p29.GEWY*/ wire GEWY_SPRITE_DELTA7 = not(YDIFF_S7);

    /*p29.GOVU*/ wire GOVU_SPSIZE_MATCH = or (YDIFF_S3, top.XYMO_LCDC_SPSIZE.q());
    /*p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, YDIFF_C7, GOVU_SPSIZE_MATCH);
    /*p29.GESE*/ wire GESE_SCAN_MATCH_Y = not(WOTA_SCAN_MATCH_Yn);
    /*p29.CEHA*/ wire CEHA_SCANNINGp = not(CENO_SCANNINGp.qn());
    /*p29.CARE*/ CARE_STORE_ENp_ABxxEFxx = and (top.XOCE_AxxDExxH(), CEHA_SCANNINGp, GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.
  }
}

//------------------------------------------------------------------------------

SignalHash SpriteScanner::commit() {
  SignalHash hash;

  /*p28.BESU*/ hash << BESU_SCANNINGp.commit();
  /*p29.CENO*/ hash << CENO_SCANNINGp.commit();

  /*p29.DEGE*/ hash << DEGE_SPRITE_DELTA0.commit();
  /*p29.DABY*/ hash << DABY_SPRITE_DELTA1.commit();
  /*p29.DABU*/ hash << DABU_SPRITE_DELTA2.commit();
  /*p29.GYSA*/ hash << GYSA_SPRITE_DELTA3.commit();
  /*p29.CARE*/ hash << CARE_STORE_ENp_ABxxEFxx.commit();

  /*p28.YFEL*/ hash << YFEL_SCAN0.commit();
  /*p28.WEWY*/ hash << WEWY_SCAN1.commit();
  /*p28.GOSO*/ hash << GOSO_SCAN2.commit();
  /*p28.ELYN*/ hash << ELYN_SCAN3.commit();
  /*p28.FAHA*/ hash << FAHA_SCAN4.commit();
  /*p28.FONY*/ hash << FONY_SCAN5.commit();
  /*p29.BYBA*/ hash << BYBA_SCAN_DONE_A.commit();
  /*p29.DOBA*/ hash << DOBA_SCAN_DONE_B.commit();

  return hash;
}

//------------------------------------------------------------------------------
