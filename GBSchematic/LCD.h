#pragma once
#include "Schematics.h"

struct LCD {

  bool LOBY;
  bool POKY;
  bool ROXO;
  bool TOMU;
  bool TOFU;
  bool SEGU;

  void tick();

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