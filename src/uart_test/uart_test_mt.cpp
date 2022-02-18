#include <stdio.h>

#include "Logic.h"

#include "../CoreLib/Tests.h"
#include "../CoreLib/Log.h"

#include "metron_tools.h"
#include "uart_top.h"

TestResults logic_test_truncate() {
  TEST_INIT("logic_test_truncate");

  logic<10> a = 0b1111111111;
  logic<5>  b = 0;

  // A direct assignment should not compile
  //b = a;

  // Assignment after addition should compile and truncate.
  b = a + 0;
  
  EXPECT_EQ(b, 0b11111);

  TEST_DONE();
}

TestResults logic_test_add() {
  TEST_INIT("logic_test_add");

  logic<8> a = 100;
  logic<8> b = 72;
  logic<8> c = a + b;

  EXPECT_EQ(c, 172);

  TEST_DONE();
}

TestResults logic_test_cat() {
  TEST_INIT("logic_test_cat");

  logic<5> x = cat(logic<2>(0b10), logic<3>(0b101));

  EXPECT_EQ(x[4], 1);
  EXPECT_EQ(x[3], 0);
  EXPECT_EQ(x[2], 1);
  EXPECT_EQ(x[1], 0);
  EXPECT_EQ(x[0], 1);

  TEST_DONE();
}

TestResults logic_test_proxy() {
  TEST_INIT(__FUNCTION__);

  logic<8> a = 0;
  a[3] = 1;
  EXPECT_EQ(8, a);

  TEST_DONE();
}

TestResults logic_test_suite() {
  TEST_INIT("logic_test_suite");

  results += logic_test_cat();
  results += logic_test_add();
  results += logic_test_truncate();
  results += logic_test_proxy();

  TEST_DONE();
}

#if 0
int main(int argc, char** argv) {

  auto r = logic_test_suite();

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