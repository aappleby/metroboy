#include "Z80.h"

#include "Constants.h"

#include <assert.h>

//-----------------------------------------------------------------------------

#define OP_ROW        ((op >> 3) & 7)
#define OP_COL        (op & 0x07)

#define NOP           (op == 0x00)


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

#define STM_BC_A      (op == 0x02)
#define STM_DE_A      (op == 0x12)
#define STM_HLP_A     (op == 0x22)
#define STM_HLM_A     (op == 0x32)
#define STM_RR_A      ((op & 0b11001111) == 0b00000010)

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

#define STM_HL_A      (op == 0x77)
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

Z80State first_state(uint8_t op);
AluOut alu_cb(const uint8_t quad, const uint8_t row, const uint8_t x, const uint8_t f);
AluOut alu(const uint8_t op, const uint8_t x, const uint8_t y, const uint8_t f);
AluOut rlu(const uint8_t op, const uint8_t x, const uint8_t f);

enum Z80State {
  DECODE = 0,

  INT0,
  INT1,
  INT2,
  INT3,
  INT4,

  HALT0,
  HALT1,

  CB0,
  CB1,

  ALU1,
  ALU2,

  PUSHN,
  PUSH0,
  PUSH1,
  PUSH2,

  POPN,
  POP0,
  POP1,
  POP2,

  ARG0,
  ARG1,
  ARG2,

  READ0,
  READ1,

  WRITE0,
  WRITE1,
  WRITE2,

  PTR0,
  PTR1,

  INVALID
};

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
  ime_ = false;
  imask_latch = 0;
  interrupt = false;
  state = DECODE;

  if (new_pc == 0x100) {
    af = 0x01B0;
    bc = 0x0013;
    de = 0x00D8;
    hl = 0x014D;
    sp = 0xFFFE;
    pc = new_pc;
    addr = new_pc - 1;
  }
  else {
    af = 0x0000;
    bc = 0x0000;
    de = 0x0000;
    hl = 0x0000;
    sp = 0x0000;
    pc = new_pc;
    addr = new_pc - 1;
  }
}









//-----------------------------------------------------------------------------
// TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0 TICK 0


CpuBus Z80::tick_t0() const {
  return { addr, data_out, false, (bool)write };
}








//-----------------------------------------------------------------------------
// TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0 TOCK 0

void Z80::tock_t0(uint8_t imask, uint8_t intf, uint8_t bus_) {
  bus = bus_;

  //----------------------------------------
  // Update interrupt & state

  ime = ime_;

  if (state == DECODE) {
    op = bus;
    int_ack_ = 0;
    imask_ = imask;
    intf_ = intf;

    if (PUSH_RR || POP_RR) {
    } else {
      pc = addr + 1;
    }

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
      state = INT0;
    }
    else {
      state = first_state(op);
    }
  }

  //----------------------------------------
  // Do the meat of executing the instruction

  AluOut& out = alu_out;
  //AluOut out = {0};
  bool nb = no_branch;
  bool tb = !no_branch;
  state_ = INVALID;

  write = 0xFF;

  if (DEC_R             && state == ALU1)   {                          out = alu(2, reg_get8(), 1, 0);                          reg_put8(OP_ROW, out.x); set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }  

  if (INC_R             && state == ALU1)   {                          out = alu(1, reg_get8(), 1, 0);                          reg_put8(OP_ROW, out.x); set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (DI                && state == ALU1)   { ime = false; ime_ = false;                                                                                 set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (EI                && state == ALU1)   { ime = ime_; ime_ = true;                                                                                   set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (JP_HL             && state == PTR1)   { pc = hl;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }

  if (MV_R_R            && state == ALU1)   {                                                                              reg_put8(OP_ROW, reg_get8()); set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (NOP               && state == ALU1)   {                                                                                                            set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (RLU_R             && state == ALU1)   {                          out = rlu(OP_ROW, reg_get8(), f);                        a = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (ALU_A_R           && state == ALU1)   {                          out = alu(OP_ROW, a, reg_get8(), f);                     a = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (ADD_HL_BC         && state == ALU1)   {                          out = alu(1, l, c, 0);                                   l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (ADD_HL_BC         && state == ALU2)   {                          out = alu(1, h, b, f);                                   h = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (ADD_HL_DE         && state == ALU1)   {                          out = alu(1, l, e, 0);                                   l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (ADD_HL_DE         && state == ALU2)   {                          out = alu(1, h, d, f);                                   h = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (ADD_HL_HL         && state == ALU1)   {                          out = alu(1, l, l, 0);                                   l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (ADD_HL_HL         && state == ALU2)   {                          out = alu(1, h, h, f);                                   h = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (ADD_HL_SP         && state == ALU1)   {                          out = alu(1, l, p, 0);                                   l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (ADD_HL_SP         && state == ALU2)   {                          out = alu(1, h, s, f);                                   h = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (ADD_SP_R8         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ALU1; }
  if (ADD_SP_R8         && state == ALU1)   {                          out = alu(0, p, bus, f);                                 p = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (ADD_SP_R8         && state == ALU2)   { pc = addr + 1;           out = alu(1, s, (bus & 0x80) ? 0xFF : 0x00, f);          s = out.x;                                addr = pc;           write = false; state_ = DECODE; }

  if (ALU_A_D8          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (ALU_A_D8          && state == ARG1)   { pc = addr + 1;           out = alu(OP_ROW, a, bus, f);                            a = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (ALU_A_HL          && state == READ0)  {                                                                                                                             addr = hl;           write = false; state_ = ALU1; }
  if (ALU_A_HL          && state == ALU1)   {                          out = alu(OP_ROW, a, bus, f);                            a = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }

  if (CALL_A16          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (CALL_A16          && state == ARG1)   { pc = addr + 1; lo = bus;                                                                                                    addr = pc;           write = false; state_ = ARG2; }
  if (CALL_A16          && state == ARG2)   { pc = addr + 1; hi = bus;                                                                                                    addr = sp;           write = false; state_ = PUSH0; }
  if (CALL_A16          && state == PUSH0)  { sp = addr - 1;                                                                    data_out = pch;                           addr = sp;           write = true;  state_ = PUSH1; }
  if (CALL_A16          && state == PUSH1)  { sp = addr - 1;                                                                    data_out = pcl;                           addr = sp;           write = true;  state_ = PUSH2; }
  if (CALL_A16          && state == PUSH2)  { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE; }

  if (CALL_CC_A16       && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (CALL_CC_A16       && state == ARG1)   { pc = addr + 1; lo = bus;                                                                                                    addr = pc;           write = false; state_ = ARG2; }
  if (CALL_CC_A16 && nb && state == ARG2)   { pc = addr + 1; hi = bus;                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (CALL_CC_A16 && tb && state == ARG2)   { pc = addr + 1; hi = bus;                                                                                                    addr = sp;           write = false; state_ = PUSH0; }
  if (CALL_CC_A16       && state == PUSH0)  { sp = addr - 1;                                                                    data_out = pch;                           addr = sp;           write = true;  state_ = PUSH1; }
  if (CALL_CC_A16       && state == PUSH1)  { sp = addr - 1;                                                                    data_out = pcl;                           addr = sp;           write = true;  state_ = PUSH2; }
  if (CALL_CC_A16       && state == PUSH2)  { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE; }

  if (RST_NN            && state == PUSHN)  {                                                                                                                             addr = sp;           write = false; state_ = PUSH0; }
  if (RST_NN            && state == PUSH0)  { sp = addr - 1;                                                                    data_out = pch;                           addr = sp;           write = true;  state_ = PUSH1; }
  if (RST_NN            && state == PUSH1)  { sp = addr - 1;                                                                    data_out = pcl;                           addr = sp;           write = true;  state_ = PUSH2; }
  if (RST_NN            && state == PUSH2)  { pc = op - 0xC7;                                                                                                             addr = pc;           write = false; state_ = DECODE; }

  if (INC_AT_HL         && state == READ0)  {                                                                                                                             addr = hl;           write = false; state_ = READ1; }
  if (INC_AT_HL         && state == READ1)  {                           out = alu(0, bus, 1, 0);                                data_out = out.x;        set_flag(out.f); addr = hl;           write = true;  state_ = WRITE1; }
  if (INC_AT_HL         && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }

  if (DEC_AT_HL         && state == READ0)  {                                                                                                                             addr = hl;           write = false; state_ = READ1; }
  if (DEC_AT_HL         && state == READ1)  {                           out = alu(2, bus, 1, 0);                                data_out = out.x;        set_flag(out.f); addr = hl;           write = true;  state_ = WRITE1; }
  if (DEC_AT_HL         && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }

  if (HALT && !no_halt  && state == HALT0)  { unhalt = 0;                                                                                                                 addr = pc;           write = false; state_ = HALT1; }
  if (HALT && !unhalt   && state == HALT1)  {                                                                                                                             addr = pc;           write = false; state_ = HALT1; }
  if (HALT && no_halt   && state == HALT0)  {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (HALT && unhalt    && state == HALT1)  {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }

  if (INC_BC            && state == PTR0)   {                                                                                                                             addr = bc;           write = false; state_ = PTR1; }
  if (DEC_BC            && state == PTR0)   {                                                                                                                             addr = bc;           write = false; state_ = PTR1; }
  if (INC_DE            && state == PTR0)   {                                                                                                                             addr = de;           write = false; state_ = PTR1; }
  if (DEC_DE            && state == PTR0)   {                                                                                                                             addr = de;           write = false; state_ = PTR1; }
  if (INC_HL            && state == PTR0)   {                                                                                                                             addr = hl;           write = false; state_ = PTR1; }
  if (DEC_HL            && state == PTR0)   {                                                                                                                             addr = hl;           write = false; state_ = PTR1; }
  if (INC_SP            && state == PTR0)   {                                                                                                                             addr = sp;           write = false; state_ = PTR1; }
  if (DEC_SP            && state == PTR0)   {                                                                                                                             addr = sp;           write = false; state_ = PTR1; }
  if (INC_BC            && state == PTR1)   { bc = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (DEC_BC            && state == PTR1)   { bc = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (INC_DE            && state == PTR1)   { de = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (DEC_DE            && state == PTR1)   { de = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (INC_HL            && state == PTR1)   { hl = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (DEC_HL            && state == PTR1)   { hl = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (INC_SP            && state == PTR1)   { sp = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (DEC_SP            && state == PTR1)   { sp = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}

  if (JP_A16            && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (JP_A16            && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (JP_A16            && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = PTR1; }
  if (JP_A16            && state == PTR1)   { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}

  if (JP_CC_A16         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (JP_CC_A16         && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (JP_CC_A16 && nb   && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (JP_CC_A16 && nb   && state == PTR1)   {                                                                                                                             addr = pc;           write = false; state_ = DECODE;}
  if (JP_CC_A16 && tb   && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = PTR1; }
  if (JP_CC_A16 && tb   && state == PTR1)   { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}
  
  if (JR_CC_R8          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ALU1; }
  if (JR_CC_R8 && nb    && state == ALU1)   { pc = addr + 1; lo = bus;                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (JR_CC_R8 && nb    && state == PTR1)   {                                                                                                                             addr = pc;           write = false; state_ = DECODE;}
  if (JR_CC_R8 && tb    && state == ALU1)   { pc = addr + 1; lo = bus; out = alu(0, pcl, lo, 0);                                pcl = out.x;                              addr = pc;           write = false; state_ = ALU2; }
  if (JR_CC_R8 && tb    && state == ALU2)   {                          out = alu(1, pch, (lo & 0x80) ? 0xFF : 0x00, alu_out.f); pch = out.x;                              addr = pc;           write = false; state_ = DECODE; }

  if (JR_R8             && state == ALU1)   { pc = addr + 1; lo = bus; out = alu(0, pcl, lo, 0);                                pcl = out.x;                              addr = pc;           write = false; state_ = ALU2; }
  if (JR_R8             && state == ALU2)   {                          out = alu(1, pch, (lo & 0x80) ? 0xFF : 0x00, alu_out.f); pch = out.x;                              addr = pc;           write = false; state_ = DECODE; }
  if (JR_R8             && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ALU1; }

  if (LD_RR_D16         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (LD_BC_D16         && state == ARG1)   { c = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LD_BC_D16         && state == ARG2)   { b = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (LD_DE_D16         && state == ARG1)   { e = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LD_DE_D16         && state == ARG2)   { d = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (LD_HL_D16         && state == ARG1)   { l = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LD_HL_D16         && state == ARG2)   { h = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (LD_SP_D16         && state == ARG1)   { p = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LD_SP_D16         && state == ARG2)   { s = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }

  if (LD_HL_SP_R8       && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ALU1; }
  if (LD_HL_SP_R8       && state == ARG1)   { pc = addr + 1; lo = bus;                                                                                                    addr = pc;           write = false; state_ = ALU1; }
  if (LD_HL_SP_R8       && state == ALU1)   { pc = addr + 1; lo = bus; out = alu(0, p, lo, f);                                  l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (LD_HL_SP_R8       && state == ALU2)   {                          out = alu(1, s, (lo & 0x80) ? 0xFF : 0x00, f);           h = out.x;                                addr = pc;           write = false; state_ = DECODE; }

  if (LDM_A_BC          && state == READ0)  {                                                                                                                             addr = bc;           write = false; state_ = READ1; }
  if (LDM_A_BC          && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (LDM_A_DE          && state == READ0)  {                                                                                                                             addr = de;           write = false; state_ = READ1; }
  if (LDM_A_DE          && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (LDM_A_HLP         && state == READ0)  {                                                                                                                             addr = hl;           write = false; state_ = READ1; }
  if (LDM_A_HLP         && state == READ1)  { hl = addr + 1; a = bus;                                                                                                     addr = pc;           write = false; state_ = DECODE; }
  if (LDM_A_HLM         && state == READ0)  {                                                                                                                             addr = hl;           write = false; state_ = READ1; }
  if (LDM_A_HLM         && state == READ1)  { hl = addr - 1; a = bus;                                                                                                     addr = pc;           write = false; state_ = DECODE; }

  if (LD_R_D8           && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (LD_R_D8           && state == ARG1)   { reg_put8(OP_ROW, bus);  pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }

  if (LD_SP_HL          && state == PTR0)   {                                                                                                                             addr = hl;           write = false; state_ = PTR1; }
  if (LD_SP_HL          && state == PTR1)   { sp = addr;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}

  if (LDM_A_A16         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (LDM_A_A16         && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LDM_A_A16         && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                      addr = temp;         write = false; state_ = READ1; }
  if (LDM_A_A16         && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }

  if (LDM_A_A8          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (LDM_A_A8          && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = 0xFF00 | bus; write = false; state_ = READ1; }
  if (LDM_A_A8          && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }

  if (LDM_A_C           && state == READ0)  {                                                                                                                             addr = 0xFF00 | c;   write = false; state_ = READ1; }
  if (LDM_A_C           && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }

  if (LDM_R_HL          && state == READ0)  {                                                                                                                             addr = hl;           write = false; state_ = READ1; }
  if (LDM_R_HL          && state == READ1)  { reg_put8(OP_ROW, bus);                                                                                                      addr = pc;           write = false; state_ = DECODE; }

  if (POP_RR            && state == POP0)   { pc = addr + 1;                                                                                                              addr = sp;           write = false; state_ = POP1; }
  if (POP_BC            && state == POP1)   { sp = addr + 1; c = bus;                                                                                                     addr = sp;           write = false; state_ = POP2;}
  if (POP_DE            && state == POP1)   { sp = addr + 1; e = bus;                                                                                                     addr = sp;           write = false; state_ = POP2;}
  if (POP_HL            && state == POP1)   { sp = addr + 1; l = bus;                                                                                                     addr = sp;           write = false; state_ = POP2;}
  if (POP_AF            && state == POP1)   { sp = addr + 1; f = bus & 0xF0;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (POP_BC            && state == POP2)   { sp = addr + 1; b = bus;                                                                                                     addr = pc;           write = false; state_ = DECODE; }
  if (POP_DE            && state == POP2)   { sp = addr + 1; d = bus;                                                                                                     addr = pc;           write = false; state_ = DECODE; }
  if (POP_HL            && state == POP2)   { sp = addr + 1; h = bus;                                                                                                     addr = pc;           write = false; state_ = DECODE; }
  if (POP_AF            && state == POP2)   { sp = addr + 1; a = bus;                                                                                                     addr = pc;           write = false; state_ = DECODE; }

  if (PUSH_BC           && state == PUSHN)  { pc = addr + 1;                                                                                                              addr = sp;           write = false; state_ = PUSH0; }
  if (PUSH_DE           && state == PUSHN)  { pc = addr + 1;                                                                                                              addr = sp;           write = false; state_ = PUSH0; }
  if (PUSH_HL           && state == PUSHN)  { pc = addr + 1;                                                                                                              addr = sp;           write = false; state_ = PUSH0; }
  if (PUSH_AF           && state == PUSHN)  { pc = addr + 1;                                                                                                              addr = sp;           write = false; state_ = PUSH0; }
  if (PUSH_BC           && state == PUSH0)  { sp = addr - 1;                                                                  data_out = b;                               addr = sp;           write = true;  state_ = PUSH1; }
  if (PUSH_DE           && state == PUSH0)  { sp = addr - 1;                                                                  data_out = d;                               addr = sp;           write = true;  state_ = PUSH1; }
  if (PUSH_HL           && state == PUSH0)  { sp = addr - 1;                                                                  data_out = h;                               addr = sp;           write = true;  state_ = PUSH1; }
  if (PUSH_AF           && state == PUSH0)  { sp = addr - 1;                                                                  data_out = a;                               addr = sp;           write = true;  state_ = PUSH1; }
  if (PUSH_BC           && state == PUSH1)  { sp = addr - 1;                                                                  data_out = c;                               addr = sp;           write = true;  state_ = PUSH2; }
  if (PUSH_DE           && state == PUSH1)  { sp = addr - 1;                                                                  data_out = e;                               addr = sp;           write = true;  state_ = PUSH2; }
  if (PUSH_HL           && state == PUSH1)  { sp = addr - 1;                                                                  data_out = l;                               addr = sp;           write = true;  state_ = PUSH2; }
  if (PUSH_AF           && state == PUSH1)  { sp = addr - 1;                                                                  data_out = f;                               addr = sp;           write = true;  state_ = PUSH2; }
  if (PUSH_RR           && state == PUSH2)  {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }

  if (RET               && state == POP0)   {                                                                                                                             addr = sp;           write = false; state_ = POP1; }
  if (RET               && state == POP1)   { sp = addr + 1; lo = bus;                                                                                                    addr = sp;           write = false; state_ = POP2;}
  if (RET               && state == POP2)   { sp = addr + 1; hi = bus;                                                                                                    addr = pc;           write = false; state_ = PTR1; }
  if (RET               && state == PTR1)   { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}

  if (RET_CC            && state == POPN)   { /* flag test go here? */                                                                                                    addr = pc;           write = false; state_ = POP0; }
  if (RET_CC && nb      && state == POP0)   {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (RET_CC && tb      && state == POP0)   {                                                                                                                             addr = sp;           write = false; state_ = POP1; }
  if (RET_CC            && state == POP1)   { lo = bus;       sp = addr + 1;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (RET_CC            && state == POP2)   { hi = bus;       sp = addr + 1;                                                                                              addr = pc;           write = false; state_ = PTR1; }
  if (RET_CC && nb      && state == PTR1)   {                                                                                                                             addr = pc;           write = false; state_ = DECODE;}
  if (RET_CC && tb      && state == PTR1)   { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}

  if (RETI              && state == POP0)   {                                                                                                                             addr = sp;           write = false; state_ = POP1; }
  if (RETI              && state == POP1)   { lo = bus;       sp = addr + 1;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (RETI              && state == POP2)   { hi = bus;       sp = addr + 1;                                                                                              addr = pc;           write = false; state_ = PTR1; }
  if (RETI              && state == PTR1)   { ime = true; ime_ = true; pc = temp;                                                                                         addr = pc;           write = false; state_ = DECODE;}


  if (STM_A16_A         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (STM_A16_A         && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (STM_A16_A         && state == ARG2)   { hi = bus; data_out = a; pc = addr + 1;                                                                                      addr = temp;         write = true;  state_ = WRITE1; }
  if (STM_A16_A         && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }

  if (STM_A8_A          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (STM_A8_A          && state == ARG1)   { data_out = a;           pc = addr + 1;                                                                                      addr = 0xFF00 | bus; write = true;  state_ = WRITE1; }
  if (STM_A8_A          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_C_A           && state == WRITE0) { data_out = a;                                                                                                               addr = 0xFF00 | c;   write = true;  state_ = WRITE1;}
  if (STM_C_A           && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_HL_A          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_HL_D8         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (STM_HL_D8         && state == ARG1)   { data_out = bus;         pc = addr + 1;                                                                                      addr = hl;           write = true;  state_ = WRITE1; }
  if (STM_HL_D8         && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_HL_R          && state == WRITE0) { data_out = reg_get8();                                                                                                      addr = hl;           write = true;  state_ = WRITE1;}
  if (STM_HL_R          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }

  if (STM_BC_A          && state == WRITE0) { data_out = a;                                                                                                               addr = bc;           write = true;  state_ = WRITE1;}
  if (STM_BC_A          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_DE_A          && state == WRITE0) { data_out = a;                                                                                                               addr = de;           write = true;  state_ = WRITE1;}
  if (STM_DE_A          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_HLM_A         && state == WRITE0) { data_out = a;                                                                                                               addr = hl;           write = true;  state_ = WRITE1;}
  if (STM_HLM_A         && state == WRITE1) { hl = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE; }
  if (STM_HLP_A         && state == WRITE0) { data_out = a;                                                                                                               addr = hl;           write = true;  state_ = WRITE1;}
  if (STM_HLP_A         && state == WRITE1) { hl = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE; }

  if (STM_A16_SP        && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (STM_A16_SP        && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (STM_A16_SP        && state == ARG2)   { hi = bus; data_out = p; pc = addr + 1;                                                                                      addr = temp;         write = true;  state_ = WRITE1; }
  if (STM_A16_SP        && state == WRITE1) { data_out = s; temp = addr + 1;                                                                                              addr = temp;         write = true;  state_ = WRITE2; }
  if (STM_A16_SP        && state == WRITE2) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }

  if (PREFIX_CB) {
    if (OP_CB_R           && state == CB0)    {                                                                                                                           addr = pc;           write = false; state_ = CB1; }
    if (OP_CB_HL          && state == CB0)    {                                                                                                                           addr = pc;           write = false; state_ = CB1; }

    if (state == CB1) {
      if (OP_CB_R)  cb = bus;
      if (OP_CB_HL) cb = bus;
    }

    if (OP_CB_R           && state == CB1)    { pc = addr + 1; out = alu_cb(CB_QUAD, CB_ROW, reg_get8(CB_COL), f);             reg_put8(CB_COL, out.x); set_flag(out.f);  addr = pc;            write = false; state_ = DECODE; }
    if (OP_CB_HL          && state == CB1)    { pc = addr + 1;                                                                                                            addr = hl;            write = false; state_ = READ1; }
    if (OP_CB_HL          && state == READ1)  {                out = alu_cb(CB_QUAD, CB_ROW, reg_get8(CB_COL), f);             reg_put8(CB_COL, out.x); set_flag(out.f);  addr = hl;            write = true;  state_ = WRITE1; }
    if (OP_CB_HL          && state == WRITE1) {                                                                                                                           addr = pc;            write = false; state_ = DECODE; }
  }

#if 0
  if (OP_CB_R           && state == CB0)    {                                                                                                                             addr = pc;           write = false; state_ = CB1; }
  if (OP_CB_HL          && state == CB0)    {                                                                                                                             addr = pc;           write = false; state_ = CB1; }
  if (OP_CB_R           && state == CB1)    { pc = addr + 1; out = alu_cb(CB_QUAD, CB_ROW, r, f);                               reg_put8(CB_COL, out.x); set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (OP_CB_HL          && state == CB1)    { pc = addr + 1;                                                                                                              addr = hl;           write = false; state_ = READ1; }
  if (NOP               && state == ALU1)   {                                                                                                            set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (DI                && state == ALU1)   { ime = false; ime_ = false;                                                                            set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (EI                && state == ALU1)   { ime = ime_; ime_ = true;                                                                         set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (MV_R_R            && state == ALU1)   {                                                                                   reg_put8(OP_ROW, r);     set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (ALU_A_R           && state == ALU1)   {                          out = alu(OP_ROW, a, r, f);                              a = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (ALU_A_HL          && state == ALU1)   {                          out = alu(OP_ROW, a, bus, f);                            a = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (RLU_R             && state == ALU1)   {                          out = rlu(OP_ROW, r, f);                                 a = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (INC_R             && state == ALU1)   {                          out = alu(1, r, 1, 0);                                   reg_put8(OP_ROW, out.x); set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (DEC_R             && state == ALU1)   {                          out = alu(2, r, 1, 0);                                   reg_put8(OP_ROW, out.x); set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }  
  if (ADD_HL_BC         && state == ALU1)   {                          out = alu(1, l, c, 0);                                   l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (ADD_HL_DE         && state == ALU1)   {                          out = alu(1, l, e, 0);                                   l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (ADD_HL_HL         && state == ALU1)   {                          out = alu(1, l, l, 0);                                   l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (ADD_HL_SP         && state == ALU1)   {                          out = alu(1, l, p, 0);                                   l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (ADD_SP_R8         && state == ALU1)   { pc = addr + 1; lo = bus; out = alu(0, p, lo, f);                                  p = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (LD_HL_SP_R8       && state == ALU1)   { pc = addr + 1; lo = bus; out = alu(0, p, lo, f);                                  l = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = ALU2; }
  if (JR_R8             && state == ALU1)   { pc = addr + 1; lo = bus; out = alu(0, pcl, lo, 0);                                pcl = out.x;                              addr = pc;           write = false; state_ = ALU2; }
  if (JR_CC_R8 && tb    && state == ALU1)   { pc = addr + 1; lo = bus; out = alu(0, pcl, lo, 0);                                pcl = out.x;                              addr = pc;           write = false; state_ = ALU2; }
  if (JR_CC_R8 && nb    && state == ALU1)   { pc = addr + 1; lo = bus;                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (ADD_HL_BC         && state == ALU2)   {                          out = alu(1, h, b, f);                                   h = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (ADD_HL_DE         && state == ALU2)   {                          out = alu(1, h, d, f);                                   h = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (ADD_HL_HL         && state == ALU2)   {                          out = alu(1, h, h, f);                                   h = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (ADD_HL_SP         && state == ALU2)   {                          out = alu(1, h, s, f);                                   h = out.x;               set_flag(out.f); addr = pc;           write = false; state_ = DECODE; }
  if (ADD_SP_R8         && state == ALU2)   {                          out = alu(1, s, (lo & 0x80) ? 0xFF : 0x00, f);           s = out.x;                                addr = pc;           write = false; state_ = DECODE; }
  if (LD_HL_SP_R8       && state == ALU2)   {                          out = alu(1, s, (lo & 0x80) ? 0xFF : 0x00, f);           h = out.x;                                addr = pc;           write = false; state_ = DECODE; }
  if (JR_R8             && state == ALU2)   {                          out = alu(1, pch, (lo & 0x80) ? 0xFF : 0x00, alu_out.f); pch = out.x;                              addr = pc;           write = false; state_ = DECODE; }
  if (JR_CC_R8 && tb    && state == ALU2)   {                          out = alu(1, pch, (lo & 0x80) ? 0xFF : 0x00, alu_out.f); pch = out.x;                              addr = pc;           write = false; state_ = DECODE; }
  if (HALT && no_halt   && state == HALT0)  {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (HALT && !no_halt  && state == HALT0)  { unhalt = 0;                                                                                                                 addr = pc;           write = false; state_ = HALT1; }
  if (HALT && unhalt    && state == HALT1)  {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (HALT && !unhalt   && state == HALT1)  {                                                                                                                             addr = pc;           write = false; state_ = HALT1; }
  if (PUSH_BC           && state == PUSHN)  {                                                                                                                             addr = sp;           write = false; state_ = PUSH0; }
  if (PUSH_DE           && state == PUSHN)  {                                                                                                                             addr = sp;           write = false; state_ = PUSH0; }
  if (PUSH_HL           && state == PUSHN)  {                                                                                                                             addr = sp;           write = false; state_ = PUSH0; }
  if (PUSH_AF           && state == PUSHN)  {                                                                                                                             addr = sp;           write = false; state_ = PUSH0; }
  if (RST_NN            && state == PUSHN)  {                                                                                                                             addr = sp;           write = false; state_ = PUSH0; }
  if (PUSH_BC           && state == PUSH0)  { data_out = b;   sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH1; }
  if (PUSH_DE           && state == PUSH0)  { data_out = d;   sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH1; }
  if (PUSH_HL           && state == PUSH0)  { data_out = h;   sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH1; }
  if (PUSH_AF           && state == PUSH0)  { data_out = a;   sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH1; }
  if (RST_NN            && state == PUSH0)  { data_out = pch; sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH1; }
  if (CALL_A16          && state == PUSH0)  { data_out = pch; sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH1; }
  if (CALL_CC_A16       && state == PUSH0)  { data_out = pch; sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH1; }
  if (PUSH_BC           && state == PUSH1)  { data_out = c;   sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH2; }
  if (PUSH_DE           && state == PUSH1)  { data_out = e;   sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH2; }
  if (PUSH_HL           && state == PUSH1)  { data_out = l;   sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH2; }
  if (PUSH_AF           && state == PUSH1)  { data_out = f;   sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH2; }
  if (RST_NN            && state == PUSH1)  { data_out = pcl; sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH2; }
  if (CALL_A16          && state == PUSH1)  { data_out = pcl; sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH2; }
  if (CALL_CC_A16       && state == PUSH1)  { data_out = pcl; sp = addr - 1;                                                                                              addr = sp;           write = true;  state_ = PUSH2; }
  if (PUSH_RR           && state == PUSH2)  {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (RST_NN            && state == PUSH2)  { pc = op - 0xC7;                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (CALL_A16          && state == PUSH2)  { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE; }
  if (CALL_CC_A16       && state == PUSH2)  { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE; }
  if (RET_CC            && state == POPN)   { /* flag test go here? */                                                                                                    addr = pc;           write = false; state_ = POP0; }
  if (RET_CC && nb      && state == POP0)   {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (RET               && state == POP0)   {                                                                                                                             addr = sp;           write = false; state_ = POP1; }
  if (RETI              && state == POP0)   {                                                                                                                             addr = sp;           write = false; state_ = POP1; }
  if (POP_RR            && state == POP0)   {                                                                                                                             addr = sp;           write = false; state_ = POP1; }
  if (RET_CC && tb      && state == POP0)   {                                                                                                                             addr = sp;           write = false; state_ = POP1; }
  if (POP_BC            && state == POP1)   { c = bus;        sp = addr + 1;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (POP_DE            && state == POP1)   { e = bus;        sp = addr + 1;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (POP_HL            && state == POP1)   { l = bus;        sp = addr + 1;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (POP_AF            && state == POP1)   { f = bus & 0xF0; sp = addr + 1;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (RET               && state == POP1)   { lo = bus;       sp = addr + 1;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (RETI              && state == POP1)   { lo = bus;       sp = addr + 1;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (RET_CC            && state == POP1)   { lo = bus;       sp = addr + 1;                                                                                              addr = sp;           write = false; state_ = POP2;}
  if (POP_BC            && state == POP2)   { b = bus;        sp = addr + 1;                                                                                              addr = pc;           write = false; state_ = DECODE; }
  if (POP_DE            && state == POP2)   { d = bus;        sp = addr + 1;                                                                                              addr = pc;           write = false; state_ = DECODE; }
  if (POP_HL            && state == POP2)   { h = bus;        sp = addr + 1;                                                                                              addr = pc;           write = false; state_ = DECODE; }
  if (POP_AF            && state == POP2)   { a = bus;        sp = addr + 1;                                                                                              addr = pc;           write = false; state_ = DECODE; }
  if (RET               && state == POP2)   { hi = bus;       sp = addr + 1;                                                                                                                   write = false; state_ = PTR1; }
  if (RETI              && state == POP2)   { hi = bus;       sp = addr + 1;                                                                                                                   write = false; state_ = PTR1; }
  if (RET_CC            && state == POP2)   { hi = bus;       sp = addr + 1;                                                                                                                   write = false; state_ = PTR1; }
  if (LD_R_D8           && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (LDM_A_A8          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (STM_HL_D8         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (STM_A8_A          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (ALU_A_D8          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (LDM_A_A16         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (LD_RR_D16         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (STM_A16_A         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (STM_A16_SP        && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (JP_A16            && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (JP_CC_A16         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (CALL_A16          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (CALL_CC_A16       && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ARG1; }
  if (JR_R8             && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ALU1; }
  if (JR_CC_R8          && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ALU1; }
  if (LD_HL_SP_R8       && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ALU1; }
  if (ADD_SP_R8         && state == ARG0)   {                                                                                                                             addr = pc;           write = false; state_ = ALU1; }
  if (LD_R_D8           && state == ARG1)   { reg_put8(OP_ROW, bus);  pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (ALU_A_D8          && state == ARG1)   { out = alu(OP_ROW, a, bus, f); a = out.x; set_flag(out.f);                     pc = addr + 1;                                addr = pc;           write = false; state_ = DECODE; }
  if (LD_BC_D16         && state == ARG1)   { c = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LD_DE_D16         && state == ARG1)   { e = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LD_HL_D16         && state == ARG1)   { l = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LD_SP_D16         && state == ARG1)   { p = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (JP_A16            && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (JP_CC_A16         && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (CALL_A16          && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (CALL_CC_A16       && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (STM_A16_A         && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (STM_A16_SP        && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LDM_A_A16         && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = ARG2; }
  if (LD_HL_SP_R8       && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                                           write = false; state_ = ALU1; }
  if (LDM_A_A8          && state == ARG1)   { lo = bus;               pc = addr + 1;                                                                                      addr = 0xFF00 | bus; write = false; state_ = READ1; }
  if (STM_A8_A          && state == ARG1)   { data_out = a;           pc = addr + 1;                                                                                      addr = 0xFF00 | bus; write = true;  state_ = WRITE1; }
  if (STM_HL_D8         && state == ARG1)   { data_out = bus;         pc = addr + 1;                                                                                      addr = hl;           write = true;  state_ = WRITE1; }
  if (LD_BC_D16         && state == ARG2)   { b = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (LD_DE_D16         && state == ARG2)   { d = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (LD_HL_D16         && state == ARG2)   { h = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (LD_SP_D16         && state == ARG2)   { s = bus;                pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (JP_CC_A16 && nb   && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (CALL_CC_A16 && nb && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (CALL_A16          && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                      addr = sp;           write = false; state_ = PUSH0; }
  if (CALL_CC_A16 && tb && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                      addr = sp;           write = false; state_ = PUSH0; }
  if (LDM_A_A16         && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                      addr = temp;         write = false; state_ = READ1; }
  if (STM_A16_A         && state == ARG2)   { hi = bus; data_out = a; pc = addr + 1;                                                                                      addr = temp;         write = true;  state_ = WRITE1; }
  if (STM_A16_SP        && state == ARG2)   { hi = bus; data_out = p; pc = addr + 1;                                                                                      addr = temp;         write = true;  state_ = WRITE1; }
  if (JP_A16            && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                                           write = false; state_ = PTR1; }
  if (JP_CC_A16 && tb   && state == ARG2)   { hi = bus;               pc = addr + 1;                                                                                                           write = false; state_ = PTR1; }
  if (ALU_A_HL          && state == READ0)  {                         pc = addr + 1;                                                                                      addr = hl;           write = false; state_ = ALU1; }
  if (INC_AT_HL         && state == READ0)  {                         pc = addr + 1;                                                                                      addr = hl;           write = false; state_ = READ1; }
  if (DEC_AT_HL         && state == READ0)  {                         pc = addr + 1;                                                                                      addr = hl;           write = false; state_ = READ1; }
  if (LDM_R_HL          && state == READ0)  {                         pc = addr + 1;                                                                                      addr = hl;           write = false; state_ = READ1; }
  if (LDM_A_BC          && state == READ0)  {                         pc = addr + 1;                                                                                      addr = bc;           write = false; state_ = READ1; }
  if (LDM_A_DE          && state == READ0)  {                         pc = addr + 1;                                                                                      addr = de;           write = false; state_ = READ1; }
  if (LDM_A_HLP         && state == READ0)  {                         pc = addr + 1;                                                                                      addr = hl;           write = false; state_ = READ1; }
  if (LDM_A_HLM         && state == READ0)  {                         pc = addr + 1;                                                                                      addr = hl;           write = false; state_ = READ1; }
  if (LDM_A_C           && state == READ0)  {                         pc = addr + 1;                                                                                      addr = 0xFF00 | c;   write = false; state_ = READ1; }
  if (LDM_A_BC          && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (LDM_A_DE          && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (LDM_A_HLP         && state == READ1)  { a = bus; hl = addr + 1;                                                                                                     addr = pc;           write = false; state_ = DECODE; }
  if (LDM_A_HLM         && state == READ1)  { a = bus; hl = addr - 1;                                                                                                     addr = pc;           write = false; state_ = DECODE; }
  if (LDM_A_A8          && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (LDM_A_C           && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (LDM_A_A16         && state == READ1)  { a = bus;                                                                                                                    addr = pc;           write = false; state_ = DECODE; }
  if (LDM_R_HL          && state == READ1)  { reg_put8(OP_ROW, bus);                                                                                                      addr = pc;           write = false; state_ = DECODE; }
  if (INC_AT_HL         && state == READ1)  { out = alu(0, bus, 1, 0);                set_flag(out.f); data_out = out.x;                                                  addr = hl;           write = true;  state_ = WRITE1; }
  if (DEC_AT_HL         && state == READ1)  { out = alu(2, bus, 1, 0);                set_flag(out.f); data_out = out.x;                                                  addr = hl;           write = true;  state_ = WRITE1; }
  if (OP_CB_HL          && state == READ1)  { out = alu_cb(CB_QUAD, CB_ROW, bus, f);  set_flag(out.f); data_out = out.x;                                                  addr = hl;           write = true;  state_ = WRITE1; }
  if (STM_BC_A          && state == WRITE0) { data_out = a;                                                                                                               addr = bc;           write = true;  state_ = WRITE1;}
  if (STM_DE_A          && state == WRITE0) { data_out = a;                                                                                                               addr = de;           write = true;  state_ = WRITE1;}
  if (STM_HLP_A         && state == WRITE0) { data_out = a;                                                                                                               addr = hl;           write = true;  state_ = WRITE1;}
  if (STM_HLM_A         && state == WRITE0) { data_out = a;                                                                                                               addr = hl;           write = true;  state_ = WRITE1;}
  if (STM_HL_R          && state == WRITE0) { data_out = r;                                                                                                               addr = hl;           write = true;  state_ = WRITE1;}
  if (STM_C_A           && state == WRITE0) { data_out = a;                                                                                                               addr = 0xFF00 | c;   write = true;  state_ = WRITE1;}
  if (STM_A16_SP        && state == WRITE1) { data_out = s; temp = addr + 1;                                                                                              addr = temp;         write = true;  state_ = WRITE2; }
  if (STM_A16_A         && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_A8_A          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_C_A           && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_HL_A          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_BC_A          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_DE_A          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_HLP_A         && state == WRITE1) { hl = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE; }
  if (STM_HLM_A         && state == WRITE1) { hl = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE; }
  if (STM_HL_R          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_HL_D8         && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (INC_AT_HL         && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (DEC_AT_HL         && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (OP_CB_HL          && state == WRITE1) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (STM_A16_SP        && state == WRITE2) {                                                                                                                             addr = pc;           write = false; state_ = DECODE; }
  if (INC_BC            && state == PTR0)   {                                                                                                                             addr = bc;           write = false; state_ = PTR1; }
  if (INC_DE            && state == PTR0)   {                                                                                                                             addr = de;           write = false; state_ = PTR1; }
  if (INC_HL            && state == PTR0)   {                                                                                                                             addr = hl;           write = false; state_ = PTR1; }
  if (INC_SP            && state == PTR0)   {                                                                                                                             addr = sp;           write = false; state_ = PTR1; }
  if (DEC_BC            && state == PTR0)   {                                                                                                                             addr = bc;           write = false; state_ = PTR1; }
  if (DEC_DE            && state == PTR0)   {                                                                                                                             addr = de;           write = false; state_ = PTR1; }
  if (DEC_HL            && state == PTR0)   {                                                                                                                             addr = hl;           write = false; state_ = PTR1; }
  if (DEC_SP            && state == PTR0)   {                                                                                                                             addr = sp;           write = false; state_ = PTR1; }
  if (LD_SP_HL          && state == PTR0)   {                                                                                                                             addr = hl;           write = false; state_ = PTR1; }
  if (JP_HL             && state == PTR1)   { pc = hl;                                                                                                                    addr = pc;           write = false; state_ = DECODE;}
  if (RET               && state == PTR1)   { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}
  if (RETI              && state == PTR1)   { ime = true; ime_ = true; pc = temp;                                                                                    addr = pc;           write = false; state_ = DECODE;}
  if (RET_CC && tb      && state == PTR1)   { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}
  if (JP_A16            && state == PTR1)   { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}
  if (JP_CC_A16 && tb   && state == PTR1)   { pc = temp;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}
  if (INC_BC            && state == PTR1)   { bc = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (INC_DE            && state == PTR1)   { de = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (INC_HL            && state == PTR1)   { hl = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (INC_SP            && state == PTR1)   { sp = addr + 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (DEC_BC            && state == PTR1)   { bc = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (DEC_DE            && state == PTR1)   { de = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (DEC_HL            && state == PTR1)   { hl = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (DEC_SP            && state == PTR1)   { sp = addr - 1;                                                                                                              addr = pc;           write = false; state_ = DECODE;}
  if (LD_SP_HL          && state == PTR1)   { sp = addr;                                                                                                                  addr = pc;           write = false; state_ = DECODE;}
  if (RET_CC && nb      && state == PTR1)   {                                                                                                                             addr = pc;           write = false; state_ = DECODE;}
  if (JP_CC_A16 && nb   && state == PTR1)   {                                                                                                                             addr = pc;           write = false; state_ = DECODE;}
  if (JR_CC_R8 && nb    && state == PTR1)   {                                                                                                                             addr = pc;           write = false; state_ = DECODE;}
#endif

  switch (state) {

  //----------
  // interrupts are probably totally broken, run some microtests later

  case INT0:
    if (interrupt) { addr = pc; write = false; state_ = INT1; }
    else printf("fail int0");
    break;

  case INT1:
    if (interrupt) { addr = sp; write = false; state_ = INT2; }
    else printf("fail int1");
    break;

  case INT2:
    if (interrupt) { addr = sp; data_out = pch; write = true; state_ = INT3; }
    else printf("fail int2");
    break;

  case INT3:
    // gameboy interrupt quirk thingy
    imask_latch = imask_;
    if (interrupt) { sp = addr - 1; data_out = pcl; addr = sp; write = true; state_ = INT4; }
    else printf("fail int3");
    break;

  case INT4: {
    sp = addr - 1;

    // move to int3?
    uint8_t interrupts = imask_latch & intf_;
    int vector = -1;
    if (interrupts & INT_JOYPAD) vector = 4; // joypad
    if (interrupts & INT_SERIAL) vector = 3; // serial
    if (interrupts & INT_TIMER)  vector = 2; // timer
    if (interrupts & INT_STAT)   vector = 1; // lcd stat
    if (interrupts & INT_VBLANK) vector = 0; // vblank

    // Someone could've changed the interrupt mask or flags while we were
    // handling the interrupt, so we have to compute the new PC at the very
    // last second.
    if (vector >= 0) {
      int_ack_ = 1 << vector;
      pc = uint16_t(0x0040 + (vector << 3));
    } else {
      pc = 0x0000;
    }
    ime = false;
    ime_ = false;

    addr = pc;
    write = false;
    state_ = DECODE;
    break;
  }
  }

  if (write == 0xFF) printf("write fail");
  if (state_ == INVALID) printf("fail state invalid");
}











//-----------------------------------------------------------------------------
// TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2 TICK 2

CpuBus Z80::tick_t2() const {
  return { addr, data_out, true, false };
}










//-----------------------------------------------------------------------------
// TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2 TOCK 2

void Z80::tock_t2() {
  state = state_;
  cycle++;
}













void Z80::set_flag(uint8_t f_) {
  uint8_t mask = PREFIX_CB ? cb_flag_mask[CB_QUAD] : flag_mask[op];
  f = (f & ~mask) | (f_ & mask);

  // RLCA, RRCA, RLA, and RRA always clear the zero bit - hardware bug?
  if ((op & 0b11100111) == 0b00000111) f &= ~F_ZERO;

  // ADD_SP_R8 and LD_HL_SP_R8 always clear the zero bit and negative bit.
  if ((op & 0b11101111) == 0b11101000) f &= ~(F_ZERO | F_NEGATIVE);
}







//-----------------------------------------------------------------------------

Z80State first_state(uint8_t op) {
  if (PREFIX_CB)   return CB0;

  if (HALT)        return HALT0;

  if (NOP)         return ALU1;
  if (ALU_A_R)     return ALU1;
  if (INC_R)       return ALU1;
  if (DEC_R)       return ALU1;
  if (RLU_R)       return ALU1;
  if (MV_R_R)      return ALU1;
  if (ADD_HL_RR)   return ALU1;
  if (DI)          return ALU1;
  if (EI)          return ALU1;

  if (PUSH_RR)     return PUSHN;
  if (RST_NN)      return PUSHN;

  if (RET)         return POP0;
  if (RETI)        return POP0;
  if (RET_CC)      return POPN; // ret_cc has an extra delay cycle before pop
  if (POP_RR)      return POP0;

  if (LD_R_D8)     return ARG0;
  if (LDM_A_A8)    return ARG0;
  if (STM_HL_D8)   return ARG0;
  if (JR_CC_R8)    return ARG0;
  if (JR_R8)       return ARG0;
  if (LD_HL_SP_R8) return ARG0;
  if (STM_A8_A)    return ARG0;
  if (ALU_A_D8)    return ARG0;
  if (ADD_SP_R8)   return ARG0;
  if (LDM_A_A16)   return ARG0;
  if (LD_RR_D16)   return ARG0;
  if (STM_A16_A)   return ARG0;
  if (STM_A16_SP)  return ARG0;
  if (JP_A16)      return ARG0;
  if (JP_CC_A16)   return ARG0;
  if (CALL_A16)    return ARG0;
  if (CALL_CC_A16) return ARG0;

  if (ALU_A_HL)    return READ0;
  if (INC_AT_HL)   return READ0;
  if (DEC_AT_HL)   return READ0;
  if (LDM_R_HL)    return READ0;
  if (LDM_A_RR)    return READ0;
  if (LDM_A_C)     return READ0;

  if (STM_HL_R)    return WRITE0;
  if (STM_RR_A)    return WRITE0;
  if (STM_C_A)     return WRITE0;

  if (INC_RR)      return PTR0;
  if (DEC_RR)      return PTR0;
  if (LD_SP_HL)    return PTR0;
  if (JP_HL)       return PTR1;

  printf("fail first_state");
  return INVALID;
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

uint8_t Z80::reg_get8(int mux) const {
  switch(mux) {
  case 0: return b;
  case 1: return c;
  case 2: return d;
  case 3: return e;
  case 4: return h;
  case 5: return l;
  case 6: return bus;
  case 7: return a;
  }
  printf("reg_get8 fail");
  return 0;
}

uint8_t Z80::reg_get8() const {
  int mux = (op & 0b11000000) ? OP_COL : OP_ROW;
  if (RLU_R) mux = OP_COL;
  return reg_get8(mux);
}

void Z80::reg_put8(int mux, uint8_t reg) {
  switch (mux) {
  case 0: b = reg; break;
  case 1: c = reg; break;
  case 2: d = reg; break;
  case 3: e = reg; break;
  case 4: h = reg; break;
  case 5: l = reg; break;
  case 6: data_out = reg; break;  // can probably merge with bus reg later
  case 7: a = reg; break;
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

__declspec(noinline) AluOut alu(const uint8_t op, const uint8_t x, const uint8_t y, const uint8_t f) {
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

__declspec(noinline) AluOut alu(const uint8_t op, const uint8_t x, const uint8_t y, const uint8_t f) {
  uint16_t d1 = 0, d2 = 0, c = ((f >> 4) & 1);

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
  sprintf(out, "ime_ %d\n", ime_);
}

//-----------------------------------------------------------------------------
