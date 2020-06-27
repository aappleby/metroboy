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

  /*p04.MAKA*/ wire MAKA_FROM_CPU5_SYNC;
  /*p04.DECY*/ wire DECY_FROM_CPU5n;
  /*p28.LEKO*/ wire LEKO_CPU_RDp;

  /*p08.MATE*/ wire MATE_LATCH_CPU_ADDRp;
  /*p08.LAVO*/ wire LAVO_LATCH_CPU_DATAp;

  /*p08.RORU*/ wire RORU_IBUS_TO_EBUSn;
};

struct CpuRegisters {
  CpuSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  /*p04.MAKA*/ Reg MAKA_FROM_CPU5_SYNC;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics