#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Z80 {
  void    reset(uint16_t new_pc);

  uint8_t get_int_ack() const { return int_ack; }

  Req  get_bus_req() const;
  void on_bus_ack(Ack ibus_ack_);

  void    tock(const int tcycle_, const uint8_t imask_, const uint8_t intf_);
  void    dump(std::string& d);

  uint16_t get_op_addr() const { return op_addr; }
  uint8_t  get_a()  const { return a; }
  uint8_t  get_op() const { return op; }
  int      get_state() const { return state; }
  
  bool    unhalt;

private:
  int tcycle;

  uint16_t op_addr;
  uint8_t op;
  uint8_t cb;
  bool ime, ime_;
  bool interrupt;

  int state, state_;
  uint16_t addr;
  uint8_t  data;
  bool     read;
  bool     write;
  AluOut   alu_out;
  uint8_t  int_ack;

#pragma warning(push)
#pragma warning(disable : 4201)
  union { uint16_t pc; struct { uint8_t pcl; uint8_t pch; }; };
  union { uint16_t bc; struct { uint8_t   c; uint8_t   b; }; };
  union { uint16_t de; struct { uint8_t   e; uint8_t   d; }; };
  union { uint16_t hl; struct { uint8_t   l; uint8_t   h; }; };
  union { uint16_t af; struct { uint8_t   f; uint8_t   a; }; };
  union { uint16_t sp; struct { uint8_t   p; uint8_t   s; }; };
  union { uint16_t xy; struct { uint8_t   y; uint8_t   x; }; };
#pragma warning(pop)


private:

  uint8_t  reg_get8(int mux) const;
  void     reg_put8(int mux, uint8_t reg);

  uint8_t  reg_get8() const;
  void     reg_put8(uint8_t reg);

  uint8_t  cb_get8() const;
  void     cb_put8(uint8_t reg);

  void     update_flags();

  AluOut   alu_cb(const uint8_t cb, const uint8_t x);
  AluOut   alu(const uint8_t op, const uint8_t x, const uint8_t y);
  AluOut   rlu(const uint8_t op, const uint8_t x);
  AluOut   daa(uint8_t x, uint8_t f);
};

//-----------------------------------------------------------------------------
