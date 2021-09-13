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

  reg.cpu_abus.reset_to_bootrom();
  reg.cpu_dbus.reset_to_bootrom();

  reg.sprite_ibus.reset_to_bootrom();
  reg.sprite_lbus.reset_to_bootrom();

  reg.reg_dma.NAFA_DMA_A08n.state = 0b00011010;
  reg.reg_dma.PYNE_DMA_A09n.state = 0b00011010;
  reg.reg_dma.PARA_DMA_A10n.state = 0b00011010;
  reg.reg_dma.NYDO_DMA_A11n.state = 0b00011010;
  reg.reg_dma.NYGY_DMA_A12n.state = 0b00011010;
  reg.reg_dma.PULA_DMA_A13n.state = 0b00011010;
  reg.reg_dma.POKU_DMA_A14n.state = 0b00011010;
  reg.reg_dma.MARU_DMA_A15n.state = 0b00011010;

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
  reg.reg_joy.reset_to_bootrom();

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

  CHECK_N(bit(reg.sys_clk.AFUR_xxxxEFGH.qp_old()));
  CHECK_P(bit(reg.sys_clk.ALEF_AxxxxFGH.qp_old()));
  CHECK_P(bit(reg.sys_clk.APUK_ABxxxxGH.qp_old()));
  CHECK_P(bit(reg.sys_clk.ADYK_ABCxxxxH.qp_old()));

  sys.phase_total = 0;
  sys.phase_origin = 46880720;
;

  //----------------------------------------
  // Wait for SIG_CPU_START

  while(bit(~reg.sys_rst.SIG_CPU_STARTp.out_old())) {
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
    reg.reg_div.TERO_DIV03p.state = 0b00011010;
    reg.reg_div.UNYK_DIV04p.state = 0b00011010;
    reg.reg_div.UPOF_DIV15p.state = 0b00011011;
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
  reg.cpu_abus.reset_to_cart_new();
  reg.cpu_dbus.reset_to_cart_new();

  reg.vram_abus.lo.reset_to_cart();
  reg.vram_abus.hi.reset_to_cart();
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
  reg.ext_abus.lo.reset_to_cart();
  reg.ext_abus.hi.reset_to_cart();
  reg.ext_dbus.reset_to_cart();
  reg.ext_addr_latch.reset_to_cart();
  reg.ext_data_latch.reset_to_cart();

  //zram_bus.reset_to_cart();



  reg.sys_rst.reset_to_cart();
  reg.sys_clk.reset_to_cart();
  reg.reg_div.reset_to_cart();
  
  //interrupts.reset_to_cart();
  reg.reg_if.reset_to_cart();
  reg.reg_ie.reset_to_cart();
  reg.int_latch.reset_to_cart();
  reg.cpu_int.reset_to_cart();
  reg.cpu_ack.reset_to_cart();

  //serial.reset_to_cart();

  //reset_sprite_store();
  reg.sprite_counter.BESE_SPRITE_COUNT0.state = 0b00011010;
  reg.sprite_counter.CUXY_SPRITE_COUNT1.state = 0b00011010;
  reg.sprite_counter.BEGO_SPRITE_COUNT2.state = 0b00011010;
  reg.sprite_counter.DYBE_SPRITE_COUNT3.state = 0b00011010;

  reg.sprite_scanner.reset_to_cart();
  reg.scan_counter.reset_to_cart();
  reg.sprite_index.reset_to_cart();

  reg.sfetch_counter.reset_to_cart();
  reg.sfetch_control.reset_to_cart();

  reg.int_ctrl.RUPO_LYC_MATCHn.state = 0b00011000;

  reg.reg_stat.reset_to_cart();
  reg.pix_count.reset_to_cart();
  reg.mask_pipe.reset_to_cart();
  reg.lcd.REMY_LD0n.state = 0b00011000;
  reg.lcd.RAVO_LD1n.state = 0b00011000;

  reg.dma_lo.reset_to_cart();
  reg.reg_dma.reset_to_cart();
  reg.MATU_DMA_RUNNINGp.state = 0b00011010;
  reg.dma_ctrl.reset_to_cart();

  reg.reg_bgp.reset_to_cart();
  reg.reg_obp0.reset_to_cart();
  reg.reg_obp1.reset_to_cart();
  
  //joy.reset_to_cart();
  reg.joy_int.reset_to_cart();
  reg.reg_joy.reset_to_cart();

  reg.reg_lcdc.reset_to_cart();
  reg.lcd.reset_to_cart();

  reg.reg_lx.reset_to_cart();
  reg.reg_ly.reset_to_cart();

  reg.int_ctrl.ROPO_LY_MATCH_SYNCp.state = 0b00011001;
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
  GEN_OFFSET(reg.cpu_abus),
  GEN_OFFSET(reg.cpu_dbus),
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
  GEN_OFFSET(reg.ext_abus.lo),
  GEN_OFFSET(reg.ext_abus.hi),
  GEN_OFFSET(reg.ext_dbus),
  GEN_OFFSET(reg.ext_data_latch),
  GEN_OFFSET(reg.ext_addr_latch),
  GEN_OFFSET(reg.ext_mbc),
  GEN_OFFSET(reg.zram_bus),
  GEN_OFFSET(reg.VOGA_HBLANKp),
  GEN_OFFSET(reg.XYMU_RENDERINGn),
  GEN_OFFSET(reg.sys_rst),
  GEN_OFFSET(reg.sys_clk),
  GEN_OFFSET(reg.reg_div),
  GEN_OFFSET(reg.reg_tima),
  GEN_OFFSET(reg.reg_tma),
  GEN_OFFSET(reg.reg_tac),
  GEN_OFFSET(reg.MATU_DMA_RUNNINGp),
  GEN_OFFSET(reg.dma_ctrl),
  GEN_OFFSET(reg.dma_lo),
  GEN_OFFSET(reg.reg_dma),
  GEN_OFFSET(reg.int_ctrl),
  GEN_OFFSET(reg.reg_if),
  GEN_OFFSET(reg.reg_ie),
  GEN_OFFSET(reg.int_latch),
  GEN_OFFSET(reg.cpu_int),
  GEN_OFFSET(reg.cpu_ack),
  GEN_OFFSET(reg.joy_int),
  GEN_OFFSET(reg.reg_joy),
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
  GEN_OFFSET(reg.sprite_counter),
  GEN_OFFSET(reg.FEPO_STORE_MATCHp),
  GEN_OFFSET(reg.sprite_match_flags),
  GEN_OFFSET(reg.sprite_reset_flags),
  GEN_OFFSET(reg.sprite_store_flags),
  GEN_OFFSET(reg.sprite_scanner),
  GEN_OFFSET(reg.scan_counter),
  GEN_OFFSET(reg.sprite_index),
  GEN_OFFSET(reg.sfetch_control),
  GEN_OFFSET(reg.sprite_pix_a),
  GEN_OFFSET(reg.sprite_pix_b),
  GEN_OFFSET(reg.tfetch_counter),
  GEN_OFFSET(reg.tfetch_control),
  GEN_OFFSET(reg.tile_temp_a),
  GEN_OFFSET(reg.tile_temp_b),
  GEN_OFFSET(reg.reg_lcdc),
  GEN_OFFSET(reg.reg_stat),
  GEN_OFFSET(reg.reg_scx),
  GEN_OFFSET(reg.reg_scy),
  GEN_OFFSET(reg.reg_wy),
  GEN_OFFSET(reg.reg_wx),
  GEN_OFFSET(reg.win_x.map),
  GEN_OFFSET(reg.win_y.tile),
  GEN_OFFSET(reg.win_y.map),
  GEN_OFFSET(reg.win_ctrl),
  GEN_OFFSET(reg.fine_count),
  GEN_OFFSET(reg.fine_scroll),
  GEN_OFFSET(reg.pix_count),
  GEN_OFFSET(reg.mask_pipe),
  GEN_OFFSET(reg.bgw_pipe_a),
  GEN_OFFSET(reg.bgw_pipe_b),
  GEN_OFFSET(reg.spr_pipe_a),
  GEN_OFFSET(reg.spr_pipe_b),
  GEN_OFFSET(reg.pal_pipe),
  GEN_OFFSET(reg.lcd.REMY_LD0n),
  GEN_OFFSET(reg.lcd.RAVO_LD1n),
  GEN_OFFSET(reg.lcd),
  GEN_OFFSET(reg.reg_lx),
  GEN_OFFSET(reg.reg_ly),
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
  cpu.cpu_data_latch &= (uint8_t)bit_pack(reg.cpu_dbus);
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
  const GateBoyReg  reg_old = reg;
  GateBoyReg& reg_new = reg;

  //----------

  /*#p27.SYLO*/ wire SYLO_WIN_HITn_old = not1(reg_old.win_ctrl.RYDY_WIN_HITp.out_old());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp_old = not1(SYLO_WIN_HITn_old);

  //----------

  wire EXT_vcc = 1;
  wire EXT_gnd = 0;

  /*_SIG_VCC*/ reg.SIG_VCC.sig_in(EXT_vcc);
  /*_SIG_GND*/ reg.SIG_GND.sig_in(EXT_gnd);

  //-----------------------------------------------------------------------------

  memset(&reg.cpu_abus, BIT_NEW | BIT_PULLED | 1, sizeof(reg.cpu_abus));
  memset(&reg.cpu_dbus, BIT_NEW | BIT_PULLED | 1, sizeof(reg.cpu_dbus));

  //-----------------------------------------------------------------------------

  {
    bool EXT_cpu_latch_ext;

    if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      // Data has to be driven on EFGH or we fail the wave tests
      reg.cpu_dbus.set_data(cpu.bus_req_new.write, cpu.bus_req_new.data_lo);
      EXT_cpu_latch_ext = cpu.bus_req_new.read;
    }
    else {
      reg.cpu_dbus.set_data(false, 0);
      EXT_cpu_latch_ext = 0;
    }
    /*_SIG_IN_CPU_LATCH_EXT*/ reg.cpu_signals.SIG_IN_CPU_DBUS_FREE.sig_in(EXT_cpu_latch_ext);

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
      reg.cpu_abus.set_addr(cpu.bus_req_new.addr & 0x00FF);

      if (addr_high) EXT_addr_new = false;
      if (addr_boot) EXT_addr_new = false;
      if (addr_vram) EXT_addr_new = false;
    }
    else {
      EXT_cpu_rd = cpu.bus_req_new.read;
      EXT_cpu_wr = cpu.bus_req_new.write;
      reg.cpu_abus.set_addr(cpu.bus_req_new.addr);

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

    /*_PIN_74*/ reg.sys_clk.PIN_74_CLK.pin_clk(EXT_clkin, EXT_clkgood);
    /*_PIN_71*/ reg.sys_rst.PIN_71_RST.pin_in(EXT_sys_rst);
    /*_PIN_76*/ reg.sys_rst.PIN_76_T2.pin_in(EXT_sys_t2);
    /*_PIN_77*/ reg.sys_rst.PIN_77_T1.pin_in(EXT_sys_t1);

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

    /*_SIG_CPU_CLKREQ*/ reg.sys_clk.SIG_CPU_CLKREQ.sig_in(EXT_sys_clkreq);

    /*_SIG_CPU_ADDR_HIp*/ reg.cpu_signals.SIG_CPU_ADDR_HIp.sig_out(reg.cpu_abus.SYRO_FE00_FFFF());
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
  tock_reset_gates(bit(sys.fastboot) ? reg.reg_div.TERO_DIV03p : reg.reg_div.UPOF_DIV15p);
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
    /*#p29.DOBA*/ reg.sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(ALET_xBxDxFxH(), BAGY_LINE_RSTn, reg_old.sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
    /*#p29.BYBA*/ reg.sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(XUPY_ABxxEFxx(), BAGY_LINE_RSTn, reg_old.sprite_scanner.FETO_SCAN_DONEp.out_old());
    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(reg.sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp, reg.sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);

    /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(ATAR_VID_RSTp(), reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new());
    /*#p29.CENO*/ reg.sprite_scanner.CENO_SCANNINGn.dff17(XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), reg_old.sprite_scanner.BESU_SCANNINGn.qp_old());
    /*#p28.BESU*/ reg.sprite_scanner.BESU_SCANNINGn.nor_latch(reg.lcd.CATU_x113p.qp_new(), ASEN_SCAN_DONE_TRIGp);

    /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(reg.MATU_DMA_RUNNINGp.qp_new());
    /*#p28.ACYL*/ reg.ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, reg.sprite_scanner.BESU_SCANNINGn.qp_new());

    // Sprite scanner grabs the sprite index off the _old_ oam address bus
    /*_p28.YFOT*/ wire YFOT_OAM_A2p_old = not1(reg_old.oam_abus.BUS_OAM_A02n.out_old());
    /*_p28.YFOC*/ wire YFOC_OAM_A3p_old = not1(reg_old.oam_abus.BUS_OAM_A03n.out_old());
    /*_p28.YVOM*/ wire YVOM_OAM_A4p_old = not1(reg_old.oam_abus.BUS_OAM_A04n.out_old());
    /*_p28.YMEV*/ wire YMEV_OAM_A5p_old = not1(reg_old.oam_abus.BUS_OAM_A05n.out_old());
    /*_p28.XEMU*/ wire XEMU_OAM_A6p_old = not1(reg_old.oam_abus.BUS_OAM_A06n.out_old());
    /*_p28.YZET*/ wire YZET_OAM_A7p_old = not1(reg_old.oam_abus.BUS_OAM_A07n.out_old());
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

  /*#p21.VOGA*/ reg.VOGA_HBLANKp.dff17(ALET_xBxDxFxH(), TADY_LINE_RSTn, reg_old.WODU_HBLANKp.out_old());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), reg.VOGA_HBLANKp.qp_new());
  /*#p21.XYMU*/ reg.XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new());
  /*_p24.LOBY*/ wire LOBY_RENDERINGn = not1(reg.XYMU_RENDERINGn.qn_new());
  /*#p27.PAHA*/ wire PAHA_RENDERINGn = not1(reg.XYMU_RENDERINGn.qn_new());
  /*_p29.TEPA*/ wire TEPA_RENDERINGp = not1(reg.XYMU_RENDERINGn.qn_new());

  //----------------------------------------
  // Sprite fetch sequencer

  {
    /*_p27.TUKU*/ wire TUKU_WIN_HITn_old = not1(TOMU_WIN_HITp_old);
    /*_p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_old = not1(reg_old.sfetch_control.TAKA_SFETCH_RUNNINGp.qp_old());
    /*_p27.TEKY*/ wire TEKY_SFETCH_REQp_old = and4(reg_old.FEPO_STORE_MATCHp.out_old(), TUKU_WIN_HITn_old, reg_old.tfetch_control.LYRY_BFETCH_DONEp.out_old(), SOWO_SFETCH_RUNNINGn_old);
    /*_p27.SUDA*/ reg.sfetch_control.SUDA_SFETCH_REQp.dff17(LAPE_AxCxExGx(), reg.SIG_VCC.out_new(), reg_old.sfetch_control.SOBU_SFETCH_REQp.qp_old());
    /*_p27.SOBU*/ reg.sfetch_control.SOBU_SFETCH_REQp.dff17(TAVA_xBxDxFxH(), reg.SIG_VCC.out_new(), TEKY_SFETCH_REQp_old);

    /*#p29.TYFO*/ reg.sfetch_control.TYFO_SFETCH_S0p_D1.dff17(LAPE_AxCxExGx(), reg.SIG_VCC.out_new(),        reg_old.sfetch_counter.TOXE_SFETCH_S0p.qp_old());
    
    /*#p29.TOBU*/ reg.sfetch_control.TOBU_SFETCH_S1p_D2.dff17(TAVA_xBxDxFxH(), reg.XYMU_RENDERINGn.qn_new(), reg_old.sfetch_counter.TULY_SFETCH_S1p.qp_old());
    /*#p29.VONU*/ reg.sfetch_control.VONU_SFETCH_S1p_D4.dff17(TAVA_xBxDxFxH(), reg.XYMU_RENDERINGn.qn_new(), reg_old.sfetch_control.TOBU_SFETCH_S1p_D2.qp_old());
    /*#p29.SEBA*/ reg.sfetch_control.SEBA_SFETCH_S1p_D5.dff17(LAPE_AxCxExGx(), reg.XYMU_RENDERINGn.qn_new(), reg_old.sfetch_control.VONU_SFETCH_S1p_D4.qp_old());

    /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(reg.sfetch_control.SOBU_SFETCH_REQp.qp_new(), reg.sfetch_control.SUDA_SFETCH_REQp.qn_new());
    /*#p27.SECA*/ wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), reg.ATEJ_LINE_RSTp.out_new());

    // Feedback loop
    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand2(reg.sfetch_counter.TESE_SFETCH_S2p.qp_any(), reg.sfetch_counter.TOXE_SFETCH_S0p.qp_any());
      /*#p29.TOMA*/ wire TOMA_SFETCH_xBxDxFxH = nand2(LAPE_AxCxExGx(), TAME_SFETCH_CLK_GATE);
      /*#p29.TOXE*/ reg.sfetch_counter.TOXE_SFETCH_S0p.dff17_any(TOMA_SFETCH_xBxDxFxH,                        SECA_SFETCH_STARTn, reg.sfetch_counter.TOXE_SFETCH_S0p.qn_any());
      /*#p29.TULY*/ reg.sfetch_counter.TULY_SFETCH_S1p.dff17_any(reg.sfetch_counter.TOXE_SFETCH_S0p.qn_any(), SECA_SFETCH_STARTn, reg.sfetch_counter.TULY_SFETCH_S1p.qn_any());
      /*#p29.TESE*/ reg.sfetch_counter.TESE_SFETCH_S2p.dff17_any(reg.sfetch_counter.TULY_SFETCH_S1p.qn_any(), SECA_SFETCH_STARTn, reg.sfetch_counter.TESE_SFETCH_S2p.qn_any());
    }

    /*_p29.SAKY*/ wire SAKY_SFETCHn = nor2(reg.sfetch_counter.TULY_SFETCH_S1p.qp_new(), reg.sfetch_control.VONU_SFETCH_S1p_D4.qp_new());
    /*_p29.TYSO*/ wire TYSO_SFETCHINGn = or2(SAKY_SFETCHn, TEPA_RENDERINGp); // def or
    /*_p29.TEXY*/ reg.sfetch_control.TEXY_SFETCHINGp = not1(TYSO_SFETCHINGn);

    /*_p29.TYNO*/ wire TYNO = nand3(reg.sfetch_counter.TOXE_SFETCH_S0p.qp_new(), reg.sfetch_control.SEBA_SFETCH_S1p_D5.qp_new(), reg.sfetch_control.VONU_SFETCH_S1p_D4.qp_new());
    /*_p29.VUSA*/ wire VUSA_SPRITE_DONEn = or2(reg.sfetch_control.TYFO_SFETCH_S0p_D1.qn_new(), TYNO);
    /*_p29.WUTY*/ reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp = not1(VUSA_SPRITE_DONEn);
  }

  /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(reg.sfetch_control.SOBU_SFETCH_REQp.qp_new(), reg.sfetch_control.SUDA_SFETCH_REQp.qn_new());
  /*#p27.SECA*/ wire SECA_SFETCH_STARTn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), reg.ATEJ_LINE_RSTp.out_new());

  //----------------------------------------
  // Window state has some interaction with the tile fetcher here.

  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg.reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn, XAPO_VID_RSTn());
  /*_p27.NUNU*/ reg.win_ctrl.NUNU_WIN_MATCHp.dff17(MEHE_AxCxExGx(), XAPO_VID_RSTn(), reg_old.win_ctrl.PYCO_WIN_MATCHp.qp_old());
  /*_p27.NOPA*/ reg.win_ctrl.NOPA_WIN_MODE_Bp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), reg_old.win_ctrl.PYNU_WIN_MODE_Ap.qp_old());
  /*_p27.PYNU*/ reg.win_ctrl.PYNU_WIN_MODE_Ap.nor_latch(reg.win_ctrl.NUNU_WIN_MATCHp.qp_new(), XOFO_WIN_RSTp);

  /*#p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and2(reg.win_ctrl.PYNU_WIN_MODE_Ap.qp_new(), reg.win_ctrl.NOPA_WIN_MODE_Bp.qn_new());
  /*_p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not1(NUNY_WIN_MODE_TRIGp);
  /*_p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not1(NYFO_WIN_MODE_TRIGn);
  /*_p24.NAFY*/ wire NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);

  /*_p24.PYGO*/ reg.tfetch_control.PYGO_FETCH_DONEp.dff17(ALET_xBxDxFxH(), reg.XYMU_RENDERINGn.qn_new(), reg_old.tfetch_control.PORY_FETCH_DONEp.qp_old());
  /*_p24.PORY*/ reg.tfetch_control.PORY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NAFY_WIN_MODE_TRIGn, reg_old.tfetch_control.NYKA_FETCH_DONEp.qp_old());
  /*_p24.NYKA*/ reg.tfetch_control.NYKA_FETCH_DONEp.dff17(ALET_xBxDxFxH(), NAFY_WIN_MODE_TRIGn, reg_old.tfetch_control.LYRY_BFETCH_DONEp.out_old());
  /*_p24.POKY*/ reg.tfetch_control.POKY_PRELOAD_LATCHp.nor_latch(reg.tfetch_control.PYGO_FETCH_DONEp.qp_new(), LOBY_RENDERINGn);

  /*_p27.SOVY*/ reg.win_ctrl.SOVY_WIN_HITp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), reg_old.win_ctrl.RYDY_WIN_HITp.out_old());
  /*_p27.RYDY*/ reg.win_ctrl.RYDY_WIN_HITp = nor3(reg.win_ctrl.PUKU_WIN_HITn.out_mid(), reg.tfetch_control.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ reg.win_ctrl.PUKU_WIN_HITn = nor2(reg.win_ctrl.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);
  /*_p27.RYDY*/ reg.win_ctrl.RYDY_WIN_HITp = nor3(reg.win_ctrl.PUKU_WIN_HITn.out_mid(), reg.tfetch_control.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /*_p27.PUKU*/ reg.win_ctrl.PUKU_WIN_HITn = nor2(reg.win_ctrl.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp);

  /*#p27.SYLO*/ wire SYLO_WIN_HITn = not1(reg.win_ctrl.RYDY_WIN_HITp.out_new());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp = not1(SYLO_WIN_HITn);
  /*#p24.SOCY*/ wire SOCY_WIN_HITn = not1(TOMU_WIN_HITp);

  /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn = not1(reg.tfetch_control.POKY_PRELOAD_LATCHp.qp_new());
  /*_p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn = nand4(reg.XYMU_RENDERINGn.qn_new(), ROMO_PRELOAD_DONEn, reg.tfetch_control.NYKA_FETCH_DONEp.qp_new(), reg.tfetch_control.PORY_FETCH_DONEp.qp_new());
  /*_p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp = not1(SUVU_PRELOAD_DONE_TRIGn);

  /*_p27.VEKU*/ wire VEKU_SFETCH_ENDn = nor2(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), TAVE_PRELOAD_DONE_TRIGp); // def nor
  /*_p27.TAKA*/ reg.sfetch_control.TAKA_SFETCH_RUNNINGp.nand_latch(SECA_SFETCH_STARTn, VEKU_SFETCH_ENDn);

  //----------------------------------------
  // OAM temp register B stores sprite X coordinate during scan, so we need a copy of it for later.

  auto oam_temp_b_old = reg_old.oam_temp_b;

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  {
    /*_p25.CUFE*/ wire CUFE_OAM_CLKp = not_or_and3(reg.cpu_abus.SARO_ADDR_OAMp(), reg.MATU_DMA_RUNNINGp.qp_new(), MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
    /*_p25.AVER*/ wire AVER_AxxxExxx = nand2(reg.ACYL_SCANNINGp.out_new(), XYSO_xBCDxFGH());
    /*_p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(reg.sfetch_counter.TOXE_SFETCH_S0p.qp_new());
    /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(reg.sfetch_control.TYFO_SFETCH_S0p_D1.qp_new(), TYTU_SFETCH_S0n);
    /*_p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(TEPA_RENDERINGp, reg.sfetch_counter.TULY_SFETCH_S1p.qp_new(), reg.sfetch_counter.TESE_SFETCH_S2p.qp_new());
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
    /*_p29.DEZY*/ reg.sprite_scanner.DEZY_COUNT_CLKp.dff17_any(ZEME_AxCxExGx(), XAPO_VID_RSTn(), DYTY_COUNT_CLKp);

    // There's a feedback loop here, but we don't actually need to loop - BAKY holds the clock line high once the sprite store is full, so doing a second logic pass
    // doesn't actually change any of the dffs.
    /*#p29.BAKY*/ wire BAKY_SPRITES_FULL_new = and2(reg.sprite_counter.CUXY_SPRITE_COUNT1.qp_any(), reg.sprite_counter.DYBE_SPRITE_COUNT3.qp_any());
    /*#p29.CAKE*/ wire CAKE_COUNT_CLKp_new = or2(BAKY_SPRITES_FULL_new, reg.sprite_scanner.DEZY_COUNT_CLKp.qp_any());
    /*#p28.AZYB*/ wire AZYB_LINE_TRIGn = not1(reg.ATEJ_LINE_RSTp.out_new());
    /*_p29.BESE*/ reg.sprite_counter.BESE_SPRITE_COUNT0.dff17_any(CAKE_COUNT_CLKp_new, AZYB_LINE_TRIGn, reg.sprite_counter.BESE_SPRITE_COUNT0.qn_any());
    /*_p29.CUXY*/ reg.sprite_counter.CUXY_SPRITE_COUNT1.dff17_any(reg.sprite_counter.BESE_SPRITE_COUNT0.qn_any(), AZYB_LINE_TRIGn, reg.sprite_counter.CUXY_SPRITE_COUNT1.qn_any());
    /*_p29.BEGO*/ reg.sprite_counter.BEGO_SPRITE_COUNT2.dff17_any(reg.sprite_counter.CUXY_SPRITE_COUNT1.qn_any(), AZYB_LINE_TRIGn, reg.sprite_counter.BEGO_SPRITE_COUNT2.qn_any());
    /*_p29.DYBE*/ reg.sprite_counter.DYBE_SPRITE_COUNT3.dff17_any(reg.sprite_counter.BEGO_SPRITE_COUNT2.qn_any(), AZYB_LINE_TRIGn, reg.sprite_counter.DYBE_SPRITE_COUNT3.qn_any());

    /*_p28.ABAK*/ wire ABAK_LINE_RSTp = or2(reg.ATEJ_LINE_RSTp.out_new(), AMYG_VID_RSTp());
    /*_p28.BYVA*/ wire BYVA_LINE_RSTn = not1(ABAK_LINE_RSTp);

    //update_sprite_reset_flags(sfetch_control.WUTY_SFETCH_DONE_TRIGp, BYVA_LINE_RSTn, sprite_match_flags, sprite_reset_flags);

    /*_p29.EBOJ*/ reg.sprite_reset_flags.EBOJ_STORE0_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GUVA_SPRITE0_GETp.out_old());
    /*_p29.CEDY*/ reg.sprite_reset_flags.CEDY_STORE1_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.ENUT_SPRITE1_GETp.out_old());
    /*_p29.EGAV*/ reg.sprite_reset_flags.EGAV_STORE2_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.EMOL_SPRITE2_GETp.out_old());
    /*_p29.GOTA*/ reg.sprite_reset_flags.GOTA_STORE3_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GYFY_SPRITE3_GETp.out_old());
    /*_p29.XUDY*/ reg.sprite_reset_flags.XUDY_STORE4_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GONO_SPRITE4_GETp.out_old());
    /*_p29.WAFY*/ reg.sprite_reset_flags.WAFY_STORE5_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GEGA_SPRITE5_GETp.out_old());
    /*_p29.WOMY*/ reg.sprite_reset_flags.WOMY_STORE6_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.XOJA_SPRITE6_GETp.out_old());
    /*_p29.WAPO*/ reg.sprite_reset_flags.WAPO_STORE7_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GUTU_SPRITE7_GETp.out_old());
    /*_p29.EXUQ*/ reg.sprite_reset_flags.EXUQ_STORE8_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.FOXA_SPRITE8_GETp.out_old());
    /*_p29.FONO*/ reg.sprite_reset_flags.FONO_STORE9_RSTp.dff17(reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags.GUZE_SPRITE9_GETp.out_old());

    SpriteStoreFlags sprite_store_flags_old = reg_old.sprite_store_flags;

    update_sprite_store_flags_gates(reg.sprite_counter, DYTY_COUNT_CLKp, reg.sprite_store_flags);
    store_sprite_gates(sprite_store_flags_old, reg.sprite_store_flags, reg.sprite_reset_flags, BYVA_LINE_RSTn, oam_temp_b_old);
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
    /*#p27.RONE*/ wire RONE_SCX_FINE_MATCHn_old = nand4(reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any(), SUHA_SCX_FINE_MATCHp_old, SYBY_SCX_FINE_MATCHp_old, SOZU_SCX_FINE_MATCHp_old);
    /*#p27.POHU*/ wire POHU_SCX_FINE_MATCHp_old = not1(RONE_SCX_FINE_MATCHn_old);

    /*_p29.CEHA*/ wire CEHA_SCANNINGp = not1(reg.sprite_scanner.CENO_SCANNINGn.qn_new());
    /*#p29.BYJO*/ wire BYJO_SCANNINGn = not1(CEHA_SCANNINGp);
    /*#p29.AZEM*/ wire AZEM_RENDERINGp = and2(reg.XYMU_RENDERINGn.qn_new(), BYJO_SCANNINGn);
    /*#p29.AROR*/ wire AROR_MATCH_ENp = and2(AZEM_RENDERINGp, reg.reg_lcdc.XYLO_LCDC_SPENn.qn_new());

    /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(reg.FEPO_STORE_MATCHp.out_mid(), reg_old.WODU_HBLANKp.out_old(), MYVO_AxCxExGx());
    /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, reg.tfetch_control.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
    /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);

    /*#p27.NYZE*/ reg.fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17_any(MOXE_AxCxExGx(), reg.XYMU_RENDERINGn.qn_new(), reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any());
    /*#p27.PUXA*/ reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17_any(ROXO_CLKPIPE_odd, reg.XYMU_RENDERINGn.qn_new(), POHU_SCX_FINE_MATCHp_old);

    /*#p27.POVA*/ wire POVA_FINE_MATCH_TRIGp = and2(reg.fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any(), reg.fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_any());
    /*#p27.ROXY*/ reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(PAHA_RENDERINGn, POVA_FINE_MATCH_TRIGp);

    /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, reg.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any());
    // Pixel counter, has carry lookahead because this can increment every tcycle
    /*_p21.RYBO*/ wire RYBO_old = xor2(reg_old.pix_count.XEHO_PX0p.qp_any(), reg_old.pix_count.SAVY_PX1p.qp_any()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /*_p21.XUKE*/ wire XUKE_old = and2(reg_old.pix_count.XEHO_PX0p.qp_any(), reg_old.pix_count.SAVY_PX1p.qp_any());
    /*_p21.XYLE*/ wire XYLE_old = and2(reg_old.pix_count.XODU_PX2p.qp_any(), XUKE_old);
    /*_p21.XEGY*/ wire XEGY_old = xor2(reg_old.pix_count.XODU_PX2p.qp_any(), XUKE_old); // feet facing gnd
    /*_p21.XORA*/ wire XORA_old = xor2(reg_old.pix_count.XYDO_PX3p.qp_any(), XYLE_old); // feet facing gnd

    /*_p21.XEHO*/ reg.pix_count.XEHO_PX0p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, reg.pix_count.XEHO_PX0p.qn_any());
    /*_p21.SAVY*/ reg.pix_count.SAVY_PX1p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, RYBO_old);
    /*_p21.XODU*/ reg.pix_count.XODU_PX2p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEGY_old);
    /*_p21.XYDO*/ reg.pix_count.XYDO_PX3p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XORA_old);

    /*_p24.TOCA*/ wire TOCA_new = not1(reg.pix_count.XYDO_PX3p.qp_any());
    /*_p21.SAKE*/ wire SAKE_old = xor2(reg_old.pix_count.TUHU_PX4p.qp_any(), reg_old.pix_count.TUKY_PX5p.qp_any());
    /*_p21.TYBA*/ wire TYBA_old = and2(reg_old.pix_count.TUHU_PX4p.qp_any(), reg_old.pix_count.TUKY_PX5p.qp_any());
    /*_p21.SURY*/ wire SURY_old = and2(reg_old.pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /*_p21.TYGE*/ wire TYGE_old = xor2(reg_old.pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /*_p21.ROKU*/ wire ROKU_old = xor2(reg_old.pix_count.SYBE_PX7p.qp_any(), SURY_old);

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
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, reg.tfetch_control.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
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

  tock_window_gates(reg_old, SEGU_CLKPIPE_evn, REPU_VBLANKp);

  //----------------------------------------
  // Tile fetch sequencer

  /*_p27.LYZU*/ reg.tfetch_control.LYZU_BFETCH_S0p_D1.dff17(ALET_xBxDxFxH(), reg.XYMU_RENDERINGn.qn_new(), reg_old.tfetch_counter.LAXU_BFETCH_S0p.qp_old());

  /*_p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp = nor2(reg.win_ctrl.RYFA_WIN_FETCHn_A.qn_new(), reg.win_ctrl.RENE_WIN_FETCHn_B.qp_new());
  /*_p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne = nand2(SYLO_WIN_HITn, reg.win_ctrl.SOVY_WIN_HITp.qp_new());
  /*_p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne = not1(TUXY_WIN_FIRST_TILEne);
  /*_p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp = or3(SEKO_WIN_FETCH_TRIGp, SUZU_WIN_FIRST_TILEne, TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  /*_p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp.out_new(), MOSU_WIN_MODE_TRIGp, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(reg.tfetch_counter.LAXU_BFETCH_S0p.qp_any(), reg.tfetch_counter.NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn);
    /*_p27.LEBO*/ wire LEBO_AxCxExGx = nand2(ALET_xBxDxFxH(), MOCE_BFETCH_DONEn);

    /*_p27.LAXU*/ reg.tfetch_counter.LAXU_BFETCH_S0p.dff17_any(LEBO_AxCxExGx,                               NYXU_BFETCH_RSTn, reg.tfetch_counter.LAXU_BFETCH_S0p.qn_any());
    /*_p27.MESU*/ reg.tfetch_counter.MESU_BFETCH_S1p.dff17_any(reg.tfetch_counter.LAXU_BFETCH_S0p.qn_any(), NYXU_BFETCH_RSTn, reg.tfetch_counter.MESU_BFETCH_S1p.qn_any());
    /*_p27.NYVA*/ reg.tfetch_counter.NYVA_BFETCH_S2p.dff17_any(reg.tfetch_counter.MESU_BFETCH_S1p.qn_any(), NYXU_BFETCH_RSTn, reg.tfetch_counter.NYVA_BFETCH_S2p.qn_any());
  }

  /*_p27.LOVY*/ reg.tfetch_control.LOVY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, reg_old.tfetch_control.LYRY_BFETCH_DONEp.out_old());

  /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(reg.tfetch_counter.LAXU_BFETCH_S0p.qp_new(), reg.tfetch_counter.NYVA_BFETCH_S2p.qp_new(), NYXU_BFETCH_RSTn);
  /*_p27.LYRY*/ reg.tfetch_control.LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  /*_p27.LURY*/ wire LURY_BG_FETCH_DONEn = and2(reg.tfetch_control.LOVY_FETCH_DONEp.qn_new(), reg.XYMU_RENDERINGn.qn_new());
  /*_p27.LONY*/ reg.tfetch_control.LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, LURY_BG_FETCH_DONEn);


  //----------------------------------------
  // Fine match counter

  /*#p27.PASO*/ wire PASO_FINE_RST = nor2(PAHA_RENDERINGn, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p27.ROZE*/ wire ROZE_FINE_COUNT_7n = nand3(reg.fine_count.RUBU_FINE_CNT2.qp_any(), reg.fine_count.ROGA_FINE_CNT1.qp_any(), reg.fine_count.RYKU_FINE_CNT0.qp_any());
    /*#p27.PECU*/ wire PECU_FINE_CLK = nand2(ROXO_CLKPIPE_odd, ROZE_FINE_COUNT_7n);
    /*#p27.RYKU*/ reg.fine_count.RYKU_FINE_CNT0.dff17_any(PECU_FINE_CLK, PASO_FINE_RST, reg.fine_count.RYKU_FINE_CNT0.qn_any());
    /*#p27.ROGA*/ reg.fine_count.ROGA_FINE_CNT1.dff17_any(reg.fine_count.RYKU_FINE_CNT0.qn_any(), PASO_FINE_RST, reg.fine_count.ROGA_FINE_CNT1.qn_any());
    /*#p27.RUBU*/ reg.fine_count.RUBU_FINE_CNT2.dff17_any(reg.fine_count.ROGA_FINE_CNT1.qn_any(), PASO_FINE_RST, reg.fine_count.RUBU_FINE_CNT2.qn_any());
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





















































































































//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_logic(const blob& cart_blob) {
  //const GateBoyState state_old = state;
  //GateBoyState& state_new = state;

  const GateBoyReg reg_old = reg;
  GateBoyReg& reg_new = reg;


  uint8_t phase_old = 1 << (7 - ((sys.phase_total + 0) & 7));
  uint8_t phase_new = 1 << (7 - ((sys.phase_total + 1) & 7));

  //----------

  reg_new.SIG_VCC = 1;
  reg_new.SIG_GND = 0;

  //-----------------------------------------------------------------------------

  uint16_t cpu_addr_old = (uint16_t)bit_pack(reg_old.cpu_abus);
  bool cpu_addr_vram_old = (cpu_addr_old >= 0x8000) && (cpu_addr_old <= 0x9FFF);
  bool cpu_addr_oam_old = (cpu_addr_old >= 0xFE00) && (cpu_addr_old <= 0xFEFF);

  bit_unpack(reg_new.cpu_abus, 0xFFFF);
  bit_unpack(reg_new.cpu_dbus, 0xFF);

  if (gen_clk_new(0b00001111)) {
    // Data has to be driven on EFGH or we fail the wave tests
    if (cpu.bus_req_new.write) bit_unpack(reg_new.cpu_dbus, cpu.bus_req_new.data_lo);
    reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE = cpu.bus_req_new.read;
  }
  else {
    reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE = 0;
  }

  if (gen_clk_new(0b10000000)) {
    reg_new.cpu_signals.SIG_IN_CPU_RDp = 0;
    reg_new.cpu_signals.SIG_IN_CPU_WRp = 0;
    bit_unpack(reg_new.cpu_abus, cpu.bus_req_new.addr & 0x00FF);
  }
  else {
    reg_new.cpu_signals.SIG_IN_CPU_RDp = cpu.bus_req_new.read;
    reg_new.cpu_signals.SIG_IN_CPU_WRp = cpu.bus_req_new.write;
    bit_unpack(reg_new.cpu_abus, cpu.bus_req_new.addr);
  }

  bool EXT_addr_new = (cpu.bus_req_new.read || cpu.bus_req_new.write);
  if ((cpu.bus_req_new.addr >= 0x8000) && (cpu.bus_req_new.addr < 0x9FFF) && gen_clk_new(0b10000000)) EXT_addr_new = false;
  if ((cpu.bus_req_new.addr >= 0xFE00)) EXT_addr_new = false;
  if ((cpu.bus_req_new.addr <= 0x00FF) && !reg_new.cpu_signals.TEPU_BOOT_BITn) EXT_addr_new = false;
  reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp = EXT_addr_new;

  uint16_t cpu_addr_new = (uint16_t)bit_pack(reg_new.cpu_abus);

  bool cpu_addr_vram_new = (cpu_addr_new >= 0x8000) && (cpu_addr_new <= 0x9FFF);
  bool cpu_addr_ram_new = (cpu_addr_new >= 0xA000) && (cpu_addr_new <= 0xFDFF);
  bool cpu_addr_oam_new = (cpu_addr_new >= 0xFE00) && (cpu_addr_new <= 0xFEFF);

  //-----------------------------------------------------------------------------

  reg_new.sys_clk.PIN_74_CLK.CLK = gen_clk_new(0b10101010);
  reg_new.sys_clk.PIN_74_CLK.CLKGOOD = 1;

  reg_new.sys_rst.PIN_71_RST = 0;
  reg_new.sys_rst.PIN_76_T2 = 0;
  reg_new.sys_rst.PIN_77_T1 = 0;

  reg_new.cpu_ack.SIG_CPU_ACK_VBLANK = get_bit(cpu.core.int_ack, BIT_VBLANK);
  reg_new.cpu_ack.SIG_CPU_ACK_STAT   = get_bit(cpu.core.int_ack, BIT_STAT);
  reg_new.cpu_ack.SIG_CPU_ACK_TIMER  = get_bit(cpu.core.int_ack, BIT_TIMER);
  reg_new.cpu_ack.SIG_CPU_ACK_SERIAL = get_bit(cpu.core.int_ack, BIT_SERIAL);
  reg_new.cpu_ack.SIG_CPU_ACK_JOYPAD = get_bit(cpu.core.int_ack, BIT_JOYPAD);

  reg_new.sys_clk.SIG_CPU_CLKREQ = 1;

  reg_new.cpu_signals.SIG_CPU_ADDR_HIp = cpu_addr_new >= 0xFE00 && cpu_addr_new <= 0xFFFF;
  reg_new.cpu_signals.SIG_CPU_UNOR_DBG = 0;
  reg_new.cpu_signals.SIG_CPU_UMUT_DBG = 0;

  //-----------------------------------------------------------------------------
  // Sys clock signals

  reg_new.sys_clk.PIN_73_CLK_DRIVE = reg_new.sys_clk.PIN_74_CLK.CLK;
  reg_new.sys_clk.AVET_DEGLITCH = reg_new.sys_clk.PIN_74_CLK.CLK;
  reg_new.sys_clk.ANOS_DEGLITCH = !reg_new.sys_clk.PIN_74_CLK.CLK;

  reg_new.sys_clk.AFUR_xxxxEFGH = gen_clk_new(0b00001111);
  reg_new.sys_clk.ALEF_AxxxxFGH = gen_clk_new(0b10000111);
  reg_new.sys_clk.APUK_ABxxxxGH = gen_clk_new(0b11000011);
  reg_new.sys_clk.ADYK_ABCxxxxH = gen_clk_new(0b11100001);

  reg_new.sys_clk.PIN_75_CLK_OUT = gen_clk_new(0b00001111);

  reg_new.sys_clk.SIG_CPU_BOWA_Axxxxxxx = gen_clk_new(0b10000000);
  reg_new.sys_clk.SIG_CPU_BEDO_xBCDEFGH = gen_clk_new(0b01111111);
  reg_new.sys_clk.SIG_CPU_BEKO_ABCDxxxx = gen_clk_new(0b11110000);
  reg_new.sys_clk.SIG_CPU_BUDE_xxxxEFGH = gen_clk_new(0b00001111);
  reg_new.sys_clk.SIG_CPU_BOLO_ABCDEFxx = gen_clk_new(0b11111100);
  reg_new.sys_clk.SIG_CPU_BUKE_AxxxxxGH = gen_clk_new(0b10000011);
  reg_new.sys_clk.SIG_CPU_BOMA_xBCDEFGH = gen_clk_new(0b01111111);
  reg_new.sys_clk.SIG_CPU_BOGA_Axxxxxxx = gen_clk_new(0b10000000);

  reg_new.cpu_signals.TEDO_CPU_RDp = reg_new.cpu_signals.SIG_IN_CPU_RDp;
  reg_new.cpu_signals.APOV_CPU_WRp = gen_clk_new(0b00001110) && reg_new.cpu_signals.SIG_IN_CPU_WRp;
  reg_new.cpu_signals.TAPU_CPU_WRp = reg_new.cpu_signals.APOV_CPU_WRp;
  reg_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK = gen_clk_new(0b00111111) && reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp;

  //----------
  // DIV

  if (gen_clk_new(0b10000000)) bit_unpack(reg_new.reg_div, bit_pack(reg_new.reg_div) + 1);
  if (cpu_addr_new == 0xFF04 && reg_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00001110)) bit_unpack(reg_new.reg_div, 0);
  if (cpu_addr_new == 0xFF04 && reg_new.cpu_signals.SIG_IN_CPU_RDp) bit_unpack(reg_new.cpu_dbus, bit_pack(reg_new.reg_div) >> 6);

  //----------
  // In logic mode we don't care about the power-on behavior, we only want behavior to match when running code. So, we set
  // this stuff to zeroes.

  reg_new.sys_rst.AFER_SYS_RSTp = 0;
  reg_new.sys_rst.TUBO_WAITINGp = 0;
  reg_new.sys_rst.ASOL_POR_DONEn = 0;
  reg_new.sys_rst.SIG_CPU_EXT_CLKGOOD = 1;
  reg_new.sys_rst.SIG_CPU_EXT_RESETp = 0;
  reg_new.sys_rst.SIG_CPU_STARTp = 0;
  reg_new.sys_rst.SIG_CPU_INT_RESETp = 0;;
  reg_new.sys_rst.SOTO_DBG_VRAMp = 0;

  //----------
  // LCDC
  // has to be near the top as it controls the video reset signal

  if (reg_new.cpu_signals.SIG_IN_CPU_WRp && cpu_addr_new == 0xFF40 && gen_clk_new(0b00000001)) {
    bit_unpack_inv(reg_new.reg_lcdc, bit_pack(reg_old.cpu_dbus));
  }

  if (reg_new.cpu_signals.SIG_IN_CPU_RDp && (cpu_addr_new == 0xFF40)) {
    bit_unpack(reg_new.cpu_dbus, bit_pack_inv(reg_new.reg_lcdc));
  }

  //----------
  // Video clocks

  reg_new.sys_clk.WOSU_AxxDExxH = !reg_new.reg_lcdc.XONA_LCDC_LCDENn && gen_clk_new(0b10011001);
  reg_new.sys_clk.WUVU_ABxxEFxx = !reg_new.reg_lcdc.XONA_LCDC_LCDENn && gen_clk_new(0b11001100);
  reg_new.sys_clk.VENA_xxCDEFxx = !reg_new.reg_lcdc.XONA_LCDC_LCDENn && gen_clk_new(0b00111100);

  //----------
  // LYC

  if (cpu_addr_new == 0xFF45) {
    if (reg_new.cpu_signals.SIG_IN_CPU_RDp) bit_unpack(reg_new.cpu_dbus, bit_pack_inv(reg_old.reg_lyc));
    if (reg_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00000001)) bit_unpack_inv(reg_new.reg_lyc, bit_pack(reg_old.cpu_dbus));
  }

  if (!reg_new.reg_lcdc.XONA_LCDC_LCDENn && gen_clk_new(0b00100000)) {
    reg_new.int_ctrl.ROPO_LY_MATCH_SYNCp = bit_pack(reg_old.reg_ly) == bit_pack_inv(reg_old.reg_lyc);
  }

  //----------
  /// LX, LY, lcd flags

  if (reg_new.reg_lcdc.XONA_LCDC_LCDENn) {
    reg_new.lcd.ANEL_x113p = 0;
    reg_new.lcd.CATU_x113p = 0;
    reg_new.lcd.NYPE_x113p = 0;
    reg_new.lcd.RUTU_x113p = 0;
    reg_new.lcd.POPU_y144p = 0;
    reg_new.lcd.MYTA_y153p = 0;
    reg_new.lcd.SYGU_LINE_STROBE = 0;
    reg_new.ATEJ_LINE_RSTp = 1;
    bit_unpack(reg_new.reg_lx, 0);
    bit_unpack(reg_new.reg_ly, 0);
  }
  else {
    wire ly_144_old = (bit_pack(reg_old.reg_ly) & 144) == 144;
    wire ly_153_old = (bit_pack(reg_old.reg_ly) & 153) == 153;

    if (gen_clk_new(0b10000000)) {
      reg_new.lcd.CATU_x113p = reg_new.lcd.RUTU_x113p && !ly_144_old;
    }

    if (gen_clk_new(0b00100000)) {
      reg_new.lcd.ANEL_x113p = reg_old.lcd.CATU_x113p;
      reg_new.lcd.NYPE_x113p = reg_old.lcd.RUTU_x113p;

      if (!reg_old.lcd.NYPE_x113p && reg_new.lcd.NYPE_x113p) {
        reg_new.lcd.POPU_y144p = ly_144_old;
        reg_new.lcd.MYTA_y153p = ly_153_old;
      }

      bit_unpack(reg_new.reg_lx, bit_pack(reg_old.reg_lx) + 1);
    }

    if (gen_clk_new(0b00001000)) {
      reg_new.lcd.CATU_x113p = reg_new.lcd.RUTU_x113p && !ly_144_old;
    }

    if (gen_clk_new(0b00000010)) {
      reg_new.lcd.ANEL_x113p = reg_old.lcd.CATU_x113p;
      reg_new.lcd.RUTU_x113p = bit_pack(reg_old.reg_lx) == 113;

      if (!reg_old.lcd.RUTU_x113p && reg_new.lcd.RUTU_x113p) {
        bit_unpack(reg_new.reg_ly, bit_pack(reg_old.reg_ly) + 1);
      }

      uint8_t lx_old = (uint8_t)bit_pack(reg_old.reg_lx);
      reg_new.lcd.SYGU_LINE_STROBE = (lx_old == 0) || (lx_old == 7) || (lx_old == 45) || (lx_old == 83);
    }

    reg_new.ATEJ_LINE_RSTp = !reg_new.lcd.ANEL_x113p && reg_new.lcd.CATU_x113p;
    if (reg_new.lcd.RUTU_x113p) bit_unpack(reg_new.reg_lx, 0);
    if (reg_new.lcd.MYTA_y153p) bit_unpack(reg_new.reg_ly, 0);
  }

  if (reg_new.cpu_signals.SIG_IN_CPU_RDp && (cpu_addr_new == 0xFF44)) {
    bit_unpack(reg_new.cpu_dbus, bit_pack(reg_new.reg_ly));
  }

  //----------

  if (reg_new.cpu_signals.SIG_IN_CPU_WRp && cpu_addr_new == 0xFF00 && gen_clk_new(0b00000001)) {
    reg_new.reg_joy.KELY_JOYP_UDLRp = reg_old.cpu_dbus.BUS_CPU_D04p;
    reg_new.reg_joy.COFY_JOYP_ABCSp = reg_old.cpu_dbus.BUS_CPU_D05p;

    reg_new.joy_ext.PIN_63_JOY_P14 = !reg_new.reg_joy.KELY_JOYP_UDLRp;
    reg_new.joy_ext.PIN_62_JOY_P15 = !reg_new.reg_joy.COFY_JOYP_ABCSp;
  }

  bool EXT_button0 = 0, EXT_button1 = 0, EXT_button2 = 0, EXT_button3 = 0;

  if (reg_new.joy_ext.PIN_63_JOY_P14) {
    EXT_button0 = get_bit(sys.buttons, 0); // RIGHT
    EXT_button1 = get_bit(sys.buttons, 1); // LEFT
    EXT_button2 = get_bit(sys.buttons, 2); // UP
    EXT_button3 = get_bit(sys.buttons, 3); // DOWN
  }
  else if (reg_new.joy_ext.PIN_62_JOY_P15) {
    EXT_button0 = get_bit(sys.buttons, 4); // A
    EXT_button1 = get_bit(sys.buttons, 5); // B
    EXT_button2 = get_bit(sys.buttons, 6); // SELECT
    EXT_button3 = get_bit(sys.buttons, 7); // START
  }

  reg_new.joy_ext.PIN_67_JOY_P10 = EXT_button0;
  reg_new.joy_ext.PIN_66_JOY_P11 = EXT_button1;
  reg_new.joy_ext.PIN_65_JOY_P12 = EXT_button2;
  reg_new.joy_ext.PIN_64_JOY_P13 = EXT_button3;

  wire any_button = EXT_button0 || EXT_button1 || EXT_button2 || EXT_button3;

  if (gen_clk_new(0b10000000)) {
    reg_new.int_ctrl.AWOB_WAKE_CPU = !any_button;
    reg_new.int_ctrl.SIG_CPU_WAKE = !any_button;
  }

  if (gen_clk_new(0b10000000)) {
    reg_new.joy_int.APUG_JP_GLITCH3 = reg_new.joy_int.AGEM_JP_GLITCH2;
    reg_new.joy_int.AGEM_JP_GLITCH2 = reg_new.joy_int.ACEF_JP_GLITCH1;
    reg_new.joy_int.ACEF_JP_GLITCH1 = reg_new.joy_int.BATU_JP_GLITCH0;
    reg_new.joy_int.BATU_JP_GLITCH0 = !any_button;
  }

  if (cpu_addr_new == 0xFF00 && reg_new.cpu_signals.SIG_IN_CPU_RDp) {
    reg_new.cpu_dbus.BUS_CPU_D00p = !reg_new.joy_latch.KEVU_JOYP_L0n;
    reg_new.cpu_dbus.BUS_CPU_D01p = !reg_new.joy_latch.KAPA_JOYP_L1n;
    reg_new.cpu_dbus.BUS_CPU_D02p = !reg_new.joy_latch.KEJA_JOYP_L2n;
    reg_new.cpu_dbus.BUS_CPU_D03p = !reg_new.joy_latch.KOLO_JOYP_L3n;
    reg_new.cpu_dbus.BUS_CPU_D04p =  reg_new.reg_joy.KELY_JOYP_UDLRp;
    reg_new.cpu_dbus.BUS_CPU_D05p =  reg_new.reg_joy.COFY_JOYP_ABCSp;
  }
  else {
    reg_new.joy_latch.KEVU_JOYP_L0n = reg_new.joy_ext.PIN_67_JOY_P10;
    reg_new.joy_latch.KAPA_JOYP_L1n = reg_new.joy_ext.PIN_66_JOY_P11;
    reg_new.joy_latch.KEJA_JOYP_L2n = reg_new.joy_ext.PIN_65_JOY_P12;
    reg_new.joy_latch.KOLO_JOYP_L3n = reg_new.joy_ext.PIN_64_JOY_P13;
  }

  //tock_serial_logic();
  //tock_timer_logic();

  if (reg_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00000001)) {
    if (cpu_addr_new == 0xFF06) bit_unpack(reg_new.reg_tma, bit_pack(reg_new.cpu_dbus));
    if (cpu_addr_new == 0xFF07) bit_unpack(reg_new.reg_tac, bit_pack(reg_new.cpu_dbus));
  }

  if (gen_clk_new(0b10000000)) {
    reg_new.int_ctrl.MOBA_TIMER_OVERFLOWp = !reg_old.reg_tima.NUGA_TIMA7p && reg_old.int_ctrl.NYDU_TIMA7p_DELAY;
    reg_new.int_ctrl.NYDU_TIMA7p_DELAY = reg_old.reg_tima.NUGA_TIMA7p;
  }

  {
    wire UKAP_CLK_MUXa_new = reg_new.reg_tac.SOPU_TAC0p ? reg_new.reg_div.TAMA_DIV05p : reg_new.reg_div.TERO_DIV03p;
    wire TEKO_CLK_MUXb_new = reg_new.reg_tac.SOPU_TAC0p ? reg_new.reg_div.UFOR_DIV01p : reg_new.reg_div.TULU_DIV07p;
    wire TECY_CLK_MUXc_new = reg_new.reg_tac.SAMY_TAC1p ? UKAP_CLK_MUXa_new : TEKO_CLK_MUXb_new;
    wire SOGU_TIMA_CLKn_new = TECY_CLK_MUXc_new && reg_new.reg_tac.SABO_TAC2p;

    bool UKAP_CLK_MUXa_old = reg_old.reg_tac.SOPU_TAC0p ? reg_old.reg_div.TAMA_DIV05p : reg_old.reg_div.TERO_DIV03p;
    bool TEKO_CLK_MUXb_old = reg_old.reg_tac.SOPU_TAC0p ? reg_old.reg_div.UFOR_DIV01p : reg_old.reg_div.TULU_DIV07p;
    bool TECY_CLK_MUXc_old = reg_old.reg_tac.SAMY_TAC1p ? UKAP_CLK_MUXa_old : TEKO_CLK_MUXb_old;
    bool SOGU_TIMA_CLKn_old = TECY_CLK_MUXc_old && reg_old.reg_tac.SABO_TAC2p;

    if (SOGU_TIMA_CLKn_old && !SOGU_TIMA_CLKn_new) {
      bit_unpack(reg_new.reg_tima, bit_pack(reg_new.reg_tima) + 1);
    }
  }

  if (cpu_addr_new == 0xFF05 && gen_clk_new(0b00001110) && reg_new.cpu_signals.SIG_IN_CPU_WRp) {
    if (!reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE || reg_new.int_ctrl.MOBA_TIMER_OVERFLOWp) {
      reg_new.int_ctrl.NYDU_TIMA7p_DELAY = 0;
      bit_unpack(reg_new.reg_tima, bit_pack(reg_new.cpu_dbus));
    }
  }
  else {
    if (reg_new.int_ctrl.MOBA_TIMER_OVERFLOWp) {
      reg_new.int_ctrl.NYDU_TIMA7p_DELAY = 0;
      bit_unpack(reg_new.reg_tima, bit_pack(reg_new.reg_tma));
    }
  }


  if (reg_new.cpu_signals.SIG_IN_CPU_RDp) {
    if (cpu_addr_new == 0xFF05) bit_unpack(reg_new.cpu_dbus, bit_pack(reg_new.reg_tima));
    if (cpu_addr_new == 0xFF06) bit_unpack(reg_new.cpu_dbus, bit_pack(reg_new.reg_tma));
    if (cpu_addr_new == 0xFF07) bit_unpack(reg_new.cpu_dbus, bit_pack(reg_new.reg_tac) | 0b11111000);
  }

  if (reg_new.cpu_signals.SIG_IN_CPU_WRp && cpu_addr_new == 0xFF50 && gen_clk_new(0b00000001)) {
    reg_new.cpu_signals.TEPU_BOOT_BITn = reg_new.SATO_BOOT_BITn;
  }

  reg_new.cpu_signals.SIG_CPU_BOOTp = 0;
  reg_new.cpu_signals.SIG_BOOT_CSp = 0;

  if (cpu_addr_new <= 0x00FF) {

    reg_new.cpu_signals.SIG_CPU_BOOTp = !reg_new.cpu_signals.TEPU_BOOT_BITn;

    if (reg_new.cpu_signals.SIG_IN_CPU_RDp && !reg_new.cpu_signals.TEPU_BOOT_BITn) {
      reg_new.cpu_signals.SIG_BOOT_CSp = 1;
      bit_unpack(reg_new.cpu_dbus, DMG_ROM_blob[cpu_addr_new & 0xFF]);
    }
  }

  if (reg_new.cpu_signals.SIG_IN_CPU_RDp && (cpu_addr_new == 0xFF50)) {
    reg_new.cpu_dbus.BUS_CPU_D00p = reg_new.cpu_signals.TEPU_BOOT_BITn;
  }

  reg_new.SATO_BOOT_BITn = reg_new.cpu_dbus.BUS_CPU_D00p || reg_new.cpu_signals.TEPU_BOOT_BITn;

  //----------------------------------------
  // dma

  if (cpu_addr_new == 0xFF46 && reg_new.cpu_signals.SIG_IN_CPU_RDp) {
    bit_unpack(reg_new.cpu_dbus, bit_pack_inv(reg_old.reg_dma));
  }

  if (gen_clk_new(0b10000000)) {
    reg_new.dma_ctrl.LUVY_DMA_TRIG_d0 = reg_new.dma_ctrl.LYXE_DMA_LATCHp;
    reg_new.MATU_DMA_RUNNINGp = reg_new.dma_ctrl.LOKY_DMA_LATCHp;

    if (reg_new.dma_ctrl.LOKY_DMA_LATCHp && !reg_new.dma_ctrl.LENE_DMA_TRIG_d4) {
      bit_unpack(reg_new.dma_lo, bit_pack(reg_old.dma_lo) + 1);
    }
  }
  else if (gen_clk_new(0b00001000)) {
    if (cpu_addr_new == 0xFF46 && reg_new.cpu_signals.SIG_IN_CPU_WRp) {
      reg_new.dma_ctrl.LYXE_DMA_LATCHp.state = 1;
    }

    if (bit_pack(reg_old.dma_lo) == 159) {
      reg_new.dma_ctrl.MYTE_DMA_DONE = 1;
      reg_new.dma_ctrl.LARA_DMA_LATCHn = 1;
      reg_new.dma_ctrl.LOKY_DMA_LATCHp = 0;
    }

    reg_new.dma_ctrl.LENE_DMA_TRIG_d4 = reg_old.dma_ctrl.LUVY_DMA_TRIG_d0;

    if (reg_new.dma_ctrl.LUVY_DMA_TRIG_d0) {
      reg_new.dma_ctrl.MYTE_DMA_DONE = 0;
      reg_new.dma_ctrl.LYXE_DMA_LATCHp = 0;
      bit_unpack(reg_new.dma_lo, 0);
      reg_new.dma_ctrl.LARA_DMA_LATCHn = 0;
      reg_new.dma_ctrl.LOKY_DMA_LATCHp = 1;
    }
  }
  else if (gen_clk_new(0b00000001)) {
    if (cpu_addr_new == 0xFF46 && reg_new.cpu_signals.SIG_IN_CPU_WRp) {
      bit_unpack_inv(reg_new.reg_dma, bit_pack(reg_old.cpu_dbus));
    }
  }

  auto dma_addr_new = (bit_pack_inv(reg_new.reg_dma) << 8) | bit_pack(reg_new.dma_lo);
  auto dma_addr_vram_new = reg_new.MATU_DMA_RUNNINGp && (dma_addr_new >= 0x8000) && (dma_addr_new <= 0x9FFF);

  //-----------------------
  // VID RST BRANCH

  if (reg_new.reg_lcdc.XONA_LCDC_LCDENn) {
    reg.sprite_scanner.DOBA_SCAN_DONE_Bp = 0;
    reg.sprite_scanner.BYBA_SCAN_DONE_Ap = 0;
    reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp = 0;
    reg.sprite_scanner.BESU_SCANNINGn = 0;
    reg.sprite_scanner.CENO_SCANNINGn = 0;
    bit_unpack(reg.scan_counter, 0);
    reg.sprite_scanner.FETO_SCAN_DONEp = 0;
    reg.VOGA_HBLANKp = 0;
    reg.XYMU_RENDERINGn = 1;

    if (gen_clk_new(0b01010101)) {
      reg.sfetch_control.SOBU_SFETCH_REQp.  state = reg.FEPO_STORE_MATCHp && !reg_old.win_ctrl.RYDY_WIN_HITp && reg.tfetch_control.LYRY_BFETCH_DONEp && !reg.sfetch_control.TAKA_SFETCH_RUNNINGp;
    }
    if (gen_clk_new(0b10101010)) {
      reg.sfetch_control.SUDA_SFETCH_REQp   = reg.sfetch_control.SOBU_SFETCH_REQp;
      reg.sfetch_control.TYFO_SFETCH_S0p_D1 = reg.sfetch_counter.TOXE_SFETCH_S0p;
    }

    reg.sfetch_counter.TOXE_SFETCH_S0p = 0;
    reg.sfetch_counter.TULY_SFETCH_S1p = 0;
    reg.sfetch_counter.TESE_SFETCH_S2p = 0;
    reg.sfetch_control.TOBU_SFETCH_S1p_D2 = 0;
    reg.sfetch_control.VONU_SFETCH_S1p_D4 = 0;
    reg.sfetch_control.SEBA_SFETCH_S1p_D5 = 0;

    reg.sfetch_control.TEXY_SFETCHINGp = 0;

    reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp = 0;

    reg.win_ctrl.NUNU_WIN_MATCHp = 0;
    reg.win_ctrl.NOPA_WIN_MODE_Bp = 0;
    reg.win_ctrl.PYNU_WIN_MODE_Ap = 0;
    reg.win_ctrl.SOVY_WIN_HITp = 0;
    reg.win_ctrl.RYDY_WIN_HITp = 0;
    reg.win_ctrl.PUKU_WIN_HITn = 1;

    reg.tfetch_control.PYGO_FETCH_DONEp = 0;
    reg.tfetch_control.PORY_FETCH_DONEp = 0;
    reg.tfetch_control.NYKA_FETCH_DONEp = 0;
    reg.tfetch_control.POKY_PRELOAD_LATCHp = 0;

    reg.sfetch_control.TAKA_SFETCH_RUNNINGp = 1;

    reg.sprite_scanner.DEZY_COUNT_CLKp = 0;

    bit_unpack(reg.sprite_counter, 0);
    bit_unpack(reg.sprite_reset_flags, 0);
    bit_unpack(reg.sprite_store_flags, 0);

    bit_unpack(reg.store_x0, 0xFF);
    bit_unpack(reg.store_x1, 0xFF);
    bit_unpack(reg.store_x2, 0xFF);
    bit_unpack(reg.store_x3, 0xFF);
    bit_unpack(reg.store_x4, 0xFF);
    bit_unpack(reg.store_x5, 0xFF);
    bit_unpack(reg.store_x6, 0xFF);
    bit_unpack(reg.store_x7, 0xFF);
    bit_unpack(reg.store_x8, 0xFF);
    bit_unpack(reg.store_x9, 0xFF);
  }

  //-----------------------
  // VID RUN BRANCH

  if (!reg_new.reg_lcdc.XONA_LCDC_LCDENn) {
    if (reg_new.ATEJ_LINE_RSTp) {
      reg.sprite_scanner.DOBA_SCAN_DONE_Bp = 0;
      reg.sprite_scanner.BYBA_SCAN_DONE_Ap = 0;
      reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp = 0;
      bit_unpack(reg.scan_counter, 0);
      reg.sprite_scanner.BESU_SCANNINGn = 1;
      reg_new.VOGA_HBLANKp = 0;
    }
    else {
      if (gen_clk_new(0b01010101)) {
        reg.sprite_scanner.DOBA_SCAN_DONE_Bp = reg_old.sprite_scanner.BYBA_SCAN_DONE_Ap;
        reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp = !reg.sprite_scanner.DOBA_SCAN_DONE_Bp && reg.sprite_scanner.BYBA_SCAN_DONE_Ap;
      }
      else if (gen_clk_new(0b10001000)) {
        reg.sprite_scanner.BYBA_SCAN_DONE_Ap = bit_pack(reg_old.scan_counter) == 39;
        reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp = !reg.sprite_scanner.DOBA_SCAN_DONE_Bp && reg.sprite_scanner.BYBA_SCAN_DONE_Ap;
        
        if (bit_pack(reg_old.scan_counter) != 39) {
          bit_unpack(reg.scan_counter, bit_pack(reg_old.scan_counter) + 1);
        }
      }

      if (reg.lcd.CATU_x113p) reg.sprite_scanner.BESU_SCANNINGn = 1;
      if (reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp) reg.sprite_scanner.BESU_SCANNINGn = 0;
    }

    if (gen_clk_new(0b01010101)) {
      reg_new.VOGA_HBLANKp = reg_old.WODU_HBLANKp;
      reg.sfetch_control.SOBU_SFETCH_REQp   = reg.FEPO_STORE_MATCHp && !reg_old.win_ctrl.RYDY_WIN_HITp && reg.tfetch_control.LYRY_BFETCH_DONEp && !reg.sfetch_control.TAKA_SFETCH_RUNNINGp;
      reg.sfetch_control.VONU_SFETCH_S1p_D4 = reg.sfetch_control.TOBU_SFETCH_S1p_D2;
      reg.sfetch_control.TOBU_SFETCH_S1p_D2 = reg.sfetch_counter.TULY_SFETCH_S1p;

      uint8_t sfetch_phase_old = pack(!(reg_old.sfetch_control.TYFO_SFETCH_S0p_D1 ^ reg_old.sfetch_counter.TOXE_SFETCH_S0p), reg_old.sfetch_counter.TOXE_SFETCH_S0p, reg_old.sfetch_counter.TULY_SFETCH_S1p, reg_old.sfetch_counter.TESE_SFETCH_S2p);
      if ((sfetch_phase_old >> 1) != 5) {
        bit_unpack(reg.sfetch_counter, (sfetch_phase_old >> 1) + 1);
      }

      if (reg.sfetch_control.SOBU_SFETCH_REQp && !reg.sfetch_control.SUDA_SFETCH_REQp) {
        bit_unpack(reg.sfetch_counter, 0);
      }
      reg.win_ctrl.NOPA_WIN_MODE_Bp = reg.win_ctrl.PYNU_WIN_MODE_Ap;
      reg.tfetch_control.PYGO_FETCH_DONEp = reg.tfetch_control.PORY_FETCH_DONEp;
      reg.tfetch_control.NYKA_FETCH_DONEp = reg.tfetch_control.LYRY_BFETCH_DONEp;

      reg.win_ctrl.SOVY_WIN_HITp = reg.win_ctrl.RYDY_WIN_HITp;
    }

    if (gen_clk_new(0b10101010)) {
      if (gen_clk_new(0b10001000)) {
        bit_unpack_inv(reg.sprite_index, bit_pack(reg.oam_abus) >> 2);
        reg.sprite_scanner.CENO_SCANNINGn = reg_old.sprite_scanner.BESU_SCANNINGn;
      }

      reg.sfetch_control.SUDA_SFETCH_REQp   = reg.sfetch_control.SOBU_SFETCH_REQp;
      reg.sfetch_control.TYFO_SFETCH_S0p_D1 = reg.sfetch_counter.TOXE_SFETCH_S0p;
      reg.sfetch_control.SEBA_SFETCH_S1p_D5 = reg.sfetch_control.VONU_SFETCH_S1p_D4;
      reg.win_ctrl.NUNU_WIN_MATCHp = reg.win_ctrl.PYCO_WIN_MATCHp;
      reg.tfetch_control.PORY_FETCH_DONEp = reg.tfetch_control.NYKA_FETCH_DONEp;
    }

    if (reg.sfetch_control.SOBU_SFETCH_REQp && !reg.sfetch_control.SUDA_SFETCH_REQp) {
      reg.sfetch_control.TAKA_SFETCH_RUNNINGp = 1;
    }

    reg_new.sprite_scanner.FETO_SCAN_DONEp = bit_pack(reg.scan_counter) == 39;



    if (reg_new.ATEJ_LINE_RSTp) {
      bit_unpack(reg.sfetch_counter, 0);
      reg.win_ctrl.PYNU_WIN_MODE_Ap = 0;
      reg.sfetch_control.TAKA_SFETCH_RUNNINGp = 1;
    }

    if (reg_new.VOGA_HBLANKp) reg_new.XYMU_RENDERINGn = 1;
    if (reg_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp) reg_new.XYMU_RENDERINGn = 0;
    if (reg_new.XYMU_RENDERINGn) {
      reg.sfetch_control.TOBU_SFETCH_S1p_D2 = 0;
      reg.sfetch_control.VONU_SFETCH_S1p_D4 = 0;
      reg.sfetch_control.SEBA_SFETCH_S1p_D5 = 0;
      reg.sfetch_control.TEXY_SFETCHINGp = 0;
      reg.tfetch_control.PYGO_FETCH_DONEp = 0;
      reg.tfetch_control.PORY_FETCH_DONEp = 0;
      reg.tfetch_control.NYKA_FETCH_DONEp = 0;
      reg.tfetch_control.POKY_PRELOAD_LATCHp = 0;
      reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp = 0;
    }
    else {
      reg.sfetch_control.TEXY_SFETCHINGp = (reg.sfetch_counter.TULY_SFETCH_S1p || reg.sfetch_control.VONU_SFETCH_S1p_D4);
      if (!reg.tfetch_control.POKY_PRELOAD_LATCHp && reg.tfetch_control.NYKA_FETCH_DONEp && reg.tfetch_control.PORY_FETCH_DONEp) {
        reg.sfetch_control.TAKA_SFETCH_RUNNINGp = 0;
      }
      reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp =
        reg.sfetch_control.TYFO_SFETCH_S0p_D1 &&
        reg.sfetch_counter.TOXE_SFETCH_S0p &&
        reg.sfetch_control.SEBA_SFETCH_S1p_D5 &&
        reg.sfetch_control.VONU_SFETCH_S1p_D4;

      if (reg.tfetch_control.PYGO_FETCH_DONEp) {
        reg.tfetch_control.POKY_PRELOAD_LATCHp = 1;
      }

      if (reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp) {
        reg.sfetch_control.TAKA_SFETCH_RUNNINGp = 0;
      }

      if (!reg_old.sfetch_control.WUTY_SFETCH_DONE_TRIGp && reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp) {
        bit_unpack(reg_new.sprite_reset_flags, bit_pack(reg_old.sprite_match_flags));
      }
    }
  }


  uint8_t sfetch_phase_old = pack(!(reg_old.sfetch_control.TYFO_SFETCH_S0p_D1 ^ reg_old.sfetch_counter.TOXE_SFETCH_S0p), reg_old.sfetch_counter.TOXE_SFETCH_S0p, reg_old.sfetch_counter.TULY_SFETCH_S1p, reg_old.sfetch_counter.TESE_SFETCH_S2p);
  uint8_t sfetch_phase_new = pack(!(reg_new.sfetch_control.TYFO_SFETCH_S0p_D1 ^ reg_new.sfetch_counter.TOXE_SFETCH_S0p), reg_new.sfetch_counter.TOXE_SFETCH_S0p, reg_new.sfetch_counter.TULY_SFETCH_S1p, reg_new.sfetch_counter.TESE_SFETCH_S2p);

  if (!reg_old.XYMU_RENDERINGn) {
    if ((sfetch_phase_old == 5) && (sfetch_phase_new == 6 || reg_new.XYMU_RENDERINGn)) {
      bit_unpack_inv(reg_new.sprite_pix_a, bit_pack(reg_old.flipped_sprite));
    }

    if ((sfetch_phase_old == 9) && (sfetch_phase_new == 10 || reg_new.XYMU_RENDERINGn)) {
      bit_unpack_inv(reg_new.sprite_pix_b, bit_pack(reg_old.flipped_sprite));
    }
  }

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.


  {
    reg_new.ACYL_SCANNINGp = !reg_new.MATU_DMA_RUNNINGp && reg.sprite_scanner.BESU_SCANNINGn && !reg_new.reg_lcdc.XONA_LCDC_LCDENn;

    wire oam_busy_old = (cpu_addr_old >= 0xFE00 && cpu_addr_old <= 0xFEFF) || reg_new.MATU_DMA_RUNNINGp;
    wire oam_busy_new = (cpu_addr_new >= 0xFE00 && cpu_addr_new <= 0xFEFF) || reg_new.MATU_DMA_RUNNINGp;

    CHECK_N(!reg_old.XYMU_RENDERINGn && reg_new.ACYL_SCANNINGp);
    CHECK_N(!reg_new.XYMU_RENDERINGn && reg_new.ACYL_SCANNINGp);
    CHECK_N(!reg_old.XYMU_RENDERINGn && reg_old.ACYL_SCANNINGp);

    uint8_t BYCU_OAM_CLKp_old = 1;
    if (reg_old.ACYL_SCANNINGp)  BYCU_OAM_CLKp_old &= gen_clk_old(0b10001000);
    if (oam_busy_old)  BYCU_OAM_CLKp_old &= gen_clk_old(0b11110000);
    if (!reg_old.XYMU_RENDERINGn) BYCU_OAM_CLKp_old &= sfetch_phase_old != 3;

    uint8_t BYCU_OAM_CLKp_new = 1;
    if (reg_new.ACYL_SCANNINGp)  BYCU_OAM_CLKp_new &= gen_clk_new(0b10001000);
    if (oam_busy_new)  BYCU_OAM_CLKp_new &= gen_clk_new(0b11110000);
    if (!reg_new.XYMU_RENDERINGn) BYCU_OAM_CLKp_new &= sfetch_phase_new != 3;

    if (!BYCU_OAM_CLKp_old && BYCU_OAM_CLKp_new) {
      bit_unpack(reg.oam_temp_a, bit_pack_inv(reg.oam_latch_a));
      bit_unpack(reg.oam_temp_b, bit_pack_inv(reg.oam_latch_b));
    }
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  if (reg_new.reg_lcdc.XONA_LCDC_LCDENn || reg_new.ATEJ_LINE_RSTp) {
    bit_unpack(reg.sprite_counter, 0);
    bit_unpack(reg.sprite_reset_flags, 0);
    bit_unpack(reg.sprite_store_flags, 0);

    bit_unpack(reg.store_x0, 0xFF);
    bit_unpack(reg.store_x1, 0xFF);
    bit_unpack(reg.store_x2, 0xFF);
    bit_unpack(reg.store_x3, 0xFF);
    bit_unpack(reg.store_x4, 0xFF);
    bit_unpack(reg.store_x5, 0xFF);
    bit_unpack(reg.store_x6, 0xFF);
    bit_unpack(reg.store_x7, 0xFF);
    bit_unpack(reg.store_x8, 0xFF);
    bit_unpack(reg.store_x9, 0xFF);
  }
  else {
    bool ssf_clk = gen_clk_new(0b10011001) || !reg.sprite_scanner.CENO_SCANNINGn;

    int ly = (int)bit_pack(reg_new.reg_ly);
    int sy = (int)bit_pack(reg_new.oam_temp_a) - 16;
    int sprite_height = reg_new.reg_lcdc.XYMO_LCDC_SPSIZEn ? 8 : 16;

    if (ly < sy || ly >= sy + sprite_height) ssf_clk = 1;

    if (gen_clk_new(0b10101010)) {
      reg.sprite_scanner.DEZY_COUNT_CLKp = ssf_clk;
      if (!reg_old.sprite_scanner.DEZY_COUNT_CLKp && reg_new.sprite_scanner.DEZY_COUNT_CLKp) {
        if (bit_pack(reg_old.sprite_counter) != 10) {
          bit_unpack(reg_new.sprite_counter, bit_pack(reg_old.sprite_counter) + 1);
        }
      }
    }
    else {
      for (int i = 0; i < 10; i++) {
        (&reg.sprite_store_flags.DYHU_STORE0_CLKn)[i] = (i == (int)bit_pack(reg_new.sprite_counter)) && !bit(ssf_clk);
      }
    }


    auto store_clk_pe = (~bit_pack_inv(reg_old.sprite_store_flags)) & bit_pack_inv(reg_new.sprite_store_flags);
    auto store_clk_ne = bit_pack_inv(reg_old.sprite_store_flags) & (~bit_pack_inv(reg_new.sprite_store_flags));

    if (get_bit(store_clk_ne, 0)) bit_unpack_inv(reg.store_i0, bit_pack(reg.sprite_ibus));
    if (get_bit(store_clk_ne, 1)) bit_unpack_inv(reg.store_i1, bit_pack(reg.sprite_ibus));
    if (get_bit(store_clk_ne, 2)) bit_unpack_inv(reg.store_i2, bit_pack(reg.sprite_ibus));
    if (get_bit(store_clk_ne, 3)) bit_unpack_inv(reg.store_i3, bit_pack(reg.sprite_ibus));
    if (get_bit(store_clk_ne, 4)) bit_unpack_inv(reg.store_i4, bit_pack(reg.sprite_ibus));
    if (get_bit(store_clk_ne, 5)) bit_unpack_inv(reg.store_i5, bit_pack(reg.sprite_ibus));
    if (get_bit(store_clk_ne, 6)) bit_unpack_inv(reg.store_i6, bit_pack(reg.sprite_ibus));
    if (get_bit(store_clk_ne, 7)) bit_unpack_inv(reg.store_i7, bit_pack(reg.sprite_ibus));
    if (get_bit(store_clk_ne, 8)) bit_unpack_inv(reg.store_i8, bit_pack(reg.sprite_ibus));
    if (get_bit(store_clk_ne, 9)) bit_unpack_inv(reg.store_i9, bit_pack(reg.sprite_ibus));

    if (get_bit(store_clk_ne, 0)) bit_unpack_inv(reg.store_l0, bit_pack(reg.sprite_lbus));
    if (get_bit(store_clk_ne, 1)) bit_unpack_inv(reg.store_l1, bit_pack(reg.sprite_lbus));
    if (get_bit(store_clk_ne, 2)) bit_unpack_inv(reg.store_l2, bit_pack(reg.sprite_lbus));
    if (get_bit(store_clk_ne, 3)) bit_unpack_inv(reg.store_l3, bit_pack(reg.sprite_lbus));
    if (get_bit(store_clk_ne, 4)) bit_unpack_inv(reg.store_l4, bit_pack(reg.sprite_lbus));
    if (get_bit(store_clk_ne, 5)) bit_unpack_inv(reg.store_l5, bit_pack(reg.sprite_lbus));
    if (get_bit(store_clk_ne, 6)) bit_unpack_inv(reg.store_l6, bit_pack(reg.sprite_lbus));
    if (get_bit(store_clk_ne, 7)) bit_unpack_inv(reg.store_l7, bit_pack(reg.sprite_lbus));
    if (get_bit(store_clk_ne, 8)) bit_unpack_inv(reg.store_l8, bit_pack(reg.sprite_lbus));
    if (get_bit(store_clk_ne, 9)) bit_unpack_inv(reg.store_l9, bit_pack(reg.sprite_lbus));

    if (get_bit(store_clk_pe, 0)) bit_unpack(reg.store_x0, bit_pack(reg.oam_temp_b));
    if (get_bit(store_clk_pe, 1)) bit_unpack(reg.store_x1, bit_pack(reg.oam_temp_b));
    if (get_bit(store_clk_pe, 2)) bit_unpack(reg.store_x2, bit_pack(reg.oam_temp_b));
    if (get_bit(store_clk_pe, 3)) bit_unpack(reg.store_x3, bit_pack(reg.oam_temp_b));
    if (get_bit(store_clk_pe, 4)) bit_unpack(reg.store_x4, bit_pack(reg.oam_temp_b));
    if (get_bit(store_clk_pe, 5)) bit_unpack(reg.store_x5, bit_pack(reg.oam_temp_b));
    if (get_bit(store_clk_pe, 6)) bit_unpack(reg.store_x6, bit_pack(reg.oam_temp_b));
    if (get_bit(store_clk_pe, 7)) bit_unpack(reg.store_x7, bit_pack(reg.oam_temp_b));
    if (get_bit(store_clk_pe, 8)) bit_unpack(reg.store_x8, bit_pack(reg.oam_temp_b));
    if (get_bit(store_clk_pe, 9)) bit_unpack(reg.store_x9, bit_pack(reg.oam_temp_b));

    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 0)) bit_unpack(reg.store_x0, 0xFF);
    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 1)) bit_unpack(reg.store_x1, 0xFF);
    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 2)) bit_unpack(reg.store_x2, 0xFF);
    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 3)) bit_unpack(reg.store_x3, 0xFF);
    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 4)) bit_unpack(reg.store_x4, 0xFF);
    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 5)) bit_unpack(reg.store_x5, 0xFF);
    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 6)) bit_unpack(reg.store_x6, 0xFF);
    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 7)) bit_unpack(reg.store_x7, 0xFF);
    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 8)) bit_unpack(reg.store_x8, 0xFF);
    if (get_bit(bit_pack(reg_new.sprite_reset_flags), 9)) bit_unpack(reg.store_x9, 0xFF);

  }












  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  // NOTE we reassign this below because there's a bit of a feedback loop
  wire pause_rendering_new = reg_new.win_ctrl.RYDY_WIN_HITp || !reg_new.tfetch_control.POKY_PRELOAD_LATCHp || reg_new.FEPO_STORE_MATCHp || reg_new.WODU_HBLANKp;

  if (gen_clk_new(0b01010101)) {
    if (!pause_rendering_new) {
      reg.fine_scroll.PUXA_SCX_FINE_MATCH_A = reg_old.fine_scroll.ROXY_FINE_SCROLL_DONEn && (bit_pack_inv(&reg_old.reg_scx.DATY_SCX0n, 3) == bit_pack(reg_old.fine_count));
    }
  }
  else {
    reg.fine_scroll.NYZE_SCX_FINE_MATCH_B = reg.fine_scroll.PUXA_SCX_FINE_MATCH_A;
  }

  if (reg_new.XYMU_RENDERINGn) {
    reg.fine_scroll.ROXY_FINE_SCROLL_DONEn = 1;
    reg.fine_scroll.NYZE_SCX_FINE_MATCH_B = 0;
    reg.fine_scroll.PUXA_SCX_FINE_MATCH_A = 0;
  }

  if (reg.fine_scroll.PUXA_SCX_FINE_MATCH_A && !reg.fine_scroll.NYZE_SCX_FINE_MATCH_B) {
    reg.fine_scroll.ROXY_FINE_SCROLL_DONEn = 0;
  }

  if (!reg_new.reg_lcdc.XONA_LCDC_LCDENn) {
    if (reg_new.reg_lcdc.WYMO_LCDC_WINENn) {
      reg.win_ctrl.PYNU_WIN_MODE_Ap = 0;
      reg.win_ctrl.RYDY_WIN_HITp = 0;
      reg.win_ctrl.PUKU_WIN_HITn = 1;
    }
    else {
      
      if (reg.win_ctrl.NUNU_WIN_MATCHp) {
        reg.win_ctrl.PYNU_WIN_MODE_Ap = 1;
      }
      
      if (reg.win_ctrl.PYNU_WIN_MODE_Ap && !reg.win_ctrl.NOPA_WIN_MODE_Bp) {
        reg.tfetch_control.PORY_FETCH_DONEp = 0;
        reg.tfetch_control.NYKA_FETCH_DONEp = 0;
      }

      if (reg.tfetch_control.PORY_FETCH_DONEp) {
        reg.win_ctrl.RYDY_WIN_HITp = 0;
        reg.win_ctrl.PUKU_WIN_HITn = 1;
      }
      else if (reg.win_ctrl.PYNU_WIN_MODE_Ap && !reg.win_ctrl.NOPA_WIN_MODE_Bp) {
        reg.win_ctrl.RYDY_WIN_HITp = 1;
        reg.win_ctrl.PUKU_WIN_HITn = 0;
      }
    }
  }

  pause_rendering_new = reg_new.win_ctrl.RYDY_WIN_HITp || !reg_new.tfetch_control.POKY_PRELOAD_LATCHp || reg_new.FEPO_STORE_MATCHp || reg_new.WODU_HBLANKp;

  wire pause_rendering_old = reg_old.win_ctrl.RYDY_WIN_HITp || !reg_old.tfetch_control.POKY_PRELOAD_LATCHp || reg_old.FEPO_STORE_MATCHp || reg_old.WODU_HBLANKp;
  bool SACU_CLKPIPE_old = gen_clk_old(0b10101010) || pause_rendering_old || reg_old.fine_scroll.ROXY_FINE_SCROLL_DONEn;
  wire SACU_CLKPIPE_new = gen_clk_new(0b10101010) || pause_rendering_new || reg.fine_scroll.ROXY_FINE_SCROLL_DONEn;

  if (reg_new.ATEJ_LINE_RSTp) CHECK_P(SACU_CLKPIPE_new);

  if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
    bit_unpack(reg.pix_count, bit_pack(reg.pix_count) + 1);
  }

  if (reg_new.ATEJ_LINE_RSTp) {
    bit_unpack(reg.pix_count, 0);
  }

  if (reg_new.reg_lcdc.XONA_LCDC_LCDENn) {
    bit_unpack(reg.pix_count, 0);
  }

  if (reg_new.XYMU_RENDERINGn || reg.sprite_scanner.CENO_SCANNINGn || reg.reg_lcdc.XYLO_LCDC_SPENn) {
    bit_unpack(reg.sprite_match_flags, 0);
  }

  if (!reg_new.XYMU_RENDERINGn && !reg.sprite_scanner.CENO_SCANNINGn && !reg.reg_lcdc.XYLO_LCDC_SPENn) {
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
  reg.WODU_HBLANKp = !reg.FEPO_STORE_MATCHp && (bit_pack(reg.pix_count) & 167) == 167;

  if (gen_clk_new(0b01010101)) {
    if (!pause_rendering_new) reg.lcd.PAHO_X_8_SYNC = reg_old.pix_count.XYDO_PX3p;
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  bit_unpack(reg.sprite_ibus, 0xFF);
  bit_unpack(reg.sprite_lbus, 0xFF);

  if (reg.sprite_match_flags.GUVA_SPRITE0_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i0));
  if (reg.sprite_match_flags.ENUT_SPRITE1_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i1));
  if (reg.sprite_match_flags.EMOL_SPRITE2_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i2));
  if (reg.sprite_match_flags.GYFY_SPRITE3_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i3));
  if (reg.sprite_match_flags.GONO_SPRITE4_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i4));
  if (reg.sprite_match_flags.GEGA_SPRITE5_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i5));
  if (reg.sprite_match_flags.XOJA_SPRITE6_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i6));
  if (reg.sprite_match_flags.GUTU_SPRITE7_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i7));
  if (reg.sprite_match_flags.FOXA_SPRITE8_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i8));
  if (reg.sprite_match_flags.GUZE_SPRITE9_GETp) bit_unpack(reg.sprite_ibus, bit_pack_inv(reg.store_i9));

  if (reg.sprite_match_flags.GUVA_SPRITE0_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l0));
  if (reg.sprite_match_flags.ENUT_SPRITE1_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l1));
  if (reg.sprite_match_flags.EMOL_SPRITE2_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l2));
  if (reg.sprite_match_flags.GYFY_SPRITE3_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l3));
  if (reg.sprite_match_flags.GONO_SPRITE4_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l4));
  if (reg.sprite_match_flags.GEGA_SPRITE5_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l5));
  if (reg.sprite_match_flags.XOJA_SPRITE6_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l6));
  if (reg.sprite_match_flags.GUTU_SPRITE7_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l7));
  if (reg.sprite_match_flags.FOXA_SPRITE8_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l8));
  if (reg.sprite_match_flags.GUZE_SPRITE9_GETp) bit_unpack(reg.sprite_lbus, bit_pack_inv(reg.store_l9));

  if (reg.sprite_scanner.CENO_SCANNINGn) {
    bit_unpack(reg.sprite_ibus, bit_pack(reg.sprite_index));
  }

  if (reg_new.XYMU_RENDERINGn) {
    bit_unpack(reg.sprite_ibus, bit_pack(reg.sprite_index));
  }

  if (!reg.FEPO_STORE_MATCHp) {
    auto pack_ydiff = ~bit_pack(reg_new.reg_ly) + bit_pack(reg_new.oam_temp_a);
    bit_unpack(reg.sprite_lbus, pack_ydiff);
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // WY/WX/window match

  if (gen_clk_new(0b01010101)) {
    if (!pause_rendering_new) reg.win_ctrl.PYCO_WIN_MATCHp = reg.win_ctrl.NUKO_WX_MATCHp;
  }

  if (!reg_new.XYMU_RENDERINGn) {
    if (gen_clk_new(0b01010101)) {
      reg.win_ctrl.RENE_WIN_FETCHn_B = reg.win_ctrl.RYFA_WIN_FETCHn_A;
    }

    auto fine_count_new = bit_pack(reg.fine_count);

    if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
      reg.win_ctrl.RYFA_WIN_FETCHn_A = !reg.win_ctrl.NUKO_WX_MATCHp && fine_count_new == 7;
    }
  }
  else {
    reg.win_ctrl.RENE_WIN_FETCHn_B = 0;
    reg.win_ctrl.RYFA_WIN_FETCHn_A = 0;
  }

  if (reg.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00000001)) {
    if (cpu_addr_new == 0xFF4A) bit_unpack_inv(reg.reg_wy, bit_pack(reg_old.cpu_dbus));
    if (cpu_addr_new == 0xFF4B) bit_unpack_inv(reg.reg_wx, bit_pack(reg_old.cpu_dbus));
  }

  if (reg.cpu_signals.SIG_IN_CPU_RDp) {
    if (cpu_addr_new == 0xFF4A) bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.reg_wy));
    if (cpu_addr_new == 0xFF4B) bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.reg_wx));
  }

  // FIXME get rid of this signal
  reg_new.win_ctrl.ROGE_WY_MATCHp = (bit_pack(reg_new.reg_ly) == bit_pack_inv(reg_new.reg_wy)) && !reg_new.reg_lcdc.WYMO_LCDC_WINENn;

  if (gen_clk_new(0b00100000)) {
    reg.win_ctrl.SARY_WY_MATCHp = reg_old.win_ctrl.ROGE_WY_MATCHp;
  }

  if (reg_new.reg_lcdc.XONA_LCDC_LCDENn) {
    reg.win_ctrl.PYCO_WIN_MATCHp = 0;
    reg.win_ctrl.SARY_WY_MATCHp = 0;
  }


  if (reg.win_ctrl.SARY_WY_MATCHp) reg.win_ctrl.REJO_WY_MATCH_LATCHp = 1;
  if (reg.lcd.POPU_y144p) reg.win_ctrl.REJO_WY_MATCH_LATCHp = 0;
  if (reg_new.reg_lcdc.XONA_LCDC_LCDENn) reg.win_ctrl.REJO_WY_MATCH_LATCHp = 0;

  reg.win_ctrl.NUKO_WX_MATCHp = (bit_pack_inv(reg.reg_wx) == bit_pack(reg.pix_count)) && reg.win_ctrl.REJO_WY_MATCH_LATCHp;

  //----------------------------------------
  // Tile fetch sequencer

  uint8_t bfetch_phase_old = pack(!(reg.tfetch_control.LYZU_BFETCH_S0p_D1 ^ reg.tfetch_counter.LAXU_BFETCH_S0p), reg.tfetch_counter.LAXU_BFETCH_S0p, reg.tfetch_counter.MESU_BFETCH_S1p, reg.tfetch_counter.NYVA_BFETCH_S2p);

  auto restart_fetch = [](const GateBoyReg& reg) {
    return !reg.XYMU_RENDERINGn && !reg.tfetch_control.POKY_PRELOAD_LATCHp && reg.tfetch_control.NYKA_FETCH_DONEp && reg.tfetch_control.PORY_FETCH_DONEp;
  };

  auto trigger_win_fetch = [&](const GateBoyReg& reg) {
    bool TEVO_WIN_FETCH_TRIGp = 0;
    if (reg.win_ctrl.RYFA_WIN_FETCHn_A && !reg.win_ctrl.RENE_WIN_FETCHn_B) TEVO_WIN_FETCH_TRIGp = 1;
    if (!reg.win_ctrl.RYDY_WIN_HITp && reg.win_ctrl.SOVY_WIN_HITp) TEVO_WIN_FETCH_TRIGp = 1;
    if (restart_fetch(reg)) TEVO_WIN_FETCH_TRIGp = 1;
    return TEVO_WIN_FETCH_TRIGp;
  };


  wire BFETCH_RSTp_new =
    reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp ||
    (reg.win_ctrl.PYNU_WIN_MODE_Ap && !reg.win_ctrl.NOPA_WIN_MODE_Bp) ||
    (reg.win_ctrl.RYFA_WIN_FETCHn_A && !reg.win_ctrl.RENE_WIN_FETCHn_B) ||
    (reg.win_ctrl.SOVY_WIN_HITp && !reg.win_ctrl.RYDY_WIN_HITp) ||
    restart_fetch(reg_new);

  if (gen_clk_new(0b01010101)) {
    reg.tfetch_control.LYZU_BFETCH_S0p_D1 = reg.tfetch_counter.LAXU_BFETCH_S0p;
  }

  if (reg_new.XYMU_RENDERINGn) {
    reg.tfetch_control.LYZU_BFETCH_S0p_D1 = 0;
  }

  if (BFETCH_RSTp_new) {
    bit_unpack(reg.tfetch_counter, 0);
    reg.tfetch_control.LOVY_FETCH_DONEp = 0;
    reg.tfetch_control.LONY_FETCHINGp = 1;
    reg.tfetch_control.LYRY_BFETCH_DONEp = 0;
  }
  else {
    if ((bfetch_phase_old < 10) && gen_clk_new(0b10101010)) {
      bit_unpack(reg.tfetch_counter, (bfetch_phase_old >> 1) + 1);
    }

    if (gen_clk_new(0b10101010)) {
      reg.tfetch_control.LOVY_FETCH_DONEp = reg.tfetch_control.LYRY_BFETCH_DONEp;
    }
    reg.tfetch_control.LYRY_BFETCH_DONEp = reg.tfetch_counter.LAXU_BFETCH_S0p && reg.tfetch_counter.NYVA_BFETCH_S2p;
  }

  if (reg.tfetch_control.LOVY_FETCH_DONEp || reg_new.XYMU_RENDERINGn) {
    reg.tfetch_control.LONY_FETCHINGp = 0;
  }

  uint8_t bfetch_phase_new = pack(!(reg.tfetch_control.LYZU_BFETCH_S0p_D1 ^ reg.tfetch_counter.LAXU_BFETCH_S0p), reg.tfetch_counter.LAXU_BFETCH_S0p, reg.tfetch_counter.MESU_BFETCH_S1p, reg.tfetch_counter.NYVA_BFETCH_S2p);

  if (!reg_old.XYMU_RENDERINGn) {
    // These ffs are weird because they latches on phase change _or_ if rendering stops in the middle of a fetch
    // Good example of gate-level behavior that doesn't matter

    if ((bfetch_phase_old == 6) && (bfetch_phase_new == 7 || reg_new.XYMU_RENDERINGn)) {
      bit_unpack_inv(reg.tile_temp_a, bit_pack(reg.vram_dbus));
    }

    if ((bfetch_phase_old == 2) && (bfetch_phase_new == 3 || reg_new.XYMU_RENDERINGn)) {
      bit_unpack(reg.tile_temp_b, bit_pack(reg.vram_dbus));
    }

    if ((bfetch_phase_old == 10) && (bfetch_phase_new == 11 || reg_new.XYMU_RENDERINGn)) {
      bit_unpack(reg.tile_temp_b, bit_pack(reg.vram_dbus));
    }
  }

  //----------------------------------------
  // Fine match counter

  wire TEVO_WIN_FETCH_TRIGp_old = trigger_win_fetch(reg_old);
  wire TEVO_WIN_FETCH_TRIGp_new = trigger_win_fetch(reg_new);
  
  if (reg_new.ATEJ_LINE_RSTp) CHECK_P(reg.XYMU_RENDERINGn);

  if (reg.reg_lcdc.XONA_LCDC_LCDENn) {
    bit_unpack(reg.fine_count, 0);
    bit_unpack(reg.win_x.map, 0);
    bit_unpack(reg.win_y, 0);
  }
  else {
    if (bit_pack(reg.fine_count) != 7 && !pause_rendering_old && gen_clk_new(0b10101010)) {
      bit_unpack(reg.fine_count, bit_pack(reg.fine_count) + 1);
    }

    if (reg.XYMU_RENDERINGn) {
      bit_unpack(reg.fine_count, 0);
    }

    if (reg_new.ATEJ_LINE_RSTp) {
      bit_unpack(reg.win_x.map, 0);
    }
    else if (TEVO_WIN_FETCH_TRIGp_new) {
      bit_unpack(reg.fine_count, 0);
      if (reg_new.win_ctrl.PYNU_WIN_MODE_Ap) {
        bit_unpack(reg.win_x.map, bit_pack(reg_old.win_x.map) + 1);
      }
    }


    if (reg_old.win_ctrl.PYNU_WIN_MODE_Ap && !reg_new.win_ctrl.PYNU_WIN_MODE_Ap) {
      bit_unpack(reg.win_y, bit_pack(reg_old.win_y) + 1);
    }

    if (reg_new.lcd.POPU_y144p) {
      bit_unpack(reg.win_y, 0);
    }
  }

  //----------------------------------------
  // Pal reg read/write

  if (reg.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00000001)) {
    if (cpu_addr_new == 0xFF47) bit_unpack_inv(reg.reg_bgp,  bit_pack(reg_old.cpu_dbus));
    if (cpu_addr_new == 0xFF48) bit_unpack_inv(reg.reg_obp0, bit_pack(reg_old.cpu_dbus));
    if (cpu_addr_new == 0xFF49) bit_unpack_inv(reg.reg_obp1, bit_pack(reg_old.cpu_dbus));
  }

  if (reg.cpu_signals.SIG_IN_CPU_RDp) {
    if (cpu_addr_new == 0xFF47) bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.reg_bgp));
    if (cpu_addr_new == 0xFF48) bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.reg_obp0));
    if (cpu_addr_new == 0xFF49) bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.reg_obp1));
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
    
  if (reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp || (reg.win_ctrl.PYNU_WIN_MODE_Ap && !reg.win_ctrl.NOPA_WIN_MODE_Bp) || TEVO_WIN_FETCH_TRIGp_new) {
    bpipe_a = tpix_a;
    bpipe_b = tpix_b;
  }

  if (reg.sfetch_control.WUTY_SFETCH_DONE_TRIGp) {
    auto smask = (spipe_a | spipe_b);
    spipe_a = (spipe_a & smask) | (spix_a & ~smask);
    spipe_b = (spipe_b & smask) | (spix_b & ~smask);
    mpipe = reg.oam_temp_b.DEPO_OAM_DB7p ? mpipe | ~smask : mpipe & smask;
    ppipe = reg.oam_temp_b.GOMO_OAM_DB4p ? ppipe | ~smask : ppipe & smask;
  }

  bit_unpack(reg.spr_pipe_a, spipe_a);
  bit_unpack(reg.spr_pipe_b, spipe_b);
  bit_unpack(reg.bgw_pipe_a, bpipe_a);
  bit_unpack(reg.bgw_pipe_b, bpipe_b);
  bit_unpack(reg.mask_pipe, mpipe);
  bit_unpack(reg.pal_pipe, ppipe);

  //----------------------------------------
  // Pipe merge and output

  wire PIX_BG_LOp = reg.bgw_pipe_a.PYBO_BGW_PIPE_A7 && !reg.reg_lcdc.VYXE_LCDC_BGENn;
  wire PIX_BG_HIp = reg.bgw_pipe_b.SOHU_BGW_PIPE_B7 && !reg.reg_lcdc.VYXE_LCDC_BGENn;
  wire PIX_SP_LOp = reg.spr_pipe_a.WUFY_SPR_PIPE_A7 && !reg.reg_lcdc.XYLO_LCDC_SPENn;
  wire PIX_SP_HIp = reg.spr_pipe_b.VUPY_SPR_PIPE_B7 && !reg.reg_lcdc.XYLO_LCDC_SPENn;

  auto pal_idx = 0;
  auto pal = 0;

  auto bgp  = bit_pack_inv(reg.reg_bgp);
  auto obp0 = bit_pack_inv(reg.reg_obp0);
  auto obp1 = bit_pack_inv(reg.reg_obp1);

  if (PIX_SP_HIp || PIX_SP_LOp) {
    pal_idx = pack(PIX_SP_LOp, PIX_SP_HIp);
    pal = reg.pal_pipe.LYME_PAL_PIPE_D7 ? obp1 : obp0;
  }
  else {
    pal_idx = pack(PIX_BG_LOp, PIX_BG_HIp);
    pal = bgp;
  }

  reg.lcd.REMY_LD0n = (pal >> (pal_idx * 2 + 0)) & 1;
  reg.lcd.RAVO_LD1n = (pal >> (pal_idx * 2 + 1)) & 1;

  //----------------------------------------
  // LCD pins

  if (!reg_new.reg_lcdc.XONA_LCDC_LCDENn) {
    reg.lcd.PIN_52_LCD_CNTRL = !reg.lcd.SYGU_LINE_STROBE && !reg.lcd.RUTU_x113p;

    if (reg_old.lcd.RUTU_x113p && !reg_new.lcd.RUTU_x113p) reg.lcd.LUCA_LINE_EVENp = !reg.lcd.LUCA_LINE_EVENp;
    if (!reg_old.lcd.POPU_y144p && reg_new.lcd.POPU_y144p) reg.lcd.NAPO_FRAME_EVENp = !reg.lcd.NAPO_FRAME_EVENp;
    reg.lcd.PIN_56_LCD_FLIPS = reg.lcd.NAPO_FRAME_EVENp ^ reg.lcd.LUCA_LINE_EVENp;


    auto ly = bit_pack(reg.reg_ly);

    if (reg_old.lcd.NYPE_x113p && !reg_new.lcd.NYPE_x113p) {
      reg.lcd.MEDA_VSYNC_OUTn = ly == 0;
    }

    reg.lcd.PIN_57_LCD_VSYNC = !reg.lcd.MEDA_VSYNC_OUTn;

    if (reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp && reg.lcd.PAHO_X_8_SYNC) {
      reg.lcd.POME = 0;
      reg.lcd.RUJU = 1;
      reg.lcd.POFY = 0;
    }
    else if (reg.sprite_scanner.AVAP_SCAN_DONE_TRIGp) {
      reg.lcd.POME = 0;
      reg.lcd.RUJU = 0;
      reg.lcd.POFY = 1;
    }
    else if (reg.lcd.PAHO_X_8_SYNC) {
      reg.lcd.POME = 1;
      reg.lcd.RUJU = 1;
      reg.lcd.POFY = 0;
    }

    reg.lcd.PIN_50_LCD_DATA1 = reg.lcd.RAVO_LD1n;
    reg.lcd.PIN_51_LCD_DATA0 = reg.lcd.REMY_LD0n;
    reg.lcd.PIN_54_LCD_HSYNC = !reg.lcd.POFY;
    reg.lcd.PIN_55_LCD_LATCH = !reg.lcd.RUTU_x113p;

    if (reg.pix_count.XEHO_PX0p && reg.pix_count.XYDO_PX3p) reg.lcd.WUSA_LCD_CLOCK_GATE = 1;
    if (reg.VOGA_HBLANKp) reg.lcd.WUSA_LCD_CLOCK_GATE = 0;

    reg.lcd.PIN_53_LCD_CLOCK = (!reg.lcd.WUSA_LCD_CLOCK_GATE || !SACU_CLKPIPE_new) && (!reg.fine_scroll.PUXA_SCX_FINE_MATCH_A || reg.fine_scroll.NYZE_SCX_FINE_MATCH_B);
  }
  else {
    reg.lcd.LUCA_LINE_EVENp = 0;
    reg.lcd.NAPO_FRAME_EVENp = 0;
    reg.lcd.MEDA_VSYNC_OUTn = 0;
    reg.lcd.WUSA_LCD_CLOCK_GATE = 0;

    reg.lcd.POME = 1;
    reg.lcd.RUJU = 1;
    reg.lcd.POFY = 0;

    reg.lcd.PIN_50_LCD_DATA1 = reg.lcd.RAVO_LD1n;
    reg.lcd.PIN_51_LCD_DATA0 = reg.lcd.REMY_LD0n;
    reg.lcd.PIN_52_LCD_CNTRL = 1;
    reg.lcd.PIN_53_LCD_CLOCK = 1;
    reg.lcd.PIN_54_LCD_HSYNC = 1;
    reg.lcd.PIN_55_LCD_LATCH = !reg.reg_div.UGOT_DIV06p;
    reg.lcd.PIN_56_LCD_FLIPS = !reg.reg_div.TULU_DIV07p;
    reg.lcd.PIN_57_LCD_VSYNC = 1;
  }

  //----------------------------------------
  // Audio

  //tock_spu_logic();

  //----------------------------------------
  // Memory buses

  if (reg.cpu_signals.SIG_IN_CPU_EXT_BUSp && !cpu_addr_vram_new) {
    bit_unpack(reg.ext_addr_latch, bit_pack(reg.cpu_abus));
  }

  if (reg_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) {
    reg.ext_ctrl.PIN_80_CSn = !reg.reg_dma.MARU_DMA_A15n;
    bit_unpack_inv(reg.ext_abus.lo, bit_pack(reg.dma_lo));
    bit_unpack_inv(reg.ext_abus.hi, bit_pack_inv(reg.reg_dma));
  }
  else {
    reg.ext_ctrl.PIN_80_CSn = reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK && cpu_addr_ram_new;
    bit_unpack_inv(reg.ext_abus, bit_pack(reg.ext_addr_latch));
  }

  //----------------------------------------

  if (!(reg_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) && reg.cpu_signals.SIG_IN_CPU_EXT_BUSp && reg.cpu_signals.SIG_IN_CPU_WRp) {
    reg.ext_ctrl.PIN_79_RDn = cpu_addr_vram_new;
    reg.ext_ctrl.PIN_78_WRn = gen_clk_new(0b00001110) && !cpu_addr_vram_new;
  }
  else {
    reg.ext_ctrl.PIN_79_RDn = 1;
    reg.ext_ctrl.PIN_78_WRn = 0;
  }

  if (reg_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) {
    reg.ext_abus.hi.PIN_16_A15 = reg.reg_dma.MARU_DMA_A15n;
  }
  else if (!reg.cpu_signals.TEPU_BOOT_BITn && cpu_addr_new <= 0x00FF) {
    reg.ext_abus.hi.PIN_16_A15 = 0;
  }
  else {
    reg.ext_abus.hi.PIN_16_A15 = reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK && !reg.cpu_abus.BUS_CPU_A15p;
  }

  CHECK_N(reg.cpu_signals.SIG_IN_CPU_RDp && reg.cpu_signals.SIG_IN_CPU_WRp);

  if (reg.cpu_signals.SIG_IN_CPU_EXT_BUSp && reg.cpu_signals.SIG_IN_CPU_WRp && !cpu_addr_vram_new) {
    bit_unpack_inv(reg.ext_dbus, bit_pack(reg.cpu_dbus));
  }
  else {
    bit_unpack(reg.ext_dbus, 0);
  }

  //----------------------------------------
  // Ext read

  if (reg.ext_ctrl.PIN_79_RDn) {
    uint16_t ext_addr = (uint16_t)bit_pack_inv(reg.ext_abus);
    
    auto rom_addr_mask = cart_rom_addr_mask(cart_blob);
    auto ram_addr_mask = cart_ram_addr_mask(cart_blob);

    const int region = ext_addr >> 13;
    uint8_t data_in = 0x00;

    bool ext_read_en = false;

    if (cart_has_mbc1(cart_blob)) {

      bool mbc1_ram_en = reg.ext_mbc.MBC1_RAM_EN;
      bool mbc1_mode = reg.ext_mbc.MBC1_MODE;

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

  if (reg.ext_ctrl.PIN_78_WRn) {
    auto ext_addr = bit_pack_inv(reg.ext_abus);
    auto region = ext_addr >> 13;
    auto data_out = bit_pack_inv(reg.ext_dbus);

    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_ram_en = reg.ext_mbc.MBC1_RAM_EN;
      bool mbc1_mode = reg.ext_mbc.MBC1_MODE;

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


  if (reg.cpu_signals.SIG_IN_CPU_RDp && reg.cpu_signals.SIG_IN_CPU_EXT_BUSp && !cpu_addr_vram_new && reg.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
    bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.ext_data_latch));
  }
  else {
    bit_unpack_inv(reg.ext_data_latch, bit_pack_inv(reg.ext_dbus));
  }

  {
    bit_unpack(reg.vram_abus, 0xFFFF);
    bit_unpack(reg.vram_dbus, 0xFF);

    //--------------------------------------------
    // CPU vram read address

    if (!dma_addr_vram_new && reg.XYMU_RENDERINGn) {
      bit_unpack_inv(reg.vram_abus, bit_pack(reg.cpu_abus));
    }

    //--------------------------------------------
    // DMA vram read address

    if (dma_addr_vram_new) {
      bit_unpack_inv(reg.vram_abus, bit_pack(reg.dma_lo));
      bit_unpack_inv(reg.vram_abus.hi, bit_pack_inv(reg.reg_dma));
    }

    //--------------------------------------------
    // SCX/SCY regs and BG map read address

    if (reg.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00000001)) {
      if (cpu_addr_new == 0xFF42) bit_unpack_inv(reg.reg_scy, bit_pack(reg_old.cpu_dbus));
      if (cpu_addr_new == 0xFF43) bit_unpack_inv(reg.reg_scx, bit_pack(reg_old.cpu_dbus));
    }

    if (reg.cpu_signals.SIG_IN_CPU_RDp) {
      if (cpu_addr_new == 0xFF42) bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.reg_scy));
      if (cpu_addr_new == 0xFF43) bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.reg_scx));
    }

    if (reg.tfetch_control.LONY_FETCHINGp) {
      auto px  = bit_pack(reg.pix_count);
      auto ly  = bit_pack(reg.reg_ly);
      auto scx = bit_pack_inv(reg.reg_scx);
      auto scy = bit_pack_inv(reg.reg_scy);

      auto sum_x = px + scx;
      auto sum_y = ly + scy;

      //--------------------------------------------
      // BG map read address

      if (!reg.tfetch_counter.MESU_BFETCH_S1p && !reg.tfetch_counter.NYVA_BFETCH_S2p && !reg.win_ctrl.PYNU_WIN_MODE_Ap) {
        uint32_t addr = 0;
        auto bgmap_en = !reg.reg_lcdc.XAFO_LCDC_BGMAPn.state;

        bit_cat(addr,  0,  4, (px + scx) >> 3);
        bit_cat(addr,  5,  9, (ly + scy) >> 3);
        bit_cat(addr, 10, 10, bgmap_en);
        bit_cat(addr, 11, 11, 1);
        bit_cat(addr, 12, 12, 1);

        bit_unpack_inv(reg.vram_abus, addr);
      }

      //--------------------------------------------
      // BG/Win tile read address

      if (reg.tfetch_counter.MESU_BFETCH_S1p || reg.tfetch_counter.NYVA_BFETCH_S2p) {
        uint32_t addr  = 0;

        auto hilo = reg.tfetch_counter.NYVA_BFETCH_S2p.state;
        auto tile_y = (reg.win_ctrl.PYNU_WIN_MODE_Ap ? bit_pack(reg.win_y.tile) : (sum_y & 0b111));
        auto map_y = bit_pack(reg.tile_temp_b);
        auto map = (!reg.tile_temp_b.PYJU_TILE_DB7p && reg.reg_lcdc.WEXU_LCDC_BGTILEn);

        bit_cat(addr,  0,  0, hilo);
        bit_cat(addr,  1,  3, tile_y);
        bit_cat(addr,  4, 11, map_y);
        bit_cat(addr, 12, 12, map);

        bit_unpack_inv(reg.vram_abus, addr);
      }
    }


    if ( reg_new.tfetch_control.LONY_FETCHINGp &&
        !reg_new.tfetch_counter.MESU_BFETCH_S1p &&
        !reg_new.tfetch_counter.NYVA_BFETCH_S2p &&
         reg_new.win_ctrl.PYNU_WIN_MODE_Ap) {
      uint32_t addr = 0;

      bit_cat(addr,  0,  4, bit_pack_inv(reg.win_x.map));
      bit_cat(addr,  5,  9, bit_pack_inv(reg.win_y.map));
      bit_cat(addr, 10, 10, reg.reg_lcdc.WOKY_LCDC_WINMAPn.state);

      bit_unpack(reg.vram_abus, addr);
    }

    //--------------------------------------------
    // Sprite read address

    if (reg.sfetch_control.TEXY_SFETCHINGp) {
      uint32_t addr = 0;

      addr |= reg.sfetch_control.VONU_SFETCH_S1p_D4.state;

      auto line = bit_pack(reg.sprite_lbus);

      if (!reg.oam_temp_b.YZOS_OAM_DB6p) line ^= 0b1111;

      auto tile = bit_pack(reg.oam_temp_a);

      if (reg.reg_lcdc.XYMO_LCDC_SPSIZEn) {
        addr |= (tile & 0b11111111) << 4;
        addr |= (line & 0b01111) << 1;
      }
      else {
        addr |= (tile & 0b11111110) << 4;
        addr |= (line & 0b11111) << 1;
      }

      bit_unpack_inv(&reg.vram_abus.lo.BUS_VRAM_A00n, 13, addr);
    }

    //--------------------------------------------
    // Vram address pin driver

    bit_unpack_inv(reg.vram_ext_abus, bit_pack_inv(reg.vram_abus));

    //--------------------------------------------
    // CPU bus to Vram data bus

    if (reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK && reg.XYMU_RENDERINGn && cpu_addr_vram_new && reg.cpu_signals.SIG_IN_CPU_WRp) {
      bit_unpack(reg.vram_dbus, bit_pack(reg.cpu_dbus));
    }

    //--------------------------------------------
    // Vram control pins

    if (reg.XYMU_RENDERINGn) {
      reg.vram_ext_ctrl.PIN_43_VRAM_CSn = (cpu_addr_vram_new && gen_clk_new(0b00111111) && reg.cpu_signals.SIG_IN_CPU_EXT_BUSp) || dma_addr_vram_new;
      reg.vram_ext_ctrl.PIN_45_VRAM_OEn = (!cpu_addr_vram_new || !reg.cpu_signals.SIG_IN_CPU_WRp) || dma_addr_vram_new;
      reg.vram_ext_ctrl.PIN_49_VRAM_WRn = cpu_addr_vram_new && gen_clk_new(0b00001110) && reg.cpu_signals.SIG_IN_CPU_WRp && reg.cpu_signals.SIG_IN_CPU_EXT_BUSp;
    }
    else {
      reg.vram_ext_ctrl.PIN_45_VRAM_OEn = dma_addr_vram_new || reg.tfetch_control.LONY_FETCHINGp || (reg.sfetch_control.TEXY_SFETCHINGp && (!reg.sfetch_control.TYFO_SFETCH_S0p_D1 || reg.sfetch_counter.TOXE_SFETCH_S0p));
      reg.vram_ext_ctrl.PIN_49_VRAM_WRn = 0;
      reg.vram_ext_ctrl.PIN_43_VRAM_CSn = dma_addr_vram_new || reg.tfetch_control.LONY_FETCHINGp || reg.sfetch_control.TEXY_SFETCHINGp;
    }

    uint8_t data = 0xFF;

    if (reg.vram_ext_ctrl.PIN_45_VRAM_OEn) {
      data = mem.vid_ram[bit_pack_inv(reg.vram_ext_abus)];
    }

    //--------------------------------------------
    // Vram data pin driver

    bit_unpack(reg.vram_ext_dbus, 0);

    if (reg.vram_ext_ctrl.PIN_45_VRAM_OEn) {
      bit_unpack_inv(reg.vram_ext_dbus, mem.vid_ram[bit_pack_inv(reg.vram_ext_abus)]);
    }

    if (reg.vram_ext_ctrl.PIN_49_VRAM_WRn) {
      mem.vid_ram[bit_pack_inv(reg.vram_ext_abus)] = (uint8_t)bit_pack_inv(reg.vram_ext_dbus);
    }

    if (cpu_addr_vram_new && reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK && reg.XYMU_RENDERINGn && reg.cpu_signals.SIG_IN_CPU_WRp) {
      bit_unpack_inv(reg.vram_ext_dbus, bit_pack(reg.vram_dbus));
    }

    //--------------------------------------------

    if (reg.vram_ext_ctrl.PIN_49_VRAM_WRn) {
      mem.vid_ram[bit_pack_inv(reg.vram_ext_abus)] = (uint8_t)bit_pack_inv(reg.vram_ext_dbus);
    }

    //--------------------------------------------
    // Vram pins to vram bus

    if (!cpu_addr_vram_new || !reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK || !reg.XYMU_RENDERINGn || !reg.cpu_signals.SIG_IN_CPU_WRp) {
      bit_unpack(reg.vram_dbus, bit_pack_inv(reg.vram_ext_dbus));
    }

    //--------------------------------------------
    // Vram bus to cpu bus

    if (cpu_addr_vram_new && reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK && reg.XYMU_RENDERINGn && reg.cpu_signals.SIG_IN_CPU_RDp && reg.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
      bit_unpack(reg.cpu_dbus, bit_pack(reg.vram_dbus));
    }

    //--------------------------------------------
    // Vram bus to sprite x flipper

    uint8_t pix = (uint8_t)bit_pack(reg.vram_dbus);
    if (reg.oam_temp_b.BAXO_OAM_DB5p && reg.sfetch_control.TEXY_SFETCHINGp) pix = bit_reverse(pix);
    bit_unpack(reg.flipped_sprite, pix);
  }



  //----------------------------------------
  // oam

  {
    // this is weird, why is it always 0 when not in reset?
    reg.oam_ctrl.MAKA_LATCH_EXTp = 0;

    bit_unpack(reg.oam_abus, 0xFF);
    bit_unpack(reg.oam_dbus_a, 0xFF);
    bit_unpack(reg.oam_dbus_b, 0xFF);
    reg.oam_ctrl.SIG_OAM_CLKn  = 1;
    reg.oam_ctrl.SIG_OAM_WRn_A = 1;
    reg.oam_ctrl.SIG_OAM_WRn_B = 1;
    reg.oam_ctrl.SIG_OAM_OEn   = 1;

    //----------
    // oam address

    auto cpu_oam_rd_new = cpu_addr_oam_new && reg.cpu_signals.SIG_IN_CPU_RDp;
    auto cpu_oam_wr_new = cpu_addr_oam_new && reg.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00001110);

    auto sfetch_oam_clk_new = (reg.sfetch_counter.TULY_SFETCH_S1p || reg.sfetch_counter.TESE_SFETCH_S2p || (reg.sfetch_control.TYFO_SFETCH_S0p_D1 && !reg.sfetch_counter.TOXE_SFETCH_S0p));
    auto sfetch_oam_oen_new = (reg.sfetch_counter.TULY_SFETCH_S1p || reg.sfetch_counter.TESE_SFETCH_S2p || !reg.sfetch_control.TYFO_SFETCH_S0p_D1);

    auto sscan_oam_addr_new  = (bit_pack(reg.scan_counter) << 2) | 0b00;
    auto sfetch_oam_addr_new = (bit_pack(reg.sprite_ibus)  << 2) | 0b11;
    auto dma_oam_addr_new    = bit_pack(reg.dma_lo);

    if      (reg_new.MATU_DMA_RUNNINGp) bit_unpack_inv(reg.oam_abus, dma_oam_addr_new);
    else if (reg_new.ACYL_SCANNINGp)    bit_unpack_inv(reg.oam_abus, sscan_oam_addr_new );
    else if (!reg_new.XYMU_RENDERINGn)  bit_unpack_inv(reg.oam_abus, sfetch_oam_addr_new);
    else                                bit_unpack_inv(reg.oam_abus, cpu_addr_new);

    //----------
    // oam control signals depend on address
    // The inclusion of cpu_addr_oam_new in the SCANNING and RENDERING branches is probably a hardware bug.

    if (reg_new.MATU_DMA_RUNNINGp) {
      reg.oam_ctrl.SIG_OAM_CLKn  = gen_clk_new(0b11110000);
      reg.oam_ctrl.SIG_OAM_WRn_A = gen_clk_new(0b11110000) || !reg.oam_abus.BUS_OAM_A00n;
      reg.oam_ctrl.SIG_OAM_WRn_B = gen_clk_new(0b11110000) ||  reg.oam_abus.BUS_OAM_A00n;
      reg.oam_ctrl.SIG_OAM_OEn   = 1;
    }
    else if (reg_new.ACYL_SCANNINGp) {
      reg.oam_ctrl.SIG_OAM_CLKn  = gen_clk_new(0b10001000) && (!cpu_addr_oam_new || gen_clk_new(0b11110000));
      reg.oam_ctrl.SIG_OAM_WRn_A = 1;
      reg.oam_ctrl.SIG_OAM_WRn_B = 1;
      reg.oam_ctrl.SIG_OAM_OEn   = gen_clk_new(0b10011001) && !(cpu_oam_rd_new && !reg.cpu_signals.SIG_IN_CPU_DBUS_FREE);
    }
    else if (!reg_new.XYMU_RENDERINGn) {
      reg.oam_ctrl.SIG_OAM_CLKn  = sfetch_oam_clk_new && (!cpu_addr_oam_new || gen_clk_new(0b11110000));
      reg.oam_ctrl.SIG_OAM_WRn_A = 1;
      reg.oam_ctrl.SIG_OAM_WRn_B = 1;
      reg.oam_ctrl.SIG_OAM_OEn   = sfetch_oam_oen_new && !(cpu_oam_rd_new && !reg.cpu_signals.SIG_IN_CPU_DBUS_FREE);
    }
    else if (cpu_addr_oam_new) {
      reg.oam_ctrl.SIG_OAM_CLKn  = gen_clk_new(0b11110000);
      reg.oam_ctrl.SIG_OAM_WRn_A = !cpu_oam_wr_new || !reg.oam_abus.BUS_OAM_A00n;
      reg.oam_ctrl.SIG_OAM_WRn_B = !cpu_oam_wr_new ||  reg.oam_abus.BUS_OAM_A00n;
      reg.oam_ctrl.SIG_OAM_OEn   = !reg.cpu_signals.SIG_IN_CPU_RDp || reg.cpu_signals.SIG_IN_CPU_DBUS_FREE;
    }

    //----------
    // the actual oam read

    if (!reg.oam_ctrl.SIG_OAM_OEn) {
      uint8_t oam_addr_new = (uint8_t)bit_pack_inv(reg.oam_abus) >> 1;
      bit_unpack_inv(reg.oam_dbus_a, mem.oam_ram[(oam_addr_new << 1) + 0]);
      bit_unpack_inv(reg.oam_dbus_b, mem.oam_ram[(oam_addr_new << 1) + 1]);
    }

    //----------
    // latch data from oam

    bool latch_oam = false;
    if (reg_new.ACYL_SCANNINGp) latch_oam = gen_clk_new(0b01100110);
    else if (!reg_new.XYMU_RENDERINGn)         latch_oam = !sfetch_oam_oen_new;
    else                                       latch_oam = cpu_oam_rd_new && !reg.cpu_signals.SIG_IN_CPU_DBUS_FREE;

    if (latch_oam) {
      bit_unpack_inv(reg.oam_latch_a, bit_pack_inv(reg.oam_dbus_a));
      bit_unpack_inv(reg.oam_latch_b, bit_pack_inv(reg.oam_dbus_b));
    }

    //----------
    // put oam latch on cpu bus

    if (!reg_new.MATU_DMA_RUNNINGp && !reg_new.ACYL_SCANNINGp && reg_new.XYMU_RENDERINGn) {
      if (cpu_oam_rd_new && reg.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
        if (reg.oam_abus.BUS_OAM_A00n) {
          bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.oam_latch_a));
        }
        else {
          bit_unpack(reg.cpu_dbus, bit_pack_inv(reg.oam_latch_b));
        }
      }
    }

    //----------
    // if we're writing to oam, put source data on oam bus

    auto vram_data_new    = bit_pack(reg.vram_dbus);
    auto ext_data_new     = bit_pack_inv(reg.ext_dbus);
    auto cpu_oam_data_new = bit_pack(reg.cpu_dbus); // have to repack here...

    // WUJE is weird, not sure why it's necessary.
    if (gen_clk_new(0b11110000)) reg.oam_ctrl.WUJE_CPU_OAM_WRn = 1;
    if (cpu_addr_oam_new && reg.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00001110)) reg.oam_ctrl.WUJE_CPU_OAM_WRn = 0;

    if (reg_new.MATU_DMA_RUNNINGp && dma_addr_vram_new) {
      bit_unpack_inv(reg.oam_dbus_a, vram_data_new);
      bit_unpack_inv(reg.oam_dbus_b, vram_data_new);
    }
    else if (reg_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) {
      bit_unpack_inv(reg.oam_dbus_a, ext_data_new);
      bit_unpack_inv(reg.oam_dbus_b, ext_data_new);
    }
    else if (!reg_new.ACYL_SCANNINGp && reg_new.XYMU_RENDERINGn) {
      if (cpu_addr_oam_new) {
        if (!reg.oam_ctrl.WUJE_CPU_OAM_WRn) {
          bit_unpack_inv(reg.oam_dbus_a, cpu_oam_data_new);
          bit_unpack_inv(reg.oam_dbus_b, cpu_oam_data_new);
        }
      }
      else {
        bit_unpack_inv(reg.oam_dbus_a, cpu_oam_data_new);
        bit_unpack_inv(reg.oam_dbus_b, cpu_oam_data_new);
      }
    }

    //----------
    // the actual oam write

    if (reg_old.oam_ctrl.SIG_OAM_CLKn && !reg_new.oam_ctrl.SIG_OAM_CLKn) {
      uint8_t oam_addr_new = (uint8_t)bit_pack_inv(reg.oam_abus) >> 1;
      if (!reg.oam_ctrl.SIG_OAM_WRn_A) mem.oam_ram[(oam_addr_new << 1) + 0] = (uint8_t)bit_pack_inv(reg.oam_dbus_a);
      if (!reg.oam_ctrl.SIG_OAM_WRn_B) mem.oam_ram[(oam_addr_new << 1) + 1] = (uint8_t)bit_pack_inv(reg.oam_dbus_b);
    }
    reg.oam_ctrl.old_oam_clk = !reg.oam_ctrl.SIG_OAM_CLKn; // vestige of gate mode
  }

  //----------------------------------------
  // zram

  {
    wire CSp = (cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE);

    if (reg.zram_bus.clk_old && !reg.cpu_signals.TAPU_CPU_WRp && CSp) {
      mem.zero_ram[cpu_addr_new & 0x007F] = (uint8_t)bit_pack(reg_old.cpu_dbus);
    }
    reg.zram_bus.clk_old = reg.cpu_signals.TAPU_CPU_WRp;

    uint8_t data = mem.zero_ram[cpu_addr_new & 0x007F];

    if (CSp && reg.cpu_signals.TEDO_CPU_RDp) {
      bit_unpack(reg.cpu_dbus, data);
    }
  }

  //----------------------------------------
  // And finally, interrupts.

  auto pack_cpu_dbus_old = bit_pack(reg_old.cpu_dbus);
  auto pack_cpu_dbus_new = bit_pack(reg_new.cpu_dbus);
  auto pack_ie = bit_pack(reg.reg_ie);
  auto pack_if = bit_pack(reg.reg_if);
  auto pack_stat = bit_pack(reg.reg_stat);

  // FIXME this seems slightly wrong...
  if (reg_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00001110) && cpu_addr_new == 0xFF41) {
  }
  else {
    reg_new.int_ctrl.RUPO_LYC_MATCHn = 1;
  }

  // but the "reset" arm of the latch overrides the "set" arm, so it doesn't completely break?
  if (reg_new.int_ctrl.ROPO_LY_MATCH_SYNCp) {
    reg_new.int_ctrl.RUPO_LYC_MATCHn = 0;
  }

  if (cpu_addr_new == 0xFFFF && reg.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00000001)) {
    pack_ie = pack_cpu_dbus_old;
  }

  if (cpu_addr_new == 0xFF41 && reg.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(0b00000001)) {
    pack_stat = (~pack_cpu_dbus_old >> 3) & 0b00001111;
  }

  if (cpu_addr_new == 0xFF41 && reg_new.cpu_signals.SIG_IN_CPU_RDp) {
    uint8_t data = 0x80;

    data |= (!reg.XYMU_RENDERINGn || reg.lcd.POPU_y144p) << 0;
    data |= (!reg.XYMU_RENDERINGn || reg.ACYL_SCANNINGp) << 1;
    data |= (!reg.int_ctrl.RUPO_LYC_MATCHn) << 2;
    data |= (pack_stat ^ 0b1111) << 3;

    pack_cpu_dbus_new = data;
  }

  bool int_stat_old = 0;
  if (!reg_old.reg_stat.RUGU_STAT_LYI_ENn && reg_old.int_ctrl.ROPO_LY_MATCH_SYNCp) int_stat_old = 1;
  if (!reg_old.reg_stat.REFE_STAT_OAI_ENn && !reg_old.lcd.POPU_y144p && reg_old.lcd.RUTU_x113p) int_stat_old = 1;
  if (!reg_old.reg_stat.RUFO_STAT_VBI_ENn && reg_old.lcd.POPU_y144p) int_stat_old = 1;
  if (!reg_old.reg_stat.ROXE_STAT_HBI_ENn && reg_old.WODU_HBLANKp && !reg_old.lcd.POPU_y144p) int_stat_old = 1;

  bool int_lcd_old = reg_old.lcd.POPU_y144p;
  bool int_joy_old = !reg_old.joy_int.APUG_JP_GLITCH3 || !reg_old.joy_int.BATU_JP_GLITCH0;
  bool int_tim_old = reg_old.int_ctrl.MOBA_TIMER_OVERFLOWp;
  //wire int_ser_old = serial.CALY_SER_CNT3;
  bool int_ser_old = 0;

  bool int_stat_new = 0;
  if (!get_bit(pack_stat, 0) && reg_new.WODU_HBLANKp && !reg_new.lcd.POPU_y144p) int_stat_new = 1;
  if (!get_bit(pack_stat, 1) && reg_new.lcd.POPU_y144p) int_stat_new = 1;
  if (!get_bit(pack_stat, 2) && !reg_new.lcd.POPU_y144p && reg_new.lcd.RUTU_x113p) int_stat_new = 1;
  if (!get_bit(pack_stat, 3) && reg_new.int_ctrl.ROPO_LY_MATCH_SYNCp) int_stat_new = 1;

  wire int_lcd_new = reg.lcd.POPU_y144p;
  wire int_joy_new = !reg.joy_int.APUG_JP_GLITCH3 || !reg.joy_int.BATU_JP_GLITCH0;
  wire int_tim_new = reg.int_ctrl.MOBA_TIMER_OVERFLOWp;
  //wire int_ser = serial.CALY_SER_CNT3;
  wire int_ser_new = 0;

  if (!int_lcd_old  && int_lcd_new)  pack_if |= (1 << 0);
  if (!int_stat_old && int_stat_new) pack_if |= (1 << 1);
  if (!int_tim_old  && int_tim_new)  pack_if |= (1 << 2);
  if (!int_ser_old  && int_ser_new)  pack_if |= (1 << 3);
  if (!int_joy_old  && int_joy_new)  pack_if |= (1 << 4);

  // note this is an async set so it doesn't happen on the GH clock edge like other writes
  if (reg.cpu_signals.SIG_IN_CPU_WRp && (cpu_addr_new == 0xFF0F) && gen_clk_new(0b00001110)) {
    pack_if = pack_cpu_dbus_new;
  }

  // PACK IT UP!
  pack_if &= bit_pack_inv(reg_new.cpu_ack);

  if (cpu_addr_new == 0xFFFF && reg_new.cpu_signals.SIG_IN_CPU_RDp) {
    pack_cpu_dbus_new = pack_ie | 0b11100000;
  }

  if (cpu_addr_new == 0xFF0F && reg_new.cpu_signals.SIG_IN_CPU_RDp) {
    bit_unpack(reg_new.int_latch, pack_if);
    pack_cpu_dbus_new = pack_if | 0b11100000;
  }


  bit_unpack(reg_new.cpu_dbus, pack_cpu_dbus_new);


  bit_unpack(reg_new.cpu_int, pack_if);
  bit_unpack(reg_new.reg_ie, pack_ie);
  bit_unpack(reg_new.reg_if,  pack_if);
  bit_unpack(reg_new.reg_stat, pack_stat);

  //state_new.from_reg(reg_new);

  // POSTCONDITIONS

  //if (state_new.ACYL_SCANNINGp)    CHECK_P(state_new.XYMU_RENDERINGn);
  //if (!state_new.XYMU_RENDERINGn)  CHECK_N(state_new.ACYL_SCANNINGp);

  // UNPACK IT UP!
  //state_new.to_reg(reg_new);
}
