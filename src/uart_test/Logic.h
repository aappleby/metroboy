#pragma once
#include <stdint.h>
#include <assert.h>
#include <type_traits>

//----------------------------------------
// Silly template to convert size-in-bits to a primitive type.

template<int N> struct bitsize_to_basetype {};
#define DECLARE_SIZE(T, N) template<> struct bitsize_to_basetype<N> { typedef T type; };

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

//----------------------------------------

template<int N> class logic;
template<int WIDTH, typename SRC, int SRC_WIDTH> class bit_slice;

//------------------------------------------------------------------------------
// Statically sized chunk of bits.

template<int WIDTH = 1>
class logic {
public:

  typedef bitsize_to_basetype<WIDTH>::type   BASE;
  typedef std::make_signed<BASE>::type   SBASE; 
  typedef std::make_unsigned<BASE>::type UBASE;

  //----------
  // Logics can be constructed from their base type, other logics of the same
  // size, or slices of the same size.

  logic();
  logic(const BASE& y);
  logic(const logic& y);

  template<typename SRC, int SRC_WIDTH>
  logic(const bit_slice<WIDTH, SRC, SRC_WIDTH>& y);

  static logic coerce(BASE y);

  //----------
  // Logics can be assigned from their base type, other logics of the same
  // size, or slices of the same size.

  void operator=(BASE y);
  void operator=(const logic& y);
  
  template<typename SRC, int SRC_WIDTH>
  void operator=(const bit_slice<WIDTH, SRC, SRC_WIDTH>& y);

  //----------
  // Logics decay to BASE.

  operator BASE() const { return x; }
  BASE  get() const { return x; }
  SBASE as_signed()   const;
  UBASE as_unsigned() const;

  BASE& as_ref() { return x; }
  const BASE& as_ref() const { return x; }

  //----------
  // Logics can be sliced into bit_slices

  bit_slice<1, BASE, WIDTH> operator[](int i);
  template<int W> bit_slice<W, BASE, WIDTH> slice(int i = 0);
  template<int W> const bit_slice<W, BASE, WIDTH> slice(int i = 0) const;

  static const int width = WIDTH;

private:

  // Logic<>s can _not_ be copied or assigned from logic<>s of other widths.
  //template<int M> logic(const logic<M>& y);
  //template<int M> logic& operator=(const logic<M>& y);

  static_assert(std::is_integral<BASE>::value);
  static_assert(std::is_unsigned<BASE>::value);

  static const BASE mask = BASE(~0) >> ((sizeof(BASE) * 8) - WIDTH);

  BASE x;
};

//------------------------------------------------------------------------------
// BIT_SLICE

template<int WIDTH, typename SRC, int SRC_WIDTH>
class bit_slice {
public:
  static_assert(WIDTH > 0);
  static_assert(SRC_WIDTH > 0);
  static_assert(std::is_integral<SRC>::value);

  typedef logic<WIDTH>::BASE BASE;

  //----------

  bit_slice(SRC& s, int i) : self(s), i(i) {}
  bit_slice(const bit_slice& b) : self(b.self), i(b.i) {}

  //----------

  operator BASE() const { return get(); }
  operator const logic<WIDTH>() const { return get(); }

  //----------

  void operator = (const BASE& x) { set(x); }
  void operator = (const bit_slice& b) { set(b.get()); }
  void operator = (const logic<WIDTH>& b) { set(b.get()); }

  template<typename OTHER_SRC, int OTHER_SRC_WIDTH>
  void operator = (const bit_slice<WIDTH, OTHER_SRC, OTHER_SRC_WIDTH>& b) { set(b.get()); }

  //----------
  
  BASE get() const {
    return (self >> i) & mask;
  }

  template<typename T>
  void set(const T& x) {
    static_assert(std::is_integral<T>::value);
    self &= ~(mask << i);
    self |=  ((x & mask) << i);
  }

private:
  // Slices can _not_ be copied or assigned from slices of other widths.

  template<int OTHER_WIDTH, typename OTHER_SRC, int OTHER_SRC_WIDTH>
  bit_slice(const bit_slice<OTHER_WIDTH, OTHER_SRC, OTHER_SRC_WIDTH>& b);

  template<int OTHER_WIDTH>
  bit_slice& operator = (const logic<OTHER_WIDTH>& b);

  template<int OTHER_WIDTH, typename OTHER_SRC, int OTHER_SRC_WIDTH>
  bit_slice& operator = (const bit_slice<OTHER_WIDTH, OTHER_SRC, OTHER_SRC_WIDTH>& b);

