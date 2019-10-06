#include "Buttons.h"

#include "Constants.h"

void Buttons::reset() {
  *this = {};
  val = 0xFF;
  p1 = 0xCF;
}

ButtonsOut Buttons::tick() const {
  return out;
}

void Buttons::tock(CpuBus bus) {
  out = {};

  if (bus.write && bus.addr == ADDR_P1) {
    p1 = (p1 & 0xCF) | (bus.data & 0x30);
  }

  if (bus.read && bus.addr == ADDR_P1) {
    out.data = p1;
    out.oe = true;
  }

  //-----------------------------------

  switch (p1 & 0x30) {
  case 0x00: p1 = (p1 & 0xF0) | 0x0F; break;
  case 0x10: p1 = (p1 & 0xF0) | ((val >> 4) & 0xF); break;
  case 0x20: p1 = (p1 & 0xF0) | ((val >> 0) & 0xF); break;
  }

  out.val = val;
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
}