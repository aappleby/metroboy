#include "MetroBoyLib/Serial.h"
#include "CoreLib/Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void Serial::reset() {
  sb = 0x00; // FF01
  sc = 0x7E; // FF02
}

//-----------------------------------------------------------------------------

void Serial::tick(int phase_total, const Req& req, Ack& ack) const {
  (void)phase_total;

  if (req.read && ((req.addr == ADDR_SB) || (req.addr == ADDR_SC))) {
    ack.addr = req.addr;
    ack.data_lo = req.addr == ADDR_SB ? sb : sc;
    ack.read++;
  }
}

void Serial::tock(int phase_total, const Req& req) {
  if (req.write && DELTA_GH) {
    if (req.addr == ADDR_SB) sb = (uint8_t)req.data_lo;
    if (req.addr == ADDR_SC) sc = (uint8_t)req.data_lo | 0b01111110;
  }
}

//-----------------------------------------------------------------------------

void Serial::dump(Dumper& d) const {
  d("\002--------------SERIAL-----------\001\n");
  d("SB 0x%02x\n", sb);
  d("SC 0x%02x\n", sc);
}

//-----------------------------------------------------------------------------
