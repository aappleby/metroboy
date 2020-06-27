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
  RUTU_NEW_LINE_d0 = 0;
  NYPE_NEW_LINE_d4 = 0;
  CATU_VID_LINE_d4 = 0;
  ANEL_VID_LINE_d6 = 0;
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
  bool VID_LINE_d0 = RUTU_NEW_LINE_d0 && !IN_VBLANK;
  bool LINE_000n = Y != 0;
  bool LINE_153 = (Y & 153) == 153;
  bool LINE_END = (X & 113) == 113;

  if (PHASE_B) {
    X++;
    if (RUTU_NEW_LINE_d0 && !NYPE_NEW_LINE_d4) {
      MYTA_LINE_153_d4 = LINE_153;
      MEDA_VSYNC_OUTn = LINE_000n;
      POPU_VBLANK_d4 = IN_VBLANK;
    }
    NYPE_NEW_LINE_d4 = RUTU_NEW_LINE_d0;
  }

  if (PHASE_F) {
    if (LINE_END && !RUTU_NEW_LINE_d0) Y++;
    RUTU_NEW_LINE_d0 = LINE_END;
  }

  if (PHASE_B || PHASE_F) {
    CATU_VID_LINE_d4 = VID_LINE_d0;
  }

  if (PHASE_D || PHASE_H) {
    ANEL_VID_LINE_d6 = CATU_VID_LINE_d4;
  }

  if (RUTU_NEW_LINE_d0) X = 0;
  if (MYTA_LINE_153_d4) Y = 0;

  if (!XONA_LCDC_EN) {
    X = 0;
    Y = 0;
    RUTU_NEW_LINE_d0 = 0;
    NYPE_NEW_LINE_d4 = 0;
    MYTA_LINE_153_d4 = 0;
    MEDA_VSYNC_OUTn = 0;
    POPU_VBLANK_d4 = 0;
  }

  // the !LCDC_EN here seems weird
  VID_LINE_TRIG_d4 = (CATU_VID_LINE_d4 && !ANEL_VID_LINE_d6) || !XONA_LCDC_EN;
}

//-----------------------------------------------------------------------------

void LCD::dump(std::string& d) {
  sprintf(d, "\002--------------LCD--------------\001\n");

  sprintf(d, "X            = %d\n", X);
  sprintf(d, "Y            = %d\n", Y);
  sprintf(d, "RUTU_NEW_LINE_d0  = %d\n", RUTU_NEW_LINE_d0);
  sprintf(d, "NYPE_NEW_LINE_d4  = %d\n", NYPE_NEW_LINE_d4);
  sprintf(d, "CATU_VID_LINE_d4  = %d\n", CATU_VID_LINE_d4);
  sprintf(d, "ANEL_VID_LINE_d6  = %d\n", ANEL_VID_LINE_d6);
  sprintf(d, "MYTA_LINE_153_d4  = %d\n", MYTA_LINE_153_d4);
  sprintf(d, "MEDA_VSYNC_OUTn   = %d\n", MEDA_VSYNC_OUTn);
  sprintf(d, "POPU_VBLANK_d4 = %d\n", POPU_VBLANK_d4);
}

//-----------------------------------------------------------------------------
