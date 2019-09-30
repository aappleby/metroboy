#include "Timer.h"
#include "Constants.h"

const char* to_binary(uint8_t b);

//-----------------------------------------------------------------------------

TimerOut Timer::reset() {
  counter = 9826;
  tima = 0x00;
  tma = 0x00;
  tac = 0xF8;
  overflow = false;
  tick = false;

  old_counter = counter;
  old_tima = tima;
  old_tma = tma;
  old_tac = tac;
  old_overflow = overflow;
  old_tick = tick;

  return { 0 };
}

//-----------------------------------------------------------------------------

static const int masks[] = { 0x80, 0x02, 0x08, 0x20 };

TimerOut Timer::tock(int tphase, CpuBus bus) {
  old_counter = counter;
  old_tima = tima;
  old_tma = tma;
  old_tac = tac;
  old_overflow = overflow;
  old_tick = tick;

  if (tphase == 0) {
    if      (bus.write && bus.addr == ADDR_DIV)  counter = 0;
    else if (bus.write && bus.addr == ADDR_TIMA) tima = bus.data;
    else if (bus.write && bus.addr == ADDR_TAC)  tac = bus.data;
    else if (bus.write && bus.addr == ADDR_TMA)  tma = bus.data;

    counter = counter + 1;
    tick = (counter & masks[tac & 3]) && (tac & TAC_RUN);
    old_tima = tima;
    if (old_tick && !tick) tima = old_tima + 1;
    old_tick = tick;
    if (overflow) tima = tma;

    overflow = (old_tima == 0xFF) && (tima == 0x00);
    return { 0, 0, overflow };
  }

  else if (tphase == 1) {
    return { 0, 0, overflow };
  }

  else if (tphase == 2) {
    TimerOut out = { 0, 0, overflow };
    if (bus.read && bus.addr == ADDR_TAC)  { out.oe = true; out.data = tac | 0b11111000; }
    if (bus.read && bus.addr == ADDR_TMA)  { out.oe = true; out.data = tma; }
    if (bus.read && bus.addr == ADDR_DIV)  { out.oe = true; out.data = uint8_t(counter >> 6); }
    if (bus.read && bus.addr == ADDR_TIMA) { out.oe = true; out.data = tima; }
    return out;
  }

  else {
    return { 0, 0, overflow };
  }
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& out) {
  sprintf(out, "CNT      0x%08x\n", counter);
  sprintf(out, "DIV      0x%02x\n", (counter >> 6) & 0xFF);
  sprintf(out, "TIMA     0x%02x\n", tima);
  sprintf(out, "TMA      0x%02x\n", tma);
  sprintf(out, "TAC      %s\n",     to_binary(tac));
  sprintf(out, "TICK     %d\n",     tick);
  sprintf(out, "OVF      %d\n",     overflow);

  sprintf(out, "OLD_CNT  0x%08x\n", old_counter);
  sprintf(out, "OLD_DIV  0x%02x\n", (old_counter >> 6) & 0xFF);
  sprintf(out, "OLD_TIMA 0x%02x\n", old_tima);
  sprintf(out, "OLD_TMA  0x%02x\n", old_tma);
  sprintf(out, "OLD_TAC  %s\n",     to_binary(old_tac));
  sprintf(out, "OLD_TICK %d\n",     old_tick);
  sprintf(out, "OLD_OVF  %d\n",     old_overflow);
}

//-----------------------------------------------------------------------------
