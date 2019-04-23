#include "ZRAM.h"
#include "Constants.h"

void ZRAM::reset() {
  bus_out = 0x00;
  bus_oe = false;

  memset(ram, 0, sizeof(ram));
}

void ZRAM::tock(uint16_t addr, uint8_t data, bool read, bool write) {
  bus_out = 0x00;
  bus_oe = false;

  if (addr < ADDR_ZEROPAGE_BEGIN || ADDR_ZEROPAGE_END < addr) {
    return;
  }

  if (write) {
    ram[addr - ADDR_ZEROPAGE_BEGIN] = data;
  }

  if (read) {
    bus_out = ram[addr - ADDR_ZEROPAGE_BEGIN];
    bus_oe = true;
  }
}
