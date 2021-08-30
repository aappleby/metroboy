#pragma once

#include <stdio.h>
#include <stdarg.h>

//-----------------------------------------------------------------------------

inline void LOG(const char* format = "", ...) {
  static int log_indent = 0;
  static int log_cursor_x = 0;
  static char buffer[256];

  va_list args;
  va_start(args, format);
  vsnprintf(buffer, 256, format, args);
  va_end(args);

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
      for (; log_cursor_x < log_indent; log_cursor_x++) putchar(' ');
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

double timestamp();

#define LOG_R(...) do { printf("[%07.3f] \u001b[38;2;255;128;128m", timestamp()); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_G(...) do { printf("[%07.3f] \u001b[38;2;128;255;128m", timestamp()); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_B(...) do { printf("[%07.3f] \u001b[38;2;128;128;255m", timestamp()); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_Y(...) do { printf("[%07.3f] \u001b[38;2;255;255;128m", timestamp()); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);

#define LOG_INDENT() LOG("\t");
#define LOG_DEDENT() LOG("\v");

#define CONCAT2(a, b) a##b
#define CONCAT1(a, b) CONCAT2(a, b)
#define LOG_SCOPE_INDENT() ScopeIndent CONCAT1(scope_indent_, __LINE__)

//-----------------------------------------------------------------------------
