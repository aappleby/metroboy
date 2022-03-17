// RISC-V SiMPLE SV -- ALU controller module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_ALU_CONTROL_H
#define RVSIMPLE_ALU_CONTROL_H

#include "config.h"
#include "constants.h"
#include "metron_tools.h"

class alu_control {
 public:
  logic<5> alu_function;

  void tock(logic<2> alu_op_type, logic<3> inst_funct3, logic<7> inst_funct7) {
    using namespace rv_constants;

    logic<5> default_funct;
    switch (inst_funct3) {
      case FUNCT3_ALU_ADD_SUB:
        default_funct = ALU_ADD;
        break;
      case FUNCT3_ALU_SLL:
        default_funct = ALU_SLL;
        break;
      case FUNCT3_ALU_SLT:
        default_funct = ALU_SLT;
        break;
      case FUNCT3_ALU_SLTU:
        default_funct = ALU_SLTU;
        break;
      case FUNCT3_ALU_XOR:
        default_funct = ALU_XOR;
        break;
      case FUNCT3_ALU_SHIFTR:
        default_funct = ALU_SRL;
        break;
      case FUNCT3_ALU_OR:
        default_funct = ALU_OR;
        break;
      case FUNCT3_ALU_AND:
        default_funct = ALU_AND;
        break;
      default:
        default_funct = b5(DONTCARE);
        break;
    }

    logic<5> secondary_funct;
    switch (inst_funct3) {
      case FUNCT3_ALU_ADD_SUB:
        secondary_funct = ALU_SUB;
        break;
      case FUNCT3_ALU_SHIFTR:
        secondary_funct = ALU_SRA;
        break;
      default:
        secondary_funct = b5(DONTCARE);
        break;
    }

    logic<5> op_funct;
    if (inst_funct7[5])
      op_funct = secondary_funct;
    else
      op_funct = default_funct;

    logic<5> op_imm_funct;
    if (inst_funct7[5] && b2(inst_funct3) == b2(0b01))
      op_imm_funct = secondary_funct;
    else
      op_imm_funct = default_funct;

    logic<5> branch_funct;
    switch (inst_funct3) {
      case FUNCT3_BRANCH_EQ:
        branch_funct = ALU_SEQ;
        break;
      case FUNCT3_BRANCH_NE:
        branch_funct = ALU_SEQ;
        break;
      case FUNCT3_BRANCH_LT:
        branch_funct = ALU_SLT;
        break;
      case FUNCT3_BRANCH_GE:
        branch_funct = ALU_SLT;
        break;
      case FUNCT3_BRANCH_LTU:
        branch_funct = ALU_SLTU;
        break;
      case FUNCT3_BRANCH_GEU:
        branch_funct = ALU_SLTU;
        break;
      default:
        branch_funct = b5(DONTCARE);
        break;
    }

    switch (alu_op_type) {
      case CTL_ALU_ADD:
        alu_function = ALU_ADD;
        break;
      case CTL_ALU_OP:
        alu_function = op_funct;
        break;
      case CTL_ALU_OP_IMM:
        alu_function = op_imm_funct;
        break;
      case CTL_ALU_BRANCH:
        alu_function = branch_funct;
        break;
      default:
        alu_function = b5(DONTCARE);
        break;
    }
  }
};

#endif  // RVSIMPLE_ALU_CONTROL_H