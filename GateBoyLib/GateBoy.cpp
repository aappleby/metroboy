#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"

#include <set>

//#pragma warning(disable:4189) // local variable unused
#pragma warning(disable:4100)

const wire GateBoy::SIG_VCC = 1;
const wire GateBoy::SIG_GND = 0;

//-----------------------------------------------------------------------------

void GateBoy::reset_to_bootrom(bool fastboot)
{
  uint8_t* _boot_buf  = boot_buf;
  size_t   _boot_size = boot_size;
  uint8_t* _cart_buf  = cart_buf;
  size_t   _cart_size = cart_size;

  memset(this, 0, sizeof(*this));

  for (auto c = reg_begin(); c != reg_end(); c++) {
    *c = 0b00011000;
  }

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

  boot_buf  = _boot_buf;
  boot_size = _boot_size;
  cart_buf  = _cart_buf;
  cart_size = _cart_size;

  sentinel1 = SENTINEL1;
  sentinel2 = SENTINEL2;
  sentinel3 = SENTINEL3;
  sentinel4 = SENTINEL4;

  check_state_old_and_driven_or_pullup();

  sys_fastboot = fastboot;

  //----------------------------------------
  // Update the sim without ticking the clock to to settle initial reset signals.

  sys_rst = 1;

  tock_slow(0);
  commit_and_hash();

  //----------------------------------------
  // Release reset, start clock, and sync with phase

  sys_rst = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  run_phases(2);

  CHECK_N(bit(clk.AFUR_xxxxEFGHp.qp_old()));
  CHECK_P(bit(clk.ALEF_AxxxxFGHp.qp_old()));
  CHECK_P(bit(clk.APUK_ABxxxxGHp.qp_old()));
  CHECK_P(bit(clk.ADYK_ABCxxxxHp.qp_old()));

  phase_total = 0;
  phase_origin = 0;

  //----------------------------------------
  // Wait for SIG_CPU_START

  while(bit(~rst.SIG_CPU_STARTp.qp_old())) {
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
    div.TERO_DIV03p.state = 0b00011010;
    div.UNYK_DIV04p.state = 0b00011010;
    div.UPOF_DIV15p.state = 0b00011011;
  }

  memset(framebuffer, 4, sizeof(framebuffer));
}

//-----------------------------------------------------------------------------

void GateBoy::reset_to_cart() {
  reset_to_bootrom(true);

  VOGA_HBLANKp.state = 0b00011001;

  old_bus.reset_to_cart_new();
  new_bus.reset_to_cart_new();

  cpu_bus.reset_to_cart();
  oam_bus.reset_to_cart();
  ext_bus.reset_to_cart();
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
  sprite_store.reset_to_cart();
  sprite_scanner.reset_to_cart();
  sprite_fetcher.reset_to_cart();
  reg_stat.reset_to_cart();
  pix_count.reset_to_cart();
  pix_pipes.reset_to_cart();
  dma.reset_to_cart();
  reg_bgp.reset_to_cart();
  reg_obp0.reset_to_cart();
  reg_obp1.reset_to_cart();

  reg_lcdc.reset_to_cart();
  lcd.reset_to_cart();

  reg_lx.reset_to_cart();
  reg_ly.reset_to_cart();
  reg_lyc.reset_to_cart();

  check_state_old_and_driven_or_pullup();

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
  const int o_old_bus        = offsetof(GateBoy, old_bus);
  const int o_new_bus        = offsetof(GateBoy, new_bus);
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
  const int o_joypad         = offsetof(GateBoy, joy);
  const int o_ser_reg        = offsetof(GateBoy, serial);

  const int o_SPR_TRI_I      = offsetof(GateBoy, new_bus.BUS_SPR_I0);
  const int o_SPR_TRI_L      = offsetof(GateBoy, new_bus.BUS_SPR_L0);

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

  const int o_win_map_x      = offsetof(GateBoy, win_map_x);
  const int o_win_map_y      = offsetof(GateBoy, win_map_y);
  const int o_win_reg        = offsetof(GateBoy, win_reg    );
  const int o_fine_scroll    = offsetof(GateBoy, fine_scroll);

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
    cpu_data_latch = (uint8_t)BitBase::pack_old(8, &new_bus.BUS_CPU_D00p);

    // this one latches funny, some hardware bug
    intf_halt_latch &= ~INT_TIMER_MASK;
    if (bit(interrupts.NYBO_FF0F_D2p.qp_old())) intf_halt_latch |= INT_TIMER_MASK;

    intf_latch = 0;
    if (bit(interrupts.LOPE_FF0F_D0p.qp_old())) intf_latch |= INT_VBLANK_MASK;
    if (bit(interrupts.LALU_FF0F_D1p.qp_old())) intf_latch |= INT_STAT_MASK;
    if (bit(interrupts.NYBO_FF0F_D2p.qp_old())) intf_latch |= INT_TIMER_MASK;
    if (bit(interrupts.UBUL_FF0F_D3p.qp_old())) intf_latch |= INT_SERIAL_MASK;
    if (bit(interrupts.ULAK_FF0F_D4p.qp_old())) intf_latch |= INT_JOYPAD_MASK;
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

    if (bit(interrupts.LOPE_FF0F_D0p.qp_old())) intf_halt_latch |= INT_VBLANK_MASK;
    if (bit(interrupts.LALU_FF0F_D1p.qp_old())) intf_halt_latch |= INT_STAT_MASK;
    if (bit(interrupts.UBUL_FF0F_D3p.qp_old())) intf_halt_latch |= INT_SERIAL_MASK;
    if (bit(interrupts.ULAK_FF0F_D4p.qp_old())) intf_halt_latch |= INT_JOYPAD_MASK;

    if (sys_cpu_en) {
      cpu.tock_de((uint8_t)BitBase::pack_old(5, &interrupts.IE_D0), intf_halt_latch);
    }
  }

  //----------------------------------------
  // Run one pass of our simulation.

  probes.begin_pass(0);
  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);

  tock_slow(0);

#ifdef CHECK_SINGLE_PASS
  uint64_t hash_old = commit_and_hash();

  static GateBoy gb1;
  memcpy(&gb1, this, sizeof(GateBoy));

  tock_slow(1);
  auto& gb2 = *this;
#endif

  probes.end_pass(true);

  uint64_t hash_new = commit_and_hash();

#ifdef CHECK_SINGLE_PASS
  if (hash_old != hash_new) {
    LOG_Y("Sim not stable after second pass!\n");

    int start = offsetof(GateBoy, sentinel1) + sizeof(sentinel1);
    int end   = offsetof(GateBoy, sentinel2);

    uint8_t* blob_old = (uint8_t*)&gb1;
    uint8_t* blob_new = (uint8_t*)&gb2;

    for (int i = start; i < end; i++) {
      if (blob_old[i] != blob_new[i]) {
        printf("%06lld %04d %02d %02d\n", phase_total, i, blob_old[i], blob_new[i]);
      }
    }

    printf("\n");
  }
