#pragma once
#include "Platform.h"

struct Gameboy;

/*__forceinline*/ inline uint32_t xrand()
{
  static uint32_t x = 0;

  if (x == 0) {
    srand((uint32_t)time(0));
    x = rand();
  }

  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return x;
}

void dprintf(const char* fmt, ...);

bool assert_fail(const char* expr, const char* file, int line);
#define zassert(expression) (void)((!!(expression)) || assert_fail(#expression, __FILE__, __LINE__))


const char* to_binary(uint8_t b);
const char* to_binary(uint8_t lo, uint8_t hi);
const char* to_binary(uint16_t lo, uint16_t hi);
uint8_t flip(uint8_t x);
uint8_t flip2(uint8_t b);

//-----------------------------------------------------------------------------
// Bit-width-checking logic type, for sanity tests

template<typename T, int B>
struct ulogic {

  ulogic() {}

  template<typename U>
  ulogic(U x) {
    SDL_assert_release(x >= 0);
    SDL_assert_release(x <= (1 << B) - 1);
    v = T(x);
  }

  template<typename U>
  ulogic& operator = (U x) {
    SDL_assert_release(x >= 0);
    SDL_assert_release(x <= (1 << B) - 1);
    v = T(x);
    return *this;
  }

  operator const T&() const { return v; }

  ulogic& operator--() { return *this = (v - 1); }
  ulogic operator--(int) { ulogic temp = *this; *this = (v - 1); return temp; }
  ulogic& operator++() { return *this = (v + 1); }
  ulogic operator++(int) { ulogic temp = *this; *this = (v + 1); return temp; }

  template<typename U> ulogic& operator +=  (U x) { return *this = (v + x); }
  template<typename U> ulogic& operator -=  (U x) { return *this = (v - x); }
  template<typename U> ulogic& operator *=  (U x) { return *this = (v * x); }
  template<typename U> ulogic& operator /=  (U x) { return *this = (v / x); }
  template<typename U> ulogic& operator &=  (U x) { return *this = (v & x); }
  template<typename U> ulogic& operator |=  (U x) { return *this = (v | x); }
  template<typename U> ulogic& operator ^=  (U x) { return *this = (v ^ x); }
  template<typename U> ulogic& operator >>= (U x) { return *this = (v >> x); }
  template<typename U> ulogic& operator <<= (U x) { return *this = (v << x); }

private:
  T v;
};

#if 0

typedef ulogic<uint8_t, 1>  ubit1_t;
typedef ulogic<uint8_t, 2>  ubit2_t;
typedef ulogic<uint8_t, 3>  ubit3_t;
typedef ulogic<uint8_t, 4>  ubit4_t;
typedef ulogic<uint8_t, 5>  ubit5_t;
typedef ulogic<uint8_t, 6>  ubit6_t;
typedef ulogic<uint8_t, 7>  ubit7_t;
typedef ulogic<uint8_t, 8>  ubit8_t;
typedef ulogic<uint16_t, 9>  ubit9_t;
typedef ulogic<uint16_t, 10> ubit10_t;
typedef ulogic<uint16_t, 11> ubit11_t;
typedef ulogic<uint16_t, 12> ubit12_t;
typedef ulogic<uint16_t, 13> ubit13_t;
typedef ulogic<uint16_t, 14> ubit14_t;
typedef ulogic<uint16_t, 15> ubit15_t;
typedef ulogic<uint16_t, 16> ubit16_t;
typedef ulogic<uint32_t, 32> ubit32_t;

typedef int8_t sbit5_t;
typedef int8_t sbit8_t;
typedef int16_t sbit9_t;

#else

typedef uint8_t  ubit1_t;
typedef uint8_t  ubit2_t;
typedef uint8_t  ubit3_t;
typedef uint8_t  ubit4_t;
typedef uint8_t  ubit5_t;
typedef uint8_t  ubit6_t;
typedef uint8_t  ubit7_t;
typedef uint8_t  ubit8_t;
typedef uint16_t ubit9_t;
typedef uint16_t ubit10_t;
typedef uint16_t ubit11_t;
typedef uint16_t ubit12_t;
typedef uint16_t ubit13_t;
typedef uint16_t ubit14_t;
typedef uint16_t ubit15_t;
typedef uint16_t ubit16_t;

typedef uint32_t ubit32_t;

typedef int8_t sbit5_t;
typedef int8_t sbit8_t;
typedef int16_t sbit9_t;

#endif

//-----------------------------------------------------------------------------
