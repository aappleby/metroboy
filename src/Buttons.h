#pragma once
#include <stdint.h>

#pragma warning(disable : 4201)

struct Buttons {
  Buttons();
  void reset();

  void tock(uint16_t addr, uint8_t data, bool read, bool write);

  uint8_t get() { return val; }
  void set(uint8_t new_val) { val = new_val; }
  char* dump(char* text_buf);

  uint8_t bus_out;
  bool bus_oe;
  uint8_t val;

private:

  uint8_t p1; // FF00
};