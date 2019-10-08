#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {

  struct Out {
    uint16_t addr;
    uint8_t  data;
    uint16_t data16;
    bool     oe;
  };

  void reset();
  Out  tick() const;
  void tock(int tphase, const CpuBus bus);
  void dump(std::string& out) const;

  const uint8_t* get() const { return (uint8_t*)ram; }

private:
  Out out;
  uint16_t ram[128];
};

//-----------------------------------------------------------------------------
