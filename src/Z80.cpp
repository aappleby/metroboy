#include "Z80.h"

#include "Constants.h"

#include <assert.h>

//-----------------------------------------------------------------------------

#define ST_BC_A       (op_ == 0x02)
#define ST_DE_A       (op_ == 0x12)
#define ST_HLP_A      (op_ == 0x22)
#define ST_HLM_A      (op_ == 0x32)
#define ST_RR_A       (quad_ == 0 && col_ == 2 && !odd_row_)

#define ST_A8_A       (op_ == 0xE0)
#define ST_C_A        (op_ == 0xE2)
#define ST_A16_SP     (op_ == 0x08)
#define ST_A16_A      (op_ == 0xEA)
#define ST_HL_D8      (op_ == 0x36)

#define ADD_SP_R8     (op_ == 0xE8)
#define LD_HL_SP_R8   (op_ == 0xF8)
#define INC_AT_HL     (op_ == 0x34)
#define DEC_AT_HL     (op_ == 0x35)
#define LD_A_AT_A16   (op_ == 0xFA)
#define LD_A_AT_C     (op_ == 0xF2)
#define LD_A_AT_A8    (op_ == 0xF0)
#define LD_A_AT_BC    (op_ == 0x0A)
#define LD_A_AT_DE    (op_ == 0x1A)
#define LD_A_AT_HLP   (op_ == 0x2A)
#define LD_A_AT_HLM   (op_ == 0x3A)

#define JP_A16        (op_ == 0xC3)
#define JR_R8         (op_ == 0x18)
#define JP_HL         (op_ == 0xE9)
#define CALL_A16      (op_ == 0xCD)
#define RET           (op_ == 0xC9)
#define RETI          (op_ == 0xD9)
#define POP_AF        (op_ == 0xF1)
#define DI            (op_ == 0xF3)
#define EI            (op_ == 0xFB)
#define HALT          (op_ == 0x76)
#define PREFIX_CB     (op_ == 0xCB)
#define MV_SP_HL      (op_ == 0xF9)

#define MV_OPS        (quad_ == 1)
#define MV_OPS_ST_HL  (quad_ == 1 && row_ == 6)
#define ALU_OPS       (quad_ == 2)
#define ALU_A_D8      (quad_ == 3 && col_ == 6)

#define JR_CC_R8      (quad_ == 0 && col_ == 0 && row_ >= 4)
#define LD_RR_D16     (quad_ == 0 && col_ == 1 && !odd_row_)
#define ADD_HL_RR     (quad_ == 0 && col_ == 1 && odd_row_)
#define LD_A_AT_RR    (quad_ == 0 && col_ == 2 && odd_row_)
#define INC_RR        (quad_ == 0 && col_ == 3 && !odd_row_)
#define DEC_RR        (quad_ == 0 && col_ == 3 && odd_row_)
#define INC_R         (quad_ == 0 && col_ == 4)
#define DEC_R         (quad_ == 0 && col_ == 5)
#define LD_R_D8       (quad_ == 0 && col_ == 6)
#define ROTATE_OPS    (quad_ == 0 && col_ == 7)


#define PUSH_RR       (quad_ == 3 && col_ == 5 && !odd_row_)
#define POP_RR        (quad_ == 3 && col_ == 1 && !odd_row_)
#define RET_CC        (quad_ == 3 && col_ == 0 && row_ <= 3)
#define JP_CC_A16     (quad_ == 3 && col_ == 2 && row_ <= 3)
#define CALL_CC_A16   (quad_ == 3 && col_ == 4 && row_ <= 3)
#define RST_NN        (quad_ == 3 && col_ == 7)

//-----------------------------------------------------------------------------

CpuOut Z80::reset(int new_model, uint16_t new_pc) {
  model = new_model;
  bus_tag = bus_tag_ = TAG_OPCODE;
  mem_addr = mem_addr_ = new_pc;
  mem_read_ = true;
  mem_write_ = false;
  mem_out_ = 0;
  int_ack_ = 0;

  state = state_ = Z80_STATE_DECODE;

  if (new_pc == 0x100) {
    af = 0x01B0;
    bc = 0x0013;
    de = 0x00D8;
    hl = 0x014D;
    sp = 0xFFFE;
    pc = pc_ = new_pc;
  }
  else {
    af = 0x0000;
    bc = 0x0000;
    de = 0x0000;
    hl = 0x0000;
    sp = 0x0000;
    pc = pc_ = new_pc;
  }

  op_ = 0;
  quad_ = 0;
  row_ = 0;
  col_ = 0;
  odd_row_ = false;

  cb_quad_ = 0;
  cb_row_ = 0;
  cb_col_ = 0;

  get_hl_ = false;
  put_hl_ = false;
  pop_d16_ = false;
  push_d16_ = false;
  fetch_d8_ = false;
  fetch_d16_ = false;
  any_read_ = false;
  any_write_ = false;
  take_branch_ = false;
  interrupt2 = false;
  ime = false;
  ime_delay = false;
  alu_out_ = 0;
  opcount = 0;
  cycle = 0;
  unhalt = 0;

  return { 0 };
}

