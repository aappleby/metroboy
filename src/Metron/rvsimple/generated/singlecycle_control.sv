// RISC-V SiMPLE SV -- single-cycle controller
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_SINGLECYCLE_CONTROL_H
`define RVSIMPLE_SINGLECYCLE_CONTROL_H

`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

module singlecycle_control
(
  input logic clock,
  input logic[6:0] inst_opcode,
  input logic take_branch,
  output logic[1:0] next_pc_select,
  output logic pc_write_enable,
  output logic regfile_write_enable,
  output logic alu_operand_a_select,
  output logic alu_operand_b_select,
  output logic[1:0] alu_op_type,
  output logic data_mem_read_enable,
  output logic data_mem_write_enable,
  output logic[2:0] reg_writeback_select
);
 /*public:*/
  /*logic<1> pc_write_enable;*/
  /*logic<1> regfile_write_enable;*/
  /*logic<1> alu_operand_a_select;*/
  /*logic<1> alu_operand_b_select;*/
  /*logic<2> alu_op_type;*/
  /*logic<1> data_mem_read_enable;*/
  /*logic<1> data_mem_write_enable;*/
  /*logic<3> reg_writeback_select;*/
  /*logic<2> next_pc_select;*/

  always_comb begin : tock_next_pc_select
    import rv_constants::*;
    case (inst_opcode) 
      /*case*/ OPCODE_BRANCH:
        next_pc_select = take_branch ? CTL_PC_PC_IMM : CTL_PC_PC4;
        /*break;*/
      /*case*/ OPCODE_JALR:
        next_pc_select = CTL_PC_RS1_IMM;
        /*break;*/
      /*case*/ OPCODE_JAL:
        next_pc_select = CTL_PC_PC_IMM;
        /*break;*/
      default:
        next_pc_select = CTL_PC_PC4;
        /*break;*/
    endcase
  end

  always_comb begin : tock_decode
    import rv_constants::*;

    case (inst_opcode) 
      /*case*/ OPCODE_LOAD: begin
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd1;
        alu_operand_a_select = CTL_ALU_A_RS1;
        alu_operand_b_select = CTL_ALU_B_IMM;
        alu_op_type = CTL_ALU_ADD;
        data_mem_read_enable = 1'd1;
        data_mem_write_enable = 1'd0;
        reg_writeback_select = CTL_WRITEBACK_DATA;
        /*break;*/
      end

      /*case*/ OPCODE_MISC_MEM: begin
        // Fence - ignore
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd0;
        alu_operand_a_select = 1'x;
        alu_operand_b_select = 1'x;
        alu_op_type = 2'x;
        data_mem_read_enable = 1'd0;
        data_mem_write_enable = 1'd0;
        reg_writeback_select = 3'x;
        /*break;*/
      end

      /*case*/ OPCODE_OP_IMM: begin
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd1;
        alu_operand_a_select = CTL_ALU_A_RS1;
        alu_operand_b_select = CTL_ALU_B_IMM;
        alu_op_type = CTL_ALU_OP_IMM;
        data_mem_read_enable = 1'd0;
        data_mem_write_enable = 1'd0;
        reg_writeback_select = CTL_WRITEBACK_ALU;
        /*break;*/
      end

      /*case*/ OPCODE_AUIPC: begin
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd1;
        alu_operand_a_select = CTL_ALU_A_PC;
        alu_operand_b_select = CTL_ALU_B_IMM;
        alu_op_type = CTL_ALU_ADD;
        data_mem_read_enable = 1'd0;
        data_mem_write_enable = 1'd0;
        reg_writeback_select = CTL_WRITEBACK_ALU;
        /*break;*/
      end

      /*case*/ OPCODE_STORE: begin
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd0;
        alu_operand_a_select = CTL_ALU_A_RS1;
        alu_operand_b_select = CTL_ALU_B_IMM;
        alu_op_type = CTL_ALU_ADD;
        data_mem_read_enable = 1'd0;
        data_mem_write_enable = 1'd1;
        reg_writeback_select = 3'x;
        /*break;*/
      end

      /*case*/ OPCODE_OP: begin
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd1;
        alu_operand_a_select = CTL_ALU_A_RS1;
        alu_operand_b_select = CTL_ALU_B_RS2;
        reg_writeback_select = CTL_WRITEBACK_ALU;
        alu_op_type = CTL_ALU_OP;
        data_mem_read_enable = 1'd0;
        data_mem_write_enable = 1'd0;
        /*break;*/
      end

      /*case*/ OPCODE_LUI: begin
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd1;
        alu_operand_a_select = CTL_ALU_A_RS1;
        alu_operand_b_select = CTL_ALU_B_RS2;
        alu_op_type = 2'x;
        data_mem_read_enable = 1'd0;
        data_mem_write_enable = 1'd0;
        reg_writeback_select = CTL_WRITEBACK_IMM;
        /*break;*/
      end

      /*case*/ OPCODE_BRANCH: begin
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd0;
        alu_operand_a_select = CTL_ALU_A_RS1;
        alu_operand_b_select = CTL_ALU_B_RS2;
        alu_op_type = CTL_ALU_BRANCH;
        data_mem_read_enable = 1'd0;
        data_mem_write_enable = 1'd0;
        reg_writeback_select = 3'x;
        /*break;*/
      end

      /*case*/ OPCODE_JALR: begin
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd1;
        alu_operand_a_select = CTL_ALU_A_RS1;
        alu_operand_b_select = CTL_ALU_B_IMM;
        alu_op_type = CTL_ALU_ADD;
        data_mem_read_enable = 1'd0;
        data_mem_write_enable = 1'd0;
        reg_writeback_select = CTL_WRITEBACK_PC4;
        /*break;*/
      end

      /*case*/ OPCODE_JAL: begin
        pc_write_enable = 1'd1;
        regfile_write_enable = 1'd1;
        alu_operand_a_select = CTL_ALU_A_PC;
        alu_operand_b_select = CTL_ALU_B_IMM;
        alu_op_type = CTL_ALU_ADD;
        data_mem_read_enable = 1'd0;
        data_mem_write_enable = 1'd0;
        reg_writeback_select = CTL_WRITEBACK_PC4;
        /*break;*/
      end

      default: begin
        pc_write_enable = 1'x;
        regfile_write_enable = 1'x;
        alu_operand_a_select = CTL_ALU_A_RS1;
        alu_operand_b_select = CTL_ALU_B_IMM;
        alu_op_type = CTL_ALU_ADD;
        data_mem_read_enable = 1'x;
        data_mem_write_enable = 1'x;
        reg_writeback_select = 3'x;
        /*break;*/
      end
    endcase
  end
endmodule;

`endif  // RVSIMPLE_SINGLECYCLE_CONTROL_H
