#pragma once

#include "CoreLib/File.h"
#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Log.h"

#include "GateBoyLib/Probe.h"

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
#include "GateBoyLib/GateBoySPU.h"
#include "GateBoyLib/GateBoyState.h"

#include <atomic>
#include <cstring>

uint32_t cart_rom_addr_mask(const blob& cart_blob);
uint32_t cart_ram_addr_mask(const blob & cart_blob);
bool cart_has_mbc1(const blob & cart_blob);
bool cart_has_ram(const blob & cart_blob);

struct GateBoy;

void print_field_at(int offset);
void diff_gb(GateBoy* gba, GateBoy* gbb, uint8_t mask);

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoyReg {
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
  RegDma  reg_dma;  // 0xFF46
  RegBGP  reg_bgp;  // 0xFF47
  RegOBP0 reg_obp0; // 0xFF48
  RegOBP1 reg_obp1; // 0xFF49
  RegWY   reg_wy;   // 0xFF4A
  RegWX   reg_wx;   // 0xFF4B
  RegLX   reg_lx;   // Not mapped
  RegIE   reg_ie;   // 0xFFFF

  GateBoyResetDebug sys_rst;
  GateBoyClock      sys_clk;

  /*#p21.VOGA*/ DFF17    VOGA_HBLANKp;
  /*#p21.XYMU*/ NorLatch XYMU_RENDERINGn;
  /*#p04.MATU*/ DFF17    MATU_DMA_RUNNINGp;
  /*#p28.ACYL*/ Gate     ACYL_SCANNINGp;
  /*#p21.WODU*/ Gate     WODU_HBLANKp;
  /*_p07.SATO*/ Gate     SATO_BOOT_BITn;
  /*_p28.ATEJ*/ Gate     ATEJ_LINE_RSTp;
  /*_p29.FEPO*/ Gate     FEPO_STORE_MATCHp;

  GateBoyCpuSignals cpu_signals;
  GateBoyCpuABus    cpu_abus;
  GateBoyCpuDBus    cpu_dbus;

  InterruptControl int_ctrl;
  InterruptLatch   int_latch;

  struct { VramABusLo lo; VramABusHi hi; } vram_abus;
  VramDBus       vram_dbus;
  VramExtControl vram_ext_ctrl;
  VramExtABus    vram_ext_abus;
  VramExtDBus    vram_ext_dbus;
                
  OamControl     oam_ctrl;
  OamABus        oam_abus;
  OamDBusA       oam_dbus_a;
  OamDBusB       oam_dbus_b;
  OamLatchA      oam_latch_a;
  OamLatchB      oam_latch_b;
  OamTempA       oam_temp_a;
  OamTempB       oam_temp_b;
                 
  ExtControl     ext_ctrl;
  
  struct {
    ExtABusLo      lo;
    ExtABusHi      hi;
  } ext_abus;

  ExtDBus        ext_dbus;
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
  JoyExt   joy_ext;

  //----------

  SpriteScanner    sprite_scanner;
  ScanCounter      scan_counter;
  SpriteCounter    sprite_counter;
  SpriteIndex      sprite_index;
  SpriteMatchFlags sprite_match_flags;
  SpriteResetFlags sprite_reset_flags;
  SpriteStoreFlags sprite_store_flags;

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


  SpriteFetchCounter sfetch_counter;
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

  FineCount  fine_count;
  FineScroll fine_scroll;

  //----------

  SpritePix flipped_sprite;
  SpritePixA sprite_pix_a;
  SpritePixB sprite_pix_b;

  PixCount pix_count;
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

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoyCpu {
  MetroBoyCPU core;
  Req      bus_req_new = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;
  uint8_t  intf_latch = 0;
  uint8_t  intf_latch_delay = 0;
  uint8_t  intf_halt_latch = 0;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoyMem {
  uint8_t vid_ram [8192];
  uint8_t cart_ram[32768];
  uint8_t int_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];
  uint8_t framebuffer[160*144];
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoySys {
  // External signals

  uint8_t rst = 0;
  uint8_t t1 = 0;
  uint8_t t2 = 0;
  uint8_t clken = 0;
  uint8_t clkgood = 0;
  uint8_t clkreq = 0;
  uint8_t cpu_en = 0;
  uint8_t fastboot = 0;
  uint8_t buttons = 0;

  // Debug stuff
  bool     logic_mode = config_fastmode; // Fastmode builds use logic mode by default.
  uint64_t phase_total = 0;
  double   sim_time = 0;
  uint64_t phase_origin = 0;
  Probes   probes;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {
  void reset_to_bootrom(const blob& cart_blob, bool fastboot);
  void reset_to_cart(const blob& cart_blob);

  //----------------------------------------

  static bool check_sentinel(const blob& b) {
    if (b.size() < sizeof(GateBoy)) return false;

    GateBoy* gb = (GateBoy*)b.data();
    if (gb->sentinel1 != SENTINEL1) return false;
    if (gb->sentinel2 != SENTINEL2) return false;
    return true;
  }

  void from_blob(const blob& b) {
    bool old_logic_mode = sys.logic_mode;
    CHECK_P(b.size() >= sizeof(GateBoy));
    memcpy(this, b.data(), sizeof(GateBoy));
    CHECK_P(sentinel1 == SENTINEL1);
    CHECK_P(sentinel2 == SENTINEL2);
    sys.logic_mode = old_logic_mode;
  }

  void to_blob(blob& b) {
    uint8_t* bytes = (uint8_t*)this;
    b.insert(b.end(), bytes, bytes + sizeof(*this));
  }

  //----------------------------------------

  void check_div() const {
    int div_val = bit_pack(reg.reg_div);
    if (div_val != BOOT_DIV) {
      LOG_R("div fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  bool dbg_read (const blob& cart_blob, int addr, uint8_t& out);
  bool dbg_write(const blob& cart_blob, int addr, uint8_t data);

  void set_boot_bit(const blob& cart_blob) {
    dbg_write(cart_blob, 0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  void run_phases(const blob& cart_blob, int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase(cart_blob);
    }
  }

  bool next_phase(const blob& cart_blob);

  void tock_cpu();
  void tock_gates(const blob& cart_blob);
  void tock_logic(const blob& cart_blob);

  void update_framebuffer(int lcd_x, int lcd_y, wire DATA0, wire DATA1);

  void wipe() {
    bool old_logic_mode = sys.logic_mode;
    memset(this, 0, sizeof(*this));

    if (!old_logic_mode) {
      memset(&reg, 0b00011000, sizeof(reg));
    }
    else {
      memset(&reg, 0, sizeof(reg));
    }

    sentinel1 = SENTINEL1;
    sentinel2 = SENTINEL2;
    sys.logic_mode = old_logic_mode;
  }

  void wipe_flags() {
    uint8_t* blob = (uint8_t*)&reg;
    for (int i = 0; i < sizeof(reg); i++) {
      blob[i] &= 1;
    }
  }

  bool check_no_flags() {
    uint8_t* blob = (uint8_t*)&reg;
    for (int i = 0; i < sizeof(reg); i++) {
      CHECK_N(blob[i] & ~1);
    }
    return true;
  }

  int64_t hash_regression() {
    if (sys.logic_mode) check_no_flags();
    return hash_low_bit(&reg, sizeof(reg), HASH_INIT);
  }

  int64_t hash_all() {
    return hash_all_bits(&reg, sizeof(reg), HASH_INIT);
  }

  void commit() {
    commit_blob(&reg, sizeof(reg));
  }

  void check_state_old_and_driven_or_pulled() {
    if (config_drive_flags) {
      uint8_t* blob = (uint8_t*)&reg;
      for (auto i = 0; i < sizeof(GateBoyReg); i++) {
        auto r = blob[i];
        (void)r;
        CHECK_P((r & 0xF0) == BIT_OLD);
        CHECK_P(bool(r & BIT_DRIVEN) != bool(r & BIT_PULLED));
      }
    }
  }

  //-----------------------------------------------------------------------------

  void tock_serial_logic(bool cpu_wr_old, bool cpu_wr_new, uint16_t cpu_addr_old, uint16_t cpu_addr_new, uint16_t div_old, uint16_t div_new);

  void tock_lcdc_gates(const GateBoyReg& reg_old);
  void tock_lyc_gates(const GateBoyReg& reg_old);
  void tock_lcd_gates();
  void tock_dma_gates(const GateBoyReg& reg_old);
  void tock_joypad_gates(const GateBoyReg& reg_old);
  void tock_interrupts_gates(const GateBoyReg& reg_old);
  void tock_clocks_gates();
  void tock_vid_clocks_gates();
  void tock_div_gates();
  void tock_timer_gates(const GateBoyReg& reg_old);
  void tock_reset_gates(DFF17 UPOF_DIV15p);
  void tock_ext_gates(const blob& cart_blob);
  void tock_oam_bus_gates();
  void tock_serial_gates();
  void tock_vram_bus_gates(const GateBoyReg& reg_old, wire TEVO_WIN_FETCH_TRIGp);
  void tock_zram_gates(const GateBoyReg& reg_old);
  void tock_pix_pipes_gates(const GateBoyReg& reg_old, wire SACU_CLKPIPE_evn, wire NYXU_BFETCH_RSTn);
  void tock_bootrom_gates();
  void tock_window_gates(const GateBoyReg& reg_old, wire SEGU_CLKPIPE_evn, wire REPU_VBLANKp);
  void update_sprite_store_flags_gates(SpriteCounter& sprite_counter, wire DYTY_COUNT_CLKp, SpriteStoreFlags& sprite_store_flags);

  void tock_spu_gates();
  void tock_spu_logic();

  void store_sprite_gates(SpriteStoreFlags& sprite_store_flags_old, SpriteStoreFlags& sprite_store_flags_new, SpriteResetFlags& sprite_reset_flags, wire BYVA_LINE_RSTn, OamTempB& oam_temp_b_old);
  void get_sprite_match_flags_gates(wire AROR_MATCH_ENp, SpriteMatchFlags& sprite_get_flag);
  void sprite_match_to_bus_gates(SpriteMatchFlags& sprite_get_flag);

  void sprite_scan_to_bus_gates(SpriteDeltaY delta, NorLatch XYMU_RENDERINGn, Gate FEPO_STORE_MATCHp);

  void set_lcd_pins_gates(wire SACU_CLKPIPE_evn);

  SpriteDeltaY sub_sprite_y_gates();

  static void oam_latch_to_temp_a(wire COTA_OAM_CLKn, const OamLatchA& old_oam_latch_a, OamTempA& oam_temp_a);
  static void oam_latch_to_temp_b(wire COTA_OAM_CLKn, const OamLatchB& old_oam_latch_b, OamTempB& oam_temp_b);

  //----------------------------------------

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp() const { return or2(reg.sys_rst.AFER_SYS_RSTp.qp_new(), reg.sys_rst.ASOL_POR_DONEn.qp_new()); }
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn() const { return not1(AVOR_SYS_RSTp()); }
  /*#p01.DULA*/ wire DULA_SYS_RSTp() const { return not1(ALUR_SYS_RSTn()); }
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn() const { return not1(DULA_SYS_RSTp()); }
  /*#p01.XORE*/ wire XORE_SYS_RSTp() const { return not1(CUNU_SYS_RSTn()); }
  /*_p01.XEBE*/ wire XEBE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*#p01.WALU*/ wire WALU_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*_p01.WESY*/ wire WESY_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*_p01.XARE*/ wire XARE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*_p03.MULO*/ wire MULO_SYS_RSTn() const { return not1(ALUR_SYS_RSTn()); }

  /*_p01.XODO*/ wire XODO_VID_RSTp() const { return nand2(XEBE_SYS_RSTn(), reg.reg_lcdc.XONA_LCDC_LCDENn.qn_new()); }
  /*_p01.XAPO*/ wire XAPO_VID_RSTn() const { return not1(XODO_VID_RSTp()); }
  /*_p01.LYHA*/ wire LYHA_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*_p01.LYFE*/ wire LYFE_VID_RSTn() const { return not1(LYHA_VID_RSTp()); }
  /*_p01.TOFU*/ wire TOFU_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*_p01.ROSY*/ wire ROSY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ATAR*/ wire ATAR_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not1(ATAR_VID_RSTp()); }
  /*_p01.PYRY*/ wire PYRY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*_p01.AMYG*/ wire AMYG_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }

  /*_p07.UBET*/ wire UBETp()           const { return not1(reg.sys_rst.PIN_77_T1.qp_int_new()); }
  /*_p07.UVAR*/ wire UVARp()           const { return not1(reg.sys_rst.PIN_76_T2.qp_int_new()); }
  /*_p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and2(reg.sys_rst.PIN_77_T1.qp_int_new(), UVARp()); }
  /*_p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and2(reg.sys_rst.PIN_76_T2.qp_int_new(), UBETp()); }
  /*_p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand3(UBETp(), UVARp(), reg.sys_rst.PIN_71_RST.qp_int_new()); }
  /*_p08.RYCA*/ wire RYCA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /*_p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /*_p08.MULE*/ wire MULE_MODE_DBG1n() const { return not1(UMUT_MODE_DBG1p()); }
  /*_p25.TUTO*/ wire TUTO_VRAM_DBGp()  const { return and2(UNOR_MODE_DBG2p(), reg.sys_rst.SOTO_DBG_VRAMp.qn_new()); }

  /*_p01.UCOB*/ wire UCOB_CLKBADp() const { return not1(reg.sys_clk.PIN_74_CLK.clkgood()); }
  /*_p01.ATEZ*/ wire ATEZ_CLKBADp() const { return not1(reg.sys_clk.PIN_74_CLK.clkgood()); }

  /*_p01.ABOL*/ wire ABOL_CLKREQn() const { return not1(reg.sys_clk.SIG_CPU_CLKREQ.out_new()); }
  /*#p01.BUTY*/ wire BUTY_CLKREQp() const { return not1(ABOL_CLKREQn()); }

  wire gen_clk_old(uint8_t mask) {
    uint8_t phase_mask_old = 1 << (7 - ((sys.phase_total + 0) & 7));
    return !!(phase_mask_old & mask);
  }

  wire gen_clk_new(uint8_t mask) {
    uint8_t phase_mask_new = 1 << (7 - ((sys.phase_total + 1) & 7));
    return !!(phase_mask_new & mask);
  }

  wire AZOF_AxCxExGx() const {
    /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(reg.sys_clk.AVET_DEGLITCH.out_mid());
    /*_p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
    return AZOF_AxCxExGx;
  }

  /*_p01.ZAXY*/ wire ZAXY_xBxDxFxH() const { return not1(AZOF_AxCxExGx()); }
  /*#p01.ZEME*/ wire ZEME_AxCxExGx() const { return not1(ZAXY_xBxDxFxH()); }
  /*_p29.XYVA*/ wire XYVA_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /*_p29.XOTA*/ wire XOTA_AxCxExGx() const { return not1(XYVA_xBxDxFxH()); }
  /*_p29.XYFY*/ wire XYFY_xBxDxFxH() const { return not1(XOTA_AxCxExGx()); }
  /*#p01.ALET*/ wire ALET_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /*_p01.LAPE*/ wire LAPE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MEHE*/ wire MEHE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MYVO*/ wire MYVO_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*_p27.MOXE*/ wire MOXE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*_p27.TAVA*/ wire TAVA_xBxDxFxH() const { return not1(LAPE_AxCxExGx()); }

  /*#p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not1(reg.sys_clk.AFUR_xxxxEFGH.qp_new()); }
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH() const { return not1(reg.sys_clk.ALEF_AxxxxFGH.qn_new()); }
  /*#p01.AROV*/ wire AROV_xxCDEFxx() const { return not1(reg.sys_clk.APUK_ABxxxxGH.qp_new()); }
  /*#p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not1(reg.sys_clk.ADYK_ABCxxxxH.qn_new()); }

  /*#p01.BEKO*/ wire BEKO_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); } // BEKO+BAVY parallel
  /*#p01.BAPY*/ wire BAPY_xxxxxxGH() const { return nor3(ABOL_CLKREQn(), AROV_xxCDEFxx(), ATYP_ABCDxxxx()); }
  /*#p01.BERU*/ wire BERU_ABCDEFxx() const { return not1(BAPY_xxxxxxGH()); }
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH() const { return not1(BERU_ABCDEFxx()); }
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx() const { return not1(BUFA_xxxxxxGH()); }
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH() const { return nand4(BOLO_ABCDEFxx(), BOLO_ABCDEFxx(), BEKO_ABCDxxxx(), BEKO_ABCDxxxx()); }
  /*#p01.BANE*/ wire BANE_ABCDxxxx() const { return not1(BEJA_xxxxEFGH()); }
  /*#p01.BELO*/ wire BELO_xxxxEFGH() const { return not1(BANE_ABCDxxxx()); }
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx() const { return not1(BELO_xxxxEFGH()); }
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH() const { return nand3(AFEP_AxxxxFGH(), ATYP_ABCDxxxx(), BAZE_ABCDxxxx()); }
  /*#p01.BELE*/ wire BELE_Axxxxxxx() const { return not1(BUTO_xBCDEFGH()); }
  /*#p01.BYJU*/ wire BYJU_Axxxxxxx() const { return or2(BELE_Axxxxxxx(), ATEZ_CLKBADp()); }
  /*#p01.BALY*/ wire BALY_xBCDEFGH() const { return not1(BYJU_Axxxxxxx()); }
  /*_p01.BOGA*/ wire BOGA_Axxxxxxx() const { return not1(BALY_xBCDEFGH()); }
  /*#p01.BUVU*/ wire BUVU_Axxxxxxx() const { return and2(BALY_xBCDEFGH(), BUTY_CLKREQp()); }
  /*#p01.BYXO*/ wire BYXO_xBCDEFGH() const { return not1(BUVU_Axxxxxxx()); }
  /*#p01.BEDO*/ wire BEDO_Axxxxxxx() const { return not1(BYXO_xBCDEFGH()); }
  /*#p01.BOWA*/ wire BOWA_xBCDEFGH() const { return not1(BEDO_Axxxxxxx()); }
  /*#p01.BUGO*/ wire BUGO_xBCDExxx() const { return not1(AFEP_AxxxxFGH()); }
  /*#p01.BATE*/ wire BATE_AxxxxxGH() const { return nor3(BUGO_xBCDExxx(), AROV_xxCDEFxx(), ABOL_CLKREQn()); }
  /*#p01.BASU*/ wire BASU_xBCDEFxx() const { return not1(BATE_AxxxxxGH()); }
  /*#p01.BUKE*/ wire BUKE_AxxxxxGH() const { return not1(BASU_xBCDEFxx()); }
  /*#p01.BOMA*/ wire BOMA_xBCDEFGH() const { return not1(BOGA_Axxxxxxx()); }
  /*#p01.BELU*/ wire BELU_xxxxEFGH() const { return nor2(ATYP_ABCDxxxx(), ABOL_CLKREQn()); }
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx() const { return not1(BELU_xxxxEFGH()); }
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH() const { return not1(BYRY_ABCDxxxx()); }
  /*_p01.UVYT*/ wire UVYT_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); }
  /*_p04.MOPA*/ wire MOPA_xxxxEFGH() const { return not1(UVYT_ABCDxxxx()); }
  /*_p28.XYNY*/ wire XYNY_ABCDxxxx() const { return not1(MOPA_xxxxEFGH()); }

  /*#p21.TALU*/ wire TALU_xxCDEFxx() const { return not1(reg.sys_clk.VENA_xxCDEFxx.qn_new()); }
  /*#p29.XUPY*/ wire XUPY_ABxxEFxx() const { return not1(reg.sys_clk.WUVU_ABxxEFxx.qn_new()); }
  /*#p29.XOCE*/ wire XOCE_xBCxxFGx() const { return not1(reg.sys_clk.WOSU_AxxDExxH.qp_new()); }
  /*#p29.WOJO*/ wire WOJO_AxxxExxx() const { return nor2(reg.sys_clk.WOSU_AxxDExxH.qn_new(), reg.sys_clk.WUVU_ABxxEFxx.qn_new()); }
  /*#p21.SONO*/ wire SONO_ABxxxxGH() const { return not1(TALU_xxCDEFxx()); }
  /*_p29.XYSO*/ wire XYSO_xBCDxFGH() const { return not1(WOJO_AxxxExxx()); }
  /*#p30.CYKE*/ wire CYKE_ABxxEFxx() const { return not1(XUPY_ABxxEFxx()); }
  /*#p30.WUDA*/ wire WUDA_xxCDxxGH() const { return not1(CYKE_ABxxEFxx()); }
  /*#p28.AWOH*/ wire AWOH_xxCDxxGH() const { return not1(XUPY_ABxxEFxx()); }
  /*#p01.AJAX*/ wire AJAX_xxxxEFGH() const { return not1(ATYP_ABCDxxxx()); }

  //-----------------------------------------------------------------------------

  /*_p07.AJAS*/ wire AJAS_CPU_RDn      () const { return not1(reg.cpu_signals.TEDO_CPU_RDp.out_new()); }
  /*_p07.DYKY*/ wire DYKY_CPU_WRn      () const { return not1(reg.cpu_signals.TAPU_CPU_WRp.out_new()); }
  /*_p07.ASOT*/ wire ASOT_CPU_RDp      () const { return not1(AJAS_CPU_RDn()); }
  /*_p28.MYNU*/ wire MYNU_CPU_RDn      () const { return nand2(ASOT_CPU_RDp(), CATY_LATCH_EXTp()); }
  /*_p28.LEKO*/ wire LEKO_CPU_RDp      () const { return not1(MYNU_CPU_RDn()); }
  /*_p07.CUPA*/ wire CUPA_CPU_WRp      () const { return not1(DYKY_CPU_WRn()); }
  /*_p08.REDU*/ wire REDU_CPU_RDn      () const { return not1(reg.cpu_signals.TEDO_CPU_RDp.out_new()); }
  /*_p08.MEXO*/ wire MEXO_CPU_WRn      () const { return not1(reg.cpu_signals.APOV_CPU_WRp.out_new()); }

  /*_p04.DECY*/ wire DECY_LATCH_EXTn   () const { return not1(reg.cpu_signals.SIG_IN_CPU_DBUS_FREE.out_new()); }
  /*_p04.CATY*/ wire CATY_LATCH_EXTp   () const { return not1(DECY_LATCH_EXTn()); }
  /*#p28.BOFE*/ wire BOFE_LATCH_EXTn   () const { return not1(CATY_LATCH_EXTp()); }

  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   () const { return and2(reg.cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new(), reg.cpu_abus.TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   () const { return nor2(reg.cpu_abus.SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   () const { return and2(TEFA_ADDR_VRAMp(), reg.cpu_abus.BUS_CPU_A15p.out_new()); }
  /*_p08.LEVO*/ wire LEVO_ADDR_VRAMn   () const { return not1(TEXO_ADDR_VRAMn()); }
  /*_p25.TUJA*/ wire TUJA_CPU_VRAM_WRp () const { return and2(SOSE_ADDR_VRAMp(), reg.cpu_signals.APOV_CPU_WRp.out_new()); }

  wire TOLE_CPU_VRAM_RDp() const
  {
   // Ignoring debug for now
    ///*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
    ///*#p25.TAVY*/ wire TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp = mux2p(TEFY_VRAM_MCSp, TUTO_DBG_VRAMp, TUCA_CPU_VRAM_RDp);

    /*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = nand2(SOSE_ADDR_VRAMp(), reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK.out_new());
    /*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp = not1(TUCA_CPU_VRAM_RDp);

    return TOLE_CPU_VRAM_RDp;
  }

  wire SALE_CPU_VRAM_WRn() const
  {
    // Ignoring debug for now
    ///*#p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_ADDR_VRAMp(), SIG_IN_CPU_WRp.qp_new());  // Schematic wrong, second input is SIG_IN_CPU_WRp
    ///*#p25.TAVY*/ wire TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.SALE*/ wire SALE_CPU_VRAM_WRn = mux2p(TUTO_DBG_VRAMp, TAVY_MOEp, TEGU_CPU_VRAM_WRn);

    /*#p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = and2(SOSE_ADDR_VRAMp(), reg.cpu_signals.SIG_IN_CPU_WRp.out_new());  // Schematic wrong, second input is SIG_IN_CPU_WRp
    /*#p25.SALE*/ wire SALE_CPU_VRAM_WRn = not1(TEGU_CPU_VRAM_WRn);

    return SALE_CPU_VRAM_WRn;
  }

  //-----------------------------------------------------------------------------

  void dump_sys(Dumper& d) const;

  void dump_cpu_bus(Dumper& d);
  void dump_ext_bus(Dumper& d);
  void dump_vram_bus(Dumper& d);
  void dump_oam_bus(Dumper& d);

  void dump_sprite_scanner(Dumper& d);

  void dump_tile_fetcher(Dumper& d);
  void dump_sprite_fetcher(Dumper& d);

  void dump_resets(Dumper& d);

  void dump_timer(Dumper& d);
  void dump_clocks(Dumper& d);
  void dump_interrupts(Dumper& d);
  void dump_joypad(Dumper& d);
  void dump_lcd(Dumper& d);
  void dump_sprite_store(Dumper& d);
  void dump_mbc1(Dumper& d);
  void dump_dma(Dumper& d);
  void dump_serial(Dumper& d);
  void dump_ppu(Dumper& d);
  void dump_spu(Dumper& d);

  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything between sentinel 1 and
  // sentinel 2 _must_ derive from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  GateBoyReg reg;
  GateBoyCpu cpu;
  GateBoyMem mem;
  GateBoySys sys;

  GateBoyState _state;

  uint64_t sentinel2 = SENTINEL2;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
