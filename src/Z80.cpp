#include "Z80.h"

#include "Constants.h"

#include <assert.h>
#include <imgui.h>

#pragma warning(disable : 4100)

//-----------------------------------------------------------------------------

#define OP_ROW        ((op >> 3) & 7)
#define OP_COL        (op & 0x07)

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

#define LD_BC_D16     (op == 0x01)
#define LD_DE_D16     (op == 0x11)
#define LD_HL_D16     (op == 0x21)
#define LD_SP_D16     (op == 0x31)

#define ADD_HL_BC     (op == 0x09)
#define ADD_HL_DE     (op == 0x19)
#define ADD_HL_HL     (op == 0x29)
#define ADD_HL_SP     (op == 0x39)

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

#define LDM_R_HL      ((op & 0b11000111) == 0b01000110 && !HALT)

#define LDM_B_HL      (op == 0x46)
#define LDM_C_HL      (op == 0x4E)
#define LDM_D_HL      (op == 0x56)
#define LDM_E_HL      (op == 0x5E)
#define LDM_H_HL      (op == 0x66)
#define LDM_L_HL      (op == 0x6E)
#define LDM_A_HL      (op == 0x7E)


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

#define POP_BC        (op == 0b11000001)
#define POP_DE        (op == 0b11010001)
#define POP_HL        (op == 0b11100001)
#define POP_AF        (op == 0b11110001)

#define ALU_A_D8      ((op & 0b11000111) == 0b11000110)
#define RST_NN        ((op & 0b11000111) == 0b11000111)

#define PREFIX_CB     (op == 0xCB)
#define CB_QUAD       ((cb >> 6) & 3)
#define CB_ROW        ((cb >> 3) & 7)
#define CB_COL        ((cb >> 0) & 7)
#define OP_CB_R       (PREFIX_CB && CB_COL != 6)
#define OP_CB_HL      (PREFIX_CB && CB_COL == 6)

#define INTERRUPT     ((imask_ & intf_) && ime)

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

  AluOut& ao = alu_out;
  bool nb = no_branch;
  bool tb = !no_branch;
  state_ = -1;

  write = 0xFF;

  if (state == -1) {
    printf("x\n");
  }


  if (handle_int && state == 4) {
    if (imask & intf & INT_JOYPAD_MASK) { xy = 0x0060; int_ack = INT_JOYPAD_MASK; }
    if (imask & intf & INT_SERIAL_MASK) { xy = 0x0058; int_ack = INT_SERIAL_MASK; }
    if (imask & intf & INT_TIMER_MASK)  { xy = 0x0050; int_ack = INT_TIMER_MASK; }
    if (imask & intf & INT_STAT_MASK)   { xy = 0x0048; int_ack = INT_STAT_MASK; }
    if (imask & intf & INT_VBLANK_MASK) { xy = 0x0040; int_ack = INT_VBLANK_MASK; }
  }

  if (PREFIX_CB && state == 1) cb = data;
  if (LDM_A_A8  && state == 0) x = 0xFF;
  if (STM_A8_A  && state == 0) x = 0xFF;
  if (LDM_A_C   && state == 0) x = 0xFF;
  if (STM_C_A   && state == 0) x = 0xFF;

  do {
    uint8_t r = reg_get8();

#define READ(A)     { addr = A;           write = 0; read = 1; }
#define WRITE(A, B) { addr = A; data = B; write = 1; read = 0; }
#define PASS(A)     { addr = A;           write = 0; read = 0; }

    // interrupts
    if (handle_int        && state == 0) {                                                                                                                                                                   PASS(sp);       state_ = 1; break; }
    if (handle_int        && state == 1) { sp = addr - 1;                                                                                                                                                    WRITE(sp, pch); state_ = 2; break; }
    if (handle_int        && state == 2) { sp = addr - 1;                                                                                                                                                    WRITE(sp, pcl); state_ = 3; break; }
    if (handle_int        && state == 3) {                                                                                                                                                                   PASS(xy);       state_ = 4; break; }
    if (handle_int        && state == 4) {                                                                                                               ao.f = f;                                           READ(xy);       state_ = 0; break; }
    
    // single-cycle ops
    if (NOP               && state == 0) { pc = addr + 1;                                                                                                update_flags();                                     READ(pc);       state_ = 0; break; }
    if (STOP              && state == 0) { pc = addr + 1;                                                                                                update_flags();                                     READ(pc);       state_ = 0; break; }
    if (DI                && state == 0) { pc = addr + 1;                                                                                                update_flags();                                     READ(pc);       state_ = 0; break; }
    if (EI                && state == 0) { pc = addr + 1;                                                                                                update_flags();                                     READ(pc);       state_ = 0; break; }
    if (JP_HL             && state == 0) { pc = addr + 1;                                                                                                update_flags();                                     READ(hl);       state_ = 0; break; }
    if (MV_R_R            && state == 0) { pc = addr + 1;   ao = alu(0,      r,      0            );               reg_put8(ao.x);                       update_flags();                                     READ(pc);       state_ = 0; break; }
    if (DEC_R             && state == 0) { pc = addr + 1;   ao = alu(2,      r,      1            );               reg_put8(ao.x);                       update_flags();                                     READ(pc);       state_ = 0; break; }
    if (INC_R             && state == 0) { pc = addr + 1;   ao = alu(0,      r,      1            );               reg_put8(ao.x);                       update_flags();                                     READ(pc);       state_ = 0; break; }
    if (RLU_R             && state == 0) { pc = addr + 1;   ao = rlu(OP_ROW, r                    );               a = ao.x;                             update_flags();                                     READ(pc);       state_ = 0; break; }
    if (ALU_A_R           && state == 0) { pc = addr + 1;   ao = alu(OP_ROW, a,      r            );               a = ao.x;                             update_flags();                                     READ(pc);       state_ = 0; break; }

    //----------

    if (CALL_A16          && state == 0) { pc = addr + 1;                                                                                                                                                    READ(pc);       state_ = 1; break; }
    if (CALL_A16          && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                                                                                 READ(pc);       state_ = 2; break; }
    if (CALL_A16          && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                                                                                 PASS(sp);       state_ = 3; break; }
    if (CALL_A16          && state == 3) { sp = addr - 1;                                                                                                                                                    WRITE(sp, pch); state_ = 4; break; }
    if (CALL_A16          && state == 4) { sp = addr - 1;                                                                                                                                                    WRITE(sp, pcl); state_ = 5; break; }
    if (CALL_A16          && state == 5) {                                                                                                               ao.f = f;                                           READ(xy);       state_ = 0; break; }


    if (INC_AT_HL         && state == 0) { pc = addr + 1;                                                                                                                                                    READ(hl);       state_ = 1; break; }
    if (INC_AT_HL         && state == 1) {                  ao = alu(0,      data,   1            );               y = ao.x;                                                                                 WRITE(hl, y);   state_ = 2; break; }
    if (INC_AT_HL         && state == 2) {                                                                                                               update_flags();                                     READ(pc);       state_ = 0; break; }
                                                                                                                                                                                                                                  
    if (DEC_AT_HL         && state == 0) { pc = addr + 1;                                                                                                                                                    READ(hl);       state_ = 1; break; }
    if (DEC_AT_HL         && state == 1) {                  ao = alu(2,      data,   1            );               y = ao.x;                             update_flags();                                     WRITE(hl, y);   state_ = 2; break; }
    if (DEC_AT_HL         && state == 2) {                                                                                                                                                                   READ(pc);       state_ = 0; break; }
                                                                                                                                                                                                                             

    if (ADD_HL_BC         && state == 0) { pc = addr + 1;   ao = alu(0,      l,      c            );               l = ao.x;                             update_flags();                                     READ(pc);       state_ = 1; break; }
    if (ADD_HL_DE         && state == 0) { pc = addr + 1;   ao = alu(0,      l,      e            );               l = ao.x;                             update_flags();                                     READ(pc);       state_ = 1; break; }
    if (ADD_HL_HL         && state == 0) { pc = addr + 1;   ao = alu(0,      l,      l            );               l = ao.x;                             update_flags();                                     READ(pc);       state_ = 1; break; }
    if (ADD_HL_SP         && state == 0) { pc = addr + 1;   ao = alu(0,      l,      p            );               l = ao.x;                             update_flags();                                     READ(pc);       state_ = 1; break; }

    if (ADD_HL_BC         && state == 1) {                  ao = alu(1,      h,      b            );               h = ao.x;                             update_flags();                                     READ(pc);       state_ = 0; break; }
    if (ADD_HL_DE         && state == 1) {                  ao = alu(1,      h,      d            );               h = ao.x;                             update_flags();                                     READ(pc);       state_ = 0; break; }
    if (ADD_HL_HL         && state == 1) {                  ao = alu(1,      h,      h            );               h = ao.x;                             update_flags();                                     READ(pc);       state_ = 0; break; }
    if (ADD_HL_SP         && state == 1) {                  ao = alu(1,      h,      s            );               h = ao.x;                             update_flags();                                     READ(pc);       state_ = 0; break; }

    if (ADD_SP_R8         && state == 0) { pc = addr + 1;                                                                                                                                                    READ(pc);       state_ = 1; break; }
    if (ADD_SP_R8         && state == 1) { pc = addr + 1;   ao = alu(0,      p,      data         );               y = ao.x;                             update_flags();                                     PASS(pc);       state_ = 2; break; }
    if (ADD_SP_R8         && state == 2) {                  ao = alu(1,      s,      sxt(data)    );               x = ao.x;                                                                                 PASS(xy)        state_ = 3; break; }
    if (ADD_SP_R8         && state == 3) { sp = addr;                                                                                                    ao.f = f;                                           READ(pc);       state_ = 0; break; }

    if (ALU_A_D8          && state == 0) { pc = addr + 1;                                                                                                                                                    READ(pc);       state_ = 1; break; }
    if (ALU_A_D8          && state == 1) { pc = addr + 1;   ao = alu(OP_ROW, a,      data         );               a = ao.x;                             update_flags();                                     READ(pc);       state_ = 0; break; }

    if (ALU_A_HL          && state == 0) { pc = addr + 1;                                                                                                                                                    READ(hl);       state_ = 1; break; }
    if (ALU_A_HL          && state == 1) {                  ao = alu(OP_ROW, a,      data         );               a = ao.x;                             update_flags();                                     READ(pc);       state_ = 0; break; }
                                                                                                                                                                                                                                 
    if (JR_CC_R8 && tb    && state == 0) { pc = addr + 1;                                                                                                                                                    READ(pc);       state_ = 1; break; }
    if (JR_CC_R8 && tb    && state == 1) { pc = addr + 1;   ao = alu(0,      pcl,    data         );               y = ao.x;                                                                                 PASS(pc);       state_ = 2; break; }
    if (JR_CC_R8 && tb    && state == 2) {                  ao = alu(1,      pch,    sxt(data)    );               x = ao.x;                             ao.f = f;                                           READ(xy);       state_ = 0; break; }
                                                                                                                                                                                                                                  
    if (JR_CC_R8 && nb    && state == 0) { pc = addr + 1;                                                                                                                                                    READ(pc);       state_ = 1; break; }
    if (JR_CC_R8 && nb    && state == 1) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
                                                                                                                                                                                                                                  
    if (JR_R8             && state == 0) { pc = addr + 1;                                                                                                                                                    READ(pc);       state_ = 1; break; }
    if (JR_R8             && state == 1) { pc = addr + 1;   ao = alu(0,      pcl,    data         );               y = ao.x;                                                                                 PASS(pc);       state_ = 2; break; }
    if (JR_R8             && state == 2) {                  ao = alu(1,      pch,    sxt(data)    );               x = ao.x;                             ao.f = f;                                           READ(xy);       state_ = 0; break; }
                                                                    
    if (LD_HL_SP_R8       && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (LD_HL_SP_R8       && state == 1) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(pc);       state_ = 2; break; }
    if (LD_HL_SP_R8       && state == 2) {                  ao = alu(0,      p,      data         );               l = ao.x;                             update_flags();                                     PASS(pc);       state_ = 3; break; }
    if (LD_HL_SP_R8       && state == 3) {                  ao = alu(1,      s,      sxt(data)    );               h = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
                                                                                                                                                                                                                             

    if (CALL_CC_A16 && tb && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (CALL_CC_A16 && tb && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (CALL_CC_A16 && tb && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           PASS(sp);       state_ = 3; break; }
    if (CALL_CC_A16 && tb && state == 3) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, pch); state_ = 4; break; }
    if (CALL_CC_A16 && tb && state == 4) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, pcl); state_ = 5; break; }
    if (CALL_CC_A16 && tb && state == 5) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(xy);       state_ = 0; break; }
    if (CALL_CC_A16 && nb && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (CALL_CC_A16 && nb && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (CALL_CC_A16 && nb && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (RST_NN            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(sp);       state_ = 1; break; }
    if (RST_NN            && state == 1) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, pch); state_ = 2; break; }
    if (RST_NN            && state == 2) { sp = addr - 1;   ao = alu(0,      0,      0            );               x = ao.x;                             ao.f = f;                                           WRITE(sp, pcl); state_ = 3; break; }
    if (RST_NN            && state == 3) {                  ao = alu(0,      0,      op & 0x38    );               y = ao.x;                             ao.f = f;                                           READ(xy);       state_ = 0; break; }
    if (HALT && !no_halt  && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (HALT && no_halt   && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (HALT && !unhalt   && state == 1) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (HALT && unhalt    && state == 1) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (INC_BC            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(bc);       state_ = 1; break; }
    if (INC_BC            && state == 1) { bc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (INC_DE            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(de)        state_ = 1; break; }
    if (INC_DE            && state == 1) { de = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (INC_HL            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(hl);       state_ = 1; break; }
    if (INC_HL            && state == 1) { hl = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (INC_SP            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(sp);       state_ = 1; break; }
    if (INC_SP            && state == 1) { sp = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (DEC_BC            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(bc);       state_ = 1; break; }
    if (DEC_BC            && state == 1) { bc = addr - 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (DEC_DE            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(de);       state_ = 1; break; }
    if (DEC_DE            && state == 1) { de = addr - 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (DEC_HL            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(hl);       state_ = 1; break; }
    if (DEC_HL            && state == 1) { hl = addr - 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (DEC_SP            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(sp);       state_ = 1; break; }
    if (DEC_SP            && state == 1) { sp = addr - 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (JP_A16            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (JP_A16            && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (JP_A16            && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           PASS(pc);       state_ = 3; break; }
    if (JP_A16            && state == 3) {                                                                                                               ao.f = f;                                           READ(xy);       state_ = 0; break; }
    if (JP_CC_A16 && tb   && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (JP_CC_A16 && tb   && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (JP_CC_A16 && tb   && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 3; break; }
    if (JP_CC_A16 && tb   && state == 3) {                                                                                                               ao.f = f;                                           READ(xy);       state_ = 0; break; }
    if (JP_CC_A16 && nb   && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (JP_CC_A16 && nb   && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (JP_CC_A16 && nb   && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
                                                                                                                                                                                                                                  
                                                                                                                                                                                                                                  
    //----------                                                                                                                                                                                                                  
                                                                                                                                                                                                                                  
    if (LD_BC_D16         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (LD_DE_D16         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (LD_HL_D16         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (LD_SP_D16         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }

    if (LD_BC_D16         && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               c = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (LD_DE_D16         && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               e = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (LD_HL_D16         && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               l = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (LD_SP_D16         && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               p = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }

    if (LD_BC_D16         && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               b = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LD_DE_D16         && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               d = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LD_HL_D16         && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               h = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LD_SP_D16         && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               s = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }

    if (LDM_A_BC          && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(bc);       state_ = 1; break; }
    if (LDM_A_DE          && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(de);       state_ = 1; break; }
    if (LDM_A_HLP         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(hl);       state_ = 1; break; }
    if (LDM_A_HLM         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(hl);       state_ = 1; break; }

    if (LDM_A_BC          && state == 1) { bc = addr;       ao = alu(0,      0,      data         );               a = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_A_DE          && state == 1) { de = addr;       ao = alu(0,      0,      data         );               a = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_A_HLP         && state == 1) { hl = addr + 1;   ao = alu(0,      0,      data         );               a = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_A_HLM         && state == 1) { hl = addr - 1;   ao = alu(0,      0,      data         );               a = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }


    if (LD_R_D8           && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (LD_R_D8           && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               reg_put8(ao.x);                       ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LD_SP_HL          && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(hl);       state_ = 1; break; }
    if (LD_SP_HL          && state == 1) { sp = addr;                                                                                                    ao.f = f;                                           READ(pc);       state_ = 0; break; }

    if (LDM_A_A16         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (LDM_A_A16         && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (LDM_A_A16         && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           READ(xy);       state_ = 3; break; }
    if (LDM_A_A16         && state == 3) {                  ao = alu(0,      0,      data         );               a = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }

    if (LDM_R_HL          && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(hl);       state_ = 1; break; }
    if (LDM_B_HL          && state == 1) {                  ao = alu(0,      0,      data         );               b = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_C_HL          && state == 1) {                  ao = alu(0,      0,      data         );               c = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_D_HL          && state == 1) {                  ao = alu(0,      0,      data         );               d = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_E_HL          && state == 1) {                  ao = alu(0,      0,      data         );               e = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_H_HL          && state == 1) {                  ao = alu(0,      0,      data         );               h = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_L_HL          && state == 1) {                  ao = alu(0,      0,      data         );               l = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_A_HL          && state == 1) {                  ao = alu(0,      0,      data         );               a = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }

    if (POP_BC            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }
    if (POP_DE            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }
    if (POP_HL            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }
    if (POP_AF            && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }

    if (POP_BC            && state == 1) { sp = addr + 1;   ao = alu(0,      0,      data         );               c = ao.x;                             ao.f = f;                                           READ(sp);       state_ = 2; break; }
    if (POP_DE            && state == 1) { sp = addr + 1;   ao = alu(0,      0,      data         );               e = ao.x;                             ao.f = f;                                           READ(sp);       state_ = 2; break; }
    if (POP_HL            && state == 1) { sp = addr + 1;   ao = alu(0,      0,      data         );               l = ao.x;                             ao.f = f;                                           READ(sp);       state_ = 2; break; }
    if (POP_AF            && state == 1) { sp = addr + 1;   ao = alu(0,      0,      data         );               f = ao.x;                             ao.f = f;                                           READ(sp);       state_ = 2; break; }

    if (POP_BC            && state == 2) { sp = addr + 1;   ao = alu(0,      0,      data         );               b = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (POP_DE            && state == 2) { sp = addr + 1;   ao = alu(0,      0,      data         );               d = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (POP_HL            && state == 2) { sp = addr + 1;   ao = alu(0,      0,      data         );               h = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (POP_AF            && state == 2) { sp = addr + 1;   ao = alu(0,      0,      data         );               a = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }

    if (PUSH_BC           && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }
    if (PUSH_DE           && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }
    if (PUSH_HL           && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }
    if (PUSH_AF           && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }

    if (PUSH_BC           && state == 1) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, b);   state_ = 2; break; }
    if (PUSH_DE           && state == 1) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, d);   state_ = 2; break; }
    if (PUSH_HL           && state == 1) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, h);   state_ = 2; break; }
    if (PUSH_AF           && state == 1) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, a);   state_ = 2; break; }

    if (PUSH_BC           && state == 2) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, c);   state_ = 3; break; }
    if (PUSH_DE           && state == 2) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, e);   state_ = 3; break; }
    if (PUSH_HL           && state == 2) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, l);   state_ = 3; break; }
    if (PUSH_AF           && state == 2) { sp = addr - 1;                                                                                                ao.f = f;                                           WRITE(sp, f);   state_ = 3; break; }

    if (PUSH_BC           && state == 3) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (PUSH_DE           && state == 3) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (PUSH_HL           && state == 3) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (PUSH_AF           && state == 3) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }

    if (RET               && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }
    if (RET               && state == 1) { sp = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(sp);       state_ = 2; break; }
    if (RET               && state == 2) { sp = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           PASS(xy);       state_ = 3; break; }
    if (RET               && state == 3) {                                                                                                               ao.f = f;                                           READ(xy);       state_ = 0; break; }
    if (RET_CC && tb      && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           PASS(pc);       state_ = 1; break; }
    if (RET_CC && tb      && state == 1) {                                                                                                               ao.f = f;                                           READ(sp);       state_ = 2; break; }
    if (RET_CC && tb      && state == 2) { sp = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(sp);       state_ = 3; break; }
    if (RET_CC && tb      && state == 3) { sp = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           PASS(xy);       state_ = 4; break; }
    if (RET_CC && tb      && state == 4) {                                                                                                               ao.f = f;                                           READ(xy);       state_ = 0; break; }
    if (RET_CC && nb      && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (RET_CC && nb      && state == 1) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (RETI              && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(sp);       state_ = 1; break; }
    if (RETI              && state == 1) { sp = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(sp);       state_ = 2; break; }
    if (RETI              && state == 2) { sp = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           PASS(xy);       state_ = 3; break; }
    if (RETI              && state == 3) {                                                                                                               ao.f = f;                                           READ(xy);       state_ = 0; break; }
    if (STM_A16_A         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (STM_A16_A         && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (STM_A16_A         && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           WRITE(xy, a);   state_ = 3; break; }
    if (STM_A16_A         && state == 3) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_A_A8          && state == 0) { pc = addr + 1;   ao = alu(0,      0,      data         );                                                     ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (LDM_A_A8          && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(xy);       state_ = 2; break; }
    if (LDM_A_A8          && state == 2) {                  ao = alu(0,      0,      data         );               a = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (STM_A8_A          && state == 0) { pc = addr + 1;   ao = alu(0,      0,      data         );                                                     ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (STM_A8_A          && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           WRITE(xy, a);   state_ = 2; break; }
    if (STM_A8_A          && state == 2) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (LDM_A_C           && state == 0) { pc = addr + 1;   ao = alu(0,      0,      c            );               y = ao.x;                             ao.f = f;                                           READ(xy);       state_ = 1; break; }
    if (LDM_A_C           && state == 1) {                  ao = alu(0,      0,      data         );               a = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (STM_C_A           && state == 0) { pc = addr + 1;   ao = alu(0,      0,      c            );               y = ao.x;                             ao.f = f;                                           WRITE(xy, a);   state_ = 1; break; }
    if (STM_C_A           && state == 1) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (STM_HL_D8         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (STM_HL_D8         && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           WRITE(hl, y);   state_ = 2; break; }
    if (STM_HL_D8         && state == 2) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (STM_HL_R          && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           WRITE(hl, r);   state_ = 1; break; }
    if (STM_HL_R          && state == 1) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (STM_BC_A          && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           WRITE(bc, a);   state_ = 1; break; }
    if (STM_BC_A          && state == 1) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (STM_DE_A          && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           WRITE(de, a);   state_ = 1; break; }
    if (STM_DE_A          && state == 1) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (STM_HLM_A         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           WRITE(hl, a);   state_ = 1; break; }
    if (STM_HLM_A         && state == 1) { hl = addr - 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (STM_HLP_A         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           WRITE(hl, a);   state_ = 1; break; }
    if (STM_HLP_A         && state == 1) { hl = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 0; break; }
    if (STM_A16_SP        && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (STM_A16_SP        && state == 1) { pc = addr + 1;   ao = alu(0,      0,      data         );               y = ao.x;                             ao.f = f;                                           READ(pc);       state_ = 2; break; }
    if (STM_A16_SP        && state == 2) { pc = addr + 1;   ao = alu(0,      0,      data         );               x = ao.x;                             ao.f = f;                                           WRITE(xy, p);   state_ = 3; break; }
    if (STM_A16_SP        && state == 3) { xy = addr + 1;                                                                                                ao.f = f;                                           WRITE(xy, s);   state_ = 4; break; }
    if (STM_A16_SP        && state == 4) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }
    
    if (PREFIX_CB         && state == 0) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(pc);       state_ = 1; break; }
    if (OP_CB_R           && state == 1) { pc = addr + 1;   ao = alu_cb(cb,  reg_get8(CB_COL)     );               reg_put8(CB_COL, ao.x);               update_flags();                                     READ(pc);       state_ = 0; break; }
    if (OP_CB_HL          && state == 1) { pc = addr + 1;                                                                                                ao.f = f;                                           READ(hl);       state_ = 2; break; }
    if (OP_CB_HL          && state == 2) {                  ao = alu_cb(cb,  data                 );               y = ao.x;                             update_flags();                                     WRITE(hl, y);   state_ = 3; break; }
    if (OP_CB_HL          && state == 3) {                                                                                                               ao.f = f;                                           READ(pc);       state_ = 0; break; }

  } while(0);

  ao.f &= 0xF0;
  f &= 0xF0;

  if (state_ == 0 && ao.f != f) __debugbreak();

  if (DI && state_ == 0) { ime = false; ime_ = false; }
  if (EI && state_ == 0) { ime = ime_; ime_ = true;   }

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
  case 6: return f;
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
  case 6: f = reg; return;
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
