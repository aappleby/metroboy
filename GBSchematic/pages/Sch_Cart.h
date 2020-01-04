#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Cart {
  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  void tick(bool CLK, bool WRn, bool RDn, bool CSn, bool RSTn, BusTristates& bus_tri);

  uint16_t addr;
  bool old_clk;
  uint8_t rom[65536];
};

//-----------------------------------------------------------------------------

};