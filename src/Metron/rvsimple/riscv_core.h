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

  singlecycle_datapath  datapath;
  singlecycle_ctlpath   ctlpath;
  data_memory_interface dmem;

  //----------------------------------------

  void tick(logic<1> reset) {
    datapath.tick(
      reset,
      ctlpath.pc_write_enable,
      ctlpath.regfile_write_enable);
  }

  //----------------------------------------

  void tock_pc() {
    datapath.tock_pc();
    pc = datapath.pc;
  }

  void tock_execute(logic<32> inst) {
    datapath.tock_decode(inst);

    datapath.tock_regfile();

    ctlpath.tock_decode(datapath.inst_opcode);

    ctlpath.tock_alu_control(
      datapath.inst_funct3,
      datapath.inst_funct7);

    datapath.tock_alu(
      ctlpath.alu_function,
      ctlpath.alu_operand_a_select,
      ctlpath.alu_operand_b_select);

    ctlpath.tock_next_pc_select(
      datapath.inst_opcode,
      datapath.inst_funct3,
      datapath.alu_result_equal_zero2);

    datapath.tock_next_pc(ctlpath.next_pc_select);

    dmem.tock_bus(
      ctlpath.data_mem_read_enable,
      ctlpath.data_mem_write_enable,
      datapath.inst_funct3,
      datapath.data_mem_address,
      datapath.data_mem_write_data);
    
    bus_address      = dmem.bus_address;
    bus_write_data   = dmem.bus_write_data;
    bus_read_enable  = dmem.bus_read_enable;
    bus_write_enable = dmem.bus_write_enable;
    bus_byte_enable  = dmem.bus_byte_enable;
  }

  void tock_writeback(logic<32> bus_read_data) {
    dmem.tock_read_data(
      datapath.inst_funct3,
      datapath.data_mem_address,
      bus_read_data);

    datapath.tock_writeback(
      dmem.read_data,
      ctlpath.reg_writeback_select);
  }

  //----------------------------------------
};

#endif // RVSIMPLE_RISCV_CORE_H