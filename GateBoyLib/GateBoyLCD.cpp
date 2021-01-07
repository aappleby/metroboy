#include "GateBoyLib/GateBoyLCD.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyTimer.h"
#include "GateBoyLib/GateBoyPixPipe.h"

//------------------------------------------------------------------------------------------------------------------------

wire GateBoyLCD::ATEJ_LINE_RSTp_old() const {
  /* p28.ABAF*/ wire _ABAF_LINE_P000n_old = not1(CATU_LINE_P000p.qp_old());
  /* p28.BYHA*/ wire _BYHA_LINE_TRIGn_old = or_and3(ANEL_LINE_P002p.qp_old(), _ABAF_LINE_P000n_old, ABEZ_VID_RSTn_old()); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire _ATEJ_LINE_RSTp_old = not1(_BYHA_LINE_TRIGn_old);
  return bit(_ATEJ_LINE_RSTp_old);
}

//------------------------------------------------------------------------------------------------------------------------

wire GateBoyLCD::ATEJ_LINE_RSTp_new() const {
  /* p28.ABAF*/ wire _ABAF_LINE_P000n_new = not1(CATU_LINE_P000p.qp_new());
  /* p28.BYHA*/ wire _BYHA_LINE_TRIGn_new = or_and3(ANEL_LINE_P002p.qp_new(), _ABAF_LINE_P000n_new, ABEZ_VID_RSTn()); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire _ATEJ_LINE_RSTp_new = not1(_BYHA_LINE_TRIGn_new);
  return bit(_ATEJ_LINE_RSTp_new);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::tock(
  GateBoyResetDebug& rst,
  GateBoyClock& clk,
  wire XYVO_y144p_old,
  DFF17 RUTU_x113p_old)
{
  _XODO_VID_RSTp = rst.XODO_VID_RSTp();

  /*#p21.POPU*/ POPU_VBLANKp.dff17(reg_lx.NYPE_x113p.qp_new(), rst.LYFE_VID_RSTn(), XYVO_y144p_old);

  /*#p21.PURE*/ wire _PURE_LINE_ENDn_old = not1(RUTU_x113p_old.qp_old());
  /*#p21.SELA*/ wire _SELA_LINE_P908p_old = not1(_PURE_LINE_ENDn_old);
  /*#p29.ALES*/ wire _ALES_y144n_old = not1(XYVO_y144p_old);
  /*#p29.ABOV*/ wire _ABOV_LINE_P908p_old = and2(_SELA_LINE_P908p_old, _ALES_y144n_old);
  /*#p28.ANEL*/ ANEL_LINE_P002p.dff17(clk.AWOH_xxCDxxGH(), rst.ABEZ_VID_RSTn(),  CATU_LINE_P000p.qp_old());
  /*#p29.CATU*/ CATU_LINE_P000p.dff17(clk.XUPY_ABxxEFxx(), rst.ABEZ_VID_RSTn(), _ABOV_LINE_P908p_old);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_ctrl(GateBoyResetDebug& rst, GateBoyClock& clk) {
  /*#p21.SYGU*/ SYGU_LINE_STROBE.dff17(clk.SONO_ABxxxxGH(), rst.LYFE_VID_RSTn(), reg_lx.TEGY_STROBE());
  /*#p21.RYNO*/ wire _RYNO = or2(SYGU_LINE_STROBE.qp_new(), reg_lx.RUTU_x113p.qp_new());
  /*#p21.POGU*/ wire _POGU = not1(_RYNO);
  /*PIN_52*/ PIN_52_LCD_CNTRL.pin_out_dp(_POGU);
}

//------------------------------------------------------------------------------------------------------------------------
// if LCDC_ENn, PIN_56_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)

void GateBoyLCD::set_pin_flip(GateBoyResetDebug& rst, DFF17  TULU_DIV07p, DFF9 XONA_LCDC_LCDENn) {
  /*#p24.LUCA*/ LUCA_LINE_EVENp .dff17(reg_lx.LOFU_x113n_new(), rst.LYFE_VID_RSTn(), LUCA_LINE_EVENp.qn_old());
  /*#p21.NAPO*/ NAPO_FRAME_EVENp.dff17(POPU_VBLANKp.qp_new(),   rst.LYFE_VID_RSTn(), NAPO_FRAME_EVENp.qn_old());

  /*#p24.MAGU*/ wire _MAGU = xor2(NAPO_FRAME_EVENp.qp_new(), LUCA_LINE_EVENp.qn_new());
  /*#p24.MECO*/ wire _MECO = not1(_MAGU);
  /*#p24.KEBO*/ wire _KEBO = not1(_MECO);
  /* p01.UREK*/ wire _UREK_DIV07n = not1(TULU_DIV07p.qp_new());
  /*#p24.USEC*/ wire _USEC_DIV07p = not1(_UREK_DIV07n);
  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(XONA_LCDC_LCDENn.qn_new());
  /*#p24.KUPA*/ wire _KUPA = amux2(XONA_LCDC_LCDENn.qn_new(), _KEBO, _KEDY_LCDC_ENn, _USEC_DIV07p);
  /*#p24.KOFO*/ wire _KOFO = not1(_KUPA);
  /*PIN_56*/ PIN_56_LCD_FLIPS.pin_out_dp(_KOFO);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_vsync(GateBoyResetDebug& rst) {
  /*#p24.MEDA*/ MEDA_VSYNC_OUTn.dff17(reg_lx.NYPE_x113p.qn_new(), rst.LYFE_VID_RSTn(), reg_ly.NERU_y000p());
  /*#p24.MURE*/ wire _MURE_VSYNC = not1(MEDA_VSYNC_OUTn.qp_new());
  /*PIN_57*/ PIN_57_LCD_VSYNC.pin_out_dp(_MURE_VSYNC);
}

//------------------------------------------------------------------------------------------------------------------------
// FIXME inversion
// I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
// seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
// Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
// That would be a loooot of gate delay.
// Could we possibly be incrementing X3p one phase early?

void GateBoyLCD::set_pin_hsync(GateBoyResetDebug& rst, wire TYFA_CLKPIPE_odd, NorLatch XYMU_RENDERINGn, DFF17 XYDO_PX3p_old, wire AVAP_SCAN_DONE_TRIGp) {
  /*#p24.SEGU*/ wire _SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.ROXO*/ wire _ROXO_CLKPIPE_odd = not1(_SEGU_CLKPIPE_evn);

  /* p24.PAHO*/ PAHO_X_8_SYNC.dff17(_ROXO_CLKPIPE_odd, XYMU_RENDERINGn.qn_new(), XYDO_PX3p_old.qp_old());

  // LCD horizontal sync pin latch
  /*#p24.POME*/ POME = nor2(AVAP_SCAN_DONE_TRIGp, POFY);
  /*#p24.RUJU*/ RUJU = or3(PAHO_X_8_SYNC.qp_new(), rst.TOFU_VID_RSTp(), POME);
  /*#p24.POFY*/ POFY = not1(RUJU);
  /*#p24.POME*/ POME = nor2(AVAP_SCAN_DONE_TRIGp, POFY);
  /*#p24.RUJU*/ RUJU = or3(PAHO_X_8_SYNC.qp_new(), rst.TOFU_VID_RSTp(), POME);
  /*#p24.POFY*/ POFY = not1(RUJU);

  /*#p24.RUZE*/ wire _RUZE_HSYNCn = not1(POFY);
  /*PIN_54*/ PIN_54_LCD_HSYNC.pin_out_dp( _RUZE_HSYNCn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_data(wire REMY_LD0n, wire RAVO_LD1n) {
  /*PIN_51*/ PIN_51_LCD_DATA0.pin_out_dp(REMY_LD0n);
  /*PIN_50*/ PIN_50_LCD_DATA1.pin_out_dp(RAVO_LD1n);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_latch(GateBoyDiv& div, RegLCDC& reg_lcdc) {
  /* p01.UMEK*/ wire _UMEK_DIV06n = not1(div.UGOT_DIV06p.qp_new());
  /*#p24.KASA*/ wire _KASA_LINE_ENDp = not1(reg_lx.PURE_LINE_ENDn());
  /*#p24.UMOB*/ wire _UMOB_DIV_06p = not1(_UMEK_DIV06n);
  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(reg_lcdc.XONA_LCDC_LCDENn.qn_new());
  /*#p24.KAHE*/ wire _KAHE_LINE_ENDp = amux2(reg_lcdc.XONA_LCDC_LCDENn.qn_new(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, _UMOB_DIV_06p);
  /*#p24.KYMO*/ wire _KYMO_LINE_ENDn = not1(_KAHE_LINE_ENDp);
  /*PIN_55*/ PIN_55_LCD_LATCH.pin_out_dp(_KYMO_LINE_ENDn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_clock(PixCount& pix_count, FineScroll& fine_scroll, wire WEGO_HBLANKp, wire SACU_CLKPIPE_evn) {
  /*#p21.XAJO*/ wire _XAJO_X_009p = and2(pix_count.XEHO_PX0p.qp_new(), pix_count.XYDO_PX3p.qp_new());
  /*#p21.WUSA*/ WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p, WEGO_HBLANKp);
  /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK = and2(WUSA_LCD_CLOCK_GATE.qp_new(), SACU_CLKPIPE_evn);
  /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, fine_scroll.POVA_FINE_MATCH_TRIGp());
  /*#p21.RYPO*/ wire _RYPO_LCD_CLOCK = not1(_SEMU_LCD_CLOCK);
  /*PIN_53*/ PIN_53_LCD_CLOCK.pin_out_dp(_RYPO_LCD_CLOCK);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::update_lcd_pipe() {
  /*
  lcd_pix_lo.nor_latch(PIN_51_LCD_DATA0.qp_new(), PIN_53_LCD_CLOCK.qp_new() | PIN_54_LCD_HSYNC.qp_new());
  lcd_pix_hi.nor_latch(PIN_50_LCD_DATA1.qp_new(), PIN_53_LCD_CLOCK.qp_new() | PIN_54_LCD_HSYNC.qp_new());

  for (int i = 0; i < 159; i++) {
    lcd_pipe_lo[i].dff(bit(~PIN_53_LCD_CLOCK.qp_new()), 1, 1, lcd_pipe_lo[i + 1].qp_old());
    lcd_pipe_hi[i].dff(bit(~PIN_53_LCD_CLOCK.qp_new()), 1, 1, lcd_pipe_hi[i + 1].qp_old());
  }

  lcd_pipe_lo[159].dff(bit(~PIN_53_LCD_CLOCK.qp_new()), 1, 1, lcd_pix_lo.qp_new());
  lcd_pipe_hi[159].dff(bit(~PIN_53_LCD_CLOCK.qp_new()), 1, 1, lcd_pix_hi.qp_new());
  */
}

//------------------------------------------------------------------------------------------------------------------------