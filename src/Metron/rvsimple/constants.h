// RISC-V SiMPLE SV -- constants
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#ifndef RV_CONSTANTS
#define RV_CONSTANTS

//////////////////////////////////////////
//              Constants               //
//////////////////////////////////////////

#define ON              0b1
#define OFF             0b0
#define ZERO  0b00000000000000000000000000000000

// Instruction opcodes
#define OPCODE_LOAD      0b0000011
#define OPCODE_LOAD_FP   0b0000111
#define OPCODE_MISC_MEM  0b0001111
#define OPCODE_OP_IMM    0b0010011
#define OPCODE_AUIPC     0b0010111
#define OPCODE_STORE     0b0100011
#define OPCODE_STORE_FP  0b0100111
#define OPCODE_OP        0b0110011
#define OPCODE_LUI       0b0110111
#define OPCODE_OP_FP     0b1010011
#define OPCODE_BRANCH    0b1100011
#define OPCODE_JALR      0b1100111
#define OPCODE_JAL       0b1101111
#define OPCODE_SYSTEM    0b1110011

// Interpretations of the "funct3" field
#define FUNCT3_ALU_ADD_SUB  0b000
#define FUNCT3_ALU_SLL      0b001
#define FUNCT3_ALU_SLT      0b010
#define FUNCT3_ALU_SLTU     0b011
#define FUNCT3_ALU_XOR      0b100
#define FUNCT3_ALU_SHIFTR   0b101
#define FUNCT3_ALU_OR       0b110
#define FUNCT3_ALU_AND      0b111

// Interpretations of the "funct3" field for extension M
#define FUNCT3_ALU_MUL      0b000
#define FUNCT3_ALU_MULH     0b001
#define FUNCT3_ALU_MULHSU   0b010
#define FUNCT3_ALU_MULHU    0b011
#define FUNCT3_ALU_DIV      0b100
#define FUNCT3_ALU_DIVU     0b101
#define FUNCT3_ALU_REM      0b110
#define FUNCT3_ALU_REMU     0b111

// Interpretations of the "funct7" field for extension F
#define FUNCT7_FPALU_ADD     0b0000000
#define FUNCT7_FPALU_SUB     0b0000100
#define FUNCT7_FPALU_MUL     0b0001000
#define FUNCT7_FPALU_DIV     0b0001100
#define FUNCT7_FPALU_SQRT    0b0101100
#define FUNCT7_FPALU_SIGN    0b0010000
#define FUNCT7_FPALU_MINMAX  0b0010100
#define FUNCT7_FPALU_CVT_W   0b1100000
#define FUNCT7_FPALU_MV_X    0b1110000
#define FUNCT7_FPALU_COMP    0b1010000
#define FUNCT7_FPALU_CLASS   0b1110000
#define FUNCT7_FPALU_CVT_S   0b1101000
#define FUNCT7_FPALU_MV_W    0b1111000

// Interpretations of the "funct3" field for extension F (rounding modes)
#define FUNCT3_ROUND_RNE     0b000
#define FUNCT3_ROUND_RTZ     0b001
#define FUNCT3_ROUND_RDN     0b010
#define FUNCT3_ROUND_RUP     0b011
#define FUNCT3_ROUND_RMM     0b100
#define FUNCT3_ROUND_DYN     0b111

// Interpretations of the "funct3" field for loads and stores
#define FUNCT3_MEM_BYTE      0b000
#define FUNCT3_MEM_HALF      0b001
#define FUNCT3_MEM_WORD      0b010
#define FUNCT3_MEM_BYTE_U    0b100
#define FUNCT3_MEM_HALF_U    0b101

// Interpretations of the "funct3" field for branches
#define FUNCT3_BRANCH_EQ     0b000
#define FUNCT3_BRANCH_NE     0b001
#define FUNCT3_BRANCH_LT     0b100
#define FUNCT3_BRANCH_GE     0b101
#define FUNCT3_BRANCH_LTU    0b110
#define FUNCT3_BRANCH_GEU    0b111

// Interpretations of the "funct3" field for system opcode
#define FUNCT3_SYSTEM_ENV     0b000
#define FUNCT3_SYSTEM_CSRRW   0b001
#define FUNCT3_SYSTEM_CSRRS   0b010
#define FUNCT3_SYSTEM_CSRRC   0b011
#define FUNCT3_SYSTEM_CSRRWI  0b101
#define FUNCT3_SYSTEM_CSRRSS  0b110
#define FUNCT3_SYSTEM_CSRRCI  0b111

// ALU operations
#define ALU_ADD     0b00001
#define ALU_SUB     0b00010
#define ALU_SLL     0b00011
#define ALU_SRL     0b00100
#define ALU_SRA     0b00101
#define ALU_SEQ     0b00110
#define ALU_SLT     0b00111
#define ALU_SLTU    0b01000
#define ALU_XOR     0b01001
#define ALU_OR      0b01010
#define ALU_AND     0b01011
#define ALU_MUL     0b01100
#define ALU_MULH    0b01101
#define ALU_MULHSU  0b01110
#define ALU_MULHU   0b01111
#define ALU_DIV     0b10000
#define ALU_DIVU    0b10001
#define ALU_REM     0b10010
#define ALU_REMU    0b10011

// ALU op types
#define CTL_ALU_ADD     0b00
#define CTL_ALU_BRANCH  0b01
#define CTL_ALU_OP      0b10
#define CTL_ALU_OP_IMM  0b11

// Register data sources
#define CTL_WRITEBACK_ALU   0b000
#define CTL_WRITEBACK_DATA  0b001
#define CTL_WRITEBACK_PC4   0b010
#define CTL_WRITEBACK_IMM   0b011

// ALU 1st operand source
#define CTL_ALU_A_RS1  0b0
#define CTL_ALU_A_PC   0b1

// ALU 2nd operand source
#define CTL_ALU_B_RS2  0b0
#define CTL_ALU_B_IMM  0b1

// PC source
#define CTL_PC_PC4      0b00
#define CTL_PC_PC_IMM   0b01
#define CTL_PC_RS1_IMM  0b10
#define CTL_PC_PC4_BR   0b11

// PC source in multicycle
#define MC_CTL_PC_ALU_RES  0b0
#define MC_CTL_PC_ALU_OUT  0b1

// ALU 2nd operand source in multicycle
#define MC_CTL_ALU_B_RS2  0b00
#define MC_CTL_ALU_B_IMM  0b01
#define MC_CTL_ALU_B_4    0b10

#endif

