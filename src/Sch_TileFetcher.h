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

  // not sure about this one
  // ROMO = not(POKY_04)
  // SUVU = nand(XYMU, ROMO, NYKA_17, PORY_17).... hmmmm

  // -> sprite fetcher, top.TEVO
  /*p27.TAVE*/ wire TAVE_PORCH_DONE_TRIGp() const {
    /*p27.ROMO*/ wire _ROMO_PORCH_DONEn = not(_POKY_PORCH_DONEp);
    /*p27.SUVU*/ wire _SUVU_PORCH_ENDn = nand(_XYMU_RENDERINGp, _ROMO_PORCH_DONEn, _NYKA_FETCH_DONE_Ap.q(), _PORY_FETCH_DONE_Bp.q());
    return  not(_SUVU_PORCH_ENDn);
  }

  // -> top.TYFA
  /*p24.POKY*/ wire POKY_PORCH_DONEp()        const { return _POKY_PORCH_DONEp; }

  // -> pix pipe
  /*p24.PORY*/ wire PORY_TILE_FETCH_DONE_Bp() const { return _PORY_FETCH_DONE_Bp.q(); }

  // -> sprite fetcher
  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp() const { return not(MOCE_BFETCH_DONEn()); }

  // -> vram bus
  /*p27.MYMA*/ wire MYMA_BGW_VRAM_RDn() const { return not(_LONY_BG_READ_VRAM_LATCHp); }

  // -> vram bus
  wire LENA_BGW_VRM_RDp() const {
    /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(_LONY_BG_READ_VRAM_LATCHp);
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

  void dump(Dumper& d) {
    d("----------TileFetcher---------\n");
    int scx = pack(BAKE_SCX7.q(), CABU_SCX6.q(), CUZY_SCX5.q(), BEMY_SCX4.q(), GUBO_SCX3.q(), CYXU_SCX2.q(), DUZU_SCX1.q(), DATY_SCX0.q());
    int scy = pack(FUNY_SCY7.q(), FOHA_SCY6.q(), FOTY_SCY5.q(), DEDE_SCY4.q(), FUJO_SCY3.q(), FEZU_SCY2.q(), FYMO_SCY1.q(), GAVE_SCY0.q());

    d("SCX  %03d\n", scx);
    d("SCY  %03d\n", scy);
    d("LAXU_BFETCH_S0           %c\n", _LAXU_BFETCH_S0          .c());
    d("MESU_BFETCH_S1           %c\n", _MESU_BFETCH_S1          .c());
    d("NYVA_BFETCH_S2           %c\n", _NYVA_BFETCH_S2          .c());
    d("LOVY_FETCH_DONEp         %c\n", _LOVY_FETCH_DONEp        .c());
    d("NYKA_FETCH_DONE_Ap       %c\n", _NYKA_FETCH_DONE_Ap      .c());
    d("PORY_FETCH_DONE_Bp       %c\n", _PORY_FETCH_DONE_Bp      .c());
    d("LYZU_BFETCH_S0_DELAY     %c\n", _LYZU_BFETCH_S0_DELAY    .c());
    d("PYGO_FETCH_DONE_Cp       %c\n", _PYGO_FETCH_DONE_Cp      .c());
    d("POKY_PORCH_DONEp         %c\n", _POKY_PORCH_DONEp        .c());
    d("LONY_BG_READ_VRAM_LATCHp %c\n", _LONY_BG_READ_VRAM_LATCHp.c());

    d("\n");
  }

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

private:
  /*p27.LAXE*/ wire LAXE_BFETCH_S0n() const { return not(_LAXU_BFETCH_S0.q()); }
  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn() const { return nand(_LAXU_BFETCH_S0.q(), _NYVA_BFETCH_S2.q(), _NYXU_TILE_FETCHER_RSTn); }

  Sig _NYDY_LATCH_TILE_DAp;
  Sig _MOFU_LATCH_TILE_DBn;

  Sig _XYMU_RENDERINGp;
  Sig _NYXU_TILE_FETCHER_RSTn;

  /*p27.LAXU*/ Reg _LAXU_BFETCH_S0 = REG_D0C0;
  /*p27.MESU*/ Reg _MESU_BFETCH_S1 = REG_D0C0;
  /*p27.NYVA*/ Reg _NYVA_BFETCH_S2 = REG_D0C0;

  /*p27.LOVY*/ Reg _LOVY_FETCH_DONEp = REG_D0C0;

  /*p24.NYKA*/ Reg _NYKA_FETCH_DONE_Ap = REG_D0C0;
  /*p24.PORY*/ Reg _PORY_FETCH_DONE_Bp = REG_D0C0;
  /*p27.LYZU*/ Reg _LYZU_BFETCH_S0_DELAY = REG_D0C0;

  /*p24.PYGO*/ Reg _PYGO_FETCH_DONE_Cp = REG_D0C0;
  /*p24.POKY*/ Tri _POKY_PORCH_DONEp = TRI_D0NP;            // stops clkpipe until after first tile fetch
  /*p27.LONY*/ Tri _LONY_BG_READ_VRAM_LATCHp = TRI_D0NP;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics