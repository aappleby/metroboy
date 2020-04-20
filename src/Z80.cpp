#include "Z80.h"
#include "Constants.h"
#include <assert.h>

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

const uint8_t F_CARRY = 0x10;
const uint8_t F_HALF_CARRY = 0x20;
const uint8_t F_NEGATIVE = 0x40;
const uint8_t F_ZERO = 0x80;

//-----------------------------------------------------------------------------

void Z80::reset(uint16_t new_pc) {

  if (new_pc == 0x100) {
    unhalt = 0;
    ime = ime_delay = 0;

    state = 0;
             
    int_ack = 0;

    pc = 0xFF;
    op_addr = pc;
    op = 0;
    cb = 0;
    addr = 0xFF;
    data_in = 0;
    state = 0;

    bc = 0x0013;
    de = 0x00D8;
    hl = 0x014D;
    af = 0x01B0;
    sp = 0xFFFE;
    xy = 0x0000;
  }
  else {
    unhalt = 0;
    ime = ime_delay = 0;

    state = 0;
             
    int_ack = 0;

    pc = 0xFFFF;
    op_addr = pc;
    op = 0;
    cb = 0;
    addr = pc;
    state = 0;

    bc = 0x0000;
    de = 0x0000;
    hl = 0x0000;
    af = 0x0000;
    sp = 0x0000;
    xy = 0x0000;
  }

  bus = 0;
}

//-----------------------------------------------------------------------------

Req Z80::get_bus_req() const {
  return {
    .addr  = addr,
    .data  = uint16_t(write ? data_out : 0),
    .read  = (bool)!write,
    .write = (bool)write,
  };
}

void Z80::on_bus_ack(Ack ibus_ack_) {
  data_in = (uint8_t)ibus_ack_.data;
}

//-----------------------------------------------------------------------------
// Do the meat of executing the instruction
// pc update _must_ happen in tcycle 0 of state 0, because if an interrupt fires it should _not_ happen.

#pragma warning(disable:4189)

void Z80::tock_t30(const uint8_t imask, const uint8_t intf) {
  //printf("tock_t30\n");

  state = state_;
  ime = ime_delay;

  if (state == 0) {
    op_addr = addr;
    op = data_in;
    int_ack = 0;

    if ((imask & intf) && ime) {
      op = 0xF4; // fake opcode
      ime = false;
      ime_delay = false;
    }

    // should do some decoding here to make the op table smaller...
  }
}

//-----------------------------------------------------------------------------

void Z80::tock_t01(const uint8_t /*imask*/, const uint8_t /*intf*/) {
}

//-----------------------------------------------------------------------------

void Z80::tock_t23(const uint8_t /*imask*/, const uint8_t /*intf*/) {
}

//-----------------------------------------------------------------------------

