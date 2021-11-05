#pragma once
#include "CoreLib/Types.h"

#include <stdio.h>
#include <stdarg.h>
#include <string>

//-----------------------------------------------------------------------------

struct StringDumper : public Dumper {
  std::string s;

  const char* c_str() {
    return s.c_str();
  }

  void add_char(char c) override {
    s.push_back(c);
  }

  void operator()(const char* format, ...) override {
    char source_buf[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (source_buf, 1024 ,format, args);
    va_end (args);
    s.append(source_buf);
  }
  void clear() { s.clear(); }
};

//-----------------------------------------------------------------------------

struct ConsoleDumper : public Dumper {
  void operator()(const char* format, ...) override {
    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);
  }
};

//-----------------------------------------------------------------------------
