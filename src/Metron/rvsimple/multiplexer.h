// RISC-V SiMPLE SV -- multiplexer module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../metron_tools.h"
#include "config.h"
#include "constants.h"

template<int WIDTH = 32, int CHANNELS = 2, int SEL_BITS = clog2(CHANNELS)>
struct multiplexer {
  logic<WIDTH> o_out;

  void tock(logic<WIDTH> in_bus[CHANNELS], logic<SEL_BITS> sel) {
    o_out = in_bus[sel];
  }
};