void Z80::tock_t12(const uint8_t imask, const uint8_t intf) {
#ifdef FUZZ_TEST
  uint8_t z;
  {
    static uint32_t r = 1;
    r *= 0x1234567;
    r ^= r >> 16;
    z = uint8_t(r);
  }
#else
  const uint8_t z = 0;
#endif

  //--------------------------------------------------------------------------------
  // For OP_CB_HL and INC/DEC_AT_HL, we have to do a read-modify-write in two cycles. The only way that can work is if the data input latch can be made to drive the bus directly without an extra cycle...
  // so there can't be a register between the bus mux and the bus itself, which makes sense if the bus input is selected via OE or something

  if (INT) {
    uint8_t int_addr = 0;

    if      (imask & intf & INT_JOYPAD_MASK) { int_addr = 0x60; int_ack = INT_JOYPAD_MASK; }
    else if (imask & intf & INT_SERIAL_MASK) { int_addr = 0x58; int_ack = INT_SERIAL_MASK; }
    else if (imask & intf & INT_TIMER_MASK)  { int_addr = 0x50; int_ack = INT_TIMER_MASK; }
    else if (imask & intf & INT_STAT_MASK)   { int_addr = 0x48; int_ack = INT_STAT_MASK; }
    else if (imask & intf & INT_VBLANK_MASK) { int_addr = 0x40; int_ack = INT_VBLANK_MASK; }
    else                                     { int_addr = 0x00; int_ack = 0; }

    if (state == 0 && INT)                    /**/ {                                                      /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && INT)                    /**/ {                                       sp = addr - 1; /**/ bus = pch;               data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 2; }
    if (state == 2 && INT)                    /**/ {                                       sp = addr - 1; /**/ bus = pcl;               data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 3; }
    if (state == 3 && INT)                    /**/ {                                                      /**/ bus = 0;                        x = bus; addr = xy; write = 0;                /**/                                                              state_ = 4; }
    if (state == 4 && INT)                    /**/ {                                                      /**/ bus = int_addr;                 y = bus; addr = xy; write = 0;                /**/                                                              state_ = 0; }
  }                                                                                                                                                                                                                   
  else if (HALT) {                                                                                                                                                                                                    
    bool no_halt = ((imask & intf) && !ime);                                                                                                                                                                          
    if (HALT && state == 0) unhalt = 0;                                                                                                                                                                               
                                                                                                                                                                                                                      
    if (state == 0 && HALT)                   /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = !no_halt; }
    if (state == 1 && HALT)                   /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = !unhalt; }
  }                                                                                                                                                                                                                   
  else if (PREFIX_CB) {                                                                                                                                                                                               
    if (state == 1) cb = data_in;                                                                                                                                                                                     
    uint8_t mask = cb_flag_mask[CB_QUAD];                                                                                                                                                                             
                                                                                                                                                                                                                      
    if (state == 0)                           /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && OP_CB_R)                /**/ { bus = reg(CB_COL);       alu_x = bus; pc = addr + 1; /**/ bus = alu_cb(cb, f);  reg(CB_COL) = bus; addr = pc; write = 0;                /**/                                          update_flags(mask); state_ = 0; }
    if (state == 1 && OP_CB_HL)               /**/ {                                       pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2)                           /**/ { bus = data_in;           alu_x = bus;                /**/ bus = alu_cb(cb, f);     data_out = bus; addr = hl; write = 1;                /**/                                          update_flags(mask); state_ = 3; }
    if (state == 3)                           /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
  }                                                                                                                                                                                                                   
  else {                                                                                                                                                                                                              
    bool branch = false;                                                                                                                                                                                              
    switch (OP_ROW & 3) {                                                                                                                                                                                             
    case 0: branch = !(f & F_ZERO); break;                                                                                                                                                                            
    case 1: branch =  (f & F_ZERO); break;                                                                                                                                                                            
    case 2: branch = !(f & F_CARRY); break;                                                                                                                                                                           
    case 3: branch =  (f & F_CARRY); break;                                                                                                                                                                           
    }                                                                                                                                                                                                                 
                                                                                                                                                                                                                      
    // misc
    if (state == 0 && NOP)                    /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 0 && STOP)                   /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 0 && DI)                     /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 0 && EI)                     /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 0 && MV_R_R)                 /**/ { bus = reg(OP_COL); reg(OP_ROW) = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    // 8-bit alu

    if (state == 0 && ALU_A_R)                /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/ bus = reg(OP_COL);          alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(OP_ROW, f);           a = bus; update_flags(0xF0); state_ = 0; }
    if (state == 0 && ALU_A_D8)               /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && ALU_A_D8)               /**/ { bus = data_in;           alu_y = bus; pc = addr + 1; /**/ bus = alu(OP_ROW, f);           a = bus; addr = pc; write = 0;                /**/                                          update_flags(0xF0); state_ = 0; }
    if (state == 0 && ALU_A_HL)               /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && ALU_A_HL)               /**/ { bus = data_in;           alu_y = bus;                /**/ bus = alu(OP_ROW, f);           a = bus; addr = pc; write = 0;                /**/                                          update_flags(0xF0); state_ = 0; }
                                                                                                                                    
    if (state == 0 && RLC_A)                  /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/ bus = rlu(OP_ROW, f);           a = bus; update_flags(0xF0); state_ = 0; }
    if (state == 0 && RRC_A)                  /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/ bus = rlu(OP_ROW, f);           a = bus; update_flags(0xF0); state_ = 0; }
    if (state == 0 && RL_A)                   /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/ bus = rlu(OP_ROW, f);           a = bus; update_flags(0xF0); state_ = 0; }
    if (state == 0 && RR_A)                   /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/ bus = rlu(OP_ROW, f);           a = bus; update_flags(0xF0); state_ = 0; }
    if (state == 0 && DAA)                    /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/ bus = rlu(OP_ROW, f);           a = bus; update_flags(0xB0); state_ = 0; }
    if (state == 0 && CPL)                    /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/ bus = rlu(OP_ROW, f);           a = bus; update_flags(0x60); state_ = 0; }
    if (state == 0 && SCF)                    /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/ bus = rlu(OP_ROW, f);           a = bus; update_flags(0x70); state_ = 0; }
    if (state == 0 && CCF)                    /**/ { bus = a;                 alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/ bus = rlu(OP_ROW, f);           a = bus; update_flags(0x70); state_ = 0; }

    if (state == 0 && INC_R)                  /**/ { bus = reg(OP_ROW);       alu_x = bus; pc = addr + 1; /**/ bus = 1;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(0, f);      reg(OP_ROW) = bus; update_flags(0xE0); state_ = 0; }
    if (state == 0 && DEC_R)                  /**/ { bus = reg(OP_ROW);       alu_x = bus; pc = addr + 1; /**/ bus = 1;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(2, f);      reg(OP_ROW) = bus; update_flags(0xE0); state_ = 0; }

    if (state == 0 && INC_AT_HL)              /**/ { bus = 1;                 alu_y = bus; pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && INC_AT_HL)              /**/ { bus = data_in;           alu_x = bus;                /**/ bus = alu(0, f);         data_out = bus; addr = hl; write = 1;                /**/                                          update_flags(0xE0); state_ = 2; }
    if (state == 2 && INC_AT_HL)              /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && DEC_AT_HL)              /**/ { bus = 1;                 alu_y = bus; pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && DEC_AT_HL)              /**/ { bus = data_in;           alu_x = bus;                /**/ bus = alu(2, f);         data_out = bus; addr = hl; write = 1;                /**/                                          update_flags(0xE0); state_ = 2; }
    if (state == 2 && DEC_AT_HL)              /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    // 16-bit alu

    if (state == 0 && ADD_SP_R8)              /**/ { bus = p;                 alu_y = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && ADD_SP_R8)              /**/ { bus = data_in;           alu_x = bus;                /**/ bus = alu(0, f);                p = bus; addr = pc; write = 0;                /**/                                          update_flags(0xF0); state_ = 2; }
    if (state == 2 && ADD_SP_R8)              /**/ { bus = sxt(data_in);      alu_x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 3; }
    if (state == 3 && ADD_SP_R8)              /**/ { bus = s;                 alu_y = bus;                /**/ bus = alu(1, f);                s = bus; addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && LD_HL_SP_R8)            /**/ { bus = p;                 alu_y = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && LD_HL_SP_R8)            /**/ { bus = data_in;           alu_x = bus; pc = addr + 1; /**/ bus = alu(0, f);                l = bus;            write = 0;                /**/                                          update_flags(0xF0); state_ = 2; }
    if (state == 2 && LD_HL_SP_R8)            /**/ { bus = s;                 alu_y = bus;                /**/                                                     write = 0;                /**/                                                              state_ = 3; }
    if (state == 3 && LD_HL_SP_R8)            /**/ { bus = sxt(data_in);      alu_x = bus;                /**/ bus = alu(1, f);                h = bus; addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && INC_BC)                 /**/ {                                       pc = addr + 1; /**/                                          addr = bc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && INC_DE)                 /**/ {                                       pc = addr + 1; /**/                                          addr = de; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && INC_HL)                 /**/ {                                       pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && INC_SP)                 /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && INC_BC)                 /**/ {                                       bc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 1 && INC_DE)                 /**/ {                                       de = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 1 && INC_HL)                 /**/ {                                       hl = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 1 && INC_SP)                 /**/ {                                       sp = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && DEC_BC)                 /**/ {                                       pc = addr + 1; /**/                                          addr = bc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && DEC_DE)                 /**/ {                                       pc = addr + 1; /**/                                          addr = de; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && DEC_HL)                 /**/ {                                       pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && DEC_SP)                 /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && DEC_BC)                 /**/ {                                       bc = addr - 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 1 && DEC_DE)                 /**/ {                                       de = addr - 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 1 && DEC_HL)                 /**/ {                                       hl = addr - 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 1 && DEC_SP)                 /**/ {                                       sp = addr - 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    // load/store

    if (state == 0 && STM_A16_SP)             /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && STM_A16_SP)             /**/ { bus = data_in;               y = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && STM_A16_SP)             /**/ { bus = data_in;               x = bus; pc = addr + 1; /**/ bus = p;                 data_out = bus; addr = xy; write = 1;                /**/                                                              state_ = 3; }
    if (state == 3 && STM_A16_SP)             /**/ {                                       xy = addr + 1; /**/ bus = s;                 data_out = bus; addr = xy; write = 1;                /**/                                                              state_ = 4; }
    if (state == 4 && STM_A16_SP)             /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && STM_A16_A)              /**/ {                                       pc = addr + 1; /**/ bus = a;                 data_out = bus; addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && STM_A16_A)              /**/ { bus = data_in;               y = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && STM_A16_A)              /**/ { bus = data_in;               x = bus; pc = addr + 1; /**/                                          addr = xy; write = 1;                /**/                                                              state_ = 3; }
    if (state == 3 && STM_A16_A)              /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && LDM_A_A16)              /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && LDM_A_A16)              /**/ { bus = data_in;               y = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && LDM_A_A16)              /**/ { bus = data_in;               x = bus; pc = addr + 1; /**/                                          addr = xy; write = 0;                /**/                                                              state_ = 3; }
    if (state == 3 && LDM_A_A16)              /**/ { bus = data_in;               a = bus;                /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && LD_R_D8)                /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && LD_R_D8)                /**/ { bus = data_in;     reg(OP_ROW) = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && LDM_A_BC)               /**/ {                                       pc = addr + 1; /**/                                          addr = bc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && LDM_A_DE)               /**/ {                                       pc = addr + 1; /**/                                          addr = de; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && LDM_A_HLP)              /**/ {                                       pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && LDM_A_HLM)              /**/ {                                       pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && LDM_A_BC)               /**/ { bus = data_in;               a = bus;                /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 1 && LDM_A_DE)               /**/ { bus = data_in;               a = bus;                /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 1 && LDM_A_HLP)              /**/ { bus = data_in;               a = bus; hl = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 1 && LDM_A_HLM)              /**/ { bus = data_in;               a = bus; hl = addr - 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && LDM_R_HL)               /**/ {                                       pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && LDM_R_HL)               /**/ { bus = data_in;     reg(OP_ROW) = bus;                /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && LD_SP_HL)               /**/ {                                       pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && LD_SP_HL)               /**/ {                                       sp = addr + 0; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && STM_HL_R)               /**/ {                                       pc = addr + 1; /**/ bus = reg(OP_COL);       data_out = bus; addr = hl; write = 1;                /**/                                                              state_ = 1; }
    if (state == 1 && STM_HL_R)               /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && STM_HLP_A)              /**/ {                                       pc = addr + 1; /**/ bus = a;                 data_out = bus; addr = hl; write = 1;                /**/                                                              state_ = 1; }
    if (state == 1 && STM_HLP_A)              /**/ {                                       hl = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && STM_HLM_A)              /**/ {                                       pc = addr + 1; /**/ bus = a;                 data_out = bus; addr = hl; write = 1;                /**/                                                              state_ = 1; }
    if (state == 1 && STM_HLM_A)              /**/ {                                       hl = addr - 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && STM_HL_D8)              /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && STM_HL_D8)              /**/ {                                       pc = addr + 1; /**/                                          addr = hl; write = 1;                /**/                                                              state_ = 2; }
    if (state == 2 && STM_HL_D8)              /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    // indirect load/store

    if (state == 0 && STM_BC_A)               /**/ {                                       pc = addr + 1; /**/ bus = a;                 data_out = bus; addr = bc; write = 1;                /**/                                                              state_ = 1; }
    if (state == 1 && STM_BC_A)               /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && STM_DE_A)               /**/ {                                       pc = addr + 1; /**/ bus = a;                 data_out = bus; addr = de; write = 1;                /**/                                                              state_ = 1; }
    if (state == 1 && STM_DE_A)               /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    // zero-page load/store

    if (state == 0 && LDM_A_A8)               /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/ bus = 0xFF;                     x = bus;                     state_ = 1; }
    if (state == 1 && LDM_A_A8)               /**/ { bus = data_in;               y = bus; pc = addr + 1; /**/                                          addr = xy; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && LDM_A_A8)               /**/ { bus = data_in;               a = bus;                /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && STM_A8_A)               /**/ {                                       pc = addr + 1; /**/ bus = a;                 data_out = bus; addr = pc; write = 0;                /**/ bus = 0xFF;                     x = bus;                     state_ = 1; }
    if (state == 1 && STM_A8_A)               /**/ { bus = data_in;               y = bus; pc = addr + 1; /**/                                          addr = xy; write = 1;                /**/                                                              state_ = 2; }
    if (state == 2 && STM_A8_A)               /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && LDM_A_C)                /**/ {                                       pc = addr + 1; /**/                                          addr = bc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && LDM_A_C)                /**/ { bus = data_in;               a = bus;                /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (LDM_A_C  && state == 0) { ah = 0xFF; }

    if (state == 0 && STM_C_A)                /**/ {                                       pc = addr + 1; /**/ bus = a;                 data_out = bus; addr = bc; write = 1;                /**/                                                              state_ = 1; }
    if (state == 1 && STM_C_A)                /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (STM_C_A  && state == 0) { ah = 0xFF; }

    // push / pop

    if (state == 0 && PUSH_BC)                /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && PUSH_DE)                /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && PUSH_HL)                /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && PUSH_AF)                /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && PUSH_BC)                /**/ {                                       sp = addr - 1; /**/ bus = b;                 data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 2; }
    if (state == 1 && PUSH_DE)                /**/ {                                       sp = addr - 1; /**/ bus = d;                 data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 2; }
    if (state == 1 && PUSH_HL)                /**/ {                                       sp = addr - 1; /**/ bus = h;                 data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 2; }
    if (state == 1 && PUSH_AF)                /**/ {                                       sp = addr - 1; /**/ bus = a;                 data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 2; }
    if (state == 2 && PUSH_BC)                /**/ {                                       sp = addr - 1; /**/ bus = c;                 data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 3; }
    if (state == 2 && PUSH_DE)                /**/ {                                       sp = addr - 1; /**/ bus = e;                 data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 3; }
    if (state == 2 && PUSH_HL)                /**/ {                                       sp = addr - 1; /**/ bus = l;                 data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 3; }
    if (state == 2 && PUSH_AF)                /**/ {                                       sp = addr - 1; /**/ bus = f;                 data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 3; }
    if (state == 3 && PUSH_BC)                /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 3 && PUSH_DE)                /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 3 && PUSH_HL)                /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 3 && PUSH_AF)                /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && POP_BC)                 /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && POP_DE)                 /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && POP_HL)                 /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && POP_AF)                 /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && POP_BC)                 /**/ { bus = data_in;               c = bus; sp = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 2; }
    if (state == 1 && POP_DE)                 /**/ { bus = data_in;               e = bus; sp = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 2; }
    if (state == 1 && POP_HL)                 /**/ { bus = data_in;               l = bus; sp = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 2; }
    if (state == 1 && POP_AF)                 /**/ { bus = data_in;               f = bus; sp = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && POP_BC)                 /**/ { bus = data_in;               b = bus; sp = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 2 && POP_DE)                 /**/ { bus = data_in;               d = bus; sp = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 2 && POP_HL)                 /**/ { bus = data_in;               h = bus; sp = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 2 && POP_AF)                 /**/ { bus = data_in;               a = bus; sp = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    // 16-bit stuff

    if (state == 0 && LD_BC_D16)              /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && LD_DE_D16)              /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && LD_HL_D16)              /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 0 && LD_SP_D16)              /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && LD_BC_D16)              /**/ { bus = data_in;               c = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 1 && LD_DE_D16)              /**/ { bus = data_in;               e = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 1 && LD_HL_D16)              /**/ { bus = data_in;               l = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 1 && LD_SP_D16)              /**/ { bus = data_in;               p = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && LD_BC_D16)              /**/ { bus = data_in;               b = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 2 && LD_DE_D16)              /**/ { bus = data_in;               d = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 2 && LD_HL_D16)              /**/ { bus = data_in;               h = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 2 && LD_SP_D16)              /**/ { bus = data_in;               s = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && ADD_HL_BC)              /**/ { bus = c;                 alu_x = bus; pc = addr + 1; /**/ bus = l;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(0, f);                l = bus;                     state_ = 1; }
    if (state == 0 && ADD_HL_DE)              /**/ { bus = e;                 alu_x = bus; pc = addr + 1; /**/ bus = l;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(0, f);                l = bus;                     state_ = 1; }
    if (state == 0 && ADD_HL_HL)              /**/ { bus = l;                 alu_x = bus; pc = addr + 1; /**/ bus = l;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(0, f);                l = bus;                     state_ = 1; }
    if (state == 0 && ADD_HL_SP)              /**/ { bus = p;                 alu_x = bus; pc = addr + 1; /**/ bus = l;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(0, f);                l = bus;                     state_ = 1; }
    if (state == 1 && ADD_HL_BC)              /**/ { bus = b;                 alu_x = bus;                /**/ bus = h;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(1, alu_f);            h = bus; update_flags(0x70); state_ = 0; }
    if (state == 1 && ADD_HL_DE)              /**/ { bus = d;                 alu_x = bus;                /**/ bus = h;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(1, alu_f);            h = bus; update_flags(0x70); state_ = 0; }
    if (state == 1 && ADD_HL_HL)              /**/ { bus = h;                 alu_x = bus;                /**/ bus = h;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(1, alu_f);            h = bus; update_flags(0x70); state_ = 0; }
    if (state == 1 && ADD_HL_SP)              /**/ { bus = s;                 alu_x = bus;                /**/ bus = h;                    alu_y = bus; addr = pc; write = 0;                /**/ bus = alu(1, alu_f);            h = bus; update_flags(0x70); state_ = 0; }

    // jump/call/ret

    if (state == 0 && JP_HL)                  /**/ {                                       pc = addr + 1; /**/                                          addr = hl; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && JR_R8)                  /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0; pc = addr + 1; /**/ bus = pcl;                  alu_x = bus;                     state_ = 1; }
    if (state == 1 && JR_R8)                  /**/ { bus = data_in;           alu_y = bus;                /**/ bus = alu(0, f);                y = bus;            write = 0;                /**/ bus = pch;                  alu_x = bus;                     state_ = 2; }                                                         
    if (state == 2 && JR_R8)                  /**/ { bus = sxt(data_in);      alu_y = bus;                /**/ bus = alu(1, alu_f);            x = bus; addr = xy; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && JR_CC_R8 && branch)     /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0; pc = addr + 1; /**/ bus = pcl;                  alu_x = bus;                     state_ = 1; }
    if (state == 1 && JR_CC_R8 && branch)     /**/ { bus = data_in;           alu_y = bus;                /**/ bus = alu(0, f);                y = bus;            write = 0;                /**/ bus = pch;                  alu_x = bus;                     state_ = 2; }
    if (state == 2 && JR_CC_R8 && branch)     /**/ { bus = sxt(data_in);      alu_y = bus;                /**/ bus = alu(1, alu_f);            x = bus; addr = xy; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && JR_CC_R8 && !branch)    /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0; pc = addr + 1; /**/ bus = pcl;                  alu_x = bus;                     state_ = 1; }
    if (state == 1 && JR_CC_R8 && !branch)    /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && JP_CC_A16)              /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && JP_CC_A16)              /**/ { bus = data_in;               y = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && JP_CC_A16 && !branch)   /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 2 && JP_CC_A16 && branch)    /**/ { bus = data_in;               x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 3; }
    if (state == 3 && JP_CC_A16 && branch)    /**/ {                                                      /**/                                          addr = xy; write = 0;                /**/                                                              state_ = 0; }

    // why does this take four cycles? because some of the logic is shared with jp_cc_a16?                                                                                                                            
    if (state == 0 && JP_A16)                 /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && JP_A16)                 /**/ { bus = data_in;               y = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && JP_A16)                 /**/ { bus = data_in;               x = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 3; }
    if (state == 3 && JP_A16)                 /**/ {                                                      /**/                                          addr = xy; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && CALL_CC_A16)            /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && CALL_CC_A16)            /**/ { bus = data_in;               y = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && CALL_CC_A16 && branch)  /**/ { bus = data_in;               x = bus; pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 3; }
    if (state == 2 && CALL_CC_A16 && !branch) /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }
    if (state == 3 && CALL_CC_A16)            /**/ {                                       sp = addr - 1; /**/ bus = pch;               data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 4; }
    if (state == 4 && CALL_CC_A16)            /**/ {                                       sp = addr - 1; /**/ bus = pcl;               data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 5; }
    if (state == 5 && CALL_CC_A16)            /**/ {                                                      /**/                                          addr = xy; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && CALL_A16)               /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && CALL_A16)               /**/ { bus = data_in;               y = bus; pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && CALL_A16)               /**/ { bus = data_in;               x = bus; pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 3; }
    if (state == 3 && CALL_A16)               /**/ {                                       sp = addr - 1; /**/ bus = pch;               data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 4; }
    if (state == 4 && CALL_A16)               /**/ {                                       sp = addr - 1; /**/ bus = pcl;               data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 5; }
    if (state == 5 && CALL_A16)               /**/ {                                                      /**/                                          addr = xy; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && RET)                    /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && RET)                    /**/ { bus = data_in;             pcl = bus; sp = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && RET)                    /**/ { bus = data_in;             pch = bus; sp = addr + 1; /**/                                                     write = 0;                /**/                                                              state_ = 3; }
    if (state == 3 && RET)                    /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && RETI)                   /**/ {                                       pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && RETI)                   /**/ { bus = data_in;             pcl = bus; sp = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && RETI)                   /**/ { bus = data_in;             pch = bus; sp = addr + 1; /**/                                                     write = 0;                /**/                                                              state_ = 3; }
    if (state == 3 && RETI)                   /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && RET_CC && !branch)      /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && RET_CC && !branch)      /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && RET_CC && branch)       /**/ {                                       pc = addr + 1; /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && RET_CC && branch)       /**/ {                                                      /**/                                          addr = pc; write = 0;                /**/                                                              state_ = 2; }
    if (state == 2 && RET_CC)                 /**/ {                                                      /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 3; }
    if (state == 3 && RET_CC)                 /**/ { bus = data_in;               y = bus; sp = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 4; }
    if (state == 4 && RET_CC)                 /**/ { bus = data_in;               x = bus; sp = addr + 1; /**/                                          addr = xy; write = 0;                /**/                                                              state_ = 0; }

    if (state == 0 && RST_NN)                 /**/ { bus = 0x38;              alu_y = bus; pc = addr + 1; /**/                                          addr = sp; write = 0;                /**/                                                              state_ = 1; }
    if (state == 1 && RST_NN)                 /**/ {                                       sp = addr - 1; /**/ bus = pch;               data_out = bus; addr = sp; write = 1;                /**/                                                              state_ = 2; }
    if (state == 2 && RST_NN)                 /**/ {                                       sp = addr - 1; /**/ bus = pcl;               data_out = bus; addr = sp; write = 1;                /**/ bus = 0x00;                     x = bus;                     state_ = 3; }
    if (state == 3 && RST_NN)                 /**/ { bus = op;                alu_x = bus;                /**/ bus = alu(4, f);                y = bus; addr = xy; write = 0;                /**/                                                              state_ = 0; }
  }

  f &= 0xF0;

  if (RETI && state_ == 0) {ime = true;       ime_delay = true;}
  if (DI)                  {ime = false;      ime_delay = false;}
  if (EI)                  {ime = ime_delay;  ime_delay = true;}
}

