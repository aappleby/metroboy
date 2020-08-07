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
  void dump(Dumper& d, const SchematicTop& top) const;

  // not sure about this one
  // ROMO = not1(POKY_04)
  // SUVU = nand2(XYMU, ROMO, NYKA_17, PORY_17).... hmmmm

  // -> sprite fetcher, top.TEVO
  /*p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp() const {
    /*p27.ROMO*/ wire _ROMO_PRELOAD_DONEn = not1(_POKY_PRELOAD_DONEp.qp());
    /*p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn = nand4(_XYMU_RENDERINGp, _ROMO_PRELOAD_DONEn, _NYKA_FETCH_DONE_Ap.qp(), _PORY_FETCH_DONE_Bp.qp());
    return  not1(_SUVU_PRELOAD_DONE_TRIGn);
  }

  // -> top.TYFA
  /*p24.POKY*/ wire POKY_PRELOAD_DONEp()        const { return _POKY_PRELOAD_DONEp.qp(); }

  // -> pix pipe
  /*p24.PORY*/ wire PORY_TILE_FETCH_DONE_Bp() const { return _PORY_FETCH_DONE_Bp.qp(); }

  // -> sprite fetcher
  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp() const { return not1(MOCE_BFETCH_DONEn()); }

  // -> vram bus
  /*p27.MYMA*/ wire MYMA_BGW_VRAM_RDn() const { return not1(_LONY_BG_READ_VRAM_LATCHp.qp()); }

  // -> vram bus
  wire LENA_BGW_VRM_RDp() const {
    /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not1(_LONY_BG_READ_VRAM_LATCHp.qp());
    /*p27.LENA*/ wire LENA_BGW_VRM_RDp = not1(LUSU_BGW_VRAM_RDn);
    return LENA_BGW_VRM_RDp;
  }

  // -> vram bus
  /*p27.NYDY*/ wire NYDY_LATCH_TILE_DAp() const { return _NYDY_LATCH_TILE_DAp; }

  // -> vram bus
  /*p27.MOFU*/ wire MOFU_LATCH_TILE_DBn() const { return _MOFU_LATCH_TILE_DBn; }

  // -> vram bus
  /*p27.NAKO*/ wire NAKO_BFETCH_S1n() const { return not1(_MESU_BFETCH_S1.qp()); }

  // -> vram bus
  /*p27.NOFU*/ wire NOFU_BFETCH_S2n() const { return not1(_NYVA_BFETCH_S2.qp()); }

  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ Reg GAVE_SCY0 = REG_D0C0;
  /*p23.FYMO*/ Reg FYMO_SCY1 = REG_D0C0;
  /*p23.FEZU*/ Reg FEZU_SCY2 = REG_D0C0;
  /*p23.FUJO*/ Reg FUJO_SCY3 = REG_D0C0;
  /*p23.DEDE*/ Reg DEDE_SCY4 = REG_D0C0;
  /*p23.FOTY*/ Reg FOTY_SCY5 = REG_D0C0;
  /*p23.FOHA*/ Reg FOHA_SCY6 = REG_D0C0;
  /*p23.FUNY*/ Reg FUNY_SCY7 = REG_D0C0;

  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ Reg DATY_SCX0 = REG_D0C0;
  /*p23.DUZU*/ Reg DUZU_SCX1 = REG_D0C0;
  /*p23.CYXU*/ Reg CYXU_SCX2 = REG_D0C0;
  /*p23.GUBO*/ Reg GUBO_SCX3 = REG_D0C0;
  /*p23.BEMY*/ Reg BEMY_SCX4 = REG_D0C0;
  /*p23.CUZY*/ Reg CUZY_SCX5 = REG_D0C0;
  /*p23.CABU*/ Reg CABU_SCX6 = REG_D0C0;
  /*p23.BAKE*/ Reg BAKE_SCX7 = REG_D0C0;

//private:
  /*p27.LAXE*/ wire LAXE_BFETCH_S0n() const { return not1(_LAXU_BFETCH_S0.qp()); }

  // high for 10 phases during fetch, low for 6 phases
  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn() const { return nand3(_LAXU_BFETCH_S0.qp(), _NYVA_BFETCH_S2.qp(), _NYXU_TILE_FETCHER_RSTn); }

  Sig _NYDY_LATCH_TILE_DAp;
  Sig _MOFU_LATCH_TILE_DBn;

  Sig _XYMU_RENDERINGp;
  Sig _NYXU_TILE_FETCHER_RSTn;

  /*p27.LAXU*/ RegQPN _LAXU_BFETCH_S0 = REG_D0C0;
  /*p27.MESU*/ RegQPN _MESU_BFETCH_S1 = REG_D0C0;
  /*p27.NYVA*/ RegQPN _NYVA_BFETCH_S2 = REG_D0C0;

  /*p27.LOVY*/ RegQN _LOVY_FETCH_DONEp = REG_D0C0;

  /*p24.NYKA*/ RegQP _NYKA_FETCH_DONE_Ap = REG_D0C0;
  /*p24.PORY*/ RegQP _PORY_FETCH_DONE_Bp = REG_D0C0;
  /*p27.LYZU*/ RegQP _LYZU_BFETCH_S0_DELAY = REG_D0C0;

  /*p24.PYGO*/ RegQP _PYGO_FETCH_DONE_Cp = REG_D0C0;
  /*p24.POKY*/ Tri _POKY_PRELOAD_DONEp = TRI_D0NP;            // stops clkpipe until after first tile fetch
  /*p27.LONY*/ Tri _LONY_BG_READ_VRAM_LATCHp = TRI_D0NP;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics