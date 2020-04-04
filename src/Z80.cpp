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

#define ALU_A_D8      ((OP_QUAD == 3) && (OP_COL == 6))
#define RST_NN        ((OP_QUAD == 3) && (OP_COL == 7))

uint8_t  sxt(uint8_t x) { return x & 0x80 ? 0xFF : 0x00; }

//-----------------------------------------------------------------------------                                                            

void Z80::reset(uint16_t new_pc) {

  if (new_pc == 0x100) {
    unhalt = 0;
    ime = ime_delay = 0;
    interrupt = 0;

    state = 0;
    read = 0;
    write = 0;
    alu_out = {0};
    int_ack = 0;

    pc = 0xFF;
    op_addr = pc;
    op = 0;
    cb = 0;
    addr = 0xFF;
    data = 0;
    state = 0;

    bc = 0x0013;
    de = 0x00D8;
    hl = 0x014D;
    af = 0x01B0;
    sp = 0xFFFE;
    xy = 0x0000;
  }
  else {
    unhalt = 0;
    ime = ime_delay = 0;
    interrupt = 0;

    state = 0;
    read = 0;
    write = 0;
    alu_out = {0};
    int_ack = 0;

    pc = 0xFFFF;
    op_addr = pc;
    op = 0;
    cb = 0;
    addr = pc;
    state = 0;

    bc = 0x0000;
    de = 0x0000;
    hl = 0x0000;
    af = 0x0000;
    sp = 0x0000;
    xy = 0x0000;
  }
}

//-----------------------------------------------------------------------------

Req Z80::get_bus_req_t30() const {
  return {
    .addr  = addr,
    .data  = data,
    .read  = (bool)read,
    .write = (bool)write,
  };
}

void Z80::on_bus_ack_t01(Ack ibus_ack_) {
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

void Z80::tock_t30(const uint8_t imask, const uint8_t intf) {
  alu_out.f &= 0xF0;
  f &= 0xF0;

  if (state_ == -1) __debugbreak();

  state = state_;
  ime = ime_delay;

  if (state == 0) {
    op_addr = addr;
    alu_out.f = f;
  }

  if (state == 0) {
    op = data;
    int_ack = 0;
    interrupt = (imask & intf) && ime;
  }

  read = 0;
  write = 0;

  state_ = -1;
}

//-----------------------------------------------------------------------------

void Z80::tock_t01(const uint8_t imask, const uint8_t intf) {

  //----------------------------------------
  // INTERRUPTS

  if (interrupt) {
    if (state == 0) {
      ime = false;
      ime_delay = false;
    }
    else if (state == 1) { sp = addr - 1; }
    else if (state == 2) { sp = addr - 1; }
    else if (state == 3) {                }
    else if (state == 4) {
      if      (imask & intf & INT_JOYPAD_MASK) {  }
      else if (imask & intf & INT_SERIAL_MASK) {  }
      else if (imask & intf & INT_TIMER_MASK)  {  }
      else if (imask & intf & INT_STAT_MASK)   {  }
      else if (imask & intf & INT_VBLANK_MASK) {  }
      else                                     {  }
    }
  }

  //----------------------------------------
  // CB OPS

  else if (PREFIX_CB) {
    AluOut& ao = alu_out;

    if      (state == 0)       { pc = addr + 1; }
    else if (state == 1) {
      cb = data;
      if (OP_CB_R  )           { pc = addr + 1; }
      if (OP_CB_HL )           { pc = addr + 1; }
    }
    else if (state == 2)       {                }
    else if (state == 3)       {                }
  }

  //----------------------------------------
  // REGULAR OPS

  else {
    AluOut& ao = alu_out;

    //----------------------------------------
    // QUAD 0

    if (OP_QUAD == 0) {
      if (OP_COL == 0) {
        if (NOP)                     { pc = addr + 1; }
        else if (STM_A16_SP) {}                                             
        else if (STOP)               { pc = addr + 1; }
        else if (JR_R8) {                             
          if      (state == 0)       { pc = addr + 1; }
          else if (state == 1)       { pc = addr + 1; }
          else if (state == 2)       {                }
        }                                                                                                              
        else if (JR_CC_R8) {                                                                                           
        }                                                                                               
      }                                                                                                 
      else if (OP_COL == 1) {                                                                           
        if (LD_RR_D16) {                                                                                
          if      (state == 0)       { pc = addr + 1; }                                           
          else if (state == 1) {                      
            if      (LD_BC_D16)      { pc = addr + 1; }
            else if (LD_DE_D16)      { pc = addr + 1; }
            else if (LD_HL_D16)      { pc = addr + 1; }
            else if (LD_SP_D16)      { pc = addr + 1; }
          }                                           
          else if (state == 2) {                      
            if      (LD_BC_D16)      { pc = addr + 1; }
            else if (LD_DE_D16)      { pc = addr + 1; }
            else if (LD_HL_D16)      { pc = addr + 1; }
            else if (LD_SP_D16)      { pc = addr + 1; }
          }                                           
        }                                             
        else if (ADD_HL_RR) {                         
          if      (state == 0)       { pc = addr + 1; }                                           
          else if (state == 1) {                      
            if      (ADD_HL_BC)      {                }
            else if (ADD_HL_DE)      {                }
            else if (ADD_HL_HL)      {                }
            else if (ADD_HL_SP)      {                }
          }
        }
      }
      else if (OP_COL == 2) {
        if      (state == 0)         { pc = addr + 1; }
        else if (state == 1) {                        
          if      (STM_BC_A )        {                }
          else if (STM_DE_A )        {                }
          else if (STM_HLP_A)        { hl = addr + 1; }
          else if (STM_HLM_A)        { hl = addr - 1; }
          else if (LDM_A_BC )        { bc = addr;     }
          else if (LDM_A_DE )        { de = addr;     }
          else if (LDM_A_HLP)        { hl = addr + 1; }
          else if (LDM_A_HLM)        { hl = addr - 1; }
        }                                             
      }                                               
      else if (OP_COL == 3) {                         
        if (state == 0)              { pc = addr + 1; }                                             
        else if (state == 1) {                        
          if      (INC_BC)           { bc = addr + 1; }
          else if (DEC_BC)           { bc = addr - 1; }
          else if (INC_DE)           { de = addr + 1; }
          else if (DEC_DE)           { de = addr - 1; }
          else if (INC_HL)           { hl = addr + 1; }
          else if (DEC_HL)           { hl = addr - 1; }
          else if (INC_SP)           { sp = addr + 1; }
          else if (DEC_SP)           { sp = addr - 1; }
        }
      }
      else if (OP_COL == 4) {
        if (INC_AT_HL) {
          if      (state == 0)       { pc = addr + 1; }
          else if (state == 1)       {                }
          else if (state == 2)       {                }
        }
        else if (INC_R)              { pc = addr + 1; }
      }
      else if (OP_COL == 5) {
        if (DEC_AT_HL) {
          if      (state == 0)       { pc = addr + 1; }
          else if (state == 1)       {                }
          else if (state == 2)       {                }
        }
        else if (DEC_R)              { pc = addr + 1; }
      }
      else if (OP_COL == 6) {
        if (STM_HL_D8) {
          if      (state == 0)       { pc = addr + 1; }
          else if (state == 1)       { pc = addr + 1; }
          else if (state == 2)       {                }
        }
        else if (LD_R_D8) {
          if      (state == 0)       { pc = addr + 1; }
          else if (state == 1)       { pc = addr + 1; }
        }
      }
      else if (OP_COL == 7)          { pc = addr + 1; }
    }

    //----------------------------------------
    // QUAD 1

    else if (OP_QUAD == 1) {
      if (HALT) {
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       {                }
      }                                             
      else if (OP_COL == 6) {                       
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       {                }
      }                                                                                                  
      else if (OP_ROW == 6) {                                                                            
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       {                }
      }                                                                                                  
      else                         { pc = addr + 1; }
    }

    //----------------------------------------
    // QUAD 2

    else if (OP_QUAD == 2) {
      if (OP_COL == 6) {
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       {                }
      }                                             
      else                         { pc = addr + 1; }
    }

    //----------------------------------------
    // QUAD 3

    else if (OP_QUAD == 3) {
      if (POP_RR) {
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { sp = addr + 1; }
        else if (state == 2)       { sp = addr + 1; }
      }                                             
      else if (PUSH_RR) {                           
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { sp = addr - 1; }                 
        else if (state == 2)       { sp = addr - 1; }
        else if (state == 3)       {                }                                           
      }                                             
      else if (STM_A8_A) {                          
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
        else if (state == 2)       {                }
      }                                             
      else if (ADD_SP_R8) {                         
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
        else if (state == 2)       {                }
        else if (state == 3)       { sp = addr;     }
      }                                             
      else if (LDM_A_A8) {                          
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
        else if (state == 2)       {                }
      }                                             
      else if (LD_HL_SP_R8) {                       
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
        else if (state == 2)       {                }
        else if (state == 3)       {                }
      }
      else if (RET_CC) {
      }                                             
      else if (RET) {                               
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { sp = addr + 1; }
        else if (state == 2)       { sp = addr + 1; }
        else if (state == 3)       {                }
      }                                             
      else if (RETI) {                              
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { sp = addr + 1; }
        else if (state == 2)       { sp = addr + 1; }
        else if (state == 3)       {                }
      }                                             
      else if (JP_HL)              { pc = hl;       } // must have a fast path
      else if (LD_SP_HL) {                          
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { sp = addr;     }
      }                                                                                                  
      else if (JP_CC_A16) {                                                                              
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
        else if (state == 2)       { pc = addr + 1; }
        else if (state == 3)       {                }
      }                                             
      else if (STM_C_A) {                           
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       {                }
      }                                             
      else if (STM_A16_A) {                         
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
        else if (state == 2)       { pc = addr + 1; }
        else if (state == 3)       {                }
      }                                             
      else if (LDM_A_C) {                           
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       {                }
      }                                             
      else if (LDM_A_A16) {                         
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
        else if (state == 2)       { pc = addr + 1; }
        else if (state == 3)       {                }
      }                                             
      else if (JP_A16) {                            
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
        else if (state == 2)       { pc = addr + 1; }
        else if (state == 3)       { pc = xy;       }
      }                                             
      else if (DI)                 { pc = addr + 1; }
      else if (EI)                 { pc = addr + 1; }
      else if (CALL_CC_A16) {
      }                                             
      else if (CALL_A16) {                          
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
        else if (state == 2)       { pc = addr + 1; }
        else if (state == 3)       { sp = addr - 1; }
        else if (state == 4)       { sp = addr - 1; }
        else if (state == 5)       {                }
      }
      else if (ALU_A_D8) {
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { pc = addr + 1; }
      }
      else if (RST_NN) {
        if      (state == 0)       { pc = addr + 1; }
        else if (state == 1)       { sp = addr - 1; }
        else if (state == 2)       { sp = addr - 1; }
        else if (state == 3)       { xy = op & 0x38;}
      }
    }
  }

  //----------------------------------------
  // INTERRUPTS

  if (interrupt) {
  }

  //----------------------------------------
  // CB OPS

  else if (PREFIX_CB) {
    AluOut& ao = alu_out;

    if      (state == 0)       { }
    else if (state == 1) {
      cb = data;
      if (OP_CB_R  )           { ao = alu_cb(cb, reg_get8(CB_COL)); reg_put8(CB_COL, ao.x); update_flags(); }
      if (OP_CB_HL )           {                                                                            }
    }
    else if (state == 2)       { y = data; ao = alu_cb(cb, y); y = ao.x;                           update_flags(); }
    else if (state == 3)       {                                                                            }
  }

  //----------------------------------------
  // REGULAR OPS

  else {
    AluOut& ao = alu_out;

    //----------------------------------------
    // QUAD 0

    if (OP_QUAD == 0) {
      if (OP_COL == 0) {
        if (NOP)                     {                                                    }
        else if (STM_A16_SP) {                                                            }                                                                                 
        else if (STOP)               {                                                    }
        else if (JR_R8) {                                                                 
          if (state == 0)            {                                                    }
          else if (state == 1)       { y = data; ao = alu(0, pcl,      y); pcl = ao.x;    }
          else if (state == 2)       {           ao = alu(1, pch, sxt(y)); pch = ao.x;    }
        }                                                                                                              
        else if (JR_CC_R8) {                                                                                           
        }                                                                                               
      }                                                                                                 
      else if (OP_COL == 1) {                                                                           
        if (LD_RR_D16) {                                                                                
          if (state == 0) {                                                                             
            if      (LD_BC_D16)      {                                                    }
            else if (LD_DE_D16)      {                                                    }
            else if (LD_HL_D16)      {                                                    }
            else if (LD_SP_D16)      {                                                    }
          }                                                                               
          else if (state == 1) {                                                          
            if      (LD_BC_D16)      { c = data;                                          }
            else if (LD_DE_D16)      { e = data;                                          }
            else if (LD_HL_D16)      { l = data;                                          }
            else if (LD_SP_D16)      { p = data;                                          }
          }                                                                               
          else if (state == 2) {                                                          
            if      (LD_BC_D16)      { b = data;                                          }
            else if (LD_DE_D16)      { d = data;                                          }
            else if (LD_HL_D16)      { h = data;                                          }
            else if (LD_SP_D16)      { s = data;                                          }
          }                                                                               
        }                                                                                 
        else if (ADD_HL_RR) {                                                             
          if (state == 0) {                                                               
            if      (ADD_HL_BC)      { ao = alu(0, l, c); l = ao.x;                       }
            else if (ADD_HL_DE)      { ao = alu(0, l, e); l = ao.x;                       }
            else if (ADD_HL_HL)      { ao = alu(0, l, l); l = ao.x;                       }
            else if (ADD_HL_SP)      { ao = alu(0, l, p); l = ao.x;                       }
          }                                                                               
          else if (state == 1) {                                                          
            if      (ADD_HL_BC)      { ao = alu(1, h, b); h = ao.x;       update_flags(); }
            else if (ADD_HL_DE)      { ao = alu(1, h, d); h = ao.x;       update_flags(); }
            else if (ADD_HL_HL)      { ao = alu(1, h, h); h = ao.x;       update_flags(); }
            else if (ADD_HL_SP)      { ao = alu(1, h, s); h = ao.x;       update_flags(); }
          }
        }
      }
      else if (OP_COL == 2) {
        if (state == 0) {
          if      (STM_BC_A )        {                                                    }
          else if (STM_DE_A )        {                                                    }
          else if (STM_HLP_A)        {                                                    }
          else if (STM_HLM_A)        {                                                    }
          else if (LDM_A_BC )        {                                                    }
          else if (LDM_A_DE )        {                                                    }
          else if (LDM_A_HLP)        {                                                    }
          else if (LDM_A_HLM)        {                                                    }
        }                                                                                 
        else if (state == 1) {                                                            
          if      (STM_BC_A )        {                                                    }
          else if (STM_DE_A )        {                                                    }
          else if (STM_HLP_A)        {                                                    }
          else if (STM_HLM_A)        {                                                    }
          else if (LDM_A_BC )        { a = data;                                          }
          else if (LDM_A_DE )        { a = data;                                          }
          else if (LDM_A_HLP)        { a = data;                                          }
          else if (LDM_A_HLM)        { a = data;                                          }
        }                                                                                
      }                                                                                  
      else if (OP_COL == 3) {                                                            
        if (state == 0) {                                                                
          if      (INC_BC)           {                                                    }
          else if (DEC_BC)           {                                                    }
          else if (INC_DE)           {                                                    }
          else if (DEC_DE)           {                                                    }
          else if (INC_HL)           {                                                    }
          else if (DEC_HL)           {                                                    }
          else if (INC_SP)           {                                                    }
          else if (DEC_SP)           {                                                    }
        }                                                                                 
        else if (state == 1) {                                                            
          if      (INC_BC)           {                                                    }
          else if (DEC_BC)           {                                                    }
          else if (INC_DE)           {                                                    }
          else if (DEC_DE)           {                                                    }
          else if (INC_HL)           {                                                    }
          else if (DEC_HL)           {                                                    }
          else if (INC_SP)           {                                                    }
          else if (DEC_SP)           {                                                    }
        }
      }
      else if (OP_COL == 4) {
        if (INC_AT_HL) {
          if      (state == 0)       {                                                                       }
          else if (state == 1)       { y = data; ao = alu(0, y, 1); y = ao.x;                update_flags(); }
          else if (state == 2)       {                                                                       }
        }
        else if (INC_R)              {           ao = alu(0, reg_get8(), 1); reg_put8(ao.x); update_flags(); }
      }
      else if (OP_COL == 5) {
        if (DEC_AT_HL) {
          if      (state == 0)       {                                                                       }
          else if (state == 1)       { y = data; ao = alu(2, y, 1); y = ao.x;                update_flags(); }
          else if (state == 2)       {                                                                       }
        }
        else if (DEC_R)              {           ao = alu(2, reg_get8(), 1); reg_put8(ao.x); update_flags(); }
      }
      else if (OP_COL == 6) {
        if (STM_HL_D8) {
          if      (state == 0)       {                                                    }
          else if (state == 1)       { y = data;                                          }
          else if (state == 2)       {                                                    }
        }
        else if (LD_R_D8) {
          if      (state == 0)       {                                                    }
          else if (state == 1)       { y = data; reg_put8(y);                             }
        }
      }
      else if (OP_COL == 7)          {           ao = rlu(OP_ROW, reg_get8()); a = ao.x;     update_flags(); }
    }

    //----------------------------------------
    // QUAD 1

    else if (OP_QUAD == 1) {
      if (HALT) {
        bool no_halt = ((imask & intf) && !ime);
        if      (state == 0)       { unhalt = 0;                                          }
        else if (state == 1)       {                                                      }
      }                                                                                   
      else if (OP_COL == 6) {                                                             
        if      (state == 0)       {                                                      }
        else if (state == 1)       { y = data; reg_put8(y);                               }
      }                                                                                                  
      else if (OP_ROW == 6) {                                                                            
        if      (state == 0)       {                                                      }
        else if (state == 1)       {                                                      }
      }                                                                                   
      else                         { y = reg_get8(); reg_put8(y);                         }
    }

    //----------------------------------------
    // QUAD 2

    else if (OP_QUAD == 2) {
      if (OP_COL == 6) {
        if      (state == 0)       {                                                            }
        else if (state == 1)       { y = reg_get8(); ao = alu(OP_ROW, a, y); a = ao.x; update_flags(); }
      }                                                                                         
      else                         { y = reg_get8(); ao = alu(OP_ROW, a, y); a = ao.x; update_flags(); }
    }

    //----------------------------------------
    // QUAD 3

    else if (OP_QUAD == 3) {
      if (POP_RR) {
        if (state == 0)            {                                                      }
        else if (state == 1) {                                                            
          if (POP_BC)              { c = data;                                            }
          if (POP_DE)              { e = data;                                            }
          if (POP_HL)              { l = data;                                            }
          if (POP_AF)              { f = data;                                            }
        }                                                                                 
        else if (state == 2) {                                                            
          if (POP_BC)              { b = data;                                            }
          if (POP_DE)              { d = data;                                            }
          if (POP_HL)              { h = data;                                            }
          if (POP_AF)              { a = data;                                            }
        }                                                                                 
      }                                                                                   
      else if (PUSH_RR) {                                                                 
        if (state == 0)            {                                                      }                                        
        else if (state == 1) {                                                            
          if (PUSH_BC)             {                                                      }
          if (PUSH_DE)             {                                                      }
          if (PUSH_HL)             {                                                      }
          if (PUSH_AF)             {                                                      }
        }                                                                                 
        else if (state == 2) {                                                            
          if (PUSH_BC)             {                                                      }
          if (PUSH_DE)             {                                                      }
          if (PUSH_HL)             {                                                      }
          if (PUSH_AF)             {                                                      }
        }                                                                                 
        else if (state == 3) {                                                            
          if (PUSH_BC)             {                                                      }
          if (PUSH_DE)             {                                                      }
          if (PUSH_HL)             {                                                      }
          if (PUSH_AF)             {                                                      }
        }                                                                                 
      }                                                                                   
      else if (STM_A8_A) {                                                                
        if      (state == 0)       {                                                      }
        else if (state == 1)       { xy = 0xFF00 | data;                                  }
        else if (state == 2)       {                                                      }
      }                                                                                   
      else if (ADD_SP_R8) {                                                               
        if      (state == 0)       {                                                      }
        else if (state == 1)       { x = data; ao = alu(0, p, x);      y = ao.x; update_flags(); }
        else if (state == 2)       {           ao = alu(1, s, sxt(x)); x = ao.x;                 }
        else if (state == 3)       {                                                      }
      }                                                                                   
      else if (LDM_A_A8) {                                                                
        if      (state == 0)       {                                                      }
        else if (state == 1)       { xy = 0xFF00 | data;                                  }
        else if (state == 2)       { a = data;                                            }
      }                                                                                   
      else if (LD_HL_SP_R8) {                                                             
        if      (state == 0)       {                                                      }
        else if (state == 1)       {                                                      }
        else if (state == 2)       { x = data; ao = alu(0, p, x);      l = ao.x; update_flags(); }
        else if (state == 3)       {           ao = alu(1, s, sxt(x)); h = ao.x;                 }
      }
      else if (RET_CC)             {}                                                                                                  
      else if (RET)                {}                                                                                   
      else if (RETI)               {}                                                                                   
      else if (JP_HL)              {}
      else if (LD_SP_HL)           {}                                                                                                  
      else if (JP_CC_A16)          {}                                                                                                  
      else if (STM_C_A)            {}                                                                                                  
      else if (STM_A16_A)          {}                                                                                   
      else if (LDM_A_C)            {}                                                                                   
      else if (LDM_A_A16)          {}                                                                                   
      else if (JP_A16)             {}                                                                                   
      else if (DI)                 {}
      else if (EI)                 {}
      else if (CALL_CC_A16)        {}                                                                                   
      else if (CALL_A16)           {}
      else if (ALU_A_D8)           {}
      else if (RST_NN)             {}
    }
  }
}

//-----------------------------------------------------------------------------

#define READ(A)     { addr = A;           write = 0; read = 1; }
#define WRITE(A, B) { addr = A; data = B; write = 1; read = 0; }
#define PASS(A)     { addr = A;           write = 0; read = 0; }

void Z80::tock_t12(const uint8_t imask, const uint8_t intf) {
  //----------------------------------------
  // INTERRUPTS

  if (interrupt) {
    if      (state == 0) { PASS(sp); state_ = 1; }
    else if (state == 1) { WRITE(sp, pch); state_ = 2; }
    else if (state == 2) { WRITE(sp, pcl); state_ = 3; }
    else if (state == 3) { PASS(xy);       state_ = 4; }
    else if (state == 4) {
      if      (imask & intf & INT_JOYPAD_MASK) { READ(0x0060); int_ack = INT_JOYPAD_MASK; }
      else if (imask & intf & INT_SERIAL_MASK) { READ(0x0058); int_ack = INT_SERIAL_MASK; }
      else if (imask & intf & INT_TIMER_MASK)  { READ(0x0050); int_ack = INT_TIMER_MASK; }
      else if (imask & intf & INT_STAT_MASK)   { READ(0x0048); int_ack = INT_STAT_MASK; }
      else if (imask & intf & INT_VBLANK_MASK) { READ(0x0040); int_ack = INT_VBLANK_MASK; }
      else                                     { READ(0x0000); int_ack = 0; } // glitch int address?
      state_ = 0;
    }
  }

  //----------------------------------------
  // CB OPS

  else if (PREFIX_CB) {
    AluOut& ao = alu_out;

    if      (state == 0)       { READ(pc);     state_ = 1; }
    else if (state == 1) {
      if (OP_CB_R  )           { READ(pc);     state_ = 0; }
      if (OP_CB_HL )           { READ(hl);     state_ = 2; }
    }
    else if (state == 2)       { WRITE(hl, y); state_ = 3; }
    else if (state == 3)       { READ(pc);     state_ = 0; }
  }

  //----------------------------------------
  // REGULAR OPS

  else {
    bool cond_fail = false;
    switch (OP_ROW & 3) {
    case 0: cond_fail = (f & F_ZERO); break;
    case 1: cond_fail = !(f & F_ZERO); break;
    case 2: cond_fail = (f & F_CARRY); break;
    case 3: cond_fail = !(f & F_CARRY); break;
    }
    bool nb = cond_fail;
    bool tb = !cond_fail;

    bool no_halt = ((imask & intf) && !ime);

    AluOut& ao = alu_out;

    //----------------------------------------
    // QUAD 0

    if (OP_QUAD == 0) {
      if (OP_COL == 0) {
        if (NOP)                     { READ(pc);     state_ = 0; }
        else if (STM_A16_SP) {         
          if      (state == 0)       { pc = addr + 1;           READ(pc);     state_ = 1; }
          else if (state == 1)       { pc = addr + 1; y = data; READ(pc);     state_ = 2; }
          else if (state == 2)       { pc = addr + 1; x = data; WRITE(xy, p); state_ = 3; }
          else if (state == 3)       { xy = addr + 1;           WRITE(xy, s); state_ = 4; }
          else if (state == 4)       {                          READ(pc);     state_ = 0; }
        }                              
        else if (STOP)               { READ(pc);     state_ = 0; }
        else if (JR_R8) {                            
          if (state == 0)            { READ(pc);     state_ = 1; }
          else if (state == 1)       { PASS(pc);     state_ = 2; }
          else if (state == 2)       { READ(pc);     state_ = 0; }
        }                                                                                                              
        else if (JR_CC_R8) {                                                                                           
          if      (state == 0)       { pc = addr + 1;                                                addr = pc; write = 0; read = 1; state_ = 1; }
          else if (state == 1)       { pc = addr + 1; x = data; ao = alu(0, pcl,      x); y = ao.x;  addr = pc; write = 0; read = 1; state_ = cond_fail ? 0 : 2; }
          else if (state == 2)       {                          ao = alu(1, pch, sxt(x)); x = ao.x;  addr = xy; write = 0; read = 1; state_ = 0; }
        }                                                                                               
      }                                                                                                 
      else if (OP_COL == 1) {                                                                           
        if (LD_RR_D16) {                                                                                
          if (state == 0) {                                                                             
            if      (LD_BC_D16)      { READ(pc);     state_ = 1; }
            else if (LD_DE_D16)      { READ(pc);     state_ = 1; }
            else if (LD_HL_D16)      { READ(pc);     state_ = 1; }
            else if (LD_SP_D16)      { READ(pc);     state_ = 1; }
          }                                          
          else if (state == 1) {                     
            if      (LD_BC_D16)      { READ(pc);     state_ = 2; }
            else if (LD_DE_D16)      { READ(pc);     state_ = 2; }
            else if (LD_HL_D16)      { READ(pc);     state_ = 2; }
            else if (LD_SP_D16)      { READ(pc);     state_ = 2; }
          }                                          
          else if (state == 2) {                     
            if      (LD_BC_D16)      { READ(pc);     state_ = 0; }
            else if (LD_DE_D16)      { READ(pc);     state_ = 0; }
            else if (LD_HL_D16)      { READ(pc);     state_ = 0; }
            else if (LD_SP_D16)      { READ(pc);     state_ = 0; }
          }                                          
        }                                            
        else if (ADD_HL_RR) {                        
          if (state == 0) {                          
            if      (ADD_HL_BC)      { READ(pc);     state_ = 1; }
            else if (ADD_HL_DE)      { READ(pc);     state_ = 1; }
            else if (ADD_HL_HL)      { READ(pc);     state_ = 1; }
            else if (ADD_HL_SP)      { READ(pc);     state_ = 1; }
          }                                     
          else if (state == 1) {                
            if      (ADD_HL_BC)      { READ(pc);     state_ = 0; }
            else if (ADD_HL_DE)      { READ(pc);     state_ = 0; }
            else if (ADD_HL_HL)      { READ(pc);     state_ = 0; }
            else if (ADD_HL_SP)      { READ(pc);     state_ = 0; }
          }
        }
      }
      else if (OP_COL == 2) {
        if (state == 0) {
          if      (STM_BC_A )        { WRITE(bc, a); state_ = 1; }
          else if (STM_DE_A )        { WRITE(de, a); state_ = 1; }
          else if (STM_HLP_A)        { WRITE(hl, a); state_ = 1; }
          else if (STM_HLM_A)        { WRITE(hl, a); state_ = 1; }
          else if (LDM_A_BC )        { READ(bc);     state_ = 1; }
          else if (LDM_A_DE )        { READ(de);     state_ = 1; }
          else if (LDM_A_HLP)        { READ(hl);     state_ = 1; }
          else if (LDM_A_HLM)        { READ(hl);     state_ = 1; }
        }                              
        else if (state == 1) {         
          if      (STM_BC_A )        { READ(pc);     state_ = 0; }
          else if (STM_DE_A )        { READ(pc);     state_ = 0; }
          else if (STM_HLP_A)        { READ(pc);     state_ = 0; }
          else if (STM_HLM_A)        { READ(pc);     state_ = 0; }
          else if (LDM_A_BC )        { READ(pc);     state_ = 0; }
          else if (LDM_A_DE )        { READ(pc);     state_ = 0; }
          else if (LDM_A_HLP)        { READ(pc);     state_ = 0; }
          else if (LDM_A_HLM)        { READ(pc);     state_ = 0; }
        }                              
      }                                
      else if (OP_COL == 3) {          
        if (state == 0) {              
          if      (INC_BC)           { PASS(bc);     state_ = 1; }
          else if (DEC_BC)           { PASS(bc);     state_ = 1; }
          else if (INC_DE)           { PASS(de);     state_ = 1; }
          else if (DEC_DE)           { PASS(de);     state_ = 1; }
          else if (INC_HL)           { PASS(hl);     state_ = 1; }
          else if (DEC_HL)           { PASS(hl);     state_ = 1; }
          else if (INC_SP)           { PASS(sp);     state_ = 1; }
          else if (DEC_SP)           { PASS(sp);     state_ = 1; }
        }                                            
        else if (state == 1) {                       
          if      (INC_BC)           { READ(pc);     state_ = 0; }
          else if (DEC_BC)           { READ(pc);     state_ = 0; }
          else if (INC_DE)           { READ(pc);     state_ = 0; }
          else if (DEC_DE)           { READ(pc);     state_ = 0; }
          else if (INC_HL)           { READ(pc);     state_ = 0; }
          else if (DEC_HL)           { READ(pc);     state_ = 0; }
          else if (INC_SP)           { READ(pc);     state_ = 0; }
          else if (DEC_SP)           { READ(pc);     state_ = 0; }
        }
      }
      else if (OP_COL == 4) {
        if (INC_AT_HL) {
          if      (state == 0)       { READ(hl);     state_ = 1; }
          else if (state == 1)       { WRITE(hl, y); state_ = 2; }
          else if (state == 2)       { READ(pc);     state_ = 0; }
        }
        else if (INC_R)              { READ(pc);     state_ = 0; }
      }
      else if (OP_COL == 5) {
        if (DEC_AT_HL) {
          if      (state == 0)       { READ(hl);     state_ = 1; }
          else if (state == 1)       { WRITE(hl, y); state_ = 2; }
          else if (state == 2)       { READ(pc);     state_ = 0; }
        }
        else if (DEC_R)              { READ(pc);     state_ = 0; }
      }
      else if (OP_COL == 6) {
        if (STM_HL_D8) {
          if      (state == 0)       { READ(pc);     state_ = 1; }
          else if (state == 1)       { WRITE(hl, y); state_ = 2; }
          else if (state == 2)       { READ(pc);     state_ = 0; }
        }
        else if (LD_R_D8) {
          if      (state == 0)       { READ(pc);     state_ = 1; }
          else if (state == 1)       { READ(pc);     state_ = 0; }
        }
      }
      else if (OP_COL == 7)          { READ(pc);     state_ = 0; }
    }

    //----------------------------------------
    // QUAD 1

    else if (OP_QUAD == 1) {
      if (HALT) {
        if      (state == 0)       { READ(pc);     state_ = !no_halt; }
        else if (state == 1)       { READ(pc);     state_ = !unhalt; }
      }                              
      else if (OP_COL == 6) {        
        if      (state == 0)       { READ(hl);     state_ = 1; }
        else if (state == 1)       { READ(pc);     state_ = 0; }
      }                              
      else if (OP_ROW == 6) {        
        if      (state == 0)       { x = reg_get8(); WRITE(hl, x); state_ = 1; }
        else if (state == 1)       { READ(pc);     state_ = 0; }
      }                              
      else                         { READ(pc);     state_ = 0; }
    }

    //----------------------------------------
    // QUAD 2

    else if (OP_QUAD == 2) {
      if (OP_COL == 6) {
        if      (state == 0)       { READ(hl);     state_ = 1; }
        else if (state == 1)       { READ(pc);     state_ = 0; }
      }                                   
      else                         { READ(pc);     state_ = 0; }
    }

    //----------------------------------------
    // QUAD 3

    else if (OP_QUAD == 3) {
      if (POP_RR) {
        if (state == 0)            { READ(sp);       state_ = 1; }
        else if (state == 1) {                       
          if (POP_BC)              { READ(sp);       state_ = 2; }
          if (POP_DE)              { READ(sp);       state_ = 2; }
          if (POP_HL)              { READ(sp);       state_ = 2; }
          if (POP_AF)              { READ(sp);       state_ = 2; }
        }                                            
        else if (state == 2) {                       
          if (POP_BC)              { READ(pc);       state_ = 0; }
          if (POP_DE)              { READ(pc);       state_ = 0; }
          if (POP_HL)              { READ(pc);       state_ = 0; }
          if (POP_AF)              { READ(pc);       state_ = 0; }
        }                                            
      }                                              
      else if (PUSH_RR) {                            
        if (state == 0)            { READ(sp);       state_ = 1; }                                        
        else if (state == 1) {                       
          if (PUSH_BC)             { WRITE(sp, b);   state_ = 2; }
          if (PUSH_DE)             { WRITE(sp, d);   state_ = 2; }
          if (PUSH_HL)             { WRITE(sp, h);   state_ = 2; }
          if (PUSH_AF)             { WRITE(sp, a);   state_ = 2; }
        }                                            
        else if (state == 2) {                       
          if (PUSH_BC)             { WRITE(sp, c);   state_ = 3; }
          if (PUSH_DE)             { WRITE(sp, e);   state_ = 3; }
          if (PUSH_HL)             { WRITE(sp, l);   state_ = 3; }
          if (PUSH_AF)             { WRITE(sp, f);   state_ = 3; }
        }                                            
        else if (state == 3) {                       
          if (PUSH_BC)             { READ(pc);       state_ = 0; }
          if (PUSH_DE)             { READ(pc);       state_ = 0; }
          if (PUSH_HL)             { READ(pc);       state_ = 0; }
          if (PUSH_AF)             { READ(pc);       state_ = 0; }
        }                                            
      }                                              
      else if (STM_A8_A) {                           
        if      (state == 0)       { READ(pc);       state_ = 1; }
        else if (state == 1)       { WRITE(xy, a);   state_ = 2; }
        else if (state == 2)       { READ(pc);       state_ = 0; }
      }                                              
      else if (ADD_SP_R8) {                          
        if      (state == 0)       { READ(pc);       state_ = 1; }
        else if (state == 1)       { PASS(pc);       state_ = 2; }
        else if (state == 2)       { PASS(xy);       state_ = 3; }
        else if (state == 3)       { READ(pc);       state_ = 0; }
      }                                              
      else if (LDM_A_A8) {                           
        if      (state == 0)       { READ(pc);       state_ = 1; }
        else if (state == 1)       { READ(xy);       state_ = 2; }
        else if (state == 2)       { READ(pc);       state_ = 0; }
      }                                              
      else if (LD_HL_SP_R8) {                        
        if      (state == 0)       { READ(pc);       state_ = 1; }
        else if (state == 1)       { PASS(pc);       state_ = 2; }
        else if (state == 2)       { PASS(pc);       state_ = 3; }
        else if (state == 3)       { READ(pc);       state_ = 0; }
      }
      else if (RET_CC) {
        // dunno why this takes five cycles...
        if      (state == 0)       { pc = addr + 1;             PASS(pc);       state_ = 1; }
        else if (state == 1)       {                            READ(pc);       state_ = cond_fail ? 0 : 2; }
        else if (state == 2)       {                            READ(sp);       state_ = 3; }
        else if (state == 3)       { sp = addr + 1; y = data;   READ(sp);       state_ = 4; }
        else if (state == 4)       { sp = addr + 1; x = data;   READ(xy);       state_ = 0; }
      }                                              
      else if (RET) {                                
        if      (state == 0)       {             READ(sp);       state_ = 1; }
        else if (state == 1)       { pcl = data; READ(sp);       state_ = 2; }
        else if (state == 2)       { pch = data; PASS(pc);       state_ = 3; }
        else if (state == 3)       {             READ(pc);       state_ = 0; }
      }                                              
      else if (RETI) {                               
        if      (state == 0)       {                               READ(sp);       state_ = 1; }
        else if (state == 1)       { pcl = data;                   READ(sp);       state_ = 2; }
        else if (state == 2)       { pch = data;                   PASS(pc);       state_ = 3; }
        else if (state == 3)       { ime = true; ime_delay = true; READ(pc);       state_ = 0; }
      }                                              
      else if (JP_HL)              { READ(pc);       state_ = 0; } // must have a fast path
      else if (LD_SP_HL) {                           
        if      (state == 0)       { PASS(hl);       state_ = 1; }
        else if (state == 1)       { READ(pc);       state_ = 0; }
      }                                                                                                                  
      else if (JP_CC_A16) {                                                                                              
        if      (state == 0)       {           READ(pc);       state_ = 1; }
        else if (state == 1)       { y = data; READ(pc);       state_ = 2; }
        else if (state == 2)       { x = data; READ(pc);       state_ = cond_fail ? 0 : 3; }
        else if (state == 3)       {           READ(xy);       state_ = 0; }
      }                              
      else if (STM_C_A) {            
        if      (state == 0)       { xy = 0xFF00 | c; WRITE(xy, a);   state_ = 1; }
        else if (state == 1)       {                  READ(pc);       state_ = 0; }
      }                                              
      else if (STM_A16_A) {                          
        if      (state == 0)       {           READ(pc);       state_ = 1; }
        else if (state == 1)       { y = data; READ(pc);       state_ = 2; }
        else if (state == 2)       { x = data; WRITE(xy, a);   state_ = 3; }
        else if (state == 3)       {           READ(pc);       state_ = 0; }
      }                              
      else if (LDM_A_C) {            
        if      (state == 0)       { xy = 0xFF00 | c; READ(xy);       state_ = 1; }
        else if (state == 1)       { a = data;        READ(pc);       state_ = 0; }
      }                                              
      else if (LDM_A_A16) {                          
        if      (state == 0)       {           READ(pc);       state_ = 1; }
        else if (state == 1)       { y = data; READ(pc);       state_ = 2; }
        else if (state == 2)       { x = data; READ(xy);       state_ = 3; }
        else if (state == 3)       { a = data; READ(pc);       state_ = 0; }
      }                                              
      else if (JP_A16) {         
        if      (state == 0)       {           READ(pc);       state_ = 1; }
        else if (state == 1)       { y = data; READ(pc);       state_ = 2; }
        else if (state == 2)       { x = data; PASS(xy);       state_ = 3; }
        else if (state == 3)       {           READ(xy);       state_ = 0; }
      }                              
      else if (DI)                 { ime = false;      ime_delay = false;                 READ(pc);       state_ = 0; }
      else if (EI)                 { ime = ime_delay;  ime_delay = true;                  READ(pc);       state_ = 0; }
      else if (CALL_CC_A16) {
        if (nb) {
          if      (state == 0)     { pc = addr + 1;           READ(pc);       state_ = 1; }
          else if (state == 1)     { pc = addr + 1; y = data; READ(pc);       state_ = 2; }
          else if (state == 2)     { pc = addr + 1; x = data; READ(pc);       state_ = 0; }
        }
        else {
          if      (state == 0)     { pc = addr + 1;           READ(pc);       state_ = 1; }
          else if (state == 1)     { pc = addr + 1; y = data; READ(pc);       state_ = 2; }
          else if (state == 2)     { pc = addr + 1; x = data; PASS(sp);       state_ = 3; }
          else if (state == 3)     { sp = addr - 1;           WRITE(sp, pch); state_ = 4; }
          else if (state == 4)     { sp = addr - 1;           WRITE(sp, pcl); state_ = 5; }
          else if (state == 5)     { pc = addr + 1;           READ(xy);       state_ = 0; }
        }
      }                              
      else if (CALL_A16) {           
        if      (state == 0)       {           READ(pc);       state_ = 1; }
        else if (state == 1)       { y = data; READ(pc);       state_ = 2; }
        else if (state == 2)       { x = data; PASS(sp);       state_ = 3; }
        else if (state == 3)       {           WRITE(sp, pch); state_ = 4; }
        else if (state == 4)       {           WRITE(sp, pcl); state_ = 5; }
        else if (state == 5)       {           READ(xy);       state_ = 0; }
      }
      else if (ALU_A_D8) {
        if      (state == 0)       {                                                             READ(pc);       state_ = 1; }  
        else if (state == 1)       { y = data; ao = alu(OP_ROW, a, y); a = ao.x; update_flags(); READ(pc);       state_ = 0; }
      }
      else if (RST_NN) {
        if      (state == 0)       { PASS(sp);       state_ = 1; }
        else if (state == 1)       { WRITE(sp, pch); state_ = 2; }
        else if (state == 2)       { WRITE(sp, pcl); state_ = 3; }
        else if (state == 3)       { READ(xy);       state_ = 0; }
      }
    }
  }
}

//-----------------------------------------------------------------------------

void Z80::tock_t23(const uint8_t imask, const uint8_t intf) {
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

void Z80::dump(std::string& o) {
  sprintf(o, "state       %d\n", state);
  sprintf(o, "\n");

  sprintf(o, "op_addr     0x%04x\n", op_addr);
  sprintf(o, "OP          0x%02x\n", op);
  sprintf(o, "CB          0x%02x\n", cb);
  sprintf(o, "\n");

  sprintf(o, "PC          0x%04x 0x%02x 0x%02x\n", pc, pcl, pch);
  sprintf(o, "BC          0x%04x 0x%02x 0x%02x\n", bc, b, c);
  sprintf(o, "DE          0x%04x 0x%02x 0x%02x\n", de, d, e);
  sprintf(o, "HL          0x%04x 0x%02x 0x%02x\n", hl, h, l);
  sprintf(o, "SP          0x%04x 0x%02x 0x%02x\n", sp, s, p);
  sprintf(o, "AF          0x%04x 0x%02x 0x%02x\n", af, a, f);
  sprintf(o, "TEMP        0x%04x 0x%02x 0x%02x\n", xy, x, y);
  sprintf(o, "alu.x       0x%02x\n", alu_out.x);
  sprintf(o, "alu.g       0x%02x\n", alu_out.f);
  sprintf(o, "\n");

  sprintf(o, "addr        0x%04x\n", addr);
  sprintf(o, "data        0x%02x\n", data);
  sprintf(o, "write       %d\n",     write);
  sprintf(o, "\n");

  sprintf(o, "IME         %d\n", ime);
  sprintf(o, "IME_        %d\n", ime_delay);
  sprintf(o, "interrupt   %d\n", interrupt);
  sprintf(o, "int_ack     0x%02x\n", int_ack);
}

//-----------------------------------------------------------------------------
