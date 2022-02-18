// Copyright lowRISC contributors.
// Copyright 2017 ETH Zurich and University of Bologna, see also CREDITS.md.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include "metron_tools.h"

/**
 * Package with constants used by Ibex
 */
namespace ibex_pkg {

  ////////////////
  // IO Structs //
  ////////////////

  struct crash_dump_t {
    logic<32> current_pc;
    logic<32> next_pc;
    logic<32> last_data_addr;
    logic<32> exception_addr;
  };

  struct core2rf_t {
    logic<1> dummy_instr_id;
    logic<5> raddr_a;
    logic<5> waddr_a;
    logic<1> we_a;
    logic<5> raddr_b;
  };

  /////////////////////
  // Parameter Enums //
  /////////////////////

  enum class regfile_e : int {
    RegFileFF    = 0,
    RegFileFPGA  = 1,
    RegFileLatch = 2
  };

  enum class rv32m_e : int {
    RV32MNone        = 0,
    RV32MSlow        = 1,
    RV32MFast        = 2,
    RV32MSingleCycle = 3
  };

  enum class rv32b_e : int {
    RV32BNone       = 0,
    RV32BBalanced   = 1,
    RV32BOTEarlGrey = 2,
    RV32BFull       = 3
  };

  /////////////
  // Opcodes //
  /////////////

  enum class opcode_e : logic<7>::BASE {
    OPCODE_LOAD     = 0x03,
    OPCODE_MISC_MEM = 0x0f,
    OPCODE_OP_IMM   = 0x13,
    OPCODE_AUIPC    = 0x17,
    OPCODE_STORE    = 0x23,
    OPCODE_OP       = 0x33,
    OPCODE_LUI      = 0x37,
    OPCODE_BRANCH   = 0x63,
    OPCODE_JALR     = 0x67,
    OPCODE_JAL      = 0x6f,
    OPCODE_SYSTEM   = 0x73
  };


  ////////////////////
  // ALU operations //
  ////////////////////

  enum class alu_op_e : logic<7>::BASE {
    // Arithmetics
    ALU_ADD,
    ALU_SUB,

    // Logics
    ALU_XOR,
    ALU_OR,
    ALU_AND,
    // RV32B
    ALU_XNOR,
    ALU_ORN,
    ALU_ANDN,

    // Shifts
    ALU_SRA,
    ALU_SRL,
    ALU_SLL,
    // RV32B
    ALU_SRO,
    ALU_SLO,
    ALU_ROR,
    ALU_ROL,
    ALU_GREV,
    ALU_GORC,
    ALU_SHFL,
    ALU_UNSHFL,
    ALU_XPERM_N,
    ALU_XPERM_B,
    ALU_XPERM_H,

    // Address Calculations
    // RV32B
    ALU_SH1ADD,
    ALU_SH2ADD,
    ALU_SH3ADD,

    // Comparisons
    ALU_LT,
    ALU_LTU,
    ALU_GE,
    ALU_GEU,
    ALU_EQ,
    ALU_NE,
    // RV32B
    ALU_MIN,
    ALU_MINU,
    ALU_MAX,
    ALU_MAXU,

    // Pack
    // RV32B
    ALU_PACK,
    ALU_PACKU,
    ALU_PACKH,

    // Sign-Extend
    // RV32B
    ALU_SEXTB,
    ALU_SEXTH,

    // Bitcounting
    // RV32B
    ALU_CLZ,
    ALU_CTZ,
    ALU_CPOP,

    // Set lower than
    ALU_SLT,
    ALU_SLTU,

    // Ternary Bitmanip Operations
    // RV32B
    ALU_CMOV,
    ALU_CMIX,
    ALU_FSL,
    ALU_FSR,

    // Single-Bit Operations
    // RV32B
    ALU_BSET,
    ALU_BCLR,
    ALU_BINV,
    ALU_BEXT,

    // Bit Compress / Decompress
    // RV32B
    ALU_BCOMPRESS,
    ALU_BDECOMPRESS,

    // Bit Field Place
    // RV32B
    ALU_BFP,

