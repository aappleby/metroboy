#include "MetroBoyLib/OAM.h"

#include "CoreLib/Constants.h"
#include <assert.h>
#include <memory.h>

//-----------------------------------------------------------------------------

void OAM::reset() {
  memset(ram, 0, sizeof(ram));
}

//-----------------------------------------------------------------------------

void OAM::tock(const Req& req) {
  if (req.write && (req.addr >= ADDR_OAM_BEGIN) && (req.addr <= ADDR_OAM_END)) {
    uint16_t oam_addr = req.addr & 0x00FF;
    uint16_t d = ram[oam_addr >> 1];
    if (oam_addr & 1) d = (d & 0x00FF) | (req.data_lo << 8);
    else              d = (d & 0xFF00) | (req.data_lo << 0);
    ram[oam_addr >> 1] = d;
  }
}

void OAM::tick(const Req& req, Ack& ack) const {
  if (req.read && (req.addr >= ADDR_OAM_BEGIN) && (req.addr <= ADDR_OAM_END)) {
    ack.addr = req.addr;
    if (req.addr & 1) {
      ack.data = ram[(req.addr & 0x00FF) >> 1] >> 8;
    }
    else {
      ack.data = ram[(req.addr & 0x00FF) >> 1];
    }
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

void OAM::dump(Dumper& d) const {
  d("\002--------------OAM--------------\001\n");

  uint8_t* flat = (uint8_t*)ram;
  for (int y = 0; y < 20; y++) {
    d("%04x: ", y * 8 + ADDR_OAM_BEGIN);
    for (int x = 0; x < 8; x++) {
      uint8_t b = flat[x + y * 8];
      uint8_t l = (b >> 0) & 0x0F;
      uint8_t h = (b >> 4) & 0x0F;

      d("%c%c ", h > 9 ? '7' + h : '0' + h, l > 9 ? '7' + l : '0' + l);
    }
    d("\n");
  }

  d("\n");
}

//-----------------------------------------------------------------------------
