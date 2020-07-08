#pragma once
#include "Types.h"

// lcd x/y, lyc match, stat mode & int

struct LCD {
  void reset();
  void tick(const Req& req, Ack& ack);
  void tock(int phase, const Req& req, bool XONA_LCDC_EN);

  void dump(Dumper& dump) const;

  uint8_t X;
  uint8_t Y;

  // states

  /*p21.RUTU*/ bool RUTU_LINE_END_F  = 0;
  /*p21.NYPE*/ bool NYPE_LINE_END_B  = 0;
  /*p29.CATU*/ bool CATU_LINE_END_B  = 0;
  /*p28.ANEL*/ bool ANEL_LINE_END_D  = 0;
  /*p21.MYTA*/ bool MYTA_LINE_153_d4  = 0;
  /*p24.MEDA*/ bool MEDA_VSYNC_OUTn   = 0;
  /*p24.MEDA*/ bool POPU_VBLANK_d4 = 0;
  /*p24.PAHO*/ bool PAHO_X_8_SYNC     = 0;

  // signals

  /*p28.ATEJ*/ bool VID_LINE_TRIG_d4 = 0;
};