#include "GateBoyLib/GateBoySpriteScanner.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

wire GateBoy::ACYL_SCANNINGp() {
  /*#p28.ACYL*/ wire ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn(), sprite_scanner.BESU_SCANNINGp.qp_new());
  return ACYL_SCANNINGp;
}

wire GateBoy::AVAP_SCAN_DONE_TRIGp() {
  /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp_new(), sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
  /*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  return AVAP_SCAN_DONE_TRIGp;
}

//------------------------------------------------------------------------------------------------------------------------