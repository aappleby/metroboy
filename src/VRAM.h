#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct VRAM {
  void reset();

  BusOut tock(CpuBus bus);

  union {
    struct {
      uint8_t tiles0[2048];  // 8000 - tile pattern 0
      uint8_t tiles1[2048];  // 8800 - tile pattern 1
      uint8_t tiles2[2048];  // 9000 - tile pattern 2
      uint8_t map0[1024];    // 9800
      uint8_t map1[1024];    // 9C00
    };
    uint8_t ram[8192];
  };
};

//-----------------------------------------------------------------------------
