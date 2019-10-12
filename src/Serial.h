#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Serial {
  void reset();
  Bus  tick() const;
  void tock(int tphase_, Bus bus_to_serial_);
  void dump(std::string& d);

private:
  int tphase;
  Bus bus_to_serial;
  Bus serial_to_bus;

  uint8_t sb; // FF01
  uint8_t sc; // FF02
};

//-----------------------------------------------------------------------------
