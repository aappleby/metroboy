// RISC-V SiMPLE SV -- single-cycle data path
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_SINGLECYCLE_DATAPATH_H
`define RVSIMPLE_SINGLECYCLE_DATAPATH_H

`include "adder.sv"
`include "alu.sv"
`include "config.sv"
`include "constants.sv"
`include "immediate_generator.sv"
`include "instruction_decoder.sv"
`include "metron_tools.sv"
`include "multiplexer2.sv"
`include "multiplexer4.sv"
`include "multiplexer8.sv"
`include "regfile.sv"
`include "register.sv"

import rv_config::*;

module singlecycle_datapath
(
  input logic clock,
  input logic reset,
  input logic pc_write_enable,
  input logic regfile_write_enable,
  input logic[31:0] inst,
  input logic[4:0] alu_function,
  input logic alu_operand_a_select,
  input logic alu_operand_b_select,
  input logic[1:0] next_pc_select,
  input logic[31:0] data_mem_read_data,
  input logic[2:0] reg_writeback_select,
  output logic[31:0] pc,
  output logic[6:0] inst_funct7,
  output logic[2:0] inst_funct3,
  output logic[6:0] inst_opcode,
  output logic[31:0] data_mem_write_data,
  output logic[31:0] data_mem_address,
  output logic alu_result_equal_zero2
);
  // Submodule output port bindings
  logic[32-1:0] adder_pc_plus_4_result;
  logic[32-1:0] adder_pc_plus_immediate_result;
  logic[31:0] alu_core_result;
  logic alu_core_result_equal_zero;
  logic[6:0] idec_inst_opcode;
  logic[2:0] idec_inst_funct3;
  logic[6:0] idec_inst_funct7;
  logic[4:0] idec_inst_rd;
  logic[4:0] idec_inst_rs1;
  logic[4:0] idec_inst_rs2;
  logic[31:0] igen_immediate;
  logic[32-1:0] program_counter_value;
  logic[32-1:0] mux_next_pc_select_out;
  logic[32-1:0] mux_operand_a_out;
  logic[32-1:0] mux_operand_b_out;
  logic[32-1:0] mux_reg_writeback_out;
  logic[31:0] regs_rs1_data;
  logic[31:0] regs_rs2_data;

 /*public:*/
  /*logic<32> data_mem_address;*/
  /*logic<32> data_mem_write_data;*/
  /*logic<32> pc;*/
  /*logic<7> inst_opcode;*/
  /*logic<3> inst_funct3;*/
  /*logic<7> inst_funct7;*/
  /*logic<1> alu_result_equal_zero2;*/

  //----------------------------------------

  initial begin : init /*program_counter.init();*/ end

  //----------------------------------------

  always_ff @(posedge clock) begin : tick
    /*program_counter.tick(reset, pc_write_enable, mux_next_pc_select.out);*/

    /*regs.tick(regfile_write_enable, idec.inst_rd, mux_reg_writeback.out);*/
  end

  //----------------------------------------

  always_comb begin : tock_pc
    /*program_counter.tock();*/
    pc = program_counter_value;
  end

  always_comb begin : tock_decode
    /*idec.tock(inst);*/
    /*igen.tock(inst);*/
    inst_funct7 = idec_inst_funct7;
    inst_funct3 = idec_inst_funct3;
    inst_opcode = idec_inst_opcode;
  end

  always_comb begin : tock_regfile
    /*regs.tock(idec.inst_rs1, idec.inst_rs2);*/
    data_mem_write_data = regs_rs2_data;
  end

  always_comb begin : tock_alu
    /*mux_operand_a.tock(alu_operand_a_select, regs.rs1_data,
                       program_counter.value);*/

    /*mux_operand_b.tock(alu_operand_b_select, regs.rs2_data, igen.immediate);*/

    /*alu_core.tock(alu_function, mux_operand_a.out, mux_operand_b.out);*/

    data_mem_address = alu_core_result;
    alu_result_equal_zero2 = alu_core_result_equal_zero;
  end

  always_comb begin : tock_next_pc
    /*adder_pc_plus_immediate.tock(pc, igen.immediate);*/
    /*adder_pc_plus_4.tock(b32(0x00000004), pc);*/

    /*mux_next_pc_select.tock(next_pc_select, adder_pc_plus_4.result,
                            adder_pc_plus_immediate.result,
                            cat(b31(alu_core.result, 1), b1(0)), b32(0));*/
  end

  always_comb begin : tock_writeback
    /*mux_reg_writeback.tock(reg_writeback_select, alu_core.result,
                           data_mem_read_data, adder_pc_plus_4.result,
                           igen.immediate, b32(0), b32(0), b32(0), b32(0));*/
  end

  //----------------------------------------

 /*private:*/
  adder #(32) adder_pc_plus_4(
    // Inputs
    .clock(clock),
    .operand_a(32'h00000004), 
    .operand_b(pc), 
    // Outputs
    .result(adder_pc_plus_4_result)
  );

  adder #(32) adder_pc_plus_immediate(
    // Inputs
    .clock(clock),
    .operand_a(pc), 
    .operand_b(igen_immediate), 
    // Outputs
    .result(adder_pc_plus_immediate_result)
  );

  alu alu_core(
    // Inputs
    .clock(clock),
    .alu_function(alu_function), 
    .operand_a(mux_operand_a_out), 
    .operand_b(mux_operand_b_out), 
    // Outputs
    .result(alu_core_result), 
    .result_equal_zero(alu_core_result_equal_zero)
  );

  instruction_decoder idec(
    // Inputs
    .clock(clock),
    .inst(inst), 
    // Outputs
    .inst_opcode(idec_inst_opcode), 
    .inst_funct3(idec_inst_funct3), 
    .inst_funct7(idec_inst_funct7), 
    .inst_rd(idec_inst_rd), 
    .inst_rs1(idec_inst_rs1), 
    .inst_rs2(idec_inst_rs2)
  );

  immediate_generator igen(
    // Inputs
    .clock(clock),
    .inst(inst), 
    // Outputs
    .immediate(igen_immediate)
  );

  single_register #(32, INITIAL_PC) program_counter(
    // Inputs
    .clock(clock),
    .reset(reset), 
    .write_enable(pc_write_enable), 
    .next(mux_next_pc_select_out), 
    // Outputs
    .value(program_counter_value)
  );

  multiplexer4 #(32) mux_next_pc_select(
    // Inputs
    .clock(clock),
    .sel(next_pc_select), 
    .in0(adder_pc_plus_4_result), 
    .in1(adder_pc_plus_immediate_result), 
    .in2({alu_core.result[31:1], 1'd0}), 
    .in3(32'd0), 
    // Outputs
    .out(mux_next_pc_select_out)
  );

  multiplexer2 #(32) mux_operand_a(
    // Inputs
    .clock(clock),
    .sel(alu_operand_a_select), 
    .in0(regs_rs1_data), 
    .in1(program_counter_value), 
    // Outputs
    .out(mux_operand_a_out)
  );

  multiplexer2 #(32) mux_operand_b(
    // Inputs
    .clock(clock),
    .sel(alu_operand_b_select), 
    .in0(regs_rs2_data), 
    .in1(igen_immediate), 
    // Outputs
    .out(mux_operand_b_out)
  );

  multiplexer8 #(32) mux_reg_writeback(
    // Inputs
    .clock(clock),
    .sel(reg_writeback_select), 
    .in0(alu_core_result), 
    .in1(data_mem_read_data), 
    .in2(adder_pc_plus_4_result), 
    .in3(igen_immediate), 
    .in4(32'd0), 
    .in5(32'd0), 
    .in6(32'd0), 
    .in7(32'd0), 
    // Outputs
    .out(mux_reg_writeback_out)
  );

  regfile regs(
    // Inputs
    .clock(clock),
    .write_enable(regfile_write_enable), 
    .rd_address(idec_inst_rd), 
    .rd_data(mux_reg_writeback_out), 
    .rs1_address(idec_inst_rs1), 
    .rs2_address(idec_inst_rs2), 
    // Outputs
    .rs1_data(regs_rs1_data), 
    .rs2_data(regs_rs2_data)
  );

endmodule;

`endif  // RVSIMPLE_SINGLECYCLE_DATAPATH_H
