#pragma once
#include "Types.h"

struct IRAM {

  void reset();
  Ack  on_ebus_req(Req ebus_req);
  uint8_t* get() { return ram; }

private:
  uint8_t ram[8192];
};