#include "Z80.h"

#include "Constants.h"

#include <assert.h>

//-----------------------------------------------------------------------------

#define NOP           (op_ == 0x00)

#define ST_BC_A       (op_ == 0x02)
#define ST_DE_A       (op_ == 0x12)
#define ST_HLP_A      (op_ == 0x22)
#define ST_HLM_A      (op_ == 0x32)

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

#define JR_CC_R8      (quad_ == 0 && col_ == 0 && row_ >= 4)
#define LD_RR_D16     (quad_ == 0 && col_ == 1 && !odd_row_)
#define ADD_HL_RR     (quad_ == 0 && col_ == 1 &&  odd_row_)
#define LD_A_AT_RR    (quad_ == 0 && col_ == 2 &&  odd_row_)
#define INC_RR        (quad_ == 0 && col_ == 3 && !odd_row_)
#define DEC_RR        (quad_ == 0 && col_ == 3 &&  odd_row_)
#define INC_R         (quad_ == 0 && col_ == 4)
#define DEC_R         (quad_ == 0 && col_ == 5)
#define LD_R_D8       (quad_ == 0 && col_ == 6)
#define ROTATE_OPS    (quad_ == 0 && col_ == 7)

#define MV_OPS        (quad_ == 1)
#define MV_OPS_ST_HL  (quad_ == 1 && row_ == 6)
#define MV_OPS_LD_HL  (quad_ == 1 && col_ == 6)

#define ALU_OPS       (quad_ == 2)
#define ALU_OPS_LD_HL (quad_ == 2 && col_ == 6)

#define RET_CC        (quad_ == 3 && col_ == 0 && row_ <= 3)
#define POP_RR        (quad_ == 3 && col_ == 1 && !odd_row_)
#define JP_CC_A16     (quad_ == 3 && col_ == 2 && row_ <= 3)
#define CALL_CC_A16   (quad_ == 3 && col_ == 4 && row_ <= 3)
#define PUSH_RR       (quad_ == 3 && col_ == 5 && !odd_row_)
#define ALU_A_D8      (quad_ == 3 && col_ == 6)
#define RST_NN        (quad_ == 3 && col_ == 7)

AluOut cb(const uint8_t quad, const uint8_t row, const uint8_t x, const uint8_t f);

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
    if (interrupt2) {
      op_ = 0x00;
    }
  }

  quad_ = (op_ >> 6) & 3;
  row_ = (op_ >> 3) & 7;
  col_ = (op_ >> 0) & 7;
  odd_row_ = row_ & 1;
  cb_quad_ = (op_cb_ >> 6) & 3;
  cb_row_ = (op_cb_ >> 3) & 7;
  cb_col_ = (op_cb_ >> 0) & 7;

  //----------

  take_branch_ = false;

  bool cond_pass = false;
  switch (row_ & 3) {
  case 0: cond_pass = !(f & F_ZERO); break;
  case 1: cond_pass = (f & F_ZERO); break;
  case 2: cond_pass = !(f & F_CARRY); break;
  case 3: cond_pass = (f & F_CARRY); break;
  }

  take_branch_ |= CALL_A16 || JP_A16 || RET || RETI || JP_HL || RST_NN || JR_R8;
  take_branch_ |= (JR_CC_R8 || RET_CC || JP_CC_A16 || CALL_CC_A16) && cond_pass;
  take_branch_ |= (interrupt2 != 0);

  //----------

  state_ = next_state();

  if (state_ == Z80_STATE_PUSH1) sp--;
  if (state_ == Z80_STATE_PUSH2) sp--;

  //----------------------------------------
  // compute new pc

  int next_int = next_interrupt();
  pc_ = next_pc(next_int);
  if (next_int >= 0) int_ack_ = 1 << next_int;

  //----------------------------------------

  CpuBus next_bus2 = next_bus();

  if (state_ == Z80_STATE_POP1)  sp++;
  if (state_ == Z80_STATE_POP2)  sp++;

  bus_tag_ = (MemTag)next_bus2.tag;
  mem_addr_ = next_bus2.addr;
  mem_out_ = next_bus2.data;
  mem_read_ = next_bus2.read;
  mem_write_ = next_bus2.write;

  return next_bus2;
}

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

