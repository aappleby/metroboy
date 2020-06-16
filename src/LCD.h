#pragma once
#include "Types.h"

// lcd x/y, lyc match, stat mode & int

struct LCD {
  void reset();
  void tick(const Req& req, Ack& ack);
  void tock(int phase, const Req& req, bool LCDC_EN);

  void dump(std::string& d);

  uint8_t X;
  uint8_t Y;

  /*p21.RUTU*/ bool NEW_LINE_d0 = 0;
  /*p21.NYPE*/ bool NEW_LINE_d4 = 0;
  /*p21.MYTA*/ bool LINE_153_d4 = 0;
};