//-----------------------------------------------------------------------------

CpuBus Z80::tick_t0(uint8_t imask, uint8_t intf, uint8_t bus_data) {
  imask_ = imask;
  intf_ = intf;

  pc_ = pc;
  bus_tag_ = TAG_NONE;
  state_ = state;
  int_ack_ = 0;
  data_lo_ = data_lo;
  data_hi_ = data_hi;

  if      (bus_tag == TAG_OPCODE)    op_      = bus_data;
  if      (bus_tag == TAG_OPCODE_CB) op_cb_   = bus_data;
  if      (bus_tag == TAG_DATA0)     data_lo_ = bus_data;
  else if (bus_tag == TAG_DATA1)     data_hi_ = bus_data;
  else if (bus_tag == TAG_ARG0)      data_lo_ = bus_data;
  else if (bus_tag == TAG_ARG1)      data_hi_ = bus_data;

  if (state == Z80_STATE_DECODE) {
    interrupt2 = (imask_ & intf_) && ime;
    if (interrupt2) op_ = 0x00;
  }

  decode();
  cb_quad_ = (op_cb_ >> 6) & 3;
  cb_row_ = (op_cb_ >> 3) & 7;
  cb_col_ = (op_cb_ >> 0) & 7;

  state_ = next_state();
  if (state == Z80_STATE_DECODE && state_ == Z80_STATE_HALT) unhalt = 0;

  //----------------------------------------
  // compute new pc

  int next_int = next_interrupt();
  pc_ = next_pc(next_int);
  if (next_int >= 0) int_ack_ = 1 << next_int;

  //----------------------------------------

  AluOut out = exec(reg_fetch8());
  alu_out_ = out.x;
  f_ = out.f;

  //----------------------------------------

  CpuBus next_bus2 = next_bus();

  bus_tag_ = (MemTag)next_bus2.tag;
  mem_addr_ = next_bus2.addr;
  mem_out_ = next_bus2.data;
  mem_read_ = next_bus2.read;
  mem_write_ = next_bus2.write;

  return next_bus2;
}

//-----------------------------------------------------------------------------

void Z80::tock_t0() {
}

//-----------------------------------------------------------------------------

void Z80::tick_t2() {
}

//-----------------------------------------------------------------------------

/*
static const uint8_t flag_mask[256] = {
0,    0,    0,    0, 0xE0, 0xE0,    0, 0xF0,
0, 0x70,    0,    0, 0xE0, 0xE0,    0, 0xF0,
0,    0,    0,    0, 0xE0, 0xE0,    0, 0xF0,
0, 0x70,    0,    0, 0xE0, 0xE0,    0, 0xF0,
0,    0,    0,    0, 0xE0, 0xE0,    0, 0xB0,
0, 0x70,    0,    0, 0xE0, 0xE0,    0, 0x60,
0,    0,    0,    0, 0xE0, 0xE0,    0, 0x70,
0, 0x70,    0,    0, 0xE0, 0xE0,    0, 0x70,

0,    0,    0,    0,    0,    0,    0,    0,
0,    0,    0,    0,    0,    0,    0,    0,
0,    0,    0,    0,    0,    0,    0,    0,
0,    0,    0,    0,    0,    0,    0,    0,
0,    0,    0,    0,    0,    0,    0,    0,
0,    0,    0,    0,    0,    0,    0,    0,
0,    0,    0,    0,    0,    0,    0,    0,
0,    0,    0,    0,    0,    0,    0,    0,

0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,

   0,    0,    0,    0,    0,    0, 0xF0,    0,
   0,    0,    0,    0,    0,    0, 0xF0,    0,
   0,    0,    0,    0,    0,    0, 0xF0,    0,
   0,    0,    0,    0,    0,    0, 0xF0,    0,
   0,    0,    0,    0,    0,    0, 0xF0,    0,
0xF0,    0,    0,    0,    0,    0, 0xF0,    0,
   0, 0xF0,    0,    0,    0,    0, 0xF0,    0,
0xF0,    0,    0,    0,    0,    0, 0xF0,    0,
};

static const uint8_t cb_flag_mask[4] = { 0xF0, 0xE0,    0, 0x0 };
*/

