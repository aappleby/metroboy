#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct GateBoyVramBus {

  void reset_app() {
    BUS_VRAM_An[ 0].reset(REG_D1C0);
    BUS_VRAM_An[ 1].reset(REG_D1C0);
    BUS_VRAM_An[ 2].reset(REG_D1C0);
    BUS_VRAM_An[ 3].reset(REG_D1C0);
    BUS_VRAM_An[ 4].reset(REG_D0C0);
    BUS_VRAM_An[ 5].reset(REG_D1C0);
    BUS_VRAM_An[ 6].reset(REG_D0C0);
    BUS_VRAM_An[ 7].reset(REG_D1C0);
    BUS_VRAM_An[ 8].reset(REG_D1C0);
    BUS_VRAM_An[ 9].reset(REG_D1C0);
    BUS_VRAM_An[10].reset(REG_D1C0);
    BUS_VRAM_An[11].reset(REG_D1C0);
    BUS_VRAM_An[12].reset(REG_D1C0);

    PIN_VRAM_Ap[ 0].reset(REG_D0C0);
    PIN_VRAM_Ap[ 1].reset(REG_D0C0);
    PIN_VRAM_Ap[ 2].reset(REG_D0C0);
    PIN_VRAM_Ap[ 3].reset(REG_D0C0);
    PIN_VRAM_Ap[ 4].reset(REG_D1C0);
    PIN_VRAM_Ap[ 5].reset(REG_D0C0);
    PIN_VRAM_Ap[ 6].reset(REG_D1C0);
    PIN_VRAM_Ap[ 7].reset(REG_D0C0);
    PIN_VRAM_Ap[ 8].reset(REG_D0C0);
    PIN_VRAM_Ap[ 9].reset(REG_D0C0);
    PIN_VRAM_Ap[10].reset(REG_D0C0);
    PIN_VRAM_Ap[11].reset(REG_D0C0);
    PIN_VRAM_Ap[12].reset(REG_D0C0);
  }

  void cpu_addr_to_vram_addr(Signal BUS_CPU_A[16], wire XYMU_RENDERINGp, wire LUFA_DMA_VRAMp);
  void dma_addr_to_vram_addr(const GateBoyDMA& dma);
  void scroll_to_addr(BGScrollX scroll_x, BGScrollY scroll_y, wire POTU_BGW_MAP_READp, wire AXAD_WIN_MODEn, wire XAFO_LCDC_BGMAPp);
  void win_to_addr(const WinMapX& win_map_x, const WinLineY& win_line_y, wire POTU_BGW_MAP_READp, wire PORE_WIN_MODEp, wire WOKY_LCDC_WINMAPp);
  void tile_to_addr(const BGScrollY scroll_y, const WinLineY win_line_y, const TileTempB tile_temp_b, wire NETA_BGW_TILE_READp, wire XUHA_FETCH_HILOp, wire WEXU_LCDC_BGTILEp, wire PORE_WIN_MODEp, wire AXAD_WIN_MODEn);
  void sprite_to_addr(BusIO SPR_TRI_L[4], const OamTempA& oam_temp_a, const OamTempB& oam_temp_b, wire XUQU_SPRITE_AB, wire SAKY_SFETCHn, wire XYMU_RENDERINGp, wire XYMO_LCDC_SPSIZEp);
  void addr_to_pins();
  void cpu_data_to_bus_data(const GateBoyCpuBus& cpu_bus, wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn);
  void bus_data_to_pins(wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn);

  void set_pin_cs(wire TUTO_VRAM_DBGp, wire SERE_CPU_VRAM_RDp, wire LUFA_DMA_VRAMp, wire LENA_BFETCHINGp, wire TEXY_SFETCHINGp);
  void set_pin_wr(wire TUTO_VRAM_DBGp, wire SERE_CPU_VRAM_RDp, wire TUJA_CPU_VRAM_WRp);
  void set_pin_oe(wire TUTO_VRAM_DBGp, wire SALE_CPU_VRAM_WRn, wire LUFA_DMA_VRAMp, wire XYMU_RENDERINGp, wire LONY_BFETCHINGp, wire SOHO_SPR_VRAM_RDp);

  void tock_vram(uint8_t* vid_ram);

  void pins_to_data_bus(wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn);
  void data_bus_to_cpu_bus(GateBoyCpuBus& cpu_bus, wire SERE_CPU_VRAM_RDp);

  void dump(Dumper& d) {
    d.dump_slice2n("BUS_VRAM_An ", BUS_VRAM_An, 13);
    d.dump_slice2p("BUS_VRAM_Dp ", BUS_VRAM_Dp, 8);
    d.dump_bitp   ("PIN_VRAM_CSn", PIN_VRAM_CSn.state);
    d.dump_bitp   ("PIN_VRAM_OEn", PIN_VRAM_OEn.state);
    d.dump_bitp   ("PIN_VRAM_WRn", PIN_VRAM_WRn.state);
    d.dump_slice2p("PIN_VRAM_Ap ", PIN_VRAM_Ap, 13);
    d.dump_slice2p("PIN_VRAM_Dp ", PIN_VRAM_Dp, 8);
  }

  BusOut BUS_VRAM_An[13];
  BusIO  BUS_VRAM_Dp[8];
  PinOut PIN_VRAM_CSn; // PIN_43
  PinOut PIN_VRAM_OEn; // PIN_45
  PinOut PIN_VRAM_WRn; // PIN_49
  PinOut PIN_VRAM_Ap[13];
  PinIO  PIN_VRAM_Dp[8];
};

//-----------------------------------------------------------------------------
