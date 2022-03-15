#include "metron_tools.h"

#include <stdarg.h>

#include <map>
#include <string>
#include <vector>

#include "../CoreLib/Log.h"

#pragma warning(disable : 4996)


//----------------------------------------

static std::vector<std::string> plusargs;

void metron_init(int argc, char** argv) {
  for (int argi = 0; argi < argc; argi++) {
    if (argv[argi][0] == '+') {
      plusargs.push_back(argv[argi] + 1);
    }
  }
}

void metron_reset() {
  plusargs.clear();
}

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

//----------------------------------------

void parse_hex(const char* src_filename, void* dst_data, int dst_size) {
  FILE* f = fopen(src_filename, "rb");
  fseek(f, 0, SEEK_END);
  int src_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  uint8_t* src_data = (uint8_t*)malloc(src_size);
  size_t read = fread(src_data, 1, src_size, f);

  uint8_t* dst_bytes = (uint8_t*)dst_data;

  int dst_cursor = 0;
  int hi_lo = 1;
  for (int src_cursor = 0; src_cursor < src_size; src_cursor++) {
    auto c = src_data[src_cursor];
    int d = -1;
    if (c >= '0' && c <= '9') d = c - '0';
    if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
    if (c >= 'A' && c <= 'F') d = c - 'A' + 10;

    if (d >= 0) {
      if (hi_lo == 1) {
        dst_bytes[dst_cursor] |= d << 4;
        hi_lo = 0;
      } else {
        dst_bytes[dst_cursor] |= d << 0;
        dst_cursor++;
        hi_lo = 1;
      }
    }

    if (dst_cursor == dst_size) break;
  }

  fclose(f);
  free(src_data);
}

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

