// RISC-V SiMPLE SV -- ALU module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_ALU_H
`define RVSIMPLE_ALU_H

`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

module alu
(
  input logic clock,
  input logic[4:0] alu_function,
  input logic[31:0] operand_a,
  input logic[31:0] operand_b,
  output logic[31:0] result,
  output logic result_equal_zero
);
 /*public:*/
  /*logic<32> result;*/
  /*logic<1> result_equal_zero;*/

  always_comb begin : tock
    import rv_constants::*;

    case (alu_function) 
      /*case*/ ALU_ADD:
        result = operand_a + operand_b;
        /*break;*/
      /*case*/ ALU_SUB:
        result = operand_a - operand_b;
        /*break;*/
      /*case*/ ALU_SLL:
        result = operand_a << 5'(operand_b);
        /*break;*/
      /*case*/ ALU_SRL:
        result = operand_a >> 5'(operand_b);
        /*break;*/
      /*case*/ ALU_SRA:
        result = ($signed(operand_a) >>> 5'(operand_b));
        /*break;*/
      /*case*/ ALU_SEQ:
        result = {31'd0, 1'(operand_a == operand_b)};
        /*break;*/
      /*case*/ ALU_SLT:
        result = {31'd0, 1'($signed(operand_a) < $signed(operand_b))};
        /*break;*/
      /*case*/ ALU_SLTU:
        result = {31'd0, 1'($unsigned(operand_a) < $unsigned(operand_b))};
        /*break;*/
      /*case*/ ALU_XOR:
        result = operand_a ^ operand_b;
        /*break;*/
      /*case*/ ALU_OR:
        result = operand_a | operand_b;
        /*break;*/
      /*case*/ ALU_AND:
        result = operand_a & operand_b;
        /*break;*/
      default:
        result = 32'(ZERO);
        /*break;*/
    endcase

    result_equal_zero = (result == 32'd0);
  end
endmodule;

`endif  // RVSIMPLE_ALU_H
