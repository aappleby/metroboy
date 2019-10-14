#include "IRAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void IRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

IRAM::Out IRAM::tick() const {
  return {
    iram_to_bus,
    iram_to_dma
  };
}

void IRAM::tock(int tcycle_, Bus bus_to_iram_, Bus dma_to_iram_) {
  const int tphase = tcycle_ & 3;
  if (tphase != 0) return;

  tcycle = tcycle_;
  bus_to_iram = bus_to_iram_;
  dma_to_iram = dma_to_iram_;
  iram_to_bus = {};
  iram_to_dma = {};

  if ((ADDR_IRAM_BEGIN <= dma_to_iram.addr && dma_to_iram.addr <= ADDR_IRAM_END) ||
      (ADDR_ECHO_BEGIN <= dma_to_iram.addr && dma_to_iram.addr <= ADDR_ECHO_END)) {
    iram_to_dma = dma_to_iram;
    iram_to_dma.ack = true;
  }

  if ((ADDR_IRAM_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_IRAM_END) ||
      (ADDR_ECHO_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_ECHO_END)) {
    iram_to_bus = bus_to_iram;
    iram_to_bus.ack = true;
  }

  if (dma_to_iram.read) {
    if (ADDR_IRAM_BEGIN <= dma_to_iram.addr && dma_to_iram.addr <= ADDR_IRAM_END) {
      iram_to_dma.data = ram[dma_to_iram.addr - ADDR_IRAM_BEGIN];
    }
    else if (ADDR_ECHO_BEGIN <= dma_to_iram.addr && dma_to_iram.addr <= ADDR_ECHO_END) {
      iram_to_dma.data = ram[dma_to_iram.addr - ADDR_ECHO_BEGIN];
    }   
  }
  else if (bus_to_iram.write) {
    if (ADDR_IRAM_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_IRAM_END) {
      ram[bus_to_iram.addr - ADDR_IRAM_BEGIN] = (uint8_t)bus_to_iram.data;
    }
    else if (ADDR_ECHO_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_ECHO_END) {
      ram[bus_to_iram.addr - ADDR_ECHO_BEGIN] = (uint8_t)bus_to_iram.data;
    }
  }
  else if (bus_to_iram.read) {
    if (ADDR_IRAM_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_IRAM_END) {
      iram_to_bus.data = ram[bus_to_iram.addr - ADDR_IRAM_BEGIN];
    }
    else if (ADDR_ECHO_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_ECHO_END) {
      iram_to_bus.data = ram[bus_to_iram.addr - ADDR_ECHO_BEGIN];
    }   
  }
}

//-----------------------------------------------------------------------------

void IRAM::dump(std::string& d) {
  print_bus(d, "bus_to_iram", bus_to_iram);
  print_bus(d, "iram_to_bus", iram_to_bus);
  sprintf(d, "\n");
  print_bus(d, "dma_to_iram", dma_to_iram);
  print_bus(d, "iram_to_dma", iram_to_dma);
}

//-----------------------------------------------------------------------------
