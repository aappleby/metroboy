#include "GateBoyLib/Sch_SpriteFetcher.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4189)

//------------------------------------------------------------------------------

void SpriteFetcher::dump(Dumper& d) const {
  d("\002===== SpriteFetch =====\001\n");
  d("SOBU_SFETCH_REQp     %c\n", SOBU_SFETCH_REQp    .c());
  d("SUDA_SFETCH_REQp     %c\n", SUDA_SFETCH_REQp    .c());
  d("\n");
  d("TAKA_SFETCH_RUNNINGp %c\n", TAKA_SFETCH_RUNNINGp.c());
  d("\n");
  d("TOXE_SFETCH_S0       %c\n", TOXE_SFETCH_S0      .c());
  d("TYFO_SFETCH_S0_D1    %c\n", TYFO_SFETCH_S0_D1   .c());
  d("\n");
  d("TULY_SFETCH_S1       %c\n", TULY_SFETCH_S1      .c());
  d("TOBU_SFETCH_S1_D2    %c\n", TOBU_SFETCH_S1_D2   .c());
  d("VONU_SFETCH_S1_D4    %c\n", VONU_SFETCH_S1_D4   .c());
  d("SEBA_SFETCH_S1_D5    %c\n", SEBA_SFETCH_S1_D5   .c());
  d("\n");
  d("TESE_SFETCH_S2       %c\n", TESE_SFETCH_S2      .c());
  d("\n");
}

//------------------------------------------------------------------------------
