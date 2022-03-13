// RISC-V SiMPLE SV -- data memory model
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_EXAMPLE_DATA_MEMORY_H
#define RVSIMPLE_EXAMPLE_DATA_MEMORY_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

using namespace rv_config;

class example_data_memory {
public:

  logic<32> mem[pow2(DATA_BITS - 2)];
	logic<32> o_q;

  void init() {
    readmemh(DATA_HEX, mem, 0, pow2(DATA_BITS) - 1);
  }

  void tick(logic<DATA_BITS - 2> i_address, logic<1> i_wren, logic<4> i_byteena, logic<32> i_data) {
    if (i_wren) {
      logic<8> byte0 = b8(mem[i_address], 0);
      logic<8> byte1 = b8(mem[i_address], 8);
      logic<8> byte2 = b8(mem[i_address], 16);
      logic<8> byte3 = b8(mem[i_address], 24);

      if (i_byteena[0]) byte0 = b8(i_data, 0);
      if (i_byteena[1]) byte1 = b8(i_data, 8);
      if (i_byteena[2]) byte2 = b8(i_data, 16);
      if (i_byteena[3]) byte3 = b8(i_data, 24);

      mem[i_address] = cat(byte3, byte2, byte1, byte0);
    }
  }

  void tock(logic<DATA_BITS - 2> i_address) {
    o_q = mem[i_address];
  }
};

#endif // RVSIMPLE_EXAMPLE_DATA_MEMORY_H