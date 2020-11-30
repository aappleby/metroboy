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

  //LOG_G("Run reset sequence\n");
  //LOG_SCOPE_INDENT();

  //----------------------------------------

  CHECK_P(cart_buf != nullptr);
  CHECK_P(cart_size);

  // In reset
  //LOG_B("In reset\n");
  sys_rst = 1;
  sys_fastboot = fastboot;
  run(5);

  // Out of reset
  // Start clock and sync with phase
  //LOG_B("Out of reset\n");
  sys_rst = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  run(3);

  CHECK_N(clk_reg.AFUR_xxxxEFGHp_s.qp_old());
  CHECK_P(clk_reg.ALEF_AxxxxFGHp_s.qp_old());
  CHECK_P(clk_reg.APUK_ABxxxxGHp_s.qp_old());
  CHECK_P(clk_reg.ADYK_ABCxxxxHp_s.qp_old());

  // Wait for PIN_CPU_START
  //LOG_B("Wait for PIN_CPU_START\n");
  while(!sys_cpu_start) {
    run(8);
  }

  // Delay to sync w/ expected div value after bootrom
  run(8);
  run(8);

  // Done, initialize bus with whatever the CPU wants.
  //LOG_B("Reset done\n");
  cpu.reset_boot();
  sys_cpuready = 1;
  sys_cpu_en = true;
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
  const int o_lcd_pix        = offsetof(GateBoy, lcd_pix_lo);

} gb_offsets;

static std::set<int> bad_bits;

void GateBoy::next_phase() {

  //----------------------------------------
  // Run one pass of our simulation.

  uint8_t* blob_begin = ((uint8_t*)&sentinel1) + sizeof(sentinel1);
  uint8_t* blob_end   = ((uint8_t*)&sentinel2);

  probes.begin_pass(0);
  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);
  tock_slow();
  pass_total++;
  probes.end_pass(false);

  uint64_t pass_hash_old = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

  probes.begin_pass(1);
  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);
  tock_slow();
  pass_total++;
  probes.end_pass(true);

  uint64_t pass_hash_new = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

  if (pass_hash_old != pass_hash_new) {
    LOG_Y("Sim not stable after second pass!\n");
    ASSERT_P(false);
  }

  /*
  int start = offsetof(GateBoy, sentinel1) + sizeof(sentinel1);
  int end   = offsetof(GateBoy, sentinel2);

  uint8_t* blob_old = (uint8_t*)&gb_old;
  uint8_t* blob_new = (uint8_t*)this;

  for (int i = start; i < end; i++) {
    if (blob_old[i] != blob_new[i]) {
      printf("%06d %02d %04d %02d %02d\n", phase_total, pass_count, i, blob_old[i], blob_new[i]);
    }
  }
  */

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

void GateBoy::tock_slow() {
  if (DELTA_AB) {
    cpu_req = cpu.bus_req;
    bus_req = {0};
    if (sys_cpu_en) bus_req = cpu_req;
    if (dbg_req.read || dbg_req.write) bus_req = dbg_req;
  }

  // Interrupt acks
  Pin2 PIN_CPU_ACK_VBLANK; // bottom right port PORTB_01: -> P02.LETY, vblank int ack
  Pin2 PIN_CPU_ACK_STAT  ; // bottom right port PORTB_05: -> P02.LEJA, stat int ack
  Pin2 PIN_CPU_ACK_TIMER ; // bottom right port PORTB_09: -> P02.LESA, timer int ack
  Pin2 PIN_CPU_ACK_SERIAL; // bottom right port PORTB_13: -> P02.LUFE, serial int ack
  Pin2 PIN_CPU_ACK_JOYPAD; // bottom right port PORTB_17: -> P02.LAMO, joypad int ack

  PIN_CPU_ACK_VBLANK.pin_in(1, wire(cpu.int_ack & INT_VBLANK_MASK));
  PIN_CPU_ACK_STAT  .pin_in(1, wire(cpu.int_ack & INT_STAT_MASK));
  PIN_CPU_ACK_TIMER .pin_in(1, wire(cpu.int_ack & INT_TIMER_MASK));
  PIN_CPU_ACK_SERIAL.pin_in(1, wire(cpu.int_ack & INT_SERIAL_MASK));
  PIN_CPU_ACK_JOYPAD.pin_in(1, wire(cpu.int_ack & INT_JOYPAD_MASK));

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  Bus2 BUS_CPU_Dp_in[8];
  Bus2 BUS_CPU_Dp_out[8];

  uint16_t cpu_bus_addr;

  if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
    cpu_bus_addr = bus_req.addr;
  }
  else {
    cpu_bus_addr = bus_req.addr & 0xFF;
  }

  // PIN_CPU_RDp / PIN_CPU_WRp
  Pin2 PIN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  Pin2 PIN_CPU_WRp;           // top right port PORTA_01: ->
  if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
    PIN_CPU_RDp.pin_in(1, bus_req.read);
    PIN_CPU_WRp.pin_in(1, bus_req.write);
  }
  else {
    PIN_CPU_RDp.pin_in(1, 0);
    PIN_CPU_WRp.pin_in(1, 0);
  }

  // PIN_CPU_LATCH_EXT
  Pin2 PIN_CPU_LATCH_EXT_s;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  // not at all certain about this. seems to break some oam read glitches.
  if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req.read && (bus_req.addr < 0xFF00))) {
    PIN_CPU_LATCH_EXT_s.pin_in(1, 1);
  }
  else {
    PIN_CPU_LATCH_EXT_s.pin_in(1, 0);
  }
  /* p04.DECY*/ wire _DECY_LATCH_EXTn = not1(PIN_CPU_LATCH_EXT_s.qp_new());
  /* p04.CATY*/ wire _CATY_LATCH_EXTp = not1(_DECY_LATCH_EXTn);

  Pin2 PIN_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  PIN_CPU_6.pin_in(1, 0);


  // Data has to be driven on EFGH or we fail the wave tests

  wire BUS_CPU_OEp = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req.write;

  BUS_CPU_Dp_in[0].tri(BUS_CPU_OEp, bus_req.data_lo & 0x01);
  BUS_CPU_Dp_in[1].tri(BUS_CPU_OEp, bus_req.data_lo & 0x02);
  BUS_CPU_Dp_in[2].tri(BUS_CPU_OEp, bus_req.data_lo & 0x04);
  BUS_CPU_Dp_in[3].tri(BUS_CPU_OEp, bus_req.data_lo & 0x08);
  BUS_CPU_Dp_in[4].tri(BUS_CPU_OEp, bus_req.data_lo & 0x10);
  BUS_CPU_Dp_in[5].tri(BUS_CPU_OEp, bus_req.data_lo & 0x20);
  BUS_CPU_Dp_in[6].tri(BUS_CPU_OEp, bus_req.data_lo & 0x40);
  BUS_CPU_Dp_in[7].tri(BUS_CPU_OEp, bus_req.data_lo & 0x80);

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  wire GND = 0;
  wire _WEFE_VCC = 1;
  wire _VYPO_VCC = 1;
  wire _PESU_VCC = 1;

  //----------------------------------------
  // Address decoders

  bool BUS_CPU_A[16];
  BUS_CPU_A[ 0] = wire(cpu_bus_addr & 0x0001);
  BUS_CPU_A[ 1] = wire(cpu_bus_addr & 0x0002);
  BUS_CPU_A[ 2] = wire(cpu_bus_addr & 0x0004);
  BUS_CPU_A[ 3] = wire(cpu_bus_addr & 0x0008);
  BUS_CPU_A[ 4] = wire(cpu_bus_addr & 0x0010);
  BUS_CPU_A[ 5] = wire(cpu_bus_addr & 0x0020);
  BUS_CPU_A[ 6] = wire(cpu_bus_addr & 0x0040);
  BUS_CPU_A[ 7] = wire(cpu_bus_addr & 0x0080);
  BUS_CPU_A[ 8] = wire(cpu_bus_addr & 0x0100);
  BUS_CPU_A[ 9] = wire(cpu_bus_addr & 0x0200);
  BUS_CPU_A[10] = wire(cpu_bus_addr & 0x0400);
  BUS_CPU_A[11] = wire(cpu_bus_addr & 0x0800);
  BUS_CPU_A[12] = wire(cpu_bus_addr & 0x1000);
  BUS_CPU_A[13] = wire(cpu_bus_addr & 0x2000);
  BUS_CPU_A[14] = wire(cpu_bus_addr & 0x4000);
  BUS_CPU_A[15] = wire(cpu_bus_addr & 0x8000);

  /* p06.SARE*/ wire _SARE_XX00_XX07p = nor5 (BUS_CPU_A[ 7], BUS_CPU_A[ 6], BUS_CPU_A[ 5],
                                              BUS_CPU_A[ 4], BUS_CPU_A[ 3]);
  /* p07.TUNA*/ wire _TUNA_0000_FDFFp = nand7(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13],
                                              BUS_CPU_A[12], BUS_CPU_A[11], BUS_CPU_A[10],
                                              BUS_CPU_A[ 9]);

  /* p07.TULO*/ wire _TULO_00000000_XXp = nor8(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13], BUS_CPU_A[12],
                                               BUS_CPU_A[11], BUS_CPU_A[10], BUS_CPU_A[ 9], BUS_CPU_A[ 8]);
  /* p07.ZORO*/ wire _ZORO_0000xxxx_XXp = nor4(BUS_CPU_A[15], BUS_CPU_A[14], BUS_CPU_A[13], BUS_CPU_A[12]);
  /* p07.ZADU*/ wire _ZADU_xxxx0000_XXp = nor4(BUS_CPU_A[11], BUS_CPU_A[10], BUS_CPU_A[ 9], BUS_CPU_A[ 8]);

  /* p07.SEMY*/ wire _SEMY_XX_0000xxxxp = nor4(BUS_CPU_A[ 7], BUS_CPU_A[ 6], BUS_CPU_A[ 5], BUS_CPU_A[ 4]);
  /* p07.SAPA*/ wire _SAPA_XX_xxxx1111p = and4(BUS_CPU_A[ 0], BUS_CPU_A[ 1], BUS_CPU_A[ 2], BUS_CPU_A[ 3]);
  /* p22.XALY*/ wire _XALY_XX_0x00xxxxp = nor3(BUS_CPU_A[ 7], BUS_CPU_A[ 5], BUS_CPU_A[ 4]);
  /* p10.AMUS*/ wire _AMUS_XX_0xx00000p = nor6(BUS_CPU_A[ 0], BUS_CPU_A[ 1], BUS_CPU_A[ 2], BUS_CPU_A[ 3], BUS_CPU_A[ 4], BUS_CPU_A[ 7]);
  /* p07.TYRO*/ wire _TYFO_XX_0x0x0000p = nor6(BUS_CPU_A[ 7], BUS_CPU_A[ 5], BUS_CPU_A[ 3], BUS_CPU_A[ 2], BUS_CPU_A[ 1], BUS_CPU_A[ 0]);
  /* p07.TUFA*/ wire _TUFA_XX_x1x1xxxxp = and2(BUS_CPU_A[ 4], BUS_CPU_A[ 6]);

  /* p03.TOVY*/ wire _TOVY_A00n = not1(BUS_CPU_A[ 0]);
  /* p22.XOLA*/ wire _XOLA_A00n = not1(BUS_CPU_A[ 0]);
  /* p08.TOLA*/ wire _TOLA_A01n = not1(BUS_CPU_A[ 1]);
  /* p22.XENO*/ wire _XENO_A01n = not1(BUS_CPU_A[ 1]);
  /* p06.SEFY*/ wire _SEFY_A02n = not1(BUS_CPU_A[ 2]);
  /* p22.XUSY*/ wire _XUSY_A02n = not1(BUS_CPU_A[ 2]);
  /* p22.XERA*/ wire _XERA_A03n = not1(BUS_CPU_A[ 3]);
  /* p10.BYKO*/ wire _BYKO_A05n = not1(BUS_CPU_A[ 5]);
  /* p10.AKUG*/ wire _AKUG_A06n = not1(BUS_CPU_A[ 6]);
  /* p07.TONA*/ wire _TONA_A08n = not1(BUS_CPU_A[ 8]);

  /* p22.WADO*/ wire _WADO_A00p = not1(_XOLA_A00n);
  /* p22.WESA*/ wire _WESA_A01p = not1(_XENO_A01n);
  /* p22.WALO*/ wire _WALO_A02p = not1(_XUSY_A02n);
  /* p22.WEPO*/ wire _WEPO_A03p = not1(_XERA_A03n);

  /* p07.ZUFA*/ wire _ZUFA_0000_00FFp  = and2(_ZORO_0000xxxx_XXp, _ZADU_xxxx0000_XXp);
  /* p07.SYKE*/ wire _SYKE_FF00_FFFFp  = nor2(_TUNA_0000_FDFFp, _TONA_A08n);
  /* p03.RYFO*/ wire _RYFO_FF04_FF07p  = and3(_SYKE_FF00_FFFFp, BUS_CPU_A[ 2], _SARE_XX00_XX07p);
  /* p25.SYRO*/ wire _SYRO_FE00_FFFFp  = not1(_TUNA_0000_FDFFp);
  /* p07.RYCU*/ wire _RYCU_0000_FDFFn  = not1(_TUNA_0000_FDFFp);
  /* p07.SOHA*/ wire _SOHA_FF00_FFFFn  = not1(_SYKE_FF00_FFFFp);
  /* p07.ROPE*/ wire _ROPE_FE00_FEFFn  = nand2(_SOHA_FF00_FFFFn, _RYCU_0000_FDFFn);
  /* p07.SARO*/ wire _SARO_FE00_FEFFp  = not1(_ROPE_FE00_FEFFn);
  /* p28.ADAH*/ wire _ADAH_FE00_FEFFn  = not1(_SARO_FE00_FEFFp);
  /*#p08.SORE*/ wire _SORE_0000_7FFFp  = not1(BUS_CPU_A[15]);
  /*#p08.TEVY*/ wire _TEVY_8000_9FFFn  = or3(BUS_CPU_A[13], BUS_CPU_A[14], _SORE_0000_7FFFp);
  /* p06.SANO*/ wire _SANO_FF00_FF03p  = and3(_SARE_XX00_XX07p, _SEFY_A02n, _SYKE_FF00_FFFFp);
  /* p10.ANAP*/ wire _ANAP_FF_0xx00000 = and2(_SYKE_FF00_FFFFp, _AMUS_XX_0xx00000p);
  /* p22.WUTU*/ wire _WUTU_FF40_FF4Fn  = nand3(_SYKE_FF00_FFFFp, BUS_CPU_A[ 6], _XALY_XX_0x00xxxxp);
  /* p22.WERO*/ wire _WERO_FF40_FF4Fp  = not1(_WUTU_FF40_FF4Fn);

  /* p22.WORU*/ wire _WORU_FF40n = nand5(_WERO_FF40_FF4Fp, _XOLA_A00n, _XENO_A01n, _XUSY_A02n, _XERA_A03n);
  /* p22.WOFA*/ wire _WOFA_FF41n = nand5(_WERO_FF40_FF4Fp, _WADO_A00p, _XENO_A01n, _XUSY_A02n, _XERA_A03n);
  /* p22.WEBU*/ wire _WEBU_FF42n = nand5(_WERO_FF40_FF4Fp, _XOLA_A00n, _WESA_A01p, _XUSY_A02n, _XERA_A03n);
  /* p22.WAVU*/ wire _WAVU_FF43n = nand5(_WERO_FF40_FF4Fp, _WADO_A00p, _WESA_A01p, _XUSY_A02n, _XERA_A03n);
  /* p22.WYLE*/ wire _WYLE_FF44n = nand5(_WERO_FF40_FF4Fp, _XOLA_A00n, _XENO_A01n, _WALO_A02p, _XERA_A03n);
  /* p22.WETY*/ wire _WETY_FF45n = nand5(_WERO_FF40_FF4Fp, _WADO_A00p, _XENO_A01n, _WALO_A02p, _XERA_A03n);
  /*#p22.WATE*/ wire _WATE_FF46n = nand5(_WERO_FF40_FF4Fp, _XOLA_A00n, _WESA_A01p, _WALO_A02p, _XERA_A03n);
  /* p22.WYBO*/ wire _WYBO_FF47n = nand5(_WERO_FF40_FF4Fp, _WADO_A00p, _WESA_A01p, _WALO_A02p, _XERA_A03n);
  /* p22.WETA*/ wire _WETA_FF48n = nand5(_WERO_FF40_FF4Fp, _XOLA_A00n, _XENO_A01n, _XUSY_A02n, _WEPO_A03p);
  /* p22.VAMA*/ wire _VAMA_FF49n = nand5(_WERO_FF40_FF4Fp, _WADO_A00p, _XENO_A01n, _XUSY_A02n, _WEPO_A03p);
  /* p22.WYVO*/ wire _WYVO_FF4An = nand5(_WERO_FF40_FF4Fp, _XOLA_A00n, _WESA_A01p, _XUSY_A02n, _WEPO_A03p);
  /* p22.WAGE*/ wire _WAGE_FF4Bn = nand5(_WERO_FF40_FF4Fp, _WADO_A00p, _WESA_A01p, _XUSY_A02n, _WEPO_A03p);

  /* p22.VOCA*/ wire _VOCA_FF40p = not1(_WORU_FF40n);
  /* p22.VARY*/ wire _VARY_FF41p = not1(_WOFA_FF41n);
  /* p22.XARO*/ wire _XARO_FF42p = not1(_WEBU_FF42n);
  /* p22.XAVY*/ wire _XAVY_FF43p = not1(_WAVU_FF43n);
  /* p22.XOGY*/ wire _XOGY_FF44p = not1(_WYLE_FF44n);
  /* p22.XAYU*/ wire _XAYU_FF45p = not1(_WETY_FF45n);
  /*#p22.XEDA*/ wire _XEDA_FF46p = not1(_WATE_FF46n);
  /* p22.WERA*/ wire _WERA_FF47p = not1(_WYBO_FF47n);
  /* p22.XAYO*/ wire _XAYO_FF48p = not1(_WETA_FF48n);
  /* p22.TEGO*/ wire _TEGO_FF49p = not1(_VAMA_FF49n);
  /* p22.VYGA*/ wire _VYGA_FF4Ap = not1(_WYVO_FF4An);
  /* p22.VUMY*/ wire _VUMY_FF4Bp = not1(_WAGE_FF4Bn);

  /* p08.SOGY*/ wire _SOGY_A14n = not1(BUS_CPU_A[14]);
  /* p08.TUMA*/ wire _TUMA_CART_RAM = and3(BUS_CPU_A[13], _SOGY_A14n, BUS_CPU_A[15]);
  /* p08.TYNU*/ wire _TYNU_ADDR_RAM = and_or3(BUS_CPU_A[15], BUS_CPU_A[14], _TUMA_CART_RAM);

  {
    Pin2 PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
    PIN_CPU_ADDR_HIp.pin_in(1, _SYRO_FE00_FFFFp);
  }

  //----------------------------------------
  // Debug control signals.

  /* p07.UBET*/ wire _UBETp_new  = not1(wire(sys_t1));
  /* p07.UVAR*/ wire _UVARp_new  = not1(wire(sys_t2));
  /* p07.UMUT*/ wire _UMUT_MODE_DBG1p_new = and2(wire(sys_t1), _UVARp_new);
  /* p07.UNOR*/ wire _UNOR_MODE_DBG2p_new = and2(wire(sys_t2), _UBETp_new);
  /* p07.UPOJ*/ wire _UPOJ_MODE_PRODn_new = nand3(_UBETp_new, _UVARp_new, wire(sys_rst));
  /* p08.RYCA*/ wire _RYCA_MODE_DBG2n_new = not1(_UNOR_MODE_DBG2p_new);
  /* p08.TOVA*/ wire _TOVA_MODE_DBG2n_new = not1(_UNOR_MODE_DBG2p_new);
  /* p08.MULE*/ wire _MULE_MODE_DBG1n_new = not1(_UMUT_MODE_DBG1p_new);

  {
    Pin2 PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
    Pin2 PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
    PIN_CPU_UNOR_DBG.pin_in(1, _UNOR_MODE_DBG2p_new);
    PIN_CPU_UMUT_DBG.pin_in(1, _UMUT_MODE_DBG1p_new);
  }

  //----------------------------------------
  // Root clocks - ignoring the deglitcher here

  /* p01.ATAL*/ wire _ATAL_xBxDxFxH_clk = !(phase_total & 1) && sys_clken;
  /* p01.UCOB*/ wire _UCOB_CLKBADp_clk  = not1(sys_clkgood);
  /* p01.ATEZ*/ wire _ATEZ_CLKBADp_clk  = not1(sys_clkgood);
  /* p01.ABOL*/ wire _ABOL_CLKREQn_clk  = not1(sys_cpuready);

  /* p01.AZOF*/ wire _AZOF_AxCxExGx_clk = not1(_ATAL_xBxDxFxH_clk);
  /* p01.ZAXY*/ wire _ZAXY_xBxDxFxH_clk = not1(_AZOF_AxCxExGx_clk);
  /*#p01.ZEME*/ wire _ZEME_AxCxExGx_clk = not1(_ZAXY_xBxDxFxH_clk);
  /*#p01.ALET*/ wire _ALET_xBxDxFxH_clk = not1(_ZEME_AxCxExGx_clk);
  /*#p27.MYVO*/ wire _MYVO_AxCxExGx_clk = not1(_ALET_xBxDxFxH_clk);
  /*#p27.MEHE*/ wire _MEHE_AxCxExGx_clk = not1(_ALET_xBxDxFxH_clk);
  /* p01.LAPE*/ wire _LAPE_AxCxExGx_clk = not1(_ALET_xBxDxFxH_clk);
  /* p27.MOXE*/ wire _MOXE_AxCxExGx_clk = not1(_ALET_xBxDxFxH_clk);
  /* p27.TAVA*/ wire _TAVA_xBxDxFxH_clk = not1(_LAPE_AxCxExGx_clk);

  //----------------------------------------
  // Sys clock signals

  {
    wire AFUR_xxxxEFGHp_old = clk_reg.ADYK_ABCxxxxHp_s.qp_old();
    wire ALEF_AxxxxFGHn_old = clk_reg.AFUR_xxxxEFGHp_s.qn_old();
    wire APUK_ABxxxxGHn_old = clk_reg.ALEF_AxxxxFGHp_s.qn_old();
    wire ADYK_ABCxxxxHn_old = clk_reg.APUK_ABxxxxGHp_s.qn_old();

    /* p01.AFUR*/ clk_reg.AFUR_xxxxEFGHp_s.dff9(!_ATAL_xBxDxFxH_clk, _UPOJ_MODE_PRODn_new, AFUR_xxxxEFGHp_old);
    /* p01.ALEF*/ clk_reg.ALEF_AxxxxFGHp_s.dff9( _ATAL_xBxDxFxH_clk, _UPOJ_MODE_PRODn_new, ALEF_AxxxxFGHn_old);
    /* p01.APUK*/ clk_reg.APUK_ABxxxxGHp_s.dff9(!_ATAL_xBxDxFxH_clk, _UPOJ_MODE_PRODn_new, APUK_ABxxxxGHn_old);
    /* p01.ADYK*/ clk_reg.ADYK_ABCxxxxHp_s.dff9( _ATAL_xBxDxFxH_clk, _UPOJ_MODE_PRODn_new, ADYK_ABCxxxxHn_old);
  }

  /*#p01.ATYP*/ wire _ATYP_ABCDxxxx_clk = not1(clk_reg.AFUR_xxxxEFGHp_s.qp_new());
  /*#p01.AFEP*/ wire _AFEP_AxxxxFGH_clk = not1(clk_reg.ALEF_AxxxxFGHp_s.qn_new());
  /*#p01.AROV*/ wire _AROV_xxCDEFxx_clk = not1(clk_reg.APUK_ABxxxxGHp_s.qp_new());
  /*#p01.ADAR*/ wire _ADAR_ABCxxxxH_clk = not1(clk_reg.ADYK_ABCxxxxHp_s.qn_new());

  /*#p01.AFAS*/ wire _AFAS_xxxxEFGx_clk = nor2(_ADAR_ABCxxxxH_clk, _ATYP_ABCDxxxx_clk);
  /*#p01.BELU*/ wire _BELU_xxxxEFGH_clk = nor2(_ATYP_ABCDxxxx_clk, _ABOL_CLKREQn_clk);
  /*#p01.BYRY*/ wire _BYRY_ABCDxxxx_clk = not1(_BELU_xxxxEFGH_clk);
  /*#p01.BUDE*/ wire _BUDE_xxxxEFGH_clk = not1(_BYRY_ABCDxxxx_clk);
  /*#p01.BAPY*/ wire _BAPY_xxxxxxGH_clk = nor3(_ABOL_CLKREQn_clk, _AROV_xxCDEFxx_clk, _ATYP_ABCDxxxx_clk);
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
  /*#p01.BYJU*/ wire _BYJU_Axxxxxxx_clk = or2(_BELE_Axxxxxxx_clk, _ATEZ_CLKBADp_clk);
  /*#p01.BALY*/ wire _BALY_xBCDEFGH_clk = not1(_BYJU_Axxxxxxx_clk);
  /* p01.BOGA*/ wire _BOGA_Axxxxxxx_clk = not1(_BALY_xBCDEFGH_clk);
  /*#p01.AJAX*/ wire _AJAX_xxxxEFGH_clk = not1(_ATYP_ABCDxxxx_clk);
  /* p01.UVYT*/ wire _UVYT_ABCDxxxx_clk = not1(_BUDE_xxxxEFGH_clk);
  /* p04.MOPA*/ wire _MOPA_xxxxEFGH_clk = not1(_UVYT_ABCDxxxx_clk);

  //----------------------------------------
  // CPU write signals

  /* p01.AREV*/ wire _AREV_CPU_WRn_ABCDxxxH_new = nand2(PIN_CPU_WRp.qp_new(), _AFAS_xxxxEFGx_clk);
  /* p01.APOV*/ wire _APOV_CPU_WRp_xxxxEFGx_new = not1(_AREV_CPU_WRn_ABCDxxxH_new);
  /* p07.UBAL*/ wire _UBAL_CPU_WRn_ABCDxxxH_new = mux2n(_UNOR_MODE_DBG2p_new, /*PIN_EXT_WRn.qn()*/ 0, _APOV_CPU_WRp_xxxxEFGx_new); // Ignoring debug stuff for now
  /* p07.TAPU*/ wire _TAPU_CPU_WRp_xxxxEFGx_new = not1(_UBAL_CPU_WRn_ABCDxxxH_new);
  /* p07.DYKY*/ wire _DYKY_CPU_WRn_ABCDxxxH_new = not1(_TAPU_CPU_WRp_xxxxEFGx_new);
  /* p07.CUPA*/ wire _CUPA_CPU_WRp_xxxxEFGx_new = not1(_DYKY_CPU_WRn_ABCDxxxH_new);

  /* p10.ATOZ*/ wire _ATOZ_FF00_WRn_new = nand4(_TAPU_CPU_WRp_xxxxEFGx_new, _ANAP_FF_0xx00000, _AKUG_A06n, _BYKO_A05n);
  /* p06.URYS*/ wire _URYS_FF01_WRn_new = nand4(_TAPU_CPU_WRp_xxxxEFGx_new, _SANO_FF00_FF03p, BUS_CPU_A[ 0], _TOLA_A01n);
  /* p06.UWAM*/ wire _UWAM_FF02_WRn_new = nand4(_TAPU_CPU_WRp_xxxxEFGx_new, _SANO_FF00_FF03p, _TOVY_A00n, BUS_CPU_A[ 1]);
  /* p01.TAPE*/ wire _TAPE_FF04_WRp_new =  and4(_TAPU_CPU_WRp_xxxxEFGx_new, _RYFO_FF04_FF07p,  _TOLA_A01n, _TOVY_A00n);
  /*#p03.TOPE*/ wire _TOPE_FF05_WRn_new = nand4(_TAPU_CPU_WRp_xxxxEFGx_new, _RYFO_FF04_FF07p,  BUS_CPU_A[ 0], _TOLA_A01n);
  /* p03.TYJU*/ wire _TYJU_FF06_WRn_new = nand4(_TAPU_CPU_WRp_xxxxEFGx_new, _RYFO_FF04_FF07p,  BUS_CPU_A[ 1], _TOVY_A00n);
  /* p03.SARA*/ wire _SARA_FF07_WRn_new = nand4(_TAPU_CPU_WRp_xxxxEFGx_new, _RYFO_FF04_FF07p,  BUS_CPU_A[ 1], BUS_CPU_A[ 0]);
  /* p07.REFA*/ wire _REFA_FF0F_WRn_new = nand4(_TAPU_CPU_WRp_xxxxEFGx_new, _SYKE_FF00_FFFFp,  _SEMY_XX_0000xxxxp, _SAPA_XX_xxxx1111p); // schematic wrong, is NAND

  /* p23.WARU*/ wire _WARU_FF40_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _VOCA_FF40p);
  /* p21.SEPA*/ wire _SEPA_FF41_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _VARY_FF41p);
  /* p23.BEDY*/ wire _BEDY_FF42_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _XARO_FF42p);
  /* p23.ARUR*/ wire _ARUR_FF43_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _XAVY_FF43p);
  /* p23.XUFA*/ wire _XUFA_FF45_WRn_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _XAYU_FF45p);
  /*#p04.LAVY*/ wire _LAVY_FF46_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _XEDA_FF46p);
  /* p36.VELY*/ wire _VELY_FF47_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _WERA_FF47p);
  /* p36.XOMA*/ wire _XOMA_FF48_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _XAYO_FF48p);
  /* p36.MYXE*/ wire _MYXE_FF49_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _TEGO_FF49p);
  /* p23.WEKO*/ wire _WEKO_FF4A_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _VYGA_FF4Ap);
  /* p23.WUZA*/ wire _WUZA_FF4B_WRp_new = and2(_CUPA_CPU_WRp_xxxxEFGx_new, _VUMY_FF4Bp);

  /* p06.DAKU*/ wire _DAKU_FF01_WRp_new = not1(_URYS_FF01_WRn_new);
  /* p02.ROTU*/ wire _ROTU_FF0F_WRp_new = not1(_REFA_FF0F_WRn_new);
  /* p23.XUBO*/ wire _XUBO_FF40_WRn_new = not1(_WARU_FF40_WRp_new);
  /* p21.RYVE*/ wire _RYVE_FF41_WRn_new = not1(_SEPA_FF41_WRp_new);
  /* p21.RYJU*/ wire _RYJU_FF41_WRn_new = not1(_SEPA_FF41_WRp_new);
  /* p23.CAVO*/ wire _CAVO_FF42_WRn_new = not1(_BEDY_FF42_WRp_new);
  /* p23.AMUN*/ wire _AMUN_FF43_WRn_new = not1(_ARUR_FF43_WRp_new);
  /* p23.WANE*/ wire _WANE_FF45_WRp_new = not1(_XUFA_FF45_WRn_new);
  /* p36.TEPO*/ wire _TEPO_FF47_WRp_new = not1(_VELY_FF47_WRp_new);
  /* p36.XELO*/ wire _XELO_FF48_WRn_new = not1(_XOMA_FF48_WRp_new);
  /* p36.LEHO*/ wire _LEHO_FF49_WRn_new = not1(_MYXE_FF49_WRp_new);
  /* p23.VEFU*/ wire _VEFU_FF4A_WRn_new = not1(_WEKO_FF4A_WRp_new);
  /* p23.VOXU*/ wire _VOXU_FF4B_WRn_new = not1(_WUZA_FF4B_WRp_new);

  //----------------------------------------
  // Clocks that go back to the CPU

  {
    Pin2 PIN_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: // this is the "put address on bus" clock
    Pin2 PIN_CPU_BEDO_xBCDEFGH; // top left port PORTD_02:
    Pin2 PIN_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: // this is the "reset for next cycle" clock
    Pin2 PIN_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: // this is the "put write data on bus" clock
    Pin2 PIN_CPU_BOLO_ABCDEFxx; // top left port PORTD_05:
    Pin2 PIN_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: // this is probably the "latch bus data" clock
    Pin2 PIN_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: (RESET_CLK) // These two clocks are the only ones that run before PIN_CPU_READYp is asserted.
    Pin2 PIN_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: - test pad 3

    /*#p01.BUTY*/ wire _BUTY_CLKREQ   = not1(_ABOL_CLKREQn_clk);
    /*#p01.BUVU*/ wire _BUVU_Axxxxxxx = and2(_BALY_xBCDEFGH_clk, _BUTY_CLKREQ);
    /*#p01.BYXO*/ wire _BYXO_xBCDEFGH = not1(_BUVU_Axxxxxxx);
    /*#p01.BEDO*/ wire _BEDO_Axxxxxxx = not1(_BYXO_xBCDEFGH);
    /*#p01.BOWA*/ wire _BOWA_xBCDEFGH = not1(_BEDO_Axxxxxxx);

    /*#p01.BUGO*/ wire _BUGO_xBCDExxx = not1(_AFEP_AxxxxFGH_clk);
    /*#p01.BATE*/ wire _BATE_AxxxxxGH = nor3(_BUGO_xBCDExxx, _AROV_xxCDEFxx_clk, _ABOL_CLKREQn_clk);
    /*#p01.BASU*/ wire _BASU_xBCDEFxx = not1(_BATE_AxxxxxGH);
    /*#p01.BUKE*/ wire _BUKE_AxxxxxGH = not1(_BASU_xBCDEFxx);
    /*#p01.BOMA*/ wire _BOMA_xBCDEFGH = not1(_BOGA_Axxxxxxx_clk);

    PIN_CPU_BOWA_Axxxxxxx.pin_in(1, _BOWA_xBCDEFGH);
    PIN_CPU_BEDO_xBCDEFGH.pin_in(1, _BEDO_Axxxxxxx);
    PIN_CPU_BEKO_ABCDxxxx.pin_in(1, _BEKO_ABCDxxxx_clk);
    PIN_CPU_BUDE_xxxxEFGH.pin_in(1, _BUDE_xxxxEFGH_clk);
    PIN_CPU_BOLO_ABCDEFxx.pin_in(1, _BOLO_ABCDEFxx_clk);
    PIN_CPU_BUKE_AxxxxxGH.pin_in(1, _BUKE_AxxxxxGH);
    PIN_CPU_BOMA_xBCDEFGH.pin_in(1, _BOMA_xBCDEFGH);
    PIN_CPU_BOGA_Axxxxxxx.pin_in(1, _BOGA_Axxxxxxx_clk);
  }

  //----------------------------------------
  // CPU read signals

  // Ignoring debug stuff for now
  /* p07.UJYV*/ wire _UJYV_CPU_RDn_new = mux2n(_UNOR_MODE_DBG2p_new, /*PIN_EXT_RDn.qn()*/ 0, PIN_CPU_RDp.qp_new());
  /* p07.TEDO*/ wire _TEDO_CPU_RDp_new = not1(_UJYV_CPU_RDn_new);
  /* p07.AJAS*/ wire _AJAS_CPU_RDn_new = not1(_TEDO_CPU_RDp_new);
  /* p07.ASOT*/ wire _ASOT_CPU_RDp_new = not1(_AJAS_CPU_RDn_new);
  /* p08.REDU*/ wire _REDU_CPU_RDn_new = not1(_TEDO_CPU_RDp_new);

  /* p10.ACAT*/ wire _ACAT_FF00_RDp_new =  and4(_TEDO_CPU_RDp_new, _ANAP_FF_0xx00000,  _AKUG_A06n,         _BYKO_A05n);
  /* p06.UFEG*/ wire _UFEG_FF01_RDp_new =  and4(_TEDO_CPU_RDp_new, _SANO_FF00_FF03p,   BUS_CPU_A[ 0],      _TOLA_A01n);
  /* p06.UCOM*/ wire _UCOM_FF02_RDp_new =  and4(_TEDO_CPU_RDp_new, _SANO_FF00_FF03p,   _TOVY_A00n,         BUS_CPU_A[ 1]);
  /* p01.TAGY*/ wire _TAGY_FF04_RDp_new =  and4(_TEDO_CPU_RDp_new, _RYFO_FF04_FF07p,   _TOLA_A01n,         _TOVY_A00n);
  /*#p03.TEDA*/ wire _TEDA_FF05_RDp_new =  and4(_TEDO_CPU_RDp_new, _RYFO_FF04_FF07p,   _TOLA_A01n,         BUS_CPU_A[ 0]);
  /* p03.TUBY*/ wire _TUBY_FF06_RDp_new =  and4(_TEDO_CPU_RDp_new, _RYFO_FF04_FF07p,   BUS_CPU_A[ 1],      _TOVY_A00n);
  /* p03.SORA*/ wire _SORA_FF07_RDp_new =  and4(_TEDO_CPU_RDp_new, _RYFO_FF04_FF07p,   BUS_CPU_A[ 1],      BUS_CPU_A[ 0]);
  /* p07.ROLO*/ wire _ROLO_FF0F_RDn_new = nand4(_TEDO_CPU_RDp_new, _SYKE_FF00_FFFFp,   _SEMY_XX_0000xxxxp, _SAPA_XX_xxxx1111p); // schematic wrong, is NAND
  /* p07.TEXE*/ wire _TEXE_FF50_RDp_new =  and4(_TEDO_CPU_RDp_new, _SYKE_FF00_FFFFp,   _TYFO_XX_0x0x0000p, _TUFA_XX_x1x1xxxxp);
  /* p23.VYRE*/ wire _VYRE_FF40_RDp_new =  and2(_ASOT_CPU_RDp_new, _VOCA_FF40p);
  /* p21.TOBE*/ wire _TOBE_FF41_RDp_new =  and2(_ASOT_CPU_RDp_new, _VARY_FF41p);
  /* p23.ANYP*/ wire _ANYP_FF42_RDp_new =  and2(_ASOT_CPU_RDp_new, _XARO_FF42p);
  /* p23.AVOG*/ wire _AVOG_FF43_RDp_new =  and2(_ASOT_CPU_RDp_new, _XAVY_FF43p);
  /* p23.WAFU*/ wire _WAFU_FF44_RDp_new =  and2(_ASOT_CPU_RDp_new, _XOGY_FF44p);
  /* p23.XYLY*/ wire _XYLY_FF45_RDp_new =  and2(_ASOT_CPU_RDp_new, _XAYU_FF45p);
  /*#p04.MOLU*/ wire _MOLU_FF46_RDp_new =  and2(_ASOT_CPU_RDp_new, _XEDA_FF46p);
  /* p36.VUSO*/ wire _VUSO_FF47_RDp_new =  and2(_ASOT_CPU_RDp_new, _WERA_FF47p);
  /* p36.XUFY*/ wire _XUFY_FF48_RDp_new =  and2(_ASOT_CPU_RDp_new, _XAYO_FF48p);
  /* p36.MUMY*/ wire _MUMY_FF49_RDp_new =  and2(_ASOT_CPU_RDp_new, _TEGO_FF49p);
  /* p23.WAXU*/ wire _WAXU_FF4A_RDp_new =  and2(_ASOT_CPU_RDp_new, _VYGA_FF4Ap);
  /* p23.WYZE*/ wire _WYZE_FF4B_RDp_new =  and2(_ASOT_CPU_RDp_new, _VUMY_FF4Bp);

  /* p05.BYZO*/ wire _BYZO_FF00_RDn_new = not1(_ACAT_FF00_RDp_new);
  /* p02.POLA*/ wire _POLA_FF0F_RDp_new = not1(_ROLO_FF0F_RDn_new);
  /* p23.WYCE*/ wire _WYCE_FF40_RDn_new = not1(_VYRE_FF40_RDp_new);
  /* p21.VAVE*/ wire _VAVE_FF41_RDn_new = not1(_TOBE_FF41_RDp_new);
  /* p23.BUWY*/ wire _BUWY_FF42_RDn_new = not1(_ANYP_FF42_RDp_new);
  /* p23.BEBA*/ wire _BEBA_FF43_RDn_new = not1(_AVOG_FF43_RDp_new);
  /* p23.VARO*/ wire _VARO_FF44_RDn_new = not1(_WAFU_FF44_RDp_new);
  /* p23.WEKU*/ wire _WEKU_FF45_RDn_new = not1(_XYLY_FF45_RDp_new);
  /*#p04.NYGO*/ wire _NYGO_FF46_RDn_new = not1(_MOLU_FF46_RDp_new);
  /* p36.TEPY*/ wire _TEPY_FF47_RDn_new = not1(_VUSO_FF47_RDp_new);
  /* p36.XOZY*/ wire _XOZY_FF48_RDn_new = not1(_XUFY_FF48_RDp_new);
  /* p36.LOTE*/ wire _LOTE_FF49_RDn_new = not1(_MUMY_FF49_RDp_new);
  /* p23.VOMY*/ wire _VOMY_FF4A_RDn_new = not1(_WAXU_FF4A_RDp_new);
  /* p23.VYCU*/ wire _VYCU_FF4B_RDn_new = not1(_WYZE_FF4B_RDp_new);

  /*#p04.PUSY*/ wire _PUSY_FF46_RDp_new = not1(_NYGO_FF46_RDn_new);

  wire FFFF_HIT_new = cpu_bus_addr == 0xFFFF;
  wire FFFF_RDn_new = nand2(_TEDO_CPU_RDp_new, FFFF_HIT_new);

  //----------------------------------------

#pragma region DIV
  {
    /* p01.UFOL*/ wire _UFOL_DIV_RSTn_new = nor3(_UCOB_CLKBADp_clk, sys_rst, _TAPE_FF04_WRp_new);
    /* p01.UKUP*/ tim_reg.UKUP_DIV00p.dff17(_BOGA_Axxxxxxx_clk,           _UFOL_DIV_RSTn_new, tim_reg.UKUP_DIV00p.qn_old());
    /* p01.UFOR*/ tim_reg.UFOR_DIV01p.dff17(tim_reg.UKUP_DIV00p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.UFOR_DIV01p.qn_old());
    /* p01.UNER*/ tim_reg.UNER_DIV02p.dff17(tim_reg.UFOR_DIV01p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.UNER_DIV02p.qn_old());
    /*#p01.TERO*/ tim_reg.TERO_DIV03p.dff17(tim_reg.UNER_DIV02p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.TERO_DIV03p.qn_old());
    /* p01.UNYK*/ tim_reg.UNYK_DIV04p.dff17(tim_reg.TERO_DIV03p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.UNYK_DIV04p.qn_old());
    /* p01.TAMA*/ tim_reg.TAMA_DIV05p.dff17(tim_reg.UNYK_DIV04p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.TAMA_DIV05p.qn_old());

    // this is hacked up because we're ignoring the debug reg for the moment
    /* p01.ULUR*/ wire _ULUR_DIV_06_CLK_new = /*mux2p(FF60_1, _BOGA_Axxxxxxx,*/ tim_reg.TAMA_DIV05p.qn_new() /*)*/;

    /* p01.UGOT*/ tim_reg.UGOT_DIV06p.dff17(_ULUR_DIV_06_CLK_new,         _UFOL_DIV_RSTn_new, tim_reg.UGOT_DIV06p.qn_old());
    /* p01.TULU*/ tim_reg.TULU_DIV07p.dff17(tim_reg.UGOT_DIV06p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.TULU_DIV07p.qn_old());
    /* p01.TUGO*/ tim_reg.TUGO_DIV08p.dff17(tim_reg.TULU_DIV07p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.TUGO_DIV08p.qn_old());
    /* p01.TOFE*/ tim_reg.TOFE_DIV09p.dff17(tim_reg.TUGO_DIV08p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.TOFE_DIV09p.qn_old());
    /* p01.TERU*/ tim_reg.TERU_DIV10p.dff17(tim_reg.TOFE_DIV09p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.TERU_DIV10p.qn_old());
    /* p01.SOLA*/ tim_reg.SOLA_DIV11p.dff17(tim_reg.TERU_DIV10p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.SOLA_DIV11p.qn_old());
    /* p01.SUBU*/ tim_reg.SUBU_DIV12p.dff17(tim_reg.SOLA_DIV11p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.SUBU_DIV12p.qn_old());
    /* p01.TEKA*/ tim_reg.TEKA_DIV13p.dff17(tim_reg.SUBU_DIV12p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.TEKA_DIV13p.qn_old());
    /* p01.UKET*/ tim_reg.UKET_DIV14p.dff17(tim_reg.TEKA_DIV13p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.UKET_DIV14p.qn_old());
    /* p01.UPOF*/ tim_reg.UPOF_DIV15p.dff17(tim_reg.UKET_DIV14p.qn_new(), _UFOL_DIV_RSTn_new, tim_reg.UPOF_DIV15p.qn_old());
  }
#pragma endregion

  //----------------------------------------
  // Power-on reset handler

#pragma region RESET
  {
    /* p01.UPYF*/ wire _UPYF_new = or2(sys_rst, _UCOB_CLKBADp_clk);
    /* p01.TUBO*/ clk_reg.TUBO_WAITINGp_s.nor_latch(_UPYF_new, sys_cpuready);

    /* p01.UNUT*/ wire _UNUT_POR_TRIGn_new = and2(clk_reg.TUBO_WAITINGp_s.qp_new(), sys_fastboot ? tim_reg.TERO_DIV03p.qp_new() : tim_reg.UPOF_DIV15p.qp_new());
    /* p01.TABA*/ wire _TABA_POR_TRIGn_new = or3(_UNOR_MODE_DBG2p_new, _UMUT_MODE_DBG1p_new, _UNUT_POR_TRIGn_new);
    /*#p01.ALYP*/ wire _ALYP_RSTn_new = not1(_TABA_POR_TRIGn_new);
    /*#p01.AFAR*/ wire _AFAR_RSTp_new  = nor2(sys_rst, _ALYP_RSTn_new);
    /* p01.ASOL*/ clk_reg.ASOL_POR_DONEn_s.nor_latch(sys_rst, _AFAR_RSTp_new); // Schematic wrong, this is a latch.

    Pin2 PIN_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
    Pin2 PIN_CPU_EXT_RST;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
    Pin2 PIN_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET

    PIN_CPU_EXT_CLKGOOD.pin_in(1, sys_clkgood);
    PIN_CPU_EXT_RST.pin_in(1, sys_rst);
    PIN_CPU_STARTp.pin_in(1, _TABA_POR_TRIGn_new);
    sys_cpu_start = PIN_CPU_STARTp.qp_new();

    /* p01.AFER*/ clk_reg.AFER_SYS_RSTp_s.dff13(_BOGA_Axxxxxxx_clk, _UPOJ_MODE_PRODn_new, clk_reg.ASOL_POR_DONEn_s.qp_new());
    Pin2 PIN_CPU_SYS_RSTp;      // top center port PORTC_01: <- P01.AFER , reset related state
    PIN_CPU_SYS_RSTp.pin_in(1, clk_reg.AFER_SYS_RSTp_s.qp_new());
  }

  /*#p01.AVOR*/ wire _AVOR_SYS_RSTp_new = or2(clk_reg.AFER_SYS_RSTp_s.qp_new(), clk_reg.ASOL_POR_DONEn_s.qp_new());

  /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_new = not1(_AVOR_SYS_RSTp_new);
  /*#p01.DULA*/ wire _DULA_SYS_RSTp_new = not1(_ALUR_SYS_RSTn_new);
  /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_new = not1(_DULA_SYS_RSTp_new);
  /*#p01.XORE*/ wire _XORE_SYS_RSTp_new = not1(_CUNU_SYS_RSTn_new);
  /* p01.WESY*/ wire _WESY_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);
  /* p01.XEBE*/ wire _XEBE_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);
  /*#p01.WALU*/ wire _WALU_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);
  /* p01.XARE*/ wire _XARE_SYS_RSTn_new = not1(_XORE_SYS_RSTp_new);

#pragma endregion

  //----------------------------------------

  {
    // FF50 - disable bootrom bit
    /* p07.TUGE*/ wire _TUGE_FF50_WRn_new = nand4(_TAPU_CPU_WRp_xxxxEFGx_new, _SYKE_FF00_FFFFp, _TYFO_XX_0x0x0000p, _TUFA_XX_x1x1xxxxp);
    /* p07.SATO*/ wire _SATO_BOOT_BITn_old = or2(BUS_CPU_Dp_in[0].qp_new(), BOOT_BITn.qp_old());
    /* p07.TEPU*/ BOOT_BITn.dff17(_TUGE_FF50_WRn_new, _ALUR_SYS_RSTn_new, _SATO_BOOT_BITn_old);
  }
  /* p07.TERA*/ wire _TERA_BOOT_BITp_new  = not1(BOOT_BITn.qp_new());
  /* p07.TUTU*/ wire _TUTU_ADDR_BOOTp_new = and2(_TERA_BOOT_BITp_new, _TULO_00000000_XXp);

  Pin2 PIN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP
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

  /*#p08.TEXO*/ wire _TEXO_8000_9FFFn_new  = and2(PIN_CPU_EXT_BUSp.qp_new(), _TEVY_8000_9FFFn);
  /*#p25.TEFA*/ wire _TEFA_8000_9FFFp_new  = nor2(_SYRO_FE00_FFFFp, _TEXO_8000_9FFFn_new);
  /*#p25.SOSE*/ wire _SOSE_8000_9FFFp_new  = and2(_TEFA_8000_9FFFp_new, BUS_CPU_A[15]);
  /*#p01.AGUT*/ wire _AGUT_xxCDEFGH_new = or_and3(_AROV_xxCDEFxx_clk, _AJAX_xxxxEFGH_clk, PIN_CPU_EXT_BUSp.qp_new());
  /*#p01.AWOD*/ wire _AWOD_ABxxxxxx_new = nor2(_UNOR_MODE_DBG2p_new, _AGUT_xxCDEFGH_new);
  /*#p01.ABUZ*/ wire _ABUZ_xxCDEFGH_new = not1(_AWOD_ABxxxxxx_new);

  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_new = nor2(_TEXO_8000_9FFFn_new, _UMUT_MODE_DBG1p_new);
  /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_new = not1(_TEXO_8000_9FFFn_new);
  /* p08.LAGU*/ wire _LAGU_new = and_or3(PIN_CPU_RDp.qp_new(), _LEVO_ADDR_INT_OR_ADDR_VRAM_new, PIN_CPU_WRp.qp_new());
  /* p08.LYWE*/ wire _LYWE_new = not1(_LAGU_new);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_new = or2(_MOCA_DBG_EXT_RD_new, _LYWE_new);

  // Ignoring debug for now
  /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn_new = nand2(_SOSE_8000_9FFFp_new, PIN_CPU_WRp.qp_new());  // Schematic wrong, second input is PIN_CPU_WRp
  ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_VRAM_OEn.qn());
  /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn_new = /*mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp,*/ _TEGU_CPU_VRAM_WRn_new /*)*/;

  /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp_new = and2(_SOSE_8000_9FFFp_new, _ABUZ_xxCDEFGH_new);
  ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_VRAM_CSn.qn());
  /*#p25.TOLE*/ wire _TOLE_VRAM_RDp_new     = /*mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, */ _TUCA_CPU_VRAM_RDp_new /*)*/;

  {
    /*#p25.SYCY*/ wire _SYCY_DBG_CLOCKn_new = not1(_UNOR_MODE_DBG2p_new);
    /*#p25.SOTO*/ SOTO_DBG_VRAMp.dff17(_SYCY_DBG_CLOCKn_new, _CUNU_SYS_RSTn_new, SOTO_DBG_VRAMp.qn_old());
  }
  /* p25.TUTO*/ wire _TUTO_DBG_VRAMp_new = and2(_UNOR_MODE_DBG2p_new, SOTO_DBG_VRAMp.qn_new());
  /*#p25.RACO*/ wire _RACO_DBG_VRAMn_new = not1(_TUTO_DBG_VRAMp_new);

  //----------------------------------------

#pragma region Timer
  {
    /*#p03.MERY*/ wire _MERY_TIMER_OVERFLOWp_old = nor2(tim_reg.NUGA_TIMA7p.qp_old(), tim_reg.NYDU_TIMA7p_DELAY.qn_old());
    /*#p03.MOBA*/ tim_reg.MOBA_TIMER_OVERFLOWp.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, _MERY_TIMER_OVERFLOWp_old);

    /*#p03.MEKE*/ wire _MEKE_TIMER_OVERFLOWn_new = not1(tim_reg.MOBA_TIMER_OVERFLOWp.qp_new());
    /*#p03.MUZU*/ wire _MUZU_CPU_LOAD_TIMAn_new  = or2(PIN_CPU_LATCH_EXT_s.qp_new(), _TOPE_FF05_WRn_new);
    /*#p03.MEXU*/ wire _MEXU_TIMA_LOADp_new      = nand3(_MUZU_CPU_LOAD_TIMAn_new, _ALUR_SYS_RSTn_new, _MEKE_TIMER_OVERFLOWn_new);
    /*#p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn_new   = not1(_MEXU_TIMA_LOADp_new);
    /*#p03.NYDU*/ tim_reg.NYDU_TIMA7p_DELAY   .dff17(_BOGA_Axxxxxxx_clk, _MUGY_TIMA_MAX_RSTn_new, tim_reg.NUGA_TIMA7p.qp_old());

    // FF06 TMA
    /* p03.SABU*/ tim_reg.SABU_TMA0p.dff17(_TYJU_FF06_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p03.NYKE*/ tim_reg.NYKE_TMA1p.dff17(_TYJU_FF06_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p03.MURU*/ tim_reg.MURU_TMA2p.dff17(_TYJU_FF06_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p03.TYVA*/ tim_reg.TYVA_TMA3p.dff17(_TYJU_FF06_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p03.TYRU*/ tim_reg.TYRU_TMA4p.dff17(_TYJU_FF06_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p03.SUFY*/ tim_reg.SUFY_TMA5p.dff17(_TYJU_FF06_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p03.PETO*/ tim_reg.PETO_TMA6p.dff17(_TYJU_FF06_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p03.SETA*/ tim_reg.SETA_TMA7p.dff17(_TYJU_FF06_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[7].qp_new());

    // FF07 TAC
    /* p03.SOPU*/ tim_reg.SOPU_TAC0p.dff17(_SARA_FF07_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p03.SAMY*/ tim_reg.SAMY_TAC1p.dff17(_SARA_FF07_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p03.SABO*/ tim_reg.SABO_TAC2p.dff17(_SARA_FF07_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[2].qp_new());

    /*#p03.ROKE*/ wire _ROKE_TIMA_D0_new = mux2n(_TOPE_FF05_WRn_new, tim_reg.SABU_TMA0p.qp_new(), BUS_CPU_Dp_in[0].qp_new());
    /*#p03.PETU*/ wire _PETU_TIMA_D1_new = mux2n(_TOPE_FF05_WRn_new, tim_reg.NYKE_TMA1p.qp_new(), BUS_CPU_Dp_in[1].qp_new());
    /*#p03.NYKU*/ wire _NYKU_TIMA_D2_new = mux2n(_TOPE_FF05_WRn_new, tim_reg.MURU_TMA2p.qp_new(), BUS_CPU_Dp_in[2].qp_new());
    /*#p03.SOCE*/ wire _SOCE_TIMA_D3_new = mux2n(_TOPE_FF05_WRn_new, tim_reg.TYVA_TMA3p.qp_new(), BUS_CPU_Dp_in[3].qp_new());
    /*#p03.SALA*/ wire _SALA_TIMA_D4_new = mux2n(_TOPE_FF05_WRn_new, tim_reg.TYRU_TMA4p.qp_new(), BUS_CPU_Dp_in[4].qp_new());
    /*#p03.SYRU*/ wire _SYRU_TIMA_D5_new = mux2n(_TOPE_FF05_WRn_new, tim_reg.SUFY_TMA5p.qp_new(), BUS_CPU_Dp_in[5].qp_new());
    /*#p03.REFU*/ wire _REFU_TIMA_D6_new = mux2n(_TOPE_FF05_WRn_new, tim_reg.PETO_TMA6p.qp_new(), BUS_CPU_Dp_in[6].qp_new());
    /*#p03.RATO*/ wire _RATO_TIMA_D7_new = mux2n(_TOPE_FF05_WRn_new, tim_reg.SETA_TMA7p.qp_new(), BUS_CPU_Dp_in[7].qp_new());

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

    /*#p03.REGA*/ tim_reg.REGA_TIMA0p.dff20(_SOGU_TIMA_CLKn_new,          _MEXU_TIMA_LOADp_new, _PUXY_TIMA_D0_new);
    /*#p03.POVY*/ tim_reg.POVY_TIMA1p.dff20(tim_reg.REGA_TIMA0p.qp_new(), _MEXU_TIMA_LOADp_new, _NERO_TIMA_D1_new);
    /*#p03.PERU*/ tim_reg.PERU_TIMA2p.dff20(tim_reg.POVY_TIMA1p.qp_new(), _MEXU_TIMA_LOADp_new, _NADA_TIMA_D2_new);
    /*#p03.RATE*/ tim_reg.RATE_TIMA3p.dff20(tim_reg.PERU_TIMA2p.qp_new(), _MEXU_TIMA_LOADp_new, _REPA_TIMA_D3_new);
    /*#p03.RUBY*/ tim_reg.RUBY_TIMA4p.dff20(tim_reg.RATE_TIMA3p.qp_new(), _MEXU_TIMA_LOADp_new, _ROLU_TIMA_D4_new);
    /*#p03.RAGE*/ tim_reg.RAGE_TIMA5p.dff20(tim_reg.RUBY_TIMA4p.qp_new(), _MEXU_TIMA_LOADp_new, _RUGY_TIMA_D5_new);
    /*#p03.PEDA*/ tim_reg.PEDA_TIMA6p.dff20(tim_reg.RAGE_TIMA5p.qp_new(), _MEXU_TIMA_LOADp_new, _PYMA_TIMA_D6_new);
    /*#p03.NUGA*/ tim_reg.NUGA_TIMA7p.dff20(tim_reg.PEDA_TIMA6p.qp_new(), _MEXU_TIMA_LOADp_new, _PAGU_TIMA_D7_new);
  }
#pragma endregion

  //----------------------------------------

  /*#p23.VYXE*/ wire _VYXE_LCDC_BGENn_old   = pix_pipe.VYXE_LCDC_BGENn  .qp_old();
  /* p23.XYLO*/ wire _XYLO_LCDC_SPENn_old   = pix_pipe.XYLO_LCDC_SPENn  .qp_old();
  /* p23.XYMO*/ wire _XYMO_LCDC_SPSIZEn_old = pix_pipe.XYMO_LCDC_SPSIZEn.qp_old();
  /* p23.XAFO*/ wire _XAFO_LCDC_BGMAPn_old  = pix_pipe.XAFO_LCDC_BGMAPn .qp_old();
  /* p23.WEXU*/ wire _WEXU_LCDC_BGTILEn_old = pix_pipe.WEXU_LCDC_BGTILEn.qp_old();
  /* p23.WYMO*/ wire _WYMO_LCDC_WINENn_old  = pix_pipe.WYMO_LCDC_WINENn .qp_old();
  /* p23.WOKY*/ wire _WOKY_LCDC_WINMAPn_old = pix_pipe.WOKY_LCDC_WINMAPn.qp_old();
  /* p23.XONA*/ wire _XONA_LCDC_LCDENn_old  = pix_pipe.XONA_LCDC_LCDENn .qp_old();

  /*#p23.VYXE*/ wire _VYXE_LCDC_BGENp_old   = pix_pipe.VYXE_LCDC_BGENn  .qn_old();
  /* p23.XYLO*/ wire _XYLO_LCDC_SPENp_old   = pix_pipe.XYLO_LCDC_SPENn  .qn_old();
  /* p23.XYMO*/ wire _XYMO_LCDC_SPSIZEp_old = pix_pipe.XYMO_LCDC_SPSIZEn.qn_old();
  /* p23.XAFO*/ wire _XAFO_LCDC_BGMAPp_old  = pix_pipe.XAFO_LCDC_BGMAPn .qn_old();
  /* p23.WEXU*/ wire _WEXU_LCDC_BGTILEp_old = pix_pipe.WEXU_LCDC_BGTILEn.qn_old();
  /* p23.WYMO*/ wire _WYMO_LCDC_WINENp_old  = pix_pipe.WYMO_LCDC_WINENn .qn_old();
  /* p23.WOKY*/ wire _WOKY_LCDC_WINMAPp_old = pix_pipe.WOKY_LCDC_WINMAPn.qn_old();
  /* p23.XONA*/ wire _XONA_LCDC_LCDENp_old  = pix_pipe.XONA_LCDC_LCDENn .qn_old();

#pragma region Regs_Write
  {
    // JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
    // That also means that _both_ P14 and P15 are selected at reset :/
    /* p05.JUTE*/ joypad.JUTE_JOYP_RA     .dff17(_ATOZ_FF00_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p05.KECY*/ joypad.KECY_JOYP_LB     .dff17(_ATOZ_FF00_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p05.JALE*/ joypad.JALE_JOYP_UC     .dff17(_ATOZ_FF00_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p05.KYME*/ joypad.KYME_JOYP_DS     .dff17(_ATOZ_FF00_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p05.KELY*/ joypad.KELY_JOYP_UDLRp  .dff17(_ATOZ_FF00_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p05.COFY*/ joypad.COFY_JOYP_ABCSp  .dff17(_ATOZ_FF00_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p05.KUKO*/ joypad.KUKO_DBG_FF00_D6n.dff17(_ATOZ_FF00_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p05.KERU*/ joypad.KERU_DBG_FF00_D7n.dff17(_ATOZ_FF00_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[7].qp_new());
  }
  {
    // FF40 LCDC
    /*#p23.VYXE*/ pix_pipe.VYXE_LCDC_BGENn  .dff9(_XUBO_FF40_WRn_new, _XARE_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p23.XYLO*/ pix_pipe.XYLO_LCDC_SPENn  .dff9(_XUBO_FF40_WRn_new, _XARE_SYS_RSTn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p23.XYMO*/ pix_pipe.XYMO_LCDC_SPSIZEn.dff9(_XUBO_FF40_WRn_new, _XARE_SYS_RSTn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p23.XAFO*/ pix_pipe.XAFO_LCDC_BGMAPn .dff9(_XUBO_FF40_WRn_new, _XARE_SYS_RSTn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p23.WEXU*/ pix_pipe.WEXU_LCDC_BGTILEn.dff9(_XUBO_FF40_WRn_new, _XARE_SYS_RSTn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p23.WYMO*/ pix_pipe.WYMO_LCDC_WINENn .dff9(_XUBO_FF40_WRn_new, _XARE_SYS_RSTn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p23.WOKY*/ pix_pipe.WOKY_LCDC_WINMAPn.dff9(_XUBO_FF40_WRn_new, _XARE_SYS_RSTn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p23.XONA*/ pix_pipe.XONA_LCDC_LCDENn .dff9(_XUBO_FF40_WRn_new, _XARE_SYS_RSTn_new, BUS_CPU_Dp_in[7].qp_new());
  }
  {
    // FF41 STAT
    /* p21.ROXE*/ pix_pipe.ROXE_STAT_HBI_ENn_s.dff9(_RYVE_FF41_WRn_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p21.RUFO*/ pix_pipe.RUFO_STAT_VBI_ENn_s.dff9(_RYVE_FF41_WRn_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p21.REFE*/ pix_pipe.REFE_STAT_OAI_ENn_s.dff9(_RYVE_FF41_WRn_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p21.RUGU*/ pix_pipe.RUGU_STAT_LYI_ENn_s.dff9(_RYVE_FF41_WRn_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[6].qp_new());
  }
  {
    // FF42 SCY
    /* p23.GAVE*/ pix_pipe.GAVE_SCY0n_s.dff9(_CAVO_FF42_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p23.FYMO*/ pix_pipe.FYMO_SCY1n_s.dff9(_CAVO_FF42_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p23.FEZU*/ pix_pipe.FEZU_SCY2n_s.dff9(_CAVO_FF42_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p23.FUJO*/ pix_pipe.FUJO_SCY3n_s.dff9(_CAVO_FF42_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p23.DEDE*/ pix_pipe.DEDE_SCY4n_s.dff9(_CAVO_FF42_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p23.FOTY*/ pix_pipe.FOTY_SCY5n_s.dff9(_CAVO_FF42_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p23.FOHA*/ pix_pipe.FOHA_SCY6n_s.dff9(_CAVO_FF42_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p23.FUNY*/ pix_pipe.FUNY_SCY7n_s.dff9(_CAVO_FF42_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[7].qp_new());
  }
  {
    // FF43 SCX
    /* p23.DATY*/ pix_pipe.DATY_SCX0n_s.dff9(_AMUN_FF43_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p23.DUZU*/ pix_pipe.DUZU_SCX1n_s.dff9(_AMUN_FF43_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p23.CYXU*/ pix_pipe.CYXU_SCX2n_s.dff9(_AMUN_FF43_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p23.GUBO*/ pix_pipe.GUBO_SCX3n_s.dff9(_AMUN_FF43_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p23.BEMY*/ pix_pipe.BEMY_SCX4n_s.dff9(_AMUN_FF43_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p23.CUZY*/ pix_pipe.CUZY_SCX5n_s.dff9(_AMUN_FF43_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p23.CABU*/ pix_pipe.CABU_SCX6n_s.dff9(_AMUN_FF43_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p23.BAKE*/ pix_pipe.BAKE_SCX7n_s.dff9(_AMUN_FF43_WRn_new, _CUNU_SYS_RSTn_new, BUS_CPU_Dp_in[7].qp_new());
  }
  {
    // FF45 LYC
    /* p23.SYRY*/ lcd_reg.SYRY_LYC0n_s.dff9(_WANE_FF45_WRp_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p23.VUCE*/ lcd_reg.VUCE_LYC1n_s.dff9(_WANE_FF45_WRp_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p23.SEDY*/ lcd_reg.SEDY_LYC2n_s.dff9(_WANE_FF45_WRp_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p23.SALO*/ lcd_reg.SALO_LYC3n_s.dff9(_WANE_FF45_WRp_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p23.SOTA*/ lcd_reg.SOTA_LYC4n_s.dff9(_WANE_FF45_WRp_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p23.VAFA*/ lcd_reg.VAFA_LYC5n_s.dff9(_WANE_FF45_WRp_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p23.VEVO*/ lcd_reg.VEVO_LYC6n_s.dff9(_WANE_FF45_WRp_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p23.RAHA*/ lcd_reg.RAHA_LYC7n_s.dff9(_WANE_FF45_WRp_new, _WESY_SYS_RSTn_new, BUS_CPU_Dp_in[7].qp_new());
  }
  {
    // FF47 BGP
    /*#p36.PAVO*/ pix_pipe.PAVO_BGP_D0n_s.dff8p(_TEPO_FF47_WRp_new, BUS_CPU_Dp_in[0].qp_new());
    /* p36.NUSY*/ pix_pipe.NUSY_BGP_D1n_s.dff8p(_TEPO_FF47_WRp_new, BUS_CPU_Dp_in[1].qp_new());
    /* p36.PYLU*/ pix_pipe.PYLU_BGP_D2n_s.dff8p(_TEPO_FF47_WRp_new, BUS_CPU_Dp_in[2].qp_new());
    /* p36.MAXY*/ pix_pipe.MAXY_BGP_D3n_s.dff8p(_TEPO_FF47_WRp_new, BUS_CPU_Dp_in[3].qp_new());
    /* p36.MUKE*/ pix_pipe.MUKE_BGP_D4n_s.dff8p(_TEPO_FF47_WRp_new, BUS_CPU_Dp_in[4].qp_new());
    /* p36.MORU*/ pix_pipe.MORU_BGP_D5n_s.dff8p(_TEPO_FF47_WRp_new, BUS_CPU_Dp_in[5].qp_new());
    /* p36.MOGY*/ pix_pipe.MOGY_BGP_D6n_s.dff8p(_TEPO_FF47_WRp_new, BUS_CPU_Dp_in[6].qp_new());
    /* p36.MENA*/ pix_pipe.MENA_BGP_D7n_s.dff8p(_TEPO_FF47_WRp_new, BUS_CPU_Dp_in[7].qp_new());
  }
  {
    // FF48 OBP0
    /* p36.XUFU*/ pix_pipe.XUFU_OBP0_D0n_s.dff8p(_XELO_FF48_WRn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p36.XUKY*/ pix_pipe.XUKY_OBP0_D1n_s.dff8p(_XELO_FF48_WRn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p36.XOVA*/ pix_pipe.XOVA_OBP0_D2n_s.dff8p(_XELO_FF48_WRn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p36.XALO*/ pix_pipe.XALO_OBP0_D3n_s.dff8p(_XELO_FF48_WRn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p36.XERU*/ pix_pipe.XERU_OBP0_D4n_s.dff8p(_XELO_FF48_WRn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p36.XYZE*/ pix_pipe.XYZE_OBP0_D5n_s.dff8p(_XELO_FF48_WRn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p36.XUPO*/ pix_pipe.XUPO_OBP0_D6n_s.dff8p(_XELO_FF48_WRn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p36.XANA*/ pix_pipe.XANA_OBP0_D7n_s.dff8p(_XELO_FF48_WRn_new, BUS_CPU_Dp_in[7].qp_new());
  }
  {
    // FF49 OBP1
    /* p36.MOXY*/ pix_pipe.MOXY_OBP1_D0n_s.dff8p(_LEHO_FF49_WRn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p36.LAWO*/ pix_pipe.LAWO_OBP1_D1n_s.dff8p(_LEHO_FF49_WRn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p36.MOSA*/ pix_pipe.MOSA_OBP1_D2n_s.dff8p(_LEHO_FF49_WRn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p36.LOSE*/ pix_pipe.LOSE_OBP1_D3n_s.dff8p(_LEHO_FF49_WRn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p36.LUNE*/ pix_pipe.LUNE_OBP1_D4n_s.dff8p(_LEHO_FF49_WRn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p36.LUGU*/ pix_pipe.LUGU_OBP1_D5n_s.dff8p(_LEHO_FF49_WRn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p36.LEPU*/ pix_pipe.LEPU_OBP1_D6n_s.dff8p(_LEHO_FF49_WRn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p36.LUXO*/ pix_pipe.LUXO_OBP1_D7n_s.dff8p(_LEHO_FF49_WRn_new, BUS_CPU_Dp_in[7].qp_new());
  }
  {
    // FF4A WY
    /* p23.NESO*/ pix_pipe.NESO_WY0n_s.dff9(_VEFU_FF4A_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p23.NYRO*/ pix_pipe.NYRO_WY1n_s.dff9(_VEFU_FF4A_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p23.NAGA*/ pix_pipe.NAGA_WY2n_s.dff9(_VEFU_FF4A_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p23.MELA*/ pix_pipe.MELA_WY3n_s.dff9(_VEFU_FF4A_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p23.NULO*/ pix_pipe.NULO_WY4n_s.dff9(_VEFU_FF4A_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p23.NENE*/ pix_pipe.NENE_WY5n_s.dff9(_VEFU_FF4A_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p23.NUKA*/ pix_pipe.NUKA_WY6n_s.dff9(_VEFU_FF4A_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p23.NAFU*/ pix_pipe.NAFU_WY7n_s.dff9(_VEFU_FF4A_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[7].qp_new());
  }
  {
    // FF4B WX
    /* p23.MYPA*/ pix_pipe.MYPA_WX0n_s.dff9(_VOXU_FF4B_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp_new());
    /* p23.NOFE*/ pix_pipe.NOFE_WX1n_s.dff9(_VOXU_FF4B_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[1].qp_new());
    /* p23.NOKE*/ pix_pipe.NOKE_WX2n_s.dff9(_VOXU_FF4B_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[2].qp_new());
    /* p23.MEBY*/ pix_pipe.MEBY_WX3n_s.dff9(_VOXU_FF4B_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[3].qp_new());
    /* p23.MYPU*/ pix_pipe.MYPU_WX4n_s.dff9(_VOXU_FF4B_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[4].qp_new());
    /* p23.MYCE*/ pix_pipe.MYCE_WX5n_s.dff9(_VOXU_FF4B_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[5].qp_new());
    /* p23.MUVO*/ pix_pipe.MUVO_WX6n_s.dff9(_VOXU_FF4B_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[6].qp_new());
    /* p23.NUKU*/ pix_pipe.NUKU_WX7n_s.dff9(_VOXU_FF4B_WRn_new, _WALU_SYS_RSTn_new, BUS_CPU_Dp_in[7].qp_new());
  }
#pragma endregion

  //----------------------------------------
  // Video clock signals

  /* p01.XODO*/ wire _XODO_VID_RSTp_new = nand2(_XEBE_SYS_RSTn_new, pix_pipe.XONA_LCDC_LCDENn.qn_new());
  /* p01.XAPO*/ wire _XAPO_VID_RSTn_new = not1(_XODO_VID_RSTp_new);
  /*#p01.ATAR*/ wire _ATAR_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
  /*#p01.ABEZ*/ wire _ABEZ_VID_RSTn_new = not1(_ATAR_VID_RSTp_new);
  /* p01.LYHA*/ wire _LYHA_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
  /* p01.LYFE*/ wire _LYFE_VID_RSTn_new = not1(_LYHA_VID_RSTp_new);
  /* p01.PYRY*/ wire _PYRY_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
  /* p01.ROSY*/ wire _ROSY_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
  /* p01.AMYG*/ wire _AMYG_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);
  /* p01.TOFU*/ wire _TOFU_VID_RSTp_new = not1(_XAPO_VID_RSTn_new);

  {
    /* p29.XYVA*/ wire _XYVA_xBxDxFxH = not1(_ZEME_AxCxExGx_clk);
    /* p29.XOTA*/ wire _XOTA_AxCxExGx = not1(_XYVA_xBxDxFxH);
    /* p29.XYFY*/ wire _XYFY_xBxDxFxH = not1(_XOTA_AxCxExGx);
    /* p29.WOSU*/ clk_reg.WOSU_AxxDExxHp.dff17(_XYFY_xBxDxFxH,                  _XAPO_VID_RSTn_new, clk_reg.WUVU_ABxxEFxxp.qn_old());
    /* p29.WUVU*/ clk_reg.WUVU_ABxxEFxxp.dff17(_XOTA_AxCxExGx,                  _XAPO_VID_RSTn_new, clk_reg.WUVU_ABxxEFxxp.qn_old());
    /* p21.VENA*/ clk_reg.VENA_xxCDEFxxp.dff17(clk_reg.WUVU_ABxxEFxxp.qn_new(), _XAPO_VID_RSTn_new, clk_reg.VENA_xxCDEFxxp.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
  }
  /*#p29.XOCE*/ wire _XOCE_xBCxxFGx_new = not1(clk_reg.WOSU_AxxDExxHp.qp_new());
  /*#p29.XUPY*/ wire _XUPY_ABxxEFxx_new = not1(clk_reg.WUVU_ABxxEFxxp.qn_new());
  /*#p21.TALU*/ wire _TALU_xxCDEFxx_new = not1(clk_reg.VENA_xxCDEFxxp.qn_new());
  /*#p21.SONO*/ wire _SONO_ABxxxxGH_new = not1(_TALU_xxCDEFxx_new);
  /*#p28.AWOH*/ wire _AWOH_xxCDxxGH_new = not1(_XUPY_ABxxEFxx_new);

#pragma region DMA_Registers
  {
    /*#p04.LORU*/ wire _LORU_FF46_WRn = not1(_LAVY_FF46_WRp_new);
    /* p04.MOPA*/ wire _MOPA_xxxxEFGH = not1(_UVYT_ABCDxxxx_clk);

    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn_old = nand6(dma_reg.NAKY_DMA_A00p.qp_old(), dma_reg.PYRO_DMA_A01p.qp_old(),  // 128+16+8+4+2+1 = 159
                                               dma_reg.NEFY_DMA_A02p.qp_old(), dma_reg.MUTY_DMA_A03p.qp_old(),
                                               dma_reg.NYKO_DMA_A04p.qp_old(), dma_reg.MUGU_DMA_A07p.qp_old());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp_old = not1(_NAVO_DMA_DONEn_old);

    /*#p04.LUPA*/ wire _LUPA_DMA_TRIG_old = nor2(_LAVY_FF46_WRp_new, dma_reg.LYXE_DMA_LATCHp.qn_old());
    /*#p04.LUVY*/ dma_reg.LUVY_DMA_TRIG_d0.dff17(_UVYT_ABCDxxxx_clk, _CUNU_SYS_RSTn_new, _LUPA_DMA_TRIG_old);
    /*#p04.LENE*/ dma_reg.LENE_DMA_TRIG_d4.dff17(_MOPA_xxxxEFGH,   _CUNU_SYS_RSTn_new, dma_reg.LUVY_DMA_TRIG_d0.qp_new());

    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp_new = nand2(dma_reg.LENE_DMA_TRIG_d4.qn_new(), _CUNU_SYS_RSTn_new);
    /*#p04.LYXE*/ dma_reg.LYXE_DMA_LATCHp.nor_latch(_LAVY_FF46_WRp_new, _LOKO_DMA_RSTp_new);

    /*#p04.LAPA*/ wire _LAPA_DMA_RSTn_new = not1(_LOKO_DMA_RSTp_new);
    /*#p04.MYTE*/ dma_reg.MYTE_DMA_DONE.dff17(_MOPA_xxxxEFGH, _LAPA_DMA_RSTn_new, _NOLO_DMA_DONEp_old);

    /* p04.LARA*/ dma_reg.LARA_DMA_LATCHn = nand3(dma_reg.LOKY_DMA_LATCHp.qp_old(), dma_reg.MYTE_DMA_DONE.qn_new(), _CUNU_SYS_RSTn_new);
    /*#p04.LOKY*/ dma_reg.LOKY_DMA_LATCHp = nand2(dma_reg.LARA_DMA_LATCHn.qp_new(), dma_reg.LENE_DMA_TRIG_d4.qn_new());
    /* p04.LARA*/ dma_reg.LARA_DMA_LATCHn = nand3(dma_reg.LOKY_DMA_LATCHp.qp_new(), dma_reg.MYTE_DMA_DONE.qn_new(), _CUNU_SYS_RSTn_new);

    /*#p04.META*/ wire _META_DMA_CLKp_new = and2(_UVYT_ABCDxxxx_clk, dma_reg.LOKY_DMA_LATCHp.qp_new());
    /*#p04.NAKY*/ dma_reg.NAKY_DMA_A00p.dff17(_META_DMA_CLKp_new,                 _LAPA_DMA_RSTn_new, dma_reg.NAKY_DMA_A00p.qn_old());
    /*#p04.PYRO*/ dma_reg.PYRO_DMA_A01p.dff17(dma_reg.NAKY_DMA_A00p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.PYRO_DMA_A01p.qn_old());
    /* p04.NEFY*/ dma_reg.NEFY_DMA_A02p.dff17(dma_reg.PYRO_DMA_A01p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.NEFY_DMA_A02p.qn_old());
    /* p04.MUTY*/ dma_reg.MUTY_DMA_A03p.dff17(dma_reg.NEFY_DMA_A02p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.MUTY_DMA_A03p.qn_old());
    /* p04.NYKO*/ dma_reg.NYKO_DMA_A04p.dff17(dma_reg.MUTY_DMA_A03p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.NYKO_DMA_A04p.qn_old());
    /* p04.PYLO*/ dma_reg.PYLO_DMA_A05p.dff17(dma_reg.NYKO_DMA_A04p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.PYLO_DMA_A05p.qn_old());
    /* p04.NUTO*/ dma_reg.NUTO_DMA_A06p.dff17(dma_reg.PYLO_DMA_A05p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.NUTO_DMA_A06p.qn_old());
    /* p04.MUGU*/ dma_reg.MUGU_DMA_A07p.dff17(dma_reg.NUTO_DMA_A06p.qn_new(), _LAPA_DMA_RSTn_new, dma_reg.MUGU_DMA_A07p.qn_old());

    /*#p04.NAFA*/ dma_reg.NAFA_DMA_A08n.dff8p(_LORU_FF46_WRn, BUS_CPU_Dp_in[0].qp_new());
    /* p04.PYNE*/ dma_reg.PYNE_DMA_A09n.dff8p(_LORU_FF46_WRn, BUS_CPU_Dp_in[1].qp_new());
    /* p04.PARA*/ dma_reg.PARA_DMA_A10n.dff8p(_LORU_FF46_WRn, BUS_CPU_Dp_in[2].qp_new());
    /* p04.NYDO*/ dma_reg.NYDO_DMA_A11n.dff8p(_LORU_FF46_WRn, BUS_CPU_Dp_in[3].qp_new());
    /* p04.NYGY*/ dma_reg.NYGY_DMA_A12n.dff8p(_LORU_FF46_WRn, BUS_CPU_Dp_in[4].qp_new());
    /* p04.PULA*/ dma_reg.PULA_DMA_A13n.dff8p(_LORU_FF46_WRn, BUS_CPU_Dp_in[5].qp_new());
    /* p04.POKU*/ dma_reg.POKU_DMA_A14n.dff8p(_LORU_FF46_WRn, BUS_CPU_Dp_in[6].qp_new());
    /* p04.MARU*/ dma_reg.MARU_DMA_A15n.dff8p(_LORU_FF46_WRn, BUS_CPU_Dp_in[7].qp_new());

    /*#p04.MATU*/ dma_reg.MATU_DMA_RUNNINGp.dff17(_UVYT_ABCDxxxx_clk, _CUNU_SYS_RSTn_new, dma_reg.LOKY_DMA_LATCHp.qp_new());
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

    /*#p21.NYPE*/ lcd_reg.NYPE_x113p.dff17(_TALU_xxCDEFxx_new,          _LYFE_VID_RSTn_new, lcd_reg.RUTU_x113p.qp_old());
    /*#p21.RUTU*/ lcd_reg.RUTU_x113p.dff17(_SONO_ABxxxxGH_new,          _LYFE_VID_RSTn_new, _SANU_x113p_old);
    /*#p21.MYTA*/ lcd_reg.MYTA_y153p.dff17(lcd_reg.NYPE_x113p.qn_new(), _LYFE_VID_RSTn_new, _NOKO_y153p_old);

    /*#p29.CATU*/ lcd_reg.CATU_LINE_P000p .dff17(_XUPY_ABxxEFxx_new,          _ABEZ_VID_RSTn_new, _ABOV_LINE_P908p_old);
    /*#p28.ANEL*/ lcd_reg.ANEL_LINE_P002p .dff17(_AWOH_xxCDxxGH_new,          _ABEZ_VID_RSTn_new, lcd_reg.CATU_LINE_P000p.qp_new());
    /*#p21.POPU*/ lcd_reg.POPU_VBLANK_CLKp.dff17(lcd_reg.NYPE_x113p.qp_new(), _LYFE_VID_RSTn_new, _XYVO_y144p_old);
  }

  /* p28.ABAF*/ wire _ABAF_LINE_P000n_new = not1(lcd_reg.CATU_LINE_P000p.qp_new());
  /* p28.BYHA*/ wire _BYHA_LINE_TRIGn_new = or_and3(lcd_reg.ANEL_LINE_P002p.qp_new(), _ABAF_LINE_P000n_new, _ABEZ_VID_RSTn_new); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire _ATEJ_LINE_TRIGp_new = not1(_BYHA_LINE_TRIGn_new);

  {
    /*#p21.MUDE*/ wire _MUDE_X_RSTn_new = nor2(lcd_reg.RUTU_x113p.qp_new(), _LYHA_VID_RSTp_new);
    /*#p21.SAXO*/ lcd_reg.SAXO_LX0p.dff17(_TALU_xxCDEFxx_new,           _MUDE_X_RSTn_new, lcd_reg.SAXO_LX0p.qn_old());
    /*#p21.TYPO*/ lcd_reg.TYPO_LX1p.dff17(lcd_reg.SAXO_LX0p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.TYPO_LX1p.qn_old());
    /*#p21.VYZO*/ lcd_reg.VYZO_LX2p.dff17(lcd_reg.TYPO_LX1p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.VYZO_LX2p.qn_old());
    /*#p21.TELU*/ lcd_reg.TELU_LX3p.dff17(lcd_reg.VYZO_LX2p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.TELU_LX3p.qn_old());
    /*#p21.SUDE*/ lcd_reg.SUDE_LX4p.dff17(lcd_reg.TELU_LX3p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.SUDE_LX4p.qn_old());
    /*#p21.TAHA*/ lcd_reg.TAHA_LX5p.dff17(lcd_reg.SUDE_LX4p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.TAHA_LX5p.qn_old());
    /*#p21.TYRY*/ lcd_reg.TYRY_LX6p.dff17(lcd_reg.TAHA_LX5p.qn_new(),   _MUDE_X_RSTn_new, lcd_reg.TYRY_LX6p.qn_old());
  }

  {
    /*#p21.LAMA*/ wire _LAMA_Y_RSTn_new = nor2(lcd_reg.MYTA_y153p.qp_new(), _LYHA_VID_RSTp_new);
    /*#p21.MUWY*/ lcd_reg.MUWY_LY0p.dff17(lcd_reg.RUTU_x113p.qp_new(),  _LAMA_Y_RSTn_new, lcd_reg.MUWY_LY0p.qn_old());
    /*#p21.MYRO*/ lcd_reg.MYRO_LY1p.dff17(lcd_reg.MUWY_LY0p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.MYRO_LY1p.qn_old());
    /*#p21.LEXA*/ lcd_reg.LEXA_LY2p.dff17(lcd_reg.MYRO_LY1p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LEXA_LY2p.qn_old());
    /*#p21.LYDO*/ lcd_reg.LYDO_LY3p.dff17(lcd_reg.LEXA_LY2p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LYDO_LY3p.qn_old());
    /*#p21.LOVU*/ lcd_reg.LOVU_LY4p.dff17(lcd_reg.LYDO_LY3p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LOVU_LY4p.qn_old());
    /*#p21.LEMA*/ lcd_reg.LEMA_LY5p.dff17(lcd_reg.LOVU_LY4p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LEMA_LY5p.qn_old());
    /*#p21.MATO*/ lcd_reg.MATO_LY6p.dff17(lcd_reg.LEMA_LY5p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.MATO_LY6p.qn_old());
    /*#p21.LAFO*/ lcd_reg.LAFO_LY7p.dff17(lcd_reg.MATO_LY6p.qn_new(),   _LAMA_Y_RSTn_new, lcd_reg.LAFO_LY7p.qn_old());
  }

  //----------------------------------------
  // Map scroll adder

  /*#p26.FAFO*/ wire _FAFO_TILE_Y0S = add_s(lcd_reg.MUWY_LY0p.qp_new(), pix_pipe.GAVE_SCY0n_s.qn_new(), 0);
  /*#p26.FAFO*/ wire _FAFO_TILE_Y0C = add_c(lcd_reg.MUWY_LY0p.qp_new(), pix_pipe.GAVE_SCY0n_s.qn_new(), 0);
  /* p26.EMUX*/ wire _EMUX_TILE_Y1S = add_s(lcd_reg.MYRO_LY1p.qp_new(), pix_pipe.FYMO_SCY1n_s.qn_new(), _FAFO_TILE_Y0C);
  /* p26.EMUX*/ wire _EMUX_TILE_Y1C = add_c(lcd_reg.MYRO_LY1p.qp_new(), pix_pipe.FYMO_SCY1n_s.qn_new(), _FAFO_TILE_Y0C);
  /* p26.ECAB*/ wire _ECAB_TILE_Y2S = add_s(lcd_reg.LEXA_LY2p.qp_new(), pix_pipe.FEZU_SCY2n_s.qn_new(), _EMUX_TILE_Y1C);
  /* p26.ECAB*/ wire _ECAB_TILE_Y2C = add_c(lcd_reg.LEXA_LY2p.qp_new(), pix_pipe.FEZU_SCY2n_s.qn_new(), _EMUX_TILE_Y1C);
  /* p26.ETAM*/ wire _ETAM_MAP_Y0S  = add_s(lcd_reg.LYDO_LY3p.qp_new(), pix_pipe.FUJO_SCY3n_s.qn_new(), _ECAB_TILE_Y2C);
  /* p26.ETAM*/ wire _ETAM_MAP_Y0C  = add_c(lcd_reg.LYDO_LY3p.qp_new(), pix_pipe.FUJO_SCY3n_s.qn_new(), _ECAB_TILE_Y2C);
  /* p26.DOTO*/ wire _DOTO_MAP_Y1S  = add_s(lcd_reg.LOVU_LY4p.qp_new(), pix_pipe.DEDE_SCY4n_s.qn_new(), _ETAM_MAP_Y0C);
  /* p26.DOTO*/ wire _DOTO_MAP_Y1C  = add_c(lcd_reg.LOVU_LY4p.qp_new(), pix_pipe.DEDE_SCY4n_s.qn_new(), _ETAM_MAP_Y0C);
  /* p26.DABA*/ wire _DABA_MAP_Y2S  = add_s(lcd_reg.LEMA_LY5p.qp_new(), pix_pipe.FOTY_SCY5n_s.qn_new(), _DOTO_MAP_Y1C);
  /* p26.DABA*/ wire _DABA_MAP_Y2C  = add_c(lcd_reg.LEMA_LY5p.qp_new(), pix_pipe.FOTY_SCY5n_s.qn_new(), _DOTO_MAP_Y1C);
  /* p26.EFYK*/ wire _EFYK_MAP_Y3S  = add_s(lcd_reg.MATO_LY6p.qp_new(), pix_pipe.FOHA_SCY6n_s.qn_new(), _DABA_MAP_Y2C);
  /* p26.EFYK*/ wire _EFYK_MAP_Y3C  = add_c(lcd_reg.MATO_LY6p.qp_new(), pix_pipe.FOHA_SCY6n_s.qn_new(), _DABA_MAP_Y2C);
  /* p26.EJOK*/ wire _EJOK_MAP_Y4S  = add_s(lcd_reg.LAFO_LY7p.qp_new(), pix_pipe.FUNY_SCY7n_s.qn_new(), _EFYK_MAP_Y3C);
  ///* p26.EJOK*/ wire _EJOK_MAP_Y4C  = add_c(lcd_reg.LAFO_LY7p.qp17_new(), pix_pipe.FUNY_SCY7n.qn08_new(), _EFYK_MAP_Y3C);

#pragma endregion

  /* p28.YFEL*/ wire _YFEL_SCAN0_old = sprite_scanner.YFEL_SCAN0.qp_old();
  /* p28.WEWY*/ wire _WEWY_SCAN1_old = sprite_scanner.WEWY_SCAN1.qp_old();
  /* p28.GOSO*/ wire _GOSO_SCAN2_old = sprite_scanner.GOSO_SCAN2.qp_old();
  /* p28.ELYN*/ wire _ELYN_SCAN3_old = sprite_scanner.ELYN_SCAN3.qp_old();
  /* p28.FAHA*/ wire _FAHA_SCAN4_old = sprite_scanner.FAHA_SCAN4.qp_old();
  /* p28.FONY*/ wire _FONY_SCAN5_old = sprite_scanner.FONY_SCAN5.qp_old();


  /* p29.CEHA*/ wire _CEHA_SCANNINGp_old = not1(sprite_scanner.CENO_SCANNINGp.qn_old());
  /*#p29.BYJO*/ wire _BYJO_SCANNINGn_old = not1(_CEHA_SCANNINGp_old);

  /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp_new = [&]() {
    /*#p28.ANOM*/ wire _ANOM_LINE_RSTn_new  = nor2(_ATEJ_LINE_TRIGp_new, _ATAR_VID_RSTp_new);

    // FIXME this is weird

    /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4.RSTn(_ANOM_LINE_RSTn_new);
    /* p28.FONY*/ sprite_scanner.FONY_SCAN5.RSTn(_ANOM_LINE_RSTn_new);

    /*#p28.FETO*/ wire _FETO_SCAN_DONEp = and4(sprite_scanner.YFEL_SCAN0.qp(), sprite_scanner.WEWY_SCAN1.qp(), sprite_scanner.GOSO_SCAN2.qp(), sprite_scanner.FONY_SCAN5.qp()); // 32 + 4 + 2 + 1 = 39
    /* p28.GAVA*/ wire _GAVA_ABxxEFxx = or2(_FETO_SCAN_DONEp, _XUPY_ABxxEFxx_new);
    /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0.dff17(_GAVA_ABxxEFxx,                 _ANOM_LINE_RSTn_new, sprite_scanner.YFEL_SCAN0.qn());
    /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1.dff17(sprite_scanner.YFEL_SCAN0.qn(), _ANOM_LINE_RSTn_new, sprite_scanner.WEWY_SCAN1.qn());
    /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2.dff17(sprite_scanner.WEWY_SCAN1.qn(), _ANOM_LINE_RSTn_new, sprite_scanner.GOSO_SCAN2.qn());
    /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3.dff17(sprite_scanner.GOSO_SCAN2.qn(), _ANOM_LINE_RSTn_new, sprite_scanner.ELYN_SCAN3.qn());
    /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4.dff17(sprite_scanner.ELYN_SCAN3.qn(), _ANOM_LINE_RSTn_new, sprite_scanner.FAHA_SCAN4.qn());
    /* p28.FONY*/ sprite_scanner.FONY_SCAN5.dff17(sprite_scanner.FAHA_SCAN4.qn(), _ANOM_LINE_RSTn_new, sprite_scanner.FONY_SCAN5.qn());

    /*#p29.BALU*/ wire _BALU_LINE_RSTp_new  = not1(_ANOM_LINE_RSTn_new);
    /*#p29.BAGY*/ wire _BAGY_LINE_RSTn_new  = not1(_BALU_LINE_RSTp_new);
    /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(_XUPY_ABxxEFxx_new, _BAGY_LINE_RSTn_new, _FETO_SCAN_DONEp);
    /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(_ALET_xBxDxFxH_clk, _BAGY_LINE_RSTn_new, sprite_scanner.BYBA_SCAN_DONE_Ap.qp_new());
    /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn_new = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp_new(), _BALU_LINE_RSTp_new, sprite_scanner.BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp_new = not1(_BEBU_SCAN_DONE_TRIGn_new);
    /*#p28.ASEN*/ wire _ASEN_SCAN_DONE_TRIGp_new = or2(_ATAR_VID_RSTp_new, _AVAP_SCAN_DONE_TRIGp_new);
    /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd_reg.CATU_LINE_P000p.qp_new(), _ASEN_SCAN_DONE_TRIGp_new);
    /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGp.dff17(_XUPY_ABxxEFxx_new, _ABEZ_VID_RSTn_new,  sprite_scanner.BESU_SCANNINGp.qp_new());

    return _AVAP_SCAN_DONE_TRIGp_new;
  }();

  /* p29.CEHA*/ wire _CEHA_SCANNINGp_new = not1(sprite_scanner.CENO_SCANNINGp.qn());
  /*#p29.BYJO*/ wire _BYJO_SCANNINGn_new = not1(_CEHA_SCANNINGp_new);































































































  //--------------------------------------------------------------------------------------------------------------------------------------------
  // THE LINE OF GOODNESS AND DECENCY
  //--------------------------------------------------------------------------------------------------------------------------------------------



















































  /*#p27.MYLO*/ wire _MYLO_WX_MATCH0p_old = xnor2(pix_pipe.XEHO_PX0p.qp_old(), pix_pipe.MYPA_WX0n_s.qn_new());
  /* p27.PUWU*/ wire _PUWU_WX_MATCH1p_old = xnor2(pix_pipe.SAVY_PX1p.qp_old(), pix_pipe.NOFE_WX1n_s.qn_new());
  /* p27.PUHO*/ wire _PUHO_WX_MATCH2p_old = xnor2(pix_pipe.XODU_PX2p.qp_old(), pix_pipe.NOKE_WX2n_s.qn_new());
  /* p27.NYTU*/ wire _NYTU_WX_MATCH3p_old = xnor2(pix_pipe.XYDO_PX3p.qp_old(), pix_pipe.MEBY_WX3n_s.qn_new());
  /* p27.NEZO*/ wire _NEZO_WX_MATCH4p_old = xnor2(pix_pipe.TUHU_PX4p.qp_old(), pix_pipe.MYPU_WX4n_s.qn_new());
  /* p27.NORY*/ wire _NORY_WX_MATCH5p_old = xnor2(pix_pipe.TUKY_PX5p.qp_old(), pix_pipe.MYCE_WX5n_s.qn_new());
  /* p27.NONO*/ wire _NONO_WX_MATCH6p_old = xnor2(pix_pipe.TAKO_PX6p.qp_old(), pix_pipe.MUVO_WX6n_s.qn_new());
  /* p27.PASE*/ wire _PASE_WX_MATCH7p_old = xnor2(pix_pipe.SYBE_PX7p.qp_old(), pix_pipe.NUKU_WX7n_s.qn_new());
  /* p27.PUKY*/ wire _PUKY_WX_MATCHn_old  = nand5(pix_pipe.REJO_WY_MATCHp.qp_old(), _NEZO_WX_MATCH4p_old, _NORY_WX_MATCH5p_old, _NONO_WX_MATCH6p_old, _PASE_WX_MATCH7p_old);
  /* p27.NUFA*/ wire _NUFA_WX_MATCHp_old  = not1(_PUKY_WX_MATCHn_old);
  /* p27.NOGY*/ wire _NOGY_WX_MATCHn_old  = nand5(_NUFA_WX_MATCHp_old, _MYLO_WX_MATCH0p_old, _PUWU_WX_MATCH1p_old, _PUHO_WX_MATCH2p_old, _NYTU_WX_MATCH3p_old);
  /* p27.NUKO*/ wire _NUKO_WX_MATCHp_old  = not1(_NOGY_WX_MATCHn_old);










































  /*#p21.ACAM*/ wire _ACAM_PX0n_old = not1(pix_pipe.XEHO_PX0p.qp_old());
  /* p21.AZUB*/ wire _AZUB_PX1n_old = not1(pix_pipe.SAVY_PX1p.qp_old());
  /* p21.AMEL*/ wire _AMEL_PX2n_old = not1(pix_pipe.XODU_PX2p.qp_old());
  /* p21.AHAL*/ wire _AHAL_PX3n_old = not1(pix_pipe.XYDO_PX3p.qp_old());
  /* p21.APUX*/ wire _APUX_PX4n_old = not1(pix_pipe.TUHU_PX4p.qp_old());
  /* p21.ABEF*/ wire _ABEF_PX5n_old = not1(pix_pipe.TUKY_PX5p.qp_old());
  /* p21.ADAZ*/ wire _ADAZ_PX6n_old = not1(pix_pipe.TAKO_PX6p.qp_old());
  /* p21.ASAH*/ wire _ASAH_PX7n_old = not1(pix_pipe.SYBE_PX7p.qp_old());

  /*#p31.ZOGY*/ wire _ZOGY_STORE0_MATCH0n_old = xor2(sprite_store.XEPE_STORE0_X0p.qn_old(), _ACAM_PX0n_old);
  /* p31.ZEBA*/ wire _ZEBA_STORE0_MATCH1n_old = xor2(sprite_store.YLAH_STORE0_X1p.qn_old(), _AZUB_PX1n_old);
  /* p31.ZAHA*/ wire _ZAHA_STORE0_MATCH2n_old = xor2(sprite_store.ZOLA_STORE0_X2p.qn_old(), _AMEL_PX2n_old);
  /* p31.ZOKY*/ wire _ZOKY_STORE0_MATCH3n_old = xor2(sprite_store.ZULU_STORE0_X3p.qn_old(), _AHAL_PX3n_old);
  /* p31.WOJU*/ wire _WOJU_STORE0_MATCH4n_old = xor2(sprite_store.WELO_STORE0_X4p.qn_old(), _APUX_PX4n_old);
  /* p31.YFUN*/ wire _YFUN_STORE0_MATCH5n_old = xor2(sprite_store.XUNY_STORE0_X5p.qn_old(), _ABEF_PX5n_old);
  /* p31.WYZA*/ wire _WYZA_STORE0_MATCH6n_old = xor2(sprite_store.WOTE_STORE0_X6p.qn_old(), _ADAZ_PX6n_old);
  /* p31.YPUK*/ wire _YPUK_STORE0_MATCH7n_old = xor2(sprite_store.XAKO_STORE0_X7p.qn_old(), _ASAH_PX7n_old);

  /* p31.EDYM*/ wire _EDYM_STORE1_MATCH0n_old = xor2(sprite_store.DANY_STORE1_X0p.qn_old(), _ACAM_PX0n_old);
  /* p31.EMYB*/ wire _EMYB_STORE1_MATCH1n_old = xor2(sprite_store.DUKO_STORE1_X1p.qn_old(), _AZUB_PX1n_old);
  /* p31.EBEF*/ wire _EBEF_STORE1_MATCH2n_old = xor2(sprite_store.DESU_STORE1_X2p.qn_old(), _AMEL_PX2n_old);
  /* p31.EWOK*/ wire _EWOK_STORE1_MATCH3n_old = xor2(sprite_store.DAZO_STORE1_X3p.qn_old(), _AHAL_PX3n_old);
  /* p31.COLA*/ wire _COLA_STORE1_MATCH4n_old = xor2(sprite_store.DAKE_STORE1_X4p.qn_old(), _APUX_PX4n_old);
  /* p31.BOBA*/ wire _BOBA_STORE1_MATCH5n_old = xor2(sprite_store.CESO_STORE1_X5p.qn_old(), _ABEF_PX5n_old);
  /* p31.COLU*/ wire _COLU_STORE1_MATCH6n_old = xor2(sprite_store.DYFU_STORE1_X6p.qn_old(), _ADAZ_PX6n_old);
  /* p31.BAHU*/ wire _BAHU_STORE1_MATCH7n_old = xor2(sprite_store.CUSY_STORE1_X7p.qn_old(), _ASAH_PX7n_old);

  /* p31.FUZU*/ wire _FUZU_STORE2_MATCH0n_old = xor2(sprite_store.FOKA_STORE2_X0p.qn_old(), _ACAM_PX0n_old);
  /* p31.FESO*/ wire _FESO_STORE2_MATCH1n_old = xor2(sprite_store.FYTY_STORE2_X1p.qn_old(), _AZUB_PX1n_old);
  /* p31.FOKY*/ wire _FOKY_STORE2_MATCH2n_old = xor2(sprite_store.FUBY_STORE2_X2p.qn_old(), _AMEL_PX2n_old);
  /* p31.FYVA*/ wire _FYVA_STORE2_MATCH3n_old = xor2(sprite_store.GOXU_STORE2_X3p.qn_old(), _AHAL_PX3n_old);
  /* p31.CEKO*/ wire _CEKO_STORE2_MATCH4n_old = xor2(sprite_store.DUHY_STORE2_X4p.qn_old(), _APUX_PX4n_old);
  /* p31.DETY*/ wire _DETY_STORE2_MATCH5n_old = xor2(sprite_store.EJUF_STORE2_X5p.qn_old(), _ABEF_PX5n_old);
  /* p31.DOZO*/ wire _DOZO_STORE2_MATCH6n_old = xor2(sprite_store.ENOR_STORE2_X6p.qn_old(), _ADAZ_PX6n_old);
  /* p31.CONY*/ wire _CONY_STORE2_MATCH7n_old = xor2(sprite_store.DEPY_STORE2_X7p.qn_old(), _ASAH_PX7n_old);

  /* p31.YHOK*/ wire _YHOK_STORE3_MATCH0n_old = xor2(sprite_store.XOLY_STORE3_X0p.qn_old(), _ACAM_PX0n_old);
  /* p31.YCAH*/ wire _YCAH_STORE3_MATCH1n_old = xor2(sprite_store.XYBA_STORE3_X1p.qn_old(), _AZUB_PX1n_old);
  /* p31.YDAJ*/ wire _YDAJ_STORE3_MATCH2n_old = xor2(sprite_store.XABE_STORE3_X2p.qn_old(), _AMEL_PX2n_old);
  /* p31.YVUZ*/ wire _YVUZ_STORE3_MATCH3n_old = xor2(sprite_store.XEKA_STORE3_X3p.qn_old(), _AHAL_PX3n_old);
  /* p31.YVAP*/ wire _YVAP_STORE3_MATCH4n_old = xor2(sprite_store.XOMY_STORE3_X4p.qn_old(), _APUX_PX4n_old);
  /* p31.XENY*/ wire _XENY_STORE3_MATCH5n_old = xor2(sprite_store.WUHA_STORE3_X5p.qn_old(), _ABEF_PX5n_old);
  /* p31.XAVU*/ wire _XAVU_STORE3_MATCH6n_old = xor2(sprite_store.WYNA_STORE3_X6p.qn_old(), _ADAZ_PX6n_old);
  /* p31.XEVA*/ wire _XEVA_STORE3_MATCH7n_old = xor2(sprite_store.WECO_STORE3_X7p.qn_old(), _ASAH_PX7n_old);

  /* p31.XEJU*/ wire _XEJU_STORE4_MATCH0n_old = xor2(sprite_store.WEDU_STORE4_X0p.qn_old(), _ACAM_PX0n_old);
  /* p31.ZATE*/ wire _ZATE_STORE4_MATCH1n_old = xor2(sprite_store.YGAJ_STORE4_X1p.qn_old(), _AZUB_PX1n_old);
  /* p31.ZAKU*/ wire _ZAKU_STORE4_MATCH2n_old = xor2(sprite_store.ZYJO_STORE4_X2p.qn_old(), _AMEL_PX2n_old);
  /* p31.YBOX*/ wire _YBOX_STORE4_MATCH3n_old = xor2(sprite_store.XURY_STORE4_X3p.qn_old(), _AHAL_PX3n_old);
  /* p31.ZYKU*/ wire _ZYKU_STORE4_MATCH4n_old = xor2(sprite_store.YBED_STORE4_X4p.qn_old(), _APUX_PX4n_old);
  /* p31.ZYPU*/ wire _ZYPU_STORE4_MATCH5n_old = xor2(sprite_store.ZALA_STORE4_X5p.qn_old(), _ABEF_PX5n_old);
  /* p31.XAHA*/ wire _XAHA_STORE4_MATCH6n_old = xor2(sprite_store.WYDE_STORE4_X6p.qn_old(), _ADAZ_PX6n_old);
  /* p31.ZEFE*/ wire _ZEFE_STORE4_MATCH7n_old = xor2(sprite_store.XEPA_STORE4_X7p.qn_old(), _ASAH_PX7n_old);

  /* p31.GUZO*/ wire _GUZO_STORE5_MATCH0n_old = xor2(sprite_store.FUSA_STORE5_X0p.qn_old(), _ACAM_PX0n_old);
  /* p31.GOLA*/ wire _GOLA_STORE5_MATCH1n_old = xor2(sprite_store.FAXA_STORE5_X1p.qn_old(), _AZUB_PX1n_old);
  /* p31.GEVE*/ wire _GEVE_STORE5_MATCH2n_old = xor2(sprite_store.FOZY_STORE5_X2p.qn_old(), _AMEL_PX2n_old);
  /* p31.GUDE*/ wire _GUDE_STORE5_MATCH3n_old = xor2(sprite_store.FESY_STORE5_X3p.qn_old(), _AHAL_PX3n_old);
  /* p31.BAZY*/ wire _BAZY_STORE5_MATCH4n_old = xor2(sprite_store.CYWE_STORE5_X4p.qn_old(), _APUX_PX4n_old);
  /* p31.CYLE*/ wire _CYLE_STORE5_MATCH5n_old = xor2(sprite_store.DYBY_STORE5_X5p.qn_old(), _ABEF_PX5n_old);
  /* p31.CEVA*/ wire _CEVA_STORE5_MATCH6n_old = xor2(sprite_store.DURY_STORE5_X6p.qn_old(), _ADAZ_PX6n_old);
  /* p31.BUMY*/ wire _BUMY_STORE5_MATCH7n_old = xor2(sprite_store.CUVY_STORE5_X7p.qn_old(), _ASAH_PX7n_old);

  /* p31.XOSU*/ wire _XOSU_STORE6_MATCH0n_old = xor2(sprite_store.YCOL_STORE6_X0p.qn_old(), _ACAM_PX0n_old);
  /* p31.ZUVU*/ wire _ZUVU_STORE6_MATCH1n_old = xor2(sprite_store.YRAC_STORE6_X1p.qn_old(), _AZUB_PX1n_old);
  /* p31.XUCO*/ wire _XUCO_STORE6_MATCH2n_old = xor2(sprite_store.YMEM_STORE6_X2p.qn_old(), _AMEL_PX2n_old);
  /* p31.ZULO*/ wire _ZULO_STORE6_MATCH3n_old = xor2(sprite_store.YVAG_STORE6_X3p.qn_old(), _AHAL_PX3n_old);
  /* p31.ZARE*/ wire _ZARE_STORE6_MATCH4n_old = xor2(sprite_store.ZOLY_STORE6_X4p.qn_old(), _APUX_PX4n_old);
  /* p31.ZEMU*/ wire _ZEMU_STORE6_MATCH5n_old = xor2(sprite_store.ZOGO_STORE6_X5p.qn_old(), _ABEF_PX5n_old);
  /* p31.ZYGO*/ wire _ZYGO_STORE6_MATCH6n_old = xor2(sprite_store.ZECU_STORE6_X6p.qn_old(), _ADAZ_PX6n_old);
  /* p31.ZUZY*/ wire _ZUZY_STORE6_MATCH7n_old = xor2(sprite_store.ZESA_STORE6_X7p.qn_old(), _ASAH_PX7n_old);

  /* p31.DUSE*/ wire _DUSE_STORE7_MATCH0n_old = xor2(sprite_store.ERAZ_STORE7_X0p.qn_old(), _ACAM_PX0n_old);
  /* p31.DAGU*/ wire _DAGU_STORE7_MATCH1n_old = xor2(sprite_store.EPUM_STORE7_X1p.qn_old(), _AZUB_PX1n_old);
  /* p31.DYZE*/ wire _DYZE_STORE7_MATCH2n_old = xor2(sprite_store.EROL_STORE7_X2p.qn_old(), _AMEL_PX2n_old);
  /* p31.DESO*/ wire _DESO_STORE7_MATCH3n_old = xor2(sprite_store.EHYN_STORE7_X3p.qn_old(), _AHAL_PX3n_old);
  /* p31.EJOT*/ wire _EJOT_STORE7_MATCH4n_old = xor2(sprite_store.FAZU_STORE7_X4p.qn_old(), _APUX_PX4n_old);
  /* p31.ESAJ*/ wire _ESAJ_STORE7_MATCH5n_old = xor2(sprite_store.FAXE_STORE7_X5p.qn_old(), _ABEF_PX5n_old);
  /* p31.DUCU*/ wire _DUCU_STORE7_MATCH6n_old = xor2(sprite_store.EXUK_STORE7_X6p.qn_old(), _ADAZ_PX6n_old);
  /* p31.EWUD*/ wire _EWUD_STORE7_MATCH7n_old = xor2(sprite_store.FEDE_STORE7_X7p.qn_old(), _ASAH_PX7n_old);

  /* p31.DUZE*/ wire _DUZE_STORE8_MATCH0n_old = xor2(sprite_store.EZUF_STORE8_X0p.qn_old(), _APUX_PX4n_old);
  /* p31.DAGA*/ wire _DAGA_STORE8_MATCH1n_old = xor2(sprite_store.ENAD_STORE8_X1p.qn_old(), _ABEF_PX5n_old);
  /* p31.DAWU*/ wire _DAWU_STORE8_MATCH2n_old = xor2(sprite_store.EBOW_STORE8_X2p.qn_old(), _ADAZ_PX6n_old);
  /* p31.EJAW*/ wire _EJAW_STORE8_MATCH3n_old = xor2(sprite_store.FYCA_STORE8_X3p.qn_old(), _ASAH_PX7n_old);
  /* p31.GOHO*/ wire _GOHO_STORE8_MATCH4n_old = xor2(sprite_store.GAVY_STORE8_X4p.qn_old(), _ACAM_PX0n_old);
  /* p31.GASU*/ wire _GASU_STORE8_MATCH5n_old = xor2(sprite_store.GYPU_STORE8_X5p.qn_old(), _AZUB_PX1n_old);
  /* p31.GABU*/ wire _GABU_STORE8_MATCH6n_old = xor2(sprite_store.GADY_STORE8_X6p.qn_old(), _AMEL_PX2n_old);
  /* p31.GAFE*/ wire _GAFE_STORE8_MATCH7n_old = xor2(sprite_store.GAZA_STORE8_X7p.qn_old(), _AHAL_PX3n_old);

  /* p31.YMAM*/ wire _YMAM_STORE9_MATCH0n_old = xor2(sprite_store.XUVY_STORE9_X0p.qn_old(), _ACAM_PX0n_old);
  /* p31.YTYP*/ wire _YTYP_STORE9_MATCH1n_old = xor2(sprite_store.XERE_STORE9_X1p.qn_old(), _AZUB_PX1n_old);
  /* p31.YFOP*/ wire _YFOP_STORE9_MATCH2n_old = xor2(sprite_store.XUZO_STORE9_X2p.qn_old(), _AMEL_PX2n_old);
  /* p31.YVAC*/ wire _YVAC_STORE9_MATCH3n_old = xor2(sprite_store.XEXA_STORE9_X3p.qn_old(), _AHAL_PX3n_old);
  /* p31.ZYWU*/ wire _ZYWU_STORE9_MATCH4n_old = xor2(sprite_store.YPOD_STORE9_X4p.qn_old(), _APUX_PX4n_old);
  /* p31.ZUZA*/ wire _ZUZA_STORE9_MATCH5n_old = xor2(sprite_store.YROP_STORE9_X5p.qn_old(), _ABEF_PX5n_old);
  /* p31.ZEJO*/ wire _ZEJO_STORE9_MATCH6n_old = xor2(sprite_store.YNEP_STORE9_X6p.qn_old(), _ADAZ_PX6n_old);
  /* p31.ZEDA*/ wire _ZEDA_STORE9_MATCH7n_old = xor2(sprite_store.YZOF_STORE9_X7p.qn_old(), _ASAH_PX7n_old);

  /* p31.ZAKO*/ wire _ZAKO_STORE0_MATCHAp_old = nor4(_ZOGY_STORE0_MATCH0n_old, _ZEBA_STORE0_MATCH1n_old, _ZAHA_STORE0_MATCH2n_old, _ZOKY_STORE0_MATCH3n_old);
  /* p31.XEBA*/ wire _XEBA_STORE0_MATCHBp_old = nor4(_WOJU_STORE0_MATCH4n_old, _YFUN_STORE0_MATCH5n_old, _WYZA_STORE0_MATCH6n_old, _YPUK_STORE0_MATCH7n_old);
  /* p31.CYVY*/ wire _CYVY_STORE1_MATCHBp_old = nor4(_COLA_STORE1_MATCH4n_old, _BOBA_STORE1_MATCH5n_old, _COLU_STORE1_MATCH6n_old, _BAHU_STORE1_MATCH7n_old);
  /* p31.EWAM*/ wire _EWAM_STORE1_MATCHAp_old = nor4(_EDYM_STORE1_MATCH0n_old, _EMYB_STORE1_MATCH1n_old, _EBEF_STORE1_MATCH2n_old, _EWOK_STORE1_MATCH3n_old);
  /* p31.CEHU*/ wire _CEHU_STORE2_MATCHAp_old = nor4(_CEKO_STORE2_MATCH4n_old, _DETY_STORE2_MATCH5n_old, _DOZO_STORE2_MATCH6n_old, _CONY_STORE2_MATCH7n_old);
  /* p31.EKES*/ wire _EKES_STORE2_MATCHBp_old = nor4(_FUZU_STORE2_MATCH0n_old, _FESO_STORE2_MATCH1n_old, _FOKY_STORE2_MATCH2n_old, _FYVA_STORE2_MATCH3n_old);
  /* p31.ZURE*/ wire _ZURE_STORE3_MATCHAp_old = nor4(_YHOK_STORE3_MATCH0n_old, _YCAH_STORE3_MATCH1n_old, _YDAJ_STORE3_MATCH2n_old, _YVUZ_STORE3_MATCH3n_old);
  /* p31.YWOS*/ wire _YWOS_STORE3_MATCHBp_old = nor4(_YVAP_STORE3_MATCH4n_old, _XENY_STORE3_MATCH5n_old, _XAVU_STORE3_MATCH6n_old, _XEVA_STORE3_MATCH7n_old);
  /* p31.YKOK*/ wire _YKOK_STORE4_MATCHAp_old = nor4(_XEJU_STORE4_MATCH0n_old, _ZATE_STORE4_MATCH1n_old, _ZAKU_STORE4_MATCH2n_old, _YBOX_STORE4_MATCH3n_old);
  /* p31.YNAZ*/ wire _YNAZ_STORE4_MATCHBp_old = nor4(_ZYKU_STORE4_MATCH4n_old, _ZYPU_STORE4_MATCH5n_old, _XAHA_STORE4_MATCH6n_old, _ZEFE_STORE4_MATCH7n_old);
  /* p31.COGY*/ wire _COGY_STORE5_MATCHAp_old = nor4(_BAZY_STORE5_MATCH4n_old, _CYLE_STORE5_MATCH5n_old, _CEVA_STORE5_MATCH6n_old, _BUMY_STORE5_MATCH7n_old);
  /* p31.FYMA*/ wire _FYMA_STORE5_MATCHBp_old = nor4(_GUZO_STORE5_MATCH0n_old, _GOLA_STORE5_MATCH1n_old, _GEVE_STORE5_MATCH2n_old, _GUDE_STORE5_MATCH3n_old);
  /* p31.YWAP*/ wire _YWAP_STORE6_MATCHAp_old = nor4(_ZARE_STORE6_MATCH4n_old, _ZEMU_STORE6_MATCH5n_old, _ZYGO_STORE6_MATCH6n_old, _ZUZY_STORE6_MATCH7n_old);
  /* p31.YDOT*/ wire _YDOT_STORE6_MATCHBp_old = nor4(_XOSU_STORE6_MATCH0n_old, _ZUVU_STORE6_MATCH1n_old, _XUCO_STORE6_MATCH2n_old, _ZULO_STORE6_MATCH3n_old);
  /* p31.CYCO*/ wire _CYCO_STORE7_MATCHAp_old = nor4(_DUSE_STORE7_MATCH0n_old, _DAGU_STORE7_MATCH1n_old, _DYZE_STORE7_MATCH2n_old, _DESO_STORE7_MATCH3n_old);
  /* p31.DAJE*/ wire _DAJE_STORE7_MATCHBp_old = nor4(_EJOT_STORE7_MATCH4n_old, _ESAJ_STORE7_MATCH5n_old, _DUCU_STORE7_MATCH6n_old, _EWUD_STORE7_MATCH7n_old);
  /* p31.DAMA*/ wire _DAMA_STORE8_MATCHAp_old = nor4(_DUZE_STORE8_MATCH0n_old, _DAGA_STORE8_MATCH1n_old, _DAWU_STORE8_MATCH2n_old, _EJAW_STORE8_MATCH3n_old);
  /* p31.FEHA*/ wire _FEHA_STORE8_MATCHBp_old = nor4(_GOHO_STORE8_MATCH4n_old, _GASU_STORE8_MATCH5n_old, _GABU_STORE8_MATCH6n_old, _GAFE_STORE8_MATCH7n_old);
  /* p31.YLEV*/ wire _YLEV_STORE9_MATCHAp_old = nor4(_YMAM_STORE9_MATCH0n_old, _YTYP_STORE9_MATCH1n_old, _YFOP_STORE9_MATCH2n_old, _YVAC_STORE9_MATCH3n_old);
  /* p31.YTUB*/ wire _YTUB_STORE9_MATCHBp_old = nor4(_ZYWU_STORE9_MATCH4n_old, _ZUZA_STORE9_MATCH5n_old, _ZEJO_STORE9_MATCH6n_old, _ZEDA_STORE9_MATCH7n_old);

  /*#p29.AZEM*/ wire _AZEM_RENDERINGp_old = and2(pix_pipe.XYMU_RENDERINGn.qn_old(), _BYJO_SCANNINGn_old);
  /*#p29.AROR*/ wire _AROR_MATCH_ENp_old     = and2(_AZEM_RENDERINGp_old, _XYLO_LCDC_SPENp_old);
  /* p29.YDUG*/ wire _YDUG_STORE0_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _ZAKO_STORE0_MATCHAp_old, _XEBA_STORE0_MATCHBp_old);
  /* p29.DYDU*/ wire _DYDU_STORE1_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _EWAM_STORE1_MATCHAp_old, _CYVY_STORE1_MATCHBp_old);
  /* p29.DEGO*/ wire _DEGO_STORE2_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _CEHU_STORE2_MATCHAp_old, _EKES_STORE2_MATCHBp_old);
  /* p29.YLOZ*/ wire _YLOZ_STORE3_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _ZURE_STORE3_MATCHAp_old, _YWOS_STORE3_MATCHBp_old);
  /* p29.XAGE*/ wire _XAGE_STORE4_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _YKOK_STORE4_MATCHAp_old, _YNAZ_STORE4_MATCHBp_old);
  /* p29.EGOM*/ wire _EGOM_STORE5_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _COGY_STORE5_MATCHAp_old, _FYMA_STORE5_MATCHBp_old);
  /* p29.YBEZ*/ wire _YBEZ_STORE6_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _YWAP_STORE6_MATCHAp_old, _YDOT_STORE6_MATCHBp_old);
  /* p29.DYKA*/ wire _DYKA_STORE7_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _CYCO_STORE7_MATCHAp_old, _DAJE_STORE7_MATCHBp_old);
  /* p29.EFYL*/ wire _EFYL_STORE8_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _DAMA_STORE8_MATCHAp_old, _FEHA_STORE8_MATCHBp_old);
  /* p29.YGEM*/ wire _YGEM_STORE9_MATCHn_old = nand3(_AROR_MATCH_ENp_old, _YLEV_STORE9_MATCHAp_old, _YTUB_STORE9_MATCHBp_old);

  /* p29.WEFU*/ wire _WEFU_STORE0_MATCH_old = not1(_YDUG_STORE0_MATCHn_old);
  /* p29.GAJA*/ wire _GAJA_STORE1_MATCH_old = not1(_DYDU_STORE1_MATCHn_old);
  /* p29.GUPO*/ wire _GUPO_STORE2_MATCH_old = not1(_DEGO_STORE2_MATCHn_old);
  /* p29.WEBO*/ wire _WEBO_STORE3_MATCH_old = not1(_YLOZ_STORE3_MATCHn_old);
  /* p29.WUNA*/ wire _WUNA_STORE4_MATCH_old = not1(_XAGE_STORE4_MATCHn_old);
  /* p29.GABA*/ wire _GABA_STORE5_MATCH_old = not1(_EGOM_STORE5_MATCHn_old);
  /* p29.WASE*/ wire _WASE_STORE6_MATCH_old = not1(_YBEZ_STORE6_MATCHn_old);
  /* p29.GYTE*/ wire _GYTE_STORE7_MATCH_old = not1(_DYKA_STORE7_MATCHn_old);
  /* p29.GEKE*/ wire _GEKE_STORE8_MATCH_old = not1(_EFYL_STORE8_MATCHn_old);

  /* p29.GEZE*/ wire _GEZE_STORE0_MATCH_old = or2(_WEFU_STORE0_MATCH_old, GND);
  /* p29.FUMA*/ wire _FUMA_STORE1_MATCH_old = or2(_GAJA_STORE1_MATCH_old, _GEZE_STORE0_MATCH_old);
  /* p29.GEDE*/ wire _GEDE_STORE2_MATCH_old = or2(_GUPO_STORE2_MATCH_old, _FUMA_STORE1_MATCH_old);
  /* p29.WUTO*/ wire _WUTO_STORE3_MATCH_old = or2(_WEBO_STORE3_MATCH_old, _GEDE_STORE2_MATCH_old);
  /* p29.XYLA*/ wire _XYLA_STORE4_MATCH_old = or2(_WUNA_STORE4_MATCH_old, _WUTO_STORE3_MATCH_old);
  /* p29.WEJA*/ wire _WEJA_STORE5_MATCH_old = or2(_GABA_STORE5_MATCH_old, _XYLA_STORE4_MATCH_old);
  /* p29.WYLA*/ wire _WYLA_STORE6_MATCH_old = or2(_WASE_STORE6_MATCH_old, _WEJA_STORE5_MATCH_old);
  /* p29.FAVO*/ wire _FAVO_STORE7_MATCH_old = or2(_GYTE_STORE7_MATCH_old, _WYLA_STORE6_MATCH_old);
  /* p29.GYGA*/ wire _GYGA_STORE8_MATCH_old = or2(_GEKE_STORE8_MATCH_old, _FAVO_STORE7_MATCH_old);

  /* p29.GUVA*/ wire _GUVA_SPRITE0_GETp_old = nor2(_YDUG_STORE0_MATCHn_old, GND);
  /* p29.ENUT*/ wire _ENUT_SPRITE1_GETp_old = nor2(_DYDU_STORE1_MATCHn_old, _GEZE_STORE0_MATCH_old);
  /* p29.EMOL*/ wire _EMOL_SPRITE2_GETp_old = nor2(_DEGO_STORE2_MATCHn_old, _FUMA_STORE1_MATCH_old);
  /* p29.GYFY*/ wire _GYFY_SPRITE3_GETp_old = nor2(_YLOZ_STORE3_MATCHn_old, _GEDE_STORE2_MATCH_old);
  /* p29.GONO*/ wire _GONO_SPRITE4_GETp_old = nor2(_XAGE_STORE4_MATCHn_old, _WUTO_STORE3_MATCH_old);
  /* p29.GEGA*/ wire _GEGA_SPRITE5_GETp_old = nor2(_EGOM_STORE5_MATCHn_old, _XYLA_STORE4_MATCH_old);
  /* p29.XOJA*/ wire _XOJA_SPRITE6_GETp_old = nor2(_YBEZ_STORE6_MATCHn_old, _WEJA_STORE5_MATCH_old);
  /* p29.GUTU*/ wire _GUTU_SPRITE7_GETp_old = nor2(_DYKA_STORE7_MATCHn_old, _WYLA_STORE6_MATCH_old);
  /* p29.FOXA*/ wire _FOXA_SPRITE8_GETp_old = nor2(_EFYL_STORE8_MATCHn_old, _FAVO_STORE7_MATCH_old);
  /* p29.GUZE*/ wire _GUZE_SPRITE9_GETp_old = nor2(_YGEM_STORE9_MATCHn_old, _GYGA_STORE8_MATCH_old);

  /* p29.FURO*/ wire _FURO_SPRITE0_GETn_old = not1(_GUVA_SPRITE0_GETp_old);
  /* p29.DYDO*/ wire _DYDO_SPRITE1_GETn_old = not1(_ENUT_SPRITE1_GETp_old);
  /* p29.FAME*/ wire _FAME_SPRITE2_GETn_old = not1(_EMOL_SPRITE2_GETp_old);
  /* p29.GYMA*/ wire _GYMA_SPRITE3_GETn_old = not1(_GYFY_SPRITE3_GETp_old);
  /* p29.GOWO*/ wire _GOWO_SPRITE4_GETn_old = not1(_GONO_SPRITE4_GETp_old);
  /* p29.GYGY*/ wire _GYGY_SPRITE5_GETn_old = not1(_GEGA_SPRITE5_GETp_old);
  /* p29.XYME*/ wire _XYME_SPRITE6_GETn_old = not1(_XOJA_SPRITE6_GETp_old);
  /* p29.GUGY*/ wire _GUGY_SPRITE7_GETn_old = not1(_GUTU_SPRITE7_GETp_old);
  /* p29.DENY*/ wire _DENY_SPRITE8_GETn_old = not1(_FOXA_SPRITE8_GETp_old);
  /* p29.FADO*/ wire _FADO_SPRITE9_GETn_old = not1(_GUZE_SPRITE9_GETp_old);

  /* p29.FEFY*/ wire _FEFY_STORE_MATCHp_old = nand5(_XAGE_STORE4_MATCHn_old, _YLOZ_STORE3_MATCHn_old, _DEGO_STORE2_MATCHn_old, _DYDU_STORE1_MATCHn_old, _YDUG_STORE0_MATCHn_old);
  /* p29.FOVE*/ wire _FOVE_STORE_MATCHp_old = nand5(_YGEM_STORE9_MATCHn_old, _EFYL_STORE8_MATCHn_old, _DYKA_STORE7_MATCHn_old, _YBEZ_STORE6_MATCHn_old, _EGOM_STORE5_MATCHn_old);
  /* p29.FEPO*/ wire _FEPO_STORE_MATCHp_old = or2(_FEFY_STORE_MATCHp_old, _FOVE_STORE_MATCHp_old);
  /*#p21.XENA*/ wire _XENA_STORE_MATCHn_old = not1(_FEPO_STORE_MATCHp_old);

  /*#p21.XUGU*/ wire _XUGU_PX167n_old = nand5(pix_pipe.XEHO_PX0p.qp_old(), pix_pipe.SAVY_PX1p.qp_old(), pix_pipe.XODU_PX2p.qp_old(), pix_pipe.TUKY_PX5p.qp_old(), pix_pipe.SYBE_PX7p.qp_old()); // 128 + 32 + 4 + 2 + 1 = 167
  /*#p21.XANO*/ wire _XANO_PX167p_old = not1(_XUGU_PX167n_old);
  /*#p21.WODU*/ wire _WODU_HBLANKp_old = and2(_XENA_STORE_MATCHn_old, _XANO_PX167p_old);







































  /*#p27.SYLO*/ wire _SYLO_WIN_HITn_old = not1(pix_pipe.RYDY_WIN_HITp.qp_old());
  /*#p24.TOMU*/ wire _TOMU_WIN_HITp_old = not1(_SYLO_WIN_HITn_old);
  /*#p24.SOCY*/ wire _SOCY_WIN_HITn_old = not1(_TOMU_WIN_HITp_old);


  /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn_old      = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp_old());
  /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn_old = nand4(pix_pipe.XYMU_RENDERINGn.qn_old(), _ROMO_PRELOAD_DONEn_old, tile_fetcher.NYKA_FETCH_DONEp.qp_old(), tile_fetcher.PORY_FETCH_DONEp.qp_old());
  /* p27.TAVE*/ wire _TAVE_PRELOAD_DONE_TRIGp_old = not1(_SUVU_PRELOAD_DONE_TRIGn_old);
  /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp_old    = nor2(pix_pipe.RYFA_WIN_FETCHn_A.qn_old(), pix_pipe.RENE_WIN_FETCHn_B.qp_old());
  /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_old = nand2(_SYLO_WIN_HITn_old, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp_old());
  /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_old = not1(_TUXY_WIN_FIRST_TILEne_old);














  /* p21.TADY*/ wire _TADY_LINE_RSTn_new  = nor2(_ATEJ_LINE_TRIGp_new, _TOFU_VID_RSTp_new);
  /*#p21.VOGA*/ pix_pipe.VOGA_HBLANKp.dff17(_ALET_xBxDxFxH_clk, _TADY_LINE_RSTn_new, _WODU_HBLANKp_old);

  /*#p21.WEGO*/ wire _WEGO_HBLANKp_new = or2(_TOFU_VID_RSTp_new, pix_pipe.VOGA_HBLANKp.qp_new());
  /*#p21.XYMU*/ pix_pipe.XYMU_RENDERINGn.nor_latch(_WEGO_HBLANKp_new, _AVAP_SCAN_DONE_TRIGp_new);
  /*#p27.PAHA*/ wire _PAHA_RENDERINGn_new = not1(pix_pipe.XYMU_RENDERINGn.qn_new());




  /* p27.NUNU*/ pix_pipe.NUNU_WX_MATCH_Bp.dff17(_MEHE_AxCxExGx_clk, _XAPO_VID_RSTn_new, pix_pipe.PYCO_WX_MATCH_Ap.qp_old());
  /* p27.XAHY*/ wire _XAHY_LINE_TRIGn_new = not1(_ATEJ_LINE_TRIGp_new);
  /*#p27.XOFO*/ wire _XOFO_WIN_RSTp_new   = nand3(pix_pipe.WYMO_LCDC_WINENn.qn_new(), _XAHY_LINE_TRIGn_new, _XAPO_VID_RSTn_new);


  /*#p27.NUNY*/ wire _NUNY_WX_MATCH_TRIGp_old = and2(pix_pipe.PYNU_WIN_MODE_Ap.qp_old(), pix_pipe.NOPA_WIN_MODE_Bp.qn_old());
  /* p27.NYFO*/ wire _NYFO_WX_MATCH_TRIGn_old = not1(_NUNY_WX_MATCH_TRIGp_old);
  /* p27.MOSU*/ wire _MOSU_WX_MATCH_TRIGp_old = not1(_NYFO_WX_MATCH_TRIGn_old);


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
    /* p27.RYDY*/ pix_pipe.RYDY_WIN_HITp = nor3(pix_pipe.PUKU_WIN_HITn.qp_old(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), _PYRY_VID_RSTp_new);
    /* p27.PUKU*/ pix_pipe.PUKU_WIN_HITn = nor2(pix_pipe.RYDY_WIN_HITp.qp_new(), _NUNY_WX_MATCH_TRIGp_new);
    /* p27.RYDY*/ pix_pipe.RYDY_WIN_HITp = nor3(pix_pipe.PUKU_WIN_HITn.qp_new(), tile_fetcher.PORY_FETCH_DONEp.qp_new(), _PYRY_VID_RSTp_new);
  }

  /* p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.qp_new(), _LOBY_RENDERINGn_new);




  /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn_new      = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp_new());

  // FIXME old/new
  /* p27.SUVU*/ bool _SUVU_PRELOAD_DONE_TRIGn_xxx = nand4(pix_pipe.XYMU_RENDERINGn.qn_new(), _ROMO_PRELOAD_DONEn_new, tile_fetcher.NYKA_FETCH_DONEp.qp_old(), tile_fetcher.PORY_FETCH_DONEp.qp_new());
  /* p27.TAVE*/ bool _TAVE_PRELOAD_DONE_TRIGp_xxx = not1(_SUVU_PRELOAD_DONE_TRIGn_xxx);

  /*#p27.SYLO*/ wire _SYLO_WIN_HITn_new = not1(pix_pipe.RYDY_WIN_HITp.qp_new());
  /*#p24.TOMU*/ wire _TOMU_WIN_HITp_new = not1(_SYLO_WIN_HITn_new);
  /*#p24.SOCY*/ wire _SOCY_WIN_HITn_new = not1(_TOMU_WIN_HITp_new);

  // FIXME old/new
  /*#p24.VYBO*/ wire _VYBO_CLKPIPE_xBxDxFxH = nor3(_FEPO_STORE_MATCHp_old, _WODU_HBLANKp_old, _MYVO_AxCxExGx_clk);
  /*#p24.TYFA*/ wire _TYFA_CLKPIPE_xBxDxFxH = and3(_SOCY_WIN_HITn_new, tile_fetcher.POKY_PRELOAD_LATCHp.qp_new(), _VYBO_CLKPIPE_xBxDxFxH);
  /*#p24.SEGU*/ wire _SEGU_CLKPIPE_AxCxExGx = not1(_TYFA_CLKPIPE_xBxDxFxH);
  /* p27.ROCO*/ wire _ROCO_CLKPIPE_xBxDxFxH = not1(_SEGU_CLKPIPE_AxCxExGx);
  /* p27.PYCO*/ pix_pipe.PYCO_WX_MATCH_Ap.dff17(_ROCO_CLKPIPE_xBxDxFxH, _XAPO_VID_RSTn_new, _NUKO_WX_MATCHp_old);

  /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_new = nand2(_SYLO_WIN_HITn_new, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp_new());
  /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_new = not1(_TUXY_WIN_FIRST_TILEne_new);

  // FIXME old/new
  /* p27.TEVO*/ bool _TEVO_FETCH_TRIGp = or3(_SEKO_WIN_FETCH_TRIGp_old, _SUZU_WIN_FIRST_TILEne_new, _TAVE_PRELOAD_DONE_TRIGp_xxx); // Schematic wrong, this is OR



  // If we use the "new" values of FINE_CNT here, things break because the signal is then "newer" than the clock to RYFA
  /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n_old = nand3(pix_pipe.RUBU_FINE_CNT2.qp_old(), pix_pipe.ROGA_FINE_CNT1.qp_old(), pix_pipe.RYKU_FINE_CNT0.qp_old());
  /* p27.PANY*/ wire _PANY_WIN_FETCHn_old = nor2(_NUKO_WX_MATCHp_old, _ROZE_FINE_COUNT_7n_old);


  /* p27.RYFA*/ pix_pipe.RYFA_WIN_FETCHn_A.dff17(_SEGU_CLKPIPE_AxCxExGx, pix_pipe.XYMU_RENDERINGn.qn_new(), _PANY_WIN_FETCHn_old);
  /* p27.RENE*/ pix_pipe.RENE_WIN_FETCHn_B.dff17(_ALET_xBxDxFxH_clk,       pix_pipe.XYMU_RENDERINGn.qn_new(), pix_pipe.RYFA_WIN_FETCHn_A.qp_new());
  /* p27.SEKO*/ wire _SEKO_WIN_FETCH_TRIGp_new = nor2(pix_pipe.RYFA_WIN_FETCHn_A.qn_new(), pix_pipe.RENE_WIN_FETCHn_B.qp_new());




  /* p27.NYXU*/ bool _NYXU_FETCH_TRIGn = nor3(_AVAP_SCAN_DONE_TRIGp_new, _MOSU_WX_MATCH_TRIGp_new, _TEVO_FETCH_TRIGp);
  /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn = nand3(tile_fetcher.LAXU_BFETCH_S0p.qp(), tile_fetcher.NYVA_BFETCH_S2p.qp(), _NYXU_FETCH_TRIGn);
  /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp = not1(_MOCE_BFETCH_DONEn);
  /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17(_ALET_xBxDxFxH_clk, _NAFY_RENDERING_AND_NOT_WIN_TRIG_new, _LYRY_BFETCH_DONEp);
  /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn = nand4(pix_pipe.XYMU_RENDERINGn.qn(), _ROMO_PRELOAD_DONEn_new, tile_fetcher.NYKA_FETCH_DONEp.qp_new(), tile_fetcher.PORY_FETCH_DONEp.qp_new());
  /* p27.TAVE*/ wire _TAVE_PRELOAD_DONE_TRIGp = not1(_SUVU_PRELOAD_DONE_TRIGn);
  /* p27.TEVO*/ _TEVO_FETCH_TRIGp = or3(_SEKO_WIN_FETCH_TRIGp_new, _SUZU_WIN_FIRST_TILEne_new, _TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  /* p27.NYXU*/ _NYXU_FETCH_TRIGn = nor3(_AVAP_SCAN_DONE_TRIGp_new, _MOSU_WX_MATCH_TRIGp_new, _TEVO_FETCH_TRIGp);




  {
    /* p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(_ALET_xBxDxFxH_clk, _MOCE_BFETCH_DONEn);
    /* p27.LAXU*/ tile_fetcher.LAXU_BFETCH_S0p   .dff17(_LEBO_AxCxExGx,                    _NYXU_FETCH_TRIGn,             tile_fetcher.LAXU_BFETCH_S0p.qn_old());
    /* p27.MESU*/ tile_fetcher.MESU_BFETCH_S1p   .dff17(tile_fetcher.LAXU_BFETCH_S0p.qn(), _NYXU_FETCH_TRIGn,             tile_fetcher.MESU_BFETCH_S1p.qn_old());
    /* p27.NYVA*/ tile_fetcher.NYVA_BFETCH_S2p   .dff17(tile_fetcher.MESU_BFETCH_S1p.qn(), _NYXU_FETCH_TRIGn,             tile_fetcher.NYVA_BFETCH_S2p.qn_old());
    /* p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0p_D1.dff17(_ALET_xBxDxFxH_clk,                  pix_pipe.XYMU_RENDERINGn.qn(), tile_fetcher.LAXU_BFETCH_S0p.qp_new());
  }



























  /*#p24.ROXO*/ wire _ROXO_CLKPIPE_xBxDxFxH = not1(_SEGU_CLKPIPE_AxCxExGx);
  {
    /*#p27.PECU*/ wire _PECU_FINE_CLK_AxCxExGx = nand2(_ROXO_CLKPIPE_xBxDxFxH, _ROZE_FINE_COUNT_7n_old);
    /*#p27.PASO*/ wire _PASO_FINE_RST = nor2(_PAHA_RENDERINGn_new, _TEVO_FETCH_TRIGp);
    /*#p27.RYKU*/ pix_pipe.RYKU_FINE_CNT0.dff17(_PECU_FINE_CLK_AxCxExGx,      _PASO_FINE_RST, pix_pipe.RYKU_FINE_CNT0.qn_old());
    /*#p27.ROGA*/ pix_pipe.ROGA_FINE_CNT1.dff17(pix_pipe.RYKU_FINE_CNT0.qn(), _PASO_FINE_RST, pix_pipe.ROGA_FINE_CNT1.qn_old());
    /*#p27.RUBU*/ pix_pipe.RUBU_FINE_CNT2.dff17(pix_pipe.ROGA_FINE_CNT1.qn(), _PASO_FINE_RST, pix_pipe.RUBU_FINE_CNT2.qn_old());

    /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp = xnor2(pix_pipe.DATY_SCX0n_s.qn_new(), pix_pipe.RYKU_FINE_CNT0.qp_new());
    /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp = xnor2(pix_pipe.DUZU_SCX1n_s.qn_new(), pix_pipe.ROGA_FINE_CNT1.qp_new());
    /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp = xnor2(pix_pipe.CYXU_SCX2n_s.qn_new(), pix_pipe.RUBU_FINE_CNT2.qp_new());
    /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn = nand4(pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.qp(), _SUHA_SCX_FINE_MATCHp, _SYBY_SCX_FINE_MATCHp, _SOZU_SCX_FINE_MATCHp);
    /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp = not1(_RONE_SCX_FINE_MATCHn);
    /*#p27.PUXA*/ pix_pipe.PUXA_SCX_FINE_MATCH_A.dff17(_ROXO_CLKPIPE_xBxDxFxH, pix_pipe.XYMU_RENDERINGn.qn(), _POHU_SCX_FINE_MATCHp);
    /*#p27.NYZE*/ pix_pipe.NYZE_SCX_FINE_MATCH_B.dff17(_MOXE_AxCxExGx_clk,       pix_pipe.XYMU_RENDERINGn.qn(), pix_pipe.PUXA_SCX_FINE_MATCH_A.qp());

    /*#p27.POVA*/ wire _POVA_FINE_MATCH_TRIGp_new = and2(pix_pipe.PUXA_SCX_FINE_MATCH_A.qp_new(), pix_pipe.NYZE_SCX_FINE_MATCH_B.qn_new());
    /*#p27.ROXY*/ pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.nor_latch(_PAHA_RENDERINGn_new, _POVA_FINE_MATCH_TRIGp_new);
  }

  /*#p24.SACU*/ wire _SACU_CLKPIPE_AxCxExGx = or2(_SEGU_CLKPIPE_AxCxExGx, pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.qp_new()); // Schematic wrong, this is OR
  {
    // Pixel counter, has carry lookahead because this can increment every tcycle
    /* p21.RYBO*/ wire _RYBO = xor2(pix_pipe.XEHO_PX0p.qp_old(), pix_pipe.SAVY_PX1p.qp_old()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /* p21.XUKE*/ wire _XUKE = and2(pix_pipe.XEHO_PX0p.qp_old(), pix_pipe.SAVY_PX1p.qp_old());
    /* p21.XYLE*/ wire _XYLE = and2(pix_pipe.XODU_PX2p.qp_old(), _XUKE);
    /* p21.XEGY*/ wire _XEGY = xor2(pix_pipe.XODU_PX2p.qp_old(), _XUKE); // feet facing gnd
    /* p21.XORA*/ wire _XORA = xor2(pix_pipe.XYDO_PX3p.qp_old(), _XYLE); // feet facing gnd

    /* p21.XEHO*/ pix_pipe.XEHO_PX0p.dff17(_SACU_CLKPIPE_AxCxExGx, _TADY_LINE_RSTn_new, pix_pipe.XEHO_PX0p.qn_old());
    /* p21.SAVY*/ pix_pipe.SAVY_PX1p.dff17(_SACU_CLKPIPE_AxCxExGx, _TADY_LINE_RSTn_new, _RYBO);
    /* p21.XODU*/ pix_pipe.XODU_PX2p.dff17(_SACU_CLKPIPE_AxCxExGx, _TADY_LINE_RSTn_new, _XEGY);
    /* p21.XYDO*/ pix_pipe.XYDO_PX3p.dff17(_SACU_CLKPIPE_AxCxExGx, _TADY_LINE_RSTn_new, _XORA);

    /* p24.TOCA*/ wire _TOCA = not1(pix_pipe.XYDO_PX3p.qp_new());
    /* p21.SAKE*/ wire _SAKE = xor2(pix_pipe.TUHU_PX4p.qp_old(), pix_pipe.TUKY_PX5p.qp_old());
    /* p21.TYBA*/ wire _TYBA = and2(pix_pipe.TUHU_PX4p.qp_old(), pix_pipe.TUKY_PX5p.qp_old());
    /* p21.SURY*/ wire _SURY = and2(pix_pipe.TAKO_PX6p.qp_old(), _TYBA);
    /* p21.TYGE*/ wire _TYGE = xor2(pix_pipe.TAKO_PX6p.qp_old(), _TYBA);
    /* p21.ROKU*/ wire _ROKU = xor2(pix_pipe.SYBE_PX7p.qp_old(), _SURY);

    /* p21.TUHU*/ pix_pipe.TUHU_PX4p.dff17(_TOCA, _TADY_LINE_RSTn_new, pix_pipe.TUHU_PX4p.qn_old());
    /* p21.TUKY*/ pix_pipe.TUKY_PX5p.dff17(_TOCA, _TADY_LINE_RSTn_new, _SAKE);
    /* p21.TAKO*/ pix_pipe.TAKO_PX6p.dff17(_TOCA, _TADY_LINE_RSTn_new, _TYGE);
    /* p21.SYBE*/ pix_pipe.SYBE_PX7p.dff17(_TOCA, _TADY_LINE_RSTn_new, _ROKU);
  }

  /*#p21.XUGU*/ wire _XUGU_PX167n_new = nand5(pix_pipe.XEHO_PX0p.qp_new(), pix_pipe.SAVY_PX1p.qp_new(), pix_pipe.XODU_PX2p.qp_new(), pix_pipe.TUKY_PX5p.qp_new(), pix_pipe.SYBE_PX7p.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
  /*#p21.XANO*/ wire _XANO_PX167p_new = not1(_XUGU_PX167n_new);
  /*#p21.WODU*/ wire _WODU_HBLANKp_new = and2(_XENA_STORE_MATCHn_old, _XANO_PX167p_new);


  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

























































































































  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // THE LINE OF DESPAIR AND ANGUISH
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------































































































































































































  /* p29.WUTY*/ bool _WUTY_SPRITE_DONEp;
  {
    /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1.dff17(_LAPE_AxCxExGx_clk, _VYPO_VCC,                     sprite_fetcher.TOXE_SFETCH_S0p.qp());
    /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5.dff17(_LAPE_AxCxExGx_clk, pix_pipe.XYMU_RENDERINGn.qn(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp());
    /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4.dff17(_TAVA_xBxDxFxH_clk, pix_pipe.XYMU_RENDERINGn.qn(), sprite_fetcher.TOBU_SFETCH_S1p_D2.qp());
    /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2.dff17(_TAVA_xBxDxFxH_clk, pix_pipe.XYMU_RENDERINGn.qn(), sprite_fetcher.TULY_SFETCH_S1p.qp());

    /* p29.TYNO*/ wire _TYNO = nand3(sprite_fetcher.TOXE_SFETCH_S0p.qp(), sprite_fetcher.SEBA_SFETCH_S1p_D5.qp(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp());
    /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn  = or2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qn(), _TYNO);
    /* p29.WUTY*/ _WUTY_SPRITE_DONEp = not1(_VUSA_SPRITE_DONEn);

    /* p27.SOWO*/ wire _SOWO_SFETCH_RUNNINGn = not1(sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp());
    /* p27.TUKU*/ wire _TUKU_WIN_HITn = not1(_TOMU_WIN_HITp_new);
    /* p27.TEKY*/ wire _TEKY_SFETCH_REQp = and4(_FEPO_STORE_MATCHp_old, _TUKU_WIN_HITn, _LYRY_BFETCH_DONEp, _SOWO_SFETCH_RUNNINGn);
    /* p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp.dff17(_TAVA_xBxDxFxH_clk, _VYPO_VCC, _TEKY_SFETCH_REQp);
    /* p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp.dff17(_LAPE_AxCxExGx_clk, _VYPO_VCC, sprite_fetcher.SOBU_SFETCH_REQp.qp());

    /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp(), sprite_fetcher.SUDA_SFETCH_REQp.qn());
    /*#p27.SECA*/ wire _SECA_SFETCH_RUNNING_SETn = nor3(_RYCE_SFETCH_TRIGp, _ROSY_VID_RSTp_new, _ATEJ_LINE_TRIGp_new);
    /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor2(_WUTY_SPRITE_DONEp, _TAVE_PRELOAD_DONE_TRIGp); // def nor
    /* p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RUNNING_SETn, _VEKU_SFETCH_RUNNING_RSTn);

    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.RSTn(_SECA_SFETCH_RUNNING_SETn);
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p.RSTn(_SECA_SFETCH_RUNNING_SETn);
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p.RSTn(_SECA_SFETCH_RUNNING_SETn);

    /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(sprite_fetcher.TESE_SFETCH_S2p.qp(), sprite_fetcher.TOXE_SFETCH_S0p.qp());
    /*#p29.TOMA*/ wire _TOMA_SFETCH_CLK_xBxDxFxH = nand2(_LAPE_AxCxExGx_clk, _TAME_SFETCH_CLK_GATE);
    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.dff17(_TOMA_SFETCH_CLK_xBxDxFxH,           _SECA_SFETCH_RUNNING_SETn, sprite_fetcher.TOXE_SFETCH_S0p.qn());
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p.dff17(sprite_fetcher.TOXE_SFETCH_S0p.qn(), _SECA_SFETCH_RUNNING_SETn, sprite_fetcher.TULY_SFETCH_S1p.qn());
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p.dff17(sprite_fetcher.TULY_SFETCH_S1p.qn(), _SECA_SFETCH_RUNNING_SETn, sprite_fetcher.TESE_SFETCH_S2p.qn());
  }

  /*#p28.ACYL*/ wire _ACYL_SCANNINGp = and2(_BOGE_DMA_RUNNINGn_new, sprite_scanner.BESU_SCANNINGp.qp());
  /* p28.AJON*/ wire _AJON_PPU_OAM_ENp = and2(_BOGE_DMA_RUNNINGn_new, pix_pipe.XYMU_RENDERINGn.qn()); // def AND. ppu can read oam when there's rendering but no dma
  /* p04.DUGA*/ wire _DUGA_DMA_OAM_RDn  = not1(dma_reg.MATU_DMA_RUNNINGp.qp());
  /* p28.ASAM*/ wire _ASAM_CPU_OAM_RDn  = or3(_ACYL_SCANNINGp, pix_pipe.XYMU_RENDERINGn.qn(), dma_reg.MATU_DMA_RUNNINGp.qp());
  /* p28.BETE*/ wire _BETE_PPU_OAM_RDn  = not1(_AJON_PPU_OAM_ENp);



  Bus2 SPR_TRI_I_in[6]; // from sprite store
  Bus2 SPR_TRI_L_in[4]; // from sprite store
  Bus2 BUS_OAM_An[8];
  uint8_t oam_addr;

  {
    /* p30.ZETU*/ SPR_TRI_I_in[0].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.YGUS_STORE0_I0n.qp());
    /* p30.ZECE*/ SPR_TRI_I_in[1].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.YSOK_STORE0_I1n.qp());
    /* p30.ZAVE*/ SPR_TRI_I_in[2].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.YZEP_STORE0_I2n.qp());
    /* p30.WOKO*/ SPR_TRI_I_in[3].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.WYTE_STORE0_I3n.qp());
    /* p30.ZUMU*/ SPR_TRI_I_in[4].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.ZONY_STORE0_I4n.qp());
    /*#p30.ZEDY*/ SPR_TRI_I_in[5].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.YWAK_STORE0_I5n.qp());
    /* p30.WEHE*/ SPR_TRI_L_in[0].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.GYHO_STORE0_L0n.qp());
    /* p30.BUKY*/ SPR_TRI_L_in[1].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.CUFO_STORE0_L1n.qp());
    /* p30.AJAL*/ SPR_TRI_L_in[2].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.BOZU_STORE0_L2n.qp());
    /* p30.GOFO*/ SPR_TRI_L_in[3].tri6_nn(_FURO_SPRITE0_GETn_old, sprite_store.FYHY_STORE0_L3n.qp());

    /*#p30.CUBO*/ SPR_TRI_I_in[0].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.CADU_STORE1_I0n.qp());
    /* p30.CELU*/ SPR_TRI_I_in[1].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.CEBO_STORE1_I1n.qp());
    /* p30.CEGY*/ SPR_TRI_I_in[2].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.CUFA_STORE1_I2n.qp());
    /* p30.BETY*/ SPR_TRI_I_in[3].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.COMA_STORE1_I3n.qp());
    /* p30.CYBY*/ SPR_TRI_I_in[4].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.CUZA_STORE1_I4n.qp());
    /* p30.BEMO*/ SPR_TRI_I_in[5].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.CAJY_STORE1_I5n.qp());
    /* p30.BYRO*/ SPR_TRI_L_in[0].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.AMES_STORE1_L0n.qp());
    /* p30.AHUM*/ SPR_TRI_L_in[1].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.AROF_STORE1_L1n.qp());
    /* p30.BACO*/ SPR_TRI_L_in[2].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.ABOP_STORE1_L2n.qp());
    /* p30.BEFE*/ SPR_TRI_L_in[3].tri6_nn(_DYDO_SPRITE1_GETn_old, sprite_store.ABUG_STORE1_L3n.qp());

    /* p30.CUBE*/ SPR_TRI_I_in[0].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.BUHE_STORE2_I0n.qp());
    /* p30.AFOZ*/ SPR_TRI_I_in[1].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.BYHU_STORE2_I1n.qp());
    /* p30.APON*/ SPR_TRI_I_in[2].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.BECA_STORE2_I2n.qp());
    /* p30.CUVU*/ SPR_TRI_I_in[3].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.BULU_STORE2_I3n.qp());
    /* p30.CYRO*/ SPR_TRI_I_in[4].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.BUNA_STORE2_I4n.qp());
    /* p30.AXEC*/ SPR_TRI_I_in[5].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.BOXA_STORE2_I5n.qp());
    /* p30.ZUKE*/ SPR_TRI_L_in[0].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.YLOV_STORE2_L0n.qp());
    /* p30.WERE*/ SPR_TRI_L_in[1].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.XOSY_STORE2_L1n.qp());
    /* p30.WUXE*/ SPR_TRI_L_in[2].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.XAZY_STORE2_L2n.qp());
    /* p30.ZABY*/ SPR_TRI_L_in[3].tri6_nn(_FAME_SPRITE2_GETn_old, sprite_store.YKUK_STORE2_L3n.qp());

    /* p30.ENAP*/ SPR_TRI_I_in[0].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.DEVY_STORE3_I0n.qp());
    /* p30.DYGO*/ SPR_TRI_I_in[1].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.DESE_STORE3_I1n.qp());
    /* p30.DOWA*/ SPR_TRI_I_in[2].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.DUNY_STORE3_I2n.qp());
    /* p30.DONY*/ SPR_TRI_I_in[3].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.DUHA_STORE3_I3n.qp());
    /* p30.EFUD*/ SPR_TRI_I_in[4].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.DEBA_STORE3_I4n.qp());
    /* p30.DEZU*/ SPR_TRI_I_in[5].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.DAFU_STORE3_I5n.qp());
    /* p30.ZEXE*/ SPR_TRI_L_in[0].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.ZURO_STORE3_L0n.qp());
    /* p30.YWAV*/ SPR_TRI_L_in[1].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.ZYLU_STORE3_L1n.qp());
    /* p30.YJEM*/ SPR_TRI_L_in[2].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.ZENE_STORE3_L2n.qp());
    /* p30.ZYPO*/ SPR_TRI_L_in[3].tri6_nn(_GYMA_SPRITE3_GETn_old, sprite_store.ZURY_STORE3_L3n.qp());

    /* p30.WUXU*/ SPR_TRI_I_in[0].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.XAVE_STORE4_I0n.qp());
    /* p30.WEPY*/ SPR_TRI_I_in[1].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.XEFE_STORE4_I1n.qp());
    /* p30.WERU*/ SPR_TRI_I_in[2].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.WANU_STORE4_I2n.qp());
    /* p30.XYRE*/ SPR_TRI_I_in[3].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.XABO_STORE4_I3n.qp());
    /* p30.WOXY*/ SPR_TRI_I_in[4].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.XEGE_STORE4_I4n.qp());
    /* p30.WAJA*/ SPR_TRI_I_in[5].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.XYNU_STORE4_I5n.qp());
    /* p30.BUCE*/ SPR_TRI_L_in[0].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.CAPO_STORE4_L0n.qp());
    /* p30.BEVY*/ SPR_TRI_L_in[1].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.CAJU_STORE4_L1n.qp());
    /* p30.BOVE*/ SPR_TRI_L_in[2].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.CONO_STORE4_L2n.qp());
    /* p30.BYDO*/ SPR_TRI_L_in[3].tri6_nn(_GOWO_SPRITE4_GETn_old, sprite_store.CUMU_STORE4_L3n.qp());

    /* p30.DOBO*/ SPR_TRI_I_in[0].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.EKOP_STORE5_I0n.qp());
    /* p30.DYNY*/ SPR_TRI_I_in[1].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.ETYM_STORE5_I1n.qp());
    /* p30.WAGA*/ SPR_TRI_I_in[2].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.GORU_STORE5_I2n.qp());
    /* p30.DUZA*/ SPR_TRI_I_in[3].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.EBEX_STORE5_I3n.qp());
    /* p30.DALY*/ SPR_TRI_I_in[4].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.ETAV_STORE5_I4n.qp());
    /* p30.DALO*/ SPR_TRI_I_in[5].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.EKAP_STORE5_I5n.qp());
    /* p30.BACE*/ SPR_TRI_L_in[0].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.ACEP_STORE5_L0n.qp());
    /* p30.BUJA*/ SPR_TRI_L_in[1].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.ABEG_STORE5_L1n.qp());
    /* p30.BODU*/ SPR_TRI_L_in[2].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.ABUX_STORE5_L2n.qp());
    /* p30.AWAT*/ SPR_TRI_L_in[3].tri6_nn(_GYGY_SPRITE5_GETn_old, sprite_store.ANED_STORE5_L3n.qp());

    /* p30.WATO*/ SPR_TRI_I_in[0].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.GABO_STORE6_I0n.qp());
    /* p30.WYWY*/ SPR_TRI_I_in[1].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.GACY_STORE6_I1n.qp());
    /* p30.EZOC*/ SPR_TRI_I_in[2].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.FOGO_STORE6_I2n.qp());
    /* p30.WABO*/ SPR_TRI_I_in[3].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.GOHU_STORE6_I3n.qp());
    /* p30.ELYC*/ SPR_TRI_I_in[4].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.FOXY_STORE6_I4n.qp());
    /* p30.WOCY*/ SPR_TRI_I_in[5].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.GECU_STORE6_I5n.qp());
    /* p30.YBUK*/ SPR_TRI_L_in[0].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.ZUMY_STORE6_L0n.qp());
    /* p30.YKOZ*/ SPR_TRI_L_in[1].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.ZAFU_STORE6_L1n.qp());
    /* p30.ZYTO*/ SPR_TRI_L_in[2].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.ZEXO_STORE6_L2n.qp());
    /* p30.ZUDO*/ SPR_TRI_L_in[3].tri6_nn(_XYME_SPRITE6_GETn_old, sprite_store.ZUBE_STORE6_L3n.qp());

    /* p30.WAKO*/ SPR_TRI_I_in[0].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.GULE_STORE7_I0n.qp());
    /* p30.WYGO*/ SPR_TRI_I_in[1].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.GYNO_STORE7_I1n.qp());
    /* p30.ELEP*/ SPR_TRI_I_in[2].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.FEFA_STORE7_I2n.qp());
    /* p30.ETAD*/ SPR_TRI_I_in[3].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.FYSU_STORE7_I3n.qp());
    /* p30.WABA*/ SPR_TRI_I_in[4].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.GESY_STORE7_I4n.qp());
    /* p30.EVYT*/ SPR_TRI_I_in[5].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.FUZO_STORE7_I5n.qp());
    /* p30.WAXE*/ SPR_TRI_L_in[0].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.XYNA_STORE7_L0n.qp());
    /* p30.YPOZ*/ SPR_TRI_L_in[1].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.YGUM_STORE7_L1n.qp());
    /* p30.WABU*/ SPR_TRI_L_in[2].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.XAKU_STORE7_L2n.qp());
    /* p30.WANA*/ SPR_TRI_L_in[3].tri6_nn(_GUGY_SPRITE7_GETn_old, sprite_store.XYGO_STORE7_L3n.qp());

    /* p30.APOC*/ SPR_TRI_I_in[0].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.AXUV_STORE8_I0n.qp());
    /* p30.AKYH*/ SPR_TRI_I_in[1].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.BADA_STORE8_I1n.qp());
    /* p30.AFEN*/ SPR_TRI_I_in[2].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.APEV_STORE8_I2n.qp());
    /* p30.APYV*/ SPR_TRI_I_in[3].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.BADO_STORE8_I3n.qp());
    /* p30.APOB*/ SPR_TRI_I_in[4].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.BEXY_STORE8_I4n.qp());
    /* p30.ADYB*/ SPR_TRI_I_in[5].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.BYHE_STORE8_I5n.qp());
    /* p30.BOSO*/ SPR_TRI_L_in[0].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.AZAP_STORE8_L0n.qp());
    /* p30.BAZU*/ SPR_TRI_L_in[1].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.AFYX_STORE8_L1n.qp());
    /* p30.AHAC*/ SPR_TRI_L_in[2].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.AFUT_STORE8_L2n.qp());
    /* p30.BUJY*/ SPR_TRI_L_in[3].tri6_nn(_DENY_SPRITE8_GETn_old, sprite_store.AFYM_STORE8_L3n.qp());

    /* p30.ZARO*/ SPR_TRI_I_in[0].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.YBER_STORE9_I0n.qp());
    /* p30.ZOJY*/ SPR_TRI_I_in[1].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.YZOR_STORE9_I1n.qp());
    /* p30.YNEV*/ SPR_TRI_I_in[2].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.XYFE_STORE9_I2n.qp());
    /* p30.XYRA*/ SPR_TRI_I_in[3].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.XOTU_STORE9_I3n.qp());
    /* p30.YRAD*/ SPR_TRI_I_in[4].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.XUTE_STORE9_I4n.qp());
    /* p30.YHAL*/ SPR_TRI_I_in[5].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.XUFO_STORE9_I5n.qp());
    /* p30.BYME*/ SPR_TRI_L_in[0].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.CANA_STORE9_L0n.qp());
    /* p30.GATE*/ SPR_TRI_L_in[1].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.FOFO_STORE9_L1n.qp());
    /* p30.COHO*/ SPR_TRI_L_in[2].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.DYSY_STORE9_L2n.qp());
    /* p30.CAWO*/ SPR_TRI_L_in[3].tri6_nn(_FADO_SPRITE9_GETn_old, sprite_store.DEWU_STORE9_L3n.qp());

    /* SCAN  -> OBA */
    /* p28.APAR*/ wire _APAR_SCANNINGn = not1(_ACYL_SCANNINGp);
    /* p28.GEFY*/ BUS_OAM_An[0].tri6_nn(_APAR_SCANNINGn, GND);
    /* p28.WUWE*/ BUS_OAM_An[1].tri6_nn(_APAR_SCANNINGn, GND);
    /* p28.GUSE*/ BUS_OAM_An[2].tri6_nn(_APAR_SCANNINGn, _YFEL_SCAN0_old); // _MUST_ be qp17_old, but WHYYYY. probably related to oam clock or something.
    /* p28.GEMA*/ BUS_OAM_An[3].tri6_nn(_APAR_SCANNINGn, _WEWY_SCAN1_old);
    /* p28.FUTO*/ BUS_OAM_An[4].tri6_nn(_APAR_SCANNINGn, _GOSO_SCAN2_old);
    /* p28.FAKU*/ BUS_OAM_An[5].tri6_nn(_APAR_SCANNINGn, _ELYN_SCAN3_old);
    /* p28.GAMA*/ BUS_OAM_An[6].tri6_nn(_APAR_SCANNINGn, _FAHA_SCAN4_old);
    /* p28.GOBY*/ BUS_OAM_An[7].tri6_nn(_APAR_SCANNINGn, _FONY_SCAN5_old);

    /* DBA   -> OBA */
    /* p28.FODO*/ BUS_OAM_An[0].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.NAKY_DMA_A00p.qp());
    /* p28.FESA*/ BUS_OAM_An[1].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.PYRO_DMA_A01p.qp());
    /* p28.FAGO*/ BUS_OAM_An[2].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.NEFY_DMA_A02p.qp());
    /* p28.FYKY*/ BUS_OAM_An[3].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.MUTY_DMA_A03p.qp());
    /* p28.ELUG*/ BUS_OAM_An[4].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.NYKO_DMA_A04p.qp());
    /* p28.EDOL*/ BUS_OAM_An[5].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.PYLO_DMA_A05p.qp());
    /* p28.FYDU*/ BUS_OAM_An[6].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.NUTO_DMA_A06p.qp());
    /* p28.FETU*/ BUS_OAM_An[7].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.MUGU_DMA_A07p.qp());

    /* CBA   -> OBA */
    /* p28.GARO*/ BUS_OAM_An[0].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A[ 0]);
    /* p28.WACU*/ BUS_OAM_An[1].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A[ 1]);
    /* p28.GOSE*/ BUS_OAM_An[2].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A[ 2]);
    /* p28.WAPE*/ BUS_OAM_An[3].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A[ 3]);
    /* p28.FEVU*/ BUS_OAM_An[4].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A[ 4]);
    /* p28.GERA*/ BUS_OAM_An[5].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A[ 5]);
    /* p28.WAXA*/ BUS_OAM_An[6].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A[ 6]);
    /* p28.FOBY*/ BUS_OAM_An[7].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A[ 7]);

    /* SPR_I -> OBA */
    /* p28.GECA*/ BUS_OAM_An[0].tri6_nn(_BETE_PPU_OAM_RDn, _WEFE_VCC);
    /* p28.WYDU*/ BUS_OAM_An[1].tri6_nn(_BETE_PPU_OAM_RDn, _WEFE_VCC);
    /* p28.GYBU*/ BUS_OAM_An[2].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I_in[0].qp());
    /* p28.GYKA*/ BUS_OAM_An[3].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I_in[1].qp());
    /* p28.FABY*/ BUS_OAM_An[4].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I_in[2].qp());
    /* p28.FACO*/ BUS_OAM_An[5].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I_in[3].qp());
    /* p28.FUGU*/ BUS_OAM_An[6].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I_in[4].qp());
    /* p28.FYKE*/ BUS_OAM_An[7].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I_in[5].qp());

    /* p28.YFOT*/ wire _YFOT_OAM_A2p = not1(BUS_OAM_An[2].qp());
    /* p28.YFOC*/ wire _YFOC_OAM_A3p = not1(BUS_OAM_An[3].qp());
    /* p28.YVOM*/ wire _YVOM_OAM_A4p = not1(BUS_OAM_An[4].qp());
    /* p28.YMEV*/ wire _YMEV_OAM_A5p = not1(BUS_OAM_An[5].qp());
    /* p28.XEMU*/ wire _XEMU_OAM_A6p = not1(BUS_OAM_An[6].qp());
    /* p28.YZET*/ wire _YZET_OAM_A7p = not1(BUS_OAM_An[7].qp());

    /*#p30.CYKE*/ wire _CYKE_ABxxEFxx = not1(_XUPY_ABxxEFxx_new);
    /*#p30.WUDA*/ wire _WUDA_xxCDxxGH = not1(_CYKE_ABxxEFxx);
    /* p30.XADU*/ sprite_scanner.XADU_SPRITE_IDX0p.dff13(_WUDA_xxCDxxGH, _WEFE_VCC, _YFOT_OAM_A2p);
    /* p30.XEDY*/ sprite_scanner.XEDY_SPRITE_IDX1p.dff13(_WUDA_xxCDxxGH, _WEFE_VCC, _YFOC_OAM_A3p);
    /* p30.ZUZE*/ sprite_scanner.ZUZE_SPRITE_IDX2p.dff13(_WUDA_xxCDxxGH, _WEFE_VCC, _YVOM_OAM_A4p);
    /* p30.XOBE*/ sprite_scanner.XOBE_SPRITE_IDX3p.dff13(_WUDA_xxCDxxGH, _WEFE_VCC, _YMEV_OAM_A5p);
    /* p30.YDUF*/ sprite_scanner.YDUF_SPRITE_IDX4p.dff13(_WUDA_xxCDxxGH, _WEFE_VCC, _XEMU_OAM_A6p);
    /* p30.XECU*/ sprite_scanner.XECU_SPRITE_IDX5p.dff13(_WUDA_xxCDxxGH, _WEFE_VCC, _YZET_OAM_A7p);

    oam_addr = pack_u8n_new(7, &BUS_OAM_An[1]);
  }


  /* p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp.dff17(_MYVO_AxCxExGx_clk, _NYXU_FETCH_TRIGn, _LYRY_BFETCH_DONEp);
  /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn = and2(tile_fetcher.LOVY_FETCH_DONEp.qn(), pix_pipe.XYMU_RENDERINGn.qn());
  /* p27.LONY*/ tile_fetcher.LONY_FETCHINGp.nand_latch(_NYXU_FETCH_TRIGn, _LURY_BG_FETCH_DONEn);

  /* p29.TEPA*/ wire _TEPA_RENDERINGn = not1(pix_pipe.XYMU_RENDERINGn.qn());
  /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp = nor3(_TEPA_RENDERINGn, sprite_fetcher.TULY_SFETCH_S1p.qp(), sprite_fetcher.TESE_SFETCH_S2p.qp());
  /* p29.TYTU*/ wire _TYTU_SFETCH_S0n = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp());
  /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp(), _TYTU_SFETCH_S0n);

  /*#p29.XUQU*/ wire _XUQU_SPRITE_AB = not1(sprite_fetcher.VONU_SFETCH_S1p_D4.qn());
  /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp());
  /* p29.TYSO*/   wire _TYSO_SPRITE_READn = or2(_SAKY_SFETCH_MYSTERY, _TEPA_RENDERINGn); // def or

  /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp = not1(_TYSO_SPRITE_READn);

  /*#p25.ROPY*/ wire _ROPY_RENDERINGn = not1(pix_pipe.XYMU_RENDERINGn.qn());
  /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp  = and2(_TOLE_VRAM_RDp_new, _ROPY_RENDERINGn);
  /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp = not1(_SALE_CPU_VRAM_WRn_new);
  /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp  = and2(_SERE_CPU_VRM_RDp, _RUVY_CPU_VRAM_WRp);
  /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn  = not1(_SAZO_CBD_TO_VPDp);
  /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp  = not1(_RYJE_CBD_TO_VPDn);
  /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp  = or2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
  /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn  = not1(_RELA_CBD_TO_VPDp);

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n = not1(tile_fetcher.LAXU_BFETCH_S0p.qp());
  /*#p27.MYSO*/ wire _MYSO_BG_TRIGp = nor3(_LOBY_RENDERINGn_new, _LAXE_BFETCH_S0n, tile_fetcher.LYZU_BFETCH_S0p_D1.qp()); // MYSO fires on fetch phase 2, 6, 10
  /* p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn = not1(tile_fetcher.LONY_FETCHINGp.qp());
  /* p27.LUSU*/ wire _LUSU_BGW_VRAM_RDn = not1(tile_fetcher.LONY_FETCHINGp.qp());
  /* p27.LENA*/ wire _LENA_BGW_VRAM_RDp = not1(_LUSU_BGW_VRAM_RDn);
  /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n = not1(tile_fetcher.MESU_BFETCH_S1p.qp());
  /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n = not1(tile_fetcher.NYVA_BFETCH_S2p.qp());
  /* p27.NOGU*/ wire _NOGU_BFETCH_01p = nand2(_NAKO_BFETCH_S1n, _NOFU_BFETCH_S2n);
  /* p27.NENY*/ wire _NENY_BFETCH_01n = not1(_NOGU_BFETCH_01p);
  /*#p27.XUHA*/ wire _XUHA_FETCH_S2p = not1(_NOFU_BFETCH_S2n);
  /* p27.POTU*/ wire _POTU_MAP_READp = and2(_LENA_BGW_VRAM_RDp, _NENY_BFETCH_01n);
  /* p27.NETA*/ wire _NETA_TILE_READp = and2(_LENA_BGW_VRAM_RDp, _NOGU_BFETCH_01p);

  /*#p25.XANE*/ wire _XANE_VRAM_LOCKn = nor2(_LUFA_DMA_VRAMp_new, pix_pipe.XYMU_RENDERINGn.qn());

  uint16_t vram_addr;
  {
    Bus2 BUS_VRAM_An[13];

    {
      /* CBA -> VBA */
      /* p25.XEDU*/ wire _XEDU_VRAM_LOCKp = not1(_XANE_VRAM_LOCKn);
      /* p25.XAKY*/ BUS_VRAM_An[ 0].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 0]);
      /* p25.XUXU*/ BUS_VRAM_An[ 1].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 1]);
      /* p25.XYNE*/ BUS_VRAM_An[ 2].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 2]);
      /* p25.XODY*/ BUS_VRAM_An[ 3].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 3]);
      /* p25.XECA*/ BUS_VRAM_An[ 4].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 4]);
      /* p25.XOBA*/ BUS_VRAM_An[ 5].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 5]);
      /* p25.XOPO*/ BUS_VRAM_An[ 6].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 6]);
      /* p25.XYBO*/ BUS_VRAM_An[ 7].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 7]);
      /* p25.RYSU*/ BUS_VRAM_An[ 8].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 8]);
      /* p25.RESE*/ BUS_VRAM_An[ 9].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[ 9]);
      /* p25.RUSE*/ BUS_VRAM_An[10].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[10]);
      /* p25.RYNA*/ BUS_VRAM_An[11].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[11]);
      /* p25.RUMO*/ BUS_VRAM_An[12].tri6_nn(_XEDU_VRAM_LOCKp, BUS_CPU_A[12]);
    }

    {
      /* DBA -> VBA */
      /* p04.AHOC*/ wire _AHOC_DMA_VRAMn = not1(_LUFA_DMA_VRAMp_new);
      /* p04.NAKY*/ wire _NAKY_DMA_A00p = dma_reg.NAKY_DMA_A00p.qp();
      /* p04.PYRO*/ wire _PYRO_DMA_A01p = dma_reg.PYRO_DMA_A01p.qp();
      /* p04.NEFY*/ wire _NEFY_DMA_A02p = dma_reg.NEFY_DMA_A02p.qp();
      /* p04.MUTY*/ wire _MUTY_DMA_A03p = dma_reg.MUTY_DMA_A03p.qp();
      /* p04.NYKO*/ wire _NYKO_DMA_A04p = dma_reg.NYKO_DMA_A04p.qp();
      /* p04.PYLO*/ wire _PYLO_DMA_A05p = dma_reg.PYLO_DMA_A05p.qp();
      /* p04.NUTO*/ wire _NUTO_DMA_A06p = dma_reg.NUTO_DMA_A06p.qp();
      /* p04.MUGU*/ wire _MUGU_DMA_A07p = dma_reg.MUGU_DMA_A07p.qp();
      /* p04.NAFA*/ wire _NAFA_DMA_A08p = dma_reg.NAFA_DMA_A08n.qn();
      /* p04.PYNE*/ wire _PYNE_DMA_A09p = dma_reg.PYNE_DMA_A09n.qn();
      /* p04.PARA*/ wire _PARA_DMA_A10p = dma_reg.PARA_DMA_A10n.qn();
      /* p04.NYDO*/ wire _NYDO_DMA_A11p = dma_reg.NYDO_DMA_A11n.qn();
      /* p04.NYGY*/ wire _NYGY_DMA_A12p = dma_reg.NYGY_DMA_A12n.qn();

      /* p04.ECAL*/ BUS_VRAM_An[ 0].tri6_nn(_AHOC_DMA_VRAMn, _NAKY_DMA_A00p);
      /* p04.EGEZ*/ BUS_VRAM_An[ 1].tri6_nn(_AHOC_DMA_VRAMn, _PYRO_DMA_A01p);
      /* p04.FUHE*/ BUS_VRAM_An[ 2].tri6_nn(_AHOC_DMA_VRAMn, _NEFY_DMA_A02p);
      /* p04.FYZY*/ BUS_VRAM_An[ 3].tri6_nn(_AHOC_DMA_VRAMn, _MUTY_DMA_A03p);
      /* p04.DAMU*/ BUS_VRAM_An[ 4].tri6_nn(_AHOC_DMA_VRAMn, _NYKO_DMA_A04p);
      /* p04.DAVA*/ BUS_VRAM_An[ 5].tri6_nn(_AHOC_DMA_VRAMn, _PYLO_DMA_A05p);
      /* p04.ETEG*/ BUS_VRAM_An[ 6].tri6_nn(_AHOC_DMA_VRAMn, _NUTO_DMA_A06p);
      /*#p04.EREW*/ BUS_VRAM_An[ 7].tri6_nn(_AHOC_DMA_VRAMn, _MUGU_DMA_A07p);
      /*#p04.EVAX*/ BUS_VRAM_An[ 8].tri6_nn(_AHOC_DMA_VRAMn, _NAFA_DMA_A08p);
      /* p04.DUVE*/ BUS_VRAM_An[ 9].tri6_nn(_AHOC_DMA_VRAMn, _PYNE_DMA_A09p);
      /* p04.ERAF*/ BUS_VRAM_An[10].tri6_nn(_AHOC_DMA_VRAMn, _PARA_DMA_A10p);
      /* p04.FUSY*/ BUS_VRAM_An[11].tri6_nn(_AHOC_DMA_VRAMn, _NYDO_DMA_A11p);
      /* p04.EXYF*/ BUS_VRAM_An[12].tri6_nn(_AHOC_DMA_VRAMn, _NYGY_DMA_A12p);
    }

    /* Sprite fetcher read */
    {
      /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not1(pix_pipe.XYMO_LCDC_SPSIZEn.qn());
      /*#p29.WUKY*/ wire _WUKY_FLIP_Yp = not1(oam_bus.YZOS_OAM_DB6p.qp());
      /*#p29.CYVU*/ wire _CYVU_L0 = xor2(_WUKY_FLIP_Yp, SPR_TRI_L_in[0].qp());
      /*#p29.BORE*/ wire _BORE_L1 = xor2(_WUKY_FLIP_Yp, SPR_TRI_L_in[1].qp());
      /*#p29.BUVY*/ wire _BUVY_L2 = xor2(_WUKY_FLIP_Yp, SPR_TRI_L_in[2].qp());
      /*#p29.WAGO*/ wire _WAGO_L3 = xor2(_WUKY_FLIP_Yp, SPR_TRI_L_in[3].qp());
      /*#p29.GEJY*/ wire _GEJY_L3 = amux2(oam_bus.XUSO_OAM_DA0p.qp(), _FUFO_LCDC_SPSIZEn, pix_pipe.XYMO_LCDC_SPSIZEn.qn(), _WAGO_L3);

      /* p29.ABON*/ wire _ABON_SPR_VRM_RDn = not1(_TEXY_SPR_READ_VRAMp);
      /* p29.ABEM*/ BUS_VRAM_An[ 0].tri6_nn(_ABON_SPR_VRM_RDn, _XUQU_SPRITE_AB);
      /* p29.BAXE*/ BUS_VRAM_An[ 1].tri6_nn(_ABON_SPR_VRM_RDn, _CYVU_L0);
      /* p29.ARAS*/ BUS_VRAM_An[ 2].tri6_nn(_ABON_SPR_VRM_RDn, _BORE_L1);
      /* p29.AGAG*/ BUS_VRAM_An[ 3].tri6_nn(_ABON_SPR_VRM_RDn, _BUVY_L2);
      /* p29.FAMU*/ BUS_VRAM_An[ 4].tri6_nn(_ABON_SPR_VRM_RDn, _GEJY_L3);
      /*#p29.FUGY*/ BUS_VRAM_An[ 5].tri6_nn(_ABON_SPR_VRM_RDn, oam_bus.XEGU_OAM_DA1p.qp());
      /* p29.GAVO*/ BUS_VRAM_An[ 6].tri6_nn(_ABON_SPR_VRM_RDn, oam_bus.YJEX_OAM_DA2p.qp());
      /* p29.WYGA*/ BUS_VRAM_An[ 7].tri6_nn(_ABON_SPR_VRM_RDn, oam_bus.XYJU_OAM_DA3p.qp());
      /* p29.WUNE*/ BUS_VRAM_An[ 8].tri6_nn(_ABON_SPR_VRM_RDn, oam_bus.YBOG_OAM_DA4p.qp());
      /* p29.GOTU*/ BUS_VRAM_An[ 9].tri6_nn(_ABON_SPR_VRM_RDn, oam_bus.WYSO_OAM_DA5p.qp());
      /* p29.GEGU*/ BUS_VRAM_An[10].tri6_nn(_ABON_SPR_VRM_RDn, oam_bus.XOTE_OAM_DA6p.qp());
      /* p29.XEHE*/ BUS_VRAM_An[11].tri6_nn(_ABON_SPR_VRM_RDn, oam_bus.YZAB_OAM_DA7p.qp());
      /* p29.DYSO*/ BUS_VRAM_An[12].tri6_nn(_ABON_SPR_VRM_RDn, 0);   // sprites always in low half of tile store
    }

    /*#p27.NOCU*/ wire _NOCU_WIN_MODEn = not1(pix_pipe.PYNU_WIN_MODE_Ap.qp());
    /* p27.PORE*/ wire _PORE_WIN_MODEp = not1(_NOCU_WIN_MODEn);
    /* p26.AXAD*/ wire _AXAD_WIN_MODEn = not1(_PORE_WIN_MODEp);

    /* Background & window map/tile read */
    {
      //----------------------------------------
      // Window matcher

      ///*#p26.ATAD*/ wire _ATAD_TILE_X0S = add_s(pix_pipe.XEHO_PX0p.qp17_new(), pix_pipe.DATY_SCX0n.qn08_new(), 0);
      /*#p26.ATAD*/ wire _ATAD_TILE_X0C = add_c(pix_pipe.XEHO_PX0p.qp(), pix_pipe.DATY_SCX0n_s.qn(), 0);
      ///* p26.BEHU*/ wire _BEHU_TILE_X1S = add_s(pix_pipe.SAVY_PX1p.qp17_new(), pix_pipe.DUZU_SCX1n.qn08_new(), _ATAD_TILE_X0C);
      /* p26.BEHU*/ wire _BEHU_TILE_X1C = add_c(pix_pipe.SAVY_PX1p.qp(), pix_pipe.DUZU_SCX1n_s.qn(), _ATAD_TILE_X0C);
      ///* p26.APYH*/ wire _APYH_TILE_X2S = add_s(pix_pipe.XODU_PX2p.qp17_new(), pix_pipe.CYXU_SCX2n.qn08_new(), _BEHU_TILE_X1C);
      /* p26.APYH*/ wire _APYH_TILE_X2C = add_c(pix_pipe.XODU_PX2p.qp(), pix_pipe.CYXU_SCX2n_s.qn(), _BEHU_TILE_X1C);
      /* p26.BABE*/ wire _BABE_MAP_X0S  = add_s(pix_pipe.XYDO_PX3p.qp(), pix_pipe.GUBO_SCX3n_s.qn(), _APYH_TILE_X2C);
      /* p26.BABE*/ wire _BABE_MAP_X0C  = add_c(pix_pipe.XYDO_PX3p.qp(), pix_pipe.GUBO_SCX3n_s.qn(), _APYH_TILE_X2C);
      /* p26.ABOD*/ wire _ABOD_MAP_X1S  = add_s(pix_pipe.TUHU_PX4p.qp(), pix_pipe.BEMY_SCX4n_s.qn(), _BABE_MAP_X0C);
      /* p26.ABOD*/ wire _ABOD_MAP_X1C  = add_c(pix_pipe.TUHU_PX4p.qp(), pix_pipe.BEMY_SCX4n_s.qn(), _BABE_MAP_X0C);
      /* p26.BEWY*/ wire _BEWY_MAP_X2S  = add_s(pix_pipe.TUKY_PX5p.qp(), pix_pipe.CUZY_SCX5n_s.qn(), _ABOD_MAP_X1C);
      /* p26.BEWY*/ wire _BEWY_MAP_X2C  = add_c(pix_pipe.TUKY_PX5p.qp(), pix_pipe.CUZY_SCX5n_s.qn(), _ABOD_MAP_X1C);
      /* p26.BYCA*/ wire _BYCA_MAP_X3S  = add_s(pix_pipe.TAKO_PX6p.qp(), pix_pipe.CABU_SCX6n_s.qn(), _BEWY_MAP_X2C);
      /* p26.BYCA*/ wire _BYCA_MAP_X3C  = add_c(pix_pipe.TAKO_PX6p.qp(), pix_pipe.CABU_SCX6n_s.qn(), _BEWY_MAP_X2C);
      /* p26.ACUL*/ wire _ACUL_MAP_X4S  = add_s(pix_pipe.SYBE_PX7p.qp(), pix_pipe.BAKE_SCX7n_s.qn(), _BYCA_MAP_X3C);
      ///* p26.ACUL*/ wire _ACUL_MAP_X4C  = add_c(pix_pipe.SYBE_PX7p.qp17_new(), pix_pipe.BAKE_SCX7n.qn08_new(), _BYCA_MAP_X3C);

      /* p26.ACEN*/ wire _ACEN_BG_MAP_READp = and2(_POTU_MAP_READp, _AXAD_WIN_MODEn);
      /* p26.BAFY*/ wire _BAFY_BG_MAP_READn = not1(_ACEN_BG_MAP_READp);
      /* p26.AXEP*/ BUS_VRAM_An[ 0].tri6_nn(_BAFY_BG_MAP_READn, _BABE_MAP_X0S);
      /* p26.AFEB*/ BUS_VRAM_An[ 1].tri6_nn(_BAFY_BG_MAP_READn, _ABOD_MAP_X1S);
      /* p26.ALEL*/ BUS_VRAM_An[ 2].tri6_nn(_BAFY_BG_MAP_READn, _BEWY_MAP_X2S);
      /* p26.COLY*/ BUS_VRAM_An[ 3].tri6_nn(_BAFY_BG_MAP_READn, _BYCA_MAP_X3S);
      /* p26.AJAN*/ BUS_VRAM_An[ 4].tri6_nn(_BAFY_BG_MAP_READn, _ACUL_MAP_X4S);
      /* p26.DUHO*/ BUS_VRAM_An[ 5].tri6_nn(_BAFY_BG_MAP_READn, _ETAM_MAP_Y0S);
      /* p26.CASE*/ BUS_VRAM_An[ 6].tri6_nn(_BAFY_BG_MAP_READn, _DOTO_MAP_Y1S);
      /* p26.CYPO*/ BUS_VRAM_An[ 7].tri6_nn(_BAFY_BG_MAP_READn, _DABA_MAP_Y2S);
      /* p26.CETA*/ BUS_VRAM_An[ 8].tri6_nn(_BAFY_BG_MAP_READn, _EFYK_MAP_Y3S);
      /* p26.DAFE*/ BUS_VRAM_An[ 9].tri6_nn(_BAFY_BG_MAP_READn, _EJOK_MAP_Y4S);
      /*#p26.AMUV*/ BUS_VRAM_An[10].tri6_nn(_BAFY_BG_MAP_READn, pix_pipe.XAFO_LCDC_BGMAPn.qn());
      /* p26.COVE*/ BUS_VRAM_An[11].tri6_nn(_BAFY_BG_MAP_READn, 1);
      /* p26.COXO*/ BUS_VRAM_An[12].tri6_nn(_BAFY_BG_MAP_READn, 1);
    }

    {
      /*#p25.XEZE*/ wire _XEZE_WIN_MAP_READp = and2(_POTU_MAP_READp, _PORE_WIN_MODEp);
      /*#p25.WUKO*/ wire _WUKO_WIN_MAP_READn = not1(_XEZE_WIN_MAP_READp);
      /*#p27.XEJA*/ BUS_VRAM_An[ 0].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.WYKA_WIN_X3.qp());
      /* p27.XAMO*/ BUS_VRAM_An[ 1].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.WODY_WIN_X4.qp());
      /* p27.XAHE*/ BUS_VRAM_An[ 2].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.WOBO_WIN_X5.qp());
      /* p27.XULO*/ BUS_VRAM_An[ 3].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.WYKO_WIN_X6.qp());
      /* p27.WUJU*/ BUS_VRAM_An[ 4].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.XOLO_WIN_X7.qp());
      /*#p27.VYTO*/ BUS_VRAM_An[ 5].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.TUFU_WIN_Y3.qp());
      /* p27.VEHA*/ BUS_VRAM_An[ 6].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.TAXA_WIN_Y4.qp());
      /* p27.VACE*/ BUS_VRAM_An[ 7].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.TOZO_WIN_Y5.qp());
      /* p27.VOVO*/ BUS_VRAM_An[ 8].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.TATE_WIN_Y6.qp());
      /* p27.VULO*/ BUS_VRAM_An[ 9].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.TEKE_WIN_Y7.qp());
      /*#p27.VEVY*/ BUS_VRAM_An[10].tri6_nn(_WUKO_WIN_MAP_READn, pix_pipe.WOKY_LCDC_WINMAPn.qn());
      /* p27.VEZA*/ BUS_VRAM_An[11].tri6_nn(_WUKO_WIN_MAP_READn, 1);
      /* p27.VOGU*/ BUS_VRAM_An[12].tri6_nn(_WUKO_WIN_MAP_READn, 1);
    }

    {
      /* p26.ASUL*/ wire _ASUL_BG_TILE_READp = and2(_NETA_TILE_READp, _AXAD_WIN_MODEn);
      /* p26.BEJE*/ wire _BEJE_BG_TILE_READn = not1(_ASUL_BG_TILE_READp);
      /*#p26.ASUM*/ BUS_VRAM_An[0].tri6_nn(_BEJE_BG_TILE_READn, _XUHA_FETCH_S2p);
      /* p26.EVAD*/ BUS_VRAM_An[1].tri6_nn(_BEJE_BG_TILE_READn, _FAFO_TILE_Y0S);
      /* p26.DAHU*/ BUS_VRAM_An[2].tri6_nn(_BEJE_BG_TILE_READn, _EMUX_TILE_Y1S);
      /* p26.DODE*/ BUS_VRAM_An[3].tri6_nn(_BEJE_BG_TILE_READn, _ECAB_TILE_Y2S); // check outputs of ECAB
    }

    {
      /* p25.XUCY*/ wire _XUCY_WIN_TILE_READn = nand2(_NETA_TILE_READp, _PORE_WIN_MODEp);
      /*#p25.XONU*/ BUS_VRAM_An[0].tri6_nn(_XUCY_WIN_TILE_READn, _XUHA_FETCH_S2p);
      /*#p25.WUDO*/ BUS_VRAM_An[1].tri6_nn(_XUCY_WIN_TILE_READn, pix_pipe.VYNO_WIN_Y0.qp());
      /*#p25.WAWE*/ BUS_VRAM_An[2].tri6_nn(_XUCY_WIN_TILE_READn, pix_pipe.VUJO_WIN_Y1.qp());
      /*#p25.WOLU*/ BUS_VRAM_An[3].tri6_nn(_XUCY_WIN_TILE_READn, pix_pipe.VYMU_WIN_Y2.qp());
      /*#p25.VAPY*/ BUS_VRAM_An[ 4].tri6_pn(_NETA_TILE_READp,    vram_bus.RAWU_TILE_DB0p.qp());
      /*#p25.SEZU*/ BUS_VRAM_An[ 5].tri6_pn(_NETA_TILE_READp,    vram_bus.POZO_TILE_DB1p.qp());
      /*#p25.VEJY*/ BUS_VRAM_An[ 6].tri6_pn(_NETA_TILE_READp,    vram_bus.PYZO_TILE_DB2p.qp());
      /*#p25.RUSA*/ BUS_VRAM_An[ 7].tri6_pn(_NETA_TILE_READp,    vram_bus.POXA_TILE_DB3p.qp());
      /*#p25.ROHA*/ BUS_VRAM_An[ 8].tri6_pn(_NETA_TILE_READp,    vram_bus.PULO_TILE_DB4p.qp());
      /*#p25.RESO*/ BUS_VRAM_An[ 9].tri6_pn(_NETA_TILE_READp,    vram_bus.POJU_TILE_DB5p.qp());
      /*#p25.SUVO*/ BUS_VRAM_An[10].tri6_pn(_NETA_TILE_READp,    vram_bus.POWY_TILE_DB6p.qp());
      /*#p25.TOBO*/ BUS_VRAM_An[11].tri6_pn(_NETA_TILE_READp,    vram_bus.PYJU_TILE_DB7p.qp());

      /*#p25.VUZA*/ wire _VUZA_TILE_BANKp = nor2(vram_bus.PYJU_TILE_DB7p.qp(), pix_pipe.WEXU_LCDC_BGTILEn.qn());
      /*#p25.VURY*/ BUS_VRAM_An[12].tri6_pn(_NETA_TILE_READp, _VUZA_TILE_BANKp);
    }

    {
      // VRAM addr bus -> VRAM addr pin
      /* p25.MYFU*/ wire _MYFUp = not1(BUS_VRAM_An[ 0].qp());
      /* p25.MASA*/ wire _MASAp = not1(BUS_VRAM_An[ 1].qp());
      /* p25.MYRE*/ wire _MYREp = not1(BUS_VRAM_An[ 2].qp());
      /* p25.MAVU*/ wire _MAVUp = not1(BUS_VRAM_An[ 3].qp());
      /* p25.MEPA*/ wire _MEPAp = not1(BUS_VRAM_An[ 4].qp());
      /* p25.MYSA*/ wire _MYSAp = not1(BUS_VRAM_An[ 5].qp());
      /* p25.MEWY*/ wire _MEWYp = not1(BUS_VRAM_An[ 6].qp());
      /* p25.MUME*/ wire _MUMEp = not1(BUS_VRAM_An[ 7].qp());
      /* p25.VOVA*/ wire _VOVAp = not1(BUS_VRAM_An[ 8].qp());
      /* p25.VODE*/ wire _VODEp = not1(BUS_VRAM_An[ 9].qp());
      /* p25.RUKY*/ wire _RUKYp = not1(BUS_VRAM_An[10].qp());
      /* p25.RUMA*/ wire _RUMAp = not1(BUS_VRAM_An[11].qp());
      /* p25.REHO*/ wire _REHOp = not1(BUS_VRAM_An[12].qp());

      /* p25.LEXE*/ wire _LEXEn = not1(_MYFUp);
      /* p25.LOZU*/ wire _LOZUn = not1(_MASAp);
      /* p25.LACA*/ wire _LACAn = not1(_MYREp);
      /* p25.LUVO*/ wire _LUVOn = not1(_MAVUp);
      /* p25.LOLY*/ wire _LOLYn = not1(_MEPAp);
      /* p25.LALO*/ wire _LALOn = not1(_MYSAp);
      /* p25.LEFA*/ wire _LEFAn = not1(_MEWYp);
      /* p25.LUBY*/ wire _LUBYn = not1(_MUMEp);
      /* p25.TUJY*/ wire _TUJYn = not1(_VOVAp);
      /* p25.TAGO*/ wire _TAGOn = not1(_VODEp);
      /* p25.NUVA*/ wire _NUVAn = not1(_RUKYp);
      /* p25.PEDU*/ wire _PEDUn = not1(_RUMAp);
      /* p25.PONY*/ wire _PONYn = not1(_REHOp);

      Pin2 PIN_VRAM_Ap[13];
      PIN_VRAM_Ap[ 0].pin_out(1, _LEXEn, _LEXEn);
      PIN_VRAM_Ap[ 1].pin_out(1, _LOZUn, _LOZUn);
      PIN_VRAM_Ap[ 2].pin_out(1, _LACAn, _LACAn);
      PIN_VRAM_Ap[ 3].pin_out(1, _LUVOn, _LUVOn);
      PIN_VRAM_Ap[ 4].pin_out(1, _LOLYn, _LOLYn);
      PIN_VRAM_Ap[ 5].pin_out(1, _LALOn, _LALOn);
      PIN_VRAM_Ap[ 6].pin_out(1, _LEFAn, _LEFAn);
      PIN_VRAM_Ap[ 7].pin_out(1, _LUBYn, _LUBYn);
      PIN_VRAM_Ap[ 8].pin_out(1, _TUJYn, _TUJYn);
      PIN_VRAM_Ap[ 9].pin_out(1, _TAGOn, _TAGOn);
      PIN_VRAM_Ap[10].pin_out(1, _NUVAn, _NUVAn);
      PIN_VRAM_Ap[11].pin_out(1, _PEDUn, _PEDUn);
      PIN_VRAM_Ap[12].pin_out(1, _PONYn, _PONYn);

      vram_addr = pack_u16p_new(13, PIN_VRAM_Ap);
    }
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



  {
    /*#p27.NOCU*/ wire _NOCU_WIN_MODEn = not1(pix_pipe.PYNU_WIN_MODE_Ap.qp());
    /* p27.PORE*/ wire _PORE_WIN_MODEp = not1(_NOCU_WIN_MODEn);
    /* p27.VETU*/ wire _VETU_WIN_MAP_CLK = and2(_TEVO_FETCH_TRIGp, _PORE_WIN_MODEp);
    /* p27.XACO*/ wire _XACO_WIN_RSTn = not1(_XOFO_WIN_RSTp_new);
    /* p27.WYKA*/ pix_pipe.WYKA_WIN_X3.dff17(_VETU_WIN_MAP_CLK,               _XACO_WIN_RSTn, pix_pipe.WYKA_WIN_X3.qn());
    /* p27.WODY*/ pix_pipe.WODY_WIN_X4.dff17(pix_pipe.WYKA_WIN_X3.qn(), _XACO_WIN_RSTn, pix_pipe.WODY_WIN_X4.qn());
    /* p27.WOBO*/ pix_pipe.WOBO_WIN_X5.dff17(pix_pipe.WODY_WIN_X4.qn(), _XACO_WIN_RSTn, pix_pipe.WOBO_WIN_X5.qn());
    /* p27.WYKO*/ pix_pipe.WYKO_WIN_X6.dff17(pix_pipe.WOBO_WIN_X5.qn(), _XACO_WIN_RSTn, pix_pipe.WYKO_WIN_X6.qn());
    /* p27.XOLO*/ pix_pipe.XOLO_WIN_X7.dff17(pix_pipe.WYKO_WIN_X6.qn(), _XACO_WIN_RSTn, pix_pipe.XOLO_WIN_X7.qn());
  }

  {
    /*#p27.NAZE*/ wire _NAZE_WY_MATCH0p = xnor2(pix_pipe.NESO_WY0n_s.qn(), lcd_reg.MUWY_LY0p.qp());
    /* p27.PEBO*/ wire _PEBO_WY_MATCH1p = xnor2(pix_pipe.NYRO_WY1n_s.qn(), lcd_reg.MYRO_LY1p.qp());
    /* p27.POMO*/ wire _POMO_WY_MATCH2p = xnor2(pix_pipe.NAGA_WY2n_s.qn(), lcd_reg.LEXA_LY2p.qp());
    /* p27.NEVU*/ wire _NEVU_WY_MATCH3p = xnor2(pix_pipe.MELA_WY3n_s.qn(), lcd_reg.LYDO_LY3p.qp());
    /* p27.NOJO*/ wire _NOJO_WY_MATCH4p = xnor2(pix_pipe.NULO_WY4n_s.qn(), lcd_reg.LOVU_LY4p.qp());
    /* p27.PAGA*/ wire _PAGA_WY_MATCH5p = xnor2(pix_pipe.NENE_WY5n_s.qn(), lcd_reg.LEMA_LY5p.qp());
    /* p27.PEZO*/ wire _PEZO_WY_MATCH6p = xnor2(pix_pipe.NUKA_WY6n_s.qn(), lcd_reg.MATO_LY6p.qp());
    /* p27.NUPA*/ wire _NUPA_WY_MATCH7p = xnor2(pix_pipe.NAFU_WY7n_s.qn(), lcd_reg.LAFO_LY7p.qp());

    /*#p27.PALO*/ wire _PALO_WY_MATCH_HIn  = nand5(pix_pipe.WYMO_LCDC_WINENn.qn(), _NOJO_WY_MATCH4p, _PAGA_WY_MATCH5p, _PEZO_WY_MATCH6p, _NUPA_WY_MATCH7p);
    /* p27.NELE*/ wire _NELE_WY_MATCH_HIp  = not1(_PALO_WY_MATCH_HIn);
    /* p27.PAFU*/ wire _PAFU_WY_MATCHn     = nand5(_NELE_WY_MATCH_HIp, _NAZE_WY_MATCH0p, _PEBO_WY_MATCH1p, _POMO_WY_MATCH2p, _NEVU_WY_MATCH3p);
    /* p27.ROGE*/ wire _ROGE_WY_MATCHp     = not1(_PAFU_WY_MATCHn);

    /*#p21.PARU*/ wire _PARU_VBLANKp = not1(lcd_reg.POPU_VBLANK_CLKp.qn());
    /* p27.REPU*/ wire _REPU_VBLANKp = or2(_PARU_VBLANKp, _PYRY_VID_RSTp_new);

    /* p27.SARY*/ pix_pipe.SARY_WY_MATCHp.dff17(_TALU_xxCDEFxx_new, _XAPO_VID_RSTn_new, _ROGE_WY_MATCHp);
    /* p27.REJO*/ pix_pipe.REJO_WY_MATCHp.nor_latch(pix_pipe.SARY_WY_MATCHp.qp(), _REPU_VBLANKp);

    // Every time we leave win mode we increment win_y
    /*#p27.NOCU*/ wire _NOCU_WIN_Y_CLKp = not1(pix_pipe.PYNU_WIN_MODE_Ap.qp());
    /* p27.PORE*/ wire _PORE_WIN_Y_CLKn = not1(_NOCU_WIN_Y_CLKp);
    /* p27.WAZY*/ wire _WAZY_WIN_Y_CLKp = not1(_PORE_WIN_Y_CLKn);
    /* p27.SYNY*/ wire _SYNY_VBLANK_RSTn_t3 = not1(_REPU_VBLANKp);
    /* p27.VYNO*/ pix_pipe.VYNO_WIN_Y0.dff17(_WAZY_WIN_Y_CLKp,                _SYNY_VBLANK_RSTn_t3, pix_pipe.VYNO_WIN_Y0.qn());
    /* p27.VUJO*/ pix_pipe.VUJO_WIN_Y1.dff17(pix_pipe.VYNO_WIN_Y0.qn(), _SYNY_VBLANK_RSTn_t3, pix_pipe.VUJO_WIN_Y1.qn());
    /* p27.VYMU*/ pix_pipe.VYMU_WIN_Y2.dff17(pix_pipe.VUJO_WIN_Y1.qn(), _SYNY_VBLANK_RSTn_t3, pix_pipe.VYMU_WIN_Y2.qn());
    /* p27.TUFU*/ pix_pipe.TUFU_WIN_Y3.dff17(pix_pipe.VYMU_WIN_Y2.qn(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TUFU_WIN_Y3.qn());
    /* p27.TAXA*/ pix_pipe.TAXA_WIN_Y4.dff17(pix_pipe.TUFU_WIN_Y3.qn(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TAXA_WIN_Y4.qn());
    /* p27.TOZO*/ pix_pipe.TOZO_WIN_Y5.dff17(pix_pipe.TAXA_WIN_Y4.qn(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TOZO_WIN_Y5.qn());
    /* p27.TATE*/ pix_pipe.TATE_WIN_Y6.dff17(pix_pipe.TOZO_WIN_Y5.qn(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TATE_WIN_Y6.qn());
    /* p27.TEKE*/ pix_pipe.TEKE_WIN_Y7.dff17(pix_pipe.TATE_WIN_Y6.qn(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TEKE_WIN_Y7.qn());
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region OAM_in
  {
    /* oam_ram -> OBD */
    // FIXME This should be using PIN_OAM_CLK (which might actually be PIN_OAM_CSn?)


    /* p28.WEFY*/ wire _WEFY_SPR_READp = and2(_TUVO_PPU_OAM_RDp, sprite_fetcher.TYFO_SFETCH_S0p_D1.qp());
    /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(_WEFY_SPR_READp);
    /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(_ACYL_SCANNINGp, _XOCE_xBCxxFGx_new); // schematic wrong, is def nand2
    /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn = not1(_CATY_LATCH_EXTp);
    /*#p28.BOTA*/ wire _BOTA_OAM_OEn  = nand3(_BOFE_LATCH_EXTn, _SARO_FE00_FEFFp, _ASOT_CPU_RDp_new); // Schematic wrong, this is NAND
    /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn); // def and
    /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);
    /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
    /*#p28.YRYV*/ wire _YRYU_OAM_OEp = not1(_YVAL_OAM_OEn);
    /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYU_OAM_OEp);

    Pin2 PIN_OAM_OEn;
    PIN_OAM_OEn.pin_in(1, _ZODO_OAM_OEn);

    uint8_t oam_data_a = oam_ram[(oam_addr << 1) + 0];
    uint8_t oam_data_b = oam_ram[(oam_addr << 1) + 1];

    Bus2 BUS_OAM_DAn_in[8];
    Bus2 BUS_OAM_DBn_in[8];

    BUS_OAM_DAn_in[0].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_a & 0x01));
    BUS_OAM_DAn_in[1].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_a & 0x02));
    BUS_OAM_DAn_in[2].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_a & 0x04));
    BUS_OAM_DAn_in[3].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_a & 0x08));
    BUS_OAM_DAn_in[4].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_a & 0x10));
    BUS_OAM_DAn_in[5].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_a & 0x20));
    BUS_OAM_DAn_in[6].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_a & 0x40));
    BUS_OAM_DAn_in[7].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_a & 0x80));

    BUS_OAM_DBn_in[0].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_b & 0x01));
    BUS_OAM_DBn_in[1].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_b & 0x02));
    BUS_OAM_DBn_in[2].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_b & 0x04));
    BUS_OAM_DBn_in[3].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_b & 0x08));
    BUS_OAM_DBn_in[4].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_b & 0x10));
    BUS_OAM_DBn_in[5].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_b & 0x20));
    BUS_OAM_DBn_in[6].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_b & 0x40));
    BUS_OAM_DBn_in[7].tri6_nn(PIN_OAM_OEn.qp(), (oam_data_b & 0x80));

    // OPD -> OBL
    /* p29.YDYV*/ oam_bus.YDYV_OAM_LATCH_DA0n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[0].qp());
    /* p29.YCEB*/ oam_bus.YCEB_OAM_LATCH_DA1n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[1].qp());
    /* p29.ZUCA*/ oam_bus.ZUCA_OAM_LATCH_DA2n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[2].qp());
    /* p29.WONE*/ oam_bus.WONE_OAM_LATCH_DA3n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[3].qp());
    /* p29.ZAXE*/ oam_bus.ZAXE_OAM_LATCH_DA4n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[4].qp());
    /* p29.XAFU*/ oam_bus.XAFU_OAM_LATCH_DA5n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[5].qp());
    /* p29.YSES*/ oam_bus.YSES_OAM_LATCH_DA6n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[6].qp());
    /* p29.ZECA*/ oam_bus.ZECA_OAM_LATCH_DA7n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[7].qp());

    /*#p31.XYKY*/ oam_bus.XYKY_OAM_LATCH_DB0n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[0].qp());
    /* p31.YRUM*/ oam_bus.YRUM_OAM_LATCH_DB1n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[1].qp());
    /* p31.YSEX*/ oam_bus.YSEX_OAM_LATCH_DB2n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[2].qp());
    /* p31.YVEL*/ oam_bus.YVEL_OAM_LATCH_DB3n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[3].qp());
    /* p31.WYNO*/ oam_bus.WYNO_OAM_LATCH_DB4n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[4].qp());
    /* p31.CYRA*/ oam_bus.CYRA_OAM_LATCH_DB5n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[5].qp());
    /* p31.ZUVE*/ oam_bus.ZUVE_OAM_LATCH_DB6n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[6].qp());
    /* p31.ECED*/ oam_bus.ECED_OAM_LATCH_DB7n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[7].qp());
  }

  //----------------------------------------

  // OBL -> temp
  /* p25.COTA*/ bool _COTA_AxxxxExxx;
  {
    /*#p29.WOJO*/ wire _WOJO_AxxxExxxp  = nor2(clk_reg.WOSU_AxxDExxHp.qn(), clk_reg.WUVU_ABxxEFxxp.qn());
    /* p29.XYSO*/ wire _XYSO_AxxxExxxn  = not1(_WOJO_AxxxExxxp);
    /* p25.AVER*/ wire _AVER_AxxxExxx   = nand2(_ACYL_SCANNINGp, _XYSO_AxxxExxxn);
    /* p25.VAPE*/ wire _VAPE_OAM_CLKENn = and2(_TUVO_PPU_OAM_RDp, _TACU_SPR_SEQ_5_TRIG);
    /* p25.XUJY*/ wire _XUJY_OAM_CLKENp = not1(_VAPE_OAM_CLKENn);
    /* p25.CUFE*/ wire _CUFE_OAM_CLKENp = not1(or_and3(_SARO_FE00_FEFFp, dma_reg.MATU_DMA_RUNNINGp.qp(), _MOPA_xxxxEFGH_clk)); // CUFE looks like BYHA minus an inverter
    /* p25.BYCU*/ wire _BYCU_xBCDxFGH   = nand3(_AVER_AxxxExxx, _XUJY_OAM_CLKENp, _CUFE_OAM_CLKENp);
    /* p25.COTA*/ _COTA_AxxxxExxx       = not1(_BYCU_xBCDxFGH);
  }

  {
    /* p29.YWOK*/ wire _YWOK_xBCDxFGH    = not1(_COTA_AxxxxExxx);
    /*#p29.XUSO*/ oam_bus.XUSO_OAM_DA0p.dff8n(_YWOK_xBCDxFGH, oam_bus.YDYV_OAM_LATCH_DA0n.qp());
    /* p29.XEGU*/ oam_bus.XEGU_OAM_DA1p.dff8n(_YWOK_xBCDxFGH, oam_bus.YCEB_OAM_LATCH_DA1n.qp());
    /* p29.YJEX*/ oam_bus.YJEX_OAM_DA2p.dff8n(_YWOK_xBCDxFGH, oam_bus.ZUCA_OAM_LATCH_DA2n.qp());
    /* p29.XYJU*/ oam_bus.XYJU_OAM_DA3p.dff8n(_YWOK_xBCDxFGH, oam_bus.WONE_OAM_LATCH_DA3n.qp());
    /* p29.YBOG*/ oam_bus.YBOG_OAM_DA4p.dff8n(_YWOK_xBCDxFGH, oam_bus.ZAXE_OAM_LATCH_DA4n.qp());
    /* p29.WYSO*/ oam_bus.WYSO_OAM_DA5p.dff8n(_YWOK_xBCDxFGH, oam_bus.XAFU_OAM_LATCH_DA5n.qp());
    /* p29.XOTE*/ oam_bus.XOTE_OAM_DA6p.dff8n(_YWOK_xBCDxFGH, oam_bus.YSES_OAM_LATCH_DA6n.qp());
    /* p29.YZAB*/ oam_bus.YZAB_OAM_DA7p.dff8n(_YWOK_xBCDxFGH, oam_bus.ZECA_OAM_LATCH_DA7n.qp());
  }

  {
    /* p31.XEGA*/ wire _XEGA_xBCDxFGH    = not1(_COTA_AxxxxExxx);
    /* p31.YLOR*/ oam_bus.YLOR_OAM_DB0p.dff8n(_XEGA_xBCDxFGH, oam_bus.XYKY_OAM_LATCH_DB0n.qp());
    /* p31.ZYTY*/ oam_bus.ZYTY_OAM_DB1p.dff8n(_XEGA_xBCDxFGH, oam_bus.YRUM_OAM_LATCH_DB1n.qp());
    /* p31.ZYVE*/ oam_bus.ZYVE_OAM_DB2p.dff8n(_XEGA_xBCDxFGH, oam_bus.YSEX_OAM_LATCH_DB2n.qp());
    /* p31.ZEZY*/ oam_bus.ZEZY_OAM_DB3p.dff8n(_XEGA_xBCDxFGH, oam_bus.YVEL_OAM_LATCH_DB3n.qp());
    /* p31.GOMO*/ oam_bus.GOMO_OAM_DB4p.dff8n(_XEGA_xBCDxFGH, oam_bus.WYNO_OAM_LATCH_DB4n.qp());
    /* p31.BAXO*/ oam_bus.BAXO_OAM_DB5p.dff8n(_XEGA_xBCDxFGH, oam_bus.CYRA_OAM_LATCH_DB5n.qp());
    /* p31.YZOS*/ oam_bus.YZOS_OAM_DB6p.dff8n(_XEGA_xBCDxFGH, oam_bus.ZUVE_OAM_LATCH_DB6n.qp());
    /* p31.DEPO*/ oam_bus.DEPO_OAM_DB7p.dff8n(_XEGA_xBCDxFGH, oam_bus.ECED_OAM_LATCH_DB7n.qp());
  }
#pragma endregion

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region TileData

  Bus2 BUS_VRAM_Dp_in[8];
  {
    /* p25.RYLU*/ wire _RYLU_CPU_VRAM_RDn = nand2(_SALE_CPU_VRAM_WRn_new, _XANE_VRAM_LOCKn);
    /* p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp = and2(_TACU_SPR_SEQ_5_TRIG, _TEXY_SPR_READ_VRAMp);
    /* p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn = not1(_SOHO_SPR_VRAM_RDp);

    /* p25.APAM*/ wire _APAM_DMA_VRAMn    = not1(_LUFA_DMA_VRAMp_new);
    /* p25.RACU*/ wire _RACU_MOEn = and4(_RYLU_CPU_VRAM_RDn, _RAWA_SPR_VRAM_RDn, _MYMA_BGW_VRAM_RDn, _APAM_DMA_VRAMn); // def and
    /* p25.SEMA*/ wire _SEMA_MOEn_A = and2(_RACU_MOEn, _RACO_DBG_VRAMn_new);
    /* p25.RUTE*/ wire _RUTE_MOEn_D = or2(_RACU_MOEn, _TUTO_DBG_VRAMp_new); // schematic wrong, second input is RACU
    /* p25.REFO*/ wire _REFO_MOEn_A = not1(_SEMA_MOEn_A);
    /* p25.SAHA*/ wire _SAHA_MOEn_D = not1(_RUTE_MOEn_D);
    Pin2 PIN_VRAM_OEn; // PIN_45
    PIN_VRAM_OEn.pin_out(1, _REFO_MOEn_A, _SAHA_MOEn_D);

    uint8_t  vram_data = vid_ram[vram_addr];

    Pin2 PIN_VRAM_Dp_in[8];  // This pin isn't driven between tile fetches while rendering; where's the pullup?
    PIN_VRAM_Dp_in[0].pin_in(!PIN_VRAM_OEn.qp(), (vram_data & 0x01));
    PIN_VRAM_Dp_in[1].pin_in(!PIN_VRAM_OEn.qp(), (vram_data & 0x02));
    PIN_VRAM_Dp_in[2].pin_in(!PIN_VRAM_OEn.qp(), (vram_data & 0x04));
    PIN_VRAM_Dp_in[3].pin_in(!PIN_VRAM_OEn.qp(), (vram_data & 0x08));
    PIN_VRAM_Dp_in[4].pin_in(!PIN_VRAM_OEn.qp(), (vram_data & 0x10));
    PIN_VRAM_Dp_in[5].pin_in(!PIN_VRAM_OEn.qp(), (vram_data & 0x20));
    PIN_VRAM_Dp_in[6].pin_in(!PIN_VRAM_OEn.qp(), (vram_data & 0x40));
    PIN_VRAM_Dp_in[7].pin_in(!PIN_VRAM_OEn.qp(), (vram_data & 0x80));

    /* p25.RODY*/ BUS_VRAM_Dp_in[0].tri6_pn(_RENA_CBD_TO_VPDn, PIN_VRAM_Dp_in[0].qn());
    /* p25.REBA*/ BUS_VRAM_Dp_in[1].tri6_pn(_RENA_CBD_TO_VPDn, PIN_VRAM_Dp_in[1].qn());
    /* p25.RYDO*/ BUS_VRAM_Dp_in[2].tri6_pn(_RENA_CBD_TO_VPDn, PIN_VRAM_Dp_in[2].qn());
    /* p25.REMO*/ BUS_VRAM_Dp_in[3].tri6_pn(_RENA_CBD_TO_VPDn, PIN_VRAM_Dp_in[3].qn());
    /* p25.ROCE*/ BUS_VRAM_Dp_in[4].tri6_pn(_RENA_CBD_TO_VPDn, PIN_VRAM_Dp_in[4].qn());
    /* p25.ROPU*/ BUS_VRAM_Dp_in[5].tri6_pn(_RENA_CBD_TO_VPDn, PIN_VRAM_Dp_in[5].qn());
    /* p25.RETA*/ BUS_VRAM_Dp_in[6].tri6_pn(_RENA_CBD_TO_VPDn, PIN_VRAM_Dp_in[6].qn());
    /* p25.RAKU*/ BUS_VRAM_Dp_in[7].tri6_pn(_RENA_CBD_TO_VPDn, PIN_VRAM_Dp_in[7].qn());
  }

  {
    /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn = nand3(_MYSO_BG_TRIGp, tile_fetcher.MESU_BFETCH_S1p.qp(), _NOFU_BFETCH_S2n); // NYDY on fetch phase 6
    /*#p32.METE*/ wire _METE_LATCH_TILE_DAp = not1(_NYDY_LATCH_TILE_DAn);
    /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn = not1(_METE_LATCH_TILE_DAp);
    /* p32.LEGU*/ vram_bus.LEGU_TILE_DA0n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[0].qp());
    /* p32.NUDU*/ vram_bus.NUDU_TILE_DA1n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[1].qp());
    /* p32.MUKU*/ vram_bus.MUKU_TILE_DA2n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[2].qp());
    /* p32.LUZO*/ vram_bus.LUZO_TILE_DA3n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[3].qp());
    /* p32.MEGU*/ vram_bus.MEGU_TILE_DA4n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[4].qp());
    /* p32.MYJY*/ vram_bus.MYJY_TILE_DA5n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[5].qp());
    /* p32.NASA*/ vram_bus.NASA_TILE_DA6n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[6].qp());
    /* p32.NEFO*/ vram_bus.NEFO_TILE_DA7n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[7].qp());
  }

  {
    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /* p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp = and2(_MYSO_BG_TRIGp, _NAKO_BFETCH_S1n); // MOFU fires on fetch phase 2 and 10
    /* p32.LESO*/ wire _LESO_LATCH_TILE_DBn = not1(_MOFU_LATCH_TILE_DBp);
    /* p??.LUVE*/ wire _LUVE_LATCH_TILE_DBp = not1(_LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /* p32.LABU*/ wire _LABU_LATCH_TILE_DBn = not1(_LUVE_LATCH_TILE_DBp);
    /* p32.RAWU*/ vram_bus.RAWU_TILE_DB0p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[0].qp());
    /* p32.POZO*/ vram_bus.POZO_TILE_DB1p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[1].qp());
    /* p32.PYZO*/ vram_bus.PYZO_TILE_DB2p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[2].qp());
    /* p32.POXA*/ vram_bus.POXA_TILE_DB3p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[3].qp());
    /* p32.PULO*/ vram_bus.PULO_TILE_DB4p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[4].qp());
    /* p32.POJU*/ vram_bus.POJU_TILE_DB5p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[5].qp());
    /* p32.POWY*/ vram_bus.POWY_TILE_DB6p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[6].qp());
    /* p32.PYJU*/ vram_bus.PYJU_TILE_DB7p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[7].qp());
  }
#pragma endregion

  //----------------------------------------
  // 10 sprite stores
  // Resetting the store X coords to 0 doesn't make sense, as they'd fire during a line even if we never stored any sprites.
  // I guess it must store inverted X, so that when reset X = 0xFF?

  {
    Bus2 SPR_TRI_I_out[6]; // to sprite store
    Bus2 SPR_TRI_L_out[4]; // to sprite store

    bool _GESE_SCAN_MATCH_Yp;
    {
      // this is using an adder as a subtracter by inverting the first input.

      /*#p29.EBOS*/ wire _EBOS_LY0n = not1(lcd_reg.MUWY_LY0p.qp());
      /* p29.DASA*/ wire _DASA_LY1n = not1(lcd_reg.MYRO_LY1p.qp());
      /* p29.FUKY*/ wire _FUKY_LY2n = not1(lcd_reg.LEXA_LY2p.qp());
      /* p29.FUVE*/ wire _FUVE_LY3n = not1(lcd_reg.LYDO_LY3p.qp());
      /* p29.FEPU*/ wire _FEPU_LY4n = not1(lcd_reg.LOVU_LY4p.qp());
      /* p29.FOFA*/ wire _FOFA_LY5n = not1(lcd_reg.LEMA_LY5p.qp());
      /* p29.FEMO*/ wire _FEMO_LY6n = not1(lcd_reg.MATO_LY6p.qp());
      /* p29.GUSU*/ wire _GUSU_LY7n = not1(lcd_reg.LAFO_LY7p.qp());

      /* p29.ERUC*/ wire _ERUC_YDIFF_S0 = add_s(_EBOS_LY0n, oam_bus.XUSO_OAM_DA0p.qp(), GND);
      /* p29.ERUC*/ wire _ERUC_YDIFF_C0 = add_c(_EBOS_LY0n, oam_bus.XUSO_OAM_DA0p.qp(), GND);
      /* p29.ENEF*/ wire _ENEF_YDIFF_S1 = add_s(_DASA_LY1n, oam_bus.XEGU_OAM_DA1p.qp(), _ERUC_YDIFF_C0);
      /* p29.ENEF*/ wire _ENEF_YDIFF_C1 = add_c(_DASA_LY1n, oam_bus.XEGU_OAM_DA1p.qp(), _ERUC_YDIFF_C0);
      /* p29.FECO*/ wire _FECO_YDIFF_S2 = add_s(_FUKY_LY2n, oam_bus.YJEX_OAM_DA2p.qp(), _ENEF_YDIFF_C1);
      /* p29.FECO*/ wire _FECO_YDIFF_C2 = add_c(_FUKY_LY2n, oam_bus.YJEX_OAM_DA2p.qp(), _ENEF_YDIFF_C1);
      /* p29.GYKY*/ wire _GYKY_YDIFF_S3 = add_s(_FUVE_LY3n, oam_bus.XYJU_OAM_DA3p.qp(), _FECO_YDIFF_C2);
      /* p29.GYKY*/ wire _GYKY_YDIFF_C3 = add_c(_FUVE_LY3n, oam_bus.XYJU_OAM_DA3p.qp(), _FECO_YDIFF_C2);
      /* p29.GOPU*/ wire _GOPU_YDIFF_S4 = add_s(_FEPU_LY4n, oam_bus.YBOG_OAM_DA4p.qp(), _GYKY_YDIFF_C3);
      /* p29.GOPU*/ wire _GOPU_YDIFF_C4 = add_c(_FEPU_LY4n, oam_bus.YBOG_OAM_DA4p.qp(), _GYKY_YDIFF_C3);
      /* p29.FUWA*/ wire _FUWA_YDIFF_S5 = add_s(_FOFA_LY5n, oam_bus.WYSO_OAM_DA5p.qp(), _GOPU_YDIFF_C4);
      /* p29.FUWA*/ wire _FUWA_YDIFF_C5 = add_c(_FOFA_LY5n, oam_bus.WYSO_OAM_DA5p.qp(), _GOPU_YDIFF_C4);
      /* p29.GOJU*/ wire _GOJU_YDIFF_S6 = add_s(_FEMO_LY6n, oam_bus.XOTE_OAM_DA6p.qp(), _FUWA_YDIFF_C5);
      /* p29.GOJU*/ wire _GOJU_YDIFF_C6 = add_c(_FEMO_LY6n, oam_bus.XOTE_OAM_DA6p.qp(), _FUWA_YDIFF_C5);
      /* p29.WUHU*/ wire _WUHU_YDIFF_S7 = add_s(_GUSU_LY7n, oam_bus.YZAB_OAM_DA7p.qp(), _GOJU_YDIFF_C6);
      /* p29.WUHU*/ wire _WUHU_YDIFF_C7 = add_c(_GUSU_LY7n, oam_bus.YZAB_OAM_DA7p.qp(), _GOJU_YDIFF_C6);

      /* p29.GACE*/ wire _GACE_SPRITE_DELTA4 = not1(_GOPU_YDIFF_S4);
      /* p29.GUVU*/ wire _GUVU_SPRITE_DELTA5 = not1(_FUWA_YDIFF_S5);
      /* p29.GYDA*/ wire _GYDA_SPRITE_DELTA6 = not1(_GOJU_YDIFF_S6);
      /* p29.GEWY*/ wire _GEWY_SPRITE_DELTA7 = not1(_WUHU_YDIFF_S7);
      /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH  = or2(pix_pipe.XYMO_LCDC_SPSIZEn.qn(), _GYKY_YDIFF_S3);

      /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn = nand6(_GACE_SPRITE_DELTA4, _GUVU_SPRITE_DELTA5, _GYDA_SPRITE_DELTA6, _GEWY_SPRITE_DELTA7, _WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH);

      /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn = and2(sprite_scanner.CENO_SCANNINGp.qn(), pix_pipe.XYMU_RENDERINGn.qn());
      /*#p30.WUZY*/ SPR_TRI_I_out[0].tri6_nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.XADU_SPRITE_IDX0p.qn());
      /* p30.WYSE*/ SPR_TRI_I_out[1].tri6_nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.XEDY_SPRITE_IDX1p.qn());
      /* p30.ZYSU*/ SPR_TRI_I_out[2].tri6_nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.ZUZE_SPRITE_IDX2p.qn());
      /* p30.WYDA*/ SPR_TRI_I_out[3].tri6_nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.XOBE_SPRITE_IDX3p.qn());
      /* p30.WUCO*/ SPR_TRI_I_out[4].tri6_nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.YDUF_SPRITE_IDX4p.qn());
      /* p30.WEZA*/ SPR_TRI_I_out[5].tri6_nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.XECU_SPRITE_IDX5p.qn());

      /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0 = not1(_ERUC_YDIFF_S0);
      /* p29.DABY*/ wire _DABY_SPRITE_DELTA1 = not1(_ENEF_YDIFF_S1);
      /* p29.DABU*/ wire _DABU_SPRITE_DELTA2 = not1(_FECO_YDIFF_S2);
      /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3 = not1(_GYKY_YDIFF_S3);

      /*#p30.CUCU*/ SPR_TRI_L_out[0].tri6_nn(_FEPO_STORE_MATCHp_old, _DEGE_SPRITE_DELTA0);
      /*#p30.CUCA*/ SPR_TRI_L_out[1].tri6_nn(_FEPO_STORE_MATCHp_old, _DABY_SPRITE_DELTA1);
      /*#p30.CEGA*/ SPR_TRI_L_out[2].tri6_nn(_FEPO_STORE_MATCHp_old, _DABU_SPRITE_DELTA2);
      /*#p30.WENU*/ SPR_TRI_L_out[3].tri6_nn(_FEPO_STORE_MATCHp_old, _GYSA_SPRITE_DELTA3);

      /* p29.GESE*/ _GESE_SCAN_MATCH_Yp = not1(_WOTA_SCAN_MATCH_Yn);
    }

    /* p29.CARE*/ wire _CARE_STORE_ENp_ABxxEFxx = and3(_XOCE_xBCxxFGx_new, _CEHA_SCANNINGp_new, _GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /* p29.DYTY*/ wire _DYTY_STORE_ENn_xxCDxxGH = not1(_CARE_STORE_ENp_ABxxEFxx);
    /* p29.DEZY*/ sprite_store.DEZY_STORE_ENn.dff17(_ZEME_AxCxExGx_clk, _XAPO_VID_RSTn_new, _DYTY_STORE_ENn_xxCDxxGH);

    // Sprite store counter. The sprite count clock stops ticking once we have 10 sprites.
    /*#p29.BAKY*/ wire _BAKY_SPRITES_FULL = and2(sprite_store.CUXY_SPRITE_COUNT1.qp(), sprite_store.DYBE_SPRITE_COUNT3.qp());
    /*#p29.CAKE*/ wire _CAKE_CLKp = or2(_BAKY_SPRITES_FULL, sprite_store.DEZY_STORE_ENn.qp());
    /*#p28.AZYB*/ wire _AZYB_LINE_TRIGn = not1(_ATEJ_LINE_TRIGp_new);
    /* p29.BESE*/ sprite_store.BESE_SPRITE_COUNT0.dff17(_CAKE_CLKp,                           _AZYB_LINE_TRIGn, sprite_store.BESE_SPRITE_COUNT0.qn());
    /* p29.CUXY*/ sprite_store.CUXY_SPRITE_COUNT1.dff17(sprite_store.BESE_SPRITE_COUNT0.qn(), _AZYB_LINE_TRIGn, sprite_store.CUXY_SPRITE_COUNT1.qn());
    /* p29.BEGO*/ sprite_store.BEGO_SPRITE_COUNT2.dff17(sprite_store.CUXY_SPRITE_COUNT1.qn(), _AZYB_LINE_TRIGn, sprite_store.BEGO_SPRITE_COUNT2.qn());
    /* p29.DYBE*/ sprite_store.DYBE_SPRITE_COUNT3.dff17(sprite_store.BEGO_SPRITE_COUNT2.qn(), _AZYB_LINE_TRIGn, sprite_store.DYBE_SPRITE_COUNT3.qn());

    /* p29.EDEN*/ wire _EDEN_SPRITE_COUNT0n = not1(sprite_store.BESE_SPRITE_COUNT0.qp());
    /* p29.CYPY*/ wire _CYPY_SPRITE_COUNT1n = not1(sprite_store.CUXY_SPRITE_COUNT1.qp());
    /* p29.CAPE*/ wire _CAPE_SPRITE_COUNT2n = not1(sprite_store.BEGO_SPRITE_COUNT2.qp());
    /* p29.CAXU*/ wire _CAXU_SPRITE_COUNT3n = not1(sprite_store.DYBE_SPRITE_COUNT3.qp());

    /* p29.FYCU*/ wire _FYCU_SPRITE_COUNT0p = not1(_EDEN_SPRITE_COUNT0n);
    /* p29.FONE*/ wire _FONE_SPRITE_COUNT1p = not1(_CYPY_SPRITE_COUNT1n);
    /* p29.EKUD*/ wire _EKUD_SPRITE_COUNT2p = not1(_CAPE_SPRITE_COUNT2n);
    /* p29.ELYG*/ wire _ELYG_SPRITE_COUNT3p = not1(_CAXU_SPRITE_COUNT3n);

    /*#p31.ZAGO*/ wire _ZAGO_SPX0n = not1(oam_bus.YLOR_OAM_DB0p.qp());
    /* p31.ZOCY*/ wire _ZOCY_SPX1n = not1(oam_bus.ZYTY_OAM_DB1p.qp());
    /* p31.YPUR*/ wire _YPUR_SPX2n = not1(oam_bus.ZYVE_OAM_DB2p.qp());
    /* p31.YVOK*/ wire _YVOK_SPX3n = not1(oam_bus.ZEZY_OAM_DB3p.qp());
    /* p31.COSE*/ wire _COSE_SPX4n = not1(oam_bus.GOMO_OAM_DB4p.qp());
    /* p31.AROP*/ wire _AROP_SPX5n = not1(oam_bus.BAXO_OAM_DB5p.qp());
    /* p31.XATU*/ wire _XATU_SPX6n = not1(oam_bus.YZOS_OAM_DB6p.qp());
    /* p31.BADY*/ wire _BADY_SPX7n = not1(oam_bus.DEPO_OAM_DB7p.qp());

    /* p28.ABAK*/ wire _ABAK_LINE_RSTp =  or2(_ATEJ_LINE_TRIGp_new, _AMYG_VID_RSTp_new);
    /* p28.BYVA*/ wire _BYVA_LINE_RSTn = not1(_ABAK_LINE_RSTp);
    /* p29.DYBA*/ wire _DYBA_LINE_RSTp = not1(_BYVA_LINE_RSTn);

    {
      /* p29.DEZO*/ wire _DEZO_STORE0_SELn = nand4(_EDEN_SPRITE_COUNT0n, _CYPY_SPRITE_COUNT1n, _CAPE_SPRITE_COUNT2n, _CAXU_SPRITE_COUNT3n);
      /* p29.CEMY*/ wire CEMY_STORE0_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _DEZO_STORE0_SELn);
      /* p29.DYHU*/ wire DYHU_STORE0_CLKn = not1(CEMY_STORE0_CLKp);      /* p29.GENY*/ wire GENY_STORE0_CLKp = not1(DYHU_STORE0_CLKn);
      /* p29.ENOB*/ wire ENOB_STORE0_CLKp = not1(DYHU_STORE0_CLKn);
      /* p29.FUXU*/ wire FUXU_STORE0_CLKp = not1(DYHU_STORE0_CLKn);

      /* p30.YGUS*/ sprite_store.YGUS_STORE0_I0n.dff8n(GENY_STORE0_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.YSOK*/ sprite_store.YSOK_STORE0_I1n.dff8n(GENY_STORE0_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.YZEP*/ sprite_store.YZEP_STORE0_I2n.dff8n(GENY_STORE0_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.WYTE*/ sprite_store.WYTE_STORE0_I3n.dff8n(GENY_STORE0_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.ZONY*/ sprite_store.ZONY_STORE0_I4n.dff8n(GENY_STORE0_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.YWAK*/ sprite_store.YWAK_STORE0_I5n.dff8n(GENY_STORE0_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.GYHO*/ sprite_store.GYHO_STORE0_L0n.dff8n(ENOB_STORE0_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.CUFO*/ sprite_store.CUFO_STORE0_L1n.dff8n(ENOB_STORE0_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.BOZU*/ sprite_store.BOZU_STORE0_L2n.dff8n(ENOB_STORE0_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.FYHY*/ sprite_store.FYHY_STORE0_L3n.dff8n(ENOB_STORE0_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.EBOJ*/ sprite_store.EBOJ_STORE0_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _GUVA_SPRITE0_GETp_old); // FIXME bad clock/signal
      /* p29.DYWE*/ wire _DYWE_STORE0_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.EBOJ_STORE0_RSTp.qp());
      /* p29.DYNA*/ wire _DYNA_STORE0_RSTn = not1(_DYWE_STORE0_RSTp);

      /*#p31.XEPE*/ sprite_store.XEPE_STORE0_X0p.dff9(FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _ZAGO_SPX0n);
      /* p31.YLAH*/ sprite_store.YLAH_STORE0_X1p.dff9(FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _ZOCY_SPX1n);
      /* p31.ZOLA*/ sprite_store.ZOLA_STORE0_X2p.dff9(FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _YPUR_SPX2n);
      /* p31.ZULU*/ sprite_store.ZULU_STORE0_X3p.dff9(FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _YVOK_SPX3n);
      /* p31.WELO*/ sprite_store.WELO_STORE0_X4p.dff9(FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _COSE_SPX4n);
      /* p31.XUNY*/ sprite_store.XUNY_STORE0_X5p.dff9(FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _AROP_SPX5n);
      /* p31.WOTE*/ sprite_store.WOTE_STORE0_X6p.dff9(FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _XATU_SPX6n);
      /* p31.XAKO*/ sprite_store.XAKO_STORE0_X7p.dff9(FUXU_STORE0_CLKp, _DYNA_STORE0_RSTn, _BADY_SPX7n);
    }

    {

      /* p29.CUVA*/ wire _CUVA_STORE1_SELn = nand4(_FYCU_SPRITE_COUNT0p, _CYPY_SPRITE_COUNT1n, _CAPE_SPRITE_COUNT2n, _CAXU_SPRITE_COUNT3n);
      /* p29.BYBY*/ wire BYBY_STORE1_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _CUVA_STORE1_SELn);
      /* p29.BUCO*/ wire BUCO_STORE1_CLKn = not1(BYBY_STORE1_CLKp);      /* p29.BYVY*/ wire BYVY_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
      /* p29.AHOF*/ wire AHOF_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
      /* p29.ASYS*/ wire ASYS_STORE1_CLKp = not1(BUCO_STORE1_CLKn);

      /* p30.CADU*/ sprite_store.CADU_STORE1_I0n.dff8n(BYVY_STORE1_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.CEBO*/ sprite_store.CEBO_STORE1_I1n.dff8n(BYVY_STORE1_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.CUFA*/ sprite_store.CUFA_STORE1_I2n.dff8n(BYVY_STORE1_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.COMA*/ sprite_store.COMA_STORE1_I3n.dff8n(BYVY_STORE1_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.CUZA*/ sprite_store.CUZA_STORE1_I4n.dff8n(BYVY_STORE1_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.CAJY*/ sprite_store.CAJY_STORE1_I5n.dff8n(BYVY_STORE1_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.AMES*/ sprite_store.AMES_STORE1_L0n.dff8n(AHOF_STORE1_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.AROF*/ sprite_store.AROF_STORE1_L1n.dff8n(AHOF_STORE1_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.ABOP*/ sprite_store.ABOP_STORE1_L2n.dff8n(AHOF_STORE1_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.ABUG*/ sprite_store.ABUG_STORE1_L3n.dff8n(AHOF_STORE1_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.CEDY*/ sprite_store.CEDY_STORE1_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _ENUT_SPRITE1_GETp_old);
      /* p29.EFEV*/ wire _EFEV_STORE1_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.CEDY_STORE1_RSTp.qp());
      /* p29.DOKU*/ wire _DOKU_STORE1_RSTn = not1(_EFEV_STORE1_RSTp);

      /* p31.DANY*/ sprite_store.DANY_STORE1_X0p.dff9(ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _ZAGO_SPX0n);
      /* p31.DUKO*/ sprite_store.DUKO_STORE1_X1p.dff9(ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _ZOCY_SPX1n);
      /* p31.DESU*/ sprite_store.DESU_STORE1_X2p.dff9(ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _YPUR_SPX2n);
      /* p31.DAZO*/ sprite_store.DAZO_STORE1_X3p.dff9(ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _YVOK_SPX3n);
      /* p31.DAKE*/ sprite_store.DAKE_STORE1_X4p.dff9(ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _COSE_SPX4n);
      /* p31.CESO*/ sprite_store.CESO_STORE1_X5p.dff9(ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _AROP_SPX5n);
      /* p31.DYFU*/ sprite_store.DYFU_STORE1_X6p.dff9(ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _XATU_SPX6n);
      /* p31.CUSY*/ sprite_store.CUSY_STORE1_X7p.dff9(ASYS_STORE1_CLKp, _DOKU_STORE1_RSTn, _BADY_SPX7n);

    }

    {

      /* p29.GEBU*/ wire _GEBU_STORE2_SELn = nand4(_EDEN_SPRITE_COUNT0n, _FONE_SPRITE_COUNT1p, _CAPE_SPRITE_COUNT2n, _CAXU_SPRITE_COUNT3n);
      /* p29.WYXO*/ wire WYXO_STORE2_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _GEBU_STORE2_SELn);
      /* p29.GYFO*/ wire GYFO_STORE2_CLKn = not1(WYXO_STORE2_CLKp);
      /* p29.BUZY*/ wire BUZY_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
      /* p29.FUKE*/ wire FUKE_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
      /* p29.CACU*/ wire CACU_STORE2_CLKp = not1(GYFO_STORE2_CLKn);

      /* p30.BUHE*/ sprite_store.BUHE_STORE2_I0n.dff8n(BUZY_STORE2_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.BYHU*/ sprite_store.BYHU_STORE2_I1n.dff8n(BUZY_STORE2_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.BECA*/ sprite_store.BECA_STORE2_I2n.dff8n(BUZY_STORE2_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.BULU*/ sprite_store.BULU_STORE2_I3n.dff8n(BUZY_STORE2_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.BUNA*/ sprite_store.BUNA_STORE2_I4n.dff8n(BUZY_STORE2_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.BOXA*/ sprite_store.BOXA_STORE2_I5n.dff8n(BUZY_STORE2_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.YLOV*/ sprite_store.YLOV_STORE2_L0n.dff8n(FUKE_STORE2_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.XOSY*/ sprite_store.XOSY_STORE2_L1n.dff8n(FUKE_STORE2_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.XAZY*/ sprite_store.XAZY_STORE2_L2n.dff8n(FUKE_STORE2_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.YKUK*/ sprite_store.YKUK_STORE2_L3n.dff8n(FUKE_STORE2_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.EGAV*/ sprite_store.EGAV_STORE2_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _EMOL_SPRITE2_GETp_old);
      /* p29.FOKO*/ wire _FOKO_STORE2_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.EGAV_STORE2_RSTp.qp());
      /* p29.GAMY*/ wire _GAMY_STORE2_RSTn = not1(_FOKO_STORE2_RSTp);

      /* p31.FOKA*/ sprite_store.FOKA_STORE2_X0p.dff9(CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _ZAGO_SPX0n);
      /* p31.FYTY*/ sprite_store.FYTY_STORE2_X1p.dff9(CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _ZOCY_SPX1n);
      /* p31.FUBY*/ sprite_store.FUBY_STORE2_X2p.dff9(CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _YPUR_SPX2n);
      /* p31.GOXU*/ sprite_store.GOXU_STORE2_X3p.dff9(CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _YVOK_SPX3n);
      /* p31.DUHY*/ sprite_store.DUHY_STORE2_X4p.dff9(CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _COSE_SPX4n);
      /* p31.EJUF*/ sprite_store.EJUF_STORE2_X5p.dff9(CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _AROP_SPX5n);
      /* p31.ENOR*/ sprite_store.ENOR_STORE2_X6p.dff9(CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _XATU_SPX6n);
      /* p31.DEPY*/ sprite_store.DEPY_STORE2_X7p.dff9(CACU_STORE2_CLKp, _GAMY_STORE2_RSTn, _BADY_SPX7n);
    }

    {

      /* p29.FOCO*/ wire _FOCO_STORE3_SELn = nand4(_FYCU_SPRITE_COUNT0p, _FONE_SPRITE_COUNT1p, _CAPE_SPRITE_COUNT2n, _CAXU_SPRITE_COUNT3n);
      /* p29.GUVE*/ wire GUVE_STORE3_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _FOCO_STORE3_SELn);
      /* p29.GUSA*/ wire GUSA_STORE3_CLKn = not1(GUVE_STORE3_CLKp);
      /* p29.FEKA*/ wire FEKA_STORE3_CLKp = not1(GUSA_STORE3_CLKn);
      /* p29.XYHA*/ wire XYHA_STORE3_CLKp = not1(GUSA_STORE3_CLKn);
      /* p29.YFAG*/ wire YFAG_STORE3_CLKp = not1(GUSA_STORE3_CLKn);

      /* p30.DEVY*/ sprite_store.DEVY_STORE3_I0n.dff8n(FEKA_STORE3_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.DESE*/ sprite_store.DESE_STORE3_I1n.dff8n(FEKA_STORE3_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.DUNY*/ sprite_store.DUNY_STORE3_I2n.dff8n(FEKA_STORE3_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.DUHA*/ sprite_store.DUHA_STORE3_I3n.dff8n(FEKA_STORE3_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.DEBA*/ sprite_store.DEBA_STORE3_I4n.dff8n(FEKA_STORE3_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.DAFU*/ sprite_store.DAFU_STORE3_I5n.dff8n(FEKA_STORE3_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.ZURO*/ sprite_store.ZURO_STORE3_L0n.dff8n(XYHA_STORE3_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.ZYLU*/ sprite_store.ZYLU_STORE3_L1n.dff8n(XYHA_STORE3_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.ZENE*/ sprite_store.ZENE_STORE3_L2n.dff8n(XYHA_STORE3_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.ZURY*/ sprite_store.ZURY_STORE3_L3n.dff8n(XYHA_STORE3_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.GOTA*/ sprite_store.GOTA_STORE3_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _GYFY_SPRITE3_GETp_old);
      /* p29.GAKE*/ wire _GAKE_STORE3_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.GOTA_STORE3_RSTp.qp());
      /* p29.WUPA*/ wire _WUPA_STORE3_RSTn = not1(_GAKE_STORE3_RSTp);

      /* p31.XOLY*/ sprite_store.XOLY_STORE3_X0p.dff9(YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _ZAGO_SPX0n);
      /* p31.XYBA*/ sprite_store.XYBA_STORE3_X1p.dff9(YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _ZOCY_SPX1n);
      /* p31.XABE*/ sprite_store.XABE_STORE3_X2p.dff9(YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _YPUR_SPX2n);
      /* p31.XEKA*/ sprite_store.XEKA_STORE3_X3p.dff9(YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _YVOK_SPX3n);
      /* p31.XOMY*/ sprite_store.XOMY_STORE3_X4p.dff9(YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _COSE_SPX4n);
      /* p31.WUHA*/ sprite_store.WUHA_STORE3_X5p.dff9(YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _AROP_SPX5n);
      /* p31.WYNA*/ sprite_store.WYNA_STORE3_X6p.dff9(YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _XATU_SPX6n);
      /* p31.WECO*/ sprite_store.WECO_STORE3_X7p.dff9(YFAG_STORE3_CLKp, _WUPA_STORE3_RSTn, _BADY_SPX7n);
    }

    {

      /* p29.CUPE*/ wire _CUPE_STORE4_SELn = nand4(_EDEN_SPRITE_COUNT0n, _CYPY_SPRITE_COUNT1n, _EKUD_SPRITE_COUNT2p, _CAXU_SPRITE_COUNT3n);
      /* p29.CECU*/ wire CECU_STORE4_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _CUPE_STORE4_SELn);
      /* p29.DUKE*/ wire DUKE_STORE4_CLKn = not1(CECU_STORE4_CLKp);
      /* p29.WOFO*/ wire WOFO_STORE4_CLKp = not1(DUKE_STORE4_CLKn);
      /* p29.WYLU*/ wire WYLU_STORE4_CLKp = not1(DUKE_STORE4_CLKn);
      /* p29.EWOT*/ wire EWOT_STORE4_CLKp = not1(DUKE_STORE4_CLKn);

      /* p30.XAVE*/ sprite_store.XAVE_STORE4_I0n.dff8n(WYLU_STORE4_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.XEFE*/ sprite_store.XEFE_STORE4_I1n.dff8n(WYLU_STORE4_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.WANU*/ sprite_store.WANU_STORE4_I2n.dff8n(WYLU_STORE4_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.XABO*/ sprite_store.XABO_STORE4_I3n.dff8n(WYLU_STORE4_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.XEGE*/ sprite_store.XEGE_STORE4_I4n.dff8n(WYLU_STORE4_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.XYNU*/ sprite_store.XYNU_STORE4_I5n.dff8n(WYLU_STORE4_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.CAPO*/ sprite_store.CAPO_STORE4_L0n.dff8n(EWOT_STORE4_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.CAJU*/ sprite_store.CAJU_STORE4_L1n.dff8n(EWOT_STORE4_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.CONO*/ sprite_store.CONO_STORE4_L2n.dff8n(EWOT_STORE4_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.CUMU*/ sprite_store.CUMU_STORE4_L3n.dff8n(EWOT_STORE4_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.XUDY*/ sprite_store.XUDY_STORE4_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _GONO_SPRITE4_GETp_old);
      /* p29.WOHU*/ wire _WOHU_STORE4_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.XUDY_STORE4_RSTp.qp());
      /* p29.WUNU*/ wire _WUNU_STORE4_RSTn = not1(_WOHU_STORE4_RSTp);

      /* p31.WEDU*/ sprite_store.WEDU_STORE4_X0p.dff9(WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _ZAGO_SPX0n);
      /* p31.YGAJ*/ sprite_store.YGAJ_STORE4_X1p.dff9(WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _ZOCY_SPX1n);
      /* p31.ZYJO*/ sprite_store.ZYJO_STORE4_X2p.dff9(WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _YPUR_SPX2n);
      /* p31.XURY*/ sprite_store.XURY_STORE4_X3p.dff9(WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _YVOK_SPX3n);
      /* p31.YBED*/ sprite_store.YBED_STORE4_X4p.dff9(WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _COSE_SPX4n);
      /* p31.ZALA*/ sprite_store.ZALA_STORE4_X5p.dff9(WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _AROP_SPX5n);
      /* p31.WYDE*/ sprite_store.WYDE_STORE4_X6p.dff9(WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _XATU_SPX6n);
      /* p31.XEPA*/ sprite_store.XEPA_STORE4_X7p.dff9(WOFO_STORE4_CLKp, _WUNU_STORE4_RSTn, _BADY_SPX7n);
    }

    {

      /* p29.CUGU*/ wire _CUGU_STORE5_SELn = nand4(_FYCU_SPRITE_COUNT0p, _CYPY_SPRITE_COUNT1n, _EKUD_SPRITE_COUNT2p, _CAXU_SPRITE_COUNT3n);
      /* p29.CADO*/ wire CADO_STORE5_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _CUGU_STORE5_SELn);
      /* p29.BEDE*/ wire BEDE_STORE5_CLKn = not1(CADO_STORE5_CLKp);
      /* p29.CYLA*/ wire CYLA_STORE5_CLKp = not1(BEDE_STORE5_CLKn);
      /* p29.DYMO*/ wire DYMO_STORE5_CLKp = not1(BEDE_STORE5_CLKn);
      /* p29.BUCY*/ wire BUCY_STORE5_CLKp = not1(BEDE_STORE5_CLKn);

      /* p30.EKOP*/ sprite_store.EKOP_STORE5_I0n.dff8n(DYMO_STORE5_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.ETYM*/ sprite_store.ETYM_STORE5_I1n.dff8n(DYMO_STORE5_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.GORU*/ sprite_store.GORU_STORE5_I2n.dff8n(DYMO_STORE5_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.EBEX*/ sprite_store.EBEX_STORE5_I3n.dff8n(DYMO_STORE5_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.ETAV*/ sprite_store.ETAV_STORE5_I4n.dff8n(DYMO_STORE5_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.EKAP*/ sprite_store.EKAP_STORE5_I5n.dff8n(DYMO_STORE5_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.ACEP*/ sprite_store.ACEP_STORE5_L0n.dff8n(BUCY_STORE5_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.ABEG*/ sprite_store.ABEG_STORE5_L1n.dff8n(BUCY_STORE5_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.ABUX*/ sprite_store.ABUX_STORE5_L2n.dff8n(BUCY_STORE5_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.ANED*/ sprite_store.ANED_STORE5_L3n.dff8n(BUCY_STORE5_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.WAFY*/ sprite_store.WAFY_STORE5_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _GEGA_SPRITE5_GETp_old);
      /* p29.FEVE*/ wire _FEVE_STORE5_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.WAFY_STORE5_RSTp.qp());
      /* p29.EJAD*/ wire _EJAD_STORE5_RSTn = not1(_FEVE_STORE5_RSTp);

      /* p31.FUSA*/ sprite_store.FUSA_STORE5_X0p.dff9(CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _ZAGO_SPX0n);
      /* p31.FAXA*/ sprite_store.FAXA_STORE5_X1p.dff9(CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _ZOCY_SPX1n);
      /* p31.FOZY*/ sprite_store.FOZY_STORE5_X2p.dff9(CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _YPUR_SPX2n);
      /* p31.FESY*/ sprite_store.FESY_STORE5_X3p.dff9(CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _YVOK_SPX3n);
      /* p31.CYWE*/ sprite_store.CYWE_STORE5_X4p.dff9(CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _COSE_SPX4n);
      /* p31.DYBY*/ sprite_store.DYBY_STORE5_X5p.dff9(CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _AROP_SPX5n);
      /* p31.DURY*/ sprite_store.DURY_STORE5_X6p.dff9(CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _XATU_SPX6n);
      /* p31.CUVY*/ sprite_store.CUVY_STORE5_X7p.dff9(CYLA_STORE5_CLKp, _EJAD_STORE5_RSTn, _BADY_SPX7n);
    }

    {

      /* p29.WOMU*/ wire _WOMU_STORE6_SELn = nand4(_EDEN_SPRITE_COUNT0n, _FONE_SPRITE_COUNT1p, _EKUD_SPRITE_COUNT2p, _CAXU_SPRITE_COUNT3n);
      /* p29.XUJO*/ wire XUJO_STORE6_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _WOMU_STORE6_SELn);
      /* p29.WEKA*/ wire WEKA_STORE6_CLKn = not1(XUJO_STORE6_CLKp);
      /* p29.ZAPE*/ wire ZAPE_STORE6_CLKp = not1(WEKA_STORE6_CLKn);
      /* p29.WUSE*/ wire WUSE_STORE6_CLKp = not1(WEKA_STORE6_CLKn);
      /* p29.ZURU*/ wire ZURU_STORE6_CLKp = not1(WEKA_STORE6_CLKn);

      /* p30.GABO*/ sprite_store.GABO_STORE6_I0n.dff8n(WUSE_STORE6_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.GACY*/ sprite_store.GACY_STORE6_I1n.dff8n(WUSE_STORE6_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.FOGO*/ sprite_store.FOGO_STORE6_I2n.dff8n(WUSE_STORE6_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.GOHU*/ sprite_store.GOHU_STORE6_I3n.dff8n(WUSE_STORE6_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.FOXY*/ sprite_store.FOXY_STORE6_I4n.dff8n(WUSE_STORE6_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.GECU*/ sprite_store.GECU_STORE6_I5n.dff8n(WUSE_STORE6_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.ZUMY*/ sprite_store.ZUMY_STORE6_L0n.dff8n(ZURU_STORE6_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.ZAFU*/ sprite_store.ZAFU_STORE6_L1n.dff8n(ZURU_STORE6_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.ZEXO*/ sprite_store.ZEXO_STORE6_L2n.dff8n(ZURU_STORE6_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.ZUBE*/ sprite_store.ZUBE_STORE6_L3n.dff8n(ZURU_STORE6_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.WOMY*/ sprite_store.WOMY_STORE6_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _XOJA_SPRITE6_GETp_old);
      /* p29.WACY*/ wire _WACY_STORE6_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.WOMY_STORE6_RSTp.qp());
      /* p29.XAHO*/ wire _XAHO_STORE6_RSTn = not1(_WACY_STORE6_RSTp);

      /* p31.YCOL*/ sprite_store.YCOL_STORE6_X0p.dff9(ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _ZAGO_SPX0n);
      /* p31.YRAC*/ sprite_store.YRAC_STORE6_X1p.dff9(ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _ZOCY_SPX1n);
      /* p31.YMEM*/ sprite_store.YMEM_STORE6_X2p.dff9(ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _YPUR_SPX2n);
      /* p31.YVAG*/ sprite_store.YVAG_STORE6_X3p.dff9(ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _YVOK_SPX3n);
      /* p31.ZOLY*/ sprite_store.ZOLY_STORE6_X4p.dff9(ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _COSE_SPX4n);
      /* p31.ZOGO*/ sprite_store.ZOGO_STORE6_X5p.dff9(ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _AROP_SPX5n);
      /* p31.ZECU*/ sprite_store.ZECU_STORE6_X6p.dff9(ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _XATU_SPX6n);
      /* p31.ZESA*/ sprite_store.ZESA_STORE6_X7p.dff9(ZAPE_STORE6_CLKp, _XAHO_STORE6_RSTn, _BADY_SPX7n);
    }

    {

      /* p29.GUNA*/ wire _GUNA_STORE7_SELn = nand4(_FYCU_SPRITE_COUNT0p, _FONE_SPRITE_COUNT1p, _EKUD_SPRITE_COUNT2p, _CAXU_SPRITE_COUNT3n);
      /* p29.GAPE*/ wire GAPE_STORE7_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _GUNA_STORE7_SELn);
      /* p29.GYVO*/ wire GYVO_STORE7_CLKn = not1(GAPE_STORE7_CLKp);
      /* p29.GECY*/ wire GECY_STORE7_CLKp = not1(GYVO_STORE7_CLKn);
      /* p29.FEFO*/ wire FEFO_STORE7_CLKp = not1(GYVO_STORE7_CLKn);
      /* p29.WABE*/ wire WABE_STORE7_CLKp = not1(GYVO_STORE7_CLKn);

      /* p30.GULE*/ sprite_store.GULE_STORE7_I0n.dff8n(FEFO_STORE7_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.GYNO*/ sprite_store.GYNO_STORE7_I1n.dff8n(FEFO_STORE7_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.FEFA*/ sprite_store.FEFA_STORE7_I2n.dff8n(FEFO_STORE7_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.FYSU*/ sprite_store.FYSU_STORE7_I3n.dff8n(FEFO_STORE7_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.GESY*/ sprite_store.GESY_STORE7_I4n.dff8n(FEFO_STORE7_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.FUZO*/ sprite_store.FUZO_STORE7_I5n.dff8n(FEFO_STORE7_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.XYNA*/ sprite_store.XYNA_STORE7_L0n.dff8n(WABE_STORE7_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.YGUM*/ sprite_store.YGUM_STORE7_L1n.dff8n(WABE_STORE7_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.XAKU*/ sprite_store.XAKU_STORE7_L2n.dff8n(WABE_STORE7_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.XYGO*/ sprite_store.XYGO_STORE7_L3n.dff8n(WABE_STORE7_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.WAPO*/ sprite_store.WAPO_STORE7_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _GUTU_SPRITE7_GETp_old);
      /* p29.GUKY*/ wire _GUKY_STORE7_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.WAPO_STORE7_RSTp.qp());
      /* p29.GAFY*/ wire _GAFY_STORE7_RSTn = not1(_GUKY_STORE7_RSTp);

      /* p31.ERAZ*/ sprite_store.ERAZ_STORE7_X0p.dff9(GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _ZAGO_SPX0n);
      /* p31.EPUM*/ sprite_store.EPUM_STORE7_X1p.dff9(GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _ZOCY_SPX1n);
      /* p31.EROL*/ sprite_store.EROL_STORE7_X2p.dff9(GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _YPUR_SPX2n);
      /* p31.EHYN*/ sprite_store.EHYN_STORE7_X3p.dff9(GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _YVOK_SPX3n);
      /* p31.FAZU*/ sprite_store.FAZU_STORE7_X4p.dff9(GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _COSE_SPX4n);
      /* p31.FAXE*/ sprite_store.FAXE_STORE7_X5p.dff9(GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _AROP_SPX5n);
      /* p31.EXUK*/ sprite_store.EXUK_STORE7_X6p.dff9(GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _XATU_SPX6n);
      /* p31.FEDE*/ sprite_store.FEDE_STORE7_X7p.dff9(GECY_STORE7_CLKp, _GAFY_STORE7_RSTn, _BADY_SPX7n);
    }

    {

      /* p29.DEWY*/ wire _DEWY_STORE8_SELn = nand4(_EDEN_SPRITE_COUNT0n, _CYPY_SPRITE_COUNT1n, _CAPE_SPRITE_COUNT2n, _ELYG_SPRITE_COUNT3p);
      /* p29.CAHO*/ wire _CAHO_STORE8_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _DEWY_STORE8_SELn);
      /* p29.BUKA*/ wire _BUKA_STORE8_CLKn = not1(_CAHO_STORE8_CLKp);
      /* p29.CEXU*/ wire _CEXU_STORE8_CLKp = not1(_BUKA_STORE8_CLKn);
      /* p29.AKOL*/ wire _AKOL_STORE8_CLKp = not1(_BUKA_STORE8_CLKn);
      /* p29.BYMY*/ wire _BYMY_STORE8_CLKp = not1(_BUKA_STORE8_CLKn);

      /* p30.AXUV*/ sprite_store.AXUV_STORE8_I0n.dff8n(_AKOL_STORE8_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.BADA*/ sprite_store.BADA_STORE8_I1n.dff8n(_AKOL_STORE8_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.APEV*/ sprite_store.APEV_STORE8_I2n.dff8n(_AKOL_STORE8_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.BADO*/ sprite_store.BADO_STORE8_I3n.dff8n(_AKOL_STORE8_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.BEXY*/ sprite_store.BEXY_STORE8_I4n.dff8n(_AKOL_STORE8_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.BYHE*/ sprite_store.BYHE_STORE8_I5n.dff8n(_AKOL_STORE8_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.AZAP*/ sprite_store.AZAP_STORE8_L0n.dff8n(_BYMY_STORE8_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.AFYX*/ sprite_store.AFYX_STORE8_L1n.dff8n(_BYMY_STORE8_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.AFUT*/ sprite_store.AFUT_STORE8_L2n.dff8n(_BYMY_STORE8_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.AFYM*/ sprite_store.AFYM_STORE8_L3n.dff8n(_BYMY_STORE8_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.EXUQ*/ sprite_store.EXUQ_STORE8_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _FOXA_SPRITE8_GETp_old);
      /* p29.GORO*/ wire _GORO_STORE8_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.EXUQ_STORE8_RSTp.qp());
      /* p29.WUZO*/ wire _WUZO_STORE8_RSTn = not1(_GORO_STORE8_RSTp);

      /* p31.EZUF*/ sprite_store.EZUF_STORE8_X0p.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _COSE_SPX4n);
      /* p31.ENAD*/ sprite_store.ENAD_STORE8_X1p.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _AROP_SPX5n);
      /* p31.EBOW*/ sprite_store.EBOW_STORE8_X2p.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _XATU_SPX6n);
      /* p31.FYCA*/ sprite_store.FYCA_STORE8_X3p.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _BADY_SPX7n);
      /* p31.GAVY*/ sprite_store.GAVY_STORE8_X4p.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _ZAGO_SPX0n);
      /* p31.GYPU*/ sprite_store.GYPU_STORE8_X5p.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _ZOCY_SPX1n);
      /* p31.GADY*/ sprite_store.GADY_STORE8_X6p.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _YPUR_SPX2n);
      /* p31.GAZA*/ sprite_store.GAZA_STORE8_X7p.dff9(_CEXU_STORE8_CLKp, _WUZO_STORE8_RSTn, _YVOK_SPX3n);
    }

    {
      /* p29.DOGU*/ wire _DOGU_STORE9_SELn = nand4(_FYCU_SPRITE_COUNT0p, _CYPY_SPRITE_COUNT1n, _CAPE_SPRITE_COUNT2n, _ELYG_SPRITE_COUNT3p);
      /* p29.CATO*/ wire _CATO_STORE9_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, _DOGU_STORE9_SELn);
      /* p29.DECU*/ wire _DECU_STORE9_CLKn = not1(_CATO_STORE9_CLKp);
      /* p29.WEME*/ wire _WEME_STORE9_CLKp = not1(_DECU_STORE9_CLKn);
      /* p29.WUFA*/ wire _WUFA_STORE9_CLKp = not1(_DECU_STORE9_CLKn);
      /* p29.FAKA*/ wire _FAKA_STORE9_CLKp = not1(_DECU_STORE9_CLKn);

      /* p30.YBER*/ sprite_store.YBER_STORE9_I0n.dff8n(_WUFA_STORE9_CLKp, SPR_TRI_I_out[0].qp());
      /* p30.YZOR*/ sprite_store.YZOR_STORE9_I1n.dff8n(_WUFA_STORE9_CLKp, SPR_TRI_I_out[1].qp());
      /* p30.XYFE*/ sprite_store.XYFE_STORE9_I2n.dff8n(_WUFA_STORE9_CLKp, SPR_TRI_I_out[2].qp());
      /* p30.XOTU*/ sprite_store.XOTU_STORE9_I3n.dff8n(_WUFA_STORE9_CLKp, SPR_TRI_I_out[3].qp());
      /* p30.XUTE*/ sprite_store.XUTE_STORE9_I4n.dff8n(_WUFA_STORE9_CLKp, SPR_TRI_I_out[4].qp());
      /* p30.XUFO*/ sprite_store.XUFO_STORE9_I5n.dff8n(_WUFA_STORE9_CLKp, SPR_TRI_I_out[5].qp());
      /* p30.CANA*/ sprite_store.CANA_STORE9_L0n.dff8n(_FAKA_STORE9_CLKp, SPR_TRI_L_out[0].qp());
      /* p30.FOFO*/ sprite_store.FOFO_STORE9_L1n.dff8n(_FAKA_STORE9_CLKp, SPR_TRI_L_out[1].qp());
      /* p30.DYSY*/ sprite_store.DYSY_STORE9_L2n.dff8n(_FAKA_STORE9_CLKp, SPR_TRI_L_out[2].qp());
      /* p30.DEWU*/ sprite_store.DEWU_STORE9_L3n.dff8n(_FAKA_STORE9_CLKp, SPR_TRI_L_out[3].qp());

      /* p29.FONO*/ sprite_store.FONO_STORE9_RSTp.dff17(_WUTY_SPRITE_DONEp, _BYVA_LINE_RSTn, _GUZE_SPRITE9_GETp_old);
      /* p29.DUBU*/ wire _DUBU_STORE9_RSTp = or2(_DYBA_LINE_RSTp, sprite_store.FONO_STORE9_RSTp.qp());
      /* p29.DOSY*/ wire _DOSY_STORE9_RSTn = not1(_DUBU_STORE9_RSTp);

      /* p31.XUVY*/ sprite_store.XUVY_STORE9_X0p.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _ZAGO_SPX0n);
      /* p31.XERE*/ sprite_store.XERE_STORE9_X1p.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _ZOCY_SPX1n);
      /* p31.XUZO*/ sprite_store.XUZO_STORE9_X2p.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _YPUR_SPX2n);
      /* p31.XEXA*/ sprite_store.XEXA_STORE9_X3p.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _YVOK_SPX3n);
      /* p31.YPOD*/ sprite_store.YPOD_STORE9_X4p.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _COSE_SPX4n);
      /* p31.YROP*/ sprite_store.YROP_STORE9_X5p.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _AROP_SPX5n);
      /* p31.YNEP*/ sprite_store.YNEP_STORE9_X6p.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _XATU_SPX6n);
      /* p31.YZOF*/ sprite_store.YZOF_STORE9_X7p.dff9(_WEME_STORE9_CLKp, _DOSY_STORE9_RSTn, _BADY_SPX7n);
    }
  }

  //----------------------------------------
  // Ext pins

  Pin2 PIN_EXT_D_in[8];

  Pin2 PIN_EXT_A[16];
  {
    /* p08.LOXO*/ wire _LOXO_HOLDn = and_or3(_MULE_MODE_DBG1n_new, _TEXO_8000_9FFFn_new, _UMUT_MODE_DBG1p_new);
    /* p08.LASY*/ wire _LASY_HOLDp = not1(_LOXO_HOLDn);
    /* p08.MATE*/ wire _MATE_HOLDn = not1(_LASY_HOLDp);
    /* p08.ALOR*/ ext_bus.ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 0]);
    /* p08.APUR*/ ext_bus.APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 1]);
    /* p08.ALYR*/ ext_bus.ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 2]);
    /* p08.ARET*/ ext_bus.ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 3]);
    /* p08.AVYS*/ ext_bus.AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 4]);
    /* p08.ATEV*/ ext_bus.ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 5]);
    /* p08.AROS*/ ext_bus.AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 6]);
    /* p08.ARYM*/ ext_bus.ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 7]);
    /* p08.LUNO*/ ext_bus.LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 8]);
    /* p08.LYSA*/ ext_bus.LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 9]);
    /* p08.PATE*/ ext_bus.PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn, BUS_CPU_A[10]);
    /* p08.LUMY*/ ext_bus.LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn, BUS_CPU_A[11]);
    /* p08.LOBU*/ ext_bus.LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn, BUS_CPU_A[12]);
    /* p08.LONU*/ ext_bus.LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn, BUS_CPU_A[13]);
    /* p08.NYRE*/ ext_bus.NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn, BUS_CPU_A[14]);

    /* p08.AMET*/ wire _AMET_A00p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NAKY_DMA_A00p.qp(), ext_bus.ALOR_EXT_ADDR_LATCH_00p.qp());
    /* p08.ATOL*/ wire _ATOL_A01p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PYRO_DMA_A01p.qp(), ext_bus.APUR_EXT_ADDR_LATCH_01p.qp());
    /* p08.APOK*/ wire _APOK_A02p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NEFY_DMA_A02p.qp(), ext_bus.ALYR_EXT_ADDR_LATCH_02p.qp());
    /* p08.AMER*/ wire _AMER_A03p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MUTY_DMA_A03p.qp(), ext_bus.ARET_EXT_ADDR_LATCH_03p.qp());
    /* p08.ATEM*/ wire _ATEM_A04p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NYKO_DMA_A04p.qp(), ext_bus.AVYS_EXT_ADDR_LATCH_04p.qp());
    /* p08.ATOV*/ wire _ATOV_A05p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PYLO_DMA_A05p.qp(), ext_bus.ATEV_EXT_ADDR_LATCH_05p.qp());
    /* p08.ATYR*/ wire _ATYR_A06p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NUTO_DMA_A06p.qp(), ext_bus.AROS_EXT_ADDR_LATCH_06p.qp());
    /*#p08.ASUR*/ wire _ASUR_A07p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MUGU_DMA_A07p.qp(), ext_bus.ARYM_EXT_ADDR_LATCH_07p.qp());
    /*#p08.MANO*/ wire _MANO_A08p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NAFA_DMA_A08n.qn(), ext_bus.LUNO_EXT_ADDR_LATCH_08p.qp());
    /* p08.MASU*/ wire _MASU_A09p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PYNE_DMA_A09n.qn(), ext_bus.LYSA_EXT_ADDR_LATCH_09p.qp());
    /* p08.PAMY*/ wire _PAMY_A10p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PARA_DMA_A10n.qn(), ext_bus.PATE_EXT_ADDR_LATCH_10p.qp());
    /* p08.MALE*/ wire _MALE_A11p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NYDO_DMA_A11n.qn(), ext_bus.LUMY_EXT_ADDR_LATCH_11p.qp());
    /* p08.MOJY*/ wire _MOJY_A12p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.NYGY_DMA_A12n.qn(), ext_bus.LOBU_EXT_ADDR_LATCH_12p.qp());
    /* p08.MUCE*/ wire _MUCE_A13p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.PULA_DMA_A13n.qn(), ext_bus.LONU_EXT_ADDR_LATCH_13p.qp());
    /* p08.PEGE*/ wire _PEGE_A14p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.POKU_DMA_A14n.qn(), ext_bus.NYRE_EXT_ADDR_LATCH_14p.qp());

    /* p08.KUPO*/ wire _KUPO = nand2(_AMET_A00p, _TOVA_MODE_DBG2n_new);
    /* p08.CABA*/ wire _CABA = nand2(_ATOL_A01p, _TOVA_MODE_DBG2n_new);
    /* p08.BOKU*/ wire _BOKU = nand2(_APOK_A02p, _TOVA_MODE_DBG2n_new);
    /* p08.BOTY*/ wire _BOTY = nand2(_AMER_A03p, _TOVA_MODE_DBG2n_new);
    /* p08.BYLA*/ wire _BYLA = nand2(_ATEM_A04p, _TOVA_MODE_DBG2n_new);
    /* p08.BADU*/ wire _BADU = nand2(_ATOV_A05p, _TOVA_MODE_DBG2n_new);
    /* p08.CEPU*/ wire _CEPU = nand2(_ATYR_A06p, _TOVA_MODE_DBG2n_new);
    /* p08.DEFY*/ wire _DEFY = nand2(_ASUR_A07p, _TOVA_MODE_DBG2n_new);
    /* p08.MYNY*/ wire _MYNY = nand2(_MANO_A08p, _TOVA_MODE_DBG2n_new);
    /* p08.MUNE*/ wire _MUNE = nand2(_MASU_A09p, _TOVA_MODE_DBG2n_new);
    /* p08.ROXU*/ wire _ROXU = nand2(_PAMY_A10p, _TOVA_MODE_DBG2n_new);
    /* p08.LEPY*/ wire _LEPY = nand2(_MALE_A11p, _TOVA_MODE_DBG2n_new);
    /* p08.LUCE*/ wire _LUCE = nand2(_MOJY_A12p, _TOVA_MODE_DBG2n_new);
    /* p08.LABE*/ wire _LABE = nand2(_MUCE_A13p, _TOVA_MODE_DBG2n_new);
    /* p08.PUHE*/ wire _PUHE = nand2(_PEGE_A14p, _TOVA_MODE_DBG2n_new);

    /* p08.KOTY*/ wire _KOTY = nor2 (_AMET_A00p, _UNOR_MODE_DBG2p_new);
    /* p08.COTU*/ wire _COTU = nor2 (_ATOL_A01p, _UNOR_MODE_DBG2p_new);
    /* p08.BAJO*/ wire _BAJO = nor2 (_APOK_A02p, _UNOR_MODE_DBG2p_new);
    /* p08.BOLA*/ wire _BOLA = nor2 (_AMER_A03p, _UNOR_MODE_DBG2p_new);
    /* p08.BEVO*/ wire _BEVO = nor2 (_ATEM_A04p, _UNOR_MODE_DBG2p_new);
    /* p08.AJAV*/ wire _AJAV = nor2 (_ATOV_A05p, _UNOR_MODE_DBG2p_new);
    /* p08.CYKA*/ wire _CYKA = nor2 (_ATYR_A06p, _UNOR_MODE_DBG2p_new);
    /* p08.COLO*/ wire _COLO = nor2 (_ASUR_A07p, _UNOR_MODE_DBG2p_new);
    /* p08.MEGO*/ wire _MEGO = nor2 (_MANO_A08p, _UNOR_MODE_DBG2p_new);
    /* p08.MENY*/ wire _MENY = nor2 (_MASU_A09p, _UNOR_MODE_DBG2p_new);
    /* p08.RORE*/ wire _RORE = nor2 (_PAMY_A10p, _UNOR_MODE_DBG2p_new);
    /* p08.LYNY*/ wire _LYNY = nor2 (_MALE_A11p, _UNOR_MODE_DBG2p_new);
    /* p08.LOSO*/ wire _LOSO = nor2 (_MOJY_A12p, _UNOR_MODE_DBG2p_new);
    /* p08.LEVA*/ wire _LEVA = nor2 (_MUCE_A13p, _UNOR_MODE_DBG2p_new);
    /* p08.PAHY*/ wire _PAHY = nor2 (_PEGE_A14p, _UNOR_MODE_DBG2p_new);

    PIN_EXT_A[ 0].pin_out(1, _KUPO, _KOTY);
    PIN_EXT_A[ 1].pin_out(1, _CABA, _COTU);
    PIN_EXT_A[ 2].pin_out(1, _BOKU, _BAJO);
    PIN_EXT_A[ 3].pin_out(1, _BOTY, _BOLA);
    PIN_EXT_A[ 4].pin_out(1, _BYLA, _BEVO);
    PIN_EXT_A[ 5].pin_out(1, _BADU, _AJAV);
    PIN_EXT_A[ 6].pin_out(1, _CEPU, _CYKA);
    PIN_EXT_A[ 7].pin_out(1, _DEFY, _COLO);
    PIN_EXT_A[ 8].pin_out(1, _MYNY, _MEGO);
    PIN_EXT_A[ 9].pin_out(1, _MUNE, _MENY);
    PIN_EXT_A[10].pin_out(1, _ROXU, _RORE);
    PIN_EXT_A[11].pin_out(1, _LEPY, _LYNY);
    PIN_EXT_A[12].pin_out(1, _LUCE, _LOSO);
    PIN_EXT_A[13].pin_out(1, _LABE, _LEVA);
    PIN_EXT_A[14].pin_out(1, _PUHE, _PAHY);

    // A15 is "special"
    /* p08.SOBY*/ wire _SOBY_A15n = nor2(BUS_CPU_A[15], _TUTU_ADDR_BOOTp_new);
    /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx = nand2(_ABUZ_xxCDEFGH_new, _SOBY_A15n);
    /* p08.TAZY*/ wire _TAZY_A15p = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MARU_DMA_A15n.qn(), _SEPY_A15p_ABxxxxxx);
    /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n = nand2(_TAZY_A15p, _RYCA_MODE_DBG2n_new);
    /* p08.RULO*/ wire _RULO_PIN_EXT_A15n = nor2 (_TAZY_A15p, _UNOR_MODE_DBG2p_new);

    PIN_EXT_A[15].pin_out(1, _SUZE_PIN_EXT_A15n, _RULO_PIN_EXT_A15n);
  }

  Pin2 PIN_EXT_CLK;    // PIN_75
  {
    PIN_EXT_CLK.pin_out(1, _BUDE_xxxxEFGH_clk, _BUDE_xxxxEFGH_clk);
  }

  Pin2 PIN_EXT_RDn;    // PIN_79 // RDn idles low, goes high on phase B for an external write
  {
    /* p08.TYMU*/ wire _TYMU_EXT_RDn = nor2(_LUMA_DMA_CARTp_new, _MOTY_CPU_EXT_RD_new);
    /* p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMU_EXT_RDn, _TOVA_MODE_DBG2n_new);
    /* p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMU_EXT_RDn, _UNOR_MODE_DBG2p_new);
    PIN_EXT_RDn.pin_out(1, _UGAC_RD_A, _URUN_RD_D);
  }

  Pin2 PIN_EXT_WRn;    // PIN_78 // WRn idles high, goes low during EFG if there's a write
  {
    /* p08.MEXO*/ wire _MEXO_CPU_WRn_ABCDxxxH = not1(_APOV_CPU_WRp_xxxxEFGx_new);
    /* p08.NEVY*/ wire _NEVY = or2(_MEXO_CPU_WRn_ABCDxxxH, _MOCA_DBG_EXT_RD_new);
    /* p08.PUVA*/ wire _PUVA_EXT_WRn = or2(_NEVY, _LUMA_DMA_CARTp_new);
    /* p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_WRn, _TOVA_MODE_DBG2n_new);
    /* p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_WRn, _UNOR_MODE_DBG2p_new);
    PIN_EXT_WRn.pin_out(1, _UVER_WR_A, _USUF_WR_D);
  }

  Pin2 PIN_EXT_CSn;    // PIN_80 // CS changes on phase C if addr in [A000,FDFF]
  {
    /* p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH = and3(_ABUZ_xxCDEFGH_new, _TYNU_ADDR_RAM, _TUNA_0000_FDFFp);
    /* p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH = mux2p(_LUMA_DMA_CARTp_new, dma_reg.MARU_DMA_A15n.qn(), _TOZA_PIN_EXT_CS_A_xxCDEFGH);
    PIN_EXT_CSn.pin_out(1, _TYHO_PIN_EXT_CS_A_xxCDEFGH, _TYHO_PIN_EXT_CS_A_xxCDEFGH);
  }

  uint16_t ext_addr = pack_u16p_new(16, &PIN_EXT_A[ 0]);
  {
    // ROM read
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire rom_OEn = PIN_EXT_RDn.qp();
    wire rom_CEn = PIN_EXT_A[15].qp();
    wire rom_OEp = !rom_CEn && !rom_OEn && cart_buf;

    PIN_EXT_D_in[0].pin_in(rom_OEp, cart_buf[rom_addr] & 0x01);
    PIN_EXT_D_in[1].pin_in(rom_OEp, cart_buf[rom_addr] & 0x02);
    PIN_EXT_D_in[2].pin_in(rom_OEp, cart_buf[rom_addr] & 0x04);
    PIN_EXT_D_in[3].pin_in(rom_OEp, cart_buf[rom_addr] & 0x08);
    PIN_EXT_D_in[4].pin_in(rom_OEp, cart_buf[rom_addr] & 0x10);
    PIN_EXT_D_in[5].pin_in(rom_OEp, cart_buf[rom_addr] & 0x20);
    PIN_EXT_D_in[6].pin_in(rom_OEp, cart_buf[rom_addr] & 0x40);
    PIN_EXT_D_in[7].pin_in(rom_OEp, cart_buf[rom_addr] & 0x80);
  }
  {
    // Ext RAM read
    uint16_t eram_addr = (ext_addr & 0x1FFF);
    wire eram_CE1n = PIN_EXT_CSn.qp();
    wire eram_CE2  = PIN_EXT_A[14].qp();
    wire eram_WRn  = PIN_EXT_WRn.qp();
    wire eram_OEn  = PIN_EXT_RDn.qp();
    wire eram_OEp  = !eram_CE1n && eram_CE2 && eram_WRn && !eram_OEn;

    PIN_EXT_D_in[0].pin_in(eram_OEp, ext_ram[eram_addr] & 0x01);
    PIN_EXT_D_in[1].pin_in(eram_OEp, ext_ram[eram_addr] & 0x02);
    PIN_EXT_D_in[2].pin_in(eram_OEp, ext_ram[eram_addr] & 0x04);
    PIN_EXT_D_in[3].pin_in(eram_OEp, ext_ram[eram_addr] & 0x08);
    PIN_EXT_D_in[4].pin_in(eram_OEp, ext_ram[eram_addr] & 0x10);
    PIN_EXT_D_in[5].pin_in(eram_OEp, ext_ram[eram_addr] & 0x20);
    PIN_EXT_D_in[6].pin_in(eram_OEp, ext_ram[eram_addr] & 0x40);
    PIN_EXT_D_in[7].pin_in(eram_OEp, ext_ram[eram_addr] & 0x80);
  }
  {
    // Cart RAM read
    uint16_t cram_addr = (ext_addr & 0x1FFF);
    wire cram_CS1n = PIN_EXT_CSn.qp();
    wire cram_CS2  = PIN_EXT_A[13].qp() && !PIN_EXT_A[14].qp() && PIN_EXT_A[15].qp();
    wire cram_OEn  = PIN_EXT_RDn.qp();
    wire cram_OEp  = !cram_CS1n && cram_CS2 && !cram_OEn;

    PIN_EXT_D_in[0].pin_in(cram_OEp, cart_ram[cram_addr] & 0x01);
    PIN_EXT_D_in[1].pin_in(cram_OEp, cart_ram[cram_addr] & 0x02);
    PIN_EXT_D_in[2].pin_in(cram_OEp, cart_ram[cram_addr] & 0x04);
    PIN_EXT_D_in[3].pin_in(cram_OEp, cart_ram[cram_addr] & 0x08);
    PIN_EXT_D_in[4].pin_in(cram_OEp, cart_ram[cram_addr] & 0x10);
    PIN_EXT_D_in[5].pin_in(cram_OEp, cart_ram[cram_addr] & 0x20);
    PIN_EXT_D_in[6].pin_in(cram_OEp, cart_ram[cram_addr] & 0x40);
    PIN_EXT_D_in[7].pin_in(cram_OEp, cart_ram[cram_addr] & 0x80);
  }

  {
    // CPU data bus -> external data bus
    // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /* p08.RORU*/ wire _RORU_CBD_TO_EPDn = mux2p(_UNOR_MODE_DBG2p_new, _REDU_CPU_RDn_new, _MOTY_CPU_EXT_RD_new);
    /* p08.LULA*/ wire _LULA_CBD_TO_EPDp = not1(_RORU_CBD_TO_EPDn);

    /* p25.RUXA*/ wire _RUXA = nand2(BUS_CPU_Dp_in[0].qp(), _LULA_CBD_TO_EPDp);
    /* p25.RUJA*/ wire _RUJA = nand2(BUS_CPU_Dp_in[1].qp(), _LULA_CBD_TO_EPDp);
    /* p25.RABY*/ wire _RABY = nand2(BUS_CPU_Dp_in[2].qp(), _LULA_CBD_TO_EPDp);
    /* p25.RERA*/ wire _RERA = nand2(BUS_CPU_Dp_in[3].qp(), _LULA_CBD_TO_EPDp);
    /* p25.RORY*/ wire _RORY = nand2(BUS_CPU_Dp_in[4].qp(), _LULA_CBD_TO_EPDp);
    /* p25.RYVO*/ wire _RYVO = nand2(BUS_CPU_Dp_in[5].qp(), _LULA_CBD_TO_EPDp);
    /* p25.RAFY*/ wire _RAFY = nand2(BUS_CPU_Dp_in[6].qp(), _LULA_CBD_TO_EPDp);
    /* p25.RAVU*/ wire _RAVU = nand2(BUS_CPU_Dp_in[7].qp(), _LULA_CBD_TO_EPDp);

    /* p08.RUNE*/ wire _RUNE = nor2 (BUS_CPU_Dp_in[0].qp(), _RORU_CBD_TO_EPDn);
    /* p08.RYPU*/ wire _RYPU = nor2 (BUS_CPU_Dp_in[1].qp(), _RORU_CBD_TO_EPDn);
    /* p08.SULY*/ wire _SULY = nor2 (BUS_CPU_Dp_in[2].qp(), _RORU_CBD_TO_EPDn);
    /* p08.SEZE*/ wire _SEZE = nor2 (BUS_CPU_Dp_in[3].qp(), _RORU_CBD_TO_EPDn);
    /* p08.RESY*/ wire _RESY = nor2 (BUS_CPU_Dp_in[4].qp(), _RORU_CBD_TO_EPDn);
    /* p08.TAMU*/ wire _TAMU = nor2 (BUS_CPU_Dp_in[5].qp(), _RORU_CBD_TO_EPDn);
    /* p08.ROGY*/ wire _ROGY = nor2 (BUS_CPU_Dp_in[6].qp(), _RORU_CBD_TO_EPDn);
    /* p08.RYDA*/ wire _RYDA = nor2 (BUS_CPU_Dp_in[7].qp(), _RORU_CBD_TO_EPDn);

    Pin2 PIN_EXT_D_out[8];
    PIN_EXT_D_out[0].pin_out(_LULA_CBD_TO_EPDp, _RUXA, _RUNE);
    PIN_EXT_D_out[1].pin_out(_LULA_CBD_TO_EPDp, _RUJA, _RYPU);
    PIN_EXT_D_out[2].pin_out(_LULA_CBD_TO_EPDp, _RABY, _SULY);
    PIN_EXT_D_out[3].pin_out(_LULA_CBD_TO_EPDp, _RERA, _SEZE);
    PIN_EXT_D_out[4].pin_out(_LULA_CBD_TO_EPDp, _RORY, _RESY);
    PIN_EXT_D_out[5].pin_out(_LULA_CBD_TO_EPDp, _RYVO, _TAMU);
    PIN_EXT_D_out[6].pin_out(_LULA_CBD_TO_EPDp, _RAFY, _ROGY);
    PIN_EXT_D_out[7].pin_out(_LULA_CBD_TO_EPDp, _RAVU, _RYDA);

    {
      // ERAM write
      uint16_t eram_addr = (ext_addr & 0x1FFF);
      wire eram_WRn  = PIN_EXT_WRn.qp();
      wire eram_CE1n = PIN_EXT_CSn.qp();
      wire eram_CE2  = PIN_EXT_A[14].qp();
      if (!eram_CE1n && eram_CE2 && !eram_WRn) {
        ext_ram[eram_addr] = pack_u8p_new(8, &PIN_EXT_D_out[0]);
      }
    }

    {
      // CRAM write
      uint16_t cram_addr = (ext_addr & 0x1FFF);
      wire cram_WRn  = PIN_EXT_WRn.qp();
      wire cram_CS1n = PIN_EXT_CSn.qp();
      wire cram_CS2  = PIN_EXT_A[13].qp() && !PIN_EXT_A[14].qp() && PIN_EXT_A[15].qp();
      if (!cram_CS1n && cram_CS2 && !cram_WRn) {
        cart_ram[cram_addr] = pack_u8p_new(8, &PIN_EXT_D_out[0]);
      }
    }
  }

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

    wire cart_rom_a14 = bank_0 ? 1 : MBC1_BANK_D0.qp();
    wire cart_rom_a15 = bank_0 ? 0 : MBC1_BANK_D1.qp();
    wire cart_rom_a16 = bank_0 ? 0 : MBC1_BANK_D2.qp();
    wire cart_rom_a17 = bank_0 ? 0 : MBC1_BANK_D3.qp();
    wire cart_rom_a18 = bank_0 ? 0 : MBC1_BANK_D4.qp();
    wire cart_rom_a19 = MBC1_MODE.qp() ? 0 : bank_0 ? 0 : MBC1_BANK_D5.qp();
    wire cart_rom_a20 = MBC1_MODE.qp() ? 0 : bank_0 ? 0 : MBC1_BANK_D6.qp();

    wire cart_ram_a13 = MBC1_MODE.qp() ? MBC1_BANK_D5.qp() : 0;
    wire cart_ram_a14 = MBC1_MODE.qp() ? MBC1_BANK_D6.qp() : 0;

    // ROM read
    {
      uint16_t rom_addr = ext_addr & 0x7FFF;
      wire OEn = PIN_EXT_RDn.qp();
      wire CEn = PIN_EXT_A[15].qp();

      if (!CEn && !OEn) {
        ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
    }
  }
  */

#pragma region LYC_Matcher
  {
    // LYC matcher
    /* p21.RYME*/ wire _RYME_LY_MATCH0n = xor2(lcd_reg.MUWY_LY0p.qp(), lcd_reg.SYRY_LYC0n_s.qn());
    /* p21.TYDE*/ wire _TYDE_LY_MATCH1n = xor2(lcd_reg.MYRO_LY1p.qp(), lcd_reg.VUCE_LYC1n_s.qn());
    /* p21.REDA*/ wire _REDA_LY_MATCH2n = xor2(lcd_reg.LEXA_LY2p.qp(), lcd_reg.SEDY_LYC2n_s.qn());
    /* p21.RASY*/ wire _RASY_LY_MATCH3n = xor2(lcd_reg.LYDO_LY3p.qp(), lcd_reg.SALO_LYC3n_s.qn());
    /* p21.TYKU*/ wire _TYKU_LY_MATCH4n = xor2(lcd_reg.LOVU_LY4p.qp(), lcd_reg.SOTA_LYC4n_s.qn());
    /* p21.TUCY*/ wire _TUCY_LY_MATCH5n = xor2(lcd_reg.LEMA_LY5p.qp(), lcd_reg.VAFA_LYC5n_s.qn());
    /* p21.TERY*/ wire _TERY_LY_MATCH6n = xor2(lcd_reg.MATO_LY6p.qp(), lcd_reg.VEVO_LYC6n_s.qn());
    /* p21.SYFU*/ wire _SYFU_LY_MATCH7n = xor2(lcd_reg.LAFO_LY7p.qp(), lcd_reg.RAHA_LYC7n_s.qn());

    /* p21.SOVU*/ wire _SOVU_LY_MATCHA = nor4 (_SYFU_LY_MATCH7n, _TERY_LY_MATCH6n, _TUCY_LY_MATCH5n, _TYKU_LY_MATCH4n); // def nor4
    /* p21.SUBO*/ wire _SUBO_LY_MATCHB = nor4 (_RASY_LY_MATCH3n, _REDA_LY_MATCH2n, _TYDE_LY_MATCH1n, _RYME_LY_MATCH0n); // def nor4
    /* p21.RAPE*/ wire _RAPE_LY_MATCHn = nand2(_SOVU_LY_MATCHA, _SUBO_LY_MATCHB); // def nand2
    /* p21.PALY*/ wire _PALY_LY_MATCHa = not1(_RAPE_LY_MATCHn); // def not

    /*#p21.ROPO*/ lcd_reg.ROPO_LY_MATCH_SYNCp.dff17(_TALU_xxCDEFxx_new,  _WESY_SYS_RSTn_new, _PALY_LY_MATCHa);
  }
#pragma endregion

#pragma region SpriteData
  // VRAM tri -> sprite pix temp + x flip
  {
    /* p29.SYCU*/ wire _SYCU_SFETCH_S0pe = nor3(_TYTU_SFETCH_S0n, _LOBY_RENDERINGn_new, sprite_fetcher.TYFO_SFETCH_S0p_D1.qp());

    /*#p29.XONO*/ wire _XONO_FLIP_X = and2(oam_bus.BAXO_OAM_DB5p.qp(), _TEXY_SPR_READ_VRAMp);
    /* p33.PUTE*/ wire _PUTE_FLIP0p = mux2p(_XONO_FLIP_X, BUS_VRAM_Dp_in[7].qp(), BUS_VRAM_Dp_in[0].qp());
    /* p33.PELO*/ wire _PELO_FLIP1p = mux2p(_XONO_FLIP_X, BUS_VRAM_Dp_in[6].qp(), BUS_VRAM_Dp_in[1].qp());
    /* p33.PONO*/ wire _PONO_FLIP2p = mux2p(_XONO_FLIP_X, BUS_VRAM_Dp_in[5].qp(), BUS_VRAM_Dp_in[2].qp());
    /* p33.POBE*/ wire _POBE_FLIP3p = mux2p(_XONO_FLIP_X, BUS_VRAM_Dp_in[4].qp(), BUS_VRAM_Dp_in[3].qp());
    /* p33.PACY*/ wire _PACY_FLIP4p = mux2p(_XONO_FLIP_X, BUS_VRAM_Dp_in[3].qp(), BUS_VRAM_Dp_in[4].qp());
    /* p33.PUGU*/ wire _PUGU_FLIP5p = mux2p(_XONO_FLIP_X, BUS_VRAM_Dp_in[2].qp(), BUS_VRAM_Dp_in[5].qp());
    /* p33.PAWE*/ wire _PAWE_FLIP6p = mux2p(_XONO_FLIP_X, BUS_VRAM_Dp_in[1].qp(), BUS_VRAM_Dp_in[6].qp());
    /* p33.PULY*/ wire _PULY_FLIP7p = mux2p(_XONO_FLIP_X, BUS_VRAM_Dp_in[0].qp(), BUS_VRAM_Dp_in[7].qp());

    /*#p29.RACA*/ wire _RACA_CLKp = and2(sprite_fetcher.VONU_SFETCH_S1p_D4.qp(), _SYCU_SFETCH_S0pe);
    /*#p29.PEBY*/ wire _PEBY_CLKn = not1(_RACA_CLKp);
    /*#p29.NYBE*/ wire _NYBE_CLKp = not1(_PEBY_CLKn);
    /*#p29.PUCO*/ wire _PUCO_CLKn = not1(_NYBE_CLKp);
    /* p33.PEFO*/ vram_bus.PEFO_SPRITE_DB0n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PUTE_FLIP0p);
    /* p33.ROKA*/ vram_bus.ROKA_SPRITE_DB1n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PELO_FLIP1p);
    /* p33.MYTU*/ vram_bus.MYTU_SPRITE_DB2n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PONO_FLIP2p);
    /* p33.RAMU*/ vram_bus.RAMU_SPRITE_DB3n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _POBE_FLIP3p);
    /* p33.SELE*/ vram_bus.SELE_SPRITE_DB4n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PACY_FLIP4p);
    /* p33.SUTO*/ vram_bus.SUTO_SPRITE_DB5n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PUGU_FLIP5p);
    /* p33.RAMA*/ vram_bus.RAMA_SPRITE_DB6n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PAWE_FLIP6p);
    /* p33.RYDU*/ vram_bus.RYDU_SPRITE_DB7n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PULY_FLIP7p);

    /*#p29.TOPU*/ wire _TOPU_CLKp = and2(sprite_fetcher.TULY_SFETCH_S1p.qp(),    _SYCU_SFETCH_S0pe);
    /*#p29.VYWA*/ wire _VYWA_CLKn = not1(_TOPU_CLKp);
    /*#p29.WENY*/ wire _WENY_CLKp = not1(_VYWA_CLKn);
    /*#p29.XADO*/ wire _XADO_CLKn = not1(_WENY_CLKp);
    /* p33.REWO*/ vram_bus.REWO_SPRITE_DA0n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PUTE_FLIP0p);
    /* p33.PEBA*/ vram_bus.PEBA_SPRITE_DA1n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PELO_FLIP1p);
    /* p33.MOFO*/ vram_bus.MOFO_SPRITE_DA2n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PONO_FLIP2p);
    /* p33.PUDU*/ vram_bus.PUDU_SPRITE_DA3n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _POBE_FLIP3p);
    /* p33.SAJA*/ vram_bus.SAJA_SPRITE_DA4n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PACY_FLIP4p);
    /* p33.SUNY*/ vram_bus.SUNY_SPRITE_DA5n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PUGU_FLIP5p);
    /* p33.SEMO*/ vram_bus.SEMO_SPRITE_DA6n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PAWE_FLIP6p);
    /* p33.SEGA*/ vram_bus.SEGA_SPRITE_DA7n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PULY_FLIP7p);
  }
#pragma endregion

#pragma region Pixel_Pipes
  // Reload all the pipes using set/rst
  {
    /* p32.LOZE*/ wire _LOZE_PIPE_A_LOADp = not1(_NYXU_FETCH_TRIGn);
    /* p32.LAKY*/ wire BG_PIPE_A_SET0 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.LEGU_TILE_DA0n.qn());
    /* p32.NYXO*/ wire BG_PIPE_A_SET1 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.NUDU_TILE_DA1n.qn());
    /* p32.LOTO*/ wire BG_PIPE_A_SET2 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.MUKU_TILE_DA2n.qn());
    /* p32.LYDU*/ wire BG_PIPE_A_SET3 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.LUZO_TILE_DA3n.qn());
    /* p32.MYVY*/ wire BG_PIPE_A_SET4 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.MEGU_TILE_DA4n.qn());
    /* p32.LODO*/ wire BG_PIPE_A_SET5 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.MYJY_TILE_DA5n.qn());
    /* p32.NUTE*/ wire BG_PIPE_A_SET6 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.NASA_TILE_DA6n.qn());
    /* p32.NAJA*/ wire BG_PIPE_A_SET7 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.NEFO_TILE_DA7n.qn());

    /* p32.LUHE*/ wire BG_PIX_A0n = not1(vram_bus.LEGU_TILE_DA0n.qn());
    /* p32.NOLY*/ wire BG_PIX_A1n = not1(vram_bus.NUDU_TILE_DA1n.qn());
    /* p32.LEKE*/ wire BG_PIX_A2n = not1(vram_bus.MUKU_TILE_DA2n.qn());
    /* p32.LOMY*/ wire BG_PIX_A3n = not1(vram_bus.LUZO_TILE_DA3n.qn());
    /* p32.LALA*/ wire BG_PIX_A4n = not1(vram_bus.MEGU_TILE_DA4n.qn());
    /* p32.LOXA*/ wire BG_PIX_A5n = not1(vram_bus.MYJY_TILE_DA5n.qn());
    /* p32.NEZE*/ wire BG_PIX_A6n = not1(vram_bus.NASA_TILE_DA6n.qn());
    /* p32.NOBO*/ wire BG_PIX_A7n = not1(vram_bus.NEFO_TILE_DA7n.qn());

    /* p32.LOTY*/ wire BG_PIPE_A_RST0 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A0n);
    /* p32.NEXA*/ wire BG_PIPE_A_RST1 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A1n);
    /* p32.LUTU*/ wire BG_PIPE_A_RST2 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A2n);
    /* p32.LUJA*/ wire BG_PIPE_A_RST3 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A3n);
    /* p32.MOSY*/ wire BG_PIPE_A_RST4 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A4n);
    /* p32.LERU*/ wire BG_PIPE_A_RST5 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A5n);
    /* p32.NYHA*/ wire BG_PIPE_A_RST6 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A6n);
    /* p32.NADY*/ wire BG_PIPE_A_RST7 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A7n);

    /* p32.PYBO*/ pix_pipe.PYBO_BG_PIPE_A7.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_A_SET7, BG_PIPE_A_RST7, pix_pipe.NEDA_BG_PIPE_A6.qp());
    /* p32.NEDA*/ pix_pipe.NEDA_BG_PIPE_A6.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_A_SET6, BG_PIPE_A_RST6, pix_pipe.MODU_BG_PIPE_A5.qp());
    /* p32.MODU*/ pix_pipe.MODU_BG_PIPE_A5.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_A_SET5, BG_PIPE_A_RST5, pix_pipe.NEPO_BG_PIPE_A4.qp());
    /* p32.NEPO*/ pix_pipe.NEPO_BG_PIPE_A4.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_A_SET4, BG_PIPE_A_RST4, pix_pipe.MACU_BG_PIPE_A3.qp());
    /* p32.MACU*/ pix_pipe.MACU_BG_PIPE_A3.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_A_SET3, BG_PIPE_A_RST3, pix_pipe.MOJU_BG_PIPE_A2.qp());
    /* p32.MOJU*/ pix_pipe.MOJU_BG_PIPE_A2.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_A_SET2, BG_PIPE_A_RST2, pix_pipe.NOZO_BG_PIPE_A1.qp());
    /* p32.NOZO*/ pix_pipe.NOZO_BG_PIPE_A1.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_A_SET1, BG_PIPE_A_RST1, pix_pipe.MYDE_BG_PIPE_A0.qp());
    /* p32.MYDE*/ pix_pipe.MYDE_BG_PIPE_A0.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_A_SET0, BG_PIPE_A_RST0, GND);
  }

  {
    /* p32.LUXA*/ wire LUXA_PIPE_B_LOADp = not1(_NYXU_FETCH_TRIGn);
    /* p32.TUXE*/ wire BG_PIPE_B_SET0 = nand2(LUXA_PIPE_B_LOADp, vram_bus.RAWU_TILE_DB0p.qp());
    /* p32.SOLY*/ wire BG_PIPE_B_SET1 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POZO_TILE_DB1p.qp());
    /* p32.RUCE*/ wire BG_PIPE_B_SET2 = nand2(LUXA_PIPE_B_LOADp, vram_bus.PYZO_TILE_DB2p.qp());
    /* p32.RYJA*/ wire BG_PIPE_B_SET3 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POXA_TILE_DB3p.qp());
    /* p32.RUTO*/ wire BG_PIPE_B_SET4 = nand2(LUXA_PIPE_B_LOADp, vram_bus.PULO_TILE_DB4p.qp());
    /* p32.RAJA*/ wire BG_PIPE_B_SET5 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POJU_TILE_DB5p.qp());
    /* p32.RAJO*/ wire BG_PIPE_B_SET6 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POWY_TILE_DB6p.qp());
    /* p32.RAGA*/ wire BG_PIPE_B_SET7 = nand2(LUXA_PIPE_B_LOADp, vram_bus.PYJU_TILE_DB7p.qp());

    /* p32.TOSA*/ wire BG_PIX_B0n = not1(vram_bus.RAWU_TILE_DB0p.qp());
    /* p32.RUCO*/ wire BG_PIX_B1n = not1(vram_bus.POZO_TILE_DB1p.qp());
    /* p32.TYCE*/ wire BG_PIX_B2n = not1(vram_bus.PYZO_TILE_DB2p.qp());
    /* p32.REVY*/ wire BG_PIX_B3n = not1(vram_bus.POXA_TILE_DB3p.qp());
    /* p32.RYGA*/ wire BG_PIX_B4n = not1(vram_bus.PULO_TILE_DB4p.qp());
    /* p32.RYLE*/ wire BG_PIX_B5n = not1(vram_bus.POJU_TILE_DB5p.qp());
    /* p32.RAPU*/ wire BG_PIX_B6n = not1(vram_bus.POWY_TILE_DB6p.qp());
    /* p32.SOJA*/ wire BG_PIX_B7n = not1(vram_bus.PYJU_TILE_DB7p.qp());

    /* p32.SEJA*/ wire BG_PIPE_B_RST0 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B0n);
    /* p32.SENO*/ wire BG_PIPE_B_RST1 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B1n);
    /* p32.SURE*/ wire BG_PIPE_B_RST2 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B2n);
    /* p32.SEBO*/ wire BG_PIPE_B_RST3 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B3n);
    /* p32.SUCA*/ wire BG_PIPE_B_RST4 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B4n);
    /* p32.SYWE*/ wire BG_PIPE_B_RST5 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B5n);
    /* p32.SUPU*/ wire BG_PIPE_B_RST6 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B6n);
    /* p32.RYJY*/ wire BG_PIPE_B_RST7 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B7n);

    /* p32.SOHU*/ pix_pipe.SOHU_BG_PIPE_B7.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_B_SET7, BG_PIPE_B_RST7, pix_pipe.RALU_BG_PIPE_B6.qp());
    /* p32.RALU*/ pix_pipe.RALU_BG_PIPE_B6.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_B_SET6, BG_PIPE_B_RST6, pix_pipe.SETU_BG_PIPE_B5.qp());
    /* p32.SETU*/ pix_pipe.SETU_BG_PIPE_B5.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_B_SET5, BG_PIPE_B_RST5, pix_pipe.SOBO_BG_PIPE_B4.qp());
    /* p32.SOBO*/ pix_pipe.SOBO_BG_PIPE_B4.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_B_SET4, BG_PIPE_B_RST4, pix_pipe.RYSA_BG_PIPE_B3.qp());
    /* p32.RYSA*/ pix_pipe.RYSA_BG_PIPE_B3.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_B_SET3, BG_PIPE_B_RST3, pix_pipe.SADY_BG_PIPE_B2.qp());
    /* p32.SADY*/ pix_pipe.SADY_BG_PIPE_B2.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_B_SET2, BG_PIPE_B_RST2, pix_pipe.TACA_BG_PIPE_B1.qp());
    /* p32.TACA*/ pix_pipe.TACA_BG_PIPE_B1.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_B_SET1, BG_PIPE_B_RST1, pix_pipe.TOMY_BG_PIPE_B0.qp());
    /* p32.TOMY*/ pix_pipe.TOMY_BG_PIPE_B0.dff22(_SACU_CLKPIPE_AxCxExGx, BG_PIPE_B_SET0, BG_PIPE_B_RST0, GND);
  }

  {
    /* p29.XEFY*/ wire XEPY_SPRITE_DONEn  = not1(_WUTY_SPRITE_DONEp);
    /* p34.MEFU*/ wire MEFU_SPRITE_MASK0n = or3(XEPY_SPRITE_DONEn, pix_pipe.NYLU_SPR_PIPE_B0.qp(), pix_pipe.NURO_SPR_PIPE_A0.qp()); // def or
    /* p34.MEVE*/ wire MEVE_SPRITE_MASK1n = or3(XEPY_SPRITE_DONEn, pix_pipe.PEFU_SPR_PIPE_B1.qp(), pix_pipe.MASO_SPR_PIPE_A1.qp());
    /* p34.MYZO*/ wire MYZO_SPRITE_MASK2n = or3(XEPY_SPRITE_DONEn, pix_pipe.NATY_SPR_PIPE_B2.qp(), pix_pipe.LEFE_SPR_PIPE_A2.qp());
    /* p34.RUDA*/ wire RUDA_SPRITE_MASK3n = or3(XEPY_SPRITE_DONEn, pix_pipe.PYJO_SPR_PIPE_B3.qp(), pix_pipe.LESU_SPR_PIPE_A3.qp());
    /* p34.VOTO*/ wire VOTO_SPRITE_MASK4n = or3(XEPY_SPRITE_DONEn, pix_pipe.VARE_SPR_PIPE_B4.qp(), pix_pipe.WYHO_SPR_PIPE_A4.qp());
    /* p34.VYSA*/ wire VYSA_SPRITE_MASK5n = or3(XEPY_SPRITE_DONEn, pix_pipe.WEBA_SPR_PIPE_B5.qp(), pix_pipe.WORA_SPR_PIPE_A5.qp());
    /* p34.TORY*/ wire TORY_SPRITE_MASK6n = or3(XEPY_SPRITE_DONEn, pix_pipe.VANU_SPR_PIPE_B6.qp(), pix_pipe.VAFO_SPR_PIPE_A6.qp());
    /* p34.WOPE*/ wire WOPE_SPRITE_MASK7n = or3(XEPY_SPRITE_DONEn, pix_pipe.VUPY_SPR_PIPE_B7.qp(), pix_pipe.WUFY_SPR_PIPE_A7.qp());

    // The mask is 1 where there are _no_ sprite pixels
    /* p34.LESY*/ wire LESY_SPRITE_MASK0p = not1(MEFU_SPRITE_MASK0n);
    /* p34.LOTA*/ wire LOTA_SPRITE_MASK1p = not1(MEVE_SPRITE_MASK1n);
    /* p34.LYKU*/ wire LYKU_SPRITE_MASK2p = not1(MYZO_SPRITE_MASK2n);
    /* p34.ROBY*/ wire ROBY_SPRITE_MASK3p = not1(RUDA_SPRITE_MASK3n);
    /* p34.TYTA*/ wire TYTA_SPRITE_MASK4p = not1(VOTO_SPRITE_MASK4n);
    /* p34.TYCO*/ wire TYCO_SPRITE_MASK5p = not1(VYSA_SPRITE_MASK5n);
    /* p34.SOKA*/ wire SOKA_SPRITE_MASK6p = not1(TORY_SPRITE_MASK6n);
    /* p34.XOVU*/ wire XOVU_SPRITE_MASK7p = not1(WOPE_SPRITE_MASK7n);


    // Sprite pipe A
    {
      /* p33.PABE*/ wire PABE_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, vram_bus.REWO_SPRITE_DA0n.qn());
      /* p33.MYTO*/ wire MYTO_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, vram_bus.PEBA_SPRITE_DA1n.qn());
      /* p33.LELA*/ wire LELA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, vram_bus.MOFO_SPRITE_DA2n.qn());
      /* p33.MAME*/ wire MAME_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, vram_bus.PUDU_SPRITE_DA3n.qn());
      /* p33.VEXU*/ wire VEXU_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, vram_bus.SAJA_SPRITE_DA4n.qn());
      /* p33.VABY*/ wire VABY_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, vram_bus.SUNY_SPRITE_DA5n.qn());
      /* p33.TUXA*/ wire TUXA_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, vram_bus.SEMO_SPRITE_DA6n.qn());
      /* p33.VUNE*/ wire VUNE_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, vram_bus.SEGA_SPRITE_DA7n.qn());

      /* p33.RATA*/ wire RATA_SPR_PIX_DA0n = not1(vram_bus.REWO_SPRITE_DA0n.qn());
      /* p33.NUCA*/ wire NUCA_SPR_PIX_DA1n = not1(vram_bus.PEBA_SPRITE_DA1n.qn());
      /* p33.LASE*/ wire LASE_SPR_PIX_DA2n = not1(vram_bus.MOFO_SPRITE_DA2n.qn());
      /* p33.LUBO*/ wire LUBO_SPR_PIX_DA3n = not1(vram_bus.PUDU_SPRITE_DA3n.qn());
      /* p33.WERY*/ wire WERY_SPR_PIX_DA4n = not1(vram_bus.SAJA_SPRITE_DA4n.qn());
      /* p33.WURA*/ wire WURA_SPR_PIX_DA5n = not1(vram_bus.SUNY_SPRITE_DA5n.qn());
      /* p33.SULU*/ wire SULU_SPR_PIX_DA6n = not1(vram_bus.SEMO_SPRITE_DA6n.qn());
      /* p33.WAMY*/ wire WAMY_SPR_PIX_DA7n = not1(vram_bus.SEGA_SPRITE_DA7n.qn());

      /* p33.PYZU*/ wire PYZU_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, RATA_SPR_PIX_DA0n);
      /* p33.MADA*/ wire MADA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, NUCA_SPR_PIX_DA1n);
      /* p33.LYDE*/ wire LYDE_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LASE_SPR_PIX_DA2n);
      /* p33.LUFY*/ wire LUFY_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, LUBO_SPR_PIX_DA3n);
      /* p33.XATO*/ wire XATO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, WERY_SPR_PIX_DA4n);
      /* p33.XEXU*/ wire XEXU_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WURA_SPR_PIX_DA5n);
      /* p33.TUPE*/ wire TUPE_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SULU_SPR_PIX_DA6n);
      /* p33.XYVE*/ wire XYVE_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, WAMY_SPR_PIX_DA7n);

      /* p33.WUFY*/ pix_pipe.WUFY_SPR_PIPE_A7.dff22(_SACU_CLKPIPE_AxCxExGx, VUNE_SPR_PIX_SET7, XYVE_SPR_PIX_RST7, pix_pipe.VAFO_SPR_PIPE_A6.qp());
      /* p33.VAFO*/ pix_pipe.VAFO_SPR_PIPE_A6.dff22(_SACU_CLKPIPE_AxCxExGx, TUXA_SPR_PIX_SET6, TUPE_SPR_PIX_RST6, pix_pipe.WORA_SPR_PIPE_A5.qp());
      /* p33.WORA*/ pix_pipe.WORA_SPR_PIPE_A5.dff22(_SACU_CLKPIPE_AxCxExGx, VABY_SPR_PIX_SET5, XEXU_SPR_PIX_RST5, pix_pipe.WYHO_SPR_PIPE_A4.qp());
      /* p33.WYHO*/ pix_pipe.WYHO_SPR_PIPE_A4.dff22(_SACU_CLKPIPE_AxCxExGx, VEXU_SPR_PIX_SET4, XATO_SPR_PIX_RST4, pix_pipe.LESU_SPR_PIPE_A3.qp());
      /* p33.LESU*/ pix_pipe.LESU_SPR_PIPE_A3.dff22(_SACU_CLKPIPE_AxCxExGx, MAME_SPR_PIX_SET3, LUFY_SPR_PIX_RST3, pix_pipe.LEFE_SPR_PIPE_A2.qp());
      /* p33.LEFE*/ pix_pipe.LEFE_SPR_PIPE_A2.dff22(_SACU_CLKPIPE_AxCxExGx, LELA_SPR_PIX_SET2, LYDE_SPR_PIX_RST2, pix_pipe.MASO_SPR_PIPE_A1.qp());
      /* p33.MASO*/ pix_pipe.MASO_SPR_PIPE_A1.dff22(_SACU_CLKPIPE_AxCxExGx, MYTO_SPR_PIX_SET1, MADA_SPR_PIX_RST1, pix_pipe.NURO_SPR_PIPE_A0.qp());
      /* p33.NURO*/ pix_pipe.NURO_SPR_PIPE_A0.dff22(_SACU_CLKPIPE_AxCxExGx, PABE_SPR_PIX_SET0, PYZU_SPR_PIX_RST0, GND);
    }

    // Sprite pipe B
    {
      /* p33.MEZU*/ wire MEZU_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, vram_bus.PEFO_SPRITE_DB0n.qn());
      /* p33.RUSY*/ wire RUSY_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, vram_bus.ROKA_SPRITE_DB1n.qn());
      /* p33.MYXA*/ wire MYXA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, vram_bus.MYTU_SPRITE_DB2n.qn());
      /* p33.RANO*/ wire RANO_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, vram_bus.RAMU_SPRITE_DB3n.qn());
      /* p33.TYGA*/ wire TYGA_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, vram_bus.SELE_SPRITE_DB4n.qn());
      /* p33.VUME*/ wire VUME_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, vram_bus.SUTO_SPRITE_DB5n.qn());
      /* p33.TAPO*/ wire TAPO_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, vram_bus.RAMA_SPRITE_DB6n.qn());
      /* p33.TESO*/ wire TESO_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, vram_bus.RYDU_SPRITE_DB7n.qn());

      /* p33.LOZA*/ wire LOZA_SPR_PIX_DB0n = not1(vram_bus.PEFO_SPRITE_DB0n.qn());
      /* p33.SYBO*/ wire SYBO_SPR_PIX_DB1n = not1(vram_bus.ROKA_SPRITE_DB1n.qn());
      /* p33.LUMO*/ wire LUMO_SPR_PIX_DB2n = not1(vram_bus.MYTU_SPRITE_DB2n.qn());
      /* p33.SOLO*/ wire SOLO_SPR_PIX_DB3n = not1(vram_bus.RAMU_SPRITE_DB3n.qn());
      /* p33.VOBY*/ wire VOBY_SPR_PIX_DB4n = not1(vram_bus.SELE_SPRITE_DB4n.qn());
      /* p33.WYCO*/ wire WYCO_SPR_PIX_DB5n = not1(vram_bus.SUTO_SPRITE_DB5n.qn());
      /* p33.SERY*/ wire SERY_SPR_PIX_DB6n = not1(vram_bus.RAMA_SPRITE_DB6n.qn());
      /* p33.SELU*/ wire SELU_SPR_PIX_DB7n = not1(vram_bus.RYDU_SPRITE_DB7n.qn());

      /* p33.MOFY*/ wire MOFY_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, LOZA_SPR_PIX_DB0n);
      /* p33.RUCA*/ wire RUCA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, SYBO_SPR_PIX_DB1n);
      /* p33.MAJO*/ wire MAJO_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LUMO_SPR_PIX_DB2n);
      /* p33.REHU*/ wire REHU_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, SOLO_SPR_PIX_DB3n);
      /* p33.WAXO*/ wire WAXO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, VOBY_SPR_PIX_DB4n);
      /* p33.XOLE*/ wire XOLE_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WYCO_SPR_PIX_DB5n);
      /* p33.TABY*/ wire TABY_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SERY_SPR_PIX_DB6n);
      /* p33.TULA*/ wire TULA_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, SELU_SPR_PIX_DB7n);

      /* p33.VUPY*/ pix_pipe.VUPY_SPR_PIPE_B7.dff22(_SACU_CLKPIPE_AxCxExGx, TESO_SPR_PIX_SET7, TULA_SPR_PIX_RST7, pix_pipe.VANU_SPR_PIPE_B6.qp());
      /* p33.VANU*/ pix_pipe.VANU_SPR_PIPE_B6.dff22(_SACU_CLKPIPE_AxCxExGx, TAPO_SPR_PIX_SET6, TABY_SPR_PIX_RST6, pix_pipe.WEBA_SPR_PIPE_B5.qp());
      /* p33.WEBA*/ pix_pipe.WEBA_SPR_PIPE_B5.dff22(_SACU_CLKPIPE_AxCxExGx, VUME_SPR_PIX_SET5, XOLE_SPR_PIX_RST5, pix_pipe.VARE_SPR_PIPE_B4.qp());
      /* p33.VARE*/ pix_pipe.VARE_SPR_PIPE_B4.dff22(_SACU_CLKPIPE_AxCxExGx, TYGA_SPR_PIX_SET4, WAXO_SPR_PIX_RST4, pix_pipe.PYJO_SPR_PIPE_B3.qp());
      /* p33.PYJO*/ pix_pipe.PYJO_SPR_PIPE_B3.dff22(_SACU_CLKPIPE_AxCxExGx, RANO_SPR_PIX_SET3, REHU_SPR_PIX_RST3, pix_pipe.NATY_SPR_PIPE_B2.qp());
      /* p33.NATY*/ pix_pipe.NATY_SPR_PIPE_B2.dff22(_SACU_CLKPIPE_AxCxExGx, MYXA_SPR_PIX_SET2, MAJO_SPR_PIX_RST2, pix_pipe.PEFU_SPR_PIPE_B1.qp());
      /* p33.PEFU*/ pix_pipe.PEFU_SPR_PIPE_B1.dff22(_SACU_CLKPIPE_AxCxExGx, RUSY_SPR_PIX_SET1, RUCA_SPR_PIX_RST1, pix_pipe.NYLU_SPR_PIPE_B0.qp());
      /* p33.NYLU*/ pix_pipe.NYLU_SPR_PIPE_B0.dff22(_SACU_CLKPIPE_AxCxExGx, MEZU_SPR_PIX_SET0, MOFY_SPR_PIX_RST0, GND);
    }

    // Palette pipe
    {
      /* p34.PUME*/ wire PUME_PAL_PIPE_SET0n = nand2(LESY_SPRITE_MASK0p, oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.SORO*/ wire SORO_PAL_PIPE_SET1n = nand2(LOTA_SPRITE_MASK1p, oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.PAMO*/ wire PAMO_PAL_PIPE_SET2n = nand2(LYKU_SPRITE_MASK2p, oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.SUKY*/ wire SUKY_PAL_PIPE_SET3n = nand2(ROBY_SPRITE_MASK3p, oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.RORA*/ wire RORA_PAL_PIPE_SET4n = nand2(TYTA_SPRITE_MASK4p, oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.MENE*/ wire MENE_PAL_PIPE_SET5n = nand2(TYCO_SPRITE_MASK5p, oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.LUKE*/ wire LUKE_PAL_PIPE_SET6n = nand2(SOKA_SPRITE_MASK6p, oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.LAMY*/ wire LAMY_PAL_PIPE_SET7n = nand2(XOVU_SPRITE_MASK7p, oam_bus.GOMO_OAM_DB4p.qp());

      /* p34.SYPY*/ wire SYPY = not1(oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.TOTU*/ wire TOTU = not1(oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.NARO*/ wire NARO = not1(oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.WEXY*/ wire WEXY = not1(oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.RYZY*/ wire RYZY = not1(oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.RYFE*/ wire RYFE = not1(oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.LADY*/ wire LADY = not1(oam_bus.GOMO_OAM_DB4p.qp());
      /* p34.LAFY*/ wire LAFY = not1(oam_bus.GOMO_OAM_DB4p.qp());

      /* p34.SUCO*/ wire SUCO_PAL_PIPE_RST0n = nand2(LESY_SPRITE_MASK0p, SYPY);
      /* p34.TAFA*/ wire TAFA_PAL_PIPE_RST1n = nand2(LOTA_SPRITE_MASK1p, TOTU);
      /* p34.PYZY*/ wire PYZY_PAL_PIPE_RST2n = nand2(LYKU_SPRITE_MASK2p, NARO);
      /* p34.TOWA*/ wire TOWA_PAL_PIPE_RST3n = nand2(ROBY_SPRITE_MASK3p, WEXY);
      /* p34.RUDU*/ wire RUDU_PAL_PIPE_RST4n = nand2(TYTA_SPRITE_MASK4p, RYZY);
      /* p34.PAZO*/ wire PAZO_PAL_PIPE_RST5n = nand2(TYCO_SPRITE_MASK5p, RYFE);
      /* p34.LOWA*/ wire LOWA_PAL_PIPE_RST6n = nand2(SOKA_SPRITE_MASK6p, LADY);
      /* p34.LUNU*/ wire LUNU_PAL_PIPE_RST7n = nand2(XOVU_SPRITE_MASK7p, LAFY);

      /* p34.LYME*/ pix_pipe.LYME_PAL_PIPE_7.dff22(_SACU_CLKPIPE_AxCxExGx, LAMY_PAL_PIPE_SET7n, LUNU_PAL_PIPE_RST7n, pix_pipe.MODA_PAL_PIPE_6.qp());
      /* p34.MODA*/ pix_pipe.MODA_PAL_PIPE_6.dff22(_SACU_CLKPIPE_AxCxExGx, LUKE_PAL_PIPE_SET6n, LOWA_PAL_PIPE_RST6n, pix_pipe.NUKE_PAL_PIPE_5.qp());
      /* p34.NUKE*/ pix_pipe.NUKE_PAL_PIPE_5.dff22(_SACU_CLKPIPE_AxCxExGx, MENE_PAL_PIPE_SET5n, PAZO_PAL_PIPE_RST5n, pix_pipe.PALU_PAL_PIPE_4.qp());
      /* p34.PALU*/ pix_pipe.PALU_PAL_PIPE_4.dff22(_SACU_CLKPIPE_AxCxExGx, RORA_PAL_PIPE_SET4n, RUDU_PAL_PIPE_RST4n, pix_pipe.SOMY_PAL_PIPE_3.qp());
      /* p34.SOMY*/ pix_pipe.SOMY_PAL_PIPE_3.dff22(_SACU_CLKPIPE_AxCxExGx, SUKY_PAL_PIPE_SET3n, TOWA_PAL_PIPE_RST3n, pix_pipe.ROSA_PAL_PIPE_2.qp());
      /* p34.ROSA*/ pix_pipe.ROSA_PAL_PIPE_2.dff22(_SACU_CLKPIPE_AxCxExGx, PAMO_PAL_PIPE_SET2n, PYZY_PAL_PIPE_RST2n, pix_pipe.SATA_PAL_PIPE_1.qp());
      /* p34.SATA*/ pix_pipe.SATA_PAL_PIPE_1.dff22(_SACU_CLKPIPE_AxCxExGx, SORO_PAL_PIPE_SET1n, TAFA_PAL_PIPE_RST1n, pix_pipe.RUGO_PAL_PIPE_0.qp());
      /* p34.RUGO*/ pix_pipe.RUGO_PAL_PIPE_0.dff22(_SACU_CLKPIPE_AxCxExGx, PUME_PAL_PIPE_SET0n, SUCO_PAL_PIPE_RST0n, GND);
    }

    // Background mask pipe
    {
      /* p26.TEDE*/ wire TEDE_MASK_PIPE_SET0 = nand2(LESY_SPRITE_MASK0p, oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.XALA*/ wire XALA_MASK_PIPE_SET1 = nand2(LOTA_SPRITE_MASK1p, oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.TYRA*/ wire TYRA_MASK_PIPE_SET2 = nand2(LYKU_SPRITE_MASK2p, oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.XYRU*/ wire XYRU_MASK_PIPE_SET3 = nand2(ROBY_SPRITE_MASK3p, oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.XUKU*/ wire XUKU_MASK_PIPE_SET4 = nand2(TYTA_SPRITE_MASK4p, oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.XELY*/ wire XELY_MASK_PIPE_SET5 = nand2(TYCO_SPRITE_MASK5p, oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.TYKO*/ wire TYKO_MASK_PIPE_SET6 = nand2(SOKA_SPRITE_MASK6p, oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.TUWU*/ wire TUWU_MASK_PIPE_SET7 = nand2(XOVU_SPRITE_MASK7p, oam_bus.DEPO_OAM_DB7p.qp());

      /* p26.XOGA*/ wire XOGA = not1(oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.XURA*/ wire XURA = not1(oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.TAJO*/ wire TAJO = not1(oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.XENU*/ wire XENU = not1(oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.XYKE*/ wire XYKE = not1(oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.XABA*/ wire XABA = not1(oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.TAFU*/ wire TAFU = not1(oam_bus.DEPO_OAM_DB7p.qp());
      /* p26.XUHO*/ wire XUHO = not1(oam_bus.DEPO_OAM_DB7p.qp());

      /* p26.WOKA*/ wire WOKA_MASK_PIPE_RST0 = nand2(LESY_SPRITE_MASK0p, XOGA);
      /* p26.WEDE*/ wire WEDE_MASK_PIPE_RST1 = nand2(LOTA_SPRITE_MASK1p, XURA);
      /* p26.TUFO*/ wire TUFO_MASK_PIPE_RST2 = nand2(LYKU_SPRITE_MASK2p, TAJO);
      /* p26.WEVO*/ wire WEVO_MASK_PIPE_RST3 = nand2(ROBY_SPRITE_MASK3p, XENU);
      /* p26.WEDY*/ wire WEDY_MASK_PIPE_RST4 = nand2(TYTA_SPRITE_MASK4p, XYKE);
      /* p26.WUJA*/ wire WUJA_MASK_PIPE_RST5 = nand2(TYCO_SPRITE_MASK5p, XABA);
      /* p26.TENA*/ wire TENA_MASK_PIPE_RST6 = nand2(SOKA_SPRITE_MASK6p, TAFU);
      /* p26.WUBU*/ wire WUBU_MASK_PIPE_RST7 = nand2(XOVU_SPRITE_MASK7p, XUHO);

      /* p26.VAVA*/ pix_pipe.VAVA_MASK_PIPE_7.dff22(_SACU_CLKPIPE_AxCxExGx, TUWU_MASK_PIPE_SET7, WUBU_MASK_PIPE_RST7, pix_pipe.VUMO_MASK_PIPE_6.qp());
      /* p26.VUMO*/ pix_pipe.VUMO_MASK_PIPE_6.dff22(_SACU_CLKPIPE_AxCxExGx, TYKO_MASK_PIPE_SET6, TENA_MASK_PIPE_RST6, pix_pipe.WODA_MASK_PIPE_5.qp());
      /* p26.WODA*/ pix_pipe.WODA_MASK_PIPE_5.dff22(_SACU_CLKPIPE_AxCxExGx, XELY_MASK_PIPE_SET5, WUJA_MASK_PIPE_RST5, pix_pipe.XETE_MASK_PIPE_4.qp());
      /* p26.XETE*/ pix_pipe.XETE_MASK_PIPE_4.dff22(_SACU_CLKPIPE_AxCxExGx, XUKU_MASK_PIPE_SET4, WEDY_MASK_PIPE_RST4, pix_pipe.WYFU_MASK_PIPE_3.qp());
      /* p26.WYFU*/ pix_pipe.WYFU_MASK_PIPE_3.dff22(_SACU_CLKPIPE_AxCxExGx, XYRU_MASK_PIPE_SET3, WEVO_MASK_PIPE_RST3, pix_pipe.VOSA_MASK_PIPE_2.qp());
      /* p26.VOSA*/ pix_pipe.VOSA_MASK_PIPE_2.dff22(_SACU_CLKPIPE_AxCxExGx, TYRA_MASK_PIPE_SET2, TUFO_MASK_PIPE_RST2, pix_pipe.WURU_MASK_PIPE_1.qp());
      /* p26.WURU*/ pix_pipe.WURU_MASK_PIPE_1.dff22(_SACU_CLKPIPE_AxCxExGx, XALA_MASK_PIPE_SET1, WEDE_MASK_PIPE_RST1, pix_pipe.VEZO_MASK_PIPE_0.qp());
      /* p26.VEZO*/ pix_pipe.VEZO_MASK_PIPE_0.dff22(_SACU_CLKPIPE_AxCxExGx, TEDE_MASK_PIPE_SET0, WOKA_MASK_PIPE_RST0, _VYPO_VCC);
    }
  }
#pragma endregion

#pragma region Joypad
  {
    wire BURO_FF60_0p = 0; // FIXME hacking out debug stuff
    /* p05.KURA*/ wire _KURA_JOYP_DBGn = not1(BURO_FF60_0p);

    /*
    // lcd ribbon voltages after bootrom
    04 5 left & b
    05 0 diodes 1&2
    06 5 down & start
    07 5 up & select
    08 5 right & a
    09 0 diodes 3 & 4
    */

    /* p05.KARU*/ wire _KARU_OEp = or2(joypad.KELY_JOYP_UDLRp.qn(), _KURA_JOYP_DBGn);
    /* p05.CELA*/ wire _CELA_OEp = or2(joypad.COFY_JOYP_ABCSp.qn(), _KURA_JOYP_DBGn);

    Pin2 PIN_JOY_P14; // PIN_63
    Pin2 PIN_JOY_P15; // PIN_62
    PIN_JOY_P14.pin_out(_KARU_OEp, joypad.KELY_JOYP_UDLRp.qn(), joypad.KELY_JOYP_UDLRp.qn());
    PIN_JOY_P15.pin_out(_CELA_OEp, joypad.COFY_JOYP_ABCSp.qn(), joypad.COFY_JOYP_ABCSp.qn());

    // Pressing a button pulls the corresponding pin _down_.

    Pin2 PIN_JOY_P10; // PIN_67
    Pin2 PIN_JOY_P11; // PIN_66
    Pin2 PIN_JOY_P12; // PIN_65
    Pin2 PIN_JOY_P13; // PIN_64

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


    if (PIN_JOY_P14.qp()) {
      PIN_JOY_P10.pin_in(1, !(sys_buttons & 0x01));
      PIN_JOY_P11.pin_in(1, !(sys_buttons & 0x02));
      PIN_JOY_P12.pin_in(1, !(sys_buttons & 0x04));
      PIN_JOY_P13.pin_in(1, !(sys_buttons & 0x08));
    }
    else if (PIN_JOY_P15.qp()) {
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
    /* p02.KERY*/ wire _KERY_ANY_BUTTONp = or4(PIN_JOY_P13.qn(), PIN_JOY_P12.qn(), PIN_JOY_P11.qn(), PIN_JOY_P10.qn());

    {
      /* p02.AWOB*/ joypad.AWOB_WAKE_CPU.tp_latch(_BOGA_Axxxxxxx_clk, _KERY_ANY_BUTTONp);
      wire _AWOB_WAKE_CPUp = joypad.AWOB_WAKE_CPU.qp();

      Pin2 PIN_CPU_WAKE; // top right wire by itself <- P02.AWOB
      PIN_CPU_WAKE.pin_in(1, _AWOB_WAKE_CPUp);

      /* p02.APUG*/ joypad.APUG_JP_GLITCH3.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, joypad.AGEM_JP_GLITCH2.qp());
      /* p02.AGEM*/ joypad.AGEM_JP_GLITCH2.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, joypad.ACEF_JP_GLITCH1.qp());
      /* p02.ACEF*/ joypad.ACEF_JP_GLITCH1.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, joypad.BATU_JP_GLITCH0.qp());
      /* p02.BATU*/ joypad.BATU_JP_GLITCH0.dff17(_BOGA_Axxxxxxx_clk, _ALUR_SYS_RSTn_new, _KERY_ANY_BUTTONp);

      /* p05.KEVU*/ joypad.KEVU_JOYP_L0n.tp_latch(_BYZO_FF00_RDn_new, PIN_JOY_P10.qn());
      /* p05.KAPA*/ joypad.KAPA_JOYP_L1n.tp_latch(_BYZO_FF00_RDn_new, PIN_JOY_P11.qn());
      /* p05.KEJA*/ joypad.KEJA_JOYP_L2n.tp_latch(_BYZO_FF00_RDn_new, PIN_JOY_P12.qn());
      /* p05.KOLO*/ joypad.KOLO_JOYP_L3n.tp_latch(_BYZO_FF00_RDn_new, PIN_JOY_P13.qn());
    }
  }
#pragma endregion

#pragma region Serial
  {
    //----------
    // Serial pins

    /* PIN_68 */ Pin2 PIN_SCK;
    /* PIN_69 */ Pin2 PIN_SIN;
    /* PIN_70 */ Pin2 PIN_SOUT;

    /* hack */    PIN_SIN.pin_out(1, 1, 1);

    /* p06.CULY*/ ser_reg.CULY_XFER_DIR.dff17(_UWAM_FF02_WRn_new, _ALUR_SYS_RSTn_new, BUS_CPU_Dp_in[0].qp());

    /* p01.UVYN*/ wire _UVYN_DIV_05n_next = not1(tim_reg.TAMA_DIV05p.qp());
    /* p06.COTY*/ ser_reg.COTY_SER_CLK.dff17(_UVYN_DIV_05n_next, _UWAM_FF02_WRn_new, ser_reg.COTY_SER_CLK.qn());

    {
      /* p06.KEXU*/ wire _KEXU = nand2(ser_reg.COTY_SER_CLK.qp(), ser_reg.CULY_XFER_DIR.qp());
      /* p06.JAGO*/ wire _JAGO = not1(ser_reg.CULY_XFER_DIR.qp());
      /* p06.KUJO*/ wire _KUJO = nor2(ser_reg.COTY_SER_CLK.qp(), _JAGO);

      // FIXME this doesn't look quite right
      /* p06.KEXU*/ PIN_SCK.pin_out(_KEXU, _KUJO, ser_reg.CULY_XFER_DIR.qp());
    }

    /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn = nand2(BUS_CPU_Dp_in[0].qp(), _DAKU_FF01_WRp_new);
    /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn = nand2(BUS_CPU_Dp_in[1].qp(), _DAKU_FF01_WRp_new);
    /* p06.DELA*/ wire _DELA_SER_DATA2_SETn = nand2(BUS_CPU_Dp_in[2].qp(), _DAKU_FF01_WRp_new);
    /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn = nand2(BUS_CPU_Dp_in[3].qp(), _DAKU_FF01_WRp_new);
    /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn = nand2(BUS_CPU_Dp_in[4].qp(), _DAKU_FF01_WRp_new);
    /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn = nand2(BUS_CPU_Dp_in[5].qp(), _DAKU_FF01_WRp_new);
    /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn = nand2(BUS_CPU_Dp_in[6].qp(), _DAKU_FF01_WRp_new);
    /* p06.EFEF*/ wire _EFEL_SER_DATA7_SETn = nand2(BUS_CPU_Dp_in[7].qp(), _DAKU_FF01_WRp_new);

    /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn = or_and3(_URYS_FF01_WRn_new, BUS_CPU_Dp_in[0].qp(), _ALUR_SYS_RSTn_new);
    /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn = or_and3(_URYS_FF01_WRn_new, BUS_CPU_Dp_in[1].qp(), _ALUR_SYS_RSTn_new);
    /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn = or_and3(_URYS_FF01_WRn_new, BUS_CPU_Dp_in[2].qp(), _ALUR_SYS_RSTn_new);
    /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn = or_and3(_URYS_FF01_WRn_new, BUS_CPU_Dp_in[3].qp(), _ALUR_SYS_RSTn_new);
    /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn = or_and3(_URYS_FF01_WRn_new, BUS_CPU_Dp_in[4].qp(), _ALUR_SYS_RSTn_new);
    /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn = or_and3(_URYS_FF01_WRn_new, BUS_CPU_Dp_in[5].qp(), _ALUR_SYS_RSTn_new);
    /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn = or_and3(_URYS_FF01_WRn_new, BUS_CPU_Dp_in[6].qp(), _ALUR_SYS_RSTn_new);
    /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn = or_and3(_URYS_FF01_WRn_new, BUS_CPU_Dp_in[7].qp(), _ALUR_SYS_RSTn_new);

    /* p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(ser_reg.CULY_XFER_DIR.qp(), ser_reg.COTY_SER_CLK.qp(), PIN_SCK.qn());
    /* p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, ser_reg.ETAF_SER_RUNNING.qn()); // this must stop the clock or something when the transmit's done

    /* p06.CARO*/ wire _CARO_SER_RST = and2(_UWAM_FF02_WRn_new, _ALUR_SYS_RSTn_new);
    /* p06.CAFA*/ ser_reg.CAFA_SER_CNT0.dff17(_DAWA_SER_CLK,                    _CARO_SER_RST, ser_reg.CAFA_SER_CNT0.qn());
    /* p06.CYLO*/ ser_reg.CYLO_SER_CNT1.dff17(ser_reg.CAFA_SER_CNT0.qn(), _CARO_SER_RST, ser_reg.CYLO_SER_CNT1.qn());
    /* p06.CYDE*/ ser_reg.CYDE_SER_CNT2.dff17(ser_reg.CYLO_SER_CNT1.qn(), _CARO_SER_RST, ser_reg.CYDE_SER_CNT2.qn());
    /* p06.CALY*/ ser_reg.CALY_SER_CNT3.dff17(ser_reg.CYDE_SER_CNT2.qn(), _CARO_SER_RST, ser_reg.CALY_SER_CNT3.qn());

    /* p06.COBA*/ wire _COBA_SER_CNT3n  = not1(ser_reg.CALY_SER_CNT3.qp());
    /* p06.CABY*/ wire _CABY_XFER_RESET = and2(_COBA_SER_CNT3n, _ALUR_SYS_RSTn_new);
    /* p06.ETAF*/ ser_reg.ETAF_SER_RUNNING.dff17(_UWAM_FF02_WRn_new, _CABY_XFER_RESET, BUS_CPU_Dp_in[7].qp());

    /* p06.CAGE*/ wire _CAGE_SER_IN  = not1(PIN_SIN.qn());
    /* p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
    /* p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
    /* p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
    /* p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

    /* p06.ELYS*/ ser_reg.ELYS_SER_OUT  .dff17(_EDYL_SER_CLK, _ALUR_SYS_RSTn_new,                           ser_reg.EDER_SER_DATA7.qp());
    /* p06.EDER*/ ser_reg.EDER_SER_DATA7.dff22(_EPYT_SER_CLK, _EFEL_SER_DATA7_SETn, _EGUV_SER_DATA7_RSTn, ser_reg.EROD_SER_DATA6.qp());
    /* p06.EROD*/ ser_reg.EROD_SER_DATA6.dff22(_EPYT_SER_CLK, _EDEL_SER_DATA6_SETn, _EFAK_SER_DATA6_RSTn, ser_reg.EJAB_SER_DATA5.qp());
    /* p06.EJAB*/ ser_reg.EJAB_SER_DATA5.dff22(_EPYT_SER_CLK, _ELOK_SER_DATA5_SETn, _EHUJ_SER_DATA5_RSTn, ser_reg.DOVU_SER_DATA4.qp());
    /* p06.DOVU*/ ser_reg.DOVU_SER_DATA4.dff22(_EPYT_SER_CLK, _DOLA_SER_DATA4_SETn, _DYLY_SER_DATA4_RSTn, ser_reg.DOJO_SER_DATA3.qp());
    /* p06.DOJO*/ ser_reg.DOJO_SER_DATA3.dff22(_DAWE_SER_CLK, _DYGE_SER_DATA3_SETn, _DAJU_SER_DATA3_RSTn, ser_reg.DYRA_SER_DATA2.qp());
    /* p06.DYRA*/ ser_reg.DYRA_SER_DATA2.dff22(_DAWE_SER_CLK, _DELA_SER_DATA2_SETn, _DYBO_SER_DATA2_RSTn, ser_reg.DEGU_SER_DATA1.qp());
    /* p06.DEGU*/ ser_reg.DEGU_SER_DATA1.dff22(_DAWE_SER_CLK, _DOCU_SER_DATA1_SETn, _DUMO_SER_DATA1_RSTn, ser_reg.CUBA_SER_DATA0.qp());
    /* p06.CUBA*/ ser_reg.CUBA_SER_DATA0.dff22(_DAWE_SER_CLK, _CUFU_SER_DATA0_SETn, _COHY_SER_DATA0_RSTn, _CAGE_SER_IN);

    // FIXME hacking out debug stuff
    ///* p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);
    /* p05.KENA*/ PIN_SOUT.pin_out(1, ser_reg.ELYS_SER_OUT.qp(), ser_reg.ELYS_SER_OUT.qp());

  }
#pragma endregion

#pragma region Interrupts
  {
    Pin2 PIN_CPU_INT_VBLANK; // bottom right port PORTB_03: <- P02.LOPE, vblank int
    Pin2 PIN_CPU_INT_STAT  ; // bottom right port PORTB_07: <- P02.LALU, stat int
    Pin2 PIN_CPU_INT_TIMER ; // bottom right port PORTB_11: <- P02.NYBO, timer int
    Pin2 PIN_CPU_INT_SERIAL; // bottom right port PORTB_15: <- P02.UBUL, serial int
    Pin2 PIN_CPU_INT_JOYPAD; // bottom right port PORTB_19: <- P02.ULAK, joypad int

    // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
    // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
    // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
    // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
    // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

    /* p02.LETY*/ wire _LETY_INT_VBL_ACKn  = not1(PIN_CPU_ACK_VBLANK.qp());
    /* p02.LEJA*/ wire _LEJA_INT_STAT_ACKn = not1(PIN_CPU_ACK_STAT.qp());
    /* p02.LESA*/ wire _LESA_INT_TIM_ACKn  = not1(PIN_CPU_ACK_TIMER.qp());
    /* p02.LUFE*/ wire _LUFE_INT_SER_ACKn  = not1(PIN_CPU_ACK_SERIAL.qp());
    /* p02.LAMO*/ wire _LAMO_INT_JOY_ACKn  = not1(PIN_CPU_ACK_JOYPAD.qp());

    /* p02.MYZU*/ wire _MYZU_FF0F_SET0n = nand3(_ROTU_FF0F_WRp_new, _LETY_INT_VBL_ACKn,  BUS_CPU_Dp_in[0].qp());
    /* p02.MODY*/ wire _MODY_FF0F_SET1n = nand3(_ROTU_FF0F_WRp_new, _LEJA_INT_STAT_ACKn, BUS_CPU_Dp_in[1].qp());
    /* p02.PYHU*/ wire _PYHU_FF0F_SET2n = nand3(_ROTU_FF0F_WRp_new, _LESA_INT_TIM_ACKn,  BUS_CPU_Dp_in[2].qp());
    /* p02.TOME*/ wire _TOME_FF0F_SET3n = nand3(_ROTU_FF0F_WRp_new, _LUFE_INT_SER_ACKn,  BUS_CPU_Dp_in[3].qp());
    /* p02.TOGA*/ wire _TOGA_FF0F_SET4n = nand3(_ROTU_FF0F_WRp_new, _LAMO_INT_JOY_ACKn,  BUS_CPU_Dp_in[4].qp());

    /*#p02.MUXE*/ wire _MUXE_INT0_WRn = or2(BUS_CPU_Dp_in[0].qp(), _REFA_FF0F_WRn_new);
    /* p02.NABE*/ wire _NABE_INT1_WRn = or2(BUS_CPU_Dp_in[1].qp(), _REFA_FF0F_WRn_new);
    /* p02.RAKE*/ wire _RAKE_INT2_WRn = or2(BUS_CPU_Dp_in[2].qp(), _REFA_FF0F_WRn_new);
    /* p02.SULO*/ wire _SULO_INT3_WRn = or2(BUS_CPU_Dp_in[3].qp(), _REFA_FF0F_WRn_new);
    /* p02.SEME*/ wire _SEME_INT4_WRn = or2(BUS_CPU_Dp_in[4].qp(), _REFA_FF0F_WRn_new);

    /*#p02.LYTA*/ wire _LYTA_FF0F_RST0n = and3(_MUXE_INT0_WRn, _LETY_INT_VBL_ACKn,  _ALUR_SYS_RSTn_new);
    /* p02.MOVU*/ wire _MOVU_FF0F_RST1n = and3(_NABE_INT1_WRn, _LEJA_INT_STAT_ACKn, _ALUR_SYS_RSTn_new);
    /* p02.PYGA*/ wire _PYGA_FF0F_RST2n = and3(_RAKE_INT2_WRn, _LESA_INT_TIM_ACKn,  _ALUR_SYS_RSTn_new);
    /* p02.TUNY*/ wire _TUNY_FF0F_RST3n = and3(_SULO_INT3_WRn, _LUFE_INT_SER_ACKn,  _ALUR_SYS_RSTn_new);
    /* p02.TYME*/ wire _TYME_FF0F_RST4n = and3(_SEME_INT4_WRn, _LAMO_INT_JOY_ACKn,  _ALUR_SYS_RSTn_new);

    /*#p21.PARU*/ wire _PARU_VBLANKp = not1(lcd_reg.POPU_VBLANK_CLKp.qn());
    /*#p21.TOLU*/ wire _TOLU_VBLANKn    = not1(_PARU_VBLANKp);
    /*#p21.PURE*/ wire _PURE_LINE_ENDn = not1(lcd_reg.RUTU_x113p.qp());
    /*#p21.SELA*/ wire _SELA_LINE_ENDp  = not1(_PURE_LINE_ENDn);
    /*#p21.TAPA*/ wire _TAPA_INT_OAM_t3   = and2(_TOLU_VBLANKn, _SELA_LINE_ENDp);
    /*#p21.TARU*/ wire _TARU_INT_HBL_t3   = and2(_WODU_HBLANKp_new, _TOLU_VBLANKn);
    /*#p21.SUKO*/ wire _SUKO_INT_STATp_t3 = amux4(pix_pipe.RUGU_STAT_LYI_ENn_s.qn(), lcd_reg.ROPO_LY_MATCH_SYNCp.qp(),
                                                  pix_pipe.REFE_STAT_OAI_ENn_s.qn(), _TAPA_INT_OAM_t3,
                                                  pix_pipe.RUFO_STAT_VBI_ENn_s.qn(), _PARU_VBLANKp, // polarity?
                                                  pix_pipe.ROXE_STAT_HBI_ENn_s.qn(), _TARU_INT_HBL_t3);

    /*#p21.VYPU*/ wire _VYPU_INT_VBLANKp_t3 = not1(_TOLU_VBLANKn);
    /* p02.ASOK*/ wire _ASOK_INT_JOYp       = and2(joypad.APUG_JP_GLITCH3.qp(), joypad.BATU_JP_GLITCH0.qp());
    /*#p21.TUVA*/ wire _TUVA_INT_STATn_t3   = not1(_SUKO_INT_STATp_t3);
    /*#p21.VOTY*/ wire _VOTY_INT_STATp_t3   = not1(_TUVA_INT_STATn_t3);

    /* p02.LOPE*/ int_reg.LOPE_FF0F_D0p.dff22(_VYPU_INT_VBLANKp_t3,                    _MYZU_FF0F_SET0n, _LYTA_FF0F_RST0n, _PESU_VCC);
    /* p02.LALU*/ int_reg.LALU_FF0F_D1p.dff22(_VOTY_INT_STATp_t3,                      _MODY_FF0F_SET1n, _MOVU_FF0F_RST1n, _PESU_VCC);
    /* p02.NYBO*/ int_reg.NYBO_FF0F_D2p.dff22(tim_reg.MOBA_TIMER_OVERFLOWp.qp(), _PYHU_FF0F_SET2n, _PYGA_FF0F_RST2n, _PESU_VCC);
    /* p02.UBUL*/ int_reg.UBUL_FF0F_D3p.dff22(ser_reg.CALY_SER_CNT3.qp(),        _TOME_FF0F_SET3n, _TUNY_FF0F_RST3n, _PESU_VCC); // FIXME should use new
    /* p02.ULAK*/ int_reg.ULAK_FF0F_D4p.dff22(_ASOK_INT_JOYp,                          _TOGA_FF0F_SET4n, _TYME_FF0F_RST4n, _PESU_VCC);

    /* p02.LOPE*/ wire _LOPE_FF0F_D0p_t4 = int_reg.LOPE_FF0F_D0p.qp();
    /* p02.LALU*/ wire _LALU_FF0F_D1p_t4 = int_reg.LALU_FF0F_D1p.qp();
    /* p02.NYBO*/ wire _NYBO_FF0F_D2p_t4 = int_reg.NYBO_FF0F_D2p.qp();
    /* p02.UBUL*/ wire _UBUL_FF0F_D3p_t4 = int_reg.UBUL_FF0F_D3p.qp();
    /* p02.ULAK*/ wire _ULAK_FF0F_D4p_t4 = int_reg.ULAK_FF0F_D4p.qp();

    PIN_CPU_INT_VBLANK.pin_in(1, _LOPE_FF0F_D0p_t4);
    PIN_CPU_INT_STAT  .pin_in(1, _LALU_FF0F_D1p_t4);
    PIN_CPU_INT_TIMER .pin_in(1, _NYBO_FF0F_D2p_t4);
    PIN_CPU_INT_SERIAL.pin_in(1, _UBUL_FF0F_D3p_t4);
    PIN_CPU_INT_JOYPAD.pin_in(1, _ULAK_FF0F_D4p_t4);

    if (DELTA_DE) {
      int_vblank_halt = PIN_CPU_INT_VBLANK.qp();
      int_stat_halt   = PIN_CPU_INT_STAT.qp();
      int_serial_halt = PIN_CPU_INT_SERIAL.qp();
      int_joypad_halt = PIN_CPU_INT_JOYPAD.qp();
    }

    if (DELTA_GH) {
      // this one latches funny, some hardware bug
      int_timer_halt = PIN_CPU_INT_TIMER.qp();

      int_vblank = PIN_CPU_INT_VBLANK.qp();
      int_stat   = PIN_CPU_INT_STAT.qp();
      int_timer  = PIN_CPU_INT_TIMER.qp();
      int_serial = PIN_CPU_INT_SERIAL.qp();
      int_joypad = PIN_CPU_INT_JOYPAD.qp();
    }

    {
      // FFFF - IE
      // This is technically in the CPU, but we're going to implement it here for now.
      wire FFFF_WRn = nand2(_TAPU_CPU_WRp_xxxxEFGx_new, FFFF_HIT_new);

      IE_D0.dff(FFFF_WRn, 1, !sys_rst, BUS_CPU_Dp_in[0].qp());
      IE_D1.dff(FFFF_WRn, 1, !sys_rst, BUS_CPU_Dp_in[1].qp());
      IE_D2.dff(FFFF_WRn, 1, !sys_rst, BUS_CPU_Dp_in[2].qp());
      IE_D3.dff(FFFF_WRn, 1, !sys_rst, BUS_CPU_Dp_in[3].qp());
      IE_D4.dff(FFFF_WRn, 1, !sys_rst, BUS_CPU_Dp_in[4].qp());

      if (DELTA_HA) {
        imask_latch = pack_u8p_new(5, &IE_D0);
      }
    }
  }
#pragma endregion



  /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(BUS_OAM_An[0].qp());
  /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);
  /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn      = nor3(dma_reg.MATU_DMA_RUNNINGp.qp(), _ACYL_SCANNINGp, _AJON_PPU_OAM_ENp); // def nor
  /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(_SARO_FE00_FEFFp, _AJUJ_OAM_BUSYn); // def and




#pragma region OAM_write
  Bus2 BUS_OAM_DAn_out[8];
  Bus2 BUS_OAM_DBn_out[8];

  /* DMA EBD -> OBD */
  {
    /* p25.RALO*/ wire _RALO_EXT_D0p = not1(PIN_EXT_D_in[0].qn());
    /* p25.TUNE*/ wire _TUNE_EXT_D1p = not1(PIN_EXT_D_in[1].qn());
    /* p25.SERA*/ wire _SERA_EXT_D2p = not1(PIN_EXT_D_in[2].qn());
    /* p25.TENU*/ wire _TENU_EXT_D3p = not1(PIN_EXT_D_in[3].qn());
    /* p25.SYSA*/ wire _SYSA_EXT_D4p = not1(PIN_EXT_D_in[4].qn());
    /* p25.SUGY*/ wire _SUGY_EXT_D5p = not1(PIN_EXT_D_in[5].qn());
    /* p25.TUBE*/ wire _TUBE_EXT_D6p = not1(PIN_EXT_D_in[6].qn());
    /* p25.SYZO*/ wire _SYZO_EXT_D7p = not1(PIN_EXT_D_in[7].qn());

    /* p25.CEDE*/ wire _CEDE_EBD_TO_OBDn = not1(_LUMA_DMA_CARTp_new);
    /* p25.WASA*/ BUS_OAM_DAn_out[0].tri6_nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /* p25.BOMO*/ BUS_OAM_DAn_out[1].tri6_nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /* p25.BASA*/ BUS_OAM_DAn_out[2].tri6_nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /* p25.CAKO*/ BUS_OAM_DAn_out[3].tri6_nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /* p25.BUMA*/ BUS_OAM_DAn_out[4].tri6_nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /* p25.BUPY*/ BUS_OAM_DAn_out[5].tri6_nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /* p25.BASY*/ BUS_OAM_DAn_out[6].tri6_nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /* p25.BAPE*/ BUS_OAM_DAn_out[7].tri6_nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);

    /* p25.WEJO*/ BUS_OAM_DBn_out[0].tri6_nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /* p25.BUBO*/ BUS_OAM_DBn_out[1].tri6_nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /* p25.BETU*/ BUS_OAM_DBn_out[2].tri6_nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /* p25.CYME*/ BUS_OAM_DBn_out[3].tri6_nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /* p25.BAXU*/ BUS_OAM_DBn_out[4].tri6_nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /* p25.BUHU*/ BUS_OAM_DBn_out[5].tri6_nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /* p25.BYNY*/ BUS_OAM_DBn_out[6].tri6_nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /* p25.BYPY*/ BUS_OAM_DBn_out[7].tri6_nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);
  }

  /* DMA VBD -> OBD */
  {
    /* p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not1(_LUFA_DMA_VRAMp_new);
    /* p28.WUZU*/ BUS_OAM_DAn_out[0].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[0].qp());
    /* p28.AXER*/ BUS_OAM_DAn_out[1].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[1].qp());
    /* p28.ASOX*/ BUS_OAM_DAn_out[2].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[2].qp());
    /* p28.CETU*/ BUS_OAM_DAn_out[3].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[3].qp());
    /* p28.ARYN*/ BUS_OAM_DAn_out[4].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[4].qp());
    /* p28.ACOT*/ BUS_OAM_DAn_out[5].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[5].qp());
    /* p28.CUJE*/ BUS_OAM_DAn_out[6].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[6].qp());
    /* p28.ATER*/ BUS_OAM_DAn_out[7].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[7].qp());

    /* p28.WOWA*/ BUS_OAM_DBn_out[0].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[0].qp());
    /* p28.AVEB*/ BUS_OAM_DBn_out[1].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[1].qp());
    /* p28.AMUH*/ BUS_OAM_DBn_out[2].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[2].qp());
    /* p28.COFO*/ BUS_OAM_DBn_out[3].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[3].qp());
    /* p28.AZOZ*/ BUS_OAM_DBn_out[4].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[4].qp());
    /* p28.AGYK*/ BUS_OAM_DBn_out[5].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[5].qp());
    /* p28.BUSE*/ BUS_OAM_DBn_out[6].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[6].qp());
    /* p28.ANUM*/ BUS_OAM_DBn_out[7].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[7].qp());
  }

  {
    /* p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and2(_SARO_FE00_FEFFp, _CUPA_CPU_WRp_xxxxEFGx_new);
    /* p28.XYNY*/ wire _XYNY_ABCDxxxx = not1(_MOPA_xxxxEFGH_clk);
    /* p28.WUJE*/ oam_bus.WUJE_CPU_OAM_WRn.nor_latch(_XYNY_ABCDxxxx, _XUTO_CPU_OAM_WRp);
    /* p28.XUPA*/ wire _XUPA_CPU_OAM_WRp = not1(oam_bus.WUJE_CPU_OAM_WRn.qp());
    /* p28.APAG*/ wire _APAG_CBD_TO_OBDp = amux2(_XUPA_CPU_OAM_WRp, _AMAB_CPU_OAM_ENp, _AJUJ_OAM_BUSYn, _ADAH_FE00_FEFFn);
    /* p28.AZUL*/ wire _AZUL_CBD_TO_OBDn = not1(_APAG_CBD_TO_OBDp);
    /* p28.ZAXA*/ BUS_OAM_DAn_out[0].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[0].qp());
    /* p28.ZAKY*/ BUS_OAM_DAn_out[1].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[1].qp());
    /* p28.WULE*/ BUS_OAM_DAn_out[2].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[2].qp());
    /* p28.ZOZO*/ BUS_OAM_DAn_out[3].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[3].qp());
    /* p28.ZUFO*/ BUS_OAM_DAn_out[4].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[4].qp());
    /* p28.ZATO*/ BUS_OAM_DAn_out[5].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[5].qp());
    /* p28.YVUC*/ BUS_OAM_DAn_out[6].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[6].qp());
    /* p28.ZUFE*/ BUS_OAM_DAn_out[7].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[7].qp());

    /* p28.ZAMY*/ BUS_OAM_DBn_out[0].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[0].qp());
    /* p28.ZOPU*/ BUS_OAM_DBn_out[1].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[1].qp());
    /* p28.WYKY*/ BUS_OAM_DBn_out[2].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[2].qp());
    /* p28.ZAJA*/ BUS_OAM_DBn_out[3].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[3].qp());
    /* p28.ZUGA*/ BUS_OAM_DBn_out[4].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[4].qp());
    /* p28.ZUMO*/ BUS_OAM_DBn_out[5].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[5].qp());
    /* p28.XYTO*/ BUS_OAM_DBn_out[6].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[6].qp());
    /* p28.ZYFA*/ BUS_OAM_DBn_out[7].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[7].qp());
  }

  {
    Pin2 PIN_OAM_CLK;
    PIN_OAM_CLK.pin_in(1, _COTA_AxxxxExxx);

    /* p04.MAKA*/ oam_bus.MAKA_HOLD_MEMp.dff17(_ZEME_AxCxExGx_clk, _CUNU_SYS_RSTn_new, _CATY_LATCH_EXTp);
    /* p04.NAXY*/ wire _NAXY_DMA_OAM_WRp = nor2(_UVYT_ABCDxxxx_clk, oam_bus.MAKA_HOLD_MEMp.qp()); // def nor2
    /* p04.POWU*/ wire _POWU_DMA_OAM_WRp  = and2(dma_reg.MATU_DMA_RUNNINGp.qp(), _NAXY_DMA_OAM_WRp); // def and
    /* p04.WYJA*/ wire _WYJA_OAM_WRp   = and_or3(_AMAB_CPU_OAM_ENp, _CUPA_CPU_WRp_xxxxEFGx_new, _POWU_DMA_OAM_WRp);
    /* p28.YNYC*/ wire _YNYC_OAM_A_WRp = and2(_WYJA_OAM_WRp, _WAFO_OAM_A0n);
    /* p28.YLYC*/ wire _YLYC_OAM_B_WRp = and2(_WYJA_OAM_WRp, _GEKA_OAM_A0p);
    /* p28.ZOFE*/ wire _ZOFE_OAM_A_WRn = not1(_YNYC_OAM_A_WRp);
    /* p28.ZONE*/ wire _ZONE_OAM_B_WRn = not1(_YLYC_OAM_B_WRp);

    // FIXME This should be using PIN_OAM_CLK (which might actually be PIN_OAM_CSn?)
    Pin2 PIN_OAM_WR_A;
    Pin2 PIN_OAM_WR_B;

    PIN_OAM_WR_A.pin_in(1, _ZOFE_OAM_A_WRn);
    PIN_OAM_WR_B.pin_in(1, _ZONE_OAM_B_WRn);

    if (!PIN_OAM_WR_A.qp()) {
      oam_ram[(oam_addr << 1) + 0] = pack_u8n_new(8, &BUS_OAM_DAn_out[0]);
    }

    if (!PIN_OAM_WR_B.qp()) {
      oam_ram[(oam_addr << 1) + 1] = pack_u8n_new(8, &BUS_OAM_DBn_out[0]);
    }
  }
#pragma endregion

#pragma region VRAM_write
  {
    Bus2 BUS_VRAM_Dp_out[8];
    /* CBD -> VBD */
    /* p25.ROCY*/ wire _ROCY_CBD_TO_VPDp = and2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
    /* p25.RAHU*/ wire _RAHU_CBD_TO_VPDn = not1(_ROCY_CBD_TO_VPDp);
    /*#p25.ROVE*/ wire _ROVE_CBD_TO_VPDp = not1(_RAHU_CBD_TO_VPDn);

    {
      /* p25.TEME*/ BUS_VRAM_Dp_out[0].tri10_np(_RAHU_CBD_TO_VPDn, BUS_CPU_Dp_in[0].qp());
      /* p25.TEWU*/ BUS_VRAM_Dp_out[1].tri10_np(_RAHU_CBD_TO_VPDn, BUS_CPU_Dp_in[1].qp());
      /*#p25.TYGO*/ BUS_VRAM_Dp_out[2].tri10_np(_RAHU_CBD_TO_VPDn, BUS_CPU_Dp_in[2].qp());
      /* p25.SOTE*/ BUS_VRAM_Dp_out[3].tri10_np(_RAHU_CBD_TO_VPDn, BUS_CPU_Dp_in[3].qp());
      /* p25.SEKE*/ BUS_VRAM_Dp_out[4].tri10_np(_RAHU_CBD_TO_VPDn, BUS_CPU_Dp_in[4].qp());
      /* p25.RUJO*/ BUS_VRAM_Dp_out[5].tri10_np(_RAHU_CBD_TO_VPDn, BUS_CPU_Dp_in[5].qp());
      /* p25.TOFA*/ BUS_VRAM_Dp_out[6].tri10_np(_RAHU_CBD_TO_VPDn, BUS_CPU_Dp_in[6].qp());
      /* p25.SUZA*/ BUS_VRAM_Dp_out[7].tri10_np(_RAHU_CBD_TO_VPDn, BUS_CPU_Dp_in[7].qp());
    }


    {
      /*#p25.SUTU*/ wire _SUTU_MCSn = nor4(_LENA_BGW_VRAM_RDp, _LUFA_DMA_VRAMp_new, _TEXY_SPR_READ_VRAMp, _SERE_CPU_VRM_RDp);
      /* p25.TODE*/ wire _TODE_MCSn_A = and2(_SUTU_MCSn, _RACO_DBG_VRAMn_new);
      /* p25.SEWO*/ wire _SEWO_MCSn_D = or2(_SUTU_MCSn, _TUTO_DBG_VRAMp_new);
      /* p25.SOKY*/ wire _SOKY_MCSp_A = not1(_TODE_MCSn_A);
      /* p25.SETY*/ wire _SETY_MCSp_D = not1(_SEWO_MCSn_D);

      // FIXME not actually using this pin, but we should
      Pin2 PIN_VRAM_CSn; // PIN_43
      PIN_VRAM_CSn.pin_out(1, _SOKY_MCSp_A, _SETY_MCSp_D);
    }




    {
      /* p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp = and2(_SOSE_8000_9FFFp_new, _APOV_CPU_WRp_xxxxEFGx_new);
      /* p25.SUDO*/ wire _SUDO_MWRp = not1(/*vram_bus.PIN_VRAM_WRn.qn()*/ 1); // Ignoring debug stuff for now
      /* p25.TYJY*/ wire _TYJY_VRAM_WRp = mux2p(_TUTO_DBG_VRAMp_new, _SUDO_MWRp, _TUJA_CPU_VRAM_WRp);
      /* p25.SOHY*/ wire _SOHY_MWRn = nand2(_TYJY_VRAM_WRp, _SERE_CPU_VRM_RDp);
      /* p25.TAXY*/ wire _TAXY_MWRn_A = and2(_SOHY_MWRn, _RACO_DBG_VRAMn_new);
      /* p25.SOFY*/ wire _SOFY_MWRn_D = or2(_SOHY_MWRn, _TUTO_DBG_VRAMp_new);
      /* p25.SYSY*/ wire _SYSY_MWRp_A = not1(_TAXY_MWRn_A);
      /* p25.RAGU*/ wire _RAGU_MWRp_D = not1(_SOFY_MWRn_D);

      Pin2 PIN_VRAM_WRn; // PIN_49
      PIN_VRAM_WRn.pin_out(1, _SYSY_MWRp_A, _RAGU_MWRp_D);


      /*#p25.SEFA*/ wire _SEFA_D0p = and2(BUS_VRAM_Dp_out[0].qp(), _ROVE_CBD_TO_VPDp);
      /* p25.SOGO*/ wire _SOGO_D1p = and2(BUS_VRAM_Dp_out[1].qp(), _ROVE_CBD_TO_VPDp);
      /* p25.SEFU*/ wire _SEFU_D2p = and2(BUS_VRAM_Dp_out[2].qp(), _ROVE_CBD_TO_VPDp);
      /* p25.SUNA*/ wire _SUNA_D3p = and2(BUS_VRAM_Dp_out[3].qp(), _ROVE_CBD_TO_VPDp);
      /* p25.SUMO*/ wire _SUMO_D4p = and2(BUS_VRAM_Dp_out[4].qp(), _ROVE_CBD_TO_VPDp);
      /* p25.SAZU*/ wire _SAZU_D5p = and2(BUS_VRAM_Dp_out[5].qp(), _ROVE_CBD_TO_VPDp);
      /* p25.SAMO*/ wire _SAMO_D6p = and2(BUS_VRAM_Dp_out[6].qp(), _ROVE_CBD_TO_VPDp);
      /* p25.SUKE*/ wire _SUKE_D7p = and2(BUS_VRAM_Dp_out[7].qp(), _ROVE_CBD_TO_VPDp);

      /*#p25.SYNU*/ wire _SYNU_D0p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp_out[0].qp());
      /* p25.SYMA*/ wire _SYMA_D1p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp_out[1].qp());
      /* p25.ROKO*/ wire _ROKO_D2p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp_out[2].qp());
      /* p25.SYBU*/ wire _SYBU_D3p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp_out[3].qp());
      /* p25.SAKO*/ wire _SAKO_D4p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp_out[4].qp());
      /* p25.SEJY*/ wire _SEJY_D5p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp_out[5].qp());
      /* p25.SEDO*/ wire _SEDO_D6p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp_out[6].qp());
      /* p25.SAWU*/ wire _SAWU_D7p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp_out[7].qp());

      /*#p25.REGE*/ wire _REGE_D0n = not1(_SEFA_D0p);
      /* p25.RYKY*/ wire _RYKY_D1n = not1(_SOGO_D1p);
      /* p25.RAZO*/ wire _RAZO_D2n = not1(_SEFU_D2p);
      /* p25.RADA*/ wire _RADA_D3n = not1(_SUNA_D3p);
      /* p25.RYRO*/ wire _RYRO_D4n = not1(_SUMO_D4p);
      /* p25.REVU*/ wire _REVU_D5n = not1(_SAZU_D5p);
      /* p25.REKU*/ wire _REKU_D6n = not1(_SAMO_D6p);
      /* p25.RYZE*/ wire _RYZE_D7n = not1(_SUKE_D7p);

      /*#p25.RURA*/ wire _RURA_D0n = not1(_SYNU_D0p);
      /* p25.RULY*/ wire _RULY_D1n = not1(_SYMA_D1p);
      /* p25.RARE*/ wire _RARE_D2n = not1(_ROKO_D2p);
      /* p25.RODU*/ wire _RODU_D3n = not1(_SYBU_D3p);
      /* p25.RUBE*/ wire _RUBE_D4n = not1(_SAKO_D4p);
      /* p25.RUMU*/ wire _RUMU_D5n = not1(_SEJY_D5p);
      /* p25.RYTY*/ wire _RYTY_D6n = not1(_SEDO_D6p);
      /* p25.RADY*/ wire _RADY_D7n = not1(_SAWU_D7p);

      /*#p25.ROFA*/ wire _ROFA_CBD_TO_VPDp = not1(_RENA_CBD_TO_VPDn);

      Pin2 PIN_VRAM_Dp_out[8];
      PIN_VRAM_Dp_out[0].pin_out(_ROFA_CBD_TO_VPDp, _REGE_D0n, _RURA_D0n);
      PIN_VRAM_Dp_out[1].pin_out(_ROFA_CBD_TO_VPDp, _RYKY_D1n, _RULY_D1n);
      PIN_VRAM_Dp_out[2].pin_out(_ROFA_CBD_TO_VPDp, _RAZO_D2n, _RARE_D2n);
      PIN_VRAM_Dp_out[3].pin_out(_ROFA_CBD_TO_VPDp, _RADA_D3n, _RODU_D3n);
      PIN_VRAM_Dp_out[4].pin_out(_ROFA_CBD_TO_VPDp, _RYRO_D4n, _RUBE_D4n);
      PIN_VRAM_Dp_out[5].pin_out(_ROFA_CBD_TO_VPDp, _REVU_D5n, _RUMU_D5n);
      PIN_VRAM_Dp_out[6].pin_out(_ROFA_CBD_TO_VPDp, _REKU_D6n, _RYTY_D6n);
      PIN_VRAM_Dp_out[7].pin_out(_ROFA_CBD_TO_VPDp, _RYZE_D7n, _RADY_D7n);

      // We're getting a fake write on the first phase because PIN_VRAM_WRn resets to 0...
      // ignore it if we're in reset

      if (!sys_rst && !PIN_VRAM_WRn.qp()) {
        vid_ram[vram_addr] = pack_u8p_new(8, PIN_VRAM_Dp_out);
      }
    }
  }
#pragma endregion
















































  //------------------------------------------------------------------------------

  /* LCD Pixel Pipe */
  {
    /*PIN_50*/ Pin2 PIN_LCD_DATA1;
    /*PIN_51*/ Pin2 PIN_LCD_DATA0;
    {
      // Pixel merge + emit
      /*#p35.RAJY*/ wire RAJY_PIX_BG_LOp  = and2(pix_pipe.PYBO_BG_PIPE_A7.qp(), pix_pipe.VYXE_LCDC_BGENn.qn());
      /*#p35.TADE*/ wire TADE_PIX_BG_HIp  = and2(pix_pipe.SOHU_BG_PIPE_B7.qp(), pix_pipe.VYXE_LCDC_BGENn.qn());
      /*#p35.XULA*/ wire XULA_PIX_SP_LOp  = and2(pix_pipe.XYLO_LCDC_SPENn.qn(), pix_pipe.WUFY_SPR_PIPE_A7.qp());
      /*#p35.WOXA*/ wire WOXA_PIX_SP_HIp  = and2(pix_pipe.XYLO_LCDC_SPENn.qn(), pix_pipe.VUPY_SPR_PIPE_B7.qp());

      /*#p35.NULY*/ wire NULY_PIX_SP_MASKn = nor2(WOXA_PIX_SP_HIp, XULA_PIX_SP_LOp);

      /*#p35.RYFU*/ wire RYFU_MASK_BG0 = and2(RAJY_PIX_BG_LOp, pix_pipe.VAVA_MASK_PIPE_7.qp());
      /*#p35.RUTA*/ wire RUTA_MASK_BG1 = and2(TADE_PIX_BG_HIp, pix_pipe.VAVA_MASK_PIPE_7.qp());
      /*#p35.POKA*/ wire POKA_BGPIXELn = nor3(NULY_PIX_SP_MASKn, RUTA_MASK_BG1, RYFU_MASK_BG0);

      /*#p34.LOME*/ wire LOME_PAL_PIPE_7n = not1(pix_pipe.LYME_PAL_PIPE_7.qp());
      /*#p34.LAFU*/ wire LAFU_OBP0PIXELn = nand2(LOME_PAL_PIPE_7n, POKA_BGPIXELn);
      /*#p34.LEKA*/ wire LEKA_OBP1PIXELn = nand2(pix_pipe.LYME_PAL_PIPE_7.qp(), POKA_BGPIXELn);

      //----------
      // Sprite palette 0 lookup

      /*#p35.WELE*/ wire WELE_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
      /*#p35.WOLO*/ wire WOLO_PIX_SP_LOp = not1(WELE_PIX_SP_LOn);
      /*#p35.VUMU*/ wire VUMU_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
      /*#p35.WYRU*/ wire WYRU_PIX_SP_HIp = not1(VUMU_PIX_SP_HIn);

      /*#p35.LAVA*/ wire LAVA_MASK_OPB0  = not1(LAFU_OBP0PIXELn);

      /*#p35.VUGO*/ wire VUGO_PAL_OBP0A = and3(VUMU_PIX_SP_HIn, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
      /*#p35.VOLO*/ wire VOLO_PAL_OBP0B = and3(VUMU_PIX_SP_HIn, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm
      /*#p35.VATA*/ wire VATA_PAL_OBP0C = and3(WYRU_PIX_SP_HIp, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
      /*#p35.VYRO*/ wire VYRO_PAL_OBP0D = and3(WYRU_PIX_SP_HIp, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm

      /*#p35.WUFU*/ wire WUFU_COL_OBP0_HI = amux4(pix_pipe.XANA_OBP0_D7n_s.qn(), VYRO_PAL_OBP0D,
                                                  pix_pipe.XYZE_OBP0_D5n_s.qn(), VATA_PAL_OBP0C,
                                                  pix_pipe.XALO_OBP0_D3n_s.qn(), VOLO_PAL_OBP0B,
                                                  pix_pipe.XUKY_OBP0_D1n_s.qn(), VUGO_PAL_OBP0A);

      /*#p35.WALY*/ wire WALY_COL_OBP0_LO = amux4(pix_pipe.XUPO_OBP0_D6n_s.qn(), VYRO_PAL_OBP0D,
                                                  pix_pipe.XERU_OBP0_D4n_s.qn(), VATA_PAL_OBP0C,
                                                  pix_pipe.XOVA_OBP0_D2n_s.qn(), VOLO_PAL_OBP0B,
                                                  pix_pipe.XUFU_OBP0_D0n_s.qn(), VUGO_PAL_OBP0A);

      //----------
      // Sprite palette 1 lookup

      /*#p35.MABY*/ wire MABY_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
      /*#p35.LYLE*/ wire LYLE_PIX_SP_LOp = not1(MABY_PIX_SP_LOn);
      /*#p35.MEXA*/ wire MEXA_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
      /*#p35.LOZO*/ wire LOZO_PIX_SP_HIp = not1(MEXA_PIX_SP_HIn);

      /*#p35.LUKU*/ wire LUKU_MASK_OBP1  = not1(LEKA_OBP1PIXELn);

      /* p#35.LOPU*/ wire LOPU_PAL_OBP1A = and3(MEXA_PIX_SP_HIn, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
      /* p#35.LYKY*/ wire LYKY_PAL_OBP1B = and3(MEXA_PIX_SP_HIn, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm
      /* p#35.LARU*/ wire LARU_PAL_OBP1C = and3(LOZO_PIX_SP_HIp, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
      /* p#35.LEDO*/ wire LEDO_PAL_OBP1D = and3(LOZO_PIX_SP_HIp, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm

      /*#p35.MOKA*/ wire MOKA_COL_OBP1_HI = amux4(pix_pipe.LUXO_OBP1_D7n_s.qn(), LEDO_PAL_OBP1D,
                                                  pix_pipe.LUGU_OBP1_D5n_s.qn(), LARU_PAL_OBP1C,
                                                  pix_pipe.LOSE_OBP1_D3n_s.qn(), LYKY_PAL_OBP1B,
                                                  pix_pipe.LAWO_OBP1_D1n_s.qn(), LOPU_PAL_OBP1A);

      /*#p35.MUFA*/ wire MUFA_COL_OBP1_LO = amux4(LEDO_PAL_OBP1D, pix_pipe.LEPU_OBP1_D6n_s.qn(),
                                                  LARU_PAL_OBP1C, pix_pipe.LUNE_OBP1_D4n_s.qn(),
                                                  LYKY_PAL_OBP1B, pix_pipe.MOSA_OBP1_D2n_s.qn(),
                                                  LOPU_PAL_OBP1A, pix_pipe.MOXY_OBP1_D0n_s.qn());

      //----------
      // Background/window palette lookup

      /* p35.SOBA*/ wire SOBA_PIX_BG_LOn = not1(RAJY_PIX_BG_LOp);
      /* p35.NUPO*/ wire NUPO_PIX_BG_LOp = not1(SOBA_PIX_BG_LOn);
      /* p35.VYCO*/ wire VYCO_PIX_BG_HIn = not1(TADE_PIX_BG_HIp);
      /* p35.NALE*/ wire NALE_PIX_BG_HIp = not1(VYCO_PIX_BG_HIn);

      /* p35.MUVE*/ wire MUVE_MASK_BGP = not1(POKA_BGPIXELn);

      /* p35.POBU*/ wire POBU_PAL_BGPA = and3(VYCO_PIX_BG_HIn, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
      /* p35.NUXO*/ wire NUXO_PAL_BGPB = and3(VYCO_PIX_BG_HIn, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm
      /* p35.NUMA*/ wire NUMA_PAL_BGPC = and3(NALE_PIX_BG_HIp, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
      /* p35.NYPO*/ wire NYPO_PAL_BGPD = and3(NALE_PIX_BG_HIp, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm

      /*#p35.NELO*/ wire NELO_COL_BG_LO = amux4(NYPO_PAL_BGPD, pix_pipe.MOGY_BGP_D6n_s.qn(),
                                                NUMA_PAL_BGPC, pix_pipe.MUKE_BGP_D4n_s.qn(),
                                                NUXO_PAL_BGPB, pix_pipe.PYLU_BGP_D2n_s.qn(),
                                                POBU_PAL_BGPA, pix_pipe.PAVO_BGP_D0n_s.qn());

      /*#p35.NURA*/ wire NURA_COL_BG_HI = amux4(pix_pipe.MENA_BGP_D7n_s.qn(), NYPO_PAL_BGPD,
                                                pix_pipe.MORU_BGP_D5n_s.qn(), NUMA_PAL_BGPC,
                                                pix_pipe.MAXY_BGP_D3n_s.qn(), NUXO_PAL_BGPB,
                                                pix_pipe.NUSY_BGP_D1n_s.qn(), POBU_PAL_BGPA);

      //----------
      // Pixel merge and send

      /*#p35.PERO*/ wire _PERO_COL_LO = or3(NELO_COL_BG_LO, WALY_COL_OBP0_LO, MUFA_COL_OBP1_LO);
      /*#p35.PATY*/ wire _PATY_COL_HI = or3(NURA_COL_BG_HI, WUFU_COL_OBP0_HI, MOKA_COL_OBP1_HI);

      /*#p35.REMY*/ wire _REMY_LD0n = not1(_PERO_COL_LO);
      /*#p35.RAVO*/ wire _RAVO_LD1n = not1(_PATY_COL_HI);

      //lcd_data0_delay.set(_REMY_LD0n);
      //lcd_data1_delay.set(_RAVO_LD1n);

      // so q1 works but q2 has tiny errors? wat?
      PIN_LCD_DATA0.pin_out(1, /*lcd_data0_delay.q1()*/ _REMY_LD0n, /*lcd_data0_delay.q1()*/ _REMY_LD0n);
      PIN_LCD_DATA1.pin_out(1, /*lcd_data1_delay.q1()*/ _RAVO_LD1n, /*lcd_data1_delay.q1()*/ _RAVO_LD1n);
    }

    // LCD horizontal sync pin latch

    // FIXME inversion
    // I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
    // seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
    // Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
    // That would be a loooot of gate delay.
    // Could we possibly be incrementing X3p one phase early?


    /*PIN_54*/ Pin2 PIN_LCD_HSYNC;
    {
      /* p24.PAHO*/ pix_pipe.PAHO_X_8_SYNC.dff17(_ROXO_CLKPIPE_xBxDxFxH, pix_pipe.XYMU_RENDERINGn.qn(), pix_pipe.XYDO_PX3p.qp());

      /*#p24.POME*/ pix_pipe.POME.set(nor2(_AVAP_SCAN_DONE_TRIGp_new, pix_pipe.POFY.qp()));
      /*#p24.RUJU*/ pix_pipe.RUJU.set(or3(pix_pipe.PAHO_X_8_SYNC.qp(), _TOFU_VID_RSTp_new, pix_pipe.POME.qp()));
      /*#p24.POFY*/ pix_pipe.POFY.set(not1(pix_pipe.RUJU.qp()));
      /*#p24.POME*/ pix_pipe.POME.set(nor2(_AVAP_SCAN_DONE_TRIGp_new, pix_pipe.POFY.qp()));

      /*#p24.RUZE*/ wire _RUZE_HSYNCn = not1(pix_pipe.POFY.qp());
      PIN_LCD_HSYNC.pin_out(1, _RUZE_HSYNCn, _RUZE_HSYNCn);
    }

    /*PIN_56*/ Pin2 PIN_LCD_FLIPS;
    {
      /*#p24.LOFU*/ wire _LOFU_LINE_ENDn  = not1(lcd_reg.RUTU_x113p.qp());
      /*#p24.LUCA*/ lcd_reg.LUCA_LINE_EVENp .dff17(_LOFU_LINE_ENDn,              _LYFE_VID_RSTn_new, lcd_reg.LUCA_LINE_EVENp.qn());
      /*#p21.NAPO*/ lcd_reg.NAPO_FRAME_EVENp.dff17(lcd_reg.POPU_VBLANK_CLKp.qp(), _LYFE_VID_RSTn_new, lcd_reg.NAPO_FRAME_EVENp.qn());
      // if LCDC_ENn, LCD_PIN_ALTSG = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)
      /*#p24.MAGU*/ wire _MAGU = xor2(lcd_reg.NAPO_FRAME_EVENp.qp(), lcd_reg.LUCA_LINE_EVENp.qn());
      /*#p24.MECO*/ wire _MECO = not1(_MAGU);
      /*#p24.KEBO*/ wire _KEBO = not1(_MECO);
      /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(pix_pipe.XONA_LCDC_LCDENn.qn());
      /* p01.UREK*/ wire _UREK_DIV07n = not1(tim_reg.TULU_DIV07p.qp());
      /*#p24.USEC*/ wire _USEC_DIV_07p = not1(_UREK_DIV07n);
      /*#p24.KUPA*/ wire _KUPA = amux2(pix_pipe.XONA_LCDC_LCDENn.qn(), _KEBO, _KEDY_LCDC_ENn, _USEC_DIV_07p);
      /*#p24.KOFO*/ wire _KOFO = not1(_KUPA);

      PIN_LCD_FLIPS.pin_out(1, _KOFO, _KOFO);
    }

    /*PIN_52*/ Pin2 PIN_LCD_CNTRL;
    {
      /*#p21.SAXO*/ wire _SAXO_LX0p = lcd_reg.SAXO_LX0p.qp_new();
      /*#p21.TYPO*/ wire _TYPO_LX1p = lcd_reg.TYPO_LX1p.qp_new();
      /*#p21.VYZO*/ wire _VYZO_LX2p = lcd_reg.VYZO_LX2p.qp_new();
      /*#p21.TELU*/ wire _TELU_LX3p = lcd_reg.TELU_LX3p.qp_new();
      /*#p21.SUDE*/ wire _SUDE_LX4p = lcd_reg.SUDE_LX4p.qp_new();
      /*#p21.TAHA*/ wire _TAHA_LX5p = lcd_reg.TAHA_LX5p.qp_new();
      /*#p21.TYRY*/ wire _TYRY_LX6p = lcd_reg.TYRY_LX6p.qp_new();

      /*#p21.TOCU*/ wire _TOCU_LX0n = not1(_SAXO_LX0p);
      /*#p21.VEPE*/ wire _VEPE_LX1n = not1(_TYPO_LX1p);
      /* p21.VUTY*/ wire _VUTY_LX2n = not1(_VYZO_LX2p);
      /* p21.VATE*/ wire _VATE_LX3n = not1(_TELU_LX3p);
      /* p21.TUDA*/ wire _TUDA_LX4n = not1(_SUDE_LX4p);
      /* p21.TAFY*/ wire _TAFY_LX5n = not1(_TAHA_LX5p);
      /* p21.TUJU*/ wire _TUJU_LX6n = not1(_TYRY_LX6p);

      /* p21.VOKU*/ wire _VOKU_LX000n = nand7(_TUJU_LX6n, _TAFY_LX5n, _TUDA_LX4n, _VATE_LX3n, _VUTY_LX2n, _VEPE_LX1n, _TOCU_LX0n); // 0000000 == 0
      /* p21.TOZU*/ wire _TOZU_LX007n = nand7(_TUJU_LX6n, _TAFY_LX5n, _TUDA_LX4n, _VATE_LX3n, _VYZO_LX2p, _TYPO_LX1p, _SAXO_LX0p); // 0000111 == 7
      /* p21.TECE*/ wire _TECE_LX045n = nand7(_TUJU_LX6n, _TAHA_LX5p, _TUDA_LX4n, _TELU_LX3p, _VYZO_LX2p, _VEPE_LX1n, _SAXO_LX0p); // 0101101 == 45
      /*#p21.TEBO*/ wire _TEBO_LX083n = nand7(_TYRY_LX6p, _TAFY_LX5n, _SUDE_LX4p, _VATE_LX3n, _VUTY_LX2n, _TYPO_LX1p, _SAXO_LX0p); // 1010011 == 83
      /*#p21.TEGY*/ wire _TEGY_STROBE = nand4(_VOKU_LX000n, _TOZU_LX007n, _TECE_LX045n, _TEBO_LX083n);
      /*#p21.SYGU*/ lcd_reg.SYGU_LINE_STROBE.dff17(_SONO_ABxxxxGH_new,  _LYFE_VID_RSTn_new, _TEGY_STROBE);

      /*#p21.RYNO*/ wire _RYNO = or2(lcd_reg.SYGU_LINE_STROBE.qp_new(), lcd_reg.RUTU_x113p.qp_new());
      /*#p21.POGU*/ wire _POGU = not1(_RYNO);
      PIN_LCD_CNTRL.pin_out(1, _POGU, _POGU);
    }

    /*PIN_55*/ Pin2 PIN_LCD_LATCH;
    {
      /*#p21.PURE*/ wire _PURE_LINE_ENDn = not1(lcd_reg.RUTU_x113p.qp());
      /*#p24.KASA*/ wire _KASA_LINE_ENDp = not1(_PURE_LINE_ENDn);
      /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(pix_pipe.XONA_LCDC_LCDENn.qn());
      /* p01.UMEK*/ wire _UMEK_DIV06n = not1(tim_reg.UGOT_DIV06p.qp());
      /*#p24.UMOB*/ wire UMOB_DIV_06p = not1(_UMEK_DIV06n);
      /*#p24.KAHE*/ wire _KAHE_LINE_ENDp = amux2(pix_pipe.XONA_LCDC_LCDENn.qn(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, UMOB_DIV_06p);
      /*#p24.KYMO*/ wire _KYMO_LINE_ENDn = not1(_KAHE_LINE_ENDp);
      PIN_LCD_LATCH.pin_out(1, _KYMO_LINE_ENDn, _KYMO_LINE_ENDn);
    }

    /*PIN_53*/ Pin2 PIN_LCD_CLOCK;
    {
      /*#p21.XAJO*/ wire _XAJO_X_009p = and2(pix_pipe.XEHO_PX0p.qp(), pix_pipe.XYDO_PX3p.qp());
      /*#p21.WUSA*/ pix_pipe.WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p, _WEGO_HBLANKp_new);
      /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK = and2(pix_pipe.WUSA_LCD_CLOCK_GATE.qp(), _SACU_CLKPIPE_AxCxExGx);
      /*#p27.POVA*/ wire _POVA_FINE_MATCHpe = and2(pix_pipe.PUXA_SCX_FINE_MATCH_A.qp(), pix_pipe.NYZE_SCX_FINE_MATCH_B.qn());
      /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, _POVA_FINE_MATCHpe);
      /*#p21.RYPO*/ wire _RYPO_LCD_CLOCK = not1(_SEMU_LCD_CLOCK);
      PIN_LCD_CLOCK.pin_out(1, _RYPO_LCD_CLOCK, _RYPO_LCD_CLOCK);
    }

    /*PIN_57*/ Pin2 PIN_LCD_VSYNC;
    {
      /*#p24.NERU*/ wire _NERU_y000p = nor8(lcd_reg.LAFO_LY7p.qp(), lcd_reg.LOVU_LY4p.qp(), lcd_reg.LYDO_LY3p.qp(), lcd_reg.MUWY_LY0p.qp(),
                                               lcd_reg.MYRO_LY1p.qp(), lcd_reg.LEXA_LY2p.qp(), lcd_reg.LEMA_LY5p.qp(), lcd_reg.MATO_LY6p.qp());
      /*#p24.MEDA*/ lcd_reg.MEDA_VSYNC_OUTn.dff17(lcd_reg.NYPE_x113p.qn(), _LYFE_VID_RSTn_new, _NERU_y000p);
      /*#p24.MURE*/ wire _MURE_VSYNC = not1(lcd_reg.MEDA_VSYNC_OUTn.qp());
      /*#*/ PIN_LCD_VSYNC.pin_out(1, _MURE_VSYNC, _MURE_VSYNC);
    }

    if (!old_lcd_clock && PIN_LCD_CLOCK.qp()) {
      gb_screen_x++;
    }
    if (PIN_LCD_HSYNC.qp() || PIN_LCD_LATCH.qp()) {
      gb_screen_x = 0;
    }

    lcd_pix_lo.nor_latch(PIN_LCD_DATA0.qp(), PIN_LCD_CLOCK.qp() | PIN_LCD_HSYNC.qp());
    lcd_pix_hi.nor_latch(PIN_LCD_DATA1.qp(), PIN_LCD_CLOCK.qp() | PIN_LCD_HSYNC.qp());

    if (!old_lcd_latch && PIN_LCD_LATCH.qp()) {
      if (gb_screen_y < 144) {
        for (int x = 0; x < 159; x++) {
          uint8_t p0 = lcd_pipe_lo[x + 1].qp();
          uint8_t p1 = lcd_pipe_hi[x + 1].qp();
          framebuffer[x + gb_screen_y * 160] = p0 + p1 * 2;
        }
        {
          uint8_t p0 = lcd_pix_lo.qp();
          uint8_t p1 = lcd_pix_hi.qp();
          framebuffer[159 + gb_screen_y * 160] = p0 + p1 * 2;
        }
      }

      gb_screen_y++;
      if (PIN_LCD_VSYNC.qp()) {
        gb_screen_y = 0;
      }
    }

    {
      for (int i = 0; i < 159; i++) {
        lcd_pipe_lo[i].dff(!PIN_LCD_CLOCK.qp(), 1, 1, lcd_pipe_lo[i + 1].qp());
        lcd_pipe_hi[i].dff(!PIN_LCD_CLOCK.qp(), 1, 1, lcd_pipe_hi[i + 1].qp());
      }

      lcd_pipe_lo[159].dff(!PIN_LCD_CLOCK.qp(), 1, 1, lcd_pix_lo.qp());
      lcd_pipe_hi[159].dff(!PIN_LCD_CLOCK.qp(), 1, 1, lcd_pix_hi.qp());

    }

    old_lcd_clock = PIN_LCD_CLOCK.qp();
    old_lcd_latch = PIN_LCD_LATCH.qp();
  }

  //------------------------------------------------------------------------------
  // Latch stuff for CPU

#pragma region BUS_CPU_Dp_out_Drivers
  {
    /* FF00 JOYP */
    /* p05.KEMA*/ BUS_CPU_Dp_out[0].tri6_nn(_BYZO_FF00_RDn_new, joypad.KEVU_JOYP_L0n.qp());
    /* p05.KURO*/ BUS_CPU_Dp_out[1].tri6_nn(_BYZO_FF00_RDn_new, joypad.KAPA_JOYP_L1n.qp());
    /* p05.KUVE*/ BUS_CPU_Dp_out[2].tri6_nn(_BYZO_FF00_RDn_new, joypad.KEJA_JOYP_L2n.qp());
    /* p05.JEKU*/ BUS_CPU_Dp_out[3].tri6_nn(_BYZO_FF00_RDn_new, joypad.KOLO_JOYP_L3n.qp());
    /* p05.KOCE*/ BUS_CPU_Dp_out[4].tri6_nn(_BYZO_FF00_RDn_new, joypad.KELY_JOYP_UDLRp.qn());
    /* p05.CUDY*/ BUS_CPU_Dp_out[5].tri6_nn(_BYZO_FF00_RDn_new, joypad.COFY_JOYP_ABCSp.qn());
    /* p??.????*/ BUS_CPU_Dp_out[6].tri6_nn(_BYZO_FF00_RDn_new, joypad.KUKO_DBG_FF00_D6n.qp());
    /* p??.????*/ BUS_CPU_Dp_out[7].tri6_nn(_BYZO_FF00_RDn_new, joypad.KERU_DBG_FF00_D7n.qp());
  }
  {
    /* FF01 SER */
    /*#p06.CUGY*/ BUS_CPU_Dp_out[0].tri6_pn(_UFEG_FF01_RDp_new, ser_reg.CUBA_SER_DATA0.qn());
    /* p06.DUDE*/ BUS_CPU_Dp_out[1].tri6_pn(_UFEG_FF01_RDp_new, ser_reg.DEGU_SER_DATA1.qn());
    /* p06.DETU*/ BUS_CPU_Dp_out[2].tri6_pn(_UFEG_FF01_RDp_new, ser_reg.DYRA_SER_DATA2.qn());
    /* p06.DASO*/ BUS_CPU_Dp_out[3].tri6_pn(_UFEG_FF01_RDp_new, ser_reg.DOJO_SER_DATA3.qn());
    /* p06.DAME*/ BUS_CPU_Dp_out[4].tri6_pn(_UFEG_FF01_RDp_new, ser_reg.DOVU_SER_DATA4.qn());
    /* p06.EVOK*/ BUS_CPU_Dp_out[5].tri6_pn(_UFEG_FF01_RDp_new, ser_reg.EJAB_SER_DATA5.qn());
    /* p06.EFAB*/ BUS_CPU_Dp_out[6].tri6_pn(_UFEG_FF01_RDp_new, ser_reg.EROD_SER_DATA6.qn());
    /*#p06.ETAK*/ BUS_CPU_Dp_out[7].tri6_pn(_UFEG_FF01_RDp_new, ser_reg.EDER_SER_DATA7.qn());

    /* FF02 SER */
    /* p06.CORE*/ BUS_CPU_Dp_out[0].tri6_pn(_UCOM_FF02_RDp_new, ser_reg.CULY_XFER_DIR.qn());
    /* p06.ELUV*/ BUS_CPU_Dp_out[7].tri6_pn(_UCOM_FF02_RDp_new, ser_reg.ETAF_SER_RUNNING.qn());
  }

  {
    /* FF04 DIV */
    /* p01.UMEK*/ wire _UMEK_DIV06n = not1(tim_reg.UGOT_DIV06p.qp());
    /* p01.UREK*/ wire _UREK_DIV07n = not1(tim_reg.TULU_DIV07p.qp());
    /* p01.UTOK*/ wire _UTOK_DIV08n = not1(tim_reg.TUGO_DIV08p.qp());
    /* p01.SAPY*/ wire _SAPY_DIV09n = not1(tim_reg.TOFE_DIV09p.qp());
    /* p01.UMER*/ wire _UMER_DIV10n = not1(tim_reg.TERU_DIV10p.qp());
    /* p01.RAVE*/ wire _RAVE_DIV11n = not1(tim_reg.SOLA_DIV11p.qp());
    /* p01.RYSO*/ wire _RYSO_DIV12n = not1(tim_reg.SUBU_DIV12p.qp());
    /* p01.UDOR*/ wire _UDOR_DIV13n = not1(tim_reg.TEKA_DIV13p.qp());

    /* p01.TAWU*/ BUS_CPU_Dp_out[0].tri6_pn(_TAGY_FF04_RDp_new, _UMEK_DIV06n);
    /* p01.TAKU*/ BUS_CPU_Dp_out[1].tri6_pn(_TAGY_FF04_RDp_new, _UREK_DIV07n);
    /* p01.TEMU*/ BUS_CPU_Dp_out[2].tri6_pn(_TAGY_FF04_RDp_new, _UTOK_DIV08n);
    /* p01.TUSE*/ BUS_CPU_Dp_out[3].tri6_pn(_TAGY_FF04_RDp_new, _SAPY_DIV09n);
    /* p01.UPUG*/ BUS_CPU_Dp_out[4].tri6_pn(_TAGY_FF04_RDp_new, _UMER_DIV10n); // Schematic wrong, UPUG/SEPU driving D5/D4
    /* p01.SEPU*/ BUS_CPU_Dp_out[5].tri6_pn(_TAGY_FF04_RDp_new, _RAVE_DIV11n);
    /* p01.SAWA*/ BUS_CPU_Dp_out[6].tri6_pn(_TAGY_FF04_RDp_new, _RYSO_DIV12n);
    /* p01.TATU*/ BUS_CPU_Dp_out[7].tri6_pn(_TAGY_FF04_RDp_new, _UDOR_DIV13n);
  }

  {
    /* FF05 TIMA */
    /*#p03.SOKU*/ BUS_CPU_Dp_out[0].tri6_pn(_TEDA_FF05_RDp_new, tim_reg.REGA_TIMA0p.qn());
    /*#p03.RACY*/ BUS_CPU_Dp_out[1].tri6_pn(_TEDA_FF05_RDp_new, tim_reg.POVY_TIMA1p.qn());
    /*#p03.RAVY*/ BUS_CPU_Dp_out[2].tri6_pn(_TEDA_FF05_RDp_new, tim_reg.PERU_TIMA2p.qn());
    /*#p03.SOSY*/ BUS_CPU_Dp_out[3].tri6_pn(_TEDA_FF05_RDp_new, tim_reg.RATE_TIMA3p.qn());
    /*#p03.SOMU*/ BUS_CPU_Dp_out[4].tri6_pn(_TEDA_FF05_RDp_new, tim_reg.RUBY_TIMA4p.qn());
    /*#p03.SURO*/ BUS_CPU_Dp_out[5].tri6_pn(_TEDA_FF05_RDp_new, tim_reg.RAGE_TIMA5p.qn());
    /*#p03.ROWU*/ BUS_CPU_Dp_out[6].tri6_pn(_TEDA_FF05_RDp_new, tim_reg.PEDA_TIMA6p.qn());
    /*#p03.PUSO*/ BUS_CPU_Dp_out[7].tri6_pn(_TEDA_FF05_RDp_new, tim_reg.NUGA_TIMA7p.qn());
  }

  {
    /* FF06 TMA */
    /*#p03.SETE*/ BUS_CPU_Dp_out[0].tri6_pn(_TUBY_FF06_RDp_new, tim_reg.SABU_TMA0p.qn());
    /*#p03.PYRE*/ BUS_CPU_Dp_out[1].tri6_pn(_TUBY_FF06_RDp_new, tim_reg.NYKE_TMA1p.qn());
    /*#p03.NOLA*/ BUS_CPU_Dp_out[2].tri6_pn(_TUBY_FF06_RDp_new, tim_reg.MURU_TMA2p.qn());
    /*#p03.SALU*/ BUS_CPU_Dp_out[3].tri6_pn(_TUBY_FF06_RDp_new, tim_reg.TYVA_TMA3p.qn());
    /*#p03.SUPO*/ BUS_CPU_Dp_out[4].tri6_pn(_TUBY_FF06_RDp_new, tim_reg.TYRU_TMA4p.qn());
    /*#p03.SOTU*/ BUS_CPU_Dp_out[5].tri6_pn(_TUBY_FF06_RDp_new, tim_reg.SUFY_TMA5p.qn());
    /*#p03.REVA*/ BUS_CPU_Dp_out[6].tri6_pn(_TUBY_FF06_RDp_new, tim_reg.PETO_TMA6p.qn());
    /*#p03.SAPU*/ BUS_CPU_Dp_out[7].tri6_pn(_TUBY_FF06_RDp_new, tim_reg.SETA_TMA7p.qn());
  }

  {
    /* FF07 TAC */
    /*#p03.RYLA*/ BUS_CPU_Dp_out[0].tri6_pn(_SORA_FF07_RDp_new, tim_reg.SOPU_TAC0p.qn());
    /*#p03.ROTE*/ BUS_CPU_Dp_out[1].tri6_pn(_SORA_FF07_RDp_new, tim_reg.SAMY_TAC1p.qn());
    /*#p03.SUPE*/ BUS_CPU_Dp_out[2].tri6_pn(_SORA_FF07_RDp_new, tim_reg.SABO_TAC2p.qn());
  }

  {
    /* FF0F INTF */
    // FIXME this inversion fixes a bunch of tests...
    // MATY is one of those big yellow latchy things
    /* p02.MATY*/ int_reg.MATY_FF0F_L0p.tp_latch(!_ROLO_FF0F_RDn_new, int_reg.LOPE_FF0F_D0p.qp()); // OUTPUT ON RUNG 10
    /* p02.MOPO*/ int_reg.MOPO_FF0F_L1p.tp_latch(!_ROLO_FF0F_RDn_new, int_reg.LALU_FF0F_D1p.qp()); // OUTPUT ON RUNG 10
    /* p02.PAVY*/ int_reg.PAVY_FF0F_L2p.tp_latch(!_ROLO_FF0F_RDn_new, int_reg.NYBO_FF0F_D2p.qp()); // OUTPUT ON RUNG 10
    /* p02.NEJY*/ int_reg.NEJY_FF0F_L3p.tp_latch(!_ROLO_FF0F_RDn_new, int_reg.UBUL_FF0F_D3p.qp()); // OUTPUT ON RUNG 10
    /* p02.NUTY*/ int_reg.NUTY_FF0F_L4p.tp_latch(!_ROLO_FF0F_RDn_new, int_reg.ULAK_FF0F_D4p.qp()); // OUTPUT ON RUNG 10

    /*#p02.NELA*/ BUS_CPU_Dp_out[0].tri6_pn(_POLA_FF0F_RDp_new, int_reg.MATY_FF0F_L0p.qn());
    /*#p02.NABO*/ BUS_CPU_Dp_out[1].tri6_pn(_POLA_FF0F_RDp_new, int_reg.MOPO_FF0F_L1p.qn());
    /*#p02.ROVA*/ BUS_CPU_Dp_out[2].tri6_pn(_POLA_FF0F_RDp_new, int_reg.PAVY_FF0F_L2p.qn());
    /*#p02.PADO*/ BUS_CPU_Dp_out[3].tri6_pn(_POLA_FF0F_RDp_new, int_reg.NEJY_FF0F_L3p.qn());
    /*#p02.PEGY*/ BUS_CPU_Dp_out[4].tri6_pn(_POLA_FF0F_RDp_new, int_reg.NUTY_FF0F_L4p.qn());
  }

  {
    /* FF40 LCDC */
    /*#p23.WYPO*/ BUS_CPU_Dp_out[0].tri6_nn(_WYCE_FF40_RDn_new, pix_pipe.VYXE_LCDC_BGENn  .qp());
    /*#p23.XERO*/ BUS_CPU_Dp_out[1].tri6_nn(_WYCE_FF40_RDn_new, pix_pipe.XYLO_LCDC_SPENn  .qp());
    /* p23.WYJU*/ BUS_CPU_Dp_out[2].tri6_nn(_WYCE_FF40_RDn_new, pix_pipe.XYMO_LCDC_SPSIZEn.qp());
    /* p23.WUKA*/ BUS_CPU_Dp_out[3].tri6_nn(_WYCE_FF40_RDn_new, pix_pipe.XAFO_LCDC_BGMAPn .qp());
    /* p23.VOKE*/ BUS_CPU_Dp_out[4].tri6_nn(_WYCE_FF40_RDn_new, pix_pipe.WEXU_LCDC_BGTILEn.qp());
    /* p23.VATO*/ BUS_CPU_Dp_out[5].tri6_nn(_WYCE_FF40_RDn_new, pix_pipe.WYMO_LCDC_WINENn .qp());
    /*#p23.VAHA*/ BUS_CPU_Dp_out[6].tri6_nn(_WYCE_FF40_RDn_new, pix_pipe.WOKY_LCDC_WINMAPn.qp());
    /*#p23.XEBU*/ BUS_CPU_Dp_out[7].tri6_nn(_WYCE_FF40_RDn_new, pix_pipe.XONA_LCDC_LCDENn .qp());
  }

  {
    /* FF41 STAT */
    // This latch isn't actually used as a latch, it's effectively an inverter.
    /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = or2(_WESY_SYS_RSTn_new, _RYJU_FF41_WRn_new);
    /* p21.RUPO*/ pix_pipe.RUPO_LYC_MATCH_LATCHn.nor_latch(_PAGO_LYC_MATCH_RST, lcd_reg.ROPO_LY_MATCH_SYNCp.qp());

    /*#p21.PARU*/ wire _PARU_VBLANKp = not1(lcd_reg.POPU_VBLANK_CLKp.qn());
    /* p21.SADU*/ wire _SADU_STAT_MODE0n = nor2(pix_pipe.XYMU_RENDERINGn.qn(), _PARU_VBLANKp); // die NOR
    /* p21.XATY*/ wire _XATY_STAT_MODE1n = nor2(_ACYL_SCANNINGp, pix_pipe.XYMU_RENDERINGn.qn()); // die NOR
    /*#p21.TEBY*/ BUS_CPU_Dp_out[0].tri6_pn(_TOBE_FF41_RDp_new, _SADU_STAT_MODE0n);
    /*#p21.WUGA*/ BUS_CPU_Dp_out[1].tri6_pn(_TOBE_FF41_RDp_new, _XATY_STAT_MODE1n);
    /*#p21.SEGO*/ BUS_CPU_Dp_out[2].tri6_pn(_TOBE_FF41_RDp_new, pix_pipe.RUPO_LYC_MATCH_LATCHn.qp());
    /* p21.PUZO*/ BUS_CPU_Dp_out[3].tri6_nn(_VAVE_FF41_RDn_new, pix_pipe.ROXE_STAT_HBI_ENn_s.qp());
    /* p21.POFO*/ BUS_CPU_Dp_out[4].tri6_nn(_VAVE_FF41_RDn_new, pix_pipe.RUFO_STAT_VBI_ENn_s.qp());
    /* p21.SASY*/ BUS_CPU_Dp_out[5].tri6_nn(_VAVE_FF41_RDn_new, pix_pipe.REFE_STAT_OAI_ENn_s.qp());
    /* p21.POTE*/ BUS_CPU_Dp_out[6].tri6_nn(_VAVE_FF41_RDn_new, pix_pipe.RUGU_STAT_LYI_ENn_s.qp());
  }

  {
    /* FF42 SCY */
    /*#p23.WARE*/ BUS_CPU_Dp_out[0].tri6_nn(_BUWY_FF42_RDn_new, pix_pipe.GAVE_SCY0n_s.qp());
    /* p23.GOBA*/ BUS_CPU_Dp_out[1].tri6_nn(_BUWY_FF42_RDn_new, pix_pipe.FYMO_SCY1n_s.qp());
    /* p23.GONU*/ BUS_CPU_Dp_out[2].tri6_nn(_BUWY_FF42_RDn_new, pix_pipe.FEZU_SCY2n_s.qp());
    /* p23.GODO*/ BUS_CPU_Dp_out[3].tri6_nn(_BUWY_FF42_RDn_new, pix_pipe.FUJO_SCY3n_s.qp());
    /* p23.CUSA*/ BUS_CPU_Dp_out[4].tri6_nn(_BUWY_FF42_RDn_new, pix_pipe.DEDE_SCY4n_s.qp());
    /* p23.GYZO*/ BUS_CPU_Dp_out[5].tri6_nn(_BUWY_FF42_RDn_new, pix_pipe.FOTY_SCY5n_s.qp());
    /* p23.GUNE*/ BUS_CPU_Dp_out[6].tri6_nn(_BUWY_FF42_RDn_new, pix_pipe.FOHA_SCY6n_s.qp());
    /* p23.GYZA*/ BUS_CPU_Dp_out[7].tri6_nn(_BUWY_FF42_RDn_new, pix_pipe.FUNY_SCY7n_s.qp());
  }

  {
    /* FF43 SCX */
    /*#p23.EDOS*/ BUS_CPU_Dp_out[0].tri6_nn(_BEBA_FF43_RDn_new, pix_pipe.DATY_SCX0n_s.qp());
    /* p23.EKOB*/ BUS_CPU_Dp_out[1].tri6_nn(_BEBA_FF43_RDn_new, pix_pipe.DUZU_SCX1n_s.qp());
    /* p23.CUGA*/ BUS_CPU_Dp_out[2].tri6_nn(_BEBA_FF43_RDn_new, pix_pipe.CYXU_SCX2n_s.qp());
    /* p23.WONY*/ BUS_CPU_Dp_out[3].tri6_nn(_BEBA_FF43_RDn_new, pix_pipe.GUBO_SCX3n_s.qp());
    /* p23.CEDU*/ BUS_CPU_Dp_out[4].tri6_nn(_BEBA_FF43_RDn_new, pix_pipe.BEMY_SCX4n_s.qp());
    /* p23.CATA*/ BUS_CPU_Dp_out[5].tri6_nn(_BEBA_FF43_RDn_new, pix_pipe.CUZY_SCX5n_s.qp());
    /* p23.DOXE*/ BUS_CPU_Dp_out[6].tri6_nn(_BEBA_FF43_RDn_new, pix_pipe.CABU_SCX6n_s.qp());
    /* p23.CASY*/ BUS_CPU_Dp_out[7].tri6_nn(_BEBA_FF43_RDn_new, pix_pipe.BAKE_SCX7n_s.qp());
  }

  {
    /* FF44 LY */
    /*#p23.WURY*/ wire _WURY_LY0n = not1(lcd_reg.MUWY_LY0p.qp());
    /* p23.XEPO*/ wire _XEPO_LY1n = not1(lcd_reg.MYRO_LY1p.qp());
    /* p23.MYFA*/ wire _MYFA_LY2n = not1(lcd_reg.LEXA_LY2p.qp());
    /* p23.XUHY*/ wire _XUHY_LY3n = not1(lcd_reg.LYDO_LY3p.qp());
    /* p23.WATA*/ wire _WATA_LY4n = not1(lcd_reg.LOVU_LY4p.qp());
    /* p23.XAGA*/ wire _XAGA_LY5n = not1(lcd_reg.LEMA_LY5p.qp());
    /* p23.XUCE*/ wire _XUCE_LY6n = not1(lcd_reg.MATO_LY6p.qp());
    /* p23.XOWO*/ wire _XOWO_LY7n = not1(lcd_reg.LAFO_LY7p.qp());

    /* p23.VEGA*/ BUS_CPU_Dp_out[0].tri6_nn(_VARO_FF44_RDn_new, _WURY_LY0n);
    /* p23.WUVA*/ BUS_CPU_Dp_out[1].tri6_nn(_VARO_FF44_RDn_new, _XEPO_LY1n);
    /* p23.LYCO*/ BUS_CPU_Dp_out[2].tri6_nn(_VARO_FF44_RDn_new, _MYFA_LY2n);
    /* p23.WOJY*/ BUS_CPU_Dp_out[3].tri6_nn(_VARO_FF44_RDn_new, _XUHY_LY3n);
    /* p23.VYNE*/ BUS_CPU_Dp_out[4].tri6_nn(_VARO_FF44_RDn_new, _WATA_LY4n);
    /* p23.WAMA*/ BUS_CPU_Dp_out[5].tri6_nn(_VARO_FF44_RDn_new, _XAGA_LY5n);
    /* p23.WAVO*/ BUS_CPU_Dp_out[6].tri6_nn(_VARO_FF44_RDn_new, _XUCE_LY6n);
    /* p23.WEZE*/ BUS_CPU_Dp_out[7].tri6_nn(_VARO_FF44_RDn_new, _XOWO_LY7n);
  }

  {
    /* FF45 LYC */
    /*#p23.RETU*/ BUS_CPU_Dp_out[0].tri6_nn(_WEKU_FF45_RDn_new, lcd_reg.SYRY_LYC0n_s.qp());
    /* p23.VOJO*/ BUS_CPU_Dp_out[1].tri6_nn(_WEKU_FF45_RDn_new, lcd_reg.VUCE_LYC1n_s.qp());
    /* p23.RAZU*/ BUS_CPU_Dp_out[2].tri6_nn(_WEKU_FF45_RDn_new, lcd_reg.SEDY_LYC2n_s.qp());
    /* p23.REDY*/ BUS_CPU_Dp_out[3].tri6_nn(_WEKU_FF45_RDn_new, lcd_reg.SALO_LYC3n_s.qp());
    /* p23.RACE*/ BUS_CPU_Dp_out[4].tri6_nn(_WEKU_FF45_RDn_new, lcd_reg.SOTA_LYC4n_s.qp());
    /*#p23.VAZU*/ BUS_CPU_Dp_out[5].tri6_nn(_WEKU_FF45_RDn_new, lcd_reg.VAFA_LYC5n_s.qp());
    /* p23.VAFE*/ BUS_CPU_Dp_out[6].tri6_nn(_WEKU_FF45_RDn_new, lcd_reg.VEVO_LYC6n_s.qp());
    /* p23.PUFY*/ BUS_CPU_Dp_out[7].tri6_nn(_WEKU_FF45_RDn_new, lcd_reg.RAHA_LYC7n_s.qp());
  }

  {
    /* FF46 DMA */
    /*#p04.POLY*/ BUS_CPU_Dp_out[0].tri6_pn(_PUSY_FF46_RDp_new, dma_reg.NAFA_DMA_A08n.qp());
    /* p04.ROFO*/ BUS_CPU_Dp_out[1].tri6_pn(_PUSY_FF46_RDp_new, dma_reg.PYNE_DMA_A09n.qp());
    /* p04.REMA*/ BUS_CPU_Dp_out[2].tri6_pn(_PUSY_FF46_RDp_new, dma_reg.PARA_DMA_A10n.qp());
    /* p04.PANE*/ BUS_CPU_Dp_out[3].tri6_pn(_PUSY_FF46_RDp_new, dma_reg.NYDO_DMA_A11n.qp());
    /* p04.PARE*/ BUS_CPU_Dp_out[4].tri6_pn(_PUSY_FF46_RDp_new, dma_reg.NYGY_DMA_A12n.qp());
    /* p04.RALY*/ BUS_CPU_Dp_out[5].tri6_pn(_PUSY_FF46_RDp_new, dma_reg.PULA_DMA_A13n.qp());
    /* p04.RESU*/ BUS_CPU_Dp_out[6].tri6_pn(_PUSY_FF46_RDp_new, dma_reg.POKU_DMA_A14n.qp());
    /* p04.NUVY*/ BUS_CPU_Dp_out[7].tri6_pn(_PUSY_FF46_RDp_new, dma_reg.MARU_DMA_A15n.qp());
  }

  {
    /* FF47 BGP */
    /*#p36.RARO*/ BUS_CPU_Dp_out[0].tri6_nn(_TEPY_FF47_RDn_new, pix_pipe.PAVO_BGP_D0n_s.qp());
    /* p36.PABA*/ BUS_CPU_Dp_out[1].tri6_nn(_TEPY_FF47_RDn_new, pix_pipe.NUSY_BGP_D1n_s.qp());
    /* p36.REDO*/ BUS_CPU_Dp_out[2].tri6_nn(_TEPY_FF47_RDn_new, pix_pipe.PYLU_BGP_D2n_s.qp());
    /* p36.LOBE*/ BUS_CPU_Dp_out[3].tri6_nn(_TEPY_FF47_RDn_new, pix_pipe.MAXY_BGP_D3n_s.qp());
    /* p36.LACE*/ BUS_CPU_Dp_out[4].tri6_nn(_TEPY_FF47_RDn_new, pix_pipe.MUKE_BGP_D4n_s.qp());
    /* p36.LYKA*/ BUS_CPU_Dp_out[5].tri6_nn(_TEPY_FF47_RDn_new, pix_pipe.MORU_BGP_D5n_s.qp());
    /* p36.LODY*/ BUS_CPU_Dp_out[6].tri6_nn(_TEPY_FF47_RDn_new, pix_pipe.MOGY_BGP_D6n_s.qp());
    /* p36.LARY*/ BUS_CPU_Dp_out[7].tri6_nn(_TEPY_FF47_RDn_new, pix_pipe.MENA_BGP_D7n_s.qp());
  }

  {
    /* FF48 OBP0 */
    /*#p36.XARY*/ BUS_CPU_Dp_out[0].tri6_nn(_XOZY_FF48_RDn_new, pix_pipe.XUFU_OBP0_D0n_s.qp());
    /* p36.XOKE*/ BUS_CPU_Dp_out[1].tri6_nn(_XOZY_FF48_RDn_new, pix_pipe.XUKY_OBP0_D1n_s.qp());
    /* p36.XUNO*/ BUS_CPU_Dp_out[2].tri6_nn(_XOZY_FF48_RDn_new, pix_pipe.XOVA_OBP0_D2n_s.qp());
    /* p36.XUBY*/ BUS_CPU_Dp_out[3].tri6_nn(_XOZY_FF48_RDn_new, pix_pipe.XALO_OBP0_D3n_s.qp());
    /* p36.XAJU*/ BUS_CPU_Dp_out[4].tri6_nn(_XOZY_FF48_RDn_new, pix_pipe.XERU_OBP0_D4n_s.qp());
    /* p36.XOBO*/ BUS_CPU_Dp_out[5].tri6_nn(_XOZY_FF48_RDn_new, pix_pipe.XYZE_OBP0_D5n_s.qp());
    /* p36.XAXA*/ BUS_CPU_Dp_out[6].tri6_nn(_XOZY_FF48_RDn_new, pix_pipe.XUPO_OBP0_D6n_s.qp());
    /* p36.XAWO*/ BUS_CPU_Dp_out[7].tri6_nn(_XOZY_FF48_RDn_new, pix_pipe.XANA_OBP0_D7n_s.qp());
  }

  {
    /* FF49 OBP1 */
    /*#p36.LAJU*/ BUS_CPU_Dp_out[0].tri6_nn(_LOTE_FF49_RDn_new, pix_pipe.MOXY_OBP1_D0n_s.qp());
    /* p36.LEPA*/ BUS_CPU_Dp_out[1].tri6_nn(_LOTE_FF49_RDn_new, pix_pipe.LAWO_OBP1_D1n_s.qp());
    /* p36.LODE*/ BUS_CPU_Dp_out[2].tri6_nn(_LOTE_FF49_RDn_new, pix_pipe.MOSA_OBP1_D2n_s.qp());
    /* p36.LYZA*/ BUS_CPU_Dp_out[3].tri6_nn(_LOTE_FF49_RDn_new, pix_pipe.LOSE_OBP1_D3n_s.qp());
    /* p36.LUKY*/ BUS_CPU_Dp_out[4].tri6_nn(_LOTE_FF49_RDn_new, pix_pipe.LUNE_OBP1_D4n_s.qp());
    /* p36.LUGA*/ BUS_CPU_Dp_out[5].tri6_nn(_LOTE_FF49_RDn_new, pix_pipe.LUGU_OBP1_D5n_s.qp());
    /* p36.LEBA*/ BUS_CPU_Dp_out[6].tri6_nn(_LOTE_FF49_RDn_new, pix_pipe.LEPU_OBP1_D6n_s.qp());
    /* p36.LELU*/ BUS_CPU_Dp_out[7].tri6_nn(_LOTE_FF49_RDn_new, pix_pipe.LUXO_OBP1_D7n_s.qp());
  }

  {
    /* FF4A WY */
    /*#p23.PUNU*/ BUS_CPU_Dp_out[0].tri6_nn(_VOMY_FF4A_RDn_new, pix_pipe.NESO_WY0n_s.qp());
    /* p23.PODA*/ BUS_CPU_Dp_out[1].tri6_nn(_VOMY_FF4A_RDn_new, pix_pipe.NYRO_WY1n_s.qp());
    /* p23.PYGU*/ BUS_CPU_Dp_out[2].tri6_nn(_VOMY_FF4A_RDn_new, pix_pipe.NAGA_WY2n_s.qp());
    /* p23.LOKA*/ BUS_CPU_Dp_out[3].tri6_nn(_VOMY_FF4A_RDn_new, pix_pipe.MELA_WY3n_s.qp());
    /* p23.MEGA*/ BUS_CPU_Dp_out[4].tri6_nn(_VOMY_FF4A_RDn_new, pix_pipe.NULO_WY4n_s.qp());
    /* p23.PELA*/ BUS_CPU_Dp_out[5].tri6_nn(_VOMY_FF4A_RDn_new, pix_pipe.NENE_WY5n_s.qp());
    /* p23.POLO*/ BUS_CPU_Dp_out[6].tri6_nn(_VOMY_FF4A_RDn_new, pix_pipe.NUKA_WY6n_s.qp());
    /* p23.MERA*/ BUS_CPU_Dp_out[7].tri6_nn(_VOMY_FF4A_RDn_new, pix_pipe.NAFU_WY7n_s.qp());
  }

  {
    /* FF4B WX */
    /*#p23.LOVA*/ BUS_CPU_Dp_out[0].tri6_nn(_VYCU_FF4B_RDn_new, pix_pipe.MYPA_WX0n_s.qp());
    /* p23.MUKA*/ BUS_CPU_Dp_out[1].tri6_nn(_VYCU_FF4B_RDn_new, pix_pipe.NOFE_WX1n_s.qp());
    /* p23.MOKO*/ BUS_CPU_Dp_out[2].tri6_nn(_VYCU_FF4B_RDn_new, pix_pipe.NOKE_WX2n_s.qp());
    /* p23.LOLE*/ BUS_CPU_Dp_out[3].tri6_nn(_VYCU_FF4B_RDn_new, pix_pipe.MEBY_WX3n_s.qp());
    /* p23.MELE*/ BUS_CPU_Dp_out[4].tri6_nn(_VYCU_FF4B_RDn_new, pix_pipe.MYPU_WX4n_s.qp());
    /* p23.MUFE*/ BUS_CPU_Dp_out[5].tri6_nn(_VYCU_FF4B_RDn_new, pix_pipe.MYCE_WX5n_s.qp());
    /* p23.MULY*/ BUS_CPU_Dp_out[6].tri6_nn(_VYCU_FF4B_RDn_new, pix_pipe.MUVO_WX6n_s.qp());
    /* p23.MARA*/ BUS_CPU_Dp_out[7].tri6_nn(_VYCU_FF4B_RDn_new, pix_pipe.NUKU_WX7n_s.qp());
  }

  {
    /* FF50 BOOT */
    /* p07.SYPU*/ BUS_CPU_Dp_out[0].tri6_pn(_TEXE_FF50_RDp_new, BOOT_BITn.qp());
  }

  {
    /* FFFF IE */ // This is technically in the CPU, but we're going to implement it here for now.
    BUS_CPU_Dp_out[0].tri6_nn(FFFF_RDn_new, IE_D0.qn());
    BUS_CPU_Dp_out[1].tri6_nn(FFFF_RDn_new, IE_D1.qn());
    BUS_CPU_Dp_out[2].tri6_nn(FFFF_RDn_new, IE_D2.qn());
    BUS_CPU_Dp_out[3].tri6_nn(FFFF_RDn_new, IE_D3.qn());
    BUS_CPU_Dp_out[4].tri6_nn(FFFF_RDn_new, IE_D4.qn());
  }

  {
    /* EBL -> CBD */
    /* p08.LAVO*/ wire _LAVO_HOLDn = nand3(PIN_CPU_RDp.qp(), _TEXO_8000_9FFFn_new, PIN_CPU_LATCH_EXT_s.qp());
    /*#p08.SOMA*/ ext_bus.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[0].qn());
    /* p08.RONY*/ ext_bus.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[1].qn());
    /* p08.RAXY*/ ext_bus.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[2].qn());
    /* p08.SELO*/ ext_bus.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[3].qn());
    /* p08.SODY*/ ext_bus.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[4].qn());
    /* p08.SAGO*/ ext_bus.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[5].qn());
    /* p08.RUPA*/ ext_bus.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[6].qn());
    /* p08.SAZY*/ ext_bus.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[7].qn());

    /*#p08.RYMA*/ BUS_CPU_Dp_out[0].tri6_nn(_LAVO_HOLDn, ext_bus.SOMA_EXT_DATA_LATCH_D0n.qp());
    /* p08.RUVO*/ BUS_CPU_Dp_out[1].tri6_nn(_LAVO_HOLDn, ext_bus.RONY_EXT_DATA_LATCH_D1n.qp());
    /* p08.RYKO*/ BUS_CPU_Dp_out[2].tri6_nn(_LAVO_HOLDn, ext_bus.RAXY_EXT_DATA_LATCH_D2n.qp());
    /* p08.TAVO*/ BUS_CPU_Dp_out[3].tri6_nn(_LAVO_HOLDn, ext_bus.SELO_EXT_DATA_LATCH_D3n.qp());
    /* p08.TEPE*/ BUS_CPU_Dp_out[4].tri6_nn(_LAVO_HOLDn, ext_bus.SODY_EXT_DATA_LATCH_D4n.qp());
    /* p08.SAFO*/ BUS_CPU_Dp_out[5].tri6_nn(_LAVO_HOLDn, ext_bus.SAGO_EXT_DATA_LATCH_D5n.qp());
    /* p08.SEVU*/ BUS_CPU_Dp_out[6].tri6_nn(_LAVO_HOLDn, ext_bus.RUPA_EXT_DATA_LATCH_D6n.qp());
    /* p08.TAJU*/ BUS_CPU_Dp_out[7].tri6_nn(_LAVO_HOLDn, ext_bus.SAZY_EXT_DATA_LATCH_D7n.qp());
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
    /* p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1(_UMUT_MODE_DBG1p_new);
    /* p07.YULA*/ wire _YULA_BOOT_RDp   = and3(_TEDO_CPU_RDp_new, _YAZA_MODE_DBG1n, _TUTU_ADDR_BOOTp_new); // def AND
    /* p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2(_YULA_BOOT_RDp, _ZUFA_0000_00FFp);
    /* p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1(_ZADO_BOOT_CSn);
    BUS_CPU_Dp_out[0].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x01));
    BUS_CPU_Dp_out[1].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x02));
    BUS_CPU_Dp_out[2].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x04));
    BUS_CPU_Dp_out[3].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x08));
    BUS_CPU_Dp_out[4].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x10));
    BUS_CPU_Dp_out[5].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x20));
    BUS_CPU_Dp_out[6].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x40));
    BUS_CPU_Dp_out[7].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x80));

    Pin2 PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
    PIN_CPU_BOOTp.pin_in(1, _TUTU_ADDR_BOOTp_new);
  }


  {
    /* p28.MYNU*/ wire _MYNU_CPU_RDn = nand2(_ASOT_CPU_RDp_new, _CATY_LATCH_EXTp);
    /* p28.LEKO*/ wire _LEKO_CPU_RDp = not1(_MYNU_CPU_RDn);

    /* VBD  -> CBD */
    {
      /* p25.TYVY*/ wire _TYVY_VBD_TO_CBDn = nand2(_SERE_CPU_VRM_RDp, _LEKO_CPU_RDp);
      /* p25.SEBY*/ wire _SEBY_VBD_TO_CBDp = not1(_TYVY_VBD_TO_CBDn);
      /* p25.RERY*/ wire _RERY_VBUS_D0n = not1(BUS_VRAM_Dp_in[0].qp());
      /* p25.RUNA*/ wire _RUNA_VBUS_D1n = not1(BUS_VRAM_Dp_in[1].qp());
      /* p25.RONA*/ wire _RONA_VBUS_D2n = not1(BUS_VRAM_Dp_in[2].qp());
      /* p25.RUNO*/ wire _RUNO_VBUS_D3n = not1(BUS_VRAM_Dp_in[3].qp());
      /* p25.SANA*/ wire _SANA_VBUS_D4n = not1(BUS_VRAM_Dp_in[4].qp());
      /* p25.RORO*/ wire _RORO_VBUS_D5n = not1(BUS_VRAM_Dp_in[5].qp());
      /* p25.RABO*/ wire _RABO_VBUS_D6n = not1(BUS_VRAM_Dp_in[6].qp());
      /* p25.SAME*/ wire _SAME_VBUS_D7n = not1(BUS_VRAM_Dp_in[7].qp());

      /*#p25.RUGA*/ BUS_CPU_Dp_out[0].tri6_pn(_SEBY_VBD_TO_CBDp, _RERY_VBUS_D0n);
      /* p25.ROTA*/ BUS_CPU_Dp_out[1].tri6_pn(_SEBY_VBD_TO_CBDp, _RUNA_VBUS_D1n);
      /* p25.RYBU*/ BUS_CPU_Dp_out[2].tri6_pn(_SEBY_VBD_TO_CBDp, _RONA_VBUS_D2n);
      /* p25.RAJU*/ BUS_CPU_Dp_out[3].tri6_pn(_SEBY_VBD_TO_CBDp, _RUNO_VBUS_D3n);
      /* p25.TYJA*/ BUS_CPU_Dp_out[4].tri6_pn(_SEBY_VBD_TO_CBDp, _SANA_VBUS_D4n);
      /* p25.REXU*/ BUS_CPU_Dp_out[5].tri6_pn(_SEBY_VBD_TO_CBDp, _RORO_VBUS_D5n);
      /* p25.RUPY*/ BUS_CPU_Dp_out[6].tri6_pn(_SEBY_VBD_TO_CBDp, _RABO_VBUS_D6n);
      /* p25.TOKU*/ BUS_CPU_Dp_out[7].tri6_pn(_SEBY_VBD_TO_CBDp, _SAME_VBUS_D7n);
    }

    /* OBL  -> CBD */
    {
      /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and3(_LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _GEKA_OAM_A0p);
      /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not1(_WUKU_OBL_TO_CBDp);
      /* p31.XACA*/ BUS_CPU_Dp_out[0].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.XYKY_OAM_LATCH_DB0n.qn());
      /* p31.XAGU*/ BUS_CPU_Dp_out[1].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.YRUM_OAM_LATCH_DB1n.qn());
      /* p31.XEPU*/ BUS_CPU_Dp_out[2].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.YSEX_OAM_LATCH_DB2n.qn());
      /* p31.XYGU*/ BUS_CPU_Dp_out[3].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.YVEL_OAM_LATCH_DB3n.qn());
      /* p31.XUNA*/ BUS_CPU_Dp_out[4].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.WYNO_OAM_LATCH_DB4n.qn());
      /* p31.DEVE*/ BUS_CPU_Dp_out[5].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.CYRA_OAM_LATCH_DB5n.qn());
      /* p31.ZEHA*/ BUS_CPU_Dp_out[6].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.ZUVE_OAM_LATCH_DB6n.qn());
      /* p31.FYRA*/ BUS_CPU_Dp_out[7].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.ECED_OAM_LATCH_DB7n.qn());
    }
    {
      /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and3(_LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _WAFO_OAM_A0n);
      /* p28.WUME*/ wire _WUME_OBL_TO_CBDn = not1(_GUKO_OBL_TO_CBDp);
      /* p29.YFAP*/ BUS_CPU_Dp_out[0].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.YDYV_OAM_LATCH_DA0n.qn());
      /* p29.XELE*/ BUS_CPU_Dp_out[1].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.YCEB_OAM_LATCH_DA1n.qn());
      /* p29.YPON*/ BUS_CPU_Dp_out[2].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.ZUCA_OAM_LATCH_DA2n.qn());
      /* p29.XUVO*/ BUS_CPU_Dp_out[3].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.WONE_OAM_LATCH_DA3n.qn());
      /* p29.ZYSA*/ BUS_CPU_Dp_out[4].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.ZAXE_OAM_LATCH_DA4n.qn());
      /* p29.YWEG*/ BUS_CPU_Dp_out[5].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.XAFU_OAM_LATCH_DA5n.qn());
      /* p29.XABU*/ BUS_CPU_Dp_out[6].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.YSES_OAM_LATCH_DA6n.qn());
      /* p29.YTUX*/ BUS_CPU_Dp_out[7].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.ZECA_OAM_LATCH_DA7n.qn());
    }
  }

  {
    // ZRAM control signals are

    // clk_reg.PIN_CPU_BUKE_AxxxxxGH
    // TEDO_CPU_RDp();
    // TAPU_CPU_WRp_xxxxEFGx()
    // _SYKE_FF00_FFFFp
    // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

    if ((cpu_bus_addr >= 0xFF80) && (cpu_bus_addr <= 0xFFFE)) {
      if (_TAPU_CPU_WRp_xxxxEFGx_new) {
        zero_ram[cpu_bus_addr & 0x007F] = pack_u8p_new(8, &BUS_CPU_Dp_in[0]);
      }

      BUS_CPU_Dp_out[0].tri(_TEDO_CPU_RDp_new, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x01));
      BUS_CPU_Dp_out[1].tri(_TEDO_CPU_RDp_new, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x02));
      BUS_CPU_Dp_out[2].tri(_TEDO_CPU_RDp_new, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x04));
      BUS_CPU_Dp_out[3].tri(_TEDO_CPU_RDp_new, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x08));
      BUS_CPU_Dp_out[4].tri(_TEDO_CPU_RDp_new, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x10));
      BUS_CPU_Dp_out[5].tri(_TEDO_CPU_RDp_new, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x20));
      BUS_CPU_Dp_out[6].tri(_TEDO_CPU_RDp_new, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x40));
      BUS_CPU_Dp_out[7].tri(_TEDO_CPU_RDp_new, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x80));
    }
  }

  if (DELTA_GH) {
    cpu_data_latch = pack_u8p_new(8, &BUS_CPU_Dp_out[0]);
  }
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

  /*#p08.KOVA*/ wire KOVA_A00p = not1(PIN_EXT_A[ 0].qn());
  /* p08.CAMU*/ wire CAMU_A01p = not1(PIN_EXT_A[ 1].qn());
  /* p08.BUXU*/ wire BUXU_A02p = not1(PIN_EXT_A[ 2].qn());
  /* p08.BASE*/ wire BASE_A03p = not1(PIN_EXT_A[ 3].qn());
  /* p08.AFEC*/ wire AFEC_A04p = not1(PIN_EXT_A[ 4].qn());
  /* p08.ABUP*/ wire ABUP_A05p = not1(PIN_EXT_A[ 5].qn());
  /* p08.CYGU*/ wire CYGU_A06p = not1(PIN_EXT_A[ 6].qn());
  /* p08.COGO*/ wire COGO_A07p = not1(PIN_EXT_A[ 7].qn());
  /* p08.MUJY*/ wire MUJY_A08p = not1(PIN_EXT_A[ 8].qn());
  /* p08.NENA*/ wire NENA_A09p = not1(PIN_EXT_A[ 9].qn());
  /* p08.SURA*/ wire SURA_A10p = not1(PIN_EXT_A[10].qn());
  /* p08.MADY*/ wire MADY_A11p = not1(PIN_EXT_A[11].qn());
  /* p08.LAHE*/ wire LAHE_A12p = not1(PIN_EXT_A[12].qn());
  /* p08.LURA*/ wire LURA_A13p = not1(PIN_EXT_A[13].qn());
  /* p08.PEVO*/ wire PEVO_A14p = not1(PIN_EXT_A[14].qn());
  /* p08.RAZA*/ wire RAZA_A15p = not1(PIN_EXT_A[15].qn());

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
