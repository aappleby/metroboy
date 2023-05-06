#include "MetroBoyLib/MetroBoyBootrom.h"

#include "metrolib/gameboy/MetroBoyCPU.h"
#include "metrolib/gameboy/GBConstants.h"

#include "metrolib/core/Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void MetroBoyBootrom::tick(int phase_total, const Req& req, Ack& ack) const {
  (void)phase_total;
  if (req.read && req.addr <= 0x00FF && !disable_bootrom) {
    ack.addr = req.addr;
    ack.data_lo = get_DMG_ROM()[req.addr];
    ack.read++;
  }
}

void MetroBoyBootrom::tock(int phase_total, const Req& req) {
  if (MB_DELTA_GH && req.write && req.addr == ADDR_DISABLE_BOOTROM) {
    disable_bootrom |= (req.data_lo != 0);
  }
}

//-----------------------------------------------------------------------------
