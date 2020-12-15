#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyDMA;
struct WinMapX;
struct WinLineY;
struct TileTempB;
struct OamTempA;
struct OamTempB;

//-----------------------------------------------------------------------------

struct BGScrollX {
  wire ATAD_TILE_X0S;
  wire BEHU_TILE_X1S;
  wire APYH_TILE_X2S;
  wire BABE_MAP_X0S;
  wire ABOD_MAP_X1S;
  wire BEWY_MAP_X2S;
  wire BYCA_MAP_X3S;
  wire ACUL_MAP_X4S;
};

struct BGScrollY {
  wire FAFO_TILE_Y0S;
  wire EMUX_TILE_Y1S;
  wire ECAB_TILE_Y2S;
  wire ETAM_MAP_Y0S;
  wire DOTO_MAP_Y1S;
  wire DABA_MAP_Y2S;
  wire EFYK_MAP_Y3S;
  wire EJOK_MAP_Y4S;
};

struct SpriteFlipX {
  wire PUTE_FLIP0p;
  wire PELO_FLIP1p;
  wire PONO_FLIP2p;
  wire POBE_FLIP3p;
  wire PACY_FLIP4p;
  wire PUGU_FLIP5p;
  wire PAWE_FLIP6p;
  wire PULY_FLIP7p;
};

//-----------------------------------------------------------------------------

struct SpriteTempA {
  void store_sprite(SpriteFlipX sprite, wire XADO_STORE_SPRITE_An);

  /*p33.REWO*/ DFF8n REWO_SPRITE_DA0n; // xBxDxFxH
  /*p33.PEBA*/ DFF8n PEBA_SPRITE_DA1n; // xBxDxFxH
  /*p33.MOFO*/ DFF8n MOFO_SPRITE_DA2n; // xBxDxFxH
  /*p33.PUDU*/ DFF8n PUDU_SPRITE_DA3n; // xBxDxFxH
  /*p33.SAJA*/ DFF8n SAJA_SPRITE_DA4n; // xBxDxFxH
  /*p33.SUNY*/ DFF8n SUNY_SPRITE_DA5n; // xBxDxFxH
  /*p33.SEMO*/ DFF8n SEMO_SPRITE_DA6n; // xBxDxFxH
  /*p33.SEGA*/ DFF8n SEGA_SPRITE_DA7n; // xBxDxFxH
};

struct SpriteTempB {
  void store_sprite(SpriteFlipX sprite, wire PUCO_STORE_SPRITE_Bn);

  /*p33.PEFO*/ DFF8n PEFO_SPRITE_DB0n; // xBxDxFxH
  /*p33.ROKA*/ DFF8n ROKA_SPRITE_DB1n; // xBxDxFxH
  /*p33.MYTU*/ DFF8n MYTU_SPRITE_DB2n; // xBxDxFxH
  /*p33.RAMU*/ DFF8n RAMU_SPRITE_DB3n; // xBxDxFxH
  /*p33.SELE*/ DFF8n SELE_SPRITE_DB4n; // xBxDxFxH
  /*p33.SUTO*/ DFF8n SUTO_SPRITE_DB5n; // xBxDxFxH
  /*p33.RAMA*/ DFF8n RAMA_SPRITE_DB6n; // xBxDxFxH
  /*p33.RYDU*/ DFF8n RYDU_SPRITE_DB7n; // xBxDxFxH
};

struct GateBoyVramBus {

  void cpu_addr_to_vram_addr(Signal BUS_CPU_A[16], wire XYMU_RENDERINGp, wire LUFA_DMA_VRAMp);
  void dma_addr_to_vram_addr(const GateBoyDMA& dma);
  void scroll_to_addr(BGScrollX scroll_x, BGScrollY scroll_y, wire POTU_BGW_MAP_READp, wire AXAD_WIN_MODEn, wire XAFO_LCDC_BGMAPp);
  void win_to_addr(const WinMapX& win_map_x, const WinLineY& win_line_y, wire POTU_BGW_MAP_READp, wire PORE_WIN_MODEp, wire WOKY_LCDC_WINMAPp);
  void tile_to_addr(const BGScrollY scroll_y, const WinLineY win_line_y, const TileTempB tile_temp_b, wire NETA_BGW_TILE_READp, wire XUHA_FETCH_HILOp, wire WEXU_LCDC_BGTILEp, wire PORE_WIN_MODEp, wire AXAD_WIN_MODEn);
  void sprite_to_addr(const SpriteStore& sprite_store, const OamTempA& oam_temp_a, const OamTempB& oam_temp_b, wire XUQU_SPRITE_AB, wire SAKY_SFETCHn, wire XYMU_RENDERINGp, wire XYMO_LCDC_SPSIZEp);
  void addr_to_pins();
  void cpu_data_to_bus_data(const GateBoyCpuBus& cpu_bus, wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn);
  void bus_data_to_pins(wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn);

  BusOut BUS_VRAM_An[13];
  BusIO  BUS_VRAM_Dp[8];
  PinOut PIN_VRAM_CSn; // PIN_43
  PinOut PIN_VRAM_OEn; // PIN_45
  PinOut PIN_VRAM_WRn; // PIN_49
  PinOut PIN_VRAM_Ap[13];
  PinIO  PIN_VRAM_Dp[8];
};

//-----------------------------------------------------------------------------
