#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
//#include <intrin.h>
#include <chrono>
#include <thread>
#include <time.h>

//-----------------------------------------------------------------------------

inline void log_set_color(uint32_t color) {
  static uint32_t log_color = 0;
  if (color == log_color) return;

  if (color == 0) {
    printf("\u001b[0m");
  }
  else {
    printf("\u001b[38;2;%d;%d;%dm", (color >> 0) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
  }

  log_color = color;
}

//-----------------------------------------------------------------------------

inline void log_prefix() {
  timespec ts;
  (void)timespec_get(&ts, TIME_UTC);
  uint64_t now = ts.tv_sec * 1000000000ull + ts.tv_nsec;

  static uint64_t time_origin = 0;
  if (!time_origin) time_origin = now;

  log_set_color(0);
  printf("[%07.3f] ", double(now - time_origin) / 1.0e9);
}

//-----------------------------------------------------------------------------

inline void log_print(uint32_t color, const char* buffer, int len) {
  static int log_indent = 0;
  static bool log_start_line = true;

  log_set_color(color);
  for (int i = 0; i < len; i++) {
    auto c = buffer[i];

    if (c == '\t') {
      log_indent += 2;
    }
    else if (c == '\v') {
      log_indent -= 2;
    }
    else {
      if (log_start_line) {
        log_prefix();
        log_set_color(color);
        for (int j = 0; j < log_indent; j++) putchar(' ');
        log_start_line = false;
      }

      putchar(c);
      if (c == '\n') log_start_line = true;
    }
  }
  log_set_color(0);
}

//-----------------------------------------------------------------------------

inline void log_printf(uint32_t color, const char* format = "", ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  int len = vsnprintf(buffer, 256, format, args);
  va_end(args);
  log_print(color, buffer, len);
}

//-----------------------------------------------------------------------------

#define LOG(...)   log_printf(0x00000000, __VA_ARGS__);
#define LOG_R(...) log_printf(0x008888FF, __VA_ARGS__);
#define LOG_G(...) log_printf(0x0088FF88, __VA_ARGS__);
#define LOG_B(...) log_printf(0x00FF8888, __VA_ARGS__);

#define LOG_C(...) log_printf(0x00FFFF88, __VA_ARGS__);
#define LOG_M(...) log_printf(0x00FF88FF, __VA_ARGS__);
#define LOG_Y(...) log_printf(0x0088FFFF, __VA_ARGS__);
#define LOG_W(...) log_printf(0x00FFFFFF, __VA_ARGS__);

#define LOG_INDENT() log_printf(0, "\t")
#define LOG_DEDENT() log_printf(0, "\v")

struct LogIndenter {
  LogIndenter()  { LOG_INDENT(); }
  ~LogIndenter() { LOG_DEDENT(); }
};

#define LOG_INDENT_SCOPE() LogIndenter indenter##__LINE__;

//-----------------------------------------------------------------------------
