#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct Z80 {
  void reset(uint16_t new_pc);

  void tock_ha(uint8_t imask_, uint8_t intf_, uint8_t bus_data);

  void execute_int(uint8_t imask_, uint8_t intf_);
  void execute_halt(uint8_t imask_, uint8_t intf_);
  void execute_op();

  void dump(Dumper& d) const;

  //----------------------------------------

  int      state = 0, state_ = 0;
  uint16_t op_addr = 0;

  uint8_t  op = 0;
  uint8_t  cb = 0;
  bool     ime = 0, ime_delay = 0;

  uint8_t  in = 0;
  uint8_t  out = 0;

  Req      bus_req = {0};

  uint8_t alu_x = 0;
  uint8_t alu_y = 0;
  uint8_t alu_f = 0;
  uint8_t alu_o = 0;

  uint8_t inc_x = 0;
  uint8_t inc_y = 0;
  uint8_t inc_c = 0;

  uint8_t inc(uint8_t ix, uint8_t iy) {
    inc_x = ix;
    inc_y = iy;
    uint8_t inc_o = inc_x + inc_y;
    inc_c = (inc_o == 0x00);
    return inc_o;
  }

  uint8_t dec(uint8_t ix, uint8_t iy) {
    inc_x = ix;
    inc_y = iy;
    uint8_t inc_o = inc_x - inc_y;
    inc_c = (inc_o == 0xFF);
    return inc_o;
  }

  uint8_t  int_ack = 0;

#pragma warning(push)
#pragma warning(disable : 4201)
#pragma warning(disable : 26495)

  // left "data" bus                low side     high side
  union { uint16_t bc = 0; struct { uint8_t   c; uint8_t   b; }; };
  union { uint16_t de = 0; struct { uint8_t   e; uint8_t   d; }; };
  union { uint16_t hl = 0; struct { uint8_t   l; uint8_t   h; }; };
  union { uint16_t af = 0; struct { uint8_t   f; uint8_t   a; }; };

  // right "addr" bus
  union { uint16_t xy = 0; struct { uint8_t xyl; uint8_t xyh; }; };
  union { uint16_t pc = 0; struct { uint8_t pcl; uint8_t pch; }; };
  union { uint16_t sp = 0; struct { uint8_t spl; uint8_t sph; }; };

  uint8_t t;

#pragma warning(pop)

  void set_bus(uint16_t addr, bool read, bool write) {
    bus_req.addr    = addr;
    bus_req.data_lo = write ? out : 0;
    bus_req.read    = read;
    bus_req.write   = write;
  }

  void set_bus(uint16_t new_addr, int new_write) {
    bus_req.addr    = new_addr;
    bus_req.data_lo = uint16_t(new_write ? out : 0);
    bus_req.read    = (bool)!new_write;
    bus_req.write   = (bool)new_write;
  }

  void bus_nop(uint16_t addr = 0) {
    bus_req.addr    = addr;
    bus_req.data_lo = 0;
    bus_req.read    = 0;
    bus_req.write   = 0;
  }

  void bus_read(uint16_t addr)  {
    bus_req.addr    = addr;
    bus_req.data_lo = 0;
    bus_req.read    = 1;
    bus_req.write   = 0;
  }

  void bus_write(uint16_t addr, uint8_t data) {
    bus_req.addr    = addr;
    bus_req.data_lo = data;
    bus_req.read    = 0;
    bus_req.write   = 1;
  }

  void bus_write(uint16_t addr) {
    bus_req.addr    = addr;
    bus_req.data_lo = out;
    bus_req.read    = 0;
    bus_req.write   = 1;
  }

  void op_done(uint16_t addr) {
    bus_req.addr    = addr;
    bus_req.data_lo = 0;
    bus_req.read    = 1;
    bus_req.write   = 0;
    state_ = 0;
  }

  uint8_t get_reg(int mux);
  void set_reg(int mux, uint8_t data);

  void     set_f(uint8_t mask);
  uint8_t  alu_cb(int op, uint8_t flags);

  uint8_t  alu(uint8_t arg1, uint8_t arg2, int op, uint8_t flags);

  uint8_t  alu(int op, uint8_t flags);
  uint8_t  rlu(int op, uint8_t flags);
  uint8_t  daa(uint8_t x, uint8_t f);
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
