// RISC-V SiMPLE SV -- control transfer unit
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_CONTROL_TRANSFER_H
#define RVSIMPLE_CONTROL_TRANSFER_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

class control_transfer {
public:

  logic<1> o_take_branch;
  
  void tock(logic<3> i_inst_funct3,
            logic<1> i_result_equal_zero) {
    using namespace rv_constants;

    switch (i_inst_funct3) {
      case FUNCT3_BRANCH_EQ:  o_take_branch = !i_result_equal_zero; break;
      case FUNCT3_BRANCH_NE:  o_take_branch = i_result_equal_zero; break;
      case FUNCT3_BRANCH_LT:  o_take_branch = !i_result_equal_zero; break;
      case FUNCT3_BRANCH_GE:  o_take_branch = i_result_equal_zero; break;
      case FUNCT3_BRANCH_LTU: o_take_branch = !i_result_equal_zero; break;
      case FUNCT3_BRANCH_GEU: o_take_branch = i_result_equal_zero; break;
      default:                o_take_branch = b1(DONTCARE); break;
    }
  }
};

#endif // RVSIMPLE_CONTROL_TRANSFER_H