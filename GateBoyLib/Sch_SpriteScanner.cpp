#include "GateBoyLib/Sch_SpriteScanner.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void SpriteScanner::dump(Dumper& d, const SchematicTop& top) const {
  d("\002===== SpriteScan =====\001\n");

  d("SCAN INDEX       %02d\n", pack_p(YFEL_SCAN0.qp17(), WEWY_SCAN1.qp17(), GOSO_SCAN2.qp17(), ELYN_SCAN3.qp17(),
                                     FAHA_SCAN4.qp17(), FONY_SCAN5.qp17(), 0, 0));
  d("SPRITE INDEX     %02d\n", pack_p(XADU_SPRITE_IDX0p.qn12(), XEDY_SPRITE_IDX1p.qn12(), ZUZE_SPRITE_IDX2p.qn12(), XOBE_SPRITE_IDX3p.qn12(),
                                      YDUF_SPRITE_IDX4p.qn12(), XECU_SPRITE_IDX5p.qn12(), 0, 0));

  d("BESU_SCANNINGp   %c\n", BESU_SCANNINGp  .c());
  d("CENO_SCANNINGp   %c\n", CENO_SCANNINGp  .c());
  d("BYBA_SCAN_DONE_A %c\n", BYBA_SCAN_DONE_A.c());
  d("DOBA_SCAN_DONE_B %c\n", DOBA_SCAN_DONE_B.c());
  d("\n");
  d("LCD Y      %03d\n", top.lcd_reg.get_ly());
  d("Y DIFF     %03d\n", pack_p(DEGE_SPRITE_DELTA0, DABY_SPRITE_DELTA1, DABU_SPRITE_DELTA2, GYSA_SPRITE_DELTA3,
                                GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7));
  d("CARE_STORE_ENp_ABxxEFxx %d\n", (wire)CARE_STORE_ENp_ABxxEFxx);

  d("\n");
}

//------------------------------------------------------------------------------

