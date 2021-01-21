#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyLCD {

  void reset_to_cart() {
    reg_lx.reset_to_cart();
    reg_ly.reset_to_cart();
    reg_lyc.reset_to_cart();

    CATU_START_SCANNING.state = 0b00011010;
    ANEL_LINE_P002p.state = 0b00011000;
    POPU_VBLANKp.state = 0b00011001;
    SYGU_LINE_STROBE.state = 0b00011010;
    MEDA_VSYNC_OUTn.state = 0b00011011;
    LUCA_LINE_EVENp.state = 0b00011011;
    NAPO_FRAME_EVENp.state = 0b00011010;
    RUJU.state = 0b00011001;
    POFY.state = 0b00011000;
    POME.state = 0b00011001;
    PAHO_X_8_SYNC.state = 0b00011000;
    WUSA_LCD_CLOCK_GATE.state = 0b00011000;
    PIN_50_LCD_DATA1.state = 0b00011000;
    PIN_51_LCD_DATA0.state = 0b00011000;
    PIN_54_LCD_HSYNC.state = 0b00011001;
    PIN_56_LCD_FLIPS.state = 0b00011001;
    PIN_52_LCD_CNTRL.state = 0b00011001;
    PIN_55_LCD_LATCH.state = 0b00011001;
    PIN_53_LCD_CLOCK.state = 0b00011001;
    PIN_57_LCD_VSYNC.state = 0b00011000;
  }

  void dump(Dumper& d) {
    d.dump_bitp("CATU_START_SCANNING     : ", CATU_START_SCANNING.state);
    d.dump_bitp("ANEL_LINE_P002p     : ", ANEL_LINE_P002p.state);
    d.dump_bitp("POPU_VBLANKp        : ", POPU_VBLANKp.state);
    d.dump_bitp("SYGU_LINE_STROBE    : ", SYGU_LINE_STROBE.state);
    d.dump_bitn("MEDA_VSYNC_OUTn     : ", MEDA_VSYNC_OUTn.state);
    d.dump_bitp("LUCA_LINE_EVENp     : ", LUCA_LINE_EVENp.state);
    d.dump_bitp("NAPO_FRAME_EVENp    : ", NAPO_FRAME_EVENp.state);
    d.dump_bitp("RUJU                : ", RUJU.get_state());
    d.dump_bitp("POFY                : ", POFY.get_state());
    d.dump_bitp("POME                : ", POME.get_state());
    d.dump_bitp("PAHO_X_8_SYNC       : ", PAHO_X_8_SYNC.state);
    d.dump_bitp("WUSA_LCD_CLOCK_GATE : ", WUSA_LCD_CLOCK_GATE.state);
    //d.dump_bitp("lcd_pix_lo          : ", lcd_pix_lo.state);
    //d.dump_bitp("lcd_pix_hi          : ", lcd_pix_hi.state);
    d("\n");
    d.dump_bitp("PIN_50_LCD_DATA1     : ", PIN_50_LCD_DATA1.state);
    d.dump_bitp("PIN_51_LCD_DATA0     : ", PIN_51_LCD_DATA0.state);
    d.dump_bitp("PIN_54_LCD_HSYNC     : ", PIN_54_LCD_HSYNC.state);
    d.dump_bitp("PIN_56_LCD_FLIPS     : ", PIN_56_LCD_FLIPS.state);
    d.dump_bitp("PIN_52_LCD_CNTRL     : ", PIN_52_LCD_CNTRL.state);
    d.dump_bitp("PIN_55_LCD_LATCH     : ", PIN_55_LCD_LATCH.state);
    d.dump_bitp("PIN_53_LCD_CLOCK     : ", PIN_53_LCD_CLOCK.state);
    d.dump_bitp("PIN_57_LCD_VSYNC     : ", PIN_57_LCD_VSYNC.state);
  }

  RegLX  reg_lx;
  RegLY  reg_ly;
  RegLYC reg_lyc;

  // H deltas are due to reg writes
  /*p29.CATU*/ DFF17 CATU_START_SCANNING;  // Axxxxxxx
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

  /*PIN_50*/ PinOut PIN_50_LCD_DATA1;
  /*PIN_51*/ PinOut PIN_51_LCD_DATA0;
  /*PIN_54*/ PinOut PIN_54_LCD_HSYNC;
  /*PIN_56*/ PinOut PIN_56_LCD_FLIPS;
  /*PIN_52*/ PinOut PIN_52_LCD_CNTRL;
  /*PIN_55*/ PinOut PIN_55_LCD_LATCH;
  /*PIN_53*/ PinOut PIN_53_LCD_CLOCK;
  /*PIN_57*/ PinOut PIN_57_LCD_VSYNC;
};

//------------------------------------------------------------------------------------------------------------------------
