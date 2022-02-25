// RISC-V SiMPLE SV -- program text memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../metron_tools.h"
#include "config.h"
#include "constants.h"
#include "example_text_memory.h"

struct example_text_memory_bus {
  logic<32> o_read_data;

  example_text_memory text_memory;

  void tock(logic<32> i_address) {
    text_memory.tock(bx<TEXT_BITS-2>(i_address, 2));

    o_read_data = b32(DONTCARE);
    if (i_address >= TEXT_BEGIN && i_address <= TEXT_END) o_read_data = text_memory.o_q;
  }
};
