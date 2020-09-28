#include "CoreLib/Z80.h"
#include "CoreLib/Constants.h"
#include <assert.h>
#include <stdio.h>

#pragma warning(disable:4189)
#pragma warning(disable:4244)
#pragma warning(disable:4458)

//-----------------------------------------------------------------------------

#define OP_QUAD       ((op >> 6) & 3)
#define OP_ROW        ((op >> 3) & 7)
#define OP_COL        ((op >> 0) & 7)

#define NOP           (op == 0x00)
#define STOP          (op == 0x10)

#define STM_A8_A      (op == 0xE0)
#define STM_C_A       (op == 0xE2)
#define STM_A16_SP    (op == 0x08)
#define STM_A16_A     (op == 0xEA)

#define ADD_SP_R8     (op == 0xE8)
#define LD_HL_SP_R8   (op == 0xF8)

#define LDM_A_A16     (op == 0xFA)
#define LDM_A_C       (op == 0xF2)
#define LDM_A_A8      (op == 0xF0)

#define JP_A16        (op == 0xC3)
#define JR_R8         (op == 0x18)
#define JP_HL         (op == 0xE9)
#define CALL_A16      (op == 0xCD)
#define RET           (op == 0xC9)
#define RETI          (op == 0xD9)
#define DI            (op == 0xF3)
#define EI            (op == 0xFB)
#define HALT          (op == 0x76)
#define LD_SP_HL      (op == 0xF9)

#define LD_BC_D16     (op == 0b00000001)
#define LD_DE_D16     (op == 0b00010001)
#define LD_HL_D16     (op == 0b00100001)
#define LD_SP_D16     (op == 0b00110001)
#define LD_RR_D16     ((op & 0b11001111) == 0b00000001)

#define ADD_HL_BC     (op == 0b00001001)
#define ADD_HL_DE     (op == 0b00011001)
#define ADD_HL_HL     (op == 0b00101001)
#define ADD_HL_SP     (op == 0b00111001)
#define ADD_HL_RR     ((op & 0b11001111) == 0b00001001)

#define STM_BC_A      (op == 0x02)
#define STM_DE_A      (op == 0x12)
#define STM_HLP_A     (op == 0x22)
#define STM_HLM_A     (op == 0x32)

#define LDM_A_BC      (op == 0x0A)
#define LDM_A_DE      (op == 0x1A)
#define LDM_A_HLP     (op == 0x2A)
#define LDM_A_HLM     (op == 0x3A)

#define INC_BC        (op == 0b00000011)
#define INC_DE        (op == 0b00010011)
#define INC_HL        (op == 0b00100011)
#define INC_SP        (op == 0b00110011)

#define DEC_BC        (op == 0x0B)
#define DEC_DE        (op == 0x1B)
#define DEC_HL        (op == 0x2B)
#define DEC_SP        (op == 0x3B)

#define INC_AT_HL     (op == 0x34)
#define DEC_AT_HL     (op == 0x35)
#define INC_R         ((op & 0b11000111) == 0b00000100 && !INC_AT_HL)
#define DEC_R         ((op & 0b11000111) == 0b00000101 && !DEC_AT_HL)

#define STM_HL_A      (op == 0x77)
#define STM_HL_D8     (op == 0x36)
#define LD_R_D8       ((op & 0b11000111) == 0b00000110 && !STM_HL_D8)

#define RLU_A         ((op & 0b11000111) == 0b00000111)

#define RLC_A         (op == 0x07)
#define RRC_A         (op == 0x0F)
#define RL_A          (op == 0x17)
#define RR_A          (op == 0x1F)
#define DAA           (op == 0x27)
#define CPL           (op == 0x2F)
#define SCF           (op == 0x37)
#define CCF           (op == 0x3F)

#define LDM_B_HL      (op == 0x46)
#define LDM_C_HL      (op == 0x4E)
#define LDM_D_HL      (op == 0x56)
#define LDM_E_HL      (op == 0x5E)
#define LDM_H_HL      (op == 0x66)
#define LDM_L_HL      (op == 0x6E)
#define LDM_A_HL      (op == 0x7E)

#define LDM_R_HL      ((op & 0b11000111) == 0b01000110 && !HALT)
#define STM_HL_R      ((op & 0b11111000) == 0b01110000 && !HALT)
#define MV_R_R        ((op & 0b11000000) == 0b01000000 && (op & 0b00000111) != 0b00000110 && (op & 0b00111000) != 0b00110000)

#define JR_CC_R8      ((op & 0b11100111) == 0b00100000)
#define RET_CC        ((op & 0b11100111) == 0b11000000)
#define JP_CC_A16     ((op & 0b11100111) == 0b11000010)
#define CALL_CC_A16   ((op & 0b11100111) == 0b11000100)

#define PUSH_BC       (op == 0b11000101)
#define PUSH_DE       (op == 0b11010101)
#define PUSH_HL       (op == 0b11100101)
#define PUSH_AF       (op == 0b11110101)
#define PUSH_RR       ((op & 0b11001111) == 0b11000101)

#define POP_BC        (op == 0b11000001)
#define POP_DE        (op == 0b11010001)
#define POP_HL        (op == 0b11100001)
#define POP_AF        (op == 0b11110001)
#define POP_RR        ((op & 0b11001111) == 0b11000001)

#define PREFIX_CB     (op == 0xCB)
#define CB_QUAD       ((cb >> 6) & 3)
#define CB_ROW        ((cb >> 3) & 7)
#define CB_COL        ((cb >> 0) & 7)
#define OP_CB_R       (PREFIX_CB && CB_COL != 6)
#define OP_CB_HL      (PREFIX_CB && CB_COL == 6)

#define ALU_A_R       ((OP_QUAD == 2) && (OP_COL != 6))
#define ALU_A_HL      ((OP_QUAD == 2) && (OP_COL == 6))
#define ALU_A_D8      ((OP_QUAD == 3) && (OP_COL == 6))
#define RST_NN        ((OP_QUAD == 3) && (OP_COL == 7))

// fake opcode to indicate interrupt handling
#define INT           (op == 0xF4)

uint8_t  sxt(uint8_t x) { return x & 0x80 ? 0xFF : 0x00; }

constexpr uint8_t F_CARRY = 0x10;
constexpr uint8_t F_HALF_CARRY = 0x20;
constexpr uint8_t F_NEGATIVE = 0x40;
constexpr uint8_t F_ZERO = 0x80;

//-----------------------------------------------------------------------------

void Z80::reset(uint16_t new_pc) {
  *this = Z80();

  if (new_pc == 0x100) {
    ime = ime_delay = 0;

    state = 0;
             
    int_ack = 0;

    pc = 0x100;
    op_addr = pc;
    op = 0;
    cb = 0;
    state = 0;

    bc = 0x0013;
    de = 0x00D8;
    hl = 0x014D;
    af = 0x01B0;
    sp = 0xFFFE;
    xy = 0x0000;
  }
  else {
    ime = ime_delay = 0;

    state = 0;
             
    int_ack = 0;

    pc = 0x0000;
    op_addr = pc;
    op = 0;
    cb = 0;
    state = 0;

    bc = 0x0000;
    de = 0x0000;
    hl = 0x0000;
    af = 0x0000;
    sp = 0x0000;
    xy = 0x0000;
  }

  set_bus(pc, 0);
}

//-----------------------------------------------------------------------------

void Z80::set_bus(uint16_t new_addr, int new_write) {
  bus_req.addr    = new_addr;
  bus_req.data_lo = uint16_t(new_write ? out : 0);
  bus_req.read    = (bool)!new_write;
  bus_req.write   = (bool)new_write;
}

void Z80::clear_bus() {
  bus_req.addr    = 0x0000;
  bus_req.data_lo = 0;
  bus_req.read    = 0;
  bus_req.write   = 0;
}

#define GET_CB    get_reg(CB_COL)
#define SET_CB(A) set_reg(CB_COL, A)

//-----------------------------------------------------------------------------
// Do the meat of executing the instruction
// pc update _must_ happen in tcycle 0 of state 0, because if an interrupt fires it should _not_ happen.

void Z80::tock(uint8_t imask_, uint8_t intf_, uint8_t bus_data) {
  state = state_;
  ime = ime_delay;

  if (bus_req.read) {
    in = bus_data;
  }

  if (state == 0) {
    op_addr = bus_req.addr;
    op = bus_data;     

    if (imask_ & intf_) {
      //cpu_halted = false;
      if (ime) {
        //printf("interrupt!\n");
        op = 0xF4; // fake opcode
        ime = false;
        ime_delay = false;
      }
    }
  }

  alu_x = 0;
  alu_y = 0;
  int_ack = 0;

  if (INT) {
    execute_int(imask_, intf_);
  }                                                                                                                                                                                                                             
  else if (HALT) {
    execute_halt(imask_, intf_);
  }                                                                                                                                                                                                                                          
  else {                                                         
    execute_op();                                                                                                                                      
  }
}

//-----------------------------------------------------------------------------

void Z80::execute_int(uint8_t imask_, uint8_t intf_) {
  if (state == 0) {
    set_bus(sp, 0);
    state_ = 1;
  }

  if (state == 1) {
    spl = dec(spl, 1);
    out = pch;
    sph = dec(sph, inc_c);
    set_bus(sp, 1);
    state_ = 2;
  }
    
  if (state == 2) {
    spl = dec(spl, 1);
    out = pcl;
    sph = dec(sph, inc_c);
    set_bus(sp, 1);
    state_ = 3;
  }

  if (state == 3) {
    set_bus(xy, 0);
    state_ = 4;
  }

  if (state == 4) {
    uint8_t int_addr = 0;

    if      (imask_ & intf_ & INT_JOYPAD_MASK) { int_addr = 0x60; int_ack = INT_JOYPAD_MASK; }
    else if (imask_ & intf_ & INT_SERIAL_MASK) { int_addr = 0x58; int_ack = INT_SERIAL_MASK; }
    else if (imask_ & intf_ & INT_TIMER_MASK)  { int_addr = 0x50; int_ack = INT_TIMER_MASK; }
    else if (imask_ & intf_ & INT_STAT_MASK)   { int_addr = 0x48; int_ack = INT_STAT_MASK; }
    else if (imask_ & intf_ & INT_VBLANK_MASK) { int_addr = 0x40; int_ack = INT_VBLANK_MASK; }
    else                                       { int_addr = 0x00; int_ack = 0; }

    pc = int_addr;
    set_bus(pc, 0);
    state_ = 0;
  }
}

//-----------------------------------------------------------------------------

void Z80::execute_halt(uint8_t imask_, uint8_t intf_) {

  if (state == 0) {
    pcl = inc(pcl, 1);
    pch = inc(pch, inc_c);
    set_bus(pc, 0);
    if (imask_ & intf_) {
      state_ = 0;
    }
    else {
      state_ = 1;
    }
  }
  else if (state == 1) {
    if (imask_ & intf_) {
      set_bus(pc, 0);
      state_ = 0;
    }
  }
}

//-----------------------------------------------------------------------------

