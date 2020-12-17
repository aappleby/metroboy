#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyLCD {

  void reset_app() {
    CATU_LINE_P000p.reset(REG_D0C1);
    ANEL_LINE_P002p.reset(REG_D0C0);
    POPU_VBLANKp.reset(REG_D1C0);
    SYGU_LINE_STROBE.reset(REG_D0C1);
    MEDA_VSYNC_OUTn.reset(REG_D1C1);
    LUCA_LINE_EVENp.reset(REG_D1C1);
    NAPO_FRAME_EVENp.reset(REG_D0C1);
    RUJU.reset(REG_D1C0);
    POFY.reset(REG_D0C0);
    POME.reset(REG_D1C0);
    PAHO_X_8_SYNC.reset(REG_D0C1);
    WUSA_LCD_CLOCK_GATE.reset(REG_D0C0);

    PIN_LCD_DATA1.reset(REG_D0C0);
    PIN_LCD_DATA0.reset(REG_D0C0);
    PIN_LCD_HSYNC.reset(REG_D0C0);
    PIN_LCD_FLIPS.reset(REG_D0C0);
    PIN_LCD_CNTRL.reset(REG_D0C0);
    PIN_LCD_LATCH.reset(REG_D0C0);
    PIN_LCD_CLOCK.reset(REG_D0C0);
    PIN_LCD_VSYNC.reset(REG_D1C0);

    lcd_pix_lo.reset(REG_D0C0);
    lcd_pix_hi.reset(REG_D0C0);
    memset(lcd_pipe_lo, 0x02, sizeof(lcd_pipe_lo));
    memset(lcd_pipe_hi, 0x02, sizeof(lcd_pipe_hi));
  }

  /* p28.ATEJ*/ wire ATEJ_LINE_RSTp() const;
  /* p27.XAHY*/ wire XAHY_LINE_RSTn() const { return not1(ATEJ_LINE_RSTp()); }
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn() const { return nor2(ATEJ_LINE_RSTp(), ATAR_VID_RSTp(_XODO_VID_RSTp.qp())); }
  /* p28.ABAK*/ wire ABAK_LINE_RSTp() const { return  or2(ATEJ_LINE_RSTp(), AMYG_VID_RSTp(_XODO_VID_RSTp.qp())); }
  /*#p29.BALU*/ wire BALU_LINE_RSTp() const { return not1(ANOM_LINE_RSTn()); }
  /* p28.BYVA*/ wire BYVA_LINE_RSTn() const { return not1(ABAK_LINE_RSTp()); }
  /* p29.DYBA*/ wire DYBA_LINE_RSTp() const { return not1(BYVA_LINE_RSTn()); }
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn() const { return not1(BALU_LINE_RSTp()); }
  /* p21.TADY*/ wire TADY_LINE_RSTn() const { return nor2(ATEJ_LINE_RSTp(), TOFU_VID_RSTp(_XODO_VID_RSTp.qp())); }
  /*#p21.PARU*/ wire PARU_VBLANKp() const { return not1(POPU_VBLANKp.qn_new()); }

  void tock(
    GateBoyResetDebug& rst,
    GateBoyClock& clk,
    const RegLX& reg_lx,
    wire XYVO_y144p_old,
    wire RUTU_x113p_old);

  void set_pin_data(wire REMY_LD0n, wire RAVO_LD1n);
  void set_pin_ctrl(GateBoyResetDebug& rst, GateBoyClock& clk, const RegLX& reg_lx);
  void set_pin_flip(GateBoyResetDebug& rst, const RegLX& reg_lx, wire TULU_DIV07p, wire XONA_LCDC_LCDENp);
  void set_pin_vsync(GateBoyResetDebug& rst, const RegLX& reg_lx, const RegLY& reg_ly);
  void set_pin_hsync(GateBoyResetDebug& rst, wire TYFA_CLKPIPE_odd, wire XYMU_RENDERINGp, wire XYDO_PX3p, wire AVAP_SCAN_DONE_TRIGp);
  void set_pin_latch(GateBoyDiv& div, RegLX& reg_lx, RegLCDC& reg_lcdc);
  void set_pin_clock(PixCount& pix_count, FineScroll& fine_scroll, wire WEGO_HBLANKp, wire SACU_CLKPIPE_evn);

  void dump(Dumper& d) {
    d.dump_bitp("CATU_LINE_P000p    ", CATU_LINE_P000p.state);
    d.dump_bitp("ANEL_LINE_P002p    ", ANEL_LINE_P002p.state);
    d.dump_bitp("POPU_VBLANKp       ", POPU_VBLANKp.state);
    d.dump_bitp("SYGU_LINE_STROBE   ", SYGU_LINE_STROBE.state);
    d.dump_bitn("MEDA_VSYNC_OUTn    ", MEDA_VSYNC_OUTn.state);
    d.dump_bitp("LUCA_LINE_EVENp    ", LUCA_LINE_EVENp.state);
    d.dump_bitp("NAPO_FRAME_EVENp   ", NAPO_FRAME_EVENp.state);
    d.dump_bitp("RUJU               ", RUJU.state);
    d.dump_bitp("POFY               ", POFY.state);
    d.dump_bitp("POME               ", POME.state);
    d.dump_bitp("PAHO_X_8_SYNC      ", PAHO_X_8_SYNC.state);
    d.dump_bitp("WUSA_LCD_CLOCK_GATE", WUSA_LCD_CLOCK_GATE.state);
    d.dump_bitp("lcd_pix_lo         ", lcd_pix_lo.state);
    d.dump_bitp("lcd_pix_hi         ", lcd_pix_hi.state);
  }

  Signal _XODO_VID_RSTp;

  // H deltas are due to reg writes
  /*p29.CATU*/ DFF17 CATU_LINE_P000p;  // Axxxxxxx
  /*p28.ANEL*/ DFF17 ANEL_LINE_P002p;  // xxCxxxxx
  /*p21.POPU*/ DFF17 POPU_VBLANKp;     // xxCxxxxH

  /*p21.SYGU*/ DFF17 SYGU_LINE_STROBE; // xxxxxxGH
  /*p24.MEDA*/ DFF17 MEDA_VSYNC_OUTn;  // xxCxxxxH
  /*p24.LUCA*/ DFF17 LUCA_LINE_EVENp;  // xxxxxxGH
  /*p21.NAPO*/ DFF17 NAPO_FRAME_EVENp; // xxCxxxxx

  // RUJU+POFY+POME form a nor latch
  /*p24.RUJU*/ Gate RUJU;                    // AxxxxFxx
  /*p24.POFY*/ Gate POFY;                    // AxxxxFxx
  /*p24.POME*/ Gate POME;                    // AxxxxFxx
  /*p24.PAHO*/ DFF17 PAHO_X_8_SYNC;          // xBxDxFxH
  /*p21.WUSA*/ NorLatch WUSA_LCD_CLOCK_GATE; // xBxDxFGH High on G at beginning of line, low on H at end of line. Not sure what's up with the others. Scroll/sprite count?

  /*PIN_50*/ PinOut PIN_LCD_DATA1;
  /*PIN_51*/ PinOut PIN_LCD_DATA0;
  /*PIN_54*/ PinOut PIN_LCD_HSYNC;
  /*PIN_56*/ PinOut PIN_LCD_FLIPS;
  /*PIN_52*/ PinOut PIN_LCD_CNTRL;
  /*PIN_55*/ PinOut PIN_LCD_LATCH;
  /*PIN_53*/ PinOut PIN_LCD_CLOCK;
  /*PIN_57*/ PinOut PIN_LCD_VSYNC;

  Signal old_lcd_clock;
  Signal old_lcd_latch;

  NorLatch lcd_pix_lo;
  NorLatch lcd_pix_hi;

  DFF lcd_pipe_lo[160];
  DFF lcd_pipe_hi[160];
};

//------------------------------------------------------------------------------------------------------------------------
