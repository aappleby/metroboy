// RISC-V SiMPLE SV -- control path
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_SINGLECYCLE_CTLPATH_H
`define RVSIMPLE_SINGLECYCLE_CTLPATH_H

`include "alu_control.sv"
`include "config.sv"
`include "constants.sv"
`include "control_transfer.sv"
`include "metron_tools.sv"
`include "singlecycle_control.sv"

module singlecycle_ctlpath
(
  input logic clock,
  input logic[6:0] inst_opcode,
  input logic[2:0] inst_funct3,
  input logic alu_result_equal_zero,
  input logic[6:0] inst_funct7,
  output logic pc_write_enable,
  output logic regfile_write_enable,
  output logic alu_operand_a_select,
  output logic alu_operand_b_select,
  output logic data_mem_read_enable,
  output logic data_mem_write_enable,
  output logic[2:0] reg_writeback_select,
  output logic[1:0] next_pc_select,
  output logic[4:0] alu_function
);
  // Submodule output port bindings
  logic[1:0] control_next_pc_select;
  logic control_pc_write_enable;
  logic control_regfile_write_enable;
  logic control_alu_operand_a_select;
  logic control_alu_operand_b_select;
  logic[1:0] control_alu_op_type;
  logic control_data_mem_read_enable;
  logic control_data_mem_write_enable;
  logic[2:0] control_reg_writeback_select;
  logic transfer_take_branch;
  logic[4:0] alu_alu_function;

 /*public:*/
  /*logic<1> pc_write_enable;*/
  /*logic<1> regfile_write_enable;*/
  /*logic<1> alu_operand_a_select;*/
  /*logic<1> alu_operand_b_select;*/
  /*logic<1> data_mem_read_enable;*/
  /*logic<1> data_mem_write_enable;*/
  /*logic<3> reg_writeback_select;*/
  /*logic<5> alu_function;*/
  /*logic<2> next_pc_select;*/

  //----------------------------------------

  always_comb begin : tock_decode
    /*control.tock_decode(inst_opcode);*/
    pc_write_enable = control_pc_write_enable;
    regfile_write_enable = control_regfile_write_enable;
    alu_operand_a_select = control_alu_operand_a_select;
    alu_operand_b_select = control_alu_operand_b_select;
    data_mem_read_enable = control_data_mem_read_enable;
    data_mem_write_enable = control_data_mem_write_enable;
    reg_writeback_select = control_reg_writeback_select;
  end

  always_comb begin : tock_next_pc_select
    /*transfer.tock(inst_funct3, alu_result_equal_zero);*/

    /*control.tock_next_pc_select(inst_opcode, transfer.take_branch);*/
    next_pc_select = control_next_pc_select;
  end

  always_comb begin : tock_alu_control
    /*alu.tock(control.alu_op_type, inst_funct3, inst_funct7);*/
    alu_function = alu_alu_function;
  end

  //----------------------------------------

 /*private:*/
  singlecycle_control control(
    // Inputs
    .clock(clock),
    .inst_opcode(inst_opcode), 
    .take_branch(transfer_take_branch), 
    // Outputs
    .next_pc_select(control_next_pc_select), 
    .pc_write_enable(control_pc_write_enable), 
    .regfile_write_enable(control_regfile_write_enable), 
    .alu_operand_a_select(control_alu_operand_a_select), 
    .alu_operand_b_select(control_alu_operand_b_select), 
    .alu_op_type(control_alu_op_type), 
    .data_mem_read_enable(control_data_mem_read_enable), 
    .data_mem_write_enable(control_data_mem_write_enable), 
    .reg_writeback_select(control_reg_writeback_select)
  );

  control_transfer transfer(
    // Inputs
    .clock(clock),
    .inst_funct3(inst_funct3), 
    .result_equal_zero(alu_result_equal_zero), 
    // Outputs
    .take_branch(transfer_take_branch)
  );

  alu_control alu(
    // Inputs
    .clock(clock),
    .alu_op_type(control_alu_op_type), 
    .inst_funct3(inst_funct3), 
    .inst_funct7(inst_funct7), 
    // Outputs
    .alu_function(alu_alu_function)
  );

endmodule;

`endif  // RVSIMPLE_SINGLECYCLE_CTLPATH_H
