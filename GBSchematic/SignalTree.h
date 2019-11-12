#pragma once
#include <vector>
#include <cstddef>

struct SignalData {
  SignalData(const char* name_) {
    name = name_;
    byte_offset = 0;
    bit_offset = 0;
    bit_width = 0;
  }

  SignalData(const char* name_, int byte_offset_) {
    name = name_;
    byte_offset = byte_offset_;
    bit_offset = 0;
    bit_width = 1;
  }

  SignalData(const char* name_, int byte_offset_, int bit_offset_, int bit_width_) {
    name = name_;
    byte_offset = byte_offset_;
    bit_offset = bit_offset_;
    bit_width = bit_width_;
  }

  SignalData(const char* name_, int byte_offset_, std::vector<SignalData> children_) {
    name = name_;
    byte_offset = byte_offset_;
    bit_offset = 0;
    bit_width = 0;
    children = children_;
  }

  const char* name;
  int byte_offset;
  int bit_offset;
  int bit_width;
  std::vector<SignalData> children;
};

