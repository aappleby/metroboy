#include "VRAM.h"

#include "Constants.h"

extern uint8_t bootrom_logo[];

//-----------------------------------------------------------------------------

void VRAM::reset() {
  *this = {};
  memcpy(ram, bootrom_logo, 416);
}

//-----------------------------------------------------------------------------

VRAM::Out VRAM::tick() const {
  return {
    vram_to_bus,
    vram_to_dma,
    vram_to_ppu,
  };
}

void VRAM::tock(int tcycle_, Bus bus_to_vram_, Bus dma_to_vram_, Bus ppu_to_vram_) {
  tcycle = tcycle_;
  bus_to_vram = bus_to_vram_;
  dma_to_vram = dma_to_vram_;
  ppu_to_vram = ppu_to_vram_;

  vram_to_bus = {};
  vram_to_dma = {};
  vram_to_ppu = {};

  if (ppu_to_vram.lock) {
    if (ppu_to_vram.read) {
      vram_to_ppu.addr  = ppu_to_vram.addr;
      vram_to_ppu.data  = ram[ppu_to_vram.addr - ADDR_VRAM_BEGIN];
      vram_to_ppu.read  = true;
      vram_to_ppu.write = false;
      vram_to_ppu.lock  = true;
      vram_to_ppu.dma   = false;
      vram_to_ppu.ack   = true;
    }
  }
  else {
    if (dma_to_vram.read) {
      if (ADDR_VRAM_BEGIN <= dma_to_vram.addr && dma_to_vram.addr <= ADDR_VRAM_END) {
        vram_to_dma.addr  = dma_to_vram.addr;
        vram_to_dma.data  = ram[dma_to_vram.addr - ADDR_VRAM_BEGIN];
        vram_to_dma.read  = true;
        vram_to_dma.write = false;
        vram_to_dma.lock  = false;
        vram_to_dma.dma   = true;
        vram_to_dma.ack   = true;
      }
    }
    else if (bus_to_vram.write) {
      if (ADDR_VRAM_BEGIN <= bus_to_vram.addr && bus_to_vram.addr <= ADDR_VRAM_END) {
        ram[bus_to_vram.addr - ADDR_VRAM_BEGIN] = uint8_t(bus_to_vram.data);

        vram_to_bus.addr  = bus_to_vram.addr;
        vram_to_bus.data  = bus_to_vram.data;
        vram_to_bus.read  = false;
        vram_to_bus.write = true;
        vram_to_bus.lock  = false;
        vram_to_bus.dma   = false;
        vram_to_bus.ack   = true;
      }
    }
    else if (bus_to_vram.read) {
      if (ADDR_VRAM_BEGIN <= bus_to_vram.addr && bus_to_vram.addr <= ADDR_VRAM_END) {
        vram_to_bus.addr  = bus_to_vram.addr;
        vram_to_bus.data  = ram[bus_to_vram.addr - ADDR_VRAM_BEGIN];
        vram_to_bus.read  = true;
        vram_to_bus.write = false;
        vram_to_bus.lock  = false;
        vram_to_bus.dma   = false;
        vram_to_bus.ack   = true;
      }
    }
  }
}

//-----------------------------------------------------------------------------

void VRAM::dump(std::string& d) {
  print_bus(d, "bus_to_vram", bus_to_vram);
  print_bus(d, "vram_to_bus", vram_to_bus);
  sprintf(d, "\n");

  print_bus(d, "dma_to_vram", dma_to_vram);
  print_bus(d, "vram_to_dma", vram_to_dma);
  sprintf(d, "\n");

  print_bus(d, "ppu_to_vram", ppu_to_vram);
  print_bus(d, "vram_to_ppu", vram_to_ppu);
}

//-----------------------------------------------------------------------------
