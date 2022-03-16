// RISC-V SiMPLE SV -- instruction decoder
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_INSTRUCTION_DECODER_H
#define RVSIMPLE_INSTRUCTION_DECODER_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

class instruction_decoder {
public:
  logic<7> inst_opcode;
  logic<3> inst_funct3;
  logic<7> inst_funct7;
  logic<5> inst_rd;
  logic<5> inst_rs1;
  logic<5> inst_rs2;

  void tock(logic<32> inst) {
    inst_opcode = b7(inst, 0);
    inst_funct3 = b3(inst, 12);
    inst_funct7 = b7(inst, 25);
    inst_rd     = b5(inst, 7);
    inst_rs1    = b5(inst, 15);
    inst_rs2    = b5(inst, 20);
  }
};

#endif // RVSIMPLE_INSTRUCTION_DECODER_H