// RISC-V SiMPLE SV -- control transfer unit
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_CONTROL_TRANSFER_H
#define RVSIMPLE_CONTROL_TRANSFER_H

#include "config.h"
#include "constants.h"
#include "metron_tools.h"

class control_transfer {
 public:
  logic<1> take_branch;

  void tock(logic<3> inst_funct3, logic<1> result_equal_zero) {
    using namespace rv_constants;

    switch (inst_funct3) {
      case FUNCT3_BRANCH_EQ:
        take_branch = !result_equal_zero;
        break;
      case FUNCT3_BRANCH_NE:
        take_branch = result_equal_zero;
        break;
      case FUNCT3_BRANCH_LT:
        take_branch = !result_equal_zero;
        break;
      case FUNCT3_BRANCH_GE:
        take_branch = result_equal_zero;
        break;
      case FUNCT3_BRANCH_LTU:
        take_branch = !result_equal_zero;
        break;
      case FUNCT3_BRANCH_GEU:
        take_branch = result_equal_zero;
        break;
      default:
        take_branch = b1(DONTCARE);
        break;
    }
  }
};

#endif  // RVSIMPLE_CONTROL_TRANSFER_H