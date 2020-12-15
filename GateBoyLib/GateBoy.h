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
#include "GateBoyLib/GateBoyCpuBus.h"

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

  void tock_pix_pipe(
    wire XODO_VID_RSTp,
    wire XYMU_RENDERINGp,
    wire SACU_CLKPIPE_evn,
    wire NYXU_BFETCH_RSTn
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
    Signal REMY_LD0n,
    Signal RAVO_LD1n
  );

  void tock_vram(
    wire AVOR_SYS_RSTp,
    wire XODO_VID_RSTp,
    wire ATAL_xBxDxFxH,
    wire ABUZ_xxCDEFGH,
    wire TUTO_DBG_VRAMp_new,
    wire ATEJ_LINE_RSTp,
    wire TEVO_FETCH_TRIGp_new,
    wire NYXU_BFETCH_RSTn_new_xxx,
    wire PARU_VBLANKp_new_evn,
    wire WYMO_LCDC_WINENn_new,
    wire XYMU_RENDERINGp_new_xxx
  );

  void tock_oam(
    wire AVOR_SYS_RSTp,
    wire ATAL_xBxDxFxH_clk_odd,
    wire UVYT_ABCDxxxx_clkevn,
    wire XOCE_xBCxxFGx_clkodd,
    wire XYSO_xBCDxFGH_clkodd,
    wire XYMU_RENDERINGp_new_xxx,
    wire ACYL_SCANNINGp_new_evn
  );

  void tock_zram(
  );

  static wire ROGE_WY_MATCHp(const RegWY& reg_wy, const RegLY& reg_ly, wire WYMO_LCDC_WINENp) {
    /*#p27.NAZE*/ wire _NAZE_WY_MATCH0p = xnor2(reg_wy.NESO_WY0n.qn_old(), reg_ly.MUWY_LY0p.qp_old());
    /* p27.PEBO*/ wire _PEBO_WY_MATCH1p = xnor2(reg_wy.NYRO_WY1n.qn_old(), reg_ly.MYRO_LY1p.qp_old());
    /* p27.POMO*/ wire _POMO_WY_MATCH2p = xnor2(reg_wy.NAGA_WY2n.qn_old(), reg_ly.LEXA_LY2p.qp_old());
    /* p27.NEVU*/ wire _NEVU_WY_MATCH3p = xnor2(reg_wy.MELA_WY3n.qn_old(), reg_ly.LYDO_LY3p.qp_old());
    /* p27.NOJO*/ wire _NOJO_WY_MATCH4p = xnor2(reg_wy.NULO_WY4n.qn_old(), reg_ly.LOVU_LY4p.qp_old());
    /* p27.PAGA*/ wire _PAGA_WY_MATCH5p = xnor2(reg_wy.NENE_WY5n.qn_old(), reg_ly.LEMA_LY5p.qp_old());
    /* p27.PEZO*/ wire _PEZO_WY_MATCH6p = xnor2(reg_wy.NUKA_WY6n.qn_old(), reg_ly.MATO_LY6p.qp_old());
    /* p27.NUPA*/ wire _NUPA_WY_MATCH7p = xnor2(reg_wy.NAFU_WY7n.qn_old(), reg_ly.LAFO_LY7p.qp_old());

    /*#p27.PALO*/ wire _PALO_WY_MATCHn  = nand5(WYMO_LCDC_WINENp, _NOJO_WY_MATCH4p, _PAGA_WY_MATCH5p, _PEZO_WY_MATCH6p, _NUPA_WY_MATCH7p);
    /* p27.NELE*/ wire _NELE_WY_MATCHp  = not1(_PALO_WY_MATCHn);
    /* p27.PAFU*/ wire _PAFU_WY_MATCHn  = nand5(_NELE_WY_MATCHp, _NAZE_WY_MATCH0p, _PEBO_WY_MATCH1p, _POMO_WY_MATCH2p, _NEVU_WY_MATCH3p);
    /* p27.ROGE*/ wire _ROGE_WY_MATCHp  = not1(_PAFU_WY_MATCHn);
    return _ROGE_WY_MATCHp;
  }

  static wire NUKO_WX_MATCHp(const PixCounter& pix_count, const RegWX& reg_wx, wire REJO_WY_MATCH_LATCHp) {
    /*#p27.MYLO*/ wire _MYLO_WX_MATCH0p = xnor2(pix_count.XEHO_PX0p.qp_old(), reg_wx.MYPA_WX0n.qn_old());
    /* p27.PUWU*/ wire _PUWU_WX_MATCH1p = xnor2(pix_count.SAVY_PX1p.qp_old(), reg_wx.NOFE_WX1n.qn_old());
    /* p27.PUHO*/ wire _PUHO_WX_MATCH2p = xnor2(pix_count.XODU_PX2p.qp_old(), reg_wx.NOKE_WX2n.qn_old());
    /* p27.NYTU*/ wire _NYTU_WX_MATCH3p = xnor2(pix_count.XYDO_PX3p.qp_old(), reg_wx.MEBY_WX3n.qn_old());
    /* p27.NEZO*/ wire _NEZO_WX_MATCH4p = xnor2(pix_count.TUHU_PX4p.qp_old(), reg_wx.MYPU_WX4n.qn_old());
    /* p27.NORY*/ wire _NORY_WX_MATCH5p = xnor2(pix_count.TUKY_PX5p.qp_old(), reg_wx.MYCE_WX5n.qn_old());
    /* p27.NONO*/ wire _NONO_WX_MATCH6p = xnor2(pix_count.TAKO_PX6p.qp_old(), reg_wx.MUVO_WX6n.qn_old());
    /* p27.PASE*/ wire _PASE_WX_MATCH7p = xnor2(pix_count.SYBE_PX7p.qp_old(), reg_wx.NUKU_WX7n.qn_old());

    /* p27.PUKY*/ wire _PUKY_WX_MATCHn  = nand5(REJO_WY_MATCH_LATCHp, _NEZO_WX_MATCH4p, _NORY_WX_MATCH5p, _NONO_WX_MATCH6p, _PASE_WX_MATCH7p);
    /* p27.NUFA*/ wire _NUFA_WX_MATCHp  = not1(_PUKY_WX_MATCHn);
    /* p27.NOGY*/ wire _NOGY_WX_MATCHn  = nand5(_NUFA_WX_MATCHp, _MYLO_WX_MATCH0p, _PUWU_WX_MATCH1p, _PUHO_WX_MATCH2p, _NYTU_WX_MATCH3p);
    /* p27.NUKO*/ wire _NUKO_WX_MATCHp  = not1(_NOGY_WX_MATCHn);
    return _NUKO_WX_MATCHp;
  }

  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything in this section should derive
  // from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  //----------
  // CPU interface

  GateBoyCpuBus cpu_bus;

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

  GateBoyResetDebug rst;
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
