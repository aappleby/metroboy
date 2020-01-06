#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct DecoderSignals {

  static DecoderSignals tick(const SysSignals&   sys_sig,
                             const ClkSignals&   clk_sig,
                             const BusTristates& bus_tri);

  /*p10.AMUS*/ bool ADDR_0xx00000;
  /*p07.TUNA*/ bool ADDR_0000_FE00;
  /*p07.TONA*/ bool ADDR_08n;
  /*p10.ANAP*/ bool ADDR_111111110xx00000;
  /*p07.SYKE*/ bool ADDR_FFXX;

  /*p07.SEMY*/ bool ADDR_XX0X;
  /*p07.SAPA*/ bool ADDR_XXXF;

  /*p22.VOCA*/ bool FF40;
  /*p22.VARY*/ bool FF41;
  /*p22.XARO*/ bool FF42;
  /*p22.XAVY*/ bool FF43;
  /*p22.XOGY*/ bool FF44;
  /*p22.XAYU*/ bool FF45;
  /*p22.XAYU*/ bool FF46;
  /*p22.WERA*/ bool FF47;
  /*p22.XAYO*/ bool FF48;
  /*p22.TEGO*/ bool FF49;
  /*p22.VYGA*/ bool FF4A;
  /*p22.VUMY*/ bool FF4B;

  /*p01.ABUZ*/ bool ADDR_VALID_xBCxxxxx;

  /*p03.TOVY*/ bool TOVY_A00n;
  /*p08.TOLA*/ bool TOLA_A01n;
  /*p06.SEFY*/ bool SEFY_A02n;

  /*p06.SANO*/ bool ADDR_FF00_FF03;

  /*p03.RYFO*/ bool FF04_FF07;

  /*p07.SARO*/ bool ADDR_OAM;
};    
    
//-----------------------------------------------------------------------------

};