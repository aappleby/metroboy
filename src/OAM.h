#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {
  void reset();

  void tock(uint16_t addr, uint8_t data, bool read, bool write);

  const Sprite& get_sprite(int index) const {
    return ((Sprite*)ram)[index];
  }
  uint8_t* get_ram() {
    return ram;
  }

  char* dump(char* cursor);

  uint8_t bus_out;
  bool bus_oe;

//private:

  uint8_t ram[256];
};

//-----------------------------------------------------------------------------
