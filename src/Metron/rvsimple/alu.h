// RISC-V SiMPLE SV -- ALU module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../metron_tools.h"
#include "config.h"
#include "constants.h"

struct alu {
  logic<32> o_result;
  logic<1>  o_result_equal_zero;

#ifdef M_MODULE
  void tock1() {
  }
#endif

    
  void tock(
    logic<5>  i_alu_function,
    logic<32> i_operand_a, // signed?
    logic<32> i_operand_b  // signed?
  ) {
#ifdef M_MODULE
    logic<64> signed_multiplication;
    logic<64> unsigned_multiplication;
    logic<64> signed_unsigned_multiplication;

    signed_multiplication          = signed(i_operand_a)   * signed(i_operand_b);
    unsigned_multiplication        = unsigned(i_operand_a) * unsigned(i_operand_b);
    signed_unsigned_multiplication = signed(i_operand_a)   * unsigned(i_operand_b);
#endif

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
#ifdef M_MODULE
    case ALU_MUL:    o_result = b32(signed_multiplication);
    case ALU_MULH:   o_result = b32(signed_multiplication, 32);
    case ALU_MULHSU: o_result = b32(signed_unsigned_multiplication, 32);
    case ALU_MULHU:  o_result = b32(unsigned_multiplication, 32);
    case ALU_DIV:
      if (operand_b == ZERO)
        result = b32(1);
      else if ((operand_a == 0x80000000) && (operand_b == b32(1)))
        result = 0x80000000;
      else
        result = b64(operand_a) / b64(operand_b); // FIXME should be signed?
    case ALU_DIVU:
      if (operand_b == ZERO)
        result = b32(1);
      else
        result = b64(operand_a) / b64(operand_b);
    case ALU_REM:
      if (operand_b == ZERO)
        result = operand_a;
      else if ((operand_a == b32(0x80000000)) && (operand_b == b32(1)))
        result = ZERO;
      else
        result = b64(operand_a) % b64(operand_b);
    case ALU_REMU:
      if (operand_b == ZERO)
        result = operand_a;
      else
        result = b64(operand_a) % b64(operand_b);
#endif
    default:
      o_result = b32(ZERO);
      break;
    }

    o_result_equal_zero = (o_result == b32(0));
  }


};
