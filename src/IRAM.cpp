#include "IRAM.h"

#include "Constants.h"
#include <assert.h>

#pragma warning(disable:4100)

//-----------------------------------------------------------------------------

void IRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

void IRAM::ebus_req(Req ebus_req) {
  ack = {0};
  bool hit = (ebus_req.addr & 0xC000) == 0xC000;
  if (!hit) {
    return;
  }
  else if (ebus_req.read) {
    ack = {
      .addr  = ebus_req.addr,
      .data  = ram[ebus_req.addr & 0x1FFF],
      .read  = 1,
      .write = 0,
    };
  }
  else if (ebus_req.write) {
    ram[ebus_req.addr & 0x1FFF] = uint8_t(ebus_req.data);
    ack = {
      .addr  = ebus_req.addr,
      .data  = ebus_req.data,
      .read  = 0,
      .write = 1,
    };
  }
  else {
    assert(false);
  }
}

void IRAM::ebus_ack(Ack& ebus_ack) const {
  ebus_ack.addr  += ack.addr;
  ebus_ack.data  += ack.data;
  ebus_ack.read  += ack.read;
  ebus_ack.write += ack.write;
}

//-----------------------------------------------------------------------------
