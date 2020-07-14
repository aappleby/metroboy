#include "LCD.h"
#include "Constants.h"

#pragma warning(disable:4189)

bool posedge(bool& old_v, bool new_v) {
  bool ret = !old_v && new_v;
  old_v = new_v;
  return ret;
}

//-----------------------------------------------------------------------------

void LCD::reset() {
  X = 99;
  Y = 0;
  _RUTU_LINE_ENDp = 0;
  NYPE_LINE_STARTp = 0;
  CATU_VID_LINE_ENDp = 0;
  _ANEL_VID_LINE_ENDp = 0;
  MYTA_LINE_153p = 1;
  MEDA_VSYNC_OUTn = 0;
  POPU_IN_VBLANKp = 0;
  _PAHO_X_8_SYNC = 0;
}

//-----------------------------------------------------------------------------

void LCD::tick(const Req& req, Ack& ack) {
  if (req.read && req.addr == 0xFF44) {
    ack.addr = req.addr;
    ack.data_lo = Y;
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

void LCD::tock(int phase, const Req& /*req*/, bool XONA_LCDC_EN) {
  bool IN_VBLANK = (Y & 144) == 144;
  bool VID_LINE_d0 = _RUTU_LINE_ENDp && !IN_VBLANK;
  bool LINE_000n = Y != 0;
  bool LINE_153 = (Y & 153) == 153;
  bool LINE_END = (X & 113) == 113;

  if (PHASE_B) {
    X++;
    if (_RUTU_LINE_ENDp && !NYPE_LINE_STARTp) {
      MYTA_LINE_153p = LINE_153;
      MEDA_VSYNC_OUTn = LINE_000n;
      POPU_IN_VBLANKp = IN_VBLANK;
    }
    NYPE_LINE_STARTp = _RUTU_LINE_ENDp;
  }

  if (PHASE_F) {
    if (LINE_END && !_RUTU_LINE_ENDp) Y++;
    _RUTU_LINE_ENDp = LINE_END;
  }

  if (PHASE_B || PHASE_F) {
    CATU_VID_LINE_ENDp = VID_LINE_d0;
  }

  if (PHASE_D || PHASE_H) {
    _ANEL_VID_LINE_ENDp = CATU_VID_LINE_ENDp;
  }

  if (_RUTU_LINE_ENDp) X = 0;
  if (MYTA_LINE_153p) Y = 0;

  if (!XONA_LCDC_EN) {
    X = 0;
    Y = 0;
    _RUTU_LINE_ENDp = 0;
    NYPE_LINE_STARTp = 0;
    MYTA_LINE_153p = 0;
    MEDA_VSYNC_OUTn = 0;
    POPU_IN_VBLANKp = 0;
  }

  // the !LCDC_EN here seems weird
  VID_LINE_TRIG_d4 = (CATU_VID_LINE_ENDp && !_ANEL_VID_LINE_ENDp) || !XONA_LCDC_EN;
}

//-----------------------------------------------------------------------------

void LCD::dump(Dumper& dump) const {
  dump("\002--------------LCD--------------\001\n");

  dump("X            = %d\n", X);
  dump("Y            = %d\n", Y);
  dump("_RUTU_LINE_ENDp  = %d\n", _RUTU_LINE_ENDp);
  dump("NYPE_LINE_STARTp  = %d\n", NYPE_LINE_STARTp);
  dump("CATU_VID_LINE_ENDp  = %d\n", CATU_VID_LINE_ENDp);
  dump("_ANEL_VID_LINE_ENDp  = %d\n", _ANEL_VID_LINE_ENDp);
  dump("MYTA_LINE_153p  = %d\n", MYTA_LINE_153p);
  dump("MEDA_VSYNC_OUTn   = %d\n", MEDA_VSYNC_OUTn);
  dump("POPU_IN_VBLANKp = %d\n", POPU_IN_VBLANKp);
}

//-----------------------------------------------------------------------------
