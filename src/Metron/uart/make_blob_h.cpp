#include <stdio.h>
#include <stdint.h>
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv) {
  const char* src_filename = argv[1];

  FILE* f = fopen(src_filename, "rb");

  /*
  int byte_count = 0;
  int lo = -1;
  int hi = -1;

  printf("uint8_t %s[] = {\n", argv[2]);
  for(auto c = getc(f); c != EOF; c = getc(f)) {
    int d = -1;
    if (c >= '0' && c <= '9') d = c - '0';
    if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
    if (c >= 'A' && c <= 'F') d = c - 'A' + 10;
    if (d == -1) continue;

    if (hi == -1) {
      hi = d;
    } else {
      lo = d;
      printf("%02x, ", (hi << 4) | lo);
      hi = lo = -1;
      byte_count++;
      if (byte_count % 16 == 0) printf("\n");
    }
  }
  printf("};\n");
  */

  char buf_name[512];
  strcpy(buf_name, src_filename);
  for (int i = 0; i < 512; i++) if (buf_name[i] == '.') buf_name[i] = 0;

  int byte_count = 0;
  printf("uint8_t %s[] = {\n", buf_name);
  for(auto c = getc(f); c != EOF; c = getc(f)) {
    if (byte_count % 16 == 0) printf("  ");
    printf("0x%02x, ", c);
    if (byte_count % 16 == 15) printf("\n");
    byte_count++;
  }
  printf("};\n");
  printf("const int %s_len = sizeof(%s);\n", buf_name, buf_name);

  fclose(f);
}
