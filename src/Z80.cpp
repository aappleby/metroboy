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

#define INTERRUPT     ((imask_ & intf_) && ime)

#define ALU_A_D8      ((OP_QUAD == 3) && (OP_COL == 6))
#define RST_NN        ((OP_QUAD == 3) && (OP_COL == 7))

uint8_t  sxt(uint8_t x) { return x & 0x80 ? 0xFF : 0x00; }

//-----------------------------------------------------------------------------

void Z80::reset(uint16_t new_pc) {
  *this = {};

  tcycle = -1;

  if (new_pc == 0x100) {
    pc = new_pc;
    bc = 0x0013;
    de = 0x00D8;
    hl = 0x014D;
    af = 0x01B0;
    sp = 0xFFFE;
    xy = 0x0000;
  }
  else {
    pc = new_pc;
    bc = 0x0000;
    de = 0x0000;
    hl = 0x0000;
    af = 0x0000;
    sp = 0x0000;
    xy = 0x0000;
  }
  op_addr = pc;

  addr = new_pc;
  data = 0;
  write = 0;

  out_int_ack = 0;
  handle_int = 0;
}

//-----------------------------------------------------------------------------

Req Z80::get_bus_req() const {
  return {
    .addr  = addr,
    .data  = data,
    .read  = (bool)read,
    .write = (bool)write,
  };
}

void Z80::on_bus_ack(Ack ibus_ack_) {
  if (read) {
    data = (uint8_t)ibus_ack_.data;
  }
}

//-----------------------------------------------------------------------------

void Z80::update_flags() {
  uint8_t f_ = alu_out.f;
  uint8_t mask = PREFIX_CB ? cb_flag_mask[CB_QUAD] : flag_mask[op];
  f = (f & ~mask) | (f_ & mask);

  // RLCA, RRCA, RLA, and RRA always clear the zero bit - hardware bug?
  if ((op & 0b11100111) == 0b00000111) f &= ~F_ZERO;

  // ADD_SP_R8 and LD_HL_SP_R8 always clear the zero bit and negative bit.
  if ((op & 0b11101111) == 0b11101000) f &= ~(F_ZERO | F_NEGATIVE);

  alu_out.f = f;
}

//-----------------------------------------------------------------------------
// Do the meat of executing the instruction

#pragma warning(disable:4189)

