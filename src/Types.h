#pragma once
#include <stdint.h>
#include <string>

typedef int16_t sample_t;

#define dumpit(a, b) sprintf(d, "%-14s " b "\n", #a, a);

//-----------------------------------------------------------------------------

struct Bus {
  uint16_t addr;
  uint16_t data;
  uint8_t  read;
  uint8_t  write;
  uint8_t  lock;
  uint8_t  dma;
  uint8_t  ack;
};

struct Framebuffer {
  uint8_t buf[160*144];
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

//#pragma warning(disable:4100)

template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}

inline void print_bus(std::string& d, const char* name, const Bus& bus) {
  /*
  struct Bus {
    uint16_t addr;
    uint16_t data;
    uint8_t  read;
    uint8_t  write;
    uint8_t  lock;
    uint8_t  dma;
    uint8_t  ack;
  };
  */

  sprintf(d, "%-11s %04x:%04x %s%s%s%s%s\n", name, bus.addr, bus.data,
    bus.read  ? "\003R \001" : "- ",
    bus.write ? "\002W \001" : "- ",
    bus.lock  ? "\004L \001" : "- ",
    bus.dma   ? "\005D \001" : "- ",
    bus.ack   ? "\006A \001" : "- ");

  /*
  if (bus.lock) {
    if (bus.write) {
      sprintf(d, "\007%-10s \002W*0x%04x\001 = 0x%04x\n", name, bus.addr, bus.data);
    } else if (bus.read) {
      sprintf(d, "\007%-10s \003R*0x%04x\001 = 0x%04x\n", name, bus.addr, bus.data);
    } else {
      sprintf(d, "\007%-10s \001-*------\001\n", name);
    }
  }
  else {
    if (bus.write) {
      sprintf(d, "\007%-10s \002W:0x%04x\001 = 0x%04x\n", name, bus.addr, bus.data);
    } else if (bus.read) {
      sprintf(d, "\007%-10s \003R:0x%04x\001 = 0x%04x\n", name, bus.addr, bus.data);
    } else {
      sprintf(d, "\007%-10s \001-:------\001\n", name);
    }
  }
  */
}

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

