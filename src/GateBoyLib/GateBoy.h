#pragma once

#include "CoreLib/File.h"
#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Log.h"

#include "GateBoyLib/IGateBoy.h"
#include "GateBoyLib/Probe.h"
#include "GateBoyLib/GateBoyState.h"
#include "GateBoyLib/LogicBoyState.h"

#include <atomic>
#include <cstring>

uint32_t cart_rom_addr_mask(const blob& cart_blob);
uint32_t cart_ram_addr_mask(const blob & cart_blob);
bool cart_has_mbc1(const blob & cart_blob);
bool cart_has_ram(const blob & cart_blob);

struct GateBoy;

//-----------------------------------------------------------------------------

struct GateBoyCpu {

  void wipe() {
    memset(this, 0, sizeof(*this));
  }

  MetroBoyCPU core;
  Req      bus_req_new = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;
  uint8_t  intf_latch = 0;
  uint8_t  intf_latch_delay = 0;
  uint8_t  intf_halt_latch = 0;
};

//-----------------------------------------------------------------------------

struct GateBoyMem {
  void wipe() {
    memset(this, 0, sizeof(*this));
  }

  uint8_t vid_ram [8192];
  uint8_t cart_ram[32768];
  uint8_t int_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];
  uint8_t framebuffer[160*144];
};

//-----------------------------------------------------------------------------

struct GateBoySys {
  void wipe() {
    memset(this, 0, sizeof(*this));
  }

  // External signals
  uint8_t rst = 0;
  uint8_t t1 = 0;
  uint8_t t2 = 0;
  uint8_t clk_en = 0;
  uint8_t clk_good = 0;
  uint8_t clk_req = 0;
  uint8_t cpu_en = 0;
  uint8_t fastboot = 0;
  uint8_t buttons = 0;

  // Debug stuff
  uint64_t phase_total = 0;
  double   sim_time = 0;
};

//-----------------------------------------------------------------------------

struct GateBoy  : public IGateBoy {
  virtual ~GateBoy() {}

  //----------------------------------------

  void reset_to_bootrom(const blob& cart_blob, bool fastboot) override;
  void reset_to_cart(const blob& cart_blob) override;

  Result<uint8_t, Error> peek(const blob& cart_blob, int addr) const override;
  Result<uint8_t, Error> poke(blob& cart_blob, int addr, uint8_t data_in) override;

  Result<uint8_t, Error> dbg_read (const blob& cart_blob, int addr) override;
  Result<uint8_t, Error> dbg_write(const blob& cart_blob, int addr, uint8_t data) override;

  void run_phases(const blob& cart_blob, int phase_count) override;
  bool next_phase(const blob& cart_blob) override;

  virtual void set_buttons(uint8_t buttons) { sys.buttons = buttons; }

  const uint8_t* get_framebuffer() const { return mem.framebuffer; }
  const uint8_t* get_vram() const { return mem.vid_ram; }
  void get_state(blob& state_out) const {
    state_out.resize(sizeof(gb_state));
    memcpy(state_out.data(), &gb_state, sizeof(gb_state));
  }

  void load_raw_dump(const blob& dump_in) override {
    CHECK_P(dump_in.size() >= sizeof(GateBoy));
    memcpy(this, dump_in.data(), sizeof(GateBoy));
    CHECK_P(sentinel1 == SENTINEL1);
    CHECK_P(sentinel2 == SENTINEL2);
  }

  void save_raw_dump(blob& dump_out) const override {
    uint8_t* bytes = (uint8_t*)this;
    dump_out.insert(dump_out.end(), bytes, bytes + sizeof(*this));
  }

  const FieldInfo* get_field_info() const override {
    return GateBoyState::fields;
  }

  //----------------------------------------

  static bool check_sentinel(const blob& b) {
    if (b.size() < sizeof(GateBoy)) return false;

    GateBoy* gb = (GateBoy*)b.data();
    if (gb->sentinel1 != SENTINEL1) return false;
    if (gb->sentinel2 != SENTINEL2) return false;
    return true;
  }

  //----------------------------------------

