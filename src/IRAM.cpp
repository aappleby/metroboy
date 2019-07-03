#include "Platform.h"
#include "IRAM.h"

#include "Constants.h"

void IRAM::reset() {
  memset(ram, 0, sizeof(ram));
}

BusOut IRAM::tock_t2(CpuBus bus) {
  BusOut out = { 0,0 };

  if (bus.read) {
    if (ADDR_IRAM_BEGIN <= bus.addr && bus.addr <= ADDR_IRAM_END) {
      out.data = ram[bus.addr - ADDR_IRAM_BEGIN];
      out.oe = true;
    }
    else if (ADDR_ECHO_BEGIN <= bus.addr && bus.addr <= ADDR_ECHO_END) {
      out.data = ram[bus.addr - ADDR_ECHO_BEGIN];
      out.oe = true;
    }   
  }

  if (bus.write) {
    if (ADDR_IRAM_BEGIN <= bus.addr && bus.addr <= ADDR_IRAM_END) {
      ram[bus.addr - ADDR_IRAM_BEGIN] = bus.data;
    }
    else if (ADDR_ECHO_BEGIN <= bus.addr && bus.addr <= ADDR_ECHO_END) {
      ram[bus.addr - ADDR_ECHO_BEGIN] = bus.data;
    }
  }

  return out;
}
