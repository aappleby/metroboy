#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  bool get_interrupt() const;

  void tick(const Req& req, Ack& ack) const;
  void tock(int tphase, const Req& req);

  void dump(std::string& out);

private:
  uint16_t counter = 0;
  uint8_t  old_tima = 0;
  uint8_t  new_tima = 0;
  uint8_t  tma = 0;
  uint8_t  tac = 0;
  bool     do_int = 0;
  bool     do_tick = 0;
};

//-----------------------------------------------------------------------------




#if 0

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  void tock(int tphase, uint16_t addr, uint8_t data, bool read, bool write);
  void dump(std::string& out);

  bool overflow;
  uint8_t bus_out;
  bool bus_oe;

  uint16_t counter;  // FF04
  uint8_t  old_tima; // FF05
  uint8_t  new_tima;
  uint8_t  tma;      // FF06
  uint8_t  tac;      // FF07
  bool     overflow;
  bool     tick;
};

//-----------------------------------------------------------------------------

#endif