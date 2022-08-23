#pragma once

#include "Log.h"
#include "Config.h"
#include <vector> // for blob
#include <stdint.h>
#include <string.h>
#include <math.h>

//------------------------------------------------------------------------------

#ifdef __GNUC__
#include <csignal>
#define debugbreak() raise(SIGTRAP);

typedef int64_t LARGE_INTEGER;
#endif

#ifdef _MSC_VER

#define debugbreak() __debugbreak()

#endif

//------------------------------------------------------------------------------

//#ifdef _MSC_VER

#include <span>
typedef std::span<const char> cspan;

//#define debugbreak() __debugbreak();

//#endif

//------------------------------------------------------------------------------

typedef int16_t sample_t;

typedef const uint8_t wire;
//typedef uint8_t wire;
struct triwire { wire state; };

/*__attribute__((always_inline))*/ inline wire bit0(uint32_t w) { return wire(w & 1); }
inline wire bit(uint32_t w, int i) { return wire((w >> i) & 1); }
inline wire bit(int w, int i) { return wire((w >> i) & 1); }
inline wire bit(int w) { return wire(w & 1); }

template<typename T>
inline void set_bit(T& t, int c, bool x) {
  t &= ~(1 << c);
  t |=  (x << c);
}

uint32_t swap(uint32_t x);
uint64_t swap(uint64_t x);

constexpr uint32_t xorshift32(uint32_t& x)
{
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return x;
}

