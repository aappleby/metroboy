#pragma once
#include "Types.h"

struct IRAM {
  void reset();
  void tock(const Req& ebus_req);
  void tick(const Req& ebus_req, Ack& ebus_ack) const;

  uint8_t* get() { return ram; }

private:
  uint8_t ram[8192] = {};
};