#include "GateBoyLib/GateBoySpriteScanner.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

wire GateBoy::ACYL_SCANNINGp() {
  /*#p28.ACYL*/ return and2(BOGE_DMA_RUNNINGn(), sprite_scanner.BESU_SCANNINGp.qp_new());
}

wire GateBoy::AVAP_SCAN_DONE_TRIGp() {
  /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp_new(), sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
  /*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  return AVAP_SCAN_DONE_TRIGp;
}

void GateBoy::tock_sprite_scanner() {
  SigIn SIG_VCC = 1;

  /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(ALET_xBxDxFxH(), BAGY_LINE_RSTn_new(), sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
  /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(XUPY_ABxxEFxx(), BAGY_LINE_RSTn_new(), sprite_scanner.FETO_SCAN_DONEp_old());

  /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp_new(), sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
  /*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(rst.ATAR_VID_RSTp(), AVAP_SCAN_DONE_TRIGp);

  /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGn.dff17(XUPY_ABxxEFxx(), rst.ABEZ_VID_RSTn(),  sprite_scanner.BESU_SCANNINGp.qp_old());
  /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd.CATU_START_SCANNING.qp_new(), ASEN_SCAN_DONE_TRIGp);

  /* p28.YFOT*/ wire _YFOT_OAM_A2p_old = not1(new_bus.BUS_OAM_A02n.qp_old());
  /* p28.YFOC*/ wire _YFOC_OAM_A3p_old = not1(new_bus.BUS_OAM_A03n.qp_old());
  /* p28.YVOM*/ wire _YVOM_OAM_A4p_old = not1(new_bus.BUS_OAM_A04n.qp_old());
  /* p28.YMEV*/ wire _YMEV_OAM_A5p_old = not1(new_bus.BUS_OAM_A05n.qp_old());
  /* p28.XEMU*/ wire _XEMU_OAM_A6p_old = not1(new_bus.BUS_OAM_A06n.qp_old());
  /* p28.YZET*/ wire _YZET_OAM_A7p_old = not1(new_bus.BUS_OAM_A07n.qp_old());

  // Sprite store grabs the sprite index off the _old_ oam address new_bus
  /* p30.XADU*/ sprite_scanner.XADU_SPRITE_IDX0p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YFOT_OAM_A2p_old);
  /* p30.XEDY*/ sprite_scanner.XEDY_SPRITE_IDX1p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YFOC_OAM_A3p_old);
  /* p30.ZUZE*/ sprite_scanner.ZUZE_SPRITE_IDX2p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YVOM_OAM_A4p_old);
  /* p30.XOBE*/ sprite_scanner.XOBE_SPRITE_IDX3p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YMEV_OAM_A5p_old);
  /* p30.YDUF*/ sprite_scanner.YDUF_SPRITE_IDX4p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _XEMU_OAM_A6p_old);
  /* p30.XECU*/ sprite_scanner.XECU_SPRITE_IDX5p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YZET_OAM_A7p_old);

  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(rst.TOFU_VID_RSTp(), VOGA_HBLANKp.qp_new());
  ///*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_Bp.qp_new(), lcd.BALU_LINE_RSTp_new(), BYBA_SCAN_DONE_Ap.qn_new());
  ///*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  ///*#p28.ACYL*/ wire ACYL_SCANNINGp = and2(dma.BOGE_DMA_RUNNINGn(), BESU_SCANNINGp.qp_new());
  /*#p21.XYMU*/ XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, AVAP_SCAN_DONE_TRIGp);

  /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn_new = and2(sprite_scanner.CENO_SCANNINGn.qn_new(), XYMU_RENDERINGn.qn_new());
  /*#p30.WUZY_STORE_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XADU_SPRITE_IDX0p.qn_new());
  /* p30.WYSE_STORE_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XEDY_SPRITE_IDX1p.qn_new());
  /* p30.ZYSU_STORE_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.ZUZE_SPRITE_IDX2p.qn_new());
  /* p30.WYDA_STORE_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XOBE_SPRITE_IDX3p.qn_new());
  /* p30.WUCO_STORE_I4*/ new_bus.BUS_SPR_I4.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.YDUF_SPRITE_IDX4p.qn_new());
  /* p30.WEZA_STORE_I5*/ new_bus.BUS_SPR_I5.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XECU_SPRITE_IDX5p.qn_new());

  for (int feedback = 0; feedback < 2; feedback++) {
    /* p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(sprite_scanner.FETO_SCAN_DONEp_any(), XUPY_ABxxEFxx());
    /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp,                   ANOM_LINE_RSTn_new(), sprite_scanner.YFEL_SCAN0.qn_any());
    /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1.dff17_any(sprite_scanner.YFEL_SCAN0.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.WEWY_SCAN1.qn_any());
    /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2.dff17_any(sprite_scanner.WEWY_SCAN1.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.GOSO_SCAN2.qn_any());
    /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3.dff17_any(sprite_scanner.GOSO_SCAN2.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.ELYN_SCAN3.qn_any());
    /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4.dff17_any(sprite_scanner.ELYN_SCAN3.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.FAHA_SCAN4.qn_any());
    /* p28.FONY*/ sprite_scanner.FONY_SCAN5.dff17_any(sprite_scanner.FAHA_SCAN4.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.FONY_SCAN5.qn_any());
  }
}

//------------------------------------------------------------------------------------------------------------------------