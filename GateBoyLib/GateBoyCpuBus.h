#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyBuses.h"

struct GateBoyCpuBus {
  void reset_to_bootrom();
  void reset_to_cart();

  Gate ABUZ_EXT_RAM_CS_CLK;

  /*SIG_CPU_RDp     */  SigIn  SIG_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  /*SIG_CPU_WRp     */  SigIn  SIG_CPU_WRp;           // top right port PORTA_01: ->
  /*SIG_CPU_UNOR_DBG*/  SigOut SIG_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  /*SIG_CPU_ADDR_HIp*/  SigOut SIG_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  /*SIG_CPU_UMUT_DBG*/  SigOut SIG_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  /*SIG_CPU_EXT_BUSp*/  SigIn  SIG_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP
  ///*SIG_CPU_6*/         SigOut SIG_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  /*SIG_CPU_LATCH_EXT*/ SigIn  SIG_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU
  /*SIG_CPU_BOOTp*/     SigOut SIG_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  /*SIG_BOOT_CSp*/      SigOut SIG_BOOT_CSp;          // cs for bootrom

  /* p07.TEPU*/ DFF17 TEPU_BOOT_BITn_h;

  Gate TEDO_CPU_RDp;
  Gate APOV_CPU_WRp;
  Gate TAPU_CPU_WRp;
};