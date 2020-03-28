#include "IRAM.h"

#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void IRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

bool IRAM::on_ebus_req(Req ebus_req, Ack& ebus_ack) {
  bool hit = (ebus_req.addr & 0xC000) == 0xC000;
  if (!hit) return false;

  assert(!ebus_ack.read && !ebus_ack.write);

  if (ebus_req.read) {
    ebus_ack = {
      .addr  = ebus_req.addr,
      .data  = ram[ebus_req.addr & 0x1FFF],
      .read  = 1,
      .write = 0,
    };
    return true;
  }
  else if (ebus_req.write) {
    ram[ebus_req.addr & 0x1FFF] = uint8_t(ebus_req.data);
    ebus_ack = {
      .addr  = ebus_req.addr,
      .data  = ebus_req.data,
      .read  = 0,
      .write = 1,
    };
    return true;
  }
  else {
    assert(false);
    return false;
  }
}

//-----------------------------------------------------------------------------
