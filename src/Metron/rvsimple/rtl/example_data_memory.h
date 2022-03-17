// RISC-V SiMPLE SV -- data memory model
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_EXAMPLE_DATA_MEMORY_H
#define RVSIMPLE_EXAMPLE_DATA_MEMORY_H

#include "config.h"
#include "constants.h"
#include "metron_tools.h"

using namespace rv_config;

class example_data_memory {
 public:
  logic<32> q;

  void init() {
    std::string s;
    value_plusargs("data_file=%s", s);
    readmemh(s, mem);
  }

  void tick(logic<DATA_BITS - 2> address, logic<1> wren, logic<4> byteena,
            logic<32> data) {
    if (wren) {
      uint32_t mask = 0;
      if (byteena[0]) mask |= 0x000000FF;
      if (byteena[1]) mask |= 0x0000FF00;
      if (byteena[2]) mask |= 0x00FF0000;
      if (byteena[3]) mask |= 0xFF000000;
      mem[address] = (mem[address] & ~mask) | (data & mask);
    }
  }

  void tock(logic<DATA_BITS - 2> address) { q = mem[address]; }

 private:
  logic<32> mem[pow2(DATA_BITS - 2)];
};

#endif  // RVSIMPLE_EXAMPLE_DATA_MEMORY_H