#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct CpuSignals {
  /*p07.TEDO*/ wire TEDO_CPU_RD;
  /*p07.ASOT*/ wire ASOT_CPU_RD;

  /*p08.MOCA*/ wire MOCA_DBG_EXT_RD;
  /*p08.MOTY*/ wire MOTY_CPU_EXT_RD;

  /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH;
  /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH;
  /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH;

  /*p04.DECY*/ wire DECY_FROM_CPU5n;
};

struct CpuRegisters {
  CpuSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();
};

//-----------------------------------------------------------------------------

}; // namespace Schematics