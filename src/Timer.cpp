#include "Timer.h"
#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void Timer::reset() {
  ack = {0};
  counter = 10995; // only one value will pass poweron_004_div and poweron_005_div
  tima = 0;
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

void Timer::tock_req(const Req& req) {
  const bool timer_hit = (req.addr & 0xFFFC) == 0xFF04;

  if (!timer_hit) {
    ack = {0};
  }
  else if (req.write) {
    if (req.addr == ADDR_DIV)  { counter = 1; }
    if (req.addr == ADDR_TIMA) { tima    = uint8_t(req.data); }
    if (req.addr == ADDR_TMA)  { tma     = uint8_t(req.data); }
    if (req.addr == ADDR_TAC)  { tac     = uint8_t(req.data) | 0b11111000; }
    ack = {
      .addr  = req.addr,
      .data  = 0,
      .read  = 0,
      .write = 1,
    };
  }
  else if (req.read) {
    uint8_t data = 0;
    if (req.addr == ADDR_DIV)  { data = uint8_t(counter >> 6); }
    if (req.addr == ADDR_TIMA) { data = uint8_t(tima); }
    if (req.addr == ADDR_TMA)  { data = tma; }
    if (req.addr == ADDR_TAC)  { data = tac | 0b11111000; }
    ack = {
      .addr  = req.addr,
      .data  = data,
      .read  = 1,
      .write = 0,
    };
  }
}

void Timer::tick_ack(Ack& ack_) const {
  ack_.addr  += ack.addr;
  ack_.data  += ack.data;
  ack_.read  += ack.read;
  ack_.write += ack.write;
}

//-----------------------------------------------------------------------------

void Timer::tock_t0() {
  do_int = false;
  counter++;

  static const uint16_t masks[] = { 0x80, 0x02, 0x08, 0x20 };
  const bool do_tick_ = (counter & masks[tac & 3]) && (tac & 4);

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

