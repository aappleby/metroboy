// RISC-V SiMPLE SV -- data memory model
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../../../src/uart_test/metron_tools.h"
#include "../../synth/config.h"
#include "../common/constants.h"

struct example_data_memory {

  logic<32> mem[pow2(DATA_BITS - 2)];
	logic<32> o_q;

  void init() {
    readmemh(DATA_HEX, mem, 0, int(pow2(DATA_BITS) - 1));
  }

  void tick(logic<DATA_BITS - 2> i_address, logic<1> i_wren, logic<4> i_byteena, logic<32> i_data) {
    if (i_wren) {
      if (i_byteena[0]) s8(mem[i_address], 0)  = b8(i_data, 0);
      if (i_byteena[1]) s8(mem[i_address], 8)  = b8(i_data, 8);
      if (i_byteena[2]) s8(mem[i_address], 16) = b8(i_data, 16);
      if (i_byteena[3]) s8(mem[i_address], 24) = b8(i_data, 24);
    }
  }

  void tock(logic<DATA_BITS - 2> i_address) {
    o_q = mem[i_address];
  }
};
