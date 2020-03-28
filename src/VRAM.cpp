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

bool VRAM::on_vbus_req(Req vbus_req, Ack& vbus_ack) {
  bool vram_hit = (vbus_req.addr & 0xE000) == 0x8000;
  if (!vram_hit) return false;

  assert(!vbus_ack.read && !vbus_ack.write);

  if (vram_hit && vbus_req.write) {
    ram[vbus_req.addr & 0x1FFF] = uint8_t(vbus_req.data);
    vbus_ack = {
      .addr  = vbus_req.addr,
      .data  = vbus_req.data,
      .read  = 0,
      .write = 1,
    };
    return true;
  }
  else if (vram_hit && vbus_req.read) {
    vbus_ack = {
      .addr  = vbus_req.addr,
      .data  = ram[vbus_req.addr & 0x1FFF],
      .read  = 1,
      .write = 0,
    };
    return true;
  }
  else {
    assert(false);
    return false;
  }
}

//-----------------------------------------------------------------------------
