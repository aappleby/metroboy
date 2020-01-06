#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SysPins;

//-----------------------------------------------------------------------------

struct DebugSignals {
  /*p07.UBET*/ bool T1n;
  /*p07.UVAR*/ bool T2n;

  /*p07.UMUT*/ bool MODE_DBG1;
  /*p07.UNOR*/ bool MODE_DBG2;
  /*p07.UPOJ*/ bool MODE_PROD;

  /*p08.TOVA*/ bool MODE_DBG2n1;
  /*p08.RYCA*/ bool MODE_DBG2n2;

  /*p05.KUKO*/ bool DBG_FF00_D6;
  /*p05.KERU*/ bool DBG_FF00_D7;
  /*p05.KORE*/ bool P05_NC0;
  /*p05.KYWE*/ bool P05_NC1;

  /*p07.BURO*/ bool FF60_0;
  /*p05.KURA*/ bool FF60_0n;
  /*p07.AMUT*/ bool FF60_1;

  /*p25.TUTO*/ bool DBG_VRAM;
  /*p25.RACO*/ bool DBG_VRAMn;
};

struct DebugRegisters {

  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  void tick(const BusSignals& bus_sig,
            const ClkSignals& clk_sig,
            const SysPins& sys_pins,
            const JoypadPins& joy_pins,
            const DecoderSignals& dec_sig,
            const RstSignals& rst_sig1,
            BusTristates& bus_tri);

  /*p07.BURO*/ Reg2 FF60_0;
  /*p07.AMUT*/ Reg2 FF60_1;
  /*p25.SOTO*/ Reg2 SOTO_DBG;
};

//-----------------------------------------------------------------------------

};