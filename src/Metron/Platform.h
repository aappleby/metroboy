#pragma once

//------------------------------------------------------------------------------

#include "tree_sitter/api.h"
#include "TreeSymbols.h"

#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include <algorithm>
#include <compare>
#include <functional>
#include <regex>
#include <set>
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

inline std::strong_ordering operator<=>(const TSNode& a, const TSNode& b) {
  constexpr auto eq = std::strong_ordering::equal;
  if (auto x = (a.context[0] <=> b.context[0]); x != eq) return x;
  if (auto x = (a.context[1] <=> b.context[1]); x != eq) return x;
  if (auto x = (a.context[2] <=> b.context[2]); x != eq) return x;
  if (auto x = (a.context[3] <=> b.context[3]); x != eq) return x;
  if (auto x = (a.id <=> b.id);         x != eq) return x;
  if (auto x = (a.tree <=> b.tree);       x != eq) return x;
  return eq;
}

inline std::strong_ordering operator<=>(const TSTreeCursor& a, const TSTreeCursor& b) {
  constexpr auto eq = std::strong_ordering::equal;
  if (auto x = a.context[0] <=> b.context[0]; x != eq) return x;
  if (auto x = a.context[1] <=> b.context[1]; x != eq) return x;
  if (auto x = a.tree <=> b.tree;       x != eq) return x;
  if (auto x = a.id <=> b.id;         x != eq) return x;
  return eq;
}

//------------------------------------------------------------------------------

struct MtCursor;
struct MtModule;
struct MtModLibrary;
struct MtNode;
struct MtIterator;

typedef std::vector<uint8_t> blob;
