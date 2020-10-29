#include "GateBoyLib/Sch_Timer.h"

#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

void Timer::dump(Dumper& d) const {
  d("\002===== Timer =====\001\n");
  d("DIV    : 0x%04x %d\n", get_div(), get_div());
  d("TIMA A : 0x%02x %d\n", get_tima_a(), get_tima_a());
  d("TIMA B : 0x%02x %d\n", get_tima_b(), get_tima_b());
  d("TMA    : 0x%02x %d\n", get_tma(), get_tma());
  d("TAC    : 0x%02x %d\n", get_tac(), get_tac());
  d("NYDU_TIMA_D7_DELAY   %c\n", NYDU_TIMA_D7_DELAY.c());
  d("MOBA_TIMER_OVERFLOWp %c\n", MOBA_TIMER_OVERFLOWp.c());
  d("\n");
}


//------------------------------------------------------------------------------