uint8_t flag_mask2(uint8_t op, uint8_t cb) {
  uint8_t quad = (op >> 6) & 3;
  uint8_t row = (op >> 3) & 7;
  uint8_t col = (op >> 0) & 7;

  if (op == 0xCB) {
    quad = (cb >> 6) & 3;
    if (quad == 0) return 0xF0;
    if (quad == 1) return 0xE0;
    return 0;
  }

  if (quad == 0) {
    if (col == 1) return row & 1 ? 0x70 : 0x00;
    if (col == 4) return 0xE0;
    if (col == 5) return 0xE0;
    if (col == 7) {
      if (row == 0) return 0xF0;
      if (row == 1) return 0xF0;
      if (row == 2) return 0xF0;
      if (row == 3) return 0xF0;
      if (row == 4) return 0xB0;
      if (row == 5) return 0x60;
      if (row == 6) return 0x70;
      if (row == 7) return 0x70;
    }
  }

  if (quad == 1) return 0x00;
  if (quad == 2) return 0xF0;

  if (quad == 3) {
    if (col == 6) return 0xF0;
    if (op == 0xE8) return 0xF0;
    if (op == 0xF1) return 0xF0;
    if (op == 0xF8) return 0xF0;
  }
  return 0;
}


CpuOut Z80::tock_t2() {

  if (state_ == Z80_STATE_DECODE) {

    // Write all our registers from the previous instruction before the new opcode shows up.
    // Not idempotent yet
    pc = pc_;


    uint8_t mask = PREFIX_CB ? cb_flag_mask[cb_quad_] : flag_mask[op_];

    uint8_t mask2 = flag_mask2(op_, op_cb_);
    if (mask != mask2) printf("x");
    
    f = POP_AF ? data_lo_ & 0xF0 : (f & ~mask) | (f_ & mask);

    opcount = opcount + 1;

    if      (MV_OPS)      reg_put8(row_,    reg_fetch8());
    else if (PREFIX_CB)   reg_put8(cb_col_, (uint8_t)alu_out_);
    else if (INC_R)       reg_put8(row_,    (uint8_t)alu_out_);
    else if (DEC_R)       reg_put8(row_,    (uint8_t)alu_out_);
    else if (ALU_A_D8)    reg_put8(7,       (uint8_t)alu_out_);
    else if (ALU_OPS)     reg_put8(7,       (uint8_t)alu_out_);
    else if (ROTATE_OPS)  reg_put8(7,       (uint8_t)alu_out_);
    else if (LD_R_D8)     reg_put8(row_,    data_lo_);
    else if (LD_A_AT_RR)  reg_put8(7,       data_lo_);
    else if (LD_A_AT_A8)  reg_put8(7,       data_lo_);
    else if (LD_A_AT_C)   reg_put8(7,       data_lo_);
    else if (LD_A_AT_A16) reg_put8(7,       data_lo_);


    if      (LD_RR_D16)   reg_put16(row_ >> 1, data16_);
    else if (INC_RR)      reg_put16(row_ >> 1, reg_fetch16() + 1);
    else if (DEC_RR)      reg_put16(row_ >> 1, reg_fetch16() - 1);
    else if (POP_RR)      reg_put16(row_ >> 1, data16_);

    if      (ADD_HL_RR)   hl = alu_out_;
    else if (LD_HL_SP_R8) hl = alu_out_;
    else if (ST_HLP_A)    hl++;
    else if (ST_HLM_A)    hl--;
    else if (LD_A_AT_HLP) hl++;
    else if (LD_A_AT_HLM) hl--;

    if      (ADD_SP_R8)   sp = alu_out_;
    else if (MV_SP_HL)    sp = hl;
    else if (push_d16_)   sp = sp - 2;
    else if (RET)         sp = sp + 2;
    else if (RETI)        sp = sp + 2;
    else if (POP_RR)      sp = sp + 2;
    else if (RET_CC && take_branch_) sp = sp + 2;
  }

  //----------
  // When we finish an instruction, update our interrupt master enable.

  ime = ime_delay;

  if (state_ == Z80_STATE_DECODE) {
    if (interrupt2) {
      ime = false;
      ime_delay = false;
    }
    else if (RETI) {
      ime = true;
      ime_delay = true;
    }
    else if (DI) {
      // on dmg this should disable interrupts immediately?
      ime = false;
      ime_delay = false;
    }
    else if (EI) {
      ime_delay = true;
    }
  }

  //----------
  // Gameboy weirdness - the "real" interrupt vector is determined by the
  // state of imask/intf at the end of the first write cycle.

  if (state_ == Z80_STATE_MEM_WRITE2) {
    imask_latch = imask_;
  }

  state = state_;
  bus_tag = bus_tag_;
  data_lo = data_lo_;
  data_hi = data_hi_;
  mem_addr = mem_addr_;

  cycle++;

  return { 0 };
}

