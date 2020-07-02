#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct TileFetcherSignals {
  /*p27.LYRY*/ Signal LYRY_BFETCH_DONEp;             // to ppu TEKY_SPRITE_FETCH

  /*p27.TAVE*/ Signal TAVE_PORCH_DONE_TRIGp;               // to ppu TEVO
  /*p24.POKY*/ Signal POKY_PORCH_DONEp;       // to TYFA_CLKPIPEp_xBxDxFxH
  /*p24.PORY*/ Signal PORY_FETCH_DONE_Bp;   // to RYDY_WIN_HIT_LATCHp? that doesn't seem right

  /*p27.NYXU*/ Signal NYXU_TILE_FETCHER_RSTn;

  /*p27.LONY*/ Signal LONY_BG_READ_VRAM_LATCHp;

  /*p32.LEGU*/ Signal BG_PIX_A0; // 8-rung
  /*p32.NUDU*/ Signal BG_PIX_A1;
  /*p32.MUKU*/ Signal BG_PIX_A2;
  /*p32.LUZO*/ Signal BG_PIX_A3;
  /*p32.MEGU*/ Signal BG_PIX_A4;
  /*p32.MYJY*/ Signal BG_PIX_A5;
  /*p32.NASA*/ Signal BG_PIX_A6;
  /*p32.NEFO*/ Signal BG_PIX_A7;

  /*p32.RAWU*/ Signal BG_PIX_B0; // 11-rung, also holds tile index during fetch
  /*p32.POZO*/ Signal BG_PIX_B1; // 11-rung
  /*p32.PYZO*/ Signal BG_PIX_B2; 
  /*p32.POXA*/ Signal BG_PIX_B3; 
  /*p32.PULO*/ Signal BG_PIX_B4; 
  /*p32.POJU*/ Signal BG_PIX_B5; 
  /*p32.POWY*/ Signal BG_PIX_B6; 
  /*p32.PYJU*/ Signal BG_PIX_B7;

  /*p32.LOZE*/ Signal LOZE_PIPE_A_LOAD;
  /*p32.LUXA*/ Signal LUXA_PIPE_B_LOAD;
};

//-----------------------------------------------------------------------------

struct TileFetcher {
  TileFetcherSignals sig(const SchematicTop& gb) const;
  void tick(SchematicTop& gb);
  SignalHash commit();

private:

  /*p27.LAXU*/ Reg LAXU_BFETCH_S0;
  /*p27.MESU*/ Reg MESU_BFETCH_S1;
  /*p27.NYVA*/ Reg NYVA_BFETCH_S2;

  /*p27.LOVY*/ Reg LOVY_FETCH_DONEp;

  /*p24.NYKA*/ Reg NYKA_FETCH_DONE_Ap;
  /*p24.PORY*/ Reg PORY_FETCH_DONE_Bp;
  /*p27.LYZU*/ Reg LYZU_BFETCH_S0_DELAY;

  /*p24.PYGO*/ Reg PYGO_FETCH_DONE_Cp;
  /*p24.POKY*/ NorLatch POKY_PORCH_DONEp;            // stops clkpipe until after first tile fetch
  /*p27.LONY*/ NandLatch LONY_BG_READ_VRAM_LATCHp;

  /*p32.LEGU*/ Reg8 BG_PIX_A0; // 8-rung
  /*p32.NUDU*/ Reg8 BG_PIX_A1;
  /*p32.MUKU*/ Reg8 BG_PIX_A2;
  /*p32.LUZO*/ Reg8 BG_PIX_A3;
  /*p32.MEGU*/ Reg8 BG_PIX_A4;
  /*p32.MYJY*/ Reg8 BG_PIX_A5;
  /*p32.NASA*/ Reg8 BG_PIX_A6;
  /*p32.NEFO*/ Reg8 BG_PIX_A7; // color wrong on die

  /*p32.RAWU*/ Reg BG_PIX_B0; // 11-rung, also holds tile index during fetch
  /*p32.POZO*/ Reg BG_PIX_B1; // 11-rung
  /*p32.PYZO*/ Reg BG_PIX_B2; 
  /*p32.POXA*/ Reg BG_PIX_B3; 
  /*p32.PULO*/ Reg BG_PIX_B4; 
  /*p32.POJU*/ Reg BG_PIX_B5; 
  /*p32.POWY*/ Reg BG_PIX_B6; 
  /*p32.PYJU*/ Reg BG_PIX_B7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics