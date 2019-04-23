#pragma once
#include "Platform.h"

//-----------------------------------------------------------------------------

struct Sprite {
  uint8_t y;
  uint8_t x;
  uint8_t p;
  union {
    struct {
      uint8_t pad0 : 1;
      uint8_t pad1 : 1;
      uint8_t pad2 : 1;
      uint8_t pad3 : 1;
      uint8_t pal : 1;
      uint8_t flipX : 1;
      uint8_t flipY : 1;
      uint8_t pri : 1;
    } bits;
    uint8_t f;
  } flags;
};

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
