#pragma once
#include <map>
#include <string>
#include <assert.h>

#pragma warning(disable:4996)

class VcdDump {
public:

  typedef std::map<std::string, std::string> tag_to_string;
  typedef std::map<std::string, uint64_t> tag_to_value;

  VcdDump(const char* filename) {
    file = fopen(filename, "wb");
    time = 0;
  }

  ~VcdDump() {
    fclose(file);
    file = nullptr;
  }

  void set_time(uint64_t _time) {
    time = _time;
    fprintf(file, "#%I64d\n", time);
  };

  void set_value(const char* key, uint64_t value, int width) {
    auto it = field_state.find(key);
    if ((it == field_state.end()) || (value != (*it).second)) {
      if (width == 1) {
        assert((value & ~1) == 0);
        fprintf(file, "%I64d%s\n", value & 1, key);
      }
      else {
        fprintf(file, "b");
        for (int i = (int)width - 1; i >= 0; i--) {
          fprintf(file, "%d", int(value >> i) & 1);
        }
        fprintf(file, " %s\n", key);
      }
    }
    field_state[key] = value;
  }

  FILE* file;
  uint64_t time;
  tag_to_string field_path;
  tag_to_value field_state;
};