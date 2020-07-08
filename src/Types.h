#pragma once

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef int16_t sample_t;

//-----------------------------------------------------------------------------

struct Req {
  uint16_t addr = 0;
  uint8_t  data = 0;
  uint8_t  read = 0;
  uint8_t  write = 0;

  operator bool() const {
    return read || write;
  }
};

struct Ack {
  uint16_t addr = 0;
  uint16_t data = 0;
  uint8_t  read = 0;
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

struct Dumper {
  virtual void operator()(const char* format, ...) = 0;
  virtual void clear() = 0;
};

void dump_req(Dumper& d, const Req& req);
void dump_ack(Dumper& d, const Ack& ack);

//-----------------------------------------------------------------------------