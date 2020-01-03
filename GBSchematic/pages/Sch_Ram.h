#pragma once

#include "Sch_Common.h"

struct Ram8K {

  void tick(uint16_t addr, uint8_t data, bool wr, bool oe);

  uint8_t ram[8192];
};