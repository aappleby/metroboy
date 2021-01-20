#include "GateBoyLib/GateBoySpriteScanner.h"

#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyDMA.h"
#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyOamBus.h"
#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoyResetDebug.h"

//------------------------------------------------------------------------------------------------------------------------

wire SpriteScanner::ACYL_SCANNINGp(GateBoyDMA& dma) {
  /*#p28.ACYL*/ return and2(dma.BOGE_DMA_RUNNINGn(), BESU_SCANNINGp.qp_new());
}

wire SpriteScanner::AVAP_SCAN_DONE_TRIGp(GateBoyLCD& lcd) {
  /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_Bp.qp_new(), lcd.BALU_LINE_RSTp_new(), BYBA_SCAN_DONE_Ap.qn_new());
  /*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  return AVAP_SCAN_DONE_TRIGp;
}

void SpriteScanner::tock(
  GateBoyBuses& new_bus,
  GateBoyResetDebug& rst,
  GateBoyClock& clk,
  GateBoyLCD& lcd,
  GateBoySpriteStore& sprite_store,
  GateBoyOamBus& oam_bus,
  DFF17& VOGA_HBLANKp,
  NorLatch& XYMU_RENDERINGn
) {
  (void)oam_bus;
  (void)sprite_store;

  SigIn SIG_VCC = 1;

  /*#p29.DOBA*/ DOBA_SCAN_DONE_Bp.dff17(clk.ALET_xBxDxFxH(), lcd.BAGY_LINE_RSTn_new(), BYBA_SCAN_DONE_Ap.qp_old());
  /*#p29.BYBA*/ BYBA_SCAN_DONE_Ap.dff17(clk.XUPY_ABxxEFxx(), lcd.BAGY_LINE_RSTn_new(), FETO_SCAN_DONEp_old());

  /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_Bp.qp_new(), lcd.BALU_LINE_RSTp_new(), BYBA_SCAN_DONE_Ap.qn_new());
  /*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(rst.ATAR_VID_RSTp(), AVAP_SCAN_DONE_TRIGp);

  /*#p29.CENO*/ CENO_SCANNINGn.dff17(clk.XUPY_ABxxEFxx(), rst.ABEZ_VID_RSTn(),  BESU_SCANNINGp.qp_old());
  /*#p28.BESU*/ BESU_SCANNINGp.nor_latch(lcd.CATU_START_SCANNING.qp_new(), ASEN_SCAN_DONE_TRIGp);

  /* p28.YFOT*/ wire _YFOT_OAM_A2p_old = not1(new_bus.BUS_OAM_A02n.qp_old());
  /* p28.YFOC*/ wire _YFOC_OAM_A3p_old = not1(new_bus.BUS_OAM_A03n.qp_old());
  /* p28.YVOM*/ wire _YVOM_OAM_A4p_old = not1(new_bus.BUS_OAM_A04n.qp_old());
  /* p28.YMEV*/ wire _YMEV_OAM_A5p_old = not1(new_bus.BUS_OAM_A05n.qp_old());
  /* p28.XEMU*/ wire _XEMU_OAM_A6p_old = not1(new_bus.BUS_OAM_A06n.qp_old());
  /* p28.YZET*/ wire _YZET_OAM_A7p_old = not1(new_bus.BUS_OAM_A07n.qp_old());

  // Sprite store grabs the sprite index off the _old_ oam address new_bus
  /* p30.XADU*/ XADU_SPRITE_IDX0p.dff13(clk.WUDA_xxCDxxGH(), SIG_VCC, _YFOT_OAM_A2p_old);
  /* p30.XEDY*/ XEDY_SPRITE_IDX1p.dff13(clk.WUDA_xxCDxxGH(), SIG_VCC, _YFOC_OAM_A3p_old);
  /* p30.ZUZE*/ ZUZE_SPRITE_IDX2p.dff13(clk.WUDA_xxCDxxGH(), SIG_VCC, _YVOM_OAM_A4p_old);
  /* p30.XOBE*/ XOBE_SPRITE_IDX3p.dff13(clk.WUDA_xxCDxxGH(), SIG_VCC, _YMEV_OAM_A5p_old);
  /* p30.YDUF*/ YDUF_SPRITE_IDX4p.dff13(clk.WUDA_xxCDxxGH(), SIG_VCC, _XEMU_OAM_A6p_old);
  /* p30.XECU*/ XECU_SPRITE_IDX5p.dff13(clk.WUDA_xxCDxxGH(), SIG_VCC, _YZET_OAM_A7p_old);

  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(rst.TOFU_VID_RSTp(), VOGA_HBLANKp.qp_new());
  ///*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_Bp.qp_new(), lcd.BALU_LINE_RSTp_new(), BYBA_SCAN_DONE_Ap.qn_new());
  ///*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  ///*#p28.ACYL*/ wire ACYL_SCANNINGp = and2(dma.BOGE_DMA_RUNNINGn(), BESU_SCANNINGp.qp_new());
  /*#p21.XYMU*/ XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, AVAP_SCAN_DONE_TRIGp);

  /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn_new = and2(CENO_SCANNINGn.qn_new(), XYMU_RENDERINGn.qn_new());
  /*#p30.WUZY_STORE_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XADU_SPRITE_IDX0p.qn_new());
  /* p30.WYSE_STORE_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XEDY_SPRITE_IDX1p.qn_new());
  /* p30.ZYSU_STORE_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, ZUZE_SPRITE_IDX2p.qn_new());
  /* p30.WYDA_STORE_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XOBE_SPRITE_IDX3p.qn_new());
  /* p30.WUCO_STORE_I4*/ new_bus.BUS_SPR_I4.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, YDUF_SPRITE_IDX4p.qn_new());
  /* p30.WEZA_STORE_I5*/ new_bus.BUS_SPR_I5.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, XECU_SPRITE_IDX5p.qn_new());

  for (int feedback = 0; feedback < 2; feedback++) {
    /* p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(FETO_SCAN_DONEp_any(), clk.XUPY_ABxxEFxx());
    /* p28.YFEL*/ YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp,    lcd.ANOM_LINE_RSTn_new(), YFEL_SCAN0.qn_any());
    /* p28.WEWY*/ WEWY_SCAN1.dff17_any(YFEL_SCAN0.qn_any(), lcd.ANOM_LINE_RSTn_new(), WEWY_SCAN1.qn_any());
    /* p28.GOSO*/ GOSO_SCAN2.dff17_any(WEWY_SCAN1.qn_any(), lcd.ANOM_LINE_RSTn_new(), GOSO_SCAN2.qn_any());
    /* p28.ELYN*/ ELYN_SCAN3.dff17_any(GOSO_SCAN2.qn_any(), lcd.ANOM_LINE_RSTn_new(), ELYN_SCAN3.qn_any());
    /* p28.FAHA*/ FAHA_SCAN4.dff17_any(ELYN_SCAN3.qn_any(), lcd.ANOM_LINE_RSTn_new(), FAHA_SCAN4.qn_any());
    /* p28.FONY*/ FONY_SCAN5.dff17_any(FAHA_SCAN4.qn_any(), lcd.ANOM_LINE_RSTn_new(), FONY_SCAN5.qn_any());
  }
}

//------------------------------------------------------------------------------------------------------------------------