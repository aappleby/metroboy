#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct MetroBoyCPU {
public:
  void reset_to_bootrom();
  void reset_to_cart();
  void dump(Dumper& d) const;

  void latch_op(uint8_t _op) {
    if (state == 0) {
      op_addr = _bus_addr;
      op = _op;
    }
  }

  void check_int(uint8_t _imask, uint8_t _intf) {
    if (state == 0) {
      if ((_imask & _intf) && ime) {
        op = 0xF4; // fake opcode
        ime = false;
        ime_delay = false;
      }
    }
    int_ack = 0;
    ime = ime_delay; // must be after int check, before op execution
  }

  void execute(uint8_t _imask, uint8_t _intf) {
    if      (op == 0xF4 /*INT*/ ) execute_int(_imask, _intf);
    else if (op == 0x76 /*HALT*/) execute_halt(_imask, _intf);
    else if (op == 0xCB /*CB*/  ) execute_cb();
    else                          execute_op();
  }

  void update_halt(uint8_t _imask, uint8_t _intf_halt_latch) {
    if (op == 0x76 && (_imask & _intf_halt_latch)) state_ = 0;
  }

  void latch_bus_data(uint8_t _data) {
    if (_bus_read) in = _data;
    state = state_;
  }

  //----------------------------------------

  uint8_t  get_int_ack() const { return int_ack; }
  uint8_t  get_op()      const { return op; }
  uint16_t get_op_addr() const { return op_addr; }
  uint8_t  get_a() const { return a; }

  uint16_t get_bus_addr()  const { return _bus_addr; }
  uint8_t  get_bus_data()  const { return _bus_data; }
  uint8_t  get_bus_read()  const { return _bus_read; }
  uint8_t  get_bus_write() const { return _bus_write; }

  //----------------------------------------

private:

  void execute_int(uint8_t imask_, uint8_t intf_);
  void execute_halt(uint8_t imask_, uint8_t intf_);
  void execute_cb();
  void execute_op();


  uint8_t get_reg(int mux);
  void    set_reg(int mux, uint8_t data);
  void    set_f(uint8_t mask);
  uint8_t alu(uint8_t x, uint8_t y, int op, uint8_t flags);
  uint8_t rlu(uint8_t x, int op, uint8_t flags);
  uint8_t daa(uint8_t x, uint8_t f);
  uint8_t alu_cb(uint8_t arg1, int op, uint8_t flags);

  uint16_t _bus_addr;
  uint8_t  _bus_data;
  uint8_t  _bus_read;
  uint8_t  _bus_write;

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
