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
  NEW_LINE_d0 = 0;
  NEW_LINE_d4 = 0;
  LINE_153_d4 = 1;
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
// this bit matches the die

void LCD::tock(int phase, const Req& /*req*/, bool LCDC_EN) {
  if (PHASE_B) {
    X++;
    if (NEW_LINE_d0 && !NEW_LINE_d4) LINE_153_d4 = (Y == 153);
    NEW_LINE_d4 = NEW_LINE_d0;
  }

  if (PHASE_F) {
    if (X == 113 && !NEW_LINE_d0) Y++;
    NEW_LINE_d0 = (X == 113);
  }

  if (NEW_LINE_d0) X = 0;
  if (LINE_153_d4) Y = 0;

  if (!LCDC_EN) {
    X = 0;
    Y = 0;
    NEW_LINE_d0 = 0;
    NEW_LINE_d4 = 0;
    LINE_153_d4 = 0;
  }
}

//-----------------------------------------------------------------------------

void LCD::dump(std::string& d) {
  sprintf(d, "\002--------------LCD--------------\001\n");

  sprintf(d, "X            = %d\n", X);
  sprintf(d, "Y            = %d\n", Y);
  sprintf(d, "NEW_LINE_d0  = %d\n", NEW_LINE_d0);
  sprintf(d, "NEW_LINE_d4  = %d\n", NEW_LINE_d4);
  sprintf(d, "LINE_153_d4  = %d\n", LINE_153_d4);
}

//-----------------------------------------------------------------------------
