#include "CoreLib/File.h"

size_t file_size(const char* filename) {
  FILE* f = nullptr;
  fopen_s(&f, filename, "rb");

  if (f == nullptr) {
    printf("Failed to open %s\n", filename);
    return 0;
  }

  fseek(f, 0, SEEK_END);
  size_t result = ftell(f);
  fclose(f);
  return result;
}

size_t load_blob(const char* filename, void* dst) {
  FILE* f = nullptr;
  fopen_s(&f, filename, "rb");

  if (f == nullptr) {
    printf("Failed to open %s\n", filename);
    return 0;
  }

  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  fseek(f, 0, SEEK_SET);
  fread(dst, 1, size, f);
  fclose(f);
  return size;
}

void load_blob(const char* filename, void* dst, size_t size) {
  FILE* f = nullptr;
  fopen_s(&f, filename, "rb");
  fread(dst, 1, size, f);
  fclose(f);
}

void save_blob(const char* filename, void* src, size_t size) {
  FILE* f = nullptr;
  fopen_s(&f, filename, "wb");
  fwrite(src, 1, size, f);
  fclose(f);
}