  void set_boot_bit(const blob& cart_blob) {
    dbg_write(cart_blob, 0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  void tock_cpu();
  void tock_gates(const blob& cart_blob);
  void update_framebuffer();

  //-----------------------------------------------------------------------------

  //void tock_spu_logic();
  //void tock_serial_logic(bool cpu_wr_old, bool cpu_wr_new, uint16_t cpu_addr_old, uint16_t cpu_addr_new, uint16_t div_old, uint16_t div_new);

  void tock_lcdc_gates(const GateBoyState& reg_old);
  void tock_lyc_gates(const GateBoyState& reg_old);
  void tock_lcd_gates();
  void tock_dma_gates(const GateBoyState& reg_old);
  void tock_joypad_gates(const GateBoyState& reg_old);
  void tock_interrupts_gates(const GateBoyState& reg_old);
  void tock_clocks_gates();
  void tock_vid_clocks_gates();
  void tock_div_gates();
  void tock_timer_gates(const GateBoyState& reg_old);
  void tock_reset_gates(DFF17 UPOF_DIV15p);
  void tock_ext_gates(const blob& cart_blob);
  void tock_oam_bus_gates();
  void tock_serial_gates();
  void tock_vram_bus_gates(const GateBoyState& reg_old, wire TEVO_WIN_FETCH_TRIGp);
  void tock_zram_gates(const GateBoyState& reg_old);
  void tock_pix_pipes_gates(const GateBoyState& reg_old, wire SACU_CLKPIPE_evn, wire NYXU_BFETCH_RSTn);
  void tock_bootrom_gates();
  void tock_window_gates(const GateBoyState& reg_old, wire SEGU_CLKPIPE_evn, wire REPU_VBLANKp);
  void update_sprite_store_flags_gates(SpriteCounter& sprite_counter, wire DYTY_COUNT_CLKp, SpriteStoreFlags& sprite_store_flags);
  void tock_spu_gates();
  void store_sprite_gates(SpriteStoreFlags& sprite_store_flags_old, SpriteStoreFlags& sprite_store_flags_new, SpriteResetFlags& sprite_reset_flags, wire BYVA_LINE_RSTn, OamTempB& oam_temp_b_old);
  void get_sprite_match_flags_gates(wire AROR_MATCH_ENp, SpriteMatchFlags& sprite_get_flag);
  void sprite_match_to_bus_gates(SpriteMatchFlags& sprite_get_flag);
  void sprite_scan_to_bus_gates(SpriteDeltaY delta, NorLatch XYMU_RENDERINGn, Gate FEPO_STORE_MATCHp);
  void set_lcd_pins_gates(wire SACU_CLKPIPE_evn);
  SpriteDeltaY sub_sprite_y_gates();
  static void oam_latch_to_temp_a_gates(wire COTA_OAM_CLKn, const OamLatchA& old_oam_latch_a, OamTempA& oam_temp_a);
  static void oam_latch_to_temp_b_gates(wire COTA_OAM_CLKn, const OamLatchB& old_oam_latch_b, OamTempB& oam_temp_b);

  //----------------------------------------

  /*_p01.XODO*/ wire XODO_VID_RSTp() const { return nand2(gb_state.sys_rst.XEBE_SYS_RSTn(), gb_state.reg_lcdc.XONA_LCDC_LCDENn.qn_new()); }
  /*_p01.XAPO*/ wire XAPO_VID_RSTn() const { return not1(XODO_VID_RSTp()); }
  /*_p01.LYHA*/ wire LYHA_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*_p01.LYFE*/ wire LYFE_VID_RSTn() const { return not1(LYHA_VID_RSTp()); }
  /*_p01.TOFU*/ wire TOFU_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*_p01.ROSY*/ wire ROSY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ATAR*/ wire ATAR_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not1(ATAR_VID_RSTp()); }
  /*_p01.PYRY*/ wire PYRY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*_p01.AMYG*/ wire AMYG_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p08.TEXO*/ wire TEXO_ADDR_VRAMn   () const { return and2(gb_state.cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new(), gb_state.cpu_abus.TEVY_ADDR_VRAMn()); }
  /*#p25.TEFA*/ wire TEFA_ADDR_VRAMp   () const { return nor2(gb_state.cpu_abus.SYRO_FE00_FFFF(), TEXO_ADDR_VRAMn()); }
  /*#p25.SOSE*/ wire SOSE_ADDR_VRAMp   () const { return and2(TEFA_ADDR_VRAMp(), gb_state.cpu_abus.BUS_CPU_A15p.out_new()); }
  /*_p08.LEVO*/ wire LEVO_ADDR_VRAMn   () const { return not1(TEXO_ADDR_VRAMn()); }
  /*_p25.TUJA*/ wire TUJA_CPU_VRAM_WRp () const { return and2(SOSE_ADDR_VRAMp(), gb_state.cpu_signals.APOV_CPU_WRp.out_new()); }

  wire TOLE_CPU_VRAM_RDp() const
  {
   // Ignoring debug for now
    ///*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp =  and2(SOSE_ADDR_VRAMp(), ABUZ_EXT_RAM_CS_CLK);
    ///*#p25.TAVY*/ wire TAVY_MOEp         = not1(vram_bus.PIN_45_VRAM_OEn.qn_new());
    ///*#p25.TEFY*/ wire TEFY_VRAM_MCSp    = not1(vram_bus.PIN_43_VRAM_CSn.qn_new());
    ///*#p25.TOLE*/ wire TOLE_CPU_VRAM_RDp = mux2p(TEFY_VRAM_MCSp, TUTO_DBG_VRAMp, TUCA_CPU_VRAM_RDp);

    /*#p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = nand2(SOSE_ADDR_VRAMp(), gb_state.cpu_signals.ABUZ_EXT_RAM_CS_CLK.out_new());
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

    /*#p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = and2(SOSE_ADDR_VRAMp(), gb_state.cpu_signals.SIG_IN_CPU_WRp.out_new());  // Schematic wrong, second input is SIG_IN_CPU_WRp
    /*#p25.SALE*/ wire SALE_CPU_VRAM_WRn = not1(TEGU_CPU_VRAM_WRn);

    return SALE_CPU_VRAM_WRn;
  }

  //-----------------------------------------------------------------------------

  uint64_t sentinel1 = SENTINEL1;

  GateBoyState gb_state;
  GateBoyCpu cpu;
  GateBoyMem mem;
  GateBoySys sys;
  Probes probes;

  uint64_t sentinel2 = SENTINEL2;
};

//-----------------------------------------------------------------------------
