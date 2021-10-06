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

#pragma pack(push, 1)
struct GateBoyCpu {
  void reset_to_poweron() {
    memset(this, 0, sizeof(*this));
  }

  void reset_to_bootrom() {
    core.reset_to_bootrom();
    bus_req_new.addr = 0x0000;
    bus_req_new.data = 0;
    bus_req_new.read = 0;
    bus_req_new.write = 0;
    cpu_data_latch = 49;
    imask_latch = 0;
    intf_latch = 0;
    intf_latch_delay = 0;
    intf_halt_latch = 0;
  }

  void reset_to_cart() {
    core.reset_to_cart();
    bus_req_new.addr = 0xFF50;
    bus_req_new.data = 1;
    bus_req_new.read = 0;
    bus_req_new.write = 1;
    cpu_data_latch = 1;
    imask_latch = 0;
    intf_latch = 1;
    intf_latch_delay = 0;
    intf_halt_latch = 0;
  }

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
  void reset_to_poweron() {
    memset(this, 0, sizeof(*this));

    // The first thing the bootrom does is clear VRAM, so put some recognizable
    // pattern in vram so we can see it running.
    for (int i = 0; i < 8192; i++) {
      uint32_t h = i * 0x1234567;
      vid_ram[i] = uint8_t(h ^ (h >> 4));
    }

    memset(framebuffer, 4, sizeof(framebuffer));
  }

  void reset_to_bootrom() {
    memset(this, 0, sizeof(*this));

    // The first thing the bootrom does is clear VRAM, so put some recognizable
    // pattern in vram so we can see it running.
    for (int i = 0; i < 8192; i++) {
      uint32_t h = i * 0x1234567;
      vid_ram[i] = uint8_t(h ^ (h >> 4));
    }

    memset(framebuffer, 4, sizeof(framebuffer));
  }

  void reset_to_cart() {
    memset(this, 0, sizeof(*this));

    memcpy(vid_ram, vram_boot, 8192);
    zero_ram[0x7A] = 0x39;
    zero_ram[0x7B] = 0x01;
    zero_ram[0x7C] = 0x2E;
    memcpy(framebuffer, framebuffer_boot, 160*144);
  }

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
  void reset_to_poweron() {
    memset(this, 0, sizeof(*this));
  }

  void reset_to_bootrom() {
    rst = false;
    t1 = false;
    t2 = false;
    clk_en = true;
    clk_good = true;
    clk_req = true;
    cpu_en = true;
    fastboot = true;
    buttons = 0;
    gb_phase_total = 80;
  }

  void reset_to_cart() {
    rst = false;
    t1 = false;
    t2 = false;
    clk_en = true;
    clk_good = true;
    clk_req = true;
    cpu_en = true;
    fastboot = true;
    buttons = 0;
    gb_phase_total = 46880720;
  }

  // External signals
  uint8_t  rst = 0;
  uint8_t  t1 = 0;
  uint8_t  t2 = 0;
  uint8_t  clk_en = 0;
  uint8_t  clk_good = 0;
  uint8_t  clk_req = 0;
  uint8_t  cpu_en = 0;
  uint8_t  fastboot = 0;
  uint8_t  buttons = 0;
  uint64_t gb_phase_total = 0;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

struct GateBoy  : public IGateBoy {
  GateBoy() {}

  virtual ~GateBoy() {}

  GateBoy* clone() const override {
    GateBoy* gb2 = new GateBoy();
    gb2->gb_state = gb_state;
    gb2->cpu = cpu;
    gb2->mem = mem;
    gb2->sys = sys;
    gb2->pins = pins;
    gb2->probes = probes;
    return gb2;
  }

  int size_bytes() override {
    return sizeof(GateBoy);
  }

  GBResult get_flags() const override {
    return GBResult(BIT_NEW | BIT_OLD | BIT_DRIVEN | BIT_PULLED | BIT_CLOCK | BIT_DATA);
  }

  //----------------------------------------

  GBResult load_raw_dump(BlobStream& bs) override {
    bool read_ok = true;
    read_ok &= bs.read(gb_state);
    read_ok &= bs.read(cpu);
    read_ok &= bs.read(mem);
    read_ok &= bs.read(sys);
    read_ok &= bs.read(pins);
    read_ok &= bs.read(probes);

    probes.reset_to_bootrom();

    return read_ok ? GBResult::ok() : Error::CORRUPT;
  }

  GBResult save_raw_dump(BlobStream& bs) const override {
    bool write_ok = true;
    write_ok &= bs.write(gb_state);
    write_ok &= bs.write(cpu);
    write_ok &= bs.write(mem);
    write_ok &= bs.write(sys);
    write_ok &= bs.write(pins);
    write_ok &= bs.write(probes);
    return write_ok ? GBResult::ok() : Error::CORRUPT;;
  }

  GBResult reset_to_poweron(const blob& cart_blob) override;
  GBResult run_poweron_reset(const blob& cart_blob, bool fastboot) override;
  GBResult reset_to_bootrom(const blob& cart_blob) override;
  GBResult reset_to_cart(const blob& cart_blob) override;

  GBResult peek(int addr) const override;
  GBResult poke(int addr, uint8_t data_in) override;

