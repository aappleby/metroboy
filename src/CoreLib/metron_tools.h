#pragma once
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma warning(disable : 4996)   // unsafe fopen
#pragma warning(disable : 26451)  // Very picky arithmetic overflow warning
#endif

//------------------------------------------------------------------------------
// This file contains classes to support Verilog-style bit manipulation in C++.
//
// There are two fundamental types - "logic" for storing blocks of up to 64
// bits, and "bitslice" for manipulating the bits inside logics and primitive
// types in a similar fashion as Verilog's "a[7:2] = 6'b010101" syntax.
//
// Logics can also be type-safely concatenated and replicated - "cat(a,b)" is
// equivalent to Verilog's "{a,b}" and "dup<13>(a)" is equivalent to Verilog's
// "{13{a}}".
//
// Assigning logics of different sizes obey Verilog's rules, which is to say
// there's no typechecking and values are truncated or zero-extended as needed.
// Adding static assertions to disallow some assignments is a future option.

//------------------------------------------------------------------------------
// This template converts size-in-bits to a primitive type. There's probably a
// far better way to do this, hopefully that doesn't #include the universe...

template <int N>
struct bitsize_to_basetype {};
#define DECLARE_SIZE(T, N)        \
  template <>                     \
  struct bitsize_to_basetype<N> { \
    typedef u##T unsigned_type;   \
    typedef T signed_type;        \
  };

DECLARE_SIZE(int8_t, 1);
DECLARE_SIZE(int8_t, 2);
DECLARE_SIZE(int8_t, 3);
DECLARE_SIZE(int8_t, 4);
DECLARE_SIZE(int8_t, 5);
DECLARE_SIZE(int8_t, 6);
DECLARE_SIZE(int8_t, 7);
DECLARE_SIZE(int8_t, 8);

DECLARE_SIZE(int16_t, 9);
DECLARE_SIZE(int16_t, 10);
DECLARE_SIZE(int16_t, 11);
DECLARE_SIZE(int16_t, 12);
DECLARE_SIZE(int16_t, 13);
DECLARE_SIZE(int16_t, 14);
DECLARE_SIZE(int16_t, 15);
DECLARE_SIZE(int16_t, 16);

DECLARE_SIZE(int32_t, 17);
DECLARE_SIZE(int32_t, 18);
DECLARE_SIZE(int32_t, 19);
DECLARE_SIZE(int32_t, 20);
DECLARE_SIZE(int32_t, 21);
DECLARE_SIZE(int32_t, 22);
DECLARE_SIZE(int32_t, 23);
DECLARE_SIZE(int32_t, 24);
DECLARE_SIZE(int32_t, 25);
DECLARE_SIZE(int32_t, 26);
DECLARE_SIZE(int32_t, 27);
DECLARE_SIZE(int32_t, 28);
DECLARE_SIZE(int32_t, 29);
DECLARE_SIZE(int32_t, 30);
DECLARE_SIZE(int32_t, 31);
DECLARE_SIZE(int32_t, 32);

DECLARE_SIZE(int64_t, 33);
DECLARE_SIZE(int64_t, 34);
DECLARE_SIZE(int64_t, 35);
DECLARE_SIZE(int64_t, 36);
DECLARE_SIZE(int64_t, 37);
DECLARE_SIZE(int64_t, 38);
DECLARE_SIZE(int64_t, 39);
DECLARE_SIZE(int64_t, 40);
DECLARE_SIZE(int64_t, 41);
DECLARE_SIZE(int64_t, 42);
DECLARE_SIZE(int64_t, 43);
DECLARE_SIZE(int64_t, 44);
DECLARE_SIZE(int64_t, 45);
DECLARE_SIZE(int64_t, 46);
DECLARE_SIZE(int64_t, 47);
DECLARE_SIZE(int64_t, 48);
DECLARE_SIZE(int64_t, 49);
DECLARE_SIZE(int64_t, 50);
DECLARE_SIZE(int64_t, 51);
DECLARE_SIZE(int64_t, 52);
DECLARE_SIZE(int64_t, 53);
DECLARE_SIZE(int64_t, 54);
DECLARE_SIZE(int64_t, 55);
DECLARE_SIZE(int64_t, 56);
DECLARE_SIZE(int64_t, 57);
DECLARE_SIZE(int64_t, 58);
DECLARE_SIZE(int64_t, 59);
DECLARE_SIZE(int64_t, 60);
DECLARE_SIZE(int64_t, 61);
DECLARE_SIZE(int64_t, 62);
DECLARE_SIZE(int64_t, 63);
DECLARE_SIZE(int64_t, 64);

