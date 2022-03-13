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
	logic<32> q;

  void init() {
    readmemh(DATA_HEX, mem, 0, pow2(DATA_BITS) - 1);
  }

  void tick(logic<DATA_BITS - 2> address, logic<1> wren, logic<4> byteena, logic<32> data) {
    if (wren) {
      logic<8> byte0 = b8(mem[address], 0);
      logic<8> byte1 = b8(mem[address], 8);
      logic<8> byte2 = b8(mem[address], 16);
      logic<8> byte3 = b8(mem[address], 24);

      if (byteena[0]) byte0 = b8(data, 0);
      if (byteena[1]) byte1 = b8(data, 8);
      if (byteena[2]) byte2 = b8(data, 16);
      if (byteena[3]) byte3 = b8(data, 24);

      mem[address] = cat(byte3, byte2, byte1, byte0);
    }
  }

  void tock(logic<DATA_BITS - 2> i_address) {
    q = mem[i_address];
  }
};

#endif // RVSIMPLE_EXAMPLE_DATA_MEMORY_H