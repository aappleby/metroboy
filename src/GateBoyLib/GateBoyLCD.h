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
    POME_X8_LATCH.state                = BIT_OLD | BIT_DRIVEN;
    PAHO_X8_SYNC.state       = BIT_OLD | BIT_DRIVEN;
    WUSA_LCD_CLOCK_GATE.state = BIT_OLD | BIT_DRIVEN;
    REMY_LD0n.state           = BIT_OLD | BIT_DRIVEN;
    RAVO_LD1n.state           = BIT_OLD | BIT_DRIVEN;
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
    POME_X8_LATCH.state                = BIT_OLD | BIT_DRIVEN | 1;
    PAHO_X8_SYNC.state       = BIT_OLD | BIT_DRIVEN;
    WUSA_LCD_CLOCK_GATE.state = BIT_OLD | BIT_DRIVEN;
    REMY_LD0n.state           = BIT_OLD | BIT_DRIVEN | 1;
    RAVO_LD1n.state           = BIT_OLD | BIT_DRIVEN | 1;
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
    POME_X8_LATCH.state                = 0b00011001;
    PAHO_X8_SYNC.state       = 0b00011000;
    WUSA_LCD_CLOCK_GATE.state = 0b00011000;
    REMY_LD0n.state           = 0b00011000;
    RAVO_LD1n.state           = 0b00011000;
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
  /*#p24.POME*/ Gate POME_X8_LATCH;                    // AxxxxFxx
  /*_p24.PAHO*/ DFF17 PAHO_X8_SYNC;          // xBxDxFxH
  /*#p21.WUSA*/ NorLatch WUSA_LCD_CLOCK_GATE; // xBxDxFGH High on G at beginning of line, low on H at end of line. Not sure what's up with the others. Scroll/sprite count?

  Gate REMY_LD0n;
  Gate RAVO_LD1n;
};

//-----------------------------------------------------------------------------
