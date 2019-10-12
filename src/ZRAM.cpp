#include "ZRAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void ZRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

Bus ZRAM::tick() const {
  return zram_to_bus;
}

void ZRAM::tock(int tphase_, Bus bus_to_zram_) {
  tphase = tphase_;
  bus_to_zram = bus_to_zram_;
  zram_to_bus = {};

  if (bus_to_zram.write) {
    if (ADDR_ZEROPAGE_BEGIN <= bus_to_zram.addr && bus_to_zram.addr <= ADDR_ZEROPAGE_END) {
      ram[bus_to_zram.addr - ADDR_ZEROPAGE_BEGIN] = (uint8_t)bus_to_zram.data;
      zram_to_bus = { bus_to_zram.addr, bus_to_zram.data, false, true };
    }
  }
  else if (bus_to_zram.read) {
    if (ADDR_ZEROPAGE_BEGIN <= bus_to_zram.addr && bus_to_zram.addr <= ADDR_ZEROPAGE_END) {
      zram_to_bus = { bus_to_zram.addr, ram[bus_to_zram.addr - ADDR_ZEROPAGE_BEGIN], true, false };
    }
  }

}

//-----------------------------------------------------------------------------

void ZRAM::dump(std::string& d) {
  sprintf(d, "tphase %d\n", tphase);
  print_bus(d, "bus_to_zram", bus_to_zram);
  print_bus(d, "zram_to_bus", zram_to_bus);
}

//-----------------------------------------------------------------------------
