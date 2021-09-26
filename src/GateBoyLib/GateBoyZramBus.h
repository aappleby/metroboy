#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct GateBoyZram {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  Gate clk_old;
};

//-----------------------------------------------------------------------------
