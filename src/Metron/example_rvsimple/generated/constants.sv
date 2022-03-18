// RISC-V SiMPLE SV -- constants
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_CONSTANTS_H
`define RVSIMPLE_CONSTANTS_H

`include "metron_tools.sv"

package rv_constants;

//////////////////////////////////////////
//              Constants               //
//////////////////////////////////////////

//#define ON              0b1
//#define OFF             0b0
parameter int ZERO = 32'b00000000000000000000000000000000;

// Instruction opcodes
parameter int OPCODE_LOAD = 7'b0000011;
parameter int OPCODE_LOAD_FP = 7'b0000111;
parameter int OPCODE_MISC_MEM = 7'b0001111;
parameter int OPCODE_OP_IMM = 7'b0010011;
parameter int OPCODE_AUIPC = 7'b0010111;
parameter int OPCODE_STORE = 7'b0100011;
parameter int OPCODE_STORE_FP = 7'b0100111;
parameter int OPCODE_OP = 7'b0110011;
parameter int OPCODE_LUI = 7'b0110111;
parameter int OPCODE_OP_FP = 7'b1010011;
parameter int OPCODE_BRANCH = 7'b1100011;
parameter int OPCODE_JALR = 7'b1100111;
parameter int OPCODE_JAL = 7'b1101111;
parameter int OPCODE_SYSTEM = 7'b1110011;

// Interpretations of the "funct3" field
parameter int FUNCT3_ALU_ADD_SUB = 3'b000;
parameter int FUNCT3_ALU_SLL = 3'b001;
parameter int FUNCT3_ALU_SLT = 3'b010;
parameter int FUNCT3_ALU_SLTU = 3'b011;
parameter int FUNCT3_ALU_XOR = 3'b100;
parameter int FUNCT3_ALU_SHIFTR = 3'b101;
parameter int FUNCT3_ALU_OR = 3'b110;
parameter int FUNCT3_ALU_AND = 3'b111;

// Interpretations of the "funct3" field for extension M
parameter int FUNCT3_ALU_MUL = 3'b000;
parameter int FUNCT3_ALU_MULH = 3'b001;
parameter int FUNCT3_ALU_MULHSU = 3'b010;
parameter int FUNCT3_ALU_MULHU = 3'b011;
parameter int FUNCT3_ALU_DIV = 3'b100;
parameter int FUNCT3_ALU_DIVU = 3'b101;
parameter int FUNCT3_ALU_REM = 3'b110;
parameter int FUNCT3_ALU_REMU = 3'b111;

// Interpretations of the "funct7" field for extension F
parameter int FUNCT7_FPALU_ADD = 7'b0000000;
parameter int FUNCT7_FPALU_SUB = 7'b0000100;
parameter int FUNCT7_FPALU_MUL = 7'b0001000;
parameter int FUNCT7_FPALU_DIV = 7'b0001100;
parameter int FUNCT7_FPALU_SQRT = 7'b0101100;
parameter int FUNCT7_FPALU_SIGN = 7'b0010000;
parameter int FUNCT7_FPALU_MINMAX = 7'b0010100;
parameter int FUNCT7_FPALU_CVT_W = 7'b1100000;
parameter int FUNCT7_FPALU_MV_X = 7'b1110000;
parameter int FUNCT7_FPALU_COMP = 7'b1010000;
parameter int FUNCT7_FPALU_CLASS = 7'b1110000;
parameter int FUNCT7_FPALU_CVT_S = 7'b1101000;
parameter int FUNCT7_FPALU_MV_W = 7'b1111000;

// Interpretations of the "funct3" field for extension F (rounding modes)
parameter int FUNCT3_ROUND_RNE = 3'b000;
parameter int FUNCT3_ROUND_RTZ = 3'b001;
parameter int FUNCT3_ROUND_RDN = 3'b010;
parameter int FUNCT3_ROUND_RUP = 3'b011;
parameter int FUNCT3_ROUND_RMM = 3'b100;
parameter int FUNCT3_ROUND_DYN = 3'b111;

