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

//typedef const bool wire;
typedef bool wire;

#define PHASE_A  ((phase & 7) == 0)
#define PHASE_B  ((phase & 7) == 1)
#define PHASE_C  ((phase & 7) == 2)
#define PHASE_D  ((phase & 7) == 3)
#define PHASE_E  ((phase & 7) == 4)
#define PHASE_F  ((phase & 7) == 5)
#define PHASE_G  ((phase & 7) == 6)
#define PHASE_H  ((phase & 7) == 7)
#define PHASE_HI ((phase & 1) == 1)
#define PHASE_LO ((phase & 1) == 0)

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201)

struct Req {
  uint16_t addr = 0;
  union {
    struct {
      uint8_t data_lo;
      uint8_t data_hi;
    };
    uint16_t data = 0;
  };
  uint8_t  read = 0;
  uint8_t  write = 0;
  uint16_t pad1 = 0;

  operator bool() const {
    return read || write;
  }
};

static_assert(sizeof(Req) == 8, "Req size != 8");

struct Ack {
  uint16_t addr = 0;
  union {
    struct {
      uint8_t data_lo;
      uint8_t data_hi;
    };
    uint16_t data = 0;
  };
  uint8_t  read = 0;
  uint8_t  pad1 = 0;
  uint16_t pad2 = 0;
};

static_assert(sizeof(Ack) == 8, "Ack size != 8");

#pragma warning(pop)

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

#if _DEBUG

#define CHECK_P(A) do { if (!(A)) __debugbreak(); } while(0);
#define CHECK_N(A) do { if ((A)) __debugbreak(); } while(0);

#else 

#define CHECK_P(A)
#define CHECK_N(A)

#endif

//-----------------------------------------------------------------------------