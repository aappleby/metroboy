#pragma once

#include "GateBoyLib/GateBoyTimer.h"
#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyDMA.h"
#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyPixPipe.h"
#include "GateBoyLib/GateBoyJoypad.h"
#include "GateBoyLib/GateBoySerial.h"
#include "GateBoyLib/GateBoyInterrupts.h"
#include "GateBoyLib/GateBoySpriteFetcher.h"
#include "GateBoyLib/GateBoyTileFetcher.h"
#include "GateBoyLib/GateBoySpriteScanner.h"
#include "GateBoyLib/GateBoyExtBus.h"
#include "GateBoyLib/GateBoyOamBus.h"
#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyZramBus.h"
#include "GateBoyLib/GateBoyReset.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoySPU.h"
#include "GateBoyLib/GateBoyPins.h"
#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------
// All the SOC registers, pins, buses. Everything in GateBoyState _must_
// derive from BitBase.

#pragma pack(push, 1)
struct GateBoyState {

  //----------------------------------------

  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  int64_t hash_regression();
  int64_t hash_all();

  GBResult peek(int addr) const;
  GBResult poke(int addr, uint8_t data_in);

  void commit();
  void check_state_old_and_driven_or_pulled();
  bool diff(const GateBoyState& gbb, uint8_t mask) const;

  static FieldInfo fields[];

  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   () const;
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   () const;
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   () const;
  /*_p08.LEVO*/ wire LEVO_ADDR_VRAMn   () const;
  /*_p25.TUJA*/ wire TUJA_CPU_VRAM_WRp () const;

  /*_p01.XODO*/ wire XODO_VID_RSTp() const;
  /*_p01.XAPO*/ wire XAPO_VID_RSTn() const;
  /*_p01.LYHA*/ wire LYHA_VID_RSTp() const;
  /*_p01.LYFE*/ wire LYFE_VID_RSTn() const;
  /*_p01.TOFU*/ wire TOFU_VID_RSTp() const;
  /*_p01.ROSY*/ wire ROSY_VID_RSTp() const;
  /*#p01.ATAR*/ wire ATAR_VID_RSTp() const;
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn() const;
  /*_p01.PYRY*/ wire PYRY_VID_RSTp() const;
  /*_p01.AMYG*/ wire AMYG_VID_RSTp() const;

  /*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp() const;
  /*#p25.SALE*/ wire SALE_CPU_VRAM_WRn() const;

  //----------------------------------------

  /*_SIG_VCC*/ SigIn SIG_VCC;
  /*_SIG_GND*/ SigIn SIG_GND;
  
  RegJoy  reg_joy;  // 0xFF00
  //RegSB   reg_sb;   // 0xFF01
  //RegSC   reg_sc;   // 0xFF02
  RegDIV  reg_div;  // 0xFF04
  RegTIMA reg_tima; // 0xFF05
  RegTMA  reg_tma;  // 0xFF06
  RegTAC  reg_tac;  // 0xFF07
  RegIF   reg_if;   // 0xFF0F

  RegLCDC reg_lcdc; // 0xFF40
  RegStat reg_stat; // 0xFF41
  RegSCY  reg_scy;  // 0xFF42
  RegSCX  reg_scx;  // 0xFF43
  RegLY   reg_ly;   // 0xFF44
  RegLYC  reg_lyc;  // 0xFF45
  RegDmaHi  reg_dma;  // 0xFF46
  RegBGP  reg_bgp;  // 0xFF47
  RegOBP0 reg_obp0; // 0xFF48
  RegOBP1 reg_obp1; // 0xFF49
  RegWY   reg_wy;   // 0xFF4A
  RegWX   reg_wx;   // 0xFF4B
  RegLX   reg_lx;   // Not mapped
  RegIE   reg_ie;   // 0xFFFF

  GateBoyReset sys_rst;
  GateBoyClock sys_clk;

  /*#p21.VOGA*/ DFF17    VOGA_HBLANKp_evn;
  /*#p21.XYMU*/ NorLatch XYMU_RENDERING_LATCHn;
  /*#p04.MATU*/ DFF17    MATU_DMA_RUNNINGp_odd;
  /*#p28.ACYL*/ Gate     ACYL_SCANNINGp_odd;
  /*#p21.WODU*/ Gate     WODU_HBLANK_GATEp_odd;
  /*_p28.ATEJ*/ Gate     ATEJ_LINE_RST_TRIGp_odd;
  /*_p29.FEPO*/ Gate     FEPO_STORE_MATCHp_odd;

