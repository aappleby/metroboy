#pragma once
#include "Platform.h"

struct Serial {
  void reset();
  void tock(uint16_t addr, uint8_t data, bool read, bool write);

  uint8_t bus_out;
  bool bus_oe;

private:
  uint8_t sb; // FF01
  uint8_t sc; // FF02
};