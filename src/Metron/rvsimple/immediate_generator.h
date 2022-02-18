// RISC-V SiMPLE SV -- immediate generator
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../../../src/uart_test/metron_tools.h"
#include "../../synth/config.h"
#include "constants.h"

struct immediate_generator {
  logic<32> o_immediate;

  // Immediate format
  //       31.............30........20.19........12.11.....11.10.........5.4..........1.0.....0
  // I = { {21{inst[31]}},                                     inst[30:25], inst[24:20]         };
  // S = { {21{inst[31]}},                                     inst[30:25], inst[11:7]          };
  // B = { {20{inst[31]}}, inst[7],                            inst[30:25], inst[11:8],   1'b0  };
  // U = { {1{inst[31]}},  inst[30:20], inst[19:12],                                      12'b0 };
  // J = { {12{inst[31]}},              inst[19:12], inst[20], inst[30:25], inst[24:21],  1'b0  };
    
  void tock(logic<32> i_inst) {
    o_immediate = b32(0);
    switch (b7(i_inst)) { // Opcode
      case OPCODE_LOAD:
      case OPCODE_LOAD_FP:
      case OPCODE_OP_IMM:
      case OPCODE_JALR: {  // I-type immediate
        o_immediate = cat( dup<21>(i_inst[31]), b6(i_inst, 26), b5(i_inst, 20) );
        break;
      }
      case OPCODE_STORE_FP:
      case OPCODE_STORE:  // S-type immediate
        o_immediate = cat( dup<21>(i_inst[31]), b6(i_inst, 25), b5(i_inst, 7) );
        break;
      case OPCODE_BRANCH: // B-type immediate
        o_immediate = cat( dup<20>(i_inst[31]), i_inst[7], b6(i_inst, 25), b4(i_inst, 8), b1(0) );
        break;
      case OPCODE_AUIPC:
      case OPCODE_LUI:    // U-type immediate
        o_immediate = cat( i_inst[31], b11(i_inst, 20), b8(i_inst, 12), b12(0) );
        break;
      case OPCODE_JAL:    // J-type immediate
        o_immediate = cat( dup<12>(i_inst[31]), b8(i_inst, 12), i_inst[20], b6(i_inst, 25), b4(i_inst, 21), b1(0) );
        break;
      default:
        o_immediate = b32(0);
        break;
    }
  }
};

