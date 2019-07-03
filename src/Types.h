#pragma once

typedef int16_t sample_t;

struct CpuBus {
  uint16_t addr;
  uint8_t data;
  bool read;
  bool write;
};

struct BusOut {
  uint8_t data;
  bool oe;
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
