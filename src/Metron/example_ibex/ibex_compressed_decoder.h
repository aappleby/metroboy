// Copyright lowRISC contributors.
// Copyright 2018 ETH Zurich and University of Bologna, see also CREDITS.md.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

/**
 * Compressed instruction decoder
 *
 * Decodes RISC-V compressed instructions into their RV32 equivalent.
 * This module is fully combinatorial, clock and reset are used for
 * assertions only.
 */

#pragma once
#include "metron_tools.h"
#include "ibex_pkg.h"

struct ibex_compressed_decoder {
  logic<1>  is_compressed_o;
  logic<1>  illegal_instr_o;
  logic<32> instr_o;

  // valid_i indicates if instr_i is valid and is used for assertions only.
  // The following signal is used to avoid possible lint errors.
  logic<1> unused_valid;
  void tock2(bool rst_n, logic<1> valid_i) {
    unused_valid = valid_i;
  }
  
  ////////////////////////
  // Compressed decoder //
  ////////////////////////

  void tock(bool rst_n, logic<1> valid_i, logic<32> instr_i) {
    using namespace ibex_pkg;

    // By default, forward incoming instruction, mark it as legal.
    instr_o = instr_i;
    illegal_instr_o = b1(0b0);

    // Check if incoming instruction is compressed.
    switch (b2(instr_i)) {
      // C0
      case 0b00: {
        switch (b3(instr_i, 13)) {
          case 0b000: {
            // c.addi4spn -> addi rd', x2, imm
            instr_o = cat(b2(0b0), b4(instr_i, 7), b2(instr_i, 11), instr_i[5],
                       instr_i[6], b2(0b00), b5(0x02), b3(0b000), b2(0b01), b3(instr_i, 2), b7(opcode_e::OPCODE_OP_IMM));
            if (b8(instr_i, 5) == b8(0b0))  illegal_instr_o = b1(0b1);
            break;
          }

          case 0b010: {
            // c.lw -> lw rd', imm(rs1')
            instr_o = cat(b5(0b0), instr_i[5], b3(instr_i, 10), instr_i[6],
                       b2(0b00), b2(0b01), b3(instr_i, 7), b3(0b010), b2(0b01), b3(instr_i, 2), b7(opcode_e::OPCODE_LOAD));
            break;
          }

          case 0b110: {
            // c.sw -> sw rs2', imm(rs1')
            instr_o = cat(b5(0b0), instr_i[5], instr_i[12], b2(0b01), b3(instr_i, 2),
                       b2(0b01), b3(instr_i, 7), b3(0b010), b2(instr_i, 10), instr_i[6],
                       b2(0b00), b7(opcode_e::OPCODE_STORE));
            break;
          }
          
          case 0b001:
          case 0b011:
          case 0b100:
          case 0b101:
          case 0b111: {
            illegal_instr_o = b1(0b1);
          }

          default: {
            illegal_instr_o = b1(0b1);
          }
        }
        break;
      }

      // C1
      //
      // Register address checks for RV32E are performed in the regular instruction decoder.
      // If this check fails, an illegal instruction exception is triggered and the controller
      // writes the actual faulting instruction to mtval.
      case 0b01: {
        switch (b3(instr_i, 13)) {
          case 0b000: {
            // c.addi -> addi rd, rd, nzimm
            // c.nop
            instr_o = cat(dup<6>(instr_i[12]), instr_i[12], b5(instr_i, 2),
                       b5(instr_i, 7), b3(0b0), b5(instr_i, 7), b7(opcode_e::OPCODE_OP_IMM));
            break;
          }

          case 0b001: case 0b101: {
            // 001: c.jal -> jal x1, imm
            // 101: c.j   -> jal x0, imm
            instr_o = cat(instr_i[12], instr_i[8], b2(instr_i, 9), instr_i[6],
                          instr_i[7], instr_i[2], instr_i[11], b3(instr_i, 3),
                          dup<9>(instr_i[12]), b4(0b0), ~instr_i[15], b7(opcode_e::OPCODE_JAL));
            break;
          }

          case 0b010: {
            // c.li -> addi rd, x0, nzimm
            // (c.li hints are translated into an addi hint)
            instr_o = cat(dup<6>(instr_i[12]), instr_i[12], b5(instr_i, 2), b5(0b0),
                       b3(0b0), b5(instr_i, 7), b7(opcode_e::OPCODE_OP_IMM));
            break;
          }

          case 0b011: {
            // c.lui -> lui rd, imm
            // (c.lui hints are translated into a lui hint)
            instr_o = cat(dup<15>(instr_i[12]), b5(instr_i, 2), b5(instr_i, 7), b7(opcode_e::OPCODE_LUI));

            if (b5(instr_i, 7) == b5(0x02)) {
              // c.addi16sp -> addi x2, x2, nzimm
              instr_o = cat(dup<3>(instr_i[12]), b2(instr_i, 3), instr_i[5], instr_i[2],
                         instr_i[6], b4(0b0), b5(0x02), b3(0b000), b5(0x02), b7(opcode_e::OPCODE_OP_IMM));
            }

            if (cat(instr_i[12], b5(instr_i, 2)) == b6(0b0)) illegal_instr_o = b1(0b1);
            break;
          }

          case 0b100: {
            switch (b2(instr_i, 10)) {
              case 0b00:
              case 0b01: {
                // 00: c.srli -> srli rd, rd, shamt
                // 01: c.srai -> srai rd, rd, shamt
                // (c.srli/c.srai hints are translated into a srli/srai hint)
                instr_o = cat(b1(0b0), instr_i[10], b5(0b0), b5(instr_i, 2), b2(0b01), b3(instr_i, 7),
                           b3(0b101), b2(0b01), b3(instr_i, 7), b7(opcode_e::OPCODE_OP_IMM));
                if (instr_i[12] == 0b1)  illegal_instr_o = b1(0b1);
                break;
              }

              case 0b10: {
                // c.andi -> andi rd, rd, imm
                instr_o = cat(dup<6>(instr_i[12]), instr_i[12], b5(instr_i, 2), b2(0b01), b3(instr_i, 7),
                           b3(0b111), b2(0b01), b3(instr_i, 7), b7(opcode_e::OPCODE_OP_IMM));
                break;
              }

              case 0b11: {
                switch (cat(instr_i[12], b2(instr_i, 5))) {
                  case 0b000: {
                    // c.sub -> sub rd', rd', rs2'
                    instr_o = cat(b2(0b01), b5(0b0), b2(0b01), b3(instr_i, 2), b2(0b01), b3(instr_i, 7),
                               b3(0b000), b2(0b01), b3(instr_i, 7), b7(opcode_e::OPCODE_OP));
                    break;
                  }

                  case 0b001: {
                    // c.xor -> xor rd', rd', rs2'
                    instr_o = cat(b7(0b0), b2(0b01), b3(instr_i, 2), b2(0b01), b3(instr_i, 7), b3(0b100),
                               b2(0b01), b3(instr_i, 7), b7(opcode_e::OPCODE_OP));
                    break;
                  }

                  case 0b010: {
                    // c.or  -> or  rd', rd', rs2'
                    instr_o = cat(b7(0b0), b2(0b01), b3(instr_i, 2), b2(0b01), b3(instr_i, 7), b3(0b110),
                               b2(0b01), b3(instr_i, 7), b7(opcode_e::OPCODE_OP));
                    break;
                  }

                  case 0b011: {
                    // c.and -> and rd', rd', rs2'
                    instr_o = cat(b7(0b0), b2(0b01), b3(instr_i, 2), b2(0b01), b3(instr_i, 7), b3(0b111),
                               b2(0b01), b3(instr_i, 7), b7(opcode_e::OPCODE_OP));
                    break;
                  }

                  case 0b100:
                  case 0b101:
                  case 0b110:
                  case 0b111: {
                    // 100: c.subw
                    // 101: c.addw
                    illegal_instr_o = b1(0b1);
                    break;
                  }

                  default: {
                    illegal_instr_o = b1(0b1);
                    break;
                  }
                }
                break;
              }

              default: {
                illegal_instr_o = b1(0b1);
                break;
              }
            }
            break;
          }

          case 0b110: case 0b111: {
            // 0: c.beqz -> beq rs1', x0, imm
            // 1: c.bnez -> bne rs1', x0, imm
            instr_o = cat(dup<4>(instr_i[12]), b2(instr_i, 5), instr_i[2], b5(0b0), b2(0b01),
                       b3(instr_i, 7), b2(0b00), instr_i[13], b2(instr_i, 10), b2(instr_i, 3),
                       instr_i[12], b7(opcode_e::OPCODE_BRANCH));
            break;
          }

          default: {
            illegal_instr_o = b1(0b1);
            break;
          }
        }
        break;
      }

      // C2
      //
      // Register address checks for RV32E are performed in the regular instruction decoder.
      // If this check fails, an illegal instruction exception is triggered and the controller
      // writes the actual faulting instruction to mtval.
      case 0b10: {
        switch (b3(instr_i, 13)) {
          case 0b000: {
            // c.slli -> slli rd, rd, shamt
            // (c.ssli hints are translated into a slli hint)
            instr_o = cat(b7(0b0), b5(instr_i, 2), b5(instr_i, 7), b3(0b001), b5(instr_i, 7), b7(opcode_e::OPCODE_OP_IMM));
            if (instr_i[12] == 0b1)  illegal_instr_o = b1(0b1); // reserved for custom extensions
            break;
          }

          case 0b010: {
            // c.lwsp -> lw rd, imm(x2)
            instr_o = cat(b4(0b0), b2(instr_i, 2), instr_i[12], b3(instr_i, 4), b2(0b00), b5(0x02),
                       b3(0b010), b5(instr_i, 7), b7(opcode_e::OPCODE_LOAD));
            if (b5(instr_i, 7) == b5(0b0))  illegal_instr_o = b1(0b1);
            break;
          }

          case 0b100: {
            if (instr_i[12] == 0b0) {
              if (b5(instr_i, 2) != b5(0b0)) {
                // c.mv -> add rd/rs1, x0, rs2
                // (c.mv hints are translated into an add hint)
                instr_o = cat(b7(0b0), b5(instr_i, 2), b5(0b0), b3(0b0), b5(instr_i, 7), b7(opcode_e::OPCODE_OP));
              } else {
                // c.jr -> jalr x0, rd/rs1, 0
                instr_o = cat(b12(0b0), b5(instr_i, 7), b3(0b0), b5(0b0), b7(opcode_e::OPCODE_JALR));
                if (b5(instr_i, 7) == b5(0b0)) illegal_instr_o = b1(0b1);
              }
            } else {
              if (b5(instr_i, 2) != b5(0b0)) {
                // c.add -> add rd, rd, rs2
                // (c.add hints are translated into an add hint)
                instr_o = cat(b7(0b0), b5(instr_i, 2), b5(instr_i, 7), b3(0b0), b5(instr_i, 7), b7(opcode_e::OPCODE_OP));
              } else {
                if (b5(instr_i, 7) == b5(0b0)) {
                  // c.ebreak -> ebreak
                  instr_o = b32(0x00'10'00'73);
                } else {
                  // c.jalr -> jalr x1, rs1, 0
                  instr_o = cat(b12(0b0), b5(instr_i, 7), b3(0b000), b5(0b00001), b7(opcode_e::OPCODE_JALR));
                }
              }
            }
            break;
          }

          case 0b110: {
            // c.swsp -> sw rs2, imm(x2)
            instr_o = cat(b4(0b0), b2(instr_i, 7), instr_i[12], b5(instr_i, 2), b5(0x02), b3(0b010),
                       b3(instr_i, 9), b2(0b00), b7(opcode_e::OPCODE_STORE));
            break;
          }

          case 0b001:
          case 0b011:
          case 0b101:
          case 0b111: {
            illegal_instr_o = b1(0b1);
            break;
          }

          default: {
            illegal_instr_o = b1(0b1);
            break;
          }
        }
        break;
      }

      // Incoming instruction is not compressed.
      case 0b11:; break;

      default: {
        illegal_instr_o = b1(0b1);
      }
    }
  }

  void tock3(logic<32> instr_i) {
    is_compressed_o = (b2(instr_i) != 0b11);
  }
  /*#
  ////////////////
  // Assertions //
  ////////////////

  // The valid_i signal used to gate below assertions must be known.
  `ASSERT_KNOWN(IbexInstrValidKnown, valid_i)

  // Selectors must be known/valid.
  `ASSERT(IbexInstrLSBsKnown, valid_i |->
      !$isunknown(instr_i[1:0]))
  `ASSERT(IbexC0Known1, (valid_i && (instr_i[1:0] == 2'b00)) |->
      !$isunknown(instr_i[15:13]))
  `ASSERT(IbexC1Known1, (valid_i && (instr_i[1:0] == 2'b01)) |->
      !$isunknown(instr_i[15:13]))
  `ASSERT(IbexC1Known2, (valid_i && (instr_i[1:0] == 2'b01) && (instr_i[15:13] == 3'b100)) |->
      !$isunknown(instr_i[11:10]))
  `ASSERT(IbexC1Known3, (valid_i &&
      (instr_i[1:0] == 2'b01) && (instr_i[15:13] == 3'b100) && (instr_i[11:10] == 2'b11)) |->
      !$isunknown({instr_i[12], instr_i[6:5]}))
  `ASSERT(IbexC2Known1, (valid_i && (instr_i[1:0] == 2'b10)) |->
      !$isunknown(instr_i[15:13]))
   #*/
};
