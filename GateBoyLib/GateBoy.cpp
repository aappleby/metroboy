#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"

/*SIG_GND*/ SigIn GateBoy::SIG_VCC = 1;
/*SIG_VCC*/ SigIn GateBoy::SIG_GND = 0;

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
  phase_origin = 46880720;
;

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

  reg_lcdc.reset_to_cart();
  lcd.reset_to_cart();

  reg_lx.reset_to_cart();
  reg_ly.reset_to_cart();
  reg_lyc.reset_to_cart();

  WODU_HBLANKp.state = 0b00011001;

  sprite_scanner.FETO_SCAN_DONEp.state = 0b00011001;
  ATEJ_LINE_RSTp.state = 0b00011000;

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
  phase_total = 46880720;
  phase_origin = 46880720;
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
  const int o_cpu_bus        = offsetof(GateBoy, cpu_signals);
  const int o_ext_bus        = offsetof(GateBoy, ext_pins);
  const int o_vram_bus       = offsetof(GateBoy, vram_bus);
  const int o_oam_bus        = offsetof(GateBoy, oam_bus);
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

  const int o_SPR_TRI_I      = offsetof(GateBoy, sprite_bus.BUS_SPR_I0);
  const int o_SPR_TRI_L      = offsetof(GateBoy, sprite_bus.BUS_SPR_L0);

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

  const int o_win_coords     = offsetof(GateBoy, win_coords);
  const int o_win_reg        = offsetof(GateBoy, win_reg    );
  const int o_fine_scroll    = offsetof(GateBoy, fine_scroll);

  const int o_pix_count      = offsetof(GateBoy, pix_count);
  const int o_pix_pipes      = offsetof(GateBoy, pix_pipes);
  const int o_lcd            = offsetof(GateBoy, lcd      );

} gb_offsets;

