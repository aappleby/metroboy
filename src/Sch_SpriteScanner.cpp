#include "Sch_SpriteScanner.h"
#include "Sch_Top.h"

using namespace Schematics;

void SpriteScanner::dump(Dumper& d, const SchematicTop& top) const {
  d("----------SpriteScan ---------\n");

  /*p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or3(BALU_LINE_RSTp, DOBA_SCAN_DONE_B.qp(), BYBA_SCAN_DONE_A.qn());
  /*p29.AVAP*/ wire AVAP_RENDER_START_TRIGp = not1(_BEBU_SCAN_DONE_TRIGn);
  /*p28.ASEN*/ wire _ASEN_SCAN_DONE_PE = or2(top.clk_reg.ATAR_VID_RSTp(), AVAP_RENDER_START_TRIGp);

  d("BALU_LINE_RSTp         %d\n", BALU_LINE_RSTp.as_wire());
  d("BEBU_SCAN_DONE_TRIGn   %d\n", _BEBU_SCAN_DONE_TRIGn);
  d("AVAP_RENDER_START_TRIGp %d\n", AVAP_RENDER_START_TRIGp);
  d("ASEN_SCAN_DONE_PE      %d\n", _ASEN_SCAN_DONE_PE);

  d("BESU_SCANNINGp   %c\n", BESU_SCANNINGp  .c());
  d("CENO_SCANNINGp   %c\n", CENO_SCANNINGp  .c());
  d("BYBA_SCAN_DONE_A %c\n", BYBA_SCAN_DONE_A.c());
  d("DOBA_SCAN_DONE_B %c\n", DOBA_SCAN_DONE_B.c());

  /*p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and4(_YFEL_SCAN0.qp(), _WEWY_SCAN1.qp(), _GOSO_SCAN2.qp(), _FONY_SCAN5.qp()); // die AND. 32 + 4 + 2 + 1 = 39
  /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or2(_FETO_SCAN_DONE_d0,   top.clk_reg.XUPY_xxCDxxGH());
  /*p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp(), top.clk_reg.ATAR_VID_RSTp());
  d("_GAVA_SCAN_CLK %d\n", _GAVA_SCAN_CLK);
  d("ANOM_LINE_RSTn %d\n", ANOM_LINE_RSTn);
  d("SCAN INDEX        %02d\n", 
    pack_p(
      _YFEL_SCAN0.qp(),
      _WEWY_SCAN1.qp(),
      _GOSO_SCAN2.qp(),
      _ELYN_SCAN3.qp(),
      _FAHA_SCAN4.qp(),
      _FONY_SCAN5.qp(),
      0,
      0
    )
  );

  int y_diff = pack_p(_ERUC_YDIFF_S0, _ENEF_YDIFF_S1, _FECO_YDIFF_S2, _GYKY_YDIFF_S3,
                      _GOPU_YDIFF_S4, _FUWA_YDIFF_S5, _GOJU_YDIFF_S6, _WUHU_YDIFF_S7);

  uint8_t lcd_y  = top.lcd_reg.get_y();

  d("LCD Y    %03d\n", lcd_y);
  d("Y DIFF   %03d\n", y_diff);

  d("_GACE_SPRITE_DELTA4      %d\n", (wire)_GACE_SPRITE_DELTA4);
  d("_GUVU_SPRITE_DELTA5      %d\n", (wire)_GUVU_SPRITE_DELTA5);
  d("_GYDA_SPRITE_DELTA6      %d\n", (wire)_GYDA_SPRITE_DELTA6);
  d("_GEWY_SPRITE_DELTA7      %d\n", (wire)_GEWY_SPRITE_DELTA7);
  d("_GOVU_SPSIZE_MATCH       %d\n", (wire)_GOVU_SPSIZE_MATCH );
  d("_WOTA_SCAN_MATCH_Yn      %d\n", (wire)_WOTA_SCAN_MATCH_Yn);
  d("_GESE_SCAN_MATCH_Y       %d\n", (wire)_GESE_SCAN_MATCH_Y );
  d("_CARE_STORE_ENp_ABxxEFxx %d\n", (wire)_CARE_STORE_ENp_ABxxEFxx);
  d("\n");
}


//------------------------------------------------------------------------------

void SpriteScanner::tick(const SchematicTop& top) {
  wire GND = 0;

  {
    _XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGp();

    // ANOM := nor(ATEJ, ATAR);
    // BALU := not(ANOM);
    // BAGY := not(BALU);

    /*p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp(), top.clk_reg.ATAR_VID_RSTp());
    /*p29.BALU*/ BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
    /*p29.BAGY*/ BAGY_LINE_RSTn = not1(BALU_LINE_RSTp);
  }

  {
    /*p29.EBOS*/ wire _EBOS_Y0n = not1(top.lcd_reg.MUWY_Y0p.qp());
    /*p29.DASA*/ wire _DASA_Y1n = not1(top.lcd_reg.MYRO_Y1p.qp());
    /*p29.FUKY*/ wire _FUKY_Y2n = not1(top.lcd_reg.LEXA_Y2p.qp());
    /*p29.FUVE*/ wire _FUVE_Y3n = not1(top.lcd_reg.LYDO_Y3p.qp());
    /*p29.FEPU*/ wire _FEPU_Y4n = not1(top.lcd_reg.LOVU_Y4p.qp());
    /*p29.FOFA*/ wire _FOFA_Y5n = not1(top.lcd_reg.LEMA_Y5p.qp());
    /*p29.FEMO*/ wire _FEMO_Y6n = not1(top.lcd_reg.MATO_Y6p.qp());
    /*p29.GUSU*/ wire _GUSU_Y7n = not1(top.lcd_reg.LAFO_Y7p.qp());

    // so these adders operate on inverted inputs?

    /*p29.ERUC*/ _ERUC_YDIFF_S0 = add_s(_EBOS_Y0n, top.oam_bus.XUSO_OAM_DA0n.qp(), GND);
    /*p29.ERUC*/ _ERUC_YDIFF_C0 = add_c(_EBOS_Y0n, top.oam_bus.XUSO_OAM_DA0n.qp(), GND);
    /*p29.ENEF*/ _ENEF_YDIFF_S1 = add_s(_DASA_Y1n, top.oam_bus.XEGU_OAM_DA1n.qp(), _ERUC_YDIFF_C0);
    /*p29.ENEF*/ _ENEF_YDIFF_C1 = add_c(_DASA_Y1n, top.oam_bus.XEGU_OAM_DA1n.qp(), _ERUC_YDIFF_C0);
    /*p29.FECO*/ _FECO_YDIFF_S2 = add_s(_FUKY_Y2n, top.oam_bus.YJEX_OAM_DA2n.qp(), _ENEF_YDIFF_C1);
    /*p29.FECO*/ _FECO_YDIFF_C2 = add_c(_FUKY_Y2n, top.oam_bus.YJEX_OAM_DA2n.qp(), _ENEF_YDIFF_C1);
    /*p29.GYKY*/ _GYKY_YDIFF_S3 = add_s(_FUVE_Y3n, top.oam_bus.XYJU_OAM_DA3n.qp(), _FECO_YDIFF_C2);
    /*p29.GYKY*/ _GYKY_YDIFF_C3 = add_c(_FUVE_Y3n, top.oam_bus.XYJU_OAM_DA3n.qp(), _FECO_YDIFF_C2);
    /*p29.GOPU*/ _GOPU_YDIFF_S4 = add_s(_FEPU_Y4n, top.oam_bus.YBOG_OAM_DA4n.qp(), _GYKY_YDIFF_C3);
    /*p29.GOPU*/ _GOPU_YDIFF_C4 = add_c(_FEPU_Y4n, top.oam_bus.YBOG_OAM_DA4n.qp(), _GYKY_YDIFF_C3);
    /*p29.FUWA*/ _FUWA_YDIFF_S5 = add_s(_FOFA_Y5n, top.oam_bus.WYSO_OAM_DA5n.qp(), _GOPU_YDIFF_C4);
    /*p29.FUWA*/ _FUWA_YDIFF_C5 = add_c(_FOFA_Y5n, top.oam_bus.WYSO_OAM_DA5n.qp(), _GOPU_YDIFF_C4);
    /*p29.GOJU*/ _GOJU_YDIFF_S6 = add_s(_FEMO_Y6n, top.oam_bus.XOTE_OAM_DA6n.qp(), _FUWA_YDIFF_C5);
    /*p29.GOJU*/ _GOJU_YDIFF_C6 = add_c(_FEMO_Y6n, top.oam_bus.XOTE_OAM_DA6n.qp(), _FUWA_YDIFF_C5);
    /*p29.WUHU*/ _WUHU_YDIFF_S7 = add_s(_GUSU_Y7n, top.oam_bus.YZAB_OAM_DA7n.qp(), _GOJU_YDIFF_C6);
    /*p29.WUHU*/ _WUHU_YDIFF_C7 = add_c(_GUSU_Y7n, top.oam_bus.YZAB_OAM_DA7n.qp(), _GOJU_YDIFF_C6);
  }

  {
    /*p29.GACE*/ _GACE_SPRITE_DELTA4 = not1(_GOPU_YDIFF_S4);
    /*p29.GUVU*/ _GUVU_SPRITE_DELTA5 = not1(_FUWA_YDIFF_S5);
    /*p29.GYDA*/ _GYDA_SPRITE_DELTA6 = not1(_GOJU_YDIFF_S6);
    /*p29.GEWY*/ _GEWY_SPRITE_DELTA7 = not1(_WUHU_YDIFF_S7);

    /*p29.GOVU*/ _GOVU_SPSIZE_MATCH = or2(_GYKY_YDIFF_S3, top.pix_pipe.XYMO_LCDC_SPSIZE.qp());
    /*p29.WOTA*/ _WOTA_SCAN_MATCH_Yn = nand6(_GACE_SPRITE_DELTA4, _GUVU_SPRITE_DELTA5, _GYDA_SPRITE_DELTA6, _GEWY_SPRITE_DELTA7, _WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH);
    /*p29.GESE*/ _GESE_SCAN_MATCH_Y = not1(_WOTA_SCAN_MATCH_Yn);
    /*p29.CARE*/ _CARE_STORE_ENp_ABxxEFxx = and3(top.clk_reg.XOCE_AxxDExxH(), top.sprite_scanner.CEHA_SCANNINGp(), _GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.
  }
}

