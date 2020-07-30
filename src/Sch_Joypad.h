#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Joypad {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);

  void preset_buttons(uint8_t buttons);
  /*p02.ASOK*/ wire ASOK_INT_JOYPADp() const { return and(APUG_JP_GLITCH3.q(), BATU_JP_GLITCH0.q()); }

  void dump(Dumper& d) {
    d("----------  Joypad  ----------\n");
    d("AWOB_WAKE_CPU %c\n", AWOB_WAKE_CPU.c());
    d("CPU_PIN_WAKE  %c\n", CPU_PIN_WAKE .c()); 

    d("BATU_JP_GLITCH0  %c\n", BATU_JP_GLITCH0  .c());
    d("ACEF_JP_GLITCH1  %c\n", ACEF_JP_GLITCH1  .c());
    d("AGEM_JP_GLITCH2  %c\n", AGEM_JP_GLITCH2  .c());
    d("APUG_JP_GLITCH3  %c\n", APUG_JP_GLITCH3  .c());
    d("JUTE_JOYP_RA     %c\n", JUTE_JOYP_RA     .c());
    d("KECY_JOYP_LB     %c\n", KECY_JOYP_LB     .c());
    d("JALE_JOYP_UC     %c\n", JALE_JOYP_UC     .c());
    d("KYME_JOYP_DS     %c\n", KYME_JOYP_DS     .c());
    d("KELY_JOYP_UDLR   %c\n", KELY_JOYP_UDLR   .c());
    d("COFY_JOYP_ABCS   %c\n", COFY_JOYP_ABCS   .c());
    d("KUKO_DBG_FF00_D6 %c\n", KUKO_DBG_FF00_D6 .c());
    d("KERU_DBG_FF00_D7 %c\n", KERU_DBG_FF00_D7 .c());
    d("KEVU_JOYP_L0     %c\n", KEVU_JOYP_L0     .c());
    d("KAPA_JOYP_L1     %c\n", KAPA_JOYP_L1     .c());
    d("KEJA_JOYP_L2     %c\n", KEJA_JOYP_L2     .c());
    d("KOLO_JOYP_L3     %c\n", KOLO_JOYP_L3     .c());

    d("JOY_PIN_P10 %c%c%c%c\n", JOY_PIN_P10_A.c(), JOY_PIN_P10_B.c(), JOY_PIN_P10_C.c(), JOY_PIN_P10_D.c()); 
    d("JOY_PIN_P11 %c%c%c%c\n", JOY_PIN_P11_A.c(), JOY_PIN_P11_B.c(), JOY_PIN_P11_C.c(), JOY_PIN_P11_D.c()); 
    d("JOY_PIN_P12 %c%c%c%c\n", JOY_PIN_P12_A.c(), JOY_PIN_P12_B.c(), JOY_PIN_P12_C.c(), JOY_PIN_P12_D.c()); 
    d("JOY_PIN_P13 %c%c%c%c\n", JOY_PIN_P13_A.c(), JOY_PIN_P13_B.c(), JOY_PIN_P13_C.c(), JOY_PIN_P13_D.c()); 
    d("JOY_PIN_P14 %c%c%c%c\n", JOY_PIN_P14_A.c(), '_',               '_',               JOY_PIN_P14_D.c());
    d("JOY_PIN_P15 %c%c%c%c\n", JOY_PIN_P15_A.c(), '_',               '_',               JOY_PIN_P15_D.c());
    d("\n");
  }

