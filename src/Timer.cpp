#include "Timer.h"
#include "Constants.h"

//-----------------------------------------------------------------------------

void Timer::reset() {
  *this = {};
  counter = 10994 + 1;
}

//-----------------------------------------------------------------------------


Bus Timer::tick() const {
  return timer_to_bus;
}

bool Timer::get_interrupt() const {
  return do_int;
}

void Timer::tock(const int tcycle_, const Bus bus_to_timer_) {
  const int tphase = tcycle_ & 3;
  bus_to_timer = bus_to_timer_;
  timer_to_bus = {};

  if ((bus_to_timer.addr & 0xFFFC) == 0xFF04) {
    timer_to_bus = bus_to_timer;
    timer_to_bus.ack = true;
  }

  if (bus_to_timer.read) {
    if (bus_to_timer.addr == ADDR_DIV)  { timer_to_bus.data = uint8_t(counter >> 6); }
    if (bus_to_timer.addr == ADDR_TIMA) { timer_to_bus.data = uint8_t(tima); }
    if (bus_to_timer.addr == ADDR_TMA)  { timer_to_bus.data = tma; }
    if (bus_to_timer.addr == ADDR_TAC)  { timer_to_bus.data = tac | 0b11111000; }
  }

  //----------------------------------------

  if (tphase == 0) {
    do_int = false;
    counter++;
    if (counter & 0x100) {
      counter = tma;
      do_int = true;
    }
  }

  //----------------------------------------
  // Writes

  if (bus_to_timer.write) {
    if (bus_to_timer.addr == ADDR_DIV)  { counter = 0; }
    if (bus_to_timer.addr == ADDR_TIMA) { tima    = uint8_t(bus_to_timer_.data); }
    if (bus_to_timer.addr == ADDR_TMA)  { tma     = uint8_t(bus_to_timer_.data); }
    if (bus_to_timer.addr == ADDR_TAC)  { tac     = uint8_t(bus_to_timer_.data) | 0b11111000; }
  }
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& d) {
  sprintf(d,   "CNT         0x%04x\n", counter);
  sprintf(d,   "DIV         0x%02x\n", uint8_t(counter >> 6));
  sprintf(d,   "TIMA        0x%02x\n", tima);
  sprintf(d,   "TMA         0x%02x\n", tma);
  sprintf(d,   "TAC         %s\n",     byte_to_bits(tac));
  sprintf(d,   "INT         %d\n",     do_int);
  sprintf(d,   "\n");

  print_bus(d, "bus_to_tmr", bus_to_timer);
  print_bus(d, "tmr_to_bus", timer_to_bus);
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

