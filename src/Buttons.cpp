#include "Buttons.h"

#include "Constants.h"

void Buttons::reset() {
  *this = {};
  out.val = 0xFF;
  p1 = 0xCF;
}

Buttons::Out Buttons::tick() const {
  return out;
}

void Buttons::tock(int tphase, CpuBus bus) {
  if (tphase == 0 && bus.read) {
    out = {};
    if (bus.addr == ADDR_P1) {
      out.addr = bus.addr;
      out.data = p1;
      out.oe = true;
    }
  }

  switch (p1 & 0x30) {
  case 0x00: p1 = (p1 & 0xF0) | 0x0F; break;
  case 0x10: p1 = (p1 & 0xF0) | ((out.val >> 4) & 0xF); break;
  case 0x20: p1 = (p1 & 0xF0) | ((out.val >> 0) & 0xF); break;
  }

  if (tphase == 2) {
    if (bus.write && bus.addr == ADDR_P1) {
      p1 = (p1 & 0xCF) | (bus.data & 0x30);
    }
  }
}

void Buttons::set(uint8_t new_val) {
  out.val = new_val;
}

void Buttons::dump(std::string& d) const {
  sprintf(d, "%c %c %c %c %c %c %c %c\n",
          out.val & 0x01 ? '-' : 'R',
          out.val & 0x02 ? '-' : 'L',
          out.val & 0x04 ? '-' : 'U',
          out.val & 0x08 ? '-' : 'D',
          out.val & 0x10 ? '-' : 'A',
          out.val & 0x20 ? '-' : 'B',
          out.val & 0x40 ? '-' : 'E',
          out.val & 0x80 ? '-' : 'S');
  dumpit(out.addr, "0x%04x");
  dumpit(out.data, "0x%02x");
  dumpit(out.oe,   "%d");
}