void Z80::tock(const int tcycle_, const uint8_t imask_, const uint8_t intf_) {
  const int tphase = tcycle_ & 3;
  if (tphase != 0) return;

  tcycle = tcycle_;
  ime = ime_;

  if (state == 0) {
    op_addr = pc;
    op = data;
  }

  bool cond_fail = false;
  switch (OP_ROW & 3) {
  case 0: cond_fail = (f & F_ZERO); break;
  case 1: cond_fail = !(f & F_ZERO); break;
  case 2: cond_fail = (f & F_CARRY); break;
  case 3: cond_fail = !(f & F_CARRY); break;
  }

  if (state == 0) {
    int_ack = 0;
    imask = imask_;
    intf = intf_;
    handle_int = 0;

    no_branch = (JR_CC_R8 || RET_CC || JP_CC_A16 || CALL_CC_A16) && cond_fail;
    no_halt = ((imask & intf) && !ime);

    interrupt = (imask & intf) && ime;

    if (interrupt) {
      handle_int = 1;
      ime = false;
      ime_ = false;
    }
  }



  if (PREFIX_CB && state == 1) cb = data;
  if (LDM_A_A8  && state == 0) x = 0xFF;
  if (STM_A8_A  && state == 0) x = 0xFF;
  if (LDM_A_C   && state == 0) x = 0xFF;
  if (STM_C_A   && state == 0) x = 0xFF;

  do {
    state_ = -1;
    write = 0xFF;
    AluOut& ao = alu_out;
    bool nb = no_branch;
    bool tb = !no_branch;
    uint8_t r = reg_get8();

#define READ(A)     { addr = A;           write = 0; read = 1; }
#define WRITE(A, B) { addr = A; data = B; write = 1; read = 0; }
#define PASS(A)     { addr = A;           write = 0; read = 0; }

    //----------------------------------------
    // INTERRUPTS

    if (handle_int) {
      if (state == 0) {                PASS(sp);       state_ = 1; break; }
      if (state == 1) { sp = addr - 1; WRITE(sp, pch); state_ = 2; break; }
      if (state == 2) { sp = addr - 1; WRITE(sp, pcl); state_ = 3; break; }
      if (state == 3) {                PASS(xy);       state_ = 4; break; }
      if (state == 4) {
        if (imask & intf & INT_JOYPAD_MASK) { xy = 0x0060; int_ack = INT_JOYPAD_MASK; }
        if (imask & intf & INT_SERIAL_MASK) { xy = 0x0058; int_ack = INT_SERIAL_MASK; }
        if (imask & intf & INT_TIMER_MASK)  { xy = 0x0050; int_ack = INT_TIMER_MASK; }
        if (imask & intf & INT_STAT_MASK)   { xy = 0x0048; int_ack = INT_STAT_MASK; }
        if (imask & intf & INT_VBLANK_MASK) { xy = 0x0040; int_ack = INT_VBLANK_MASK; }
        READ(xy);
        state_ = 0;
        break;
      }
    }

    //----------------------------------------
    // QUAD 0

    else if (OP_QUAD == 0) {
      if (OP_COL == 0) {
        if (NOP)          { pc = addr + 1;           READ(pc); state_ = 0; }
        else if (STM_A16_SP) {
          if (state == 0) { pc = addr + 1;           READ(pc);     state_ = 1; }
          if (state == 1) { pc = addr + 1; y = data; READ(pc);     state_ = 2; }
          if (state == 2) { pc = addr + 1; x = data; WRITE(xy, p); state_ = 3; }
          if (state == 3) { xy = addr + 1;           WRITE(xy, s); state_ = 4; }
          if (state == 4) {                          READ(pc);     state_ = 0; }
        }
        else if (STOP)    { pc = addr + 1;           READ(pc); state_ = 0;  }
        else if (JR_R8) {
          if (state == 0) { pc = addr + 1;                                        READ(pc); state_ = 1; }
          if (state == 1) { pc = addr + 1; ao = alu(0, pcl,      data); y = ao.x; PASS(pc); state_ = 2; }
          if (state == 2) {                ao = alu(1, pch, sxt(data)); x = ao.x; READ(xy); state_ = 0; }
        }
        else if (JR_CC_R8) {
          if (state == 0)       { pc = addr + 1;                                        READ(pc); state_ = 1; }
          if (state == 1 && nb) { pc = addr + 1;                                        READ(pc); state_ = 0; }
          if (state == 1 && tb) { pc = addr + 1; ao = alu(0, pcl,      data); y = ao.x; PASS(pc); state_ = 2; }
          if (state == 2)       {                ao = alu(1, pch, sxt(data)); x = ao.x; READ(xy); state_ = 0; }
        }
        break;
      }
      else if (OP_COL == 1) {
        if (LD_RR_D16) {
          if (state == 0) {
            if (LD_BC_D16) { pc = addr + 1;           READ(pc); state_ = 1; }
            if (LD_DE_D16) { pc = addr + 1;           READ(pc); state_ = 1; }
            if (LD_HL_D16) { pc = addr + 1;           READ(pc); state_ = 1; }
            if (LD_SP_D16) { pc = addr + 1;           READ(pc); state_ = 1; }
          }
          else if (state == 1) {
            if (LD_BC_D16) { pc = addr + 1; c = data; READ(pc); state_ = 2; }
            if (LD_DE_D16) { pc = addr + 1; e = data; READ(pc); state_ = 2; }
            if (LD_HL_D16) { pc = addr + 1; l = data; READ(pc); state_ = 2; }
            if (LD_SP_D16) { pc = addr + 1; p = data; READ(pc); state_ = 2; }
          }
          else if (state == 2) {
            if (LD_BC_D16) { pc = addr + 1; b = data; READ(pc); state_ = 0; }
            if (LD_DE_D16) { pc = addr + 1; d = data; READ(pc); state_ = 0; }
            if (LD_HL_D16) { pc = addr + 1; h = data; READ(pc); state_ = 0; }
            if (LD_SP_D16) { pc = addr + 1; s = data; READ(pc); state_ = 0; }
          }
        }
        else if (ADD_HL_RR) {
          if (state == 0) {
            if (ADD_HL_BC) { pc = addr + 1; ao = alu(0, l, c ); l = ao.x;                 READ(pc); state_ = 1; }
            if (ADD_HL_DE) { pc = addr + 1; ao = alu(0, l, e ); l = ao.x;                 READ(pc); state_ = 1; }
            if (ADD_HL_HL) { pc = addr + 1; ao = alu(0, l, l ); l = ao.x;                 READ(pc); state_ = 1; }
            if (ADD_HL_SP) { pc = addr + 1; ao = alu(0, l, p ); l = ao.x;                 READ(pc); state_ = 1; }
            update_flags();                             
          }                                             
          else if (state == 1) {                             
            if (ADD_HL_BC) {                ao = alu(1, h, b ); h = ao.x; update_flags(); READ(pc); state_ = 0; }
            if (ADD_HL_DE) {                ao = alu(1, h, d ); h = ao.x; update_flags(); READ(pc); state_ = 0; }
            if (ADD_HL_HL) {                ao = alu(1, h, h ); h = ao.x; update_flags(); READ(pc); state_ = 0; }
            if (ADD_HL_SP) {                ao = alu(1, h, s ); h = ao.x; update_flags(); READ(pc); state_ = 0; }
          }
        }
        break;
      }
      else if (OP_COL == 2) {
        if (state == 0) {
          if (STM_BC_A ) { pc = addr + 1;           WRITE(bc, a); state_ = 1; }
          if (STM_DE_A ) { pc = addr + 1;           WRITE(de, a); state_ = 1; }
          if (STM_HLP_A) { pc = addr + 1;           WRITE(hl, a); state_ = 1; }
          if (STM_HLM_A) { pc = addr + 1;           WRITE(hl, a); state_ = 1; }
          if (LDM_A_BC ) { pc = addr + 1;           READ(bc);     state_ = 1; }
          if (LDM_A_DE ) { pc = addr + 1;           READ(de);     state_ = 1; }
          if (LDM_A_HLP) { pc = addr + 1;           READ(hl);     state_ = 1; }
          if (LDM_A_HLM) { pc = addr + 1;           READ(hl);     state_ = 1; }
        }
        else if (state == 1) {
          if (STM_BC_A ) {                          READ(pc);     state_ = 0; }
          if (STM_DE_A ) {                          READ(pc);     state_ = 0; }
          if (STM_HLP_A) { hl = addr + 1;           READ(pc);     state_ = 0; }
          if (STM_HLM_A) { hl = addr - 1;           READ(pc);     state_ = 0; }
          if (LDM_A_BC ) { bc = addr;     a = data; READ(pc);     state_ = 0; }
          if (LDM_A_DE ) { de = addr;     a = data; READ(pc);     state_ = 0; }
          if (LDM_A_HLP) { hl = addr + 1; a = data; READ(pc);     state_ = 0; }
          if (LDM_A_HLM) { hl = addr - 1; a = data; READ(pc);     state_ = 0; }
        }
        break;
      }
      else if (OP_COL == 3) {
        if (state == 0) {
          if (INC_BC) { pc = addr + 1; PASS(bc); state_ = 1; }
          if (DEC_BC) { pc = addr + 1; PASS(bc); state_ = 1; }
          if (INC_DE) { pc = addr + 1; PASS(de); state_ = 1; }
          if (DEC_DE) { pc = addr + 1; PASS(de); state_ = 1; }
          if (INC_HL) { pc = addr + 1; PASS(hl); state_ = 1; }
          if (DEC_HL) { pc = addr + 1; PASS(hl); state_ = 1; }
          if (INC_SP) { pc = addr + 1; PASS(sp); state_ = 1; }
          if (DEC_SP) { pc = addr + 1; PASS(sp); state_ = 1; }
        }
        else if (state == 1) {
          if (INC_BC) { bc = addr + 1; READ(pc); state_ = 0; }
          if (DEC_BC) { bc = addr - 1; READ(pc); state_ = 0; }
          if (INC_DE) { de = addr + 1; READ(pc); state_ = 0; }
          if (DEC_DE) { de = addr - 1; READ(pc); state_ = 0; }
          if (INC_HL) { hl = addr + 1; READ(pc); state_ = 0; }
          if (DEC_HL) { hl = addr - 1; READ(pc); state_ = 0; }
          if (INC_SP) { sp = addr + 1; READ(pc); state_ = 0; }
          if (DEC_SP) { sp = addr - 1; READ(pc); state_ = 0; }
        }
        break;
      }
      else if (OP_COL == 4) {
      }
      else if (OP_COL == 5) {
      }
      else if (OP_COL == 6) {
      }
      else if (OP_COL == 7) {
      }
      else {
        if (state == 0) pc = addr + 1;
      }
    }

    //----------------------------------------
    // QUAD 1

    else if (OP_QUAD == 1) {
      if (HALT) {
        if (state == 0) {
          pc = addr + 1;
          if (no_halt)  { state_ = 0; }
          if (!no_halt) { state_ = 1; }
        }
        else if (state == 1) {
          if (!unhalt  ) { state_ = 1; }
          if (unhalt   ) { state_ = 0; }
        }
        READ(pc);
      }
      else if (OP_COL == 6) {
        // ld r, (hl)
        if (state == 0) { pc = addr + 1;  READ(hl);       state_ = 1; }
        if (state == 1) { reg_put8(data); READ(pc);       state_ = 0; }
      }
      else if (OP_ROW == 6) {
        // st (hl),r
        if (state == 0) { pc = addr + 1;  WRITE(hl, r);   state_ = 1; }
        if (state == 1) {                 READ(pc);       state_ = 0; }
      }
      else {
        // mv r,r
        pc = addr + 1;
        reg_put8(r);
        READ(pc);
        state_ = 0;
      }
      break;
    }

    //----------------------------------------
    // QUAD 2

    else if (OP_QUAD == 2) {
      if (OP_COL == 6) {
        // alu a, (hl)
        if (state == 0) { pc = addr + 1;                                    READ(hl); state_ = 1; }
        if (state == 1) { ao = alu(OP_ROW, a, r); a = ao.x; update_flags(); READ(pc); state_ = 0; }
      }
      else {
        // alu a, r
        { pc = addr + 1; ao = alu(OP_ROW, a, r); a = ao.x; update_flags(); READ(pc); state_ = 0; }
      }
      break;
    }

    //----------------------------------------
    // QUAD 3

    else if (OP_QUAD == 3) {
      if (POP_RR) {
        if (state == 0) { pc = addr + 1; READ(sp); state_ = 1; }
        if (state == 1) {                                        
          if (POP_BC) { sp = addr + 1; c = data; READ(sp);       state_ = 2; }
          if (POP_DE) { sp = addr + 1; e = data; READ(sp);       state_ = 2; }
          if (POP_HL) { sp = addr + 1; l = data; READ(sp);       state_ = 2; }
          if (POP_AF) { sp = addr + 1; f = data; READ(sp);       state_ = 2; }
        }                                                        
        if (state == 2) {                                        
          if (POP_BC) { sp = addr + 1; b = data; READ(pc);       state_ = 0; }
          if (POP_DE) { sp = addr + 1; d = data; READ(pc);       state_ = 0; }
          if (POP_HL) { sp = addr + 1; h = data; READ(pc);       state_ = 0; }
          if (POP_AF) { sp = addr + 1; a = data; READ(pc);       state_ = 0; }
        }
      }
      else if (PUSH_RR) {
        if (state == 0) { pc = addr + 1; READ(sp); state_ = 1; }                                        
        if (state == 1) {                        
          if (PUSH_BC) { sp = addr - 1;          WRITE(sp, b);   state_ = 2; }
          if (PUSH_DE) { sp = addr - 1;          WRITE(sp, d);   state_ = 2; }
          if (PUSH_HL) { sp = addr - 1;          WRITE(sp, h);   state_ = 2; }
          if (PUSH_AF) { sp = addr - 1;          WRITE(sp, a);   state_ = 2; }
        }                                        
        if (state == 2) {                        
          if (PUSH_BC) { sp = addr - 1;          WRITE(sp, c);   state_ = 3; }
          if (PUSH_DE) { sp = addr - 1;          WRITE(sp, e);   state_ = 3; }
          if (PUSH_HL) { sp = addr - 1;          WRITE(sp, l);   state_ = 3; }
          if (PUSH_AF) { sp = addr - 1;          WRITE(sp, f);   state_ = 3; }
        }                                        
        if (state == 3) {                        
          if (PUSH_BC) {                         READ(pc);       state_ = 0; }
          if (PUSH_DE) {                         READ(pc);       state_ = 0; }
          if (PUSH_HL) {                         READ(pc);       state_ = 0; }
          if (PUSH_AF) {                         READ(pc);       state_ = 0; }
        }
      }
      else if (STM_A8_A) {
        if (state == 0) { pc = addr + 1;           READ(pc);     state_ = 1; }
        if (state == 1) { pc = addr + 1; y = data; WRITE(xy, a); state_ = 2; }
        if (state == 2) {                          READ(pc);     state_ = 0; }
      }
      else if (ADD_SP_R8) {
        if (state == 0) { pc = addr + 1;                                      update_flags(); READ(pc); state_ = 1; }
        if (state == 1) { pc = addr + 1; ao = alu(0, p,      data); y = ao.x; update_flags(); PASS(pc); state_ = 2; }
        if (state == 2) {                ao = alu(1, s, sxt(data)); x = ao.x;                 PASS(xy); state_ = 3; }
        if (state == 3) { sp = addr;                                                          READ(pc); state_ = 0; }
      }
      else if (LDM_A_A8) {
        if (state == 0) { pc = addr + 1;                     READ(pc); state_ = 1; }
        if (state == 1) { pc = addr + 1; xy = 0xFF00 | data; READ(xy); state_ = 2; }
        if (state == 2) {                a = data;           READ(pc); state_ = 0; }
      }
      else if (LD_HL_SP_R8) {
        if (state == 0) { pc = addr + 1;                                       READ(pc); state_ = 1; }
        if (state == 1) { pc = addr + 1;                                       PASS(pc); state_ = 2; }
        if (state == 2) { ao = alu(0, p,      data); l = ao.x; update_flags(); PASS(pc); state_ = 3; }
        if (state == 3) { ao = alu(1, s, sxt(data)); h = ao.x;                 READ(pc); state_ = 0; }
      }
      else if (RET_CC) {
        if (state == 0)       { pc = addr + 1;           PASS(pc); state_ = 1; }
        if (state == 1 && tb) {                          READ(sp); state_ = 2; }
        if (state == 1 && nb) {                          READ(pc); state_ = 0; }
        if (state == 2)       { sp = addr + 1; y = data; READ(sp); state_ = 3; }
        if (state == 3)       { sp = addr + 1; x = data; PASS(xy); state_ = 4; }
        if (state == 4)       {                          READ(xy); state_ = 0; }
      }
      else if (RET) {
        if (state == 0) { pc = addr + 1;           READ(sp); state_ = 1; }
        if (state == 1) { sp = addr + 1; y = data; READ(sp); state_ = 2; }
        if (state == 2) { sp = addr + 1; x = data; PASS(xy); state_ = 3; }
        if (state == 3) {                          READ(xy); state_ = 0; }
      }
      else if (RETI) {
        if (state == 0) { pc = addr + 1;           READ(sp); state_ = 1; }
        if (state == 1) { sp = addr + 1; y = data; READ(sp); state_ = 2; }
        if (state == 2) { sp = addr + 1; x = data; PASS(xy); state_ = 3; }
        if (state == 3) {                          READ(xy); state_ = 0; }
      }
      else if (JP_HL) {
        if (state == 0) { pc = addr + 1; READ(hl); state_ = 0; }
      }
      else if (LD_SP_HL) {
        if (state == 0) { pc = addr + 1; PASS(hl); state_ = 1; }
        if (state == 1) { sp = addr;     READ(pc); state_ = 0; }
      }
      else if (JP_CC_A16) {
        if (state == 0)       { pc = addr + 1;           READ(pc); state_ = 1; }
        if (state == 1)       { pc = addr + 1; y = data; READ(pc); state_ = 2; }
        if (state == 2 && nb) { pc = addr + 1; x = data; READ(pc); state_ = 0; }
        if (state == 2 && tb) { pc = addr + 1; x = data; READ(pc); state_ = 3; }
        if (state == 3)       {                          READ(xy); state_ = 0; }
      }
      else if (STM_C_A) {
        if (state == 0) { pc = addr + 1; y = c;    WRITE(xy, a);   state_ = 1; }
        if (state == 1) {                          READ(pc);       state_ = 0; }
      }
      else if (STM_A16_A) {
        if (state == 0) { pc = addr + 1;           READ(pc);     state_ = 1; }
        if (state == 1) { pc = addr + 1; y = data; READ(pc);     state_ = 2; }
        if (state == 2) { pc = addr + 1; x = data; WRITE(xy, a); state_ = 3; }
        if (state == 3) {                          READ(pc);     state_ = 0; }
      }
      else if (LDM_A_C) {
        if (state == 0) { pc = addr + 1; y = c;    READ(xy); state_ = 1; }
        if (state == 1) {                a = data; READ(pc); state_ = 0; }
      }
      else if (LDM_A_A16) {
        if (state == 0) { pc = addr + 1;           READ(pc); state_ = 1; }
        if (state == 1) { pc = addr + 1; y = data; READ(pc); state_ = 2; }
        if (state == 2) { pc = addr + 1; x = data; READ(xy); state_ = 3; }
        if (state == 3) {                a = data; READ(pc); state_ = 0; }
      }
      else if (JP_A16) {
        // jp a16
        if (state == 0) { pc = addr + 1;           READ(pc); state_ = 1; }
        if (state == 1) { pc = addr + 1; y = data; READ(pc); state_ = 2; }
        if (state == 2) { pc = addr + 1; x = data; PASS(pc); state_ = 3; }
        if (state == 3) {                          READ(xy); state_ = 0; }
      }
      else if (PREFIX_CB) {
        if (state == 0) { pc = addr + 1; READ(pc); state_ = 1; }
        if (state == 1) {
          if (OP_CB_R  ) { pc = addr + 1;   ao = alu_cb(cb,  reg_get8(CB_COL)     );  reg_put8(CB_COL, ao.x);    update_flags();    READ(pc);       state_ = 0; }
          if (OP_CB_HL ) { pc = addr + 1;                                                                                           READ(hl);       state_ = 2; }
        }
        if (state == 2) { ao = alu_cb(cb, data); y = ao.x; update_flags(); WRITE(hl, y); state_ = 3; }
        if (state == 3) {                                                  READ(pc);     state_ = 0; }
      }
      else if (DI) {
        pc = addr + 1;
        ime = false;
        ime_ = false;
        READ(pc);
        state_ = 0;
      }
      else if (EI) {
        pc = addr + 1;
        ime = ime_;
        ime_ = true;
        READ(pc);
        state_ = 0;
      }
      else if (CALL_CC_A16) {
        if (state == 0)       { pc = addr + 1;           READ(pc);       state_ = 1; }
        if (state == 1)       { pc = addr + 1; y = data; READ(pc);       state_ = 2; }
        if (state == 2 && tb) { pc = addr + 1; x = data; PASS(sp);       state_ = 3; }
        if (state == 2 && nb) { pc = addr + 1;           READ(pc);       state_ = 0; }
        if (state == 3)       { sp = addr - 1;           WRITE(sp, pch); state_ = 4; }
        if (state == 4)       { sp = addr - 1;           WRITE(sp, pcl); state_ = 5; }
        if (state == 5)       { pc = addr + 1;           READ(xy);       state_ = 0; }
      }
      else if (CALL_A16) {
        if (state == 0) { pc = addr + 1;           READ(pc);       state_ = 1; }
        if (state == 1) { pc = addr + 1; y = data; READ(pc);       state_ = 2; }
        if (state == 2) { pc = addr + 1; x = data; PASS(sp);       state_ = 3; }
        if (state == 3) { sp = addr - 1;           WRITE(sp, pch); state_ = 4; }
        if (state == 4) { sp = addr - 1;           WRITE(sp, pcl); state_ = 5; }
        if (state == 5) {                          READ(xy);       state_ = 0; }
      }
      else if (ALU_A_D8) {
        if (state == 0) { pc = addr + 1;                                                      READ(pc); state_ = 1; }
        if (state == 1) { pc = addr + 1; ao = alu(OP_ROW, a, data); a = ao.x; update_flags(); READ(pc); state_ = 0; }
      }
      else if (RST_NN) {
        if (state == 0) { pc = addr + 1;  PASS(sp);       state_ = 1; }
        if (state == 1) { sp = addr - 1;  WRITE(sp, pch); state_ = 2; }
        if (state == 2) { sp = addr - 1;  WRITE(sp, pcl); state_ = 3; }
        if (state == 3) { xy = op & 0x38; READ(xy);       state_ = 0; }
      }
      break;
    }

    //----------------------------------------

    if (state == 0) {
      pc = addr + 1;
      if (INC_R            ) { ao = alu(0,      r,      1            );  reg_put8(ao.x);            READ(pc);       state_ = 0; }
      if (INC_AT_HL        ) {                                                                      READ(hl);       state_ = 1; }
      if (DEC_R            ) { ao = alu(2,      r,      1            );  reg_put8(ao.x);            READ(pc);       state_ = 0; }
      if (DEC_AT_HL        ) {                                                                      READ(hl);       state_ = 1; }
      if (LD_R_D8          ) {                                                                      READ(pc);       state_ = 1; }
      if (STM_HL_D8        ) {                                                                      READ(pc);       state_ = 1; }
      if (RLU_R            ) { ao = rlu(OP_ROW, r                    );  a = ao.x;                  READ(pc);       state_ = 0; }
      update_flags();
      break;

    }

    if (state == 1) {
      if (INC_AT_HL        ) {                  ao = alu(0,      data,   1            );  y = ao.x;                                     WRITE(hl, y);   state_ = 2; break; }
      if (DEC_AT_HL        ) {                  ao = alu(2,      data,   1            );  y = ao.x;                  update_flags();    WRITE(hl, y);   state_ = 2; break; }
      if (LD_R_D8          ) { pc = addr + 1;                                             reg_put8(data);                               READ(pc);       state_ = 0; break; }
      if (STM_HL_D8        ) { pc = addr + 1;                                             y = data;                                     WRITE(hl, y);   state_ = 2; break; }
    }

    if (state == 2) {
      if (INC_AT_HL        ) {                                                                                       update_flags();    READ(pc);       state_ = 0; break; }
      if (DEC_AT_HL        ) {                                                                                                          READ(pc);       state_ = 0; break; }
      if (STM_HL_D8        ) {                                                                                                          READ(pc);       state_ = 0; break; }
    }

  } while(0);

  if (state_ == 0) {
    alu_out.f = f;
  }

  alu_out.f &= 0xF0;
  f &= 0xF0;

  if (HALT && !no_halt && state == 0) { unhalt = 0; }
  if (RETI && state_ == 0) { ime = true; ime_ = true; }

  if (write == 0xFF)     printf("write fail\n");
  if (state_ == -1) printf("fail state invalid\n");

  state = state_;
  out_int_ack = int_ack;
}


