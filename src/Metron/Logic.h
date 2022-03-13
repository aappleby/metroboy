#pragma once
#include <assert.h>
#include <stdint.h>

#include <type_traits>

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
// far better way to do this.

template <int N>
struct bitsize_to_basetype {};
#define DECLARE_SIZE(T, N)        \
  template <>                     \
  struct bitsize_to_basetype<N> { \
    typedef T type;               \
  };

DECLARE_SIZE(uint8_t, 1);
DECLARE_SIZE(uint8_t, 2);
DECLARE_SIZE(uint8_t, 3);
DECLARE_SIZE(uint8_t, 4);
DECLARE_SIZE(uint8_t, 5);
DECLARE_SIZE(uint8_t, 6);
DECLARE_SIZE(uint8_t, 7);
DECLARE_SIZE(uint8_t, 8);

DECLARE_SIZE(uint16_t, 9);
DECLARE_SIZE(uint16_t, 10);
DECLARE_SIZE(uint16_t, 11);
DECLARE_SIZE(uint16_t, 12);
DECLARE_SIZE(uint16_t, 13);
DECLARE_SIZE(uint16_t, 14);
DECLARE_SIZE(uint16_t, 15);
DECLARE_SIZE(uint16_t, 16);

DECLARE_SIZE(uint32_t, 17);
DECLARE_SIZE(uint32_t, 18);
DECLARE_SIZE(uint32_t, 19);
DECLARE_SIZE(uint32_t, 20);
DECLARE_SIZE(uint32_t, 21);
DECLARE_SIZE(uint32_t, 22);
DECLARE_SIZE(uint32_t, 23);
DECLARE_SIZE(uint32_t, 24);
DECLARE_SIZE(uint32_t, 25);
DECLARE_SIZE(uint32_t, 26);
DECLARE_SIZE(uint32_t, 27);
DECLARE_SIZE(uint32_t, 28);
DECLARE_SIZE(uint32_t, 29);
DECLARE_SIZE(uint32_t, 30);
DECLARE_SIZE(uint32_t, 31);
DECLARE_SIZE(uint32_t, 32);

DECLARE_SIZE(uint64_t, 33);
DECLARE_SIZE(uint64_t, 34);
DECLARE_SIZE(uint64_t, 35);
DECLARE_SIZE(uint64_t, 36);
DECLARE_SIZE(uint64_t, 37);
DECLARE_SIZE(uint64_t, 38);
DECLARE_SIZE(uint64_t, 39);
DECLARE_SIZE(uint64_t, 40);
DECLARE_SIZE(uint64_t, 41);
DECLARE_SIZE(uint64_t, 42);
DECLARE_SIZE(uint64_t, 43);
DECLARE_SIZE(uint64_t, 44);
DECLARE_SIZE(uint64_t, 45);
DECLARE_SIZE(uint64_t, 46);
DECLARE_SIZE(uint64_t, 47);
DECLARE_SIZE(uint64_t, 48);
DECLARE_SIZE(uint64_t, 49);
DECLARE_SIZE(uint64_t, 50);
DECLARE_SIZE(uint64_t, 51);
DECLARE_SIZE(uint64_t, 52);
DECLARE_SIZE(uint64_t, 53);
DECLARE_SIZE(uint64_t, 54);
DECLARE_SIZE(uint64_t, 55);
DECLARE_SIZE(uint64_t, 56);
DECLARE_SIZE(uint64_t, 57);
DECLARE_SIZE(uint64_t, 58);
DECLARE_SIZE(uint64_t, 59);
DECLARE_SIZE(uint64_t, 60);
DECLARE_SIZE(uint64_t, 61);
DECLARE_SIZE(uint64_t, 62);
DECLARE_SIZE(uint64_t, 63);
DECLARE_SIZE(uint64_t, 64);

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
  typedef typename bitsize_to_basetype<WIDTH>::type BASE;
  typedef typename std::make_signed<BASE>::type SBASE;
  typedef typename std::make_unsigned<BASE>::type UBASE;

  BASE x = 0;

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
  void set(BASE y) {
    static const BASE mask = BASE(~0) >> ((sizeof(BASE) * 8) - WIDTH);
    x = y & mask;
  }

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
    const int mask_width = M > WIDTH ? WIDTH : M;
    static const BASE mask = BASE(~0) >> ((sizeof(BASE) * 8) - mask_width);
    x = (x & ~mask) | (y & mask);
    return *this;
  }

  //----------
  // Logics decay to BASE.

  operator BASE() const { return x; }

  SBASE as_signed() const {
    SBASE t = x;
    t <<= (sizeof(SBASE) * 8) - WIDTH;
    t >>= (sizeof(SBASE) * 8) - WIDTH;
    return t;
  }

  UBASE as_unsigned() const { return get(); }

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

    const DST mask = DST(-1ll) >> ((sizeof(DST) * 8) - (hi - lo + 1));
    self = DST((self & ~(mask << LO)) | ((x & mask) << LO));
  }
};

template <int HI, int LO, int WIDTH>
inline auto slice(logic<WIDTH>& x)
    -> bitslice<HI, LO, WIDTH, typename logic<WIDTH>::BASE> {
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
  return logic<WIDTH>::BASE(a) & logic<WIDTH>::BASE(b);
}
template <int WIDTH>
inline logic<WIDTH> operator|(const logic<WIDTH>& a, const logic<WIDTH>& b) {
  return logic<WIDTH>::BASE(a) | logic<WIDTH>::BASE(b);
}
template <int WIDTH>
inline logic<WIDTH> operator^(const logic<WIDTH>& a, const logic<WIDTH>& b) {
  return logic<WIDTH>::BASE(a) ^ logic<WIDTH>::BASE(b);
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
  auto t = x.get();
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
  return (logic<WIDTH1 + WIDTH2>::BASE(a.get()) << WIDTH2) |
         logic<WIDTH1 + WIDTH2>::BASE(b.get());
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

template <int DUPS, int WIDTH>
logic<WIDTH * DUPS> dup(const logic<WIDTH>& a) {
  typename logic<WIDTH* DUPS>::BASE temp = 0;
  for (int i = 0; i < DUPS; i++) {
    temp = temp << WIDTH;
    temp |= a;
  }
  return temp;
}

//------------------------------------------------------------------------------
