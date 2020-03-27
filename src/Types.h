#pragma once
#include <stdint.h>
#include <string>

typedef int16_t sample_t;

#define dumpit(a, b) sprintf(d, "%-14s " b "\n", #a, a);

//-----------------------------------------------------------------------------

struct Req {
  uint16_t addr;
  uint16_t data;
  uint16_t read;
  uint16_t write;
};

struct Ack {
  uint16_t addr;
  uint16_t data;
  uint16_t read;
  uint16_t write;
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

template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}

inline void print_req(std::string& d, const char* name, const Req& req) {
  sprintf(d, "%-16s %04x:%04x %s%s\n", name, req.addr, req.data,
    req.read  ? "\003R \001" : "- ",
    req.write ? "\002W \001" : "- ");
}

inline void print_ack(std::string& d, const char* name, const Ack& ack) {
  sprintf(d, "%-16s %04x:%04x %s%s\n", name, ack.addr, ack.data,
    ack.read  ? "\003R \001" : "- ",
    ack.write ? "\002W \001" : "- ");
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

