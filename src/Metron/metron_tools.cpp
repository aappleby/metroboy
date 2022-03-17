#include "metron_tools.h"

#include <stdarg.h>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "../CoreLib/Log.h"

#pragma warning(disable : 4996)

//----------------------------------------

static std::vector<std::string> plusargs;

void metron_init(int argc, const char** argv) {
  for (int argi = 0; argi < argc; argi++) {
    if (argv[argi][0] == '+') {
      plusargs.push_back(argv[argi] + 1);
    }
  }
}

void metron_reset() { plusargs.clear(); }

void value_plusargs(const char* fmt, std::string& out) {
  int prefix_len = 0;
  const char* cursor = fmt;
  while (*cursor != '=' && *cursor != 0) {
    prefix_len++;
    cursor++;
  }

  std::string prefix(fmt, fmt + prefix_len);

  for (const auto& arg : plusargs) {
    if (arg.starts_with(prefix)) {
      out = arg.c_str() + prefix_len + 1;
    }
  }
}

//----------------------------------------

static_assert(clog2(0) == 0);
static_assert(clog2(1) == 1);
static_assert(clog2(2) == 1);
static_assert(clog2(3) == 2);
static_assert(clog2(255) == 8);
static_assert(clog2(256) == 8);
static_assert(clog2(257) == 9);

//------------------------------------------------------------------------------

int to_hex(uint8_t c) {
  if      (c >= '0' && c <= '9') return c - '0';
  else if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  else if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  else return -1;
}

//------------------------------------------------------------------------------

void parse_hex(const char* src_filename, void* dst_data, int dst_size) {
  FILE* f = fopen(src_filename, "rb");
  fseek(f, 0, SEEK_END);
  int src_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  // Stick a chunk of nulls on the end so we don't have to worry about reading a
  // byte past the buffer.
  void* src_data = malloc(src_size + 256);
  memset(src_data, 0, src_size + 256);
  size_t read = fread(src_data, 1, src_size, f);
  fclose(f);

  uint8_t* sc = (uint8_t*)src_data;
  uint8_t* sc_end = sc + src_size;

  uint8_t* dc = (uint8_t*)dst_data;
  uint8_t* dc_end = dc + (dst_size == -1 ? 0xFFFFFFFF : dst_size);

  while(sc[0] && sc < sc_end) {

    // Skip single-line comments
    if (sc[0] == '/' && sc[1] == '/') {
      while(sc[0] && sc[0] != '\n') sc++;
      sc++;
      continue;
    }

    // Skip multi-line comments
    if (sc[0] == '/' && sc[1] == '*') {
      while(sc[0] && (sc[0] != '*' || sc[1] != '/')) sc++;
      sc += 2;
      continue;
    }

    // Skip whitespace
    if (isspace(sc[0])) {
      sc++;
      continue;
    }

    // Current char isn't a comment or whitespace, so check for address marker.
    bool is_addr = false;
    if (sc[0] == '@') {
      is_addr = true;
      sc++;
    }

    // We should be at a big-endian hex value now, decode it.
    int chunk_data= 0;
    int chunk_size = 0;
    while(sc[0]) {
      int d = to_hex(sc[0]);
      if (d == -1) break;
      chunk_data <<= 4;
      chunk_data |= d;
      chunk_size++;
      sc++;
    }

    if (!chunk_size || (chunk_size & 1)) {
      printf("Error loading %s: Invalid vmem character 0x%02x (%c)\n", src_filename, sc[0], sc[0]);
      return;
    }

    // Store hex value in address or in output stream, little-endian.
    if (is_addr) {
      dc = (uint8_t*)dst_data + (chunk_data * 4);
    }
    else {
      for (;chunk_size; chunk_size -= 2) {
        if (dc < dc_end) *dc++ = chunk_data & 0xFF;
        chunk_data >>= 8;
      }
    }
  }

  free(src_data);
}

//------------------------------------------------------------------------------

/*
void print_hex(const char* buf_name, void* src_data, int src_size) {
  char buf_name[512];
  strcpy(buf_name, src_filename);
  for (char* cursor = buf_name; *cursor; cursor++) {
    if (*cursor == '.') *cursor = '_';
  }

  printf("uint8_t %s[%d] = {\n", buf_name, src_size);
  int dst_cursor = 0;
  for (int y = 0; y < dst_size / 16; y++) {
    printf("  ");
    for (int x = 0; x < 16; x++) {
      printf("%02x, ", dst_data[dst_cursor++]);
    }
    printf("\n");
  }
  printf("};\n");
}
*/

//----------------------------------------
// 'end' is INCLUSIVE

void readmemh(const char* path, void* mem, int begin, int end) {
  memset(mem, 0, end - begin + 1);
  parse_hex(path, (uint8_t*)mem + begin, end - begin + 1);
}

void readmemh(const std::string& path, void* mem, int begin, int end) {
  memset(mem, 0, end - begin + 1);
  parse_hex(path.c_str(), (uint8_t*)mem + begin, end - begin + 1);
}

void readmemh(const char* path, void* mem) {
  parse_hex(path, (uint8_t*)mem, 0xFFFFFFFF);
}

void readmemh(const std::string& path, void* mem) {
  parse_hex(path.c_str(), (uint8_t*)mem, 0xFFFFFFFF);
}

//----------------------------------------

int display(const char* fmt, ...) {
  char buffer[256];
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(buffer, 256, fmt, args);
  va_end(args);
  log_print(0xFFFFFF, buffer, len);
  log_printf(0xFFFFFF, "\n");
  return len;
}

int write(const char* fmt, ...) {
  char buffer[256];
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(buffer, 256, fmt, args);
  va_end(args);
  log_print(0xFFFFFF, buffer, len);
  return len;
}
