#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void reset_to_cart();

  void tock(GateBoyResetDebug& rst, GateBoyClock& clk, wire XYMU_RENDERINGp, wire ATEJ_LINE_RSTp, wire TAVE_PRELOAD_DONE_TRIGp, wire TEKY_SFETCH_REQp_old);

  /* p29.SAKY*/ wire SAKY_SFETCHn() const { return nor2(TULY_SFETCH_S1p.qp(), VONU_SFETCH_S1p_D4.qp()); }

  /* p29.TEXY*/ wire TEXY_SFETCHINGp() const {
    /* p29.TEPA*/ wire _TEPA_RENDERINGp = not1(_XYMU_RENDERINGp.qp());
    /* p29.TYSO*/ wire _TYSO_SFETCHINGn = or2(SAKY_SFETCHn(), _TEPA_RENDERINGp); // def or
    /* p29.TEXY*/ wire _TEXY_SFETCHINGp = not1(_TYSO_SFETCHINGn);
    return _TEXY_SFETCHINGp;
  }

  /* p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn() const { return not1(TAKA_SFETCH_RUNNINGp.qp()); }

  /* p29.TYTU*/ wire TYTU_SFETCH_S0n() const { return not1(TOXE_SFETCH_S0p.qp_new()); }

  /* p29.TUVO*/ wire TUVO_PPU_OAM_RDp() const {
    /* p29.TEPA*/ wire _TEPA_RENDERINGp = not1(_XYMU_RENDERINGp.qp());
    return nor3(_TEPA_RENDERINGp, TULY_SFETCH_S1p.qp_new(), TESE_SFETCH_S2p.qp_new());
  }

  wire TACU_SPR_SEQ_5_TRIG() const {
    /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG = nand2(TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n());
    return _TACU_SPR_SEQ_5_TRIG;
  }

  wire XUJY_OAM_CLKENp() const {
    /* p25.VAPE*/ wire _VAPE_OAM_CLKENn = and2(TUVO_PPU_OAM_RDp(), TACU_SPR_SEQ_5_TRIG());
    /* p25.XUJY*/ wire _XUJY_OAM_CLKENp = not1(_VAPE_OAM_CLKENn);
    return _XUJY_OAM_CLKENp;
  }


  wire XUJA_SPR_OAM_LATCHn() const {
    /* p28.WEFY*/ wire _WEFY_SPR_READp = and2(TUVO_PPU_OAM_RDp(), TYFO_SFETCH_S0p_D1.qp_new());
    /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(_WEFY_SPR_READp);
    return _XUJA_SPR_OAM_LATCHn;
  }

  /* p29.WUTY*/ wire WUTY_SFETCH_DONE_TRIGp() const {
    /* p29.TYNO*/ wire _TYNO = nand3(TOXE_SFETCH_S0p.qp_new(), SEBA_SFETCH_S1p_D5.qp_new(), VONU_SFETCH_S1p_D4.qp_new());
    /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn = or2(TYFO_SFETCH_S0p_D1.qn_new(), _TYNO);
    /* p29.WUTY*/ wire _WUTY_SFETCH_DONE_TRIGp = not1(_VUSA_SPRITE_DONEn);
    return _WUTY_SFETCH_DONE_TRIGp;
  }

  wire SYCU_SFETCH_S0pe() const {
    /* p24.LOBY*/ wire _LOBY_RENDERINGn = not1(_XYMU_RENDERINGp.qp());
    /* p29.SYCU*/ wire _SYCU_SFETCH_S0pe = nor3(TYTU_SFETCH_S0n(), _LOBY_RENDERINGn, TYFO_SFETCH_S0p_D1.qp_new());
    return _SYCU_SFETCH_S0pe;
  }

  wire XADO_STORE_SPRITE_An() const {
    /*#p29.TOPU*/ wire _TOPU_STORE_SPRITE_Ap = and2(TULY_SFETCH_S1p.qp_new(), SYCU_SFETCH_S0pe());
    /*#p29.VYWA*/ wire _VYWA_STORE_SPRITE_An = not1(_TOPU_STORE_SPRITE_Ap);
    /*#p29.WENY*/ wire _WENY_STORE_SPRITE_Ap = not1(_VYWA_STORE_SPRITE_An);
    /*#p29.XADO*/ wire _XADO_STORE_SPRITE_An = not1(_WENY_STORE_SPRITE_Ap);
    return _XADO_STORE_SPRITE_An;
  }

  wire PUCO_STORE_SPRITE_Bn() const {
    /*#p29.RACA*/ wire _RACA_STORE_SPRITE_Bp = and2(VONU_SFETCH_S1p_D4.qp_new(), SYCU_SFETCH_S0pe());
    /*#p29.PEBY*/ wire _PEBY_STORE_SPRITE_Bn = not1(_RACA_STORE_SPRITE_Bp);
    /*#p29.NYBE*/ wire _NYBE_STORE_SPRITE_Bp = not1(_PEBY_STORE_SPRITE_Bn);
    /*#p29.PUCO*/ wire _PUCO_STORE_SPRITE_Bn = not1(_NYBE_STORE_SPRITE_Bp);
    return _PUCO_STORE_SPRITE_Bn;
  }

  /*#p29.XUQU*/ wire XUQU_SPRITE_AB() const { return not1(VONU_SFETCH_S1p_D4.qn_new()); }

  wire SOHO_SPR_VRAM_RDp() const {
    /* p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp = and2(TACU_SPR_SEQ_5_TRIG(), TEXY_SFETCHINGp());
    return _SOHO_SPR_VRAM_RDp;
  }

  void dump(Dumper& d) {
    d.dump_bitp("TAKA_SFETCH_RUNNINGp", TAKA_SFETCH_RUNNINGp.state);
    d.dump_bitp("SOBU_SFETCH_REQp    ", SOBU_SFETCH_REQp    .state);
    d.dump_bitp("SUDA_SFETCH_REQp    ", SUDA_SFETCH_REQp    .state);
    d.dump_bitp("TOXE_SFETCH_S0      ", TOXE_SFETCH_S0p     .state);
    d.dump_bitp("TULY_SFETCH_S1      ", TULY_SFETCH_S1p     .state);
    d.dump_bitp("TESE_SFETCH_S2      ", TESE_SFETCH_S2p     .state);
    d.dump_bitp("TYFO_SFETCH_S0_D1   ", TYFO_SFETCH_S0p_D1  .state);
    d.dump_bitp("TOBU_SFETCH_S1_D2   ", TOBU_SFETCH_S1p_D2  .state);
    d.dump_bitp("VONU_SFETCH_S1_D4   ", VONU_SFETCH_S1p_D4  .state);
    d.dump_bitp("SEBA_SFETCH_S1_D5   ", SEBA_SFETCH_S1p_D5  .state);
  }

  Signal _XYMU_RENDERINGp;

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUNNINGp; // ABCDEFGH Set on odd, cleared on even
  /*p27.SOBU*/ DFF17 SOBU_SFETCH_REQp;         // xBxDxFxH
  /*p27.SUDA*/ DFF17 SUDA_SFETCH_REQp;         // AxBxExGx

  /*p29.TOXE*/ DFF17 TOXE_SFETCH_S0p;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.
  /*p29.TULY*/ DFF17 TULY_SFETCH_S1p;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.
  /*p29.TESE*/ DFF17 TESE_SFETCH_S2p;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.

  /*p29.TYFO*/ DFF17 TYFO_SFETCH_S0p_D1;       // AxCxExGx
  /*p29.TOBU*/ DFF17 TOBU_SFETCH_S1p_D2;       // xBxDxFxH
  /*p29.VONU*/ DFF17 VONU_SFETCH_S1p_D4;       // xBxDxFxH
  /*p29.SEBA*/ DFF17 SEBA_SFETCH_S1p_D5;       // AxCxExGx

  SpritePixA sprite_pix_a;
  SpritePixB sprite_pix_b;
};

//-----------------------------------------------------------------------------