  static const int mask_width = WIDTH > SRC_WIDTH ? SRC_WIDTH : WIDTH;
  static const SRC mask = SRC(-1) >> (sizeof(SRC) * 8 - mask_width);
  SRC& self;
  const int i;

};

//------------------------------------------------------------------------------

template<int WIDTH, typename SRC, int SRC_WIDTH>
inline bit_slice<WIDTH, SRC, SRC_WIDTH> make_prim_slice(SRC& x, int i = 0) {
  return {x, i};
}

template<int WIDTH, typename SRC, int SRC_WIDTH>
inline const bit_slice<WIDTH, SRC, SRC_WIDTH> make_const_prim_slice(const SRC& x, int i = 0) {
  return {const_cast<SRC&>(x), i};
}

template<int WIDTH, int SRC_WIDTH>
inline bit_slice<WIDTH, typename logic<SRC_WIDTH>::BASE, SRC_WIDTH> make_logic_slice(logic<SRC_WIDTH>& x, int i = 0) {
  return {x.as_ref(), i};
}

template<int WIDTH, int SRC_WIDTH>
inline const bit_slice<WIDTH, typename logic<SRC_WIDTH>::BASE, SRC_WIDTH> make_const_logic_slice(const logic<SRC_WIDTH>& x, int i = 0) {
  return {const_cast<logic<SRC_WIDTH>::BASE&>(x.as_ref()), i};
}

//------------------------------------------------------------------------------
// Logic impl

template<int WIDTH>
inline logic<WIDTH>::logic() {
  x = 0;
}

template<int WIDTH>
inline logic<WIDTH>::logic(const BASE& y) {
  x = y & mask;
}

template<int WIDTH>
inline logic<WIDTH>::logic(const logic& y) {
  x = y.x;
}

template<int WIDTH>
template<typename SRC, int SRC_WIDTH>
inline logic<WIDTH>::logic(const bit_slice<WIDTH, SRC, SRC_WIDTH>& y) {
  x = y.get();
}

//----------

template<int WIDTH>
void logic<WIDTH>::operator=(BASE y) {
  x = y & mask;
}

template<int WIDTH>
void logic<WIDTH>::operator=(const logic& y) {
  x = y.get();
}
  
template<int WIDTH>
template<typename SRC, int SRC_WIDTH>
void logic<WIDTH>::operator=(const bit_slice<WIDTH, SRC, SRC_WIDTH>& y) {
  x = y.get();
}

//----------

template<int WIDTH>
logic<WIDTH>::SBASE logic<WIDTH>::as_signed() const {
  auto s = 1 << (WIDTH - 1);
  return (x & ~s) - s;
}

template<int WIDTH>
logic<WIDTH>::UBASE logic<WIDTH>::as_unsigned() const {
  return x;
}

template<int WIDTH>
inline logic<WIDTH> logic<WIDTH>::coerce(BASE y) {
  logic r;
  r.x = y & mask;
  return r;
}

//----------

template<int WIDTH>
bit_slice<1, typename logic<WIDTH>::BASE, WIDTH>
logic<WIDTH>::operator[](int i) {
  return make_prim_slice<1, BASE, WIDTH>(x, i);
}

template<int WIDTH>
template<int SLICE_WIDTH>
bit_slice<SLICE_WIDTH, typename logic<WIDTH>::BASE, WIDTH>
logic<WIDTH>::slice(int i) {
  return make_prim_slice<SLICE_WIDTH, BASE, WIDTH>(x, i);
}

template<int WIDTH>
template<int SLICE_WIDTH>
const bit_slice<SLICE_WIDTH, typename logic<WIDTH>::BASE, WIDTH>
logic<WIDTH>::slice(int i) const {
  return make_const_prim_slice<SLICE_WIDTH, BASE, WIDTH>(x, i);
}

//------------------------------------------------------------------------------
// boolean ops on logic/slice that preserve bit size

template<int WIDTH>                                 inline logic<WIDTH> operator ~ (const logic<WIDTH>& x)           { return ~x.get(); }
template<int WIDTH, typename SRC, int SRC_WIDTH>    inline logic<WIDTH> operator ~ (const bit_slice<WIDTH, SRC, SRC_WIDTH>& x) { return ~x.get(); }

