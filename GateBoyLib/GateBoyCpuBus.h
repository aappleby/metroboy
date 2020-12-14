#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyCpuBus {
  Signal BUS_CPU_A[16];
  Signal BUS_CPU_D[8];
  BusOut BUS_CPU_D_out[8];

  PinIn  PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  PinIn  PIN_CPU_WRp;           // top right port PORTA_01: ->
  PinOut PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinOut PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  PinOut PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinOut PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  PinIn  PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  PinIn  PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  PinIn  PIN_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU
};