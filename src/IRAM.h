#pragma once

struct IRAM {
  void reset();
  void tock_t2(uint16_t addr, uint8_t data, bool read, bool write);

  const uint8_t* get() {
    return ram;
  }

  uint8_t bus_out;
  bool bus_oe;
  uint8_t ram[8192];
};