template<int WIDTH>                                 inline logic<WIDTH> operator & (const logic<WIDTH>& a, const logic<WIDTH>& b)                     { return logic<WIDTH>::BASE(a) & logic<WIDTH>::BASE(b); }
template<int WIDTH>                                 inline logic<WIDTH> operator | (const logic<WIDTH>& a, const logic<WIDTH>& b)                     { return logic<WIDTH>::BASE(a) | logic<WIDTH>::BASE(b); }
template<int WIDTH>                                 inline logic<WIDTH> operator ^ (const logic<WIDTH>& a, const logic<WIDTH>& b)                     { return logic<WIDTH>::BASE(a) ^ logic<WIDTH>::BASE(b); }

template<int WIDTH, typename SRC, int SRC_WIDTH>    inline logic<WIDTH> operator & (const logic<WIDTH>& a, const bit_slice<WIDTH, SRC, SRC_WIDTH>& b) { return logic<WIDTH>::BASE(a) & logic<WIDTH>::BASE(b); }
template<int WIDTH, typename SRC, int SRC_WIDTH>    inline logic<WIDTH> operator | (const logic<WIDTH>& a, const bit_slice<WIDTH, SRC, SRC_WIDTH>& b) { return logic<WIDTH>::BASE(a) | logic<WIDTH>::BASE(b); }
template<int WIDTH, typename SRC, int SRC_WIDTH>    inline logic<WIDTH> operator ^ (const logic<WIDTH>& a, const bit_slice<WIDTH, SRC, SRC_WIDTH>& b) { return logic<WIDTH>::BASE(a) ^ logic<WIDTH>::BASE(b); }

template<int WIDTH, typename SRC, int SRC_WIDTH>    inline logic<WIDTH> operator & (const bit_slice<WIDTH, SRC, SRC_WIDTH>& a, const logic<WIDTH>& b) { return logic<WIDTH>::BASE(a) & logic<WIDTH>::BASE(b); }
template<int WIDTH, typename SRC, int SRC_WIDTH>    inline logic<WIDTH> operator | (const bit_slice<WIDTH, SRC, SRC_WIDTH>& a, const logic<WIDTH>& b) { return logic<WIDTH>::BASE(a) | logic<WIDTH>::BASE(b); }
template<int WIDTH, typename SRC, int SRC_WIDTH>    inline logic<WIDTH> operator ^ (const bit_slice<WIDTH, SRC, SRC_WIDTH>& a, const logic<WIDTH>& b) { return logic<WIDTH>::BASE(a) ^ logic<WIDTH>::BASE(b); }
                                                                                                                                                                        
template<int WIDTH, typename SRC1, int SRC_WIDTH1, typename SRC2, int SRC_WIDTH2> inline logic<WIDTH> operator & (const bit_slice<WIDTH, SRC1, SRC_WIDTH1>& a, const bit_slice<WIDTH, SRC2, SRC_WIDTH2>& b) { return logic<WIDTH>::BASE(a) & logic<WIDTH>::BASE(b); }
template<int WIDTH, typename SRC1, int SRC_WIDTH1, typename SRC2, int SRC_WIDTH2> inline logic<WIDTH> operator | (const bit_slice<WIDTH, SRC1, SRC_WIDTH1>& a, const bit_slice<WIDTH, SRC2, SRC_WIDTH2>& b) { return logic<WIDTH>::BASE(a) | logic<WIDTH>::BASE(b); }
template<int WIDTH, typename SRC1, int SRC_WIDTH1, typename SRC2, int SRC_WIDTH2> inline logic<WIDTH> operator ^ (const bit_slice<WIDTH, SRC1, SRC_WIDTH1>& a, const bit_slice<WIDTH, SRC2, SRC_WIDTH2>& b) { return logic<WIDTH>::BASE(a) ^ logic<WIDTH>::BASE(b); }

//------------------------------------------------------------------------------
// Bit-size casting functions

template<int WIDTH, typename SRC>  inline       bit_slice<WIDTH, SRC, sizeof(SRC) * 8>                       bx(      SRC& a, int i = 0)              { return make_prim_slice<WIDTH, SRC, sizeof(SRC) * 8>(a, i); }
template<int WIDTH, typename SRC>  inline const bit_slice<WIDTH, SRC, sizeof(SRC) * 8>                       bx(const SRC& a, int i = 0)              { return make_const_prim_slice<WIDTH, SRC, sizeof(SRC) * 8>(a, i); }
template<int WIDTH, int SRC_WIDTH> inline       bit_slice<WIDTH, typename logic<SRC_WIDTH>::BASE, SRC_WIDTH> bx(      logic<SRC_WIDTH>& a, int i = 0) { return make_logic_slice<WIDTH, SRC_WIDTH>(a, i); }
template<int WIDTH, int SRC_WIDTH> inline const bit_slice<WIDTH, typename logic<SRC_WIDTH>::BASE, SRC_WIDTH> bx(const logic<SRC_WIDTH>& a, int i = 0) { return make_const_logic_slice<WIDTH, SRC_WIDTH>(a, i); }

