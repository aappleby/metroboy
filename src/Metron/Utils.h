#pragma once
#include <vector>
#include <span>
#include <assert.h>
#include <stdio.h>

#pragma warning(disable:4996) // unsafe fopen()

using namespace std;

typedef std::vector<uint8_t> blob;
typedef std::span<const char> cspan;

//------------------------------------------------------------------------------

inline blob load_blob(const char* filename) {
  FILE* f = fopen(filename, "rb");
  assert(f);

  blob result;
  fseek(f, 0, SEEK_END);
  result.resize(ftell(f));
  fseek(f, 0, SEEK_SET);

  fread(result.data(), 1, result.size(), f);
  fclose(f);
  return result;
}

//------------------------------------------------------------------------------

inline cspan make_span(TSNode node, const char* src) {
  const char* a = &src[ts_node_start_byte(node)];
  const char* b = &src[ts_node_end_byte(node)];

  while ((a < b) && (isspace(a[0]) || a[0] == '\r'))  a++;
  while ((a < b) && (isspace(b[-1]) || b[-1] == '\r')) b--;
  return cspan(a, b);
}

inline std::string body(TSNode node, const char* src) {
  auto s = make_span(node, src);
  return std::string(s.begin(), s.end());
}

//------------------------------------------------------------------------------

inline void print_escaped(char s) {
  if (s == '\n') printf("\\n");
  else if (s == '\r') printf("\\r");
  else if (s == '\t') printf("\\t");
  else if (s == '"')  printf("\\\"");
  else if (s == '\\') printf("\\\\");
  else                printf("%c", s);
}

inline void print_escaped(const char* source, uint32_t a, uint32_t b) {
  printf("\"");
  for (; a < b; a++) {
    print_escaped(source[a]);
  }
  printf("\"");
}

inline void indent(int depth) {
  for (int i = 0; i < depth; i++) printf("|   ");
}

//------------------------------------------------------------------------------
