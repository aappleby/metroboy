#include "Z80.h"

#include "Constants.h"

#include <assert.h>

//-----------------------------------------------------------------------------

#define OP_QUAD       (op >> 6)
#define OP_ROW        ((op >> 3) & 7)
#define OP_ODD_ROW    ((op >> 3) & 1)
#define OP_COL        ((op >> 0) & 7)

#define CB_QUAD       (op_cb >> 6)
#define CB_ROW        ((op_cb >> 3) & 7)
#define CB_COL        ((op_cb >> 0) & 7)

#define NOP           (op == 0x00)

#define ST_BC_A       (op == 0x02)
#define ST_DE_A       (op == 0x12)
#define ST_HLP_A      (op == 0x22)
#define ST_HLM_A      (op == 0x32)

#define ST_A8_A       (op == 0xE0)
#define ST_C_A        (op == 0xE2)
#define ST_A16_SP     (op == 0x08)
#define ST_A16_A      (op == 0xEA)
#define ST_HL_D8      (op == 0x36)

#define ADD_SP_R8     (op == 0xE8)
#define LD_HL_SP_R8   (op == 0xF8)
#define INC_AT_HL     (op == 0x34)
#define DEC_AT_HL     (op == 0x35)
#define LD_A_AT_A16   (op == 0xFA)
#define LD_A_AT_C     (op == 0xF2)
#define LD_A_AT_A8    (op == 0xF0)
#define LD_A_AT_BC    (op == 0x0A)
#define LD_A_AT_DE    (op == 0x1A)
#define LD_A_AT_HLP   (op == 0x2A)
#define LD_A_AT_HLM   (op == 0x3A)

#define JP_A16        (op == 0xC3)
#define JR_R8         (op == 0x18)
#define JP_HL         (op == 0xE9)
#define CALL_A16      (op == 0xCD)
#define RET           (op == 0xC9)
#define RETI          (op == 0xD9)
#define POP_AF        (op == 0xF1)
#define DI            (op == 0xF3)
#define EI            (op == 0xFB)
#define HALT          (op == 0x76)
#define PREFIX_CB     (op == 0xCB)
#define MV_SP_HL      (op == 0xF9)

#define JR_CC_R8      (OP_QUAD == 0 && OP_COL == 0 && OP_ROW >= 4)
#define LD_RR_D16     (OP_QUAD == 0 && OP_COL == 1 && !OP_ODD_ROW)
#define ADD_HL_RR     (OP_QUAD == 0 && OP_COL == 1 &&  OP_ODD_ROW)
#define LD_A_AT_RR    (OP_QUAD == 0 && OP_COL == 2 &&  OP_ODD_ROW)
#define INC_RR        (OP_QUAD == 0 && OP_COL == 3 && !OP_ODD_ROW)
#define DEC_RR        (OP_QUAD == 0 && OP_COL == 3 &&  OP_ODD_ROW)
#define INC_R         (OP_QUAD == 0 && OP_COL == 4)
#define DEC_R         (OP_QUAD == 0 && OP_COL == 5)
#define LD_R_D8       (OP_QUAD == 0 && OP_COL == 6)
#define ROTATE_OPS    (OP_QUAD == 0 && OP_COL == 7)

#define MV_OPS        (OP_QUAD == 1)
#define MV_OPS_ST_HL  (OP_QUAD == 1 && OP_ROW == 6)
#define MV_OPS_LD_HL  (OP_QUAD == 1 && OP_COL == 6)

#define ALU_OPS       (OP_QUAD == 2)
#define ALU_OPS_LD_HL (OP_QUAD == 2 && OP_COL == 6)

#define RET_CC        (OP_QUAD == 3 && OP_COL == 0 && OP_ROW <= 3)
#define POP_RR        (OP_QUAD == 3 && OP_COL == 1 && !OP_ODD_ROW)
#define JP_CC_A16     (OP_QUAD == 3 && OP_COL == 2 && OP_ROW <= 3)
#define CALL_CC_A16   (OP_QUAD == 3 && OP_COL == 4 && OP_ROW <= 3)
#define PUSH_RR       (OP_QUAD == 3 && OP_COL == 5 && !OP_ODD_ROW)
#define ALU_A_D8      (OP_QUAD == 3 && OP_COL == 6)
#define RST_NN        (OP_QUAD == 3 && OP_COL == 7)

