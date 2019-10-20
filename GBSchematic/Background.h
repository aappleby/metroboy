#pragma once
#include "Schematics.h"

struct Window;
struct Vram;

struct Background {
  bool VAVA3;

  void tick(const Window& win, Vram& vram);
  void tick_bgpixelshifter(const Window& win, const Vram& vram);
};
