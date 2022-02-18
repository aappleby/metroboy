// RISC-V SiMPLE SV -- text memory model
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../../../src/uart_test/metron_tools.h"
#include "../../synth/config.h"
#include "../common/constants.h"

struct example_text_memory {

  logic<32> mem[pow2(TEXT_BITS - 2)];
  logic<32> o_q;

  void init() {
    readmemh(TEXT_HEX, mem, 0, int(pow2(TEXT_BITS) - 1));
  }

  void tock(logic<TEXT_BITS - 2> address) {
    o_q = mem[address];
  }
};

