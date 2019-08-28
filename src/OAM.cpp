#include "OAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

BusOut OAM::reset() {
  for (int i = 0; i < 256; i++) ram[i] = 0;
  return { 0 };
}

BusOut OAM::tock(CpuBus bus) {
  BusOut ret = { 0,false };

  if (bus.read && ADDR_OAM_BEGIN <= bus.addr && bus.addr <= ADDR_OAM_END) {
    ret.data = ram[bus.addr - ADDR_OAM_BEGIN];
    ret.oe = true;
  }

  if (bus.write && ADDR_OAM_BEGIN <= bus.addr && bus.addr <= ADDR_OAM_END) {
    ram[bus.addr - ADDR_OAM_BEGIN] = bus.data;
  }

  return ret;
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