void Z80::execute_op() {
  bool read_arg = false;
  bool op_done = false;
  bool op_done2 = false;
  bool pop_sp = false;
  bool no_bus = false;

  if (PREFIX_CB) {                                                                                                                                                                                                                      
    if (state == 1) cb = in;                                                                                                                                                                                                           
    uint8_t mask = cb_flag_mask[CB_QUAD];                                                                                                                                                                                                    

    bool OP_CB_BIT = PREFIX_CB && (CB_QUAD == 1);

    if (state == 0)                           /**/ {                                            /**/                  pcl = inc(pcl, 1);           /**/                              pch = inc(pch, inc_c);    set_bus(pc, 0); state_ = 1; }

    if (OP_CB_R) {
      if (state == 1)                         /**/ { alu_x = GET_CB;                            /**/                  pcl = inc(pcl, 1);           /**/ SET_CB(alu_cb(cb, f));       pch = inc(pch, inc_c);    op_done = 1; set_f(mask); }
    }
    else {
      if (OP_CB_BIT) {
        if (state == 1)                       /**/ {                                            /**/                                               /**/                                                        set_bus(hl, 0); state_ = 2; }
        if (state == 2)                       /**/ { alu_x = in;                                /**/                  pcl = inc(pcl, 1);           /**/ out = alu_cb(cb, f);         pch = inc(pch, inc_c);    op_done = 1; set_f(mask); }
      }
      else {
        if (state == 1)                       /**/ {                                            /**/                                               /**/                                                        set_bus(hl, 0); state_ = 2; }
        if (state == 2)                       /**/ { alu_x = in;                                /**/                  pcl = inc(pcl, 1);           /**/ out = alu_cb(cb, f);         pch = inc(pch, inc_c);    set_bus(hl, 1); state_ = 3; set_f(mask); }
        if (state == 3)                       /**/ {                                            /**/                                               /**/                                                        op_done = 1; }
      }
    }
  }
  else {
    bool branch = false;                                                                                                                                                                               
    switch (OP_ROW & 3) {                                                                                                                                                                              
    case 0: branch = !(f & F_ZERO); break;                                                                                                                                                             
    case 1: branch =  (f & F_ZERO); break;                                                                                                                                                             
    case 2: branch = !(f & F_CARRY); break;                                                                                                                                                            
    case 3: branch =  (f & F_CARRY); break;                                                                                                                                                            
    }                                                                                                                                                                                                  

    uint8_t bus = 0xDD;

    // IN OUT BC DE HL AF XY | PC SP AD

    // misc

    if (state == 0 && NOP)                    /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 0 && STOP)                   /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 0 && DI)                     /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 0 && EI)                     /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 0 && MV_R_R)                 /**/ { set_reg(OP_ROW, get_reg(OP_COL));           /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && LD_SP_HL)               /**/ {                                             /**/                            /**/                                       sp = hl; no_bus = 1; }
    if (state == 1 && LD_SP_HL)               /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && LD_R_D8)                /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && LD_R_D8)                /**/ { set_reg(OP_ROW, in);                        /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    // 8-bit alu                                                                                                                                                                                                                          
                                                                                                                                                                                               
    if (state == 0 && ALU_A_R)                /**/ { alu_y = get_reg(OP_COL);                    /**/ alu_x = a;                 /**/ a = alu(OP_ROW, f);                   op_done2 = 1; set_f(0xF0); }
    if (state == 0 && INC_R)                  /**/ { alu_x = get_reg(OP_ROW);                    /**/                            /**/ set_reg(OP_ROW, alu(1, F_CARRY));     op_done2 = 1; set_f(0xE0); }
    if (state == 0 && DEC_R)                  /**/ { alu_x = get_reg(OP_ROW);                    /**/                            /**/ set_reg(OP_ROW, alu(3, F_CARRY));     op_done2 = 1; set_f(0xE0); }
    if (state == 0 && RLC_A)                  /**/ { alu_x = a;                                  /**/                            /**/ a = rlu(OP_ROW, f);                   op_done2 = 1; set_f(0xF0); }
    if (state == 0 && RRC_A)                  /**/ { alu_x = a;                                  /**/                            /**/ a = rlu(OP_ROW, f);                   op_done2 = 1; set_f(0xF0); }
    if (state == 0 && RL_A)                   /**/ { alu_x = a;                                  /**/                            /**/ a = rlu(OP_ROW, f);                   op_done2 = 1; set_f(0xF0); }
    if (state == 0 && RR_A)                   /**/ { alu_x = a;                                  /**/                            /**/ a = rlu(OP_ROW, f);                   op_done2 = 1; set_f(0xF0); }
    if (state == 0 && DAA)                    /**/ { alu_x = a;                                  /**/                            /**/ a = rlu(OP_ROW, f);                   op_done2 = 1; set_f(0xB0); }
    if (state == 0 && CPL)                    /**/ { alu_x = a;                                  /**/                            /**/ a = rlu(OP_ROW, f);                   op_done2 = 1; set_f(0x60); }
    if (state == 0 && SCF)                    /**/ { alu_x = a;                                  /**/                            /**/ a = rlu(OP_ROW, f);                   op_done2 = 1; set_f(0x70); }
    if (state == 0 && CCF)                    /**/ { alu_x = a;                                  /**/                            /**/ a = rlu(OP_ROW, f);                   op_done2 = 1; set_f(0x70); }
                                                                                                                                                                                               
    if (state == 0 && ALU_A_D8)               /**/ {                                             /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(pc, 0); state_ = state + 1; }
    if (state == 1 && ALU_A_D8)               /**/ { alu_y = in;                                 /**/ alu_x = a;                 /**/ a = alu(OP_ROW, f);                   op_done2 = 1; set_f(0xF0); }
                                                                                                                                                                                               
    if (state == 0 && ALU_A_HL)               /**/ {                                             /**/                            /**/                                       set_bus(hl, 0); state_ = state + 1; }
    if (state == 1 && ALU_A_HL)               /**/ { alu_y = in;                                 /**/ alu_x = a;                 /**/ a = alu(OP_ROW, f);                   op_done2 = 1; set_f(0xF0); }
                                                                                                                                                                                               
                                                                                                                                                                                               
    if (state == 0 && INC_AT_HL)              /**/ {                                             /**/                            /**/                                       set_bus(hl, 0); state_ = state + 1; }
    if (state == 0 && DEC_AT_HL)              /**/ {                                             /**/                            /**/                                       set_bus(hl, 0); state_ = state + 1; }
    if (state == 1 && INC_AT_HL)              /**/ { alu_x = in;                                 /**/                            /**/ out = alu(1, F_CARRY);                set_bus(hl, 1); state_ = state + 1; set_f(0xE0); }
    if (state == 1 && DEC_AT_HL)              /**/ { alu_x = in;                                 /**/                            /**/ out = alu(3, F_CARRY);                set_bus(hl, 1); state_ = state + 1; set_f(0xE0); }
    if (state == 2 && INC_AT_HL)              /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 2 && DEC_AT_HL)              /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    // 16-bit alu                                                                                                                                                                              
                                                                                                                                                                                               
    if (state == 0 && ADD_SP_R8)              /**/ {                                             /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(pc, 0); state_ = state + 1; }
    if (state == 1 && ADD_SP_R8)              /**/ { alu_x = in;                                 /**/ alu_y = spl;               /**/ spl = alu(0, f);                      clear_bus(); state_ = state + 1; set_f(0xF0); }
    if (state == 2 && ADD_SP_R8)              /**/ { alu_x = sxt(in);                            /**/ alu_y = sph;               /**/ sph = alu(1, f);                      clear_bus(); state_ = state + 1; }
    if (state == 3 && ADD_SP_R8)              /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    // FIXME                                                                                                                                                                                   
    if (state == 0 && LD_HL_SP_R8)            /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && LD_HL_SP_R8)            /**/ { alu_x = in;                                 /**/ alu_y = spl;               /**/ l = alu(0, f);                        no_bus = 1; set_f(0xF0); }
    if (state == 2 && LD_HL_SP_R8)            /**/ { alu_x = sxt(in);                            /**/ alu_y = sph;               /**/ h = alu(1, f);                        op_done2 = 1; }
                                                                                                                                                                                               
    // FIXME                                                                                                                                                                                   
    if (state == 0 && INC_BC)                 /**/ {                                             /**/                            /**/                                       c = inc(  c, 1);   b = inc(  b, inc_c);      no_bus = 1; }
    if (state == 0 && DEC_BC)                 /**/ {                                             /**/                            /**/                                       c = dec(  c, 1);   b = dec(  b, inc_c);      no_bus = 1; }
    if (state == 0 && INC_DE)                 /**/ {                                             /**/                            /**/                                       e = inc(  e, 1);   d = inc(  d, inc_c);      no_bus = 1; }
    if (state == 0 && DEC_DE)                 /**/ {                                             /**/                            /**/                                       e = dec(  e, 1);   d = dec(  d, inc_c);      no_bus = 1; }
    if (state == 0 && INC_HL)                 /**/ {                                             /**/                            /**/                                       l = inc(  l, 1);   h = inc(  h, inc_c);      no_bus = 1; }
    if (state == 0 && DEC_HL)                 /**/ {                                             /**/                            /**/                                       l = dec(  l, 1);   h = dec(  h, inc_c);      no_bus = 1; }
    if (state == 0 && INC_SP)                 /**/ {                                             /**/                            /**/                                       spl = inc(spl, 1); sph = inc(sph, inc_c);    no_bus = 1; }
    if (state == 0 && DEC_SP)                 /**/ {                                             /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    no_bus = 1; }
                                                                                                                                                                                               
    if (state == 1 && INC_BC)                 /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 1 && DEC_BC)                 /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 1 && INC_DE)                 /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 1 && DEC_DE)                 /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 1 && INC_HL)                 /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 1 && DEC_HL)                 /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 1 && INC_SP)                 /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 1 && DEC_SP)                 /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    // load/store                                                                                                                                                                              
                                                                                                                                                                                               
    if (state == 0 && STM_A16_SP)             /**/ { out = spl;                                  /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && STM_A16_SP)             /**/ {                   xyl = in;                 /**/                            /**/                                       read_arg = 1; }
    if (state == 2 && STM_A16_SP)             /**/ {                   xyh = in;                 /**/                            /**/                                       set_bus(xy, 1); state_ = state + 1; }
    if (state == 3 && STM_A16_SP)             /**/ { out = sph;                                  /**/                            /**/                                       xyl = inc(xyl, 1); xyh = inc(xyh, inc_c);    set_bus(xy, 1); state_ = state + 1; }
    if (state == 4 && STM_A16_SP)             /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && STM_A16_A)              /**/ { out = a;                                    /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && STM_A16_A)              /**/ {                   xyl = in;                 /**/                            /**/                                       read_arg = 1; }
    if (state == 2 && STM_A16_A)              /**/ {                   xyh = in;                 /**/                            /**/                                       set_bus(xy, 1); state_ = state + 1; }
    if (state == 3 && STM_A16_A)              /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && LDM_A_A16)              /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && LDM_A_A16)              /**/ {                   xyl = in;                 /**/                            /**/                                       read_arg = 1; }
    if (state == 2 && LDM_A_A16)              /**/ {                   xyh = in;                 /**/                            /**/                                       set_bus(xy, 0); state_ = state + 1; }
    if (state == 3 && LDM_A_A16)              /**/ { a = in;                                     /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    // indirect load/store                                                                                                                                                                     
                                                                                                                                                                                               
    if (state == 0 && LDM_A_BC)               /**/ {                                             /**/                            /**/                                       set_bus(bc, 0); state_ = state + 1; }
    if (state == 1 && LDM_A_BC)               /**/ { a = in;                                     /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && LDM_A_DE)               /**/ {                                             /**/                            /**/                                       set_bus(de, 0); state_ = state + 1; }
    if (state == 1 && LDM_A_DE)               /**/ { a = in;                                     /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && STM_BC_A)               /**/ { out = a;                                    /**/                            /**/                                       set_bus(bc, 1); state_ = state + 1; }
    if (state == 1 && STM_BC_A)               /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && STM_DE_A)               /**/ { out = a;                                    /**/                            /**/                                       set_bus(de, 1); state_ = state + 1; }
    if (state == 1 && STM_DE_A)               /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && LDM_R_HL)               /**/ {                                             /**/                            /**/                                       set_bus(hl, 0); state_ = state + 1; }
    if (state == 1 && LDM_R_HL)               /**/ { set_reg(OP_ROW, in);                        /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && STM_HL_R)               /**/ { out = get_reg(OP_COL);                      /**/                            /**/                                       set_bus(hl, 1); state_ = state + 1; }
    if (state == 1 && STM_HL_R)               /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && STM_HL_D8)              /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && STM_HL_D8)              /**/ { out = in;                                   /**/                            /**/                                       set_bus(hl, 1); state_ = state + 1; }
    if (state == 2 && STM_HL_D8)              /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && LDM_A_HLP)              /**/ {                                             /**/                            /**/                                                                                    set_bus(hl, 0); state_ = state + 1; }
    if (state == 0 && STM_HLP_A)              /**/ { out = a;                                    /**/                            /**/                                                                                    set_bus(hl, 1); state_ = state + 1; }
    if (state == 0 && LDM_A_HLM)              /**/ {                                             /**/                            /**/                                                                                    set_bus(hl, 0); state_ = state + 1; }
    if (state == 0 && STM_HLM_A)              /**/ { out = a;                                    /**/                            /**/                                                                                    set_bus(hl, 1); state_ = state + 1; }
    if (state == 1 && LDM_A_HLP)              /**/ { a = in;                                     /**/                            /**/ hl++;                                 op_done2 = 1; }
    if (state == 1 && STM_HLP_A)              /**/ {                                             /**/                            /**/ hl++;                                 op_done2 = 1; }
    if (state == 1 && LDM_A_HLM)              /**/ { a = in;                                     /**/                            /**/ hl--;                                 op_done2 = 1; }
    if (state == 1 && STM_HLM_A)              /**/ {                                             /**/                            /**/ hl--;                                 op_done2 = 1; }
                                                                                                                                                                                               
    // zero-page load/store                                                                                                                                                                                                               
                                                                                                                                                                                               
    if (state == 0 && LDM_A_A8)               /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 0 && STM_A8_A)               /**/ {                                             /**/                            /**/                                       read_arg = 1; }
                                                                                                                                                                                               
    if (state == 0 && LDM_A_C)                /**/ {                                             /**/                            /**/                                       xyl = c;           xyh = 0xFF;               set_bus(xy, 0); state_ = state + 1; }
    if (state == 0 && STM_C_A)                /**/ { out = a;                                    /**/                            /**/                                       xyl = c;           xyh = 0xFF;               set_bus(xy, 1); state_ = state + 1; }
    if (state == 1 && LDM_A_A8)               /**/ {                                             /**/                            /**/                                       xyl = in;          xyh = 0xFF;               set_bus(xy, 0); state_ = state + 1; }
    if (state == 1 && STM_A8_A)               /**/ { out = a;                                    /**/                            /**/                                       xyl = in;          xyh = 0xFF;               set_bus(xy, 1); state_ = state + 1; }
                                                                                                                                                                                               
    if (state == 1 && LDM_A_C)                /**/ { a = in;                                     /**/                            /**/                                       op_done2 = 1; }
    if (state == 1 && STM_C_A)                /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 2 && LDM_A_A8)               /**/ { a = in;                                     /**/                            /**/                                       op_done2 = 1; }
    if (state == 2 && STM_A8_A)               /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    // push / pop                                                                                                                                                                                                                         
                                                                                                                                                                                               
    if (state == 0 && PUSH_BC)                /**/ {                                             /**/                            /**/                                                                                    set_bus(sp, 0); state_ = state + 1; }
    if (state == 0 && PUSH_DE)                /**/ {                                             /**/                            /**/                                                                                    set_bus(sp, 0); state_ = state + 1; }
    if (state == 0 && PUSH_HL)                /**/ {                                             /**/                            /**/                                                                                    set_bus(sp, 0); state_ = state + 1; }
    if (state == 0 && PUSH_AF)                /**/ {                                             /**/                            /**/                                                                                    set_bus(sp, 0); state_ = state + 1; }
    if (state == 1 && PUSH_BC)                /**/ { out = b;                                    /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 1 && PUSH_DE)                /**/ { out = d;                                    /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 1 && PUSH_HL)                /**/ { out = h;                                    /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 1 && PUSH_AF)                /**/ { out = a;                                    /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 2 && PUSH_BC)                /**/ { out = c;                                    /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 2 && PUSH_DE)                /**/ { out = e;                                    /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 2 && PUSH_HL)                /**/ { out = l;                                    /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 2 && PUSH_AF)                /**/ { out = f;                                    /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 3 && PUSH_BC)                /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 3 && PUSH_DE)                /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 3 && PUSH_HL)                /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
    if (state == 3 && PUSH_AF)                /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && POP_BC)                 /**/ {                                             /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 0 && POP_DE)                 /**/ {                                             /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 0 && POP_HL)                 /**/ {                                             /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 0 && POP_AF)                 /**/ {                                             /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 1 && POP_BC)                 /**/ { c = in;                                     /**/                            /**/                                       spl = inc(spl, 1); sph = inc(sph, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 1 && POP_DE)                 /**/ { e = in;                                     /**/                            /**/                                       spl = inc(spl, 1); sph = inc(sph, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 1 && POP_HL)                 /**/ { l = in;                                     /**/                            /**/                                       spl = inc(spl, 1); sph = inc(sph, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 1 && POP_AF)                 /**/ { f = in;                                     /**/                            /**/                                       spl = inc(spl, 1); sph = inc(sph, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 2 && POP_BC)                 /**/ { b = in;                                     /**/                            /**/                                       spl = inc(spl, 1); sph = inc(sph, inc_c);    op_done = 1; }
    if (state == 2 && POP_DE)                 /**/ { d = in;                                     /**/                            /**/                                       spl = inc(spl, 1); sph = inc(sph, inc_c);    op_done = 1; }
    if (state == 2 && POP_HL)                 /**/ { h = in;                                     /**/                            /**/                                       spl = inc(spl, 1); sph = inc(sph, inc_c);    op_done = 1; }
    if (state == 2 && POP_AF)                 /**/ { a = in;                                     /**/                            /**/                                       spl = inc(spl, 1); sph = inc(sph, inc_c);    op_done = 1; }
                                                                                                                                                                                               
    // 16-bit stuff                                                                                                                                                                                                                                   
                                                                                                                                                                                               
    if (state == 0 && LD_BC_D16)              /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 0 && LD_DE_D16)              /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 0 && LD_HL_D16)              /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 0 && LD_SP_D16)              /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && LD_BC_D16)              /**/ { c = in;                                     /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && LD_DE_D16)              /**/ { e = in;                                     /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && LD_HL_D16)              /**/ { l = in;                                     /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && LD_SP_D16)              /**/ { spl = in;                                   /**/                            /**/                                       read_arg = 1; }
    if (state == 2 && LD_BC_D16)              /**/ { b = in;                                     /**/                            /**/                                       op_done2 = 1; }
    if (state == 2 && LD_DE_D16)              /**/ { d = in;                                     /**/                            /**/                                       op_done2 = 1; }
    if (state == 2 && LD_HL_D16)              /**/ { h = in;                                     /**/                            /**/                                       op_done2 = 1; }
    if (state == 2 && LD_SP_D16)              /**/ { sph = in;                                   /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && ADD_HL_BC)              /**/ { alu_x = c;                                  /**/ alu_y = l;                 /**/ l = alu(0, f);                        no_bus = 1; }
    if (state == 0 && ADD_HL_DE)              /**/ { alu_x = e;                                  /**/ alu_y = l;                 /**/ l = alu(0, f);                        no_bus = 1; }
    if (state == 0 && ADD_HL_HL)              /**/ { alu_x = l;                                  /**/ alu_y = l;                 /**/ l = alu(0, f);                        no_bus = 1; }
    if (state == 0 && ADD_HL_SP)              /**/ { alu_x = spl;                                /**/ alu_y = l;                 /**/ l = alu(0, f);                        no_bus = 1; }
    if (state == 1 && ADD_HL_BC)              /**/ { alu_x = b;                                  /**/ alu_y = h;                 /**/ h = alu(1, alu_f);                    op_done2 = 1; set_f(0x70); }
    if (state == 1 && ADD_HL_DE)              /**/ { alu_x = d;                                  /**/ alu_y = h;                 /**/ h = alu(1, alu_f);                    op_done2 = 1; set_f(0x70); }
    if (state == 1 && ADD_HL_HL)              /**/ { alu_x = h;                                  /**/ alu_y = h;                 /**/ h = alu(1, alu_f);                    op_done2 = 1; set_f(0x70); }
    if (state == 1 && ADD_HL_SP)              /**/ { alu_x = sph;                                /**/ alu_y = h;                 /**/ h = alu(1, alu_f);                    op_done2 = 1; set_f(0x70); }
                                                                                                                                                                                                                                         
    // conditional branches                                                                                                                                                                                                              
                                                                                                                                                                                                                                         
    if (state == 0 && JR_R8)                  /**/ {                                             /**/                            /**/                                       xyl = inc(pcl, 1); xyh = inc(pch, inc_c);    set_bus(xy, 0); state_ = state + 1; }
    if (state == 1 && JR_R8)                  /**/ { alu_x = xyl;                                /**/ alu_y = in;                /**/                                       pcl = alu(1, F_CARRY);                       clear_bus();    state_ = state + 1; }                                                         
    if (state == 2 && JR_R8)                  /**/ { alu_x = xyh;                                /**/ alu_y = sxt(in);           /**/                                       pch = alu(1, alu_f);                         op_done = 1; }
                                                                                                                                                                                               
    if (state == 0 && JR_CC_R8    &&  branch) /**/ {                                             /**/                            /**/                                       xyl = inc(pcl, 1); xyh = inc(pch, inc_c);    set_bus(xy, 0); state_ = state + 1; }
    if (state == 1 && JR_CC_R8    &&  branch) /**/ { alu_x = xyl;                                /**/ alu_y = in;                /**/                                       pcl = alu(1, F_CARRY);                       set_bus(xy, 0); state_ = state + 1; }
    if (state == 2 && JR_CC_R8    &&  branch) /**/ { alu_x = xyh;                                /**/ alu_y = sxt(in);           /**/                                       pch = alu(1, alu_f);                         op_done = 1; }
                                                                                                                                                                                               
    if (state == 0 && JR_CC_R8    && !branch) /**/ {                                             /**/                            /**/                                       xyl = inc(pcl, 1); xyh = inc(pch, inc_c);    set_bus(xy, 0); state_ = state + 1; }
    if (state == 1 && JR_CC_R8    && !branch) /**/ {                                             /**/                            /**/                                       pcl = inc(xyl, 1); pch = inc(xyh, inc_c);    op_done = 1; }
                                                                                                                                                                                               
                                                                                                                                                                                               
    if (state == 0 && JP_CC_A16   &&  branch) /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 0 && JP_CC_A16   && !branch) /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && JP_CC_A16   &&  branch) /**/ { xyl = in;                                   /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && JP_CC_A16   && !branch) /**/ { xyl = in;                                   /**/                            /**/                                       read_arg = 1; }
    if (state == 2 && JP_CC_A16   &&  branch) /**/ { xyh = in;                                   /**/                            /**/                                       clear_bus(); state_ = state + 1; }
    if (state == 2 && JP_CC_A16   && !branch) /**/ { xyh = in;                                   /**/                            /**/                                       op_done2 = 1; }
    if (state == 3 && JP_CC_A16   &&  branch) /**/ {                                             /**/                            /**/                                       pc = xy; op_done = 1; }
                                                                                                                                                                                               
    if (state == 0 && JP_A16)                 /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && JP_A16)                 /**/ { xyl = in;                                   /**/                            /**/                                       read_arg = 1; }
    if (state == 2 && JP_A16)                 /**/ { xyh = in;                                   /**/                            /**/                                       clear_bus(); state_ = state + 1; }
    if (state == 3 && JP_A16)                 /**/ {                                             /**/                            /**/                                       pc = xy; op_done = 1; }
                                                                                                                                                                                               
    if (state == 0 && JP_HL)                  /**/ {                                             /**/                            /**/                                       pc = hl; op_done = 1; }
                                                                                                                                                                                               
    if (state == 0 && CALL_CC_A16 &&  branch) /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && CALL_CC_A16 &&  branch) /**/ { xyl = in;                                   /**/                            /**/                                       read_arg = 1; }
    if (state == 2 && CALL_CC_A16 &&  branch) /**/ { xyh = in;                                   /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 3 && CALL_CC_A16 &&  branch) /**/ { out = pch;                                  /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 4 && CALL_CC_A16 &&  branch) /**/ { out = pcl;                                  /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 5 && CALL_CC_A16 &&  branch) /**/ {                                             /**/                            /**/                                       pc = xy; op_done = 1; }
                                                                                                                                                                                               
    if (state == 0 && CALL_CC_A16 && !branch) /**/ {                                             /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(pc, 0); state_ = state + 1; }
    if (state == 1 && CALL_CC_A16 && !branch) /**/ { xyl = in;                                   /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(pc, 0); state_ = state + 1; }
    if (state == 2 && CALL_CC_A16 && !branch) /**/ {                                             /**/                            /**/                                       op_done2 = 1; }
                                                                                                                                                                                               
    if (state == 0 && CALL_A16)               /**/ {                                             /**/                            /**/                                       read_arg = 1; }
    if (state == 1 && CALL_A16)               /**/ { xyl = in;                                   /**/                            /**/                                       read_arg = 1; }
    if (state == 2 && CALL_A16)               /**/ { xyh = in;                                   /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    set_bus(sp, 0); state_ = state + 1; }
    if (state == 3 && CALL_A16)               /**/ { out = pch;                                  /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 4 && CALL_A16)               /**/ { out = pcl;                                  /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 5 && CALL_A16)               /**/ {                                             /**/                            /**/                                       pc = xy; op_done = 1; }
                                                                                                                                                                                               
    if (state == 0 && RST_NN)                 /**/ { alu_y = 0x38;                               /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    clear_bus();    state_ = state + 1; }
    if (state == 1 && RST_NN)                 /**/ { out = pch;                                  /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 2 && RST_NN)                 /**/ { out = pcl;                                  /**/                            /**/                                       spl = dec(spl, 1); sph = dec(sph, inc_c);    set_bus(sp, 1); state_ = state + 1; }
    if (state == 3 && RST_NN)                 /**/ {                                             /**/                            /**/                                       pc = op - 0xC7;                              op_done = 1; }

    if (state == 0 && RET_CC      && !branch) /**/ {                                             /**/                            /**/                                       pcl = inc(pcl, 1); pch = inc(pch, inc_c);    clear_bus();    state_ = state + 1; }
    if (state == 1 && RET_CC      && !branch) /**/ {                                             /**/                            /**/                                                                                    op_done = 1; }
                                                                                                                                                                                               
    if (state == 0 && RET_CC      &&  branch) /**/ {           no_bus = 1; }
    if (state == 1 && RET_CC      &&  branch) /**/ {           no_bus = 1; }
    if (state == 2 && RET_CC      &&  branch) /**/ {           pop_sp = 1; }
    if (state == 3 && RET_CC      &&  branch) /**/ { pcl = in; pop_sp = 1; }
    if (state == 4 && RET_CC      &&  branch) /**/ { pch = in; op_done = 1; }
                                                                                  
    if (state == 0 && RETI)                   /**/ {           pop_sp = 1; }
    if (state == 1 && RETI)                   /**/ { pcl = in; pop_sp = 1; }
    if (state == 2 && RETI)                   /**/ { pch = in; no_bus = 1; }
    if (state == 3 && RETI)                   /**/ {           op_done = 1; }
                                                                                  
    if (state == 0 && RET)                    /**/ {           pop_sp = 1; }
    if (state == 1 && RET)                    /**/ { pcl = in; pop_sp = 1; }
    if (state == 2 && RET)                    /**/ { pch = in; no_bus = 1; }
    if (state == 3 && RET)                    /**/ {           op_done = 1; }
                                                                                                                                                                                               
    f &= 0xF0;

    if (RETI && state_ == 0) {ime = true;       ime_delay = true;}
    if (DI)                  {ime = false;      ime_delay = false;}
    if (EI)                  {ime = ime_delay;  ime_delay = true;}
  }

  if (no_bus) {
    clear_bus();
    state_ = state + 1;
  }

  if (pop_sp) {
    set_bus(sp, 0);
    spl = inc(spl, 1);
    sph = inc(sph, inc_c);
    state_ = state + 1;
  }

  if (read_arg) {
    pcl = inc(pcl, 1);
    pch = inc(pch, inc_c);    
    set_bus(pc, 0);
    state_ = state + 1;
  }

  if (op_done2) {
    pcl = inc(pcl, 1);
    pch = inc(pch, inc_c);
    set_bus(pc, 0);
    state_ = 0;
  }
  else if (op_done) {
    set_bus(pc, 0);
    state_ = 0;
  }
}

