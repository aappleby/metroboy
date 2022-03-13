// RISC-V SiMPLE SV -- register file
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_REGFILE_H
#define RVSIMPLE_REGFILE_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

class regfile {
public:

  logic<32> rs1_data;
  logic<32> rs2_data;

  // 32 registers of 32-bit width
  logic<32> regs[32];
  logic<32> reg_out_1;
  logic<32> reg_out_2;
   
  // Register x0 is always 0
  void init() {
    regs[0] = b32(0);
  }

  // Write port for rd
  void tick(logic<1>  write_enable,
            logic<5>  rd_address,
            logic<5>  rs1_address,
            logic<5>  rs2_address,
            logic<32> rd_data) {
    // Read ports for rs1 and rs2
    reg_out_1 = regs[rs1_address];
    reg_out_2 = regs[rs2_address];

    if (write_enable && rd_address != b5(0)) {
      regs[rd_address] = rd_data;
    }
  }

  void tock() {
    rs1_data = reg_out_1;
    rs2_data = reg_out_2;
  }
};

#endif // RVSIMPLE_REGFILE_H