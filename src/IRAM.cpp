#include "IRAM.h"

#include "Constants.h"
#include <assert.h>

#pragma warning(disable:4100)

//-----------------------------------------------------------------------------

void IRAM::reset() {
  ack = {0};
  memset(ram, 0, sizeof(ram));
}

//-----------------------------------------------------------------------------

void IRAM::tock_req(const Req& req) {
  ack = {0};
  bool hit = req && ((req.addr & 0xC000) == 0xC000);
  if (!hit) {
    return;
  }
  else if (req.read) {
    ack = {
      .addr  = req.addr,
      .data  = ram[req.addr & 0x1FFF],
      .read  = 1,
    };
  }
  else if (req.write) {
    ram[req.addr & 0x1FFF] = uint8_t(req.data);
    ack = {
      .addr  = req.addr,
      .data  = req.data,
      .read  = 0,
    };
  }
  else {
    assert(false);
  }
}

void IRAM::tick_ack(Ack& ack_) const {
  ack_.addr  += ack.addr;
  ack_.data  += ack.data;
  ack_.read  += ack.read;
}

//-----------------------------------------------------------------------------
