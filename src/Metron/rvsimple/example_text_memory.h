// RISC-V SiMPLE SV -- text memory model
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław


#ifndef RVSIMPLE_EXAMPLE_TEXT_MEMORY_H
#define RVSIMPLE_EXAMPLE_TEXT_MEMORY_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

using namespace rv_config;

class example_text_memory {
public:

  logic<32> mem[pow2(rv_config::TEXT_BITS - 2)];
  logic<32> o_q;

  void init() {
    readmemh(rv_config::TEXT_HEX, mem, 0, pow2(rv_config::TEXT_BITS) - 1);
  }

  void tock(logic<rv_config::TEXT_BITS - 2> address) {
    o_q = mem[address];
  }
};

#endif // RVSIMPLE_EXAMPLE_TEXT_MEMORY_H