CpuOut Z80::tock_t2() {

  //if (opcount == 0x0017519b) __debugbreak();

  AluOut out = exec(reg_fetch8());
  alu_out_ = out.x;
  f_ = out.f;

  if (state == Z80_STATE_DECODE && state_ == Z80_STATE_HALT) unhalt = 0;

  // Write all our registers from the previous instruction before the new opcode shows up.
  if (state_ == Z80_STATE_DECODE) {
    pc = pc_;
    opcount = opcount + 1;
    uint8_t mask = PREFIX_CB ? cb_flag_mask[cb_quad_] : flag_mask[op_];
    if (POP_AF)  f = data_lo_ & 0xF0;
    else         f = (f & ~mask) | (f_ & mask);
  }

  if (state_ == Z80_STATE_DECODE) {
    if      (MV_OPS)      reg_put8(row_,    (uint8_t)reg_fetch8());
    else if (INC_R)       reg_put8(row_,    (uint8_t)alu_out_);
    else if (DEC_R)       reg_put8(row_,    (uint8_t)alu_out_);
    else if (LD_R_D8)     reg_put8(row_,    (uint8_t)data16_);
    else if (ALU_A_D8)    reg_put8(7,       (uint8_t)alu_out_);
    else if (ALU_OPS)     reg_put8(7,       (uint8_t)alu_out_);
    else if (ROTATE_OPS)  reg_put8(7,       (uint8_t)alu_out_);
    else if (LD_A_AT_RR)  reg_put8(7,       (uint8_t)data16_);
    else if (LD_A_AT_A8)  reg_put8(7,       (uint8_t)data16_);
    else if (LD_A_AT_C)   reg_put8(7,       (uint8_t)data16_);
    else if (LD_A_AT_A16) reg_put8(7,       (uint8_t)data16_);
    else if (PREFIX_CB)   reg_put8(cb_col_, (uint8_t)alu_out_);
  }

  if (state_ == Z80_STATE_DECODE) {
    if      (LD_RR_D16) {
      switch(row_ >> 1) {
      case 0: bc = data16_; break;
      case 1: de = data16_; break;
      case 2: hl = data16_; break;
      case 3: sp = data16_; break;
      }
    }
    else if (INC_RR) {
      switch(row_ >> 1) {
      case 0: bc++; break;
      case 1: de++; break;
      case 2: hl++; break;
      case 3: sp++; break;
      }
    }
    else if (DEC_RR) {
      switch(row_ >> 1) {
      case 0: bc--; break;
      case 1: de--; break;
      case 2: hl--; break;
      case 3: sp--; break;
      }
    }
    else if (POP_RR)      {
      switch(row_ >> 1) {
      case 0: bc = data16_; break;
      case 1: de = data16_; break;
      case 2: hl = data16_; break;
      case 3: af = data16_ & 0xFFF0; break;
      }

    }
    else if (ADD_HL_RR)                   hl = alu_out_;
    else if (LD_HL_SP_R8)                 hl = alu_out_;
    else if (ST_HLP_A)                    hl = hl + 1;
    else if (ST_HLM_A)                    hl = hl - 1;
    else if (LD_A_AT_HLP)                 hl = hl + 1;
    else if (LD_A_AT_HLM)                 hl = hl - 1;
    else if (ADD_SP_R8)                   sp = alu_out_;
    else if (MV_SP_HL)                    sp = hl;
    else if (ADD_SP_R8)                   sp = sp + (int8_t)data_lo_;
  }

  //----------
  // When we finish an instruction, update our interrupt master enable.

  ime = ime_delay;

  if (state_ == Z80_STATE_DECODE) {
    if (interrupt2) { ime = false;     ime_delay = false; }
    else if (RETI)  { ime = true;      ime_delay = true; }
    else if (DI)    { ime = false;     ime_delay = false; }
    else if (EI)    { ime = ime_delay; ime_delay = true; }
  }

  //----------
  // Gameboy weirdness - the "real" interrupt vector is determined by the
  // state of imask/intf after pushing the first byte of PC onto the stack.

  if (interrupt2 && state_ == Z80_STATE_PUSH2) {
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

void fail() { printf("fail\n"); }

Z80::Z80State Z80::next_state() const {
  Z80State next = Z80_STATE_DECODE;

  switch (state) {
  case Z80_STATE_DECODE:
    if      (interrupt2)    next = Z80_STATE_INTERRUPT;
    else if (HALT)          next = ((imask_ & intf_) && !ime) ? Z80_STATE_DECODE : Z80_STATE_HALT;
    else if (MV_OPS_ST_HL)  next = Z80_STATE_MEM_WRITE1;
    else if (ST_HLP_A)      next = Z80_STATE_MEM_WRITE1;
    else if (ST_HLM_A)      next = Z80_STATE_MEM_WRITE1;
    else if (ST_C_A)        next = Z80_STATE_MEM_WRITE1;
    else if (ST_BC_A)       next = Z80_STATE_MEM_WRITE1;
    else if (ST_DE_A)       next = Z80_STATE_MEM_WRITE1;
    
    else if (INC_AT_HL)     next = Z80_STATE_MEM_READ1;
    else if (DEC_AT_HL)     next = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_HLP)   next = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_HLM)   next = Z80_STATE_MEM_READ1;
    else if (MV_OPS_LD_HL)  next = Z80_STATE_MEM_READ1;
    else if (ALU_OPS_LD_HL) next = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_BC)    next = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_DE)    next = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_C)     next = Z80_STATE_MEM_READ1;
    
    else if (RET_CC)        next = Z80_STATE_DELAY_A;
    else if (RST_NN)        next = Z80_STATE_DELAY_A;
    
    else if (INC_RR)        next = Z80_STATE_DELAY_C;
    else if (DEC_RR)        next = Z80_STATE_DELAY_C;
    else if (ADD_HL_RR)     next = Z80_STATE_DELAY_C;
    else if (MV_SP_HL)      next = Z80_STATE_DELAY_C;
    
    else if (PREFIX_CB)     next = Z80_STATE_DECODE_CB;
    
    else if (PUSH_RR)       next = Z80_STATE_PUSH_DELAY;

    else if (RET)           next = Z80_STATE_POP1;
    else if (RETI)          next = Z80_STATE_POP1;
    else if (POP_RR)        next = Z80_STATE_POP1;

    else if (LD_R_D8)       next = Z80_STATE_ARG1;
    else if (JR_CC_R8)      next = Z80_STATE_ARG1;
    else if (JR_R8)         next = Z80_STATE_ARG1;
    else if (LD_A_AT_A8)    next = Z80_STATE_ARG1;
    else if (LD_HL_SP_R8)   next = Z80_STATE_ARG1;
    else if (ST_A8_A)       next = Z80_STATE_ARG1;
    else if (ALU_A_D8)      next = Z80_STATE_ARG1;
    else if (ADD_SP_R8)     next = Z80_STATE_ARG1;
    else if (LD_A_AT_A16)   next = Z80_STATE_ARG1;
    else if (LD_RR_D16)     next = Z80_STATE_ARG1;
    else if (ST_A16_A)      next = Z80_STATE_ARG1;
    else if (ST_A16_SP)     next = Z80_STATE_ARG1;
    else if (JP_A16)        next = Z80_STATE_ARG1;
    else if (JP_CC_A16)     next = Z80_STATE_ARG1;
    else if (CALL_A16)      next = Z80_STATE_ARG1;
    else if (CALL_CC_A16)   next = Z80_STATE_ARG1;

    else if (NOP)           next = Z80_STATE_DECODE;
    else if (DI)            next = Z80_STATE_DECODE;
    else if (EI)            next = Z80_STATE_DECODE;
    else if (MV_OPS)        next = Z80_STATE_DECODE;
    else if (ALU_OPS)       next = Z80_STATE_DECODE;
    else if (INC_R)         next = Z80_STATE_DECODE;
    else if (DEC_R)         next = Z80_STATE_DECODE;
    else if (ROTATE_OPS)    next = Z80_STATE_DECODE;
    else if (JP_HL)         next = Z80_STATE_DECODE;

    else {
      printf("fail 0x%02x\n", op_);
    }
    break;

  case Z80_STATE_DECODE_CB:
    if (PREFIX_CB)          next = cb_col_ == 6 ? Z80_STATE_MEM_READ1 : Z80_STATE_DECODE;
    else fail();
    break;

  case Z80_STATE_HALT:
    if (HALT)               next = unhalt ? Z80_STATE_DECODE : Z80_STATE_HALT;
    else fail();
    break;

  case Z80_STATE_INTERRUPT:
    if (interrupt2) next = Z80_STATE_PUSH_DELAY;
    else fail();
    break;

  //----------

  case Z80_STATE_PUSH_DELAY:
    if      (interrupt2)    next = Z80_STATE_PUSH1;
    else if (PUSH_RR)       next = Z80_STATE_PUSH1;
    else if (CALL_CC_A16)   next = Z80_STATE_PUSH1;
    else if (CALL_A16)      next = Z80_STATE_PUSH1;
    else fail();
    break;

  case Z80_STATE_PUSH1:
    if      (interrupt2)    next = Z80_STATE_PUSH2;
    else if (PUSH_RR)       next = Z80_STATE_PUSH2;
    else if (CALL_A16)      next = Z80_STATE_PUSH2;
    else if (CALL_CC_A16)   next = Z80_STATE_PUSH2;
    else if (RST_NN)        next = Z80_STATE_PUSH2;
    else fail();
    break;

  case Z80_STATE_PUSH2:
    if      (interrupt2)    next = Z80_STATE_DECODE;
    else if (PUSH_RR)       next = Z80_STATE_DECODE;
    else if (CALL_A16)      next = Z80_STATE_DECODE;
    else if (CALL_CC_A16)   next = Z80_STATE_DECODE;
    else if (RST_NN)        next = Z80_STATE_DECODE;
    else fail();
    break;

  //----------

  case Z80_STATE_POP1:
    if      (RET)           next = Z80_STATE_POP2;
    else if (RETI)          next = Z80_STATE_POP2;
    else if (RET_CC)        next = Z80_STATE_POP2;
    else if (POP_RR)        next = Z80_STATE_POP2;
    else fail();
    break;

  case Z80_STATE_POP2:
    if      (RET)           next = Z80_STATE_DELAY_C;
    else if (RETI)          next = Z80_STATE_DELAY_C;
    else if (RET_CC)        next = Z80_STATE_DELAY_C;
    else if (POP_RR)        next = Z80_STATE_DECODE;
    else fail();
    break;

  //----------

  case Z80_STATE_ARG1:
    if      (LD_A_AT_A8)    next = Z80_STATE_MEM_READ1;
    else if (ST_HL_D8)      next = Z80_STATE_MEM_WRITE1;
    else if (ST_A8_A)       next = Z80_STATE_MEM_WRITE1;
    else if (ADD_SP_R8)     next = Z80_STATE_DELAY_B;
    else if (LD_HL_SP_R8)   next = Z80_STATE_DELAY_C;
    else if (JR_R8)         next = Z80_STATE_DELAY_C;
    else if (LD_A_AT_A16)   next = Z80_STATE_ARG2;
    else if (LD_RR_D16)     next = Z80_STATE_ARG2;
    else if (ST_A16_A)      next = Z80_STATE_ARG2;
    else if (ST_A16_SP)     next = Z80_STATE_ARG2;
    else if (JP_A16)        next = Z80_STATE_ARG2;
    else if (JP_CC_A16)     next = Z80_STATE_ARG2;
    else if (CALL_A16)      next = Z80_STATE_ARG2;
    else if (CALL_CC_A16)   next = Z80_STATE_ARG2;
    else if (LD_R_D8)       next = Z80_STATE_DECODE;
    else if (ALU_A_D8)      next = Z80_STATE_DECODE;
    else if (JR_CC_R8)      next = take_branch_ ? Z80_STATE_DELAY_C : Z80_STATE_DECODE;
    else fail();

    break;

  case Z80_STATE_ARG2:
    if      (LD_A_AT_A16)   next = Z80_STATE_MEM_READ1;
    else if (ST_A16_A)      next = Z80_STATE_MEM_WRITE1;
    else if (ST_A16_SP)     next = Z80_STATE_MEM_WRITE1;
    else if (CALL_A16)      next = Z80_STATE_PUSH_DELAY;
    else if (JP_A16)        next = Z80_STATE_DELAY_C;
    else if (LD_RR_D16)     next = Z80_STATE_DECODE;
    else if (CALL_CC_A16)   next = take_branch_ ? Z80_STATE_PUSH_DELAY : Z80_STATE_DECODE;
    else if (JP_CC_A16)     next = take_branch_ ? Z80_STATE_DELAY_C : Z80_STATE_DECODE;
    else fail();
    break;

  //----------

  case Z80_STATE_MEM_READ1:
    if      (INC_AT_HL)     next = Z80_STATE_MEM_WRITE1;
    else if (DEC_AT_HL)     next = Z80_STATE_MEM_WRITE1;
    else if (ST_HLP_A)      next = Z80_STATE_MEM_WRITE1;
    else if (ST_HLM_A)      next = Z80_STATE_MEM_WRITE1;
    else if (PREFIX_CB)     next = Z80_STATE_MEM_WRITE1;
    else if (INC_AT_HL)     next = Z80_STATE_DECODE;
    else if (DEC_AT_HL)     next = Z80_STATE_DECODE;
    else if (LD_A_AT_HLP)   next = Z80_STATE_DECODE;
    else if (LD_A_AT_HLM)   next = Z80_STATE_DECODE;
    else if (MV_OPS_LD_HL)  next = Z80_STATE_DECODE;
    else if (ALU_OPS_LD_HL) next = Z80_STATE_DECODE;
    else if (LD_A_AT_BC)    next = Z80_STATE_DECODE;
    else if (LD_A_AT_DE)    next = Z80_STATE_DECODE;
    else if (LD_A_AT_C)     next = Z80_STATE_DECODE;
    else if (LD_A_AT_A16)   next = Z80_STATE_DECODE;
    else if (LD_A_AT_A8)    next = Z80_STATE_DECODE;
    else fail();

    break;

  case Z80_STATE_MEM_WRITE1:
    if      (ST_A16_SP)     next = Z80_STATE_MEM_WRITE2;
    else if (INC_AT_HL)     next = Z80_STATE_DECODE;
    else if (DEC_AT_HL)     next = Z80_STATE_DECODE;
    else if (ST_HLP_A)      next = Z80_STATE_DECODE;
    else if (ST_HLM_A)      next = Z80_STATE_DECODE;
    else if (ST_A16_A)      next = Z80_STATE_DECODE;
    else if (ST_A8_A)       next = Z80_STATE_DECODE;
    else if (MV_OPS_ST_HL)  next = Z80_STATE_DECODE;
    else if (ST_DE_A)       next = Z80_STATE_DECODE;
    else if (ST_HL_D8)      next = Z80_STATE_DECODE;
    else if (ST_C_A)        next = Z80_STATE_DECODE;
    else if (ST_BC_A)       next = Z80_STATE_DECODE;
    else if (PREFIX_CB)     next = Z80_STATE_DECODE;
    else fail();
    break;

  case Z80_STATE_MEM_WRITE2:
    if      (ST_A16_SP)     next = Z80_STATE_DECODE;
    else fail();
    break;

  case Z80_STATE_DELAY_A:
    if      (RST_NN)        next = Z80_STATE_PUSH1;
    else if (RET_CC)        next = take_branch_ ? Z80_STATE_POP1 : Z80_STATE_DECODE;
    else fail();
    break;

  case Z80_STATE_DELAY_B:
    if      (ADD_SP_R8)     next = Z80_STATE_DELAY_C;
    else fail();
    break;

  case Z80_STATE_DELAY_C:
    if      (ADD_SP_R8)     next = Z80_STATE_DECODE;
    else if (INC_RR)        next = Z80_STATE_DECODE;
    else if (DEC_RR)        next = Z80_STATE_DECODE;
    else if (ADD_HL_RR)     next = Z80_STATE_DECODE;
    else if (MV_SP_HL)      next = Z80_STATE_DECODE;
    else if (RET)           next = Z80_STATE_DECODE;
    else if (RETI)          next = Z80_STATE_DECODE;
    else if (RET_CC)        next = Z80_STATE_DECODE;
    else if (LD_HL_SP_R8)   next = Z80_STATE_DECODE;
    else if (JR_R8)         next = Z80_STATE_DECODE;
    else if (JR_CC_R8)      next = Z80_STATE_DECODE;
    else if (JP_A16)        next = Z80_STATE_DECODE;
    else if (JP_CC_A16)     next = Z80_STATE_DECODE;
    else fail();
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
  
  if (take_branch_) {
    if      (JP_HL)       return hl;
    else if (RST_NN)      return op_ - 0xC7;
    else if (JR_R8)       return pc + 2 + (int8_t)data_lo_;
    else if (JR_CC_R8)    return pc + 2 + (int8_t)data_lo_;
    else if (JP_A16)      return data16_;
    else if (JP_CC_A16)   return data16_;
    else if (CALL_A16)    return data16_;
    else if (CALL_CC_A16) return data16_;
    else if (RET)         return data16_;
    else if (RETI)        return data16_;
    else if (RET_CC)      return data16_;
    else                  return pc + 1; // do we ever get here?
  }

  else if (LD_RR_D16)   return pc + 3;
  else if (LD_A_AT_A16) return pc + 3;
  else if (ST_A16_SP)   return pc + 3;
  else if (ST_A16_A)    return pc + 3;
  else if (JP_A16)      return pc + 3;
  else if (JP_CC_A16)   return pc + 3;
  else if (CALL_A16)    return pc + 3;
  else if (CALL_CC_A16) return pc + 3;

  else if (LD_R_D8)     return pc + 2;
  else if (JR_CC_R8)    return pc + 2;
  else if (JR_R8)       return pc + 2;
  else if (LD_A_AT_A8)  return pc + 2;
  else if (LD_HL_SP_R8) return pc + 2;
  else if (ST_A8_A)     return pc + 2;
  else if (ALU_A_D8)    return pc + 2;
  else if (ADD_SP_R8)   return pc + 2;
  else if (PREFIX_CB)   return pc + 2;

  else                  return pc + 1;
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

  case Z80_STATE_INTERRUPT:
    break;

  case Z80_STATE_PUSH_DELAY:
    break;

  case Z80_STATE_PUSH1:
    if      (interrupt2)  bus.data = (uint8_t)((pc) >> 8);
    else if (PUSH_RR) {
      switch(row_ >> 1) {
      case 0: bus.data = b; break;
      case 1: bus.data = d; break;
      case 2: bus.data = h; break;
      case 3: bus.data = a; break;
      }
    }
    else if (CALL_A16)    bus.data = (uint8_t)((pc + 3) >> 8);
    else if (CALL_CC_A16) bus.data = (uint8_t)((pc + 3) >> 8);
    else if (RST_NN)      bus.data = (uint8_t)((pc + 1) >> 8);
    else fail();
    bus.addr = sp;
    bus.write = true;
    break;

  case Z80_STATE_PUSH2:
    if      (interrupt2)  bus.data = (uint8_t)(pc);
    else if (PUSH_RR) {
      switch(row_ >> 1) {
      case 0: bus.data = c; break;
      case 1: bus.data = e; break;
      case 2: bus.data = l; break;
      case 3: bus.data = f; break;
      }
    }
    else if (CALL_A16)    bus.data = (uint8_t)(pc + 3);
    else if (CALL_CC_A16) bus.data = (uint8_t)(pc + 3);
    else if (RST_NN)      bus.data = (uint8_t)(pc + 1);
    else fail();
    bus.addr = sp;
    bus.write = true;
    break;

  case Z80_STATE_POP1:
    if      (RET)    {}
    else if (RETI)   {}
    else if (RET_CC) {}
    else if (POP_RR) {}
    else fail();
    bus.tag = TAG_DATA0;
    bus.addr = sp;
    bus.read = true;
    break;

  case Z80_STATE_POP2:
    if      (RET)    {}
    else if (RETI)   {}
    else if (RET_CC) {}
    else if (POP_RR) {}
    bus.tag = TAG_DATA1;
    bus.addr = sp;
    bus.read = true;
    break;

  case Z80_STATE_ARG1:
    bus.tag = TAG_ARG0;
    bus.addr = pc + 1;
    bus.read = true;
    break;

  case Z80_STATE_ARG2:
    bus.tag = TAG_ARG1;
    bus.addr = pc + 2;
    bus.read = true;
    break;

  case Z80_STATE_MEM_READ1:
    if      (LD_A_AT_A16)   { bus.tag = TAG_DATA0; bus.addr = data16_; }
    else if (LD_A_AT_A8)    { bus.tag = TAG_DATA0; bus.addr = 0xFF00 | data_lo_; }
    else if (LD_A_AT_C)     { bus.tag = TAG_DATA0; bus.addr = 0xFF00 | c; }
    else if (LD_A_AT_BC)    { bus.tag = TAG_DATA0; bus.addr = bc; }
    else if (LD_A_AT_DE)    { bus.tag = TAG_DATA0; bus.addr = de; }
    else if (INC_AT_HL)     { bus.tag = TAG_DATA0; bus.addr = hl; }
    else if (DEC_AT_HL)     { bus.tag = TAG_DATA0; bus.addr = hl; }
    else if (LD_A_AT_HLP)   { bus.tag = TAG_DATA0; bus.addr = hl; }
    else if (LD_A_AT_HLM)   { bus.tag = TAG_DATA0; bus.addr = hl; }
    else if (MV_OPS_LD_HL)  { bus.tag = TAG_DATA0; bus.addr = hl; }
    else if (ALU_OPS_LD_HL) { bus.tag = TAG_DATA0; bus.addr = hl; }
    else if (PREFIX_CB)     { bus.tag = TAG_DATA0; bus.addr = hl; }
    else fail();
    bus.read = true;
    break;

  case Z80_STATE_MEM_WRITE1:
    if      (ST_BC_A)      { bus.addr = bc; bus.data = a; }
    else if (ST_DE_A)      { bus.addr = de; bus.data = a; }
    else if (ST_HLP_A)     { bus.addr = hl; bus.data = a; }
    else if (ST_HLM_A)     { bus.addr = hl; bus.data = a; }
    else if (INC_AT_HL)    { bus.addr = hl; bus.data = data_lo_ + 1; }
    else if (DEC_AT_HL)    { bus.addr = hl; bus.data = data_lo_ - 1; }
    else if (ST_HL_D8)     { bus.addr = hl; bus.data = (uint8_t)data_lo_; }
    else if (MV_OPS_ST_HL) { bus.addr = hl; bus.data = reg_fetch8(); }
    else if (PREFIX_CB)    { bus.addr = hl; bus.data = (uint8_t)cb(cb_quad_, cb_row_, reg_fetch8(), f).x; }
    else if (ST_A16_A)     { bus.addr = data16_; bus.data = a; }
    else if (ST_A8_A)      { bus.addr = 0xFF00 | data_lo_; bus.data = a; }
    else if (ST_C_A)       { bus.addr = 0xFF00 | c; bus.data = a; }
    else if (ST_A16_SP)    { bus.addr = data16_; bus.data = (uint8_t)sp; }
    else fail();
    bus.write = true;
    break;

  case Z80_STATE_MEM_WRITE2:
    if      (ST_A16_SP)   { bus.addr = data16_ + 1; bus.data = (uint8_t)(sp >> 8); }
    else fail();
    bus.write = true;
    break;

  case Z80_STATE_DELAY_A: break;
  case Z80_STATE_DELAY_B: break;
  case Z80_STATE_DELAY_C: break;
  }

  return bus;
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
    uint16_t blah = 0;
    switch(row_ >> 1) {
    case 0: blah = bc; break;
    case 1: blah = de; break;
    case 2: blah = hl; break;
    case 3: blah = sp; break;
    }

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