//------------------------------------------------------------------------------

void SpriteScanner::tock(const SchematicTop& top) {

  /*p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and4(_YFEL_SCAN0.qp(), _WEWY_SCAN1.qp(), _GOSO_SCAN2.qp(), _FONY_SCAN5.qp()); // die AND. 32 + 4 + 2 + 1 = 39

  //----------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  {
    // LGTM

    // BYBA_01 == BYBA_12
    // BYBA_02 << XUPY_03 : CLKp
    // BYBA_03 == BYBA_09
    // BYBA_04 NC
    // BYBA_05 NC
    // BYBA_06 << BAGY_02 : RSTn
    // BYBA_07 << FETO_06 : D
    // BYBA_08 NC
    // BYBA_09 == BYBA_03
    // BYBA_10 NC
    // BYBA_11 NC
    // BYBA_12 == BYBA_01
    // BYBA_13 << BAGY_02 : RSTn
    // BYBA_14 NC
    // BYBA_15 NC
    // BYBA_16 >> BEBU_03 : QN
    // BYBA_17 >> DOBA_07 : Q

    // DOBA_01 == DOBA_12 : SC
    // DOBA_02 << ALET_03 : CLKp
    // DOBA_03 == DOBA_09 : SC
    // DOBA_04 NC
    // DOBA_05 NC
    // DOBA_06 << BAGY_02 : RSTn
    // DOBA_07 << BYBA_17 : D
    // DOBA_08 NC
    // DOBA_09 == DOBA_03 : SC
    // DOBA_10 NC
    // DOBA_11 NC
    // DOBA_12 == DOBA_01 : SC
    // DOBA_13 << BAGY_02 : RSTn
    // DOBA_14 NC
    // DOBA_15 NC
    // DOBA_16 >> NC      : QN
    // DOBA_17 >> BEBU_01 : Q

    // BEBU := or3(DOBA_Q, BALU, BYBA_QN);
    // AVAP := not(BEBU);
    // ASEN := or(ATAR, AVAP)
    // BESU := latch(CATU_Q, ASEN)
    
    // CENO_01 == CENO_12 : SC
    // CENO_02 << XUPY_03 : CLKp
    // CENO_03 == CENO_09 : SC
    // CENO_04 NC
    // CENO_05 NC
    // CENO_06 << ABEZ_02 : RSTn
    // CENO_07 << BESU_04 : D
    // CENO_08 NC
    // CENO_09 == CENO_03 : SC
    // CENO_10 NC
    // CENO_11 NC
    // CENO_12 == CENO_01 : SC
    // CENO_13 << ABEZ_02 : RSTn
    // CENO_14 NC
    // CENO_15 NC
    // CENO_16 >> CEHA_01 : QN
    // CENO_17 >> NC      : Q

    /*p29.BYBA*/ BYBA_SCAN_DONE_A = dff17_AB(top.clk_reg.XUPY_xxCDxxGH(), BAGY_LINE_RSTn, _FETO_SCAN_DONE_d0);
    /*p29.DOBA*/ DOBA_SCAN_DONE_B = dff17_B (top.clk_reg.ALET_xBxDxFxH(), BAGY_LINE_RSTn, BYBA_SCAN_DONE_A.qp());

    /*p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_B.qp(), BALU_LINE_RSTp, BYBA_SCAN_DONE_A.qn());
    /*p29.AVAP*/ wire AVAP_RENDER_START_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);

    /*p28.ASEN*/ wire ASEN_SCAN_DONE_PE = or2(top.clk_reg.ATAR_VID_RSTp(), AVAP_RENDER_START_TRIGp);
    /*p28.BESU*/ BESU_SCANNINGp = nor_latch(top.lcd_reg._CATU_LINE_P910.qp(), ASEN_SCAN_DONE_PE);
    /*p29.CENO*/ CENO_SCANNINGp = dff17_A(top.clk_reg.XUPY_xxCDxxGH(), top.clk_reg.ABEZ_VID_RSTn(), BESU_SCANNINGp.qp());
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or2(_FETO_SCAN_DONE_d0,   top.clk_reg.XUPY_xxCDxxGH());
    /*p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(top.lcd_reg.ATEJ_VID_LINE_END_TRIGp(), top.clk_reg.ATAR_VID_RSTp());

    /*p28.YFEL*/ _YFEL_SCAN0 = dff17_AB(_GAVA_SCAN_CLK,   ANOM_LINE_RSTn, _YFEL_SCAN0.qn());
    /*p28.WEWY*/ _WEWY_SCAN1 = dff17_AB(_YFEL_SCAN0.qn(), ANOM_LINE_RSTn, _WEWY_SCAN1.qn());
    /*p28.GOSO*/ _GOSO_SCAN2 = dff17_AB(_WEWY_SCAN1.qn(), ANOM_LINE_RSTn, _GOSO_SCAN2.qn());
    /*p28.ELYN*/ _ELYN_SCAN3 = dff17_AB(_GOSO_SCAN2.qn(), ANOM_LINE_RSTn, _ELYN_SCAN3.qn());
    /*p28.FAHA*/ _FAHA_SCAN4 = dff17_AB(_ELYN_SCAN3.qn(), ANOM_LINE_RSTn, _FAHA_SCAN4.qn());
    /*p28.FONY*/ _FONY_SCAN5 = dff17_AB(_FAHA_SCAN4.qn(), ANOM_LINE_RSTn, _FONY_SCAN5.qn());
  }
}

//------------------------------------------------------------------------------
