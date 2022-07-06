#pragma once

#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct GateBoyWaveBus {
  void reset_to_cart();

#if 0
  /*_BUS_WAVE_A00*/ Bus BUS_WAVE_A00;
  /*_BUS_WAVE_A01*/ Bus BUS_WAVE_A01;
  /*_BUS_WAVE_A02*/ Bus BUS_WAVE_A02;
  /*_BUS_WAVE_A03*/ Bus BUS_WAVE_A03;
#endif

  /*_BUS_WAVE_D00*/ Bus BUS_WAVE_D00;
  /*_BUS_WAVE_D01*/ Bus BUS_WAVE_D01;
  /*_BUS_WAVE_D02*/ Bus BUS_WAVE_D02;
  /*_BUS_WAVE_D03*/ Bus BUS_WAVE_D03;
  /*_BUS_WAVE_D04*/ Bus BUS_WAVE_D04;
  /*_BUS_WAVE_D05*/ Bus BUS_WAVE_D05;
  /*_BUS_WAVE_D06*/ Bus BUS_WAVE_D06;
  /*_BUS_WAVE_D07*/ Bus BUS_WAVE_D07;
};

//-----------------------------------------------------------------------------