//------------------------------------------------------------------------------------------------------------------------

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

  new_bus.reset_for_pass();
  new_bus.set_data(int(phase_total), bus_req_new);
  new_bus.set_addr(int(phase_total), bus_req_new);

  set_pins();

  //-----------------------------------------------------------------------------
  // Sys clock signals

  tock_clocks();

  {
    /* p07.UJYV*/ wire _UJYV_CPU_RDn = not1(cpu_signals.SIG_CPU_RDp.qp_new());
    /* p07.TEDO*/ cpu_signals.TEDO_CPU_RDp = not1(_UJYV_CPU_RDn);

    /*#p01.AFAS*/ wire _AFAS_xxxxEFGx = nor2(ADAR_ABCxxxxH(), ATYP_ABCDxxxx());
    /* p01.AREV*/ wire _AREV_CPU_WRn = nand2(cpu_signals.SIG_CPU_WRp.qp_new(), _AFAS_xxxxEFGx);
    /* p01.APOV*/ cpu_signals.APOV_CPU_WRp = not1(_AREV_CPU_WRn);

    /* p07.UBAL*/ wire _UBAL_CPU_WRn = not1(cpu_signals.APOV_CPU_WRp.qp_new());
    /* p07.TAPU*/ cpu_signals.TAPU_CPU_WRp = not1(_UBAL_CPU_WRn); // xxxxEFGx

    /*#p01.AGUT*/ wire _AGUT_xxCDEFGH = or_and3(AROV_xxCDEFxx(), AJAX_xxxxEFGH(), cpu_signals.SIG_CPU_EXT_BUSp.qp_new());
    /*#p01.AWOD*/ wire _AWOD_ABxxxxxx = nor2(UNOR_MODE_DBG2p(), _AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ cpu_signals.ABUZ_EXT_RAM_CS_CLK = not1(_AWOD_ABxxxxxx);
  }

  tock_div();
  tock_reset(bit(sys_fastboot) ? div.TERO_DIV03p : div.UPOF_DIV15p);

  // LCDC. Has to be near the top as it controls the video reset signal
  tock_lcdc();

  rst.set_signals(reg_lcdc.XONA_LCDC_LCDENn);
  tock_vid_clocks();

  tock_lyc();
  tock_joypad();
  tock_serial();
  tock_timer();

  tock_bootrom();

  //----------------------------------------

  tock_lcd();

  /*#p21.PARU*/ wire PARU_VBLANKp = not1(lcd.POPU_VBLANKp.qn_new());
  /* p27.REPU*/ wire REPU_VBLANKp = or2(PARU_VBLANKp, PYRY_VID_RSTp());

  /* p21.TADY*/ wire TADY_LINE_RSTn = nor2(ATEJ_LINE_RSTp.qp_new(), TOFU_VID_RSTp());
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(ATEJ_LINE_RSTp.qp_new(), ATAR_VID_RSTp());
  /*#p29.BALU*/ wire BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn = not1(BALU_LINE_RSTp);
  /* p27.XAHY*/ wire XAHY_LINE_RSTn = not1(ATEJ_LINE_RSTp.qp_new());

  //----------------------------------------

  tock_dma();

  //----------------------------------------
  // Sprite scanner

  {
    /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(ALET_xBxDxFxH(), BAGY_LINE_RSTn, sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
    /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(XUPY_ABxxEFxx(), BAGY_LINE_RSTn, sprite_scanner.FETO_SCAN_DONEp.qp_old());
    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp, sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ sprite_scanner.AVAP_SCAN_DONE_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);

    /*#p28.ASEN*/ wire ASEN_SCAN_DONE_TRIGp = or2(ATAR_VID_RSTp(), sprite_scanner.AVAP_SCAN_DONE_TRIGp.qp_new());
    /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGn.dff17(XUPY_ABxxEFxx(), ABEZ_VID_RSTn(), sprite_scanner.BESU_SCANNINGp.qp_old());
    /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd.CATU_START_SCANNING.qp_new(), ASEN_SCAN_DONE_TRIGp);

    /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(dma.MATU_DMA_RUNNINGp.qp_new());
    /*#p28.ACYL*/ sprite_scanner.ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, sprite_scanner.BESU_SCANNINGp.qp_new());

    // Sprite store grabs the sprite index off the _old_ oam address bus
    /* p28.YFOT*/ wire _YFOT_OAM_A2p_old = not1(oam_bus.BUS_OAM_A02n.qp_old());
    /* p28.YFOC*/ wire _YFOC_OAM_A3p_old = not1(oam_bus.BUS_OAM_A03n.qp_old());
    /* p28.YVOM*/ wire _YVOM_OAM_A4p_old = not1(oam_bus.BUS_OAM_A04n.qp_old());
    /* p28.YMEV*/ wire _YMEV_OAM_A5p_old = not1(oam_bus.BUS_OAM_A05n.qp_old());
    /* p28.XEMU*/ wire _XEMU_OAM_A6p_old = not1(oam_bus.BUS_OAM_A06n.qp_old());
    /* p28.YZET*/ wire _YZET_OAM_A7p_old = not1(oam_bus.BUS_OAM_A07n.qp_old());
    /* p30.XADU*/ sprite_scanner.XADU_SPRITE_IDX0p.dff13(WUDA_xxCDxxGH(), SIG_VCC.qp_new(), _YFOT_OAM_A2p_old);
    /* p30.XEDY*/ sprite_scanner.XEDY_SPRITE_IDX1p.dff13(WUDA_xxCDxxGH(), SIG_VCC.qp_new(), _YFOC_OAM_A3p_old);
    /* p30.ZUZE*/ sprite_scanner.ZUZE_SPRITE_IDX2p.dff13(WUDA_xxCDxxGH(), SIG_VCC.qp_new(), _YVOM_OAM_A4p_old);
    /* p30.XOBE*/ sprite_scanner.XOBE_SPRITE_IDX3p.dff13(WUDA_xxCDxxGH(), SIG_VCC.qp_new(), _YMEV_OAM_A5p_old);
    /* p30.YDUF*/ sprite_scanner.YDUF_SPRITE_IDX4p.dff13(WUDA_xxCDxxGH(), SIG_VCC.qp_new(), _XEMU_OAM_A6p_old);
    /* p30.XECU*/ sprite_scanner.XECU_SPRITE_IDX5p.dff13(WUDA_xxCDxxGH(), SIG_VCC.qp_new(), _YZET_OAM_A7p_old);

    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p28.FETO*/ wire _FETO_SCAN_DONEp = and4(sprite_scanner.YFEL_SCAN0.qp_mid(), sprite_scanner.WEWY_SCAN1.qp_mid(), sprite_scanner.GOSO_SCAN2.qp_mid(), sprite_scanner.FONY_SCAN5.qp_mid()); // 32 + 4 + 2 + 1 = 39

      /* p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(_FETO_SCAN_DONEp, XUPY_ABxxEFxx());
      /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0.dff17_any(GAVA_SCAN_CLOCKp,                   ANOM_LINE_RSTn, sprite_scanner.YFEL_SCAN0.qn_any());
      /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1.dff17_any(sprite_scanner.YFEL_SCAN0.qn_any(), ANOM_LINE_RSTn, sprite_scanner.WEWY_SCAN1.qn_any());
      /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2.dff17_any(sprite_scanner.WEWY_SCAN1.qn_any(), ANOM_LINE_RSTn, sprite_scanner.GOSO_SCAN2.qn_any());
      /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3.dff17_any(sprite_scanner.GOSO_SCAN2.qn_any(), ANOM_LINE_RSTn, sprite_scanner.ELYN_SCAN3.qn_any());
      /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4.dff17_any(sprite_scanner.ELYN_SCAN3.qn_any(), ANOM_LINE_RSTn, sprite_scanner.FAHA_SCAN4.qn_any());
      /* p28.FONY*/ sprite_scanner.FONY_SCAN5.dff17_any(sprite_scanner.FAHA_SCAN4.qn_any(), ANOM_LINE_RSTn, sprite_scanner.FONY_SCAN5.qn_any());
    }

    /*#p28.FETO*/ sprite_scanner.FETO_SCAN_DONEp = and4(sprite_scanner.YFEL_SCAN0.qp_new(), sprite_scanner.WEWY_SCAN1.qp_new(), sprite_scanner.GOSO_SCAN2.qp_new(), sprite_scanner.FONY_SCAN5.qp_new()); // 32 + 4 + 2 + 1 = 39
  }


  //----------------------------------------
  // Global rendering flag 'XYMU'

  /*#p21.VOGA*/ VOGA_HBLANKp.dff17(ALET_xBxDxFxH(), TADY_LINE_RSTn, WODU_HBLANKp.qp_old());
  /*#p21.WEGO*/ wire WEGO_HBLANKp = or2(TOFU_VID_RSTp(), VOGA_HBLANKp.qp_new());
  /*#p21.XYMU*/ XYMU_RENDERINGn.nor_latch(WEGO_HBLANKp, sprite_scanner.AVAP_SCAN_DONE_TRIGp.qp_new());
  /* p24.LOBY*/ wire LOBY_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
  /*#p27.PAHA*/ wire PAHA_RENDERINGn = not1(XYMU_RENDERINGn.qn_new());
  /* p29.TEPA*/ wire TEPA_RENDERINGp = not1(XYMU_RENDERINGn.qn_new());

  //----------------------------------------
  // Sprite fetch sequencer

  {
    /*#p27.SYLO*/ wire SYLO_WIN_HITn_old = not1(win_reg.RYDY_WIN_HITp.qp_old());
    /*#p24.TOMU*/ wire TOMU_WIN_HITp_old = not1(SYLO_WIN_HITn_old);
    /* p27.TUKU*/ wire TUKU_WIN_HITn_old = not1(TOMU_WIN_HITp_old);
    /* p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_old = not1(sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp_old());
    /* p27.TEKY*/ wire TEKY_SFETCH_REQp_old = and4(sprite_match_flags.FEPO_STORE_MATCHp.qp_old(), TUKU_WIN_HITn_old, tile_fetcher.LYRY_BFETCH_DONEp.qp_old(), SOWO_SFETCH_RUNNINGn_old);
    /* p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp.dff17(LAPE_AxCxExGx(), SIG_VCC.qp_new(), sprite_fetcher.SOBU_SFETCH_REQp.qp_old());
    /* p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp.dff17(TAVA_xBxDxFxH(), SIG_VCC.qp_new(), TEKY_SFETCH_REQp_old);

    /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1.dff17(LAPE_AxCxExGx(), SIG_VCC.qp_new(),         sprite_fetcher.TOXE_SFETCH_S0p.qp_old());
    /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5.dff17(LAPE_AxCxExGx(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_old());
    /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TOBU_SFETCH_S1p_D2.qp_old());
    /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TULY_SFETCH_S1p.qp_old());

    /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
    /*#p27.SECA*/ wire _SECA_SFETCH_STARTn = nor3(_RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), ATEJ_LINE_RSTp.qp_new());

    // Feedback loop
    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(sprite_fetcher.TESE_SFETCH_S2p.qp_any(), sprite_fetcher.TOXE_SFETCH_S0p.qp_any());
      /*#p29.TOMA*/ wire _TOMA_SFETCH_xBxDxFxH_= nand2(LAPE_AxCxExGx(), _TAME_SFETCH_CLK_GATE);
      /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.dff17_any(_TOMA_SFETCH_xBxDxFxH_,                  _SECA_SFETCH_STARTn, sprite_fetcher.TOXE_SFETCH_S0p.qn_any());
      /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p.dff17_any(sprite_fetcher.TOXE_SFETCH_S0p.qn_any(), _SECA_SFETCH_STARTn, sprite_fetcher.TULY_SFETCH_S1p.qn_any());
      /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p.dff17_any(sprite_fetcher.TULY_SFETCH_S1p.qn_any(), _SECA_SFETCH_STARTn, sprite_fetcher.TESE_SFETCH_S2p.qn_any());
    }

    /* p29.SAKY*/ wire _SAKY_SFETCHn = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /* p29.TYSO*/ wire _TYSO_SFETCHINGn = or2(_SAKY_SFETCHn, TEPA_RENDERINGp); // def or
    /* p29.TEXY*/ sprite_fetcher.TEXY_SFETCHINGp = not1(_TYSO_SFETCHINGn);

    /* p29.TYNO*/ wire _TYNO = nand3(sprite_fetcher.TOXE_SFETCH_S0p.qp_new(), sprite_fetcher.SEBA_SFETCH_S1p_D5.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn = or2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qn_new(), _TYNO);
    /* p29.WUTY*/ sprite_fetcher.WUTY_SFETCH_DONE_TRIGp = not1(_VUSA_SPRITE_DONEn);
  }

  /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
  /*#p27.SECA*/ wire _SECA_SFETCH_STARTn = nor3(_RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), ATEJ_LINE_RSTp.qp_new());

  //----------------------------------------
  // Window state has some interaction with the tile fetcher here.

  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg_lcdc.WYMO_LCDC_WINENn.qn_new(), XAHY_LINE_RSTn, XAPO_VID_RSTn());
  /* p27.NUNU*/ win_reg.NUNU_WIN_MATCHp. dff17(MEHE_AxCxExGx(), XAPO_VID_RSTn(), win_reg.PYCO_WIN_MATCHp.qp_old());
  /* p27.NOPA*/ win_reg.NOPA_WIN_MODE_Bp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), win_reg.PYNU_WIN_MODE_Ap.qp_old());
  /* p27.PYNU*/ win_reg.PYNU_WIN_MODE_Ap.nor_latch(win_reg.NUNU_WIN_MATCHp.qp_new(), XOFO_WIN_RSTp);

  /*#p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and2(win_reg.PYNU_WIN_MODE_Ap.qp_new(), win_reg.NOPA_WIN_MODE_Bp.qn_new());
  /* p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not1(NUNY_WIN_MODE_TRIGp);
  /* p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not1(NYFO_WIN_MODE_TRIGn);
  /* p24.NAFY*/ wire NAFY_WIN_MODE_TRIGn = nor2(MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);

  /* p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp.dff17(ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), tile_fetcher.PORY_FETCH_DONEp.qp_old());
  /* p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NAFY_WIN_MODE_TRIGn,      tile_fetcher.NYKA_FETCH_DONEp.qp_old());
  /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17(ALET_xBxDxFxH(), NAFY_WIN_MODE_TRIGn,      tile_fetcher.LYRY_BFETCH_DONEp.qp_old());
  /* p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.qp_new(), LOBY_RENDERINGn);

  /* p27.SOVY*/ win_reg.SOVY_WIN_HITp.dff17(ALET_xBxDxFxH(), XAPO_VID_RSTn(), win_reg.RYDY_WIN_HITp.qp_old());
  /* p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn.qp_mid(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /* p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp.qp_mid(), NUNY_WIN_MODE_TRIGp);
  /* p27.RYDY*/ win_reg.RYDY_WIN_HITp = nor3(win_reg.PUKU_WIN_HITn.qp_mid(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), PYRY_VID_RSTp());
  /* p27.PUKU*/ win_reg.PUKU_WIN_HITn = nor2(win_reg.RYDY_WIN_HITp.qp_mid(), NUNY_WIN_MODE_TRIGp);

  /*#p27.SYLO*/ wire SYLO_WIN_HITn = not1(win_reg.RYDY_WIN_HITp.qp_new());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp = not1(SYLO_WIN_HITn);
  /*#p24.SOCY*/ wire SOCY_WIN_HITn = not1(TOMU_WIN_HITp);

  /* p27.ROMO*/ wire ROMO_PRELOAD_DONEn      = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp_new());
  /* p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn = nand4(XYMU_RENDERINGn.qn_new(), ROMO_PRELOAD_DONEn, tile_fetcher.NYKA_FETCH_DONEp.qp_new(), tile_fetcher.PORY_FETCH_DONEp.qp_new());
  /* p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp = not1(SUVU_PRELOAD_DONE_TRIGn);

  /* p27.VEKU*/ wire _VEKU_SFETCH_ENDn = nor2(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp.qp_new(), TAVE_PRELOAD_DONE_TRIGp); // def nor
  /* p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_STARTn, _VEKU_SFETCH_ENDn);

  //----------------------------------------
  // OAM temp register B stores sprite X coordinate during scan, so we need a copy of it for later.

  auto oam_temp_b_old = oam_temp_b;

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  {
    /* p25.CUFE*/ wire _CUFE_OAM_CLKp = not_or_and3(new_bus.SARO_ADDR_OAMp(), dma.MATU_DMA_RUNNINGp.qp_new(), MOPA_xxxxEFGH()); // CUFE looks like BYHA minus an inverter
    /* p25.AVER*/ wire _AVER_AxxxExxx = nand2(sprite_scanner.ACYL_SCANNINGp.qp_new(), XYSO_xBCDxFGH());
    /* p29.TYTU*/ wire _TYTU_SFETCH_S0n = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
    /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new(), _TYTU_SFETCH_S0n);
    /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp = nor3(TEPA_RENDERINGp, sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.TESE_SFETCH_S2p.qp_new());
    /* p25.VAPE*/ wire _VAPE_OAM_CLKENn = and2(_TUVO_PPU_OAM_RDp, _TACU_SPR_SEQ_5_TRIG);
    /* p25.XUJY*/ wire _XUJY_OAM_CLKENp = not1(_VAPE_OAM_CLKENn);
    /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_AxxxExxx, _XUJY_OAM_CLKENp, _CUFE_OAM_CLKp);
    /* p25.COTA*/ wire _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);
    oam_latch_to_temp_a(_COTA_OAM_CLKn, oam_latch_a, oam_temp_a);
    oam_latch_to_temp_b(_COTA_OAM_CLKn, oam_latch_b, oam_temp_b);
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if a sprite overlaps the current LCD Y coordinate.

  SpriteDeltaY delta = sub_sprite_y(reg_ly, oam_temp_a);

  {
    /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH = or2(reg_lcdc.XYMO_LCDC_SPSIZEn.qn_new(), delta.GYKY_YDIFF3.sum);
    /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn = nand6(delta.GACE_SPRITE_DELTA4.qp_new(), delta.GUVU_SPRITE_DELTA5.qp_new(), delta.GYDA_SPRITE_DELTA6.qp_new(), delta.GEWY_SPRITE_DELTA7.qp_new(), delta.WUHU_YDIFF7.carry, _GOVU_SPSIZE_MATCH);
    /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp = not1(_WOTA_SCAN_MATCH_Yn);
    /* p29.CEHA*/ wire _CEHA_SCANNINGp = not1(sprite_scanner.CENO_SCANNINGn.qn_new());
    /* p29.CARE*/ wire _CARE_COUNT_CLKn = and3(XOCE_xBCxxFGx(), _CEHA_SCANNINGp, _GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /* p29.DYTY*/ wire _DYTY_COUNT_CLKp = not1(_CARE_COUNT_CLKn);
    /* p29.DEZY*/ sprite_counter.DEZY_COUNT_CLKp.dff17_any(ZEME_AxCxExGx(), XAPO_VID_RSTn(), _DYTY_COUNT_CLKp);

    // There's a feedback loop here, but we don't actually need to loop - BAKY holds the clock line high once the sprite store is full, so doing a second logic pass
    // doesn't actually change any of the dffs.
    /*#p29.BAKY*/ wire _BAKY_SPRITES_FULL_new = and2(sprite_counter.CUXY_SPRITE_COUNT1.qp_any(), sprite_counter.DYBE_SPRITE_COUNT3.qp_any());
    /*#p29.CAKE*/ wire _CAKE_COUNT_CLKp_new = or2(_BAKY_SPRITES_FULL_new, sprite_counter.DEZY_COUNT_CLKp.qp_any());
    /*#p28.AZYB*/ wire _AZYB_LINE_TRIGn = not1(ATEJ_LINE_RSTp.qp_new());
    /* p29.BESE*/ sprite_counter.BESE_SPRITE_COUNT0.dff17_any(_CAKE_COUNT_CLKp_new,                       _AZYB_LINE_TRIGn, sprite_counter.BESE_SPRITE_COUNT0.qn_any());
    /* p29.CUXY*/ sprite_counter.CUXY_SPRITE_COUNT1.dff17_any(sprite_counter.BESE_SPRITE_COUNT0.qn_any(), _AZYB_LINE_TRIGn, sprite_counter.CUXY_SPRITE_COUNT1.qn_any());
    /* p29.BEGO*/ sprite_counter.BEGO_SPRITE_COUNT2.dff17_any(sprite_counter.CUXY_SPRITE_COUNT1.qn_any(), _AZYB_LINE_TRIGn, sprite_counter.BEGO_SPRITE_COUNT2.qn_any());
    /* p29.DYBE*/ sprite_counter.DYBE_SPRITE_COUNT3.dff17_any(sprite_counter.BEGO_SPRITE_COUNT2.qn_any(), _AZYB_LINE_TRIGn, sprite_counter.DYBE_SPRITE_COUNT3.qn_any());

    /* p28.ABAK*/ wire _ABAK_LINE_RSTp = or2(ATEJ_LINE_RSTp.qp_new(), AMYG_VID_RSTp());
    /* p28.BYVA*/ wire _BYVA_LINE_RSTn = not1(_ABAK_LINE_RSTp);

    update_sprite_reset_flags(sprite_fetcher.WUTY_SFETCH_DONE_TRIGp, _BYVA_LINE_RSTn, sprite_match_flags, sprite_reset_flags);
    update_sprite_store_flags(sprite_counter, _DYTY_COUNT_CLKp, sprite_store_flags);
    store_sprite(sprite_store_flags, sprite_reset_flags, _BYVA_LINE_RSTn, sprite_bus, oam_temp_b_old, sprite_store);
  }

  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  auto XYDO_PX3p_old = pix_count.XYDO_PX3p;

  // Weird feedback loop here - fine scroll and pix count affect store match, which affects pipe clock, which affects fine scroll and pix count.
  // Implementing the feedback doesn't seem to change any test results, but... paranoia...

  //for (int feedback = 0; feedback < 2; feedback++) {
  {
    /* p27.SUHA*/ wire SUHA_SCX_FINE_MATCHp_old = xnor2(reg_scx.DATY_SCX0n.qn_old(), fine_scroll.RYKU_FINE_CNT0.qp_old());
    /* p27.SYBY*/ wire SYBY_SCX_FINE_MATCHp_old = xnor2(reg_scx.DUZU_SCX1n.qn_old(), fine_scroll.ROGA_FINE_CNT1.qp_old());
    /* p27.SOZU*/ wire SOZU_SCX_FINE_MATCHp_old = xnor2(reg_scx.CYXU_SCX2n.qn_old(), fine_scroll.RUBU_FINE_CNT2.qp_old());
    /*#p27.RONE*/ wire RONE_SCX_FINE_MATCHn_old = nand4(fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any(), SUHA_SCX_FINE_MATCHp_old, SYBY_SCX_FINE_MATCHp_old, SOZU_SCX_FINE_MATCHp_old);
    /*#p27.POHU*/ wire POHU_SCX_FINE_MATCHp_old = not1(RONE_SCX_FINE_MATCHn_old);

    /* p29.CEHA*/ wire CEHA_SCANNINGp = not1(sprite_scanner.CENO_SCANNINGn.qn_new());
    /*#p29.BYJO*/ wire BYJO_SCANNINGn = not1(CEHA_SCANNINGp);
    /*#p29.AZEM*/ wire AZEM_RENDERINGp = and2(XYMU_RENDERINGn.qn_new(), BYJO_SCANNINGn);
    /*#p29.AROR*/ wire AROR_MATCH_ENp = and2(AZEM_RENDERINGp, reg_lcdc.XYLO_LCDC_SPENn.qn_new());

    /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(sprite_match_flags.FEPO_STORE_MATCHp.qp_any(), WODU_HBLANKp.qp_old(), MYVO_AxCxExGx());
    /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
    /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
    /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);

    /*#p27.NYZE*/ fine_scroll.NYZE_SCX_FINE_MATCH_B.dff17_any(MOXE_AxCxExGx(),  XYMU_RENDERINGn.qn_new(), fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any());
    /*#p27.PUXA*/ fine_scroll.PUXA_SCX_FINE_MATCH_A.dff17_any(ROXO_CLKPIPE_odd, XYMU_RENDERINGn.qn_new(), POHU_SCX_FINE_MATCHp_old);

    /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp = and2(fine_scroll.PUXA_SCX_FINE_MATCH_A.qp_any(), fine_scroll.NYZE_SCX_FINE_MATCH_B.qn_any());
    /*#p27.ROXY*/ fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(PAHA_RENDERINGn, _POVA_FINE_MATCH_TRIGp);

    /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any());
    // Pixel counter, has carry lookahead because this can increment every tcycle
    /* p21.RYBO*/ wire RYBO_old = xor2(pix_count.XEHO_PX0p.qp_any(), pix_count.SAVY_PX1p.qp_any()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /* p21.XUKE*/ wire XUKE_old = and2(pix_count.XEHO_PX0p.qp_any(), pix_count.SAVY_PX1p.qp_any());
    /* p21.XYLE*/ wire XYLE_old = and2(pix_count.XODU_PX2p.qp_any(), XUKE_old);
    /* p21.XEGY*/ wire XEGY_old = xor2(pix_count.XODU_PX2p.qp_any(), XUKE_old); // feet facing gnd
    /* p21.XORA*/ wire XORA_old = xor2(pix_count.XYDO_PX3p.qp_any(), XYLE_old); // feet facing gnd

    /* p21.XEHO*/ pix_count.XEHO_PX0p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, pix_count.XEHO_PX0p.qn_any());
    /* p21.SAVY*/ pix_count.SAVY_PX1p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, RYBO_old);
    /* p21.XODU*/ pix_count.XODU_PX2p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XEGY_old);
    /* p21.XYDO*/ pix_count.XYDO_PX3p.dff17_any(SACU_CLKPIPE_evn, TADY_LINE_RSTn, XORA_old);

    /* p24.TOCA*/ wire TOCA_new = not1(pix_count.XYDO_PX3p.qp_any());
    /* p21.SAKE*/ wire SAKE_old = xor2(pix_count.TUHU_PX4p.qp_any(), pix_count.TUKY_PX5p.qp_any());
    /* p21.TYBA*/ wire TYBA_old = and2(pix_count.TUHU_PX4p.qp_any(), pix_count.TUKY_PX5p.qp_any());
    /* p21.SURY*/ wire SURY_old = and2(pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /* p21.TYGE*/ wire TYGE_old = xor2(pix_count.TAKO_PX6p.qp_any(), TYBA_old);
    /* p21.ROKU*/ wire ROKU_old = xor2(pix_count.SYBE_PX7p.qp_any(), SURY_old);

    /* p21.TUHU*/ pix_count.TUHU_PX4p.dff17_any(TOCA_new, TADY_LINE_RSTn, pix_count.TUHU_PX4p.qn_any());
    /* p21.TUKY*/ pix_count.TUKY_PX5p.dff17_any(TOCA_new, TADY_LINE_RSTn, SAKE_old);
    /* p21.TAKO*/ pix_count.TAKO_PX6p.dff17_any(TOCA_new, TADY_LINE_RSTn, TYGE_old);
    /* p21.SYBE*/ pix_count.SYBE_PX7p.dff17_any(TOCA_new, TADY_LINE_RSTn, ROKU_old);

    get_sprite_match_flags(pix_count, AROR_MATCH_ENp, sprite_store, sprite_match_flags);
  }

  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  {
    /*#p21.XENA*/ wire XENA_STORE_MATCHn = not1(sprite_match_flags.FEPO_STORE_MATCHp.qp_new());
    /*#p21.XUGU*/ wire XUGU_PX167n = nand5(pix_count.XEHO_PX0p.qp_new(), pix_count.SAVY_PX1p.qp_new(), pix_count.XODU_PX2p.qp_new(), pix_count.TUKY_PX5p.qp_new(), pix_count.SYBE_PX7p.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire XANO_PX167p = not1(XUGU_PX167n);
    /*#p21.WODU*/ WODU_HBLANKp = and2(XENA_STORE_MATCHn, XANO_PX167p); // WODU goes high on odd, cleared on H
  }

  /*#p24.VYBO*/ wire VYBO_CLKPIPE_odd = nor3(sprite_match_flags.FEPO_STORE_MATCHp.qp_new(), WODU_HBLANKp.qp_new(), MYVO_AxCxExGx());
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_odd = and3(SOCY_WIN_HITn, tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), VYBO_CLKPIPE_odd);
  /*#p24.SEGU*/ wire SEGU_CLKPIPE_evn = not1(TYFA_CLKPIPE_odd);
  /*#p24.ROXO*/ wire ROXO_CLKPIPE_odd = not1(SEGU_CLKPIPE_evn);
  /*#p24.SACU*/ wire SACU_CLKPIPE_evn = or2(SEGU_CLKPIPE_evn, fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  /* p24.PAHO*/ lcd.PAHO_X_8_SYNC.dff17(ROXO_CLKPIPE_odd, XYMU_RENDERINGn.qn_new(), XYDO_PX3p_old.qp_old());

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  sprite_bus.reset_for_pass();
  sprite_match_to_bus(sprite_store, sprite_match_flags, sprite_bus);
  sprite_scan_to_bus(sprite_scanner, delta, XYMU_RENDERINGn, sprite_match_flags.FEPO_STORE_MATCHp, sprite_bus);

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // WY/WX/window match

  tock_window(SEGU_CLKPIPE_evn, REPU_VBLANKp);

  //----------------------------------------
  // Tile fetch sequencer

  /* p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp = nor2(win_reg.RYFA_WIN_FETCHn_A.qn_new(), win_reg.RENE_WIN_FETCHn_B.qp_new());
  /* p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne = nand2(SYLO_WIN_HITn, win_reg.SOVY_WIN_HITp.qp_new());
  /* p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne = not1(TUXY_WIN_FIRST_TILEne);
  /* p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp = or3(SEKO_WIN_FETCH_TRIGp, SUZU_WIN_FIRST_TILEne, TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  /* p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(sprite_scanner.AVAP_SCAN_DONE_TRIGp.qp_new(), MOSU_WIN_MODE_TRIGp, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn = nand3(tile_fetcher._LAXU_BFETCH_S0p.qp_any(), tile_fetcher._NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn);
    /* p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(ALET_xBxDxFxH(), _MOCE_BFETCH_DONEn);

    /* p27.LAXU*/ tile_fetcher._LAXU_BFETCH_S0p.dff17_any(_LEBO_AxCxExGx,                         NYXU_BFETCH_RSTn, tile_fetcher._LAXU_BFETCH_S0p.qn_any());
    /* p27.MESU*/ tile_fetcher._MESU_BFETCH_S1p.dff17_any(tile_fetcher._LAXU_BFETCH_S0p.qn_any(), NYXU_BFETCH_RSTn, tile_fetcher._MESU_BFETCH_S1p.qn_any());
    /* p27.NYVA*/ tile_fetcher._NYVA_BFETCH_S2p.dff17_any(tile_fetcher._MESU_BFETCH_S1p.qn_any(), NYXU_BFETCH_RSTn, tile_fetcher._NYVA_BFETCH_S2p.qn_any());
  }

  /* p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp.dff17(MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, tile_fetcher.LYRY_BFETCH_DONEp.qp_old());

  /* p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(tile_fetcher._LAXU_BFETCH_S0p.qp_new(), tile_fetcher._NYVA_BFETCH_S2p.qp_new(), NYXU_BFETCH_RSTn);
  /* p27.LYRY*/ tile_fetcher.LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn = and2(tile_fetcher.LOVY_FETCH_DONEp.qn_new(), XYMU_RENDERINGn.qn_new());
  /* p27.LONY*/ tile_fetcher.LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, _LURY_BG_FETCH_DONEn);
  /* p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0p_D1.dff17(ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), tile_fetcher._LAXU_BFETCH_S0p.qp_new());

  //----------------------------------------
  // Fine match counter

  /*#p27.PASO*/ wire _PASO_FINE_RST = nor2(PAHA_RENDERINGn, TEVO_WIN_FETCH_TRIGp);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n = nand3(fine_scroll.RUBU_FINE_CNT2.qp_any(), fine_scroll.ROGA_FINE_CNT1.qp_any(), fine_scroll.RYKU_FINE_CNT0.qp_any());
    /*#p27.PECU*/ wire _PECU_FINE_CLK = nand2(ROXO_CLKPIPE_odd, _ROZE_FINE_COUNT_7n);
    /*#p27.RYKU*/ fine_scroll.RYKU_FINE_CNT0.dff17_any(_PECU_FINE_CLK,                      _PASO_FINE_RST, fine_scroll.RYKU_FINE_CNT0.qn_any());
    /*#p27.ROGA*/ fine_scroll.ROGA_FINE_CNT1.dff17_any(fine_scroll.RYKU_FINE_CNT0.qn_any(), _PASO_FINE_RST, fine_scroll.ROGA_FINE_CNT1.qn_any());
    /*#p27.RUBU*/ fine_scroll.RUBU_FINE_CNT2.dff17_any(fine_scroll.ROGA_FINE_CNT1.qn_any(), _PASO_FINE_RST, fine_scroll.RUBU_FINE_CNT2.qn_any());
  }

  //----------------------------------------
  // PPU / LCD output

  tock_pix_pipes(SACU_CLKPIPE_evn, NYXU_BFETCH_RSTn);
  set_lcd_pins(SACU_CLKPIPE_evn);
  update_framebuffer();

  //----------------------------------------
  // Memory buses

  tock_ext();
  tock_vram_bus(TEVO_WIN_FETCH_TRIGp);
  tock_oam_bus();
  tock_zram();

  //----------------------------------------
  // And finally, interrupts.

  tock_interrupts();

  const_cast<GateBoyCpuBus&>(old_bus) = new_bus;
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