#define INTERRUPT     ((imask_ & intf_) && ime)

AluOut cb(const uint8_t quad, const uint8_t row, const uint8_t x, const uint8_t f);

//-----------------------------------------------------------------------------

void Z80::reset(int new_model, uint16_t new_pc) {
  int_ack_ = 0;
  imask_ = 0;
  intf_ = 0;
  unhalt = 0;

  model = new_model;
  cycle = 0;
  op = 0;
  op_cb = 0;
  ime = false;
  ime_delay = false;
  imask_latch = 0;
  interrupt = false;
  state = Z80_STATE_DECODE;

  if (new_pc == 0x100) {
    af = 0x01B0;
    bc = 0x0013;
    de = 0x00D8;
    hl = 0x014D;
    sp = 0xFFFE;
    pc = new_pc;
    addr = 0;
  }
  else {
    af = 0x0000;
    bc = 0x0000;
    de = 0x0000;
    hl = 0x0000;
    sp = 0x0000;
    pc = new_pc;
    addr = 0;
  }
}

//-----------------------------------------------------------------------------

CpuBus Z80::tick_t0(uint8_t imask, uint8_t intf, uint8_t bus_data) {
  int_ack_ = 0;
  imask_ = imask;
  intf_ = intf;

  switch(state) {
  case Z80_STATE_DECODE:    op = bus_data; break;
  case Z80_STATE_DECODE_CB: op_cb = bus_data; break;
  case Z80_STATE_POP1:      lo = bus_data; break;
  case Z80_STATE_POP2:      hi = bus_data; break;
  case Z80_STATE_ARG1:      lo = bus_data; break;
  case Z80_STATE_ARG2:      hi = bus_data; break;
  case Z80_STATE_MEM_READ1: lo = bus_data; break;
  }

  //----------------------------------------

  bool cond_fail = false;

  switch (OP_ROW & 3) {
  case 0: cond_fail = (f & F_ZERO); break;
  case 1: cond_fail = !(f & F_ZERO); break;
  case 2: cond_fail = (f & F_CARRY); break;
  case 3: cond_fail = !(f & F_CARRY); break;
  }

  no_branch = (JR_CC_R8 || RET_CC || JP_CC_A16 || CALL_CC_A16) && cond_fail;
  no_halt = ((imask_ & intf_) && !ime);

  if (state == Z80_STATE_DECODE) {
    interrupt = (imask_ & intf_) && ime;
    if (interrupt) op = 0x00;
  }

  //----------------------------------------

  CpuBus bus = {0, 0, false, false};

  state_ = Z80_STATE_DECODE;

  switch (state) {
  case Z80_STATE_DECODE: {
    state_ = Z80_STATE_DECODE;
    if      (interrupt)     state_ = Z80_STATE_INTERRUPT;
    else if (HALT)          state_ = no_halt ? Z80_STATE_DECODE : Z80_STATE_HALT;
    else if (MV_OPS_ST_HL)  state_ = Z80_STATE_MEM_WRITE1;
    else if (ST_HLP_A)      state_ = Z80_STATE_MEM_WRITE1;
    else if (ST_HLM_A)      state_ = Z80_STATE_MEM_WRITE1;
    else if (ST_C_A)        state_ = Z80_STATE_MEM_WRITE1;
    else if (ST_BC_A)       state_ = Z80_STATE_MEM_WRITE1;
    else if (ST_DE_A)       state_ = Z80_STATE_MEM_WRITE1;

    else if (INC_AT_HL)     state_ = Z80_STATE_MEM_READ1;
    else if (DEC_AT_HL)     state_ = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_HLP)   state_ = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_HLM)   state_ = Z80_STATE_MEM_READ1;
    else if (MV_OPS_LD_HL)  state_ = Z80_STATE_MEM_READ1;
    else if (ALU_OPS_LD_HL) state_ = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_BC)    state_ = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_DE)    state_ = Z80_STATE_MEM_READ1;
    else if (LD_A_AT_C)     state_ = Z80_STATE_MEM_READ1;

    else if (RET_CC)        state_ = Z80_STATE_RET_DELAY;
    else if (RST_NN)        state_ = Z80_STATE_PUSH_DELAY;

    else if (INC_RR)        state_ = Z80_STATE_DELAY_C;
    else if (DEC_RR)        state_ = Z80_STATE_DELAY_C;
    else if (ADD_HL_RR)     state_ = Z80_STATE_DELAY_C;
    else if (MV_SP_HL)      state_ = Z80_STATE_DELAY_C;

    else if (PREFIX_CB)     state_ = Z80_STATE_DECODE_CB;

    else if (PUSH_RR)       state_ = Z80_STATE_PUSH_DELAY;

    else if (RET)           state_ = Z80_STATE_POP1;
    else if (RETI)          state_ = Z80_STATE_POP1;
    else if (POP_RR)        state_ = Z80_STATE_POP1;

    else if (LD_R_D8)       state_ = Z80_STATE_ARG1;
    else if (JR_CC_R8)      state_ = Z80_STATE_ARG1;
    else if (JR_R8)         state_ = Z80_STATE_ARG1;
    else if (LD_A_AT_A8)    state_ = Z80_STATE_ARG1;
    else if (LD_HL_SP_R8)   state_ = Z80_STATE_ARG1;
    else if (ST_A8_A)       state_ = Z80_STATE_ARG1;
    else if (ALU_A_D8)      state_ = Z80_STATE_ARG1;
    else if (ADD_SP_R8)     state_ = Z80_STATE_ARG1;
    else if (LD_A_AT_A16)   state_ = Z80_STATE_ARG1;
    else if (LD_RR_D16)     state_ = Z80_STATE_ARG1;
    else if (ST_A16_A)      state_ = Z80_STATE_ARG1;
    else if (ST_A16_SP)     state_ = Z80_STATE_ARG1;
    else if (JP_A16)        state_ = Z80_STATE_ARG1;
    else if (JP_CC_A16)     state_ = Z80_STATE_ARG1;
    else if (CALL_A16)      state_ = Z80_STATE_ARG1;
    else if (CALL_CC_A16)   state_ = Z80_STATE_ARG1;

    break;
  }

  case Z80_STATE_DECODE_CB:
    state_ = CB_COL == 6 ? Z80_STATE_MEM_READ1 : Z80_STATE_DECODE;
    break;

  case Z80_STATE_HALT:
    state_ = unhalt ? Z80_STATE_DECODE : Z80_STATE_HALT;
    break;

  case Z80_STATE_INTERRUPT:
    state_ = Z80_STATE_PUSH_DELAY;
    break;

  case Z80_STATE_PUSH_DELAY:
    state_ = Z80_STATE_PUSH1;
    break;

  case Z80_STATE_PUSH1:
    state_ = Z80_STATE_PUSH2;
    break;

  case Z80_STATE_PUSH2:
    state_ = Z80_STATE_DECODE;
    break;

  case Z80_STATE_POP1:
    state_ = Z80_STATE_POP2;
    break;

  case Z80_STATE_POP2:
    state_ = POP_RR ? Z80_STATE_DECODE : Z80_STATE_DELAY_C;
    break;

  case Z80_STATE_ARG1:
    state_ = Z80_STATE_ARG2;
    if      (LD_A_AT_A8)    state_ = Z80_STATE_MEM_READ1;
    else if (ST_HL_D8)      state_ = Z80_STATE_MEM_WRITE1;
    else if (ST_A8_A)       state_ = Z80_STATE_MEM_WRITE1;
    else if (ADD_SP_R8)     state_ = Z80_STATE_DELAY_B;

    // second alu op delay
    else if (LD_HL_SP_R8)   state_ = Z80_STATE_DELAY_C;
    else if (JR_R8)         state_ = Z80_STATE_DELAY_C;
    else if (JR_CC_R8)      state_ = Z80_STATE_DELAY_C;

    else if (LD_R_D8)       state_ = Z80_STATE_DECODE;
    else if (ALU_A_D8)      state_ = Z80_STATE_DECODE;

    if (JR_CC_R8 && no_branch) state_ = Z80_STATE_DECODE;
    break;

  case Z80_STATE_ARG2:
    if      (LD_A_AT_A16)   state_ = Z80_STATE_MEM_READ1;
    else if (ST_A16_A)      state_ = Z80_STATE_MEM_WRITE1;
    else if (ST_A16_SP)     state_ = Z80_STATE_MEM_WRITE1;
    else if (CALL_A16)      state_ = Z80_STATE_PUSH_DELAY;
    else if (JP_A16)        state_ = Z80_STATE_DELAY_C;
    else if (JP_CC_A16)     state_ = Z80_STATE_DELAY_C;
    else if (LD_RR_D16)     state_ = Z80_STATE_DECODE;
    else if (CALL_CC_A16)   state_ = Z80_STATE_PUSH_DELAY;
    if (no_branch) state_ = Z80_STATE_DECODE;
    break;

  case Z80_STATE_MEM_READ1:
    state_ = Z80_STATE_DECODE;
    if      (INC_AT_HL)     state_ = Z80_STATE_MEM_WRITE1;
    else if (DEC_AT_HL)     state_ = Z80_STATE_MEM_WRITE1;
    else if (ST_HLP_A)      state_ = Z80_STATE_MEM_WRITE1;
    else if (ST_HLM_A)      state_ = Z80_STATE_MEM_WRITE1;
    else if (PREFIX_CB)     state_ = Z80_STATE_MEM_WRITE1;

    break;

  case Z80_STATE_MEM_WRITE1:
    state_ = Z80_STATE_DECODE;
    if (ST_A16_SP) state_ = Z80_STATE_MEM_WRITE2;
    break;

  case Z80_STATE_MEM_WRITE2:
    state_ = Z80_STATE_DECODE;
    break;

  case Z80_STATE_RET_DELAY:
    state_ = Z80_STATE_POP1;
    if (no_branch) state_ = Z80_STATE_DECODE;
    break;

  case Z80_STATE_DELAY_B:
    state_ = Z80_STATE_DELAY_C;
    break;

  case Z80_STATE_DELAY_C:
    state_ = Z80_STATE_DECODE;
    break;
  }

  //----------------------------------------

  if (state == Z80_STATE_DECODE) pc++;
  if (state == Z80_STATE_DECODE_CB) pc++;

  switch(state_) {

  case Z80_STATE_PUSH1:
    addr = sp;
    if      (interrupt)  data_out = (uint8_t)(temp >> 8);
    else if (PUSH_RR) {
      switch(OP_ROW >> 1) {
      case 0: data_out = b; break;
      case 1: data_out = d; break;
      case 2: data_out = h; break;
      case 3: data_out = a; break;
      }
    }
    else if (CALL_A16)    data_out = (uint8_t)(pc >> 8);
    else if (CALL_CC_A16) data_out = (uint8_t)(pc >> 8);
    else if (RST_NN)      data_out = (uint8_t)(pc >> 8);


    bus.addr = addr;
    bus.data = data_out;
    bus.write = true;
    break;

  case Z80_STATE_PUSH2:
    addr = sp;
    if      (interrupt)  data_out = (uint8_t)(temp);
    if (PUSH_RR) {
      switch(OP_ROW >> 1) {
      case 0: data_out = c; break;
      case 1: data_out = e; break;
      case 2: data_out = l; break;
      case 3: data_out = f; break;
      }
    }
    if (CALL_A16)    data_out = (uint8_t)(pc);
    if (CALL_CC_A16) data_out = (uint8_t)(pc);
    if (RST_NN)      data_out = (uint8_t)(pc);

    bus.addr = addr;
    bus.data = data_out;
    bus.write = true;
    break;

  case Z80_STATE_MEM_WRITE1:
    if      (ST_BC_A)       { addr = bc;          data_out = a; }
    else if (ST_DE_A)       { addr = de;          data_out = a; }
    else if (ST_HLP_A)      { addr = hl;          data_out = a; }
    else if (ST_HLM_A)      { addr = hl;          data_out = a; }
    else if (INC_AT_HL)     { addr = hl;          data_out = lo + 1; }
    else if (DEC_AT_HL)     { addr = hl;          data_out = lo - 1; }
    else if (ST_HL_D8)      { addr = hl;          data_out = lo; }
    else if (MV_OPS_ST_HL)  { addr = hl;          data_out = reg_fetch8(); }
    else if (PREFIX_CB)     { addr = hl;          data_out = (uint8_t)cb(CB_QUAD, CB_ROW, reg_fetch8(), f).x; }
    else if (ST_A16_A)      { addr = temp;        data_out = a; }
    else if (ST_A8_A)       { addr = 0xFF00 | lo; data_out = a; }
    else if (ST_C_A)        { addr = 0xFF00 | c;  data_out = a; }
    else if (ST_A16_SP)     { addr = temp;        data_out = (uint8_t)sp; }

    bus.addr = addr;
    bus.data = data_out;
    bus.write = true;
    break;

  case Z80_STATE_MEM_WRITE2:
    addr = temp + 1;
    data_out = (uint8_t)(sp >> 8);

    bus.addr = addr;
    bus.data = data_out;
    bus.write = true;
    break;
  }

  //----------------------------------------

  return bus;
}

