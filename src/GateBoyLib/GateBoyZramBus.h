#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct GateBoyZram {
  void reset();

  Gate clk_old;
};

//-----------------------------------------------------------------------------
