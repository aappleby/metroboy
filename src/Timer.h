#pragma once
#include <stdint.h>

struct Timer {
  void reset();
  void tock(int tphase, uint16_t addr, uint8_t data, bool read, bool write);
  char* dump(char* cursor);

  bool overflow;
  uint8_t bus_out;
  bool bus_oe;

//private:

  void do_tima();

  uint16_t counter;  // FF04
  uint8_t  old_tima; // FF05
  uint8_t  new_tima;
  uint8_t  tma;      // FF06
  uint8_t  tac;      // FF07
  bool     tick;
};