//-----------------------------------------------------------------------------

uint8_t Z80::get_reg(int mux) {
  switch(mux) {
  case 0: return b;
  case 1: return c;
  case 2: return d;
  case 3: return e;
  case 4: return h;
  case 5: return l;
  case 6: return 0;
  case 7: return a;
  }
  return 0;
}

void Z80::set_reg(int mux, uint8_t data) {
  switch(mux) {
  case 0: b = data; break;
  case 1: c = data; break;
  case 2: d = data; break;
  case 3: e = data; break;
  case 4: h = data; break;
  case 5: l = data; break;
  case 6: break;
  case 7: a = data; break;
  }
}

//-----------------------------------------------------------------------------

void Z80::set_f(uint8_t mask) {
  f = (f & ~mask) | (alu_f & mask);
  if (ADD_SP_R8)   { f &= ~(F_ZERO | F_NEGATIVE); }
  if (LD_HL_SP_R8) { f &= ~(F_ZERO | F_NEGATIVE); }
}

//-----------------------------------------------------------------------------

uint8_t Z80::alu(int op, uint8_t flags) {
  alu_f = flags;
  const uint8_t x = alu_x;
  const uint8_t y = alu_y;

  uint16_t d1 = 0, d2 = 0;
  const bool c = alu_f & F_CARRY;

  switch(op) {
  case 0: d1 = (x & 0xF) + (y & 0xF);     d2 = x + y;     break;
  case 1: d1 = (x & 0xF) + (y & 0xF) + c; d2 = x + y + c; break;
  case 2: d1 = (x & 0xF) - (y & 0xF);     d2 = x - y;     break;
  case 3: d1 = (x & 0xF) - (y & 0xF) - c; d2 = x - y - c; break;
  case 4: d1 = 0xFFF;                     d2 = x & y;     break;
  case 5: d1 = 0x000;                     d2 = x ^ y;     break;
  case 6: d1 = 0x000;                     d2 = x | y;     break;
  case 7: d1 = (x & 0xF) - (y & 0xF);     d2 = x - y;     break;
  }

  uint8_t alu_o = (uint8_t)d2;
  alu_f = (op == 2 || op == 3 || op == 7) ? F_NEGATIVE : 0;

  if (d1 & 0x010) alu_f |= F_HALF_CARRY;
  if (d2 & 0x100) alu_f |= F_CARRY;
  if (alu_o == 0x000) alu_f |= F_ZERO;
  if (op == 7)  alu_o = x;

  return alu_o;
}

