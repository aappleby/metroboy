#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"
#include "CoreLib/OpDecoder.h"
#include <string.h>

uint8_t  sxt(uint8_t x) { return x & 0x80 ? 0xFF : 0x00; }

constexpr uint8_t F_CARRY = 0x10;
constexpr uint8_t F_HALF_CARRY = 0x20;
constexpr uint8_t F_NEGATIVE = 0x40;
constexpr uint8_t F_ZERO = 0x80;

//-----------------------------------------------------------------------------

FieldInfo MetroBoyCPU::fields[] = {
  DECLARE_FIELD(MetroBoyCPU, reg.bus_addr),
  DECLARE_FIELD(MetroBoyCPU, reg.bus_data),
  DECLARE_FIELD(MetroBoyCPU, reg.bus_read),
  DECLARE_FIELD(MetroBoyCPU, reg.bus_write),

  DECLARE_FIELD(MetroBoyCPU, reg.op_addr),
  DECLARE_FIELD(MetroBoyCPU, reg.op_prev),
  DECLARE_FIELD(MetroBoyCPU, reg.op_next),
  DECLARE_FIELD(MetroBoyCPU, reg.op_cb),
  DECLARE_FIELD(MetroBoyCPU, reg.op_state),
  DECLARE_FIELD(MetroBoyCPU, reg.data_in),

  DECLARE_FIELD(MetroBoyCPU, reg.ime),
  DECLARE_FIELD(MetroBoyCPU, reg.ime_delay),
  DECLARE_FIELD(MetroBoyCPU, reg.int_addr),
  DECLARE_FIELD(MetroBoyCPU, reg.int_ack),

  DECLARE_FIELD(MetroBoyCPU, reg.alu_f),
  DECLARE_FIELD(MetroBoyCPU, reg.alu_o),

  DECLARE_FIELD(MetroBoyCPU, reg.pcl),
  DECLARE_FIELD(MetroBoyCPU, reg.pch),
  DECLARE_FIELD(MetroBoyCPU, reg.spl),
  DECLARE_FIELD(MetroBoyCPU, reg.sph),
  DECLARE_FIELD(MetroBoyCPU, reg.xyl),
  DECLARE_FIELD(MetroBoyCPU, reg.xyh),

  DECLARE_FIELD(MetroBoyCPU, reg.c),
  DECLARE_FIELD(MetroBoyCPU, reg.b),
  DECLARE_FIELD(MetroBoyCPU, reg.e),
  DECLARE_FIELD(MetroBoyCPU, reg.d),
  DECLARE_FIELD(MetroBoyCPU, reg.l),
  DECLARE_FIELD(MetroBoyCPU, reg.h), 
  DECLARE_FIELD(MetroBoyCPU, reg.f),
  DECLARE_FIELD(MetroBoyCPU, reg.a),
  END_FIELDS()
};

//-----------------------------------------------------------------------------

/*
void MetroBoyCPU::reset_to_poweron?() {
  memset(this, 0, sizeof(*this));
  reg.op_next = bootrom[0];
}
*/

//-----------------------------------------------------------------------------

void MetroBoyCPU::reset() {
  reg.bus_addr  = 0xff50;
  reg.bus_data  = 1;
  reg.bus_read  = 0;
  reg.bus_write = 1;

  reg.op_addr = 0x00FE;
  reg.op_next = 0xe0;
  reg.op_cb = 0x11;
  reg.op_state = 2;
  reg.data_in = 0x50;

  reg.ime = false;
  reg.ime_delay = false;
  reg.int_ack = 0x00;

  reg.alu_f = 0xb0;
  reg.alu_o = 0x00;

  reg.pc = 0x0100;
  reg.sp = 0xFFFE;
  reg.xy = 0xFF50;
  reg.bc = 0x0013;
  reg.de = 0x00D8;
  reg.hl = 0x014D;
  reg.af = 0x01B0;
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::dump(Dumper& d_) const {
  d_("bus req : %04x:%02x %s%s\n", reg.bus_addr, reg.bus_data, reg.bus_read  ? "\003R\001" : "-", reg.bus_write ? "\002W\001" : "-");
  d_("op addr : 0x%04x\n", reg.op_addr);
  d_("opname  : '%s' @ %d\n", op_strings2[reg.op_next], reg.op_state);
  d_("opcode  : 0x%02x\n", reg.op_next);
  d_("CB      : 0x%02x\n", reg.op_cb);
  d_("in      : 0x%02x\n", reg.data_in);
  d_("\n");
  d_("IME     : %d\n", reg.ime);
  d_("IME_    : %d\n", reg.ime_delay);
  d_("isr     : %d\n", (reg.op_next == 0xF4));
  d_("int_ack : 0x%02x\n", reg.int_ack);
  d_("\n");
  d_("state.alu_f   : 0x%02x\n", reg.alu_f);
  d_("state.alu_o   : 0x%02x\n", reg.alu_o);
  d_("\n");
  d_("PC      : 0x%04x 0x%02x 0x%02x\n", reg.pc, reg.pcl, reg.pch);
  d_("SP      : 0x%04x 0x%02x 0x%02x\n", reg.sp, reg.sph, reg.spl);
  d_("XY      : 0x%04x 0x%02x 0x%02x\n", reg.xy, reg.xyh, reg.xyl);
  d_("BC      : 0x%04x 0x%02x 0x%02x\n", reg.bc, reg.b,   reg.c);
  d_("DE      : 0x%04x 0x%02x 0x%02x\n", reg.de, reg.d,   reg.e);
  d_("HL      : 0x%04x 0x%02x 0x%02x\n", reg.hl, reg.h,   reg.l);
  d_("AF      : 0x%04x 0x%02x 0x%02x\n", reg.af, reg.a,   reg.f);
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_int(uint8_t imask_, uint8_t intf_) {
  const auto reg_old = this->reg;
  auto& reg_new = this->reg;

  if (reg_old.op_state == 2) {
    if      (imask_ & intf_ & INT_VBLANK_MASK) { reg_new.int_addr = 0x40; }
    else if (imask_ & intf_ & INT_STAT_MASK)   { reg_new.int_addr = 0x48; }
    else if (imask_ & intf_ & INT_TIMER_MASK)  { reg_new.int_addr = 0x50; }
    else if (imask_ & intf_ & INT_SERIAL_MASK) { reg_new.int_addr = 0x58; }
    else if (imask_ & intf_ & INT_JOYPAD_MASK) { reg_new.int_addr = 0x60; }
    else                                       { reg_new.int_addr = 0x00; }

    if      (imask_ & intf_ & INT_VBLANK_MASK) { reg_new.int_ack = INT_VBLANK_MASK; }
    else if (imask_ & intf_ & INT_STAT_MASK)   { reg_new.int_ack = INT_STAT_MASK; }
    else if (imask_ & intf_ & INT_TIMER_MASK)  { reg_new.int_ack = INT_TIMER_MASK; }
    else if (imask_ & intf_ & INT_SERIAL_MASK) { reg_new.int_ack = INT_SERIAL_MASK; }
    else if (imask_ & intf_ & INT_JOYPAD_MASK) { reg_new.int_ack = INT_JOYPAD_MASK; }
    else                                       { reg_new.int_ack = 0; }
  }

  // we don't have to reset pc from state._bus_addr...
  if      (reg_old.op_state == 0) { reg_new.pc = reg_old.bus_addr;     reg_new.cpu_bus_pass(reg_new.sp);               reg_new.op_state = 1; }
  else if (reg_old.op_state == 1) { reg_new.sp = reg_old.bus_addr - 1; reg_new.cpu_bus_write(reg_new.sp, reg_new.pch); reg_new.op_state = 2; }
  else if (reg_old.op_state == 2) { reg_new.sp = reg_old.bus_addr - 1; reg_new.cpu_bus_write(reg_new.sp, reg_new.pcl); reg_new.op_state = 3; }
  else if (reg_old.op_state == 3) {                                    reg_new.cpu_bus_pass(reg_new.pc);               reg_new.op_state = 4; }
  else if (reg_old.op_state == 4) {                                    reg_new.cpu_bus_done(reg_new.int_addr);         reg_new.op_state = 0; }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_halt(uint8_t imask_, uint8_t intf_) {
  const auto reg_old = reg;
  auto& reg_new = reg;

  if (reg_old.op_state == 0) {
    reg_new.pc        = reg_old.bus_addr + 1;
    reg_new.bus_addr  = reg_old.bus_addr + 1;
    reg_new.bus_data  = 0;
    reg_new.bus_read  = 1;
    reg_new.bus_write = 0;
    reg_new.op_state = !(imask_ & intf_);
  }
  else if (reg_old.op_state == 1) {
    reg_new.pc        = reg_old.bus_addr;
    reg_new.bus_addr  = reg_old.bus_addr;
    reg_new.bus_data  = 0;
    reg_new.bus_read  = 1;
    reg_new.bus_write = 0;
    reg_new.op_state = 1;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_cb() {
  const auto reg_old = reg;
  auto& reg_new = this->reg;

  uint16_t ad = reg_new.bus_addr;
  uint16_t adp = ad + 1;

  if (reg_old.op_state == 0) {
    reg_new.pc = adp;
    reg_new.cpu_bus_read(adp);
    reg_new.op_state = 1;
  }
  else {
    if (reg_old.op_state == 1) reg_new.op_cb = reg_old.data_in;

    int cb_col = (reg_new.op_cb >> 0) & 7;
    int cb_quad = (reg_new.op_cb >> 6) & 3;
    uint8_t mask = cb_flag_mask[cb_quad];

    if (cb_col != 6) {
      if (reg_old.op_state == 1)   {
        reg_new.pc = adp;
        set_reg(cb_col, alu_cb(get_reg(cb_col), reg_new.op_cb, reg_new.f));
        set_f(mask);
        reg_new.cpu_bus_done(reg_new.pc);
        reg_new.op_state = 0;
      }
    }
    else {
      if (cb_quad == 1) {
        if (reg_old.op_state == 1) { reg_new.pc = adp;                                               reg_new.cpu_bus_read(reg_new.hl);                 reg_new.op_state = 2;}
        if (reg_old.op_state == 2) { alu_cb(reg_new.data_in, reg_new.op_cb, reg_new.f); set_f(mask); reg_new.cpu_bus_done(reg_new.pc);                 reg_new.op_state = 0;}
      }
      else {
        if (reg_old.op_state == 1) { reg_new.pc = adp;                                               reg_new.cpu_bus_read(reg_new.hl);                 reg_new.op_state = 2;}
        if (reg_old.op_state == 2) { alu_cb(reg_new.data_in, reg_new.op_cb, reg_new.f); set_f(mask); reg_new.cpu_bus_write(reg_new.hl, reg_new.alu_o); reg_new.op_state = 3;}
        if (reg_old.op_state == 3) {                                                                 reg_new.cpu_bus_done(reg_new.pc);                 reg_new.op_state = 0;}
      }
    }
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::execute_op() {
  auto op_state = reg.op_state;
  auto op = reg.op_next;

  uint16_t ad = reg.bus_addr;
  uint16_t adp = ad + 1;
  uint16_t adm = ad - 1;

  bool branch = false;
  switch (OP_ROW & 3) {
  case 0: branch = !(reg.f & F_ZERO); break;
  case 1: branch =  (reg.f & F_ZERO); break;
  case 2: branch = !(reg.f & F_CARRY); break;
  case 3: branch =  (reg.f & F_CARRY); break;
  }

  // IN OUT BC DE HL AF XY | PC SP AD

  // misc

  if (op_state == 0 && NOP)                    /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STOP)                   /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && DI)                     /**/ { reg.pc = adp; /**/ reg.ime = false; reg.ime_delay = false;                            /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && EI)                     /**/ { reg.pc = adp; /**/ reg.ime = reg.ime_delay; reg.ime_delay = true;                     /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && MV_R_R)                 /**/ { reg.pc = adp; /**/ set_reg(OP_ROW, get_reg(OP_COL));                                  /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && LD_SP_HL)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.hl);             reg.op_state = 1; }
  if (op_state == 1 && LD_SP_HL)               /**/ { reg.sp = ad;  /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }

  // load immediate

  if (op_state == 0 && LD_R_D8)                /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && LD_R_D8)                /**/ { reg.pc = adp; /**/ set_reg(OP_ROW, reg.data_in);                                      /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && LD_BC_D16)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 0 && LD_DE_D16)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 0 && LD_HL_D16)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 0 && LD_SP_D16)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && LD_BC_D16)              /**/ { reg.pc = adp; /**/ reg.c   = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 1 && LD_DE_D16)              /**/ { reg.pc = adp; /**/ reg.e   = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 1 && LD_HL_D16)              /**/ { reg.pc = adp; /**/ reg.l   = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 1 && LD_SP_D16)              /**/ { reg.pc = adp; /**/ reg.spl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && LD_BC_D16)              /**/ { reg.pc = adp; /**/ reg.b   = reg.data_in;                                             /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 2 && LD_DE_D16)              /**/ { reg.pc = adp; /**/ reg.d   = reg.data_in;                                             /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 2 && LD_HL_D16)              /**/ { reg.pc = adp; /**/ reg.h   = reg.data_in;                                             /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 2 && LD_SP_D16)              /**/ { reg.pc = adp; /**/ reg.sph = reg.data_in;                                             /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }

  // 8-bit alu

  if (op_state == 0 && ALU_A_R)                /**/ { reg.pc = adp; /**/ reg.a = alu(reg.a, get_reg(OP_COL), OP_ROW, reg.f);   set_f(0xF0); /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && INC_R)                  /**/ { reg.pc = adp; /**/ set_reg(OP_ROW, alu(get_reg(OP_ROW), 1, 1, 0)); set_f(0xE0);       /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && DEC_R)                  /**/ { reg.pc = adp; /**/ set_reg(OP_ROW, alu(get_reg(OP_ROW), 1, 3, 0)); set_f(0xE0);       /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && RLC_A)                  /**/ { reg.pc = adp; /**/ reg.a = rlu(reg.a, OP_ROW, reg.f); set_f(0xF0);                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && RRC_A)                  /**/ { reg.pc = adp; /**/ reg.a = rlu(reg.a, OP_ROW, reg.f); set_f(0xF0);                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && RL_A)                   /**/ { reg.pc = adp; /**/ reg.a = rlu(reg.a, OP_ROW, reg.f); set_f(0xF0);                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && RR_A)                   /**/ { reg.pc = adp; /**/ reg.a = rlu(reg.a, OP_ROW, reg.f); set_f(0xF0);                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && DAA)                    /**/ { reg.pc = adp; /**/ reg.a = rlu(reg.a, OP_ROW, reg.f); set_f(0xB0);                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && CPL)                    /**/ { reg.pc = adp; /**/ reg.a = rlu(reg.a, OP_ROW, reg.f); set_f(0x60);                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && SCF)                    /**/ { reg.pc = adp; /**/ reg.a = rlu(reg.a, OP_ROW, reg.f); set_f(0x70);                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && CCF)                    /**/ { reg.pc = adp; /**/ reg.a = rlu(reg.a, OP_ROW, reg.f); set_f(0x70);                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0;}
  if (op_state == 0 && ALU_A_D8)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && ALU_A_D8)               /**/ { reg.pc = adp; /**/ reg.a = alu(reg.a, reg.data_in, OP_ROW, reg.f); set_f(0xF0);       /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && ALU_A_HL)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.hl);             reg.op_state = 1; }
  if (op_state == 1 && ALU_A_HL)               /**/ {               /**/ reg.a = alu(reg.a, reg.data_in, OP_ROW, reg.f); set_f(0xF0);       /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && INC_AT_HL)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.hl);             reg.op_state = 1; }
  if (op_state == 1 && INC_AT_HL)              /**/ {               /**/ alu(reg.data_in, 1, 1, 0); set_f(0xE0);                            /**/ reg.cpu_bus_write(reg.hl, reg.alu_o); reg.op_state = 2; }
  if (op_state == 2 && INC_AT_HL)              /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && DEC_AT_HL)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.hl);             reg.op_state = 1; }
  if (op_state == 1 && DEC_AT_HL)              /**/ {               /**/ alu(reg.data_in, 1, 3, 0); set_f(0xE0);                            /**/ reg.cpu_bus_write(reg.hl, reg.alu_o); reg.op_state = 2; }
  if (op_state == 2 && DEC_AT_HL)              /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }

  // 16-bit alu

  if (op_state == 0 && ADD_SP_R8)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && ADD_SP_R8)              /**/ { reg.pc = adp; /**/ reg.spl = alu(reg.data_in, reg.spl, 0, reg.f); set_f(0xF0);        /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && ADD_SP_R8)              /**/ {               /**/ reg.sph = alu(sxt(reg.data_in), reg.sph, 1, reg.f);                /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 3; }
  if (op_state == 3 && ADD_SP_R8)              /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && LD_HL_SP_R8)            /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && LD_HL_SP_R8)            /**/ { reg.pc = adp; /**/ reg.l = alu(reg.data_in, reg.spl, 0, reg.f); set_f(0xF0);          /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && LD_HL_SP_R8)            /**/ {               /**/ reg.h = alu(sxt(reg.data_in), reg.sph, 1, reg.f);                  /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }

  if (op_state == 0 && INC_BC)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.bc);             reg.op_state = 1; }
  if (op_state == 1 && INC_BC)                 /**/ { reg.bc = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && DEC_BC)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.bc);             reg.op_state = 1; }
  if (op_state == 1 && DEC_BC)                 /**/ { reg.bc = adm; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && INC_DE)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.de);             reg.op_state = 1; }
  if (op_state == 1 && INC_DE)                 /**/ { reg.de = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && DEC_DE)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.de);             reg.op_state = 1; }
  if (op_state == 1 && DEC_DE)                 /**/ { reg.de = adm; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && INC_HL)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.hl);             reg.op_state = 1; }
  if (op_state == 1 && INC_HL)                 /**/ { reg.hl = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && DEC_HL)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.hl);             reg.op_state = 1; }
  if (op_state == 1 && DEC_HL)                 /**/ { reg.hl = adm; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && INC_SP)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.sp);             reg.op_state = 1; }
  if (op_state == 1 && INC_SP)                 /**/ { reg.sp = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && DEC_SP)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.sp);             reg.op_state = 1; }
  if (op_state == 1 && DEC_SP)                 /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }

  if (op_state == 0 && ADD_HL_BC)              /**/ { reg.pc = adp; /**/ reg.l = alu(reg.c, reg.l, 0, reg.f);                               /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && ADD_HL_BC)              /**/ {               /**/ reg.h = alu(reg.b, reg.h, 1, reg.alu_f); set_f(0x70);              /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && ADD_HL_DE)              /**/ { reg.pc = adp; /**/ reg.l = alu(reg.e, reg.l, 0, reg.f);                               /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && ADD_HL_DE)              /**/ {               /**/ reg.h = alu(reg.d, reg.h, 1, reg.alu_f); set_f(0x70);              /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && ADD_HL_HL)              /**/ { reg.pc = adp; /**/ reg.l = alu(reg.l, reg.l, 0, reg.f);                               /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && ADD_HL_HL)              /**/ {               /**/ reg.h = alu(reg.h, reg.h, 1, reg.alu_f); set_f(0x70);              /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && ADD_HL_SP)              /**/ { reg.pc = adp; /**/ reg.l = alu(reg.spl, reg.l, 0, reg.f);                             /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && ADD_HL_SP)              /**/ {               /**/ reg.h = alu(reg.sph, reg.h, 1, reg.alu_f); set_f(0x70);            /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }

  // load/store

  if (op_state == 0 && STM_A16_SP)             /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && STM_A16_SP)             /**/ { reg.pc = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && STM_A16_SP)             /**/ { reg.pc = adp; /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_write(reg.xy, reg.spl);   reg.op_state = 3; }
  if (op_state == 3 && STM_A16_SP)             /**/ { reg.xy = adp; /**/                                                                    /**/ reg.cpu_bus_write(reg.xy, reg.sph);   reg.op_state = 4; }
  if (op_state == 4 && STM_A16_SP)             /**/ { reg.xy = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STM_A16_A)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && STM_A16_A)              /**/ { reg.pc = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && STM_A16_A)              /**/ { reg.pc = adp; /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_write(reg.xy, reg.a);     reg.op_state = 3; }
  if (op_state == 3 && STM_A16_A)              /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && LDM_A_A16)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && LDM_A_A16)              /**/ { reg.pc = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && LDM_A_A16)              /**/ { reg.pc = adp; /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.xy);             reg.op_state = 3; }
  if (op_state == 3 && LDM_A_A16)              /**/ {               /**/ reg.a = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
                                                                                                                                                                                           
  // indirect load/store                                                                                                                                                                   
                                                                                                                                                                                           
  if (op_state == 0 && LDM_A_BC)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.bc);             reg.op_state = 1; }
  if (op_state == 1 && LDM_A_BC)               /**/ {               /**/ reg.a = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && LDM_A_DE)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.de);             reg.op_state = 1; }
  if (op_state == 1 && LDM_A_DE)               /**/ {               /**/ reg.a = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && LDM_R_HL)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.hl);             reg.op_state = 1; }
  if (op_state == 1 && LDM_R_HL)               /**/ {               /**/ set_reg(OP_ROW, reg.data_in);                                      /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && LDM_A_HLP)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.hl);             reg.op_state = 1; }
  if (op_state == 1 && LDM_A_HLP)              /**/ { reg.hl = adp; /**/ reg.a = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && LDM_A_HLM)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.hl);             reg.op_state = 1; }
  if (op_state == 1 && LDM_A_HLM)              /**/ { reg.hl = adm; /**/ reg.a = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STM_BC_A)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_write(reg.bc, reg.a);     reg.op_state = 1; }
  if (op_state == 1 && STM_BC_A)               /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STM_DE_A)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_write(reg.de, reg.a);     reg.op_state = 1; }
  if (op_state == 1 && STM_DE_A)               /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STM_HL_R)               /**/ { reg.pc = adp; /**/ uint8_t t = get_reg(OP_COL);                                       /**/ reg.cpu_bus_write(reg.hl, t);         reg.op_state = 1; }
  if (op_state == 1 && STM_HL_R)               /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STM_HL_D8)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && STM_HL_D8)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_write(reg.hl, reg.data_in);    reg.op_state = 2; }
  if (op_state == 2 && STM_HL_D8)              /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STM_HLP_A)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_write(reg.hl, reg.a);     reg.op_state = 1; }
  if (op_state == 1 && STM_HLP_A)              /**/ { reg.hl = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STM_HLM_A)              /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_write(reg.hl, reg.a);     reg.op_state = 1; }
  if (op_state == 1 && STM_HLM_A)              /**/ { reg.hl = adm; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
                                                                                                                                                                                           
  // zero-page load/store                                                                                                                                                                  
                                                                                                                                                                                           
  if (op_state == 0 && LDM_A_C)                /**/ { reg.pc = adp; /**/ reg.xy = 0xFF00 | reg.c;                                           /**/ reg.cpu_bus_read(reg.xy);             reg.op_state = 1; }
  if (op_state == 1 && LDM_A_C)                /**/ {               /**/ reg.a = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && LDM_A_A8)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && LDM_A_A8)               /**/ { reg.pc = adp; /**/ reg.xy = 0xFF00 | reg.data_in;                                     /**/ reg.cpu_bus_read(reg.xy);             reg.op_state = 2; }
  if (op_state == 2 && LDM_A_A8)               /**/ {               /**/ reg.a = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STM_C_A)                /**/ { reg.pc = adp; /**/ reg.xy = 0xFF00 | reg.c;                                           /**/ reg.cpu_bus_write(reg.xy, reg.a);     reg.op_state = 1; }
  if (op_state == 1 && STM_C_A)                /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && STM_A8_A)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && STM_A8_A)               /**/ { reg.pc = adp; /**/ reg.xy = 0xFF00 | reg.data_in;                                     /**/ reg.cpu_bus_write(reg.xy, reg.a);     reg.op_state = 2; }
  if (op_state == 2 && STM_A8_A)               /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
                                                                                                                                                                                           
  // push / pop                                                                                                                                                                            
                                                                                                                                                                                           
  if (op_state == 0 && PUSH_BC)                /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.sp);             reg.op_state = 1; }
  if (op_state == 0 && PUSH_DE)                /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.sp);             reg.op_state = 1; }
  if (op_state == 0 && PUSH_HL)                /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.sp);             reg.op_state = 1; }
  if (op_state == 0 && PUSH_AF)                /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.sp);             reg.op_state = 1; }
  if (op_state == 1 && PUSH_BC)                /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.b);     reg.op_state = 2; }
  if (op_state == 1 && PUSH_DE)                /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.d);     reg.op_state = 2; }
  if (op_state == 1 && PUSH_HL)                /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.h);     reg.op_state = 2; }
  if (op_state == 1 && PUSH_AF)                /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.a);     reg.op_state = 2; }
  if (op_state == 2 && PUSH_BC)                /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.c);     reg.op_state = 3; }
  if (op_state == 2 && PUSH_DE)                /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.e);     reg.op_state = 3; }
  if (op_state == 2 && PUSH_HL)                /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.l);     reg.op_state = 3; }
  if (op_state == 2 && PUSH_AF)                /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.f);     reg.op_state = 3; }
  if (op_state == 3 && PUSH_BC)                /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 3 && PUSH_DE)                /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 3 && PUSH_HL)                /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 3 && PUSH_AF)                /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
                                                                                                                                                                                       
  if (op_state == 0 && POP_BC)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 1; }
  if (op_state == 0 && POP_DE)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 1; }
  if (op_state == 0 && POP_HL)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 1; }
  if (op_state == 0 && POP_AF)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 1; }
  if (op_state == 1 && POP_BC)                 /**/ { reg.sp = adp; /**/ reg.c = reg.data_in;                                               /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 2; }
  if (op_state == 1 && POP_DE)                 /**/ { reg.sp = adp; /**/ reg.e = reg.data_in;                                               /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 2; }
  if (op_state == 1 && POP_HL)                 /**/ { reg.sp = adp; /**/ reg.l = reg.data_in;                                               /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 2; }
  if (op_state == 1 && POP_AF)                 /**/ { reg.sp = adp; /**/ reg.f = reg.data_in;                                               /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 2; }
  if (op_state == 2 && POP_BC)                 /**/ { reg.sp = adp; /**/ reg.b = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 2 && POP_DE)                 /**/ { reg.sp = adp; /**/ reg.d = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 2 && POP_HL)                 /**/ { reg.sp = adp; /**/ reg.h = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 2 && POP_AF)                 /**/ { reg.sp = adp; /**/ reg.a = reg.data_in;                                               /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
                                                                                                                                                                                           
  // conditional branches                                                                                                                                                                  
                                                                                                                                                                                           
  if (op_state == 0 && JR_R8)                  /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && JR_R8)                  /**/ { reg.pc = adp; /**/ reg.pc += int8_t(reg.data_in);                                     /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && JR_R8)                  /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && JR_CC_R8  &&  branch)   /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && JR_CC_R8  &&  branch)   /**/ { reg.pc = adp; /**/ reg.pc += int8_t(reg.data_in);                                     /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && JR_CC_R8  &&  branch)   /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && JR_CC_R8  && !branch)   /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && JR_CC_R8  && !branch)   /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && JP_CC_A16 &&  branch)   /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && JP_CC_A16 &&  branch)   /**/ { reg.pc = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && JP_CC_A16 &&  branch)   /**/ {               /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 3; }
  if (op_state == 3 && JP_CC_A16 &&  branch)   /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.xy);             reg.op_state = 0; }
  if (op_state == 0 && JP_CC_A16 && !branch)   /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && JP_CC_A16 && !branch)   /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && JP_CC_A16 && !branch)   /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && JP_A16)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && JP_A16)                 /**/ { reg.pc = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && JP_A16)                 /**/ {               /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 3; }
  if (op_state == 3 && JP_A16)                 /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.xy);             reg.op_state = 0; }
  if (op_state == 0 && JP_HL)                  /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.hl);             reg.op_state = 0; }
                                                                                                                                                                                           
  // calls                                                                                                                                                                                 
                                                                                                                                                                                           
  if (op_state == 0 && CALL_CC_A16 &&  branch) /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && CALL_CC_A16 &&  branch) /**/ { reg.pc = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && CALL_CC_A16 &&  branch) /**/ { reg.pc = adp; /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_pass(reg.sp);             reg.op_state = 3; }
  if (op_state == 3 && CALL_CC_A16 &&  branch) /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.pch);   reg.op_state = 4; }
  if (op_state == 4 && CALL_CC_A16 &&  branch) /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.pcl);   reg.op_state = 5; }
  if (op_state == 5 && CALL_CC_A16 &&  branch) /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.xy);             reg.op_state = 0; }
  if (op_state == 0 && CALL_CC_A16 && !branch) /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && CALL_CC_A16 && !branch) /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && CALL_CC_A16 && !branch) /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && CALL_A16)               /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && CALL_A16)               /**/ { reg.pc = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.pc);             reg.op_state = 2; }
  if (op_state == 2 && CALL_A16)               /**/ { reg.pc = adp; /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_pass(reg.sp);             reg.op_state = 3; }
  if (op_state == 3 && CALL_A16)               /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.pch);   reg.op_state = 4; }
  if (op_state == 4 && CALL_A16)               /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.pcl);   reg.op_state = 5; }
  if (op_state == 5 && CALL_A16)               /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.xy);             reg.op_state = 0; }
  if (op_state == 0 && RST_NN)                 /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.sp);             reg.op_state = 1; }
  if (op_state == 1 && RST_NN)                 /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.pch);   reg.op_state = 2; }
  if (op_state == 2 && RST_NN)                 /**/ { reg.sp = adm; /**/                                                                    /**/ reg.cpu_bus_write(reg.sp, reg.pcl);   reg.op_state = 3; }
  if (op_state == 3 && RST_NN)                 /**/ {               /**/ reg.xy = reg.op_next - 0xC7;                                       /**/ reg.cpu_bus_done(reg.xy);             reg.op_state = 0; }
                                                                                                                                                                                           
  // returns                                                                                                                                                                               
                                                                                                                                                                                           
  if (op_state == 0 && RET_CC && !branch)      /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && RET_CC && !branch)      /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.pc);             reg.op_state = 0; }
  if (op_state == 0 && RET_CC &&  branch)      /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 1; }
  if (op_state == 1 && RET_CC &&  branch)      /**/ {               /**/                                                                    /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 2; }
  if (op_state == 2 && RET_CC &&  branch)      /**/ { reg.sp = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 3; }
  if (op_state == 3 && RET_CC &&  branch)      /**/ { reg.sp = adp; /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 4; }
  if (op_state == 4 && RET_CC &&  branch)      /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.xy);             reg.op_state = 0; }
  if (op_state == 0 && RETI)                   /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 1; }
  if (op_state == 1 && RETI)                   /**/ { reg.sp = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 2; }
  if (op_state == 2 && RETI)                   /**/ { reg.sp = adp; /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 3; }
  if (op_state == 3 && RETI)                   /**/ {               /**/ reg.ime = true; reg.ime_delay = true;                              /**/ reg.cpu_bus_done(reg.xy);             reg.op_state = 0; }
  if (op_state == 0 && RET)                    /**/ { reg.pc = adp; /**/                                                                    /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 1; }
  if (op_state == 1 && RET)                    /**/ { reg.sp = adp; /**/ reg.xyl = reg.data_in;                                             /**/ reg.cpu_bus_read(reg.sp);             reg.op_state = 2; }
  if (op_state == 2 && RET)                    /**/ { reg.sp = adp; /**/ reg.xyh = reg.data_in;                                             /**/ reg.cpu_bus_pass(reg.pc);             reg.op_state = 3; }
  if (op_state == 3 && RET)                    /**/ {               /**/                                                                    /**/ reg.cpu_bus_done(reg.xy);             reg.op_state = 0; }

  reg.f &= 0xF0;
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::get_reg(int mux) {
  switch(mux) {
  case 0: return reg.b;
  case 1: return reg.c;
  case 2: return reg.d;
  case 3: return reg.e;
  case 4: return reg.h;
  case 5: return reg.l;
  case 6: return 0;
  case 7: return reg.a;
  }
  return 0;
}

