#pragma once
#include "Schematics.h"

struct Background {
  bool VAVA3;

  void tick(const Window& win, Vram& vram, const Clocks& clk);
  void tick_bgpixelshifter(const Window& win, const Vram& vram, const Clocks& clk);
};
