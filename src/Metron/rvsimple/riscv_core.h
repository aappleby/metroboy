// RISC-V SiMPLE SV -- Single-cycle RISC-V core
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_RISCV_CORE_H
#define RVSIMPLE_RISCV_CORE_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"
#include "singlecycle_datapath.h"
#include "singlecycle_ctlpath.h"
#include "data_memory_interface.h"

class riscv_core {
public:

  logic<32> pc;
  logic<32> bus_address;
  logic<32> bus_write_data;
  logic<4>  bus_byte_enable;
  logic<1>  bus_read_enable;
  logic<1>  bus_write_enable;

  singlecycle_datapath singlecycle_datapath;
  singlecycle_ctlpath singlecycle_ctlpath;
  data_memory_interface data_memory_interface;

  void tick(logic<1> reset) {
    singlecycle_datapath.tick(
      reset,
      singlecycle_ctlpath.pc_write_enable,
      singlecycle_ctlpath.regfile_write_enable);
  }

  void tock_pc() {
    singlecycle_datapath.tock_pc();

    pc = singlecycle_datapath.pc;
  }

  void tock_execute(logic<32> inst) {
    singlecycle_datapath.tock_regfile();

    singlecycle_datapath.tock_decode(inst);

    singlecycle_ctlpath.tock_decode(singlecycle_datapath.inst_opcode);

    singlecycle_ctlpath.tock_alu_control(
      singlecycle_datapath.inst_funct3,
      singlecycle_datapath.inst_funct7);

    singlecycle_datapath.tock_alu(
      singlecycle_ctlpath.alu_function,
      singlecycle_ctlpath.alu_operand_a_select,
      singlecycle_ctlpath.alu_operand_b_select);

    singlecycle_ctlpath.tock_next_pc_select(
      singlecycle_datapath.inst_opcode,
      singlecycle_datapath.inst_funct3,
      singlecycle_datapath.alu_result_equal_zero2);

    singlecycle_datapath.tock_next_pc(singlecycle_ctlpath.next_pc_select);

    data_memory_interface.tock_bus(
      singlecycle_ctlpath.data_mem_read_enable,
      singlecycle_ctlpath.data_mem_write_enable,
      singlecycle_datapath.inst_funct3,
      singlecycle_datapath.data_mem_address,
      singlecycle_datapath.data_mem_write_data);
    
    bus_address      = data_memory_interface.bus_address;
    bus_write_data   = data_memory_interface.bus_write_data;
    bus_read_enable  = data_memory_interface.bus_read_enable;
    bus_write_enable = data_memory_interface.bus_write_enable;
    bus_byte_enable  = data_memory_interface.bus_byte_enable;
  }

  void tock_writeback(logic<32> bus_read_data) {
    data_memory_interface.tock_read_data(
      singlecycle_datapath.inst_funct3,
      singlecycle_datapath.data_mem_address,
      bus_read_data);

    singlecycle_datapath.tock_writeback(
      data_memory_interface.read_data,
      singlecycle_ctlpath.reg_writeback_select);
  }
};

#endif // RVSIMPLE_RISCV_CORE_H