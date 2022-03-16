// RISC-V SiMPLE SV -- multiplexer module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_MULTIPLEXER2_H
#define RVSIMPLE_MULTIPLEXER2_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

template <int WIDTH = 32>
class multiplexer2 {
 public:
  logic<WIDTH> out;

  void tock(logic<1> sel, logic<WIDTH> in0, logic<WIDTH> in1) {
    switch (sel) {
      case 0:
        out = in0;
        break;
      case 1:
        out = in1;
        break;
      default:
        out = bx<WIDTH>(DONTCARE);
        break;
    }
  }
};

#endif // RVSIMPLE_MULTIPLEXER2_H
