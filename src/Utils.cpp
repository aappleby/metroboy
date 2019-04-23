#include "Utils.h"

//-----------------------------------------------------------------------------

void dprintf(const char* fmt, ...) {
  char buf[1024];
  va_list args;
  va_start(args, fmt);
  //vsprintf_s(buf, sizeof(buf), fmt, args);
  vsnprintf(buf, sizeof(buf), fmt, args);
  printf("%s", buf);
}


bool assert_fail(const char* expr, const char* file, int line) {
  dprintf("\nAssertion \"%s\" failed @ %s : %d\n", expr, file, line);
  //__debugbreak();
  assert(false);
  return false;
}

//-----------------------------------------------------------------------------
// FIXME std::string?

char binarybuf[256];

const char* to_binary(uint8_t b) {
  binarybuf[0] = (b & 0x80) ? '1' : '0';
  binarybuf[1] = (b & 0x40) ? '1' : '0';
  binarybuf[2] = (b & 0x20) ? '1' : '0';
  binarybuf[3] = (b & 0x10) ? '1' : '0';
  binarybuf[4] = ':';
  binarybuf[5] = (b & 0x08) ? '1' : '0';
  binarybuf[6] = (b & 0x04) ? '1' : '0';
  binarybuf[7] = (b & 0x02) ? '1' : '0';
  binarybuf[8] = (b & 0x01) ? '1' : '0';
  binarybuf[9] = 0;
  return binarybuf;
}

const char* to_binary(uint8_t lo, uint8_t hi) {
  for (int i = 0; i < 8; i++) {
    int a = (lo >> (7 - i)) & 1;
    int b = (hi >> (7 - i)) & 1;
    int c = b * 2 + a;
    binarybuf[i] = char('0' + c);
  }
  binarybuf[8] = 0;
  return binarybuf;
}

const char* to_binary(uint16_t lo, uint16_t hi) {
  for (int i = 0; i < 16; i++) {
    int a = (lo >> (15 - i)) & 1;
    int b = (hi >> (15 - i)) & 1;
    int c = b * 2 + a;
    binarybuf[i] = char('0' + c);
  }
  binarybuf[16] = 0;
  return binarybuf;
}

//-----------------------------------------------------------------------------

uint8_t flip(uint8_t x) {
  x = ((x & 0b10101010) >> 1) | ((x & 0b01010101) << 1);
  x = ((x & 0b11001100) >> 2) | ((x & 0b00110011) << 2);
  x = ((x & 0b11110000) >> 4) | ((x & 0b00001111) << 4);
  return x;
}

uint8_t flip2(uint8_t b) {
  return uint8_t(((b * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32);
}

//-----------------------------------------------------------------------------
