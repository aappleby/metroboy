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

  // Can't run logic mode during power-on-reset, so save and restore it.
  bool old_logic_mode = logic_mode;
  logic_mode = false;

  // Put some recognizable pattern in vram so we can see that we're in the bootrom
  for (int i = 0; i < 8192; i++) {
    uint32_t h = i * 0x1234567;
    vid_ram[i] = uint8_t(h ^ (h >> 4));
  }

  cpu_abus_new.reset_to_bootrom();
  cpu_dbus_new.reset_to_bootrom();

  sprite_ibus.reset_to_bootrom();
  sprite_lbus.reset_to_bootrom();

  dma_hi.NAFA_DMA_A08n.state = 0b00011010;
  dma_hi.PYNE_DMA_A09n.state = 0b00011010;
  dma_hi.PARA_DMA_A10n.state = 0b00011010;
  dma_hi.NYDO_DMA_A11n.state = 0b00011010;
  dma_hi.NYGY_DMA_A12n.state = 0b00011010;
  dma_hi.PULA_DMA_A13n.state = 0b00011010;
  dma_hi.POKU_DMA_A14n.state = 0b00011010;
  dma_hi.MARU_DMA_A15n.state = 0b00011010;

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

  joy_int.reset_to_bootrom();
  joy_reg.reset_to_bootrom();

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

  uint8_t data_out;
  dbg_read(cart_blob, 0x0000, data_out);

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

  logic_mode = old_logic_mode;
  if (logic_mode) wipe_flags();
}

//-----------------------------------------------------------------------------

void GateBoy::reset_to_cart(const blob& cart_blob) {
  reset_to_bootrom(cart_blob, true);

  VOGA_HBLANKp.state = 0b00011001;

  cpu_signals.reset_to_cart();
  cpu_abus_old.reset_to_cart_new();
  cpu_dbus_old.reset_to_cart_new();
  cpu_abus_new.reset_to_cart_new();
  cpu_dbus_new.reset_to_cart_new();

  vram_abus.reset_to_cart();
  vram_dbus.reset_to_cart();
  vram_ext_ctrl.reset_to_cart();
  vram_ext_abus.reset_to_cart();
  vram_ext_dbus.reset_to_cart();

  sprite_ibus.reset_to_cart();
  sprite_lbus.reset_to_cart();

  oam_ctrl.reset_to_cart();
  oam_abus.reset_to_cart();
  oam_dbus_a.reset_to_cart();
  oam_dbus_b.reset_to_cart();
  oam_latch_a.reset_to_cart();
  oam_latch_b.reset_to_cart();
  oam_temp_a.reset_to_cart();
  oam_temp_b.reset_to_cart();

  ext_ctrl.reset_to_cart();
  ext_abus.reset_to_cart();
  ext_dbus.reset_to_cart();
  ext_addr_latch.reset_to_cart();
  ext_data_latch.reset_to_cart();

  //zram_bus.reset_to_cart();



  rst.reset_to_cart();
  clk.reset_to_cart();
  div.reset_to_cart();
  
  //interrupts.reset_to_cart();
  reg_if.reset_to_cart();
  reg_ie.reset_to_cart();
  latch_if.reset_to_cart();
  cpu_int.reset_to_cart();
  cpu_ack.reset_to_cart();

  //serial.reset_to_cart();

  //reset_sprite_store();
  DEZY_COUNT_CLKp.state = 0b00011011;
  sprite_counter.BESE_SPRITE_COUNT0.state = 0b00011010;
  sprite_counter.CUXY_SPRITE_COUNT1.state = 0b00011010;
  sprite_counter.BEGO_SPRITE_COUNT2.state = 0b00011010;
  sprite_counter.DYBE_SPRITE_COUNT3.state = 0b00011010;

  sprite_scanner.reset_to_cart();
  scan_counter.reset_to_cart();
  sprite_index.reset_to_cart();


  sprite_fetcher.reset_to_cart();

  RUPO_LYC_MATCHn.state = 0b00011000;

  reg_stat.reset_to_cart();
  pix_count.reset_to_cart();
  mask_pipe.reset_to_cart();
  REMY_LD0n.state = 0b00011000;
  RAVO_LD1n.state = 0b00011000;

  dma_lo.reset_to_cart();
  dma_hi.reset_to_cart();
  dma_ctrl.reset_to_cart();

  reg_bgp.reset_to_cart();
  reg_obp0.reset_to_cart();
  reg_obp1.reset_to_cart();
  
  //joy.reset_to_cart();
  joy_int.reset_to_cart();
  joy_reg.reset_to_cart();

  reg_lcdc.reset_to_cart();
  lcd.reset_to_cart();

  reg_lx.reset_to_cart();
  reg_ly.reset_to_cart();

  ROPO_LY_MATCH_SYNCp.state = 0b00011001;
  reg_lyc.reset_to_cart();

  WODU_HBLANKp.state = 0b00011001;

  sprite_scanner.FETO_SCAN_DONEp.state = 0b00011001;
  ATEJ_LINE_RSTp.state = 0b00011000;

  //reg_NR50.reset_to_cart();
  //reg_NR51.reset_to_cart();
  //reg_NR52.reset_to_cart();

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

  if (logic_mode) wipe_flags();
}

//------------------------------------------------------------------------------------------------------------------------

bool GateBoy::dbg_read(const blob& cart_blob, int addr, uint8_t& out) {
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

  out = cpu_data_latch;
  return true;
}

//------------------------------------------------------------------------------

bool GateBoy::dbg_write(const blob& cart_blob, int addr, uint8_t data) {
  CHECK_P((phase_total & 7) == 0);

  Req old_req = bus_req_new;
  bool old_cpu_en = sys_cpu_en;
  sys_cpu_en = false;

  bus_req_new.addr = uint16_t(addr);
  bus_req_new.data = data;
  bus_req_new.read = 0;
  bus_req_new.write = 1;
  run_phases(cart_blob, 8);

  bus_req_new = old_req;
  sys_cpu_en = old_cpu_en;
  return true;
}

//------------------------------------------------------------------------------------------------------------------------

struct MemberOffset {
  const char* name;
  int offset;
};

#define GEN_OFFSET(A) { #A, offsetof(GateBoy, A) }

MemberOffset gb_offsets[] = {
  GEN_OFFSET(sentinel1),
  GEN_OFFSET(SIG_VCC),
  GEN_OFFSET(SIG_GND),
  GEN_OFFSET(cpu_signals),
  GEN_OFFSET(cpu_abus_old),
  GEN_OFFSET(cpu_dbus_old),
  GEN_OFFSET(cpu_abus_new),
  GEN_OFFSET(cpu_dbus_new),
  GEN_OFFSET(vram_abus),
  GEN_OFFSET(vram_dbus),
  GEN_OFFSET(vram_ext_ctrl),
  GEN_OFFSET(vram_ext_abus),
  GEN_OFFSET(vram_ext_dbus),
  GEN_OFFSET(sprite_ibus),
  GEN_OFFSET(sprite_lbus),
  GEN_OFFSET(oam_ctrl),
  GEN_OFFSET(oam_abus),
  GEN_OFFSET(oam_dbus_a),
  GEN_OFFSET(oam_dbus_b),
  GEN_OFFSET(oam_latch_a),
  GEN_OFFSET(oam_latch_b),
  GEN_OFFSET(oam_temp_a),
  GEN_OFFSET(oam_temp_b),
  GEN_OFFSET(ext_ctrl),
  GEN_OFFSET(ext_abus),
  GEN_OFFSET(ext_dbus),
  GEN_OFFSET(ext_data_latch),
  GEN_OFFSET(ext_addr_latch),
  GEN_OFFSET(ext_mbc),
  GEN_OFFSET(zram_bus),
  GEN_OFFSET(VOGA_HBLANKp),
  GEN_OFFSET(XYMU_RENDERINGn),
  GEN_OFFSET(rst),
  GEN_OFFSET(clk),
  GEN_OFFSET(div),
  GEN_OFFSET(tima),
  GEN_OFFSET(tma),
  GEN_OFFSET(tac),
  GEN_OFFSET(dma_ctrl),
  GEN_OFFSET(dma_lo),
  GEN_OFFSET(dma_hi),
  GEN_OFFSET(int_ctrl),
  GEN_OFFSET(reg_if),
  GEN_OFFSET(reg_ie),
  GEN_OFFSET(latch_if),
  GEN_OFFSET(cpu_int),
  GEN_OFFSET(cpu_ack),
  GEN_OFFSET(joy_int),
  GEN_OFFSET(joy_reg),
  GEN_OFFSET(joy_latch),
  GEN_OFFSET(joy_ext),
  //GEN_OFFSET(serial),
  GEN_OFFSET(store_i0),
  GEN_OFFSET(store_i1),
  GEN_OFFSET(store_i2),
  GEN_OFFSET(store_i3),
  GEN_OFFSET(store_i4),
  GEN_OFFSET(store_i5),
  GEN_OFFSET(store_i6),
  GEN_OFFSET(store_i7),
  GEN_OFFSET(store_i8),
  GEN_OFFSET(store_i9),
  GEN_OFFSET(store_l0),
  GEN_OFFSET(store_l1),
  GEN_OFFSET(store_l2),
  GEN_OFFSET(store_l3),
  GEN_OFFSET(store_l4),
  GEN_OFFSET(store_l5),
  GEN_OFFSET(store_l6),
  GEN_OFFSET(store_l7),
  GEN_OFFSET(store_l8),
  GEN_OFFSET(store_l9),
  GEN_OFFSET(store_x0),
  GEN_OFFSET(store_x1),
  GEN_OFFSET(store_x2),
  GEN_OFFSET(store_x3),
  GEN_OFFSET(store_x4),
  GEN_OFFSET(store_x5),
  GEN_OFFSET(store_x6),
  GEN_OFFSET(store_x7),
  GEN_OFFSET(store_x8),
  GEN_OFFSET(store_x9),
  GEN_OFFSET(DEZY_COUNT_CLKp),
  GEN_OFFSET(sprite_counter),
  GEN_OFFSET(FEPO_STORE_MATCHp),
  GEN_OFFSET(sprite_match_flags),
  GEN_OFFSET(sprite_reset_flags),
  GEN_OFFSET(sprite_store_flags),
  GEN_OFFSET(sprite_scanner),
  GEN_OFFSET(scan_counter),
  GEN_OFFSET(sprite_index),
  GEN_OFFSET(sprite_fetcher),
  GEN_OFFSET(sprite_pix_a),
  GEN_OFFSET(sprite_pix_b),
  GEN_OFFSET(tile_fetcher),
  GEN_OFFSET(tile_temp_a),
  GEN_OFFSET(tile_temp_b),
  GEN_OFFSET(RUPO_LYC_MATCHn),
  GEN_OFFSET(reg_lcdc),
  GEN_OFFSET(reg_stat),
  GEN_OFFSET(reg_scx),
  GEN_OFFSET(reg_scy),
  GEN_OFFSET(reg_wy),
  GEN_OFFSET(reg_wx),
  GEN_OFFSET(win_x),
  GEN_OFFSET(win_y),
  GEN_OFFSET(win_reg),
  GEN_OFFSET(fine_scroll),
  GEN_OFFSET(pix_count),
  GEN_OFFSET(mask_pipe),
  GEN_OFFSET(bgw_pipe_a),
  GEN_OFFSET(bgw_pipe_b),
  GEN_OFFSET(spr_pipe_a),
  GEN_OFFSET(spr_pipe_b),
  GEN_OFFSET(pal_pipe),
  GEN_OFFSET(REMY_LD0n),
  GEN_OFFSET(RAVO_LD1n),
  GEN_OFFSET(lcd),
  GEN_OFFSET(reg_lx),
  GEN_OFFSET(reg_ly),
  GEN_OFFSET(ROPO_LY_MATCH_SYNCp),
  GEN_OFFSET(reg_lyc),
  GEN_OFFSET(reg_bgp),
  GEN_OFFSET(reg_obp0),
  GEN_OFFSET(reg_obp1),
  GEN_OFFSET(WODU_HBLANKp),
  GEN_OFFSET(SATO_BOOT_BITn),
  GEN_OFFSET(ATEJ_LINE_RSTp),
  GEN_OFFSET(flipped_sprite),
  GEN_OFFSET(sentinel2),
  GEN_OFFSET(gb_cpu),
  GEN_OFFSET(bus_req_new),
  GEN_OFFSET(cpu_data_latch),
  GEN_OFFSET(imask_latch),
  GEN_OFFSET(intf_latch),
  GEN_OFFSET(intf_latch_delay),
  GEN_OFFSET(intf_halt_latch),
  GEN_OFFSET(vid_ram),
  GEN_OFFSET(cart_ram),
  GEN_OFFSET(int_ram),
  GEN_OFFSET(oam_ram),
  GEN_OFFSET(zero_ram),
  GEN_OFFSET(framebuffer),
  GEN_OFFSET(sentinel3),
  GEN_OFFSET(sys_rst),
  GEN_OFFSET(sys_t1),
  GEN_OFFSET(sys_t2),
  GEN_OFFSET(sys_clken),
  GEN_OFFSET(sys_clkgood),
  GEN_OFFSET(sys_clkreq),
  GEN_OFFSET(sys_cpu_en),
  GEN_OFFSET(sys_fastboot),
  GEN_OFFSET(sys_buttons),
  GEN_OFFSET(logic_mode),
  GEN_OFFSET(phase_total),
  GEN_OFFSET(sim_time),
  GEN_OFFSET(phase_origin),
  GEN_OFFSET(probes),
  GEN_OFFSET(sentinel4),
};

void print_field_at(int offset) {
  int len = sizeof(gb_offsets) / sizeof(gb_offsets[0]);

  int min_idx = -1;
  int min_delta = 1000000;

  for (int i = 0; i < len; i++) {
    int delta = offset - gb_offsets[i].offset;
    if (delta >= 0 && delta < min_delta) {
      min_idx = i;
      min_delta = delta;
    }
  }

  if (min_idx >= 0) {
    LOG_R("%12.12s + %d", gb_offsets[min_idx].name, min_delta);
  }
  else {
    LOG_R("<could not find field>");
  }
}

void diff_gb(GateBoy* gba, GateBoy* gbb, uint8_t mask) {
  TestResults results;

  uint8_t* bytes_a = (uint8_t*)gba;
  uint8_t* bytes_b = (uint8_t*)gbb;

  for (int i = 0; i < sizeof(GateBoy); i++) {
    // Ignore the logic mode flag, that's _supposed_ to be different.
    if (i == offsetof(GateBoy, logic_mode)) continue;

    int byte_a = bytes_a[i] & mask;
    int byte_b = bytes_b[i] & mask;

    if (byte_a != byte_b) {
      LOG_R("MISMATCH @ %5d - ", i);
      print_field_at(i);
      LOG_R(": 0x%02x 0x%02x\n", byte_a, byte_b);
    }

    //EXPECT_EQ(byte_a, byte_b, ": [%5d] = 0x%02x, [%5d] = 0x%02x\n", ia, byte_a, ib, byte_b);
  }
}

//------------------------------------------------------------------------------------------------------------------------

bool GateBoy::next_phase(const blob& cart_blob) {
  CHECK_N(!sys_clkreq && logic_mode);

  tock_cpu();

  if (logic_mode) {
    tock_logic(cart_blob);
    update_framebuffer(bit_pack(pix_count) - 8, bit_pack(reg_ly), ~lcd.PIN_51_LCD_DATA0.state, ~lcd.PIN_50_LCD_DATA1.state);
    phase_total++;
    return true;
  }
  else {
    tock_gates(cart_blob);
    update_framebuffer(bit_pack(pix_count) - 8, bit_pack(reg_ly), lcd.PIN_51_LCD_DATA0.qp_ext_old(), lcd.PIN_50_LCD_DATA1.qp_ext_old());
    phase_total++;
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
  cpu_data_latch &= (uint8_t)bit_pack(cpu_dbus_new);
  imask_latch = (uint8_t)bit_pack(reg_ie);

  if (DELTA_HA) {
    if (gb_cpu.op == 0x76 && (imask_latch & intf_halt_latch)) gb_cpu.state_ = 0;
    intf_halt_latch = 0;
  }

  // +ha -ab -bc -cd -de -ef -fg -gh
  if (DELTA_HA) {
    // this one latches funny, some hardware bug
    if (bit(reg_if.NYBO_FF0F_D2p.state)) intf_halt_latch |= INT_TIMER_MASK;
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
    if (bit(reg_if.LOPE_FF0F_D0p.state)) intf_halt_latch |= INT_VBLANK_MASK;
    if (bit(reg_if.LALU_FF0F_D1p.state)) intf_halt_latch |= INT_STAT_MASK;
    if (bit(reg_if.UBUL_FF0F_D3p.state)) intf_halt_latch |= INT_SERIAL_MASK;
    if (bit(reg_if.ULAK_FF0F_D4p.state)) intf_halt_latch |= INT_JOYPAD_MASK;
  }

  // -ha -ab -bc -cd -de -ef +fg +gh
  if (DELTA_GH) {
    cpu_data_latch = 0xFF;
  }

  // +ha -ab -bc -cd -de -ef -fg +gh
  if (DELTA_GH) {
    intf_latch = (uint8_t)bit_pack(reg_if);
  }
}

//------------------------------------------------------------------------------------------------------------------------































































//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_gates(const blob& cart_blob) {

  //----------

  /*#p27.SYLO*/ wire SYLO_WIN_HITn_old = not1(win_reg.RYDY_WIN_HITp.out_old());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp_old = not1(SYLO_WIN_HITn_old);

  //----------

  wire EXT_vcc = 1;
  wire EXT_gnd = 0;

  /*_SIG_VCC*/ SIG_VCC.sig_in(EXT_vcc);
  /*_SIG_GND*/ SIG_GND.sig_in(EXT_gnd);

  //-----------------------------------------------------------------------------

  memset(&cpu_abus_new, BIT_NEW | BIT_PULLED | 1, sizeof(cpu_abus_new));
  memset(&cpu_dbus_new, BIT_NEW | BIT_PULLED | 1, sizeof(cpu_dbus_new));

  //-----------------------------------------------------------------------------

  {
    bool EXT_cpu_latch_ext;

    if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      // Data has to be driven on EFGH or we fail the wave tests
      cpu_dbus_new.set_data(bus_req_new.write, bus_req_new.data_lo);
      EXT_cpu_latch_ext = bus_req_new.read;
    }
    else {
      cpu_dbus_new.set_data(false, 0);
      EXT_cpu_latch_ext = 0;
    }
    /*_SIG_IN_CPU_LATCH_EXT*/ cpu_signals.SIG_IN_CPU_LATCH_EXT.sig_in(EXT_cpu_latch_ext);

    bool EXT_addr_new = (bus_req_new.read || bus_req_new.write);
    bool in_bootrom = bit(~cpu_signals.TEPU_BOOT_BITn.qp_old());
    bool addr_boot = (bus_req_new.addr <= 0x00FF) && in_bootrom;
    bool addr_vram = (bus_req_new.addr >= 0x8000) && (bus_req_new.addr < 0x9FFF);
    bool addr_high = (bus_req_new.addr >= 0xFE00);

    bool EXT_cpu_rd;
    bool EXT_cpu_wr;

    if (DELTA_HA) {
      EXT_cpu_rd = 0;
      EXT_cpu_wr = 0;
      cpu_abus_new.set_addr(bus_req_new.addr & 0x00FF);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
      if (addr_vram) EXT_addr_new = false;
    }
    else {
      EXT_cpu_rd = bus_req_new.read;
      EXT_cpu_wr = bus_req_new.write;
      cpu_abus_new.set_addr(bus_req_new.addr);

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

    wire EXT_ack_vblank = get_bit(gb_cpu.int_ack, BIT_VBLANK);
    wire EXT_ack_stat = get_bit(gb_cpu.int_ack, BIT_STAT);
    wire EXT_ack_timer = get_bit(gb_cpu.int_ack, BIT_TIMER);
    wire EXT_ack_serial = get_bit(gb_cpu.int_ack, BIT_SERIAL);
    wire EXT_ack_joypad = get_bit(gb_cpu.int_ack, BIT_JOYPAD);

    /*_SIG_CPU_ACK_VBLANK*/ cpu_ack.SIG_CPU_ACK_VBLANK.sig_in(EXT_ack_vblank);
    /*_SIG_CPU_ACK_STAT  */ cpu_ack.SIG_CPU_ACK_STAT.sig_in(EXT_ack_stat);
    /*_SIG_CPU_ACK_TIMER */ cpu_ack.SIG_CPU_ACK_TIMER.sig_in(EXT_ack_timer);
    /*_SIG_CPU_ACK_SERIAL*/ cpu_ack.SIG_CPU_ACK_SERIAL.sig_in(EXT_ack_serial);
    /*_SIG_CPU_ACK_JOYPAD*/ cpu_ack.SIG_CPU_ACK_JOYPAD.sig_in(EXT_ack_joypad);

    /*_SIG_CPU_CLKREQ*/ clk.SIG_CPU_CLKREQ.sig_in(EXT_sys_clkreq);

    /*_SIG_CPU_ADDR_HIp*/ cpu_signals.SIG_CPU_ADDR_HIp.sig_out(cpu_abus_new.SYRO_FE00_FFFF());
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

    /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(dma_ctrl.MATU_DMA_RUNNINGp.qp_new());
    /*#p28.ACYL*/ sprite_scanner.ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, sprite_scanner.BESU_SCANNINGn.qp_new());

    // Sprite scanner grabs the sprite index off the _old_ oam address bus
    /*_p28.YFOT*/ wire YFOT_OAM_A2p_old = not1(oam_abus.BUS_OAM_A02n.out_old());
    /*_p28.YFOC*/ wire YFOC_OAM_A3p_old = not1(oam_abus.BUS_OAM_A03n.out_old());
    /*_p28.YVOM*/ wire YVOM_OAM_A4p_old = not1(oam_abus.BUS_OAM_A04n.out_old());
    /*_p28.YMEV*/ wire YMEV_OAM_A5p_old = not1(oam_abus.BUS_OAM_A05n.out_old());
    /*_p28.XEMU*/ wire XEMU_OAM_A6p_old = not1(oam_abus.BUS_OAM_A06n.out_old());
    /*_p28.YZET*/ wire YZET_OAM_A7p_old = not1(oam_abus.BUS_OAM_A07n.out_old());
    /*_p30.XADU*/ sprite_index.XADU_SPRITE_IDX0p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YFOT_OAM_A2p_old);
    /*_p30.XEDY*/ sprite_index.XEDY_SPRITE_IDX1p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YFOC_OAM_A3p_old);
    /*_p30.ZUZE*/ sprite_index.ZUZE_SPRITE_IDX2p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YVOM_OAM_A4p_old);
    /*_p30.XOBE*/ sprite_index.XOBE_SPRITE_IDX3p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YMEV_OAM_A5p_old);
    /*_p30.YDUF*/ sprite_index.YDUF_SPRITE_IDX4p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), XEMU_OAM_A6p_old);
    /*_p30.XECU*/ sprite_index.XECU_SPRITE_IDX5p.dff13(WUDA_xxCDxxGH(), SIG_VCC.out_new(), YZET_OAM_A7p_old);

    // FIXME is this OK without the second assignment of FETO?

    for (int feedback = 0; feedback < 2; feedback++) {
      // 32 + 4 + 2 + 1 = 39
      /*#p28.FETO*/ sprite_scanner.FETO_SCAN_DONEp = and4(scan_counter.YFEL_SCAN0.qp_mid(), scan_counter.WEWY_SCAN1.qp_mid(), scan_counter.GOSO_SCAN2.qp_mid(), scan_counter.FONY_SCAN5.qp_mid());

      /*#p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(sprite_scanner.FETO_SCAN_DONEp.out_new(), XUPY_ABxxEFxx());

      /*#p28.YFEL*/ scan_counter.YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp,                 ANOM_LINE_RSTn, scan_counter.YFEL_SCAN0.qn_any());
      /*_p28.WEWY*/ scan_counter.WEWY_SCAN1.dff17_any(scan_counter.YFEL_SCAN0.qn_any(), ANOM_LINE_RSTn, scan_counter.WEWY_SCAN1.qn_any());
      /*_p28.GOSO*/ scan_counter.GOSO_SCAN2.dff17_any(scan_counter.WEWY_SCAN1.qn_any(), ANOM_LINE_RSTn, scan_counter.GOSO_SCAN2.qn_any());
      /*_p28.ELYN*/ scan_counter.ELYN_SCAN3.dff17_any(scan_counter.GOSO_SCAN2.qn_any(), ANOM_LINE_RSTn, scan_counter.ELYN_SCAN3.qn_any());
      /*_p28.FAHA*/ scan_counter.FAHA_SCAN4.dff17_any(scan_counter.ELYN_SCAN3.qn_any(), ANOM_LINE_RSTn, scan_counter.FAHA_SCAN4.qn_any());
      /*_p28.FONY*/ scan_counter.FONY_SCAN5.dff17_any(scan_counter.FAHA_SCAN4.qn_any(), ANOM_LINE_RSTn, scan_counter.FONY_SCAN5.qn_any());
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
    /*_p27.TEKY*/ wire TEKY_SFETCH_REQp_old = and4(FEPO_STORE_MATCHp.out_old(), TUKU_WIN_HITn_old, tile_fetcher.LYRY_BFETCH_DONEp.out_old(), SOWO_SFETCH_RUNNINGn_old);
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
    /*_p25.CUFE*/ wire CUFE_OAM_CLKp = not_or_and3(cpu_abus_new.SARO_ADDR_OAMp(), dma_ctrl.MATU_DMA_RUNNINGp.qp_new(), MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
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

  SpriteDeltaY sprite_delta_y = sub_sprite_y_gates();

  {
    /*_p29.GACE*/ wire GACE_SPRITE_DELTA4 = not1(sprite_delta_y.GOPU_YDIFF4.sum);
    /*_p29.GUVU*/ wire GUVU_SPRITE_DELTA5 = not1(sprite_delta_y.FUWA_YDIFF5.sum);
    /*_p29.GYDA*/ wire GYDA_SPRITE_DELTA6 = not1(sprite_delta_y.GOJU_YDIFF6.sum);
    /*_p29.GEWY*/ wire GEWY_SPRITE_DELTA7 = not1(sprite_delta_y.WUHU_YDIFF7.sum);

    /*#p29.GOVU*/ wire GOVU_SPSIZE_MATCH = or2(reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new(), sprite_delta_y.GYKY_YDIFF3.sum);
    /*_p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, sprite_delta_y.WUHU_YDIFF7.carry, GOVU_SPSIZE_MATCH);
    /*_p29.GESE*/ wire GESE_SCAN_MATCH_Yp = not1(WOTA_SCAN_MATCH_Yn);
    /*_p29.CEHA*/ wire CEHA_SCANNINGp = not1(sprite_scanner.CENO_SCANNINGn.qn_new());
    /*_p29.CARE*/ wire CARE_COUNT_CLKn = and3(XOCE_xBCxxFGx(), CEHA_SCANNINGp, GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /*_p29.DYTY*/ wire DYTY_COUNT_CLKp = not1(CARE_COUNT_CLKn);
    /*_p29.DEZY*/ DEZY_COUNT_CLKp.dff17_any(ZEME_AxCxExGx(), XAPO_VID_RSTn(), DYTY_COUNT_CLKp);

    // There's a feedback loop here, but we don't actually need to loop - BAKY holds the clock line high once the sprite store is full, so doing a second logic pass
    // doesn't actually change any of the dffs.
    /*#p29.BAKY*/ wire BAKY_SPRITES_FULL_new = and2(sprite_counter.CUXY_SPRITE_COUNT1.qp_any(), sprite_counter.DYBE_SPRITE_COUNT3.qp_any());
    /*#p29.CAKE*/ wire CAKE_COUNT_CLKp_new = or2(BAKY_SPRITES_FULL_new, DEZY_COUNT_CLKp.qp_any());
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
    store_sprite_gates(sprite_store_flags_old, sprite_store_flags, sprite_reset_flags, BYVA_LINE_RSTn, oam_temp_b_old);
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

    /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(FEPO_STORE_MATCHp.out_mid(), WODU_HBLANKp.out_old(), MYVO_AxCxExGx());
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

    get_sprite_match_flags_gates(AROR_MATCH_ENp, sprite_match_flags);
  }

  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  {
    /*#p21.XENA*/ wire XENA_STORE_MATCHn = not1(FEPO_STORE_MATCHp.out_new());
    /*#p21.XUGU*/ wire XUGU_PX167n = nand5(pix_count.XEHO_PX0p.qp_new(), pix_count.SAVY_PX1p.qp_new(), pix_count.XODU_PX2p.qp_new(), pix_count.TUKY_PX5p.qp_new(), pix_count.SYBE_PX7p.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire XANO_PX167p = not1(XUGU_PX167n);
    /*#p21.WODU*/ WODU_HBLANKp = and2(XENA_STORE_MATCHn, XANO_PX167p); // WODU goes high on odd, cleared on H
  }

  /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(FEPO_STORE_MATCHp.out_new(), WODU_HBLANKp.out_new(), MYVO_AxCxExGx());
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);
  /*#p24.SACU*/ wire SACU_CLKPIPE_new = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  /*_p24.PAHO*/ lcd.PAHO_X_8_SYNC.dff17(ROXO_CLKPIPE_odd, XYMU_RENDERINGn.qn_new(), XYDO_PX3p_old.qp_old());

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  memset(&sprite_ibus, BIT_NEW | BIT_PULLED | 1, sizeof(sprite_ibus));
  memset(&sprite_lbus, BIT_NEW | BIT_PULLED | 1, sizeof(sprite_lbus));

  sprite_match_to_bus_gates(sprite_match_flags);
  sprite_scan_to_bus_gates(sprite_delta_y, XYMU_RENDERINGn, FEPO_STORE_MATCHp);

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

  tock_spu_gates();

  //----------------------------------------
  // Memory buses

  tock_ext_gates(cart_blob);
  tock_vram_bus_gates(TEVO_WIN_FETCH_TRIGp);
  tock_oam_bus_gates();
  tock_zram_gates();

  //----------------------------------------
  // And finally, interrupts.

  tock_interrupts_gates();

  cpu_abus_old = cpu_abus_new;
  cpu_dbus_old = cpu_dbus_new;
  commit();
}





















































































































//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_logic(const blob& cart_blob) {
  wire SYLO_WIN_HITn_old = !win_reg.RYDY_WIN_HITp;
  wire TOMU_WIN_HITp_old = !SYLO_WIN_HITn_old;
  wire SOCY_WIN_HITn_old = !TOMU_WIN_HITp_old;
  wire VYBO_CLKPIPE_old = !FEPO_STORE_MATCHp && !WODU_HBLANKp &&  !gen_clk_old(0b10101010);
  wire TYFA_CLKPIPE_old = SOCY_WIN_HITn_old && tile_fetcher.POKY_PRELOAD_LATCHp && VYBO_CLKPIPE_old;
  wire SEGU_CLKPIPE_old = !TYFA_CLKPIPE_old;
  wire SACU_CLKPIPE_old = SEGU_CLKPIPE_old || fine_scroll.ROXY_FINE_SCROLL_DONEn;
  wire ROCO_CLKPIPE_old = !SEGU_CLKPIPE_old;

  wire clkpipe_en_old = 1;
  if (win_reg.RYDY_WIN_HITp) clkpipe_en_old = 0;
  if (!tile_fetcher.POKY_PRELOAD_LATCHp) clkpipe_en_old = 0;
  if (FEPO_STORE_MATCHp) clkpipe_en_old = 0;
  if (WODU_HBLANKp) clkpipe_en_old = 0;

  wire CLKPIPE_old = gen_clk_old(0b10101010);
  if (!clkpipe_en_old) CLKPIPE_old = 1;
  if (fine_scroll.ROXY_FINE_SCROLL_DONEn) CLKPIPE_old = 1;

  wire TEVO_WIN_FETCH_TRIGp_old = 0;
  if (win_reg.RYFA_WIN_FETCHn_A && !win_reg.RENE_WIN_FETCHn_B) TEVO_WIN_FETCH_TRIGp_old = 1;
  if (!win_reg.RYDY_WIN_HITp && win_reg.SOVY_WIN_HITp) TEVO_WIN_FETCH_TRIGp_old = 1;
  if (!XYMU_RENDERINGn && !tile_fetcher.POKY_PRELOAD_LATCHp && tile_fetcher.NYKA_FETCH_DONEp && tile_fetcher.PORY_FETCH_DONEp) TEVO_WIN_FETCH_TRIGp_old = 1;

  auto wodu_hblank_old = WODU_HBLANKp.state;

  bool int_stat_old = 0;
  if (!reg_stat.RUGU_STAT_LYI_ENn.state && ROPO_LY_MATCH_SYNCp.state) int_stat_old = 1;
  if (!reg_stat.REFE_STAT_OAI_ENn.state && !lcd.POPU_y144p.state && lcd.RUTU_x113p.state) int_stat_old = 1;
  if (!reg_stat.RUFO_STAT_VBI_ENn.state && lcd.POPU_y144p.state) int_stat_old = 1;
  if (!reg_stat.ROXE_STAT_HBI_ENn.state && wodu_hblank_old && !lcd.POPU_y144p.state) int_stat_old = 1;

  wire int_lcd_old = lcd.POPU_y144p.state;
  wire int_joy_old = !joy_int.APUG_JP_GLITCH3.state || !joy_int.BATU_JP_GLITCH0.state;
  wire int_tim_old = int_ctrl.MOBA_TIMER_OVERFLOWp.state;
  //wire int_ser_old = serial.CALY_SER_CNT3.state;
  wire int_ser_old = 0;


  wire UKAP_CLK_MUXa_old = tac.SOPU_TAC0p.state ? div.TAMA_DIV05p.state : div.TERO_DIV03p.state;
  wire TEKO_CLK_MUXb_old = tac.SOPU_TAC0p.state ? div.UFOR_DIV01p.state : div.TULU_DIV07p.state;
  wire TECY_CLK_MUXc_old = tac.SAMY_TAC1p.state ? UKAP_CLK_MUXa_old : TEKO_CLK_MUXb_old;
  wire SOGU_TIMA_CLKn_old = TECY_CLK_MUXc_old && tac.SABO_TAC2p.state;



  //----------

  SIG_VCC.state = 1;
  SIG_GND.state = 0;

  //-----------------------------------------------------------------------------

  auto cpu_addr_old = bit_pack(cpu_abus_old);
  //auto cpu_addr_vram_old = (cpu_addr_old >= 0x8000) && (cpu_addr_old <= 0x9FFF);
  //auto cpu_addr_oam_old = (cpu_addr_old >= 0xFE00) && (cpu_addr_old <= 0xFEFF);
  //auto cpu_data_old = (uint8_t)pack(8, (BitBase*)&cpu_dbus_old.BUS_CPU_D00p);
  //auto cpu_rd_old = cpu_signals.SIG_IN_CPU_RDp;
  //auto cpu_wr_old = cpu_signals.SIG_IN_CPU_WRp;

  {
    bit_set(cpu_abus_new);
    bit_set(cpu_dbus_new);

    bool EXT_cpu_latch_ext;

    if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      // Data has to be driven on EFGH or we fail the wave tests
      if (bus_req_new.write) bit_unpack(cpu_dbus_new, bus_req_new.data_lo);
      EXT_cpu_latch_ext = bus_req_new.read;
    }
    else {
      EXT_cpu_latch_ext = 0;
    }

    cpu_signals.SIG_IN_CPU_LATCH_EXT.state = EXT_cpu_latch_ext;

    bool EXT_addr_new = (bus_req_new.read || bus_req_new.write);
    bool in_bootrom = !cpu_signals.TEPU_BOOT_BITn.state;
    bool addr_boot = (bus_req_new.addr <= 0x00FF) && in_bootrom;
    bool addr_vram = (bus_req_new.addr >= 0x8000) && (bus_req_new.addr < 0x9FFF);
    bool addr_high = (bus_req_new.addr >= 0xFE00);

    bool EXT_cpu_rd;
    bool EXT_cpu_wr;

    if (DELTA_HA) {
      EXT_cpu_rd = 0;
      EXT_cpu_wr = 0;
      bit_unpack(cpu_abus_new, bus_req_new.addr & 0x00FF);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
      if (addr_vram) EXT_addr_new = false;
    }
    else {
      EXT_cpu_rd = bus_req_new.read;
      EXT_cpu_wr = bus_req_new.write;
      bit_unpack(cpu_abus_new, bus_req_new.addr);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
    }

    cpu_signals.SIG_IN_CPU_RDp.state = EXT_cpu_rd;
    cpu_signals.SIG_IN_CPU_WRp.state = EXT_cpu_wr;
    cpu_signals.SIG_IN_CPU_EXT_BUSp.state = EXT_addr_new;
  }

  auto cpu_addr_new = bit_pack(cpu_abus_new);

  bool cpu_addr_vram_new = (cpu_addr_new >= 0x8000) && (cpu_addr_new <= 0x9FFF);
  bool cpu_addr_ram_new = (cpu_addr_new >= 0xA000) && (cpu_addr_new <= 0xFDFF);
  //auto cpu_addr_oam_new = (cpu_addr_new >= 0xFE00) && (cpu_addr_new <= 0xFEFF);
  //auto cpu_data_new = (uint8_t)pack(8, (BitBase*)&cpu_dbus_new.BUS_CPU_D00p);
  bool cpu_rd_new = cpu_signals.SIG_IN_CPU_RDp;
  bool cpu_wr_new = cpu_signals.SIG_IN_CPU_WRp;

  //-----------------------------------------------------------------------------

  clk.PIN_74_CLK.CLK.state = phase_total & 1;
  clk.PIN_74_CLK.CLKGOOD.state = 1;

  rst.PIN_71_RST = 0;
  rst.PIN_76_T2 = 0;
  rst.PIN_77_T1 = 0;

  cpu_ack.SIG_CPU_ACK_VBLANK = get_bit(gb_cpu.int_ack, BIT_VBLANK);
  cpu_ack.SIG_CPU_ACK_STAT   = get_bit(gb_cpu.int_ack, BIT_STAT);
  cpu_ack.SIG_CPU_ACK_TIMER  = get_bit(gb_cpu.int_ack, BIT_TIMER);
  cpu_ack.SIG_CPU_ACK_SERIAL = get_bit(gb_cpu.int_ack, BIT_SERIAL);
  cpu_ack.SIG_CPU_ACK_JOYPAD = get_bit(gb_cpu.int_ack, BIT_JOYPAD);

  clk.SIG_CPU_CLKREQ.state = 1;

  cpu_signals.SIG_CPU_ADDR_HIp.state = cpu_addr_new >= 0xFE00 && cpu_addr_new <= 0xFFFF;
  cpu_signals.SIG_CPU_UNOR_DBG.state = 0;
  cpu_signals.SIG_CPU_UMUT_DBG.state = 0;

  //-----------------------------------------------------------------------------
  // Sys clock signals

  {
    wire c = clk.PIN_74_CLK.CLK;
    clk.PIN_73_CLK_DRIVE.state = c;
    clk.AVET_DEGLITCH = c;
    clk.ANOS_DEGLITCH = !c;
  }

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
  wire CLK_xxxxEFGx = gen_clk_new(0b00001110);
  wire CLK_xxCDEFGH = gen_clk_new(0b00111111);
  wire CLK_AxCxExGx = gen_clk_new(0b10101010);
  wire CLK_AxxxExxx = gen_clk_new(0b10001000);
  wire CLK_xBxDxFxG = gen_clk_new(0b01010101);

  clk.AFUR_xxxxEFGH = CLK_xxxxEFGH;
  clk.ALEF_AxxxxFGH = CLK_AxxxxFGH;
  clk.APUK_ABxxxxGH = CLK_ABxxxxGH;
  clk.ADYK_ABCxxxxH = CLK_ABCxxxxH;

  clk.PIN_75_CLK_OUT.state = CLK_xxxxEFGH;

  clk.SIG_CPU_BOWA_Axxxxxxx.state = CLK_Axxxxxxx;
  clk.SIG_CPU_BEDO_xBCDEFGH.state = CLK_xBCDEFGH;
  clk.SIG_CPU_BEKO_ABCDxxxx.state = CLK_ABCDxxxx;
  clk.SIG_CPU_BUDE_xxxxEFGH.state = CLK_xxxxEFGH;
  clk.SIG_CPU_BOLO_ABCDEFxx.state = CLK_ABCDEFxx;
  clk.SIG_CPU_BUKE_AxxxxxGH.state = CLK_AxxxxxGH;
  clk.SIG_CPU_BOMA_xBCDEFGH.state = CLK_xBCDEFGH;
  clk.SIG_CPU_BOGA_Axxxxxxx.state = CLK_Axxxxxxx;

  cpu_signals.TEDO_CPU_RDp = cpu_rd_new;
  cpu_signals.APOV_CPU_WRp = CLK_xxxxEFGx && cpu_wr_new;
  cpu_signals.TAPU_CPU_WRp = cpu_signals.APOV_CPU_WRp;
  cpu_signals.ABUZ_EXT_RAM_CS_CLK = CLK_xxCDEFGH && cpu_signals.SIG_IN_CPU_EXT_BUSp;

  //----------
  // DIV

  if (DELTA_HA) bit_unpack(div, bit_pack(div) + 1);
  if (cpu_addr_new == 0xFF04 && cpu_wr_new && CLK_xxxxEFGx) bit_unpack(div, 0);
  if (cpu_addr_new == 0xFF04 && cpu_rd_new) bit_unpack(cpu_dbus_new, bit_pack(div) >> 6);

  //----------
  // In logic mode we don't care about the power-on behavior, we only want behavior to match when running code. So, we set
  // this stuff to zeroes.

  rst.AFER_SYS_RSTp = 0;
  rst.TUBO_WAITINGp = 0;
  rst.ASOL_POR_DONEn = 0;
  rst.SIG_CPU_EXT_CLKGOOD = 1;
  rst.SIG_CPU_EXT_RESETp = 0;
  rst.SIG_CPU_STARTp = 0;
  rst.SIG_CPU_INT_RESETp = 0;;
  rst.SOTO_DBG_VRAMp = 0;

  //----------
  // LCDC
  // has to be near the top as it controls the video reset signal

  //wire vid_rst_old = reg_lcdc.XONA_LCDC_LCDENn;
  //wire winen_old = !reg_lcdc.WYMO_LCDC_WINENn;

  {
    if (cpu_wr_new && cpu_addr_new == 0xFF40 && DELTA_GH) {
      bit_copy_inv(reg_lcdc, cpu_dbus_old);
    }

    if (cpu_rd_new && (cpu_addr_new == 0xFF40)) {
      bit_copy_inv(cpu_dbus_new, reg_lcdc);
    }
  }
  bool vid_rst_new = reg_lcdc.XONA_LCDC_LCDENn;
  bool winen_new = !reg_lcdc.WYMO_LCDC_WINENn;

  //----------
  // Video clocks

  clk.WOSU_AxxDExxH = !vid_rst_new && gen_clk_new(0b10011001);
  clk.WUVU_ABxxEFxx = !vid_rst_new && gen_clk_new(0b11001100);
  clk.VENA_xxCDEFxx = !vid_rst_new && gen_clk_new(0b00111100);

  //----------
  // LYC

  {
    if (cpu_addr_new == 0xFF45) {
      if (cpu_rd_new) bit_copy_inv(cpu_dbus_new, reg_lyc);
      if (cpu_wr_new && DELTA_GH) bit_copy_inv(reg_lyc, cpu_dbus_old);
    }

    if (!vid_rst_new && DELTA_BC) {
      auto ly = bit_pack(reg_ly);
      auto lyc = bit_pack_inv(reg_lyc);
      ROPO_LY_MATCH_SYNCp = ly == lyc;
    }

    // FIXME this seems slightly wrong...
    if (cpu_wr_new && CLK_xxxxEFGx && cpu_addr_new == 0xFF41) {
    }
    else {
      RUPO_LYC_MATCHn = 1;
    }

    // but the "reset" arm of the latch overrides the "set" arm, so it doesn't completely break?
    if (ROPO_LY_MATCH_SYNCp) {
      RUPO_LYC_MATCHn = 0;
    }
  }

  //----------
  /// LX, LY, lcd flags

  bool rutu_x113p_old = lcd.RUTU_x113p;
  bool popu_y144p_old = lcd.POPU_y144p;
  bool nype_x113p_old = lcd.NYPE_x113p;

  auto lx_old = bit_pack(reg_lx);
  auto ly_old = bit_pack(reg_ly);
  
  bool line_rst_old = ATEJ_LINE_RSTp;

  if (vid_rst_new) {
    lcd.ANEL_x113p = 0;
    lcd.CATU_x113p = 0;
    lcd.NYPE_x113p = 0;
    lcd.RUTU_x113p = 0;
    lcd.POPU_y144p = 0;
    lcd.MYTA_y153p = 0;
    lcd.SYGU_LINE_STROBE = 0;

    ATEJ_LINE_RSTp = 1;

    bit_clear(reg_lx);
    bit_clear(reg_ly);
  }
  else {
    wire ly_144 = (ly_old & 144) == 144;
    wire ly_153 = (ly_old & 153) == 153;

    if (DELTA_HA) {
      lcd.CATU_x113p = lcd.RUTU_x113p.state && !ly_144;
    }

    if (DELTA_BC) {
      bool nype_old = lcd.NYPE_x113p;

      lcd.ANEL_x113p = lcd.CATU_x113p;
      lcd.NYPE_x113p = lcd.RUTU_x113p;

      bool nype_new = lcd.NYPE_x113p;
      if (!nype_old && nype_new) {
        lcd.POPU_y144p = ly_144;
        lcd.MYTA_y153p = ly_153;
      }

      bit_unpack(reg_lx, lx_old + 1);
    }

    if (DELTA_DE) {
      lcd.CATU_x113p = lcd.RUTU_x113p.state && !ly_144;
    }

    if (DELTA_FG) {
      bool rutu_old = lcd.RUTU_x113p;

      lcd.ANEL_x113p = lcd.CATU_x113p;
      lcd.RUTU_x113p = lx_old == 113;

      bool rutu_new = lcd.RUTU_x113p;
      if (!rutu_old && rutu_new) {
        bit_unpack(reg_ly, ly_old + 1);
      }

      wire strobe = (lx_old == 0) || (lx_old == 7) || (lx_old == 45) || (lx_old == 83);
      lcd.SYGU_LINE_STROBE = strobe;
    }

    ATEJ_LINE_RSTp = !lcd.ANEL_x113p && lcd.CATU_x113p;
    if (lcd.RUTU_x113p) bit_clear(reg_lx);
    if (lcd.MYTA_y153p) bit_clear(reg_ly);
  }

  if (cpu_rd_new && (cpu_addr_new == 0xFF44)) {
    bit_copy(cpu_dbus_new, reg_ly);
  }

  auto lx_new = bit_pack(reg_lx);
  auto ly_new = bit_pack(reg_ly);

  bool rutu_x113p_new = lcd.RUTU_x113p;
  bool popu_y144p_new = lcd.POPU_y144p;
  bool nype_x113p_new = lcd.NYPE_x113p;

  bool line_rst_new = ATEJ_LINE_RSTp;

  //----------

  {
    if (cpu_signals.SIG_IN_CPU_WRp.state && cpu_addr_new == 0xFF00 && DELTA_GH) {
      joy_reg.KELY_JOYP_UDLRp.state = cpu_dbus_old.BUS_CPU_D04p.state;
      joy_reg.COFY_JOYP_ABCSp.state = cpu_dbus_old.BUS_CPU_D05p.state;
      joy_ext.PIN_63_JOY_P14.state = !joy_reg.KELY_JOYP_UDLRp.state;
      joy_ext.PIN_62_JOY_P15.state = !joy_reg.COFY_JOYP_ABCSp.state;
    }

    bool EXT_button0 = 0, EXT_button1 = 0, EXT_button2 = 0, EXT_button3 = 0;

    if (joy_ext.PIN_63_JOY_P14.state) {
      EXT_button0 = get_bit(sys_buttons, 0); // RIGHT
      EXT_button1 = get_bit(sys_buttons, 1); // LEFT
      EXT_button2 = get_bit(sys_buttons, 2); // UP
      EXT_button3 = get_bit(sys_buttons, 3); // DOWN
    }
    else if (joy_ext.PIN_62_JOY_P15.state) {
      EXT_button0 = get_bit(sys_buttons, 4); // A
      EXT_button1 = get_bit(sys_buttons, 5); // B
      EXT_button2 = get_bit(sys_buttons, 6); // SELECT
      EXT_button3 = get_bit(sys_buttons, 7); // START
    }

    joy_ext.PIN_67_JOY_P10.state = EXT_button0;
    joy_ext.PIN_66_JOY_P11.state = EXT_button1;
    joy_ext.PIN_65_JOY_P12.state = EXT_button2;
    joy_ext.PIN_64_JOY_P13.state = EXT_button3;

    wire any_button = EXT_button0 || EXT_button1 || EXT_button2 || EXT_button3;

    if (gen_clk_new(0b10000000)) {
      int_ctrl.AWOB_WAKE_CPU.state = !any_button;
      int_ctrl.SIG_CPU_WAKE.state = !any_button;
    }

    if (DELTA_HA) {
      joy_int.APUG_JP_GLITCH3.state = joy_int.AGEM_JP_GLITCH2.state;
      joy_int.AGEM_JP_GLITCH2.state = joy_int.ACEF_JP_GLITCH1.state;
      joy_int.ACEF_JP_GLITCH1.state = joy_int.BATU_JP_GLITCH0.state;
      joy_int.BATU_JP_GLITCH0.state = !any_button;
    }

    if (cpu_signals.SIG_IN_CPU_RDp.state && (cpu_addr_new == 0xFF00)) {
      cpu_dbus_new.BUS_CPU_D00p.state = !joy_latch.KEVU_JOYP_L0n.state;
      cpu_dbus_new.BUS_CPU_D01p.state = !joy_latch.KAPA_JOYP_L1n.state;
      cpu_dbus_new.BUS_CPU_D02p.state = !joy_latch.KEJA_JOYP_L2n.state;
      cpu_dbus_new.BUS_CPU_D03p.state = !joy_latch.KOLO_JOYP_L3n.state;
      cpu_dbus_new.BUS_CPU_D04p.state =  joy_reg.KELY_JOYP_UDLRp.state;
      cpu_dbus_new.BUS_CPU_D05p.state =  joy_reg.COFY_JOYP_ABCSp.state;
    }
    else {
      joy_latch.KEVU_JOYP_L0n.state = joy_ext.PIN_67_JOY_P10.state;
      joy_latch.KAPA_JOYP_L1n.state = joy_ext.PIN_66_JOY_P11.state;
      joy_latch.KEJA_JOYP_L2n.state = joy_ext.PIN_65_JOY_P12.state;
      joy_latch.KOLO_JOYP_L3n.state = joy_ext.PIN_64_JOY_P13.state;
    }
  }

  //tock_serial_logic();
  //tock_timer_logic();

  {
    auto new_addr = bit_pack(cpu_abus_new);

    if (cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
      if (new_addr == 0xFF06) bit_copy(tma, cpu_dbus_new);
      if (new_addr == 0xFF07) bit_copy(&tac, 3, &cpu_dbus_new);
    }

    
    if (posedge(gen_clk_old(0b10000000), gen_clk_new(0b10000000))) {
      wire MERY_TIMER_OVERFLOWp_old = !tima.NUGA_TIMA7p.state && int_ctrl.NYDU_TIMA7p_DELAY.state;
      int_ctrl.MOBA_TIMER_OVERFLOWp.state = MERY_TIMER_OVERFLOWp_old;
    }

    wire TOPE_FF05_WRn = !(CLK_xxxxEFGx && cpu_signals.SIG_IN_CPU_WRp.state && new_addr == 0xFF05);

    wire MUZU_CPU_LOAD_TIMAn = cpu_signals.SIG_IN_CPU_LATCH_EXT.state || TOPE_FF05_WRn;
    wire MEXU_TIMA_LOADp = !MUZU_CPU_LOAD_TIMAn || int_ctrl.MOBA_TIMER_OVERFLOWp.state;

    if (posedge(gen_clk_old(0b10000000), gen_clk_new(0b10000000))) {
      int_ctrl.NYDU_TIMA7p_DELAY.state = tima.NUGA_TIMA7p.state;
    }

    if (MEXU_TIMA_LOADp) {
      int_ctrl.NYDU_TIMA7p_DELAY.state = 0;
    }


    // FIXME gonna need old and new div for this

    wire UKAP_CLK_MUXa_new = tac.SOPU_TAC0p.state ? div.TAMA_DIV05p.state : div.TERO_DIV03p.state;
    wire TEKO_CLK_MUXb_new = tac.SOPU_TAC0p.state ? div.UFOR_DIV01p.state : div.TULU_DIV07p.state;
    wire TECY_CLK_MUXc_new = tac.SAMY_TAC1p.state ? UKAP_CLK_MUXa_new : TEKO_CLK_MUXb_new;
    wire SOGU_TIMA_CLKn_new = TECY_CLK_MUXc_new && tac.SABO_TAC2p.state;

    wire ROKE_TIMA_D0 = TOPE_FF05_WRn ? tma.SABU_TMA0p.state : cpu_dbus_new.BUS_CPU_D00p.state;
    wire PETU_TIMA_D1 = TOPE_FF05_WRn ? tma.NYKE_TMA1p.state : cpu_dbus_new.BUS_CPU_D01p.state;
    wire NYKU_TIMA_D2 = TOPE_FF05_WRn ? tma.MURU_TMA2p.state : cpu_dbus_new.BUS_CPU_D02p.state;
    wire SOCE_TIMA_D3 = TOPE_FF05_WRn ? tma.TYVA_TMA3p.state : cpu_dbus_new.BUS_CPU_D03p.state;
    wire SALA_TIMA_D4 = TOPE_FF05_WRn ? tma.TYRU_TMA4p.state : cpu_dbus_new.BUS_CPU_D04p.state;
    wire SYRU_TIMA_D5 = TOPE_FF05_WRn ? tma.SUFY_TMA5p.state : cpu_dbus_new.BUS_CPU_D05p.state;
    wire REFU_TIMA_D6 = TOPE_FF05_WRn ? tma.PETO_TMA6p.state : cpu_dbus_new.BUS_CPU_D06p.state;
    wire RATO_TIMA_D7 = TOPE_FF05_WRn ? tma.SETA_TMA7p.state : cpu_dbus_new.BUS_CPU_D07p.state;

    if (negedge(SOGU_TIMA_CLKn_old, SOGU_TIMA_CLKn_new)) {
      bit_unpack(tima, bit_pack(tima) + 1);
    }

    if (MEXU_TIMA_LOADp) {
      tima.REGA_TIMA0p.set_data(ROKE_TIMA_D0);
      tima.POVY_TIMA1p.set_data(PETU_TIMA_D1);
      tima.PERU_TIMA2p.set_data(NYKU_TIMA_D2);
      tima.RATE_TIMA3p.set_data(SOCE_TIMA_D3);
      tima.RUBY_TIMA4p.set_data(SALA_TIMA_D4);
      tima.RAGE_TIMA5p.set_data(SYRU_TIMA_D5);
      tima.PEDA_TIMA6p.set_data(REFU_TIMA_D6);
      tima.NUGA_TIMA7p.set_data(RATO_TIMA_D7);
    }

    if (cpu_signals.SIG_IN_CPU_RDp.state) {
      if (new_addr == 0xFF05) bit_copy(cpu_dbus_new, tima);
      if (new_addr == 0xFF06) bit_copy(cpu_dbus_new, tma);
      if (new_addr == 0xFF07) bit_copy(cpu_dbus_new, tac);
    }
  }

  {
    if (cpu_signals.SIG_IN_CPU_WRp.state && cpu_addr_new == 0xFF50 && DELTA_GH) {
      cpu_signals.TEPU_BOOT_BITn.state = SATO_BOOT_BITn.state;
    }

    cpu_signals.SIG_CPU_BOOTp.state = 0;
    cpu_signals.SIG_BOOT_CSp.state = 0;

    if (cpu_addr_new <= 0x00FF) {

      cpu_signals.SIG_CPU_BOOTp.state = !cpu_signals.TEPU_BOOT_BITn.state;

      if (cpu_signals.SIG_IN_CPU_RDp.state && !cpu_signals.TEPU_BOOT_BITn.state) {
        cpu_signals.SIG_BOOT_CSp.state = 1;
        bit_unpack(cpu_dbus_new, DMG_ROM_blob[cpu_addr_new & 0xFF]);
      }
    }

    if (cpu_signals.SIG_IN_CPU_RDp.state && (cpu_addr_new == 0xFF50)) {
      cpu_dbus_new.BUS_CPU_D00p.state = cpu_signals.TEPU_BOOT_BITn.state;
    }

    SATO_BOOT_BITn = cpu_dbus_new.BUS_CPU_D00p.state || cpu_signals.TEPU_BOOT_BITn.state;
  }

  auto dma_addr_old = (bit_pack_inv(dma_hi) << 8) | bit_pack(dma_lo);
  bool dma_running_old = dma_ctrl.MATU_DMA_RUNNINGp;
  {
    wire FF46_RDp = cpu_signals.SIG_IN_CPU_RDp.state && (cpu_addr_new == 0xFF46);
    wire FF46_WRp = cpu_signals.SIG_IN_CPU_WRp.state && (cpu_addr_new == 0xFF46);

    dma_ctrl.LYXE_DMA_LATCHp.state |= (FF46_WRp & CLK_xxxxEFGx);

    auto dma_lo_old = bit_pack(dma_lo);

    if (DELTA_DE) {
      if (dma_lo_old == 159) {
        dma_ctrl.MYTE_DMA_DONE.state = 1;
        dma_ctrl.LARA_DMA_LATCHn = 1;
        dma_ctrl.LOKY_DMA_LATCHp = 0;
      }

      dma_ctrl.LENE_DMA_TRIG_d4.state = dma_ctrl.LUVY_DMA_TRIG_d0.state;

      if (bit(dma_ctrl.LUVY_DMA_TRIG_d0.state)) {
        dma_ctrl.MYTE_DMA_DONE.state = 0;
        dma_ctrl.LYXE_DMA_LATCHp.state = 0;
        bit_clear(dma_lo);
        dma_ctrl.LARA_DMA_LATCHn = 0;
        dma_ctrl.LOKY_DMA_LATCHp = 1;
      }
    }

    if (FF46_RDp) {
      bit_unpack(cpu_dbus_new, bit_pack_inv(dma_hi));
    }

    if (FF46_WRp && DELTA_GH) {
      auto old_data = bit_pack(cpu_dbus_old);
      bit_unpack_inv(dma_hi, old_data);
    }

    if (DELTA_HA) {
      dma_ctrl.LUVY_DMA_TRIG_d0.state = ~FF46_WRp & dma_ctrl.LYXE_DMA_LATCHp.state;
      dma_ctrl.MATU_DMA_RUNNINGp.state = dma_ctrl.LOKY_DMA_LATCHp.state;

      if (dma_ctrl.LOKY_DMA_LATCHp.state && !dma_ctrl.LENE_DMA_TRIG_d4.state) {
        bit_unpack(dma_lo, dma_lo_old + 1);
      }
    }

  }
  bool dma_running_new = dma_ctrl.MATU_DMA_RUNNINGp;
  auto dma_addr_new = (bit_pack_inv(dma_hi) << 8) | bit_pack(dma_lo);

  auto dma_addr_vram_new = dma_running_new && (dma_addr_new >= 0x8000) && (dma_addr_new <= 0x9FFF);

  //----------------------------------------
  // Sprite scanner

  bool scanning_old = sprite_scanner.ACYL_SCANNINGp;

  {
    auto scan_count_old = bit_pack(scan_counter);
    bool BESU_old = sprite_scanner.BESU_SCANNINGn;
    bool BYBU_old = sprite_scanner.BYBA_SCAN_DONE_Ap;
    bool FETO_old = bit_pack(scan_counter) == 39;

    //----------

    if (vid_rst_new || line_rst_new) {
      sprite_scanner.DOBA_SCAN_DONE_Bp = 0;
      sprite_scanner.BYBA_SCAN_DONE_Ap = 0;
      sprite_scanner.AVAP_SCAN_DONE_TRIGp = 0;
    }
    else {
      if (DELTA_EVEN) sprite_scanner.DOBA_SCAN_DONE_Bp = BYBU_old;
      if (DELTA_HA || DELTA_DE) sprite_scanner.BYBA_SCAN_DONE_Ap = FETO_old;
      sprite_scanner.AVAP_SCAN_DONE_TRIGp = !sprite_scanner.DOBA_SCAN_DONE_Bp && sprite_scanner.BYBA_SCAN_DONE_Ap;
    }

    //----------

    if (vid_rst_new) {
      sprite_scanner.BESU_SCANNINGn = 0;
    }
    else if (line_rst_new) {
      sprite_scanner.BESU_SCANNINGn = 1;
    }
    else {
      if (lcd.CATU_x113p) sprite_scanner.BESU_SCANNINGn = 1;
      if (sprite_scanner.AVAP_SCAN_DONE_TRIGp) sprite_scanner.BESU_SCANNINGn = 0;
    }

    sprite_scanner.ACYL_SCANNINGp = !vid_rst_new && !dma_running_new && sprite_scanner.BESU_SCANNINGn;

    //----------

    if (!vid_rst_new && (DELTA_HA || DELTA_DE)) {
      bit_copy_inv(&sprite_index, 6, &oam_abus.BUS_OAM_A02n);
      sprite_scanner.CENO_SCANNINGn = BESU_old;
    }

    if (vid_rst_new) {
      sprite_scanner.CENO_SCANNINGn = 0;
    }

    //----------

    if (vid_rst_new || line_rst_new) {
      bit_clear(scan_counter);
    }
    else if ((DELTA_HA || DELTA_DE) && (scan_count_old != 39)) {
      bit_unpack(scan_counter, scan_count_old + 1);
    }

    // this is unused now
    sprite_scanner.FETO_SCAN_DONEp = bit_pack(scan_counter) == 39;
  }

  bool scanning_new = sprite_scanner.ACYL_SCANNINGp.state;

  //----------------------------------------
  // Global rendering flag 'XYMU'

  if (DELTA_EVEN) {
    VOGA_HBLANKp.state = wodu_hblank_old;
  }

  if (line_rst_new) {
    VOGA_HBLANKp.state = 0;
  }

  bool rendering_old = !XYMU_RENDERINGn.state;

  if (vid_rst_new) XYMU_RENDERINGn.state = 1;
  if (VOGA_HBLANKp.state) XYMU_RENDERINGn.state = 1;
  if (sprite_scanner.AVAP_SCAN_DONE_TRIGp.state) XYMU_RENDERINGn.state = 0;

  bool rendering_new = !XYMU_RENDERINGn.state;

  //----------------------------------------
  // Sprite fetch sequencer

  uint8_t sfetch_phase_old = pack(!(sprite_fetcher.TYFO_SFETCH_S0p_D1.state ^ sprite_fetcher.TOXE_SFETCH_S0p.state), sprite_fetcher.TOXE_SFETCH_S0p.state, sprite_fetcher.TULY_SFETCH_S1p.state, sprite_fetcher.TESE_SFETCH_S2p.state);

  if (DELTA_EVEN) {
    sprite_fetcher.SOBU_SFETCH_REQp.  state = FEPO_STORE_MATCHp.state && !TOMU_WIN_HITp_old && tile_fetcher.LYRY_BFETCH_DONEp.state && !sprite_fetcher.TAKA_SFETCH_RUNNINGp.state;
    sprite_fetcher.VONU_SFETCH_S1p_D4.state = sprite_fetcher.TOBU_SFETCH_S1p_D2;
    sprite_fetcher.TOBU_SFETCH_S1p_D2.state = sprite_fetcher.TULY_SFETCH_S1p;

    if ((sfetch_phase_old >> 1) != 5) {
      bit_unpack(&sprite_fetcher.TOXE_SFETCH_S0p, 3, (sfetch_phase_old >> 1) + 1);
    }

    if (bit(and2(sprite_fetcher.SOBU_SFETCH_REQp.state, ~sprite_fetcher.SUDA_SFETCH_REQp.state))) {
      bit_clear(&sprite_fetcher.TOXE_SFETCH_S0p, 3);
    }
  }
  else {
    sprite_fetcher.SUDA_SFETCH_REQp  .state = sprite_fetcher.SOBU_SFETCH_REQp.state;
    sprite_fetcher.TYFO_SFETCH_S0p_D1.state = sprite_fetcher.TOXE_SFETCH_S0p.state;
    sprite_fetcher.SEBA_SFETCH_S1p_D5.state = sprite_fetcher.VONU_SFETCH_S1p_D4.state;
  }


  if (vid_rst_new || line_rst_new) {
    bit_clear(&sprite_fetcher.TOXE_SFETCH_S0p, 3);
  }

  if (!rendering_new) {
    sprite_fetcher.TOBU_SFETCH_S1p_D2.state = 0;
    sprite_fetcher.VONU_SFETCH_S1p_D4.state = 0;
    sprite_fetcher.SEBA_SFETCH_S1p_D5.state = 0;
  }

  sprite_fetcher.TEXY_SFETCHINGp = (sprite_fetcher.TULY_SFETCH_S1p || sprite_fetcher.VONU_SFETCH_S1p_D4) && rendering_new;

  wire WUTY_SFETCH_DONE_TRIGp_old = sprite_fetcher.WUTY_SFETCH_DONE_TRIGp;

  sprite_fetcher.WUTY_SFETCH_DONE_TRIGp = sprite_fetcher.TYFO_SFETCH_S0p_D1 && sprite_fetcher.TOXE_SFETCH_S0p && sprite_fetcher.SEBA_SFETCH_S1p_D5 && sprite_fetcher.VONU_SFETCH_S1p_D4;

  wire WUTY_SFETCH_DONE_TRIGp_new = sprite_fetcher.WUTY_SFETCH_DONE_TRIGp;

  uint8_t sfetch_phase_new = pack(~(sprite_fetcher.TYFO_SFETCH_S0p_D1 ^ sprite_fetcher.TOXE_SFETCH_S0p), sprite_fetcher.TOXE_SFETCH_S0p, sprite_fetcher.TULY_SFETCH_S1p, sprite_fetcher.TESE_SFETCH_S2p);

  //----------------------------------------
  // Window state has some interaction with the tile fetcher here.

  auto PYNU_WIN_MODE_Ap_old = win_reg.PYNU_WIN_MODE_Ap.state;

  if (DELTA_ODD) {
    win_reg.NUNU_WIN_MATCHp.state = win_reg.PYCO_WIN_MATCHp.state;
  }

  if (DELTA_EVEN) {
    win_reg.NOPA_WIN_MODE_Bp.state = PYNU_WIN_MODE_Ap_old;
  }


  if (vid_rst_new) {
    win_reg.NUNU_WIN_MATCHp.state = 0;
    win_reg.NOPA_WIN_MODE_Bp.state = 0;
  }

  if (win_reg.NUNU_WIN_MATCHp.state) {
    win_reg.PYNU_WIN_MODE_Ap.state = 1;
  }

  if (!winen_new || line_rst_new || vid_rst_new) {
    win_reg.PYNU_WIN_MODE_Ap.state = 0;
  }

  auto PYNU_WIN_MODE_Ap_new = win_reg.PYNU_WIN_MODE_Ap.state;

  //----------

  if (DELTA_EVEN) {
    tile_fetcher.PYGO_FETCH_DONEp.state = tile_fetcher.PORY_FETCH_DONEp.state;
    tile_fetcher.NYKA_FETCH_DONEp.state = tile_fetcher.LYRY_BFETCH_DONEp.state;
  }
  else {

    tile_fetcher.PORY_FETCH_DONEp.state = tile_fetcher.NYKA_FETCH_DONEp.state;
  }

  if (PYNU_WIN_MODE_Ap_new && !win_reg.NOPA_WIN_MODE_Bp.state) {
    tile_fetcher.PORY_FETCH_DONEp.state = 0;
    tile_fetcher.NYKA_FETCH_DONEp.state = 0;
  }

  if (tile_fetcher.PYGO_FETCH_DONEp.state) {
    tile_fetcher.POKY_PRELOAD_LATCHp.state = 1;
  }

  if (!rendering_new) {
    tile_fetcher.PYGO_FETCH_DONEp.state = 0;
    tile_fetcher.PORY_FETCH_DONEp.state = 0;
    tile_fetcher.NYKA_FETCH_DONEp.state = 0;
    tile_fetcher.POKY_PRELOAD_LATCHp.state = 0;
  }

  //----------


  if (DELTA_EVEN) {
    win_reg.SOVY_WIN_HITp.state = win_reg.RYDY_WIN_HITp.state;
  }

  if (vid_rst_new) {
    win_reg.SOVY_WIN_HITp.state = 0;
  }


  if (vid_rst_new || tile_fetcher.PORY_FETCH_DONEp.state) {
    win_reg.RYDY_WIN_HITp = 0;
    win_reg.PUKU_WIN_HITn = 1;
  }
  else if (win_reg.PYNU_WIN_MODE_Ap.state && !win_reg.NOPA_WIN_MODE_Bp.state) {
    win_reg.RYDY_WIN_HITp = 1;
    win_reg.PUKU_WIN_HITn = 0;
  }


  if ((sprite_fetcher.SOBU_SFETCH_REQp && !sprite_fetcher.SUDA_SFETCH_REQp) || vid_rst_new || line_rst_new) {
    sprite_fetcher.TAKA_SFETCH_RUNNINGp.state = 1;
  }

  if (rendering_new && !tile_fetcher.POKY_PRELOAD_LATCHp.state && tile_fetcher.NYKA_FETCH_DONEp.state && tile_fetcher.PORY_FETCH_DONEp.state) {
    sprite_fetcher.TAKA_SFETCH_RUNNINGp.state = 0;
  }

  if (sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.state) {
    sprite_fetcher.TAKA_SFETCH_RUNNINGp.state = 0;
  }


  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  // This chunk is weird.

  {
    auto CLK_ABCDxxxx_old = gen_clk_old(0b11110000);
    auto CLK_ABCDxxxx_new = gen_clk_new(0b11110000);
    auto CLK_AxxxExxx_old = gen_clk_old(0b10001000);
    auto CLK_AxxxExxx_new = gen_clk_new(0b10001000);

    wire oam_busy_old = (cpu_addr_old >= 0xFE00 && cpu_addr_old <= 0xFEFF) || dma_running_old;
    uint8_t BYCU_OAM_CLKp_old = (oam_busy_old ? CLK_ABCDxxxx_old : 1) && (!rendering_old || (sfetch_phase_old != 3)) && (!scanning_old || CLK_AxxxExxx_old);

    wire oam_busy_new = (cpu_addr_new >= 0xFE00 && cpu_addr_new <= 0xFEFF) || dma_running_new;
    uint8_t BYCU_OAM_CLKp_new = (oam_busy_new ? CLK_ABCDxxxx_new : 1) && (!rendering_new || (sfetch_phase_new != 3)) && (!scanning_new || CLK_AxxxExxx_new);
    auto clk_old = BYCU_OAM_CLKp_old;
    auto clk_new = BYCU_OAM_CLKp_new;

    if (posedge(clk_old, clk_new)) {
      bit_copy_inv(oam_temp_a, oam_latch_a);
      bit_copy_inv(oam_temp_b, oam_latch_b);
    }
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  // FIXME need to ditch these adders

  Adder ERUC_YDIFF0 = add3(!reg_ly.MUWY_LY0p.state, oam_temp_a.XUSO_OAM_DA0p.state, 0);
  Adder ENEF_YDIFF1 = add3(!reg_ly.MYRO_LY1p.state, oam_temp_a.XEGU_OAM_DA1p.state, ERUC_YDIFF0.carry);
  Adder FECO_YDIFF2 = add3(!reg_ly.LEXA_LY2p.state, oam_temp_a.YJEX_OAM_DA2p.state, ENEF_YDIFF1.carry);
  Adder GYKY_YDIFF3 = add3(!reg_ly.LYDO_LY3p.state, oam_temp_a.XYJU_OAM_DA3p.state, FECO_YDIFF2.carry);
  Adder GOPU_YDIFF4 = add3(!reg_ly.LOVU_LY4p.state, oam_temp_a.YBOG_OAM_DA4p.state, GYKY_YDIFF3.carry);
  Adder FUWA_YDIFF5 = add3(!reg_ly.LEMA_LY5p.state, oam_temp_a.WYSO_OAM_DA5p.state, GOPU_YDIFF4.carry);
  Adder GOJU_YDIFF6 = add3(!reg_ly.MATO_LY6p.state, oam_temp_a.XOTE_OAM_DA6p.state, FUWA_YDIFF5.carry);
  Adder WUHU_YDIFF7 = add3(!reg_ly.LAFO_LY7p.state, oam_temp_a.YZAB_OAM_DA7p.state, GOJU_YDIFF6.carry);

  SpriteDeltaY sprite_delta_y = {
    ERUC_YDIFF0,
    ENEF_YDIFF1,
    FECO_YDIFF2,
    GYKY_YDIFF3,
    GOPU_YDIFF4,
    FUWA_YDIFF5,
    GOJU_YDIFF6,
    WUHU_YDIFF7,
  };

  wire GOVU_SPSIZE_MATCH = or2(~reg_lcdc.XYMO_LCDC_SPSIZEn.state, sprite_delta_y.GYKY_YDIFF3.sum);

  if (vid_rst_new) {
    DEZY_COUNT_CLKp.rst();

    bit_clear(sprite_counter);
    bit_clear(sprite_reset_flags);
    bit_clear(sprite_store_flags);

    bit_set(store_x0);
    bit_set(store_x1);
    bit_set(store_x2);
    bit_set(store_x3);
    bit_set(store_x4);
    bit_set(store_x5);
    bit_set(store_x6);
    bit_set(store_x7);
    bit_set(store_x8);
    bit_set(store_x9);
  }
  else if (line_rst_new) {

    // FIXME does this even matter?
    if (DELTA_ODD) {
      DEZY_COUNT_CLKp.state = 1;
    }

    bit_clear(sprite_counter);
    bit_clear(sprite_reset_flags);
    bit_clear(sprite_store_flags);

    bit_set(store_x0);
    bit_set(store_x1);
    bit_set(store_x2);
    bit_set(store_x3);
    bit_set(store_x4);
    bit_set(store_x5);
    bit_set(store_x6);
    bit_set(store_x7);
    bit_set(store_x8);
    bit_set(store_x9);
  }
  else {
    wire GACE_SPRITE_DELTA4 = !sprite_delta_y.GOPU_YDIFF4.sum;
    wire GUVU_SPRITE_DELTA5 = !sprite_delta_y.FUWA_YDIFF5.sum;
    wire GYDA_SPRITE_DELTA6 = !sprite_delta_y.GOJU_YDIFF6.sum;
    wire GEWY_SPRITE_DELTA7 = !sprite_delta_y.WUHU_YDIFF7.sum;

    wire WOTA_SCAN_MATCH_Yn = !GACE_SPRITE_DELTA4 || !GUVU_SPRITE_DELTA5 || !GYDA_SPRITE_DELTA6 || !GEWY_SPRITE_DELTA7 || !sprite_delta_y.WUHU_YDIFF7.carry || !GOVU_SPSIZE_MATCH;

    auto ssf_clk = !CLK_xBCxxFGx || !sprite_scanner.CENO_SCANNINGn.state || WOTA_SCAN_MATCH_Yn;

    auto sprite_count_old = bit_pack(sprite_counter);


    if (DELTA_ODD) {
      if (posedge(DEZY_COUNT_CLKp.state, ssf_clk)) {
        if (sprite_count_old != 10) {
          bit_unpack(sprite_counter, sprite_count_old + 1);
        }
      }

      DEZY_COUNT_CLKp.state = ssf_clk;
    }


    auto sprite_count_new = bit_pack(sprite_counter);

    if (!WUTY_SFETCH_DONE_TRIGp_old && WUTY_SFETCH_DONE_TRIGp_new) {
      auto pack_sprite_match_flags = bit_pack(sprite_match_flags);
      bit_unpack(sprite_reset_flags, pack_sprite_match_flags);
    }

    auto store_rst = bit_pack(sprite_reset_flags);

    auto store_clk_old = bit_pack_inv(sprite_store_flags);
    for (int i = 0; i < 10; i++) {
      (&sprite_store_flags.DYHU_STORE0_CLKn)[i] = (i == (int)sprite_count_new) && !bit(ssf_clk);
    }
    auto store_clk_new = bit_pack_inv(sprite_store_flags);

    auto store_clk_pe = (~store_clk_old) & store_clk_new;
    auto store_clk_ne = store_clk_old & (~store_clk_new);

    if (get_bit(store_clk_ne, 0)) bit_copy_inv(store_i0, sprite_ibus);
    if (get_bit(store_clk_ne, 1)) bit_copy_inv(store_i1, sprite_ibus);
    if (get_bit(store_clk_ne, 2)) bit_copy_inv(store_i2, sprite_ibus);
    if (get_bit(store_clk_ne, 3)) bit_copy_inv(store_i3, sprite_ibus);
    if (get_bit(store_clk_ne, 4)) bit_copy_inv(store_i4, sprite_ibus);
    if (get_bit(store_clk_ne, 5)) bit_copy_inv(store_i5, sprite_ibus);
    if (get_bit(store_clk_ne, 6)) bit_copy_inv(store_i6, sprite_ibus);
    if (get_bit(store_clk_ne, 7)) bit_copy_inv(store_i7, sprite_ibus);
    if (get_bit(store_clk_ne, 8)) bit_copy_inv(store_i8, sprite_ibus);
    if (get_bit(store_clk_ne, 9)) bit_copy_inv(store_i9, sprite_ibus);

    if (get_bit(store_clk_ne, 0)) bit_copy_inv(store_l0, sprite_lbus);
    if (get_bit(store_clk_ne, 1)) bit_copy_inv(store_l1, sprite_lbus);
    if (get_bit(store_clk_ne, 2)) bit_copy_inv(store_l2, sprite_lbus);
    if (get_bit(store_clk_ne, 3)) bit_copy_inv(store_l3, sprite_lbus);
    if (get_bit(store_clk_ne, 4)) bit_copy_inv(store_l4, sprite_lbus);
    if (get_bit(store_clk_ne, 5)) bit_copy_inv(store_l5, sprite_lbus);
    if (get_bit(store_clk_ne, 6)) bit_copy_inv(store_l6, sprite_lbus);
    if (get_bit(store_clk_ne, 7)) bit_copy_inv(store_l7, sprite_lbus);
    if (get_bit(store_clk_ne, 8)) bit_copy_inv(store_l8, sprite_lbus);
    if (get_bit(store_clk_ne, 9)) bit_copy_inv(store_l9, sprite_lbus);

    if (get_bit(store_clk_pe, 0)) bit_copy(store_x0, oam_temp_b);
    if (get_bit(store_clk_pe, 1)) bit_copy(store_x1, oam_temp_b);
    if (get_bit(store_clk_pe, 2)) bit_copy(store_x2, oam_temp_b);
    if (get_bit(store_clk_pe, 3)) bit_copy(store_x3, oam_temp_b);
    if (get_bit(store_clk_pe, 4)) bit_copy(store_x4, oam_temp_b);
    if (get_bit(store_clk_pe, 5)) bit_copy(store_x5, oam_temp_b);
    if (get_bit(store_clk_pe, 6)) bit_copy(store_x6, oam_temp_b);
    if (get_bit(store_clk_pe, 7)) bit_copy(store_x7, oam_temp_b);
    if (get_bit(store_clk_pe, 8)) bit_copy(store_x8, oam_temp_b);
    if (get_bit(store_clk_pe, 9)) bit_copy(store_x9, oam_temp_b);

    if (get_bit(store_rst, 0)) bit_set(store_x0);
    if (get_bit(store_rst, 1)) bit_set(store_x1);
    if (get_bit(store_rst, 2)) bit_set(store_x2);
    if (get_bit(store_rst, 3)) bit_set(store_x3);
    if (get_bit(store_rst, 4)) bit_set(store_x4);
    if (get_bit(store_rst, 5)) bit_set(store_x5);
    if (get_bit(store_rst, 6)) bit_set(store_x6);
    if (get_bit(store_rst, 7)) bit_set(store_x7);
    if (get_bit(store_rst, 8)) bit_set(store_x8);
    if (get_bit(store_rst, 9)) bit_set(store_x9);

  }












  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  auto XYDO_PX3p_old = pix_count.XYDO_PX3p;
  auto scx_old = bit_pack_inv(&reg_scx.DATY_SCX0n, 3);
  auto fine_cnt_old = bit_pack(&fine_scroll.RYKU_FINE_CNT0, 3);
  wire fine_match_old = fine_scroll.ROXY_FINE_SCROLL_DONEn.state && (scx_old == fine_cnt_old);

  wire clkpipe_en_new = 1;
  if (win_reg.RYDY_WIN_HITp.state) clkpipe_en_new = 0;
  if (!tile_fetcher.POKY_PRELOAD_LATCHp.state) clkpipe_en_new = 0;
  if (FEPO_STORE_MATCHp.state) clkpipe_en_new = 0;
  if (WODU_HBLANKp.state) clkpipe_en_new = 0;

  if (DELTA_EVEN) {
    if (clkpipe_en_new) {
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

  if (fine_scroll.PUXA_SCX_FINE_MATCH_A.state && !fine_scroll.NYZE_SCX_FINE_MATCH_B.state) {
    fine_scroll.ROXY_FINE_SCROLL_DONEn.rst();
  }

  wire CLKPIPE_new = CLK_AxCxExGx;
  if (!clkpipe_en_new) CLKPIPE_new = 1;
  if (fine_scroll.ROXY_FINE_SCROLL_DONEn.state) CLKPIPE_new = 1;

  wire px_old = (uint8_t)bit_pack(pix_count);

  if (posedge(CLKPIPE_old, CLKPIPE_new)) {
    bit_unpack(pix_count, px_old + 1);
  }

  if (line_rst_new || vid_rst_new) {
    bit_clear(pix_count);
  }

  if (!rendering_new || sprite_scanner.CENO_SCANNINGn.state || reg_lcdc.XYLO_LCDC_SPENn.state) {
    bit_clear(sprite_match_flags);
  }
  else {
    uint8_t px = (uint8_t)bit_pack(pix_count);

    bool M0 = px == bit_pack(store_x0);
    bool M1 = px == bit_pack(store_x1);
    bool M2 = px == bit_pack(store_x2);
    bool M3 = px == bit_pack(store_x3);
    bool M4 = px == bit_pack(store_x4);
    bool M5 = px == bit_pack(store_x5);
    bool M6 = px == bit_pack(store_x6);
    bool M7 = px == bit_pack(store_x7);
    bool M8 = px == bit_pack(store_x8);
    bool M9 = px == bit_pack(store_x9);

    FEPO_STORE_MATCHp = M0 | M1 | M2 | M3 | M4 | M5 | M6 | M7 | M8 | M9;

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


  auto px_new = bit_pack(pix_count);

  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  WODU_HBLANKp = !FEPO_STORE_MATCHp.state && (px_new & 167) == 167; // WODU goes high on odd, cleared on H
  auto wodu_hblank_new = WODU_HBLANKp.state;

  {
    wire clk_old = clkpipe_en_old && gen_clk_old(0b01010101);
    wire clk_new = clkpipe_en_new && gen_clk_new(0b01010101);

    if (!clk_old && clk_new) {
      lcd.PAHO_X_8_SYNC.state = XYDO_PX3p_old.state;
    }
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  bit_set(sprite_ibus);
  bit_set(sprite_lbus);

  if (sprite_match_flags.GUVA_SPRITE0_GETp.state) bit_copy_inv(sprite_ibus, store_i0);
  if (sprite_match_flags.ENUT_SPRITE1_GETp.state) bit_copy_inv(sprite_ibus, store_i1);
  if (sprite_match_flags.EMOL_SPRITE2_GETp.state) bit_copy_inv(sprite_ibus, store_i2);
  if (sprite_match_flags.GYFY_SPRITE3_GETp.state) bit_copy_inv(sprite_ibus, store_i3);
  if (sprite_match_flags.GONO_SPRITE4_GETp.state) bit_copy_inv(sprite_ibus, store_i4);
  if (sprite_match_flags.GEGA_SPRITE5_GETp.state) bit_copy_inv(sprite_ibus, store_i5);
  if (sprite_match_flags.XOJA_SPRITE6_GETp.state) bit_copy_inv(sprite_ibus, store_i6);
  if (sprite_match_flags.GUTU_SPRITE7_GETp.state) bit_copy_inv(sprite_ibus, store_i7);
  if (sprite_match_flags.FOXA_SPRITE8_GETp.state) bit_copy_inv(sprite_ibus, store_i8);
  if (sprite_match_flags.GUZE_SPRITE9_GETp.state) bit_copy_inv(sprite_ibus, store_i9);

  if (sprite_match_flags.GUVA_SPRITE0_GETp.state) bit_copy_inv(sprite_lbus, store_l0);
  if (sprite_match_flags.ENUT_SPRITE1_GETp.state) bit_copy_inv(sprite_lbus, store_l1);
  if (sprite_match_flags.EMOL_SPRITE2_GETp.state) bit_copy_inv(sprite_lbus, store_l2);
  if (sprite_match_flags.GYFY_SPRITE3_GETp.state) bit_copy_inv(sprite_lbus, store_l3);
  if (sprite_match_flags.GONO_SPRITE4_GETp.state) bit_copy_inv(sprite_lbus, store_l4);
  if (sprite_match_flags.GEGA_SPRITE5_GETp.state) bit_copy_inv(sprite_lbus, store_l5);
  if (sprite_match_flags.XOJA_SPRITE6_GETp.state) bit_copy_inv(sprite_lbus, store_l6);
  if (sprite_match_flags.GUTU_SPRITE7_GETp.state) bit_copy_inv(sprite_lbus, store_l7);
  if (sprite_match_flags.FOXA_SPRITE8_GETp.state) bit_copy_inv(sprite_lbus, store_l8);
  if (sprite_match_flags.GUZE_SPRITE9_GETp.state) bit_copy_inv(sprite_lbus, store_l9);

  if (sprite_scanner.CENO_SCANNINGn.state || !rendering_new) {
    auto pack_sprite_index = bit_pack(sprite_index);
    bit_unpack(sprite_ibus, pack_sprite_index);
  }

  if (!FEPO_STORE_MATCHp.state) {
    sprite_lbus.BUS_SPR_L0.state = sprite_delta_y.ERUC_YDIFF0.sum;
    sprite_lbus.BUS_SPR_L1.state = sprite_delta_y.ENEF_YDIFF1.sum;
    sprite_lbus.BUS_SPR_L2.state = sprite_delta_y.FECO_YDIFF2.sum;
    sprite_lbus.BUS_SPR_L3.state = sprite_delta_y.GYKY_YDIFF3.sum;
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // WY/WX/window match

  {
    wire VYBO_CLKPIPE_odd = !FEPO_STORE_MATCHp.state && !WODU_HBLANKp.state && !MYVO_AxCxExGx();
    wire ROCO_CLKPIPE_new = !win_reg.RYDY_WIN_HITp.state && tile_fetcher.POKY_PRELOAD_LATCHp.state && VYBO_CLKPIPE_odd;

    if (!ROCO_CLKPIPE_old && ROCO_CLKPIPE_new) {
      win_reg.PYCO_WIN_MATCHp.state = win_reg.NUKO_WX_MATCHp.state;
    }
  }


  if (rendering_new) {
    if (DELTA_EVEN) {
      win_reg.RENE_WIN_FETCHn_B.state = win_reg.RYFA_WIN_FETCHn_A.state;
    }

    if (!CLKPIPE_old && CLKPIPE_new) {
      win_reg.RYFA_WIN_FETCHn_A.state = !win_reg.NUKO_WX_MATCHp.state && fine_scroll.RUBU_FINE_CNT2.state && fine_scroll.ROGA_FINE_CNT1.state && fine_scroll.RYKU_FINE_CNT0.state;
    }
  }
  else {
    win_reg.RENE_WIN_FETCHn_B.state = 0;
    win_reg.RYFA_WIN_FETCHn_A.state = 0;
  }

  if (cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
    if (cpu_addr_new == 0xFF4A) bit_copy_inv(reg_wy, cpu_dbus_old);
    if (cpu_addr_new == 0xFF4B) bit_copy_inv(reg_wx, cpu_dbus_old);
  }

  if (cpu_signals.SIG_IN_CPU_RDp.state) {
    if (cpu_addr_new == 0xFF4A) bit_copy_inv(cpu_dbus_new, reg_wy);
    if (cpu_addr_new == 0xFF4B) bit_copy_inv(cpu_dbus_new, reg_wx);
  }

  uint8_t wy_new = (uint8_t)bit_pack_inv(reg_wy);
  uint8_t wx_new = (uint8_t)bit_pack_inv(reg_wx);

  if (DELTA_BC) {
    win_reg.SARY_WY_MATCHp.state = win_reg.ROGE_WY_MATCHp.state;
  }

  if (vid_rst_new) {
    win_reg.PYCO_WIN_MATCHp.state = 0;
    win_reg.SARY_WY_MATCHp.state = 0;
  }

  win_reg.ROGE_WY_MATCHp = (ly_new == wy_new) && winen_new;

  if (win_reg.SARY_WY_MATCHp.state) win_reg.REJO_WY_MATCH_LATCHp.state = 1;
  if (lcd.POPU_y144p.state || vid_rst_new) win_reg.REJO_WY_MATCH_LATCHp.state = 0;

  win_reg.NUKO_WX_MATCHp = (wx_new == px_new) && win_reg.REJO_WY_MATCH_LATCHp.state;

  //----------------------------------------
  // Tile fetch sequencer

  uint8_t bfetch_phase_old = pack(!(tile_fetcher.LYZU_BFETCH_S0p_D1.state ^ tile_fetcher.LAXU_BFETCH_S0p.state), tile_fetcher.LAXU_BFETCH_S0p.state, tile_fetcher.MESU_BFETCH_S1p.state, tile_fetcher.NYVA_BFETCH_S2p.state);

  if (DELTA_EVEN) {
    tile_fetcher.LYZU_BFETCH_S0p_D1.state = tile_fetcher.LAXU_BFETCH_S0p.state;
  }

  if (!rendering_new) {
    tile_fetcher.LYZU_BFETCH_S0p_D1.state = 0;
  }

  if ((bfetch_phase_old < 10) && DELTA_ODD) {
    bit_unpack(&tile_fetcher.LAXU_BFETCH_S0p, 3, (bfetch_phase_old >> 1) + 1);
  }

  auto lyry_old = tile_fetcher.LYRY_BFETCH_DONEp.state;
  tile_fetcher.LYRY_BFETCH_DONEp = tile_fetcher.LAXU_BFETCH_S0p.state && tile_fetcher.NYVA_BFETCH_S2p.state;

  if (DELTA_ODD) {
    tile_fetcher.LOVY_FETCH_DONEp.state = lyry_old;
  }

  wire BFETCH_RSTp =
    sprite_scanner.AVAP_SCAN_DONE_TRIGp.state ||
    (PYNU_WIN_MODE_Ap_new && !win_reg.NOPA_WIN_MODE_Bp.state) ||
    (win_reg.RYFA_WIN_FETCHn_A.state && !win_reg.RENE_WIN_FETCHn_B.state) ||
    (!win_reg.RYDY_WIN_HITp.state && win_reg.SOVY_WIN_HITp.state) ||
    (rendering_new && !tile_fetcher.POKY_PRELOAD_LATCHp.state && tile_fetcher.NYKA_FETCH_DONEp.state && tile_fetcher.PORY_FETCH_DONEp.state);

  if (BFETCH_RSTp) {
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

  if (tile_fetcher.LOVY_FETCH_DONEp.state || !rendering_new) {
    tile_fetcher.LONY_FETCHINGp.state = 0;
  }

  uint8_t bfetch_phase_new = pack(!(tile_fetcher.LYZU_BFETCH_S0p_D1.state ^ tile_fetcher.LAXU_BFETCH_S0p.state), tile_fetcher.LAXU_BFETCH_S0p.state, tile_fetcher.MESU_BFETCH_S1p.state, tile_fetcher.NYVA_BFETCH_S2p.state);


  //----------------------------------------
  // Fine match counter

  wire TEVO_WIN_FETCH_TRIGp_new = 0;
  if (win_reg.RYFA_WIN_FETCHn_A.state && !win_reg.RENE_WIN_FETCHn_B.state) TEVO_WIN_FETCH_TRIGp_new = 1;
  if (!win_reg.RYDY_WIN_HITp.state && win_reg.SOVY_WIN_HITp.state) TEVO_WIN_FETCH_TRIGp_new = 1;
  if (!XYMU_RENDERINGn.state && !tile_fetcher.POKY_PRELOAD_LATCHp.state && tile_fetcher.NYKA_FETCH_DONEp.state && tile_fetcher.PORY_FETCH_DONEp.state) TEVO_WIN_FETCH_TRIGp_new = 1;

  wire NYXU_BFETCH_RSTn = !sprite_scanner.AVAP_SCAN_DONE_TRIGp.state && (!PYNU_WIN_MODE_Ap_new || win_reg.NOPA_WIN_MODE_Bp.state) && !TEVO_WIN_FETCH_TRIGp_new;

  wire TYFA_CLKPIPE_new = !win_reg.RYDY_WIN_HITp.state && tile_fetcher.POKY_PRELOAD_LATCHp.state && !FEPO_STORE_MATCHp.state && !wodu_hblank_new && CLK_xBxDxFxG;
  wire SACU_CLKPIPE_new = !TYFA_CLKPIPE_new || fine_scroll.ROXY_FINE_SCROLL_DONEn.state;

  {
    auto fs_old = bit_pack(&fine_scroll.RYKU_FINE_CNT0, 3);
    if (fs_old != 7 && (TYFA_CLKPIPE_old && !TYFA_CLKPIPE_new)) {
      bit_unpack(&fine_scroll.RYKU_FINE_CNT0, 3, fs_old + 1);
    }

    if (TEVO_WIN_FETCH_TRIGp_new || XYMU_RENDERINGn.state) {
      bit_clear(&fine_scroll.RYKU_FINE_CNT0, 3);
    }
  }

  //----------------------------------------
  // PPU / LCD output

  {
    auto& sf = sprite_fetcher;

    // These ffs are weird because they latches on phase change _or_ if rendering stops in the middle of a fetch
    // Good example of gate-level behavior that doesn't matter

    if (rendering_old) {
      if ((bfetch_phase_old == 6) && (bfetch_phase_new == 7 || !rendering_new)) {
        bit_copy_inv(tile_temp_a, vram_dbus);
      }

      if ((bfetch_phase_old == 2) && (bfetch_phase_new == 3 || !rendering_new)) {
        bit_copy(tile_temp_b, vram_dbus);
      }

      if ((bfetch_phase_old == 10) && (bfetch_phase_new == 11 || !rendering_new)) {
        bit_copy(tile_temp_b, vram_dbus);
      }

      if ((sfetch_phase_old == 5) && (sfetch_phase_new == 6 || !rendering_new)) {
        bit_copy_inv(sprite_pix_a, flipped_sprite);
      }

      if ((sfetch_phase_old == 9) && (sfetch_phase_new == 10 || !rendering_new)) {
        bit_copy_inv(sprite_pix_b, flipped_sprite);
      }
    }

    //----------------------------------------
    // Pal reg read/write

    if (cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
      if (cpu_addr_new == 0xFF47) bit_copy_inv(reg_bgp,  cpu_dbus_old);
      if (cpu_addr_new == 0xFF48) bit_copy_inv(reg_obp0, cpu_dbus_old);
      if (cpu_addr_new == 0xFF49) bit_copy_inv(reg_obp1, cpu_dbus_old);
    }

    if (cpu_signals.SIG_IN_CPU_RDp.state) {
      if (cpu_addr_new == 0xFF47) bit_copy_inv(cpu_dbus_new, reg_bgp);
      if (cpu_addr_new == 0xFF48) bit_copy_inv(cpu_dbus_new, reg_obp0);
      if (cpu_addr_new == 0xFF49) bit_copy_inv(cpu_dbus_new, reg_obp1);
    }

    //----------------------------------------
    // Pixel pipes

    auto tpix_a = bit_pack_inv(tile_temp_a);
    auto tpix_b = bit_pack(tile_temp_b);
    auto spix_a = bit_pack_inv(sprite_pix_a);
    auto spix_b = bit_pack_inv(sprite_pix_b);

    auto spipe_a = bit_pack(spr_pipe_a);
    auto spipe_b = bit_pack(spr_pipe_b);
    auto bpipe_a = bit_pack(bgw_pipe_a);
    auto bpipe_b = bit_pack(bgw_pipe_b);
    auto mpipe   = bit_pack(mask_pipe);
    auto ppipe   = bit_pack(pal_pipe);

    if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
      spipe_a = (spipe_a << 1) | 0;
      spipe_b = (spipe_b << 1) | 0;
      bpipe_a = (bpipe_a << 1) | 0;
      bpipe_b = (bpipe_b << 1) | 0;
      mpipe   = (mpipe   << 1) | 1;
      ppipe   = (ppipe   << 1) | 0;
    }
    
    if (!NYXU_BFETCH_RSTn) bpipe_a = tpix_a;
    if (!NYXU_BFETCH_RSTn) bpipe_b = tpix_b;

    if (sf.WUTY_SFETCH_DONE_TRIGp.state) {
      auto smask = (spipe_a | spipe_b);
      spipe_a = (spipe_a & smask) | (spix_a & ~smask);
      spipe_b = (spipe_b & smask) | (spix_b & ~smask);
      mpipe = oam_temp_b.DEPO_OAM_DB7p.state ? mpipe | ~smask : mpipe & smask;
      ppipe = oam_temp_b.GOMO_OAM_DB4p.state ? ppipe | ~smask : ppipe & smask;
    }

    bit_unpack(spr_pipe_a, spipe_a);
    bit_unpack(spr_pipe_b, spipe_b);
    bit_unpack(bgw_pipe_a, bpipe_a);
    bit_unpack(bgw_pipe_b, bpipe_b);
    bit_unpack(mask_pipe, mpipe);
    bit_unpack(pal_pipe, ppipe);

    //----------------------------------------
    // Pipe merge and output

    wire PIX_BG_LOp = bgw_pipe_a.PYBO_BGW_PIPE_A7.state && !reg_lcdc.VYXE_LCDC_BGENn.state;
    wire PIX_BG_HIp = bgw_pipe_b.SOHU_BGW_PIPE_B7.state && !reg_lcdc.VYXE_LCDC_BGENn.state;
    wire PIX_SP_LOp = spr_pipe_a.WUFY_SPR_PIPE_A7.state && !reg_lcdc.XYLO_LCDC_SPENn.state;
    wire PIX_SP_HIp = spr_pipe_b.VUPY_SPR_PIPE_B7.state && !reg_lcdc.XYLO_LCDC_SPENn.state;

    auto pal_idx = 0;
    auto pal = 0;

    auto bgp  = bit_pack_inv(reg_bgp);
    auto obp0 = bit_pack_inv(reg_obp0);
    auto obp1 = bit_pack_inv(reg_obp1);

    if (bit(or2(PIX_SP_HIp, PIX_SP_LOp))) {
      pal_idx = pack(PIX_SP_LOp, PIX_SP_HIp);
      pal = pal_pipe.LYME_PAL_PIPE_D7.state ? obp1 : obp0;
    }
    else {
      pal_idx = pack(PIX_BG_LOp, PIX_BG_HIp);
      pal = bgp;
    }

    REMY_LD0n = (pal >> (pal_idx * 2 + 0)) & 1;
    RAVO_LD1n = (pal >> (pal_idx * 2 + 1)) & 1;
  }

  //----------------------------------------
  // LCD pins

  {
    if (!vid_rst_new) {
      lcd.PIN_52_LCD_CNTRL.state = !lcd.SYGU_LINE_STROBE.state && !lcd.RUTU_x113p.state;

      if (rutu_x113p_old && !rutu_x113p_new) {
        lcd.LUCA_LINE_EVENp.state = !lcd.LUCA_LINE_EVENp.state;
      }


      if (!popu_y144p_old && popu_y144p_new) {
        lcd.NAPO_FRAME_EVENp.state = !lcd.NAPO_FRAME_EVENp.state;
      }

      lcd.PIN_56_LCD_FLIPS.state = lcd.NAPO_FRAME_EVENp.state ^ lcd.LUCA_LINE_EVENp.state;


      auto ly = bit_pack(reg_ly);

      if (nype_x113p_old && !nype_x113p_new) {
        lcd.MEDA_VSYNC_OUTn.state = ly == 0;
      }

      lcd.PIN_57_LCD_VSYNC.state = !lcd.MEDA_VSYNC_OUTn.state;

      if (sprite_scanner.AVAP_SCAN_DONE_TRIGp.state && lcd.PAHO_X_8_SYNC.state) {
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

      lcd.PIN_50_LCD_DATA1.state = RAVO_LD1n.state;
      lcd.PIN_51_LCD_DATA0.state = REMY_LD0n.state;
      lcd.PIN_54_LCD_HSYNC.state = !lcd.POFY.state;
      lcd.PIN_55_LCD_LATCH.state = !lcd.RUTU_x113p.state;

      if (pix_count.XEHO_PX0p.state && pix_count.XYDO_PX3p.state) {
        lcd.WUSA_LCD_CLOCK_GATE.state = 1;
      }
      if (VOGA_HBLANKp.state) {
        lcd.WUSA_LCD_CLOCK_GATE.state = 0;
      }

      {
        wire TOBA_LCD_CLOCK = lcd.WUSA_LCD_CLOCK_GATE.state && SACU_CLKPIPE_new;
        wire POVA_FINE_MATCH_TRIGp = fine_scroll.PUXA_SCX_FINE_MATCH_A.state && !fine_scroll.NYZE_SCX_FINE_MATCH_B.state;
        wire SEMU_LCD_CLOCK = TOBA_LCD_CLOCK || POVA_FINE_MATCH_TRIGp;
        lcd.PIN_53_LCD_CLOCK.state = !SEMU_LCD_CLOCK;
      }
    }
    else {
      lcd.LUCA_LINE_EVENp.state = 0;
      lcd.NAPO_FRAME_EVENp.state = 0;
      lcd.MEDA_VSYNC_OUTn.state = 0;
      lcd.WUSA_LCD_CLOCK_GATE.state = 0;

      lcd.POME = 1;
      lcd.RUJU = 1;
      lcd.POFY = 0;

      lcd.PIN_50_LCD_DATA1.state = RAVO_LD1n.state;
      lcd.PIN_51_LCD_DATA0.state = REMY_LD0n.state;
      lcd.PIN_52_LCD_CNTRL.state = 1;
      lcd.PIN_53_LCD_CLOCK.state = 1;
      lcd.PIN_54_LCD_HSYNC.state = 1;
      lcd.PIN_55_LCD_LATCH.state = !div.UGOT_DIV06p.state;
      lcd.PIN_56_LCD_FLIPS.state = !div.TULU_DIV07p.state;
      lcd.PIN_57_LCD_VSYNC.state = 1;
    }
  }

  //----------------------------------------
  // Audio

  tock_spu_logic();

  //----------------------------------------
  // Memory buses

  //wire addr_vram = (cpu_addr_new >= 0x8000) && (cpu_addr_new <= 0x9FFF);
  //wire addr_ram = (cpu_addr_new >= 0xA000) && (cpu_addr_new <= 0xFDFF);
  //
  //// FIXME this was packing the whole dma address without inverting the top half before, how did that ever work?
  //auto dma_addr = (pack_inv(reg_dma_hi) << 16) | pack(reg_dma_lo);
  //
  //wire dma_vram = (dma_addr >= 0x8000) && (dma_addr <= 0x9FFF);

  wire LUMA_DMA_CARTp = dma_running_new && !dma_addr_vram_new;
  wire TUTU_READ_BOOTROMp = !cpu_signals.TEPU_BOOT_BITn.state && cpu_addr_new <= 0x00FF;

  if (cpu_signals.SIG_IN_CPU_EXT_BUSp.state && !cpu_addr_vram_new) {
    bit_copy(&ext_addr_latch, 15, &cpu_abus_new);
  }

  //----------------------------------------

  if (LUMA_DMA_CARTp) {
    ext_ctrl.PIN_80_CSn.state = !dma_hi.MARU_DMA_A15n.state;
    bit_copy_inv(ext_abus, dma_lo);
    bit_copy(&ext_abus.PIN_09_A08, 7, &dma_hi.NAFA_DMA_A08n);
  }
  else {
    ext_ctrl.PIN_80_CSn.state = cpu_signals.ABUZ_EXT_RAM_CS_CLK.state && cpu_addr_ram_new;
    bit_copy_inv(&ext_abus.PIN_01_A00, 15, &ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p);
  }

  //----------------------------------------

  if (!LUMA_DMA_CARTp && cpu_signals.SIG_IN_CPU_EXT_BUSp.state && cpu_signals.SIG_IN_CPU_WRp.state) {
    ext_ctrl.PIN_79_RDn.state = cpu_addr_vram_new;
    ext_ctrl.PIN_78_WRn.state = CLK_xxxxEFGx && !cpu_addr_vram_new;
  }
  else {
    ext_ctrl.PIN_79_RDn.state = 1;
    ext_ctrl.PIN_78_WRn.state = 0;
  }

  //----------------------------------------


  if (LUMA_DMA_CARTp) {
    ext_abus.PIN_16_A15.state = dma_hi.MARU_DMA_A15n.state;
  }
  else if (TUTU_READ_BOOTROMp) {
    ext_abus.PIN_16_A15.state = 0;
  }
  else {
    ext_abus.PIN_16_A15.state = cpu_signals.ABUZ_EXT_RAM_CS_CLK.state && !cpu_abus_new.BUS_CPU_A15p.state;
  }

  CHECK_N(cpu_signals.SIG_IN_CPU_RDp.state && cpu_signals.SIG_IN_CPU_WRp.state);

  if (cpu_signals.SIG_IN_CPU_EXT_BUSp.state && cpu_signals.SIG_IN_CPU_WRp.state && !cpu_addr_vram_new) {
    bit_copy_inv(ext_dbus, cpu_dbus_new);
  }
  else {
    bit_clear(ext_dbus);
  }

  //----------------------------------------
  // Ext read

  if (ext_ctrl.PIN_79_RDn.state) {
    uint16_t ext_addr = (uint16_t)bit_pack_inv(ext_abus);
    
    auto rom_addr_mask = cart_rom_addr_mask(cart_blob);
    auto ram_addr_mask = cart_ram_addr_mask(cart_blob);

    const int region = ext_addr >> 13;
    uint8_t data_in = 0x00;

    bool ext_read_en = false;

    if (cart_has_mbc1(cart_blob)) {

      bool mbc1_ram_en = ext_mbc.MBC1_RAM_EN.state;
      bool mbc1_mode = ext_mbc.MBC1_MODE.state;

      uint32_t mbc1_rom0_bank = mbc1_mode ? bit_pack(&ext_mbc.MBC1_BANK5, 2) : 0;
      uint32_t mbc1_rom0_addr = ((ext_addr & 0x3FFF) | (mbc1_rom0_bank << 19)) & rom_addr_mask;

      uint32_t mbc1_rom1_bank = bit_pack(&ext_mbc.MBC1_BANK0, 7);
      if ((mbc1_rom1_bank & 0x1F) == 0) mbc1_rom1_bank |= 1;
      uint32_t mbc1_rom1_addr = ((ext_addr & 0x3FFF) | (mbc1_rom1_bank << 14)) & rom_addr_mask;

      uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&ext_mbc.MBC1_BANK5, 2) : 0;
      uint32_t mbc1_ram_addr = ((ext_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & ram_addr_mask;

      switch (region) {
      case 0: ext_read_en = true; data_in = cart_blob[mbc1_rom0_addr]; break;
      case 1: ext_read_en = true; data_in = cart_blob[mbc1_rom0_addr]; break;
      case 2: ext_read_en = true; data_in = cart_blob[mbc1_rom1_addr]; break;
      case 3: ext_read_en = true; data_in = cart_blob[mbc1_rom1_addr]; break;
      case 4: data_in = 0x00; break;
      case 5: ext_read_en = true; data_in = mbc1_ram_en ? cart_ram[mbc1_ram_addr] : 0xFF; break;
      case 6: ext_read_en = true; data_in = int_ram[ext_addr & 0x1FFF]; break;
      case 7: ext_read_en = true; data_in = int_ram[ext_addr & 0x1FFF]; break;
      }
    }
    else {
      switch (region) {
      case 0: ext_read_en = true; data_in = cart_blob[ext_addr & rom_addr_mask]; break;
      case 1: ext_read_en = true; data_in = cart_blob[ext_addr & rom_addr_mask]; break;
      case 2: ext_read_en = true; data_in = cart_blob[ext_addr & rom_addr_mask]; break;
      case 3: ext_read_en = true; data_in = cart_blob[ext_addr & rom_addr_mask]; break;
      case 4: data_in = 0x00; break;
      case 5: data_in = 0x00; break;
      case 6: ext_read_en = true; data_in = int_ram[ext_addr & 0x1FFF]; break;
      case 7: ext_read_en = true; data_in = int_ram[ext_addr & 0x1FFF]; break;
      }
    }

    if (ext_read_en) bit_unpack_inv(ext_dbus, data_in);
  }


  //----------------------------------------
  // Ext write


  if (ext_ctrl.PIN_78_WRn.state) {
    auto ext_addr = bit_pack_inv(ext_abus);
    auto region = ext_addr >> 13;
    auto data_out = bit_pack_inv(ext_dbus);

    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_ram_en = ext_mbc.MBC1_RAM_EN.state;
      bool mbc1_mode = ext_mbc.MBC1_MODE.state;

      auto mbc1_ram_bank = mbc1_mode ? bit_pack(&ext_mbc.MBC1_BANK5, 2) : 0;
      auto mbc1_ram_addr = ((ext_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask(cart_blob);

      switch (region) {
      case 0: ext_mbc.MBC1_RAM_EN = (data_out & 0x0F) == 0x0A; break;
      case 1: bit_unpack(&ext_mbc.MBC1_BANK0, 5, data_out); break;
      case 2: bit_unpack(&ext_mbc.MBC1_BANK5, 2, data_out); break;
      case 3: ext_mbc.MBC1_MODE = (data_out & 1); break;
      case 4: break;
      case 5: if (cart_has_ram(cart_blob) && mbc1_ram_en) cart_ram[mbc1_ram_addr & cart_ram_addr_mask(cart_blob)] = (uint8_t)data_out; break;
      case 6: int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out; break;
      case 7: int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out; break;
      }
    }
    else {
      switch (region) {
      case 0: break;
      case 1: break;
      case 2: break;
      case 3: break;
      case 4: break;
      case 5: if (cart_has_ram(cart_blob)) cart_ram[ext_addr & cart_ram_addr_mask(cart_blob)] = (uint8_t)data_out; break;
      case 6: int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out;break;
      case 7: int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out;break;
      }
    }
  }

  //----------------------------------------


  if (cpu_signals.SIG_IN_CPU_RDp.state && cpu_signals.SIG_IN_CPU_EXT_BUSp.state && !cpu_addr_vram_new && cpu_signals.SIG_IN_CPU_LATCH_EXT.state) {
    bit_copy_inv(cpu_dbus_new, ext_data_latch);
  }
  else {
    bit_copy(ext_data_latch, ext_dbus);
  }

  {
    bit_set(vram_abus);
    bit_set(vram_dbus);

    //--------------------------------------------
    // CPU vram read address

    if (!dma_addr_vram_new && XYMU_RENDERINGn.state) {
      bit_copy_inv(vram_abus, cpu_abus_new);
    }

    //--------------------------------------------
    // DMA vram read address

    if (bit(dma_addr_vram_new)) {
      bit_copy_inv(vram_abus, dma_lo);
      bit_copy(&vram_abus.BUS_VRAM_A08n, 5, &dma_hi.NAFA_DMA_A08n);
    }

    //--------------------------------------------
    // SCX/SCY regs and BG map read address

    if (cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
      if (cpu_addr_new == 0xFF42) bit_copy_inv(reg_scy, cpu_dbus_old);
      if (cpu_addr_new == 0xFF43) bit_copy_inv(reg_scx, cpu_dbus_old);
    }

    if (cpu_signals.SIG_IN_CPU_RDp.state) {
      if (cpu_addr_new == 0xFF42) bit_copy_inv(cpu_dbus_new, reg_scy);
      if (cpu_addr_new == 0xFF43) bit_copy_inv(cpu_dbus_new, reg_scx);
    }

    auto px  = bit_pack(pix_count);
    auto ly  = bit_pack(reg_ly);
    auto scx = bit_pack_inv(reg_scx);
    auto scy = bit_pack_inv(reg_scy);

    auto sum_x = px + scx;
    auto sum_y = ly + scy;

    if (tile_fetcher.LONY_FETCHINGp.state && !tile_fetcher.MESU_BFETCH_S1p.state && !tile_fetcher.NYVA_BFETCH_S2p.state && !PYNU_WIN_MODE_Ap_new) {
      bit_unpack_inv(&vram_abus.BUS_VRAM_A00n, 5, sum_x >> 3);
      bit_unpack_inv(&vram_abus.BUS_VRAM_A05n, 5, sum_y >> 3);
      vram_abus.BUS_VRAM_A10n.state = reg_lcdc.XAFO_LCDC_BGMAPn.state;
      vram_abus.BUS_VRAM_A11n.state = 0;
      vram_abus.BUS_VRAM_A12n.state = 0;
    }

    //--------------------------------------------
    // Win coord x

    wire VETU_WIN_MAPp_old = TEVO_WIN_FETCH_TRIGp_old && PYNU_WIN_MODE_Ap_old;
    wire VETU_WIN_MAPp_new = TEVO_WIN_FETCH_TRIGp_new && PYNU_WIN_MODE_Ap_new;

    wire XOFO_WIN_RSTp = !winen_new || line_rst_new || reg_lcdc.XONA_LCDC_LCDENn.state;

    auto win_map_x_old = bit_pack(win_x);

    if (!VETU_WIN_MAPp_old && VETU_WIN_MAPp_new) {
      bit_unpack(win_x, win_map_x_old + 1);
    }

    if (XOFO_WIN_RSTp) bit_clear(win_x);

    auto win_map_x_new = bit_pack(win_x);

    //--------------------------------------------
    // Win coord y

    auto REPU_VBLANKp = lcd.POPU_y144p || reg_lcdc.XONA_LCDC_LCDENn;

    auto win_y_old = REPU_VBLANKp ? 0 : bit_pack(win_y);

    if (PYNU_WIN_MODE_Ap_old && !PYNU_WIN_MODE_Ap_new) {
      bit_unpack(win_y, win_y_old + 1);
    }

    if (REPU_VBLANKp) {
      win_y.VYNO_WIN_TILE_Y0 = 0;
      win_y.VUJO_WIN_TILE_Y1 = 0;
      win_y.VYMU_WIN_TILE_Y2 = 0;
      win_y.TUFU_WIN_MAP_Y0  = 0;
      win_y.TAXA_WIN_MAP_Y1  = 0;
      win_y.TOZO_WIN_MAP_Y2  = 0;
      win_y.TATE_WIN_MAP_Y3  = 0;
      win_y.TEKE_WIN_MAP_Y4  = 0;
    }

    auto win_y_new = bit_pack(win_y);


    if (tile_fetcher.LONY_FETCHINGp.state && !tile_fetcher.MESU_BFETCH_S1p.state && !tile_fetcher.NYVA_BFETCH_S2p.state && PYNU_WIN_MODE_Ap_new) {
      uint32_t addr = 0;

      auto wx = bit_pack_inv(&win_x.WYKA_WIN_MAP_X0, 5);
      auto wy = bit_pack_inv(&win_y.TUFU_WIN_MAP_Y0, 5);

      bit_unpack(&vram_abus.BUS_VRAM_A00n, 5, wx);
      bit_unpack(&vram_abus.BUS_VRAM_A05n, 5, wy);
      bit_copy_inv(&vram_abus.BUS_VRAM_A05n, 5, &win_y.TUFU_WIN_MAP_Y0);

      addr |= reg_lcdc.WOKY_LCDC_WINMAPn.state << 10;

      vram_abus.BUS_VRAM_A10n.state = get_bit(addr, 10);
      vram_abus.BUS_VRAM_A11n.state = get_bit(addr, 11);
      vram_abus.BUS_VRAM_A12n.state = get_bit(addr, 12);
    }

    //--------------------------------------------
    // BG/Win tile read address

    if (tile_fetcher.LONY_FETCHINGp.state && (tile_fetcher.MESU_BFETCH_S1p.state || tile_fetcher.NYVA_BFETCH_S2p.state)) {
      uint32_t addr  = 0;

      addr |= tile_fetcher.NYVA_BFETCH_S2p.state << 0;
      addr |= (PYNU_WIN_MODE_Ap_new ? bit_pack(&win_y, 3) : bit_mask(sum_y, 3)) << 1;
      addr |= bit_pack(tile_temp_b) << 4;
      addr |= (!tile_temp_b.PYJU_TILE_DB7p.state && reg_lcdc.WEXU_LCDC_BGTILEn.state) << 12;

      bit_unpack_inv(vram_abus, addr);
    }

    //--------------------------------------------
    // Sprite read address

    if (sprite_fetcher.TEXY_SFETCHINGp.state) {
      uint32_t addr = 1 << 12;

      addr |= sprite_fetcher.VONU_SFETCH_S1p_D4.state;

      auto line = bit_pack(sprite_lbus) ^ bit_widen(!oam_temp_b.YZOS_OAM_DB6p, 4);
      auto tile = bit_pack(oam_temp_a);

      if (reg_lcdc.XYMO_LCDC_SPSIZEn) {
        addr |= ((tile & 0b11111111) << 4) | ((line & 0b01111) << 1);
      }
      else {
        addr |= ((tile & 0b11111110) << 4) | ((line & 0b11111) << 1);
      }

      bit_unpack_inv(&vram_abus.BUS_VRAM_A00n, 12, addr);
    }

    //--------------------------------------------
    // Vram address pin driver

    bit_copy(vram_ext_abus, vram_abus);

    auto vram_addr = bit_pack_inv(vram_ext_abus);

    //--------------------------------------------
    // CPU bus to Vram data bus

    if (cpu_signals.ABUZ_EXT_RAM_CS_CLK && XYMU_RENDERINGn && cpu_addr_vram_new && cpu_signals.SIG_IN_CPU_WRp.state) {
      bit_copy(vram_dbus, cpu_dbus_new);
    }

    //--------------------------------------------
    // Vram control pins

    if (XYMU_RENDERINGn.state) {
      wire APOV_CPU_WRp = CLK_xxxxEFGx && cpu_signals.SIG_IN_CPU_WRp.state;
      wire ABUZ_EXT_RAM_CS_CLK = CLK_xxCDEFGH && cpu_signals.SIG_IN_CPU_EXT_BUSp.state;

      if (dma_addr_vram_new) {
        vram_ext_ctrl.PIN_43_VRAM_CSn.state = 1;
        vram_ext_ctrl.PIN_45_VRAM_OEn.state = 1;
      }
      else {
        vram_ext_ctrl.PIN_43_VRAM_CSn.state = cpu_addr_vram_new && ABUZ_EXT_RAM_CS_CLK;
        vram_ext_ctrl.PIN_45_VRAM_OEn.state = !cpu_addr_vram_new || !cpu_signals.SIG_IN_CPU_WRp.state;
      }

      vram_ext_ctrl.PIN_49_VRAM_WRn.state = cpu_addr_vram_new && APOV_CPU_WRp && ABUZ_EXT_RAM_CS_CLK;
    }
    else {
      if (dma_addr_vram_new) {
        vram_ext_ctrl.PIN_43_VRAM_CSn.state = 1;
        vram_ext_ctrl.PIN_49_VRAM_WRn.state = 0;
        vram_ext_ctrl.PIN_45_VRAM_OEn.state = 1;
      }
      else if (tile_fetcher.LONY_FETCHINGp.state) {
        vram_ext_ctrl.PIN_43_VRAM_CSn.state = 1;
        vram_ext_ctrl.PIN_49_VRAM_WRn.state = 0;
        vram_ext_ctrl.PIN_45_VRAM_OEn.state = 1;
      }
      else if (sprite_fetcher.TEXY_SFETCHINGp.state) {
        vram_ext_ctrl.PIN_43_VRAM_CSn.state = 1;
        vram_ext_ctrl.PIN_49_VRAM_WRn.state = 0;
        vram_ext_ctrl.PIN_45_VRAM_OEn.state = !sprite_fetcher.TYFO_SFETCH_S0p_D1.state || sprite_fetcher.TOXE_SFETCH_S0p.state;
      }
      else {
        vram_ext_ctrl.PIN_43_VRAM_CSn.state = 0;
        vram_ext_ctrl.PIN_49_VRAM_WRn.state = 0;
        vram_ext_ctrl.PIN_45_VRAM_OEn.state = 0;
      }
    }

    uint8_t data = 0xFF;
    if (bit(vram_ext_ctrl.PIN_45_VRAM_OEn.state)) {
      data = vid_ram[vram_addr];
    }

    //--------------------------------------------
    // Vram data pin driver

    bit_clear(vram_ext_dbus);

    if (vram_ext_ctrl.PIN_45_VRAM_OEn.state) {
      bit_unpack_inv(vram_ext_dbus, vid_ram[vram_addr]);
    }

    if (vram_ext_ctrl.PIN_49_VRAM_WRn.state) {
      vid_ram[vram_addr] = (uint8_t)bit_pack_inv(vram_ext_dbus);
    }

    if (cpu_addr_vram_new && cpu_signals.ABUZ_EXT_RAM_CS_CLK.state && XYMU_RENDERINGn.state && cpu_signals.SIG_IN_CPU_WRp.state) {
      bit_copy_inv(vram_ext_dbus, vram_dbus);
    }

    //--------------------------------------------

    if (vram_ext_ctrl.PIN_49_VRAM_WRn.state) {
      vid_ram[vram_addr] = (uint8_t)bit_pack_inv(vram_ext_dbus);
    }

    //--------------------------------------------
    // Vram pins to vram bus

    if (!cpu_addr_vram_new || !cpu_signals.ABUZ_EXT_RAM_CS_CLK.state || !XYMU_RENDERINGn.state || !cpu_signals.SIG_IN_CPU_WRp.state) {
      bit_copy_inv(vram_dbus, vram_ext_dbus);
    }

    //--------------------------------------------
    // Vram bus to cpu bus

    if (cpu_addr_vram_new && cpu_signals.ABUZ_EXT_RAM_CS_CLK.state && XYMU_RENDERINGn.state && cpu_signals.SIG_IN_CPU_RDp.state && cpu_signals.SIG_IN_CPU_LATCH_EXT.state) {
      bit_copy(cpu_dbus_new, vram_dbus);
    }

    //--------------------------------------------
    // Vram bus to sprite x flipper

    wire XONO_FLIP_X = oam_temp_b.BAXO_OAM_DB5p.state && sprite_fetcher.TEXY_SFETCHINGp.state;

    flipped_sprite = {
      XONO_FLIP_X ? vram_dbus.BUS_VRAM_D07p.state : vram_dbus.BUS_VRAM_D00p.state,
      XONO_FLIP_X ? vram_dbus.BUS_VRAM_D06p.state : vram_dbus.BUS_VRAM_D01p.state,
      XONO_FLIP_X ? vram_dbus.BUS_VRAM_D05p.state : vram_dbus.BUS_VRAM_D02p.state,
      XONO_FLIP_X ? vram_dbus.BUS_VRAM_D04p.state : vram_dbus.BUS_VRAM_D03p.state,
      XONO_FLIP_X ? vram_dbus.BUS_VRAM_D03p.state : vram_dbus.BUS_VRAM_D04p.state,
      XONO_FLIP_X ? vram_dbus.BUS_VRAM_D02p.state : vram_dbus.BUS_VRAM_D05p.state,
      XONO_FLIP_X ? vram_dbus.BUS_VRAM_D01p.state : vram_dbus.BUS_VRAM_D06p.state,
      XONO_FLIP_X ? vram_dbus.BUS_VRAM_D00p.state : vram_dbus.BUS_VRAM_D07p.state,
    };
  }



  //----------------------------------------
  // oam

  {
    wire cpu_rd = cpu_signals.SIG_IN_CPU_RDp.state;
    wire cpu_wr = cpu_signals.SIG_IN_CPU_WRp.state && CLK_xxxxEFGx;
  
    // FIXME I'm feeling that the cpu_latch signal is really "cpu data bus free"
    wire dbus_free = cpu_signals.SIG_IN_CPU_LATCH_EXT.state;
    wire dbus_busy = !dbus_free;

    wire addr_oam = (cpu_addr_new >= 0xFE00) && (cpu_addr_new <= 0xFEFF);

    bool cpu_reading_oam = dbus_busy && addr_oam && cpu_rd;

    // this is weird, why is it always 0 when not in reset?
    oam_ctrl.MAKA_LATCH_EXTp.state = 0;

    bit_set(oam_abus);
    bit_set(oam_dbus_a);
    bit_set(oam_dbus_b);

    if (CLK_ABCDxxxx) {
      oam_ctrl.WUJE_CPU_OAM_WRn.state = 1;
    }

    if (addr_oam && cpu_wr) {
      oam_ctrl.WUJE_CPU_OAM_WRn.state = 0;
    }

    wire oam_clk_old = oam_ctrl.SIG_OAM_CLKn.state;

    if (dma_running_new) {
      bit_copy_inv(oam_abus, dma_lo);

      if ((dma_addr_new >= 0x8000) && (dma_addr_new <= 0x9FFF)) {
        bit_copy_inv(oam_dbus_a, vram_dbus);
        bit_copy_inv(oam_dbus_b, vram_dbus);
      }
      else {
        bit_copy(oam_dbus_a, ext_dbus);
        bit_copy(oam_dbus_b, ext_dbus);
      }

      oam_ctrl.SIG_OAM_CLKn .state = CLK_ABCDxxxx;
      oam_ctrl.SIG_OAM_WRn_A.state = CLK_ABCDxxxx || !oam_abus.BUS_OAM_A00n.state;
      oam_ctrl.SIG_OAM_WRn_B.state = CLK_ABCDxxxx ||  oam_abus.BUS_OAM_A00n.state;
      oam_ctrl.SIG_OAM_OEn  .state = !cpu_reading_oam;
    }
    else if (scanning_new) {
      oam_abus.BUS_OAM_A00n.state = 1;
      oam_abus.BUS_OAM_A01n.state = 1;
      bit_copy_inv(&oam_abus.BUS_OAM_A02n, 6, &scan_counter);


      oam_ctrl.SIG_OAM_CLKn .state = clk.WOSU_AxxDExxH.state && clk.WUVU_ABxxEFxx.state && (!addr_oam || !CLK_xxxxEFGH);
      oam_ctrl.SIG_OAM_WRn_A.state = 1;
      oam_ctrl.SIG_OAM_WRn_B.state = 1;
      oam_ctrl.SIG_OAM_OEn  .state = clk.WOSU_AxxDExxH.state && !cpu_reading_oam;
    }
    else if (rendering_new) {
      oam_abus.BUS_OAM_A00n.state = 0;
      oam_abus.BUS_OAM_A01n.state = 0;
      bit_copy_inv(&oam_abus.BUS_OAM_A02n, 6, &sprite_ibus.BUS_SPR_I0);

      oam_ctrl.SIG_OAM_CLKn .state = bit(and2(nand3(~sprite_fetcher.TULY_SFETCH_S1p.state, ~sprite_fetcher.TESE_SFETCH_S2p.state, nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.state, not1(sprite_fetcher.TOXE_SFETCH_S0p.state))), nand2(addr_oam, CLK_xxxxEFGH)));
      oam_ctrl.SIG_OAM_WRn_A.state = 1;
      oam_ctrl.SIG_OAM_WRn_B.state = 1;
      oam_ctrl.SIG_OAM_OEn  .state = bit(and2(nand3(~sprite_fetcher.TULY_SFETCH_S1p.state, ~sprite_fetcher.TESE_SFETCH_S2p.state, sprite_fetcher.TYFO_SFETCH_S0p_D1.state), !cpu_reading_oam));
    }
    else if (addr_oam) {
      bit_copy_inv(oam_abus, cpu_abus_new);
      if (bit(~oam_ctrl.WUJE_CPU_OAM_WRn.state)) {
        bit_copy_inv(oam_dbus_a, cpu_dbus_new);
        bit_copy_inv(oam_dbus_b, cpu_dbus_new);
      }
      oam_ctrl.SIG_OAM_CLKn .state = bit(CLK_ABCDxxxx);
      oam_ctrl.SIG_OAM_WRn_A.state = bit(nand2(cpu_wr,  oam_abus.BUS_OAM_A00n.state));
      oam_ctrl.SIG_OAM_WRn_B.state = bit(nand2(cpu_wr, !oam_abus.BUS_OAM_A00n.state));
      oam_ctrl.SIG_OAM_OEn  .state = bit(nand2(cpu_rd, dbus_busy));
    }
    else {
      bit_copy_inv(oam_abus,   cpu_abus_new);
      bit_copy_inv(oam_dbus_a, cpu_dbus_new);
      bit_copy_inv(oam_dbus_b, cpu_dbus_new);
      oam_ctrl.SIG_OAM_CLKn .state = 1;
      oam_ctrl.SIG_OAM_WRn_A.state = 1;
      oam_ctrl.SIG_OAM_WRn_B.state = 1;
      oam_ctrl.SIG_OAM_OEn  .state = 1;
    }

    wire oam_clk_new = oam_ctrl.SIG_OAM_CLKn.state;

    // data in from oam
    uint8_t oam_data_a, oam_data_b;
    {

      uint8_t oam_addr = (uint8_t)bit_pack_inv(oam_abus) >> 1;
      oam_data_a = (uint8_t)bit_pack_inv(oam_dbus_a);
      oam_data_b = (uint8_t)bit_pack_inv(oam_dbus_b);

      if (negedge(oam_clk_old, oam_clk_new)) {
        if (bit(!oam_ctrl.SIG_OAM_WRn_A.state)) oam_ram[(oam_addr << 1) + 0] = oam_data_a;
        if (bit(!oam_ctrl.SIG_OAM_WRn_B.state)) oam_ram[(oam_addr << 1) + 1] = oam_data_b;
      }
      oam_ctrl.old_oam_clk = bit(!oam_ctrl.SIG_OAM_CLKn.state);

      oam_data_a = oam_ram[(oam_addr << 1) + 0];
      oam_data_b = oam_ram[(oam_addr << 1) + 1];
    }






    bool latch_oam = false;
    latch_oam |= cpu_reading_oam;
    latch_oam |= scanning_new && gen_clk_new(0b01100110);
    latch_oam |= rendering_new && (bool)bit(and3(!sprite_fetcher.TULY_SFETCH_S1p.state, !sprite_fetcher.TESE_SFETCH_S2p.state, sprite_fetcher.TYFO_SFETCH_S0p_D1.state));

    if (latch_oam) {
      bit_unpack_inv(oam_dbus_a, oam_data_a);
      bit_unpack_inv(oam_dbus_b, oam_data_b);
      bit_copy(oam_latch_a, oam_dbus_a);
      bit_copy(oam_latch_b, oam_dbus_b);
    }

    if (cpu_rd && dbus_free && addr_oam && !latch_oam && !dma_running_new && !scanning_new && !rendering_new) {
      if (bit(oam_abus.BUS_OAM_A00n.state)) {
        bit_copy_inv(cpu_dbus_new, oam_latch_a);
      }
      else {
        bit_copy_inv(cpu_dbus_new, oam_latch_b);
      }
    }
  }

  //----------------------------------------
  // zram

  {
    wire CSp = (cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE);

    if (bit(zram_bus.clk_old.state & !cpu_signals.TAPU_CPU_WRp.state & CSp)) {
      zero_ram[cpu_addr_new & 0x007F] = (uint8_t)bit_pack(cpu_dbus_old);
    }
    zram_bus.clk_old = cpu_signals.TAPU_CPU_WRp.state;

    uint8_t data = zero_ram[cpu_addr_new & 0x007F];

    if (CSp && bit(cpu_signals.TEDO_CPU_RDp.state)) {
      bit_unpack(cpu_dbus_new, data);
    }
  }

  //----------------------------------------
  // And finally, interrupts.

  {
    auto pack_cpu_dbus_old = bit_pack(cpu_dbus_old);
    auto pack_cpu_dbus_new = bit_pack(cpu_dbus_new);
    auto pack_ie = bit_pack(reg_ie);
    auto pack_if = bit_pack(reg_if);
    auto pack_stat = bit_pack(reg_stat);

    auto CLK_xxxxEFGx_new = gen_clk_new(0b00001110);

    if (cpu_addr_new == 0xFFFF && bit(cpu_signals.SIG_IN_CPU_WRp.state) && DELTA_GH) {
      pack_ie = pack_cpu_dbus_old;
    }

    if (cpu_addr_new == 0xFF41 && bit(cpu_signals.SIG_IN_CPU_WRp.state) && DELTA_GH) {
      pack_stat = (~pack_cpu_dbus_old >> 3) & 0b00001111;
    }

    if (cpu_addr_new == 0xFF41 && cpu_rd_new) {
      uint8_t data = 0x80;

      data |= (!XYMU_RENDERINGn.state || lcd.POPU_y144p.state) << 0;
      data |= (!XYMU_RENDERINGn.state || sprite_scanner.ACYL_SCANNINGp.state) << 1;
      data |= (!RUPO_LYC_MATCHn.state) << 2;
      data |= (pack_stat ^ 0b1111) << 3;

      pack_cpu_dbus_new = data;
    }


    bool int_stat_new = 0;
    if (!get_bit(pack_stat, 0) && bit(and2(wodu_hblank_new, !lcd.POPU_y144p.state))) int_stat_new = 1;
    if (!get_bit(pack_stat, 1) && bit(lcd.POPU_y144p.state)) int_stat_new = 1;
    if (!get_bit(pack_stat, 2) && bit(and2(!lcd.POPU_y144p.state, lcd.RUTU_x113p.qp_new()))) int_stat_new = 1;
    if (!get_bit(pack_stat, 3) && bit(ROPO_LY_MATCH_SYNCp.state)) int_stat_new = 1;

    wire int_lcd_new = lcd.POPU_y144p.state;
    wire int_joy_new = nand2(joy_int.APUG_JP_GLITCH3.state, joy_int.BATU_JP_GLITCH0.state);
    wire int_tim_new = int_ctrl.MOBA_TIMER_OVERFLOWp.state;
    //wire int_ser = serial.CALY_SER_CNT3.state;
    wire int_ser_new = 0;



    if (posedge(int_lcd_old, int_lcd_new))   pack_if |= (1 << 0);
    if (posedge(int_stat_old, int_stat_new)) pack_if |= (1 << 1);
    if (posedge(int_tim_old, int_tim_new))   pack_if |= (1 << 2);
    if (posedge(int_ser_old, int_ser_new))   pack_if |= (1 << 3);
    if (posedge(int_joy_old, int_joy_new))   pack_if |= (1 << 4);

    // note this is an async set so it doesn't happen on the GH clock edge like other writes
    if (cpu_signals.SIG_IN_CPU_WRp.state & (cpu_addr_new == 0xFF0F) & CLK_xxxxEFGx_new) {
      pack_if = pack_cpu_dbus_new;
    }

    pack_if &= ~bit_pack(cpu_ack);

    if (cpu_addr_new == 0xFFFF && bit(cpu_signals.SIG_IN_CPU_RDp.state)) {
      pack_cpu_dbus_new = pack_ie | 0b11100000;
    }

    if (cpu_addr_new == 0xFF0F && bit(cpu_signals.SIG_IN_CPU_RDp.state)) {
      bit_unpack(latch_if,  pack_if);
      pack_cpu_dbus_new = pack_if | 0b11100000;
    }

    bit_unpack(cpu_dbus_new, pack_cpu_dbus_new);
    bit_unpack(cpu_int, pack_if);
    bit_unpack(reg_ie, pack_ie);
    bit_unpack(reg_if, pack_if);
    bit_unpack(reg_stat, pack_stat);
  }

  //----------------------------------------

  cpu_abus_old = cpu_abus_new;
  cpu_dbus_old = cpu_dbus_new;
}
