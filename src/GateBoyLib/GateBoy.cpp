#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"

//-----------------------------------------------------------------------------

void GateBoy::reset_to_bootrom(const blob& cart_blob, bool fastboot)
{
  memset(this, 0, sizeof(*this));

  for (auto c = reg_begin(); c != reg_end(); c++) {
    *c = 0b00011000;
  }

  new_bus.reset_to_bootrom();
  sprite_bus.reset_to_bootrom();

  dma.NAFA_DMA_A08n.state = 0b00011010;
  dma.PYNE_DMA_A09n.state = 0b00011010;
  dma.PARA_DMA_A10n.state = 0b00011010;
  dma.NYDO_DMA_A11n.state = 0b00011010;
  dma.NYGY_DMA_A12n.state = 0b00011010;
  dma.PULA_DMA_A13n.state = 0b00011010;
  dma.POKU_DMA_A14n.state = 0b00011010;
  dma.MARU_DMA_A15n.state = 0b00011010;

  reg_bgp.PAVO_BGP_D0n.state = 0b00011010;
  reg_bgp.NUSY_BGP_D1n.state = 0b00011010;
  reg_bgp.PYLU_BGP_D2n.state = 0b00011010;
  reg_bgp.MAXY_BGP_D3n.state = 0b00011010;
  reg_bgp.MUKE_BGP_D4n.state = 0b00011010;
  reg_bgp.MORU_BGP_D5n.state = 0b00011010;
  reg_bgp.MOGY_BGP_D6n.state = 0b00011010;
  reg_bgp.MENA_BGP_D7n.state = 0b00011010;

  reg_obp0.XUFU_OBP0_D0n.state = 0b00011010;
  reg_obp0.XUKY_OBP0_D1n.state = 0b00011010;
  reg_obp0.XOVA_OBP0_D2n.state = 0b00011010;
  reg_obp0.XALO_OBP0_D3n.state = 0b00011010;
  reg_obp0.XERU_OBP0_D4n.state = 0b00011010;
  reg_obp0.XYZE_OBP0_D5n.state = 0b00011010;
  reg_obp0.XUPO_OBP0_D6n.state = 0b00011010;
  reg_obp0.XANA_OBP0_D7n.state = 0b00011010;

  reg_obp1.MOXY_OBP1_D0n.state = 0b00011010;
  reg_obp1.LAWO_OBP1_D1n.state = 0b00011010;
  reg_obp1.MOSA_OBP1_D2n.state = 0b00011010;
  reg_obp1.LOSE_OBP1_D3n.state = 0b00011010;
  reg_obp1.LUNE_OBP1_D4n.state = 0b00011010;
  reg_obp1.LUGU_OBP1_D5n.state = 0b00011010;
  reg_obp1.LEPU_OBP1_D6n.state = 0b00011010;
  reg_obp1.LUXO_OBP1_D7n.state = 0b00011010;

  joy.reset_to_bootrom();

  sentinel1 = SENTINEL1;
  sentinel2 = SENTINEL2;
  sentinel3 = SENTINEL3;
  sentinel4 = SENTINEL4;

  check_state_old_and_driven_or_pulled();

  sys_fastboot = fastboot;

  //----------------------------------------
  // Update the sim without ticking the clock to to settle initial reset signals.

  sys_rst = 1;

  tock_slow(cart_blob, 0);
  commit_and_hash();

  //----------------------------------------
  // Release reset, start clock, and sync with phase

  sys_rst = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  run_phases(cart_blob, 2);

  CHECK_N(bit(clk.AFUR_xxxxEFGH.qp_old()));
  CHECK_P(bit(clk.ALEF_AxxxxFGH.qp_old()));
  CHECK_P(bit(clk.APUK_ABxxxxGH.qp_old()));
  CHECK_P(bit(clk.ADYK_ABCxxxxH.qp_old()));

  phase_total = 0;
  phase_origin = 46880720;
;

  //----------------------------------------
  // Wait for SIG_CPU_START

  while(bit(~rst.SIG_CPU_STARTp.out_old())) {
    run_phases(cart_blob, 8);
  }

  //----------------------------------------
  // Delay to sync up with expected div value

  run_phases(cart_blob, 16);

  //----------------------------------------
  // Fetch the first instruction in the bootrom

  dbg_read(cart_blob, 0x0000);

  //----------------------------------------
  // We're ready to go, release the CPU so it can start running the bootrom.

  sys_clkreq = 1;
  sys_cpu_en = true;

  if (fastboot) {
    div.TERO_DIV03p.state = 0b00011010;
    div.UNYK_DIV04p.state = 0b00011010;
    div.UPOF_DIV15p.state = 0b00011011;
  }

  memset(framebuffer, 4, sizeof(framebuffer));

  probes.reset_to_cart();
}

//-----------------------------------------------------------------------------

void GateBoy::reset_to_cart(const blob& cart_blob) {
  reset_to_bootrom(cart_blob, true);

  VOGA_HBLANKp.state = 0b00011001;

  old_bus.reset_to_cart_new();
  new_bus.reset_to_cart_new();


  vram_pins.reset_to_cart();
  sprite_bus.reset_to_cart();

  cpu_signals.reset_to_cart();
  oam_bus.reset_to_cart();
  ext_pins.reset_to_cart();
  vram_bus.reset_to_cart();

  oam_latch_a.reset_to_cart();
  oam_latch_b.reset_to_cart();
  oam_temp_a.reset_to_cart();
  oam_temp_b.reset_to_cart();

  ext_addr_latch.reset_to_cart();
  ext_data_latch.reset_to_cart();

  rst.reset_to_cart();
  clk.reset_to_cart();
  div.reset_to_cart();
  interrupts.reset_to_cart();
  serial.reset_to_cart();
  reset_sprite_store();
  sprite_scanner.reset_to_cart();
  sprite_fetcher.reset_to_cart();
  reg_stat.reset_to_cart();
  pix_count.reset_to_cart();
  pix_pipes.reset_to_cart();
  dma.reset_to_cart();
  reg_bgp.reset_to_cart();
  reg_obp0.reset_to_cart();
  reg_obp1.reset_to_cart();
  joy.reset_to_cart();

  reg_lcdc.reset_to_cart();
  lcd.reset_to_cart();

  reg_lx.reset_to_cart();
  reg_ly.reset_to_cart();
  reg_lyc.reset_to_cart();

  WODU_HBLANKp.state = 0b00011001;

  sprite_scanner.FETO_SCAN_DONEp.state = 0b00011001;
  ATEJ_LINE_RSTp.state = 0b00011000;

  reg_NR50.reset_to_cart();
  reg_NR51.reset_to_cart();
  reg_NR52.reset_to_cart();

  check_state_old_and_driven_or_pulled();

  line_phase_x = 98 * 8 + 6;

  sys_rst = false;
  sys_t1 = false;
  sys_t2 = false;
  sys_clken = true;
  sys_clkgood = true;
  sys_clkreq = true;
  sys_fastboot = true;

  gb_cpu.reset_to_cart();

  bus_req_new.addr = 0xFF50;
  bus_req_new.data = 1;
  bus_req_new.read = 0;
  bus_req_new.write = 1;

  cpu_data_latch = 1;
  intf_latch = 1;
  intf_latch_delay = 0;
  intf_halt_latch = 0;

  memcpy(vid_ram, vram_boot, 8192);

  zero_ram[0x7A] = 0x39;
  zero_ram[0x7B] = 0x01;
  zero_ram[0x7C] = 0x2E;

  memcpy(framebuffer, framebuffer_boot, 160*144);

  sim_time = 169.62587129999756;
  //phase_total = 46880720;
  //phase_origin = 46880720;
  phase_total = 0;
  phase_origin = 0;
  phase_hash = 0xad3faa49f49984b4;
  cumulative_hash = 0x6e58a5c197ff4af1;

  probes.reset_to_cart();
}

