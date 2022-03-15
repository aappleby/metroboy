// RISC-V SiMPLE SV -- single-cycle data path
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_SINGLECYCLE_DATAPATH_H
#define RVSIMPLE_SINGLECYCLE_DATAPATH_H

#include "metron_tools.h"
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
  logic<32> data_mem_address;
  logic<32> data_mem_write_data;
  logic<32> pc;
  logic<7>  inst_opcode;
  logic<3>  inst_funct3;
  logic<7>  inst_funct7;
  logic<1>  alu_result_equal_zero2;

  adder<32> adder_pc_plus_4;
  adder<32> adder_pc_plus_immediate;
  alu alu;
  instruction_decoder instruction_decoder;
  immediate_generator immediate_generator;
  single_register<32, INITIAL_PC> program_counter;
  multiplexer4<32> mux_next_pc_select;
  multiplexer2<32> mux_operand_a;
  multiplexer2<32> mux_operand_b;
  multiplexer8<32> mux_reg_writeback;
  regfile regfile;

  //----------------------------------------

  void init() {
    program_counter.init();
  }

  //----------------------------------------

  void tick(logic<1> reset,
            logic<1> pc_write_enable,
            logic<1> regfile_write_enable)
  {
    program_counter.tick(reset,
                         pc_write_enable,
                         mux_next_pc_select.out);

    regfile.tick(regfile_write_enable,
                 instruction_decoder.inst_rd,
                 mux_reg_writeback.out);
  }

  //----------------------------------------

  void tock_pc() {
    program_counter.tock();
    pc = program_counter.value;
  }

  void tock_decode(logic<32> inst) {
    instruction_decoder.tock(inst);
    immediate_generator.tock(inst);
    inst_funct7 = instruction_decoder.inst_funct7;
    inst_funct3 = instruction_decoder.inst_funct3;
    inst_opcode = instruction_decoder.inst_opcode;
  }

  void tock_regfile() {
    regfile.tock(instruction_decoder.inst_rs1, instruction_decoder.inst_rs2);
    data_mem_write_data = regfile.rs2_data; 
  }

  void tock_alu(
    logic<5>  alu_function,
    logic<1>  alu_operand_a_select,
    logic<1>  alu_operand_b_select)
  {
    mux_operand_a.tock(alu_operand_a_select,
                       regfile.rs1_data,
                       program_counter.value);

    mux_operand_b.tock(alu_operand_b_select,
                       regfile.rs2_data,
                       immediate_generator.immediate);

    alu.tock(alu_function,
             mux_operand_a.out,
             mux_operand_b.out);

    data_mem_address = alu.result;
    alu_result_equal_zero2 = alu.result_equal_zero;
  }

  void tock_next_pc(logic<2> next_pc_select) {
    adder_pc_plus_immediate.tock(pc, immediate_generator.immediate);
    adder_pc_plus_4.tock(b32(0x00000004), pc);

    mux_next_pc_select.tock(
      next_pc_select,
      adder_pc_plus_4.result,
      adder_pc_plus_immediate.result,
      cat(b31(alu.result, 1), b1(0)),
      b32(0));
  }

  void tock_writeback(logic<32> data_mem_read_data,
                      logic<3>  reg_writeback_select)
  {   
    mux_reg_writeback.tock(
      reg_writeback_select,
      alu.result,
      data_mem_read_data,
      adder_pc_plus_4.result,
      immediate_generator.immediate,
      b32(0),
      b32(0),
      b32(0),
      b32(0));
  }
};

#endif // RVSIMPLE_SINGLECYCLE_DATAPATH_H