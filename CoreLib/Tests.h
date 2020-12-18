#pragma once

#include <stdio.h>
#include <stdarg.h>

//-----------------------------------------------------------------------------

inline void LOG(const char* format = "", ...) {
  static int log_indent = 0;
  static int log_cursor_x = 0;
  static char buffer[256];

  va_list args;
  va_start (args, format);
  vsnprintf(buffer, 256, format, args);
  va_end (args);

  for (char* b = buffer; *b; b++) {
    if (*b == '\n') {
      putchar(*b);
      log_cursor_x = 0;
    }
    else if (*b == '\t') {
      log_indent += 2;
    }
    else if (*b == '\v') {
      log_indent -= 2;
    }
    else {
      for (;log_cursor_x < log_indent; log_cursor_x++) putchar(' ');
      putchar(*b);
    }
  }
}

struct ScopeIndent {
  ScopeIndent() {
    LOG("\t");
  }
  ~ScopeIndent() {
    LOG("\v");
  }
};


#define LOG_R(...) do { printf("\u001b[38;2;255;128;128m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_G(...) do { printf("\u001b[38;2;128;255;128m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_B(...) do { printf("\u001b[38;2;128;128;255m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_Y(...) do { printf("\u001b[38;2;255;255;128m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);

#define LOG_INDENT() LOG("\t");
#define LOG_DEDENT() LOG("\v");

#define CONCAT2(a, b) a##b
#define CONCAT1(a, b) CONCAT2(a, b)
#define LOG_SCOPE_INDENT() ScopeIndent CONCAT1(scope_indent_, __LINE__)

#define TEST_START(...) do { LOG("%s: ", __FUNCTION__); LOG_B(__VA_ARGS__); LOG("\n\t"); } while(0); int failures = 0;
#define TEST_END()      do { LOG("\v"); if (failures) LOG_R("%s: FAIL\n", __FUNCTION__); return failures; } while(0);

//#define EXPECT_EQ(A, B)      if ((A) != (B)) { LOG_Y("EXPECT_EQ fail: %s @ %d\n", __FILE__, __LINE__); LOG_Y("EXPECT_EQ fail: 0x%llx != 0x%llx\n", uint64_t(A), uint64_t(B)); failures++; }
//#define EXPECT_NE(A, B)      if ((A) == (B)) { LOG_Y("EXPECT_NE fail: %s @ %d\n", __FILE__, __LINE__); LOG_Y("EXPECT_NE fail: 0x%llx == 0x%llx\n", uint64_t(A), uint64_t(B)); failures++; }

#define EXPECT_EQ(A, B, ...) if ((A) != (B)) { LOG_Y("EXPECT_EQ fail: %s @ %d\n", __FILE__, __LINE__); LOG_Y(__VA_ARGS__); failures++; }
#define EXPECT_NE(A, B, ...) if ((A) == (B)) { LOG_Y("EXPECT_NE fail: %s @ %d\n", __FILE__, __LINE__); LOG_Y(__VA_ARGS__); failures++; }

#define ASSERT_EQ(A, B, ...) if ((A) != (B)) { LOG_R("ASSERT_EQ fail: %s @ %d\n", __FILE__, __LINE__); LOG_R("ASSERT_EQ fail: 0x%llx != 0x%llx\n", uint64_t(A), uint64_t(B)); LOG_R(__VA_ARGS__); LOG("\n"); failures++; TEST_END(); }
#define ASSERT_NE(A, B, ...) if ((A) == (B)) { LOG_R("ASSERT_NE fail: %s @ %d\n", __FILE__, __LINE__); LOG_R("ASSERT_NE fail: 0x%llx == 0x%llx\n", uint64_t(A), uint64_t(B)); LOG_R(__VA_ARGS__); LOG("\n"); failures++; TEST_END(); }

//-----------------------------------------------------------------------------
