#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct LCDControl {
  void reset_to_cart();

  // H deltas are due to reg writes
  /*#p29.CATU*/ DFF17 CATU_LINE_ENDp_odd; // Axxxxxxx
  /*#p28.ANEL*/ DFF17 ANEL_LINE_ENDp_odd; // xxCxxxxx
  /*#p21.POPU*/ DFF17 POPU_VBLANKp_odd; // xxCxxxxH
  /*#p21.MYTA*/ DFF17 MYTA_FRAME_ENDp_odd; // xxCxxxxH
  /*#p21.RUTU*/ DFF17 RUTU_LINE_ENDp_odd; // xxxxxxGx
  /*#p21.NYPE*/ DFF17 NYPE_LINE_ENDp_odd; // xxCxxxxx

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
