#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct LCDControl {
  void reset_to_poweron() {
    CATU_x113p.state          = BIT_OLD | BIT_DRIVEN;
    ANEL_x113p.state          = BIT_OLD | BIT_DRIVEN;
    POPU_y144p.state          = BIT_OLD | BIT_DRIVEN;
    MYTA_y153p.state          = BIT_OLD | BIT_DRIVEN;
    RUTU_x113p.state          = BIT_OLD | BIT_DRIVEN;
    NYPE_x113p.state          = BIT_OLD | BIT_DRIVEN;
    SYGU_LINE_STROBE.state    = BIT_OLD | BIT_DRIVEN;
    MEDA_VSYNC_OUTn.state     = BIT_OLD | BIT_DRIVEN;
    LUCA_LINE_EVENp.state     = BIT_OLD | BIT_DRIVEN;
    NAPO_FRAME_EVENp.state    = BIT_OLD | BIT_DRIVEN;
    RUJU.state                = BIT_OLD | BIT_DRIVEN;
    POFY.state                = BIT_OLD | BIT_DRIVEN;
    POME.state                = BIT_OLD | BIT_DRIVEN;
    PAHO_X_8_SYNC.state       = BIT_OLD | BIT_DRIVEN;
    WUSA_LCD_CLOCK_GATE.state = BIT_OLD | BIT_DRIVEN;
    REMY_LD0n.state           = BIT_OLD | BIT_DRIVEN;
    RAVO_LD1n.state           = BIT_OLD | BIT_DRIVEN;
    PIN_50_LCD_DATA1.state    = BIT_OLD | BIT_DRIVEN;
    PIN_51_LCD_DATA0.state    = BIT_OLD | BIT_DRIVEN;
    PIN_54_LCD_HSYNC.state    = BIT_OLD | BIT_DRIVEN;
    PIN_56_LCD_FLIPS.state    = BIT_OLD | BIT_DRIVEN;
    PIN_52_LCD_CNTRL.state    = BIT_OLD | BIT_DRIVEN;
    PIN_55_LCD_LATCH.state    = BIT_OLD | BIT_DRIVEN;
    PIN_53_LCD_CLOCK.state    = BIT_OLD | BIT_DRIVEN;
    PIN_57_LCD_VSYNC.state    = BIT_OLD | BIT_DRIVEN;
  }

  void reset_to_bootrom() {
    CATU_x113p.state          = BIT_OLD | BIT_DRIVEN;
    ANEL_x113p.state          = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    POPU_y144p.state          = BIT_OLD | BIT_DRIVEN;
    MYTA_y153p.state          = BIT_OLD | BIT_DRIVEN;
    RUTU_x113p.state          = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    NYPE_x113p.state          = BIT_OLD | BIT_DRIVEN;
    SYGU_LINE_STROBE.state    = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    MEDA_VSYNC_OUTn.state     = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    LUCA_LINE_EVENp.state     = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    NAPO_FRAME_EVENp.state    = BIT_OLD | BIT_DRIVEN;
    RUJU.state                = BIT_OLD | BIT_DRIVEN | 1;
    POFY.state                = BIT_OLD | BIT_DRIVEN | 0;
    POME.state                = BIT_OLD | BIT_DRIVEN | 1;
    PAHO_X_8_SYNC.state       = BIT_OLD | BIT_DRIVEN;
    WUSA_LCD_CLOCK_GATE.state = BIT_OLD | BIT_DRIVEN;
    REMY_LD0n.state           = BIT_OLD | BIT_DRIVEN;
    RAVO_LD1n.state           = BIT_OLD | BIT_DRIVEN;
    PIN_50_LCD_DATA1.state    = BIT_OLD | BIT_DRIVEN;
    PIN_51_LCD_DATA0.state    = BIT_OLD | BIT_DRIVEN;
    PIN_54_LCD_HSYNC.state    = BIT_OLD | BIT_DRIVEN | 1;
    PIN_56_LCD_FLIPS.state    = BIT_OLD | BIT_DRIVEN | 1;
    PIN_52_LCD_CNTRL.state    = BIT_OLD | BIT_DRIVEN | 1;
    PIN_55_LCD_LATCH.state    = BIT_OLD | BIT_DRIVEN | 1;
    PIN_53_LCD_CLOCK.state    = BIT_OLD | BIT_DRIVEN | 1;
    PIN_57_LCD_VSYNC.state    = BIT_OLD | BIT_DRIVEN | 1;
  }

  void reset_to_cart() {
    CATU_x113p.state          = 0b00011010;
    ANEL_x113p.state          = 0b00011000;
    POPU_y144p.state          = 0b00011001;
    MYTA_y153p.state          = 0b00011001;
    RUTU_x113p.state          = 0b00011010;
    NYPE_x113p.state          = 0b00011000;
    SYGU_LINE_STROBE.state    = 0b00011010;
    MEDA_VSYNC_OUTn.state     = 0b00011011;
    LUCA_LINE_EVENp.state     = 0b00011011;
    NAPO_FRAME_EVENp.state    = 0b00011010;
    RUJU.state                = 0b00011001;
    POFY.state                = 0b00011000;
    POME.state                = 0b00011001;
    PAHO_X_8_SYNC.state       = 0b00011000;
    WUSA_LCD_CLOCK_GATE.state = 0b00011000;
    PIN_50_LCD_DATA1.state    = 0b00011000;
    PIN_51_LCD_DATA0.state    = 0b00011000;
    PIN_54_LCD_HSYNC.state    = 0b00011001;
    PIN_56_LCD_FLIPS.state    = 0b00011001;
    PIN_52_LCD_CNTRL.state    = 0b00011001;
    PIN_55_LCD_LATCH.state    = 0b00011001;
    PIN_53_LCD_CLOCK.state    = 0b00011001;
    PIN_57_LCD_VSYNC.state    = 0b00011000;
  }

  // H deltas are due to reg writes
  /*#p29.CATU*/ DFF17 CATU_x113p; // Axxxxxxx
  /*#p28.ANEL*/ DFF17 ANEL_x113p; // xxCxxxxx
  /*#p21.POPU*/ DFF17 POPU_y144p; // xxCxxxxH
  /*#p21.MYTA*/ DFF17 MYTA_y153p; // xxCxxxxH
  /*#p21.RUTU*/ DFF17 RUTU_x113p; // xxxxxxGx
  /*#p21.NYPE*/ DFF17 NYPE_x113p; // xxCxxxxx

  /*#p21.SYGU*/ DFF17 SYGU_LINE_STROBE; // xxxxxxGH
  /*#p24.MEDA*/ DFF17 MEDA_VSYNC_OUTn;  // xxCxxxxH
  /*#p24.LUCA*/ DFF17 LUCA_LINE_EVENp;  // xxxxxxGH
  /*#p21.NAPO*/ DFF17 NAPO_FRAME_EVENp; // xxCxxxxx

  // RUJU+POFY+POME form a nor latch
  /*#p24.RUJU*/ Gate RUJU;                    // AxxxxFxx
  /*#p24.POFY*/ Gate POFY;                    // AxxxxFxx
  /*#p24.POME*/ Gate POME;                    // AxxxxFxx
  /*_p24.PAHO*/ DFF17 PAHO_X_8_SYNC;          // xBxDxFxH
  /*#p21.WUSA*/ NorLatch WUSA_LCD_CLOCK_GATE; // xBxDxFGH High on G at beginning of line, low on H at end of line. Not sure what's up with the others. Scroll/sprite count?

  Gate REMY_LD0n;
  Gate RAVO_LD1n;

  /*_PIN_50*/ PinOut PIN_50_LCD_DATA1;
  /*_PIN_51*/ PinOut PIN_51_LCD_DATA0;
  /*_PIN_54*/ PinOut PIN_54_LCD_HSYNC;
  /*_PIN_56*/ PinOut PIN_56_LCD_FLIPS;
  /*_PIN_52*/ PinOut PIN_52_LCD_CNTRL;
  /*_PIN_55*/ PinOut PIN_55_LCD_LATCH;
  /*_PIN_53*/ PinOut PIN_53_LCD_CLOCK;
  /*_PIN_57*/ PinOut PIN_57_LCD_VSYNC;
};

//-----------------------------------------------------------------------------
