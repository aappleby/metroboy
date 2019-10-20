#pragma once
#include "Schematics.h"

struct System {
  //----------
  // outputs

  bool BOOT_CS; // output enables boot rom bus override?
  bool HRAM_CS; // output enables hram bus override?

  bool SARO;

  bool FF60_D0;
  bool FF60_D1;

  // all output only
  bool BOOTROM_A7n;
  bool BOOTROM_A6n;
  bool BOOTROM_A5nA4n;
  bool BOOTROM_A5nA4;
  bool BOOTROM_A5A4n;
  bool BOOTROM_A5AA4;
  bool BOOTROM_A3n;
  bool BOOTROM_A2n;
  bool BOOTROM_A1nA0n;
  bool BOOTROM_A1nA0;
  bool BOOTROM_A1A0n;
  bool BOOTROM_A1A0;

  //----------
  // registers

  reg TEPU;
  reg AMUT;
  reg BURO;
};

extern System sys;
