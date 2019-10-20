#pragma once
#include "Schematics.h"

struct Resets;

struct LCD {

  bool LOBY;
  bool POKY;
  bool ROXO;
  bool TOMU;
  bool TOFU;
  bool SEGU;

  void tick(const Resets& rst);

  reg NYKA;
  reg PORY;

private:

  reg PYGO;
  reg PAHO;
  reg LUCA;
  reg LEBE;
  reg MEDA;
};

extern LCD lcd;