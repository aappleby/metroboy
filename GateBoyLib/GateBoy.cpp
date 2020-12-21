#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"

#include <set>

//#pragma warning(disable:4189) // local variable unused
#pragma warning(disable:4100)

void GateBoy::dump(Dumper& d) const {
  const char* phases[] = {
    "\002A_______\001",
    "\003_B______\001",
    "\002__C_____\001",
    "\003___D____\001",
    "\002____E___\001",
    "\003_____F__\001",
    "\002______G_\001",
    "\003_______H\001",
  };

  d             ("phase        : %s\n", phases[phase_total & 7]);
  d             ("sys_rst      : %d\n", sys_rst);
  d             ("sys_t1       : %d\n", sys_t1);
  d             ("sys_t2       : %d\n", sys_t2);
  d             ("sys_clken    : %d\n", sys_clken);
  d             ("sys_clkgood  : %d\n", sys_clkgood);
  d             ("sys_cpuready : %d\n", sys_clkreq);
  d             ("sys_cpu_en   : %d\n", sys_cpu_en);
  d             ("sys_fastboot : %d\n", sys_fastboot);
  d             ("sys_buttons  : %d\n", sys_buttons);
  d.dump_req    ("bus_req_old  : ", bus_req_old);
  d.dump_req    ("bus_req_new  : ", bus_req_new);
  d.dump_bytep  ("data_latch   : ", cpu_data_latch);
  d("\n");
  //d             ("gb_screen_x  : %d\n", gb_screen_x);
  //d             ("gb_screen_y  : %d\n", gb_screen_y);
  //d.dump_bitp   ("lcd_pix_lo   : ", lcd.lcd_pix_lo.state);
  //d.dump_bitp   ("lcd_pix_hi   : ", lcd.lcd_pix_hi.state);
  //d.dump_slice2p("lcd_pipe_lo  : ", lcd.lcd_pipe_lo, 8);
  //d.dump_slice2p("lcd_pipe_hi  : ", lcd.lcd_pipe_hi, 8);
  //d("\n");
  d             ("sim_time     : %f\n",      sim_time);
  d             ("phase_total  : %lld\n",    phase_total);
  d             ("phase_origin : %lld\n",    phase_origin);
  d             ("phase_delta  : %lld\n",    phase_total - phase_origin);
  d             ("pass_hash    : %016llx\n", phase_hash);
  d             ("total_hash   : %016llx\n", cumulative_hash);
}

//-----------------------------------------------------------------------------

void GateBoy::reset_to_bootrom(bool fastboot)
{
  uint8_t* _boot_buf  = boot_buf;
  size_t   _boot_size = boot_size;
  uint8_t* _cart_buf  = cart_buf;
  size_t   _cart_size = cart_size;

  memset(this, 0, sizeof(*this));

  boot_buf  = _boot_buf;
  boot_size = _boot_size;
  cart_buf  = _cart_buf;
  cart_size = _cart_size;

  sentinel1 = SENTINEL1;
  sentinel2 = SENTINEL2;
  sentinel3 = SENTINEL3;
  sentinel4 = SENTINEL4;

  cpu_bus.reset_to_bootrom();

  //----------------------------------------
  // Update the sim without ticking the clock twice to to settle initial reset signals.

  sys_fastboot = fastboot;
  sys_rst = 1;

  // FIXME? - Not sure why it takes 7 passes to stabilize.
  for (int i = 0; i < 7; i++) {
    tock_slow(0); commit_and_hash();
  }

  //----------------------------------------
  // Release reset, start clock, and sync with phase

  sys_rst = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  run_phases(2);

  CHECK_N(clk.AFUR_xxxxEFGHp.qp_old());
  CHECK_P(clk.ALEF_AxxxxFGHp.qp_old());
  CHECK_P(clk.APUK_ABxxxxGHp.qp_old());
  CHECK_P(clk.ADYK_ABCxxxxHp.qp_old());

  phase_total = 0;
  phase_origin = 0;

  //----------------------------------------
  // Wait for SIG_CPU_START

  while(!rst.SIG_CPU_STARTp.qp_old()) {
    run_phases(8);
  }

  //----------------------------------------
  // Delay to sync up with expected div value

  run_phases(16);

  //----------------------------------------
  // Fetch the first instruction in the bootrom

  dbg_read(0x0000);

  //----------------------------------------
  // We're ready to go, release the CPU so it can start running the bootrom.

  sys_clkreq = 1;
  sys_cpu_en = true;

  if (fastboot) {
    div.TERO_DIV03p.reset(1, 0);
    div.UNYK_DIV04p.reset(1, 0);
    div.UPOF_DIV15p.reset(1, 1);
  }

  memset(framebuffer, 4, sizeof(framebuffer));
}

//-----------------------------------------------------------------------------

void GateBoy::reset_to_cart() {
  reset_to_bootrom(true);

  cpu_bus.reset_to_cart();
  oam_bus.reset_to_cart();
  ext_bus.reset_to_cart();
  vram_bus.reset_to_cart();
  rst.reset_to_cart();
  clk.reset_to_cart();
  div.reset_to_cart();
  interrupts.reset_to_cart();
  serial.reset_to_cart();

  SPR_TRI_I[0].reset(0);
  SPR_TRI_I[1].reset(0);
  SPR_TRI_I[2].reset(1);
  SPR_TRI_I[3].reset(0);
  SPR_TRI_I[4].reset(1);
  SPR_TRI_I[5].reset(0);

  SPR_TRI_L[0].reset(1);
  SPR_TRI_L[1].reset(1);
  SPR_TRI_L[2].reset(1);
  SPR_TRI_L[3].reset(1);

  sprite_store.reset_to_cart();
  sprite_scanner.reset_to_cart();
  sprite_fetcher.reset_to_cart();
  reg_stat.reset_to_cart();
  ppu_reg.reset_to_cart();
  pix_count.reset_to_cart();
  pix_pipes.reset_to_cart();
  pix_pipes.reg_bgp.reset_to_cart();
  reg_lcdc.reset_to_cart();
  lcd.reset_to_cart();

  sys_rst = false;
  sys_t1 = false;
  sys_t2 = false;
  sys_clken = true;
  sys_clkgood = true;
  sys_clkreq = true;
  sys_fastboot = true;

  cpu.reset_to_cart();

  bus_req_old.addr = 0xFF50;
  bus_req_old.data = 1;
  bus_req_old.read = 0;
  bus_req_old.write = 1;

  bus_req_new.addr = 0xFF50;
  bus_req_new.data = 1;
  bus_req_new.read = 0;
  bus_req_new.write = 1;

  cpu_data_latch = 1;
  int_ack_latch = 0;
  intf_latch = 1;
  intf_latch_delay = 0;
  intf_halt_latch = 1;

  memcpy(vid_ram, vram_boot, 8192);

  zero_ram[0x7A] = 0x39;
  zero_ram[0x7B] = 0x01;
  zero_ram[0x7C] = 0x2E;

  memcpy(framebuffer, framebuffer_boot, 160*144);

  sim_time = 169.62587129999756;
  phase_total = 46880728;
  phase_origin = 46880728;
  phase_hash = 0xad3faa49f49984b4;
  cumulative_hash = 0x6e58a5c197ff4af1;

  probes.reset_to_cart();
}

//------------------------------------------------------------------------------

void GateBoy::load_cart(uint8_t* _boot_buf, size_t _boot_size,
                       uint8_t* _cart_buf, size_t _cart_size)
{
  boot_buf  = _boot_buf;
  boot_size = _boot_size;
  cart_buf  = _cart_buf;
  cart_size = _cart_size;
}

//------------------------------------------------------------------------------------------------------------------------

uint8_t GateBoy::dbg_read(int addr) {
  CHECK_P((phase_total & 7) == 0);

  Req old_req = bus_req_new;
  bool old_cpu_en = sys_cpu_en;
  sys_cpu_en = false;

  bus_req_new.addr = uint16_t(addr);
  bus_req_new.data = 0;
  bus_req_new.read = 1;
  bus_req_new.write = 0;
  run_phases(8);

  bus_req_new = old_req;
  sys_cpu_en = old_cpu_en;

  return cpu_data_latch;
}

//------------------------------------------------------------------------------

