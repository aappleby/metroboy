//--------------------------------------------------------------------------------
// MODULE:       pmp_mseccfg_t
// MODULEPARAMS: 
// INPUTS:       
// OUTPUTS:      
// LOCALPARAMS:  
// FIELDS:       rlb, mmwp, mml, 
// SUBMODULES:   
// TASKS:        
// FUNCTIONS:    
/* verilator lint_off WIDTH */
`default_nettype none
// Copyright lowRISC contributors.
// Copyright 2017 ETH Zurich and University of Bologna, see also CREDITS.md.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0


`include "metron.h.sv"

/**
 * Package with constants used by Ibex
 */
package ibex_pkg;

  ////////////////
  // IO Structs //
  ////////////////

  typedef struct packed {
    logic[31:0] current_pc;
    logic[31:0] next_pc;
    logic[31:0] last_data_addr;
    logic[31:0] exception_addr;
  } crash_dump_t;

  typedef struct packed {
    logic dummy_instr_id;
    logic[4:0] raddr_a;
    logic[4:0] waddr_a;
    logic we_a;
    logic[4:0] raddr_b;
  } core2rf_t;

  /////////////////////
  // Parameter Enums //
  /////////////////////

  typedef enum logic[1:0] {
    RegFileFF    = 'd0,
    RegFileFPGA  = 'd1,
    RegFileLatch = 'd2
  } regfile_e;

  typedef enum logic[1:0] {
    RV32MNone        = 'd0,
    RV32MSlow        = 'd1,
    RV32MFast        = 'd2,
    RV32MSingleCycle = 'd3
  } rv32m_e;

  typedef enum logic[1:0] {
    RV32BNone       = 'd0,
    RV32BBalanced   = 'd1,
    RV32BOTEarlGrey = 'd2,
    RV32BFull       = 'd3
  } rv32b_e;

  /////////////
  // Opcodes //
  /////////////

  typedef enum logic[6:0] {
    OPCODE_LOAD     = 'h03,
    OPCODE_MISC_MEM = 'h0f,
    OPCODE_OP_IMM   = 'h13,
    OPCODE_AUIPC    = 'h17,
    OPCODE_STORE    = 'h23,
    OPCODE_OP       = 'h33,
    OPCODE_LUI      = 'h37,
    OPCODE_BRANCH   = 'h63,
    OPCODE_JALR     = 'h67,
    OPCODE_JAL      = 'h6f,
    OPCODE_SYSTEM   = 'h73
  } opcode_e;

  ////////////////////
  // ALU operations //
  ////////////////////

  typedef enum logic[6:0] {
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
  } alu_op_e;

  typedef enum logic[1:0] {
    // Multiplier/divider
    MD_OP_MULL,
    MD_OP_MULH,
    MD_OP_DIV,
    MD_OP_REM
  } md_op_e;

  //////////////////////////////////
  // Control and status registers //
  //////////////////////////////////

  // CSR operations
  typedef enum logic[1:0] {
    CSR_OP_READ,
    CSR_OP_WRITE,
    CSR_OP_SET,
    CSR_OP_CLEAR
  } csr_op_e;

  // Privileged mode
  typedef enum logic[1:0] {
    PRIV_LVL_M = 'b11,
    PRIV_LVL_H = 'b10,
    PRIV_LVL_S = 'b01,
    PRIV_LVL_U = 'b00
  } priv_lvl_e;

  // Constants for the dcsr.xdebugver fields
  typedef enum logic[3:0] {
    XDEBUGVER_NO     = 'd0, // no external debug support
    XDEBUGVER_STD    = 'd4, // external debug according to RISC-V debug spec
    XDEBUGVER_NONSTD = 'd15 // debug not conforming to RISC-V debug spec
  } x_debug_ver_e;

  //////////////
  // WB stage //
  //////////////

  // Type of instruction present in writeback stage
  typedef enum logic[1:0] {
    WB_INSTR_LOAD,  // Instruction is awaiting load data
    WB_INSTR_STORE, // Instruction is awaiting store response
    WB_INSTR_OTHER  // Instruction doesn't fit into above categories
  } wb_instr_type_e;

  //////////////
  // ID stage //
  //////////////

  // Operand a selection
  typedef enum logic[1:0] {
    OP_A_REG_A,
    OP_A_FWD,
    OP_A_CURRPC,
    OP_A_IMM
  } op_a_sel_e;

  // Immediate a selection
  typedef enum logic[0:0] {
    IMM_A_Z,
    IMM_A_ZERO
  } imm_a_sel_e;

  // Operand b selection
  typedef enum logic[0:0] {
    OP_B_REG_B,
    OP_B_IMM
  } op_b_sel_e;

  // Immediate b selection
  typedef enum logic[2:0] {
    IMM_B_I,
    IMM_B_S,
    IMM_B_B,
    IMM_B_U,
    IMM_B_J,
    IMM_B_INCR_PC,
    IMM_B_INCR_ADDR
  } imm_b_sel_e;

  // Regfile write data selection
  typedef enum logic[0:0] {
    RF_WD_EX,
    RF_WD_CSR
  } rf_wd_sel_e;

  //////////////
  // IF stage //
  //////////////

  // PC mux selection
  typedef enum logic[2:0] {
    PC_BOOT,
    PC_JUMP,
    PC_EXC,
    PC_ERET,
    PC_DRET,
    PC_BP
  } pc_sel_e;

  // Exception PC mux selection
  typedef enum logic[1:0] {
    EXC_PC_EXC,
    EXC_PC_IRQ,
    EXC_PC_DBD,
    EXC_PC_DBG_EXC // Exception while in debug mode
  } exc_pc_sel_e;

  // Interrupt requests
  typedef struct packed {
    logic  irq_software;
    logic  irq_timer;
    logic  irq_external;
    logic[14:0] irq_fast; // 15 fast interrupts,
                          // one interrupt is reserved for NMI (not visible through mip/mie)
  } irqs_t;

  // Exception cause
  typedef enum logic[5:0] {
    EXC_CAUSE_IRQ_SOFTWARE_M     = 'b100011,
    EXC_CAUSE_IRQ_TIMER_M        = 'b100111,
    EXC_CAUSE_IRQ_EXTERNAL_M     = 'b101011,
    // EXC_CAUSE_IRQ_FAST_0      = 0b110000,
    // EXC_CAUSE_IRQ_FAST_14     = 0b111110,
    EXC_CAUSE_IRQ_NM             = 'b111111, // == EXC_CAUSE_IRQ_FAST_15
    EXC_CAUSE_INSN_ADDR_MISA     = 'b000000,
    EXC_CAUSE_INSTR_ACCESS_FAULT = 'b000001,
    EXC_CAUSE_ILLEGAL_INSN       = 'b000010,
    EXC_CAUSE_BREAKPOINT         = 'b000011,
    EXC_CAUSE_LOAD_ACCESS_FAULT  = 'b000101,
    EXC_CAUSE_STORE_ACCESS_FAULT = 'b000111,
    EXC_CAUSE_ECALL_UMODE        = 'b001000,
    EXC_CAUSE_ECALL_MMODE        = 'b001011
  } exc_cause_e;

  // Debug cause
  typedef enum logic[2:0] {
    DBG_CAUSE_NONE    = 'h0,
    DBG_CAUSE_EBREAK  = 'h1,
    DBG_CAUSE_TRIGGER = 'h2,
    DBG_CAUSE_HALTREQ = 'h3,
    DBG_CAUSE_STEP    = 'h4
  } dbg_cause_e;

  // ICache constants
  parameter int ADDR_W           = 'd32; 

  parameter int BUS_SIZE         = 'd32;
  parameter int BUS_BYTES        = BUS_SIZE/'d8;
  parameter int BUS_W            = $clog2(BUS_BYTES);
  parameter int IC_SIZE_BYTES    = 'd4096;
  parameter int IC_NUM_WAYS      = 'd2;
  parameter int IC_LINE_SIZE     = 'd64;
  parameter int IC_LINE_BYTES    = IC_LINE_SIZE/'d8;
  parameter int IC_LINE_W        = $clog2(IC_LINE_BYTES);
  parameter int IC_NUM_LINES     = IC_SIZE_BYTES / IC_NUM_WAYS / IC_LINE_BYTES;
  parameter int IC_LINE_BEATS    = IC_LINE_BYTES / BUS_BYTES;
  parameter int IC_LINE_BEATS_W  = $clog2(IC_LINE_BEATS);
  parameter int IC_INDEX_W       = $clog2(IC_NUM_LINES);
  parameter int IC_INDEX_HI      = IC_INDEX_W + IC_LINE_W - 'd1;
  parameter int IC_TAG_SIZE      = ADDR_W - IC_INDEX_W - IC_LINE_W + 'd1; // 1 valid bit
  parameter int IC_OUTPUT_BEATS  = (BUS_BYTES / 'd2); // number of halfwords
  // ICache Scrambling Parameters
  parameter int SCRAMBLE_KEY_W   = 'd128;
  parameter int SCRAMBLE_NONCE_W = 'd64;

  // PMP constants
  parameter int PMP_MAX_REGIONS      = 'd16;
  parameter int PMP_CFG_W            = 'd8;

  // PMP acces type
  parameter int PMP_I  = 'd0;
  parameter int PMP_I2 = 'd1;
  parameter int PMP_D  = 'd2;

  typedef enum logic[1:0] {
    PMP_ACC_EXEC    = 'b00,
    PMP_ACC_WRITE   = 'b01,
    PMP_ACC_READ    = 'b10
  } pmp_req_e;

  // PMP cfg structures
  typedef enum logic[1:0] {
    PMP_MODE_OFF   = 'b00,
    PMP_MODE_TOR   = 'b01,
    PMP_MODE_NA4   = 'b10,
    PMP_MODE_NAPOT = 'b11
  } pmp_cfg_mode_e;

  typedef struct packed {
    logic       lock;
    pmp_cfg_mode_e mode;
    logic       exec;
    logic       write;
    logic       read;
  } pmp_config_t;

  // Machine Security Configuration (ePMP)
  typedef struct packed {
    logic rlb;  // Rule Locking Bypass
    logic mmwp; // Machine Mode Whitelist Policy
    logic mml;  // Machine Mode Lockdown
  } pmp_mseccfg_t;

  // CSRs
  typedef enum logic[11:0] {
    // Machine information
    CSR_MVENDORID = 'hF11,
    CSR_MARCHID   = 'hF12,
    CSR_MIMPID    = 'hF13,
    CSR_MHARTID   = 'hF14,

    // Machine trap setup
    CSR_MSTATUS   = 'h300,
    CSR_MISA      = 'h301,
    CSR_MIE       = 'h304,
    CSR_MTVEC     = 'h305,
    CSR_MCOUNTEREN= 'h306,

    // Machine trap handling
    CSR_MSCRATCH  = 'h340,
    CSR_MEPC      = 'h341,
    CSR_MCAUSE    = 'h342,
    CSR_MTVAL     = 'h343,
    CSR_MIP       = 'h344,

    // Physical memory protection
    CSR_PMPCFG0   = 'h3A0,
    CSR_PMPCFG1   = 'h3A1,
    CSR_PMPCFG2   = 'h3A2,
    CSR_PMPCFG3   = 'h3A3,
    CSR_PMPADDR0  = 'h3B0,
    CSR_PMPADDR1  = 'h3B1,
    CSR_PMPADDR2  = 'h3B2,
    CSR_PMPADDR3  = 'h3B3,
    CSR_PMPADDR4  = 'h3B4,
    CSR_PMPADDR5  = 'h3B5,
    CSR_PMPADDR6  = 'h3B6,
    CSR_PMPADDR7  = 'h3B7,
    CSR_PMPADDR8  = 'h3B8,
    CSR_PMPADDR9  = 'h3B9,
    CSR_PMPADDR10 = 'h3BA,
    CSR_PMPADDR11 = 'h3BB,
    CSR_PMPADDR12 = 'h3BC,
    CSR_PMPADDR13 = 'h3BD,
    CSR_PMPADDR14 = 'h3BE,
    CSR_PMPADDR15 = 'h3BF,

    // ePMP control
    CSR_MSECCFG   = 'h747,
    CSR_MSECCFGH  = 'h757,

    // Debug trigger
    CSR_TSELECT   = 'h7A0,
    CSR_TDATA1    = 'h7A1,
    CSR_TDATA2    = 'h7A2,
    CSR_TDATA3    = 'h7A3,
    CSR_MCONTEXT  = 'h7A8,
    CSR_SCONTEXT  = 'h7AA,

    // Debug/trace
    CSR_DCSR      = 'h7b0,
    CSR_DPC       = 'h7b1,

    // Debug
    CSR_DSCRATCH0 = 'h7b2, // optional
    CSR_DSCRATCH1 = 'h7b3, // optional

    // Machine Counter/Timers
    CSR_MCOUNTINHIBIT  = 'h320,
    CSR_MHPMEVENT3     = 'h323,
    CSR_MHPMEVENT4     = 'h324,
    CSR_MHPMEVENT5     = 'h325,
    CSR_MHPMEVENT6     = 'h326,
    CSR_MHPMEVENT7     = 'h327,
    CSR_MHPMEVENT8     = 'h328,
    CSR_MHPMEVENT9     = 'h329,
    CSR_MHPMEVENT10    = 'h32A,
    CSR_MHPMEVENT11    = 'h32B,
    CSR_MHPMEVENT12    = 'h32C,
    CSR_MHPMEVENT13    = 'h32D,
    CSR_MHPMEVENT14    = 'h32E,
    CSR_MHPMEVENT15    = 'h32F,
    CSR_MHPMEVENT16    = 'h330,
    CSR_MHPMEVENT17    = 'h331,
    CSR_MHPMEVENT18    = 'h332,
    CSR_MHPMEVENT19    = 'h333,
    CSR_MHPMEVENT20    = 'h334,
    CSR_MHPMEVENT21    = 'h335,
    CSR_MHPMEVENT22    = 'h336,
    CSR_MHPMEVENT23    = 'h337,
    CSR_MHPMEVENT24    = 'h338,
    CSR_MHPMEVENT25    = 'h339,
    CSR_MHPMEVENT26    = 'h33A,
    CSR_MHPMEVENT27    = 'h33B,
    CSR_MHPMEVENT28    = 'h33C,
    CSR_MHPMEVENT29    = 'h33D,
    CSR_MHPMEVENT30    = 'h33E,
    CSR_MHPMEVENT31    = 'h33F,
    CSR_MCYCLE         = 'hB00,
    CSR_MINSTRET       = 'hB02,
    CSR_MHPMCOUNTER3   = 'hB03,
    CSR_MHPMCOUNTER4   = 'hB04,
    CSR_MHPMCOUNTER5   = 'hB05,
    CSR_MHPMCOUNTER6   = 'hB06,
    CSR_MHPMCOUNTER7   = 'hB07,
    CSR_MHPMCOUNTER8   = 'hB08,
    CSR_MHPMCOUNTER9   = 'hB09,
    CSR_MHPMCOUNTER10  = 'hB0A,
    CSR_MHPMCOUNTER11  = 'hB0B,
    CSR_MHPMCOUNTER12  = 'hB0C,
    CSR_MHPMCOUNTER13  = 'hB0D,
    CSR_MHPMCOUNTER14  = 'hB0E,
    CSR_MHPMCOUNTER15  = 'hB0F,
    CSR_MHPMCOUNTER16  = 'hB10,
    CSR_MHPMCOUNTER17  = 'hB11,
    CSR_MHPMCOUNTER18  = 'hB12,
    CSR_MHPMCOUNTER19  = 'hB13,
    CSR_MHPMCOUNTER20  = 'hB14,
    CSR_MHPMCOUNTER21  = 'hB15,
    CSR_MHPMCOUNTER22  = 'hB16,
    CSR_MHPMCOUNTER23  = 'hB17,
    CSR_MHPMCOUNTER24  = 'hB18,
    CSR_MHPMCOUNTER25  = 'hB19,
    CSR_MHPMCOUNTER26  = 'hB1A,
    CSR_MHPMCOUNTER27  = 'hB1B,
    CSR_MHPMCOUNTER28  = 'hB1C,
    CSR_MHPMCOUNTER29  = 'hB1D,
    CSR_MHPMCOUNTER30  = 'hB1E,
    CSR_MHPMCOUNTER31  = 'hB1F,
    CSR_MCYCLEH        = 'hB80,
    CSR_MINSTRETH      = 'hB82,
    CSR_MHPMCOUNTER3H  = 'hB83,
    CSR_MHPMCOUNTER4H  = 'hB84,
    CSR_MHPMCOUNTER5H  = 'hB85,
    CSR_MHPMCOUNTER6H  = 'hB86,
    CSR_MHPMCOUNTER7H  = 'hB87,
    CSR_MHPMCOUNTER8H  = 'hB88,
    CSR_MHPMCOUNTER9H  = 'hB89,
    CSR_MHPMCOUNTER10H = 'hB8A,
    CSR_MHPMCOUNTER11H = 'hB8B,
    CSR_MHPMCOUNTER12H = 'hB8C,
    CSR_MHPMCOUNTER13H = 'hB8D,
    CSR_MHPMCOUNTER14H = 'hB8E,
    CSR_MHPMCOUNTER15H = 'hB8F,
    CSR_MHPMCOUNTER16H = 'hB90,
    CSR_MHPMCOUNTER17H = 'hB91,
    CSR_MHPMCOUNTER18H = 'hB92,
    CSR_MHPMCOUNTER19H = 'hB93,
    CSR_MHPMCOUNTER20H = 'hB94,
    CSR_MHPMCOUNTER21H = 'hB95,
    CSR_MHPMCOUNTER22H = 'hB96,
    CSR_MHPMCOUNTER23H = 'hB97,
    CSR_MHPMCOUNTER24H = 'hB98,
    CSR_MHPMCOUNTER25H = 'hB99,
    CSR_MHPMCOUNTER26H = 'hB9A,
    CSR_MHPMCOUNTER27H = 'hB9B,
    CSR_MHPMCOUNTER28H = 'hB9C,
    CSR_MHPMCOUNTER29H = 'hB9D,
    CSR_MHPMCOUNTER30H = 'hB9E,
    CSR_MHPMCOUNTER31H = 'hB9F,
    CSR_CPUCTRL        = 'h7C0,
    CSR_SECURESEED     = 'h7C1
  } csr_num_e;

  // CSR pmp-related offsets
  parameter logic[11:0] CSR_OFF_PMP_CFG  = 'h3A0; // pmp_cfg  @ 12'h3a0 - 12'h3a3
  parameter logic[11:0] CSR_OFF_PMP_ADDR = 'h3B0; // pmp_addr @ 12'h3b0 - 12'h3bf

  // CSR status bits
  parameter int CSR_MSTATUS_MIE_BIT      = 'd3;
  parameter int CSR_MSTATUS_MPIE_BIT     = 'd7;
  parameter int CSR_MSTATUS_MPP_BIT_LOW  = 'd11;
  parameter int CSR_MSTATUS_MPP_BIT_HIGH = 'd12;
  parameter int CSR_MSTATUS_MPRV_BIT     = 'd17;
  parameter int CSR_MSTATUS_TW_BIT       = 'd21;

  // CSR machine ISA
  parameter logic[1:0] CSR_MISA_MXL = 'd1; // M-XLEN: XLEN in M-Mode for RV32

  // CSR interrupt pending/enable bits
  parameter int CSR_MSIX_BIT      = 'd3;
  parameter int CSR_MTIX_BIT      = 'd7;
  parameter int CSR_MEIX_BIT      = 'd11;
  parameter int CSR_MFIX_BIT_LOW  = 'd16;
  parameter int CSR_MFIX_BIT_HIGH = 'd30;

  // CSR Machine Security Configuration bits
  parameter int CSR_MSECCFG_MML_BIT  = 'd0;
  parameter int CSR_MSECCFG_MMWP_BIT = 'd1;
  parameter int CSR_MSECCFG_RLB_BIT  = 'd2;

  // Vendor ID
  // No JEDEC ID has been allocated to lowRISC so the value is 0 to indicate the field is not
  // implemented
  parameter logic[31:0] CSR_MVENDORID_VALUE  = 'd0;

  // Architecture ID
  // Top bit is unset to indicate an open source project. The lower bits are an ID allocated by the
  // RISC-V Foundation. Note this is allocated specifically to Ibex, should significant changes be
  // made a different architecture ID should be supplied.
  parameter logic[31:0] CSR_MARCHID_VALUE = 'd22;

  // Implementation ID
  // 0 indicates this field is not implemeted. Ibex implementors may wish to indicate an RTL/netlist
  // version here using their own unique encoding (e.g. 32 bits of the git hash of the implemented
  // commit).
  parameter logic[31:0] CSR_MIMPID_VALUE = 'd0;

  // These LFSR parameters have been generated with
  // $ opentitan/util/design/gen-lfsr-seed.py --width 32 --seed 2480124384 --prefix ""
  /*
  static const int LfsrWidth = 32;
  typedef logic [LfsrWidth-1:0] lfsr_seed_t;
  typedef logic [LfsrWidth-1:0][$clog2(LfsrWidth)-1:0] lfsr_perm_t;
  parameter lfsr_seed_t RndCnstLfsrSeedDefault = 32'hac533bf4;
  parameter lfsr_perm_t RndCnstLfsrPermDefault = { 160'h1e35ecba467fd1b12e958152c04fa43878a8daed };
  parameter logic [SCRAMBLE_KEY_W-1:0]   RndCnstIbexKeyDefault = 128'h14e8cecae3040d5e12286bb3cc113298;
  parameter logic [SCRAMBLE_NONCE_W-1:0] RndCnstIbexNonceDefault = 64'hf79780bc735f3843;
  */
endpackage