//-----------------------------------------------------------------------------
// The logic is more annoying, but this can be implemented as two 4-bit additions

uint8_t Z80::daa(uint8_t x, uint8_t f) {
  bool c = f & F_CARRY;
  bool h = f & F_HALF_CARRY;
  bool n = f & F_NEGATIVE;

  // low nibble
  uint8_t lo = (x >> 0) & 0xF;
  bool o = lo > 9;
  uint8_t d = 0;
  if (+h || +o) d = 0x6;
  if (+h && +n) d = 0xA;
  if (!h && +n) d = 0x0;
  lo += d;

  // high nibble
  uint8_t hi = (x >> 4) & 0xF;
  o = (lo >> 4) ? (hi > 8) : (hi > 9);
  d = 0;
  if (+n && (!h && +c)) d = 0xA;
  if (+n && (+h && !c)) d = 0xF;
  if (+n && (+h && +c)) d = 0x9;
  if (!n && (+o || +c)) d = 0x6;
  hi += d + (lo >> 4);

  // output
  uint8_t alu_o = uint8_t((hi << 4) | (lo & 0xF));
  alu_f = 0;
  if (c) alu_f |= F_CARRY;
  if ((hi >> 4) && !n) alu_f |= F_CARRY;
  if (!alu_o) alu_f |= F_ZERO;
  return alu_o;
}