template <typename T>
struct always_false {
  enum { value = false };
};
//------------------------------------------------------------------------------
// A logic behaves like an unsigned integer with any number of bits, up to the
// largest primitive type in bitsize_to_basetype above.

template <int WIDTH = 1>
class logic {
 public:
  //----------
  // A logic's internal representation is just the smallest unsigned primitive
  // type that can hold them.

  static const int width = WIDTH;
  typedef typename bitsize_to_basetype<WIDTH>::unsigned_type BASE;
  typedef typename bitsize_to_basetype<WIDTH>::unsigned_type UBASE;
  typedef typename bitsize_to_basetype<WIDTH>::signed_type SBASE;

  BASE x = 0;
  static const BASE mask = BASE(~BASE(0)) >> ((sizeof(BASE) * 8) - WIDTH);
  static const BASE max = BASE(~BASE(0)) >> ((sizeof(BASE) * 8) - WIDTH);

  //----------
  // Logics can be constructed and assigned from their base type or other logics
  // of the same size.

  logic() = default;

  logic(const logic& y) = default;
  logic(const BASE& y) { set(y); }

  logic& operator=(const logic& y) = default;
  void operator=(BASE y) { set(y); }

  //----------
  // Logics have a getter, setter, and 'coercer' for convenience.

  BASE get() const { return x; }
  void set(BASE y) { x = y & mask; }

  template <typename T>
  static logic coerce(const T& a) {
    return logic(BASE(a));
  }

  //----------
  // Logics cannot be constructed from logics of other sizes. If there's a good
  // reason to do this, let me know.

  template <int M>
  logic(const logic<M>& y) = delete;

  //----------
  // Assigning from logics of other sizes truncates or zero-extends the source
  // as needed. If a "strict" mode is needed, we can add static_asserts to this
  // method.

  template <int M>
  logic& operator=(const logic<M>& y) {
    set(BASE(y.x));
    return *this;
  }

  //----------
  // Disallow using "<=" with logic<>s, as it means "non-blocking assign" in
  // Verilog and "less than or equal" in C - a typo while porting could cause
  // unexpected behavior.

  // Maybe this isn't needed? FIXME - We should add a test...

#if 0
  template <typename T>
  logic& operator<=(const T& t) {
    static_assert(always_false<T>::value, "Using <= with logic<> is forbidden");
    return *this;
  }
#endif

  //----------
  // Logics decay to BASE.

  operator BASE() const { return x; }

  SBASE as_signed() const {
    if (sizeof(SBASE) == (WIDTH * 8)) {
      return SBASE(x);
    } else {
      SBASE t = x;
      t <<= (sizeof(SBASE) * 8) - WIDTH;
      t >>= (sizeof(SBASE) * 8) - WIDTH;
      return t;
    }
  }

  UBASE as_unsigned() const { return x; }

  //----------
  // Logics can be indexed like a bit array.

  logic<1> operator[](int i) const { return (x >> i) & 1; }
};

//------------------------------------------------------------------------------
// Helper struct to emulate Verilog's "foo[7:2] = bar;" slice assignment syntax.

// Using this will probably break Metron's multiple-write detection, so... use
// sparingly.

template <int HI, int LO, int WIDTH, typename DST>
struct bitslice {
  DST& self;

  bitslice(DST& s) : self(s) {}
  bitslice(const bitslice& b) = delete;
  bitslice& operator=(const bitslice& b) = delete;

  void operator=(logic<HI - LO + 1> x) {
    int hi = HI;
    int lo = LO;
    if (hi > WIDTH - 1) hi = WIDTH - 1;

    const DST mask = (~DST(0)) >> ((sizeof(DST) * 8) - (hi - lo + 1));
    self = DST((self & ~(mask << LO)) | ((x & mask) << LO));
  }
};

template <int HI, int LO, int WIDTH>
inline bitslice<HI, LO, WIDTH, typename logic<WIDTH>::BASE> slice(
    logic<WIDTH>& x) {
  return {x.x};
}