void GateBoy::dbg_write(int addr, uint8_t data) {
  CHECK_P((phase_total & 7) == 0);

  Req old_req = bus_req_new;
  bool old_cpu_en = sys_cpu_en;
  sys_cpu_en = false;

  //printf("old req %d %d %d %d\n", old_req.addr, old_req.data, old_req.read, old_req.write);

  bus_req_new.addr = uint16_t(addr);
  bus_req_new.data = data;
  bus_req_new.read = 0;
  bus_req_new.write = 1;
  run_phases(8);

  bus_req_new = old_req;
  sys_cpu_en = old_cpu_en;
}

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyOffsets {
  const int o_cpu_bus        = offsetof(GateBoy, cpu_bus);
  const int o_ext_bus        = offsetof(GateBoy, ext_bus);
  const int o_vram_bus       = offsetof(GateBoy, vram_bus);
  const int o_oam_bus        = offsetof(GateBoy, oam_bus);
  const int o_zram_bus       = offsetof(GateBoy, zram_bus);

  const int o_rst_reg        = offsetof(GateBoy, rst);
  const int o_clk_reg        = offsetof(GateBoy, clk);
  const int o_div_reg        = offsetof(GateBoy, div);
  const int o_tim_reg        = offsetof(GateBoy, timer);
  const int o_dma_reg        = offsetof(GateBoy, dma);
  const int o_int_reg        = offsetof(GateBoy, interrupts);
  const int o_joypad         = offsetof(GateBoy, joypad);
  const int o_ser_reg        = offsetof(GateBoy, serial);

  const int o_SPR_TRI_I      = offsetof(GateBoy, SPR_TRI_I);
  const int o_SPR_TRI_L      = offsetof(GateBoy, SPR_TRI_L);

  const int o_sprite_store   = offsetof(GateBoy, sprite_store);
  const int o_sprite_scanner = offsetof(GateBoy, sprite_scanner);
  const int o_sprite_fetcher = offsetof(GateBoy, sprite_fetcher);
  const int o_tile_fetcher   = offsetof(GateBoy, tile_fetcher);

  const int o_reg_lcdc       = offsetof(GateBoy, reg_lcdc);
  const int o_reg_stat       = offsetof(GateBoy, reg_stat);
  const int o_reg_scx        = offsetof(GateBoy, reg_scx );
  const int o_reg_scy        = offsetof(GateBoy, reg_scy );
  const int o_reg_wy         = offsetof(GateBoy, reg_wy  );
  const int o_reg_wx         = offsetof(GateBoy, reg_wx  );

  const int o_win_map_x      = offsetof(GateBoy, win_map_x  );
  const int o_win_line_y     = offsetof(GateBoy, win_line_y );
  const int o_win_reg        = offsetof(GateBoy, win_reg    );
  const int o_fine_scroll    = offsetof(GateBoy, fine_scroll);

  const int o_ppu_reg        = offsetof(GateBoy, ppu_reg  );
  const int o_pix_count      = offsetof(GateBoy, pix_count);
  const int o_pix_pipes      = offsetof(GateBoy, pix_pipes);
  const int o_lcd            = offsetof(GateBoy, lcd      );

} gb_offsets;

//------------------------------------------------------------------------------------------------------------------------

/*
#define INT_VBLANK_MASK  0b00000001
#define INT_STAT_MASK    0b00000010
#define INT_TIMER_MASK   0b00000100
#define INT_SERIAL_MASK  0b00001000
#define INT_JOYPAD_MASK  0b00010000
*/

void GateBoy::next_phase() {

  //----------------------------------------

  if (DELTA_HA) {
    cpu_data_latch = (uint8_t)BitBase::pack_old(8, &cpu_bus.BUS_CPU_D[0]);

    // this one latches funny, some hardware bug
    intf_halt_latch &= ~INT_TIMER_MASK;
    if (interrupts.NYBO_FF0F_D2p.qp_old()) intf_halt_latch |= INT_TIMER_MASK;

    intf_latch = 0;
    if (interrupts.LOPE_FF0F_D0p.qp_old()) intf_latch |= INT_VBLANK_MASK;
    if (interrupts.LALU_FF0F_D1p.qp_old()) intf_latch |= INT_STAT_MASK;
    if (interrupts.NYBO_FF0F_D2p.qp_old()) intf_latch |= INT_TIMER_MASK;
    if (interrupts.UBUL_FF0F_D3p.qp_old()) intf_latch |= INT_SERIAL_MASK;
    if (interrupts.ULAK_FF0F_D4p.qp_old()) intf_latch |= INT_JOYPAD_MASK;
  }

  if (DELTA_AB) {
    if (sys_cpu_en) {
      cpu.tock_ab((uint8_t)BitBase::pack_old(5, &interrupts.IE_D0), intf_latch, cpu_data_latch);
    }
  }

  if (DELTA_AB) {
    if (sys_cpu_en) {
      bus_req_new.addr  = cpu._bus_addr;
      bus_req_new.data  = cpu._bus_data;
      bus_req_new.read  = cpu._bus_read;
      bus_req_new.write = cpu._bus_write;
    }
    int_ack_latch = cpu.int_ack;
  }

  if (DELTA_DE) {
    intf_halt_latch &= ~INT_VBLANK_MASK;
    intf_halt_latch &= ~INT_STAT_MASK;
    intf_halt_latch &= ~INT_SERIAL_MASK;
    intf_halt_latch &= ~INT_JOYPAD_MASK;

    if (interrupts.LOPE_FF0F_D0p.qp_old()) intf_halt_latch |= INT_VBLANK_MASK;
    if (interrupts.LALU_FF0F_D1p.qp_old()) intf_halt_latch |= INT_STAT_MASK;
    if (interrupts.UBUL_FF0F_D3p.qp_old()) intf_halt_latch |= INT_SERIAL_MASK;
    if (interrupts.ULAK_FF0F_D4p.qp_old()) intf_halt_latch |= INT_JOYPAD_MASK;

    if (sys_cpu_en) {
      cpu.tock_de((uint8_t)BitBase::pack_old(5, &interrupts.IE_D0), intf_halt_latch);
    }
  }

  //----------------------------------------
  // Run one pass of our simulation.

  uint8_t* blob_begin = ((uint8_t*)&sentinel1) + sizeof(sentinel1);
  uint8_t* blob_end   = ((uint8_t*)&sentinel2);

  probes.begin_pass(0);
  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);

  tock_slow(0);

  probes.end_pass(false);

#ifdef CHECK_SINGLE_PASS
  uint64_t pass_hash_old = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

  static GateBoy gb_old;
  gb_old = *this;

  probes.begin_pass(1);
  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);

  tock_slow(1);
  probes.end_pass(true);
  auto& gb_new = *this;
#endif

  uint64_t phase_hash_new = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

#ifdef CHECK_SINGLE_PASS
  if (pass_hash_old != phase_hash_new) {
    LOG_Y("Sim not stable after second pass!\n");

    int start = offsetof(GateBoy, sentinel1) + sizeof(sentinel1);
    int end   = offsetof(GateBoy, sentinel2);

    uint8_t* blob_old = (uint8_t*)&gb_old;
    uint8_t* blob_new = (uint8_t*)&gb_new;

    for (int i = start; i < end; i++) {
      if (blob_old[i] != blob_new[i]) {
        printf("%06lld %04d %02d %02d\n", phase_total, i, blob_old[i], blob_new[i]);
      }
    }

    printf("\n");

    //ASSERT_P(false);
  }
