#include "../CoreLib/Tests.h"

#include "test_utils.h"

TestResults test_utils();
TestResults test_logic();
TestResults test_modules();
TestResults test_uart();
TestResults test_ibex();
TestResults test_rvsimple();
TestResults test_deltas();
TestResults test_build();

//------------------------------------------------------------------------------

int main(int argc, char** argv) {

  TestResults results;
  results += test_utils();   // this looks ok
  results += test_logic();   // this looks ok
  results += test_modules();
  //results += test_uart();
  //results += test_ibex();
  //results += test_rvsimple();
  results += test_deltas();
  //results += test_build();

  LOG_G("%s: %6d expect pass\n", __FUNCTION__, results.expect_pass);
  LOG_G("%s: %6d test pass\n", __FUNCTION__,   results.test_pass);

  if (results.expect_fail) LOG_R("%s: %6d expect fail\n", __FUNCTION__, results.expect_fail);
  if (results.test_fail)   LOG_R("%s: %6d test fail\n", __FUNCTION__,   results.test_fail);

  if (results.test_fail) {
    LOG_R("\n");
    LOG_R("########################################\n");
    LOG_R("##               FAIL                 ##\n");
    LOG_R("########################################\n");
    LOG_R("\n");
    return -1;
  }

  return 0;
}

//------------------------------------------------------------------------------
