#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <vector>

#pragma warning(disable:4201) // nameless struct/union

typedef int16_t sample_t;
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

//-----------------------------------------------------------------------------

struct Req {
  uint16_t addr = 0;
  uint8_t  data = 0;
  uint8_t  read = 0;
  uint8_t  write = 0;

  operator bool() const {
    return read || write;
  }
};

struct Ack {
  uint16_t addr = 0;
  uint16_t data = 0;
  uint8_t  read = 0;
};

//-----------------------------------------------------------------------------

template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}

//-----------------------------------------------------------------------------

inline void print_req(std::string& d, const Req& req) {
  if (req.write)  {
    sprintf(d, "%04x:%04x %s%s\n", req.addr, req.data,
      req.read  ? "\003R \001" : "- ",
      req.write ? "\002W \001" : "- ");
  }
  else {
    sprintf(d, "%04x:---- %s%s\n", req.addr,
      req.read  ? "\003R \001" : "- ",
      req.write ? "\002W \001" : "- ");
  }
}

inline void print_ack(std::string& d, const Ack& ack) {
  sprintf(d, "%04x:%04x %s%s\n", ack.addr, ack.data,
    ack.read  ? "\003R \001" : "- ",
    "- ");
}

//-----------------------------------------------------------------------------

struct Sprite {
  uint8_t y;
  uint8_t x;
  uint8_t p;
  union {
    struct {
      uint8_t pad0 : 1;
      uint8_t pad1 : 1;
      uint8_t pad2 : 1;
      uint8_t pad3 : 1;
      uint8_t pal : 1;
      uint8_t flipX : 1;
      uint8_t flipY : 1;
      uint8_t pri : 1;
    } bits;
    uint8_t f;
  } flags;
};

//-----------------------------------------------------------------------------

