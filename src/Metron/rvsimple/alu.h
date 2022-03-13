// RISC-V SiMPLE SV -- ALU module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_ALU_H
#define RVSIMPLE_ALU_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"


class alu {
public:
  logic<32> o_result;
  logic<1>  o_result_equal_zero;

  void tock(
    logic<5>  i_alu_function,
    logic<32> i_operand_a, // signed?
    logic<32> i_operand_b  // signed?
  ) {
    using namespace rv_constants;

    switch (i_alu_function) {
    case ALU_ADD:   o_result = i_operand_a +    i_operand_b; break;
    case ALU_SUB:   o_result = i_operand_a -    i_operand_b; break;
    case ALU_SLL:   o_result = i_operand_a <<   b5(i_operand_b); break;
    case ALU_SRL:   o_result = i_operand_a >>   b5(i_operand_b); break;
    case ALU_SRA:   o_result = signed(i_operand_a) >>  b5(i_operand_b); break;
    case ALU_SEQ:   o_result = cat(b31(0), b1(i_operand_a == i_operand_b)); break;
    case ALU_SLT:   o_result = cat(b31(0), b1(i_operand_a < i_operand_b)); break;
    case ALU_SLTU:  o_result = cat(b31(0), b1(i_operand_a < i_operand_b)); break;
    case ALU_XOR:   o_result = i_operand_a ^    i_operand_b; break;
    case ALU_OR:    o_result = i_operand_a |    i_operand_b; break;
    case ALU_AND:   o_result = i_operand_a &    i_operand_b; break;
    default:
      o_result = b32(ZERO);
      break;
    }

    o_result_equal_zero = (o_result == b32(0));
  }
};

#endif // RVSIMPLE_ALU_H