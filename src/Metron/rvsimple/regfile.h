// RISC-V SiMPLE SV -- register file
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../../../src/uart_test/metron_tools.h"
#include "../../synth/config.h"
#include "constants.h"

struct regfile {

  logic<32> o_rs1_data;
  logic<32> o_rs2_data;

  // 32 registers of 32-bit width
  logic<32> regs[32];
  logic<32> reg_out_1;
  logic<32> reg_out_2;
   
  // Register x0 is always 0
  void init() {
    regs[0] = b32(0);
  }

  // Write port for rd
  void tick(logic<1>  i_write_enable,
            logic<5>  i_rd_address,
            logic<5>  i_rs1_address,
            logic<5>  i_rs2_address,
            logic<32> i_rd_data) {
    // Read ports for rs1 and rs2
    reg_out_1 = regs[i_rs1_address];
    reg_out_2 = regs[i_rs2_address];

    if (i_write_enable && i_rd_address != b5(0)) {
      regs[i_rd_address] = i_rd_data;
    }
  }

  void tock() {
    o_rs1_data = reg_out_1;
    o_rs2_data = reg_out_2;
  }

};