//-----------------------------------------------------------------------------

/*
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
*/

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
  if (op == 2 || op == 3 || op == 7) out.f |= F_NEGATIVE;
  if (c2)         out.f |= F_HALF_CARRY;
  if (d2 & 0x10)  out.f |= F_CARRY;
  if (out.x == 0) out.f |= F_ZERO;
  if (op == 7) out.x = x;
  return out;
}

#else

#pragma warning(disable : 4458)

AluOut Z80::alu(const uint8_t op, const uint8_t x, const uint8_t y) {
  uint16_t d1 = 0, d2 = 0;
  const bool c = alu_out.f & F_CARRY;

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

  uint8_t z = (uint8_t)d2;
  uint8_t g = (op == 2 || op == 3 || op == 7) ? F_NEGATIVE : 0;

  if (d1 & 0x010) g |= F_HALF_CARRY;
  if (d2 & 0x100) g |= F_CARRY;
  if (z == 0x000) g |= F_ZERO;
  if (op == 7)    z = x;

  return {z, g};
}

#endif

//-----------------------------------------------------------------------------
// The logic is more annoying, but this can be implemented as two 4-bit additions

AluOut Z80::daa(uint8_t x, uint8_t f) {
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

AluOut Z80::rlu(const uint8_t op, const uint8_t x) {
  const uint8_t f = alu_out.f;
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

AluOut Z80::alu_cb(uint8_t cb, const uint8_t x) {
  const uint8_t f = alu_out.f;

  const uint8_t quad = CB_QUAD;
  const uint8_t row = CB_ROW;

  AluOut out = {0};

  switch (quad) {
  case 0: {
    switch (row) {
    case 0:
    case 1:
    case 2:
    case 3: {
      out = rlu(row, x);
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

#pragma warning(disable:4458)

void Z80::dump(std::string& d) {
  sprintf(d, "tcycle         %d\n", tcycle);
  sprintf(d, "bgb cycle      0x%08x\n", (tcycle * 8) + 0x00B2D5E6);
  sprintf(d, "state          %d\n", state);
  sprintf(d, "state_         %d\n", state_);
  sprintf(d, "\n");

  sprintf(d, "op_addr        0x%04x\n", op_addr);
  sprintf(d, "OP             0x%02x\n", op);
  sprintf(d, "CB             0x%02x\n", cb);
  sprintf(d, "no_branch      %d\n", no_branch);
  sprintf(d, "no_halt        %d\n", no_halt);
  sprintf(d, "\n");

  sprintf(d, "PC             0x%04x\n", pc);
  sprintf(d, "BC             0x%04x\n", bc);
  sprintf(d, "DE             0x%04x\n", de);
  sprintf(d, "HL             0x%04x\n", hl);
  sprintf(d, "SP             0x%04x\n", sp);
  sprintf(d, "AF             0x%04x\n", af);
  sprintf(d, "TEMP           0x%04x\n", xy);
  sprintf(d, "alu.x          0x%02x\n", alu_out.x);
  sprintf(d, "alu.g          0x%02x\n", alu_out.f);
  sprintf(d, "\n");

  sprintf(d, "addr           0x%04x\n", addr);
  sprintf(d, "data           0x%02x\n", data);
  sprintf(d, "write          %d\n",     write);
  sprintf(d, "\n");

  sprintf(d, "IME            %d\n", ime);
  sprintf(d, "IME_           %d\n", ime_);
  sprintf(d, "interrupt      %d\n", interrupt);
  sprintf(d, "imask          0x%02x\n", imask);
  sprintf(d, "intf           0x%02x\n", intf);
  sprintf(d, "int_ack        0x%02x\n", int_ack);
  sprintf(d, "out_int_ack    0x%02x\n", out_int_ack);
}

//-----------------------------------------------------------------------------
