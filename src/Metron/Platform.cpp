#include "Platform.h"

//------------------------------------------------------------------------------

#include "tree_sitter/api.h"
#include "TreeSymbols.h"

#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include <algorithm>
//#include <compare> // not in gcc?
#include <functional>
#include <regex>
#include <set>
#include <map>
#include <string>
#include <vector>

//------------------------------------------------------------------------------
// GCC platform

#ifdef __GNUC__
#include <csignal>
#include <unistd.h>

typedef int64_t LARGE_INTEGER;

//int mkdir(const char* path, int mode) {
//  mkdir(path, mode);
//}

void debugbreak() {
  raise(SIGTRAP);
}

void dprintf(const char* format = "", ...) {
  static char buffer[256];
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

/*
  {
    std::string cwd;
    cwd.resize(FILENAME_MAX);
    getcwd(cwd.data(),FILENAME_MAX);
    LOG_R("cwd %s\n", cwd);
  }
*/ 

#endif

//------------------------------------------------------------------------------
// Visual Studio platform

#ifdef _MSC_VER
#include <Windows.h>
#include <direct.h>

#pragma warning(disable:4996) // unsafe fopen

void debugbreak() {
  __debugbreak();
}

int mkdir(const char* path, int mode) {
  return _mkdir(path);
}

void dprintf(const char* format, ...) {
  static char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, 256, format, args);
  va_end(args);
  OutputDebugString(buffer);
}

/*
  {
    std::string cwd;
    cwd.resize(FILENAME_MAX);
    getcwd(cwd.data(),FILENAME_MAX);
    LOG_R("cwd %s\n", cwd);
  }
*/ 

#endif

//------------------------------------------------------------------------------

bool operator<(const TSNode& a, const TSNode& b) {
  if (a.context[0] < b.context[0]) return true;
  if (a.context[0] > b.context[0]) return false;
  if (a.context[1] < b.context[1]) return true;
  if (a.context[1] > b.context[1]) return false;
  if (a.context[2] < b.context[2]) return true;
  if (a.context[2] > b.context[2]) return false;
  if (a.context[3] < b.context[3]) return true;
  if (a.context[3] > b.context[3]) return false;
  if (a.id < b.id) return true;
  if (a.id > b.id) return false;
  if (a.tree < b.tree) return true;
  if (a.tree > b.tree) return false;
  return false;
}

bool operator==(const TSNode& a, const TSNode& b) {
  if (a.context[0] != b.context[0]) return false;
  if (a.context[1] != b.context[1]) return false;
  if (a.context[2] != b.context[2]) return false;
  if (a.context[3] != b.context[3]) return false;
  if (a.id != b.id) return false;
  if (a.tree != b.tree) return false;
  return true;
}

bool operator!=(const TSNode& a, const TSNode& b) { return !(a == b); }

bool operator<(const TSTreeCursor& a, const TSTreeCursor& b) {
  if (a.context[0] < b.context[0]) return true;
  if (a.context[0] > b.context[0]) return false;
  if (a.context[1] < b.context[1]) return true;
  if (a.context[1] > b.context[1]) return false;
  if (a.tree < b.tree) return true;
  if (a.tree > b.tree) return false;
  if (a.id < b.id) return true;
  if (a.id > b.id) return false;
  return false;
}

bool operator == (const TSTreeCursor& a, const TSTreeCursor& b) {
  if (a.context[0] != b.context[0]) return false;
  if (a.context[1] != b.context[1]) return false;
  if (a.tree != b.tree) return false;
  if (a.id != b.id) return false;
  return true;
}

bool operator != (const TSTreeCursor& a, const TSTreeCursor& b) { return !(a == b); }

//------------------------------------------------------------------------------

struct MtCursor;
struct MtModule;
struct MtModLibrary;
struct MtNode;
struct MtIterator;

typedef std::vector<uint8_t> blob;

//------------------------------------------------------------------------------
