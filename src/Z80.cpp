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

  if (INT) {
    uint8_t int_addr = 0;

    if      (imask & intf & INT_JOYPAD_MASK) { int_addr = 0x60; int_ack = INT_JOYPAD_MASK; }
    else if (imask & intf & INT_SERIAL_MASK) { int_addr = 0x58; int_ack = INT_SERIAL_MASK; }
    else if (imask & intf & INT_TIMER_MASK)  { int_addr = 0x50; int_ack = INT_TIMER_MASK; }
    else if (imask & intf & INT_STAT_MASK)   { int_addr = 0x48; int_ack = INT_STAT_MASK; }
    else if (imask & intf & INT_VBLANK_MASK) { int_addr = 0x40; int_ack = INT_VBLANK_MASK; }
    else                                     { int_addr = 0x00; int_ack = 0; }

    if (state == 0 && INT)                         /**/ {                  alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/                          addr = sp;   write = 0;  /**/ state_ = 1; }
    if (state == 1 && INT)                         /**/ { sp = addr - 1;   alu_x = 0;            /**/ alu_y = pch;         alu_f = z;     alu_op = 0;      alu(); /**/ data_out    = alu_o;     addr = sp;   write = 1;  /**/ state_ = 2; }
    if (state == 2 && INT)                         /**/ { sp = addr - 1;   alu_x = 0;            /**/ alu_y = pcl;         alu_f = z;     alu_op = 0;      alu(); /**/ data_out    = alu_o;     addr = sp;   write = 1;  /**/ state_ = 3; }
    if (state == 3 && INT)                         /**/ {                  alu_x = 0;            /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ x           = alu_o;     addr = xy;   write = 0;  /**/ state_ = 4; }
    if (state == 4 && INT)                         /**/ {                  alu_x = 0;            /**/ alu_y = int_addr;    alu_f = z;     alu_op = 0;      alu(); /**/ y           = alu_o;     addr = xy;   write = 0;  /**/ state_ = 0; }
  }                                                                                                                                                                                                          
  else if (HALT) {                                                                                                                                                                                           
    bool no_halt = ((imask & intf) && !ime);
    if (HALT && state == 0) unhalt = 0;

    if (state == 0 && HALT)                        /**/ { pc = addr + 1;   alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/                          addr = pc;   write = 0;  /**/ state_ = !no_halt; }
    if (state == 1 && HALT)                        /**/ {                  alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/                          addr = pc;   write = 0;  /**/ state_ = !unhalt; }
  }                                                                                                                                                                                                          
  else if (PREFIX_CB) {                                                                                                                                                                                      
    if (state == 1) cb = data_in;
    uint8_t mask = cb_flag_mask[CB_QUAD];

    if (state == 0 && PREFIX_CB)                   /**/ { pc = addr + 1;   alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/                          addr = pc;   write = 0;  /**/ state_ = 1; }
    if (state == 1 && OP_CB_R)                     /**/ { pc = addr + 1;   alu_x = reg(CB_COL);  /**/ alu_y = z;           alu_f = f;     alu_op = cb;  alu_cb(); /**/ reg(CB_COL) = alu_o;     addr = pc;   write = 0;  /**/ state_ = 0; update_flags(mask); }
    if (state == 1 && OP_CB_HL)                    /**/ { pc = addr + 1;   alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/                          addr = hl;   write = 0;  /**/ state_ = 2; }
    if (state == 2 && PREFIX_CB)                   /**/ {                  alu_x = data_in;      /**/ alu_y = z;           alu_f = f;     alu_op = cb;  alu_cb(); /**/ data_out    = alu_o;     addr = hl;   write = 1;  /**/ state_ = 3; update_flags(mask); }
    if (state == 3 && PREFIX_CB)                   /**/ {                  alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/                          addr = pc;   write = 0;  /**/ state_ = 0; }
  }                                                                                                                                                                                                          
  else {                                                                                                                                                                                                     
    bool branch = false;
    switch (OP_ROW & 3) {
    case 0: branch = !(f & F_ZERO); break;
    case 1: branch =  (f & F_ZERO); break;
    case 2: branch = !(f & F_CARRY); break;
    case 3: branch =  (f & F_CARRY); break;
    }

    uint8_t bus;

    if (state == 0 && JP_HL)                  /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = hl;   write = 0; state_ = 0; }
    if (state == 0 && NOP)                    /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && STOP)                   /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && DI)                     /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && EI)                     /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && MV_R_R)                 /**/ { pc = addr + 1; bus = 0;              /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = reg(OP_COL); /**/ alu_y = bus; alu(); bus = alu_o; /**/ reg(OP_ROW) = bus;                     addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && INC_R)                  /**/ { pc = addr + 1; bus = reg(OP_ROW);    /**/ alu_x = bus; alu_f = f;     alu_op = 0;      bus = 1;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ reg(OP_ROW) = bus; update_flags(0xE0); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && DEC_R)                  /**/ { pc = addr + 1; bus = reg(OP_ROW);    /**/ alu_x = bus; alu_f = f;     alu_op = 2;      bus = 1;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ reg(OP_ROW) = bus; update_flags(0xE0); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && ALU_A_R)                /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = f;     alu_op = OP_ROW; bus = reg(OP_COL); /**/ alu_y = bus; alu(); bus = alu_o; /**/ a = bus;           update_flags(0xF0); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && RLC_A)                  /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = f;     alu_op = OP_ROW; bus = z;           /**/ alu_y = bus; rlu(); bus = alu_o; /**/ a = bus;           update_flags(0xF0); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && RRC_A)                  /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = f;     alu_op = OP_ROW; bus = z;           /**/ alu_y = bus; rlu(); bus = alu_o; /**/ a = bus;           update_flags(0xF0); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && RL_A)                   /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = f;     alu_op = OP_ROW; bus = z;           /**/ alu_y = bus; rlu(); bus = alu_o; /**/ a = bus;           update_flags(0xF0); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && RR_A)                   /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = f;     alu_op = OP_ROW; bus = z;           /**/ alu_y = bus; rlu(); bus = alu_o; /**/ a = bus;           update_flags(0xF0); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && DAA)                    /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = f;     alu_op = OP_ROW; bus = z;           /**/ alu_y = bus; rlu(); bus = alu_o; /**/ a = bus;           update_flags(0xB0); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && CPL)                    /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = f;     alu_op = OP_ROW; bus = z;           /**/ alu_y = bus; rlu(); bus = alu_o; /**/ a = bus;           update_flags(0x60); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && SCF)                    /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = f;     alu_op = OP_ROW; bus = z;           /**/ alu_y = bus; rlu(); bus = alu_o; /**/ a = bus;           update_flags(0x70); addr = pc;   write = 0; state_ = 0; }
    if (state == 0 && CCF)                    /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = f;     alu_op = OP_ROW; bus = z;           /**/ alu_y = bus; rlu(); bus = alu_o; /**/ a = bus;           update_flags(0x70); addr = pc;   write = 0; state_ = 0; }
                                                                                                                                                                                                             
    if (state == 0 && STM_A8_A)               /**/ { pc = addr + 1; bus = a;              /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ data_out = bus;                        addr = pc;   write = 0; state_ = 1; }
    if (state == 1 && STM_A8_A)               /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = xy;   write = 1; state_ = 2; }
    if (state == 2 && STM_A8_A)               /**/ {                bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 0; }

    if (state == 0 && LDM_A_A8)               /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 1; }
    if (state == 1 && LDM_A_A8)               /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = xy;   write = 0; state_ = 2; }
    if (state == 2 && LDM_A_A8)               /**/ {                bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ a = bus;                               addr = pc;   write = 0; state_ = 0; }
                                                                                                                                                                                                                                                 
    if (state == 0 && JR_R8)                  /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 1; }
    if (state == 1 && JR_R8)                  /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = pcl;         /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = addr; write = 0; state_ = 2; }
    if (state == 2 && JR_R8)                  /**/ {                bus = sxt(data_in);   /**/ alu_x = bus; alu_f = alu_f; alu_op = 1;      bus = pch;         /**/ alu_y = bus; alu(); bus = alu_o; /**/ x = bus;                               addr = xy;   write = 0; state_ = 0; }
                                                                                                                                                                                                                                                 
    if (state == 0 && JR_CC_R8)               /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 1; }
    if (state == 1 && JR_CC_R8 && branch)     /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = pcl;         /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = addr; write = 0; state_ = 2; }
    if (state == 1 && JR_CC_R8 && !branch)    /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = pcl;         /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = pc;   write = 0; state_ = 0; }
    if (state == 2 && JR_CC_R8)               /**/ {                bus = sxt(data_in);   /**/ alu_x = bus; alu_f = alu_f; alu_op = 1;      bus = pch;         /**/ alu_y = bus; alu(); bus = alu_o; /**/ x = bus;                               addr = xy;   write = 0; state_ = 0; }
                                                                                                                                                                  
    if (state == 0 && STM_A16_SP)             /**/ { pc = addr + 1; bus = 0;              /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = p;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ data_out = bus;                        addr = pc;   write = 0; state_ = 1; }
    if (state == 1 && STM_A16_SP)             /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = pc;   write = 0; state_ = 2; }
    if (state == 2 && STM_A16_SP)             /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ x = bus;                               addr = xy;   write = 1; state_ = 3; }
    if (state == 3 && STM_A16_SP)             /**/ { xy = addr + 1; bus = 0;              /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = s;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ data_out = bus;                        addr = xy;   write = 1; state_ = 4; }
    if (state == 4 && STM_A16_SP)             /**/ {                bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 0; }

    if (state == 0 && STM_A16_A)              /**/ { pc = addr + 1; bus = 0;              /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = a;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ data_out = bus;                        addr = pc;   write = 0; state_ = 1; }
    if (state == 1 && STM_A16_A)              /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = pc;   write = 0; state_ = 2; }
    if (state == 2 && STM_A16_A)              /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ x = bus;                               addr = xy;   write = 1; state_ = 3; }
    if (state == 3 && STM_A16_A)              /**/ {                bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 0; }
                                                                                                                                                                                                                                                 
    if (state == 0 && LDM_A_A16)              /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 1; }
    if (state == 1 && LDM_A_A16)              /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = pc;   write = 0; state_ = 2; }
    if (state == 2 && LDM_A_A16)              /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ x = bus;                               addr = xy;   write = 0; state_ = 3; }
    if (state == 3 && LDM_A_A16)              /**/ {                bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ a = bus;                               addr = pc;   write = 0; state_ = 0; }

    if (state == 0 && JP_A16)                 /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 1; }
    if (state == 1 && JP_A16)                 /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = pc;   write = 0; state_ = 2; }
    if (state == 2 && JP_A16)                 /**/ { pc = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ x = bus;                               addr = xy;   write = 0; state_ = 3; }
    if (state == 3 && JP_A16)                 /**/ {                bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = xy;   write = 0; state_ = 0; }

    if (state == 0 && RST_NN)                 /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = sp;   write = 0; state_ = 1; }
    if (state == 1 && RST_NN)                 /**/ { sp = addr - 1; bus = 0;              /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = pch;         /**/ alu_y = bus; alu(); bus = alu_o; /**/ data_out = bus;                        addr = sp;   write = 1; state_ = 2; }
    if (state == 2 && RST_NN)                 /**/ { sp = addr - 1; bus = 0;              /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = pcl;         /**/ alu_y = bus; alu(); bus = alu_o; /**/ data_out = bus;                        addr = sp;   write = 1; state_ = 3; }
    if (state == 3 && RST_NN)                 /**/ {                bus = op;             /**/ alu_x = bus; alu_f = z;     alu_op = 4;      bus = 0x38;        /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = xy;   write = 0; state_ = 0; }

    if (state == 0 && RET_CC && !branch)      /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 6; }
    if (state == 0 && RET_CC && branch)       /**/ { pc = addr + 1; bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 1; }
    if (state == 1 && RET_CC && branch)       /**/ {                bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = pc;   write = 0; state_ = 2; }
    if (state == 2 && RET_CC)                 /**/ {                bus = z;              /**/ alu_x = bus; alu_f = z;     alu_op = z;      bus = z;           /**/ alu_y = bus; alu(); bus = alu_o; /**/                                        addr = sp;   write = 0; state_ = 3; }
    if (state == 3 && RET_CC)                 /**/ { sp = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ y = bus;                               addr = sp;   write = 0; state_ = 4; }
    if (state == 4 && RET_CC)                 /**/ { sp = addr + 1; bus = data_in;        /**/ alu_x = bus; alu_f = z;     alu_op = 0;      bus = 0;           /**/ alu_y = bus; alu(); bus = alu_o; /**/ x = bus;                               addr = xy;   write = 0; state_ = 0; }




    if (state == 0 && LDM_A_BC)               /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = bc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && INC_BC)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = bc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && DEC_BC)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = bc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LDM_A_C)                /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = bc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LDM_A_DE)               /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = de;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && INC_DE)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = de;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && DEC_DE)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = de;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && ALU_A_HL)               /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LDM_A_HLP)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LDM_A_HLM)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && INC_HL)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && DEC_HL)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && INC_AT_HL)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && DEC_AT_HL)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LDM_R_HL)               /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LD_SP_HL)               /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && INC_SP)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && DEC_SP)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && POP_BC)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && POP_DE)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && POP_HL)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && POP_AF)                 /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && PUSH_BC)                /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && PUSH_DE)                /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && PUSH_HL)                /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && PUSH_AF)                /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && RET)                    /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && RETI)                   /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = sp;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LD_BC_D16)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LD_DE_D16)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LD_HL_D16)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LD_SP_D16)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && STM_HL_D8)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LD_R_D8)                /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && JP_CC_A16)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && CALL_CC_A16)            /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && CALL_A16)               /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && ALU_A_D8)               /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && STM_BC_A )              /**/ { pc = addr + 1; alu_x = 0;            /**/ alu_y = a;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = bc;   write = 1;  /**/ data_out    = alu_o;                         state_ = 6; }
    if (state == 0 && STM_DE_A )              /**/ { pc = addr + 1; alu_x = 0;            /**/ alu_y = a;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = de;   write = 1;  /**/ data_out    = alu_o;                         state_ = 6; }
    if (state == 0 && STM_HLP_A)              /**/ { pc = addr + 1; alu_x = 0;            /**/ alu_y = a;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = hl;   write = 1;  /**/ data_out    = alu_o;                         state_ = 1; }
    if (state == 0 && STM_HLM_A)              /**/ { pc = addr + 1; alu_x = 0;            /**/ alu_y = a;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = hl;   write = 1;  /**/ data_out    = alu_o;                         state_ = 1; }
    if (state == 0 && STM_C_A)                /**/ { pc = addr + 1; alu_x = 0;            /**/ alu_y = a;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = bc;   write = 1;  /**/ data_out    = alu_o;                         state_ = 6; }
    if (state == 0 && ADD_HL_BC)              /**/ { pc = addr + 1; alu_x = c;            /**/ alu_y = l;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ l           = alu_o;                         state_ = 1; }
    if (state == 0 && ADD_HL_DE)              /**/ { pc = addr + 1; alu_x = e;            /**/ alu_y = l;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ l           = alu_o;                         state_ = 1; }
    if (state == 0 && ADD_HL_HL)              /**/ { pc = addr + 1; alu_x = l;            /**/ alu_y = l;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ l           = alu_o;                         state_ = 1; }
    if (state == 0 && ADD_HL_SP)              /**/ { pc = addr + 1; alu_x = p;            /**/ alu_y = l;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ l           = alu_o;                         state_ = 1; }
    if (state == 0 && STM_HL_R)               /**/ { pc = addr + 1; alu_x = 0;            /**/ alu_y = reg(OP_COL); alu_f = z;     alu_op = 0;      alu(); /**/ addr = hl;   write = 1;  /**/ data_out    = alu_o;                         state_ = 6; }
    if (state == 0 && ADD_SP_R8)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 0 && LD_HL_SP_R8)            /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 1; }
    if (state == 1 && INC_AT_HL)              /**/ {                alu_x = data_in;      /**/ alu_y = 1;           alu_f = f;     alu_op = 0;      alu(); /**/ addr = hl;   write = 1;  /**/ data_out    = alu_o;     update_flags(0xE0); state_ = 2; }
    if (state == 1 && DEC_AT_HL)              /**/ {                alu_x = data_in;      /**/ alu_y = 1;           alu_f = f;     alu_op = 2;      alu(); /**/ addr = hl;   write = 1;  /**/ data_out    = alu_o;     update_flags(0xE0); state_ = 2; }
    if (state == 1 && LD_BC_D16)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ c           = alu_o;                         state_ = 2; }
    if (state == 1 && LD_DE_D16)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ e           = alu_o;                         state_ = 2; }
    if (state == 1 && LD_HL_D16)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ l           = alu_o;                         state_ = 2; }
    if (state == 1 && LD_SP_D16)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ p           = alu_o;                         state_ = 2; }
    if (state == 1 && JP_CC_A16)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ y           = alu_o;                         state_ = 2; }
    if (state == 1 && CALL_CC_A16)            /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ y           = alu_o;                         state_ = 2; }
    if (state == 1 && CALL_A16)               /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ y           = alu_o;                         state_ = 2; }
    if (state == 1 && ADD_SP_R8)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = p;           alu_f = f;     alu_op = 0;      alu(); /**/ addr = addr; write = 0;  /**/ p           = alu_o;     update_flags(0xF0); state_ = 2; }
    if (state == 1 && LD_HL_SP_R8)            /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = p;           alu_f = f;     alu_op = 0;      alu(); /**/ addr = addr; write = 0;  /**/ l           = alu_o;     update_flags(0xF0); state_ = 2; }
    if (state == 1 && POP_BC)                 /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 0;  /**/ c           = alu_o;                         state_ = 2; }
    if (state == 1 && POP_DE)                 /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 0;  /**/ e           = alu_o;                         state_ = 2; }
    if (state == 1 && POP_HL)                 /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 0;  /**/ l           = alu_o;                         state_ = 2; }
    if (state == 1 && POP_AF)                 /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 0;  /**/ f           = alu_o;                         state_ = 2; }
    if (state == 1 && RET)                    /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 0;  /**/ pcl         = alu_o;                         state_ = 2; }
    if (state == 1 && RETI)                   /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 0;  /**/ pcl         = alu_o;                         state_ = 2; }
    if (state == 1 && PUSH_BC)                /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = b;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 2; }
    if (state == 1 && PUSH_DE)                /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = d;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 2; }
    if (state == 1 && PUSH_HL)                /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = h;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 2; }
    if (state == 1 && PUSH_AF)                /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = a;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 2; }
    if (state == 1 && INC_R)                  /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 1;           alu_f = f;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ reg(OP_ROW) = alu_o;     update_flags(0xE0); state_ = 0; }
    if (state == 1 && LD_R_D8)                /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ reg(OP_ROW) = alu_o;                         state_ = 0; }
    if (state == 1 && LDM_A_HLP)              /**/ { hl = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ a           = alu_o;                         state_ = 0; }
    if (state == 1 && LDM_A_HLM)              /**/ { hl = addr - 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ a           = alu_o;                         state_ = 0; }
    if (state == 1 && LDM_R_HL)               /**/ {                alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ reg(OP_ROW) = alu_o;                         state_ = 0; }
    if (state == 1 && LDM_A_C)                /**/ {                alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ a           = alu_o;                         state_ = 0; }
    if (state == 1 && ALU_A_D8)               /**/ { pc = addr + 1; alu_x = a;            /**/ alu_y = data_in;     alu_f = f;     alu_op = OP_ROW; alu(); /**/ addr = pc;   write = 0;  /**/ a           = alu_o;     update_flags(0xF0); state_ = 0; }
    if (state == 1 && ALU_A_HL)               /**/ {                alu_x = a;            /**/ alu_y = data_in;     alu_f = f;     alu_op = OP_ROW; alu(); /**/ addr = pc;   write = 0;  /**/ a           = alu_o;     update_flags(0xF0); state_ = 0; }
    if (state == 1 && ADD_HL_BC)              /**/ {                alu_x = b;            /**/ alu_y = h;           alu_f = alu_f; alu_op = 1;      alu(); /**/ addr = pc;   write = 0;  /**/ h           = alu_o;     update_flags(0x70); state_ = 0; }
    if (state == 1 && ADD_HL_DE)              /**/ {                alu_x = d;            /**/ alu_y = h;           alu_f = alu_f; alu_op = 1;      alu(); /**/ addr = pc;   write = 0;  /**/ h           = alu_o;     update_flags(0x70); state_ = 0; }
    if (state == 1 && ADD_HL_HL)              /**/ {                alu_x = h;            /**/ alu_y = h;           alu_f = alu_f; alu_op = 1;      alu(); /**/ addr = pc;   write = 0;  /**/ h           = alu_o;     update_flags(0x70); state_ = 0; }
    if (state == 1 && ADD_HL_SP)              /**/ {                alu_x = s;            /**/ alu_y = h;           alu_f = alu_f; alu_op = 1;      alu(); /**/ addr = pc;   write = 0;  /**/ h           = alu_o;     update_flags(0x70); state_ = 0; }
    if (state == 1 && INC_BC)                 /**/ { bc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && DEC_BC)                 /**/ { bc = addr - 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && INC_DE)                 /**/ { de = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && DEC_DE)                 /**/ { de = addr - 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && INC_HL)                 /**/ { hl = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && DEC_HL)                 /**/ { hl = addr - 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && INC_SP)                 /**/ { sp = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && DEC_SP)                 /**/ { sp = addr - 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && LD_SP_HL)               /**/ { sp = addr + 0; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && STM_HLP_A)              /**/ { hl = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && STM_HLM_A)              /**/ { hl = addr - 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 1 && LDM_A_BC )              /**/ {                alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ a           = alu_o;                         state_ = 0; }
    if (state == 1 && LDM_A_DE )              /**/ {                alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ a           = alu_o;                         state_ = 0; }
    if (state == 1 && STM_HL_D8)              /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = hl;   write = 1;  /**/                                              state_ = 2; }
    if (state == 2 && ADD_SP_R8)              /**/ {                alu_x = sxt(data_in); /**/ alu_y = s;           alu_f = f;     alu_op = 1;      alu(); /**/              write = 0;  /**/ s           = alu_o;                         state_ = 6; }
    if (state == 2 && LD_HL_SP_R8)            /**/ {                alu_x = sxt(data_in); /**/ alu_y = s;           alu_f = f;     alu_op = 1;      alu(); /**/              write = 0;  /**/ h           = alu_o;                         state_ = 6; }
    if (state == 2 && PUSH_BC)                /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = c;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 6; }
    if (state == 2 && PUSH_DE)                /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = e;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 6; }
    if (state == 2 && PUSH_HL)                /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = l;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 6; }
    if (state == 2 && PUSH_AF)                /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = f;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 6; }
    if (state == 2 && RET)                    /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/              write = 0;  /**/ pch         = alu_o;                         state_ = 6; }
    if (state == 2 && RETI)                   /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/              write = 0;  /**/ pch         = alu_o;                         state_ = 6; }
    if (state == 2 && CALL_CC_A16 && branch)  /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 0;  /**/ x           = alu_o;                         state_ = 3; }
    if (state == 2 && CALL_A16)               /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 0;  /**/ x           = alu_o;                         state_ = 3; }
    if (state == 2 && LD_BC_D16)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ b           = alu_o;                         state_ = 0; }
    if (state == 2 && LD_DE_D16)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ d           = alu_o;                         state_ = 0; }
    if (state == 2 && LD_HL_D16)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ h           = alu_o;                         state_ = 0; }
    if (state == 2 && LD_SP_D16)              /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ s           = alu_o;                         state_ = 0; }
    if (state == 2 && POP_BC)                 /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ b           = alu_o;                         state_ = 0; }
    if (state == 2 && POP_DE)                 /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ d           = alu_o;                         state_ = 0; }
    if (state == 2 && POP_HL)                 /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ h           = alu_o;                         state_ = 0; }
    if (state == 2 && POP_AF)                 /**/ { sp = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ a           = alu_o;                         state_ = 0; }
    if (state == 2 && JP_CC_A16 && !branch)   /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 2 && CALL_CC_A16 && !branch) /**/ { pc = addr + 1; alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 2 && JP_CC_A16 && branch)    /**/ { pc = addr + 1; alu_x = data_in;      /**/ alu_y = 0;           alu_f = z;     alu_op = 0;      alu(); /**/ addr = pc;   write = 0;  /**/ x           = alu_o;                         state_ = 5; }
    if (state == 2 && INC_AT_HL)              /**/ {                alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 2 && DEC_AT_HL)              /**/ {                alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 2 && STM_HL_D8)              /**/ {                alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
    if (state == 3 && CALL_CC_A16)            /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = pch;         alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 4; }
    if (state == 3 && CALL_A16)               /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = pch;         alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 4; }
    if (state == 4 && CALL_CC_A16)            /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = pcl;         alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 5; }
    if (state == 4 && CALL_A16)               /**/ { sp = addr - 1; alu_x = 0;            /**/ alu_y = pcl;         alu_f = z;     alu_op = 0;      alu(); /**/ addr = sp;   write = 1;  /**/ data_out    = alu_o;                         state_ = 5; }
    if (state == 5)                           /**/ {                alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = xy;   write = 0;  /**/                                              state_ = 0; }
    if (state == 6)                           /**/ {                alu_x = z;            /**/ alu_y = z;           alu_f = z;     alu_op = z;      alu(); /**/ addr = pc;   write = 0;  /**/                                              state_ = 0; }
  }

  f &= 0xF0;

  if (STM_A8_A && state == 1) { ah = 0xFF; }
  if (LDM_A_A8 && state == 1) { ah = 0xFF; }
  if (STM_C_A  && state == 0) { ah = 0xFF; }
  if (LDM_A_C  && state == 0) { ah = 0xFF; }
  if (RST_NN   && state == 3) { ah = 0x00; }

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

void Z80::alu() {
  const uint8_t x = alu_x;
  const uint8_t y = alu_y;

  uint16_t d1 = 0, d2 = 0;
  const bool c = alu_f & F_CARRY;

  switch(alu_op) {
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
  alu_f = (alu_op == 2 || alu_op == 3 || alu_op == 7) ? F_NEGATIVE : 0;

  if (d1 & 0x010) alu_f |= F_HALF_CARRY;
  if (d2 & 0x100) alu_f |= F_CARRY;
  if (alu_o == 0x000) alu_f |= F_ZERO;
  if (alu_op == 7)  alu_o = x;
}

//-----------------------------------------------------------------------------
// The logic is more annoying, but this can be implemented as two 4-bit additions

void Z80::daa(uint8_t x, uint8_t f) {
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
}

//-----------------------------------------------------------------------------

void Z80::rlu() {
  const uint8_t x = alu_x;
  const uint8_t f = alu_f;

  switch (alu_op) {
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
}

//-----------------------------------------------------------------------------

void Z80::alu_cb() {
  const uint8_t x = alu_x;
  const uint8_t f = alu_f;
  const uint8_t quad = CB_QUAD;
  const uint8_t row = CB_ROW;
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
