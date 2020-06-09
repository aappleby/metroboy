#include "OAM.h"

#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void OAM::reset() {
  memset(ram, 0, sizeof(ram));
}

//-----------------------------------------------------------------------------

void OAM::tock(int phase, const Req& req) {
  if (req.write && (req.addr >= ADDR_OAM_BEGIN) && (req.addr <= ADDR_OAM_END)) {
    uint16_t oam_addr = req.addr & 0x00FF;
    uint16_t d = ram[oam_addr >> 1];
    if (oam_addr & 1) d = (d & 0x00FF) | (req.data << 8);
    else              d = (d & 0xFF00) | (req.data << 0);
    ram[oam_addr >> 1] = d;
  }
  
}

void OAM::tick(int phase, const Req& req, Ack& ack) const {
  if (req.read && (req.addr >= ADDR_OAM_BEGIN) && (req.addr <= ADDR_OAM_END)) {
    ack.addr = req.addr;
    ack.data = ram[(req.addr & 0x00FF) >> 1];
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

void OAM::dump(std::string& d) const {
  sprintf(d, "\002--------------OAM--------------\001\n");

  uint8_t* flat = (uint8_t*)ram;
  for (int y = 0; y < 20; y++) {
    sprintf(d, "%04x: ", y * 8 + ADDR_OAM_BEGIN);
    for (int x = 0; x < 8; x++) {
      uint8_t b = flat[x + y * 8];
      uint8_t l = (b >> 0) & 0x0F;
      uint8_t h = (b >> 4) & 0x0F;

      d.push_back(h > 9 ? '7' + h : '0' + h);
      d.push_back(l > 9 ? '7' + l : '0' + l);
      d.push_back(' ');
    }
    d.push_back('\n');
  }

  /*
  for (int y = 0; y < 40; y++) {
    sprintf(d, "%04x: ", y * 8 + ADDR_OAM_BEGIN);
    for (int x = 0; x < 4; x++) {
      uint8_t b = flat[x + y * 4];
      uint8_t l = (b >> 0) & 0x0F;
      uint8_t h = (b >> 4) & 0x0F;

      d.push_back(h > 9 ? '7' + h : '0' + h);
      d.push_back(l > 9 ? '7' + l : '0' + l);
      d.push_back(' ');
    }
    d.push_back('\n');
  }
  */

  sprintf(d, "\n");
}

//-----------------------------------------------------------------------------



/*

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
*/