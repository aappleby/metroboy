#pragma once
#include "CoreLib/Types.h"

struct Interrupts {
  Interrupts() = default;

  void tick(int phase_total, const Req& req, Ack& ack) const;
  void tock(int phase_total, const Req& req,
            uint8_t cpu_int_ack,
            bool vblank_int,
            bool stat_int,
            bool timer_int,
            bool serial_int,
            bool joypad_int);

  void dump(Dumper& dump) const;

  uint8_t intf  = 0xE1;
  uint8_t imask = 0x00;
};