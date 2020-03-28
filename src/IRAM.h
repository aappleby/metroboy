#pragma once
#include "Types.h"

struct IRAM {

  void reset();
  bool on_ebus_req(Req ebus_req, Ack& ebus_ack);
  uint8_t* get() { return ram; }

private:
  uint8_t ram[8192];
};