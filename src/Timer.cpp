#include "Timer.h"
#include "Constants.h"

const char* to_binary(uint8_t b);

//-----------------------------------------------------------------------------

TimerOut Timer::reset() {
  counter = 10994 * 4 + 2;
  tima = 0x00;
  tma = 0x00;
  tac = 0xF8;

  tick = false;
  out = {0};

  return out;
}

//-----------------------------------------------------------------------------

static const uint16_t masks[] = { 0x200, 0x08, 0x20, 0x80 };

TimerOut Timer::tickB() const {
  return out;
}

void Timer::tock(int tphase, CpuBus bus) {
  if (tphase != 0 && tphase != 2) return;

  uint16_t tima_ = tima;
  bool tick_ = tick;

  counter++;
  counter++;

  tick_ = (counter & masks[tac & 3]) && (tac & 0b100);
  if (tick && !tick_) tima_ = tima + 1;

  if (tphase == 0) {
    if (bus.write && bus.addr == ADDR_DIV)  counter = 0;
    if (bus.write && bus.addr == ADDR_TIMA) tima_ = bus.data;
    if (bus.write && bus.addr == ADDR_TMA)  tma = bus.data;
    if (bus.write && bus.addr == ADDR_TAC)  tac = bus.data;

    out.interrupt = tima >> 8;
    if (out.interrupt) tima_ = tma;
  }

  if (tphase == 2) {
    out.data = 0x00;
    out.oe = false;

    if (bus.read && bus.addr == ADDR_DIV)  { out.oe = true; out.data = uint8_t(counter >> 8); }
    if (bus.read && bus.addr == ADDR_TIMA) { out.oe = true; out.data = uint8_t(tima); }
    if (bus.read && bus.addr == ADDR_TMA)  { out.oe = true; out.data = tma; }
    if (bus.read && bus.addr == ADDR_TAC)  { out.oe = true; out.data = tac | 0b11111000; }
  }

  tima = tima_;
  tick = tick_;
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& dump_out) {
  sprintf(dump_out, "CNT      0x%08x\n", counter);
  sprintf(dump_out, "DIV      0x%02x\n", (counter >> 8) & 0xFF);
  sprintf(dump_out, "TIMA     0x%02x\n", tima);
  sprintf(dump_out, "TMA      0x%02x\n", tma);
  sprintf(dump_out, "TAC      %s\n",     to_binary(tac));
}

//-----------------------------------------------------------------------------







/*
if (tphase == 0) {
counter = counter + 2;
old_tima = new_tima;

bool new_tick = (counter & masks[tac & 3]) && (tac & TAC_RUN);
if (tick && !new_tick) new_tima++;
tick = new_tick;
}

if (bus.write && bus.addr == ADDR_TIMA) new_tima = bus.data;
if (bus.write && bus.addr == ADDR_DIV) counter = 0;
if (bus.write && bus.addr == ADDR_TAC) tac = bus.data;
if (bus.write && bus.addr == ADDR_TMA) tma = bus.data;

if (tphase == 1) {
if (overflow) new_tima = tma;
overflow = (old_tima == 0xFF) && (new_tima == 0x00);

}

out.data = 0x00;
out.oe = false;
if (bus.read && bus.addr == ADDR_TAC)  { out.oe = true; out.data = tac | 0b11111000; }
if (bus.read && bus.addr == ADDR_TMA)  { out.oe = true; out.data = tma; }
if (bus.read && bus.addr == ADDR_DIV)  { out.oe = true; out.data = uint8_t(counter >> 7); }
if (bus.read && bus.addr == ADDR_TIMA) { out.oe = true; out.data = new_tima; }

out.overflow = overflow;

return out;
*/
