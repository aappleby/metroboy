#include "MetroBoyLib/Bootrom.h"
#include "CoreLib/Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void Bootrom::reset(uint16_t new_pc) {
  disable_bootrom = new_pc != 0x0000;
}

//-----------------------------------------------------------------------------

void Bootrom::tock(const Req& req) {
  if (req.write && req.addr == ADDR_DISABLE_BOOTROM) {
    disable_bootrom |= (req.data_lo != 0);
  }
}

void Bootrom::tick(const Req& req, Ack& ack) const {
  if (req.read && req.addr <= 0x00FF && !disable_bootrom) {
    ack.addr = req.addr;
    ack.data_lo = DMG_ROM_bin[req.addr];
    ack.read++;
  }
}

//-----------------------------------------------------------------------------
