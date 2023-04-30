/// plait_noparse
#pragma once

#include "MetroLib/src/CoreLib/Result.h"

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
#include "GateBoyLib/GateBoyWaveBus.h"
#include "GateBoyLib/GateBoyPins.h"

struct GateBoyState;

struct LogicBoyState {
  void reset();

  Result<uint8_t, Error> peek(int addr) const;
  Result<uint8_t, Error> poke(int addr, uint8_t data_in);

  void to_gb_state(GateBoyState& dst) const;
  void from_gb_state(const GateBoyState& src);

  uint8_t  reg_joy = 0;  // 0xFF00
  uint8_t  reg_sb = 0; // 0xFF01
  uint8_t  reg_sc = 0; // 0xFF02
  uint16_t reg_div = 0;  // 0xFF04
  uint8_t  reg_tima = 0; // 0xFF05
  uint8_t  reg_tma = 0;  // 0xFF06
  uint8_t  reg_tac = 0;  // 0xFF07
  uint8_t  reg_if = 0;   // 0xFF0F

  uint8_t  reg_lcdc = 0; // 0xFF40
  uint8_t  reg_stat = 0; // 0xFF41
  uint8_t  reg_scy = 0;  // 0xFF42
  uint8_t  reg_scx = 0;  // 0xFF43
  uint8_t  reg_ly = 0;   // 0xFF44
  uint8_t  reg_lyc = 0;  // 0xFF45
  uint8_t  reg_dma = 0;  // 0xFF46
  uint8_t  reg_bgp = 0;  // 0xFF47
  uint8_t  reg_obp0 = 0; // 0xFF48
  uint8_t  reg_obp1 = 0; // 0xFF49
  uint8_t  reg_wy = 0;   // 0xFF4A
  uint8_t  reg_wx = 0;   // 0xFF4B
  uint8_t  reg_lx = 0;   // Not mapped
  uint8_t  reg_ie = 0;   // 0xFFFF

  GateBoyReset sys_rst;
  GateBoyClock sys_clk;

  /*#p21.VOGA*/ uint8_t VOGA_HBLANKp = 0;
  /*#p21.XYMU*/ uint8_t XYMU_RENDERINGn = 0;
  /*#p04.MATU*/ uint8_t MATU_DMA_RUNNINGp = 0;
  /*#p28.ACYL*/ uint8_t ACYL_SCANNINGp_odd = 0;
  /*#p21.WODU*/ uint8_t WODU_HBLANKp_odd = 0;
  /*_p28.ATEJ*/ uint8_t ATEJ_LINE_RSTp_odd = 0;
  /*_p29.FEPO*/ uint8_t FEPO_STORE_MATCHp = 0;

  GateBoyCpuSignals cpu_signals;
  uint16_t cpu_abus = 0;
  uint8_t cpu_dbus = 0;

  InterruptControl int_ctrl;
  uint8_t int_latch = 0;

  uint16_t vram_abus = 0;
  uint8_t vram_dbus = 0;

  OamControl oam_ctrl;
  uint8_t  oam_abus = 0;
  uint8_t  oam_dbus_a = 0;
  uint8_t  oam_dbus_b = 0;
  uint8_t  oam_latch_a = 0;
  uint8_t  oam_latch_b = 0;
  uint8_t  oam_temp_a = 0;
  uint8_t  oam_temp_b = 0;

  uint8_t ext_data_latch = 0;
  uint16_t ext_addr_latch = 0;

  GateBoyMBC     ext_mbc;
  GateBoyZram    zram_bus;

  DmaControl dma_ctrl;
  uint8_t dma_lo = 0;


  uint8_t cpu_int = 0;
  uint8_t cpu_ack = 0;

  JoyInt   joy_int;
  uint8_t joy_latch = 0;

  //----------

  SpriteScanner    sprite_scanner;
  uint8_t scan_counter = 0;
  uint8_t sprite_counter = 0;
  uint8_t sprite_index = 0;
  uint16_t sprite_match_flags = 0;
  uint16_t sprite_reset_flags = 0;
  uint16_t sprite_store_flags = 0;

  uint8_t sprite_ibus = 0;
  uint8_t sprite_lbus = 0;

  uint8_t store_i[10] = {0};
  uint8_t store_l[10] = {0};
  uint8_t store_x[10] = {0};

  //----------


  uint8_t sfetch_counter_evn = 0;
  SpriteFetchControl sfetch_control;
  //----------

  uint8_t tfetch_counter_odd = 0;
  TileFetchControl tfetch_control;
  uint8_t tile_temp_a = 0;
  uint8_t tile_temp_b = 0;

  //----------

  WinControl win_ctrl;

  struct {
    uint8_t map = 0;
  } win_x;

  struct {
    uint8_t tile = 0;
    uint8_t map = 0;
  } win_y;

  uint8_t fine_count_odd = 0;
  FineScroll fine_scroll = {};

  //----------

  uint8_t flipped_sprite = 0;
  uint8_t sprite_pix_a = 0;
  uint8_t sprite_pix_b = 0;

  uint8_t pix_count = 0;
  uint8_t mask_pipe = 0;
  uint8_t bgw_pipe_a = 0;
  uint8_t bgw_pipe_b = 0;
  uint8_t spr_pipe_a = 0;
  uint8_t spr_pipe_b = 0;
  uint8_t pal_pipe = 0;

  LCDControl lcd = {};

#ifdef SIM_AUDIO
  int64_t apu_phase_new = 0;
  GateBoySPU  spu = {};
  SpuChannel1 ch1 = {};
  SpuChannel2 ch2 = {};
  SpuChannel3 ch3 = {};
  SpuChannel4 ch4 = {};
  GateBoyWaveBus wave_dbus = {};
#endif

  // new stuff that doesn't translate to GBState

  int64_t phase_lcd = 0;

  int phase_tfetch = 0;
  int phase_sfetch = 0;
};
