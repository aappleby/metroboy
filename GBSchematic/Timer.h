#pragma once
#include "Schematics.h"

struct Resets;
struct AddressDecoder;
struct MemBus;

struct Timer {
public:

  void tick(bool FF04_D1n, const Resets& rst, AddressDecoder& dec, MemBus& mem);

private:

  reg NYDU;
  reg MOBA;

  // tac
  reg SABO,SAMY,SOPU;

  // tima
  reg REGA,POVY,PERU,RATE,RUBY,RAGE,PEDA,NUGA;

  // tma
  reg PETO,MURU,NYKE,SETA,SABU,TYRU,SUFY,TYVA;
};
