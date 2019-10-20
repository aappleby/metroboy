#pragma once
#include "Schematics.h"

struct Resets;

struct Joypad {

  void tick(const Resets& rst);

private:

  bool SOUT; // unused?

  //----------
  // registers

  bool KYME_Q, KYME_CLK;
  bool KUKO_Q, KUKO_CLK;
  bool KERU_Q, KERU_CLK;
  bool JALE_Q, JALE_CLK;
  bool KELY_Q, KELY_CLK;
  bool COFY_Q, COFY_CLK;
  bool JUTE_Q, JUTE_CLK;
  bool KECY_Q, KECY_CLK;

  bool KOLO_L;
  bool KEJA_L;
  bool KEVU_L;
  bool KAPA_L;

};

extern Joypad joy;
