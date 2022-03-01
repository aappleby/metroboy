// RISC-V SiMPLE SV -- data memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../metron_tools.h"
#include "config.h"
#include "constants.h"
#include "example_data_memory.h"

struct example_data_memory_bus {

  example_data_memory data_memory;
  logic<32> o_read_data;

  void tick(logic<32> i_address, logic<1> i_write_enable, logic<4> i_byte_enable, logic<32> i_write_data) {
    data_memory.tick(
      bx<DATA_BITS-2>(i_address, 2),
      b1(i_write_enable && i_address >= DATA_BEGIN && i_address <= DATA_END),
      i_byte_enable,
      i_write_data);
  }

  void tock(logic<32> i_address, logic<1> i_read_enable) {
    data_memory.tock(bx<DATA_BITS-2>(i_address, 2));

    if (i_read_enable && i_address >= DATA_BEGIN && i_address <= DATA_END) {
      o_read_data = data_memory.o_q;
    }
    else {
      o_read_data = b32(DONTCARE);
    }
  }
};
