#pragma once
#include "metron.h"

//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

struct blockram_512x8 {
  static const int size = 512;

  int     i_addr;
  uint8_t o_data;

  uint8_t memory[size];
  int     addr;

  void initial(const uint8_t* src, int size) {
    assert(size == 512);
    memcpy(memory, src, size);
    addr = 0;
  }

  void tick() {
    o_data = memory[addr];
  }

  void tock() {
    addr = i_addr;
  }
};