// Interpretations of the "funct3" field for loads and stores
parameter int FUNCT3_MEM_BYTE = 3'b000;
parameter int FUNCT3_MEM_HALF = 3'b001;
parameter int FUNCT3_MEM_WORD = 3'b010;
parameter int FUNCT3_MEM_BYTE_U = 3'b100;
parameter int FUNCT3_MEM_HALF_U = 3'b101;

// Interpretations of the "funct3" field for branches
parameter int FUNCT3_BRANCH_EQ = 3'b000;
parameter int FUNCT3_BRANCH_NE = 3'b001;
parameter int FUNCT3_BRANCH_LT = 3'b100;
parameter int FUNCT3_BRANCH_GE = 3'b101;
parameter int FUNCT3_BRANCH_LTU = 3'b110;
parameter int FUNCT3_BRANCH_GEU = 3'b111;

// Interpretations of the "funct3" field for system opcode
parameter int FUNCT3_SYSTEM_ENV = 3'b000;
parameter int FUNCT3_SYSTEM_CSRRW = 3'b001;
parameter int FUNCT3_SYSTEM_CSRRS = 3'b010;
parameter int FUNCT3_SYSTEM_CSRRC = 3'b011;
parameter int FUNCT3_SYSTEM_CSRRWI = 3'b101;
parameter int FUNCT3_SYSTEM_CSRRSS = 3'b110;
parameter int FUNCT3_SYSTEM_CSRRCI = 3'b111;

// ALU operations
parameter int ALU_ADD = 5'b00001;
parameter int ALU_SUB = 5'b00010;
parameter int ALU_SLL = 5'b00011;
parameter int ALU_SRL = 5'b00100;
parameter int ALU_SRA = 5'b00101;
parameter int ALU_SEQ = 5'b00110;
parameter int ALU_SLT = 5'b00111;
parameter int ALU_SLTU = 5'b01000;
parameter int ALU_XOR = 5'b01001;
parameter int ALU_OR = 5'b01010;
parameter int ALU_AND = 5'b01011;
parameter int ALU_MUL = 5'b01100;
parameter int ALU_MULH = 5'b01101;
parameter int ALU_MULHSU = 5'b01110;
parameter int ALU_MULHU = 5'b01111;
parameter int ALU_DIV = 5'b10000;
parameter int ALU_DIVU = 5'b10001;
parameter int ALU_REM = 5'b10010;
parameter int ALU_REMU = 5'b10011;

// ALU op types
parameter int CTL_ALU_ADD = 2'b00;
parameter int CTL_ALU_BRANCH = 2'b01;
parameter int CTL_ALU_OP = 2'b10;
parameter int CTL_ALU_OP_IMM = 2'b11;

// Register data sources
parameter int CTL_WRITEBACK_ALU = 3'b000;
parameter int CTL_WRITEBACK_DATA = 3'b001;
parameter int CTL_WRITEBACK_PC4 = 3'b010;
parameter int CTL_WRITEBACK_IMM = 3'b011;

// ALU 1st operand source
parameter int CTL_ALU_A_RS1 = 1'b0;
parameter int CTL_ALU_A_PC = 1'b1;

// ALU 2nd operand source
parameter int CTL_ALU_B_RS2 = 1'b0;
parameter int CTL_ALU_B_IMM = 1'b1;

// PC source
parameter int CTL_PC_PC4 = 2'b00;
parameter int CTL_PC_PC_IMM = 2'b01;
parameter int CTL_PC_RS1_IMM = 2'b10;
parameter int CTL_PC_PC4_BR = 2'b11;

// PC source in multicycle
parameter int MC_CTL_PC_ALU_RES = 1'b0;
parameter int MC_CTL_PC_ALU_OUT = 1'b1;

// ALU 2nd operand source in multicycle
parameter int MC_CTL_ALU_B_RS2 = 2'b00;
parameter int MC_CTL_ALU_B_IMM = 2'b01;
parameter int MC_CTL_ALU_B_4 = 2'b10;

endpackage;  // namespace rv_constants

`endif  // RVSIMPLE_CONSTANTS_H