  GateBoyCpuSignals cpu_signals;
  GateBoyCpuABus    cpu_abus;
  GateBoyCpuDBus    cpu_dbus;

  InterruptControl int_ctrl;
  InterruptLatch   int_latch;

  struct { VramABusLo lo; VramABusHi hi; } vram_abus;
  VramDBus       vram_dbus;
                
  OamControl     oam_ctrl;
  OamABus        oam_abus;
  OamDBusA       oam_dbus_a;
  OamDBusB       oam_dbus_b;
  OamLatchA      oam_latch_a;
  OamLatchB      oam_latch_b;
  OamTempA       oam_temp_a;
  OamTempB       oam_temp_b;
                 
  ExtDataLatch   ext_data_latch;
  ExtAddrLatch   ext_addr_latch;

  GateBoyMBC     ext_mbc;

  GateBoyZram    zram_bus;


  DmaControl dma_ctrl;
  RegDmaLo   dma_lo;
  

  CpuInt  cpu_int;
  CpuAck  cpu_ack;

  JoyInt   joy_int;
  JoyLatch joy_latch;

  //----------

  SpriteScanner    sprite_scanner;
  ScanCounter      scan_counter;
  SpriteCounter    sprite_counter;
  SpriteIndex      sprite_index;
  SpriteMatchFlags sprite_match_flags_odd;
  SpriteResetFlags sprite_reset_flags_odd;
  SpriteStoreFlags sprite_store_flags_evn;

  SpriteIBus sprite_ibus;
  SpriteLBus sprite_lbus;

  StoreI0 store_i0;
  StoreI1 store_i1;
  StoreI2 store_i2;
  StoreI3 store_i3;
  StoreI4 store_i4;
  StoreI5 store_i5;
  StoreI6 store_i6;
  StoreI7 store_i7;
  StoreI8 store_i8;
  StoreI9 store_i9;

  StoreL0 store_l0;
  StoreL1 store_l1;
  StoreL2 store_l2;
  StoreL3 store_l3;
  StoreL4 store_l4;
  StoreL5 store_l5;
  StoreL6 store_l6;
  StoreL7 store_l7;
  StoreL8 store_l8;
  StoreL9 store_l9;

  StoreX0 store_x0;
  StoreX1 store_x1;
  StoreX2 store_x2;
  StoreX3 store_x3;
  StoreX4 store_x4;
  StoreX5 store_x5;
  StoreX6 store_x6;
  StoreX7 store_x7;
  StoreX8 store_x8;
  StoreX9 store_x9;

  //----------


  SpriteFetchCounter sfetch_counter_evn;
  SpriteFetchControl sfetch_control;

  //----------

  TileFetchCounter tfetch_counter;
  TileFetchControl tfetch_control;
  TileTempA tile_temp_a;
  TileTempB tile_temp_b;

  //----------

  WinControl win_ctrl;

  struct {
    WinMapX    map;
  } win_x;

  struct {
    WinTileY   tile;
    WinMapY    map;
  } win_y;

  FineCount  fine_count_odd;
  FineScroll fine_scroll;

  //----------

  SpritePix flipped_sprite;
  SpritePixA sprite_pix_a;
  SpritePixB sprite_pix_b;

  PixCount pix_count;   // odd
  MaskPipe mask_pipe;
  BgwPipeA bgw_pipe_a;
  BgwPipeB bgw_pipe_b;
  SprPipeA spr_pipe_a;
  SprPipeB spr_pipe_b;
  PalPipe  pal_pipe;

  LCDControl lcd;

  //NR10 reg_NR10;
  //NR11 reg_NR11;
  //NR12 reg_NR12;
  //NR14 reg_NR14;

  //NR21 reg_NR21;
  //NR22 reg_NR22;
  //NR24 reg_NR24;

  //NR30 reg_NR30;
  //NR31 reg_NR31;
  //NR32 reg_NR32;
  //NR34 reg_NR34;

  //NR41 reg_NR41;
  //NR42 reg_NR42;
  //NR43 reg_NR43;
  //NR44 reg_NR44;

  //NR50 reg_NR50;
  //NR51 reg_NR51;
  //NR52 reg_NR52;
};
#pragma pack(pop)