//-----------------------------------------------------------------------------

uint8_t& Z80::reg(int mux) {
  switch(mux) {
  case 0: return b;
  case 1: return c;
  case 2: return d;
  case 3: return e;
  case 4: return h;
  case 5: return l;
  case 6: return data_in;
  case 7: return a;
  }
  static uint8_t fail = 0;
  return fail;
}

//-----------------------------------------------------------------------------

void Z80::update_flags(uint8_t mask) {
  f = (f & ~mask) | (alu_f & mask);
  if (ADD_SP_R8)   { f &= ~(F_ZERO | F_NEGATIVE); }
  if (LD_HL_SP_R8) { f &= ~(F_ZERO | F_NEGATIVE); }
}

//-----------------------------------------------------------------------------
#pragma warning(disable : 4458)

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

  alu_o = (uint8_t)d2;
  alu_f = (op == 2 || op == 3 || op == 7) ? F_NEGATIVE : 0;

  if (d1 & 0x010) alu_f |= F_HALF_CARRY;
  if (d2 & 0x100) alu_f |= F_CARRY;
  if (alu_o == 0x000) alu_f |= F_ZERO;
  if (op == 7)  alu_o = x;

  bus = alu_o;
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
  alu_o = uint8_t((hi << 4) | (lo & 0xF));
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
    daa(x, f);
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

  bus = alu_o;
  return alu_o;
}

