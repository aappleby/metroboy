#include "Timer.h"
#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void Timer::reset() {
  *this = {};
  counter = 10994 + 1;
}

//-----------------------------------------------------------------------------

bool Timer::get_interrupt() const {
  return do_int;
}

//-----------------------------------------------------------------------------

bool Timer::on_ibus_req(Req ibus_req, Ack& ibus_ack) {
  bool timer_hit = (ibus_req.addr & 0xFFFC) == 0xFF04;
  if (!timer_hit) return false;

  assert(!ibus_ack.read && !ibus_ack.write);

  ibus_ack = {
    .addr  = ibus_req.addr,
    .data  = 0,
    .read  = ibus_req.read,
    .write = ibus_req.write,
  };

  if (ibus_req.write) {
    if (ibus_req.addr == ADDR_DIV)  { counter = 1; }
    if (ibus_req.addr == ADDR_TIMA) { tima    = uint8_t(ibus_req.data); }
    if (ibus_req.addr == ADDR_TMA)  { tma     = uint8_t(ibus_req.data); }
    if (ibus_req.addr == ADDR_TAC)  { tac     = uint8_t(ibus_req.data) | 0b11111000; }
  }

  if (ibus_req.read) {
    if (ibus_req.addr == ADDR_DIV)  { ibus_ack.data = uint8_t(counter >> 6); }
    if (ibus_req.addr == ADDR_TIMA) { ibus_ack.data = uint8_t(tima); }
    if (ibus_req.addr == ADDR_TMA)  { ibus_ack.data = tma; }
    if (ibus_req.addr == ADDR_TAC)  { ibus_ack.data = tac | 0b11111000; }
  }

  return true;
}

//-----------------------------------------------------------------------------

void Timer::tock(int tcycle) {
  const int tphase = tcycle & 3;
  if (tphase != 0) return;

  //----------------------------------------

  do_int = false;
  counter++;

  static const uint16_t masks[] = { 0x80, 0x02, 0x08, 0x20 };
  bool do_tick_ = (counter & masks[tac & 3]) && (tac & 4);

  if (do_tick && !do_tick_) {
    tima++;
    if (tima & 0x100) {
      tima = tma;
      do_int = true;
    }
  }
  do_tick = do_tick_;
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& d) {
  sprintf(d, "\002--------------TIMER------------\001\n");
  sprintf(d,   "CNT         0x%04x\n", counter);
  sprintf(d,   "DIV         0x%02x\n", uint8_t(counter >> 6));
  sprintf(d,   "TIMA        0x%02x\n", tima);
  sprintf(d,   "TMA         0x%02x\n", tma);
  sprintf(d,   "TAC         %s\n",     byte_to_bits(tac));
  sprintf(d,   "DO_INT      %d\n",     do_int);
  sprintf(d,   "DO_TICK     %d\n",     do_tick);
  sprintf(d,   "\n");
}

//-----------------------------------------------------------------------------




/*
tphase = tcycle_ & 3;
bus_to_timer = bus_to_timer_;
timer_to_bus = {};

const int clk_neg = (tcycle_ - 1) & (~tcycle_);
const bool clk_neg_1m = clk_neg & 0x0002;

if (clk_neg_1m) {
uint16_t tima_ = tima;
bool tick_ = do_tick;

counter++;
static const uint16_t masks[] = { 0x80, 0x02, 0x08, 0x20 };
tick_ = (counter & masks[tac & 3]) && (tac & 0b100);
if (do_tick && !tick_) tima_ = tima + 1;

do_interrupt = tima >> 8;
if (do_interrupt) tima_ = tma;

tima = tima_;
do_tick = tick_;
}

if ((bus_to_timer.addr & 0xFFFC) == 0xFF04) {
timer_to_bus = bus_to_timer;
timer_to_bus.ack = true;
}

if (bus_to_timer.write) {
if (bus_to_timer.addr == ADDR_DIV)  counter = 0;
if (bus_to_timer.addr == ADDR_TIMA) tima = (uint8_t)bus_to_timer.data;
if (bus_to_timer.addr == ADDR_TMA)  tma = (uint8_t)bus_to_timer.data;
if (bus_to_timer.addr == ADDR_TAC)  tac = (uint8_t)bus_to_timer.data | 0b11111000;
}

if (bus_to_timer.read) {
if (bus_to_timer.addr == ADDR_DIV)  { timer_to_bus.data = uint8_t(counter >> 6); }
if (bus_to_timer.addr == ADDR_TIMA) { timer_to_bus.data = uint8_t(tima); }
if (bus_to_timer.addr == ADDR_TMA)  { timer_to_bus.data = tma; }
if (bus_to_timer.addr == ADDR_TAC)  { timer_to_bus.data = tac | 0b11111000; }
}
*/

