#pragma once
#include "Types.h"

enum Z80State;

//-----------------------------------------------------------------------------

struct Z80 {
  void    reset(uint16_t new_pc);
  Bus     tick() const;
  uint8_t get_int_ack() const { return out_int_ack; }
  void    tock(const int tcycle_, const Bus cpu_in_, const uint8_t imask_, const uint8_t intf_);
  void    dump(std::string& d);

  uint16_t get_pc() const { return pc2; }
  uint8_t  get_a()  const { return a; }
  uint8_t  get_op() const { return op; }
  int      get_state() const { return state; }
  
  bool    unhalt;

private:
  int tcycle;
  Bus bus_to_cpu;
  Bus cpu_to_bus;
  uint8_t out_int_ack;

  uint8_t op, cb;
  bool ime, ime_;
  bool interrupt;

  Z80State state, state2, state_;

#pragma warning(push)
#pragma warning(disable : 4201)
  union { uint16_t pc;   struct { uint8_t pcl; uint8_t pch; }; };
  union { uint16_t bc;   struct { uint8_t   c; uint8_t   b; }; };
  union { uint16_t de;   struct { uint8_t   e; uint8_t   d; }; };
  union { uint16_t hl;   struct { uint8_t   l; uint8_t   h; }; };
  union { uint16_t af;   struct { uint8_t   f; uint8_t   a; }; };
  union { uint16_t sp;   struct { uint8_t   p; uint8_t   s; }; };
  union { uint16_t temp; struct { uint8_t  lo; uint8_t  hi; }; };
#pragma warning(pop)

  uint8_t bus;

  // internal address register
  uint16_t addr;
  uint8_t data_out;
  uint8_t write;

  bool no_branch;
  bool no_halt;

  AluOut alu_out;

  uint8_t int_ack;
  uint8_t imask;
  uint8_t intf;
  uint16_t pc2;

  //----------

private:

  void     state_machine();
  uint8_t  reg_get8(int mux) const;
  uint8_t  reg_get8() const;
  void     reg_put8(int mux, uint8_t reg);
  void     set_flag(uint8_t f_);
};

//-----------------------------------------------------------------------------
