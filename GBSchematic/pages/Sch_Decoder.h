#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------


struct Decoder {
  void reset() {
    /*p10.AMUS*/ ADDR_xxxxxxxx0xx00000 = 1;
    /*p07.TUNA*/ ADDR_0000_FE00 = 1;
    /*p07.TONA*/ ADDR_08n = 1;
    /*p10.ANAP*/ ADDR_111111110xx00000 = 0;
    /*p07.SYKE*/ ADDR_FFXX = 0;

    /*p07.SEMY*/ ADDR_XX0X = 1;
    /*p07.SAPA*/ ADDR_XXXF = 0;

    /*p22.VOCA*/ FF40 = 0;
    /*p22.VARY*/ FF41 = 0;
    /*p22.XARO*/ FF42 = 0;
    /*p22.XAVY*/ FF43 = 0;
    /*p22.XOGY*/ FF44 = 0;
    /*p22.XAYU*/ FF45 = 0;
    /*p22.XAYU*/ FF46 = 0;
    /*p22.WERA*/ FF47 = 0;
    /*p22.XAYO*/ FF48 = 0;
    /*p22.TEGO*/ FF49 = 0;
    /*p22.VYGA*/ FF4A = 0;
    /*p22.VUMY*/ FF4B = 0;

    /*p07.TUTU*/ ADDR_BOOT = 0;
    /*p25.SOSE*/ ADDR_VRAM = 0;
    /*p01.ABUZ*/ ADDR_VALID_xBCxxxxx = 1;

    /*p03.TOVY*/ TOVY_A00n = 1;
    /*p08.TOLA*/ TOLA_A01n = 1;
    /*p06.SEFY*/ SEFY_A02n = 1;

    /*p06.SANO*/ ADDR_FF00_FF03 = 0;

    /*p03.RYFO*/ FF04_FF07 = 0;

    /*p08.TEVY*/ ADDR_NOT_VRAM = 1;
    /*p08.TEXO*/ ADDR_VALID_AND_NOT_VRAM = 0;
    /*p08.LEVO*/ ADDR_VALID_AND_NOT_VRAMn = 1;

    /*p07.SARO*/ ADDR_OAM = 0;
  }

  void tick(const Bus& bus,
            const Clocks& clocks,
            wire BOOT_BIT,
            wire MODE_DBG2,
            wire ADDR_VALID);

  /*p10.AMUS*/ bool ADDR_xxxxxxxx0xx00000;
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

  /*p07.TUTU*/ bool ADDR_BOOT;
  /*p25.SOSE*/ bool ADDR_VRAM;
  /*p01.ABUZ*/ bool ADDR_VALID_xBCxxxxx;

  /*p03.TOVY*/ bool TOVY_A00n;
  /*p08.TOLA*/ bool TOLA_A01n;
  /*p06.SEFY*/ bool SEFY_A02n;

  /*p06.SANO*/ bool ADDR_FF00_FF03;

  /*p03.RYFO*/ bool FF04_FF07;

  /*p08.TEVY*/ bool ADDR_NOT_VRAM;
  /*p08.TEXO*/ bool ADDR_VALID_AND_NOT_VRAM;
  /*p08.LEVO*/ bool ADDR_VALID_AND_NOT_VRAMn;

  /*p07.SARO*/ bool ADDR_OAM;
};    
    
//-----------------------------------------------------------------------------

};