// RISC-V SiMPLE SV -- ALU controller module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../../../src/Metron/top.h"
#include "../../synth/config.h"
#include "constants.h"

struct alu_control {
  logic<5> o_alu_function;

  void tock(
    logic<2> i_alu_op_type,
    logic<3> i_inst_funct3,
    logic<7> i_inst_funct7) {

    logic<5> default_funct;
    switch (i_inst_funct3) {
      case FUNCT3_ALU_ADD_SUB: default_funct = ALU_ADD; break;
      case FUNCT3_ALU_SLL:     default_funct = ALU_SLL; break;
      case FUNCT3_ALU_SLT:     default_funct = ALU_SLT; break;
      case FUNCT3_ALU_SLTU:    default_funct = ALU_SLTU; break;
      case FUNCT3_ALU_XOR:     default_funct = ALU_XOR; break;
      case FUNCT3_ALU_SHIFTR:  default_funct = ALU_SRL; break;
      case FUNCT3_ALU_OR:      default_funct = ALU_OR; break;
      case FUNCT3_ALU_AND:     default_funct = ALU_AND; break;
      default:                 default_funct = b5(DONTCARE); break;
    }

    logic<5> secondary_funct;
    switch (i_inst_funct3) {
      case FUNCT3_ALU_ADD_SUB: secondary_funct = ALU_SUB; break;
      case FUNCT3_ALU_SHIFTR:  secondary_funct = ALU_SRA; break;
      default:                 secondary_funct = b5(DONTCARE); break;
    }
   
    logic<5> op_funct;
#ifdef M_MODULE
    logic<5> m_extension_funct;
    switch (inst_funct3) {
      case FUNCT3_ALU_MUL:    m_extension_funct = ALU_MUL; break;
      case FUNCT3_ALU_MULH:   m_extension_funct = ALU_MULH; break;
      case FUNCT3_ALU_MULHSU: m_extension_funct = ALU_MULHSU; break;
      case FUNCT3_ALU_MULHU:  m_extension_funct = ALU_MULHU; break;
      case FUNCT3_ALU_DIV:    m_extension_funct = ALU_DIV; break;
      case FUNCT3_ALU_DIVU:   m_extension_funct = ALU_DIVU; break;
      case FUNCT3_ALU_REM:    m_extension_funct = ALU_REM; break;
      case FUNCT3_ALU_REMU:   m_extension_funct = ALU_REMU; break;
      default:                m_extension_funct = b5(DONTCARE); break;
    }

    if (i_inst_funct7[5])   op_funct = secondary_funct;
    else if(inst_funct7[0]) op_funct = extension_funct;
    else                    op_funct = default_funct;
#else
    if (i_inst_funct7[5])   op_funct = secondary_funct;
    else                    op_funct = default_funct;
#endif

    logic<5> op_imm_funct;
    if (i_inst_funct7[5] && b2(i_inst_funct3) == b2(0b01))
      op_imm_funct = secondary_funct;
    else
      op_imm_funct = default_funct;

    logic<5> branch_funct;
    switch (i_inst_funct3) {
      case FUNCT3_BRANCH_EQ:   branch_funct = ALU_SEQ; break;
      case FUNCT3_BRANCH_NE:   branch_funct = ALU_SEQ; break;
      case FUNCT3_BRANCH_LT:   branch_funct = ALU_SLT; break;
      case FUNCT3_BRANCH_GE:   branch_funct = ALU_SLT; break;
      case FUNCT3_BRANCH_LTU:  branch_funct = ALU_SLTU; break;
      case FUNCT3_BRANCH_GEU:  branch_funct = ALU_SLTU; break;
      default:                 branch_funct = b5(DONTCARE); break;
    }
    
    switch (i_alu_op_type) {
      case CTL_ALU_ADD:       o_alu_function = ALU_ADD; break;
      case CTL_ALU_OP:        o_alu_function = op_funct; break;
      case CTL_ALU_OP_IMM:    o_alu_function = op_imm_funct; break;
      case CTL_ALU_BRANCH:    o_alu_function = branch_funct; break;
      default:                o_alu_function = b5(DONTCARE); break;
    }
  }
};

