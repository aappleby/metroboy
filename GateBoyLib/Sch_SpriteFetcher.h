#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct SpriteFetcher {

  wire TACU_SPR_SEQ_5_TRIG() const {
    /*#p29.TYFO*/ wire _TYFO_SFETCH_S0p_D1 = TYFO_SFETCH_S0p_D1_evn.qp_new();
    /* p29.TYTU*/ wire _TYTU_SFETCH_S0n = not1(TOXE_SFETCH_S0p_odd.qp_new());
    /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG = nand2(_TYFO_SFETCH_S0p_D1, _TYTU_SFETCH_S0n);
    return _TACU_SPR_SEQ_5_TRIG;
  }

  wire XUJY_OAM_CLKENp(wire XYMU_RENDERINGp) const {
    /* p29.TEPA*/ wire _TEPA_RENDERINGp = not1(XYMU_RENDERINGp);
    /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp = nor3(_TEPA_RENDERINGp, TULY_SFETCH_S1p_odd.qp_new(), TESE_SFETCH_S2p_odd.qp_new());
    /* p25.VAPE*/ wire _VAPE_OAM_CLKENn = and2(_TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG());
    /* p25.XUJY*/ wire _XUJY_OAM_CLKENp = not1(_VAPE_OAM_CLKENn);
    return _XUJY_OAM_CLKENp;
  }


  wire XUJA_SPR_OAM_LATCHn(wire XYMU_RENDERINGp) const {
    /*#p29.TYFO*/ wire _TYFO_SFETCH_S0p_D1 = TYFO_SFETCH_S0p_D1_evn.qp_new();
    /* p29.TEPA*/ wire _TEPA_RENDERINGp = not1(XYMU_RENDERINGp);
    /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp = nor3(_TEPA_RENDERINGp, TULY_SFETCH_S1p_odd.qp_new(), TESE_SFETCH_S2p_odd.qp_new());
    /* p28.WEFY*/ wire _WEFY_SPR_READp = and2(_TUVO_PPU_OAM_RDp, _TYFO_SFETCH_S0p_D1);
    /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(_WEFY_SPR_READp);
    return _XUJA_SPR_OAM_LATCHn;
  }

  /* p29.WUTY*/ wire WUTY_SFETCH_DONE_TRIGp() const {
    /* p29.TYNO*/ wire _TYNO_new = nand3(TOXE_SFETCH_S0p_odd.qp_new(), SEBA_SFETCH_S1p_D5_evn.qp_new(), VONU_SFETCH_S1p_D4_odd.qp_new());
    /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn_new = or2(TYFO_SFETCH_S0p_D1_evn.qn_new(), _TYNO_new);
    /* p29.WUTY*/ wire _WUTY_SFETCH_DONE_TRIGp = not1(_VUSA_SPRITE_DONEn_new);
    return _WUTY_SFETCH_DONE_TRIGp;
  }

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUNNINGp_xxx; // ABCDEFGH Set on odd, cleared on even
  /*p27.SOBU*/ DFF17 SOBU_SFETCH_REQp_odd;         // xBxDxFxH
  /*p27.SUDA*/ DFF17 SUDA_SFETCH_REQp_evn;         // AxBxExGx

  /*p29.TOXE*/ DFF17 TOXE_SFETCH_S0p_odd;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.
  /*p29.TULY*/ DFF17 TULY_SFETCH_S1p_odd;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.
  /*p29.TESE*/ DFF17 TESE_SFETCH_S2p_odd;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.

  /*p29.TYFO*/ DFF17 TYFO_SFETCH_S0p_D1_evn;       // AxCxExGx
  /*p29.TOBU*/ DFF17 TOBU_SFETCH_S1p_D2_odd;       // xBxDxFxH
  /*p29.VONU*/ DFF17 VONU_SFETCH_S1p_D4_odd;       // xBxDxFxH
  /*p29.SEBA*/ DFF17 SEBA_SFETCH_S1p_D5_evn;       // AxCxExGx
};

//-----------------------------------------------------------------------------