constexpr uint8_t bit_reverse(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

constexpr wire gen_clk(int64_t phase, uint8_t mask) {
  if (phase < 0 || phase >> 7) debugbreak();
  return !!(bit_reverse(mask) & (1 << phase));
}

//constexpr uint64_t HASH_INIT = 0x12345678;
constexpr uint64_t HASH_INIT = 0;
uint32_t mix(uint32_t h);
uint64_t mix(uint64_t h);
uint64_t hash_blob(const void* blob, size_t len);
uint64_t hash_blob(uint64_t h, const void* blob, size_t len);

double timestamp();

template<typename S, typename T>
inline T clamp_val(S a, T min, T max) {
  return (a < min) ? min : (a > max) ? max : T(a);
}

const uint32_t COL_DARK_GREY    = 0x00444444;
const uint32_t COL_DARK_RED     = 0x00444488;
const uint32_t COL_MID_RED      = 0x004444CC;

const uint32_t COL_DARK_GREEN   = 0x00448844;
const uint32_t COL_PALE_BROWN   = 0x005DA2A2;
const uint32_t COL_ORANGE       = 0x004488CC;

const uint32_t COL_MID_GREEN    = 0x0044CC44;
const uint32_t COL_LIME         = 0x0044CC88;
const uint32_t COL_MID_YELLOW   = 0x0066CCCC;

const uint32_t COL_DARK_BLUE    = 0x00884444;
const uint32_t COL_DARK_MAGENTA = 0x00884488;
const uint32_t COL_ROSE         = 0x008844CC;

const uint32_t COL_DARK_TEAL    = 0x00888844;
const uint32_t COL_MID_GREY     = 0x00999999;
const uint32_t COL_PALE_RED     = 0x008888CC;

const uint32_t COL_MINT         = 0x0088CC44;
const uint32_t COL_PALE_GREEN   = 0x0088CC88;
const uint32_t COL_PALE_YELLOW  = 0x0088CCCC;

const uint32_t COL_MID_BLUE     = 0x00CC4444;
const uint32_t COL_VIOLET       = 0x00CC4488;
const uint32_t COL_MID_MAGENTA  = 0x00CC44CC;

const uint32_t COL_AZURE        = 0x00CC8844;
const uint32_t COL_PALE_BLUE    = 0x00CC8888;
const uint32_t COL_PALE_MAGENTA = 0x00CC88CC;

const uint32_t COL_MID_TEAL     = 0x00CCCC44;
const uint32_t COL_PALE_TEAL    = 0x00CCCC88;
const uint32_t COL_PALE_GREY    = 0x00CCCCCC;

const uint32_t COL_HINT1 = 0x00111111;
const uint32_t COL_HINT2 = 0x00222222;
const uint32_t COL_HINT3 = 0x00333333;

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
  virtual void operator()(const char* format, ...) = 0;

  virtual void add_char(char c) { operator()("%c", c); }
  virtual void newline() { add_char('\n'); }
  virtual void indent() {}
  virtual void dedent() {}

  //----------

  void dump_bitp(const char* tag, wire b) {
    operator()("%s%c\n", tag, (b & 0x01) ? '1' : '0');
  }

  void dump_bytep(const char* tag, uint8_t b) {
    operator()(tag);
    for (int i = 7; i >= 0; i--) {
      add_char((b & (1 << i)) ? '1' : '0');
    }
    operator()(" 0x%02x %d\n", b, b);
  }
  
  void dump_flags(const char* tag, const void* blob, int byte_count) {
    operator()(tag);
    const uint8_t* d = (const uint8_t*)blob;
    for (int i = byte_count - 1; i >= 0; i--) {
      add_char((d[i] & 0x01) ? '1' : '0');
    }
    add_char('\n');
  }

  void dump_slicep(const char* tag, const void* blob, int byte_count) {
    operator()(tag);
    for (int i = byte_count; i < 8; i++) add_char(' ');

    const uint8_t* d = (const uint8_t*)blob;
    for (int i = byte_count - 1; i >= 0; i--) {
      // high clock = green, low clock = red
      add_char((d[i] & 0x02) ? '\002' : '\003');
      add_char((d[i] & 0x01) ? '1' : '0');
    }
    add_char('\001');

    uint16_t val = 0;
    for (int i = 0; i < byte_count; i++) val |= ((d[i] & 1) << i);

    operator()(" 0x%02x %d\n", val, val);
  }

  void dump_slice2p(const char* tag, const void* blob, int byte_count) {
    operator()(tag);
    for (int i = byte_count; i < 8; i++) add_char(' ');

    const uint8_t* d = (const uint8_t*)blob;
    for (int i = byte_count - 1; i >= 0; i--) {
      // high clock = green, low clock = red
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

    uint16_t val = 0;
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

struct FieldInfo {
  const char* name;
  int offset;
  int size;
};

#define DECLARE_FIELD(T, A) { #A, offsetof(T, A), sizeof(T::A) }
#define END_FIELDS() { nullptr, 0, 0 }

void print_field_at(int offset, const FieldInfo* fields);
bool diff_blobs(const void* blob_a, const void* blob_b, size_t size, uint8_t mask, const FieldInfo* fields);

//-----------------------------------------------------------------------------

// Checks that always happen in every build.
#define CHECK_P(A)  if (!(A)) { LOG_R("ASSERT_P fail @ %s:%d : %s\n", __FILE__, __LINE__, #A); debugbreak(); }
#define CHECK_N(A)  if ((A))  { LOG_R("ASSERT_N fail @ %s:%d : %s\n", __FILE__, __LINE__, #A); debugbreak(); }

// Checks that only happen if "config_dcheck" is on.
#define DCHECK_P(A)   if (config_dcheck) {if (!(A)) { LOG_R("CHECK_P fail @ %s:%d : %s\n", __FILE__, __LINE__, #A);  debugbreak(); }}
#define DCHECK_N(A)   if (config_dcheck) {if ((A))  { LOG_R("CHECK_N fail @ %s:%d : %s\n", __FILE__, __LINE__, #A);  debugbreak(); }}

//-----------------------------------------------------------------------------

template<typename RES, typename ERR>
struct Result {
  Result() = delete;
  explicit Result(RES r) : _err(static_cast<ERR>(0)), _res(r), _ok(1) {}
  Result(ERR e) : _err(e), _res(static_cast<RES>(0)), _ok(0) {}

  //operator RES() const {
  //  CHECK_P(_ok);
  //  return _res;
  //}
  //
  //operator ERR() const {
  //  CHECK_N(_ok);
  //  return _err;
  //}

  bool operator==(const Result& x) const {
    return _ok == x._ok && _res == x._res && _err == x._err;
  }

  //bool operator==(const RES& x) const {
  //  CHECK_P(_ok);
  //  return _res == x;
  //}

  Result& operator &= (const Result r) {
    if (r.is_err()) {
      _res = r._res;
      _err = r._err;
      _ok = r._ok;
    }
    return *this;
  }

  static Result ok() { return Result(static_cast<RES>(0)); }

  bool is_ok()  const { return  _ok; }
  bool is_err() const { return !_ok; }

  RES unwrap() const {
    CHECK_P(_ok);
    return _res;
  }

private:
  //Result() : _err(static_cast<ERR>(0)), _res(static_cast<RES>(0)), _ok(1) {}

  RES _res;
  ERR _err;
  bool _ok;
};

enum struct Error {
  NONE = 0,
  NULLPTR,
  NOT_FOUND,
  CORRUPT,
  MISMATCH
};

typedef Result<uint8_t, Error> GBResult;

//-----------------------------------------------------------------------------

typedef std::vector<uint8_t> blob;

struct BlobStream {
  template<typename T>
  inline bool read(T& t) {
    if (error) return false;

    CHECK_P(b.size() - cursor >= sizeof(uint64_t) + sizeof(T));
    uint64_t size = 0;
    read(&size, sizeof(uint64_t));
    if (size != sizeof(T)) {
      error = true;
      return false;
    }

    read(&t, sizeof(T));
    return true;
  }

  template<typename T>
  inline bool write(const T& t) {
    uint64_t size = sizeof(T);
    write(&size, sizeof(uint64_t));
    write(&t, sizeof(T));
    return true;
  }

  blob rest() {
    auto result = blob(b.begin() + cursor, b.end());
    cursor = b.size();
    return result;
  }

  inline void read(void* dst, size_t size) {
    memcpy(dst, b.data() + cursor, size);
    cursor += size;
  }

  inline void write(const void* src, size_t size) {
    b.resize(cursor + size);
    memcpy(b.data() + cursor, src, size);
    cursor += size;
  }

  blob b;
  size_t cursor = 0;
  bool error = false;
};

//-----------------------------------------------------------------------------

struct IntWrapper {

  IntWrapper() {}
  IntWrapper(const IntWrapper & b) : a(b.a) {}
  explicit IntWrapper(const int b) : a(b) {}

  //operator int & ()             { return a; }
  //operator const int & () const { return a; }
  int unwrap() const { return a; }

  IntWrapper& operator =  (const IntWrapper & rhs)       { a = rhs.a; return *this;}
  IntWrapper& operator =  (const int & rhs)              { a = rhs; return *this;}
  bool        operator == (const IntWrapper & rhs) const { return a == rhs.a; }
  bool        operator <  (const IntWrapper & rhs) const { return a < rhs.a; }

  int a = 0;
};

//-----------------------------------------------------------------------------

template<typename T>
T remap(T x, T a1, T a2, T b1, T b2) {
  double y = (double(x) - double(a1)) / (double(a2) - double(a1));
  double z = y * (double(b2) - double(b1)) + b1;
  return T(z);
}

template<typename T>
T remap_clamp(T x, T a1, T a2, T b1, T b2) {
  double y = (double(x) - double(a1)) / (double(a2) - double(a1));
  if (y < 0) y = 0;
  if (y > 1) y = 1;
  double z = y * (double(b2) - double(b1)) + b1;
  return T(z);
}

template<typename T>
T clamp(T x, T a, T b) {
  if (x < a) x = a;
  if (x > b) x = b;
  return x;
}

template<typename T>
T remap_wrap(T x, T a1, T a2, T b1, T b2) {
  double y = (double(x) - double(a1)) / (double(a2) - double(a1));
  y = fmod(double(y), 1.0);
  double z = y * (double(b2) - double(b1)) + b1;
  return T(z);
}

template<typename T>
void sort(T& a, T& b) {
  if (a > b) {
    auto temp = a;
    a = b;
    b = temp;
  }
}

