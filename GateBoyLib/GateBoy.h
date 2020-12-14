#pragma once

#include "CoreLib/File.h"
#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"

#include "GateBoyLib/Probe.h"

#include "GateBoyLib/GateBoyBootrom.h"
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
#include "GateBoyLib/GateBoyResetDebug.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {

  void dump(Dumper& d) const;

  void reset_boot(uint8_t* _boot_buf, size_t _boot_size,
                     uint8_t* _cart_buf, size_t _cart_size, bool fastboot);

  void reset_cart(uint8_t* _boot_buf, size_t _boot_size,
                   uint8_t* _cart_buf, size_t _cart_size);

  void set_cart(uint8_t* _boot_buf, size_t _boot_size,
                uint8_t* _cart_buf, size_t _cart_size);


  void load_post_bootrom_state();

  //----------------------------------------

  void load_dump(const char* filename) {
    load_obj(filename, *this);
    check_sentinel();
  }

  void save_dump(const char* filename) {
    save_obj(filename, *this);
  }

  //----------------------------------------

  void check_div() const {
    int div_val = div.get_div();
    if (div_val != BOOT_DIV) {
      printf("div fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  void check_sentinel() const {
    if (sentinel1 != SENTINEL1) {
      printf("sentinel1 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel2 != SENTINEL2) {
      printf("sentinel2 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel3 != SENTINEL3) {
      printf("sentinel3 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel4 != SENTINEL4) {
      printf("sentinel4 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  uint8_t dbg_read (int addr);
  void dbg_write(int addr, uint8_t data);

  void set_boot_bit() {
    dbg_write(0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  void run(int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase();
    }
  }

  void next_phase();

  void tock_slow(int pass_index);

  uint8_t* reg_begin() { return (uint8_t*)(&sentinel1) + sizeof(sentinel1); }
  uint8_t* reg_end()   { return (uint8_t*)(&sentinel2); }
  int64_t commit_and_hash() {
    return ::commit_and_hash(reg_begin(), reg_end() - reg_begin());
  }

  void tock_reg_lyc(
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,

    wire WERO_ADDR_PPUp_ext,
    wire XOLA_A00n_ext, wire WADO_A00p_ext,
    wire XENO_A01n_ext, wire WESA_A01p_ext,
    wire XUSY_A02n_ext, wire WALO_A02p_ext,
    wire XERA_A03n_ext, wire WEPO_A03p_ext
  );

  void tock_reg_stat(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,

    wire ACYL_SCANNINGp_new_evn,
    wire XYMU_RENDERINGp_new_xxx,
    wire PARU_VBLANKp_new_evn
  );

  void tock_pix_pipe(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire XODO_VID_RSTp_new_h,

    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,

    wire XYMU_RENDERINGp_new_xxx,
    wire SACU_CLKPIPE_AxCxExGx_clknew_evn,
    wire NYXU_BFETCH_RSTn_new_xxx,

    wire &REMY_LD0n_new,
    wire &RAVO_LD1n_new
  );

  void tock_lcd(
    wire TALU_xxCDEFxx,
    wire TYFA_CLKPIPE_odd,
    wire SACU_CLKPIPE_evn,
    wire XONA_LCDC_LCDENp,
    wire XYMU_RENDERINGp,
    wire AVAP_SCAN_DONE_TRIGp,
    wire WEGO_HBLANKp,
    wire PURE_LINE_ENDn,
    wire REMY_LD0n,
    wire RAVO_LD1n
  );

  void tock_ext(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire ABUZ_xxCDEFGH_clk_evn,
    wire TEDO_CPU_RDp,
    wire APOV_CPU_WRp_clkevn,
    wire TEXO_ADDR_VRAMn_ext,
    wire TUTU_READ_BOOTROMp_new
  );

  void tock_vram(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],

    wire AVOR_SYS_RSTp,
    wire XODO_VID_RSTp_new_h,

    wire ATAL_xBxDxFxH_clk_odd,
    wire ABUZ_xxCDEFGH_clk_evn,

    wire CATY_LATCH_EXTp_ext,
    wire SOSE_ADDR_VRAMp_ext,

    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    wire APOV_CPU_WRp_clkevn,

    wire TUTO_DBG_VRAMp_new,

    wire ATEJ_LINE_RSTp,
    wire TEVO_FETCH_TRIGp_new,
    wire NYXU_BFETCH_RSTn_new_xxx,
    wire PARU_VBLANKp_new_evn,
    wire WYMO_LCDC_WINENn_new,
    wire XYMU_RENDERINGp_new_xxx
  );

  void tock_oam(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire ATAL_xBxDxFxH_clk_odd,
    wire UVYT_ABCDxxxx_clkevn,
    wire XOCE_xBCxxFGx_clkodd,
    wire XYSO_xBCDxFGH_clkodd,
    wire TAPU_CPU_WRp,
    wire TEDO_CPU_RDp,
    wire CATY_LATCH_EXTp_ext,
    wire XYMU_RENDERINGp_new_xxx,
    wire ACYL_SCANNINGp_new_evn
  );

  void tock_zram(
    wire BUS_CPU_A[16],
    wire BUS_CPU_D[8],
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp
  );

  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything in this section should derive
  // from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  //----------
  // CPU interface

  BusOut BUS_CPU_D_out[8];

  PinIn  PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  PinIn  PIN_CPU_WRp;           // top right port PORTA_01: ->
  PinOut PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinOut PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  PinOut PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinOut PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  PinIn  PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP

  PinIn  PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  PinIn  PIN_CPU_LATCH_EXT;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  //----------

  OamTempA oam_temp_a;
  OamTempB oam_temp_b;
  OamLatchA oam_latch_a;
  OamLatchB oam_latch_b;
  GateBoyOamBus oam_bus;

  //----------

  ExtDataLatch ext_data_latch;
  ExtAddrLatch ext_addr_latch;
  GateBoyExtBus ext_bus;

  //----------

  SpriteTempA sprite_temp_a;
  SpriteTempB sprite_temp_b;
  GateBoyVramBus vram_bus;

  //----------

  GateBoyResetDebug   rstdbg;
  GateBoyPhaseClock pclk;
  GateBoyVideoClock vclk;

  //----------

  GateBoyDiv     div;
  GateBoyTimer   timer;
  GateBoyBootrom bootrom;

  GateBoyDMA dma;

  GateBoyInterrupts interrupts;

  GateBoyJoypad joypad;

  GateBoySerial serial;

  //----------

  SpriteCounter sprite_counter;
  SpriteStore   sprite_store;
  SpriteScanner sprite_scanner;
  ScanCounter   scan_counter;

  //----------

  TileFetcher tile_fetcher;
  TileTempA tile_temp_a;
  TileTempB tile_temp_b;
  RegSCX reg_scx;
  RegSCY reg_scy;
  WinMapX win_map_x;
  WinLineY win_line_y;

  //----------

  SpriteFetcher sprite_fetcher;

  //----------

  PixCounter pix_count;
  RegStat reg_stat;
  RegBGP  reg_bgp;
  RegOBP0 reg_obp0;
  RegOBP1 reg_obp1;
  WindowRegisters win_reg;
  FineScroll fine_scroll;
  PPURegisters ppu_reg;
  PixelPipes pix_pipes;
  RegWY reg_wy;
  RegWX reg_wx;

  //----------

  RegLCDC reg_lcdc;
  RegLX   reg_lx;
  RegLY   reg_ly;
  RegLYC  reg_lyc;
  GateBoyLCD lcd;

  //----------

  uint64_t sentinel2 = SENTINEL2;

  bool oam_clk_old = 0;
  bool zram_clk_old = 0;

  //-----------------------------------------------------------------------------
  // Control stuff

  bool sys_rst = 0;
  bool sys_t1 = 0;
  bool sys_t2 = 0;
  bool sys_clken = 0;
  bool sys_clkgood = 0;
  bool sys_clkreq = 0;
  bool sys_cpu_en = 0;
  bool sys_fastboot = 0;
  bool sys_cpu_start = 0;
  uint8_t sys_buttons = 0;

  //-----------------------------------------------------------------------------
  // CPU

  MetroBoyCPU      cpu;
  Req      cpu_req = {0};
  Req      dbg_req = {0};
  Req      bus_req = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;

  bool int_vblank = 0;
  bool int_vblank_halt = 0;

  bool int_stat = 0;
  bool int_stat_halt = 0;

  bool int_timer = 0;
  bool int_timer_halt = 0;

  bool int_serial = 0;
  bool int_serial_halt = 0;

  bool int_joypad = 0;
  bool int_joypad_halt = 0;

  //-----------------------------------------------------------------------------
  // Memory

  uint8_t vid_ram [8192];
  uint8_t cart_ram[8192];
  uint8_t ext_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];

  //-----------------------------------------------------------------------------
  // LCD and framebuffer

  uint8_t framebuffer[160*144];
  uint8_t gb_screen_x = 0;
  uint8_t gb_screen_y = 0;
  bool old_lcd_clock = 0;
  bool old_lcd_latch = 0;
  uint8_t lcd_data_latch = 0;

  //-----------------------------------------------------------------------------

  uint64_t sentinel3 = SENTINEL3;

  double   sim_time = 0;
  int32_t  phase_total = 0;
  uint64_t phase_hash = 0;
  uint64_t cumulative_hash = 0;

  Probes   probes;

  uint8_t* boot_buf = nullptr;
  size_t   boot_size = 0;

  uint8_t* cart_buf = nullptr;
  size_t   cart_size = 0;

  uint64_t sentinel4 = SENTINEL4;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
