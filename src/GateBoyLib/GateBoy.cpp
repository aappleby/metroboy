#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"
#include "GateBoyLib/GateBoyState.h"

//-----------------------------------------------------------------------------

void GateBoy::reset_to_bootrom(const blob& cart_blob, bool fastboot)
{
  wipe();

  // Can't run logic mode during power-on-reset, so save and restore it.
  bool old_logic_mode = sys.logic_mode;
  sys.logic_mode = false;

  // Put some recognizable pattern in vram so we can see that we're in the bootrom
  for (int i = 0; i < 8192; i++) {
    uint32_t h = i * 0x1234567;
    mem.vid_ram[i] = uint8_t(h ^ (h >> 4));
  }

  gb_state.cpu_abus.reset_to_bootrom();
  gb_state.cpu_dbus.reset_to_bootrom();

  gb_state.sprite_ibus.reset_to_bootrom();
  gb_state.sprite_lbus.reset_to_bootrom();

  gb_state.reg_dma.NAFA_DMA_A08n.state = 0b00011010;
  gb_state.reg_dma.PYNE_DMA_A09n.state = 0b00011010;
  gb_state.reg_dma.PARA_DMA_A10n.state = 0b00011010;
  gb_state.reg_dma.NYDO_DMA_A11n.state = 0b00011010;
  gb_state.reg_dma.NYGY_DMA_A12n.state = 0b00011010;
  gb_state.reg_dma.PULA_DMA_A13n.state = 0b00011010;
  gb_state.reg_dma.POKU_DMA_A14n.state = 0b00011010;
  gb_state.reg_dma.MARU_DMA_A15n.state = 0b00011010;

  gb_state.reg_bgp.PAVO_BGP_D0n.state = 0b00011010;
  gb_state.reg_bgp.NUSY_BGP_D1n.state = 0b00011010;
  gb_state.reg_bgp.PYLU_BGP_D2n.state = 0b00011010;
  gb_state.reg_bgp.MAXY_BGP_D3n.state = 0b00011010;
  gb_state.reg_bgp.MUKE_BGP_D4n.state = 0b00011010;
  gb_state.reg_bgp.MORU_BGP_D5n.state = 0b00011010;
  gb_state.reg_bgp.MOGY_BGP_D6n.state = 0b00011010;
  gb_state.reg_bgp.MENA_BGP_D7n.state = 0b00011010;

  gb_state.reg_obp0.XUFU_OBP0_D0n.state = 0b00011010;
  gb_state.reg_obp0.XUKY_OBP0_D1n.state = 0b00011010;
  gb_state.reg_obp0.XOVA_OBP0_D2n.state = 0b00011010;
  gb_state.reg_obp0.XALO_OBP0_D3n.state = 0b00011010;
  gb_state.reg_obp0.XERU_OBP0_D4n.state = 0b00011010;
  gb_state.reg_obp0.XYZE_OBP0_D5n.state = 0b00011010;
  gb_state.reg_obp0.XUPO_OBP0_D6n.state = 0b00011010;
  gb_state.reg_obp0.XANA_OBP0_D7n.state = 0b00011010;

  gb_state.reg_obp1.MOXY_OBP1_D0n.state = 0b00011010;
  gb_state.reg_obp1.LAWO_OBP1_D1n.state = 0b00011010;
  gb_state.reg_obp1.MOSA_OBP1_D2n.state = 0b00011010;
  gb_state.reg_obp1.LOSE_OBP1_D3n.state = 0b00011010;
  gb_state.reg_obp1.LUNE_OBP1_D4n.state = 0b00011010;
  gb_state.reg_obp1.LUGU_OBP1_D5n.state = 0b00011010;
  gb_state.reg_obp1.LEPU_OBP1_D6n.state = 0b00011010;
  gb_state.reg_obp1.LUXO_OBP1_D7n.state = 0b00011010;

  gb_state.joy_int.reset_to_bootrom();
  gb_state.reg_joy.reset_to_bootrom();

  gb_state.check_state_old_and_driven_or_pulled();

  sys.fastboot = fastboot;

  //----------------------------------------
  // Update the sim without ticking the clock to to settle initial reset signals.

  sys.rst = 1;

  tock_cpu();
  tock_gates(cart_blob);

  //----------------------------------------
  // Release reset, start clock, and sync with phase

  sys.rst = 0;
  sys.clk_en = 1;
  sys.clk_good = 1;
  run_phases(cart_blob, 2);

  CHECK_N(bit(gb_state.sys_clk.AFUR_xxxxEFGH.qp_old()));
  CHECK_P(bit(gb_state.sys_clk.ALEF_AxxxxFGH.qp_old()));
  CHECK_P(bit(gb_state.sys_clk.APUK_ABxxxxGH.qp_old()));
  CHECK_P(bit(gb_state.sys_clk.ADYK_ABCxxxxH.qp_old()));

  sys.phase_total = 0;
  sys.phase_origin = 46880720;
;

  //----------------------------------------
  // Wait for SIG_CPU_START

  while(bit(~gb_state.sys_rst.SIG_CPU_STARTp.out_old())) {
    run_phases(cart_blob, 8);
  }

  //----------------------------------------
  // Delay to sync up with expected div value

  run_phases(cart_blob, 16);

  //----------------------------------------
  // Fetch the first instruction in the bootrom

  uint8_t data_out;
  dbg_read(cart_blob, 0x0000, data_out);

  //----------------------------------------
  // We're ready to go, release the CPU so it can start running the bootrom.

  sys.clk_req = 1;
  sys.cpu_en = true;

  if (fastboot) {
    gb_state.reg_div.TERO_DIV03p.state = 0b00011010;
    gb_state.reg_div.UNYK_DIV04p.state = 0b00011010;
    gb_state.reg_div.UPOF_DIV15p.state = 0b00011011;
  }

  memset(mem.framebuffer, 4, sizeof(mem.framebuffer));

  sys.probes.reset_to_cart();

  sys.logic_mode = old_logic_mode;
  //if (sys.logic_mode) wipe_flags();

  lb_state.from_gb_state(gb_state, sys.phase_total);
}

//-----------------------------------------------------------------------------

