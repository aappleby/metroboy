#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct TileFetcherSignals {
  /*p27.LYRY*/ Signal LYRY_BFETCH_DONEp;

  /*p27.TAVE*/ Signal TAVE_PORCH_ENDp;
  /*p24.POKY*/ Signal POKY_AFTER_PORCH_LATCHp;
  /*p24.PORY*/ Signal PORY_BFETCH_DONE_SYNC_DELAY;

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
};

//-----------------------------------------------------------------------------

struct TileFetcher {
  TileFetcherSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

private:

  /*p27.LAXU*/ Reg LAXU_BFETCH_S0;
  /*p27.MESU*/ Reg MESU_BFETCH_S1;
  /*p27.NYVA*/ Reg NYVA_FETCH_TILE_AB;

  /*p27.LOVY*/ Reg LOVY_BG_SEQ5_SYNC;

  /*p24.NYKA*/ Reg NYKA_BFETCH_DONE_SYNC;
  /*p24.PORY*/ Reg PORY_BFETCH_DONE_SYNC_DELAY;
  /*p27.LYZU*/ Reg LYZU_BFETCH_S0_DELAY;

  /*p24.PYGO*/ Reg PYGO_TILE_DONE;
  /*p24.POKY*/ NorLatch POKY_AFTER_PORCH_LATCHp;
  /*p27.LONY*/ NandLatch LONY_BG_READ_VRAM_LATCHp;

  /*p32.LEGU*/ Reg BG_PIX_A0; // 8-rung
  /*p32.NUDU*/ Reg BG_PIX_A1;
  /*p32.MUKU*/ Reg BG_PIX_A2;
  /*p32.LUZO*/ Reg BG_PIX_A3;
  /*p32.MEGU*/ Reg BG_PIX_A4;
  /*p32.MYJY*/ Reg BG_PIX_A5;
  /*p32.NASA*/ Reg BG_PIX_A6;
  /*p32.NEFO*/ Reg BG_PIX_A7;

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