void MetroBoyCPU::set_reg(int mux, uint8_t data) {
  switch(mux) {
  case 0: reg.b = data; break;
  case 1: reg.c = data; break;
  case 2: reg.d = data; break;
  case 3: reg.e = data; break;
  case 4: reg.h = data; break;
  case 5: reg.l = data; break;
  case 6: break;
  case 7: reg.a = data; break;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCPU::set_f(uint8_t mask) {
  auto op = reg.op_next;

  reg.f = (reg.f & ~mask) | (reg.alu_f & mask);
  if (ADD_SP_R8)   { reg.f &= ~(F_ZERO | F_NEGATIVE); }
  if (LD_HL_SP_R8) { reg.f &= ~(F_ZERO | F_NEGATIVE); }
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::alu(uint8_t x, uint8_t y, int op, uint8_t flags) {
  reg.alu_f = flags;

  uint16_t d1 = 0, d2 = 0;
  const bool c = reg.alu_f & F_CARRY;

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

  reg.alu_o = (uint8_t)d2;
  reg.alu_f = (op == 2 || op == 3 || op == 7) ? F_NEGATIVE : 0;

  if (d1 & 0x010) reg.alu_f |= F_HALF_CARRY;
  if (d2 & 0x100) reg.alu_f |= F_CARRY;
  if (reg.alu_o == 0x000) reg.alu_f |= F_ZERO;
  if (op == 7)  reg.alu_o = x;

  return reg.alu_o;
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::rlu(uint8_t x, int op, uint8_t flags) {
  reg.alu_f = flags;
  const uint8_t f = reg.alu_f;

  switch (op) {
  case 0:
    reg.alu_o = (x << 1) | (x >> 7);
    reg.alu_f = (x >> 7) ? F_CARRY : 0;
    reg.alu_f &= ~F_ZERO;
    break;
  case 1:
    reg.alu_o = (x >> 1) | (x << 7);
    reg.alu_f = (x & 1) ? F_CARRY : 0;
    reg.alu_f &= ~F_ZERO;
    break;
  case 2:
    reg.alu_o = (x << 1) | (f & F_CARRY ? 1 : 0);
    reg.alu_f = (x >> 7) ? F_CARRY : 0;
    reg.alu_f &= ~F_ZERO;
    break;
  case 3:
    reg.alu_o = (x >> 1) | ((f & F_CARRY ? 1 : 0) << 7);
    reg.alu_f = (x & 1) ? F_CARRY : 0;
    reg.alu_f &= ~F_ZERO;
    break;
  case 4:
    reg.alu_o = daa(x, f);
    if (!reg.alu_o) reg.alu_f |= F_ZERO;
    break;
  case 5:
    reg.alu_o = ~x;
    reg.alu_f = f | 0x60;
    if (!reg.alu_o) reg.alu_f |= F_ZERO;
    break;
  case 6:
    reg.alu_o = x;
    reg.alu_f = (f & 0x80) | 0x10;
    if (!reg.alu_o) reg.alu_f |= F_ZERO;
    break;
  case 7:
    reg.alu_o = x;
    reg.alu_f = (f & 0x90) ^ 0x10;
    if (!reg.alu_o) reg.alu_f |= F_ZERO;
    break;
  }

  return reg.alu_o;
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
  reg.alu_o = uint8_t((hi << 4) | (lo & 0xF));
  reg.alu_f = 0;
  if (c) reg.alu_f |= F_CARRY;
  if ((hi >> 4) && !n) reg.alu_f |= F_CARRY;
  if (!reg.alu_o) reg.alu_f |= F_ZERO;
  return reg.alu_o;
}

//-----------------------------------------------------------------------------

uint8_t MetroBoyCPU::alu_cb(uint8_t x, int op, uint8_t flags) {
  reg.alu_f = flags;
  const uint8_t quad = ((op >> 6) & 3);
  const uint8_t row = ((op >> 3) & 7);
  const bool bit_mux = (x >> row) & 1;

  reg.alu_o = 0;

  switch (quad) {
  case 0:
    switch (row) {
    case 0:
      reg.alu_o = (x << 1) | (x >> 7);
      reg.alu_f = (x >> 7) ? F_CARRY : 0;
      break;
    case 1:
      reg.alu_o = (x >> 1) | (x << 7);
      reg.alu_f = (x & 1) ? F_CARRY : 0;
      break;
    case 2:
      reg.alu_o = (x << 1) | (reg.f & F_CARRY ? 1 : 0);
      reg.alu_f = (x >> 7) ? F_CARRY : 0;
      break;
    case 3:
      reg.alu_o = (x >> 1) | ((reg.f & F_CARRY ? 1 : 0) << 7);
      reg.alu_f = (x & 1) ? F_CARRY : 0;
      break;
    case 4:
      reg.alu_o = (x << 1) & 0xFF;
      reg.alu_f = 0;
      if (x >> 7)     reg.alu_f |= F_CARRY;
      break;
    case 5:
      reg.alu_o = ((x >> 1) | (x & 0x80)) & 0xFF;
      reg.alu_f = 0;
      if (x & 1)      reg.alu_f |= F_CARRY;
      break;
    case 6:
      reg.alu_o = ((x << 4) | (x >> 4)) & 0xFF;
      reg.alu_f = 0;
      break;
    case 7:
      reg.alu_o = (x >> 1) & 0xFF;
      reg.alu_f = 0;
      if (x & 1)      reg.alu_f |= F_CARRY;
      break;
    }
    if (reg.alu_o == 0) reg.alu_f |= F_ZERO;
    break;
  case 1:
    reg.alu_o = x;
    reg.alu_f = (reg.f & 0x10) | 0x20;
    if (!bit_mux) reg.alu_f |= F_ZERO;
    break;
  case 2:
    reg.alu_o = x & (~(1 << row));
    reg.alu_f = reg.f;
    break;
  case 3:
    reg.alu_o = x | (1 << row);
    reg.alu_f = reg.f;
    break;
  }

  return reg.alu_o;
}

//-----------------------------------------------------------------------------
