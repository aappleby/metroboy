#pragma once
#include "CoreLib/Types.h"
#include <string>

//-----------------------------------------------------------------------------

size_t file_size(const char* filename);
bool load_blob(const char* filename, void* dst, size_t size);
bool save_blob(const char* filename, const void* src, size_t size);

inline bool load_blob(const char* filename, blob& data) {
  size_t size = file_size(filename);
  if (size == 0) return false;
  data.resize(size);
  return load_blob(filename, data.data(), size);
}

inline bool save_blob(const char* filename, const blob& blob) {
  return save_blob(filename, blob.data(), blob.size());
}
template<typename T>
inline bool load_obj(const char* filename, T& data) {
  return load_blob(filename, &data, sizeof(T));
}

template<typename T>
inline bool save_obj(const std::string& filename, T& data) {
  return save_blob(filename.c_str(), &data, sizeof(T));
}



