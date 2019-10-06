#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {
  void reset();

  struct Out {
    uint16_t addr;
    uint8_t  data;
    uint16_t data16;
    bool     oe;
  };

  Out tick() const;
  void tock(const CpuBus bus);

  void dump(std::string& out) const;
  const uint8_t* get() const { return (uint8_t*)ram; }

private:
  Out out;
  uint16_t ram[128];
};

//-----------------------------------------------------------------------------
