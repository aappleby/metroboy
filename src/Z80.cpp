#include "Z80.h"

#include "Constants.h"

#include <assert.h>

//-----------------------------------------------------------------------------

#define OP_ROW        ((op >> 3) & 7)
#define OP_COL        (op & 0x07)

#define NOP           (op == 0x00)

#define STM_BC_A      (op == 0x02)
#define STM_DE_A      (op == 0x12)
#define STM_HLP_A     (op == 0x22)
#define STM_HLM_A     (op == 0x32)
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

#define LD_BC_D16     (op == 0x01)
#define LD_DE_D16     (op == 0x11)
#define LD_HL_D16     (op == 0x21)
#define LD_SP_D16     (op == 0x31)
#define LD_RR_D16     ((op & 0b11001111) == 0b00000001)

#define ADD_HL_BC     (op == 0x09)
#define ADD_HL_DE     (op == 0x19)
#define ADD_HL_HL     (op == 0x29)
#define ADD_HL_SP     (op == 0x39)
#define ADD_HL_RR     ((op & 0b11001111) == 0b00001001)

#define LDM_A_BC      (op == 0x0A)
#define LDM_A_DE      (op == 0x1A)
#define LDM_A_HLP     (op == 0x2A)
#define LDM_A_HLM     (op == 0x3A)
#define LDM_A_RR      ((op & 0b11001111) == 0b00001010)

#define INC_BC        (op == 0b00000011)
#define INC_DE        (op == 0b00010011)
#define INC_HL        (op == 0b00100011)
#define INC_SP        (op == 0b00110011)
#define INC_RR        ((op & 0b11001111) == 0b00000011)

#define DEC_BC        (op == 0x0B)
#define DEC_DE        (op == 0x1B)
#define DEC_HL        (op == 0x2B)
#define DEC_SP        (op == 0x3B)
#define DEC_RR        ((op & 0b11001111) == 0b00001011)

#define INC_AT_HL     (op == 0x34)
#define DEC_AT_HL     (op == 0x35)
#define INC_R         ((op & 0b11000111) == 0b00000100 && !INC_AT_HL)
#define DEC_R         ((op & 0b11000111) == 0b00000101 && !DEC_AT_HL)

#define STM_HL_D8     (op == 0x36)
#define LD_R_D8       ((op & 0b11000111) == 0b00000110 && !STM_HL_D8)

#define RLU_R         ((op & 0b11000111) == 0b00000111)

#define LDM_R_HL      ((op & 0b11000111) == 0b01000110 && !HALT)
#define STM_HL_R      ((op & 0b11111000) == 0b01110000 && !HALT)
#define MV_R_R        ((op & 0b11000000) == 0b01000000 && (op & 0b00000111) != 0b00000110 && (op & 0b00111000) != 0b00110000)

#define ALU_A_HL      ((op & 0b11000111) == 0b10000110)
#define ALU_A_R       ((op & 0b11000000) == 0b10000000 && (op & 0b11000111) != 0b10000110)

#define JR_NZ_R8      (op == 0b00100000)
#define JR_Z_R8       (op == 0b00101000)
#define JR_NC_R8      (op == 0b00110000)
#define JR_C_R8       (op == 0b00111000)
#define JR_CC_R8      ((op & 0b11100111) == 0b00100000)

#define RET_NZ        (op == 0b11000000)
#define RET_Z         (op == 0b11001000)
#define RET_NC        (op == 0b11010000)
#define RET_C         (op == 0b11011000)
#define RET_CC        ((op & 0b11100111) == 0b11000000)

#define JP_NZ_A16     (op == 0b11000010)
#define JP_Z_A16      (op == 0b11001010)
#define JP_NC_A16     (op == 0b11010010)
#define JP_C_A16      (op == 0b11011010)
#define JP_CC_A16     ((op & 0b11100111) == 0b11000010)

#define CALL_NZ_A16   (op == 0b11000100)
#define CALL_Z_A16    (op == 0b11001101)
#define CALL_NC_A16   (op == 0b11010100)
#define CALL_C_A16    (op == 0b11011101)
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

#define ALU_A_D8      ((op & 0b11000111) == 0b11000110)
#define RST_NN        ((op & 0b11000111) == 0b11000111)

