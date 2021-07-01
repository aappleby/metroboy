#pragma once
#include "CoreLib/Types.h"
#include <string>

//-----------------------------------------------------------------------------

size_t file_size(const char* filename);
size_t load_blob(const char* filename, void* dst, size_t dst_size);
void save_blob(const char* filename, const void* src, size_t size);

template<typename T>
inline void load_array(const std::string& filename, T& data) {
  size_t size = file_size(filename.c_str());
  if (size == 0) return;
  data.resize(size / sizeof(typename T::value_type));
  load_blob(filename.c_str(), data.data(), size);
}

template<typename T>
inline void save_array(const std::string& filename, T& data) {
  save_blob(filename.c_str(), (void*)data.data(), data.size() * sizeof(typename T::value_type));
}

template<typename T>
inline void load_obj(const char* filename, T& data) {
  load_blob(filename, &data, sizeof(T));
}

template<typename T>
inline void load_obj(const std::string& filename, T& data) {
  load_blob(filename.c_str(), &data, sizeof(T));
}

template<typename T>
inline void save_obj(const std::string& filename, T& data) {
  save_blob(filename.c_str(), &data, sizeof(T));
}

inline blob load_blob(const std::string& filename) {
  blob ret;
  load_array(filename, ret);
  return ret;
}

inline void save_blob(const char* filename, const blob& blob) {
  save_blob(filename, blob.data(), blob.size());
}