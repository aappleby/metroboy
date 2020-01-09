#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct JoypadSignals;
struct TimerSignals;

struct InterruptSignals {
};

struct InterruptRegisters {

  void tick(const SysSignals& sys_sig,
            const BusSignals& ctl,
            const CpuPins& cpu,
            const LcdSignals& lcd_sig,
            const SerialSignals& ser_sig,
            const JoypadSignals& joy_sig,
            const VidSignals& vid_sig,
            const TimerSignals& tim_sig,
            const RstSignals& rst_sig,
            const DecoderSignals& dec,
            BusTristates& bus_tri);
  /*
  Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
  Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
  Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
  Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
  Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
  */

  /*p02.LOPE*/ Reg2 FF0F_0;
  /*p02.UBUL*/ Reg2 FF0F_1;
  /*p02.ULAK*/ Reg2 FF0F_2;
  /*p02.LALU*/ Reg2 FF0F_3;
  /*p02.NYBO*/ Reg2 FF0F_4;

  /*p02.MATY*/ Reg2 FF0F_L0;
  /*p02.NEJY*/ Reg2 FF0F_L1;
  /*p02.NUTY*/ Reg2 FF0F_L2;
  /*p02.MOPO*/ Reg2 FF0F_L3;
  /*p02.PAVY*/ Reg2 FF0F_L4;

};

};