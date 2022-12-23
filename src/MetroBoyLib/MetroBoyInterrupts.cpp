#include "MetroBoyLib/MetroBoyInterrupts.h"

#include "GameboyLib/MetroBoyCPU.h"
#include "GameboyLib/Constants.h"
#include "CoreLib/Constants.h"

//-----------------------------------------------------------------------------

#define MB_DELTA_AB   ((phase_total & 7) == 0)
#define MB_DELTA_BC   ((phase_total & 7) == 1)
#define MB_DELTA_CD   ((phase_total & 7) == 2)
#define MB_DELTA_DE   ((phase_total & 7) == 3)
#define MB_DELTA_EF   ((phase_total & 7) == 4)
#define MB_DELTA_FG   ((phase_total & 7) == 5)
#define MB_DELTA_GH   ((phase_total & 7) == 6)
#define MB_DELTA_HA   ((phase_total & 7) == 7)

//-----------------------------------------------------------------------------

void MetroBoyInterrupts::tick(int phase_total, const Req& req, Ack& ack) const {
  (void)phase_total;

  if (req.read) {
    if (req.addr == ADDR_IF) {
      ack.addr = req.addr;
      ack.data = 0b11100000 | intf;
      ack.read++;
    }
    if (req.addr == ADDR_IE) {
      ack.addr = req.addr;
      ack.data = imask;
      ack.read++;
    }
  }
}

//-----------------------------------------------------------------------------

void MetroBoyInterrupts::tock(int phase_total, const Req& req,
                      uint8_t cpu_int_ack,
                      bool vblank_int,
                      bool stat_int,
                      bool timer_int,
                      bool serial_int,
                      bool joypad_int) {

  if (MB_DELTA_AB) {
    intf &= ~cpu_int_ack;
  }

  if (MB_DELTA_GH) {
    if (req.write) {
      if (req.addr == ADDR_IF) intf  = (uint8_t)req.data | 0b11100000;
      if (req.addr == ADDR_IE) imask = (uint8_t)req.data;
    }
    if (vblank_int) intf |= INT_VBLANK_MASK;
    if (stat_int)   intf |= INT_STAT_MASK;
    if (timer_int)  intf |= INT_TIMER_MASK;
    if (serial_int) intf |= INT_JOYPAD_MASK;
    if (joypad_int) intf |= INT_JOYPAD_MASK;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyInterrupts::dump(Dumper& d) const {
  d("\002--------------INTERRUPTS----------\001\n");

  d("imask  %s\n", byte_to_bits(imask));
  d("intf   %s\n", byte_to_bits(intf));
}

//-----------------------------------------------------------------------------
