#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"

#include <set>

#define FAST_BOOT

//#pragma warning(disable:4189) // local variable unused

//-----------------------------------------------------------------------------

void GateBoy::reset_boot(uint8_t* _boot_buf, size_t _boot_size,
                         uint8_t* _cart_buf, size_t _cart_size,
                         bool fastboot) {
  boot_buf  = _boot_buf;
  boot_size = _boot_size;
  cart_buf  = _cart_buf;
  cart_size = _cart_size;

  cpu.reset_boot();

  cpu_req = {0};
  dbg_req = {0};
  bus_req = {0};
  cpu_data_latch = 0;
  imask_latch = 0;

  int_vblank = 0;
  int_vblank_halt = 0;

  int_stat = 0;
  int_stat_halt = 0;

  int_timer = 0;
  int_timer_halt = 0;

  int_serial = 0;
  int_serial_halt = 0;

  int_joypad = 0;
  int_joypad_halt = 0;

  phase_total = 0;
  pass_total = 0;
  pass_hash = HASH_INIT;
  total_hash = HASH_INIT;

  sys_rst = 1;
  sys_t1 = 0;
  sys_t2 = 0;
  sys_clken = 0;
  sys_clkgood = 0;
  sys_cpuready = 0;
  sys_cpu_en = 0;
  sys_buttons = 0;

  memset(vid_ram, 0, 8192);
  memset(cart_ram, 0, 8192);
  memset(ext_ram, 0, 8192);
  memset(oam_ram, 0, 256);
  memset(zero_ram, 0, 128);
  memset(framebuffer, 4, 160*144);

  oam_bus.reset_boot();
  ext_bus.reset_boot();
  vram_bus.reset_boot();

  clk_reg.reset_boot();
  dma_reg.reset_boot();
  int_reg.reset_boot();
  joypad.reset_boot();
  lcd_reg.reset_boot();
  pix_pipe.reset_boot();
  ser_reg.reset_boot();
  sprite_store.reset_boot();
  tim_reg.reset_boot();
  tile_fetcher.reset_boot();
  sprite_fetcher.reset_boot();
  sprite_scanner.reset_boot();
  BOOT_BITn.reset(REG_D0C0);

  SOTO_DBG_VRAMp.reset(REG_D0C0);

  IE_D0.reset(REG_D0C0);
  IE_D1.reset(REG_D0C0);
  IE_D2.reset(REG_D0C0);
  IE_D3.reset(REG_D0C0);
  IE_D4.reset(REG_D0C0);

  lcd_pix_lo.reset(0);
  lcd_pix_hi.reset(0);

  for (int i = 0; i < 160; i++) {
    lcd_pipe_lo[i].reset(REG_D0C0);
    lcd_pipe_hi[i].reset(REG_D0C0);
  }

  run_reset_sequence(fastboot);
}

//-----------------------------------------------------------------------------

void GateBoy::reset_cart(uint8_t* _boot_buf, size_t _boot_size,
                         uint8_t* _cart_buf, size_t _cart_size) {
  boot_buf  = _boot_buf;
  boot_size = _boot_size;
  cart_buf  = _cart_buf;
  cart_size = _cart_size;

  cpu.reset_cart();

  cpu_req.addr = 0xff50;
  cpu_req.data = 1;
  cpu_req.read = 0;
  cpu_req.write = 1;
  dbg_req = {0};
  bus_req = cpu_req;

  cpu_data_latch = 1;
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

  phase_total = 0x02cf5798;
  pass_total = 0x0c23db7e;
  pass_hash = 0xdd0849d964666f73;
  total_hash = 0xdfa0b6a3a264e502;

  sys_rst = 0;
  sys_t1 = 0;
  sys_t2 = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  sys_cpuready = 1;
  sys_cpu_en = 1;

  memcpy(vid_ram, vram_boot, 8192);
  memset(cart_ram, 0, sizeof(cart_ram));
  memset(ext_ram, 0, sizeof(ext_ram));
  memset(oam_ram, 0, sizeof(oam_ram));
  memset(zero_ram, 0, sizeof(zero_ram));
  zero_ram[0x7A] = 0x39;
  zero_ram[0x7B] = 0x01;
  zero_ram[0x7C] = 0x2E;
  memcpy(framebuffer, framebuffer_boot, 160*144);

  gb_screen_x = 0;
  gb_screen_y = 152;
  lcd_data_latch = 0;

  oam_bus.reset_cart();
  ext_bus.reset_cart();
  vram_bus.reset_cart();

  clk_reg.reset_cart();
  dma_reg.reset_cart();
  int_reg.reset_cart();
  joypad.reset_cart();
  lcd_reg.reset_cart();
  pix_pipe.reset_cart();
  ser_reg.reset_cart();
  sprite_store.reset_cart();
  tim_reg.reset_cart();
  tile_fetcher.reset_cart();
  sprite_fetcher.reset_cart();
  sprite_scanner.reset_cart();
  BOOT_BITn.reset(REG_D1C1);

  SOTO_DBG_VRAMp.reset(REG_D0C1);

  IE_D0.reset(REG_D0C1);
  IE_D1.reset(REG_D0C1);
  IE_D2.reset(REG_D0C1);
  IE_D3.reset(REG_D0C1);
  IE_D4.reset(REG_D0C1);

  lcd_pix_lo.reset(0);
  lcd_pix_hi.reset(0);

  for (int i = 0; i < 160; i++) {
    lcd_pipe_lo[i].reset(REG_D0C0);
    lcd_pipe_hi[i].reset(REG_D0C0);
  }
}

//------------------------------------------------------------------------------

void GateBoy::load_post_bootrom_state() {
  load_obj("gateboy_post_bootrom.raw.dump", *this);
  check_sentinel();
  check_div();
  cart_buf = nullptr;
  cart_size = 0;
}

//------------------------------------------------------------------------------

void GateBoy::run_reset_sequence(bool fastboot) {
  sys_in_reset_sequence = 1;

  //LOG_G("Run reset sequence\n");
  //LOG_SCOPE_INDENT();

  //----------------------------------------

  CHECK_P(cart_buf != nullptr);
  CHECK_P(cart_size);

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

  CHECK_N(clk_reg.AFUR_xxxxEFGHp.qp_old());
  CHECK_P(clk_reg.ALEF_AxxxxFGHp.qp_old());
  CHECK_P(clk_reg.APUK_ABxxxxGHp.qp_old());
  CHECK_P(clk_reg.ADYK_ABCxxxxHp.qp_old());

  // Wait for PIN_CPU_START
  while(!sys_cpu_start) {
    run(8);
  }

  // Delay to sync w/ expected div value after bootrom
  run(8);
  run(8);

  // Done, initialize bus with whatever the CPU wants.
  cpu.reset_boot();
  sys_cpuready = 1;
  sys_cpu_en = true;

  sys_in_reset_sequence = 0;
}

//------------------------------------------------------------------------------

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


//-----------------------------------------------------------------------------

struct GateBoyOffsets {
  const int o_oam_bus        = offsetof(GateBoy, oam_bus);
  const int o_ext_bus        = offsetof(GateBoy, ext_bus);
  const int o_vram_bus       = offsetof(GateBoy, vram_bus);
  const int o_clk_reg        = offsetof(GateBoy, clk_reg);
  const int o_dma_reg        = offsetof(GateBoy, dma_reg);
  const int o_int_reg        = offsetof(GateBoy, int_reg);
  const int o_joypad         = offsetof(GateBoy, joypad);
  const int o_lcd_reg        = offsetof(GateBoy, lcd_reg);
  const int o_pix_pipe       = offsetof(GateBoy, pix_pipe);
  const int o_ser_reg        = offsetof(GateBoy, ser_reg);
  const int o_sprite_store   = offsetof(GateBoy, sprite_store);
  const int o_tim_reg        = offsetof(GateBoy, tim_reg);
  const int o_tile_fetcher   = offsetof(GateBoy, tile_fetcher);
  const int o_sprite_fetcher = offsetof(GateBoy, sprite_fetcher);
  const int o_sprite_scanner = offsetof(GateBoy, sprite_scanner);
  const int o_pins           = offsetof(GateBoy, PIN_CPU_WAKE);
  //const int o_vram           = offsetof(GateBoy, PIN_VRAM_Ap);
  const int o_lcd_pix        = offsetof(GateBoy, lcd_pix_lo);

} gb_offsets;

static std::set<int> bad_bits;

//-----------------------------------------------------------------------------

void GateBoy::next_phase() {

  //----------------------------------------
  // Run one pass of our simulation.

  uint8_t* blob_begin = ((uint8_t*)&sentinel1) + sizeof(sentinel1);
  uint8_t* blob_end   = ((uint8_t*)&sentinel2);

  probes.begin_pass(0);
  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);
  tock_slow(0);
  pass_total++;
  probes.end_pass(false);

  uint64_t pass_hash_old = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

  GateBoy gb_old = *this;

  probes.begin_pass(1);
  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);
  tock_slow(1);
  pass_total++;
  probes.end_pass(true);

  uint64_t pass_hash_new = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

  if (pass_hash_old != pass_hash_new) {
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

  //----------------------------------------
  // Once the simulation converges, latch the data that needs to go back to the
  // CPU or test function and update the CPU if necessary.

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
  pass_hash = pass_hash_new;
  combine_hash(total_hash, pass_hash);
}

//-----------------------------------------------------------------------------

void GateBoy::tock_slow(int pass_index) {

  if (DELTA_HA || DELTA_BC || DELTA_DE || DELTA_FG) {
    vram_data_latch = vid_ram[vram_addr_latch];
  }

  if (pass_index == 0) {
    oam_data_latch_a = oam_ram[(oam_addr_latch << 1) + 0];
    oam_data_latch_b = oam_ram[(oam_addr_latch << 1) + 1];
  }


  if (DELTA_AB) {
    cpu_req = cpu.bus_req;
    bus_req = {0};
    if (sys_cpu_en) bus_req = cpu_req;
    if (dbg_req.read || dbg_req.write) bus_req = dbg_req;
  }

  PIN_CPU_ACK_VBLANK.pin_in(1, wire(cpu.int_ack & INT_VBLANK_MASK));
  PIN_CPU_ACK_STAT  .pin_in(1, wire(cpu.int_ack & INT_STAT_MASK));
  PIN_CPU_ACK_TIMER .pin_in(1, wire(cpu.int_ack & INT_TIMER_MASK));
  PIN_CPU_ACK_SERIAL.pin_in(1, wire(cpu.int_ack & INT_SERIAL_MASK));
  PIN_CPU_ACK_JOYPAD.pin_in(1, wire(cpu.int_ack & INT_JOYPAD_MASK));

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  uint16_t cpu_bus_addr;

  cpu_bus_addr = bus_req.addr;
  if (DELTA_HA) cpu_bus_addr &= 0x00FF;

  // PIN_CPU_RDp / PIN_CPU_WRp
  if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
    PIN_CPU_RDp.pin_in(1, bus_req.read);
    PIN_CPU_WRp.pin_in(1, bus_req.write);
  }
  else {
    PIN_CPU_RDp.pin_in(1, 0);
    PIN_CPU_WRp.pin_in(1, 0);
  }


  // not at all certain about this. seems to break some oam read glitches.
  if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req.read && (bus_req.addr < 0xFF00))) {
    PIN_CPU_LATCH_EXT.pin_in(1, 1);
  }
  else {
    PIN_CPU_LATCH_EXT.pin_in(1, 0);
  }
  /* p04.DECY*/ wire _DECY_LATCH_EXTn_ext = not1(PIN_CPU_LATCH_EXT.qp_ext());
  /* p04.CATY*/ wire _CATY_LATCH_EXTp_ext = not1(_DECY_LATCH_EXTn_ext);

  PIN_CPU_6.pin_in(1, 0);

  // Data has to be driven on EFGH or we fail the wave tests
  wire BUS_CPU_OEp = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req.write;

  BUS_CPU_Dp_ext[0].reset();
  BUS_CPU_Dp_ext[1].reset();
  BUS_CPU_Dp_ext[2].reset();
  BUS_CPU_Dp_ext[3].reset();
  BUS_CPU_Dp_ext[4].reset();
  BUS_CPU_Dp_ext[5].reset();
  BUS_CPU_Dp_ext[6].reset();
  BUS_CPU_Dp_ext[7].reset();

  BUS_CPU_Dp_ext[0].tri(BUS_CPU_OEp, bus_req.data_lo & 0x01);
  BUS_CPU_Dp_ext[1].tri(BUS_CPU_OEp, bus_req.data_lo & 0x02);
  BUS_CPU_Dp_ext[2].tri(BUS_CPU_OEp, bus_req.data_lo & 0x04);
  BUS_CPU_Dp_ext[3].tri(BUS_CPU_OEp, bus_req.data_lo & 0x08);
  BUS_CPU_Dp_ext[4].tri(BUS_CPU_OEp, bus_req.data_lo & 0x10);
  BUS_CPU_Dp_ext[5].tri(BUS_CPU_OEp, bus_req.data_lo & 0x20);
  BUS_CPU_Dp_ext[6].tri(BUS_CPU_OEp, bus_req.data_lo & 0x40);
  BUS_CPU_Dp_ext[7].tri(BUS_CPU_OEp, bus_req.data_lo & 0x80);

  BUS_CPU_Dp_out[0].reset();
  BUS_CPU_Dp_out[1].reset();
  BUS_CPU_Dp_out[2].reset();
  BUS_CPU_Dp_out[3].reset();
  BUS_CPU_Dp_out[4].reset();
  BUS_CPU_Dp_out[5].reset();
  BUS_CPU_Dp_out[6].reset();
  BUS_CPU_Dp_out[7].reset();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  wire GND = 0;
  wire _VYPO_VCC = 1;

  //----------------------------------------
  // Address decoders

  bool BUS_CPU_A_ext[16];
  BUS_CPU_A_ext[ 0] = wire(cpu_bus_addr & 0x0001);
  BUS_CPU_A_ext[ 1] = wire(cpu_bus_addr & 0x0002);
  BUS_CPU_A_ext[ 2] = wire(cpu_bus_addr & 0x0004);
  BUS_CPU_A_ext[ 3] = wire(cpu_bus_addr & 0x0008);
  BUS_CPU_A_ext[ 4] = wire(cpu_bus_addr & 0x0010);
  BUS_CPU_A_ext[ 5] = wire(cpu_bus_addr & 0x0020);
  BUS_CPU_A_ext[ 6] = wire(cpu_bus_addr & 0x0040);
  BUS_CPU_A_ext[ 7] = wire(cpu_bus_addr & 0x0080);
  BUS_CPU_A_ext[ 8] = wire(cpu_bus_addr & 0x0100);
  BUS_CPU_A_ext[ 9] = wire(cpu_bus_addr & 0x0200);
  BUS_CPU_A_ext[10] = wire(cpu_bus_addr & 0x0400);
  BUS_CPU_A_ext[11] = wire(cpu_bus_addr & 0x0800);
  BUS_CPU_A_ext[12] = wire(cpu_bus_addr & 0x1000);
  BUS_CPU_A_ext[13] = wire(cpu_bus_addr & 0x2000);
  BUS_CPU_A_ext[14] = wire(cpu_bus_addr & 0x4000);
  BUS_CPU_A_ext[15] = wire(cpu_bus_addr & 0x8000);

  /* p06.SARE*/ wire _SARE_XX00_XX07p_ext = nor5 (BUS_CPU_A_ext[ 7], BUS_CPU_A_ext[ 6], BUS_CPU_A_ext[ 5],
                                                  BUS_CPU_A_ext[ 4], BUS_CPU_A_ext[ 3]);
  /* p07.TUNA*/ wire _TUNA_0000_FDFFp_ext = nand7(BUS_CPU_A_ext[15], BUS_CPU_A_ext[14], BUS_CPU_A_ext[13],
                                                  BUS_CPU_A_ext[12], BUS_CPU_A_ext[11], BUS_CPU_A_ext[10],
                                                  BUS_CPU_A_ext[ 9]);

  /* p07.TULO*/ wire _TULO_00000000_XXp_ext = nor8(BUS_CPU_A_ext[15], BUS_CPU_A_ext[14], BUS_CPU_A_ext[13], BUS_CPU_A_ext[12],
                                                   BUS_CPU_A_ext[11], BUS_CPU_A_ext[10], BUS_CPU_A_ext[ 9], BUS_CPU_A_ext[ 8]);
  /* p07.ZORO*/ wire _ZORO_0000xxxx_XXp_ext = nor4(BUS_CPU_A_ext[15], BUS_CPU_A_ext[14], BUS_CPU_A_ext[13], BUS_CPU_A_ext[12]);
  /* p07.ZADU*/ wire _ZADU_xxxx0000_XXp_ext = nor4(BUS_CPU_A_ext[11], BUS_CPU_A_ext[10], BUS_CPU_A_ext[ 9], BUS_CPU_A_ext[ 8]);

  /* p07.SEMY*/ wire _SEMY_XX_0000xxxxp_ext = nor4(BUS_CPU_A_ext[ 7], BUS_CPU_A_ext[ 6], BUS_CPU_A_ext[ 5], BUS_CPU_A_ext[ 4]);
  /* p07.SAPA*/ wire _SAPA_XX_xxxx1111p_ext = and4(BUS_CPU_A_ext[ 0], BUS_CPU_A_ext[ 1], BUS_CPU_A_ext[ 2], BUS_CPU_A_ext[ 3]);
  /* p22.XALY*/ wire _XALY_XX_0x00xxxxp_ext = nor3(BUS_CPU_A_ext[ 7], BUS_CPU_A_ext[ 5], BUS_CPU_A_ext[ 4]);
  /* p10.AMUS*/ wire _AMUS_XX_0xx00000p_ext = nor6(BUS_CPU_A_ext[ 0], BUS_CPU_A_ext[ 1], BUS_CPU_A_ext[ 2], BUS_CPU_A_ext[ 3], BUS_CPU_A_ext[ 4], BUS_CPU_A_ext[ 7]);
  /* p07.TYRO*/ wire _TYFO_XX_0x0x0000p_ext = nor6(BUS_CPU_A_ext[ 7], BUS_CPU_A_ext[ 5], BUS_CPU_A_ext[ 3], BUS_CPU_A_ext[ 2], BUS_CPU_A_ext[ 1], BUS_CPU_A_ext[ 0]);
  /* p07.TUFA*/ wire _TUFA_XX_x1x1xxxxp_ext = and2(BUS_CPU_A_ext[ 4], BUS_CPU_A_ext[ 6]);

  /* p03.TOVY*/ wire _TOVY_A00n_ext = not1(BUS_CPU_A_ext[ 0]);
  /* p22.XOLA*/ wire _XOLA_A00n_ext = not1(BUS_CPU_A_ext[ 0]);
  /* p08.TOLA*/ wire _TOLA_A01n_ext = not1(BUS_CPU_A_ext[ 1]);
  /* p22.XENO*/ wire _XENO_A01n_ext = not1(BUS_CPU_A_ext[ 1]);
  /* p06.SEFY*/ wire _SEFY_A02n_ext = not1(BUS_CPU_A_ext[ 2]);
  /* p22.XUSY*/ wire _XUSY_A02n_ext = not1(BUS_CPU_A_ext[ 2]);
  /* p22.XERA*/ wire _XERA_A03n_ext = not1(BUS_CPU_A_ext[ 3]);
  /* p10.BYKO*/ wire _BYKO_A05n_ext = not1(BUS_CPU_A_ext[ 5]);
  /* p10.AKUG*/ wire _AKUG_A06n_ext = not1(BUS_CPU_A_ext[ 6]);
  /* p07.TONA*/ wire _TONA_A08n_ext = not1(BUS_CPU_A_ext[ 8]);
  /* p08.SOGY*/ wire _SOGY_A14n_ext = not1(BUS_CPU_A_ext[14]);
  /*#p08.SORE*/ wire _SORE_A15n_ext = not1(BUS_CPU_A_ext[15]);

  /* p22.WADO*/ wire _WADO_A00p_ext = not1(_XOLA_A00n_ext);
  /* p22.WESA*/ wire _WESA_A01p_ext = not1(_XENO_A01n_ext);
  /* p22.WALO*/ wire _WALO_A02p_ext = not1(_XUSY_A02n_ext);
  /* p22.WEPO*/ wire _WEPO_A03p_ext = not1(_XERA_A03n_ext);

  /* p07.ZUFA*/ wire _ZUFA_0000_00FFp_ext  = and2(_ZORO_0000xxxx_XXp_ext, _ZADU_xxxx0000_XXp_ext);
  /* p07.SYKE*/ wire _SYKE_FF00_FFFFp_ext  = nor2(_TUNA_0000_FDFFp_ext, _TONA_A08n_ext);
  /* p03.RYFO*/ wire _RYFO_FF04_FF07p_ext  = and3(_SYKE_FF00_FFFFp_ext, BUS_CPU_A_ext[ 2], _SARE_XX00_XX07p_ext);
  /* p25.SYRO*/ wire _SYRO_FE00_FFFFp_ext  = not1(_TUNA_0000_FDFFp_ext);
  /* p07.RYCU*/ wire _RYCU_0000_FDFFn_ext  = not1(_TUNA_0000_FDFFp_ext);
  /* p07.SOHA*/ wire _SOHA_FF00_FFFFn_ext  = not1(_SYKE_FF00_FFFFp_ext);
  /* p07.ROPE*/ wire _ROPE_FE00_FEFFn_ext  = nand2(_SOHA_FF00_FFFFn_ext, _RYCU_0000_FDFFn_ext);
  /* p07.SARO*/ wire _SARO_FE00_FEFFp_ext  = not1(_ROPE_FE00_FEFFn_ext);
  /* p28.ADAH*/ wire _ADAH_FE00_FEFFn_ext  = not1(_SARO_FE00_FEFFp_ext);
  /*#p08.TEVY*/ wire _TEVY_8000_9FFFn_ext  = or3(BUS_CPU_A_ext[13], BUS_CPU_A_ext[14], _SORE_A15n_ext);
  /* p06.SANO*/ wire _SANO_FF00_FF03p_ext  = and3(_SARE_XX00_XX07p_ext, _SEFY_A02n_ext, _SYKE_FF00_FFFFp_ext);
  /* p22.WUTU*/ wire _WUTU_FF40_FF4Fn_ext  = nand3(_SYKE_FF00_FFFFp_ext, BUS_CPU_A_ext[ 6], _XALY_XX_0x00xxxxp_ext);
  /* p22.WERO*/ wire _WERO_FF40_FF4Fp_ext  = not1(_WUTU_FF40_FF4Fn_ext);
  /* p08.TUMA*/ wire _TUMA_A000_BFFFp_ext  = and3(BUS_CPU_A_ext[13], _SOGY_A14n_ext, BUS_CPU_A_ext[15]);
  /* p08.TYNU*/ wire _TYNU_A000_FFFFp_ext  = and_or3(BUS_CPU_A_ext[15], BUS_CPU_A_ext[14], _TUMA_A000_BFFFp_ext);
  /* p10.ANAP*/ wire _ANAP_FF_0xx00000_ext = and2(_SYKE_FF00_FFFFp_ext, _AMUS_XX_0xx00000p_ext);

  /* p22.WORU*/ wire _WORU_FF40n_ext = nand5(_WERO_FF40_FF4Fp_ext, _XOLA_A00n_ext, _XENO_A01n_ext, _XUSY_A02n_ext, _XERA_A03n_ext);
  /* p22.WOFA*/ wire _WOFA_FF41n_ext = nand5(_WERO_FF40_FF4Fp_ext, _WADO_A00p_ext, _XENO_A01n_ext, _XUSY_A02n_ext, _XERA_A03n_ext);
  /* p22.WEBU*/ wire _WEBU_FF42n_ext = nand5(_WERO_FF40_FF4Fp_ext, _XOLA_A00n_ext, _WESA_A01p_ext, _XUSY_A02n_ext, _XERA_A03n_ext);
  /* p22.WAVU*/ wire _WAVU_FF43n_ext = nand5(_WERO_FF40_FF4Fp_ext, _WADO_A00p_ext, _WESA_A01p_ext, _XUSY_A02n_ext, _XERA_A03n_ext);
  /* p22.WYLE*/ wire _WYLE_FF44n_ext = nand5(_WERO_FF40_FF4Fp_ext, _XOLA_A00n_ext, _XENO_A01n_ext, _WALO_A02p_ext, _XERA_A03n_ext);
  /* p22.WETY*/ wire _WETY_FF45n_ext = nand5(_WERO_FF40_FF4Fp_ext, _WADO_A00p_ext, _XENO_A01n_ext, _WALO_A02p_ext, _XERA_A03n_ext);
  /*#p22.WATE*/ wire _WATE_FF46n_ext = nand5(_WERO_FF40_FF4Fp_ext, _XOLA_A00n_ext, _WESA_A01p_ext, _WALO_A02p_ext, _XERA_A03n_ext);
  /* p22.WYBO*/ wire _WYBO_FF47n_ext = nand5(_WERO_FF40_FF4Fp_ext, _WADO_A00p_ext, _WESA_A01p_ext, _WALO_A02p_ext, _XERA_A03n_ext);
  /* p22.WETA*/ wire _WETA_FF48n_ext = nand5(_WERO_FF40_FF4Fp_ext, _XOLA_A00n_ext, _XENO_A01n_ext, _XUSY_A02n_ext, _WEPO_A03p_ext);
  /* p22.VAMA*/ wire _VAMA_FF49n_ext = nand5(_WERO_FF40_FF4Fp_ext, _WADO_A00p_ext, _XENO_A01n_ext, _XUSY_A02n_ext, _WEPO_A03p_ext);
  /* p22.WYVO*/ wire _WYVO_FF4An_ext = nand5(_WERO_FF40_FF4Fp_ext, _XOLA_A00n_ext, _WESA_A01p_ext, _XUSY_A02n_ext, _WEPO_A03p_ext);
  /* p22.WAGE*/ wire _WAGE_FF4Bn_ext = nand5(_WERO_FF40_FF4Fp_ext, _WADO_A00p_ext, _WESA_A01p_ext, _XUSY_A02n_ext, _WEPO_A03p_ext);

  /* p22.VOCA*/ wire _VOCA_FF40p_ext = not1(_WORU_FF40n_ext);
  /* p22.VARY*/ wire _VARY_FF41p_ext = not1(_WOFA_FF41n_ext);
  /* p22.XARO*/ wire _XARO_FF42p_ext = not1(_WEBU_FF42n_ext);
  /* p22.XAVY*/ wire _XAVY_FF43p_ext = not1(_WAVU_FF43n_ext);
  /* p22.XOGY*/ wire _XOGY_FF44p_ext = not1(_WYLE_FF44n_ext);
  /* p22.XAYU*/ wire _XAYU_FF45p_ext = not1(_WETY_FF45n_ext);
  /*#p22.XEDA*/ wire _XEDA_FF46p_ext = not1(_WATE_FF46n_ext);
  /* p22.WERA*/ wire _WERA_FF47p_ext = not1(_WYBO_FF47n_ext);
  /* p22.XAYO*/ wire _XAYO_FF48p_ext = not1(_WETA_FF48n_ext);
  /* p22.TEGO*/ wire _TEGO_FF49p_ext = not1(_VAMA_FF49n_ext);
  /* p22.VYGA*/ wire _VYGA_FF4Ap_ext = not1(_WYVO_FF4An_ext);
  /* p22.VUMY*/ wire _VUMY_FF4Bp_ext = not1(_WAGE_FF4Bn_ext);


  {
    PIN_CPU_ADDR_HIp.pin_in(1, _SYRO_FE00_FFFFp_ext);
  }

  //----------------------------------------
  // Debug control signals.

  /* p07.UBET*/ wire _UBETp_ext  = not1(wire(sys_t1));
  /* p07.UVAR*/ wire _UVARp_ext  = not1(wire(sys_t2));
  /* p07.UMUT*/ wire _UMUT_MODE_DBG1p_ext = and2(wire(sys_t1), _UVARp_ext);
  /* p07.UNOR*/ wire _UNOR_MODE_DBG2p_ext = and2(wire(sys_t2), _UBETp_ext);
  /* p07.UPOJ*/ wire _UPOJ_MODE_PRODn_ext = nand3(_UBETp_ext, _UVARp_ext, wire(sys_rst));
  /* p08.RYCA*/ wire _RYCA_MODE_DBG2n_ext = not1(_UNOR_MODE_DBG2p_ext);
  /* p08.TOVA*/ wire _TOVA_MODE_DBG2n_ext = not1(_UNOR_MODE_DBG2p_ext);
  /* p08.MULE*/ wire _MULE_MODE_DBG1n_ext = not1(_UMUT_MODE_DBG1p_ext);

  {
    PIN_CPU_UNOR_DBG.pin_in(1, _UNOR_MODE_DBG2p_ext);
    PIN_CPU_UMUT_DBG.pin_in(1, _UMUT_MODE_DBG1p_ext);
  }

  //----------------------------------------
  // Root clocks - ignoring the deglitcher here

  /* p01.UCOB*/ wire _UCOB_CLKBADp_ext  = not1(sys_clkgood);
  /* p01.ATEZ*/ wire _ATEZ_CLKBADp_ext  = not1(sys_clkgood);
  /* p01.ABOL*/ wire _ABOL_CLKREQn_ext  = not1(sys_cpuready);

  /* p01.ATAL*/ wire _ATAL_xBxDxFxH_clk = !(phase_total & 1) && sys_clken;
  /* p01.AZOF*/ wire _AZOF_AxCxExGx_clk = not1(_ATAL_xBxDxFxH_clk);
  /* p01.ZAXY*/ wire _ZAXY_xBxDxFxH_clk = not1(_AZOF_AxCxExGx_clk);
  /*#p01.ZEME*/ wire _ZEME_AxCxExGx_clk = not1(_ZAXY_xBxDxFxH_clk);
  /*#p01.ALET*/ wire _ALET_xBxDxFxH_clk = not1(_ZEME_AxCxExGx_clk);
  /*#p27.MYVO*/ wire _MYVO_AxCxExGx_clk = not1(_ALET_xBxDxFxH_clk);
  /*#p27.MEHE*/ wire _MEHE_AxCxExGx_clk = not1(_ALET_xBxDxFxH_clk);
  /* p27.MOXE*/ wire _MOXE_AxCxExGx_clk = not1(_ALET_xBxDxFxH_clk);

  //----------------------------------------
  // Sys clock signals

  {
    wire ADYK_ABCxxxxHp_old = clk_reg.ADYK_ABCxxxxHp.qp_old();
    wire AFUR_xxxxEFGHn_old = clk_reg.AFUR_xxxxEFGHp.qn_old();
    wire ALEF_AxxxxFGHn_old = clk_reg.ALEF_AxxxxFGHp.qn_old();
    wire APUK_ABxxxxGHn_old = clk_reg.APUK_ABxxxxGHp.qn_old();

    /* p01.AFUR*/ clk_reg.AFUR_xxxxEFGHp.dff9(!_ATAL_xBxDxFxH_clk, _UPOJ_MODE_PRODn_ext, ADYK_ABCxxxxHp_old);
    /* p01.ALEF*/ clk_reg.ALEF_AxxxxFGHp.dff9( _ATAL_xBxDxFxH_clk, _UPOJ_MODE_PRODn_ext, AFUR_xxxxEFGHn_old);
    /* p01.APUK*/ clk_reg.APUK_ABxxxxGHp.dff9(!_ATAL_xBxDxFxH_clk, _UPOJ_MODE_PRODn_ext, ALEF_AxxxxFGHn_old);
    /* p01.ADYK*/ clk_reg.ADYK_ABCxxxxHp.dff9( _ATAL_xBxDxFxH_clk, _UPOJ_MODE_PRODn_ext, APUK_ABxxxxGHn_old);
  }

  /*#p01.ATYP*/ wire _ATYP_ABCDxxxx_clk = not1(clk_reg.AFUR_xxxxEFGHp.qp_new());
  /*#p01.AFEP*/ wire _AFEP_AxxxxFGH_clk = not1(clk_reg.ALEF_AxxxxFGHp.qn_new());
  /*#p01.AROV*/ wire _AROV_xxCDEFxx_clk = not1(clk_reg.APUK_ABxxxxGHp.qp_new());
  /*#p01.ADAR*/ wire _ADAR_ABCxxxxH_clk = not1(clk_reg.ADYK_ABCxxxxHp.qn_new());

  /*#p01.AFAS*/ wire _AFAS_xxxxEFGx_clk = nor2(_ADAR_ABCxxxxH_clk, _ATYP_ABCDxxxx_clk);
  /*#p01.BELU*/ wire _BELU_xxxxEFGH_clk = nor2(_ATYP_ABCDxxxx_clk, _ABOL_CLKREQn_ext);
  /*#p01.BYRY*/ wire _BYRY_ABCDxxxx_clk = not1(_BELU_xxxxEFGH_clk);
  /*#p01.BUDE*/ wire _BUDE_xxxxEFGH_clk = not1(_BYRY_ABCDxxxx_clk);
  /*#p01.BAPY*/ wire _BAPY_xxxxxxGH_clk = nor3(_ABOL_CLKREQn_ext, _AROV_xxCDEFxx_clk, _ATYP_ABCDxxxx_clk);
  /*#p01.BERU*/ wire _BERU_ABCDEFxx_clk = not1(_BAPY_xxxxxxGH_clk);
  /*#p01.BUFA*/ wire _BUFA_xxxxxxGH_clk = not1(_BERU_ABCDEFxx_clk);
  /*#p01.BOLO*/ wire _BOLO_ABCDEFxx_clk = not1(_BUFA_xxxxxxGH_clk);
  /*#p01.BEKO*/ wire _BEKO_ABCDxxxx_clk = not1(_BUDE_xxxxEFGH_clk); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire _BEJA_xxxxEFGH_clk = nand4(_BOLO_ABCDEFxx_clk, _BOLO_ABCDEFxx_clk, _BEKO_ABCDxxxx_clk, _BEKO_ABCDxxxx_clk);
  /*#p01.BANE*/ wire _BANE_ABCDxxxx_clk = not1(_BEJA_xxxxEFGH_clk);
  /*#p01.BELO*/ wire _BELO_xxxxEFGH_clk = not1(_BANE_ABCDxxxx_clk);
  /*#p01.BAZE*/ wire _BAZE_ABCDxxxx_clk = not1(_BELO_xxxxEFGH_clk);
  /*#p01.BUTO*/ wire _BUTO_xBCDEFGH_clk = nand3(_AFEP_AxxxxFGH_clk, _ATYP_ABCDxxxx_clk, _BAZE_ABCDxxxx_clk);
  /*#p01.BELE*/ wire _BELE_Axxxxxxx_clk = not1(_BUTO_xBCDEFGH_clk);
  /*#p01.BYJU*/ wire _BYJU_Axxxxxxx_clk = or2(_BELE_Axxxxxxx_clk, _ATEZ_CLKBADp_ext);
  /*#p01.BALY*/ wire _BALY_xBCDEFGH_clk = not1(_BYJU_Axxxxxxx_clk);
  /* p01.BOGA*/ wire _BOGA_Axxxxxxx_clk = not1(_BALY_xBCDEFGH_clk);
  /*#p01.AJAX*/ wire _AJAX_xxxxEFGH_clk = not1(_ATYP_ABCDxxxx_clk);
  /* p01.UVYT*/ wire _UVYT_ABCDxxxx_clk = not1(_BUDE_xxxxEFGH_clk);
  /* p04.MOPA*/ wire _MOPA_xxxxEFGH_clk = not1(_UVYT_ABCDxxxx_clk);

  //----------------------------------------
  // CPU write signals

  /* p01.AREV*/ wire _AREV_CPU_WRn_ABCDxxxH_clk = nand2(PIN_CPU_WRp.qp_ext(), _AFAS_xxxxEFGx_clk);
  /* p01.APOV*/ wire _APOV_CPU_WRp_xxxxEFGx_clk = not1(_AREV_CPU_WRn_ABCDxxxH_clk);
  /* p07.UBAL*/ wire _UBAL_CPU_WRn_ABCDxxxH_clk = mux2n(_UNOR_MODE_DBG2p_ext, /*PIN_EXT_WRn.qn_new()*/ 0, _APOV_CPU_WRp_xxxxEFGx_clk); // Ignoring debug stuff for now
  /* p07.TAPU*/ wire _TAPU_CPU_WRp_xxxxEFGx_clk = not1(_UBAL_CPU_WRn_ABCDxxxH_clk);
  /* p07.DYKY*/ wire _DYKY_CPU_WRn_ABCDxxxH_clk = not1(_TAPU_CPU_WRp_xxxxEFGx_clk);
  /* p07.CUPA*/ wire _CUPA_CPU_WRp_xxxxEFGx_clk = not1(_DYKY_CPU_WRn_ABCDxxxH_clk);

  /* p10.ATOZ*/ wire _ATOZ_FF00_WRn_clk = nand4(_TAPU_CPU_WRp_xxxxEFGx_clk, _ANAP_FF_0xx00000_ext, _AKUG_A06n_ext, _BYKO_A05n_ext);
  /* p06.URYS*/ wire _URYS_FF01_WRn_clk = nand4(_TAPU_CPU_WRp_xxxxEFGx_clk, _SANO_FF00_FF03p_ext, BUS_CPU_A_ext[ 0], _TOLA_A01n_ext);
  /* p06.UWAM*/ wire _UWAM_FF02_WRn_clk = nand4(_TAPU_CPU_WRp_xxxxEFGx_clk, _SANO_FF00_FF03p_ext, _TOVY_A00n_ext, BUS_CPU_A_ext[ 1]);
  /* p01.TAPE*/ wire _TAPE_FF04_WRp_clk =  and4(_TAPU_CPU_WRp_xxxxEFGx_clk, _RYFO_FF04_FF07p_ext,  _TOLA_A01n_ext, _TOVY_A00n_ext);
  /*#p03.TOPE*/ wire _TOPE_FF05_WRn_clk = nand4(_TAPU_CPU_WRp_xxxxEFGx_clk, _RYFO_FF04_FF07p_ext,  BUS_CPU_A_ext[ 0], _TOLA_A01n_ext);
  /* p03.TYJU*/ wire _TYJU_FF06_WRn_clk = nand4(_TAPU_CPU_WRp_xxxxEFGx_clk, _RYFO_FF04_FF07p_ext,  BUS_CPU_A_ext[ 1], _TOVY_A00n_ext);
  /* p03.SARA*/ wire _SARA_FF07_WRn_clk = nand4(_TAPU_CPU_WRp_xxxxEFGx_clk, _RYFO_FF04_FF07p_ext,  BUS_CPU_A_ext[ 1], BUS_CPU_A_ext[ 0]);
  /* p07.REFA*/ wire _REFA_FF0F_WRn_clk = nand4(_TAPU_CPU_WRp_xxxxEFGx_clk, _SYKE_FF00_FFFFp_ext,  _SEMY_XX_0000xxxxp_ext, _SAPA_XX_xxxx1111p_ext); // schematic wrong, is NAND

  /* p23.WARU*/ wire _WARU_FF40_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _VOCA_FF40p_ext);
  /* p21.SEPA*/ wire _SEPA_FF41_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _VARY_FF41p_ext);
  /* p23.BEDY*/ wire _BEDY_FF42_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _XARO_FF42p_ext);
  /* p23.ARUR*/ wire _ARUR_FF43_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _XAVY_FF43p_ext);
  /* p23.XUFA*/ wire _XUFA_FF45_WRn_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _XAYU_FF45p_ext);
  /*#p04.LAVY*/ wire _LAVY_FF46_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _XEDA_FF46p_ext);
  /* p36.VELY*/ wire _VELY_FF47_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _WERA_FF47p_ext);
  /* p36.XOMA*/ wire _XOMA_FF48_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _XAYO_FF48p_ext);
  /* p36.MYXE*/ wire _MYXE_FF49_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _TEGO_FF49p_ext);
  /* p23.WEKO*/ wire _WEKO_FF4A_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _VYGA_FF4Ap_ext);
  /* p23.WUZA*/ wire _WUZA_FF4B_WRp_clk = and2(_CUPA_CPU_WRp_xxxxEFGx_clk, _VUMY_FF4Bp_ext);

  /* p23.XUBO*/ wire _XUBO_FF40_WRn_clk = not1(_WARU_FF40_WRp_clk);
  /* p21.RYVE*/ wire _RYVE_FF41_WRn_clk = not1(_SEPA_FF41_WRp_clk);
  /* p21.RYJU*/ wire _RYJU_FF41_WRn_clk = not1(_SEPA_FF41_WRp_clk);
  /* p23.CAVO*/ wire _CAVO_FF42_WRn_clk = not1(_BEDY_FF42_WRp_clk);
  /* p23.AMUN*/ wire _AMUN_FF43_WRn_clk = not1(_ARUR_FF43_WRp_clk);
  /* p23.WANE*/ wire _WANE_FF45_WRp_clk = not1(_XUFA_FF45_WRn_clk);
  /* p36.TEPO*/ wire _TEPO_FF47_WRp_clk = not1(_VELY_FF47_WRp_clk);
  /* p36.XELO*/ wire _XELO_FF48_WRn_clk = not1(_XOMA_FF48_WRp_clk);
  /* p36.LEHO*/ wire _LEHO_FF49_WRn_clk = not1(_MYXE_FF49_WRp_clk);
  /* p23.VEFU*/ wire _VEFU_FF4A_WRn_clk = not1(_WEKO_FF4A_WRp_clk);
  /* p23.VOXU*/ wire _VOXU_FF4B_WRn_clk = not1(_WUZA_FF4B_WRp_clk);

  //----------------------------------------
  // Clocks that go back to the CPU

  {
    /*#p01.BUTY*/ wire _BUTY_CLKREQ_ext   = not1(_ABOL_CLKREQn_ext);
    /*#p01.BUVU*/ wire _BUVU_Axxxxxxx_clk = and2(_BALY_xBCDEFGH_clk, _BUTY_CLKREQ_ext);
    /*#p01.BYXO*/ wire _BYXO_xBCDEFGH_clk = not1(_BUVU_Axxxxxxx_clk);
    /*#p01.BEDO*/ wire _BEDO_Axxxxxxx_clk = not1(_BYXO_xBCDEFGH_clk);
    /*#p01.BOWA*/ wire _BOWA_xBCDEFGH_clk = not1(_BEDO_Axxxxxxx_clk);

    /*#p01.BUGO*/ wire _BUGO_xBCDExxx_clk = not1(_AFEP_AxxxxFGH_clk);
    /*#p01.BATE*/ wire _BATE_AxxxxxGH_clk = nor3(_BUGO_xBCDExxx_clk, _AROV_xxCDEFxx_clk, _ABOL_CLKREQn_ext);
    /*#p01.BASU*/ wire _BASU_xBCDEFxx_clk = not1(_BATE_AxxxxxGH_clk);
    /*#p01.BUKE*/ wire _BUKE_AxxxxxGH_clk = not1(_BASU_xBCDEFxx_clk);
    /*#p01.BOMA*/ wire _BOMA_xBCDEFGH_clk = not1(_BOGA_Axxxxxxx_clk);

    PIN_CPU_BOWA_Axxxxxxx.pin_in(1, _BOWA_xBCDEFGH_clk);
    PIN_CPU_BEDO_xBCDEFGH.pin_in(1, _BEDO_Axxxxxxx_clk);
    PIN_CPU_BEKO_ABCDxxxx.pin_in(1, _BEKO_ABCDxxxx_clk);
    PIN_CPU_BUDE_xxxxEFGH.pin_in(1, _BUDE_xxxxEFGH_clk);
    PIN_CPU_BOLO_ABCDEFxx.pin_in(1, _BOLO_ABCDEFxx_clk);
    PIN_CPU_BUKE_AxxxxxGH.pin_in(1, _BUKE_AxxxxxGH_clk);
    PIN_CPU_BOMA_xBCDEFGH.pin_in(1, _BOMA_xBCDEFGH_clk);
    PIN_CPU_BOGA_Axxxxxxx.pin_in(1, _BOGA_Axxxxxxx_clk);
  }

  //----------------------------------------
  // CPU read signals

  // Ignoring debug stuff for now
  /* p07.UJYV*/ wire _UJYV_CPU_RDn_ext = mux2n(_UNOR_MODE_DBG2p_ext, /*PIN_EXT_RDn.qn_new()*/ 0, PIN_CPU_RDp.qp_ext());
  /* p07.TEDO*/ wire _TEDO_CPU_RDp_ext = not1(_UJYV_CPU_RDn_ext);
  /* p07.AJAS*/ wire _AJAS_CPU_RDn_ext = not1(_TEDO_CPU_RDp_ext);
  /* p07.ASOT*/ wire _ASOT_CPU_RDp_ext = not1(_AJAS_CPU_RDn_ext);
  /* p08.REDU*/ wire _REDU_CPU_RDn_ext = not1(_TEDO_CPU_RDp_ext);

  /* p10.ACAT*/ wire _ACAT_FF00_RDp_ext =  and4(_TEDO_CPU_RDp_ext, _ANAP_FF_0xx00000_ext,  _AKUG_A06n_ext,         _BYKO_A05n_ext);
  /* p06.UFEG*/ wire _UFEG_FF01_RDp_ext =  and4(_TEDO_CPU_RDp_ext, _SANO_FF00_FF03p_ext,   BUS_CPU_A_ext[ 0],      _TOLA_A01n_ext);
  /* p06.UCOM*/ wire _UCOM_FF02_RDp_ext =  and4(_TEDO_CPU_RDp_ext, _SANO_FF00_FF03p_ext,   _TOVY_A00n_ext,         BUS_CPU_A_ext[ 1]);
  /* p01.TAGY*/ wire _TAGY_FF04_RDp_ext =  and4(_TEDO_CPU_RDp_ext, _RYFO_FF04_FF07p_ext,   _TOLA_A01n_ext,         _TOVY_A00n_ext);
  /*#p03.TEDA*/ wire _TEDA_FF05_RDp_ext =  and4(_TEDO_CPU_RDp_ext, _RYFO_FF04_FF07p_ext,   _TOLA_A01n_ext,         BUS_CPU_A_ext[ 0]);
  /* p03.TUBY*/ wire _TUBY_FF06_RDp_ext =  and4(_TEDO_CPU_RDp_ext, _RYFO_FF04_FF07p_ext,   BUS_CPU_A_ext[ 1],      _TOVY_A00n_ext);
  /* p03.SORA*/ wire _SORA_FF07_RDp_ext =  and4(_TEDO_CPU_RDp_ext, _RYFO_FF04_FF07p_ext,   BUS_CPU_A_ext[ 1],      BUS_CPU_A_ext[ 0]);
  /* p07.ROLO*/ wire _ROLO_FF0F_RDn_ext = nand4(_TEDO_CPU_RDp_ext, _SYKE_FF00_FFFFp_ext,   _SEMY_XX_0000xxxxp_ext, _SAPA_XX_xxxx1111p_ext); // schematic wrong, is NAND
  /* p07.TEXE*/ wire _TEXE_FF50_RDp_ext =  and4(_TEDO_CPU_RDp_ext, _SYKE_FF00_FFFFp_ext,   _TYFO_XX_0x0x0000p_ext, _TUFA_XX_x1x1xxxxp_ext);
  /* p23.VYRE*/ wire _VYRE_FF40_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _VOCA_FF40p_ext);
  /* p21.TOBE*/ wire _TOBE_FF41_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _VARY_FF41p_ext);
  /* p23.ANYP*/ wire _ANYP_FF42_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _XARO_FF42p_ext);
  /* p23.AVOG*/ wire _AVOG_FF43_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _XAVY_FF43p_ext);
  /* p23.WAFU*/ wire _WAFU_FF44_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _XOGY_FF44p_ext);
  /* p23.XYLY*/ wire _XYLY_FF45_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _XAYU_FF45p_ext);
  /*#p04.MOLU*/ wire _MOLU_FF46_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _XEDA_FF46p_ext);
  /* p36.VUSO*/ wire _VUSO_FF47_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _WERA_FF47p_ext);
  /* p36.XUFY*/ wire _XUFY_FF48_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _XAYO_FF48p_ext);
  /* p36.MUMY*/ wire _MUMY_FF49_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _TEGO_FF49p_ext);
  /* p23.WAXU*/ wire _WAXU_FF4A_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _VYGA_FF4Ap_ext);
  /* p23.WYZE*/ wire _WYZE_FF4B_RDp_ext =  and2(_ASOT_CPU_RDp_ext, _VUMY_FF4Bp_ext);

  /* p05.BYZO*/ wire _BYZO_FF00_RDn_ext = not1(_ACAT_FF00_RDp_ext);
  /* p23.WYCE*/ wire _WYCE_FF40_RDn_ext = not1(_VYRE_FF40_RDp_ext);
  /* p21.VAVE*/ wire _VAVE_FF41_RDn_ext = not1(_TOBE_FF41_RDp_ext);
  /* p23.BUWY*/ wire _BUWY_FF42_RDn_ext = not1(_ANYP_FF42_RDp_ext);
  /* p23.BEBA*/ wire _BEBA_FF43_RDn_ext = not1(_AVOG_FF43_RDp_ext);
  /* p23.WEKU*/ wire _WEKU_FF45_RDn_ext = not1(_XYLY_FF45_RDp_ext);
  /*#p04.NYGO*/ wire _NYGO_FF46_RDn_ext = not1(_MOLU_FF46_RDp_ext);
  /* p36.TEPY*/ wire _TEPY_FF47_RDn_ext = not1(_VUSO_FF47_RDp_ext);
  /* p36.XOZY*/ wire _XOZY_FF48_RDn_ext = not1(_XUFY_FF48_RDp_ext);
  /* p36.LOTE*/ wire _LOTE_FF49_RDn_ext = not1(_MUMY_FF49_RDp_ext);
  /* p23.VOMY*/ wire _VOMY_FF4A_RDn_ext = not1(_WAXU_FF4A_RDp_ext);
  /* p23.VYCU*/ wire _VYCU_FF4B_RDn_ext = not1(_WYZE_FF4B_RDp_ext);

  /*#p04.PUSY*/ wire _PUSY_FF46_RDp_ext = not1(_NYGO_FF46_RDn_ext);

  wire FFFF_HIT_ext = cpu_bus_addr == 0xFFFF;
  wire FFFF_RDn_ext = nand2(_TEDO_CPU_RDp_ext, FFFF_HIT_ext);
  wire FFFF_WRn_ext = nand2(_TAPU_CPU_WRp_xxxxEFGx_clk, FFFF_HIT_ext);

  //----------------------------------------

  [this, _UCOB_CLKBADp_ext, _BOGA_Axxxxxxx_clk, _TAGY_FF04_RDp_ext, _TAPE_FF04_WRp_clk]() {
    /* p01.UFOL*/ wire _UFOL_DIV_RSTn_ext = nor3(_UCOB_CLKBADp_ext, sys_rst, _TAPE_FF04_WRp_clk);
    /* p01.UKUP*/ tim_reg.UKUP_DIV00p.dff17(_BOGA_Axxxxxxx_clk,           _UFOL_DIV_RSTn_ext, tim_reg.UKUP_DIV00p.qn_old());
    /* p01.UFOR*/ tim_reg.UFOR_DIV01p.dff17(tim_reg.UKUP_DIV00p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.UFOR_DIV01p.qn_old());
    /* p01.UNER*/ tim_reg.UNER_DIV02p.dff17(tim_reg.UFOR_DIV01p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.UNER_DIV02p.qn_old());
    /*#p01.TERO*/ tim_reg.TERO_DIV03p.dff17(tim_reg.UNER_DIV02p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.TERO_DIV03p.qn_old());
    /* p01.UNYK*/ tim_reg.UNYK_DIV04p.dff17(tim_reg.TERO_DIV03p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.UNYK_DIV04p.qn_old());
    /* p01.TAMA*/ tim_reg.TAMA_DIV05p.dff17(tim_reg.UNYK_DIV04p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.TAMA_DIV05p.qn_old());

    // this is hacked up because we're ignoring the debug reg for the moment
    /* p01.ULUR*/ wire _ULUR_DIV_06_clknew = /*mux2p(FF60_1, _BOGA_Axxxxxxx,*/ tim_reg.TAMA_DIV05p.qn_new() /*)*/;

    /* p01.UGOT*/ tim_reg.UGOT_DIV06p.dff17(_ULUR_DIV_06_clknew,          _UFOL_DIV_RSTn_ext, tim_reg.UGOT_DIV06p.qn_old());
    /* p01.TULU*/ tim_reg.TULU_DIV07p.dff17(tim_reg.UGOT_DIV06p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.TULU_DIV07p.qn_old());
    /* p01.TUGO*/ tim_reg.TUGO_DIV08p.dff17(tim_reg.TULU_DIV07p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.TUGO_DIV08p.qn_old());
    /* p01.TOFE*/ tim_reg.TOFE_DIV09p.dff17(tim_reg.TUGO_DIV08p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.TOFE_DIV09p.qn_old());
    /* p01.TERU*/ tim_reg.TERU_DIV10p.dff17(tim_reg.TOFE_DIV09p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.TERU_DIV10p.qn_old());
    /* p01.SOLA*/ tim_reg.SOLA_DIV11p.dff17(tim_reg.TERU_DIV10p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.SOLA_DIV11p.qn_old());
    /* p01.SUBU*/ tim_reg.SUBU_DIV12p.dff17(tim_reg.SOLA_DIV11p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.SUBU_DIV12p.qn_old());
    /* p01.TEKA*/ tim_reg.TEKA_DIV13p.dff17(tim_reg.SUBU_DIV12p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.TEKA_DIV13p.qn_old());
    /* p01.UKET*/ tim_reg.UKET_DIV14p.dff17(tim_reg.TEKA_DIV13p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.UKET_DIV14p.qn_old());
    /* p01.UPOF*/ tim_reg.UPOF_DIV15p.dff17(tim_reg.UKET_DIV14p.qn_new(), _UFOL_DIV_RSTn_ext, tim_reg.UPOF_DIV15p.qn_old());

    /* FF04 DIV */
    /* p01.UMEK*/ wire _UMEK_DIV06n = not1(tim_reg.UGOT_DIV06p.qp_new());
    /* p01.UREK*/ wire _UREK_DIV07n = not1(tim_reg.TULU_DIV07p.qp_new());
    /* p01.UTOK*/ wire _UTOK_DIV08n = not1(tim_reg.TUGO_DIV08p.qp_new());
    /* p01.SAPY*/ wire _SAPY_DIV09n = not1(tim_reg.TOFE_DIV09p.qp_new());
    /* p01.UMER*/ wire _UMER_DIV10n = not1(tim_reg.TERU_DIV10p.qp_new());
    /* p01.RAVE*/ wire _RAVE_DIV11n = not1(tim_reg.SOLA_DIV11p.qp_new());
    /* p01.RYSO*/ wire _RYSO_DIV12n = not1(tim_reg.SUBU_DIV12p.qp_new());
    /* p01.UDOR*/ wire _UDOR_DIV13n = not1(tim_reg.TEKA_DIV13p.qp_new());

    /* p01.TAWU*/ BUS_CPU_Dp_out[0].tri6_pn(_TAGY_FF04_RDp_ext, _UMEK_DIV06n);
    /* p01.TAKU*/ BUS_CPU_Dp_out[1].tri6_pn(_TAGY_FF04_RDp_ext, _UREK_DIV07n);
    /* p01.TEMU*/ BUS_CPU_Dp_out[2].tri6_pn(_TAGY_FF04_RDp_ext, _UTOK_DIV08n);
    /* p01.TUSE*/ BUS_CPU_Dp_out[3].tri6_pn(_TAGY_FF04_RDp_ext, _SAPY_DIV09n);
    /* p01.UPUG*/ BUS_CPU_Dp_out[4].tri6_pn(_TAGY_FF04_RDp_ext, _UMER_DIV10n); // Schematic wrong, UPUG/SEPU driving D5/D4
    /* p01.SEPU*/ BUS_CPU_Dp_out[5].tri6_pn(_TAGY_FF04_RDp_ext, _RAVE_DIV11n);
    /* p01.SAWA*/ BUS_CPU_Dp_out[6].tri6_pn(_TAGY_FF04_RDp_ext, _RYSO_DIV12n);
    /* p01.TATU*/ BUS_CPU_Dp_out[7].tri6_pn(_TAGY_FF04_RDp_ext, _UDOR_DIV13n);
  }();

  //----------------------------------------
  // Power-on reset handler

  [this, _UCOB_CLKBADp_ext, _UMUT_MODE_DBG1p_ext, _UNOR_MODE_DBG2p_ext, _UPOJ_MODE_PRODn_ext, _BOGA_Axxxxxxx_clk]() {
    /* p01.AFER*/ clk_reg.AFER_SYS_RSTp_s.dff13(_BOGA_Axxxxxxx_clk, _UPOJ_MODE_PRODn_ext, clk_reg.ASOL_POR_DONEn_s.qp_old());

    /* p01.UPYF*/ wire _UPYF_ext = or2(sys_rst, _UCOB_CLKBADp_ext);
    /* p01.TUBO*/ clk_reg.TUBO_WAITINGp_s.nor_latch(_UPYF_ext, sys_cpuready);
    /* p01.UNUT*/ wire _UNUT_POR_TRIGn_new = and2(clk_reg.TUBO_WAITINGp_s.qp_new(), sys_fastboot ? tim_reg.TERO_DIV03p.qp_new() : tim_reg.UPOF_DIV15p.qp_new());
    /* p01.TABA*/ wire _TABA_POR_TRIGn_new = or3(_UNOR_MODE_DBG2p_ext, _UMUT_MODE_DBG1p_ext, _UNUT_POR_TRIGn_new);
    /*#p01.ALYP*/ wire _ALYP_RSTn_new = not1(_TABA_POR_TRIGn_new);
    /*#p01.AFAR*/ wire _AFAR_RSTp_new  = nor2(sys_rst, _ALYP_RSTn_new);
    /* p01.ASOL*/ clk_reg.ASOL_POR_DONEn_s.nor_latch(sys_rst, _AFAR_RSTp_new); // Schematic wrong, this is a latch.

    PIN_CPU_EXT_CLKGOOD.pin_in(1, sys_clkgood);
    PIN_CPU_EXT_RST.pin_in(1, sys_rst);
    PIN_CPU_STARTp.pin_in(1, _TABA_POR_TRIGn_new);
    sys_cpu_start = PIN_CPU_STARTp.qp_ext();

    PIN_CPU_SYS_RSTp.pin_in(1, clk_reg.AFER_SYS_RSTp_s.qp_new());
  }();

  /*#p01.AVOR*/ wire _AVOR_SYS_RSTp_new = or2(clk_reg.AFER_SYS_RSTp_s.qp_new(), clk_reg.ASOL_POR_DONEn_s.qp_new());
  /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(_AVOR_SYS_RSTp_new);
  /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
  /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);
  /*#p01.XORE*/ wire _XORE_SYS_RSTp_new = not1(_CUNU_SYS_RSTn_new);

  //----------------------------------------

  {
    // FF50 - disable bootrom bit
    /* p07.TUGE*/ wire _TUGE_FF50_WRn_clk = nand4(_TAPU_CPU_WRp_xxxxEFGx_clk, _SYKE_FF00_FFFFp_ext, _TYFO_XX_0x0x0000p_ext, _TUFA_XX_x1x1xxxxp_ext);
    /* p07.SATO*/ wire _SATO_BOOT_BITn_old = or2(BUS_CPU_Dp_ext[0].qp_ext(), BOOT_BITn.qp_old());
    /* p07.TEPU*/ BOOT_BITn.dff17(_TUGE_FF50_WRn_clk, _ALUR_SYS_RSTn_new, _SATO_BOOT_BITn_old);
  }
  /* p07.TERA*/ wire _TERA_BOOT_BITp_new  = not1(BOOT_BITn.qp_new());
  /* p07.TUTU*/ wire _TUTU_ADDR_BOOTp_new = and2(_TERA_BOOT_BITp_new, _TULO_00000000_XXp_ext);

  PIN_CPU_BOOTp.reset();
  PIN_CPU_BOOTp.pin_in(1, _TUTU_ADDR_BOOTp_new);

  {
    bool addr_ext = (bus_req.read || bus_req.write) && (bus_req.addr < 0xFE00);
    if (bus_req.addr <= 0x00FF && !BOOT_BITn.qp_new()) addr_ext = false;

    if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      PIN_CPU_EXT_BUSp.pin_in(1, addr_ext);
    }
    else {
      // This seems wrong, but it passes tests. *shrug*
      if (bus_req.addr >= 0x8000 && bus_req.addr <= 0x9FFF) {
        PIN_CPU_EXT_BUSp.pin_in(1, 0);
      }
      else {
        PIN_CPU_EXT_BUSp.pin_in(1, addr_ext);
      }
    }
  }

  /*#p08.TEXO*/ wire _TEXO_8000_9FFFn_ext  = and2(PIN_CPU_EXT_BUSp.qp_ext(), _TEVY_8000_9FFFn_ext);
  /*#p25.TEFA*/ wire _TEFA_8000_9FFFp_ext  = nor2(_SYRO_FE00_FFFFp_ext, _TEXO_8000_9FFFn_ext);
  /*#p25.SOSE*/ wire _SOSE_8000_9FFFp_ext  = and2(_TEFA_8000_9FFFp_ext, BUS_CPU_A_ext[15]);
  /*#p01.AGUT*/ wire _AGUT_xxCDEFGH_clk = or_and3(_AROV_xxCDEFxx_clk, _AJAX_xxxxEFGH_clk, PIN_CPU_EXT_BUSp.qp_ext());
  /*#p01.AWOD*/ wire _AWOD_ABxxxxxx_clk = nor2(_UNOR_MODE_DBG2p_ext, _AGUT_xxCDEFGH_clk);
  /*#p01.ABUZ*/ wire _ABUZ_xxCDEFGH_clk = not1(_AWOD_ABxxxxxx_clk);

  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(_TEXO_8000_9FFFn_ext, _UMUT_MODE_DBG1p_ext);
  /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_ext = not1(_TEXO_8000_9FFFn_ext);
  /* p08.LAGU*/ wire _LAGU_ext = and_or3(PIN_CPU_RDp.qp_ext(), _LEVO_ADDR_INT_OR_ADDR_VRAM_ext, PIN_CPU_WRp.qp_ext());
  /* p08.LYWE*/ wire _LYWE_ext = not1(_LAGU_ext);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_ext = or2(_MOCA_DBG_EXT_RD_ext, _LYWE_ext);

  // Ignoring debug for now
  /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn_ext = nand2(_SOSE_8000_9FFFp_ext, PIN_CPU_WRp.qp_ext());  // Schematic wrong, second input is PIN_CPU_WRp
  ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_VRAM_OEn.qn_new());
  /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn_ext = /*mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp,*/ _TEGU_CPU_VRAM_WRn_ext /*)*/;

  /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp_ext = and2(_SOSE_8000_9FFFp_ext, _ABUZ_xxCDEFGH_clk);
  ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_VRAM_CSn.qn_new());
  /*#p25.TOLE*/ wire _TOLE_CPU_VRAM_RDp_ext = /*mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, */ _TUCA_CPU_VRAM_RDp_ext /*)*/;

  {
    /*#p25.SYCY*/ wire _SYCY_DBG_CLOCKn_ext = not1(_UNOR_MODE_DBG2p_ext);
    /*#p25.SOTO*/ SOTO_DBG_VRAMp.dff17(_SYCY_DBG_CLOCKn_ext, _CUNU_SYS_RSTn_new, SOTO_DBG_VRAMp.qn_old());
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, _ALUR_SYS_RSTn_new, _BOGA_Axxxxxxx_clk, _TEDA_FF05_RDp_ext, _TUBY_FF06_RDp_ext, _SORA_FF07_RDp_ext, _TOPE_FF05_WRn_clk, _TYJU_FF06_WRn_clk, _SARA_FF07_WRn_clk]() {
    /*#p03.MERY*/ wire _MERY_TIMER_OVERFLOWp_old = nor2(tim_reg.NUGA_TIMA7p.qp_old(), tim_reg.NYDU_TIMA7p_DELAY.qn_old());
    /*#p03.MOBA*/ tim_reg.MOBA_TIMER_OVERFLOWp.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, _MERY_TIMER_OVERFLOWp_old);

    /*#p03.MEKE*/ wire _MEKE_TIMER_OVERFLOWn_new = not1(tim_reg.MOBA_TIMER_OVERFLOWp.qp_new());
    /*#p03.MUZU*/ wire _MUZU_CPU_LOAD_TIMAn_ext  = or2(PIN_CPU_LATCH_EXT.qp_ext(), _TOPE_FF05_WRn_clk);
    /*#p03.MEXU*/ wire _MEXU_TIMA_LOADp_new      = nand3(_MUZU_CPU_LOAD_TIMAn_ext, _ALUR_SYS_RSTn_new, _MEKE_TIMER_OVERFLOWn_new);
    /*#p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn_new   = not1(_MEXU_TIMA_LOADp_new);
    /*#p03.NYDU*/ tim_reg.NYDU_TIMA7p_DELAY   .dff17(_BOGA_Axxxxxxx_clk, _MUGY_TIMA_MAX_RSTn_new, tim_reg.NUGA_TIMA7p.qp_old());

    // FF06 TMA
    /* p03.SABU*/ tim_reg.SABU_TMA0p.dff17(_TYJU_FF06_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p03.NYKE*/ tim_reg.NYKE_TMA1p.dff17(_TYJU_FF06_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p03.MURU*/ tim_reg.MURU_TMA2p.dff17(_TYJU_FF06_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[2].qp_ext());
    /* p03.TYVA*/ tim_reg.TYVA_TMA3p.dff17(_TYJU_FF06_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p03.TYRU*/ tim_reg.TYRU_TMA4p.dff17(_TYJU_FF06_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p03.SUFY*/ tim_reg.SUFY_TMA5p.dff17(_TYJU_FF06_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p03.PETO*/ tim_reg.PETO_TMA6p.dff17(_TYJU_FF06_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[6].qp_ext());
    /* p03.SETA*/ tim_reg.SETA_TMA7p.dff17(_TYJU_FF06_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[7].qp_ext());

    // FF07 TAC
    /* p03.SOPU*/ tim_reg.SOPU_TAC0p.dff17(_SARA_FF07_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p03.SAMY*/ tim_reg.SAMY_TAC1p.dff17(_SARA_FF07_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p03.SABO*/ tim_reg.SABO_TAC2p.dff17(_SARA_FF07_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[2].qp_ext());

    /*#p03.ROKE*/ wire _ROKE_TIMA_D0_new = mux2n(_TOPE_FF05_WRn_clk, tim_reg.SABU_TMA0p.qp_new(), BUS_CPU_Dp_ext[0].qp_ext());
    /*#p03.PETU*/ wire _PETU_TIMA_D1_new = mux2n(_TOPE_FF05_WRn_clk, tim_reg.NYKE_TMA1p.qp_new(), BUS_CPU_Dp_ext[1].qp_ext());
    /*#p03.NYKU*/ wire _NYKU_TIMA_D2_new = mux2n(_TOPE_FF05_WRn_clk, tim_reg.MURU_TMA2p.qp_new(), BUS_CPU_Dp_ext[2].qp_ext());
    /*#p03.SOCE*/ wire _SOCE_TIMA_D3_new = mux2n(_TOPE_FF05_WRn_clk, tim_reg.TYVA_TMA3p.qp_new(), BUS_CPU_Dp_ext[3].qp_ext());
    /*#p03.SALA*/ wire _SALA_TIMA_D4_new = mux2n(_TOPE_FF05_WRn_clk, tim_reg.TYRU_TMA4p.qp_new(), BUS_CPU_Dp_ext[4].qp_ext());
    /*#p03.SYRU*/ wire _SYRU_TIMA_D5_new = mux2n(_TOPE_FF05_WRn_clk, tim_reg.SUFY_TMA5p.qp_new(), BUS_CPU_Dp_ext[5].qp_ext());
    /*#p03.REFU*/ wire _REFU_TIMA_D6_new = mux2n(_TOPE_FF05_WRn_clk, tim_reg.PETO_TMA6p.qp_new(), BUS_CPU_Dp_ext[6].qp_ext());
    /*#p03.RATO*/ wire _RATO_TIMA_D7_new = mux2n(_TOPE_FF05_WRn_clk, tim_reg.SETA_TMA7p.qp_new(), BUS_CPU_Dp_ext[7].qp_ext());

    /* p03.MULO*/ wire _MULO_SYS_RSTn_new = not1(_ALUR_SYS_RSTn_new);
    /*#p03.PUXY*/ wire _PUXY_TIMA_D0_new = nor2(_MULO_SYS_RSTn_new, _ROKE_TIMA_D0_new);
    /*#p03.NERO*/ wire _NERO_TIMA_D1_new = nor2(_MULO_SYS_RSTn_new, _PETU_TIMA_D1_new);
    /*#p03.NADA*/ wire _NADA_TIMA_D2_new = nor2(_MULO_SYS_RSTn_new, _NYKU_TIMA_D2_new);
    /*#p03.REPA*/ wire _REPA_TIMA_D3_new = nor2(_MULO_SYS_RSTn_new, _SOCE_TIMA_D3_new);
    /*#p03.ROLU*/ wire _ROLU_TIMA_D4_new = nor2(_MULO_SYS_RSTn_new, _SALA_TIMA_D4_new);
    /*#p03.RUGY*/ wire _RUGY_TIMA_D5_new = nor2(_MULO_SYS_RSTn_new, _SYRU_TIMA_D5_new);
    /*#p03.PYMA*/ wire _PYMA_TIMA_D6_new = nor2(_MULO_SYS_RSTn_new, _REFU_TIMA_D6_new);
    /*#p03.PAGU*/ wire _PAGU_TIMA_D7_new = nor2(_MULO_SYS_RSTn_new, _RATO_TIMA_D7_new);

    /*#p03.UBOT*/ wire _UBOT_DIV01n_new = not1(tim_reg.UFOR_DIV01p.qp_new());
    /*#p03.UVYR*/ wire _UVYR_DIV03n_new = not1(tim_reg.TERO_DIV03p.qp_new());
    /* p01.UVYN*/ wire _UVYN_DIV05n_new = not1(tim_reg.TAMA_DIV05p.qp_new());
    /* p01.UREK*/ wire _UREK_DIV07n_new = not1(tim_reg.TULU_DIV07p.qp_new());

    /*#p03.UKAP*/ wire _UKAP_CLK_MUXa_new  = mux2n(tim_reg.SOPU_TAC0p.qp_new(), _UVYN_DIV05n_new, _UVYR_DIV03n_new);
    /*#p03.TEKO*/ wire _TEKO_CLK_MUXb_new  = mux2n(tim_reg.SOPU_TAC0p.qp_new(), _UBOT_DIV01n_new, _UREK_DIV07n_new);
    /*#p03.TECY*/ wire _TECY_CLK_MUXc_new  = mux2n(tim_reg.SAMY_TAC1p.qp_new(), _UKAP_CLK_MUXa_new, _TEKO_CLK_MUXb_new);
    /*#p03.SOGU*/ wire _SOGU_TIMA_CLKn_new = nor2(_TECY_CLK_MUXc_new, tim_reg.SABO_TAC2p.qn_new());

    // note the data input here is async because of LOADp, so it has to be a new signal
    /*#p03.REGA*/ tim_reg.REGA_TIMA0p.dff20(_SOGU_TIMA_CLKn_new,          _MEXU_TIMA_LOADp_new, _PUXY_TIMA_D0_new);
    /*#p03.POVY*/ tim_reg.POVY_TIMA1p.dff20(tim_reg.REGA_TIMA0p.qp_new(), _MEXU_TIMA_LOADp_new, _NERO_TIMA_D1_new);
    /*#p03.PERU*/ tim_reg.PERU_TIMA2p.dff20(tim_reg.POVY_TIMA1p.qp_new(), _MEXU_TIMA_LOADp_new, _NADA_TIMA_D2_new);
    /*#p03.RATE*/ tim_reg.RATE_TIMA3p.dff20(tim_reg.PERU_TIMA2p.qp_new(), _MEXU_TIMA_LOADp_new, _REPA_TIMA_D3_new);
    /*#p03.RUBY*/ tim_reg.RUBY_TIMA4p.dff20(tim_reg.RATE_TIMA3p.qp_new(), _MEXU_TIMA_LOADp_new, _ROLU_TIMA_D4_new);
    /*#p03.RAGE*/ tim_reg.RAGE_TIMA5p.dff20(tim_reg.RUBY_TIMA4p.qp_new(), _MEXU_TIMA_LOADp_new, _RUGY_TIMA_D5_new);
    /*#p03.PEDA*/ tim_reg.PEDA_TIMA6p.dff20(tim_reg.RAGE_TIMA5p.qp_new(), _MEXU_TIMA_LOADp_new, _PYMA_TIMA_D6_new);
    /*#p03.NUGA*/ tim_reg.NUGA_TIMA7p.dff20(tim_reg.PEDA_TIMA6p.qp_new(), _MEXU_TIMA_LOADp_new, _PAGU_TIMA_D7_new);

    /* FF05 TIMA */
    /*#p03.SOKU*/ BUS_CPU_Dp_out[0].tri6_pn(_TEDA_FF05_RDp_ext, tim_reg.REGA_TIMA0p.qn_new());
    /*#p03.RACY*/ BUS_CPU_Dp_out[1].tri6_pn(_TEDA_FF05_RDp_ext, tim_reg.POVY_TIMA1p.qn_new());
    /*#p03.RAVY*/ BUS_CPU_Dp_out[2].tri6_pn(_TEDA_FF05_RDp_ext, tim_reg.PERU_TIMA2p.qn_new());
    /*#p03.SOSY*/ BUS_CPU_Dp_out[3].tri6_pn(_TEDA_FF05_RDp_ext, tim_reg.RATE_TIMA3p.qn_new());
    /*#p03.SOMU*/ BUS_CPU_Dp_out[4].tri6_pn(_TEDA_FF05_RDp_ext, tim_reg.RUBY_TIMA4p.qn_new());
    /*#p03.SURO*/ BUS_CPU_Dp_out[5].tri6_pn(_TEDA_FF05_RDp_ext, tim_reg.RAGE_TIMA5p.qn_new());
    /*#p03.ROWU*/ BUS_CPU_Dp_out[6].tri6_pn(_TEDA_FF05_RDp_ext, tim_reg.PEDA_TIMA6p.qn_new());
    /*#p03.PUSO*/ BUS_CPU_Dp_out[7].tri6_pn(_TEDA_FF05_RDp_ext, tim_reg.NUGA_TIMA7p.qn_new());

    /* FF06 TMA */
    /*#p03.SETE*/ BUS_CPU_Dp_out[0].tri6_pn(_TUBY_FF06_RDp_ext, tim_reg.SABU_TMA0p.qn_new());
    /*#p03.PYRE*/ BUS_CPU_Dp_out[1].tri6_pn(_TUBY_FF06_RDp_ext, tim_reg.NYKE_TMA1p.qn_new());
    /*#p03.NOLA*/ BUS_CPU_Dp_out[2].tri6_pn(_TUBY_FF06_RDp_ext, tim_reg.MURU_TMA2p.qn_new());
    /*#p03.SALU*/ BUS_CPU_Dp_out[3].tri6_pn(_TUBY_FF06_RDp_ext, tim_reg.TYVA_TMA3p.qn_new());
    /*#p03.SUPO*/ BUS_CPU_Dp_out[4].tri6_pn(_TUBY_FF06_RDp_ext, tim_reg.TYRU_TMA4p.qn_new());
    /*#p03.SOTU*/ BUS_CPU_Dp_out[5].tri6_pn(_TUBY_FF06_RDp_ext, tim_reg.SUFY_TMA5p.qn_new());
    /*#p03.REVA*/ BUS_CPU_Dp_out[6].tri6_pn(_TUBY_FF06_RDp_ext, tim_reg.PETO_TMA6p.qn_new());
    /*#p03.SAPU*/ BUS_CPU_Dp_out[7].tri6_pn(_TUBY_FF06_RDp_ext, tim_reg.SETA_TMA7p.qn_new());

    /* FF07 TAC */
    /*#p03.RYLA*/ BUS_CPU_Dp_out[0].tri6_pn(_SORA_FF07_RDp_ext, tim_reg.SOPU_TAC0p.qn_new());
    /*#p03.ROTE*/ BUS_CPU_Dp_out[1].tri6_pn(_SORA_FF07_RDp_ext, tim_reg.SAMY_TAC1p.qn_new());
    /*#p03.SUPE*/ BUS_CPU_Dp_out[2].tri6_pn(_SORA_FF07_RDp_ext, tim_reg.SABO_TAC2p.qn_new());
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  /* p23.WYMO*/ wire _WYMO_LCDC_WINENp_old = pix_pipe.WYMO_LCDC_WINENn.qn_old();

#pragma region Regs_Write
  {
    // FF40 LCDC
    /* p01.XARE*/ wire _XARE_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);
    /*#p23.VYXE*/ pix_pipe.VYXE_LCDC_BGENn  .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p23.XYLO*/ pix_pipe.XYLO_LCDC_SPENn  .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p23.XYMO*/ pix_pipe.XYMO_LCDC_SPSIZEn.dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_Dp_ext[2].qp_ext());
    /* p23.XAFO*/ pix_pipe.XAFO_LCDC_BGMAPn .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p23.WEXU*/ pix_pipe.WEXU_LCDC_BGTILEn.dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p23.WYMO*/ pix_pipe.WYMO_LCDC_WINENn .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p23.WOKY*/ pix_pipe.WOKY_LCDC_WINMAPn.dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_Dp_ext[6].qp_ext());
    /* p23.XONA*/ pix_pipe.XONA_LCDC_LCDENn .dff9(_XUBO_FF40_WRn_clk, _XARE_SYS_RSTn_new, BUS_CPU_Dp_ext[7].qp_ext());
  }
  {
    // FF41 STAT
    /* p01.WESY*/ wire _WESY_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);
    /* p21.ROXE*/ pix_pipe.ROXE_STAT_HBI_ENn.dff9(_RYVE_FF41_WRn_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p21.RUFO*/ pix_pipe.RUFO_STAT_VBI_ENn.dff9(_RYVE_FF41_WRn_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p21.REFE*/ pix_pipe.REFE_STAT_OAI_ENn.dff9(_RYVE_FF41_WRn_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p21.RUGU*/ pix_pipe.RUGU_STAT_LYI_ENn.dff9(_RYVE_FF41_WRn_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[6].qp_ext());
  }
  {
  }

  {
  }
  {
    // FF47 BGP
    /*#p36.PAVO*/ pix_pipe.PAVO_BGP_D0n.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_Dp_ext[0].qp_ext());
    /* p36.NUSY*/ pix_pipe.NUSY_BGP_D1n.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_Dp_ext[1].qp_ext());
    /* p36.PYLU*/ pix_pipe.PYLU_BGP_D2n.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_Dp_ext[2].qp_ext());
    /* p36.MAXY*/ pix_pipe.MAXY_BGP_D3n.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_Dp_ext[3].qp_ext());
    /* p36.MUKE*/ pix_pipe.MUKE_BGP_D4n.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_Dp_ext[4].qp_ext());
    /* p36.MORU*/ pix_pipe.MORU_BGP_D5n.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_Dp_ext[5].qp_ext());
    /* p36.MOGY*/ pix_pipe.MOGY_BGP_D6n.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_Dp_ext[6].qp_ext());
    /* p36.MENA*/ pix_pipe.MENA_BGP_D7n.dff8p(_TEPO_FF47_WRp_clk, BUS_CPU_Dp_ext[7].qp_ext());
  }
  {
    // FF48 OBP0
    /* p36.XUFU*/ pix_pipe.XUFU_OBP0_D0n.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_Dp_ext[0].qp_ext());
    /* p36.XUKY*/ pix_pipe.XUKY_OBP0_D1n.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_Dp_ext[1].qp_ext());
    /* p36.XOVA*/ pix_pipe.XOVA_OBP0_D2n.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_Dp_ext[2].qp_ext());
    /* p36.XALO*/ pix_pipe.XALO_OBP0_D3n.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_Dp_ext[3].qp_ext());
    /* p36.XERU*/ pix_pipe.XERU_OBP0_D4n.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_Dp_ext[4].qp_ext());
    /* p36.XYZE*/ pix_pipe.XYZE_OBP0_D5n.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_Dp_ext[5].qp_ext());
    /* p36.XUPO*/ pix_pipe.XUPO_OBP0_D6n.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_Dp_ext[6].qp_ext());
    /* p36.XANA*/ pix_pipe.XANA_OBP0_D7n.dff8p(_XELO_FF48_WRn_clk, BUS_CPU_Dp_ext[7].qp_ext());
  }
  {
    // FF49 OBP1
    /* p36.MOXY*/ pix_pipe.MOXY_OBP1_D0n.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_Dp_ext[0].qp_ext());
    /* p36.LAWO*/ pix_pipe.LAWO_OBP1_D1n.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_Dp_ext[1].qp_ext());
    /* p36.MOSA*/ pix_pipe.MOSA_OBP1_D2n.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_Dp_ext[2].qp_ext());
    /* p36.LOSE*/ pix_pipe.LOSE_OBP1_D3n.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_Dp_ext[3].qp_ext());
    /* p36.LUNE*/ pix_pipe.LUNE_OBP1_D4n.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_Dp_ext[4].qp_ext());
    /* p36.LUGU*/ pix_pipe.LUGU_OBP1_D5n.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_Dp_ext[5].qp_ext());
    /* p36.LEPU*/ pix_pipe.LEPU_OBP1_D6n.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_Dp_ext[6].qp_ext());
    /* p36.LUXO*/ pix_pipe.LUXO_OBP1_D7n.dff8p(_LEHO_FF49_WRn_clk, BUS_CPU_Dp_ext[7].qp_ext());
  }

#pragma endregion

  //----------------------------------------
  // Video clock signals

  /* p01.XEBE*/ wire _XEBE_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);
  /* p01.XODO*/ wire _XODO_VID_RSTp_new = nand2(_XEBE_SYS_RSTn_new, pix_pipe.XONA_LCDC_LCDENn.qn_new());
  /* p01.XAPO*/ wire _XAPO_VID_RSTn_new = not1(_XODO_VID_RSTp_new);
  /*#p01.ATAR*/ wire _ATAR_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
  /*#p01.ABEZ*/ wire _ABEZ_VID_RSTn_new = not1(_ATAR_VID_RSTp_new);
  /* p01.LYHA*/ wire _LYHA_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
  /* p01.LYFE*/ wire _LYFE_VID_RSTn_new = not1(_LYHA_VID_RSTp_new);
  /* p01.AMYG*/ wire _AMYG_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
  /* p01.TOFU*/ wire _TOFU_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);

  {
    /* p29.XYVA*/ wire _XYVA_xBxDxFxH_new = not1(_ZEME_AxCxExGx_clk);
    /* p29.XOTA*/ wire _XOTA_AxCxExGx_new = not1(_XYVA_xBxDxFxH_new);
    /* p29.XYFY*/ wire _XYFY_xBxDxFxH_new = not1(_XOTA_AxCxExGx_new);
    /* p29.WOSU*/ clk_reg.WOSU_AxxDExxHp.dff17(_XYFY_xBxDxFxH_new,              _XAPO_VID_RSTn_new, clk_reg.WUVU_ABxxEFxxp.qn_old());
    /* p29.WUVU*/ clk_reg.WUVU_ABxxEFxxp.dff17(_XOTA_AxCxExGx_new,              _XAPO_VID_RSTn_new, clk_reg.WUVU_ABxxEFxxp.qn_old());
    /* p21.VENA*/ clk_reg.VENA_xxCDEFxxp.dff17(clk_reg.WUVU_ABxxEFxxp.qn_new(), _XAPO_VID_RSTn_new, clk_reg.VENA_xxCDEFxxp.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
  }
  /*#p29.XOCE*/ wire _XOCE_xBCxxFGx_clknew = not1(clk_reg.WOSU_AxxDExxHp.qp_new());
  /*#p29.XUPY*/ wire _XUPY_ABxxEFxx_clknew = not1(clk_reg.WUVU_ABxxEFxxp.qn_new());
  /*#p21.TALU*/ wire _TALU_xxCDEFxx_clknew = not1(clk_reg.VENA_xxCDEFxxp.qn_new());

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region LYC_Matcher
  {
    /* p01.WESY*/ wire _WESY_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);

    // LYC matcher
    /* p21.RYME*/ wire _RYME_LY_MATCH0n_old = xor2(lcd_reg.MUWY_LY0p.qp_old(), lcd_reg.SYRY_LYC0n.qn_old());
    /* p21.TYDE*/ wire _TYDE_LY_MATCH1n_old = xor2(lcd_reg.MYRO_LY1p.qp_old(), lcd_reg.VUCE_LYC1n.qn_old());
    /* p21.REDA*/ wire _REDA_LY_MATCH2n_old = xor2(lcd_reg.LEXA_LY2p.qp_old(), lcd_reg.SEDY_LYC2n.qn_old());
    /* p21.RASY*/ wire _RASY_LY_MATCH3n_old = xor2(lcd_reg.LYDO_LY3p.qp_old(), lcd_reg.SALO_LYC3n.qn_old());
    /* p21.TYKU*/ wire _TYKU_LY_MATCH4n_old = xor2(lcd_reg.LOVU_LY4p.qp_old(), lcd_reg.SOTA_LYC4n.qn_old());
    /* p21.TUCY*/ wire _TUCY_LY_MATCH5n_old = xor2(lcd_reg.LEMA_LY5p.qp_old(), lcd_reg.VAFA_LYC5n.qn_old());
    /* p21.TERY*/ wire _TERY_LY_MATCH6n_old = xor2(lcd_reg.MATO_LY6p.qp_old(), lcd_reg.VEVO_LYC6n.qn_old());
    /* p21.SYFU*/ wire _SYFU_LY_MATCH7n_old = xor2(lcd_reg.LAFO_LY7p.qp_old(), lcd_reg.RAHA_LYC7n.qn_old());

    /* p21.SOVU*/ wire _SOVU_LY_MATCHA_old = nor4 (_SYFU_LY_MATCH7n_old, _TERY_LY_MATCH6n_old, _TUCY_LY_MATCH5n_old, _TYKU_LY_MATCH4n_old); // def nor4
    /* p21.SUBO*/ wire _SUBO_LY_MATCHB_old = nor4 (_RASY_LY_MATCH3n_old, _REDA_LY_MATCH2n_old, _TYDE_LY_MATCH1n_old, _RYME_LY_MATCH0n_old); // def nor4
    /* p21.RAPE*/ wire _RAPE_LY_MATCHn_old = nand2(_SOVU_LY_MATCHA_old, _SUBO_LY_MATCHB_old); // def nand2
    /* p21.PALY*/ wire _PALY_LY_MATCHa_old = not1(_RAPE_LY_MATCHn_old); // def not

    /*#p21.ROPO*/ lcd_reg.ROPO_LY_MATCH_SYNCp.dff17(_TALU_xxCDEFxx_clknew,  _WESY_SYS_RSTn_new, _PALY_LY_MATCHa_old);

    // FF45 LYC
    /* p23.SYRY*/ lcd_reg.SYRY_LYC0n.dff9(_WANE_FF45_WRp_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p23.VUCE*/ lcd_reg.VUCE_LYC1n.dff9(_WANE_FF45_WRp_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p23.SEDY*/ lcd_reg.SEDY_LYC2n.dff9(_WANE_FF45_WRp_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[2].qp_ext());
    /* p23.SALO*/ lcd_reg.SALO_LYC3n.dff9(_WANE_FF45_WRp_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p23.SOTA*/ lcd_reg.SOTA_LYC4n.dff9(_WANE_FF45_WRp_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p23.VAFA*/ lcd_reg.VAFA_LYC5n.dff9(_WANE_FF45_WRp_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p23.VEVO*/ lcd_reg.VEVO_LYC6n.dff9(_WANE_FF45_WRp_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[6].qp_ext());
    /* p23.RAHA*/ lcd_reg.RAHA_LYC7n.dff9(_WANE_FF45_WRp_clk, _WESY_SYS_RSTn_new, BUS_CPU_Dp_ext[7].qp_ext());

    // This latch isn't actually used as a latch, it's effectively an inverter.
    /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST_new = or2(_WESY_SYS_RSTn_new, _RYJU_FF41_WRn_clk);
    /* p21.RUPO*/ pix_pipe.RUPO_LYC_MATCH_LATCHn.nor_latch(_PAGO_LYC_MATCH_RST_new, lcd_reg.ROPO_LY_MATCH_SYNCp.qp_new());
  }
#pragma endregion

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region DMA_Registers


  {
    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn_old = nand6(dma_reg.NAKY_DMA_A00p.qp_old(), dma_reg.PYRO_DMA_A01p.qp_old(),  // 128+16+8+4+2+1 = 159
                                                   dma_reg.NEFY_DMA_A02p.qp_old(), dma_reg.MUTY_DMA_A03p.qp_old(),
                                                   dma_reg.NYKO_DMA_A04p.qp_old(), dma_reg.MUGU_DMA_A07p.qp_old());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp_old = not1(_NAVO_DMA_DONEn_old);
    /*#p04.LUPA*/ wire _LUPA_DMA_TRIG_old = nor2(_LAVY_FF46_WRp_clk, dma_reg.LYXE_DMA_LATCHp.qn_old());
    /*#p04.MATU*/ dma_reg.MATU_DMA_RUNNINGp.dff17(_UVYT_ABCDxxxx_clk, _CUNU_SYS_RSTn_new, dma_reg.LOKY_DMA_LATCHp.qp_old());

    /*#p04.LORU*/ wire _LORU_FF46_WRn_new = not1(_LAVY_FF46_WRp_clk);
    /* p04.MOPA*/ wire _MOPA_xxxxEFGH_new = not1(_UVYT_ABCDxxxx_clk);
    /*#p04.LENE*/ dma_reg.LENE_DMA_TRIG_d4.dff17(_MOPA_xxxxEFGH_new, _CUNU_SYS_RSTn_new, dma_reg.LUVY_DMA_TRIG_d0.qp_old());
    /*#p04.LUVY*/ dma_reg.LUVY_DMA_TRIG_d0.dff17(_UVYT_ABCDxxxx_clk, _CUNU_SYS_RSTn_new, _LUPA_DMA_TRIG_old);

    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp_new = nand2(dma_reg.LENE_DMA_TRIG_d4.qn_new(), _CUNU_SYS_RSTn_new);
    /*#p04.LYXE*/ dma_reg.LYXE_DMA_LATCHp.nor_latch(_LAVY_FF46_WRp_clk, _LOKO_DMA_RSTp_new);

    /*#p04.LAPA*/ wire _LAPA_DMA_RSTn_new = not1(_LOKO_DMA_RSTp_new);
    /*#p04.MYTE*/ dma_reg.MYTE_DMA_DONE.dff17(_MOPA_xxxxEFGH_new, _LAPA_DMA_RSTn_new, _NOLO_DMA_DONEp_old);

    /* p04.LARA*/ dma_reg.LARA_DMA_LATCHn = nand3(dma_reg.LOKY_DMA_LATCHp.qp_old(), dma_reg.MYTE_DMA_DONE.qn_new(), _CUNU_SYS_RSTn_new);
    /*#p04.LOKY*/ dma_reg.LOKY_DMA_LATCHp = nand2(dma_reg.LARA_DMA_LATCHn.qp_new(), dma_reg.LENE_DMA_TRIG_d4.qn_new());
    /* p04.LARA*/ dma_reg.LARA_DMA_LATCHn = nand3(dma_reg.LOKY_DMA_LATCHp.qp_new(), dma_reg.MYTE_DMA_DONE.qn_new(), _CUNU_SYS_RSTn_new);

    /*#p04.META*/ wire _META_DMA_CLKp_new = and2(_UVYT_ABCDxxxx_clk, dma_reg.LOKY_DMA_LATCHp.qp_new());
    /*#p04.NAKY*/ dma_reg.NAKY_DMA_A00p.dff17(_META_DMA_CLKp_new,             _LAPA_DMA_RSTn_new, dma_reg.NAKY_DMA_A00p.qn_old());
    /*#p04.PYRO*/ dma_reg.PYRO_DMA_A01p.dff17(dma_reg.NAKY_DMA_A00p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.PYRO_DMA_A01p.qn_old());
    /* p04.NEFY*/ dma_reg.NEFY_DMA_A02p.dff17(dma_reg.PYRO_DMA_A01p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.NEFY_DMA_A02p.qn_old());
    /* p04.MUTY*/ dma_reg.MUTY_DMA_A03p.dff17(dma_reg.NEFY_DMA_A02p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.MUTY_DMA_A03p.qn_old());
    /* p04.NYKO*/ dma_reg.NYKO_DMA_A04p.dff17(dma_reg.MUTY_DMA_A03p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.NYKO_DMA_A04p.qn_old());
    /* p04.PYLO*/ dma_reg.PYLO_DMA_A05p.dff17(dma_reg.NYKO_DMA_A04p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.PYLO_DMA_A05p.qn_old());
    /* p04.NUTO*/ dma_reg.NUTO_DMA_A06p.dff17(dma_reg.PYLO_DMA_A05p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.NUTO_DMA_A06p.qn_old());
    /* p04.MUGU*/ dma_reg.MUGU_DMA_A07p.dff17(dma_reg.NUTO_DMA_A06p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.MUGU_DMA_A07p.qn_old());

    /*#p04.NAFA*/ dma_reg.NAFA_DMA_A08n.dff8p(_LORU_FF46_WRn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p04.PYNE*/ dma_reg.PYNE_DMA_A09n.dff8p(_LORU_FF46_WRn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p04.PARA*/ dma_reg.PARA_DMA_A10n.dff8p(_LORU_FF46_WRn_new, BUS_CPU_Dp_ext[2].qp_ext());
    /* p04.NYDO*/ dma_reg.NYDO_DMA_A11n.dff8p(_LORU_FF46_WRn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p04.NYGY*/ dma_reg.NYGY_DMA_A12n.dff8p(_LORU_FF46_WRn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p04.PULA*/ dma_reg.PULA_DMA_A13n.dff8p(_LORU_FF46_WRn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p04.POKU*/ dma_reg.POKU_DMA_A14n.dff8p(_LORU_FF46_WRn_new, BUS_CPU_Dp_ext[6].qp_ext());
    /* p04.MARU*/ dma_reg.MARU_DMA_A15n.dff8p(_LORU_FF46_WRn_new, BUS_CPU_Dp_ext[7].qp_ext());

  }
  /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_new = not1(dma_reg.MATU_DMA_RUNNINGp.qp_new());
  /*#p04.LEBU*/ wire _LEBU_DMA_A15n_new     = not1(dma_reg.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_new    = nor3(dma_reg.PULA_DMA_A13n.qn_new(), dma_reg.POKU_DMA_A14n.qn_new(), _LEBU_DMA_A15n_new);
  /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_new    = nand2(dma_reg.MATU_DMA_RUNNINGp.qp_new(), _MUDA_DMA_VRAMp_new);
  /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_new    = not1(_MUHO_DMA_VRAMp_new);
  /* p04.LOGO*/ wire _LOGO_DMA_VRAMn_new    = not1(_MUDA_DMA_VRAMp_new);
  /* p04.MORY*/ wire _MORY_DMA_CARTn_new    = nand2(dma_reg.MATU_DMA_RUNNINGp.qp_new(), _LOGO_DMA_VRAMn_new);
  /* p04.LUMA*/ wire _LUMA_DMA_CARTp_new    = not1(_MORY_DMA_CARTn_new);

#pragma endregion




#pragma region LCD_Regs

  {
    /*#p21.SANU*/ wire _SANU_x113p_old = and4(lcd_reg.TYRY_LX6p.qp_old(), lcd_reg.TAHA_LX5p.qp_old(), lcd_reg.SUDE_LX4p.qp_old(), lcd_reg.SAXO_LX0p.qp_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*#p21.NOKO*/ wire _NOKO_y153p_old = and4(lcd_reg.LAFO_LY7p.qp_old(), lcd_reg.LOVU_LY4p.qp_old(), lcd_reg.LYDO_LY3p.qp_old(), lcd_reg.MUWY_LY0p.qp_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*#p21.XYVO*/ wire _XYVO_y144p_old = and2(lcd_reg.LOVU_LY4p.qp_old(), lcd_reg.LAFO_LY7p.qp_old()); // 128 + 16 = 144
    /*#p29.ALES*/ wire _ALES_y144n_old = not1(_XYVO_y144p_old);
    /*#p21.PURE*/ wire _PURE_LINE_ENDn_old = not1(lcd_reg.RUTU_x113p.qp_old());
    /*#p21.SELA*/ wire _SELA_LINE_P908p_old = not1(_PURE_LINE_ENDn_old);
    /*#p29.ABOV*/ wire _ABOV_LINE_P908p_old = and2(_SELA_LINE_P908p_old, _ALES_y144n_old);
    /*#p21.SONO*/ wire _SONO_ABxxxxGH_clknew = not1(_TALU_xxCDEFxx_clknew);

    /*#p21.NYPE*/ lcd_reg.NYPE_x113p.dff17(_TALU_xxCDEFxx_clknew,          _LYFE_VID_RSTn_new, lcd_reg.RUTU_x113p.qp_old());
    /*#p21.RUTU*/ lcd_reg.RUTU_x113p.dff17(_SONO_ABxxxxGH_clknew,          _LYFE_VID_RSTn_new, _SANU_x113p_old);
    /*#p21.MYTA*/ lcd_reg.MYTA_y153p.dff17(lcd_reg.NYPE_x113p.qn_new(), _LYFE_VID_RSTn_new, _NOKO_y153p_old);

    /*#p28.AWOH*/ wire _AWOH_xxCDxxGH_clknew = not1(_XUPY_ABxxEFxx_clknew);
    /*#p28.ANEL*/ lcd_reg.ANEL_LINE_P002p .dff17(_AWOH_xxCDxxGH_clknew,          _ABEZ_VID_RSTn_new, lcd_reg.CATU_LINE_P000p.qp_old());
    /*#p29.CATU*/ lcd_reg.CATU_LINE_P000p .dff17(_XUPY_ABxxEFxx_clknew,          _ABEZ_VID_RSTn_new, _ABOV_LINE_P908p_old);
    /*#p21.POPU*/ lcd_reg.POPU_VBLANK_CLKp.dff17(lcd_reg.NYPE_x113p.qp_new(), _LYFE_VID_RSTn_new, _XYVO_y144p_old);
  }

  /* p28.ABAF*/ wire _ABAF_LINE_P000n_new = not1(lcd_reg.CATU_LINE_P000p.qp_new());
  /* p28.BYHA*/ wire _BYHA_LINE_TRIGn_new = or_and3(lcd_reg.ANEL_LINE_P002p.qp_new(), _ABAF_LINE_P000n_new, _ABEZ_VID_RSTn_new); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire _ATEJ_LINE_TRIGp_new = not1(_BYHA_LINE_TRIGn_new);
  /* p21.TADY*/ wire _TADY_LINE_RSTn_new  = nor2(_ATEJ_LINE_TRIGp_new, _TOFU_VID_RSTp_new);

  //----------------------------------------

#pragma endregion


  /*#p29.AVAP*/ bool _AVAP_SCAN_DONE_TRIGp_new;

  [this, _ATAR_VID_RSTp_new, _ALET_xBxDxFxH_clk, _XUPY_ABxxEFxx_clknew, _BYHA_LINE_TRIGn_new, &_AVAP_SCAN_DONE_TRIGp_new]() {
    /* p28.ATEJ*/ wire _ATEJ_LINE_TRIGp_new = not1(_BYHA_LINE_TRIGn_new);
    /*#p28.ANOM*/ wire _ANOM_LINE_RSTn_new  = nor2(_ATEJ_LINE_TRIGp_new, _ATAR_VID_RSTp_new);

    /*#p28.FETO*/ wire _FETO_SCAN_DONEp_old = and4(sprite_scanner.YFEL_SCAN0.qp_old(), sprite_scanner.WEWY_SCAN1.qp_old(), sprite_scanner.GOSO_SCAN2.qp_old(), sprite_scanner.FONY_SCAN5.qp_old()); // 32 + 4 + 2 + 1 = 39
    /*#p29.BALU*/ wire _BALU_LINE_RSTp_new  = not1(_ANOM_LINE_RSTn_new);
    /*#p29.BAGY*/ wire _BAGY_LINE_RSTn_new  = not1(_BALU_LINE_RSTp_new);
    /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(_ALET_xBxDxFxH_clk, _BAGY_LINE_RSTn_new, sprite_scanner.BYBA_SCAN_DONE_Ap.qp_old());
    /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(_XUPY_ABxxEFxx_clknew, _BAGY_LINE_RSTn_new, _FETO_SCAN_DONEp_old);

    /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.FONY*/ sprite_scanner.FONY_SCAN5.RSTn(_ANOM_LINE_RSTn_new);

    /*#p28.FETO*/ wire _FETO_SCAN_DONEp_new = and4(sprite_scanner.YFEL_SCAN0.qp_new(), sprite_scanner.WEWY_SCAN1.qp_new(), sprite_scanner.GOSO_SCAN2.qp_new(), sprite_scanner.FONY_SCAN5.qp_new()); // 32 + 4 + 2 + 1 = 39
    /* p28.GAVA*/ wire _GAVA_ABxxEFxx_new = or2(_FETO_SCAN_DONEp_new, _XUPY_ABxxEFxx_clknew);

    /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0.dff17(_GAVA_ABxxEFxx_new,                 _ANOM_LINE_RSTn_new, sprite_scanner.YFEL_SCAN0.qn_new());
    /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1.dff17(sprite_scanner.YFEL_SCAN0.qn_new(), _ANOM_LINE_RSTn_new, sprite_scanner.WEWY_SCAN1.qn_new());
    /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2.dff17(sprite_scanner.WEWY_SCAN1.qn_new(), _ANOM_LINE_RSTn_new, sprite_scanner.GOSO_SCAN2.qn_new());
    /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3.dff17(sprite_scanner.GOSO_SCAN2.qn_new(), _ANOM_LINE_RSTn_new, sprite_scanner.ELYN_SCAN3.qn_new());
    /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4.dff17(sprite_scanner.ELYN_SCAN3.qn_new(), _ANOM_LINE_RSTn_new, sprite_scanner.FAHA_SCAN4.qn_new());
    /* p28.FONY*/ sprite_scanner.FONY_SCAN5.dff17(sprite_scanner.FAHA_SCAN4.qn_new(), _ANOM_LINE_RSTn_new, sprite_scanner.FONY_SCAN5.qn_new());

    /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn_new = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), _BALU_LINE_RSTp_new, sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ _AVAP_SCAN_DONE_TRIGp_new = not1(_BEBU_SCAN_DONE_TRIGn_new);

    /*#p28.ASEN*/ wire _ASEN_SCAN_DONE_TRIGp_new = or2(_ATAR_VID_RSTp_new, _AVAP_SCAN_DONE_TRIGp_new);
    /*#p01.ABEZ*/ wire _ABEZ_VID_RSTn_new = not1(_ATAR_VID_RSTp_new);
    /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGp.dff17(_XUPY_ABxxEFxx_clknew, _ABEZ_VID_RSTn_new,  sprite_scanner.BESU_SCANNINGp.qp_old());
    /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd_reg.CATU_LINE_P000p.qp_new(), _ASEN_SCAN_DONE_TRIGp_new);
  }();














  {
    /*#p21.VOGA*/ pix_pipe.VOGA_HBLANKp.dff17(_ALET_xBxDxFxH_clk, _TADY_LINE_RSTn_new, WODU_HBLANKp.qp_old());
  }

  /* p29.TEPA*/ wire _TEPA_RENDERINGn_old = not1(pix_pipe.XYMU_RENDERINGn.qn_old());

  {
    /*#p21.WEGO*/ wire _WEGO_HBLANKp_new = or2(_TOFU_VID_RSTp_new, pix_pipe.VOGA_HBLANKp.qp_new());
    /*#p21.XYMU*/ pix_pipe.XYMU_RENDERINGn.nor_latch(_WEGO_HBLANKp_new, _AVAP_SCAN_DONE_TRIGp_new);
  }




  /* p27.NUNU*/ pix_pipe.NUNU_WX_MATCH_Bp.dff17(_MEHE_AxCxExGx_clk, _XAPO_VID_RSTn_new, pix_pipe.PYCO_WX_MATCH_Ap.qp_old());
  /* p27.XAHY*/ wire _XAHY_LINE_TRIGn_new = not1(_ATEJ_LINE_TRIGp_new);
  /*#p27.XOFO*/ wire _XOFO_WIN_RSTp_new   = nand3(pix_pipe.WYMO_LCDC_WINENn.qn_new(), _XAHY_LINE_TRIGn_new, _XAPO_VID_RSTn_new);




  /* p27.NOPA*/ pix_pipe.NOPA_WIN_MODE_Bp.dff17(_ALET_xBxDxFxH_clk, _XAPO_VID_RSTn_new, pix_pipe.PYNU_WIN_MODE_Ap.qp_old());
  /* p27.PYNU*/ pix_pipe.PYNU_WIN_MODE_Ap.nor_latch(pix_pipe.NUNU_WX_MATCH_Bp.qp_new(), _XOFO_WIN_RSTp_new);
  /*#p27.NUNY*/ wire _NUNY_WX_MATCH_TRIGp_new = and2(pix_pipe.PYNU_WIN_MODE_Ap.qp_new(), pix_pipe.NOPA_WIN_MODE_Bp.qn_new());
  /* p27.NYFO*/ wire _NYFO_WX_MATCH_TRIGn_new = not1(_NUNY_WX_MATCH_TRIGp_new);
  /* p27.MOSU*/ wire _MOSU_WX_MATCH_TRIGp_new = not1(_NYFO_WX_MATCH_TRIGn_new);
  /* p24.LOBY*/ wire _LOBY_RENDERINGn_new = not1(pix_pipe.XYMU_RENDERINGn.qn_new());

  /* p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG_new = nor2(_MOSU_WX_MATCH_TRIGp_new, _LOBY_RENDERINGn_new);

  /* p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp.dff17(_ALET_xBxDxFxH_clk, pix_pipe.XYMU_RENDERINGn.qn_new(),  tile_fetcher.PORY_FETCH_DONEp.qp_old());

  /* p27.SOVY*/ pix_pipe.SOVY_WIN_FIRST_TILE_B.dff17(_ALET_xBxDxFxH_clk, _XAPO_VID_RSTn_new, pix_pipe.RYDY_WIN_HITp.qp_old());

  {
    /* p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp.dff17(_MYVO_AxCxExGx_clk, _NAFY_RENDERING_AND_NOT_WIN_TRIG_new, tile_fetcher.NYKA_FETCH_DONEp.qp_old());

    /* p01.PYRY*/ wire _PYRY_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
    /* p27.RYDY*/ pix_pipe.RYDY_WIN_HITp = nor3(pix_pipe.PUKU_WIN_HITn.qp_old(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), _PYRY_VID_RSTp_new);
    /* p27.PUKU*/ pix_pipe.PUKU_WIN_HITn = nor2(pix_pipe.RYDY_WIN_HITp.qp_new(), _NUNY_WX_MATCH_TRIGp_new);
    /* p27.RYDY*/ pix_pipe.RYDY_WIN_HITp = nor3(pix_pipe.PUKU_WIN_HITn.qp_new(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), _PYRY_VID_RSTp_new);
  }

  /* p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.qp_new(), _LOBY_RENDERINGn_new);




  /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn_new      = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp_new());


  /*#p27.SYLO*/ wire _SYLO_WIN_HITn_new = not1(pix_pipe.RYDY_WIN_HITp.qp_new());
  /*#p24.TOMU*/ wire _TOMU_WIN_HITp_new = not1(_SYLO_WIN_HITn_new);
  /*#p24.SOCY*/ wire _SOCY_WIN_HITn_new = not1(_TOMU_WIN_HITp_new);


  /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_new = nand2(_SYLO_WIN_HITn_new, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp_new());
  /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_new = not1(_TUXY_WIN_FIRST_TILEne_new);

  /* p27.RENE*/ pix_pipe.RENE_WIN_FETCHn_B.dff17(_ALET_xBxDxFxH_clk,         pix_pipe.XYMU_RENDERINGn.qn_new(), pix_pipe.RYFA_WIN_FETCHn_A.qp_old());

  /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17(_ALET_xBxDxFxH_clk, _NAFY_RENDERING_AND_NOT_WIN_TRIG_new, LYRY_BFETCH_DONEp.qp_old());

  /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn_new = nand4(pix_pipe.XYMU_RENDERINGn.qn_new(), _ROMO_PRELOAD_DONEn_new, tile_fetcher.NYKA_FETCH_DONEp.qp_new(), tile_fetcher.PORY_FETCH_DONEp.qp_new());
  /* p27.TAVE*/ wire _TAVE_PRELOAD_DONE_TRIGp_new = not1(_SUVU_PRELOAD_DONE_TRIGn_new);
  /* p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0p_D1.dff17(_ALET_xBxDxFxH_clk, pix_pipe.XYMU_RENDERINGn.qn_new(), tile_fetcher.LAXU_BFETCH_S0p.qp_old());
  /*#p27.NYZE*/ pix_pipe.NYZE_SCX_FINE_MATCH_B.dff17(_MOXE_AxCxExGx_clk,  pix_pipe.XYMU_RENDERINGn.qn_new(), pix_pipe.PUXA_SCX_FINE_MATCH_A.qp_old());

  // FIXME old/new - but does it really matter here?
  /*#p24.VYBO*/ wire _VYBO_CLKPIPE_xBxDxFxH_clknew = nor3(FEPO_STORE_MATCHp.qp_old(), WODU_HBLANKp.qp_old(), _MYVO_AxCxExGx_clk);
  /*#p24.TYFA*/ wire _TYFA_CLKPIPE_xBxDxFxH_clknew = and3(_SOCY_WIN_HITn_new, tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), _VYBO_CLKPIPE_xBxDxFxH_clknew);
  /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx_clknew = not1(_TYFA_CLKPIPE_xBxDxFxH_clknew);

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, _XORE_SYS_RSTp_new, _XAPO_VID_RSTn_new, _SEGU_CLKPIPE_AxCxExGx_clknew, _VYCU_FF4B_RDn_ext, _VOXU_FF4B_WRn_clk]() {
    /*#p27.MYLO*/ wire _MYLO_WX_MATCH0p_old = xnor2(pix_pipe.XEHO_PX0p.qp_old(), pix_pipe.MYPA_WX0n.qn_old());
    /* p27.PUWU*/ wire _PUWU_WX_MATCH1p_old = xnor2(pix_pipe.SAVY_PX1p.qp_old(), pix_pipe.NOFE_WX1n.qn_old());
    /* p27.PUHO*/ wire _PUHO_WX_MATCH2p_old = xnor2(pix_pipe.XODU_PX2p.qp_old(), pix_pipe.NOKE_WX2n.qn_old());
    /* p27.NYTU*/ wire _NYTU_WX_MATCH3p_old = xnor2(pix_pipe.XYDO_PX3p.qp_old(), pix_pipe.MEBY_WX3n.qn_old());
    /* p27.NEZO*/ wire _NEZO_WX_MATCH4p_old = xnor2(pix_pipe.TUHU_PX4p.qp_old(), pix_pipe.MYPU_WX4n.qn_old());
    /* p27.NORY*/ wire _NORY_WX_MATCH5p_old = xnor2(pix_pipe.TUKY_PX5p.qp_old(), pix_pipe.MYCE_WX5n.qn_old());
    /* p27.NONO*/ wire _NONO_WX_MATCH6p_old = xnor2(pix_pipe.TAKO_PX6p.qp_old(), pix_pipe.MUVO_WX6n.qn_old());
    /* p27.PASE*/ wire _PASE_WX_MATCH7p_old = xnor2(pix_pipe.SYBE_PX7p.qp_old(), pix_pipe.NUKU_WX7n.qn_old());
    /* p27.PUKY*/ wire _PUKY_WX_MATCHn_old  = nand5(pix_pipe.REJO_WY_MATCHp.qp_old(), _NEZO_WX_MATCH4p_old, _NORY_WX_MATCH5p_old, _NONO_WX_MATCH6p_old, _PASE_WX_MATCH7p_old);
    /* p27.NUFA*/ wire _NUFA_WX_MATCHp_old  = not1(_PUKY_WX_MATCHn_old);
    /* p27.NOGY*/ wire _NOGY_WX_MATCHn_old  = nand5(_NUFA_WX_MATCHp_old, _MYLO_WX_MATCH0p_old, _PUWU_WX_MATCH1p_old, _PUHO_WX_MATCH2p_old, _NYTU_WX_MATCH3p_old);
    /* p27.NUKO*/ wire _NUKO_WX_MATCHp_old  = not1(_NOGY_WX_MATCHn_old);

    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n_old = nand3(pix_pipe.RUBU_FINE_CNT2.qp_old(), pix_pipe.ROGA_FINE_CNT1.qp_old(), pix_pipe.RYKU_FINE_CNT0.qp_old());
    /* p27.PANY*/ wire _PANY_WIN_FETCHn_old = nor2(_NUKO_WX_MATCHp_old, _ROZE_FINE_COUNT_7n_old);

    /* p27.ROCO*/ wire _ROCO_CLKPIPE_xBxDxFxH_clknew = not1(_SEGU_CLKPIPE_AxCxExGx_clknew);
    /* p27.PYCO*/ pix_pipe.PYCO_WX_MATCH_Ap.dff17(_ROCO_CLKPIPE_xBxDxFxH_clknew, _XAPO_VID_RSTn_new, _NUKO_WX_MATCHp_old);
    /* p27.RYFA*/ pix_pipe.RYFA_WIN_FETCHn_A.dff17(_SEGU_CLKPIPE_AxCxExGx_clknew, pix_pipe.XYMU_RENDERINGn.qn_new(), _PANY_WIN_FETCHn_old);

    // FF4B WX
    /*#p01.WALU*/ wire _WALU_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);
    /* p23.MYPA*/ pix_pipe.MYPA_WX0n.dff9(_VOXU_FF4B_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p23.NOFE*/ pix_pipe.NOFE_WX1n.dff9(_VOXU_FF4B_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p23.NOKE*/ pix_pipe.NOKE_WX2n.dff9(_VOXU_FF4B_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[2].qp_ext());
    /* p23.MEBY*/ pix_pipe.MEBY_WX3n.dff9(_VOXU_FF4B_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p23.MYPU*/ pix_pipe.MYPU_WX4n.dff9(_VOXU_FF4B_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p23.MYCE*/ pix_pipe.MYCE_WX5n.dff9(_VOXU_FF4B_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p23.MUVO*/ pix_pipe.MUVO_WX6n.dff9(_VOXU_FF4B_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[6].qp_ext());
    /* p23.NUKU*/ pix_pipe.NUKU_WX7n.dff9(_VOXU_FF4B_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[7].qp_ext());

    /*#p23.LOVA*/ BUS_CPU_Dp_out[0].tri6_nn(_VYCU_FF4B_RDn_ext, pix_pipe.MYPA_WX0n.qp_new());
    /* p23.MUKA*/ BUS_CPU_Dp_out[1].tri6_nn(_VYCU_FF4B_RDn_ext, pix_pipe.NOFE_WX1n.qp_new());
    /* p23.MOKO*/ BUS_CPU_Dp_out[2].tri6_nn(_VYCU_FF4B_RDn_ext, pix_pipe.NOKE_WX2n.qp_new());
    /* p23.LOLE*/ BUS_CPU_Dp_out[3].tri6_nn(_VYCU_FF4B_RDn_ext, pix_pipe.MEBY_WX3n.qp_new());
    /* p23.MELE*/ BUS_CPU_Dp_out[4].tri6_nn(_VYCU_FF4B_RDn_ext, pix_pipe.MYPU_WX4n.qp_new());
    /* p23.MUFE*/ BUS_CPU_Dp_out[5].tri6_nn(_VYCU_FF4B_RDn_ext, pix_pipe.MYCE_WX5n.qp_new());
    /* p23.MULY*/ BUS_CPU_Dp_out[6].tri6_nn(_VYCU_FF4B_RDn_ext, pix_pipe.MUVO_WX6n.qp_new());
    /* p23.MARA*/ BUS_CPU_Dp_out[7].tri6_nn(_VYCU_FF4B_RDn_ext, pix_pipe.NUKU_WX7n.qp_new());
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp_new = nor2(pix_pipe.RYFA_WIN_FETCHn_A.qn_new(), pix_pipe.RENE_WIN_FETCHn_B.qp_new());
  /* p27.TEVO*/ wire _TEVO_FETCH_TRIGp_new = or3(_SEKO_WIN_FETCH_TRIGp_new, _SUZU_WIN_FIRST_TILEne_new, _TAVE_PRELOAD_DONE_TRIGp_new); // Schematic wrong, this is OR
  /* p27.NYXU*/ wire _NYXU_FETCH_TRIGn_new = nor3(_AVAP_SCAN_DONE_TRIGp_new, _MOSU_WX_MATCH_TRIGp_new, _TEVO_FETCH_TRIGp_new);

  {
    /* p27.LAXU*/ tile_fetcher.LAXU_BFETCH_S0p.RSTn(_NYXU_FETCH_TRIGn_new);
    /* p27.MESU*/ tile_fetcher.MESU_BFETCH_S1p.RSTn(_NYXU_FETCH_TRIGn_new);
    /* p27.NYVA*/ tile_fetcher.NYVA_BFETCH_S2p.RSTn(_NYXU_FETCH_TRIGn_new);

    /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn_mid = nand3(tile_fetcher.LAXU_BFETCH_S0p.qp_new(), tile_fetcher.NYVA_BFETCH_S2p.qp_new(), _NYXU_FETCH_TRIGn_new);
    /* p27.LEBO*/ wire _LEBO_AxCxExGx_clkmid = nand2(_ALET_xBxDxFxH_clk, _MOCE_BFETCH_DONEn_mid);

    /* p27.LAXU*/ tile_fetcher.LAXU_BFETCH_S0p.dff17(_LEBO_AxCxExGx_clkmid,                 _NYXU_FETCH_TRIGn_new, tile_fetcher.LAXU_BFETCH_S0p.qn_new());
    /* p27.MESU*/ tile_fetcher.MESU_BFETCH_S1p.dff17(tile_fetcher.LAXU_BFETCH_S0p.qn_new(), _NYXU_FETCH_TRIGn_new, tile_fetcher.MESU_BFETCH_S1p.qn_new());
    /* p27.NYVA*/ tile_fetcher.NYVA_BFETCH_S2p.dff17(tile_fetcher.MESU_BFETCH_S1p.qn_new(), _NYXU_FETCH_TRIGn_new, tile_fetcher.NYVA_BFETCH_S2p.qn_new());
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


  [this, _SEGU_CLKPIPE_AxCxExGx_clknew, _TEVO_FETCH_TRIGp_new]() {
    /*#p24.ROXO*/ wire _ROXO_CLKPIPE_xBxDxFxH_clknew = not1(_SEGU_CLKPIPE_AxCxExGx_clknew);

    /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp_old = xnor2(pix_pipe.DATY_SCX0n.qn_old(), pix_pipe.RYKU_FINE_CNT0.qp_old());
    /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp_old = xnor2(pix_pipe.DUZU_SCX1n.qn_old(), pix_pipe.ROGA_FINE_CNT1.qp_old());
    /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp_old = xnor2(pix_pipe.CYXU_SCX2n.qn_old(), pix_pipe.RUBU_FINE_CNT2.qp_old());
    /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn_old = nand4(pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.qp_old(), _SUHA_SCX_FINE_MATCHp_old, _SYBY_SCX_FINE_MATCHp_old, _SOZU_SCX_FINE_MATCHp_old);
    /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp_old = not1(_RONE_SCX_FINE_MATCHn_old);
    /*#p27.PUXA*/ pix_pipe.PUXA_SCX_FINE_MATCH_A.dff17(_ROXO_CLKPIPE_xBxDxFxH_clknew, pix_pipe.XYMU_RENDERINGn.qn_new(), _POHU_SCX_FINE_MATCHp_old);

    /*#p27.PAHA*/ wire _PAHA_RENDERINGn_new = not1(pix_pipe.XYMU_RENDERINGn.qn_new());
    /*#p27.PASO*/ wire _PASO_FINE_RST_new = nor2(_PAHA_RENDERINGn_new, _TEVO_FETCH_TRIGp_new);
    /*#p27.RYKU*/ pix_pipe.RYKU_FINE_CNT0.RSTn(_PASO_FINE_RST_new);
    /*#p27.ROGA*/ pix_pipe.ROGA_FINE_CNT1.RSTn(_PASO_FINE_RST_new);
    /*#p27.RUBU*/ pix_pipe.RUBU_FINE_CNT2.RSTn(_PASO_FINE_RST_new);

    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n_new = nand3(pix_pipe.RUBU_FINE_CNT2.qp_new(), pix_pipe.ROGA_FINE_CNT1.qp_new(), pix_pipe.RYKU_FINE_CNT0.qp_new());
    /*#p27.PECU*/ wire _PECU_FINE_CLK_AxCxExGx_clknew = nand2(_ROXO_CLKPIPE_xBxDxFxH_clknew, _ROZE_FINE_COUNT_7n_new);
    /*#p27.RYKU*/ pix_pipe.RYKU_FINE_CNT0.dff17(_PECU_FINE_CLK_AxCxExGx_clknew,   _PASO_FINE_RST_new, pix_pipe.RYKU_FINE_CNT0.qn_new());
    /*#p27.ROGA*/ pix_pipe.ROGA_FINE_CNT1.dff17(pix_pipe.RYKU_FINE_CNT0.qn_new(), _PASO_FINE_RST_new, pix_pipe.ROGA_FINE_CNT1.qn_new());
    /*#p27.RUBU*/ pix_pipe.RUBU_FINE_CNT2.dff17(pix_pipe.ROGA_FINE_CNT1.qn_new(), _PASO_FINE_RST_new, pix_pipe.RUBU_FINE_CNT2.qn_new());

    /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp_new = and2(pix_pipe.PUXA_SCX_FINE_MATCH_A.qp_new(), pix_pipe.NYZE_SCX_FINE_MATCH_B.qn_new());
    /*#p27.ROXY*/ pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.nor_latch(_PAHA_RENDERINGn_new, _POVA_FINE_MATCH_TRIGp_new);
    /* p24.PAHO*/ pix_pipe.PAHO_X_8_SYNC.dff17(_ROXO_CLKPIPE_xBxDxFxH_clknew, pix_pipe.XYMU_RENDERINGn.qn_new(), pix_pipe.XYDO_PX3p.qp_old());
  }();

  /*#p24.SACU*/ wire _SACU_CLKPIPE_AxCxExGx_clknew = or2(_SEGU_CLKPIPE_AxCxExGx_clknew, pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.qp_new());

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, _SACU_CLKPIPE_AxCxExGx_clknew, _TADY_LINE_RSTn_new]() {
    // Pixel counter, has carry lookahead because this can increment every tcycle
    /* p21.RYBO*/ wire _RYBO_old = xor2(pix_pipe.XEHO_PX0p.qp_old(), pix_pipe.SAVY_PX1p.qp_old()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /* p21.XUKE*/ wire _XUKE_old = and2(pix_pipe.XEHO_PX0p.qp_old(), pix_pipe.SAVY_PX1p.qp_old());
    /* p21.XYLE*/ wire _XYLE_old = and2(pix_pipe.XODU_PX2p.qp_old(), _XUKE_old);
    /* p21.XEGY*/ wire _XEGY_old = xor2(pix_pipe.XODU_PX2p.qp_old(), _XUKE_old); // feet facing gnd
    /* p21.XORA*/ wire _XORA_old = xor2(pix_pipe.XYDO_PX3p.qp_old(), _XYLE_old); // feet facing gnd

    /* p21.XEHO*/ pix_pipe.XEHO_PX0p.dff17(_SACU_CLKPIPE_AxCxExGx_clknew, _TADY_LINE_RSTn_new, pix_pipe.XEHO_PX0p.qn_old());
    /* p21.SAVY*/ pix_pipe.SAVY_PX1p.dff17(_SACU_CLKPIPE_AxCxExGx_clknew, _TADY_LINE_RSTn_new, _RYBO_old);
    /* p21.XODU*/ pix_pipe.XODU_PX2p.dff17(_SACU_CLKPIPE_AxCxExGx_clknew, _TADY_LINE_RSTn_new, _XEGY_old);
    /* p21.XYDO*/ pix_pipe.XYDO_PX3p.dff17(_SACU_CLKPIPE_AxCxExGx_clknew, _TADY_LINE_RSTn_new, _XORA_old);

    /* p24.TOCA*/ wire _TOCA_new = not1(pix_pipe.XYDO_PX3p.qp_new());
    /* p21.SAKE*/ wire _SAKE_old = xor2(pix_pipe.TUHU_PX4p.qp_old(), pix_pipe.TUKY_PX5p.qp_old());
    /* p21.TYBA*/ wire _TYBA_old = and2(pix_pipe.TUHU_PX4p.qp_old(), pix_pipe.TUKY_PX5p.qp_old());
    /* p21.SURY*/ wire _SURY_old = and2(pix_pipe.TAKO_PX6p.qp_old(), _TYBA_old);
    /* p21.TYGE*/ wire _TYGE_old = xor2(pix_pipe.TAKO_PX6p.qp_old(), _TYBA_old);
    /* p21.ROKU*/ wire _ROKU_old = xor2(pix_pipe.SYBE_PX7p.qp_old(), _SURY_old);

    /* p21.TUHU*/ pix_pipe.TUHU_PX4p.dff17(_TOCA_new, _TADY_LINE_RSTn_new, pix_pipe.TUHU_PX4p.qn_old());
    /* p21.TUKY*/ pix_pipe.TUKY_PX5p.dff17(_TOCA_new, _TADY_LINE_RSTn_new, _SAKE_old);
    /* p21.TAKO*/ pix_pipe.TAKO_PX6p.dff17(_TOCA_new, _TADY_LINE_RSTn_new, _TYGE_old);
    /* p21.SYBE*/ pix_pipe.SYBE_PX7p.dff17(_TOCA_new, _TADY_LINE_RSTn_new, _ROKU_old);
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, _XORE_SYS_RSTp_new, _XAPO_VID_RSTn_new, _TALU_xxCDEFxx_clknew, _WYMO_LCDC_WINENp_old, _VEFU_FF4A_WRn_clk, _VOMY_FF4A_RDn_ext]() {
    /*#p27.NAZE*/ wire _NAZE_WY_MATCH0p_old = xnor2(pix_pipe.NESO_WY0n.qn_old(), lcd_reg.MUWY_LY0p.qp_old());
    /* p27.PEBO*/ wire _PEBO_WY_MATCH1p_old = xnor2(pix_pipe.NYRO_WY1n.qn_old(), lcd_reg.MYRO_LY1p.qp_old());
    /* p27.POMO*/ wire _POMO_WY_MATCH2p_old = xnor2(pix_pipe.NAGA_WY2n.qn_old(), lcd_reg.LEXA_LY2p.qp_old());
    /* p27.NEVU*/ wire _NEVU_WY_MATCH3p_old = xnor2(pix_pipe.MELA_WY3n.qn_old(), lcd_reg.LYDO_LY3p.qp_old());
    /* p27.NOJO*/ wire _NOJO_WY_MATCH4p_old = xnor2(pix_pipe.NULO_WY4n.qn_old(), lcd_reg.LOVU_LY4p.qp_old());
    /* p27.PAGA*/ wire _PAGA_WY_MATCH5p_old = xnor2(pix_pipe.NENE_WY5n.qn_old(), lcd_reg.LEMA_LY5p.qp_old());
    /* p27.PEZO*/ wire _PEZO_WY_MATCH6p_old = xnor2(pix_pipe.NUKA_WY6n.qn_old(), lcd_reg.MATO_LY6p.qp_old());
    /* p27.NUPA*/ wire _NUPA_WY_MATCH7p_old = xnor2(pix_pipe.NAFU_WY7n.qn_old(), lcd_reg.LAFO_LY7p.qp_old());

    /*#p27.PALO*/ wire _PALO_WY_MATCH_HIn_old  = nand5(_WYMO_LCDC_WINENp_old, _NOJO_WY_MATCH4p_old, _PAGA_WY_MATCH5p_old, _PEZO_WY_MATCH6p_old, _NUPA_WY_MATCH7p_old);
    /* p27.NELE*/ wire _NELE_WY_MATCH_HIp_old  = not1(_PALO_WY_MATCH_HIn_old);
    /* p27.PAFU*/ wire _PAFU_WY_MATCHn_old     = nand5(_NELE_WY_MATCH_HIp_old, _NAZE_WY_MATCH0p_old, _PEBO_WY_MATCH1p_old, _POMO_WY_MATCH2p_old, _NEVU_WY_MATCH3p_old);
    /* p27.ROGE*/ wire _ROGE_WY_MATCHp_old     = not1(_PAFU_WY_MATCHn_old);

    /*#p01.WALU*/ wire _WALU_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);
    /* p23.NESO*/ pix_pipe.NESO_WY0n.dff9(_VEFU_FF4A_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p23.NYRO*/ pix_pipe.NYRO_WY1n.dff9(_VEFU_FF4A_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p23.NAGA*/ pix_pipe.NAGA_WY2n.dff9(_VEFU_FF4A_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[2].qp_ext());
    /* p23.MELA*/ pix_pipe.MELA_WY3n.dff9(_VEFU_FF4A_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p23.NULO*/ pix_pipe.NULO_WY4n.dff9(_VEFU_FF4A_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p23.NENE*/ pix_pipe.NENE_WY5n.dff9(_VEFU_FF4A_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p23.NUKA*/ pix_pipe.NUKA_WY6n.dff9(_VEFU_FF4A_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[6].qp_ext());
    /* p23.NAFU*/ pix_pipe.NAFU_WY7n.dff9(_VEFU_FF4A_WRn_clk, _WALU_SYS_RSTn_new, BUS_CPU_Dp_ext[7].qp_ext());

    /*#p23.PUNU*/ BUS_CPU_Dp_out[0].tri6_nn(_VOMY_FF4A_RDn_ext, pix_pipe.NESO_WY0n.qp_new());
    /* p23.PODA*/ BUS_CPU_Dp_out[1].tri6_nn(_VOMY_FF4A_RDn_ext, pix_pipe.NYRO_WY1n.qp_new());
    /* p23.PYGU*/ BUS_CPU_Dp_out[2].tri6_nn(_VOMY_FF4A_RDn_ext, pix_pipe.NAGA_WY2n.qp_new());
    /* p23.LOKA*/ BUS_CPU_Dp_out[3].tri6_nn(_VOMY_FF4A_RDn_ext, pix_pipe.MELA_WY3n.qp_new());
    /* p23.MEGA*/ BUS_CPU_Dp_out[4].tri6_nn(_VOMY_FF4A_RDn_ext, pix_pipe.NULO_WY4n.qp_new());
    /* p23.PELA*/ BUS_CPU_Dp_out[5].tri6_nn(_VOMY_FF4A_RDn_ext, pix_pipe.NENE_WY5n.qp_new());
    /* p23.POLO*/ BUS_CPU_Dp_out[6].tri6_nn(_VOMY_FF4A_RDn_ext, pix_pipe.NUKA_WY6n.qp_new());
    /* p23.MERA*/ BUS_CPU_Dp_out[7].tri6_nn(_VOMY_FF4A_RDn_ext, pix_pipe.NAFU_WY7n.qp_new());

    /* p01.PYRY*/ wire _PYRY_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
    /*#p21.PARU*/ wire _PARU_VBLANKp_new = not1(lcd_reg.POPU_VBLANK_CLKp.qn_new());
    /* p27.REPU*/ wire _REPU_VBLANKp_new = or2(_PARU_VBLANKp_new, _PYRY_VID_RSTp_new);

    /* p27.SARY*/ pix_pipe.SARY_WY_MATCHp.dff17(_TALU_xxCDEFxx_clknew, _XAPO_VID_RSTn_new, _ROGE_WY_MATCHp_old);
    /* p27.REJO*/ pix_pipe.REJO_WY_MATCHp.nor_latch(pix_pipe.SARY_WY_MATCHp.qp_new(), _REPU_VBLANKp_new);

    // Every time we leave win mode we increment win_y
    /*#p27.NOCU*/ wire _NOCU_WIN_Y_CLKp = not1(pix_pipe.PYNU_WIN_MODE_Ap.qp_new());
    /* p27.PORE*/ wire _PORE_WIN_Y_CLKn = not1(_NOCU_WIN_Y_CLKp);
    /* p27.WAZY*/ wire _WAZY_WIN_Y_CLKp = not1(_PORE_WIN_Y_CLKn);
    /* p27.SYNY*/ wire _SYNY_VBLANK_RSTn_t3 = not1(_REPU_VBLANKp_new);
    /* p27.VYNO*/ pix_pipe.VYNO_WIN_Y0.dff17(_WAZY_WIN_Y_CLKp,              _SYNY_VBLANK_RSTn_t3, pix_pipe.VYNO_WIN_Y0.qn_old());
    /* p27.VUJO*/ pix_pipe.VUJO_WIN_Y1.dff17(pix_pipe.VYNO_WIN_Y0.qn_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.VUJO_WIN_Y1.qn_old());
    /* p27.VYMU*/ pix_pipe.VYMU_WIN_Y2.dff17(pix_pipe.VUJO_WIN_Y1.qn_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.VYMU_WIN_Y2.qn_old());
    /* p27.TUFU*/ pix_pipe.TUFU_WIN_Y3.dff17(pix_pipe.VYMU_WIN_Y2.qn_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TUFU_WIN_Y3.qn_old());
    /* p27.TAXA*/ pix_pipe.TAXA_WIN_Y4.dff17(pix_pipe.TUFU_WIN_Y3.qn_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TAXA_WIN_Y4.qn_old());
    /* p27.TOZO*/ pix_pipe.TOZO_WIN_Y5.dff17(pix_pipe.TAXA_WIN_Y4.qn_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TOZO_WIN_Y5.qn_old());
    /* p27.TATE*/ pix_pipe.TATE_WIN_Y6.dff17(pix_pipe.TOZO_WIN_Y5.qn_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TATE_WIN_Y6.qn_old());
    /* p27.TEKE*/ pix_pipe.TEKE_WIN_Y7.dff17(pix_pipe.TATE_WIN_Y6.qn_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TEKE_WIN_Y7.qn_old());
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, _LYHA_VID_RSTp_new, _TALU_xxCDEFxx_clknew, _WAFU_FF44_RDp_ext]() {
    /*#p21.MUDE*/ wire _MUDE_X_RSTn_new = nor2(lcd_reg.RUTU_x113p.qp_new(), _LYHA_VID_RSTp_new);
    /*#p21.SAXO*/ lcd_reg.SAXO_LX0p.dff17(_TALU_xxCDEFxx_clknew,        _MUDE_X_RSTn_new, lcd_reg.SAXO_LX0p.qn_old());
    /*#p21.TYPO*/ lcd_reg.TYPO_LX1p.dff17(lcd_reg.SAXO_LX0p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.TYPO_LX1p.qn_old());
    /*#p21.VYZO*/ lcd_reg.VYZO_LX2p.dff17(lcd_reg.TYPO_LX1p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.VYZO_LX2p.qn_old());
    /*#p21.TELU*/ lcd_reg.TELU_LX3p.dff17(lcd_reg.VYZO_LX2p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.TELU_LX3p.qn_old());
    /*#p21.SUDE*/ lcd_reg.SUDE_LX4p.dff17(lcd_reg.TELU_LX3p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.SUDE_LX4p.qn_old());
    /*#p21.TAHA*/ lcd_reg.TAHA_LX5p.dff17(lcd_reg.SUDE_LX4p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.TAHA_LX5p.qn_old());
    /*#p21.TYRY*/ lcd_reg.TYRY_LX6p.dff17(lcd_reg.TAHA_LX5p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.TYRY_LX6p.qn_old());

    /*#p21.LAMA*/ wire _LAMA_Y_RSTn_new = nor2(lcd_reg.MYTA_y153p.qp_new(), _LYHA_VID_RSTp_new);
    /*#p21.MUWY*/ lcd_reg.MUWY_LY0p.dff17(lcd_reg.RUTU_x113p.qp_new(),  _LAMA_Y_RSTn_new, lcd_reg.MUWY_LY0p.qn_old());
    /*#p21.MYRO*/ lcd_reg.MYRO_LY1p.dff17(lcd_reg.MUWY_LY0p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.MYRO_LY1p.qn_old());
    /*#p21.LEXA*/ lcd_reg.LEXA_LY2p.dff17(lcd_reg.MYRO_LY1p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LEXA_LY2p.qn_old());
    /*#p21.LYDO*/ lcd_reg.LYDO_LY3p.dff17(lcd_reg.LEXA_LY2p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LYDO_LY3p.qn_old());
    /*#p21.LOVU*/ lcd_reg.LOVU_LY4p.dff17(lcd_reg.LYDO_LY3p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LOVU_LY4p.qn_old());
    /*#p21.LEMA*/ lcd_reg.LEMA_LY5p.dff17(lcd_reg.LOVU_LY4p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LEMA_LY5p.qn_old());
    /*#p21.MATO*/ lcd_reg.MATO_LY6p.dff17(lcd_reg.LEMA_LY5p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.MATO_LY6p.qn_old());
    /*#p21.LAFO*/ lcd_reg.LAFO_LY7p.dff17(lcd_reg.MATO_LY6p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LAFO_LY7p.qn_old());

    /* FF44 LY */
    /*#p23.WURY*/ wire _WURY_LY0n_new = not1(lcd_reg.MUWY_LY0p.qp_new());
    /* p23.XEPO*/ wire _XEPO_LY1n_new = not1(lcd_reg.MYRO_LY1p.qp_new());
    /* p23.MYFA*/ wire _MYFA_LY2n_new = not1(lcd_reg.LEXA_LY2p.qp_new());
    /* p23.XUHY*/ wire _XUHY_LY3n_new = not1(lcd_reg.LYDO_LY3p.qp_new());
    /* p23.WATA*/ wire _WATA_LY4n_new = not1(lcd_reg.LOVU_LY4p.qp_new());
    /* p23.XAGA*/ wire _XAGA_LY5n_new = not1(lcd_reg.LEMA_LY5p.qp_new());
    /* p23.XUCE*/ wire _XUCE_LY6n_new = not1(lcd_reg.MATO_LY6p.qp_new());
    /* p23.XOWO*/ wire _XOWO_LY7n_new = not1(lcd_reg.LAFO_LY7p.qp_new());

    /* p23.VARO*/ wire _VARO_FF44_RDn_ext = not1(_WAFU_FF44_RDp_ext);
    /* p23.VEGA*/ BUS_CPU_Dp_out[0].tri6_nn(_VARO_FF44_RDn_ext, _WURY_LY0n_new);
    /* p23.WUVA*/ BUS_CPU_Dp_out[1].tri6_nn(_VARO_FF44_RDn_ext, _XEPO_LY1n_new);
    /* p23.LYCO*/ BUS_CPU_Dp_out[2].tri6_nn(_VARO_FF44_RDn_ext, _MYFA_LY2n_new);
    /* p23.WOJY*/ BUS_CPU_Dp_out[3].tri6_nn(_VARO_FF44_RDn_ext, _XUHY_LY3n_new);
    /* p23.VYNE*/ BUS_CPU_Dp_out[4].tri6_nn(_VARO_FF44_RDn_ext, _WATA_LY4n_new);
    /* p23.WAMA*/ BUS_CPU_Dp_out[5].tri6_nn(_VARO_FF44_RDn_ext, _XAGA_LY5n_new);
    /* p23.WAVO*/ BUS_CPU_Dp_out[6].tri6_nn(_VARO_FF44_RDn_ext, _XUCE_LY6n_new);
    /* p23.WEZE*/ BUS_CPU_Dp_out[7].tri6_nn(_VARO_FF44_RDn_ext, _XOWO_LY7n_new);
  }();










  /* p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp.dff17(_MYVO_AxCxExGx_clk, _NYXU_FETCH_TRIGn_new, LYRY_BFETCH_DONEp.qp_old());
  /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn_new = and2(tile_fetcher.LOVY_FETCH_DONEp.qn_new(), pix_pipe.XYMU_RENDERINGn.qn_new());
  /* p27.LONY*/ tile_fetcher.LONY_FETCHINGp.nand_latch(_NYXU_FETCH_TRIGn_new, _LURY_BG_FETCH_DONEn_new);

  /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn_new = nand3(tile_fetcher.LAXU_BFETCH_S0p.qp_new(), tile_fetcher.NYVA_BFETCH_S2p.qp_new(), _NYXU_FETCH_TRIGn_new);
  /* p27.LYRY*/ LYRY_BFETCH_DONEp.set(not1(_MOCE_BFETCH_DONEn_new));














































































































  /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY_old = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp_old(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_old());



  bool _WUTY_SPRITE_DONEp_new;

  [this, _ALET_xBxDxFxH_clk, _VYPO_VCC, _XAPO_VID_RSTn_new, _ATEJ_LINE_TRIGp_new, _TAVE_PRELOAD_DONE_TRIGp_new, &_WUTY_SPRITE_DONEp_new]() {
    /* p01.LAPE*/ wire _LAPE_AxCxExGx_clk = not1(_ALET_xBxDxFxH_clk);
    /* p27.TAVA*/ wire _TAVA_xBxDxFxH_clk = not1(_LAPE_AxCxExGx_clk);
    /* p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp.dff17(_LAPE_AxCxExGx_clk, _VYPO_VCC, sprite_fetcher.SOBU_SFETCH_REQp.qp_old());
    /* p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp.dff17(_TAVA_xBxDxFxH_clk, _VYPO_VCC, TEKY_SFETCH_REQp.qp_old());


    /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1.dff17(_LAPE_AxCxExGx_clk, _VYPO_VCC,                         sprite_fetcher.TOXE_SFETCH_S0p.qp_old());
    /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5.dff17(_LAPE_AxCxExGx_clk, pix_pipe.XYMU_RENDERINGn.qn_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_old());
    /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4.dff17(_TAVA_xBxDxFxH_clk, pix_pipe.XYMU_RENDERINGn.qn_new(), sprite_fetcher.TOBU_SFETCH_S1p_D2.qp_old());
    /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2.dff17(_TAVA_xBxDxFxH_clk, pix_pipe.XYMU_RENDERINGn.qn_new(), sprite_fetcher.TULY_SFETCH_S1p.qp_old());

    /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp_new = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
    /* p01.ROSY*/ wire _ROSY_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
    /*#p27.SECA*/ wire _SECA_SFETCH_RUNNING_SETn_new = nor3(_RYCE_SFETCH_TRIGp_new, _ROSY_VID_RSTp_new, _ATEJ_LINE_TRIGp_new);
    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.RSTn(_SECA_SFETCH_RUNNING_SETn_new);
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p.RSTn(_SECA_SFETCH_RUNNING_SETn_new);
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p.RSTn(_SECA_SFETCH_RUNNING_SETn_new);

    /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE_new = nand2(sprite_fetcher.TESE_SFETCH_S2p.qp_new(), sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
    /*#p29.TOMA*/ wire _TOMA_SFETCH_xBxDxFxH_clknew = nand2(_LAPE_AxCxExGx_clk, _TAME_SFETCH_CLK_GATE_new);
    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.dff17(_TOMA_SFETCH_xBxDxFxH_clknew,            _SECA_SFETCH_RUNNING_SETn_new, sprite_fetcher.TOXE_SFETCH_S0p.qn_new());
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p.dff17(sprite_fetcher.TOXE_SFETCH_S0p.qn_new(), _SECA_SFETCH_RUNNING_SETn_new, sprite_fetcher.TULY_SFETCH_S1p.qn_new());
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p.dff17(sprite_fetcher.TULY_SFETCH_S1p.qn_new(), _SECA_SFETCH_RUNNING_SETn_new, sprite_fetcher.TESE_SFETCH_S2p.qn_new());

    /* p29.TYNO*/ wire _TYNO_new = nand3(sprite_fetcher.TOXE_SFETCH_S0p.qp_new(), sprite_fetcher.SEBA_SFETCH_S1p_D5.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn_new = or2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qn_new(), _TYNO_new);
    /* p29.WUTY*/ _WUTY_SPRITE_DONEp_new = not1(_VUSA_SPRITE_DONEn_new);
    /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn_new = nor2(_WUTY_SPRITE_DONEp_new, _TAVE_PRELOAD_DONE_TRIGp_new); // def nor
    /* p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RUNNING_SETn_new, _VEKU_SFETCH_RUNNING_RSTn_new);
  }();






  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, _XUPY_ABxxEFxx_clknew]() {
    /* p28.YFOT*/ wire _YFOT_OAM_A2p_old = not1(BUS_OAM_An[2].qp_old());
    /* p28.YFOC*/ wire _YFOC_OAM_A3p_old = not1(BUS_OAM_An[3].qp_old());
    /* p28.YVOM*/ wire _YVOM_OAM_A4p_old = not1(BUS_OAM_An[4].qp_old());
    /* p28.YMEV*/ wire _YMEV_OAM_A5p_old = not1(BUS_OAM_An[5].qp_old());
    /* p28.XEMU*/ wire _XEMU_OAM_A6p_old = not1(BUS_OAM_An[6].qp_old());
    /* p28.YZET*/ wire _YZET_OAM_A7p_old = not1(BUS_OAM_An[7].qp_old());

    /*#p30.CYKE*/ wire _CYKE_ABxxEFxx_new = not1(_XUPY_ABxxEFxx_clknew);
    /*#p30.WUDA*/ wire _WUDA_xxCDxxGH_new = not1(_CYKE_ABxxEFxx_new);
    wire _WEFE_VCC = 1;
    /* p30.XADU*/ sprite_scanner.XADU_SPRITE_IDX0p.dff13(_WUDA_xxCDxxGH_new, _WEFE_VCC, _YFOT_OAM_A2p_old);
    /* p30.XEDY*/ sprite_scanner.XEDY_SPRITE_IDX1p.dff13(_WUDA_xxCDxxGH_new, _WEFE_VCC, _YFOC_OAM_A3p_old);
    /* p30.ZUZE*/ sprite_scanner.ZUZE_SPRITE_IDX2p.dff13(_WUDA_xxCDxxGH_new, _WEFE_VCC, _YVOM_OAM_A4p_old);
    /* p30.XOBE*/ sprite_scanner.XOBE_SPRITE_IDX3p.dff13(_WUDA_xxCDxxGH_new, _WEFE_VCC, _YMEV_OAM_A5p_old);
    /* p30.YDUF*/ sprite_scanner.YDUF_SPRITE_IDX4p.dff13(_WUDA_xxCDxxGH_new, _WEFE_VCC, _XEMU_OAM_A6p_old);
    /* p30.XECU*/ sprite_scanner.XECU_SPRITE_IDX5p.dff13(_WUDA_xxCDxxGH_new, _WEFE_VCC, _YZET_OAM_A7p_old);
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  {
    /*#p27.NOCU*/ wire _NOCU_WIN_MODEn_new = not1(pix_pipe.PYNU_WIN_MODE_Ap.qp_new());
    /* p27.PORE*/ wire _PORE_WIN_MODEp_new = not1(_NOCU_WIN_MODEn_new);
    /* p27.VETU*/ wire _VETU_WIN_MAP_clknew = and2(_TEVO_FETCH_TRIGp_new, _PORE_WIN_MODEp_new);
    /* p27.XACO*/ wire _XACO_WIN_RSTn_new = not1(_XOFO_WIN_RSTp_new);
    /* p27.WYKA*/ pix_pipe.WYKA_WIN_X3.dff17(_VETU_WIN_MAP_clknew,          _XACO_WIN_RSTn_new, pix_pipe.WYKA_WIN_X3.qn_old());
    /* p27.WODY*/ pix_pipe.WODY_WIN_X4.dff17(pix_pipe.WYKA_WIN_X3.qn_new(), _XACO_WIN_RSTn_new, pix_pipe.WODY_WIN_X4.qn_old());
    /* p27.WOBO*/ pix_pipe.WOBO_WIN_X5.dff17(pix_pipe.WODY_WIN_X4.qn_new(), _XACO_WIN_RSTn_new, pix_pipe.WOBO_WIN_X5.qn_old());
    /* p27.WYKO*/ pix_pipe.WYKO_WIN_X6.dff17(pix_pipe.WOBO_WIN_X5.qn_new(), _XACO_WIN_RSTn_new, pix_pipe.WYKO_WIN_X6.qn_old());
    /* p27.XOLO*/ pix_pipe.XOLO_WIN_X7.dff17(pix_pipe.WYKO_WIN_X6.qn_new(), _XACO_WIN_RSTn_new, pix_pipe.XOLO_WIN_X7.qn_old());
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // OBL -> temp

  /* p29.TYSO*/ wire _TYSO_SPRITE_READn_old = or2(_SAKY_SFETCH_MYSTERY_old, _TEPA_RENDERINGn_old); // def or
  /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp_old = not1(_TYSO_SPRITE_READn_old);

  /*#p29.WOJO*/ wire _WOJO_AxxxExxxp_clk  = nor2(clk_reg.WOSU_AxxDExxHp.qn_new(), clk_reg.WUVU_ABxxEFxxp.qn_new());
  /* p29.XYSO*/ wire _XYSO_AxxxExxxn_clk  = not1(_WOJO_AxxxExxxp_clk);
  /*#p28.ACYL*/ wire _ACYL_SCANNINGp_new   = and2(_BOGE_DMA_RUNNINGn_new, sprite_scanner.BESU_SCANNINGp.qp_new());
  /* p25.AVER*/ wire _AVER_AxxxExxx_clknew   = nand2(_ACYL_SCANNINGp_new, _XYSO_AxxxExxxn_clk);
  /* p29.TYTU*/ wire _TYTU_SFETCH_S0n_new = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
  /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG_new = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new(), _TYTU_SFETCH_S0n_new);
  /* p29.TEPA*/ wire _TEPA_RENDERINGn_new = not1(pix_pipe.XYMU_RENDERINGn.qn_new());
  /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp_new = nor3(_TEPA_RENDERINGn_new, sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.TESE_SFETCH_S2p.qp_new());
  /* p25.VAPE*/ wire _VAPE_OAM_CLKENn_new = and2(_TUVO_PPU_OAM_RDp_new, _TACU_SPR_SEQ_5_TRIG_new);
  /* p25.XUJY*/ wire _XUJY_OAM_CLKENp_new = not1(_VAPE_OAM_CLKENn_new);
  /* p25.CUFE*/ wire _CUFE_OAM_CLKENp_clknew = not1(or_and3(_SARO_FE00_FEFFp_ext, dma_reg.MATU_DMA_RUNNINGp.qp_new(), _MOPA_xxxxEFGH_clk)); // CUFE looks like BYHA minus an inverter
  /* p25.BYCU*/ wire _BYCU_xBCDxFGH_clknew   = nand3(_AVER_AxxxExxx_clknew, _XUJY_OAM_CLKENp_new, _CUFE_OAM_CLKENp_clknew);
  /* p25.COTA*/ wire _COTA_AxxxxExxx_clknew  = not1(_BYCU_xBCDxFGH_clknew);
  /* p28.WEFY*/ wire _WEFY_SPR_READp_new = and2(_TUVO_PPU_OAM_RDp_new, sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new());
  /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn_new  = not1(_WEFY_SPR_READp_new);
  /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn_new = nand2(_ACYL_SCANNINGp_new, _XOCE_xBCxxFGx_clknew); // schematic wrong, is def nand2
  /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn_new = not1(_CATY_LATCH_EXTp_ext);
  /*#p28.BOTA*/ wire _BOTA_OAM_OEn_new  = nand3(_BOFE_LATCH_EXTn_new, _SARO_FE00_FEFFp_ext, _ASOT_CPU_RDp_ext); // Schematic wrong, this is NAND
  /*#p28.ASYT*/ wire _ASYT_OAM_OEn_new = and3(_AJEP_SCAN_OAM_LATCHn_new, _XUJA_SPR_OAM_LATCHn_new, _BOTA_OAM_OEn_new); // def and
  /*#p28.BODE*/ wire _BODE_OAM_OEp_new = not1(_ASYT_OAM_OEn_new);
  /*#p28.YVAL*/ wire _YVAL_OAM_OEn_new = not1(_BODE_OAM_OEp_new);
  /*#p28.YRYV*/ wire _YRYU_OAM_OEp_new = not1(_YVAL_OAM_OEn_new);
  /*#p28.ZODO*/ wire _ZODO_OAM_OEn_new = not1(_YRYU_OAM_OEp_new);

  PIN_OAM_OEn.pin_in(1, _ZODO_OAM_OEn_new);

  BUS_OAM_DAn_in[0].reset();
  BUS_OAM_DAn_in[1].reset();
  BUS_OAM_DAn_in[2].reset();
  BUS_OAM_DAn_in[3].reset();
  BUS_OAM_DAn_in[4].reset();
  BUS_OAM_DAn_in[5].reset();
  BUS_OAM_DAn_in[6].reset();
  BUS_OAM_DAn_in[7].reset();

  BUS_OAM_DBn_in[0].reset();
  BUS_OAM_DBn_in[1].reset();
  BUS_OAM_DBn_in[2].reset();
  BUS_OAM_DBn_in[3].reset();
  BUS_OAM_DBn_in[4].reset();
  BUS_OAM_DBn_in[5].reset();
  BUS_OAM_DBn_in[6].reset();
  BUS_OAM_DBn_in[7].reset();

  BUS_OAM_DAn_in[0].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_a & 0x01));
  BUS_OAM_DAn_in[1].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_a & 0x02));
  BUS_OAM_DAn_in[2].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_a & 0x04));
  BUS_OAM_DAn_in[3].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_a & 0x08));
  BUS_OAM_DAn_in[4].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_a & 0x10));
  BUS_OAM_DAn_in[5].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_a & 0x20));
  BUS_OAM_DAn_in[6].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_a & 0x40));
  BUS_OAM_DAn_in[7].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_a & 0x80));

  BUS_OAM_DBn_in[0].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_b & 0x01));
  BUS_OAM_DBn_in[1].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_b & 0x02));
  BUS_OAM_DBn_in[2].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_b & 0x04));
  BUS_OAM_DBn_in[3].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_b & 0x08));
  BUS_OAM_DBn_in[4].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_b & 0x10));
  BUS_OAM_DBn_in[5].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_b & 0x20));
  BUS_OAM_DBn_in[6].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_b & 0x40));
  BUS_OAM_DBn_in[7].tri6_nn(PIN_OAM_OEn.qp_ext(), (oam_data_latch_b & 0x80));


  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


#pragma region TileData

  {
    /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n_new = not1(tile_fetcher.LAXU_BFETCH_S0p.qp_new());
    /*#p27.MYSO*/ wire _MYSO_BG_TRIGp_new = nor3(_LOBY_RENDERINGn_new, _LAXE_BFETCH_S0n_new, tile_fetcher.LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

    /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n_new = not1(tile_fetcher.NYVA_BFETCH_S2p.qp_new());
    /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn_new = nand3(_MYSO_BG_TRIGp_new, tile_fetcher.MESU_BFETCH_S1p.qp_new(), _NOFU_BFETCH_S2n_new); // NYDY on fetch phase 6
    /*#p32.METE*/ wire _METE_LATCH_TILE_DAp_new = not1(_NYDY_LATCH_TILE_DAn_new);
    /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn_new = not1(_METE_LATCH_TILE_DAp_new);
    /* p32.LEGU*/ vram_bus.LEGU_TILE_DA0n.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp_in[0].qp_old());
    /* p32.NUDU*/ vram_bus.NUDU_TILE_DA1n.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp_in[1].qp_old());
    /* p32.MUKU*/ vram_bus.MUKU_TILE_DA2n.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp_in[2].qp_old());
    /* p32.LUZO*/ vram_bus.LUZO_TILE_DA3n.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp_in[3].qp_old());
    /* p32.MEGU*/ vram_bus.MEGU_TILE_DA4n.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp_in[4].qp_old());
    /* p32.MYJY*/ vram_bus.MYJY_TILE_DA5n.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp_in[5].qp_old());
    /* p32.NASA*/ vram_bus.NASA_TILE_DA6n.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp_in[6].qp_old());
    /* p32.NEFO*/ vram_bus.NEFO_TILE_DA7n.dff8p(_LOMA_LATCH_TILE_DAn_new, BUS_VRAM_Dp_in[7].qp_old());

    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n_new = not1(tile_fetcher.MESU_BFETCH_S1p.qp_new());
    /* p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp_new = and2(_MYSO_BG_TRIGp_new, _NAKO_BFETCH_S1n_new); // MOFU fires on fetch phase 2 and 10
    /* p32.LESO*/ wire _LESO_LATCH_TILE_DBn_new = not1(_MOFU_LATCH_TILE_DBp_new);
    /* p??.LUVE*/ wire _LUVE_LATCH_TILE_DBp_new = not1(_LESO_LATCH_TILE_DBn_new); // Schematic wrong, was labeled AJAR
    /* p32.LABU*/ wire _LABU_LATCH_TILE_DBn_new = not1(_LUVE_LATCH_TILE_DBp_new);
    /* p32.RAWU*/ vram_bus.RAWU_TILE_DB0p.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp_in[0].qp_old());
    /* p32.POZO*/ vram_bus.POZO_TILE_DB1p.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp_in[1].qp_old());
    /* p32.PYZO*/ vram_bus.PYZO_TILE_DB2p.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp_in[2].qp_old());
    /* p32.POXA*/ vram_bus.POXA_TILE_DB3p.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp_in[3].qp_old());
    /* p32.PULO*/ vram_bus.PULO_TILE_DB4p.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp_in[4].qp_old());
    /* p32.POJU*/ vram_bus.POJU_TILE_DB5p.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp_in[5].qp_old());
    /* p32.POWY*/ vram_bus.POWY_TILE_DB6p.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp_in[6].qp_old());
    /* p32.PYJU*/ vram_bus.PYJU_TILE_DB7p.dff11(_LABU_LATCH_TILE_DBn_new, _VYPO_VCC, BUS_VRAM_Dp_in[7].qp_old());
  }

#pragma endregion

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  /* p29.DEZY*/ sprite_store.DEZY_STORE_ENn.dff17(_ZEME_AxCxExGx_clk, _XAPO_VID_RSTn_new, DYTY_STORE_ENn.qp_old());

  {
    // Sprite store counter. The sprite count clock stops ticking once we have 10 sprites.
    /*#p28.AZYB*/ wire _AZYB_LINE_TRIGn = not1(_ATEJ_LINE_TRIGp_new);
    /* p29.BESE*/ sprite_store.BESE_SPRITE_COUNT0.RSTn(_AZYB_LINE_TRIGn);
    /* p29.CUXY*/ sprite_store.CUXY_SPRITE_COUNT1.RSTn(_AZYB_LINE_TRIGn);
    /* p29.BEGO*/ sprite_store.BEGO_SPRITE_COUNT2.RSTn(_AZYB_LINE_TRIGn);
    /* p29.DYBE*/ sprite_store.DYBE_SPRITE_COUNT3.RSTn(_AZYB_LINE_TRIGn);
    /*#p29.BAKY*/ wire _BAKY_SPRITES_FULL_new = and2(sprite_store.CUXY_SPRITE_COUNT1.qp_new(), sprite_store.DYBE_SPRITE_COUNT3.qp_new());
    /*#p29.CAKE*/ wire _CAKE_clknew = or2(_BAKY_SPRITES_FULL_new, sprite_store.DEZY_STORE_ENn.qp_new());
    /* p29.BESE*/ sprite_store.BESE_SPRITE_COUNT0.dff17(_CAKE_clknew,                             _AZYB_LINE_TRIGn, sprite_store.BESE_SPRITE_COUNT0.qn_new());
    /* p29.CUXY*/ sprite_store.CUXY_SPRITE_COUNT1.dff17(sprite_store.BESE_SPRITE_COUNT0.qn_new(), _AZYB_LINE_TRIGn, sprite_store.CUXY_SPRITE_COUNT1.qn_new());
    /* p29.BEGO*/ sprite_store.BEGO_SPRITE_COUNT2.dff17(sprite_store.CUXY_SPRITE_COUNT1.qn_new(), _AZYB_LINE_TRIGn, sprite_store.BEGO_SPRITE_COUNT2.qn_new());
    /* p29.DYBE*/ sprite_store.DYBE_SPRITE_COUNT3.dff17(sprite_store.BEGO_SPRITE_COUNT2.qn_new(), _AZYB_LINE_TRIGn, sprite_store.DYBE_SPRITE_COUNT3.qn_new());
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  /*#p31.ZAGO*/ wire _ZAGO_SPX0n_old = not1(oam_bus.YLOR_OAM_DB0p.qp_old());
  /* p31.ZOCY*/ wire _ZOCY_SPX1n_old = not1(oam_bus.ZYTY_OAM_DB1p.qp_old());
  /* p31.YPUR*/ wire _YPUR_SPX2n_old = not1(oam_bus.ZYVE_OAM_DB2p.qp_old());
  /* p31.YVOK*/ wire _YVOK_SPX3n_old = not1(oam_bus.ZEZY_OAM_DB3p.qp_old());
  /* p31.COSE*/ wire _COSE_SPX4n_old = not1(oam_bus.GOMO_OAM_DB4p.qp_old());
  /* p31.AROP*/ wire _AROP_SPX5n_old = not1(oam_bus.BAXO_OAM_DB5p.qp_old());
  /* p31.XATU*/ wire _XATU_SPX6n_old = not1(oam_bus.YZOS_OAM_DB6p.qp_old());
  /* p31.BADY*/ wire _BADY_SPX7n_old = not1(oam_bus.DEPO_OAM_DB7p.qp_old());
  /*#p29.XONO*/ wire _XONO_FLIP_X_old = and2(oam_bus.BAXO_OAM_DB5p.qp_old(), _TEXY_SPR_READ_VRAMp_old);


  wire PIN_OAM_CLKn_old = PIN_OAM_CLKn.qp_old();
  PIN_OAM_CLKn.pin_in(1, _COTA_AxxxxExxx_clknew);

  {
    /* p29.YWOK*/ wire _YWOK_xBCDxFGH_clknew    = not1(_COTA_AxxxxExxx_clknew);
    /*#p29.XUSO*/ oam_bus.XUSO_OAM_DA0p.dff8n(_YWOK_xBCDxFGH_clknew, oam_bus.YDYV_OAM_LATCH_DA0n.qp_old());
    /* p29.XEGU*/ oam_bus.XEGU_OAM_DA1p.dff8n(_YWOK_xBCDxFGH_clknew, oam_bus.YCEB_OAM_LATCH_DA1n.qp_old());
    /* p29.YJEX*/ oam_bus.YJEX_OAM_DA2p.dff8n(_YWOK_xBCDxFGH_clknew, oam_bus.ZUCA_OAM_LATCH_DA2n.qp_old());
    /* p29.XYJU*/ oam_bus.XYJU_OAM_DA3p.dff8n(_YWOK_xBCDxFGH_clknew, oam_bus.WONE_OAM_LATCH_DA3n.qp_old());
    /* p29.YBOG*/ oam_bus.YBOG_OAM_DA4p.dff8n(_YWOK_xBCDxFGH_clknew, oam_bus.ZAXE_OAM_LATCH_DA4n.qp_old());
    /* p29.WYSO*/ oam_bus.WYSO_OAM_DA5p.dff8n(_YWOK_xBCDxFGH_clknew, oam_bus.XAFU_OAM_LATCH_DA5n.qp_old());
    /* p29.XOTE*/ oam_bus.XOTE_OAM_DA6p.dff8n(_YWOK_xBCDxFGH_clknew, oam_bus.YSES_OAM_LATCH_DA6n.qp_old());
    /* p29.YZAB*/ oam_bus.YZAB_OAM_DA7p.dff8n(_YWOK_xBCDxFGH_clknew, oam_bus.ZECA_OAM_LATCH_DA7n.qp_old());

    /* p31.XEGA*/ wire _XEGA_xBCDxFGH_clknew    = not1(_COTA_AxxxxExxx_clknew);
    /* p31.YLOR*/ oam_bus.YLOR_OAM_DB0p.dff8n(_XEGA_xBCDxFGH_clknew, oam_bus.XYKY_OAM_LATCH_DB0n.qp_old());
    /* p31.ZYTY*/ oam_bus.ZYTY_OAM_DB1p.dff8n(_XEGA_xBCDxFGH_clknew, oam_bus.YRUM_OAM_LATCH_DB1n.qp_old());
    /* p31.ZYVE*/ oam_bus.ZYVE_OAM_DB2p.dff8n(_XEGA_xBCDxFGH_clknew, oam_bus.YSEX_OAM_LATCH_DB2n.qp_old());
    /* p31.ZEZY*/ oam_bus.ZEZY_OAM_DB3p.dff8n(_XEGA_xBCDxFGH_clknew, oam_bus.YVEL_OAM_LATCH_DB3n.qp_old());
    /* p31.GOMO*/ oam_bus.GOMO_OAM_DB4p.dff8n(_XEGA_xBCDxFGH_clknew, oam_bus.WYNO_OAM_LATCH_DB4n.qp_old());
    /* p31.BAXO*/ oam_bus.BAXO_OAM_DB5p.dff8n(_XEGA_xBCDxFGH_clknew, oam_bus.CYRA_OAM_LATCH_DB5n.qp_old());
    /* p31.YZOS*/ oam_bus.YZOS_OAM_DB6p.dff8n(_XEGA_xBCDxFGH_clknew, oam_bus.ZUVE_OAM_LATCH_DB6n.qp_old());
    /* p31.DEPO*/ oam_bus.DEPO_OAM_DB7p.dff8n(_XEGA_xBCDxFGH_clknew, oam_bus.ECED_OAM_LATCH_DB7n.qp_old());

  }

  //----------------------------------------
  // Sprite counter

  /* p29.CEHA*/ wire _CEHA_SCANNINGp_new = not1(sprite_scanner.CENO_SCANNINGp.qn_new());
  /*#p29.BYJO*/ wire _BYJO_SCANNINGn_new = not1(_CEHA_SCANNINGp_new);

  {
    // this is using an adder as a subtracter by inverting the first input.

    /*#p29.EBOS*/ wire _EBOS_LY0n_new = not1(lcd_reg.MUWY_LY0p.qp_new());
    /* p29.DASA*/ wire _DASA_LY1n_new = not1(lcd_reg.MYRO_LY1p.qp_new());
    /* p29.FUKY*/ wire _FUKY_LY2n_new = not1(lcd_reg.LEXA_LY2p.qp_new());
    /* p29.FUVE*/ wire _FUVE_LY3n_new = not1(lcd_reg.LYDO_LY3p.qp_new());
    /* p29.FEPU*/ wire _FEPU_LY4n_new = not1(lcd_reg.LOVU_LY4p.qp_new());
    /* p29.FOFA*/ wire _FOFA_LY5n_new = not1(lcd_reg.LEMA_LY5p.qp_new());
    /* p29.FEMO*/ wire _FEMO_LY6n_new = not1(lcd_reg.MATO_LY6p.qp_new());
    /* p29.GUSU*/ wire _GUSU_LY7n_new = not1(lcd_reg.LAFO_LY7p.qp_new());

    ///* p29.ERUC*/ wire _ERUC_YDIFF_S0_new = add_s(_EBOS_LY0n_new, oam_bus.XUSO_OAM_DA0p.qp_new(), GND);
    /* p29.ERUC*/ wire _ERUC_YDIFF_C0_new = add_c(_EBOS_LY0n_new, oam_bus.XUSO_OAM_DA0p.qp_new(), GND);
    ///* p29.ENEF*/ wire _ENEF_YDIFF_S1_new = add_s(_DASA_LY1n_new, oam_bus.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
    /* p29.ENEF*/ wire _ENEF_YDIFF_C1_new = add_c(_DASA_LY1n_new, oam_bus.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
    ///* p29.FECO*/ wire _FECO_YDIFF_S2_new = add_s(_FUKY_LY2n_new, oam_bus.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
    /* p29.FECO*/ wire _FECO_YDIFF_C2_new = add_c(_FUKY_LY2n_new, oam_bus.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
    /* p29.GYKY*/ wire _GYKY_YDIFF_S3_new = add_s(_FUVE_LY3n_new, oam_bus.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF_C2_new);
    /* p29.GYKY*/ wire _GYKY_YDIFF_C3_new = add_c(_FUVE_LY3n_new, oam_bus.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF_C2_new);
    /* p29.GOPU*/ wire _GOPU_YDIFF_S4_new = add_s(_FEPU_LY4n_new, oam_bus.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF_C3_new);
    /* p29.GOPU*/ wire _GOPU_YDIFF_C4_new = add_c(_FEPU_LY4n_new, oam_bus.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF_C3_new);
    /* p29.FUWA*/ wire _FUWA_YDIFF_S5_new = add_s(_FOFA_LY5n_new, oam_bus.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF_C4_new);
    /* p29.FUWA*/ wire _FUWA_YDIFF_C5_new = add_c(_FOFA_LY5n_new, oam_bus.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF_C4_new);
    /* p29.GOJU*/ wire _GOJU_YDIFF_S6_new = add_s(_FEMO_LY6n_new, oam_bus.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF_C5_new);
    /* p29.GOJU*/ wire _GOJU_YDIFF_C6_new = add_c(_FEMO_LY6n_new, oam_bus.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF_C5_new);
    /* p29.WUHU*/ wire _WUHU_YDIFF_S7_new = add_s(_GUSU_LY7n_new, oam_bus.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF_C6_new);
    /* p29.WUHU*/ wire _WUHU_YDIFF_C7_new = add_c(_GUSU_LY7n_new, oam_bus.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF_C6_new);

    /* p29.GACE*/ wire _GACE_SPRITE_DELTA4_new = not1(_GOPU_YDIFF_S4_new);
    /* p29.GUVU*/ wire _GUVU_SPRITE_DELTA5_new = not1(_FUWA_YDIFF_S5_new);
    /* p29.GYDA*/ wire _GYDA_SPRITE_DELTA6_new = not1(_GOJU_YDIFF_S6_new);
    /* p29.GEWY*/ wire _GEWY_SPRITE_DELTA7_new = not1(_WUHU_YDIFF_S7_new);
    /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH_new = or2(pix_pipe.XYMO_LCDC_SPSIZEn.qn_new(), _GYKY_YDIFF_S3_new);

    /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn_new = nand6(_GACE_SPRITE_DELTA4_new, _GUVU_SPRITE_DELTA5_new, _GYDA_SPRITE_DELTA6_new, _GEWY_SPRITE_DELTA7_new, _WUHU_YDIFF_C7_new, _GOVU_SPSIZE_MATCH_new);
    /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp_new = not1(_WOTA_SCAN_MATCH_Yn_new);
    /* p29.CARE*/ wire _CARE_STORE_ENp_ABxxEFxx_new = and3(_XOCE_xBCxxFGx_clknew, _CEHA_SCANNINGp_new, _GESE_SCAN_MATCH_Yp_new); // Dots on VCC, this is AND. Die shot and schematic wrong.

    /* p29.DYTY*/ DYTY_STORE_ENn.set(not1(_CARE_STORE_ENp_ABxxEFxx_new));
  }

  //----------------------------------------
  // 10 sprite stores

  /* p29.EDEN*/ wire _EDEN_SPRITE_COUNT0n_new = not1(sprite_store.BESE_SPRITE_COUNT0.qp_new());
  /* p29.CYPY*/ wire _CYPY_SPRITE_COUNT1n_new = not1(sprite_store.CUXY_SPRITE_COUNT1.qp_new());
  /* p29.CAPE*/ wire _CAPE_SPRITE_COUNT2n_new = not1(sprite_store.BEGO_SPRITE_COUNT2.qp_new());
  /* p29.CAXU*/ wire _CAXU_SPRITE_COUNT3n_new = not1(sprite_store.DYBE_SPRITE_COUNT3.qp_new());

  /* p29.FYCU*/ wire _FYCU_SPRITE_COUNT0p_new = not1(_EDEN_SPRITE_COUNT0n_new);
  /* p29.FONE*/ wire _FONE_SPRITE_COUNT1p_new = not1(_CYPY_SPRITE_COUNT1n_new);
  /* p29.EKUD*/ wire _EKUD_SPRITE_COUNT2p_new = not1(_CAPE_SPRITE_COUNT2n_new);
  /* p29.ELYG*/ wire _ELYG_SPRITE_COUNT3p_new = not1(_CAXU_SPRITE_COUNT3n_new);

  /* p29.DEZO*/ wire _DEZO_STORE0_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.CUVA*/ wire _CUVA_STORE1_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.GEBU*/ wire _GEBU_STORE2_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _FONE_SPRITE_COUNT1p_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.FOCO*/ wire _FOCO_STORE3_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _FONE_SPRITE_COUNT1p_new, _CAPE_SPRITE_COUNT2n_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.CUPE*/ wire _CUPE_STORE4_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _CYPY_SPRITE_COUNT1n_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.CUGU*/ wire _CUGU_STORE5_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _CYPY_SPRITE_COUNT1n_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.WOMU*/ wire _WOMU_STORE6_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _FONE_SPRITE_COUNT1p_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.GUNA*/ wire _GUNA_STORE7_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _FONE_SPRITE_COUNT1p_new, _EKUD_SPRITE_COUNT2p_new, _CAXU_SPRITE_COUNT3n_new);
  /* p29.DEWY*/ wire _DEWY_STORE8_SELn_new = nand4(_EDEN_SPRITE_COUNT0n_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _ELYG_SPRITE_COUNT3p_new);
  /* p29.DOGU*/ wire _DOGU_STORE9_SELn_new = nand4(_FYCU_SPRITE_COUNT0p_new, _CYPY_SPRITE_COUNT1n_new, _CAPE_SPRITE_COUNT2n_new, _ELYG_SPRITE_COUNT3p_new);

  /* p29.CEMY*/ wire _CEMY_STORE0_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _DEZO_STORE0_SELn_new);
  /* p29.BYBY*/ wire _BYBY_STORE1_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _CUVA_STORE1_SELn_new);
  /* p29.WYXO*/ wire _WYXO_STORE2_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _GEBU_STORE2_SELn_new);
  /* p29.GUVE*/ wire _GUVE_STORE3_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _FOCO_STORE3_SELn_new);
  /* p29.CECU*/ wire _CECU_STORE4_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _CUPE_STORE4_SELn_new);
  /* p29.CADO*/ wire _CADO_STORE5_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _CUGU_STORE5_SELn_new);
  /* p29.XUJO*/ wire _XUJO_STORE6_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _WOMU_STORE6_SELn_new);
  /* p29.GAPE*/ wire _GAPE_STORE7_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _GUNA_STORE7_SELn_new);
  /* p29.CAHO*/ wire _CAHO_STORE8_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _DEWY_STORE8_SELn_new);
  /* p29.CATO*/ wire _CATO_STORE9_CLKp_new = or2(DYTY_STORE_ENn.qp_new(), _DOGU_STORE9_SELn_new);

  /* p29.DYHU*/ wire _DYHU_STORE0_CLKn_new = not1(_CEMY_STORE0_CLKp_new);
  /* p29.BUCO*/ wire _BUCO_STORE1_CLKn_new = not1(_BYBY_STORE1_CLKp_new);
  /* p29.GYFO*/ wire _GYFO_STORE2_CLKn_new = not1(_WYXO_STORE2_CLKp_new);
  /* p29.GUSA*/ wire _GUSA_STORE3_CLKn_new = not1(_GUVE_STORE3_CLKp_new);
  /* p29.DUKE*/ wire _DUKE_STORE4_CLKn_new = not1(_CECU_STORE4_CLKp_new);
  /* p29.BEDE*/ wire _BEDE_STORE5_CLKn_new = not1(_CADO_STORE5_CLKp_new);
  /* p29.WEKA*/ wire _WEKA_STORE6_CLKn_new = not1(_XUJO_STORE6_CLKp_new);
  /* p29.GYVO*/ wire _GYVO_STORE7_CLKn_new = not1(_GAPE_STORE7_CLKp_new);
  /* p29.BUKA*/ wire _BUKA_STORE8_CLKn_new = not1(_CAHO_STORE8_CLKp_new);
  /* p29.DECU*/ wire _DECU_STORE9_CLKn_new = not1(_CATO_STORE9_CLKp_new);

  {
    /* p28.ABAK*/ wire _ABAK_LINE_RSTp_new =  or2(_ATEJ_LINE_TRIGp_new, _AMYG_VID_RSTp_new);
    /* p28.BYVA*/ wire _BYVA_LINE_RSTn_new = not1(_ABAK_LINE_RSTp_new);
    /* p29.EBOJ*/ sprite_store.EBOJ_STORE0_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, GUVA_SPRITE0_GETp.qp_old());
    /* p29.CEDY*/ sprite_store.CEDY_STORE1_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, ENUT_SPRITE1_GETp.qp_old());
    /* p29.EGAV*/ sprite_store.EGAV_STORE2_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, EMOL_SPRITE2_GETp.qp_old());
    /* p29.GOTA*/ sprite_store.GOTA_STORE3_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, GYFY_SPRITE3_GETp.qp_old());
    /* p29.XUDY*/ sprite_store.XUDY_STORE4_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, GONO_SPRITE4_GETp.qp_old());
    /* p29.WAFY*/ sprite_store.WAFY_STORE5_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, GEGA_SPRITE5_GETp.qp_old());
    /* p29.WOMY*/ sprite_store.WOMY_STORE6_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, XOJA_SPRITE6_GETp.qp_old());
    /* p29.WAPO*/ sprite_store.WAPO_STORE7_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, GUTU_SPRITE7_GETp.qp_old());
    /* p29.EXUQ*/ sprite_store.EXUQ_STORE8_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, FOXA_SPRITE8_GETp.qp_old());
    /* p29.FONO*/ sprite_store.FONO_STORE9_RSTp.dff17(_WUTY_SPRITE_DONEp_new, _BYVA_LINE_RSTn_new, GUZE_SPRITE9_GETp.qp_old());
  }

  {
    /* p29.GENY*/ wire _GENY_STORE0_CLKp_new = not1(_DYHU_STORE0_CLKn_new);
    /* p29.BYVY*/ wire _BYVY_STORE1_CLKp_new = not1(_BUCO_STORE1_CLKn_new);
    /* p29.BUZY*/ wire _BUZY_STORE2_CLKp_new = not1(_GYFO_STORE2_CLKn_new);
    /* p29.FEKA*/ wire _FEKA_STORE3_CLKp_new = not1(_GUSA_STORE3_CLKn_new);
    /* p29.WYLU*/ wire _WYLU_STORE4_CLKp_new = not1(_DUKE_STORE4_CLKn_new);
    /* p29.DYMO*/ wire _DYMO_STORE5_CLKp_new = not1(_BEDE_STORE5_CLKn_new);
    /* p29.WUSE*/ wire _WUSE_STORE6_CLKp_new = not1(_WEKA_STORE6_CLKn_new);
    /* p29.FEFO*/ wire _FEFO_STORE7_CLKp_new = not1(_GYVO_STORE7_CLKn_new);
    /* p29.AKOL*/ wire _AKOL_STORE8_CLKp_new = not1(_BUKA_STORE8_CLKn_new);
    /* p29.WUFA*/ wire _WUFA_STORE9_CLKp_new = not1(_DECU_STORE9_CLKn_new);

    /* p30.YGUS*/ sprite_store.YGUS_STORE0_I0n.dff8n(_GENY_STORE0_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.YSOK*/ sprite_store.YSOK_STORE0_I1n.dff8n(_GENY_STORE0_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.YZEP*/ sprite_store.YZEP_STORE0_I2n.dff8n(_GENY_STORE0_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.WYTE*/ sprite_store.WYTE_STORE0_I3n.dff8n(_GENY_STORE0_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.ZONY*/ sprite_store.ZONY_STORE0_I4n.dff8n(_GENY_STORE0_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.YWAK*/ sprite_store.YWAK_STORE0_I5n.dff8n(_GENY_STORE0_CLKp_new, SPR_TRI_I_in_new[5].qp_old());

    /* p30.CADU*/ sprite_store.CADU_STORE1_I0n.dff8n(_BYVY_STORE1_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.CEBO*/ sprite_store.CEBO_STORE1_I1n.dff8n(_BYVY_STORE1_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.CUFA*/ sprite_store.CUFA_STORE1_I2n.dff8n(_BYVY_STORE1_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.COMA*/ sprite_store.COMA_STORE1_I3n.dff8n(_BYVY_STORE1_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.CUZA*/ sprite_store.CUZA_STORE1_I4n.dff8n(_BYVY_STORE1_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.CAJY*/ sprite_store.CAJY_STORE1_I5n.dff8n(_BYVY_STORE1_CLKp_new, SPR_TRI_I_in_new[5].qp_old());

    /* p30.BUHE*/ sprite_store.BUHE_STORE2_I0n.dff8n(_BUZY_STORE2_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.BYHU*/ sprite_store.BYHU_STORE2_I1n.dff8n(_BUZY_STORE2_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.BECA*/ sprite_store.BECA_STORE2_I2n.dff8n(_BUZY_STORE2_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.BULU*/ sprite_store.BULU_STORE2_I3n.dff8n(_BUZY_STORE2_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.BUNA*/ sprite_store.BUNA_STORE2_I4n.dff8n(_BUZY_STORE2_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.BOXA*/ sprite_store.BOXA_STORE2_I5n.dff8n(_BUZY_STORE2_CLKp_new, SPR_TRI_I_in_new[5].qp_old());

    /* p30.DEVY*/ sprite_store.DEVY_STORE3_I0n.dff8n(_FEKA_STORE3_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.DESE*/ sprite_store.DESE_STORE3_I1n.dff8n(_FEKA_STORE3_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.DUNY*/ sprite_store.DUNY_STORE3_I2n.dff8n(_FEKA_STORE3_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.DUHA*/ sprite_store.DUHA_STORE3_I3n.dff8n(_FEKA_STORE3_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.DEBA*/ sprite_store.DEBA_STORE3_I4n.dff8n(_FEKA_STORE3_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.DAFU*/ sprite_store.DAFU_STORE3_I5n.dff8n(_FEKA_STORE3_CLKp_new, SPR_TRI_I_in_new[5].qp_old());

    /* p30.XAVE*/ sprite_store.XAVE_STORE4_I0n.dff8n(_WYLU_STORE4_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.XEFE*/ sprite_store.XEFE_STORE4_I1n.dff8n(_WYLU_STORE4_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.WANU*/ sprite_store.WANU_STORE4_I2n.dff8n(_WYLU_STORE4_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.XABO*/ sprite_store.XABO_STORE4_I3n.dff8n(_WYLU_STORE4_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.XEGE*/ sprite_store.XEGE_STORE4_I4n.dff8n(_WYLU_STORE4_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.XYNU*/ sprite_store.XYNU_STORE4_I5n.dff8n(_WYLU_STORE4_CLKp_new, SPR_TRI_I_in_new[5].qp_old());

    /* p30.EKOP*/ sprite_store.EKOP_STORE5_I0n.dff8n(_DYMO_STORE5_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.ETYM*/ sprite_store.ETYM_STORE5_I1n.dff8n(_DYMO_STORE5_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.GORU*/ sprite_store.GORU_STORE5_I2n.dff8n(_DYMO_STORE5_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.EBEX*/ sprite_store.EBEX_STORE5_I3n.dff8n(_DYMO_STORE5_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.ETAV*/ sprite_store.ETAV_STORE5_I4n.dff8n(_DYMO_STORE5_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.EKAP*/ sprite_store.EKAP_STORE5_I5n.dff8n(_DYMO_STORE5_CLKp_new, SPR_TRI_I_in_new[5].qp_old());

    /* p30.GABO*/ sprite_store.GABO_STORE6_I0n.dff8n(_WUSE_STORE6_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.GACY*/ sprite_store.GACY_STORE6_I1n.dff8n(_WUSE_STORE6_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.FOGO*/ sprite_store.FOGO_STORE6_I2n.dff8n(_WUSE_STORE6_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.GOHU*/ sprite_store.GOHU_STORE6_I3n.dff8n(_WUSE_STORE6_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.FOXY*/ sprite_store.FOXY_STORE6_I4n.dff8n(_WUSE_STORE6_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.GECU*/ sprite_store.GECU_STORE6_I5n.dff8n(_WUSE_STORE6_CLKp_new, SPR_TRI_I_in_new[5].qp_old());

    /* p30.GULE*/ sprite_store.GULE_STORE7_I0n.dff8n(_FEFO_STORE7_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.GYNO*/ sprite_store.GYNO_STORE7_I1n.dff8n(_FEFO_STORE7_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.FEFA*/ sprite_store.FEFA_STORE7_I2n.dff8n(_FEFO_STORE7_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.FYSU*/ sprite_store.FYSU_STORE7_I3n.dff8n(_FEFO_STORE7_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.GESY*/ sprite_store.GESY_STORE7_I4n.dff8n(_FEFO_STORE7_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.FUZO*/ sprite_store.FUZO_STORE7_I5n.dff8n(_FEFO_STORE7_CLKp_new, SPR_TRI_I_in_new[5].qp_old());

    /* p30.AXUV*/ sprite_store.AXUV_STORE8_I0n.dff8n(_AKOL_STORE8_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.BADA*/ sprite_store.BADA_STORE8_I1n.dff8n(_AKOL_STORE8_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.APEV*/ sprite_store.APEV_STORE8_I2n.dff8n(_AKOL_STORE8_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.BADO*/ sprite_store.BADO_STORE8_I3n.dff8n(_AKOL_STORE8_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.BEXY*/ sprite_store.BEXY_STORE8_I4n.dff8n(_AKOL_STORE8_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.BYHE*/ sprite_store.BYHE_STORE8_I5n.dff8n(_AKOL_STORE8_CLKp_new, SPR_TRI_I_in_new[5].qp_old());

    /* p30.YBER*/ sprite_store.YBER_STORE9_I0n.dff8n(_WUFA_STORE9_CLKp_new, SPR_TRI_I_in_new[0].qp_old());
    /* p30.YZOR*/ sprite_store.YZOR_STORE9_I1n.dff8n(_WUFA_STORE9_CLKp_new, SPR_TRI_I_in_new[1].qp_old());
    /* p30.XYFE*/ sprite_store.XYFE_STORE9_I2n.dff8n(_WUFA_STORE9_CLKp_new, SPR_TRI_I_in_new[2].qp_old());
    /* p30.XOTU*/ sprite_store.XOTU_STORE9_I3n.dff8n(_WUFA_STORE9_CLKp_new, SPR_TRI_I_in_new[3].qp_old());
    /* p30.XUTE*/ sprite_store.XUTE_STORE9_I4n.dff8n(_WUFA_STORE9_CLKp_new, SPR_TRI_I_in_new[4].qp_old());
    /* p30.XUFO*/ sprite_store.XUFO_STORE9_I5n.dff8n(_WUFA_STORE9_CLKp_new, SPR_TRI_I_in_new[5].qp_old());
  }

  {
    /* p29.ENOB*/ wire _ENOB_STORE0_CLKp_new = not1(_DYHU_STORE0_CLKn_new);
    /* p29.AHOF*/ wire _AHOF_STORE1_CLKp_new = not1(_BUCO_STORE1_CLKn_new);
    /* p29.FUKE*/ wire _FUKE_STORE2_CLKp_new = not1(_GYFO_STORE2_CLKn_new);
    /* p29.XYHA*/ wire _XYHA_STORE3_CLKp_new = not1(_GUSA_STORE3_CLKn_new);
    /* p29.EWOT*/ wire _EWOT_STORE4_CLKp_new = not1(_DUKE_STORE4_CLKn_new);
    /* p29.BUCY*/ wire _BUCY_STORE5_CLKp_new = not1(_BEDE_STORE5_CLKn_new);
    /* p29.ZURU*/ wire _ZURU_STORE6_CLKp_new = not1(_WEKA_STORE6_CLKn_new);
    /* p29.WABE*/ wire _WABE_STORE7_CLKp_new = not1(_GYVO_STORE7_CLKn_new);
    /* p29.BYMY*/ wire _BYMY_STORE8_CLKp_new = not1(_BUKA_STORE8_CLKn_new);
    /* p29.FAKA*/ wire _FAKA_STORE9_CLKp_new = not1(_DECU_STORE9_CLKn_new);

    /* p30.GYHO*/ sprite_store.GYHO_STORE0_L0n.dff8n(_ENOB_STORE0_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.CUFO*/ sprite_store.CUFO_STORE0_L1n.dff8n(_ENOB_STORE0_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.BOZU*/ sprite_store.BOZU_STORE0_L2n.dff8n(_ENOB_STORE0_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.FYHY*/ sprite_store.FYHY_STORE0_L3n.dff8n(_ENOB_STORE0_CLKp_new, SPR_TRI_L_in_new[3].qp_old());

    /* p30.AMES*/ sprite_store.AMES_STORE1_L0n.dff8n(_AHOF_STORE1_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.AROF*/ sprite_store.AROF_STORE1_L1n.dff8n(_AHOF_STORE1_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.ABOP*/ sprite_store.ABOP_STORE1_L2n.dff8n(_AHOF_STORE1_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.ABUG*/ sprite_store.ABUG_STORE1_L3n.dff8n(_AHOF_STORE1_CLKp_new, SPR_TRI_L_in_new[3].qp_old());

    /* p30.YLOV*/ sprite_store.YLOV_STORE2_L0n.dff8n(_FUKE_STORE2_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.XOSY*/ sprite_store.XOSY_STORE2_L1n.dff8n(_FUKE_STORE2_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.XAZY*/ sprite_store.XAZY_STORE2_L2n.dff8n(_FUKE_STORE2_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.YKUK*/ sprite_store.YKUK_STORE2_L3n.dff8n(_FUKE_STORE2_CLKp_new, SPR_TRI_L_in_new[3].qp_old());

    /* p30.ZURO*/ sprite_store.ZURO_STORE3_L0n.dff8n(_XYHA_STORE3_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.ZYLU*/ sprite_store.ZYLU_STORE3_L1n.dff8n(_XYHA_STORE3_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.ZENE*/ sprite_store.ZENE_STORE3_L2n.dff8n(_XYHA_STORE3_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.ZURY*/ sprite_store.ZURY_STORE3_L3n.dff8n(_XYHA_STORE3_CLKp_new, SPR_TRI_L_in_new[3].qp_old());

    /* p30.CAPO*/ sprite_store.CAPO_STORE4_L0n.dff8n(_EWOT_STORE4_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.CAJU*/ sprite_store.CAJU_STORE4_L1n.dff8n(_EWOT_STORE4_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.CONO*/ sprite_store.CONO_STORE4_L2n.dff8n(_EWOT_STORE4_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.CUMU*/ sprite_store.CUMU_STORE4_L3n.dff8n(_EWOT_STORE4_CLKp_new, SPR_TRI_L_in_new[3].qp_old());

    /* p30.ACEP*/ sprite_store.ACEP_STORE5_L0n.dff8n(_BUCY_STORE5_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.ABEG*/ sprite_store.ABEG_STORE5_L1n.dff8n(_BUCY_STORE5_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.ABUX*/ sprite_store.ABUX_STORE5_L2n.dff8n(_BUCY_STORE5_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.ANED*/ sprite_store.ANED_STORE5_L3n.dff8n(_BUCY_STORE5_CLKp_new, SPR_TRI_L_in_new[3].qp_old());

    /* p30.ZUMY*/ sprite_store.ZUMY_STORE6_L0n.dff8n(_ZURU_STORE6_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.ZAFU*/ sprite_store.ZAFU_STORE6_L1n.dff8n(_ZURU_STORE6_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.ZEXO*/ sprite_store.ZEXO_STORE6_L2n.dff8n(_ZURU_STORE6_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.ZUBE*/ sprite_store.ZUBE_STORE6_L3n.dff8n(_ZURU_STORE6_CLKp_new, SPR_TRI_L_in_new[3].qp_old());

    /* p30.XYNA*/ sprite_store.XYNA_STORE7_L0n.dff8n(_WABE_STORE7_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.YGUM*/ sprite_store.YGUM_STORE7_L1n.dff8n(_WABE_STORE7_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.XAKU*/ sprite_store.XAKU_STORE7_L2n.dff8n(_WABE_STORE7_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.XYGO*/ sprite_store.XYGO_STORE7_L3n.dff8n(_WABE_STORE7_CLKp_new, SPR_TRI_L_in_new[3].qp_old());

    /* p30.AZAP*/ sprite_store.AZAP_STORE8_L0n.dff8n(_BYMY_STORE8_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.AFYX*/ sprite_store.AFYX_STORE8_L1n.dff8n(_BYMY_STORE8_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.AFUT*/ sprite_store.AFUT_STORE8_L2n.dff8n(_BYMY_STORE8_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.AFYM*/ sprite_store.AFYM_STORE8_L3n.dff8n(_BYMY_STORE8_CLKp_new, SPR_TRI_L_in_new[3].qp_old());

    /* p30.CANA*/ sprite_store.CANA_STORE9_L0n.dff8n(_FAKA_STORE9_CLKp_new, SPR_TRI_L_in_new[0].qp_old());
    /* p30.FOFO*/ sprite_store.FOFO_STORE9_L1n.dff8n(_FAKA_STORE9_CLKp_new, SPR_TRI_L_in_new[1].qp_old());
    /* p30.DYSY*/ sprite_store.DYSY_STORE9_L2n.dff8n(_FAKA_STORE9_CLKp_new, SPR_TRI_L_in_new[2].qp_old());
    /* p30.DEWU*/ sprite_store.DEWU_STORE9_L3n.dff8n(_FAKA_STORE9_CLKp_new, SPR_TRI_L_in_new[3].qp_old());
  }
#pragma endregion

  {
    /* p29.FUXU*/ wire _FUXU_STORE0_CLKp_new = not1(_DYHU_STORE0_CLKn_new);
    /* p29.ASYS*/ wire _ASYS_STORE1_CLKp_new = not1(_BUCO_STORE1_CLKn_new);
    /* p29.CACU*/ wire _CACU_STORE2_CLKp_new = not1(_GYFO_STORE2_CLKn_new);
    /* p29.YFAG*/ wire _YFAG_STORE3_CLKp_new = not1(_GUSA_STORE3_CLKn_new);
    /* p29.WOFO*/ wire _WOFO_STORE4_CLKp_new = not1(_DUKE_STORE4_CLKn_new);
    /* p29.CYLA*/ wire _CYLA_STORE5_CLKp_new = not1(_BEDE_STORE5_CLKn_new);
    /* p29.ZAPE*/ wire _ZAPE_STORE6_CLKp_new = not1(_WEKA_STORE6_CLKn_new);
    /* p29.GECY*/ wire _GECY_STORE7_CLKp_new = not1(_GYVO_STORE7_CLKn_new);
    /* p29.CEXU*/ wire _CEXU_STORE8_CLKp_new = not1(_BUKA_STORE8_CLKn_new);
    /* p29.WEME*/ wire _WEME_STORE9_CLKp_new = not1(_DECU_STORE9_CLKn_new);

    /* p28.ABAK*/ wire _ABAK_LINE_RSTp_new =  or2(_ATEJ_LINE_TRIGp_new, _AMYG_VID_RSTp_new);
    /* p28.BYVA*/ wire _BYVA_LINE_RSTn_new = not1(_ABAK_LINE_RSTp_new);
    /* p29.DYBA*/ wire _DYBA_LINE_RSTp_new = not1(_BYVA_LINE_RSTn_new);

    /* p29.DYWE*/ wire _DYWE_STORE0_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.EBOJ_STORE0_RSTp.qp_new());
    /* p29.EFEV*/ wire _EFEV_STORE1_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.CEDY_STORE1_RSTp.qp_new());
    /* p29.FOKO*/ wire _FOKO_STORE2_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.EGAV_STORE2_RSTp.qp_new());
    /* p29.GAKE*/ wire _GAKE_STORE3_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.GOTA_STORE3_RSTp.qp_new());
    /* p29.WOHU*/ wire _WOHU_STORE4_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.XUDY_STORE4_RSTp.qp_new());
    /* p29.FEVE*/ wire _FEVE_STORE5_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.WAFY_STORE5_RSTp.qp_new());
    /* p29.WACY*/ wire _WACY_STORE6_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.WOMY_STORE6_RSTp.qp_new());
    /* p29.GUKY*/ wire _GUKY_STORE7_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.WAPO_STORE7_RSTp.qp_new());
    /* p29.GORO*/ wire _GORO_STORE8_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.EXUQ_STORE8_RSTp.qp_new());
    /* p29.DUBU*/ wire _DUBU_STORE9_RSTp_new = or2(_DYBA_LINE_RSTp_new, sprite_store.FONO_STORE9_RSTp.qp_new());

    /* p29.DYNA*/ wire _DYNA_STORE0_RSTn_new = not1(_DYWE_STORE0_RSTp_new);
    /* p29.DOKU*/ wire _DOKU_STORE1_RSTn_new = not1(_EFEV_STORE1_RSTp_new);
    /* p29.GAMY*/ wire _GAMY_STORE2_RSTn_new = not1(_FOKO_STORE2_RSTp_new);
    /* p29.WUPA*/ wire _WUPA_STORE3_RSTn_new = not1(_GAKE_STORE3_RSTp_new);
    /* p29.WUNU*/ wire _WUNU_STORE4_RSTn_new = not1(_WOHU_STORE4_RSTp_new);
    /* p29.EJAD*/ wire _EJAD_STORE5_RSTn_new = not1(_FEVE_STORE5_RSTp_new);
    /* p29.XAHO*/ wire _XAHO_STORE6_RSTn_new = not1(_WACY_STORE6_RSTp_new);
    /* p29.GAFY*/ wire _GAFY_STORE7_RSTn_new = not1(_GUKY_STORE7_RSTp_new);
    /* p29.WUZO*/ wire _WUZO_STORE8_RSTn_new = not1(_GORO_STORE8_RSTp_new);
    /* p29.DOSY*/ wire _DOSY_STORE9_RSTn_new = not1(_DUBU_STORE9_RSTp_new);

    /*#p31.XEPE*/ sprite_store.XEPE_STORE0_X0p.dff9(_FUXU_STORE0_CLKp_new, _DYNA_STORE0_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.YLAH*/ sprite_store.YLAH_STORE0_X1p.dff9(_FUXU_STORE0_CLKp_new, _DYNA_STORE0_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.ZOLA*/ sprite_store.ZOLA_STORE0_X2p.dff9(_FUXU_STORE0_CLKp_new, _DYNA_STORE0_RSTn_new, _YPUR_SPX2n_old);
    /* p31.ZULU*/ sprite_store.ZULU_STORE0_X3p.dff9(_FUXU_STORE0_CLKp_new, _DYNA_STORE0_RSTn_new, _YVOK_SPX3n_old);
    /* p31.WELO*/ sprite_store.WELO_STORE0_X4p.dff9(_FUXU_STORE0_CLKp_new, _DYNA_STORE0_RSTn_new, _COSE_SPX4n_old);
    /* p31.XUNY*/ sprite_store.XUNY_STORE0_X5p.dff9(_FUXU_STORE0_CLKp_new, _DYNA_STORE0_RSTn_new, _AROP_SPX5n_old);
    /* p31.WOTE*/ sprite_store.WOTE_STORE0_X6p.dff9(_FUXU_STORE0_CLKp_new, _DYNA_STORE0_RSTn_new, _XATU_SPX6n_old);
    /* p31.XAKO*/ sprite_store.XAKO_STORE0_X7p.dff9(_FUXU_STORE0_CLKp_new, _DYNA_STORE0_RSTn_new, _BADY_SPX7n_old);

    /* p31.DANY*/ sprite_store.DANY_STORE1_X0p.dff9(_ASYS_STORE1_CLKp_new, _DOKU_STORE1_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.DUKO*/ sprite_store.DUKO_STORE1_X1p.dff9(_ASYS_STORE1_CLKp_new, _DOKU_STORE1_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.DESU*/ sprite_store.DESU_STORE1_X2p.dff9(_ASYS_STORE1_CLKp_new, _DOKU_STORE1_RSTn_new, _YPUR_SPX2n_old);
    /* p31.DAZO*/ sprite_store.DAZO_STORE1_X3p.dff9(_ASYS_STORE1_CLKp_new, _DOKU_STORE1_RSTn_new, _YVOK_SPX3n_old);
    /* p31.DAKE*/ sprite_store.DAKE_STORE1_X4p.dff9(_ASYS_STORE1_CLKp_new, _DOKU_STORE1_RSTn_new, _COSE_SPX4n_old);
    /* p31.CESO*/ sprite_store.CESO_STORE1_X5p.dff9(_ASYS_STORE1_CLKp_new, _DOKU_STORE1_RSTn_new, _AROP_SPX5n_old);
    /* p31.DYFU*/ sprite_store.DYFU_STORE1_X6p.dff9(_ASYS_STORE1_CLKp_new, _DOKU_STORE1_RSTn_new, _XATU_SPX6n_old);
    /* p31.CUSY*/ sprite_store.CUSY_STORE1_X7p.dff9(_ASYS_STORE1_CLKp_new, _DOKU_STORE1_RSTn_new, _BADY_SPX7n_old);

    /* p31.FOKA*/ sprite_store.FOKA_STORE2_X0p.dff9(_CACU_STORE2_CLKp_new, _GAMY_STORE2_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.FYTY*/ sprite_store.FYTY_STORE2_X1p.dff9(_CACU_STORE2_CLKp_new, _GAMY_STORE2_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.FUBY*/ sprite_store.FUBY_STORE2_X2p.dff9(_CACU_STORE2_CLKp_new, _GAMY_STORE2_RSTn_new, _YPUR_SPX2n_old);
    /* p31.GOXU*/ sprite_store.GOXU_STORE2_X3p.dff9(_CACU_STORE2_CLKp_new, _GAMY_STORE2_RSTn_new, _YVOK_SPX3n_old);
    /* p31.DUHY*/ sprite_store.DUHY_STORE2_X4p.dff9(_CACU_STORE2_CLKp_new, _GAMY_STORE2_RSTn_new, _COSE_SPX4n_old);
    /* p31.EJUF*/ sprite_store.EJUF_STORE2_X5p.dff9(_CACU_STORE2_CLKp_new, _GAMY_STORE2_RSTn_new, _AROP_SPX5n_old);
    /* p31.ENOR*/ sprite_store.ENOR_STORE2_X6p.dff9(_CACU_STORE2_CLKp_new, _GAMY_STORE2_RSTn_new, _XATU_SPX6n_old);
    /* p31.DEPY*/ sprite_store.DEPY_STORE2_X7p.dff9(_CACU_STORE2_CLKp_new, _GAMY_STORE2_RSTn_new, _BADY_SPX7n_old);

    /* p31.XOLY*/ sprite_store.XOLY_STORE3_X0p.dff9(_YFAG_STORE3_CLKp_new, _WUPA_STORE3_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.XYBA*/ sprite_store.XYBA_STORE3_X1p.dff9(_YFAG_STORE3_CLKp_new, _WUPA_STORE3_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.XABE*/ sprite_store.XABE_STORE3_X2p.dff9(_YFAG_STORE3_CLKp_new, _WUPA_STORE3_RSTn_new, _YPUR_SPX2n_old);
    /* p31.XEKA*/ sprite_store.XEKA_STORE3_X3p.dff9(_YFAG_STORE3_CLKp_new, _WUPA_STORE3_RSTn_new, _YVOK_SPX3n_old);
    /* p31.XOMY*/ sprite_store.XOMY_STORE3_X4p.dff9(_YFAG_STORE3_CLKp_new, _WUPA_STORE3_RSTn_new, _COSE_SPX4n_old);
    /* p31.WUHA*/ sprite_store.WUHA_STORE3_X5p.dff9(_YFAG_STORE3_CLKp_new, _WUPA_STORE3_RSTn_new, _AROP_SPX5n_old);
    /* p31.WYNA*/ sprite_store.WYNA_STORE3_X6p.dff9(_YFAG_STORE3_CLKp_new, _WUPA_STORE3_RSTn_new, _XATU_SPX6n_old);
    /* p31.WECO*/ sprite_store.WECO_STORE3_X7p.dff9(_YFAG_STORE3_CLKp_new, _WUPA_STORE3_RSTn_new, _BADY_SPX7n_old);

    /* p31.WEDU*/ sprite_store.WEDU_STORE4_X0p.dff9(_WOFO_STORE4_CLKp_new, _WUNU_STORE4_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.YGAJ*/ sprite_store.YGAJ_STORE4_X1p.dff9(_WOFO_STORE4_CLKp_new, _WUNU_STORE4_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.ZYJO*/ sprite_store.ZYJO_STORE4_X2p.dff9(_WOFO_STORE4_CLKp_new, _WUNU_STORE4_RSTn_new, _YPUR_SPX2n_old);
    /* p31.XURY*/ sprite_store.XURY_STORE4_X3p.dff9(_WOFO_STORE4_CLKp_new, _WUNU_STORE4_RSTn_new, _YVOK_SPX3n_old);
    /* p31.YBED*/ sprite_store.YBED_STORE4_X4p.dff9(_WOFO_STORE4_CLKp_new, _WUNU_STORE4_RSTn_new, _COSE_SPX4n_old);
    /* p31.ZALA*/ sprite_store.ZALA_STORE4_X5p.dff9(_WOFO_STORE4_CLKp_new, _WUNU_STORE4_RSTn_new, _AROP_SPX5n_old);
    /* p31.WYDE*/ sprite_store.WYDE_STORE4_X6p.dff9(_WOFO_STORE4_CLKp_new, _WUNU_STORE4_RSTn_new, _XATU_SPX6n_old);
    /* p31.XEPA*/ sprite_store.XEPA_STORE4_X7p.dff9(_WOFO_STORE4_CLKp_new, _WUNU_STORE4_RSTn_new, _BADY_SPX7n_old);

    /* p31.FUSA*/ sprite_store.FUSA_STORE5_X0p.dff9(_CYLA_STORE5_CLKp_new, _EJAD_STORE5_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.FAXA*/ sprite_store.FAXA_STORE5_X1p.dff9(_CYLA_STORE5_CLKp_new, _EJAD_STORE5_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.FOZY*/ sprite_store.FOZY_STORE5_X2p.dff9(_CYLA_STORE5_CLKp_new, _EJAD_STORE5_RSTn_new, _YPUR_SPX2n_old);
    /* p31.FESY*/ sprite_store.FESY_STORE5_X3p.dff9(_CYLA_STORE5_CLKp_new, _EJAD_STORE5_RSTn_new, _YVOK_SPX3n_old);
    /* p31.CYWE*/ sprite_store.CYWE_STORE5_X4p.dff9(_CYLA_STORE5_CLKp_new, _EJAD_STORE5_RSTn_new, _COSE_SPX4n_old);
    /* p31.DYBY*/ sprite_store.DYBY_STORE5_X5p.dff9(_CYLA_STORE5_CLKp_new, _EJAD_STORE5_RSTn_new, _AROP_SPX5n_old);
    /* p31.DURY*/ sprite_store.DURY_STORE5_X6p.dff9(_CYLA_STORE5_CLKp_new, _EJAD_STORE5_RSTn_new, _XATU_SPX6n_old);
    /* p31.CUVY*/ sprite_store.CUVY_STORE5_X7p.dff9(_CYLA_STORE5_CLKp_new, _EJAD_STORE5_RSTn_new, _BADY_SPX7n_old);

    /* p31.YCOL*/ sprite_store.YCOL_STORE6_X0p.dff9(_ZAPE_STORE6_CLKp_new, _XAHO_STORE6_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.YRAC*/ sprite_store.YRAC_STORE6_X1p.dff9(_ZAPE_STORE6_CLKp_new, _XAHO_STORE6_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.YMEM*/ sprite_store.YMEM_STORE6_X2p.dff9(_ZAPE_STORE6_CLKp_new, _XAHO_STORE6_RSTn_new, _YPUR_SPX2n_old);
    /* p31.YVAG*/ sprite_store.YVAG_STORE6_X3p.dff9(_ZAPE_STORE6_CLKp_new, _XAHO_STORE6_RSTn_new, _YVOK_SPX3n_old);
    /* p31.ZOLY*/ sprite_store.ZOLY_STORE6_X4p.dff9(_ZAPE_STORE6_CLKp_new, _XAHO_STORE6_RSTn_new, _COSE_SPX4n_old);
    /* p31.ZOGO*/ sprite_store.ZOGO_STORE6_X5p.dff9(_ZAPE_STORE6_CLKp_new, _XAHO_STORE6_RSTn_new, _AROP_SPX5n_old);
    /* p31.ZECU*/ sprite_store.ZECU_STORE6_X6p.dff9(_ZAPE_STORE6_CLKp_new, _XAHO_STORE6_RSTn_new, _XATU_SPX6n_old);
    /* p31.ZESA*/ sprite_store.ZESA_STORE6_X7p.dff9(_ZAPE_STORE6_CLKp_new, _XAHO_STORE6_RSTn_new, _BADY_SPX7n_old);

    /* p31.ERAZ*/ sprite_store.ERAZ_STORE7_X0p.dff9(_GECY_STORE7_CLKp_new, _GAFY_STORE7_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.EPUM*/ sprite_store.EPUM_STORE7_X1p.dff9(_GECY_STORE7_CLKp_new, _GAFY_STORE7_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.EROL*/ sprite_store.EROL_STORE7_X2p.dff9(_GECY_STORE7_CLKp_new, _GAFY_STORE7_RSTn_new, _YPUR_SPX2n_old);
    /* p31.EHYN*/ sprite_store.EHYN_STORE7_X3p.dff9(_GECY_STORE7_CLKp_new, _GAFY_STORE7_RSTn_new, _YVOK_SPX3n_old);
    /* p31.FAZU*/ sprite_store.FAZU_STORE7_X4p.dff9(_GECY_STORE7_CLKp_new, _GAFY_STORE7_RSTn_new, _COSE_SPX4n_old);
    /* p31.FAXE*/ sprite_store.FAXE_STORE7_X5p.dff9(_GECY_STORE7_CLKp_new, _GAFY_STORE7_RSTn_new, _AROP_SPX5n_old);
    /* p31.EXUK*/ sprite_store.EXUK_STORE7_X6p.dff9(_GECY_STORE7_CLKp_new, _GAFY_STORE7_RSTn_new, _XATU_SPX6n_old);
    /* p31.FEDE*/ sprite_store.FEDE_STORE7_X7p.dff9(_GECY_STORE7_CLKp_new, _GAFY_STORE7_RSTn_new, _BADY_SPX7n_old);

    /* p31.EZUF*/ sprite_store.EZUF_STORE8_X0p.dff9(_CEXU_STORE8_CLKp_new, _WUZO_STORE8_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.ENAD*/ sprite_store.ENAD_STORE8_X1p.dff9(_CEXU_STORE8_CLKp_new, _WUZO_STORE8_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.EBOW*/ sprite_store.EBOW_STORE8_X2p.dff9(_CEXU_STORE8_CLKp_new, _WUZO_STORE8_RSTn_new, _YPUR_SPX2n_old);
    /* p31.FYCA*/ sprite_store.FYCA_STORE8_X3p.dff9(_CEXU_STORE8_CLKp_new, _WUZO_STORE8_RSTn_new, _YVOK_SPX3n_old);
    /* p31.GAVY*/ sprite_store.GAVY_STORE8_X4p.dff9(_CEXU_STORE8_CLKp_new, _WUZO_STORE8_RSTn_new, _COSE_SPX4n_old);
    /* p31.GYPU*/ sprite_store.GYPU_STORE8_X5p.dff9(_CEXU_STORE8_CLKp_new, _WUZO_STORE8_RSTn_new, _AROP_SPX5n_old);
    /* p31.GADY*/ sprite_store.GADY_STORE8_X6p.dff9(_CEXU_STORE8_CLKp_new, _WUZO_STORE8_RSTn_new, _XATU_SPX6n_old);
    /* p31.GAZA*/ sprite_store.GAZA_STORE8_X7p.dff9(_CEXU_STORE8_CLKp_new, _WUZO_STORE8_RSTn_new, _BADY_SPX7n_old);

    /* p31.XUVY*/ sprite_store.XUVY_STORE9_X0p.dff9(_WEME_STORE9_CLKp_new, _DOSY_STORE9_RSTn_new, _ZAGO_SPX0n_old);
    /* p31.XERE*/ sprite_store.XERE_STORE9_X1p.dff9(_WEME_STORE9_CLKp_new, _DOSY_STORE9_RSTn_new, _ZOCY_SPX1n_old);
    /* p31.XUZO*/ sprite_store.XUZO_STORE9_X2p.dff9(_WEME_STORE9_CLKp_new, _DOSY_STORE9_RSTn_new, _YPUR_SPX2n_old);
    /* p31.XEXA*/ sprite_store.XEXA_STORE9_X3p.dff9(_WEME_STORE9_CLKp_new, _DOSY_STORE9_RSTn_new, _YVOK_SPX3n_old);
    /* p31.YPOD*/ sprite_store.YPOD_STORE9_X4p.dff9(_WEME_STORE9_CLKp_new, _DOSY_STORE9_RSTn_new, _COSE_SPX4n_old);
    /* p31.YROP*/ sprite_store.YROP_STORE9_X5p.dff9(_WEME_STORE9_CLKp_new, _DOSY_STORE9_RSTn_new, _AROP_SPX5n_old);
    /* p31.YNEP*/ sprite_store.YNEP_STORE9_X6p.dff9(_WEME_STORE9_CLKp_new, _DOSY_STORE9_RSTn_new, _XATU_SPX6n_old);
    /* p31.YZOF*/ sprite_store.YZOF_STORE9_X7p.dff9(_WEME_STORE9_CLKp_new, _DOSY_STORE9_RSTn_new, _BADY_SPX7n_old);
  }

  //----------------------------------------
  // Ext pins

  {
    /* p08.LOXO*/ wire _LOXO_HOLDn_new = and_or3(_MULE_MODE_DBG1n_ext, _TEXO_8000_9FFFn_ext, _UMUT_MODE_DBG1p_ext);
    /* p08.LASY*/ wire _LASY_HOLDp_new = not1(_LOXO_HOLDn_new);
    /* p08.MATE*/ wire _MATE_HOLDn_new = not1(_LASY_HOLDp_new);
    /* p08.ALOR*/ ext_bus.ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 0]);
    /* p08.APUR*/ ext_bus.APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 1]);
    /* p08.ALYR*/ ext_bus.ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 2]);
    /* p08.ARET*/ ext_bus.ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 3]);
    /* p08.AVYS*/ ext_bus.AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 4]);
    /* p08.ATEV*/ ext_bus.ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 5]);
    /* p08.AROS*/ ext_bus.AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 6]);
    /* p08.ARYM*/ ext_bus.ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 7]);
    /* p08.LUNO*/ ext_bus.LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 8]);
    /* p08.LYSA*/ ext_bus.LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[ 9]);
    /* p08.PATE*/ ext_bus.PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[10]);
    /* p08.LUMY*/ ext_bus.LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[11]);
    /* p08.LOBU*/ ext_bus.LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[12]);
    /* p08.LONU*/ ext_bus.LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[13]);
    /* p08.NYRE*/ ext_bus.NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A_ext[14]);

    /* p08.AMET*/ wire _AMET_A00p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NAKY_DMA_A00p.qp_new(), ext_bus.ALOR_EXT_ADDR_LATCH_00p.qp_new());
    /* p08.ATOL*/ wire _ATOL_A01p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PYRO_DMA_A01p.qp_new(), ext_bus.APUR_EXT_ADDR_LATCH_01p.qp_new());
    /* p08.APOK*/ wire _APOK_A02p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NEFY_DMA_A02p.qp_new(), ext_bus.ALYR_EXT_ADDR_LATCH_02p.qp_new());
    /* p08.AMER*/ wire _AMER_A03p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MUTY_DMA_A03p.qp_new(), ext_bus.ARET_EXT_ADDR_LATCH_03p.qp_new());
    /* p08.ATEM*/ wire _ATEM_A04p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NYKO_DMA_A04p.qp_new(), ext_bus.AVYS_EXT_ADDR_LATCH_04p.qp_new());
    /* p08.ATOV*/ wire _ATOV_A05p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PYLO_DMA_A05p.qp_new(), ext_bus.ATEV_EXT_ADDR_LATCH_05p.qp_new());
    /* p08.ATYR*/ wire _ATYR_A06p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NUTO_DMA_A06p.qp_new(), ext_bus.AROS_EXT_ADDR_LATCH_06p.qp_new());
    /*#p08.ASUR*/ wire _ASUR_A07p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MUGU_DMA_A07p.qp_new(), ext_bus.ARYM_EXT_ADDR_LATCH_07p.qp_new());
    /*#p08.MANO*/ wire _MANO_A08p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NAFA_DMA_A08n.qn_new(), ext_bus.LUNO_EXT_ADDR_LATCH_08p.qp_new());
    /* p08.MASU*/ wire _MASU_A09p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PYNE_DMA_A09n.qn_new(), ext_bus.LYSA_EXT_ADDR_LATCH_09p.qp_new());
    /* p08.PAMY*/ wire _PAMY_A10p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PARA_DMA_A10n.qn_new(), ext_bus.PATE_EXT_ADDR_LATCH_10p.qp_new());
    /* p08.MALE*/ wire _MALE_A11p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NYDO_DMA_A11n.qn_new(), ext_bus.LUMY_EXT_ADDR_LATCH_11p.qp_new());
    /* p08.MOJY*/ wire _MOJY_A12p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NYGY_DMA_A12n.qn_new(), ext_bus.LOBU_EXT_ADDR_LATCH_12p.qp_new());
    /* p08.MUCE*/ wire _MUCE_A13p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PULA_DMA_A13n.qn_new(), ext_bus.LONU_EXT_ADDR_LATCH_13p.qp_new());
    /* p08.PEGE*/ wire _PEGE_A14p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.POKU_DMA_A14n.qn_new(), ext_bus.NYRE_EXT_ADDR_LATCH_14p.qp_new());

    /* p08.KUPO*/ wire _KUPO_new = nand2(_AMET_A00p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.CABA*/ wire _CABA_new = nand2(_ATOL_A01p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.BOKU*/ wire _BOKU_new = nand2(_APOK_A02p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.BOTY*/ wire _BOTY_new = nand2(_AMER_A03p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.BYLA*/ wire _BYLA_new = nand2(_ATEM_A04p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.BADU*/ wire _BADU_new = nand2(_ATOV_A05p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.CEPU*/ wire _CEPU_new = nand2(_ATYR_A06p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.DEFY*/ wire _DEFY_new = nand2(_ASUR_A07p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.MYNY*/ wire _MYNY_new = nand2(_MANO_A08p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.MUNE*/ wire _MUNE_new = nand2(_MASU_A09p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.ROXU*/ wire _ROXU_new = nand2(_PAMY_A10p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.LEPY*/ wire _LEPY_new = nand2(_MALE_A11p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.LUCE*/ wire _LUCE_new = nand2(_MOJY_A12p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.LABE*/ wire _LABE_new = nand2(_MUCE_A13p_new, _TOVA_MODE_DBG2n_ext);
    /* p08.PUHE*/ wire _PUHE_new = nand2(_PEGE_A14p_new, _TOVA_MODE_DBG2n_ext);

    /* p08.KOTY*/ wire _KOTY_new = nor2 (_AMET_A00p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.COTU*/ wire _COTU_new = nor2 (_ATOL_A01p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.BAJO*/ wire _BAJO_new = nor2 (_APOK_A02p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.BOLA*/ wire _BOLA_new = nor2 (_AMER_A03p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.BEVO*/ wire _BEVO_new = nor2 (_ATEM_A04p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.AJAV*/ wire _AJAV_new = nor2 (_ATOV_A05p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.CYKA*/ wire _CYKA_new = nor2 (_ATYR_A06p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.COLO*/ wire _COLO_new = nor2 (_ASUR_A07p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.MEGO*/ wire _MEGO_new = nor2 (_MANO_A08p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.MENY*/ wire _MENY_new = nor2 (_MASU_A09p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.RORE*/ wire _RORE_new = nor2 (_PAMY_A10p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.LYNY*/ wire _LYNY_new = nor2 (_MALE_A11p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.LOSO*/ wire _LOSO_new = nor2 (_MOJY_A12p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.LEVA*/ wire _LEVA_new = nor2 (_MUCE_A13p_new, _UNOR_MODE_DBG2p_ext);
    /* p08.PAHY*/ wire _PAHY_new = nor2 (_PEGE_A14p_new, _UNOR_MODE_DBG2p_ext);

    PIN_EXT_A_new[ 0].pin_out(1, _KUPO_new, _KOTY_new);
    PIN_EXT_A_new[ 1].pin_out(1, _CABA_new, _COTU_new);
    PIN_EXT_A_new[ 2].pin_out(1, _BOKU_new, _BAJO_new);
    PIN_EXT_A_new[ 3].pin_out(1, _BOTY_new, _BOLA_new);
    PIN_EXT_A_new[ 4].pin_out(1, _BYLA_new, _BEVO_new);
    PIN_EXT_A_new[ 5].pin_out(1, _BADU_new, _AJAV_new);
    PIN_EXT_A_new[ 6].pin_out(1, _CEPU_new, _CYKA_new);
    PIN_EXT_A_new[ 7].pin_out(1, _DEFY_new, _COLO_new);
    PIN_EXT_A_new[ 8].pin_out(1, _MYNY_new, _MEGO_new);
    PIN_EXT_A_new[ 9].pin_out(1, _MUNE_new, _MENY_new);
    PIN_EXT_A_new[10].pin_out(1, _ROXU_new, _RORE_new);
    PIN_EXT_A_new[11].pin_out(1, _LEPY_new, _LYNY_new);
    PIN_EXT_A_new[12].pin_out(1, _LUCE_new, _LOSO_new);
    PIN_EXT_A_new[13].pin_out(1, _LABE_new, _LEVA_new);
    PIN_EXT_A_new[14].pin_out(1, _PUHE_new, _PAHY_new);

    // A15 is "special"
    /* p08.SOBY*/ wire _SOBY_A15n_new = nor2(BUS_CPU_A_ext[15], _TUTU_ADDR_BOOTp_new);
    /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx_clknew = nand2(_ABUZ_xxCDEFGH_clk, _SOBY_A15n_new);
    /* p08.TAZY*/ wire _TAZY_A15p_new = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MARU_DMA_A15n.qn_new(), _SEPY_A15p_ABxxxxxx_clknew);
    /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n_new = nand2(_TAZY_A15p_new, _RYCA_MODE_DBG2n_ext);
    /* p08.RULO*/ wire _RULO_PIN_EXT_A15n_new = nor2 (_TAZY_A15p_new, _UNOR_MODE_DBG2p_ext);
    PIN_EXT_A_new[15].pin_out(1, _SUZE_PIN_EXT_A15n_new, _RULO_PIN_EXT_A15n_new);

    PIN_EXT_CLK.pin_out(1, _BUDE_xxxxEFGH_clk, _BUDE_xxxxEFGH_clk);

    /* p08.TYMU*/ wire _TYMU_EXT_RDn_new = nor2(_LUMA_DMA_CARTp_new, _MOTY_CPU_EXT_RD_ext);
    /* p08.UGAC*/ wire _UGAC_RD_A_new = nand2(_TYMU_EXT_RDn_new, _TOVA_MODE_DBG2n_ext);
    /* p08.URUN*/ wire _URUN_RD_D_new = nor2 (_TYMU_EXT_RDn_new, _UNOR_MODE_DBG2p_ext);
    PIN_EXT_RDn.pin_out(1, _UGAC_RD_A_new, _URUN_RD_D_new);

    /* p08.MEXO*/ wire _MEXO_CPU_WRn_ABCDxxxH_new = not1(_APOV_CPU_WRp_xxxxEFGx_clk);
    /* p08.NEVY*/ wire _NEVY_new = or2(_MEXO_CPU_WRn_ABCDxxxH_new, _MOCA_DBG_EXT_RD_ext);
    /* p08.PUVA*/ wire _PUVA_EXT_WRn_new = or2(_NEVY_new, _LUMA_DMA_CARTp_new);
    /* p08.UVER*/ wire _UVER_WR_A_new = nand2(_PUVA_EXT_WRn_new, _TOVA_MODE_DBG2n_ext);
    /* p08.USUF*/ wire _USUF_WR_D_new = nor2 (_PUVA_EXT_WRn_new, _UNOR_MODE_DBG2p_ext);
    PIN_EXT_WRn.pin_out(1, _UVER_WR_A_new, _USUF_WR_D_new);

    /* p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk = and3(_ABUZ_xxCDEFGH_clk, _TYNU_A000_FFFFp_ext, _TUNA_0000_FDFFp_ext);
    /* p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MARU_DMA_A15n.qn_new(), _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk);
    PIN_EXT_CSn.pin_out(1, _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew, _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew);
  }

  //----------------------------------------------------------------------------------------------------
  // External device interface

  {
    if (DELTA_AB || DELTA_CD || DELTA_EF || DELTA_GH) {
      ext_addr_latch = pack_u16p_new(16, &PIN_EXT_A_new[ 0]);
    }

    // ROM read
    uint16_t rom_addr = ext_addr_latch & 0x7FFF;
    wire rom_CEn_new = PIN_EXT_A_new[15].qp_ext();
    wire rom_OEp_new = !rom_CEn_new && !PIN_EXT_RDn.qp_ext() && cart_buf;

    // Ext RAM read
    uint16_t eram_addr = (ext_addr_latch & 0x1FFF);
    wire eram_CE1n_new = PIN_EXT_CSn.qp_ext();
    wire eram_CE2_new  = PIN_EXT_A_new[14].qp_ext();
    wire eram_WRp_new  = !eram_CE1n_new && eram_CE2_new && !PIN_EXT_WRn.qp_ext();
    wire eram_OEp_new  = !eram_CE1n_new && eram_CE2_new && !eram_WRp_new && !PIN_EXT_RDn.qp_ext();

    // Cart RAM read
    uint16_t cram_addr = (ext_addr_latch & 0x1FFF);
    wire cram_CS1n_new = PIN_EXT_CSn.qp_ext();
    wire cram_CS2_new  = PIN_EXT_A_new[13].qp_ext() && !PIN_EXT_A_new[14].qp_ext() && PIN_EXT_A_new[15].qp_ext();
    wire cram_WRp_new  = !cram_CS1n_new && cram_CS2_new && !PIN_EXT_WRn.qp_ext();
    wire cram_OEp_new  = !cram_CS1n_new && cram_CS2_new && !PIN_EXT_RDn.qp_ext();

    if (DELTA_HA || DELTA_BC || DELTA_DE || DELTA_FG) {
      if (rom_OEp_new)  ext_data_latch = cart_buf[rom_addr];
      if (eram_OEp_new) ext_data_latch = ext_ram[eram_addr];
      if (cram_OEp_new) ext_data_latch = cart_ram[cram_addr];
    }

    wire ext_OEp = rom_OEp_new || eram_OEp_new || cram_OEp_new;
    PIN_EXT_D_in_new[0].pin_in(ext_OEp, wire(ext_data_latch & 0x01));
    PIN_EXT_D_in_new[1].pin_in(ext_OEp, wire(ext_data_latch & 0x02));
    PIN_EXT_D_in_new[2].pin_in(ext_OEp, wire(ext_data_latch & 0x04));
    PIN_EXT_D_in_new[3].pin_in(ext_OEp, wire(ext_data_latch & 0x08));
    PIN_EXT_D_in_new[4].pin_in(ext_OEp, wire(ext_data_latch & 0x10));
    PIN_EXT_D_in_new[5].pin_in(ext_OEp, wire(ext_data_latch & 0x20));
    PIN_EXT_D_in_new[6].pin_in(ext_OEp, wire(ext_data_latch & 0x40));
    PIN_EXT_D_in_new[7].pin_in(ext_OEp, wire(ext_data_latch & 0x80));

    // CPU data bus -> external data bus
    // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /* p08.RORU*/ wire _RORU_CBD_TO_EPDn_new = mux2p(_UNOR_MODE_DBG2p_ext, _REDU_CPU_RDn_ext, _MOTY_CPU_EXT_RD_ext);
    /* p08.LULA*/ wire _LULA_CBD_TO_EPDp_new = not1(_RORU_CBD_TO_EPDn_new);

    /* p25.RUXA*/ wire _RUXA_new = nand2(BUS_CPU_Dp_ext[0].qp_ext(), _LULA_CBD_TO_EPDp_new);
    /* p25.RUJA*/ wire _RUJA_new = nand2(BUS_CPU_Dp_ext[1].qp_ext(), _LULA_CBD_TO_EPDp_new);
    /* p25.RABY*/ wire _RABY_new = nand2(BUS_CPU_Dp_ext[2].qp_ext(), _LULA_CBD_TO_EPDp_new);
    /* p25.RERA*/ wire _RERA_new = nand2(BUS_CPU_Dp_ext[3].qp_ext(), _LULA_CBD_TO_EPDp_new);
    /* p25.RORY*/ wire _RORY_new = nand2(BUS_CPU_Dp_ext[4].qp_ext(), _LULA_CBD_TO_EPDp_new);
    /* p25.RYVO*/ wire _RYVO_new = nand2(BUS_CPU_Dp_ext[5].qp_ext(), _LULA_CBD_TO_EPDp_new);
    /* p25.RAFY*/ wire _RAFY_new = nand2(BUS_CPU_Dp_ext[6].qp_ext(), _LULA_CBD_TO_EPDp_new);
    /* p25.RAVU*/ wire _RAVU_new = nand2(BUS_CPU_Dp_ext[7].qp_ext(), _LULA_CBD_TO_EPDp_new);

    /* p08.RUNE*/ wire _RUNE_new = nor2 (BUS_CPU_Dp_ext[0].qp_ext(), _RORU_CBD_TO_EPDn_new);
    /* p08.RYPU*/ wire _RYPU_new = nor2 (BUS_CPU_Dp_ext[1].qp_ext(), _RORU_CBD_TO_EPDn_new);
    /* p08.SULY*/ wire _SULY_new = nor2 (BUS_CPU_Dp_ext[2].qp_ext(), _RORU_CBD_TO_EPDn_new);
    /* p08.SEZE*/ wire _SEZE_new = nor2 (BUS_CPU_Dp_ext[3].qp_ext(), _RORU_CBD_TO_EPDn_new);
    /* p08.RESY*/ wire _RESY_new = nor2 (BUS_CPU_Dp_ext[4].qp_ext(), _RORU_CBD_TO_EPDn_new);
    /* p08.TAMU*/ wire _TAMU_new = nor2 (BUS_CPU_Dp_ext[5].qp_ext(), _RORU_CBD_TO_EPDn_new);
    /* p08.ROGY*/ wire _ROGY_new = nor2 (BUS_CPU_Dp_ext[6].qp_ext(), _RORU_CBD_TO_EPDn_new);
    /* p08.RYDA*/ wire _RYDA_new = nor2 (BUS_CPU_Dp_ext[7].qp_ext(), _RORU_CBD_TO_EPDn_new);

    PIN_EXT_D_out_new[0].pin_out(_LULA_CBD_TO_EPDp_new, _RUXA_new, _RUNE_new);
    PIN_EXT_D_out_new[1].pin_out(_LULA_CBD_TO_EPDp_new, _RUJA_new, _RYPU_new);
    PIN_EXT_D_out_new[2].pin_out(_LULA_CBD_TO_EPDp_new, _RABY_new, _SULY_new);
    PIN_EXT_D_out_new[3].pin_out(_LULA_CBD_TO_EPDp_new, _RERA_new, _SEZE_new);
    PIN_EXT_D_out_new[4].pin_out(_LULA_CBD_TO_EPDp_new, _RORY_new, _RESY_new);
    PIN_EXT_D_out_new[5].pin_out(_LULA_CBD_TO_EPDp_new, _RYVO_new, _TAMU_new);
    PIN_EXT_D_out_new[6].pin_out(_LULA_CBD_TO_EPDp_new, _RAFY_new, _ROGY_new);
    PIN_EXT_D_out_new[7].pin_out(_LULA_CBD_TO_EPDp_new, _RAVU_new, _RYDA_new);

    // Ext RAM write
    if (eram_WRp_new) ext_ram[eram_addr]  = pack_u8p_new(8, &PIN_EXT_D_out_new[0]);

    // Cart RAM write
    if (cram_WRp_new) cart_ram[cram_addr] = pack_u8p_new(8, &PIN_EXT_D_out_new[0]);
  }

  //----------------------------------------------------------------------------------------------------

  // FIXME - implement MBC1

  // 0000-3FFF - ROM Bank 00 (Read Only) This area always contains the first 16KBytes of the cartridge ROM.
  // 4000-7FFF - ROM Bank 01-7F (Read Only) This area may contain any of the further 16KByte banks of the ROM, allowing to address up to 125 ROM Banks (almost 2MByte). As described below, bank numbers 20h, 40h, and 60h cannot be used, resulting in the odd amount of 125 banks.
  // A000-BFFF - RAM Bank 00-03, if any (Read/Write) This area is used to address external RAM in the cartridge (if any). External RAM is often battery buffered, allowing to store game positions or high score tables, even if the gameboy is turned off, or if the cartridge is removed from the gameboy. Available RAM sizes are: 2KByte (at A000-A7FF), 8KByte (at A000-BFFF), and 32KByte (in form of four 8K banks at A000-BFFF).

  // 0000-1FFF - RAM Enable (Write Only)   00h  Disable RAM (default)   ?Ah  Enable RAM Practically any value with 0Ah in the lower 4 bits enables RAM, and any other value disables RAM.
  // 2000-3FFF - ROM Bank Number (Write Only) Writing to this address space selects the lower 5 bits of the ROM Bank Number (in range 01-1Fh). When 00h is written, the MBC translates that to bank 01h also. That doesn't harm so far, because ROM Bank 00h can be always directly accessed by reading from 0000-3FFF.
  // But (when using the register below to specify the upper ROM Bank bits), the same happens for Bank 20h, 40h, and 60h. Any attempt to address these ROM Banks will select Bank 21h, 41h, and 61h instead.
  // 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only) This 2bit register can be used to select a RAM Bank in range from 00-03h, or to specify the upper two bits (Bit 5-6) of the ROM Bank number, depending on the current ROM/RAM Mode. (See below.)
  // 6000-7FFF - ROM/RAM Mode Select (Write Only)  00h = ROM Banking Mode (up to 8KByte RAM, 2MByte ROM) (default)   01h = RAM Banking Mode (up to 32KByte RAM, 512KByte ROM)

  // MBC1_RAM_EN

  // MBC1_BANK_D0
  // MBC1_BANK_D1
  // MBC1_BANK_D2
  // MBC1_BANK_D3
  // MBC1_BANK_D4
  // MBC1_BANK_D5
  // MBC1_BANK_D6

  // MBC1_MODE

  /*
  {

    bool bank_0 = nor(MBC1_BANK_D0, MBC1_BANK_D1, MBC1_BANK_D2, MBC1_BANK_D3, MBC1_BANK_D4);

    wire cart_rom_a14 = bank_0 ? 1 : MBC1_BANK_D0.qp_new();
    wire cart_rom_a15 = bank_0 ? 0 : MBC1_BANK_D1.qp_new();
    wire cart_rom_a16 = bank_0 ? 0 : MBC1_BANK_D2.qp_new();
    wire cart_rom_a17 = bank_0 ? 0 : MBC1_BANK_D3.qp_new();
    wire cart_rom_a18 = bank_0 ? 0 : MBC1_BANK_D4.qp_new();
    wire cart_rom_a19 = MBC1_MODE.qp_new() ? 0 : bank_0 ? 0 : MBC1_BANK_D5.qp_new();
    wire cart_rom_a20 = MBC1_MODE.qp_new() ? 0 : bank_0 ? 0 : MBC1_BANK_D6.qp_new();

    wire cart_ram_a13 = MBC1_MODE.qp_new() ? MBC1_BANK_D5.qp_new() : 0;
    wire cart_ram_a14 = MBC1_MODE.qp_new() ? MBC1_BANK_D6.qp_new() : 0;

    // ROM read
    {
      uint16_t rom_addr = ext_addr & 0x7FFF;
      wire OEn = PIN_EXT_RDn.qp_new();
      wire CEn = PIN_EXT_A[15].qp_new();

      if (!CEn && !OEn) {
        ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
    }
  }
  */

#pragma region SpriteData
  // VRAM tri -> sprite pix temp + x flip
  {
    /* p33.PUTE*/ wire _PUTE_FLIP0p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp_in[7].qp_old(), BUS_VRAM_Dp_in[0].qp_old());
    /* p33.PELO*/ wire _PELO_FLIP1p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp_in[6].qp_old(), BUS_VRAM_Dp_in[1].qp_old());
    /* p33.PONO*/ wire _PONO_FLIP2p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp_in[5].qp_old(), BUS_VRAM_Dp_in[2].qp_old());
    /* p33.POBE*/ wire _POBE_FLIP3p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp_in[4].qp_old(), BUS_VRAM_Dp_in[3].qp_old());
    /* p33.PACY*/ wire _PACY_FLIP4p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp_in[3].qp_old(), BUS_VRAM_Dp_in[4].qp_old());
    /* p33.PUGU*/ wire _PUGU_FLIP5p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp_in[2].qp_old(), BUS_VRAM_Dp_in[5].qp_old());
    /* p33.PAWE*/ wire _PAWE_FLIP6p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp_in[1].qp_old(), BUS_VRAM_Dp_in[6].qp_old());
    /* p33.PULY*/ wire _PULY_FLIP7p_old = mux2p(_XONO_FLIP_X_old, BUS_VRAM_Dp_in[0].qp_old(), BUS_VRAM_Dp_in[7].qp_old());

    /* p29.TYTU*/ wire _TYTU_SFETCH_S0n_new = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
    /* p29.SYCU*/ wire _SYCU_SFETCH_S0pe_new = nor3(_TYTU_SFETCH_S0n_new, _LOBY_RENDERINGn_new, sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new());

    /*#p29.RACA*/ wire _RACA_CLKp_new = and2(sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new(), _SYCU_SFETCH_S0pe_new);
    /*#p29.PEBY*/ wire _PEBY_CLKn_new = not1(_RACA_CLKp_new);
    /*#p29.NYBE*/ wire _NYBE_CLKp_new = not1(_PEBY_CLKn_new);
    /*#p29.PUCO*/ wire _PUCO_CLKn_new = not1(_NYBE_CLKp_new);
    /* p33.PEFO*/ vram_bus.PEFO_SPRITE_DB0n.dff8n(_PUCO_CLKn_new, _PUTE_FLIP0p_old);
    /* p33.ROKA*/ vram_bus.ROKA_SPRITE_DB1n.dff8n(_PUCO_CLKn_new, _PELO_FLIP1p_old);
    /* p33.MYTU*/ vram_bus.MYTU_SPRITE_DB2n.dff8n(_PUCO_CLKn_new, _PONO_FLIP2p_old);
    /* p33.RAMU*/ vram_bus.RAMU_SPRITE_DB3n.dff8n(_PUCO_CLKn_new, _POBE_FLIP3p_old);
    /* p33.SELE*/ vram_bus.SELE_SPRITE_DB4n.dff8n(_PUCO_CLKn_new, _PACY_FLIP4p_old);
    /* p33.SUTO*/ vram_bus.SUTO_SPRITE_DB5n.dff8n(_PUCO_CLKn_new, _PUGU_FLIP5p_old);
    /* p33.RAMA*/ vram_bus.RAMA_SPRITE_DB6n.dff8n(_PUCO_CLKn_new, _PAWE_FLIP6p_old);
    /* p33.RYDU*/ vram_bus.RYDU_SPRITE_DB7n.dff8n(_PUCO_CLKn_new, _PULY_FLIP7p_old);

    /*#p29.TOPU*/ wire _TOPU_CLKp_new = and2(sprite_fetcher.TULY_SFETCH_S1p.qp_new(),    _SYCU_SFETCH_S0pe_new);
    /*#p29.VYWA*/ wire _VYWA_CLKn_new = not1(_TOPU_CLKp_new);
    /*#p29.WENY*/ wire _WENY_CLKp_new = not1(_VYWA_CLKn_new);
    /*#p29.XADO*/ wire _XADO_CLKn_new = not1(_WENY_CLKp_new);
    /* p33.REWO*/ vram_bus.REWO_SPRITE_DA0n.dff8n(_XADO_CLKn_new, _PUTE_FLIP0p_old);
    /* p33.PEBA*/ vram_bus.PEBA_SPRITE_DA1n.dff8n(_XADO_CLKn_new, _PELO_FLIP1p_old);
    /* p33.MOFO*/ vram_bus.MOFO_SPRITE_DA2n.dff8n(_XADO_CLKn_new, _PONO_FLIP2p_old);
    /* p33.PUDU*/ vram_bus.PUDU_SPRITE_DA3n.dff8n(_XADO_CLKn_new, _POBE_FLIP3p_old);
    /* p33.SAJA*/ vram_bus.SAJA_SPRITE_DA4n.dff8n(_XADO_CLKn_new, _PACY_FLIP4p_old);
    /* p33.SUNY*/ vram_bus.SUNY_SPRITE_DA5n.dff8n(_XADO_CLKn_new, _PUGU_FLIP5p_old);
    /* p33.SEMO*/ vram_bus.SEMO_SPRITE_DA6n.dff8n(_XADO_CLKn_new, _PAWE_FLIP6p_old);
    /* p33.SEGA*/ vram_bus.SEGA_SPRITE_DA7n.dff8n(_XADO_CLKn_new, _PULY_FLIP7p_old);
  }
#pragma endregion

#pragma region Pixel_Pipes
  /* p32.PYBO*/ pix_pipe.PYBO_BGW_PIPE_A7.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.NEDA_BGW_PIPE_A6.qp_old());
  /* p32.NEDA*/ pix_pipe.NEDA_BGW_PIPE_A6.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.MODU_BGW_PIPE_A5.qp_old());
  /* p32.MODU*/ pix_pipe.MODU_BGW_PIPE_A5.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.NEPO_BGW_PIPE_A4.qp_old());
  /* p32.NEPO*/ pix_pipe.NEPO_BGW_PIPE_A4.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.MACU_BGW_PIPE_A3.qp_old());
  /* p32.MACU*/ pix_pipe.MACU_BGW_PIPE_A3.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.MOJU_BGW_PIPE_A2.qp_old());
  /* p32.MOJU*/ pix_pipe.MOJU_BGW_PIPE_A2.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.NOZO_BGW_PIPE_A1.qp_old());
  /* p32.NOZO*/ pix_pipe.NOZO_BGW_PIPE_A1.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.MYDE_BGW_PIPE_A0.qp_old());
  /* p32.MYDE*/ pix_pipe.MYDE_BGW_PIPE_A0.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, GND);

  /* p32.SOHU*/ pix_pipe.SOHU_BGW_PIPE_B7.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.RALU_BGW_PIPE_B6.qp_old());
  /* p32.RALU*/ pix_pipe.RALU_BGW_PIPE_B6.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.SETU_BGW_PIPE_B5.qp_old());
  /* p32.SETU*/ pix_pipe.SETU_BGW_PIPE_B5.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.SOBO_BGW_PIPE_B4.qp_old());
  /* p32.SOBO*/ pix_pipe.SOBO_BGW_PIPE_B4.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.RYSA_BGW_PIPE_B3.qp_old());
  /* p32.RYSA*/ pix_pipe.RYSA_BGW_PIPE_B3.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.SADY_BGW_PIPE_B2.qp_old());
  /* p32.SADY*/ pix_pipe.SADY_BGW_PIPE_B2.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.TACA_BGW_PIPE_B1.qp_old());
  /* p32.TACA*/ pix_pipe.TACA_BGW_PIPE_B1.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.TOMY_BGW_PIPE_B0.qp_old());
  /* p32.TOMY*/ pix_pipe.TOMY_BGW_PIPE_B0.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, GND);

  /* p33.WUFY*/ pix_pipe.WUFY_SPR_PIPE_A7.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.VAFO_SPR_PIPE_A6.qp_old());
  /* p33.VAFO*/ pix_pipe.VAFO_SPR_PIPE_A6.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.WORA_SPR_PIPE_A5.qp_old());
  /* p33.WORA*/ pix_pipe.WORA_SPR_PIPE_A5.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.WYHO_SPR_PIPE_A4.qp_old());
  /* p33.WYHO*/ pix_pipe.WYHO_SPR_PIPE_A4.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.LESU_SPR_PIPE_A3.qp_old());
  /* p33.LESU*/ pix_pipe.LESU_SPR_PIPE_A3.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.LEFE_SPR_PIPE_A2.qp_old());
  /* p33.LEFE*/ pix_pipe.LEFE_SPR_PIPE_A2.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.MASO_SPR_PIPE_A1.qp_old());
  /* p33.MASO*/ pix_pipe.MASO_SPR_PIPE_A1.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.NURO_SPR_PIPE_A0.qp_old());
  /* p33.NURO*/ pix_pipe.NURO_SPR_PIPE_A0.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, GND);

  /* p33.VUPY*/ pix_pipe.VUPY_SPR_PIPE_B7.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.VANU_SPR_PIPE_B6.qp_old());
  /* p33.VANU*/ pix_pipe.VANU_SPR_PIPE_B6.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.WEBA_SPR_PIPE_B5.qp_old());
  /* p33.WEBA*/ pix_pipe.WEBA_SPR_PIPE_B5.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.VARE_SPR_PIPE_B4.qp_old());
  /* p33.VARE*/ pix_pipe.VARE_SPR_PIPE_B4.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.PYJO_SPR_PIPE_B3.qp_old());
  /* p33.PYJO*/ pix_pipe.PYJO_SPR_PIPE_B3.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.NATY_SPR_PIPE_B2.qp_old());
  /* p33.NATY*/ pix_pipe.NATY_SPR_PIPE_B2.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.PEFU_SPR_PIPE_B1.qp_old());
  /* p33.PEFU*/ pix_pipe.PEFU_SPR_PIPE_B1.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.NYLU_SPR_PIPE_B0.qp_old());
  /* p33.NYLU*/ pix_pipe.NYLU_SPR_PIPE_B0.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, GND);

  /* p34.LYME*/ pix_pipe.LYME_PAL_PIPE_D7.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.MODA_PAL_PIPE_D6.qp_old());
  /* p34.MODA*/ pix_pipe.MODA_PAL_PIPE_D6.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.NUKE_PAL_PIPE_D5.qp_old());
  /* p34.NUKE*/ pix_pipe.NUKE_PAL_PIPE_D5.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.PALU_PAL_PIPE_D4.qp_old());
  /* p34.PALU*/ pix_pipe.PALU_PAL_PIPE_D4.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.SOMY_PAL_PIPE_D3.qp_old());
  /* p34.SOMY*/ pix_pipe.SOMY_PAL_PIPE_D3.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.ROSA_PAL_PIPE_D2.qp_old());
  /* p34.ROSA*/ pix_pipe.ROSA_PAL_PIPE_D2.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.SATA_PAL_PIPE_D1.qp_old());
  /* p34.SATA*/ pix_pipe.SATA_PAL_PIPE_D1.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.RUGO_PAL_PIPE_D0.qp_old());
  /* p34.RUGO*/ pix_pipe.RUGO_PAL_PIPE_D0.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, GND);

  /* p26.VAVA*/ pix_pipe.VAVA_MASK_PIPE_7.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.VUMO_MASK_PIPE_6.qp_old());
  /* p26.VUMO*/ pix_pipe.VUMO_MASK_PIPE_6.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.WODA_MASK_PIPE_5.qp_old());
  /* p26.WODA*/ pix_pipe.WODA_MASK_PIPE_5.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.XETE_MASK_PIPE_4.qp_old());
  /* p26.XETE*/ pix_pipe.XETE_MASK_PIPE_4.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.WYFU_MASK_PIPE_3.qp_old());
  /* p26.WYFU*/ pix_pipe.WYFU_MASK_PIPE_3.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.VOSA_MASK_PIPE_2.qp_old());
  /* p26.VOSA*/ pix_pipe.VOSA_MASK_PIPE_2.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.WURU_MASK_PIPE_1.qp_old());
  /* p26.WURU*/ pix_pipe.WURU_MASK_PIPE_1.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, pix_pipe.VEZO_MASK_PIPE_0.qp_old());
  /* p26.VEZO*/ pix_pipe.VEZO_MASK_PIPE_0.dff22(_SACU_CLKPIPE_AxCxExGx_clknew, _VYPO_VCC);




  /* p29.XEFY*/ wire _XEPY_SPRITE_DONEn_new  = not1(_WUTY_SPRITE_DONEp_new);
  /* p34.MEFU*/ wire _MEFU_SPRITE_MASK0n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipe.NYLU_SPR_PIPE_B0.qp_new(), pix_pipe.NURO_SPR_PIPE_A0.qp_new()); // def or
  /* p34.MEVE*/ wire _MEVE_SPRITE_MASK1n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipe.PEFU_SPR_PIPE_B1.qp_new(), pix_pipe.MASO_SPR_PIPE_A1.qp_new());
  /* p34.MYZO*/ wire _MYZO_SPRITE_MASK2n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipe.NATY_SPR_PIPE_B2.qp_new(), pix_pipe.LEFE_SPR_PIPE_A2.qp_new());
  /* p34.RUDA*/ wire _RUDA_SPRITE_MASK3n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipe.PYJO_SPR_PIPE_B3.qp_new(), pix_pipe.LESU_SPR_PIPE_A3.qp_new());
  /* p34.VOTO*/ wire _VOTO_SPRITE_MASK4n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipe.VARE_SPR_PIPE_B4.qp_new(), pix_pipe.WYHO_SPR_PIPE_A4.qp_new());
  /* p34.VYSA*/ wire _VYSA_SPRITE_MASK5n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipe.WEBA_SPR_PIPE_B5.qp_new(), pix_pipe.WORA_SPR_PIPE_A5.qp_new());
  /* p34.TORY*/ wire _TORY_SPRITE_MASK6n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipe.VANU_SPR_PIPE_B6.qp_new(), pix_pipe.VAFO_SPR_PIPE_A6.qp_new());
  /* p34.WOPE*/ wire _WOPE_SPRITE_MASK7n_new = or3(_XEPY_SPRITE_DONEn_new, pix_pipe.VUPY_SPR_PIPE_B7.qp_new(), pix_pipe.WUFY_SPR_PIPE_A7.qp_new());

  // The mask is 1 where there are _no_ sprite pixels
  /* p34.LESY*/ wire _LESY_SPRITE_MASK0p_new = not1(_MEFU_SPRITE_MASK0n_new);
  /* p34.LOTA*/ wire _LOTA_SPRITE_MASK1p_new = not1(_MEVE_SPRITE_MASK1n_new);
  /* p34.LYKU*/ wire _LYKU_SPRITE_MASK2p_new = not1(_MYZO_SPRITE_MASK2n_new);
  /* p34.ROBY*/ wire _ROBY_SPRITE_MASK3p_new = not1(_RUDA_SPRITE_MASK3n_new);
  /* p34.TYTA*/ wire _TYTA_SPRITE_MASK4p_new = not1(_VOTO_SPRITE_MASK4n_new);
  /* p34.TYCO*/ wire _TYCO_SPRITE_MASK5p_new = not1(_VYSA_SPRITE_MASK5n_new);
  /* p34.SOKA*/ wire _SOKA_SPRITE_MASK6p_new = not1(_TORY_SPRITE_MASK6n_new);
  /* p34.XOVU*/ wire _XOVU_SPRITE_MASK7p_new = not1(_WOPE_SPRITE_MASK7n_new);



  // Reload all the pipes using set/rst
  {
    /* p32.LOZE*/ wire _LOZE_PIPE_A_LOADp_new = not1(_NYXU_FETCH_TRIGn_new);
    /* p32.LAKY*/ wire _BG_PIPE_A_SET0_new = nand2(_LOZE_PIPE_A_LOADp_new, vram_bus.LEGU_TILE_DA0n.qn_new());
    /* p32.NYXO*/ wire _BG_PIPE_A_SET1_new = nand2(_LOZE_PIPE_A_LOADp_new, vram_bus.NUDU_TILE_DA1n.qn_new());
    /* p32.LOTO*/ wire _BG_PIPE_A_SET2_new = nand2(_LOZE_PIPE_A_LOADp_new, vram_bus.MUKU_TILE_DA2n.qn_new());
    /* p32.LYDU*/ wire _BG_PIPE_A_SET3_new = nand2(_LOZE_PIPE_A_LOADp_new, vram_bus.LUZO_TILE_DA3n.qn_new());
    /* p32.MYVY*/ wire _BG_PIPE_A_SET4_new = nand2(_LOZE_PIPE_A_LOADp_new, vram_bus.MEGU_TILE_DA4n.qn_new());
    /* p32.LODO*/ wire _BG_PIPE_A_SET5_new = nand2(_LOZE_PIPE_A_LOADp_new, vram_bus.MYJY_TILE_DA5n.qn_new());
    /* p32.NUTE*/ wire _BG_PIPE_A_SET6_new = nand2(_LOZE_PIPE_A_LOADp_new, vram_bus.NASA_TILE_DA6n.qn_new());
    /* p32.NAJA*/ wire _BG_PIPE_A_SET7_new = nand2(_LOZE_PIPE_A_LOADp_new, vram_bus.NEFO_TILE_DA7n.qn_new());

    /* p32.LUHE*/ wire _BG_PIX_A0n_new = not1(vram_bus.LEGU_TILE_DA0n.qn_new());
    /* p32.NOLY*/ wire _BG_PIX_A1n_new = not1(vram_bus.NUDU_TILE_DA1n.qn_new());
    /* p32.LEKE*/ wire _BG_PIX_A2n_new = not1(vram_bus.MUKU_TILE_DA2n.qn_new());
    /* p32.LOMY*/ wire _BG_PIX_A3n_new = not1(vram_bus.LUZO_TILE_DA3n.qn_new());
    /* p32.LALA*/ wire _BG_PIX_A4n_new = not1(vram_bus.MEGU_TILE_DA4n.qn_new());
    /* p32.LOXA*/ wire _BG_PIX_A5n_new = not1(vram_bus.MYJY_TILE_DA5n.qn_new());
    /* p32.NEZE*/ wire _BG_PIX_A6n_new = not1(vram_bus.NASA_TILE_DA6n.qn_new());
    /* p32.NOBO*/ wire _BG_PIX_A7n_new = not1(vram_bus.NEFO_TILE_DA7n.qn_new());

    /* p32.LOTY*/ wire _BG_PIPE_A_RST0_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A0n_new);
    /* p32.NEXA*/ wire _BG_PIPE_A_RST1_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A1n_new);
    /* p32.LUTU*/ wire _BG_PIPE_A_RST2_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A2n_new);
    /* p32.LUJA*/ wire _BG_PIPE_A_RST3_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A3n_new);
    /* p32.MOSY*/ wire _BG_PIPE_A_RST4_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A4n_new);
    /* p32.LERU*/ wire _BG_PIPE_A_RST5_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A5n_new);
    /* p32.NYHA*/ wire _BG_PIPE_A_RST6_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A6n_new);
    /* p32.NADY*/ wire _BG_PIPE_A_RST7_new = nand2(_LOZE_PIPE_A_LOADp_new, _BG_PIX_A7n_new);

    /* p32.PYBO*/ pix_pipe.PYBO_BGW_PIPE_A7.SETnRSTn(_BG_PIPE_A_SET7_new, _BG_PIPE_A_RST7_new);
    /* p32.NEDA*/ pix_pipe.NEDA_BGW_PIPE_A6.SETnRSTn(_BG_PIPE_A_SET6_new, _BG_PIPE_A_RST6_new);
    /* p32.MODU*/ pix_pipe.MODU_BGW_PIPE_A5.SETnRSTn(_BG_PIPE_A_SET5_new, _BG_PIPE_A_RST5_new);
    /* p32.NEPO*/ pix_pipe.NEPO_BGW_PIPE_A4.SETnRSTn(_BG_PIPE_A_SET4_new, _BG_PIPE_A_RST4_new);
    /* p32.MACU*/ pix_pipe.MACU_BGW_PIPE_A3.SETnRSTn(_BG_PIPE_A_SET3_new, _BG_PIPE_A_RST3_new);
    /* p32.MOJU*/ pix_pipe.MOJU_BGW_PIPE_A2.SETnRSTn(_BG_PIPE_A_SET2_new, _BG_PIPE_A_RST2_new);
    /* p32.NOZO*/ pix_pipe.NOZO_BGW_PIPE_A1.SETnRSTn(_BG_PIPE_A_SET1_new, _BG_PIPE_A_RST1_new);
    /* p32.MYDE*/ pix_pipe.MYDE_BGW_PIPE_A0.SETnRSTn(_BG_PIPE_A_SET0_new, _BG_PIPE_A_RST0_new);
  }

  {
    /* p32.LUXA*/ wire _LUXA_PIPE_B_LOADp_new = not1(_NYXU_FETCH_TRIGn_new);
    /* p32.TUXE*/ wire _BG_PIPE_B_SET0_new = nand2(_LUXA_PIPE_B_LOADp_new, vram_bus.RAWU_TILE_DB0p.qp_new());
    /* p32.SOLY*/ wire _BG_PIPE_B_SET1_new = nand2(_LUXA_PIPE_B_LOADp_new, vram_bus.POZO_TILE_DB1p.qp_new());
    /* p32.RUCE*/ wire _BG_PIPE_B_SET2_new = nand2(_LUXA_PIPE_B_LOADp_new, vram_bus.PYZO_TILE_DB2p.qp_new());
    /* p32.RYJA*/ wire _BG_PIPE_B_SET3_new = nand2(_LUXA_PIPE_B_LOADp_new, vram_bus.POXA_TILE_DB3p.qp_new());
    /* p32.RUTO*/ wire _BG_PIPE_B_SET4_new = nand2(_LUXA_PIPE_B_LOADp_new, vram_bus.PULO_TILE_DB4p.qp_new());
    /* p32.RAJA*/ wire _BG_PIPE_B_SET5_new = nand2(_LUXA_PIPE_B_LOADp_new, vram_bus.POJU_TILE_DB5p.qp_new());
    /* p32.RAJO*/ wire _BG_PIPE_B_SET6_new = nand2(_LUXA_PIPE_B_LOADp_new, vram_bus.POWY_TILE_DB6p.qp_new());
    /* p32.RAGA*/ wire _BG_PIPE_B_SET7_new = nand2(_LUXA_PIPE_B_LOADp_new, vram_bus.PYJU_TILE_DB7p.qp_new());

    /* p32.TOSA*/ wire _BG_PIX_B0n_new = not1(vram_bus.RAWU_TILE_DB0p.qp_new());
    /* p32.RUCO*/ wire _BG_PIX_B1n_new = not1(vram_bus.POZO_TILE_DB1p.qp_new());
    /* p32.TYCE*/ wire _BG_PIX_B2n_new = not1(vram_bus.PYZO_TILE_DB2p.qp_new());
    /* p32.REVY*/ wire _BG_PIX_B3n_new = not1(vram_bus.POXA_TILE_DB3p.qp_new());
    /* p32.RYGA*/ wire _BG_PIX_B4n_new = not1(vram_bus.PULO_TILE_DB4p.qp_new());
    /* p32.RYLE*/ wire _BG_PIX_B5n_new = not1(vram_bus.POJU_TILE_DB5p.qp_new());
    /* p32.RAPU*/ wire _BG_PIX_B6n_new = not1(vram_bus.POWY_TILE_DB6p.qp_new());
    /* p32.SOJA*/ wire _BG_PIX_B7n_new = not1(vram_bus.PYJU_TILE_DB7p.qp_new());

    /* p32.SEJA*/ wire _BG_PIPE_B_RST0_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B0n_new);
    /* p32.SENO*/ wire _BG_PIPE_B_RST1_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B1n_new);
    /* p32.SURE*/ wire _BG_PIPE_B_RST2_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B2n_new);
    /* p32.SEBO*/ wire _BG_PIPE_B_RST3_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B3n_new);
    /* p32.SUCA*/ wire _BG_PIPE_B_RST4_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B4n_new);
    /* p32.SYWE*/ wire _BG_PIPE_B_RST5_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B5n_new);
    /* p32.SUPU*/ wire _BG_PIPE_B_RST6_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B6n_new);
    /* p32.RYJY*/ wire _BG_PIPE_B_RST7_new = nand2(_LUXA_PIPE_B_LOADp_new, _BG_PIX_B7n_new);

    /* p32.SOHU*/ pix_pipe.SOHU_BGW_PIPE_B7.SETnRSTn(_BG_PIPE_B_SET7_new, _BG_PIPE_B_RST7_new);
    /* p32.RALU*/ pix_pipe.RALU_BGW_PIPE_B6.SETnRSTn(_BG_PIPE_B_SET6_new, _BG_PIPE_B_RST6_new);
    /* p32.SETU*/ pix_pipe.SETU_BGW_PIPE_B5.SETnRSTn(_BG_PIPE_B_SET5_new, _BG_PIPE_B_RST5_new);
    /* p32.SOBO*/ pix_pipe.SOBO_BGW_PIPE_B4.SETnRSTn(_BG_PIPE_B_SET4_new, _BG_PIPE_B_RST4_new);
    /* p32.RYSA*/ pix_pipe.RYSA_BGW_PIPE_B3.SETnRSTn(_BG_PIPE_B_SET3_new, _BG_PIPE_B_RST3_new);
    /* p32.SADY*/ pix_pipe.SADY_BGW_PIPE_B2.SETnRSTn(_BG_PIPE_B_SET2_new, _BG_PIPE_B_RST2_new);
    /* p32.TACA*/ pix_pipe.TACA_BGW_PIPE_B1.SETnRSTn(_BG_PIPE_B_SET1_new, _BG_PIPE_B_RST1_new);
    /* p32.TOMY*/ pix_pipe.TOMY_BGW_PIPE_B0.SETnRSTn(_BG_PIPE_B_SET0_new, _BG_PIPE_B_RST0_new);
  }

  {
    // Sprite pipe A
    {
      /* p33.PABE*/ wire _PABE_SPR_PIX_SET0_new = nand2(_LESY_SPRITE_MASK0p_new, vram_bus.REWO_SPRITE_DA0n.qn_new());
      /* p33.MYTO*/ wire _MYTO_SPR_PIX_SET1_new = nand2(_LOTA_SPRITE_MASK1p_new, vram_bus.PEBA_SPRITE_DA1n.qn_new());
      /* p33.LELA*/ wire _LELA_SPR_PIX_SET2_new = nand2(_LYKU_SPRITE_MASK2p_new, vram_bus.MOFO_SPRITE_DA2n.qn_new());
      /* p33.MAME*/ wire _MAME_SPR_PIX_SET3_new = nand2(_ROBY_SPRITE_MASK3p_new, vram_bus.PUDU_SPRITE_DA3n.qn_new());
      /* p33.VEXU*/ wire _VEXU_SPR_PIX_SET4_new = nand2(_TYTA_SPRITE_MASK4p_new, vram_bus.SAJA_SPRITE_DA4n.qn_new());
      /* p33.VABY*/ wire _VABY_SPR_PIX_SET5_new = nand2(_TYCO_SPRITE_MASK5p_new, vram_bus.SUNY_SPRITE_DA5n.qn_new());
      /* p33.TUXA*/ wire _TUXA_SPR_PIX_SET6_new = nand2(_SOKA_SPRITE_MASK6p_new, vram_bus.SEMO_SPRITE_DA6n.qn_new());
      /* p33.VUNE*/ wire _VUNE_SPR_PIX_SET7_new = nand2(_XOVU_SPRITE_MASK7p_new, vram_bus.SEGA_SPRITE_DA7n.qn_new());

      /* p33.RATA*/ wire _RATA_SPR_PIX_DA0n_new = not1(vram_bus.REWO_SPRITE_DA0n.qn_new());
      /* p33.NUCA*/ wire _NUCA_SPR_PIX_DA1n_new = not1(vram_bus.PEBA_SPRITE_DA1n.qn_new());
      /* p33.LASE*/ wire _LASE_SPR_PIX_DA2n_new = not1(vram_bus.MOFO_SPRITE_DA2n.qn_new());
      /* p33.LUBO*/ wire _LUBO_SPR_PIX_DA3n_new = not1(vram_bus.PUDU_SPRITE_DA3n.qn_new());
      /* p33.WERY*/ wire _WERY_SPR_PIX_DA4n_new = not1(vram_bus.SAJA_SPRITE_DA4n.qn_new());
      /* p33.WURA*/ wire _WURA_SPR_PIX_DA5n_new = not1(vram_bus.SUNY_SPRITE_DA5n.qn_new());
      /* p33.SULU*/ wire _SULU_SPR_PIX_DA6n_new = not1(vram_bus.SEMO_SPRITE_DA6n.qn_new());
      /* p33.WAMY*/ wire _WAMY_SPR_PIX_DA7n_new = not1(vram_bus.SEGA_SPRITE_DA7n.qn_new());

      /* p33.PYZU*/ wire _PYZU_SPR_PIX_RST0_new = nand2(_LESY_SPRITE_MASK0p_new, _RATA_SPR_PIX_DA0n_new);
      /* p33.MADA*/ wire _MADA_SPR_PIX_RST1_new = nand2(_LOTA_SPRITE_MASK1p_new, _NUCA_SPR_PIX_DA1n_new);
      /* p33.LYDE*/ wire _LYDE_SPR_PIX_RST2_new = nand2(_LYKU_SPRITE_MASK2p_new, _LASE_SPR_PIX_DA2n_new);
      /* p33.LUFY*/ wire _LUFY_SPR_PIX_RST3_new = nand2(_ROBY_SPRITE_MASK3p_new, _LUBO_SPR_PIX_DA3n_new);
      /* p33.XATO*/ wire _XATO_SPR_PIX_RST4_new = nand2(_TYTA_SPRITE_MASK4p_new, _WERY_SPR_PIX_DA4n_new);
      /* p33.XEXU*/ wire _XEXU_SPR_PIX_RST5_new = nand2(_TYCO_SPRITE_MASK5p_new, _WURA_SPR_PIX_DA5n_new);
      /* p33.TUPE*/ wire _TUPE_SPR_PIX_RST6_new = nand2(_SOKA_SPRITE_MASK6p_new, _SULU_SPR_PIX_DA6n_new);
      /* p33.XYVE*/ wire _XYVE_SPR_PIX_RST7_new = nand2(_XOVU_SPRITE_MASK7p_new, _WAMY_SPR_PIX_DA7n_new);

      /* p33.WUFY*/ pix_pipe.WUFY_SPR_PIPE_A7.SETnRSTn(_VUNE_SPR_PIX_SET7_new, _XYVE_SPR_PIX_RST7_new);
      /* p33.VAFO*/ pix_pipe.VAFO_SPR_PIPE_A6.SETnRSTn(_TUXA_SPR_PIX_SET6_new, _TUPE_SPR_PIX_RST6_new);
      /* p33.WORA*/ pix_pipe.WORA_SPR_PIPE_A5.SETnRSTn(_VABY_SPR_PIX_SET5_new, _XEXU_SPR_PIX_RST5_new);
      /* p33.WYHO*/ pix_pipe.WYHO_SPR_PIPE_A4.SETnRSTn(_VEXU_SPR_PIX_SET4_new, _XATO_SPR_PIX_RST4_new);
      /* p33.LESU*/ pix_pipe.LESU_SPR_PIPE_A3.SETnRSTn(_MAME_SPR_PIX_SET3_new, _LUFY_SPR_PIX_RST3_new);
      /* p33.LEFE*/ pix_pipe.LEFE_SPR_PIPE_A2.SETnRSTn(_LELA_SPR_PIX_SET2_new, _LYDE_SPR_PIX_RST2_new);
      /* p33.MASO*/ pix_pipe.MASO_SPR_PIPE_A1.SETnRSTn(_MYTO_SPR_PIX_SET1_new, _MADA_SPR_PIX_RST1_new);
      /* p33.NURO*/ pix_pipe.NURO_SPR_PIPE_A0.SETnRSTn(_PABE_SPR_PIX_SET0_new, _PYZU_SPR_PIX_RST0_new);
    }

    // Sprite pipe B
    {
      /* p33.MEZU*/ wire _MEZU_SPR_PIX_SET0_new = nand2(_LESY_SPRITE_MASK0p_new, vram_bus.PEFO_SPRITE_DB0n.qn_new());
      /* p33.RUSY*/ wire _RUSY_SPR_PIX_SET1_new = nand2(_LOTA_SPRITE_MASK1p_new, vram_bus.ROKA_SPRITE_DB1n.qn_new());
      /* p33.MYXA*/ wire _MYXA_SPR_PIX_SET2_new = nand2(_LYKU_SPRITE_MASK2p_new, vram_bus.MYTU_SPRITE_DB2n.qn_new());
      /* p33.RANO*/ wire _RANO_SPR_PIX_SET3_new = nand2(_ROBY_SPRITE_MASK3p_new, vram_bus.RAMU_SPRITE_DB3n.qn_new());
      /* p33.TYGA*/ wire _TYGA_SPR_PIX_SET4_new = nand2(_TYTA_SPRITE_MASK4p_new, vram_bus.SELE_SPRITE_DB4n.qn_new());
      /* p33.VUME*/ wire _VUME_SPR_PIX_SET5_new = nand2(_TYCO_SPRITE_MASK5p_new, vram_bus.SUTO_SPRITE_DB5n.qn_new());
      /* p33.TAPO*/ wire _TAPO_SPR_PIX_SET6_new = nand2(_SOKA_SPRITE_MASK6p_new, vram_bus.RAMA_SPRITE_DB6n.qn_new());
      /* p33.TESO*/ wire _TESO_SPR_PIX_SET7_new = nand2(_XOVU_SPRITE_MASK7p_new, vram_bus.RYDU_SPRITE_DB7n.qn_new());

      /* p33.LOZA*/ wire _LOZA_SPR_PIX_DB0n_new = not1(vram_bus.PEFO_SPRITE_DB0n.qn_new());
      /* p33.SYBO*/ wire _SYBO_SPR_PIX_DB1n_new = not1(vram_bus.ROKA_SPRITE_DB1n.qn_new());
      /* p33.LUMO*/ wire _LUMO_SPR_PIX_DB2n_new = not1(vram_bus.MYTU_SPRITE_DB2n.qn_new());
      /* p33.SOLO*/ wire _SOLO_SPR_PIX_DB3n_new = not1(vram_bus.RAMU_SPRITE_DB3n.qn_new());
      /* p33.VOBY*/ wire _VOBY_SPR_PIX_DB4n_new = not1(vram_bus.SELE_SPRITE_DB4n.qn_new());
      /* p33.WYCO*/ wire _WYCO_SPR_PIX_DB5n_new = not1(vram_bus.SUTO_SPRITE_DB5n.qn_new());
      /* p33.SERY*/ wire _SERY_SPR_PIX_DB6n_new = not1(vram_bus.RAMA_SPRITE_DB6n.qn_new());
      /* p33.SELU*/ wire _SELU_SPR_PIX_DB7n_new = not1(vram_bus.RYDU_SPRITE_DB7n.qn_new());

      /* p33.MOFY*/ wire _MOFY_SPR_PIX_RST0_new = nand2(_LESY_SPRITE_MASK0p_new, _LOZA_SPR_PIX_DB0n_new);
      /* p33.RUCA*/ wire _RUCA_SPR_PIX_RST1_new = nand2(_LOTA_SPRITE_MASK1p_new, _SYBO_SPR_PIX_DB1n_new);
      /* p33.MAJO*/ wire _MAJO_SPR_PIX_RST2_new = nand2(_LYKU_SPRITE_MASK2p_new, _LUMO_SPR_PIX_DB2n_new);
      /* p33.REHU*/ wire _REHU_SPR_PIX_RST3_new = nand2(_ROBY_SPRITE_MASK3p_new, _SOLO_SPR_PIX_DB3n_new);
      /* p33.WAXO*/ wire _WAXO_SPR_PIX_RST4_new = nand2(_TYTA_SPRITE_MASK4p_new, _VOBY_SPR_PIX_DB4n_new);
      /* p33.XOLE*/ wire _XOLE_SPR_PIX_RST5_new = nand2(_TYCO_SPRITE_MASK5p_new, _WYCO_SPR_PIX_DB5n_new);
      /* p33.TABY*/ wire _TABY_SPR_PIX_RST6_new = nand2(_SOKA_SPRITE_MASK6p_new, _SERY_SPR_PIX_DB6n_new);
      /* p33.TULA*/ wire _TULA_SPR_PIX_RST7_new = nand2(_XOVU_SPRITE_MASK7p_new, _SELU_SPR_PIX_DB7n_new);

      /* p33.VUPY*/ pix_pipe.VUPY_SPR_PIPE_B7.SETnRSTn(_TESO_SPR_PIX_SET7_new, _TULA_SPR_PIX_RST7_new);
      /* p33.VANU*/ pix_pipe.VANU_SPR_PIPE_B6.SETnRSTn(_TAPO_SPR_PIX_SET6_new, _TABY_SPR_PIX_RST6_new);
      /* p33.WEBA*/ pix_pipe.WEBA_SPR_PIPE_B5.SETnRSTn(_VUME_SPR_PIX_SET5_new, _XOLE_SPR_PIX_RST5_new);
      /* p33.VARE*/ pix_pipe.VARE_SPR_PIPE_B4.SETnRSTn(_TYGA_SPR_PIX_SET4_new, _WAXO_SPR_PIX_RST4_new);
      /* p33.PYJO*/ pix_pipe.PYJO_SPR_PIPE_B3.SETnRSTn(_RANO_SPR_PIX_SET3_new, _REHU_SPR_PIX_RST3_new);
      /* p33.NATY*/ pix_pipe.NATY_SPR_PIPE_B2.SETnRSTn(_MYXA_SPR_PIX_SET2_new, _MAJO_SPR_PIX_RST2_new);
      /* p33.PEFU*/ pix_pipe.PEFU_SPR_PIPE_B1.SETnRSTn(_RUSY_SPR_PIX_SET1_new, _RUCA_SPR_PIX_RST1_new);
      /* p33.NYLU*/ pix_pipe.NYLU_SPR_PIPE_B0.SETnRSTn(_MEZU_SPR_PIX_SET0_new, _MOFY_SPR_PIX_RST0_new);
    }

    {
      /* p34.PUME*/ wire _PUME_PAL_PIPE_SET0n_new = nand2(_LESY_SPRITE_MASK0p_new, oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.SORO*/ wire _SORO_PAL_PIPE_SET1n_new = nand2(_LOTA_SPRITE_MASK1p_new, oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.PAMO*/ wire _PAMO_PAL_PIPE_SET2n_new = nand2(_LYKU_SPRITE_MASK2p_new, oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.SUKY*/ wire _SUKY_PAL_PIPE_SET3n_new = nand2(_ROBY_SPRITE_MASK3p_new, oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.RORA*/ wire _RORA_PAL_PIPE_SET4n_new = nand2(_TYTA_SPRITE_MASK4p_new, oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.MENE*/ wire _MENE_PAL_PIPE_SET5n_new = nand2(_TYCO_SPRITE_MASK5p_new, oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.LUKE*/ wire _LUKE_PAL_PIPE_SET6n_new = nand2(_SOKA_SPRITE_MASK6p_new, oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.LAMY*/ wire _LAMY_PAL_PIPE_SET7n_new = nand2(_XOVU_SPRITE_MASK7p_new, oam_bus.GOMO_OAM_DB4p.qp_new());

      /* p34.SYPY*/ wire _SYPY_new = not1(oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.TOTU*/ wire _TOTU_new = not1(oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.NARO*/ wire _NARO_new = not1(oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.WEXY*/ wire _WEXY_new = not1(oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.RYZY*/ wire _RYZY_new = not1(oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.RYFE*/ wire _RYFE_new = not1(oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.LADY*/ wire _LADY_new = not1(oam_bus.GOMO_OAM_DB4p.qp_new());
      /* p34.LAFY*/ wire _LAFY_new = not1(oam_bus.GOMO_OAM_DB4p.qp_new());

      /* p34.SUCO*/ wire _SUCO_PAL_PIPE_RST0n_new = nand2(_LESY_SPRITE_MASK0p_new, _SYPY_new);
      /* p34.TAFA*/ wire _TAFA_PAL_PIPE_RST1n_new = nand2(_LOTA_SPRITE_MASK1p_new, _TOTU_new);
      /* p34.PYZY*/ wire _PYZY_PAL_PIPE_RST2n_new = nand2(_LYKU_SPRITE_MASK2p_new, _NARO_new);
      /* p34.TOWA*/ wire _TOWA_PAL_PIPE_RST3n_new = nand2(_ROBY_SPRITE_MASK3p_new, _WEXY_new);
      /* p34.RUDU*/ wire _RUDU_PAL_PIPE_RST4n_new = nand2(_TYTA_SPRITE_MASK4p_new, _RYZY_new);
      /* p34.PAZO*/ wire _PAZO_PAL_PIPE_RST5n_new = nand2(_TYCO_SPRITE_MASK5p_new, _RYFE_new);
      /* p34.LOWA*/ wire _LOWA_PAL_PIPE_RST6n_new = nand2(_SOKA_SPRITE_MASK6p_new, _LADY_new);
      /* p34.LUNU*/ wire _LUNU_PAL_PIPE_RST7n_new = nand2(_XOVU_SPRITE_MASK7p_new, _LAFY_new);

      /* p34.LYME*/ pix_pipe.LYME_PAL_PIPE_D7.SETnRSTn(_LAMY_PAL_PIPE_SET7n_new, _LUNU_PAL_PIPE_RST7n_new);
      /* p34.MODA*/ pix_pipe.MODA_PAL_PIPE_D6.SETnRSTn(_LUKE_PAL_PIPE_SET6n_new, _LOWA_PAL_PIPE_RST6n_new);
      /* p34.NUKE*/ pix_pipe.NUKE_PAL_PIPE_D5.SETnRSTn(_MENE_PAL_PIPE_SET5n_new, _PAZO_PAL_PIPE_RST5n_new);
      /* p34.PALU*/ pix_pipe.PALU_PAL_PIPE_D4.SETnRSTn(_RORA_PAL_PIPE_SET4n_new, _RUDU_PAL_PIPE_RST4n_new);
      /* p34.SOMY*/ pix_pipe.SOMY_PAL_PIPE_D3.SETnRSTn(_SUKY_PAL_PIPE_SET3n_new, _TOWA_PAL_PIPE_RST3n_new);
      /* p34.ROSA*/ pix_pipe.ROSA_PAL_PIPE_D2.SETnRSTn(_PAMO_PAL_PIPE_SET2n_new, _PYZY_PAL_PIPE_RST2n_new);
      /* p34.SATA*/ pix_pipe.SATA_PAL_PIPE_D1.SETnRSTn(_SORO_PAL_PIPE_SET1n_new, _TAFA_PAL_PIPE_RST1n_new);
      /* p34.RUGO*/ pix_pipe.RUGO_PAL_PIPE_D0.SETnRSTn(_PUME_PAL_PIPE_SET0n_new, _SUCO_PAL_PIPE_RST0n_new);
    }

    {
      /* p26.TEDE*/ wire _TEDE_MASK_PIPE_SET0_new = nand2(_LESY_SPRITE_MASK0p_new, oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.XALA*/ wire _XALA_MASK_PIPE_SET1_new = nand2(_LOTA_SPRITE_MASK1p_new, oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.TYRA*/ wire _TYRA_MASK_PIPE_SET2_new = nand2(_LYKU_SPRITE_MASK2p_new, oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.XYRU*/ wire _XYRU_MASK_PIPE_SET3_new = nand2(_ROBY_SPRITE_MASK3p_new, oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.XUKU*/ wire _XUKU_MASK_PIPE_SET4_new = nand2(_TYTA_SPRITE_MASK4p_new, oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.XELY*/ wire _XELY_MASK_PIPE_SET5_new = nand2(_TYCO_SPRITE_MASK5p_new, oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.TYKO*/ wire _TYKO_MASK_PIPE_SET6_new = nand2(_SOKA_SPRITE_MASK6p_new, oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.TUWU*/ wire _TUWU_MASK_PIPE_SET7_new = nand2(_XOVU_SPRITE_MASK7p_new, oam_bus.DEPO_OAM_DB7p.qp_new());

      /* p26.XOGA*/ wire _XOGA_new = not1(oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.XURA*/ wire _XURA_new = not1(oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.TAJO*/ wire _TAJO_new = not1(oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.XENU*/ wire _XENU_new = not1(oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.XYKE*/ wire _XYKE_new = not1(oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.XABA*/ wire _XABA_new = not1(oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.TAFU*/ wire _TAFU_new = not1(oam_bus.DEPO_OAM_DB7p.qp_new());
      /* p26.XUHO*/ wire _XUHO_new = not1(oam_bus.DEPO_OAM_DB7p.qp_new());

      /* p26.WOKA*/ wire _WOKA_MASK_PIPE_RST0_new = nand2(_LESY_SPRITE_MASK0p_new, _XOGA_new);
      /* p26.WEDE*/ wire _WEDE_MASK_PIPE_RST1_new = nand2(_LOTA_SPRITE_MASK1p_new, _XURA_new);
      /* p26.TUFO*/ wire _TUFO_MASK_PIPE_RST2_new = nand2(_LYKU_SPRITE_MASK2p_new, _TAJO_new);
      /* p26.WEVO*/ wire _WEVO_MASK_PIPE_RST3_new = nand2(_ROBY_SPRITE_MASK3p_new, _XENU_new);
      /* p26.WEDY*/ wire _WEDY_MASK_PIPE_RST4_new = nand2(_TYTA_SPRITE_MASK4p_new, _XYKE_new);
      /* p26.WUJA*/ wire _WUJA_MASK_PIPE_RST5_new = nand2(_TYCO_SPRITE_MASK5p_new, _XABA_new);
      /* p26.TENA*/ wire _TENA_MASK_PIPE_RST6_new = nand2(_SOKA_SPRITE_MASK6p_new, _TAFU_new);
      /* p26.WUBU*/ wire _WUBU_MASK_PIPE_RST7_new = nand2(_XOVU_SPRITE_MASK7p_new, _XUHO_new);

      /* p26.VAVA*/ pix_pipe.VAVA_MASK_PIPE_7.SETnRSTn(_TUWU_MASK_PIPE_SET7_new, _WUBU_MASK_PIPE_RST7_new);
      /* p26.VUMO*/ pix_pipe.VUMO_MASK_PIPE_6.SETnRSTn(_TYKO_MASK_PIPE_SET6_new, _TENA_MASK_PIPE_RST6_new);
      /* p26.WODA*/ pix_pipe.WODA_MASK_PIPE_5.SETnRSTn(_XELY_MASK_PIPE_SET5_new, _WUJA_MASK_PIPE_RST5_new);
      /* p26.XETE*/ pix_pipe.XETE_MASK_PIPE_4.SETnRSTn(_XUKU_MASK_PIPE_SET4_new, _WEDY_MASK_PIPE_RST4_new);
      /* p26.WYFU*/ pix_pipe.WYFU_MASK_PIPE_3.SETnRSTn(_XYRU_MASK_PIPE_SET3_new, _WEVO_MASK_PIPE_RST3_new);
      /* p26.VOSA*/ pix_pipe.VOSA_MASK_PIPE_2.SETnRSTn(_TYRA_MASK_PIPE_SET2_new, _TUFO_MASK_PIPE_RST2_new);
      /* p26.WURU*/ pix_pipe.WURU_MASK_PIPE_1.SETnRSTn(_XALA_MASK_PIPE_SET1_new, _WEDE_MASK_PIPE_RST1_new);
      /* p26.VEZO*/ pix_pipe.VEZO_MASK_PIPE_0.SETnRSTn(_TEDE_MASK_PIPE_SET0_new, _WOKA_MASK_PIPE_RST0_new);
    }
  }
#pragma endregion

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, _ALUR_SYS_RSTn_new, _BOGA_Axxxxxxx_clk, _BYZO_FF00_RDn_ext, _ATOZ_FF00_WRn_clk]() {
    // JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
    // That also means that _both_ P14 and P15 are selected at reset :/
    /* p05.JUTE*/ joypad.JUTE_JOYP_RA     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p05.KECY*/ joypad.KECY_JOYP_LB     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p05.JALE*/ joypad.JALE_JOYP_UC     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[2].qp_ext());
    /* p05.KYME*/ joypad.KYME_JOYP_DS     .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p05.KELY*/ joypad.KELY_JOYP_UDLRp  .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p05.COFY*/ joypad.COFY_JOYP_ABCSp  .dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p05.KUKO*/ joypad.KUKO_DBG_FF00_D6n.dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[6].qp_ext());
    /* p05.KERU*/ joypad.KERU_DBG_FF00_D7n.dff17(_ATOZ_FF00_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[7].qp_ext());

    wire BURO_FF60_0p_new = 0; // FIXME hacking out debug stuff
    /* p05.KURA*/ wire _KURA_JOYP_DBGn_new = not1(BURO_FF60_0p_new);

    /*
    // lcd ribbon voltages after bootrom
    04 5 left & b
    05 0 diodes 1&2
    06 5 down & start
    07 5 up & select
    08 5 right & a
    09 0 diodes 3 & 4
    */

    /* p05.KARU*/ wire _KARU_OEp_new = or2(joypad.KELY_JOYP_UDLRp.qn_new(), _KURA_JOYP_DBGn_new);
    /* p05.CELA*/ wire _CELA_OEp_new = or2(joypad.COFY_JOYP_ABCSp.qn_new(), _KURA_JOYP_DBGn_new);

    PIN_JOY_P14.pin_out(_KARU_OEp_new, joypad.KELY_JOYP_UDLRp.qn_new(), joypad.KELY_JOYP_UDLRp.qn_new());
    PIN_JOY_P15.pin_out(_CELA_OEp_new, joypad.COFY_JOYP_ABCSp.qn_new(), joypad.COFY_JOYP_ABCSp.qn_new());


  #if 0
    /* p05.KOLE*/ wire _KOLE = nand2(joypad.JUTE_JOYP_RA.qp17(), BURO_FF60_0p);
    /* p05.KYBU*/ wire _KYBU = nor2 (joypad.JUTE_JOYP_RA.qp17(), _KURA);
    /* p05.KYTO*/ wire _KYTO = nand2(joypad.KECY_JOYP_LB.qp17(), BURO_FF60_0p);
    /* p05.KABU*/ wire _KABU = nor2 (joypad.KECY_JOYP_LB.qp17(), _KURA);
    /* p05.KYHU*/ wire _KYHU = nand2(joypad.JALE_JOYP_UC.qp17(), BURO_FF60_0p);
    /* p05.KASY*/ wire _KASY = nor2 (joypad.JALE_JOYP_UC.qp17(), _KURA);
    /* p05.KORY*/ wire _KORY = nand2(joypad.KYME_JOYP_DS.qp17(), BURO_FF60_0p);
    /* p05.KALE*/ wire _KALE = nor2 (joypad.KYME_JOYP_DS.qp17(), _KURA);

    PIN_JOY_P10.pin_out(1, _KOLE, _KYBU);
    PIN_JOY_P11.pin_out(1, _KYTO, _KABU);
    PIN_JOY_P12.pin_out(1, _KYHU, _KASY);
    PIN_JOY_P13.pin_out(1, _KORY, _KALE);
  #endif


    if (PIN_JOY_P14.qp_ext()) {
      PIN_JOY_P10.pin_in(1, !(sys_buttons & 0x01));
      PIN_JOY_P11.pin_in(1, !(sys_buttons & 0x02));
      PIN_JOY_P12.pin_in(1, !(sys_buttons & 0x04));
      PIN_JOY_P13.pin_in(1, !(sys_buttons & 0x08));
    }
    else if (PIN_JOY_P15.qp_ext()) {
      PIN_JOY_P10.pin_in(1, !(sys_buttons & 0x10));
      PIN_JOY_P11.pin_in(1, !(sys_buttons & 0x20));
      PIN_JOY_P12.pin_in(1, !(sys_buttons & 0x40));
      PIN_JOY_P13.pin_in(1, !(sys_buttons & 0x80));
    }
    else {
      PIN_JOY_P10.pin_in(1, 1);
      PIN_JOY_P11.pin_in(1, 1);
      PIN_JOY_P12.pin_in(1, 1);
      PIN_JOY_P13.pin_in(1, 1);
    }
    /* p02.KERY*/ wire _KERY_ANY_BUTTONp_ext = or4(PIN_JOY_P13.qn_ext(), PIN_JOY_P12.qn_ext(), PIN_JOY_P11.qn_ext(), PIN_JOY_P10.qn_ext());

    {
      /* p02.AWOB*/ joypad.AWOB_WAKE_CPU.tp_latch(_BOGA_Axxxxxxx_clk, _KERY_ANY_BUTTONp_ext);
      wire _AWOB_WAKE_CPUp_new = joypad.AWOB_WAKE_CPU.qp_new();

      PIN_CPU_WAKE.pin_in(1, _AWOB_WAKE_CPUp_new);

      /* p02.APUG*/ joypad.APUG_JP_GLITCH3.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, joypad.AGEM_JP_GLITCH2.qp_old());
      /* p02.AGEM*/ joypad.AGEM_JP_GLITCH2.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, joypad.ACEF_JP_GLITCH1.qp_old());
      /* p02.ACEF*/ joypad.ACEF_JP_GLITCH1.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, joypad.BATU_JP_GLITCH0.qp_old());
      /* p02.BATU*/ joypad.BATU_JP_GLITCH0.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, _KERY_ANY_BUTTONp_ext);

      /* p05.KEVU*/ joypad.KEVU_JOYP_L0n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P10.qn_ext());
      /* p05.KAPA*/ joypad.KAPA_JOYP_L1n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P11.qn_ext());
      /* p05.KEJA*/ joypad.KEJA_JOYP_L2n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P12.qn_ext());
      /* p05.KOLO*/ joypad.KOLO_JOYP_L3n.tp_latch(_BYZO_FF00_RDn_ext, PIN_JOY_P13.qn_ext());
    }
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, _ALUR_SYS_RSTn_new, _UFEG_FF01_RDp_ext, _URYS_FF01_WRn_clk, _UCOM_FF02_RDp_ext, _UWAM_FF02_WRn_clk]() {
    /* p06.CAVE*/ wire _CAVE_SER_clkold = mux2n(ser_reg.CULY_XFER_DIR.qp_old(), ser_reg.COTY_SER_CLK.qp_old(), PIN_SCK.qn_old());
    /* p06.DAWA*/ wire _DAWA_SER_clkold = or2(_CAVE_SER_clkold, ser_reg.ETAF_SER_RUNNING.qn_old()); // this must stop the clock or something when the transmit's done

    //----------
    // Serial pins

    /* hack */    PIN_SIN.pin_out(1, 1, 1);

    /* p06.CULY*/ ser_reg.CULY_XFER_DIR.dff17(_UWAM_FF02_WRn_clk, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());

    /* p01.UVYN*/ wire _UVYN_DIV_05n_next = not1(tim_reg.TAMA_DIV05p.qp_new());
    /* p06.COTY*/ ser_reg.COTY_SER_CLK.dff17(_UVYN_DIV_05n_next, _UWAM_FF02_WRn_clk, ser_reg.COTY_SER_CLK.qn_old());

    {
      /* p06.KEXU*/ wire _KEXU_new = nand2(ser_reg.COTY_SER_CLK.qp_new(), ser_reg.CULY_XFER_DIR.qp_new());
      /* p06.JAGO*/ wire _JAGO_new = not1(ser_reg.CULY_XFER_DIR.qp_new());
      /* p06.KUJO*/ wire _KUJO_new = nor2(ser_reg.COTY_SER_CLK.qp_new(), _JAGO_new);

      /* p06.KEXU*/ PIN_SCK.pin_out(ser_reg.CULY_XFER_DIR.qp_new(), _KEXU_new, _KUJO_new);
    }

    /* p06.DAKU*/ wire _DAKU_FF01_WRp_clk = not1(_URYS_FF01_WRn_clk);
    /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn_new = nand2(BUS_CPU_Dp_ext[0].qp_ext(), _DAKU_FF01_WRp_clk);
    /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn_new = nand2(BUS_CPU_Dp_ext[1].qp_ext(), _DAKU_FF01_WRp_clk);
    /* p06.DELA*/ wire _DELA_SER_DATA2_SETn_new = nand2(BUS_CPU_Dp_ext[2].qp_ext(), _DAKU_FF01_WRp_clk);
    /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn_new = nand2(BUS_CPU_Dp_ext[3].qp_ext(), _DAKU_FF01_WRp_clk);
    /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn_new = nand2(BUS_CPU_Dp_ext[4].qp_ext(), _DAKU_FF01_WRp_clk);
    /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn_new = nand2(BUS_CPU_Dp_ext[5].qp_ext(), _DAKU_FF01_WRp_clk);
    /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn_new = nand2(BUS_CPU_Dp_ext[6].qp_ext(), _DAKU_FF01_WRp_clk);
    /* p06.EFEF*/ wire _EFEL_SER_DATA7_SETn_new = nand2(BUS_CPU_Dp_ext[7].qp_ext(), _DAKU_FF01_WRp_clk);

    /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_Dp_ext[0].qp_ext(), _ALUR_SYS_RSTn_new);
    /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_Dp_ext[1].qp_ext(), _ALUR_SYS_RSTn_new);
    /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_Dp_ext[2].qp_ext(), _ALUR_SYS_RSTn_new);
    /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_Dp_ext[3].qp_ext(), _ALUR_SYS_RSTn_new);
    /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_Dp_ext[4].qp_ext(), _ALUR_SYS_RSTn_new);
    /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_Dp_ext[5].qp_ext(), _ALUR_SYS_RSTn_new);
    /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_Dp_ext[6].qp_ext(), _ALUR_SYS_RSTn_new);
    /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_Dp_ext[7].qp_ext(), _ALUR_SYS_RSTn_new);

    /* p06.CARO*/ wire _CARO_SER_RST_new = and2(_UWAM_FF02_WRn_clk, _ALUR_SYS_RSTn_new);
    /* p06.CAFA*/ ser_reg.CAFA_SER_CNT0.dff17(_DAWA_SER_clkold,               _CARO_SER_RST_new, ser_reg.CAFA_SER_CNT0.qn_old());
    /* p06.CYLO*/ ser_reg.CYLO_SER_CNT1.dff17(ser_reg.CAFA_SER_CNT0.qn_new(), _CARO_SER_RST_new, ser_reg.CYLO_SER_CNT1.qn_old());
    /* p06.CYDE*/ ser_reg.CYDE_SER_CNT2.dff17(ser_reg.CYLO_SER_CNT1.qn_new(), _CARO_SER_RST_new, ser_reg.CYDE_SER_CNT2.qn_old());
    /* p06.CALY*/ ser_reg.CALY_SER_CNT3.dff17(ser_reg.CYDE_SER_CNT2.qn_new(), _CARO_SER_RST_new, ser_reg.CALY_SER_CNT3.qn_old());

    /* p06.COBA*/ wire _COBA_SER_CNT3n_new  = not1(ser_reg.CALY_SER_CNT3.qp_new());
    /* p06.CABY*/ wire _CABY_XFER_RESET_new = and2(_COBA_SER_CNT3n_new, _ALUR_SYS_RSTn_new);
    /* p06.ETAF*/ ser_reg.ETAF_SER_RUNNING.dff17(_UWAM_FF02_WRn_clk, _CABY_XFER_RESET_new, BUS_CPU_Dp_ext[7].qp_ext());

    /* p06.CAVE*/ wire _CAVE_SER_clknew = mux2n(ser_reg.CULY_XFER_DIR.qp_new(), ser_reg.COTY_SER_CLK.qp_new(), PIN_SCK.qn_new());
    /* p06.DAWA*/ wire _DAWA_SER_clknew = or2(_CAVE_SER_clknew, ser_reg.ETAF_SER_RUNNING.qn_new()); // this must stop the clock or something when the transmit's done

    /* p06.CAFA*/ ser_reg.CAFA_SER_CNT0.dff17(_DAWA_SER_clknew,               _CARO_SER_RST_new, ser_reg.CAFA_SER_CNT0.qn_new());
    /* p06.CYLO*/ ser_reg.CYLO_SER_CNT1.dff17(ser_reg.CAFA_SER_CNT0.qn_new(), _CARO_SER_RST_new, ser_reg.CYLO_SER_CNT1.qn_new());
    /* p06.CYDE*/ ser_reg.CYDE_SER_CNT2.dff17(ser_reg.CYLO_SER_CNT1.qn_new(), _CARO_SER_RST_new, ser_reg.CYDE_SER_CNT2.qn_new());
    /* p06.CALY*/ ser_reg.CALY_SER_CNT3.dff17(ser_reg.CYDE_SER_CNT2.qn_new(), _CARO_SER_RST_new, ser_reg.CALY_SER_CNT3.qn_new());

    /* p06.CAGE*/ wire _CAGE_SER_IN_new  = not1(PIN_SIN.qn_new());
    /* p06.EDYL*/ wire _EDYL_SER_CLK_new = not1(_DAWA_SER_clknew);
    /* p06.EPYT*/ wire _EPYT_SER_CLK_new = not1(_EDYL_SER_CLK_new);
    /* p06.DEHO*/ wire _DEHO_SER_CLK_new = not1(_EPYT_SER_CLK_new);
    /* p06.DAWE*/ wire _DAWE_SER_CLK_new = not1(_DEHO_SER_CLK_new);

    /* p06.ELYS*/ ser_reg.ELYS_SER_OUT  .dff17(_EDYL_SER_CLK_new, _ALUR_SYS_RSTn_new,                                 ser_reg.EDER_SER_DATA7.qp_old());
    /* p06.EDER*/ ser_reg.EDER_SER_DATA7.dff22(_EPYT_SER_CLK_new, _EFEL_SER_DATA7_SETn_new, _EGUV_SER_DATA7_RSTn_new, ser_reg.EROD_SER_DATA6.qp_old());
    /* p06.EROD*/ ser_reg.EROD_SER_DATA6.dff22(_EPYT_SER_CLK_new, _EDEL_SER_DATA6_SETn_new, _EFAK_SER_DATA6_RSTn_new, ser_reg.EJAB_SER_DATA5.qp_old());
    /* p06.EJAB*/ ser_reg.EJAB_SER_DATA5.dff22(_EPYT_SER_CLK_new, _ELOK_SER_DATA5_SETn_new, _EHUJ_SER_DATA5_RSTn_new, ser_reg.DOVU_SER_DATA4.qp_old());
    /* p06.DOVU*/ ser_reg.DOVU_SER_DATA4.dff22(_EPYT_SER_CLK_new, _DOLA_SER_DATA4_SETn_new, _DYLY_SER_DATA4_RSTn_new, ser_reg.DOJO_SER_DATA3.qp_old());
    /* p06.DOJO*/ ser_reg.DOJO_SER_DATA3.dff22(_DAWE_SER_CLK_new, _DYGE_SER_DATA3_SETn_new, _DAJU_SER_DATA3_RSTn_new, ser_reg.DYRA_SER_DATA2.qp_old());
    /* p06.DYRA*/ ser_reg.DYRA_SER_DATA2.dff22(_DAWE_SER_CLK_new, _DELA_SER_DATA2_SETn_new, _DYBO_SER_DATA2_RSTn_new, ser_reg.DEGU_SER_DATA1.qp_old());
    /* p06.DEGU*/ ser_reg.DEGU_SER_DATA1.dff22(_DAWE_SER_CLK_new, _DOCU_SER_DATA1_SETn_new, _DUMO_SER_DATA1_RSTn_new, ser_reg.CUBA_SER_DATA0.qp_old());
    /* p06.CUBA*/ ser_reg.CUBA_SER_DATA0.dff22(_DAWE_SER_CLK_new, _CUFU_SER_DATA0_SETn_new, _COHY_SER_DATA0_RSTn_new, _CAGE_SER_IN_new);

    // FIXME hacking out debug stuff
    ///* p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);
    /* p05.KENA*/ PIN_SOUT.pin_out(1, ser_reg.ELYS_SER_OUT.qp_new(), ser_reg.ELYS_SER_OUT.qp_new());

    /* FF01 SER */
    /*#p06.CUGY*/ BUS_CPU_Dp_out[0].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.CUBA_SER_DATA0.qn_new());
    /* p06.DUDE*/ BUS_CPU_Dp_out[1].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.DEGU_SER_DATA1.qn_new());
    /* p06.DETU*/ BUS_CPU_Dp_out[2].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.DYRA_SER_DATA2.qn_new());
    /* p06.DASO*/ BUS_CPU_Dp_out[3].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.DOJO_SER_DATA3.qn_new());
    /* p06.DAME*/ BUS_CPU_Dp_out[4].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.DOVU_SER_DATA4.qn_new());
    /* p06.EVOK*/ BUS_CPU_Dp_out[5].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.EJAB_SER_DATA5.qn_new());
    /* p06.EFAB*/ BUS_CPU_Dp_out[6].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.EROD_SER_DATA6.qn_new());
    /*#p06.ETAK*/ BUS_CPU_Dp_out[7].tri6_pn(_UFEG_FF01_RDp_ext, ser_reg.EDER_SER_DATA7.qn_new());

    /* FF02 SER */
    /* p06.CORE*/ BUS_CPU_Dp_out[0].tri6_pn(_UCOM_FF02_RDp_ext, ser_reg.CULY_XFER_DIR.qn_new());
    /* p06.ELUV*/ BUS_CPU_Dp_out[7].tri6_pn(_UCOM_FF02_RDp_ext, ser_reg.ETAF_SER_RUNNING.qn_new());
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




  /*#p28.GEKA*/ wire _GEKA_OAM_A0p_new     = not1(BUS_OAM_An[0].qp_old());
  /* p28.WAFO*/ wire _WAFO_OAM_A0n_new     = not1(_GEKA_OAM_A0p_new);


#pragma region VRAM_write
  {
    /* CBD -> VBD */
    /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY_new = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /* p29.TEPA*/ wire _TEPA_RENDERINGn_new = not1(pix_pipe.XYMU_RENDERINGn.qn_new());
    /* p29.TYSO*/ wire _TYSO_SPRITE_READn_new = or2(_SAKY_SFETCH_MYSTERY_new, _TEPA_RENDERINGn_new); // def or
    /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp_new = not1(_TYSO_SPRITE_READn_new);

    /*#p25.ROPY*/ wire _ROPY_RENDERINGn_new = not1(pix_pipe.XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp_new  = and2(_TOLE_CPU_VRAM_RDp_ext, _ROPY_RENDERINGn_new);
    /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp_new = not1(_SALE_CPU_VRAM_WRn_ext);
    /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp_new  = and2(_SERE_CPU_VRM_RDp_new, _RUVY_CPU_VRAM_WRp_new);
    /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn_new  = not1(_SAZO_CBD_TO_VPDp_new);
    /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp_new  = not1(_RYJE_CBD_TO_VPDn_new);
    /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp_new  = or2(_REVO_CBD_TO_VPDp_new, _SAZO_CBD_TO_VPDp_new);
    /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn_new  = not1(_RELA_CBD_TO_VPDp_new);

    /* p25.ROCY*/ wire _ROCY_CBD_TO_VPDp_new = and2(_REVO_CBD_TO_VPDp_new, _SAZO_CBD_TO_VPDp_new);
    /* p25.RAHU*/ wire _RAHU_CBD_TO_VPDn_new = not1(_ROCY_CBD_TO_VPDp_new);
    /*#p25.ROVE*/ wire _ROVE_CBD_TO_VPDp_new = not1(_RAHU_CBD_TO_VPDn_new);

    {
      BUS_VRAM_Dp_out[0].reset();
      BUS_VRAM_Dp_out[1].reset();
      BUS_VRAM_Dp_out[2].reset();
      BUS_VRAM_Dp_out[3].reset();
      BUS_VRAM_Dp_out[4].reset();
      BUS_VRAM_Dp_out[5].reset();
      BUS_VRAM_Dp_out[6].reset();
      BUS_VRAM_Dp_out[7].reset();

      /* p25.TEME*/ BUS_VRAM_Dp_out[0].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_Dp_ext[0].qp_ext());
      /* p25.TEWU*/ BUS_VRAM_Dp_out[1].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_Dp_ext[1].qp_ext());
      /*#p25.TYGO*/ BUS_VRAM_Dp_out[2].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_Dp_ext[2].qp_ext());
      /* p25.SOTE*/ BUS_VRAM_Dp_out[3].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_Dp_ext[3].qp_ext());
      /* p25.SEKE*/ BUS_VRAM_Dp_out[4].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_Dp_ext[4].qp_ext());
      /* p25.RUJO*/ BUS_VRAM_Dp_out[5].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_Dp_ext[5].qp_ext());
      /* p25.TOFA*/ BUS_VRAM_Dp_out[6].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_Dp_ext[6].qp_ext());
      /* p25.SUZA*/ BUS_VRAM_Dp_out[7].tri10_np(_RAHU_CBD_TO_VPDn_new, BUS_CPU_Dp_ext[7].qp_ext());
    }


    {
      /* p27.LUSU*/ wire _LUSU_BGW_VRAM_RDn_new = not1(tile_fetcher.LONY_FETCHINGp.qp_new());
      /* p27.LENA*/ wire _LENA_BGW_VRAM_RDp_new = not1(_LUSU_BGW_VRAM_RDn_new);
      /*#p25.SUTU*/ wire _SUTU_MCSn_new = nor4(_LENA_BGW_VRAM_RDp_new, _LUFA_DMA_VRAMp_new, _TEXY_SPR_READ_VRAMp_new, _SERE_CPU_VRM_RDp_new);
      /* p25.TUTO*/ wire _TUTO_DBG_VRAMp_new = and2(_UNOR_MODE_DBG2p_ext, SOTO_DBG_VRAMp.qn_new());
      /*#p25.RACO*/ wire _RACO_DBG_VRAMn_new = not1(_TUTO_DBG_VRAMp_new);
      /* p25.TODE*/ wire _TODE_MCSn_A_new = and2(_SUTU_MCSn_new, _RACO_DBG_VRAMn_new);
      /* p25.SEWO*/ wire _SEWO_MCSn_D_new = or2(_SUTU_MCSn_new, _TUTO_DBG_VRAMp_new);
      /* p25.SOKY*/ wire _SOKY_MCSp_A_new = not1(_TODE_MCSn_A_new);
      /* p25.SETY*/ wire _SETY_MCSp_D_new = not1(_SEWO_MCSn_D_new);

      // FIXME not actually using this pin, but we should
      PIN_VRAM_CSn.pin_out(1, _SOKY_MCSp_A_new, _SETY_MCSp_D_new);
    }

    {
      /* p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp_ext = and2(_SOSE_8000_9FFFp_ext, _APOV_CPU_WRp_xxxxEFGx_clk);
      /* p25.SUDO*/ wire _SUDO_MWRp_new = not1(/*vram_bus.PIN_VRAM_WRn.qn_new()*/ 1); // Ignoring debug stuff for now
      /* p25.TUTO*/ wire _TUTO_DBG_VRAMp_new = and2(_UNOR_MODE_DBG2p_ext, SOTO_DBG_VRAMp.qn_new());
      /*#p25.RACO*/ wire _RACO_DBG_VRAMn_new = not1(_TUTO_DBG_VRAMp_new);
      /* p25.TYJY*/ wire _TYJY_VRAM_WRp_new = mux2p(_TUTO_DBG_VRAMp_new, _SUDO_MWRp_new, _TUJA_CPU_VRAM_WRp_ext);
      /* p25.SOHY*/ wire _SOHY_MWRn_new = nand2(_TYJY_VRAM_WRp_new, _SERE_CPU_VRM_RDp_new);
      /* p25.TAXY*/ wire _TAXY_MWRn_A_new = and2(_SOHY_MWRn_new, _RACO_DBG_VRAMn_new);
      /* p25.SOFY*/ wire _SOFY_MWRn_D_new = or2(_SOHY_MWRn_new, _TUTO_DBG_VRAMp_new);
      /* p25.SYSY*/ wire _SYSY_MWRp_A_new = not1(_TAXY_MWRn_A_new);
      /* p25.RAGU*/ wire _RAGU_MWRp_D_new = not1(_SOFY_MWRn_D_new);

      PIN_VRAM_WRn.pin_out(1, _SYSY_MWRp_A_new, _RAGU_MWRp_D_new);


      /*#p25.SEFA*/ wire _SEFA_D0p_new = and2(BUS_VRAM_Dp_out[0].qp_ext(), _ROVE_CBD_TO_VPDp_new);
      /* p25.SOGO*/ wire _SOGO_D1p_new = and2(BUS_VRAM_Dp_out[1].qp_ext(), _ROVE_CBD_TO_VPDp_new);
      /* p25.SEFU*/ wire _SEFU_D2p_new = and2(BUS_VRAM_Dp_out[2].qp_ext(), _ROVE_CBD_TO_VPDp_new);
      /* p25.SUNA*/ wire _SUNA_D3p_new = and2(BUS_VRAM_Dp_out[3].qp_ext(), _ROVE_CBD_TO_VPDp_new);
      /* p25.SUMO*/ wire _SUMO_D4p_new = and2(BUS_VRAM_Dp_out[4].qp_ext(), _ROVE_CBD_TO_VPDp_new);
      /* p25.SAZU*/ wire _SAZU_D5p_new = and2(BUS_VRAM_Dp_out[5].qp_ext(), _ROVE_CBD_TO_VPDp_new);
      /* p25.SAMO*/ wire _SAMO_D6p_new = and2(BUS_VRAM_Dp_out[6].qp_ext(), _ROVE_CBD_TO_VPDp_new);
      /* p25.SUKE*/ wire _SUKE_D7p_new = and2(BUS_VRAM_Dp_out[7].qp_ext(), _ROVE_CBD_TO_VPDp_new);

      /*#p25.SYNU*/ wire _SYNU_D0p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp_out[0].qp_ext());
      /* p25.SYMA*/ wire _SYMA_D1p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp_out[1].qp_ext());
      /* p25.ROKO*/ wire _ROKO_D2p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp_out[2].qp_ext());
      /* p25.SYBU*/ wire _SYBU_D3p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp_out[3].qp_ext());
      /* p25.SAKO*/ wire _SAKO_D4p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp_out[4].qp_ext());
      /* p25.SEJY*/ wire _SEJY_D5p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp_out[5].qp_ext());
      /* p25.SEDO*/ wire _SEDO_D6p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp_out[6].qp_ext());
      /* p25.SAWU*/ wire _SAWU_D7p_new = or2(_RAHU_CBD_TO_VPDn_new, BUS_VRAM_Dp_out[7].qp_ext());

      /*#p25.REGE*/ wire _REGE_D0n_new = not1(_SEFA_D0p_new);
      /* p25.RYKY*/ wire _RYKY_D1n_new = not1(_SOGO_D1p_new);
      /* p25.RAZO*/ wire _RAZO_D2n_new = not1(_SEFU_D2p_new);
      /* p25.RADA*/ wire _RADA_D3n_new = not1(_SUNA_D3p_new);
      /* p25.RYRO*/ wire _RYRO_D4n_new = not1(_SUMO_D4p_new);
      /* p25.REVU*/ wire _REVU_D5n_new = not1(_SAZU_D5p_new);
      /* p25.REKU*/ wire _REKU_D6n_new = not1(_SAMO_D6p_new);
      /* p25.RYZE*/ wire _RYZE_D7n_new = not1(_SUKE_D7p_new);

      /*#p25.RURA*/ wire _RURA_D0n_new = not1(_SYNU_D0p_new);
      /* p25.RULY*/ wire _RULY_D1n_new = not1(_SYMA_D1p_new);
      /* p25.RARE*/ wire _RARE_D2n_new = not1(_ROKO_D2p_new);
      /* p25.RODU*/ wire _RODU_D3n_new = not1(_SYBU_D3p_new);
      /* p25.RUBE*/ wire _RUBE_D4n_new = not1(_SAKO_D4p_new);
      /* p25.RUMU*/ wire _RUMU_D5n_new = not1(_SEJY_D5p_new);
      /* p25.RYTY*/ wire _RYTY_D6n_new = not1(_SEDO_D6p_new);
      /* p25.RADY*/ wire _RADY_D7n_new = not1(_SAWU_D7p_new);

      /*#p25.ROFA*/ wire _ROFA_CBD_TO_VPDp_new = not1(_RENA_CBD_TO_VPDn_new);

      PIN_VRAM_Dp_out[0].reset();
      PIN_VRAM_Dp_out[1].reset();
      PIN_VRAM_Dp_out[2].reset();
      PIN_VRAM_Dp_out[3].reset();
      PIN_VRAM_Dp_out[4].reset();
      PIN_VRAM_Dp_out[5].reset();
      PIN_VRAM_Dp_out[6].reset();
      PIN_VRAM_Dp_out[7].reset();

      PIN_VRAM_Dp_out[0].pin_out(_ROFA_CBD_TO_VPDp_new, _REGE_D0n_new, _RURA_D0n_new);
      PIN_VRAM_Dp_out[1].pin_out(_ROFA_CBD_TO_VPDp_new, _RYKY_D1n_new, _RULY_D1n_new);
      PIN_VRAM_Dp_out[2].pin_out(_ROFA_CBD_TO_VPDp_new, _RAZO_D2n_new, _RARE_D2n_new);
      PIN_VRAM_Dp_out[3].pin_out(_ROFA_CBD_TO_VPDp_new, _RADA_D3n_new, _RODU_D3n_new);
      PIN_VRAM_Dp_out[4].pin_out(_ROFA_CBD_TO_VPDp_new, _RYRO_D4n_new, _RUBE_D4n_new);
      PIN_VRAM_Dp_out[5].pin_out(_ROFA_CBD_TO_VPDp_new, _REVU_D5n_new, _RUMU_D5n_new);
      PIN_VRAM_Dp_out[6].pin_out(_ROFA_CBD_TO_VPDp_new, _REKU_D6n_new, _RYTY_D6n_new);
      PIN_VRAM_Dp_out[7].pin_out(_ROFA_CBD_TO_VPDp_new, _RYZE_D7n_new, _RADY_D7n_new);
    }
  }
#pragma endregion

  //------------------------------------------------------------------------------

  /* LCD Pixel Pipe */
  {
    {
      // Pixel merge + emit
      /*#p35.RAJY*/ wire RAJY_PIX_BG_LOp_new  = and2(pix_pipe.PYBO_BGW_PIPE_A7.qp_new(), pix_pipe.VYXE_LCDC_BGENn.qn_new());
      /*#p35.TADE*/ wire TADE_PIX_BG_HIp_new  = and2(pix_pipe.SOHU_BGW_PIPE_B7.qp_new(), pix_pipe.VYXE_LCDC_BGENn.qn_new());
      /*#p35.XULA*/ wire XULA_PIX_SP_LOp_new  = and2(pix_pipe.XYLO_LCDC_SPENn.qn_new(), pix_pipe.WUFY_SPR_PIPE_A7.qp_new());
      /*#p35.WOXA*/ wire WOXA_PIX_SP_HIp_new  = and2(pix_pipe.XYLO_LCDC_SPENn.qn_new(), pix_pipe.VUPY_SPR_PIPE_B7.qp_new());

      /*#p35.NULY*/ wire NULY_PIX_SP_MASKn_new = nor2(WOXA_PIX_SP_HIp_new, XULA_PIX_SP_LOp_new);

      /*#p35.RYFU*/ wire RYFU_MASK_BG0_new = and2(RAJY_PIX_BG_LOp_new, pix_pipe.VAVA_MASK_PIPE_7.qp_new());
      /*#p35.RUTA*/ wire RUTA_MASK_BG1_new = and2(TADE_PIX_BG_HIp_new, pix_pipe.VAVA_MASK_PIPE_7.qp_new());
      /*#p35.POKA*/ wire POKA_BGPIXELn_new = nor3(NULY_PIX_SP_MASKn_new, RUTA_MASK_BG1_new, RYFU_MASK_BG0_new);

      /*#p34.LOME*/ wire LOME_PAL_PIPE_7n_new = not1(pix_pipe.LYME_PAL_PIPE_D7.qp_new());
      /*#p34.LAFU*/ wire LAFU_OBP0PIXELn_new = nand2(LOME_PAL_PIPE_7n_new, POKA_BGPIXELn_new);
      /*#p34.LEKA*/ wire LEKA_OBP1PIXELn_new = nand2(pix_pipe.LYME_PAL_PIPE_D7.qp_new(), POKA_BGPIXELn_new);

      //----------
      // Sprite palette 0 lookup

      /*#p35.WELE*/ wire WELE_PIX_SP_LOn_new = not1(XULA_PIX_SP_LOp_new);
      /*#p35.WOLO*/ wire WOLO_PIX_SP_LOp_new = not1(WELE_PIX_SP_LOn_new);
      /*#p35.VUMU*/ wire VUMU_PIX_SP_HIn_new = not1(WOXA_PIX_SP_HIp_new);
      /*#p35.WYRU*/ wire WYRU_PIX_SP_HIp_new = not1(VUMU_PIX_SP_HIn_new);

      /*#p35.LAVA*/ wire LAVA_MASK_OPB0_new  = not1(LAFU_OBP0PIXELn_new);

      /*#p35.VUGO*/ wire VUGO_PAL_OBP0A_new = and3(VUMU_PIX_SP_HIn_new, WELE_PIX_SP_LOn_new, LAVA_MASK_OPB0_new); // does not have vcc arm
      /*#p35.VOLO*/ wire VOLO_PAL_OBP0B_new = and3(VUMU_PIX_SP_HIn_new, WOLO_PIX_SP_LOp_new, LAVA_MASK_OPB0_new); // does not have vcc arm
      /*#p35.VATA*/ wire VATA_PAL_OBP0C_new = and3(WYRU_PIX_SP_HIp_new, WELE_PIX_SP_LOn_new, LAVA_MASK_OPB0_new); // does not have vcc arm
      /*#p35.VYRO*/ wire VYRO_PAL_OBP0D_new = and3(WYRU_PIX_SP_HIp_new, WOLO_PIX_SP_LOp_new, LAVA_MASK_OPB0_new); // does not have vcc arm

      /*#p35.WUFU*/ wire WUFU_COL_OBP0_HI_new = amux4(pix_pipe.XANA_OBP0_D7n.qn_new(), VYRO_PAL_OBP0D_new,
                                                      pix_pipe.XYZE_OBP0_D5n.qn_new(), VATA_PAL_OBP0C_new,
                                                      pix_pipe.XALO_OBP0_D3n.qn_new(), VOLO_PAL_OBP0B_new,
                                                      pix_pipe.XUKY_OBP0_D1n.qn_new(), VUGO_PAL_OBP0A_new);

      /*#p35.WALY*/ wire WALY_COL_OBP0_LO_new = amux4(pix_pipe.XUPO_OBP0_D6n.qn_new(), VYRO_PAL_OBP0D_new,
                                                      pix_pipe.XERU_OBP0_D4n.qn_new(), VATA_PAL_OBP0C_new,
                                                      pix_pipe.XOVA_OBP0_D2n.qn_new(), VOLO_PAL_OBP0B_new,
                                                      pix_pipe.XUFU_OBP0_D0n.qn_new(), VUGO_PAL_OBP0A_new);

      //----------
      // Sprite palette 1 lookup

      /*#p35.MABY*/ wire MABY_PIX_SP_LOn_new = not1(XULA_PIX_SP_LOp_new);
      /*#p35.LYLE*/ wire LYLE_PIX_SP_LOp_new = not1(MABY_PIX_SP_LOn_new);
      /*#p35.MEXA*/ wire MEXA_PIX_SP_HIn_new = not1(WOXA_PIX_SP_HIp_new);
      /*#p35.LOZO*/ wire LOZO_PIX_SP_HIp_new = not1(MEXA_PIX_SP_HIn_new);

      /*#p35.LUKU*/ wire LUKU_MASK_OBP1_new  = not1(LEKA_OBP1PIXELn_new);

      /* p#35.LOPU*/ wire LOPU_PAL_OBP1A_new = and3(MEXA_PIX_SP_HIn_new, MABY_PIX_SP_LOn_new, LUKU_MASK_OBP1_new); // does not have vcc arm
      /* p#35.LYKY*/ wire LYKY_PAL_OBP1B_new = and3(MEXA_PIX_SP_HIn_new, LYLE_PIX_SP_LOp_new, LUKU_MASK_OBP1_new); // does not have vcc arm
      /* p#35.LARU*/ wire LARU_PAL_OBP1C_new = and3(LOZO_PIX_SP_HIp_new, MABY_PIX_SP_LOn_new, LUKU_MASK_OBP1_new); // does not have vcc arm
      /* p#35.LEDO*/ wire LEDO_PAL_OBP1D_new = and3(LOZO_PIX_SP_HIp_new, LYLE_PIX_SP_LOp_new, LUKU_MASK_OBP1_new); // does not have vcc arm

      /*#p35.MOKA*/ wire MOKA_COL_OBP1_HI_new = amux4(pix_pipe.LUXO_OBP1_D7n.qn_new(), LEDO_PAL_OBP1D_new,
                                                      pix_pipe.LUGU_OBP1_D5n.qn_new(), LARU_PAL_OBP1C_new,
                                                      pix_pipe.LOSE_OBP1_D3n.qn_new(), LYKY_PAL_OBP1B_new,
                                                      pix_pipe.LAWO_OBP1_D1n.qn_new(), LOPU_PAL_OBP1A_new);

      /*#p35.MUFA*/ wire MUFA_COL_OBP1_LO_new = amux4(LEDO_PAL_OBP1D_new, pix_pipe.LEPU_OBP1_D6n.qn_new(),
                                                      LARU_PAL_OBP1C_new, pix_pipe.LUNE_OBP1_D4n.qn_new(),
                                                      LYKY_PAL_OBP1B_new, pix_pipe.MOSA_OBP1_D2n.qn_new(),
                                                      LOPU_PAL_OBP1A_new, pix_pipe.MOXY_OBP1_D0n.qn_new());

      //----------
      // Background/window palette lookup

      /* p35.SOBA*/ wire SOBA_PIX_BG_LOn_new = not1(RAJY_PIX_BG_LOp_new);
      /* p35.NUPO*/ wire NUPO_PIX_BG_LOp_new = not1(SOBA_PIX_BG_LOn_new);
      /* p35.VYCO*/ wire VYCO_PIX_BG_HIn_new = not1(TADE_PIX_BG_HIp_new);
      /* p35.NALE*/ wire NALE_PIX_BG_HIp_new = not1(VYCO_PIX_BG_HIn_new);

      /* p35.MUVE*/ wire MUVE_MASK_BGP_new = not1(POKA_BGPIXELn_new);

      /* p35.POBU*/ wire POBU_PAL_BGPA_new = and3(VYCO_PIX_BG_HIn_new, SOBA_PIX_BG_LOn_new, MUVE_MASK_BGP_new); // does not have vcc arm
      /* p35.NUXO*/ wire NUXO_PAL_BGPB_new = and3(VYCO_PIX_BG_HIn_new, NUPO_PIX_BG_LOp_new, MUVE_MASK_BGP_new); // does not have vcc arm
      /* p35.NUMA*/ wire NUMA_PAL_BGPC_new = and3(NALE_PIX_BG_HIp_new, SOBA_PIX_BG_LOn_new, MUVE_MASK_BGP_new); // does not have vcc arm
      /* p35.NYPO*/ wire NYPO_PAL_BGPD_new = and3(NALE_PIX_BG_HIp_new, NUPO_PIX_BG_LOp_new, MUVE_MASK_BGP_new); // does not have vcc arm

      /*#p35.NELO*/ wire NELO_COL_BG_LO_new = amux4(NYPO_PAL_BGPD_new, pix_pipe.MOGY_BGP_D6n.qn_new(),
                                                    NUMA_PAL_BGPC_new, pix_pipe.MUKE_BGP_D4n.qn_new(),
                                                    NUXO_PAL_BGPB_new, pix_pipe.PYLU_BGP_D2n.qn_new(),
                                                    POBU_PAL_BGPA_new, pix_pipe.PAVO_BGP_D0n.qn_new());

      /*#p35.NURA*/ wire NURA_COL_BG_HI_new = amux4(pix_pipe.MENA_BGP_D7n.qn_new(), NYPO_PAL_BGPD_new,
                                                    pix_pipe.MORU_BGP_D5n.qn_new(), NUMA_PAL_BGPC_new,
                                                    pix_pipe.MAXY_BGP_D3n.qn_new(), NUXO_PAL_BGPB_new,
                                                    pix_pipe.NUSY_BGP_D1n.qn_new(), POBU_PAL_BGPA_new);

      //----------
      // Pixel merge and send

      /*#p35.PERO*/ wire _PERO_COL_LO_new = or3(NELO_COL_BG_LO_new, WALY_COL_OBP0_LO_new, MUFA_COL_OBP1_LO_new);
      /*#p35.PATY*/ wire _PATY_COL_HI_new = or3(NURA_COL_BG_HI_new, WUFU_COL_OBP0_HI_new, MOKA_COL_OBP1_HI_new);

      /*#p35.REMY*/ wire _REMY_LD0n_new = not1(_PERO_COL_LO_new);
      /*#p35.RAVO*/ wire _RAVO_LD1n_new = not1(_PATY_COL_HI_new);

      //lcd_data0_delay.set(_REMY_LD0n);
      //lcd_data1_delay.set(_RAVO_LD1n);

      // so q1 works but q2 has tiny errors? wat?
      PIN_LCD_DATA0.pin_out(1, /*lcd_data0_delay.q1_new()*/ _REMY_LD0n_new, /*lcd_data0_delay.q1_new()*/ _REMY_LD0n_new);
      PIN_LCD_DATA1.pin_out(1, /*lcd_data1_delay.q1_new()*/ _RAVO_LD1n_new, /*lcd_data1_delay.q1_new()*/ _RAVO_LD1n_new);
    }

    // LCD horizontal sync pin latch

    // FIXME inversion
    // I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
    // seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
    // Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
    // That would be a loooot of gate delay.
    // Could we possibly be incrementing X3p one phase early?


    {
      /*#p24.POME*/ pix_pipe.POME.set(nor2(_AVAP_SCAN_DONE_TRIGp_new, pix_pipe.POFY.qp_old()));
      /*#p24.RUJU*/ pix_pipe.RUJU.set(or3(pix_pipe.PAHO_X_8_SYNC.qp_new(), _TOFU_VID_RSTp_new, pix_pipe.POME.qp_new()));
      /*#p24.POFY*/ pix_pipe.POFY.set(not1(pix_pipe.RUJU.qp_new()));
      /*#p24.POME*/ pix_pipe.POME.set(nor2(_AVAP_SCAN_DONE_TRIGp_new, pix_pipe.POFY.qp_new()));
      /*#p24.RUJU*/ pix_pipe.RUJU.set(or3(pix_pipe.PAHO_X_8_SYNC.qp_new(), _TOFU_VID_RSTp_new, pix_pipe.POME.qp_new()));
      /*#p24.POFY*/ pix_pipe.POFY.set(not1(pix_pipe.RUJU.qp_new()));

      /*#p24.RUZE*/ wire _RUZE_HSYNCn_new = not1(pix_pipe.POFY.qp_new());
      PIN_LCD_HSYNC.pin_out(1, _RUZE_HSYNCn_new, _RUZE_HSYNCn_new);
    }

    {
      /*#p24.LOFU*/ wire _LOFU_LINE_ENDn_new  = not1(lcd_reg.RUTU_x113p.qp_new());
      /*#p24.LUCA*/ lcd_reg.LUCA_LINE_EVENp .dff17(_LOFU_LINE_ENDn_new,               _LYFE_VID_RSTn_new, lcd_reg.LUCA_LINE_EVENp.qn_old());
      /*#p21.NAPO*/ lcd_reg.NAPO_FRAME_EVENp.dff17(lcd_reg.POPU_VBLANK_CLKp.qp_new(), _LYFE_VID_RSTn_new, lcd_reg.NAPO_FRAME_EVENp.qn_old());
      // if LCDC_ENn, LCD_PIN_ALTSG = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)
      /*#p24.MAGU*/ wire _MAGU_new = xor2(lcd_reg.NAPO_FRAME_EVENp.qp_new(), lcd_reg.LUCA_LINE_EVENp.qn_new());
      /*#p24.MECO*/ wire _MECO_new = not1(_MAGU_new);
      /*#p24.KEBO*/ wire _KEBO_new = not1(_MECO_new);
      /*#p24.KEDY*/ wire _KEDY_LCDC_ENn_new = not1(pix_pipe.XONA_LCDC_LCDENn.qn_new());
      /* p01.UREK*/ wire _UREK_DIV07n_new = not1(tim_reg.TULU_DIV07p.qp_new());
      /*#p24.USEC*/ wire _USEC_DIV_07p_new = not1(_UREK_DIV07n_new);
      /*#p24.KUPA*/ wire _KUPA_new = amux2(pix_pipe.XONA_LCDC_LCDENn.qn_new(), _KEBO_new, _KEDY_LCDC_ENn_new, _USEC_DIV_07p_new);
      /*#p24.KOFO*/ wire _KOFO_new = not1(_KUPA_new);

      PIN_LCD_FLIPS.pin_out(1, _KOFO_new, _KOFO_new);
    }

    {
      /*#p21.SAXO*/ wire _SAXO_LX0p_new = lcd_reg.SAXO_LX0p.qp_new();
      /*#p21.TYPO*/ wire _TYPO_LX1p_new = lcd_reg.TYPO_LX1p.qp_new();
      /*#p21.VYZO*/ wire _VYZO_LX2p_new = lcd_reg.VYZO_LX2p.qp_new();
      /*#p21.TELU*/ wire _TELU_LX3p_new = lcd_reg.TELU_LX3p.qp_new();
      /*#p21.SUDE*/ wire _SUDE_LX4p_new = lcd_reg.SUDE_LX4p.qp_new();
      /*#p21.TAHA*/ wire _TAHA_LX5p_new = lcd_reg.TAHA_LX5p.qp_new();
      /*#p21.TYRY*/ wire _TYRY_LX6p_new = lcd_reg.TYRY_LX6p.qp_new();

      /*#p21.TOCU*/ wire _TOCU_LX0n_new = not1(_SAXO_LX0p_new);
      /*#p21.VEPE*/ wire _VEPE_LX1n_new = not1(_TYPO_LX1p_new);
      /* p21.VUTY*/ wire _VUTY_LX2n_new = not1(_VYZO_LX2p_new);
      /* p21.VATE*/ wire _VATE_LX3n_new = not1(_TELU_LX3p_new);
      /* p21.TUDA*/ wire _TUDA_LX4n_new = not1(_SUDE_LX4p_new);
      /* p21.TAFY*/ wire _TAFY_LX5n_new = not1(_TAHA_LX5p_new);
      /* p21.TUJU*/ wire _TUJU_LX6n_new = not1(_TYRY_LX6p_new);

      /* p21.VOKU*/ wire _VOKU_LX000n_new = nand7(_TUJU_LX6n_new, _TAFY_LX5n_new, _TUDA_LX4n_new, _VATE_LX3n_new, _VUTY_LX2n_new, _VEPE_LX1n_new, _TOCU_LX0n_new); // 0000000 == 0
      /* p21.TOZU*/ wire _TOZU_LX007n_new = nand7(_TUJU_LX6n_new, _TAFY_LX5n_new, _TUDA_LX4n_new, _VATE_LX3n_new, _VYZO_LX2p_new, _TYPO_LX1p_new, _SAXO_LX0p_new); // 0000111 == 7
      /* p21.TECE*/ wire _TECE_LX045n_new = nand7(_TUJU_LX6n_new, _TAHA_LX5p_new, _TUDA_LX4n_new, _TELU_LX3p_new, _VYZO_LX2p_new, _VEPE_LX1n_new, _SAXO_LX0p_new); // 0101101 == 45
      /*#p21.TEBO*/ wire _TEBO_LX083n_new = nand7(_TYRY_LX6p_new, _TAFY_LX5n_new, _SUDE_LX4p_new, _VATE_LX3n_new, _VUTY_LX2n_new, _TYPO_LX1p_new, _SAXO_LX0p_new); // 1010011 == 83
      /*#p21.TEGY*/ wire _TEGY_STROBE_new = nand4(_VOKU_LX000n_new, _TOZU_LX007n_new, _TECE_LX045n_new, _TEBO_LX083n_new);
      /*#p21.SONO*/ wire _SONO_ABxxxxGH_clknew = not1(_TALU_xxCDEFxx_clknew);
      /*#p21.SYGU*/ lcd_reg.SYGU_LINE_STROBE.dff17(_SONO_ABxxxxGH_clknew,  _LYFE_VID_RSTn_new, _TEGY_STROBE_new);

      /*#p21.RYNO*/ wire _RYNO_new = or2(lcd_reg.SYGU_LINE_STROBE.qp_new(), lcd_reg.RUTU_x113p.qp_new());
      /*#p21.POGU*/ wire _POGU_new = not1(_RYNO_new);
      PIN_LCD_CNTRL.pin_out(1, _POGU_new, _POGU_new);
    }

    {
      /*#p21.PURE*/ wire _PURE_LINE_ENDn_new = not1(lcd_reg.RUTU_x113p.qp_new());
      /*#p24.KASA*/ wire _KASA_LINE_ENDp_new = not1(_PURE_LINE_ENDn_new);
      /*#p24.KEDY*/ wire _KEDY_LCDC_ENn_new = not1(pix_pipe.XONA_LCDC_LCDENn.qn_new());
      /* p01.UMEK*/ wire _UMEK_DIV06n_new = not1(tim_reg.UGOT_DIV06p.qp_new());
      /*#p24.UMOB*/ wire _UMOB_DIV_06p_new = not1(_UMEK_DIV06n_new);
      /*#p24.KAHE*/ wire _KAHE_LINE_ENDp_new = amux2(pix_pipe.XONA_LCDC_LCDENn.qn_new(), _KASA_LINE_ENDp_new, _KEDY_LCDC_ENn_new, _UMOB_DIV_06p_new);
      /*#p24.KYMO*/ wire _KYMO_LINE_ENDn_new = not1(_KAHE_LINE_ENDp_new);
      PIN_LCD_LATCH.pin_out(1, _KYMO_LINE_ENDn_new, _KYMO_LINE_ENDn_new);
    }

    {
      /*#p21.XAJO*/ wire _XAJO_X_009p_new = and2(pix_pipe.XEHO_PX0p.qp_new(), pix_pipe.XYDO_PX3p.qp_new());
      /*#p21.WEGO*/ wire _WEGO_HBLANKp_new = or2(_TOFU_VID_RSTp_new, pix_pipe.VOGA_HBLANKp.qp_new());
      /*#p21.WUSA*/ pix_pipe.WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p_new, _WEGO_HBLANKp_new);
      /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK_new = and2(pix_pipe.WUSA_LCD_CLOCK_GATE.qp_new(), _SACU_CLKPIPE_AxCxExGx_clknew);
      /*#p27.POVA*/ wire _POVA_FINE_MATCHpe_new = and2(pix_pipe.PUXA_SCX_FINE_MATCH_A.qp_new(), pix_pipe.NYZE_SCX_FINE_MATCH_B.qn_new());
      /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK_new = or2(_TOBA_LCD_CLOCK_new, _POVA_FINE_MATCHpe_new);
      /*#p21.RYPO*/ wire _RYPO_LCD_CLOCK_new = not1(_SEMU_LCD_CLOCK_new);
      PIN_LCD_CLOCK.pin_out(1, _RYPO_LCD_CLOCK_new, _RYPO_LCD_CLOCK_new);
    }

    {
      /*#p24.NERU*/ wire _NERU_y000p_new = nor8(lcd_reg.LAFO_LY7p.qp_new(), lcd_reg.LOVU_LY4p.qp_new(), lcd_reg.LYDO_LY3p.qp_new(), lcd_reg.MUWY_LY0p.qp_new(),
                                                lcd_reg.MYRO_LY1p.qp_new(), lcd_reg.LEXA_LY2p.qp_new(), lcd_reg.LEMA_LY5p.qp_new(), lcd_reg.MATO_LY6p.qp_new());
      /*#p24.MEDA*/ lcd_reg.MEDA_VSYNC_OUTn.dff17(lcd_reg.NYPE_x113p.qn_new(), _LYFE_VID_RSTn_new, _NERU_y000p_new);
      /*#p24.MURE*/ wire _MURE_VSYNC_new = not1(lcd_reg.MEDA_VSYNC_OUTn.qp_new());
      /*#*/ PIN_LCD_VSYNC.pin_out(1, _MURE_VSYNC_new, _MURE_VSYNC_new);
    }

    if (!old_lcd_clock && PIN_LCD_CLOCK.qp_ext()) {
      gb_screen_x++;
    }
    if (PIN_LCD_HSYNC.qp_ext() || PIN_LCD_LATCH.qp_ext()) {
      gb_screen_x = 0;
    }

    lcd_pix_lo.nor_latch(PIN_LCD_DATA0.qp_ext(), PIN_LCD_CLOCK.qp_ext() | PIN_LCD_HSYNC.qp_ext());
    lcd_pix_hi.nor_latch(PIN_LCD_DATA1.qp_ext(), PIN_LCD_CLOCK.qp_ext() | PIN_LCD_HSYNC.qp_ext());

    if (!old_lcd_latch && PIN_LCD_LATCH.qp_ext()) {
      if (gb_screen_y < 144) {
        for (int x = 0; x < 159; x++) {
          uint8_t p0 = lcd_pipe_lo[x + 1].qp_old();
          uint8_t p1 = lcd_pipe_hi[x + 1].qp_old();
          framebuffer[x + gb_screen_y * 160] = p0 + p1 * 2;
        }
        {
          uint8_t p0 = lcd_pix_lo.qp_new();
          uint8_t p1 = lcd_pix_hi.qp_new();
          framebuffer[159 + gb_screen_y * 160] = p0 + p1 * 2;
        }
      }

      gb_screen_y++;
      if (PIN_LCD_VSYNC.qp_ext()) {
        gb_screen_y = 0;
      }
    }

    for (int i = 0; i < 159; i++) {
      lcd_pipe_lo[i].dff(!PIN_LCD_CLOCK.qp_ext(), 1, 1, lcd_pipe_lo[i + 1].qp_old());
      lcd_pipe_hi[i].dff(!PIN_LCD_CLOCK.qp_ext(), 1, 1, lcd_pipe_hi[i + 1].qp_old());
    }

    lcd_pipe_lo[159].dff(!PIN_LCD_CLOCK.qp_ext(), 1, 1, lcd_pix_lo.qp_new());
    lcd_pipe_hi[159].dff(!PIN_LCD_CLOCK.qp_ext(), 1, 1, lcd_pix_hi.qp_new());

    old_lcd_clock = PIN_LCD_CLOCK.qp_ext();
    old_lcd_latch = PIN_LCD_LATCH.qp_ext();
  }






































































































  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------











































































































  // OPD -> OBL
  /* p29.YDYV*/ oam_bus.YDYV_OAM_LATCH_DA0n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn_in[0].qp_ext());
  /* p29.YCEB*/ oam_bus.YCEB_OAM_LATCH_DA1n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn_in[1].qp_ext());
  /* p29.ZUCA*/ oam_bus.ZUCA_OAM_LATCH_DA2n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn_in[2].qp_ext());
  /* p29.WONE*/ oam_bus.WONE_OAM_LATCH_DA3n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn_in[3].qp_ext());
  /* p29.ZAXE*/ oam_bus.ZAXE_OAM_LATCH_DA4n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn_in[4].qp_ext());
  /* p29.XAFU*/ oam_bus.XAFU_OAM_LATCH_DA5n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn_in[5].qp_ext());
  /* p29.YSES*/ oam_bus.YSES_OAM_LATCH_DA6n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn_in[6].qp_ext());
  /* p29.ZECA*/ oam_bus.ZECA_OAM_LATCH_DA7n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DAn_in[7].qp_ext());

  /*#p31.XYKY*/ oam_bus.XYKY_OAM_LATCH_DB0n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn_in[0].qp_ext());
  /* p31.YRUM*/ oam_bus.YRUM_OAM_LATCH_DB1n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn_in[1].qp_ext());
  /* p31.YSEX*/ oam_bus.YSEX_OAM_LATCH_DB2n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn_in[2].qp_ext());
  /* p31.YVEL*/ oam_bus.YVEL_OAM_LATCH_DB3n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn_in[3].qp_ext());
  /* p31.WYNO*/ oam_bus.WYNO_OAM_LATCH_DB4n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn_in[4].qp_ext());
  /* p31.CYRA*/ oam_bus.CYRA_OAM_LATCH_DB5n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn_in[5].qp_ext());
  /* p31.ZUVE*/ oam_bus.ZUVE_OAM_LATCH_DB6n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn_in[6].qp_ext());
  /* p31.ECED*/ oam_bus.ECED_OAM_LATCH_DB7n.tp_latch(_BODE_OAM_OEp_new, BUS_OAM_DBn_in[7].qp_ext());

  //------------------------------------------------------------------------------
  // Latch stuff for CPU

  /* EBL -> CBD */
  /* p08.LAVO*/ wire _LAVO_HOLDn_new = nand3(PIN_CPU_RDp.qp_ext(), _TEXO_8000_9FFFn_ext, PIN_CPU_LATCH_EXT.qp_ext());
  /*#p08.SOMA*/ ext_bus.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D_in_new[0].qn_new());
  /* p08.RONY*/ ext_bus.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D_in_new[1].qn_new());
  /* p08.RAXY*/ ext_bus.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D_in_new[2].qn_new());
  /* p08.SELO*/ ext_bus.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D_in_new[3].qn_new());
  /* p08.SODY*/ ext_bus.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D_in_new[4].qn_new());
  /* p08.SAGO*/ ext_bus.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D_in_new[5].qn_new());
  /* p08.RUPA*/ ext_bus.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D_in_new[6].qn_new());
  /* p08.SAZY*/ ext_bus.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D_in_new[7].qn_new());

  // ZRAM control signals are

  // clk_reg.PIN_CPU_BUKE_AxxxxxGH
  // TEDO_CPU_RDp();
  // TAPU_CPU_WRp_xxxxEFGx()
  // _SYKE_FF00_FFFFp
  // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

  if ((cpu_bus_addr >= 0xFF80) && (cpu_bus_addr <= 0xFFFE)) {
    if (_TAPU_CPU_WRp_xxxxEFGx_clk) {
      zero_ram[cpu_bus_addr & 0x007F] = pack_u8p_new(8, &BUS_CPU_Dp_ext[0]);
    }
  }

#pragma region BUS_CPU_Dp_out_Drivers

  if (DELTA_GH) {

    {
      /* FF00 JOYP */
      /* p05.KEMA*/ BUS_CPU_Dp_out[0].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KEVU_JOYP_L0n.qp_new());
      /* p05.KURO*/ BUS_CPU_Dp_out[1].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KAPA_JOYP_L1n.qp_new());
      /* p05.KUVE*/ BUS_CPU_Dp_out[2].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KEJA_JOYP_L2n.qp_new());
      /* p05.JEKU*/ BUS_CPU_Dp_out[3].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KOLO_JOYP_L3n.qp_new());
      /* p05.KOCE*/ BUS_CPU_Dp_out[4].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KELY_JOYP_UDLRp.qn_new());
      /* p05.CUDY*/ BUS_CPU_Dp_out[5].tri6_nn(_BYZO_FF00_RDn_ext, joypad.COFY_JOYP_ABCSp.qn_new());
      /* p??.????*/ BUS_CPU_Dp_out[6].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KUKO_DBG_FF00_D6n.qp_new());
      /* p??.????*/ BUS_CPU_Dp_out[7].tri6_nn(_BYZO_FF00_RDn_ext, joypad.KERU_DBG_FF00_D7n.qp_new());
    }

    {
      /* FF40 LCDC */
      /*#p23.WYPO*/ BUS_CPU_Dp_out[0].tri6_nn(_WYCE_FF40_RDn_ext, pix_pipe.VYXE_LCDC_BGENn  .qp_new());
      /*#p23.XERO*/ BUS_CPU_Dp_out[1].tri6_nn(_WYCE_FF40_RDn_ext, pix_pipe.XYLO_LCDC_SPENn  .qp_new());
      /* p23.WYJU*/ BUS_CPU_Dp_out[2].tri6_nn(_WYCE_FF40_RDn_ext, pix_pipe.XYMO_LCDC_SPSIZEn.qp_new());
      /* p23.WUKA*/ BUS_CPU_Dp_out[3].tri6_nn(_WYCE_FF40_RDn_ext, pix_pipe.XAFO_LCDC_BGMAPn .qp_new());
      /* p23.VOKE*/ BUS_CPU_Dp_out[4].tri6_nn(_WYCE_FF40_RDn_ext, pix_pipe.WEXU_LCDC_BGTILEn.qp_new());
      /* p23.VATO*/ BUS_CPU_Dp_out[5].tri6_nn(_WYCE_FF40_RDn_ext, pix_pipe.WYMO_LCDC_WINENn .qp_new());
      /*#p23.VAHA*/ BUS_CPU_Dp_out[6].tri6_nn(_WYCE_FF40_RDn_ext, pix_pipe.WOKY_LCDC_WINMAPn.qp_new());
      /*#p23.XEBU*/ BUS_CPU_Dp_out[7].tri6_nn(_WYCE_FF40_RDn_ext, pix_pipe.XONA_LCDC_LCDENn .qp_new());
    }

    {
      /* FF41 STAT */

      /*#p21.PARU*/ wire _PARU_VBLANKp_new = not1(lcd_reg.POPU_VBLANK_CLKp.qn_new());
      /* p21.SADU*/ wire _SADU_STAT_MODE0n_new = nor2(pix_pipe.XYMU_RENDERINGn.qn_new(), _PARU_VBLANKp_new); // die NOR
      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_new   = and2(_BOGE_DMA_RUNNINGn_new, sprite_scanner.BESU_SCANNINGp.qp_new());
      /* p21.XATY*/ wire _XATY_STAT_MODE1n_new = nor2(_ACYL_SCANNINGp_new, pix_pipe.XYMU_RENDERINGn.qn_new()); // die NOR
      /*#p21.TEBY*/ BUS_CPU_Dp_out[0].tri6_pn(_TOBE_FF41_RDp_ext, _SADU_STAT_MODE0n_new);
      /*#p21.WUGA*/ BUS_CPU_Dp_out[1].tri6_pn(_TOBE_FF41_RDp_ext, _XATY_STAT_MODE1n_new);
      /*#p21.SEGO*/ BUS_CPU_Dp_out[2].tri6_pn(_TOBE_FF41_RDp_ext, pix_pipe.RUPO_LYC_MATCH_LATCHn.qp_new());
      /* p21.PUZO*/ BUS_CPU_Dp_out[3].tri6_nn(_VAVE_FF41_RDn_ext, pix_pipe.ROXE_STAT_HBI_ENn.qp_new());
      /* p21.POFO*/ BUS_CPU_Dp_out[4].tri6_nn(_VAVE_FF41_RDn_ext, pix_pipe.RUFO_STAT_VBI_ENn.qp_new());
      /* p21.SASY*/ BUS_CPU_Dp_out[5].tri6_nn(_VAVE_FF41_RDn_ext, pix_pipe.REFE_STAT_OAI_ENn.qp_new());
      /* p21.POTE*/ BUS_CPU_Dp_out[6].tri6_nn(_VAVE_FF41_RDn_ext, pix_pipe.RUGU_STAT_LYI_ENn.qp_new());
    }

    {
      /* FF45 LYC */
      /*#p23.RETU*/ BUS_CPU_Dp_out[0].tri6_nn(_WEKU_FF45_RDn_ext, lcd_reg.SYRY_LYC0n.qp_new());
      /* p23.VOJO*/ BUS_CPU_Dp_out[1].tri6_nn(_WEKU_FF45_RDn_ext, lcd_reg.VUCE_LYC1n.qp_new());
      /* p23.RAZU*/ BUS_CPU_Dp_out[2].tri6_nn(_WEKU_FF45_RDn_ext, lcd_reg.SEDY_LYC2n.qp_new());
      /* p23.REDY*/ BUS_CPU_Dp_out[3].tri6_nn(_WEKU_FF45_RDn_ext, lcd_reg.SALO_LYC3n.qp_new());
      /* p23.RACE*/ BUS_CPU_Dp_out[4].tri6_nn(_WEKU_FF45_RDn_ext, lcd_reg.SOTA_LYC4n.qp_new());
      /*#p23.VAZU*/ BUS_CPU_Dp_out[5].tri6_nn(_WEKU_FF45_RDn_ext, lcd_reg.VAFA_LYC5n.qp_new());
      /* p23.VAFE*/ BUS_CPU_Dp_out[6].tri6_nn(_WEKU_FF45_RDn_ext, lcd_reg.VEVO_LYC6n.qp_new());
      /* p23.PUFY*/ BUS_CPU_Dp_out[7].tri6_nn(_WEKU_FF45_RDn_ext, lcd_reg.RAHA_LYC7n.qp_new());
    }

    {
      /* FF46 DMA */
      /*#p04.POLY*/ BUS_CPU_Dp_out[0].tri6_pn(_PUSY_FF46_RDp_ext, dma_reg.NAFA_DMA_A08n.qp_new());
      /* p04.ROFO*/ BUS_CPU_Dp_out[1].tri6_pn(_PUSY_FF46_RDp_ext, dma_reg.PYNE_DMA_A09n.qp_new());
      /* p04.REMA*/ BUS_CPU_Dp_out[2].tri6_pn(_PUSY_FF46_RDp_ext, dma_reg.PARA_DMA_A10n.qp_new());
      /* p04.PANE*/ BUS_CPU_Dp_out[3].tri6_pn(_PUSY_FF46_RDp_ext, dma_reg.NYDO_DMA_A11n.qp_new());
      /* p04.PARE*/ BUS_CPU_Dp_out[4].tri6_pn(_PUSY_FF46_RDp_ext, dma_reg.NYGY_DMA_A12n.qp_new());
      /* p04.RALY*/ BUS_CPU_Dp_out[5].tri6_pn(_PUSY_FF46_RDp_ext, dma_reg.PULA_DMA_A13n.qp_new());
      /* p04.RESU*/ BUS_CPU_Dp_out[6].tri6_pn(_PUSY_FF46_RDp_ext, dma_reg.POKU_DMA_A14n.qp_new());
      /* p04.NUVY*/ BUS_CPU_Dp_out[7].tri6_pn(_PUSY_FF46_RDp_ext, dma_reg.MARU_DMA_A15n.qp_new());
    }

    {
      /* FF47 BGP */
      /*#p36.RARO*/ BUS_CPU_Dp_out[0].tri6_nn(_TEPY_FF47_RDn_ext, pix_pipe.PAVO_BGP_D0n.qp_new());
      /* p36.PABA*/ BUS_CPU_Dp_out[1].tri6_nn(_TEPY_FF47_RDn_ext, pix_pipe.NUSY_BGP_D1n.qp_new());
      /* p36.REDO*/ BUS_CPU_Dp_out[2].tri6_nn(_TEPY_FF47_RDn_ext, pix_pipe.PYLU_BGP_D2n.qp_new());
      /* p36.LOBE*/ BUS_CPU_Dp_out[3].tri6_nn(_TEPY_FF47_RDn_ext, pix_pipe.MAXY_BGP_D3n.qp_new());
      /* p36.LACE*/ BUS_CPU_Dp_out[4].tri6_nn(_TEPY_FF47_RDn_ext, pix_pipe.MUKE_BGP_D4n.qp_new());
      /* p36.LYKA*/ BUS_CPU_Dp_out[5].tri6_nn(_TEPY_FF47_RDn_ext, pix_pipe.MORU_BGP_D5n.qp_new());
      /* p36.LODY*/ BUS_CPU_Dp_out[6].tri6_nn(_TEPY_FF47_RDn_ext, pix_pipe.MOGY_BGP_D6n.qp_new());
      /* p36.LARY*/ BUS_CPU_Dp_out[7].tri6_nn(_TEPY_FF47_RDn_ext, pix_pipe.MENA_BGP_D7n.qp_new());
    }

    {
      /* FF48 OBP0 */
      /*#p36.XARY*/ BUS_CPU_Dp_out[0].tri6_nn(_XOZY_FF48_RDn_ext, pix_pipe.XUFU_OBP0_D0n.qp_new());
      /* p36.XOKE*/ BUS_CPU_Dp_out[1].tri6_nn(_XOZY_FF48_RDn_ext, pix_pipe.XUKY_OBP0_D1n.qp_new());
      /* p36.XUNO*/ BUS_CPU_Dp_out[2].tri6_nn(_XOZY_FF48_RDn_ext, pix_pipe.XOVA_OBP0_D2n.qp_new());
      /* p36.XUBY*/ BUS_CPU_Dp_out[3].tri6_nn(_XOZY_FF48_RDn_ext, pix_pipe.XALO_OBP0_D3n.qp_new());
      /* p36.XAJU*/ BUS_CPU_Dp_out[4].tri6_nn(_XOZY_FF48_RDn_ext, pix_pipe.XERU_OBP0_D4n.qp_new());
      /* p36.XOBO*/ BUS_CPU_Dp_out[5].tri6_nn(_XOZY_FF48_RDn_ext, pix_pipe.XYZE_OBP0_D5n.qp_new());
      /* p36.XAXA*/ BUS_CPU_Dp_out[6].tri6_nn(_XOZY_FF48_RDn_ext, pix_pipe.XUPO_OBP0_D6n.qp_new());
      /* p36.XAWO*/ BUS_CPU_Dp_out[7].tri6_nn(_XOZY_FF48_RDn_ext, pix_pipe.XANA_OBP0_D7n.qp_new());
    }

    {
      /* FF49 OBP1 */
      /*#p36.LAJU*/ BUS_CPU_Dp_out[0].tri6_nn(_LOTE_FF49_RDn_ext, pix_pipe.MOXY_OBP1_D0n.qp_new());
      /* p36.LEPA*/ BUS_CPU_Dp_out[1].tri6_nn(_LOTE_FF49_RDn_ext, pix_pipe.LAWO_OBP1_D1n.qp_new());
      /* p36.LODE*/ BUS_CPU_Dp_out[2].tri6_nn(_LOTE_FF49_RDn_ext, pix_pipe.MOSA_OBP1_D2n.qp_new());
      /* p36.LYZA*/ BUS_CPU_Dp_out[3].tri6_nn(_LOTE_FF49_RDn_ext, pix_pipe.LOSE_OBP1_D3n.qp_new());
      /* p36.LUKY*/ BUS_CPU_Dp_out[4].tri6_nn(_LOTE_FF49_RDn_ext, pix_pipe.LUNE_OBP1_D4n.qp_new());
      /* p36.LUGA*/ BUS_CPU_Dp_out[5].tri6_nn(_LOTE_FF49_RDn_ext, pix_pipe.LUGU_OBP1_D5n.qp_new());
      /* p36.LEBA*/ BUS_CPU_Dp_out[6].tri6_nn(_LOTE_FF49_RDn_ext, pix_pipe.LEPU_OBP1_D6n.qp_new());
      /* p36.LELU*/ BUS_CPU_Dp_out[7].tri6_nn(_LOTE_FF49_RDn_ext, pix_pipe.LUXO_OBP1_D7n.qp_new());
    }

    {
      /* FF50 BOOT */
      /* p07.SYPU*/ BUS_CPU_Dp_out[0].tri6_pn(_TEXE_FF50_RDp_ext, BOOT_BITn.qp_new());
    }

    {
      /*#p08.RYMA*/ BUS_CPU_Dp_out[0].tri6_nn(_LAVO_HOLDn_new, ext_bus.SOMA_EXT_DATA_LATCH_D0n.qp_new());
      /* p08.RUVO*/ BUS_CPU_Dp_out[1].tri6_nn(_LAVO_HOLDn_new, ext_bus.RONY_EXT_DATA_LATCH_D1n.qp_new());
      /* p08.RYKO*/ BUS_CPU_Dp_out[2].tri6_nn(_LAVO_HOLDn_new, ext_bus.RAXY_EXT_DATA_LATCH_D2n.qp_new());
      /* p08.TAVO*/ BUS_CPU_Dp_out[3].tri6_nn(_LAVO_HOLDn_new, ext_bus.SELO_EXT_DATA_LATCH_D3n.qp_new());
      /* p08.TEPE*/ BUS_CPU_Dp_out[4].tri6_nn(_LAVO_HOLDn_new, ext_bus.SODY_EXT_DATA_LATCH_D4n.qp_new());
      /* p08.SAFO*/ BUS_CPU_Dp_out[5].tri6_nn(_LAVO_HOLDn_new, ext_bus.SAGO_EXT_DATA_LATCH_D5n.qp_new());
      /* p08.SEVU*/ BUS_CPU_Dp_out[6].tri6_nn(_LAVO_HOLDn_new, ext_bus.RUPA_EXT_DATA_LATCH_D6n.qp_new());
      /* p08.TAJU*/ BUS_CPU_Dp_out[7].tri6_nn(_LAVO_HOLDn_new, ext_bus.SAZY_EXT_DATA_LATCH_D7n.qp_new());
    }
    {
      /* BOOT -> CBD */
  #if 0
    /* p07.ZYBA*/ wire ZYBA_ADDR_00n = not1(BUS_CPU_A[ 0]);
    /* p07.ZUVY*/ wire ZUVY_ADDR_01n = not1(BUS_CPU_A[ 1]);
    /* p07.ZUFY*/ wire ZUFY_ADDR_04n = not1(BUS_CPU_A[ 4]);
    /* p07.ZERA*/ wire ZERA_ADDR_05n = not1(BUS_CPU_A[ 5]);
    /* p07.ZOLE*/ wire ZOLE_ADDR_00  = and2(ZUVY_ADDR_01n, ZYBA_ADDR_00n);
    /* p07.ZAJE*/ wire ZAJE_ADDR_01  = and2(ZUVY_ADDR_01n, BUS_CPU_A[ 0]);
    /* p07.ZUBU*/ wire ZUBU_ADDR_10  = and2(BUS_CPU_A[ 1], ZYBA_ADDR_00n);
    /* p07.ZAPY*/ wire ZAPY_ADDR_11  = and2(BUS_CPU_A[ 1], BUS_CPU_A[ 0]);

    /* p07.ZETE*/ wire BOOTROM_A1nA0n = not1(ZOLE_ADDR_00);
    /* p07.ZEFU*/ wire BOOTROM_A1nA0p  = not1(ZAJE_ADDR_01);
    /* p07.ZYRO*/ wire BOOTROM_A1pA0n  = not1(ZUBU_ADDR_10);
    /* p07.ZAPA*/ wire BOOTROM_A1pA0p   = not1(ZAPY_ADDR_11);
    /* p07.ZYGA*/ wire BOOTROM_A2n    = not1(BUS_CPU_A[ 2]);
    /* p07.ZYKY*/ wire BOOTROM_A3n    = not1(BUS_CPU_A[ 3]);
    /* p07.ZYKY*/ wire BOOTROM_A5nA4n = and2(ZERA_ADDR_05n, ZUFY_ADDR_04n);
    /* p07.ZYGA*/ wire BOOTROM_A5nA4p  = and2(ZERA_ADDR_05n, BUS_CPU_A[ 4]);
    /* p07.ZOVY*/ wire BOOTROM_A5pA4n  = and2(BUS_CPU_A[ 5], ZUFY_ADDR_04n);
    /* p07.ZUKO*/ wire BOOTROM_A5pA4p   = and2(BUS_CPU_A[ 5], BUS_CPU_A[ 4]);
    /* p07.ZAGE*/ wire BOOTROM_A6n    = not1(BUS_CPU_A[ 6]);
    /* p07.ZYRA*/ wire BOOTROM_A7n    = not1(BUS_CPU_A[ 7]);
  #endif

      // this is kind of a hack
      uint8_t bootrom_data = boot_buf[cpu_bus_addr & 0xFF];
      /* p07.YAZA*/ wire _YAZA_MODE_DBG1n_ext = not1(_UMUT_MODE_DBG1p_ext);
      /* p07.YULA*/ wire _YULA_BOOT_RDp_new   = and3(_TEDO_CPU_RDp_ext, _YAZA_MODE_DBG1n_ext, _TUTU_ADDR_BOOTp_new); // def AND
      /* p07.ZADO*/ wire _ZADO_BOOT_CSn_new   = nand2(_YULA_BOOT_RDp_new, _ZUFA_0000_00FFp_ext);
      /* p07.ZERY*/ wire _ZERY_BOOT_CSp_new   = not1(_ZADO_BOOT_CSn_new);
      BUS_CPU_Dp_out[0].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x01));
      BUS_CPU_Dp_out[1].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x02));
      BUS_CPU_Dp_out[2].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x04));
      BUS_CPU_Dp_out[3].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x08));
      BUS_CPU_Dp_out[4].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x10));
      BUS_CPU_Dp_out[5].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x20));
      BUS_CPU_Dp_out[6].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x40));
      BUS_CPU_Dp_out[7].tri6_pn(_ZERY_BOOT_CSp_new, !bool(bootrom_data & 0x80));
    }

    {
      /* p28.MYNU*/ wire _MYNU_CPU_RDn_ext = nand2(_ASOT_CPU_RDp_ext, _CATY_LATCH_EXTp_ext);
      /* p28.LEKO*/ wire _LEKO_CPU_RDp_ext = not1(_MYNU_CPU_RDn_ext);

      /* VBD  -> CBD */
      {
        /*#p25.ROPY*/ wire _ROPY_RENDERINGn_new = not1(pix_pipe.XYMU_RENDERINGn.qn_new());
        /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp_new  = and2(_TOLE_CPU_VRAM_RDp_ext, _ROPY_RENDERINGn_new);
        /* p25.TYVY*/ wire _TYVY_VBD_TO_CBDn_new = nand2(_SERE_CPU_VRM_RDp_new, _LEKO_CPU_RDp_ext);
        /* p25.SEBY*/ wire _SEBY_VBD_TO_CBDp_new = not1(_TYVY_VBD_TO_CBDn_new);
        /* p25.RERY*/ wire _RERY_VBUS_D0n_ext = not1(BUS_VRAM_Dp_in[0].qp_old());
        /* p25.RUNA*/ wire _RUNA_VBUS_D1n_ext = not1(BUS_VRAM_Dp_in[1].qp_old());
        /* p25.RONA*/ wire _RONA_VBUS_D2n_ext = not1(BUS_VRAM_Dp_in[2].qp_old());
        /* p25.RUNO*/ wire _RUNO_VBUS_D3n_ext = not1(BUS_VRAM_Dp_in[3].qp_old());
        /* p25.SANA*/ wire _SANA_VBUS_D4n_ext = not1(BUS_VRAM_Dp_in[4].qp_old());
        /* p25.RORO*/ wire _RORO_VBUS_D5n_ext = not1(BUS_VRAM_Dp_in[5].qp_old());
        /* p25.RABO*/ wire _RABO_VBUS_D6n_ext = not1(BUS_VRAM_Dp_in[6].qp_old());
        /* p25.SAME*/ wire _SAME_VBUS_D7n_ext = not1(BUS_VRAM_Dp_in[7].qp_old());

        /*#p25.RUGA*/ BUS_CPU_Dp_out[0].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RERY_VBUS_D0n_ext);
        /* p25.ROTA*/ BUS_CPU_Dp_out[1].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RUNA_VBUS_D1n_ext);
        /* p25.RYBU*/ BUS_CPU_Dp_out[2].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RONA_VBUS_D2n_ext);
        /* p25.RAJU*/ BUS_CPU_Dp_out[3].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RUNO_VBUS_D3n_ext);
        /* p25.TYJA*/ BUS_CPU_Dp_out[4].tri6_pn(_SEBY_VBD_TO_CBDp_new, _SANA_VBUS_D4n_ext);
        /* p25.REXU*/ BUS_CPU_Dp_out[5].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RORO_VBUS_D5n_ext);
        /* p25.RUPY*/ BUS_CPU_Dp_out[6].tri6_pn(_SEBY_VBD_TO_CBDp_new, _RABO_VBUS_D6n_ext);
        /* p25.TOKU*/ BUS_CPU_Dp_out[7].tri6_pn(_SEBY_VBD_TO_CBDp_new, _SAME_VBUS_D7n_ext);
      }

      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_new   = and2(_BOGE_DMA_RUNNINGn_new, sprite_scanner.BESU_SCANNINGp.qp_new());
      /* p28.AJON*/ wire _AJON_PPU_OAM_ENp_new = and2(_BOGE_DMA_RUNNINGn_new, pix_pipe.XYMU_RENDERINGn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
      /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn_new   = nor3(dma_reg.MATU_DMA_RUNNINGp.qp_new(), _ACYL_SCANNINGp_new, _AJON_PPU_OAM_ENp_new); // def nor
      /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp_new = and2(_SARO_FE00_FEFFp_ext, _AJUJ_OAM_BUSYn_new); // def and

      /* OBL  -> CBD */
      {
        /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp_new = and3(_LEKO_CPU_RDp_ext, _AMAB_CPU_OAM_ENp_new, _GEKA_OAM_A0p_new);
        /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn_new = not1(_WUKU_OBL_TO_CBDp_new);
        /* p31.XACA*/ BUS_CPU_Dp_out[0].tri10_np(_WEWU_OBL_TO_CBDn_new, oam_bus.XYKY_OAM_LATCH_DB0n.qn_new());
        /* p31.XAGU*/ BUS_CPU_Dp_out[1].tri10_np(_WEWU_OBL_TO_CBDn_new, oam_bus.YRUM_OAM_LATCH_DB1n.qn_new());
        /* p31.XEPU*/ BUS_CPU_Dp_out[2].tri10_np(_WEWU_OBL_TO_CBDn_new, oam_bus.YSEX_OAM_LATCH_DB2n.qn_new());
        /* p31.XYGU*/ BUS_CPU_Dp_out[3].tri10_np(_WEWU_OBL_TO_CBDn_new, oam_bus.YVEL_OAM_LATCH_DB3n.qn_new());
        /* p31.XUNA*/ BUS_CPU_Dp_out[4].tri10_np(_WEWU_OBL_TO_CBDn_new, oam_bus.WYNO_OAM_LATCH_DB4n.qn_new());
        /* p31.DEVE*/ BUS_CPU_Dp_out[5].tri10_np(_WEWU_OBL_TO_CBDn_new, oam_bus.CYRA_OAM_LATCH_DB5n.qn_new());
        /* p31.ZEHA*/ BUS_CPU_Dp_out[6].tri10_np(_WEWU_OBL_TO_CBDn_new, oam_bus.ZUVE_OAM_LATCH_DB6n.qn_new());
        /* p31.FYRA*/ BUS_CPU_Dp_out[7].tri10_np(_WEWU_OBL_TO_CBDn_new, oam_bus.ECED_OAM_LATCH_DB7n.qn_new());
      }
      {
        /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp_new = and3(_LEKO_CPU_RDp_ext, _AMAB_CPU_OAM_ENp_new, _WAFO_OAM_A0n_new);
        /* p28.WUME*/ wire _WUME_OBL_TO_CBDn_new = not1(_GUKO_OBL_TO_CBDp_new);
        /* p29.YFAP*/ BUS_CPU_Dp_out[0].tri10_np(_WUME_OBL_TO_CBDn_new, oam_bus.YDYV_OAM_LATCH_DA0n.qn_new());
        /* p29.XELE*/ BUS_CPU_Dp_out[1].tri10_np(_WUME_OBL_TO_CBDn_new, oam_bus.YCEB_OAM_LATCH_DA1n.qn_new());
        /* p29.YPON*/ BUS_CPU_Dp_out[2].tri10_np(_WUME_OBL_TO_CBDn_new, oam_bus.ZUCA_OAM_LATCH_DA2n.qn_new());
        /* p29.XUVO*/ BUS_CPU_Dp_out[3].tri10_np(_WUME_OBL_TO_CBDn_new, oam_bus.WONE_OAM_LATCH_DA3n.qn_new());
        /* p29.ZYSA*/ BUS_CPU_Dp_out[4].tri10_np(_WUME_OBL_TO_CBDn_new, oam_bus.ZAXE_OAM_LATCH_DA4n.qn_new());
        /* p29.YWEG*/ BUS_CPU_Dp_out[5].tri10_np(_WUME_OBL_TO_CBDn_new, oam_bus.XAFU_OAM_LATCH_DA5n.qn_new());
        /* p29.XABU*/ BUS_CPU_Dp_out[6].tri10_np(_WUME_OBL_TO_CBDn_new, oam_bus.YSES_OAM_LATCH_DA6n.qn_new());
        /* p29.YTUX*/ BUS_CPU_Dp_out[7].tri10_np(_WUME_OBL_TO_CBDn_new, oam_bus.ZECA_OAM_LATCH_DA7n.qn_new());
      }
    }

    {
      if ((cpu_bus_addr >= 0xFF80) && (cpu_bus_addr <= 0xFFFE)) {
        BUS_CPU_Dp_out[0].tri(_TEDO_CPU_RDp_ext, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x01));
        BUS_CPU_Dp_out[1].tri(_TEDO_CPU_RDp_ext, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x02));
        BUS_CPU_Dp_out[2].tri(_TEDO_CPU_RDp_ext, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x04));
        BUS_CPU_Dp_out[3].tri(_TEDO_CPU_RDp_ext, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x08));
        BUS_CPU_Dp_out[4].tri(_TEDO_CPU_RDp_ext, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x10));
        BUS_CPU_Dp_out[5].tri(_TEDO_CPU_RDp_ext, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x20));
        BUS_CPU_Dp_out[6].tri(_TEDO_CPU_RDp_ext, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x40));
        BUS_CPU_Dp_out[7].tri(_TEDO_CPU_RDp_ext, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x80));
      }
    }
  }

  {
    SPR_TRI_I_out_new[0].reset();
    SPR_TRI_I_out_new[1].reset();
    SPR_TRI_I_out_new[2].reset();
    SPR_TRI_I_out_new[3].reset();
    SPR_TRI_I_out_new[4].reset();
    SPR_TRI_I_out_new[5].reset();
    SPR_TRI_L_out_new[0].reset();
    SPR_TRI_L_out_new[1].reset();
    SPR_TRI_L_out_new[2].reset();
    SPR_TRI_L_out_new[3].reset();

    /*#p21.ACAM*/ wire _ACAM_PX0n_new = not1(pix_pipe.XEHO_PX0p.qp_new());
    /* p21.AZUB*/ wire _AZUB_PX1n_new = not1(pix_pipe.SAVY_PX1p.qp_new());
    /* p21.AMEL*/ wire _AMEL_PX2n_new = not1(pix_pipe.XODU_PX2p.qp_new());
    /* p21.AHAL*/ wire _AHAL_PX3n_new = not1(pix_pipe.XYDO_PX3p.qp_new());
    /* p21.APUX*/ wire _APUX_PX4n_new = not1(pix_pipe.TUHU_PX4p.qp_new());
    /* p21.ABEF*/ wire _ABEF_PX5n_new = not1(pix_pipe.TUKY_PX5p.qp_new());
    /* p21.ADAZ*/ wire _ADAZ_PX6n_new = not1(pix_pipe.TAKO_PX6p.qp_new());
    /* p21.ASAH*/ wire _ASAH_PX7n_new = not1(pix_pipe.SYBE_PX7p.qp_new());

    /*#p31.ZOGY*/ wire _ZOGY_STORE0_MATCH0n_new = xor2(sprite_store.XEPE_STORE0_X0p.qn_new(), _ACAM_PX0n_new);
    /* p31.ZEBA*/ wire _ZEBA_STORE0_MATCH1n_new = xor2(sprite_store.YLAH_STORE0_X1p.qn_new(), _AZUB_PX1n_new);
    /* p31.ZAHA*/ wire _ZAHA_STORE0_MATCH2n_new = xor2(sprite_store.ZOLA_STORE0_X2p.qn_new(), _AMEL_PX2n_new);
    /* p31.ZOKY*/ wire _ZOKY_STORE0_MATCH3n_new = xor2(sprite_store.ZULU_STORE0_X3p.qn_new(), _AHAL_PX3n_new);
    /* p31.WOJU*/ wire _WOJU_STORE0_MATCH4n_new = xor2(sprite_store.WELO_STORE0_X4p.qn_new(), _APUX_PX4n_new);
    /* p31.YFUN*/ wire _YFUN_STORE0_MATCH5n_new = xor2(sprite_store.XUNY_STORE0_X5p.qn_new(), _ABEF_PX5n_new);
    /* p31.WYZA*/ wire _WYZA_STORE0_MATCH6n_new = xor2(sprite_store.WOTE_STORE0_X6p.qn_new(), _ADAZ_PX6n_new);
    /* p31.YPUK*/ wire _YPUK_STORE0_MATCH7n_new = xor2(sprite_store.XAKO_STORE0_X7p.qn_new(), _ASAH_PX7n_new);

    /* p31.EDYM*/ wire _EDYM_STORE1_MATCH0n_new = xor2(sprite_store.DANY_STORE1_X0p.qn_new(), _ACAM_PX0n_new);
    /* p31.EMYB*/ wire _EMYB_STORE1_MATCH1n_new = xor2(sprite_store.DUKO_STORE1_X1p.qn_new(), _AZUB_PX1n_new);
    /* p31.EBEF*/ wire _EBEF_STORE1_MATCH2n_new = xor2(sprite_store.DESU_STORE1_X2p.qn_new(), _AMEL_PX2n_new);
    /* p31.EWOK*/ wire _EWOK_STORE1_MATCH3n_new = xor2(sprite_store.DAZO_STORE1_X3p.qn_new(), _AHAL_PX3n_new);
    /* p31.COLA*/ wire _COLA_STORE1_MATCH4n_new = xor2(sprite_store.DAKE_STORE1_X4p.qn_new(), _APUX_PX4n_new);
    /* p31.BOBA*/ wire _BOBA_STORE1_MATCH5n_new = xor2(sprite_store.CESO_STORE1_X5p.qn_new(), _ABEF_PX5n_new);
    /* p31.COLU*/ wire _COLU_STORE1_MATCH6n_new = xor2(sprite_store.DYFU_STORE1_X6p.qn_new(), _ADAZ_PX6n_new);
    /* p31.BAHU*/ wire _BAHU_STORE1_MATCH7n_new = xor2(sprite_store.CUSY_STORE1_X7p.qn_new(), _ASAH_PX7n_new);

    /* p31.FUZU*/ wire _FUZU_STORE2_MATCH0n_new = xor2(sprite_store.FOKA_STORE2_X0p.qn_new(), _ACAM_PX0n_new);
    /* p31.FESO*/ wire _FESO_STORE2_MATCH1n_new = xor2(sprite_store.FYTY_STORE2_X1p.qn_new(), _AZUB_PX1n_new);
    /* p31.FOKY*/ wire _FOKY_STORE2_MATCH2n_new = xor2(sprite_store.FUBY_STORE2_X2p.qn_new(), _AMEL_PX2n_new);
    /* p31.FYVA*/ wire _FYVA_STORE2_MATCH3n_new = xor2(sprite_store.GOXU_STORE2_X3p.qn_new(), _AHAL_PX3n_new);
    /* p31.CEKO*/ wire _CEKO_STORE2_MATCH4n_new = xor2(sprite_store.DUHY_STORE2_X4p.qn_new(), _APUX_PX4n_new);
    /* p31.DETY*/ wire _DETY_STORE2_MATCH5n_new = xor2(sprite_store.EJUF_STORE2_X5p.qn_new(), _ABEF_PX5n_new);
    /* p31.DOZO*/ wire _DOZO_STORE2_MATCH6n_new = xor2(sprite_store.ENOR_STORE2_X6p.qn_new(), _ADAZ_PX6n_new);
    /* p31.CONY*/ wire _CONY_STORE2_MATCH7n_new = xor2(sprite_store.DEPY_STORE2_X7p.qn_new(), _ASAH_PX7n_new);

    /* p31.YHOK*/ wire _YHOK_STORE3_MATCH0n_new = xor2(sprite_store.XOLY_STORE3_X0p.qn_new(), _ACAM_PX0n_new);
    /* p31.YCAH*/ wire _YCAH_STORE3_MATCH1n_new = xor2(sprite_store.XYBA_STORE3_X1p.qn_new(), _AZUB_PX1n_new);
    /* p31.YDAJ*/ wire _YDAJ_STORE3_MATCH2n_new = xor2(sprite_store.XABE_STORE3_X2p.qn_new(), _AMEL_PX2n_new);
    /* p31.YVUZ*/ wire _YVUZ_STORE3_MATCH3n_new = xor2(sprite_store.XEKA_STORE3_X3p.qn_new(), _AHAL_PX3n_new);
    /* p31.YVAP*/ wire _YVAP_STORE3_MATCH4n_new = xor2(sprite_store.XOMY_STORE3_X4p.qn_new(), _APUX_PX4n_new);
    /* p31.XENY*/ wire _XENY_STORE3_MATCH5n_new = xor2(sprite_store.WUHA_STORE3_X5p.qn_new(), _ABEF_PX5n_new);
    /* p31.XAVU*/ wire _XAVU_STORE3_MATCH6n_new = xor2(sprite_store.WYNA_STORE3_X6p.qn_new(), _ADAZ_PX6n_new);
    /* p31.XEVA*/ wire _XEVA_STORE3_MATCH7n_new = xor2(sprite_store.WECO_STORE3_X7p.qn_new(), _ASAH_PX7n_new);

    /* p31.XEJU*/ wire _XEJU_STORE4_MATCH0n_new = xor2(sprite_store.WEDU_STORE4_X0p.qn_new(), _ACAM_PX0n_new);
    /* p31.ZATE*/ wire _ZATE_STORE4_MATCH1n_new = xor2(sprite_store.YGAJ_STORE4_X1p.qn_new(), _AZUB_PX1n_new);
    /* p31.ZAKU*/ wire _ZAKU_STORE4_MATCH2n_new = xor2(sprite_store.ZYJO_STORE4_X2p.qn_new(), _AMEL_PX2n_new);
    /* p31.YBOX*/ wire _YBOX_STORE4_MATCH3n_new = xor2(sprite_store.XURY_STORE4_X3p.qn_new(), _AHAL_PX3n_new);
    /* p31.ZYKU*/ wire _ZYKU_STORE4_MATCH4n_new = xor2(sprite_store.YBED_STORE4_X4p.qn_new(), _APUX_PX4n_new);
    /* p31.ZYPU*/ wire _ZYPU_STORE4_MATCH5n_new = xor2(sprite_store.ZALA_STORE4_X5p.qn_new(), _ABEF_PX5n_new);
    /* p31.XAHA*/ wire _XAHA_STORE4_MATCH6n_new = xor2(sprite_store.WYDE_STORE4_X6p.qn_new(), _ADAZ_PX6n_new);
    /* p31.ZEFE*/ wire _ZEFE_STORE4_MATCH7n_new = xor2(sprite_store.XEPA_STORE4_X7p.qn_new(), _ASAH_PX7n_new);

    /* p31.GUZO*/ wire _GUZO_STORE5_MATCH0n_new = xor2(sprite_store.FUSA_STORE5_X0p.qn_new(), _ACAM_PX0n_new);
    /* p31.GOLA*/ wire _GOLA_STORE5_MATCH1n_new = xor2(sprite_store.FAXA_STORE5_X1p.qn_new(), _AZUB_PX1n_new);
    /* p31.GEVE*/ wire _GEVE_STORE5_MATCH2n_new = xor2(sprite_store.FOZY_STORE5_X2p.qn_new(), _AMEL_PX2n_new);
    /* p31.GUDE*/ wire _GUDE_STORE5_MATCH3n_new = xor2(sprite_store.FESY_STORE5_X3p.qn_new(), _AHAL_PX3n_new);
    /* p31.BAZY*/ wire _BAZY_STORE5_MATCH4n_new = xor2(sprite_store.CYWE_STORE5_X4p.qn_new(), _APUX_PX4n_new);
    /* p31.CYLE*/ wire _CYLE_STORE5_MATCH5n_new = xor2(sprite_store.DYBY_STORE5_X5p.qn_new(), _ABEF_PX5n_new);
    /* p31.CEVA*/ wire _CEVA_STORE5_MATCH6n_new = xor2(sprite_store.DURY_STORE5_X6p.qn_new(), _ADAZ_PX6n_new);
    /* p31.BUMY*/ wire _BUMY_STORE5_MATCH7n_new = xor2(sprite_store.CUVY_STORE5_X7p.qn_new(), _ASAH_PX7n_new);

    /* p31.XOSU*/ wire _XOSU_STORE6_MATCH0n_new = xor2(sprite_store.YCOL_STORE6_X0p.qn_new(), _ACAM_PX0n_new);
    /* p31.ZUVU*/ wire _ZUVU_STORE6_MATCH1n_new = xor2(sprite_store.YRAC_STORE6_X1p.qn_new(), _AZUB_PX1n_new);
    /* p31.XUCO*/ wire _XUCO_STORE6_MATCH2n_new = xor2(sprite_store.YMEM_STORE6_X2p.qn_new(), _AMEL_PX2n_new);
    /* p31.ZULO*/ wire _ZULO_STORE6_MATCH3n_new = xor2(sprite_store.YVAG_STORE6_X3p.qn_new(), _AHAL_PX3n_new);
    /* p31.ZARE*/ wire _ZARE_STORE6_MATCH4n_new = xor2(sprite_store.ZOLY_STORE6_X4p.qn_new(), _APUX_PX4n_new);
    /* p31.ZEMU*/ wire _ZEMU_STORE6_MATCH5n_new = xor2(sprite_store.ZOGO_STORE6_X5p.qn_new(), _ABEF_PX5n_new);
    /* p31.ZYGO*/ wire _ZYGO_STORE6_MATCH6n_new = xor2(sprite_store.ZECU_STORE6_X6p.qn_new(), _ADAZ_PX6n_new);
    /* p31.ZUZY*/ wire _ZUZY_STORE6_MATCH7n_new = xor2(sprite_store.ZESA_STORE6_X7p.qn_new(), _ASAH_PX7n_new);

    /* p31.DUSE*/ wire _DUSE_STORE7_MATCH0n_new = xor2(sprite_store.ERAZ_STORE7_X0p.qn_new(), _ACAM_PX0n_new);
    /* p31.DAGU*/ wire _DAGU_STORE7_MATCH1n_new = xor2(sprite_store.EPUM_STORE7_X1p.qn_new(), _AZUB_PX1n_new);
    /* p31.DYZE*/ wire _DYZE_STORE7_MATCH2n_new = xor2(sprite_store.EROL_STORE7_X2p.qn_new(), _AMEL_PX2n_new);
    /* p31.DESO*/ wire _DESO_STORE7_MATCH3n_new = xor2(sprite_store.EHYN_STORE7_X3p.qn_new(), _AHAL_PX3n_new);
    /* p31.EJOT*/ wire _EJOT_STORE7_MATCH4n_new = xor2(sprite_store.FAZU_STORE7_X4p.qn_new(), _APUX_PX4n_new);
    /* p31.ESAJ*/ wire _ESAJ_STORE7_MATCH5n_new = xor2(sprite_store.FAXE_STORE7_X5p.qn_new(), _ABEF_PX5n_new);
    /* p31.DUCU*/ wire _DUCU_STORE7_MATCH6n_new = xor2(sprite_store.EXUK_STORE7_X6p.qn_new(), _ADAZ_PX6n_new);
    /* p31.EWUD*/ wire _EWUD_STORE7_MATCH7n_new = xor2(sprite_store.FEDE_STORE7_X7p.qn_new(), _ASAH_PX7n_new);

    /* p31.DUZE*/ wire _DUZE_STORE8_MATCH0n_new = xor2(sprite_store.EZUF_STORE8_X0p.qn_new(), _APUX_PX4n_new);
    /* p31.DAGA*/ wire _DAGA_STORE8_MATCH1n_new = xor2(sprite_store.ENAD_STORE8_X1p.qn_new(), _ABEF_PX5n_new);
    /* p31.DAWU*/ wire _DAWU_STORE8_MATCH2n_new = xor2(sprite_store.EBOW_STORE8_X2p.qn_new(), _ADAZ_PX6n_new);
    /* p31.EJAW*/ wire _EJAW_STORE8_MATCH3n_new = xor2(sprite_store.FYCA_STORE8_X3p.qn_new(), _ASAH_PX7n_new);
    /* p31.GOHO*/ wire _GOHO_STORE8_MATCH4n_new = xor2(sprite_store.GAVY_STORE8_X4p.qn_new(), _ACAM_PX0n_new);
    /* p31.GASU*/ wire _GASU_STORE8_MATCH5n_new = xor2(sprite_store.GYPU_STORE8_X5p.qn_new(), _AZUB_PX1n_new);
    /* p31.GABU*/ wire _GABU_STORE8_MATCH6n_new = xor2(sprite_store.GADY_STORE8_X6p.qn_new(), _AMEL_PX2n_new);
    /* p31.GAFE*/ wire _GAFE_STORE8_MATCH7n_new = xor2(sprite_store.GAZA_STORE8_X7p.qn_new(), _AHAL_PX3n_new);

    /* p31.YMAM*/ wire _YMAM_STORE9_MATCH0n_new = xor2(sprite_store.XUVY_STORE9_X0p.qn_new(), _ACAM_PX0n_new);
    /* p31.YTYP*/ wire _YTYP_STORE9_MATCH1n_new = xor2(sprite_store.XERE_STORE9_X1p.qn_new(), _AZUB_PX1n_new);
    /* p31.YFOP*/ wire _YFOP_STORE9_MATCH2n_new = xor2(sprite_store.XUZO_STORE9_X2p.qn_new(), _AMEL_PX2n_new);
    /* p31.YVAC*/ wire _YVAC_STORE9_MATCH3n_new = xor2(sprite_store.XEXA_STORE9_X3p.qn_new(), _AHAL_PX3n_new);
    /* p31.ZYWU*/ wire _ZYWU_STORE9_MATCH4n_new = xor2(sprite_store.YPOD_STORE9_X4p.qn_new(), _APUX_PX4n_new);
    /* p31.ZUZA*/ wire _ZUZA_STORE9_MATCH5n_new = xor2(sprite_store.YROP_STORE9_X5p.qn_new(), _ABEF_PX5n_new);
    /* p31.ZEJO*/ wire _ZEJO_STORE9_MATCH6n_new = xor2(sprite_store.YNEP_STORE9_X6p.qn_new(), _ADAZ_PX6n_new);
    /* p31.ZEDA*/ wire _ZEDA_STORE9_MATCH7n_new = xor2(sprite_store.YZOF_STORE9_X7p.qn_new(), _ASAH_PX7n_new);

    /* p31.ZAKO*/ wire _ZAKO_STORE0_MATCHAp_new = nor4(_ZOGY_STORE0_MATCH0n_new, _ZEBA_STORE0_MATCH1n_new, _ZAHA_STORE0_MATCH2n_new, _ZOKY_STORE0_MATCH3n_new);
    /* p31.XEBA*/ wire _XEBA_STORE0_MATCHBp_new = nor4(_WOJU_STORE0_MATCH4n_new, _YFUN_STORE0_MATCH5n_new, _WYZA_STORE0_MATCH6n_new, _YPUK_STORE0_MATCH7n_new);
    /* p31.CYVY*/ wire _CYVY_STORE1_MATCHBp_new = nor4(_COLA_STORE1_MATCH4n_new, _BOBA_STORE1_MATCH5n_new, _COLU_STORE1_MATCH6n_new, _BAHU_STORE1_MATCH7n_new);
    /* p31.EWAM*/ wire _EWAM_STORE1_MATCHAp_new = nor4(_EDYM_STORE1_MATCH0n_new, _EMYB_STORE1_MATCH1n_new, _EBEF_STORE1_MATCH2n_new, _EWOK_STORE1_MATCH3n_new);
    /* p31.CEHU*/ wire _CEHU_STORE2_MATCHAp_new = nor4(_CEKO_STORE2_MATCH4n_new, _DETY_STORE2_MATCH5n_new, _DOZO_STORE2_MATCH6n_new, _CONY_STORE2_MATCH7n_new);
    /* p31.EKES*/ wire _EKES_STORE2_MATCHBp_new = nor4(_FUZU_STORE2_MATCH0n_new, _FESO_STORE2_MATCH1n_new, _FOKY_STORE2_MATCH2n_new, _FYVA_STORE2_MATCH3n_new);
    /* p31.ZURE*/ wire _ZURE_STORE3_MATCHAp_new = nor4(_YHOK_STORE3_MATCH0n_new, _YCAH_STORE3_MATCH1n_new, _YDAJ_STORE3_MATCH2n_new, _YVUZ_STORE3_MATCH3n_new);
    /* p31.YWOS*/ wire _YWOS_STORE3_MATCHBp_new = nor4(_YVAP_STORE3_MATCH4n_new, _XENY_STORE3_MATCH5n_new, _XAVU_STORE3_MATCH6n_new, _XEVA_STORE3_MATCH7n_new);
    /* p31.YKOK*/ wire _YKOK_STORE4_MATCHAp_new = nor4(_XEJU_STORE4_MATCH0n_new, _ZATE_STORE4_MATCH1n_new, _ZAKU_STORE4_MATCH2n_new, _YBOX_STORE4_MATCH3n_new);
    /* p31.YNAZ*/ wire _YNAZ_STORE4_MATCHBp_new = nor4(_ZYKU_STORE4_MATCH4n_new, _ZYPU_STORE4_MATCH5n_new, _XAHA_STORE4_MATCH6n_new, _ZEFE_STORE4_MATCH7n_new);
    /* p31.COGY*/ wire _COGY_STORE5_MATCHAp_new = nor4(_BAZY_STORE5_MATCH4n_new, _CYLE_STORE5_MATCH5n_new, _CEVA_STORE5_MATCH6n_new, _BUMY_STORE5_MATCH7n_new);
    /* p31.FYMA*/ wire _FYMA_STORE5_MATCHBp_new = nor4(_GUZO_STORE5_MATCH0n_new, _GOLA_STORE5_MATCH1n_new, _GEVE_STORE5_MATCH2n_new, _GUDE_STORE5_MATCH3n_new);
    /* p31.YWAP*/ wire _YWAP_STORE6_MATCHAp_new = nor4(_ZARE_STORE6_MATCH4n_new, _ZEMU_STORE6_MATCH5n_new, _ZYGO_STORE6_MATCH6n_new, _ZUZY_STORE6_MATCH7n_new);
    /* p31.YDOT*/ wire _YDOT_STORE6_MATCHBp_new = nor4(_XOSU_STORE6_MATCH0n_new, _ZUVU_STORE6_MATCH1n_new, _XUCO_STORE6_MATCH2n_new, _ZULO_STORE6_MATCH3n_new);
    /* p31.CYCO*/ wire _CYCO_STORE7_MATCHAp_new = nor4(_DUSE_STORE7_MATCH0n_new, _DAGU_STORE7_MATCH1n_new, _DYZE_STORE7_MATCH2n_new, _DESO_STORE7_MATCH3n_new);
    /* p31.DAJE*/ wire _DAJE_STORE7_MATCHBp_new = nor4(_EJOT_STORE7_MATCH4n_new, _ESAJ_STORE7_MATCH5n_new, _DUCU_STORE7_MATCH6n_new, _EWUD_STORE7_MATCH7n_new);
    /* p31.DAMA*/ wire _DAMA_STORE8_MATCHAp_new = nor4(_DUZE_STORE8_MATCH0n_new, _DAGA_STORE8_MATCH1n_new, _DAWU_STORE8_MATCH2n_new, _EJAW_STORE8_MATCH3n_new);
    /* p31.FEHA*/ wire _FEHA_STORE8_MATCHBp_new = nor4(_GOHO_STORE8_MATCH4n_new, _GASU_STORE8_MATCH5n_new, _GABU_STORE8_MATCH6n_new, _GAFE_STORE8_MATCH7n_new);
    /* p31.YLEV*/ wire _YLEV_STORE9_MATCHAp_new = nor4(_YMAM_STORE9_MATCH0n_new, _YTYP_STORE9_MATCH1n_new, _YFOP_STORE9_MATCH2n_new, _YVAC_STORE9_MATCH3n_new);
    /* p31.YTUB*/ wire _YTUB_STORE9_MATCHBp_new = nor4(_ZYWU_STORE9_MATCH4n_new, _ZUZA_STORE9_MATCH5n_new, _ZEJO_STORE9_MATCH6n_new, _ZEDA_STORE9_MATCH7n_new);

    /*#p29.AZEM*/ wire _AZEM_RENDERINGp_new = and2(pix_pipe.XYMU_RENDERINGn.qn_new(), _BYJO_SCANNINGn_new);
    /*#p29.AROR*/ wire _AROR_MATCH_ENp_new = and2(_AZEM_RENDERINGp_new, pix_pipe.XYLO_LCDC_SPENn.qn_new());
    /* p29.YDUG*/ wire _YDUG_STORE0_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _ZAKO_STORE0_MATCHAp_new, _XEBA_STORE0_MATCHBp_new);
    /* p29.DYDU*/ wire _DYDU_STORE1_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _EWAM_STORE1_MATCHAp_new, _CYVY_STORE1_MATCHBp_new);
    /* p29.DEGO*/ wire _DEGO_STORE2_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _CEHU_STORE2_MATCHAp_new, _EKES_STORE2_MATCHBp_new);
    /* p29.YLOZ*/ wire _YLOZ_STORE3_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _ZURE_STORE3_MATCHAp_new, _YWOS_STORE3_MATCHBp_new);
    /* p29.XAGE*/ wire _XAGE_STORE4_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _YKOK_STORE4_MATCHAp_new, _YNAZ_STORE4_MATCHBp_new);
    /* p29.EGOM*/ wire _EGOM_STORE5_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _COGY_STORE5_MATCHAp_new, _FYMA_STORE5_MATCHBp_new);
    /* p29.YBEZ*/ wire _YBEZ_STORE6_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _YWAP_STORE6_MATCHAp_new, _YDOT_STORE6_MATCHBp_new);
    /* p29.DYKA*/ wire _DYKA_STORE7_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _CYCO_STORE7_MATCHAp_new, _DAJE_STORE7_MATCHBp_new);
    /* p29.EFYL*/ wire _EFYL_STORE8_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _DAMA_STORE8_MATCHAp_new, _FEHA_STORE8_MATCHBp_new);
    /* p29.YGEM*/ wire _YGEM_STORE9_MATCHn_new = nand3(_AROR_MATCH_ENp_new, _YLEV_STORE9_MATCHAp_new, _YTUB_STORE9_MATCHBp_new);

    /* p29.FEFY*/ wire _FEFY_STORE_MATCHp_new = nand5(_XAGE_STORE4_MATCHn_new, _YLOZ_STORE3_MATCHn_new, _DEGO_STORE2_MATCHn_new, _DYDU_STORE1_MATCHn_new, _YDUG_STORE0_MATCHn_new);
    /* p29.FOVE*/ wire _FOVE_STORE_MATCHp_new = nand5(_YGEM_STORE9_MATCHn_new, _EFYL_STORE8_MATCHn_new, _DYKA_STORE7_MATCHn_new, _YBEZ_STORE6_MATCHn_new, _EGOM_STORE5_MATCHn_new);
    /* p29.FEPO*/ wire _FEPO_STORE_MATCHp_new = or2(_FEFY_STORE_MATCHp_new, _FOVE_STORE_MATCHp_new);
    /*#p21.XENA*/ wire _XENA_STORE_MATCHn_new = not1(_FEPO_STORE_MATCHp_new);

    FEPO_STORE_MATCHp.set(_FEPO_STORE_MATCHp_new);

    /*#p21.XUGU*/ wire _XUGU_PX167n_new = nand5(pix_pipe.XEHO_PX0p.qp_new(), pix_pipe.SAVY_PX1p.qp_new(), pix_pipe.XODU_PX2p.qp_new(), pix_pipe.TUKY_PX5p.qp_new(), pix_pipe.SYBE_PX7p.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire _XANO_PX167p_new = not1(_XUGU_PX167n_new);
    /*#p21.WODU*/ wire _WODU_HBLANKp_new = and2(_XENA_STORE_MATCHn_new, _XANO_PX167p_new);

    WODU_HBLANKp.set(_WODU_HBLANKp_new);


    /* p29.WEFU*/ wire _WEFU_STORE0_MATCH_new = not1(_YDUG_STORE0_MATCHn_new);
    /* p29.GAJA*/ wire _GAJA_STORE1_MATCH_new = not1(_DYDU_STORE1_MATCHn_new);
    /* p29.GUPO*/ wire _GUPO_STORE2_MATCH_new = not1(_DEGO_STORE2_MATCHn_new);
    /* p29.WEBO*/ wire _WEBO_STORE3_MATCH_new = not1(_YLOZ_STORE3_MATCHn_new);
    /* p29.WUNA*/ wire _WUNA_STORE4_MATCH_new = not1(_XAGE_STORE4_MATCHn_new);
    /* p29.GABA*/ wire _GABA_STORE5_MATCH_new = not1(_EGOM_STORE5_MATCHn_new);
    /* p29.WASE*/ wire _WASE_STORE6_MATCH_new = not1(_YBEZ_STORE6_MATCHn_new);
    /* p29.GYTE*/ wire _GYTE_STORE7_MATCH_new = not1(_DYKA_STORE7_MATCHn_new);
    /* p29.GEKE*/ wire _GEKE_STORE8_MATCH_new = not1(_EFYL_STORE8_MATCHn_new);

    /* p29.GEZE*/ wire _GEZE_STORE0_MATCH_new = or2(_WEFU_STORE0_MATCH_new, GND);
    /* p29.FUMA*/ wire _FUMA_STORE1_MATCH_new = or2(_GAJA_STORE1_MATCH_new, _GEZE_STORE0_MATCH_new);
    /* p29.GEDE*/ wire _GEDE_STORE2_MATCH_new = or2(_GUPO_STORE2_MATCH_new, _FUMA_STORE1_MATCH_new);
    /* p29.WUTO*/ wire _WUTO_STORE3_MATCH_new = or2(_WEBO_STORE3_MATCH_new, _GEDE_STORE2_MATCH_new);
    /* p29.XYLA*/ wire _XYLA_STORE4_MATCH_new = or2(_WUNA_STORE4_MATCH_new, _WUTO_STORE3_MATCH_new);
    /* p29.WEJA*/ wire _WEJA_STORE5_MATCH_new = or2(_GABA_STORE5_MATCH_new, _XYLA_STORE4_MATCH_new);
    /* p29.WYLA*/ wire _WYLA_STORE6_MATCH_new = or2(_WASE_STORE6_MATCH_new, _WEJA_STORE5_MATCH_new);
    /* p29.FAVO*/ wire _FAVO_STORE7_MATCH_new = or2(_GYTE_STORE7_MATCH_new, _WYLA_STORE6_MATCH_new);
    /* p29.GYGA*/ wire _GYGA_STORE8_MATCH_new = or2(_GEKE_STORE8_MATCH_new, _FAVO_STORE7_MATCH_new);

    /* p29.GUVA*/ wire _GUVA_SPRITE0_GETp_new = nor2(_YDUG_STORE0_MATCHn_new, GND);
    /* p29.ENUT*/ wire _ENUT_SPRITE1_GETp_new = nor2(_DYDU_STORE1_MATCHn_new, _GEZE_STORE0_MATCH_new);
    /* p29.EMOL*/ wire _EMOL_SPRITE2_GETp_new = nor2(_DEGO_STORE2_MATCHn_new, _FUMA_STORE1_MATCH_new);
    /* p29.GYFY*/ wire _GYFY_SPRITE3_GETp_new = nor2(_YLOZ_STORE3_MATCHn_new, _GEDE_STORE2_MATCH_new);
    /* p29.GONO*/ wire _GONO_SPRITE4_GETp_new = nor2(_XAGE_STORE4_MATCHn_new, _WUTO_STORE3_MATCH_new);
    /* p29.GEGA*/ wire _GEGA_SPRITE5_GETp_new = nor2(_EGOM_STORE5_MATCHn_new, _XYLA_STORE4_MATCH_new);
    /* p29.XOJA*/ wire _XOJA_SPRITE6_GETp_new = nor2(_YBEZ_STORE6_MATCHn_new, _WEJA_STORE5_MATCH_new);
    /* p29.GUTU*/ wire _GUTU_SPRITE7_GETp_new = nor2(_DYKA_STORE7_MATCHn_new, _WYLA_STORE6_MATCH_new);
    /* p29.FOXA*/ wire _FOXA_SPRITE8_GETp_new = nor2(_EFYL_STORE8_MATCHn_new, _FAVO_STORE7_MATCH_new);
    /* p29.GUZE*/ wire _GUZE_SPRITE9_GETp_new = nor2(_YGEM_STORE9_MATCHn_new, _GYGA_STORE8_MATCH_new);

    GUVA_SPRITE0_GETp.set(_GUVA_SPRITE0_GETp_new);
    ENUT_SPRITE1_GETp.set(_ENUT_SPRITE1_GETp_new);
    EMOL_SPRITE2_GETp.set(_EMOL_SPRITE2_GETp_new);
    GYFY_SPRITE3_GETp.set(_GYFY_SPRITE3_GETp_new);
    GONO_SPRITE4_GETp.set(_GONO_SPRITE4_GETp_new);
    GEGA_SPRITE5_GETp.set(_GEGA_SPRITE5_GETp_new);
    XOJA_SPRITE6_GETp.set(_XOJA_SPRITE6_GETp_new);
    GUTU_SPRITE7_GETp.set(_GUTU_SPRITE7_GETp_new);
    FOXA_SPRITE8_GETp.set(_FOXA_SPRITE8_GETp_new);
    GUZE_SPRITE9_GETp.set(_GUZE_SPRITE9_GETp_new);

    /* p29.FURO*/ wire _FURO_SPRITE0_GETn_new = not1(_GUVA_SPRITE0_GETp_new);
    /* p29.DYDO*/ wire _DYDO_SPRITE1_GETn_new = not1(_ENUT_SPRITE1_GETp_new);
    /* p29.FAME*/ wire _FAME_SPRITE2_GETn_new = not1(_EMOL_SPRITE2_GETp_new);
    /* p29.GYMA*/ wire _GYMA_SPRITE3_GETn_new = not1(_GYFY_SPRITE3_GETp_new);
    /* p29.GOWO*/ wire _GOWO_SPRITE4_GETn_new = not1(_GONO_SPRITE4_GETp_new);
    /* p29.GYGY*/ wire _GYGY_SPRITE5_GETn_new = not1(_GEGA_SPRITE5_GETp_new);
    /* p29.XYME*/ wire _XYME_SPRITE6_GETn_new = not1(_XOJA_SPRITE6_GETp_new);
    /* p29.GUGY*/ wire _GUGY_SPRITE7_GETn_new = not1(_GUTU_SPRITE7_GETp_new);
    /* p29.DENY*/ wire _DENY_SPRITE8_GETn_new = not1(_FOXA_SPRITE8_GETp_new);
    /* p29.FADO*/ wire _FADO_SPRITE9_GETn_new = not1(_GUZE_SPRITE9_GETp_new);

    /* p27.TUKU*/ wire _TUKU_WIN_HITn_new = not1(_TOMU_WIN_HITp_new);
    /* p27.SOWO*/ wire _SOWO_SFETCH_RUNNINGn_new = not1(sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp_new());
    /* p27.TEKY*/ TEKY_SFETCH_REQp.set(and4(_FEPO_STORE_MATCHp_new, _TUKU_WIN_HITn_new, LYRY_BFETCH_DONEp.qp_new(), _SOWO_SFETCH_RUNNINGn_new));

    /* p30.ZETU*/ SPR_TRI_I_out_new[0].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.YGUS_STORE0_I0n.qp_new());
    /* p30.ZECE*/ SPR_TRI_I_out_new[1].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.YSOK_STORE0_I1n.qp_new());
    /* p30.ZAVE*/ SPR_TRI_I_out_new[2].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.YZEP_STORE0_I2n.qp_new());
    /* p30.WOKO*/ SPR_TRI_I_out_new[3].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.WYTE_STORE0_I3n.qp_new());
    /* p30.ZUMU*/ SPR_TRI_I_out_new[4].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.ZONY_STORE0_I4n.qp_new());
    /*#p30.ZEDY*/ SPR_TRI_I_out_new[5].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.YWAK_STORE0_I5n.qp_new());
    /* p30.WEHE*/ SPR_TRI_L_out_new[0].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.GYHO_STORE0_L0n.qp_new());
    /* p30.BUKY*/ SPR_TRI_L_out_new[1].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.CUFO_STORE0_L1n.qp_new());
    /* p30.AJAL*/ SPR_TRI_L_out_new[2].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.BOZU_STORE0_L2n.qp_new());
    /* p30.GOFO*/ SPR_TRI_L_out_new[3].tri6_nn(_FURO_SPRITE0_GETn_new, sprite_store.FYHY_STORE0_L3n.qp_new());

    /*#p30.CUBO*/ SPR_TRI_I_out_new[0].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.CADU_STORE1_I0n.qp_new());
    /* p30.CELU*/ SPR_TRI_I_out_new[1].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.CEBO_STORE1_I1n.qp_new());
    /* p30.CEGY*/ SPR_TRI_I_out_new[2].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.CUFA_STORE1_I2n.qp_new());
    /* p30.BETY*/ SPR_TRI_I_out_new[3].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.COMA_STORE1_I3n.qp_new());
    /* p30.CYBY*/ SPR_TRI_I_out_new[4].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.CUZA_STORE1_I4n.qp_new());
    /* p30.BEMO*/ SPR_TRI_I_out_new[5].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.CAJY_STORE1_I5n.qp_new());
    /* p30.BYRO*/ SPR_TRI_L_out_new[0].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.AMES_STORE1_L0n.qp_new());
    /* p30.AHUM*/ SPR_TRI_L_out_new[1].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.AROF_STORE1_L1n.qp_new());
    /* p30.BACO*/ SPR_TRI_L_out_new[2].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.ABOP_STORE1_L2n.qp_new());
    /* p30.BEFE*/ SPR_TRI_L_out_new[3].tri6_nn(_DYDO_SPRITE1_GETn_new, sprite_store.ABUG_STORE1_L3n.qp_new());

    /* p30.CUBE*/ SPR_TRI_I_out_new[0].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.BUHE_STORE2_I0n.qp_new());
    /* p30.AFOZ*/ SPR_TRI_I_out_new[1].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.BYHU_STORE2_I1n.qp_new());
    /* p30.APON*/ SPR_TRI_I_out_new[2].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.BECA_STORE2_I2n.qp_new());
    /* p30.CUVU*/ SPR_TRI_I_out_new[3].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.BULU_STORE2_I3n.qp_new());
    /* p30.CYRO*/ SPR_TRI_I_out_new[4].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.BUNA_STORE2_I4n.qp_new());
    /* p30.AXEC*/ SPR_TRI_I_out_new[5].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.BOXA_STORE2_I5n.qp_new());
    /* p30.ZUKE*/ SPR_TRI_L_out_new[0].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.YLOV_STORE2_L0n.qp_new());
    /* p30.WERE*/ SPR_TRI_L_out_new[1].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.XOSY_STORE2_L1n.qp_new());
    /* p30.WUXE*/ SPR_TRI_L_out_new[2].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.XAZY_STORE2_L2n.qp_new());
    /* p30.ZABY*/ SPR_TRI_L_out_new[3].tri6_nn(_FAME_SPRITE2_GETn_new, sprite_store.YKUK_STORE2_L3n.qp_new());

    /* p30.ENAP*/ SPR_TRI_I_out_new[0].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.DEVY_STORE3_I0n.qp_new());
    /* p30.DYGO*/ SPR_TRI_I_out_new[1].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.DESE_STORE3_I1n.qp_new());
    /* p30.DOWA*/ SPR_TRI_I_out_new[2].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.DUNY_STORE3_I2n.qp_new());
    /* p30.DONY*/ SPR_TRI_I_out_new[3].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.DUHA_STORE3_I3n.qp_new());
    /* p30.EFUD*/ SPR_TRI_I_out_new[4].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.DEBA_STORE3_I4n.qp_new());
    /* p30.DEZU*/ SPR_TRI_I_out_new[5].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.DAFU_STORE3_I5n.qp_new());
    /* p30.ZEXE*/ SPR_TRI_L_out_new[0].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.ZURO_STORE3_L0n.qp_new());
    /* p30.YWAV*/ SPR_TRI_L_out_new[1].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.ZYLU_STORE3_L1n.qp_new());
    /* p30.YJEM*/ SPR_TRI_L_out_new[2].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.ZENE_STORE3_L2n.qp_new());
    /* p30.ZYPO*/ SPR_TRI_L_out_new[3].tri6_nn(_GYMA_SPRITE3_GETn_new, sprite_store.ZURY_STORE3_L3n.qp_new());

    /* p30.WUXU*/ SPR_TRI_I_out_new[0].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.XAVE_STORE4_I0n.qp_new());
    /* p30.WEPY*/ SPR_TRI_I_out_new[1].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.XEFE_STORE4_I1n.qp_new());
    /* p30.WERU*/ SPR_TRI_I_out_new[2].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.WANU_STORE4_I2n.qp_new());
    /* p30.XYRE*/ SPR_TRI_I_out_new[3].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.XABO_STORE4_I3n.qp_new());
    /* p30.WOXY*/ SPR_TRI_I_out_new[4].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.XEGE_STORE4_I4n.qp_new());
    /* p30.WAJA*/ SPR_TRI_I_out_new[5].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.XYNU_STORE4_I5n.qp_new());
    /* p30.BUCE*/ SPR_TRI_L_out_new[0].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.CAPO_STORE4_L0n.qp_new());
    /* p30.BEVY*/ SPR_TRI_L_out_new[1].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.CAJU_STORE4_L1n.qp_new());
    /* p30.BOVE*/ SPR_TRI_L_out_new[2].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.CONO_STORE4_L2n.qp_new());
    /* p30.BYDO*/ SPR_TRI_L_out_new[3].tri6_nn(_GOWO_SPRITE4_GETn_new, sprite_store.CUMU_STORE4_L3n.qp_new());

    /* p30.DOBO*/ SPR_TRI_I_out_new[0].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.EKOP_STORE5_I0n.qp_new());
    /* p30.DYNY*/ SPR_TRI_I_out_new[1].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.ETYM_STORE5_I1n.qp_new());
    /* p30.WAGA*/ SPR_TRI_I_out_new[2].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.GORU_STORE5_I2n.qp_new());
    /* p30.DUZA*/ SPR_TRI_I_out_new[3].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.EBEX_STORE5_I3n.qp_new());
    /* p30.DALY*/ SPR_TRI_I_out_new[4].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.ETAV_STORE5_I4n.qp_new());
    /* p30.DALO*/ SPR_TRI_I_out_new[5].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.EKAP_STORE5_I5n.qp_new());
    /* p30.BACE*/ SPR_TRI_L_out_new[0].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.ACEP_STORE5_L0n.qp_new());
    /* p30.BUJA*/ SPR_TRI_L_out_new[1].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.ABEG_STORE5_L1n.qp_new());
    /* p30.BODU*/ SPR_TRI_L_out_new[2].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.ABUX_STORE5_L2n.qp_new());
    /* p30.AWAT*/ SPR_TRI_L_out_new[3].tri6_nn(_GYGY_SPRITE5_GETn_new, sprite_store.ANED_STORE5_L3n.qp_new());

    /* p30.WATO*/ SPR_TRI_I_out_new[0].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.GABO_STORE6_I0n.qp_new());
    /* p30.WYWY*/ SPR_TRI_I_out_new[1].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.GACY_STORE6_I1n.qp_new());
    /* p30.EZOC*/ SPR_TRI_I_out_new[2].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.FOGO_STORE6_I2n.qp_new());
    /* p30.WABO*/ SPR_TRI_I_out_new[3].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.GOHU_STORE6_I3n.qp_new());
    /* p30.ELYC*/ SPR_TRI_I_out_new[4].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.FOXY_STORE6_I4n.qp_new());
    /* p30.WOCY*/ SPR_TRI_I_out_new[5].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.GECU_STORE6_I5n.qp_new());
    /* p30.YBUK*/ SPR_TRI_L_out_new[0].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.ZUMY_STORE6_L0n.qp_new());
    /* p30.YKOZ*/ SPR_TRI_L_out_new[1].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.ZAFU_STORE6_L1n.qp_new());
    /* p30.ZYTO*/ SPR_TRI_L_out_new[2].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.ZEXO_STORE6_L2n.qp_new());
    /* p30.ZUDO*/ SPR_TRI_L_out_new[3].tri6_nn(_XYME_SPRITE6_GETn_new, sprite_store.ZUBE_STORE6_L3n.qp_new());

    /* p30.WAKO*/ SPR_TRI_I_out_new[0].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.GULE_STORE7_I0n.qp_new());
    /* p30.WYGO*/ SPR_TRI_I_out_new[1].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.GYNO_STORE7_I1n.qp_new());
    /* p30.ELEP*/ SPR_TRI_I_out_new[2].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.FEFA_STORE7_I2n.qp_new());
    /* p30.ETAD*/ SPR_TRI_I_out_new[3].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.FYSU_STORE7_I3n.qp_new());
    /* p30.WABA*/ SPR_TRI_I_out_new[4].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.GESY_STORE7_I4n.qp_new());
    /* p30.EVYT*/ SPR_TRI_I_out_new[5].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.FUZO_STORE7_I5n.qp_new());
    /* p30.WAXE*/ SPR_TRI_L_out_new[0].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.XYNA_STORE7_L0n.qp_new());
    /* p30.YPOZ*/ SPR_TRI_L_out_new[1].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.YGUM_STORE7_L1n.qp_new());
    /* p30.WABU*/ SPR_TRI_L_out_new[2].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.XAKU_STORE7_L2n.qp_new());
    /* p30.WANA*/ SPR_TRI_L_out_new[3].tri6_nn(_GUGY_SPRITE7_GETn_new, sprite_store.XYGO_STORE7_L3n.qp_new());

    /* p30.APOC*/ SPR_TRI_I_out_new[0].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.AXUV_STORE8_I0n.qp_new());
    /* p30.AKYH*/ SPR_TRI_I_out_new[1].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.BADA_STORE8_I1n.qp_new());
    /* p30.AFEN*/ SPR_TRI_I_out_new[2].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.APEV_STORE8_I2n.qp_new());
    /* p30.APYV*/ SPR_TRI_I_out_new[3].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.BADO_STORE8_I3n.qp_new());
    /* p30.APOB*/ SPR_TRI_I_out_new[4].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.BEXY_STORE8_I4n.qp_new());
    /* p30.ADYB*/ SPR_TRI_I_out_new[5].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.BYHE_STORE8_I5n.qp_new());
    /* p30.BOSO*/ SPR_TRI_L_out_new[0].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.AZAP_STORE8_L0n.qp_new());
    /* p30.BAZU*/ SPR_TRI_L_out_new[1].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.AFYX_STORE8_L1n.qp_new());
    /* p30.AHAC*/ SPR_TRI_L_out_new[2].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.AFUT_STORE8_L2n.qp_new());
    /* p30.BUJY*/ SPR_TRI_L_out_new[3].tri6_nn(_DENY_SPRITE8_GETn_new, sprite_store.AFYM_STORE8_L3n.qp_new());

    /* p30.ZARO*/ SPR_TRI_I_out_new[0].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.YBER_STORE9_I0n.qp_new());
    /* p30.ZOJY*/ SPR_TRI_I_out_new[1].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.YZOR_STORE9_I1n.qp_new());
    /* p30.YNEV*/ SPR_TRI_I_out_new[2].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.XYFE_STORE9_I2n.qp_new());
    /* p30.XYRA*/ SPR_TRI_I_out_new[3].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.XOTU_STORE9_I3n.qp_new());
    /* p30.YRAD*/ SPR_TRI_I_out_new[4].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.XUTE_STORE9_I4n.qp_new());
    /* p30.YHAL*/ SPR_TRI_I_out_new[5].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.XUFO_STORE9_I5n.qp_new());
    /* p30.BYME*/ SPR_TRI_L_out_new[0].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.CANA_STORE9_L0n.qp_new());
    /* p30.GATE*/ SPR_TRI_L_out_new[1].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.FOFO_STORE9_L1n.qp_new());
    /* p30.COHO*/ SPR_TRI_L_out_new[2].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.DYSY_STORE9_L2n.qp_new());
    /* p30.CAWO*/ SPR_TRI_L_out_new[3].tri6_nn(_FADO_SPRITE9_GETn_new, sprite_store.DEWU_STORE9_L3n.qp_new());

    /*#p29.EBOS*/ wire _EBOS_LY0n_new = not1(lcd_reg.MUWY_LY0p.qp_new());
    /* p29.DASA*/ wire _DASA_LY1n_new = not1(lcd_reg.MYRO_LY1p.qp_new());
    /* p29.FUKY*/ wire _FUKY_LY2n_new = not1(lcd_reg.LEXA_LY2p.qp_new());
    /* p29.FUVE*/ wire _FUVE_LY3n_new = not1(lcd_reg.LYDO_LY3p.qp_new());
    ///* p29.FEPU*/ wire _FEPU_LY4n_new = not1(lcd_reg.LOVU_LY4p.qp_new());
    ///* p29.FOFA*/ wire _FOFA_LY5n_new = not1(lcd_reg.LEMA_LY5p.qp_new());
    ///* p29.FEMO*/ wire _FEMO_LY6n_new = not1(lcd_reg.MATO_LY6p.qp_new());
    ///* p29.GUSU*/ wire _GUSU_LY7n_new = not1(lcd_reg.LAFO_LY7p.qp_new());

    /* p29.ERUC*/ wire _ERUC_YDIFF_S0_new = add_s(_EBOS_LY0n_new, oam_bus.XUSO_OAM_DA0p.qp_new(), GND);
    /* p29.ERUC*/ wire _ERUC_YDIFF_C0_new = add_c(_EBOS_LY0n_new, oam_bus.XUSO_OAM_DA0p.qp_new(), GND);
    /* p29.ENEF*/ wire _ENEF_YDIFF_S1_new = add_s(_DASA_LY1n_new, oam_bus.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
    /* p29.ENEF*/ wire _ENEF_YDIFF_C1_new = add_c(_DASA_LY1n_new, oam_bus.XEGU_OAM_DA1p.qp_new(), _ERUC_YDIFF_C0_new);
    /* p29.FECO*/ wire _FECO_YDIFF_S2_new = add_s(_FUKY_LY2n_new, oam_bus.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
    /* p29.FECO*/ wire _FECO_YDIFF_C2_new = add_c(_FUKY_LY2n_new, oam_bus.YJEX_OAM_DA2p.qp_new(), _ENEF_YDIFF_C1_new);
    /* p29.GYKY*/ wire _GYKY_YDIFF_S3_new = add_s(_FUVE_LY3n_new, oam_bus.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF_C2_new);
    ///* p29.GYKY*/ wire _GYKY_YDIFF_C3_new = add_c(_FUVE_LY3n_new, oam_bus.XYJU_OAM_DA3p.qp_new(), _FECO_YDIFF_C2_new);
    ///* p29.GOPU*/ wire _GOPU_YDIFF_S4_new = add_s(_FEPU_LY4n_new, oam_bus.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF_C3_new);
    ///* p29.GOPU*/ wire _GOPU_YDIFF_C4_new = add_c(_FEPU_LY4n_new, oam_bus.YBOG_OAM_DA4p.qp_new(), _GYKY_YDIFF_C3_new);
    ///* p29.FUWA*/ wire _FUWA_YDIFF_S5_new = add_s(_FOFA_LY5n_new, oam_bus.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF_C4_new);
    ///* p29.FUWA*/ wire _FUWA_YDIFF_C5_new = add_c(_FOFA_LY5n_new, oam_bus.WYSO_OAM_DA5p.qp_new(), _GOPU_YDIFF_C4_new);
    ///* p29.GOJU*/ wire _GOJU_YDIFF_S6_new = add_s(_FEMO_LY6n_new, oam_bus.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF_C5_new);
    ///* p29.GOJU*/ wire _GOJU_YDIFF_C6_new = add_c(_FEMO_LY6n_new, oam_bus.XOTE_OAM_DA6p.qp_new(), _FUWA_YDIFF_C5_new);
    ///* p29.WUHU*/ wire _WUHU_YDIFF_S7_new = add_s(_GUSU_LY7n_new, oam_bus.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF_C6_new);
    ///* p29.WUHU*/ wire _WUHU_YDIFF_C7_new = add_c(_GUSU_LY7n_new, oam_bus.YZAB_OAM_DA7p.qp_new(), _GOJU_YDIFF_C6_new);

    ///* p29.GACE*/ wire _GACE_SPRITE_DELTA4_new = not1(_GOPU_YDIFF_S4_new);
    ///* p29.GUVU*/ wire _GUVU_SPRITE_DELTA5_new = not1(_FUWA_YDIFF_S5_new);
    ///* p29.GYDA*/ wire _GYDA_SPRITE_DELTA6_new = not1(_GOJU_YDIFF_S6_new);
    ///* p29.GEWY*/ wire _GEWY_SPRITE_DELTA7_new = not1(_WUHU_YDIFF_S7_new);
    ///*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH_new = or2(pix_pipe.XYMO_LCDC_SPSIZEn.qn_new(), _GYKY_YDIFF_S3_new);

    ///* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn_new = nand6(_GACE_SPRITE_DELTA4_new, _GUVU_SPRITE_DELTA5_new, _GYDA_SPRITE_DELTA6_new, _GEWY_SPRITE_DELTA7_new, _WUHU_YDIFF_C7_new, _GOVU_SPSIZE_MATCH_new);

    /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn_new = and2(sprite_scanner.CENO_SCANNINGp.qn_new(), pix_pipe.XYMU_RENDERINGn.qn_new());

    SPR_TRI_I_in_new[0].reset();
    SPR_TRI_I_in_new[1].reset();
    SPR_TRI_I_in_new[2].reset();
    SPR_TRI_I_in_new[3].reset();
    SPR_TRI_I_in_new[4].reset();
    SPR_TRI_I_in_new[5].reset();
    SPR_TRI_L_in_new[0].reset();
    SPR_TRI_L_in_new[1].reset();
    SPR_TRI_L_in_new[2].reset();
    SPR_TRI_L_in_new[3].reset();

    /*#p30.WUZY*/ SPR_TRI_I_in_new[0].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XADU_SPRITE_IDX0p.qn_new());
    /* p30.WYSE*/ SPR_TRI_I_in_new[1].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XEDY_SPRITE_IDX1p.qn_new());
    /* p30.ZYSU*/ SPR_TRI_I_in_new[2].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.ZUZE_SPRITE_IDX2p.qn_new());
    /* p30.WYDA*/ SPR_TRI_I_in_new[3].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XOBE_SPRITE_IDX3p.qn_new());
    /* p30.WUCO*/ SPR_TRI_I_in_new[4].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.YDUF_SPRITE_IDX4p.qn_new());
    /* p30.WEZA*/ SPR_TRI_I_in_new[5].tri6_nn(_BUZA_STORE_SPRITE_INDXn_new, sprite_scanner.XECU_SPRITE_IDX5p.qn_new());

    /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0_new = not1(_ERUC_YDIFF_S0_new);
    /* p29.DABY*/ wire _DABY_SPRITE_DELTA1_new = not1(_ENEF_YDIFF_S1_new);
    /* p29.DABU*/ wire _DABU_SPRITE_DELTA2_new = not1(_FECO_YDIFF_S2_new);
    /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3_new = not1(_GYKY_YDIFF_S3_new);

    /*#p30.CUCU*/ SPR_TRI_L_in_new[0].tri6_nn(_FEPO_STORE_MATCHp_new, _DEGE_SPRITE_DELTA0_new);
    /*#p30.CUCA*/ SPR_TRI_L_in_new[1].tri6_nn(_FEPO_STORE_MATCHp_new, _DABY_SPRITE_DELTA1_new);
    /*#p30.CEGA*/ SPR_TRI_L_in_new[2].tri6_nn(_FEPO_STORE_MATCHp_new, _DABU_SPRITE_DELTA2_new);
    /*#p30.WENU*/ SPR_TRI_L_in_new[3].tri6_nn(_FEPO_STORE_MATCHp_new, _GYSA_SPRITE_DELTA3_new);
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  [this, _ALUR_SYS_RSTn_new, _ROLO_FF0F_RDn_ext, _REFA_FF0F_WRn_clk, FFFF_RDn_ext, FFFF_WRn_ext]() {
    // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
    // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
    // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
    // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
    // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

    /* p02.LETY*/ wire _LETY_INT_VBL_ACKn_ext  = not1(PIN_CPU_ACK_VBLANK.qp_ext());
    /* p02.LEJA*/ wire _LEJA_INT_STAT_ACKn_ext = not1(PIN_CPU_ACK_STAT.qp_ext());
    /* p02.LESA*/ wire _LESA_INT_TIM_ACKn_ext  = not1(PIN_CPU_ACK_TIMER.qp_ext());
    /* p02.LUFE*/ wire _LUFE_INT_SER_ACKn_ext  = not1(PIN_CPU_ACK_SERIAL.qp_ext());
    /* p02.LAMO*/ wire _LAMO_INT_JOY_ACKn_ext  = not1(PIN_CPU_ACK_JOYPAD.qp_ext());

    /* p02.ROTU*/ wire _ROTU_FF0F_WRp_clk = not1(_REFA_FF0F_WRn_clk);
    /* p02.MYZU*/ wire _MYZU_FF0F_SET0n_new = nand3(_ROTU_FF0F_WRp_clk, _LETY_INT_VBL_ACKn_ext,  BUS_CPU_Dp_ext[0].qp_ext());
    /* p02.MODY*/ wire _MODY_FF0F_SET1n_new = nand3(_ROTU_FF0F_WRp_clk, _LEJA_INT_STAT_ACKn_ext, BUS_CPU_Dp_ext[1].qp_ext());
    /* p02.PYHU*/ wire _PYHU_FF0F_SET2n_new = nand3(_ROTU_FF0F_WRp_clk, _LESA_INT_TIM_ACKn_ext,  BUS_CPU_Dp_ext[2].qp_ext());
    /* p02.TOME*/ wire _TOME_FF0F_SET3n_new = nand3(_ROTU_FF0F_WRp_clk, _LUFE_INT_SER_ACKn_ext,  BUS_CPU_Dp_ext[3].qp_ext());
    /* p02.TOGA*/ wire _TOGA_FF0F_SET4n_new = nand3(_ROTU_FF0F_WRp_clk, _LAMO_INT_JOY_ACKn_ext,  BUS_CPU_Dp_ext[4].qp_ext());

    /*#p02.MUXE*/ wire _MUXE_INT0_WRn_new = or2(BUS_CPU_Dp_ext[0].qp_ext(), _REFA_FF0F_WRn_clk);
    /* p02.NABE*/ wire _NABE_INT1_WRn_new = or2(BUS_CPU_Dp_ext[1].qp_ext(), _REFA_FF0F_WRn_clk);
    /* p02.RAKE*/ wire _RAKE_INT2_WRn_new = or2(BUS_CPU_Dp_ext[2].qp_ext(), _REFA_FF0F_WRn_clk);
    /* p02.SULO*/ wire _SULO_INT3_WRn_new = or2(BUS_CPU_Dp_ext[3].qp_ext(), _REFA_FF0F_WRn_clk);
    /* p02.SEME*/ wire _SEME_INT4_WRn_new = or2(BUS_CPU_Dp_ext[4].qp_ext(), _REFA_FF0F_WRn_clk);

    /*#p02.LYTA*/ wire _LYTA_FF0F_RST0n_new = and3(_MUXE_INT0_WRn_new, _LETY_INT_VBL_ACKn_ext,  _ALUR_SYS_RSTn_new);
    /* p02.MOVU*/ wire _MOVU_FF0F_RST1n_new = and3(_NABE_INT1_WRn_new, _LEJA_INT_STAT_ACKn_ext, _ALUR_SYS_RSTn_new);
    /* p02.PYGA*/ wire _PYGA_FF0F_RST2n_new = and3(_RAKE_INT2_WRn_new, _LESA_INT_TIM_ACKn_ext,  _ALUR_SYS_RSTn_new);
    /* p02.TUNY*/ wire _TUNY_FF0F_RST3n_new = and3(_SULO_INT3_WRn_new, _LUFE_INT_SER_ACKn_ext,  _ALUR_SYS_RSTn_new);
    /* p02.TYME*/ wire _TYME_FF0F_RST4n_new = and3(_SEME_INT4_WRn_new, _LAMO_INT_JOY_ACKn_ext,  _ALUR_SYS_RSTn_new);

    /*#p21.PARU*/ wire _PARU_VBLANKp_new = not1(lcd_reg.POPU_VBLANK_CLKp.qn_new());
    /*#p21.TOLU*/ wire _TOLU_VBLANKn_new    = not1(_PARU_VBLANKp_new);
    /*#p21.PURE*/ wire _PURE_LINE_ENDn_new = not1(lcd_reg.RUTU_x113p.qp_new());
    /*#p21.SELA*/ wire _SELA_LINE_ENDp_new  = not1(_PURE_LINE_ENDn_new);
    /*#p21.TAPA*/ wire _TAPA_INT_OAM_t3_new   = and2(_TOLU_VBLANKn_new, _SELA_LINE_ENDp_new);
    /*#p21.TARU*/ wire _TARU_INT_HBL_t3_new   = and2(WODU_HBLANKp.qp_new(), _TOLU_VBLANKn_new);
    /*#p21.SUKO*/ wire _SUKO_INT_STATp_t3_new = amux4(pix_pipe.RUGU_STAT_LYI_ENn.qn_new(), lcd_reg.ROPO_LY_MATCH_SYNCp.qp_new(),
                                                      pix_pipe.REFE_STAT_OAI_ENn.qn_new(), _TAPA_INT_OAM_t3_new,
                                                      pix_pipe.RUFO_STAT_VBI_ENn.qn_new(), _PARU_VBLANKp_new, // polarity?
                                                      pix_pipe.ROXE_STAT_HBI_ENn.qn_new(), _TARU_INT_HBL_t3_new);

    /*#p21.VYPU*/ wire _VYPU_INT_VBLANKp_t3_new = not1(_TOLU_VBLANKn_new);
    /* p02.ASOK*/ wire _ASOK_INT_JOYp_new       = and2(joypad.APUG_JP_GLITCH3.qp_new(), joypad.BATU_JP_GLITCH0.qp_new());
    /*#p21.TUVA*/ wire _TUVA_INT_STATn_t3_new   = not1(_SUKO_INT_STATp_t3_new);
    /*#p21.VOTY*/ wire _VOTY_INT_STATp_t3_new   = not1(_TUVA_INT_STATn_t3_new);

    wire _PESU_VCC = 1;
    /* p02.LOPE*/ int_reg.LOPE_FF0F_D0p.dff22(_VYPU_INT_VBLANKp_t3_new,              _MYZU_FF0F_SET0n_new, _LYTA_FF0F_RST0n_new, _PESU_VCC);
    /* p02.LALU*/ int_reg.LALU_FF0F_D1p.dff22(_VOTY_INT_STATp_t3_new,                _MODY_FF0F_SET1n_new, _MOVU_FF0F_RST1n_new, _PESU_VCC);
    /* p02.NYBO*/ int_reg.NYBO_FF0F_D2p.dff22(tim_reg.MOBA_TIMER_OVERFLOWp.qp_new(), _PYHU_FF0F_SET2n_new, _PYGA_FF0F_RST2n_new, _PESU_VCC);
    /* p02.UBUL*/ int_reg.UBUL_FF0F_D3p.dff22(ser_reg.CALY_SER_CNT3.qp_new(),        _TOME_FF0F_SET3n_new, _TUNY_FF0F_RST3n_new, _PESU_VCC);
    /* p02.ULAK*/ int_reg.ULAK_FF0F_D4p.dff22(_ASOK_INT_JOYp_new,                    _TOGA_FF0F_SET4n_new, _TYME_FF0F_RST4n_new, _PESU_VCC);

    // FIXME this inversion fixes a bunch of tests...
    // MATY is one of those big yellow latchy things
    /* p02.MATY*/ int_reg.MATY_FF0F_L0p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.LOPE_FF0F_D0p.qp_new()); // OUTPUT ON RUNG 10
    /* p02.MOPO*/ int_reg.MOPO_FF0F_L1p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.LALU_FF0F_D1p.qp_new()); // OUTPUT ON RUNG 10
    /* p02.PAVY*/ int_reg.PAVY_FF0F_L2p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.NYBO_FF0F_D2p.qp_new()); // OUTPUT ON RUNG 10
    /* p02.NEJY*/ int_reg.NEJY_FF0F_L3p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.UBUL_FF0F_D3p.qp_new()); // OUTPUT ON RUNG 10
    /* p02.NUTY*/ int_reg.NUTY_FF0F_L4p.tp_latch(!_ROLO_FF0F_RDn_ext, int_reg.ULAK_FF0F_D4p.qp_new()); // OUTPUT ON RUNG 10

    /* FF0F INTF */
    /* p02.POLA*/ wire _POLA_FF0F_RDp_ext = not1(_ROLO_FF0F_RDn_ext);
    /*#p02.NELA*/ BUS_CPU_Dp_out[0].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.MATY_FF0F_L0p.qn_new());
    /*#p02.NABO*/ BUS_CPU_Dp_out[1].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.MOPO_FF0F_L1p.qn_new());
    /*#p02.ROVA*/ BUS_CPU_Dp_out[2].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.PAVY_FF0F_L2p.qn_new());
    /*#p02.PADO*/ BUS_CPU_Dp_out[3].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.NEJY_FF0F_L3p.qn_new());
    /*#p02.PEGY*/ BUS_CPU_Dp_out[4].tri6_pn(_POLA_FF0F_RDp_ext, int_reg.NUTY_FF0F_L4p.qn_new());

    PIN_CPU_INT_VBLANK.pin_in(1, int_reg.LOPE_FF0F_D0p.qp_new());
    PIN_CPU_INT_STAT  .pin_in(1, int_reg.LALU_FF0F_D1p.qp_new());
    PIN_CPU_INT_TIMER .pin_in(1, int_reg.NYBO_FF0F_D2p.qp_new());
    PIN_CPU_INT_SERIAL.pin_in(1, int_reg.UBUL_FF0F_D3p.qp_new());
    PIN_CPU_INT_JOYPAD.pin_in(1, int_reg.ULAK_FF0F_D4p.qp_new());

    if (DELTA_DE) {
      int_vblank_halt = PIN_CPU_INT_VBLANK.qp_ext();
      int_stat_halt   = PIN_CPU_INT_STAT.qp_ext();
      int_serial_halt = PIN_CPU_INT_SERIAL.qp_ext();
      int_joypad_halt = PIN_CPU_INT_JOYPAD.qp_ext();
    }

    if (DELTA_GH) {
      // this one latches funny, some hardware bug
      int_timer_halt = PIN_CPU_INT_TIMER.qp_ext();

      int_vblank = PIN_CPU_INT_VBLANK.qp_ext();
      int_stat   = PIN_CPU_INT_STAT.qp_ext();
      int_timer  = PIN_CPU_INT_TIMER.qp_ext();
      int_serial = PIN_CPU_INT_SERIAL.qp_ext();
      int_joypad = PIN_CPU_INT_JOYPAD.qp_ext();
    }

    {
      // FFFF - IE
      // This is technically in the CPU, but we're going to implement it here for now.

      IE_D0.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_Dp_ext[0].qp_ext());
      IE_D1.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_Dp_ext[1].qp_ext());
      IE_D2.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_Dp_ext[2].qp_ext());
      IE_D3.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_Dp_ext[3].qp_ext());
      IE_D4.dff(FFFF_WRn_ext, 1, !sys_rst, BUS_CPU_Dp_ext[4].qp_ext());

      BUS_CPU_Dp_out[0].tri6_nn(FFFF_RDn_ext, IE_D0.qn_new());
      BUS_CPU_Dp_out[1].tri6_nn(FFFF_RDn_ext, IE_D1.qn_new());
      BUS_CPU_Dp_out[2].tri6_nn(FFFF_RDn_ext, IE_D2.qn_new());
      BUS_CPU_Dp_out[3].tri6_nn(FFFF_RDn_ext, IE_D3.qn_new());
      BUS_CPU_Dp_out[4].tri6_nn(FFFF_RDn_ext, IE_D4.qn_new());

      if (DELTA_HA) {
        imask_latch = pack_u8p_new(5, &IE_D0);
      }
    }
  }();

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  {
    /*#p25.XANE*/ wire _XANE_VRAM_LOCKn_new = nor2(_LUFA_DMA_VRAMp_new, pix_pipe.XYMU_RENDERINGn.qn_new());

    /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY_new = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /* p29.TEPA*/ wire _TEPA_RENDERINGn_new = not1(pix_pipe.XYMU_RENDERINGn.qn_new());
    /* p29.TYSO*/ wire _TYSO_SPRITE_READn_new = or2(_SAKY_SFETCH_MYSTERY_new, _TEPA_RENDERINGn_new); // def or
    /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp_new = not1(_TYSO_SPRITE_READn_new);

    /* p25.RYLU*/ wire _RYLU_CPU_VRAM_RDn_new = nand2(_SALE_CPU_VRAM_WRn_ext, _XANE_VRAM_LOCKn_new);
    /* p29.TYTU*/ wire _TYTU_SFETCH_S0n_new = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp_new());
    /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG_new = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp_new(), _TYTU_SFETCH_S0n_new);
    /* p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp_new = and2(_TACU_SPR_SEQ_5_TRIG_new, _TEXY_SPR_READ_VRAMp_new);
    /* p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn_new = not1(_SOHO_SPR_VRAM_RDp_new);

    /* p25.APAM*/ wire _APAM_DMA_VRAMn_new    = not1(_LUFA_DMA_VRAMp_new);
    /* p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn_new = not1(tile_fetcher.LONY_FETCHINGp.qp_new());
    /* p25.RACU*/ wire _RACU_MOEn_new = and4(_RYLU_CPU_VRAM_RDn_new, _RAWA_SPR_VRAM_RDn_new, _MYMA_BGW_VRAM_RDn_new, _APAM_DMA_VRAMn_new); // def and
    /* p25.TUTO*/ wire _TUTO_DBG_VRAMp_new = and2(_UNOR_MODE_DBG2p_ext, SOTO_DBG_VRAMp.qn_new());
    /*#p25.RACO*/ wire _RACO_DBG_VRAMn_new = not1(_TUTO_DBG_VRAMp_new);
    /* p25.SEMA*/ wire _SEMA_MOEn_A_new = and2(_RACU_MOEn_new, _RACO_DBG_VRAMn_new);
    /* p25.RUTE*/ wire _RUTE_MOEn_D_new = or2(_RACU_MOEn_new, _TUTO_DBG_VRAMp_new); // schematic wrong, second input is RACU
    /* p25.REFO*/ wire _REFO_MOEn_A_new = not1(_SEMA_MOEn_A_new);
    /* p25.SAHA*/ wire _SAHA_MOEn_D_new = not1(_RUTE_MOEn_D_new);

    PIN_VRAM_OEn.reset();
    PIN_VRAM_OEn.pin_out(1, _REFO_MOEn_A_new, _SAHA_MOEn_D_new);
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  Bus2 BUS_VRAM_An[13]; // not stable

  BUS_VRAM_An[ 0].reset();
  BUS_VRAM_An[ 1].reset();
  BUS_VRAM_An[ 2].reset();
  BUS_VRAM_An[ 3].reset();
  BUS_VRAM_An[ 4].reset();
  BUS_VRAM_An[ 5].reset();
  BUS_VRAM_An[ 6].reset();
  BUS_VRAM_An[ 7].reset();
  BUS_VRAM_An[ 8].reset();
  BUS_VRAM_An[ 9].reset();
  BUS_VRAM_An[10].reset();
  BUS_VRAM_An[11].reset();
  BUS_VRAM_An[12].reset();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // CPU VRAM read

  [this, &BUS_VRAM_An, BUS_CPU_A_ext]() {
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_new  = not1(dma_reg.MARU_DMA_A15n.qn_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_new = nor3(dma_reg.PULA_DMA_A13n.qn_new(), dma_reg.POKU_DMA_A14n.qn_new(), _LEBU_DMA_A15n_new);
    /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_new = nand2(dma_reg.MATU_DMA_RUNNINGp.qp_new(), _MUDA_DMA_VRAMp_new);
    /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_new = not1(_MUHO_DMA_VRAMp_new);
    /*#p25.XANE*/ wire _XANE_VRAM_LOCKn_new = nor2(_LUFA_DMA_VRAMp_new, pix_pipe.XYMU_RENDERINGn.qn_new());
    /* p25.XEDU*/ wire _XEDU_VRAM_LOCKp_new = not1(_XANE_VRAM_LOCKn_new);

    /* p25.XAKY*/ BUS_VRAM_An[ 0].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 0]);
    /* p25.XUXU*/ BUS_VRAM_An[ 1].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 1]);
    /* p25.XYNE*/ BUS_VRAM_An[ 2].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 2]);
    /* p25.XODY*/ BUS_VRAM_An[ 3].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 3]);
    /* p25.XECA*/ BUS_VRAM_An[ 4].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 4]);
    /* p25.XOBA*/ BUS_VRAM_An[ 5].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 5]);
    /* p25.XOPO*/ BUS_VRAM_An[ 6].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 6]);
    /* p25.XYBO*/ BUS_VRAM_An[ 7].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 7]);
    /* p25.RYSU*/ BUS_VRAM_An[ 8].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 8]);
    /* p25.RESE*/ BUS_VRAM_An[ 9].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[ 9]);
    /* p25.RUSE*/ BUS_VRAM_An[10].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[10]);
    /* p25.RYNA*/ BUS_VRAM_An[11].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[11]);
    /* p25.RUMO*/ BUS_VRAM_An[12].tri6_nn(_XEDU_VRAM_LOCKp_new, BUS_CPU_A_ext[12]);
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // DMA VRAM read

  [this, &BUS_VRAM_An]() {
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_new  = not1(dma_reg.MARU_DMA_A15n.qn_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_new = nor3(dma_reg.PULA_DMA_A13n.qn_new(), dma_reg.POKU_DMA_A14n.qn_new(), _LEBU_DMA_A15n_new);
    /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_new = nand2(dma_reg.MATU_DMA_RUNNINGp.qp_new(), _MUDA_DMA_VRAMp_new);
    /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_new = not1(_MUHO_DMA_VRAMp_new);

    /* p04.AHOC*/ wire _AHOC_DMA_VRAMn_new = not1(_LUFA_DMA_VRAMp_new);
    /* p04.NAKY*/ wire _NAKY_DMA_A00p_new = dma_reg.NAKY_DMA_A00p.qp_new();
    /* p04.PYRO*/ wire _PYRO_DMA_A01p_new = dma_reg.PYRO_DMA_A01p.qp_new();
    /* p04.NEFY*/ wire _NEFY_DMA_A02p_new = dma_reg.NEFY_DMA_A02p.qp_new();
    /* p04.MUTY*/ wire _MUTY_DMA_A03p_new = dma_reg.MUTY_DMA_A03p.qp_new();
    /* p04.NYKO*/ wire _NYKO_DMA_A04p_new = dma_reg.NYKO_DMA_A04p.qp_new();
    /* p04.PYLO*/ wire _PYLO_DMA_A05p_new = dma_reg.PYLO_DMA_A05p.qp_new();
    /* p04.NUTO*/ wire _NUTO_DMA_A06p_new = dma_reg.NUTO_DMA_A06p.qp_new();
    /* p04.MUGU*/ wire _MUGU_DMA_A07p_new = dma_reg.MUGU_DMA_A07p.qp_new();
    /* p04.NAFA*/ wire _NAFA_DMA_A08p_new = dma_reg.NAFA_DMA_A08n.qn_new();
    /* p04.PYNE*/ wire _PYNE_DMA_A09p_new = dma_reg.PYNE_DMA_A09n.qn_new();
    /* p04.PARA*/ wire _PARA_DMA_A10p_new = dma_reg.PARA_DMA_A10n.qn_new();
    /* p04.NYDO*/ wire _NYDO_DMA_A11p_new = dma_reg.NYDO_DMA_A11n.qn_new();
    /* p04.NYGY*/ wire _NYGY_DMA_A12p_new = dma_reg.NYGY_DMA_A12n.qn_new();

    /* p04.ECAL*/ BUS_VRAM_An[ 0].tri6_nn(_AHOC_DMA_VRAMn_new, _NAKY_DMA_A00p_new);
    /* p04.EGEZ*/ BUS_VRAM_An[ 1].tri6_nn(_AHOC_DMA_VRAMn_new, _PYRO_DMA_A01p_new);
    /* p04.FUHE*/ BUS_VRAM_An[ 2].tri6_nn(_AHOC_DMA_VRAMn_new, _NEFY_DMA_A02p_new);
    /* p04.FYZY*/ BUS_VRAM_An[ 3].tri6_nn(_AHOC_DMA_VRAMn_new, _MUTY_DMA_A03p_new);
    /* p04.DAMU*/ BUS_VRAM_An[ 4].tri6_nn(_AHOC_DMA_VRAMn_new, _NYKO_DMA_A04p_new);
    /* p04.DAVA*/ BUS_VRAM_An[ 5].tri6_nn(_AHOC_DMA_VRAMn_new, _PYLO_DMA_A05p_new);
    /* p04.ETEG*/ BUS_VRAM_An[ 6].tri6_nn(_AHOC_DMA_VRAMn_new, _NUTO_DMA_A06p_new);
    /*#p04.EREW*/ BUS_VRAM_An[ 7].tri6_nn(_AHOC_DMA_VRAMn_new, _MUGU_DMA_A07p_new);
    /*#p04.EVAX*/ BUS_VRAM_An[ 8].tri6_nn(_AHOC_DMA_VRAMn_new, _NAFA_DMA_A08p_new);
    /* p04.DUVE*/ BUS_VRAM_An[ 9].tri6_nn(_AHOC_DMA_VRAMn_new, _PYNE_DMA_A09p_new);
    /* p04.ERAF*/ BUS_VRAM_An[10].tri6_nn(_AHOC_DMA_VRAMn_new, _PARA_DMA_A10p_new);
    /* p04.FUSY*/ BUS_VRAM_An[11].tri6_nn(_AHOC_DMA_VRAMn_new, _NYDO_DMA_A11p_new);
    /* p04.EXYF*/ BUS_VRAM_An[12].tri6_nn(_AHOC_DMA_VRAMn_new, _NYGY_DMA_A12p_new);
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Sprite fetcher read

  [this, &BUS_VRAM_An]() {
    /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn_new = not1(pix_pipe.XYMO_LCDC_SPSIZEn.qn_new());
    /*#p29.WUKY*/ wire _WUKY_FLIP_Yp_new = not1(oam_bus.YZOS_OAM_DB6p.qp_new());
    /*#p29.CYVU*/ wire _CYVU_L0_new = xor2(_WUKY_FLIP_Yp_new, SPR_TRI_L_out_new[0].qp_new());
    /*#p29.BORE*/ wire _BORE_L1_new = xor2(_WUKY_FLIP_Yp_new, SPR_TRI_L_out_new[1].qp_new());
    /*#p29.BUVY*/ wire _BUVY_L2_new = xor2(_WUKY_FLIP_Yp_new, SPR_TRI_L_out_new[2].qp_new());
    /*#p29.WAGO*/ wire _WAGO_L3_new = xor2(_WUKY_FLIP_Yp_new, SPR_TRI_L_out_new[3].qp_new());
    /*#p29.GEJY*/ wire _GEJY_L3_new = amux2(oam_bus.XUSO_OAM_DA0p.qp_new(), _FUFO_LCDC_SPSIZEn_new, pix_pipe.XYMO_LCDC_SPSIZEn.qn_new(), _WAGO_L3_new);

    /*#p29.XUQU*/ wire _XUQU_SPRITE_AB_new = not1(sprite_fetcher.VONU_SFETCH_S1p_D4.qn_new());
    /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY_new = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_new());
    /* p29.TEPA*/ wire _TEPA_RENDERINGn_new = not1(pix_pipe.XYMU_RENDERINGn.qn_new());
    /* p29.TYSO*/ wire _TYSO_SPRITE_READn_new = or2(_SAKY_SFETCH_MYSTERY_new, _TEPA_RENDERINGn_new); // def or
    /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp_new = not1(_TYSO_SPRITE_READn_new);

    /* p29.ABON*/ wire _ABON_SPR_VRM_RDn_new = not1(_TEXY_SPR_READ_VRAMp_new);
    /* p29.ABEM*/ BUS_VRAM_An[ 0].tri6_nn(_ABON_SPR_VRM_RDn_new, _XUQU_SPRITE_AB_new);
    /* p29.BAXE*/ BUS_VRAM_An[ 1].tri6_nn(_ABON_SPR_VRM_RDn_new, _CYVU_L0_new);
    /* p29.ARAS*/ BUS_VRAM_An[ 2].tri6_nn(_ABON_SPR_VRM_RDn_new, _BORE_L1_new);
    /* p29.AGAG*/ BUS_VRAM_An[ 3].tri6_nn(_ABON_SPR_VRM_RDn_new, _BUVY_L2_new);
    /* p29.FAMU*/ BUS_VRAM_An[ 4].tri6_nn(_ABON_SPR_VRM_RDn_new, _GEJY_L3_new);
    /*#p29.FUGY*/ BUS_VRAM_An[ 5].tri6_nn(_ABON_SPR_VRM_RDn_new, oam_bus.XEGU_OAM_DA1p.qp_new());
    /* p29.GAVO*/ BUS_VRAM_An[ 6].tri6_nn(_ABON_SPR_VRM_RDn_new, oam_bus.YJEX_OAM_DA2p.qp_new());
    /* p29.WYGA*/ BUS_VRAM_An[ 7].tri6_nn(_ABON_SPR_VRM_RDn_new, oam_bus.XYJU_OAM_DA3p.qp_new());
    /* p29.WUNE*/ BUS_VRAM_An[ 8].tri6_nn(_ABON_SPR_VRM_RDn_new, oam_bus.YBOG_OAM_DA4p.qp_new());
    /* p29.GOTU*/ BUS_VRAM_An[ 9].tri6_nn(_ABON_SPR_VRM_RDn_new, oam_bus.WYSO_OAM_DA5p.qp_new());
    /* p29.GEGU*/ BUS_VRAM_An[10].tri6_nn(_ABON_SPR_VRM_RDn_new, oam_bus.XOTE_OAM_DA6p.qp_new());
    /* p29.XEHE*/ BUS_VRAM_An[11].tri6_nn(_ABON_SPR_VRM_RDn_new, oam_bus.YZAB_OAM_DA7p.qp_new());
    /* p29.DYSO*/ BUS_VRAM_An[12].tri6_nn(_ABON_SPR_VRM_RDn_new, 0);   // sprites always in low half of tile store
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Background & window map/tile read

  [this, _CUNU_SYS_RSTn_new, _BUWY_FF42_RDn_ext, _CAVO_FF42_WRn_clk, _BEBA_FF43_RDn_ext, _AMUN_FF43_WRn_clk, &BUS_VRAM_An]() {
    /* p27.LUSU*/ wire _LUSU_FETCHINGn_new = not1(tile_fetcher.LONY_FETCHINGp.qp_new());
    /* p27.LENA*/ wire _LENA_FETCHINGp_new = not1(_LUSU_FETCHINGn_new);

    /*#p27.NOCU*/ wire _NOCU_WIN_MODEn_new = not1(pix_pipe.PYNU_WIN_MODE_Ap.qp_new());
    /* p27.PORE*/ wire _PORE_WIN_MODEp_new = not1(_NOCU_WIN_MODEn_new);
    /* p26.AXAD*/ wire _AXAD_WIN_MODEn_new = not1(_PORE_WIN_MODEp_new);

    /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n_new = not1(tile_fetcher.MESU_BFETCH_S1p.qp_new());
    /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n_new = not1(tile_fetcher.NYVA_BFETCH_S2p.qp_new());
    /* p27.NOGU*/ wire _NOGU_BFETCH_01p_new = nand2(_NAKO_BFETCH_S1n_new, _NOFU_BFETCH_S2n_new);
    /* p27.NENY*/ wire _NENY_BFETCH_01n_new = not1(_NOGU_BFETCH_01p_new);

    /* p27.POTU*/ wire _POTU_BGW_MAP_READp_new  =  and2(_LENA_FETCHINGp_new,      _NENY_BFETCH_01n_new);
    /* p26.ACEN*/ wire _ACEN_BGD_MAP_READp_new  =  and2(_POTU_BGW_MAP_READp_new,  _AXAD_WIN_MODEn_new);
    /*#p25.XEZE*/ wire _XEZE_WIN_MAP_READp_new  =  and2(_POTU_BGW_MAP_READp_new,  _PORE_WIN_MODEp_new);

    /* p27.NETA*/ wire _NETA_BGW_TILE_READp_new =  and2(_LENA_FETCHINGp_new,      _NOGU_BFETCH_01p_new);
    /* p26.ASUL*/ wire _ASUL_BGD_TILE_READp_new =  and2(_NETA_BGW_TILE_READp_new, _AXAD_WIN_MODEn_new);
    /* p25.XUCY*/ wire _XUCY_WIN_TILE_READn_new = nand2(_NETA_BGW_TILE_READp_new, _PORE_WIN_MODEp_new);

    /*#p27.XUHA*/ wire _XUHA_FETCH_HILOp_new = not1(_NOFU_BFETCH_S2n_new);

    /* p23.DATY*/ pix_pipe.DATY_SCX0n.dff9(_AMUN_FF43_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p23.DUZU*/ pix_pipe.DUZU_SCX1n.dff9(_AMUN_FF43_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p23.CYXU*/ pix_pipe.CYXU_SCX2n.dff9(_AMUN_FF43_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[2].qp_ext());
    /* p23.GUBO*/ pix_pipe.GUBO_SCX3n.dff9(_AMUN_FF43_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p23.BEMY*/ pix_pipe.BEMY_SCX4n.dff9(_AMUN_FF43_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p23.CUZY*/ pix_pipe.CUZY_SCX5n.dff9(_AMUN_FF43_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p23.CABU*/ pix_pipe.CABU_SCX6n.dff9(_AMUN_FF43_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[6].qp_ext());
    /* p23.BAKE*/ pix_pipe.BAKE_SCX7n.dff9(_AMUN_FF43_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[7].qp_ext());

    /* p23.GAVE*/ pix_pipe.GAVE_SCY0n.dff9(_CAVO_FF42_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[0].qp_ext());
    /* p23.FYMO*/ pix_pipe.FYMO_SCY1n.dff9(_CAVO_FF42_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[1].qp_ext());
    /* p23.FEZU*/ pix_pipe.FEZU_SCY2n.dff9(_CAVO_FF42_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[2].qp_ext());
    /* p23.FUJO*/ pix_pipe.FUJO_SCY3n.dff9(_CAVO_FF42_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[3].qp_ext());
    /* p23.DEDE*/ pix_pipe.DEDE_SCY4n.dff9(_CAVO_FF42_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[4].qp_ext());
    /* p23.FOTY*/ pix_pipe.FOTY_SCY5n.dff9(_CAVO_FF42_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[5].qp_ext());
    /* p23.FOHA*/ pix_pipe.FOHA_SCY6n.dff9(_CAVO_FF42_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[6].qp_ext());
    /* p23.FUNY*/ pix_pipe.FUNY_SCY7n.dff9(_CAVO_FF42_WRn_clk, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_ext[7].qp_ext());

    /* FF42 SCY */
    /*#p23.WARE*/ BUS_CPU_Dp_out[0].tri6_nn(_BUWY_FF42_RDn_ext, pix_pipe.GAVE_SCY0n.qp_new());
    /* p23.GOBA*/ BUS_CPU_Dp_out[1].tri6_nn(_BUWY_FF42_RDn_ext, pix_pipe.FYMO_SCY1n.qp_new());
    /* p23.GONU*/ BUS_CPU_Dp_out[2].tri6_nn(_BUWY_FF42_RDn_ext, pix_pipe.FEZU_SCY2n.qp_new());
    /* p23.GODO*/ BUS_CPU_Dp_out[3].tri6_nn(_BUWY_FF42_RDn_ext, pix_pipe.FUJO_SCY3n.qp_new());
    /* p23.CUSA*/ BUS_CPU_Dp_out[4].tri6_nn(_BUWY_FF42_RDn_ext, pix_pipe.DEDE_SCY4n.qp_new());
    /* p23.GYZO*/ BUS_CPU_Dp_out[5].tri6_nn(_BUWY_FF42_RDn_ext, pix_pipe.FOTY_SCY5n.qp_new());
    /* p23.GUNE*/ BUS_CPU_Dp_out[6].tri6_nn(_BUWY_FF42_RDn_ext, pix_pipe.FOHA_SCY6n.qp_new());
    /* p23.GYZA*/ BUS_CPU_Dp_out[7].tri6_nn(_BUWY_FF42_RDn_ext, pix_pipe.FUNY_SCY7n.qp_new());

    // FF43 SCX
    /*#p23.EDOS*/ BUS_CPU_Dp_out[0].tri6_nn(_BEBA_FF43_RDn_ext, pix_pipe.DATY_SCX0n.qp_new());
    /* p23.EKOB*/ BUS_CPU_Dp_out[1].tri6_nn(_BEBA_FF43_RDn_ext, pix_pipe.DUZU_SCX1n.qp_new());
    /* p23.CUGA*/ BUS_CPU_Dp_out[2].tri6_nn(_BEBA_FF43_RDn_ext, pix_pipe.CYXU_SCX2n.qp_new());
    /* p23.WONY*/ BUS_CPU_Dp_out[3].tri6_nn(_BEBA_FF43_RDn_ext, pix_pipe.GUBO_SCX3n.qp_new());
    /* p23.CEDU*/ BUS_CPU_Dp_out[4].tri6_nn(_BEBA_FF43_RDn_ext, pix_pipe.BEMY_SCX4n.qp_new());
    /* p23.CATA*/ BUS_CPU_Dp_out[5].tri6_nn(_BEBA_FF43_RDn_ext, pix_pipe.CUZY_SCX5n.qp_new());
    /* p23.DOXE*/ BUS_CPU_Dp_out[6].tri6_nn(_BEBA_FF43_RDn_ext, pix_pipe.CABU_SCX6n.qp_new());
    /* p23.CASY*/ BUS_CPU_Dp_out[7].tri6_nn(_BEBA_FF43_RDn_ext, pix_pipe.BAKE_SCX7n.qp_new());

#pragma warning(push)
#pragma warning(disable : 4189)
    /*#p26.ATAD*/ wire _ATAD_TILE_X0S_new = add_s(pix_pipe.XEHO_PX0p.qp_new(), pix_pipe.DATY_SCX0n.qn_new(), 0);
    /*#p26.ATAD*/ wire _ATAD_TILE_X0C_new = add_c(pix_pipe.XEHO_PX0p.qp_new(), pix_pipe.DATY_SCX0n.qn_new(), 0);
    /* p26.BEHU*/ wire _BEHU_TILE_X1S_new = add_s(pix_pipe.SAVY_PX1p.qp_new(), pix_pipe.DUZU_SCX1n.qn_new(), _ATAD_TILE_X0C_new);
    /* p26.BEHU*/ wire _BEHU_TILE_X1C_new = add_c(pix_pipe.SAVY_PX1p.qp_new(), pix_pipe.DUZU_SCX1n.qn_new(), _ATAD_TILE_X0C_new);
    /* p26.APYH*/ wire _APYH_TILE_X2S_new = add_s(pix_pipe.XODU_PX2p.qp_new(), pix_pipe.CYXU_SCX2n.qn_new(), _BEHU_TILE_X1C_new);
    /* p26.APYH*/ wire _APYH_TILE_X2C_new = add_c(pix_pipe.XODU_PX2p.qp_new(), pix_pipe.CYXU_SCX2n.qn_new(), _BEHU_TILE_X1C_new);
    /* p26.BABE*/ wire _BABE_MAP_X0S_new  = add_s(pix_pipe.XYDO_PX3p.qp_new(), pix_pipe.GUBO_SCX3n.qn_new(), _APYH_TILE_X2C_new);
    /* p26.BABE*/ wire _BABE_MAP_X0C_new  = add_c(pix_pipe.XYDO_PX3p.qp_new(), pix_pipe.GUBO_SCX3n.qn_new(), _APYH_TILE_X2C_new);
    /* p26.ABOD*/ wire _ABOD_MAP_X1S_new  = add_s(pix_pipe.TUHU_PX4p.qp_new(), pix_pipe.BEMY_SCX4n.qn_new(), _BABE_MAP_X0C_new);
    /* p26.ABOD*/ wire _ABOD_MAP_X1C_new  = add_c(pix_pipe.TUHU_PX4p.qp_new(), pix_pipe.BEMY_SCX4n.qn_new(), _BABE_MAP_X0C_new);
    /* p26.BEWY*/ wire _BEWY_MAP_X2S_new  = add_s(pix_pipe.TUKY_PX5p.qp_new(), pix_pipe.CUZY_SCX5n.qn_new(), _ABOD_MAP_X1C_new);
    /* p26.BEWY*/ wire _BEWY_MAP_X2C_new  = add_c(pix_pipe.TUKY_PX5p.qp_new(), pix_pipe.CUZY_SCX5n.qn_new(), _ABOD_MAP_X1C_new);
    /* p26.BYCA*/ wire _BYCA_MAP_X3S_new  = add_s(pix_pipe.TAKO_PX6p.qp_new(), pix_pipe.CABU_SCX6n.qn_new(), _BEWY_MAP_X2C_new);
    /* p26.BYCA*/ wire _BYCA_MAP_X3C_new  = add_c(pix_pipe.TAKO_PX6p.qp_new(), pix_pipe.CABU_SCX6n.qn_new(), _BEWY_MAP_X2C_new);
    /* p26.ACUL*/ wire _ACUL_MAP_X4S_new  = add_s(pix_pipe.SYBE_PX7p.qp_new(), pix_pipe.BAKE_SCX7n.qn_new(), _BYCA_MAP_X3C_new);
    /* p26.ACUL*/ wire _ACUL_MAP_X4C_new  = add_c(pix_pipe.SYBE_PX7p.qp_new(), pix_pipe.BAKE_SCX7n.qn_new(), _BYCA_MAP_X3C_new);

    /*#p26.FAFO*/ wire _FAFO_TILE_Y0S_new = add_s(lcd_reg.MUWY_LY0p.qp_new(), pix_pipe.GAVE_SCY0n.qn_new(), 0);
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0C_new = add_c(lcd_reg.MUWY_LY0p.qp_new(), pix_pipe.GAVE_SCY0n.qn_new(), 0);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1S_new = add_s(lcd_reg.MYRO_LY1p.qp_new(), pix_pipe.FYMO_SCY1n.qn_new(), _FAFO_TILE_Y0C_new);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1C_new = add_c(lcd_reg.MYRO_LY1p.qp_new(), pix_pipe.FYMO_SCY1n.qn_new(), _FAFO_TILE_Y0C_new);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2S_new = add_s(lcd_reg.LEXA_LY2p.qp_new(), pix_pipe.FEZU_SCY2n.qn_new(), _EMUX_TILE_Y1C_new);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2C_new = add_c(lcd_reg.LEXA_LY2p.qp_new(), pix_pipe.FEZU_SCY2n.qn_new(), _EMUX_TILE_Y1C_new);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0S_new  = add_s(lcd_reg.LYDO_LY3p.qp_new(), pix_pipe.FUJO_SCY3n.qn_new(), _ECAB_TILE_Y2C_new);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0C_new  = add_c(lcd_reg.LYDO_LY3p.qp_new(), pix_pipe.FUJO_SCY3n.qn_new(), _ECAB_TILE_Y2C_new);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1S_new  = add_s(lcd_reg.LOVU_LY4p.qp_new(), pix_pipe.DEDE_SCY4n.qn_new(), _ETAM_MAP_Y0C_new);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1C_new  = add_c(lcd_reg.LOVU_LY4p.qp_new(), pix_pipe.DEDE_SCY4n.qn_new(), _ETAM_MAP_Y0C_new);
    /* p26.DABA*/ wire _DABA_MAP_Y2S_new  = add_s(lcd_reg.LEMA_LY5p.qp_new(), pix_pipe.FOTY_SCY5n.qn_new(), _DOTO_MAP_Y1C_new);
    /* p26.DABA*/ wire _DABA_MAP_Y2C_new  = add_c(lcd_reg.LEMA_LY5p.qp_new(), pix_pipe.FOTY_SCY5n.qn_new(), _DOTO_MAP_Y1C_new);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3S_new  = add_s(lcd_reg.MATO_LY6p.qp_new(), pix_pipe.FOHA_SCY6n.qn_new(), _DABA_MAP_Y2C_new);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3C_new  = add_c(lcd_reg.MATO_LY6p.qp_new(), pix_pipe.FOHA_SCY6n.qn_new(), _DABA_MAP_Y2C_new);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4S_new  = add_s(lcd_reg.LAFO_LY7p.qp_new(), pix_pipe.FUNY_SCY7n.qn_new(), _EFYK_MAP_Y3C_new);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4C_new  = add_c(lcd_reg.LAFO_LY7p.qp_new(), pix_pipe.FUNY_SCY7n.qn_new(), _EFYK_MAP_Y3C_new);
#pragma warning(pop)

    /* p26.BAFY*/ wire _BAFY_BG_MAP_READn_new = not1(_ACEN_BGD_MAP_READp_new);
    /* p26.AXEP*/ BUS_VRAM_An[ 0].tri6_nn(_BAFY_BG_MAP_READn_new, _BABE_MAP_X0S_new);
    /* p26.AFEB*/ BUS_VRAM_An[ 1].tri6_nn(_BAFY_BG_MAP_READn_new, _ABOD_MAP_X1S_new);
    /* p26.ALEL*/ BUS_VRAM_An[ 2].tri6_nn(_BAFY_BG_MAP_READn_new, _BEWY_MAP_X2S_new);
    /* p26.COLY*/ BUS_VRAM_An[ 3].tri6_nn(_BAFY_BG_MAP_READn_new, _BYCA_MAP_X3S_new);
    /* p26.AJAN*/ BUS_VRAM_An[ 4].tri6_nn(_BAFY_BG_MAP_READn_new, _ACUL_MAP_X4S_new);
    /* p26.DUHO*/ BUS_VRAM_An[ 5].tri6_nn(_BAFY_BG_MAP_READn_new, _ETAM_MAP_Y0S_new);
    /* p26.CASE*/ BUS_VRAM_An[ 6].tri6_nn(_BAFY_BG_MAP_READn_new, _DOTO_MAP_Y1S_new);
    /* p26.CYPO*/ BUS_VRAM_An[ 7].tri6_nn(_BAFY_BG_MAP_READn_new, _DABA_MAP_Y2S_new);
    /* p26.CETA*/ BUS_VRAM_An[ 8].tri6_nn(_BAFY_BG_MAP_READn_new, _EFYK_MAP_Y3S_new);
    /* p26.DAFE*/ BUS_VRAM_An[ 9].tri6_nn(_BAFY_BG_MAP_READn_new, _EJOK_MAP_Y4S_new);
    /*#p26.AMUV*/ BUS_VRAM_An[10].tri6_nn(_BAFY_BG_MAP_READn_new, pix_pipe.XAFO_LCDC_BGMAPn.qn_new());
    /* p26.COVE*/ BUS_VRAM_An[11].tri6_nn(_BAFY_BG_MAP_READn_new, 1);
    /* p26.COXO*/ BUS_VRAM_An[12].tri6_nn(_BAFY_BG_MAP_READn_new, 1);

    /*#p25.WUKO*/ wire _WUKO_WIN_MAP_READn_new = not1(_XEZE_WIN_MAP_READp_new);
    /*#p27.XEJA*/ BUS_VRAM_An[ 0].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.WYKA_WIN_X3.qp_new());
    /* p27.XAMO*/ BUS_VRAM_An[ 1].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.WODY_WIN_X4.qp_new());
    /* p27.XAHE*/ BUS_VRAM_An[ 2].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.WOBO_WIN_X5.qp_new());
    /* p27.XULO*/ BUS_VRAM_An[ 3].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.WYKO_WIN_X6.qp_new());
    /* p27.WUJU*/ BUS_VRAM_An[ 4].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.XOLO_WIN_X7.qp_new());
    /*#p27.VYTO*/ BUS_VRAM_An[ 5].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.TUFU_WIN_Y3.qp_new());
    /* p27.VEHA*/ BUS_VRAM_An[ 6].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.TAXA_WIN_Y4.qp_new());
    /* p27.VACE*/ BUS_VRAM_An[ 7].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.TOZO_WIN_Y5.qp_new());
    /* p27.VOVO*/ BUS_VRAM_An[ 8].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.TATE_WIN_Y6.qp_new());
    /* p27.VULO*/ BUS_VRAM_An[ 9].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.TEKE_WIN_Y7.qp_new());
    /*#p27.VEVY*/ BUS_VRAM_An[10].tri6_nn(_WUKO_WIN_MAP_READn_new, pix_pipe.WOKY_LCDC_WINMAPn.qn_new());
    /* p27.VEZA*/ BUS_VRAM_An[11].tri6_nn(_WUKO_WIN_MAP_READn_new, 1);
    /* p27.VOGU*/ BUS_VRAM_An[12].tri6_nn(_WUKO_WIN_MAP_READn_new, 1);

    /* p26.BEJE*/ wire _BEJE_BG_TILE_READn_new = not1(_ASUL_BGD_TILE_READp_new);
    /*#p26.ASUM*/ BUS_VRAM_An[ 0].tri6_nn(_BEJE_BG_TILE_READn_new, _XUHA_FETCH_HILOp_new);
    /* p26.EVAD*/ BUS_VRAM_An[ 1].tri6_nn(_BEJE_BG_TILE_READn_new, _FAFO_TILE_Y0S_new);
    /* p26.DAHU*/ BUS_VRAM_An[ 2].tri6_nn(_BEJE_BG_TILE_READn_new, _EMUX_TILE_Y1S_new);
    /* p26.DODE*/ BUS_VRAM_An[ 3].tri6_nn(_BEJE_BG_TILE_READn_new, _ECAB_TILE_Y2S_new);

    /*#p25.VUZA*/ wire _VUZA_TILE_BANKp_new = nor2(vram_bus.PYJU_TILE_DB7p.qp_new(), pix_pipe.WEXU_LCDC_BGTILEn.qn_new());
    /*#p25.XONU*/ BUS_VRAM_An[ 0].tri6_nn(_XUCY_WIN_TILE_READn_new, _XUHA_FETCH_HILOp_new);
    /*#p25.WUDO*/ BUS_VRAM_An[ 1].tri6_nn(_XUCY_WIN_TILE_READn_new, pix_pipe.VYNO_WIN_Y0.qp_new());
    /*#p25.WAWE*/ BUS_VRAM_An[ 2].tri6_nn(_XUCY_WIN_TILE_READn_new, pix_pipe.VUJO_WIN_Y1.qp_new());
    /*#p25.WOLU*/ BUS_VRAM_An[ 3].tri6_nn(_XUCY_WIN_TILE_READn_new, pix_pipe.VYMU_WIN_Y2.qp_new());
    /*#p25.VAPY*/ BUS_VRAM_An[ 4].tri6_pn(_NETA_BGW_TILE_READp_new, vram_bus.RAWU_TILE_DB0p.qp_new());
    /*#p25.SEZU*/ BUS_VRAM_An[ 5].tri6_pn(_NETA_BGW_TILE_READp_new, vram_bus.POZO_TILE_DB1p.qp_new());
    /*#p25.VEJY*/ BUS_VRAM_An[ 6].tri6_pn(_NETA_BGW_TILE_READp_new, vram_bus.PYZO_TILE_DB2p.qp_new());
    /*#p25.RUSA*/ BUS_VRAM_An[ 7].tri6_pn(_NETA_BGW_TILE_READp_new, vram_bus.POXA_TILE_DB3p.qp_new());
    /*#p25.ROHA*/ BUS_VRAM_An[ 8].tri6_pn(_NETA_BGW_TILE_READp_new, vram_bus.PULO_TILE_DB4p.qp_new());
    /*#p25.RESO*/ BUS_VRAM_An[ 9].tri6_pn(_NETA_BGW_TILE_READp_new, vram_bus.POJU_TILE_DB5p.qp_new());
    /*#p25.SUVO*/ BUS_VRAM_An[10].tri6_pn(_NETA_BGW_TILE_READp_new, vram_bus.POWY_TILE_DB6p.qp_new());
    /*#p25.TOBO*/ BUS_VRAM_An[11].tri6_pn(_NETA_BGW_TILE_READp_new, vram_bus.PYJU_TILE_DB7p.qp_new());
    /*#p25.VURY*/ BUS_VRAM_An[12].tri6_pn(_NETA_BGW_TILE_READp_new, _VUZA_TILE_BANKp_new);
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // VRAM addr bus -> VRAM addr pin

  [this, BUS_VRAM_An]() {
    /* p25.MYFU*/ wire _MYFUp_new = not1(BUS_VRAM_An[ 0].qp_ext());
    /* p25.MASA*/ wire _MASAp_new = not1(BUS_VRAM_An[ 1].qp_ext());
    /* p25.MYRE*/ wire _MYREp_new = not1(BUS_VRAM_An[ 2].qp_ext());
    /* p25.MAVU*/ wire _MAVUp_new = not1(BUS_VRAM_An[ 3].qp_ext());
    /* p25.MEPA*/ wire _MEPAp_new = not1(BUS_VRAM_An[ 4].qp_ext());
    /* p25.MYSA*/ wire _MYSAp_new = not1(BUS_VRAM_An[ 5].qp_ext());
    /* p25.MEWY*/ wire _MEWYp_new = not1(BUS_VRAM_An[ 6].qp_ext());
    /* p25.MUME*/ wire _MUMEp_new = not1(BUS_VRAM_An[ 7].qp_ext());
    /* p25.VOVA*/ wire _VOVAp_new = not1(BUS_VRAM_An[ 8].qp_ext());
    /* p25.VODE*/ wire _VODEp_new = not1(BUS_VRAM_An[ 9].qp_ext());
    /* p25.RUKY*/ wire _RUKYp_new = not1(BUS_VRAM_An[10].qp_ext());
    /* p25.RUMA*/ wire _RUMAp_new = not1(BUS_VRAM_An[11].qp_ext());
    /* p25.REHO*/ wire _REHOp_new = not1(BUS_VRAM_An[12].qp_ext());

    /* p25.LEXE*/ wire _LEXEn_new = not1(_MYFUp_new);
    /* p25.LOZU*/ wire _LOZUn_new = not1(_MASAp_new);
    /* p25.LACA*/ wire _LACAn_new = not1(_MYREp_new);
    /* p25.LUVO*/ wire _LUVOn_new = not1(_MAVUp_new);
    /* p25.LOLY*/ wire _LOLYn_new = not1(_MEPAp_new);
    /* p25.LALO*/ wire _LALOn_new = not1(_MYSAp_new);
    /* p25.LEFA*/ wire _LEFAn_new = not1(_MEWYp_new);
    /* p25.LUBY*/ wire _LUBYn_new = not1(_MUMEp_new);
    /* p25.TUJY*/ wire _TUJYn_new = not1(_VOVAp_new);
    /* p25.TAGO*/ wire _TAGOn_new = not1(_VODEp_new);
    /* p25.NUVA*/ wire _NUVAn_new = not1(_RUKYp_new);
    /* p25.PEDU*/ wire _PEDUn_new = not1(_RUMAp_new);
    /* p25.PONY*/ wire _PONYn_new = not1(_REHOp_new);

    Pin2 PIN_VRAM_Ap[13]; // not stable

    PIN_VRAM_Ap[ 0].reset();
    PIN_VRAM_Ap[ 1].reset();
    PIN_VRAM_Ap[ 2].reset();
    PIN_VRAM_Ap[ 3].reset();
    PIN_VRAM_Ap[ 4].reset();
    PIN_VRAM_Ap[ 5].reset();
    PIN_VRAM_Ap[ 6].reset();
    PIN_VRAM_Ap[ 7].reset();
    PIN_VRAM_Ap[ 8].reset();
    PIN_VRAM_Ap[ 9].reset();
    PIN_VRAM_Ap[10].reset();
    PIN_VRAM_Ap[11].reset();
    PIN_VRAM_Ap[12].reset();

    PIN_VRAM_Ap[ 0].pin_out(1, _LEXEn_new, _LEXEn_new);
    PIN_VRAM_Ap[ 1].pin_out(1, _LOZUn_new, _LOZUn_new);
    PIN_VRAM_Ap[ 2].pin_out(1, _LACAn_new, _LACAn_new);
    PIN_VRAM_Ap[ 3].pin_out(1, _LUVOn_new, _LUVOn_new);
    PIN_VRAM_Ap[ 4].pin_out(1, _LOLYn_new, _LOLYn_new);
    PIN_VRAM_Ap[ 5].pin_out(1, _LALOn_new, _LALOn_new);
    PIN_VRAM_Ap[ 6].pin_out(1, _LEFAn_new, _LEFAn_new);
    PIN_VRAM_Ap[ 7].pin_out(1, _LUBYn_new, _LUBYn_new);
    PIN_VRAM_Ap[ 8].pin_out(1, _TUJYn_new, _TUJYn_new);
    PIN_VRAM_Ap[ 9].pin_out(1, _TAGOn_new, _TAGOn_new);
    PIN_VRAM_Ap[10].pin_out(1, _NUVAn_new, _NUVAn_new);
    PIN_VRAM_Ap[11].pin_out(1, _PEDUn_new, _PEDUn_new);
    PIN_VRAM_Ap[12].pin_out(1, _PONYn_new, _PONYn_new);

    if (DELTA_AB || DELTA_CD || DELTA_EF || DELTA_GH) {
      vram_addr_latch = pack_u16p_new(13, PIN_VRAM_Ap);
    }
  }();

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  if (!PIN_VRAM_WRn.qp_ext()) {
    vid_ram[vram_addr_latch] = pack_u8p_new(8, PIN_VRAM_Dp_out);
  }

  if (DELTA_GH) {
    cpu_data_latch = pack_u8p_new(8, &BUS_CPU_Dp_out[0]);
  }

  {

    PIN_VRAM_Dp_in[0].reset();
    PIN_VRAM_Dp_in[1].reset();
    PIN_VRAM_Dp_in[2].reset();
    PIN_VRAM_Dp_in[3].reset();
    PIN_VRAM_Dp_in[4].reset();
    PIN_VRAM_Dp_in[5].reset();
    PIN_VRAM_Dp_in[6].reset();
    PIN_VRAM_Dp_in[7].reset();

    PIN_VRAM_Dp_in[0].pin_in(!PIN_VRAM_OEn.qp_new(), (vram_data_latch & 0x01));
    PIN_VRAM_Dp_in[1].pin_in(!PIN_VRAM_OEn.qp_new(), (vram_data_latch & 0x02));
    PIN_VRAM_Dp_in[2].pin_in(!PIN_VRAM_OEn.qp_new(), (vram_data_latch & 0x04));
    PIN_VRAM_Dp_in[3].pin_in(!PIN_VRAM_OEn.qp_new(), (vram_data_latch & 0x08));
    PIN_VRAM_Dp_in[4].pin_in(!PIN_VRAM_OEn.qp_new(), (vram_data_latch & 0x10));
    PIN_VRAM_Dp_in[5].pin_in(!PIN_VRAM_OEn.qp_new(), (vram_data_latch & 0x20));
    PIN_VRAM_Dp_in[6].pin_in(!PIN_VRAM_OEn.qp_new(), (vram_data_latch & 0x40));
    PIN_VRAM_Dp_in[7].pin_in(!PIN_VRAM_OEn.qp_new(), (vram_data_latch & 0x80));

    BUS_VRAM_Dp_in[0].reset();
    BUS_VRAM_Dp_in[1].reset();
    BUS_VRAM_Dp_in[2].reset();
    BUS_VRAM_Dp_in[3].reset();
    BUS_VRAM_Dp_in[4].reset();
    BUS_VRAM_Dp_in[5].reset();
    BUS_VRAM_Dp_in[6].reset();
    BUS_VRAM_Dp_in[7].reset();

    /*#p25.ROPY*/ wire _ROPY_RENDERINGn_new   = not1(pix_pipe.XYMU_RENDERINGn.qn_new());
    /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp_new  = and2(_TOLE_CPU_VRAM_RDp_ext, _ROPY_RENDERINGn_new);
    /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp_new = not1(_SALE_CPU_VRAM_WRn_ext);
    /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp_new  = and2(_SERE_CPU_VRM_RDp_new, _RUVY_CPU_VRAM_WRp_new);
    /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn_new  = not1(_SAZO_CBD_TO_VPDp_new);
    /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp_new  = not1(_RYJE_CBD_TO_VPDn_new);
    /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp_new  = or2(_REVO_CBD_TO_VPDp_new, _SAZO_CBD_TO_VPDp_new);
    /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn_new  = not1(_RELA_CBD_TO_VPDp_new);

    /* p25.RODY*/ BUS_VRAM_Dp_in[0].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp_in[0].qn_new());
    /* p25.REBA*/ BUS_VRAM_Dp_in[1].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp_in[1].qn_new());
    /* p25.RYDO*/ BUS_VRAM_Dp_in[2].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp_in[2].qn_new());
    /* p25.REMO*/ BUS_VRAM_Dp_in[3].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp_in[3].qn_new());
    /* p25.ROCE*/ BUS_VRAM_Dp_in[4].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp_in[4].qn_new());
    /* p25.ROPU*/ BUS_VRAM_Dp_in[5].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp_in[5].qn_new());
    /* p25.RETA*/ BUS_VRAM_Dp_in[6].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp_in[6].qn_new());
    /* p25.RAKU*/ BUS_VRAM_Dp_in[7].tri6_pn(_RENA_CBD_TO_VPDn_new, PIN_VRAM_Dp_in[7].qn_new());
  }

#pragma region OAM_write
  {
    BUS_OAM_An[0].reset();
    BUS_OAM_An[1].reset();
    BUS_OAM_An[2].reset();
    BUS_OAM_An[3].reset();
    BUS_OAM_An[4].reset();
    BUS_OAM_An[5].reset();
    BUS_OAM_An[6].reset();
    BUS_OAM_An[7].reset();

    /*#p28.ACYL*/ wire _ACYL_SCANNINGp_new   = and2(_BOGE_DMA_RUNNINGn_new, sprite_scanner.BESU_SCANNINGp.qp_new());
    /* p28.AJON*/ wire _AJON_PPU_OAM_ENp_new = and2(_BOGE_DMA_RUNNINGn_new, pix_pipe.XYMU_RENDERINGn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
    /* p04.DUGA*/ wire _DUGA_DMA_OAM_RDn_new = not1(dma_reg.MATU_DMA_RUNNINGp.qp_new());
    /* p28.ASAM*/ wire _ASAM_CPU_OAM_RDn_new = or3(_ACYL_SCANNINGp_new, pix_pipe.XYMU_RENDERINGn.qn_new(), dma_reg.MATU_DMA_RUNNINGp.qp_new());
    /* p28.BETE*/ wire _BETE_PPU_OAM_RDn_new = not1(_AJON_PPU_OAM_ENp_new);

    /* SCAN  -> OBA */
    /* p28.APAR*/ wire _APAR_SCANNINGn_new = not1(_ACYL_SCANNINGp_new);
    /* p28.GEFY*/ BUS_OAM_An[0].tri6_nn(_APAR_SCANNINGn_new, GND);
    /* p28.WUWE*/ BUS_OAM_An[1].tri6_nn(_APAR_SCANNINGn_new, GND);
    /* p28.GUSE*/ BUS_OAM_An[2].tri6_nn(_APAR_SCANNINGn_new, sprite_scanner.YFEL_SCAN0.qp_new());
    /* p28.GEMA*/ BUS_OAM_An[3].tri6_nn(_APAR_SCANNINGn_new, sprite_scanner.WEWY_SCAN1.qp_new());
    /* p28.FUTO*/ BUS_OAM_An[4].tri6_nn(_APAR_SCANNINGn_new, sprite_scanner.GOSO_SCAN2.qp_new());
    /* p28.FAKU*/ BUS_OAM_An[5].tri6_nn(_APAR_SCANNINGn_new, sprite_scanner.ELYN_SCAN3.qp_new());
    /* p28.GAMA*/ BUS_OAM_An[6].tri6_nn(_APAR_SCANNINGn_new, sprite_scanner.FAHA_SCAN4.qp_new());
    /* p28.GOBY*/ BUS_OAM_An[7].tri6_nn(_APAR_SCANNINGn_new, sprite_scanner.FONY_SCAN5.qp_new());

    /* DBA   -> OBA */
    /* p28.FODO*/ BUS_OAM_An[0].tri6_nn(_DUGA_DMA_OAM_RDn_new, dma_reg.NAKY_DMA_A00p.qp_new());
    /* p28.FESA*/ BUS_OAM_An[1].tri6_nn(_DUGA_DMA_OAM_RDn_new, dma_reg.PYRO_DMA_A01p.qp_new());
    /* p28.FAGO*/ BUS_OAM_An[2].tri6_nn(_DUGA_DMA_OAM_RDn_new, dma_reg.NEFY_DMA_A02p.qp_new());
    /* p28.FYKY*/ BUS_OAM_An[3].tri6_nn(_DUGA_DMA_OAM_RDn_new, dma_reg.MUTY_DMA_A03p.qp_new());
    /* p28.ELUG*/ BUS_OAM_An[4].tri6_nn(_DUGA_DMA_OAM_RDn_new, dma_reg.NYKO_DMA_A04p.qp_new());
    /* p28.EDOL*/ BUS_OAM_An[5].tri6_nn(_DUGA_DMA_OAM_RDn_new, dma_reg.PYLO_DMA_A05p.qp_new());
    /* p28.FYDU*/ BUS_OAM_An[6].tri6_nn(_DUGA_DMA_OAM_RDn_new, dma_reg.NUTO_DMA_A06p.qp_new());
    /* p28.FETU*/ BUS_OAM_An[7].tri6_nn(_DUGA_DMA_OAM_RDn_new, dma_reg.MUGU_DMA_A07p.qp_new());

    /* CBA   -> OBA */
    /* p28.GARO*/ BUS_OAM_An[0].tri6_nn(_ASAM_CPU_OAM_RDn_new, BUS_CPU_A_ext[ 0]);
    /* p28.WACU*/ BUS_OAM_An[1].tri6_nn(_ASAM_CPU_OAM_RDn_new, BUS_CPU_A_ext[ 1]);
    /* p28.GOSE*/ BUS_OAM_An[2].tri6_nn(_ASAM_CPU_OAM_RDn_new, BUS_CPU_A_ext[ 2]);
    /* p28.WAPE*/ BUS_OAM_An[3].tri6_nn(_ASAM_CPU_OAM_RDn_new, BUS_CPU_A_ext[ 3]);
    /* p28.FEVU*/ BUS_OAM_An[4].tri6_nn(_ASAM_CPU_OAM_RDn_new, BUS_CPU_A_ext[ 4]);
    /* p28.GERA*/ BUS_OAM_An[5].tri6_nn(_ASAM_CPU_OAM_RDn_new, BUS_CPU_A_ext[ 5]);
    /* p28.WAXA*/ BUS_OAM_An[6].tri6_nn(_ASAM_CPU_OAM_RDn_new, BUS_CPU_A_ext[ 6]);
    /* p28.FOBY*/ BUS_OAM_An[7].tri6_nn(_ASAM_CPU_OAM_RDn_new, BUS_CPU_A_ext[ 7]);

    /* SPR_I -> OBA */
    wire _WEFE_VCC = 1;
    /* p28.GECA*/ BUS_OAM_An[0].tri6_nn(_BETE_PPU_OAM_RDn_new, _WEFE_VCC);
    /* p28.WYDU*/ BUS_OAM_An[1].tri6_nn(_BETE_PPU_OAM_RDn_new, _WEFE_VCC);
    /* p28.GYBU*/ BUS_OAM_An[2].tri6_nn(_BETE_PPU_OAM_RDn_new, SPR_TRI_I_out_new[0].qp_ext());
    /* p28.GYKA*/ BUS_OAM_An[3].tri6_nn(_BETE_PPU_OAM_RDn_new, SPR_TRI_I_out_new[1].qp_ext());
    /* p28.FABY*/ BUS_OAM_An[4].tri6_nn(_BETE_PPU_OAM_RDn_new, SPR_TRI_I_out_new[2].qp_ext());
    /* p28.FACO*/ BUS_OAM_An[5].tri6_nn(_BETE_PPU_OAM_RDn_new, SPR_TRI_I_out_new[3].qp_ext());
    /* p28.FUGU*/ BUS_OAM_An[6].tri6_nn(_BETE_PPU_OAM_RDn_new, SPR_TRI_I_out_new[4].qp_ext());
    /* p28.FYKE*/ BUS_OAM_An[7].tri6_nn(_BETE_PPU_OAM_RDn_new, SPR_TRI_I_out_new[5].qp_ext());
  }

  oam_addr_latch = pack_u8n_new(7, &BUS_OAM_An[1]);


  {
    BUS_OAM_DAn_out[0].reset();
    BUS_OAM_DAn_out[1].reset();
    BUS_OAM_DAn_out[2].reset();
    BUS_OAM_DAn_out[3].reset();
    BUS_OAM_DAn_out[4].reset();
    BUS_OAM_DAn_out[5].reset();
    BUS_OAM_DAn_out[6].reset();
    BUS_OAM_DAn_out[7].reset();

    BUS_OAM_DBn_out[0].reset();
    BUS_OAM_DBn_out[1].reset();
    BUS_OAM_DBn_out[2].reset();
    BUS_OAM_DBn_out[3].reset();
    BUS_OAM_DBn_out[4].reset();
    BUS_OAM_DBn_out[5].reset();
    BUS_OAM_DBn_out[6].reset();
    BUS_OAM_DBn_out[7].reset();

    /* DMA EBD -> OBD */
    {
      /* p25.RALO*/ wire _RALO_EXT_D0p_new = not1(PIN_EXT_D_in_new[0].qn_new());
      /* p25.TUNE*/ wire _TUNE_EXT_D1p_new = not1(PIN_EXT_D_in_new[1].qn_new());
      /* p25.SERA*/ wire _SERA_EXT_D2p_new = not1(PIN_EXT_D_in_new[2].qn_new());
      /* p25.TENU*/ wire _TENU_EXT_D3p_new = not1(PIN_EXT_D_in_new[3].qn_new());
      /* p25.SYSA*/ wire _SYSA_EXT_D4p_new = not1(PIN_EXT_D_in_new[4].qn_new());
      /* p25.SUGY*/ wire _SUGY_EXT_D5p_new = not1(PIN_EXT_D_in_new[5].qn_new());
      /* p25.TUBE*/ wire _TUBE_EXT_D6p_new = not1(PIN_EXT_D_in_new[6].qn_new());
      /* p25.SYZO*/ wire _SYZO_EXT_D7p_new = not1(PIN_EXT_D_in_new[7].qn_new());

      /* p25.CEDE*/ wire _CEDE_EBD_TO_OBDn_new = not1(_LUMA_DMA_CARTp_new);
      /* p25.WASA*/ BUS_OAM_DAn_out[0].tri6_nn(_CEDE_EBD_TO_OBDn_new, _RALO_EXT_D0p_new);
      /* p25.BOMO*/ BUS_OAM_DAn_out[1].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TUNE_EXT_D1p_new);
      /* p25.BASA*/ BUS_OAM_DAn_out[2].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SERA_EXT_D2p_new);
      /* p25.CAKO*/ BUS_OAM_DAn_out[3].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TENU_EXT_D3p_new);
      /* p25.BUMA*/ BUS_OAM_DAn_out[4].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SYSA_EXT_D4p_new);
      /* p25.BUPY*/ BUS_OAM_DAn_out[5].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SUGY_EXT_D5p_new);
      /* p25.BASY*/ BUS_OAM_DAn_out[6].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TUBE_EXT_D6p_new);
      /* p25.BAPE*/ BUS_OAM_DAn_out[7].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SYZO_EXT_D7p_new);

      /* p25.WEJO*/ BUS_OAM_DBn_out[0].tri6_nn(_CEDE_EBD_TO_OBDn_new, _RALO_EXT_D0p_new);
      /* p25.BUBO*/ BUS_OAM_DBn_out[1].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TUNE_EXT_D1p_new);
      /* p25.BETU*/ BUS_OAM_DBn_out[2].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SERA_EXT_D2p_new);
      /* p25.CYME*/ BUS_OAM_DBn_out[3].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TENU_EXT_D3p_new);
      /* p25.BAXU*/ BUS_OAM_DBn_out[4].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SYSA_EXT_D4p_new);
      /* p25.BUHU*/ BUS_OAM_DBn_out[5].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SUGY_EXT_D5p_new);
      /* p25.BYNY*/ BUS_OAM_DBn_out[6].tri6_nn(_CEDE_EBD_TO_OBDn_new, _TUBE_EXT_D6p_new);
      /* p25.BYPY*/ BUS_OAM_DBn_out[7].tri6_nn(_CEDE_EBD_TO_OBDn_new, _SYZO_EXT_D7p_new);
    }

    /* DMA VBD -> OBD */
    {
      /* p28.AZAR*/ wire _AZAR_VBD_TO_OBDn_new = not1(_LUFA_DMA_VRAMp_new);
      /* p28.WUZU*/ BUS_OAM_DAn_out[0].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[0].qp_ext());
      /* p28.AXER*/ BUS_OAM_DAn_out[1].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[1].qp_ext());
      /* p28.ASOX*/ BUS_OAM_DAn_out[2].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[2].qp_ext());
      /* p28.CETU*/ BUS_OAM_DAn_out[3].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[3].qp_ext());
      /* p28.ARYN*/ BUS_OAM_DAn_out[4].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[4].qp_ext());
      /* p28.ACOT*/ BUS_OAM_DAn_out[5].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[5].qp_ext());
      /* p28.CUJE*/ BUS_OAM_DAn_out[6].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[6].qp_ext());
      /* p28.ATER*/ BUS_OAM_DAn_out[7].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[7].qp_ext());

      /* p28.WOWA*/ BUS_OAM_DBn_out[0].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[0].qp_ext());
      /* p28.AVEB*/ BUS_OAM_DBn_out[1].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[1].qp_ext());
      /* p28.AMUH*/ BUS_OAM_DBn_out[2].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[2].qp_ext());
      /* p28.COFO*/ BUS_OAM_DBn_out[3].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[3].qp_ext());
      /* p28.AZOZ*/ BUS_OAM_DBn_out[4].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[4].qp_ext());
      /* p28.AGYK*/ BUS_OAM_DBn_out[5].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[5].qp_ext());
      /* p28.BUSE*/ BUS_OAM_DBn_out[6].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[6].qp_ext());
      /* p28.ANUM*/ BUS_OAM_DBn_out[7].tri6_nn(_AZAR_VBD_TO_OBDn_new, BUS_VRAM_Dp_in[7].qp_ext());
    }

    {
      /* p28.XUTO*/ wire _XUTO_CPU_OAM_WRp_ext = and2(_SARO_FE00_FEFFp_ext, _CUPA_CPU_WRp_xxxxEFGx_clk);
      /* p28.XYNY*/ wire _XYNY_ABCDxxxx_clk = not1(_MOPA_xxxxEFGH_clk);
      /* p28.WUJE*/ oam_bus.WUJE_CPU_OAM_WRn.nor_latch(_XYNY_ABCDxxxx_clk, _XUTO_CPU_OAM_WRp_ext);
      /* p28.XUPA*/ wire _XUPA_CPU_OAM_WRp_new = not1(oam_bus.WUJE_CPU_OAM_WRn.qp_new());
      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_new   = and2(_BOGE_DMA_RUNNINGn_new, sprite_scanner.BESU_SCANNINGp.qp_new());
      /* p28.AJON*/ wire _AJON_PPU_OAM_ENp_new = and2(_BOGE_DMA_RUNNINGn_new, pix_pipe.XYMU_RENDERINGn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
      /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn_new   = nor3(dma_reg.MATU_DMA_RUNNINGp.qp_new(), _ACYL_SCANNINGp_new, _AJON_PPU_OAM_ENp_new); // def nor
      /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp_new = and2(_SARO_FE00_FEFFp_ext, _AJUJ_OAM_BUSYn_new); // def and
      /* p28.APAG*/ wire _APAG_CBD_TO_OBDp_new = amux2(_XUPA_CPU_OAM_WRp_new, _AMAB_CPU_OAM_ENp_new, _AJUJ_OAM_BUSYn_new, _ADAH_FE00_FEFFn_ext);
      /* p28.AZUL*/ wire _AZUL_CBD_TO_OBDn_new = not1(_APAG_CBD_TO_OBDp_new);
      /* p28.ZAXA*/ BUS_OAM_DAn_out[0].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[0].qp_ext());
      /* p28.ZAKY*/ BUS_OAM_DAn_out[1].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[1].qp_ext());
      /* p28.WULE*/ BUS_OAM_DAn_out[2].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[2].qp_ext());
      /* p28.ZOZO*/ BUS_OAM_DAn_out[3].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[3].qp_ext());
      /* p28.ZUFO*/ BUS_OAM_DAn_out[4].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[4].qp_ext());
      /* p28.ZATO*/ BUS_OAM_DAn_out[5].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[5].qp_ext());
      /* p28.YVUC*/ BUS_OAM_DAn_out[6].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[6].qp_ext());
      /* p28.ZUFE*/ BUS_OAM_DAn_out[7].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[7].qp_ext());

      /* p28.ZAMY*/ BUS_OAM_DBn_out[0].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[0].qp_ext());
      /* p28.ZOPU*/ BUS_OAM_DBn_out[1].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[1].qp_ext());
      /* p28.WYKY*/ BUS_OAM_DBn_out[2].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[2].qp_ext());
      /* p28.ZAJA*/ BUS_OAM_DBn_out[3].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[3].qp_ext());
      /* p28.ZUGA*/ BUS_OAM_DBn_out[4].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[4].qp_ext());
      /* p28.ZUMO*/ BUS_OAM_DBn_out[5].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[5].qp_ext());
      /* p28.XYTO*/ BUS_OAM_DBn_out[6].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[6].qp_ext());
      /* p28.ZYFA*/ BUS_OAM_DBn_out[7].tri6_nn(_AZUL_CBD_TO_OBDn_new, BUS_CPU_Dp_ext[7].qp_ext());
    }

    /* p04.MAKA*/ oam_bus.MAKA_HOLD_MEMp.dff17(_ZEME_AxCxExGx_clk, _CUNU_SYS_RSTn_new, _CATY_LATCH_EXTp_ext);
    /* p04.NAXY*/ wire _NAXY_DMA_OAM_WRp_new = nor2(_UVYT_ABCDxxxx_clk, oam_bus.MAKA_HOLD_MEMp.qp_new()); // def nor2
    /* p04.POWU*/ wire _POWU_DMA_OAM_WRp_new  = and2(dma_reg.MATU_DMA_RUNNINGp.qp_new(), _NAXY_DMA_OAM_WRp_new); // def and
    /*#p28.ACYL*/ wire _ACYL_SCANNINGp_new   = and2(_BOGE_DMA_RUNNINGn_new, sprite_scanner.BESU_SCANNINGp.qp_new());
    /* p28.AJON*/ wire _AJON_PPU_OAM_ENp_new = and2(_BOGE_DMA_RUNNINGn_new, pix_pipe.XYMU_RENDERINGn.qn_new()); // def AND. ppu can read oam when there's rendering but no dma
    /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn_new   = nor3(dma_reg.MATU_DMA_RUNNINGp.qp_new(), _ACYL_SCANNINGp_new, _AJON_PPU_OAM_ENp_new); // def nor
    /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp_new = and2(_SARO_FE00_FEFFp_ext, _AJUJ_OAM_BUSYn_new); // def and
    /* p04.WYJA*/ wire _WYJA_OAM_WRp_new   = and_or3(_AMAB_CPU_OAM_ENp_new, _CUPA_CPU_WRp_xxxxEFGx_clk, _POWU_DMA_OAM_WRp_new);
    /* p28.YNYC*/ wire _YNYC_OAM_A_WRp_new = and2(_WYJA_OAM_WRp_new, _WAFO_OAM_A0n_new);
    /* p28.YLYC*/ wire _YLYC_OAM_B_WRp_new = and2(_WYJA_OAM_WRp_new, _GEKA_OAM_A0p_new);
    /* p28.ZOFE*/ wire _ZOFE_OAM_A_WRn_new = not1(_YNYC_OAM_A_WRp_new);
    /* p28.ZONE*/ wire _ZONE_OAM_B_WRn_new = not1(_YLYC_OAM_B_WRp_new);

    // FIXME This should be using PIN_OAM_CLK (which might actually be PIN_OAM_CSn?)
    PIN_OAM_WR_A.pin_in(1, _ZOFE_OAM_A_WRn_new);
    PIN_OAM_WR_B.pin_in(1, _ZONE_OAM_B_WRn_new);

    if (PIN_OAM_CLKn_old && !PIN_OAM_CLKn.qp_new()) {
      if (!PIN_OAM_WR_A.qp_ext()) {
        oam_ram[(oam_addr_latch << 1) + 0] = pack_u8n_new(8, &BUS_OAM_DAn_out[0]);
      }

      if (!PIN_OAM_WR_B.qp_ext()) {
        oam_ram[(oam_addr_latch << 1) + 1] = pack_u8n_new(8, &BUS_OAM_DBn_out[0]);
      }
    }
  }

#pragma endregion
}

//-----------------------------------------------------------------------------















































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

//-----------------------------------------------------------------------------

#endif
