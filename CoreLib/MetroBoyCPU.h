#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct MetroBoyCPU {
  void reset_to_bootrom();
  void reset_to_cart();

  void dump(Dumper& d) const;

  void tock_ab(uint8_t imask, uint8_t intf_gh, uint8_t bus_data);
  void tock_de(uint8_t imask, uint8_t intf_cd);

  void execute_int(uint8_t imask_, uint8_t intf_);
  void execute_halt(uint8_t imask_, uint8_t intf_);
  void execute_cb();
  void execute_op();

  //----------------------------------------

  void bus_pass(uint16_t addr) {
    _bus_addr  = addr;
    _bus_data  = 0;
    _bus_read  = 0;
    _bus_write = 0;
  }

  void bus_read(uint16_t addr)  {
    _bus_addr  = addr;
    _bus_data  = 0;
    _bus_read  = 1;
    _bus_write = 0;
  }

  void bus_write(uint16_t addr, uint8_t data) {
    _bus_addr  = addr;
    _bus_data  = data;
    _bus_read  = 0;
    _bus_write = 1;
  }

  void bus_done(uint16_t addr) {
    _bus_addr  = addr;
    _bus_data  = 0;
    _bus_read  = 1;
    _bus_write = 0;
    state_ = 0;
  }

  //----------------------------------------

  uint8_t get_reg(int mux);
  void    set_reg(int mux, uint8_t data);
  void    set_f(uint8_t mask);
  uint8_t alu(uint8_t x, uint8_t y, int op, uint8_t flags);
  uint8_t rlu(uint8_t x, int op, uint8_t flags);
  uint8_t daa(uint8_t x, uint8_t f);
  uint8_t alu_cb(uint8_t arg1, int op, uint8_t flags);

  //----------------------------------------

  // hacks to make tock idempotent
  bool tocked_ab = false;
  bool tocked_de = false;

  uint16_t _bus_addr;
  uint8_t  _bus_data;
  uint8_t  _bus_read;
  uint8_t  _bus_write;

  uint16_t op_addr;
  uint8_t  op;
  uint8_t  cb;
  int      state, state_;
  uint8_t  in;

  bool     ime, ime_delay;
  uint8_t  int_addr;
  uint8_t  int_ack;

  uint8_t  alu_f;
  uint8_t  alu_o;

#pragma warning(push)
#pragma warning(disable : 4201)
#pragma warning(disable : 26495)

  // right "addr" bus           low side     high side
  union { uint16_t pc; struct { uint8_t pcl; uint8_t pch; }; };
  union { uint16_t sp; struct { uint8_t spl; uint8_t sph; }; };
  union { uint16_t xy; struct { uint8_t xyl; uint8_t xyh; }; };

  // left "data" bus            low side     high side
  union { uint16_t bc; struct { uint8_t   c; uint8_t   b; }; };
  union { uint16_t de; struct { uint8_t   e; uint8_t   d; }; };
  union { uint16_t hl; struct { uint8_t   l; uint8_t   h; }; };
  union { uint16_t af; struct { uint8_t   f; uint8_t   a; }; };

#pragma warning(pop)
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
