#include "GateBoyLib/GateBoyLCD.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

wire GateBoy::ATEJ_LINE_RSTp_old() const {
  /* p28.ABAF*/ wire _ABAF_LINE_P000n_old = not1(lcd.CATU_START_SCANNING.qp_old());
  /* p28.BYHA*/ wire _BYHA_LINE_RSTn_old = or_and3(lcd.ANEL_LINE_P002p.qp_old(), _ABAF_LINE_P000n_old, ABEZ_VID_RSTn_old()); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire _ATEJ_LINE_RSTp_old = not1(_BYHA_LINE_RSTn_old);
  return bit(_ATEJ_LINE_RSTp_old);
}

//------------------------------------------------------------------------------------------------------------------------

wire GateBoy::ATEJ_LINE_RSTp_new() const {
  /* p28.ABAF*/ wire _ABAF_LINE_P000n_new = not1(lcd.CATU_START_SCANNING.qp_new());
  /* p28.BYHA*/ wire _BYHA_LINE_RSTn_new = or_and3(lcd.ANEL_LINE_P002p.qp_new(), _ABAF_LINE_P000n_new, ABEZ_VID_RSTn()); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire _ATEJ_LINE_RSTp_new = not1(_BYHA_LINE_RSTn_new);
  return bit(_ATEJ_LINE_RSTp_new);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_lcd(wire XYVO_y144p_old, DFF17 RUTU_x113p_old)
{
  /*#p21.POPU*/ lcd.POPU_VBLANKp.dff17(lcd.reg_lx.NYPE_x113p.qp_new(), rst.LYFE_VID_RSTn(), XYVO_y144p_old);

  /*#p21.PURE*/ wire _PURE_LINE_ENDn_old = not1(RUTU_x113p_old.qp_old());
  /*#p21.SELA*/ wire _SELA_LINE_P908p_old = not1(_PURE_LINE_ENDn_old);
  /*#p29.ALES*/ wire _ALES_y144n_old = not1(XYVO_y144p_old);
  /*#p29.ABOV*/ wire _ABOV_LINE_P908p_old = and2(_SELA_LINE_P908p_old, _ALES_y144n_old);
  /*#p28.ANEL*/ lcd.ANEL_LINE_P002p.dff17(AWOH_xxCDxxGH(), rst.ABEZ_VID_RSTn(),  lcd.CATU_START_SCANNING.qp_old());
  /*#p29.CATU*/ lcd.CATU_START_SCANNING.dff17(XUPY_ABxxEFxx(), rst.ABEZ_VID_RSTn(), _ABOV_LINE_P908p_old);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_lcd_pin_ctrl() {
  /*#p21.SYGU*/ lcd.SYGU_LINE_STROBE.dff17(SONO_ABxxxxGH(), rst.LYFE_VID_RSTn(), TEGY_STROBE());
  /*#p21.RYNO*/ wire _RYNO = or2(lcd.SYGU_LINE_STROBE.qp_new(), lcd.reg_lx.RUTU_x113p.qp_new());
  /*#p21.POGU*/ wire _POGU = not1(_RYNO);
  /*PIN_52*/ lcd.PIN_52_LCD_CNTRL.pin_out_dp(_POGU);
}

//------------------------------------------------------------------------------------------------------------------------
// if LCDC_ENn, PIN_56_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)

void GateBoy::set_lcd_pin_flip(DFF17  TULU_DIV07p, DFF9 XONA_LCDC_LCDENn) {
  /*#p24.LUCA*/ lcd.LUCA_LINE_EVENp .dff17(lcd.reg_lx.LOFU_x113n_new(), rst.LYFE_VID_RSTn(), lcd.LUCA_LINE_EVENp.qn_old());
  /*#p21.NAPO*/ lcd.NAPO_FRAME_EVENp.dff17(lcd.POPU_VBLANKp.qp_new(),   rst.LYFE_VID_RSTn(), lcd.NAPO_FRAME_EVENp.qn_old());

  /*#p24.MAGU*/ wire _MAGU = xor2(lcd.NAPO_FRAME_EVENp.qp_new(), lcd.LUCA_LINE_EVENp.qn_new());
  /*#p24.MECO*/ wire _MECO = not1(_MAGU);
  /*#p24.KEBO*/ wire _KEBO = not1(_MECO);
  /* p01.UREK*/ wire _UREK_DIV07n = not1(TULU_DIV07p.qp_new());
  /*#p24.USEC*/ wire _USEC_DIV07p = not1(_UREK_DIV07n);
  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(XONA_LCDC_LCDENn.qn_new());
  /*#p24.KUPA*/ wire _KUPA = amux2(XONA_LCDC_LCDENn.qn_new(), _KEBO, _KEDY_LCDC_ENn, _USEC_DIV07p);
  /*#p24.KOFO*/ wire _KOFO = not1(_KUPA);
  /*PIN_56*/ lcd.PIN_56_LCD_FLIPS.pin_out_dp(_KOFO);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_lcd_pin_vsync() {
  /*#p24.MEDA*/ lcd.MEDA_VSYNC_OUTn.dff17(lcd.reg_lx.NYPE_x113p.qn_new(), rst.LYFE_VID_RSTn(), lcd.reg_ly.NERU_VSYNCp());
  /*#p24.MURE*/ wire _MURE_VSYNC = not1(lcd.MEDA_VSYNC_OUTn.qp_new());
  /*PIN_57*/ lcd.PIN_57_LCD_VSYNC.pin_out_dp(_MURE_VSYNC);
}

//------------------------------------------------------------------------------------------------------------------------
// FIXME inversion
// I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
// seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
// Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
// That would be a loooot of gate delay.
// Could we possibly be incrementing X3p one phase early?

void GateBoy::set_lcd_pin_hsync(wire TYFA_CLKPIPE_odd, DFF17 XYDO_PX3p_old) {
  /*#p24.SEGU*/ wire _SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.ROXO*/ wire _ROXO_CLKPIPE_odd = not1(_SEGU_CLKPIPE_evn);

  /* p24.PAHO*/ lcd.PAHO_X_8_SYNC.dff17(_ROXO_CLKPIPE_odd, XYMU_RENDERINGn.qn_new(), XYDO_PX3p_old.qp_old());

  // LCD horizontal sync pin latch
  /*#p24.POME*/ lcd.POME = nor2(AVAP_SCAN_DONE_TRIGp(), lcd.POFY);
  /*#p24.RUJU*/ lcd.RUJU = or3(lcd.PAHO_X_8_SYNC.qp_new(), rst.TOFU_VID_RSTp(), lcd.POME);
  /*#p24.POFY*/ lcd.POFY = not1(lcd.RUJU);
  /*#p24.POME*/ lcd.POME = nor2(AVAP_SCAN_DONE_TRIGp(), lcd.POFY);
  /*#p24.RUJU*/ lcd.RUJU = or3(lcd.PAHO_X_8_SYNC.qp_new(), rst.TOFU_VID_RSTp(), lcd.POME);
  /*#p24.POFY*/ lcd.POFY = not1(lcd.RUJU);

  /*#p24.RUZE*/ wire _RUZE_HSYNCn = not1(lcd.POFY);
  /*PIN_54*/ lcd.PIN_54_LCD_HSYNC.pin_out_dp( _RUZE_HSYNCn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_lcd_pin_data(wire REMY_LD0n, wire RAVO_LD1n) {
  /*PIN_51*/ lcd.PIN_51_LCD_DATA0.pin_out_dp(REMY_LD0n);
  /*PIN_50*/ lcd.PIN_50_LCD_DATA1.pin_out_dp(RAVO_LD1n);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_lcd_pin_latch() {
  /* p01.UMEK*/ wire _UMEK_DIV06n = not1(div.UGOT_DIV06p.qp_new());
  /*#p24.KASA*/ wire _KASA_LINE_ENDp = not1(lcd.reg_lx.PURE_LINE_ENDn());
  /*#p24.UMOB*/ wire _UMOB_DIV_06p = not1(_UMEK_DIV06n);
  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(reg_lcdc.XONA_LCDC_LCDENn.qn_new());
  /*#p24.KAHE*/ wire _KAHE_LINE_ENDp = amux2(reg_lcdc.XONA_LCDC_LCDENn.qn_new(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, _UMOB_DIV_06p);
  /*#p24.KYMO*/ wire _KYMO_LINE_ENDn = not1(_KAHE_LINE_ENDp);
  /*PIN_55*/ lcd.PIN_55_LCD_LATCH.pin_out_dp(_KYMO_LINE_ENDn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_lcd_pin_clock(wire WEGO_HBLANKp, wire SACU_CLKPIPE_evn) {
  /*#p21.XAJO*/ wire _XAJO_X_009p = and2(pix_count.XEHO_PX0p.qp_new(), pix_count.XYDO_PX3p.qp_new());
  /*#p21.WUSA*/ lcd.WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p, WEGO_HBLANKp);
  /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK = and2(lcd.WUSA_LCD_CLOCK_GATE.qp_new(), SACU_CLKPIPE_evn);
  /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, POVA_FINE_MATCH_TRIGp());
  /*#p21.RYPO*/ wire _RYPO_LCD_CLOCK = not1(_SEMU_LCD_CLOCK);
  /*PIN_53*/ lcd.PIN_53_LCD_CLOCK.pin_out_dp(_RYPO_LCD_CLOCK);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::update_lcd_pipe() {
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