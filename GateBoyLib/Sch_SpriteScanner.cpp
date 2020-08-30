#include "GateBoyLib/Sch_SpriteScanner.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void SpriteScanner::dump(Dumper& d, const SchematicTop& top) const {
  d("----------SpriteScan ---------\n");

  d("BESU_SCANNINGp   %c\n", BESU_SCANNINGp  .c());
  d("CENO_SCANNINGp   %c\n", CENO_SCANNINGp  .c());
  d("BYBA_SCAN_DONE_A %c\n", BYBA_SCAN_DONE_A.c());
  d("DOBA_SCAN_DONE_B %c\n", DOBA_SCAN_DONE_B.c());
  d("SCAN INDEX %02d\n", pack_p(YFEL_SCAN0.qp(), WEWY_SCAN1.qp(), GOSO_SCAN2.qp(), ELYN_SCAN3.qp(),
                                FAHA_SCAN4.qp(), FONY_SCAN5.qp(), 0, 0));
  d("LCD Y      %03d\n", top.lcd_reg.get_y());
  d("Y DIFF     %03d\n", pack_p(DEGE_SPRITE_DELTA0, DABY_SPRITE_DELTA1, DABU_SPRITE_DELTA2, GYSA_SPRITE_DELTA3,
                                GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7));

  d("CARE_STORE_ENp_ABxxEFxx %d\n", (wire)CARE_STORE_ENp_ABxxEFxx);
  d("\n");
}

//------------------------------------------------------------------------------

