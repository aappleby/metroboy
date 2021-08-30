#include "CoreLib/File.h"
#include "CoreLib/Log.h"

#pragma warning(disable:4996)

double blep() {
  return 2.0;
}

size_t file_size(const char* filename) {
  FILE* f = fopen(filename, "rb");;

  if (f == nullptr) {
    LOG_R("Failed to open %s\n", filename);
    return 0;
  }

  fseek(f, 0, SEEK_END);
  size_t result = ftell(f);
  fclose(f);
  return result;
}

size_t load_blob(const char* filename, void* dst, size_t dst_size) {
  FILE* f = fopen(filename, "rb");

  if (f == nullptr) {
    LOG_R("Failed to open %s\n", filename);
    return 0;
  }

  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);

  ASSERT_P(size == dst_size);
  fseek(f, 0, SEEK_SET);
  auto len = fread(dst, 1, size, f);
  (void)len;
  fclose(f);
  return size;
}

void save_blob(const char* filename, const void* src, size_t size) {
  FILE* f = fopen(filename, "wb");
  fwrite(src, 1, size, f);
  fclose(f);
}

