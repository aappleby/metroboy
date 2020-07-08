#pragma once

#include <string>
#include <vector> // for blob

typedef std::vector<uint8_t> blob;

//-----------------------------------------------------------------------------

inline size_t file_size(const char* filename) {
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

inline void load_blob(const char* filename, void* dst, size_t size) {
  FILE* f = nullptr;
  fopen_s(&f, filename, "rb");
  fread(dst, 1, size, f);
  fclose(f);
}

inline void save_blob(const char* filename, void* src, size_t size) {
  FILE* f = nullptr;
  fopen_s(&f, filename, "wb");
  fwrite(src, 1, size, f);
  fclose(f);
}

template<typename T>
inline void load_array(const std::string& filename, T& data) {
  size_t size = file_size(filename.c_str());
  if (size == 0) return;
  data.resize(size / sizeof(T::value_type));
  load_blob(filename.c_str(), data.data(), size);
}

template<typename T>
inline void save_array(const std::string& filename, T& data) {
  save_blob(filename.c_str(), (void*)data.data(), data.size() * sizeof(T::value_type));
}

template<typename T>
inline void load_obj(const std::string& filename, T& data) {
  load_blob(filename.c_str(), &data, sizeof(T));
}

template<typename T>
inline void save_obj(const std::string& filename, T& data) {
  save_blob(filename.c_str(), &data, sizeof(T));
}

