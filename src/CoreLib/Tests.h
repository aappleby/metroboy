#pragma once

#include "Log.h"

//-----------------------------------------------------------------------------

#define TEST_START(...) do { LOG("%s: ", __FUNCTION__); LOG_B(__VA_ARGS__); LOG("\n"); LOG_INDENT(); } while(0); int failures = 0;
#define TEST_END()      do { LOG_DEDENT(); if (failures) { LOG_R("%s: %d failures\n", __FUNCTION__, failures); } return failures; } while(0);

#define EXPECT_EQ(A, B, ...) if ((A) != (B)) { LOG_Y("EXPECT_EQ fail: %llx != %llx @ %s : %d\n", uint64_t(A), uint64_t(B), __FILE__, __LINE__); LOG_Y(__VA_ARGS__); LOG("\n"); failures++; }
#define EXPECT_NE(A, B, ...) if ((A) == (B)) { LOG_Y("EXPECT_NE fail: %llx == %llx @ %s : %d\n", uint64_t(A), uint64_t(B), __FILE__, __LINE__); LOG_Y(__VA_ARGS__); LOG("\n"); failures++; }

#define ASSERT_EQ(A, B, ...) if ((A) != (B)) { LOG_R("ASSERT_EQ fail: %llx != %llx @ %s : %d\n", uint64_t(A), uint64_t(B), __FILE__, __LINE__); LOG_R(__VA_ARGS__); LOG("\n"); failures++; TEST_END(); }
#define ASSERT_NE(A, B, ...) if ((A) == (B)) { LOG_R("ASSERT_NE fail: %llx == %llx @ %s : %d\n", uint64_t(A), uint64_t(B), __FILE__, __LINE__); LOG_R(__VA_ARGS__); LOG("\n"); failures++; TEST_END(); }

//-----------------------------------------------------------------------------
