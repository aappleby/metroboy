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
  state._bus_addr  = 0xff50;
  state._bus_data  = 1;
  state._bus_read  = 0;
  state._bus_write = 1;

  state.op_addr = 0x00FE;
  state.op_next = 0xe0;
  state.op_cb = 0x11;
  state.op_state = 1;
  state.op_state_ = 2;
  state.in = 0x50;

  state.ime = false;
  state.ime_delay = false;
  state.int_ack = 0x00;

  state.alu_f = 0xb0;
  state.alu_o = 0x00;

  state.pc = 0x0100;
  state.sp = 0xFFFE;
  state.xy = 0xFF50;
  state.bc = 0x0013;
  state.de = 0x00D8;
  state.hl = 0x014D;
  state.af = 0x01B0;
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::dump(Dumper& d_) const {
  d_("bus req : %04x:%02x %s%s\n", state._bus_addr, state._bus_data, state._bus_read  ? "\003R\001" : "-", state._bus_write ? "\002W\001" : "-");
  d_("op addr : 0x%04x\n", state.op_addr);
  d_("opname  : '%s' @ %d->%d\n", op_strings2[state.op_next], state.op_state, state.op_state_);
  d_("opcode  : 0x%02x\n", state.op_next);
  d_("CB      : 0x%02x\n", state.op_cb);
  d_("in      : 0x%02x\n", state.in);
  d_("\n");
  d_("IME     : %d\n", state.ime);
  d_("IME_    : %d\n", state.ime_delay);
  d_("isr     : %d\n", (state.op_next == 0xF4));
  d_("int_ack : 0x%02x\n", state.int_ack);
  d_("\n");
  d_("state.alu_f   : 0x%02x\n", state.alu_f);
  d_("state.alu_o   : 0x%02x\n", state.alu_o);
  d_("\n");
  d_("PC      : 0x%04x 0x%02x 0x%02x\n", state.pc, state.pcl, state.pch);
  d_("SP      : 0x%04x 0x%02x 0x%02x\n", state.sp, state.sph, state.spl);
  d_("XY      : 0x%04x 0x%02x 0x%02x\n", state.xy, state.xyh, state.xyl);
  d_("BC      : 0x%04x 0x%02x 0x%02x\n", state.bc, state.b,   state.c);
  d_("DE      : 0x%04x 0x%02x 0x%02x\n", state.de, state.d,   state.e);
  d_("HL      : 0x%04x 0x%02x 0x%02x\n", state.hl, state.h,   state.l);
  d_("AF      : 0x%04x 0x%02x 0x%02x\n", state.af, state.a,   state.f);
  d_("\n");
}

//-----------------------------------------------------------------------------
// Do the meat of executing the instruction

/*
void MetroBoyCPU::tock_ab(uint8_t imask, uint8_t intf_gh, uint8_t bus_data) {
  state = state_;

  if (_bus_read) in = bus_data;

  if (state == 0) {
    op_addr = state._bus_addr;
    op = bus_data;

    if ((imask & intf_gh) && ime) {
      op = 0xF4; // fake opcode
      ime = false;
      ime_delay = false;
    }
  }

  int_ack = 0;
  ime = ime_delay; // must be after int check, before op execution

  // #define HALT (op == 0x76)

  if      (INT)       execute_int(imask, intf_gh);
  else if (HALT)      execute_halt(imask, intf_gh);
  else if (PREFIX_CB) execute_cb();
  else                execute_op();
}
*/

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_int(uint8_t imask_, uint8_t intf_) {
  state.op_state = state.op_state_;
  state.op_state_ = state.op_state + 1;

  if (state.op_state == 2) {
    if      (imask_ & intf_ & INT_VBLANK_MASK) { state.int_addr = 0x40; }
    else if (imask_ & intf_ & INT_STAT_MASK)   { state.int_addr = 0x48; }
    else if (imask_ & intf_ & INT_TIMER_MASK)  { state.int_addr = 0x50; }
    else if (imask_ & intf_ & INT_SERIAL_MASK) { state.int_addr = 0x58; }
    else if (imask_ & intf_ & INT_JOYPAD_MASK) { state.int_addr = 0x60; }
    else                                       { state.int_addr = 0x00; }

    if      (imask_ & intf_ & INT_VBLANK_MASK) { state.int_ack = INT_VBLANK_MASK; }
    else if (imask_ & intf_ & INT_STAT_MASK)   { state.int_ack = INT_STAT_MASK; }
    else if (imask_ & intf_ & INT_TIMER_MASK)  { state.int_ack = INT_TIMER_MASK; }
    else if (imask_ & intf_ & INT_SERIAL_MASK) { state.int_ack = INT_SERIAL_MASK; }
    else if (imask_ & intf_ & INT_JOYPAD_MASK) { state.int_ack = INT_JOYPAD_MASK; }
    else                                       { state.int_ack = 0; }
  }

  // we don't have to reset pc from state._bus_addr...
  if      (state.op_state == 0) { state.pc = state._bus_addr;     bus_pass(state.sp); }
  else if (state.op_state == 1) { state.sp = state._bus_addr - 1; bus_write(state.sp, state.pch); }
  else if (state.op_state == 2) { state.sp = state._bus_addr - 1; bus_write(state.sp, state.pcl); }
  else if (state.op_state == 3) {                                 bus_pass(state.pc); }
  else if (state.op_state == 4) {                                 bus_done(state.int_addr); state.op_state_ = 0; }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_halt(uint8_t imask_, uint8_t intf_) {
  state.op_state = state.op_state_;
  if (state.op_state == 0) {
    state.pc = state._bus_addr + 1;
    bus_read(state.pc);
    state.op_state_ = !(imask_& intf_);

    //if (!ime) state_ = 0;
  }
  else if (state.op_state == 1) {
    state.pc = state._bus_addr;
    bus_read(state.pc);
    state.op_state_ = 1;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_cb() {
  state.op_state = state.op_state_;
  auto op = state.op_next;

  state.op_state_ = state.op_state + 1;

  uint16_t ad = state._bus_addr;
  uint16_t adp = ad + 1;

  if (state.op_state == 1) state.op_cb = state.in;
  uint8_t mask = cb_flag_mask[CB_QUAD];

  bool OP_CB_BIT = PREFIX_CB && (CB_QUAD == 1);

  if (state.op_state == 0) {
    state.pc = adp; bus_read(state.pc);
  }
  else if (OP_CB_R) {
    if (state.op_state == 1)   { state.pc = adp; set_reg(CB_COL, alu_cb(get_reg(CB_COL), state.op_cb, state.f)); set_f(mask); bus_done(state.pc); state.op_state_ = 0; }
  }
  else {
    if (OP_CB_BIT) {
      if (state.op_state == 1) { state.pc = adp;                                bus_read(state.hl); state.op_state_ = 2;}
      if (state.op_state == 2) { alu_cb(state.in, state.op_cb, state.f); set_f(mask); bus_done(state.pc); state.op_state_ = 0;}
    }
    else {
      if (state.op_state == 1) { state.pc = adp;                                bus_read(state.hl); }
      if (state.op_state == 2) { alu_cb(state.in, state.op_cb, state.f); set_f(mask); bus_write(state.hl, state.alu_o); }
      if (state.op_state == 3) {                                                bus_done(state.pc); state.op_state_ = 0;}
    }
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_op() {
  state.op_state = state.op_state_;
  auto op = state.op_next;

  uint16_t ad = state._bus_addr;
  uint16_t adp = ad + 1;
  uint16_t adm = ad - 1;

  bool branch = false;
  switch (OP_ROW & 3) {
  case 0: branch = !(state.f & F_ZERO); break;
  case 1: branch =  (state.f & F_ZERO); break;
  case 2: branch = !(state.f & F_CARRY); break;
  case 3: branch =  (state.f & F_CARRY); break;
  }

  // IN OUT BC DE HL AF XY | PC SP AD

  // misc

  if (state.op_state == 0 && NOP)                    /**/ { state.pc = adp; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }
  if (state.op_state == 0 && STOP)                   /**/ { state.pc = adp; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }
  if (state.op_state == 0 && DI)                     /**/ { state.pc = adp; /**/ state.ime = false; state.ime_delay = false;          /**/ bus_done(state.pc); state.op_state_ = 0; }
  if (state.op_state == 0 && EI)                     /**/ { state.pc = adp; /**/ state.ime = state.ime_delay; state.ime_delay = true; /**/ bus_done(state.pc); state.op_state_ = 0; }
  if (state.op_state == 0 && MV_R_R)                 /**/ { state.pc = adp; /**/ set_reg(OP_ROW, get_reg(OP_COL));        /**/ bus_done(state.pc); state.op_state_ = 0; }
  if (state.op_state == 0 && LD_SP_HL)               /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.hl); state.op_state_ = 1; }
  if (state.op_state == 1 && LD_SP_HL)               /**/ { state.sp = ad;  /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  // load immediate

  if (state.op_state == 0 && LD_R_D8)                /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && LD_R_D8)                /**/ { state.pc = adp; /**/ set_reg(OP_ROW, state.in);               /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && LD_BC_D16)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc); state.op_state_ = 1; }
  if (state.op_state == 0 && LD_DE_D16)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc); state.op_state_ = 1; }
  if (state.op_state == 0 && LD_HL_D16)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc); state.op_state_ = 1; }
  if (state.op_state == 0 && LD_SP_D16)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && LD_BC_D16)              /**/ { state.pc = adp; /**/ state.c   = state.in;                    /**/ bus_read(state.pc); state.op_state_ = 2; }
  if (state.op_state == 1 && LD_DE_D16)              /**/ { state.pc = adp; /**/ state.e   = state.in;                    /**/ bus_read(state.pc); state.op_state_ = 2; }
  if (state.op_state == 1 && LD_HL_D16)              /**/ { state.pc = adp; /**/ state.l   = state.in;                    /**/ bus_read(state.pc); state.op_state_ = 2; }
  if (state.op_state == 1 && LD_SP_D16)              /**/ { state.pc = adp; /**/ state.spl = state.in;                    /**/ bus_read(state.pc); state.op_state_ = 2; }
  if (state.op_state == 2 && LD_BC_D16)              /**/ { state.pc = adp; /**/ state.b   = state.in;                    /**/ bus_done(state.pc); state.op_state_ = 0; }
  if (state.op_state == 2 && LD_DE_D16)              /**/ { state.pc = adp; /**/ state.d   = state.in;                    /**/ bus_done(state.pc); state.op_state_ = 0; }
  if (state.op_state == 2 && LD_HL_D16)              /**/ { state.pc = adp; /**/ state.h   = state.in;                    /**/ bus_done(state.pc); state.op_state_ = 0; }
  if (state.op_state == 2 && LD_SP_D16)              /**/ { state.pc = adp; /**/ state.sph = state.in;                    /**/ bus_done(state.pc); state.op_state_ = 0; }

  // 8-bit alu

  if (state.op_state == 0 && ALU_A_R)                /**/ { state.pc = adp; /**/ state.a = alu(state.a, get_reg(OP_COL), OP_ROW, state.f);   set_f(0xF0); /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && INC_R)                  /**/ { state.pc = adp; /**/ set_reg(OP_ROW, alu(get_reg(OP_ROW), 1, 1, 0)); set_f(0xE0);             /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && DEC_R)                  /**/ { state.pc = adp; /**/ set_reg(OP_ROW, alu(get_reg(OP_ROW), 1, 3, 0)); set_f(0xE0);             /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && RLC_A)                  /**/ { state.pc = adp; /**/ state.a = rlu(state.a, OP_ROW, state.f); set_f(0xF0);                    /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && RRC_A)                  /**/ { state.pc = adp; /**/ state.a = rlu(state.a, OP_ROW, state.f); set_f(0xF0);                    /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && RL_A)                   /**/ { state.pc = adp; /**/ state.a = rlu(state.a, OP_ROW, state.f); set_f(0xF0);                    /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && RR_A)                   /**/ { state.pc = adp; /**/ state.a = rlu(state.a, OP_ROW, state.f); set_f(0xF0);                    /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && DAA)                    /**/ { state.pc = adp; /**/ state.a = rlu(state.a, OP_ROW, state.f); set_f(0xB0);                    /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && CPL)                    /**/ { state.pc = adp; /**/ state.a = rlu(state.a, OP_ROW, state.f); set_f(0x60);                    /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && SCF)                    /**/ { state.pc = adp; /**/ state.a = rlu(state.a, OP_ROW, state.f); set_f(0x70);                    /**/ bus_done(state.pc); state.op_state_ = 0;}
  if (state.op_state == 0 && CCF)                    /**/ { state.pc = adp; /**/ state.a = rlu(state.a, OP_ROW, state.f); set_f(0x70);                    /**/ bus_done(state.pc); state.op_state_ = 0;}

  if (state.op_state == 0 && ALU_A_D8)               /**/ { state.pc = adp; /**/                                                                  /**/ bus_read(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && ALU_A_D8)               /**/ { state.pc = adp; /**/ state.a = alu(state.a, state.in, OP_ROW, state.f); set_f(0xF0);  /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && ALU_A_HL)               /**/ { state.pc = adp; /**/                                                                  /**/ bus_read(state.hl); state.op_state_ = 1; }
  if (state.op_state == 1 && ALU_A_HL)               /**/ {                 /**/ state.a = alu(state.a, state.in, OP_ROW, state.f); set_f(0xF0);  /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && INC_AT_HL)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.hl);               state.op_state_ = 1; }
  if (state.op_state == 1 && INC_AT_HL)              /**/ {                 /**/ alu(state.in, 1, 1, 0); set_f(0xE0);     /**/ bus_write(state.hl, state.alu_o); state.op_state_ = 2; }
  if (state.op_state == 2 && INC_AT_HL)              /**/ {                 /**/                                          /**/ bus_done(state.pc);               state.op_state_ = 0; }

  if (state.op_state == 0 && DEC_AT_HL)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.hl);               state.op_state_ = 1; }
  if (state.op_state == 1 && DEC_AT_HL)              /**/ {                 /**/ alu(state.in, 1, 3, 0); set_f(0xE0);     /**/ bus_write(state.hl, state.alu_o); state.op_state_ = 2; }
  if (state.op_state == 2 && DEC_AT_HL)              /**/ {                 /**/                                          /**/ bus_done(state.pc);               state.op_state_ = 0; }

  // 16-bit alu

  if (state.op_state == 0 && ADD_SP_R8)              /**/ { state.pc = adp; /**/                                                                  /**/ bus_read(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && ADD_SP_R8)              /**/ { state.pc = adp; /**/ state.spl = alu(state.in, state.spl, 0, state.f); set_f(0xF0);   /**/ bus_pass(state.pc); state.op_state_ = 2; }
  if (state.op_state == 2 && ADD_SP_R8)              /**/ {                 /**/ state.sph = alu(sxt(state.in), state.sph, 1, state.f);           /**/ bus_pass(state.pc); state.op_state_ = 3; }
  if (state.op_state == 3 && ADD_SP_R8)              /**/ {                 /**/                                                                  /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && LD_HL_SP_R8)            /**/ { state.pc = adp; /**/                                                                  /**/ bus_read(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && LD_HL_SP_R8)            /**/ { state.pc = adp; /**/ state.l = alu(state.in, state.spl, 0, state.f); set_f(0xF0);     /**/ bus_pass(state.pc); state.op_state_ = 2; }
  if (state.op_state == 2 && LD_HL_SP_R8)            /**/ {                 /**/ state.h = alu(sxt(state.in), state.sph, 1, state.f);             /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && INC_BC)                 /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.bc); state.op_state_ = 1; }
  if (state.op_state == 1 && INC_BC)                 /**/ { state.bc = adp; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && DEC_BC)                 /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.bc); state.op_state_ = 1; }
  if (state.op_state == 1 && DEC_BC)                 /**/ { state.bc = adm; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && INC_DE)                 /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.de); state.op_state_ = 1; }
  if (state.op_state == 1 && INC_DE)                 /**/ { state.de = adp; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && DEC_DE)                 /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.de); state.op_state_ = 1; }
  if (state.op_state == 1 && DEC_DE)                 /**/ { state.de = adm; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && INC_HL)                 /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.hl); state.op_state_ = 1; }
  if (state.op_state == 1 && INC_HL)                 /**/ { state.hl = adp; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && DEC_HL)                 /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.hl); state.op_state_ = 1; }
  if (state.op_state == 1 && DEC_HL)                 /**/ { state.hl = adm; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && INC_SP)                 /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.sp); state.op_state_ = 1; }
  if (state.op_state == 1 && INC_SP)                 /**/ { state.sp = adp; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && DEC_SP)                 /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.sp); state.op_state_ = 1; }
  if (state.op_state == 1 && DEC_SP)                 /**/ { state.sp = adm; /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && ADD_HL_BC)              /**/ { state.pc = adp; /**/ state.l = alu(state.c, state.l, 0, state.f);                     /**/ bus_pass(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && ADD_HL_BC)              /**/ {                 /**/ state.h = alu(state.b, state.h, 1, state.alu_f); set_f(0x70);    /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && ADD_HL_DE)              /**/ { state.pc = adp; /**/ state.l = alu(state.e, state.l, 0, state.f);                     /**/ bus_pass(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && ADD_HL_DE)              /**/ {                 /**/ state.h = alu(state.d, state.h, 1, state.alu_f); set_f(0x70);    /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && ADD_HL_HL)              /**/ { state.pc = adp; /**/ state.l = alu(state.l, state.l, 0, state.f);                     /**/ bus_pass(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && ADD_HL_HL)              /**/ {                 /**/ state.h = alu(state.h, state.h, 1, state.alu_f); set_f(0x70);    /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && ADD_HL_SP)              /**/ { state.pc = adp; /**/ state.l = alu(state.spl, state.l, 0, state.f);                   /**/ bus_pass(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && ADD_HL_SP)              /**/ {                 /**/ state.h = alu(state.sph, state.h, 1, state.alu_f); set_f(0x70);  /**/ bus_done(state.pc); state.op_state_ = 0; }

  // load/store

  if (state.op_state == 0 && STM_A16_SP)             /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);             state.op_state_ = 1; }
  if (state.op_state == 1 && STM_A16_SP)             /**/ { state.pc = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.pc);             state.op_state_ = 2; }
  if (state.op_state == 2 && STM_A16_SP)             /**/ { state.pc = adp; /**/ state.xyh = state.in;                    /**/ bus_write(state.xy, state.spl); state.op_state_ = 3; }
  if (state.op_state == 3 && STM_A16_SP)             /**/ { state.xy = adp; /**/                                          /**/ bus_write(state.xy, state.sph); state.op_state_ = 4; }
  if (state.op_state == 4 && STM_A16_SP)             /**/ { state.xy = adp; /**/                                          /**/ bus_done(state.pc);             state.op_state_ = 0; }

  if (state.op_state == 0 && STM_A16_A)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);             state.op_state_ = 1; }
  if (state.op_state == 1 && STM_A16_A)              /**/ { state.pc = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.pc);             state.op_state_ = 2; }
  if (state.op_state == 2 && STM_A16_A)              /**/ { state.pc = adp; /**/ state.xyh = state.in;                    /**/ bus_write(state.xy, state.a);   state.op_state_ = 3; }
  if (state.op_state == 3 && STM_A16_A)              /**/ {                 /**/                                          /**/ bus_done(state.pc);             state.op_state_ = 0; }

  if (state.op_state == 0 && LDM_A_A16)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);       state.op_state_ = 1; }
  if (state.op_state == 1 && LDM_A_A16)              /**/ { state.pc = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.pc);       state.op_state_ = 2; }
  if (state.op_state == 2 && LDM_A_A16)              /**/ { state.pc = adp; /**/ state.xyh = state.in;                    /**/ bus_read(state.xy);       state.op_state_ = 3; }
  if (state.op_state == 3 && LDM_A_A16)              /**/ {                 /**/ state.a = state.in;                      /**/ bus_done(state.pc);       state.op_state_ = 0; }

  // indirect load/store

  if (state.op_state == 0 && LDM_A_BC)               /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.bc);            state.op_state_ = 1; }
  if (state.op_state == 1 && LDM_A_BC)               /**/ {                 /**/ state.a = state.in;                      /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && LDM_A_DE)               /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.de);            state.op_state_ = 1; }
  if (state.op_state == 1 && LDM_A_DE)               /**/ {                 /**/ state.a = state.in;                      /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && LDM_R_HL)               /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.hl);            state.op_state_ = 1; }
  if (state.op_state == 1 && LDM_R_HL)               /**/ {                 /**/ set_reg(OP_ROW, state.in);               /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && LDM_A_HLP)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.hl);            state.op_state_ = 1; }
  if (state.op_state == 1 && LDM_A_HLP)              /**/ { state.hl = adp; /**/ state.a = state.in;                      /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && LDM_A_HLM)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.hl);            state.op_state_ = 1; }
  if (state.op_state == 1 && LDM_A_HLM)              /**/ { state.hl = adm; /**/ state.a = state.in;                      /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && STM_BC_A)               /**/ { state.pc = adp; /**/                                          /**/ bus_write(state.bc, state.a);  state.op_state_ = 1; }
  if (state.op_state == 1 && STM_BC_A)               /**/ {                 /**/                                          /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && STM_DE_A)               /**/ { state.pc = adp; /**/                                          /**/ bus_write(state.de, state.a);  state.op_state_ = 1; }
  if (state.op_state == 1 && STM_DE_A)               /**/ {                 /**/                                          /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && STM_HL_R)               /**/ { state.pc = adp; /**/ uint8_t t = get_reg(OP_COL);             /**/ bus_write(state.hl, t);        state.op_state_ = 1; }
  if (state.op_state == 1 && STM_HL_R)               /**/ {                 /**/                                          /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && STM_HL_D8)              /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);            state.op_state_ = 1; }
  if (state.op_state == 1 && STM_HL_D8)              /**/ { state.pc = adp; /**/                                          /**/ bus_write(state.hl, state.in); state.op_state_ = 2; }
  if (state.op_state == 2 && STM_HL_D8)              /**/ {                 /**/                                          /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && STM_HLP_A)              /**/ { state.pc = adp; /**/                                          /**/ bus_write(state.hl, state.a);  state.op_state_ = 1; }
  if (state.op_state == 1 && STM_HLP_A)              /**/ { state.hl = adp; /**/                                          /**/ bus_done(state.pc);            state.op_state_ = 0; }

  if (state.op_state == 0 && STM_HLM_A)              /**/ { state.pc = adp; /**/                                          /**/ bus_write(state.hl, state.a);  state.op_state_ = 1; }
  if (state.op_state == 1 && STM_HLM_A)              /**/ { state.hl = adm; /**/                                          /**/ bus_done(state.pc);            state.op_state_ = 0; }

  // zero-page load/store

  if (state.op_state == 0 && LDM_A_C)                /**/ { state.pc = adp; /**/ state.xy = 0xFF00 | state.c;             /**/ bus_read(state.xy);           state.op_state_ = 1; }
  if (state.op_state == 1 && LDM_A_C)                /**/ {                 /**/ state.a = state.in;                      /**/ bus_done(state.pc);           state.op_state_ = 0; }

  if (state.op_state == 0 && LDM_A_A8)               /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);           state.op_state_ = 1; }
  if (state.op_state == 1 && LDM_A_A8)               /**/ { state.pc = adp; /**/ state.xy = 0xFF00 | state.in;            /**/ bus_read(state.xy);           state.op_state_ = 2; }
  if (state.op_state == 2 && LDM_A_A8)               /**/ {                 /**/ state.a = state.in;                      /**/ bus_done(state.pc);           state.op_state_ = 0; }

  if (state.op_state == 0 && STM_C_A)                /**/ { state.pc = adp; /**/ state.xy = 0xFF00 | state.c;             /**/ bus_write(state.xy, state.a); state.op_state_ = 1; }
  if (state.op_state == 1 && STM_C_A)                /**/ {                 /**/                                          /**/ bus_done(state.pc);           state.op_state_ = 0; }

  if (state.op_state == 0 && STM_A8_A)               /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);           state.op_state_ = 1; }
  if (state.op_state == 1 && STM_A8_A)               /**/ { state.pc = adp; /**/ state.xy = 0xFF00 | state.in;            /**/ bus_write(state.xy, state.a); state.op_state_ = 2; }
  if (state.op_state == 2 && STM_A8_A)               /**/ {                 /**/                                          /**/ bus_done(state.pc);           state.op_state_ = 0; }

  // push / pop

  if (state.op_state == 0 && PUSH_BC)                /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.sp);           state.op_state_ = 1; }
  if (state.op_state == 0 && PUSH_DE)                /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.sp);           state.op_state_ = 1; }
  if (state.op_state == 0 && PUSH_HL)                /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.sp);           state.op_state_ = 1; }
  if (state.op_state == 0 && PUSH_AF)                /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.sp);           state.op_state_ = 1; }
  if (state.op_state == 1 && PUSH_BC)                /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.b); state.op_state_ = 2; }
  if (state.op_state == 1 && PUSH_DE)                /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.d); state.op_state_ = 2; }
  if (state.op_state == 1 && PUSH_HL)                /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.h); state.op_state_ = 2; }
  if (state.op_state == 1 && PUSH_AF)                /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.a); state.op_state_ = 2; }
  if (state.op_state == 2 && PUSH_BC)                /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.c); state.op_state_ = 3; }
  if (state.op_state == 2 && PUSH_DE)                /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.e); state.op_state_ = 3; }
  if (state.op_state == 2 && PUSH_HL)                /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.l); state.op_state_ = 3; }
  if (state.op_state == 2 && PUSH_AF)                /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.f); state.op_state_ = 3; }
  if (state.op_state == 3 && PUSH_BC)                /**/ {                 /**/                                          /**/ bus_done(state.pc);           state.op_state_ = 0; }
  if (state.op_state == 3 && PUSH_DE)                /**/ {                 /**/                                          /**/ bus_done(state.pc);           state.op_state_ = 0; }
  if (state.op_state == 3 && PUSH_HL)                /**/ {                 /**/                                          /**/ bus_done(state.pc);           state.op_state_ = 0; }
  if (state.op_state == 3 && PUSH_AF)                /**/ {                 /**/                                          /**/ bus_done(state.pc);           state.op_state_ = 0; }

  if (state.op_state == 0 && POP_BC)                 /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.sp);     state.op_state_ = 1; }
  if (state.op_state == 0 && POP_DE)                 /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.sp);     state.op_state_ = 1; }
  if (state.op_state == 0 && POP_HL)                 /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.sp);     state.op_state_ = 1; }
  if (state.op_state == 0 && POP_AF)                 /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.sp);     state.op_state_ = 1; }
  if (state.op_state == 1 && POP_BC)                 /**/ { state.sp = adp; /**/ state.c = state.in;                      /**/ bus_read(state.sp);     state.op_state_ = 2; }
  if (state.op_state == 1 && POP_DE)                 /**/ { state.sp = adp; /**/ state.e = state.in;                      /**/ bus_read(state.sp);     state.op_state_ = 2; }
  if (state.op_state == 1 && POP_HL)                 /**/ { state.sp = adp; /**/ state.l = state.in;                      /**/ bus_read(state.sp);     state.op_state_ = 2; }
  if (state.op_state == 1 && POP_AF)                 /**/ { state.sp = adp; /**/ state.f = state.in;                      /**/ bus_read(state.sp);     state.op_state_ = 2; }
  if (state.op_state == 2 && POP_BC)                 /**/ { state.sp = adp; /**/ state.b = state.in;                      /**/ bus_done(state.pc);     state.op_state_ = 0; }
  if (state.op_state == 2 && POP_DE)                 /**/ { state.sp = adp; /**/ state.d = state.in;                      /**/ bus_done(state.pc);     state.op_state_ = 0; }
  if (state.op_state == 2 && POP_HL)                 /**/ { state.sp = adp; /**/ state.h = state.in;                      /**/ bus_done(state.pc);     state.op_state_ = 0; }
  if (state.op_state == 2 && POP_AF)                 /**/ { state.sp = adp; /**/ state.a = state.in;                      /**/ bus_done(state.pc);     state.op_state_ = 0; }

  // conditional branches

  if (state.op_state == 0 && JR_R8)                  /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);     state.op_state_ = 1; }
  if (state.op_state == 1 && JR_R8)                  /**/ { state.pc = adp; /**/ state.pc += int8_t(state.in);            /**/ bus_pass(state.pc);     state.op_state_ = 2; }
  if (state.op_state == 2 && JR_R8)                  /**/ {                 /**/                                          /**/ bus_done(state.pc);     state.op_state_ = 0; }

  if (state.op_state == 0 && JR_CC_R8    &&  branch) /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);     state.op_state_ = 1; }
  if (state.op_state == 1 && JR_CC_R8    &&  branch) /**/ { state.pc = adp; /**/ state.pc += int8_t(state.in);            /**/ bus_pass(state.pc);     state.op_state_ = 2; }
  if (state.op_state == 2 && JR_CC_R8    &&  branch) /**/ {                 /**/                                          /**/ bus_done(state.pc);     state.op_state_ = 0; }

  if (state.op_state == 0 && JR_CC_R8    && !branch) /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);     state.op_state_ = 1; }
  if (state.op_state == 1 && JR_CC_R8    && !branch) /**/ { state.pc = adp; /**/                                          /**/ bus_done(state.pc);     state.op_state_ = 0; }


  if (state.op_state == 0 && JP_CC_A16   &&  branch) /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);     state.op_state_ = 1; }
  if (state.op_state == 1 && JP_CC_A16   &&  branch) /**/ { state.pc = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.pc);     state.op_state_ = 2; }
  if (state.op_state == 2 && JP_CC_A16   &&  branch) /**/ {                 /**/ state.xyh = state.in;                    /**/ bus_pass(state.pc);     state.op_state_ = 3; }
  if (state.op_state == 3 && JP_CC_A16   &&  branch) /**/ {                 /**/                                          /**/ bus_done(state.xy);     state.op_state_ = 0; }

  if (state.op_state == 0 && JP_CC_A16   && !branch) /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);     state.op_state_ = 1; }
  if (state.op_state == 1 && JP_CC_A16   && !branch) /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);     state.op_state_ = 2; }
  if (state.op_state == 2 && JP_CC_A16   && !branch) /**/ { state.pc = adp; /**/                                          /**/ bus_done(state.pc);     state.op_state_ = 0; }

  if (state.op_state == 0 && JP_A16)                 /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);     state.op_state_ = 1; }
  if (state.op_state == 1 && JP_A16)                 /**/ { state.pc = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.pc);     state.op_state_ = 2; }
  if (state.op_state == 2 && JP_A16)                 /**/ {                 /**/ state.xyh = state.in;                    /**/ bus_pass(state.pc);     state.op_state_ = 3; }
  if (state.op_state == 3 && JP_A16)                 /**/ {                 /**/                                          /**/ bus_done(state.xy);     state.op_state_ = 0; }

  if (state.op_state == 0 && JP_HL)                  /**/ { state.pc = adp; /**/                                          /**/ bus_done(state.hl);     state.op_state_ = 0; }

  // calls

  if (state.op_state == 0 && CALL_CC_A16 &&  branch) /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);       state.op_state_ = 1; }
  if (state.op_state == 1 && CALL_CC_A16 &&  branch) /**/ { state.pc = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.pc);       state.op_state_ = 2; }
  if (state.op_state == 2 && CALL_CC_A16 &&  branch) /**/ { state.pc = adp; /**/ state.xyh = state.in;                    /**/ bus_pass(state.sp);       state.op_state_ = 3; }
  if (state.op_state == 3 && CALL_CC_A16 &&  branch) /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.pch); state.op_state_ = 4; }
  if (state.op_state == 4 && CALL_CC_A16 &&  branch) /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.pcl); state.op_state_ = 5; }
  if (state.op_state == 5 && CALL_CC_A16 &&  branch) /**/ {                 /**/                                          /**/ bus_done(state.xy);       state.op_state_ = 0; }

  if (state.op_state == 0 && CALL_CC_A16 && !branch) /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);       state.op_state_ = 1; }
  if (state.op_state == 1 && CALL_CC_A16 && !branch) /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);       state.op_state_ = 2; }
  if (state.op_state == 2 && CALL_CC_A16 && !branch) /**/ { state.pc = adp; /**/                                          /**/ bus_done(state.pc);       state.op_state_ = 0; }

  if (state.op_state == 0 && CALL_A16)               /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.pc);       state.op_state_ = 1; }
  if (state.op_state == 1 && CALL_A16)               /**/ { state.pc = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.pc);       state.op_state_ = 2; }
  if (state.op_state == 2 && CALL_A16)               /**/ { state.pc = adp; /**/ state.xyh = state.in;                    /**/ bus_pass(state.sp);       state.op_state_ = 3; }
  if (state.op_state == 3 && CALL_A16)               /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.pch); state.op_state_ = 4; }
  if (state.op_state == 4 && CALL_A16)               /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.pcl); state.op_state_ = 5; }
  if (state.op_state == 5 && CALL_A16)               /**/ {                 /**/                                          /**/ bus_done(state.xy);       state.op_state_ = 0; }

  if (state.op_state == 0 && RST_NN)                 /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.sp);       state.op_state_ = 1; }
  if (state.op_state == 1 && RST_NN)                 /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.pch); state.op_state_ = 2; }
  if (state.op_state == 2 && RST_NN)                 /**/ { state.sp = adm; /**/                                          /**/ bus_write(state.sp, state.pcl); state.op_state_ = 3; }
  if (state.op_state == 3 && RST_NN)                 /**/ {                 /**/ state.xy = state.op_next - 0xC7;         /**/ bus_done(state.xy);       state.op_state_ = 0; }

  // returns

  if (state.op_state == 0 && RET_CC      && !branch) /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && RET_CC      && !branch) /**/ {                 /**/                                          /**/ bus_done(state.pc); state.op_state_ = 0; }

  if (state.op_state == 0 && RET_CC      &&  branch) /**/ { state.pc = adp; /**/                                          /**/ bus_pass(state.pc); state.op_state_ = 1; }
  if (state.op_state == 1 && RET_CC      &&  branch) /**/ {                 /**/                                          /**/ bus_read(state.sp); state.op_state_ = 2; }
  if (state.op_state == 2 && RET_CC      &&  branch) /**/ { state.sp = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.sp); state.op_state_ = 3; }
  if (state.op_state == 3 && RET_CC      &&  branch) /**/ { state.sp = adp; /**/ state.xyh = state.in;                    /**/ bus_pass(state.pc); state.op_state_ = 4; }
  if (state.op_state == 4 && RET_CC      &&  branch) /**/ {                 /**/                                          /**/ bus_done(state.xy); state.op_state_ = 0; }

  if (state.op_state == 0 && RETI)                   /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.sp); state.op_state_ = 1; }
  if (state.op_state == 1 && RETI)                   /**/ { state.sp = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.sp); state.op_state_ = 2; }
  if (state.op_state == 2 && RETI)                   /**/ { state.sp = adp; /**/ state.xyh = state.in;                    /**/ bus_pass(state.pc); state.op_state_ = 3; }
  if (state.op_state == 3 && RETI)                   /**/ {                 /**/ state.ime = true; state.ime_delay = true;/**/ bus_done(state.xy); state.op_state_ = 0; }

  if (state.op_state == 0 && RET)                    /**/ { state.pc = adp; /**/                                          /**/ bus_read(state.sp); state.op_state_ = 1; }
  if (state.op_state == 1 && RET)                    /**/ { state.sp = adp; /**/ state.xyl = state.in;                    /**/ bus_read(state.sp); state.op_state_ = 2; }
  if (state.op_state == 2 && RET)                    /**/ { state.sp = adp; /**/ state.xyh = state.in;                    /**/ bus_pass(state.pc); state.op_state_ = 3; }
  if (state.op_state == 3 && RET)                    /**/ {                 /**/                                          /**/ bus_done(state.xy); state.op_state_ = 0; }

  state.f &= 0xF0;
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::get_reg(int mux) {
  switch(mux) {
  case 0: return state.b;
  case 1: return state.c;
  case 2: return state.d;
  case 3: return state.e;
  case 4: return state.h;
  case 5: return state.l;
  case 6: return 0;
  case 7: return state.a;
  }
  return 0;
}

