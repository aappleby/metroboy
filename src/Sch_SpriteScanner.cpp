#include "Sch_SpriteScanner.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void SpriteScanner::tick_ymatch(SchematicTop& top) {
  {
    /*p29.EBOS*/ wire _EBOS_Y0n = not(top.MUWY_Y0());
    /*p29.DASA*/ wire _DASA_Y1n = not(top.MYRO_Y1());
    /*p29.FUKY*/ wire _FUKY_Y2n = not(top.LEXA_Y2());
    /*p29.FUVE*/ wire _FUVE_Y3n = not(top.LYDO_Y3());
    /*p29.FEPU*/ wire _FEPU_Y4n = not(top.LOVU_Y4());
    /*p29.FOFA*/ wire _FOFA_Y5n = not(top.LEMA_Y5());
    /*p29.FEMO*/ wire _FEMO_Y6n = not(top.MATO_Y6());
    /*p29.GUSU*/ wire _GUSU_Y7n = not(top.LAFO_Y7());

    /*p29.ERUC*/ ERUC_YDIFF_S0 = add_c(_EBOS_Y0n, top.XUSO_SPRITE_Y0(), top.GND);
    /*p29.ERUC*/ ERUC_YDIFF_C0 = add_s(_EBOS_Y0n, top.XUSO_SPRITE_Y0(), top.GND);
    /*p29.ENEF*/ ENEF_YDIFF_S1 = add_s(_DASA_Y1n, top.XEGU_SPRITE_Y1(), ERUC_YDIFF_C0);
    /*p29.ENEF*/ ENEF_YDIFF_C1 = add_c(_DASA_Y1n, top.XEGU_SPRITE_Y1(), ERUC_YDIFF_C0);
    /*p29.FECO*/ FECO_YDIFF_S2 = add_s(_FUKY_Y2n, top.YJEX_SPRITE_Y2(), ENEF_YDIFF_C1);
    /*p29.FECO*/ FECO_YDIFF_C2 = add_c(_FUKY_Y2n, top.YJEX_SPRITE_Y2(), ENEF_YDIFF_C1);
    /*p29.GYKY*/ GYKY_YDIFF_S3 = add_s(_FUVE_Y3n, top.XYJU_SPRITE_Y3(), FECO_YDIFF_C2);
    /*p29.GYKY*/ GYKY_YDIFF_C3 = add_c(_FUVE_Y3n, top.XYJU_SPRITE_Y3(), FECO_YDIFF_C2);
    /*p29.GOPU*/ GOPU_YDIFF_S4 = add_s(_FEPU_Y4n, top.YBOG_SPRITE_Y4(), GYKY_YDIFF_C3);
    /*p29.GOPU*/ GOPU_YDIFF_C4 = add_c(_FEPU_Y4n, top.YBOG_SPRITE_Y4(), GYKY_YDIFF_C3);
    /*p29.FUWA*/ FUWA_YDIFF_S5 = add_s(_FOFA_Y5n, top.WYSO_SPRITE_Y5(), GOPU_YDIFF_C4);
    /*p29.FUWA*/ FUWA_YDIFF_C5 = add_c(_FOFA_Y5n, top.WYSO_SPRITE_Y5(), GOPU_YDIFF_C4);
    /*p29.GOJU*/ GOJU_YDIFF_S6 = add_s(_FEMO_Y6n, top.XOTE_SPRITE_Y6(), FUWA_YDIFF_C5);
    /*p29.GOJU*/ GOJU_YDIFF_C6 = add_c(_FEMO_Y6n, top.XOTE_SPRITE_Y6(), FUWA_YDIFF_C5);
    /*p29.WUHU*/ WUHU_YDIFF_S7 = add_s(_GUSU_Y7n, top.YZAB_SPRITE_Y7(), GOJU_YDIFF_C6);
    /*p29.WUHU*/ WUHU_YDIFF_C7 = add_c(_GUSU_Y7n, top.YZAB_SPRITE_Y7(), GOJU_YDIFF_C6);
  }

  {
    /*p29.GACE*/ wire _GACE_SPRITE_DELTA4 = not(GOPU_YDIFF_S4);
    /*p29.GUVU*/ wire _GUVU_SPRITE_DELTA5 = not(FUWA_YDIFF_S5);
    /*p29.GYDA*/ wire _GYDA_SPRITE_DELTA6 = not(GOJU_YDIFF_S6);
    /*p29.GEWY*/ wire _GEWY_SPRITE_DELTA7 = not(WUHU_YDIFF_S7);

    /*p29.GOVU*/ wire _GOVU_SPSIZE_MATCH = or (GYKY_YDIFF_S3, top.XYMO_LCDC_SPSIZE.q());
    /*p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn = nand(_GACE_SPRITE_DELTA4, _GUVU_SPRITE_DELTA5, _GYDA_SPRITE_DELTA6, _GEWY_SPRITE_DELTA7, WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH);
    /*p29.GESE*/ wire _GESE_SCAN_MATCH_Y = not(_WOTA_SCAN_MATCH_Yn);
    /*p29.CARE*/ CARE_STORE_ENp_ABxxEFxx = and (top.XOCE_AxxDExxH(), top.CEHA_SCANNINGp(), _GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.
  }
}

