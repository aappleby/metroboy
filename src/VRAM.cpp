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

void VRAM::vbus_req(Req vbus_req) {
  bool vram_hit = (vbus_req.addr & 0xE000) == 0x8000;
  
  if (!vram_hit) {
    ack = {0};
  }
  else if (vbus_req.write) {
    ram[vbus_req.addr & 0x1FFF] = uint8_t(vbus_req.data);
    ack = {
      .addr  = vbus_req.addr,
      .data  = vbus_req.data,
      .read  = 0,
      .write = 1,
    };
  }
  else if (vbus_req.read) {
    ack = {
      .addr  = vbus_req.addr,
      .data  = ram[vbus_req.addr & 0x1FFF],
      .read  = 1,
      .write = 0,
    };
  }
}

void VRAM::vbus_ack(Ack& vbus_ack) const {
  vbus_ack.addr  += ack.addr;
  vbus_ack.data  += ack.data;
  vbus_ack.read  += ack.read;
  vbus_ack.write += ack.write;
}

//-----------------------------------------------------------------------------
