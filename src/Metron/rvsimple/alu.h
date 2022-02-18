// RISC-V SiMPLE SV -- ALU module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../../../src/uart_test/metron_tools.h"
#include "../../synth/config.h"
#include "constants.h"

struct alu {

  logic<32> o_result;
  logic<1>  o_result_equal_zero;

/*
#ifdef M_MODULE
        logic [63:0] signed_multiplication;
        logic [63:0] unsigned_multiplication;
        logic [63:0] signed_unsigned_multiplication;
#endif
*/
    
  void tock(
    logic<5>  i_alu_function,
    logic<32> i_operand_a, // signed?
    logic<32> i_operand_b  // signed?
  ) {
    o_result = b32(ZERO);
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
/*
#ifdef M_MODULE
    case ALU_MUL:    result_o = b32(signed_multiplication);
    case ALU_MULH:   result_o = b32(signed_multiplication, 32);
    case ALU_MULHSU: result_o = b32(signed_unsigned_multiplication, 32);
    case ALU_MULHU:  result_o = b32(unsigned_multiplication, 32);
    case ALU_DIV:
      if (operand_b == ZERO)
        result = b32(1);
      else if ((operand_a == 0x80000000) && (operand_b == b32(1)))
        result = 0x80000000;
      else
        result = int64_t(operand_a) / int64_t(operand_b);
    case ALU_DIVU:
      if (operand_b == ZERO)
        result = b32(1);
      else
        result = uint64_t(operand_a) / uint64_t(operand_b);
    case ALU_REM:
      if (operand_b == ZERO)
        result = operand_a;
      else if ((operand_a == b32(0x80000000)) && (operand_b == b32(1)))
        result = ZERO;
      else
        result = int64_t(operand_a) % int64_t(operand_b);
    case ALU_REMU:
      if (operand_b == ZERO)
        result = operand_a;
      else
        result = uint64_t(operand_a) % uint64_t(operand_b);
#endif
*/
    default:
      o_result = ZERO; break;
    }

    o_result_equal_zero = (o_result == b32(0));
  }
/*  
#ifdef M_MODULE
        always_comb begin
            signed_multiplication   = operand_a * operand_b;
            unsigned_multiplication = $unsigned(operand_a) * $unsigned(operand_b);
            signed_unsigned_multiplication = $signed(operand_a) * $unsigned(operand_b);
        end
#endif
*/

};