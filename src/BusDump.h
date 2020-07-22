#pragma once
#include "Types.h"

struct BusDump {
  int phase;
  uint16_t cpu_addr;
  uint8_t cpu_data;
  bool cpu_rd;
  bool cpu_wr;
  uint16_t bus_addr;
  uint8_t bus_data;
  bool bus_rd;
  bool bus_wr;
  bool bus_ack;
};


void dump_bus_dump(Dumper& d, BusDump* dump, int start, int count);