//------------------------------------------------------------------------------------------------------------------------

uint8_t GateBoy::dbg_read(const blob& cart_blob, int addr) {
  CHECK_P((phase_total & 7) == 0);

  Req old_req = bus_req_new;
  bool old_cpu_en = sys_cpu_en;
  sys_cpu_en = false;

  bus_req_new.addr = uint16_t(addr);
  bus_req_new.data = 0;
  bus_req_new.read = 1;
  bus_req_new.write = 0;
  run_phases(cart_blob, 8);

  bus_req_new = old_req;
  sys_cpu_en = old_cpu_en;

  return cpu_data_latch;
}

//------------------------------------------------------------------------------

void GateBoy::dbg_write(const blob& cart_blob, int addr, uint8_t data) {
  CHECK_P((phase_total & 7) == 0);

  Req old_req = bus_req_new;
  bool old_cpu_en = sys_cpu_en;
  sys_cpu_en = false;

  bus_req_new.addr = uint16_t(addr);
  bus_req_new.data = data;
  bus_req_new.read = 0;
  bus_req_new.write = 1;

  next_phase(cart_blob);
  next_phase(cart_blob);
  next_phase(cart_blob);
  next_phase(cart_blob);
  next_phase(cart_blob);
  next_phase(cart_blob);
  next_phase(cart_blob);
  next_phase(cart_blob);

  bus_req_new = old_req;
  sys_cpu_en = old_cpu_en;
}

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyOffsets {
  const int o_old_bus        = offsetof(GateBoy, old_bus);
  const int o_new_bus        = offsetof(GateBoy, new_bus);
  const int o_vram_bus       = offsetof(GateBoy, vram_bus);
  const int o_sprite_bus     = offsetof(GateBoy, sprite_bus);
  const int o_oam_bus        = offsetof(GateBoy, oam_bus);

  const int o_cpu_bus        = offsetof(GateBoy, cpu_signals);
  const int o_ext_bus        = offsetof(GateBoy, ext_pins);
  const int o_vram_pins      = offsetof(GateBoy, vram_pins);
  const int o_oam            = offsetof(GateBoy, oam);
  const int o_zram_bus       = offsetof(GateBoy, zram_bus);

  const int o_oam_latch_a    = offsetof(GateBoy, oam_latch_a);
  const int o_oam_latch_b    = offsetof(GateBoy, oam_latch_b);
  const int o_oam_temp_a     = offsetof(GateBoy, oam_temp_a );
  const int o_oam_temp_b     = offsetof(GateBoy, oam_temp_b );
  const int o_ext_data_latch = offsetof(GateBoy, ext_data_latch);
  const int o_ext_addr_latch = offsetof(GateBoy, ext_addr_latch);
  const int o_rst_reg        = offsetof(GateBoy, rst);
  const int o_clk_reg        = offsetof(GateBoy, clk);
  const int o_div_reg        = offsetof(GateBoy, div);
  const int o_tim_reg        = offsetof(GateBoy, timer);
  const int o_dma_reg        = offsetof(GateBoy, dma);
  const int o_int_reg        = offsetof(GateBoy, interrupts);
  const int o_joypad         = offsetof(GateBoy, joy);
  const int o_ser_reg        = offsetof(GateBoy, serial);

  const int o_sprite_store   = offsetof(GateBoy, sprite_store);
  const int o_sprite_counter = offsetof(GateBoy, sprite_counter);
  const int o_sprite_scanner = offsetof(GateBoy, sprite_scanner);
  const int o_sprite_fetcher = offsetof(GateBoy, sprite_fetcher);
  const int o_sprite_pix_a   = offsetof(GateBoy, sprite_pix_a);
  const int o_sprite_pix_b   = offsetof(GateBoy, sprite_pix_b);

  const int o_tile_fetcher   = offsetof(GateBoy, tile_fetcher);
  const int o_tile_temp_a    = offsetof(GateBoy, tile_temp_a);
  const int o_tile_temp_b    = offsetof(GateBoy, tile_temp_b);

  const int o_reg_lcdc       = offsetof(GateBoy, reg_lcdc);
  const int o_reg_stat       = offsetof(GateBoy, reg_stat);
  const int o_reg_scx        = offsetof(GateBoy, reg_scx );
  const int o_reg_scy        = offsetof(GateBoy, reg_scy );
  const int o_reg_wy         = offsetof(GateBoy, reg_wy  );
  const int o_reg_wx         = offsetof(GateBoy, reg_wx  );

  const int o_win_coords     = offsetof(GateBoy, win_coords);
  const int o_win_reg        = offsetof(GateBoy, win_reg    );
  const int o_fine_scroll    = offsetof(GateBoy, fine_scroll);

  const int o_pix_count      = offsetof(GateBoy, pix_count);
  const int o_pix_pipes      = offsetof(GateBoy, pix_pipes);
  const int o_lcd            = offsetof(GateBoy, lcd      );
  const int o_reg_lx         = offsetof(GateBoy, reg_lx   );
  const int o_reg_ly         = offsetof(GateBoy, reg_ly   );
  const int o_reg_lyc        = offsetof(GateBoy, reg_lyc  );
  const int o_reg_bgp        = offsetof(GateBoy, reg_bgp  );
  const int o_reg_obp0       = offsetof(GateBoy, reg_obp0 );
  const int o_reg_obp1       = offsetof(GateBoy, reg_obp1 );

  const int o_reg_NR50       = offsetof(GateBoy, reg_NR50 );
  const int o_reg_NR51       = offsetof(GateBoy, reg_NR51 );
  const int o_reg_NR52       = offsetof(GateBoy, reg_NR52 );

  const int o_vid_ram        = offsetof(GateBoy, vid_ram );
  const int o_cart_ram       = offsetof(GateBoy, cart_ram);
  const int o_int_ram        = offsetof(GateBoy, int_ram );
  const int o_oam_ram        = offsetof(GateBoy, oam_ram );
  const int o_zero_ram       = offsetof(GateBoy, zero_ram);

} gb_offsets;

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::next_phase(const blob& cart_blob) {

  probes.begin_pass((phase_total + 1) & 7);

#ifdef YES_LOGIC_VS_GATES

  GateBoy& gb1 = *this;

  static GateBoy gb2;
  memcpy(&gb2, &gb1, sizeof(GateBoy));

  gb1.logic_mode = false;
  gb2.logic_mode = true;

  gb1.tock_slow(cart_blob, 0);
  gb2.tock_slow(cart_blob, 0);

  uint64_t gb1_hash = gb1.commit_and_hash();
  uint64_t gb2_hash = gb2.commit_and_hash();

  // GateBoy and LogicBoy are allowed to diverge in the POR phase, so only check for a match if sys_clkreq is asserted
  // (meaning we're no longer in POR.

  if (sys_clkreq && (gb1_hash != gb2_hash)) {
    LOG_R("Logic mode and gates mode mismatch!\n");
    ASSERT_P(false);
  }

  uint64_t hash_new = gb1_hash;

#else

  tock_slow(cart_blob, 0);

#ifdef YES_HASH
  uint64_t hash_old = commit_and_hash();

  static GateBoy gb1;
  memcpy(&gb1, this, sizeof(GateBoy));

  tock_slow(cart_blob, 1);
  auto& gb2 = *this;
#endif

  uint64_t hash_new = commit_and_hash();

#ifdef YES_HASH
  if (hash_old != hash_new) {
    LOG_Y("Sim not stable after second pass!\n");

    int start = offsetof(GateBoy, sentinel1) + sizeof(sentinel1);
    int end   = offsetof(GateBoy, sentinel2);

    uint8_t* blob_old = (uint8_t*)&gb1;
    uint8_t* blob_new = (uint8_t*)&gb2;

    for (int i = start; i < end; i++) {
      if (blob_old[i] != blob_new[i]) {
        LOG_R("%06lld %04d %02d %02d\n", phase_total, i, blob_old[i], blob_new[i]);
      }
    }

    LOG_R("\n");
  }
#endif

#endif

  //----------------------------------------

  line_phase_x++;
  if (line_phase_x == 912) line_phase_x = 0;
  if (bit(reg_lcdc.XONA_LCDC_LCDENn.qp_old())) line_phase_x = 4;

  {
    int x1 = reg_lx.get_old();
    int x2 = line_phase_x >> 3;

    if (line_phase_x >= 908) x2 = 0;

    if (x1 != x2) {
      LOG_R("*** %03d %03d %03d %llu\n", x1, x2, line_phase_x, phase_total);
    }
  }

  probes.end_pass();
  phase_total++;
  phase_hash = hash_new;
  combine_hash(cumulative_hash, phase_hash);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_slow(const blob& cart_blob, int pass_index) {
  phase_mask_old = 1 << (7 - ((phase_total + 0) & 7));
  phase_mask_new = 1 << (7 - ((phase_total + 1) & 7));

  (void)pass_index;

  wire EXT_vcc = 1;
  wire EXT_gnd = 0;

  /*_SIG_VCC*/ SIG_VCC.sig_in(EXT_vcc);
  /*_SIG_GND*/ SIG_GND.sig_in(EXT_gnd);

  if (pass_index == 0) {
    cpu_data_latch &= (uint8_t)pack_old(8, (BitBase*)&new_bus.BUS_CPU_D00p);
    imask_latch = (uint8_t)pack_old(5, &interrupts.IE_D0);

    if (DELTA_HA) {
      if (gb_cpu.op == 0x76 && (imask_latch & intf_halt_latch)) gb_cpu.state_ = 0;
      intf_halt_latch = 0;
    }

    // +ha -ab -bc -cd -de -ef -fg -gh
    if (DELTA_HA) {
      // this one latches funny, some hardware bug
      if (bit(interrupts.NYBO_FF0F_D2p.qp_old())) intf_halt_latch |= INT_TIMER_MASK;
    }

    // -ha +ab -bc
    if (DELTA_AB) {
      if (sys_cpu_en) {
        gb_cpu.tock_ab(imask_latch, intf_latch, cpu_data_latch);
      }
    }

    if (DELTA_AB) {
      if (sys_cpu_en) {
        bus_req_new.addr = gb_cpu._bus_addr;
        bus_req_new.data = gb_cpu._bus_data;
        bus_req_new.read = gb_cpu._bus_read;
        bus_req_new.write = gb_cpu._bus_write;
      }
    }

    // -bc +cd +de -ef -fg -gh -ha -ab
    if (DELTA_DE) {
      if (bit(interrupts.LOPE_FF0F_D0p.qp_old())) intf_halt_latch |= INT_VBLANK_MASK;
      if (bit(interrupts.LALU_FF0F_D1p.qp_old())) intf_halt_latch |= INT_STAT_MASK;
      if (bit(interrupts.UBUL_FF0F_D3p.qp_old())) intf_halt_latch |= INT_SERIAL_MASK;
      if (bit(interrupts.ULAK_FF0F_D4p.qp_old())) intf_halt_latch |= INT_JOYPAD_MASK;
    }

    // -ha -ab -bc -cd -de -ef +fg +gh
    if (DELTA_GH) {
      cpu_data_latch = 0xFF;
    }

    // +ha -ab -bc -cd -de -ef -fg +gh
    if (DELTA_GH) {
      intf_latch = (uint8_t)pack_old(5, &interrupts.LOPE_FF0F_D0p);
    }
  }

  //-----------------------------------------------------------------------------

  memset(&new_bus, BIT_NEW | BIT_PULLED | 1, sizeof(new_bus));

  //-----------------------------------------------------------------------------

  {
    bool EXT_cpu_latch_ext;

    if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      // Data has to be driven on EFGH or we fail the wave tests
      new_bus.set_data(bus_req_new.write, bus_req_new.data_lo);
      EXT_cpu_latch_ext = bus_req_new.read;
    }
    else {
      new_bus.set_data(false, 0);
      EXT_cpu_latch_ext = 0;
    }
    /*_SIG_IN_CPU_LATCH_EXT*/ cpu_signals.SIG_IN_CPU_LATCH_EXT.sig_in(EXT_cpu_latch_ext);

    bool EXT_addr_new = (bus_req_new.read || bus_req_new.write);
    bool in_bootrom = bit(~cpu_signals.TEPU_BOOT_BITn_h.qp_old());
    bool addr_boot = (bus_req_new.addr <= 0x00FF) && in_bootrom;
    bool addr_vram = (bus_req_new.addr >= 0x8000) && (bus_req_new.addr < 0x9FFF);
    bool addr_high = (bus_req_new.addr >= 0xFE00);

    bool EXT_cpu_rd;
    bool EXT_cpu_wr;

    if (DELTA_HA) {
      EXT_cpu_rd = 0;
      EXT_cpu_wr = 0;
      new_bus.set_addr(bus_req_new.addr & 0x00FF);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
      if (addr_vram) EXT_addr_new = false;
    }
    else {
      EXT_cpu_rd = bus_req_new.read;
      EXT_cpu_wr = bus_req_new.write;
      new_bus.set_addr(bus_req_new.addr);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
    }

    /*_SIG_IN_CPU_RDp*/ cpu_signals.SIG_IN_CPU_RDp.sig_in(EXT_cpu_rd);
    /*_SIG_IN_CPU_WRp*/ cpu_signals.SIG_IN_CPU_WRp.sig_in(EXT_cpu_wr);
    /*_SIG_IN_CPU_EXT_BUSp*/ cpu_signals.SIG_IN_CPU_EXT_BUSp.sig_in(EXT_addr_new);
  }

  //-----------------------------------------------------------------------------

  {
    wire EXT_sys_clkreq = bit(sys_clkreq);
    wire EXT_sys_rst = bit(~sys_rst);
    wire EXT_sys_t2 = bit(~sys_t2);
    wire EXT_sys_t1 = bit(~sys_t1);

    wire EXT_clkin = !(phase_total & 1) && sys_clken;
    wire EXT_clkgood = bit(~sys_clkgood);

    /*_PIN_74*/ clk.PIN_74_CLK.pin_clk(EXT_clkin, EXT_clkgood);
    /*_PIN_71*/ rst.PIN_71_RST.pin_in(EXT_sys_rst);
    /*_PIN_76*/ rst.PIN_76_T2.pin_in(EXT_sys_t2);
    /*_PIN_77*/ rst.PIN_77_T1.pin_in(EXT_sys_t1);

    wire EXT_ack_vblank = bit(gb_cpu.int_ack, BIT_VBLANK);
    wire EXT_ack_stat = bit(gb_cpu.int_ack, BIT_STAT);
    wire EXT_ack_timer = bit(gb_cpu.int_ack, BIT_TIMER);
    wire EXT_ack_serial = bit(gb_cpu.int_ack, BIT_SERIAL);
    wire EXT_ack_joypad = bit(gb_cpu.int_ack, BIT_JOYPAD);

    /*_SIG_CPU_ACK_VBLANK*/ interrupts.SIG_CPU_ACK_VBLANK.sig_in(EXT_ack_vblank);
    /*_SIG_CPU_ACK_STAT  */ interrupts.SIG_CPU_ACK_STAT.sig_in(EXT_ack_stat);
    /*_SIG_CPU_ACK_TIMER */ interrupts.SIG_CPU_ACK_TIMER.sig_in(EXT_ack_timer);
    /*_SIG_CPU_ACK_SERIAL*/ interrupts.SIG_CPU_ACK_SERIAL.sig_in(EXT_ack_serial);
    /*_SIG_CPU_ACK_JOYPAD*/ interrupts.SIG_CPU_ACK_JOYPAD.sig_in(EXT_ack_joypad);

    /*_SIG_CPU_CLKREQ*/ clk.SIG_CPU_CLKREQ.sig_in(EXT_sys_clkreq);

    /*_SIG_CPU_ADDR_HIp*/ cpu_signals.SIG_CPU_ADDR_HIp.sig_out(new_bus.SYRO_FE00_FFFF());
    /*_SIG_CPU_UNOR_DBG*/ cpu_signals.SIG_CPU_UNOR_DBG.sig_out(UNOR_MODE_DBG2p());
    /*_SIG_CPU_UMUT_DBG*/ cpu_signals.SIG_CPU_UMUT_DBG.sig_out(UMUT_MODE_DBG1p());
  }

  //-----------------------------------------------------------------------------
  // Sys clock signals

  logic_mode ? tock_clocks_logic() : tock_clocks_gates();

  if (logic_mode) {
    wire CLK_xxxxEFGx = !!(phase_mask_new & 0b00001110);
    wire CLK_xxCDEFGH = !!(phase_mask_new & 0b00111111);

    cpu_signals.TEDO_CPU_RDp        = cpu_signals.SIG_IN_CPU_RDp.state;
    cpu_signals.APOV_CPU_WRp        = CLK_xxxxEFGx & cpu_signals.SIG_IN_CPU_WRp.state;
    cpu_signals.TAPU_CPU_WRp        = cpu_signals.APOV_CPU_WRp.state;
    cpu_signals.ABUZ_EXT_RAM_CS_CLK = CLK_xxCDEFGH & cpu_signals.SIG_IN_CPU_EXT_BUSp.state;
  }
  else {
    /*_p07.UJYV*/ wire UJYV_CPU_RDn = not1(cpu_signals.SIG_IN_CPU_RDp.out_new());
    /*_p07.TEDO*/ cpu_signals.TEDO_CPU_RDp = not1(UJYV_CPU_RDn);

    /*#p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(ADAR_ABCxxxxH(), ATYP_ABCDxxxx());
    /*_p01.AREV*/ wire AREV_CPU_WRn = nand2(cpu_signals.SIG_IN_CPU_WRp.out_new(), AFAS_xxxxEFGx);
    /*_p01.APOV*/ cpu_signals.APOV_CPU_WRp = not1(AREV_CPU_WRn);

    /*_p07.UBAL*/ wire UBAL_CPU_WRn = not1(cpu_signals.APOV_CPU_WRp.out_new());
    /*_p07.TAPU*/ cpu_signals.TAPU_CPU_WRp = not1(UBAL_CPU_WRn); // xxxxEFGx

    /*#p01.AGUT*/ wire AGUT_xxCDEFGH = or_and3(AROV_xxCDEFxx(), AJAX_xxxxEFGH(), cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new());
    /*#p01.AWOD*/ wire AWOD_ABxxxxxx = nor2(UNOR_MODE_DBG2p(), AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ cpu_signals.ABUZ_EXT_RAM_CS_CLK = not1(AWOD_ABxxxxxx);
  }

  logic_mode ? tock_div_logic() : tock_div_gates();

  if (logic_mode) {
    tock_reset_logic(bit(sys_fastboot) ? div.TERO_DIV03p : div.UPOF_DIV15p);
  }
  else {
    tock_reset_gates(bit(sys_fastboot) ? div.TERO_DIV03p : div.UPOF_DIV15p);
  }

  tock_lcdc(); // LCDC has to be near the top as it controls the video reset signal

  if (logic_mode) {
    tock_vid_clocks_logic();
    tock_lyc_logic();
    tock_lcd_logic();
    tock_joypad_logic();
    tock_serial_logic();
    tock_timer_logic();
    tock_bootrom_logic();
    tock_dma_logic();
  }
  else {
    tock_vid_clocks_gates();
    tock_lyc_gates();
    tock_lcd_gates();
    tock_joypad_gates();
    tock_serial_gates();
    tock_timer_gates();
    tock_bootrom_gates();
    tock_dma_gates();
  }

  //----------------------------------------


  /*#p21.PARU*/ wire PARU_VBLANKp = not1(lcd.POPU_y144p.qn_new());
  /*_p27.REPU*/ wire REPU_VBLANKp = or2(PARU_VBLANKp, PYRY_VID_RSTp());

  /*_p21.TADY*/ wire TADY_LINE_RSTn = nor2(ATEJ_LINE_RSTp.out_new(), TOFU_VID_RSTp());
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(ATEJ_LINE_RSTp.out_new(), ATAR_VID_RSTp());
  /*#p29.BALU*/ wire BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn = not1(BALU_LINE_RSTp);
  /*_p27.XAHY*/ wire XAHY_LINE_RSTn = not1(ATEJ_LINE_RSTp.out_new());

  //----------------------------------------

  //----------------------------------------
  // Sprite scanner

  {
    /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(ALET_xBxDxFxH(), BAGY_LINE_RSTn, sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
    /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(XUPY_ABxxEFxx(), BAGY_LINE_RSTn, sprite_scanner.FETO_SCAN_DONEp.out_old());
    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp, sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ sprite_scanner.AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);

    /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(ATAR_VID_RSTp(), sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new());
    /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGn.dff17(XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), sprite_scanner.BESU_SCANNINGp.qp_old());
    /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd.CATU_x113p.qp_new(), ASEN_SCAN_DONE_TRIGp);

    /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(dma.MATU_DMA_RUNNINGp.qp_new());
    /*#p28.ACYL*/ sprite_scanner.ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, sprite_scanner.BESU_SCANNINGp.qp_new());

    // Sprite scanner grabs the sprite index off the _old_ oam address bus
    /*_p28.YFOT*/ wire YFOT_OAM_A2p_old = not1(oam_bus.BUS_OAM_A02n.out_old());
    /*_p28.YFOC*/ wire YFOC_OAM_A3p_old = not1(oam_bus.BUS_OAM_A03n.out_old());
    /*_p28.YVOM*/ wire YVOM_OAM_A4p_old = not1(oam_bus.BUS_OAM_A04n.out_old());
    /*_p28.YMEV*/ wire YMEV_OAM_A5p_old = not1(oam_bus.BUS_OAM_A05n.out_old());
    /*_p28.XEMU*/ wire XEMU_OAM_A6p_old = not1(oam_bus.BUS_OAM_A06n.out_old());
    /*_p28.YZET*/ wire YZET_OAM_A7p_old = not1(oam_bus.BUS_OAM_A07n.out_old());
    /*_p30.XADU*/ sprite_scanner.XADU_SPRITE_IDX0p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YFOT_OAM_A2p_old);
    /*_p30.XEDY*/ sprite_scanner.XEDY_SPRITE_IDX1p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YFOC_OAM_A3p_old);
    /*_p30.ZUZE*/ sprite_scanner.ZUZE_SPRITE_IDX2p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YVOM_OAM_A4p_old);
    /*_p30.XOBE*/ sprite_scanner.XOBE_SPRITE_IDX3p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YMEV_OAM_A5p_old);
    /*_p30.YDUF*/ sprite_scanner.YDUF_SPRITE_IDX4p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), XEMU_OAM_A6p_old);
    /*_p30.XECU*/ sprite_scanner.XECU_SPRITE_IDX5p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YZET_OAM_A7p_old);

    // FIXME is this OK without the second assignment of FETO?

    for (int feedback = 0; feedback < 2; feedback++) {
      // 32 + 4 + 2 + 1 = 39
      /*#p28.FETO*/ sprite_scanner.FETO_SCAN_DONEp = and4(sprite_scanner.YFEL_SCAN0.qp_mid(), sprite_scanner.WEWY_SCAN1.qp_mid(), sprite_scanner.GOSO_SCAN2.qp_mid(), sprite_scanner.FONY_SCAN5.qp_mid());

      /*#p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(sprite_scanner.FETO_SCAN_DONEp.out_new(), XUPY_ABxxEFxx());

      /*#p28.YFEL*/ sprite_scanner.YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp,                   ANOM_LINE_RSTn, sprite_scanner.YFEL_SCAN0.qn_any());
      /*_p28.WEWY*/ sprite_scanner.WEWY_SCAN1.dff17_any(sprite_scanner.YFEL_SCAN0.qn_any(), ANOM_LINE_RSTn, sprite_scanner.WEWY_SCAN1.qn_any());
      /*_p28.GOSO*/ sprite_scanner.GOSO_SCAN2.dff17_any(sprite_scanner.WEWY_SCAN1.qn_any(), ANOM_LINE_RSTn, sprite_scanner.GOSO_SCAN2.qn_any());
      /*_p28.ELYN*/ sprite_scanner.ELYN_SCAN3.dff17_any(sprite_scanner.GOSO_SCAN2.qn_any(), ANOM_LINE_RSTn, sprite_scanner.ELYN_SCAN3.qn_any());
      /*_p28.FAHA*/ sprite_scanner.FAHA_SCAN4.dff17_any(sprite_scanner.ELYN_SCAN3.qn_any(), ANOM_LINE_RSTn, sprite_scanner.FAHA_SCAN4.qn_any());
      /*_p28.FONY*/ sprite_scanner.FONY_SCAN5.dff17_any(sprite_scanner.FAHA_SCAN4.qn_any(), ANOM_LINE_RSTn, sprite_scanner.FONY_SCAN5.qn_any());
    }

    ///*#p28.FETO*/ sprite_scanner.FETO_SCAN_DONEp = and4(sprite_scanner.YFEL_SCAN0.qp_new(), sprite_scanner.WEWY_SCAN1.qp_new(), sprite_scanner.GOSO_SCAN2.qp_new(), sprite_scanner.FONY_SCAN5.qp_new());
  }


  //----------------------------------------
  // Global rendering flag 'XYMU'

  /*#p21.VOGA*/ VOGA_HBLANKp.dff17(ALET_xBxDxFxH(), TADY_LINE_RSTn, WODU_HBLANKp.out_old());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), VOGA_HBLANKp.qp_new());
  /*#p21.XYMU*/ XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new());
  /*_p24.LOBY*/ wire LOBY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
  /*#p27.PAHA*/ wire PAHA_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
  /*_p29.TEPA*/ wire TEPA_RENDERINGp = not1(XYMU_RENDERINGn.qn_new());

  //----------------------------------------
  // Sprite fetch sequencer

  {
    /*#p27.SYLO*/ wire SYLO_WIN_HITn_old = not1(win_reg.RYDY_WIN_HITp.out_old());
    /*#p24.TOMU*/ wire TOMU_WIN_HITp_old = not1(SYLO_WIN_HITn_old);
    /*_p27.TUKU*/ wire TUKU_WIN_HITn_old = not1(TOMU_WIN_HITp_old);
    /*_p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_old = not1(sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp_old());
    /*_p27.TEKY*/ wire TEKY_SFETCH_REQp_old = and4(sprite_match_flags.FEPO_STORE_MATCHp.out_old(), TUKU_WIN_HITn_old, tile_fetcher.LYRY_BFETCH_DONEp.out_old(), SOWO_SFETCH_RUNNINGn_old);
    /*_p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp.dff17(LAPE_AxCxExGx(), SIG_VCC.out_new(), sprite_fetcher.SOBU_SFETCH_REQp.qp_old());
    /*_p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp.dff17(TAVA_xBxDxFxH(), SIG_VCC.out_new(), TEKY_SFETCH_REQp_old);

    /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1.dff17(LAPE_AxCxExGx(), SIG_VCC.out_new(),         sprite_fetcher.TOXE_SFETCH_S0p.qp_old());
    /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5.dff17(LAPE_AxCxExGx(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_old());
    /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TOBU_SFETCH_S1p_D2.qp_old());
    /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TULY_SFETCH_S1p.qp_old());

    /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
    /*#p27.SECA*/ wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), ATEJ_LINE_RSTp.out_new());

    // Feedback loop
    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand2(sprite_fetcher.TESE_SFETCH_S2p.qp_any(), sprite_fetcher.TOXE_SFETCH_S0p.qp_any());
      /*#p29.TOMA*/ wire TOMA_SFETCH_xBxDxFxH = nand2(LAPE_AxCxExGx(), TAME_SFETCH_CLK_GATE);
      /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.dff17_any(TOMA_SFETCH_xBxDxFxH,                    SECA_SFETCH_STARTn, sprite_fetcher.TOXE_SFETCH_S0p.qn_any());
      /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p.dff17_any(sprite_fetcher.TOXE_SFETCH_S0p.qn_any(), SECA_SFETCH_STARTn, sprite_fetcher.TULY_SFETCH_S1p.qn_any());
      /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p.dff17_any(sprite_fetcher.TULY_SFETCH_S1p.qn_any(), SECA_SFETCH_STARTn, sprite_fetcher.TESE_SFETCH_S2p.qn_any());
    }

    /*_p29.SAKY*/ wire SAKY_SFETCHn = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /*_p29.TYSO*/ wire TYSO_SFETCHINGn = or2(SAKY_SFETCHn, TEPA_RENDERINGp); // def or
    /*_p29.TEXY*/ sprite_fetcher.TEXY_SFETCHINGp = not1(TYSO_SFETCHINGn);

    /*_p29.TYNO*/ wire TYNO = nand3(sprite_fetcher.TOXE_SFETCH_S0p.qp_new(), sprite_fetcher.SEBA_SFETCH_S1p_D5.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /*_p29.VUSA*/ wire VUSA_SPRITE_DONEn = or2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qn_new(), TYNO);
    /*_p29.WUTY*/ sprite_fetcher.WUTY_SFETCH_DONE_TRIGp = not1(VUSA_SPRITE_DONEn);
  }

  /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
  /*#p27.SECA*/ wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), ATEJ_LINE_RSTp.out_new());

  //----------------------------------------
  // Window state has some interaction with the tile fetcher here.

  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn, XAPO_VID_RSTn());
  /*_p27.NUNU*/ win_reg.NUNU_WIN_MATCHp. dff17(MEHE_AxCxExGx(), XAPO_VID_RSTn(), win_reg.PYCO_WIN_MATCHp.qp_old());
  /*_p27.NOPA*/ win_reg.NOPA_WIN_MODE_Bp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), win_reg.PYNU_WIN_MODE_Ap.qp_old());
  /*_p27.PYNU*/ win_reg.PYNU_WIN_MODE_Ap.nor_latch(win_reg.NUNU_WIN_MATCHp.qp_new(), XOFO_WIN_RSTp);

  /*#p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and2(win_reg.PYNU_WIN_MODE_Ap.qp_new(), win_reg.NOPA_WIN_MODE_Bp.qn_new());
  /*_p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not1(NUNY_WIN_MODE_TRIGp);
  /*_p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not1(NYFO_WIN_MODE_TRIGn);
  /*_p24.NAFY*/ wire NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);

  /*_p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp.dff17(ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), tile_fetcher.PORY_FETCH_DONEp.qp_old());
  /*_p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NAFY_WIN_MODE_TRIGn,      tile_fetcher.NYKA_FETCH_DONEp.qp_old());
  /*_p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17(ALET_xBxDxFxH(), NAFY_WIN_MODE_TRIGn,      tile_fetcher.LYRY_BFETCH_DONEp.out_old());
  /*_p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.qp_new(), LOBY_RENDERINGn);

  /*_p27.SOVY*/ win_reg.SOVY_WIN_HITp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), win_reg.RYDY_WIN_HITp.out_old());
  /*_p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn.out_mid(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);
  /*_p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn.out_mid(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);

  /*#p27.SYLO*/ wire SYLO_WIN_HITn = not1(win_reg.RYDY_WIN_HITp.out_new());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp = not1(SYLO_WIN_HITn);
  /*#p24.SOCY*/ wire SOCY_WIN_HITn = not1(TOMU_WIN_HITp);

  /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn      = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp_new());
  /*_p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn = nand4(XYMU_RENDERINGn.qn_new(), ROMO_PRELOAD_DONEn, tile_fetcher.NYKA_FETCH_DONEp.qp_new(), tile_fetcher.PORY_FETCH_DONEp.qp_new());
  /*_p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp = not1(SUVU_PRELOAD_DONE_TRIGn);

  /*_p27.VEKU*/ wire VEKU_SFETCH_ENDn = nor2(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), TAVE_PRELOAD_DONE_TRIGp); // def nor
  /*_p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp.nand_latch(SECA_SFETCH_STARTn, VEKU_SFETCH_ENDn);

  //----------------------------------------
  // OAM temp register B stores sprite X coordinate during scan, so we need a copy of it for later.

  auto oam_temp_b_old = oam_temp_b;

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  {
    /*_p25.CUFE*/ wire CUFE_OAM_CLKp = not_or_and3(new_bus.SARO_ADDR_OAMp(), dma.MATU_DMA_RUNNINGp.qp_new(), MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
    /*_p25.AVER*/ wire AVER_AxxxExxx = nand2(sprite_scanner.ACYL_SCANNINGp.out_new(), XYSO_xBCDxFGH());
    /*_p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
    /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n);
    /*_p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(TEPA_RENDERINGp, sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.TESE_SFETCH_S2p.qp_new());
    /*_p25.VAPE*/ wire VAPE_OAM_CLKENn = and2(TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
    /*_p25.XUJY*/ wire XUJY_OAM_CLKENp = not1(VAPE_OAM_CLKENn);
    /*_p25.BYCU*/ wire BYCU_OAM_CLKp = nand3(AVER_AxxxExxx, XUJY_OAM_CLKENp, CUFE_OAM_CLKp);
    /*_p25.COTA*/ wire COTA_OAM_CLKn = not1(BYCU_OAM_CLKp);
    oam_latch_to_temp_a(COTA_OAM_CLKn, oam_latch_a, oam_temp_a);
    oam_latch_to_temp_b(COTA_OAM_CLKn, oam_latch_b, oam_temp_b);
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  SpriteDeltaY sprite_delta_y = sub_sprite_y();

  {
    /*#p29.GOVU*/ wire GOVU_SPSIZE_MATCH = or2(reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new(), sprite_delta_y.GYKY_YDIFF3.sum);
    /*_p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand6(sprite_delta_y.GACE_SPRITE_DELTA4, sprite_delta_y.GUVU_SPRITE_DELTA5, sprite_delta_y.GYDA_SPRITE_DELTA6, sprite_delta_y.GEWY_SPRITE_DELTA7, sprite_delta_y.WUHU_YDIFF7.carry, GOVU_SPSIZE_MATCH);
    /*_p29.GESE*/ wire GESE_SCAN_MATCH_Yp = not1(WOTA_SCAN_MATCH_Yn);
    /*_p29.CEHA*/ wire CEHA_SCANNINGp = not1(sprite_scanner.CENO_SCANNINGn.qn_new());
    /*_p29.CARE*/ wire CARE_COUNT_CLKn = and3(XOCE_xBCxxFGx(), CEHA_SCANNINGp, GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /*_p29.DYTY*/ wire DYTY_COUNT_CLKp = not1(CARE_COUNT_CLKn);
    /*_p29.DEZY*/ sprite_counter.DEZY_COUNT_CLKp.dff17_any(ZEME_AxCxExGx(), XAPO_VID_RSTn(), DYTY_COUNT_CLKp);

    // There's a feedback loop here, but we don't actually need to loop - BAKY holds the clock line high once the sprite store is full, so doing a second logic pass
    // doesn't actually change any of the dffs.
    /*#p29.BAKY*/ wire BAKY_SPRITES_FULL_new = and2(sprite_counter.CUXY_SPRITE_COUNT1.qp_any(), sprite_counter.DYBE_SPRITE_COUNT3.qp_any());
    /*#p29.CAKE*/ wire CAKE_COUNT_CLKp_new = or2(BAKY_SPRITES_FULL_new, sprite_counter.DEZY_COUNT_CLKp.qp_any());
    /*#p28.AZYB*/ wire AZYB_LINE_TRIGn = not1(ATEJ_LINE_RSTp.out_new());
    /*_p29.BESE*/ sprite_counter.BESE_SPRITE_COUNT0.dff17_any(CAKE_COUNT_CLKp_new,                       AZYB_LINE_TRIGn, sprite_counter.BESE_SPRITE_COUNT0.qn_any());
    /*_p29.CUXY*/ sprite_counter.CUXY_SPRITE_COUNT1.dff17_any(sprite_counter.BESE_SPRITE_COUNT0.qn_any(), AZYB_LINE_TRIGn, sprite_counter.CUXY_SPRITE_COUNT1.qn_any());
    /*_p29.BEGO*/ sprite_counter.BEGO_SPRITE_COUNT2.dff17_any(sprite_counter.CUXY_SPRITE_COUNT1.qn_any(), AZYB_LINE_TRIGn, sprite_counter.BEGO_SPRITE_COUNT2.qn_any());
    /*_p29.DYBE*/ sprite_counter.DYBE_SPRITE_COUNT3.dff17_any(sprite_counter.BEGO_SPRITE_COUNT2.qn_any(), AZYB_LINE_TRIGn, sprite_counter.DYBE_SPRITE_COUNT3.qn_any());

    /*_p28.ABAK*/ wire ABAK_LINE_RSTp = or2(ATEJ_LINE_RSTp.out_new(), AMYG_VID_RSTp());
    /*_p28.BYVA*/ wire BYVA_LINE_RSTn = not1(ABAK_LINE_RSTp);

    update_sprite_reset_flags(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp, BYVA_LINE_RSTn, sprite_match_flags, sprite_reset_flags);
    update_sprite_store_flags(sprite_counter, DYTY_COUNT_CLKp, sprite_store_flags);
    store_sprite(sprite_store_flags, sprite_reset_flags, BYVA_LINE_RSTn, sprite_bus, oam_temp_b_old, sprite_store);
  }

  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  auto XYDO_PX3p_old = pix_count.XYDO_PX3p;

  // Weird feedback loop here - fine scroll and pix count affect store match, which affects pipe clock, which affects fine scroll and pix count.
  // Implementing the feedback doesn't seem to change any test results, but... paranoia...

  //for (int feedback = 0; feedback < 2; feedback++) {
  {
    /*_p27.SUHA*/ wire SUHA_SCX_FINE_MATCHp_old = xnor2(reg_scx.DATY_SCX0n.qn_old(), fine_scroll.RYKU_FINE_CNT0.qp_old());
    /*_p27.SYBY*/ wire SYBY_SCX_FINE_MATCHp_old = xnor2(reg_scx.DUZU_SCX1n.qn_old(), fine_scroll.ROGA_FINE_CNT1.qp_old());
    /*_p27.SOZU*/ wire SOZU_SCX_FINE_MATCHp_old = xnor2(reg_scx.CYXU_SCX2n.qn_old(), fine_scroll.RUBU_FINE_CNT2.qp_old());
    /*#p27.RONE*/ wire RONE_SCX_FINE_MATCHn_old = nand4(fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any(), SUHA_SCX_FINE_MATCHp_old, SYBY_SCX_FINE_MATCHp_old, SOZU_SCX_FINE_MATCHp_old);
    /*#p27.POHU*/ wire POHU_SCX_FINE_MATCHp_old = not1(RONE_SCX_FINE_MATCHn_old);

    /*_p29.CEHA*/ wire CEHA_SCANNINGp = not1(sprite_scanner.CENO_SCANNINGn.qn_new());
    /*#p29.BYJO*/ wire BYJO_SCANNINGn = not1(CEHA_SCANNINGp);
    /*#p29.AZEM*/ wire AZEM_RENDERINGp = and2(XYMU_RENDERINGn.qn_new(), BYJO_SCANNINGn);
    /*#p29.AROR*/ wire AROR_MATCH_ENp = and2(AZEM_RENDERINGp, reg_lcdc.XYLO_LCDC_SPENn.qn_new());

    /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(sprite_match_flags.FEPO_STORE_MATCHp.out_mid(), WODU_HBLANKp.out_old(), MYVO_AxCxExGx());
    /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
    /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);

    /*#p27.NYZE*/ fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17_any(MOXE_AxCxExGx(),  XYMU_RENDERINGn.qn_new(), fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any());
    /*#p27.PUXA*/ fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17_any(ROXO_CLKPIPE_odd, XYMU_RENDERINGn.qn_new(), POHU_SCX_FINE_MATCHp_old);

    /*#p27.POVA*/ wire POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_any());
    /*#p27.ROXY*/ fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(PAHA_RENDERINGn, POVA_FINE_MATCH_TRIGp);

    /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any());
    // Pixel counter, has carry lookahead because this can increment every tcycle
    /*_p21.RYBO*/ wire RYBO_old = xor2(pix_count.XEHO_PX0p.qp_any(), pix_count.SAVY_PX1p.qp_any()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /*_p21.XUKE*/ wire XUKE_old = and2(pix_count.XEHO_PX0p.qp_any(), pix_count.SAVY_PX1p.qp_any());
    /*_p21.XYLE*/ wire XYLE_old = and2(pix_count.XODU_PX2p.qp_any(), XUKE_old);
    /*_p21.XEGY*/ wire XEGY_old = xor2(pix_count.XODU_PX2p.qp_any(), XUKE_old); // feet facing gnd
    /*_p21.XORA*/ wire XORA_old = xor2(pix_count.XYDO_PX3p.qp_any(), XYLE_old); // feet facing gnd

    /*_p21.XEHO*/ pix_count.XEHO_PX0p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, pix_count.XEHO_PX0p.qn_any());
    /*_p21.SAVY*/ pix_count.SAVY_PX1p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, RYBO_old);
    /*_p21.XODU*/ pix_count.XODU_PX2p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEGY_old);
    /*_p21.XYDO*/ pix_count.XYDO_PX3p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XORA_old);

    /*_p24.TOCA*/ wire TOCA_new = not1(pix_count.XYDO_PX3p.qp_any());
    /*_p21.SAKE*/ wire SAKE_old = xor2(pix_count.TUHU_PX4p.qp_any(), pix_count.TUKY_PX5p.qp_any());
    /*_p21.TYBA*/ wire TYBA_old = and2(pix_count.TUHU_PX4p.qp_any(), pix_count.TUKY_PX5p.qp_any());
    /*_p21.SURY*/ wire SURY_old = and2(pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /*_p21.TYGE*/ wire TYGE_old = xor2(pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /*_p21.ROKU*/ wire ROKU_old = xor2(pix_count.SYBE_PX7p.qp_any(), SURY_old);

    /*_p21.TUHU*/ pix_count.TUHU_PX4p.dff17_any(TOCA_new, TADY_LINE_RSTn, pix_count.TUHU_PX4p.qn_any());
    /*_p21.TUKY*/ pix_count.TUKY_PX5p.dff17_any(TOCA_new, TADY_LINE_RSTn, SAKE_old);
    /*_p21.TAKO*/ pix_count.TAKO_PX6p.dff17_any(TOCA_new, TADY_LINE_RSTn, TYGE_old);
    /*_p21.SYBE*/ pix_count.SYBE_PX7p.dff17_any(TOCA_new, TADY_LINE_RSTn, ROKU_old);

    get_sprite_match_flags(pix_count, AROR_MATCH_ENp, sprite_store, sprite_match_flags, SIG_GND);
  }

  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  {
    /*#p21.XENA*/ wire XENA_STORE_MATCHn = not1(sprite_match_flags.FEPO_STORE_MATCHp.out_new());
    /*#p21.XUGU*/ wire XUGU_PX167n = nand5(pix_count.XEHO_PX0p.qp_new(), pix_count.SAVY_PX1p.qp_new(), pix_count.XODU_PX2p.qp_new(), pix_count.TUKY_PX5p.qp_new(), pix_count.SYBE_PX7p.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire XANO_PX167p = not1(XUGU_PX167n);
    /*#p21.WODU*/ WODU_HBLANKp = and2(XENA_STORE_MATCHn, XANO_PX167p); // WODU goes high on odd, cleared on H
  }

  /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(sprite_match_flags.FEPO_STORE_MATCHp.out_new(), WODU_HBLANKp.out_new(), MYVO_AxCxExGx());
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);
  /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  /*_p24.PAHO*/ lcd.PAHO_X_8_SYNC.dff17(ROXO_CLKPIPE_odd, XYMU_RENDERINGn.qn_new(), XYDO_PX3p_old.qp_old());

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  memset(&sprite_bus, BIT_NEW | BIT_PULLED | 1, sizeof(sprite_bus));

  sprite_match_to_bus(sprite_store, sprite_match_flags, sprite_bus);
  sprite_scan_to_bus(sprite_delta_y, XYMU_RENDERINGn, sprite_match_flags.FEPO_STORE_MATCHp);

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // WY/WX/window match

  tock_window(SEGU_CLKPIPE_evn, REPU_VBLANKp);

  //----------------------------------------
  // Tile fetch sequencer

  /*_p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp = nor2(win_reg.RYFA_WIN_FETCHn_A.qn_new(), win_reg.RENE_WIN_FETCHn_B.qp_new());
  /*_p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne = nand2(SYLO_WIN_HITn, win_reg.SOVY_WIN_HITp.qp_new());
  /*_p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne = not1(TUXY_WIN_FIRST_TILEne);
  /*_p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp = or3(SEKO_WIN_FETCH_TRIGp, SUZU_WIN_FIRST_TILEne, TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  /*_p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new(), MOSU_WIN_MODE_TRIGp, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(tile_fetcher.LAXU_BFETCH_S0p.qp_any(), tile_fetcher.NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn);
    /*_p27.LEBO*/ wire LEBO_AxCxExGx = nand2(ALET_xBxDxFxH(), MOCE_BFETCH_DONEn);

    /*_p27.LAXU*/ tile_fetcher.LAXU_BFETCH_S0p.dff17_any(LEBO_AxCxExGx,                         NYXU_BFETCH_RSTn, tile_fetcher.LAXU_BFETCH_S0p.qn_any());
    /*_p27.MESU*/ tile_fetcher.MESU_BFETCH_S1p.dff17_any(tile_fetcher.LAXU_BFETCH_S0p.qn_any(), NYXU_BFETCH_RSTn, tile_fetcher.MESU_BFETCH_S1p.qn_any());
    /*_p27.NYVA*/ tile_fetcher.NYVA_BFETCH_S2p.dff17_any(tile_fetcher.MESU_BFETCH_S1p.qn_any(), NYXU_BFETCH_RSTn, tile_fetcher.NYVA_BFETCH_S2p.qn_any());
  }

  /*_p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, tile_fetcher.LYRY_BFETCH_DONEp.out_old());

  /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(tile_fetcher.LAXU_BFETCH_S0p.qp_new(), tile_fetcher.NYVA_BFETCH_S2p.qp_new(), NYXU_BFETCH_RSTn);
  /*_p27.LYRY*/ tile_fetcher.LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  /*_p27.LURY*/ wire LURY_BG_FETCH_DONEn = and2(tile_fetcher.LOVY_FETCH_DONEp.qn_new(), XYMU_RENDERINGn.qn_new());
  /*_p27.LONY*/ tile_fetcher.LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, LURY_BG_FETCH_DONEn);
  /*_p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0p_D1.dff17(ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), tile_fetcher.LAXU_BFETCH_S0p.qp_new());

  //----------------------------------------
  // Fine match counter

  /*#p27.PASO*/ wire PASO_FINE_RST = nor2(PAHA_RENDERINGn, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p27.ROZE*/ wire ROZE_FINE_COUNT_7n = nand3(fine_scroll.RUBU_FINE_CNT2.qp_any(), fine_scroll.ROGA_FINE_CNT1.qp_any(), fine_scroll.RYKU_FINE_CNT0.qp_any());
    /*#p27.PECU*/ wire PECU_FINE_CLK = nand2(ROXO_CLKPIPE_odd, ROZE_FINE_COUNT_7n);
    /*#p27.RYKU*/ fine_scroll.RYKU_FINE_CNT0.dff17_any(PECU_FINE_CLK,                       PASO_FINE_RST, fine_scroll.RYKU_FINE_CNT0.qn_any());
    /*#p27.ROGA*/ fine_scroll.ROGA_FINE_CNT1.dff17_any(fine_scroll.RYKU_FINE_CNT0.qn_any(), PASO_FINE_RST, fine_scroll.ROGA_FINE_CNT1.qn_any());
    /*#p27.RUBU*/ fine_scroll.RUBU_FINE_CNT2.dff17_any(fine_scroll.ROGA_FINE_CNT1.qn_any(), PASO_FINE_RST, fine_scroll.RUBU_FINE_CNT2.qn_any());
  }

  //----------------------------------------
  // PPU / LCD output

  tock_pix_pipes(SACU_CLKPIPE_evn, NYXU_BFETCH_RSTn);
  logic_mode ? set_lcd_pins_logic(SACU_CLKPIPE_evn) : set_lcd_pins_gates(SACU_CLKPIPE_evn);
  update_framebuffer();

  //----------------------------------------
  // Audio

  tock_spu();

  //----------------------------------------
  // Memory buses

  tock_ext(cart_blob);
  logic_mode ? tock_vram_bus_logic(TEVO_WIN_FETCH_TRIGp) : tock_vram_bus_gates(TEVO_WIN_FETCH_TRIGp);
  logic_mode ? tock_oam_bus_logic() : tock_oam_bus_gates();
  logic_mode ? tock_zram_logic() : tock_zram_gates();

  //----------------------------------------
  // And finally, interrupts.

  logic_mode ? tock_interrupts_logic() : tock_interrupts_gates();

  old_bus = new_bus;
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::update_framebuffer()
{
  int lcd_x = pix_count.get_new() - 8;
  int lcd_y = reg_ly.get_new();

  if (lcd_y >= 0 && lcd_y < 144 && lcd_x >= 0 && lcd_x < 160) {
    wire p0 = bit(lcd.PIN_51_LCD_DATA0.qp_ext_new());
    wire p1 = bit(lcd.PIN_50_LCD_DATA1.qp_ext_new());
    auto new_pix = p0 + p1 * 2;

    framebuffer[lcd_x + lcd_y * 160] = uint8_t(3 - new_pix);
  }

  old_lcd_x = lcd_x;
  old_lcd_y = lcd_y;

#if 0
  if (bit(~lcd.old_lcd_clock.qp_old()) && lcd.PIN_53_LCD_CLOCK.qp_new()) {
    gb_screen_x++;
  }
  if (lcd.PIN_54_LCD_HSYNC.qp_new() || lcd.PIN_55_LCD_LATCH.qp_new()) {
    gb_screen_x = 0;
  }

  if (bit(~lcd.old_lcd_latch.qp_old()) && lcd.PIN_55_LCD_LATCH.qp_new()) {
    if (gb_screen_y < 144) {
      for (int x = 0; x < 159; x++) {
        uint8_t p0 = lcd.lcd_pipe_lo[x + 1].qp_new();
        uint8_t p1 = lcd.lcd_pipe_hi[x + 1].qp_new();
        framebuffer[x + gb_screen_y * 160] = p0 + p1 * 2;
      }
      {
        uint8_t p0 = lcd.lcd_pix_lo.qp_new();
        uint8_t p1 = lcd.lcd_pix_hi.qp_new();
        framebuffer[159 + gb_screen_y * 160] = p0 + p1 * 2;
      }
    }

    if (lcd.PIN_57_LCD_VSYNC.qp_new()) {
      gb_screen_y = 0;
    }
    else {
      gb_screen_y++;
    }
  }

  lcd.old_lcd_clock.set_new(lcd.PIN_53_LCD_CLOCK.qp_new());
  lcd.old_lcd_latch.set_new(lcd.PIN_55_LCD_LATCH.qp_new());
#endif
}

//------------------------------------------------------------------------------------------------------------------------
