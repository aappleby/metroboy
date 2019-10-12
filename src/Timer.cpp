#include "Timer.h"
#include "Constants.h"

//-----------------------------------------------------------------------------

void Timer::reset() {
  *this = {};

  counter = 10994 * 4 + 2;
  tima = 0x00;
  tma = 0x00;
  tac = 0xF8;
}

//-----------------------------------------------------------------------------

static const uint16_t masks[] = { 0x200, 0x08, 0x20, 0x80 };

Bus Timer::tick() const {
  return timer_to_bus;
}

bool Timer::get_interrupt() const {
  return do_interrupt;
}

void Timer::tock(int tphase_, Bus bus_to_timer_) {
  tphase = tphase_;
  bus_to_timer = bus_to_timer_;
  timer_to_bus = {};

  if (tphase != 0 && tphase != 2) return;

  uint16_t tima_ = tima;
  bool tick_ = do_tick;

  counter++;
  counter++;

  tick_ = (counter & masks[tac & 3]) && (tac & 0b100);
  if (do_tick && !tick_) tima_ = tima + 1;

  if (bus_to_timer.write) {
    if (bus_to_timer.addr == ADDR_DIV)  counter = 0;
    if (bus_to_timer.addr == ADDR_TIMA) tima_ = (uint8_t)bus_to_timer.data;
    if (bus_to_timer.addr == ADDR_TMA)  tma = (uint8_t)bus_to_timer.data;
    if (bus_to_timer.addr == ADDR_TAC)  tac = (uint8_t)bus_to_timer.data;
  }
  else if (bus_to_timer.read) {
    if (bus_to_timer.addr == ADDR_DIV)  { timer_to_bus.addr = bus_to_timer.addr; timer_to_bus.read = true; timer_to_bus.ack = true; timer_to_bus.data = uint8_t(counter >> 8); }
    if (bus_to_timer.addr == ADDR_TIMA) { timer_to_bus.addr = bus_to_timer.addr; timer_to_bus.read = true; timer_to_bus.ack = true; timer_to_bus.data = uint8_t(tima); }
    if (bus_to_timer.addr == ADDR_TMA)  { timer_to_bus.addr = bus_to_timer.addr; timer_to_bus.read = true; timer_to_bus.ack = true; timer_to_bus.data = tma; }
    if (bus_to_timer.addr == ADDR_TAC)  { timer_to_bus.addr = bus_to_timer.addr; timer_to_bus.read = true; timer_to_bus.ack = true; timer_to_bus.data = tac | 0b11111000; }
  }

  // should this go in tphase 0 now?
  if (tphase == 2) {
    do_interrupt = tima >> 8;
    if (do_interrupt) tima_ = tma;
  }

  tima = tima_;
  do_tick = tick_;
}

//-----------------------------------------------------------------------------

void Timer::dump(std::string& d) {
  sprintf(d, "tphase %d\n", tphase);
  print_bus(d, "bus_to_tmr", bus_to_timer);
  print_bus(d, "tmr_to_bus", timer_to_bus);
  sprintf(d, "do_interrupt %d\n", do_interrupt);

  sprintf(d, "CNT            0x%04x\n", counter);
  sprintf(d, "DIV            0x%02x\n", (counter >> 8) & 0xFF);
  sprintf(d, "TIMA           0x%02x\n", tima);
  sprintf(d, "TMA            0x%02x\n", tma);
  sprintf(d, "TAC            %s\n",     byte_to_bits(tac));
  sprintf(d, "\n");
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
