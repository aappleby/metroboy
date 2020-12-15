#include "GateBoyLib/GateBoySpriteScanner.h"

#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyOamBus.h"

//------------------------------------------------------------------------------------------------------------------------

wire SpriteDeltaY::GESE_SCAN_MATCH_Yp(wire XYMO_LCDC_SPSIZEn) {
  /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH_new = or2(XYMO_LCDC_SPSIZEn, GYKY_YDIFF_S3);
  /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn_new = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH_new);
  /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp_new = not1(_WOTA_SCAN_MATCH_Yn_new);
  return _GESE_SCAN_MATCH_Yp_new;
}

//------------------------------------------------------------------------------------------------------------------------

SpriteDeltaY sprite_delta_y(const OamTempA& oam_temp_a, const RegLY& reg_ly) {
  wire GND = 0;

  /*#p29.EBOS*/ wire EBOS_LY0n_new_evn = not1(reg_ly.MUWY_LY0p.qp_new());
  /* p29.DASA*/ wire DASA_LY1n_new_evn = not1(reg_ly.MYRO_LY1p.qp_new());
  /* p29.FUKY*/ wire FUKY_LY2n_new_evn = not1(reg_ly.LEXA_LY2p.qp_new());
  /* p29.FUVE*/ wire FUVE_LY3n_new_evn = not1(reg_ly.LYDO_LY3p.qp_new());
  /* p29.FEPU*/ wire FEPU_LY4n_new_evn = not1(reg_ly.LOVU_LY4p.qp_new());
  /* p29.FOFA*/ wire FOFA_LY5n_new_evn = not1(reg_ly.LEMA_LY5p.qp_new());
  /* p29.FEMO*/ wire FEMO_LY6n_new_evn = not1(reg_ly.MATO_LY6p.qp_new());
  /* p29.GUSU*/ wire GUSU_LY7n_new_evn = not1(reg_ly.LAFO_LY7p.qp_new());

  /* p29.ERUC*/ wire _ERUC_YDIFF_S0_new = add_s(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p.qp_new(), GND);
  /* p29.ERUC*/ wire _ERUC_YDIFF_C0_new = add_c(EBOS_LY0n_new_evn, oam_temp_a.XUSO_OAM_DA0p.qp_new(), GND);
  /* p29.ENEF*/ wire _ENEF_YDIFF_S1_new = add_s(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
  /* p29.ENEF*/ wire _ENEF_YDIFF_C1_new = add_c(DASA_LY1n_new_evn, oam_temp_a.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
  /* p29.FECO*/ wire _FECO_YDIFF_S2_new = add_s(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
  /* p29.FECO*/ wire _FECO_YDIFF_C2_new = add_c(FUKY_LY2n_new_evn, oam_temp_a.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
  /* p29.GYKY*/ wire _GYKY_YDIFF_S3_new = add_s(FUVE_LY3n_new_evn, oam_temp_a.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF_C2_new);
  /* p29.GYKY*/ wire _GYKY_YDIFF_C3_new = add_c(FUVE_LY3n_new_evn, oam_temp_a.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF_C2_new);
  /* p29.GOPU*/ wire _GOPU_YDIFF_S4_new = add_s(FEPU_LY4n_new_evn, oam_temp_a.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF_C3_new);
  /* p29.GOPU*/ wire _GOPU_YDIFF_C4_new = add_c(FEPU_LY4n_new_evn, oam_temp_a.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF_C3_new);
  /* p29.FUWA*/ wire _FUWA_YDIFF_S5_new = add_s(FOFA_LY5n_new_evn, oam_temp_a.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF_C4_new);
  /* p29.FUWA*/ wire _FUWA_YDIFF_C5_new = add_c(FOFA_LY5n_new_evn, oam_temp_a.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF_C4_new);
  /* p29.GOJU*/ wire _GOJU_YDIFF_S6_new = add_s(FEMO_LY6n_new_evn, oam_temp_a.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF_C5_new);
  /* p29.GOJU*/ wire _GOJU_YDIFF_C6_new = add_c(FEMO_LY6n_new_evn, oam_temp_a.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF_C5_new);
  /* p29.WUHU*/ wire _WUHU_YDIFF_S7_new = add_s(GUSU_LY7n_new_evn, oam_temp_a.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF_C6_new);
  /* p29.WUHU*/ wire _WUHU_YDIFF_C7_new = add_c(GUSU_LY7n_new_evn, oam_temp_a.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF_C6_new);

  /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0_new = not1(_ERUC_YDIFF_S0_new);
  /* p29.DABY*/ wire _DABY_SPRITE_DELTA1_new = not1(_ENEF_YDIFF_S1_new);
  /* p29.DABU*/ wire _DABU_SPRITE_DELTA2_new = not1(_FECO_YDIFF_S2_new);
  /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3_new = not1(_GYKY_YDIFF_S3_new);
  /* p29.GACE*/ wire _GACE_SPRITE_DELTA4_new = not1(_GOPU_YDIFF_S4_new);
  /* p29.GUVU*/ wire _GUVU_SPRITE_DELTA5_new = not1(_FUWA_YDIFF_S5_new);
  /* p29.GYDA*/ wire _GYDA_SPRITE_DELTA6_new = not1(_GOJU_YDIFF_S6_new);
  /* p29.GEWY*/ wire _GEWY_SPRITE_DELTA7_new = not1(_WUHU_YDIFF_S7_new);

  return {
    _DEGE_SPRITE_DELTA0_new,
    _DABY_SPRITE_DELTA1_new,
    _DABU_SPRITE_DELTA2_new,
    _GYSA_SPRITE_DELTA3_new,
    _GACE_SPRITE_DELTA4_new,
    _GUVU_SPRITE_DELTA5_new,
    _GYDA_SPRITE_DELTA6_new,
    _GEWY_SPRITE_DELTA7_new,
    _GYKY_YDIFF_S3_new,
    _WUHU_YDIFF_C7_new
  };
}

//------------------------------------------------------------------------------------------------------------------------

void ScanCounter::tock(wire XUPY_ABxxEFxx, wire ANOM_LINE_RSTn) {
/* p28.YFEL*/ YFEL_SCAN0.RSTn(ANOM_LINE_RSTn);
/* p28.WEWY*/ WEWY_SCAN1.RSTn(ANOM_LINE_RSTn);
/* p28.GOSO*/ GOSO_SCAN2.RSTn(ANOM_LINE_RSTn);
/* p28.ELYN*/ ELYN_SCAN3.RSTn(ANOM_LINE_RSTn);
/* p28.FAHA*/ FAHA_SCAN4.RSTn(ANOM_LINE_RSTn);
/* p28.FONY*/ FONY_SCAN5.RSTn(ANOM_LINE_RSTn);

/* p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(FETO_SCAN_DONEp(), XUPY_ABxxEFxx);
/* p28.YFEL*/ YFEL_SCAN0.dff17(GAVA_SCAN_CLOCKp,    ANOM_LINE_RSTn, YFEL_SCAN0.qn_any());
/* p28.WEWY*/ WEWY_SCAN1.dff17(YFEL_SCAN0.qn_any(), ANOM_LINE_RSTn, WEWY_SCAN1.qn_any());
/* p28.GOSO*/ GOSO_SCAN2.dff17(WEWY_SCAN1.qn_any(), ANOM_LINE_RSTn, GOSO_SCAN2.qn_any());
/* p28.ELYN*/ ELYN_SCAN3.dff17(GOSO_SCAN2.qn_any(), ANOM_LINE_RSTn, ELYN_SCAN3.qn_any());
/* p28.FAHA*/ FAHA_SCAN4.dff17(ELYN_SCAN3.qn_any(), ANOM_LINE_RSTn, FAHA_SCAN4.qn_any());
/* p28.FONY*/ FONY_SCAN5.dff17(FAHA_SCAN4.qn_any(), ANOM_LINE_RSTn, FONY_SCAN5.qn_any());
}

//------------------------------------------------------------------------------------------------------------------------