//------------------------------------------------------------------------------
// Boolean operations applied to logics of the same size produce logics of the
// same size.

template <int WIDTH>
inline logic<WIDTH> operator~(const logic<WIDTH>& x) {
  return ~x.get();
}
template <int WIDTH>
inline logic<WIDTH> operator&(const logic<WIDTH>& a, const logic<WIDTH>& b) {
  return typename logic<WIDTH>::BASE(a) & typename logic<WIDTH>::BASE(b);
}
template <int WIDTH>
inline logic<WIDTH> operator|(const logic<WIDTH>& a, const logic<WIDTH>& b) {
  return typename logic<WIDTH>::BASE(a) | typename logic<WIDTH>::BASE(b);
}
template <int WIDTH>
inline logic<WIDTH> operator^(const logic<WIDTH>& a, const logic<WIDTH>& b) {
  return typename logic<WIDTH>::BASE(a) ^ typename logic<WIDTH>::BASE(b);
}

//------------------------------------------------------------------------------
// Size-casting logics and creating slices is so common that it's helpful to
// have abbreviations. "bN(x)" produces logic<N>(x), "sN(x)" produces a
// bitslice<N...>(x). Both can also take an optional offset.
//
// logic<15> a = 1234;
// logic<7> b = b7(a, 2);
//
// logic<47> c = 1234;
// s5(c, 42) = 17;

template <int WIDTH, typename SRC>
inline const logic<WIDTH> bx(const SRC& a, int offset = 0) {
  return logic<WIDTH>::coerce(a >> offset);
}
template <int WIDTH, int SRC_WIDTH>
inline const logic<WIDTH> bx(const logic<SRC_WIDTH>& a, int offset = 0) {
  return logic<WIDTH>::coerce(a.get() >> offset);
}

#if 0

#define DECLARE_BN_SN_HELPERS(WIDTH)                                           \
  template <typename SRC>                                                      \
  inline bitslice<WIDTH, SRC, sizeof(SRC) * 8> s##WIDTH(SRC& a,                \
                                                        int offset = 0) {      \
    return slice<WIDTH, SRC, sizeof(SRC) * 8>(a, offset);                      \
  }                                                                            \
  template <int SRC_WIDTH>                                                     \
  inline bitslice<WIDTH, typename logic<SRC_WIDTH>::BASE, SRC_WIDTH> s##WIDTH( \
      logic<SRC_WIDTH>& a, int offset = 0) {                                   \
    return slice<WIDTH, logic<SRC_WIDTH>::BASE, SRC_WIDTH>(a.x, offset);       \
  }                                                                            \
  template <typename SRC>                                                      \
  inline const logic<WIDTH> b##WIDTH(const SRC& a, int offset = 0) {           \
    return logic<WIDTH>::coerce(a >> offset);                                  \
  }                                                                            \
  template <int SRC_WIDTH>                                                     \
  inline const logic<WIDTH> b##WIDTH(const logic<SRC_WIDTH>& a,                \
                                     int offset = 0) {                         \
    return logic<WIDTH>::coerce(a >> offset);                                  \
  }

#endif

#define DECLARE_BN_SN_HELPERS(WIDTH)                                 \
  template <typename SRC>                                            \
  inline const logic<WIDTH> b##WIDTH(const SRC& a, int offset = 0) { \
    return logic<WIDTH>::coerce(a >> offset);                        \
  }                                                                  \
  template <int SRC_WIDTH>                                           \
  inline const logic<WIDTH> b##WIDTH(const logic<SRC_WIDTH>& a,      \
                                     int offset = 0) {               \
    return logic<WIDTH>::coerce(a >> offset);                        \
  }

DECLARE_BN_SN_HELPERS(1);
DECLARE_BN_SN_HELPERS(2);
DECLARE_BN_SN_HELPERS(3);
DECLARE_BN_SN_HELPERS(4);
DECLARE_BN_SN_HELPERS(5);
DECLARE_BN_SN_HELPERS(6);
DECLARE_BN_SN_HELPERS(7);
DECLARE_BN_SN_HELPERS(8);

DECLARE_BN_SN_HELPERS(9);
DECLARE_BN_SN_HELPERS(10);
DECLARE_BN_SN_HELPERS(11);
DECLARE_BN_SN_HELPERS(12);
DECLARE_BN_SN_HELPERS(13);
DECLARE_BN_SN_HELPERS(14);
DECLARE_BN_SN_HELPERS(15);
DECLARE_BN_SN_HELPERS(16);

DECLARE_BN_SN_HELPERS(17);
DECLARE_BN_SN_HELPERS(18);
DECLARE_BN_SN_HELPERS(19);
DECLARE_BN_SN_HELPERS(20);
DECLARE_BN_SN_HELPERS(21);
DECLARE_BN_SN_HELPERS(22);
DECLARE_BN_SN_HELPERS(23);
DECLARE_BN_SN_HELPERS(24);
DECLARE_BN_SN_HELPERS(25);
DECLARE_BN_SN_HELPERS(26);
DECLARE_BN_SN_HELPERS(27);
DECLARE_BN_SN_HELPERS(28);
DECLARE_BN_SN_HELPERS(29);
DECLARE_BN_SN_HELPERS(30);
DECLARE_BN_SN_HELPERS(31);
DECLARE_BN_SN_HELPERS(32);

DECLARE_BN_SN_HELPERS(33);
DECLARE_BN_SN_HELPERS(34);
DECLARE_BN_SN_HELPERS(35);
DECLARE_BN_SN_HELPERS(36);
DECLARE_BN_SN_HELPERS(37);
DECLARE_BN_SN_HELPERS(38);
DECLARE_BN_SN_HELPERS(39);
DECLARE_BN_SN_HELPERS(40);
DECLARE_BN_SN_HELPERS(41);
DECLARE_BN_SN_HELPERS(42);
DECLARE_BN_SN_HELPERS(43);
DECLARE_BN_SN_HELPERS(44);
DECLARE_BN_SN_HELPERS(45);
DECLARE_BN_SN_HELPERS(46);
DECLARE_BN_SN_HELPERS(47);
DECLARE_BN_SN_HELPERS(48);
DECLARE_BN_SN_HELPERS(49);
DECLARE_BN_SN_HELPERS(50);
DECLARE_BN_SN_HELPERS(51);
DECLARE_BN_SN_HELPERS(52);
DECLARE_BN_SN_HELPERS(53);
DECLARE_BN_SN_HELPERS(54);
DECLARE_BN_SN_HELPERS(55);
DECLARE_BN_SN_HELPERS(56);
DECLARE_BN_SN_HELPERS(57);
DECLARE_BN_SN_HELPERS(58);
DECLARE_BN_SN_HELPERS(59);
DECLARE_BN_SN_HELPERS(60);
DECLARE_BN_SN_HELPERS(61);
DECLARE_BN_SN_HELPERS(62);
DECLARE_BN_SN_HELPERS(63);
DECLARE_BN_SN_HELPERS(64);

//------------------------------------------------------------------------------
// We can't replicate Verilog's "^A" syntax, but we can provide equivalent
// reduce_xor/or/and funcs.

template <int WIDTH>
inline logic<1> reduce_xor(const logic<WIDTH>& x) {
  uint64_t t = x.get();
  t ^= t >> 32;
  t ^= t >> 16;
  t ^= t >> 8;
  t ^= t >> 4;
  t ^= t >> 2;
  t ^= t >> 1;
  return t & 1;
}

template <int WIDTH>
inline logic<1> reduce_or(const logic<WIDTH>& x) {
  return x.get() != 0;
}

template <int WIDTH>
inline logic<1> reduce_and(const logic<WIDTH>& x) {
  return x.get() == logic<WIDTH>::mask;
}

//------------------------------------------------------------------------------
// Concatenate any number of logic<>s into one logic<>, big-endian-style.
//
// logic<2> a = 0b11;
// logic<3> b = 0b000;
// logic<4> c = 0b1101;
// logic<9> d = cat(a, b, c);
// assert(d == 0b110001101);

template <int WIDTH1, int WIDTH2>
inline logic<WIDTH1 + WIDTH2> cat(const logic<WIDTH1>& a,
                                  const logic<WIDTH2>& b) {
  return (typename logic<WIDTH1 + WIDTH2>::BASE(a.get()) << WIDTH2) |
         typename logic<WIDTH1 + WIDTH2>::BASE(b.get());
}

