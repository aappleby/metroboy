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
  tphase = tcycle_ & 3;
  bus_to_oam = bus_to_oam_;
  dma_to_oam = dma_to_oam_;
  ppu_to_oam = ppu_to_oam_;

  oam_to_bus = {};
  oam_to_ppu = {};

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
  else if (bus_to_oam.write) {
    if (ADDR_OAM_BEGIN <= bus_to_oam.addr && bus_to_oam.addr <= ADDR_OAM_END) {
      uint16_t d = ram[(bus_to_oam.addr - ADDR_OAM_BEGIN) >> 1];
      if (bus_to_oam.addr & 1) d = (d & 0x00FF) | (bus_to_oam.data << 8);
      else                 d = (d & 0xFF00) | (bus_to_oam.data << 0);
      ram[(bus_to_oam.addr - ADDR_OAM_BEGIN) >> 1] = d;

      oam_to_bus.addr = bus_to_oam.addr;
      oam_to_bus.data = bus_to_oam.data;
      oam_to_bus.read = false;
      oam_to_bus.write = true;
      oam_to_bus.lock = false;
      oam_to_bus.dma = bus_to_oam.dma;
      oam_to_bus.ack = true;
    }
  }
  else if (bus_to_oam.read) {
    if (ADDR_OAM_BEGIN <= bus_to_oam.addr && bus_to_oam.addr <= ADDR_OAM_END) {
      uint16_t data16 = ram[(bus_to_oam.addr - ADDR_OAM_BEGIN) >> 1];
      oam_to_bus.addr = bus_to_oam.addr;
      oam_to_bus.data = uint16_t(bus_to_oam.addr & 1 ? (data16 >> 8) : (data16 & 0xFF));
      oam_to_bus.read = true;
      oam_to_bus.write = false;
      oam_to_bus.lock = false;
      oam_to_bus.dma = bus_to_oam.dma;
      oam_to_bus.ack = true;
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
// https://github.com/furrtek/DMG-CPU-Inside/blob/master/Schematics/28_OAM.png

struct HardOam {

  bool YFEL = 0;
  bool WEWY = 0;
  bool GOSO = 0;
  bool ELYN = 0;
  bool FAHA = 0;
  bool FONY = 0;

  void tick(bool ANOM, bool XUPY) {
    bool FETO = (YFEL && WEWY && FONY && GOSO);
    bool GAVA = (FETO || XUPY);

    // clk
    if (GAVA) {
      if (ANOM) {
        YFEL = 0;
        WEWY = 0;
        GOSO = 0;
        ELYN = 0;
        FAHA = 0;
        FONY = 0;
      } else {
        YFEL = !YFEL;
        if (!YFEL) {
          WEWY = !WEWY;
          if (!WEWY) {
            GOSO = !GOSO;
            if (!GOSO) {
              ELYN = !ELYN;
              if (!ELYN) {
                FAHA = !FAHA;
                if (!FAHA) {
                  FONY = !FONY;
                }
              }
            }
          }
        }
      }
    }
  }
};


//-----------------------------------------------------------------------------