//-----------------------------------------------------------------------------

Z80::Z80State Z80::next_state() const {
  Z80State next = Z80_STATE_DECODE;

  switch (state) {
  case Z80_STATE_DECODE:
    if (any_write_) next = Z80_STATE_MEM_WRITE1;
    if (any_read_)  next = Z80_STATE_MEM_READ1;
    if (RET_CC || RST_NN || PUSH_RR) next = Z80_STATE_DELAY_A;
    if (INC_RR || DEC_RR || ADD_HL_RR || MV_SP_HL) next = Z80_STATE_DELAY_C;
    if (PREFIX_CB) next = Z80_STATE_DECODE_CB;
    if (HALT) {
      next = ((imask_ & intf_) && !ime) ? Z80_STATE_DECODE : Z80_STATE_HALT;
    }
    break;

  case Z80_STATE_DECODE_CB:
    next = (cb_col_ == 6) ? Z80_STATE_MEM_READ_CB : Z80_STATE_DECODE;
    break;

  case Z80_STATE_HALT:
    next = unhalt ? Z80_STATE_DECODE : Z80_STATE_HALT;
    break;

  case Z80_STATE_MEM_READ1:
    if (JR_R8 || (JR_CC_R8 && take_branch_) || LD_HL_SP_R8) next = Z80_STATE_DELAY_C;
    if (ADD_SP_R8) next = Z80_STATE_DELAY_B;
    if (any_write_) next = Z80_STATE_MEM_WRITE1;
    if (LD_A_AT_A8 || fetch_d16_ || pop_d16_) next = Z80_STATE_MEM_READ2;
    break;

  case Z80_STATE_MEM_READ2:
    if (RET_CC || RET || RETI || JP_A16 || (JP_CC_A16 && take_branch_)) next = Z80_STATE_DELAY_C;
    if (any_write_) next = Z80_STATE_MEM_WRITE1;
    if ((CALL_CC_A16 && take_branch_) || CALL_A16) next = Z80_STATE_DELAY_B;
    if (LD_A_AT_A16) next = Z80_STATE_MEM_READ3;
    break;

  case Z80_STATE_MEM_READ3:
    next = Z80_STATE_DECODE;
    break;

  case Z80_STATE_MEM_READ_CB:
    next = (cb_col_ == 6 && cb_quad_ != 1) ? Z80_STATE_MEM_WRITE_CB : Z80_STATE_DECODE;
    break;

  case Z80_STATE_MEM_WRITE1:
    next = (push_d16_ || ST_A16_SP) ? Z80_STATE_MEM_WRITE2 : Z80_STATE_DECODE;
    if (RST_NN) next = Z80_STATE_MEM_WRITE2;
    break;

  case Z80_STATE_MEM_WRITE2:
    next = interrupt2 ? Z80_STATE_DELAY_B : Z80_STATE_DECODE;
    break;

  case Z80_STATE_MEM_WRITE_CB:
    next = Z80_STATE_DECODE;
    break;

  case Z80_STATE_DELAY_A:
    next = Z80_STATE_MEM_WRITE1;
    if (RET_CC) next = take_branch_ ? Z80_STATE_MEM_READ1  : Z80_STATE_DECODE;
    break;

  case Z80_STATE_DELAY_B:
    next = Z80_STATE_DELAY_C;
    if ((CALL_CC_A16 && take_branch_) || CALL_A16) next = Z80_STATE_MEM_WRITE1;
    break;

  case Z80_STATE_DELAY_C:
    next = Z80_STATE_DECODE;
    break;
  }

  return next;
}

//-----------------------------------------------------------------------------

int Z80::next_interrupt() const {
  if (state_ != Z80_STATE_DECODE) return -1;

  if (interrupt2) {
    // Someone could've changed the interrupt mask or flags while we were
    // handling the interrupt, so we have to compute the new PC at the very
    // last second.

    int actual_interrupt = -1;
    uint8_t interrupts = imask_latch & intf_;
    if (interrupts & INT_JOYPAD) actual_interrupt = 4; // joypad
    if (interrupts & INT_SERIAL) actual_interrupt = 3; // serial
    if (interrupts & INT_TIMER)  actual_interrupt = 2; // timer
    if (interrupts & INT_STAT)   actual_interrupt = 1; // lcd stat
    if (interrupts & INT_VBLANK) actual_interrupt = 0; // vblank

    return actual_interrupt;
  }

  return -1;
}

