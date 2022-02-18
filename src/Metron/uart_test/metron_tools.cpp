//#include "metron_tools.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

#pragma warning(disable:4996)

void parse_hex(const char* src_filename, void* dst_data, int dst_size) {

  FILE* f = fopen(src_filename, "rb");
  fseek(f, 0, SEEK_END);
  int src_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  uint8_t* src_data = (uint8_t*)malloc(src_size);
  fread(src_data, 1, src_size, f);

  memset(dst_data, 0, dst_size);
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
      }
      else {
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


void readmemh(const char* path, void* mem, int begin, int end) {
  parse_hex(path, (uint8_t*)mem + begin, end - begin + 1);
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

