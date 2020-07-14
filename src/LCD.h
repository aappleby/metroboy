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

  /*p21.RUTU*/ bool _RUTU_LINE_ENDp  = 0;
  /*p21.NYPE*/ bool NYPE_LINE_STARTp  = 0;
  /*p29.CATU*/ bool CATU_VID_LINE_ENDp  = 0;
  /*p28.ANEL*/ bool _ANEL_VID_LINE_ENDp  = 0;
  /*p21.MYTA*/ bool MYTA_LINE_153p  = 0;
  /*p24.MEDA*/ bool MEDA_VSYNC_OUTn   = 0;
  /*p24.MEDA*/ bool POPU_IN_VBLANKp = 0;
  /*p24.PAHO*/ bool _PAHO_X_8_SYNC     = 0;

  // signals

  /*p28.ATEJ*/ bool VID_LINE_TRIG_d4 = 0;
};