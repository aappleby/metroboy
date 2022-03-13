// RISC-V SiMPLE SV -- control path
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_SINGLECYCLE_CTLPATH_H
#define RVSIMPLE_SINGLECYCLE_CTLPATH_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

#include "singlecycle_control.h"
#include "control_transfer.h"
#include "alu_control.h"

class singlecycle_ctlpath {
public:

  logic<1> pc_write_enable;
  logic<1> regfile_write_enable;
  logic<1> alu_operand_a_select;
  logic<1> alu_operand_b_select;
  logic<1> data_mem_read_enable;
  logic<1> data_mem_write_enable;
  logic<3> reg_writeback_select;
  logic<5> alu_function;
  logic<2> next_pc_select;

  //----------------------------------------

  singlecycle_control singlecycle_control;
  control_transfer control_transfer;
  alu_control alu_control;

  //----------------------------------------

  void tock_decode(logic<7> inst_opcode) {
    singlecycle_control.tock_decode(inst_opcode);
    pc_write_enable       = singlecycle_control.pc_write_enable;
    regfile_write_enable  = singlecycle_control.regfile_write_enable;
    alu_operand_a_select  = singlecycle_control.alu_operand_a_select;
    alu_operand_b_select  = singlecycle_control.alu_operand_b_select;
    data_mem_read_enable  = singlecycle_control.data_mem_read_enable;
    data_mem_write_enable = singlecycle_control.data_mem_write_enable;
    reg_writeback_select  = singlecycle_control.reg_writeback_select;
  }

  void tock_next_pc_select(logic<7> inst_opcode,
                           logic<3> inst_funct3,
                           logic<1> alu_result_equal_zero)
  {
    control_transfer.tock(
      inst_funct3,
      alu_result_equal_zero);

    singlecycle_control.tock_next_pc_select(
      inst_opcode,
      control_transfer.take_branch);
    next_pc_select        = singlecycle_control.next_pc_select;
  }

  void tock_alu_control(logic<3> inst_funct3,
                        logic<7> inst_funct7)
  {
    alu_control.tock(
      singlecycle_control.alu_op_type,
      inst_funct3,
      inst_funct7);
    alu_function = alu_control.alu_function;
  }

};

#endif // RVSIMPLE_SINGLECYCLE_CTLPATH_H