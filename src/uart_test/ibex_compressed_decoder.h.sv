//--------------------------------------------------------------------------------
// MODULE:       ibex_compressed_decoder
// MODULEPARAMS: 
// INPUTS:       valid_i, instr_i, 
// OUTPUTS:      is_compressed_o, illegal_instr_o, instr_o, 
// LOCALPARAMS:  
// FIELDS:       opcode_e, unused_valid, 
// SUBMODULES:   
// TASKS:        
// FUNCTIONS:    
/* verilator lint_off WIDTH */
`default_nettype none
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


`include "metron.h.sv"

module ibex_compressed_decoder
(clk, rst_n, valid_i, instr_i, is_compressed_o, illegal_instr_o, instr_o);
  /*verilator public_module*/
  
  input logic clk;
  input logic rst_n;
  input logic valid_i;
  input logic[31:0] instr_i; 

  output logic  is_compressed_o;
  output logic  illegal_instr_o;
  output logic[31:0] instr_o;

  typedef enum {
    OPCODE_LOAD = 'h03,
    OPCODE_MISC_MEM = 'h0f,
    OPCODE_OP_IMM = 'h13,
    OPCODE_AUIPC = 'h17,
    OPCODE_STORE = 'h23,
    OPCODE_OP = 'h33,
    OPCODE_LUI = 'h37,
    OPCODE_BRANCH = 'h63,
    OPCODE_JALR = 'h67,
    OPCODE_JAL = 'h6f,
    OPCODE_SYSTEM = 'h73
  } opcode_e;

  //----------------------------------------

  initial begin : init
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : tick
  end

  // valid_i indicates if instr_i is valid and is used for assertions only.
  // The following signal is used to avoid possible lint errors.
  logic unused_valid;
  //assign unused_valid = valid_i;

  ////////////////////////
  // Compressed decoder //
  ////////////////////////

  always_comb begin : tock
    // By default, forward incoming instruction, mark it as legal.
    instr_o = instr_i;
    illegal_instr_o = 1'b0;

    // Check if incoming instruction is compressed.
    case (2'(instr_i)) 
      // C0
      'b00: begin
        case (instr_i[15:13]) 
          'b000: begin
            // c.addi4spn -> addi rd', x2, imm
            instr_o = {2'd0, instr_i[10:7], instr_i[12:11], instr_i[5],
                       instr_i[6], 2'b00, 5'h02, 3'b000, 2'b01, instr_i[4:2], 7'(OPCODE_OP_IMM)};
            if (instr_i[12:5] == 8'd0)  illegal_instr_o = 1'b1;
            /*break;*/
          end
          'b010: begin
            // c.lw -> lw rd', imm(rs1')
            instr_o = {5'd0, instr_i[5], instr_i[12:10], instr_i[6],
                       2'b00, 2'b01, instr_i[9:7], 3'b010, 2'b01, instr_i[4:2], 7'(OPCODE_LOAD)};
            /*break;*/
          end
          'b110: begin
            // c.sw -> sw rs2', imm(rs1')
            instr_o = {5'd0, instr_i[5], instr_i[12], 2'b01, instr_i[4:2],
                       2'b01, instr_i[9:7], 3'b010, instr_i[11:10], instr_i[6],
                       2'b00, 7'(OPCODE_STORE)};
            /*break;*/
          end
          
          'b001,
          'b011,
          'b100,
          'b101,
          'b111: begin
            illegal_instr_o = 1'b1;
          end

          default: begin
            illegal_instr_o = 1'b1;
          end
        endcase
        /*break;*/
      end

      // C1
      //
      // Register address checks for RV32E are performed in the regular instruction decoder.
      // If this check fails, an illegal instruction exception is triggered and the controller
      // writes the actual faulting instruction to mtval.
      'b01: begin
        case (instr_i[15:13]) 
          'b000: begin
            // c.addi -> addi rd, rd, nzimm
            // c.nop
            instr_o = {{6 {instr_i[12]}}, instr_i[12], instr_i[6:2],
                       instr_i[11:7], 3'b0, instr_i[11:7], 7'(OPCODE_OP_IMM)};
            /*break;*/
          end

          'b001, 'b101: begin
            // 001: c.jal -> jal x1, imm
            // 101: c.j   -> jal x0, imm
            instr_o = {instr_i[12], instr_i[8], instr_i[10:9], instr_i[6],
                       instr_i[7], instr_i[2], instr_i[11], instr_i[5:3],
                       {9 {instr_i[12]}}, 4'b0, ~instr_i[15], 7'(OPCODE_JAL)};
            /*break;*/
          end

          'b010: begin
            // c.li -> addi rd, x0, nzimm
            // (c.li hints are translated into an addi hint)
            instr_o = {{6 {instr_i[12]}}, instr_i[12], instr_i[6:2], 5'b0,
                       3'b0, instr_i[11:7], 7'(OPCODE_OP_IMM)};
            /*break;*/
          end

          'b011: begin
            // c.lui -> lui rd, imm
            // (c.lui hints are translated into a lui hint)
            instr_o = {{15 {instr_i[12]}}, instr_i[6:2], instr_i[11:7], 7'(OPCODE_LUI)};

            if (instr_i[11:7] == 5'h02) begin
              // c.addi16sp -> addi x2, x2, nzimm
              instr_o = {{3 {instr_i[12]}}, instr_i[4:3], instr_i[5], instr_i[2],
                         instr_i[6], 4'b0, 5'h02, 3'b0, 5'h02, 7'(OPCODE_OP_IMM)};
            end

            if ({instr_i[12], instr_i[6:2]} == 6'b0) illegal_instr_o = 1'b1;
            /*break;*/
          end

          'b100: begin
            case (instr_i[11:10]) 
              'b00,
              'b01: begin
                // 00: c.srli -> srli rd, rd, shamt
                // 01: c.srai -> srai rd, rd, shamt
                // (c.srli/c.srai hints are translated into a srli/srai hint)
                instr_o = {1'b0, instr_i[10], 5'b0, instr_i[6:2], 2'b01, instr_i[9:7],
                           3'b101, 2'b01, instr_i[9:7], 7'(OPCODE_OP_IMM)};
                if (instr_i[12] == 'b1)  illegal_instr_o = 1'b1;
                /*break;*/
              end
              'b10: begin
                // c.andi -> andi rd, rd, imm
                instr_o = {{6 {instr_i[12]}}, instr_i[12], instr_i[6:2], 2'b01, instr_i[9:7],
                           3'b111, 2'b01, instr_i[9:7], 7'(OPCODE_OP_IMM)};
                /*break;*/
              end
              'b11: begin
                case ({instr_i[12], instr_i[6:5]}) 
                  'b000: begin
                    // c.sub -> sub rd', rd', rs2'
                    instr_o = {2'b01, 5'b0, 2'b01, instr_i[4:2], 2'b01, instr_i[9:7],
                               3'b000, 2'b01, instr_i[9:7], 7'(OPCODE_OP)};
                    /*break;*/
                  end
                  'b001: begin
                    // c.xor -> xor rd', rd', rs2'
                    instr_o = {7'b0, 2'b01, instr_i[4:2], 2'b01, instr_i[9:7], 3'b100,
                               2'b01, instr_i[9:7], 7'(OPCODE_OP)};
                    /*break;*/
                  end
                  'b010: begin
                    // c.or  -> or  rd', rd', rs2'
                    instr_o = {7'b0, 2'b01, instr_i[4:2], 2'b01, instr_i[9:7], 3'b110,
                               2'b01, instr_i[9:7], 7'(OPCODE_OP)};
                    /*break;*/
                  end
                  'b011: begin
                    // c.and -> and rd', rd', rs2'
                    instr_o = {7'b0, 2'b01, instr_i[4:2], 2'b01, instr_i[9:7], 3'b111,
                               2'b01, instr_i[9:7], 7'(OPCODE_OP)};
                    /*break;*/
                  end

                  'b100,
                  'b101,
                  'b110,
                  'b111: begin
                    // 100: c.subw
                    // 101: c.addw
                    illegal_instr_o = 1'b1;
                    /*break;*/
                  end

                  default: begin
                    illegal_instr_o = 1'b1;
                    /*break;*/
                  end
                endcase
                /*break;*/
              end
              default: begin
                illegal_instr_o = 1'b1;
                /*break;*/
              end
            endcase
            /*break;*/
          end

          'b110, 'b111: begin
            // 0: c.beqz -> beq rs1', x0, imm
            // 1: c.bnez -> bne rs1', x0, imm
            instr_o = {{4 {instr_i[12]}}, instr_i[6:5], instr_i[2], 5'b0, 2'b01,
                       instr_i[9:7], 2'b00, instr_i[13], instr_i[11:10], instr_i[4:3],
                       instr_i[12], 7'(OPCODE_BRANCH)};
            /*break;*/
          end

          default: begin
            illegal_instr_o = 1'b1;
            /*break;*/
          end
        endcase
        /*break;*/
      end

      // C2
      //
      // Register address checks for RV32E are performed in the regular instruction decoder.
      // If this check fails, an illegal instruction exception is triggered and the controller
      // writes the actual faulting instruction to mtval.
      'b10: begin
        case (instr_i[15:13]) 
          'b000: begin
            // c.slli -> slli rd, rd, shamt
            // (c.ssli hints are translated into a slli hint)
            instr_o = {7'b0, instr_i[6:2], instr_i[11:7], 3'b001, instr_i[11:7], 7'(OPCODE_OP_IMM)};
            if (instr_i[12] == 'b1)  illegal_instr_o = 1'b1; // reserved for custom extensions
            /*break;*/
          end

          'b010: begin
            // c.lwsp -> lw rd, imm(x2)
            instr_o = {4'b0, instr_i[3:2], instr_i[12], instr_i[6:4], 2'b00, 5'h02,
                       3'b010, instr_i[11:7], 7'(OPCODE_LOAD)};
            if (instr_i[11:7] == 5'b0)  illegal_instr_o = 1'b1;
            /*break;*/
          end

          'b100: begin
            if (instr_i[12] == 'b0) begin
              if (instr_i[6:2] != 5'b0) begin
                // c.mv -> add rd/rs1, x0, rs2
                // (c.mv hints are translated into an add hint)
                instr_o = {7'b0, instr_i[6:2], 5'b0, 3'b0, instr_i[11:7], 7'(OPCODE_OP)};
              end else begin
                // c.jr -> jalr x0, rd/rs1, 0
                instr_o = {12'b0, instr_i[11:7], 3'b0, 5'b0, 7'(OPCODE_JALR)};
                if (instr_i[11:7] == 5'b0) illegal_instr_o = 1'b1;
              end
            end else begin
              if (instr_i[6:2] != 5'b0) begin
                // c.add -> add rd, rd, rs2
                // (c.add hints are translated into an add hint)
                instr_o = {7'b0, instr_i[6:2], instr_i[11:7], 3'b0, instr_i[11:7], 7'(OPCODE_OP)};
              end else begin
                if (instr_i[11:7] == 5'b0) begin
                  // c.ebreak -> ebreak
                  instr_o = 32'h00100073;
                end else begin
                  // c.jalr -> jalr x1, rs1, 0
                  instr_o = {12'b0, instr_i[11:7], 3'b000, 5'b00001, 7'(OPCODE_JALR)};
                end
              end
            end
            /*break;*/
          end

          'b110: begin
            // c.swsp -> sw rs2, imm(x2)
            instr_o = {4'b0, instr_i[8:7], instr_i[12], instr_i[6:2], 5'h02, 3'b010,
                       instr_i[11:9], 2'b00, 7'(OPCODE_STORE)};
            /*break;*/
          end

          'b001,
          'b011,
          'b101,
          'b111: begin
            illegal_instr_o = 1'b1;
            /*break;*/
          end

          default: begin
            illegal_instr_o = 1'b1;
            /*break;*/
          end
        endcase
        /*break;*/
      end

      // Incoming instruction is not compressed.
      'b11: begin
        /*break;*/
      end

      default: begin
        illegal_instr_o = 1'b1;
      end
    endcase

    is_compressed_o = 2'(instr_i) != 'b11;
  end
endmodule

//==============================================================================
