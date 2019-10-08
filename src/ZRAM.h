#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  struct Out {
    uint16_t addr;
    uint8_t data;
    bool oe;
  };

  void reset();
  Out  tick() const;
  void tock(int tphase, CpuBus bus);
  void dump(std::string& d);

  const uint8_t* get() const { return ram; }

private:
  Out out;
  uint8_t ram[127];
};

//-----------------------------------------------------------------------------
