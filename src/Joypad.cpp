#include "Joypad.h"
#include "Constants.h"
#include <assert.h>

void Joypad::reset() {
  val = 0xFF;
  p1 = 0xCF; // FF00
}

void Joypad::tock(int phase, const Req& req) {
  if (PHASE_F && req.write && req.addr == ADDR_P1) {
    p1 = (p1 & 0xCF) | (req.data_lo & 0x30);
    switch (p1 & 0x30) {
    case 0x00: p1 = (p1 & 0xF0) | 0x0F; break;
    case 0x10: p1 = (p1 & 0xF0) | ((val >> 4) & 0xF); break;
    case 0x20: p1 = (p1 & 0xF0) | ((val >> 0) & 0xF); break;
    }
  }
}

void Joypad::tick(const Req& req, Ack& ack) const {
  if (req.read && req.addr == ADDR_P1) {
    ack.addr = req.addr;
    ack.data_lo = p1;
    ack.read++;
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

void Joypad::dump(Dumper& dump) const {
  dump("\002--------------JOYPAD----------\001\n");
  dump("%c %c %c %c %c %c %c %c\n",
          val & 0x01 ? '-' : 'R',
          val & 0x02 ? '-' : 'L',
          val & 0x04 ? '-' : 'U',
          val & 0x08 ? '-' : 'D',
          val & 0x10 ? '-' : 'A',
          val & 0x20 ? '-' : 'B',
          val & 0x40 ? '-' : 'E',
          val & 0x80 ? '-' : 'S');
}