#include "Platform.h"
#include "Buttons.h"

#include "Constants.h"

Buttons::Buttons() {
  reset();
}

void Buttons::reset() {
  val = 0xFF;
  p1 = 0xCF;
}

BusOut Buttons::tock(CpuBus bus) {
  BusOut ret = { 0,0 };

  if (bus.write && bus.addr == ADDR_P1) {
    p1 = (p1 & 0xCF) | (bus.data & 0x30);
  }

  if (bus.read && bus.addr == ADDR_P1) {
    ret.data = p1;
    ret.oe = true;
  }

  //-----------------------------------

  switch (p1 & 0x30) {
  case 0x00: p1 = (p1 & 0xF0) | 0x0F; break;
  case 0x10: p1 = (p1 & 0xF0) | ((val >> 4) & 0xF); break;
  case 0x20: p1 = (p1 & 0xF0) | ((val >> 0) & 0xF); break;
  }

  return ret;
}

void Buttons::dump(std::string& out) {
  sprintf(out, "%c %c %c %c %c %c %c %c\n",
          val & 0x01 ? '-' : 'R',
          val & 0x02 ? '-' : 'L',
          val & 0x04 ? '-' : 'U',
          val & 0x08 ? '-' : 'D',
          val & 0x10 ? '-' : 'A',
          val & 0x20 ? '-' : 'B',
          val & 0x40 ? '-' : 'E',
          val & 0x80 ? '-' : 'S');
}