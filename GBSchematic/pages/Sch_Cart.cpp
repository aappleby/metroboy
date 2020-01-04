#include "Sch_Cart.h"
#include "Sch_BusControl.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Cart::pwron() {
}

void Cart::reset() {
}

void Cart::phase_begin() {
}

void Cart::phase_end() {
}

void Cart::pass_begin() {
}

bool Cart::pass_end() {
  return false;
}

void Cart::tick(bool CLK, bool /*WRn*/, bool RDn, bool CSn, bool /*RSTn*/, BusTristates& bus_tri) {
  if (!old_clk && CLK) {
    addr = (uint16_t)bus_tri.get_addr();
  }

  if (!RDn && !CSn) {
    if (addr >= 0 && addr <= 0x7FFF) {
      bus_tri.set_data(rom[addr]);
    }
  }

  old_clk = CLK;
}


//-----------------------------------------------------------------------------

};