#define BIT_EXTRACT(A) \
template<typename SRC> inline       bit_slice<A, typename SRC, sizeof(SRC)*8>        b##A(      SRC& a,          int i = 0) { return make_prim_slice<A, SRC, sizeof(SRC)*8>(a, i); } \
template<typename SRC> inline const bit_slice<A, typename SRC, sizeof(SRC)*8>        b##A(const SRC& a,          int i = 0) { return make_const_prim_slice<A, SRC, sizeof(SRC)*8>(a, i); } \
template<int WIDTH>    inline       bit_slice<A, typename logic<WIDTH>::BASE, WIDTH> b##A(      logic<WIDTH>& a, int i = 0) { return make_logic_slice<A, WIDTH>(a, i); } \
template<int WIDTH>    inline const bit_slice<A, typename logic<WIDTH>::BASE, WIDTH> b##A(const logic<WIDTH>& a, int i = 0) { return make_const_logic_slice<A, WIDTH>(a, i); } \

#if 1
BIT_EXTRACT(1);
BIT_EXTRACT(2);
BIT_EXTRACT(3);
BIT_EXTRACT(4);
BIT_EXTRACT(5);
BIT_EXTRACT(6);
BIT_EXTRACT(7);
BIT_EXTRACT(8);

BIT_EXTRACT(9);
BIT_EXTRACT(10);
BIT_EXTRACT(11);
BIT_EXTRACT(12);
BIT_EXTRACT(13);
BIT_EXTRACT(14);
BIT_EXTRACT(15);
BIT_EXTRACT(16);

BIT_EXTRACT(17);
BIT_EXTRACT(18);
BIT_EXTRACT(19);
BIT_EXTRACT(20);
BIT_EXTRACT(21);
BIT_EXTRACT(22);
BIT_EXTRACT(23);
BIT_EXTRACT(24);
BIT_EXTRACT(25);
BIT_EXTRACT(26);
BIT_EXTRACT(27);
BIT_EXTRACT(28);
BIT_EXTRACT(29);
BIT_EXTRACT(30);
BIT_EXTRACT(31);
BIT_EXTRACT(32);

BIT_EXTRACT(33);
BIT_EXTRACT(34);
BIT_EXTRACT(35);
BIT_EXTRACT(36);
BIT_EXTRACT(37);
BIT_EXTRACT(38);
BIT_EXTRACT(39);
BIT_EXTRACT(40);
BIT_EXTRACT(41);
BIT_EXTRACT(42);
BIT_EXTRACT(43);
BIT_EXTRACT(44);
BIT_EXTRACT(45);
BIT_EXTRACT(46);
BIT_EXTRACT(47);
BIT_EXTRACT(48);
BIT_EXTRACT(49);
BIT_EXTRACT(50);
BIT_EXTRACT(51);
BIT_EXTRACT(52);
BIT_EXTRACT(53);
BIT_EXTRACT(54);
BIT_EXTRACT(55);
BIT_EXTRACT(56);
BIT_EXTRACT(57);
BIT_EXTRACT(58);
BIT_EXTRACT(59);
BIT_EXTRACT(60);
BIT_EXTRACT(61);
BIT_EXTRACT(62);
BIT_EXTRACT(63);
BIT_EXTRACT(64);
#endif

//------------------------------------------------------------------------------

template<int WIDTH, typename SRC, int SRC_WIDTH>
inline logic<1> reduce_xor(const bit_slice<WIDTH, SRC, SRC_WIDTH>& x) {
  auto t = x.get();
  t ^= t >> 32;
  t ^= t >> 16;
  t ^= t >> 8;
  t ^= t >> 4;
  t ^= t >> 2;
  t ^= t >> 1;
  return t & 1;
}

template<int WIDTH, typename SRC, int SRC_WIDTH>
inline logic<1> reduce_or(const bit_slice<WIDTH, SRC, SRC_WIDTH>& x) {
  return x.get() != 0;
}