//-----------------------------------------------------------------------------

uint8_t Z80::rlu(int op, uint8_t flags) {
  alu_f = flags;
  const uint8_t x = alu_x;
  const uint8_t f = alu_f;

  uint8_t alu_o = 0;

  switch (op) {
  case 0:
    alu_o = (x << 1) | (x >> 7);
    alu_f = (x >> 7) ? F_CARRY : 0;
    alu_f &= ~F_ZERO;
    break;
  case 1:
    alu_o = (x >> 1) | (x << 7);
    alu_f = (x & 1) ? F_CARRY : 0;
    alu_f &= ~F_ZERO;
    break;
  case 2:
    alu_o = (x << 1) | (f & F_CARRY ? 1 : 0);
    alu_f = (x >> 7) ? F_CARRY : 0;
    alu_f &= ~F_ZERO;
    break;
  case 3:
    alu_o = (x >> 1) | ((f & F_CARRY ? 1 : 0) << 7);
    alu_f = (x & 1) ? F_CARRY : 0;
    alu_f &= ~F_ZERO;
    break;
  case 4:
    alu_o = daa(x, f);
    if (!alu_o) alu_f |= F_ZERO;
    break;
  case 5:
    alu_o = ~x;
    alu_f = f | 0x60;
    if (!alu_o) alu_f |= F_ZERO;
    break;
  case 6:
    alu_o = x;
    alu_f = (f & 0x80) | 0x10;
    if (!alu_o) alu_f |= F_ZERO;
    break;
  case 7:
    alu_o = x;
    alu_f = (f & 0x90) ^ 0x10;
    if (!alu_o) alu_f |= F_ZERO;
    break;
  }

  return alu_o;
}

