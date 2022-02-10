#pragma once
#include <stdint.h>
#include <assert.h>

//----------------------------------------
// Silly template to convert size-in-bits to a primitive type.

template<int N> struct bitsize_to_basetype {};
#define DECLARE_SIZE(T, N) template<> struct bitsize_to_basetype<N> { typedef T basetype; };

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

template<typename T> struct to_signed;

template<> struct to_signed<uint8_t>  { typedef int8_t  signed_type; };
template<> struct to_signed<uint16_t> { typedef int16_t signed_type; };
template<> struct to_signed<uint32_t> { typedef int32_t signed_type; };
template<> struct to_signed<uint64_t> { typedef int64_t signed_type; };

//----------------------------------------

template<int N> struct logic;
template<typename T> struct bit_proxy;

//----------------------------------------
// Statically sized chunk of bits.

template<int N>
struct logic {
  typedef typename bitsize_to_basetype<N>::basetype basetype;
  static const int width = N;
  static const uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> (64 - N);

  logic()                   { x = 0; }
  logic(uint64_t y)         { x = basetype(y) & mask; }
  operator uint64_t() const { return x; }

  bit_proxy<basetype> operator[](uint64_t i);

  logic operator~() const { return ~uint64_t(x); }

private:
  basetype x;
};

//----------------------------------------

template<typename T>
struct bit_proxy {
  bit_proxy(T& s, uint64_t i) : self(s), i(i) {}

  T& self;
  uint64_t i;

  bit_proxy& operator = (uint64_t x) {
    self &= ~(1 << i);
    self |=  ((x & 1) << i);
    return *this;
  }

  operator logic<1>() const;
  logic<1> operator ~() const;
  logic<1> operator !() const;

  template<typename S>
  inline logic<1> operator& (const bit_proxy<S>& b) const {
    auto ax = (self >> i) & 1;
    auto bx = (b.self >> b.i) & 1;
    return uint64_t(ax & bx);
  }

  operator uint64_t() const { return (self >> i) & 1; }
};

//----------------------------------------

template<typename T>
inline bit_proxy<T>::operator logic<1>() const {
  return uint64_t((self >> i) & 1);
}

template<typename T>
inline logic<1> bit_proxy<T>::operator ~() const {
  return uint64_t((~self >> i) & 1);
}

template<typename T>
inline logic<1> bit_proxy<T>::operator !() const {
  return uint64_t((~self >> i) & 1);
}

//----------------------------------------

template<int N>
bit_proxy<typename logic<N>::basetype> logic<N>::operator[](uint64_t i) {
  bit_proxy<basetype> r(*(basetype*)(this), i);
  return r;
}


//----------------------------------------
// BitExtract helper methods

template<int N> inline logic<N> bx(uint64_t a)        { return a; }
template<int N> inline logic<N> bx(uint64_t a, int e) { return a >> e; }

#define BIT_EXTRACT(A) \
inline logic<A> b##A(uint64_t a)            { return a; } \
inline logic<A> b##A(uint64_t a, int e)     { return a >> e; } \

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

template<int N> inline logic<N> operator&(logic<N> a, logic<N> b) { return a & b; }
template<int N> inline logic<N> operator|(logic<N> a, logic<N> b) { return a | b; }
template<int N> inline logic<N> operator^(logic<N> a, logic<N> b) { return a ^ b; }

//----------------------------------------
// Concatenate any number of logic<>s and bit_proxy<>s into one logic<>.

template<int NA, int NB>
inline logic<NA + NB> cat(logic<NA> a, logic<NB> b) {
  return (a << NB) | b;
}

template<typename T, int N>
inline logic<1 + N> cat(bit_proxy<T> a, logic<N> b) {
  return (a << N) | b;
}

template<int N, typename T>
inline logic<N + 1> cat(logic<N> a, bit_proxy<T> b) {
  return (a << N) | b;
}

template<typename S, typename T>
inline logic<2> cat(bit_proxy<S> a, bit_proxy<T> b) {
  return (a << 1) | b;
}

template<int N, typename... Args>
inline auto cat(logic<N> a, Args... args) -> logic<N + decltype(cat(args...))::width> {
  return cat(a, cat(args...));
}

template<typename T, typename... Args>
inline auto cat(bit_proxy<T> a, Args... args) -> logic<1 + decltype(cat(args...))::width> {
  return cat(b1(a), cat(args...));
}

//----------------------------------------
// Duplicate a logic<> or bit_proxy<>
//
// logic<3> boop = 0b101;
// logic<9> moop = dup<3>(boop);

template<int D>
struct duper {
  template<int N>
  static logic<N* D> dup(logic<N> a) {
    return cat(a, duper<D - 1>::dup(a));
  }
};

template<>
struct duper<1> {
  template<int N>
  static logic<N> dup(logic<N> a) { return a; }
};

template<int D, int N>
logic<D* N> dup(logic<N> a) { return duper<D>::dup(a); }

template<int D, typename T>
logic<D> dup(bit_proxy<T> a) { return duper<D>::dup(b1(a)); }