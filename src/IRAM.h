#pragma once
#include "Types.h"

struct IRAM {
  void reset();
  void tock_req(const Req& ebus_req);
  void tick_ack(Ack& ebus_ack) const;

  uint8_t* get() { return ram; }

private:
  Ack ack;
  uint8_t ram[8192] = {};
};