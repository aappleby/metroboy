#include "GateBoyLib/Sch_SpriteScanner.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void SpriteScanner::dump(Dumper& d, const SchematicTop& top) const {
  d("\002===== SpriteScan =====\001\n");

  d("SCAN INDEX       %02d\n", pack_p(YFEL_SCAN0.qp(), WEWY_SCAN1.qp(), GOSO_SCAN2.qp(), ELYN_SCAN3.qp(),
                                     FAHA_SCAN4.qp(), FONY_SCAN5.qp(), 0, 0));
  d("SPRITE INDEX     %02d\n", pack_p(XADU_SPRITE_IDX0p.qn(), XEDY_SPRITE_IDX1p.qn(), ZUZE_SPRITE_IDX2p.qn(), XOBE_SPRITE_IDX3p.qn(),
                                      YDUF_SPRITE_IDX4p.qn(), XECU_SPRITE_IDX5p.qn(), 0, 0));

  d("BESU_SCANNINGp   %c\n", BESU_SCANNINGp  .c());
  d("CENO_SCANNINGp   %c\n", CENO_SCANNINGp  .c());
  d("BYBA_SCAN_DONE_A %c\n", BYBA_SCAN_DONE_A.c());
  d("DOBA_SCAN_DONE_B %c\n", DOBA_SCAN_DONE_B.c());
  d("\n");
  d("LCD Y      %03d\n", top.lcd_reg.get_ly());
  d("Y DIFF     %03d\n", pack_p(DEGE_SPRITE_DELTA0, DABY_SPRITE_DELTA1, DABU_SPRITE_DELTA2, GYSA_SPRITE_DELTA3,
                                GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7));
  d("CARE_STORE_ENp_ABxxEFxx %d\n", (wire)CARE_STORE_ENp_ABxxEFxx);

  int oam_temp_a = pack_p(XUSO_OAM_DA0p.q08p(), XEGU_OAM_DA1p.q08p(), YJEX_OAM_DA2p.q08p(), XYJU_OAM_DA3p.q08p(),
                          YBOG_OAM_DA4p.q08p(), WYSO_OAM_DA5p.q08p(), XOTE_OAM_DA6p.q08p(), YZAB_OAM_DA7p.q08p());

  int oam_temp_b = pack_p(YLOR_OAM_DB0p.q08p(), ZYTY_OAM_DB1p.q08p(), ZYVE_OAM_DB2p.q08p(), ZEZY_OAM_DB3p.q08p(),
                          GOMO_OAM_DB4p.q08p(), BAXO_OAM_DB5p.q08p(), YZOS_OAM_DB6p.q08p(), DEPO_OAM_DB7p.q08p());


  d("OAM TEMP A     %03d %02x %c%c%c%c%c%c%c%c\n",
    oam_temp_a,
    oam_temp_a,
    YZAB_OAM_DA7p.c(), XOTE_OAM_DA6p.c(), WYSO_OAM_DA5p.c(), YBOG_OAM_DA4p.c(),
    XYJU_OAM_DA3p.c(), YJEX_OAM_DA2p.c(), XEGU_OAM_DA1p.c(), XUSO_OAM_DA0p.c());

  d("OAM TEMP B     %03d %2x %c%c%c%c%c%c%c%c\n",
    oam_temp_b,
    oam_temp_b,
    DEPO_OAM_DB7p.c(), YZOS_OAM_DB6p.c(), BAXO_OAM_DB5p.c(), GOMO_OAM_DB4p.c(),
    ZEZY_OAM_DB3p.c(), ZYVE_OAM_DB2p.c(), ZYTY_OAM_DB1p.c(), YLOR_OAM_DB0p.c());

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

  /*p29.ERUC*/ wire ERUC_YDIFF_S0 = add_s(EBOS_Y0n, XUSO_OAM_DA0p.q08p(), GND);
  /*p29.ERUC*/ wire ERUC_YDIFF_C0 = add_c(EBOS_Y0n, XUSO_OAM_DA0p.q08p(), GND);
  /*p29.ENEF*/ wire ENEF_YDIFF_S1 = add_s(DASA_Y1n, XEGU_OAM_DA1p.q08p(), ERUC_YDIFF_C0);
  /*p29.ENEF*/ wire ENEF_YDIFF_C1 = add_c(DASA_Y1n, XEGU_OAM_DA1p.q08p(), ERUC_YDIFF_C0);
  /*p29.FECO*/ wire FECO_YDIFF_S2 = add_s(FUKY_Y2n, YJEX_OAM_DA2p.q08p(), ENEF_YDIFF_C1);
  /*p29.FECO*/ wire FECO_YDIFF_C2 = add_c(FUKY_Y2n, YJEX_OAM_DA2p.q08p(), ENEF_YDIFF_C1);
  /*p29.GYKY*/ wire GYKY_YDIFF_S3 = add_s(FUVE_Y3n, XYJU_OAM_DA3p.q08p(), FECO_YDIFF_C2);
  /*p29.GYKY*/ wire GYKY_YDIFF_C3 = add_c(FUVE_Y3n, XYJU_OAM_DA3p.q08p(), FECO_YDIFF_C2);
  /*p29.GOPU*/ wire GOPU_YDIFF_S4 = add_s(FEPU_Y4n, YBOG_OAM_DA4p.q08p(), GYKY_YDIFF_C3);
  /*p29.GOPU*/ wire GOPU_YDIFF_C4 = add_c(FEPU_Y4n, YBOG_OAM_DA4p.q08p(), GYKY_YDIFF_C3);
  /*p29.FUWA*/ wire FUWA_YDIFF_S5 = add_s(FOFA_Y5n, WYSO_OAM_DA5p.q08p(), GOPU_YDIFF_C4);
  /*p29.FUWA*/ wire FUWA_YDIFF_C5 = add_c(FOFA_Y5n, WYSO_OAM_DA5p.q08p(), GOPU_YDIFF_C4);
  /*p29.GOJU*/ wire GOJU_YDIFF_S6 = add_s(FEMO_Y6n, XOTE_OAM_DA6p.q08p(), FUWA_YDIFF_C5);
  /*p29.GOJU*/ wire GOJU_YDIFF_C6 = add_c(FEMO_Y6n, XOTE_OAM_DA6p.q08p(), FUWA_YDIFF_C5);
  /*p29.WUHU*/ wire WUHU_YDIFF_S7 = add_s(GUSU_Y7n, YZAB_OAM_DA7p.q08p(), GOJU_YDIFF_C6);
  /*p29.WUHU*/ wire WUHU_YDIFF_C7 = add_c(GUSU_Y7n, YZAB_OAM_DA7p.q08p(), GOJU_YDIFF_C6);

  /* p29.DEGE*/ DEGE_SPRITE_DELTA0 = not1(ERUC_YDIFF_S0);
  /* p29.DABY*/ DABY_SPRITE_DELTA1 = not1(ENEF_YDIFF_S1);
  /* p29.DABU*/ DABU_SPRITE_DELTA2 = not1(FECO_YDIFF_S2);
  /* p29.GYSA*/ GYSA_SPRITE_DELTA3 = not1(GYKY_YDIFF_S3);
  /* p29.GACE*/ GACE_SPRITE_DELTA4 = not1(GOPU_YDIFF_S4);
  /* p29.GUVU*/ GUVU_SPRITE_DELTA5 = not1(FUWA_YDIFF_S5);
  /* p29.GYDA*/ GYDA_SPRITE_DELTA6 = not1(GOJU_YDIFF_S6);
  /* p29.GEWY*/ GEWY_SPRITE_DELTA7 = not1(WUHU_YDIFF_S7);

  /*#p29.GOVU*/ wire GOVU_SPSIZE_MATCH  = or2(top.pix_pipe.XYMO_LCDC_SPSIZEn.q08(), GYKY_YDIFF_S3);
  /* p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, WUHU_YDIFF_C7, GOVU_SPSIZE_MATCH);
  /* p29.GESE*/ wire GESE_SCAN_MATCH_Y  = not1(WOTA_SCAN_MATCH_Yn);
  /* p29.CEHA*/ wire CEHA_SCANNINGp     = not1(CENO_SCANNINGp.qn());

  /* p29.CARE*/ CARE_STORE_ENp_ABxxEFxx = and3(top.clk_reg.XOCE_xBCxxFGx, CEHA_SCANNINGp, GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.
}