//-----------------------------------------------------------------------------

uint8_t Z80::alu_cb(int op, uint8_t flags) {
  alu_f = flags;
  const uint8_t x = alu_x;
  const uint8_t quad = ((op >> 6) & 3);
  const uint8_t row = ((op >> 3) & 7);
  const bool bit_mux = (x >> row) & 1;

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

  bus = alu_o;
  return alu_o;
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4458)

void Z80::dump(std::string& o) {
  sprintf(o, "state       %d\n", state);
  sprintf(o, "\n");

  sprintf(o, "op_addr     0x%04x\n", op_addr);
  sprintf(o, "OP          0x%02x\n", op);
  sprintf(o, "CB          0x%02x\n", cb);
  sprintf(o, "\n");

  sprintf(o, "PC          0x%04x 0x%02x 0x%02x\n", pc, pcl, pch);
  sprintf(o, "BC          0x%04x 0x%02x 0x%02x\n", bc, b, c);
  sprintf(o, "DE          0x%04x 0x%02x 0x%02x\n", de, d, e);
  sprintf(o, "HL          0x%04x 0x%02x 0x%02x\n", hl, h, l);
  sprintf(o, "SP          0x%04x 0x%02x 0x%02x\n", sp, s, p);
  sprintf(o, "AF          0x%04x 0x%02x 0x%02x\n", af, a, f);
  sprintf(o, "TEMP        0x%04x 0x%02x 0x%02x\n", xy, x, y);
  sprintf(o, "alu_o       0x%02x\n", alu_o);
  sprintf(o, "alu_f       0x%02x\n", alu_f);
  sprintf(o, "\n");

  sprintf(o, "addr        0x%04x\n", addr);
  sprintf(o, "data_in     0x%02x\n", data_in);
  sprintf(o, "write       %d\n",     write);
  sprintf(o, "\n");

  sprintf(o, "IME         %d\n", ime);
  sprintf(o, "IME_        %d\n", ime_delay);
  sprintf(o, "interrupt   %d\n", INT);
  sprintf(o, "int_ack     0x%02x\n", int_ack);
}

//-----------------------------------------------------------------------------
