#include "Timer.h"
#include "Constants.h"

const char* to_binary(uint8_t b);

//-----------------------------------------------------------------------------

TimerOut Timer::reset() {
  counter = 10994;
  tima = 0x00;
  tma = 0x00;
  tac = 0xF8;
  tick = false;
  overflow = false;

  return { 0 };
}

//-----------------------------------------------------------------------------

static const int masks[] = { 0x80, 0x02, 0x08, 0x20 };

/*
!o !t !w - old_tima
!o !t  w - bus
!o  t !w - tima + 1
!o  t  w - bus + 1
 o !t !w - tma
 o !t  w - bus
 o  t !w - tma
 o  t  w - bus
*/

TimerOut Timer::tock(int tphase, CpuBus bus) {
  if (tphase != 2) return out;

  out.data = 0;
  out.oe = false;

  if (bus.write && bus.addr == ADDR_TMA)  tma = bus.data;
  if (bus.write && bus.addr == ADDR_TAC)  tac = bus.data | 0b11111000;

  counter = counter + 1;

  if (bus.write && bus.addr == ADDR_DIV)  counter = 0; 

  bool old_tick = tick;
  bool new_tick = (counter & masks[tac & 3]) && (tac & TAC_RUN);

  uint8_t old_tima = tima;
  uint8_t new_tima = tima;

  bool o = overflow;
  bool t = old_tick && !new_tick;
  bool w = bus.write && bus.addr == ADDR_TIMA;

  if (!o && !t && !w) new_tima = old_tima;
  if (!o && !t &&  w) new_tima = bus.data;
  if (!o &&  t && !w) new_tima = new_tima + 1;
  if (!o &&  t &&  w) new_tima = bus.data + 1;
  if ( o && !t && !w) new_tima = tma;
  if ( o && !t &&  w) new_tima = bus.data;
  if ( o &&  t && !w) new_tima = tma;
  if ( o &&  t &&  w) new_tima = bus.data;

  overflow = (old_tima == 0xFF) && (new_tima == 0x00);

  // read after counter inc
  if (bus.read && bus.addr == ADDR_DIV)  { out.oe = true; out.data = uint8_t(counter >> 6); } 
  if (bus.read && bus.addr == ADDR_TMA)  { out.oe = true; out.data = tma; }
  if (bus.read && bus.addr == ADDR_TAC)  { out.oe = true; out.data = tac; }
  if (bus.read && bus.addr == ADDR_TIMA) { out.oe = true; out.data = new_tima; }

  tima = new_tima;
  tick = new_tick;
  out.overflow = overflow;

  return out;
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& dump_out) {
  sprintf(dump_out, "CNT      0x%08x\n", counter);
  sprintf(dump_out, "DIV      0x%02x\n", (counter >> 6) & 0xFF);
  sprintf(dump_out, "TIMA     0x%02x\n", tima);
  sprintf(dump_out, "TMA      0x%02x\n", tma);
  sprintf(dump_out, "TAC      %s\n",     to_binary(tac));
  sprintf(dump_out, "TICK     %d\n",     tick);
  sprintf(dump_out, "OVF      %d\n",     overflow);

  /*
  sprintf(out, "OLD_CNT  0x%08x\n", old_counter);
  sprintf(out, "OLD_DIV  0x%02x\n", (old_counter >> 6) & 0xFF);
  sprintf(out, "OLD_TIMA 0x%02x\n", old_tima);
  sprintf(out, "OLD_TMA  0x%02x\n", old_tma);
  sprintf(out, "OLD_TAC  %s\n",     to_binary(old_tac));
  sprintf(out, "OLD_TICK %d\n",     old_tick);
  sprintf(out, "OLD_OVF  %d\n",     old_overflow);
  */
}

//-----------------------------------------------------------------------------