#define PREFIX_CB     (op == 0xCB)
#define CB_QUAD       ((cb >> 6) & 3)
#define CB_ROW        ((cb >> 3) & 7)
#define CB_COL        ((cb >> 0) & 7)
#define OP_CB_R       (PREFIX_CB && CB_COL != 6)
#define OP_CB_HL      (PREFIX_CB && CB_COL == 6)

#define INTERRUPT     ((imask_ & intf_) && ime)

AluOut alu_cb(const uint8_t quad, const uint8_t row, const uint8_t x, const uint8_t f);
AluOut alu(const uint8_t op, const uint8_t x, const uint8_t y, const uint8_t f);
AluOut rlu(const uint8_t op, const uint8_t x, const uint8_t f);

//-----------------------------------------------------------------------------

void Z80::reset(int new_model, uint16_t new_pc) {
  int_ack_ = 0;
  imask_ = 0;
  intf_ = 0;
  unhalt = 0;

  model = new_model;
  cycle = 0;
  op = 0;
  cb = 0;
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
// TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0


CpuBus Z80::tick_t0() const {
  bool write = (state == Z80_STATE_MEM_WRITE1 ||
                state == Z80_STATE_MEM_WRITE2 ||
                state == Z80_STATE_PUSH1 ||
                state == Z80_STATE_PUSH2);
  return { addr, data_out, false, write };
}








AluOut add(uint16_t x, uint16_t y, uint8_t f) {
  uint16_t cr = ((f & F_CARRY) ? 1 : 0);
  uint16_t zh = (x + y + cr);
  uint16_t hc = ((x & 0xF) + (y & 0xF) + cr) >> 4;
  
  return {
    zh,
    uint8_t((hc ? F_HALF_CARRY : 0) | ((zh >> 8) ? F_CARRY : 0) | (zh & 0xFF ? 0 : F_ZERO))
  };
}



//-----------------------------------------------------------------------------
// TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0

void Z80::tock_t0(uint8_t imask, uint8_t intf, uint8_t bus_data) {

  //----------------------------------------
  // Handle read

  switch(state) {
  case Z80_STATE_DECODE:    op = bus_data; break;
  case Z80_STATE_CB1:       cb = bus_data; break;
  
  case Z80_STATE_POP1:      lo = bus_data; break;
  case Z80_STATE_POP2:      hi = bus_data; break;

  case Z80_STATE_ARG1:      lo = bus_data; break;
  case Z80_STATE_ARG2:      hi = bus_data; break;
  case Z80_STATE_MEM_READ1: lo = bus_data; break;
  }

  //----------------------------------------

  ime = ime_delay;

  if (state == Z80_STATE_DECODE) {
    int_ack_ = 0;
    imask_ = imask;
    intf_ = intf;

    bool cond_fail = false;

    switch (OP_ROW & 3) {
    case 0: cond_fail = (f & F_ZERO); break;
    case 1: cond_fail = !(f & F_ZERO); break;
    case 2: cond_fail = (f & F_CARRY); break;
    case 3: cond_fail = !(f & F_CARRY); break;
    }

    no_branch = (JR_CC_R8 || RET_CC || JP_CC_A16 || CALL_CC_A16) && cond_fail;
    no_halt = ((imask_ & intf_) && !ime);

    interrupt = (imask_ & intf_) && ime;

    if (interrupt) {
      op = 0x00;
      state = Z80_STATE_INT0;
    }
    else {
      state = first_state();
    }
  }

  // reads interrupt, no_branch, no_halt, unhalt
  state_ = next_state();

  //----------------------------------------

  switch (state) {
  case Z80_STATE_DECODE: break;

  case Z80_STATE_CB0: break;
  case Z80_STATE_CB1: {
    if (OP_CB_R) {
      AluOut out = alu_cb(CB_QUAD, CB_ROW, reg_get8(), f);
      uint8_t mask = cb_flag_mask[CB_QUAD];
      f = (f & ~mask) | (out.f & mask);
      reg_put8(CB_COL, (uint8_t)out.x);
    }
    break;
  }

  case Z80_STATE_HALT0:
    unhalt = 0;
    break;
  case Z80_STATE_HALT1:
    break;

  case Z80_STATE_INT0: break;
  case Z80_STATE_INT1: break;
  case Z80_STATE_INT2: break;
  case Z80_STATE_INT3: break;
  case Z80_STATE_INT4: break;

  //*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(

  case Z80_STATE_ALU_LO: {
    if (MV_R_R) {
      reg_put8(OP_ROW, (uint8_t)reg_get8());
      break;
    }

    AluOut out = {0};
    if (ALU_A_R) {
      out = alu(OP_ROW, a, reg_get8(), f);
      a = (uint8_t)out.x;
    }
    else if (ALU_A_HL) {
      out = alu(OP_ROW, a, bus_data, f);
      a = (uint8_t)out.x;
    }
    else if (RLU_R) {
      out = rlu(OP_ROW, reg_get8(), f);
      a = (uint8_t)out.x;
    }
    else if (INC_R) {
      out = add(reg_get8(), 1, 0);
      reg_put8(OP_ROW, (uint8_t)out.x);
    }
    else if (DEC_R) {
      out = alu(2, reg_get8(), 1, 0);
      reg_put8(OP_ROW, (uint8_t)out.x);
    }  
    else if (ADD_HL_RR) {
      uint8_t y = 0;
      if (ADD_HL_BC) y = c;
      if (ADD_HL_DE) y = e;
      if (ADD_HL_HL) y = l;
      if (ADD_HL_SP) y = p;

      out = add(l, y, 0);
      l = (uint8_t)out.x;
    }

    uint8_t mask = flag_mask[op];
    f = (f & ~mask) | (out.f & mask);

    // RLCA, RRCA, RLA, and RRA always clear the zero bit - hardware bug?
    if ((op & 0b11100111) == 0b00000111) f &= ~F_ZERO;

    break;
  }

  //*(*(*(*(*(*(*(*(*(*(*(*(*(*


  case Z80_STATE_ALU_HI:

    if (ADD_HL_RR) {
      uint8_t y = 0;
      if (ADD_HL_BC) y = b;
      if (ADD_HL_DE) y = d;
      if (ADD_HL_HL) y = h;
      if (ADD_HL_SP) y = s;

      AluOut out = add(h, y, f);
      uint8_t mask = flag_mask[op];
      h = (uint8_t)out.x;
      f = (f & ~mask) | (out.f & mask);
    }
    else if (LD_HL_SP_R8) {
      bool halfcarry = (sp & 0x000F) + (bus_data & 0x000F) > 0x000F;
      bool carry =     (sp & 0x00FF) + (bus_data & 0x00FF) > 0x00FF;

      hl = sp + (int8_t)lo;
      f  = (halfcarry ? F_HALF_CARRY : 0) | (carry ? F_CARRY : 0);
    }

    break;

  //*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(*(

  // Gameboy weirdness - the "real" interrupt vector is determined by the
  // state of imask/intf after pushing the first byte of PC onto the stack.

  case Z80_STATE_PUSH0: break;
  case Z80_STATE_PUSH1: imask_latch = imask_; break;
  case Z80_STATE_PUSH2: break;

  //----------------------------------------

  case Z80_STATE_POP0:
    break;

  case Z80_STATE_POP1:
    if (POP_BC) c = bus_data;
    if (POP_DE) e = bus_data;
    if (POP_HL) l = bus_data;
    if (POP_AF) f = bus_data & 0xF0;
    break;

  case Z80_STATE_POP2:
    if (POP_BC) b = bus_data;
    if (POP_DE) d = bus_data;
    if (POP_HL) h = bus_data;
    if (POP_AF) a = bus_data;
    break;

  //----------------------------------------

  case Z80_STATE_ARG0: break;

  case Z80_STATE_ARG1:
    if (LD_R_D8) {
      reg_put8(OP_ROW, bus_data);
      break;
    }

    if (ALU_A_D8) {
      AluOut out = alu(OP_ROW, a, bus_data, f);
      uint8_t mask = flag_mask[op];
      f = (f & ~mask) | (out.f & mask);
      a = (uint8_t)out.x;
    }
    else if (ADD_SP_R8) {
      bool halfcarry = (sp & 0x000F) + (bus_data & 0x000F) > 0x000F;
      bool carry =     (sp & 0x00FF) + (bus_data & 0x00FF) > 0x00FF;

      sp = sp + (int8_t)lo;
      f = (halfcarry ? F_HALF_CARRY : 0) | (carry ? F_CARRY : 0);
    }
    else if (LD_BC_D16) c = bus_data;
    else if (LD_DE_D16) e = bus_data;
    else if (LD_HL_D16) l = bus_data;
    else if (LD_SP_D16) p = bus_data;

    break;

  case Z80_STATE_ARG2:
    if      (LD_BC_D16) b = bus_data;
    else if (LD_DE_D16) d = bus_data;
    else if (LD_HL_D16) h = bus_data;
    else if (LD_SP_D16) s = bus_data;
    break;

  //----------------------------------------

  case Z80_STATE_MEM_READ0:
    break;

  case Z80_STATE_MEM_READ1: {

    if      (LDM_A_RR)  reg_put8(7,      bus_data);
    else if (LDM_A_A8)  reg_put8(7,      bus_data);
    else if (LDM_A_C)   reg_put8(7,      bus_data);
    else if (LDM_A_A16) reg_put8(7,      bus_data);
    else if (LDM_R_HL)  reg_put8(OP_ROW, bus_data);
    else if (INC_AT_HL) {
      AluOut out = alu(0, bus_data, 1, 0);
      uint8_t mask = flag_mask[op];
      f = (f & ~mask) | (out.f & mask);
      data_out = bus_data + 1;
    }
    else if (DEC_AT_HL) {
      AluOut out = alu(2, bus_data, 1, 0);
      uint8_t mask = flag_mask[op];
      f = (f & ~mask) | (out.f & mask);
      data_out = bus_data - 1;
    }
    else if (OP_CB_HL) {
      AluOut out = alu_cb(CB_QUAD, CB_ROW, bus_data, f);
      uint8_t mask = cb_flag_mask[CB_QUAD];
      f = (f & ~mask) | (out.f & mask);
      data_out = (uint8_t)out.x;
    }
    break;
  }

  //----------------------------------------

  case Z80_STATE_MEM_WRITE0: break;
  case Z80_STATE_MEM_WRITE1: break;
  case Z80_STATE_MEM_WRITE2: break;

  //----------------------------------------

  case Z80_STATE_DELAY_C:
    // these three do not use the alu
    if (INC_RR) {
      switch(OP_ROW >> 1) {
      case 0: bc++; break;
      case 1: de++; break;
      case 2: hl++; break;
      case 3: sp++; break;
      }
    }
    else if (DEC_RR) {
      switch(OP_ROW >> 1) {
      case 0: bc--; break;
      case 1: de--; break;
      case 2: hl--; break;
      case 3: sp--; break;
      }
    }
    else if (LD_SP_HL)    sp = hl;

    break;
  }

  //--------------------------------------------------------------------------------
  // Set up read

  switch(state_) {
  case Z80_STATE_DECODE: {
    if (state == Z80_STATE_INT4) {
      // Someone could've changed the interrupt mask or flags while we were
      // handling the interrupt, so we have to compute the new PC at the very
      // last second.

      uint8_t interrupts = imask_latch & intf_;
      int vector = -1;
      if (interrupts & INT_JOYPAD) vector = 4; // joypad
      if (interrupts & INT_SERIAL) vector = 3; // serial
      if (interrupts & INT_TIMER)  vector = 2; // timer
      if (interrupts & INT_STAT)   vector = 1; // lcd stat
      if (interrupts & INT_VBLANK) vector = 0; // vblank

      if (vector >= 0) {
        int_ack_ = 1 << vector;
        pc = uint16_t(0x0040 + (vector << 3));
      } else {
        pc = 0x0000;
      }
      addr = pc;
    }
    else {
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
      addr = pc;
      pc = addr + 1;
    }

    if (interrupt)  { ime = false;     ime_delay = false; }
    else if (RETI)  { ime = true;      ime_delay = true; }
    else if (DI)    { ime = false;     ime_delay = false; }
    else if (EI)    { ime = ime_delay; ime_delay = true; }
    break;
  }

  case Z80_STATE_CB1:
  case Z80_STATE_ARG1:
  case Z80_STATE_ARG2:
    addr = pc;
    pc = addr + 1;
    break;

  case Z80_STATE_POP1:
  case Z80_STATE_POP2:
    addr = sp;
    sp = addr + 1;
    break;

  case Z80_STATE_ALU_LO:
    if (ALU_A_HL) addr = hl;
    break;
  }

  if (state_ == Z80_STATE_MEM_READ1) {
    if      (LDM_A_A16)   { addr = temp; }
    else if (LDM_A_A8)    { addr = 0xFF00 | lo; }
    else if (LDM_A_C)     { addr = 0xFF00 | c; }
    else if (LDM_A_BC)    { addr = bc; }
    else if (LDM_A_DE)    { addr = de; }

    else if (LDM_R_HL)    { addr = hl; }
    else if (LDM_A_HLP)   { addr = hl; hl++; }
    else if (LDM_A_HLM)   { addr = hl; hl--; }
    else if (INC_AT_HL)   { addr = hl; }
    else if (DEC_AT_HL)   { addr = hl; }
    else if (OP_CB_HL)    { addr = hl; }
  }
}











//-----------------------------------------------------------------------------
// TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2

CpuBus Z80::tick_t2() const {
  return { addr, data_out, true, false };
}










//-----------------------------------------------------------------------------
// TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2

void Z80::tock_t2() {

  //----------------------------------------
  // set up addr/data_out for write

  switch (state_) {
  case Z80_STATE_MEM_WRITE1: {
    if      (STM_BC_A)       { addr = bc;          data_out = a; }
    else if (STM_DE_A)       { addr = de;          data_out = a; }
    else if (STM_HLP_A)      { addr = hl;          data_out = a; hl++; }
    else if (STM_HLM_A)      { addr = hl;          data_out = a; hl--; }
    else if (STM_HL_D8)      { addr = hl;          data_out = lo; }
    else if (STM_HL_R)       { addr = hl;          data_out = reg_get8(); }
    else if (STM_A16_A)      { addr = temp;        data_out = a; }
    else if (STM_A8_A)       { addr = 0xFF00 | lo; data_out = a; }
    else if (STM_C_A)        { addr = 0xFF00 | c;  data_out = a; }
    else if (STM_A16_SP)     { addr = temp;        data_out = (uint8_t)sp; }
    else if (INC_AT_HL)      { addr = hl;          data_out = data_out; }
    else if (DEC_AT_HL)      { addr = hl;          data_out = data_out; }
    else if (OP_CB_HL)       { addr = hl;          data_out = data_out; }
    break;
  }

  case Z80_STATE_MEM_WRITE2: {
    addr = temp + 1;
    data_out = (uint8_t)(sp >> 8);
    break;
  }

  case Z80_STATE_PUSH1: {
    addr = sp - 1;
    sp   = sp - 1;
    if      (PUSH_BC)     data_out = b;
    else if (PUSH_DE)     data_out = d;
    else if (PUSH_HL)     data_out = h;
    else if (PUSH_AF)     data_out = a;
    else if (CALL_A16)    data_out = (uint8_t)(pc >> 8);
    else if (CALL_CC_A16) data_out = (uint8_t)(pc >> 8);
    else if (RST_NN)      data_out = (uint8_t)(pc >> 8);
    break;
  }

  case Z80_STATE_PUSH2: {
    addr = sp - 1;
    sp   = sp - 1;
    if      (PUSH_BC)     data_out = c;
    else if (PUSH_DE)     data_out = e;
    else if (PUSH_HL)     data_out = l;
    else if (PUSH_AF)     data_out = f;
    else if (CALL_A16)    data_out = (uint8_t)(pc);
    else if (CALL_CC_A16) data_out = (uint8_t)(pc);
    else if (RST_NN)      data_out = (uint8_t)(pc);
    break;
  }

  case Z80_STATE_INT3: {
    sp--;
    addr = sp;
    data_out = (uint8_t)(pc >> 8);
    break;
  }

  case Z80_STATE_INT4: {
    sp--;
    addr = sp;
    data_out = (uint8_t)(pc >> 0);
    break;
  }
  }

  //----------------------------------------

  state = state_;
  cycle++;
}





















//-----------------------------------------------------------------------------

Z80State Z80::first_state() {
  if (PREFIX_CB)   return Z80_STATE_CB0;

  if (HALT)        return Z80_STATE_HALT0;

  if (ALU_A_R)     return Z80_STATE_ALU_LO;
  if (INC_R)       return Z80_STATE_ALU_LO;
  if (DEC_R)       return Z80_STATE_ALU_LO;
  if (RLU_R)       return Z80_STATE_ALU_LO;
  if (MV_R_R)      return Z80_STATE_ALU_LO;
  if (ADD_HL_RR)   return Z80_STATE_ALU_LO;
  
  if (POP_RR)      return Z80_STATE_POP0;
  if (RET)         return Z80_STATE_POP0;
  if (RETI)        return Z80_STATE_POP0;
  // ret_cc has an extra delay cycle before pop

  if (LD_R_D8)     return Z80_STATE_ARG0;
  if (LDM_A_A8)    return Z80_STATE_ARG0;
  if (STM_HL_D8)   return Z80_STATE_ARG0;
  if (JR_CC_R8)    return Z80_STATE_ARG0;
  if (JR_R8)       return Z80_STATE_ARG0;
  if (LD_HL_SP_R8) return Z80_STATE_ARG0;
  if (STM_A8_A)    return Z80_STATE_ARG0;
  if (ALU_A_D8)    return Z80_STATE_ARG0;
  if (ADD_SP_R8)   return Z80_STATE_ARG0;

  if (LDM_A_A16)   return Z80_STATE_ARG0;
  if (LD_RR_D16)   return Z80_STATE_ARG0;
  if (STM_A16_A)   return Z80_STATE_ARG0;
  if (STM_A16_SP)  return Z80_STATE_ARG0;
  
  if (JP_A16)      return Z80_STATE_ARG0;
  if (JP_CC_A16)   return Z80_STATE_ARG0;

  if (CALL_A16)    return Z80_STATE_ARG0;
  if (CALL_CC_A16) return Z80_STATE_ARG0;

  if (INC_AT_HL)   return Z80_STATE_MEM_READ0;
  if (DEC_AT_HL)   return Z80_STATE_MEM_READ0;
  if (LDM_A_HLP)   return Z80_STATE_MEM_READ0;
  if (LDM_A_HLM)   return Z80_STATE_MEM_READ0;
  if (LDM_R_HL)    return Z80_STATE_MEM_READ0;
  if (LDM_A_BC)    return Z80_STATE_MEM_READ0;
  if (LDM_A_DE)    return Z80_STATE_MEM_READ0;
  if (LDM_A_C)     return Z80_STATE_MEM_READ0;
  if (ALU_A_HL)    return Z80_STATE_MEM_READ0;

  if (STM_HL_R)    return Z80_STATE_MEM_WRITE0;
  if (STM_HLP_A)   return Z80_STATE_MEM_WRITE0;
  if (STM_HLM_A)   return Z80_STATE_MEM_WRITE0;
  if (STM_C_A)     return Z80_STATE_MEM_WRITE0;
  if (STM_BC_A)    return Z80_STATE_MEM_WRITE0;
  if (STM_DE_A)    return Z80_STATE_MEM_WRITE0;

  return Z80_STATE_DECODE;
}

//-----------------------------------------------------------------------------

Z80State Z80::next_state() {
  Z80State next = Z80_STATE_DECODE;

  switch (state) {
  case Z80_STATE_DECODE: {
    if      (interrupt)     next = Z80_STATE_INT1;
    else if (NOP)           next = Z80_STATE_DECODE;
    else if (DI)            next = Z80_STATE_DECODE;
    else if (EI)            next = Z80_STATE_DECODE;
    else if (JP_HL)         next = Z80_STATE_DECODE;

    else if (RET_CC)        next = Z80_STATE_POP0;
    else if (RST_NN)        next = Z80_STATE_PUSH0;

    else if (INC_RR)        next = Z80_STATE_DELAY_C;
    else if (DEC_RR)        next = Z80_STATE_DELAY_C;
    else if (LD_SP_HL)      next = Z80_STATE_DELAY_C;

    else if (PUSH_RR)       next = Z80_STATE_PUSH0;

    else {
      printf("?");
    }
    break;
  }

  //----------

  case Z80_STATE_INT0: next = Z80_STATE_INT1; break;
  case Z80_STATE_INT1: next = Z80_STATE_INT2; break;
  case Z80_STATE_INT2: next = Z80_STATE_INT3; break;
  case Z80_STATE_INT3: next = Z80_STATE_INT4; break;
  case Z80_STATE_INT4: next = Z80_STATE_DECODE; break;

  //----------

  case Z80_STATE_HALT0:
    next = no_halt ? Z80_STATE_DECODE : Z80_STATE_HALT1;
    break;

  case Z80_STATE_HALT1:
    next = unhalt ? Z80_STATE_DECODE : Z80_STATE_HALT1;
    break;

  //----------

  case Z80_STATE_CB0:
    next = Z80_STATE_CB1;
    break;

  case Z80_STATE_CB1:
    next = CB_COL == 6 ? Z80_STATE_MEM_READ1 : Z80_STATE_DECODE;
    break;

  //----------

  case Z80_STATE_ALU_LO:
    next = ADD_HL_RR ? Z80_STATE_ALU_HI : Z80_STATE_DECODE;
    break;

  case Z80_STATE_ALU_HI:
    next = Z80_STATE_DECODE;
    break;

  //----------

  case Z80_STATE_PUSH0: next = Z80_STATE_PUSH1; break;
  case Z80_STATE_PUSH1: next = Z80_STATE_PUSH2; break;
  case Z80_STATE_PUSH2: next = Z80_STATE_DECODE; break;

  //----------

  case Z80_STATE_POP0:
    next = Z80_STATE_POP1;
    if (RET_CC) {
      next = no_branch ? Z80_STATE_DECODE : Z80_STATE_POP1;
    }
    break;

  case Z80_STATE_POP1:
    next = Z80_STATE_POP2;
    break;

  case Z80_STATE_POP2:
    if      (POP_RR) next = Z80_STATE_DECODE;
    else if (RET)    next = Z80_STATE_DELAY_C;
    else if (RETI)   next = Z80_STATE_DELAY_C;
    else if (RET_CC) next = Z80_STATE_DELAY_C;
    else printf("fail");
    break;

  //----------

  case Z80_STATE_ARG0:
    next = Z80_STATE_ARG1;
    break;

  case Z80_STATE_ARG1:
    next = Z80_STATE_ARG2;
    if      (LDM_A_A8)      next = Z80_STATE_MEM_READ1;
    else if (STM_HL_D8)     next = Z80_STATE_MEM_WRITE1;
    else if (STM_A8_A)      next = Z80_STATE_MEM_WRITE1;
    else if (ADD_SP_R8)     next = Z80_STATE_ALU_HI;
    else if (LD_HL_SP_R8)   next = Z80_STATE_ALU_HI;
    else if (JR_R8)         next = Z80_STATE_DELAY_C;
    else if (JR_CC_R8)      next = Z80_STATE_DELAY_C;
    else if (LD_R_D8)       next = Z80_STATE_DECODE;
    else if (ALU_A_D8)      next = Z80_STATE_DECODE;

    if (JR_CC_R8 && no_branch) next = Z80_STATE_DECODE;
    break;

  case Z80_STATE_ARG2:
    if      (LDM_A_A16)     next = Z80_STATE_MEM_READ1;
    else if (STM_A16_A)     next = Z80_STATE_MEM_WRITE1;
    else if (STM_A16_SP)    next = Z80_STATE_MEM_WRITE1;
    else if (CALL_A16)      next = Z80_STATE_PUSH0;
    else if (JP_A16)        next = Z80_STATE_DELAY_C;
    else if (JP_CC_A16)     next = Z80_STATE_DELAY_C;
    else if (LD_RR_D16)     next = Z80_STATE_DECODE;
    else if (CALL_CC_A16)   next = Z80_STATE_PUSH0;
    if (no_branch) next = Z80_STATE_DECODE;
    break;

  //----------

  case Z80_STATE_MEM_READ0:
    next = Z80_STATE_MEM_READ1;
    if (ALU_A_HL)    next = Z80_STATE_ALU_LO;
    break;

  case Z80_STATE_MEM_READ1:
    if      (INC_AT_HL)    next = Z80_STATE_MEM_WRITE1;
    else if (DEC_AT_HL)    next = Z80_STATE_MEM_WRITE1;
    else if (STM_HLP_A)    next = Z80_STATE_MEM_WRITE1;
    else if (STM_HLM_A)    next = Z80_STATE_MEM_WRITE1;
    else if (OP_CB_R)      next = Z80_STATE_MEM_WRITE1;
    else if (OP_CB_HL)     next = Z80_STATE_MEM_WRITE1;

    break;

  //----------

  case Z80_STATE_MEM_WRITE0:
    next = Z80_STATE_MEM_WRITE1;
    break;

  case Z80_STATE_MEM_WRITE1:
    next = STM_A16_SP ? Z80_STATE_MEM_WRITE2 : Z80_STATE_DECODE;
    break;

  case Z80_STATE_MEM_WRITE2:
    next = Z80_STATE_DECODE;
    break;

  //----------

  case Z80_STATE_DELAY_C:
    next = Z80_STATE_DECODE;
    break;
  }

  return next;
}

//-----------------------------------------------------------------------------

uint8_t flag_mask2(uint8_t op, uint8_t cb) {
  uint8_t quad = (op >> 6) & 3;
  uint8_t row  = (op >> 3) & 7;
  uint8_t col  = (op >> 0) & 7;

  if (quad == 2) return 0xF0;
  if (quad == 0 && col == 1) return row & 1 ? 0x70 : 0x00;
  if (quad == 0 && col == 4) return 0xE0;
  if (quad == 0 && col == 5) return 0xE0;
  if (quad == 3 && col == 6) return 0xF0;

  if (op == 0xCB && (cb >> 6) == 0) return 0xF0;
  if (op == 0xCB && (cb >> 6) == 1) return 0xE0;

  if (op == 0x07) return 0xF0;
  if (op == 0x0F) return 0xF0;
  if (op == 0x17) return 0xF0;
  if (op == 0x1F) return 0xF0;
  if (op == 0x27) return 0xB0;
  if (op == 0x2F) return 0x60;
  if (op == 0x37) return 0x70;
  if (op == 0x3F) return 0x70;
  if (op == 0xE8) return 0xF0;
  if (op == 0xF1) return 0xF0;
  if (op == 0xF8) return 0xF0;

  return 0;
}

//-----------------------------------------------------------------------------

uint8_t Z80::reg_get8() const {
  int mux = (op & 0b11000000) ? OP_COL : OP_ROW;
  if (PREFIX_CB) mux = CB_COL;
  if (RLU_R) mux = OP_COL;

  switch(mux) {
  case 0: return b;
  case 1: return c;
  case 2: return d;
  case 3: return e;
  case 4: return h;
  case 5: return l;
  case 6: {
    printf("reg_get8 fail\n");
    return 0;
  }
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
  case 6: {
    printf("reg_put8 fail\n");
    break;
  }
  case 7: a = (uint8_t)reg; break;
  }
}

//-----------------------------------------------------------------------------
// 4-bit-at-a-time ALU just for amusement

#if 0
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
  if (op == 7) out.x = x;
  return out;
}
#endif

