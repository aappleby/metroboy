#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  void reset();
  Bus  tick() const;
  void tock(const int tcycle_, const Bus bus_to_zram_);
  void dump(std::string& d);

  const uint8_t* get() const { return ram; }

private:
  int tcycle;
  Bus bus_to_zram;
  Bus zram_to_bus;

  uint8_t  ram[127];
};

//-----------------------------------------------------------------------------
