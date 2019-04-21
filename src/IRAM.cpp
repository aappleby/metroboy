#include "IRAM.h"
#include "constants.h"

#include <memory.h>

void IRAM::reset() {
  bus_out = 0x00;
  bus_oe = false;

  memset(ram, 0, sizeof(ram));
}

void IRAM::tock_t3(uint16_t addr, uint8_t data, bool read, bool write) {
  bus_out = 0x00;
  bus_oe = false;

  if (read) {
    if (ADDR_IRAM_BEGIN <= addr && addr <= ADDR_IRAM_END) {
      bus_out = ram[addr - ADDR_IRAM_BEGIN];
      bus_oe = true;
    }
    else if (ADDR_ECHO_BEGIN <= addr && addr <= ADDR_ECHO_END) {
      bus_out = ram[addr - ADDR_ECHO_BEGIN];
      bus_oe = true;
    }   
  }

  if (write) {
    if (ADDR_IRAM_BEGIN <= addr && addr <= ADDR_IRAM_END) {
      ram[addr - ADDR_IRAM_BEGIN] = data;
    }
    else if (ADDR_ECHO_BEGIN <= addr && addr <= ADDR_ECHO_END) {
      ram[addr - ADDR_ECHO_BEGIN] = data;
    }
  }
}
