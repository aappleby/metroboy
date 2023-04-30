#include "MetroBoyLib/MetroBoyOAM.h"

#include "MetroLib/src/GameboyLib/MetroBoyCPU.h"
#include "MetroLib/src/GameboyLib/Constants.h"

#include <assert.h>
#include <memory.h>

//-----------------------------------------------------------------------------

void MetroBoyOAM::reset() {
  memset(ram, 0, sizeof(ram));
}

//-----------------------------------------------------------------------------

void MetroBoyOAM::tick(int phase_total, const Req& req, Ack& ack) const {
  (void)phase_total;

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

// FIXME this probably writes on phases other than GH during DMA...

void MetroBoyOAM::tock(int phase_total, const Req& req) {
  if (MB_DELTA_GH && req.write && (req.addr >= ADDR_OAM_BEGIN) && (req.addr <= ADDR_OAM_END)) {
    uint16_t oam_addr = req.addr & 0x00FF;
    uint16_t d = ram[oam_addr >> 1];
    if (oam_addr & 1) d = (d & 0x00FF) | (req.data_lo << 8);
    else              d = (d & 0xFF00) | (req.data_lo << 0);
    ram[oam_addr >> 1] = d;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyOAM::dump(Dumper& d) const {
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
