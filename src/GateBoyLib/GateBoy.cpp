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
  bool old_logic_mode = sys.logic_mode;
  sys.logic_mode = false;

  // Put some recognizable pattern in vram so we can see that we're in the bootrom
  for (int i = 0; i < 8192; i++) {
    uint32_t h = i * 0x1234567;
    mem.vid_ram[i] = uint8_t(h ^ (h >> 4));
  }

  reg.cpu_abus_new.reset_to_bootrom();
  reg.cpu_dbus_new.reset_to_bootrom();

  reg.sprite_ibus.reset_to_bootrom();
  reg.sprite_lbus.reset_to_bootrom();

  reg.dma_hi.NAFA_DMA_A08n.state = 0b00011010;
  reg.dma_hi.PYNE_DMA_A09n.state = 0b00011010;
  reg.dma_hi.PARA_DMA_A10n.state = 0b00011010;
  reg.dma_hi.NYDO_DMA_A11n.state = 0b00011010;
  reg.dma_hi.NYGY_DMA_A12n.state = 0b00011010;
  reg.dma_hi.PULA_DMA_A13n.state = 0b00011010;
  reg.dma_hi.POKU_DMA_A14n.state = 0b00011010;
  reg.dma_hi.MARU_DMA_A15n.state = 0b00011010;

  reg.reg_bgp.PAVO_BGP_D0n.state = 0b00011010;
  reg.reg_bgp.NUSY_BGP_D1n.state = 0b00011010;
  reg.reg_bgp.PYLU_BGP_D2n.state = 0b00011010;
  reg.reg_bgp.MAXY_BGP_D3n.state = 0b00011010;
  reg.reg_bgp.MUKE_BGP_D4n.state = 0b00011010;
  reg.reg_bgp.MORU_BGP_D5n.state = 0b00011010;
  reg.reg_bgp.MOGY_BGP_D6n.state = 0b00011010;
  reg.reg_bgp.MENA_BGP_D7n.state = 0b00011010;

  reg.reg_obp0.XUFU_OBP0_D0n.state = 0b00011010;
  reg.reg_obp0.XUKY_OBP0_D1n.state = 0b00011010;
  reg.reg_obp0.XOVA_OBP0_D2n.state = 0b00011010;
  reg.reg_obp0.XALO_OBP0_D3n.state = 0b00011010;
  reg.reg_obp0.XERU_OBP0_D4n.state = 0b00011010;
  reg.reg_obp0.XYZE_OBP0_D5n.state = 0b00011010;
  reg.reg_obp0.XUPO_OBP0_D6n.state = 0b00011010;
  reg.reg_obp0.XANA_OBP0_D7n.state = 0b00011010;

  reg.reg_obp1.MOXY_OBP1_D0n.state = 0b00011010;
  reg.reg_obp1.LAWO_OBP1_D1n.state = 0b00011010;
  reg.reg_obp1.MOSA_OBP1_D2n.state = 0b00011010;
  reg.reg_obp1.LOSE_OBP1_D3n.state = 0b00011010;
  reg.reg_obp1.LUNE_OBP1_D4n.state = 0b00011010;
  reg.reg_obp1.LUGU_OBP1_D5n.state = 0b00011010;
  reg.reg_obp1.LEPU_OBP1_D6n.state = 0b00011010;
  reg.reg_obp1.LUXO_OBP1_D7n.state = 0b00011010;

  reg.joy_int.reset_to_bootrom();
  reg.joy.reset_to_bootrom();

  check_state_old_and_driven_or_pulled();

  sys.fastboot = fastboot;

  //----------------------------------------
  // Update the sim without ticking the clock to to settle initial reset signals.

  sys.rst = 1;

  tock_cpu();
  tock_gates(cart_blob);

  //----------------------------------------
  // Release reset, start clock, and sync with phase

  sys.rst = 0;
  sys.clken = 1;
  sys.clkgood = 1;
  run_phases(cart_blob, 2);

  CHECK_N(bit(reg.clk.AFUR_xxxxEFGH.qp_old()));
  CHECK_P(bit(reg.clk.ALEF_AxxxxFGH.qp_old()));
  CHECK_P(bit(reg.clk.APUK_ABxxxxGH.qp_old()));
  CHECK_P(bit(reg.clk.ADYK_ABCxxxxH.qp_old()));

  sys.phase_total = 0;
  sys.phase_origin = 46880720;
;

  //----------------------------------------
  // Wait for SIG_CPU_START

  while(bit(~reg.rst.SIG_CPU_STARTp.out_old())) {
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

  sys.clkreq = 1;
  sys.cpu_en = true;

  if (fastboot) {
    reg.div.TERO_DIV03p.state = 0b00011010;
    reg.div.UNYK_DIV04p.state = 0b00011010;
    reg.div.UPOF_DIV15p.state = 0b00011011;
  }

  memset(mem.framebuffer, 4, sizeof(mem.framebuffer));

  sys.probes.reset_to_cart();

  sys.logic_mode = old_logic_mode;
  if (sys.logic_mode) wipe_flags();
}

//-----------------------------------------------------------------------------

