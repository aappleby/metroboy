#include "Joypad.h"
#include "Constants.h"
#include <assert.h>

void Joypad::reset() {
  *this = {};
  val = 0xFF;
  p1 = 0xCF;
}

bool Joypad::on_ibus_req(Req ibus_req, Ack& ibus_ack) {
  if (ibus_req.addr != ADDR_P1) return false;

  assert(!ibus_ack.read && !ibus_ack.write);

  if (ibus_req.read) {
    ibus_ack = {
      .addr  = ibus_req.addr,
      .data  = p1,
      .read  = 1,
      .write = 0,
    };
    return true;
  }
  else if (ibus_req.write) {
    p1 = (p1 & 0xCF) | (ibus_req.data & 0x30);
    switch (p1 & 0x30) {
    case 0x00: p1 = (p1 & 0xF0) | 0x0F; break;
    case 0x10: p1 = (p1 & 0xF0) | ((val >> 4) & 0xF); break;
    case 0x20: p1 = (p1 & 0xF0) | ((val >> 0) & 0xF); break;
    }

    ibus_ack = {
      .addr  = ibus_req.addr,
      .data  = ibus_req.data,
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

void Joypad::set(uint8_t new_val) {
  val = new_val;
  switch (p1 & 0x30) {
  case 0x00: p1 = (p1 & 0xF0) | 0x0F; break;
  case 0x10: p1 = (p1 & 0xF0) | ((val >> 4) & 0xF); break;
  case 0x20: p1 = (p1 & 0xF0) | ((val >> 0) & 0xF); break;
  }
}

void Joypad::dump(std::string& d) const {
  sprintf(d, "\002--------------JOYPAD----------\001\n");
  sprintf(d, "%c %c %c %c %c %c %c %c\n",
          val & 0x01 ? '-' : 'R',
          val & 0x02 ? '-' : 'L',
          val & 0x04 ? '-' : 'U',
          val & 0x08 ? '-' : 'D',
          val & 0x10 ? '-' : 'A',
          val & 0x20 ? '-' : 'B',
          val & 0x40 ? '-' : 'E',
          val & 0x80 ? '-' : 'S');
}