    // Carry-less Multiply
    // RV32B
    ALU_CLMUL,
    ALU_CLMULR,
    ALU_CLMULH,

    // Cyclic Redundancy Check
    ALU_CRC32_B,
    ALU_CRC32C_B,
    ALU_CRC32_H,
    ALU_CRC32C_H,
    ALU_CRC32_W,
    ALU_CRC32C_W
  };

  enum class md_op_e : logic<2>::BASE {
    // Multiplier/divider
    MD_OP_MULL,
    MD_OP_MULH,
    MD_OP_DIV,
    MD_OP_REM
  };


  //////////////////////////////////
  // Control and status registers //
  //////////////////////////////////

  // CSR operations
  enum class csr_op_e : logic<2>::BASE {
    CSR_OP_READ,
    CSR_OP_WRITE,
    CSR_OP_SET,
    CSR_OP_CLEAR
  };

  // Privileged mode
  enum class priv_lvl_e : logic<2>::BASE {
    PRIV_LVL_M = 0b11,
    PRIV_LVL_H = 0b10,
    PRIV_LVL_S = 0b01,
    PRIV_LVL_U = 0b00
  };

  // Constants for the dcsr.xdebugver fields
  enum class x_debug_ver_e : logic<4>::BASE {
    XDEBUGVER_NO     = 0, // no external debug support
    XDEBUGVER_STD    = 4, // external debug according to RISC-V debug spec
    XDEBUGVER_NONSTD = 15 // debug not conforming to RISC-V debug spec
  };

  //////////////
  // WB stage //
  //////////////

  // Type of instruction present in writeback stage
  enum class wb_instr_type_e : logic<2>::BASE {
    WB_INSTR_LOAD,  // Instruction is awaiting load data
    WB_INSTR_STORE, // Instruction is awaiting store response
    WB_INSTR_OTHER  // Instruction doesn't fit into above categories
  };

  //////////////
  // ID stage //
  //////////////

  // Operand a selection
  enum class op_a_sel_e : logic<2>::BASE {
    OP_A_REG_A,
    OP_A_FWD,
    OP_A_CURRPC,
    OP_A_IMM
  };

  // Immediate a selection
  enum class imm_a_sel_e : logic<1>::BASE {
    IMM_A_Z,
    IMM_A_ZERO
  };

  // Operand b selection
  enum class op_b_sel_e : logic<1>::BASE  {
    OP_B_REG_B,
    OP_B_IMM
  };

  // Immediate b selection
  enum class imm_b_sel_e : logic<3>::BASE {
    IMM_B_I,
    IMM_B_S,
    IMM_B_B,
    IMM_B_U,
    IMM_B_J,
    IMM_B_INCR_PC,
    IMM_B_INCR_ADDR
  };

  // Regfile write data selection
  enum class rf_wd_sel_e : logic<1>::BASE  {
    RF_WD_EX,
    RF_WD_CSR
  };

  //////////////
  // IF stage //
  //////////////

  // PC mux selection
  enum class pc_sel_e : logic<3>::BASE {
    PC_BOOT,
    PC_JUMP,
    PC_EXC,
    PC_ERET,
    PC_DRET,
    PC_BP
  };

  // Exception PC mux selection
  enum class exc_pc_sel_e : logic<2>::BASE {
    EXC_PC_EXC,
    EXC_PC_IRQ,
    EXC_PC_DBD,
    EXC_PC_DBG_EXC // Exception while in debug mode
  };

  // Interrupt requests
  struct irqs_t {
    logic<1>  irq_software;
    logic<1>  irq_timer;
    logic<1>  irq_external;
    logic<15> irq_fast; // 15 fast interrupts,
                          // one interrupt is reserved for NMI (not visible through mip/mie)
  };

  // Exception cause
  enum class exc_cause_e : logic<6>::BASE {
    EXC_CAUSE_IRQ_SOFTWARE_M     = 0b100011,
    EXC_CAUSE_IRQ_TIMER_M        = 0b100111,
    EXC_CAUSE_IRQ_EXTERNAL_M     = 0b101011,
    // EXC_CAUSE_IRQ_FAST_0      = 0b110000,
    // EXC_CAUSE_IRQ_FAST_14     = 0b111110,
    EXC_CAUSE_IRQ_NM             = 0b111111, // == EXC_CAUSE_IRQ_FAST_15
    EXC_CAUSE_INSN_ADDR_MISA     = 0b000000,
    EXC_CAUSE_INSTR_ACCESS_FAULT = 0b000001,
    EXC_CAUSE_ILLEGAL_INSN       = 0b000010,
    EXC_CAUSE_BREAKPOINT         = 0b000011,
    EXC_CAUSE_LOAD_ACCESS_FAULT  = 0b000101,
    EXC_CAUSE_STORE_ACCESS_FAULT = 0b000111,
    EXC_CAUSE_ECALL_UMODE        = 0b001000,
    EXC_CAUSE_ECALL_MMODE        = 0b001011
  };

  // Debug cause
  enum class dbg_cause_e : logic<3>::BASE {
    DBG_CAUSE_NONE    = 0x0,
    DBG_CAUSE_EBREAK  = 0x1,
    DBG_CAUSE_TRIGGER = 0x2,
    DBG_CAUSE_HALTREQ = 0x3,
    DBG_CAUSE_STEP    = 0x4
  };

  // ICache constants
  static const unsigned int ADDR_W           = 32; 
  static const unsigned int BUS_SIZE         = 32;
  static const unsigned int BUS_BYTES        = BUS_SIZE/8;
  static const unsigned int BUS_W            = clog2(BUS_BYTES);
  static const unsigned int IC_SIZE_BYTES    = 4096;
  static const unsigned int IC_NUM_WAYS      = 2;
  static const unsigned int IC_LINE_SIZE     = 64;
  static const unsigned int IC_LINE_BYTES    = IC_LINE_SIZE/8;
  static const unsigned int IC_LINE_W        = clog2(IC_LINE_BYTES);
  static const unsigned int IC_NUM_LINES     = IC_SIZE_BYTES / IC_NUM_WAYS / IC_LINE_BYTES;
  static const unsigned int IC_LINE_BEATS    = IC_LINE_BYTES / BUS_BYTES;
  static const unsigned int IC_LINE_BEATS_W  = clog2(IC_LINE_BEATS);
  static const unsigned int IC_INDEX_W       = clog2(IC_NUM_LINES);
  static const unsigned int IC_INDEX_HI      = IC_INDEX_W + IC_LINE_W - 1;
  static const unsigned int IC_TAG_SIZE      = ADDR_W - IC_INDEX_W - IC_LINE_W + 1; // 1 valid bit
  static const unsigned int IC_OUTPUT_BEATS  = (BUS_BYTES / 2); // number of halfwords
  // ICache Scrambling Parameters
  static const unsigned int SCRAMBLE_KEY_W   = 128;
  static const unsigned int SCRAMBLE_NONCE_W = 64;

  // PMP constants
  static const unsigned int PMP_MAX_REGIONS      = 16;
  static const unsigned int PMP_CFG_W            = 8;

  // PMP acces type
  static const unsigned int PMP_I  = 0;
  static const unsigned int PMP_I2 = 1;
  static const unsigned int PMP_D  = 2;

  enum class pmp_req_e : logic<2>::BASE {
    PMP_ACC_EXEC    = 0b00,
    PMP_ACC_WRITE   = 0b01,
    PMP_ACC_READ    = 0b10
  };

  // PMP cfg structures
  enum class pmp_cfg_mode_e : logic<2>::BASE {
    PMP_MODE_OFF   = 0b00,
    PMP_MODE_TOR   = 0b01,
    PMP_MODE_NA4   = 0b10,
    PMP_MODE_NAPOT = 0b11
  };

  struct pmp_cfg_t {
    logic<1>       lock;
    pmp_cfg_mode_e mode;
    logic<1>       exec;
    logic<1>       write;
    logic<1>       read;
  };

