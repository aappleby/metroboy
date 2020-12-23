#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct GateBoyVramBus {

  void reset_to_cart() {
    BUS_VRAM_An[ 0].reset(1);
    BUS_VRAM_An[ 1].reset(1);
    BUS_VRAM_An[ 2].reset(1);
    BUS_VRAM_An[ 3].reset(1);
    BUS_VRAM_An[ 4].reset(0);
    BUS_VRAM_An[ 5].reset(1);
    BUS_VRAM_An[ 6].reset(0);
    BUS_VRAM_An[ 7].reset(1);
    BUS_VRAM_An[ 8].reset(1);
    BUS_VRAM_An[ 9].reset(1);
    BUS_VRAM_An[10].reset(1);
    BUS_VRAM_An[11].reset(1);
    BUS_VRAM_An[12].reset(1);

    PIN34_VRAM_ADDR[ 0].reset(1);
    PIN34_VRAM_ADDR[ 1].reset(1);
    PIN34_VRAM_ADDR[ 2].reset(1);
    PIN34_VRAM_ADDR[ 3].reset(1);
    PIN34_VRAM_ADDR[ 4].reset(0);
    PIN34_VRAM_ADDR[ 5].reset(1);
    PIN34_VRAM_ADDR[ 6].reset(0);
    PIN34_VRAM_ADDR[ 7].reset(1);
    PIN34_VRAM_ADDR[ 8].reset(1);
    PIN34_VRAM_ADDR[ 9].reset(1);
    PIN34_VRAM_ADDR[10].reset(1);
    PIN34_VRAM_ADDR[11].reset(1);
    PIN34_VRAM_ADDR[12].reset(1);
  }

  void reset_buses() {
    BUS_VRAM_An[ 0].reset_for_pass();
    BUS_VRAM_An[ 1].reset_for_pass();
    BUS_VRAM_An[ 2].reset_for_pass();
    BUS_VRAM_An[ 3].reset_for_pass();
    BUS_VRAM_An[ 4].reset_for_pass();
    BUS_VRAM_An[ 5].reset_for_pass();
    BUS_VRAM_An[ 6].reset_for_pass();
    BUS_VRAM_An[ 7].reset_for_pass();
    BUS_VRAM_An[ 8].reset_for_pass();
    BUS_VRAM_An[ 9].reset_for_pass();
    BUS_VRAM_An[10].reset_for_pass();
    BUS_VRAM_An[11].reset_for_pass();
    BUS_VRAM_An[12].reset_for_pass();

    BUS_VRAM_Dp[0].reset_for_pass();
    BUS_VRAM_Dp[1].reset_for_pass();
    BUS_VRAM_Dp[2].reset_for_pass();
    BUS_VRAM_Dp[3].reset_for_pass();
    BUS_VRAM_Dp[4].reset_for_pass();
    BUS_VRAM_Dp[5].reset_for_pass();
    BUS_VRAM_Dp[6].reset_for_pass();
    BUS_VRAM_Dp[7].reset_for_pass();

    PIN43_VRAM_CSn.reset_for_pass();
    PIN45_VRAM_OEn.reset_for_pass();
    PIN49_VRAM_WRn.reset_for_pass();

    PIN34_VRAM_ADDR[ 0].reset_for_pass();
    PIN34_VRAM_ADDR[ 1].reset_for_pass();
    PIN34_VRAM_ADDR[ 2].reset_for_pass();
    PIN34_VRAM_ADDR[ 3].reset_for_pass();
    PIN34_VRAM_ADDR[ 4].reset_for_pass();
    PIN34_VRAM_ADDR[ 5].reset_for_pass();
    PIN34_VRAM_ADDR[ 6].reset_for_pass();
    PIN34_VRAM_ADDR[ 7].reset_for_pass();
    PIN34_VRAM_ADDR[ 8].reset_for_pass();
    PIN34_VRAM_ADDR[ 9].reset_for_pass();
    PIN34_VRAM_ADDR[10].reset_for_pass();
    PIN34_VRAM_ADDR[11].reset_for_pass();
    PIN34_VRAM_ADDR[12].reset_for_pass();

    PIN25_VRAM_DATA[0].reset_for_pass();
    PIN25_VRAM_DATA[1].reset_for_pass();
    PIN25_VRAM_DATA[2].reset_for_pass();
    PIN25_VRAM_DATA[3].reset_for_pass();
    PIN25_VRAM_DATA[4].reset_for_pass();
    PIN25_VRAM_DATA[5].reset_for_pass();
    PIN25_VRAM_DATA[6].reset_for_pass();
    PIN25_VRAM_DATA[7].reset_for_pass();
  }

  void cpu_addr_to_vram_addr(Signal BUS_CPU_A[16], wire2 XYMU_RENDERINGp, wire2 LUFA_DMA_VRAMp);
  void dma_addr_to_vram_addr(const GateBoyDMA& dma);
  void scroll_to_addr(BGScrollX scroll_x, BGScrollY scroll_y, wire2 POTU_BGW_MAP_READp, wire2 AXAD_WIN_MODEn, wire2 XAFO_LCDC_BGMAPp);
  void win_to_addr(const WinMapX& win_map_x, const WinLineY& win_line_y, wire2 POTU_BGW_MAP_READp, wire2 PORE_WIN_MODEp, wire2 WOKY_LCDC_WINMAPp);
  void tile_to_addr(const BGScrollY scroll_y, const WinLineY win_line_y, const TileTempB tile_temp_b, wire2 NETA_BGW_TILE_READp, wire2 XUHA_FETCH_HILOp, wire2 WEXU_LCDC_BGTILEp, wire2 PORE_WIN_MODEp, wire2 AXAD_WIN_MODEn);
  void sprite_to_addr(Bus SPR_TRI_L[4], const OamTempA& oam_temp_a, const OamTempB& oam_temp_b, wire2 XUQU_SPRITE_AB, wire2 SAKY_SFETCHn, wire2 XYMU_RENDERINGp, wire2 XYMO_LCDC_SPSIZEp);
  void addr_to_pins();
  void cpu_data_to_bus_data(const GateBoyCpuBus& cpu_bus, wire2 SERE_CPU_VRAM_RDp, wire2 SALE_CPU_VRAM_WRn);
  void bus_data_to_pins(wire2 SERE_CPU_VRAM_RDp, wire2 SALE_CPU_VRAM_WRn);

  void set_pin_cs(wire2 TUTO_VRAM_DBGp, wire2 SERE_CPU_VRAM_RDp, wire2 LUFA_DMA_VRAMp, wire2 LENA_BFETCHINGp, wire2 TEXY_SFETCHINGp);
  void set_pin_wr(wire2 TUTO_VRAM_DBGp, wire2 SERE_CPU_VRAM_RDp, wire2 TUJA_CPU_VRAM_WRp);
  void set_pin_oe(wire2 TUTO_VRAM_DBGp, wire2 SALE_CPU_VRAM_WRn, wire2 LUFA_DMA_VRAMp, wire2 XYMU_RENDERINGp, wire2 LONY_BFETCHINGp, wire2 SOHO_SPR_VRAM_RDp);

  void read_vram(const uint8_t* vid_ram);
  void write_vram(uint8_t* vid_ram);

  void pins_to_data_bus(wire2 SERE_CPU_VRAM_RDp, wire2 SALE_CPU_VRAM_WRn);
  void data_bus_to_cpu_bus(GateBoyCpuBus& cpu_bus, wire2 SERE_CPU_VRAM_RDp);

  void dump(Dumper& d) {
    d.dump_bitp   ("PIN43_VRAM_CSn  : ", PIN43_VRAM_CSn.state);
    d.dump_bitp   ("PIN45_VRAM_OEn  : ", PIN45_VRAM_OEn.state);
    d.dump_bitp   ("PIN49_VRAM_WRn  : ", PIN49_VRAM_WRn.state);
    d.dump_slice2p("PIN34_VRAM_ADDR : ", PIN34_VRAM_ADDR, 13);
    d.dump_slice2p("PIN25_VRAM_DATA : ", PIN25_VRAM_DATA, 8);
    d.dump_slice2n("BUS_VRAM_An     : ", BUS_VRAM_An, 13);
    d.dump_slice2p("BUS_VRAM_Dp     : ", BUS_VRAM_Dp, 8);
  }

  Bus BUS_VRAM_An[13];
  Bus BUS_VRAM_Dp[8];
  PinOut PIN43_VRAM_CSn;
  PinOut PIN45_VRAM_OEn;
  PinOut PIN49_VRAM_WRn;
  PinOut PIN34_VRAM_ADDR[13]; // Pins 34 - 48
  PinIO  PIN25_VRAM_DATA[8];  // Pins 25 - 33
};

//-----------------------------------------------------------------------------