void GateBoy::reset_to_cart(const blob& cart_blob) {
  reset_to_bootrom(cart_blob, true);

  reg.VOGA_HBLANKp.state = 0b00011001;

  reg.cpu_signals.reset_to_cart();
  reg.cpu_abus_old.reset_to_cart_new();
  reg.cpu_dbus_old.reset_to_cart_new();
  reg.cpu_abus_new.reset_to_cart_new();
  reg.cpu_dbus_new.reset_to_cart_new();

  reg.vram_abus.reset_to_cart();
  reg.vram_dbus.reset_to_cart();
  reg.vram_ext_ctrl.reset_to_cart();
  reg.vram_ext_abus.reset_to_cart();
  reg.vram_ext_dbus.reset_to_cart();

  reg.sprite_ibus.reset_to_cart();
  reg.sprite_lbus.reset_to_cart();

  reg.oam_ctrl.reset_to_cart();
  reg.oam_abus.reset_to_cart();
  reg.oam_dbus_a.reset_to_cart();
  reg.oam_dbus_b.reset_to_cart();
  reg.oam_latch_a.reset_to_cart();
  reg.oam_latch_b.reset_to_cart();
  reg.oam_temp_a.reset_to_cart();
  reg.oam_temp_b.reset_to_cart();

  reg.ext_ctrl.reset_to_cart();
  reg.ext_abus.reset_to_cart();
  reg.ext_dbus.reset_to_cart();
  reg.ext_addr_latch.reset_to_cart();
  reg.ext_data_latch.reset_to_cart();

  //zram_bus.reset_to_cart();



  reg.rst.reset_to_cart();
  reg.clk.reset_to_cart();
  reg.div.reset_to_cart();
  
  //interrupts.reset_to_cart();
  reg.reg_if.reset_to_cart();
  reg.reg_ie.reset_to_cart();
  reg.latch_if.reset_to_cart();
  reg.cpu_int.reset_to_cart();
  reg.cpu_ack.reset_to_cart();

  //serial.reset_to_cart();

  //reset_sprite_store();
  reg.DEZY_COUNT_CLKp.state = 0b00011011;
  reg.sprite_counter.BESE_SPRITE_COUNT0.state = 0b00011010;
  reg.sprite_counter.CUXY_SPRITE_COUNT1.state = 0b00011010;
  reg.sprite_counter.BEGO_SPRITE_COUNT2.state = 0b00011010;
  reg.sprite_counter.DYBE_SPRITE_COUNT3.state = 0b00011010;

  reg.sprite_scanner.reset_to_cart();
  reg.scan_counter.reset_to_cart();
  reg.sprite_index.reset_to_cart();


  reg.sprite_fetcher.reset_to_cart();

  reg.RUPO_LYC_MATCHn.state = 0b00011000;

  reg.reg_stat.reset_to_cart();
  reg.pix_count.reset_to_cart();
  reg.mask_pipe.reset_to_cart();
  reg.REMY_LD0n.state = 0b00011000;
  reg.RAVO_LD1n.state = 0b00011000;

  reg.dma_lo.reset_to_cart();
  reg.dma_hi.reset_to_cart();
  reg.dma_ctrl.reset_to_cart();

  reg.reg_bgp.reset_to_cart();
  reg.reg_obp0.reset_to_cart();
  reg.reg_obp1.reset_to_cart();
  
  //joy.reset_to_cart();
  reg.joy_int.reset_to_cart();
  reg.joy.reset_to_cart();

  reg.reg_lcdc.reset_to_cart();
  reg.lcd.reset_to_cart();

  reg.reg_lx.reset_to_cart();
  reg.reg_ly.reset_to_cart();

  reg.ROPO_LY_MATCH_SYNCp.state = 0b00011001;
  reg.reg_lyc.reset_to_cart();

  reg.WODU_HBLANKp.state = 0b00011001;

  reg.sprite_scanner.FETO_SCAN_DONEp.state = 0b00011001;
  reg.ATEJ_LINE_RSTp.state = 0b00011000;

  //reg_NR50.reset_to_cart();
  //reg_NR51.reset_to_cart();
  //reg_NR52.reset_to_cart();

  check_state_old_and_driven_or_pulled();

  sys.rst = false;
  sys.t1 = false;
  sys.t2 = false;
  sys.clken = true;
  sys.clkgood = true;
  sys.clkreq = true;
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

  if (sys.logic_mode) wipe_flags();
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

struct MemberOffset {
  const char* name;
  int offset;
};

#define GEN_OFFSET(A) { #A, offsetof(GateBoy, A) }

MemberOffset gb_offsets[] = {
  GEN_OFFSET(sentinel1),

  GEN_OFFSET(reg.SIG_VCC),
  GEN_OFFSET(reg.SIG_GND),
  GEN_OFFSET(reg.cpu_signals),
  GEN_OFFSET(reg.cpu_abus_old),
  GEN_OFFSET(reg.cpu_dbus_old),
  GEN_OFFSET(reg.cpu_abus_new),
  GEN_OFFSET(reg.cpu_dbus_new),
  GEN_OFFSET(reg.vram_abus),
  GEN_OFFSET(reg.vram_dbus),
  GEN_OFFSET(reg.vram_ext_ctrl),
  GEN_OFFSET(reg.vram_ext_abus),
  GEN_OFFSET(reg.vram_ext_dbus),
  GEN_OFFSET(reg.sprite_ibus),
  GEN_OFFSET(reg.sprite_lbus),
  GEN_OFFSET(reg.oam_ctrl),
  GEN_OFFSET(reg.oam_abus),
  GEN_OFFSET(reg.oam_dbus_a),
  GEN_OFFSET(reg.oam_dbus_b),
  GEN_OFFSET(reg.oam_latch_a),
  GEN_OFFSET(reg.oam_latch_b),
  GEN_OFFSET(reg.oam_temp_a),
  GEN_OFFSET(reg.oam_temp_b),
  GEN_OFFSET(reg.ext_ctrl),
  GEN_OFFSET(reg.ext_abus),
  GEN_OFFSET(reg.ext_dbus),
  GEN_OFFSET(reg.ext_data_latch),
  GEN_OFFSET(reg.ext_addr_latch),
  GEN_OFFSET(reg.ext_mbc),
  GEN_OFFSET(reg.zram_bus),
  GEN_OFFSET(reg.VOGA_HBLANKp),
  GEN_OFFSET(reg.XYMU_RENDERINGn),
  GEN_OFFSET(reg.rst),
  GEN_OFFSET(reg.clk),
  GEN_OFFSET(reg.div),
  GEN_OFFSET(reg.tima),
  GEN_OFFSET(reg.tma),
  GEN_OFFSET(reg.tac),
  GEN_OFFSET(reg.dma_ctrl),
  GEN_OFFSET(reg.dma_lo),
  GEN_OFFSET(reg.dma_hi),
  GEN_OFFSET(reg.int_ctrl),
  GEN_OFFSET(reg.reg_if),
  GEN_OFFSET(reg.reg_ie),
  GEN_OFFSET(reg.latch_if),
  GEN_OFFSET(reg.cpu_int),
  GEN_OFFSET(reg.cpu_ack),
  GEN_OFFSET(reg.joy_int),
  GEN_OFFSET(reg.joy),
  GEN_OFFSET(reg.joy_latch),
  GEN_OFFSET(reg.joy_ext),
  //GEN_OFFSET(serial),
  GEN_OFFSET(reg.store_i0),
  GEN_OFFSET(reg.store_i1),
  GEN_OFFSET(reg.store_i2),
  GEN_OFFSET(reg.store_i3),
  GEN_OFFSET(reg.store_i4),
  GEN_OFFSET(reg.store_i5),
  GEN_OFFSET(reg.store_i6),
  GEN_OFFSET(reg.store_i7),
  GEN_OFFSET(reg.store_i8),
  GEN_OFFSET(reg.store_i9),
  GEN_OFFSET(reg.store_l0),
  GEN_OFFSET(reg.store_l1),
  GEN_OFFSET(reg.store_l2),
  GEN_OFFSET(reg.store_l3),
  GEN_OFFSET(reg.store_l4),
  GEN_OFFSET(reg.store_l5),
  GEN_OFFSET(reg.store_l6),
  GEN_OFFSET(reg.store_l7),
  GEN_OFFSET(reg.store_l8),
  GEN_OFFSET(reg.store_l9),
  GEN_OFFSET(reg.store_x0),
  GEN_OFFSET(reg.store_x1),
  GEN_OFFSET(reg.store_x2),
  GEN_OFFSET(reg.store_x3),
  GEN_OFFSET(reg.store_x4),
  GEN_OFFSET(reg.store_x5),
  GEN_OFFSET(reg.store_x6),
  GEN_OFFSET(reg.store_x7),
  GEN_OFFSET(reg.store_x8),
  GEN_OFFSET(reg.store_x9),
  GEN_OFFSET(reg.DEZY_COUNT_CLKp),
  GEN_OFFSET(reg.sprite_counter),
  GEN_OFFSET(reg.FEPO_STORE_MATCHp),
  GEN_OFFSET(reg.sprite_match_flags),
  GEN_OFFSET(reg.sprite_reset_flags),
  GEN_OFFSET(reg.sprite_store_flags),
  GEN_OFFSET(reg.sprite_scanner),
  GEN_OFFSET(reg.scan_counter),
  GEN_OFFSET(reg.sprite_index),
  GEN_OFFSET(reg.sprite_fetcher),
  GEN_OFFSET(reg.sprite_pix_a),
  GEN_OFFSET(reg.sprite_pix_b),
  GEN_OFFSET(reg.tile_fetcher),
  GEN_OFFSET(reg.tile_temp_a),
  GEN_OFFSET(reg.tile_temp_b),
  GEN_OFFSET(reg.RUPO_LYC_MATCHn),
  GEN_OFFSET(reg.reg_lcdc),
  GEN_OFFSET(reg.reg_stat),
  GEN_OFFSET(reg.reg_scx),
  GEN_OFFSET(reg.reg_scy),
  GEN_OFFSET(reg.reg_wy),
  GEN_OFFSET(reg.reg_wx),
  GEN_OFFSET(reg.win_x),
  GEN_OFFSET(reg.win_y),
  GEN_OFFSET(reg.win_ctrl),
  GEN_OFFSET(reg.fine_scroll),
  GEN_OFFSET(reg.pix_count),
  GEN_OFFSET(reg.mask_pipe),
  GEN_OFFSET(reg.bgw_pipe_a),
  GEN_OFFSET(reg.bgw_pipe_b),
  GEN_OFFSET(reg.spr_pipe_a),
  GEN_OFFSET(reg.spr_pipe_b),
  GEN_OFFSET(reg.pal_pipe),
  GEN_OFFSET(reg.REMY_LD0n),
  GEN_OFFSET(reg.RAVO_LD1n),
  GEN_OFFSET(reg.lcd),
  GEN_OFFSET(reg.reg_lx),
  GEN_OFFSET(reg.reg_ly),
  GEN_OFFSET(reg.ROPO_LY_MATCH_SYNCp),
  GEN_OFFSET(reg.reg_lyc),
  GEN_OFFSET(reg.reg_bgp),
  GEN_OFFSET(reg.reg_obp0),
  GEN_OFFSET(reg.reg_obp1),
  GEN_OFFSET(reg.WODU_HBLANKp),
  GEN_OFFSET(reg.SATO_BOOT_BITn),
  GEN_OFFSET(reg.ATEJ_LINE_RSTp),
  GEN_OFFSET(reg.flipped_sprite),

  GEN_OFFSET(cpu.core),
  GEN_OFFSET(cpu.bus_req_new),
  GEN_OFFSET(cpu.cpu_data_latch),
  GEN_OFFSET(cpu.imask_latch),
  GEN_OFFSET(cpu.intf_latch),
  GEN_OFFSET(cpu.intf_latch_delay),
  GEN_OFFSET(cpu.intf_halt_latch),

  GEN_OFFSET(mem.vid_ram),
  GEN_OFFSET(mem.cart_ram),
  GEN_OFFSET(mem.int_ram),
  GEN_OFFSET(mem.oam_ram),
  GEN_OFFSET(mem.zero_ram),
  GEN_OFFSET(mem.framebuffer),

  GEN_OFFSET(sys.rst),
  GEN_OFFSET(sys.t1),
  GEN_OFFSET(sys.t2),
  GEN_OFFSET(sys.clken),
  GEN_OFFSET(sys.clkgood),
  GEN_OFFSET(sys.clkreq),
  GEN_OFFSET(sys.cpu_en),
  GEN_OFFSET(sys.fastboot),
  GEN_OFFSET(sys.buttons),
  GEN_OFFSET(sys.logic_mode),
  GEN_OFFSET(sys.phase_total),
  GEN_OFFSET(sys.sim_time),
  GEN_OFFSET(sys.phase_origin),
  GEN_OFFSET(sys.probes),

  GEN_OFFSET(sentinel2),
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
    if (i == offsetof(GateBoy, sys.logic_mode)) continue;

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
  CHECK_N(!sys.clkreq && sys.logic_mode);

  tock_cpu();

  if (sys.logic_mode) {
    tock_logic(cart_blob);
    update_framebuffer(bit_pack(reg.pix_count) - 8, bit_pack(reg.reg_ly), ~reg.lcd.PIN_51_LCD_DATA0.state, ~reg.lcd.PIN_50_LCD_DATA1.state);
    sys.phase_total++;
    return true;
  }
  else {
    tock_gates(cart_blob);
    update_framebuffer(bit_pack(reg.pix_count) - 8, bit_pack(reg.reg_ly), reg.lcd.PIN_51_LCD_DATA0.qp_ext_old(), reg.lcd.PIN_50_LCD_DATA1.qp_ext_old());
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
  cpu.cpu_data_latch &= (uint8_t)bit_pack(reg.cpu_dbus_new);
  cpu.imask_latch = (uint8_t)bit_pack(reg.reg_ie);

  if (DELTA_HA) {
    if (cpu.core.op == 0x76 && (cpu.imask_latch & cpu.intf_halt_latch)) cpu.core.state_ = 0;
    cpu.intf_halt_latch = 0;
  }

  // +ha -ab -bc -cd -de -ef -fg -gh
  if (DELTA_HA) {
    // this one latches funny, some hardware bug
    if (bit(reg.reg_if.NYBO_FF0F_D2p.state)) cpu.intf_halt_latch |= INT_TIMER_MASK;
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
    if (bit(reg.reg_if.LOPE_FF0F_D0p.state)) cpu.intf_halt_latch |= INT_VBLANK_MASK;
    if (bit(reg.reg_if.LALU_FF0F_D1p.state)) cpu.intf_halt_latch |= INT_STAT_MASK;
    if (bit(reg.reg_if.UBUL_FF0F_D3p.state)) cpu.intf_halt_latch |= INT_SERIAL_MASK;
    if (bit(reg.reg_if.ULAK_FF0F_D4p.state)) cpu.intf_halt_latch |= INT_JOYPAD_MASK;
  }

  // -ha -ab -bc -cd -de -ef +fg +gh
  if (DELTA_GH) {
    cpu.cpu_data_latch = 0xFF;
  }

  // +ha -ab -bc -cd -de -ef -fg +gh
  if (DELTA_GH) {
    cpu.intf_latch = (uint8_t)bit_pack(reg.reg_if);
  }
}

//------------------------------------------------------------------------------------------------------------------------































































//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_gates(const blob& cart_blob) {

  //----------

  /*#p27.SYLO*/ wire SYLO_WIN_HITn_old = not1(reg.win_ctrl.RYDY_WIN_HITp.out_old());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp_old = not1(SYLO_WIN_HITn_old);

  //----------

  wire EXT_vcc = 1;
  wire EXT_gnd = 0;

  /*_SIG_VCC*/ reg.SIG_VCC.sig_in(EXT_vcc);
  /*_SIG_GND*/ reg.SIG_GND.sig_in(EXT_gnd);

  //-----------------------------------------------------------------------------

  memset(&reg.cpu_abus_new, BIT_NEW | BIT_PULLED | 1, sizeof(reg.cpu_abus_new));
  memset(&reg.cpu_dbus_new, BIT_NEW | BIT_PULLED | 1, sizeof(reg.cpu_dbus_new));

  //-----------------------------------------------------------------------------

  {
    bool EXT_cpu_latch_ext;

    if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      // Data has to be driven on EFGH or we fail the wave tests
      reg.cpu_dbus_new.set_data(cpu.bus_req_new.write, cpu.bus_req_new.data_lo);
      EXT_cpu_latch_ext = cpu.bus_req_new.read;
    }
    else {
      reg.cpu_dbus_new.set_data(false, 0);
      EXT_cpu_latch_ext = 0;
    }
    /*_SIG_IN_CPU_LATCH_EXT*/ reg.cpu_signals.SIG_IN_CPU_LATCH_EXT.sig_in(EXT_cpu_latch_ext);

    bool EXT_addr_new = (cpu.bus_req_new.read || cpu.bus_req_new.write);
    bool in_bootrom = bit(~reg.cpu_signals.TEPU_BOOT_BITn.qp_old());
    bool addr_boot = (cpu.bus_req_new.addr <= 0x00FF) && in_bootrom;
    bool addr_vram = (cpu.bus_req_new.addr >= 0x8000) && (cpu.bus_req_new.addr < 0x9FFF);
    bool addr_high = (cpu.bus_req_new.addr >= 0xFE00);

    bool EXT_cpu_rd;
    bool EXT_cpu_wr;

    if (DELTA_HA) {
      EXT_cpu_rd = 0;
      EXT_cpu_wr = 0;
      reg.cpu_abus_new.set_addr(cpu.bus_req_new.addr & 0x00FF);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
      if (addr_vram) EXT_addr_new = false;
    }
    else {
      EXT_cpu_rd = cpu.bus_req_new.read;
      EXT_cpu_wr = cpu.bus_req_new.write;
      reg.cpu_abus_new.set_addr(cpu.bus_req_new.addr);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
    }

    /*_SIG_IN_CPU_RDp*/ reg.cpu_signals.SIG_IN_CPU_RDp.sig_in(EXT_cpu_rd);
    /*_SIG_IN_CPU_WRp*/ reg.cpu_signals.SIG_IN_CPU_WRp.sig_in(EXT_cpu_wr);
    /*_SIG_IN_CPU_EXT_BUSp*/ reg.cpu_signals.SIG_IN_CPU_EXT_BUSp.sig_in(EXT_addr_new);
  }

  //-----------------------------------------------------------------------------

  {
    wire EXT_sys_clkreq = bit(sys.clkreq);
    wire EXT_sys_rst = bit(~sys.rst);
    wire EXT_sys_t2 = bit(~sys.t2);
    wire EXT_sys_t1 = bit(~sys.t1);

    wire EXT_clkin = !(sys.phase_total & 1) && sys.clken;
    wire EXT_clkgood = bit(~sys.clkgood);

    /*_PIN_74*/ reg.clk.PIN_74_CLK.pin_clk(EXT_clkin, EXT_clkgood);
    /*_PIN_71*/ reg.rst.PIN_71_RST.pin_in(EXT_sys_rst);
    /*_PIN_76*/ reg.rst.PIN_76_T2.pin_in(EXT_sys_t2);
    /*_PIN_77*/ reg.rst.PIN_77_T1.pin_in(EXT_sys_t1);

    wire EXT_ack_vblank = get_bit(cpu.core.int_ack, BIT_VBLANK);
    wire EXT_ack_stat = get_bit(cpu.core.int_ack, BIT_STAT);
    wire EXT_ack_timer = get_bit(cpu.core.int_ack, BIT_TIMER);
    wire EXT_ack_serial = get_bit(cpu.core.int_ack, BIT_SERIAL);
    wire EXT_ack_joypad = get_bit(cpu.core.int_ack, BIT_JOYPAD);

    /*_SIG_CPU_ACK_VBLANK*/ reg.cpu_ack.SIG_CPU_ACK_VBLANK.sig_in(EXT_ack_vblank);
    /*_SIG_CPU_ACK_STAT  */ reg.cpu_ack.SIG_CPU_ACK_STAT.sig_in(EXT_ack_stat);
    /*_SIG_CPU_ACK_TIMER */ reg.cpu_ack.SIG_CPU_ACK_TIMER.sig_in(EXT_ack_timer);
    /*_SIG_CPU_ACK_SERIAL*/ reg.cpu_ack.SIG_CPU_ACK_SERIAL.sig_in(EXT_ack_serial);
    /*_SIG_CPU_ACK_JOYPAD*/ reg.cpu_ack.SIG_CPU_ACK_JOYPAD.sig_in(EXT_ack_joypad);

    /*_SIG_CPU_CLKREQ*/ reg.clk.SIG_CPU_CLKREQ.sig_in(EXT_sys_clkreq);

    /*_SIG_CPU_ADDR_HIp*/ reg.cpu_signals.SIG_CPU_ADDR_HIp.sig_out(reg.cpu_abus_new.SYRO_FE00_FFFF());
    /*_SIG_CPU_UNOR_DBG*/ reg.cpu_signals.SIG_CPU_UNOR_DBG.sig_out(UNOR_MODE_DBG2p());
    /*_SIG_CPU_UMUT_DBG*/ reg.cpu_signals.SIG_CPU_UMUT_DBG.sig_out(UMUT_MODE_DBG1p());
  }

  //-----------------------------------------------------------------------------
  // Sys clock signals

  tock_clocks_gates();

  /*_p07.UJYV*/ wire UJYV_CPU_RDn = not1(reg.cpu_signals.SIG_IN_CPU_RDp.out_new());
  /*_p07.TEDO*/ reg.cpu_signals.TEDO_CPU_RDp = not1(UJYV_CPU_RDn);

  /*#p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(ADAR_ABCxxxxH(), ATYP_ABCDxxxx());
  /*_p01.AREV*/ wire AREV_CPU_WRn = nand2(reg.cpu_signals.SIG_IN_CPU_WRp.out_new(), AFAS_xxxxEFGx);
  /*_p01.APOV*/ reg.cpu_signals.APOV_CPU_WRp = not1(AREV_CPU_WRn);

  /*_p07.UBAL*/ wire UBAL_CPU_WRn = not1(reg.cpu_signals.APOV_CPU_WRp.out_new());
  /*_p07.TAPU*/ reg.cpu_signals.TAPU_CPU_WRp = not1(UBAL_CPU_WRn); // xxxxEFGx

  /*#p01.AGUT*/ wire AGUT_xxCDEFGH = or_and3(AROV_xxCDEFxx(), AJAX_xxxxEFGH(), reg.cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new());
  /*#p01.AWOD*/ wire AWOD_ABxxxxxx = nor2(UNOR_MODE_DBG2p(), AGUT_xxCDEFGH);
  /*#p01.ABUZ*/ reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK = not1(AWOD_ABxxxxxx);

  tock_div_gates();
  tock_reset_gates(bit(sys.fastboot) ? reg.div.TERO_DIV03p : reg.div.UPOF_DIV15p);
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


  /*#p21.PARU*/ wire PARU_VBLANKp = not1(reg.lcd.POPU_y144p.qn_new());
  /*_p27.REPU*/ wire REPU_VBLANKp = or2(PARU_VBLANKp, PYRY_VID_RSTp());

  /*_p21.TADY*/ wire TADY_LINE_RSTn = nor2(reg.ATEJ_LINE_RSTp.out_new(), TOFU_VID_RSTp());
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(reg.ATEJ_LINE_RSTp.out_new(), ATAR_VID_RSTp());
  /*#p29.BALU*/ wire BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn = not1(BALU_LINE_RSTp);
  /*_p27.XAHY*/ wire XAHY_LINE_RSTn = not1(reg.ATEJ_LINE_RSTp.out_new());

  //----------------------------------------

  //----------------------------------------
  // Sprite scanner

  {
    /*#p29.DOBA*/ reg.sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(ALET_xBxDxFxH(), BAGY_LINE_RSTn, reg.sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
    /*#p29.BYBA*/ reg.sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(XUPY_ABxxEFxx(), BAGY_LINE_RSTn, reg.sprite_scanner.FETO_SCAN_DONEp.out_old());
    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(reg.sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp, reg.sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);

    /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(ATAR_VID_RSTp(), reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new());
    /*#p29.CENO*/ reg.sprite_scanner.CENO_SCANNINGn.dff17(XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), reg.sprite_scanner.BESU_SCANNINGn.qp_old());
    /*#p28.BESU*/ reg.sprite_scanner.BESU_SCANNINGn.nor_latch(reg.lcd.CATU_x113p.qp_new(), ASEN_SCAN_DONE_TRIGp);

    /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(reg.dma_ctrl.MATU_DMA_RUNNINGp.qp_new());
    /*#p28.ACYL*/ reg.sprite_scanner.ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, reg.sprite_scanner.BESU_SCANNINGn.qp_new());

    // Sprite scanner grabs the sprite index off the _old_ oam address bus
    /*_p28.YFOT*/ wire YFOT_OAM_A2p_old = not1(reg.oam_abus.BUS_OAM_A02n.out_old());
    /*_p28.YFOC*/ wire YFOC_OAM_A3p_old = not1(reg.oam_abus.BUS_OAM_A03n.out_old());
    /*_p28.YVOM*/ wire YVOM_OAM_A4p_old = not1(reg.oam_abus.BUS_OAM_A04n.out_old());
    /*_p28.YMEV*/ wire YMEV_OAM_A5p_old = not1(reg.oam_abus.BUS_OAM_A05n.out_old());
    /*_p28.XEMU*/ wire XEMU_OAM_A6p_old = not1(reg.oam_abus.BUS_OAM_A06n.out_old());
    /*_p28.YZET*/ wire YZET_OAM_A7p_old = not1(reg.oam_abus.BUS_OAM_A07n.out_old());
    /*_p30.XADU*/ reg.sprite_index.XADU_SPRITE_IDX0p.dff13(WUDA_xxCDxxGH(), reg.SIG_VCC.out_new(), YFOT_OAM_A2p_old);
    /*_p30.XEDY*/ reg.sprite_index.XEDY_SPRITE_IDX1p.dff13(WUDA_xxCDxxGH(), reg.SIG_VCC.out_new(), YFOC_OAM_A3p_old);
    /*_p30.ZUZE*/ reg.sprite_index.ZUZE_SPRITE_IDX2p.dff13(WUDA_xxCDxxGH(), reg.SIG_VCC.out_new(), YVOM_OAM_A4p_old);
    /*_p30.XOBE*/ reg.sprite_index.XOBE_SPRITE_IDX3p.dff13(WUDA_xxCDxxGH(), reg.SIG_VCC.out_new(), YMEV_OAM_A5p_old);
    /*_p30.YDUF*/ reg.sprite_index.YDUF_SPRITE_IDX4p.dff13(WUDA_xxCDxxGH(), reg.SIG_VCC.out_new(), XEMU_OAM_A6p_old);
    /*_p30.XECU*/ reg.sprite_index.XECU_SPRITE_IDX5p.dff13(WUDA_xxCDxxGH(), reg.SIG_VCC.out_new(), YZET_OAM_A7p_old);

    // FIXME is this OK without the second assignment of FETO?

    for (int feedback = 0; feedback < 2; feedback++) {
      // 32 + 4 + 2 + 1 = 39
      /*#p28.FETO*/ reg.sprite_scanner.FETO_SCAN_DONEp = and4(reg.scan_counter.YFEL_SCAN0.qp_mid(), reg.scan_counter.WEWY_SCAN1.qp_mid(), reg.scan_counter.GOSO_SCAN2.qp_mid(), reg.scan_counter.FONY_SCAN5.qp_mid());

      /*#p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(reg.sprite_scanner.FETO_SCAN_DONEp.out_new(), XUPY_ABxxEFxx());

      /*#p28.YFEL*/ reg.scan_counter.YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp,                 ANOM_LINE_RSTn, reg.scan_counter.YFEL_SCAN0.qn_any());
      /*_p28.WEWY*/ reg.scan_counter.WEWY_SCAN1.dff17_any(reg.scan_counter.YFEL_SCAN0.qn_any(), ANOM_LINE_RSTn, reg.scan_counter.WEWY_SCAN1.qn_any());
      /*_p28.GOSO*/ reg.scan_counter.GOSO_SCAN2.dff17_any(reg.scan_counter.WEWY_SCAN1.qn_any(), ANOM_LINE_RSTn, reg.scan_counter.GOSO_SCAN2.qn_any());
      /*_p28.ELYN*/ reg.scan_counter.ELYN_SCAN3.dff17_any(reg.scan_counter.GOSO_SCAN2.qn_any(), ANOM_LINE_RSTn, reg.scan_counter.ELYN_SCAN3.qn_any());
      /*_p28.FAHA*/ reg.scan_counter.FAHA_SCAN4.dff17_any(reg.scan_counter.ELYN_SCAN3.qn_any(), ANOM_LINE_RSTn, reg.scan_counter.FAHA_SCAN4.qn_any());
      /*_p28.FONY*/ reg.scan_counter.FONY_SCAN5.dff17_any(reg.scan_counter.FAHA_SCAN4.qn_any(), ANOM_LINE_RSTn, reg.scan_counter.FONY_SCAN5.qn_any());
    }

    ///*#p28.FETO*/ sprite_scanner.FETO_SCAN_DONEp = and4(sprite_scanner.YFEL_SCAN0.qp_new(), sprite_scanner.WEWY_SCAN1.qp_new(), sprite_scanner.GOSO_SCAN2.qp_new(), sprite_scanner.FONY_SCAN5.qp_new());
  }


  //----------------------------------------
  // Global rendering flag 'XYMU'

  /*#p21.VOGA*/ reg.VOGA_HBLANKp.dff17(ALET_xBxDxFxH(), TADY_LINE_RSTn, reg.WODU_HBLANKp.out_old());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), reg.VOGA_HBLANKp.qp_new());
  /*#p21.XYMU*/ reg.XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new());
  /*_p24.LOBY*/ wire LOBY_RENDERINGn = not1(reg.XYMU_RENDERINGn.qn_new());
  /*#p27.PAHA*/ wire PAHA_RENDERINGn = not1(reg.XYMU_RENDERINGn.qn_new());
  /*_p29.TEPA*/ wire TEPA_RENDERINGp = not1(reg.XYMU_RENDERINGn.qn_new());

  //----------------------------------------
  // Sprite fetch sequencer

  {
    /*_p27.TUKU*/ wire TUKU_WIN_HITn_old = not1(TOMU_WIN_HITp_old);
    /*_p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_old = not1(reg.sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp_old());
    /*_p27.TEKY*/ wire TEKY_SFETCH_REQp_old = and4(reg.FEPO_STORE_MATCHp.out_old(), TUKU_WIN_HITn_old, reg.tile_fetcher.LYRY_BFETCH_DONEp.out_old(), SOWO_SFETCH_RUNNINGn_old);
    /*_p27.SUDA*/ reg.sprite_fetcher.SUDA_SFETCH_REQp.dff17(LAPE_AxCxExGx(), reg.SIG_VCC.out_new(), reg.sprite_fetcher.SOBU_SFETCH_REQp.qp_old());
    /*_p27.SOBU*/ reg.sprite_fetcher.SOBU_SFETCH_REQp.dff17(TAVA_xBxDxFxH(), reg.SIG_VCC.out_new(), TEKY_SFETCH_REQp_old);

    /*#p29.TYFO*/ reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.dff17(LAPE_AxCxExGx(), reg.SIG_VCC.out_new(), reg.sprite_fetcher.TOXE_SFETCH_S0p.qp_old());
    /*#p29.SEBA*/ reg.sprite_fetcher.SEBA_SFETCH_S1p_D5.dff17(LAPE_AxCxExGx(), reg.XYMU_RENDERINGn.qn_new(), reg.sprite_fetcher.VONU_SFETCH_S1p_D4.qp_old());
    /*#p29.VONU*/ reg.sprite_fetcher.VONU_SFETCH_S1p_D4.dff17(TAVA_xBxDxFxH(), reg.XYMU_RENDERINGn.qn_new(), reg.sprite_fetcher.TOBU_SFETCH_S1p_D2.qp_old());
    /*#p29.TOBU*/ reg.sprite_fetcher.TOBU_SFETCH_S1p_D2.dff17(TAVA_xBxDxFxH(), reg.XYMU_RENDERINGn.qn_new(), reg.sprite_fetcher.TULY_SFETCH_S1p.qp_old());

    /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(reg.sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), reg.sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
    /*#p27.SECA*/ wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), reg.ATEJ_LINE_RSTp.out_new());

    // Feedback loop
    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand2(reg.sprite_fetcher.TESE_SFETCH_S2p.qp_any(), reg.sprite_fetcher.TOXE_SFETCH_S0p.qp_any());
      /*#p29.TOMA*/ wire TOMA_SFETCH_xBxDxFxH = nand2(LAPE_AxCxExGx(), TAME_SFETCH_CLK_GATE);
      /*#p29.TOXE*/ reg.sprite_fetcher.TOXE_SFETCH_S0p.dff17_any(TOMA_SFETCH_xBxDxFxH, SECA_SFETCH_STARTn, reg.sprite_fetcher.TOXE_SFETCH_S0p.qn_any());
      /*#p29.TULY*/ reg.sprite_fetcher.TULY_SFETCH_S1p.dff17_any(reg.sprite_fetcher.TOXE_SFETCH_S0p.qn_any(), SECA_SFETCH_STARTn, reg.sprite_fetcher.TULY_SFETCH_S1p.qn_any());
      /*#p29.TESE*/ reg.sprite_fetcher.TESE_SFETCH_S2p.dff17_any(reg.sprite_fetcher.TULY_SFETCH_S1p.qn_any(), SECA_SFETCH_STARTn, reg.sprite_fetcher.TESE_SFETCH_S2p.qn_any());
    }

    /*_p29.SAKY*/ wire SAKY_SFETCHn = nor2(reg.sprite_fetcher.TULY_SFETCH_S1p.qp_new(), reg.sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /*_p29.TYSO*/ wire TYSO_SFETCHINGn = or2(SAKY_SFETCHn, TEPA_RENDERINGp); // def or
    /*_p29.TEXY*/ reg.sprite_fetcher.TEXY_SFETCHINGp = not1(TYSO_SFETCHINGn);

    /*_p29.TYNO*/ wire TYNO = nand3(reg.sprite_fetcher.TOXE_SFETCH_S0p.qp_new(), reg.sprite_fetcher.SEBA_SFETCH_S1p_D5.qp_new(), reg.sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /*_p29.VUSA*/ wire VUSA_SPRITE_DONEn = or2(reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.qn_new(), TYNO);
    /*_p29.WUTY*/ reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp = not1(VUSA_SPRITE_DONEn);
  }

  /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(reg.sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), reg.sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
  /*#p27.SECA*/ wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), reg.ATEJ_LINE_RSTp.out_new());

  //----------------------------------------
  // Window state has some interaction with the tile fetcher here.

  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg.reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn, XAPO_VID_RSTn());
  /*_p27.NUNU*/ reg.win_ctrl.NUNU_WIN_MATCHp.dff17(MEHE_AxCxExGx(), XAPO_VID_RSTn(), reg.win_ctrl.PYCO_WIN_MATCHp.qp_old());
  /*_p27.NOPA*/ reg.win_ctrl.NOPA_WIN_MODE_Bp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), reg.win_ctrl.PYNU_WIN_MODE_Ap.qp_old());
  /*_p27.PYNU*/ reg.win_ctrl.PYNU_WIN_MODE_Ap.nor_latch(reg.win_ctrl.NUNU_WIN_MATCHp.qp_new(), XOFO_WIN_RSTp);

  /*#p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and2(reg.win_ctrl.PYNU_WIN_MODE_Ap.qp_new(), reg.win_ctrl.NOPA_WIN_MODE_Bp.qn_new());
  /*_p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not1(NUNY_WIN_MODE_TRIGp);
  /*_p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not1(NYFO_WIN_MODE_TRIGn);
  /*_p24.NAFY*/ wire NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);

  /*_p24.PYGO*/ reg.tile_fetcher.PYGO_FETCH_DONEp.dff17(ALET_xBxDxFxH(), reg.XYMU_RENDERINGn.qn_new(), reg.tile_fetcher.PORY_FETCH_DONEp.qp_old());
  /*_p24.PORY*/ reg.tile_fetcher.PORY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NAFY_WIN_MODE_TRIGn, reg.tile_fetcher.NYKA_FETCH_DONEp.qp_old());
  /*_p24.NYKA*/ reg.tile_fetcher.NYKA_FETCH_DONEp.dff17(ALET_xBxDxFxH(), NAFY_WIN_MODE_TRIGn, reg.tile_fetcher.LYRY_BFETCH_DONEp.out_old());
  /*_p24.POKY*/ reg.tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(reg.tile_fetcher.PYGO_FETCH_DONEp.qp_new(), LOBY_RENDERINGn);

  /*_p27.SOVY*/ reg.win_ctrl.SOVY_WIN_HITp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), reg.win_ctrl.RYDY_WIN_HITp.out_old());
  /*_p27.RYDY*/ reg.win_ctrl.RYDY_WIN_HITp = nor3(reg.win_ctrl.PUKU_WIN_HITn.out_mid(), reg.tile_fetcher.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ reg.win_ctrl.PUKU_WIN_HITn = nor2(reg.win_ctrl.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);
  /*_p27.RYDY*/ reg.win_ctrl.RYDY_WIN_HITp = nor3(reg.win_ctrl.PUKU_WIN_HITn.out_mid(), reg.tile_fetcher.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ reg.win_ctrl.PUKU_WIN_HITn = nor2(reg.win_ctrl.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);

  /*#p27.SYLO*/ wire SYLO_WIN_HITn = not1(reg.win_ctrl.RYDY_WIN_HITp.out_new());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp = not1(SYLO_WIN_HITn);
  /*#p24.SOCY*/ wire SOCY_WIN_HITn = not1(TOMU_WIN_HITp);

  /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn = not1(reg.tile_fetcher.POKY_PRELOAD_LATCHp.qp_new());
  /*_p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn = nand4(reg.XYMU_RENDERINGn.qn_new(), ROMO_PRELOAD_DONEn, reg.tile_fetcher.NYKA_FETCH_DONEp.qp_new(), reg.tile_fetcher.PORY_FETCH_DONEp.qp_new());
  /*_p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp = not1(SUVU_PRELOAD_DONE_TRIGn);

  /*_p27.VEKU*/ wire VEKU_SFETCH_ENDn = nor2(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), TAVE_PRELOAD_DONE_TRIGp); // def nor
  /*_p27.TAKA*/ reg.sprite_fetcher.TAKA_SFETCH_RUNNINGp.nand_latch(SECA_SFETCH_STARTn, VEKU_SFETCH_ENDn);

  //----------------------------------------
  // OAM temp register B stores sprite X coordinate during scan, so we need a copy of it for later.

  auto oam_temp_b_old = reg.oam_temp_b;

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  {
    /*_p25.CUFE*/ wire CUFE_OAM_CLKp = not_or_and3(reg.cpu_abus_new.SARO_ADDR_OAMp(), reg.dma_ctrl.MATU_DMA_RUNNINGp.qp_new(), MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
    /*_p25.AVER*/ wire AVER_AxxxExxx = nand2(reg.sprite_scanner.ACYL_SCANNINGp.out_new(), XYSO_xBCDxFGH());
    /*_p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(reg.sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
    /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n);
    /*_p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(TEPA_RENDERINGp, reg.sprite_fetcher.TULY_SFETCH_S1p.qp_new(), reg.sprite_fetcher.TESE_SFETCH_S2p.qp_new());
    /*_p25.VAPE*/ wire VAPE_OAM_CLKENn = and2(TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
    /*_p25.XUJY*/ wire XUJY_OAM_CLKENp = not1(VAPE_OAM_CLKENn);
    /*_p25.BYCU*/ wire BYCU_OAM_CLKp = nand3(AVER_AxxxExxx, XUJY_OAM_CLKENp, CUFE_OAM_CLKp);
    /*_p25.COTA*/ wire COTA_OAM_CLKn = not1(BYCU_OAM_CLKp);
    oam_latch_to_temp_a(COTA_OAM_CLKn, reg.oam_latch_a, reg.oam_temp_a);
    oam_latch_to_temp_b(COTA_OAM_CLKn, reg.oam_latch_b, reg.oam_temp_b);
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  SpriteDeltaY sprite_delta_y = sub_sprite_y_gates();

  {
    /*_p29.GACE*/ wire GACE_SPRITE_DELTA4 = not1(sprite_delta_y.GOPU_YDIFF4.sum);
    /*_p29.GUVU*/ wire GUVU_SPRITE_DELTA5 = not1(sprite_delta_y.FUWA_YDIFF5.sum);
    /*_p29.GYDA*/ wire GYDA_SPRITE_DELTA6 = not1(sprite_delta_y.GOJU_YDIFF6.sum);
    /*_p29.GEWY*/ wire GEWY_SPRITE_DELTA7 = not1(sprite_delta_y.WUHU_YDIFF7.sum);

    /*#p29.GOVU*/ wire GOVU_SPSIZE_MATCH = or2(reg.reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new(), sprite_delta_y.GYKY_YDIFF3.sum);
    /*_p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, sprite_delta_y.WUHU_YDIFF7.carry, GOVU_SPSIZE_MATCH);
    /*_p29.GESE*/ wire GESE_SCAN_MATCH_Yp = not1(WOTA_SCAN_MATCH_Yn);
    /*_p29.CEHA*/ wire CEHA_SCANNINGp = not1(reg.sprite_scanner.CENO_SCANNINGn.qn_new());
    /*_p29.CARE*/ wire CARE_COUNT_CLKn = and3(XOCE_xBCxxFGx(), CEHA_SCANNINGp, GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /*_p29.DYTY*/ wire DYTY_COUNT_CLKp = not1(CARE_COUNT_CLKn);
    /*_p29.DEZY*/ reg.DEZY_COUNT_CLKp.dff17_any(ZEME_AxCxExGx(), XAPO_VID_RSTn(), DYTY_COUNT_CLKp);

    // There's a feedback loop here, but we don't actually need to loop - BAKY holds the clock line high once the sprite store is full, so doing a second logic pass
    // doesn't actually change any of the dffs.
    /*#p29.BAKY*/ wire BAKY_SPRITES_FULL_new = and2(reg.sprite_counter.CUXY_SPRITE_COUNT1.qp_any(), reg.sprite_counter.DYBE_SPRITE_COUNT3.qp_any());
    /*#p29.CAKE*/ wire CAKE_COUNT_CLKp_new = or2(BAKY_SPRITES_FULL_new, reg.DEZY_COUNT_CLKp.qp_any());
    /*#p28.AZYB*/ wire AZYB_LINE_TRIGn = not1(reg.ATEJ_LINE_RSTp.out_new());
    /*_p29.BESE*/ reg.sprite_counter.BESE_SPRITE_COUNT0.dff17_any(CAKE_COUNT_CLKp_new, AZYB_LINE_TRIGn, reg.sprite_counter.BESE_SPRITE_COUNT0.qn_any());
    /*_p29.CUXY*/ reg.sprite_counter.CUXY_SPRITE_COUNT1.dff17_any(reg.sprite_counter.BESE_SPRITE_COUNT0.qn_any(), AZYB_LINE_TRIGn, reg.sprite_counter.CUXY_SPRITE_COUNT1.qn_any());
    /*_p29.BEGO*/ reg.sprite_counter.BEGO_SPRITE_COUNT2.dff17_any(reg.sprite_counter.CUXY_SPRITE_COUNT1.qn_any(), AZYB_LINE_TRIGn, reg.sprite_counter.BEGO_SPRITE_COUNT2.qn_any());
    /*_p29.DYBE*/ reg.sprite_counter.DYBE_SPRITE_COUNT3.dff17_any(reg.sprite_counter.BEGO_SPRITE_COUNT2.qn_any(), AZYB_LINE_TRIGn, reg.sprite_counter.DYBE_SPRITE_COUNT3.qn_any());

    /*_p28.ABAK*/ wire ABAK_LINE_RSTp = or2(reg.ATEJ_LINE_RSTp.out_new(), AMYG_VID_RSTp());
    /*_p28.BYVA*/ wire BYVA_LINE_RSTn = not1(ABAK_LINE_RSTp);

    //update_sprite_reset_flags(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp, BYVA_LINE_RSTn, sprite_match_flags, sprite_reset_flags);

    /*_p29.EBOJ*/ reg.sprite_reset_flags.EBOJ_STORE0_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.GUVA_SPRITE0_GETp.out_old());
    /*_p29.CEDY*/ reg.sprite_reset_flags.CEDY_STORE1_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.ENUT_SPRITE1_GETp.out_old());
    /*_p29.EGAV*/ reg.sprite_reset_flags.EGAV_STORE2_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.EMOL_SPRITE2_GETp.out_old());
    /*_p29.GOTA*/ reg.sprite_reset_flags.GOTA_STORE3_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.GYFY_SPRITE3_GETp.out_old());
    /*_p29.XUDY*/ reg.sprite_reset_flags.XUDY_STORE4_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.GONO_SPRITE4_GETp.out_old());
    /*_p29.WAFY*/ reg.sprite_reset_flags.WAFY_STORE5_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.GEGA_SPRITE5_GETp.out_old());
    /*_p29.WOMY*/ reg.sprite_reset_flags.WOMY_STORE6_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.XOJA_SPRITE6_GETp.out_old());
    /*_p29.WAPO*/ reg.sprite_reset_flags.WAPO_STORE7_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.GUTU_SPRITE7_GETp.out_old());
    /*_p29.EXUQ*/ reg.sprite_reset_flags.EXUQ_STORE8_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.FOXA_SPRITE8_GETp.out_old());
    /*_p29.FONO*/ reg.sprite_reset_flags.FONO_STORE9_RSTp.dff17(reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg.sprite_match_flags.GUZE_SPRITE9_GETp.out_old());

    SpriteStoreFlags sprite_store_flags_old = reg.sprite_store_flags;

    update_sprite_store_flags_gates(reg.sprite_counter, DYTY_COUNT_CLKp, reg.sprite_store_flags);
    store_sprite_gates(sprite_store_flags_old, reg.sprite_store_flags, reg.sprite_reset_flags, BYVA_LINE_RSTn, oam_temp_b_old);
  }

  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  auto XYDO_PX3p_old = reg.pix_count.XYDO_PX3p;

  // Weird feedback loop here - fine scroll and pix count affect store match, which affects pipe clock, which affects fine scroll and pix count.
  // Implementing the feedback doesn't seem to change any test results, but... paranoia...

  //for (int feedback = 0; feedback < 2; feedback++) {
  {
    /*_p27.SUHA*/ wire SUHA_SCX_FINE_MATCHp_old = xnor2(reg.reg_scx.DATY_SCX0n.qn_old(), reg.fine_scroll.RYKU_FINE_CNT0.qp_old());
    /*_p27.SYBY*/ wire SYBY_SCX_FINE_MATCHp_old = xnor2(reg.reg_scx.DUZU_SCX1n.qn_old(), reg.fine_scroll.ROGA_FINE_CNT1.qp_old());
    /*_p27.SOZU*/ wire SOZU_SCX_FINE_MATCHp_old = xnor2(reg.reg_scx.CYXU_SCX2n.qn_old(), reg.fine_scroll.RUBU_FINE_CNT2.qp_old());
    /*#p27.RONE*/ wire RONE_SCX_FINE_MATCHn_old = nand4(reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any(), SUHA_SCX_FINE_MATCHp_old, SYBY_SCX_FINE_MATCHp_old, SOZU_SCX_FINE_MATCHp_old);
    /*#p27.POHU*/ wire POHU_SCX_FINE_MATCHp_old = not1(RONE_SCX_FINE_MATCHn_old);

    /*_p29.CEHA*/ wire CEHA_SCANNINGp = not1(reg.sprite_scanner.CENO_SCANNINGn.qn_new());
    /*#p29.BYJO*/ wire BYJO_SCANNINGn = not1(CEHA_SCANNINGp);
    /*#p29.AZEM*/ wire AZEM_RENDERINGp = and2(reg.XYMU_RENDERINGn.qn_new(), BYJO_SCANNINGn);
    /*#p29.AROR*/ wire AROR_MATCH_ENp = and2(AZEM_RENDERINGp, reg.reg_lcdc.XYLO_LCDC_SPENn.qn_new());

    /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(reg.FEPO_STORE_MATCHp.out_mid(), reg.WODU_HBLANKp.out_old(), MYVO_AxCxExGx());
    /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, reg.tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
    /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);

    /*#p27.NYZE*/ reg.fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17_any(MOXE_AxCxExGx(), reg.XYMU_RENDERINGn.qn_new(), reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any());
    /*#p27.PUXA*/ reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17_any(ROXO_CLKPIPE_odd, reg.XYMU_RENDERINGn.qn_new(), POHU_SCX_FINE_MATCHp_old);

    /*#p27.POVA*/ wire POVA_FINE_MATCH_TRIGp = and2(reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any(), reg.fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_any());
    /*#p27.ROXY*/ reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(PAHA_RENDERINGn, POVA_FINE_MATCH_TRIGp);

    /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any());
    // Pixel counter, has carry lookahead because this can increment every tcycle
    /*_p21.RYBO*/ wire RYBO_old = xor2(reg.pix_count.XEHO_PX0p.qp_any(), reg.pix_count.SAVY_PX1p.qp_any()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /*_p21.XUKE*/ wire XUKE_old = and2(reg.pix_count.XEHO_PX0p.qp_any(), reg.pix_count.SAVY_PX1p.qp_any());
    /*_p21.XYLE*/ wire XYLE_old = and2(reg.pix_count.XODU_PX2p.qp_any(), XUKE_old);
    /*_p21.XEGY*/ wire XEGY_old = xor2(reg.pix_count.XODU_PX2p.qp_any(), XUKE_old); // feet facing gnd
    /*_p21.XORA*/ wire XORA_old = xor2(reg.pix_count.XYDO_PX3p.qp_any(), XYLE_old); // feet facing gnd

    /*_p21.XEHO*/ reg.pix_count.XEHO_PX0p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, reg.pix_count.XEHO_PX0p.qn_any());
    /*_p21.SAVY*/ reg.pix_count.SAVY_PX1p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, RYBO_old);
    /*_p21.XODU*/ reg.pix_count.XODU_PX2p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEGY_old);
    /*_p21.XYDO*/ reg.pix_count.XYDO_PX3p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XORA_old);

    /*_p24.TOCA*/ wire TOCA_new = not1(reg.pix_count.XYDO_PX3p.qp_any());
    /*_p21.SAKE*/ wire SAKE_old = xor2(reg.pix_count.TUHU_PX4p.qp_any(), reg.pix_count.TUKY_PX5p.qp_any());
    /*_p21.TYBA*/ wire TYBA_old = and2(reg.pix_count.TUHU_PX4p.qp_any(), reg.pix_count.TUKY_PX5p.qp_any());
    /*_p21.SURY*/ wire SURY_old = and2(reg.pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /*_p21.TYGE*/ wire TYGE_old = xor2(reg.pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /*_p21.ROKU*/ wire ROKU_old = xor2(reg.pix_count.SYBE_PX7p.qp_any(), SURY_old);

    /*_p21.TUHU*/ reg.pix_count.TUHU_PX4p.dff17_any(TOCA_new, TADY_LINE_RSTn, reg.pix_count.TUHU_PX4p.qn_any());
    /*_p21.TUKY*/ reg.pix_count.TUKY_PX5p.dff17_any(TOCA_new, TADY_LINE_RSTn, SAKE_old);
    /*_p21.TAKO*/ reg.pix_count.TAKO_PX6p.dff17_any(TOCA_new, TADY_LINE_RSTn, TYGE_old);
    /*_p21.SYBE*/ reg.pix_count.SYBE_PX7p.dff17_any(TOCA_new, TADY_LINE_RSTn, ROKU_old);

    get_sprite_match_flags_gates(AROR_MATCH_ENp, reg.sprite_match_flags);
  }

  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  {
    /*#p21.XENA*/ wire XENA_STORE_MATCHn = not1(reg.FEPO_STORE_MATCHp.out_new());
    /*#p21.XUGU*/ wire XUGU_PX167n = nand5(reg.pix_count.XEHO_PX0p.qp_new(), reg.pix_count.SAVY_PX1p.qp_new(), reg.pix_count.XODU_PX2p.qp_new(), reg.pix_count.TUKY_PX5p.qp_new(), reg.pix_count.SYBE_PX7p.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire XANO_PX167p = not1(XUGU_PX167n);
    /*#p21.WODU*/ reg.WODU_HBLANKp = and2(XENA_STORE_MATCHn, XANO_PX167p); // WODU goes high on odd, cleared on H
  }

  /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(reg.FEPO_STORE_MATCHp.out_new(), reg.WODU_HBLANKp.out_new(), MYVO_AxCxExGx());
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, reg.tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);
  /*#p24.SACU*/ wire SACU_CLKPIPE_new = or2(SEGU_CLKPIPE_evn, reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  /*_p24.PAHO*/ reg.lcd.PAHO_X_8_SYNC.dff17(ROXO_CLKPIPE_odd, reg.XYMU_RENDERINGn.qn_new(), XYDO_PX3p_old.qp_old());

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  memset(&reg.sprite_ibus, BIT_NEW | BIT_PULLED | 1, sizeof(reg.sprite_ibus));
  memset(&reg.sprite_lbus, BIT_NEW | BIT_PULLED | 1, sizeof(reg.sprite_lbus));

  sprite_match_to_bus_gates(reg.sprite_match_flags);
  sprite_scan_to_bus_gates(sprite_delta_y, reg.XYMU_RENDERINGn, reg.FEPO_STORE_MATCHp);

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // WY/WX/window match

  tock_window_gates(SEGU_CLKPIPE_evn, REPU_VBLANKp);

  //----------------------------------------
  // Tile fetch sequencer

  /*_p27.LYZU*/ reg.tile_fetcher.LYZU_BFETCH_S0p_D1.dff17(ALET_xBxDxFxH(), reg.XYMU_RENDERINGn.qn_new(), reg.tile_fetcher.LAXU_BFETCH_S0p.qp_old());

  /*_p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp = nor2(reg.win_ctrl.RYFA_WIN_FETCHn_A.qn_new(), reg.win_ctrl.RENE_WIN_FETCHn_B.qp_new());
  /*_p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne = nand2(SYLO_WIN_HITn, reg.win_ctrl.SOVY_WIN_HITp.qp_new());
  /*_p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne = not1(TUXY_WIN_FIRST_TILEne);
  /*_p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp = or3(SEKO_WIN_FETCH_TRIGp, SUZU_WIN_FIRST_TILEne, TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  /*_p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new(), MOSU_WIN_MODE_TRIGp, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(reg.tile_fetcher.LAXU_BFETCH_S0p.qp_any(), reg.tile_fetcher.NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn);
    /*_p27.LEBO*/ wire LEBO_AxCxExGx = nand2(ALET_xBxDxFxH(), MOCE_BFETCH_DONEn);

    /*_p27.LAXU*/ reg.tile_fetcher.LAXU_BFETCH_S0p.dff17_any(LEBO_AxCxExGx, NYXU_BFETCH_RSTn, reg.tile_fetcher.LAXU_BFETCH_S0p.qn_any());
    /*_p27.MESU*/ reg.tile_fetcher.MESU_BFETCH_S1p.dff17_any(reg.tile_fetcher.LAXU_BFETCH_S0p.qn_any(), NYXU_BFETCH_RSTn, reg.tile_fetcher.MESU_BFETCH_S1p.qn_any());
    /*_p27.NYVA*/ reg.tile_fetcher.NYVA_BFETCH_S2p.dff17_any(reg.tile_fetcher.MESU_BFETCH_S1p.qn_any(), NYXU_BFETCH_RSTn, reg.tile_fetcher.NYVA_BFETCH_S2p.qn_any());
  }

  /*_p27.LOVY*/ reg.tile_fetcher.LOVY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, reg.tile_fetcher.LYRY_BFETCH_DONEp.out_old());

  /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(reg.tile_fetcher.LAXU_BFETCH_S0p.qp_new(), reg.tile_fetcher.NYVA_BFETCH_S2p.qp_new(), NYXU_BFETCH_RSTn);
  /*_p27.LYRY*/ reg.tile_fetcher.LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  /*_p27.LURY*/ wire LURY_BG_FETCH_DONEn = and2(reg.tile_fetcher.LOVY_FETCH_DONEp.qn_new(), reg.XYMU_RENDERINGn.qn_new());
  /*_p27.LONY*/ reg.tile_fetcher.LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, LURY_BG_FETCH_DONEn);


  //----------------------------------------
  // Fine match counter

  /*#p27.PASO*/ wire PASO_FINE_RST = nor2(PAHA_RENDERINGn, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p27.ROZE*/ wire ROZE_FINE_COUNT_7n = nand3(reg.fine_scroll.RUBU_FINE_CNT2.qp_any(), reg.fine_scroll.ROGA_FINE_CNT1.qp_any(), reg.fine_scroll.RYKU_FINE_CNT0.qp_any());
    /*#p27.PECU*/ wire PECU_FINE_CLK = nand2(ROXO_CLKPIPE_odd, ROZE_FINE_COUNT_7n);
    /*#p27.RYKU*/ reg.fine_scroll.RYKU_FINE_CNT0.dff17_any(PECU_FINE_CLK, PASO_FINE_RST, reg.fine_scroll.RYKU_FINE_CNT0.qn_any());
    /*#p27.ROGA*/ reg.fine_scroll.ROGA_FINE_CNT1.dff17_any(reg.fine_scroll.RYKU_FINE_CNT0.qn_any(), PASO_FINE_RST, reg.fine_scroll.ROGA_FINE_CNT1.qn_any());
    /*#p27.RUBU*/ reg.fine_scroll.RUBU_FINE_CNT2.dff17_any(reg.fine_scroll.ROGA_FINE_CNT1.qn_any(), PASO_FINE_RST, reg.fine_scroll.RUBU_FINE_CNT2.qn_any());
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

  reg.cpu_abus_old = reg.cpu_abus_new;
  reg.cpu_dbus_old = reg.cpu_dbus_new;
  commit();
}





















































































































