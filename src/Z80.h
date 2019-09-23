#pragma once
#include "Types.h"

enum Z80State {
  Z80_STATE_DECODE = 0,
  Z80_STATE_DECODE_CB,
  Z80_STATE_HALT,
  Z80_STATE_INTERRUPT,

  Z80_STATE_PUSH_DELAY,
  Z80_STATE_PUSH1,
  Z80_STATE_PUSH2,

  Z80_STATE_POP1,
  Z80_STATE_POP2,

  Z80_STATE_ARG1,
  Z80_STATE_ARG2,

  Z80_STATE_MEM_READ1,

  Z80_STATE_MEM_WRITE1,
  Z80_STATE_MEM_WRITE2,

  Z80_STATE_RET_DELAY,
  Z80_STATE_DELAY_B,
  Z80_STATE_DELAY_C,
};

//-----------------------------------------------------------------------------

struct Z80 {
  void   reset(int new_model, uint16_t new_pc);
  CpuBus tick_t0(uint8_t imask, uint8_t intf, uint8_t bus_data);
  void   tock_t0(uint8_t imask, uint8_t intf, uint8_t bus_data);
  CpuBus tick_t2(uint8_t imask, uint8_t intf, uint8_t bus_data);
  void   tock_t2(uint8_t imask, uint8_t intf, uint8_t bus_data);

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
#pragma warning(pop)

  //----------

private:

  AluOut   exec(uint8_t src) const;
  uint8_t  reg_fetch8() const;
  void     reg_put8(int mux, uint8_t reg);
};

//-----------------------------------------------------------------------------