void MetroBoyCPU::set_reg(int mux, uint8_t data) {
  switch(mux) {
  case 0: state.b = data; break;
  case 1: state.c = data; break;
  case 2: state.d = data; break;
  case 3: state.e = data; break;
  case 4: state.h = data; break;
  case 5: state.l = data; break;
  case 6: break;
  case 7: state.a = data; break;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::set_f(uint8_t mask) {
  auto op = state.op_next;

  state.f = (state.f & ~mask) | (state.alu_f & mask);
  if (ADD_SP_R8)   { state.f &= ~(F_ZERO | F_NEGATIVE); }
  if (LD_HL_SP_R8) { state.f &= ~(F_ZERO | F_NEGATIVE); }
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::alu(uint8_t x, uint8_t y, int op, uint8_t flags) {
  state.alu_f = flags;

  uint16_t d1 = 0, d2 = 0;
  const bool c = state.alu_f & F_CARRY;

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

  state.alu_o = (uint8_t)d2;
  state.alu_f = (op == 2 || op == 3 || op == 7) ? F_NEGATIVE : 0;

  if (d1 & 0x010) state.alu_f |= F_HALF_CARRY;
  if (d2 & 0x100) state.alu_f |= F_CARRY;
  if (state.alu_o == 0x000) state.alu_f |= F_ZERO;
  if (op == 7)  state.alu_o = x;

  return state.alu_o;
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::rlu(uint8_t x, int op, uint8_t flags) {
  state.alu_f = flags;
  const uint8_t f = state.alu_f;

  uint8_t alu_o = 0;

  switch (op) {
  case 0:
    state.alu_o = (x << 1) | (x >> 7);
    state.alu_f = (x >> 7) ? F_CARRY : 0;
    state.alu_f &= ~F_ZERO;
    break;
  case 1:
    state.alu_o = (x >> 1) | (x << 7);
    state.alu_f = (x & 1) ? F_CARRY : 0;
    state.alu_f &= ~F_ZERO;
    break;
  case 2:
    state.alu_o = (x << 1) | (f & F_CARRY ? 1 : 0);
    state.alu_f = (x >> 7) ? F_CARRY : 0;
    state.alu_f &= ~F_ZERO;
    break;
  case 3:
    state.alu_o = (x >> 1) | ((f & F_CARRY ? 1 : 0) << 7);
    state.alu_f = (x & 1) ? F_CARRY : 0;
    state.alu_f &= ~F_ZERO;
    break;
  case 4:
    state.alu_o = daa(x, f);
    if (!state.alu_o) state.alu_f |= F_ZERO;
    break;
  case 5:
    state.alu_o = ~x;
    state.alu_f = f | 0x60;
    if (!state.alu_o) state.alu_f |= F_ZERO;
    break;
  case 6:
    state.alu_o = x;
    state.alu_f = (f & 0x80) | 0x10;
    if (!state.alu_o) state.alu_f |= F_ZERO;
    break;
  case 7:
    state.alu_o = x;
    state.alu_f = (f & 0x90) ^ 0x10;
    if (!state.alu_o) state.alu_f |= F_ZERO;
    break;
  }

  return state.alu_o;
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
  state.alu_f = 0;
  if (c) state.alu_f |= F_CARRY;
  if ((hi >> 4) && !n) state.alu_f |= F_CARRY;
  if (!state.alu_o) state.alu_f |= F_ZERO;
  return state.alu_o;
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::alu_cb(uint8_t x, int op, uint8_t flags) {
  state.alu_f = flags;
  const uint8_t quad = ((op >> 6) & 3);
  const uint8_t row = ((op >> 3) & 7);
  const bool bit_mux = (x >> row) & 1;

  state.alu_o = 0;

  switch (quad) {
  case 0:
    switch (row) {
    case 0:
      state.alu_o = (x << 1) | (x >> 7);
      state.alu_f = (x >> 7) ? F_CARRY : 0;
      break;
    case 1:
      state.alu_o = (x >> 1) | (x << 7);
      state.alu_f = (x & 1) ? F_CARRY : 0;
      break;
    case 2:
      state.alu_o = (x << 1) | (state.f & F_CARRY ? 1 : 0);
      state.alu_f = (x >> 7) ? F_CARRY : 0;
      break;
    case 3:
      state.alu_o = (x >> 1) | ((state.f & F_CARRY ? 1 : 0) << 7);
      state.alu_f = (x & 1) ? F_CARRY : 0;
      break;
    case 4:
      state.alu_o = (x << 1) & 0xFF;
      state.alu_f = 0;
      if (x >> 7)     state.alu_f |= F_CARRY;
      break;
    case 5:
      state.alu_o = ((x >> 1) | (x & 0x80)) & 0xFF;
      state.alu_f = 0;
      if (x & 1)      state.alu_f |= F_CARRY;
      break;
    case 6:
      state.alu_o = ((x << 4) | (x >> 4)) & 0xFF;
      state.alu_f = 0;
      break;
    case 7:
      state.alu_o = (x >> 1) & 0xFF;
      state.alu_f = 0;
      if (x & 1)      state.alu_f |= F_CARRY;
      break;
    }
    if (state.alu_o == 0) state.alu_f |= F_ZERO;
    break;
  case 1:
    state.alu_o = x;
    state.alu_f = (state.f & 0x10) | 0x20;
    if (!bit_mux) state.alu_f |= F_ZERO;
    break;
  case 2:
    state.alu_o = x & (~(1 << row));
    state.alu_f = state.f;
    break;
  case 3:
    state.alu_o = x | (1 << row);
    state.alu_f = state.f;
    break;
  }

  return state.alu_o;
}

//-----------------------------------------------------------------------------
