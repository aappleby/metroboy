#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Z80 {
  CpuOut reset(int new_model, uint16_t new_pc);
  CpuBus tick_t0(uint8_t imask, uint8_t intf, uint8_t mem_out);
  CpuOut tock_t2();

  uint16_t get_pc() const { return pc; }
  uint8_t  get_a()  const { return a; }
  uint8_t  get_op() const { return op_; }

  void dump(std::string& out);

  int model = 0;
  uint8_t int_ack_;
  uint8_t imask_;
  uint8_t intf_;
  bool unhalt;

private:

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

    Z80_STATE_DELAY_A,
    Z80_STATE_DELAY_B,
    Z80_STATE_DELAY_C,
  };

  Z80State state, state_;

#pragma warning(push)
#pragma warning(disable : 4201)
  // Registers
  union { uint16_t bc; struct { uint8_t c; uint8_t b; }; };
  union { uint16_t de; struct { uint8_t e; uint8_t d; }; };
  union { uint16_t hl; struct { uint8_t l; uint8_t h; }; };
  union { uint16_t af; struct { uint8_t f; uint8_t a; }; };
  union { uint16_t sp; struct { uint8_t p; uint8_t s; }; };
  uint16_t pc;
  uint8_t f_;

  union {
    struct {
      uint8_t data_lo_;
      uint8_t data_hi_;
    };
    uint16_t data16_;
  };
#pragma warning(pop)

  // Signals

  uint8_t op_;
  uint8_t op_cb_;

  // Interrupt stuff

  bool interrupt2;
  bool ime;
  bool ime_delay;

  uint8_t imask_latch;

  // Wires
  uint16_t alu_out_;

  // Misc

  uint32_t opcount;
  int cycle;

  //----------

private:

  Z80State next_state() const;
  int next_interrupt() const;

  bool next_branch() const;
  CpuBus next_bus() const;
  AluOut exec(uint8_t src) const;
  uint8_t reg_fetch8() const;
  uint16_t reg_fetch16() const;
  void reg_put8(int mux, uint8_t reg);
  void reg_put16(int mux, uint16_t reg);
};

//-----------------------------------------------------------------------------