//-----------------------------------------------------------------------------

uint8_t Z80::alu_cb(int op, uint8_t flags) {
  alu_f = flags;
  const uint8_t x = alu_x;
  const uint8_t quad = ((op >> 6) & 3);
  const uint8_t row = ((op >> 3) & 7);
  const bool bit_mux = (x >> row) & 1;

  uint8_t alu_o = 0;

  switch (quad) {
  case 0:
    switch (row) {
    case 0:
      alu_o = (x << 1) | (x >> 7);
      alu_f = (x >> 7) ? F_CARRY : 0;
      break;
    case 1:
      alu_o = (x >> 1) | (x << 7);
      alu_f = (x & 1) ? F_CARRY : 0;
      break;
    case 2:
      alu_o = (x << 1) | (f & F_CARRY ? 1 : 0);
      alu_f = (x >> 7) ? F_CARRY : 0;
      break;
    case 3:
      alu_o = (x >> 1) | ((f & F_CARRY ? 1 : 0) << 7);
      alu_f = (x & 1) ? F_CARRY : 0;
      break;
    case 4:
      alu_o = (x << 1) & 0xFF;
      alu_f = 0;
      if (x >> 7)     alu_f |= F_CARRY;
      break;
    case 5:
      alu_o = ((x >> 1) | (x & 0x80)) & 0xFF;
      alu_f = 0;
      if (x & 1)      alu_f |= F_CARRY;
      break;
    case 6:
      alu_o = ((x << 4) | (x >> 4)) & 0xFF;
      alu_f = 0;
      break;
    case 7:
      alu_o = (x >> 1) & 0xFF;
      alu_f = 0;
      if (x & 1)      alu_f |= F_CARRY;
      break;
    }
    if (alu_o == 0) alu_f |= F_ZERO;
    break;
  case 1:
    alu_o = x;
    alu_f = (f & 0x10) | 0x20;
    if (!bit_mux) alu_f |= F_ZERO;
    break;
  case 2:
    alu_o = x & (~(1 << row));
    alu_f = f;
    break;
  case 3:
    alu_o = x | (1 << row);
    alu_f = f;
    break;
  }

  return alu_o;
}

//-----------------------------------------------------------------------------

void Z80::dump(Dumper& d_) const {
  d_("\002------------- CPU --------------\001\n");
  d_("state    %d\n", state);
  //d_("halted   %d\n", cpu_halted);
  d_("op_addr  0x%04x\n", op_addr);
  d_("OP       0x%02x '%s' @ %d\n", op, op_strings2[op], state);
  d_("CB       0x%02x\n", cb);
  d_("in       0x%02x\n", in);
  d_("out      0x%02x\n", out);
  d_("\n");
  d_("bus req   ");
  dump_req(d_, bus_req);
  d_("PC        0x%04x 0x%02x 0x%02x\n", pc, pcl, pch);
  d_("SP        0x%04x 0x%02x 0x%02x\n", sp, sph, spl);
  d_("XY        0x%04x 0x%02x 0x%02x\n", xy, xyh, xyl);
  d_("BC        0x%04x 0x%02x 0x%02x\n", bc, b, c);
  d_("DE        0x%04x 0x%02x 0x%02x\n", de, d, e);
  d_("HL        0x%04x 0x%02x 0x%02x\n", hl, h, l);
  d_("AF        0x%04x 0x%02x 0x%02x\n", af, a, f);
  d_("alu_f     0x%02x\n", alu_f);
  d_("\n");
  d_("IME       %d\n", ime);
  d_("IME_      %d\n", ime_delay);
  d_("interrupt %d\n", INT);
  d_("int_ack   0x%02x\n", int_ack);
  d_("\n");
}

//-----------------------------------------------------------------------------
