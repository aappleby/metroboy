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
  uint8_t int_ack;
};

struct CpuOut {
  uint8_t data;
  bool oe;
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

struct PpuTickOut {
  bool fire_int_stat1;
  bool fire_int_stat2;
  bool fire_int_vblank1;
  bool fire_int_vblank2;
};

struct PpuOut {
  uint8_t data;
  bool oe;

  bool vram_lock;
  uint16_t vram_addr;
  bool vram_read;

  bool oam_lock;
  uint16_t oam_addr;
  bool oam_read;

  int x;
  int y;
  int counter;
  uint8_t pix_out;
  bool pix_oe;

  bool vblank_int;
  bool stat_int;
};

struct ButtonsOut {
  uint8_t data;
  bool oe;
  uint8_t val;
};

struct TimerOut {
  uint8_t data;
  bool oe;
  bool interrupt;
};

struct GameboyOut {
  int x;
  int y;
  int counter;
  uint8_t pix;
  bool pix_oe;
  sample_t out_r;
  sample_t out_l;
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

template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}
