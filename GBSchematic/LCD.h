#pragma once
#include "Schematics.h"

struct Resets;
struct Window;

struct LCD {

  void tick(const Resets& rst, const Window& win);

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
  reg LUCA;
  reg LEBE;
  reg MEDA;
};
