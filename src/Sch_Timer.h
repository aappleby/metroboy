#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct TimerSignals {
  static TimerSignals get(TestGB& gb);

  /*p01.UVYN*/ wire UVYN_DIV_05n;
  /*p01.UMEK*/ wire UMEK_DIV_06n;
  /*p01.UREK*/ wire UREK_DIV_07n;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics