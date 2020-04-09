#include "Z80.h"

#include "Constants.h"

#include <assert.h>
#include <imgui.h>

#pragma warning(disable : 4100)

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

#define RLU_R         ((op & 0b11000111) == 0b00000111)


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

#define ALU_A_D8      ((OP_QUAD == 3) && (OP_COL == 6))
#define RST_NN        ((OP_QUAD == 3) && (OP_COL == 7))

uint8_t  sxt(uint8_t x) { return x & 0x80 ? 0xFF : 0x00; }

// STM_A8_A 0xE0 0b11100000
// LDM_A_A8 0xF0 0b11110000
// STM_C_A  0xE2 0b11100010
// LDM_A_C  0xF2 0b11110010

//          op & 0b11101101 == 0b11100000

//-----------------------------------------------------------------------------

void Z80::reset(uint16_t new_pc) {

  if (new_pc == 0x100) {
    unhalt = 0;
    ime = ime_delay = 0;
    interrupt = 0;

    state = 0;
             
    write = 0;
    int_ack = 0;

    pc = 0xFF;
    op_addr = pc;
    op = 0;
    cb = 0;
    addr = 0xFF;
    data = 0;
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
    interrupt = 0;

    state = 0;
             
    write = 0;
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

Req Z80::get_bus_req_t30() const {
  return {
    .addr  = addr,
    .data  = data,
    .read  = (bool)!write,
    .write = (bool)write,
  };
}

void Z80::on_bus_ack_t01(Ack ibus_ack_) {
  data = (uint8_t)ibus_ack_.data;
}

//-----------------------------------------------------------------------------

void Z80::update_flags() {
  uint8_t f_ = alu_f;
  uint8_t mask = PREFIX_CB ? cb_flag_mask[CB_QUAD] : flag_mask[op];
  f = (f & ~mask) | (f_ & mask);

  // RLCA, RRCA, RLA, and RRA always clear the zero bit - hardware bug?
  if ((op & 0b11100111) == 0b00000111) f &= ~F_ZERO;

  // ADD_SP_R8 and LD_HL_SP_R8 always clear the zero bit and negative bit.
  if ((op & 0b11101111) == 0b11101000) f &= ~(F_ZERO | F_NEGATIVE);

  alu_f = f;
}

//-----------------------------------------------------------------------------
// Do the meat of executing the instruction

#pragma warning(disable:4189)

void Z80::tock_t30(const uint8_t imask, const uint8_t intf) {
  alu_f &= 0xF0;
  f &= 0xF0;

  if (state_ == -1) __debugbreak();

  state = state_;
  ime = ime_delay;

  if (state == 0) {
    op_addr = addr;
    alu_f = f;
  }

  if (state == 0) {
    op = data;
    int_ack = 0;
    interrupt = (imask & intf) && ime;
  }
   
  if (PREFIX_CB && state == 1) cb = data;

  if (state == 0 && interrupt) {
    ime = false; ime_delay = false;
  }

  write = 0;
  state_ = -1;
}

//-----------------------------------------------------------------------------

void Z80::tock_t01(const uint8_t imask, const uint8_t intf) {
}

//-----------------------------------------------------------------------------

void Z80::tock_t12(const uint8_t imask, const uint8_t intf) {
  uint16_t abus = 0;
  uint8_t bus = 0;

  bool cond_fail = false;
  switch (OP_ROW & 3) {
  case 0: cond_fail = (f & F_ZERO); break;
  case 1: cond_fail = !(f & F_ZERO); break;
  case 2: cond_fail = (f & F_CARRY); break;
  case 3: cond_fail = !(f & F_CARRY); break;
  }
  bool nb = cond_fail;
  bool tb = !cond_fail;

  bool no_halt = ((imask & intf) && !ime);

  if (HALT && state == 0) unhalt = 0;

  // zeropage read/write
  if ((op & 0b11101101) == 0b11100000) x = 0xFF;

  // pc update _must_ happen in tcycle 0 of state 0, because if an interrupt fires it should _not_ happen.

  //----------
  // INTERRUPTS

  if (interrupt) {
    uint8_t int_addr = 0;
    uint8_t which_ack = 0;

    if      (imask & intf & INT_JOYPAD_MASK) { int_addr = 0x60; which_ack = INT_JOYPAD_MASK; }
    else if (imask & intf & INT_SERIAL_MASK) { int_addr = 0x58; which_ack = INT_SERIAL_MASK; }
    else if (imask & intf & INT_TIMER_MASK)  { int_addr = 0x50; which_ack = INT_TIMER_MASK; }
    else if (imask & intf & INT_STAT_MASK)   { int_addr = 0x48; which_ack = INT_STAT_MASK; }
    else if (imask & intf & INT_VBLANK_MASK) { int_addr = 0x40; which_ack = INT_VBLANK_MASK; }
    else                                     { int_addr = 0x00; which_ack = 0; }

    if      (state == 0)       /**/ {                                              /**/                                            /**/                          addr = sp; write = 0; /**/ state_ = 1; }
    else if (state == 1)       /**/ { sp = addr - 1;                               /**/                                            /**/ data = pch;              addr = sp; write = 1; /**/ state_ = 2; }
    else if (state == 2)       /**/ { sp = addr - 1;                               /**/                                            /**/ data = pcl;              addr = sp; write = 1; /**/ state_ = 3; }
    else if (state == 3)       /**/ {                  alu_y = 0;                  /**/ alu_x = 0x00;                              /**/ x = alu_x;               addr = xy; write = 0; /**/ state_ = 4; }
    else if (state == 4)       /**/ {                  alu_y = 0;                  /**/ alu_x = int_addr;                          /**/ y = alu_x;               addr = xy; write = 0; /**/ state_ = 0; int_ack = which_ack;  }
  }                            /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
  //----------                 /**/                                                /**/                                            /**/                                                 /**/                  
  // CB OPS                    /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
  else if (PREFIX_CB) {        /**/                                                /**/                                            /**/                                                 /**/                  
    if      (state == 0)       /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 1; }
    else if (state == 1) {     /**/                                                /**/                                            /**/                                                 /**/ 
      if (OP_CB_R  )           /**/ { pc = addr + 1;   alu_x = reg_get8(CB_COL);   /**/ alu_y = 0;          alu_op = cb; alu_cb(); /**/ reg_put8(CB_COL, alu_o); addr = pc; write = 0;  /**/ state_ = 0; update_flags();  }
      if (OP_CB_HL )           /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = hl; write = 0;  /**/ state_ = 2; }
    }                          /**/                                                /**/                                            /**/                                                 /**/                  
    else if (state == 2)       /**/ {                  alu_x = data;               /**/ alu_y = 0;          alu_op = cb; alu_cb(); /**/ data = alu_o;            addr = hl; write = 1;  /**/ state_ = 3; }
    else if (state == 3)       /**/ {                                              /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; update_flags();  }
  }                            /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
  //----------                 /**/                                                /**/                                            /**/                                                 /**/                  
  // REGULAR OPS               /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
  else {                       /**/                                                /**/                                            /**/                                                 /**/                  
    //----------               /**/                                                /**/                                            /**/                                                 /**/                  
    // QUAD 0                  /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
    if (OP_QUAD == 0) {        /**/                                                /**/                                            /**/                                                 /**/                  
      if (OP_COL == 0) {       /**/                                                /**/                                            /**/                                                 /**/                  
        if      (NOP)          /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
        else if (STM_A16_SP) { /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;          alu_op = 0; alu();     /**/ y = alu_o;               addr = pc; write = 0;  /**/ state_ = 2; }
          else if (state == 2) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;          alu_op = 0; alu();     /**/ x = alu_o; data = p;     addr = xy; write = 1;  /**/ state_ = 3; }
          else if (state == 3) /**/ { xy = addr + 1;                               /**/                                            /**/            data = s;     addr = xy; write = 1;  /**/ state_ = 4; }
          else if (state == 4) /**/ {                                              /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
        else if (STOP)         /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
        else if (JR_R8) {      /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = pcl;        alu_op = 0; alu();     /**/ pcl = alu_o;             addr = pc; write = 0;  /**/ state_ = 2; }
          else if (state == 2) /**/ {                  alu_x = sxt(alu_x);         /**/ alu_y = pch;        alu_op = 1; alu();     /**/ pch = alu_o;             addr = pc; write = 0;  /**/ state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
        else if (JR_CC_R8) {   /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = pcl;        alu_op = 0; alu();     /**/ y = alu_o;               addr = pc; write = 0;  /**/ state_ = cond_fail ? 0 : 2; }
          else if (state == 2) /**/ {                  alu_x = sxt(alu_x);         /**/ alu_y = pch;        alu_op = 1; alu();     /**/ x = alu_o;               addr = xy; write = 0;  /**/ state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                      
      }                        /**/                                                /**/                                            /**/                                                 /**/                      
      else if (OP_COL == 1) {  /**/                                                /**/                                            /**/                                                 /**/                      
        if      (LD_BC_D16) {  /**/                                                /**/                                            /**/                                                 /**/                      
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ c = alu_x;               addr = pc; write = 0;  /**/ state_ = 2; }
          else if (state == 2) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ b = alu_x;               addr = pc; write = 0;  /**/ state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/ 
        else if (LD_DE_D16) {  /**/                                                /**/                                            /**/                                                 /**/ 
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ e = alu_x;               addr = pc; write = 0;  /**/ state_ = 2; }
          else if (state == 2) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ d = alu_x;               addr = pc; write = 0;  /**/ state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/ 
        else if (LD_HL_D16) {  /**/                                                /**/                                            /**/                                                 /**/ 
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ l = alu_x;               addr = pc; write = 0;  /**/ state_ = 2; }
          else if (state == 2) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ h = alu_x;               addr = pc; write = 0;  /**/ state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/ 
        else if (LD_SP_D16) {  /**/                                                /**/                                            /**/                                                 /**/ 
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ p = alu_x;               addr = pc; write = 0;  /**/ state_ = 2; }
          else if (state == 2) /**/ { pc = addr + 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ s = alu_x;               addr = pc; write = 0;  /**/ state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                                              
        else if (ADD_HL_BC) {  /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;   alu_x = c;                  /**/ alu_y = l;          alu_op = 0; alu();     /**/ l = alu_o;               addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ {                  alu_x = b;                  /**/ alu_y = h;          alu_op = 1; alu();     /**/ h = alu_o;               addr = pc; write = 0;  /**/ state_ = 0; update_flags(); }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
        else if (ADD_HL_DE) {  /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;   alu_x = e;                  /**/ alu_y = l;          alu_op = 0; alu();     /**/ l = alu_o;               addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ {                  alu_x = d;                  /**/ alu_y = h;          alu_op = 1; alu();     /**/ h = alu_o;               addr = pc; write = 0;  /**/ state_ = 0; update_flags(); }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
        else if (ADD_HL_HL) {  /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;   alu_x = l;                  /**/ alu_y = l;          alu_op = 0; alu();     /**/ l = alu_o;               addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ {                  alu_x = h;                  /**/ alu_y = h;          alu_op = 1; alu();     /**/ h = alu_o;               addr = pc; write = 0;  /**/ state_ = 0; update_flags(); }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
        else if (ADD_HL_SP) {  /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;   alu_x = p;                  /**/ alu_y = l;          alu_op = 0; alu();     /**/ l = alu_o;               addr = pc; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ {                  alu_x = s;                  /**/ alu_y = h;          alu_op = 1; alu();     /**/ h = alu_o;               addr = pc; write = 0;  /**/ state_ = 0; update_flags(); }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (OP_COL == 2) {  /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0) {      /**/                                                /**/                                            /**/                                                 /**/                  
          if      (STM_BC_A )  /**/ { pc = addr + 1;                               /**/                                            /**/ data = a;                addr = bc; write = 1;  /**/ state_ = 1; }
          else if (STM_DE_A )  /**/ { pc = addr + 1;                               /**/                                            /**/ data = a;                addr = de; write = 1;  /**/ state_ = 1; }
          else if (STM_HLP_A)  /**/ { pc = addr + 1;                               /**/                                            /**/ data = a;                addr = hl; write = 1;  /**/ state_ = 1; }
          else if (STM_HLM_A)  /**/ { pc = addr + 1;                               /**/                                            /**/ data = a;                addr = hl; write = 1;  /**/ state_ = 1; }
          else if (LDM_A_BC )  /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = bc; write = 0;  /**/ state_ = 1; }
          else if (LDM_A_DE )  /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = de; write = 0;  /**/ state_ = 1; }
          else if (LDM_A_HLP)  /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = hl; write = 0;  /**/ state_ = 1; }
          else if (LDM_A_HLM)  /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = hl; write = 0;  /**/ state_ = 1; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
        else if (state == 1) { /**/                                                /**/                                            /**/                                                 /**/                  
          if      (STM_BC_A )  /**/ {                                              /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (STM_DE_A )  /**/ {                                              /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (STM_HLP_A)  /**/ { hl = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (STM_HLM_A)  /**/ { hl = addr - 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (LDM_A_BC )  /**/ {                  alu_x = data;               /**/ alu_y = 0;                                 /**/ a = alu_x;               addr = pc; write = 0;  /**/ state_ = 0; }
          else if (LDM_A_DE )  /**/ {                  alu_x = data;               /**/ alu_y = 0;                                 /**/ a = alu_x;               addr = pc; write = 0;  /**/ state_ = 0; }
          else if (LDM_A_HLP)  /**/ { hl = addr + 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ a = alu_x;               addr = pc; write = 0;  /**/ state_ = 0; }
          else if (LDM_A_HLM)  /**/ { hl = addr - 1;   alu_x = data;               /**/ alu_y = 0;                                 /**/ a = alu_x;               addr = pc; write = 0;  /**/ state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/ 
      }                        /**/                                                /**/                                            /**/                                                 /**/ 
      else if (OP_COL == 3) {  /**/                                                /**/                                            /**/                                                 /**/ 
        if (state == 0) {      /**/                                                /**/                                            /**/                                                 /**/ 
          if      (INC_BC)     /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = bc; write = 0;  /**/ state_ = 1; }
          else if (DEC_BC)     /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = bc; write = 0;  /**/ state_ = 1; }
          else if (INC_DE)     /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = de; write = 0;  /**/ state_ = 1; }
          else if (DEC_DE)     /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = de; write = 0;  /**/ state_ = 1; }
          else if (INC_HL)     /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = hl; write = 0;  /**/ state_ = 1; }
          else if (DEC_HL)     /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = hl; write = 0;  /**/ state_ = 1; }
          else if (INC_SP)     /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = sp; write = 0;  /**/ state_ = 1; }
          else if (DEC_SP)     /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = sp; write = 0;  /**/ state_ = 1; }
        }                      /**/                                                /**/                                            /**/                                                 /**/ 
        else if (state == 1) { /**/                                                /**/                                            /**/                                                 /**/ 
          if      (INC_BC)     /**/ { bc = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (DEC_BC)     /**/ { bc = addr - 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (INC_DE)     /**/ { de = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (DEC_DE)     /**/ { de = addr - 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (INC_HL)     /**/ { hl = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (DEC_HL)     /**/ { hl = addr - 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (INC_SP)     /**/ { sp = addr + 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
          else if (DEC_SP)     /**/ { sp = addr - 1;                               /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (OP_COL == 4) {  /**/                                                /**/                                            /**/                                                 /**/                  
        if (INC_AT_HL) {       /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = hl; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ {                  alu_x = data;               /**/ alu_y = 1;          alu_op = 0; alu();     /**/ data = alu_o;            addr = hl; write = 1;  /**/ state_ = 2; }
          else if (state == 2) /**/ {                                              /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; update_flags(); }
        }                      /**/                                                /**/                                            /**/                                                 /**/                                        
        else if (INC_R)        /**/ { pc = addr + 1;   alu_x = reg_get8();         /**/ alu_y = 1;          alu_op = 0; alu();     /**/ reg_put8(alu_o);         addr = pc; write = 0;  /**/ state_ = 0; update_flags(); }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (OP_COL == 5) {  /**/                                                /**/                                            /**/                                                 /**/                  
        if (DEC_AT_HL) {       /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                          addr = hl; write = 0;  /**/ state_ = 1; }
          else if (state == 1) /**/ {                  alu_x = data;               /**/ alu_y = 1;          alu_op = 2; alu();     /**/ data = alu_o;            addr = hl; write = 1;  /**/ state_ = 2; }
          else if (state == 2) /**/ {                                              /**/                                            /**/                          addr = pc; write = 0;  /**/ state_ = 0; update_flags(); }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
        else if (DEC_R)        /**/ { pc = addr + 1;   alu_x = reg_get8();         /**/ alu_y = 1;          alu_op = 2; alu();     /**/ reg_put8(alu_o);         addr = pc; write = 0;  /**/ state_ = 0; update_flags(); }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (OP_COL == 6) {  /**/                                                /**/                                            /**/                                                 /**/                  
        if (STM_HL_D8) {       /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
          else if (state == 1) /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = hl; write = 1; state_ = 2; }
          else if (state == 2) /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
        else if (LD_R_D8) {    /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
          else if (state == 1) /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ reg_put8(alu_x);                                /**/                  addr = pc; write = 0; state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (OP_COL == 7) {  /**/                                                /**/                                            /**/                                                 /**/                  
        if (RLU_R) {           /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 0) /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = reg_get8();                        /**/ alu_op = OP_ROW; rlu(); a = alu_o;              /**/ update_flags();  addr = pc; write = 0; state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
    }                          /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
    //----------               /**/                                                /**/                                            /**/                                                 /**/                  
    // QUAD 1                  /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
    else if (OP_QUAD == 1) {   /**/                                                /**/                                            /**/                                                 /**/                  
      if (HALT) {              /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = !no_halt; }
        else if (state == 1)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = !unhalt; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (OP_COL == 6) {  /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = hl; write = 0; state_ = 1; }
        else if (state == 1)   /**/ {                  alu_y = 0;                  /**/ alu_x = data;                              /**/ reg_put8(alu_x);                                /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (OP_ROW == 6) {  /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = reg_get8();                        /**/                                                 /**/ data = alu_x;    addr = hl; write = 1; state_ = 1; }
        else if (state == 1)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else {                   /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = reg_get8();                        /**/ reg_put8(alu_x);                                /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
    }                          /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
    //----------               /**/                                                /**/                                            /**/                                                 /**/                  
    // QUAD 2                  /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
    else if (OP_QUAD == 2) {   /**/                                                /**/                                            /**/                                                 /**/                  
      if (OP_COL == 6) {       /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = hl; write = 0; state_ = 1; }
        if (state == 1)        /**/ {                  alu_y = reg_get8();         /**/ alu_x = a;                                 /**/ alu_op = OP_ROW; alu(); a = alu_o;              /**/ update_flags();  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else {                   /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;   alu_y = reg_get8();         /**/ alu_x = a;                                 /**/ alu_op = OP_ROW; alu(); a = alu_o;              /**/ update_flags();  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
    }                          /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
    //----------               /**/                                                /**/                                            /**/                                                 /**/                  
    // QUAD 3                  /**/                                                /**/                                            /**/                                                 /**/                  
                               /**/                                                /**/                                            /**/                                                 /**/                  
    else if (OP_QUAD == 3) {   /**/                                                /**/                                            /**/                                                 /**/                  
      if (POP_BC) {            /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ c = alu_x;                                      /**/                  addr = sp; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ b = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (POP_DE) {       /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ e = alu_x;                                      /**/                  addr = sp; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ d = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (POP_HL) {       /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ l = alu_x;                                      /**/                  addr = sp; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ h = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (POP_AF) {       /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ f = alu_x;                                      /**/                  addr = sp; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ a = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (PUSH_BC) {      /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;   alu_y = 0;                  /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr - 1;   alu_y = 0;                  /**/ alu_x = b;                                 /**/                                                 /**/ data = alu_x;    addr = sp; write = 1; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr - 1;                               /**/ alu_x = c;                                 /**/                                                 /**/ data = alu_x;    addr = sp; write = 1; state_ = 3; }
        else if (state == 3)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (PUSH_DE) {      /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr - 1;   alu_y = 0;                  /**/ alu_x = d;                                 /**/                                                 /**/ data = alu_x;    addr = sp; write = 1; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr - 1;   alu_y = 0;                  /**/ alu_x = e;                                 /**/                                                 /**/ data = alu_x;    addr = sp; write = 1; state_ = 3; }
        else if (state == 3)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (PUSH_HL) {      /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr - 1;   alu_y = 0;                  /**/ alu_x = h;                                 /**/                                                 /**/ data = alu_x;    addr = sp; write = 1; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr - 1;   alu_y = 0;                  /**/ alu_x = l;                                 /**/                                                 /**/ data = alu_x;    addr = sp; write = 1; state_ = 3; }
        else if (state == 3)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (PUSH_AF) {      /**/                                                /**/                                            /**/                                                 /**/                  
        if (state == 0)        /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr - 1;   alu_y = 0;                  /**/ alu_x = a;                                 /**/                                                 /**/ data = alu_x;    addr = sp; write = 1; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr - 1;   alu_y = 0;                  /**/ alu_x = f;                                 /**/                                                 /**/ data = alu_x;    addr = sp; write = 1; state_ = 3; }
        else if (state == 3)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (STM_A8_A) {     /**/                                                /**/                                            /**/                                                 /**/                  
        // yeah how do we get the "data = a" out of the last column?                                                               
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ y = alu_x;                                      /**/ data = a;        addr = xy; write = 1; state_ = 2; }
        else if (state == 2)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (ADD_SP_R8) {    /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = p;                  /**/ alu_x = data;                              /**/ alu_op = 0; alu(); y = alu_o;                   /**/ update_flags();  addr = pc; write = 0; state_ = 2; }
        else if (state == 2)   /**/ {                  alu_y = s;                  /**/ alu_x = sxt(alu_x);                        /**/ alu_op = 1; alu(); x = alu_o;                   /**/                  addr = xy; write = 0; state_ = 3; }
        else if (state == 3)   /**/ { sp = addr;                                   /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (LDM_A_A8) {     /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ y = alu_x;                                      /**/                  addr = xy; write = 0; state_ = 2; }
        else if (state == 2)   /**/ {                  alu_y = 0;                  /**/ alu_x = data;                              /**/ a = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (LD_HL_SP_R8) {  /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = p;                  /**/ alu_x = data;                              /**/ alu_op = 0; alu(); l = alu_o;                   /**/ update_flags();  addr = pc; write = 0; state_ = 2; }
        else if (state == 2)   /**/ {                  alu_y = s;                  /**/ alu_x = sxt(alu_x);                        /**/ alu_op = 1; alu(); h = alu_o;                   /**/                  addr = pc; write = 0; state_ = 3; }
        else if (state == 3)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (RET_CC) {       /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = cond_fail ? 0 : 2; }
        else if (state == 2)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 3; }
        else if (state == 3)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ y = alu_x;                                      /**/                  addr = sp; write = 0; state_ = 4; }
        else if (state == 4)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ x = alu_x;                                      /**/                  addr = xy; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (RET) {          /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ pcl = alu_x;                                    /**/                  addr = sp; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ pch = alu_x;                                    /**/                  addr = pc; write = 0; state_ = 3; }
        else if (state == 3)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (RETI) {         /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ pcl = alu_x;                                    /**/                  addr = sp; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ pch = alu_x;                                    /**/                  addr = pc; write = 0; state_ = 3; }
        else if (state == 3)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (JP_HL)          /**/ { pc = hl;                                     /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      else if (LD_SP_HL) {     /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = hl; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr;                                   /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (JP_CC_A16) {    /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ y = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ x = alu_x;                                      /**/                  addr = pc; write = 0; state_ = cond_fail ? 0 : 3; }
        else if (state == 3)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = xy; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (STM_C_A) {      /**/                                                /**/                                            /**/                                                 /**/                  
                                                                                                                                   
        // not sure if this one fits the pattern...                                                                                
        if      (state == 0)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = c;                                 /**/ y = alu_x;                                      /**/ data = a;        addr = xy; write = 1; state_ = 1; }
        else if (state == 1)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (STM_A16_A) {    /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ y = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ x = alu_x;                                      /**/ data = a;        addr = xy; write = 1; state_ = 3; }
        else if (state == 3)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (LDM_A_C) {      /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = c;                                 /**/ y = alu_x;                                      /**/                  addr = xy; write = 0; state_ = 1; }
        else if (state == 1)   /**/ {                  alu_y = 0;                  /**/ alu_x = data;                              /**/ a = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (LDM_A_A16) {    /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ y = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ x = alu_x;                                      /**/                  addr = xy; write = 0; state_ = 3; }
        else if (state == 3)   /**/ {                  alu_y = 0;                  /**/ alu_x = data;                              /**/ a = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (JP_A16) {       /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ y = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ x = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 3; }
        else if (state == 3)   /**/ { pc = xy;                                     /**/                                            /**/                                                 /**/                  addr = xy; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (DI)             /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      else if (EI)             /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 0; }
      else if (CALL_CC_A16) {  /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ y = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 2; }
                               /**/                                                /**/                                            /**/                                                 /**/                  
        if (nb) {              /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 2) /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ x = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
        else {                 /**/                                                /**/                                            /**/                                                 /**/                  
          if      (state == 2) /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ x = alu_x;                                      /**/                  addr = sp; write = 0; state_ = 3; }
          else if (state == 3) /**/ { sp = addr - 1;                               /**/                                            /**/                                                 /**/ data = pch;      addr = sp; write = 1; state_ = 4; }
          else if (state == 4) /**/ { sp = addr - 1;                               /**/                                            /**/                                                 /**/ data = pcl;      addr = sp; write = 1; state_ = 5; }
          else if (state == 5) /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = xy; write = 0; state_ = 0; }
        }                      /**/                                                /**/                                            /**/                                                 /**/                  
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (CALL_A16) {     /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ y = alu_x;                                      /**/                  addr = pc; write = 0; state_ = 2; }
        else if (state == 2)   /**/ { pc = addr + 1;   alu_y = 0;                  /**/ alu_x = data;                              /**/ x = alu_x;                                      /**/                  addr = sp; write = 0; state_ = 3; }
        else if (state == 3)   /**/ { sp = addr - 1;                               /**/                                            /**/                                                 /**/ data = pch;      addr = sp; write = 1; state_ = 4; }
        else if (state == 4)   /**/ { sp = addr - 1;                               /**/                                            /**/                                                 /**/ data = pcl;      addr = sp; write = 1; state_ = 5; }
        else if (state == 5)   /**/ {                                              /**/                                            /**/                                                 /**/                  addr = xy; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (ALU_A_D8) {     /**/                                                /**/                                            /**/                                                 /**/                  
        // hmmmmmmm this one may also not fit the pat tern.... does the alu need a "swap inputs" control?                          
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = pc; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { pc = addr + 1;   alu_x = a;                  /**/ alu_y = data;                              /**/ alu_op = OP_ROW; alu(); a = alu_o;              /**/ update_flags();  addr = pc; write = 0; state_ = 0; }
      }                        /**/                                                /**/                                            /**/                                                 /**/                  
      else if (RST_NN) {       /**/                                                /**/                                            /**/                                                 /**/                  
        if      (state == 0)   /**/ { pc = addr + 1;                               /**/                                            /**/                                                 /**/                  addr = sp; write = 0; state_ = 1; }
        else if (state == 1)   /**/ { sp = addr - 1;                               /**/                                            /**/                                                 /**/ data = pch;      addr = sp; write = 1; state_ = 2; }
        else if (state == 2)   /**/ { sp = addr - 1;   alu_y = 0;                  /**/ alu_x = 0;                                 /**/ x = alu_x;                                      /**/ data = pcl;      addr = sp; write = 1; state_ = 3; }
        else if (state == 3)   /**/ {                  alu_y = 0x38;               /**/ alu_x = op;                                /**/ y = alu_y & alu_x;                              /**/                  addr = xy; write = 0; state_ = 0; }
      }
    }
  }

  if (RETI && state_ == 0) {ime = true; ime_delay = true;}
  if (DI)                  {ime = false;      ime_delay = false;}
  if (EI)                  {ime = ime_delay;  ime_delay = true;}
}

//-----------------------------------------------------------------------------

void Z80::tock_t23(const uint8_t imask, const uint8_t intf) {
}

//-----------------------------------------------------------------------------

uint8_t Z80::reg_get8(int mux) const {
  switch(mux) {
  case 0: return b;
  case 1: return c;
  case 2: return d;
  case 3: return e;
  case 4: return h;
  case 5: return l;
  case 6: return data;
  case 7: return a;
  }
  return 0;
}

void Z80::reg_put8(int mux, uint8_t reg) {
  switch (mux) {
  case 0: b = reg; return;
  case 1: c = reg; return;
  case 2: d = reg; return;
  case 3: e = reg; return;
  case 4: h = reg; return;
  case 5: l = reg; return;
  case 6: data = reg; return;
  case 7: a = reg; return;
  }
  return;
}

uint8_t Z80::reg_get8() const {
  int mux = (op & 0b11000000) ? OP_COL : OP_ROW;
  if (RLU_R) mux = OP_COL;
  return reg_get8(mux);
}

void Z80::reg_put8(uint8_t reg) {
  return reg_put8(OP_ROW, reg);
}

//-----------------------------------------------------------------------------
// 4-bit-at-a-time ALU just for amusement

#if 0

uint8_t Z80::alu4(const uint8_t op, const uint8_t a, const uint8_t b, const uint8_t c) {
  switch (op) {
  case 0: return a + b + c; // add
  case 1: return a + b + c; // adc
  case 2: return a - b - c; // sub
  case 3: return a - b - c; // sbc
  case 4: return a & b;     // and
  case 5: return a ^ b;     // xor
  case 6: return a | b;     // or
  case 7: return a - b - c; // cp
  }
  return 0;
}

AluOut Z80::alu(const uint8_t op, const uint8_t x, const uint8_t y, const uint8_t f) {
  uint8_t c1 = (op == 0 || op == 2 || op == 7) ? 0 : (f >> 4) & 1;
  uint8_t d1 = alu4(op, x & 0xF, y & 0xF, c1);

  uint8_t c2 = (op == 4) ? 1 : (d1 >> 4) & 1;
  uint8_t d2 = alu4(op, x >> 4, y >> 4, c2);

  AluOut out = { uint8_t((d2 << 4) | (d1 & 0xF)), 0 };
  if (op == 2 || op == 3 || op == 7) alu_f |= F_NEGATIVE;
  if (c2)         alu_f |= F_HALF_CARRY;
  if (d2 & 0x10)  alu_f |= F_CARRY;
  if (alu_o == 0) alu_f |= F_ZERO;
  if (op == 7) alu_o = x;
  return out;
}

#else

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

#endif

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
    daa(x, f);
    break;
  case 5:
    alu_o = ~x;
    alu_f = f | 0x60;
    break;
  case 6:
    alu_o = x;
    alu_f = (f & 0x80) | 0x10;
    break;
  case 7:
    alu_o = x;
    alu_f = (f & 0x90) ^ 0x10;
    break;
  }
  if (!alu_o) alu_f |= F_ZERO;
}

//-----------------------------------------------------------------------------
// idempotent

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
  sprintf(o, "data        0x%02x\n", data);
  sprintf(o, "write       %d\n",     write);
  sprintf(o, "\n");

  sprintf(o, "IME         %d\n", ime);
  sprintf(o, "IME_        %d\n", ime_delay);
  sprintf(o, "interrupt   %d\n", interrupt);
  sprintf(o, "int_ack     0x%02x\n", int_ack);
}

//-----------------------------------------------------------------------------
