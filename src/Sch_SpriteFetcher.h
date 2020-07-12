#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void tick(SchematicTop& gb);
  void tock(SchematicTop& gb);
  SignalHash commit();

  /*p29.TYTU*/ wire TYTU_SFETCH_S0n() const { return not(TOXE_SFETCH_S0.q()); }
  /*p29.SAKY*/ wire SAKY() const            { return nor(TULY_SFETCH_S1.q(), VONU_SFETCH_S1_D4.q()); }

  wire VUSA_SPRITE_DONEn() const {
    // TYNO_01 << TOXE_17
    // TYNO_02 << SEBA_17
    // TYNO_03 << VONU_17
    // TYNO_04 >> VUSA_02

    // VUSA_01 << TYFO_16
    // VUSA_02 << TYNO_04
    // VUSA_03 nc
    // VUSA_04 >>

    /*p29.TYNO*/ wire TYNO = nand(TOXE_SFETCH_S0.q(), SEBA_SFETCH_S1_D5.q(), VONU_SFETCH_S1_D4.q());
    /*p29.VUSA*/ wire VUSA_SPRITE_DONEn = or(TYFO_SFETCH_S0_D1.qn(), TYNO);
    return VUSA_SPRITE_DONEn;
  }

  /*p29.WUTY*/ wire WUTY_SPRITE_DONEp() const { return not(VUSA_SPRITE_DONEn()); }
  /*p29.XEFY*/ wire XEPY_SPRITE_DONEn() const { return not(WUTY_SPRITE_DONEp()); }

  wire VAPE_FETCH_OAM_CLK(wire TEPA_RENDERINGn) const {
    /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, TULY_SFETCH_S1.q(), TESE_SFETCH_S2.q());
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0n());
    /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and (TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
    return VAPE_FETCH_OAM_CLK;
  }

  /*p25.XUJY*/ wire XUJY_PPU_OAM_CLK(wire TEPA_RENDERINGn) const { 
    return not(VAPE_FETCH_OAM_CLK(TEPA_RENDERINGn));
  }

  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG() const { return nand(TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0n()); }

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUNNINGp;
  /*p27.SOBU*/ Reg17 SOBU_SPRITE_FETCH_A;
  /*p27.SUDA*/ Reg17 SUDA_SPRITE_FETCH_B;

  // When TOXE & TYFO & VONU & SEBA, sprites are loaded into the pipe.

  /*p29.TOXE*/ Reg17 TOXE_SFETCH_S0;
  /*p29.TYFO*/ Reg17 TYFO_SFETCH_S0_D1;

  /*p29.TULY*/ Reg17 TULY_SFETCH_S1;
  /*p29.TESE*/ Reg17 TESE_SFETCH_S2;

  /*p29.TOBU*/ Reg17 TOBU_SFETCH_S1_D2;
  /*p29.VONU*/ Reg17 VONU_SFETCH_S1_D4;
  /*p29.SEBA*/ Reg17 SEBA_SFETCH_S1_D5;

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
};


//-----------------------------------------------------------------------------

}; // namespace Schematics