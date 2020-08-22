#include "MetroBoyLib/ZRAM.h"
#include "CoreLib/Constants.h"
#include <assert.h.>
#include <memory.h>

//-----------------------------------------------------------------------------

void ZRAM::reset() {
  memset(ram, 0, 128);
}

//-----------------------------------------------------------------------------

void ZRAM::tock(const Req& req) {
  if (req.write && (req.addr >= 0xFF80) && (req.addr <= 0xFFFE)) {
    ram[req.addr & 0x007F] = (uint8_t)req.data_lo;
  }
}

void ZRAM::tick(const Req& req, Ack& ack) const {
  if (req.read && (req.addr >= 0xFF80) && (req.addr <= 0xFFFE)) {
    ack.addr = req.addr;
    ack.data_lo = ram[req.addr & 0x007F];
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

void ZRAM::dump(Dumper& d) const {
  d("\002---------------ZRAM-------------\001\n");
  for (int y = 0; y < 16; y++) {
    d("%04x: ", y * 8 + ADDR_ZEROPAGE_BEGIN);
    for (int x = 0; x < 8; x++) {
      uint8_t b = ram[x + y * 8];
      uint8_t l = (b >> 0) & 0x0F;
      uint8_t h = (b >> 4) & 0x0F;

      d("%c%c ", h > 9 ? '7' + h : '0' + h, l > 9 ? '7' + l : '0' + l);
    }
    d("\n");
  }
}

//-----------------------------------------------------------------------------
