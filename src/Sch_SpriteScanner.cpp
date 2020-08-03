#include "Sch_SpriteScanner.h"
#include "Sch_Top.h"

using namespace Schematics;

void SpriteScanner::dump(Dumper& d, const SchematicTop& top) const {
  d("----------SpriteScan ---------\n");

  /*p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or(_BALU_LINE_RSTp, DOBA_SCAN_DONE_B(), !BYBA_SCAN_DONE_A());
  /*p29.AVAP*/ wire AVAP_RENDER_START_TRIGp = not(_BEBU_SCAN_DONE_TRIGn);
  wire CATU_VID_LINE_ENDp = top.lcd_reg.CATU_VID_LINE_ENDp();
  /*p28.ASEN*/ wire _ASEN_SCAN_DONE_PE = or (top.clk_reg.ATAR_VID_RSTp(), AVAP_RENDER_START_TRIGp);

  d("_BALU_LINE_RSTp         %d\n", _BALU_LINE_RSTp.as_wire());
  d("_BEBU_SCAN_DONE_TRIGn   %d\n", _BEBU_SCAN_DONE_TRIGn);
  d("AVAP_RENDER_START_TRIGp %d\n", AVAP_RENDER_START_TRIGp);
  d("CATU_VID_LINE_ENDp      %d\n", CATU_VID_LINE_ENDp);
  d("_ASEN_SCAN_DONE_PE      %d\n", _ASEN_SCAN_DONE_PE);

  d("_BESU_SCANNINGp   %c\n", _BESU_SCANNINGp  .c());
  d("_CENO_SCANNINGp   %c\n", _CENO_SCANNINGp  .c());
  d("_BYBA_SCAN_DONE_A %c\n", _BYBA_SCAN_DONE_A.c());
  d("_DOBA_SCAN_DONE_B %c\n", _DOBA_SCAN_DONE_B.c());

  /*p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and (_YFEL_SCAN0.q(), _WEWY_SCAN1.q(), _GOSO_SCAN2.q(), _FONY_SCAN5.q()); // die AND. 32 + 4 + 2 + 1 = 39
  /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or(_FETO_SCAN_DONE_d0,   top.clk_reg.XUPY_ABxxEFxx());
  /*p28.ANOM*/ wire ANOM_LINE_RSTn = nor(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.ATAR_VID_RSTp());
  d("_GAVA_SCAN_CLK %d\n", _GAVA_SCAN_CLK);
  d("ANOM_LINE_RSTn %d\n", ANOM_LINE_RSTn);
  d("SCAN INDEX        %02d\n", 
    pack(
      _YFEL_SCAN0.q(),
      _WEWY_SCAN1.q(),
      _GOSO_SCAN2.q(),
      _ELYN_SCAN3.q(),
      _FAHA_SCAN4.q(),
      _FONY_SCAN5.q(),
      0,
      0
    )
  );

  int y_diff = pack(_ERUC_YDIFF_S0, _ENEF_YDIFF_S1, _FECO_YDIFF_S2, _GYKY_YDIFF_S3,
                    _GOPU_YDIFF_S4, _FUWA_YDIFF_S5, _GOJU_YDIFF_S6, _WUHU_YDIFF_S7);

  d("LCD Y    %d\n", top.lcd_reg.get_y());
  d("SPRITE Y %d\n", top.oam_bus.get_temp_b());
  d("Y DIFF   %02d\n", y_diff);

  d("\n");
}


//------------------------------------------------------------------------------