template <int WIDTH, typename... Args>
inline auto cat(const logic<WIDTH>& a, Args... args)
    -> logic<WIDTH + decltype(cat(args...))::width> {
  return cat(a, cat(args...));
}

//------------------------------------------------------------------------------
// Duplicate a logic<>.
//
// logic<3> boop = 0b101;
// logic<9> moop = dup<3>(boop);
// assert(moop == 0b101101101);

constexpr uint64_t dup_pattern(int width, int dups) {
  uint64_t p = 0;
  for (int i = 0; i < dups; i++) {
    p <<= width;
    p |= 1;
  }
  return p;
}

template <int DUPS, int WIDTH>
inline logic<WIDTH * DUPS> dup(const logic<WIDTH>& a) {
  const uint64_t p = dup_pattern(WIDTH, DUPS);
  return p * a;
}

//-----------------------------------------------------------------------------

template <int DST_WIDTH, int SRC_WIDTH>
inline logic<DST_WIDTH> sign_extend(const logic<SRC_WIDTH> a) {
  static_assert(DST_WIDTH >= SRC_WIDTH);
  return cat(dup<DST_WIDTH - SRC_WIDTH + 1>(a[SRC_WIDTH - 1]),
             bx<SRC_WIDTH - 1>(a));
}

//------------------------------------------------------------------------------
// Trivial support for Verilog's "+foo=bar" test arg syntax.
// This only works for $value$plusargs("NAME=%s", s);

inline std::vector<std::string>& get_plusargs() {
  static std::vector<std::string> plusargs;
  return plusargs;
}

inline void metron_init(int argc, const char** argv) {
  get_plusargs().clear();
  for (int argi = 0; argi < argc; argi++) {
    if (argv[argi][0] == '+') {
      get_plusargs().push_back(argv[argi] + 1);
    }
  }
}

inline void value_plusargs(const char* fmt, std::string& out) {
  int prefix_len = 0;
  const char* cursor = fmt;
  while (*cursor != '=' && *cursor != 0) {
    prefix_len++;
    cursor++;
  }

  std::string prefix(fmt, fmt + prefix_len);

  for (const auto& arg : get_plusargs()) {
    if (arg.starts_with(prefix)) {
      out = arg.c_str() + prefix_len + 1;
    }
  }
}

//------------------------------------------------------------------------------
// "Magic" constant that gets translated to 'x' in Verilog

static const uint64_t DONTCARE = 0;

//------------------------------------------------------------------------------

constexpr int clog2(uint64_t x) {
  if (x == 0) return 0;
  x--;
  for (int i = 63; i >= 0; i--)
    if (x & (uint64_t(1) << i)) return i + 1;
  return 1;
}

static_assert(clog2(0) == 0);
static_assert(clog2(1) == 1);
static_assert(clog2(2) == 1);
static_assert(clog2(3) == 2);
static_assert(clog2(255) == 8);
static_assert(clog2(256) == 8);
static_assert(clog2(257) == 9);

constexpr uint64_t pow2(int x) { return (1ull << x); }

void parse_hex(const char* src_filename, void* dst_data, int dst_size);

//------------------------------------------------------------------------------
// 'end' is INCLUSIVE

inline void readmemh(const char* path, void* mem, int begin, int end) {
  memset(mem, 0, end - begin + 1);
  parse_hex(path, (uint8_t*)mem + begin, end - begin + 1);
}

inline void readmemh(const std::string& path, void* mem, int begin, int end) {
  memset(mem, 0, end - begin + 1);
  parse_hex(path.c_str(), (uint8_t*)mem + begin, end - begin + 1);
}

inline void readmemh(const char* path, void* mem) {
  parse_hex(path, (uint8_t*)mem, 0xFFFFFFFF);
}

inline void readmemh(const std::string& path, void* mem) {
  parse_hex(path.c_str(), (uint8_t*)mem, 0xFFFFFFFF);
}

//----------------------------------------

inline int display(const char* fmt, ...) {
  char buffer[256];
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(buffer, 256, fmt, args);
  va_end(args);
  fwrite(buffer, 1, len, stdout);
  // TinyLog::get().printf(0x88CCFF, "MT> ");
  // TinyLog::get().print_buffer(0x88CCFF, buffer, len);
  // TinyLog::get().printf(0x88CCFF, "\n");
  return len;
}

