// RISC-V SiMPLE SV -- generic register
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_REGISTER_H
#define RVSIMPLE_REGISTER_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

template<int WIDTH = 32, int INITIAL = 0>
class _register {
public:

  logic<WIDTH> value;
  logic<WIDTH> o_value;

  void init() {
    value = INITIAL;
  }

  void tick(logic<1> i_reset, logic<1> i_write_enable, logic<WIDTH> i_next) {
    if (i_reset) value = INITIAL;
    else if (i_write_enable) value = i_next;
  }

  void tock() {
    o_value = value;
  }
};

#endif // RVSIMPLE_REGISTER_H