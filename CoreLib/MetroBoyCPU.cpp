#include "CoreLib/MetroBoyCPU.h"
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

void MetroBoyCPU::reset_boot() {
  *this = MetroBoyCPU();

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

  set_bus(pc, 0);
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::reset_cart() {
  state = 2;
  state_ = 0;
  op_addr = 0xFE;
  op = 0xE0;
  cb = 0x11;
  ime = false;
  ime_delay = false;
  in = 0x50;
  out = 0x00;
  bus_req.addr = 0x0100;
  bus_req.data = 0x00;
  bus_req.read = 0x01;
  bus_req.write = 0x00;
  bus_req.pad1 = 0x00;
  alu_x = 0x00;
  alu_y = 0x00;
  alu_f = 0xb0;
  alu_o = 0x00;
  inc_x = 0x00;
  inc_y = 0x00;
  inc_c = 0x00;
  int_ack = 0x00;
  bc = 0x0013;
  de = 0x00D8;
  hl = 0x014D;
  af = 0x01B0;
  xy = 0xFF50;
  pc = 0x0100;
  sp = 0xFFFE;
  t = 0x00;
}

//-----------------------------------------------------------------------------
// Do the meat of executing the instruction
// pc update _must_ happen in tcycle 0 of state 0, because if an interrupt fires it should _not_ happen.

void MetroBoyCPU::tock_ha(uint8_t imask, uint8_t intf_gh, uint8_t bus_data) {
  state = state_;
  ime = ime_delay;

  if (bus_req.read) {
    in = bus_data;
  }

  if (state == 0) {
    pc = bus_req.addr;
    op_addr = bus_req.addr;
    op = bus_data;

    if ((imask & intf_gh) && ime) {
      op = 0xF4; // fake opcode
      ime = false;
      ime_delay = false;
    }
  }

  alu_x = 0;
  alu_y = 0;
  int_ack = 0;

  if (INT) {
    execute_int(imask, intf_gh);
  }
  else if (HALT) {
    execute_halt(imask, intf_gh);
  }
  else {
    execute_op();
  }
}

void MetroBoyCPU::tock_de(uint8_t imask, uint8_t intf_cd) {
  if (HALT) {
    if ((imask & intf_cd) && ime) {
      state_ = 0;
    }
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_int(uint8_t imask_, uint8_t intf_) {
  uint8_t _int_addr = 0;
  uint8_t _int_ack = 0;

  uint16_t ad = bus_req.addr;
  uint16_t adp = ad + 1;
  uint16_t adm = ad - 1;

  state_ = state + 1;

  if      (imask_ & intf_ & INT_JOYPAD_MASK) { _int_addr = 0x60; _int_ack = INT_JOYPAD_MASK; }
  else if (imask_ & intf_ & INT_SERIAL_MASK) { _int_addr = 0x58; _int_ack = INT_SERIAL_MASK; }
  else if (imask_ & intf_ & INT_TIMER_MASK)  { _int_addr = 0x50; _int_ack = INT_TIMER_MASK; }
  else if (imask_ & intf_ & INT_STAT_MASK)   { _int_addr = 0x48; _int_ack = INT_STAT_MASK; }
  else if (imask_ & intf_ & INT_VBLANK_MASK) { _int_addr = 0x40; _int_ack = INT_VBLANK_MASK; }
  else                                       { _int_addr = 0x00; _int_ack = 0; }

  if      (state == 0) { pc = ad;            bus_nop(sp); }
  else if (state == 1) { sp = adm;           bus_write(sp, pch); }
  else if (state == 2) { sp = adm;           bus_write(sp, pcl); }
  else if (state == 3) {                     bus_nop(); }
  else if (state == 4) { int_ack = _int_ack; op_done(_int_addr); }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_halt(uint8_t imask_, uint8_t intf_) {
  uint16_t ad = bus_req.addr;
  uint16_t adp = ad + 1;
  uint16_t adm = ad - 1;

  if (state == 0) {
    pc = adp;
    bus_read(pc);
    state_ = !(imask_ & intf_);
  }
  else if (state == 1) {
    pc = ad;
    bus_read(pc);
    //state_ = !(imask_ & intf_);
    state_ = 1;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_op() {
  state_ = state + 1;

  uint16_t ad = bus_req.addr;
  uint16_t adp = ad + 1;
  uint16_t adm = ad - 1;

  if (PREFIX_CB) {
    if (state == 1) cb = in;
    uint8_t mask = cb_flag_mask[CB_QUAD];

    bool OP_CB_BIT = PREFIX_CB && (CB_QUAD == 1);

    if (state == 0)                           /**/ { pc = adp; bus_read(pc); }

    if (OP_CB_R) {
      if (state == 1)                         /**/ { pc = adp; alu_x = get_reg(CB_COL);      set_reg(CB_COL, alu_cb(cb, f)); set_f(mask);      op_done(pc); }
    }
    else {
      if (OP_CB_BIT) {
        if (state == 1)                       /**/ { pc = adp;                                      bus_read(hl); }
        if (state == 2)                       /**/ { alu_x = in; alu_cb(cb, f); set_f(mask);        op_done(pc); }
      }
      else {
        if (state == 1)                       /**/ { pc = adp;                                      bus_read(hl); }
        if (state == 2)                       /**/ { alu_x = in; alu_cb(cb, f); set_f(mask);        bus_write(hl, alu_o); }
        if (state == 3)                       /**/ {                                                op_done(pc); }
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

    // IN OUT BC DE HL AF XY | PC SP AD

    // misc

    if (state == 0 && NOP)                    /**/ { pc = adp; /**/                                          /**/ op_done(pc); }
    if (state == 0 && STOP)                   /**/ { pc = adp; /**/                                          /**/ op_done(pc); }
    if (state == 0 && DI)                     /**/ { pc = adp; /**/ ime = false; ime_delay = false;          /**/ op_done(pc); }
    if (state == 0 && EI)                     /**/ { pc = adp; /**/ ime = ime_delay;  ime_delay = true;      /**/ op_done(pc); }
    if (state == 0 && MV_R_R)                 /**/ { pc = adp; /**/ set_reg(OP_ROW, get_reg(OP_COL));        /**/ op_done(pc); }
    if (state == 0 && LD_SP_HL)               /**/ { pc = adp; /**/                                          /**/ bus_nop(hl); }
    if (state == 1 && LD_SP_HL)               /**/ { sp = ad;  /**/                                          /**/ op_done(pc); }

    // load immediate

    if (state == 0 && LD_R_D8)                /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && LD_R_D8)                /**/ { pc = adp; /**/ set_reg(OP_ROW, in);                     /**/ op_done(pc); }

    if (state == 0 && LD_BC_D16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 0 && LD_DE_D16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 0 && LD_HL_D16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 0 && LD_SP_D16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && LD_BC_D16)              /**/ { pc = adp; /**/ c = in;                                  /**/ bus_read(pc); }
    if (state == 1 && LD_DE_D16)              /**/ { pc = adp; /**/ e = in;                                  /**/ bus_read(pc); }
    if (state == 1 && LD_HL_D16)              /**/ { pc = adp; /**/ l = in;                                  /**/ bus_read(pc); }
    if (state == 1 && LD_SP_D16)              /**/ { pc = adp; /**/ spl = in;                                /**/ bus_read(pc); }
    if (state == 2 && LD_BC_D16)              /**/ { pc = adp; /**/ b = in;                                  /**/ op_done(pc); }
    if (state == 2 && LD_DE_D16)              /**/ { pc = adp; /**/ d = in;                                  /**/ op_done(pc); }
    if (state == 2 && LD_HL_D16)              /**/ { pc = adp; /**/ h = in;                                  /**/ op_done(pc); }
    if (state == 2 && LD_SP_D16)              /**/ { pc = adp; /**/ sph = in;                                /**/ op_done(pc); }

    // 8-bit alu

    if (state == 0 && ALU_A_R)                /**/ { pc = adp; /**/ a = alu(a, get_reg(OP_COL), OP_ROW, f); set_f(0xF0);         /**/ op_done(pc); }
    if (state == 0 && INC_R)                  /**/ { pc = adp; /**/ set_reg(OP_ROW, alu(get_reg(OP_ROW), 1, 1, 0)); set_f(0xE0); /**/ op_done(pc); }
    if (state == 0 && DEC_R)                  /**/ { pc = adp; /**/ set_reg(OP_ROW, alu(get_reg(OP_ROW), 1, 3, 0)); set_f(0xE0); /**/ op_done(pc); }
    if (state == 0 && RLC_A)                  /**/ { pc = adp; /**/ a = rlu(OP_ROW, f); set_f(0xF0);         /**/ op_done(pc); }
    if (state == 0 && RRC_A)                  /**/ { pc = adp; /**/ a = rlu(OP_ROW, f); set_f(0xF0);         /**/ op_done(pc); }
    if (state == 0 && RL_A)                   /**/ { pc = adp; /**/ a = rlu(OP_ROW, f); set_f(0xF0);         /**/ op_done(pc); }
    if (state == 0 && RR_A)                   /**/ { pc = adp; /**/ a = rlu(OP_ROW, f); set_f(0xF0);         /**/ op_done(pc); }
    if (state == 0 && DAA)                    /**/ { pc = adp; /**/ a = rlu(OP_ROW, f); set_f(0xB0);         /**/ op_done(pc); }
    if (state == 0 && CPL)                    /**/ { pc = adp; /**/ a = rlu(OP_ROW, f); set_f(0x60);         /**/ op_done(pc); }
    if (state == 0 && SCF)                    /**/ { pc = adp; /**/ a = rlu(OP_ROW, f); set_f(0x70);         /**/ op_done(pc); }
    if (state == 0 && CCF)                    /**/ { pc = adp; /**/ a = rlu(OP_ROW, f); set_f(0x70);         /**/ op_done(pc); }

    if (state == 0 && ALU_A_D8)               /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && ALU_A_D8)               /**/ { pc = adp; /**/ a = alu(a, in, OP_ROW, f); set_f(0xF0);  /**/ op_done(pc); }

    if (state == 0 && ALU_A_HL)               /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
    if (state == 1 && ALU_A_HL)               /**/ {           /**/ a = alu(a, in, OP_ROW, f); set_f(0xF0);  /**/ op_done(pc); }

    if (state == 0 && INC_AT_HL)              /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
    if (state == 1 && INC_AT_HL)              /**/ {           /**/ alu(in, 1, 1, 0); set_f(0xE0);           /**/ bus_write(hl, alu_o); }
    if (state == 2 && INC_AT_HL)              /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && DEC_AT_HL)              /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
    if (state == 1 && DEC_AT_HL)              /**/ {           /**/ alu(in, 1, 3, 0); set_f(0xE0);           /**/ bus_write(hl, alu_o); }
    if (state == 2 && DEC_AT_HL)              /**/ {           /**/                                          /**/ op_done(pc); }

    // 16-bit alu

    if (state == 0 && ADD_SP_R8)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && ADD_SP_R8)              /**/ { pc = adp; /**/ spl = alu(in, spl, 0, f); set_f(0xF0);   /**/ bus_nop(); }
    if (state == 2 && ADD_SP_R8)              /**/ {           /**/ sph = alu(sxt(in), sph, 1, f);           /**/ bus_nop(); }
    if (state == 3 && ADD_SP_R8)              /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && LD_HL_SP_R8)            /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && LD_HL_SP_R8)            /**/ { pc = adp; /**/ l = alu(in, spl, 0, f); set_f(0xF0);     /**/ bus_nop(); }
    if (state == 2 && LD_HL_SP_R8)            /**/ {           /**/ h = alu(sxt(in), sph, 1, f);             /**/ op_done(pc); }

    if (state == 0 && INC_BC)                 /**/ { pc = adp; /**/                                          /**/ bus_nop(bc); }
    if (state == 1 && INC_BC)                 /**/ { bc = adp; /**/                                          /**/ op_done(pc); }

    if (state == 0 && DEC_BC)                 /**/ { pc = adp; /**/                                          /**/ bus_nop(bc); }
    if (state == 1 && DEC_BC)                 /**/ { bc = adm; /**/                                          /**/ op_done(pc); }

    if (state == 0 && INC_DE)                 /**/ { pc = adp; /**/                                          /**/ bus_nop(de); }
    if (state == 1 && INC_DE)                 /**/ { de = adp; /**/                                          /**/ op_done(pc); }

    if (state == 0 && DEC_DE)                 /**/ { pc = adp; /**/                                          /**/ bus_nop(de); }
    if (state == 1 && DEC_DE)                 /**/ { de = adm; /**/                                          /**/ op_done(pc); }

    if (state == 0 && INC_HL)                 /**/ { pc = adp; /**/                                          /**/ bus_nop(hl); }
    if (state == 1 && INC_HL)                 /**/ { hl = adp; /**/                                          /**/ op_done(pc); }

    if (state == 0 && DEC_HL)                 /**/ { pc = adp; /**/                                          /**/ bus_nop(hl); }
    if (state == 1 && DEC_HL)                 /**/ { hl = adm; /**/                                          /**/ op_done(pc); }

    if (state == 0 && INC_SP)                 /**/ { pc = adp; /**/                                          /**/ bus_nop(sp); }
    if (state == 1 && INC_SP)                 /**/ { sp = adp; /**/                                          /**/ op_done(pc); }

    if (state == 0 && DEC_SP)                 /**/ { pc = adp; /**/                                          /**/ bus_nop(sp); }
    if (state == 1 && DEC_SP)                 /**/ { sp = adm; /**/                                          /**/ op_done(pc); }

    if (state == 0 && ADD_HL_BC)              /**/ { pc = adp; /**/ l = alu(c,   l, 0, f);                   /**/ bus_nop(); }
    if (state == 1 && ADD_HL_BC)              /**/ {           /**/ h = alu(b,   h, 1, alu_f); set_f(0x70);  /**/ op_done(pc); }

    if (state == 0 && ADD_HL_DE)              /**/ { pc = adp; /**/ l = alu(e,   l, 0, f);                   /**/ bus_nop(); }
    if (state == 1 && ADD_HL_DE)              /**/ {           /**/ h = alu(d,   h, 1, alu_f); set_f(0x70);  /**/ op_done(pc); }

    if (state == 0 && ADD_HL_HL)              /**/ { pc = adp; /**/ l = alu(l,   l, 0, f);                   /**/ bus_nop(); }
    if (state == 1 && ADD_HL_HL)              /**/ {           /**/ h = alu(h,   h, 1, alu_f); set_f(0x70);  /**/ op_done(pc); }

    if (state == 0 && ADD_HL_SP)              /**/ { pc = adp; /**/ l = alu(spl, l, 0, f);                   /**/ bus_nop(); }
    if (state == 1 && ADD_HL_SP)              /**/ {           /**/ h = alu(sph, h, 1, alu_f); set_f(0x70);  /**/ op_done(pc); }

    // load/store

    if (state == 0 && STM_A16_SP)             /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && STM_A16_SP)             /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
    if (state == 2 && STM_A16_SP)             /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_write(xy, spl); }
    if (state == 3 && STM_A16_SP)             /**/ { xy = adp; /**/                                          /**/ bus_write(xy, sph); }
    if (state == 4 && STM_A16_SP)             /**/ { xy = adp; /**/                                          /**/ op_done(pc); }

    if (state == 0 && STM_A16_A)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && STM_A16_A)              /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
    if (state == 2 && STM_A16_A)              /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_write(xy, a); }
    if (state == 3 && STM_A16_A)              /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && LDM_A_A16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && LDM_A_A16)              /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
    if (state == 2 && LDM_A_A16)              /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_read(xy); }
    if (state == 3 && LDM_A_A16)              /**/ {           /**/ a = in;                                  /**/ op_done(pc); }

    // indirect load/store                    /**/             /**/                                          /**/

    if (state == 0 && LDM_A_BC)               /**/ { pc = adp; /**/                                          /**/ bus_read(bc); }
    if (state == 1 && LDM_A_BC)               /**/ {           /**/ a = in;                                  /**/ op_done(pc); }

    if (state == 0 && LDM_A_DE)               /**/ { pc = adp; /**/                                          /**/ bus_read(de); }
    if (state == 1 && LDM_A_DE)               /**/ {           /**/ a = in;                                  /**/ op_done(pc); }

    if (state == 0 && LDM_R_HL)               /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
    if (state == 1 && LDM_R_HL)               /**/ {           /**/ set_reg(OP_ROW, in);                     /**/ op_done(pc); }

    if (state == 0 && LDM_A_HLP)              /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
    if (state == 1 && LDM_A_HLP)              /**/ { hl = adp; /**/ a = in;                                  /**/ op_done(pc); }

    if (state == 0 && LDM_A_HLM)              /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
    if (state == 1 && LDM_A_HLM)              /**/ { hl = adm; /**/ a = in;                                  /**/ op_done(pc); }

    if (state == 0 && STM_BC_A)               /**/ { pc = adp; /**/                                          /**/ bus_write(bc, a); }
    if (state == 1 && STM_BC_A)               /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && STM_DE_A)               /**/ { pc = adp; /**/                                          /**/ bus_write(de, a); }
    if (state == 1 && STM_DE_A)               /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && STM_HL_R)               /**/ { pc = adp; /**/                                          /**/ bus_write(hl, get_reg(OP_COL)); }
    if (state == 1 && STM_HL_R)               /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && STM_HL_D8)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && STM_HL_D8)              /**/ { pc = adp; /**/                                          /**/ bus_write(hl, in); }
    if (state == 2 && STM_HL_D8)              /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && STM_HLP_A)              /**/ { pc = adp; /**/                                          /**/ bus_write(hl, a); }
    if (state == 1 && STM_HLP_A)              /**/ { hl = adp; /**/                                          /**/ op_done(pc); }

    if (state == 0 && STM_HLM_A)              /**/ { pc = adp; /**/                                          /**/ bus_write(hl, a); }
    if (state == 1 && STM_HLM_A)              /**/ { hl = adm; /**/                                          /**/ op_done(pc); }

    // zero-page load/store

    if (state == 0 && LDM_A_C)                /**/ { pc = adp; /**/ xy = 0xFF00 | c;                         /**/ bus_read(xy); }
    if (state == 1 && LDM_A_C)                /**/ {           /**/ a = in;                                  /**/ op_done(pc); }

    if (state == 0 && LDM_A_A8)               /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && LDM_A_A8)               /**/ { pc = adp; /**/ xy = 0xFF00 | in;                        /**/ bus_read(xy); }
    if (state == 2 && LDM_A_A8)               /**/ {           /**/ a = in;                                  /**/ op_done(pc); }

    if (state == 0 && STM_C_A)                /**/ { pc = adp; /**/ xy = 0xFF00 | c;                         /**/ bus_write(xy, a); }
    if (state == 1 && STM_C_A)                /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && STM_A8_A)               /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && STM_A8_A)               /**/ { pc = adp; /**/ xy = 0xFF00 | in;                        /**/ bus_write(xy, a); }
    if (state == 2 && STM_A8_A)               /**/ {           /**/                                          /**/ op_done(pc); }

    // push / pop

    if (state == 0 && PUSH_BC)                /**/ { pc = adp; /**/                                          /**/ bus_nop(sp); }
    if (state == 0 && PUSH_DE)                /**/ { pc = adp; /**/                                          /**/ bus_nop(sp); }
    if (state == 0 && PUSH_HL)                /**/ { pc = adp; /**/                                          /**/ bus_nop(sp); }
    if (state == 0 && PUSH_AF)                /**/ { pc = adp; /**/                                          /**/ bus_nop(sp); }
    if (state == 1 && PUSH_BC)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, b); }
    if (state == 1 && PUSH_DE)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, d); }
    if (state == 1 && PUSH_HL)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, h); }
    if (state == 1 && PUSH_AF)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, a); }
    if (state == 2 && PUSH_BC)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, c); }
    if (state == 2 && PUSH_DE)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, e); }
    if (state == 2 && PUSH_HL)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, l); }
    if (state == 2 && PUSH_AF)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, f); }
    if (state == 3 && PUSH_BC)                /**/ {           /**/                                          /**/ op_done(pc); }
    if (state == 3 && PUSH_DE)                /**/ {           /**/                                          /**/ op_done(pc); }
    if (state == 3 && PUSH_HL)                /**/ {           /**/                                          /**/ op_done(pc); }
    if (state == 3 && PUSH_AF)                /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && POP_BC)                 /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
    if (state == 0 && POP_DE)                 /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
    if (state == 0 && POP_HL)                 /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
    if (state == 0 && POP_AF)                 /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
    if (state == 1 && POP_BC)                 /**/ { sp = adp; /**/ c = in;                                  /**/ bus_read(sp); }
    if (state == 1 && POP_DE)                 /**/ { sp = adp; /**/ e = in;                                  /**/ bus_read(sp); }
    if (state == 1 && POP_HL)                 /**/ { sp = adp; /**/ l = in;                                  /**/ bus_read(sp); }
    if (state == 1 && POP_AF)                 /**/ { sp = adp; /**/ f = in;                                  /**/ bus_read(sp); }
    if (state == 2 && POP_BC)                 /**/ { sp = adp; /**/ b = in;                                  /**/ op_done(pc); }
    if (state == 2 && POP_DE)                 /**/ { sp = adp; /**/ d = in;                                  /**/ op_done(pc); }
    if (state == 2 && POP_HL)                 /**/ { sp = adp; /**/ h = in;                                  /**/ op_done(pc); }
    if (state == 2 && POP_AF)                 /**/ { sp = adp; /**/ a = in;                                  /**/ op_done(pc); }

    // conditional branches

    if (state == 0 && JR_R8)                  /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && JR_R8)                  /**/ { pc = adp; /**/ pc += int8_t(in);                        /**/ bus_nop(pc); }         // probably signed add + carry to get the extra inc in
    if (state == 2 && JR_R8)                  /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && JR_CC_R8    &&  branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && JR_CC_R8    &&  branch) /**/ { pc = adp; /**/ pc += int8_t(in);                        /**/ bus_nop(pc); }
    if (state == 2 && JR_CC_R8    &&  branch) /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && JR_CC_R8    && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && JR_CC_R8    && !branch) /**/ { pc = adp; /**/                                          /**/ op_done(pc); }


    if (state == 0 && JP_CC_A16   &&  branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && JP_CC_A16   &&  branch) /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
    if (state == 2 && JP_CC_A16   &&  branch) /**/ {           /**/ xyh = in;                                /**/ bus_nop(); }
    if (state == 3 && JP_CC_A16   &&  branch) /**/ {           /**/                                          /**/ op_done(xy); }

    if (state == 0 && JP_CC_A16   && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && JP_CC_A16   && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 2 && JP_CC_A16   && !branch) /**/ { pc = adp; /**/                                          /**/ op_done(pc); }

    if (state == 0 && JP_A16)                 /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && JP_A16)                 /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
    if (state == 2 && JP_A16)                 /**/ {           /**/ xyh = in;                                /**/ bus_nop(); }
    if (state == 3 && JP_A16)                 /**/ {           /**/                                          /**/ op_done(xy); }

    if (state == 0 && JP_HL)                  /**/ { pc = adp; /**/                                          /**/ op_done(hl); }

    // calls

    if (state == 0 && CALL_CC_A16 &&  branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && CALL_CC_A16 &&  branch) /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
    if (state == 2 && CALL_CC_A16 &&  branch) /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_nop(sp); }
    if (state == 3 && CALL_CC_A16 &&  branch) /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pch); }
    if (state == 4 && CALL_CC_A16 &&  branch) /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pcl); }
    if (state == 5 && CALL_CC_A16 &&  branch) /**/ {           /**/                                          /**/ op_done(xy); }

    if (state == 0 && CALL_CC_A16 && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && CALL_CC_A16 && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 2 && CALL_CC_A16 && !branch) /**/ { pc = adp; /**/                                          /**/ op_done(pc); }

    if (state == 0 && CALL_A16)               /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
    if (state == 1 && CALL_A16)               /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
    if (state == 2 && CALL_A16)               /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_nop(sp); }
    if (state == 3 && CALL_A16)               /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pch); }
    if (state == 4 && CALL_A16)               /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pcl); }
    if (state == 5 && CALL_A16)               /**/ {           /**/                                          /**/ op_done(xy); }

    if (state == 0 && RST_NN)                 /**/ { pc = adp; /**/                                          /**/ bus_nop(sp); }
    if (state == 1 && RST_NN)                 /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pch); }
    if (state == 2 && RST_NN)                 /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pcl); }
    if (state == 3 && RST_NN)                 /**/ {           /**/ xy = op - 0xC7;                          /**/ op_done(xy); }

    // returns

    if (state == 0 && RET_CC      && !branch) /**/ { pc = adp; /**/                                          /**/ bus_nop(); }
    if (state == 1 && RET_CC      && !branch) /**/ {           /**/                                          /**/ op_done(pc); }

    if (state == 0 && RET_CC      &&  branch) /**/ { pc = adp; /**/                                          /**/ bus_nop(); }
    if (state == 1 && RET_CC      &&  branch) /**/ {           /**/                                          /**/ bus_read(sp); }
    if (state == 2 && RET_CC      &&  branch) /**/ { sp = adp; /**/ xyl = in;                                /**/ bus_read(sp); }
    if (state == 3 && RET_CC      &&  branch) /**/ { sp = adp; /**/ xyh = in;                                /**/ bus_nop(); }
    if (state == 4 && RET_CC      &&  branch) /**/ {           /**/                                          /**/ op_done(xy); }

    if (state == 0 && RETI)                   /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
    if (state == 1 && RETI)                   /**/ { sp = adp; /**/ xyl = in;                                /**/ bus_read(sp); }
    if (state == 2 && RETI)                   /**/ { sp = adp; /**/ xyh = in;                                /**/ bus_nop(); }
    if (state == 3 && RETI)                   /**/ {           /**/ ime = true; ime_delay = true;            /**/ op_done(xy); }

    if (state == 0 && RET)                    /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
    if (state == 1 && RET)                    /**/ { sp = adp; /**/ xyl = in;                                /**/ bus_read(sp); }
    if (state == 2 && RET)                    /**/ { sp = adp; /**/ xyh = in;                                /**/ bus_nop(); }
    if (state == 3 && RET)                    /**/ {           /**/                                          /**/ op_done(xy); }

    f &= 0xF0;
  }
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::get_reg(int mux) {
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

void MetroBoyCPU::set_reg(int mux, uint8_t data) {
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

void MetroBoyCPU::set_f(uint8_t mask) {
  f = (f & ~mask) | (alu_f & mask);
  if (ADD_SP_R8)   { f &= ~(F_ZERO | F_NEGATIVE); }
  if (LD_HL_SP_R8) { f &= ~(F_ZERO | F_NEGATIVE); }
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::alu(uint8_t arg1, uint8_t arg2, int op, uint8_t flags) {
  alu_x = arg1;
  alu_y = arg2;
  return alu(op, flags);
}

uint8_t MetroBoyCPU::alu(int op, uint8_t flags) {
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

  return alu_o;
}

//-----------------------------------------------------------------------------
// The logic is more annoying, but this can be implemented as two 4-bit additions

uint8_t MetroBoyCPU::daa(uint8_t x, uint8_t f) {
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

uint8_t MetroBoyCPU::rlu(int op, uint8_t flags) {
  alu_f = flags;
  const uint8_t x = a;
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

uint8_t MetroBoyCPU::alu_cb(int op, uint8_t flags) {
  alu_f = flags;
  const uint8_t x = alu_x;
  const uint8_t quad = ((op >> 6) & 3);
  const uint8_t row = ((op >> 3) & 7);
  const bool bit_mux = (x >> row) & 1;

  alu_o = 0;

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

void MetroBoyCPU::dump(Dumper& d_) const {
  d_("\002===== CPU =====\001\n");
  d_("state    %d\n", state);
  d_("state_   %d\n", state_);
  d_("op addr  0x%04x\n", op_addr);
  d_("opcode   0x%02x\n", op);
  d_("opname   '%s' @ %d\n", op_strings2[op], state);
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
