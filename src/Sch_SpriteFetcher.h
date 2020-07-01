#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct SpriteFetcherSignals {
  /*p33.PEFO*/ Signal SPR_PIX_A0;
  /*p33.ROKA*/ Signal SPR_PIX_A1;
  /*p33.MYTU*/ Signal SPR_PIX_A2;
  /*p33.RAMU*/ Signal SPR_PIX_A3;
  /*p33.SELE*/ Signal SPR_PIX_A4;
  /*p33.SUTO*/ Signal SPR_PIX_A5;
  /*p33.RAMA*/ Signal SPR_PIX_A6;
  /*p33.RYDU*/ Signal SPR_PIX_A7;

  /*p33.REWO*/ Signal SPR_PIX_B0;
  /*p33.PEBA*/ Signal SPR_PIX_B1;
  /*p33.MOFO*/ Signal SPR_PIX_B2;
  /*p33.PUDU*/ Signal SPR_PIX_B3;
  /*p33.SAJA*/ Signal SPR_PIX_B4;
  /*p33.SUNY*/ Signal SPR_PIX_B5;
  /*p33.SEMO*/ Signal SPR_PIX_B6;
  /*p33.SEGA*/ Signal SPR_PIX_B7;

  /*p29.TYFO*/ Signal TYFO_SFETCH_S0_D1;
  /*p29.TYTU*/ Signal TYTU_SFETCH_S0_D0n;
  /*p29.TACU*/ Signal TACU_SPR_SEQ_5_TRIG;
  /*p29.TOPU*/ Signal TOPU_LATCH_SPPIXA;
  /*p29.RACA*/ Signal RACA_LATCH_SPPIXB;
  /*p29.TUVO*/ Signal TUVO_PPU_OAM_RDp;
  /*p28.XUJA*/ Signal XUJA_SPR_READn;
  /*p25.XUJY*/ Signal XUJY;
  /*p29.SAKY*/ Signal SAKY;
  /*p29.WUTY*/ Signal WUTY_SPRITE_DONEp;
  /*p27.SOWO*/ Signal SOWO_SPRITE_FETCH_LATCHn;
  /*p29.TEXY*/ Signal TEXY_SPRITE_READp;
};

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  SpriteFetcherSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

//private:

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUN_LATCH;
  /*p27.SOBU*/ Reg SOBU_SPRITE_FETCH_SYNC1;
  /*p27.SUDA*/ Reg SUDA_SPRITE_FETCH_SYNC2;

  /*p29.TOXE*/ Reg TOXE_SFETCH_S0;
  /*p29.TULY*/ Reg TULY_SFETCH_S1;
  /*p29.TESE*/ Reg TESE_SFETCH_S2;

  /*p29.TOBU*/ Reg TOBU_SFETCH_S1_D2;
  /*p29.VONU*/ Reg VONU_SFETCH_S1_D4;
  /*p29.SEBA*/ Reg SEBA_SFETCH_S1_D5;
  /*p29.TYFO*/ Reg TYFO_SFETCH_S0_D1;

  /*p33.PEFO*/ Reg SPR_PIX_A0;
  /*p33.ROKA*/ Reg SPR_PIX_A1;
  /*p33.MYTU*/ Reg SPR_PIX_A2;
  /*p33.RAMU*/ Reg SPR_PIX_A3;
  /*p33.SELE*/ Reg SPR_PIX_A4;
  /*p33.SUTO*/ Reg SPR_PIX_A5;
  /*p33.RAMA*/ Reg SPR_PIX_A6;
  /*p33.RYDU*/ Reg SPR_PIX_A7;

  /*p33.REWO*/ Reg SPR_PIX_B0;
  /*p33.PEBA*/ Reg SPR_PIX_B1;
  /*p33.MOFO*/ Reg SPR_PIX_B2;
  /*p33.PUDU*/ Reg SPR_PIX_B3;
  /*p33.SAJA*/ Reg SPR_PIX_B4;
  /*p33.SUNY*/ Reg SPR_PIX_B5;
  /*p33.SEMO*/ Reg SPR_PIX_B6;
  /*p33.SEGA*/ Reg SPR_PIX_B7;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics