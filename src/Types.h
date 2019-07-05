#pragma once
#include <stdint.h>
#include <string>

typedef int16_t sample_t;

//-----------------------------------------------------------------------------

struct CpuBus {
  uint16_t addr;
  uint8_t data;
  bool read;
  bool write;
};

struct BusOut {
  uint8_t data;
  bool oe;
};

struct SpuOut {
  uint8_t data;
  bool oe;
  sample_t out_r;
  sample_t out_l;
};

struct PpuOut {
  uint8_t data;
  bool oe;
  int x;
  int y;
  int counter;
  uint8_t pix;
};

struct GameboyOut {
  int x;
  int y;
  int counter;
  uint8_t pix;
  sample_t out_r;
  sample_t out_l;
};

struct TimerOut {
  uint8_t data;
  bool oe;
  bool overflow;
};

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

template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}