inline int write(const char* fmt, ...) {
  char buffer[256];
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(buffer, 256, fmt, args);
  va_end(args);
  fwrite(buffer, 1, len, stdout);
  // TinyLog::get().printf(0x88CCFF, "MT> ");
  // TinyLog::get().print_buffer(0x88CCFF, buffer, len);
  return len;
}

//----------------------------------------
// Verilog's signed right shift doesn't work quite the same as C++'s, so we
// patch around it here.

// FIXME get rid of this now that as_signed() works correctly

template <int WIDTH>
inline logic<WIDTH> sra(logic<WIDTH> x, int s) {
  return x.as_signed() >> s;
}

//----------------------------------------

inline int to_hex(uint8_t c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  else if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  else if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  else
    return -1;
}

//------------------------------------------------------------------------------

inline void parse_hex(const char* src_filename, void* dst_data, int dst_size) {
  FILE* f = fopen(src_filename, "rb");
  fseek(f, 0, SEEK_END);
  int src_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  // Stick a chunk of nulls on the end so we don't have to worry about reading a
  // byte past the buffer.
  void* src_data = malloc(src_size + 256);
  memset(src_data, 0, src_size + 256);
  size_t count = fread(src_data, 1, src_size, f);
  (void)count;
  fclose(f);

  uint8_t* sc = (uint8_t*)src_data;
  uint8_t* sc_end = sc + src_size;

  uint8_t* dc = (uint8_t*)dst_data;
  uint8_t* dc_end = dc + (dst_size == -1 ? 0xFFFFFFFF : dst_size);

  while (sc[0] && sc < sc_end) {
    // Skip single-line comments
    if (sc[0] == '/' && sc[1] == '/') {
      while (sc[0] && sc[0] != '\n') sc++;
      sc++;
      continue;
    }

    // Skip multi-line comments
    if (sc[0] == '/' && sc[1] == '*') {
      while (sc[0] && (sc[0] != '*' || sc[1] != '/')) sc++;
      sc += 2;
      continue;
    }

    // Skip whitespace
    if (isspace(sc[0])) {
      sc++;
      continue;
    }

    // Current char isn't a comment or whitespace, so check for address marker.
    bool is_addr = false;
    if (sc[0] == '@') {
      is_addr = true;
      sc++;
    }

    // We should be at a big-endian hex value now, decode it.
    int chunk_data = 0;
    int chunk_size = 0;
    while (sc[0]) {
      int d = to_hex(sc[0]);
      if (d == -1) break;
      chunk_data <<= 4;
      chunk_data |= d;
      chunk_size++;
      sc++;
    }

    if (!chunk_size || (chunk_size & 1)) {
      // KCOV_OFF
      printf("Error loading %s: Invalid vmem character 0x%02x (%c)\n",
             src_filename, sc[0], sc[0]);
      // LOG_R("Error loading %s: Invalid vmem character 0x%02x (%c)\n",
      //        src_filename, sc[0], sc[0]);
      return;
      // KCOV_ON
    }

    // Store hex value in address or in output stream, little-endian.
    if (is_addr) {
      dc = (uint8_t*)dst_data + (chunk_data * 4);
    } else {
      for (; chunk_size; chunk_size -= 2) {
        if (dc < dc_end) *dc++ = chunk_data & 0xFF;
        chunk_data >>= 8;
      }
    }
  }

  free(src_data);
}

//------------------------------------------------------------------------------

/*
void print_hex(const char* buf_name, void* src_data, int src_size) {
  char buf_name[512];
  strcpy(buf_name, src_filename);
  for (char* cursor = buf_name; *cursor; cursor++) {
    if (*cursor == '.') *cursor = '_';
  }

  LOG_G("uint8_t %s[%d] = {\n", buf_name, src_size);
  int dst_cursor = 0;
  for (int y = 0; y < dst_size / 16; y++) {
    LOG_G("  ");
    for (int x = 0; x < 16; x++) {
      LOG_G("%02x, ", dst_data[dst_cursor++]);
    }
    LOG_G("\n");
  }
  LOG_G("};\n");
}
*/

//------------------------------------------------------------------------------

//#define IV_TEST(A, B)

//------------------------------------------------------------------------------
