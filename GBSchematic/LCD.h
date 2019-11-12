#pragma once
#include "Schematics.h"

struct Resets;
struct Window;

struct LCD {

  void tick(const Resets& rst, const Window& win, Clocks& clk);

  bool LOBY;
  bool POKY;
  bool ROXO;
  bool TOMU;
  bool TOFU;
  bool SEGU;

  reg NYKA;
  reg PORY;

private:

  reg PYGO;
  reg PAHO;
  reg LUCA; // some sort of counter
  reg LEBE; // some sort of counter
  
  reg VSYNC; /*MEDA*/
};
