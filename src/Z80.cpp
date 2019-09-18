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
  reg_in = reg_in_ = 0;

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
  bus_data_ = bus_data;

  pc_ = pc;
  bus_tag_ = TAG_NONE;
  state_ = state;
  reg_in_ = reg_in;
  int_ack_ = 0;
  data_lo_ = data_lo;
  data_hi_ = data_hi;

  if      (bus_tag == TAG_OPCODE) op_ = bus_data;
  if      (bus_tag == TAG_OPCODE_CB) op_cb_ = bus_data;
  if      (bus_tag == TAG_DATA0) data_lo_ = bus_data;
  else if (bus_tag == TAG_DATA1) data_hi_ = bus_data;
  else if (bus_tag == TAG_ARG0)  data_lo_ = bus_data;
  else if (bus_tag == TAG_ARG1)  data_hi_ = bus_data;

  //----------------------------------------
  // handle input data

  switch(state) {
  case Z80_STATE_DECODE:
    interrupt2 = (imask_ & intf_) && ime;
    if (interrupt2) {
      ime_ = false;
      op_ = 0x00;
    }

    decode();
    reg_in_ = reg_fetch(bus_data);
    break;
  case Z80_STATE_DECODE_CB:
    cb_quad_ = (op_cb_ >> 6) & 3;
    cb_row_ = (op_cb_ >> 3) & 7;
    cb_col_ = (op_cb_ >> 0) & 7;
    break;
  case Z80_STATE_HALT:
    break;

  case Z80_STATE_MEM_READ1:
    reg_in_ = bus_data_;
    break;
  case Z80_STATE_MEM_READ2:
    break;
  case Z80_STATE_MEM_READ3:
    break;
  case Z80_STATE_MEM_READ_CB:
    reg_in_ = bus_data_;
    break;

  case Z80_STATE_MEM_WRITE1:
    break;
  case Z80_STATE_MEM_WRITE2:
    break;
  case Z80_STATE_MEM_WRITE_CB:
    break;

  case Z80_STATE_DELAY_A:
    break;
  case Z80_STATE_DELAY_B:
    break;
  case Z80_STATE_DELAY_C:
    break;
  }

  //----------------------------------------
  // this alu chunk is moving down to tock()

  AluOut out = {0};

  switch(state) {
  case Z80_STATE_DECODE:
    out = exec((uint8_t)reg_in_);
    alu_out_ = out.x;
    f_ = out.f;
    break;
  case Z80_STATE_DECODE_CB:
    break;
  case Z80_STATE_HALT:
    break;

  case Z80_STATE_MEM_READ1:
    out = exec((uint8_t)reg_in_);
    alu_out_ = out.x;
    f_ = out.f;
    break;
  case Z80_STATE_MEM_READ2:
    out = exec((uint8_t)reg_in_);
    alu_out_ = out.x;
    f_ = out.f;
    break;
  case Z80_STATE_MEM_READ3:
    out = exec((uint8_t)reg_in_);
    alu_out_ = out.x;
    f_ = out.f;
    break;
  case Z80_STATE_MEM_READ_CB:
    out = exec((uint8_t)reg_in_);
    alu_out_ = out.x;
    f_ = out.f;
    break;

  case Z80_STATE_MEM_WRITE1:
    out = exec((uint8_t)reg_in_);
    alu_out_ = out.x;
    f_ = out.f;
    break;
  case Z80_STATE_MEM_WRITE2:
    out = exec((uint8_t)reg_in_);
    alu_out_ = out.x;
    f_ = out.f;
    break;
  case Z80_STATE_MEM_WRITE_CB:
    // breaks something
    //tick_exec_cb();
    break;

  case Z80_STATE_DELAY_A:
    break;
  case Z80_STATE_DELAY_B:
    break;
  case Z80_STATE_DELAY_C:
    break;
  }

  //----------------------------------------
  // choose new state

  switch (state) {
  case Z80_STATE_DECODE:
    state_ = Z80_STATE_DECODE;
    if (any_write_) state_ = Z80_STATE_MEM_WRITE1;
    if (any_read_)  state_ = Z80_STATE_MEM_READ1;
    if (RET_CC || RST_NN || PUSH_RR) state_ = Z80_STATE_DELAY_A;
    if (INC_RR || DEC_RR || ADD_HL_RR || MV_SP_HL) state_ = Z80_STATE_DELAY_B;
    if (PREFIX_CB) state_ = Z80_STATE_DECODE_CB;
    if (HALT) state_ = ((imask_ & intf_) && !ime) ? Z80_STATE_DECODE : Z80_STATE_HALT;
    break;

  case Z80_STATE_DECODE_CB:
    state_ = Z80_STATE_DECODE;
    state_ = (cb_col_ == 6) ? Z80_STATE_MEM_READ_CB : Z80_STATE_DECODE;
    break;

  case Z80_STATE_HALT:
    state_ = Z80_STATE_DECODE;
    state_ = unhalt ? Z80_STATE_DECODE : Z80_STATE_HALT;
    break;

  case Z80_STATE_MEM_READ1:
    state_ = Z80_STATE_DECODE;
    if (JR_R8 || (JR_CC_R8 && take_branch_) || LD_HL_SP_R8) state_ = Z80_STATE_DELAY_B;
    if (ADD_SP_R8) state_ = Z80_STATE_DELAY_C;
    if (any_write_) state_ = Z80_STATE_MEM_WRITE1;
    if (LD_A_AT_A8 || fetch_d16_ || pop_d16_) state_ = Z80_STATE_MEM_READ2;
    break;

  case Z80_STATE_MEM_READ2:
    state_ = Z80_STATE_DECODE;
    if (RET_CC || RET || RETI || JP_A16 || (JP_CC_A16 && take_branch_)) state_ = Z80_STATE_DELAY_B;
    if (any_write_) state_ = Z80_STATE_MEM_WRITE1;
    if ((CALL_CC_A16 && take_branch_) || CALL_A16) state_ = Z80_STATE_DELAY_C;
    if (LD_A_AT_A16) state_ = Z80_STATE_MEM_READ3;
    break;

  case Z80_STATE_MEM_READ3:
    state_ = Z80_STATE_DECODE;
    break;

  case Z80_STATE_MEM_READ_CB:
    state_ = Z80_STATE_DECODE;
    state_ = (cb_col_ == 6 && cb_quad_ != 1) ? Z80_STATE_MEM_WRITE_CB : Z80_STATE_DECODE;
    break;

  case Z80_STATE_MEM_WRITE1:
    state_ = Z80_STATE_DECODE;
    state_ = (push_d16_ || ST_A16_SP) ? Z80_STATE_MEM_WRITE2 : Z80_STATE_DECODE;
    if (RST_NN) state_ = Z80_STATE_MEM_WRITE2;
    break;

  case Z80_STATE_MEM_WRITE2:
    state_ = interrupt2 ? Z80_STATE_DELAY_C : Z80_STATE_DECODE;
    break;

  case Z80_STATE_MEM_WRITE_CB:
    state_ = Z80_STATE_DECODE;
    break;

  case Z80_STATE_DELAY_A:
    state_ = Z80_STATE_MEM_WRITE1;
    if (RET_CC) state_ = take_branch_ ? Z80_STATE_MEM_READ1  : Z80_STATE_DECODE;
    break;

  case Z80_STATE_DELAY_B:
    state_ = Z80_STATE_DECODE;
    break;

  case Z80_STATE_DELAY_C:
    state_ = Z80_STATE_DELAY_B;
    if ((CALL_CC_A16 && take_branch_) || CALL_A16) state_ = Z80_STATE_MEM_WRITE1;
    break;
  }

  //----------------------------------------
  // set up new state

  pc_ = pc;
  bus_tag_ = TAG_NONE;
  mem_addr_ = 0;
  mem_out_ = 0;
  mem_read_ = false;
  mem_write_ = false;

  switch(state_) {
  case Z80_STATE_DECODE:
    // We're at the end of a tick() for a completed instruction.
    // Compute the new PC and put it on the bus so we can fetch the next instruction.
    // If we're jumping to an interrupt vector, ack the interrupt that triggered it.

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

      if (actual_interrupt >= 0) {
        pc_ = uint16_t(0x0040 + (actual_interrupt * 8));
        int_ack_ = 1 << actual_interrupt;
      }
      else {
        pc_ = 0x0000;
      }
    }
    else if (take_branch_) {
      bool jump16 = JP_CC_A16 || JP_A16 || CALL_CC_A16 || CALL_A16 || RET || RETI || RET_CC;

      if (JP_HL)                  pc_ = hl;
      else if (RST_NN)            pc_ = op_ - 0xC7;
      else if (JR_CC_R8 || JR_R8) pc_ = pc + 2 + (int8_t)data_lo_;
      else if (jump16)            pc_ = data16_;
    }
    else if (fetch_d16_) {
      pc_ = pc + 3;
    }
    else if (fetch_d8_ || PREFIX_CB) {
      pc_ = pc + 2;
    }
    else {
      pc_ = pc + 1;
    }

    bus_tag_ = TAG_OPCODE;
    mem_addr_ = pc_;
    mem_read_ = true;
    mem_write_ = false;
    break;
  case Z80_STATE_DECODE_CB:
    bus_tag_ = TAG_OPCODE_CB;
    mem_addr_ = pc + 1;
    mem_read_ = true;
    break;
  case Z80_STATE_HALT:
    unhalt = 0;
    bus_tag_ = TAG_OPCODE;
    mem_addr_ = pc_;
    mem_read_ = true;
    break;




  case Z80_STATE_MEM_READ1:
    if (fetch_d8_)       { bus_tag_ = TAG_ARG0;  mem_addr_ = pc + 1; }
    else if (fetch_d16_) { bus_tag_ = TAG_ARG0;  mem_addr_ = pc + 1; }
    else if (LD_A_AT_C)  { bus_tag_ = TAG_DATA0; mem_addr_ = 0xFF00 | c; }
    else if (LD_A_AT_BC) { bus_tag_ = TAG_DATA0; mem_addr_ = bc; }
    else if (LD_A_AT_DE) { bus_tag_ = TAG_DATA0; mem_addr_ = de; }
    else if (get_hl_)    { bus_tag_ = TAG_DATA0; mem_addr_ = hl; }
    else if (pop_d16_)   { bus_tag_ = TAG_DATA0; mem_addr_ = sp; }
    else if (RET_CC)     { bus_tag_ = TAG_DATA0; mem_addr_ = sp; }
    else                 { assert(false); }
    mem_read_ = true;
    break;
  case Z80_STATE_MEM_READ2:
    if      (fetch_d16_) { bus_tag_ = TAG_ARG1;  mem_addr_ = pc + 2; }
    else if (pop_d16_)   { bus_tag_ = TAG_DATA1; mem_addr_ = sp + 1; }
    else if (LD_A_AT_A8) { bus_tag_ = TAG_DATA0; mem_addr_ = 0xFF00 | bus_data_; }
    else                 { assert(false); }
    mem_read_ = true;
    break;
  case Z80_STATE_MEM_READ3:
    bus_tag_ = TAG_DATA0;
    mem_addr_ = data16_;
    mem_read_ = true;
    break;
  case Z80_STATE_MEM_READ_CB:
    bus_tag_ = TAG_ARG1;
    mem_addr_ = hl;
    mem_read_ = true;
    break;



  case Z80_STATE_MEM_WRITE1:
    bus_tag_ = TAG_NONE;

    if (ST_RR_A) {
      if      (ST_BC_A)  mem_addr_ = bc;
      else if (ST_DE_A)  mem_addr_ = de;
      else if (ST_HLP_A) mem_addr_ = hl;
      else if (ST_HLM_A) mem_addr_ = hl;

      mem_out_ = a;
    }
    else if (put_hl_) {
      mem_addr_ = hl;

      if      (INC_AT_HL)    mem_out_ = (uint8_t)alu_out_;
      else if (DEC_AT_HL)    mem_out_ = (uint8_t)alu_out_;
      else if (ST_HL_D8)     mem_out_ = (uint8_t)bus_data_;
      else if (MV_OPS_ST_HL) mem_out_ = (uint8_t)reg_in_;
    }
    else if (push_d16_) {
      mem_addr_ = sp - 1;
      if (interrupt2)                    mem_out_ = (uint8_t)((pc) >> 8);
      else  if (CALL_CC_A16 || CALL_A16) mem_out_ = (uint8_t)((pc + 3) >> 8);
      else if (RST_NN)                   mem_out_ = (uint8_t)((pc + 1) >> 8);
      else                               mem_out_ = (uint8_t)(reg_in_ >> 8);
    }
    else if (ST_A16_A) {
      mem_addr_ = data16_;
      mem_out_ = a;
    }
    else if (ST_A8_A) {
      mem_addr_ = 0xFF00 | bus_data_;
      mem_out_ = a;
    }
    else if (ST_C_A) {
      mem_addr_ = 0xFF00 | c;
      mem_out_ = a;
    }
    else if (ST_A16_SP) {
      mem_addr_ = data16_;
      mem_out_ = (uint8_t)sp;
    }

    mem_read_ = false;
    mem_write_ = true;
    break;
  case Z80_STATE_MEM_WRITE2:
    if (push_d16_) {
      mem_addr_ = sp - 2;
      if (interrupt2)                   mem_out_ = (uint8_t)(pc);
      else if (CALL_CC_A16 || CALL_A16) mem_out_ = (uint8_t)(pc + 3);
      else if (RST_NN)                  mem_out_ = (uint8_t)(pc + 1);
      else                              mem_out_ = (uint8_t)reg_in_;
    }
    else if (ST_A16_SP) {
      mem_addr_ = data16_ + 1;
      mem_out_ = (uint8_t)(sp >> 8);
    }
    mem_write_ = true;
    break;
  case Z80_STATE_MEM_WRITE_CB:
    mem_addr_ = hl;
    mem_write_ = true;
    mem_out_ = (uint8_t)alu_out_;
    break;





  case Z80_STATE_DELAY_A: break;
  case Z80_STATE_DELAY_B: break;
  case Z80_STATE_DELAY_C: break;
  }

  //----------

  return {
    mem_addr_,
    mem_out_,
    mem_read_,
    mem_write_
  };
}

