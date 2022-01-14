#include <stdint.h>

typedef const uint8_t wire;
inline wire bit0(uint32_t w) { return wire(w & 1); }
inline wire bit(uint32_t w, int i) { return wire((w >> i) & 1); }

constexpr int clog2(uint32_t x) {
  for (int i = 31; i >= 0; i--) if (x & (1 << i)) return i + 1;
  return 0;
}


template<int N = 1>
struct logic {
  typedef uint32_t base_type;
  static constexpr int width = N;
  static constexpr base_type mask = (1 << width) - 1;

  logic() : x(0) {}
  logic(base_type x) : x(x& mask) {}
  operator base_type() const { return x; }
  base_type x;
};