template<int WIDTH, typename SRC, int SRC_WIDTH>
inline logic<1> reduce_and(const bit_slice<WIDTH, SRC, SRC_WIDTH>& x) {
  return x.get() == logic<WIDTH>::mask;
}

template<int WIDTH>
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

template<int WIDTH>
inline logic<1> reduce_or(const logic<WIDTH>& x) {
  return x.get() != 0;
}

template<int WIDTH>
inline logic<1> reduce_and(const logic<WIDTH>& x) {
  return x.get() == logic<WIDTH>::mask;
}

//----------------------------------------
// Concatenate any number of logic<>s and bit_slice<>s into one logic<>.

template<int WIDTH1, int WIDTH2>
inline logic<WIDTH1 + WIDTH2> cat(const logic<WIDTH1>& a, const logic<WIDTH2>& b) {
  return (logic<WIDTH1 + WIDTH2>::BASE(a.get()) << WIDTH2) | logic<WIDTH1 + WIDTH2>::BASE(b.get());
}

template<int WIDTH1, typename SRC1, int SRC_WIDTH1, int WIDTH2>
inline logic<WIDTH1 + WIDTH2> cat(const bit_slice<WIDTH1, SRC1, SRC_WIDTH1>& a, const logic<WIDTH2>& b) {
  return (logic<WIDTH1 + WIDTH2>::BASE(a.get()) << WIDTH2) | logic<WIDTH1 + WIDTH2>::BASE(b.get());
}

template<int WIDTH1, int WIDTH2, typename SRC2, int SRC_WIDTH2>
inline logic<WIDTH1 + WIDTH2> cat(const logic<WIDTH1>& a, const bit_slice<WIDTH2, SRC2, SRC_WIDTH2>& b) {
  return (logic<WIDTH1 + WIDTH2>::BASE(a.get()) << WIDTH2) | logic<WIDTH1 + WIDTH2>::BASE(b.get());
}

template<int WIDTH1, typename SRC1, int SRC_WIDTH1, int WIDTH2, typename SRC2, int SRC_WIDTH2>
inline logic<WIDTH1 + WIDTH2> cat(const bit_slice<WIDTH1, SRC1, SRC_WIDTH1>& a, const bit_slice<WIDTH2, SRC2, SRC_WIDTH2>& b) {
  return (logic<WIDTH1 + WIDTH2>::BASE(a.get()) << WIDTH2) | logic<WIDTH1 + WIDTH2>::BASE(b.get());
}

template<int WIDTH, typename... Args>
inline auto cat(const logic<WIDTH>& a, Args... args) -> logic<WIDTH + decltype(cat(args...))::width> {
  return cat(a, cat(args...));
}

template<int WIDTH, typename SRC, int SRC_WIDTH, typename... Args>
inline auto cat(const bit_slice<WIDTH, SRC, SRC_WIDTH>& a, Args... args) -> logic<WIDTH + decltype(cat(args...))::width> {
  return cat(a, cat(args...));
}

//----------------------------------------
// Duplicate a logic<> or bit_slice<>
//
// logic<3> boop = 0b101;
// logic<9> moop = dup<3>(boop);

template<int DUPS>
struct duper {
  template<int WIDTH>
  static logic<WIDTH * DUPS> dup(const logic<WIDTH>& a) {
    return cat(a, duper<DUPS - 1>::dup(a));
  }

  template<int WIDTH, typename SRC, int SRC_WIDTH>
  static logic<WIDTH * DUPS> dup(const bit_slice<WIDTH, SRC, SRC_WIDTH>& a) {
    return cat(a, duper<DUPS - 1>::dup(a));
  }
};

template<>
struct duper<1> {
  template<int WIDTH>
  static logic<WIDTH> dup(const logic<WIDTH>& a) { return a; }

  template<int WIDTH, typename SRC, int SRC_WIDTH>
  static logic<WIDTH> dup(const bit_slice<WIDTH, SRC, SRC_WIDTH>& a) { return a; }
};

template<int DUPS, int WIDTH>
logic<WIDTH*DUPS> dup(const logic<WIDTH>& a) { return duper<DUPS>::dup<WIDTH>(a); }

template<int DUPS, int WIDTH, typename SRC, int SRC_WIDTH>
logic<WIDTH*DUPS> dup(const bit_slice<WIDTH, SRC, SRC_WIDTH>& a) { return duper<DUPS>::dup<WIDTH>(a); }