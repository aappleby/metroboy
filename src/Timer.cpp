#include "Timer.h"
#include "Constants.h"

const char* to_binary(uint8_t b);

//-----------------------------------------------------------------------------

void Timer::reset() {
  overflow = false;

  counter = 0x2AF3;
  old_tima = 0x00;
  new_tima = 0x00;
  tma = 0x00;
  tac = 0xF8;

  tick = false;
}

//-----------------------------------------------------------------------------

static const int masks[] = { 0x80, 0x02, 0x08, 0x20 };

BusOut Timer::tock(int tphase, CpuBus bus) {
  BusOut out = { 0,0 };

  if (bus.read) {
    if (bus.addr == ADDR_TAC)  { out.oe = true; out.data = tac | 0b11111000; }
    if (bus.addr == ADDR_TMA)  { out.oe = true; out.data = tma; }
    if (bus.addr == ADDR_DIV)  { out.oe = true; out.data = uint8_t(counter >> 6); }
    if (bus.addr == ADDR_TIMA) { out.oe = true; out.data = new_tima; }
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

  if (tphase == 1) {
    if (overflow) new_tima = tma;
    overflow = (old_tima == 0xFF) && (new_tima == 0x00);
  }

  if (bus.write) {
    if (bus.addr == ADDR_TIMA) new_tima = bus.data;
    if (bus.addr == ADDR_DIV) counter = 0;
    if (bus.addr == ADDR_TAC) tac = bus.data;
    if (bus.addr == ADDR_TMA) tma = bus.data;
  }

  return out;
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& out) {
  sprintf(out, "TICK 0x%08x\n", counter);
  sprintf(out, "DIV  0x%02x\n", (counter >> 6) & 0xFF);
  sprintf(out, "TIMA 0x%02x\n", new_tima);
  sprintf(out, "TMA  0x%02x\n", tma);
  sprintf(out, "TAC  %s\n", to_binary(tac));
}

//-----------------------------------------------------------------------------
