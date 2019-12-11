#pragma once
#include "Schematics.h"

namespace Schematics {

struct Interrupts {

  /*
  Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
  Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
  Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
  Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
  Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
  */

  /*p02.LOPE*/ bool FF0F_0;
  /*p02.UBUL*/ bool FF0F_1;
  /*p02.ULAK*/ bool FF0F_2;
  /*p02.LALU*/ bool FF0F_3;
  /*p02.NYBO*/ bool FF0F_4;

  /*p02.MATY*/ bool FF0F_L0;
  /*p02.NEJY*/ bool FF0F_L1;
  /*p02.NUTY*/ bool FF0F_L2;
  /*p02.MOPO*/ bool FF0F_L3;
  /*p02.PAVY*/ bool FF0F_L4;

};

};