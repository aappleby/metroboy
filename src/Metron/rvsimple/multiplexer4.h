// RISC-V SiMPLE SV -- multiplexer module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_MULTIPLEXER4_H
#define RVSIMPLE_MULTIPLEXER4_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

template <int WIDTH = 32>
class multiplexer4 {
 public:
  logic<WIDTH> out;

  void tock(logic<2> sel, logic<WIDTH> in0, logic<WIDTH> in1, logic<WIDTH> in2,
            logic<WIDTH> in3) {
    switch (sel) {
      case 0:
        out = in0;
        break;
      case 1:
        out = in1;
        break;
      case 2:
        out = in2;
        break;
      case 3:
        out = in3;
        break;
      default:
        out = bx<WIDTH>(DONTCARE);
        break;
    }
  }
};

#endif // RVSIMPLE_MULTIPLEXER4_H
