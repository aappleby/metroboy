#include "VRAM.h"

#include "Constants.h"
#include <assert.h>

extern uint8_t bootrom_logo[];

//-----------------------------------------------------------------------------

void VRAM::reset() {
  *this = {};
  memcpy(ram, bootrom_logo, 416);
}

//-----------------------------------------------------------------------------

void VRAM::tock_req(const Req& req) {
  const bool vram_hit = (req.addr & 0xE000) == 0x8000;
  
  if (!vram_hit) {
    ack = {0};
  }
  else if (req.write) {
    ram[req.addr & 0x1FFF] = uint8_t(req.data);
    ack = {
      .addr  = req.addr,
      .data  = req.data,
      .read  = 0,
      .write = 1,
    };
  }
  else if (req.read) {
    ack = {
      .addr  = req.addr,
      .data  = ram[req.addr & 0x1FFF],
      .read  = 1,
      .write = 0,
    };
  }
}

void VRAM::tick_ack(Ack& ack_) const {
  ack_.addr  += ack.addr;
  ack_.data  += ack.data;
  ack_.read  += ack.read;
  ack_.write += ack.write;
}

//-----------------------------------------------------------------------------
