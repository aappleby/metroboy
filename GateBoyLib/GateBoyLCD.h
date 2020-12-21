#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyLCD {

  void reset_to_cart() {
    _XODO_VID_RSTp.reset(0);

    reg_lx.reset_to_cart();
    reg_ly.reset_to_cart();
    reg_lyc.reset_to_cart();

    CATU_LINE_P000p.reset(1, 0);
    ANEL_LINE_P002p.reset(0, 0);
    POPU_VBLANKp.reset(0, 1);
    SYGU_LINE_STROBE.reset(1, 0);
    MEDA_VSYNC_OUTn.reset(1, 1);
    LUCA_LINE_EVENp.reset(1, 1);
    NAPO_FRAME_EVENp.reset(1, 0);
    RUJU.reset(1);
    POFY.reset(0);
    POME.reset(1);
    PAHO_X_8_SYNC.reset(1, 0);
    WUSA_LCD_CLOCK_GATE.reset(0);
    PIN50_LCD_DATA1.reset(1);
    PIN51_LCD_DATA0.reset(1);
    PIN54_LCD_HSYNC.reset(1);
    PIN56_LCD_FLIPS.reset(1);
    PIN52_LCD_CNTRL.reset(1);
    PIN55_LCD_LATCH.reset(1);
    PIN53_LCD_CLOCK.reset(1);
    PIN57_LCD_VSYNC.reset(0);

    //lcd_pix_lo.reset_to_cart(REG_D0C0);
    //lcd_pix_hi.reset_to_cart(REG_D0C0);
    //memset(lcd_pipe_lo, 0x02, sizeof(lcd_pipe_lo));
    //memset(lcd_pipe_hi, 0x02, sizeof(lcd_pipe_hi));
  }

  /* p28.ATEJ*/ wire ATEJ_LINE_RSTp_old() const;
  /* p27.XAHY*/ wire XAHY_LINE_RSTn_old() const { return not1(ATEJ_LINE_RSTp_old()); }
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn_old() const { return nor2(ATEJ_LINE_RSTp_old(), ATAR_VID_RSTp(_XODO_VID_RSTp.qp_old())); }
  /* p28.ABAK*/ wire ABAK_LINE_RSTp_old() const { return  or2(ATEJ_LINE_RSTp_old(), AMYG_VID_RSTp(_XODO_VID_RSTp.qp_old())); }
  /*#p29.BALU*/ wire BALU_LINE_RSTp_old() const { return not1(ANOM_LINE_RSTn_old()); }
  /* p28.BYVA*/ wire BYVA_LINE_RSTn_old() const { return not1(ABAK_LINE_RSTp_old()); }
  /* p29.DYBA*/ wire DYBA_LINE_RSTp_old() const { return not1(BYVA_LINE_RSTn_old()); }
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn_old() const { return not1(BALU_LINE_RSTp_old()); }
  /* p21.TADY*/ wire TADY_LINE_RSTn_old() const { return nor2(ATEJ_LINE_RSTp_old(), TOFU_VID_RSTp(_XODO_VID_RSTp.qp_old())); }

  /* p28.ATEJ*/ wire ATEJ_LINE_RSTp_new() const;
  /* p27.XAHY*/ wire XAHY_LINE_RSTn_new() const { return not1(ATEJ_LINE_RSTp_new()); }
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn_new() const { return nor2(ATEJ_LINE_RSTp_new(), ATAR_VID_RSTp(_XODO_VID_RSTp.qp_new())); }
  /* p28.ABAK*/ wire ABAK_LINE_RSTp_new() const { return  or2(ATEJ_LINE_RSTp_new(), AMYG_VID_RSTp(_XODO_VID_RSTp.qp_new())); }
  /*#p29.BALU*/ wire BALU_LINE_RSTp_new() const { return not1(ANOM_LINE_RSTn_new()); }
  /* p28.BYVA*/ wire BYVA_LINE_RSTn_new() const { return not1(ABAK_LINE_RSTp_new()); }
  /* p29.DYBA*/ wire DYBA_LINE_RSTp_new() const { return not1(BYVA_LINE_RSTn_new()); }
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn_new() const { return not1(BALU_LINE_RSTp_new()); }
  /* p21.TADY*/ wire TADY_LINE_RSTn_new() const { return nor2(ATEJ_LINE_RSTp_new(), TOFU_VID_RSTp(_XODO_VID_RSTp.qp_new())); }

  /*#p21.PARU*/ wire PARU_VBLANKp() const { return not1(POPU_VBLANKp.qn_new()); }

  void tock(
    GateBoyResetDebug& rst,
    GateBoyClock& clk,
    wire XYVO_y144p_old,
    wire RUTU_x113p_old);

  void set_pin_data(wire REMY_LD0n, wire RAVO_LD1n);
  void set_pin_ctrl(GateBoyResetDebug& rst, GateBoyClock& clk);
  void set_pin_flip(GateBoyResetDebug& rst, wire TULU_DIV07p, wire XONA_LCDC_LCDENp);
  void set_pin_vsync(GateBoyResetDebug& rst);
  void set_pin_hsync(GateBoyResetDebug& rst, wire TYFA_CLKPIPE_odd, wire XYMU_RENDERINGp, wire XYDO_PX3p, wire AVAP_SCAN_DONE_TRIGp);
  void set_pin_latch(GateBoyDiv& div, RegLCDC& reg_lcdc);
  void set_pin_clock(PixCount& pix_count, FineScroll& fine_scroll, wire WEGO_HBLANKp, wire SACU_CLKPIPE_evn);
  void update_lcd_pipe();

  void dump(Dumper& d) {
    d.dump_bitp("CATU_LINE_P000p     : ", CATU_LINE_P000p.state);
    d.dump_bitp("ANEL_LINE_P002p     : ", ANEL_LINE_P002p.state);
    d.dump_bitp("POPU_VBLANKp        : ", POPU_VBLANKp.state);
    d.dump_bitp("SYGU_LINE_STROBE    : ", SYGU_LINE_STROBE.state);
    d.dump_bitn("MEDA_VSYNC_OUTn     : ", MEDA_VSYNC_OUTn.state);
    d.dump_bitp("LUCA_LINE_EVENp     : ", LUCA_LINE_EVENp.state);
    d.dump_bitp("NAPO_FRAME_EVENp    : ", NAPO_FRAME_EVENp.state);
    d.dump_bitp("RUJU                : ", RUJU.state);
    d.dump_bitp("POFY                : ", POFY.state);
    d.dump_bitp("POME                : ", POME.state);
    d.dump_bitp("PAHO_X_8_SYNC       : ", PAHO_X_8_SYNC.state);
    d.dump_bitp("WUSA_LCD_CLOCK_GATE : ", WUSA_LCD_CLOCK_GATE.state);
    //d.dump_bitp("lcd_pix_lo          : ", lcd_pix_lo.state);
    //d.dump_bitp("lcd_pix_hi          : ", lcd_pix_hi.state);
    d("\n");
    d.dump_bitp("PIN50_LCD_DATA1     : ", PIN50_LCD_DATA1.state);
    d.dump_bitp("PIN51_LCD_DATA0     : ", PIN51_LCD_DATA0.state);
    d.dump_bitp("PIN54_LCD_HSYNC     : ", PIN54_LCD_HSYNC.state);
    d.dump_bitp("PIN56_LCD_FLIPS     : ", PIN56_LCD_FLIPS.state);
    d.dump_bitp("PIN52_LCD_CNTRL     : ", PIN52_LCD_CNTRL.state);
    d.dump_bitp("PIN55_LCD_LATCH     : ", PIN55_LCD_LATCH.state);
    d.dump_bitp("PIN53_LCD_CLOCK     : ", PIN53_LCD_CLOCK.state);
    d.dump_bitp("PIN57_LCD_VSYNC     : ", PIN57_LCD_VSYNC.state);
  }

  Signal _XODO_VID_RSTp;

  RegLX  reg_lx;
  RegLY  reg_ly;
  RegLYC reg_lyc;

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

  //Signal old_lcd_clock;
  //Signal old_lcd_latch;

  //NorLatch lcd_pix_lo;
  //NorLatch lcd_pix_hi;

  //DFF lcd_pipe_lo[160];
  //DFF lcd_pipe_hi[160];

  PinOut PIN50_LCD_DATA1;
  PinOut PIN51_LCD_DATA0;
  PinOut PIN54_LCD_HSYNC;
  PinOut PIN56_LCD_FLIPS;
  PinOut PIN52_LCD_CNTRL;
  PinOut PIN55_LCD_LATCH;
  PinOut PIN53_LCD_CLOCK;
  PinOut PIN57_LCD_VSYNC;
};

//------------------------------------------------------------------------------------------------------------------------
