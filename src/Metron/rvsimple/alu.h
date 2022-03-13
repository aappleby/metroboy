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
  logic<32> result;
  logic<1>  result_equal_zero;

  void tock(
    logic<5>  alu_function,
    logic<32> operand_a, // signed?
    logic<32> operand_b  // signed?
  ) {
    using namespace rv_constants;

    switch (alu_function) {
    case ALU_ADD:   result = operand_a +    operand_b; break;
    case ALU_SUB:   result = operand_a -    operand_b; break;
    case ALU_SLL:   result = operand_a <<   b5(operand_b); break;
    case ALU_SRL:   result = operand_a >>   b5(operand_b); break;
    case ALU_SRA:   result = signed(operand_a) >>  b5(operand_b); break;
    case ALU_SEQ:   result = cat(b31(0), b1(operand_a == operand_b)); break;
    case ALU_SLT:   result = cat(b31(0), b1(operand_a < operand_b)); break;
    case ALU_SLTU:  result = cat(b31(0), b1(operand_a < operand_b)); break;
    case ALU_XOR:   result = operand_a ^    operand_b; break;
    case ALU_OR:    result = operand_a |    operand_b; break;
    case ALU_AND:   result = operand_a &    operand_b; break;
    default:
      result = b32(ZERO);
      break;
    }

    result_equal_zero = (result == b32(0));
  }
};

#endif // RVSIMPLE_ALU_H