void GateBoy::reset_to_cart(const blob& cart_blob) {
  reset_to_bootrom(cart_blob, true);

  gb_state.VOGA_HBLANKp.state = 0b00011001;

  gb_state.cpu_signals.reset_to_cart();
  gb_state.cpu_abus.reset_to_cart_new();
  gb_state.cpu_dbus.reset_to_cart_new();

  gb_state.vram_abus.lo.reset_to_cart();
  gb_state.vram_abus.hi.reset_to_cart();
  gb_state.vram_dbus.reset_to_cart();
  gb_state.vram_ext_ctrl.reset_to_cart();
  gb_state.vram_ext_abus.reset_to_cart();
  gb_state.vram_ext_dbus.reset_to_cart();

  gb_state.sprite_ibus.reset_to_cart();
  gb_state.sprite_lbus.reset_to_cart();

  gb_state.oam_ctrl.reset_to_cart();
  gb_state.oam_abus.reset_to_cart();
  gb_state.oam_dbus_a.reset_to_cart();
  gb_state.oam_dbus_b.reset_to_cart();
  gb_state.oam_latch_a.reset_to_cart();
  gb_state.oam_latch_b.reset_to_cart();
  gb_state.oam_temp_a.reset_to_cart();
  gb_state.oam_temp_b.reset_to_cart();

  gb_state.ext_ctrl.reset_to_cart();
  gb_state.ext_abus.lo.reset_to_cart();
  gb_state.ext_abus.hi.reset_to_cart();
  gb_state.ext_dbus.reset_to_cart();
  gb_state.ext_addr_latch.reset_to_cart();
  gb_state.ext_data_latch.reset_to_cart();

  //zram_bus.reset_to_cart();



  gb_state.sys_rst.reset_to_cart();
  gb_state.sys_clk.reset_to_cart();
  gb_state.reg_div.reset_to_cart();
  
  //interrupts.reset_to_cart();
  gb_state.reg_if.reset_to_cart();
  gb_state.reg_ie.reset_to_cart();
  gb_state.int_latch.reset_to_cart();
  gb_state.cpu_int.reset_to_cart();
  gb_state.cpu_ack.reset_to_cart();

  //serial.reset_to_cart();

  //reset_sprite_store();
  gb_state.sprite_counter.BESE_SPRITE_COUNT0.state = 0b00011010;
  gb_state.sprite_counter.CUXY_SPRITE_COUNT1.state = 0b00011010;
  gb_state.sprite_counter.BEGO_SPRITE_COUNT2.state = 0b00011010;
  gb_state.sprite_counter.DYBE_SPRITE_COUNT3.state = 0b00011010;

  gb_state.sprite_scanner.reset_to_cart();
  gb_state.scan_counter.reset_to_cart();
  gb_state.sprite_index.reset_to_cart();

  gb_state.sfetch_counter.reset_to_cart();
  gb_state.sfetch_control.reset_to_cart();

  gb_state.int_ctrl.RUPO_LYC_MATCHn.state = 0b00011000;

  gb_state.reg_stat.reset_to_cart();
  gb_state.pix_count.reset_to_cart();
  gb_state.mask_pipe.reset_to_cart();
  gb_state.lcd.REMY_LD0n.state = 0b00011000;
  gb_state.lcd.RAVO_LD1n.state = 0b00011000;

  gb_state.dma_lo.reset_to_cart();
  gb_state.reg_dma.reset_to_cart();
  gb_state.MATU_DMA_RUNNINGp.state = 0b00011010;
  gb_state.dma_ctrl.reset_to_cart();

  gb_state.reg_bgp.reset_to_cart();
  gb_state.reg_obp0.reset_to_cart();
  gb_state.reg_obp1.reset_to_cart();
  
  //joy.reset_to_cart();
  gb_state.joy_int.reset_to_cart();
  gb_state.reg_joy.reset_to_cart();

  gb_state.reg_lcdc.reset_to_cart();
  gb_state.lcd.reset_to_cart();

  gb_state.reg_lx.reset_to_cart();
  gb_state.reg_ly.reset_to_cart();

  gb_state.int_ctrl.ROPO_LY_MATCH_SYNCp.state = 0b00011001;
  gb_state.reg_lyc.reset_to_cart();

  gb_state.WODU_HBLANKp.state = 0b00011001;

  gb_state.sprite_scanner.FETO_SCAN_DONEp.state = 0b00011001;
  gb_state.ATEJ_LINE_RSTp.state = 0b00011000;

  //reg_NR50.reset_to_cart();
  //reg_NR51.reset_to_cart();
  //reg_NR52.reset_to_cart();

  gb_state.check_state_old_and_driven_or_pulled();

  sys.rst = false;
  sys.t1 = false;
  sys.t2 = false;
  sys.clk_en = true;
  sys.clk_good = true;
  sys.clk_req = true;
  sys.fastboot = true;

  cpu.core.reset_to_cart();

  cpu.bus_req_new.addr = 0xFF50;
  cpu.bus_req_new.data = 1;
  cpu.bus_req_new.read = 0;
  cpu.bus_req_new.write = 1;

  cpu.cpu_data_latch = 1;
  cpu.intf_latch = 1;
  cpu.intf_latch_delay = 0;
  cpu.intf_halt_latch = 0;

  memcpy(mem.vid_ram, vram_boot, 8192);

  mem.zero_ram[0x7A] = 0x39;
  mem.zero_ram[0x7B] = 0x01;
  mem.zero_ram[0x7C] = 0x2E;

  memcpy(mem.framebuffer, framebuffer_boot, 160*144);

  sys.sim_time = 169.62587129999756;
  sys.phase_total = 0;
  sys.phase_origin = 0;

  sys.probes.reset_to_cart();

  //if (sys.logic_mode) wipe_flags();

  lb_state.from_gb_state(gb_state, sys.phase_total);
}

//------------------------------------------------------------------------------------------------------------------------

bool GateBoy::dbg_read(const blob& cart_blob, int addr, uint8_t& out) {
  CHECK_P((sys.phase_total & 7) == 0);

  Req old_req = cpu.bus_req_new;
  bool old_cpu_en = sys.cpu_en;
  sys.cpu_en = false;

  cpu.bus_req_new.addr = uint16_t(addr);
  cpu.bus_req_new.data = 0;
  cpu.bus_req_new.read = 1;
  cpu.bus_req_new.write = 0;
  run_phases(cart_blob, 8);

  cpu.bus_req_new = old_req;
  sys.cpu_en = old_cpu_en;

  out = cpu.cpu_data_latch;
  return true;
}

//------------------------------------------------------------------------------

bool GateBoy::dbg_write(const blob& cart_blob, int addr, uint8_t data) {
  CHECK_P((sys.phase_total & 7) == 0);

  Req old_req = cpu.bus_req_new;
  bool old_cpu_en = sys.cpu_en;
  sys.cpu_en = false;

  cpu.bus_req_new.addr = uint16_t(addr);
  cpu.bus_req_new.data = data;
  cpu.bus_req_new.read = 0;
  cpu.bus_req_new.write = 1;
  run_phases(cart_blob, 8);

  cpu.bus_req_new = old_req;
  sys.cpu_en = old_cpu_en;
  return true;
}


//------------------------------------------------------------------------------------------------------------------------