  // Machine Security Configuration (ePMP)
  struct pmp_mseccfg_t {
    logic<1> rlb;  // Rule Locking Bypass
    logic<1> mmwp; // Machine Mode Whitelist Policy
    logic<1> mml;  // Machine Mode Lockdown
  };

  // CSRs
  enum class csr_num_e : logic<12>::BASE {
    // Machine information
    CSR_MVENDORID = 0xF11,
    CSR_MARCHID   = 0xF12,
    CSR_MIMPID    = 0xF13,
    CSR_MHARTID   = 0xF14,

    // Machine trap setup
    CSR_MSTATUS   = 0x300,
    CSR_MISA      = 0x301,
    CSR_MIE       = 0x304,
    CSR_MTVEC     = 0x305,
    CSR_MCOUNTEREN= 0x306,

    // Machine trap handling
    CSR_MSCRATCH  = 0x340,
    CSR_MEPC      = 0x341,
    CSR_MCAUSE    = 0x342,
    CSR_MTVAL     = 0x343,
    CSR_MIP       = 0x344,

    // Physical memory protection
    CSR_PMPCFG0   = 0x3A0,
    CSR_PMPCFG1   = 0x3A1,
    CSR_PMPCFG2   = 0x3A2,
    CSR_PMPCFG3   = 0x3A3,
    CSR_PMPADDR0  = 0x3B0,
    CSR_PMPADDR1  = 0x3B1,
    CSR_PMPADDR2  = 0x3B2,
    CSR_PMPADDR3  = 0x3B3,
    CSR_PMPADDR4  = 0x3B4,
    CSR_PMPADDR5  = 0x3B5,
    CSR_PMPADDR6  = 0x3B6,
    CSR_PMPADDR7  = 0x3B7,
    CSR_PMPADDR8  = 0x3B8,
    CSR_PMPADDR9  = 0x3B9,
    CSR_PMPADDR10 = 0x3BA,
    CSR_PMPADDR11 = 0x3BB,
    CSR_PMPADDR12 = 0x3BC,
    CSR_PMPADDR13 = 0x3BD,
    CSR_PMPADDR14 = 0x3BE,
    CSR_PMPADDR15 = 0x3BF,

    // ePMP control
    CSR_MSECCFG   = 0x747,
    CSR_MSECCFGH  = 0x757,

    // Debug trigger
    CSR_TSELECT   = 0x7A0,
    CSR_TDATA1    = 0x7A1,
    CSR_TDATA2    = 0x7A2,
    CSR_TDATA3    = 0x7A3,
    CSR_MCONTEXT  = 0x7A8,
    CSR_SCONTEXT  = 0x7AA,

    // Debug/trace
    CSR_DCSR      = 0x7b0,
    CSR_DPC       = 0x7b1,

    // Debug
    CSR_DSCRATCH0 = 0x7b2, // optional
    CSR_DSCRATCH1 = 0x7b3, // optional

