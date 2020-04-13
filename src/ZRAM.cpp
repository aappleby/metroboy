#include "ZRAM.h"
#include "Constants.h"
#include <assert.h.>

//-----------------------------------------------------------------------------

void ZRAM::reset() {
  memset(ram, 0, 128);
}

//-----------------------------------------------------------------------------

void ZRAM::ibus_req(Req ibus_req) {
  bool zram_hit = ((ibus_req.addr & 0xFF80) == 0xFF80) && (ibus_req.addr != 0xFFFF);

  if (!zram_hit) {
    ack = {0};
  }
  else if (ibus_req.write) {
    ram[ibus_req.addr & 0x007F] = (uint8_t)ibus_req.data;
    ack = {
      .addr  = ibus_req.addr,
      .data  = 0,
      .read  = 0,
      .write = 1,
    };
  }
  else if (ibus_req.read) {
    ack = {
      .addr  = ibus_req.addr,
      .data  = ram[ibus_req.addr & 0x007F],
      .read  = 1,
      .write = 0,
    };
  }
  else {
    __debugbreak();
  }
}

void ZRAM::ibus_ack(Ack& ibus_ack) const {
  ibus_ack.addr  += ack.addr;
  ibus_ack.data  += ack.data;
  ibus_ack.read  += ack.read;
  ibus_ack.write += ack.write;
}

//-----------------------------------------------------------------------------

void ZRAM::dump(std::string& d) {
  sprintf(d, "\002---------------ZRAM-------------\001\n");
  for (int y = 0; y < 16; y++) {
    sprintf(d, "%04x: ", y * 8 + ADDR_ZEROPAGE_BEGIN);
    for (int x = 0; x < 8; x++) {
      uint8_t b = ram[x + y * 8];
      uint8_t l = (b >> 0) & 0x0F;
      uint8_t h = (b >> 4) & 0x0F;

      d.push_back(h > 9 ? '7' + h : '0' + h);
      d.push_back(l > 9 ? '7' + l : '0' + l);
      d.push_back(' ');
    }
    d.push_back('\n');
  }
}

//-----------------------------------------------------------------------------
