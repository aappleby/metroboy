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

//-----------------------------------------------------------------------------

inline int pack_p(wire a, wire b, wire c, wire d) {
  return (a << 0) | (b << 1) | (c << 2) | (d << 3);
}

inline int pack_p(wire a, wire b, wire c, wire d,
                wire e, wire f, wire g, wire h) {
  return (pack_p(a, b, c, d) << 0) | (pack_p(e, f, g, h) << 4);
}

inline int pack_p(wire a, wire b, wire c, wire d,
                wire e, wire f, wire g, wire h,
                wire i, wire j, wire k, wire l,
                wire m, wire n, wire o, wire p) {
  return (pack_p(a, b, c, d, e, f, g, h) << 0) | (pack_p(i, j, k, l, m, n, o, p) << 8);
}

//-----------------------------------------------------------------------------

inline int pack_n(wire a, wire b, wire c, wire d) {
  return (!a << 0) | (!b << 1) | (!c << 2) | (!d << 3);
}

inline int pack_n(wire a, wire b, wire c, wire d,
                wire e, wire f, wire g, wire h) {
  return (pack_n(a, b, c, d) << 0) | (pack_n(e, f, g, h) << 4);
}

inline int pack_n(wire a, wire b, wire c, wire d,
                  wire e, wire f, wire g, wire h,
                  wire i, wire j, wire k, wire l,
                  wire m, wire n, wire o, wire p) {
  return (pack_n(a, b, c, d, e, f, g, h) << 0) | (pack_n(i, j, k, l, m, n, o, p) << 8);
}

//-----------------------------------------------------------------------------

#define PHASE(A) ((A) & (1 << (7 - phase)))

#define DELTA_AB  ((phase & 7) == 0)
#define DELTA_BC  ((phase & 7) == 1)
#define DELTA_CD  ((phase & 7) == 2)
#define DELTA_DE  ((phase & 7) == 3)
#define DELTA_EF  ((phase & 7) == 4)
#define DELTA_FG  ((phase & 7) == 5)
#define DELTA_GH  ((phase & 7) == 6)
#define DELTA_HA  ((phase & 7) == 7)
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

  inline void dump_reg(const char* tag, wire D0, wire D1, wire D2, wire D3, wire D4, wire D5, wire D6, wire D7) {
    int D = pack_p(D0, D1, D2, D3, D4, D5, D6, D7);
    operator()("%-10s : %-3d 0x%02x 0b%d%d%d%d%d%d%d%d\n", tag, D, D, D7, D6, D5, D4, D3, D2, D1, D0);
  }
};

void dump_req(Dumper& d, const Req& req);
void dump_ack(Dumper& d, const Ack& ack);

//-----------------------------------------------------------------------------

#if _DEBUG

inline void CHECK_P (wire A)         { if (!A)     __debugbreak(); }
inline void CHECK_N (wire A)         { if (A)      __debugbreak(); }
inline void CHECK_EQ(wire A, wire B) { if (A != B) __debugbreak(); }
inline void CHECK_NE(wire A, wire B) { if (A == B) __debugbreak(); }

#else 

#define CHECK_P(A)
#define CHECK_N(A)
#define CHECK_EQ(A,B)
#define CHECK_NE(A,B)

#endif

//-----------------------------------------------------------------------------