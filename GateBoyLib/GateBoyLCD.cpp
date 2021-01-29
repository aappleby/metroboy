#include "GateBoyLib/GateBoyLCD.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_lcd_pin_ctrl() {
  /*#p21.SYGU*/ lcd.SYGU_LINE_STROBE.dff17(SONO_ABxxxxGH(), LYFE_VID_RSTn(), TEGY_STROBE());
  /*#p21.RYNO*/ wire _RYNO = or2(lcd.SYGU_LINE_STROBE.qp_new(), reg_lx.RUTU_x113p.qp_new());
  /*#p21.POGU*/ wire _POGU = not1(_RYNO);
  /*PIN_52*/ lcd.PIN_52_LCD_CNTRL.pin_out_dp(_POGU);
}

//------------------------------------------------------------------------------------------------------------------------
// if LCDC_ENn, PIN_56_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)

void GateBoy::set_lcd_pin_flip(DFF17  TULU_DIV07p, DFF9 XONA_LCDC_LCDENn) {
  /*#p24.LOFU*/ wire LOFU_x113n = not1(reg_lx.RUTU_x113p.qp_new());
  /*#p24.LUCA*/ lcd.LUCA_LINE_EVENp .dff17(LOFU_x113n, LYFE_VID_RSTn(), lcd.LUCA_LINE_EVENp.qn_old());
  /*#p21.NAPO*/ lcd.NAPO_FRAME_EVENp.dff17(lcd.POPU_VBLANKp.qp_new(),   LYFE_VID_RSTn(), lcd.NAPO_FRAME_EVENp.qn_old());

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
  /*#p24.NERU*/ wire _NERU_VSYNCp = nor8(reg_ly.LAFO_LY7p.qp_new(), reg_ly.LOVU_LY4p.qp_new(), reg_ly.LYDO_LY3p.qp_new(), reg_ly.MUWY_LY0p.qp_new(), reg_ly.MYRO_LY1p.qp_new(), reg_ly.LEXA_LY2p.qp_new(), reg_ly.LEMA_LY5p.qp_new(), reg_ly.MATO_LY6p.qp_new());
  /*#p24.MEDA*/ lcd.MEDA_VSYNC_OUTn.dff17(reg_lx.NYPE_x113p.qn_new(), LYFE_VID_RSTn(), _NERU_VSYNCp);
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

void GateBoy::set_lcd_pin_hsync() {
  // LCD horizontal sync pin latch
  /*#p24.POME*/ lcd.POME = nor2(AVAP_SCAN_DONE_TRIGp.qp_new(), lcd.POFY.qp_mid());
  /*#p24.RUJU*/ lcd.RUJU = or3(lcd.PAHO_X_8_SYNC.qp_new(), TOFU_VID_RSTp(), lcd.POME.qp_mid());
  /*#p24.POFY*/ lcd.POFY = not1(lcd.RUJU.qp_mid());
  /*#p24.POME*/ lcd.POME = nor2(AVAP_SCAN_DONE_TRIGp.qp_new(), lcd.POFY.qp_mid());
  /*#p24.RUJU*/ lcd.RUJU = or3(lcd.PAHO_X_8_SYNC.qp_new(), TOFU_VID_RSTp(), lcd.POME.qp_mid());
  /*#p24.POFY*/ lcd.POFY = not1(lcd.RUJU.qp_mid());

  /*#p24.RUZE*/ wire _RUZE_HSYNCn = not1(lcd.POFY.qp_mid());
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
  /*#p21.PURE*/ wire _PURE_LINE_ENDn = not1(reg_lx.RUTU_x113p.qp_new());
  /*#p24.KASA*/ wire _KASA_LINE_ENDp = not1(_PURE_LINE_ENDn);
  /*#p24.UMOB*/ wire _UMOB_DIV_06p = not1(_UMEK_DIV06n);
  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(reg_lcdc.XONA_LCDC_LCDENn.qn_new());
  /*#p24.KAHE*/ wire _KAHE_LINE_ENDp = amux2(reg_lcdc.XONA_LCDC_LCDENn.qn_new(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, _UMOB_DIV_06p);
  /*#p24.KYMO*/ wire _KYMO_LINE_ENDn = not1(_KAHE_LINE_ENDp);
  /*PIN_55*/ lcd.PIN_55_LCD_LATCH.pin_out_dp(_KYMO_LINE_ENDn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_lcd_pin_clock(wire SACU_CLKPIPE_evn) {
  /*#p21.XAJO*/ wire _XAJO_X_009p = and2(pix_count.XEHO_PX0p.qp_new(), pix_count.XYDO_PX3p.qp_new());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), VOGA_HBLANKp.qp_new());
  /*#p21.WUSA*/ lcd.WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p, WEGO_HBLANKp);
  /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK = and2(lcd.WUSA_LCD_CLOCK_GATE.qp_new(), SACU_CLKPIPE_evn);
  /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_new(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_new());
  /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, _POVA_FINE_MATCH_TRIGp);
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