void SpriteScanner::tick(const SchematicTop& top) {
  wire GND = 0;

  {
    _XYLO_LCDC_SPEN = top.pix_pipe.XYLO_LCDC_SPEN.q();
    _XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGp();

    /*p28.ANOM*/ wire ANOM_LINE_RSTn = nor(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.ATAR_VID_RSTp());

    /*p29.BALU*/ _BALU_LINE_RSTp = not(ANOM_LINE_RSTn);
    /*p29.BAGY*/ _BAGY_LINE_RSTn = not(_BALU_LINE_RSTp);
  }

  {
    /*p29.EBOS*/ wire _EBOS_Y0n = not(top.lcd_reg.MUWY_Y0.q());
    /*p29.DASA*/ wire _DASA_Y1n = not(top.lcd_reg.MYRO_Y1.q());
    /*p29.FUKY*/ wire _FUKY_Y2n = not(top.lcd_reg.LEXA_Y2.q());
    /*p29.FUVE*/ wire _FUVE_Y3n = not(top.lcd_reg.LYDO_Y3.q());
    /*p29.FEPU*/ wire _FEPU_Y4n = not(top.lcd_reg.LOVU_Y4.q());
    /*p29.FOFA*/ wire _FOFA_Y5n = not(top.lcd_reg.LEMA_Y5.q());
    /*p29.FEMO*/ wire _FEMO_Y6n = not(top.lcd_reg.MATO_Y6.q());
    /*p29.GUSU*/ wire _GUSU_Y7n = not(top.lcd_reg.LAFO_Y7.q());

    /*p29.ERUC*/ _ERUC_YDIFF_S0 = add_c(_EBOS_Y0n, top.oam_bus.XUSO_OAM_DB0.q(), GND);
    /*p29.ERUC*/ _ERUC_YDIFF_C0 = add_s(_EBOS_Y0n, top.oam_bus.XUSO_OAM_DB0.q(), GND);
    /*p29.ENEF*/ _ENEF_YDIFF_S1 = add_s(_DASA_Y1n, top.oam_bus.XEGU_OAM_DB1.q(), _ERUC_YDIFF_C0);
    /*p29.ENEF*/ _ENEF_YDIFF_C1 = add_c(_DASA_Y1n, top.oam_bus.XEGU_OAM_DB1.q(), _ERUC_YDIFF_C0);
    /*p29.FECO*/ _FECO_YDIFF_S2 = add_s(_FUKY_Y2n, top.oam_bus.YJEX_OAM_DB2.q(), _ENEF_YDIFF_C1);
    /*p29.FECO*/ _FECO_YDIFF_C2 = add_c(_FUKY_Y2n, top.oam_bus.YJEX_OAM_DB2.q(), _ENEF_YDIFF_C1);
    /*p29.GYKY*/ _GYKY_YDIFF_S3 = add_s(_FUVE_Y3n, top.oam_bus.XYJU_OAM_DB3.q(), _FECO_YDIFF_C2);
    /*p29.GYKY*/ _GYKY_YDIFF_C3 = add_c(_FUVE_Y3n, top.oam_bus.XYJU_OAM_DB3.q(), _FECO_YDIFF_C2);
    /*p29.GOPU*/ _GOPU_YDIFF_S4 = add_s(_FEPU_Y4n, top.oam_bus.YBOG_OAM_DB4.q(), _GYKY_YDIFF_C3);
    /*p29.GOPU*/ _GOPU_YDIFF_C4 = add_c(_FEPU_Y4n, top.oam_bus.YBOG_OAM_DB4.q(), _GYKY_YDIFF_C3);
    /*p29.FUWA*/ _FUWA_YDIFF_S5 = add_s(_FOFA_Y5n, top.oam_bus.WYSO_OAM_DB5.q(), _GOPU_YDIFF_C4);
    /*p29.FUWA*/ _FUWA_YDIFF_C5 = add_c(_FOFA_Y5n, top.oam_bus.WYSO_OAM_DB5.q(), _GOPU_YDIFF_C4);
    /*p29.GOJU*/ _GOJU_YDIFF_S6 = add_s(_FEMO_Y6n, top.oam_bus.XOTE_OAM_DB6.q(), _FUWA_YDIFF_C5);
    /*p29.GOJU*/ _GOJU_YDIFF_C6 = add_c(_FEMO_Y6n, top.oam_bus.XOTE_OAM_DB6.q(), _FUWA_YDIFF_C5);
    /*p29.WUHU*/ _WUHU_YDIFF_S7 = add_s(_GUSU_Y7n, top.oam_bus.YZAB_OAM_DB7.q(), _GOJU_YDIFF_C6);
    /*p29.WUHU*/ _WUHU_YDIFF_C7 = add_c(_GUSU_Y7n, top.oam_bus.YZAB_OAM_DB7.q(), _GOJU_YDIFF_C6);
  }

  {
    /*p29.GACE*/ wire _GACE_SPRITE_DELTA4 = not(_GOPU_YDIFF_S4);
    /*p29.GUVU*/ wire _GUVU_SPRITE_DELTA5 = not(_FUWA_YDIFF_S5);
    /*p29.GYDA*/ wire _GYDA_SPRITE_DELTA6 = not(_GOJU_YDIFF_S6);
    /*p29.GEWY*/ wire _GEWY_SPRITE_DELTA7 = not(_WUHU_YDIFF_S7);

    /*p29.GOVU*/ wire _GOVU_SPSIZE_MATCH = or (_GYKY_YDIFF_S3, top.pix_pipe.XYMO_LCDC_SPSIZE.q());
    /*p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn = nand(_GACE_SPRITE_DELTA4, _GUVU_SPRITE_DELTA5, _GYDA_SPRITE_DELTA6, _GEWY_SPRITE_DELTA7, _WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH);
    /*p29.GESE*/ wire _GESE_SCAN_MATCH_Y = not(_WOTA_SCAN_MATCH_Yn);
    /*p29.CARE*/ _CARE_STORE_ENp_ABxxEFxx = and (top.clk_reg.XOCE_AxxDExxH(), top.sprite_scanner.CEHA_SCANNINGp(), _GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.
  }
}

//------------------------------------------------------------------------------

void SpriteScanner::tock(const SchematicTop& top) {

  /*p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and (_YFEL_SCAN0.q(), _WEWY_SCAN1.q(), _GOSO_SCAN2.q(), _FONY_SCAN5.q()); // die AND. 32 + 4 + 2 + 1 = 39

  //----------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  {
    /*p29.BYBA*/ _BYBA_SCAN_DONE_A = dff17(top.clk_reg.XUPY_ABxxEFxx(), _BAGY_LINE_RSTn, _FETO_SCAN_DONE_d0);
    /*p29.DOBA*/ _DOBA_SCAN_DONE_B = dff17(top.clk_reg.ALET_xBxDxFxH(), _BAGY_LINE_RSTn, _BYBA_SCAN_DONE_A.q());

    /*p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or(_BALU_LINE_RSTp, DOBA_SCAN_DONE_B(), !BYBA_SCAN_DONE_A());
    /*p29.AVAP*/ wire AVAP_RENDER_START_TRIGp = not(_BEBU_SCAN_DONE_TRIGn);

    /*p28.ASEN*/ wire _ASEN_SCAN_DONE_PE = or (top.clk_reg.ATAR_VID_RSTp(), AVAP_RENDER_START_TRIGp);
    /*p28.BESU*/ _BESU_SCANNINGp = nor_latch(top.lcd_reg.CATU_VID_LINE_ENDp(), _ASEN_SCAN_DONE_PE);
    /*p29.CENO*/ _CENO_SCANNINGp = dff17(top.clk_reg.XUPY_ABxxEFxx(), top.clk_reg.ABEZ_VID_RSTn(), _BESU_SCANNINGp);
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or(_FETO_SCAN_DONE_d0,   top.clk_reg.XUPY_ABxxEFxx());
    /*p28.ANOM*/ wire ANOM_LINE_RSTn = nor(top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.ATAR_VID_RSTp());

    /*p28.YFEL*/ _YFEL_SCAN0 = dff17(_GAVA_SCAN_CLK,   ANOM_LINE_RSTn, _YFEL_SCAN0.qn());
    /*p28.WEWY*/ _WEWY_SCAN1 = dff17(_YFEL_SCAN0.qn(), ANOM_LINE_RSTn, _WEWY_SCAN1.qn());
    /*p28.GOSO*/ _GOSO_SCAN2 = dff17(_WEWY_SCAN1.qn(), ANOM_LINE_RSTn, _GOSO_SCAN2.qn());
    /*p28.ELYN*/ _ELYN_SCAN3 = dff17(_GOSO_SCAN2.qn(), ANOM_LINE_RSTn, _ELYN_SCAN3.qn());
    /*p28.FAHA*/ _FAHA_SCAN4 = dff17(_ELYN_SCAN3.qn(), ANOM_LINE_RSTn, _FAHA_SCAN4.qn());
    /*p28.FONY*/ _FONY_SCAN5 = dff17(_FAHA_SCAN4.qn(), ANOM_LINE_RSTn, _FONY_SCAN5.qn());
  }
}

//------------------------------------------------------------------------------
