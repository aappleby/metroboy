// RISC-V SiMPLE SV -- instruction decoder
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../metron_tools.h"
#include "config.h"
#include "constants.h"

struct instruction_decoder {
  logic<7> o_inst_opcode;
  logic<3> o_inst_funct3;
  logic<7> o_inst_funct7;
  logic<5> o_inst_rd;
  logic<5> o_inst_rs1;
  logic<5> o_inst_rs2;

  void tock(logic<32> i_inst) {
    o_inst_opcode = b7(i_inst, 0);
    o_inst_funct3 = b3(i_inst, 12);
    o_inst_funct7 = b7(i_inst, 25);
    o_inst_rd     = b5(i_inst, 7);
    o_inst_rs1    = b5(i_inst, 15);
    o_inst_rs2    = b5(i_inst, 20);
  }
};