    // Machine Counter/Timers
    CSR_MCOUNTINHIBIT  = 0x320,
    CSR_MHPMEVENT3     = 0x323,
    CSR_MHPMEVENT4     = 0x324,
    CSR_MHPMEVENT5     = 0x325,
    CSR_MHPMEVENT6     = 0x326,
    CSR_MHPMEVENT7     = 0x327,
    CSR_MHPMEVENT8     = 0x328,
    CSR_MHPMEVENT9     = 0x329,
    CSR_MHPMEVENT10    = 0x32A,
    CSR_MHPMEVENT11    = 0x32B,
    CSR_MHPMEVENT12    = 0x32C,
    CSR_MHPMEVENT13    = 0x32D,
    CSR_MHPMEVENT14    = 0x32E,
    CSR_MHPMEVENT15    = 0x32F,
    CSR_MHPMEVENT16    = 0x330,
    CSR_MHPMEVENT17    = 0x331,
    CSR_MHPMEVENT18    = 0x332,
    CSR_MHPMEVENT19    = 0x333,
    CSR_MHPMEVENT20    = 0x334,
    CSR_MHPMEVENT21    = 0x335,
    CSR_MHPMEVENT22    = 0x336,
    CSR_MHPMEVENT23    = 0x337,
    CSR_MHPMEVENT24    = 0x338,
    CSR_MHPMEVENT25    = 0x339,
    CSR_MHPMEVENT26    = 0x33A,
    CSR_MHPMEVENT27    = 0x33B,
    CSR_MHPMEVENT28    = 0x33C,
    CSR_MHPMEVENT29    = 0x33D,
    CSR_MHPMEVENT30    = 0x33E,
    CSR_MHPMEVENT31    = 0x33F,
    CSR_MCYCLE         = 0xB00,
    CSR_MINSTRET       = 0xB02,
    CSR_MHPMCOUNTER3   = 0xB03,
    CSR_MHPMCOUNTER4   = 0xB04,
    CSR_MHPMCOUNTER5   = 0xB05,
    CSR_MHPMCOUNTER6   = 0xB06,
    CSR_MHPMCOUNTER7   = 0xB07,
    CSR_MHPMCOUNTER8   = 0xB08,
    CSR_MHPMCOUNTER9   = 0xB09,
    CSR_MHPMCOUNTER10  = 0xB0A,
    CSR_MHPMCOUNTER11  = 0xB0B,
    CSR_MHPMCOUNTER12  = 0xB0C,
    CSR_MHPMCOUNTER13  = 0xB0D,
    CSR_MHPMCOUNTER14  = 0xB0E,
    CSR_MHPMCOUNTER15  = 0xB0F,
    CSR_MHPMCOUNTER16  = 0xB10,
    CSR_MHPMCOUNTER17  = 0xB11,
    CSR_MHPMCOUNTER18  = 0xB12,
    CSR_MHPMCOUNTER19  = 0xB13,
    CSR_MHPMCOUNTER20  = 0xB14,
    CSR_MHPMCOUNTER21  = 0xB15,
    CSR_MHPMCOUNTER22  = 0xB16,
    CSR_MHPMCOUNTER23  = 0xB17,
    CSR_MHPMCOUNTER24  = 0xB18,
    CSR_MHPMCOUNTER25  = 0xB19,
    CSR_MHPMCOUNTER26  = 0xB1A,
    CSR_MHPMCOUNTER27  = 0xB1B,
    CSR_MHPMCOUNTER28  = 0xB1C,
    CSR_MHPMCOUNTER29  = 0xB1D,
    CSR_MHPMCOUNTER30  = 0xB1E,
    CSR_MHPMCOUNTER31  = 0xB1F,
    CSR_MCYCLEH        = 0xB80,
    CSR_MINSTRETH      = 0xB82,
    CSR_MHPMCOUNTER3H  = 0xB83,
    CSR_MHPMCOUNTER4H  = 0xB84,
    CSR_MHPMCOUNTER5H  = 0xB85,
    CSR_MHPMCOUNTER6H  = 0xB86,
    CSR_MHPMCOUNTER7H  = 0xB87,
    CSR_MHPMCOUNTER8H  = 0xB88,
    CSR_MHPMCOUNTER9H  = 0xB89,
    CSR_MHPMCOUNTER10H = 0xB8A,
    CSR_MHPMCOUNTER11H = 0xB8B,
    CSR_MHPMCOUNTER12H = 0xB8C,
    CSR_MHPMCOUNTER13H = 0xB8D,
    CSR_MHPMCOUNTER14H = 0xB8E,
    CSR_MHPMCOUNTER15H = 0xB8F,
    CSR_MHPMCOUNTER16H = 0xB90,
    CSR_MHPMCOUNTER17H = 0xB91,
    CSR_MHPMCOUNTER18H = 0xB92,
    CSR_MHPMCOUNTER19H = 0xB93,
    CSR_MHPMCOUNTER20H = 0xB94,
    CSR_MHPMCOUNTER21H = 0xB95,
    CSR_MHPMCOUNTER22H = 0xB96,
    CSR_MHPMCOUNTER23H = 0xB97,
    CSR_MHPMCOUNTER24H = 0xB98,
    CSR_MHPMCOUNTER25H = 0xB99,
    CSR_MHPMCOUNTER26H = 0xB9A,
    CSR_MHPMCOUNTER27H = 0xB9B,
    CSR_MHPMCOUNTER28H = 0xB9C,
    CSR_MHPMCOUNTER29H = 0xB9D,
    CSR_MHPMCOUNTER30H = 0xB9E,
    CSR_MHPMCOUNTER31H = 0xB9F,
    CSR_CPUCTRL        = 0x7C0,
    CSR_SECURESEED     = 0x7C1
  };

