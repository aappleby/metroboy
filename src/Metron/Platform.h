#pragma once
#include <stdint.h>
#include <stdio.h>

//------------------------------------------------------------------------------

/*
#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include <algorithm>

#include "TreeSymbols.h"
//#include <compare> // not in gcc?
#include <functional>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>
*/

//------------------------------------------------------------------------------

void debugbreak();
int mkdir(const char* path, int mode);
void dprintf(const char* format = "", ...);
char* getcwd();

inline void print_escaped(char s) {
  if (s == '\n')
    printf("\\n");
  else if (s == '\r')
    printf("\\r");
  else if (s == '\t')
    printf("\\t");
  else if (s == '"')
    printf("\\\"");
  else if (s == '\\')
    printf("\\\\");
  else
    printf("%c", s);
}

inline void print_escaped(const char* source, uint32_t a, uint32_t b) {
  printf("\"");
  for (; a < b; a++) {
    print_escaped(source[a]);
  }
  printf("\"");
}

//------------------------------------------------------------------------------
