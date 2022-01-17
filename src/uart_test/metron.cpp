#include "metron.h"
#include <stdio.h>

void readmemh(const char* path, logic<8>* mem, int begin, int end) {
  FILE* f = fopen(path, "rb");
  fread(&mem[begin], 1, (end - begin) + 1, f);
  fclose(f);
}