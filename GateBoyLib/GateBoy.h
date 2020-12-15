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

#pragma warning(push)
#pragma warning(disable : 4189)

inline BGScrollX scroll_x_adder(PixCount& pix_count, RegSCX reg_scx) {
  /*#p26.ATAD*/ wire _ATAD_TILE_X0S = add_s(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n.qn_new(), 0);
  /*#p26.ATAD*/ wire _ATAD_TILE_X0C = add_c(pix_count.XEHO_PX0p.qp_new(), reg_scx.DATY_SCX0n.qn_new(), 0);
  /* p26.BEHU*/ wire _BEHU_TILE_X1S = add_s(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n.qn_new(), _ATAD_TILE_X0C);
  /* p26.BEHU*/ wire _BEHU_TILE_X1C = add_c(pix_count.SAVY_PX1p.qp_new(), reg_scx.DUZU_SCX1n.qn_new(), _ATAD_TILE_X0C);
  /* p26.APYH*/ wire _APYH_TILE_X2S = add_s(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n.qn_new(), _BEHU_TILE_X1C);
  /* p26.APYH*/ wire _APYH_TILE_X2C = add_c(pix_count.XODU_PX2p.qp_new(), reg_scx.CYXU_SCX2n.qn_new(), _BEHU_TILE_X1C);
  /* p26.BABE*/ wire _BABE_MAP_X0S  = add_s(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n.qn_new(), _APYH_TILE_X2C);
  /* p26.BABE*/ wire _BABE_MAP_X0C  = add_c(pix_count.XYDO_PX3p.qp_new(), reg_scx.GUBO_SCX3n.qn_new(), _APYH_TILE_X2C);
  /* p26.ABOD*/ wire _ABOD_MAP_X1S  = add_s(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n.qn_new(), _BABE_MAP_X0C);
  /* p26.ABOD*/ wire _ABOD_MAP_X1C  = add_c(pix_count.TUHU_PX4p.qp_new(), reg_scx.BEMY_SCX4n.qn_new(), _BABE_MAP_X0C);
  /* p26.BEWY*/ wire _BEWY_MAP_X2S  = add_s(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n.qn_new(), _ABOD_MAP_X1C);
  /* p26.BEWY*/ wire _BEWY_MAP_X2C  = add_c(pix_count.TUKY_PX5p.qp_new(), reg_scx.CUZY_SCX5n.qn_new(), _ABOD_MAP_X1C);
  /* p26.BYCA*/ wire _BYCA_MAP_X3S  = add_s(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n.qn_new(), _BEWY_MAP_X2C);
  /* p26.BYCA*/ wire _BYCA_MAP_X3C  = add_c(pix_count.TAKO_PX6p.qp_new(), reg_scx.CABU_SCX6n.qn_new(), _BEWY_MAP_X2C);
  /* p26.ACUL*/ wire _ACUL_MAP_X4S  = add_s(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n.qn_new(), _BYCA_MAP_X3C);
  /* p26.ACUL*/ wire _ACUL_MAP_X4C  = add_c(pix_count.SYBE_PX7p.qp_new(), reg_scx.BAKE_SCX7n.qn_new(), _BYCA_MAP_X3C);

  return {
    _ATAD_TILE_X0S,
    _BEHU_TILE_X1S,
    _APYH_TILE_X2S,
    _BABE_MAP_X0S,
    _ABOD_MAP_X1S,
    _BEWY_MAP_X2S,
    _BYCA_MAP_X3S,
    _ACUL_MAP_X4S
  };
}

