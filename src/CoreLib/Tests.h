#pragma once

#include "Log.h"

//-----------------------------------------------------------------------------

struct TestResults {
  void operator += (TestResults r) {
    expect_pass += r.expect_pass;
    expect_fail += r.expect_fail;
    test_pass += r.test_pass;
    test_fail += r.test_fail;
  }

  int expect_pass = 0;
  int expect_fail = 0;

  int test_pass = 0;
  int test_fail = 0;

  bool ok() const { return (expect_fail == 0) && (test_fail == 0); }

  TestResults& finish(const char* function_name) {
    LOG_DEDENT();
    if (!test_pass && !test_fail) {
      expect_fail ? test_fail++ : test_pass++;
    }

    if (test_fail) {
      LOG_R("%s: %d expect failures, %d test failures\n", function_name, expect_fail, test_fail);
    }
    else {
      //LOG_G("PASS!\n");
    }
    return *this;
  }
};

#define TEST_INIT(...) TestResults results; do { LOG("%s: ", __FUNCTION__); LOG_B("" __VA_ARGS__); LOG("\n"); LOG_INDENT(); } while(0);
#define TEST_DONE()    do { return results.finish(__FUNCTION__); } while(0);
#define TEST_PASS()    do { results.test_pass++; return results.finish(__FUNCTION__); } while(0);
#define TEST_FAIL(...) do { results.test_fail++; LOG_R("\n"); LOG_R("%s: ", __FUNCTION__); LOG_R("" __VA_ARGS__); LOG_R("\n"); return results.finish(__FUNCTION__); } while(0);

#define EXPECT(A, ...)       if ((A))        { results.expect_pass++; } else { LOG_Y("EXPECT fail @ %s : %d ",                                          __FILE__, __LINE__); LOG_Y(__VA_ARGS__); LOG("\n"); results.expect_fail++; } 
#define EXPECT_EQ(A, B, ...) if ((A) == (B)) { results.expect_pass++; } else { LOG_Y("EXPECT_EQ fail: %02x != %02x @ %s : %d ", A, B, __FILE__, __LINE__); LOG_Y(__VA_ARGS__); LOG("\n"); results.expect_fail++; } 
#define EXPECT_NE(A, B, ...) if ((A) != (B)) { results.expect_pass++; } else { LOG_Y("EXPECT_NE fail: %02x == %02x @ %s : %d ", A, B, __FILE__, __LINE__); LOG_Y(__VA_ARGS__); LOG("\n"); results.expect_fail++; } 

#define ASSERT_EQ(A, B, ...) if ((A) == (B)) { results.expect_pass++; } else { LOG_R("ASSERT_EQ fail: %02x != %02x @ %s : %d ", A, B, __FILE__, __LINE__); LOG_R(__VA_ARGS__); LOG("\n"); TEST_FAIL(); }
#define ASSERT_NE(A, B, ...) if ((A) != (B)) { results.expect_pass++; } else { LOG_R("ASSERT_NE fail: %02x == %02x @ %s : %d ", A, B, __FILE__, __LINE__); LOG_R(__VA_ARGS__); LOG("\n"); TEST_FAIL(); }

//-----------------------------------------------------------------------------
