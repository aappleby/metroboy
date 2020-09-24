#include "MetroBoyLib/Bootrom.h"
#include "CoreLib/Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void Bootrom::reset(uint16_t new_pc) {
  disable_bootrom = new_pc != 0x0000;
}

//-----------------------------------------------------------------------------

void Bootrom::tick(int phase_total, const Req& req, Ack& ack) const {
  (void)phase_total;
  if (req.read && req.addr <= 0x00FF && !disable_bootrom) {
    ack.addr = req.addr;
    ack.data_lo = DMG_ROM_bin[req.addr];
    ack.read++;
  }
}

void Bootrom::tock(int phase_total, const Req& req) {
  if (DELTA_GH && req.write && req.addr == ADDR_DISABLE_BOOTROM) {
    disable_bootrom |= (req.data_lo != 0);
  }
}

//-----------------------------------------------------------------------------
