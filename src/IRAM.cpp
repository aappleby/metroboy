#include "IRAM.h"

#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void IRAM::reset() {
  memset(ram, 0, sizeof(ram));
}

//-----------------------------------------------------------------------------

void IRAM::tock(int phase, const Req& req) {
 if (PHASE_F && req.write && ((req.addr & 0xC000) == 0xC000)) {
    ram[req.addr & 0x1FFF] = uint8_t(req.data);
  }
}

void IRAM::tick(int phase, const Req& req, Ack& ack) const {
  if (req.read && ((req.addr & 0xC000) == 0xC000)) {
    ack.addr = req.addr;
    ack.data = ram[req.addr & 0x1FFF];
    ack.read++;
  }
}

//-----------------------------------------------------------------------------
