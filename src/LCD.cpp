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
  VID_LINE_d4 = 0;
  VID_LINE_d6 = 0;
  LINE_153_d4 = 1;
  VSYNC_OUTn = 0;
  IN_VBLANK_d4 = 0;
  X_8_SYNC = 0;
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

#if 0
// ly match
{
  if (PHASE_B) {
    LY_MATCH_SYNC = LY == LYC;
  }

  /*p21.SEPA*/ wire FF41_WR = and (CUPA_BUS_WR_xxxxxFGH, req.addr == 0xFF41);


  // PAGO00 << WESY
  // PAGO01 << RYJU
  // PAGO02 nc
  // PAGO03 >> RUPO

  // WESY polarity wrong? nope.

  /*p21.ROPO*/ vid_reg.LY_MATCH_SYNC.set(TALU_xBCDExxx, WESY_RSTn, LY_MATCHa);

  /*p21.PAGO*/ wire PAGO_LYC_MATCH_RST = nor(WESY_RSTn, !FF41_WR);

  // LONY00 << LURY03
  // LONY01 nc
  // LONY02 == nc
  // LONY03 >> LUSU00, MYMA00
  // LONY04 nc
  // LONY05 << NYXU03

  // RUPO has "arms" on the ground side
  // LONY has "arms" on the VCC side
  // XYMU has "arms" on the ground side
  // WUSA has "arms" on the ground side

  // RUPO00 << ROPO16
  // RUPO01 nc
  // RUPO02 >> SEGO03
  // RUPO03 >> nc
  // RUPO04 nc
  // RUPO05 << PAGO03


  /*p21.RUPO*/ LYC_MATCH_LATCHn.sr_latch(LY_MATCH_SYNC, PAGO_LYC_MATCH_RST);

}
#endif

void LCD::tock(int phase, const Req& /*req*/, bool LCDC_EN) {
  /*p21.XYVO*/ bool IN_VBLANK = (Y & 144) == 144;
  /*p29.ABOV*/ bool VID_LINE_d0 = NEW_LINE_d0 && !IN_VBLANK;
  /*p24.NERU*/ bool LINE_000n = Y != 0;
  /*p21.NOKO*/ bool LINE_153 = (Y & 153) == 153;
  /*p21.SANU*/ bool LINE_END = (X & 113) == 113;

  if (PHASE_B) {
    X++;
    if (NEW_LINE_d0 && !NEW_LINE_d4) {
      LINE_153_d4 = LINE_153;
      VSYNC_OUTn = LINE_000n;
      IN_VBLANK_d4 = IN_VBLANK;
    }
    NEW_LINE_d4 = NEW_LINE_d0;
  }

  if (PHASE_F) {
    if (LINE_END && !NEW_LINE_d0) Y++;
    NEW_LINE_d0 = LINE_END;
  }

  if (PHASE_B || PHASE_F) {
    VID_LINE_d4 = VID_LINE_d0;
  }

  if (PHASE_D || PHASE_H) {
    VID_LINE_d6 = VID_LINE_d4;
  }

  if (NEW_LINE_d0) X = 0;
  if (LINE_153_d4) Y = 0;

  if (!LCDC_EN) {
    X = 0;
    Y = 0;
    NEW_LINE_d0 = 0;
    NEW_LINE_d4 = 0;
    LINE_153_d4 = 0;
    VSYNC_OUTn = 0;
    IN_VBLANK_d4 = 0;
  }

  // the !LCDC_EN here seems weird
  VID_LINE_TRIG_d4 = (VID_LINE_d4 && !VID_LINE_d6) || !LCDC_EN;
}

//-----------------------------------------------------------------------------

void LCD::dump(std::string& d) {
  sprintf(d, "\002--------------LCD--------------\001\n");

  sprintf(d, "X            = %d\n", X);
  sprintf(d, "Y            = %d\n", Y);
  sprintf(d, "NEW_LINE_d0  = %d\n", NEW_LINE_d0);
  sprintf(d, "NEW_LINE_d4  = %d\n", NEW_LINE_d4);
  sprintf(d, "VID_LINE_d4  = %d\n", VID_LINE_d4);
  sprintf(d, "VID_LINE_d6  = %d\n", VID_LINE_d6);
  sprintf(d, "LINE_153_d4  = %d\n", LINE_153_d4);
  sprintf(d, "VSYNC_OUTn   = %d\n", VSYNC_OUTn);
  sprintf(d, "IN_VBLANK_d4 = %d\n", IN_VBLANK_d4);
}

//-----------------------------------------------------------------------------