//-----------------------------------------------------------------------------
// We're at the end of a tick() for a completed instruction.
// Compute the new PC and put it on the bus so we can fetch the next instruction.
// If we're jumping to an interrupt vector, ack the interrupt that triggered it.

uint16_t Z80::next_pc(int next_interrupt) const {
  if (state_ != Z80_STATE_DECODE) return pc;

  if (interrupt2) {
    if (next_interrupt >= 0) {
      return uint16_t(0x0040 + (next_interrupt * 8));
    }
    else {
      return 0x0000;
    }
  }
  else if (take_branch_) {
    bool jump16 = JP_CC_A16 || JP_A16 || CALL_CC_A16 || CALL_A16 || RET || RETI || RET_CC;

    if (JP_HL)                  return hl;
    else if (RST_NN)            return op_ - 0xC7;
    else if (JR_CC_R8 || JR_R8) return pc + 2 + (int8_t)data_lo_;
    else if (jump16)            return data16_;
  }
  else if (fetch_d16_) {
    return pc + 3;
  }
  else if (fetch_d8_ || PREFIX_CB) {
    return pc + 2;
  }
  else {
    return pc + 1;
  }

  return pc;
}

//-----------------------------------------------------------------------------

CpuBus Z80::next_bus() const {
  CpuBus bus = {TAG_NONE, 0, 0, false, false};

  switch(state_) {
  case Z80_STATE_DECODE:
    bus.tag = TAG_OPCODE;
    bus.addr = pc_;
    bus.read = true;
    bus.write = false;
    break;
  case Z80_STATE_DECODE_CB:
    bus.tag = TAG_OPCODE_CB;
    bus.addr = pc + 1;
    bus.read = true;
    break;
  case Z80_STATE_HALT:
    bus.tag = TAG_OPCODE;
    bus.addr = pc_;
    bus.read = true;
    break;

  case Z80_STATE_MEM_READ1:
    if (fetch_d8_)       { bus.tag = TAG_ARG0;  bus.addr = pc + 1; }
    else if (fetch_d16_) { bus.tag = TAG_ARG0;  bus.addr = pc + 1; }
    else if (LD_A_AT_C)  { bus.tag = TAG_DATA0; bus.addr = 0xFF00 | c; }
    else if (LD_A_AT_BC) { bus.tag = TAG_DATA0; bus.addr = bc; }
    else if (LD_A_AT_DE) { bus.tag = TAG_DATA0; bus.addr = de; }
    else if (get_hl_)    { bus.tag = TAG_DATA0; bus.addr = hl; }
    else if (pop_d16_)   { bus.tag = TAG_DATA0; bus.addr = sp; }
    else if (RET_CC)     { bus.tag = TAG_DATA0; bus.addr = sp; }
    else                 { assert(false); }
    bus.read = true;
    break;
  case Z80_STATE_MEM_READ2:
    if      (fetch_d16_) { bus.tag = TAG_ARG1;  bus.addr = pc + 2; }
    else if (pop_d16_)   { bus.tag = TAG_DATA1; bus.addr = sp + 1; }
    else if (LD_A_AT_A8) { bus.tag = TAG_DATA0; bus.addr = 0xFF00 | data_lo_; }
    else                 { assert(false); }
    bus.read = true;
    break;
  case Z80_STATE_MEM_READ3:
    bus.tag = TAG_DATA0;
    bus.addr = data16_;
    bus.read = true;
    break;
  case Z80_STATE_MEM_READ_CB:
    bus.tag = TAG_DATA0;
    bus.addr = hl;
    bus.read = true;
    break;

  case Z80_STATE_MEM_WRITE1:
    if (ST_RR_A) {
      if      (ST_BC_A)  bus.addr = bc;
      else if (ST_DE_A)  bus.addr = de;
      else if (ST_HLP_A) bus.addr = hl;
      else if (ST_HLM_A) bus.addr = hl;

      bus.data = a;
    }
    else if (put_hl_) {
      bus.addr = hl;

      if      (INC_AT_HL)    bus.data = data_lo_ + 1;
      else if (DEC_AT_HL)    bus.data = data_lo_ - 1;
      else if (ST_HL_D8)     bus.data = (uint8_t)data_lo_;
      else if (MV_OPS_ST_HL) {
        bus.data = reg_fetch8();
      }
    }
    else if (push_d16_) {
      bus.addr = sp - 1;
      if (interrupt2)                    bus.data = (uint8_t)((pc) >> 8);
      else  if (CALL_CC_A16 || CALL_A16) bus.data = (uint8_t)((pc + 3) >> 8);
      else if (RST_NN)                   bus.data = (uint8_t)((pc + 1) >> 8);
      else {
        bus.data = (uint8_t)(reg_fetch16() >> 8);
      }
    }
    else if (ST_A16_A) {
      bus.addr = data16_;
      bus.data = a;
    }
    else if (ST_A8_A) {
      bus.addr = 0xFF00 | data_lo_;
      bus.data = a;
    }
    else if (ST_C_A) {
      bus.addr = 0xFF00 | c;
      bus.data = a;
    }
    else if (ST_A16_SP) {
      bus.addr = data16_;
      bus.data = (uint8_t)sp;
    }

    bus.read = false;
    bus.write = true;
    break;
  case Z80_STATE_MEM_WRITE2:
    if (push_d16_) {
      bus.addr = sp - 2;
      if (interrupt2)                   bus.data = (uint8_t)(pc);
      else if (CALL_CC_A16 || CALL_A16) bus.data = (uint8_t)(pc + 3);
      else if (RST_NN)                  bus.data = (uint8_t)(pc + 1);
      else                              bus.data = (uint8_t)reg_fetch16();
    }
    else if (ST_A16_SP) {
      bus.addr = data16_ + 1;
      bus.data = (uint8_t)(sp >> 8);
    }
    bus.write = true;
    break;
  case Z80_STATE_MEM_WRITE_CB:
    bus.addr = hl;
    bus.write = true;
    bus.data = (uint8_t)alu_out_; // this is why we can't move the alu execution down...
    break;

  case Z80_STATE_DELAY_A: break;
  case Z80_STATE_DELAY_B: break;
  case Z80_STATE_DELAY_C: break;
  }

  return bus;
}

