#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Decoder {
  /*p10.AMUS*/ bool ADDR_xxxxxxxx0xx00000;
  /*p07.TUNA*/ bool ADDR_0000_FE00;
  /*p07.TONA*/ bool ADDR_08n;
  /*p10.ANAP*/ bool ADDR_111111110xx00000;
  /*p07.SYKE*/ bool ADDR_FFXX;

  /*p22.VOCA*/ bool FF40;
  /*p22.XARO*/ bool FF42;
  /*p22.XAVY*/ bool FF43;
  /*p22.XAYU*/ bool FF45;
  /*p22.WERA*/ bool FF47;
  /*p22.XAYO*/ bool FF48;
  /*p22.TEGO*/ bool FF49;
  /*p22.VYGA*/ bool FF4A;
  /*p22.VUMY*/ bool FF4B;
};

//-----------------------------------------------------------------------------

};