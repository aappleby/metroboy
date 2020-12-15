#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"

#include <set>

//#pragma warning(disable:4189) // local variable unused
#pragma warning(disable:4100)

//-----------------------------------------------------------------------------

void GateBoy::reset_boot(uint8_t* _boot_buf, size_t _boot_size,
                            uint8_t* _cart_buf, size_t _cart_size,
                            bool fastboot)
{
  memset(this, 0, sizeof(*this));
  sentinel1 = SENTINEL1;
  sentinel2 = SENTINEL2;
  sentinel3 = SENTINEL3;
  sentinel4 = SENTINEL4;

  boot_buf  = _boot_buf;
  boot_size = _boot_size;
  cart_buf  = _cart_buf;
  cart_size = _cart_size;

  // Run the sim twice to settle initial reset signals.

  tock_slow(0);
  commit_and_hash();
  tock_slow(0);
  commit_and_hash();

  //----------------------------------------
  // In reset

  sys_rst = 1;
  sys_fastboot = fastboot;
  run(5);

  // Out of reset
  // Start clock and sync with phase
  sys_rst = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  run(3);

  CHECK_N(clk.AFUR_xxxxEFGHp.qp_old());
  CHECK_P(clk.ALEF_AxxxxFGHp.qp_old());
  CHECK_P(clk.APUK_ABxxxxGHp.qp_old());
  CHECK_P(clk.ADYK_ABCxxxxHp.qp_old());

  // Wait for PIN_CPU_START
  while(!rst.PIN_CPU_STARTp.qp_any()) {
    run(8);
  }

  // Delay to sync w/ expected div value after bootrom
  run(8);
  run(8);

  // Done, initialize bus with whatever the CPU wants.
  cpu.reset_boot();
  sys_clkreq = 1;
  sys_cpu_en = true;

  if (fastboot) {
    div.TERO_DIV03p.reset(REG_D0C1);
    div.UNYK_DIV04p.reset(REG_D0C1);
    div.UPOF_DIV15p.reset(REG_D1C1);
  }

  // And clear the framebuffer
  memset(framebuffer, 4, sizeof(framebuffer));
}

//-----------------------------------------------------------------------------

void GateBoy::reset_cart(uint8_t* _boot_buf, size_t _boot_size, uint8_t* _cart_buf, size_t _cart_size) {
  reset_boot(_boot_buf, _boot_size, _cart_buf, _cart_size, true);

  cpu_bus.reset_cart();
  oam_latch_a.reset_cart();
  oam_latch_b.reset_cart();
  oam_temp_a.reset_cart();
  oam_temp_b.reset_cart();
  oam_bus.reset_cart();
  ext_addr_latch.reset_cart();
  ext_data_latch.reset_cart();
  ext_bus.reset_cart();
  vram_bus.reset_cart();
  rst.reset_cart();
  clk.reset_cart();
  div.reset_cart();
  interrupts.reset_cart();
  serial.reset_cart();
  sprite_counter.reset_cart();
  sprite_store.reset_cart();
  sprite_scanner.reset_cart();
  scan_counter.reset_cart();
  sprite_fetcher.reset_cart();
  reg_stat.reset_cart();
  ppu_reg.reset_cart();
  pix_count.reset_cart();
  pix_pipes.reset_cart();
  reg_bgp.reset_cart();
  reg_lcdc.reset_cart();
  reg_lx.reset_cart();
  reg_ly.reset_cart();
  reg_lyc.reset_cart();
  lcd.reset_cart();

  sys_rst = false;
  sys_t1 = false;
  sys_t2 = false;
  sys_clken = true;
  sys_clkgood = true;
  sys_clkreq = true;
  sys_cpu_en = true;
  sys_fastboot = true;

  cpu.reset_cart();

  cpu_req.addr = 0xff50;
  cpu_req.data = 1;
  cpu_req.read = 0;
  cpu_req.write = 1;
  dbg_req = {0};
  bus_req = cpu_req;

  cpu_data_latch = 0xFF;
  imask_latch = 0;

  int_vblank = true;
  int_vblank_halt = true;
  int_stat = false;
  int_stat_halt = false;
  int_timer = false;
  int_timer_halt = false;
  int_serial = false;
  int_serial_halt = false;
  int_joypad = false;
  int_joypad_halt = false;

  memcpy(vid_ram, vram_boot, 8192);

  zero_ram[0x7A] = 0x39;
  zero_ram[0x7B] = 0x01;
  zero_ram[0x7C] = 0x2E;

  memcpy(framebuffer, framebuffer_boot, 160*144);

  gb_screen_x = 159;
  gb_screen_y = 152;

  sim_time = 169.62587129999756;
  phase_total = 46880728;
  phase_hash = 0xd53410c0b6bcb522;
  cumulative_hash = 0x2532ab22e64c63aa;
}

void GateBoy::set_cart(uint8_t* _boot_buf, size_t _boot_size,
                       uint8_t* _cart_buf, size_t _cart_size)
{
  boot_buf  = _boot_buf;
  boot_size = _boot_size;
  cart_buf  = _cart_buf;
  cart_size = _cart_size;
}


//------------------------------------------------------------------------------

void GateBoy::load_post_bootrom_state() {
  load_obj("gateboy_post_bootrom.raw.dump", *this);
  check_sentinel();
  check_div();
  cart_buf = nullptr;
  cart_size = 0;
}

//------------------------------------------------------------------------------------------------------------------------

uint8_t GateBoy::dbg_read(int addr) {
  CHECK_P((phase_total & 7) == 0);

  dbg_req.addr = uint16_t(addr);
  dbg_req.data = 0;
  dbg_req.read = 1;
  dbg_req.write = 0;
  run(8);
  return cpu_data_latch;
}

//------------------------------------------------------------------------------

