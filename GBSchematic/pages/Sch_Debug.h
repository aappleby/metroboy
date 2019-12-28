#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Debug {

  void tick(const Bus& bus,
            const BusControl& ctl,
            const ClockSignals& clk,
            const Pins& pins,
            const Decoder& dec,
            const ResetRegisters& rst_reg,
            const Debug& b,
            Bus& bus_out);

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

  /*p07.BURO*/ Reg FF60_0;
  /*p07.AMUT*/ Reg FF60_1;
  /*p05.KURA*/ bool FF60_0n;

  /*p25.SOTO*/ Reg DBG_SOTO;
  /*p25.TUTO*/ bool DBG_VRAM;
  /*p25.RACO*/ bool DBG_VRAMn;

};

//-----------------------------------------------------------------------------

};