// RISC-V SiMPLE SV -- adder module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../metron_tools.h"
#include "config.h"
#include "constants.h"

template<int WIDTH = 32>
struct adder {
  logic<WIDTH> o_result;

  void tock(logic<WIDTH> i_operand_a, logic<WIDTH> i_operand_b) {
    o_result = i_operand_a + i_operand_b;
  }
};
