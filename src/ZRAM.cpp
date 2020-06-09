#include "ZRAM.h"
#include "Constants.h"
#include <assert.h.>

//-----------------------------------------------------------------------------

void ZRAM::reset() {
  memset(ram, 0, 128);
}

//-----------------------------------------------------------------------------

void ZRAM::tock_req(const Req& req) {
  const bool zram_hit = ((req.addr & 0xFF80) == 0xFF80) && (req.addr != 0xFFFF);

  if (!zram_hit) {
    ack = {0};
  }
  else if (req.write) {
    ram[req.addr & 0x007F] = (uint8_t)req.data;
    ack = {
      .addr  = req.addr,
      .data  = 0,
      .read  = 0,
    };
  }
  else if (req.read) {
    ack = {
      .addr  = req.addr,
      .data  = ram[req.addr & 0x007F],
      .read  = 1,
    };
  }
  else {
    __debugbreak();
  }
}

void ZRAM::tick_ack(Ack& ack_) const {
  ack_.addr  += ack.addr;
  ack_.data  += ack.data;
  ack_.read  += ack.read;
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