//-----------------------------------------------------------------------------

CpuOut Z80::tock_t2() {
  ime = ime_delay;

  if (state_ == Z80_STATE_DECODE) {

    // do final alu, etc.

    switch(state) {
    case Z80_STATE_DECODE:
    case Z80_STATE_DECODE_CB:    
    case Z80_STATE_HALT:
    {
      reg_in_ = reg_fetch(bus_data_);
      AluOut out = exec((uint8_t)reg_in_);
      alu_out_ = out.x;
      f_ = out.f;
      break;
    }
    case Z80_STATE_MEM_READ1:
    case Z80_STATE_MEM_READ2:
    case Z80_STATE_MEM_READ3:
    case Z80_STATE_MEM_READ_CB: {
      reg_in_ = bus_data_;
      AluOut out = exec((uint8_t)reg_in_);
      alu_out_ = out.x;
      f_ = out.f;
      break;
    }

    case Z80_STATE_MEM_WRITE1:
    case Z80_STATE_MEM_WRITE2:
    case Z80_STATE_MEM_WRITE_CB:
    {
      AluOut out = exec((uint8_t)reg_in_);
      alu_out_ = out.x;
      f_ = out.f;
      break;
    }

    case Z80_STATE_DELAY_A:
    case Z80_STATE_DELAY_B:
    case Z80_STATE_DELAY_C:
    {
      break;
    }
    }

    // Write all our registers from the previous instruction before the new opcode shows up.
    // Not idempotent yet
    pc = pc_;

    uint8_t mask = PREFIX_CB ? cb_flag_mask[cb_quad_] : flag_mask[op_];
    f = (f & ~mask) | (f_ & mask);

    if      (POP_AF)      f = data_lo_ & 0xF0;
    else if (ST_HLP_A)    reg_in_ = hl + 1;
    else if (LD_A_AT_HLP) reg_in_ = hl + 1;
    else if (ST_HLM_A)    reg_in_ = hl - 1;
    else if (LD_A_AT_HLM) reg_in_ = hl - 1;
    else if (MV_SP_HL)    reg_in_ = hl;
    else if (push_d16_)   reg_in_ = sp - 2;

    opcount = opcount + 1;

    if (PREFIX_CB) {
      switch (cb_col_) {
      case 0: b = (uint8_t)alu_out_; break;
      case 1: c = (uint8_t)alu_out_; break;
      case 2: d = (uint8_t)alu_out_; break;
      case 3: e = (uint8_t)alu_out_; break;
      case 4: h = (uint8_t)alu_out_; break;
      case 5: l = (uint8_t)alu_out_; break;
      case 6: break;
      case 7: a = (uint8_t)alu_out_; break;
      }
    }

    else if (INC_R || DEC_R) switch (row_) {
    case 0: b = (uint8_t)alu_out_; break;
    case 1: c = (uint8_t)alu_out_; break;
    case 2: d = (uint8_t)alu_out_; break;
    case 3: e = (uint8_t)alu_out_; break;
    case 4: h = (uint8_t)alu_out_; break;
    case 5: l = (uint8_t)alu_out_; break;
    case 6: break;
    case 7: a = (uint8_t)alu_out_; break;
    }

    else if (LD_R_D8 || MV_OPS) {
      switch (row_) {
      case 0: b = (uint8_t)reg_in_; break;
      case 1: c = (uint8_t)reg_in_; break;
      case 2: d = (uint8_t)reg_in_; break;
      case 3: e = (uint8_t)reg_in_; break;
      case 4: h = (uint8_t)reg_in_; break;
      case 5: l = (uint8_t)reg_in_; break;
      case 6: break;
      case 7: a = (uint8_t)reg_in_; break;
      }
    }

    else if (ALU_A_D8 || ALU_OPS || ROTATE_OPS) {
      a = (uint8_t)alu_out_;
    }

    else if (LD_A_AT_RR || LD_A_AT_A8 || LD_A_AT_C || LD_A_AT_A16) {
      a = data_lo_;
      if (LD_A_AT_HLP) hl++;
      if (LD_A_AT_HLM) hl--;
    }

    else if (LD_RR_D16) switch (row_ >> 1) {
    case 0: bc = data16_; break;
    case 1: de = data16_; break;
    case 2: hl = data16_; break;
    case 3: sp = data16_; break;
    }

    else if (INC_RR) switch (row_ >> 1) {
    case 0: bc++; break;
    case 1: de++; break;
    case 2: hl++; break;
    case 3: sp++; break;
    }

    else if (DEC_RR) switch (row_ >> 1) {
    case 0: bc--; break;
    case 1: de--; break;
    case 2: hl--; break;
    case 3: sp--; break;
    }

    else if (POP_RR) {
      switch (row_ >> 1) {
      case 0: bc = data16_; break;
      case 1: de = data16_; break;
      case 2: hl = data16_; break;
      case 3: af = data16_ & 0xFFF0; break;
      }
      sp = sp + 2;
    }

    else if (ADD_HL_RR)   hl = alu_out_;
    else if (LD_HL_SP_R8) hl = alu_out_;
    else if (ST_HLP_A)    hl++;
    else if (ST_HLM_A)    hl--;
    else if (ADD_SP_R8)   sp = alu_out_;
    else if (MV_SP_HL)    sp = hl;
    else if (push_d16_)   sp = sp - 2;
    else if ((RET_CC && take_branch_) || RET || RETI) sp = sp + 2;

    //----------
    // Update our interrupt master enable.

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

  if (state_ == Z80_STATE_MEM_WRITE2) {
    // Gameboy weirdness - the "real" interrupt vector is determined by the
    // state of imask/intf at the end of the second write cycle.
    imask_latch = imask_;
  }

  state = state_;
  reg_in = reg_in_;
  bus_tag = bus_tag_;
  data_lo = data_lo_;
  data_hi = data_hi_;
  mem_addr = mem_addr_;

  cycle++;

  return { 0 };
}

//-----------------------------------
// New opcode arrived, decode it and dispatch next state.

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

  //----------
  // take_branch

  take_branch_ |= CALL_A16 || JP_A16 || RET || RETI || JP_HL || RST_NN || JR_R8;
  take_branch_ |= (JR_CC_R8 || RET_CC || JP_CC_A16 || CALL_CC_A16) && cond_pass;

  //----------
  // get/put hl

  get_hl_ |= INC_AT_HL || DEC_AT_HL || LD_A_AT_HLP || LD_A_AT_HLM;
  get_hl_ |= (col_ == 6);

  put_hl_ = INC_AT_HL || DEC_AT_HL || ST_HL_D8 || ST_HLP_A || ST_HLM_A;
  put_hl_ |= (quad_ == 1) && (row_ == 6);

  //----------
  // push

  push_d16_ |= (quad_ == 3) && ((col_ == 5) || (col_ == 7));
  push_d16_ |= (take_branch_ && CALL_CC_A16);

  //----------
  // pop

  pop_d16_ |= RET || RETI;
  pop_d16_ |= (quad_ == 3) && (col_ == 1 && !odd_row_);
  pop_d16_ |= (take_branch_ && RET_CC);

  //----------
  // fetch d8/d16

  fetch_d8_ |= (quad_ == 0) && (col_ == 6);
  fetch_d8_ |= (quad_ == 0) && (col_ == 0 && row_ >= 3);
  fetch_d8_ |= LD_A_AT_A8 || LD_HL_SP_R8 || ST_A8_A || ALU_A_D8 || ADD_SP_R8;

  //----------
  // fetch d16

  fetch_d16_ |= (quad_ == 0) && (col_ == 1 && !odd_row_);
  fetch_d16_ |= (quad_ == 3) && (col_ == 2 && row_ <= 3);
  fetch_d16_ |= (quad_ == 3) && (col_ == 4);
  fetch_d16_ |= ST_A16_SP || CALL_A16 || JP_A16 || ST_A16_A || LD_A_AT_A16;

  //----------
  // any_read_

  any_read_ |= fetch_d8_ || fetch_d16_ || get_hl_ || pop_d16_;
  any_read_ |= LD_A_AT_BC || LD_A_AT_DE || LD_A_AT_C;

  //----------
  // any_write_

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

uint16_t Z80::reg_fetch(uint8_t bus) const {
  if (PREFIX_CB) {
    uint8_t x = 0;
    switch(cb_col_) {
    case 0: x = b; break;
    case 1: x = c; break;
    case 2: x = d; break;
    case 3: x = e; break;
    case 4: x = h; break;
    case 5: x = l; break;
    case 6: x = bus; break;
    case 7: x = a; break;
    }
    return x;
  }
  
  if (ROTATE_OPS) {
    return a;
  }
  
  if (ADD_HL_RR || INC_RR || DEC_RR) switch(row_ / 2) {
    case 0: return bc; break;
    case 1: return de; break;
    case 2: return hl; break;
    case 3: return sp; break;
  }
  
  if (PUSH_RR || POP_RR) switch(row_ / 2) {
    case 0: return bc; break;
    case 1: return de; break;
    case 2: return hl; break;
    case 3: return af; break;
  }
  
  
  switch (quad_ == 0 ? row_ : col_) {
    case 0: return b; break;
    case 1: return c; break;
    case 2: return d; break;
    case 3: return e; break;
    case 4: return h; break;
    case 5: return l; break;
    case 6: return 0; break;
    case 7: return a; break;
  }

  return 0;
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
    bool halfcarry = (reg_in_ & 0x0FFF) + (hl & 0x0FFF) > 0x0FFF;
    bool carry =     (reg_in_ & 0xFFFF) + (hl & 0xFFFF) > 0xFFFF;

    out.x = reg_in_ + hl;
    out.f = (halfcarry ? F_HALF_CARRY : 0) | (carry ? F_CARRY : 0);
  }
  else if (ADD_SP_R8 || LD_HL_SP_R8) {
    bool halfcarry = (sp & 0x000F) + (bus_data_ & 0x000F) > 0x000F;
    bool carry =     (sp & 0x00FF) + (bus_data_ & 0x00FF) > 0x00FF;

    out.x = sp + (int8_t)bus_data_;
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
