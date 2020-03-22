#include "IRAM.h"

#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void IRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

Ack IRAM::on_ebus_req(Req ebus_req) {
  bool hit = (ebus_req.addr & 0xC000) == 0xC000;
  if (!hit) return {};

  if (ebus_req.read) {
    return {
      .phase = ebus_req.phase,
      .addr  = ebus_req.addr,
      .data  = ram[ebus_req.addr & 0x1FFF],
      .read  = 1,
      .write = 0,
    };
  }
  else if (ebus_req.write) {
    ram[ebus_req.addr & 0x1FFF] = uint8_t(ebus_req.data);
    return {
      .phase = ebus_req.phase,
      .addr  = ebus_req.addr,
      .data  = ebus_req.data,
      .read  = 0,
      .write = 1,
    };
  }
  else {
    assert(false);
    return {};
  }
}

//-----------------------------------------------------------------------------
