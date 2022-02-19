#pragma once

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

#ifdef __GNUC__
#include <csignal>
#define debugbreak() raise(SIGTRAP);
typedef int64_t LARGE_INTEGER;
#else
#define debugbreak() __debugbreak();
#endif

//------------------------------------------------------------------------------

inline bool operator<(const TSNode& a, const TSNode& b) {
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

inline bool operator==(const TSNode& a, const TSNode& b) {
  if (a.context[0] != b.context[0]) return false;
  if (a.context[1] != b.context[1]) return false;
  if (a.context[2] != b.context[2]) return false;
  if (a.context[3] != b.context[3]) return false;
  if (a.id != b.id) return false;
  if (a.tree != b.tree) return false;
  return true;
}

inline bool operator!=(const TSNode& a, const TSNode& b) { return !(a == b); }

inline bool operator<(const TSTreeCursor& a, const TSTreeCursor& b) {
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

inline bool operator == (const TSTreeCursor& a, const TSTreeCursor& b) {
  if (a.context[0] != b.context[0]) return false;
  if (a.context[1] != b.context[1]) return false;
  if (a.tree != b.tree) return false;
  if (a.id != b.id) return false;
  return true;
}

inline bool operator != (const TSTreeCursor& a, const TSTreeCursor& b) { return !(a == b); }

//------------------------------------------------------------------------------

struct MtCursor;
struct MtModule;
struct MtModLibrary;
struct MtNode;
struct MtIterator;

typedef std::vector<uint8_t> blob;
