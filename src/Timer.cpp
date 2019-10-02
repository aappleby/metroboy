#include "Timer.h"
#include "Constants.h"

const char* to_binary(uint8_t b);

//-----------------------------------------------------------------------------

TimerOut Timer::reset() {
  counter = 10994;
  //tima = 0x00;
  old_tima = 0x00;
  new_tima = 0x00;
  tma = 0x00;
  tac = 0xF8;
  tick = false;

  out.data = 0;
  out.oe = 0;
  out.overflow = false;

  return { 0 };
}

//-----------------------------------------------------------------------------

static const int masks[] = { 0x80, 0x02, 0x08, 0x20 };

TimerOut Timer::tock(int tphase, CpuBus bus) {

  if (tphase == 0) {
    counter = counter + 1;
    old_tima = new_tima;

    bool new_tick = (counter & masks[tac & 3]) && (tac & TAC_RUN);
    if (tick && !new_tick) new_tima++;
    tick = new_tick;

    if (bus.write) {
      if (bus.addr == ADDR_TIMA) new_tima = bus.data;
      if (bus.addr == ADDR_DIV) counter = 0;
      if (bus.addr == ADDR_TAC) tac = bus.data;
      if (bus.addr == ADDR_TMA) tma = bus.data;
    }
  }

  if (tphase == 2) {
    if (out.overflow) new_tima = tma;
    out.overflow = (old_tima == 0xFF) && (new_tima == 0x00);

    if (bus.read) {
      out.data = 0x00;
      out.oe = false;
      if (bus.addr == ADDR_TAC)  { out.oe = true; out.data = tac | 0b11111000; }
      if (bus.addr == ADDR_TMA)  { out.oe = true; out.data = tma; }
      if (bus.addr == ADDR_DIV)  { out.oe = true; out.data = uint8_t(counter >> 6); }
      if (bus.addr == ADDR_TIMA) { out.oe = true; out.data = new_tima; }
    }
  }

  return out;
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& dump_out) {
  sprintf(dump_out, "CNT      0x%08x\n", counter);
  sprintf(dump_out, "DIV      0x%02x\n", (counter >> 6) & 0xFF);
  sprintf(dump_out, "TIMA     0x%02x\n", old_tima);
  sprintf(dump_out, "TIMA     0x%02x\n", new_tima);
  sprintf(dump_out, "TMA      0x%02x\n", tma);
  sprintf(dump_out, "TAC      %s\n",     to_binary(tac));
  sprintf(dump_out, "TICK     %d\n",     tick);
  sprintf(dump_out, "OVF      %d\n",     out.overflow);
}

//-----------------------------------------------------------------------------
