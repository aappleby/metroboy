#include "MetroBoyLib/LCD.h"
#include "CoreLib/Constants.h"

#pragma warning(disable:4189)

//-----------------------------------------------------------------------------

void LCD::reset() {
  X = 99;
  Y = 0;
  RUTU_LINE_P908 = 0;
  NYPE_LINE_STARTp = 0;
  CATU_LINE_ENDp = 0;
  ANEL_LINE_P000 = 0;
  MYTA_LINE_153p = 1;
  MEDA_VSYNC_OUTn = 0;
  POPU_IN_VBLANKp = 0;
  PAHO_X_8_SYNC = 0;
}

//-----------------------------------------------------------------------------

void LCD::tick(int phase_total, const Req& req, Ack& ack) {
  (void)phase_total;

  if (req.read && req.addr == 0xFF44) {
    ack.addr = req.addr;
    ack.data_lo = Y;
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

void LCD::tock(int phase_total, const Req& /*req*/, bool XONA_LCDC_LCDENn) {
  bool IN_VBLANK = (Y & 144) == 144;
  bool VID_LINE_d0 = RUTU_LINE_P908 && !IN_VBLANK;
  bool LINE_000n = Y != 0;
  bool LINE_153 = (Y & 153) == 153;
  bool LINE_END = (X & 113) == 113;

  if (DELTA_BC) {
    X++;
    if (RUTU_LINE_P908 && !NYPE_LINE_STARTp) {
      MYTA_LINE_153p = LINE_153;
      MEDA_VSYNC_OUTn = LINE_000n;
      POPU_IN_VBLANKp = IN_VBLANK;
    }
    NYPE_LINE_STARTp = RUTU_LINE_P908;
  }

  if (DELTA_FG) {
    if (LINE_END && !RUTU_LINE_P908) Y++;
    RUTU_LINE_P908 = LINE_END;
  }

  if (DELTA_BC || DELTA_FG) {
    CATU_LINE_ENDp = VID_LINE_d0;
  }

  if (DELTA_DE || DELTA_HA) {
    ANEL_LINE_P000 = CATU_LINE_ENDp;
  }

  if (RUTU_LINE_P908) X = 0;
  if (MYTA_LINE_153p) Y = 0;

  if (!XONA_LCDC_LCDENn) {
    X = 0;
    Y = 0;
    RUTU_LINE_P908 = 0;
    NYPE_LINE_STARTp = 0;
    MYTA_LINE_153p = 0;
    MEDA_VSYNC_OUTn = 0;
    POPU_IN_VBLANKp = 0;
  }

  // the !LCDC_EN here seems weird
  VID_LINE_TRIG_d4 = (CATU_LINE_ENDp && !ANEL_LINE_P000) || !XONA_LCDC_LCDENn;
}

//-----------------------------------------------------------------------------

void LCD::dump(Dumper& dump) const {
  dump("\002--------------LCD--------------\001\n");

  dump("LCD X = %d\n", X);
  dump("LCD Y = %d\n", Y);
  dump("RUTU_LINE_P908  = %d\n", RUTU_LINE_P908);
  dump("NYPE_LINE_P000  = %d\n", NYPE_LINE_STARTp);
  dump("CATU_LINE_ENDp  = %d\n", CATU_LINE_ENDp);
  dump("ANEL_LINE_P000  = %d\n", ANEL_LINE_P000);
  dump("MYTA_LINE_153p  = %d\n", MYTA_LINE_153p);
  dump("MEDA_VSYNC_OUTn = %d\n", MEDA_VSYNC_OUTn);
  dump("POPU_IN_VBLANKp = %d\n", POPU_IN_VBLANKp);
}

//-----------------------------------------------------------------------------
