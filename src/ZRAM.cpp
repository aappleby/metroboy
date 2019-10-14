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

void ZRAM::tock(const int tcycle_, const Bus bus_to_zram_) {
  const int tphase = tcycle_ & 3;
  if (tphase != 0) return;

  tcycle = tcycle_;
  bus_to_zram = bus_to_zram_;
  zram_to_bus = {};

  bool hit = (ADDR_ZEROPAGE_BEGIN <= bus_to_zram.addr && bus_to_zram.addr <= ADDR_ZEROPAGE_END);

  if (hit) {
    zram_to_bus = bus_to_zram;
    zram_to_bus.ack = true;
  }

  if (bus_to_zram.write) {
    if (ADDR_ZEROPAGE_BEGIN <= bus_to_zram.addr && bus_to_zram.addr <= ADDR_ZEROPAGE_END) {
      ram[bus_to_zram.addr - ADDR_ZEROPAGE_BEGIN] = (uint8_t)bus_to_zram.data;
    }
  }
  else if (bus_to_zram.read) {
    if (ADDR_ZEROPAGE_BEGIN <= bus_to_zram.addr && bus_to_zram.addr <= ADDR_ZEROPAGE_END) {
      zram_to_bus.data = ram[bus_to_zram.addr - ADDR_ZEROPAGE_BEGIN];
    }
  }
}

//-----------------------------------------------------------------------------

void ZRAM::dump(std::string& d) {
  sprintf(d, "tcycle %d\n", tcycle);
  print_bus(d, "bus_to_zram", bus_to_zram);
  print_bus(d, "zram_to_bus", zram_to_bus);
}

//-----------------------------------------------------------------------------