inline BGScrollY scroll_y_adder(RegLY& reg_ly, RegSCY& reg_scy) {
  /*#p26.FAFO*/ wire _FAFO_TILE_Y0S = add_s(reg_ly.MUWY_LY0p.qp_new(), reg_scy.GAVE_SCY0n.qn_new(), 0);
  /*#p26.FAFO*/ wire _FAFO_TILE_Y0C = add_c(reg_ly.MUWY_LY0p.qp_new(), reg_scy.GAVE_SCY0n.qn_new(), 0);
  /* p26.EMUX*/ wire _EMUX_TILE_Y1S = add_s(reg_ly.MYRO_LY1p.qp_new(), reg_scy.FYMO_SCY1n.qn_new(), _FAFO_TILE_Y0C);
  /* p26.EMUX*/ wire _EMUX_TILE_Y1C = add_c(reg_ly.MYRO_LY1p.qp_new(), reg_scy.FYMO_SCY1n.qn_new(), _FAFO_TILE_Y0C);
  /* p26.ECAB*/ wire _ECAB_TILE_Y2S = add_s(reg_ly.LEXA_LY2p.qp_new(), reg_scy.FEZU_SCY2n.qn_new(), _EMUX_TILE_Y1C);
  /* p26.ECAB*/ wire _ECAB_TILE_Y2C = add_c(reg_ly.LEXA_LY2p.qp_new(), reg_scy.FEZU_SCY2n.qn_new(), _EMUX_TILE_Y1C);
  /* p26.ETAM*/ wire _ETAM_MAP_Y0S  = add_s(reg_ly.LYDO_LY3p.qp_new(), reg_scy.FUJO_SCY3n.qn_new(), _ECAB_TILE_Y2C);
  /* p26.ETAM*/ wire _ETAM_MAP_Y0C  = add_c(reg_ly.LYDO_LY3p.qp_new(), reg_scy.FUJO_SCY3n.qn_new(), _ECAB_TILE_Y2C);
  /* p26.DOTO*/ wire _DOTO_MAP_Y1S  = add_s(reg_ly.LOVU_LY4p.qp_new(), reg_scy.DEDE_SCY4n.qn_new(), _ETAM_MAP_Y0C);
  /* p26.DOTO*/ wire _DOTO_MAP_Y1C  = add_c(reg_ly.LOVU_LY4p.qp_new(), reg_scy.DEDE_SCY4n.qn_new(), _ETAM_MAP_Y0C);
  /* p26.DABA*/ wire _DABA_MAP_Y2S  = add_s(reg_ly.LEMA_LY5p.qp_new(), reg_scy.FOTY_SCY5n.qn_new(), _DOTO_MAP_Y1C);
  /* p26.DABA*/ wire _DABA_MAP_Y2C  = add_c(reg_ly.LEMA_LY5p.qp_new(), reg_scy.FOTY_SCY5n.qn_new(), _DOTO_MAP_Y1C);
  /* p26.EFYK*/ wire _EFYK_MAP_Y3S  = add_s(reg_ly.MATO_LY6p.qp_new(), reg_scy.FOHA_SCY6n.qn_new(), _DABA_MAP_Y2C);
  /* p26.EFYK*/ wire _EFYK_MAP_Y3C  = add_c(reg_ly.MATO_LY6p.qp_new(), reg_scy.FOHA_SCY6n.qn_new(), _DABA_MAP_Y2C);
  /* p26.EJOK*/ wire _EJOK_MAP_Y4S  = add_s(reg_ly.LAFO_LY7p.qp_new(), reg_scy.FUNY_SCY7n.qn_new(), _EFYK_MAP_Y3C);
  /* p26.EJOK*/ wire _EJOK_MAP_Y4C  = add_c(reg_ly.LAFO_LY7p.qp_new(), reg_scy.FUNY_SCY7n.qn_new(), _EFYK_MAP_Y3C);

  return {
    _FAFO_TILE_Y0S,
    _EMUX_TILE_Y1S,
    _ECAB_TILE_Y2S,
    _ETAM_MAP_Y0S,
    _DOTO_MAP_Y1S,
    _DABA_MAP_Y2S,
    _EFYK_MAP_Y3S,
    _EJOK_MAP_Y4S
  };
}

#pragma warning(pop)

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

  void update_lcd();

  uint8_t* reg_begin() { return (uint8_t*)(&sentinel1) + sizeof(sentinel1); }
  uint8_t* reg_end()   { return (uint8_t*)(&sentinel2); }
  int64_t commit_and_hash() {
    return ::commit_and_hash(reg_begin(), reg_end() - reg_begin());
  }

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

  static wire NUKO_WX_MATCHp(const PixCount& pix_count, const RegWX& reg_wx, wire REJO_WY_MATCH_LATCHp) {
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
  GateBoyClock clk;

  //----------

  GateBoyDiv     div;
  GateBoyTimer   timer;

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

  PixCount pix_count;
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

  Signal old_zram_clk;

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
