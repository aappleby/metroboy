#include "Sch_SpriteScanner.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void SpriteScanner::tick(const SchematicTop& top) {
  wire GND = 0;

  {
    _XYLO_LCDC_SPEN = top.pix_pipe.XYLO_LCDC_SPEN.q();
    _XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGp();

    /*p29.BALU*/ _BALU_LINE_RSTp = not(top.lcd_reg.ANOM_LINE_RSTn());
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
    /*p29.BYBA*/ _BYBA_SCAN_DONE_A = ff17_r2(top.clk_reg.XUPY_ABxxEFxx(), _BAGY_LINE_RSTn, _FETO_SCAN_DONE_d0);
    /*p29.DOBA*/ _DOBA_SCAN_DONE_B = ff17_r2(top.clk_reg.ALET_xBxDxFxH(), _BAGY_LINE_RSTn, _BYBA_SCAN_DONE_A.q());

    /*p28.ASEN*/ wire _ASEN_SCAN_DONE_PE = or (top.clk_reg.ATAR_VID_RSTp(), top.sprite_scanner.AVAP_RENDER_START_TRIGp());
    /*p28.BESU*/ _BESU_SCANNINGp = nor_latch_r2(top.lcd_reg.CATU_VID_LINE_ENDp(), _ASEN_SCAN_DONE_PE);
    /*p29.CENO*/ _CENO_SCANNINGp = ff17_r2(top.clk_reg.XUPY_ABxxEFxx(), top.clk_reg.ABEZ_VID_RSTn(), _BESU_SCANNINGp.q());
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or(_FETO_SCAN_DONE_d0,   top.clk_reg.XUPY_ABxxEFxx());
    /*p28.YFEL*/ _YFEL_SCAN0 = ff17_r2(_GAVA_SCAN_CLK,   top.lcd_reg.ANOM_LINE_RSTn(), _YFEL_SCAN0.qn());
    /*p28.WEWY*/ _WEWY_SCAN1 = ff17_r2(_YFEL_SCAN0.qn(), top.lcd_reg.ANOM_LINE_RSTn(), _WEWY_SCAN1.qn());
    /*p28.GOSO*/ _GOSO_SCAN2 = ff17_r2(_WEWY_SCAN1.qn(), top.lcd_reg.ANOM_LINE_RSTn(), _GOSO_SCAN2.qn());
    /*p28.ELYN*/ _ELYN_SCAN3 = ff17_r2(_GOSO_SCAN2.qn(), top.lcd_reg.ANOM_LINE_RSTn(), _ELYN_SCAN3.qn());
    /*p28.FAHA*/ _FAHA_SCAN4 = ff17_r2(_ELYN_SCAN3.qn(), top.lcd_reg.ANOM_LINE_RSTn(), _FAHA_SCAN4.qn());
    /*p28.FONY*/ _FONY_SCAN5 = ff17_r2(_FAHA_SCAN4.qn(), top.lcd_reg.ANOM_LINE_RSTn(), _FONY_SCAN5.qn());
  }
}

//------------------------------------------------------------------------------

uint64_t SpriteScanner::commit() {
  uint64_t ret = commit_and_hash((uint8_t*)this, sizeof(*this));
  return {ret};
}

//------------------------------------------------------------------------------
