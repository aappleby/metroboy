#pragma once

#include "Config.h"
#include <vector> // for blob

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef int16_t sample_t;

typedef std::vector<uint8_t> blob;

//typedef const bool wire;
typedef bool wire;
typedef uint8_t wire2;

inline bool bit(wire2 w)        { return bool(w & 1); }
inline bool bit(wire2 w, int i) { return bool((w >> i) & 1); }

enum RunMode {
  RUN_STOP = 0, // don't run
  RUN_STEP = 1, // run one step
  RUN_FAST = 2, // run steps as fast as possible
  RUN_SYNC = 3, // run until end of frame, then wait for vsync signal
};

enum StepSize {
  STEP_PASS  = 0,
  STEP_PHASE = 1,
};

static const char* runmode_names[] = {
  "RUN_STOP ",
  "RUN_STEP ",
  "RUN_FAST ",
  "RUN_SYNC ",
};

static const char* stepmode_names[] = {
  "STEP_PASS  ",
  "STEP_PHASE ",
  "STEP_CYCLE ",
  "STEP_LINE  ",
  "STEP_FRAME ",
};

static const char* phase_names[] = {
  "\002A_______\001",
  "\003_B______\001",
  "\002__C_____\001",
  "\003___D____\001",
  "\002____E___\001",
  "\003_____F__\001",
  "\002______G_\001",
  "\003_______H\001",
};

constexpr uint64_t HASH_INIT = 0x12345678;
uint64_t mix(uint64_t h);
uint64_t hash_states(void* blob, int len);

double timestamp();

template<typename S, typename T>
inline T clamp_val(S a, T min, T max) {
  return (a < min) ? min : (a > max) ? max : T(a);
}

//-----------------------------------------------------------------------------

//#define PHASE(A) ((A) & (1 << (7 - phase)))

#define DELTA_HA  ((((phase_total + 0) & 7) == 7) && (((phase_total + 1) & 7) == 0))
#define DELTA_AB  ((((phase_total + 0) & 7) == 0) && (((phase_total + 1) & 7) == 1))
#define DELTA_BC  ((((phase_total + 0) & 7) == 1) && (((phase_total + 1) & 7) == 2))
#define DELTA_CD  ((((phase_total + 0) & 7) == 2) && (((phase_total + 1) & 7) == 3))
#define DELTA_DE  ((((phase_total + 0) & 7) == 3) && (((phase_total + 1) & 7) == 4))
#define DELTA_EF  ((((phase_total + 0) & 7) == 4) && (((phase_total + 1) & 7) == 5))
#define DELTA_FG  ((((phase_total + 0) & 7) == 5) && (((phase_total + 1) & 7) == 6))
#define DELTA_GH  ((((phase_total + 0) & 7) == 6) && (((phase_total + 1) & 7) == 7))

#define DELTA_EVEN ((phase_total & 1) == 0)
#define DELTA_ODD  ((phase_total & 1) == 1)

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201)

#pragma pack(push, 1)
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
#pragma pack(pop)

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
  virtual void add_char(char c) { (void)c; }
  virtual void newline() { add_char('\n'); }

  virtual void operator()(const char* format, ...) = 0;
  virtual void indent() {}
  virtual void dedent() {}

  inline void dump_bitp(const char* tag, uint8_t b) {
    operator()("%s%c\n", tag, (b & 0x01) ? '1' : '0');
  }

  inline void dump_bitn(const char* tag, uint8_t b) {
    operator()("%s%c\n", tag, (b & 0x01) ? '0' : '1');
  }

  inline void dump_bytep(const char* tag, uint8_t b) {
    operator()(tag);
    for (int i = 7; i >= 0; i--) {
      add_char((b & (1 << i)) ? '1' : '0');
    }
    operator()(" 0x%02x %d\n", b, b);
  }

  void dump_slice2p(const char* tag, const void* blob, int byte_count) {
    operator()(tag);
    for (int i = byte_count; i < 8; i++) add_char(' ');

    const uint8_t* d = (const uint8_t*)blob;
    for (int i = byte_count - 1; i >= 0; i--) {
      add_char((d[i] & 0x02) ? '\002' : '\003');
      add_char((d[i] & 0x01) ? '1' : '0');
    }
    add_char('\001');

    uint16_t val = 0;
    for (int i = 0; i < byte_count; i++) val |= ((d[i] & 1) << i);
    if (byte_count <= 8) {
      operator()(" 0x%02x %d\n", val, val);
    } else {
      operator()("\n%s0x%04x %d\n", tag, val, val);
    }
  }

  void dump_slice2n(const char* tag, const void* blob, int byte_count) {
    operator()(tag);
    for (int i = byte_count; i < 8; i++) add_char(' ');

    const uint8_t* d = (const uint8_t*)blob;
    for (int i = byte_count - 1; i >= 0; i--) {
      add_char((d[i] & 0x02) ? '\002' : '\003');
      add_char((d[i] & 0x01) ? '0' : '1');
    }
    add_char('\001');

    uint8_t val = 0;
    for (int i = 0; i < byte_count; i++) val |= ((~d[i] & 1) << i);
    if (byte_count <= 8) {
      operator()(" 0x%02x %d\n", val, val);
    } else {
      operator()("\n%s0x%04x %d\n", tag, val, val);
    }
  }

  void dump_req(const char* tag, const Req& req) {
    operator()("%s%04x:%04x %s%s\n", tag, req.addr, req.data,
      req.read  ? "\003R\001" : "-",
      req.write ? "\002W\001" : "-");
  }

  void dump_ack(const char* tag, const Ack& ack) {
    operator()("%s%04x:%04x %s -\n", tag, ack.addr, ack.data,
      ack.read  ? "\003R\001" : "-");
  }
};

#define DUMP_BITP(A) d.dump_bitp(#A " : ", A.state);

//-----------------------------------------------------------------------------

#define ASSERT_P(A)  if (!(A)) { printf("ASSERT_P fail : " #A); __debugbreak(); }
#define ASSERT_N(A)  if ((A))  { printf("ASSERT_N fail : " #A); __debugbreak(); }

#ifdef ALWAYS_CHECK

#define CHECK_P(A)   if (!(A)) { printf("CHECK_P fail : " #A);  __debugbreak(); }
#define CHECK_N(A)   if ((A))  { printf("CHECK_N fail : " #A);  __debugbreak(); }

#else

#define CHECK_P(A)
#define CHECK_N(A)

#endif

//-----------------------------------------------------------------------------