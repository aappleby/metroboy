#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void tick(SchematicTop& gb);
  void tock(SchematicTop& gb);
  SignalHash commit();

  /*p29.TYTU*/ wire TYTU_SFETCH_S0n() const { return not(_TOXE_SFETCH_S0.q()); }
  /*p29.SAKY*/ wire SAKY() const            { return nor(_TULY_SFETCH_S1.q(), _VONU_SFETCH_S1_D4.q()); }

  wire VUSA_SPRITE_DONEn() const {
    /*p29.TYNO*/ wire TYNO = nand(_TOXE_SFETCH_S0.q(), _SEBA_SFETCH_S1_D5.q(), _VONU_SFETCH_S1_D4.q());
    /*p29.VUSA*/ wire VUSA_SPRITE_DONEn = or(_TYFO_SFETCH_S0_D1.qn(), TYNO);
    return VUSA_SPRITE_DONEn;
  }

  /*p29.WUTY*/ wire WUTY_SPRITE_DONEp() const { return not(VUSA_SPRITE_DONEn()); }
  /*p29.XEFY*/ wire XEPY_SPRITE_DONEn() const { return not(WUTY_SPRITE_DONEp()); }

  wire VAPE_FETCH_OAM_CLK() const {
    /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(_TEPA_RENDERINGn, _TULY_SFETCH_S1.q(), _TESE_SFETCH_S2.q());
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(_TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0n());
    /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and (TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
    return VAPE_FETCH_OAM_CLK;
  }

  /*p25.XUJY*/ wire XUJY_PPU_OAM_CLK() const { 
    return not(VAPE_FETCH_OAM_CLK());
  }

  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG() const { return nand(_TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0n()); }


  /*p28.WEFY*/ wire WEFY_SPR_READp() const {
    /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(_TEPA_RENDERINGn, _TULY_SFETCH_S1.q(), _TESE_SFETCH_S2.q());
    return and(TUVO_PPU_OAM_RDp, _TYFO_SFETCH_S0_D1.q());
  }

  /*p28.XUJA*/ wire XUJA_SPR_OAM_LATCHn() const { return not(WEFY_SPR_READp()); }

  /*p33.PEFO*/ Reg8 SPR_PIX_A0;
  /*p33.ROKA*/ Reg8 SPR_PIX_A1;
  /*p33.MYTU*/ Reg8 SPR_PIX_A2;
  /*p33.RAMU*/ Reg8 SPR_PIX_A3;
  /*p33.SELE*/ Reg8 SPR_PIX_A4;
  /*p33.SUTO*/ Reg8 SPR_PIX_A5;
  /*p33.RAMA*/ Reg8 SPR_PIX_A6;
  /*p33.RYDU*/ Reg8 SPR_PIX_A7;

  /*p33.REWO*/ Reg8 SPR_PIX_B0;
  /*p33.PEBA*/ Reg8 SPR_PIX_B1;
  /*p33.MOFO*/ Reg8 SPR_PIX_B2;
  /*p33.PUDU*/ Reg8 SPR_PIX_B3;
  /*p33.SAJA*/ Reg8 SPR_PIX_B4;
  /*p33.SUNY*/ Reg8 SPR_PIX_B5;
  /*p33.SEMO*/ Reg8 SPR_PIX_B6;
  /*p33.SEGA*/ Reg8 SPR_PIX_B7;

private:

  Signal _TEPA_RENDERINGn;

  /*p27.TAKA*/ NandLatch _TAKA_SFETCH_RUNNINGp;
  /*p27.SOBU*/ Reg17 _SOBU_SPRITE_FETCH_A;
  /*p27.SUDA*/ Reg17 _SUDA_SPRITE_FETCH_B;

  // When TOXE & TYFO & VONU & SEBA, sprites are loaded into the pipe.

  /*p29.TOXE*/ Reg17 _TOXE_SFETCH_S0;
  /*p29.TYFO*/ Reg17 _TYFO_SFETCH_S0_D1;

  /*p29.TULY*/ Reg17 _TULY_SFETCH_S1;
  /*p29.TESE*/ Reg17 _TESE_SFETCH_S2;

  /*p29.TOBU*/ Reg17 _TOBU_SFETCH_S1_D2;
  /*p29.VONU*/ Reg17 _VONU_SFETCH_S1_D4;
  /*p29.SEBA*/ Reg17 _SEBA_SFETCH_S1_D5;

};


//-----------------------------------------------------------------------------

}; // namespace Schematics