#endif

  phase_total++;

  //----------
  // Done, move to the next phase.

  bus_req_old = bus_req_new;
  phase_hash = phase_hash_new;
  combine_hash(cumulative_hash, phase_hash);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_slow(int pass_index) {
  (void)pass_index;

  //-----------------------------------------------------------------------------
  // We need the sprite match result from the previous cycle, so we recalculate it here. :/

  /* p29.CEHA*/ wire _CEHA_SCANNINGp_old = not1(sprite_scanner.CENO_SCANNINGp.qn_old());
  /*#p29.BYJO*/ wire _BYJO_SCANNINGn_old = not1(_CEHA_SCANNINGp_old);
  /*#p29.AZEM*/ wire _AZEM_RENDERINGp_old = and2(ppu_reg.XYMU_RENDERINGn.qn_old(), _BYJO_SCANNINGn_old);
  /*#p29.AROR*/ wire _AROR_MATCH_ENp_old = and2(_AZEM_RENDERINGp_old, reg_lcdc.XYLO_LCDC_SPENn.qn_old());

  SpriteMatchFlag old_match = sprite_store.get_match_flags_old(_AROR_MATCH_ENp_old, pix_count);

  /* p29.FEPO*/ wire FEPO_STORE_MATCHp_old = old_match.FEPO_STORE_MATCHp();

  SpriteFirstMatch old_first_match = sprite_store.get_first_match(old_match);

  /*#p21.XENA*/ wire XENA_STORE_MATCHn_old = not1(FEPO_STORE_MATCHp_old);
  /*#p21.WODU*/ wire WODU_HBLANKp_old = and2(XENA_STORE_MATCHn_old, pix_count.XANO_PX167p_old());

  wire TAVE_PRELOAD_DONE_TRIGp_old = tile_fetcher.TAVE_PRELOAD_DONE_TRIGp_old();

  wire TEVO_FETCH_TRIGp_old = or3(win_reg.SEKO_WIN_FETCH_TRIGp_old(), win_reg.SUZU_WIN_FIRST_TILEne_old(), TAVE_PRELOAD_DONE_TRIGp_old); // Schematic wrong, this is OR

  wire BALU_LINE_RSTp_old = lcd.BALU_LINE_RSTp_old();

  wire AVAP_SCAN_DONE_TRIGp_old = sprite_scanner.AVAP_SCAN_DONE_TRIGp_old(BALU_LINE_RSTp_old);

  wire NYXU_BFETCH_RSTn_old = nor3(AVAP_SCAN_DONE_TRIGp_old, win_reg.MOSU_WIN_MODE_TRIGp_old(), TEVO_FETCH_TRIGp_old);

  wire MOCE_BFETCH_DONEn_old = tile_fetcher.MOCE_BFETCH_DONEn_old(NYXU_BFETCH_RSTn_old);
  wire LYRY_BFETCH_DONEp_old = tile_fetcher.LYRY_BFETCH_DONEp_old(NYXU_BFETCH_RSTn_old);
  wire TEKY_SFETCH_REQp_old = and4(FEPO_STORE_MATCHp_old, win_reg.TUKU_WIN_HITn_old(), LYRY_BFETCH_DONEp_old, sprite_fetcher.SOWO_SFETCH_RUNNINGn_old());

  wire WYMO_LCDC_WINENp_old = reg_lcdc.WYMO_LCDC_WINENn.qn_old();

  /* p27.ROGE*/ wire _ROGE_WY_MATCHp_old = ROGE_WY_MATCHp_old(reg_wy, lcd.reg_ly, WYMO_LCDC_WINENp_old);
  wire REJO_WY_MATCH_LATCHp_old = win_reg.REJO_WY_MATCH_LATCHp.qp_old();
  /* p27.NUKO*/ wire _NUKO_WX_MATCHp_old = NUKO_WX_MATCHp_old(pix_count, reg_wx, REJO_WY_MATCH_LATCHp_old); // FIXME old/new?

  /*#p21.XYVO*/ wire XYVO_y144p_old = lcd.reg_ly.XYVO_y144p_old();
  /* p21.RUTU*/ wire RUTU_x113p_old = lcd.reg_lx.RUTU_x113p_old();

  wire FETO_SCAN_DONEp_old = sprite_scanner.FETO_SCAN_DONEp_old();

  /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp_old = xnor2(reg_scx.DATY_SCX0n.qn_old(), fine_scroll.RYKU_FINE_CNT0.qp_old());
  /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp_old = xnor2(reg_scx.DUZU_SCX1n.qn_old(), fine_scroll.ROGA_FINE_CNT1.qp_old());
  /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp_old = xnor2(reg_scx.CYXU_SCX2n.qn_old(), fine_scroll.RUBU_FINE_CNT2.qp_old());
  /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn_old = nand4(fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_old(), _SUHA_SCX_FINE_MATCHp_old, _SYBY_SCX_FINE_MATCHp_old, _SOZU_SCX_FINE_MATCHp_old);
  /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp_old = not1(_RONE_SCX_FINE_MATCHn_old);

  /* p27.RYDY*/ wire RYDY_WIN_HITp_old = win_reg.RYDY_WIN_HITp.qp_old();

  wire RYFA_WIN_FETCHn_A_old = win_reg.RYFA_WIN_FETCHn_A.qp_old();

  wire XYDO_PX3p_old = pix_count.XYDO_PX3p.qp_old();

  //RegBGP old_reg_bgp = pix_pipes.reg_bgp;
  //RegOBP0 old_reg_obp0 = pix_pipes.reg_obp0;
  //RegOBP1 old_reg_obp1 = pix_pipes.reg_obp1;

  //-----------------------------------------------------------------------------

  rst.PIN71_RST.reset_for_pass();
  clk.PIN74_CLKGOOD.reset_for_pass();
  clk.PIN74_CLK_IN.reset_for_pass();
  rst.PIN76_T2.reset_for_pass();
  rst.PIN77_T1.reset_for_pass();

  rst.PIN71_RST.pin_in_dp(!sys_rst);
  clk.PIN74_CLKGOOD.pin_in_dp(!sys_clkgood);
  clk.PIN74_CLK_IN.pin_in_dp((phase_total & 1) && sys_clken);
  rst.PIN76_T2.pin_in_dp(!sys_t2);
  rst.PIN77_T1.pin_in_dp(!sys_t1);

  clk.SIG_CPU_CLKREQ.set_new(sys_clkreq);
  interrupts.SIG_CPU_ACK_VBLANK.set_new(wire(int_ack_latch & INT_VBLANK_MASK));
  interrupts.SIG_CPU_ACK_STAT  .set_new(wire(int_ack_latch & INT_STAT_MASK));
  interrupts.SIG_CPU_ACK_TIMER .set_new(wire(int_ack_latch & INT_TIMER_MASK));
  interrupts.SIG_CPU_ACK_SERIAL.set_new(wire(int_ack_latch & INT_SERIAL_MASK));
  interrupts.SIG_CPU_ACK_JOYPAD.set_new(wire(int_ack_latch & INT_JOYPAD_MASK));

  //----------------------------------------
  // Sys clock signals

  clk.tock(rst);

  cpu_bus.set_addr(int(phase_total), bus_req_new);
  cpu_bus.set_pins(rst, clk, int(phase_total), bus_req_new);

  div.tock(rst, clk, cpu_bus);

  wire div_trigger = sys_fastboot ? div.TERO_DIV03p.qp_new() : div.UPOF_DIV15p.qp_new();
  rst.tock(clk, sys_clkreq, sys_clkgood, div_trigger);

  reg_lcdc.write_sync(rst, cpu_bus); // LCDC. Has to be near the top as it controls the video reset signal

  wire XONA_LCDC_LCDENp_new = reg_lcdc.XONA_LCDC_LCDENn.qn_new();

  rst.set_signals(XONA_LCDC_LCDENp_new);
  clk.tock_vid(rst);

  lcd.reg_lyc.tock2(rst, clk, lcd.reg_ly);

  // Sync writes to registers
  {
    joypad.write_sync(rst, cpu_bus);
    reg_scx.write_sync(rst, cpu_bus);
    reg_scy.write_sync(rst, cpu_bus);
    lcd.reg_lyc.write_sync(rst, cpu_bus); // must be before reg_ly.tock()
    reg_stat.write_sync(rst, cpu_bus);
    timer.write_tma_sync(rst, cpu_bus);
    timer.write_tac_sync(rst, cpu_bus);
    GateBoyBootrom::write_boot_bit_sync(rst, cpu_bus);
    dma.write_dma_sync(cpu_bus);
    reg_wy.write_sync(rst, cpu_bus);
    reg_wx.write_sync(rst, cpu_bus);
    pix_pipes.reg_bgp.write_sync(cpu_bus);
    pix_pipes.reg_obp0.write_sync(cpu_bus);
    pix_pipes.reg_obp1.write_sync(cpu_bus);
    interrupts.write_ie(rst, cpu_bus);
    zram_bus.write(cpu_bus, zero_ram);
  }

  serial.tock1(rst, cpu_bus, div);

  {
    cpu_bus.BUS_CPU_D[0].reset_for_pass();
    cpu_bus.BUS_CPU_D[1].reset_for_pass();
    cpu_bus.BUS_CPU_D[2].reset_for_pass();
    cpu_bus.BUS_CPU_D[3].reset_for_pass();
    cpu_bus.BUS_CPU_D[4].reset_for_pass();
    cpu_bus.BUS_CPU_D[5].reset_for_pass();
    cpu_bus.BUS_CPU_D[6].reset_for_pass();
    cpu_bus.BUS_CPU_D[7].reset_for_pass();

    cpu_bus.set_data(int(phase_total), bus_req_new);

  }

  serial.tock2(rst, cpu_bus);
  timer.tock2(rst, clk, cpu_bus, div);
  timer.write_tima_async(rst, cpu_bus);
  dma.tock(rst, clk, cpu_bus);

  lcd.reg_lx.tock(rst, clk);
  lcd.reg_ly.tock2(rst, lcd.reg_lx);

  lcd.tock(rst, clk, XYVO_y144p_old, RUTU_x113p_old);

  /*#p21.VOGA*/ ppu_reg.VOGA_HBLANKp.dff17(clk.ALET_xBxDxFxH(), lcd.TADY_LINE_RSTn_new(), WODU_HBLANKp_old);


  /* p27.REPU*/ wire _REPU_VBLANKp = or2(lcd.PARU_VBLANKp(), rst.PYRY_VID_RSTp());
  /* p27.SARY*/ win_reg.SARY_WY_MATCHp.dff17(clk.TALU_xxCDEFxx(), rst.XAPO_VID_RSTn(), _ROGE_WY_MATCHp_old);
  /* p27.REJO*/ win_reg.REJO_WY_MATCH_LATCHp.nor_latch(win_reg.SARY_WY_MATCHp.qp_new(), _REPU_VBLANKp);


  // Sprite scanner

  /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(clk.ALET_xBxDxFxH(), lcd.BAGY_LINE_RSTn_new(), sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
  /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(clk.XUPY_ABxxEFxx(), lcd.BAGY_LINE_RSTn_new(), FETO_SCAN_DONEp_old);

  /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), lcd.BALU_LINE_RSTp_new(), sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
  /*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(rst.ATAR_VID_RSTp(), AVAP_SCAN_DONE_TRIGp);

  /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGp.dff17(clk.XUPY_ABxxEFxx(), rst.ABEZ_VID_RSTn(),  sprite_scanner.BESU_SCANNINGp.qp_old());
  /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd.CATU_LINE_P000p.qp_new(), ASEN_SCAN_DONE_TRIGp);
  /*#p28.ACYL*/ wire ACYL_SCANNINGp = and2(dma.BOGE_DMA_RUNNINGn(), sprite_scanner.BESU_SCANNINGp.qp_new());

  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(rst.TOFU_VID_RSTp(), ppu_reg.VOGA_HBLANKp.qp_new());
  /*#p21.XYMU*/ ppu_reg.XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, AVAP_SCAN_DONE_TRIGp);
  /* p24.LOBY*/ wire LOBY_RENDERINGn = not1(ppu_reg.XYMU_RENDERINGp());

  cpu_bus._XYMU_RENDERINGp.set_new(ppu_reg.XYMU_RENDERINGp());
  dma._XYMU_RENDERINGp.set_new(ppu_reg.XYMU_RENDERINGp());
  win_reg._XYMU_RENDERINGp.set_new(ppu_reg.XYMU_RENDERINGp());
  tile_fetcher._XYMU_RENDERINGp.set_new(ppu_reg.XYMU_RENDERINGp());

  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg_lcdc.WYMO_LCDC_WINENn.qn_new(), lcd.XAHY_LINE_RSTn_new(), rst.XAPO_VID_RSTn());

  /* p27.NUNU*/ win_reg.NUNU_WIN_MATCHp.dff17(clk.MEHE_AxCxExGx(), rst.XAPO_VID_RSTn(), win_reg.PYCO_WIN_MATCHp.qp_old());
  /* p27.PYNU*/ win_reg.PYNU_WIN_MODE_Ap.nor_latch(win_reg.NUNU_WIN_MATCHp.qp_new(), XOFO_WIN_RSTp);
  /* p27.NOPA*/ win_reg.NOPA_WIN_MODE_Bp.dff17(clk.ALET_xBxDxFxH(), rst.XAPO_VID_RSTn(), win_reg.PYNU_WIN_MODE_Ap.qp_new());

  // vvvvv
  /* p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp.dff17(clk.ALET_xBxDxFxH(), ppu_reg.XYMU_RENDERINGp(),     tile_fetcher.PORY_FETCH_DONEp.qp_old());
  /* p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp.dff17(clk.MYVO_AxCxExGx(), win_reg.NAFY_WIN_MODE_TRIGn(), tile_fetcher.NYKA_FETCH_DONEp.qp_old());
  /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17(clk.ALET_xBxDxFxH(), win_reg.NAFY_WIN_MODE_TRIGn(), LYRY_BFETCH_DONEp_old);
  /* p27.RYDY*/ win_reg.RYDY_WIN_HITp.set_new(nor3(win_reg.PUKU_WIN_HITn.qp_old(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), rst.PYRY_VID_RSTp()));
  /* p27.PUKU*/ win_reg.PUKU_WIN_HITn.set_new(nor2(win_reg.RYDY_WIN_HITp.qp_new(), win_reg.NUNY_WIN_MODE_TRIGp_new()));
  /* p27.RYDY*/ win_reg.RYDY_WIN_HITp.set_new(nor3(win_reg.PUKU_WIN_HITn.qp_new(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), rst.PYRY_VID_RSTp()));
  /* p27.PUKU*/ win_reg.PUKU_WIN_HITn.set_new(nor2(win_reg.RYDY_WIN_HITp.qp_new(), win_reg.NUNY_WIN_MODE_TRIGp_new()));
  // ^^^^^


  /* p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.qp_new(), LOBY_RENDERINGn);
  /*#p24.VYBO*/ wire VYBO_CLKPIPE_b = nor3(FEPO_STORE_MATCHp_old, WODU_HBLANKp_old, clk.MYVO_AxCxExGx()); // FIXME old/new - but does it really matter here?
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_b = and3(win_reg.SOCY_WIN_HITn_new(), tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_b);

  /*#p24.SEGU*/ wire _SEGU_CLKPIPE_a = not1(TYFA_CLKPIPE_b);
  /*#p24.ROXO*/ wire _ROXO_CLKPIPE_b = not1(_SEGU_CLKPIPE_a);

  /*#p27.NYZE*/ fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17(clk.MOXE_AxCxExGx(), ppu_reg.XYMU_RENDERINGp(), fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_old());
  /*#p27.PUXA*/ fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17(_ROXO_CLKPIPE_b, ppu_reg.XYMU_RENDERINGp(), _POHU_SCX_FINE_MATCHp_old);
  /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_new(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_new());
  /*#p27.PAHA*/ wire _PAHA_RENDERINGn = not1(ppu_reg.XYMU_RENDERINGp());

  /*#p27.ROXY*/ fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(_PAHA_RENDERINGn, _POVA_FINE_MATCH_TRIGp);

  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_b);
  /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  /* p21.TADY*/ wire TADY_LINE_RSTn = nor2(lcd.ATEJ_LINE_RSTp_new(), rst.TOFU_VID_RSTp());
  pix_count.tock(TADY_LINE_RSTn, SACU_CLKPIPE_evn);


  // vvvvvvvvvv
  sprite_fetcher.tock(rst, clk, ppu_reg.XYMU_RENDERINGp(), lcd.ATEJ_LINE_RSTp_new(), tile_fetcher.TAVE_PRELOAD_DONE_TRIGp_new(), TEKY_SFETCH_REQp_old);
  oam_bus.latch_to_temp_a(clk, cpu_bus, ACYL_SCANNINGp, dma.MATU_DMA_RUNNINGp.qp_new(), sprite_fetcher.XUJY_OAM_CLKENp());
  SpriteDeltaY delta = SpriteDeltaY::sub(oam_bus.oam_temp_a, lcd.reg_ly);
  /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp = delta.GESE_SCAN_MATCH_Yp(reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new());
  /* p29.CARE*/ wire _CARE_COUNT_CLKn = and3(clk.XOCE_xBCxxFGx(), sprite_scanner.CEHA_SCANNINGp(), _GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
  /* p29.DYTY*/ wire _DYTY_COUNT_CLKp = not1(_CARE_COUNT_CLKn);
  sprite_store.update_count(rst.XAPO_VID_RSTn(), clk.ZEME_AxCxExGx(), lcd.ATEJ_LINE_RSTp_new(), _DYTY_COUNT_CLKp);
  SpriteStoreFlag store_flag = sprite_store.get_store_flags(_DYTY_COUNT_CLKp);
  sprite_store.store_sprite_x(store_flag, oam_bus.oam_temp_b, lcd.ABAK_LINE_RSTp_new(), sprite_fetcher.WUTY_SFETCH_DONE_TRIGp(), old_first_match);
  sprite_store.store_sprite_index(store_flag, SPR_TRI_I);
  sprite_store.store_sprite_line (store_flag, SPR_TRI_L);
  //^^^^^

  SPR_TRI_I[0].reset_for_pass();
  SPR_TRI_I[1].reset_for_pass();
  SPR_TRI_I[2].reset_for_pass();
  SPR_TRI_I[3].reset_for_pass();
  SPR_TRI_I[4].reset_for_pass();
  SPR_TRI_I[5].reset_for_pass();
  SPR_TRI_L[0].reset_for_pass();
  SPR_TRI_L[1].reset_for_pass();
  SPR_TRI_L[2].reset_for_pass();
  SPR_TRI_L[3].reset_for_pass();

  /*#p29.AZEM*/ wire _AZEM_RENDERINGp = and2(ppu_reg.XYMU_RENDERINGp(), sprite_scanner.BYJO_SCANNINGn());
  /*#p29.AROR*/ wire _AROR_MATCH_ENp = and2(_AZEM_RENDERINGp, reg_lcdc.XYLO_LCDC_SPENn.qn_new());
  SpriteMatchFlag sprite_match = sprite_store.get_match_flags_new(_AROR_MATCH_ENp, pix_count);
  SpriteFirstMatch first_match = sprite_store.get_first_match(sprite_match);
  sprite_store.get_sprite(first_match, SPR_TRI_I, SPR_TRI_L);


  win_reg.tock(rst, clk, ppu_reg.XYMU_RENDERINGp(), TYFA_CLKPIPE_b, _NUKO_WX_MATCHp_old, fine_scroll.ROZE_FINE_COUNT_7n_old(), RYDY_WIN_HITp_old);

  /* p27.RENE*/ win_reg.RENE_WIN_FETCHn_B.dff17(clk.ALET_xBxDxFxH(), ppu_reg.XYMU_RENDERINGp(), RYFA_WIN_FETCHn_A_old);

  /* p27.TEVO*/ wire TEVO_FETCH_TRIGp = or3(win_reg.SEKO_WIN_FETCH_TRIGp_new(), win_reg.SUZU_WIN_FIRST_TILEne_new(), tile_fetcher.TAVE_PRELOAD_DONE_TRIGp_new()); // Schematic wrong, this is OR
  /* p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(AVAP_SCAN_DONE_TRIGp, win_reg.MOSU_WIN_MODE_TRIGp_new(), TEVO_FETCH_TRIGp);

  fine_scroll.tock(ppu_reg.XYMU_RENDERINGp(), TYFA_CLKPIPE_b, TEVO_FETCH_TRIGp);

  SpritePix sprite_pix = SpritePix::flip(vram_bus.BUS_VRAM_Dp, sprite_fetcher.TEXY_SFETCHINGp(), oam_bus.oam_temp_b.BAXO_OAM_DB5p.qp_old());


  oam_bus.latch_to_temp_b(clk, cpu_bus, ACYL_SCANNINGp, dma.MATU_DMA_RUNNINGp.qp_new(), sprite_fetcher.XUJY_OAM_CLKENp());

  sprite_store.oam_addr_to_sprite_index(clk.WUDA_xxCDxxGH(), ppu_reg.XYMU_RENDERINGp(), sprite_scanner.CENO_SCANNINGp.qn_new(), oam_bus.BUS_OAM_An, SPR_TRI_I);
  sprite_store.ly_to_sprite_line(sprite_match.FEPO_STORE_MATCHp(), lcd.reg_ly, oam_bus.oam_temp_a, SPR_TRI_L);

  reg_stat.tock(rst, cpu_bus, lcd.reg_lyc);

  tile_fetcher.tock(clk, vram_bus.BUS_VRAM_Dp, ppu_reg.XYMU_RENDERINGp(), NYXU_BFETCH_RSTn, MOCE_BFETCH_DONEn_old);
  sprite_fetcher.sprite_pix_a.store_sprite_pix(sprite_pix, sprite_fetcher.XADO_STORE_SPRITE_An());
  sprite_fetcher.sprite_pix_b.store_sprite_pix(sprite_pix, sprite_fetcher.PUCO_STORE_SPRITE_Bn());

  {
    pix_pipes.tock_bgw_pipe   (tile_fetcher.tile_temp_a, tile_fetcher.tile_temp_b, SACU_CLKPIPE_evn, NYXU_BFETCH_RSTn);
    pix_pipes.tock_mask_pipe  (oam_bus.oam_temp_b, sprite_fetcher.WUTY_SFETCH_DONE_TRIGp(), SACU_CLKPIPE_evn);
    pix_pipes.tock_pal_pipe   (oam_bus.oam_temp_b, sprite_fetcher.WUTY_SFETCH_DONE_TRIGp(), SACU_CLKPIPE_evn);
    pix_pipes.tock_sprite_pipe(sprite_fetcher.sprite_pix_a, sprite_fetcher.sprite_pix_b, sprite_fetcher.WUTY_SFETCH_DONE_TRIGp(),SACU_CLKPIPE_evn);
    pix_pipes.tock_pix_output(
      reg_lcdc,
      //old_reg_bgp,
      //old_reg_obp0,
      //old_reg_obp1
      pix_pipes.reg_bgp,
      pix_pipes.reg_obp0,
      pix_pipes.reg_obp1
    );
  }

  {
    lcd.PIN50_LCD_DATA1.reset_for_pass();
    lcd.PIN51_LCD_DATA0.reset_for_pass();
    lcd.PIN54_LCD_HSYNC.reset_for_pass();
    lcd.PIN56_LCD_FLIPS.reset_for_pass();
    lcd.PIN52_LCD_CNTRL.reset_for_pass();
    lcd.PIN55_LCD_LATCH.reset_for_pass();
    lcd.PIN53_LCD_CLOCK.reset_for_pass();
    lcd.PIN57_LCD_VSYNC.reset_for_pass();

    lcd.set_pin_data(pix_pipes.REMY_LD0n.qp_new(), pix_pipes.RAVO_LD1n.qp_new());
    lcd.set_pin_ctrl(rst, clk);
    lcd.set_pin_flip(rst, div.TULU_DIV07p.qp_new(), XONA_LCDC_LCDENp_new);
    lcd.set_pin_vsync(rst);
    lcd.set_pin_hsync(rst, TYFA_CLKPIPE_b, ppu_reg.XYMU_RENDERINGp(), XYDO_PX3p_old, AVAP_SCAN_DONE_TRIGp);
    lcd.set_pin_latch(div, reg_lcdc);
    lcd.set_pin_clock(pix_count, fine_scroll, WEGO_HBLANKp, SACU_CLKPIPE_evn);

    lcd.update_lcd_pipe();
    update_framebuffer();
  }

  joypad.tock2(rst, clk, sys_buttons);

  sprite_scanner.tock(clk.XUPY_ABxxEFxx(), lcd.ANOM_LINE_RSTn_new());

  win_map_x.tock(rst, TEVO_FETCH_TRIGp, win_reg.PORE_WIN_MODEp(), reg_lcdc.WYMO_LCDC_WINENn.qn_new(), lcd.XAHY_LINE_RSTn_new());
  win_line_y.tock(rst, win_reg.PORE_WIN_MODEp(), lcd.PARU_VBLANKp());

  {
    /*#p21.WODU*/ wire WODU_HBLANKp = and2(sprite_match.XENA_STORE_MATCHn(), pix_count.XANO_PX167p_new()); // WODU goes high on odd, cleared on H
    interrupts.tock(rst, cpu_bus, joypad, reg_stat, lcd.reg_lyc, serial, lcd.PARU_VBLANKp(), lcd.reg_lx.PURE_LINE_ENDn(), timer.MOBA_TIMER_OVERFLOWp.qp_new(), WODU_HBLANKp);
  }

  /*#p01.AGUT*/ wire _AGUT_xxCDEFGH = or_and3(clk.AROV_xxCDEFxx(), clk.AJAX_xxxxEFGH(), cpu_bus.SIG_CPU_EXT_BUSp.qp_new());
  /*#p01.AWOD*/ wire _AWOD_ABxxxxxx = nor2(rst.UNOR_MODE_DBG2p(), _AGUT_xxCDEFGH);
  /*#p01.ABUZ*/ wire _ABUZ_EXT_RAM_CS_CLK = not1(_AWOD_ABxxxxxx);

  {
    ext_bus.PIN80_CSn.reset_for_pass();
    ext_bus.PIN79_RDn.reset_for_pass();
    ext_bus.PIN78_WRn.reset_for_pass();

    ext_bus.PIN01_ADDR[ 0].reset_for_pass();
    ext_bus.PIN01_ADDR[ 1].reset_for_pass();
    ext_bus.PIN01_ADDR[ 2].reset_for_pass();
    ext_bus.PIN01_ADDR[ 3].reset_for_pass();
    ext_bus.PIN01_ADDR[ 4].reset_for_pass();
    ext_bus.PIN01_ADDR[ 5].reset_for_pass();
    ext_bus.PIN01_ADDR[ 6].reset_for_pass();
    ext_bus.PIN01_ADDR[ 7].reset_for_pass();
    ext_bus.PIN01_ADDR[ 8].reset_for_pass();
    ext_bus.PIN01_ADDR[ 9].reset_for_pass();
    ext_bus.PIN01_ADDR[10].reset_for_pass();
    ext_bus.PIN01_ADDR[11].reset_for_pass();
    ext_bus.PIN01_ADDR[12].reset_for_pass();
    ext_bus.PIN01_ADDR[13].reset_for_pass();
    ext_bus.PIN01_ADDR[14].reset_for_pass();
    ext_bus.PIN01_ADDR[15].reset_for_pass();

    // FIXME this is slightly weird
    ext_bus.PIN17_DATA[0].state = 0b00110000;
    ext_bus.PIN17_DATA[1].state = 0b00110000;
    ext_bus.PIN17_DATA[2].state = 0b00110000;
    ext_bus.PIN17_DATA[3].state = 0b00110000;
    ext_bus.PIN17_DATA[4].state = 0b00110000;
    ext_bus.PIN17_DATA[5].state = 0b00110000;
    ext_bus.PIN17_DATA[6].state = 0b00110000;
    ext_bus.PIN17_DATA[7].state = 0b00110000;

    ext_bus.set_control_pins(rst, cpu_bus, dma, _ABUZ_EXT_RAM_CS_CLK);
    ext_bus.copy_cpu_addr_to_addr_latch(rst, cpu_bus);
    ext_bus.copy_addr_latch_to_pins(rst, cpu_bus, dma, _ABUZ_EXT_RAM_CS_CLK);

    ext_bus.copy_cpu_data_to_pins(rst, cpu_bus);
    ext_bus.read_ext_to_pins(cart_buf, cart_ram, int_ram);

    ext_bus.write_pins_to_ext(cart_ram, int_ram);
    ext_bus.copy_pins_to_data_latch(cpu_bus);
    ext_bus.copy_data_latch_to_cpu_bus(cpu_bus);
  }


  {
    vram_bus.BUS_VRAM_An[ 0].reset_for_pass();
    vram_bus.BUS_VRAM_An[ 1].reset_for_pass();
    vram_bus.BUS_VRAM_An[ 2].reset_for_pass();
    vram_bus.BUS_VRAM_An[ 3].reset_for_pass();
    vram_bus.BUS_VRAM_An[ 4].reset_for_pass();
    vram_bus.BUS_VRAM_An[ 5].reset_for_pass();
    vram_bus.BUS_VRAM_An[ 6].reset_for_pass();
    vram_bus.BUS_VRAM_An[ 7].reset_for_pass();
    vram_bus.BUS_VRAM_An[ 8].reset_for_pass();
    vram_bus.BUS_VRAM_An[ 9].reset_for_pass();
    vram_bus.BUS_VRAM_An[10].reset_for_pass();
    vram_bus.BUS_VRAM_An[11].reset_for_pass();
    vram_bus.BUS_VRAM_An[12].reset_for_pass();

    vram_bus.BUS_VRAM_Dp[0].reset_for_pass();
    vram_bus.BUS_VRAM_Dp[1].reset_for_pass();
    vram_bus.BUS_VRAM_Dp[2].reset_for_pass();
    vram_bus.BUS_VRAM_Dp[3].reset_for_pass();
    vram_bus.BUS_VRAM_Dp[4].reset_for_pass();
    vram_bus.BUS_VRAM_Dp[5].reset_for_pass();
    vram_bus.BUS_VRAM_Dp[6].reset_for_pass();
    vram_bus.BUS_VRAM_Dp[7].reset_for_pass();

    vram_bus.PIN43_VRAM_CSn.reset_for_pass();
    vram_bus.PIN45_VRAM_OEn.reset_for_pass();
    vram_bus.PIN49_VRAM_WRn.reset_for_pass();

    vram_bus.PIN34_VRAM_ADDR[ 0].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[ 1].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[ 2].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[ 3].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[ 4].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[ 5].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[ 6].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[ 7].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[ 8].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[ 9].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[10].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[11].reset_for_pass();
    vram_bus.PIN34_VRAM_ADDR[12].reset_for_pass();

    vram_bus.PIN25_VRAM_DATA[0].reset_for_pass();
    vram_bus.PIN25_VRAM_DATA[1].reset_for_pass();
    vram_bus.PIN25_VRAM_DATA[2].reset_for_pass();
    vram_bus.PIN25_VRAM_DATA[3].reset_for_pass();
    vram_bus.PIN25_VRAM_DATA[4].reset_for_pass();
    vram_bus.PIN25_VRAM_DATA[5].reset_for_pass();
    vram_bus.PIN25_VRAM_DATA[6].reset_for_pass();
    vram_bus.PIN25_VRAM_DATA[7].reset_for_pass();

    auto scroll_x = BGScrollX::add(pix_count, reg_scx);
    auto scroll_y = BGScrollY::add(lcd.reg_ly, reg_scy);

    /*#p25.ROPY*/ wire _ROPY_RENDERINGn = not1(ppu_reg.XYMU_RENDERINGp());
    /*#p25.SERE*/ wire _SERE_CPU_VRAM_RDp = and2(cpu_bus.TOLE_CPU_VRAM_RDp(_ABUZ_EXT_RAM_CS_CLK), _ROPY_RENDERINGn);

    vram_bus.cpu_addr_to_vram_addr(cpu_bus.BUS_CPU_A, ppu_reg.XYMU_RENDERINGp(), dma.LUFA_DMA_VRAMp());
    vram_bus.dma_addr_to_vram_addr(dma);
    vram_bus.scroll_to_addr(scroll_x, scroll_y, tile_fetcher.POTU_BGW_MAP_READp(), win_reg.AXAD_WIN_MODEn(), reg_lcdc.XAFO_LCDC_BGMAPn.qn_new());
    vram_bus.win_to_addr(win_map_x, win_line_y, tile_fetcher.POTU_BGW_MAP_READp(), win_reg.PORE_WIN_MODEp(), reg_lcdc.WOKY_LCDC_WINMAPn.qn_new());
    vram_bus.tile_to_addr(scroll_y, win_line_y, tile_fetcher.tile_temp_b, tile_fetcher.NETA_BGW_TILE_READp(), tile_fetcher.XUHA_FETCH_HILOp(), reg_lcdc.WEXU_LCDC_BGTILEn.qn_new(), win_reg.PORE_WIN_MODEp(), win_reg.AXAD_WIN_MODEn());
    vram_bus.sprite_to_addr(SPR_TRI_L, oam_bus.oam_temp_a, oam_bus.oam_temp_b, sprite_fetcher.XUQU_SPRITE_AB(), sprite_fetcher.SAKY_SFETCHn(), ppu_reg.XYMU_RENDERINGp(), reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new());
    vram_bus.addr_to_pins();

    vram_bus.cpu_data_to_bus_data(cpu_bus, _SERE_CPU_VRAM_RDp, cpu_bus.SALE_CPU_VRAM_WRn());
    vram_bus.bus_data_to_pins(_SERE_CPU_VRAM_RDp, cpu_bus.SALE_CPU_VRAM_WRn());

    vram_bus.set_pin_cs(rst.TUTO_VRAM_DBGp(), _SERE_CPU_VRAM_RDp, dma.LUFA_DMA_VRAMp(), tile_fetcher.LENA_BFETCHINGp(), sprite_fetcher.TEXY_SFETCHINGp());
    vram_bus.set_pin_wr(rst.TUTO_VRAM_DBGp(), _SERE_CPU_VRAM_RDp, cpu_bus.TUJA_CPU_VRAM_WRp());
    vram_bus.set_pin_oe(rst.TUTO_VRAM_DBGp(), cpu_bus.SALE_CPU_VRAM_WRn(), dma.LUFA_DMA_VRAMp(), ppu_reg.XYMU_RENDERINGp(), tile_fetcher.LONY_FETCHINGp.qp_new(), sprite_fetcher.SOHO_SPR_VRAM_RDp());

    vram_bus.read_vram(vid_ram);
    vram_bus.write_vram(vid_ram);

    vram_bus.pins_to_data_bus(_SERE_CPU_VRAM_RDp, cpu_bus.SALE_CPU_VRAM_WRn());
    vram_bus.data_bus_to_cpu_bus(cpu_bus, _SERE_CPU_VRAM_RDp);
  }

  {
    for(int i = 0; i < 8; i++) {
      oam_bus.BUS_OAM_An[i].reset_for_pass();
      oam_bus.BUS_OAM_DAn[i].reset_for_pass();
      oam_bus.BUS_OAM_DBn[i].reset_for_pass();
    }

    oam_bus.dma_to_addr_bus(dma);
    oam_bus.sprite_index_to_addr_bus(dma, SPR_TRI_I, ppu_reg.XYMU_RENDERINGp());
    oam_bus.scan_index_to_addr_bus(sprite_scanner, ACYL_SCANNINGp);
    oam_bus.cpu_to_addr_bus(cpu_bus, ppu_reg.XYMU_RENDERINGp(), dma.MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp);
    oam_bus.ext_to_data_bus(dma, ext_bus.PIN17_DATA);
    oam_bus.vram_to_data_bus(dma, vram_bus.BUS_VRAM_Dp);
    oam_bus.cpu_to_data_bus(clk, cpu_bus, ppu_reg.XYMU_RENDERINGp(), dma.MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp);
    oam_bus.set_pin_clk(clk, cpu_bus, dma.MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp, sprite_fetcher.XUJY_OAM_CLKENp());
    oam_bus.set_pin_wr (rst, clk, cpu_bus, ppu_reg.XYMU_RENDERINGp(), dma.MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp);
    oam_bus.set_pin_oe (clk, cpu_bus, ACYL_SCANNINGp, sprite_fetcher.XUJA_SPR_OAM_LATCHn());
    oam_bus.tock(cpu_bus, clk.XOCE_xBCxxFGx(), ACYL_SCANNINGp, sprite_fetcher.XUJA_SPR_OAM_LATCHn(), oam_ram);
    oam_bus.latch_bus(cpu_bus, ACYL_SCANNINGp, clk.XOCE_xBCxxFGx(), sprite_fetcher.XUJA_SPR_OAM_LATCHn());
    oam_bus.latch_to_cpu(cpu_bus, dma.MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp, ppu_reg.XYMU_RENDERINGp());
  }

  //----------------------------------------
  // Async writes and reads

  {
    interrupts.read_ie(cpu_bus);
    interrupts.read_intf(cpu_bus);
    reg_stat.read(cpu_bus, ACYL_SCANNINGp, ppu_reg.XYMU_RENDERINGp(), lcd.PARU_VBLANKp());
    joypad.read(cpu_bus);
    reg_scx.read(cpu_bus);
    reg_scy.read(cpu_bus);
    dma.read_dma(cpu_bus);
    serial.read_sb(cpu_bus);
    serial.read_sc(cpu_bus);
    div.read(cpu_bus);
    GateBoyBootrom::read_bootrom(rst, cpu_bus, boot_buf);
    GateBoyBootrom::read_boot_bit(cpu_bus);
    timer.read_tima(cpu_bus);
    timer.read_tma(cpu_bus);
    timer.read_tac(cpu_bus);
    reg_lcdc.read(cpu_bus);
    lcd.reg_lyc.read(cpu_bus);
    lcd.reg_ly.read(cpu_bus);
    reg_wy.read(cpu_bus);
    reg_wx.read(cpu_bus);
    pix_pipes.reg_bgp.read(cpu_bus);
    pix_pipes.reg_obp0.read(cpu_bus);
    pix_pipes.reg_obp1.read(cpu_bus);
    zram_bus.read(cpu_bus, zero_ram);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::update_framebuffer()
{
  int lcd_x = pix_count.get_new() - 8;
  int lcd_y = lcd.reg_ly.get_new();

  if (lcd_y >= 0 && lcd_y < 144 && lcd_x >= 0 && lcd_x < 160) {
    //if (lcd_x != old_lcd_x) framebuffer[lcd_x + lcd_y * 160] = 3;

    uint8_t p0 = pix_pipes.REMY_LD0n.qn_new();
    uint8_t p1 = pix_pipes.RAVO_LD1n.qn_new();

    //uint8_t old_pix = framebuffer[lcd_x + lcd_y * 160];
    uint8_t new_pix = p0 + p1 * 2;

    framebuffer[lcd_x + lcd_y * 160] = new_pix;
  }

  old_lcd_x = lcd_x;
  old_lcd_y = lcd_y;

#if 0
  if (!lcd.old_lcd_clock.qp_old() && lcd.PIN53_LCD_CLOCK.qp_new()) {
    //printf("gb_screen_x++\n");
    gb_screen_x++;
  }
  if (lcd.PIN54_LCD_HSYNC.qp_new() || lcd.PIN55_LCD_LATCH.qp_new()) {
    //printf("gb_screen_x = 0\n");
    gb_screen_x = 0;
  }

  if (!lcd.old_lcd_latch.qp_old() && lcd.PIN55_LCD_LATCH.qp_new()) {
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

    if (lcd.PIN57_LCD_VSYNC.qp_new()) {
      //printf("gb_screen_y = 0\n");
      gb_screen_y = 0;
    }
    else {
      //printf("gb_screen_y++\n");
      gb_screen_y++;
    }
  }

  lcd.old_lcd_clock.set_new(lcd.PIN53_LCD_CLOCK.qp_new());
  lcd.old_lcd_latch.set_new(lcd.PIN55_LCD_LATCH.qp_new());
#endif
}

//------------------------------------------------------------------------------------------------------------------------















//------------------------------------------------------------------------------------------------------------------------
// Debug stuff I disabled

#if 0

/* p07.APET*/ wire APET_MODE_DBG = or2(clk_reg.UMUT_MODE_DBG1p(), UNOR_MODE_DBG2p); // suggests UMUTp
/* p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A[ 5], BUS_CPU_A[ 6], TAPU_CPUWR, ADDR_111111110xx00000);

//----------
// weird debug things, probably not right

/* p05.AXYN*/ wire AXYN_xBCDEFGH = not1(clk_reg.BEDO_Axxxxxxx);
/* p05.ADYR*/ wire ADYR_Axxxxxxx = not1(AXYN_xBCDEFGH);
/* p05.APYS*/ wire APYS_xBCDEFGH = nor4(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
/* p05.AFOP*/ wire AFOP_Axxxxxxx = not1(APYS_xBCDEFGH);
/* p07.LECO*/ wire LECO_xBCDEFGH = nor4(clk_reg.BEDO_Axxxxxxx, sys_sig.MODE_DBG2);

if (AFOP_Axxxxxxx) set_data(
  /* p05.ANOC*/ not1(GND),
  /* p05.ATAJ*/ not1(GND),
  /* p05.AJEC*/ not1(GND),
  /* p05.ASUZ*/ not1(GND),
  /* p05.BENU*/ not1(GND),
  /* p05.AKAJ*/ not1(GND),
  /* p05.ARAR*/ not1(GND),
  /* p05.BEDA*/ not1(GND)
);

if (LECO_xBCDEFGH) set_data(
  /* p07.ROMY*/ GND,
  /* p07.RYNE*/ GND,
  /* p07.REJY*/ GND,
  /* p07.RASE*/ GND,
  /* p07.REKA*/ GND,
  /* p07.ROWE*/ GND,
  /* p07.RYKE*/ GND,
  /* p07.RARU*/ GND
);



//----------
// more debug stuff

/* p25.TUSO*/ wire TUSO = nor4(MODE_DBG2, dff20.SIG_CPU_BOGA_xBCDEFGH);
/* p25.SOLE*/ wire SOLE = not1(TUSO);

if (VYPO_GND) bus_out.set_data(
  /* p25.TOVU*/ SOLE,
  /* p25.SOSA*/ SOLE,
  /* p25.SEDU*/ SOLE,
  /* p25.TAXO*/ SOLE,
  /* p25.TAHY*/ SOLE,
  /* p25.TESU*/ SOLE,
  /* p25.TAZU*/ SOLE,
  /* p25.TEWA*/ SOLE
);

///* p05.KORE*/ wire P05_NC0 = nand2(KERU_DBG_FF00_D7, FF60_0);
///* p05.KYWE*/ wire P05_NC1 = nor4 (KERU_DBG_FF00_D7, FF60_0o);

/* p08.LYRA*/ wire DBG_D_RDn = nand2(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
/* p08.TUTY*/ if (DBG_D_RDn) BUS_CPU_D[0] = not1(/* p08.TOVO*/ not1(pins.PIN17_D0_C));
/* p08.SYWA*/ if (DBG_D_RDn) BUS_CPU_D[1] = not1(/* p08.RUZY*/ not1(pins.PIN18_D1_C));
/* p08.SUGU*/ if (DBG_D_RDn) BUS_CPU_D[2] = not1(/* p08.ROME*/ not1(pins.PIN19_D2_C));
/* p08.TAWO*/ if (DBG_D_RDn) BUS_CPU_D[3] = not1(/* p08.SAZA*/ not1(pins.PIN20_D3_C));
/* p08.TUTE*/ if (DBG_D_RDn) BUS_CPU_D[4] = not1(/* p08.TEHE*/ not1(pins.PIN21_D4_C));
/* p08.SAJO*/ if (DBG_D_RDn) BUS_CPU_D[5] = not1(/* p08.RATU*/ not1(pins.PIN22_D5_C));
/* p08.TEMY*/ if (DBG_D_RDn) BUS_CPU_D[6] = not1(/* p08.SOCA*/ not1(pins.PIN23_D6_C));
/* p08.ROPA*/ if (DBG_D_RDn) BUS_CPU_D[7] = not1(/* p08.RYBA*/ not1(pins.PIN24_D7_C));

// hack, not correct
{
  // FF60 debug state
  /* p07.APET*/ wire APET_MODE_DBG = or2(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /* p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A[ 5], BUS_CPU_A[ 6], bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /* p05.KURA*/ wire FF60_0n = not1(BURO_FF60_0);
  /* p05.JEVA*/ wire FF60_0o = not1(BURO_FF60_0);
  /* p07.BURO*/ BURO_FF60_0 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D[0]);
  /* p07.AMUT*/ AMUT_FF60_1 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D[1]);

  ///* p05.KURA*/ wire FF60_0n = not1(FF60);
  ///* p05.JEVA*/ wire FF60_0o = not1(FF60);
}


// so the address bus is technically a tribuf, but we're going to ignore
// this debug circuit for now.
{
  // If we're in debug mode 2, drive external address bus onto internal address

  /*#p08.KOVA*/ wire KOVA_A00p = not1(PIN01_EXT_A[ 0].qn_new());
  /* p08.CAMU*/ wire CAMU_A01p = not1(PIN01_EXT_A[ 1].qn_new());
  /* p08.BUXU*/ wire BUXU_A02p = not1(PIN01_EXT_A[ 2].qn_new());
  /* p08.BASE*/ wire BASE_A03p = not1(PIN01_EXT_A[ 3].qn_new());
  /* p08.AFEC*/ wire AFEC_A04p = not1(PIN01_EXT_A[ 4].qn_new());
  /* p08.ABUP*/ wire ABUP_A05p = not1(PIN01_EXT_A[ 5].qn_new());
  /* p08.CYGU*/ wire CYGU_A06p = not1(PIN01_EXT_A[ 6].qn_new());
  /* p08.COGO*/ wire COGO_A07p = not1(PIN01_EXT_A[ 7].qn_new());
  /* p08.MUJY*/ wire MUJY_A08p = not1(PIN01_EXT_A[ 8].qn_new());
  /* p08.NENA*/ wire NENA_A09p = not1(PIN01_EXT_A[ 9].qn_new());
  /* p08.SURA*/ wire SURA_A10p = not1(PIN01_EXT_A[10].qn_new());
  /* p08.MADY*/ wire MADY_A11p = not1(PIN01_EXT_A[11].qn_new());
  /* p08.LAHE*/ wire LAHE_A12p = not1(PIN01_EXT_A[12].qn_new());
  /* p08.LURA*/ wire LURA_A13p = not1(PIN01_EXT_A[13].qn_new());
  /* p08.PEVO*/ wire PEVO_A14p = not1(PIN01_EXT_A[14].qn_new());
  /* p08.RAZA*/ wire RAZA_A15p = not1(PIN01_EXT_A[15].qn_new());

  // KEJO_01 << KOVA_02
  // KEJO_02
  // KEJO_03
  // KEJO_04 << TOVA_02
  // KEJO_05 << KOVA_02
  // KEJO_06
  // KEJO_07
  // KEJO_08
  // KEJO_09 >> BUS_CPU_A[ 0]p
  // KEJO_10

  /* p08.KEJO*/ BUS_CPU_A[ 0] = tribuf_10np(TOVA_MODE_DBG2n, KOVA_A00p);
  /* p08.BYXE*/ BUS_CPU_A[ 1] = tribuf_10np(TOVA_MODE_DBG2n, CAMU_A01p);
  /* p08.AKAN*/ BUS_CPU_A[ 2] = tribuf_10np(TOVA_MODE_DBG2n, BUXU_A02p);
  /* p08.ANAR*/ BUS_CPU_A[ 3] = tribuf_10np(TOVA_MODE_DBG2n, BASE_A03p);
  /* p08.AZUV*/ BUS_CPU_A[ 4] = tribuf_10np(TOVA_MODE_DBG2n, AFEC_A04p);
  /* p08.AJOV*/ BUS_CPU_A[ 5] = tribuf_10np(TOVA_MODE_DBG2n, ABUP_A05p);
  /* p08.BYNE*/ BUS_CPU_A[ 6] = tribuf_10np(TOVA_MODE_DBG2n, CYGU_A06p);
  /* p08.BYNA*/ BUS_CPU_A[ 7] = tribuf_10np(TOVA_MODE_DBG2n, COGO_A07p);
  /* p08.LOFA*/ BUS_CPU_A[ 8] = tribuf_10np(TOVA_MODE_DBG2n, MUJY_A08p);
  /* p08.MAPU*/ BUS_CPU_A[ 9] = tribuf_10np(TOVA_MODE_DBG2n, NENA_A09p);
  /* p08.RALA*/ BUS_CPU_A[10] = tribuf_10np(TOVA_MODE_DBG2n, SURA_A10p);
  /* p08.LORA*/ BUS_CPU_A[11] = tribuf_10np(TOVA_MODE_DBG2n, MADY_A11p);
  /* p08.LYNA*/ BUS_CPU_A[12] = tribuf_10np(TOVA_MODE_DBG2n, LAHE_A12p);
  /* p08.LEFY*/ BUS_CPU_A[13] = tribuf_10np(TOVA_MODE_DBG2n, LURA_A13p);
  /* p08.NEFE*/ BUS_CPU_A[14] = tribuf_10np(TOVA_MODE_DBG2n, PEVO_A14p);
  /* p08.SYZU*/ BUS_CPU_A[15] = tribuf_10np(TOVA_MODE_DBG2n, RAZA_A15p);
}

//------------------------------------------------------------------------------------------------------------------------

#endif
