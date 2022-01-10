#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <assert.h>
#include <inttypes.h>

#pragma warning(disable:4996)

class VcdDump {
public:

  typedef std::unordered_map<std::string, std::string> tag_to_string;
  typedef std::unordered_map<std::string, uint64_t> tag_to_value;

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
    fprintf(file, "#%" PRIu64 "\n", time);
  };

  void advance(uint64_t delta) {
    time += delta;
    fprintf(file, "#%" PRIu64 "\n", time);
  };

  void set_value(const char* key, uint64_t value, int width) {
    auto it = field_state.find(key);
    if ((it == field_state.end()) || (value != (*it).second)) {
      if (width == 1) {
        assert((value & ~1) == 0);
        fprintf(file, "%" PRIu64 "%s\n", value & 1, key);
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