void SpriteScanner::tick(const SchematicTop& top) {
  wire GND = 0;

  // this is using an adder as a subtracter by inverting the first input.

  /*#p29.EBOS*/ wire _EBOS_Y0n = not1(top.lcd_reg.MUWY_Y0p.qp17());
  /* p29.DASA*/ wire _DASA_Y1n = not1(top.lcd_reg.MYRO_Y1p.qp17());
  /* p29.FUKY*/ wire _FUKY_Y2n = not1(top.lcd_reg.LEXA_Y2p.qp17());
  /* p29.FUVE*/ wire _FUVE_Y3n = not1(top.lcd_reg.LYDO_Y3p.qp17());
  /* p29.FEPU*/ wire _FEPU_Y4n = not1(top.lcd_reg.LOVU_Y4p.qp17());
  /* p29.FOFA*/ wire _FOFA_Y5n = not1(top.lcd_reg.LEMA_Y5p.qp17());
  /* p29.FEMO*/ wire _FEMO_Y6n = not1(top.lcd_reg.MATO_Y6p.qp17());
  /* p29.GUSU*/ wire _GUSU_Y7n = not1(top.lcd_reg.LAFO_Y7p.qp17());

  /*p29.ERUC*/ wire _ERUC_YDIFF_S0 = add_s(_EBOS_Y0n, top.oam_bus.XUSO_OAM_DA0p.qp08(), GND);
  /*p29.ERUC*/ wire _ERUC_YDIFF_C0 = add_c(_EBOS_Y0n, top.oam_bus.XUSO_OAM_DA0p.qp08(), GND);
  /*p29.ENEF*/ wire _ENEF_YDIFF_S1 = add_s(_DASA_Y1n, top.oam_bus.XEGU_OAM_DA1p.qp08(), _ERUC_YDIFF_C0);
  /*p29.ENEF*/ wire _ENEF_YDIFF_C1 = add_c(_DASA_Y1n, top.oam_bus.XEGU_OAM_DA1p.qp08(), _ERUC_YDIFF_C0);
  /*p29.FECO*/ wire _FECO_YDIFF_S2 = add_s(_FUKY_Y2n, top.oam_bus.YJEX_OAM_DA2p.qp08(), _ENEF_YDIFF_C1);
  /*p29.FECO*/ wire _FECO_YDIFF_C2 = add_c(_FUKY_Y2n, top.oam_bus.YJEX_OAM_DA2p.qp08(), _ENEF_YDIFF_C1);
  /*p29.GYKY*/ wire _GYKY_YDIFF_S3 = add_s(_FUVE_Y3n, top.oam_bus.XYJU_OAM_DA3p.qp08(), _FECO_YDIFF_C2);
  /*p29.GYKY*/ wire _GYKY_YDIFF_C3 = add_c(_FUVE_Y3n, top.oam_bus.XYJU_OAM_DA3p.qp08(), _FECO_YDIFF_C2);
  /*p29.GOPU*/ wire _GOPU_YDIFF_S4 = add_s(_FEPU_Y4n, top.oam_bus.YBOG_OAM_DA4p.qp08(), _GYKY_YDIFF_C3);
  /*p29.GOPU*/ wire _GOPU_YDIFF_C4 = add_c(_FEPU_Y4n, top.oam_bus.YBOG_OAM_DA4p.qp08(), _GYKY_YDIFF_C3);
  /*p29.FUWA*/ wire _FUWA_YDIFF_S5 = add_s(_FOFA_Y5n, top.oam_bus.WYSO_OAM_DA5p.qp08(), _GOPU_YDIFF_C4);
  /*p29.FUWA*/ wire _FUWA_YDIFF_C5 = add_c(_FOFA_Y5n, top.oam_bus.WYSO_OAM_DA5p.qp08(), _GOPU_YDIFF_C4);
  /*p29.GOJU*/ wire _GOJU_YDIFF_S6 = add_s(_FEMO_Y6n, top.oam_bus.XOTE_OAM_DA6p.qp08(), _FUWA_YDIFF_C5);
  /*p29.GOJU*/ wire _GOJU_YDIFF_C6 = add_c(_FEMO_Y6n, top.oam_bus.XOTE_OAM_DA6p.qp08(), _FUWA_YDIFF_C5);
  /*p29.WUHU*/ wire _WUHU_YDIFF_S7 = add_s(_GUSU_Y7n, top.oam_bus.YZAB_OAM_DA7p.qp08(), _GOJU_YDIFF_C6);
  /*p29.WUHU*/ wire _WUHU_YDIFF_C7 = add_c(_GUSU_Y7n, top.oam_bus.YZAB_OAM_DA7p.qp08(), _GOJU_YDIFF_C6);

  /* p29.DEGE*/ DEGE_SPRITE_DELTA0 = not1(_ERUC_YDIFF_S0);
  /* p29.DABY*/ DABY_SPRITE_DELTA1 = not1(_ENEF_YDIFF_S1);
  /* p29.DABU*/ DABU_SPRITE_DELTA2 = not1(_FECO_YDIFF_S2);
  /* p29.GYSA*/ GYSA_SPRITE_DELTA3 = not1(_GYKY_YDIFF_S3);
  /* p29.GACE*/ GACE_SPRITE_DELTA4 = not1(_GOPU_YDIFF_S4);
  /* p29.GUVU*/ GUVU_SPRITE_DELTA5 = not1(_FUWA_YDIFF_S5);
  /* p29.GYDA*/ GYDA_SPRITE_DELTA6 = not1(_GOJU_YDIFF_S6);
  /* p29.GEWY*/ GEWY_SPRITE_DELTA7 = not1(_WUHU_YDIFF_S7);

  /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH  = or2(top.pix_pipe.XYMO_LCDC_SPSIZEn.qn08(), _GYKY_YDIFF_S3);
  /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, _WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH);
  /* p29.GESE*/ wire _GESE_SCAN_MATCH_Y  = not1(_WOTA_SCAN_MATCH_Yn);
  /* p29.CEHA*/ CEHA_SCANNINGp     = not1(CENO_SCANNINGp.qn16());

  /* p29.CARE*/ CARE_STORE_ENp_ABxxEFxx = and3(top.clk_reg.XOCE_xBCxxFGx, CEHA_SCANNINGp, _GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.

  /*#p28.ACYL*/ ACYL_SCANNINGp = and2(top.dma_reg.BOGE_DMA_RUNNINGn, BESU_SCANNINGp.qp04());

  /*#p28.ANOM*/ ANOM_LINE_RSTn = nor2(top.lcd_reg.ATEJ_LINE_TRIGp, top.clk_reg.ATAR_VID_RSTp);
  /*#p29.BALU*/ wire _BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
  /*#p29.BAGY*/ BAGY_LINE_RSTn = not1(_BALU_LINE_RSTp);

  /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_B.qp17(), _BALU_LINE_RSTp, BYBA_SCAN_DONE_A.qn16());
  /*#p29.AVAP*/ AVAP_RENDER_START_TRIGp = not1(_BEBU_SCAN_DONE_TRIGn);
}

