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
#include "GateBoyLib/GateBoyZramBus.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyCpuBus.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {

  void dump(Dumper& d) const;

  void reset_to_bootrom(bool fastboot);
  void reset_to_cart();
  void load_cart(uint8_t* _boot_buf, size_t _boot_size,
                uint8_t* _cart_buf, size_t _cart_size);

  //----------------------------------------

  void from_blob(const blob& b) {
    ASSERT_P(b.size() == sizeof(*this));
    memcpy(this, b.data(), sizeof(*this));
    check_sentinel();
  }

  void to_blob(blob& b) {
    uint8_t* bytes = (uint8_t*)this;
    b.insert(b.end(), bytes, bytes + sizeof(*this));
  }

  //----------------------------------------

  void check_div() const {
    int div_val = div.get_div();
    if (div_val != BOOT_DIV) {
      printf("div fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  static bool check_sentinel(const blob& b) {
    if (b.size() != sizeof(GateBoy)) return false;

    GateBoy* gb = (GateBoy*)b.data();
    if (gb->sentinel1 != SENTINEL1) return false;
    if (gb->sentinel2 != SENTINEL2) return false;
    if (gb->sentinel3 != SENTINEL3) return false;
    if (gb->sentinel4 != SENTINEL4) return false;
    return true;
  }

  void check_sentinel() const {
    ASSERT_P(sentinel1 == SENTINEL1);
    ASSERT_P(sentinel2 == SENTINEL2);
    ASSERT_P(sentinel3 == SENTINEL3);
    ASSERT_P(sentinel4 == SENTINEL4);
  }

  uint8_t dbg_read (int addr);
  void dbg_write(int addr, uint8_t data);

  void set_boot_bit() {
    dbg_write(0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  void run_phases(int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase();
    }
  }

  void next_phase();

  void tock_slow(int pass_index);

  void update_framebuffer();

  uint8_t* reg_begin() { return (uint8_t*)(&sentinel1) + sizeof(sentinel1); }
  uint8_t* reg_end()   { return (uint8_t*)(&sentinel2); }

  int64_t commit_and_hash() {
    return ::commit_and_hash(reg_begin(), reg_end() - reg_begin());
  }

  void set_old_bits() {
    uint8_t* b = reg_begin();
    size_t s = reg_end() - reg_begin();
    for (size_t i = 0; i < s; i++) {
      b[i] |= 0b00010000;
    }
  }

  static wire2 ROGE_WY_MATCHp_old(const RegWY& reg_wy_old, const RegLY& reg_ly_old, wire2 WYMO_LCDC_WINENp_old) {
    /*#p27.NAZE*/ wire2 _NAZE_WY_MATCH0p = xnor2b(reg_wy_old.NESO_WY0n.qn_old2(), reg_ly_old.MUWY_LY0p.qp_old2());
    /* p27.PEBO*/ wire2 _PEBO_WY_MATCH1p = xnor2b(reg_wy_old.NYRO_WY1n.qn_old2(), reg_ly_old.MYRO_LY1p.qp_old2());
    /* p27.POMO*/ wire2 _POMO_WY_MATCH2p = xnor2b(reg_wy_old.NAGA_WY2n.qn_old2(), reg_ly_old.LEXA_LY2p.qp_old2());
    /* p27.NEVU*/ wire2 _NEVU_WY_MATCH3p = xnor2b(reg_wy_old.MELA_WY3n.qn_old2(), reg_ly_old.LYDO_LY3p.qp_old2());
    /* p27.NOJO*/ wire2 _NOJO_WY_MATCH4p = xnor2b(reg_wy_old.NULO_WY4n.qn_old2(), reg_ly_old.LOVU_LY4p.qp_old2());
    /* p27.PAGA*/ wire2 _PAGA_WY_MATCH5p = xnor2b(reg_wy_old.NENE_WY5n.qn_old2(), reg_ly_old.LEMA_LY5p.qp_old2());
    /* p27.PEZO*/ wire2 _PEZO_WY_MATCH6p = xnor2b(reg_wy_old.NUKA_WY6n.qn_old2(), reg_ly_old.MATO_LY6p.qp_old2());
    /* p27.NUPA*/ wire2 _NUPA_WY_MATCH7p = xnor2b(reg_wy_old.NAFU_WY7n.qn_old2(), reg_ly_old.LAFO_LY7p.qp_old2());

    /*#p27.PALO*/ wire2 _PALO_WY_MATCHn  = nand5b(WYMO_LCDC_WINENp_old, _NOJO_WY_MATCH4p, _PAGA_WY_MATCH5p, _PEZO_WY_MATCH6p, _NUPA_WY_MATCH7p);
    /* p27.NELE*/ wire2 _NELE_WY_MATCHp  = not1b(_PALO_WY_MATCHn);
    /* p27.PAFU*/ wire2 _PAFU_WY_MATCHn  = nand5b(_NELE_WY_MATCHp, _NAZE_WY_MATCH0p, _PEBO_WY_MATCH1p, _POMO_WY_MATCH2p, _NEVU_WY_MATCH3p);
    /* p27.ROGE*/ wire2 _ROGE_WY_MATCHp  = not1b(_PAFU_WY_MATCHn);
    return _ROGE_WY_MATCHp;
  }

  static wire2 NUKO_WX_MATCHp_old(const PixCount& pix_count_old, const RegWX& reg_wx_old, wire2 REJO_WY_MATCH_LATCHp_old) {
    /*#p27.MYLO*/ wire2 _MYLO_WX_MATCH0p = xnor2b(pix_count_old.XEHO_PX0p.qp_old2(), reg_wx_old.MYPA_WX0n.qn_old2());
    /* p27.PUWU*/ wire2 _PUWU_WX_MATCH1p = xnor2b(pix_count_old.SAVY_PX1p.qp_old2(), reg_wx_old.NOFE_WX1n.qn_old2());
    /* p27.PUHO*/ wire2 _PUHO_WX_MATCH2p = xnor2b(pix_count_old.XODU_PX2p.qp_old2(), reg_wx_old.NOKE_WX2n.qn_old2());
    /* p27.NYTU*/ wire2 _NYTU_WX_MATCH3p = xnor2b(pix_count_old.XYDO_PX3p.qp_old2(), reg_wx_old.MEBY_WX3n.qn_old2());
    /* p27.NEZO*/ wire2 _NEZO_WX_MATCH4p = xnor2b(pix_count_old.TUHU_PX4p.qp_old2(), reg_wx_old.MYPU_WX4n.qn_old2());
    /* p27.NORY*/ wire2 _NORY_WX_MATCH5p = xnor2b(pix_count_old.TUKY_PX5p.qp_old2(), reg_wx_old.MYCE_WX5n.qn_old2());
    /* p27.NONO*/ wire2 _NONO_WX_MATCH6p = xnor2b(pix_count_old.TAKO_PX6p.qp_old2(), reg_wx_old.MUVO_WX6n.qn_old2());
    /* p27.PASE*/ wire2 _PASE_WX_MATCH7p = xnor2b(pix_count_old.SYBE_PX7p.qp_old2(), reg_wx_old.NUKU_WX7n.qn_old2());

    /* p27.PUKY*/ wire2 _PUKY_WX_MATCHn  = nand5b(REJO_WY_MATCH_LATCHp_old, _NEZO_WX_MATCH4p, _NORY_WX_MATCH5p, _NONO_WX_MATCH6p, _PASE_WX_MATCH7p);
    /* p27.NUFA*/ wire2 _NUFA_WX_MATCHp  = not1b(_PUKY_WX_MATCHn);
    /* p27.NOGY*/ wire2 _NOGY_WX_MATCHn  = nand5b(_NUFA_WX_MATCHp, _MYLO_WX_MATCH0p, _PUWU_WX_MATCH1p, _PUHO_WX_MATCH2p, _NYTU_WX_MATCH3p);
    /* p27.NUKO*/ wire2 _NUKO_WX_MATCHp  = not1b(_NOGY_WX_MATCHn);
    return _NUKO_WX_MATCHp;
  }

  /*#p01.ABUZ*/ static wire2 ABUZ_EXT_RAM_CS_CLK(const GateBoyResetDebug& rst, const GateBoyClock& clk, const GateBoyCpuBus& cpu_bus) {
    /*#p01.AGUT*/ wire2 _AGUT_xxCDEFGH = or_and3(clk.AROV_xxCDEFxx(), clk.AJAX_xxxxEFGH(), cpu_bus.SIG_CPU_EXT_BUSp.qp_new2());
    /*#p01.AWOD*/ wire2 _AWOD_ABxxxxxx = nor2b(rst.UNOR_MODE_DBG2p(), _AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ wire2 _ABUZ_EXT_RAM_CS_CLK = not1b(_AWOD_ABxxxxxx);
    return _ABUZ_EXT_RAM_CS_CLK;
  }


  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything in this section should derive
  // from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  //----------
  // CPU interface

  GateBoyCpuBus  cpu_bus;
  GateBoyExtBus  ext_bus;
  GateBoyVramBus vram_bus;
  GateBoyOamBus  oam_bus;
  GateBoyZramBus zram_bus;

  //----------
  GateBoyResetDebug rst;
  GateBoyClock      clk;
  GateBoyDiv        div;
  GateBoyTimer      timer;
  GateBoyDMA        dma;
  GateBoyInterrupts interrupts;
  GateBoyJoypad     joypad;
  GateBoySerial     serial;

  //----------

  Bus SPR_TRI_I[6]; // AxCxExGx
  Bus SPR_TRI_L[4]; // AxCxExGx

  SpriteStore   sprite_store;
  SpriteScanner sprite_scanner;
  SpriteFetcher sprite_fetcher;
  TileFetcher   tile_fetcher;

  RegLCDC reg_lcdc;
  RegStat reg_stat;
  RegSCX  reg_scx;
  RegSCY  reg_scy;
  RegWY   reg_wy;
  RegWX   reg_wx;

  WinMapX         win_map_x;
  WinLineY        win_line_y;
  WindowRegisters win_reg;
  FineScroll      fine_scroll;

  PPURegisters ppu_reg;
  PixCount     pix_count;
  PixelPipes   pix_pipes;
  GateBoyLCD   lcd;

  //----------

  uint64_t sentinel2 = SENTINEL2;

  //-----------------------------------------------------------------------------
  // Control stuff

  uint8_t sys_rst = 0;
  uint8_t sys_t1 = 0;
  uint8_t sys_t2 = 0;
  uint8_t sys_clken = 0;
  uint8_t sys_clkgood = 0;
  uint8_t sys_clkreq = 0;
  uint8_t sys_cpu_en = 0;
  uint8_t sys_fastboot = 0;
  uint8_t sys_buttons = 0;

  //-----------------------------------------------------------------------------
  // CPU

  MetroBoyCPU      cpu;

  Req      bus_req_old = {0};
  Req      bus_req_new = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  int_ack_latch = 0;
  uint8_t  intf_latch = 0;
  uint8_t  intf_latch_delay = 0;
  uint8_t  intf_halt_latch = 0;

  //-----------------------------------------------------------------------------
  // Memory

  uint8_t vid_ram [8192];
  uint8_t cart_ram[8192];
  uint8_t int_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];

  uint64_t sentinel3 = SENTINEL3;

  //-----------------------------------------------------------------------------
  // LCD and framebuffer

  int old_lcd_x = 0;
  int old_lcd_y = 0;

  uint8_t framebuffer[160*144];
  //uint8_t gb_screen_x = 0;
  //uint8_t gb_screen_y = 0;

  //-----------------------------------------------------------------------------

  double   sim_time = 0;
  uint64_t phase_total = 0;
  uint64_t phase_origin = 0;

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
