#pragma once

//-----------------------------------------------------------------------------

struct VRAM {
  void reset();

  void tock(uint16_t addr, uint8_t data, bool read, bool write);

  uint8_t bus_out;

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