void GateBoy::dbg_write(int addr, uint8_t data) {
  CHECK_P((phase_total & 7) == 0);

  dbg_req.addr = uint16_t(addr);
  dbg_req.data = data;
  dbg_req.read = 0;
  dbg_req.write = 1;
  run(8);
  dbg_req = {0};
}

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyOffsets {
  const int o_cpu_bus        = offsetof(GateBoy, cpu_bus);
  const int o_oam_bus        = offsetof(GateBoy, oam_bus);
  const int o_ext_bus        = offsetof(GateBoy, ext_bus);
  const int o_vram_bus       = offsetof(GateBoy, vram_bus);
  const int o_rst_reg        = offsetof(GateBoy, rst);
  const int o_clk_reg        = offsetof(GateBoy, clk);
  const int o_div_reg        = offsetof(GateBoy, div);
  const int o_tim_reg        = offsetof(GateBoy, timer);
  const int o_dma_reg        = offsetof(GateBoy, dma);
  const int o_int_reg        = offsetof(GateBoy, interrupts);
  const int o_joypad         = offsetof(GateBoy, joypad);
  const int o_ser_reg        = offsetof(GateBoy, serial);
  const int o_sprite_store   = offsetof(GateBoy, sprite_store);
  const int o_sprite_scanner = offsetof(GateBoy, sprite_scanner);
  const int o_tile_fetcher   = offsetof(GateBoy, tile_fetcher);
  const int o_sprite_fetcher = offsetof(GateBoy, sprite_fetcher);
  const int o_pix_pipe       = offsetof(GateBoy, ppu_reg);
  const int o_lcd_reg        = offsetof(GateBoy, lcd);

  const int o_cpu            = offsetof(GateBoy, cpu);
  const int o_vid_ram        = offsetof(GateBoy, vid_ram);
  const int o_cart_ram       = offsetof(GateBoy, cart_ram);
  const int o_ext_ram        = offsetof(GateBoy, ext_ram);
  const int o_oam_ram        = offsetof(GateBoy, oam_ram);
  const int o_zero_ram       = offsetof(GateBoy, zero_ram);
  const int o_framebuffer    = offsetof(GateBoy, framebuffer);
  const int o_gb_screen_x    = offsetof(GateBoy, gb_screen_x);

} gb_offsets;

