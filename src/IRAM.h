#pragma once
#include "Types.h"

struct IRAM {
  struct Out {
    Bus iram_to_bus;
    Bus iram_to_dma;
  };

  void reset();
  Out  tick() const;
  void tock(int tphase, Bus bus_to_iram_, Bus dma_to_iram_);
  void dump(std::string& d);

  const uint8_t* get() { return ram; }
private:
  int tphase;
  Bus bus_to_iram;
  Bus dma_to_iram;
  Bus iram_to_bus;
  Bus iram_to_dma;
  uint8_t ram[8192];
};