//------------------------------------------------------------------------------

void SpriteScanner::tock(SchematicTop& top) {

  /*p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and (YFEL_SCAN0.q(), WEWY_SCAN1.q(), GOSO_SCAN2.q(), FONY_SCAN5.q()); // die AND. 32 + 4 + 2 + 1 = 39

  //----------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  {
    /*p29.BAGY*/ wire _BAGY_LINE_RSTn = not(top.BALU_LINE_RSTp());
    /*p29.BYBA*/ BYBA_SCAN_DONE_A.set(top.XUPY_ABxxEFxx(), _BAGY_LINE_RSTn, _FETO_SCAN_DONE_d0);
    /*p29.DOBA*/ DOBA_SCAN_DONE_B.set(top.ALET_xBxDxFxH(), _BAGY_LINE_RSTn, BYBA_SCAN_DONE_A.q());

    /*p28.ASEN*/ wire _ASEN_SCAN_DONE_PE = or (top.ATAR_VID_RSTp(), top.AVAP_RENDER_START_RST());
    /*p28.BESU*/ BESU_SCANNINGp.nor_latch(top.CATU_LINE_END(), _ASEN_SCAN_DONE_PE);
    /*p29.CENO*/ CENO_SCANNINGp.set(top.XUPY_ABxxEFxx(), top.ABEZ_VID_RSTn(), BESU_SCANNINGp.q());
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or(_FETO_SCAN_DONE_d0,   top.XUPY_ABxxEFxx());
    /*p28.YFEL*/ YFEL_SCAN0.set(_GAVA_SCAN_CLK,  top.ANOM_LINE_RSTn(), YFEL_SCAN0.qn());
    /*p28.WEWY*/ WEWY_SCAN1.set(YFEL_SCAN0.qn(), top.ANOM_LINE_RSTn(), WEWY_SCAN1.qn());
    /*p28.GOSO*/ GOSO_SCAN2.set(WEWY_SCAN1.qn(), top.ANOM_LINE_RSTn(), GOSO_SCAN2.qn());
    /*p28.ELYN*/ ELYN_SCAN3.set(GOSO_SCAN2.qn(), top.ANOM_LINE_RSTn(), ELYN_SCAN3.qn());
    /*p28.FAHA*/ FAHA_SCAN4.set(ELYN_SCAN3.qn(), top.ANOM_LINE_RSTn(), FAHA_SCAN4.qn());
    /*p28.FONY*/ FONY_SCAN5.set(FAHA_SCAN4.qn(), top.ANOM_LINE_RSTn(), FONY_SCAN5.qn());
  }
}

//------------------------------------------------------------------------------

SignalHash SpriteScanner::commit() {
  SignalHash hash;

  /*p28.BESU*/ hash << BESU_SCANNINGp.commit();
  /*p29.CENO*/ hash << CENO_SCANNINGp.commit();

  /*p29.ERUC*/ hash << ERUC_YDIFF_S0.commit();
  /*p29.ERUC*/ hash << ERUC_YDIFF_C0.commit();
  /*p29.ENEF*/ hash << ENEF_YDIFF_S1.commit();
  /*p29.ENEF*/ hash << ENEF_YDIFF_C1.commit();
  /*p29.FECO*/ hash << FECO_YDIFF_S2.commit();
  /*p29.FECO*/ hash << FECO_YDIFF_C2.commit();
  /*p29.GYKY*/ hash << GYKY_YDIFF_S3.commit();
  /*p29.GYKY*/ hash << GYKY_YDIFF_C3.commit();
  /*p29.GOPU*/ hash << GOPU_YDIFF_S4.commit();
  /*p29.GOPU*/ hash << GOPU_YDIFF_C4.commit();
  /*p29.FUWA*/ hash << FUWA_YDIFF_S5.commit();
  /*p29.FUWA*/ hash << FUWA_YDIFF_C5.commit();
  /*p29.GOJU*/ hash << GOJU_YDIFF_S6.commit();
  /*p29.GOJU*/ hash << GOJU_YDIFF_C6.commit();
  /*p29.WUHU*/ hash << WUHU_YDIFF_S7.commit();
  /*p29.WUHU*/ hash << WUHU_YDIFF_C7.commit();

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