private:
  friend struct SchematicTop;

  // This is driven by what we think is a latch and it goes straight to the CPU - maybe there's a pull-down?
  /*p02.AWOB*/ Tri AWOB_WAKE_CPU    = TRI_D0NP;

  Tri CPU_PIN_WAKE  = TRI_HZNP;   // top right wire by itself <- P02.AWOB

  /*p02.BATU*/ Reg BATU_JP_GLITCH0  = REG_D0C0;
  /*p02.ACEF*/ Reg ACEF_JP_GLITCH1  = REG_D0C0;
  /*p02.AGEM*/ Reg AGEM_JP_GLITCH2  = REG_D0C0;
  /*p02.APUG*/ Reg APUG_JP_GLITCH3  = REG_D0C0;
  
  /*p05.JUTE*/ Reg JUTE_JOYP_RA     = REG_D0C0;
  /*p05.KECY*/ Reg KECY_JOYP_LB     = REG_D0C0;
  /*p05.JALE*/ Reg JALE_JOYP_UC     = REG_D0C0;
  /*p05.KYME*/ Reg KYME_JOYP_DS     = REG_D0C0;

  /*p05.KELY*/ Reg KELY_JOYP_UDLR   = REG_D0C0;
  /*p05.COFY*/ Reg COFY_JOYP_ABCS   = REG_D0C0;
  /*p05.KUKO*/ Reg KUKO_DBG_FF00_D6 = REG_D0C0;
  /*p05.KERU*/ Reg KERU_DBG_FF00_D7 = REG_D0C0;

  /*p05.KEVU*/ Tri KEVU_JOYP_L0     = TRI_D0NP; // 10-rung, looks like pass gate or something
  /*p05.KAPA*/ Tri KAPA_JOYP_L1     = TRI_D0NP; // 10-rung, looks like pass gate or something
  /*p05.KEJA*/ Tri KEJA_JOYP_L2     = TRI_D0NP; // 10-rung, looks like pass gate or something
  /*p05.KOLO*/ Tri KOLO_JOYP_L3     = TRI_D0NP; // 10-rung, looks like pass gate or something

  Tri JOY_PIN_P10_A = TRI_HZNP;   // PIN_67 <- P05.KOLE
  Tri JOY_PIN_P10_B = TRI_HZNP;   // PIN_67 <- tied low between BONE and BUFY
  Tri JOY_PIN_P10_C = TRI_D0NP;   // PIN_67 -> P02.KERY, P05.KEVU
  Tri JOY_PIN_P10_D = TRI_HZNP;   // PIN_67 <- P05.KYBU

  Tri JOY_PIN_P11_A = TRI_HZNP;   // PIN_66 <- P05.KYTO
  Tri JOY_PIN_P11_B = TRI_HZNP;   // PIN_66 <- tied low between BONE and BUFY
  Tri JOY_PIN_P11_C = TRI_D0NP;   // PIN_66 -> P02.KERY, P05.KAPA
  Tri JOY_PIN_P11_D = TRI_HZNP;   // PIN_66 <- P05.KABU

  Tri JOY_PIN_P12_A = TRI_HZNP;   // PIN_65 <- P05.KYHU
  Tri JOY_PIN_P12_B = TRI_HZNP;   // PIN_65 <- tied low between BONE and BUFY
  Tri JOY_PIN_P12_C = TRI_D0NP;   // PIN_65 -> P02.KERY, P05.KEJA
  Tri JOY_PIN_P12_D = TRI_HZNP;   // PIN_65 <- P05.KASY

  Tri JOY_PIN_P13_A = TRI_HZNP;   // PIN_64 <- P05.KORY
  Tri JOY_PIN_P13_B = TRI_HZNP;   // PIN_64 <- tied low between BONE and BUFY
  Tri JOY_PIN_P13_C = TRI_D0NP;   // PIN_64 -> P02.KERY, P05.KOLO
  Tri JOY_PIN_P13_D = TRI_HZNP;   // PIN_64 <- P05.KALE

  Tri JOY_PIN_P14_A = TRI_HZNP;   // PIN_63 <- p05.KARU
  Tri JOY_PIN_P14_D = TRI_HZNP;   // PIN_63 <- p05.KELY

  Tri JOY_PIN_P15_A = TRI_HZNP;   // PIN_62 <- p05.CELA
  Tri JOY_PIN_P15_D = TRI_HZNP;   // PIN_62 <- p05.COFY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics