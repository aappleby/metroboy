#include "GateBoyLib/Sch_TileFetcher.h"
#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

#pragma warning(disable:4189)

//------------------------------------------------------------------------------

void TileFetcher::dump(Dumper& d, const SchematicTop& /*top*/) const {
  d("\002=====TileFetcher=====\001\n");
  d("LAXU_BFETCH_S0           %c\n", LAXU_BFETCH_S0.c());
  d("MESU_BFETCH_S1           %c\n", MESU_BFETCH_S1.c());
  d("NYVA_BFETCH_S2           %c\n", NYVA_BFETCH_S2.c());
  d("LYZU_BFETCH_S0_D1        %c\n", LYZU_BFETCH_S0_D1.c());
  d("\n");
  d("NYKA_FETCH_DONE_P11      %c\n", NYKA_FETCH_DONE_P11.c());
  d("PORY_FETCH_DONE_P12      %c\n", PORY_FETCH_DONE_P12.c());
  d("PYGO_FETCH_DONE_P13      %c\n", PYGO_FETCH_DONE_P13.c());
  d("POKY_PRELOAD_DONEp       %c\n", POKY_PRELOAD_LATCHp.c());
  d("\n");
  d("LONY_FETCH_RUNNINGp      %c\n", LONY_BG_FETCH_RUNNINGp.c()); // 1 for phases 0-11, 0 for 12-15
  d("LOVY_FETCH_DONEp         %c\n", LOVY_BG_FETCH_DONEp.c());    // 0 for phases 0-11, 1 for 12-15
  d("\n");
}

//------------------------------------------------------------------------------