//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_logic(const blob& cart_blob) {
  GateBoyReg  reg_old = reg;
  GateBoyReg& reg_new = reg;


  uint8_t phase_old = 1 << (7 - ((sys.phase_total + 0) & 7));
  uint8_t phase_new = 1 << (7 - ((sys.phase_total + 1) & 7));


  bool SYLO_WIN_HITn_old = !reg_old.win_ctrl.RYDY_WIN_HITp;
  bool TOMU_WIN_HITp_old = !SYLO_WIN_HITn_old;
  bool SOCY_WIN_HITn_old = !TOMU_WIN_HITp_old;
  bool VYBO_CLKPIPE_old = !reg_old.FEPO_STORE_MATCHp && !reg_old.WODU_HBLANKp &&  !(phase_old & 0b10101010);
  bool TYFA_CLKPIPE_old = SOCY_WIN_HITn_old && reg_old.tile_fetcher.POKY_PRELOAD_LATCHp && VYBO_CLKPIPE_old;
  bool SEGU_CLKPIPE_old = !TYFA_CLKPIPE_old;
  bool SACU_CLKPIPE_old = SEGU_CLKPIPE_old || reg_old.fine_scroll.ROXY_FINE_SCROLL_DONEn;
  bool ROCO_CLKPIPE_old = !SEGU_CLKPIPE_old;

  bool clkpipe_en_old = 1;
  if (reg_old.win_ctrl.RYDY_WIN_HITp) clkpipe_en_old = 0;
  if (!reg_old.tile_fetcher.POKY_PRELOAD_LATCHp) clkpipe_en_old = 0;
  if (reg_old.FEPO_STORE_MATCHp) clkpipe_en_old = 0;
  if (reg_old.WODU_HBLANKp) clkpipe_en_old = 0;

  bool CLKPIPE_old = phase_old & 0b10101010;
  if (!clkpipe_en_old) CLKPIPE_old = 1;
  if (reg_old.fine_scroll.ROXY_FINE_SCROLL_DONEn) CLKPIPE_old = 1;

  bool TEVO_WIN_FETCH_TRIGp_old = 0;
  if (reg_old.win_ctrl.RYFA_WIN_FETCHn_A && !reg_old.win_ctrl.RENE_WIN_FETCHn_B) TEVO_WIN_FETCH_TRIGp_old = 1;
  if (!reg_old.win_ctrl.RYDY_WIN_HITp && reg_old.win_ctrl.SOVY_WIN_HITp) TEVO_WIN_FETCH_TRIGp_old = 1;
  if (!reg_old.XYMU_RENDERINGn && !reg_old.tile_fetcher.POKY_PRELOAD_LATCHp && reg_old.tile_fetcher.NYKA_FETCH_DONEp && reg_old.tile_fetcher.PORY_FETCH_DONEp) TEVO_WIN_FETCH_TRIGp_old = 1;

  auto wodu_hblank_old = reg_old.WODU_HBLANKp.state;

  bool int_stat_old = 0;
  if (!reg_old.reg_stat.RUGU_STAT_LYI_ENn.state && reg_old.ROPO_LY_MATCH_SYNCp.state) int_stat_old = 1;
  if (!reg_old.reg_stat.REFE_STAT_OAI_ENn.state && !reg_old.lcd.POPU_y144p.state && reg_old.lcd.RUTU_x113p.state) int_stat_old = 1;
  if (!reg_old.reg_stat.RUFO_STAT_VBI_ENn.state && reg_old.lcd.POPU_y144p.state) int_stat_old = 1;
  if (!reg_old.reg_stat.ROXE_STAT_HBI_ENn.state && wodu_hblank_old && !reg_old.lcd.POPU_y144p.state) int_stat_old = 1;

  bool int_lcd_old = reg_old.lcd.POPU_y144p.state;
  bool int_joy_old = !reg_old.joy_int.APUG_JP_GLITCH3.state || !reg_old.joy_int.BATU_JP_GLITCH0.state;
  bool int_tim_old = reg_old.int_ctrl.MOBA_TIMER_OVERFLOWp.state;
  //wire int_ser_old = serial.CALY_SER_CNT3.state;
  bool int_ser_old = 0;


  bool UKAP_CLK_MUXa_old = reg_old.tac.SOPU_TAC0p.state ? reg_old.div.TAMA_DIV05p.state : reg_old.div.TERO_DIV03p.state;
  bool TEKO_CLK_MUXb_old = reg_old.tac.SOPU_TAC0p.state ? reg_old.div.UFOR_DIV01p.state : reg_old.div.TULU_DIV07p.state;
  bool TECY_CLK_MUXc_old = reg_old.tac.SAMY_TAC1p.state ? UKAP_CLK_MUXa_old : TEKO_CLK_MUXb_old;
  bool SOGU_TIMA_CLKn_old = TECY_CLK_MUXc_old && reg_old.tac.SABO_TAC2p.state;



  //----------

  reg_new.SIG_VCC.state = 1;
  reg_new.SIG_GND.state = 0;

  //-----------------------------------------------------------------------------

  uint16_t cpu_addr_old = (uint16_t)bit_pack(reg_old.cpu_abus_old);
  bool cpu_addr_vram_old = (cpu_addr_old >= 0x8000) && (cpu_addr_old <= 0x9FFF);
  bool cpu_addr_oam_old = (cpu_addr_old >= 0xFE00) && (cpu_addr_old <= 0xFEFF);
  uint8_t cpu_data_old = (uint8_t)bit_pack(reg_old.cpu_dbus_old);
  bool cpu_rd_old = reg_old.cpu_signals.SIG_IN_CPU_RDp;
  bool cpu_wr_old = reg_old.cpu_signals.SIG_IN_CPU_WRp;

  bit_set(reg_new.cpu_abus_new);
  bit_set(reg_new.cpu_dbus_new);

  if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
    // Data has to be driven on EFGH or we fail the wave tests
    if (cpu.bus_req_new.write) bit_unpack(reg_new.cpu_dbus_new, cpu.bus_req_new.data_lo);
    reg_new.cpu_signals.SIG_IN_CPU_LATCH_EXT.state = cpu.bus_req_new.read;
  }
  else {
    reg_new.cpu_signals.SIG_IN_CPU_LATCH_EXT.state = 0;
  }

  bool EXT_addr_new = (cpu.bus_req_new.read || cpu.bus_req_new.write);
  bool in_bootrom = !reg_new.cpu_signals.TEPU_BOOT_BITn.state;
  bool addr_boot = (cpu.bus_req_new.addr <= 0x00FF) && in_bootrom;
  bool addr_vram = (cpu.bus_req_new.addr >= 0x8000) && (cpu.bus_req_new.addr < 0x9FFF);
  bool addr_high = (cpu.bus_req_new.addr >= 0xFE00);

  bool EXT_cpu_rd;
  bool EXT_cpu_wr;

  if (DELTA_HA) {
    EXT_cpu_rd = 0;
    EXT_cpu_wr = 0;
    bit_unpack(reg_new.cpu_abus_new, cpu.bus_req_new.addr & 0x00FF);

    if (addr_high) EXT_addr_new = false;
    if (addr_boot) EXT_addr_new = false;
    if (addr_vram) EXT_addr_new = false;
  }
  else {
    EXT_cpu_rd = cpu.bus_req_new.read;
    EXT_cpu_wr = cpu.bus_req_new.write;
    bit_unpack(reg_new.cpu_abus_new, cpu.bus_req_new.addr);

    if (addr_high) EXT_addr_new = false;
    if (addr_boot) EXT_addr_new = false;
  }

  reg_new.cpu_signals.SIG_IN_CPU_RDp.state = EXT_cpu_rd;
  reg_new.cpu_signals.SIG_IN_CPU_WRp.state = EXT_cpu_wr;
  reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = EXT_addr_new;

  uint16_t cpu_addr_new = (uint16_t)bit_pack(reg_new.cpu_abus_new);

  bool cpu_addr_vram_new = (cpu_addr_new >= 0x8000) && (cpu_addr_new <= 0x9FFF);
  bool cpu_addr_ram_new = (cpu_addr_new >= 0xA000) && (cpu_addr_new <= 0xFDFF);
  bool cpu_addr_oam_new = (cpu_addr_new >= 0xFE00) && (cpu_addr_new <= 0xFEFF);
  uint8_t cpu_data_new = (uint8_t)bit_pack(reg_new.cpu_dbus_new);
  bool cpu_rd_new = reg_new.cpu_signals.SIG_IN_CPU_RDp;
  bool cpu_wr_new = reg_new.cpu_signals.SIG_IN_CPU_WRp;

  //-----------------------------------------------------------------------------

  reg_new.clk.PIN_74_CLK.CLK.state = sys.phase_total & 1;
  reg_new.clk.PIN_74_CLK.CLKGOOD.state = 1;

  reg_new.rst.PIN_71_RST = 0;
  reg_new.rst.PIN_76_T2 = 0;
  reg_new.rst.PIN_77_T1 = 0;

  reg_new.cpu_ack.SIG_CPU_ACK_VBLANK = get_bit(cpu.core.int_ack, BIT_VBLANK);
  reg_new.cpu_ack.SIG_CPU_ACK_STAT   = get_bit(cpu.core.int_ack, BIT_STAT);
  reg_new.cpu_ack.SIG_CPU_ACK_TIMER  = get_bit(cpu.core.int_ack, BIT_TIMER);
  reg_new.cpu_ack.SIG_CPU_ACK_SERIAL = get_bit(cpu.core.int_ack, BIT_SERIAL);
  reg_new.cpu_ack.SIG_CPU_ACK_JOYPAD = get_bit(cpu.core.int_ack, BIT_JOYPAD);

  reg_new.clk.SIG_CPU_CLKREQ.state = 1;

  reg_new.cpu_signals.SIG_CPU_ADDR_HIp.state = cpu_addr_new >= 0xFE00 && cpu_addr_new <= 0xFFFF;
  reg_new.cpu_signals.SIG_CPU_UNOR_DBG.state = 0;
  reg_new.cpu_signals.SIG_CPU_UMUT_DBG.state = 0;

  //-----------------------------------------------------------------------------
  // Sys clock signals

  reg_new.clk.PIN_73_CLK_DRIVE.state = reg_new.clk.PIN_74_CLK.CLK;
  reg_new.clk.AVET_DEGLITCH = reg_new.clk.PIN_74_CLK.CLK;
  reg_new.clk.ANOS_DEGLITCH = !reg_new.clk.PIN_74_CLK.CLK;

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

  reg_new.clk.AFUR_xxxxEFGH = CLK_xxxxEFGH;
  reg_new.clk.ALEF_AxxxxFGH = CLK_AxxxxFGH;
  reg_new.clk.APUK_ABxxxxGH = CLK_ABxxxxGH;
  reg_new.clk.ADYK_ABCxxxxH = CLK_ABCxxxxH;

  reg_new.clk.PIN_75_CLK_OUT.state = CLK_xxxxEFGH;

  reg_new.clk.SIG_CPU_BOWA_Axxxxxxx.state = !!(phase_new & 0b10000000);
  reg_new.clk.SIG_CPU_BEDO_xBCDEFGH.state = !!(phase_new & 0b01111111);
  reg_new.clk.SIG_CPU_BEKO_ABCDxxxx.state = !!(phase_new & 0b11110000);
  reg_new.clk.SIG_CPU_BUDE_xxxxEFGH.state = !!(phase_new & 0b00001111);
  reg_new.clk.SIG_CPU_BOLO_ABCDEFxx.state = !!(phase_new & 0b11111100);
  reg_new.clk.SIG_CPU_BUKE_AxxxxxGH.state = !!(phase_new & 0b10000011);
  reg_new.clk.SIG_CPU_BOMA_xBCDEFGH.state = !!(phase_new & 0b01111111);
  reg_new.clk.SIG_CPU_BOGA_Axxxxxxx.state = !!(phase_new & 0b10000000);

  reg_new.cpu_signals.TEDO_CPU_RDp = cpu_rd_new;
  reg_new.cpu_signals.APOV_CPU_WRp = CLK_xxxxEFGx && cpu_wr_new;
  reg_new.cpu_signals.TAPU_CPU_WRp = reg_new.cpu_signals.APOV_CPU_WRp;
  reg_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK = CLK_xxCDEFGH && reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp;

  //----------
  // DIV

  if (DELTA_HA) bit_unpack(reg_new.div, bit_pack(reg_new.div) + 1);
  if (cpu_addr_new == 0xFF04 && cpu_wr_new && CLK_xxxxEFGx) bit_unpack(reg_new.div, 0);
  if (cpu_addr_new == 0xFF04 && cpu_rd_new) bit_unpack(reg_new.cpu_dbus_new, bit_pack(reg_new.div) >> 6);

  //----------
  // In logic mode we don't care about the power-on behavior, we only want behavior to match when running code. So, we set
  // this stuff to zeroes.

  reg_new.rst.AFER_SYS_RSTp = 0;
  reg_new.rst.TUBO_WAITINGp = 0;
  reg_new.rst.ASOL_POR_DONEn = 0;
  reg_new.rst.SIG_CPU_EXT_CLKGOOD = 1;
  reg_new.rst.SIG_CPU_EXT_RESETp = 0;
  reg_new.rst.SIG_CPU_STARTp = 0;
  reg_new.rst.SIG_CPU_INT_RESETp = 0;;
  reg_new.rst.SOTO_DBG_VRAMp = 0;

  //----------
  // LCDC
  // has to be near the top as it controls the video reset signal

  wire vid_rst_old = reg_new.reg_lcdc.XONA_LCDC_LCDENn;
  wire winen_old = !reg_new.reg_lcdc.WYMO_LCDC_WINENn;
  if (cpu_wr_new && cpu_addr_new == 0xFF40 && DELTA_GH) {
    bit_copy_inv(reg_new.reg_lcdc, reg_new.cpu_dbus_old);
  }

  if (cpu_rd_new && (cpu_addr_new == 0xFF40)) {
    bit_copy_inv(reg_new.cpu_dbus_new, reg_new.reg_lcdc);
  }
  bool vid_rst_new = reg_new.reg_lcdc.XONA_LCDC_LCDENn;
  bool winen_new = !reg_new.reg_lcdc.WYMO_LCDC_WINENn;

  //----------
  // Video clocks

  reg_new.clk.WOSU_AxxDExxH = !vid_rst_new && gen_clk_new(0b10011001);
  reg_new.clk.WUVU_ABxxEFxx = !vid_rst_new && gen_clk_new(0b11001100);
  reg_new.clk.VENA_xxCDEFxx = !vid_rst_new && gen_clk_new(0b00111100);

  //----------
  // LYC

  if (cpu_addr_new == 0xFF45) {
    if (cpu_rd_new) bit_copy_inv(reg_new.cpu_dbus_new, reg_new.reg_lyc);
    if (cpu_wr_new && DELTA_GH) bit_copy_inv(reg_new.reg_lyc, reg_new.cpu_dbus_old);
  }

  if (!vid_rst_new && DELTA_BC) {
    auto ly = bit_pack(reg_new.reg_ly);
    auto lyc = bit_pack_inv(reg_new.reg_lyc);
    reg_new.ROPO_LY_MATCH_SYNCp = ly == lyc;
  }

  // FIXME this seems slightly wrong...
  if (cpu_wr_new && CLK_xxxxEFGx && cpu_addr_new == 0xFF41) {
  }
  else {
    reg_new.RUPO_LYC_MATCHn = 1;
  }

  // but the "reset" arm of the latch overrides the "set" arm, so it doesn't completely break?
  if (reg_new.ROPO_LY_MATCH_SYNCp) {
    reg_new.RUPO_LYC_MATCHn = 0;
  }

  //----------
  /// LX, LY, lcd flags

  bool rutu_x113p_old = reg_old.lcd.RUTU_x113p;
  bool popu_y144p_old = reg_old.lcd.POPU_y144p;
  bool nype_x113p_old = reg_old.lcd.NYPE_x113p;

  auto lx_old = bit_pack(reg_old.reg_lx);
  auto ly_old = bit_pack(reg_old.reg_ly);
  
  bool line_rst_old = reg_old.ATEJ_LINE_RSTp;

  if (vid_rst_new) {
    reg_new.lcd.ANEL_x113p = 0;
    reg_new.lcd.CATU_x113p = 0;
    reg_new.lcd.NYPE_x113p = 0;
    reg_new.lcd.RUTU_x113p = 0;
    reg_new.lcd.POPU_y144p = 0;
    reg_new.lcd.MYTA_y153p = 0;
    reg_new.lcd.SYGU_LINE_STROBE = 0;

    reg_new.ATEJ_LINE_RSTp = 1;

    bit_clear(reg_new.reg_lx);
    bit_clear(reg_new.reg_ly);
  }
  else {
    wire ly_144 = (ly_old & 144) == 144;
    wire ly_153 = (ly_old & 153) == 153;

    if (DELTA_HA) {
      reg_new.lcd.CATU_x113p = reg_new.lcd.RUTU_x113p.state && !ly_144;
    }

    if (DELTA_BC) {
      bool nype_old = reg_old.lcd.NYPE_x113p;

      reg_new.lcd.ANEL_x113p = reg_old.lcd.CATU_x113p;
      reg_new.lcd.NYPE_x113p = reg_old.lcd.RUTU_x113p;

      bool nype_new = reg_new.lcd.NYPE_x113p;
      if (!nype_old && nype_new) {
        reg_new.lcd.POPU_y144p = ly_144;
        reg_new.lcd.MYTA_y153p = ly_153;
      }

      bit_unpack(reg_new.reg_lx, lx_old + 1);
    }

    if (DELTA_DE) {
      reg_new.lcd.CATU_x113p = reg_new.lcd.RUTU_x113p.state && !ly_144;
    }

    if (DELTA_FG) {
      bool rutu_old = reg_old.lcd.RUTU_x113p;

      reg_new.lcd.ANEL_x113p = reg_old.lcd.CATU_x113p;
      reg_new.lcd.RUTU_x113p = lx_old == 113;

      bool rutu_new = reg_new.lcd.RUTU_x113p;
      if (!rutu_old && rutu_new) {
        bit_unpack(reg_new.reg_ly, ly_old + 1);
      }

      wire strobe = (lx_old == 0) || (lx_old == 7) || (lx_old == 45) || (lx_old == 83);
      reg_new.lcd.SYGU_LINE_STROBE = strobe;
    }

    reg_new.ATEJ_LINE_RSTp = !reg_new.lcd.ANEL_x113p && reg_new.lcd.CATU_x113p;
    if (reg_new.lcd.RUTU_x113p) bit_clear(reg_new.reg_lx);
    if (reg_new.lcd.MYTA_y153p) bit_clear(reg_new.reg_ly);
  }

  if (cpu_rd_new && (cpu_addr_new == 0xFF44)) {
    bit_copy(reg_new.cpu_dbus_new, reg_new.reg_ly);
  }

  bool rutu_x113p_new = reg_new.lcd.RUTU_x113p;
  bool popu_y144p_new = reg_new.lcd.POPU_y144p;
  bool nype_x113p_new = reg_new.lcd.NYPE_x113p;

  bool line_rst_new = reg_new.ATEJ_LINE_RSTp;

  //----------

  if (reg_new.cpu_signals.SIG_IN_CPU_WRp.state && cpu_addr_new == 0xFF00 && DELTA_GH) {
    reg_new.joy.KELY_JOYP_UDLRp.state = reg_new.cpu_dbus_old.BUS_CPU_D04p.state;
    reg_new.joy.COFY_JOYP_ABCSp.state = reg_new.cpu_dbus_old.BUS_CPU_D05p.state;
    reg_new.joy_ext.PIN_63_JOY_P14.state = !reg_new.joy.KELY_JOYP_UDLRp.state;
    reg_new.joy_ext.PIN_62_JOY_P15.state = !reg_new.joy.COFY_JOYP_ABCSp.state;
  }

  bool EXT_button0 = 0, EXT_button1 = 0, EXT_button2 = 0, EXT_button3 = 0;

  if (reg_new.joy_ext.PIN_63_JOY_P14.state) {
    EXT_button0 = get_bit(sys.buttons, 0); // RIGHT
    EXT_button1 = get_bit(sys.buttons, 1); // LEFT
    EXT_button2 = get_bit(sys.buttons, 2); // UP
    EXT_button3 = get_bit(sys.buttons, 3); // DOWN
  }
  else if (reg_new.joy_ext.PIN_62_JOY_P15.state) {
    EXT_button0 = get_bit(sys.buttons, 4); // A
    EXT_button1 = get_bit(sys.buttons, 5); // B
    EXT_button2 = get_bit(sys.buttons, 6); // SELECT
    EXT_button3 = get_bit(sys.buttons, 7); // START
  }

  reg_new.joy_ext.PIN_67_JOY_P10.state = EXT_button0;
  reg_new.joy_ext.PIN_66_JOY_P11.state = EXT_button1;
  reg_new.joy_ext.PIN_65_JOY_P12.state = EXT_button2;
  reg_new.joy_ext.PIN_64_JOY_P13.state = EXT_button3;

  wire any_button = EXT_button0 || EXT_button1 || EXT_button2 || EXT_button3;

  if (gen_clk_new(0b10000000)) {
    reg_new.int_ctrl.AWOB_WAKE_CPU.state = !any_button;
    reg_new.int_ctrl.SIG_CPU_WAKE.state = !any_button;
  }

  if (DELTA_HA) {
    reg_new.joy_int.APUG_JP_GLITCH3.state = reg_new.joy_int.AGEM_JP_GLITCH2.state;
    reg_new.joy_int.AGEM_JP_GLITCH2.state = reg_new.joy_int.ACEF_JP_GLITCH1.state;
    reg_new.joy_int.ACEF_JP_GLITCH1.state = reg_new.joy_int.BATU_JP_GLITCH0.state;
    reg_new.joy_int.BATU_JP_GLITCH0.state = !any_button;
  }

  if (cpu_addr_new == 0xFF00 && reg_new.cpu_signals.SIG_IN_CPU_RDp.state) {
    reg_new.cpu_dbus_new.BUS_CPU_D00p.state = !reg_new.joy_latch.KEVU_JOYP_L0n.state;
    reg_new.cpu_dbus_new.BUS_CPU_D01p.state = !reg_new.joy_latch.KAPA_JOYP_L1n.state;
    reg_new.cpu_dbus_new.BUS_CPU_D02p.state = !reg_new.joy_latch.KEJA_JOYP_L2n.state;
    reg_new.cpu_dbus_new.BUS_CPU_D03p.state = !reg_new.joy_latch.KOLO_JOYP_L3n.state;
    reg_new.cpu_dbus_new.BUS_CPU_D04p.state =  reg_new.joy.KELY_JOYP_UDLRp.state;
    reg_new.cpu_dbus_new.BUS_CPU_D05p.state =  reg_new.joy.COFY_JOYP_ABCSp.state;
  }
  else {
    reg_new.joy_latch.KEVU_JOYP_L0n.state = reg_new.joy_ext.PIN_67_JOY_P10.state;
    reg_new.joy_latch.KAPA_JOYP_L1n.state = reg_new.joy_ext.PIN_66_JOY_P11.state;
    reg_new.joy_latch.KEJA_JOYP_L2n.state = reg_new.joy_ext.PIN_65_JOY_P12.state;
    reg_new.joy_latch.KOLO_JOYP_L3n.state = reg_new.joy_ext.PIN_64_JOY_P13.state;
  }

  //tock_serial_logic();
  //tock_timer_logic();

  if (reg_new.cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
    if (cpu_addr_new == 0xFF06) bit_copy(reg_new.tma, reg_new.cpu_dbus_new);
    if (cpu_addr_new == 0xFF07) bit_copy(&reg_new.tac, 3, &reg_new.cpu_dbus_new);
  }

    
  if (!gen_clk_old(0b10000000) && gen_clk_new(0b10000000)) {
    wire MERY_TIMER_OVERFLOWp_old = !reg_new.tima.NUGA_TIMA7p.state && reg_new.int_ctrl.NYDU_TIMA7p_DELAY.state;
    reg_new.int_ctrl.MOBA_TIMER_OVERFLOWp.state = MERY_TIMER_OVERFLOWp_old;
  }

  wire TOPE_FF05_WRn = !CLK_xxxxEFGx || !reg_new.cpu_signals.SIG_IN_CPU_WRp.state || cpu_addr_new != 0xFF05;

  wire MUZU_CPU_LOAD_TIMAn = reg_new.cpu_signals.SIG_IN_CPU_LATCH_EXT.state || TOPE_FF05_WRn;
  wire MEXU_TIMA_LOADp = !MUZU_CPU_LOAD_TIMAn || reg_new.int_ctrl.MOBA_TIMER_OVERFLOWp.state;

  if (!gen_clk_old(0b10000000) && gen_clk_new(0b10000000)) {
    reg_new.int_ctrl.NYDU_TIMA7p_DELAY.state = reg_new.tima.NUGA_TIMA7p.state;
  }

  if (MEXU_TIMA_LOADp) {
    reg_new.int_ctrl.NYDU_TIMA7p_DELAY.state = 0;
  }

  wire UKAP_CLK_MUXa_new = reg_new.tac.SOPU_TAC0p.state ? reg_new.div.TAMA_DIV05p.state : reg_new.div.TERO_DIV03p.state;
  wire TEKO_CLK_MUXb_new = reg_new.tac.SOPU_TAC0p.state ? reg_new.div.UFOR_DIV01p.state : reg_new.div.TULU_DIV07p.state;
  wire TECY_CLK_MUXc_new = reg_new.tac.SAMY_TAC1p.state ? UKAP_CLK_MUXa_new : TEKO_CLK_MUXb_new;
  wire SOGU_TIMA_CLKn_new = TECY_CLK_MUXc_new && reg_new.tac.SABO_TAC2p.state;

  if (SOGU_TIMA_CLKn_old && !SOGU_TIMA_CLKn_new) {
    bit_unpack(reg_new.tima, bit_pack(reg_new.tima) + 1);
  }

  if (MEXU_TIMA_LOADp) {
    if (TOPE_FF05_WRn) {
      bit_copy(reg_new.tima, reg_new.tma);
    }
    else {
      bit_copy(reg_new.tima, reg_new.cpu_dbus_new);
    }
  }

  if (reg_new.cpu_signals.SIG_IN_CPU_RDp.state) {
    if (cpu_addr_new == 0xFF05) bit_copy(reg_new.cpu_dbus_new, reg_new.tima);
    if (cpu_addr_new == 0xFF06) bit_copy(reg_new.cpu_dbus_new, reg_new.tma);
    if (cpu_addr_new == 0xFF07) bit_copy(reg_new.cpu_dbus_new, reg_new.tac);
  }

  if (reg_new.cpu_signals.SIG_IN_CPU_WRp.state && cpu_addr_new == 0xFF50 && DELTA_GH) {
    reg_new.cpu_signals.TEPU_BOOT_BITn.state = reg_new.SATO_BOOT_BITn.state;
  }

  reg_new.cpu_signals.SIG_CPU_BOOTp.state = 0;
  reg_new.cpu_signals.SIG_BOOT_CSp.state = 0;

  if (cpu_addr_new <= 0x00FF) {

    reg_new.cpu_signals.SIG_CPU_BOOTp.state = !reg_new.cpu_signals.TEPU_BOOT_BITn.state;

    if (reg_new.cpu_signals.SIG_IN_CPU_RDp.state && !reg_new.cpu_signals.TEPU_BOOT_BITn.state) {
      reg_new.cpu_signals.SIG_BOOT_CSp.state = 1;
      bit_unpack(reg_new.cpu_dbus_new, DMG_ROM_blob[cpu_addr_new & 0xFF]);
    }
  }

  if (reg_new.cpu_signals.SIG_IN_CPU_RDp.state && (cpu_addr_new == 0xFF50)) {
    reg_new.cpu_dbus_new.BUS_CPU_D00p.state = reg_new.cpu_signals.TEPU_BOOT_BITn.state;
  }

  reg_new.SATO_BOOT_BITn = reg_new.cpu_dbus_new.BUS_CPU_D00p.state || reg_new.cpu_signals.TEPU_BOOT_BITn.state;

  auto dma_addr_old = (bit_pack_inv(reg_new.dma_hi) << 8) | bit_pack(reg_new.dma_lo);
  bool dma_running_old = reg_new.dma_ctrl.MATU_DMA_RUNNINGp;

  reg_new.dma_ctrl.LYXE_DMA_LATCHp.state |= cpu_addr_new == 0xFF46 && reg_new.cpu_signals.SIG_IN_CPU_WRp.state && CLK_xxxxEFGx;

  auto dma_lo_old = bit_pack(reg_old.dma_lo);

  if (DELTA_DE) {
    if (dma_lo_old == 159) {
      reg_new.dma_ctrl.MYTE_DMA_DONE.state = 1;
      reg_new.dma_ctrl.LARA_DMA_LATCHn = 1;
      reg_new.dma_ctrl.LOKY_DMA_LATCHp = 0;
    }

    reg_new.dma_ctrl.LENE_DMA_TRIG_d4.state = reg_old.dma_ctrl.LUVY_DMA_TRIG_d0.state;

    if (reg_new.dma_ctrl.LUVY_DMA_TRIG_d0.state) {
      reg_new.dma_ctrl.MYTE_DMA_DONE.state = 0;
      reg_new.dma_ctrl.LYXE_DMA_LATCHp.state = 0;
      bit_clear(reg_new.dma_lo);
      reg_new.dma_ctrl.LARA_DMA_LATCHn = 0;
      reg_new.dma_ctrl.LOKY_DMA_LATCHp = 1;
    }
  }

  if (cpu_addr_new == 0xFF46 && reg_new.cpu_signals.SIG_IN_CPU_RDp.state) {
    bit_unpack(reg_new.cpu_dbus_new, bit_pack_inv(reg_new.dma_hi));
  }

  if (cpu_addr_new == 0xFF46 && reg_new.cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
    auto old_data = bit_pack(reg_new.cpu_dbus_old);
    bit_unpack_inv(reg_new.dma_hi, old_data);
  }

  if (DELTA_HA) {
    reg_new.dma_ctrl.LUVY_DMA_TRIG_d0.state = (cpu_addr_new != 0xFF46 || !reg_new.cpu_signals.SIG_IN_CPU_WRp.state || !CLK_xxxxEFGx) && reg_new.dma_ctrl.LYXE_DMA_LATCHp.state;
    reg_new.dma_ctrl.MATU_DMA_RUNNINGp.state = reg_new.dma_ctrl.LOKY_DMA_LATCHp.state;

    if (reg_new.dma_ctrl.LOKY_DMA_LATCHp.state && !reg_new.dma_ctrl.LENE_DMA_TRIG_d4.state) {
      bit_unpack(reg_new.dma_lo, dma_lo_old + 1);
    }
  }

  bool dma_running_new = reg_new.dma_ctrl.MATU_DMA_RUNNINGp;
  auto dma_addr_new = (bit_pack_inv(reg_new.dma_hi) << 8) | bit_pack(reg_new.dma_lo);
  auto dma_addr_vram_new = dma_running_new && (dma_addr_new >= 0x8000) && (dma_addr_new <= 0x9FFF);

  //----------------------------------------
  // Sprite scanner

  auto scan_count_old = bit_pack(reg_old.scan_counter);

  //----------

  if (vid_rst_new || line_rst_new) {
    reg.sprite_scanner.DOBA_SCAN_DONE_Bp = 0;
    reg.sprite_scanner.BYBA_SCAN_DONE_Ap = 0;
    reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp = 0;
  }
  else {
    if (DELTA_EVEN) reg.sprite_scanner.DOBA_SCAN_DONE_Bp = reg_old.sprite_scanner.BYBA_SCAN_DONE_Ap;
    if (DELTA_HA || DELTA_DE) reg.sprite_scanner.BYBA_SCAN_DONE_Ap = scan_count_old == 39;
    reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp = !reg.sprite_scanner.DOBA_SCAN_DONE_Bp && reg.sprite_scanner.BYBA_SCAN_DONE_Ap;
  }

  //----------

  if (vid_rst_new) {
    reg.sprite_scanner.BESU_SCANNINGn = 0;
  }
  else if (line_rst_new) {
    reg.sprite_scanner.BESU_SCANNINGn = 1;
  }
  else {
    if (reg.lcd.CATU_x113p) reg.sprite_scanner.BESU_SCANNINGn = 1;
    if (reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp) reg.sprite_scanner.BESU_SCANNINGn = 0;
  }

  reg.sprite_scanner.ACYL_SCANNINGp = !vid_rst_new && !dma_running_new && reg.sprite_scanner.BESU_SCANNINGn;

  //----------

  if (!vid_rst_new && (DELTA_HA || DELTA_DE)) {
    bit_copy_inv(&reg.sprite_index, 6, &reg.oam_abus.BUS_OAM_A02n);
    reg.sprite_scanner.CENO_SCANNINGn = reg_old.sprite_scanner.BESU_SCANNINGn;
  }

  if (vid_rst_new) {
    reg.sprite_scanner.CENO_SCANNINGn = 0;
  }

  //----------

  if (vid_rst_new || line_rst_new) {
    bit_clear(reg.scan_counter);
  }
  else if ((DELTA_HA || DELTA_DE) && (scan_count_old != 39)) {
    bit_unpack(reg.scan_counter, scan_count_old + 1);
  }

  // this is unused now
  reg.sprite_scanner.FETO_SCAN_DONEp = bit_pack(reg.scan_counter) == 39;

  bool scanning_new = reg.sprite_scanner.ACYL_SCANNINGp.state;

  //----------------------------------------
  // Global rendering flag 'XYMU'

  if (DELTA_EVEN) {
    reg.VOGA_HBLANKp.state = wodu_hblank_old;
  }

  if (line_rst_new) {
    reg.VOGA_HBLANKp.state = 0;
  }

  bool rendering_old = !reg.XYMU_RENDERINGn.state;

  if (vid_rst_new) reg.XYMU_RENDERINGn.state = 1;
  if (reg.VOGA_HBLANKp.state) reg.XYMU_RENDERINGn.state = 1;
  if (reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.state) reg.XYMU_RENDERINGn.state = 0;

  uint8_t sfetch_phase_old = pack(!(reg_old.sprite_fetcher.TYFO_SFETCH_S0p_D1.state ^ reg_old.sprite_fetcher.TOXE_SFETCH_S0p.state), reg_old.sprite_fetcher.TOXE_SFETCH_S0p.state, reg_old.sprite_fetcher.TULY_SFETCH_S1p.state, reg_old.sprite_fetcher.TESE_SFETCH_S2p.state);

  //-----------------------
  // VID RST BRANCH

  if (vid_rst_new) {
    if (DELTA_EVEN) {
      reg.sprite_fetcher.SOBU_SFETCH_REQp.  state = reg.FEPO_STORE_MATCHp.state && !TOMU_WIN_HITp_old && reg.tile_fetcher.LYRY_BFETCH_DONEp.state && !reg.sprite_fetcher.TAKA_SFETCH_RUNNINGp.state;
    }
    if (DELTA_ODD) {
      reg.sprite_fetcher.SUDA_SFETCH_REQp  .state = reg.sprite_fetcher.SOBU_SFETCH_REQp.state;
      reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.state = reg.sprite_fetcher.TOXE_SFETCH_S0p.state;
    }

    reg.sprite_fetcher.TOXE_SFETCH_S0p.state = 0;
    reg.sprite_fetcher.TULY_SFETCH_S1p.state = 0;
    reg.sprite_fetcher.TESE_SFETCH_S2p.state = 0;
    reg.sprite_fetcher.TOBU_SFETCH_S1p_D2.state = 0;
    reg.sprite_fetcher.VONU_SFETCH_S1p_D4.state = 0;
    reg.sprite_fetcher.SEBA_SFETCH_S1p_D5.state = 0;

    reg.sprite_fetcher.TEXY_SFETCHINGp = 0;

    reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp = 0;

    reg.win_ctrl.NUNU_WIN_MATCHp.state = 0;
    reg.win_ctrl.NOPA_WIN_MODE_Bp.state = 0;
    reg.win_ctrl.PYNU_WIN_MODE_Ap.state = 0;
    reg.win_ctrl.SOVY_WIN_HITp.state = 0;
    reg.win_ctrl.RYDY_WIN_HITp = 0;
    reg.win_ctrl.PUKU_WIN_HITn = 1;

    reg.tile_fetcher.PYGO_FETCH_DONEp.state = 0;
    reg.tile_fetcher.PORY_FETCH_DONEp.state = 0;
    reg.tile_fetcher.NYKA_FETCH_DONEp.state = 0;
    reg.tile_fetcher.POKY_PRELOAD_LATCHp.state = 0;

    reg.sprite_fetcher.TAKA_SFETCH_RUNNINGp.state = 1;
  }

  //-----------------------
  // VID RUN BRANCH

  else {

    if (DELTA_EVEN) {
      reg.sprite_fetcher.SOBU_SFETCH_REQp.  state = reg.FEPO_STORE_MATCHp.state && !TOMU_WIN_HITp_old && reg.tile_fetcher.LYRY_BFETCH_DONEp.state && !reg.sprite_fetcher.TAKA_SFETCH_RUNNINGp.state;
      reg.sprite_fetcher.VONU_SFETCH_S1p_D4.state = reg.sprite_fetcher.TOBU_SFETCH_S1p_D2;
      reg.sprite_fetcher.TOBU_SFETCH_S1p_D2.state = reg.sprite_fetcher.TULY_SFETCH_S1p;

      if ((sfetch_phase_old >> 1) != 5) {
        bit_unpack(&reg.sprite_fetcher.TOXE_SFETCH_S0p, 3, (sfetch_phase_old >> 1) + 1);
      }

      if (reg.sprite_fetcher.SOBU_SFETCH_REQp.state && !reg.sprite_fetcher.SUDA_SFETCH_REQp.state) {
        bit_clear(&reg.sprite_fetcher.TOXE_SFETCH_S0p, 3);
      }
    }

    if (DELTA_ODD) {
      reg.sprite_fetcher.SUDA_SFETCH_REQp  .state = reg.sprite_fetcher.SOBU_SFETCH_REQp.state;
      reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.state = reg.sprite_fetcher.TOXE_SFETCH_S0p.state;
      reg.sprite_fetcher.SEBA_SFETCH_S1p_D5.state = reg.sprite_fetcher.VONU_SFETCH_S1p_D4.state;
    }

    if (line_rst_new) {
      bit_clear(&reg.sprite_fetcher.TOXE_SFETCH_S0p, 3);
    }

    if (reg_new.XYMU_RENDERINGn.state) {
      reg.sprite_fetcher.TOBU_SFETCH_S1p_D2.state = 0;
      reg.sprite_fetcher.VONU_SFETCH_S1p_D4.state = 0;
      reg.sprite_fetcher.SEBA_SFETCH_S1p_D5.state = 0;
    }

    reg.sprite_fetcher.TEXY_SFETCHINGp = (reg.sprite_fetcher.TULY_SFETCH_S1p || reg.sprite_fetcher.VONU_SFETCH_S1p_D4) && !reg_new.XYMU_RENDERINGn.state;

    reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp = reg.sprite_fetcher.TYFO_SFETCH_S0p_D1 && reg.sprite_fetcher.TOXE_SFETCH_S0p && reg.sprite_fetcher.SEBA_SFETCH_S1p_D5 && reg.sprite_fetcher.VONU_SFETCH_S1p_D4;

    if (DELTA_ODD) {
      reg.win_ctrl.NUNU_WIN_MATCHp.state = reg.win_ctrl.PYCO_WIN_MATCHp.state;
    }

    if (DELTA_EVEN) {
      reg.win_ctrl.NOPA_WIN_MODE_Bp.state = reg.win_ctrl.PYNU_WIN_MODE_Ap.state;
    }

    if (reg.win_ctrl.NUNU_WIN_MATCHp.state) {
      reg.win_ctrl.PYNU_WIN_MODE_Ap.state = 1;
    }

    if (!winen_new) {
      reg.win_ctrl.PYNU_WIN_MODE_Ap.state = 0;
    }

    if (line_rst_new) {
      reg.win_ctrl.PYNU_WIN_MODE_Ap.state = 0;
    }

    if (DELTA_EVEN) {
      reg.tile_fetcher.PYGO_FETCH_DONEp.state = reg.tile_fetcher.PORY_FETCH_DONEp.state;
      reg.tile_fetcher.NYKA_FETCH_DONEp.state = reg.tile_fetcher.LYRY_BFETCH_DONEp.state;
    }

    if (DELTA_ODD) {
      reg.tile_fetcher.PORY_FETCH_DONEp.state = reg.tile_fetcher.NYKA_FETCH_DONEp.state;
    }

    if (reg.win_ctrl.PYNU_WIN_MODE_Ap.state && !reg.win_ctrl.NOPA_WIN_MODE_Bp.state) {
      reg.tile_fetcher.PORY_FETCH_DONEp.state = 0;
      reg.tile_fetcher.NYKA_FETCH_DONEp.state = 0;
    }

    if (reg.tile_fetcher.PYGO_FETCH_DONEp.state) {
      reg.tile_fetcher.POKY_PRELOAD_LATCHp.state = 1;
    }

    if (reg_new.XYMU_RENDERINGn.state) {
      reg.tile_fetcher.PYGO_FETCH_DONEp.state = 0;
      reg.tile_fetcher.PORY_FETCH_DONEp.state = 0;
      reg.tile_fetcher.NYKA_FETCH_DONEp.state = 0;
      reg.tile_fetcher.POKY_PRELOAD_LATCHp.state = 0;
    }

    if (DELTA_EVEN) {
      reg.win_ctrl.SOVY_WIN_HITp.state = reg.win_ctrl.RYDY_WIN_HITp.state;
    }

    if (reg.tile_fetcher.PORY_FETCH_DONEp.state) {
      reg.win_ctrl.RYDY_WIN_HITp = 0;
      reg.win_ctrl.PUKU_WIN_HITn = 1;
    }

    if (!reg.tile_fetcher.PORY_FETCH_DONEp.state) {
      if (reg.win_ctrl.PYNU_WIN_MODE_Ap.state && !reg.win_ctrl.NOPA_WIN_MODE_Bp.state) {
        reg.win_ctrl.RYDY_WIN_HITp = 1;
        reg.win_ctrl.PUKU_WIN_HITn = 0;
      }
    }

    if (line_rst_new) {
      reg.sprite_fetcher.TAKA_SFETCH_RUNNINGp.state = 1;
    }

    if (reg.sprite_fetcher.SOBU_SFETCH_REQp && !reg.sprite_fetcher.SUDA_SFETCH_REQp) {
      reg.sprite_fetcher.TAKA_SFETCH_RUNNINGp.state = 1;
    }

    if (!reg_new.XYMU_RENDERINGn.state && !reg.tile_fetcher.POKY_PRELOAD_LATCHp.state && reg.tile_fetcher.NYKA_FETCH_DONEp.state && reg.tile_fetcher.PORY_FETCH_DONEp.state) {
      reg.sprite_fetcher.TAKA_SFETCH_RUNNINGp.state = 0;
    }

    if (reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.state) {
      reg.sprite_fetcher.TAKA_SFETCH_RUNNINGp.state = 0;
    }
  }

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  uint8_t sfetch_phase_new = pack(~(reg_new.sprite_fetcher.TYFO_SFETCH_S0p_D1 ^ reg_new.sprite_fetcher.TOXE_SFETCH_S0p), reg_new.sprite_fetcher.TOXE_SFETCH_S0p, reg_new.sprite_fetcher.TULY_SFETCH_S1p, reg_new.sprite_fetcher.TESE_SFETCH_S2p);

  {
    wire oam_busy_old = (cpu_addr_old >= 0xFE00 && cpu_addr_old <= 0xFEFF) || dma_running_old;
    wire oam_busy_new = (cpu_addr_new >= 0xFE00 && cpu_addr_new <= 0xFEFF) || dma_running_new;

    CHECK_N(rendering_old && scanning_new);
    CHECK_N(!reg_new.XYMU_RENDERINGn.state && scanning_new);
    CHECK_N(rendering_old && reg_old.sprite_scanner.ACYL_SCANNINGp);

    uint8_t BYCU_OAM_CLKp_old = 1;
    if (reg_old.sprite_scanner.ACYL_SCANNINGp)  BYCU_OAM_CLKp_old &= gen_clk_old(0b10001000);
    if (oam_busy_old)  BYCU_OAM_CLKp_old &= gen_clk_old(0b11110000);
    if (rendering_old) BYCU_OAM_CLKp_old &= sfetch_phase_old != 3;

    uint8_t BYCU_OAM_CLKp_new = 1;
    if (scanning_new)  BYCU_OAM_CLKp_new &= gen_clk_new(0b10001000);
    if (oam_busy_new)  BYCU_OAM_CLKp_new &= gen_clk_new(0b11110000);
    if (!reg_new.XYMU_RENDERINGn.state) BYCU_OAM_CLKp_new &= sfetch_phase_new != 3;

    if (!BYCU_OAM_CLKp_old && BYCU_OAM_CLKp_new) {
      bit_copy_inv(reg.oam_temp_a, reg.oam_latch_a);
      bit_copy_inv(reg.oam_temp_b, reg.oam_latch_b);
    }
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  // FIXME need to ditch these adders

  if (vid_rst_new) {
    reg.DEZY_COUNT_CLKp.state = 0;

    bit_clear(reg.sprite_counter);
    bit_clear(reg.sprite_reset_flags);
    bit_clear(reg.sprite_store_flags);

    bit_set(reg.store_x0);
    bit_set(reg.store_x1);
    bit_set(reg.store_x2);
    bit_set(reg.store_x3);
    bit_set(reg.store_x4);
    bit_set(reg.store_x5);
    bit_set(reg.store_x6);
    bit_set(reg.store_x7);
    bit_set(reg.store_x8);
    bit_set(reg.store_x9);
  }
  else if (line_rst_new) {

    // FIXME does this even matter?
    if (DELTA_ODD) {
      reg.DEZY_COUNT_CLKp.state = 1;
    }

    bit_clear(reg.sprite_counter);
    bit_clear(reg.sprite_reset_flags);
    bit_clear(reg.sprite_store_flags);

    bit_set(reg.store_x0);
    bit_set(reg.store_x1);
    bit_set(reg.store_x2);
    bit_set(reg.store_x3);
    bit_set(reg.store_x4);
    bit_set(reg.store_x5);
    bit_set(reg.store_x6);
    bit_set(reg.store_x7);
    bit_set(reg.store_x8);
    bit_set(reg.store_x9);
  }
  else {
    auto pack_ly_new = bit_pack(reg_new.reg_ly);
    auto pack_oam_temp_a_new = bit_pack(reg_new.oam_temp_a);

    Adder ERUC_YDIFF0 = add3(!get_bit(pack_ly_new, 0), get_bit(pack_oam_temp_a_new, 0), 0);
    Adder ENEF_YDIFF1 = add3(!get_bit(pack_ly_new, 1), get_bit(pack_oam_temp_a_new, 1), ERUC_YDIFF0.carry);
    Adder FECO_YDIFF2 = add3(!get_bit(pack_ly_new, 2), get_bit(pack_oam_temp_a_new, 2), ENEF_YDIFF1.carry);
    Adder GYKY_YDIFF3 = add3(!get_bit(pack_ly_new, 3), get_bit(pack_oam_temp_a_new, 3), FECO_YDIFF2.carry);
    Adder GOPU_YDIFF4 = add3(!get_bit(pack_ly_new, 4), get_bit(pack_oam_temp_a_new, 4), GYKY_YDIFF3.carry);
    Adder FUWA_YDIFF5 = add3(!get_bit(pack_ly_new, 5), get_bit(pack_oam_temp_a_new, 5), GOPU_YDIFF4.carry);
    Adder GOJU_YDIFF6 = add3(!get_bit(pack_ly_new, 6), get_bit(pack_oam_temp_a_new, 6), FUWA_YDIFF5.carry);
    Adder WUHU_YDIFF7 = add3(!get_bit(pack_ly_new, 7), get_bit(pack_oam_temp_a_new, 7), GOJU_YDIFF6.carry);

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

    auto ssf_clk =
      !CLK_xBCxxFGx ||
      !reg.sprite_scanner.CENO_SCANNINGn.state ||
      sprite_delta_y.GOPU_YDIFF4.sum ||
      sprite_delta_y.FUWA_YDIFF5.sum ||
      sprite_delta_y.GOJU_YDIFF6.sum ||
      sprite_delta_y.WUHU_YDIFF7.sum ||
      !sprite_delta_y.WUHU_YDIFF7.carry ||
      (reg.reg_lcdc.XYMO_LCDC_SPSIZEn.state && !sprite_delta_y.GYKY_YDIFF3.sum);



    if (DELTA_ODD) {
      if (!reg.DEZY_COUNT_CLKp.state && ssf_clk) {
        auto sprite_count_old = bit_pack(reg.sprite_counter);
        if (sprite_count_old != 10) {
          bit_unpack(reg.sprite_counter, sprite_count_old + 1);
        }
      }

      reg.DEZY_COUNT_CLKp.state = ssf_clk;
    }


    auto sprite_count_new = bit_pack(reg.sprite_counter);

    if (!reg_old.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp && reg.sprite_fetcher.WUTY_SFETCH_DONE_TRIGp) {
      auto pack_sprite_match_flags = bit_pack(reg_old.sprite_match_flags);
      bit_unpack(reg_new.sprite_reset_flags, pack_sprite_match_flags);
    }

    auto store_rst = bit_pack(reg.sprite_reset_flags);

    auto store_clk_old = bit_pack_inv(reg.sprite_store_flags);
    for (int i = 0; i < 10; i++) {
      (&reg.sprite_store_flags.DYHU_STORE0_CLKn)[i] = (i == (int)sprite_count_new) && !bit(ssf_clk);
    }
    auto store_clk_new = bit_pack_inv(reg.sprite_store_flags);

    auto store_clk_pe = (~store_clk_old) & store_clk_new;
    auto store_clk_ne = store_clk_old & (~store_clk_new);

    if (get_bit(store_clk_ne, 0)) bit_copy_inv(reg.store_i0, reg.sprite_ibus);
    if (get_bit(store_clk_ne, 1)) bit_copy_inv(reg.store_i1, reg.sprite_ibus);
    if (get_bit(store_clk_ne, 2)) bit_copy_inv(reg.store_i2, reg.sprite_ibus);
    if (get_bit(store_clk_ne, 3)) bit_copy_inv(reg.store_i3, reg.sprite_ibus);
    if (get_bit(store_clk_ne, 4)) bit_copy_inv(reg.store_i4, reg.sprite_ibus);
    if (get_bit(store_clk_ne, 5)) bit_copy_inv(reg.store_i5, reg.sprite_ibus);
    if (get_bit(store_clk_ne, 6)) bit_copy_inv(reg.store_i6, reg.sprite_ibus);
    if (get_bit(store_clk_ne, 7)) bit_copy_inv(reg.store_i7, reg.sprite_ibus);
    if (get_bit(store_clk_ne, 8)) bit_copy_inv(reg.store_i8, reg.sprite_ibus);
    if (get_bit(store_clk_ne, 9)) bit_copy_inv(reg.store_i9, reg.sprite_ibus);

    if (get_bit(store_clk_ne, 0)) bit_copy_inv(reg.store_l0, reg.sprite_lbus);
    if (get_bit(store_clk_ne, 1)) bit_copy_inv(reg.store_l1, reg.sprite_lbus);
    if (get_bit(store_clk_ne, 2)) bit_copy_inv(reg.store_l2, reg.sprite_lbus);
    if (get_bit(store_clk_ne, 3)) bit_copy_inv(reg.store_l3, reg.sprite_lbus);
    if (get_bit(store_clk_ne, 4)) bit_copy_inv(reg.store_l4, reg.sprite_lbus);
    if (get_bit(store_clk_ne, 5)) bit_copy_inv(reg.store_l5, reg.sprite_lbus);
    if (get_bit(store_clk_ne, 6)) bit_copy_inv(reg.store_l6, reg.sprite_lbus);
    if (get_bit(store_clk_ne, 7)) bit_copy_inv(reg.store_l7, reg.sprite_lbus);
    if (get_bit(store_clk_ne, 8)) bit_copy_inv(reg.store_l8, reg.sprite_lbus);
    if (get_bit(store_clk_ne, 9)) bit_copy_inv(reg.store_l9, reg.sprite_lbus);

    if (get_bit(store_clk_pe, 0)) bit_copy(reg.store_x0, reg.oam_temp_b);
    if (get_bit(store_clk_pe, 1)) bit_copy(reg.store_x1, reg.oam_temp_b);
    if (get_bit(store_clk_pe, 2)) bit_copy(reg.store_x2, reg.oam_temp_b);
    if (get_bit(store_clk_pe, 3)) bit_copy(reg.store_x3, reg.oam_temp_b);
    if (get_bit(store_clk_pe, 4)) bit_copy(reg.store_x4, reg.oam_temp_b);
    if (get_bit(store_clk_pe, 5)) bit_copy(reg.store_x5, reg.oam_temp_b);
    if (get_bit(store_clk_pe, 6)) bit_copy(reg.store_x6, reg.oam_temp_b);
    if (get_bit(store_clk_pe, 7)) bit_copy(reg.store_x7, reg.oam_temp_b);
    if (get_bit(store_clk_pe, 8)) bit_copy(reg.store_x8, reg.oam_temp_b);
    if (get_bit(store_clk_pe, 9)) bit_copy(reg.store_x9, reg.oam_temp_b);

    if (get_bit(store_rst, 0)) bit_set(reg.store_x0);
    if (get_bit(store_rst, 1)) bit_set(reg.store_x1);
    if (get_bit(store_rst, 2)) bit_set(reg.store_x2);
    if (get_bit(store_rst, 3)) bit_set(reg.store_x3);
    if (get_bit(store_rst, 4)) bit_set(reg.store_x4);
    if (get_bit(store_rst, 5)) bit_set(reg.store_x5);
    if (get_bit(store_rst, 6)) bit_set(reg.store_x6);
    if (get_bit(store_rst, 7)) bit_set(reg.store_x7);
    if (get_bit(store_rst, 8)) bit_set(reg.store_x8);
    if (get_bit(store_rst, 9)) bit_set(reg.store_x9);

  }












  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  auto XYDO_PX3p_old = reg_old.pix_count.XYDO_PX3p;
  auto scx_old = bit_pack_inv(&reg_old.reg_scx.DATY_SCX0n, 3);
  auto fine_cnt_old = bit_pack(&reg_old.fine_scroll.RYKU_FINE_CNT0, 3);
  wire fine_match_old = reg_old.fine_scroll.ROXY_FINE_SCROLL_DONEn.state && (scx_old == fine_cnt_old);

  wire clkpipe_en_new = 1;
  if (reg.win_ctrl.RYDY_WIN_HITp.state) clkpipe_en_new = 0;
  if (!reg.tile_fetcher.POKY_PRELOAD_LATCHp.state) clkpipe_en_new = 0;
  if (reg.FEPO_STORE_MATCHp.state) clkpipe_en_new = 0;
  if (reg.WODU_HBLANKp.state) clkpipe_en_new = 0;

  if (DELTA_EVEN) {
    if (clkpipe_en_new) {
      reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.state = fine_match_old;
    }
  }
  else {
    reg.fine_scroll.NYZE_SCX_FINE_MATCH_B.state = reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.state;
  }

  if (reg_new.XYMU_RENDERINGn.state) {
    reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.state = 1;
    reg.fine_scroll.NYZE_SCX_FINE_MATCH_B.state = 0;
    reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.state = 0;
  }

  if (reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.state && !reg.fine_scroll.NYZE_SCX_FINE_MATCH_B.state) {
    reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.state = 0;
  }

  wire CLKPIPE_new = CLK_AxCxExGx;
  if (!clkpipe_en_new) CLKPIPE_new = 1;
  if (reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.state) CLKPIPE_new = 1;

  wire px_old = (uint8_t)bit_pack(reg.pix_count);

  if (!CLKPIPE_old && CLKPIPE_new) {
    bit_unpack(reg.pix_count, px_old + 1);
  }

  if (line_rst_new || vid_rst_new) {
    bit_clear(reg.pix_count);
  }

  if (reg_new.XYMU_RENDERINGn.state || reg.sprite_scanner.CENO_SCANNINGn.state || reg.reg_lcdc.XYLO_LCDC_SPENn.state) {
    bit_clear(reg.sprite_match_flags);
  }
  else {
    uint8_t px = (uint8_t)bit_pack(reg.pix_count);

    bool M0 = px == bit_pack(reg.store_x0);
    bool M1 = px == bit_pack(reg.store_x1);
    bool M2 = px == bit_pack(reg.store_x2);
    bool M3 = px == bit_pack(reg.store_x3);
    bool M4 = px == bit_pack(reg.store_x4);
    bool M5 = px == bit_pack(reg.store_x5);
    bool M6 = px == bit_pack(reg.store_x6);
    bool M7 = px == bit_pack(reg.store_x7);
    bool M8 = px == bit_pack(reg.store_x8);
    bool M9 = px == bit_pack(reg.store_x9);

    reg.FEPO_STORE_MATCHp = M0 | M1 | M2 | M3 | M4 | M5 | M6 | M7 | M8 | M9;

    reg.sprite_match_flags.GUVA_SPRITE0_GETp = M0 & !(0);
    reg.sprite_match_flags.ENUT_SPRITE1_GETp = M1 & !(M0);
    reg.sprite_match_flags.EMOL_SPRITE2_GETp = M2 & !(M0 | M1);
    reg.sprite_match_flags.GYFY_SPRITE3_GETp = M3 & !(M0 | M1 | M2);
    reg.sprite_match_flags.GONO_SPRITE4_GETp = M4 & !(M0 | M1 | M2 | M3);
    reg.sprite_match_flags.GEGA_SPRITE5_GETp = M5 & !(M0 | M1 | M2 | M3 | M4);
    reg.sprite_match_flags.XOJA_SPRITE6_GETp = M6 & !(M0 | M1 | M2 | M3 | M4 | M5);
    reg.sprite_match_flags.GUTU_SPRITE7_GETp = M7 & !(M0 | M1 | M2 | M3 | M4 | M5 | M6);
    reg.sprite_match_flags.FOXA_SPRITE8_GETp = M8 & !(M0 | M1 | M2 | M3 | M4 | M5 | M6 | M7);
    reg.sprite_match_flags.GUZE_SPRITE9_GETp = M9 & !(M0 | M1 | M2 | M3 | M4 | M5 | M6 | M7 | M8);
  }


  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  reg.WODU_HBLANKp = !reg.FEPO_STORE_MATCHp.state && (bit_pack(reg.pix_count) & 167) == 167; // WODU goes high on odd, cleared on H
  auto wodu_hblank_new = reg.WODU_HBLANKp.state;

  {
    wire clk_old = clkpipe_en_old && gen_clk_old(0b01010101);
    wire clk_new = clkpipe_en_new && gen_clk_new(0b01010101);

    if (!clk_old && clk_new) {
      reg.lcd.PAHO_X_8_SYNC.state = XYDO_PX3p_old.state;
    }
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  bit_set(reg.sprite_ibus);
  bit_set(reg.sprite_lbus);

  if (reg.sprite_match_flags.GUVA_SPRITE0_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i0);
  if (reg.sprite_match_flags.ENUT_SPRITE1_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i1);
  if (reg.sprite_match_flags.EMOL_SPRITE2_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i2);
  if (reg.sprite_match_flags.GYFY_SPRITE3_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i3);
  if (reg.sprite_match_flags.GONO_SPRITE4_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i4);
  if (reg.sprite_match_flags.GEGA_SPRITE5_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i5);
  if (reg.sprite_match_flags.XOJA_SPRITE6_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i6);
  if (reg.sprite_match_flags.GUTU_SPRITE7_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i7);
  if (reg.sprite_match_flags.FOXA_SPRITE8_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i8);
  if (reg.sprite_match_flags.GUZE_SPRITE9_GETp.state) bit_copy_inv(reg.sprite_ibus, reg.store_i9);

  if (reg.sprite_match_flags.GUVA_SPRITE0_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l0);
  if (reg.sprite_match_flags.ENUT_SPRITE1_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l1);
  if (reg.sprite_match_flags.EMOL_SPRITE2_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l2);
  if (reg.sprite_match_flags.GYFY_SPRITE3_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l3);
  if (reg.sprite_match_flags.GONO_SPRITE4_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l4);
  if (reg.sprite_match_flags.GEGA_SPRITE5_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l5);
  if (reg.sprite_match_flags.XOJA_SPRITE6_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l6);
  if (reg.sprite_match_flags.GUTU_SPRITE7_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l7);
  if (reg.sprite_match_flags.FOXA_SPRITE8_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l8);
  if (reg.sprite_match_flags.GUZE_SPRITE9_GETp.state) bit_copy_inv(reg.sprite_lbus, reg.store_l9);

  if (reg.sprite_scanner.CENO_SCANNINGn.state || reg_new.XYMU_RENDERINGn.state) {
    auto pack_sprite_index = bit_pack(reg.sprite_index);
    bit_unpack(reg.sprite_ibus, pack_sprite_index);
  }

  if (!reg.FEPO_STORE_MATCHp.state) {
    auto pack_ly_new = bit_pack(reg_new.reg_ly);
    auto pack_oam_temp_a_new = bit_pack(reg_new.oam_temp_a);
    auto pack_ydiff = ~pack_ly_new + pack_oam_temp_a_new;
    bit_unpack(&reg.sprite_lbus, 4, pack_ydiff);
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // WY/WX/window match

  {
    wire VYBO_CLKPIPE_odd = !reg.FEPO_STORE_MATCHp.state && !reg.WODU_HBLANKp.state && gen_clk_new(0b01010101);
    wire ROCO_CLKPIPE_new = !reg.win_ctrl.RYDY_WIN_HITp.state && reg.tile_fetcher.POKY_PRELOAD_LATCHp.state && VYBO_CLKPIPE_odd;

    if (!ROCO_CLKPIPE_old && ROCO_CLKPIPE_new) {
      reg.win_ctrl.PYCO_WIN_MATCHp.state = reg.win_ctrl.NUKO_WX_MATCHp.state;
    }
  }


  if (!reg_new.XYMU_RENDERINGn.state) {
    if (DELTA_EVEN) {
      reg.win_ctrl.RENE_WIN_FETCHn_B.state = reg.win_ctrl.RYFA_WIN_FETCHn_A.state;
    }

    if (!CLKPIPE_old && CLKPIPE_new) {
      reg.win_ctrl.RYFA_WIN_FETCHn_A.state = !reg.win_ctrl.NUKO_WX_MATCHp.state && reg.fine_scroll.RUBU_FINE_CNT2.state && reg.fine_scroll.ROGA_FINE_CNT1.state && reg.fine_scroll.RYKU_FINE_CNT0.state;
    }
  }
  else {
    reg.win_ctrl.RENE_WIN_FETCHn_B.state = 0;
    reg.win_ctrl.RYFA_WIN_FETCHn_A.state = 0;
  }

  if (reg.cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
    if (cpu_addr_new == 0xFF4A) bit_copy_inv(reg.reg_wy, reg.cpu_dbus_old);
    if (cpu_addr_new == 0xFF4B) bit_copy_inv(reg.reg_wx, reg.cpu_dbus_old);
  }

  if (reg.cpu_signals.SIG_IN_CPU_RDp.state) {
    if (cpu_addr_new == 0xFF4A) bit_copy_inv(reg.cpu_dbus_new, reg.reg_wy);
    if (cpu_addr_new == 0xFF4B) bit_copy_inv(reg.cpu_dbus_new, reg.reg_wx);
  }

  if (DELTA_BC) {
    reg.win_ctrl.SARY_WY_MATCHp.state = reg.win_ctrl.ROGE_WY_MATCHp.state;
  }

  if (vid_rst_new) {
    reg.win_ctrl.PYCO_WIN_MATCHp.state = 0;
    reg.win_ctrl.SARY_WY_MATCHp.state = 0;
  }

  reg.win_ctrl.ROGE_WY_MATCHp = (bit_pack(reg.reg_ly) == bit_pack_inv(reg.reg_wy)) && winen_new;

  if (reg.win_ctrl.SARY_WY_MATCHp.state) reg.win_ctrl.REJO_WY_MATCH_LATCHp.state = 1;
  if (reg.lcd.POPU_y144p.state || vid_rst_new) reg.win_ctrl.REJO_WY_MATCH_LATCHp.state = 0;

  reg.win_ctrl.NUKO_WX_MATCHp = (bit_pack_inv(reg.reg_wx) == bit_pack(reg.pix_count)) && reg.win_ctrl.REJO_WY_MATCH_LATCHp.state;

  //----------------------------------------
  // Tile fetch sequencer

  uint8_t bfetch_phase_old = pack(!(reg.tile_fetcher.LYZU_BFETCH_S0p_D1.state ^ reg.tile_fetcher.LAXU_BFETCH_S0p.state), reg.tile_fetcher.LAXU_BFETCH_S0p.state, reg.tile_fetcher.MESU_BFETCH_S1p.state, reg.tile_fetcher.NYVA_BFETCH_S2p.state);

  wire BFETCH_RSTp =
    reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.state ||
    (reg.win_ctrl.PYNU_WIN_MODE_Ap.state && !reg.win_ctrl.NOPA_WIN_MODE_Bp.state) ||
    (reg.win_ctrl.RYFA_WIN_FETCHn_A.state && !reg.win_ctrl.RENE_WIN_FETCHn_B.state) ||
    (reg.win_ctrl.SOVY_WIN_HITp.state && !reg.win_ctrl.RYDY_WIN_HITp.state) ||
    (!reg_new.XYMU_RENDERINGn.state && !reg.tile_fetcher.POKY_PRELOAD_LATCHp.state && reg.tile_fetcher.NYKA_FETCH_DONEp.state && reg.tile_fetcher.PORY_FETCH_DONEp.state);

  if (gen_clk_new(0b01010101)) {
    reg.tile_fetcher.LYZU_BFETCH_S0p_D1.state = reg.tile_fetcher.LAXU_BFETCH_S0p.state;
  }

  if (reg_new.XYMU_RENDERINGn.state) {
    reg.tile_fetcher.LYZU_BFETCH_S0p_D1.state = 0;
  }

  if (BFETCH_RSTp) {
    reg.tile_fetcher.LAXU_BFETCH_S0p.state = 0;
    reg.tile_fetcher.MESU_BFETCH_S1p.state = 0;
    reg.tile_fetcher.NYVA_BFETCH_S2p.state = 0;
    reg.tile_fetcher.LOVY_FETCH_DONEp.state = 0;
    reg.tile_fetcher.LONY_FETCHINGp.state = 1;
    reg.tile_fetcher.LYRY_BFETCH_DONEp.state = 0;
  }
  else {
    if ((bfetch_phase_old < 10) && DELTA_ODD) {
      bit_unpack(&reg.tile_fetcher.LAXU_BFETCH_S0p, 3, (bfetch_phase_old >> 1) + 1);
    }

    if (gen_clk_new(0b10101010)) {
      reg.tile_fetcher.LOVY_FETCH_DONEp.state = reg.tile_fetcher.LYRY_BFETCH_DONEp.state;
    }
    reg.tile_fetcher.LYRY_BFETCH_DONEp = reg.tile_fetcher.LAXU_BFETCH_S0p.state && reg.tile_fetcher.NYVA_BFETCH_S2p.state;
  }

  if (reg.tile_fetcher.LOVY_FETCH_DONEp.state || reg_new.XYMU_RENDERINGn.state) {
    reg.tile_fetcher.LONY_FETCHINGp.state = 0;
  }

  uint8_t bfetch_phase_new = pack(!(reg.tile_fetcher.LYZU_BFETCH_S0p_D1.state ^ reg.tile_fetcher.LAXU_BFETCH_S0p.state), reg.tile_fetcher.LAXU_BFETCH_S0p.state, reg.tile_fetcher.MESU_BFETCH_S1p.state, reg.tile_fetcher.NYVA_BFETCH_S2p.state);


  //----------------------------------------
  // Fine match counter

  wire TEVO_WIN_FETCH_TRIGp_new = 0;
  if (reg.win_ctrl.RYFA_WIN_FETCHn_A.state && !reg.win_ctrl.RENE_WIN_FETCHn_B.state) TEVO_WIN_FETCH_TRIGp_new = 1;
  if (!reg.win_ctrl.RYDY_WIN_HITp.state && reg.win_ctrl.SOVY_WIN_HITp.state) TEVO_WIN_FETCH_TRIGp_new = 1;
  if (!reg.XYMU_RENDERINGn.state && !reg.tile_fetcher.POKY_PRELOAD_LATCHp.state && reg.tile_fetcher.NYKA_FETCH_DONEp.state && reg.tile_fetcher.PORY_FETCH_DONEp.state) TEVO_WIN_FETCH_TRIGp_new = 1;

  {
    wire TYFA_CLKPIPE_new = (!reg.win_ctrl.RYDY_WIN_HITp.state && reg.tile_fetcher.POKY_PRELOAD_LATCHp.state && !reg.FEPO_STORE_MATCHp.state && !wodu_hblank_new && CLK_xBxDxFxG);
    auto fs_old = bit_pack(&reg.fine_scroll.RYKU_FINE_CNT0, 3);
    if (fs_old != 7 && (TYFA_CLKPIPE_old && !TYFA_CLKPIPE_new)) {
      bit_unpack(&reg.fine_scroll.RYKU_FINE_CNT0, 3, fs_old + 1);
    }

    if (TEVO_WIN_FETCH_TRIGp_new || reg.XYMU_RENDERINGn.state) {
      bit_clear(&reg.fine_scroll.RYKU_FINE_CNT0, 3);
    }
  }

  wire SACU_CLKPIPE_new =
    CLK_AxCxExGx ||
    reg.win_ctrl.RYDY_WIN_HITp.state ||
    !reg.tile_fetcher.POKY_PRELOAD_LATCHp.state ||
    reg.FEPO_STORE_MATCHp.state ||
    wodu_hblank_new ||
    reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.state;

  //----------------------------------------
  // PPU / LCD output

  {
    auto& sf = reg.sprite_fetcher;

    // These ffs are weird because they latches on phase change _or_ if rendering stops in the middle of a fetch
    // Good example of gate-level behavior that doesn't matter

    if (rendering_old) {
      if ((bfetch_phase_old == 6) && (bfetch_phase_new == 7 || reg_new.XYMU_RENDERINGn.state)) {
        bit_copy_inv(reg.tile_temp_a, reg.vram_dbus);
      }

      if ((bfetch_phase_old == 2) && (bfetch_phase_new == 3 || reg_new.XYMU_RENDERINGn.state)) {
        bit_copy(reg.tile_temp_b, reg.vram_dbus);
      }

      if ((bfetch_phase_old == 10) && (bfetch_phase_new == 11 || reg_new.XYMU_RENDERINGn.state)) {
        bit_copy(reg.tile_temp_b, reg.vram_dbus);
      }

      if ((sfetch_phase_old == 5) && (sfetch_phase_new == 6 || reg_new.XYMU_RENDERINGn.state)) {
        bit_copy_inv(reg.sprite_pix_a, reg.flipped_sprite);
      }

      if ((sfetch_phase_old == 9) && (sfetch_phase_new == 10 || reg_new.XYMU_RENDERINGn.state)) {
        bit_copy_inv(reg.sprite_pix_b, reg.flipped_sprite);
      }
    }

    //----------------------------------------
    // Pal reg read/write

    if (reg.cpu_signals.SIG_IN_CPU_WRp.state && gen_clk_new(0b00000001)) {
      if (cpu_addr_new == 0xFF47) bit_copy_inv(reg.reg_bgp,  reg.cpu_dbus_old);
      if (cpu_addr_new == 0xFF48) bit_copy_inv(reg.reg_obp0, reg.cpu_dbus_old);
      if (cpu_addr_new == 0xFF49) bit_copy_inv(reg.reg_obp1, reg.cpu_dbus_old);
    }

    if (reg.cpu_signals.SIG_IN_CPU_RDp.state) {
      if (cpu_addr_new == 0xFF47) bit_copy_inv(reg.cpu_dbus_new, reg.reg_bgp);
      if (cpu_addr_new == 0xFF48) bit_copy_inv(reg.cpu_dbus_new, reg.reg_obp0);
      if (cpu_addr_new == 0xFF49) bit_copy_inv(reg.cpu_dbus_new, reg.reg_obp1);
    }

    //----------------------------------------
    // Pixel pipes

    auto tpix_a = bit_pack_inv(reg.tile_temp_a);
    auto tpix_b = bit_pack(reg.tile_temp_b);
    auto spix_a = bit_pack_inv(reg.sprite_pix_a);
    auto spix_b = bit_pack_inv(reg.sprite_pix_b);

    auto spipe_a = bit_pack(reg.spr_pipe_a);
    auto spipe_b = bit_pack(reg.spr_pipe_b);
    auto bpipe_a = bit_pack(reg.bgw_pipe_a);
    auto bpipe_b = bit_pack(reg.bgw_pipe_b);
    auto mpipe   = bit_pack(reg.mask_pipe);
    auto ppipe   = bit_pack(reg.pal_pipe);

    if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
      spipe_a = (spipe_a << 1) | 0;
      spipe_b = (spipe_b << 1) | 0;
      bpipe_a = (bpipe_a << 1) | 0;
      bpipe_b = (bpipe_b << 1) | 0;
      mpipe   = (mpipe   << 1) | 1;
      ppipe   = (ppipe   << 1) | 0;
    }
    
    if (reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.state || !(!reg.win_ctrl.PYNU_WIN_MODE_Ap.state || reg.win_ctrl.NOPA_WIN_MODE_Bp.state) || TEVO_WIN_FETCH_TRIGp_new) {
      bpipe_a = tpix_a;
      bpipe_b = tpix_b;
    }

    if (sf.WUTY_SFETCH_DONE_TRIGp.state) {
      auto smask = (spipe_a | spipe_b);
      spipe_a = (spipe_a & smask) | (spix_a & ~smask);
      spipe_b = (spipe_b & smask) | (spix_b & ~smask);
      mpipe = reg.oam_temp_b.DEPO_OAM_DB7p.state ? mpipe | ~smask : mpipe & smask;
      ppipe = reg.oam_temp_b.GOMO_OAM_DB4p.state ? ppipe | ~smask : ppipe & smask;
    }

    bit_unpack(reg.spr_pipe_a, spipe_a);
    bit_unpack(reg.spr_pipe_b, spipe_b);
    bit_unpack(reg.bgw_pipe_a, bpipe_a);
    bit_unpack(reg.bgw_pipe_b, bpipe_b);
    bit_unpack(reg.mask_pipe, mpipe);
    bit_unpack(reg.pal_pipe, ppipe);

    //----------------------------------------
    // Pipe merge and output

    wire PIX_BG_LOp = reg.bgw_pipe_a.PYBO_BGW_PIPE_A7.state && !reg.reg_lcdc.VYXE_LCDC_BGENn.state;
    wire PIX_BG_HIp = reg.bgw_pipe_b.SOHU_BGW_PIPE_B7.state && !reg.reg_lcdc.VYXE_LCDC_BGENn.state;
    wire PIX_SP_LOp = reg.spr_pipe_a.WUFY_SPR_PIPE_A7.state && !reg.reg_lcdc.XYLO_LCDC_SPENn.state;
    wire PIX_SP_HIp = reg.spr_pipe_b.VUPY_SPR_PIPE_B7.state && !reg.reg_lcdc.XYLO_LCDC_SPENn.state;

    auto pal_idx = 0;
    auto pal = 0;

    auto bgp  = bit_pack_inv(reg.reg_bgp);
    auto obp0 = bit_pack_inv(reg.reg_obp0);
    auto obp1 = bit_pack_inv(reg.reg_obp1);

    if (PIX_SP_HIp || PIX_SP_LOp) {
      pal_idx = pack(PIX_SP_LOp, PIX_SP_HIp);
      pal = reg.pal_pipe.LYME_PAL_PIPE_D7.state ? obp1 : obp0;
    }
    else {
      pal_idx = pack(PIX_BG_LOp, PIX_BG_HIp);
      pal = bgp;
    }

    reg.REMY_LD0n = (pal >> (pal_idx * 2 + 0)) & 1;
    reg.RAVO_LD1n = (pal >> (pal_idx * 2 + 1)) & 1;
  }

  //----------------------------------------
  // LCD pins

  if (!vid_rst_new) {
    reg.lcd.PIN_52_LCD_CNTRL.state = !reg.lcd.SYGU_LINE_STROBE.state && !reg.lcd.RUTU_x113p.state;

    if (rutu_x113p_old && !rutu_x113p_new) {
      reg.lcd.LUCA_LINE_EVENp.state = !reg.lcd.LUCA_LINE_EVENp.state;
    }


    if (!popu_y144p_old && popu_y144p_new) {
      reg.lcd.NAPO_FRAME_EVENp.state = !reg.lcd.NAPO_FRAME_EVENp.state;
    }

    reg.lcd.PIN_56_LCD_FLIPS.state = reg.lcd.NAPO_FRAME_EVENp.state ^ reg.lcd.LUCA_LINE_EVENp.state;


    auto ly = bit_pack(reg.reg_ly);

    if (nype_x113p_old && !nype_x113p_new) {
      reg.lcd.MEDA_VSYNC_OUTn.state = ly == 0;
    }

    reg.lcd.PIN_57_LCD_VSYNC.state = !reg.lcd.MEDA_VSYNC_OUTn.state;

    if (reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.state && reg.lcd.PAHO_X_8_SYNC.state) {
      reg.lcd.POME = 0;
      reg.lcd.RUJU = 1;
      reg.lcd.POFY = 0;
    }
    else if (reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.state) {
      reg.lcd.POME = 0;
      reg.lcd.RUJU = 0;
      reg.lcd.POFY = 1;
    }
    else if (reg.lcd.PAHO_X_8_SYNC.state) {
      reg.lcd.POME = 1;
      reg.lcd.RUJU = 1;
      reg.lcd.POFY = 0;
    }

    reg.lcd.PIN_50_LCD_DATA1.state = reg.RAVO_LD1n.state;
    reg.lcd.PIN_51_LCD_DATA0.state = reg.REMY_LD0n.state;
    reg.lcd.PIN_54_LCD_HSYNC.state = !reg.lcd.POFY.state;
    reg.lcd.PIN_55_LCD_LATCH.state = !reg.lcd.RUTU_x113p.state;

    if (reg.pix_count.XEHO_PX0p.state && reg.pix_count.XYDO_PX3p.state) {
      reg.lcd.WUSA_LCD_CLOCK_GATE.state = 1;
    }
    if (reg.VOGA_HBLANKp.state) {
      reg.lcd.WUSA_LCD_CLOCK_GATE.state = 0;
    }

    reg.lcd.PIN_53_LCD_CLOCK.state = (!reg.lcd.WUSA_LCD_CLOCK_GATE.state || !SACU_CLKPIPE_new) && (!reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.state || reg.fine_scroll.NYZE_SCX_FINE_MATCH_B.state);
  }
  else {
    reg.lcd.LUCA_LINE_EVENp.state = 0;
    reg.lcd.NAPO_FRAME_EVENp.state = 0;
    reg.lcd.MEDA_VSYNC_OUTn.state = 0;
    reg.lcd.WUSA_LCD_CLOCK_GATE.state = 0;

    reg.lcd.POME = 1;
    reg.lcd.RUJU = 1;
    reg.lcd.POFY = 0;

    reg.lcd.PIN_50_LCD_DATA1.state = reg.RAVO_LD1n.state;
    reg.lcd.PIN_51_LCD_DATA0.state = reg.REMY_LD0n.state;
    reg.lcd.PIN_52_LCD_CNTRL.state = 1;
    reg.lcd.PIN_53_LCD_CLOCK.state = 1;
    reg.lcd.PIN_54_LCD_HSYNC.state = 1;
    reg.lcd.PIN_55_LCD_LATCH.state = !reg.div.UGOT_DIV06p.state;
    reg.lcd.PIN_56_LCD_FLIPS.state = !reg.div.TULU_DIV07p.state;
    reg.lcd.PIN_57_LCD_VSYNC.state = 1;
  }

  //----------------------------------------
  // Audio

  //tock_spu_logic();

  //----------------------------------------
  // Memory buses

  if (reg.cpu_signals.SIG_IN_CPU_EXT_BUSp.state && !cpu_addr_vram_new) {
    bit_copy(&reg.ext_addr_latch, 15, &reg.cpu_abus_new);
  }

  //----------------------------------------

  if (dma_running_new && !dma_addr_vram_new) {
    reg.ext_ctrl.PIN_80_CSn.state = !reg.dma_hi.MARU_DMA_A15n.state;
    bit_copy_inv(reg.ext_abus, reg.dma_lo);
    bit_copy(&reg.ext_abus.PIN_09_A08, 7, &reg.dma_hi.NAFA_DMA_A08n);
  }
  else {
    reg.ext_ctrl.PIN_80_CSn.state = reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK.state && cpu_addr_ram_new;
    bit_copy_inv(&reg.ext_abus.PIN_01_A00, 15, &reg.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p);
  }

  //----------------------------------------

  if (!(dma_running_new && !dma_addr_vram_new) && reg.cpu_signals.SIG_IN_CPU_EXT_BUSp.state && reg.cpu_signals.SIG_IN_CPU_WRp.state) {
    reg.ext_ctrl.PIN_79_RDn.state = cpu_addr_vram_new;
    reg.ext_ctrl.PIN_78_WRn.state = CLK_xxxxEFGx && !cpu_addr_vram_new;
  }
  else {
    reg.ext_ctrl.PIN_79_RDn.state = 1;
    reg.ext_ctrl.PIN_78_WRn.state = 0;
  }

  //----------------------------------------


  if (dma_running_new && !dma_addr_vram_new) {
    reg.ext_abus.PIN_16_A15.state = reg.dma_hi.MARU_DMA_A15n.state;
  }
  else if (!reg.cpu_signals.TEPU_BOOT_BITn.state && cpu_addr_new <= 0x00FF) {
    reg.ext_abus.PIN_16_A15.state = 0;
  }
  else {
    reg.ext_abus.PIN_16_A15.state = reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK.state && !reg.cpu_abus_new.BUS_CPU_A15p.state;
  }

  CHECK_N(reg.cpu_signals.SIG_IN_CPU_RDp.state && reg.cpu_signals.SIG_IN_CPU_WRp.state);

  if (reg.cpu_signals.SIG_IN_CPU_EXT_BUSp.state && reg.cpu_signals.SIG_IN_CPU_WRp.state && !cpu_addr_vram_new) {
    bit_copy_inv(reg.ext_dbus, reg.cpu_dbus_new);
  }
  else {
    bit_clear(reg.ext_dbus);
  }

  //----------------------------------------
  // Ext read

  if (reg.ext_ctrl.PIN_79_RDn.state) {
    uint16_t ext_addr = (uint16_t)bit_pack_inv(reg.ext_abus);
    
    auto rom_addr_mask = cart_rom_addr_mask(cart_blob);
    auto ram_addr_mask = cart_ram_addr_mask(cart_blob);

    const int region = ext_addr >> 13;
    uint8_t data_in = 0x00;

    bool ext_read_en = false;

    if (cart_has_mbc1(cart_blob)) {

      bool mbc1_ram_en = reg.ext_mbc.MBC1_RAM_EN.state;
      bool mbc1_mode = reg.ext_mbc.MBC1_MODE.state;

      uint32_t mbc1_rom0_bank = mbc1_mode ? bit_pack(&reg.ext_mbc.MBC1_BANK5, 2) : 0;
      uint32_t mbc1_rom0_addr = ((ext_addr & 0x3FFF) | (mbc1_rom0_bank << 19)) & rom_addr_mask;

      uint32_t mbc1_rom1_bank = bit_pack(&reg.ext_mbc.MBC1_BANK0, 7);
      if ((mbc1_rom1_bank & 0x1F) == 0) mbc1_rom1_bank |= 1;
      uint32_t mbc1_rom1_addr = ((ext_addr & 0x3FFF) | (mbc1_rom1_bank << 14)) & rom_addr_mask;

      uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&reg.ext_mbc.MBC1_BANK5, 2) : 0;
      uint32_t mbc1_ram_addr = ((ext_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & ram_addr_mask;

      switch (region) {
      case 0: ext_read_en = true; data_in = cart_blob[mbc1_rom0_addr]; break;
      case 1: ext_read_en = true; data_in = cart_blob[mbc1_rom0_addr]; break;
      case 2: ext_read_en = true; data_in = cart_blob[mbc1_rom1_addr]; break;
      case 3: ext_read_en = true; data_in = cart_blob[mbc1_rom1_addr]; break;
      case 4: data_in = 0x00; break;
      case 5: ext_read_en = true; data_in = mbc1_ram_en ? mem.cart_ram[mbc1_ram_addr] : 0xFF; break;
      case 6: ext_read_en = true; data_in = mem.int_ram[ext_addr & 0x1FFF]; break;
      case 7: ext_read_en = true; data_in = mem.int_ram[ext_addr & 0x1FFF]; break;
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
      case 6: ext_read_en = true; data_in = mem.int_ram[ext_addr & 0x1FFF]; break;
      case 7: ext_read_en = true; data_in = mem.int_ram[ext_addr & 0x1FFF]; break;
      }
    }

    if (ext_read_en) bit_unpack_inv(reg.ext_dbus, data_in);
  }


  //----------------------------------------
  // Ext write


  if (reg.ext_ctrl.PIN_78_WRn.state) {
    auto ext_addr = bit_pack_inv(reg.ext_abus);
    auto region = ext_addr >> 13;
    auto data_out = bit_pack_inv(reg.ext_dbus);

    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_ram_en = reg.ext_mbc.MBC1_RAM_EN.state;
      bool mbc1_mode = reg.ext_mbc.MBC1_MODE.state;

      auto mbc1_ram_bank = mbc1_mode ? bit_pack(&reg.ext_mbc.MBC1_BANK5, 2) : 0;
      auto mbc1_ram_addr = ((ext_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask(cart_blob);

      switch (region) {
      case 0: reg.ext_mbc.MBC1_RAM_EN = (data_out & 0x0F) == 0x0A; break;
      case 1: bit_unpack(&reg.ext_mbc.MBC1_BANK0, 5, data_out); break;
      case 2: bit_unpack(&reg.ext_mbc.MBC1_BANK5, 2, data_out); break;
      case 3: reg.ext_mbc.MBC1_MODE = (data_out & 1); break;
      case 4: break;
      case 5: if (cart_has_ram(cart_blob) && mbc1_ram_en) mem.cart_ram[mbc1_ram_addr & cart_ram_addr_mask(cart_blob)] = (uint8_t)data_out; break;
      case 6: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out; break;
      case 7: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out; break;
      }
    }
    else {
      switch (region) {
      case 0: break;
      case 1: break;
      case 2: break;
      case 3: break;
      case 4: break;
      case 5: if (cart_has_ram(cart_blob)) mem.cart_ram[ext_addr & cart_ram_addr_mask(cart_blob)] = (uint8_t)data_out; break;
      case 6: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out;break;
      case 7: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out;break;
      }
    }
  }

  //----------------------------------------


  if (reg.cpu_signals.SIG_IN_CPU_RDp.state && reg.cpu_signals.SIG_IN_CPU_EXT_BUSp.state && !cpu_addr_vram_new && reg.cpu_signals.SIG_IN_CPU_LATCH_EXT.state) {
    bit_copy_inv(reg.cpu_dbus_new, reg.ext_data_latch);
  }
  else {
    bit_copy(reg.ext_data_latch, reg.ext_dbus);
  }

  {
    bit_set(reg.vram_abus);
    bit_set(reg.vram_dbus);

    //--------------------------------------------
    // CPU vram read address

    if (!dma_addr_vram_new && reg.XYMU_RENDERINGn.state) {
      bit_copy_inv(reg.vram_abus, reg.cpu_abus_new);
    }

    //--------------------------------------------
    // DMA vram read address

    if (dma_addr_vram_new) {
      bit_copy_inv(reg.vram_abus, reg.dma_lo);
      bit_copy(&reg.vram_abus.BUS_VRAM_A08n, 5, &reg.dma_hi.NAFA_DMA_A08n);
    }

    //--------------------------------------------
    // SCX/SCY regs and BG map read address

    if (reg.cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
      if (cpu_addr_new == 0xFF42) bit_copy_inv(reg.reg_scy, reg.cpu_dbus_old);
      if (cpu_addr_new == 0xFF43) bit_copy_inv(reg.reg_scx, reg.cpu_dbus_old);
    }

    if (reg.cpu_signals.SIG_IN_CPU_RDp.state) {
      if (cpu_addr_new == 0xFF42) bit_copy_inv(reg.cpu_dbus_new, reg.reg_scy);
      if (cpu_addr_new == 0xFF43) bit_copy_inv(reg.cpu_dbus_new, reg.reg_scx);
    }

    auto px  = bit_pack(reg.pix_count);
    auto ly  = bit_pack(reg.reg_ly);
    auto scx = bit_pack_inv(reg.reg_scx);
    auto scy = bit_pack_inv(reg.reg_scy);

    auto sum_x = px + scx;
    auto sum_y = ly + scy;

    if (reg.tile_fetcher.LONY_FETCHINGp.state && !reg.tile_fetcher.MESU_BFETCH_S1p.state && !reg.tile_fetcher.NYVA_BFETCH_S2p.state && !reg.win_ctrl.PYNU_WIN_MODE_Ap.state) {
      bit_unpack_inv(&reg.vram_abus.BUS_VRAM_A00n, 5, sum_x >> 3);
      bit_unpack_inv(&reg.vram_abus.BUS_VRAM_A05n, 5, sum_y >> 3);
      reg.vram_abus.BUS_VRAM_A10n.state = reg.reg_lcdc.XAFO_LCDC_BGMAPn.state;
      reg.vram_abus.BUS_VRAM_A11n.state = 0;
      reg.vram_abus.BUS_VRAM_A12n.state = 0;
    }

    //--------------------------------------------
    // Win coord x

    auto win_map_x_old = bit_pack(reg.win_x);

    if (!(TEVO_WIN_FETCH_TRIGp_old && reg_old.win_ctrl.PYNU_WIN_MODE_Ap.state) && TEVO_WIN_FETCH_TRIGp_new && reg.win_ctrl.PYNU_WIN_MODE_Ap.state) {
      bit_unpack(reg.win_x, win_map_x_old + 1);
    }

    if (!winen_new || line_rst_new || reg.reg_lcdc.XONA_LCDC_LCDENn.state) bit_clear(reg.win_x);

    auto win_map_x_new = bit_pack(reg.win_x);

    //--------------------------------------------
    // Win coord y

    auto win_y_old = reg.lcd.POPU_y144p || reg.reg_lcdc.XONA_LCDC_LCDENn ? 0 : bit_pack(reg.win_y);

    if (reg_old.win_ctrl.PYNU_WIN_MODE_Ap.state && !reg.win_ctrl.PYNU_WIN_MODE_Ap.state) {
      bit_unpack(reg.win_y, win_y_old + 1);
    }

    if (reg.lcd.POPU_y144p || reg.reg_lcdc.XONA_LCDC_LCDENn) {
      reg.win_y.VYNO_WIN_TILE_Y0 = 0;
      reg.win_y.VUJO_WIN_TILE_Y1 = 0;
      reg.win_y.VYMU_WIN_TILE_Y2 = 0;
      reg.win_y.TUFU_WIN_MAP_Y0  = 0;
      reg.win_y.TAXA_WIN_MAP_Y1  = 0;
      reg.win_y.TOZO_WIN_MAP_Y2  = 0;
      reg.win_y.TATE_WIN_MAP_Y3  = 0;
      reg.win_y.TEKE_WIN_MAP_Y4  = 0;
    }

    auto win_y_new = bit_pack(reg.win_y);


    if (reg.tile_fetcher.LONY_FETCHINGp.state && !reg.tile_fetcher.MESU_BFETCH_S1p.state && !reg.tile_fetcher.NYVA_BFETCH_S2p.state && reg.win_ctrl.PYNU_WIN_MODE_Ap.state) {
      uint32_t addr = 0;

      auto wx = bit_pack_inv(&reg.win_x.WYKA_WIN_MAP_X0, 5);
      auto wy = bit_pack_inv(&reg.win_y.TUFU_WIN_MAP_Y0, 5);

      bit_unpack(&reg.vram_abus.BUS_VRAM_A00n, 5, wx);
      bit_unpack(&reg.vram_abus.BUS_VRAM_A05n, 5, wy);
      bit_copy_inv(&reg.vram_abus.BUS_VRAM_A05n, 5, &reg.win_y.TUFU_WIN_MAP_Y0);

      addr |= reg.reg_lcdc.WOKY_LCDC_WINMAPn.state << 10;

      reg.vram_abus.BUS_VRAM_A10n.state = get_bit(addr, 10);
      reg.vram_abus.BUS_VRAM_A11n.state = get_bit(addr, 11);
      reg.vram_abus.BUS_VRAM_A12n.state = get_bit(addr, 12);
    }

    //--------------------------------------------
    // BG/Win tile read address

    if (reg.tile_fetcher.LONY_FETCHINGp.state && (reg.tile_fetcher.MESU_BFETCH_S1p.state || reg.tile_fetcher.NYVA_BFETCH_S2p.state)) {
      uint32_t addr  = 0;

      addr |= reg.tile_fetcher.NYVA_BFETCH_S2p.state << 0;
      addr |= (reg.win_ctrl.PYNU_WIN_MODE_Ap.state ? bit_pack(&reg.win_y, 3) : bit_mask(sum_y, 3)) << 1;
      addr |= bit_pack(reg.tile_temp_b) << 4;
      addr |= (!reg.tile_temp_b.PYJU_TILE_DB7p.state && reg.reg_lcdc.WEXU_LCDC_BGTILEn.state) << 12;

      bit_unpack_inv(reg.vram_abus, addr);
    }

    //--------------------------------------------
    // Sprite read address

    if (reg.sprite_fetcher.TEXY_SFETCHINGp.state) {
      uint32_t addr = 1 << 12;

      addr |= reg.sprite_fetcher.VONU_SFETCH_S1p_D4.state;

      auto line = bit_pack(reg.sprite_lbus) ^ bit_widen(!reg.oam_temp_b.YZOS_OAM_DB6p, 4);
      auto tile = bit_pack(reg.oam_temp_a);

      if (reg.reg_lcdc.XYMO_LCDC_SPSIZEn) {
        addr |= ((tile & 0b11111111) << 4) | ((line & 0b01111) << 1);
      }
      else {
        addr |= ((tile & 0b11111110) << 4) | ((line & 0b11111) << 1);
      }

      bit_unpack_inv(&reg.vram_abus.BUS_VRAM_A00n, 12, addr);
    }

    //--------------------------------------------
    // Vram address pin driver

    bit_copy(reg.vram_ext_abus, reg.vram_abus);

    //--------------------------------------------
    // CPU bus to Vram data bus

    if (reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK && reg.XYMU_RENDERINGn && cpu_addr_vram_new && reg.cpu_signals.SIG_IN_CPU_WRp.state) {
      bit_copy(reg.vram_dbus, reg.cpu_dbus_new);
    }

    //--------------------------------------------
    // Vram control pins

    if (reg.XYMU_RENDERINGn.state) {
      wire APOV_CPU_WRp = CLK_xxxxEFGx && reg.cpu_signals.SIG_IN_CPU_WRp.state;
      wire ABUZ_EXT_RAM_CS_CLK = CLK_xxCDEFGH && reg.cpu_signals.SIG_IN_CPU_EXT_BUSp.state;

      if (dma_addr_vram_new) {
        reg.vram_ext_ctrl.PIN_43_VRAM_CSn.state = 1;
        reg.vram_ext_ctrl.PIN_45_VRAM_OEn.state = 1;
      }
      else {
        reg.vram_ext_ctrl.PIN_43_VRAM_CSn.state = cpu_addr_vram_new && ABUZ_EXT_RAM_CS_CLK;
        reg.vram_ext_ctrl.PIN_45_VRAM_OEn.state = !cpu_addr_vram_new || !reg.cpu_signals.SIG_IN_CPU_WRp.state;
      }

      reg.vram_ext_ctrl.PIN_49_VRAM_WRn.state = cpu_addr_vram_new && APOV_CPU_WRp && ABUZ_EXT_RAM_CS_CLK;
    }
    else {
      if (dma_addr_vram_new) {
        reg.vram_ext_ctrl.PIN_43_VRAM_CSn.state = 1;
        reg.vram_ext_ctrl.PIN_49_VRAM_WRn.state = 0;
        reg.vram_ext_ctrl.PIN_45_VRAM_OEn.state = 1;
      }
      else if (reg.tile_fetcher.LONY_FETCHINGp.state) {
        reg.vram_ext_ctrl.PIN_43_VRAM_CSn.state = 1;
        reg.vram_ext_ctrl.PIN_49_VRAM_WRn.state = 0;
        reg.vram_ext_ctrl.PIN_45_VRAM_OEn.state = 1;
      }
      else if (reg.sprite_fetcher.TEXY_SFETCHINGp.state) {
        reg.vram_ext_ctrl.PIN_43_VRAM_CSn.state = 1;
        reg.vram_ext_ctrl.PIN_49_VRAM_WRn.state = 0;
        reg.vram_ext_ctrl.PIN_45_VRAM_OEn.state = !reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.state || reg.sprite_fetcher.TOXE_SFETCH_S0p.state;
      }
      else {
        reg.vram_ext_ctrl.PIN_43_VRAM_CSn.state = 0;
        reg.vram_ext_ctrl.PIN_49_VRAM_WRn.state = 0;
        reg.vram_ext_ctrl.PIN_45_VRAM_OEn.state = 0;
      }
    }

    auto vram_addr = bit_pack_inv(reg.vram_ext_abus);
    uint8_t data = 0xFF;

    if (reg.vram_ext_ctrl.PIN_45_VRAM_OEn.state) {
      data = mem.vid_ram[vram_addr];
    }

    //--------------------------------------------
    // Vram data pin driver

    bit_clear(reg.vram_ext_dbus);

    if (reg.vram_ext_ctrl.PIN_45_VRAM_OEn.state) {
      bit_unpack_inv(reg.vram_ext_dbus, mem.vid_ram[vram_addr]);
    }

    if (reg.vram_ext_ctrl.PIN_49_VRAM_WRn.state) {
      mem.vid_ram[vram_addr] = (uint8_t)bit_pack_inv(reg.vram_ext_dbus);
    }

    if (cpu_addr_vram_new && reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK.state && reg.XYMU_RENDERINGn.state && reg.cpu_signals.SIG_IN_CPU_WRp.state) {
      bit_copy_inv(reg.vram_ext_dbus, reg.vram_dbus);
    }

    //--------------------------------------------

    if (reg.vram_ext_ctrl.PIN_49_VRAM_WRn.state) {
      mem.vid_ram[vram_addr] = (uint8_t)bit_pack_inv(reg.vram_ext_dbus);
    }

    //--------------------------------------------
    // Vram pins to vram bus

    if (!cpu_addr_vram_new || !reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK.state || !reg.XYMU_RENDERINGn.state || !reg.cpu_signals.SIG_IN_CPU_WRp.state) {
      bit_copy_inv(reg.vram_dbus, reg.vram_ext_dbus);
    }

    //--------------------------------------------
    // Vram bus to cpu bus

    if (cpu_addr_vram_new && reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK.state && reg.XYMU_RENDERINGn.state && reg.cpu_signals.SIG_IN_CPU_RDp.state && reg.cpu_signals.SIG_IN_CPU_LATCH_EXT.state) {
      bit_copy(reg.cpu_dbus_new, reg.vram_dbus);
    }

    //--------------------------------------------
    // Vram bus to sprite x flipper

    wire XONO_FLIP_X = reg.oam_temp_b.BAXO_OAM_DB5p.state && reg.sprite_fetcher.TEXY_SFETCHINGp.state;

    reg.flipped_sprite = {
      XONO_FLIP_X ? reg.vram_dbus.BUS_VRAM_D07p.state : reg.vram_dbus.BUS_VRAM_D00p.state,
      XONO_FLIP_X ? reg.vram_dbus.BUS_VRAM_D06p.state : reg.vram_dbus.BUS_VRAM_D01p.state,
      XONO_FLIP_X ? reg.vram_dbus.BUS_VRAM_D05p.state : reg.vram_dbus.BUS_VRAM_D02p.state,
      XONO_FLIP_X ? reg.vram_dbus.BUS_VRAM_D04p.state : reg.vram_dbus.BUS_VRAM_D03p.state,
      XONO_FLIP_X ? reg.vram_dbus.BUS_VRAM_D03p.state : reg.vram_dbus.BUS_VRAM_D04p.state,
      XONO_FLIP_X ? reg.vram_dbus.BUS_VRAM_D02p.state : reg.vram_dbus.BUS_VRAM_D05p.state,
      XONO_FLIP_X ? reg.vram_dbus.BUS_VRAM_D01p.state : reg.vram_dbus.BUS_VRAM_D06p.state,
      XONO_FLIP_X ? reg.vram_dbus.BUS_VRAM_D00p.state : reg.vram_dbus.BUS_VRAM_D07p.state,
    };
  }



  //----------------------------------------
  // oam

  {
    wire cpu_rd = reg.cpu_signals.SIG_IN_CPU_RDp.state;
    wire cpu_wr = reg.cpu_signals.SIG_IN_CPU_WRp.state && CLK_xxxxEFGx;
  
    wire dbus_free = reg.cpu_signals.SIG_IN_CPU_LATCH_EXT.state;
    wire dbus_busy = !dbus_free;

    wire addr_oam = (cpu_addr_new >= 0xFE00) && (cpu_addr_new <= 0xFEFF);

    // this is weird, why is it always 0 when not in reset?
    reg.oam_ctrl.MAKA_LATCH_EXTp.state = 0;

    bit_set(reg.oam_abus);
    bit_set(reg.oam_dbus_a);
    bit_set(reg.oam_dbus_b);

    if (CLK_ABCDxxxx) {
      reg.oam_ctrl.WUJE_CPU_OAM_WRn.state = 1;
    }

    if (addr_oam && cpu_wr) {
      reg.oam_ctrl.WUJE_CPU_OAM_WRn.state = 0;
    }

    wire oam_clk_old = reg.oam_ctrl.SIG_OAM_CLKn.state;

    if (dma_running_new) {
      bit_copy_inv(reg.oam_abus, reg.dma_lo);

      if ((dma_addr_new >= 0x8000) && (dma_addr_new <= 0x9FFF)) {
        bit_copy_inv(reg.oam_dbus_a, reg.vram_dbus);
        bit_copy_inv(reg.oam_dbus_b, reg.vram_dbus);
      }
      else {
        bit_copy(reg.oam_dbus_a, reg.ext_dbus);
        bit_copy(reg.oam_dbus_b, reg.ext_dbus);
      }

      reg.oam_ctrl.SIG_OAM_CLKn .state = CLK_ABCDxxxx;
      reg.oam_ctrl.SIG_OAM_WRn_A.state = CLK_ABCDxxxx || !reg.oam_abus.BUS_OAM_A00n.state;
      reg.oam_ctrl.SIG_OAM_WRn_B.state = CLK_ABCDxxxx ||  reg.oam_abus.BUS_OAM_A00n.state;
      reg.oam_ctrl.SIG_OAM_OEn  .state = !(dbus_busy && addr_oam && cpu_rd);
    }
    else if (scanning_new) {
      reg.oam_abus.BUS_OAM_A00n.state = 1;
      reg.oam_abus.BUS_OAM_A01n.state = 1;
      bit_copy_inv(&reg.oam_abus.BUS_OAM_A02n, 6, &reg.scan_counter);


      reg.oam_ctrl.SIG_OAM_CLKn .state = (!vid_rst_new && gen_clk_new(0b10011001)) && (!vid_rst_new && gen_clk_new(0b11001100)) && (!addr_oam || !CLK_xxxxEFGH);
      reg.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      reg.oam_ctrl.SIG_OAM_WRn_B.state = 1;
      reg.oam_ctrl.SIG_OAM_OEn  .state = (!vid_rst_new && gen_clk_new(0b10011001)) && !(dbus_busy && addr_oam && cpu_rd);
    }
    else if (!reg_new.XYMU_RENDERINGn.state) {
      reg.oam_abus.BUS_OAM_A00n.state = 0;
      reg.oam_abus.BUS_OAM_A01n.state = 0;
      bit_copy_inv(&reg.oam_abus.BUS_OAM_A02n, 6, &reg.sprite_ibus.BUS_SPR_I0);

      reg.oam_ctrl.SIG_OAM_CLKn .state = 
        (reg.sprite_fetcher.TULY_SFETCH_S1p.state || reg.sprite_fetcher.TESE_SFETCH_S2p.state || (reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.state && !reg.sprite_fetcher.TOXE_SFETCH_S0p.state)) && 
        (!addr_oam || !CLK_xxxxEFGH);
      reg.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      reg.oam_ctrl.SIG_OAM_WRn_B.state = 1;
      reg.oam_ctrl.SIG_OAM_OEn  .state = 
        (reg.sprite_fetcher.TULY_SFETCH_S1p.state || reg.sprite_fetcher.TESE_SFETCH_S2p.state || !reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.state) && 
        !(dbus_busy && addr_oam && cpu_rd);
    }
    else if (addr_oam) {
      bit_copy_inv(reg.oam_abus, reg.cpu_abus_new);
      if (!reg.oam_ctrl.WUJE_CPU_OAM_WRn.state) {
        bit_copy_inv(reg.oam_dbus_a, reg.cpu_dbus_new);
        bit_copy_inv(reg.oam_dbus_b, reg.cpu_dbus_new);
      }
      reg.oam_ctrl.SIG_OAM_CLKn .state = CLK_ABCDxxxx;
      reg.oam_ctrl.SIG_OAM_WRn_A.state = (!cpu_wr || !reg.oam_abus.BUS_OAM_A00n.state);
      reg.oam_ctrl.SIG_OAM_WRn_B.state = (!cpu_wr ||  reg.oam_abus.BUS_OAM_A00n.state);
      reg.oam_ctrl.SIG_OAM_OEn  .state = (!cpu_rd || !dbus_busy);
    }
    else {
      bit_copy_inv(reg.oam_abus,   reg.cpu_abus_new);
      bit_copy_inv(reg.oam_dbus_a, reg.cpu_dbus_new);
      bit_copy_inv(reg.oam_dbus_b, reg.cpu_dbus_new);
      reg.oam_ctrl.SIG_OAM_CLKn .state = 1;
      reg.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      reg.oam_ctrl.SIG_OAM_WRn_B.state = 1;
      reg.oam_ctrl.SIG_OAM_OEn  .state = 1;
    }

    wire oam_clk_new = reg.oam_ctrl.SIG_OAM_CLKn.state;

    // data in from oam
    uint8_t oam_data_a, oam_data_b;
    {

      uint8_t oam_addr = (uint8_t)bit_pack_inv(reg.oam_abus) >> 1;
      oam_data_a = (uint8_t)bit_pack_inv(reg.oam_dbus_a);
      oam_data_b = (uint8_t)bit_pack_inv(reg.oam_dbus_b);

      if (oam_clk_old && !oam_clk_new) {
        if (!reg.oam_ctrl.SIG_OAM_WRn_A.state) mem.oam_ram[(oam_addr << 1) + 0] = oam_data_a;
        if (!reg.oam_ctrl.SIG_OAM_WRn_B.state) mem.oam_ram[(oam_addr << 1) + 1] = oam_data_b;
      }
      reg.oam_ctrl.old_oam_clk = !reg.oam_ctrl.SIG_OAM_CLKn.state;

      oam_data_a = mem.oam_ram[(oam_addr << 1) + 0];
      oam_data_b = mem.oam_ram[(oam_addr << 1) + 1];
    }






    bool latch_oam = false;
    latch_oam |= (dbus_busy && addr_oam && cpu_rd);
    latch_oam |= scanning_new && gen_clk_new(0b01100110);
    latch_oam |= !reg_new.XYMU_RENDERINGn.state && !reg.sprite_fetcher.TULY_SFETCH_S1p.state && !reg.sprite_fetcher.TESE_SFETCH_S2p.state && reg.sprite_fetcher.TYFO_SFETCH_S0p_D1.state;

    if (latch_oam) {
      bit_unpack_inv(reg.oam_dbus_a, oam_data_a);
      bit_unpack_inv(reg.oam_dbus_b, oam_data_b);
      bit_copy(reg.oam_latch_a, reg.oam_dbus_a);
      bit_copy(reg.oam_latch_b, reg.oam_dbus_b);
    }

    if (cpu_rd && dbus_free && addr_oam && !latch_oam && !dma_running_new && !scanning_new && reg_new.XYMU_RENDERINGn.state) {
      if (reg.oam_abus.BUS_OAM_A00n.state) {
        bit_copy_inv(reg.cpu_dbus_new, reg.oam_latch_a);
      }
      else {
        bit_copy_inv(reg.cpu_dbus_new, reg.oam_latch_b);
      }
    }
  }

  //----------------------------------------
  // zram

  {
    wire CSp = (cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE);

    if (reg.zram_bus.clk_old.state && !reg.cpu_signals.TAPU_CPU_WRp.state && CSp) {
      mem.zero_ram[cpu_addr_new & 0x007F] = (uint8_t)bit_pack(reg.cpu_dbus_old);
    }
    reg.zram_bus.clk_old = reg.cpu_signals.TAPU_CPU_WRp.state;

    uint8_t data = mem.zero_ram[cpu_addr_new & 0x007F];

    if (CSp && reg.cpu_signals.TEDO_CPU_RDp.state) {
      bit_unpack(reg.cpu_dbus_new, data);
    }
  }

  //----------------------------------------
  // And finally, interrupts.

  {
    auto pack_cpu_dbus_old = bit_pack(reg.cpu_dbus_old);
    auto pack_cpu_dbus_new = bit_pack(reg.cpu_dbus_new);
    auto pack_ie = bit_pack(reg.reg_ie);
    auto pack_if = bit_pack(reg.reg_if);
    auto pack_stat = bit_pack(reg.reg_stat);

    if (cpu_addr_new == 0xFFFF && reg.cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
      pack_ie = pack_cpu_dbus_old;
    }

    if (cpu_addr_new == 0xFF41 && reg.cpu_signals.SIG_IN_CPU_WRp.state && DELTA_GH) {
      pack_stat = (~pack_cpu_dbus_old >> 3) & 0b00001111;
    }

    if (cpu_addr_new == 0xFF41 && cpu_rd_new) {
      uint8_t data = 0x80;

      data |= (!reg.XYMU_RENDERINGn.state || reg.lcd.POPU_y144p.state) << 0;
      data |= (!reg.XYMU_RENDERINGn.state || reg.sprite_scanner.ACYL_SCANNINGp.state) << 1;
      data |= (!reg.RUPO_LYC_MATCHn.state) << 2;
      data |= (pack_stat ^ 0b1111) << 3;

      pack_cpu_dbus_new = data;
    }


    bool int_stat_new = 0;
    if (!get_bit(pack_stat, 0) && wodu_hblank_new && !reg.lcd.POPU_y144p.state) int_stat_new = 1;
    if (!get_bit(pack_stat, 1) && reg.lcd.POPU_y144p.state) int_stat_new = 1;
    if (!get_bit(pack_stat, 2) && !reg.lcd.POPU_y144p.state && reg.lcd.RUTU_x113p.state) int_stat_new = 1;
    if (!get_bit(pack_stat, 3) && reg.ROPO_LY_MATCH_SYNCp.state) int_stat_new = 1;

    wire int_lcd_new = reg.lcd.POPU_y144p.state;
    wire int_joy_new = !reg.joy_int.APUG_JP_GLITCH3.state || !reg.joy_int.BATU_JP_GLITCH0.state;
    wire int_tim_new = reg.int_ctrl.MOBA_TIMER_OVERFLOWp.state;
    //wire int_ser = serial.CALY_SER_CNT3.state;
    wire int_ser_new = 0;

    if (!int_lcd_old  && int_lcd_new)  pack_if |= (1 << 0);
    if (!int_stat_old && int_stat_new) pack_if |= (1 << 1);
    if (!int_tim_old  && int_tim_new)  pack_if |= (1 << 2);
    if (!int_ser_old  && int_ser_new)  pack_if |= (1 << 3);
    if (!int_joy_old  && int_joy_new)  pack_if |= (1 << 4);

    // note this is an async set so it doesn't happen on the GH clock edge like other writes
    if (reg.cpu_signals.SIG_IN_CPU_WRp.state & (cpu_addr_new == 0xFF0F) & gen_clk_new(0b00001110)) {
      pack_if = pack_cpu_dbus_new;
    }

    pack_if &= ~bit_pack(reg.cpu_ack);

    if (cpu_addr_new == 0xFFFF && reg.cpu_signals.SIG_IN_CPU_RDp.state) {
      pack_cpu_dbus_new = pack_ie | 0b11100000;
    }

    if (cpu_addr_new == 0xFF0F && reg.cpu_signals.SIG_IN_CPU_RDp.state) {
      bit_unpack(reg.latch_if,  pack_if);
      pack_cpu_dbus_new = pack_if | 0b11100000;
    }

    bit_unpack(reg.cpu_dbus_new, pack_cpu_dbus_new);
    bit_unpack(reg.cpu_int, pack_if);
    bit_unpack(reg.reg_ie, pack_ie);
    bit_unpack(reg.reg_if, pack_if);
    bit_unpack(reg.reg_stat, pack_stat);
  }

  //----------------------------------------

  reg.cpu_abus_old = reg.cpu_abus_new;
  reg.cpu_dbus_old = reg.cpu_dbus_new;
}
