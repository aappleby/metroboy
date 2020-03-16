#include "OAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void OAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

OAM::Out OAM::tick() const {
  return {
    oam_to_bus,
    oam_to_ppu
  };
}

void OAM::tock(int tcycle_, const Bus bus_to_oam_, Bus dma_to_oam_, Bus ppu_to_oam_) {
  const int tphase = tcycle_ & 3;
  tcycle = tcycle_;
  bus_to_oam = bus_to_oam_;
  dma_to_oam = dma_to_oam_;
  ppu_to_oam = ppu_to_oam_;

  oam_to_bus = {};
  oam_to_ppu = {};

  // oam - cpu bus

  if (tphase == 0) {
    uint16_t addr = bus_to_oam.addr;
    uint8_t data = (uint8_t)bus_to_oam.data;
    bool ack = (ADDR_OAM_BEGIN <= addr && addr <= ADDR_OAM_END);
    if (ppu_to_oam.lock) ack = false;

    if (ack && bus_to_oam.write) {
      uint16_t d = ram[(addr - ADDR_OAM_BEGIN) >> 1];
      
      if (addr & 1) d = (d & 0x00FF) | (data << 8);
      else d = (d & 0xFF00) | (data << 0);
      
      ram[(addr - ADDR_OAM_BEGIN) >> 1] = d;
    }
    else if (ack && bus_to_oam.read) {
      uint16_t data16 = ram[(addr - ADDR_OAM_BEGIN) >> 1];
      data = uint16_t(addr & 1 ? (data16 >> 8) : (data16 & 0xFF));
    }

    oam_to_bus = {};
    if (ack) {
      oam_to_bus = bus_to_oam;
      oam_to_bus.data = data;
      oam_to_bus.ack = true;
    }
  }


  if (ppu_to_oam.lock) {
    if (ppu_to_oam.read) {
      oam_to_ppu.addr = ppu_to_oam.addr;
      oam_to_ppu.data = ram[(ppu_to_oam.addr - ADDR_OAM_BEGIN) >> 1];
      oam_to_ppu.read = true;
      oam_to_ppu.write = false;
      oam_to_ppu.lock = true;
      oam_to_ppu.dma = false;
      oam_to_ppu.ack = true;
    }
  }
  else if (dma_to_oam.write) {
    if (ADDR_OAM_BEGIN <= dma_to_oam.addr && dma_to_oam.addr <= ADDR_OAM_END) {
      uint16_t d = ram[(dma_to_oam.addr - ADDR_OAM_BEGIN) >> 1];
      if (dma_to_oam.addr & 1) d = (d & 0x00FF) | (dma_to_oam.data << 8);
      else                 d = (d & 0xFF00) | (dma_to_oam.data << 0);
      ram[(dma_to_oam.addr - ADDR_OAM_BEGIN) >> 1] = d;
    }
  }
}

void OAM::dump(std::string& d) const {
  print_bus(d, "bus_to_oam", bus_to_oam);
  print_bus(d, "oam_to_bus", oam_to_bus);
  sprintf(d, "\n");
  print_bus(d, "ppu_to_oam", ppu_to_oam);
  print_bus(d, "oam_to_ppu", oam_to_ppu);
  sprintf(d, "\n");
  print_bus(d, "dma_to_oam", dma_to_oam);
}

//-----------------------------------------------------------------------------
