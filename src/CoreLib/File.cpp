#include "CoreLib/File.h"
#include "CoreLib/Log.h"

#pragma warning(disable:4996)

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

bool load_blob(const char* filename, void* dst, size_t dst_size) {
  FILE* f = fopen(filename, "rb");
  if (f == nullptr) {
    LOG_R("Failed to open %s\n", filename);
    return false;
  }

  fseek(f, 0, SEEK_END);
  CHECK_P(ftell(f) == dst_size);
  fseek(f, 0, SEEK_SET);

  CHECK_P(fread(dst, 1, dst_size, f) == dst_size);
  fclose(f);
  return true;
}

bool save_blob(const char* filename, const void* src, size_t src_size) {
  FILE* f = fopen(filename, "wb");
  if (f == nullptr) {
    LOG_R("Failed to open %s\n", filename);
    return false;
  }

  CHECK_P(fwrite(src, 1, src_size, f) == src_size);
  fclose(f);
  return true;
}

