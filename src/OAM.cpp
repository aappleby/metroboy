#include "OAM.h"

#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void OAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

Ack OAM::on_obus_req(Req obus_req) {
  bool hit = (obus_req.addr >= ADDR_OAM_BEGIN) && (obus_req.addr <= ADDR_OAM_END);
  if (!hit) return {};

  if (hit && obus_req.write) {
    uint16_t oam_addr = obus_req.addr & 0x00FF;
    uint16_t d = ram[oam_addr >> 1];
    if (oam_addr & 1) d = (d & 0x00FF) | (obus_req.data << 8);
    else              d = (d & 0xFF00) | (obus_req.data << 0);
    ram[oam_addr >> 1] = d;

    return {
      .addr  = obus_req.addr,
      .data  = obus_req.data,
      .read  = 0,
      .write = 1,
    };
  }
  else if (hit && obus_req.read) {
    return {
      .addr  = obus_req.addr,
      .data  = ram[(obus_req.addr & 0x00FF) >> 1],
      .read  = 1,
      .write = 0,
    };
  }
  else {
    assert(false);
    return {};
  }
}

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