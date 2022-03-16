// RISC-V SiMPLE SV -- data memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_EXAMPLE_DATA_MEMORY_BUS_H
#define RVSIMPLE_EXAMPLE_DATA_MEMORY_BUS_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"
#include "example_data_memory.h"

class example_data_memory_bus {
public:

  example_data_memory data_memory;
  logic<32> read_data;

  void init() {
    data_memory.init();
  }

  void tick(logic<32> address, logic<1> write_enable, logic<4> byte_enable, logic<32> write_data) {
    data_memory.tick(
      bx<DATA_BITS-2>(address, 2),
      b1(write_enable && address >= DATA_BEGIN && address <= DATA_END),
      byte_enable,
      write_data);
  }

  void tock(logic<32> address, logic<1> read_enable) {
    data_memory.tock(bx<DATA_BITS-2>(address, 2));

    if (read_enable && address >= DATA_BEGIN && address <= DATA_END) {
      read_data = data_memory.q;
    }
    else {
      read_data = b32(DONTCARE);
    }
  }
};

#endif // RVSIMPLE_EXAMPLE_DATA_MEMORY_BUS_H