//-----------------------------------------------------------------------------

void Z80::decode() {
  
  quad_ = (op_ >> 6) & 3;
  row_ = (op_ >> 3) & 7;
  col_ = (op_ >> 0) & 7;
  odd_row_ = row_ & 1;

  //----------

  take_branch_ = false;
  get_hl_ = false;
  put_hl_ = false;
  push_d16_ = false;
  pop_d16_ = false;
  fetch_d8_ = false;
  fetch_d16_ = false;
  any_read_ = false;
  any_write_ = false;

  bool cond_pass = false;
  switch (row_ & 3) {
  case 0: cond_pass = !(f & F_ZERO); break;
  case 1: cond_pass = (f & F_ZERO); break;
  case 2: cond_pass = !(f & F_CARRY); break;
  case 3: cond_pass = (f & F_CARRY); break;
  }

  take_branch_ |= CALL_A16 || JP_A16 || RET || RETI || JP_HL || RST_NN || JR_R8;
  take_branch_ |= (JR_CC_R8 || RET_CC || JP_CC_A16 || CALL_CC_A16) && cond_pass;

  get_hl_ |= INC_AT_HL || DEC_AT_HL || LD_A_AT_HLP || LD_A_AT_HLM;
  get_hl_ |= (col_ == 6);

  put_hl_ = INC_AT_HL || DEC_AT_HL || ST_HL_D8 || ST_HLP_A || ST_HLM_A;
  put_hl_ |= (quad_ == 1) && (row_ == 6);

  push_d16_ |= (quad_ == 3) && ((col_ == 5) || (col_ == 7));
  push_d16_ |= (take_branch_ && CALL_CC_A16);

  pop_d16_ |= RET || RETI;
  pop_d16_ |= (quad_ == 3) && (col_ == 1 && !odd_row_);
  pop_d16_ |= (take_branch_ && RET_CC);

  fetch_d8_ |= (quad_ == 0) && (col_ == 6);
  fetch_d8_ |= (quad_ == 0) && (col_ == 0 && row_ >= 3);
  fetch_d8_ |= LD_A_AT_A8 || LD_HL_SP_R8 || ST_A8_A || ALU_A_D8 || ADD_SP_R8;

  fetch_d16_ |= (quad_ == 0) && (col_ == 1 && !odd_row_);
  fetch_d16_ |= (quad_ == 3) && (col_ == 2 && row_ <= 3);
  fetch_d16_ |= (quad_ == 3) && (col_ == 4);
  fetch_d16_ |= ST_A16_SP || CALL_A16 || JP_A16 || ST_A16_A || LD_A_AT_A16;

  any_read_ |= fetch_d8_ || fetch_d16_ || get_hl_ || pop_d16_;
  any_read_ |= LD_A_AT_BC || LD_A_AT_DE || LD_A_AT_C;

  any_write_ |= put_hl_;
  any_write_ |= push_d16_;
  any_write_ |= ST_A16_A || ST_A8_A || ST_C_A || ST_BC_A || ST_DE_A || ST_A16_SP;

  //----------
  // special handling for interrupts

  if (interrupt2) {
    take_branch_ = true;
    get_hl_ = false;
    put_hl_ = false;
    push_d16_ = true;
    pop_d16_ = false;
    fetch_d8_ = false;
    fetch_d16_ = false;
    any_read_ = false;
    any_write_ = true;
  }
}

