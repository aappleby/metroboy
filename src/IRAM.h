#pragma once
#include "Types.h"

struct IRAM {
  struct Out {
    uint16_t addr;
    uint8_t data;
    bool oe;
  };

  void reset();
  Out  tick() const;
  void tock(int tphase, CpuBus bus);
  void dump(std::string& d);

  const uint8_t* get() { return ram; }
private:
  uint16_t addr;
  uint8_t data;
  bool oe;
  uint8_t ram[8192];
};