  // CSR pmp-related offsets
  static const logic<12> CSR_OFF_PMP_CFG  = 0x3A0; // pmp_cfg  @ 12'h3a0 - 12'h3a3
  static const logic<12> CSR_OFF_PMP_ADDR = 0x3B0; // pmp_addr @ 12'h3b0 - 12'h3bf

  // CSR status bits
  static const unsigned int CSR_MSTATUS_MIE_BIT      = 3;
  static const unsigned int CSR_MSTATUS_MPIE_BIT     = 7;
  static const unsigned int CSR_MSTATUS_MPP_BIT_LOW  = 11;
  static const unsigned int CSR_MSTATUS_MPP_BIT_HIGH = 12;
  static const unsigned int CSR_MSTATUS_MPRV_BIT     = 17;
  static const unsigned int CSR_MSTATUS_TW_BIT       = 21;

  // CSR machine ISA
  static const logic<2> CSR_MISA_MXL = b2(1); // M-XLEN: XLEN in M-Mode for RV32

  // CSR interrupt pending/enable bits
  static const unsigned int CSR_MSIX_BIT      = 3;
  static const unsigned int CSR_MTIX_BIT      = 7;
  static const unsigned int CSR_MEIX_BIT      = 11;
  static const unsigned int CSR_MFIX_BIT_LOW  = 16;
  static const unsigned int CSR_MFIX_BIT_HIGH = 30;

  // CSR Machine Security Configuration bits
  static const unsigned int CSR_MSECCFG_MML_BIT  = 0;
  static const unsigned int CSR_MSECCFG_MMWP_BIT = 1;
  static const unsigned int CSR_MSECCFG_RLB_BIT  = 2;

  // Vendor ID
  // No JEDEC ID has been allocated to lowRISC so the value is 0 to indicate the field is not
  // implemented
  static const logic<32> CSR_MVENDORID_VALUE  = b32(0b0);

  // Architecture ID
  // Top bit is unset to indicate an open source project. The lower bits are an ID allocated by the
  // RISC-V Foundation. Note this is allocated specifically to Ibex, should significant changes be
  // made a different architecture ID should be supplied.
  static const logic<32> CSR_MARCHID_VALUE = b32(22);

  // Implementation ID
  // 0 indicates this field is not implemeted. Ibex implementors may wish to indicate an RTL/netlist
  // version here using their own unique encoding (e.g. 32 bits of the git hash of the implemented
  // commit).
  static const logic<32> CSR_MIMPID_VALUE = b32(0b0);

  // These LFSR parameters have been generated with
  // $ opentitan/util/design/gen-lfsr-seed.py --width 32 --seed 2480124384 --prefix ""
  static const int LfsrWidth = 32;
  typedef logic<LfsrWidth> lfsr_seed_t;
  typedef logic<LfsrWidth> lfsr_perm_t[clog2(LfsrWidth)]; 
  static const lfsr_seed_t RndCnstLfsrSeedDefault = b32(0xac533bf4);
  static const lfsr_perm_t RndCnstLfsrPermDefault = { b32(0x78a8daed), b32(0xc04fa438), b32(0x2e958152), b32(0x467fd1b1), b32(0x1e35ecba) };

  // we don't support 128-bit logic<>s in Metron yet
  //parameter logic [SCRAMBLE_KEY_W-1:0]   RndCnstIbexKeyDefault = 128'h14e8cecae3040d5e12286bb3cc113298;
  //parameter logic [SCRAMBLE_NONCE_W-1:0] RndCnstIbexNonceDefault = 64'hf79780bc735f3843;
}