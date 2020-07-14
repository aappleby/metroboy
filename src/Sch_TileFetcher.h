#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct VramBus;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct TileFetcher {
  void tick(const SchematicTop& top);
  void tock(SchematicTop& top, CpuBus& cpu_bus);
  SignalHash commit();

  // not sure about this one
  // ROMO = not(POKY_04)
  // SUVU = nand(XYMU, ROMO, NYKA_17, PORY_17).... hmmmm

  // -> sprite fetcher, top.TEVO
  /*p27.TAVE*/ wire TAVE_PORCH_DONE_TRIGp() const {
    /*p27.ROMO*/ wire _ROMO_PORCH_DONEn = not(_POKY_PORCH_DONEp.q());
    /*p27.SUVU*/ wire _SUVU_PORCH_ENDn = nand(_XYMU_RENDERINGp, _ROMO_PORCH_DONEn, _NYKA_FETCH_DONE_Ap.q(), _PORY_FETCH_DONE_Bp.q());
    return  not(_SUVU_PORCH_ENDn);
  }

  // -> top.TYFA
  /*p24.POKY*/ wire POKY_PORCH_DONEp()        const { return _POKY_PORCH_DONEp.q(); }

  // -> pix pipe
  /*p24.PORY*/ wire PORY_TILE_FETCH_DONE_Bp() const { return _PORY_FETCH_DONE_Bp.q(); }

  // -> sprite fetcher
  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp() const { return not(MOCE_BFETCH_DONEn()); }

  // -> vram bus
  /*p27.MYMA*/ wire MYMA_BGW_VRAM_RDn() const { return not(_LONY_BG_READ_VRAM_LATCHp.q()); }

  // -> vram bus
  wire LENA_BGW_VRM_RDp() const {
    /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(_LONY_BG_READ_VRAM_LATCHp.q());
    /*p27.LENA*/ wire LENA_BGW_VRM_RDp = not(LUSU_BGW_VRAM_RDn);
    return LENA_BGW_VRM_RDp;
  }

  // -> vram bus
  /*p27.NYDY*/ wire NYDY_LATCH_TILE_DAp() const { return _NYDY_LATCH_TILE_DAp; }

  // -> vram bus
  /*p27.MOFU*/ wire MOFU_LATCH_TILE_DBn() const { return _MOFU_LATCH_TILE_DBn; }

  // -> vram bus
  /*p27.NAKO*/ wire NAKO_BFETCH_S1n() const { return not(_MESU_BFETCH_S1.q()); }

  // -> vram bus
  /*p27.NOFU*/ wire NOFU_BFETCH_S2n() const { return not(_NYVA_BFETCH_S2.q()); }

  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ Reg9 GAVE_SCY0;
  /*p23.FYMO*/ Reg9 FYMO_SCY1;
  /*p23.FEZU*/ Reg9 FEZU_SCY2;
  /*p23.FUJO*/ Reg9 FUJO_SCY3;
  /*p23.DEDE*/ Reg9 DEDE_SCY4;
  /*p23.FOTY*/ Reg9 FOTY_SCY5;
  /*p23.FOHA*/ Reg9 FOHA_SCY6;
  /*p23.FUNY*/ Reg9 FUNY_SCY7;

  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ Reg9 DATY_SCX0;
  /*p23.DUZU*/ Reg9 DUZU_SCX1;
  /*p23.CYXU*/ Reg9 CYXU_SCX2;
  /*p23.GUBO*/ Reg9 GUBO_SCX3;
  /*p23.BEMY*/ Reg9 BEMY_SCX4;
  /*p23.CUZY*/ Reg9 CUZY_SCX5;
  /*p23.CABU*/ Reg9 CABU_SCX6;
  /*p23.BAKE*/ Reg9 BAKE_SCX7;

private:
  /*p27.LAXE*/ wire LAXE_BFETCH_S0n() const { return not(_LAXU_BFETCH_S0.q()); }
  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn() const { return nand(_LAXU_BFETCH_S0.q(), _NYVA_BFETCH_S2.q(), _NYXU_TILE_FETCHER_RSTn); }

  Signal _NYDY_LATCH_TILE_DAp;
  Signal _MOFU_LATCH_TILE_DBn;

  Signal _XYMU_RENDERINGp;
  Signal _NYXU_TILE_FETCHER_RSTn;

  /*p27.LAXU*/ Reg17 _LAXU_BFETCH_S0;
  /*p27.MESU*/ Reg17 _MESU_BFETCH_S1;
  /*p27.NYVA*/ Reg17 _NYVA_BFETCH_S2;

  /*p27.LOVY*/ Reg17 _LOVY_FETCH_DONEp;

  /*p24.NYKA*/ Reg17 _NYKA_FETCH_DONE_Ap;
  /*p24.PORY*/ Reg17 _PORY_FETCH_DONE_Bp;
  /*p27.LYZU*/ Reg17 _LYZU_BFETCH_S0_DELAY;

  /*p24.PYGO*/ Reg17 _PYGO_FETCH_DONE_Cp;
  /*p24.POKY*/ NorLatch _POKY_PORCH_DONEp;            // stops clkpipe until after first tile fetch
  /*p27.LONY*/ NandLatch _LONY_BG_READ_VRAM_LATCHp;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics