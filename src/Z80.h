#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Z80 {
  void    reset(uint16_t new_pc);

  uint8_t get_int_ack() const { return int_ack; }
  Req     get_bus_req() const;
  void    tock_t30(const uint8_t imask_, const uint8_t intf_);
  void    tock_t01(const uint8_t imask_, const uint8_t intf_);
  void    tock_t12(const uint8_t imask_, const uint8_t intf_);
  void    tock_t23(const uint8_t imask_, const uint8_t intf_);

  void    on_bus_ack(Ack ibus_ack_);

  void    dump(std::string& d);

  uint16_t get_op_addr() const { return op_addr; }
  uint8_t  get_a()  const { return a; }
  uint8_t  get_op() const { return op; }
  int      get_state() const { return state; }
  
  bool    unhalt;

private:

  uint16_t op_addr;
  uint8_t  op;
  uint8_t  cb;
  bool     ime, ime_delay;

  int      state, state_;
  uint8_t  in;
  uint8_t  out;
  bool     write;
  //uint8_t  bus;

  uint8_t alu_x;
  uint8_t alu_y;
  uint8_t alu_o;
  uint8_t alu_f;

  uint8_t  int_ack;

#pragma warning(push)
#pragma warning(disable : 4201)
  union { uint16_t bc;   struct { uint8_t   c; uint8_t   b; }; };
  union { uint16_t de;   struct { uint8_t   e; uint8_t   d; }; };
  union { uint16_t hl;   struct { uint8_t   l; uint8_t   h; }; };
  union { uint16_t af;   struct { uint8_t   f; uint8_t   a; }; };

  union { uint16_t pc;   struct { uint8_t pcl; uint8_t pch; }; };
  union { uint16_t sp;   struct { uint8_t spl; uint8_t sph; }; };
  union { uint16_t xy;   struct { uint8_t   y; uint8_t   x; }; };
  union { uint16_t ad;   struct { uint8_t  al; uint8_t  ah; }; };

#pragma warning(pop)

  void set_addr(uint16_t new_addr, int new_write);

  uint8_t apl;
  uint8_t aph;
  uint8_t aml;
  uint8_t amh;

private:

  uint8_t& reg(int mux);

  void     set_f(uint8_t mask);
  uint8_t  alu_cb(int op, uint8_t flags);
  uint8_t  alu(int op, uint8_t flags);
  uint8_t  rlu(int op, uint8_t flags);
  uint8_t  daa(uint8_t x, uint8_t f);
};

//-----------------------------------------------------------------------------
