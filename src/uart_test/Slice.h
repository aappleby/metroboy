#pragma once

#if 0

//----------------------------------------
// Dynamically sized chunk of bits, where N <= 64.

struct slice {
  slice(uint64_t _x, int _w) : x(_x), w(_w) {}
  operator uint64_t() const { return x; }
  const uint64_t x;
  const int w;
};

/*
#define B1(x)       slice((x >> 0) & 0b00000001, 1)
#define B2(x)       slice((x >> 0) & 0b00000011, 2)
#define B3(x)       slice((x >> 0) & 0b00000111, 3)
#define B4(x)       slice((x >> 0) & 0b00001111, 4)
#define B5(x)       slice((x >> 0) & 0b00011111, 5)
#define B6(x)       slice((x >> 0) & 0b00111111, 6)
#define B7(x)       slice((x >> 0) & 0b01111111, 7)
#define B8(x)       slice((x >> 0) & 0b11111111, 8)
#define BX(x, w)    slice((x >> 0) & (1 << w) - 1, w)

#define S1(x, e)    slice((x >> e) & 0b00000001, 1)
#define S2(x, e)    slice((x >> e) & 0b00000011, 2)
#define S3(x, e)    slice((x >> e) & 0b00000111, 3)
#define S4(x, e)    slice((x >> e) & 0b00001111, 4)
#define S5(x, e)    slice((x >> e) & 0b00011111, 5)
#define S6(x, e)    slice((x >> e) & 0b00111111, 6)
#define S7(x, e)    slice((x >> e) & 0b01111111, 7)
#define S8(x, e)    slice((x >> e) & 0b11111111, 8)
#define SX(x, b, e) slice((x >> e) & (1 << (b - e + 1)) - 1, b - e + 1)
*/

//----------------------------------------
// Concatenate any number of slices into one slice.

inline slice cat(slice a, slice b) { return { (a.x << b.w) | b.x, a.w + b.w }; }

template<typename... Args>
inline slice cat(slice a, Args... args) { return cat(a, cat(args...)); }

#endif