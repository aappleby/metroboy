// RISC-V SiMPLE SV -- combined text/data memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_EXAMPLE_MEMORY_BUS_H
#define RVSIMPLE_EXAMPLE_MEMORY_BUS_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"
#include "example_data_memory.h"
#include "example_text_memory.h"

class example_memory_bus {
public:
  logic<32> read_data;

  example_data_memory data_memory;
  example_text_memory text_memory;

  void tick(logic<32> address, logic<1> write_enable, logic<4> byte_enable, logic<32> write_data) {
    data_memory.tick(
      bx<DATA_BITS-2>(address),
      write_enable && address >= DATA_BEGIN && address <= DATA_END,
      byte_enable,
      write_data);
  }

  void tock(logic<32> address, logic<1>  read_enable) {
    data_memory.tock(bx<DATA_BITS-2>(address));
    text_memory.tock(bx<TEXT_BITS-2>(address));

    if (read_enable) {
      if (address >= TEXT_BEGIN && address <= TEXT_END) read_data = text_memory.q;
      else if (address >= DATA_BEGIN && address <= DATA_END) read_data = data_memory.q;
      else read_data = b32(DONTCARE);
    }
    else {
      read_data = b32(DONTCARE);
    }
  }
};

#endif // RVSIMPLE_EXAMPLE_MEMORY_BUS_H