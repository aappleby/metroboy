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
  /*p23.GAVE*/ Reg2 GAVE_SCY0 = Reg2::D0C0;
  /*p23.FYMO*/ Reg2 FYMO_SCY1 = Reg2::D0C0;
  /*p23.FEZU*/ Reg2 FEZU_SCY2 = Reg2::D0C0;
  /*p23.FUJO*/ Reg2 FUJO_SCY3 = Reg2::D0C0;
  /*p23.DEDE*/ Reg2 DEDE_SCY4 = Reg2::D0C0;
  /*p23.FOTY*/ Reg2 FOTY_SCY5 = Reg2::D0C0;
  /*p23.FOHA*/ Reg2 FOHA_SCY6 = Reg2::D0C0;
  /*p23.FUNY*/ Reg2 FUNY_SCY7 = Reg2::D0C0;

  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ Reg2 DATY_SCX0 = Reg2::D0C0;
  /*p23.DUZU*/ Reg2 DUZU_SCX1 = Reg2::D0C0;
  /*p23.CYXU*/ Reg2 CYXU_SCX2 = Reg2::D0C0;
  /*p23.GUBO*/ Reg2 GUBO_SCX3 = Reg2::D0C0;
  /*p23.BEMY*/ Reg2 BEMY_SCX4 = Reg2::D0C0;
  /*p23.CUZY*/ Reg2 CUZY_SCX5 = Reg2::D0C0;
  /*p23.CABU*/ Reg2 CABU_SCX6 = Reg2::D0C0;
  /*p23.BAKE*/ Reg2 BAKE_SCX7 = Reg2::D0C0;

private:
  /*p27.LAXE*/ wire LAXE_BFETCH_S0n() const { return not(_LAXU_BFETCH_S0.q()); }
  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn() const { return nand(_LAXU_BFETCH_S0.q(), _NYVA_BFETCH_S2.q(), _NYXU_TILE_FETCHER_RSTn); }

  Sig2 _NYDY_LATCH_TILE_DAp;
  Sig2 _MOFU_LATCH_TILE_DBn;

  Sig2 _XYMU_RENDERINGp;
  Sig2 _NYXU_TILE_FETCHER_RSTn;

  /*p27.LAXU*/ Reg2 _LAXU_BFETCH_S0 = Reg2::D0C0;
  /*p27.MESU*/ Reg2 _MESU_BFETCH_S1 = Reg2::D0C0;
  /*p27.NYVA*/ Reg2 _NYVA_BFETCH_S2 = Reg2::D0C0;

  /*p27.LOVY*/ Reg2 _LOVY_FETCH_DONEp = Reg2::D0C0;

  /*p24.NYKA*/ Reg2 _NYKA_FETCH_DONE_Ap = Reg2::D0C0;
  /*p24.PORY*/ Reg2 _PORY_FETCH_DONE_Bp = Reg2::D0C0;
  /*p27.LYZU*/ Reg2 _LYZU_BFETCH_S0_DELAY = Reg2::D0C0;

  /*p24.PYGO*/ Reg2 _PYGO_FETCH_DONE_Cp = Reg2::D0C0;
  /*p24.POKY*/ Reg2 _POKY_PORCH_DONEp = Reg2::D0C0;            // stops clkpipe until after first tile fetch
  /*p27.LONY*/ Reg2 _LONY_BG_READ_VRAM_LATCHp = Reg2::D0C0;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics