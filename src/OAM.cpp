#include "Platform.h"
#include "OAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void OAM::reset() {
  bus_out = 0x00;
  bus_oe = false;

  for (int i = 0; i < 256; i++) ram[i] = 0;
}

void OAM::tock(uint16_t addr, uint8_t data, bool read, bool write) {
  bus_out = 0x00;
  bus_oe = false;

  if (read && ADDR_OAM_BEGIN <= addr && addr <= ADDR_OAM_END) {
    bus_out = ram[addr - ADDR_OAM_BEGIN];
    bus_oe = true;
  }

  if (write && ADDR_OAM_BEGIN <= addr && addr <= ADDR_OAM_END) {
    ram[addr - ADDR_OAM_BEGIN] = data;
  }
}

void OAM::dump(std::string& out) {
  for (int i = 0; i < 10; i++) {
    sprintf(out, "%2x ", ram[i]);
  }
  sprintf(out, "\n");
}

//-----------------------------------------------------------------------------
