#pragma once
#include <stdint.h>
#include <string>

typedef int16_t sample_t;

#define dumpit(a, b) sprintf(d, "%-14s " b "\n", #a, a);

//-----------------------------------------------------------------------------

struct Framebuffer {
  uint8_t buf[160*144];
};

struct CpuBus {
  uint16_t addr;
  uint8_t data;
  bool read;
  bool write;
  uint8_t int_ack;
};

struct CpuOut {
  uint8_t data;
  bool oe;
};

struct BusOut {
  uint8_t data;
  bool oe;
  uint16_t addr;
};

struct SpuOut {
  uint8_t data;
  bool oe;
  sample_t out_r;
  sample_t out_l;
};

struct ButtonsOut {
  uint8_t data;
  bool oe;
  uint8_t val;
};

struct GameboyOut {
  int x;
  int y;
  int counter;
  uint8_t pix;
  bool pix_oe;
  sample_t out_r;
  sample_t out_l;
  uint32_t trace;
};

const uint8_t F_CARRY = 0x10;
const uint8_t F_HALF_CARRY = 0x20;
const uint8_t F_NEGATIVE = 0x40;
const uint8_t F_ZERO = 0x80;

struct AluOut {
  uint8_t x;
  uint8_t f;
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

#pragma warning(disable:4100)

template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}
