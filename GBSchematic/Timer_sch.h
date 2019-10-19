#pragma once
#include "Schematics.h"

struct Timer {
public:

  void tick(bool FF04_D1n);

private:

  // tac
  reg SABO,SAMY,SOPU;

  // tima
  reg REGA,POVY,PERU,RATE,RUBY,RAGE,PEDA,NUGA;

  reg NYDU;
  reg MOBA;

  // tma
  reg PETO,MURU,NYKE,SETA,SABU,TYRU,SUFY,TYVA;
};