#pragma once

struct Buttons {
  Buttons();
  void reset();

  void tock(uint16_t addr, uint8_t data, bool read, bool write);

  uint8_t get() { return val; }
  void set(uint8_t new_val) { val = new_val; }
  char* dump(char* text_buf);

  uint8_t bus_out = 0;
  bool bus_oe = false;
  uint8_t val = 0;

private:

  uint8_t p1 = 0; // FF00
};