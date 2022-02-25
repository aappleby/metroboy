// RISC-V SiMPLE SV -- Single-cycle RISC-V core
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../../../src/Metron/metron_tools.h"
#include "../../synth/config.h"
#include "../common/constants.h"
#include "singlecycle_datapath.h"
#include "singlecycle_ctlpath.h"
#include "../common/data_memory_interface.h"

struct riscv_core {
  logic<32> o_pc;
  logic<32> o_bus_address;
  logic<32> o_bus_write_data;
  logic<4>  o_bus_byte_enable;
  logic<1>  o_bus_read_enable;
  logic<1>  o_bus_write_enable;

  singlecycle_datapath singlecycle_datapath;
  singlecycle_ctlpath singlecycle_ctlpath;
  data_memory_interface data_memory_interface;

  void tick(logic<1> i_reset) {
    singlecycle_datapath.tick(
      i_reset,
      singlecycle_ctlpath.o_pc_write_enable,
      singlecycle_ctlpath.o_regfile_write_enable);
  }

  void tock_pc() {
    singlecycle_datapath.tock_pc();

    o_pc = singlecycle_datapath.o_pc;
  }

  void tock_execute(logic<32> i_inst) {
    singlecycle_datapath.tock_regfile();

    singlecycle_datapath.tock_decode(i_inst);

    singlecycle_ctlpath.tock_decode(singlecycle_datapath.o_inst_opcode);

    singlecycle_ctlpath.tock_alu_control(
      singlecycle_datapath.o_inst_funct3,
      singlecycle_datapath.o_inst_funct7);

    singlecycle_datapath.tock_alu(
      singlecycle_ctlpath.o_alu_function,
      singlecycle_ctlpath.o_alu_operand_a_select,
      singlecycle_ctlpath.o_alu_operand_b_select);

    singlecycle_ctlpath.tock_next_pc_select(
      singlecycle_datapath.o_inst_opcode,
      singlecycle_datapath.o_inst_funct3,
      singlecycle_datapath.o_alu_result_equal_zero);

    singlecycle_datapath.tock_next_pc(singlecycle_ctlpath.o_next_pc_select);

    data_memory_interface.tock_bus(
      singlecycle_ctlpath.o_data_mem_read_enable,
      singlecycle_ctlpath.o_data_mem_write_enable,
      singlecycle_datapath.o_inst_funct3,
      singlecycle_datapath.o_data_mem_address,
      singlecycle_datapath.o_data_mem_write_data);
    
    o_bus_address      = data_memory_interface.o_bus_address;
    o_bus_write_data   = data_memory_interface.o_bus_write_data;
    o_bus_read_enable  = data_memory_interface.o_bus_read_enable;
    o_bus_write_enable = data_memory_interface.o_bus_write_enable;
    o_bus_byte_enable  = data_memory_interface.o_bus_byte_enable;
  }

  void tock_writeback(logic<32> i_bus_read_data) {
    data_memory_interface.tock_read_data(
      singlecycle_datapath.o_inst_funct3,
      singlecycle_datapath.o_data_mem_address,
      i_bus_read_data);

    singlecycle_datapath.tock_writeback(
      data_memory_interface.o_read_data,
      singlecycle_ctlpath.o_reg_writeback_select);
  }
};
