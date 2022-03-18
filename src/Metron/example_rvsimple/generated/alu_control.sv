// RISC-V SiMPLE SV -- ALU controller module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_ALU_CONTROL_H
`define RVSIMPLE_ALU_CONTROL_H

`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

module alu_control
(
  input logic clock,
  input logic[1:0] alu_op_type,
  input logic[2:0] inst_funct3,
  input logic[6:0] inst_funct7,
  output logic[4:0] alu_function
);
 /*public:*/
  /*logic<5> alu_function;*/

  always_comb begin : tock
    logic[4:0] default_funct;
    logic[4:0] secondary_funct;
    logic[4:0] op_funct;
    logic[4:0] op_imm_funct;
    logic[4:0] branch_funct;
    import rv_constants::*;

    
    case (inst_funct3) 
      /*case*/ FUNCT3_ALU_ADD_SUB:
        default_funct = ALU_ADD;
        /*break;*/
      /*case*/ FUNCT3_ALU_SLL:
        default_funct = ALU_SLL;
        /*break;*/
      /*case*/ FUNCT3_ALU_SLT:
        default_funct = ALU_SLT;
        /*break;*/
      /*case*/ FUNCT3_ALU_SLTU:
        default_funct = ALU_SLTU;
        /*break;*/
      /*case*/ FUNCT3_ALU_XOR:
        default_funct = ALU_XOR;
        /*break;*/
      /*case*/ FUNCT3_ALU_SHIFTR:
        default_funct = ALU_SRL;
        /*break;*/
      /*case*/ FUNCT3_ALU_OR:
        default_funct = ALU_OR;
        /*break;*/
      /*case*/ FUNCT3_ALU_AND:
        default_funct = ALU_AND;
        /*break;*/
      default:
        default_funct = 5'x;
        /*break;*/
    endcase

    
    case (inst_funct3) 
      /*case*/ FUNCT3_ALU_ADD_SUB:
        secondary_funct = ALU_SUB;
        /*break;*/
      /*case*/ FUNCT3_ALU_SHIFTR:
        secondary_funct = ALU_SRA;
        /*break;*/
      default:
        secondary_funct = 5'x;
        /*break;*/
    endcase

    
    if (inst_funct7[5])
      op_funct = secondary_funct;
    else
      op_funct = default_funct;

    
    if (inst_funct7[5] && 2'(inst_funct3) == 2'b01)
      op_imm_funct = secondary_funct;
    else
      op_imm_funct = default_funct;

    
    case (inst_funct3) 
      /*case*/ FUNCT3_BRANCH_EQ:
        branch_funct = ALU_SEQ;
        /*break;*/
      /*case*/ FUNCT3_BRANCH_NE:
        branch_funct = ALU_SEQ;
        /*break;*/
      /*case*/ FUNCT3_BRANCH_LT:
        branch_funct = ALU_SLT;
        /*break;*/
      /*case*/ FUNCT3_BRANCH_GE:
        branch_funct = ALU_SLT;
        /*break;*/
      /*case*/ FUNCT3_BRANCH_LTU:
        branch_funct = ALU_SLTU;
        /*break;*/
      /*case*/ FUNCT3_BRANCH_GEU:
        branch_funct = ALU_SLTU;
        /*break;*/
      default:
        branch_funct = 5'x;
        /*break;*/
    endcase

    case (alu_op_type) 
      /*case*/ CTL_ALU_ADD:
        alu_function = ALU_ADD;
        /*break;*/
      /*case*/ CTL_ALU_OP:
        alu_function = op_funct;
        /*break;*/
      /*case*/ CTL_ALU_OP_IMM:
        alu_function = op_imm_funct;
        /*break;*/
      /*case*/ CTL_ALU_BRANCH:
        alu_function = branch_funct;
        /*break;*/
      default:
        alu_function = 5'x;
        /*break;*/
    endcase
  end
endmodule;

`endif  // RVSIMPLE_ALU_CONTROL_H