//-----------------------------------------------------------------------------

uint8_t Z80::reg_fetch8() const {
  int mux = quad_ == 0 ? row_ : col_;
  if (PREFIX_CB) mux = cb_col_;
  if (ROTATE_OPS) mux = col_;

  switch(mux) {
  case 0: return b;
  case 1: return c;
  case 2: return d;
  case 3: return e;
  case 4: return h;
  case 5: return l;
  case 6: return data_lo_;
  case 7: return a;
  }

  return 0;
}

uint16_t Z80::reg_fetch16() const {

  switch(row_ >> 1) {
  case 0: return bc; break;
  case 1: return de; break;
  case 2: return hl; break;
  case 3: return PUSH_RR || POP_RR ? af : sp; break;
  }

  return 0;
}

void Z80::reg_put8(int mux, uint8_t reg) {
  switch (mux) {
  case 0: b = (uint8_t)reg; break;
  case 1: c = (uint8_t)reg; break;
  case 2: d = (uint8_t)reg; break;
  case 3: e = (uint8_t)reg; break;
  case 4: h = (uint8_t)reg; break;
  case 5: l = (uint8_t)reg; break;
  case 6: break;
  case 7: a = (uint8_t)reg; break;
  }
}

void Z80::reg_put16(int mux, uint16_t reg) {
  switch(mux) {
  case 0: bc = reg; break;
  case 1: de = reg; break;
  case 2: hl = reg; break;
  case 3: {
    if (POP_RR) {
      af = reg & 0xFFF0;
    } else {
      sp = reg;
    }
    break;
  }
  }
}

//-----------------------------------------------------------------------------

