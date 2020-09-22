#if 0

#include "MetroBoyLib/Interrupts.h"

//-----------------------------------------------------------------------------

void Interrupts::reset() {
  intf = 0xE1;
  imask = 0x00;
}

//-----------------------------------------------------------------------------

void Interrupts::ibus_req(Req ibus_req) {
  ack = {0};
  bool hit = (ibus_req.addr == ADDR_IF) || (ibus_req.addr == ADDR_IE);

  if (!hit) {
    return;
  }
  else if (ibus_req.write) {
    if (ibus_req.addr == ADDR_IF) intf  = (uint8_t)ibus_req.data | 0b11100000;
    if (ibus_req.addr == ADDR_IE) imask = (uint8_t)ibus_req.data;
    ack = {
      .addr  = ibus_req.addr,
      .data  = ibus_req.data,
      .read  = 0,
      .write = 1,
    };
  }
  else if (ibus_req.read) {
    uint8_t data = 0;
    if (ibus_req.addr == ADDR_IF) data = 0b11100000 | intf;
    if (ibus_req.addr == ADDR_IE) data = imask;
    ack = {
      .addr = ibus_req.addr,
      .data  = data,
      .read  = 1,
      .write = 0,
    };
  }
}

void Interrupts::ibus_ack(Ack& ibus_ack) const {
  ibus_ack.addr  += ack.addr;
  ibus_ack.data  += ack.data;
  ibus_ack.read  += ack.read;
  ibus_ack.write += ack.write;
}

//-----------------------------------------------------------------------------

void Interrupts::tock() {

  uint8_t intf_ = intf;
  uint8_t imask_ = imask;

  if (tphase_new == 0 || tphase_new == 2) {
    bool fire_int_timer1   = timer.get_interrupt();
    bool fire_int_buttons1 = joypad.get() != 0xFF;

    if (ppu.vblank1)       intf_ |= INT_VBLANK_MASK;
    if (ppu.stat1)         intf_ |= INT_STAT_MASK;
    if (fire_int_timer1)   intf_ |= INT_TIMER_MASK;
    if (fire_int_buttons1) intf_ |= INT_JOYPAD_MASK;
  }

  intf = intf_;
  imask = imask_;
}

//-----------------------------------------------------------------------------

void Interrupts::dump(std::string& d) {
  sprintf(d, "imask  %s\n", byte_to_bits(imask));
  sprintf(d, "intf   %s\n", byte_to_bits(intf));
}

//-----------------------------------------------------------------------------

#endif