//-----------------------------------------------------------------------------

void Z80::tock_t0(uint8_t imask, uint8_t intf, uint8_t bus_data) {
  (void)imask;
  (void)intf;
  (void)bus_data;

  //----------------------------------------

  if (state_ == Z80_STATE_DECODE) {
    int next_int = -1;
    if (interrupt) {
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

      next_int = actual_interrupt;
    }

    if (next_int >= 0) int_ack_ = 1 << next_int;

    if (interrupt) {
      if (next_int >= 0) {
        pc = uint16_t(0x0040 + (next_int * 8));
      }
      else {
        pc = 0x0000;
      }
    }

    if (!no_branch) {
      if      (JP_HL)       pc = hl;
      else if (RST_NN)      pc = op - 0xC7;
      else if (JR_R8)       pc = pc + (int8_t)lo;
      else if (JR_CC_R8)    pc = pc + (int8_t)lo;
      else if (JP_CC_A16)   pc = temp;
      else if (CALL_CC_A16) pc = temp;
      else if (RET_CC)      pc = temp;
      else if (RET)         pc = temp;
      else if (RETI)        pc = temp;
      else if (JP_A16)      pc = temp;
      else if (CALL_A16)    pc = temp;
    }

    AluOut out = exec(reg_fetch8());

    uint8_t mask = PREFIX_CB ? cb_flag_mask[CB_QUAD] : flag_mask[op];
    if (POP_AF)  f = lo & 0xF0;
    else         f = (f & ~mask) | (out.f & mask);

    if (MV_OPS)      reg_put8(OP_ROW, (uint8_t)reg_fetch8());
    if (LD_R_D8)     reg_put8(OP_ROW, (uint8_t)temp);

    if (INC_R)       reg_put8(OP_ROW, (uint8_t)out.x);
    if (DEC_R)       reg_put8(OP_ROW, (uint8_t)out.x);
    if (ALU_A_D8)    reg_put8(7,      (uint8_t)out.x);
    if (ALU_OPS)     reg_put8(7,      (uint8_t)out.x);
    if (ROTATE_OPS)  reg_put8(7,      (uint8_t)out.x);
    if (ADD_HL_RR)   hl = out.x;
    if (LD_HL_SP_R8) hl = out.x;
    if (ADD_SP_R8)   sp = out.x;

    if (LD_A_AT_RR)  reg_put8(7,      (uint8_t)temp);
    if (LD_A_AT_A8)  reg_put8(7,      (uint8_t)temp);
    if (LD_A_AT_C)   reg_put8(7,      (uint8_t)temp);
    if (LD_A_AT_A16) reg_put8(7,      (uint8_t)temp);

    if (PREFIX_CB)   {
      reg_put8(CB_COL, (uint8_t)out.x);
    }

    if (ST_HLP_A)    hl = hl + 1;
    if (ST_HLM_A)    hl = hl - 1;
    if (LD_A_AT_HLP) hl = hl + 1;
    if (LD_A_AT_HLM) hl = hl - 1;
    if (MV_SP_HL)    sp = hl;
  }

  //----------------------------------------

  switch (state) {
  case Z80_STATE_DECODE:
    break;

  case Z80_STATE_PUSH1:
    // Gameboy weirdness - the "real" interrupt vector is determined by the
    // state of imask/intf after pushing the first byte of PC onto the stack.
    imask_latch = imask_;
    break;

  case Z80_STATE_ARG1:
    pc++;
    break;

  case Z80_STATE_ARG2:
    if (LD_RR_D16) {
      switch(OP_ROW >> 1) {
      case 0: bc = temp; break;
      case 1: de = temp; break;
      case 2: hl = temp; break;
      case 3: sp = temp; break;
      }
    }
    pc++;
    break;

  case Z80_STATE_POP2:
    if (POP_RR)      {
      switch(OP_ROW >> 1) {
      case 0: bc = temp; break;
      case 1: de = temp; break;
      case 2: hl = temp; break;
      case 3: af = temp & 0xFFF0; break;
      }
    }
    break;

  case Z80_STATE_DELAY_C:
    if (INC_RR) {
      switch(OP_ROW >> 1) {
      case 0: bc++; break;
      case 1: de++; break;
      case 2: hl++; break;
      case 3: sp++; break;
      }
    }
    if (DEC_RR) {
      switch(OP_ROW >> 1) {
      case 0: bc--; break;
      case 1: de--; break;
      case 2: hl--; break;
      case 3: sp--; break;
      }
    }
    break;
  }

  //----------------------------------------

  switch(state_) {
  case Z80_STATE_DECODE:
  case Z80_STATE_DECODE_CB:
  case Z80_STATE_HALT:
  case Z80_STATE_ARG1:
  case Z80_STATE_ARG2:
    addr = pc;
    break;

  case Z80_STATE_PUSH_DELAY:
  case Z80_STATE_PUSH1:
    sp--;
    break;

  case Z80_STATE_POP1:
  case Z80_STATE_POP2:
    addr = sp;
    sp++;
    break;

  case Z80_STATE_MEM_READ1:
    if      (LD_A_AT_A16)   { addr = temp; }
    else if (LD_A_AT_A8)    { addr = 0xFF00 | lo; }
    else if (LD_A_AT_C)     { addr = 0xFF00 | c; }
    else if (LD_A_AT_BC)    { addr = bc; }
    else if (LD_A_AT_DE)    { addr = de; }
    else if (INC_AT_HL)     { addr = hl; }
    else if (DEC_AT_HL)     { addr = hl; }
    else if (LD_A_AT_HLP)   { addr = hl; }
    else if (LD_A_AT_HLM)   { addr = hl; }
    else if (MV_OPS_LD_HL)  { addr = hl; }
    else if (ALU_OPS_LD_HL) { addr = hl; }
    else if (PREFIX_CB)     { addr = hl; }
    break;
  }
}

//-----------------------------------------------------------------------------

CpuBus Z80::tick_t2(uint8_t imask, uint8_t intf, uint8_t bus_data) {
  (void)imask;
  (void)intf;
  (void)bus_data;

  //----------------------------------------

  switch (state) {
  case Z80_STATE_DECODE: break;
  case Z80_STATE_DECODE_CB: break;
  case Z80_STATE_HALT: break;
  case Z80_STATE_INTERRUPT: break;
  case Z80_STATE_PUSH_DELAY: break;
  case Z80_STATE_PUSH1: break;
  case Z80_STATE_PUSH2: break;
  case Z80_STATE_POP1: break;
  case Z80_STATE_POP2: break;
  case Z80_STATE_ARG1: break;
  case Z80_STATE_ARG2: break;
  case Z80_STATE_MEM_READ1: break;
  case Z80_STATE_MEM_WRITE1: break;
  case Z80_STATE_MEM_WRITE2: break;
  case Z80_STATE_RET_DELAY: break;
  case Z80_STATE_DELAY_B: break;
  case Z80_STATE_DELAY_C: break;
  }

  //----------------------------------------

  switch(state_) {
  case Z80_STATE_DECODE: break;
  case Z80_STATE_DECODE_CB: break;
  case Z80_STATE_HALT: break;
  case Z80_STATE_ARG1: break;
  case Z80_STATE_ARG2: break;
  case Z80_STATE_POP1: break;
  case Z80_STATE_POP2: break;
  case Z80_STATE_MEM_READ1: break;
  case Z80_STATE_INTERRUPT: break;
  case Z80_STATE_PUSH_DELAY: break;
  case Z80_STATE_PUSH1: break;
  case Z80_STATE_PUSH2: break;
  case Z80_STATE_MEM_WRITE1: break;
  case Z80_STATE_MEM_WRITE2: break;
  case Z80_STATE_RET_DELAY: break;
  case Z80_STATE_DELAY_B: break;
  case Z80_STATE_DELAY_C: break;
  }

  CpuBus bus = { addr, 0, true, false };
  return bus;
}

//-----------------------------------------------------------------------------

void Z80::tock_t2(uint8_t imask, uint8_t intf, uint8_t bus_data) {
  (void)imask;
  (void)intf;
  (void)bus_data;

  ime = ime_delay;

  //----------------------------------------

  if (state_ == Z80_STATE_DECODE) {
    // When we finish an instruction, update our interrupt master enable.
    if (interrupt)  { ime = false;     ime_delay = false; }
    else if (RETI)  { ime = true;      ime_delay = true; }
    else if (DI)    { ime = false;     ime_delay = false; }
    else if (EI)    { ime = ime_delay; ime_delay = true; }
  }

  //----------

  if (state == Z80_STATE_DECODE && state_ == Z80_STATE_HALT) unhalt = 0;

  state = state_;
  cycle++;
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

uint8_t Z80::reg_fetch8() const {
  int mux = OP_QUAD == 0 ? OP_ROW : OP_COL;
  if (PREFIX_CB) mux = CB_COL;
  if (ROTATE_OPS) mux = OP_COL;

  switch(mux) {
  case 0: return b;
  case 1: return c;
  case 2: return d;
  case 3: return e;
  case 4: return h;
  case 5: return l;
  case 6: return lo;
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
    out = cb(CB_QUAD, CB_ROW, src, f);
  }
  else if (INC_R) {
    out = alu(0, src, 1, 0);
  }
  else if (DEC_R) {
    out = alu(2, src, 1, 0);
  }
  else if (ADD_HL_RR) {
    uint16_t blah = 0;
    switch(OP_ROW >> 1) {
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
    bool halfcarry = (sp & 0x000F) + (lo & 0x000F) > 0x000F;
    bool carry =     (sp & 0x00FF) + (lo & 0x00FF) > 0x00FF;

    out.x = sp + (int8_t)lo;
    out.f = (halfcarry ? F_HALF_CARRY : 0) | (carry ? F_CARRY : 0);
  }
  else if (ROTATE_OPS) {
    out = rlu(OP_ROW, src, f);
    if (OP_ROW <= 3) out.f &= ~F_ZERO;
  }
  else if (ALU_OPS || ALU_A_D8) {
    out = alu(OP_ROW, a, src, f);
    out.x = (OP_ROW == 7) ? a : out.x;
  }

  return out;
}

//-----------------------------------------------------------------------------

void Z80::dump(std::string& out) {
  sprintf(out, "CYC %d\n", cycle);
  int bgb = (cycle * 2) + 0x00B2D5E6;
  sprintf(out, "BGB 0x%08x\n", bgb);
  sprintf(out, "op 0x%02x\n", op);
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
