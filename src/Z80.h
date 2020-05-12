#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Z80 {
  void    reset(uint16_t new_pc);

  uint8_t get_int_ack() const { return int_ack; }
  Req     get_bus_req() const;
  void    tock_t01(const uint8_t imask_, const uint8_t intf_, const Ack ibus_ack_);
  void    tock_t12(const uint8_t imask_, const uint8_t intf_);
  void    tock_t23(const uint8_t imask_, const uint8_t intf_);
  void    tock_t30(const uint8_t imask_, const uint8_t intf_);

  void    dump(std::string& d);

  uint16_t get_op_addr() const { return op_addr; }
  uint8_t  get_a()  const { return a; }
  uint8_t  get_op() const { return op; }
  int      get_state() const { return state; }
  
  bool    unhalt;

//private:

  uint16_t op_addr;
  uint8_t  op;
  uint8_t  cb;
  bool     ime, ime_delay;

  int      state, state_;
  uint8_t  in;
  uint8_t  out;
  bool     write;
  
  uint8_t data_out;

  uint8_t alu_x;
  uint8_t alu_y;
  uint8_t alu_o;
  uint8_t alu_f;

  uint8_t inc_x;
  uint8_t inc_y;
  uint8_t inc_o;
  uint8_t inc_c;

  uint8_t inc(uint8_t ix, uint8_t iy) {
    inc_x = ix;
    inc_y = iy;
    inc_o = inc_x + inc_y;
    inc_c = (inc_o == 0x00);
    return inc_o;
  }

  uint8_t dec(uint8_t ix, uint8_t iy) {
    inc_x = ix;
    inc_y = iy;
    inc_o = inc_x - inc_y;
    inc_c = (inc_o == 0xFF);
    return inc_o;
  }

  uint8_t  int_ack;

#pragma warning(push)
#pragma warning(disable : 4201)

  // left "data" bus              low side     high side
  union { uint16_t dbus;   struct { uint8_t dbl; uint8_t dbh; }; };
  union { uint16_t bc;     struct { uint8_t   c; uint8_t   b; }; };
  union { uint16_t de;     struct { uint8_t   e; uint8_t   d; }; };
  union { uint16_t hl;     struct { uint8_t   l; uint8_t   h; }; };
  union { uint16_t af;     struct { uint8_t   f; uint8_t   a; }; };
  union { uint16_t xy;     struct { uint8_t   y; uint8_t   x; }; };
                           
  // right "addr" bus      
  union { uint16_t abus;   struct { uint8_t abl; uint8_t abh; }; };
  union { uint16_t pc;     struct { uint8_t pcl; uint8_t pch; }; };
  union { uint16_t sp;     struct { uint8_t spl; uint8_t sph; }; };
  union { uint16_t ad;     struct { uint8_t adl; uint8_t adh; }; };

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
