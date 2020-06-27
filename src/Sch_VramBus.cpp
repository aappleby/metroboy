#include "Sch_VramBus.h"

#include "TestGB.h"

using namespace Schematics;

void VramBus::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto win_sig = gb.win_reg.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);


  auto& vram_pins = gb.vram_pins;

  auto& cpu_bus = gb.cpu_bus;

  //---------------------------------------------------------------------------

  // How does DMA/PPU lock come into play here? Does it?
  // PPU locks via RENDERING_LATCH, dma by...? well it doesn't have to lock since it's just reading.
  // so really if the address is muxed right, we only need to block writes while vram's locked.
  // oh there's also some locking down in the pin driver...

  //----------

  /*p25.TAVY*/ wire TAVY_MOE_Cn = not(vram_pins.PIN_MOEn_C);
  /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand(adr_sig.SOSE_8000_9FFFp, cpu_bus.PIN_CPU_RAW_WR); // Schematic wrong, second input is CPU_RAW_WR
  /*p25.SALE*/ wire SALE_DBG_VRAM_RDb = mux2_p(TAVY_MOE_Cn, TEGU_CPU_VRAM_WRn, dbg_sig.TUTO_DBG_VRAM);
  /*p25.RUVY*/ wire RUVY_VRAM_WR = not(SALE_DBG_VRAM_RDb);
  /*p25.SAZO*/ wire SAZO_VRAM_RD = and (RUVY_VRAM_WR, ppu_sig.SERE_VRAM_RD);
  /*p25.RYJE*/ wire RYJE_VRAM_RDn = not(SAZO_VRAM_RD);
  /*p25.REVO*/ wire REVO_VRAM_RDp = not(RYJE_VRAM_RDn);

  /*p25.RELA*/ wire RELA_VRAM_DRIVEp = or (REVO_VRAM_RDp, SAZO_VRAM_RD);
  /*p25.RENA*/ wire RENA_VRAM_DRIVEn = not(RELA_VRAM_DRIVEp);


  /*p25.RODY*/ TS_MD0.set_tribuf(!RENA_VRAM_DRIVEn, vram_pins.PIN_MD0_C);
  /*p25.REBA*/ TS_MD1.set_tribuf(!RENA_VRAM_DRIVEn, vram_pins.PIN_MD1_C);
  /*p25.RYDO*/ TS_MD2.set_tribuf(!RENA_VRAM_DRIVEn, vram_pins.PIN_MD2_C);
  /*p25.REMO*/ TS_MD3.set_tribuf(!RENA_VRAM_DRIVEn, vram_pins.PIN_MD3_C);
  /*p25.ROCE*/ TS_MD4.set_tribuf(!RENA_VRAM_DRIVEn, vram_pins.PIN_MD4_C);
  /*p25.ROPU*/ TS_MD5.set_tribuf(!RENA_VRAM_DRIVEn, vram_pins.PIN_MD5_C);
  /*p25.RETA*/ TS_MD6.set_tribuf(!RENA_VRAM_DRIVEn, vram_pins.PIN_MD6_C);
  /*p25.RAKU*/ TS_MD7.set_tribuf(!RENA_VRAM_DRIVEn, vram_pins.PIN_MD7_C);
}