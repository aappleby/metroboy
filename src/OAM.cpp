#include "OAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void OAM::reset() {
  out = {0};
  for (int i = 0; i < 256; i++) ram[i] = 0;
}

BusOut OAM::tick() const {
  return out;
}

void OAM::tock(CpuBus bus) {
  if (bus.read && ADDR_OAM_BEGIN <= bus.addr && bus.addr <= ADDR_OAM_END) {
    out.data = ram[bus.addr - ADDR_OAM_BEGIN];
    out.oe = true;
  }

  if (bus.write && ADDR_OAM_BEGIN <= bus.addr && bus.addr <= ADDR_OAM_END) {
    ram[bus.addr - ADDR_OAM_BEGIN] = bus.data;
  }
}

void OAM::dump(std::string& /*out*/) const {
  /*
  for (int i = 0; i < 10; i++) {
    sprintf(out, "%2x ", ram[i]);
  }
  sprintf(out, "\n");
  */
}

//-----------------------------------------------------------------------------
