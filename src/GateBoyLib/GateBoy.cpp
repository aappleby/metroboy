#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"

//-----------------------------------------------------------------------------

void GateBoy::reset_to_bootrom(const blob& cart_blob, bool fastboot)
{
  wipe();

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

  check_state_old_and_driven_or_pulled();

  sys_fastboot = fastboot;

  //----------------------------------------
  // Update the sim without ticking the clock to to settle initial reset signals.

  sys_rst = 1;

  tock_cpu();
  tock_gates(cart_blob);

  //----------------------------------------
  // Release reset, start clock, and sync with phase

  sys_rst = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  run_phases(cart_blob, 2, false);

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
    run_phases(cart_blob, 8, false);
  }

  //----------------------------------------
  // Delay to sync up with expected div value

  run_phases(cart_blob, 16, false);

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
  
  //reset_sprite_store();
  sprite_counter.DEZY_COUNT_CLKp.state = 0b00011011;
  sprite_counter.BESE_SPRITE_COUNT0.state = 0b00011010;
  sprite_counter.CUXY_SPRITE_COUNT1.state = 0b00011010;
  sprite_counter.BEGO_SPRITE_COUNT2.state = 0b00011010;
  sprite_counter.DYBE_SPRITE_COUNT3.state = 0b00011010;

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
  phase_total = 0;
  phase_origin = 0;

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
  run_phases(cart_blob, 8, false);

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
  run_phases(cart_blob, 8, false);

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

void GateBoy::next_phase(const blob& cart_blob, bool logic_mode) {
  ASSERT_P(sys_clkreq || !logic_mode);

  tock_cpu();

  if (logic_mode) {
    tock_logic(cart_blob);
  }
  else {
    tock_gates(cart_blob);
  }

  update_framebuffer(pix_count.get_old() - 8, reg_ly.get_old(), lcd.PIN_51_LCD_DATA0.qp_ext_old(), lcd.PIN_50_LCD_DATA1.qp_ext_old());

  phase_total++;
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::update_framebuffer(int lcd_x, int lcd_y, wire DATA0, wire DATA1)
{
  //int lcd_x = pix_count.get_new() - 8;
  //int lcd_y = reg_ly.get_new();

  if (lcd_y >= 0 && lcd_y < 144 && lcd_x >= 0 && lcd_x < 160) {
    wire p0 = bit(DATA0);
    wire p1 = bit(DATA1);
    auto new_pix = p0 + p1 * 2;

    framebuffer[lcd_x + lcd_y * 160] = uint8_t(3 - new_pix);
  }

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

void GateBoy::tock_cpu() {
  cpu_data_latch &= (uint8_t)pack(8, (BitBase*)&new_bus.BUS_CPU_D00p);
  imask_latch = (uint8_t)pack(5, &interrupts.IE_D0);

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
    intf_latch = (uint8_t)pack(5, &interrupts.LOPE_FF0F_D0p);
  }
}

//------------------------------------------------------------------------------------------------------------------------































































//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_gates(const blob& cart_blob) {

  //----------

  /*#p27.SYLO*/ wire SYLO_WIN_HITn_old = not1(win_reg.RYDY_WIN_HITp.out_old());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp_old = not1(SYLO_WIN_HITn_old);
  /*#p24.SOCY*/ wire SOCY_WIN_HITn_old = not1(TOMU_WIN_HITp_old);
  /*#p24.VYBO*/ wire VYBO_CLKPIPE_old = nor3(sprite_match_flags.FEPO_STORE_MATCHp.out_old(), WODU_HBLANKp.out_old(), MYVO_AxCxExGx());
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_old = and3(SOCY_WIN_HITn_old, tile_fetcher.POKY_PRELOAD_LATCHp.qp_old(), VYBO_CLKPIPE_old);
  /*#p24.SEGU*/ wire SEGU_CLKPIPE_old = not1(TYFA_CLKPIPE_old);
  /*#p24.ROXO*/ wire ROXO_CLKPIPE_old = not1(SEGU_CLKPIPE_old);
  /*#p24.SACU*/ wire SACU_CLKPIPE_old = or2(SEGU_CLKPIPE_old, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_old());

  //----------

  wire EXT_vcc = 1;
  wire EXT_gnd = 0;

  /*_SIG_VCC*/ SIG_VCC.sig_in(EXT_vcc);
  /*_SIG_GND*/ SIG_GND.sig_in(EXT_gnd);

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

  tock_clocks_gates();

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

  tock_div_gates();
  tock_reset_gates(bit(sys_fastboot) ? div.TERO_DIV03p : div.UPOF_DIV15p);
  tock_lcdc_gates(); // LCDC has to be near the top as it controls the video reset signal
  tock_vid_clocks_gates();
  tock_lyc_gates();
  tock_lcd_gates();
  tock_joypad_gates();
  tock_serial_gates();
  tock_timer_gates();
  tock_bootrom_gates();
  tock_dma_gates();

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
    /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGn.dff17(XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), sprite_scanner.BESU_SCANNINGn.qp_old());
    /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGn.nor_latch(lcd.CATU_x113p.qp_new(), ASEN_SCAN_DONE_TRIGp);

    /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(dma.MATU_DMA_RUNNINGp.qp_new());
    /*#p28.ACYL*/ sprite_scanner.ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, sprite_scanner.BESU_SCANNINGn.qp_new());

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

      /*#p28.YFEL*/ sprite_scanner.YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp, ANOM_LINE_RSTn, sprite_scanner.YFEL_SCAN0.qn_any());
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
    /*_p27.TUKU*/ wire TUKU_WIN_HITn_old = not1(TOMU_WIN_HITp_old);
    /*_p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_old = not1(sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp_old());
    /*_p27.TEKY*/ wire TEKY_SFETCH_REQp_old = and4(sprite_match_flags.FEPO_STORE_MATCHp.out_old(), TUKU_WIN_HITn_old, tile_fetcher.LYRY_BFETCH_DONEp.out_old(), SOWO_SFETCH_RUNNINGn_old);
    /*_p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp.dff17(LAPE_AxCxExGx(), SIG_VCC.out_new(), sprite_fetcher.SOBU_SFETCH_REQp.qp_old());
    /*_p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp.dff17(TAVA_xBxDxFxH(), SIG_VCC.out_new(), TEKY_SFETCH_REQp_old);

    /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1.dff17(LAPE_AxCxExGx(), SIG_VCC.out_new(), sprite_fetcher.TOXE_SFETCH_S0p.qp_old());
    /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5.dff17(LAPE_AxCxExGx(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_old());
    /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TOBU_SFETCH_S1p_D2.qp_old());
    /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TULY_SFETCH_S1p.qp_old());

    /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
    /*#p27.SECA*/ wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), ATEJ_LINE_RSTp.out_new());

    // Feedback loop
    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand2(sprite_fetcher.TESE_SFETCH_S2p.qp_any(), sprite_fetcher.TOXE_SFETCH_S0p.qp_any());
      /*#p29.TOMA*/ wire TOMA_SFETCH_xBxDxFxH = nand2(LAPE_AxCxExGx(), TAME_SFETCH_CLK_GATE);
      /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.dff17_any(TOMA_SFETCH_xBxDxFxH, SECA_SFETCH_STARTn, sprite_fetcher.TOXE_SFETCH_S0p.qn_any());
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
  /*_p27.NUNU*/ win_reg.NUNU_WIN_MATCHp.dff17(MEHE_AxCxExGx(), XAPO_VID_RSTn(), win_reg.PYCO_WIN_MATCHp.qp_old());
  /*_p27.NOPA*/ win_reg.NOPA_WIN_MODE_Bp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), win_reg.PYNU_WIN_MODE_Ap.qp_old());
  /*_p27.PYNU*/ win_reg.PYNU_WIN_MODE_Ap.nor_latch(win_reg.NUNU_WIN_MATCHp.qp_new(), XOFO_WIN_RSTp);

  /*#p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and2(win_reg.PYNU_WIN_MODE_Ap.qp_new(), win_reg.NOPA_WIN_MODE_Bp.qn_new());
  /*_p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not1(NUNY_WIN_MODE_TRIGp);
  /*_p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not1(NYFO_WIN_MODE_TRIGn);
  /*_p24.NAFY*/ wire NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);

  /*_p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp.dff17(ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), tile_fetcher.PORY_FETCH_DONEp.qp_old());
  /*_p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NAFY_WIN_MODE_TRIGn, tile_fetcher.NYKA_FETCH_DONEp.qp_old());
  /*_p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17(ALET_xBxDxFxH(), NAFY_WIN_MODE_TRIGn, tile_fetcher.LYRY_BFETCH_DONEp.out_old());
  /*_p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.qp_new(), LOBY_RENDERINGn);

  /*_p27.SOVY*/ win_reg.SOVY_WIN_HITp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), win_reg.RYDY_WIN_HITp.out_old());
  /*_p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn.out_mid(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);
  /*_p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn.out_mid(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);

  /*#p27.SYLO*/ wire SYLO_WIN_HITn = not1(win_reg.RYDY_WIN_HITp.out_new());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp = not1(SYLO_WIN_HITn);
  /*#p24.SOCY*/ wire SOCY_WIN_HITn = not1(TOMU_WIN_HITp);

  /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp_new());
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

  SpriteDeltaY sprite_delta_y = 0 ? sub_sprite_y_logic() : sub_sprite_y_gates();

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
    /*_p29.BESE*/ sprite_counter.BESE_SPRITE_COUNT0.dff17_any(CAKE_COUNT_CLKp_new, AZYB_LINE_TRIGn, sprite_counter.BESE_SPRITE_COUNT0.qn_any());
    /*_p29.CUXY*/ sprite_counter.CUXY_SPRITE_COUNT1.dff17_any(sprite_counter.BESE_SPRITE_COUNT0.qn_any(), AZYB_LINE_TRIGn, sprite_counter.CUXY_SPRITE_COUNT1.qn_any());
    /*_p29.BEGO*/ sprite_counter.BEGO_SPRITE_COUNT2.dff17_any(sprite_counter.CUXY_SPRITE_COUNT1.qn_any(), AZYB_LINE_TRIGn, sprite_counter.BEGO_SPRITE_COUNT2.qn_any());
    /*_p29.DYBE*/ sprite_counter.DYBE_SPRITE_COUNT3.dff17_any(sprite_counter.BEGO_SPRITE_COUNT2.qn_any(), AZYB_LINE_TRIGn, sprite_counter.DYBE_SPRITE_COUNT3.qn_any());

    /*_p28.ABAK*/ wire ABAK_LINE_RSTp = or2(ATEJ_LINE_RSTp.out_new(), AMYG_VID_RSTp());
    /*_p28.BYVA*/ wire BYVA_LINE_RSTn = not1(ABAK_LINE_RSTp);

    //update_sprite_reset_flags(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp, BYVA_LINE_RSTn, sprite_match_flags, sprite_reset_flags);

    /*_p29.EBOJ*/ sprite_reset_flags.EBOJ_STORE0_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.GUVA_SPRITE0_GETp.out_old());
    /*_p29.CEDY*/ sprite_reset_flags.CEDY_STORE1_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.ENUT_SPRITE1_GETp.out_old());
    /*_p29.EGAV*/ sprite_reset_flags.EGAV_STORE2_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.EMOL_SPRITE2_GETp.out_old());
    /*_p29.GOTA*/ sprite_reset_flags.GOTA_STORE3_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.GYFY_SPRITE3_GETp.out_old());
    /*_p29.XUDY*/ sprite_reset_flags.XUDY_STORE4_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.GONO_SPRITE4_GETp.out_old());
    /*_p29.WAFY*/ sprite_reset_flags.WAFY_STORE5_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.GEGA_SPRITE5_GETp.out_old());
    /*_p29.WOMY*/ sprite_reset_flags.WOMY_STORE6_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.XOJA_SPRITE6_GETp.out_old());
    /*_p29.WAPO*/ sprite_reset_flags.WAPO_STORE7_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.GUTU_SPRITE7_GETp.out_old());
    /*_p29.EXUQ*/ sprite_reset_flags.EXUQ_STORE8_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.FOXA_SPRITE8_GETp.out_old());
    /*_p29.FONO*/ sprite_reset_flags.FONO_STORE9_RSTp.dff17(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, sprite_match_flags.GUZE_SPRITE9_GETp.out_old());

    SpriteStoreFlags sprite_store_flags_old = sprite_store_flags;

    update_sprite_store_flags_gates(sprite_counter, DYTY_COUNT_CLKp, sprite_store_flags);
    store_sprite_gates(sprite_store_flags_old, sprite_store_flags, sprite_reset_flags, BYVA_LINE_RSTn, sprite_bus, oam_temp_b_old, sprite_store);
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

    /*#p27.NYZE*/ fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17_any(MOXE_AxCxExGx(), XYMU_RENDERINGn.qn_new(), fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any());
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

    get_sprite_match_flags_gates(pix_count, AROR_MATCH_ENp, sprite_store, sprite_match_flags, SIG_GND);
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
  /*#p24.SACU*/ wire SACU_CLKPIPE_new = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  /*_p24.PAHO*/ lcd.PAHO_X_8_SYNC.dff17(ROXO_CLKPIPE_odd, XYMU_RENDERINGn.qn_new(), XYDO_PX3p_old.qp_old());

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  memset(&sprite_bus, BIT_NEW | BIT_PULLED | 1, sizeof(sprite_bus));


  sprite_match_to_bus_gates(sprite_store, sprite_match_flags, sprite_bus);
  sprite_scan_to_bus_gates(sprite_delta_y, XYMU_RENDERINGn, sprite_match_flags.FEPO_STORE_MATCHp);

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // WY/WX/window match

  tock_window_gates(SEGU_CLKPIPE_evn, REPU_VBLANKp);

  //----------------------------------------
  // Tile fetch sequencer

  /*_p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0p_D1.dff17(ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), tile_fetcher.LAXU_BFETCH_S0p.qp_old());

  /*_p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp = nor2(win_reg.RYFA_WIN_FETCHn_A.qn_new(), win_reg.RENE_WIN_FETCHn_B.qp_new());
  /*_p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne = nand2(SYLO_WIN_HITn, win_reg.SOVY_WIN_HITp.qp_new());
  /*_p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne = not1(TUXY_WIN_FIRST_TILEne);
  /*_p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp = or3(SEKO_WIN_FETCH_TRIGp, SUZU_WIN_FIRST_TILEne, TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  /*_p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new(), MOSU_WIN_MODE_TRIGp, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(tile_fetcher.LAXU_BFETCH_S0p.qp_any(), tile_fetcher.NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn);
    /*_p27.LEBO*/ wire LEBO_AxCxExGx = nand2(ALET_xBxDxFxH(), MOCE_BFETCH_DONEn);

    /*_p27.LAXU*/ tile_fetcher.LAXU_BFETCH_S0p.dff17_any(LEBO_AxCxExGx, NYXU_BFETCH_RSTn, tile_fetcher.LAXU_BFETCH_S0p.qn_any());
    /*_p27.MESU*/ tile_fetcher.MESU_BFETCH_S1p.dff17_any(tile_fetcher.LAXU_BFETCH_S0p.qn_any(), NYXU_BFETCH_RSTn, tile_fetcher.MESU_BFETCH_S1p.qn_any());
    /*_p27.NYVA*/ tile_fetcher.NYVA_BFETCH_S2p.dff17_any(tile_fetcher.MESU_BFETCH_S1p.qn_any(), NYXU_BFETCH_RSTn, tile_fetcher.NYVA_BFETCH_S2p.qn_any());
  }

  /*_p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, tile_fetcher.LYRY_BFETCH_DONEp.out_old());

  /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(tile_fetcher.LAXU_BFETCH_S0p.qp_new(), tile_fetcher.NYVA_BFETCH_S2p.qp_new(), NYXU_BFETCH_RSTn);
  /*_p27.LYRY*/ tile_fetcher.LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  /*_p27.LURY*/ wire LURY_BG_FETCH_DONEn = and2(tile_fetcher.LOVY_FETCH_DONEp.qn_new(), XYMU_RENDERINGn.qn_new());
  /*_p27.LONY*/ tile_fetcher.LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, LURY_BG_FETCH_DONEn);


  //----------------------------------------
  // Fine match counter

  /*#p27.PASO*/ wire PASO_FINE_RST = nor2(PAHA_RENDERINGn, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p27.ROZE*/ wire ROZE_FINE_COUNT_7n = nand3(fine_scroll.RUBU_FINE_CNT2.qp_any(), fine_scroll.ROGA_FINE_CNT1.qp_any(), fine_scroll.RYKU_FINE_CNT0.qp_any());
    /*#p27.PECU*/ wire PECU_FINE_CLK = nand2(ROXO_CLKPIPE_odd, ROZE_FINE_COUNT_7n);
    /*#p27.RYKU*/ fine_scroll.RYKU_FINE_CNT0.dff17_any(PECU_FINE_CLK, PASO_FINE_RST, fine_scroll.RYKU_FINE_CNT0.qn_any());
    /*#p27.ROGA*/ fine_scroll.ROGA_FINE_CNT1.dff17_any(fine_scroll.RYKU_FINE_CNT0.qn_any(), PASO_FINE_RST, fine_scroll.ROGA_FINE_CNT1.qn_any());
    /*#p27.RUBU*/ fine_scroll.RUBU_FINE_CNT2.dff17_any(fine_scroll.ROGA_FINE_CNT1.qn_any(), PASO_FINE_RST, fine_scroll.RUBU_FINE_CNT2.qn_any());
  }

  //----------------------------------------
  // PPU / LCD output

  tock_pix_pipes_gates(SACU_CLKPIPE_new, NYXU_BFETCH_RSTn);
  set_lcd_pins_gates(SACU_CLKPIPE_new);

  //----------------------------------------
  // Audio

  tock_spu();

  //----------------------------------------
  // Memory buses

  tock_ext_gates(cart_blob);
  tock_vram_bus_gates(TEVO_WIN_FETCH_TRIGp);
  tock_oam_bus_gates();
  tock_zram_gates();

  //----------------------------------------
  // And finally, interrupts.

  tock_interrupts_gates();

  old_bus = new_bus;
  commit();
}























































































































