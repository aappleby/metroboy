// RISC-V SiMPLE SV -- control path
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../metron_tools.h"
#include "config.h"
#include "constants.h"

#include "singlecycle_control.h"
#include "control_transfer.h"
#include "alu_control.h"

struct singlecycle_ctlpath {

  logic<1> o_pc_write_enable;
  logic<1> o_regfile_write_enable;
  logic<1> o_alu_operand_a_select;
  logic<1> o_alu_operand_b_select;
  logic<1> o_data_mem_read_enable;
  logic<1> o_data_mem_write_enable;
  logic<3> o_reg_writeback_select;
  logic<5> o_alu_function;
  logic<2> o_next_pc_select;

  //----------------------------------------

  singlecycle_control singlecycle_control;
  control_transfer control_transfer;
  alu_control alu_control;

  //----------------------------------------

  void tock_decode(logic<7> i_inst_opcode) {
    singlecycle_control.tock_decode(i_inst_opcode);
    o_pc_write_enable       = singlecycle_control.o_pc_write_enable;
    o_regfile_write_enable  = singlecycle_control.o_regfile_write_enable;
    o_alu_operand_a_select  = singlecycle_control.o_alu_operand_a_select;
    o_alu_operand_b_select  = singlecycle_control.o_alu_operand_b_select;
    o_data_mem_read_enable  = singlecycle_control.o_data_mem_read_enable;
    o_data_mem_write_enable = singlecycle_control.o_data_mem_write_enable;
    o_reg_writeback_select  = singlecycle_control.o_reg_writeback_select;
  }

  void tock_next_pc_select(logic<7> i_inst_opcode,
                           logic<3> i_inst_funct3,
                           logic<1> i_alu_result_equal_zero)
  {
    control_transfer.tock(
      i_inst_funct3,
      i_alu_result_equal_zero);

    singlecycle_control.tock_next_pc_select(
      i_inst_opcode,
      control_transfer.o_take_branch);
    o_next_pc_select        = singlecycle_control.o_next_pc_select;
  }

  void tock_alu_control(logic<3> i_inst_funct3,
                        logic<7> i_inst_funct7)
  {
    alu_control.tock(
      singlecycle_control.o_alu_op_type,
      i_inst_funct3,
      i_inst_funct7);
    o_alu_function = alu_control.o_alu_function;
  }

};
