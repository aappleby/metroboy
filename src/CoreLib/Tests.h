#pragma once

#include "Log.h"

//-----------------------------------------------------------------------------

struct TestResults {
  void operator += (TestResults r) {
    expect_count += r.expect_count;
    pass_count += r.pass_count;
    fail_count += r.fail_count;
  }

  int expect_count = 0;
  int pass_count = 0;
  int fail_count = 0;
};

#define TEST_START(...) do { LOG("%s: ", __FUNCTION__); LOG_B(__VA_ARGS__); LOG("\n"); LOG_INDENT(); } while(0); TestResults results;
#define TEST_END()      do { LOG_DEDENT(); if (results.fail_count) { LOG_R("%s: %d failures\n", __FUNCTION__, results.fail_count); } else { results.pass_count++; } return results; } while(0);

#define EXPECT_EQ(A, B, ...) if ((A) != (B)) { LOG_Y("EXPECT_EQ fail: %llx != %llx @ %s : %d\n", uint64_t(A), uint64_t(B), __FILE__, __LINE__); LOG_Y(__VA_ARGS__); LOG("\n"); results.fail_count++; } else { results.expect_count++; }
#define EXPECT_NE(A, B, ...) if ((A) == (B)) { LOG_Y("EXPECT_NE fail: %llx == %llx @ %s : %d\n", uint64_t(A), uint64_t(B), __FILE__, __LINE__); LOG_Y(__VA_ARGS__); LOG("\n"); results.fail_count++; } else { results.expect_count++; }

#define ASSERT_EQ(A, B, ...) if ((A) != (B)) { LOG_R("ASSERT_EQ fail: %llx != %llx @ %s : %d\n", uint64_t(A), uint64_t(B), __FILE__, __LINE__); LOG_R(__VA_ARGS__); LOG("\n"); results.fail_count++; TEST_END(); }
#define ASSERT_NE(A, B, ...) if ((A) == (B)) { LOG_R("ASSERT_NE fail: %llx == %llx @ %s : %d\n", uint64_t(A), uint64_t(B), __FILE__, __LINE__); LOG_R(__VA_ARGS__); LOG("\n"); results.fail_count++; TEST_END(); }

//-----------------------------------------------------------------------------
