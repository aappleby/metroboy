#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"
#include "CoreLib/OpDecoder.h"
#include <string.h>

//#pragma warning(disable:4189) // local variable unreferenced
//#pragma warning(disable:4244)
#pragma warning(disable:4458) // declaration hides class member

uint8_t  sxt(uint8_t x) { return x & 0x80 ? 0xFF : 0x00; }

constexpr uint8_t F_CARRY = 0x10;
constexpr uint8_t F_HALF_CARRY = 0x20;
constexpr uint8_t F_NEGATIVE = 0x40;
constexpr uint8_t F_ZERO = 0x80;

//-----------------------------------------------------------------------------

void MetroBoyCPU::reset_to_bootrom() {
  memset(this, 0, sizeof(*this));
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::reset_to_cart() {
  _bus_addr  = 0xff50;
  _bus_data  = 1;
  _bus_read  = 0;
  _bus_write = 1;

  op_addr = 0x00FE;
  op = 0xe0;
  cb = 0x11;
  state = 1;
  state_ = 2;
  in = 0x50;

  ime = false;
  ime_delay = false;
  int_ack = 0x00;

  alu_f = 0xb0;
  alu_o = 0x00;

  pc = 0x0100;
  sp = 0xFFFE;
  xy = 0xFF50;
  bc = 0x0013;
  de = 0x00D8;
  hl = 0x014D;
  af = 0x01B0;
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::dump(Dumper& d_) const {
  d_("bus req : %04x:%02x %s%s\n", _bus_addr, _bus_data, _bus_read  ? "\003R\001" : "-", _bus_write ? "\002W\001" : "-");
  d_("op addr : 0x%04x\n", op_addr);
  d_("opname  : '%s' @ %d->%d\n", op_strings2[op], state, state_);
  d_("opcode  : 0x%02x\n", op);
  d_("CB      : 0x%02x\n", cb);
  d_("in      : 0x%02x\n", in);
  d_("\n");
  d_("IME     : %d\n", ime);
  d_("IME_    : %d\n", ime_delay);
  d_("int #   : %d\n", INT);
  d_("int_ack : 0x%02x\n", int_ack);
  d_("\n");
  d_("alu_f   : 0x%02x\n", alu_f);
  d_("alu_o   : 0x%02x\n", alu_o);
  d_("\n");
  d_("PC      : 0x%04x 0x%02x 0x%02x\n", pc, pcl, pch);
  d_("SP      : 0x%04x 0x%02x 0x%02x\n", sp, sph, spl);
  d_("XY      : 0x%04x 0x%02x 0x%02x\n", xy, xyh, xyl);
  d_("BC      : 0x%04x 0x%02x 0x%02x\n", bc, b, c);
  d_("DE      : 0x%04x 0x%02x 0x%02x\n", de, d, e);
  d_("HL      : 0x%04x 0x%02x 0x%02x\n", hl, h, l);
  d_("AF      : 0x%04x 0x%02x 0x%02x\n", af, a, f);
  d_("\n");
}

//-----------------------------------------------------------------------------
// Do the meat of executing the instruction

void MetroBoyCPU::tock_ab(uint8_t imask, uint8_t intf_gh, uint8_t bus_data) {
  state = state_;

  if (_bus_read) in = bus_data;

  if (state == 0) {
    op_addr = _bus_addr;
    op = bus_data;

    if ((imask & intf_gh) && ime) {
      op = 0xF4; // fake opcode
      ime = false;
      ime_delay = false;
    }
  }

  int_ack = 0;
  ime = ime_delay; // must be after int check, before op execution

  if      (INT)       execute_int(imask, intf_gh);
  else if (HALT)      execute_halt(imask, intf_gh);
  else if (PREFIX_CB) execute_cb();
  else                execute_op();
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_int(uint8_t imask_, uint8_t intf_) {
  state_ = state + 1;

  if (state == 2) {
    if      (imask_ & intf_ & INT_VBLANK_MASK) { int_addr = 0x40; }
    else if (imask_ & intf_ & INT_STAT_MASK)   { int_addr = 0x48; }
    else if (imask_ & intf_ & INT_TIMER_MASK)  { int_addr = 0x50; }
    else if (imask_ & intf_ & INT_SERIAL_MASK) { int_addr = 0x58; }
    else if (imask_ & intf_ & INT_JOYPAD_MASK) { int_addr = 0x60; }
    else                                       { int_addr = 0x00; }

    if      (imask_ & intf_ & INT_VBLANK_MASK) { int_ack = INT_VBLANK_MASK; }
    else if (imask_ & intf_ & INT_STAT_MASK)   { int_ack = INT_STAT_MASK; }
    else if (imask_ & intf_ & INT_TIMER_MASK)  { int_ack = INT_TIMER_MASK; }
    else if (imask_ & intf_ & INT_SERIAL_MASK) { int_ack = INT_SERIAL_MASK; }
    else if (imask_ & intf_ & INT_JOYPAD_MASK) { int_ack = INT_JOYPAD_MASK; }
    else                                       { int_ack = 0; }
  }

  if      (state == 0) { pc = _bus_addr;     bus_pass(sp); }
  else if (state == 1) { sp = _bus_addr - 1; bus_write(sp, pch); }
  else if (state == 2) { sp = _bus_addr - 1; bus_write(sp, pcl); }
  else if (state == 3) {                     bus_pass(pc); }
  else if (state == 4) {                     bus_done(int_addr); }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_halt(uint8_t imask_, uint8_t intf_) {
  if (state == 0) {
    pc = _bus_addr + 1;
    bus_read(pc);
    state_ = !(imask_& intf_);

    //if (!ime) state_ = 0;
  }
  else if (state == 1) {
    pc = _bus_addr;
    bus_read(pc);
    state_ = 1;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_cb() {
  state_ = state + 1;

  uint16_t ad = _bus_addr;
  uint16_t adp = ad + 1;

  if (state == 1) cb = in;
  uint8_t mask = cb_flag_mask[CB_QUAD];

  bool OP_CB_BIT = PREFIX_CB && (CB_QUAD == 1);

  if (state == 0)     { pc = adp; bus_read(pc); }

  if (OP_CB_R) {
    if (state == 1)   { pc = adp; set_reg(CB_COL, alu_cb(get_reg(CB_COL), cb, f)); set_f(mask);      bus_done(pc); }
  }
  else {
    if (OP_CB_BIT) {
      if (state == 1) { pc = adp;                                      bus_read(hl); }
      if (state == 2) { alu_cb(in, cb, f); set_f(mask);        bus_done(pc); }
    }
    else {
      if (state == 1) { pc = adp;                                      bus_read(hl); }
      if (state == 2) { alu_cb(in, cb, f); set_f(mask);        bus_write(hl, alu_o); }
      if (state == 3) {                                                bus_done(pc); }
    }
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_op() {
  state_ = state + 1;

  uint16_t ad = _bus_addr;
  uint16_t adp = ad + 1;
  uint16_t adm = ad - 1;

  bool branch = false;
  switch (OP_ROW & 3) {
  case 0: branch = !(f & F_ZERO); break;
  case 1: branch =  (f & F_ZERO); break;
  case 2: branch = !(f & F_CARRY); break;
  case 3: branch =  (f & F_CARRY); break;
  }

  // IN OUT BC DE HL AF XY | PC SP AD

  // misc

  if (state == 0 && NOP)                    /**/ { pc = adp; /**/                                          /**/ bus_done(pc); }
  if (state == 0 && STOP)                   /**/ { pc = adp; /**/                                          /**/ bus_done(pc); }
  if (state == 0 && DI)                     /**/ { pc = adp; /**/ ime = false; ime_delay = false;          /**/ bus_done(pc); }
  if (state == 0 && EI)                     /**/ { pc = adp; /**/ ime = ime_delay;  ime_delay = true;      /**/ bus_done(pc); }
  if (state == 0 && MV_R_R)                 /**/ { pc = adp; /**/ set_reg(OP_ROW, get_reg(OP_COL));        /**/ bus_done(pc); }
  if (state == 0 && LD_SP_HL)               /**/ { pc = adp; /**/                                          /**/ bus_pass(hl); }
  if (state == 1 && LD_SP_HL)               /**/ { sp = ad;  /**/                                          /**/ bus_done(pc); }

  // load immediate

  if (state == 0 && LD_R_D8)                /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && LD_R_D8)                /**/ { pc = adp; /**/ set_reg(OP_ROW, in);                     /**/ bus_done(pc); }

  if (state == 0 && LD_BC_D16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 0 && LD_DE_D16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 0 && LD_HL_D16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 0 && LD_SP_D16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && LD_BC_D16)              /**/ { pc = adp; /**/ c = in;                                  /**/ bus_read(pc); }
  if (state == 1 && LD_DE_D16)              /**/ { pc = adp; /**/ e = in;                                  /**/ bus_read(pc); }
  if (state == 1 && LD_HL_D16)              /**/ { pc = adp; /**/ l = in;                                  /**/ bus_read(pc); }
  if (state == 1 && LD_SP_D16)              /**/ { pc = adp; /**/ spl = in;                                /**/ bus_read(pc); }
  if (state == 2 && LD_BC_D16)              /**/ { pc = adp; /**/ b = in;                                  /**/ bus_done(pc); }
  if (state == 2 && LD_DE_D16)              /**/ { pc = adp; /**/ d = in;                                  /**/ bus_done(pc); }
  if (state == 2 && LD_HL_D16)              /**/ { pc = adp; /**/ h = in;                                  /**/ bus_done(pc); }
  if (state == 2 && LD_SP_D16)              /**/ { pc = adp; /**/ sph = in;                                /**/ bus_done(pc); }

  // 8-bit alu

  if (state == 0 && ALU_A_R)                /**/ { pc = adp; /**/ a = alu(a, get_reg(OP_COL), OP_ROW, f); set_f(0xF0);         /**/ bus_done(pc); }
  if (state == 0 && INC_R)                  /**/ { pc = adp; /**/ set_reg(OP_ROW, alu(get_reg(OP_ROW), 1, 1, 0)); set_f(0xE0); /**/ bus_done(pc); }
  if (state == 0 && DEC_R)                  /**/ { pc = adp; /**/ set_reg(OP_ROW, alu(get_reg(OP_ROW), 1, 3, 0)); set_f(0xE0); /**/ bus_done(pc); }
  if (state == 0 && RLC_A)                  /**/ { pc = adp; /**/ a = rlu(a, OP_ROW, f); set_f(0xF0);      /**/ bus_done(pc); }
  if (state == 0 && RRC_A)                  /**/ { pc = adp; /**/ a = rlu(a, OP_ROW, f); set_f(0xF0);      /**/ bus_done(pc); }
  if (state == 0 && RL_A)                   /**/ { pc = adp; /**/ a = rlu(a, OP_ROW, f); set_f(0xF0);      /**/ bus_done(pc); }
  if (state == 0 && RR_A)                   /**/ { pc = adp; /**/ a = rlu(a, OP_ROW, f); set_f(0xF0);      /**/ bus_done(pc); }
  if (state == 0 && DAA)                    /**/ { pc = adp; /**/ a = rlu(a, OP_ROW, f); set_f(0xB0);      /**/ bus_done(pc); }
  if (state == 0 && CPL)                    /**/ { pc = adp; /**/ a = rlu(a, OP_ROW, f); set_f(0x60);      /**/ bus_done(pc); }
  if (state == 0 && SCF)                    /**/ { pc = adp; /**/ a = rlu(a, OP_ROW, f); set_f(0x70);      /**/ bus_done(pc); }
  if (state == 0 && CCF)                    /**/ { pc = adp; /**/ a = rlu(a, OP_ROW, f); set_f(0x70);      /**/ bus_done(pc); }

  if (state == 0 && ALU_A_D8)               /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && ALU_A_D8)               /**/ { pc = adp; /**/ a = alu(a, in, OP_ROW, f); set_f(0xF0);  /**/ bus_done(pc); }

  if (state == 0 && ALU_A_HL)               /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
  if (state == 1 && ALU_A_HL)               /**/ {           /**/ a = alu(a, in, OP_ROW, f); set_f(0xF0);  /**/ bus_done(pc); }

  if (state == 0 && INC_AT_HL)              /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
  if (state == 1 && INC_AT_HL)              /**/ {           /**/ alu(in, 1, 1, 0); set_f(0xE0);           /**/ bus_write(hl, alu_o); }
  if (state == 2 && INC_AT_HL)              /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && DEC_AT_HL)              /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
  if (state == 1 && DEC_AT_HL)              /**/ {           /**/ alu(in, 1, 3, 0); set_f(0xE0);           /**/ bus_write(hl, alu_o); }
  if (state == 2 && DEC_AT_HL)              /**/ {           /**/                                          /**/ bus_done(pc); }

  // 16-bit alu

  if (state == 0 && ADD_SP_R8)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && ADD_SP_R8)              /**/ { pc = adp; /**/ spl = alu(in, spl, 0, f); set_f(0xF0);   /**/ bus_pass(pc); }
  if (state == 2 && ADD_SP_R8)              /**/ {           /**/ sph = alu(sxt(in), sph, 1, f);           /**/ bus_pass(pc); }
  if (state == 3 && ADD_SP_R8)              /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && LD_HL_SP_R8)            /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && LD_HL_SP_R8)            /**/ { pc = adp; /**/ l = alu(in, spl, 0, f); set_f(0xF0);     /**/ bus_pass(pc); }
  if (state == 2 && LD_HL_SP_R8)            /**/ {           /**/ h = alu(sxt(in), sph, 1, f);             /**/ bus_done(pc); }

  if (state == 0 && INC_BC)                 /**/ { pc = adp; /**/                                          /**/ bus_pass(bc); }
  if (state == 1 && INC_BC)                 /**/ { bc = adp; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && DEC_BC)                 /**/ { pc = adp; /**/                                          /**/ bus_pass(bc); }
  if (state == 1 && DEC_BC)                 /**/ { bc = adm; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && INC_DE)                 /**/ { pc = adp; /**/                                          /**/ bus_pass(de); }
  if (state == 1 && INC_DE)                 /**/ { de = adp; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && DEC_DE)                 /**/ { pc = adp; /**/                                          /**/ bus_pass(de); }
  if (state == 1 && DEC_DE)                 /**/ { de = adm; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && INC_HL)                 /**/ { pc = adp; /**/                                          /**/ bus_pass(hl); }
  if (state == 1 && INC_HL)                 /**/ { hl = adp; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && DEC_HL)                 /**/ { pc = adp; /**/                                          /**/ bus_pass(hl); }
  if (state == 1 && DEC_HL)                 /**/ { hl = adm; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && INC_SP)                 /**/ { pc = adp; /**/                                          /**/ bus_pass(sp); }
  if (state == 1 && INC_SP)                 /**/ { sp = adp; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && DEC_SP)                 /**/ { pc = adp; /**/                                          /**/ bus_pass(sp); }
  if (state == 1 && DEC_SP)                 /**/ { sp = adm; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && ADD_HL_BC)              /**/ { pc = adp; /**/ l = alu(c,   l, 0, f);                   /**/ bus_pass(pc); }
  if (state == 1 && ADD_HL_BC)              /**/ {           /**/ h = alu(b,   h, 1, alu_f); set_f(0x70);  /**/ bus_done(pc); }

  if (state == 0 && ADD_HL_DE)              /**/ { pc = adp; /**/ l = alu(e,   l, 0, f);                   /**/ bus_pass(pc); }
  if (state == 1 && ADD_HL_DE)              /**/ {           /**/ h = alu(d,   h, 1, alu_f); set_f(0x70);  /**/ bus_done(pc); }

  if (state == 0 && ADD_HL_HL)              /**/ { pc = adp; /**/ l = alu(l,   l, 0, f);                   /**/ bus_pass(pc); }
  if (state == 1 && ADD_HL_HL)              /**/ {           /**/ h = alu(h,   h, 1, alu_f); set_f(0x70);  /**/ bus_done(pc); }

  if (state == 0 && ADD_HL_SP)              /**/ { pc = adp; /**/ l = alu(spl, l, 0, f);                   /**/ bus_pass(pc); }
  if (state == 1 && ADD_HL_SP)              /**/ {           /**/ h = alu(sph, h, 1, alu_f); set_f(0x70);  /**/ bus_done(pc); }

  // load/store

  if (state == 0 && STM_A16_SP)             /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && STM_A16_SP)             /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
  if (state == 2 && STM_A16_SP)             /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_write(xy, spl); }
  if (state == 3 && STM_A16_SP)             /**/ { xy = adp; /**/                                          /**/ bus_write(xy, sph); }
  if (state == 4 && STM_A16_SP)             /**/ { xy = adp; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && STM_A16_A)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && STM_A16_A)              /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
  if (state == 2 && STM_A16_A)              /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_write(xy, a); }
  if (state == 3 && STM_A16_A)              /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && LDM_A_A16)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && LDM_A_A16)              /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
  if (state == 2 && LDM_A_A16)              /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_read(xy); }
  if (state == 3 && LDM_A_A16)              /**/ {           /**/ a = in;                                  /**/ bus_done(pc); }

  // indirect load/store                    /**/             /**/                                          /**/

  if (state == 0 && LDM_A_BC)               /**/ { pc = adp; /**/                                          /**/ bus_read(bc); }
  if (state == 1 && LDM_A_BC)               /**/ {           /**/ a = in;                                  /**/ bus_done(pc); }

  if (state == 0 && LDM_A_DE)               /**/ { pc = adp; /**/                                          /**/ bus_read(de); }
  if (state == 1 && LDM_A_DE)               /**/ {           /**/ a = in;                                  /**/ bus_done(pc); }

  if (state == 0 && LDM_R_HL)               /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
  if (state == 1 && LDM_R_HL)               /**/ {           /**/ set_reg(OP_ROW, in);                     /**/ bus_done(pc); }

  if (state == 0 && LDM_A_HLP)              /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
  if (state == 1 && LDM_A_HLP)              /**/ { hl = adp; /**/ a = in;                                  /**/ bus_done(pc); }

  if (state == 0 && LDM_A_HLM)              /**/ { pc = adp; /**/                                          /**/ bus_read(hl); }
  if (state == 1 && LDM_A_HLM)              /**/ { hl = adm; /**/ a = in;                                  /**/ bus_done(pc); }

  if (state == 0 && STM_BC_A)               /**/ { pc = adp; /**/                                          /**/ bus_write(bc, a); }
  if (state == 1 && STM_BC_A)               /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && STM_DE_A)               /**/ { pc = adp; /**/                                          /**/ bus_write(de, a); }
  if (state == 1 && STM_DE_A)               /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && STM_HL_R)               /**/ { pc = adp; /**/                                          /**/ bus_write(hl, get_reg(OP_COL)); }
  if (state == 1 && STM_HL_R)               /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && STM_HL_D8)              /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && STM_HL_D8)              /**/ { pc = adp; /**/                                          /**/ bus_write(hl, in); }
  if (state == 2 && STM_HL_D8)              /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && STM_HLP_A)              /**/ { pc = adp; /**/                                          /**/ bus_write(hl, a); }
  if (state == 1 && STM_HLP_A)              /**/ { hl = adp; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && STM_HLM_A)              /**/ { pc = adp; /**/                                          /**/ bus_write(hl, a); }
  if (state == 1 && STM_HLM_A)              /**/ { hl = adm; /**/                                          /**/ bus_done(pc); }

  // zero-page load/store

  if (state == 0 && LDM_A_C)                /**/ { pc = adp; /**/ xy = 0xFF00 | c;                         /**/ bus_read(xy); }
  if (state == 1 && LDM_A_C)                /**/ {           /**/ a = in;                                  /**/ bus_done(pc); }

  if (state == 0 && LDM_A_A8)               /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && LDM_A_A8)               /**/ { pc = adp; /**/ xy = 0xFF00 | in;                        /**/ bus_read(xy); }
  if (state == 2 && LDM_A_A8)               /**/ {           /**/ a = in;                                  /**/ bus_done(pc); }

  if (state == 0 && STM_C_A)                /**/ { pc = adp; /**/ xy = 0xFF00 | c;                         /**/ bus_write(xy, a); }
  if (state == 1 && STM_C_A)                /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && STM_A8_A)               /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && STM_A8_A)               /**/ { pc = adp; /**/ xy = 0xFF00 | in;                        /**/ bus_write(xy, a); }
  if (state == 2 && STM_A8_A)               /**/ {           /**/                                          /**/ bus_done(pc); }

  // push / pop

  if (state == 0 && PUSH_BC)                /**/ { pc = adp; /**/                                          /**/ bus_pass(sp); }
  if (state == 0 && PUSH_DE)                /**/ { pc = adp; /**/                                          /**/ bus_pass(sp); }
  if (state == 0 && PUSH_HL)                /**/ { pc = adp; /**/                                          /**/ bus_pass(sp); }
  if (state == 0 && PUSH_AF)                /**/ { pc = adp; /**/                                          /**/ bus_pass(sp); }
  if (state == 1 && PUSH_BC)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, b); }
  if (state == 1 && PUSH_DE)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, d); }
  if (state == 1 && PUSH_HL)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, h); }
  if (state == 1 && PUSH_AF)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, a); }
  if (state == 2 && PUSH_BC)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, c); }
  if (state == 2 && PUSH_DE)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, e); }
  if (state == 2 && PUSH_HL)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, l); }
  if (state == 2 && PUSH_AF)                /**/ { sp = adm; /**/                                          /**/ bus_write(sp, f); }
  if (state == 3 && PUSH_BC)                /**/ {           /**/                                          /**/ bus_done(pc); }
  if (state == 3 && PUSH_DE)                /**/ {           /**/                                          /**/ bus_done(pc); }
  if (state == 3 && PUSH_HL)                /**/ {           /**/                                          /**/ bus_done(pc); }
  if (state == 3 && PUSH_AF)                /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && POP_BC)                 /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
  if (state == 0 && POP_DE)                 /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
  if (state == 0 && POP_HL)                 /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
  if (state == 0 && POP_AF)                 /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
  if (state == 1 && POP_BC)                 /**/ { sp = adp; /**/ c = in;                                  /**/ bus_read(sp); }
  if (state == 1 && POP_DE)                 /**/ { sp = adp; /**/ e = in;                                  /**/ bus_read(sp); }
  if (state == 1 && POP_HL)                 /**/ { sp = adp; /**/ l = in;                                  /**/ bus_read(sp); }
  if (state == 1 && POP_AF)                 /**/ { sp = adp; /**/ f = in;                                  /**/ bus_read(sp); }
  if (state == 2 && POP_BC)                 /**/ { sp = adp; /**/ b = in;                                  /**/ bus_done(pc); }
  if (state == 2 && POP_DE)                 /**/ { sp = adp; /**/ d = in;                                  /**/ bus_done(pc); }
  if (state == 2 && POP_HL)                 /**/ { sp = adp; /**/ h = in;                                  /**/ bus_done(pc); }
  if (state == 2 && POP_AF)                 /**/ { sp = adp; /**/ a = in;                                  /**/ bus_done(pc); }

  // conditional branches

  if (state == 0 && JR_R8)                  /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && JR_R8)                  /**/ { pc = adp; /**/ pc += int8_t(in);                        /**/ bus_pass(pc); }
  if (state == 2 && JR_R8)                  /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && JR_CC_R8    &&  branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && JR_CC_R8    &&  branch) /**/ { pc = adp; /**/ pc += int8_t(in);                        /**/ bus_pass(pc); }
  if (state == 2 && JR_CC_R8    &&  branch) /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && JR_CC_R8    && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && JR_CC_R8    && !branch) /**/ { pc = adp; /**/                                          /**/ bus_done(pc); }


  if (state == 0 && JP_CC_A16   &&  branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && JP_CC_A16   &&  branch) /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
  if (state == 2 && JP_CC_A16   &&  branch) /**/ {           /**/ xyh = in;                                /**/ bus_pass(pc); }
  if (state == 3 && JP_CC_A16   &&  branch) /**/ {           /**/                                          /**/ bus_done(xy); }

  if (state == 0 && JP_CC_A16   && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && JP_CC_A16   && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 2 && JP_CC_A16   && !branch) /**/ { pc = adp; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && JP_A16)                 /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && JP_A16)                 /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
  if (state == 2 && JP_A16)                 /**/ {           /**/ xyh = in;                                /**/ bus_pass(pc); }
  if (state == 3 && JP_A16)                 /**/ {           /**/                                          /**/ bus_done(xy); }

  if (state == 0 && JP_HL)                  /**/ { pc = adp; /**/                                          /**/ bus_done(hl); }

  // calls

  if (state == 0 && CALL_CC_A16 &&  branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && CALL_CC_A16 &&  branch) /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
  if (state == 2 && CALL_CC_A16 &&  branch) /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_pass(sp); }
  if (state == 3 && CALL_CC_A16 &&  branch) /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pch); }
  if (state == 4 && CALL_CC_A16 &&  branch) /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pcl); }
  if (state == 5 && CALL_CC_A16 &&  branch) /**/ {           /**/                                          /**/ bus_done(xy); }

  if (state == 0 && CALL_CC_A16 && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && CALL_CC_A16 && !branch) /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 2 && CALL_CC_A16 && !branch) /**/ { pc = adp; /**/                                          /**/ bus_done(pc); }

  if (state == 0 && CALL_A16)               /**/ { pc = adp; /**/                                          /**/ bus_read(pc); }
  if (state == 1 && CALL_A16)               /**/ { pc = adp; /**/ xyl = in;                                /**/ bus_read(pc); }
  if (state == 2 && CALL_A16)               /**/ { pc = adp; /**/ xyh = in;                                /**/ bus_pass(sp); }
  if (state == 3 && CALL_A16)               /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pch); }
  if (state == 4 && CALL_A16)               /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pcl); }
  if (state == 5 && CALL_A16)               /**/ {           /**/                                          /**/ bus_done(xy); }

  if (state == 0 && RST_NN)                 /**/ { pc = adp; /**/                                          /**/ bus_pass(sp); }
  if (state == 1 && RST_NN)                 /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pch); }
  if (state == 2 && RST_NN)                 /**/ { sp = adm; /**/                                          /**/ bus_write(sp, pcl); }
  if (state == 3 && RST_NN)                 /**/ {           /**/ xy = op - 0xC7;                          /**/ bus_done(xy); }

  // returns

  if (state == 0 && RET_CC      && !branch) /**/ { pc = adp; /**/                                          /**/ bus_pass(pc); }
  if (state == 1 && RET_CC      && !branch) /**/ {           /**/                                          /**/ bus_done(pc); }

  if (state == 0 && RET_CC      &&  branch) /**/ { pc = adp; /**/                                          /**/ bus_pass(pc); }
  if (state == 1 && RET_CC      &&  branch) /**/ {           /**/                                          /**/ bus_read(sp); }
  if (state == 2 && RET_CC      &&  branch) /**/ { sp = adp; /**/ xyl = in;                                /**/ bus_read(sp); }
  if (state == 3 && RET_CC      &&  branch) /**/ { sp = adp; /**/ xyh = in;                                /**/ bus_pass(pc); }
  if (state == 4 && RET_CC      &&  branch) /**/ {           /**/                                          /**/ bus_done(xy); }

  if (state == 0 && RETI)                   /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
  if (state == 1 && RETI)                   /**/ { sp = adp; /**/ xyl = in;                                /**/ bus_read(sp); }
  if (state == 2 && RETI)                   /**/ { sp = adp; /**/ xyh = in;                                /**/ bus_pass(pc); }
  if (state == 3 && RETI)                   /**/ {           /**/ ime = true; ime_delay = true;            /**/ bus_done(xy); }

  if (state == 0 && RET)                    /**/ { pc = adp; /**/                                          /**/ bus_read(sp); }
  if (state == 1 && RET)                    /**/ { sp = adp; /**/ xyl = in;                                /**/ bus_read(sp); }
  if (state == 2 && RET)                    /**/ { sp = adp; /**/ xyh = in;                                /**/ bus_pass(pc); }
  if (state == 3 && RET)                    /**/ {           /**/                                          /**/ bus_done(xy); }

  f &= 0xF0;
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

uint8_t MetroBoyCPU::alu(uint8_t x, uint8_t y, int op, uint8_t flags) {
  alu_f = flags;

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

uint8_t MetroBoyCPU::rlu(uint8_t x, int op, uint8_t flags) {
  alu_f = flags;
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

uint8_t MetroBoyCPU::alu_cb(uint8_t x, int op, uint8_t flags) {
  alu_f = flags;
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