void SpriteScanner::tick(const SchematicTop& top) {
  wire GND = 0;

  // this is using an adder as a subtracter by inverting the first input.

  /*#p29.EBOS*/ wire EBOS_Y0n = not1(top.lcd_reg.MUWY_Y0p.qp());
  /* p29.DASA*/ wire DASA_Y1n = not1(top.lcd_reg.MYRO_Y1p.qp());
  /* p29.FUKY*/ wire FUKY_Y2n = not1(top.lcd_reg.LEXA_Y2p.qp());
  /* p29.FUVE*/ wire FUVE_Y3n = not1(top.lcd_reg.LYDO_Y3p.qp());
  /* p29.FEPU*/ wire FEPU_Y4n = not1(top.lcd_reg.LOVU_Y4p.qp());
  /* p29.FOFA*/ wire FOFA_Y5n = not1(top.lcd_reg.LEMA_Y5p.qp());
  /* p29.FEMO*/ wire FEMO_Y6n = not1(top.lcd_reg.MATO_Y6p.qp());
  /* p29.GUSU*/ wire GUSU_Y7n = not1(top.lcd_reg.LAFO_Y7p.qp());

  /*p29.ERUC*/ wire ERUC_YDIFF_S0 = add_s(EBOS_Y0n, top.oam_bus.XUSO_OAM_DA0p.qp(), GND);
  /*p29.ERUC*/ wire ERUC_YDIFF_C0 = add_c(EBOS_Y0n, top.oam_bus.XUSO_OAM_DA0p.qp(), GND);
  /*p29.ENEF*/ wire ENEF_YDIFF_S1 = add_s(DASA_Y1n, top.oam_bus.XEGU_OAM_DA1p.qp(), ERUC_YDIFF_C0);
  /*p29.ENEF*/ wire ENEF_YDIFF_C1 = add_c(DASA_Y1n, top.oam_bus.XEGU_OAM_DA1p.qp(), ERUC_YDIFF_C0);
  /*p29.FECO*/ wire FECO_YDIFF_S2 = add_s(FUKY_Y2n, top.oam_bus.YJEX_OAM_DA2p.qp(), ENEF_YDIFF_C1);
  /*p29.FECO*/ wire FECO_YDIFF_C2 = add_c(FUKY_Y2n, top.oam_bus.YJEX_OAM_DA2p.qp(), ENEF_YDIFF_C1);
  /*p29.GYKY*/ wire GYKY_YDIFF_S3 = add_s(FUVE_Y3n, top.oam_bus.XYJU_OAM_DA3p.qp(), FECO_YDIFF_C2);
  /*p29.GYKY*/ wire GYKY_YDIFF_C3 = add_c(FUVE_Y3n, top.oam_bus.XYJU_OAM_DA3p.qp(), FECO_YDIFF_C2);
  /*p29.GOPU*/ wire GOPU_YDIFF_S4 = add_s(FEPU_Y4n, top.oam_bus.YBOG_OAM_DA4p.qp(), GYKY_YDIFF_C3);
  /*p29.GOPU*/ wire GOPU_YDIFF_C4 = add_c(FEPU_Y4n, top.oam_bus.YBOG_OAM_DA4p.qp(), GYKY_YDIFF_C3);
  /*p29.FUWA*/ wire FUWA_YDIFF_S5 = add_s(FOFA_Y5n, top.oam_bus.WYSO_OAM_DA5p.qp(), GOPU_YDIFF_C4);
  /*p29.FUWA*/ wire FUWA_YDIFF_C5 = add_c(FOFA_Y5n, top.oam_bus.WYSO_OAM_DA5p.qp(), GOPU_YDIFF_C4);
  /*p29.GOJU*/ wire GOJU_YDIFF_S6 = add_s(FEMO_Y6n, top.oam_bus.XOTE_OAM_DA6p.qp(), FUWA_YDIFF_C5);
  /*p29.GOJU*/ wire GOJU_YDIFF_C6 = add_c(FEMO_Y6n, top.oam_bus.XOTE_OAM_DA6p.qp(), FUWA_YDIFF_C5);
  /*p29.WUHU*/ wire WUHU_YDIFF_S7 = add_s(GUSU_Y7n, top.oam_bus.YZAB_OAM_DA7p.qp(), GOJU_YDIFF_C6);
  /*p29.WUHU*/ wire WUHU_YDIFF_C7 = add_c(GUSU_Y7n, top.oam_bus.YZAB_OAM_DA7p.qp(), GOJU_YDIFF_C6);

  /* p29.DEGE*/ DEGE_SPRITE_DELTA0 = not1(ERUC_YDIFF_S0);
  /* p29.DABY*/ DABY_SPRITE_DELTA1 = not1(ENEF_YDIFF_S1);
  /* p29.DABU*/ DABU_SPRITE_DELTA2 = not1(FECO_YDIFF_S2);
  /* p29.GYSA*/ GYSA_SPRITE_DELTA3 = not1(GYKY_YDIFF_S3);
  /* p29.GACE*/ GACE_SPRITE_DELTA4 = not1(GOPU_YDIFF_S4);
  /* p29.GUVU*/ GUVU_SPRITE_DELTA5 = not1(FUWA_YDIFF_S5);
  /* p29.GYDA*/ GYDA_SPRITE_DELTA6 = not1(GOJU_YDIFF_S6);
  /* p29.GEWY*/ GEWY_SPRITE_DELTA7 = not1(WUHU_YDIFF_S7);

  /*#p29.GOVU*/ wire GOVU_SPSIZE_MATCH  = or2(top.pix_pipe.XYMO_LCDC_SPSIZEn.qn(), GYKY_YDIFF_S3);
  /* p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, WUHU_YDIFF_C7, GOVU_SPSIZE_MATCH);
  /* p29.GESE*/ wire GESE_SCAN_MATCH_Y  = not1(WOTA_SCAN_MATCH_Yn);
  /* p29.CEHA*/ wire CEHA_SCANNINGp     = not1(CENO_SCANNINGp.qn());

  /* p29.CARE*/ CARE_STORE_ENp_ABxxEFxx = and3(top.clk_reg.XOCE_AxxDExxH, CEHA_SCANNINGp, GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.
}

//------------------------------------------------------------------------------

void SpriteScanner::tock(const SchematicTop& top) {
  /*p01.ATAR*/ wire ATAR_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not1(ATAR_VID_RSTp);

  /*p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /*p01.ALET*/ wire ALET_xBxDxFxH = not1(ZEME_AxCxExGx);

  // 32 + 4 + 2 + 1 = 39
  /*#p28.FETO*/ wire FETO_SCAN_DONE_d0 = and4(YFEL_SCAN0.qp(), WEWY_SCAN1.qp(), GOSO_SCAN2.qp(), FONY_SCAN5.qp());
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp, ATAR_VID_RSTp);

  //----------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  {
    /*#p29.BALU*/ wire BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
    /*#p29.BAGY*/ wire BAGY_LINE_RSTn = not1(BALU_LINE_RSTp);
    /*#p29.BYBA*/ BYBA_SCAN_DONE_A = dff17_AB(top.clk_reg.XUPY_xxCDxxGH, BAGY_LINE_RSTn, FETO_SCAN_DONE_d0);
    /*#p29.DOBA*/ DOBA_SCAN_DONE_B = dff17_B (ALET_xBxDxFxH, BAGY_LINE_RSTn, BYBA_SCAN_DONE_A.qp());

    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_B.qp(), BALU_LINE_RSTp, BYBA_SCAN_DONE_A.qn());
    /*#p29.AVAP*/ wire AVAP_RENDER_START_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);

    /*#p28.ASEN*/ wire ASEN_SCAN_DONE_PE = or2(ATAR_VID_RSTp, AVAP_RENDER_START_TRIGp);
    /*#p28.BESU*/ BESU_SCANNINGp = nor_latch(top.lcd_reg.CATU_VID_LINE_P910.qp(), ASEN_SCAN_DONE_PE);
    /*#p29.CENO*/ CENO_SCANNINGp = dff17_A(top.clk_reg.XUPY_xxCDxxGH, ABEZ_VID_RSTn, BESU_SCANNINGp.qp());
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire GAVA_SCAN_CLK = or2(FETO_SCAN_DONE_d0,   top.clk_reg.XUPY_xxCDxxGH);

    /*p28.YFEL*/ YFEL_SCAN0 = dff17_AB(GAVA_SCAN_CLK,   ANOM_LINE_RSTn, YFEL_SCAN0.qn());
    /*p28.WEWY*/ WEWY_SCAN1 = dff17_AB(YFEL_SCAN0.qn(), ANOM_LINE_RSTn, WEWY_SCAN1.qn());
    /*p28.GOSO*/ GOSO_SCAN2 = dff17_AB(WEWY_SCAN1.qn(), ANOM_LINE_RSTn, GOSO_SCAN2.qn());
    /*p28.ELYN*/ ELYN_SCAN3 = dff17_AB(GOSO_SCAN2.qn(), ANOM_LINE_RSTn, ELYN_SCAN3.qn());
    /*p28.FAHA*/ FAHA_SCAN4 = dff17_AB(ELYN_SCAN3.qn(), ANOM_LINE_RSTn, FAHA_SCAN4.qn());
    /*p28.FONY*/ FONY_SCAN5 = dff17_AB(FAHA_SCAN4.qn(), ANOM_LINE_RSTn, FONY_SCAN5.qn());
  }
}

//------------------------------------------------------------------------------
