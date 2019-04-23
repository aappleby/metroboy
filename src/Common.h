#pragma once

extern const uint8_t DMG_ROM_bin[];
extern uint8_t rom_buf[];

extern const uint32_t gb_colors[];

extern const char* op_strings[];
extern const char* cb_strings[];
extern const int op_sizes[];
extern const int op_times_min[];
extern const int op_times_max[];

//-----------------------------------------------------------------------------

inline uint32_t xrand()
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

const char* to_binary(uint8_t b);
const char* to_binary(uint8_t lo, uint8_t hi);
const char* to_binary(uint16_t lo, uint16_t hi);
uint8_t flip(uint8_t x);
uint8_t flip2(uint8_t b);
