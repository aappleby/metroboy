#pragma once
#include "Schematics.h"

struct MemBus;
struct Resets;
struct AddressDecoder;

struct Joypad {

  void tick(const Resets& rst, MemBus& mem, const Clocks& clk);

private:

  bool SOUT; // unused?

  //----------
  // registers

  reg JOYP_RA;  /*JUTE*/ // right/a
  reg JOYP_LB;  /*KECY*/ // left/b
  reg JOYP_UC;  /*JALE*/ // up/select
  reg JOYP_DS;  /*KYME*/ // down/start
  reg JOYP_DIR; /*KELY*/
  reg JOYP_BTN; /*COFY*/
  reg JOYP_D6;  /*KUKO*/
  reg JOYP_D7;  /*KERU*/

  reg P10_L; /*KEVU*/
  reg P11_L; /*KAPA*/
  reg P12_L; /*KEJA*/
  reg P13_L; /*KOLO*/
};
