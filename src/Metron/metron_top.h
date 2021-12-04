#include <stdint.h>

typedef const uint8_t wire;
inline wire bit0(uint32_t w) { return wire(w & 1); }
inline wire bit(uint32_t w, int i) { return wire((w >> i) & 1); }