#endif

  phase_total++;

  //----------
  // Done, move to the next phase.

  bus_req_old = bus_req_new;
  phase_hash = hash_new;
  combine_hash(cumulative_hash, phase_hash);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_slow(int pass_index) {
  (void)pass_index;

  //-----------------------------------------------------------------------------
  // We need the sprite match result from the previous cycle, so we recalculate it here. :/

  /* p29.CEHA*/ wire _CEHA_SCANNINGp_old  = not1(sprite_scanner.CENO_SCANNINGn.qn_old());
  /*#p29.BYJO*/ wire _BYJO_SCANNINGn_old  = not1(_CEHA_SCANNINGp_old);
  /*#p29.AZEM*/ wire _AZEM_RENDERINGp_old = and2(XYMU_RENDERINGn.qn_old(), _BYJO_SCANNINGn_old);
  /*#p29.AROR*/ wire _AROR_MATCH_ENp_old  = and2(_AZEM_RENDERINGp_old, reg_lcdc.XYLO_LCDC_SPENn.qn_old());

  SpriteMatchFlag old_match = get_match_flags_old(_AROR_MATCH_ENp_old);

  SpriteFirstMatch old_first_match = get_first_match(old_match);

  wire TAVE_PRELOAD_DONE_TRIGp_old = this->TAVE_PRELOAD_DONE_TRIGp_old();

  wire TEVO_WIN_FETCH_TRIGp_old = or3(SEKO_WIN_FETCH_TRIGp_old(), SUZU_WIN_FIRST_TILEne_old(), TAVE_PRELOAD_DONE_TRIGp_old); // Schematic wrong, this is OR

  wire AVAP_SCAN_DONE_TRIGp_old = this->AVAP_SCAN_DONE_TRIGp_old();

  wire NYXU_BFETCH_RSTn_old = nor3(AVAP_SCAN_DONE_TRIGp_old, MOSU_WIN_MODE_TRIGp_old(), TEVO_WIN_FETCH_TRIGp_old);

  /* p27.MOCE*/ wire MOCE_BFETCH_DONEn_old = nand3(tile_fetcher._LAXU_BFETCH_S0p.qp_old(), tile_fetcher._NYVA_BFETCH_S2p.qp_old(), NYXU_BFETCH_RSTn_old);
  /* p27.LYRY*/ wire LYRY_BFETCH_DONEp_old = not1(MOCE_BFETCH_DONEn_old);

  /* p27.TEKY*/ wire TEKY_SFETCH_REQp_old = and4(FEPO_STORE_MATCHp(old_match), TUKU_WIN_HITn_old(), LYRY_BFETCH_DONEp_old, SOWO_SFETCH_RUNNINGn_old());

  auto WYMO_LCDC_WINENn_old = reg_lcdc.WYMO_LCDC_WINENn;

  wire _ROGE_WY_MATCHp_old = ROGE_WY_MATCHp_old(reg_wy, reg_ly, WYMO_LCDC_WINENn_old);
  auto REJO_WY_MATCH_LATCHp_old = win_reg.REJO_WY_MATCH_LATCHp;
  wire _NUKO_WX_MATCHp_old = NUKO_WX_MATCHp_old(pix_count, reg_wx, REJO_WY_MATCH_LATCHp_old); // FIXME old/new?

  wire XYVO_y144p_old = this->XYVO_y144p_old();
  auto RUTU_x113p_old = reg_lx.RUTU_x113p;

  //wire FETO_SCAN_DONEp_old = sprite_scanner.FETO_SCAN_DONEp_old();

  /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp_old = xnor2(reg_scx.DATY_SCX0n.qn_old(), fine_scroll.RYKU_FINE_CNT0.qp_old());
  /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp_old = xnor2(reg_scx.DUZU_SCX1n.qn_old(), fine_scroll.ROGA_FINE_CNT1.qp_old());
  /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp_old = xnor2(reg_scx.CYXU_SCX2n.qn_old(), fine_scroll.RUBU_FINE_CNT2.qp_old());
  /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn_old = nand4(fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_old(), _SUHA_SCX_FINE_MATCHp_old, _SYBY_SCX_FINE_MATCHp_old, _SOZU_SCX_FINE_MATCHp_old);
  /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp_old = not1(_RONE_SCX_FINE_MATCHn_old);

  auto RYDY_WIN_HITp_old = win_reg.RYDY_WIN_HITp; //.qp_old();

  auto RYFA_WIN_FETCHn_A_old = win_reg.RYFA_WIN_FETCHn_A; //.qp_old();

  auto XYDO_PX3p_old = pix_count.XYDO_PX3p;

  auto BAXO_OAM_DB5p_old = oam_temp_b.BAXO_OAM_DB5p;

  auto oam_temp_b_old = oam_temp_b;

  SpritePix sprite_pix_old = flip_sprite_pix(TEXY_SFETCHINGp_old(), BAXO_OAM_DB5p_old);

  //-----------------------------------------------------------------------------

  new_bus.reset_for_pass();

  //-----------------------------------------------------------------------------

  rst.PIN_71_RST.reset_for_pass();
  pins.PIN_74_CLK.reset_for_pass();
  rst.PIN_76_T2.reset_for_pass();
  rst.PIN_77_T1.reset_for_pass();

  rst.PIN_71_RST.pin_in_dp(bit(~sys_rst));
  pins.PIN_74_CLK.pin_clk(!(phase_total & 1) && sys_clken, bit(~sys_clkgood));
  rst.PIN_76_T2.pin_in_dp(bit(~sys_t2));
  rst.PIN_77_T1.pin_in_dp(bit(~sys_t1));

  clk.SIG_CPU_CLKREQ.sig_in(sys_clkreq);
  interrupts.SIG_CPU_ACK_VBLANK.sig_in(bit(int_ack_latch, BIT_VBLANK));
  interrupts.SIG_CPU_ACK_STAT  .sig_in(bit(int_ack_latch, BIT_STAT));
  interrupts.SIG_CPU_ACK_TIMER .sig_in(bit(int_ack_latch, BIT_TIMER));
  interrupts.SIG_CPU_ACK_SERIAL.sig_in(bit(int_ack_latch, BIT_SERIAL));
  interrupts.SIG_CPU_ACK_JOYPAD.sig_in(bit(int_ack_latch, BIT_JOYPAD));

  //----------------------------------------
  // Sys clock signals

  tock_clocks();

  new_bus.set_addr(int(phase_total), bus_req_new);
  set_cpu_pins();

  reg_div_tock();

  tock_reset(bit(sys_fastboot) ? div.TERO_DIV03p : div.UPOF_DIV15p);

  reg_lcdc_write(); // LCDC. Has to be near the top as it controls the video reset signal

  rst.set_signals(reg_lcdc.XONA_LCDC_LCDENn);
  tock_vid_clocks();

  reg_lyc_tock2();

  // Sync writes to registers
  {
    reg_scx_write();
    reg_scy_write();
    reg_lyc_write(); // must be before reg_ly.tock()
    reg_stat_write();
    reg_tma_write();
    reg_tac_write();
    {
      //write_boot_bit_sync();
      /* p07.TUGE*/ wire _TUGE_FF50_WRn = nand4(cpu_bus.TAPU_CPU_WRp, new_bus.SYKE_ADDR_HIp(), new_bus.TYRO_XX_0x0x0000p(), new_bus.TUFA_XX_x1x1xxxxp());
      // FF50 - disable bootrom bit

      /* p07.SATO*/ wire _SATO_BOOT_BITn_old = or2(old_bus.BUS_CPU_D00p.qp_old(), cpu_bus.TEPU_BOOT_BITn_h.qp_old());
      /* p07.TEPU*/ cpu_bus.TEPU_BOOT_BITn_h.dff17(_TUGE_FF50_WRn, ALUR_SYS_RSTn(), _SATO_BOOT_BITn_old);
    }

    reg_dma_write();
    reg_wy_write();
    reg_wx_write();
    write_ie();
    write_zram();
  }

  {
    new_bus.set_data(int(phase_total), bus_req_new);

  }

  /*#p01.AGUT*/ wire _AGUT_xxCDEFGH = or_and3(AROV_xxCDEFxx(), AJAX_xxxxEFGH(), cpu_bus.SIG_CPU_EXT_BUSp);
  /*#p01.AWOD*/ wire _AWOD_ABxxxxxx = nor2(UNOR_MODE_DBG2p(), _AGUT_xxCDEFGH);
  /*#p01.ABUZ*/ cpu_bus.ABUZ_EXT_RAM_CS_CLK = not1(_AWOD_ABxxxxxx);

  {
    //reg_lx_tock();
    /*#p21.SANU*/ wire _SANU_x113p_old = and4(reg_lx.TYRY_LX6p.qp_old(), reg_lx.TAHA_LX5p.qp_old(), reg_lx.SUDE_LX4p.qp_old(), reg_lx.SAXO_LX0p.qp_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*#p21.NYPE*/ reg_lx.NYPE_x113p.dff17(TALU_xxCDEFxx(), LYFE_VID_RSTn(), reg_lx.RUTU_x113p.qp_old());
    /*#p21.RUTU*/ reg_lx.RUTU_x113p.dff17(SONO_ABxxxxGH(), LYFE_VID_RSTn(), _SANU_x113p_old);

    /*#p21.MUDE*/ wire _MUDE_X_RSTn = nor2(reg_lx.RUTU_x113p.qp_new(), LYHA_VID_RSTp());
    /*#p21.SAXO*/ reg_lx.SAXO_LX0p.dff17(TALU_xxCDEFxx(),           _MUDE_X_RSTn, reg_lx.SAXO_LX0p.qn_old());
    /*#p21.TYPO*/ reg_lx.TYPO_LX1p.dff17(reg_lx.SAXO_LX0p.qn_new(), _MUDE_X_RSTn, reg_lx.TYPO_LX1p.qn_old());
    /*#p21.VYZO*/ reg_lx.VYZO_LX2p.dff17(reg_lx.TYPO_LX1p.qn_new(), _MUDE_X_RSTn, reg_lx.VYZO_LX2p.qn_old());
    /*#p21.TELU*/ reg_lx.TELU_LX3p.dff17(reg_lx.VYZO_LX2p.qn_new(), _MUDE_X_RSTn, reg_lx.TELU_LX3p.qn_old());
    /*#p21.SUDE*/ reg_lx.SUDE_LX4p.dff17(reg_lx.TELU_LX3p.qn_new(), _MUDE_X_RSTn, reg_lx.SUDE_LX4p.qn_old());
    /*#p21.TAHA*/ reg_lx.TAHA_LX5p.dff17(reg_lx.SUDE_LX4p.qn_new(), _MUDE_X_RSTn, reg_lx.TAHA_LX5p.qn_old());
    /*#p21.TYRY*/ reg_lx.TYRY_LX6p.dff17(reg_lx.TAHA_LX5p.qn_new(), _MUDE_X_RSTn, reg_lx.TYRY_LX6p.qn_old());
  }
  {
    //reg_ly_tock2();
    /*#p21.NOKO*/ wire _NOKO_y153p_old = and4(reg_ly.LAFO_LY7p.qp_old(), reg_ly.LOVU_LY4p.qp_old(), reg_ly.LYDO_LY3p.qp_old(), reg_ly.MUWY_LY0p.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*#p21.MYTA*/ reg_ly.MYTA_y153p.dff17(reg_lx.NYPE_x113p.qp_new(), LYFE_VID_RSTn(), _NOKO_y153p_old);

    /*#p21.LAMA*/ wire _LAMA_Y_RSTn = nor2(reg_ly.MYTA_y153p.qp_new(), LYHA_VID_RSTp());
    /*#p21.MUWY*/ reg_ly.MUWY_LY0p.dff17(reg_lx.RUTU_x113p.qp_new(), _LAMA_Y_RSTn, reg_ly.MUWY_LY0p.qn_old());
    /*#p21.MYRO*/ reg_ly.MYRO_LY1p.dff17(reg_ly.MUWY_LY0p.qn_new(),  _LAMA_Y_RSTn, reg_ly.MYRO_LY1p.qn_old());
    /*#p21.LEXA*/ reg_ly.LEXA_LY2p.dff17(reg_ly.MYRO_LY1p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LEXA_LY2p.qn_old());
    /*#p21.LYDO*/ reg_ly.LYDO_LY3p.dff17(reg_ly.LEXA_LY2p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LYDO_LY3p.qn_old());
    /*#p21.LOVU*/ reg_ly.LOVU_LY4p.dff17(reg_ly.LYDO_LY3p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LOVU_LY4p.qn_old());
    /*#p21.LEMA*/ reg_ly.LEMA_LY5p.dff17(reg_ly.LOVU_LY4p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LEMA_LY5p.qn_old());
    /*#p21.MATO*/ reg_ly.MATO_LY6p.dff17(reg_ly.LEMA_LY5p.qn_new(),  _LAMA_Y_RSTn, reg_ly.MATO_LY6p.qn_old());
    /*#p21.LAFO*/ reg_ly.LAFO_LY7p.dff17(reg_ly.MATO_LY6p.qn_new(),  _LAMA_Y_RSTn, reg_ly.LAFO_LY7p.qn_old());
  }


  {
    //tock_lcd(XYVO_y144p_old, RUTU_x113p_old);
    /*#p21.POPU*/ lcd.POPU_VBLANKp.dff17(reg_lx.NYPE_x113p.qp_new(), LYFE_VID_RSTn(), XYVO_y144p_old);

    /*#p21.PURE*/ wire _PURE_LINE_ENDn_old = not1(RUTU_x113p_old.qp_old());
    /*#p21.SELA*/ wire _SELA_LINE_P908p_old = not1(_PURE_LINE_ENDn_old);
    /*#p29.ALES*/ wire _ALES_y144n_old = not1(XYVO_y144p_old);
    /*#p29.ABOV*/ wire _ABOV_LINE_P908p_old = and2(_SELA_LINE_P908p_old, _ALES_y144n_old);
    /*#p28.ANEL*/ lcd.ANEL_LINE_P002p.dff17(AWOH_xxCDxxGH(), ABEZ_VID_RSTn(),  lcd.CATU_START_SCANNING.qp_old());
    /*#p29.CATU*/ lcd.CATU_START_SCANNING.dff17(XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), _ABOV_LINE_P908p_old);
  }

  // DMA has to tock early
  reg_dma_tock();

  //----------------------------------------
  // Sprite scanner

  /*#p21.XENA*/ wire XENA_STORE_MATCHn_old = not1(FEPO_STORE_MATCHp(old_match));
  /*#p21.WODU*/ wire WODU_HBLANKp_old = and2(XENA_STORE_MATCHn_old, XANO_PX167p_old());
  /*#p21.VOGA*/ VOGA_HBLANKp.dff17(ALET_xBxDxFxH(), TADY_LINE_RSTn_new(), WODU_HBLANKp_old);

  {
    //tock_sprite_scanner();
    /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(ALET_xBxDxFxH(), BAGY_LINE_RSTn_new(), sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
    /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(XUPY_ABxxEFxx(), BAGY_LINE_RSTn_new(), FETO_SCAN_DONEp_old());

    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp_new(), sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
    /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(ATAR_VID_RSTp(), AVAP_SCAN_DONE_TRIGp);

    /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGn.dff17(XUPY_ABxxEFxx(), ABEZ_VID_RSTn(),  sprite_scanner.BESU_SCANNINGp.qp_old());
    /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd.CATU_START_SCANNING.qp_new(), ASEN_SCAN_DONE_TRIGp);

    /* p28.YFOT*/ wire _YFOT_OAM_A2p_old = not1(old_bus.BUS_OAM_A02n.qp_old());
    /* p28.YFOC*/ wire _YFOC_OAM_A3p_old = not1(old_bus.BUS_OAM_A03n.qp_old());
    /* p28.YVOM*/ wire _YVOM_OAM_A4p_old = not1(old_bus.BUS_OAM_A04n.qp_old());
    /* p28.YMEV*/ wire _YMEV_OAM_A5p_old = not1(old_bus.BUS_OAM_A05n.qp_old());
    /* p28.XEMU*/ wire _XEMU_OAM_A6p_old = not1(old_bus.BUS_OAM_A06n.qp_old());
    /* p28.YZET*/ wire _YZET_OAM_A7p_old = not1(old_bus.BUS_OAM_A07n.qp_old());

    // Sprite store grabs the sprite index off the _old_ oam address bus
    /* p30.XADU*/ sprite_scanner.XADU_SPRITE_IDX0p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YFOT_OAM_A2p_old);
    /* p30.XEDY*/ sprite_scanner.XEDY_SPRITE_IDX1p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YFOC_OAM_A3p_old);
    /* p30.ZUZE*/ sprite_scanner.ZUZE_SPRITE_IDX2p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YVOM_OAM_A4p_old);
    /* p30.XOBE*/ sprite_scanner.XOBE_SPRITE_IDX3p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YMEV_OAM_A5p_old);
    /* p30.YDUF*/ sprite_scanner.YDUF_SPRITE_IDX4p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _XEMU_OAM_A6p_old);
    /* p30.XECU*/ sprite_scanner.XECU_SPRITE_IDX5p.dff13(WUDA_xxCDxxGH(), SIG_VCC, _YZET_OAM_A7p_old);

    /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), VOGA_HBLANKp.qp_new());
    ///*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_Bp.qp_new(), lcd.BALU_LINE_RSTp_new(), BYBA_SCAN_DONE_Ap.qn_new());
    ///*#p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
    ///*#p28.ACYL*/ wire ACYL_SCANNINGp = and2(dma.BOGE_DMA_RUNNINGn(), BESU_SCANNINGp.qp_new());
    /*#p21.XYMU*/ XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, AVAP_SCAN_DONE_TRIGp);

    /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn_new = and2(sprite_scanner.CENO_SCANNINGn.qn_new(), XYMU_RENDERINGn.qn_new());
    /*#p30.WUZY_STORE_I0*/ new_bus.BUS_SPR_I0.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XADU_SPRITE_IDX0p.qn_new());
    /* p30.WYSE_STORE_I1*/ new_bus.BUS_SPR_I1.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XEDY_SPRITE_IDX1p.qn_new());
    /* p30.ZYSU_STORE_I2*/ new_bus.BUS_SPR_I2.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.ZUZE_SPRITE_IDX2p.qn_new());
    /* p30.WYDA_STORE_I3*/ new_bus.BUS_SPR_I3.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XOBE_SPRITE_IDX3p.qn_new());
    /* p30.WUCO_STORE_I4*/ new_bus.BUS_SPR_I4.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.YDUF_SPRITE_IDX4p.qn_new());
    /* p30.WEZA_STORE_I5*/ new_bus.BUS_SPR_I5.tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XECU_SPRITE_IDX5p.qn_new());

    for (int feedback = 0; feedback < 2; feedback++) {
      /* p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(FETO_SCAN_DONEp_any(), XUPY_ABxxEFxx());
      /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp,                   ANOM_LINE_RSTn_new(), sprite_scanner.YFEL_SCAN0.qn_any());
      /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1.dff17_any(sprite_scanner.YFEL_SCAN0.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.WEWY_SCAN1.qn_any());
      /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2.dff17_any(sprite_scanner.WEWY_SCAN1.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.GOSO_SCAN2.qn_any());
      /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3.dff17_any(sprite_scanner.GOSO_SCAN2.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.ELYN_SCAN3.qn_any());
      /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4.dff17_any(sprite_scanner.ELYN_SCAN3.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.FAHA_SCAN4.qn_any());
      /* p28.FONY*/ sprite_scanner.FONY_SCAN5.dff17_any(sprite_scanner.FAHA_SCAN4.qn_any(), ANOM_LINE_RSTn_new(), sprite_scanner.FONY_SCAN5.qn_any());
    }
  }





  /* p24.LOBY*/ wire LOBY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());

  {
    /* p27.REPU*/ wire _REPU_VBLANKp = or2(PARU_VBLANKp(), PYRY_VID_RSTp());
    /* p27.SARY*/ win_reg.SARY_WY_MATCHp.dff17(TALU_xxCDEFxx(), XAPO_VID_RSTn(), _ROGE_WY_MATCHp_old);
    /* p27.REJO*/ win_reg.REJO_WY_MATCH_LATCHp.nor_latch(win_reg.SARY_WY_MATCHp.qp_new(), _REPU_VBLANKp);
    /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn_new(), XAPO_VID_RSTn());
    /* p27.NUNU*/ win_reg.NUNU_WIN_MATCHp.dff17(MEHE_AxCxExGx(), XAPO_VID_RSTn(), win_reg.PYCO_WIN_MATCHp.qp_old());
    /* p27.PYNU*/ win_reg.PYNU_WIN_MODE_Ap.nor_latch(win_reg.NUNU_WIN_MATCHp.qp_new(), XOFO_WIN_RSTp);
    /* p27.NOPA*/ win_reg.NOPA_WIN_MODE_Bp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), win_reg.PYNU_WIN_MODE_Ap.qp_new());

    /* p24.NAFY*/ wire _NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp_new(), LOBY_RENDERINGn);

    // vvvvv
    /* p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp.dff17(ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(),     tile_fetcher.PORY_FETCH_DONEp.qp_old());
    /* p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), _NAFY_WIN_MODE_TRIGn, tile_fetcher.NYKA_FETCH_DONEp.qp_old());
    /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17(ALET_xBxDxFxH(), _NAFY_WIN_MODE_TRIGn, LYRY_BFETCH_DONEp_old);
    /* p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.qp_new(), LOBY_RENDERINGn);

    /* p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn, tile_fetcher.PORY_FETCH_DONEp.qp_any(), PYRY_VID_RSTp());
    /* p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp, NUNY_WIN_MODE_TRIGp_new());
    /* p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn, tile_fetcher.PORY_FETCH_DONEp.qp_any(), PYRY_VID_RSTp());
    /* p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp, NUNY_WIN_MODE_TRIGp_new());
    // ^^^^^

  }

  /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(FEPO_STORE_MATCHp(old_match), WODU_HBLANKp_old, MYVO_AxCxExGx()); // FIXME old/new - but does it really matter here?
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn_new(), tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);

  // vvvvvvvvvv
  {
    {
      //tock_sprite_fetcher(TEKY_SFETCH_REQp_old);
      /* p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp.dff17(LAPE_AxCxExGx(), SIG_VCC, sprite_fetcher.SOBU_SFETCH_REQp.qp_old());
      /* p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp.dff17(TAVA_xBxDxFxH(), SIG_VCC, TEKY_SFETCH_REQp_old);

      /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1.dff17(LAPE_AxCxExGx(), SIG_VCC,                  sprite_fetcher.TOXE_SFETCH_S0p.qp_old());
      /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5.dff17(LAPE_AxCxExGx(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_old());
      /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TOBU_SFETCH_S1p_D2.qp_old());
      /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TULY_SFETCH_S1p.qp_old());

      /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
      /*#p27.SECA*/ wire _SECA_SFETCH_RSTn = nor3(_RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), ATEJ_LINE_RSTp_new());

      // Feedback loop
      for (int feedback = 0; feedback < 2; feedback++) {
        /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(sprite_fetcher.TESE_SFETCH_S2p.qp_any(), sprite_fetcher.TOXE_SFETCH_S0p.qp_any());
        /*#p29.TOMA*/ wire _TOMA_SFETCH_xBxDxFxH_= nand2(LAPE_AxCxExGx(), _TAME_SFETCH_CLK_GATE);
        /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.dff17_any(_TOMA_SFETCH_xBxDxFxH_,                  _SECA_SFETCH_RSTn, sprite_fetcher.TOXE_SFETCH_S0p.qn_any());
        /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p.dff17_any(sprite_fetcher.TOXE_SFETCH_S0p.qn_any(), _SECA_SFETCH_RSTn, sprite_fetcher.TULY_SFETCH_S1p.qn_any());
        /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p.dff17_any(sprite_fetcher.TULY_SFETCH_S1p.qn_any(), _SECA_SFETCH_RSTn, sprite_fetcher.TESE_SFETCH_S2p.qn_any());
      }

      /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor2(WUTY_SFETCH_DONE_TRIGp(), TAVE_PRELOAD_DONE_TRIGp_new()); // def nor
      /* p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RSTn, _VEKU_SFETCH_RUNNING_RSTn);
    }

    oam_latch_to_temp_a();


    {
      SpriteDeltaY delta = sub_sprite_y();
      wire _GESE_SCAN_MATCH_Yp = GESE_SCAN_MATCH_Yp(delta, reg_lcdc.XYMO_LCDC_SPSIZEn);

      /* p29.CARE*/ wire _CARE_COUNT_CLKn = and3(XOCE_xBCxxFGx(), CEHA_SCANNINGp(), _GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
      /* p29.DYTY*/ wire _DYTY_COUNT_CLKp = not1(_CARE_COUNT_CLKn);
      //update_count(_DYTY_COUNT_CLKp);

      // Sprite store counter. The sprite count clock stops ticking once we have 10 sprites.
      // Sprite Y matcher. This is using an adder as a subtracter by inverting the first input.

      /*#p28.AZYB*/ wire _AZYB_LINE_TRIGn = not1(ATEJ_LINE_RSTp_new());

      for (int feedback = 0; feedback < 2; feedback++) {
        /*#p29.BAKY*/ wire _BAKY_SPRITES_FULL_new = and2(sprite_store.CUXY_SPRITE_COUNT1.qp_any(), sprite_store.DYBE_SPRITE_COUNT3.qp_any());

        /* p29.DEZY*/ sprite_store.DEZY_COUNT_CLKp.dff17_any(ZEME_AxCxExGx(), XAPO_VID_RSTn(), _DYTY_COUNT_CLKp);
        /*#p29.CAKE*/ wire _CAKE_COUNT_CLKp_new = or2(_BAKY_SPRITES_FULL_new, sprite_store.DEZY_COUNT_CLKp.qp_any());

        /* p29.BESE*/ sprite_store.BESE_SPRITE_COUNT0.dff17_any(_CAKE_COUNT_CLKp_new,                     _AZYB_LINE_TRIGn, sprite_store.BESE_SPRITE_COUNT0.qn_any());
        /* p29.CUXY*/ sprite_store.CUXY_SPRITE_COUNT1.dff17_any(sprite_store.BESE_SPRITE_COUNT0.qn_any(), _AZYB_LINE_TRIGn, sprite_store.CUXY_SPRITE_COUNT1.qn_any());
        /* p29.BEGO*/ sprite_store.BEGO_SPRITE_COUNT2.dff17_any(sprite_store.CUXY_SPRITE_COUNT1.qn_any(), _AZYB_LINE_TRIGn, sprite_store.BEGO_SPRITE_COUNT2.qn_any());
        /* p29.DYBE*/ sprite_store.DYBE_SPRITE_COUNT3.dff17_any(sprite_store.BEGO_SPRITE_COUNT2.qn_any(), _AZYB_LINE_TRIGn, sprite_store.DYBE_SPRITE_COUNT3.qn_any());
      }

      SpriteStoreFlag store_flag = get_store_flags(_DYTY_COUNT_CLKp);
      store_sprite_x(store_flag, old_first_match);
      store_sprite_index(store_flag); // this needs old oam temp b, which is tocked in oam_latch_to_temp_b()
      store_sprite_line (store_flag);
    }

    oam_latch_to_temp_b();
  }
  //^^^^^

  {
    /*#p27.NYZE*/ fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17(MOXE_AxCxExGx(), XYMU_RENDERINGn.qn_new(), fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_old());
    /*#p27.PUXA*/ fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17(ROXO_CLKPIPE_odd, XYMU_RENDERINGn.qn_new(), _POHU_SCX_FINE_MATCHp_old);
    /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_new(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_new());
    /*#p27.PAHA*/ wire _PAHA_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());

    /*#p27.ROXY*/ fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(_PAHA_RENDERINGn, _POVA_FINE_MATCH_TRIGp);
  }

  /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());
  tock_pix_counter(SACU_CLKPIPE_evn);

  /*#p29.AZEM*/ wire _AZEM_RENDERINGp = and2(XYMU_RENDERINGn.qn_new(), BYJO_SCANNINGn());
  /*#p29.AROR*/ wire _AROR_MATCH_ENp = and2(_AZEM_RENDERINGp, reg_lcdc.XYLO_LCDC_SPENn.qn_new());
  SpriteMatchFlag sprite_match = get_match_flags_new(_AROR_MATCH_ENp);
  {
    SpriteFirstMatch first_match = get_first_match(sprite_match);
    get_sprite(first_match);
    ly_to_sprite_line(FEPO_STORE_MATCHp(sprite_match));
  }
  /*#p21.WODU*/ wire WODU_HBLANKp = and2(XENA_STORE_MATCHn(sprite_match), XANO_PX167p_new()); // WODU goes high on odd, cleared on H

  {
    /* p27.RENE*/ win_reg.RENE_WIN_FETCHn_B.dff17(ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), RYFA_WIN_FETCHn_A_old.qp_old());

    /* p27.ROCO*/ wire _ROCO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);
    /* p27.PYCO*/ win_reg.PYCO_WIN_MATCHp.dff17(_ROCO_CLKPIPE_odd, XAPO_VID_RSTn(), _NUKO_WX_MATCHp_old);

    /* p27.SOVY*/ win_reg.SOVY_WIN_HITp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), RYDY_WIN_HITp_old);

    /* p27.PANY*/ wire _PANY_WIN_FETCHn_old = nor2(_NUKO_WX_MATCHp_old, ROZE_FINE_COUNT_7n_old());
    /* p27.RYFA*/ win_reg.RYFA_WIN_FETCHn_A.dff17(SEGU_CLKPIPE_evn, XYMU_RENDERINGn.qn_new(), _PANY_WIN_FETCHn_old);
  }

  /* p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp = or3(SEKO_WIN_FETCH_TRIGp_new(), SUZU_WIN_FIRST_TILEne_new(), TAVE_PRELOAD_DONE_TRIGp_new()); // Schematic wrong, this is OR
  /* p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(AVAP_SCAN_DONE_TRIGp(), MOSU_WIN_MODE_TRIGp_new(), TEVO_WIN_FETCH_TRIGp);

  {
    //tock_tile_fetcher(NYXU_BFETCH_RSTn, MOCE_BFETCH_DONEn_old);
    for (int feedback = 0; feedback < 2; feedback++) {
      /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn = nand3(tile_fetcher._LAXU_BFETCH_S0p.qp_any(), tile_fetcher._NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn);
      /* p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(ALET_xBxDxFxH(), _MOCE_BFETCH_DONEn);

      /* p27.LAXU*/ tile_fetcher._LAXU_BFETCH_S0p.dff17_any(_LEBO_AxCxExGx,                         NYXU_BFETCH_RSTn, tile_fetcher._LAXU_BFETCH_S0p.qn_any());
      /* p27.MESU*/ tile_fetcher._MESU_BFETCH_S1p.dff17_any(tile_fetcher._LAXU_BFETCH_S0p.qn_any(), NYXU_BFETCH_RSTn, tile_fetcher._MESU_BFETCH_S1p.qn_any());
      /* p27.NYVA*/ tile_fetcher._NYVA_BFETCH_S2p.dff17_any(tile_fetcher._MESU_BFETCH_S1p.qn_any(), NYXU_BFETCH_RSTn, tile_fetcher._NYVA_BFETCH_S2p.qn_any());
    }

    /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp_old = not1(MOCE_BFETCH_DONEn_old);
    /* p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, _LYRY_BFETCH_DONEp_old);
    /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn = and2(tile_fetcher.LOVY_FETCH_DONEp.qn_new(), XYMU_RENDERINGn.qn_new());
    /* p27.LONY*/ tile_fetcher.LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, _LURY_BG_FETCH_DONEn);
    /* p27.LYZU*/ tile_fetcher._LYZU_BFETCH_S0p_D1.dff17(ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), tile_fetcher._LAXU_BFETCH_S0p.qp_new());

  }

  {
    //tock_fine_scroll(TYFA_CLKPIPE_odd, TEVO_WIN_FETCH_TRIGp);
    // Fine match counter. Registers are only read as old, so this can go down as far in the list as needed.

    /*#p24.SEGU*/ wire _SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire _ROXO_CLKPIPE_odd = not1(_SEGU_CLKPIPE_evn);

    /*#p27.PAHA*/ wire _PAHA_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
    /*#p27.PASO*/ wire _PASO_FINE_RST = nor2(_PAHA_RENDERINGn, TEVO_WIN_FETCH_TRIGp);

    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n = nand3(fine_scroll.RUBU_FINE_CNT2.qp_any(), fine_scroll.ROGA_FINE_CNT1.qp_any(), fine_scroll.RYKU_FINE_CNT0.qp_any());
      /*#p27.PECU*/ wire _PECU_FINE_CLK = nand2(_ROXO_CLKPIPE_odd, _ROZE_FINE_COUNT_7n);
      /*#p27.RYKU*/ fine_scroll.RYKU_FINE_CNT0.dff17_any(_PECU_FINE_CLK,                      _PASO_FINE_RST, fine_scroll.RYKU_FINE_CNT0.qn_any());
      /*#p27.ROGA*/ fine_scroll.ROGA_FINE_CNT1.dff17_any(fine_scroll.RYKU_FINE_CNT0.qn_any(), _PASO_FINE_RST, fine_scroll.ROGA_FINE_CNT1.qn_any());
      /*#p27.RUBU*/ fine_scroll.RUBU_FINE_CNT2.dff17_any(fine_scroll.ROGA_FINE_CNT1.qn_any(), _PASO_FINE_RST, fine_scroll.RUBU_FINE_CNT2.qn_any());
    }
  }

  store_tile_temp_a();
  store_tile_temp_b();
  store_sprite_pix_a(sprite_pix_old);
  store_sprite_pix_b(sprite_pix_old);

  //----------------------------------------
  // LCD

  {
    lcd.PIN_50_LCD_DATA1.reset_for_pass();
    lcd.PIN_51_LCD_DATA0.reset_for_pass();
    lcd.PIN_54_LCD_HSYNC.reset_for_pass();
    lcd.PIN_56_LCD_FLIPS.reset_for_pass();
    lcd.PIN_52_LCD_CNTRL.reset_for_pass();
    lcd.PIN_55_LCD_LATCH.reset_for_pass();
    lcd.PIN_53_LCD_CLOCK.reset_for_pass();
    lcd.PIN_57_LCD_VSYNC.reset_for_pass();

    reg_bgp_write();
    reg_obp0_write();
    reg_obp1_write();

    tock_bgw_pipe   (SACU_CLKPIPE_evn, NYXU_BFETCH_RSTn);
    tock_mask_pipe  (SACU_CLKPIPE_evn);
    tock_pal_pipe   (SACU_CLKPIPE_evn);
    tock_sprite_pipe(SACU_CLKPIPE_evn);
    tock_pix_output ();
    set_lcd_pin_data(pix_pipes.REMY_LD0n, pix_pipes.RAVO_LD1n);

    set_lcd_pin_ctrl();
    set_lcd_pin_flip(div.TULU_DIV07p, reg_lcdc.XONA_LCDC_LCDENn);
    set_lcd_pin_vsync();
    set_lcd_pin_hsync(TYFA_CLKPIPE_odd, XYDO_PX3p_old);
    set_lcd_pin_latch();

    set_lcd_pin_clock(SACU_CLKPIPE_evn);

    update_lcd_pipe();
    update_framebuffer();

    reg_bgp_read();
    reg_obp0_read();
    reg_obp1_read();
  }

  //----------------------------------------
  // Ext new_bus

  {
    ext_bus.PIN_80_CSn.reset_for_pass();
    ext_bus.PIN_79_RDn.reset_for_pass();
    ext_bus.PIN_78_WRn.reset_for_pass();

    ext_bus.PIN_01_A00.reset_for_pass();
    ext_bus.PIN_02_A01.reset_for_pass();
    ext_bus.PIN_03_A02.reset_for_pass();
    ext_bus.PIN_04_A03.reset_for_pass();
    ext_bus.PIN_05_A04.reset_for_pass();
    ext_bus.PIN_06_A05.reset_for_pass();
    ext_bus.PIN_07_A06.reset_for_pass();
    ext_bus.PIN_08_A07.reset_for_pass();
    ext_bus.PIN_09_A08.reset_for_pass();
    ext_bus.PIN_10_A09.reset_for_pass();
    ext_bus.PIN_11_A10.reset_for_pass();
    ext_bus.PIN_12_A11.reset_for_pass();
    ext_bus.PIN_13_A12.reset_for_pass();
    ext_bus.PIN_14_A13.reset_for_pass();
    ext_bus.PIN_15_A14.reset_for_pass();
    ext_bus.PIN_16_A15.reset_for_pass();

    // FIXME this is slightly weird
    ext_bus.PIN_17_D00.state = 0b00100000;
    ext_bus.PIN_18_D01.state = 0b00100000;
    ext_bus.PIN_19_D02.state = 0b00100000;
    ext_bus.PIN_20_D03.state = 0b00100000;
    ext_bus.PIN_21_D04.state = 0b00100000;
    ext_bus.PIN_22_D05.state = 0b00100000;
    ext_bus.PIN_23_D06.state = 0b00100000;
    ext_bus.PIN_24_D07.state = 0b00100000;

    set_ext_control_pins();
    copy_cpu_addr_to_addr_latch();
    copy_addr_latch_to_pins();

    // A15 is "special"
    /* p08.SOBY*/ wire _SOBY_A15n = nor2 (new_bus.BUS_CPU_A15p.qp_new(), TUTU_READ_BOOTROMp());
    /* p08.SEPY*/ wire _SEPY_A15p = nand2(cpu_bus.ABUZ_EXT_RAM_CS_CLK, _SOBY_A15n);
    /* p08.TAZY*/ wire _TAZY_A15p = mux2p (LUMA_DMA_CARTp(), dma.MARU_DMA_A15n.qn_new(), _SEPY_A15p);
    /* p08.SUZE*/ wire _SUZE_A15n = nand2(_TAZY_A15p, RYCA_MODE_DBG2n());
    /* p08.RULO*/ wire _RULO_A15n = nor2 (_TAZY_A15p, UNOR_MODE_DBG2p());
    /*PIN_16*/ ext_bus.PIN_16_A15.pin_out_hilo(_SUZE_A15n, _RULO_A15n);

    copy_cpu_data_to_pins();
    read_ext_to_pins();

    write_pins_to_ext();
    copy_pins_to_data_latch();
    copy_data_latch_to_cpu_bus();
  }

  //----------------------------------------
  // VRAM bus

  {
    vram_bus.reset_buses();

    cpu_addr_to_vram_addr();
    dma_addr_to_vram_addr();

    auto scroll_x = add_scx();
    auto scroll_y = add_scy();
    scroll_to_vram_addr(scroll_x, scroll_y);

    tock_win_map_x(TEVO_WIN_FETCH_TRIGp);
    tock_win_map_y();
    win_to_vram_addr();

    tile_to_vram_addr(scroll_y);
    sprite_to_vram_addr();
    vram_addr_to_pins();

    cpu_data_to_vram_bus_data();
    vram_bus_data_to_pins();

    set_vram_pin_cs();
    set_vram_pin_wr();
    set_vram_pin_oe();

    read_vram();
    write_vram();

    vram_pins_to_data_bus();
    vram_data_bus_to_cpu_bus();
  }

  //----------------------------------------
  // OAM bus

  {
    dma_to_oam_addr_bus();
    sprite_index_to_oam_addr_bus();
    scan_index_to_oam_addr_bus();
    cpu_to_oam_addr_bus();
    ext_to_oam_data_bus();
    vram_to_oam_data_bus();
    cpu_to_oam_data_bus();
    set_oam_pin_clk();
    set_oam_pin_wr ();
    set_oam_pin_oe ();
    tock_oam_bus();
    latch_oam_data_bus();
    oam_latch_to_cpu();
  }

  //----------------------------------------
  // Misc tocks
  {
    tock_serial();

    tock_timer();
    tock_joypad();
    tock_interrupts(WODU_HBLANKp);
  }

  //----------------------------------------
  // Async reads

  {
    read_ie();
    read_intf();
    reg_stat_read();
    reg_scx_read();
    reg_scy_read();
    reg_dma_read();
    reg_div_read();

    //read_bootrom();
    {
      // BOOT -> CBD
      // this is kind of a hack
      uint16_t cpu_addr = (uint16_t)BitBase::pack_new(16, &new_bus.BUS_CPU_A00p);
      wire bootrom_data = boot_buf[cpu_addr & 0xFF];

      /* p07.TERA*/ wire _TERA_BOOT_BITp  = not1(cpu_bus.TEPU_BOOT_BITn_h.qp_new());
      /* p07.TUTU*/ wire _TUTU_READ_BOOTROMp = and2(_TERA_BOOT_BITp, new_bus.TULO_ADDR_BOOTROMp());
      /*SIG_CPU_BOOTp*/ cpu_bus.SIG_CPU_BOOTp.sig_out(_TUTU_READ_BOOTROMp);

      /* p07.ZORO*/ wire _ZORO_0000xxxx_XX = nor4(new_bus.BUS_CPU_A15p.qp_new(), new_bus.BUS_CPU_A14p.qp_new(), new_bus.BUS_CPU_A13p.qp_new(), new_bus.BUS_CPU_A12p.qp_new());
      /* p07.ZADU*/ wire _ZADU_xxxx0000_XX = nor4(new_bus.BUS_CPU_A11p.qp_new(), new_bus.BUS_CPU_A10p.qp_new(), new_bus.BUS_CPU_A09p.qp_new(), new_bus.BUS_CPU_A08p.qp_new());
      /* p07.ZUFA*/ wire _ZUFA_0000_00FF  = and2(_ZORO_0000xxxx_XX, _ZADU_xxxx0000_XX);
      /* p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1(UMUT_MODE_DBG1p());
      /* p07.YULA*/ wire _YULA_BOOT_RDp   = and3(cpu_bus.TEDO_CPU_RDp, _YAZA_MODE_DBG1n, _TUTU_READ_BOOTROMp); // def AND
      /* p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2(_YULA_BOOT_RDp, _ZUFA_0000_00FF);
      /* p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1(_ZADO_BOOT_CSn);

      /*SIG_BOOT_CSp*/ cpu_bus.SIG_BOOT_CSp.sig_out(_ZERY_BOOT_CSp);

      new_bus.BUS_CPU_D00p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 0));
      new_bus.BUS_CPU_D01p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 1));
      new_bus.BUS_CPU_D02p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 2));
      new_bus.BUS_CPU_D03p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 3));
      new_bus.BUS_CPU_D04p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 4));
      new_bus.BUS_CPU_D05p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 5));
      new_bus.BUS_CPU_D06p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 6));
      new_bus.BUS_CPU_D07p.tri6_pn(_ZERY_BOOT_CSp, bit(~bootrom_data, 7));
    }


    {
      //read_boot_bit();
      /* p07.TEXE*/ wire _TEXE_FF50_RDp =  and4(cpu_bus.TEDO_CPU_RDp, new_bus.SYKE_ADDR_HIp(), new_bus.TYRO_XX_0x0x0000p(), new_bus.TUFA_XX_x1x1xxxxp());
      /* p07.SYPU_BOOT_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_TEXE_FF50_RDp, cpu_bus.TEPU_BOOT_BITn_h.qp_new());
    }
    reg_tima_read();
    reg_tma_read();
    reg_tac_read();
    reg_lcdc_read();
    reg_lyc_read();
    reg_ly_read();
    reg_wy_read();
    reg_wx_read();
    read_zram();
  }

  const_cast<GateBoyBuses&>(old_bus) = new_bus;
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::update_framebuffer()
{
  int lcd_x = pix_count.get_new() - 8;
  int lcd_y = reg_ly.get_new();

  if (lcd_y >= 0 && lcd_y < 144 && lcd_x >= 0 && lcd_x < 160) {
    uint8_t p0 = bit(lcd.PIN_51_LCD_DATA0.qp_new());
    uint8_t p1 = bit(lcd.PIN_50_LCD_DATA1.qp_new());

    //probe(4, "pix lo", p0);
    //probe(5, "pix hi", p1);

    uint8_t new_pix = p0 + p1 * 2;

    framebuffer[lcd_x + lcd_y * 160] = new_pix;
  }

  old_lcd_x = lcd_x;
  old_lcd_y = lcd_y;

#if 0
  if (bit(~lcd.old_lcd_clock.qp_old()) && lcd.PIN_53_LCD_CLOCK.qp_new()) {
    //printf("gb_screen_x++\n");
    gb_screen_x++;
  }
  if (lcd.PIN_54_LCD_HSYNC.qp_new() || lcd.PIN_55_LCD_LATCH.qp_new()) {
    //printf("gb_screen_x = 0\n");
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
      //printf("gb_screen_y = 0\n");
      gb_screen_y = 0;
    }
    else {
      //printf("gb_screen_y++\n");
      gb_screen_y++;
    }
  }

  lcd.old_lcd_clock.set_new(lcd.PIN_53_LCD_CLOCK.qp_new());
  lcd.old_lcd_latch.set_new(lcd.PIN_55_LCD_LATCH.qp_new());
#endif
}

//------------------------------------------------------------------------------------------------------------------------
