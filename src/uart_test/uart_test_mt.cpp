#include <stdio.h>

#include "metron.h"
#include "uart_top.h"
#include "vcd_dump.h"

#if 0
#include <assert.h>
#include <inttypes.h>
#include <chrono>
#include <thread>


#ifdef _MSC_VER

#include <Windows.h>
#include <intrin.h>
#define NOINLINE __declspec(noinline)

#else

#include <unistd.h>
#include <x86intrin.h>
#define NOINLINE __attribute__((noinline))

#endif

const uint64_t OPCODE_OP_IMM = 0x13;

NOINLINE uint64_t test_pack1(uint64_t x) {
  return ((0b00 & 0b11)       << 30) |
         (((x >> 7) & 0b1111) << 26) |
         (((x >> 11) & 0b11)  << 24) |
         (((x >> 5) & 0b1)    << 23) |
         (((x >> 6) & 0b1)    << 22) |
         ((0b00ull & 0b11)    << 20) |
         ((0b00010ull)        << 15) |
         ((0b000ull & 0b111)  << 12) |
         ((0b01ull & 0b11)    << 10) |
         (((x >> 2) & 0b111)  << 7)  |
         ((OPCODE_OP_IMM)     << 0)  ;
}

NOINLINE uint64_t test_pack3(logic<64> x) {

  logic<32> r = cat(
    bx<2>(0),
    bx<4>(x, 10, 7),
    bx<2>(x, 12, 11),
    bx<1>(x, 5),
    bx<1>(x, 6),
    bx<2>(0),
    bx<5>(2),
    bx<3>(0),
    bx<2>(1),
    bx<3>(x, 4, 2),
    bx<7>(OPCODE_OP_IMM)
  );

  return r;
}



int main(int argc, char** arv) {

  int64_t a = 0;
  int64_t b = 0;

  for (int rep = 0; rep < 3; rep++) {
    a = __rdtsc();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    b = __rdtsc();
    printf("ghz %f\n", (b - a) / (300.0 * 1000.0 * 1000.0));
  }

  const int reps = 1000000;

  {
    int64_t min = INT64_MAX;
    for (int rep = 0; rep < reps; rep++)
    {
      a = __rdtsc();
      uint64_t x = 0;
      for (int i = 0; i < 1000; i++) {
        x = test_pack1(x);
      }

      b = __rdtsc();
      int64_t c = b - a;
      if (c < min) {
        min = c;
        printf("0x%016" PRIx64 " %" PRId64 " %d\n", x, c, rep);
      }
    }
    printf("\n");
  }

  {
    int64_t min = INT64_MAX;
    for (int rep = 0; rep < reps; rep++)
    {
      a = __rdtsc();
      uint64_t x = 0;
      for (int i = 0; i < 1000; i++) {
        x = test_pack3(x);
      }
      b = __rdtsc();
      int64_t c = b - a;
      if (c < min) {
        min = c;
        printf("0x%016" PRIx64 " %" PRId64 " %d\n", x, c, rep);
      }
    }
    printf("\n");
  }

  return 0;
}
#endif

#if 1

int main(int argc, char** arv) {


  printf("Metron simulation:\n");
  printf("================================================================================\n");

  const int cycles_per_bit = 3;
  uart_top<cycles_per_bit> top;
  top.init();
  top.tick(0);
  top.tock(0);

  for (int cycle = 0; cycle < 20000; cycle++) {
    bool old_valid = top.o_valid;
    top.tick(1);
    top.tock(1);
    if (!old_valid && top.o_valid) printf("%c", (uint8_t)top.o_data);
    
    if (top.o_done) {
      printf("\n");
      printf("================================================================================\n");
      printf("%d\n", cycle);
      if (top.o_sum == 0x0000b764) {
        printf("All tests pass.\n");
        return 0;
      }
    }
  }

  return -1;
}

#endif