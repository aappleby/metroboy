#include "Buttons.h"

#include "Constants.h"

void Buttons::reset() {
  *this = {};
  val = 0xFF;
  p1 = 0xCF;
}

Buttons::Out Buttons::tick() const {
  return {
    addr,
    data,
    oe,
    val
  };
}

void Buttons::tock(int tphase, CpuBus bus) {
  if (tphase == 0 && bus.read) {
    addr = 0;
    data = 0;
    oe = 0;
    if (bus.addr == ADDR_P1) {
      addr = bus.addr;
      data = p1;
      oe = true;
    }
  }

  switch (p1 & 0x30) {
  case 0x00: p1 = (p1 & 0xF0) | 0x0F; break;
  case 0x10: p1 = (p1 & 0xF0) | ((val >> 4) & 0xF); break;
  case 0x20: p1 = (p1 & 0xF0) | ((val >> 0) & 0xF); break;
  }

  if (tphase == 2) {
    if (bus.write && bus.addr == ADDR_P1) {
      p1 = (p1 & 0xCF) | (bus.data & 0x30);
    }
  }
}

void Buttons::set(uint8_t new_val) {
  val = new_val;
}

void Buttons::dump(std::string& d) const {
  sprintf(d, "%c %c %c %c %c %c %c %c\n",
          val & 0x01 ? '-' : 'R',
          val & 0x02 ? '-' : 'L',
          val & 0x04 ? '-' : 'U',
          val & 0x08 ? '-' : 'D',
          val & 0x10 ? '-' : 'A',
          val & 0x20 ? '-' : 'B',
          val & 0x40 ? '-' : 'E',
          val & 0x80 ? '-' : 'S');
  dumpit(addr, "0x%04x");
  dumpit(data, "0x%02x");
  dumpit(oe,   "%d");
}