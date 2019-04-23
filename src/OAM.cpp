#include "Platform.h"
#include "OAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void OAM::reset() {
  bus_out = 0x00;
  bus_oe = false;

  for (int i = 0; i < 256; i++) ram[i] = 0;
}

void OAM::tock(uint16_t addr, uint8_t data, bool read, bool write) {
  bus_out = 0x00;
  bus_oe = false;

  if (read && ADDR_OAM_BEGIN <= addr && addr <= ADDR_OAM_END) {
    bus_out = ram[addr - ADDR_OAM_BEGIN];
    bus_oe = true;
  }

  if (write && ADDR_OAM_BEGIN <= addr && addr <= ADDR_OAM_END) {
    ram[addr - ADDR_OAM_BEGIN] = data;
  }
}

char* OAM::dump(char* cursor) {
  for (int i = 0; i < 10; i++) {
    uint8_t y = ram[4 * i + 0];
    cursor += sprintf(cursor, "%3d ", y);
  }
  cursor += sprintf(cursor, "\n");

  for (int i = 0; i < 10; i++) {
    uint8_t x = ram[4 * i + 1];
    cursor += sprintf(cursor, "%3d ", x);
  }
  cursor += sprintf(cursor, "\n");

  return cursor;
}

//-----------------------------------------------------------------------------
