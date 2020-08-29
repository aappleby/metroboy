#pragma once

#include <string>
#include <vector> // for blob

typedef std::vector<uint8_t> blob;

//-----------------------------------------------------------------------------

size_t file_size(const char* filename);
size_t load_blob(const char* filename, void* dst, size_t dst_size);
void save_blob(const char* filename, void* src, size_t size);

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