  GBResult dbg_req  (uint16_t addr, uint8_t data, bool write) override;
  GBResult dbg_read (const blob& cart_blob, int addr) override;
  GBResult dbg_write(const blob& cart_blob, int addr, uint8_t data) override;

  GBResult run_phases(const blob& cart_blob, int phase_count) override;
  GBResult next_phase(const blob& cart_blob) override;

  GBResult set_buttons(uint8_t buttons) override { sys.buttons = buttons; return GBResult::ok(); }
 
  GBResult set_cpu_en(bool enabled) override {
    sys.cpu_en = enabled;
    return GBResult::ok();
  };


  const GateBoyCpu&   get_cpu() const override    { return cpu; }
  const GateBoyMem&   get_mem() const override    { return mem; }
  const GateBoyState& get_state() const override  { return gb_state; }
  const GateBoySys&   get_sys() const override    { return sys; }
  const GateBoyPins&  get_pins() const override   { return pins; }
  const Probes&       get_probes() const override { return probes; }

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
  void tock_lcd_gates(const GateBoyState& reg_old);
  void tock_dma_gates(const GateBoyState& reg_old);
  void tock_joypad_gates(const GateBoyState& reg_old);
  void tock_interrupts_gates(const GateBoyState& reg_old);
  void tock_clocks_gates(const GateBoyState& reg_old);
  void tock_vid_clocks_gates(const GateBoyState& reg_old);
  void tock_div_gates(const GateBoyState& reg_old);
  void tock_timer_gates(const GateBoyState& reg_old);
  void tock_reset_gates(const GateBoyState& reg_old, DFF17 UPOF_DIV15p);
  void tock_ext_gates(const blob& cart_blob);
  void tock_oam_bus_gates(const GateBoyState& reg_old);
  void tock_serial_gates(const GateBoyState& reg_old);
  void tock_vram_bus_gates(const GateBoyState& reg_old, wire TEVO_WIN_FETCH_TRIGp);
  void tock_zram_gates(const GateBoyState& reg_old);
  void tock_pix_pipes_gates(const GateBoyState& reg_old, wire SACU_CLKPIPE_evn, wire NYXU_BFETCH_RSTn);
  void tock_bootrom_gates(const GateBoyState& reg_old);
  void tock_window_gates(const GateBoyState& reg_old, wire SEGU_CLKPIPE_evn, wire REPU_VBLANKp);
  void tock_spu_gates(const GateBoyState& reg_old);

  void tock_latches();

  void update_sprite_store_flags_gates(SpriteCounter& sprite_counter, wire DYTY_COUNT_CLKp, SpriteStoreFlags& sprite_store_flags);
  void store_sprite_gates(SpriteStoreFlags& sprite_store_flags_old, SpriteStoreFlags& sprite_store_flags_new, SpriteResetFlags& sprite_reset_flags, wire BYVA_LINE_RSTn, OamTempB& oam_temp_b_old);
  void get_sprite_match_flags_gates(wire AROR_MATCH_ENp, SpriteMatchFlags& sprite_get_flag);
  void sprite_match_to_bus_gates(SpriteMatchFlags& sprite_get_flag);
  void sprite_scan_to_bus_gates(SpriteDeltaY delta, NorLatch XYMU_RENDERINGn, Gate FEPO_STORE_MATCHp);
  void set_lcd_pins_gates(wire SACU_CLKPIPE_evn);
  SpriteDeltaY sub_sprite_y_gates();
  static void oam_latch_to_temp_a_gates(wire COTA_OAM_CLKn, const OamLatchA& old_oam_latch_a, OamTempA& oam_temp_a);
  static void oam_latch_to_temp_b_gates(wire COTA_OAM_CLKn, const OamLatchB& old_oam_latch_b, OamTempB& oam_temp_b);

  //----------------------------------------

  wire BYJU_Axxxxxxx() const;
  wire BALY_xBCDEFGH() const;
  wire BOGA_Axxxxxxx() const;
  wire BUVU_Axxxxxxx() const;
  wire BYXO_xBCDEFGH() const;
  wire BEDO_Axxxxxxx() const;
  wire BOWA_xBCDEFGH() const;
  wire BOMA_xBCDEFGH() const;

  wire XODO_VID_RSTp() const;
  wire XAPO_VID_RSTn() const;
  wire LYHA_VID_RSTp() const;
  wire LYFE_VID_RSTn() const;
  wire TOFU_VID_RSTp() const;
  wire ROSY_VID_RSTp() const;
  wire ATAR_VID_RSTp() const;
  wire ABEZ_VID_RSTn() const;
  wire PYRY_VID_RSTp() const;
  wire AMYG_VID_RSTp() const;
  wire TEXO_ADDR_VRAMn   () const;
  wire TEFA_ADDR_VRAMp   () const;
  wire SOSE_ADDR_VRAMp   () const;
  wire LEVO_ADDR_VRAMn   () const;
  wire TUJA_CPU_VRAM_WRp () const;

  wire TOLE_CPU_VRAM_RDp() const;
  wire SALE_CPU_VRAM_WRn() const;

  //-----------------------------------------------------------------------------

  GateBoyState gb_state;
  GateBoyCpu   cpu;
  GateBoyMem   mem;
  GateBoySys   sys;
  GateBoyPins  pins;
  Probes       probes;
};

//-----------------------------------------------------------------------------