static std::set<int> bad_bits;

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::next_phase() {

  //----------------------------------------
  // Run one pass of our simulation.

  if (DELTA_AB) {
    cpu_req = cpu.bus_req;
    bus_req = {0};
    if (sys_cpu_en) bus_req = cpu_req;
    if (dbg_req.read || dbg_req.write) bus_req = dbg_req;
  }

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
#endif

  uint64_t phase_hash_new = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

#ifdef CHECK_SINGLE_PASS
  if (pass_hash_old != phase_hash_new) {
    LOG_Y("Sim not stable after second pass!\n");

    int start = offsetof(GateBoy, sentinel1) + sizeof(sentinel1);
    int end   = offsetof(GateBoy, sentinel2);

    uint8_t* blob_old = (uint8_t*)&gb_old;
    uint8_t* blob_new = (uint8_t*)this;

    for (int i = start; i < end; i++) {
      if (blob_old[i] != blob_new[i]) {
        printf("%06d %04d %02d %02d\n", phase_total, i, blob_old[i], blob_new[i]);
      }
    }

    printf("\n");

    ASSERT_P(false);
  }
#endif

  //----------------------------------------
  // Once the simulation converges, latch the data that needs to go back to the
  // CPU or test function and update the CPU if necessary.

  if (DELTA_GH) {
    cpu_data_latch = pack_u8p(8, &cpu_bus.BUS_CPU_D_out[0]);
  }

  if (DELTA_DE && sys_cpu_en) {

    uint8_t intf = 0;
    if (int_vblank_halt) intf |= INT_VBLANK_MASK;
    if (int_stat_halt)   intf |= INT_STAT_MASK;
    if (int_timer_halt)  intf |= INT_TIMER_MASK;
    if (int_serial_halt) intf |= INT_SERIAL_MASK;
    if (int_joypad_halt) intf |= INT_JOYPAD_MASK;

    cpu.tock_de(imask_latch, intf);
  }

  //----------
  // CPU updates after HA.

  if (DELTA_HA && sys_cpu_en) {

    uint8_t intf = 0;
    if (int_vblank) intf |= INT_VBLANK_MASK;
    if (int_stat)   intf |= INT_STAT_MASK;
    if (int_timer)  intf |= INT_TIMER_MASK;
    if (int_serial) intf |= INT_SERIAL_MASK;
    if (int_joypad) intf |= INT_JOYPAD_MASK;

    cpu.tock_ha(imask_latch, intf, cpu_data_latch);
  }

  //----------
  // Done, move to the next phase.

  phase_total++;
  phase_hash = phase_hash_new;
  combine_hash(cumulative_hash, phase_hash);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_slow(int pass_index) {
  (void)pass_index;

  //-----------------------------------------------------------------------------
  // We need the sprite match result from the previous cycle, so we recalculate it here. :/

  /* p29.CEHA*/ wire _CEHA_SCANNINGp_old = not1(sprite_scanner.CENO_SCANNINGp.qn_old());
  /*#p29.BYJO*/ wire _BYJO_SCANNINGn_old = not1(_CEHA_SCANNINGp_old);
  /*#p29.AZEM*/ wire _AZEM_RENDERINGp_old = and2(ppu_reg.XYMU_RENDERINGn.qn_old(), _BYJO_SCANNINGn_old);
  /*#p29.AROR*/ wire _AROR_MATCH_ENp_old = and2(_AZEM_RENDERINGp_old, reg_lcdc.XYLO_LCDC_SPENn.qn_old());

  SpriteMatch old_match = sprite_store.get_match(_AROR_MATCH_ENp_old, pix_count);

  /* p29.FEPO*/ wire FEPO_STORE_MATCHp_old = old_match.FEPO_STORE_MATCHp();

  SpriteGetFlag old_sprite_flag = old_match.get_flag();

  /*#p21.XENA*/ wire _XENA_STORE_MATCHn_old = not1(FEPO_STORE_MATCHp_old);
  /*#p21.WODU*/ wire WODU_HBLANKp_old = and2(_XENA_STORE_MATCHn_old, pix_count.XANO_PX167p());

  wire TAVE_PRELOAD_DONE_TRIGp_old = tile_fetcher.TAVE_PRELOAD_DONE_TRIGp();

  wire TEVO_FETCH_TRIGp_old = or3(win_reg.SEKO_WIN_FETCH_TRIGp(), win_reg.SUZU_WIN_FIRST_TILEne(), TAVE_PRELOAD_DONE_TRIGp_old); // Schematic wrong, this is OR

  wire BALU_LINE_RSTp_old = lcd.BALU_LINE_RSTp();

  wire AVAP_SCAN_DONE_TRIGp_old = sprite_scanner.AVAP_SCAN_DONE_TRIGp(BALU_LINE_RSTp_old);

  wire NYXU_BFETCH_RSTn_old = nor3(AVAP_SCAN_DONE_TRIGp_old, win_reg.MOSU_WIN_MODE_TRIGp(), TEVO_FETCH_TRIGp_old);

  wire MOCE_BFETCH_DONEn_old = tile_fetcher.MOCE_BFETCH_DONEn(NYXU_BFETCH_RSTn_old);
  wire LYRY_BFETCH_DONEp_old = tile_fetcher.LYRY_BFETCH_DONEp(NYXU_BFETCH_RSTn_old);
  wire TEKY_SFETCH_REQp_old = and4(FEPO_STORE_MATCHp_old, win_reg.TUKU_WIN_HITn(), LYRY_BFETCH_DONEp_old, sprite_fetcher.SOWO_SFETCH_RUNNINGn());

  //-----------------------------------------------------------------------------

  rst.PIN_SYS_RST.setp(sys_rst);
  rst.PIN_SYS_T1.setp(sys_t1);
  rst.PIN_SYS_T2.setp(sys_t2);
  clk.PIN_SYS_CLKGOOD.setp(sys_clkgood);
  clk.PIN_SYS_CLK.setp(!(phase_total & 1) && sys_clken);
  clk.PIN_SYS_CLKREQ.setp(sys_clkreq);

  if (joypad.PIN_JOY_P14.qp_old()) {
    joypad.PIN_JOY_P10.setn(sys_buttons & 0x01);
    joypad.PIN_JOY_P11.setn(sys_buttons & 0x02);
    joypad.PIN_JOY_P12.setn(sys_buttons & 0x04);
    joypad.PIN_JOY_P13.setn(sys_buttons & 0x08);
  }
  else if (joypad.PIN_JOY_P15.qp_old()) {
    joypad.PIN_JOY_P10.setn(sys_buttons & 0x10);
    joypad.PIN_JOY_P11.setn(sys_buttons & 0x20);
    joypad.PIN_JOY_P12.setn(sys_buttons & 0x40);
    joypad.PIN_JOY_P13.setn(sys_buttons & 0x80);
  }
  else {
    joypad.PIN_JOY_P10.setn(0);
    joypad.PIN_JOY_P11.setn(0);
    joypad.PIN_JOY_P12.setn(0);
    joypad.PIN_JOY_P13.setn(0);
  }

  /* hack */
  serial.PIN_SIN.pin_in(1, 1);
  serial.PIN_SCK.pin_in(0, 0);

  interrupts.PIN_CPU_ACK_VBLANK.setp(wire(cpu.int_ack & INT_VBLANK_MASK));
  interrupts.PIN_CPU_ACK_STAT  .setp(wire(cpu.int_ack & INT_STAT_MASK));
  interrupts.PIN_CPU_ACK_TIMER .setp(wire(cpu.int_ack & INT_TIMER_MASK));
  interrupts.PIN_CPU_ACK_SERIAL.setp(wire(cpu.int_ack & INT_SERIAL_MASK));
  interrupts.PIN_CPU_ACK_JOYPAD.setp(wire(cpu.int_ack & INT_JOYPAD_MASK));

  //----------------------------------------
  // Sys clock signals

  clk.tock(rst);

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  uint16_t cpu_addr = DELTA_HA ? bus_req.addr & 0x00FF : bus_req.addr;

  // PIN_CPU_RDp / PIN_CPU_WRp
  if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
    cpu_bus.PIN_CPU_RDp.setp(bus_req.read);
    cpu_bus.PIN_CPU_WRp.setp(bus_req.write);
  }
  else {
    cpu_bus.PIN_CPU_RDp.setp(0);
    cpu_bus.PIN_CPU_WRp.setp(0);
  }

  // not at all certain about this. seems to break some oam read glitches.
  if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req.read && (bus_req.addr < 0xFF00))) {
    cpu_bus.PIN_CPU_LATCH_EXT.setp(1);
  }
  else {
    cpu_bus.PIN_CPU_LATCH_EXT.setp(0);
  }

  cpu_bus.PIN_CPU_6.setp(0);

  // Data has to be driven on EFGH or we fail the wave tests
  wire BUS_CPU_OEp = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req.write;

  cpu_bus.set_addr(cpu_addr);
  cpu_bus.set_data(BUS_CPU_OEp, bus_req.data_lo);

  // This pin is weird and I'm not sure I have it right.

  {
    bool addr_ext = (bus_req.read || bus_req.write) && (bus_req.addr < 0xFE00);
    if (bus_req.addr <= 0x00FF && !cpu_bus.BOOT_BITn_h.qp_old()) addr_ext = false;

    if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      cpu_bus.PIN_CPU_EXT_BUSp.setp(addr_ext);
    }
    else {
      // This seems wrong, but it passes tests. *shrug*
      if (bus_req.addr >= 0x8000 && bus_req.addr <= 0x9FFF) {
        cpu_bus.PIN_CPU_EXT_BUSp.setp(0);
      }
      else {
        cpu_bus.PIN_CPU_EXT_BUSp.setp(addr_ext);
      }
    }
  }

  cpu_bus.PIN_CPU_ADDR_HIp.setp(SYRO_FE00_FFFF(cpu_bus.BUS_CPU_A));
  cpu_bus.PIN_CPU_UNOR_DBG.setp(rst.UNOR_MODE_DBG2p());
  cpu_bus.PIN_CPU_UMUT_DBG.setp(rst.UMUT_MODE_DBG1p());

  //----------------------------------------
  // CPU read signals

  cpu_bus.tock(rst, clk);

  div.tock(rst, clk, cpu_bus);

  wire div_trigger = sys_fastboot ? div.TERO_DIV03p.qp_new() : div.UPOF_DIV15p.qp_new();
  rst.tock(clk, sys_clkreq, sys_clkgood, div_trigger);

  GateBoyBootrom::tock(rst, cpu_bus, boot_buf);
  timer.tock(rst, clk, cpu_bus, div);
  reg_lcdc.tock(rst, cpu_bus); // LCDC. Has to be near the top as it controls the video reset signal

  rst.tock2(reg_lcdc.XONA_LCDC_LCDENn.qn_new());
  lcd._XODO_VID_RSTp = rst.XODO_VID_RSTp();

  clk.tock_vid(rst);
  dma.tock(rst, clk, cpu_bus);
  reg_lx.tock(rst, clk);
  reg_ly.tock(rst, cpu_bus, reg_lx);
  reg_lyc.tock(rst, clk, cpu_bus, reg_ly);
  lcd.tock(rst, clk, reg_lx, reg_ly);

  /*#p21.VOGA*/ ppu_reg.VOGA_HBLANKp.dff17(clk.ALET_xBxDxFxH(), lcd.TADY_LINE_RSTn(), WODU_HBLANKp_old);

  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(rst.TOFU_VID_RSTp(), ppu_reg.VOGA_HBLANKp.qp_new());

  {
    /* p27.ROGE*/ wire _ROGE_WY_MATCHp_old = ROGE_WY_MATCHp(/*old*/ reg_wy, /*old*/ reg_ly, reg_lcdc.WYMO_LCDC_WINENn.qn_old());
    /* p27.REPU*/ wire _REPU_VBLANKp = or2(lcd.PARU_VBLANKp(), rst.PYRY_VID_RSTp());

    /* p27.SARY*/ win_reg.SARY_WY_MATCHp.dff17(clk.TALU_xxCDEFxx(), rst.XAPO_VID_RSTn(), _ROGE_WY_MATCHp_old);
    /* p27.REJO*/ win_reg.REJO_WY_MATCH_LATCHp.nor_latch(win_reg.SARY_WY_MATCHp.qp_new(), _REPU_VBLANKp);
  }

  reg_wy.tock(rst, cpu_bus);
  reg_wx.tock(rst, cpu_bus);

  // Sprite scanner

  /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(clk.ALET_xBxDxFxH(), lcd.BAGY_LINE_RSTn(), sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
  /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(clk.XUPY_ABxxEFxx(), lcd.BAGY_LINE_RSTn(), /*old*/ scan_counter.FETO_SCAN_DONEp());

  /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), lcd.BALU_LINE_RSTp(), sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
  /*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(rst.ATAR_VID_RSTp(), AVAP_SCAN_DONE_TRIGp);

  /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGp.dff17(clk.XUPY_ABxxEFxx(), rst.ABEZ_VID_RSTn(),  sprite_scanner.BESU_SCANNINGp.qp_old());
  /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd.CATU_LINE_P000p.qp_new(), ASEN_SCAN_DONE_TRIGp);

  /*#p21.XYMU*/ ppu_reg.XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, AVAP_SCAN_DONE_TRIGp);
  /*#p21.XYMU*/ wire XYMU_RENDERINGp = ppu_reg.XYMU_RENDERINGn.qn_new();
  /* p24.LOBY*/ wire LOBY_RENDERINGn = not1(XYMU_RENDERINGp);

  dma._XYMU_RENDERINGp = XYMU_RENDERINGp;
  win_reg._XYMU_RENDERINGp = XYMU_RENDERINGp;
  tile_fetcher._XYMU_RENDERINGp = XYMU_RENDERINGp;

  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg_lcdc.WYMO_LCDC_WINENn.qn_new(), lcd.XAHY_LINE_RSTn(), XAPO_VID_RSTn(rst.XODO_VID_RSTp()));

  /* p27.NUNU*/ win_reg.NUNU_WIN_MATCHp.dff17(clk.MEHE_AxCxExGx(), rst.XAPO_VID_RSTn(), win_reg.PYCO_WIN_MATCHp.qp_old());
  /* p27.PYNU*/ win_reg.PYNU_WIN_MODE_Ap.nor_latch(win_reg.NUNU_WIN_MATCHp.qp_new(), XOFO_WIN_RSTp);
  /* p27.NOPA*/ win_reg.NOPA_WIN_MODE_Bp.dff17(clk.ALET_xBxDxFxH(), rst.XAPO_VID_RSTn(), win_reg.PYNU_WIN_MODE_Ap.qp_new());

  /* p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp.dff17(clk.ALET_xBxDxFxH(), XYMU_RENDERINGp,               tile_fetcher.PORY_FETCH_DONEp.qp_old());
  /* p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp.dff17(clk.MYVO_AxCxExGx(), win_reg.NAFY_WIN_MODE_TRIGn(), tile_fetcher.NYKA_FETCH_DONEp.qp_old());
  /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17(clk.ALET_xBxDxFxH(), win_reg.NAFY_WIN_MODE_TRIGn(), LYRY_BFETCH_DONEp_old);

  /* p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn.qp_old(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), rst.PYRY_VID_RSTp());
  /* p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp.qp_new(), win_reg.NUNY_WIN_MODE_TRIGp());
  /* p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn.qp_new(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), rst.PYRY_VID_RSTp());
  /* p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp.qp_new(), win_reg.NUNY_WIN_MODE_TRIGp());

  /* p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.qp_new(), LOBY_RENDERINGn);
  /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(FEPO_STORE_MATCHp_old, WODU_HBLANKp_old, clk.MYVO_AxCxExGx()); // FIXME old/new - but does it really matter here?
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(win_reg.SOCY_WIN_HITn(), tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);

  [
    this,
    XYMU_RENDERINGp,
    TYFA_CLKPIPE_odd
  ]() {
    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire _ROXO_CLKPIPE_odd = not1(_SEGU_CLKPIPE_evn);

    /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp_old = xnor2(reg_scx.DATY_SCX0n.qn_old(), fine_scroll.RYKU_FINE_CNT0.qp_old());
    /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp_old = xnor2(reg_scx.DUZU_SCX1n.qn_old(), fine_scroll.ROGA_FINE_CNT1.qp_old());
    /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp_old = xnor2(reg_scx.CYXU_SCX2n.qn_old(), fine_scroll.RUBU_FINE_CNT2.qp_old());
    /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn_old = nand4(fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_old(), _SUHA_SCX_FINE_MATCHp_old, _SYBY_SCX_FINE_MATCHp_old, _SOZU_SCX_FINE_MATCHp_old);
    /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp_old = not1(_RONE_SCX_FINE_MATCHn_old);

    /*#p27.NYZE*/ fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17(clk.MOXE_AxCxExGx(), XYMU_RENDERINGp, fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_old());
    /*#p27.PUXA*/ fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17(_ROXO_CLKPIPE_odd, XYMU_RENDERINGp, _POHU_SCX_FINE_MATCHp_old);
    /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_new(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_new());
    /*#p27.PAHA*/ wire _PAHA_RENDERINGn = not1(XYMU_RENDERINGp);

    /*#p27.ROXY*/ fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(_PAHA_RENDERINGn, _POVA_FINE_MATCH_TRIGp);
  }();

  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  /* p21.TADY*/ wire TADY_LINE_RSTn = nor2(lcd.ATEJ_LINE_RSTp(), rst.TOFU_VID_RSTp());
  pix_count.tock(TADY_LINE_RSTn, SACU_CLKPIPE_evn);

  sprite_fetcher.tock(rst, clk, XYMU_RENDERINGp, lcd.ATEJ_LINE_RSTp(), tile_fetcher.TAVE_PRELOAD_DONE_TRIGp(), TEKY_SFETCH_REQp_old);

  sprite_store.reset_matched_store(lcd.ABAK_LINE_RSTp(), sprite_fetcher.WUTY_SFETCH_DONE_TRIGp(), old_sprite_flag);

  /*#p29.AZEM*/ wire _AZEM_RENDERINGp = and2(XYMU_RENDERINGp, sprite_scanner.BYJO_SCANNINGn());
  /*#p29.AROR*/ wire _AROR_MATCH_ENp = and2(_AZEM_RENDERINGp, reg_lcdc.XYLO_LCDC_SPENn.qn_new());
  SpriteMatch sprite_match = sprite_store.get_match(_AROR_MATCH_ENp, pix_count);

  sprite_store.get_sprite(sprite_match);

  /* p27.NUKO*/ wire _NUKO_WX_MATCHp = NUKO_WX_MATCHp(pix_count, reg_wx, win_reg.REJO_WY_MATCH_LATCHp.qp_old());
  win_reg.tock(rst, clk, TYFA_CLKPIPE_odd, _NUKO_WX_MATCHp, XYMU_RENDERINGp, fine_scroll.ROZE_FINE_COUNT_7n_old());

  /* p27.RENE*/ win_reg.RENE_WIN_FETCHn_B.dff17(clk.ALET_xBxDxFxH(), XYMU_RENDERINGp, win_reg.RYFA_WIN_FETCHn_A.qp_old());

  /* p27.TEVO*/ wire TEVO_FETCH_TRIGp = or3(win_reg.SEKO_WIN_FETCH_TRIGp(), win_reg.SUZU_WIN_FIRST_TILEne(), tile_fetcher.TAVE_PRELOAD_DONE_TRIGp()); // Schematic wrong, this is OR
  /* p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(AVAP_SCAN_DONE_TRIGp, win_reg.MOSU_WIN_MODE_TRIGp(), TEVO_FETCH_TRIGp);

  fine_scroll.tock(XYMU_RENDERINGp, TYFA_CLKPIPE_odd, TEVO_FETCH_TRIGp);

  /*#p28.ACYL*/ wire ACYL_SCANNINGp = and2(dma.BOGE_DMA_RUNNINGn(), sprite_scanner.BESU_SCANNINGp.qp_new());

  oam_temp_a.latch_to_temp(clk, cpu_bus, ACYL_SCANNINGp, dma.MATU_DMA_RUNNINGp.qp_new(), sprite_fetcher.XUJY_OAM_CLKENp(), oam_latch_a);
  oam_temp_b.latch_to_temp(clk, cpu_bus, ACYL_SCANNINGp, dma.MATU_DMA_RUNNINGp.qp_new(), sprite_fetcher.XUJY_OAM_CLKENp(), oam_latch_b);

  SpriteDeltaY delta = sprite_delta_y(oam_temp_a, reg_ly);

  /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp = delta.GESE_SCAN_MATCH_Yp(reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new());
  /* p29.CARE*/ wire _CARE_COUNT_CLKn = and3(clk.XOCE_xBCxxFGx(), sprite_scanner.CEHA_SCANNINGp(), _GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
  /* p29.DYTY*/ wire _DYTY_COUNT_CLKp = not1(_CARE_COUNT_CLKn);

  sprite_counter.update_count(rst.XAPO_VID_RSTn(), clk.ZEME_AxCxExGx(), lcd.ATEJ_LINE_RSTp(), _DYTY_COUNT_CLKp);

  SpriteStoreFlag store_flag = sprite_counter.get_store_flag(_DYTY_COUNT_CLKp);

  sprite_store.store_sprite_index(store_flag);
  sprite_store.store_sprite_line (store_flag);
  sprite_store.store_sprite_x    (store_flag, oam_temp_b);

  sprite_store.get_sprite_index(clk.WUDA_xxCDxxGH(), XYMU_RENDERINGp, sprite_scanner.CENO_SCANNINGp.qn_new(), oam_bus.BUS_OAM_An);
  sprite_store.get_sprite_line(sprite_match.FEPO_STORE_MATCHp(), reg_ly, oam_temp_a);

  reg_stat.tock(rst, cpu_bus, ACYL_SCANNINGp, XYMU_RENDERINGp, lcd.PARU_VBLANKp(), reg_lyc);

  tile_fetcher.tock2(clk, XYMU_RENDERINGp, NYXU_BFETCH_RSTn, MOCE_BFETCH_DONEn_old);

  tile_temp_a.tock(vram_bus, tile_fetcher.LOMA_LATCH_TILE_DAn());
  tile_temp_b.tock(vram_bus, tile_fetcher.LABU_LATCH_TILE_DBn());

  SpriteFlipX sprite = vram_bus.get_flipped_sprite(sprite_fetcher.TEXY_SFETCHINGp(), oam_temp_b.BAXO_OAM_DB5p.qp_old());

  sprite_temp_a.store_sprite(sprite, sprite_fetcher.XADO_STORE_SPRITE_An());
  sprite_temp_b.store_sprite(sprite, sprite_fetcher.PUCO_STORE_SPRITE_Bn());

  reg_bgp.tock(cpu_bus);
  reg_obp0.tock(cpu_bus);
  reg_obp1.tock(cpu_bus);
  pix_pipes.tock(sprite_fetcher,
                 tile_temp_a,
                 tile_temp_b,
                 sprite_temp_a,
                 sprite_temp_b,
                 oam_temp_b,
                 reg_lcdc,
                 reg_bgp,
                 reg_obp0,
                 reg_obp1,
                 //XYMU_RENDERINGp,
                 SACU_CLKPIPE_evn,
                 NYXU_BFETCH_RSTn);


  lcd.set_pin_data(pix_pipes.REMY_LD0n, pix_pipes.RAVO_LD1n);
  lcd.set_pin_ctrl(rst, clk, reg_lx);
  lcd.set_pin_flip(rst, reg_lx, div.TULU_DIV07p.qp_new(), reg_lcdc.XONA_LCDC_LCDENn.qn_new());
  lcd.set_pin_vsync(rst, reg_lx, reg_ly);
  lcd.set_pin_hsync(rst, TYFA_CLKPIPE_odd, XYMU_RENDERINGp, pix_count.XYDO_PX3p.qp_old(), AVAP_SCAN_DONE_TRIGp);
  lcd.set_pin_latch(div, reg_lx, reg_lcdc);
  lcd.set_pin_clock(pix_count, fine_scroll, WEGO_HBLANKp, SACU_CLKPIPE_evn);

  update_lcd();

  joypad.tock(rst, clk, cpu_bus);
  serial.tock(rst, cpu_bus, div);
  reg_scx.tock(rst, cpu_bus);
  reg_scy.tock(rst, cpu_bus);
  scan_counter.tock(clk.XUPY_ABxxEFxx(), lcd.ANOM_LINE_RSTn());

  win_map_x.tock(rst, TEVO_FETCH_TRIGp, win_reg.PORE_WIN_MODEp(), reg_lcdc.WYMO_LCDC_WINENn.qn_new(), lcd.XAHY_LINE_RSTn());
  win_line_y.tock(rst, win_reg.PORE_WIN_MODEp(), lcd.PARU_VBLANKp());

  /*#p21.WODU*/ wire WODU_HBLANKp = and2(sprite_match.XENA_STORE_MATCHn(), pix_count.XANO_PX167p()); // WODU goes high on odd, cleared on H
  interrupts.tock(rst, cpu_bus, joypad, reg_stat, reg_lyc, serial, lcd.PARU_VBLANKp(), reg_lx.PURE_LINE_ENDn(), timer.MOBA_TIMER_OVERFLOWp.qp_new(), WODU_HBLANKp);

  /*#p01.AGUT*/ wire _AGUT_xxCDEFGH = or_and3(clk.AROV_xxCDEFxx(), clk.AJAX_xxxxEFGH(), cpu_bus.PIN_CPU_EXT_BUSp.qp_new());
  /*#p01.AWOD*/ wire _AWOD_ABxxxxxx = nor2(rst.UNOR_MODE_DBG2p(), _AGUT_xxCDEFGH);
  /*#p01.ABUZ*/ wire _ABUZ_EXT_RAM_CS_CLK = not1(_AWOD_ABxxxxxx);

  ext_addr_latch.tock(rst, cpu_bus.BUS_CPU_A, cpu_bus.TEXO_ADDR_VRAMn());
  ext_bus.addr_latch_to_pins(rst, cpu_bus, dma, ext_addr_latch, _ABUZ_EXT_RAM_CS_CLK, cpu_bus.TUTU_READ_BOOTROMp());
  ext_bus.cpu_data_to_pins(rst, cpu_bus);
  ext_bus.set_pins(rst, cpu_bus, dma, _ABUZ_EXT_RAM_CS_CLK);
  ext_bus.cart_to_pins(cart_buf, cart_ram, ext_ram);
  ext_bus.pins_to_data_latch(cpu_bus, ext_data_latch);
  ext_bus.data_latch_to_cpu_bus(cpu_bus, ext_data_latch);

  auto scroll_x = scroll_x_adder(pix_count, reg_scx);
  auto scroll_y = scroll_y_adder(reg_ly, reg_scy);

  // Ignoring debug for now
  /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp =  and2(cpu_bus.SOSE_ADDR_VRAMp(), _ABUZ_EXT_RAM_CS_CLK);
  /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand2(cpu_bus.SOSE_ADDR_VRAMp(), cpu_bus.PIN_CPU_WRp.qp_new());  // Schematic wrong, second input is PIN_CPU_WRp

  ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_VRAM_OEn.qn_new());
  ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_VRAM_CSn.qn_new());

  /*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp = /*mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, */ _TUCA_CPU_VRAM_RDp /*)*/;
  /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = /*mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp,*/       _TEGU_CPU_VRAM_WRn /*)*/;

  /*#p25.ROPY*/ wire _ROPY_RENDERINGn = not1(XYMU_RENDERINGp);
  /*#p25.SERE*/ wire _SERE_CPU_VRAM_RDp = and2(_TOLE_CPU_VRAM_RDp, _ROPY_RENDERINGn);

  vram_bus.cpu_addr_to_vram_addr(cpu_bus.BUS_CPU_A, XYMU_RENDERINGp, dma.LUFA_DMA_VRAMp());
  vram_bus.dma_addr_to_vram_addr(dma);
  vram_bus.scroll_to_addr(scroll_x, scroll_y, tile_fetcher.POTU_BGW_MAP_READp(), win_reg.AXAD_WIN_MODEn(), reg_lcdc.XAFO_LCDC_BGMAPn.qn_new());
  vram_bus.win_to_addr(win_map_x, win_line_y, tile_fetcher.POTU_BGW_MAP_READp(), win_reg.PORE_WIN_MODEp(), reg_lcdc.WOKY_LCDC_WINMAPn.qn_new());
  vram_bus.tile_to_addr(scroll_y, win_line_y, tile_temp_b, tile_fetcher.NETA_BGW_TILE_READp(), tile_fetcher.XUHA_FETCH_HILOp(), reg_lcdc.WEXU_LCDC_BGTILEn.qn_new(), win_reg.PORE_WIN_MODEp(), win_reg.AXAD_WIN_MODEn());
  vram_bus.sprite_to_addr(sprite_store, oam_temp_a, oam_temp_b, sprite_fetcher.XUQU_SPRITE_AB(), sprite_fetcher.SAKY_SFETCHn(), XYMU_RENDERINGp, reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new());
  vram_bus.addr_to_pins();
  vram_bus.cpu_data_to_bus_data(cpu_bus, _SERE_CPU_VRAM_RDp, _SALE_CPU_VRAM_WRn);
  vram_bus.bus_data_to_pins(_SERE_CPU_VRAM_RDp, _SALE_CPU_VRAM_WRn);
  vram_bus.set_pin_cs(rst.TUTO_VRAM_DBGp(), _SERE_CPU_VRAM_RDp, dma.LUFA_DMA_VRAMp(), tile_fetcher.LENA_BFETCHINGp(), sprite_fetcher.TEXY_SFETCHINGp());
  vram_bus.set_pin_wr(rst.TUTO_VRAM_DBGp(), _SERE_CPU_VRAM_RDp, cpu_bus.TUJA_CPU_VRAM_WRp());
  vram_bus.set_pin_oe(rst.TUTO_VRAM_DBGp(), _SALE_CPU_VRAM_WRn, dma.LUFA_DMA_VRAMp(), XYMU_RENDERINGp, tile_fetcher.LONY_FETCHINGp.qp_new(), sprite_fetcher.SOHO_SPR_VRAM_RDp());
  vram_bus.tock_vram(vid_ram);
  vram_bus.pins_to_data_bus(_SERE_CPU_VRAM_RDp, _SALE_CPU_VRAM_WRn);
  vram_bus.data_bus_to_cpu_bus(cpu_bus, _SERE_CPU_VRAM_RDp);

  oam_bus.dma_to_addr_bus(dma);
  oam_bus.sprite_index_to_addr_bus(dma, sprite_store, XYMU_RENDERINGp);
  oam_bus.scan_index_to_addr_bus(scan_counter, ACYL_SCANNINGp);
  oam_bus.ext_to_data_bus(dma, ext_bus.PIN_EXT_D);
  oam_bus.vram_to_data_bus(dma, vram_bus.BUS_VRAM_Dp);
  oam_bus.cpu_to_data_bus(clk, cpu_bus, XYMU_RENDERINGp, dma.MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp);
  oam_bus.update_pins(rst, clk, cpu_bus, XYMU_RENDERINGp, dma.MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp, sprite_fetcher.XUJY_OAM_CLKENp(), sprite_fetcher.XUJA_SPR_OAM_LATCHn());
  oam_bus.tock(cpu_bus, clk.XOCE_xBCxxFGx(), ACYL_SCANNINGp, sprite_fetcher.XUJA_SPR_OAM_LATCHn(), oam_ram);
  oam_latch_a.latch_bus(cpu_bus, oam_bus.BUS_OAM_DAn, ACYL_SCANNINGp, clk.XOCE_xBCxxFGx(), sprite_fetcher.XUJA_SPR_OAM_LATCHn());
  oam_latch_b.latch_bus(cpu_bus, oam_bus.BUS_OAM_DBn, ACYL_SCANNINGp, clk.XOCE_xBCxxFGx(), sprite_fetcher.XUJA_SPR_OAM_LATCHn());
  oam_latch_a.latch_to_cpu(cpu_bus, oam_bus.BUS_OAM_An, dma.MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp, XYMU_RENDERINGp);
  oam_latch_b.latch_to_cpu(cpu_bus, oam_bus.BUS_OAM_An, dma.MATU_DMA_RUNNINGp.qp_new(), ACYL_SCANNINGp, XYMU_RENDERINGp);

  {
    // ZRAM control signals are
    // clk_reg.PIN_CPU_BUKE_AxxxxxGH
    // TEDO_CPU_RDp();
    // TAPU_CPU_WRp_xxxxEFGx()
    // _SYKE_FF00_FFFFp
    // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

    uint16_t zram_addr = pack_u16p(16, cpu_bus.BUS_CPU_A);

    uint8_t zram_data_latch = pack_u8p(8, cpu_bus.BUS_CPU_D);

    wire zram_hit = (zram_addr >= 0xFF80) && (zram_addr <= 0xFFFE);

    wire zram_clk_new = nand2(cpu_bus.TAPU_CPU_WRp, zram_hit);

    if (!old_zram_clk && zram_clk_new) {
      zero_ram[zram_addr & 0x007F] = zram_data_latch;
    }
    old_zram_clk = zram_clk_new;

    if (zram_hit && cpu_bus.TEDO_CPU_RDp) {
      zram_data_latch = zero_ram[zram_addr & 0x007F];
    }

    if ((zram_addr >= 0xFF80) && (zram_addr <= 0xFFFE)) {
      cpu_bus.BUS_CPU_D_out[0].tri(cpu_bus.TEDO_CPU_RDp, wire(zram_data_latch & 0x01));
      cpu_bus.BUS_CPU_D_out[1].tri(cpu_bus.TEDO_CPU_RDp, wire(zram_data_latch & 0x02));
      cpu_bus.BUS_CPU_D_out[2].tri(cpu_bus.TEDO_CPU_RDp, wire(zram_data_latch & 0x04));
      cpu_bus.BUS_CPU_D_out[3].tri(cpu_bus.TEDO_CPU_RDp, wire(zram_data_latch & 0x08));
      cpu_bus.BUS_CPU_D_out[4].tri(cpu_bus.TEDO_CPU_RDp, wire(zram_data_latch & 0x10));
      cpu_bus.BUS_CPU_D_out[5].tri(cpu_bus.TEDO_CPU_RDp, wire(zram_data_latch & 0x20));
      cpu_bus.BUS_CPU_D_out[6].tri(cpu_bus.TEDO_CPU_RDp, wire(zram_data_latch & 0x40));
      cpu_bus.BUS_CPU_D_out[7].tri(cpu_bus.TEDO_CPU_RDp, wire(zram_data_latch & 0x80));
    }
  }

  //----------------------------------------
  // Save signals for next phase.

  if (DELTA_DE) {
    int_vblank_halt = interrupts.LOPE_FF0F_D0p.qp_new();
    int_stat_halt   = interrupts.LALU_FF0F_D1p.qp_new();
    int_serial_halt = interrupts.UBUL_FF0F_D3p.qp_new();
    int_joypad_halt = interrupts.ULAK_FF0F_D4p.qp_new();
  }

  if (DELTA_GH) {
    // this one latches funny, some hardware bug
    int_timer_halt = interrupts.NYBO_FF0F_D2p.qp_new();

    int_vblank = interrupts.LOPE_FF0F_D0p.qp_new();
    int_stat   = interrupts.LALU_FF0F_D1p.qp_new();
    int_timer  = interrupts.NYBO_FF0F_D2p.qp_new();
    int_serial = interrupts.UBUL_FF0F_D3p.qp_new();
    int_joypad = interrupts.ULAK_FF0F_D4p.qp_new();
  }

  if (DELTA_HA) {
    imask_latch = pack_u8p(5, &interrupts.IE_D0);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::update_lcd() {
  if (!lcd.old_lcd_clock && lcd.PIN_LCD_CLOCK.qp_new()) {
    gb_screen_x++;
  }
  if (lcd.PIN_LCD_HSYNC.qp_new() || lcd.PIN_LCD_LATCH.qp_new()) {
    gb_screen_x = 0;
  }

  lcd.lcd_pix_lo.nor_latch(lcd.PIN_LCD_DATA0.qp_new(), lcd.PIN_LCD_CLOCK.qp_new() | lcd.PIN_LCD_HSYNC.qp_new());
  lcd.lcd_pix_hi.nor_latch(lcd.PIN_LCD_DATA1.qp_new(), lcd.PIN_LCD_CLOCK.qp_new() | lcd.PIN_LCD_HSYNC.qp_new());

  if (!lcd.old_lcd_latch && lcd.PIN_LCD_LATCH.qp_new()) {
    if (gb_screen_y < 144) {
      for (int x = 0; x < 159; x++) {
        uint8_t p0 = lcd.lcd_pipe_lo[x + 1].qp_old();
        uint8_t p1 = lcd.lcd_pipe_hi[x + 1].qp_old();
        framebuffer[x + gb_screen_y * 160] = p0 + p1 * 2;
      }
      {
        uint8_t p0 = lcd.lcd_pix_lo.qp_new();
        uint8_t p1 = lcd.lcd_pix_hi.qp_new();
        framebuffer[159 + gb_screen_y * 160] = p0 + p1 * 2;
      }
    }

    gb_screen_y++;
    if (lcd.PIN_LCD_VSYNC.qp_new()) {
      gb_screen_y = 0;
    }
  }

  for (int i = 0; i < 159; i++) {
    lcd.lcd_pipe_lo[i].dff(!lcd.PIN_LCD_CLOCK.qp_new(), 1, 1, lcd.lcd_pipe_lo[i + 1].qp_old());
    lcd.lcd_pipe_hi[i].dff(!lcd.PIN_LCD_CLOCK.qp_new(), 1, 1, lcd.lcd_pipe_hi[i + 1].qp_old());
  }

  lcd.lcd_pipe_lo[159].dff(!lcd.PIN_LCD_CLOCK.qp_new(), 1, 1, lcd.lcd_pix_lo.qp_new());
  lcd.lcd_pipe_hi[159].dff(!lcd.PIN_LCD_CLOCK.qp_new(), 1, 1, lcd.lcd_pix_hi.qp_new());

  lcd.old_lcd_clock = lcd.PIN_LCD_CLOCK.qp_new();
  lcd.old_lcd_latch = lcd.PIN_LCD_LATCH.qp_new();

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

/* p25.TUSO*/ wire TUSO = nor4(MODE_DBG2, dff20.PIN_CPU_BOGA_xBCDEFGH);
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
/* p08.TUTY*/ if (DBG_D_RDn) BUS_CPU_D[0] = not1(/* p08.TOVO*/ not1(pins.PIN_D0_C));
/* p08.SYWA*/ if (DBG_D_RDn) BUS_CPU_D[1] = not1(/* p08.RUZY*/ not1(pins.PIN_D1_C));
/* p08.SUGU*/ if (DBG_D_RDn) BUS_CPU_D[2] = not1(/* p08.ROME*/ not1(pins.PIN_D2_C));
/* p08.TAWO*/ if (DBG_D_RDn) BUS_CPU_D[3] = not1(/* p08.SAZA*/ not1(pins.PIN_D3_C));
/* p08.TUTE*/ if (DBG_D_RDn) BUS_CPU_D[4] = not1(/* p08.TEHE*/ not1(pins.PIN_D4_C));
/* p08.SAJO*/ if (DBG_D_RDn) BUS_CPU_D[5] = not1(/* p08.RATU*/ not1(pins.PIN_D5_C));
/* p08.TEMY*/ if (DBG_D_RDn) BUS_CPU_D[6] = not1(/* p08.SOCA*/ not1(pins.PIN_D6_C));
/* p08.ROPA*/ if (DBG_D_RDn) BUS_CPU_D[7] = not1(/* p08.RYBA*/ not1(pins.PIN_D7_C));

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

  /*#p08.KOVA*/ wire KOVA_A00p = not1(PIN_EXT_A[ 0].qn_new());
  /* p08.CAMU*/ wire CAMU_A01p = not1(PIN_EXT_A[ 1].qn_new());
  /* p08.BUXU*/ wire BUXU_A02p = not1(PIN_EXT_A[ 2].qn_new());
  /* p08.BASE*/ wire BASE_A03p = not1(PIN_EXT_A[ 3].qn_new());
  /* p08.AFEC*/ wire AFEC_A04p = not1(PIN_EXT_A[ 4].qn_new());
  /* p08.ABUP*/ wire ABUP_A05p = not1(PIN_EXT_A[ 5].qn_new());
  /* p08.CYGU*/ wire CYGU_A06p = not1(PIN_EXT_A[ 6].qn_new());
  /* p08.COGO*/ wire COGO_A07p = not1(PIN_EXT_A[ 7].qn_new());
  /* p08.MUJY*/ wire MUJY_A08p = not1(PIN_EXT_A[ 8].qn_new());
  /* p08.NENA*/ wire NENA_A09p = not1(PIN_EXT_A[ 9].qn_new());
  /* p08.SURA*/ wire SURA_A10p = not1(PIN_EXT_A[10].qn_new());
  /* p08.MADY*/ wire MADY_A11p = not1(PIN_EXT_A[11].qn_new());
  /* p08.LAHE*/ wire LAHE_A12p = not1(PIN_EXT_A[12].qn_new());
  /* p08.LURA*/ wire LURA_A13p = not1(PIN_EXT_A[13].qn_new());
  /* p08.PEVO*/ wire PEVO_A14p = not1(PIN_EXT_A[14].qn_new());
  /* p08.RAZA*/ wire RAZA_A15p = not1(PIN_EXT_A[15].qn_new());

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
