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
  RUTU_LINE_END_F = 0;
  NYPE_LINE_END_B = 0;
  CATU_LINE_END_B = 0;
  ANEL_LINE_END_D = 0;
  MYTA_LINE_153_d4 = 1;
  MEDA_VSYNC_OUTn = 0;
  POPU_VBLANK_d4 = 0;
  PAHO_X_8_SYNC = 0;
}

//-----------------------------------------------------------------------------

void LCD::tick(const Req& req, Ack& ack) {
  if (req.read && req.addr == 0xFF44) {
    ack.addr = req.addr;
    ack.data = Y;
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

void LCD::tock(int phase, const Req& /*req*/, bool XONA_LCDC_EN) {
  bool IN_VBLANK = (Y & 144) == 144;
  bool VID_LINE_d0 = RUTU_LINE_END_F && !IN_VBLANK;
  bool LINE_000n = Y != 0;
  bool LINE_153 = (Y & 153) == 153;
  bool LINE_END = (X & 113) == 113;

  if (PHASE_B) {
    X++;
    if (RUTU_LINE_END_F && !NYPE_LINE_END_B) {
      MYTA_LINE_153_d4 = LINE_153;
      MEDA_VSYNC_OUTn = LINE_000n;
      POPU_VBLANK_d4 = IN_VBLANK;
    }
    NYPE_LINE_END_B = RUTU_LINE_END_F;
  }

  if (PHASE_F) {
    if (LINE_END && !RUTU_LINE_END_F) Y++;
    RUTU_LINE_END_F = LINE_END;
  }

  if (PHASE_B || PHASE_F) {
    CATU_LINE_END_B = VID_LINE_d0;
  }

  if (PHASE_D || PHASE_H) {
    ANEL_LINE_END_D = CATU_LINE_END_B;
  }

  if (RUTU_LINE_END_F) X = 0;
  if (MYTA_LINE_153_d4) Y = 0;

  if (!XONA_LCDC_EN) {
    X = 0;
    Y = 0;
    RUTU_LINE_END_F = 0;
    NYPE_LINE_END_B = 0;
    MYTA_LINE_153_d4 = 0;
    MEDA_VSYNC_OUTn = 0;
    POPU_VBLANK_d4 = 0;
  }

  // the !LCDC_EN here seems weird
  VID_LINE_TRIG_d4 = (CATU_LINE_END_B && !ANEL_LINE_END_D) || !XONA_LCDC_EN;
}

//-----------------------------------------------------------------------------

void LCD::dump(Dumper& dump) const {
  dump("\002--------------LCD--------------\001\n");

  dump("X            = %d\n", X);
  dump("Y            = %d\n", Y);
  dump("RUTU_LINE_END_F  = %d\n", RUTU_LINE_END_F);
  dump("NYPE_LINE_END_B  = %d\n", NYPE_LINE_END_B);
  dump("CATU_LINE_END_B  = %d\n", CATU_LINE_END_B);
  dump("ANEL_LINE_END_D  = %d\n", ANEL_LINE_END_D);
  dump("MYTA_LINE_153_d4  = %d\n", MYTA_LINE_153_d4);
  dump("MEDA_VSYNC_OUTn   = %d\n", MEDA_VSYNC_OUTn);
  dump("POPU_VBLANK_d4 = %d\n", POPU_VBLANK_d4);
}

//-----------------------------------------------------------------------------