//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_logic(const blob& cart_blob) {
  uint8_t phase_mask_old = 1 << (7 - ((phase_total + 0) & 7));
  uint8_t phase_mask_new = 1 << (7 - ((phase_total + 1) & 7));

  //----------

  wire SYLO_WIN_HITn_old = not1(win_reg.RYDY_WIN_HITp.out_old());
  wire TOMU_WIN_HITp_old = not1(SYLO_WIN_HITn_old);
  wire SOCY_WIN_HITn_old = not1(TOMU_WIN_HITp_old);
  wire VYBO_CLKPIPE_old = nor3(sprite_match_flags.FEPO_STORE_MATCHp.out_old(), WODU_HBLANKp.out_old(), MYVO_AxCxExGx());
  wire TYFA_CLKPIPE_old = and3(SOCY_WIN_HITn_old, tile_fetcher.POKY_PRELOAD_LATCHp.qp_old(), VYBO_CLKPIPE_old);
  wire SEGU_CLKPIPE_old = not1(TYFA_CLKPIPE_old);
  wire ROXO_CLKPIPE_old = not1(SEGU_CLKPIPE_old);
  wire CLKPIPE_old = or2(SEGU_CLKPIPE_old, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_old());

  wire CLK_AxCxExGx_old = !!(phase_mask_old & 0b10101010);

  wire vid_rst_old = bit(reg_lcdc.XONA_LCDC_LCDENn.state);

  //----------

  wire EXT_vcc = 1;
  wire EXT_gnd = 0;

  SIG_VCC.sig_in(EXT_vcc);
  SIG_GND.sig_in(EXT_gnd);

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

    cpu_signals.SIG_IN_CPU_LATCH_EXT.sig_in(EXT_cpu_latch_ext);

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

    cpu_signals.SIG_IN_CPU_RDp.sig_in(EXT_cpu_rd);
    cpu_signals.SIG_IN_CPU_WRp.sig_in(EXT_cpu_wr);
    cpu_signals.SIG_IN_CPU_EXT_BUSp.sig_in(EXT_addr_new);
  }

  //-----------------------------------------------------------------------------

  {
    wire EXT_sys_clkreq = bit(sys_clkreq);
    wire EXT_sys_rst = bit(~sys_rst);
    wire EXT_sys_t2 = bit(~sys_t2);
    wire EXT_sys_t1 = bit(~sys_t1);

    wire EXT_clkin = !(phase_total & 1) && sys_clken;
    wire EXT_clkgood = bit(~sys_clkgood);

    clk.PIN_74_CLK.pin_clk(EXT_clkin, EXT_clkgood);
    rst.PIN_71_RST.pin_in(EXT_sys_rst);
    rst.PIN_76_T2.pin_in(EXT_sys_t2);
    rst.PIN_77_T1.pin_in(EXT_sys_t1);

    wire EXT_ack_vblank = bit(gb_cpu.int_ack, BIT_VBLANK);
    wire EXT_ack_stat = bit(gb_cpu.int_ack, BIT_STAT);
    wire EXT_ack_timer = bit(gb_cpu.int_ack, BIT_TIMER);
    wire EXT_ack_serial = bit(gb_cpu.int_ack, BIT_SERIAL);
    wire EXT_ack_joypad = bit(gb_cpu.int_ack, BIT_JOYPAD);

    interrupts.SIG_CPU_ACK_VBLANK.sig_in(EXT_ack_vblank);
    interrupts.SIG_CPU_ACK_STAT.sig_in(EXT_ack_stat);
    interrupts.SIG_CPU_ACK_TIMER.sig_in(EXT_ack_timer);
    interrupts.SIG_CPU_ACK_SERIAL.sig_in(EXT_ack_serial);
    interrupts.SIG_CPU_ACK_JOYPAD.sig_in(EXT_ack_joypad);

    clk.SIG_CPU_CLKREQ.sig_in(EXT_sys_clkreq);

    cpu_signals.SIG_CPU_ADDR_HIp.sig_out(new_bus.SYRO_FE00_FFFF());
    cpu_signals.SIG_CPU_UNOR_DBG.sig_out(UNOR_MODE_DBG2p());
    cpu_signals.SIG_CPU_UMUT_DBG.sig_out(UMUT_MODE_DBG1p());
  }

  //-----------------------------------------------------------------------------
  // Sys clock signals

  tock_clocks_gates();

  wire UJYV_CPU_RDn = not1(cpu_signals.SIG_IN_CPU_RDp.out_new());
  cpu_signals.TEDO_CPU_RDp = not1(UJYV_CPU_RDn);

  wire AFAS_xxxxEFGx = nor2(ADAR_ABCxxxxH(), ATYP_ABCDxxxx());
  wire AREV_CPU_WRn = nand2(cpu_signals.SIG_IN_CPU_WRp.out_new(), AFAS_xxxxEFGx);
  cpu_signals.APOV_CPU_WRp = not1(AREV_CPU_WRn);

  wire UBAL_CPU_WRn = not1(cpu_signals.APOV_CPU_WRp.out_new());
  cpu_signals.TAPU_CPU_WRp = not1(UBAL_CPU_WRn); // xxxxEFGx

  wire AGUT_xxCDEFGH = or_and3(AROV_xxCDEFxx(), AJAX_xxxxEFGH(), cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new());
  wire AWOD_ABxxxxxx = nor2(UNOR_MODE_DBG2p(), AGUT_xxCDEFGH);
  cpu_signals.ABUZ_EXT_RAM_CS_CLK = not1(AWOD_ABxxxxxx);




















  /////
  uint16_t cpu_addr = (uint16_t)pack(16, &new_bus.BUS_CPU_A00p);

  bool cpu_rd = bit(cpu_signals.SIG_IN_CPU_RDp.state);
  bool cpu_wr = bit(cpu_signals.SIG_IN_CPU_WRp.state);

  //-----------------------------------------------------------------------------

  clk.PIN_74_CLK.pin_clk(!(phase_total & 1) && sys_clken, 0);
  rst.PIN_71_RST.state = 0;
  rst.PIN_76_T2.state = 0;
  rst.PIN_77_T1.state = 0;

  interrupts.SIG_CPU_ACK_VBLANK.state = bit(gb_cpu.int_ack, BIT_VBLANK);
  interrupts.SIG_CPU_ACK_STAT.state   = bit(gb_cpu.int_ack, BIT_STAT);
  interrupts.SIG_CPU_ACK_TIMER.state  = bit(gb_cpu.int_ack, BIT_TIMER);
  interrupts.SIG_CPU_ACK_SERIAL.state = bit(gb_cpu.int_ack, BIT_SERIAL);
  interrupts.SIG_CPU_ACK_JOYPAD.state = bit(gb_cpu.int_ack, BIT_JOYPAD);

  clk.SIG_CPU_CLKREQ.sig_in(1);

  cpu_signals.SIG_CPU_ADDR_HIp.sig_out(cpu_addr >= 0xFE00 && cpu_addr <= 0xFFFF);
  cpu_signals.SIG_CPU_UNOR_DBG.sig_out(0);
  cpu_signals.SIG_CPU_UMUT_DBG.sig_out(0);

  //-----------------------------------------------------------------------------
  // Sys clock signals

  wire c = bit(clk.PIN_74_CLK.CLK.state);

  clk.PIN_73_CLK_DRIVE.pin_out(c, c);

  clk.AVET_DEGLITCH = bit(c);
  clk.ANOS_DEGLITCH = bit(~c);

  wire CLK_AxxDExxH = gen_clk_new(0b10011001);
  wire CLK_Axxxxxxx = gen_clk_new(0b10000000);
  wire CLK_xBCDEFGH = gen_clk_new(0b01111111);
  wire CLK_ABCDxxxx = gen_clk_new(0b11110000);
  wire CLK_xxxxEFGH = gen_clk_new(0b00001111);
  wire CLK_ABCDEFxx = gen_clk_new(0b11111100);
  wire CLK_AxxxxxGH = gen_clk_new(0b10000011);
  wire CLK_AxxxxFGH = gen_clk_new(0b10000111);
  wire CLK_ABxxxxGH = gen_clk_new(0b11000011);
  wire CLK_ABCxxxxH = gen_clk_new(0b11100001);
  wire CLK_xBCxxFGx = gen_clk_new(0b01100110);
  wire CLK_xBCDxFGH = gen_clk_new(0b01110111);
  wire CLK_xxxxEFGx = gen_clk_new(0b00001110);
  wire CLK_xxCDEFGH = gen_clk_new(0b00111111);
  wire CLK_xBxDxFxH = gen_clk_new(0b01010101);
  wire CLK_AxCxExGx = gen_clk_new(0b10101010);
  wire CLK_ABxxEFxx = gen_clk_new(0b11001100);
  wire CLK_xxCDxxGH = gen_clk_new(0b00110011);
  wire CLK_xxCDEFxx = gen_clk_new(0b00111100);

  clk.AFUR_xxxxEFGH.state = CLK_xxxxEFGH;
  clk.ALEF_AxxxxFGH.state = CLK_AxxxxFGH;
  clk.APUK_ABxxxxGH.state = CLK_ABxxxxGH;
  clk.ADYK_ABCxxxxH.state = CLK_ABCxxxxH;

  clk.PIN_75_CLK_OUT.pin_out(CLK_xxxxEFGH, CLK_xxxxEFGH);

  clk.SIG_CPU_BOWA_Axxxxxxx.sig_out(CLK_Axxxxxxx);
  clk.SIG_CPU_BEDO_xBCDEFGH.sig_out(CLK_xBCDEFGH);
  clk.SIG_CPU_BEKO_ABCDxxxx.sig_out(CLK_ABCDxxxx);
  clk.SIG_CPU_BUDE_xxxxEFGH.sig_out(CLK_xxxxEFGH);
  clk.SIG_CPU_BOLO_ABCDEFxx.sig_out(CLK_ABCDEFxx);
  clk.SIG_CPU_BUKE_AxxxxxGH.sig_out(CLK_AxxxxxGH);
  clk.SIG_CPU_BOMA_xBCDEFGH.sig_out(CLK_xBCDEFGH);
  clk.SIG_CPU_BOGA_Axxxxxxx.sig_out(CLK_Axxxxxxx);


  cpu_signals.TEDO_CPU_RDp = cpu_rd;
  cpu_signals.APOV_CPU_WRp = CLK_xxxxEFGx && cpu_wr;
  cpu_signals.TAPU_CPU_WRp = cpu_signals.APOV_CPU_WRp.state;
  cpu_signals.ABUZ_EXT_RAM_CS_CLK = CLK_xxCDEFGH & cpu_signals.SIG_IN_CPU_EXT_BUSp.state;

  auto new_addr = pack(16, (BitBase*)&new_bus.BUS_CPU_A00p);

  if (DELTA_HA) {
    auto div_old = pack(16, &div.UKUP_DIV00p);
    unpack(div_old + 1, 16, &div.UKUP_DIV00p);
  }

  if (cpu_wr && new_addr == 0xFF04 && CLK_xxxxEFGx) {
    memset(&div.UKUP_DIV00p, 0, 16);
  }

  if (cpu_rd && new_addr == 0xFF04) {
    memcpy(&new_bus.BUS_CPU_D00p, &div.UGOT_DIV06p, 8);
  }

  // In logic mode we don't care about the power-on behavior, we only want behavior to match when running code. So, we set
  // this stuff to zeroes.

  rst.AFER_SYS_RSTp.state = 0;
  rst.TUBO_WAITINGp.state = 0;
  rst.ASOL_POR_DONEn.state = 0;
  rst.SIG_CPU_EXT_CLKGOOD.sig_out(1);
  rst.SIG_CPU_EXT_RESETp.sig_out(0);
  rst.SIG_CPU_STARTp.sig_out(0);
  rst.SIG_CPU_INT_RESETp.sig_out(0);
  rst.SOTO_DBG_VRAMp.state = 0;

  // LCDC has to be near the top as it controls the video reset signal

  if (cpu_wr && new_addr == 0xFF40 && DELTA_GH) {
    cpy_inv(&reg_lcdc.VYXE_LCDC_BGENn, &old_bus.BUS_CPU_D00p, 8);
  }

  if (cpu_rd && (new_addr == 0xFF40)) {
    cpy_inv(&new_bus.BUS_CPU_D00p, &reg_lcdc.VYXE_LCDC_BGENn, 8);
  }

  wire vid_rst_new = bit(reg_lcdc.XONA_LCDC_LCDENn.state);

  wire WOSU_AxxDExxH_old = !vid_rst_old && gen_clk_old(0b10011001);
  wire WUVU_ABxxEFxx_old = !vid_rst_old && gen_clk_old(0b11001100);
  wire VENA_xxCDEFxx_old = !vid_rst_old && gen_clk_old(0b00111100);

  clk.WOSU_AxxDExxH.state = !vid_rst_new && gen_clk_new(0b10011001);
  clk.WUVU_ABxxEFxx.state = !vid_rst_new && gen_clk_new(0b11001100);
  clk.VENA_xxCDEFxx.state = !vid_rst_new && gen_clk_new(0b00111100);

  if (cpu_rd && (new_addr == 0xFF45)) {
    cpy_inv(&new_bus.BUS_CPU_D00p, &reg_lyc.SYRY_LYC0n, 8);
  }

  if (cpu_wr && (new_addr == 0xFF45 && DELTA_GH)) {
    cpy_inv(&reg_lyc.SYRY_LYC0n, &old_bus.BUS_CPU_D00p, 8);
  }

  bool lcd_on = !bit(reg_lcdc.XONA_LCDC_LCDENn.state);

  if (lcd_on && DELTA_BC) {
    auto ly = pack(8, &reg_ly.MUWY_LY0p);
    auto lyc = pack_inv(8, &reg_lyc.SYRY_LYC0n);
    reg_lyc.ROPO_LY_MATCH_SYNCp.state = ly == lyc;
  }

  // FIXME this seems slightly wrong...
  if (bit(cpu_wr) && CLK_xxxxEFGx && new_addr == 0xFF41) {
  }
  else {
    reg_stat.RUPO_LYC_MATCHn.state = 1;
  }

  // but the "reset" arm of the latch overrides the "set" arm, so it doesn't completely break?
  if (bit(reg_lyc.ROPO_LY_MATCH_SYNCp.state)) {
    reg_stat.RUPO_LYC_MATCHn.state = 0;
  }

  //----------------------------------------

  auto lx_old = pack(7, &reg_lx.SAXO_LX0p);
  auto ly_old = pack(8, &reg_ly.MUWY_LY0p);

  if (!lcd_on) {
    lcd.ANEL_x113p.state = 0;
    lcd.CATU_x113p.state = 0;
    lcd.NYPE_x113p.state = 0;
    lcd.RUTU_x113p.state = 0;
    lcd.POPU_y144p.state = 0;
    lcd.MYTA_y153p.state = 0;
    lcd.SYGU_LINE_STROBE.state = 0;

    ATEJ_LINE_RSTp = 1;

    clear(7, &reg_lx.SAXO_LX0p);
    clear(8, &reg_ly.MUWY_LY0p);
  }
  else {
    wire ly_144 = (ly_old & 144) == 144;
    wire ly_153 = (ly_old & 153) == 153;

    if (DELTA_HA) {
      lcd.CATU_x113p.state = and2(lcd.RUTU_x113p.qp_old(), ~ly_144);
    }

    if (DELTA_BC) {
      wire nype_old = bit(lcd.NYPE_x113p.state);

      lcd.ANEL_x113p.state = lcd.CATU_x113p.state;
      lcd.NYPE_x113p.state = lcd.RUTU_x113p.state;

      wire nype_new = bit(lcd.NYPE_x113p.state);
      if (!nype_old && nype_new) {
        lcd.POPU_y144p.state = ly_144;
        lcd.MYTA_y153p.state = ly_153;
      }

      unpack(lx_old + 1, 7, &reg_lx.SAXO_LX0p);
    }

    if (DELTA_DE) {
      lcd.CATU_x113p.state = and2(lcd.RUTU_x113p.qp_old(), ~ly_144);
    }

    if (DELTA_FG) {
      wire rutu_old = bit(lcd.RUTU_x113p.state);

      lcd.ANEL_x113p.state = lcd.CATU_x113p.state;
      lcd.RUTU_x113p.state = lx_old == 113;

      wire rutu_new = bit(lcd.RUTU_x113p.state);
      if (!rutu_old && rutu_new) {
        unpack(ly_old + 1, 8, &reg_ly.MUWY_LY0p);
      }

      wire strobe = (lx_old == 0) || (lx_old == 7) || (lx_old == 45) || (lx_old == 83);
      lcd.SYGU_LINE_STROBE.state = strobe;
    }

    ATEJ_LINE_RSTp = nor2(lcd.ANEL_x113p.state, ~lcd.CATU_x113p.state);
    if (bit(lcd.RUTU_x113p.state)) clear(7, &reg_lx.SAXO_LX0p);
    if (bit(lcd.MYTA_y153p.state)) clear(8, &reg_ly.MUWY_LY0p);
  }

  if (cpu_rd && (new_addr == 0xFF44)) {
    memcpy(&new_bus.BUS_CPU_D00p, &reg_ly.MUWY_LY0p, 8);
  }

  auto lx_new = pack(7, &reg_lx.SAXO_LX0p);
  auto ly_new = pack(8, &reg_ly.MUWY_LY0p);

  tock_joypad_logic();
  tock_serial_logic();
  tock_timer_logic();
  tock_bootrom_logic();
  tock_dma_logic();

  //----------------------------------------
  // Sprite scanner

  if (DELTA_EVEN) {
    sprite_scanner.DOBA_SCAN_DONE_Bp.state = sprite_scanner.BYBA_SCAN_DONE_Ap.state;
  }

  if (DELTA_DE || DELTA_HA) {
    sprite_scanner.BYBA_SCAN_DONE_Ap.state = sprite_scanner.FETO_SCAN_DONEp.state;
    sprite_scanner.CENO_SCANNINGn.state = sprite_scanner.BESU_SCANNINGn.state;
    cpy_inv(&sprite_scanner.XADU_SPRITE_IDX0p, &oam_bus.BUS_OAM_A02n, 6);

    auto scan_count_old = pack(6, &sprite_scanner.YFEL_SCAN0);
    if (scan_count_old < 39) {
      unpack(scan_count_old + 1, 6, &sprite_scanner.YFEL_SCAN0);
    }
  }

  sprite_scanner.AVAP_SCAN_DONE_TRIGp = nor2(sprite_scanner.DOBA_SCAN_DONE_Bp.state, ~sprite_scanner.BYBA_SCAN_DONE_Ap.state);

  if (bit(lcd.CATU_x113p.state)) sprite_scanner.BESU_SCANNINGn.state = 1;
  if (bit(sprite_scanner.AVAP_SCAN_DONE_TRIGp.state)) sprite_scanner.BESU_SCANNINGn.state = 0;

  sprite_scanner.ACYL_SCANNINGp = and2(~dma.MATU_DMA_RUNNINGp.state, sprite_scanner.BESU_SCANNINGn.state);
  sprite_scanner.FETO_SCAN_DONEp = pack(6, &sprite_scanner.YFEL_SCAN0) == 39;


  if (bit(ATEJ_LINE_RSTp.state)) {
    sprite_scanner.DOBA_SCAN_DONE_Bp.rst();
    sprite_scanner.BYBA_SCAN_DONE_Ap.rst();
    sprite_scanner.AVAP_SCAN_DONE_TRIGp = 0;

    sprite_scanner.BESU_SCANNINGn.state = 1;
    sprite_scanner.ACYL_SCANNINGp = ~dma.MATU_DMA_RUNNINGp.state;
    clear(6, &sprite_scanner.YFEL_SCAN0);
    sprite_scanner.FETO_SCAN_DONEp = 0;
  }


  if (vid_rst_new) {
    sprite_scanner.DOBA_SCAN_DONE_Bp.state = 0;
    sprite_scanner.BYBA_SCAN_DONE_Ap.state = 0;
    sprite_scanner.AVAP_SCAN_DONE_TRIGp = 0;
    sprite_scanner.CENO_SCANNINGn.state = 0;
    sprite_scanner.BESU_SCANNINGn.state = 0;
    sprite_scanner.ACYL_SCANNINGp = 0;
    clear(6, &sprite_scanner.YFEL_SCAN0);
    sprite_scanner.FETO_SCAN_DONEp = 0;
  }

  //----------------------------------------
  // Global rendering flag 'XYMU'

  if (DELTA_EVEN) {
    VOGA_HBLANKp.state = WODU_HBLANKp.state;
  }


  if (ATEJ_LINE_RSTp.state) {
    VOGA_HBLANKp.rst();
  }

  bool rendering_old = !bit(XYMU_RENDERINGn.state);

  if (vid_rst_new) XYMU_RENDERINGn.state = 1;
  if (bit(VOGA_HBLANKp.state)) XYMU_RENDERINGn.state = 1;
  if (bit(sprite_scanner.AVAP_SCAN_DONE_TRIGp.state)) XYMU_RENDERINGn.state = 0;

  bool rendering_new = !bit(XYMU_RENDERINGn.state);

  //----------------------------------------
  // Sprite fetch sequencer

  auto sfetch_phase_old = pack(3, &sprite_fetcher.TOXE_SFETCH_S0p);
  auto tyfo_old = sprite_fetcher.TYFO_SFETCH_S0p_D1.state;

  if (DELTA_EVEN) {
    sprite_fetcher.SOBU_SFETCH_REQp.  state = and4(sprite_match_flags.FEPO_STORE_MATCHp.state, ~TOMU_WIN_HITp_old, tile_fetcher.LYRY_BFETCH_DONEp.state, ~sprite_fetcher.TAKA_SFETCH_RUNNINGp.state);
    sprite_fetcher.VONU_SFETCH_S1p_D4.state = sprite_fetcher.TOBU_SFETCH_S1p_D2.state;
    sprite_fetcher.TOBU_SFETCH_S1p_D2.state = sprite_fetcher.TULY_SFETCH_S1p.state;

    if (sfetch_phase_old != 5) {
      unpack(sfetch_phase_old + 1, 3, &sprite_fetcher.TOXE_SFETCH_S0p);
    }

    if (bit(and2(sprite_fetcher.SOBU_SFETCH_REQp.state, ~sprite_fetcher.SUDA_SFETCH_REQp.state))) {
      clear(3, &sprite_fetcher.TOXE_SFETCH_S0p);
    }
  }
  else {
    sprite_fetcher.SUDA_SFETCH_REQp  .state = sprite_fetcher.SOBU_SFETCH_REQp.state;
    sprite_fetcher.TYFO_SFETCH_S0p_D1.state = sprite_fetcher.TOXE_SFETCH_S0p.state;
    sprite_fetcher.SEBA_SFETCH_S1p_D5.state = sprite_fetcher.VONU_SFETCH_S1p_D4.state;
  }


  if (vid_rst_new || bit(ATEJ_LINE_RSTp.state)) {
    clear(3, &sprite_fetcher.TOXE_SFETCH_S0p);
  }

  if (!rendering_new) {
    sprite_fetcher.TOBU_SFETCH_S1p_D2.rst();
    sprite_fetcher.VONU_SFETCH_S1p_D4.rst();
    sprite_fetcher.SEBA_SFETCH_S1p_D5.rst();
  }

  auto sfetch_phase_new = pack(3, &sprite_fetcher.TOXE_SFETCH_S0p);
  auto tyfo_new = sprite_fetcher.TYFO_SFETCH_S0p_D1.state;

  sprite_fetcher.TEXY_SFETCHINGp = and2(or2(sprite_fetcher.TULY_SFETCH_S1p.state, sprite_fetcher.VONU_SFETCH_S1p_D4.state), rendering_new);

  wire WUTY_SFETCH_DONE_TRIGp_old = sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.state;

  sprite_fetcher.WUTY_SFETCH_DONE_TRIGp = nor4(~sprite_fetcher.TYFO_SFETCH_S0p_D1.state, ~sprite_fetcher.TOXE_SFETCH_S0p.state, ~sprite_fetcher.SEBA_SFETCH_S1p_D5.state, ~sprite_fetcher.VONU_SFETCH_S1p_D4.state);

  wire WUTY_SFETCH_DONE_TRIGp_new = sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.state;

  wire RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
  wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, vid_rst_new, ATEJ_LINE_RSTp.out_new());

  //----------------------------------------
  // Window state has some interaction with the tile fetcher here.

  if (DELTA_ODD) {
    win_reg.NUNU_WIN_MATCHp.state = win_reg.PYCO_WIN_MATCHp.state;
  }

  if (DELTA_EVEN) {
    win_reg.NOPA_WIN_MODE_Bp.state = win_reg.PYNU_WIN_MODE_Ap.state;
  }


  if (vid_rst_new) {
    win_reg.NUNU_WIN_MATCHp.rst();
    win_reg.NOPA_WIN_MODE_Bp.rst();
  }

  if (bit(win_reg.NUNU_WIN_MATCHp.state)) {
    win_reg.PYNU_WIN_MODE_Ap.set();
  }

  if (bit(nand3(~reg_lcdc.WYMO_LCDC_WINENn.state, ~ATEJ_LINE_RSTp.state, !vid_rst_new))) {
    win_reg.PYNU_WIN_MODE_Ap.rst();
  }


  if (!rendering_new) {
    tile_fetcher.PYGO_FETCH_DONEp.rst();
  }

  if (!bit(nor2(and2(win_reg.PYNU_WIN_MODE_Ap.state, ~win_reg.NOPA_WIN_MODE_Bp.state), !rendering_new))) {
    tile_fetcher.PORY_FETCH_DONEp.rst();
    tile_fetcher.NYKA_FETCH_DONEp.rst();
  }

  if (DELTA_EVEN) {
    tile_fetcher.PYGO_FETCH_DONEp.state = tile_fetcher.PORY_FETCH_DONEp.state;
    tile_fetcher.NYKA_FETCH_DONEp.state = tile_fetcher.LYRY_BFETCH_DONEp.state;
  }
  else {
    tile_fetcher.PORY_FETCH_DONEp.state = tile_fetcher.NYKA_FETCH_DONEp.state;
  }

  if (!rendering_new) {
    tile_fetcher.PYGO_FETCH_DONEp.rst();
  }

  if (!bit(nor2(and2(win_reg.PYNU_WIN_MODE_Ap.state, ~win_reg.NOPA_WIN_MODE_Bp.state), !rendering_new))) {
    tile_fetcher.PORY_FETCH_DONEp.rst();
    tile_fetcher.NYKA_FETCH_DONEp.rst();
  }


  tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.state, !rendering_new);

  win_reg.SOVY_WIN_HITp.dff17(CLK_xBxDxFxH, !vid_rst_new, win_reg.RYDY_WIN_HITp.out_old());


  if (vid_rst_new || bit(tile_fetcher.PORY_FETCH_DONEp.state)) {
    win_reg.RYDY_WIN_HITp = 0;
    win_reg.PUKU_WIN_HITn = 1;
  }
  else if (bit(and2(win_reg.PYNU_WIN_MODE_Ap.state, ~win_reg.NOPA_WIN_MODE_Bp.state))) {
    win_reg.RYDY_WIN_HITp = 1;
    win_reg.PUKU_WIN_HITn = 0;
  }


  if (bit(and2(sprite_fetcher.SOBU_SFETCH_REQp.state, ~sprite_fetcher.SUDA_SFETCH_REQp.state)) || vid_rst_new || bit(ATEJ_LINE_RSTp.state)) {
    sprite_fetcher.TAKA_SFETCH_RUNNINGp.set();
  }

  if (bit(and4(rendering_new, ~tile_fetcher.POKY_PRELOAD_LATCHp.state, tile_fetcher.NYKA_FETCH_DONEp.state, tile_fetcher.PORY_FETCH_DONEp.state))) {
    sprite_fetcher.TAKA_SFETCH_RUNNINGp.rst();
  }

  if (bit(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.state)) {
    sprite_fetcher.TAKA_SFETCH_RUNNINGp.rst();
  }



  //----------------------------------------
  // OAM temp register B stores sprite X coordinate during scan, so we need a copy of it for later.

  auto oam_temp_b_old = oam_temp_b;

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  // This chunk is weird.

  bool scanning = bit(sprite_scanner.ACYL_SCANNINGp.state);

  wire AVER_AxxxExxx = or3(!scanning, vid_rst_new, ~CLK_xBCDxFGH);
  wire temp = nand4(rendering_new, ~sprite_fetcher.TULY_SFETCH_S1p.state, ~sprite_fetcher.TESE_SFETCH_S2p.state, nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.state, ~sprite_fetcher.TOXE_SFETCH_S0p.state));

  if (cpu_addr >= 0xFE00 && cpu_addr <= 0xFEFF) {
    wire BYCU_OAM_CLKp = nand3(AVER_AxxxExxx, temp, ~CLK_xxxxEFGH);
    oam_latch_to_temp_a(~BYCU_OAM_CLKp, oam_latch_a, oam_temp_a);
    oam_latch_to_temp_b(~BYCU_OAM_CLKp, oam_latch_b, oam_temp_b);
  }
  else if (bit(dma.MATU_DMA_RUNNINGp.state)) {
    wire BYCU_OAM_CLKp = nand3(AVER_AxxxExxx, temp, ~CLK_xxxxEFGH);
    oam_latch_to_temp_a(~BYCU_OAM_CLKp, oam_latch_a, oam_temp_a);
    oam_latch_to_temp_b(~BYCU_OAM_CLKp, oam_latch_b, oam_temp_b);
  }
  else {
    wire BYCU_OAM_CLKp = nand3(AVER_AxxxExxx, temp, 1);
    oam_latch_to_temp_a(~BYCU_OAM_CLKp, oam_latch_a, oam_temp_a);
    oam_latch_to_temp_b(~BYCU_OAM_CLKp, oam_latch_b, oam_temp_b);
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  SpriteDeltaY sprite_delta_y = sub_sprite_y_logic();

  {
    wire GOVU_SPSIZE_MATCH = or2(~reg_lcdc.XYMO_LCDC_SPSIZEn.state, sprite_delta_y.GYKY_YDIFF3.sum);

    wire WOTA_SCAN_MATCH_Yn = nand6(sprite_delta_y.GACE_SPRITE_DELTA4,
      sprite_delta_y.GUVU_SPRITE_DELTA5,
      sprite_delta_y.GYDA_SPRITE_DELTA6,
      sprite_delta_y.GEWY_SPRITE_DELTA7,
      sprite_delta_y.WUHU_YDIFF7.carry,
      GOVU_SPSIZE_MATCH);

    wire DEZY_COUNT_CLKp_old = bit(sprite_counter.DEZY_COUNT_CLKp.state);

    if (vid_rst_new || bit(ATEJ_LINE_RSTp.state)) {
      if (vid_rst_new) {
        sprite_counter.DEZY_COUNT_CLKp.rst();
      }
      else {
        sprite_counter.DEZY_COUNT_CLKp.dff_pp(CLK_AxCxExGx, 1);
      }

      clear(4, &sprite_counter.BESE_SPRITE_COUNT0);
      clear(10, &sprite_reset_flags.EBOJ_STORE0_RSTp);

      memset(&sprite_store_flags, 0, 10);

      for (int i = 0; i < 10; i++) {
        set(8, &((BitBase*)&sprite_store)[18 * i + 10]);
      }
    }
    else {
      auto ssf_clk = nand4(!0, CLK_xBCxxFGx, sprite_scanner.CENO_SCANNINGn.state, ~WOTA_SCAN_MATCH_Yn);

      sprite_counter.DEZY_COUNT_CLKp.dff_pp(CLK_AxCxExGx, ssf_clk);

      wire DEZY_COUNT_CLKp_new = bit(sprite_counter.DEZY_COUNT_CLKp.state);

      auto sprite_count_old = pack(4, &sprite_counter.BESE_SPRITE_COUNT0);

      if (!bit(DEZY_COUNT_CLKp_old) && bit(DEZY_COUNT_CLKp_new)) {
        if (sprite_count_old != 10) {
          unpack(sprite_count_old + 1, 4, &sprite_counter.BESE_SPRITE_COUNT0);
        }
      }

      auto sprite_count_new = pack(4, &sprite_counter.BESE_SPRITE_COUNT0);

      if (!bit(WUTY_SFETCH_DONE_TRIGp_old) && bit(WUTY_SFETCH_DONE_TRIGp_new)) {
        memcpy(&sprite_reset_flags.EBOJ_STORE0_RSTp, &sprite_match_flags.GUVA_SPRITE0_GETp, 10);
      }

      auto store_rst = pack(10, &sprite_reset_flags.EBOJ_STORE0_RSTp);

      auto store_clk_old = pack_inv(10, (BitBase*)&sprite_store_flags.DYHU_STORE0_CLKn);
      for (int i = 0; i < 10; i++) {
        (&sprite_store_flags.DYHU_STORE0_CLKn)[i] = (i == (int)sprite_count_new) && !bit(ssf_clk);
      }
      auto store_clk_new = pack_inv(10, (BitBase*)&sprite_store_flags.DYHU_STORE0_CLKn);

      auto store_clk_pe = (~store_clk_old) & store_clk_new;
      auto store_clk_ne = store_clk_old & (~store_clk_new);
      BitBase* store_regs = (BitBase*)&sprite_store;

      for (int i = 0; i < 10; i++) {
        if (bit(store_clk_ne, i)) cpy_inv(&store_regs[18 * i + 0], &sprite_bus.BUS_SPR_I0, 10);
        if (bit(store_clk_pe, i)) cpy(&store_regs[18 * i + 10], &oam_temp_b.YLOR_OAM_DB0p.state, 8);
        if (bit(store_rst, i))    set(8, &store_regs[18 * i + 10]);
      }
    }

  }












  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  auto XYDO_PX3p_old = pix_count.XYDO_PX3p;
  auto scx_old = pack_inv(3, &reg_scx.DATY_SCX0n);
  auto fine_cnt_old = pack(3, &fine_scroll.RYKU_FINE_CNT0);
  wire fine_match_old = bit(fine_scroll.ROXY_FINE_SCROLL_DONEn.state) && (scx_old == fine_cnt_old);

  wire clkpipe_en = 1;

  if (bit(win_reg.RYDY_WIN_HITp.state)) clkpipe_en = 0;
  if (!bit(tile_fetcher.POKY_PRELOAD_LATCHp.state)) clkpipe_en = 0;
  if (bit(sprite_match_flags.FEPO_STORE_MATCHp.state)) clkpipe_en = 0;
  if (bit(WODU_HBLANKp.state)) clkpipe_en = 0;

  if (DELTA_EVEN) {
    if (bit(clkpipe_en)) {
      fine_scroll.PUXA_SCX_FINE_MATCH_A.state = fine_match_old;
    }
  }
  else {
    fine_scroll.NYZE_SCX_FINE_MATCH_B.state = fine_scroll.PUXA_SCX_FINE_MATCH_A.state;
  }

  if (!rendering_new) {
    fine_scroll.ROXY_FINE_SCROLL_DONEn.set();
    fine_scroll.NYZE_SCX_FINE_MATCH_B.rst();
    fine_scroll.PUXA_SCX_FINE_MATCH_A.rst();
  }

  if (bit(and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.state, ~fine_scroll.NYZE_SCX_FINE_MATCH_B.state))) {
    fine_scroll.ROXY_FINE_SCROLL_DONEn.rst();
  }

  wire CLKPIPE_new = CLK_AxCxExGx;
  if (!bit(clkpipe_en)) CLKPIPE_new = 1;
  if (bit(fine_scroll.ROXY_FINE_SCROLL_DONEn.state)) CLKPIPE_new = 1;

  wire px_old = (uint8_t)pack(8, &pix_count.XEHO_PX0p);

  if (!bit(CLKPIPE_old) && bit(CLKPIPE_new)) {
    unpack(px_old + 1, 8, &pix_count.XEHO_PX0p);
  }

  if (!bit(nor2(ATEJ_LINE_RSTp.state, vid_rst_new))) {
    clear(8, &pix_count.XEHO_PX0p);
  }

  wire AROR_MATCH_ENp = and3(rendering_new, ~sprite_scanner.CENO_SCANNINGn.state, ~reg_lcdc.XYLO_LCDC_SPENn.state);

  if (!AROR_MATCH_ENp) {
    memset(&sprite_match_flags, 0, sizeof(sprite_match_flags));
  }
  else {
    uint8_t px = (uint8_t)pack(8, &pix_count.XEHO_PX0p);

    bool M0 = px == pack(8, &sprite_store.XEPE_STORE0_X0p);
    bool M1 = px == pack(8, &sprite_store.DANY_STORE1_X0p);
    bool M2 = px == pack(8, &sprite_store.FOKA_STORE2_X0p);
    bool M3 = px == pack(8, &sprite_store.XOLY_STORE3_X0p);
    bool M4 = px == pack(8, &sprite_store.WEDU_STORE4_X0p);
    bool M5 = px == pack(8, &sprite_store.FUSA_STORE5_X0p);
    bool M6 = px == pack(8, &sprite_store.YCOL_STORE6_X0p);
    bool M7 = px == pack(8, &sprite_store.ERAZ_STORE7_X0p);
    bool M8 = px == pack(8, &sprite_store.EZUF_STORE8_X0p);
    bool M9 = px == pack(8, &sprite_store.XUVY_STORE9_X0p);

    sprite_match_flags.FEPO_STORE_MATCHp = M0 | M1 | M2 | M3 | M4 | M5 | M6 | M7 | M8 | M9;

    sprite_match_flags.GUVA_SPRITE0_GETp = M0 & !(0);
    sprite_match_flags.ENUT_SPRITE1_GETp = M1 & !(M0);
    sprite_match_flags.EMOL_SPRITE2_GETp = M2 & !(M0 | M1);
    sprite_match_flags.GYFY_SPRITE3_GETp = M3 & !(M0 | M1 | M2);
    sprite_match_flags.GONO_SPRITE4_GETp = M4 & !(M0 | M1 | M2 | M3);
    sprite_match_flags.GEGA_SPRITE5_GETp = M5 & !(M0 | M1 | M2 | M3 | M4);
    sprite_match_flags.XOJA_SPRITE6_GETp = M6 & !(M0 | M1 | M2 | M3 | M4 | M5);
    sprite_match_flags.GUTU_SPRITE7_GETp = M7 & !(M0 | M1 | M2 | M3 | M4 | M5 | M6);
    sprite_match_flags.FOXA_SPRITE8_GETp = M8 & !(M0 | M1 | M2 | M3 | M4 | M5 | M6 | M7);
    sprite_match_flags.GUZE_SPRITE9_GETp = M9 & !(M0 | M1 | M2 | M3 | M4 | M5 | M6 | M7 | M8);
  }


  auto px_new = pack(8, &pix_count.XEHO_PX0p);

  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  WODU_HBLANKp = and2(~sprite_match_flags.FEPO_STORE_MATCHp.state, (px_new & 167) == 167); // WODU goes high on odd, cleared on H

  lcd.PAHO_X_8_SYNC.dff17(and2(clkpipe_en, CLK_xBxDxFxH), rendering_new, XYDO_PX3p_old.state);

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  memset(&sprite_bus, BIT_NEW | BIT_PULLED | 1, sizeof(sprite_bus));

  if (bit(sprite_match_flags.GUVA_SPRITE0_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.YGUS_STORE0_I0n, 10);
  if (bit(sprite_match_flags.ENUT_SPRITE1_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.CADU_STORE1_I0n, 10);
  if (bit(sprite_match_flags.EMOL_SPRITE2_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.BUHE_STORE2_I0n, 10);
  if (bit(sprite_match_flags.GYFY_SPRITE3_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.DEVY_STORE3_I0n, 10);
  if (bit(sprite_match_flags.GONO_SPRITE4_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.XAVE_STORE4_I0n, 10);
  if (bit(sprite_match_flags.GEGA_SPRITE5_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.EKOP_STORE5_I0n, 10);
  if (bit(sprite_match_flags.XOJA_SPRITE6_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.GABO_STORE6_I0n, 10);
  if (bit(sprite_match_flags.GUTU_SPRITE7_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.GULE_STORE7_I0n, 10);
  if (bit(sprite_match_flags.FOXA_SPRITE8_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.AXUV_STORE8_I0n, 10);
  if (bit(sprite_match_flags.GUZE_SPRITE9_GETp.state)) cpy_inv(&sprite_bus.BUS_SPR_I0, &sprite_store.YBER_STORE9_I0n, 10);

  if (bit(or2(sprite_scanner.CENO_SCANNINGn.state, XYMU_RENDERINGn.state))) {
    memcpy(&sprite_bus.BUS_SPR_I0, &sprite_scanner.XADU_SPRITE_IDX0p, 6);
  }

  if (!bit(sprite_match_flags.FEPO_STORE_MATCHp.state)) {
    cpy_inv(&sprite_bus.BUS_SPR_L0, &sprite_delta_y.DEGE_SPRITE_DELTA0, 4);
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // WY/WX/window match

  win_reg.PYCO_WIN_MATCHp.dff17(~CLKPIPE_new, 1, win_reg.NUKO_WX_MATCHp.state);

  wire PANY_WIN_FETCHn_old = and4(~win_reg.NUKO_WX_MATCHp.state, fine_scroll.RUBU_FINE_CNT2.state, fine_scroll.ROGA_FINE_CNT1.state, fine_scroll.RYKU_FINE_CNT0.state);

  win_reg.RENE_WIN_FETCHn_B.dff17(ALET_xBxDxFxH(),  1, win_reg.RYFA_WIN_FETCHn_A.state);
  win_reg.RYFA_WIN_FETCHn_A.dff17(CLKPIPE_new, 1, PANY_WIN_FETCHn_old);

  if (bit(XYMU_RENDERINGn.state)) {
    win_reg.RENE_WIN_FETCHn_B.rst();
    win_reg.RYFA_WIN_FETCHn_A.rst();
  }

  if (cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
    if (new_addr == 0xFF4A) cpy_inv(&reg_wy.NESO_WY0n, &old_bus.BUS_CPU_D00p, 8);
    if (new_addr == 0xFF4B) cpy_inv(&reg_wx.MYPA_WX0n, &old_bus.BUS_CPU_D00p, 8);
  }

  if (cpu_signals.SIG_IN_CPU_RDp.state) {
    if (new_addr == 0xFF4A) cpy_inv(&new_bus.BUS_CPU_D00p, &reg_wy.NESO_WY0n, 8);
    if (new_addr == 0xFF4B) cpy_inv(&new_bus.BUS_CPU_D00p, &reg_wx.MYPA_WX0n, 8);
  }

  uint8_t wy_new = (uint8_t)pack_inv(8, &reg_wy.NESO_WY0n);
  uint8_t wx_new = (uint8_t)pack_inv(8, &reg_wx.MYPA_WX0n);
  //uint8_t ly = (uint8_t)pack(8, &reg_ly.MUWY_LY0p);
  //uint8_t px = (uint8_t)pack(8, &pix_count.XEHO_PX0p);

  win_reg.SARY_WY_MATCHp.dff17(TALU_xxCDEFxx(), 1, win_reg.ROGE_WY_MATCHp.state);

  if (vid_rst_new) {
    win_reg.PYCO_WIN_MATCHp.rst();
    win_reg.SARY_WY_MATCHp.rst();
  }

  win_reg.ROGE_WY_MATCHp = (ly_new == wy_new) && !bit(reg_lcdc.WYMO_LCDC_WINENn.state);

  if (bit(win_reg.SARY_WY_MATCHp.state)) win_reg.REJO_WY_MATCH_LATCHp.state = 1;
  if (bit(or2(lcd.POPU_y144p.state, vid_rst_new))) win_reg.REJO_WY_MATCH_LATCHp.state = 0;

  win_reg.NUKO_WX_MATCHp = (wx_new == px_new) && bit(win_reg.REJO_WY_MATCH_LATCHp.state);

  //----------------------------------------
  // Tile fetch sequencer

  wire LYZU_old = tile_fetcher.LYZU_BFETCH_S0p_D1.state;
  
  if (DELTA_EVEN) {
    tile_fetcher.LYZU_BFETCH_S0p_D1.state = tile_fetcher.LAXU_BFETCH_S0p.state;
  }

  if (!rendering_new) {
    tile_fetcher.LYZU_BFETCH_S0p_D1.rst();
  }
  
  wire LYZU_new = tile_fetcher.LYZU_BFETCH_S0p_D1.state;




  wire TEVO_WIN_FETCH_TRIGp = or3(
    and2(win_reg.RYFA_WIN_FETCHn_A.state, ~win_reg.RENE_WIN_FETCHn_B.state),
    and2(~win_reg.RYDY_WIN_HITp.state, win_reg.SOVY_WIN_HITp.state),
    and4(rendering_new, ~tile_fetcher.POKY_PRELOAD_LATCHp.state, tile_fetcher.NYKA_FETCH_DONEp.state, tile_fetcher.PORY_FETCH_DONEp.state)
  );

  auto bfetch_phase_old = pack(3, &tile_fetcher.LAXU_BFETCH_S0p);

  if (bfetch_phase_old != 5 && DELTA_ODD) {
    unpack(bfetch_phase_old + 1, 3, &tile_fetcher.LAXU_BFETCH_S0p);
  }

  auto lyry_old = tile_fetcher.LYRY_BFETCH_DONEp.state;
  tile_fetcher.LYRY_BFETCH_DONEp = and2(tile_fetcher.LAXU_BFETCH_S0p.state, tile_fetcher.NYVA_BFETCH_S2p.state);

  if (DELTA_ODD) {
    tile_fetcher.LOVY_FETCH_DONEp.state = lyry_old;
  }

  wire BFETCH_RSTp = or5(
    sprite_scanner.AVAP_SCAN_DONE_TRIGp.state,
    and2(win_reg.PYNU_WIN_MODE_Ap.state, ~win_reg.NOPA_WIN_MODE_Bp.state),
    and2(win_reg.RYFA_WIN_FETCHn_A.state, ~win_reg.RENE_WIN_FETCHn_B.state),
    and2(~win_reg.RYDY_WIN_HITp.state, win_reg.SOVY_WIN_HITp.state),
    and4(rendering_new, ~tile_fetcher.POKY_PRELOAD_LATCHp.state, tile_fetcher.NYKA_FETCH_DONEp.state, tile_fetcher.PORY_FETCH_DONEp.state)
  );


  if (bit(BFETCH_RSTp)) {
    tile_fetcher.LAXU_BFETCH_S0p.rst();
    tile_fetcher.MESU_BFETCH_S1p.rst();
    tile_fetcher.NYVA_BFETCH_S2p.rst();
    tile_fetcher.LAXU_BFETCH_S0p.rst();
    tile_fetcher.MESU_BFETCH_S1p.rst();
    tile_fetcher.NYVA_BFETCH_S2p.rst();
    tile_fetcher.LOVY_FETCH_DONEp.rst();
    tile_fetcher.LONY_FETCHINGp.state = 1;
    tile_fetcher.LYRY_BFETCH_DONEp.state = 0;
  }

  auto bfetch_phase_new = pack(3, &tile_fetcher.LAXU_BFETCH_S0p);

  if (!bit(and2(~tile_fetcher.LOVY_FETCH_DONEp.state, rendering_new))) {
    tile_fetcher.LONY_FETCHINGp.state = 0;
  }

  //----------------------------------------
  // Fine match counter

  auto fs = pack(3, &fine_scroll.RYKU_FINE_CNT0);

  if (fs != 7 && posedge(CLKPIPE_old, CLKPIPE_new)) {
    unpack(fs + 1, 3, &fine_scroll.RYKU_FINE_CNT0);
  }

  if (bit(TEVO_WIN_FETCH_TRIGp)) {
    fine_scroll.RYKU_FINE_CNT0.rst();
    fine_scroll.ROGA_FINE_CNT1.rst();
    fine_scroll.RUBU_FINE_CNT2.rst();
  }

  if (!rendering_new) {
    fine_scroll.RYKU_FINE_CNT0.rst();
    fine_scroll.ROGA_FINE_CNT1.rst();
    fine_scroll.RUBU_FINE_CNT2.rst();
  }


  //----------------------------------------
  // PPU / LCD output

  {
    auto& tf = tile_fetcher;
    auto& sf = sprite_fetcher;

    // bfetch .#.. 2
    // bfetch .... 0
    // bfetch #... 1
    // bfetch ##.. 3
    // bfetch .##. 6
    // bfetch ..#. 4
    // bfetch #.#. 5
    // bfetch ###. 7
    // bfetch .#.# 10
    // bfetch ...# 8
    // bfetch #..# 9
    // bfetch ##.# 11


    wire latch_ta_old = rendering_old && (bfetch_phase_old == 3) && bit(LYZU_old);
    wire latch_ta_new = rendering_new && (bfetch_phase_new == 3) && bit(LYZU_new);


    if (posedge(latch_ta_old, latch_ta_new)) {
      cpy_inv(&tile_temp_a.LEGU_TILE_DA0n, &vram_bus.BUS_VRAM_D00p, 8);
    }

    wire latch_db_old = rendering_old && (bfetch_phase_old == 1 || bfetch_phase_old == 5)  && !bit(LYZU_old);
    wire latch_db_new = rendering_new && (bfetch_phase_new == 1 || bfetch_phase_new == 5)  && !bit(LYZU_new);

    if (negedge(latch_db_old, latch_db_new)) {
      tile_temp_b.RAWU_TILE_DB0p.state = vram_bus.BUS_VRAM_D00p.state;
      tile_temp_b.POZO_TILE_DB1p.state = vram_bus.BUS_VRAM_D01p.state;
      tile_temp_b.PYZO_TILE_DB2p.state = vram_bus.BUS_VRAM_D02p.state;
      tile_temp_b.POXA_TILE_DB3p.state = vram_bus.BUS_VRAM_D03p.state;
      tile_temp_b.PULO_TILE_DB4p.state = vram_bus.BUS_VRAM_D04p.state;
      tile_temp_b.POJU_TILE_DB5p.state = vram_bus.BUS_VRAM_D05p.state;
      tile_temp_b.POWY_TILE_DB6p.state = vram_bus.BUS_VRAM_D06p.state;
      tile_temp_b.PYJU_TILE_DB7p.state = vram_bus.BUS_VRAM_D07p.state;
    }

    wire latch_sa_old = rendering_old && (sfetch_phase_old == 3) && !bit(tyfo_old);
    wire latch_sa_new = rendering_new && (sfetch_phase_new == 3) && !bit(tyfo_new);

    if (posedge(latch_sa_old, latch_sa_new)) {
      cpy_inv(&sprite_pix_a.REWO_SPRITE_DA0n, &flipped_sprite.PUTE_FLIP0p, 8);
    }

    wire latch_sb_old = rendering_old && (sfetch_phase_old == 5) && !bit(tyfo_old);
    wire latch_sb_new = rendering_new && (sfetch_phase_new == 5) && !bit(tyfo_new);

    if (posedge(latch_sb_old, latch_sb_new)) {
      cpy_inv(&sprite_pix_b.PEFO_SPRITE_DB0n, &flipped_sprite.PUTE_FLIP0p, 8);
    }

    //----------------------------------------
    // Pal reg read/write

    if (cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
      if (new_addr == 0xFF47) cpy_inv(&reg_bgp.PAVO_BGP_D0n, &new_bus.BUS_CPU_D00p, 8);
      if (new_addr == 0xFF48) cpy_inv(&reg_obp0.XUFU_OBP0_D0n, &new_bus.BUS_CPU_D00p, 8);
      if (new_addr == 0xFF49) cpy_inv(&reg_obp1.MOXY_OBP1_D0n, &new_bus.BUS_CPU_D00p, 8);
    }

    if (cpu_signals.SIG_IN_CPU_RDp.state) {
      if (new_addr == 0xFF47) cpy_inv(&new_bus.BUS_CPU_D00p, &reg_bgp.PAVO_BGP_D0n, 8);
      if (new_addr == 0xFF48) cpy_inv(&new_bus.BUS_CPU_D00p, &reg_obp0.XUFU_OBP0_D0n, 8);
      if (new_addr == 0xFF49) cpy_inv(&new_bus.BUS_CPU_D00p, &reg_obp1.MOXY_OBP1_D0n, 8);
    }

    //----------------------------------------
    // Pixel pipes

    uint8_t tpix_a = (uint8_t)pack_inv(8, &tile_temp_a.LEGU_TILE_DA0n);
    uint8_t tpix_b = (uint8_t)pack(8, &tile_temp_b.RAWU_TILE_DB0p);
    uint8_t spix_a = (uint8_t)pack_inv(8, &sprite_pix_a.REWO_SPRITE_DA0n);
    uint8_t spix_b = (uint8_t)pack_inv(8, &sprite_pix_b.PEFO_SPRITE_DB0n);

    uint8_t spipe_a = (uint8_t)pack(8, &pix_pipes.NURO_SPR_PIPE_A0);
    uint8_t spipe_b = (uint8_t)pack(8, &pix_pipes.NYLU_SPR_PIPE_B0);
    uint8_t bpipe_a = (uint8_t)pack(8, &pix_pipes.MYDE_BGW_PIPE_A0);
    uint8_t bpipe_b = (uint8_t)pack(8, &pix_pipes.TOMY_BGW_PIPE_B0);
    uint8_t mpipe   = (uint8_t)pack(8, &pix_pipes.VEZO_MASK_PIPE_0);
    uint8_t ppipe   = (uint8_t)pack(8, &pix_pipes.RUGO_PAL_PIPE_D0);

    if (posedge(CLKPIPE_old, CLKPIPE_new)) {
      spipe_a = (spipe_a << 1) | 0;
      spipe_b = (spipe_b << 1) | 0;
      bpipe_a = (bpipe_a << 1) | 0;
      bpipe_b = (bpipe_b << 1) | 0;
      mpipe   = (mpipe   << 1) | 1;
      ppipe   = (ppipe   << 1) | 0;
    }

    if (bit(BFETCH_RSTp)) bpipe_a = tpix_a;
    if (bit(BFETCH_RSTp)) bpipe_b = tpix_b;

    if (bit(sf.WUTY_SFETCH_DONE_TRIGp.state)) {
      uint8_t smask = (spipe_a | spipe_b);
      spipe_a = (spipe_a & smask) | (spix_a & ~smask);
      spipe_b = (spipe_b & smask) | (spix_b & ~smask);
      mpipe = bit(oam_temp_b.DEPO_OAM_DB7p.state) ? mpipe | ~smask : mpipe & smask;
      ppipe = bit(oam_temp_b.GOMO_OAM_DB4p.state) ? ppipe | ~smask : ppipe & smask;
    }

    unpack(spipe_a, 8, &pix_pipes.NURO_SPR_PIPE_A0);
    unpack(spipe_b, 8, &pix_pipes.NYLU_SPR_PIPE_B0);
    unpack(bpipe_a, 8, &pix_pipes.MYDE_BGW_PIPE_A0);
    unpack(bpipe_b, 8, &pix_pipes.TOMY_BGW_PIPE_B0);
    unpack(mpipe,   8, &pix_pipes.VEZO_MASK_PIPE_0);
    unpack(ppipe,   8, &pix_pipes.RUGO_PAL_PIPE_D0);

    //----------------------------------------
    // Pipe merge and output

    wire PIX_BG_LOp = and2(pix_pipes.PYBO_BGW_PIPE_A7.state, ~reg_lcdc.VYXE_LCDC_BGENn.state);
    wire PIX_BG_HIp = and2(pix_pipes.SOHU_BGW_PIPE_B7.state, ~reg_lcdc.VYXE_LCDC_BGENn.state);
    wire PIX_SP_LOp = and2(pix_pipes.WUFY_SPR_PIPE_A7.state, ~reg_lcdc.XYLO_LCDC_SPENn.state);
    wire PIX_SP_HIp = and2(pix_pipes.VUPY_SPR_PIPE_B7.state, ~reg_lcdc.XYLO_LCDC_SPENn.state);

    int pal_idx = 0;
    uint8_t pal = 0;

    uint8_t bgp  = (uint8_t)pack(8, &reg_bgp.PAVO_BGP_D0n);
    uint8_t obp0 = (uint8_t)pack(8, &reg_obp0.XUFU_OBP0_D0n);
    uint8_t obp1 = (uint8_t)pack(8, &reg_obp1.MOXY_OBP1_D0n);

    if (bit(or2(PIX_SP_HIp, PIX_SP_LOp))) {
      pal_idx = pack(PIX_SP_LOp, PIX_SP_HIp);
      pal = bit(pix_pipes.LYME_PAL_PIPE_D7.state) ? obp1 : obp0;
    }
    else {
      pal_idx = pack(PIX_BG_LOp, PIX_BG_HIp);
      pal = bgp;
    }

    pix_pipes.REMY_LD0n = ~bit(pal >> (pal_idx * 2 + 0));
    pix_pipes.RAVO_LD1n = ~bit(pal >> (pal_idx * 2 + 1));
  }

  {
    if (!vid_rst_new) {
      wire POGU = ~or2(lcd.SYGU_LINE_STROBE.state, lcd.RUTU_x113p.state);
      lcd.PIN_52_LCD_CNTRL.pin_out(POGU, POGU);

      wire LOFU_x113n = ~lcd.RUTU_x113p.state;
      lcd.LUCA_LINE_EVENp.dff17(LOFU_x113n,            1, ~lcd.LUCA_LINE_EVENp.state);
      lcd.NAPO_FRAME_EVENp.dff17(lcd.POPU_y144p.state, 1, ~lcd.NAPO_FRAME_EVENp.state);

      wire KOFO = ~xor2(lcd.NAPO_FRAME_EVENp.state, ~lcd.LUCA_LINE_EVENp.state);
      lcd.PIN_56_LCD_FLIPS.pin_out(KOFO, KOFO);

      lcd.MEDA_VSYNC_OUTn.dff17(~lcd.NYPE_x113p.state, 1, ly_new == 0);
      lcd.PIN_57_LCD_VSYNC.pin_out(~lcd.MEDA_VSYNC_OUTn.state, ~lcd.MEDA_VSYNC_OUTn.state);

      if (bit(sprite_scanner.AVAP_SCAN_DONE_TRIGp.state) && bit(lcd.PAHO_X_8_SYNC.state)) {
        lcd.POME = 0;
        lcd.RUJU = 1;
        lcd.POFY = 0;
      }
      else if (bit(sprite_scanner.AVAP_SCAN_DONE_TRIGp.state)) {
        lcd.POME = 0;
        lcd.RUJU = 0;
        lcd.POFY = 1;
      }
      else if (bit(lcd.PAHO_X_8_SYNC.state)) {
        lcd.POME = 1;
        lcd.RUJU = 1;
        lcd.POFY = 0;
      }

      lcd.PIN_50_LCD_DATA1.pin_out(pix_pipes.RAVO_LD1n.state, pix_pipes.RAVO_LD1n.state);
      lcd.PIN_51_LCD_DATA0.pin_out(pix_pipes.REMY_LD0n.state, pix_pipes.REMY_LD0n.state);
      lcd.PIN_54_LCD_HSYNC.pin_out(~lcd.POFY.state, ~lcd.POFY.state);
      lcd.PIN_55_LCD_LATCH.pin_out(~lcd.RUTU_x113p.state, ~lcd.RUTU_x113p.state);

      if (bit(and2(pix_count.XEHO_PX0p.state, pix_count.XYDO_PX3p.state))) {
        lcd.WUSA_LCD_CLOCK_GATE.state = 1;
      }
      if (bit(VOGA_HBLANKp.state)) {
        lcd.WUSA_LCD_CLOCK_GATE.state = 0;
      }

      {
        wire TOBA_LCD_CLOCK = and2(lcd.WUSA_LCD_CLOCK_GATE.state, CLKPIPE_new);
        wire POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.state, ~fine_scroll.NYZE_SCX_FINE_MATCH_B.state);
        wire SEMU_LCD_CLOCK = or2(TOBA_LCD_CLOCK, POVA_FINE_MATCH_TRIGp);
        lcd.PIN_53_LCD_CLOCK.pin_out(~SEMU_LCD_CLOCK, ~SEMU_LCD_CLOCK);
      }
    }
    else {
      lcd.LUCA_LINE_EVENp.state &= ~1;
      lcd.NAPO_FRAME_EVENp.state &= ~1;
      lcd.MEDA_VSYNC_OUTn.state &= ~1;
      lcd.WUSA_LCD_CLOCK_GATE.state = 0;

      lcd.POME = 1;
      lcd.RUJU = 1;
      lcd.POFY = 0;

      lcd.PIN_50_LCD_DATA1.pin_out(pix_pipes.RAVO_LD1n.state, pix_pipes.RAVO_LD1n.state);
      lcd.PIN_51_LCD_DATA0.pin_out(pix_pipes.REMY_LD0n.state, pix_pipes.REMY_LD0n.state);
      lcd.PIN_52_LCD_CNTRL.pin_out(1, 1);
      lcd.PIN_53_LCD_CLOCK.pin_out(1, 1);
      lcd.PIN_54_LCD_HSYNC.pin_out(1, 1);
      lcd.PIN_55_LCD_LATCH.pin_out(~div.UGOT_DIV06p.state, ~div.UGOT_DIV06p.state);
      lcd.PIN_56_LCD_FLIPS.pin_out(~div.TULU_DIV07p.state, ~div.TULU_DIV07p.state);
      lcd.PIN_57_LCD_VSYNC.pin_out(1, 1);
    }
  }

  
  //----------------------------------------
  // Audio

  tock_spu();

  //----------------------------------------
  // Memory buses

  tock_ext_logic(cart_blob);
  tock_vram_bus_logic(TEVO_WIN_FETCH_TRIGp);
  tock_oam_bus_logic();
  tock_zram_logic();

  //----------------------------------------
  // And finally, interrupts.

  tock_interrupts_gates();

  old_bus = new_bus;
  commit();
}
