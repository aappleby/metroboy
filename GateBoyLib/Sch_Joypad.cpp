#include "GateBoyLib/Sch_Joypad.h"

#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void Joypad::dump(Dumper& d) const {
  d("\002===== Joypad =====\001\n");
  d("PIN_JOY_P10      %c\n", PIN_JOY_P10.c());
  d("PIN_JOY_P11      %c\n", PIN_JOY_P11.c());
  d("PIN_JOY_P12      %c\n", PIN_JOY_P12.c());
  d("PIN_JOY_P13      %c\n", PIN_JOY_P13.c());
  d("PIN_JOY_P14      %c\n", PIN_JOY_P14.c());
  d("PIN_JOY_P15      %c\n", PIN_JOY_P15.c());
  d("PIN_CPU_WAKE     %c\n", PIN_CPU_WAKE .c());
  d("ASOK_INT_JOYp    %c\n", ASOK_INT_JOYp.c());
  d("AWOB_WAKE_CPU    %c\n", AWOB_WAKE_CPU.c());
  d("BATU_JP_GLITCH0  %c\n", BATU_JP_GLITCH0.c());
  d("ACEF_JP_GLITCH1  %c\n", ACEF_JP_GLITCH1.c());
  d("AGEM_JP_GLITCH2  %c\n", AGEM_JP_GLITCH2.c());
  d("APUG_JP_GLITCH3  %c\n", APUG_JP_GLITCH3.c());
  d("JUTE_JOYP_RA     %c\n", JUTE_JOYP_RA.c());
  d("KECY_JOYP_LB     %c\n", KECY_JOYP_LB.c());
  d("JALE_JOYP_UC     %c\n", JALE_JOYP_UC.c());
  d("KYME_JOYP_DS     %c\n", KYME_JOYP_DS.c());
  d("KELY_JOYP_UDLR   %c\n", KELY_JOYP_UDLR.c());
  d("COFY_JOYP_ABCS   %c\n", COFY_JOYP_ABCS.c());
  d("KUKO_DBG_FF00_D6 %c\n", KUKO_DBG_FF00_D6.c());
  d("KERU_DBG_FF00_D7 %c\n", KERU_DBG_FF00_D7.c());
  d("KEVU_JOYP_L0     %c\n", KEVU_JOYP_L0.c());
  d("KAPA_JOYP_L1     %c\n", KAPA_JOYP_L1.c());
  d("KEJA_JOYP_L2     %c\n", KEJA_JOYP_L2.c());
  d("KOLO_JOYP_L3     %c\n", KOLO_JOYP_L3.c());
  d("\n");
}

//-----------------------------------------------------------------------------

void Joypad::set_buttons(uint8_t buttons) {
  // Pressing a button pulls the corresponding pin _down_.

  PIN_JOY_P10.lock(DELTA_TRIZ);
  PIN_JOY_P11.lock(DELTA_TRIZ);
  PIN_JOY_P12.lock(DELTA_TRIZ);
  PIN_JOY_P13.lock(DELTA_TRIZ);

  if (PIN_JOY_P14.qp()) {
    if (buttons & 0x01) PIN_JOY_P10.lock(DELTA_TRI0);
    if (buttons & 0x02) PIN_JOY_P11.lock(DELTA_TRI0);
    if (buttons & 0x04) PIN_JOY_P12.lock(DELTA_TRI0);
    if (buttons & 0x08) PIN_JOY_P13.lock(DELTA_TRI0);
  }

  if (PIN_JOY_P15.qp()) {
    if (buttons & 0x10) PIN_JOY_P10.lock(DELTA_TRI0);
    if (buttons & 0x20) PIN_JOY_P11.lock(DELTA_TRI0);
    if (buttons & 0x40) PIN_JOY_P12.lock(DELTA_TRI0);
    if (buttons & 0x80) PIN_JOY_P13.lock(DELTA_TRI0);
  }
}

//------------------------------------------------------------------------------
