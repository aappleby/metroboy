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

#define LOG_R(...) do { printf("\u001b[38;2;255;128;128m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_G(...) do { printf("\u001b[38;2;128;255;128m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_B(...) do { printf("\u001b[38;2;128;128;255m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_Y(...) do { printf("\u001b[38;2;255;255;128m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);

#define TEST_START(...) do { LOG("%s: ", __FUNCTION__); LOG_B(__VA_ARGS__); LOG("\n\t"); } while(0); int err = 0;
#define TEST_END()      do { LOG("\v"); if (err) LOG_R("%s: FAIL\n", __FUNCTION__); return err; } while(0);

#define EXPECT_EQ(A, B, ...) if ((A) != (B)) { LOG_Y(__VA_ARGS__); LOG("\n"); err++; }
#define EXPECT_NE(A, B, ...) if ((A) == (B)) { LOG_Y(__VA_ARGS__); LOG("\n"); err++; }

#define ASSERT_EQ(A, B, ...) if ((A) != (B)) { LOG_R(__VA_ARGS__); LOG("\n"); err++; TEST_END(); }
#define ASSERT_NE(A, B, ...) if ((A) == (B)) { LOG_R(__VA_ARGS__); LOG("\n"); err++; TEST_END(); }

//-----------------------------------------------------------------------------
