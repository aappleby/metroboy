#include "Timer.h"
#include "Constants.h"
#include <assert.h>

static const uint16_t masks[] = { 0x80, 0x02, 0x08, 0x20 };

//-----------------------------------------------------------------------------

void Timer::reset() {
  counter = 10995; // only one value will pass poweron_004_div and poweron_005_div
  old_tima = 0;
  new_tima = 0;
  tma = 0;
  tac = 0;
  do_int = 0;
  do_tick = 0;
}

//-----------------------------------------------------------------------------

bool Timer::get_interrupt() const {
  return do_int;
}

//-----------------------------------------------------------------------------

void Timer::tick(const Req& req, Ack& ack) const {
  uint8_t data = 0;

  if (req.read) switch(req.addr) {
  case ADDR_DIV:  data = uint8_t(counter >> 6); break;
  case ADDR_TIMA: data = new_tima; break;
  case ADDR_TMA:  data = tma; break;
  case ADDR_TAC:  data = tac | 0b11111000; break;
  };

  ack.addr  += req.addr;
  ack.data  += data;
  ack.read  += req.read;
  ack.write += req.write;
}

//-----------------------------------------------------------------------------

void Timer::tock(int tphase, const Req& req) {
  if (tphase == 0) {
    counter = counter + 1;
    old_tima = new_tima;
  }

  bool do_tick_ = (counter & masks[tac & 3]) && (tac & TAC_RUN);
  if (do_tick && !do_tick_) {
    new_tima = old_tima + 1;
  }
  do_tick = do_tick_;

  if (req.write) switch(req.addr) {
  case ADDR_DIV:  counter = 0; break;
  case ADDR_TIMA: new_tima = uint8_t(req.data); break;
  case ADDR_TMA:  tma = uint8_t(req.data); break;
  case ADDR_TAC:  tac = uint8_t(req.data); break;
  }

  if (tphase == 1) {
    if (do_int) new_tima = tma;
    do_int = (old_tima == 0xFF) && (new_tima == 0x00);
  }
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& d) {
  sprintf(d, "\002--------------TIMER------------\001\n");
  sprintf(d,   "CNT         0x%04x\n", counter);
  sprintf(d,   "DIV         0x%02x\n", uint8_t(counter >> 6));
  sprintf(d,   "TIMA        0x%02x\n", new_tima);
  sprintf(d,   "TMA         0x%02x\n", tma);
  sprintf(d,   "TAC         %s\n",     byte_to_bits(tac));
  sprintf(d,   "DO_INT      %d\n",     do_int);
  sprintf(d,   "DO_TICK     %d\n",     do_tick);
  sprintf(d,   "\n");
}

//-----------------------------------------------------------------------------



#if 0
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

  do_tick = false;
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

  bool do_tick_ = (counter & masks[tac & 3]) && (tac & TAC_RUN);
  if (do_tick && !do_tick_) {
    new_tima = old_tima + 1;
  }
  do_tick = do_tick_;

  if (tphase == 1) {
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

void Timer::dump(std::string& out) {
  sprintf(out, "TICK 0x%08x\n", counter);
  sprintf(out, "DIV  0x%02x\n", (counter >> 6) & 0xFF);
  sprintf(out, "TIMA 0x%02x\n", new_tima);
  sprintf(out, "TMA  0x%02x\n", tma);
  sprintf(out, "TAC  %s\n", to_binary(tac));
}

//-----------------------------------------------------------------------------
#endif
