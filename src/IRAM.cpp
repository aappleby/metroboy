#include "IRAM.h"

#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void IRAM::reset() {
  memset(ram, 0, sizeof(ram));
}

//-----------------------------------------------------------------------------

void IRAM::tock(const Req& req) {
 if (req.write && (req.addr >= 0xC000) && (req.addr <= 0xFDFF)) {
    ram[req.addr & 0x1FFF] = uint8_t(req.data);
  }
}

void IRAM::tick(const Req& req, Ack& ack) const {
  if (req.read && (req.addr >= 0xC000) && (req.addr <= 0xFDFF)) {
    ack.addr = req.addr;
    ack.data = ram[req.addr & 0x1FFF];
    ack.read++;
  }
}

//-----------------------------------------------------------------------------
