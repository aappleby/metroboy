#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct OamLatchA {

  void latch_bus(
    GateBoyCpuBus& cpu_bus,
    BusIO BUS_OAM_DAn[8],
    wire ACYL_SCANNINGp,
    wire XOCE_xBCxxFGx,
    wire XUJA_SPR_OAM_LATCHn,
    uint8_t oam_data_latch_a);

  void latch_to_cpu(
    GateBoyCpuBus& cpu_bus,
    BusOut BUS_OAM_An[8],
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp,
    wire XYMU_RENDERINGp);

  /*p29.YDYV*/ TpLatch YDYV_OAM_LATCH_DA0n; // xBxDxFxx // Proooobably all odd clocks?
  /*p29.YCEB*/ TpLatch YCEB_OAM_LATCH_DA1n; // xBxDxFxx
  /*p29.ZUCA*/ TpLatch ZUCA_OAM_LATCH_DA2n; // xBxDxFxx
  /*p29.WONE*/ TpLatch WONE_OAM_LATCH_DA3n; // xBxDxFxx
  /*p29.ZAXE*/ TpLatch ZAXE_OAM_LATCH_DA4n; // xBxDxFxx
  /*p29.XAFU*/ TpLatch XAFU_OAM_LATCH_DA5n; // xBxDxFxx
  /*p29.YSES*/ TpLatch YSES_OAM_LATCH_DA6n; // xBxDxFxx
  /*p29.ZECA*/ TpLatch ZECA_OAM_LATCH_DA7n; // xBxDxFxx
};

//-----------------------------------------------------------------------------

struct OamLatchB {
  void latch_bus(
    GateBoyCpuBus& cpu_bus,
    BusIO BUS_OAM_DBn[8],
    wire ACYL_SCANNINGp,
    wire XOCE_xBCxxFGx,
    wire XUJA_SPR_OAM_LATCHn,
    uint8_t oam_data_latch_b);

  void latch_to_cpu(
    GateBoyCpuBus& cpu_bus,
    BusOut BUS_OAM_An[8],
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp,
    wire XYMU_RENDERINGp);

  /*p31.XYKY*/ TpLatch XYKY_OAM_LATCH_DB0n; // xBxxxFxx // Proooobably all odd clocks?
  /*p31.YRUM*/ TpLatch YRUM_OAM_LATCH_DB1n; // xBxxxFxx
  /*p31.YSEX*/ TpLatch YSEX_OAM_LATCH_DB2n; // xBxxxFxx
  /*p31.YVEL*/ TpLatch YVEL_OAM_LATCH_DB3n; // xBxxxFxx
  /*p31.WYNO*/ TpLatch WYNO_OAM_LATCH_DB4n; // xBxxxFxx
  /*p31.CYRA*/ TpLatch CYRA_OAM_LATCH_DB5n; // xBxxxFxx
  /*p31.ZUVE*/ TpLatch ZUVE_OAM_LATCH_DB6n; // xBxxxFxx
  /*p31.ECED*/ TpLatch ECED_OAM_LATCH_DB7n; // xBxxxFxx
};

//-----------------------------------------------------------------------------
// oam byte 0, byte 2

struct OamTempA {

  void latch_to_temp(
    GateBoyClock& clk,
    GateBoyCpuBus& cpu_bus,
    wire ACYL_SCANNINGp,
    wire MATU_DMA_RUNNINGp,
    wire XUJY_OAM_CLKENp,
    const OamLatchA& oam_latch_a);

  /*p29.XUSO*/ DFF8n XUSO_OAM_DA0p; // AxxxExxx - sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ DFF8n XEGU_OAM_DA1p; // AxxxExxx - sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ DFF8n YJEX_OAM_DA2p; // AxxxExxx - sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ DFF8n XYJU_OAM_DA3p; // AxxxExxx - sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ DFF8n YBOG_OAM_DA4p; // AxxxExxx - sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ DFF8n WYSO_OAM_DA5p; // AxxxExxx - sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ DFF8n XOTE_OAM_DA6p; // AxxxExxx - sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ DFF8n YZAB_OAM_DA7p; // AxxxExxx - sprite y bit 7, sprite tile index bit 7
};

//-----------------------------------------------------------------------------
// oam byte 1, byte 3

struct OamTempB {

  void latch_to_temp(
    GateBoyClock& clk,
    GateBoyCpuBus& cpu_bus,
    wire ACYL_SCANNINGp,
    wire MATU_DMA_RUNNINGp,
    wire XUJY_OAM_CLKENp,
    const OamLatchB& oam_latch_b);

  /*p31.YLOR*/ DFF8n YLOR_OAM_DB0p; // AxxxExxx - sprite x bit 0,
  /*p31.ZYTY*/ DFF8n ZYTY_OAM_DB1p; // AxxxExxx - sprite x bit 1,
  /*p31.ZYVE*/ DFF8n ZYVE_OAM_DB2p; // AxxxExxx - sprite x bit 2,
  /*p31.ZEZY*/ DFF8n ZEZY_OAM_DB3p; // AxxxExxx - sprite x bit 3,
  /*p31.GOMO*/ DFF8n GOMO_OAM_DB4p; // AxxxExxx - sprite x bit 4, sprite palette
  /*p31.BAXO*/ DFF8n BAXO_OAM_DB5p; // AxxxExxx - sprite x bit 5, sprite x flip
  /*p31.YZOS*/ DFF8n YZOS_OAM_DB6p; // AxxxExxx - sprite x bit 6, sprite y flip
  /*p31.DEPO*/ DFF8n DEPO_OAM_DB7p; // AxxxExxx - sprite x bit 7, sprite priority
};

//-----------------------------------------------------------------------------

struct GateBoyOamBus {

  void ext_to_data_bus (const GateBoyDMA& dma, PinIO PIN_EXT_D[8]);
  void vram_to_data_bus(const GateBoyDMA& dma, BusIO BUS_VRAM_Dp[8]);

  void dma_to_addr_bus(GateBoyDMA& dma);
  void sprite_index_to_addr_bus(GateBoyDMA& dma, SpriteStore& sprite_store, wire XYMU_RENDERINGp);
  void scan_index_to_addr_bus(ScanCounter& scan_counter, wire ACYL_SCANNINGp);

  void cpu_to_data_bus(
    GateBoyClock& clk,
    GateBoyCpuBus& cpu_bus,
    wire XYMU_RENDERINGp,
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp);

  void update_pins(
    GateBoyClock& clk,
    GateBoyCpuBus& cpu_bus,

    wire XYMU_RENDERINGp,
    wire MATU_DMA_RUNNINGp,
    wire ACYL_SCANNINGp,

    wire XUJY_OAM_CLKENp,
    wire XUJA_SPR_OAM_LATCHn);

  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp;       // AxxxExxx
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;   // AxxxExxx

  BusOut BUS_OAM_An[8];  // ABCDEFGH
  BusIO  BUS_OAM_DAn[8];
  BusIO  BUS_OAM_DBn[8];
  PinOut PIN_OAM_CLKn;   // ABCDEFGH
  PinOut PIN_OAM_WRn_A;  // AxxxExxH
  PinOut PIN_OAM_WRn_B;  // AxxxExxH
  PinOut PIN_OAM_OEn;    // ABCDEFGH
};

//-----------------------------------------------------------------------------
