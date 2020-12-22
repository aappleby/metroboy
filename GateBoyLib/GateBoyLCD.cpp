#include "GateBoyLib/GateBoyLCD.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyTimer.h"
#include "GateBoyLib/GateBoyPixPipe.h"

//------------------------------------------------------------------------------------------------------------------------

wire2 GateBoyLCD::ATEJ_LINE_RSTp_old() const {
  /* p28.ABAF*/ wire2 _ABAF_LINE_P000n_old = not1b(CATU_LINE_P000p.qp_old2());
  /* p28.BYHA*/ wire2 _BYHA_LINE_TRIGn_old = or_and3(ANEL_LINE_P002p.qp_old2(), _ABAF_LINE_P000n_old, ABEZ_VID_RSTn(_XODO_VID_RSTp.qp_old2())); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire2 _ATEJ_LINE_RSTp_old = not1b(_BYHA_LINE_TRIGn_old);
  return bit(_ATEJ_LINE_RSTp_old);
}

//------------------------------------------------------------------------------------------------------------------------

wire2 GateBoyLCD::ATEJ_LINE_RSTp_new() const {
  /* p28.ABAF*/ wire2 _ABAF_LINE_P000n_new = not1b(CATU_LINE_P000p.qp_new2());
  /* p28.BYHA*/ wire2 _BYHA_LINE_TRIGn_new = or_and3(ANEL_LINE_P002p.qp_new2(), _ABAF_LINE_P000n_new, ABEZ_VID_RSTn(_XODO_VID_RSTp.qp_new2())); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire2 _ATEJ_LINE_RSTp_new = not1b(_BYHA_LINE_TRIGn_new);
  return bit(_ATEJ_LINE_RSTp_new);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::tock(
  GateBoyResetDebug& rst,
  GateBoyClock& clk,
  wire2 XYVO_y144p_old,
  wire2 RUTU_x113p_old)
{
  _XODO_VID_RSTp.set(rst.XODO_VID_RSTp());

  /*#p21.POPU*/ POPU_VBLANKp.dff17(reg_lx.NYPE_x113p_new(), rst.LYFE_VID_RSTn(), XYVO_y144p_old);

  /*#p21.PURE*/ wire2 _PURE_LINE_ENDn_old = not1b(RUTU_x113p_old);
  /*#p21.SELA*/ wire2 _SELA_LINE_P908p_old = not1b(_PURE_LINE_ENDn_old);
  /*#p29.ALES*/ wire2 _ALES_y144n_old = not1b(XYVO_y144p_old);
  /*#p29.ABOV*/ wire2 _ABOV_LINE_P908p_old = and2(_SELA_LINE_P908p_old, _ALES_y144n_old);
  /*#p28.ANEL*/ ANEL_LINE_P002p.dff17(clk.AWOH_xxCDxxGH(), rst.ABEZ_VID_RSTn(),  CATU_LINE_P000p.qp_old2());
  /*#p29.CATU*/ CATU_LINE_P000p.dff17(clk.XUPY_ABxxEFxx(), rst.ABEZ_VID_RSTn(), _ABOV_LINE_P908p_old);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_ctrl(GateBoyResetDebug& rst, GateBoyClock& clk) {
  /*#p21.SYGU*/ SYGU_LINE_STROBE.dff17(clk.SONO_ABxxxxGH(), rst.LYFE_VID_RSTn(), reg_lx.TEGY_STROBE());
  /*#p21.RYNO*/ wire2 _RYNO = or2(SYGU_LINE_STROBE.qp_new2(), reg_lx.RUTU_x113p_new());
  /*#p21.POGU*/ wire2 _POGU = not1b(_RYNO);
  PIN52_LCD_CNTRL.pin_out_dp(_POGU);
}

//------------------------------------------------------------------------------------------------------------------------
// if LCDC_ENn, PIN56_LCD_FLIPS = 4k div clock. Otherwise PIN_LCD_FLIPS = xor(LINE_evn,FRAME_evn)

void GateBoyLCD::set_pin_flip(GateBoyResetDebug& rst, wire2 TULU_DIV07p, wire2 XONA_LCDC_LCDENp) {
  /*#p24.LUCA*/ LUCA_LINE_EVENp .dff17(reg_lx.LOFU_x113n_new(), rst.LYFE_VID_RSTn(), LUCA_LINE_EVENp.qn_old2());
  /*#p21.NAPO*/ NAPO_FRAME_EVENp.dff17(POPU_VBLANKp.qp_new2(),   rst.LYFE_VID_RSTn(), NAPO_FRAME_EVENp.qn_old2());

  /*#p24.MAGU*/ wire2 _MAGU = xor2(NAPO_FRAME_EVENp.qp_new2(), LUCA_LINE_EVENp.qn_new2());
  /*#p24.MECO*/ wire2 _MECO = not1b(_MAGU);
  /*#p24.KEBO*/ wire2 _KEBO = not1b(_MECO);
  /* p01.UREK*/ wire2 _UREK_DIV07n = not1b(TULU_DIV07p);
  /*#p24.USEC*/ wire2 _USEC_DIV07p = not1b(_UREK_DIV07n);
  /*#p24.KEDY*/ wire2 _KEDY_LCDC_ENn = not1b(XONA_LCDC_LCDENp);
  /*#p24.KUPA*/ wire2 _KUPA = amux2(XONA_LCDC_LCDENp, _KEBO, _KEDY_LCDC_ENn, _USEC_DIV07p);
  /*#p24.KOFO*/ wire2 _KOFO = not1b(_KUPA);
  PIN56_LCD_FLIPS.pin_out_dp(_KOFO);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_vsync(GateBoyResetDebug& rst) {
  /*#p24.MEDA*/ MEDA_VSYNC_OUTn.dff17(reg_lx.NYPE_x113n_new(), rst.LYFE_VID_RSTn(), reg_ly.NERU_y000p());
  /*#p24.MURE*/ wire2 _MURE_VSYNC = not1b(MEDA_VSYNC_OUTn.qp_new2());
  PIN57_LCD_VSYNC.pin_out_dp(_MURE_VSYNC);
}

//------------------------------------------------------------------------------------------------------------------------
// FIXME inversion
// I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
// seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
// Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
// That would be a loooot of gate delay.
// Could we possibly be incrementing X3p one phase early?

void GateBoyLCD::set_pin_hsync(GateBoyResetDebug& rst, wire2 TYFA_CLKPIPE_odd, wire2 XYMU_RENDERINGp, wire2 XYDO_PX3p, wire2 AVAP_SCAN_DONE_TRIGp) {
  /*#p24.SEGU*/ wire2 _SEGU_CLKPIPE_evn = not1b(TYFA_CLKPIPE_odd);
  /*#p24.ROXO*/ wire2 _ROXO_CLKPIPE_odd = not1b(_SEGU_CLKPIPE_evn);
  /* p24.PAHO*/ PAHO_X_8_SYNC.dff17(bit(~_ROXO_CLKPIPE_odd), XYMU_RENDERINGp, XYDO_PX3p);

  // LCD horizontal sync pin latch
  /*#p24.POME*/ POME.set(nor2b(AVAP_SCAN_DONE_TRIGp, POFY.qp_old2()));
  /*#p24.RUJU*/ RUJU.set(or3(PAHO_X_8_SYNC.qp_new2(), rst.TOFU_VID_RSTp(), POME.qp_new2()));
  /*#p24.POFY*/ POFY.set(not1b(RUJU.qp_new2()));
  /*#p24.POME*/ POME.set(nor2b(AVAP_SCAN_DONE_TRIGp, POFY.qp_new2()));
  /*#p24.RUJU*/ RUJU.set(or3(PAHO_X_8_SYNC.qp_new2(), rst.TOFU_VID_RSTp(), POME.qp_new2()));
  /*#p24.POFY*/ POFY.set(not1b(RUJU.qp_new2()));

  /*#p24.RUZE*/ wire2 _RUZE_HSYNCn = not1b(POFY.qp_new2());
  PIN54_LCD_HSYNC.pin_out_dp( _RUZE_HSYNCn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_data(wire2 REMY_LD0n, wire2 RAVO_LD1n) {
  PIN51_LCD_DATA0.pin_out_dp(REMY_LD0n);
  PIN50_LCD_DATA1.pin_out_dp(RAVO_LD1n);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_latch(GateBoyDiv& div, RegLCDC& reg_lcdc) {
  /* p01.UMEK*/ wire2 _UMEK_DIV06n = not1b(div.UGOT_DIV06p.qp_new2());
  /*#p24.KASA*/ wire2 _KASA_LINE_ENDp = not1b(reg_lx.PURE_LINE_ENDn());
  /*#p24.UMOB*/ wire2 _UMOB_DIV_06p = not1b(_UMEK_DIV06n);
  /*#p24.KEDY*/ wire2 _KEDY_LCDC_ENn = not1b(reg_lcdc.XONA_LCDC_LCDENn.qn_new2());
  /*#p24.KAHE*/ wire2 _KAHE_LINE_ENDp = amux2(reg_lcdc.XONA_LCDC_LCDENn.qn_new2(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, _UMOB_DIV_06p);
  /*#p24.KYMO*/ wire2 _KYMO_LINE_ENDn = not1b(_KAHE_LINE_ENDp);
  PIN55_LCD_LATCH.pin_out_dp(_KYMO_LINE_ENDn);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::set_pin_clock(PixCount& pix_count, FineScroll& fine_scroll, wire2 WEGO_HBLANKp, wire2 SACU_CLKPIPE_evn) {
  /*#p21.XAJO*/ wire2 _XAJO_X_009p = and2(pix_count.XEHO_PX0p.qp_new2(), pix_count.XYDO_PX3p.qp_new2());
  /*#p21.WUSA*/ WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p, WEGO_HBLANKp);
  /*#p21.TOBA*/ wire2 _TOBA_LCD_CLOCK = and2(WUSA_LCD_CLOCK_GATE.qp_new2(), SACU_CLKPIPE_evn);
  /*#p21.SEMU*/ wire2 _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, fine_scroll.POVA_FINE_MATCH_TRIGp());
  /*#p21.RYPO*/ wire2 _RYPO_LCD_CLOCK = not1b(_SEMU_LCD_CLOCK);
  PIN53_LCD_CLOCK.pin_out_dp(_RYPO_LCD_CLOCK);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyLCD::update_lcd_pipe() {
  /*
  lcd_pix_lo.nor_latch(PIN51_LCD_DATA0.qp_new2(), PIN53_LCD_CLOCK.qp_new2() | PIN54_LCD_HSYNC.qp_new2());
  lcd_pix_hi.nor_latch(PIN50_LCD_DATA1.qp_new2(), PIN53_LCD_CLOCK.qp_new2() | PIN54_LCD_HSYNC.qp_new2());

  for (int i = 0; i < 159; i++) {
    lcd_pipe_lo[i].dff(bit(~PIN53_LCD_CLOCK.qp_new2()), 1, 1, lcd_pipe_lo[i + 1].qp_old2());
    lcd_pipe_hi[i].dff(bit(~PIN53_LCD_CLOCK.qp_new2()), 1, 1, lcd_pipe_hi[i + 1].qp_old2());
  }

  lcd_pipe_lo[159].dff(bit(~PIN53_LCD_CLOCK.qp_new2()), 1, 1, lcd_pix_lo.qp_new2());
  lcd_pipe_hi[159].dff(bit(~PIN53_LCD_CLOCK.qp_new2()), 1, 1, lcd_pix_hi.qp_new2());
  */
}

//------------------------------------------------------------------------------------------------------------------------