//------------------------------------------------------------------------------

void SpriteScanner::tock(const SchematicTop& top) {
  wire WEFE_VCC = 1;

  //----------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  // 32 + 4 + 2 + 1 = 39
  /*#p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and4(YFEL_SCAN0.qp17(), WEWY_SCAN1.qp17(), GOSO_SCAN2.qp17(), FONY_SCAN5.qp17());

  {
    /*#p29.BYBA*/ BYBA_SCAN_DONE_A.dff17(top.clk_reg.XUPY_ABxxEFxx, BAGY_LINE_RSTn, _FETO_SCAN_DONE_d0);
    /*#p29.DOBA*/ DOBA_SCAN_DONE_B.dff17(top.clk_reg.ALET_xBxDxFxH, BAGY_LINE_RSTn, BYBA_SCAN_DONE_A.qp17());

    /*#p28.ASEN*/ wire _ASEN_SCAN_DONE_PE = or2(top.clk_reg.ATAR_VID_RSTp, AVAP_RENDER_START_TRIGp);
    /*#p28.BESU*/ BESU_SCANNINGp.nor_latch(top.lcd_reg.CATU_LINE_P000.qp17(), _ASEN_SCAN_DONE_PE);
    /*#p29.CENO*/ CENO_SCANNINGp.dff17(top.clk_reg.XUPY_ABxxEFxx, top.clk_reg.ABEZ_VID_RSTn, BESU_SCANNINGp.qp04());
  }

  {
    /*#p30.CYKE*/ wire _CYKE_ABxxEFxx = not1(top.clk_reg.XUPY_ABxxEFxx);
    /*#p30.WUDA*/ wire _WUDA_xxCDxxGH = not1(_CYKE_ABxxEFxx);

    /* p28.YFOT*/ wire _YFOT_OAM_A2p = not1(top.oam_bus.BUS_OAM_A2n.qp());
    /* p28.YFOC*/ wire _YFOC_OAM_A3p = not1(top.oam_bus.BUS_OAM_A3n.qp());
    /* p28.YVOM*/ wire _YVOM_OAM_A4p = not1(top.oam_bus.BUS_OAM_A4n.qp());
    /* p28.YMEV*/ wire _YMEV_OAM_A5p = not1(top.oam_bus.BUS_OAM_A5n.qp());
    /* p28.XEMU*/ wire _XEMU_OAM_A6p = not1(top.oam_bus.BUS_OAM_A6n.qp());
    /* p28.YZET*/ wire _YZET_OAM_A7p = not1(top.oam_bus.BUS_OAM_A7n.qp());

    /*p30.XADU*/ XADU_SPRITE_IDX0p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YFOT_OAM_A2p);
    /*p30.XEDY*/ XEDY_SPRITE_IDX1p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YFOC_OAM_A3p);
    /*p30.ZUZE*/ ZUZE_SPRITE_IDX2p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YVOM_OAM_A4p);
    /*p30.XOBE*/ XOBE_SPRITE_IDX3p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YMEV_OAM_A5p);
    /*p30.YDUF*/ YDUF_SPRITE_IDX4p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _XEMU_OAM_A6p);
    /*p30.XECU*/ XECU_SPRITE_IDX5p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YZET_OAM_A7p);
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or2(_FETO_SCAN_DONE_d0,   top.clk_reg.XUPY_ABxxEFxx);

    /*p28.YFEL*/ YFEL_SCAN0.dff17(_GAVA_SCAN_CLK,    ANOM_LINE_RSTn, YFEL_SCAN0.qn16());
    /*p28.WEWY*/ WEWY_SCAN1.dff17(YFEL_SCAN0.qn16(), ANOM_LINE_RSTn, WEWY_SCAN1.qn16());
    /*p28.GOSO*/ GOSO_SCAN2.dff17(WEWY_SCAN1.qn16(), ANOM_LINE_RSTn, GOSO_SCAN2.qn16());
    /*p28.ELYN*/ ELYN_SCAN3.dff17(GOSO_SCAN2.qn16(), ANOM_LINE_RSTn, ELYN_SCAN3.qn16());
    /*p28.FAHA*/ FAHA_SCAN4.dff17(ELYN_SCAN3.qn16(), ANOM_LINE_RSTn, FAHA_SCAN4.qn16());
    /*p28.FONY*/ FONY_SCAN5.dff17(FAHA_SCAN4.qn16(), ANOM_LINE_RSTn, FONY_SCAN5.qn16());
  }
}

//------------------------------------------------------------------------------
