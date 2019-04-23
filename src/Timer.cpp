#include "Platform.h"
#include "Timer.h"

#include "Common.h"
#include "Constants.h"

//-----------------------------------------------------------------------------

void Timer::reset() {
  overflow = false;
  bus_out = 0;
  bus_oe = false;

  counter = 0x2AF3;
  old_tima = 0x00;
  new_tima = 0x00;
  tma = 0x00;
  tac = 0xF8;

  tick = false;
}

//-----------------------------------------------------------------------------

static const int masks[] = { 0x80, 0x02, 0x08, 0x20 };

void Timer::tock(int tphase, uint16_t addr, uint8_t data, bool read, bool write) {
  if (read) {
    bus_out = 0x00;
    bus_oe = false;
    if (addr == ADDR_TAC)  { bus_oe = true; bus_out = tac | 0b11111000; }
    if (addr == ADDR_TMA)  { bus_oe = true; bus_out = tma; }
    if (addr == ADDR_DIV)  { bus_oe = true; bus_out = uint8_t(counter >> 6); }
    if (addr == ADDR_TIMA) { bus_oe = true; bus_out = new_tima; }
  }

  if (tphase == 0) {
    counter = counter + 1;
    old_tima = new_tima;
  }

  bool new_tick = (counter & masks[tac & 3]) && (tac & TAC_RUN);
  if (tick && !new_tick) {
    new_tima = old_tima + 1;
  }
  tick = new_tick;

  if (tphase == 2) {
    if (overflow) new_tima = tma;
    overflow = (old_tima == 0xFF) && (new_tima == 0x00);
  }

  if (write) {
    if (addr == ADDR_TIMA) new_tima = data;
    if (addr == ADDR_DIV) counter = 0;
    if (addr == ADDR_TAC) tac = data;
    if (addr == ADDR_TMA) tma = data;
  }
}

//-----------------------------------------------------------------------------

char* Timer::dump(char* cursor) {
  cursor += sprintf(cursor, "TICK 0x%08x\n", counter);
  cursor += sprintf(cursor, "DIV  0x%02x\n", (counter >> 6) & 0xFF);
  cursor += sprintf(cursor, "TIMA 0x%02x\n", new_tima);
  cursor += sprintf(cursor, "TMA  0x%02x\n", tma);
  cursor += sprintf(cursor, "TAC  %s\n", to_binary(tac));
  return cursor;
}

//-----------------------------------------------------------------------------
