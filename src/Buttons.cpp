#include "Buttons.h"

#include "Constants.h"

void Buttons::reset() {
  *this = {};
  val = 0xFF;
  p1 = 0xCF;
}

Bus Buttons::tick() const {
  return buttons_to_bus;
}

void Buttons::tock(int tphase_, Bus bus_to_buttons_) {
  tphase = tphase_;
  bus_to_buttons = bus_to_buttons_;
  buttons_to_bus = {};

  if (bus_to_buttons.read) {
    if (bus_to_buttons.addr == ADDR_P1) {
      buttons_to_bus.addr = bus_to_buttons.addr;
      buttons_to_bus.data = p1;
      buttons_to_bus.read = true;
      buttons_to_bus.write = false;
    }
  }

  switch (p1 & 0x30) {
  case 0x00: p1 = (p1 & 0xF0) | 0x0F; break;
  case 0x10: p1 = (p1 & 0xF0) | ((val >> 4) & 0xF); break;
  case 0x20: p1 = (p1 & 0xF0) | ((val >> 0) & 0xF); break;
  }

  if (tphase == 2) {
    if (bus_to_buttons.write && bus_to_buttons.addr == ADDR_P1) {
      p1 = (p1 & 0xCF) | (bus_to_buttons.data & 0x30);
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

  print_bus(d, "bus_to_buttons", bus_to_buttons);
  print_bus(d, "buttons_to_bus", buttons_to_bus);
}