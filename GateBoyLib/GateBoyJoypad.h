#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyJoypad {

  void read(GateBoyCpuBus& cpu_bus);
  void write_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);
  void tock2(GateBoyResetDebug& rst, GateBoyClock& clk, uint8_t sys_buttons);

  /* p02.ASOK*/ wire ASOK_INT_JOYp() const { return and2(APUG_JP_GLITCH3.qp_new2(), BATU_JP_GLITCH0.qp_new2()); }

  void dump(Dumper& d) {
    d.dump_bitp("AWOB_WAKE_CPU   : ", AWOB_WAKE_CPU.state);
    d.dump_bitp("SIG_CPU_WAKE    : ", SIG_CPU_WAKE.state);
    d("\n");
    d.dump_bitp("PIN67_JOY_P10   : ", PIN67_JOY_P10.state);
    d.dump_bitp("PIN66_JOY_P11   : ", PIN66_JOY_P11.state);
    d.dump_bitp("PIN65_JOY_P12   : ", PIN65_JOY_P12.state);
    d.dump_bitp("PIN64_JOY_P13   : ", PIN64_JOY_P13.state);
    d.dump_bitp("PIN63_JOY_P14   : ", PIN63_JOY_P14.state);
    d.dump_bitp("PIN62_JOY_P15   : ", PIN62_JOY_P15.state);
    d("\n");
    d.dump_bitp("KEVU_JOYP_L0n   : ", KEVU_JOYP_L0n.state);
    d.dump_bitp("KAPA_JOYP_L1n   : ", KAPA_JOYP_L1n.state);
    d.dump_bitp("KEJA_JOYP_L2n   : ", KEJA_JOYP_L2n.state);
    d.dump_bitp("KOLO_JOYP_L3n   : ", KOLO_JOYP_L3n.state);
    d("\n");
    d.dump_bitp("BATU_JP_GLITCH0 : ", BATU_JP_GLITCH0.state);
    d.dump_bitp("ACEF_JP_GLITCH1 : ", ACEF_JP_GLITCH1.state);
    d.dump_bitp("AGEM_JP_GLITCH2 : ", AGEM_JP_GLITCH2.state);
    d.dump_bitp("APUG_JP_GLITCH3 : ", APUG_JP_GLITCH3.state);
    d("\n");
    d.dump_bitp("JUTE_DBG_D0     : ", JUTE_DBG_D0.state);
    d.dump_bitp("KECY_DBG_D1     : ", KECY_DBG_D1.state);
    d.dump_bitp("JALE_DBG_D2     : ", JALE_DBG_D2.state);
    d.dump_bitp("KYME_DBG_D3     : ", KYME_DBG_D3.state);
    d.dump_bitp("KELY_JOYP_UDLR  : ", KELY_JOYP_UDLRp.state);
    d.dump_bitp("COFY_JOYP_ABCS  : ", COFY_JOYP_ABCSp.state);
    d.dump_bitp("KUKO_DBG_D6     : ", KUKO_DBG_D6.state);
    d.dump_bitp("KERU_DBG_D7     : ", KERU_DBG_D7.state);
  }

  // This is driven by what we think is a latch and it goes straight to the CPU - maybe there's a pull-down?
  /*p02.AWOB*/ TpLatch AWOB_WAKE_CPU;

  /*p02.BATU*/ DFF17 BATU_JP_GLITCH0;
  /*p02.ACEF*/ DFF17 ACEF_JP_GLITCH1;
  /*p02.AGEM*/ DFF17 AGEM_JP_GLITCH2;
  /*p02.APUG*/ DFF17 APUG_JP_GLITCH3;

  /*p05.JUTE*/ DFF17 JUTE_DBG_D0;
  /*p05.KECY*/ DFF17 KECY_DBG_D1;
  /*p05.JALE*/ DFF17 JALE_DBG_D2;
  /*p05.KYME*/ DFF17 KYME_DBG_D3;
  /*p05.KELY*/ DFF17 KELY_JOYP_UDLRp;
  /*p05.COFY*/ DFF17 COFY_JOYP_ABCSp;
  /*p05.KUKO*/ DFF17 KUKO_DBG_D6;
  /*p05.KERU*/ DFF17 KERU_DBG_D7;

  /*p05.KEVU*/ TpLatch KEVU_JOYP_L0n; // 10-rung, looks like pass gate or something
  /*p05.KAPA*/ TpLatch KAPA_JOYP_L1n; // 10-rung, looks like pass gate or something
  /*p05.KEJA*/ TpLatch KEJA_JOYP_L2n; // 10-rung, looks like pass gate or something
  /*p05.KOLO*/ TpLatch KOLO_JOYP_L3n; // 10-rung, looks like pass gate or something

  Signal SIG_CPU_WAKE;  // top right wire by itself <- P02.AWOB

  // Pressing a button pulls the corresponding pin _down_.
  PinIn  PIN67_JOY_P10;
  PinIn  PIN66_JOY_P11;
  PinIn  PIN65_JOY_P12;
  PinIn  PIN64_JOY_P13;
  PinOut PIN63_JOY_P14;
  PinOut PIN62_JOY_P15;
};

//------------------------------------------------------------------------------------------------------------------------