AluOut alu(const uint8_t op, const uint8_t x, const uint8_t y, const uint8_t f) {
  uint16_t d1 = 0, d2 = 0, c = ((f >> 4) & 1);

  if (op == 0) { d1 = (x & 0xF) + (y & 0xF);     d2 = x + y; }
  if (op == 1) { d1 = (x & 0xF) + (y & 0xF) + c; d2 = x + y + c; }
  if (op == 2) { d1 = (x & 0xF) - (y & 0xF);     d2 = x - y; }
  if (op == 3) { d1 = (x & 0xF) - (y & 0xF) - c; d2 = x - y - c; }
  if (op == 4) { d1 = 0xFFF;                     d2 = x & y; }
  if (op == 5) { d1 = 0x000;                     d2 = x ^ y; }
  if (op == 6) { d1 = 0x000;                     d2 = x | y; }
  if (op == 7) { d1 = (x & 0xF) - (y & 0xF);     d2 = x - y; }

  uint8_t z = (uint8_t)d2;
  uint8_t g = (op == 2 || op == 3 || op == 7) ? F_NEGATIVE : 0;

  if (d1 & 0x010) g |= F_HALF_CARRY;
  if (d2 & 0x100) g |= F_CARRY;
  if (z == 0x000) g |= F_ZERO;
  if (op == 7)    z = x;

  return {z, g};
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

AluOut alu_cb(const uint8_t quad, const uint8_t row, const uint8_t x, const uint8_t f) {
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