uint8_t alu4(const uint8_t op, const uint8_t a, const uint8_t b, const uint8_t c) {
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

AluOut alu(const uint8_t op, const uint8_t x, const uint8_t y, const uint8_t f) {
  uint8_t c1 = (op == 0 || op == 2 || op == 7) ? 0 : (f >> 4) & 1;
  uint8_t d1 = alu4(op, x & 0xF, y & 0xF, c1);

  uint8_t c2 = (op == 4) ? 1 : (d1 >> 4) & 1;
  uint8_t d2 = alu4(op, x >> 4, y >> 4, c2);

  AluOut out = { uint8_t((d2 << 4) | (d1 & 0xF)), 0 };
  if (op == 2 || op == 3 || op == 7) out.f |= F_NEGATIVE;
  if (c2)         out.f |= F_HALF_CARRY;
  if (d2 & 0x10)  out.f |= F_CARRY;
  if (out.x == 0) out.f |= F_ZERO;
  return out;
}

//-----------------------------------------------------------------------------
// The logic is more annoying, but this can be implemented as two 4-bit additions

AluOut daa(uint8_t x, uint8_t f) {
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
  AluOut out = { uint8_t((hi << 4) | (lo & 0xF)), 0 };
  if (c) out.f |= F_CARRY;
  if ((hi >> 4) && !n) out.f |= F_CARRY;
  if (!out.x) out.f |= F_ZERO;
  return out;
}

//-----------------------------------------------------------------------------

AluOut rlu(const uint8_t op, const uint8_t x, const uint8_t f) {
  AluOut out = { 0 };

  switch (op) {
  case 0:
    out.x = (x << 1) | (x >> 7);
    out.f = (x >> 7) ? F_CARRY : 0;
    break;
  case 1:
    out.x = (x >> 1) | (x << 7);
    out.f = (x & 1) ? F_CARRY : 0;
    break;
  case 2:
    out.x = (x << 1) | (f & F_CARRY ? 1 : 0);
    out.f = (x >> 7) ? F_CARRY : 0;
    break;
  case 3:
    out.x = (x >> 1) | ((f & F_CARRY ? 1 : 0) << 7);
    out.f = (x & 1) ? F_CARRY : 0;
    break;
  case 4:
    return daa(x, f);
  case 5:
    out.x = ~x;
    out.f = f | 0x60;
    break;
  case 6:
    out.x = x;
    out.f = (f & 0x80) | 0x10;
    break;
  case 7:
    out.x = x;
    out.f = (f & 0x90) ^ 0x10;
    break;
  }

  out.x &= 0xFF;
  if (!out.x) out.f |= F_ZERO;

  return out;
}

//-----------------------------------------------------------------------------
// idempotent

AluOut cb(const uint8_t quad, const uint8_t row, const uint8_t x, const uint8_t f) {
  AluOut out = {0};

  switch (quad) {
  case 0: {
    switch (row) {
    case 0:
    case 1:
    case 2:
    case 3: {
      out = rlu(row, x, f);
      break;
    }
    // SLA
    case 4: {
      out.x = (x << 1) & 0xFF;
      if (x >> 7)     out.f |= F_CARRY;
      if (out.x == 0) out.f |= F_ZERO;
      break;
    }
    
    // SRA
    case 5: {
      out.x = ((x >> 1) | (x & 0x80)) & 0xFF;
      if (x & 1)      out.f |= F_CARRY;
      if (out.x == 0) out.f |= F_ZERO;
      break;
    }

    // SWAP
    case 6: {
      out.x = ((x << 4) | (x >> 4)) & 0xFF;
      if (out.x == 0) out.f |= F_ZERO;
      break;
    }

    // SRL
    case 7: {
      out.x = (x >> 1) & 0xFF;
      if (x & 1)      out.f |= F_CARRY;
      if (out.x == 0) out.f |= F_ZERO;
      break;
    }
    }
    break;
  }
          // BIT
  case 1: {
    bool bit_mux = (x >> row) & 1;
    out.f = (f & 0x10) | 0x20;
    if (!bit_mux) out.f |= F_ZERO;
    out.x = x;
    break;
  }
          // RES
  case 2: {
    out.f = f;
    out.x = x & (~(1 << row));
    break;
  }
          // SET
  case 3: {
    out.f = f;
    out.x = x | (1 << row);
    break;
  }
  }

  return out;
}

//-----------------------------------------------------------------------------
// idempotent

AluOut Z80::exec(uint8_t src) const {
  AluOut out = {0};

  if (PREFIX_CB) {
    out = cb(cb_quad_, cb_row_, src, f);
  }
  else if (INC_R) {
    out = alu(0, src, 1, 0);
  }
  else if (DEC_R) {
    out = alu(2, src, 1, 0);
  }
  else if (ADD_HL_RR) {
    uint16_t blah = reg_fetch16();
    bool halfcarry = (blah & 0x0FFF) + (hl & 0x0FFF) > 0x0FFF;
    bool carry =     (blah & 0xFFFF) + (hl & 0xFFFF) > 0xFFFF;

    out.x = blah + hl;
    out.f = (halfcarry ? F_HALF_CARRY : 0) | (carry ? F_CARRY : 0);
  }
  else if (ADD_SP_R8 || LD_HL_SP_R8) {
    bool halfcarry = (sp & 0x000F) + (data_lo_ & 0x000F) > 0x000F;
    bool carry =     (sp & 0x00FF) + (data_lo_ & 0x00FF) > 0x00FF;

    out.x = sp + (int8_t)data_lo_;
    out.f = (halfcarry ? F_HALF_CARRY : 0) | (carry ? F_CARRY : 0);
  }
  else if (ROTATE_OPS) {
    out = rlu(row_, src, f);
    if (row_ <= 3) out.f &= ~F_ZERO;
  }
  else if (ALU_OPS || ALU_A_D8) {
    out = alu(row_, a, src, f);
    out.x = (row_ == 7) ? a : out.x;
  }

  return out;
}

//-----------------------------------------------------------------------------

void Z80::dump(std::string& out) {
  sprintf(out, "CYC %d\n", cycle);
  int bgb = (cycle * 2) + 0x00B2D5E6;
  sprintf(out, "BGB 0x%08x\n", bgb);
  sprintf(out, "op 0x%02x\n", op_);
  sprintf(out, "af 0x%04x\n", af);
  sprintf(out, "bc 0x%04x\n", bc);
  sprintf(out, "de 0x%04x\n", de);
  sprintf(out, "hl 0x%04x\n", hl);
  sprintf(out, "sp 0x%04x\n", sp);
  sprintf(out, "pc 0x%04x\n", pc);
  sprintf(out, "ime %d\n", ime);
  sprintf(out, "ime_delay %d\n", ime_delay);
}

//-----------------------------------------------------------------------------
