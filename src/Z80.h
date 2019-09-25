#pragma once
#include "Types.h"

enum Z80State {
  Z80_STATE_DECODE = 0,
  Z80_STATE_DECODE_CB,
  Z80_STATE_HALT,
  Z80_STATE_INTERRUPT,

  Z80_STATE_ALU_LO,
  Z80_STATE_ALU_HI,

  Z80_STATE_PUSH0,
  Z80_STATE_PUSH1,
  Z80_STATE_PUSH2,

  Z80_STATE_POP0,
  Z80_STATE_POP1,
  Z80_STATE_POP2,

  Z80_STATE_ARG0,
  Z80_STATE_ARG1,
  Z80_STATE_ARG2,

  Z80_STATE_MEM_READ0,
  Z80_STATE_MEM_READ1,

  Z80_STATE_MEM_WRITE0,
  Z80_STATE_MEM_WRITE1,
  Z80_STATE_MEM_WRITE2,

  Z80_STATE_DELAY_B,
  Z80_STATE_DELAY_C,
};

//-----------------------------------------------------------------------------

struct Z80 {
  void   reset(int new_model, uint16_t new_pc);

  CpuBus tick_t0() const;
  void   tock_t0(uint8_t imask, uint8_t intf, uint8_t bus_data);
  CpuBus tick_t2() const;
  void   tock_t2();

  uint16_t get_pc() const { return pc; }
  uint8_t  get_a()  const { return a; }
  uint8_t  get_op() const { return op; }

  void dump(std::string& out);

  uint8_t int_ack_;
  uint8_t imask_;
  uint8_t intf_;
  bool    unhalt;

private:

  CpuBus bus_temp;

  int model = 0;
  int cycle;
  uint8_t op, op_cb;
  bool ime;
  bool ime_delay;
  uint8_t imask_latch;
  bool interrupt;

  Z80State state, state_;

#pragma warning(push)
#pragma warning(disable : 4201)
  uint16_t pc;
  union { uint16_t bc;   struct { uint8_t  c; uint8_t  b; }; };
  union { uint16_t de;   struct { uint8_t  e; uint8_t  d; }; };
  union { uint16_t hl;   struct { uint8_t  l; uint8_t  h; }; };
  union { uint16_t af;   struct { uint8_t  f; uint8_t  a; }; };
  union { uint16_t sp;   struct { uint8_t  p; uint8_t  s; }; };
  union { uint16_t temp; struct { uint8_t lo; uint8_t hi; }; };

  // internal address register
  uint16_t addr;
  uint8_t data_out;

  bool no_branch;
  bool no_halt;

#pragma warning(pop)

  //----------

private:

  uint8_t  reg_get8() const;
  void     reg_put8(int mux, uint8_t reg);
  
  Z80State first_state();
  Z80State next_state();
};

//-----------------------------------------------------------------------------
