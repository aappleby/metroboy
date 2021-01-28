#include "GateBoyLib/GateBoySpriteScanner.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

wire GateBoy::ACYL_SCANNINGp() {
  /*#p28.ACYL*/ wire ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn(), sprite_scanner.BESU_SCANNINGp.qp_new());
  return ACYL_SCANNINGp;
}

//------------------------------------------------------------------------------------------------------------------------