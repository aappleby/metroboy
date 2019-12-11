#pragma once
#include "Schematics.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Hram {
  //----------
  // high ram bus

  bool HRAM_CS;

#if 0
  // addr >= FF80 and not XXFF
  /*p07.WALE*/ wire ADDR_x1111111n = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
  /*p07.WOLY*/ wire WOLY = nand(b.sys.ADDR_FFXX, b.A07, ADDR_x1111111n);
  /*p07.WUTA*/ c.hrm.HRAM_CS = not(WOLY);
#endif

};

//-----------------------------------------------------------------------------

};