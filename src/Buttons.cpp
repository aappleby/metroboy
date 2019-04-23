#include "Buttons.h"
#include "Constants.h"

Buttons::Buttons() {
  reset();
}

void Buttons::reset() {
  val = 0xFF;
  p1 = 0xCF;
  bus_out = 0;
  bus_oe = false;
}

void Buttons::tock(uint16_t addr, uint8_t data, bool read, bool write) {
  bus_out = 0x00;
  bus_oe = false;

  if (write && addr == ADDR_P1) {
    p1 = (p1 & 0xCF) | (data & 0x30);
  }

  if (read && addr == ADDR_P1) {
    bus_out = p1;
    bus_oe = true;
  }

  //-----------------------------------

  switch (p1 & 0x30) {
  case 0x00: p1 = (p1 & 0xF0) | 0x0F; break;
  case 0x10: p1 = (p1 & 0xF0) | ((val >> 4) & 0xF); break;
  case 0x20: p1 = (p1 & 0xF0) | ((val >> 0) & 0xF); break;
  }
}

char* Buttons::dump(char* text_buf) {
  char* cursor = text_buf;
  cursor += sprintf(cursor, "%c %c %c %c %c %c %c %c\n",
                    val & 0x01 ? '-' : 'R',
                    val & 0x02 ? '-' : 'L',
                    val & 0x04 ? '-' : 'U',
                    val & 0x08 ? '-' : 'D',
                    val & 0x10 ? '-' : 'A',
                    val & 0x20 ? '-' : 'B',
                    val & 0x40 ? '-' : 'E',
                    val & 0x80 ? '-' : 'S');
  return cursor;
}