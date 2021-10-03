#pragma once

#include "GateBoyLib/GateBoyTimer.h"
#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoyDMA.h"
#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyPixPipe.h"
#include "GateBoyLib/GateBoyJoypad.h"
#include "GateBoyLib/GateBoyReset.h"
#include "GateBoyLib/GateBoySerial.h"
#include "GateBoyLib/GateBoyInterrupts.h"
#include "GateBoyLib/GateBoySpriteFetcher.h"
#include "GateBoyLib/GateBoyTileFetcher.h"
#include "GateBoyLib/GateBoySpriteScanner.h"
#include "GateBoyLib/GateBoyExtBus.h"
#include "GateBoyLib/GateBoyOamBus.h"
#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyZramBus.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoySPU.h"
#include "GateBoyLib/GateBoyPins.h"

struct GateBoyState;

struct LogicBoyState {
  void reset_to_bootrom();
  void reset_to_cart();

  Result<uint8_t, Error> peek(int addr) const;
  Result<uint8_t, Error> poke(int addr, uint8_t data_in);

  void to_gb_state(GateBoyState& dst, int64_t phase_total) const;
  void from_gb_state(const GateBoyState& src, int64_t phase_total);

  uint8_t  reg_joy;  // 0xFF00
  uint8_t  reg_sb; // 0xFF01
  uint8_t  reg_sc; // 0xFF02
  uint16_t reg_div;  // 0xFF04
  uint8_t  reg_tima; // 0xFF05
  uint8_t  reg_tma;  // 0xFF06
  uint8_t  reg_tac;  // 0xFF07
  uint8_t  reg_if;   // 0xFF0F

  uint8_t  reg_lcdc; // 0xFF40
  uint8_t  reg_stat; // 0xFF41
  uint8_t  reg_scy;  // 0xFF42
  uint8_t  reg_scx;  // 0xFF43
  uint8_t  reg_ly;   // 0xFF44
  uint8_t  reg_lyc;  // 0xFF45
  uint8_t  reg_dma;  // 0xFF46
  uint8_t  reg_bgp;  // 0xFF47
  uint8_t  reg_obp0; // 0xFF48
  uint8_t  reg_obp1; // 0xFF49
  uint8_t  reg_wy;   // 0xFF4A
  uint8_t  reg_wx;   // 0xFF4B
  uint8_t  reg_lx;   // Not mapped
  uint8_t  reg_ie;   // 0xFFFF

  GateBoyReset sys_rst;
  GateBoyClock sys_clk;

  /*#p21.VOGA*/ uint8_t VOGA_HBLANKp;
  /*#p21.XYMU*/ uint8_t XYMU_RENDERINGn;
  /*#p04.MATU*/ uint8_t MATU_DMA_RUNNINGp;
  /*#p28.ACYL*/ uint8_t ACYL_SCANNINGp_odd;
  /*#p21.WODU*/ uint8_t WODU_HBLANKp_odd;
  /*_p28.ATEJ*/ uint8_t ATEJ_LINE_RSTp_odd;
  /*_p29.FEPO*/ uint8_t FEPO_STORE_MATCHp_odd;

  GateBoyCpuSignals cpu_signals;
  uint16_t cpu_abus;
  uint8_t cpu_dbus;

  InterruptControl int_ctrl;
  uint8_t int_latch;

  uint16_t vram_abus;
  uint8_t vram_dbus;
                
  OamControl oam_ctrl;
  uint8_t  oam_abus;
  uint8_t  oam_dbus_a;
  uint8_t  oam_dbus_b;
  uint8_t  oam_latch_a;
  uint8_t  oam_latch_b;
  uint8_t  oam_temp_a;
  uint8_t  oam_temp_b;
                 
  uint8_t ext_data_latch;
  uint16_t ext_addr_latch;

  GateBoyMBC     ext_mbc;

  GateBoyZram    zram_bus;


  DmaControl dma_ctrl;
  uint8_t dma_lo;
  

  uint8_t cpu_int;
  uint8_t cpu_ack;

  JoyInt   joy_int;
  uint8_t joy_latch;

  //----------

  SpriteScanner    sprite_scanner;
  uint8_t scan_counter;
  uint8_t sprite_counter;
  uint8_t sprite_index;
  uint16_t sprite_match_flags;
  uint16_t sprite_reset_flags;
  uint16_t sprite_store_flags;

  uint8_t sprite_ibus;
  uint8_t sprite_lbus;

  uint8_t store_i0;
  uint8_t store_i1;
  uint8_t store_i2;
  uint8_t store_i3;
  uint8_t store_i4;
  uint8_t store_i5;
  uint8_t store_i6;
  uint8_t store_i7;
  uint8_t store_i8;
  uint8_t store_i9;

  uint8_t store_l0;
  uint8_t store_l1;
  uint8_t store_l2;
  uint8_t store_l3;
  uint8_t store_l4;
  uint8_t store_l5;
  uint8_t store_l6;
  uint8_t store_l7;
  uint8_t store_l8;
  uint8_t store_l9;

  uint8_t store_x0;
  uint8_t store_x1;
  uint8_t store_x2;
  uint8_t store_x3;
  uint8_t store_x4;
  uint8_t store_x5;
  uint8_t store_x6;
  uint8_t store_x7;
  uint8_t store_x8;
  uint8_t store_x9;

  //----------


  uint8_t sfetch_counter_evn;
  SpriteFetchControl sfetch_control;
  //----------

  uint8_t tfetch_counter_odd;
  TileFetchControl tfetch_control;
  uint8_t tile_temp_a;
  uint8_t tile_temp_b;

  //----------

  WinControl win_ctrl;

  struct {
    uint8_t map;
  } win_x;

  struct {
    uint8_t tile;
    uint8_t map;
  } win_y;

  uint8_t fine_count_odd;
  FineScroll fine_scroll;

  //----------

  uint8_t flipped_sprite;
  uint8_t sprite_pix_a;
  uint8_t sprite_pix_b;

  uint8_t pix_count_odd;
  uint8_t mask_pipe;
  uint8_t bgw_pipe_a;
  uint8_t bgw_pipe_b;
  uint8_t spr_pipe_a;
  uint8_t spr_pipe_b;
  uint8_t pal_pipe;

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

  // new stuff that doesn't translate to GBState

  int64_t phase_lcd;

  int phase_tfetch;
  int phase_sfetch;
};
