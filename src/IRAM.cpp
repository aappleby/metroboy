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
  tphase = tcycle_ & 3;
  bus_to_iram = bus_to_iram_;
  dma_to_iram = dma_to_iram_;
  iram_to_bus = {};
  iram_to_dma = {};

  if (dma_to_iram.read) {
    if (ADDR_IRAM_BEGIN <= dma_to_iram.addr && dma_to_iram.addr <= ADDR_IRAM_END) {
      iram_to_dma.addr = dma_to_iram.addr;
      iram_to_dma.data = ram[dma_to_iram.addr - ADDR_IRAM_BEGIN];
      iram_to_dma.read = true;
      iram_to_dma.write = false;
      iram_to_dma.lock = false;
      iram_to_dma.dma = true;
      iram_to_dma.ack = true;
    }
    else if (ADDR_ECHO_BEGIN <= dma_to_iram.addr && dma_to_iram.addr <= ADDR_ECHO_END) {
      iram_to_dma.addr = dma_to_iram.addr;
      iram_to_dma.data = ram[dma_to_iram.addr - ADDR_ECHO_BEGIN];
      iram_to_dma.read = true;
      iram_to_dma.write = false;
      iram_to_dma.lock = false;
      iram_to_dma.dma = true;
      iram_to_dma.ack = true;
    }   
  }
  else if (bus_to_iram.write) {
    if (ADDR_IRAM_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_IRAM_END) {
      ram[bus_to_iram.addr - ADDR_IRAM_BEGIN] = (uint8_t)bus_to_iram.data;

      iram_to_bus.addr = bus_to_iram.addr;
      iram_to_bus.data = bus_to_iram.data;
      iram_to_bus.read = false;
      iram_to_bus.write = true;
      iram_to_bus.lock = false;
      iram_to_bus.dma = false;
      iram_to_bus.ack = true;
    }
    else if (ADDR_ECHO_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_ECHO_END) {
      ram[bus_to_iram.addr - ADDR_ECHO_BEGIN] = (uint8_t)bus_to_iram.data;

      iram_to_bus.addr = bus_to_iram.addr;
      iram_to_bus.data = bus_to_iram.data;
      iram_to_bus.read = false;
      iram_to_bus.write = true;
      iram_to_bus.lock = false;
      iram_to_bus.dma = false;
      iram_to_bus.ack = true;
    }
  }
  else if (bus_to_iram.read) {
    if (ADDR_IRAM_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_IRAM_END) {
      iram_to_bus.addr = bus_to_iram.addr;
      iram_to_bus.data = ram[bus_to_iram.addr - ADDR_IRAM_BEGIN];
      iram_to_bus.read = true;
      iram_to_bus.write = false;
      iram_to_bus.lock = false;
      iram_to_bus.dma = false;
      iram_to_bus.ack = true;
    }
    else if (ADDR_ECHO_BEGIN <= bus_to_iram.addr && bus_to_iram.addr <= ADDR_ECHO_END) {
      iram_to_bus.addr = bus_to_iram.addr;
      iram_to_bus.data = ram[bus_to_iram.addr - ADDR_ECHO_BEGIN];
      iram_to_bus.read = true;
      iram_to_bus.write = false;
      iram_to_bus.lock = false;
      iram_to_bus.dma = false;
      iram_to_bus.ack = true;
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