bool GateBoy::next_phase(const blob& cart_blob) {
  CHECK_N(!sys.clk_req && sys.logic_mode);

  tock_cpu();

  if (sys.logic_mode) {
    tock_logic(cart_blob, sys.phase_total);
    lb_state.to_gb_state(gb_state, sys.phase_total);
    update_framebuffer(bit_pack(gb_state.pix_count) - 8, bit_pack(gb_state.reg_ly), ~gb_state.lcd.PIN_51_LCD_DATA0.state, ~gb_state.lcd.PIN_50_LCD_DATA1.state);
    sys.phase_total++;
    return true;
  }
  else {
    tock_gates(cart_blob);
    update_framebuffer(bit_pack(gb_state.pix_count) - 8, bit_pack(gb_state.reg_ly), gb_state.lcd.PIN_51_LCD_DATA0.qp_ext_old(), gb_state.lcd.PIN_50_LCD_DATA1.qp_ext_old());
    sys.phase_total++;
    return true;
  }

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

    mem.framebuffer[lcd_x + lcd_y * 160] = uint8_t(3 - new_pix);
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
  cpu.cpu_data_latch &= (uint8_t)bit_pack(gb_state.cpu_dbus);
  cpu.imask_latch = (uint8_t)bit_pack(gb_state.reg_ie);

  if (DELTA_HA) {
    if (cpu.core.op == 0x76 && (cpu.imask_latch & cpu.intf_halt_latch)) cpu.core.state_ = 0;
    cpu.intf_halt_latch = 0;
  }

  // +ha -ab -bc -cd -de -ef -fg -gh
  if (DELTA_HA) {
    // this one latches funny, some hardware bug
    if (bit(gb_state.reg_if.NYBO_FF0F_D2p.state)) cpu.intf_halt_latch |= INT_TIMER_MASK;
  }

  // -ha +ab -bc
  if (DELTA_AB) {
    if (sys.cpu_en) {
      cpu.core.tock_ab(cpu.imask_latch, cpu.intf_latch, cpu.cpu_data_latch);
    }
  }

  if (DELTA_AB) {
    if (sys.cpu_en) {
      cpu.bus_req_new.addr = cpu.core._bus_addr;
      cpu.bus_req_new.data = cpu.core._bus_data;
      cpu.bus_req_new.read = cpu.core._bus_read;
      cpu.bus_req_new.write = cpu.core._bus_write;
    }
  }

  // -bc +cd +de -ef -fg -gh -ha -ab
  if (DELTA_DE) {
    if (bit(gb_state.reg_if.LOPE_FF0F_D0p.state)) cpu.intf_halt_latch |= INT_VBLANK_MASK;
    if (bit(gb_state.reg_if.LALU_FF0F_D1p.state)) cpu.intf_halt_latch |= INT_STAT_MASK;
    if (bit(gb_state.reg_if.UBUL_FF0F_D3p.state)) cpu.intf_halt_latch |= INT_SERIAL_MASK;
    if (bit(gb_state.reg_if.ULAK_FF0F_D4p.state)) cpu.intf_halt_latch |= INT_JOYPAD_MASK;
  }

  // -ha -ab -bc -cd -de -ef +fg +gh
  if (DELTA_GH) {
    cpu.cpu_data_latch = 0xFF;
  }

  // +ha -ab -bc -cd -de -ef -fg +gh
  if (DELTA_GH) {
    cpu.intf_latch = (uint8_t)bit_pack(gb_state.reg_if);
  }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_gates(const blob& cart_blob) {
  const GateBoyState  reg_old = gb_state;
  GateBoyState& reg_new = gb_state;

  //----------

  /*#p27.SYLO*/ wire SYLO_WIN_HITn_old = not1(reg_old.win_ctrl.RYDY_WIN_HITp.out_old());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp_old = not1(SYLO_WIN_HITn_old);

  //----------

  wire EXT_vcc = 1;
  wire EXT_gnd = 0;

  /*_SIG_VCC*/ gb_state.SIG_VCC.sig_in(EXT_vcc);
  /*_SIG_GND*/ gb_state.SIG_GND.sig_in(EXT_gnd);

  //-----------------------------------------------------------------------------

  memset(&gb_state.cpu_abus, BIT_NEW | BIT_PULLED | 1, sizeof(gb_state.cpu_abus));
  memset(&gb_state.cpu_dbus, BIT_NEW | BIT_PULLED | 1, sizeof(gb_state.cpu_dbus));

  //-----------------------------------------------------------------------------

  {
    bool EXT_cpu_latch_ext;

    if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      // Data has to be driven on EFGH or we fail the wave tests
      gb_state.cpu_dbus.set_data(cpu.bus_req_new.write, cpu.bus_req_new.data_lo);
      EXT_cpu_latch_ext = cpu.bus_req_new.read;
    }
    else {
      gb_state.cpu_dbus.set_data(false, 0);
      EXT_cpu_latch_ext = 0;
    }
    /*_SIG_IN_CPU_LATCH_EXT*/ gb_state.cpu_signals.SIG_IN_CPU_DBUS_FREE.sig_in(EXT_cpu_latch_ext);

    bool EXT_addr_new = (cpu.bus_req_new.read || cpu.bus_req_new.write);
    bool in_bootrom = bit(~reg_old.cpu_signals.TEPU_BOOT_BITn.qp_old());
    bool addr_boot = (cpu.bus_req_new.addr <= 0x00FF) && in_bootrom;
    bool addr_vram = (cpu.bus_req_new.addr >= 0x8000) && (cpu.bus_req_new.addr < 0x9FFF);
    bool addr_high = (cpu.bus_req_new.addr >= 0xFE00);

    bool EXT_cpu_rd;
    bool EXT_cpu_wr;

    if (DELTA_HA) {
      EXT_cpu_rd = 0;
      EXT_cpu_wr = 0;
      gb_state.cpu_abus.set_addr(cpu.bus_req_new.addr & 0x00FF);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
      if (addr_vram) EXT_addr_new = false;
    }
    else {
      EXT_cpu_rd = cpu.bus_req_new.read;
      EXT_cpu_wr = cpu.bus_req_new.write;
      gb_state.cpu_abus.set_addr(cpu.bus_req_new.addr);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
    }

    /*_SIG_IN_CPU_RDp*/ gb_state.cpu_signals.SIG_IN_CPU_RDp.sig_in(EXT_cpu_rd);
    /*_SIG_IN_CPU_WRp*/ gb_state.cpu_signals.SIG_IN_CPU_WRp.sig_in(EXT_cpu_wr);
    /*_SIG_IN_CPU_EXT_BUSp*/ gb_state.cpu_signals.SIG_IN_CPU_EXT_BUSp.sig_in(EXT_addr_new);
  }

  //-----------------------------------------------------------------------------

  {
    wire EXT_sys_clkreq = bit(sys.clk_req);
    wire EXT_sys_rst = bit(~sys.rst);
    wire EXT_sys_t2 = bit(~sys.t2);
    wire EXT_sys_t1 = bit(~sys.t1);

    wire EXT_clkin = !(sys.phase_total & 1) && sys.clk_en;
    wire EXT_clkgood = bit(~sys.clk_good);

    /*_PIN_74*/ gb_state.sys_clk.PIN_74_CLK.pin_clk(EXT_clkin, EXT_clkgood);
    /*_PIN_71*/ gb_state.sys_rst.PIN_71_RST.pin_in(EXT_sys_rst);
    /*_PIN_76*/ gb_state.sys_rst.PIN_76_T2.pin_in(EXT_sys_t2);
    /*_PIN_77*/ gb_state.sys_rst.PIN_77_T1.pin_in(EXT_sys_t1);

    wire EXT_ack_vblank = get_bit(cpu.core.int_ack, BIT_VBLANK);
    wire EXT_ack_stat = get_bit(cpu.core.int_ack, BIT_STAT);
    wire EXT_ack_timer = get_bit(cpu.core.int_ack, BIT_TIMER);
    wire EXT_ack_serial = get_bit(cpu.core.int_ack, BIT_SERIAL);
    wire EXT_ack_joypad = get_bit(cpu.core.int_ack, BIT_JOYPAD);

    /*_SIG_CPU_ACK_VBLANK*/ gb_state.cpu_ack.SIG_CPU_ACK_VBLANK.sig_in(EXT_ack_vblank);
    /*_SIG_CPU_ACK_STAT  */ gb_state.cpu_ack.SIG_CPU_ACK_STAT.sig_in(EXT_ack_stat);
    /*_SIG_CPU_ACK_TIMER */ gb_state.cpu_ack.SIG_CPU_ACK_TIMER.sig_in(EXT_ack_timer);
    /*_SIG_CPU_ACK_SERIAL*/ gb_state.cpu_ack.SIG_CPU_ACK_SERIAL.sig_in(EXT_ack_serial);
    /*_SIG_CPU_ACK_JOYPAD*/ gb_state.cpu_ack.SIG_CPU_ACK_JOYPAD.sig_in(EXT_ack_joypad);

    /*_SIG_CPU_CLKREQ*/ gb_state.sys_clk.SIG_CPU_CLKREQ.sig_in(EXT_sys_clkreq);

    /*_SIG_CPU_ADDR_HIp*/ gb_state.cpu_signals.SIG_CPU_ADDR_HIp.sig_out(gb_state.cpu_abus.SYRO_FE00_FFFF());
    /*_SIG_CPU_UNOR_DBG*/ gb_state.cpu_signals.SIG_CPU_UNOR_DBG.sig_out(gb_state.sys_rst.UNOR_MODE_DBG2p());
    /*_SIG_CPU_UMUT_DBG*/ gb_state.cpu_signals.SIG_CPU_UMUT_DBG.sig_out(gb_state.sys_rst.UMUT_MODE_DBG1p());
  }

  //-----------------------------------------------------------------------------
  // Sys clock signals

  tock_clocks_gates();

  /*_p07.UJYV*/ wire UJYV_CPU_RDn = not1(gb_state.cpu_signals.SIG_IN_CPU_RDp.out_new());
  /*_p07.TEDO*/ gb_state.cpu_signals.TEDO_CPU_RDp = not1(UJYV_CPU_RDn);

  /*#p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(gb_state.sys_clk.ADAR_ABCxxxxH(), gb_state.sys_clk.ATYP_ABCDxxxx());
  /*_p01.AREV*/ wire AREV_CPU_WRn = nand2(gb_state.cpu_signals.SIG_IN_CPU_WRp.out_new(), AFAS_xxxxEFGx);
  /*_p01.APOV*/ gb_state.cpu_signals.APOV_CPU_WRp = not1(AREV_CPU_WRn);

  /*_p07.UBAL*/ wire UBAL_CPU_WRn = not1(gb_state.cpu_signals.APOV_CPU_WRp.out_new());
  /*_p07.TAPU*/ gb_state.cpu_signals.TAPU_CPU_WRp = not1(UBAL_CPU_WRn); // xxxxEFGx

  /*#p01.AGUT*/ wire AGUT_xxCDEFGH = or_and3(gb_state.sys_clk.AROV_xxCDEFxx(), gb_state.sys_clk.AJAX_xxxxEFGH(), gb_state.cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new());
  /*#p01.AWOD*/ wire AWOD_ABxxxxxx = nor2(gb_state.sys_rst.UNOR_MODE_DBG2p(), AGUT_xxCDEFGH);
  /*#p01.ABUZ*/ gb_state.cpu_signals.ABUZ_EXT_RAM_CS_CLK = not1(AWOD_ABxxxxxx);

  tock_div_gates();
  tock_reset_gates(bit(sys.fastboot) ? gb_state.reg_div.TERO_DIV03p : gb_state.reg_div.UPOF_DIV15p);
  tock_lcdc_gates(reg_old); // LCDC has to be near the top as it controls the video reset signal
  tock_vid_clocks_gates();
  tock_lyc_gates(reg_old);
  tock_lcd_gates();
  tock_joypad_gates(reg_old);
  tock_serial_gates();
  tock_timer_gates(reg_old);
  tock_bootrom_gates();
  tock_dma_gates(reg_old);

  //----------------------------------------


  /*#p21.PARU*/ wire PARU_VBLANKp = not1(gb_state.lcd.POPU_y144p.qn_new());
  /*_p27.REPU*/ wire REPU_VBLANKp = or2(PARU_VBLANKp, PYRY_VID_RSTp());

  /*_p21.TADY*/ wire TADY_LINE_RSTn = nor2(gb_state.ATEJ_LINE_RSTp.out_new(), TOFU_VID_RSTp());
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(gb_state.ATEJ_LINE_RSTp.out_new(), ATAR_VID_RSTp());
  /*#p29.BALU*/ wire BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn = not1(BALU_LINE_RSTp);
  /*_p27.XAHY*/ wire XAHY_LINE_RSTn = not1(gb_state.ATEJ_LINE_RSTp.out_new());

  //----------------------------------------

  //----------------------------------------
  // Sprite scanner

  {
    /*#p29.DOBA*/ gb_state.sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(gb_state.sys_clk.ALET_xBxDxFxH(), BAGY_LINE_RSTn, reg_old.sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
    /*#p29.BYBA*/ gb_state.sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(gb_state.sys_clk.XUPY_ABxxEFxx(), BAGY_LINE_RSTn, reg_old.sprite_scanner.FETO_SCAN_DONEp.out_old());
    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(gb_state.sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp, gb_state.sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ gb_state.sprite_scanner.AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);

    /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(ATAR_VID_RSTp(), gb_state.sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new());
    /*#p29.CENO*/ gb_state.sprite_scanner.CENO_SCANNINGn.dff17(gb_state.sys_clk.XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), reg_old.sprite_scanner.BESU_SCANNINGn.qp_old());
    /*#p28.BESU*/ gb_state.sprite_scanner.BESU_SCANNINGn.nor_latch(gb_state.lcd.CATU_x113p.qp_new(), ASEN_SCAN_DONE_TRIGp);

    /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(gb_state.MATU_DMA_RUNNINGp.qp_new());
    /*#p28.ACYL*/ gb_state.ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, gb_state.sprite_scanner.BESU_SCANNINGn.qp_new());

    // Sprite scanner grabs the sprite index off the _old_ oam address bus
    /*_p28.YFOT*/ wire YFOT_OAM_A2p_old = not1(reg_old.oam_abus.BUS_OAM_A02n.out_old());
    /*_p28.YFOC*/ wire YFOC_OAM_A3p_old = not1(reg_old.oam_abus.BUS_OAM_A03n.out_old());
    /*_p28.YVOM*/ wire YVOM_OAM_A4p_old = not1(reg_old.oam_abus.BUS_OAM_A04n.out_old());
    /*_p28.YMEV*/ wire YMEV_OAM_A5p_old = not1(reg_old.oam_abus.BUS_OAM_A05n.out_old());
    /*_p28.XEMU*/ wire XEMU_OAM_A6p_old = not1(reg_old.oam_abus.BUS_OAM_A06n.out_old());
    /*_p28.YZET*/ wire YZET_OAM_A7p_old = not1(reg_old.oam_abus.BUS_OAM_A07n.out_old());
    /*_p30.XADU*/ gb_state.sprite_index.XADU_SPRITE_IDX0p.dff13(gb_state.sys_clk.WUDA_xxCDxxGH(), gb_state.SIG_VCC.out_new(), YFOT_OAM_A2p_old);
    /*_p30.XEDY*/ gb_state.sprite_index.XEDY_SPRITE_IDX1p.dff13(gb_state.sys_clk.WUDA_xxCDxxGH(), gb_state.SIG_VCC.out_new(), YFOC_OAM_A3p_old);
    /*_p30.ZUZE*/ gb_state.sprite_index.ZUZE_SPRITE_IDX2p.dff13(gb_state.sys_clk.WUDA_xxCDxxGH(), gb_state.SIG_VCC.out_new(), YVOM_OAM_A4p_old);
    /*_p30.XOBE*/ gb_state.sprite_index.XOBE_SPRITE_IDX3p.dff13(gb_state.sys_clk.WUDA_xxCDxxGH(), gb_state.SIG_VCC.out_new(), YMEV_OAM_A5p_old);
    /*_p30.YDUF*/ gb_state.sprite_index.YDUF_SPRITE_IDX4p.dff13(gb_state.sys_clk.WUDA_xxCDxxGH(), gb_state.SIG_VCC.out_new(), XEMU_OAM_A6p_old);
    /*_p30.XECU*/ gb_state.sprite_index.XECU_SPRITE_IDX5p.dff13(gb_state.sys_clk.WUDA_xxCDxxGH(), gb_state.SIG_VCC.out_new(), YZET_OAM_A7p_old);

    // FIXME is this OK without the second assignment of FETO?

    for (int feedback = 0; feedback < 2; feedback++) {
      // 32 + 4 + 2 + 1 = 39
      /*#p28.FETO*/ gb_state.sprite_scanner.FETO_SCAN_DONEp = and4(gb_state.scan_counter.YFEL_SCAN0.qp_mid(), gb_state.scan_counter.WEWY_SCAN1.qp_mid(), gb_state.scan_counter.GOSO_SCAN2.qp_mid(), gb_state.scan_counter.FONY_SCAN5.qp_mid());

      /*#p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(gb_state.sprite_scanner.FETO_SCAN_DONEp.out_new(), gb_state.sys_clk.XUPY_ABxxEFxx());

      /*#p28.YFEL*/ gb_state.scan_counter.YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp,                 ANOM_LINE_RSTn, gb_state.scan_counter.YFEL_SCAN0.qn_any());
      /*_p28.WEWY*/ gb_state.scan_counter.WEWY_SCAN1.dff17_any(gb_state.scan_counter.YFEL_SCAN0.qn_any(), ANOM_LINE_RSTn, gb_state.scan_counter.WEWY_SCAN1.qn_any());
      /*_p28.GOSO*/ gb_state.scan_counter.GOSO_SCAN2.dff17_any(gb_state.scan_counter.WEWY_SCAN1.qn_any(), ANOM_LINE_RSTn, gb_state.scan_counter.GOSO_SCAN2.qn_any());
      /*_p28.ELYN*/ gb_state.scan_counter.ELYN_SCAN3.dff17_any(gb_state.scan_counter.GOSO_SCAN2.qn_any(), ANOM_LINE_RSTn, gb_state.scan_counter.ELYN_SCAN3.qn_any());
      /*_p28.FAHA*/ gb_state.scan_counter.FAHA_SCAN4.dff17_any(gb_state.scan_counter.ELYN_SCAN3.qn_any(), ANOM_LINE_RSTn, gb_state.scan_counter.FAHA_SCAN4.qn_any());
      /*_p28.FONY*/ gb_state.scan_counter.FONY_SCAN5.dff17_any(gb_state.scan_counter.FAHA_SCAN4.qn_any(), ANOM_LINE_RSTn, gb_state.scan_counter.FONY_SCAN5.qn_any());
    }

    ///*#p28.FETO*/ sprite_scanner.FETO_SCAN_DONEp = and4(sprite_scanner.YFEL_SCAN0.qp_new(), sprite_scanner.WEWY_SCAN1.qp_new(), sprite_scanner.GOSO_SCAN2.qp_new(), sprite_scanner.FONY_SCAN5.qp_new());
  }


  //----------------------------------------
  // Global rendering flag 'XYMU'

  /*#p21.VOGA*/ gb_state.VOGA_HBLANKp.dff17(gb_state.sys_clk.ALET_xBxDxFxH(), TADY_LINE_RSTn, reg_old.WODU_HBLANKp.out_old());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), gb_state.VOGA_HBLANKp.qp_new());
  /*#p21.XYMU*/ gb_state.XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, gb_state.sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new());
  /*_p24.LOBY*/ wire LOBY_RENDERINGn = not1(gb_state.XYMU_RENDERINGn.qn_new());
  /*#p27.PAHA*/ wire PAHA_RENDERINGn = not1(gb_state.XYMU_RENDERINGn.qn_new());
  /*_p29.TEPA*/ wire TEPA_RENDERINGp = not1(gb_state.XYMU_RENDERINGn.qn_new());

  //----------------------------------------
  // Sprite fetch sequencer

  {
    /*_p27.TUKU*/ wire TUKU_WIN_HITn_old = not1(TOMU_WIN_HITp_old);
    /*_p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_old = not1(reg_old.sfetch_control.TAKA_SFETCH_RUNNINGp.qp_old());
    /*_p27.TEKY*/ wire TEKY_SFETCH_REQp_old = and4(reg_old.FEPO_STORE_MATCHp.out_old(), TUKU_WIN_HITn_old, reg_old.tfetch_control.LYRY_BFETCH_DONEp.out_old(), SOWO_SFETCH_RUNNINGn_old);
    /*_p27.SUDA*/ gb_state.sfetch_control.SUDA_SFETCH_REQp.dff17(gb_state.sys_clk.LAPE_AxCxExGx(), gb_state.SIG_VCC.out_new(), reg_old.sfetch_control.SOBU_SFETCH_REQp.qp_old());
    /*_p27.SOBU*/ gb_state.sfetch_control.SOBU_SFETCH_REQp.dff17(gb_state.sys_clk.TAVA_xBxDxFxH(), gb_state.SIG_VCC.out_new(), TEKY_SFETCH_REQp_old);

    /*#p29.TYFO*/ gb_state.sfetch_control.TYFO_SFETCH_S0p_D1.dff17(gb_state.sys_clk.LAPE_AxCxExGx(), gb_state.SIG_VCC.out_new(),        reg_old.sfetch_counter.TOXE_SFETCH_S0p.qp_old());
    
    /*#p29.TOBU*/ gb_state.sfetch_control.TOBU_SFETCH_S1p_D2.dff17(gb_state.sys_clk.TAVA_xBxDxFxH(), gb_state.XYMU_RENDERINGn.qn_new(), reg_old.sfetch_counter.TULY_SFETCH_S1p.qp_old());
    /*#p29.VONU*/ gb_state.sfetch_control.VONU_SFETCH_S1p_D4.dff17(gb_state.sys_clk.TAVA_xBxDxFxH(), gb_state.XYMU_RENDERINGn.qn_new(), reg_old.sfetch_control.TOBU_SFETCH_S1p_D2.qp_old());
    /*#p29.SEBA*/ gb_state.sfetch_control.SEBA_SFETCH_S1p_D5.dff17(gb_state.sys_clk.LAPE_AxCxExGx(), gb_state.XYMU_RENDERINGn.qn_new(), reg_old.sfetch_control.VONU_SFETCH_S1p_D4.qp_old());

    /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(gb_state.sfetch_control.SOBU_SFETCH_REQp.qp_new(), gb_state.sfetch_control.SUDA_SFETCH_REQp.qn_new());
    /*#p27.SECA*/ wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), gb_state.ATEJ_LINE_RSTp.out_new());

    // Feedback loop
    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand2(gb_state.sfetch_counter.TESE_SFETCH_S2p.qp_any(), gb_state.sfetch_counter.TOXE_SFETCH_S0p.qp_any());
      /*#p29.TOMA*/ wire TOMA_SFETCH_xBxDxFxH = nand2(gb_state.sys_clk.LAPE_AxCxExGx(), TAME_SFETCH_CLK_GATE);
      /*#p29.TOXE*/ gb_state.sfetch_counter.TOXE_SFETCH_S0p.dff17_any(TOMA_SFETCH_xBxDxFxH,                        SECA_SFETCH_STARTn, gb_state.sfetch_counter.TOXE_SFETCH_S0p.qn_any());
      /*#p29.TULY*/ gb_state.sfetch_counter.TULY_SFETCH_S1p.dff17_any(gb_state.sfetch_counter.TOXE_SFETCH_S0p.qn_any(), SECA_SFETCH_STARTn, gb_state.sfetch_counter.TULY_SFETCH_S1p.qn_any());
      /*#p29.TESE*/ gb_state.sfetch_counter.TESE_SFETCH_S2p.dff17_any(gb_state.sfetch_counter.TULY_SFETCH_S1p.qn_any(), SECA_SFETCH_STARTn, gb_state.sfetch_counter.TESE_SFETCH_S2p.qn_any());
    }

    /*_p29.SAKY*/ wire SAKY_SFETCHn = nor2(gb_state.sfetch_counter.TULY_SFETCH_S1p.qp_new(), gb_state.sfetch_control.VONU_SFETCH_S1p_D4.qp_new());
    /*_p29.TYSO*/ wire TYSO_SFETCHINGn = or2(SAKY_SFETCHn, TEPA_RENDERINGp); // def or
    /*_p29.TEXY*/ gb_state.sfetch_control.TEXY_SFETCHINGp = not1(TYSO_SFETCHINGn);

    /*_p29.TYNO*/ wire TYNO = nand3(gb_state.sfetch_counter.TOXE_SFETCH_S0p.qp_new(), gb_state.sfetch_control.SEBA_SFETCH_S1p_D5.qp_new(), gb_state.sfetch_control.VONU_SFETCH_S1p_D4.qp_new());
    /*_p29.VUSA*/ wire VUSA_SPRITE_DONEn = or2(gb_state.sfetch_control.TYFO_SFETCH_S0p_D1.qn_new(), TYNO);
    /*_p29.WUTY*/ gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp = not1(VUSA_SPRITE_DONEn);
  }

  /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(gb_state.sfetch_control.SOBU_SFETCH_REQp.qp_new(), gb_state.sfetch_control.SUDA_SFETCH_REQp.qn_new());
  /*#p27.SECA*/ wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), gb_state.ATEJ_LINE_RSTp.out_new());

  //----------------------------------------
  // Window state has some interaction with the tile fetcher here.

  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(gb_state.reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn, XAPO_VID_RSTn());
  /*_p27.NUNU*/ gb_state.win_ctrl.NUNU_WIN_MATCHp.dff17(gb_state.sys_clk.MEHE_AxCxExGx(), XAPO_VID_RSTn(), reg_old.win_ctrl.PYCO_WIN_MATCHp.qp_old());
  /*_p27.NOPA*/ gb_state.win_ctrl.NOPA_WIN_MODE_Bp.dff17(gb_state.sys_clk.ALET_xBxDxFxH(), XAPO_VID_RSTn(), reg_old.win_ctrl.PYNU_WIN_MODE_Ap.qp_old());
  /*_p27.PYNU*/ gb_state.win_ctrl.PYNU_WIN_MODE_Ap.nor_latch(gb_state.win_ctrl.NUNU_WIN_MATCHp.qp_new(), XOFO_WIN_RSTp);

  /*#p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and2(gb_state.win_ctrl.PYNU_WIN_MODE_Ap.qp_new(), gb_state.win_ctrl.NOPA_WIN_MODE_Bp.qn_new());
  /*_p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not1(NUNY_WIN_MODE_TRIGp);
  /*_p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not1(NYFO_WIN_MODE_TRIGn);
  /*_p24.NAFY*/ wire NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);

  /*_p24.PYGO*/ gb_state.tfetch_control.PYGO_FETCH_DONEp.dff17(gb_state.sys_clk.ALET_xBxDxFxH(), gb_state.XYMU_RENDERINGn.qn_new(), reg_old.tfetch_control.PORY_FETCH_DONEp.qp_old());
  /*_p24.PORY*/ gb_state.tfetch_control.PORY_FETCH_DONEp.dff17(gb_state.sys_clk.MYVO_AxCxExGx(), NAFY_WIN_MODE_TRIGn, reg_old.tfetch_control.NYKA_FETCH_DONEp.qp_old());
  /*_p24.NYKA*/ gb_state.tfetch_control.NYKA_FETCH_DONEp.dff17(gb_state.sys_clk.ALET_xBxDxFxH(), NAFY_WIN_MODE_TRIGn, reg_old.tfetch_control.LYRY_BFETCH_DONEp.out_old());
  /*_p24.POKY*/ gb_state.tfetch_control.POKY_PRELOAD_LATCHp.nor_latch(gb_state.tfetch_control.PYGO_FETCH_DONEp.qp_new(), LOBY_RENDERINGn);

  /*_p27.SOVY*/ gb_state.win_ctrl.SOVY_WIN_HITp.dff17(gb_state.sys_clk.ALET_xBxDxFxH(), XAPO_VID_RSTn(), reg_old.win_ctrl.RYDY_WIN_HITp.out_old());
  /*_p27.RYDY*/ gb_state.win_ctrl.RYDY_WIN_HITp = nor3(gb_state.win_ctrl.PUKU_WIN_HITn.out_mid(), gb_state.tfetch_control.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ gb_state.win_ctrl.PUKU_WIN_HITn = nor2(gb_state.win_ctrl.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);
  /*_p27.RYDY*/ gb_state.win_ctrl.RYDY_WIN_HITp = nor3(gb_state.win_ctrl.PUKU_WIN_HITn.out_mid(), gb_state.tfetch_control.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ gb_state.win_ctrl.PUKU_WIN_HITn = nor2(gb_state.win_ctrl.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);

  /*#p27.SYLO*/ wire SYLO_WIN_HITn = not1(gb_state.win_ctrl.RYDY_WIN_HITp.out_new());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp = not1(SYLO_WIN_HITn);
  /*#p24.SOCY*/ wire SOCY_WIN_HITn = not1(TOMU_WIN_HITp);

  /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn = not1(gb_state.tfetch_control.POKY_PRELOAD_LATCHp.qp_new());
  /*_p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn = nand4(gb_state.XYMU_RENDERINGn.qn_new(), ROMO_PRELOAD_DONEn, gb_state.tfetch_control.NYKA_FETCH_DONEp.qp_new(), gb_state.tfetch_control.PORY_FETCH_DONEp.qp_new());
  /*_p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp = not1(SUVU_PRELOAD_DONE_TRIGn);

  /*_p27.VEKU*/ wire VEKU_SFETCH_ENDn = nor2(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), TAVE_PRELOAD_DONE_TRIGp); // def nor
  /*_p27.TAKA*/ gb_state.sfetch_control.TAKA_SFETCH_RUNNINGp.nand_latch(SECA_SFETCH_STARTn, VEKU_SFETCH_ENDn);

  //----------------------------------------
  // OAM temp register B stores sprite X coordinate during scan, so we need a copy of it for later.

  auto oam_temp_b_old = reg_old.oam_temp_b;

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  {
    /*_p25.CUFE*/ wire CUFE_OAM_CLKp = not_or_and3(gb_state.cpu_abus.SARO_ADDR_OAMp(), gb_state.MATU_DMA_RUNNINGp.qp_new(), gb_state.sys_clk.MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
    /*_p25.AVER*/ wire AVER_AxxxExxx = nand2(gb_state.ACYL_SCANNINGp.out_new(), gb_state.sys_clk.XYSO_xBCDxFGH());
    /*_p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(gb_state.sfetch_counter.TOXE_SFETCH_S0p.qp_new());
    /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(gb_state.sfetch_control.TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n);
    /*_p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(TEPA_RENDERINGp, gb_state.sfetch_counter.TULY_SFETCH_S1p.qp_new(), gb_state.sfetch_counter.TESE_SFETCH_S2p.qp_new());
    /*_p25.VAPE*/ wire VAPE_OAM_CLKENn = and2(TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
    /*_p25.XUJY*/ wire XUJY_OAM_CLKENp = not1(VAPE_OAM_CLKENn);
    /*_p25.BYCU*/ wire BYCU_OAM_CLKp = nand3(AVER_AxxxExxx, XUJY_OAM_CLKENp, CUFE_OAM_CLKp);
    /*_p25.COTA*/ wire COTA_OAM_CLKn = not1(BYCU_OAM_CLKp);
    oam_latch_to_temp_a_gates(COTA_OAM_CLKn, gb_state.oam_latch_a, gb_state.oam_temp_a);
    oam_latch_to_temp_b_gates(COTA_OAM_CLKn, gb_state.oam_latch_b, gb_state.oam_temp_b);
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  SpriteDeltaY sprite_delta_y = sub_sprite_y_gates();

  {
    /*_p29.GACE*/ wire GACE_SPRITE_DELTA4 = not1(sprite_delta_y.GOPU_YDIFF4.sum);
    /*_p29.GUVU*/ wire GUVU_SPRITE_DELTA5 = not1(sprite_delta_y.FUWA_YDIFF5.sum);
    /*_p29.GYDA*/ wire GYDA_SPRITE_DELTA6 = not1(sprite_delta_y.GOJU_YDIFF6.sum);
    /*_p29.GEWY*/ wire GEWY_SPRITE_DELTA7 = not1(sprite_delta_y.WUHU_YDIFF7.sum);

    /*#p29.GOVU*/ wire GOVU_SPSIZE_MATCH = or2(gb_state.reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new(), sprite_delta_y.GYKY_YDIFF3.sum);
    /*_p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, sprite_delta_y.WUHU_YDIFF7.carry, GOVU_SPSIZE_MATCH);
    /*_p29.GESE*/ wire GESE_SCAN_MATCH_Yp = not1(WOTA_SCAN_MATCH_Yn);
    /*_p29.CEHA*/ wire CEHA_SCANNINGp = not1(gb_state.sprite_scanner.CENO_SCANNINGn.qn_new());
    /*_p29.CARE*/ wire CARE_COUNT_CLKn = and3(gb_state.sys_clk.XOCE_xBCxxFGx(), CEHA_SCANNINGp, GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /*_p29.DYTY*/ wire DYTY_COUNT_CLKp = not1(CARE_COUNT_CLKn);
    /*_p29.DEZY*/ gb_state.sprite_scanner.DEZY_COUNT_CLKp.dff17_any(gb_state.sys_clk.ZEME_AxCxExGx(), XAPO_VID_RSTn(), DYTY_COUNT_CLKp);

    // There's a feedback loop here, but we don't actually need to loop - BAKY holds the clock line high once the sprite store is full, so doing a second logic pass
    // doesn't actually change any of the dffs.
    /*#p29.BAKY*/ wire BAKY_SPRITES_FULL_new = and2(gb_state.sprite_counter.CUXY_SPRITE_COUNT1.qp_any(), gb_state.sprite_counter.DYBE_SPRITE_COUNT3.qp_any());
    /*#p29.CAKE*/ wire CAKE_COUNT_CLKp_new = or2(BAKY_SPRITES_FULL_new, gb_state.sprite_scanner.DEZY_COUNT_CLKp.qp_any());
    /*#p28.AZYB*/ wire AZYB_LINE_TRIGn = not1(gb_state.ATEJ_LINE_RSTp.out_new());
    /*_p29.BESE*/ gb_state.sprite_counter.BESE_SPRITE_COUNT0.dff17_any(CAKE_COUNT_CLKp_new, AZYB_LINE_TRIGn, gb_state.sprite_counter.BESE_SPRITE_COUNT0.qn_any());
    /*_p29.CUXY*/ gb_state.sprite_counter.CUXY_SPRITE_COUNT1.dff17_any(gb_state.sprite_counter.BESE_SPRITE_COUNT0.qn_any(), AZYB_LINE_TRIGn, gb_state.sprite_counter.CUXY_SPRITE_COUNT1.qn_any());
    /*_p29.BEGO*/ gb_state.sprite_counter.BEGO_SPRITE_COUNT2.dff17_any(gb_state.sprite_counter.CUXY_SPRITE_COUNT1.qn_any(), AZYB_LINE_TRIGn, gb_state.sprite_counter.BEGO_SPRITE_COUNT2.qn_any());
    /*_p29.DYBE*/ gb_state.sprite_counter.DYBE_SPRITE_COUNT3.dff17_any(gb_state.sprite_counter.BEGO_SPRITE_COUNT2.qn_any(), AZYB_LINE_TRIGn, gb_state.sprite_counter.DYBE_SPRITE_COUNT3.qn_any());

    /*_p28.ABAK*/ wire ABAK_LINE_RSTp = or2(gb_state.ATEJ_LINE_RSTp.out_new(), AMYG_VID_RSTp());
    /*_p28.BYVA*/ wire BYVA_LINE_RSTn = not1(ABAK_LINE_RSTp);

    //update_sprite_reset_flags(sfetch_control.WUTY_SFETCH_DONE_TRIGp, BYVA_LINE_RSTn, sprite_match_flags, sprite_reset_flags);

    /*_p29.EBOJ*/ gb_state.sprite_reset_flags.EBOJ_STORE0_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GUVA_SPRITE0_GETp.out_old());
    /*_p29.CEDY*/ gb_state.sprite_reset_flags.CEDY_STORE1_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.ENUT_SPRITE1_GETp.out_old());
    /*_p29.EGAV*/ gb_state.sprite_reset_flags.EGAV_STORE2_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.EMOL_SPRITE2_GETp.out_old());
    /*_p29.GOTA*/ gb_state.sprite_reset_flags.GOTA_STORE3_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GYFY_SPRITE3_GETp.out_old());
    /*_p29.XUDY*/ gb_state.sprite_reset_flags.XUDY_STORE4_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GONO_SPRITE4_GETp.out_old());
    /*_p29.WAFY*/ gb_state.sprite_reset_flags.WAFY_STORE5_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GEGA_SPRITE5_GETp.out_old());
    /*_p29.WOMY*/ gb_state.sprite_reset_flags.WOMY_STORE6_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.XOJA_SPRITE6_GETp.out_old());
    /*_p29.WAPO*/ gb_state.sprite_reset_flags.WAPO_STORE7_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GUTU_SPRITE7_GETp.out_old());
    /*_p29.EXUQ*/ gb_state.sprite_reset_flags.EXUQ_STORE8_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.FOXA_SPRITE8_GETp.out_old());
    /*_p29.FONO*/ gb_state.sprite_reset_flags.FONO_STORE9_RSTp.dff17(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GUZE_SPRITE9_GETp.out_old());

    SpriteStoreFlags sprite_store_flags_old = reg_old.sprite_store_flags;

    update_sprite_store_flags_gates(gb_state.sprite_counter, DYTY_COUNT_CLKp, gb_state.sprite_store_flags);
    store_sprite_gates(sprite_store_flags_old, gb_state.sprite_store_flags, gb_state.sprite_reset_flags, BYVA_LINE_RSTn, oam_temp_b_old);
  }

  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  auto XYDO_PX3p_old = reg_old.pix_count.XYDO_PX3p;

  // Weird feedback loop here - fine scroll and pix count affect store match, which affects pipe clock, which affects fine scroll and pix count.
  // Implementing the feedback doesn't seem to change any test results, but... paranoia...

  //for (int feedback = 0; feedback < 2; feedback++) {
  {
    /*_p27.SUHA*/ wire SUHA_SCX_FINE_MATCHp_old = xnor2(reg_old.reg_scx.DATY_SCX0n.qn_old(), reg_old.fine_count.RYKU_FINE_CNT0.qp_old());
    /*_p27.SYBY*/ wire SYBY_SCX_FINE_MATCHp_old = xnor2(reg_old.reg_scx.DUZU_SCX1n.qn_old(), reg_old.fine_count.ROGA_FINE_CNT1.qp_old());
    /*_p27.SOZU*/ wire SOZU_SCX_FINE_MATCHp_old = xnor2(reg_old.reg_scx.CYXU_SCX2n.qn_old(), reg_old.fine_count.RUBU_FINE_CNT2.qp_old());
    /*#p27.RONE*/ wire RONE_SCX_FINE_MATCHn_old = nand4(gb_state.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any(), SUHA_SCX_FINE_MATCHp_old, SYBY_SCX_FINE_MATCHp_old, SOZU_SCX_FINE_MATCHp_old);
    /*#p27.POHU*/ wire POHU_SCX_FINE_MATCHp_old = not1(RONE_SCX_FINE_MATCHn_old);

    /*_p29.CEHA*/ wire CEHA_SCANNINGp = not1(gb_state.sprite_scanner.CENO_SCANNINGn.qn_new());
    /*#p29.BYJO*/ wire BYJO_SCANNINGn = not1(CEHA_SCANNINGp);
    /*#p29.AZEM*/ wire AZEM_RENDERINGp = and2(gb_state.XYMU_RENDERINGn.qn_new(), BYJO_SCANNINGn);
    /*#p29.AROR*/ wire AROR_MATCH_ENp = and2(AZEM_RENDERINGp, gb_state.reg_lcdc.XYLO_LCDC_SPENn.qn_new());

    /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(gb_state.FEPO_STORE_MATCHp.out_mid(), reg_old.WODU_HBLANKp.out_old(), gb_state.sys_clk.MYVO_AxCxExGx());
    /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, gb_state.tfetch_control.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
    /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);

    /*#p27.NYZE*/ gb_state.fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17_any(gb_state.sys_clk.MOXE_AxCxExGx(), gb_state.XYMU_RENDERINGn.qn_new(), gb_state.fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any());
    /*#p27.PUXA*/ gb_state.fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17_any(ROXO_CLKPIPE_odd, gb_state.XYMU_RENDERINGn.qn_new(), POHU_SCX_FINE_MATCHp_old);

    /*#p27.POVA*/ wire POVA_FINE_MATCH_TRIGp = and2(gb_state.fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any(), gb_state.fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_any());
    /*#p27.ROXY*/ gb_state.fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(PAHA_RENDERINGn, POVA_FINE_MATCH_TRIGp);

    /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, gb_state.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any());
    // Pixel counter, has carry lookahead because this can increment every tcycle
    /*_p21.RYBO*/ wire RYBO_old = xor2(reg_old.pix_count.XEHO_PX0p.qp_any(), reg_old.pix_count.SAVY_PX1p.qp_any()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /*_p21.XUKE*/ wire XUKE_old = and2(reg_old.pix_count.XEHO_PX0p.qp_any(), reg_old.pix_count.SAVY_PX1p.qp_any());
    /*_p21.XYLE*/ wire XYLE_old = and2(reg_old.pix_count.XODU_PX2p.qp_any(), XUKE_old);
    /*_p21.XEGY*/ wire XEGY_old = xor2(reg_old.pix_count.XODU_PX2p.qp_any(), XUKE_old); // feet facing gnd
    /*_p21.XORA*/ wire XORA_old = xor2(reg_old.pix_count.XYDO_PX3p.qp_any(), XYLE_old); // feet facing gnd

    /*_p21.XEHO*/ gb_state.pix_count.XEHO_PX0p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, gb_state.pix_count.XEHO_PX0p.qn_any());
    /*_p21.SAVY*/ gb_state.pix_count.SAVY_PX1p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, RYBO_old);
    /*_p21.XODU*/ gb_state.pix_count.XODU_PX2p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEGY_old);
    /*_p21.XYDO*/ gb_state.pix_count.XYDO_PX3p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XORA_old);

    /*_p24.TOCA*/ wire TOCA_new = not1(gb_state.pix_count.XYDO_PX3p.qp_any());
    /*_p21.SAKE*/ wire SAKE_old = xor2(reg_old.pix_count.TUHU_PX4p.qp_any(), reg_old.pix_count.TUKY_PX5p.qp_any());
    /*_p21.TYBA*/ wire TYBA_old = and2(reg_old.pix_count.TUHU_PX4p.qp_any(), reg_old.pix_count.TUKY_PX5p.qp_any());
    /*_p21.SURY*/ wire SURY_old = and2(reg_old.pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /*_p21.TYGE*/ wire TYGE_old = xor2(reg_old.pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /*_p21.ROKU*/ wire ROKU_old = xor2(reg_old.pix_count.SYBE_PX7p.qp_any(), SURY_old);

    /*_p21.TUHU*/ gb_state.pix_count.TUHU_PX4p.dff17_any(TOCA_new, TADY_LINE_RSTn, gb_state.pix_count.TUHU_PX4p.qn_any());
    /*_p21.TUKY*/ gb_state.pix_count.TUKY_PX5p.dff17_any(TOCA_new, TADY_LINE_RSTn, SAKE_old);
    /*_p21.TAKO*/ gb_state.pix_count.TAKO_PX6p.dff17_any(TOCA_new, TADY_LINE_RSTn, TYGE_old);
    /*_p21.SYBE*/ gb_state.pix_count.SYBE_PX7p.dff17_any(TOCA_new, TADY_LINE_RSTn, ROKU_old);

    get_sprite_match_flags_gates(AROR_MATCH_ENp, gb_state.sprite_match_flags);
  }

  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  {
    /*#p21.XENA*/ wire XENA_STORE_MATCHn = not1(gb_state.FEPO_STORE_MATCHp.out_new());
    /*#p21.XUGU*/ wire XUGU_PX167n = nand5(gb_state.pix_count.XEHO_PX0p.qp_new(), gb_state.pix_count.SAVY_PX1p.qp_new(), gb_state.pix_count.XODU_PX2p.qp_new(), gb_state.pix_count.TUKY_PX5p.qp_new(), gb_state.pix_count.SYBE_PX7p.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire XANO_PX167p = not1(XUGU_PX167n);
    /*#p21.WODU*/ gb_state.WODU_HBLANKp = and2(XENA_STORE_MATCHn, XANO_PX167p); // WODU goes high on odd, cleared on H
  }

  /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(gb_state.FEPO_STORE_MATCHp.out_new(), gb_state.WODU_HBLANKp.out_new(), gb_state.sys_clk.MYVO_AxCxExGx());
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, gb_state.tfetch_control.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);
  /*#p24.SACU*/ wire SACU_CLKPIPE_new = or2(SEGU_CLKPIPE_evn, gb_state.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  /*_p24.PAHO*/ gb_state.lcd.PAHO_X_8_SYNC.dff17(ROXO_CLKPIPE_odd, gb_state.XYMU_RENDERINGn.qn_new(), XYDO_PX3p_old.qp_old());

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  memset(&gb_state.sprite_ibus, BIT_NEW | BIT_PULLED | 1, sizeof(gb_state.sprite_ibus));
  memset(&gb_state.sprite_lbus, BIT_NEW | BIT_PULLED | 1, sizeof(gb_state.sprite_lbus));

  sprite_match_to_bus_gates(gb_state.sprite_match_flags);
  sprite_scan_to_bus_gates(sprite_delta_y, gb_state.XYMU_RENDERINGn, gb_state.FEPO_STORE_MATCHp);

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // WY/WX/window match

  tock_window_gates(reg_old, SEGU_CLKPIPE_evn, REPU_VBLANKp);

  //----------------------------------------
  // Tile fetch sequencer

  /*_p27.LYZU*/ gb_state.tfetch_control.LYZU_BFETCH_S0p_D1.dff17(gb_state.sys_clk.ALET_xBxDxFxH(), gb_state.XYMU_RENDERINGn.qn_new(), reg_old.tfetch_counter.LAXU_BFETCH_S0p.qp_old());

  /*_p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp = nor2(gb_state.win_ctrl.RYFA_WIN_FETCHn_A.qn_new(), gb_state.win_ctrl.RENE_WIN_FETCHn_B.qp_new());
  /*_p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne = nand2(SYLO_WIN_HITn, gb_state.win_ctrl.SOVY_WIN_HITp.qp_new());
  /*_p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne = not1(TUXY_WIN_FIRST_TILEne);
  /*_p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp = or3(SEKO_WIN_FETCH_TRIGp, SUZU_WIN_FIRST_TILEne, TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  /*_p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(gb_state.sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new(), MOSU_WIN_MODE_TRIGp, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(gb_state.tfetch_counter.LAXU_BFETCH_S0p.qp_any(), gb_state.tfetch_counter.NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn);
    /*_p27.LEBO*/ wire LEBO_AxCxExGx = nand2(gb_state.sys_clk.ALET_xBxDxFxH(), MOCE_BFETCH_DONEn);

    /*_p27.LAXU*/ gb_state.tfetch_counter.LAXU_BFETCH_S0p.dff17_any(LEBO_AxCxExGx,                               NYXU_BFETCH_RSTn, gb_state.tfetch_counter.LAXU_BFETCH_S0p.qn_any());
    /*_p27.MESU*/ gb_state.tfetch_counter.MESU_BFETCH_S1p.dff17_any(gb_state.tfetch_counter.LAXU_BFETCH_S0p.qn_any(), NYXU_BFETCH_RSTn, gb_state.tfetch_counter.MESU_BFETCH_S1p.qn_any());
    /*_p27.NYVA*/ gb_state.tfetch_counter.NYVA_BFETCH_S2p.dff17_any(gb_state.tfetch_counter.MESU_BFETCH_S1p.qn_any(), NYXU_BFETCH_RSTn, gb_state.tfetch_counter.NYVA_BFETCH_S2p.qn_any());
  }

  /*_p27.LOVY*/ gb_state.tfetch_control.LOVY_FETCH_DONEp.dff17(gb_state.sys_clk.MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, reg_old.tfetch_control.LYRY_BFETCH_DONEp.out_old());

  /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(gb_state.tfetch_counter.LAXU_BFETCH_S0p.qp_new(), gb_state.tfetch_counter.NYVA_BFETCH_S2p.qp_new(), NYXU_BFETCH_RSTn);
  /*_p27.LYRY*/ gb_state.tfetch_control.LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  /*_p27.LURY*/ wire LURY_BG_FETCH_DONEn = and2(gb_state.tfetch_control.LOVY_FETCH_DONEp.qn_new(), gb_state.XYMU_RENDERINGn.qn_new());
  /*_p27.LONY*/ gb_state.tfetch_control.LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, LURY_BG_FETCH_DONEn);


  //----------------------------------------
  // Fine match counter

  /*#p27.PASO*/ wire PASO_FINE_RST = nor2(PAHA_RENDERINGn, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p27.ROZE*/ wire ROZE_FINE_COUNT_7n = nand3(gb_state.fine_count.RUBU_FINE_CNT2.qp_any(), gb_state.fine_count.ROGA_FINE_CNT1.qp_any(), gb_state.fine_count.RYKU_FINE_CNT0.qp_any());
    /*#p27.PECU*/ wire PECU_FINE_CLK = nand2(ROXO_CLKPIPE_odd, ROZE_FINE_COUNT_7n);
    /*#p27.RYKU*/ gb_state.fine_count.RYKU_FINE_CNT0.dff17_any(PECU_FINE_CLK, PASO_FINE_RST, gb_state.fine_count.RYKU_FINE_CNT0.qn_any());
    /*#p27.ROGA*/ gb_state.fine_count.ROGA_FINE_CNT1.dff17_any(gb_state.fine_count.RYKU_FINE_CNT0.qn_any(), PASO_FINE_RST, gb_state.fine_count.ROGA_FINE_CNT1.qn_any());
    /*#p27.RUBU*/ gb_state.fine_count.RUBU_FINE_CNT2.dff17_any(gb_state.fine_count.ROGA_FINE_CNT1.qn_any(), PASO_FINE_RST, gb_state.fine_count.RUBU_FINE_CNT2.qn_any());
  }

  //----------------------------------------
  // PPU / LCD output

  tock_pix_pipes_gates(reg_old, SACU_CLKPIPE_new, NYXU_BFETCH_RSTn);
  set_lcd_pins_gates(SACU_CLKPIPE_new);

  //----------------------------------------
  // Audio

  tock_spu_gates();

  //----------------------------------------
  // Memory buses

  tock_ext_gates(cart_blob);
  tock_vram_bus_gates(reg_old, TEVO_WIN_FETCH_TRIGp);
  tock_oam_bus_gates();
  tock_zram_gates(reg_old);

  //----------------------------------------
  // And finally, interrupts.

  tock_interrupts_gates(reg_old);

  commit();
}

//------------------------------------------------------------------------------------------------------------------------
