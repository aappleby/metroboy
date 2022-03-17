// RISC-V SiMPLE SV -- program text memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_EXAMPLE_TEXT_MEMORY_BUS_H
#define RVSIMPLE_EXAMPLE_TEXT_MEMORY_BUS_H

#include "config.h"
#include "constants.h"
#include "example_text_memory.h"
#include "metron_tools.h"

class example_text_memory_bus {
 public:
  logic<32> read_data;

  example_text_memory text_memory;

  void init() { text_memory.init(); }

  void tock(logic<32> address) {
    text_memory.tock(bx<TEXT_BITS - 2>(address, 2));

    if (address >= TEXT_BEGIN && address <= TEXT_END) {
      read_data = text_memory.q;
    } else {
      read_data = b32(DONTCARE);
    }
  }
};

#endif  // RVSIMPLE_EXAMPLE_TEXT_MEMORY_BUS_H