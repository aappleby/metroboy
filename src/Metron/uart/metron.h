#pragma once

#include <stdio.h>
#include <stdint.h>

constexpr int clog2(uint32_t x) {
  for (int i = 31; i >= 0; i--) if (x & (1 << i)) return i + 1;
  return 0;
}

struct Module {
  void initial() {}
  void reset() {}
};
