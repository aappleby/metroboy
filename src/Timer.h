#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  Bus  tick() const;
  bool get_interrupt() const;
  void tock(int tcycle_, Bus bus_to_timer_);

  void dump(std::string& out);

private:
  int tphase;
  Bus bus_to_timer;
  Bus timer_to_bus;

  uint16_t counter;  // FF04
  uint16_t tima;     // FF05
  uint8_t  tma;      // FF06
  uint8_t  tac;      // FF07
  
  bool do_tick;
  bool do_interrupt;
};

//-----------------------------------------------------------------------------

struct HardTimer {
  bool TOLA_NA1;
  bool FF04_D1;
  bool FROM_CPU5;
  bool _INT_TIMER;

  uint8_t tock(int tcycle, bool RESET2, uint16_t addr, uint8_t data, bool CPU_RD, bool CPU_WR);

  // TAC
  bool SABO = 0;
  bool SAMY = 0;
  bool SOPU = 0;

  // TMA
  bool PETO = 0;
  bool MURU = 0;
  bool NYKE = 0;
  bool SETA = 0;
  bool SABU = 0;
  bool TYRU = 0;
  bool SUFY = 0;
  bool TYVA = 0;

  // TIMA
  bool REGA = 0;
  bool POVY = 0;
  bool PERU = 0;
  bool RATE = 0;
  bool RUBY = 0;
  bool RAGE = 0;
  bool PEDA = 0;
  bool NUGA = 0;

  bool NYDU = 0;
  bool MOBA = 0;
};

//-----------------------------------------------------------------------------
