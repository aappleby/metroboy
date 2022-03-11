// RISC-V SiMPLE SV -- single-cycle data path
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_SINGLECYCLE_DATAPATH_H
#define RVSIMPLE_SINGLECYCLE_DATAPATH_H

#include "../metron_tools.h"
#include "config.h"
#include "constants.h"

#include "adder.h"
#include "alu.h"
#include "instruction_decoder.h"
#include "immediate_generator.h"
#include "register.h"
#include "regfile.h"
#include "multiplexer2.h"
#include "multiplexer4.h"
#include "multiplexer8.h"

using namespace rv_config;

class singlecycle_datapath {
public:
  logic<32> o_data_mem_address;
  logic<32> o_data_mem_write_data;
  logic<32> o_pc;
  logic<7>  o_inst_opcode;
  logic<3>  o_inst_funct3;
  logic<7>  o_inst_funct7;
  logic<1>  o_alu_result_equal_zero;

  adder<32> adder_pc_plus_4;

  adder<32> adder_pc_plus_immediate;

  alu alu;

  instruction_decoder instruction_decoder;

  immediate_generator immediate_generator;

  _register<32, INITIAL_PC> program_counter;

  multiplexer4<32> mux_next_pc_select;

  multiplexer2<32> mux_operand_a;

  multiplexer2<32> mux_operand_b;

  multiplexer8<32> mux_reg_writeback;

  regfile regfile;

  void init() {
    program_counter.init();
  }

  void tick(logic<1> i_reset,
            logic<1> i_pc_write_enable,
            logic<1> i_regfile_write_enable)
  {
    program_counter.tick(i_reset,
                         i_pc_write_enable,
                         mux_next_pc_select.o_out);

    regfile.tick(i_regfile_write_enable,
                 instruction_decoder.o_inst_rd,
                 instruction_decoder.o_inst_rs1,
                 instruction_decoder.o_inst_rs2,
                 mux_reg_writeback.o_out);
  }

  void tock_pc() {
    program_counter.tock();
    o_pc = program_counter.o_value;
  }

  void tock_regfile() {
    regfile.tock();
    o_data_mem_write_data = regfile.o_rs2_data; 
  }

  void tock_alu(
    logic<5>  i_alu_function,
    logic<1>  i_alu_operand_a_select,
    logic<1>  i_alu_operand_b_select)
  {
    mux_operand_a.tock(i_alu_operand_a_select,
                       regfile.o_rs1_data,
                       program_counter.o_value);

    mux_operand_b.tock(i_alu_operand_b_select,
                       regfile.o_rs2_data,
                       immediate_generator.o_immediate);

    alu.tock(i_alu_function,
             mux_operand_a.o_out,
             mux_operand_b.o_out);

    o_data_mem_address = alu.o_result;
    o_alu_result_equal_zero = alu.o_result_equal_zero;
  }

  void tock_decode(logic<32> i_inst) {
    instruction_decoder.tock(i_inst);
    immediate_generator.tock(i_inst);
    o_inst_funct7 = instruction_decoder.o_inst_funct7;
    o_inst_funct3 = instruction_decoder.o_inst_funct3;
    o_inst_opcode = instruction_decoder.o_inst_opcode;

  }

  void tock_next_pc(logic<2> i_next_pc_select) {
    adder_pc_plus_immediate.tock(o_pc, immediate_generator.o_immediate);
    adder_pc_plus_4.tock(b32(0x00000004), o_pc);

    mux_next_pc_select.tock(
      i_next_pc_select,
      adder_pc_plus_4.o_result,
      adder_pc_plus_immediate.o_result,
      cat(b31(alu.o_result, 1), b1(0)),
      b32(0));
  }

  void tock_writeback(logic<32> i_data_mem_read_data,
                      logic<3>  i_reg_writeback_select)
  {   
    mux_reg_writeback.tock(
      i_reg_writeback_select,
      alu.o_result,
      i_data_mem_read_data,
      adder_pc_plus_4.o_result,
      immediate_generator.o_immediate,
      b32(0),
      b32(0),
      b32(0),
      b32(0));
  }
};

#endif // RVSIMPLE_SINGLECYCLE_DATAPATH_H