//------------------------------------------------------------------------------

void SpriteScanner::tock(const SchematicTop& top) {
  wire WEFE_VCC = 1;

  /*p01.ATAR*/ wire ATAR_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not1(ATAR_VID_RSTp);

  /*p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /*p01.ALET*/ wire ALET_xBxDxFxH = not1(ZEME_AxCxExGx);

  // 32 + 4 + 2 + 1 = 39
  /*#p28.FETO*/ wire FETO_SCAN_DONE_d0 = and4(YFEL_SCAN0.qp(), WEWY_SCAN1.qp(), GOSO_SCAN2.qp(), FONY_SCAN5.qp());
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(top.lcd_reg.ATEJ_LINE_TRIGp, ATAR_VID_RSTp);

  //----------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  {
    /*#p29.BALU*/ wire BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
    /*#p29.BAGY*/ wire BAGY_LINE_RSTn = not1(BALU_LINE_RSTp);
    /*#p29.BYBA*/ BYBA_SCAN_DONE_A.dff17(top.clk_reg.XUPY_ABxxEFxx, BAGY_LINE_RSTn, FETO_SCAN_DONE_d0);
    /*#p29.DOBA*/ DOBA_SCAN_DONE_B.dff17(ALET_xBxDxFxH, BAGY_LINE_RSTn, BYBA_SCAN_DONE_A.qp());

    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_B.qp(), BALU_LINE_RSTp, BYBA_SCAN_DONE_A.qn());
    /*#p29.AVAP*/ wire AVAP_RENDER_START_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);

    /*#p28.ASEN*/ wire ASEN_SCAN_DONE_PE = or2(ATAR_VID_RSTp, AVAP_RENDER_START_TRIGp);
    /*#p28.BESU*/ BESU_SCANNINGp.nor_latch(top.lcd_reg.CATU_LINE_P000.qp(), ASEN_SCAN_DONE_PE);
    /*#p29.CENO*/ CENO_SCANNINGp.dff17(top.clk_reg.XUPY_ABxxEFxx, ABEZ_VID_RSTn, BESU_SCANNINGp.qp());
  }

  {
    /*#p30.CYKE*/ wire CYKE_ABxxEFxx = not1(top.clk_reg.XUPY_ABxxEFxx);
    /*#p30.WUDA*/ wire WUDA_xxCDxxGH = not1(CYKE_ABxxEFxx);
    /* p28.YFOT*/ wire YFOT_OAM_A2p = not1(top.oam_bus.BUS_OAM_A2n.qp());
    /* p28.YFOC*/ wire YFOC_OAM_A3p = not1(top.oam_bus.BUS_OAM_A3n.qp());
    /* p28.YVOM*/ wire YVOM_OAM_A4p = not1(top.oam_bus.BUS_OAM_A4n.qp());
    /* p28.YMEV*/ wire YMEV_OAM_A5p = not1(top.oam_bus.BUS_OAM_A5n.qp());
    /* p28.XEMU*/ wire XEMU_OAM_A6p = not1(top.oam_bus.BUS_OAM_A6n.qp());
    /* p28.YZET*/ wire YZET_OAM_A7p = not1(top.oam_bus.BUS_OAM_A7n.qp());

    /*p30.XADU*/ XADU_SPRITE_IDX0p.dff13(WUDA_xxCDxxGH, WEFE_VCC, YFOT_OAM_A2p);
    /*p30.XEDY*/ XEDY_SPRITE_IDX1p.dff13(WUDA_xxCDxxGH, WEFE_VCC, YFOC_OAM_A3p);
    /*p30.ZUZE*/ ZUZE_SPRITE_IDX2p.dff13(WUDA_xxCDxxGH, WEFE_VCC, YVOM_OAM_A4p);
    /*p30.XOBE*/ XOBE_SPRITE_IDX3p.dff13(WUDA_xxCDxxGH, WEFE_VCC, YMEV_OAM_A5p);
    /*p30.YDUF*/ YDUF_SPRITE_IDX4p.dff13(WUDA_xxCDxxGH, WEFE_VCC, XEMU_OAM_A6p);
    /*p30.XECU*/ XECU_SPRITE_IDX5p.dff13(WUDA_xxCDxxGH, WEFE_VCC, YZET_OAM_A7p);
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire GAVA_SCAN_CLK = or2(FETO_SCAN_DONE_d0,   top.clk_reg.XUPY_ABxxEFxx);

    /*p28.YFEL*/ YFEL_SCAN0.dff17(GAVA_SCAN_CLK,   ANOM_LINE_RSTn, YFEL_SCAN0.qn());
    /*p28.WEWY*/ WEWY_SCAN1.dff17(YFEL_SCAN0.qn(), ANOM_LINE_RSTn, WEWY_SCAN1.qn());
    /*p28.GOSO*/ GOSO_SCAN2.dff17(WEWY_SCAN1.qn(), ANOM_LINE_RSTn, GOSO_SCAN2.qn());
    /*p28.ELYN*/ ELYN_SCAN3.dff17(GOSO_SCAN2.qn(), ANOM_LINE_RSTn, ELYN_SCAN3.qn());
    /*p28.FAHA*/ FAHA_SCAN4.dff17(ELYN_SCAN3.qn(), ANOM_LINE_RSTn, FAHA_SCAN4.qn());
    /*p28.FONY*/ FONY_SCAN5.dff17(FAHA_SCAN4.qn(), ANOM_LINE_RSTn, FONY_SCAN5.qn());
  }

  {
    /*p29.YWOK*/ wire YWOK_OAM_CLKn = not1(top.oam_bus.COTA_OAM_CLKn);
    /*p29.YWOK*/ wire YWOK_OAM_CLKp = not1(YWOK_OAM_CLKn);

    /*#p29.XUSO*/ XUSO_OAM_DA0p.dff8(YWOK_OAM_CLKn, YWOK_OAM_CLKp, top.oam_bus.YDYV_OAM_LATCH_DA0n.q08p());
    /* p29.XEGU*/ XEGU_OAM_DA1p.dff8(YWOK_OAM_CLKn, YWOK_OAM_CLKp, top.oam_bus.YCEB_OAM_LATCH_DA1n.q08p());
    /* p29.YJEX*/ YJEX_OAM_DA2p.dff8(YWOK_OAM_CLKn, YWOK_OAM_CLKp, top.oam_bus.ZUCA_OAM_LATCH_DA2n.q08p());
    /* p29.XYJU*/ XYJU_OAM_DA3p.dff8(YWOK_OAM_CLKn, YWOK_OAM_CLKp, top.oam_bus.WONE_OAM_LATCH_DA3n.q08p());
    /* p29.YBOG*/ YBOG_OAM_DA4p.dff8(YWOK_OAM_CLKn, YWOK_OAM_CLKp, top.oam_bus.ZAXE_OAM_LATCH_DA4n.q08p());
    /* p29.WYSO*/ WYSO_OAM_DA5p.dff8(YWOK_OAM_CLKn, YWOK_OAM_CLKp, top.oam_bus.XAFU_OAM_LATCH_DA5n.q08p());
    /* p29.XOTE*/ XOTE_OAM_DA6p.dff8(YWOK_OAM_CLKn, YWOK_OAM_CLKp, top.oam_bus.YSES_OAM_LATCH_DA6n.q08p());
    /* p29.YZAB*/ YZAB_OAM_DA7p.dff8(YWOK_OAM_CLKn, YWOK_OAM_CLKp, top.oam_bus.ZECA_OAM_LATCH_DA7n.q08p());

    /* p31.XEGA*/ wire XEGA_OAM_CLKp = not1(top.oam_bus.COTA_OAM_CLKn);
    /* p31.XEGA*/ wire XEGA_OAM_CLKn = not1(XEGA_OAM_CLKp);

    /* p31.YLOR*/ YLOR_OAM_DB0p.dff8(XEGA_OAM_CLKp, XEGA_OAM_CLKn, top.oam_bus.XYKY_OAM_LATCH_DB0n.q08p());
    /* p31.ZYTY*/ ZYTY_OAM_DB1p.dff8(XEGA_OAM_CLKp, XEGA_OAM_CLKn, top.oam_bus.YRUM_OAM_LATCH_DB1n.q08p());
    /* p31.ZYVE*/ ZYVE_OAM_DB2p.dff8(XEGA_OAM_CLKp, XEGA_OAM_CLKn, top.oam_bus.YSEX_OAM_LATCH_DB2n.q08p());
    /* p31.ZEZY*/ ZEZY_OAM_DB3p.dff8(XEGA_OAM_CLKp, XEGA_OAM_CLKn, top.oam_bus.YVEL_OAM_LATCH_DB3n.q08p());
    /* p31.GOMO*/ GOMO_OAM_DB4p.dff8(XEGA_OAM_CLKp, XEGA_OAM_CLKn, top.oam_bus.WYNO_OAM_LATCH_DB4n.q08p());
    /* p31.BAXO*/ BAXO_OAM_DB5p.dff8(XEGA_OAM_CLKp, XEGA_OAM_CLKn, top.oam_bus.CYRA_OAM_LATCH_DB5n.q08p());
    /* p31.YZOS*/ YZOS_OAM_DB6p.dff8(XEGA_OAM_CLKp, XEGA_OAM_CLKn, top.oam_bus.ZUVE_OAM_LATCH_DB6n.q08p());
    /* p31.DEPO*/ DEPO_OAM_DB7p.dff8(XEGA_OAM_CLKp, XEGA_OAM_CLKn, top.oam_bus.ECED_OAM_LATCH_DB7n.q08p());
  }
}

//------------------------------------------------------------------------------
