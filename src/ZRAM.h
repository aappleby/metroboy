#pragma once
#include "Platform.h"

struct ZRAM {
  void reset();
  void tock(uint16_t addr, uint8_t data, bool read, bool write);

  const uint8_t* get() {
    return ram;
  }

  uint8_t bus_out;
  bool bus_oe;

private:
  uint8_t ram[127];
};