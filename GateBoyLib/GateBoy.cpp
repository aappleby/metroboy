#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/Probe.h"


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

  sim_stable = 0;
  phase_total = 0;
  pass_count = 0;
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

  sim_stable = 1;
  phase_total = 0x02cf5798;
  pass_count = 0;
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

  CHECK_N(clk_reg.AFUR_xxxxEFGHp.qp09_old());
  CHECK_P(clk_reg.ALEF_AxxxxFGHp.qp09_old());
  CHECK_P(clk_reg.APUK_ABxxxxGHp.qp09_old());
  CHECK_P(clk_reg.ADYK_ABCxxxxHp.qp09_old());

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

} gb_offsets;

void GateBoy::next_pass() {

  //----------------------------------------
  // Run one pass of our simulation.

  static GateBoy gb_old;

  if (sys_statediff) {
    gb_old = *this;
  }

  tock_slow();
  commit_and_hash();

  if (sys_statediff && pass_count > 1) {
    int start = offsetof(GateBoy, sentinel1) + sizeof(sentinel1);
    int end   = offsetof(GateBoy, sentinel2);

    uint8_t* blob_old = (uint8_t*)&gb_old;
    uint8_t* blob_new = (uint8_t*)this;

    for (int i = start; i < end; i++) {
      if (blob_old[i] != blob_new[i]) {
        printf("%06d %02d %04d %02d %02d\n", phase_total, pass_count, i, blob_old[i], blob_new[i]);
      }
    }
  }

  //----------------------------------------
  // Once the simulation converges, latch the data that needs to go back to the
  // CPU or test function and update the CPU if necessary.

  if (sim_stable) {

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

    pass_total += pass_count;
    pass_count = 0;
    phase_total++;
    combine_hash(total_hash, pass_hash);
  }

}

//-----------------------------------------------------------------------------

void GateBoy::commit_and_hash() {
  uint8_t* blob_begin = ((uint8_t*)&sentinel1) + sizeof(sentinel1);
  uint8_t* blob_end   = ((uint8_t*)&sentinel2);
  uint64_t pass_hash_new = ::commit_and_hash(blob_begin, int(blob_end - blob_begin));

  uint64_t pass_hash_old = pass_hash;
  sim_stable = pass_hash_old == pass_hash_new;
  pass_hash = pass_hash_new;
  pass_count++;

  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);
  probes.end_pass(sim_stable);

  if (pass_count > 90) printf("!!!STUCK!!!\n");
}

//-----------------------------------------------------------------------------

void GateBoy::tock_slow() {
  probes.begin_pass(pass_count);

  //----------------------------------------

  if (DELTA_AB) {
    cpu_req = cpu.bus_req;
    bus_req = {0};
    if (sys_cpu_en) bus_req = cpu_req;
    if (dbg_req.read || dbg_req.write) bus_req = dbg_req;
  }

  // Interrupt acks
  Pin2 PIN_CPU_ACK_VBLANK_t0; // bottom right port PORTB_01: -> P02.LETY, vblank int ack
  Pin2 PIN_CPU_ACK_STAT_t0  ; // bottom right port PORTB_05: -> P02.LEJA, stat int ack
  Pin2 PIN_CPU_ACK_TIMER_t0 ; // bottom right port PORTB_09: -> P02.LESA, timer int ack
  Pin2 PIN_CPU_ACK_SERIAL_t0; // bottom right port PORTB_13: -> P02.LUFE, serial int ack
  Pin2 PIN_CPU_ACK_JOYPAD_t0; // bottom right port PORTB_17: -> P02.LAMO, joypad int ack

  PIN_CPU_ACK_VBLANK_t0.pin_in(1, wire(cpu.int_ack & INT_VBLANK_MASK));
  PIN_CPU_ACK_STAT_t0  .pin_in(1, wire(cpu.int_ack & INT_STAT_MASK));
  PIN_CPU_ACK_TIMER_t0 .pin_in(1, wire(cpu.int_ack & INT_TIMER_MASK));
  PIN_CPU_ACK_SERIAL_t0.pin_in(1, wire(cpu.int_ack & INT_SERIAL_MASK));
  PIN_CPU_ACK_JOYPAD_t0.pin_in(1, wire(cpu.int_ack & INT_JOYPAD_MASK));

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
  Pin2 PIN_CPU_RDp_t0;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  Pin2 PIN_CPU_WRp_t0;           // top right port PORTA_01: ->
  {
    if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      PIN_CPU_RDp_t0.pin_in(1, bus_req.read);
      PIN_CPU_WRp_t0.pin_in(1, bus_req.write);
    }
    else {
      PIN_CPU_RDp_t0.pin_in(1, 0);
      PIN_CPU_WRp_t0.pin_in(1, 0);
    }
  }

  // PIN_CPU_LATCH_EXT
  Pin2 PIN_CPU_LATCH_EXT_t0;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU
  {
    // not at all certain about this. seems to break some oam read glitches.
    if ((DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && (bus_req.read && (bus_req.addr < 0xFF00))) {
      PIN_CPU_LATCH_EXT_t0.pin_in(1, 1);
    }
    else {
      PIN_CPU_LATCH_EXT_t0.pin_in(1, 0);
    }
  }

  Pin2 PIN_CPU_6_t0;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6?
  PIN_CPU_6_t0.pin_in(1, 0);


  // Data has to be driven on EFGH or we fail the wave tests

  wire BUS_C2S_D_OEp = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && bus_req.write;

  BUS_CPU_Dp_in[0].tri(BUS_C2S_D_OEp, bus_req.data_lo & 0x01);
  BUS_CPU_Dp_in[1].tri(BUS_C2S_D_OEp, bus_req.data_lo & 0x02);
  BUS_CPU_Dp_in[2].tri(BUS_C2S_D_OEp, bus_req.data_lo & 0x04);
  BUS_CPU_Dp_in[3].tri(BUS_C2S_D_OEp, bus_req.data_lo & 0x08);
  BUS_CPU_Dp_in[4].tri(BUS_C2S_D_OEp, bus_req.data_lo & 0x10);
  BUS_CPU_Dp_in[5].tri(BUS_C2S_D_OEp, bus_req.data_lo & 0x20);
  BUS_CPU_Dp_in[6].tri(BUS_C2S_D_OEp, bus_req.data_lo & 0x40);
  BUS_CPU_Dp_in[7].tri(BUS_C2S_D_OEp, bus_req.data_lo & 0x80);

  // PIN_CPU_EXT_BUSp
  Pin2 PIN_CPU_EXT_BUSp_t0;      // top right port PORTA_06: -> TEXO, APAP
  {
    bool addr_ext = (bus_req.read || bus_req.write) && (bus_req.addr < 0xFE00);
    if (bus_req.addr <= 0x00FF && !BOOT_BITn.qp17_old()) addr_ext = false;

    if (DELTA_AB || DELTA_BC || DELTA_CD || DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      PIN_CPU_EXT_BUSp_t0.pin_in(1, addr_ext);
    }
    else {
      // This seems wrong, but it passes tests. *shrug*
      if (bus_req.addr >= 0x8000 && bus_req.addr <= 0x9FFF) {
        PIN_CPU_EXT_BUSp_t0.pin_in(1, 0);
      }
      else {
        PIN_CPU_EXT_BUSp_t0.pin_in(1, addr_ext);
      }
    }
  }

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  wire CLK_t0 = !(phase_total & 1) && sys_clken;
  wire GND = 0;
  wire _WEFE_VCC = 1;
  wire _VYPO_VCC = 1;
  wire _PESU_VCC = 1;

  //----------------------------------------
  // Address decoders

  bool BUS_CPU_A_t0[16];
  BUS_CPU_A_t0[ 0] = wire(cpu_bus_addr & 0x0001);
  BUS_CPU_A_t0[ 1] = wire(cpu_bus_addr & 0x0002);
  BUS_CPU_A_t0[ 2] = wire(cpu_bus_addr & 0x0004);
  BUS_CPU_A_t0[ 3] = wire(cpu_bus_addr & 0x0008);
  BUS_CPU_A_t0[ 4] = wire(cpu_bus_addr & 0x0010);
  BUS_CPU_A_t0[ 5] = wire(cpu_bus_addr & 0x0020);
  BUS_CPU_A_t0[ 6] = wire(cpu_bus_addr & 0x0040);
  BUS_CPU_A_t0[ 7] = wire(cpu_bus_addr & 0x0080);
  BUS_CPU_A_t0[ 8] = wire(cpu_bus_addr & 0x0100);
  BUS_CPU_A_t0[ 9] = wire(cpu_bus_addr & 0x0200);
  BUS_CPU_A_t0[10] = wire(cpu_bus_addr & 0x0400);
  BUS_CPU_A_t0[11] = wire(cpu_bus_addr & 0x0800);
  BUS_CPU_A_t0[12] = wire(cpu_bus_addr & 0x1000);
  BUS_CPU_A_t0[13] = wire(cpu_bus_addr & 0x2000);
  BUS_CPU_A_t0[14] = wire(cpu_bus_addr & 0x4000);
  BUS_CPU_A_t0[15] = wire(cpu_bus_addr & 0x8000);

  /* p06.SARE*/ wire _SARE_XX00_XX07p_t0 = nor5 (BUS_CPU_A_t0[ 7], BUS_CPU_A_t0[ 6], BUS_CPU_A_t0[ 5],
                                                 BUS_CPU_A_t0[ 4], BUS_CPU_A_t0[ 3]);
  /* p07.TUNA*/ wire _TUNA_0000_FDFFp_t0 = nand7(BUS_CPU_A_t0[15], BUS_CPU_A_t0[14], BUS_CPU_A_t0[13],
                                                 BUS_CPU_A_t0[12], BUS_CPU_A_t0[11], BUS_CPU_A_t0[10],
                                                 BUS_CPU_A_t0[ 9]);

  /* p07.TULO*/ wire _TULO_00000000_XXp_t0 = nor8 (BUS_CPU_A_t0[15], BUS_CPU_A_t0[14], BUS_CPU_A_t0[13], BUS_CPU_A_t0[12],
                                                   BUS_CPU_A_t0[11], BUS_CPU_A_t0[10], BUS_CPU_A_t0[ 9], BUS_CPU_A_t0[ 8]);
  /* p07.ZORO*/ wire _ZORO_0000xxxx_XXp_t0 = nor4 (BUS_CPU_A_t0[15], BUS_CPU_A_t0[14], BUS_CPU_A_t0[13], BUS_CPU_A_t0[12]);
  /* p07.ZADU*/ wire _ZADU_xxxx0000_XXp_t0 = nor4 (BUS_CPU_A_t0[11], BUS_CPU_A_t0[10], BUS_CPU_A_t0[ 9], BUS_CPU_A_t0[ 8]);

  /* p07.SEMY*/ wire _SEMY_XX_0000xxxxp_t0 = nor4(BUS_CPU_A_t0[ 7], BUS_CPU_A_t0[ 6], BUS_CPU_A_t0[ 5], BUS_CPU_A_t0[ 4]);
  /* p07.SAPA*/ wire _SAPA_XX_xxxx1111p_t0 = and4(BUS_CPU_A_t0[ 0], BUS_CPU_A_t0[ 1], BUS_CPU_A_t0[ 2], BUS_CPU_A_t0[ 3]);
  /* p22.XALY*/ wire _XALY_XX_0x00xxxxp_t0 = nor3(BUS_CPU_A_t0[ 7], BUS_CPU_A_t0[ 5], BUS_CPU_A_t0[ 4]);
  /* p10.AMUS*/ wire _AMUS_XX_0xx00000p_t0 = nor6(BUS_CPU_A_t0[ 0], BUS_CPU_A_t0[ 1], BUS_CPU_A_t0[ 2], BUS_CPU_A_t0[ 3], BUS_CPU_A_t0[ 4], BUS_CPU_A_t0[ 7]);
  /* p07.TYRO*/ wire _TYFO_XX_0x0x0000p_t0 = nor6(BUS_CPU_A_t0[ 7], BUS_CPU_A_t0[ 5], BUS_CPU_A_t0[ 3], BUS_CPU_A_t0[ 2], BUS_CPU_A_t0[ 1], BUS_CPU_A_t0[ 0]);
  /* p07.TUFA*/ wire _TUFA_XX_x1x1xxxxp_t0 = and2(BUS_CPU_A_t0[ 4], BUS_CPU_A_t0[ 6]);

  /* p03.TOVY*/ wire _TOVY_A00n_t0 = not1(BUS_CPU_A_t0[ 0]);
  /* p22.XOLA*/ wire _XOLA_A00n_t0 = not1(BUS_CPU_A_t0[ 0]);
  /* p08.TOLA*/ wire _TOLA_A01n_t0 = not1(BUS_CPU_A_t0[ 1]);
  /* p22.XENO*/ wire _XENO_A01n_t0 = not1(BUS_CPU_A_t0[ 1]);
  /* p06.SEFY*/ wire _SEFY_A02n_t0 = not1(BUS_CPU_A_t0[ 2]);
  /* p22.XUSY*/ wire _XUSY_A02n_t0 = not1(BUS_CPU_A_t0[ 2]);
  /* p22.XERA*/ wire _XERA_A03n_t0 = not1(BUS_CPU_A_t0[ 3]);
  /* p10.BYKO*/ wire _BYKO_A05n_t0 = not1(BUS_CPU_A_t0[ 5]);
  /* p10.AKUG*/ wire _AKUG_A06n_t0 = not1(BUS_CPU_A_t0[ 6]);
  /* p07.TONA*/ wire _TONA_A08n_t0 = not1(BUS_CPU_A_t0[ 8]);

  /* p22.WADO*/ wire _WADO_A00p_t0 = not1(_XOLA_A00n_t0);
  /* p22.WESA*/ wire _WESA_A01p_t0 = not1(_XENO_A01n_t0);
  /* p22.WALO*/ wire _WALO_A02p_t0 = not1(_XUSY_A02n_t0);
  /* p22.WEPO*/ wire _WEPO_A03p_t0 = not1(_XERA_A03n_t0);

  /* p07.ZUFA*/ wire _ZUFA_0000_00FFp_t0  = and2(_ZORO_0000xxxx_XXp_t0, _ZADU_xxxx0000_XXp_t0);
  /* p07.SYKE*/ wire _SYKE_FF00_FFFFp_t0  = nor2(_TUNA_0000_FDFFp_t0, _TONA_A08n_t0);
  /* p03.RYFO*/ wire _RYFO_FF04_FF07p_t0  = and3(_SYKE_FF00_FFFFp_t0, BUS_CPU_A_t0[ 2], _SARE_XX00_XX07p_t0);
  /* p25.SYRO*/ wire _SYRO_FE00_FFFFp_t0  = not1(_TUNA_0000_FDFFp_t0);
  /* p07.RYCU*/ wire _RYCU_0000_FDFFn_t0  = not1(_TUNA_0000_FDFFp_t0);
  /* p07.SOHA*/ wire _SOHA_FF00_FFFFn_t0  = not1(_SYKE_FF00_FFFFp_t0);
  /* p07.ROPE*/ wire _ROPE_FE00_FEFFn_t0  = nand2(_SOHA_FF00_FFFFn_t0, _RYCU_0000_FDFFn_t0);
  /* p07.SARO*/ wire _SARO_FE00_FEFFp_t0  = not1(_ROPE_FE00_FEFFn_t0);
  /* p28.ADAH*/ wire _ADAH_FE00_FEFFn_t0  = not1(_SARO_FE00_FEFFp_t0);
  /*#p08.SORE*/ wire _SORE_0000_7FFFp_t0  = not1(BUS_CPU_A_t0[15]);
  /*#p08.TEVY*/ wire _TEVY_8000_9FFFn_t0  = or3(BUS_CPU_A_t0[13], BUS_CPU_A_t0[14], _SORE_0000_7FFFp_t0);
  /*#p08.TEXO*/ wire _TEXO_8000_9FFFn_t0  = and2(PIN_CPU_EXT_BUSp_t0.qp(), _TEVY_8000_9FFFn_t0);
  /*#p25.TEFA*/ wire _TEFA_8000_9FFFp_t0  = nor2(_SYRO_FE00_FFFFp_t0, _TEXO_8000_9FFFn_t0);
  /*#p25.SOSE*/ wire _SOSE_8000_9FFFp_t0  = and2(_TEFA_8000_9FFFp_t0, BUS_CPU_A_t0[15]);
  /* p06.SANO*/ wire _SANO_FF00_FF03p_t0  = and3(_SARE_XX00_XX07p_t0, _SEFY_A02n_t0, _SYKE_FF00_FFFFp_t0);
  /* p10.ANAP*/ wire _ANAP_FF_0xx00000_t0 = and2(_SYKE_FF00_FFFFp_t0, _AMUS_XX_0xx00000p_t0);
  /* p22.WUTU*/ wire _WUTU_FF40_FF4Fn_t0  = nand3(_SYKE_FF00_FFFFp_t0, BUS_CPU_A_t0[ 6], _XALY_XX_0x00xxxxp_t0);
  /* p22.WERO*/ wire _WERO_FF40_FF4Fp_t0  = not1(_WUTU_FF40_FF4Fn_t0);

  /* p22.WORU*/ wire _WORU_FF40n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _XOLA_A00n_t0, _XENO_A01n_t0, _XUSY_A02n_t0, _XERA_A03n_t0);
  /* p22.WOFA*/ wire _WOFA_FF41n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _WADO_A00p_t0, _XENO_A01n_t0, _XUSY_A02n_t0, _XERA_A03n_t0);
  /* p22.WEBU*/ wire _WEBU_FF42n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _XOLA_A00n_t0, _WESA_A01p_t0, _XUSY_A02n_t0, _XERA_A03n_t0);
  /* p22.WAVU*/ wire _WAVU_FF43n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _WADO_A00p_t0, _WESA_A01p_t0, _XUSY_A02n_t0, _XERA_A03n_t0);
  /* p22.WYLE*/ wire _WYLE_FF44n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _XOLA_A00n_t0, _XENO_A01n_t0, _WALO_A02p_t0, _XERA_A03n_t0);
  /* p22.WETY*/ wire _WETY_FF45n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _WADO_A00p_t0, _XENO_A01n_t0, _WALO_A02p_t0, _XERA_A03n_t0);
  /*#p22.WATE*/ wire _WATE_FF46n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _XOLA_A00n_t0, _WESA_A01p_t0, _WALO_A02p_t0, _XERA_A03n_t0);
  /* p22.WYBO*/ wire _WYBO_FF47n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _WADO_A00p_t0, _WESA_A01p_t0, _WALO_A02p_t0, _XERA_A03n_t0);
  /* p22.WETA*/ wire _WETA_FF48n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _XOLA_A00n_t0, _XENO_A01n_t0, _XUSY_A02n_t0, _WEPO_A03p_t0);
  /* p22.VAMA*/ wire _VAMA_FF49n_t0 = nand5(_WERO_FF40_FF4Fp_t0, _WADO_A00p_t0, _XENO_A01n_t0, _XUSY_A02n_t0, _WEPO_A03p_t0);
  /* p22.WYVO*/ wire _WYVO_FF4An_t0 = nand5(_WERO_FF40_FF4Fp_t0, _XOLA_A00n_t0, _WESA_A01p_t0, _XUSY_A02n_t0, _WEPO_A03p_t0);
  /* p22.WAGE*/ wire _WAGE_FF4Bn_t0 = nand5(_WERO_FF40_FF4Fp_t0, _WADO_A00p_t0, _WESA_A01p_t0, _XUSY_A02n_t0, _WEPO_A03p_t0);

  /* p22.VOCA*/ wire _VOCA_FF40p_t0 = not1(_WORU_FF40n_t0);
  /* p22.VARY*/ wire _VARY_FF41p_t0 = not1(_WOFA_FF41n_t0);
  /* p22.XARO*/ wire _XARO_FF42p_t0 = not1(_WEBU_FF42n_t0);
  /* p22.XAVY*/ wire _XAVY_FF43p_t0 = not1(_WAVU_FF43n_t0);
  /* p22.XOGY*/ wire _XOGY_FF44p_t0 = not1(_WYLE_FF44n_t0);
  /* p22.XAYU*/ wire _XAYU_FF45p_t0 = not1(_WETY_FF45n_t0);
  /*#p22.XEDA*/ wire _XEDA_FF46p_t0 = not1(_WATE_FF46n_t0);
  /* p22.WERA*/ wire _WERA_FF47p_t0 = not1(_WYBO_FF47n_t0);
  /* p22.XAYO*/ wire _XAYO_FF48p_t0 = not1(_WETA_FF48n_t0);
  /* p22.TEGO*/ wire _TEGO_FF49p_t0 = not1(_VAMA_FF49n_t0);
  /* p22.VYGA*/ wire _VYGA_FF4Ap_t0 = not1(_WYVO_FF4An_t0);
  /* p22.VUMY*/ wire _VUMY_FF4Bp_t0 = not1(_WAGE_FF4Bn_t0);

  /* p08.SOGY*/ wire _SOGY_A14n = not1(BUS_CPU_A_t0[14]);
  /* p08.TUMA*/ wire _TUMA_CART_RAM = and3(BUS_CPU_A_t0[13], _SOGY_A14n, BUS_CPU_A_t0[15]);
  /* p08.TYNU*/ wire _TYNU_ADDR_RAM = and_or3(BUS_CPU_A_t0[15], BUS_CPU_A_t0[14], _TUMA_CART_RAM);

  {
    Pin2 PIN_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
    PIN_CPU_ADDR_HIp.pin_in(1, _SYRO_FE00_FFFFp_t0);
  }

  //----------------------------------------
  // Debug control signals.

  /* p07.UBET*/ wire _UBET_T1p_t0  = not1(wire(sys_t1));
  /* p07.UVAR*/ wire _UVAR_T2p_t0  = not1(wire(sys_t2));
  /* p07.UMUT*/ wire _UMUT_MODE_DBG1p_t0 = and2(wire(sys_t1), _UVAR_T2p_t0);
  /* p07.UNOR*/ wire _UNOR_MODE_DBG2p_t0 = and2(wire(sys_t2), _UBET_T1p_t0);
  /* p07.UPOJ*/ wire _UPOJ_MODE_PRODn_t0 = nand3(_UBET_T1p_t0, _UVAR_T2p_t0, wire(sys_rst));
  /* p08.RYCA*/ wire _RYCA_MODE_DBG2n_t0 = not1(_UNOR_MODE_DBG2p_t0);
  /* p08.TOVA*/ wire _TOVA_MODE_DBG2n_t0 = not1(_UNOR_MODE_DBG2p_t0);
  /* p08.MULE*/ wire _MULE_MODE_DBG1n_t0 = not1(_UMUT_MODE_DBG1p_t0);

  {
    Pin2 PIN_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
    Pin2 PIN_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
    PIN_CPU_UNOR_DBG.pin_in(1, _UNOR_MODE_DBG2p_t0);
    PIN_CPU_UMUT_DBG.pin_in(1, _UMUT_MODE_DBG1p_t0);
  }

  //----------------------------------------
  // Root clocks - ignoring the deglitcher here

  /* p01.UCOB*/ wire _UCOB_CLKBADp_t0 = not1(sys_clkgood);
  /* p01.ATEZ*/ wire _ATEZ_CLKBAD_t0   = not1(sys_clkgood);
  /* p01.ABOL*/ wire _ABOL_CLKREQn_t0 = not1(sys_cpuready);

  /* p01.ATAL*/ wire _ATAL_xBxDxFxH_t0 = CLK_t0;
  /* p01.AZOF*/ wire _AZOF_AxCxExGx_t0 = not1(_ATAL_xBxDxFxH_t0);
  /* p01.ZAXY*/ wire _ZAXY_xBxDxFxH_t0 = not1(_AZOF_AxCxExGx_t0);
  /*#p01.ZEME*/ wire _ZEME_AxCxExGx_t0 = not1(_ZAXY_xBxDxFxH_t0);
  /* p29.XYVA*/ wire _XYVA_xBxDxFxH_t0 = not1(_ZEME_AxCxExGx_t0);
  /* p29.XOTA*/ wire _XOTA_AxCxExGx_t0 = not1(_XYVA_xBxDxFxH_t0);
  /* p29.XYFY*/ wire _XYFY_xBxDxFxH_t0 = not1(_XOTA_AxCxExGx_t0);
  /*#p01.ALET*/ wire _ALET_xBxDxFxH_t0 = not1(_ZEME_AxCxExGx_t0);
  /*#p27.MYVO*/ wire _MYVO_AxCxExGx_t0 = not1(_ALET_xBxDxFxH_t0);
  /*#p27.MEHE*/ wire _MEHE_AxCxExGx_t0 = not1(_ALET_xBxDxFxH_t0);
  /* p01.LAPE*/ wire _LAPE_AxCxExGx_t0 = not1(_ALET_xBxDxFxH_t0);
  /* p27.MOXE*/ wire _MOXE_AxCxExGx_t0 = not1(_ALET_xBxDxFxH_t0);
  /* p27.TAVA*/ wire _TAVA_xBxDxFxH_t0 = not1(_LAPE_AxCxExGx_t0);

  //----------------------------------------
  // Sys clock signals

  {
    wire AFUR_xxxxEFGHp = clk_reg.ADYK_ABCxxxxHp.qp09_old();
    wire ALEF_AxxxxFGHn = clk_reg.AFUR_xxxxEFGHp.qn08_old();
    wire APUK_ABxxxxGHn = clk_reg.ALEF_AxxxxFGHp.qn08_old();
    wire ADYK_ABCxxxxHn = clk_reg.APUK_ABxxxxGHp.qn08_old();

    /* p01.AFUR*/ clk_reg.AFUR_xxxxEFGHp.dff9(!_ATAL_xBxDxFxH_t0, _UPOJ_MODE_PRODn_t0, AFUR_xxxxEFGHp);
    /* p01.ALEF*/ clk_reg.ALEF_AxxxxFGHp.dff9( _ATAL_xBxDxFxH_t0, _UPOJ_MODE_PRODn_t0, ALEF_AxxxxFGHn);
    /* p01.APUK*/ clk_reg.APUK_ABxxxxGHp.dff9(!_ATAL_xBxDxFxH_t0, _UPOJ_MODE_PRODn_t0, APUK_ABxxxxGHn);
    /* p01.ADYK*/ clk_reg.ADYK_ABCxxxxHp.dff9( _ATAL_xBxDxFxH_t0, _UPOJ_MODE_PRODn_t0, ADYK_ABCxxxxHn);
  }

  /*#p01.ATYP*/ wire _ATYP_ABCDxxxx_t1 = not1(clk_reg.AFUR_xxxxEFGHp.qp09_new());
  /*#p01.AFEP*/ wire _AFEP_AxxxxFGH_t1 = not1(clk_reg.ALEF_AxxxxFGHp.qn08_new());
  /*#p01.AROV*/ wire _AROV_xxCDEFxx_t1 = not1(clk_reg.APUK_ABxxxxGHp.qp09_new());
  /*#p01.ADAR*/ wire _ADAR_ABCxxxxH_t1 = not1(clk_reg.ADYK_ABCxxxxHp.qn08_new());

  /*#p01.AFAS*/ wire _AFAS_xxxxEFGx_t1 = nor2(_ADAR_ABCxxxxH_t1, _ATYP_ABCDxxxx_t1);
  /*#p01.BELU*/ wire _BELU_xxxxEFGH_t1 = nor2(_ATYP_ABCDxxxx_t1, _ABOL_CLKREQn_t0);
  /*#p01.BYRY*/ wire _BYRY_ABCDxxxx_t1 = not1(_BELU_xxxxEFGH_t1);
  /*#p01.BUDE*/ wire _BUDE_xxxxEFGH_t1 = not1(_BYRY_ABCDxxxx_t1);
  /*#p01.BAPY*/ wire _BAPY_xxxxxxGH_t1 = nor3(_ABOL_CLKREQn_t0, _AROV_xxCDEFxx_t1, _ATYP_ABCDxxxx_t1);
  /*#p01.BERU*/ wire _BERU_ABCDEFxx_t1 = not1(_BAPY_xxxxxxGH_t1);
  /*#p01.BUFA*/ wire _BUFA_xxxxxxGH_t1 = not1(_BERU_ABCDEFxx_t1);
  /*#p01.BOLO*/ wire _BOLO_ABCDEFxx_t1 = not1(_BUFA_xxxxxxGH_t1);
  /*#p01.BEKO*/ wire _BEKO_ABCDxxxx_t1 = not1(_BUDE_xxxxEFGH_t1); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire _BEJA_xxxxEFGH_t1 = nand4(_BOLO_ABCDEFxx_t1, _BOLO_ABCDEFxx_t1, _BEKO_ABCDxxxx_t1, _BEKO_ABCDxxxx_t1);
  /*#p01.BANE*/ wire _BANE_ABCDxxxx_t1 = not1(_BEJA_xxxxEFGH_t1);
  /*#p01.BELO*/ wire _BELO_xxxxEFGH_t1 = not1(_BANE_ABCDxxxx_t1);
  /*#p01.BAZE*/ wire _BAZE_ABCDxxxx_t1 = not1(_BELO_xxxxEFGH_t1);
  /*#p01.BUTO*/ wire _BUTO_xBCDEFGH_t1 = nand3(_AFEP_AxxxxFGH_t1, _ATYP_ABCDxxxx_t1, _BAZE_ABCDxxxx_t1);
  /*#p01.BELE*/ wire _BELE_Axxxxxxx_t1 = not1(_BUTO_xBCDEFGH_t1);
  /*#p01.BYJU*/ wire _BYJU_Axxxxxxx_t1 = or2(_BELE_Axxxxxxx_t1, _ATEZ_CLKBAD_t0);
  /*#p01.BALY*/ wire _BALY_xBCDEFGH_t1 = not1(_BYJU_Axxxxxxx_t1);
  /* p01.BOGA*/ wire _BOGA_Axxxxxxx_t1 = not1(_BALY_xBCDEFGH_t1);
  /*#p01.AJAX*/ wire _AJAX_xxxxEFGH_t1 = not1(_ATYP_ABCDxxxx_t1);
  /* p01.UVYT*/ wire _UVYT_ABCDxxxx_t1 = not1(_BUDE_xxxxEFGH_t1);
  /* p04.MOPA*/ wire _MOPA_xxxxEFGH_t1 = not1(_UVYT_ABCDxxxx_t1);
  /*#p01.AGUT*/ wire _AGUT_xxCDEFGH_t1 = or_and3(_AROV_xxCDEFxx_t1, _AJAX_xxxxEFGH_t1, PIN_CPU_EXT_BUSp_t0.qp());
  /*#p01.AWOD*/ wire _AWOD_ABxxxxxx_t1 = nor2(_UNOR_MODE_DBG2p_t0, _AGUT_xxCDEFGH_t1);
  /*#p01.ABUZ*/ wire _ABUZ_xxCDEFGH_t1 = not1(_AWOD_ABxxxxxx_t1);

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

    /*#p01.BUTY*/ wire _BUTY_CLKREQ_t0   = not1(_ABOL_CLKREQn_t0);
    /*#p01.BUVU*/ wire _BUVU_Axxxxxxx_t1 = and2(_BALY_xBCDEFGH_t1, _BUTY_CLKREQ_t0);
    /*#p01.BYXO*/ wire _BYXO_xBCDEFGH_t1 = not1(_BUVU_Axxxxxxx_t1);
    /*#p01.BEDO*/ wire _BEDO_Axxxxxxx_t1 = not1(_BYXO_xBCDEFGH_t1);
    /*#p01.BOWA*/ wire _BOWA_xBCDEFGH_t1 = not1(_BEDO_Axxxxxxx_t1);

    /*#p01.BUGO*/ wire _BUGO_xBCDExxx_t1 = not1(_AFEP_AxxxxFGH_t1);
    /*#p01.BATE*/ wire _BATE_AxxxxxGH_t1 = nor3(_BUGO_xBCDExxx_t1, _AROV_xxCDEFxx_t1, _ABOL_CLKREQn_t0);
    /*#p01.BASU*/ wire _BASU_xBCDEFxx_t1 = not1(_BATE_AxxxxxGH_t1);
    /*#p01.BUKE*/ wire _BUKE_AxxxxxGH_t1 = not1(_BASU_xBCDEFxx_t1);
    /*#p01.BOMA*/ wire _BOMA_xBCDEFGH_t1 = not1(_BOGA_Axxxxxxx_t1);

    PIN_CPU_BOWA_Axxxxxxx.pin_in(1, _BOWA_xBCDEFGH_t1);
    PIN_CPU_BEDO_xBCDEFGH.pin_in(1, _BEDO_Axxxxxxx_t1);
    PIN_CPU_BEKO_ABCDxxxx.pin_in(1, _BEKO_ABCDxxxx_t1);
    PIN_CPU_BUDE_xxxxEFGH.pin_in(1, _BUDE_xxxxEFGH_t1);
    PIN_CPU_BOLO_ABCDEFxx.pin_in(1, _BOLO_ABCDEFxx_t1);
    PIN_CPU_BUKE_AxxxxxGH.pin_in(1, _BUKE_AxxxxxGH_t1);
    PIN_CPU_BOMA_xBCDEFGH.pin_in(1, _BOMA_xBCDEFGH_t1);
    PIN_CPU_BOGA_Axxxxxxx.pin_in(1, _BOGA_Axxxxxxx_t1);
  }

  //----------------------------------------
  // Power-on reset handler

  {
    /* p01.UPYF*/ wire _UPYF_t0 = or2(sys_rst, _UCOB_CLKBADp_t0);
    /* p01.TUBO*/ clk_reg.TUBO_WAITINGp.nor_latch(_UPYF_t0, sys_cpuready);

    /* p01.UNUT*/ wire _UNUT_POR_TRIGn_t0 = and2(clk_reg.TUBO_WAITINGp.qp04_new(), sys_fastboot ? tim_reg.TERO_DIV03p.qp17_old() : tim_reg.UPOF_DIV15p.qp17_old());
    /* p01.TABA*/ wire _TABA_POR_TRIGn_t0 = or3(_UNOR_MODE_DBG2p_t0, _UMUT_MODE_DBG1p_t0, _UNUT_POR_TRIGn_t0);
    /*#p01.ALYP*/ wire _ALYP_RSTn_t0 = not1(_TABA_POR_TRIGn_t0);
    /*#p01.AFAR*/ wire _AFAR_RSTp_t0  = nor2(sys_rst, _ALYP_RSTn_t0);
    /* p01.ASOL*/ clk_reg.ASOL_POR_DONEn.nor_latch(sys_rst, _AFAR_RSTp_t0); // Schematic wrong, this is a latch.

    Pin2 PIN_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
    Pin2 PIN_CPU_EXT_RST;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
    Pin2 PIN_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET

    PIN_CPU_EXT_CLKGOOD.pin_in(1, sys_clkgood);
    PIN_CPU_EXT_RST.pin_in(1, sys_rst);
    PIN_CPU_STARTp.pin_in(1, _TABA_POR_TRIGn_t0);
    sys_cpu_start = PIN_CPU_STARTp.qp();

    /* p01.AFER*/ clk_reg.AFER_SYS_RSTp.dff13(_BOGA_Axxxxxxx_t1, _UPOJ_MODE_PRODn_t0, clk_reg.ASOL_POR_DONEn.qp04_new());
    Pin2 PIN_CPU_SYS_RSTp;      // top center port PORTC_01: <- P01.AFER , reset related state
    PIN_CPU_SYS_RSTp.pin_in(1, clk_reg.AFER_SYS_RSTp.qp13_new());
  }

  //----------------------------------------
  // Reset signals

  /*#p01.AVOR*/ wire _AVOR_SYS_RSTp_t0 = or2(clk_reg.AFER_SYS_RSTp.qp13_new(), clk_reg.ASOL_POR_DONEn.qp04_new());

  /*#p01.ALUR*/ wire _ALUR_SYS_RSTn_t0 = not1(_AVOR_SYS_RSTp_t0);
  /*#p01.DULA*/ wire _DULA_SYS_RSTp_t0 = not1(_ALUR_SYS_RSTn_t0);
  /*#p01.CUNU*/ wire _CUNU_SYS_RSTn_t0 = not1(_DULA_SYS_RSTp_t0);
  /*#p01.XORE*/ wire _XORE_SYS_RSTp_t0 = not1(_CUNU_SYS_RSTn_t0);
  /* p01.WESY*/ wire _WESY_SYS_RSTn_t0 = not1(_XORE_SYS_RSTp_t0);
  /* p01.XEBE*/ wire _XEBE_SYS_RSTn_t0 = not1(_XORE_SYS_RSTp_t0);
  /*#p01.WALU*/ wire _WALU_SYS_RSTn_t0 = not1(_XORE_SYS_RSTp_t0);
  /* p03.MULO*/ wire _MULO_SYS_RSTn_t0 = not1(_ALUR_SYS_RSTn_t0);
  /* p01.XARE*/ wire _XARE_SYS_RSTn_t0 = not1(_XORE_SYS_RSTp_t0);

  /* p01.XODO*/ wire _XODO_VID_RSTp_t0 = nand2(_XEBE_SYS_RSTn_t0, pix_pipe.XONA_LCDC_LCDENn.qn08_old());
  /* p01.XAPO*/ wire _XAPO_VID_RSTn_t0 = not1(_XODO_VID_RSTp_t0);
  /*#p01.ATAR*/ wire _ATAR_VID_RSTp_t0 = not1(_XAPO_VID_RSTn_t0);
  /*#p01.ABEZ*/ wire _ABEZ_VID_RSTn_t0 = not1(_ATAR_VID_RSTp_t0);
  /* p01.LYHA*/ wire _LYHA_VID_RSTp_t0 = not1(_XAPO_VID_RSTn_t0);
  /* p01.LYFE*/ wire _LYFE_LCD_RSTn_t0 = not1(_LYHA_VID_RSTp_t0);
  /* p01.PYRY*/ wire _PYRY_VID_RSTp_t0 = not1(_XAPO_VID_RSTn_t0);
  /* p01.ROSY*/ wire _ROSY_VID_RSTp_t0 = not1(_XAPO_VID_RSTn_t0);
  /* p01.AMYG*/ wire _AMYG_VID_RSTp_t0 = not1(_XAPO_VID_RSTn_t0);
  /* p01.TOFU*/ wire _TOFU_VID_RSTp_t0 = not1(_XAPO_VID_RSTn_t0);

  //----------------------------------------
  // Video clock signals

  /* p29.WOSU*/ clk_reg.WOSU_AxxDExxHp.dff17(_XYFY_xBxDxFxH_t0,                 _XAPO_VID_RSTn_t0, clk_reg.WUVU_ABxxEFxxp.qn16_old());
  /* p29.WUVU*/ clk_reg.WUVU_ABxxEFxxp.dff17(_XOTA_AxCxExGx_t0,                 _XAPO_VID_RSTn_t0, clk_reg.WUVU_ABxxEFxxp.qn16_old());
  /* p21.VENA*/ clk_reg.VENA_xxCDEFxxp.dff17(clk_reg.WUVU_ABxxEFxxp.qn16_new(), _XAPO_VID_RSTn_t0, clk_reg.VENA_xxCDEFxxp.qn16_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird

  /*#p29.XOCE*/ wire _XOCE_xBCxxFGx_t1 = not1(clk_reg.WOSU_AxxDExxHp.qp17_new());
  /*#p29.XUPY*/ wire _XUPY_ABxxEFxx_t1 = not1(clk_reg.WUVU_ABxxEFxxp.qn16_new());
  /*#p21.TALU*/ wire _TALU_xxCDEFxx_t1 = not1(clk_reg.VENA_xxCDEFxxp.qn16_new());
  /*#p21.SONO*/ wire _SONO_ABxxxxGH_t1 = not1(_TALU_xxCDEFxx_t1);

  //----------------------------------------
  // CPU read signals

  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_t0 = nor2(_TEXO_8000_9FFFn_t0, _UMUT_MODE_DBG1p_t0);
  /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_t0 = not1(_TEXO_8000_9FFFn_t0);
  /* p08.LAGU*/ wire _LAGU_t0 = and_or3(PIN_CPU_RDp_t0.qp(), _LEVO_ADDR_INT_OR_ADDR_VRAM_t0, PIN_CPU_WRp_t0.qp());
  /* p08.LYWE*/ wire _LYWE_t0 = not1(_LAGU_t0);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_t0 = or2(_MOCA_DBG_EXT_RD_t0, _LYWE_t0);

  // Ignoring debug stuff for now
  /* p07.UJYV*/ wire _UJYV_CPU_RDn_t0 = mux2n(_UNOR_MODE_DBG2p_t0, /*PIN_EXT_RDn.qn()*/ 0, PIN_CPU_RDp_t0.qp());
  /* p07.TEDO*/ wire _TEDO_CPU_RDp_t0 = not1(_UJYV_CPU_RDn_t0);
  /* p07.AJAS*/ wire _AJAS_CPU_RDn_t0 = not1(_TEDO_CPU_RDp_t0);
  /* p07.ASOT*/ wire _ASOT_CPU_RDp_t0 = not1(_AJAS_CPU_RDn_t0);
  /* p08.REDU*/ wire _REDU_CPU_RDn_t0 = not1(_TEDO_CPU_RDp_t0);

  /* p10.ACAT*/ wire _ACAT_FF00_RDp_t0 =  and4(_TEDO_CPU_RDp_t0, _ANAP_FF_0xx00000_t0,  _AKUG_A06n_t0,         _BYKO_A05n_t0);
  /* p06.UFEG*/ wire _UFEG_FF01_RDp_t0 =  and4(_TEDO_CPU_RDp_t0, _SANO_FF00_FF03p_t0,    BUS_CPU_A_t0[ 0],     _TOLA_A01n_t0);
  /* p06.UCOM*/ wire _UCOM_FF02_RDp_t0 =  and4(_TEDO_CPU_RDp_t0, _SANO_FF00_FF03p_t0,    _TOVY_A00n_t0,        BUS_CPU_A_t0[ 1]);
  /* p01.TAGY*/ wire _TAGY_FF04_RDp_t0 =  and4(_TEDO_CPU_RDp_t0, _RYFO_FF04_FF07p_t0,   _TOLA_A01n_t0,         _TOVY_A00n_t0);
  /*#p03.TEDA*/ wire _TEDA_FF05_RDp_t0 =  and4(_TEDO_CPU_RDp_t0, _RYFO_FF04_FF07p_t0,   _TOLA_A01n_t0,         BUS_CPU_A_t0[ 0]);
  /* p03.TUBY*/ wire _TUBY_FF06_RDp_t0 =  and4(_TEDO_CPU_RDp_t0, _RYFO_FF04_FF07p_t0,   BUS_CPU_A_t0[ 1],      _TOVY_A00n_t0);
  /* p03.SORA*/ wire _SORA_FF07_RDp_t0 =  and4(_TEDO_CPU_RDp_t0, _RYFO_FF04_FF07p_t0,   BUS_CPU_A_t0[ 1],      BUS_CPU_A_t0[ 0]);
  /* p07.ROLO*/ wire _ROLO_FF0F_RDn_t0 = nand4(_TEDO_CPU_RDp_t0, _SYKE_FF00_FFFFp_t0,   _SEMY_XX_0000xxxxp_t0, _SAPA_XX_xxxx1111p_t0); // schematic wrong, is NAND
  /* p07.TEXE*/ wire _TEXE_FF50_RDp_t0 =  and4(_TEDO_CPU_RDp_t0, _SYKE_FF00_FFFFp_t0,   _TYFO_XX_0x0x0000p_t0, _TUFA_XX_x1x1xxxxp_t0);
  /* p23.VYRE*/ wire _VYRE_FF40_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _VOCA_FF40p_t0);
  /* p21.TOBE*/ wire _TOBE_FF41_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _VARY_FF41p_t0);
  /* p23.ANYP*/ wire _ANYP_FF42_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _XARO_FF42p_t0);
  /* p23.AVOG*/ wire _AVOG_FF43_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _XAVY_FF43p_t0);
  /* p23.WAFU*/ wire _WAFU_FF44_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _XOGY_FF44p_t0);
  /* p23.XYLY*/ wire _XYLY_FF45_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _XAYU_FF45p_t0);
  /*#p04.MOLU*/ wire _MOLU_FF46_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _XEDA_FF46p_t0);
  /* p36.VUSO*/ wire _VUSO_FF47_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _WERA_FF47p_t0);
  /* p36.XUFY*/ wire _XUFY_FF48_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _XAYO_FF48p_t0);
  /* p36.MUMY*/ wire _MUMY_FF49_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _TEGO_FF49p_t0);
  /* p23.WAXU*/ wire _WAXU_FF4A_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _VYGA_FF4Ap_t0);
  /* p23.WYZE*/ wire _WYZE_FF4B_RDp_t0 =  and2(_ASOT_CPU_RDp_t0, _VUMY_FF4Bp_t0);

  /* p05.BYZO*/ wire _BYZO_FF00_RDn_t0 = not1(_ACAT_FF00_RDp_t0);
  /* p02.POLA*/ wire _POLA_FF0F_RDp_t0 = not1(_ROLO_FF0F_RDn_t0);
  /* p23.WYCE*/ wire _WYCE_FF40_RDn_t0 = not1(_VYRE_FF40_RDp_t0);
  /* p21.VAVE*/ wire _VAVE_FF41_RDn_t0 = not1(_TOBE_FF41_RDp_t0);
  /* p23.BUWY*/ wire _BUWY_FF42_RDn_t0 = not1(_ANYP_FF42_RDp_t0);
  /* p23.BEBA*/ wire _BEBA_FF43_RDn_t0 = not1(_AVOG_FF43_RDp_t0);
  /* p23.VARO*/ wire _VARO_FF44_RDn_t0 = not1(_WAFU_FF44_RDp_t0);
  /* p23.WEKU*/ wire _WEKU_FF45_RDn_t0 = not1(_XYLY_FF45_RDp_t0);
  /*#p04.NYGO*/ wire _NYGO_FF46_RDn_t0 = not1(_MOLU_FF46_RDp_t0);
  /* p36.TEPY*/ wire _TEPY_FF47_RDn_t0 = not1(_VUSO_FF47_RDp_t0);
  /* p36.XOZY*/ wire _XOZY_FF48_RDn_t0 = not1(_XUFY_FF48_RDp_t0);
  /* p36.LOTE*/ wire _LOTE_FF49_RDn_t0 = not1(_MUMY_FF49_RDp_t0);
  /* p23.VOMY*/ wire _VOMY_FF4A_RDn_t0 = not1(_WAXU_FF4A_RDp_t0);
  /* p23.VYCU*/ wire _VYCU_FF4B_RDn_t0 = not1(_WYZE_FF4B_RDp_t0);

  /*#p04.PUSY*/ wire _PUSY_FF46_RDp_t0 = not1(_NYGO_FF46_RDn_t0);

  wire FFFF_HIT = cpu_bus_addr == 0xFFFF;
  wire FFFF_RDn = nand2(_TEDO_CPU_RDp_t0, FFFF_HIT);

  //----------------------------------------
  // CPU write signals

  /* p01.AREV*/ wire _AREV_CPU_WRn_ABCDxxxH_t1 = nand2(PIN_CPU_WRp_t0.qp(), _AFAS_xxxxEFGx_t1);
  /* p01.APOV*/ wire _APOV_CPU_WRp_xxxxEFGx_t1 = not1(_AREV_CPU_WRn_ABCDxxxH_t1);
  /* p07.UBAL*/ wire _UBAL_CPU_WRn_ABCDxxxH_t1 = mux2n(_UNOR_MODE_DBG2p_t0, /*PIN_EXT_WRn.qn()*/ 0, _APOV_CPU_WRp_xxxxEFGx_t1); // Ignoring debug stuff for now
  /* p07.TAPU*/ wire _TAPU_CPU_WRp_xxxxEFGx_t1 = not1(_UBAL_CPU_WRn_ABCDxxxH_t1);
  /* p07.DYKY*/ wire _DYKY_CPU_WRn_ABCDxxxH_t1 = not1(_TAPU_CPU_WRp_xxxxEFGx_t1);
  /* p07.CUPA*/ wire _CUPA_CPU_WRp_xxxxEFGx_t1 = not1(_DYKY_CPU_WRn_ABCDxxxH_t1);

  /* p10.ATOZ*/ wire _ATOZ_FF00_WRn_t1 = nand4(_TAPU_CPU_WRp_xxxxEFGx_t1, _ANAP_FF_0xx00000_t0, _AKUG_A06n_t0, _BYKO_A05n_t0);
  /* p01.TAPE*/ wire _TAPE_FF04_WRp_t1 =  and4(_TAPU_CPU_WRp_xxxxEFGx_t1, _RYFO_FF04_FF07p_t0,  _TOLA_A01n_t0, _TOVY_A00n_t0);
  /*#p03.TOPE*/ wire _TOPE_FF05_WRn_t1 = nand4(_TAPU_CPU_WRp_xxxxEFGx_t1, _RYFO_FF04_FF07p_t0,  BUS_CPU_A_t0[ 0], _TOLA_A01n_t0);
  /* p03.TYJU*/ wire _TYJU_FF06_WRn_t1 = nand4(_TAPU_CPU_WRp_xxxxEFGx_t1, _RYFO_FF04_FF07p_t0,  BUS_CPU_A_t0[ 1], _TOVY_A00n_t0);
  /* p03.SARA*/ wire _SARA_FF07_WRn_t1 = nand4(_TAPU_CPU_WRp_xxxxEFGx_t1, _RYFO_FF04_FF07p_t0,  BUS_CPU_A_t0[ 1], BUS_CPU_A_t0[ 0]);
  /* p07.REFA*/ wire _REFA_FF0F_WRn_t1 = nand4(_TAPU_CPU_WRp_xxxxEFGx_t1, _SYKE_FF00_FFFFp_t0,  _SEMY_XX_0000xxxxp_t0, _SAPA_XX_xxxx1111p_t0); // schematic wrong, is NAND

  /* p23.WARU*/ wire _WARU_FF40_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _VOCA_FF40p_t0);
  /* p21.SEPA*/ wire _SEPA_FF41_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _VARY_FF41p_t0);
  /* p23.BEDY*/ wire _BEDY_FF42_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _XARO_FF42p_t0);
  /* p23.ARUR*/ wire _ARUR_FF43_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _XAVY_FF43p_t0);
  /* p23.XUFA*/ wire _XUFA_FF45_WRn_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _XAYU_FF45p_t0);
  /*#p04.LAVY*/ wire _LAVY_FF46_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _XEDA_FF46p_t0);
  /* p36.VELY*/ wire _VELY_FF47_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _WERA_FF47p_t0);
  /* p36.XOMA*/ wire _XOMA_FF48_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _XAYO_FF48p_t0);
  /* p36.MYXE*/ wire _MYXE_FF49_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _TEGO_FF49p_t0);
  /* p23.WEKO*/ wire _WEKO_FF4A_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _VYGA_FF4Ap_t0);
  /* p23.WUZA*/ wire _WUZA_FF4B_WRp_t1 = and2(_CUPA_CPU_WRp_xxxxEFGx_t1, _VUMY_FF4Bp_t0);

  /* p02.ROTU*/ wire _ROTU_FF0F_WRp_t1 = not1(_REFA_FF0F_WRn_t1);
  /* p23.XUBO*/ wire _XUBO_FF40_WRn_t1 = not1(_WARU_FF40_WRp_t1);
  /* p21.RYVE*/ wire _RYVE_FF41_WRn_t1 = not1(_SEPA_FF41_WRp_t1);
  /* p21.RYJU*/ wire _RYJU_FF41_WRn_t1 = not1(_SEPA_FF41_WRp_t1);
  /* p23.CAVO*/ wire _CAVO_FF42_WRn_t1 = not1(_BEDY_FF42_WRp_t1);
  /* p23.AMUN*/ wire _AMUN_FF43_WRn_t1 = not1(_ARUR_FF43_WRp_t1);
  /* p23.WANE*/ wire _WANE_FF45_WRp_t1 = not1(_XUFA_FF45_WRn_t1);
  /*#p04.LORU*/ wire _LORU_FF46_WRn_t1 = not1(_LAVY_FF46_WRp_t1);
  /* p36.TEPO*/ wire _TEPO_FF47_WRp_t1 = not1(_VELY_FF47_WRp_t1);
  /* p36.XELO*/ wire _XELO_FF48_WRn_t1 = not1(_XOMA_FF48_WRp_t1);
  /* p36.LEHO*/ wire _LEHO_FF49_WRn_t1 = not1(_MYXE_FF49_WRp_t1);
  /* p23.VEFU*/ wire _VEFU_FF4A_WRn_t1 = not1(_WEKO_FF4A_WRp_t1);
  /* p23.VOXU*/ wire _VOXU_FF4B_WRn_t1 = not1(_WUZA_FF4B_WRp_t1);

  /* p06.SANO*/ wire _SANO_ADDR_FF00_FF03 = and3(_SARE_XX00_XX07p_t0, _SEFY_A02n_t0, _SYKE_FF00_FFFFp_t0);
  /* p06.URYS*/ wire _URYS_FF01_WRn = nand4(_TAPU_CPU_WRp_xxxxEFGx_t1, _SANO_ADDR_FF00_FF03, BUS_CPU_A_t0[ 0], _TOLA_A01n_t0);
  /* p06.DAKU*/ wire _DAKU_FF01_WRp = not1(_URYS_FF01_WRn);
  /* p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(_TAPU_CPU_WRp_xxxxEFGx_t1, _SANO_ADDR_FF00_FF03, _TOVY_A00n_t0, BUS_CPU_A_t0[ 1]);

  //----------------------------------------

  {
    // FF50 - disable bootrom bit
    /* p07.TUGE*/ wire _TUGE_FF50_WRn = nand4(_TAPU_CPU_WRp_xxxxEFGx_t1, _SYKE_FF00_FFFFp_t0, _TYFO_XX_0x0x0000p_t0, _TUFA_XX_x1x1xxxxp_t0);
    /* p07.SATO*/ wire _SATO_BOOT_BIT_IN = or2(BUS_CPU_Dp_in[0].to_wire_new(), BOOT_BITn.qp17_old());
    /* p07.TEPU*/ BOOT_BITn.dff17(_TUGE_FF50_WRn, _ALUR_SYS_RSTn_t0, _SATO_BOOT_BIT_IN);
  }

  /* p07.TERA*/ wire _TERA_BOOT_BITp_t0  = not1(BOOT_BITn.qp17_new());
  /* p07.TUTU*/ wire _TUTU_ADDR_BOOTp_t0 = and2(_TERA_BOOT_BITp_t0, _TULO_00000000_XXp_t0);
  /* p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1(_UMUT_MODE_DBG1p_t0);
  /* p07.YULA*/ wire _YULA_BOOT_RDp   = and3(_TEDO_CPU_RDp_t0, _YAZA_MODE_DBG1n, _TUTU_ADDR_BOOTp_t0); // def AND
  /* p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2(_YULA_BOOT_RDp, _ZUFA_0000_00FFp_t0);
  /* p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1(_ZADO_BOOT_CSn);

  {
    Pin2 PIN_CPU_BOOTp_t0;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
    PIN_CPU_BOOTp_t0.pin_in(1, _TUTU_ADDR_BOOTp_t0);
  }

  //----------------------------------------

#pragma region Timer
  {
    /*#p03.MERY*/ wire _MERY_TIMER_OVERFLOWp = nor2(tim_reg.NUGA_TIMA7p.qp01_old(), tim_reg.NYDU_TIMA7p_DELAY.qn16_old());
    /*#p03.MOBA*/ tim_reg.MOBA_TIMER_OVERFLOWp.dff17(_BOGA_Axxxxxxx_t1, _ALUR_SYS_RSTn_t0, _MERY_TIMER_OVERFLOWp);

    /*#p03.MUZU*/ wire _MUZU_CPU_LOAD_TIMAn  = or2(PIN_CPU_LATCH_EXT_t0.qp(), _TOPE_FF05_WRn_t1);
    /*#p03.MEKE*/ wire _MEKE_TIMER_OVERFLOWn = not1(tim_reg.MOBA_TIMER_OVERFLOWp.qp17_new());
    /*#p03.MEXU*/ wire _MEXU_TIMA_LOADp      = nand3(_MUZU_CPU_LOAD_TIMAn, _ALUR_SYS_RSTn_t0, _MEKE_TIMER_OVERFLOWn);
    /*#p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn = not1(_MEXU_TIMA_LOADp);
    /*#p03.NYDU*/ tim_reg.NYDU_TIMA7p_DELAY.dff17(_BOGA_Axxxxxxx_t1, _MUGY_TIMA_MAX_RSTn, tim_reg.NUGA_TIMA7p.qp01_old());

    // FF04 DIV
    /* p01.UFOL*/ wire _UFOL_DIV_RSTn = nor3(_UCOB_CLKBADp_t0, sys_rst, _TAPE_FF04_WRp_t1);
    /* p01.UKUP*/ tim_reg.UKUP_DIV00p.dff17(_BOGA_Axxxxxxx_t1,              _UFOL_DIV_RSTn, tim_reg.UKUP_DIV00p.qn16_old());
    /* p01.UFOR*/ tim_reg.UFOR_DIV01p.dff17(tim_reg.UKUP_DIV00p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.UFOR_DIV01p.qn16_old());
    /* p01.UNER*/ tim_reg.UNER_DIV02p.dff17(tim_reg.UFOR_DIV01p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.UNER_DIV02p.qn16_old());
    /*#p01.TERO*/ tim_reg.TERO_DIV03p.dff17(tim_reg.UNER_DIV02p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.TERO_DIV03p.qn16_old());
    /* p01.UNYK*/ tim_reg.UNYK_DIV04p.dff17(tim_reg.TERO_DIV03p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.UNYK_DIV04p.qn16_old());
    /* p01.TAMA*/ tim_reg.TAMA_DIV05p.dff17(tim_reg.UNYK_DIV04p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.TAMA_DIV05p.qn16_old());
    /* p01.ULUR*/ wire _ULUR_DIV_06_CLK = /*mux2p(FF60_1, _BOGA_Axxxxxxx_t1,*/ tim_reg.TAMA_DIV05p.qn16_new() /*)*/; // this is hacked up because we're ignoring the debug reg for the moment
    /* p01.UGOT*/ tim_reg.UGOT_DIV06p.dff17(_ULUR_DIV_06_CLK,               _UFOL_DIV_RSTn, tim_reg.UGOT_DIV06p.qn16_old());
    /* p01.TULU*/ tim_reg.TULU_DIV07p.dff17(tim_reg.UGOT_DIV06p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.TULU_DIV07p.qn16_old());
    /* p01.TUGO*/ tim_reg.TUGO_DIV08p.dff17(tim_reg.TULU_DIV07p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.TUGO_DIV08p.qn16_old());
    /* p01.TOFE*/ tim_reg.TOFE_DIV09p.dff17(tim_reg.TUGO_DIV08p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.TOFE_DIV09p.qn16_old());
    /* p01.TERU*/ tim_reg.TERU_DIV10p.dff17(tim_reg.TOFE_DIV09p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.TERU_DIV10p.qn16_old());
    /* p01.SOLA*/ tim_reg.SOLA_DIV11p.dff17(tim_reg.TERU_DIV10p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.SOLA_DIV11p.qn16_old());
    /* p01.SUBU*/ tim_reg.SUBU_DIV12p.dff17(tim_reg.SOLA_DIV11p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.SUBU_DIV12p.qn16_old());
    /* p01.TEKA*/ tim_reg.TEKA_DIV13p.dff17(tim_reg.SUBU_DIV12p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.TEKA_DIV13p.qn16_old());
    /* p01.UKET*/ tim_reg.UKET_DIV14p.dff17(tim_reg.TEKA_DIV13p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.UKET_DIV14p.qn16_old());
    /* p01.UPOF*/ tim_reg.UPOF_DIV15p.dff17(tim_reg.UKET_DIV14p.qn16_new(), _UFOL_DIV_RSTn, tim_reg.UPOF_DIV15p.qn16_old());

    // FF06 TMA
    /* p03.SABU*/ tim_reg.SABU_TMA0p.dff17(_TYJU_FF06_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[0].to_wire_new());
    /* p03.NYKE*/ tim_reg.NYKE_TMA1p.dff17(_TYJU_FF06_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p03.MURU*/ tim_reg.MURU_TMA2p.dff17(_TYJU_FF06_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[2].to_wire_new());
    /* p03.TYVA*/ tim_reg.TYVA_TMA3p.dff17(_TYJU_FF06_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[3].to_wire_new());
    /* p03.TYRU*/ tim_reg.TYRU_TMA4p.dff17(_TYJU_FF06_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[4].to_wire_new());
    /* p03.SUFY*/ tim_reg.SUFY_TMA5p.dff17(_TYJU_FF06_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[5].to_wire_new());
    /* p03.PETO*/ tim_reg.PETO_TMA6p.dff17(_TYJU_FF06_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[6].to_wire_new());
    /* p03.SETA*/ tim_reg.SETA_TMA7p.dff17(_TYJU_FF06_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[7].to_wire_new());

    // FF07 TAC
    /* p03.SOPU*/ tim_reg.SOPU_TAC0p.dff17(_SARA_FF07_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[0].to_wire_new());
    /* p03.SAMY*/ tim_reg.SAMY_TAC1p.dff17(_SARA_FF07_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p03.SABO*/ tim_reg.SABO_TAC2p.dff17(_SARA_FF07_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[2].to_wire_new());

    /*#p03.ROKE*/ wire _ROKE_TIMA_D0 = mux2n(_TOPE_FF05_WRn_t1, tim_reg.SABU_TMA0p.qp17_new(), BUS_CPU_Dp_in[0].to_wire_new());
    /*#p03.PETU*/ wire _PETU_TIMA_D1 = mux2n(_TOPE_FF05_WRn_t1, tim_reg.NYKE_TMA1p.qp17_new(), BUS_CPU_Dp_in[1].to_wire_new());
    /*#p03.NYKU*/ wire _NYKU_TIMA_D2 = mux2n(_TOPE_FF05_WRn_t1, tim_reg.MURU_TMA2p.qp17_new(), BUS_CPU_Dp_in[2].to_wire_new());
    /*#p03.SOCE*/ wire _SOCE_TIMA_D3 = mux2n(_TOPE_FF05_WRn_t1, tim_reg.TYVA_TMA3p.qp17_new(), BUS_CPU_Dp_in[3].to_wire_new());
    /*#p03.SALA*/ wire _SALA_TIMA_D4 = mux2n(_TOPE_FF05_WRn_t1, tim_reg.TYRU_TMA4p.qp17_new(), BUS_CPU_Dp_in[4].to_wire_new());
    /*#p03.SYRU*/ wire _SYRU_TIMA_D5 = mux2n(_TOPE_FF05_WRn_t1, tim_reg.SUFY_TMA5p.qp17_new(), BUS_CPU_Dp_in[5].to_wire_new());
    /*#p03.REFU*/ wire _REFU_TIMA_D6 = mux2n(_TOPE_FF05_WRn_t1, tim_reg.PETO_TMA6p.qp17_new(), BUS_CPU_Dp_in[6].to_wire_new());
    /*#p03.RATO*/ wire _RATO_TIMA_D7 = mux2n(_TOPE_FF05_WRn_t1, tim_reg.SETA_TMA7p.qp17_new(), BUS_CPU_Dp_in[7].to_wire_new());

    /*#p03.PUXY*/ wire _PUXY_TIMA_D0 = nor2(_MULO_SYS_RSTn_t0, _ROKE_TIMA_D0);
    /*#p03.NERO*/ wire _NERO_TIMA_D1 = nor2(_MULO_SYS_RSTn_t0, _PETU_TIMA_D1);
    /*#p03.NADA*/ wire _NADA_TIMA_D2 = nor2(_MULO_SYS_RSTn_t0, _NYKU_TIMA_D2);
    /*#p03.REPA*/ wire _REPA_TIMA_D3 = nor2(_MULO_SYS_RSTn_t0, _SOCE_TIMA_D3);
    /*#p03.ROLU*/ wire _ROLU_TIMA_D4 = nor2(_MULO_SYS_RSTn_t0, _SALA_TIMA_D4);
    /*#p03.RUGY*/ wire _RUGY_TIMA_D5 = nor2(_MULO_SYS_RSTn_t0, _SYRU_TIMA_D5);
    /*#p03.PYMA*/ wire _PYMA_TIMA_D6 = nor2(_MULO_SYS_RSTn_t0, _REFU_TIMA_D6);
    /*#p03.PAGU*/ wire _PAGU_TIMA_D7 = nor2(_MULO_SYS_RSTn_t0, _RATO_TIMA_D7);

    // FF05 TIMA
    /*#p03.UBOT*/ wire _UBOT_DIV01n_t0 = not1(tim_reg.UFOR_DIV01p.qp17_new());
    /*#p03.UVYR*/ wire _UVYR_DIV03n_t0 = not1(tim_reg.TERO_DIV03p.qp17_new());
    /* p01.UVYN*/ wire _UVYN_DIV05n_t0 = not1(tim_reg.TAMA_DIV05p.qp17_new());
    /* p01.UREK*/ wire _UREK_DIV07n_t0 = not1(tim_reg.TULU_DIV07p.qp17_new());

    /*#p03.UKAP*/ wire _UKAP_CLK_MUXa = mux2n(tim_reg.SOPU_TAC0p.qp17_new(), _UVYN_DIV05n_t0, _UVYR_DIV03n_t0);
    /*#p03.TEKO*/ wire _TEKO_CLK_MUXb = mux2n(tim_reg.SOPU_TAC0p.qp17_new(), _UBOT_DIV01n_t0, _UREK_DIV07n_t0);
    /*#p03.TECY*/ wire _TECY_CLK_MUXc = mux2n(tim_reg.SAMY_TAC1p.qp17_new(), _UKAP_CLK_MUXa, _TEKO_CLK_MUXb);
    /*#p03.SOGU*/ wire _SOGU_TIMA_CLKn = nor2(_TECY_CLK_MUXc, tim_reg.SABO_TAC2p.qn16_new());
    /*#p03.REGA*/ tim_reg.REGA_TIMA0p.dff20(_SOGU_TIMA_CLKn,                _MEXU_TIMA_LOADp, _PUXY_TIMA_D0);
    /*#p03.POVY*/ tim_reg.POVY_TIMA1p.dff20(tim_reg.REGA_TIMA0p.qp01_new(), _MEXU_TIMA_LOADp, _NERO_TIMA_D1);
    /*#p03.PERU*/ tim_reg.PERU_TIMA2p.dff20(tim_reg.POVY_TIMA1p.qp01_new(), _MEXU_TIMA_LOADp, _NADA_TIMA_D2);
    /*#p03.RATE*/ tim_reg.RATE_TIMA3p.dff20(tim_reg.PERU_TIMA2p.qp01_new(), _MEXU_TIMA_LOADp, _REPA_TIMA_D3);
    /*#p03.RUBY*/ tim_reg.RUBY_TIMA4p.dff20(tim_reg.RATE_TIMA3p.qp01_new(), _MEXU_TIMA_LOADp, _ROLU_TIMA_D4);
    /*#p03.RAGE*/ tim_reg.RAGE_TIMA5p.dff20(tim_reg.RUBY_TIMA4p.qp01_new(), _MEXU_TIMA_LOADp, _RUGY_TIMA_D5);
    /*#p03.PEDA*/ tim_reg.PEDA_TIMA6p.dff20(tim_reg.RAGE_TIMA5p.qp01_new(), _MEXU_TIMA_LOADp, _PYMA_TIMA_D6);
    /*#p03.NUGA*/ tim_reg.NUGA_TIMA7p.dff20(tim_reg.PEDA_TIMA6p.qp01_new(), _MEXU_TIMA_LOADp, _PAGU_TIMA_D7);
  }
#pragma endregion

#pragma region DMA_Registers
  {
    /*#p04.NAFA*/ dma_reg.NAFA_DMA_A08n.dff8p(_LORU_FF46_WRn_t1, BUS_CPU_Dp_in[0].to_wire_new());
    /* p04.PYNE*/ dma_reg.PYNE_DMA_A09n.dff8p(_LORU_FF46_WRn_t1, BUS_CPU_Dp_in[1].to_wire_new());
    /* p04.PARA*/ dma_reg.PARA_DMA_A10n.dff8p(_LORU_FF46_WRn_t1, BUS_CPU_Dp_in[2].to_wire_new());
    /* p04.NYDO*/ dma_reg.NYDO_DMA_A11n.dff8p(_LORU_FF46_WRn_t1, BUS_CPU_Dp_in[3].to_wire_new());
    /* p04.NYGY*/ dma_reg.NYGY_DMA_A12n.dff8p(_LORU_FF46_WRn_t1, BUS_CPU_Dp_in[4].to_wire_new());
    /* p04.PULA*/ dma_reg.PULA_DMA_A13n.dff8p(_LORU_FF46_WRn_t1, BUS_CPU_Dp_in[5].to_wire_new());
    /* p04.POKU*/ dma_reg.POKU_DMA_A14n.dff8p(_LORU_FF46_WRn_t1, BUS_CPU_Dp_in[6].to_wire_new());
    /* p04.MARU*/ dma_reg.MARU_DMA_A15n.dff8p(_LORU_FF46_WRn_t1, BUS_CPU_Dp_in[7].to_wire_new());

    /*#p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(dma_reg.LENE_DMA_TRIG_d4.qn16_old(), _CUNU_SYS_RSTn_t0);
    /*#p04.LAPA*/ wire _LAPA_DMA_RSTn = not1(_LOKO_DMA_RSTp);

    /* p04.LARA*/ dma_reg.LARA_DMA_LATCHn = nand3(dma_reg.LOKY_DMA_LATCHp.to_wire_old(), dma_reg.MYTE_DMA_DONE.qn16_old(), _CUNU_SYS_RSTn_t0);
    /*#p04.LOKY*/ dma_reg.LOKY_DMA_LATCHp = nand2(dma_reg.LARA_DMA_LATCHn.to_wire_new(), dma_reg.LENE_DMA_TRIG_d4.qn16_old());

    /*#p04.META*/ wire _META_DMA_CLKp = and2(_UVYT_ABCDxxxx_t1, dma_reg.LOKY_DMA_LATCHp.to_wire_new());

    /*#p04.LYXE*/ dma_reg.LYXE_DMA_LATCHp.nor_latch(_LAVY_FF46_WRp_t1, _LOKO_DMA_RSTp);
    /*#p04.LUPA*/ wire _LUPA_DMA_TRIG = nor2(_LAVY_FF46_WRp_t1, dma_reg.LYXE_DMA_LATCHp.qn03_new());

    /*#p04.MATU*/ dma_reg.MATU_DMA_RUNNINGp.dff17(_UVYT_ABCDxxxx_t1, _CUNU_SYS_RSTn_t0, dma_reg.LOKY_DMA_LATCHp.to_wire_new());
    /*#p04.LENE*/ dma_reg.LENE_DMA_TRIG_d4 .dff17(_MOPA_xxxxEFGH_t1, _CUNU_SYS_RSTn_t0, dma_reg.LUVY_DMA_TRIG_d0.qp17_old());
    /*#p04.LUVY*/ dma_reg.LUVY_DMA_TRIG_d0 .dff17(_UVYT_ABCDxxxx_t1, _CUNU_SYS_RSTn_t0, _LUPA_DMA_TRIG);

    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn = nand6(dma_reg.NAKY_DMA_A00p.qp17_old(), dma_reg.PYRO_DMA_A01p.qp17_old(),  // 128+16+8+4+2+1 = 159
                                               dma_reg.NEFY_DMA_A02p.qp17_old(), dma_reg.MUTY_DMA_A03p.qp17_old(),
                                               dma_reg.NYKO_DMA_A04p.qp17_old(), dma_reg.MUGU_DMA_A07p.qp17_old());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp = not1(_NAVO_DMA_DONEn);
    /*#p04.MYTE*/ dma_reg.MYTE_DMA_DONE.dff17(_MOPA_xxxxEFGH_t1, _LAPA_DMA_RSTn, _NOLO_DMA_DONEp);

    /*#p04.NAKY*/ dma_reg.NAKY_DMA_A00p.dff17(_META_DMA_CLKp,                   _LAPA_DMA_RSTn, dma_reg.NAKY_DMA_A00p.qn16_old());
    /*#p04.PYRO*/ dma_reg.PYRO_DMA_A01p.dff17(dma_reg.NAKY_DMA_A00p.qn16_new(), _LAPA_DMA_RSTn, dma_reg.PYRO_DMA_A01p.qn16_old());
    /* p04.NEFY*/ dma_reg.NEFY_DMA_A02p.dff17(dma_reg.PYRO_DMA_A01p.qn16_new(), _LAPA_DMA_RSTn, dma_reg.NEFY_DMA_A02p.qn16_old());
    /* p04.MUTY*/ dma_reg.MUTY_DMA_A03p.dff17(dma_reg.NEFY_DMA_A02p.qn16_new(), _LAPA_DMA_RSTn, dma_reg.MUTY_DMA_A03p.qn16_old());
    /* p04.NYKO*/ dma_reg.NYKO_DMA_A04p.dff17(dma_reg.MUTY_DMA_A03p.qn16_new(), _LAPA_DMA_RSTn, dma_reg.NYKO_DMA_A04p.qn16_old());
    /* p04.PYLO*/ dma_reg.PYLO_DMA_A05p.dff17(dma_reg.NYKO_DMA_A04p.qn16_new(), _LAPA_DMA_RSTn, dma_reg.PYLO_DMA_A05p.qn16_old());
    /* p04.NUTO*/ dma_reg.NUTO_DMA_A06p.dff17(dma_reg.PYLO_DMA_A05p.qn16_new(), _LAPA_DMA_RSTn, dma_reg.NUTO_DMA_A06p.qn16_old());
    /* p04.MUGU*/ dma_reg.MUGU_DMA_A07p.dff17(dma_reg.NUTO_DMA_A06p.qn16_new(), _LAPA_DMA_RSTn, dma_reg.MUGU_DMA_A07p.qn16_old());
  }
#pragma endregion

#pragma region Regs_Write
  {
    // JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
    // That also means that _both_ P14 and P15 are selected at reset :/
    /* p05.JUTE*/ joypad.JUTE_JOYP_RA     .dff17(_ATOZ_FF00_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[0].to_wire_new());
    /* p05.KECY*/ joypad.KECY_JOYP_LB     .dff17(_ATOZ_FF00_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p05.JALE*/ joypad.JALE_JOYP_UC     .dff17(_ATOZ_FF00_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[2].to_wire_new());
    /* p05.KYME*/ joypad.KYME_JOYP_DS     .dff17(_ATOZ_FF00_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[3].to_wire_new());
    /* p05.KELY*/ joypad.KELY_JOYP_UDLRp  .dff17(_ATOZ_FF00_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[4].to_wire_new());
    /* p05.COFY*/ joypad.COFY_JOYP_ABCSp  .dff17(_ATOZ_FF00_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[5].to_wire_new());
    /* p05.KUKO*/ joypad.KUKO_DBG_FF00_D6n.dff17(_ATOZ_FF00_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[6].to_wire_new());
    /* p05.KERU*/ joypad.KERU_DBG_FF00_D7n.dff17(_ATOZ_FF00_WRn_t1, _ALUR_SYS_RSTn_t0, BUS_CPU_Dp_in[7].to_wire_new());
  }

  {
    // FF40 LCDC
    /*#p23.VYXE*/ pix_pipe.VYXE_LCDC_BGENn  .dff9(_XUBO_FF40_WRn_t1, _XARE_SYS_RSTn_t0, BUS_CPU_Dp_in[0].to_wire_new());
    /* p23.XYLO*/ pix_pipe.XYLO_LCDC_SPENn  .dff9(_XUBO_FF40_WRn_t1, _XARE_SYS_RSTn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p23.XYMO*/ pix_pipe.XYMO_LCDC_SPSIZEn.dff9(_XUBO_FF40_WRn_t1, _XARE_SYS_RSTn_t0, BUS_CPU_Dp_in[2].to_wire_new());
    /* p23.XAFO*/ pix_pipe.XAFO_LCDC_BGMAPn .dff9(_XUBO_FF40_WRn_t1, _XARE_SYS_RSTn_t0, BUS_CPU_Dp_in[3].to_wire_new());
    /* p23.WEXU*/ pix_pipe.WEXU_LCDC_BGTILEn.dff9(_XUBO_FF40_WRn_t1, _XARE_SYS_RSTn_t0, BUS_CPU_Dp_in[4].to_wire_new());
    /* p23.WYMO*/ pix_pipe.WYMO_LCDC_WINENn .dff9(_XUBO_FF40_WRn_t1, _XARE_SYS_RSTn_t0, BUS_CPU_Dp_in[5].to_wire_new());
    /* p23.WOKY*/ pix_pipe.WOKY_LCDC_WINMAPn.dff9(_XUBO_FF40_WRn_t1, _XARE_SYS_RSTn_t0, BUS_CPU_Dp_in[6].to_wire_new());
    /* p23.XONA*/ pix_pipe.XONA_LCDC_LCDENn .dff9(_XUBO_FF40_WRn_t1, _XARE_SYS_RSTn_t0, BUS_CPU_Dp_in[7].to_wire_new());
  }

  {
    // FF41 STAT
    /* p21.ROXE*/ pix_pipe.ROXE_STAT_HBI_ENn.dff9(_RYVE_FF41_WRn_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[3].to_wire_new());
    /* p21.RUFO*/ pix_pipe.RUFO_STAT_VBI_ENn.dff9(_RYVE_FF41_WRn_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[4].to_wire_new());
    /* p21.REFE*/ pix_pipe.REFE_STAT_OAI_ENn.dff9(_RYVE_FF41_WRn_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[5].to_wire_new());
    /* p21.RUGU*/ pix_pipe.RUGU_STAT_LYI_ENn.dff9(_RYVE_FF41_WRn_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[6].to_wire_new());
  }

  {
    // FF42 SCY
    /* p23.GAVE*/ pix_pipe.GAVE_SCY0n.dff9(_CAVO_FF42_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[0].to_wire_new());
    /* p23.FYMO*/ pix_pipe.FYMO_SCY1n.dff9(_CAVO_FF42_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p23.FEZU*/ pix_pipe.FEZU_SCY2n.dff9(_CAVO_FF42_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[2].to_wire_new());
    /* p23.FUJO*/ pix_pipe.FUJO_SCY3n.dff9(_CAVO_FF42_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[3].to_wire_new());
    /* p23.DEDE*/ pix_pipe.DEDE_SCY4n.dff9(_CAVO_FF42_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[4].to_wire_new());
    /* p23.FOTY*/ pix_pipe.FOTY_SCY5n.dff9(_CAVO_FF42_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[5].to_wire_new());
    /* p23.FOHA*/ pix_pipe.FOHA_SCY6n.dff9(_CAVO_FF42_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[6].to_wire_new());
    /* p23.FUNY*/ pix_pipe.FUNY_SCY7n.dff9(_CAVO_FF42_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[7].to_wire_new());
  }

  {
    // FF43 SCX
    /* p23.DATY*/ pix_pipe.DATY_SCX0n.dff9(_AMUN_FF43_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[0].to_wire_new());
    /* p23.DUZU*/ pix_pipe.DUZU_SCX1n.dff9(_AMUN_FF43_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p23.CYXU*/ pix_pipe.CYXU_SCX2n.dff9(_AMUN_FF43_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[2].to_wire_new());
    /* p23.GUBO*/ pix_pipe.GUBO_SCX3n.dff9(_AMUN_FF43_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[3].to_wire_new());
    /* p23.BEMY*/ pix_pipe.BEMY_SCX4n.dff9(_AMUN_FF43_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[4].to_wire_new());
    /* p23.CUZY*/ pix_pipe.CUZY_SCX5n.dff9(_AMUN_FF43_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[5].to_wire_new());
    /* p23.CABU*/ pix_pipe.CABU_SCX6n.dff9(_AMUN_FF43_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[6].to_wire_new());
    /* p23.BAKE*/ pix_pipe.BAKE_SCX7n.dff9(_AMUN_FF43_WRn_t1, _CUNU_SYS_RSTn_t0, BUS_CPU_Dp_in[7].to_wire_new());
  }

  {
    // FF45 LYC
    /* p23.SYRY*/ lcd_reg.SYRY_LYC0n.dff9(_WANE_FF45_WRp_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[0].to_wire_new());
    /* p23.VUCE*/ lcd_reg.VUCE_LYC1n.dff9(_WANE_FF45_WRp_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p23.SEDY*/ lcd_reg.SEDY_LYC2n.dff9(_WANE_FF45_WRp_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[2].to_wire_new());
    /* p23.SALO*/ lcd_reg.SALO_LYC3n.dff9(_WANE_FF45_WRp_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[3].to_wire_new());
    /* p23.SOTA*/ lcd_reg.SOTA_LYC4n.dff9(_WANE_FF45_WRp_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[4].to_wire_new());
    /* p23.VAFA*/ lcd_reg.VAFA_LYC5n.dff9(_WANE_FF45_WRp_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[5].to_wire_new());
    /* p23.VEVO*/ lcd_reg.VEVO_LYC6n.dff9(_WANE_FF45_WRp_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[6].to_wire_new());
    /* p23.RAHA*/ lcd_reg.RAHA_LYC7n.dff9(_WANE_FF45_WRp_t1, _WESY_SYS_RSTn_t0, BUS_CPU_Dp_in[7].to_wire_new());
  }

  {
    // FF47 BGP
    /*#p36.PAVO*/ pix_pipe.PAVO_BGP_D0n.dff8p(_TEPO_FF47_WRp_t1, BUS_CPU_Dp_in[0].to_wire_new());
    /* p36.NUSY*/ pix_pipe.NUSY_BGP_D1n.dff8p(_TEPO_FF47_WRp_t1, BUS_CPU_Dp_in[1].to_wire_new());
    /* p36.PYLU*/ pix_pipe.PYLU_BGP_D2n.dff8p(_TEPO_FF47_WRp_t1, BUS_CPU_Dp_in[2].to_wire_new());
    /* p36.MAXY*/ pix_pipe.MAXY_BGP_D3n.dff8p(_TEPO_FF47_WRp_t1, BUS_CPU_Dp_in[3].to_wire_new());
    /* p36.MUKE*/ pix_pipe.MUKE_BGP_D4n.dff8p(_TEPO_FF47_WRp_t1, BUS_CPU_Dp_in[4].to_wire_new());
    /* p36.MORU*/ pix_pipe.MORU_BGP_D5n.dff8p(_TEPO_FF47_WRp_t1, BUS_CPU_Dp_in[5].to_wire_new());
    /* p36.MOGY*/ pix_pipe.MOGY_BGP_D6n.dff8p(_TEPO_FF47_WRp_t1, BUS_CPU_Dp_in[6].to_wire_new());
    /* p36.MENA*/ pix_pipe.MENA_BGP_D7n.dff8p(_TEPO_FF47_WRp_t1, BUS_CPU_Dp_in[7].to_wire_new());
  }

  {
    // FF48 OBP0
    /* p36.XUFU*/ pix_pipe.XUFU_OBP0_D0n.dff8p(_XELO_FF48_WRn_t1, BUS_CPU_Dp_in[0].to_wire_new());
    /* p36.XUKY*/ pix_pipe.XUKY_OBP0_D1n.dff8p(_XELO_FF48_WRn_t1, BUS_CPU_Dp_in[1].to_wire_new());
    /* p36.XOVA*/ pix_pipe.XOVA_OBP0_D2n.dff8p(_XELO_FF48_WRn_t1, BUS_CPU_Dp_in[2].to_wire_new());
    /* p36.XALO*/ pix_pipe.XALO_OBP0_D3n.dff8p(_XELO_FF48_WRn_t1, BUS_CPU_Dp_in[3].to_wire_new());
    /* p36.XERU*/ pix_pipe.XERU_OBP0_D4n.dff8p(_XELO_FF48_WRn_t1, BUS_CPU_Dp_in[4].to_wire_new());
    /* p36.XYZE*/ pix_pipe.XYZE_OBP0_D5n.dff8p(_XELO_FF48_WRn_t1, BUS_CPU_Dp_in[5].to_wire_new());
    /* p36.XUPO*/ pix_pipe.XUPO_OBP0_D6n.dff8p(_XELO_FF48_WRn_t1, BUS_CPU_Dp_in[6].to_wire_new());
    /* p36.XANA*/ pix_pipe.XANA_OBP0_D7n.dff8p(_XELO_FF48_WRn_t1, BUS_CPU_Dp_in[7].to_wire_new());
  }

  {
    // FF49 OBP1
    /* p36.MOXY*/ pix_pipe.MOXY_OBP1_D0n.dff8p(_LEHO_FF49_WRn_t1, BUS_CPU_Dp_in[0].to_wire_new());
    /* p36.LAWO*/ pix_pipe.LAWO_OBP1_D1n.dff8p(_LEHO_FF49_WRn_t1, BUS_CPU_Dp_in[1].to_wire_new());
    /* p36.MOSA*/ pix_pipe.MOSA_OBP1_D2n.dff8p(_LEHO_FF49_WRn_t1, BUS_CPU_Dp_in[2].to_wire_new());
    /* p36.LOSE*/ pix_pipe.LOSE_OBP1_D3n.dff8p(_LEHO_FF49_WRn_t1, BUS_CPU_Dp_in[3].to_wire_new());
    /* p36.LUNE*/ pix_pipe.LUNE_OBP1_D4n.dff8p(_LEHO_FF49_WRn_t1, BUS_CPU_Dp_in[4].to_wire_new());
    /* p36.LUGU*/ pix_pipe.LUGU_OBP1_D5n.dff8p(_LEHO_FF49_WRn_t1, BUS_CPU_Dp_in[5].to_wire_new());
    /* p36.LEPU*/ pix_pipe.LEPU_OBP1_D6n.dff8p(_LEHO_FF49_WRn_t1, BUS_CPU_Dp_in[6].to_wire_new());
    /* p36.LUXO*/ pix_pipe.LUXO_OBP1_D7n.dff8p(_LEHO_FF49_WRn_t1, BUS_CPU_Dp_in[7].to_wire_new());
  }

  {
    // FF4A WY
    /* p23.NESO*/ pix_pipe.NESO_WY0n.dff9(_VEFU_FF4A_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[0].to_wire_new());
    /* p23.NYRO*/ pix_pipe.NYRO_WY1n.dff9(_VEFU_FF4A_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p23.NAGA*/ pix_pipe.NAGA_WY2n.dff9(_VEFU_FF4A_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[2].to_wire_new());
    /* p23.MELA*/ pix_pipe.MELA_WY3n.dff9(_VEFU_FF4A_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[3].to_wire_new());
    /* p23.NULO*/ pix_pipe.NULO_WY4n.dff9(_VEFU_FF4A_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[4].to_wire_new());
    /* p23.NENE*/ pix_pipe.NENE_WY5n.dff9(_VEFU_FF4A_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[5].to_wire_new());
    /* p23.NUKA*/ pix_pipe.NUKA_WY6n.dff9(_VEFU_FF4A_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[6].to_wire_new());
    /* p23.NAFU*/ pix_pipe.NAFU_WY7n.dff9(_VEFU_FF4A_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[7].to_wire_new());
  }

  {
    // FF4B WX
    /* p23.MYPA*/ pix_pipe.MYPA_WX0n.dff9(_VOXU_FF4B_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[0].to_wire_new());
    /* p23.NOFE*/ pix_pipe.NOFE_WX1n.dff9(_VOXU_FF4B_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p23.NOKE*/ pix_pipe.NOKE_WX2n.dff9(_VOXU_FF4B_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[2].to_wire_new());
    /* p23.MEBY*/ pix_pipe.MEBY_WX3n.dff9(_VOXU_FF4B_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[3].to_wire_new());
    /* p23.MYPU*/ pix_pipe.MYPU_WX4n.dff9(_VOXU_FF4B_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[4].to_wire_new());
    /* p23.MYCE*/ pix_pipe.MYCE_WX5n.dff9(_VOXU_FF4B_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[5].to_wire_new());
    /* p23.MUVO*/ pix_pipe.MUVO_WX6n.dff9(_VOXU_FF4B_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[6].to_wire_new());
    /* p23.NUKU*/ pix_pipe.NUKU_WX7n.dff9(_VOXU_FF4B_WRn_t1, _WALU_SYS_RSTn_t0, BUS_CPU_Dp_in[7].to_wire_new());
  }
#pragma endregion






  {
    /*#p27.NUNY*/ wire _NUNY_WX_MATCH_TRIGp_t0 = and2(pix_pipe.PYNU_WIN_MODE_Ap.qp04_old(), pix_pipe.NOPA_WIN_MODE_Bp.qn16_old());
    /* p27.PUKU*/ pix_pipe.PUKU_WIN_HITn = nor2(_NUNY_WX_MATCH_TRIGp_t0, pix_pipe.RYDY_WIN_HITp.to_wire_old());
    /* p27.RYDY*/ pix_pipe.RYDY_WIN_HITp = nor3(pix_pipe.PUKU_WIN_HITn.to_wire_new(), tile_fetcher.PORY_FETCH_DONEp.qp17_old(), _PYRY_VID_RSTp_t0);
  }


  {
    /*#p21.SANU*/ wire _SANU_x113p_t0 = and4(lcd_reg.TYRY_LX6p.qp17_old(), lcd_reg.TAHA_LX5p.qp17_old(), lcd_reg.SUDE_LX4p.qp17_old(), lcd_reg.SAXO_LX0p.qp17_old()); // 113 = 64 + 32 + 16 + 1, schematic is wrong
    /*#p21.RUTU*/ lcd_reg.RUTU_LINE_P910p.dff17_ff(_SONO_ABxxxxGH_t1, _SANU_x113p_t0);
    /*#p21.RUTU*/ lcd_reg.RUTU_LINE_P910p.dff17_rst(_LYFE_LCD_RSTn_t0);
  }

  {
    /*#p21.PURE*/ wire _PURE_LINE_P908n_t0 = not1(lcd_reg.RUTU_LINE_P910p.qp17_new());
    /*#p21.SELA*/ wire _SELA_LINE_P908p_t0 = not1(_PURE_LINE_P908n_t0);
    /*#p21.XYVO*/ wire _XYVO_y144p_t0 = and2(lcd_reg.LOVU_LY4p.qp17_old(), lcd_reg.LAFO_LY7p.qp17_old()); // 128 + 16 = 144
    /*#p29.ALES*/ wire _ALES_y144n_t0 = not1(_XYVO_y144p_t0);
    /*#p29.ABOV*/ wire _ABOV_LINE_P908p_t0 = and2(_SELA_LINE_P908p_t0, _ALES_y144n_t0);
    /*#p29.CATU*/ lcd_reg.CATU_LINE_P000p.dff17_ff(_XUPY_ABxxEFxx_t1, _ABOV_LINE_P908p_t0);
    /*#p29.CATU*/ lcd_reg.CATU_LINE_P000p.dff17_rst(_ABEZ_VID_RSTn_t0);
  }

  {
    /*#p21.NYPE*/ lcd_reg.NYPE_LINE_P002p.dff17_ff(_TALU_xxCDEFxx_t1, lcd_reg.RUTU_LINE_P910p.qp17_new());
    /*#p21.NYPE*/ lcd_reg.NYPE_LINE_P002p.dff17_rst(_LYFE_LCD_RSTn_t0);
  }

  {
    /*#p28.AWOH*/ wire _AWOH_xxCDxxGH_t1 = not1(_XUPY_ABxxEFxx_t1);
    /*#p28.ANEL*/ lcd_reg.ANEL_LINE_P002p.dff17_ff(_AWOH_xxCDxxGH_t1, lcd_reg.CATU_LINE_P000p.qp17_new());
    /*#p28.ANEL*/ lcd_reg.ANEL_LINE_P002p.dff17_rst(_ABEZ_VID_RSTn_t0);
  }

  //----------------------------------------

  /* p28.ABAF*/ wire _ABAF_LINE_P000n_t2 = not1(lcd_reg.CATU_LINE_P000p.qp17_new());
  /*#p21.PURE*/ wire _PURE_LINE_P908n_t2 = not1(lcd_reg.RUTU_LINE_P910p.qp17_new());
  /*#p21.SELA*/ wire _SELA_LINE_P908p_t2 = not1(_PURE_LINE_P908n_t2);

  /* p28.BYHA*/ wire _BYHA_LINE_TRIGn_t2 = or_and3(lcd_reg.ANEL_LINE_P002p.qp17_new(), _ABAF_LINE_P000n_t2, _ABEZ_VID_RSTn_t0); // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.ATEJ*/ wire _ATEJ_LINE_TRIGp_t2 = not1(_BYHA_LINE_TRIGn_t2);
  /* p27.XAHY*/ wire _XAHY_LINE_TRIGn_t2 = not1(_ATEJ_LINE_TRIGp_t2);
  /*#p28.AZYB*/ wire _AZYB_LINE_TRIGn_t2 = not1(_ATEJ_LINE_TRIGp_t2);

  /* p28.ABAK*/ wire _ABAK_LINE_RSTp_t2 =  or2(_ATEJ_LINE_TRIGp_t2, _AMYG_VID_RSTp_t0);
  /*#p28.ANOM*/ wire _ANOM_LINE_RSTn_t2 = nor2(_ATEJ_LINE_TRIGp_t2, _ATAR_VID_RSTp_t0);
  /* p21.TADY*/ wire _TADY_LINE_RSTn_t2 = nor2(_ATEJ_LINE_TRIGp_t2, _TOFU_VID_RSTp_t0);

  /* p28.BYVA*/ wire _BYVA_LINE_RSTn_t2 = not1(_ABAK_LINE_RSTp_t2);
  /* p29.DYBA*/ wire _DYBA_LINE_RSTp_t2 = not1(_BYVA_LINE_RSTn_t2);
  /*#p29.BALU*/ wire _BALU_LINE_RSTp_t2 = not1(_ANOM_LINE_RSTn_t2);
  /*#p29.BAGY*/ wire _BAGY_LINE_RSTn_t2 = not1(_BALU_LINE_RSTp_t2);

  /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn_t2 = or3(sprite_scanner.DOBA_SCAN_DONE_Bp.qp17_old(), _BALU_LINE_RSTp_t2, sprite_scanner.BYBA_SCAN_DONE_Ap.qn16_old());
  /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp_t2 = not1(_BEBU_SCAN_DONE_TRIGn_t2);
  /*#p28.ASEN*/ wire _ASEN_SCAN_DONE_TRIGp_t2 = or2(_ATAR_VID_RSTp_t0, _AVAP_SCAN_DONE_TRIGp_t2);

  /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd_reg.CATU_LINE_P000p.qp17_new(), _ASEN_SCAN_DONE_TRIGp_t2);

  /*#p21.VOGA*/ pix_pipe.VOGA_HBLANKp.dff17_rst(_TADY_LINE_RSTn_t2);


  {
    /*#p27.XOFO*/ wire _XOFO_WIN_RSTp_t2 = nand3(pix_pipe.WYMO_LCDC_WINENn.qn08_new(), _XAHY_LINE_TRIGn_t2, _XAPO_VID_RSTn_t0);
    /* p27.PYNU*/ pix_pipe.PYNU_WIN_MODE_Ap.nor_latch(pix_pipe.NUNU_WX_MATCH_Bp.qp17_old(), _XOFO_WIN_RSTp_t2);
  }

  {
    /*#p21.XYVO*/ wire _XYVO_y144p_t0 = and2(lcd_reg.LOVU_LY4p.qp17_old(), lcd_reg.LAFO_LY7p.qp17_old()); // 128 + 16 = 144
    /*#p21.POPU*/ lcd_reg.POPU_VBLANKp.dff17_ff(lcd_reg.NYPE_LINE_P002p.qp17_new(), _XYVO_y144p_t0);
    /*#p21.POPU*/ lcd_reg.POPU_VBLANKp.dff17_rst(_LYFE_LCD_RSTn_t0);
  }

  {
    /*#p21.NOKO*/ wire _NOKO_y153p_t0 = and4(lcd_reg.LAFO_LY7p.qp17_old(), lcd_reg.LOVU_LY4p.qp17_old(), lcd_reg.LYDO_LY3p.qp17_old(), lcd_reg.MUWY_LY0p.qp17_old()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*#p21.MYTA*/ lcd_reg.MYTA_y153p.dff17_ff(lcd_reg.NYPE_LINE_P002p.qn16_new(), _NOKO_y153p_t0);
    /*#p21.MYTA*/ lcd_reg.MYTA_y153p.dff17_rst(_LYFE_LCD_RSTn_t0);
  }

  {
    /*#p24.NERU*/ wire _NERU_y000p_t0 = nor8(lcd_reg.LAFO_LY7p.qp17_old(), lcd_reg.LOVU_LY4p.qp17_old(), lcd_reg.LYDO_LY3p.qp17_old(), lcd_reg.MUWY_LY0p.qp17_old(),
                                             lcd_reg.MYRO_LY1p.qp17_old(), lcd_reg.LEXA_LY2p.qp17_old(), lcd_reg.LEMA_LY5p.qp17_old(), lcd_reg.MATO_LY6p.qp17_old());
    /*#p24.MEDA*/ lcd_reg.MEDA_VSYNC_OUTn.dff17_ff(lcd_reg.NYPE_LINE_P002p.qn16_new(), _NERU_y000p_t0);
    /*#p24.MEDA*/ lcd_reg.MEDA_VSYNC_OUTn.dff17_rst(_LYFE_LCD_RSTn_t0);
  }

  /*#p21.POPU*/ wire _POPU_VBLANKn_t3 = lcd_reg.POPU_VBLANKp.qn16_new();
  /*#p21.POPU*/ wire _POPU_VBLANKp_t3 = lcd_reg.POPU_VBLANKp.qp17_new();

  /*#p21.PARU*/ wire _PARU_VBLANKp_t3 = not1(_POPU_VBLANKn_t3);
  /*#p21.TOLU*/ wire _TOLU_VBLANKn_t3 = not1(_PARU_VBLANKp_t3);


#pragma region JOYPAD
  {
    wire BURO_FF60_0p_t0 = 0; // FIXME hacking out debug stuff
    /* p05.KURA*/ wire _KURA_JOYP_DBGn_t0 = not1(BURO_FF60_0p_t0);

    /*
    // lcd ribbon voltages after bootrom
    04 5 left & b
    05 0 diodes 1&2
    06 5 down & start
    07 5 up & select
    08 5 right & a
    09 0 diodes 3 & 4
    */

    /* p05.KARU*/ wire _KARU_OEp_t0 = or2(joypad.KELY_JOYP_UDLRp.qn16_new(), _KURA_JOYP_DBGn_t0);
    /* p05.CELA*/ wire _CELA_OEp_t0 = or2(joypad.COFY_JOYP_ABCSp.qn16_new(), _KURA_JOYP_DBGn_t0);

    Pin2 PIN_JOY_P14_t0; // PIN_63
    Pin2 PIN_JOY_P15_t0; // PIN_62
    PIN_JOY_P14_t0.pin_out(_KARU_OEp_t0, joypad.KELY_JOYP_UDLRp.qn16_new(), joypad.KELY_JOYP_UDLRp.qn16_new());
    PIN_JOY_P15_t0.pin_out(_CELA_OEp_t0, joypad.COFY_JOYP_ABCSp.qn16_new(), joypad.COFY_JOYP_ABCSp.qn16_new());

    // Pressing a button pulls the corresponding pin _down_.

    Pin2 PIN_JOY_P10_t0; // PIN_67
    Pin2 PIN_JOY_P11_t0; // PIN_66
    Pin2 PIN_JOY_P12_t0; // PIN_65
    Pin2 PIN_JOY_P13_t0; // PIN_64

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


    if (PIN_JOY_P14_t0.qp()) {
      PIN_JOY_P10_t0.pin_in(1, !(sys_buttons & 0x01));
      PIN_JOY_P11_t0.pin_in(1, !(sys_buttons & 0x02));
      PIN_JOY_P12_t0.pin_in(1, !(sys_buttons & 0x04));
      PIN_JOY_P13_t0.pin_in(1, !(sys_buttons & 0x08));
    }
    else if (PIN_JOY_P15_t0.qp()) {
      PIN_JOY_P10_t0.pin_in(1, !(sys_buttons & 0x10));
      PIN_JOY_P11_t0.pin_in(1, !(sys_buttons & 0x20));
      PIN_JOY_P12_t0.pin_in(1, !(sys_buttons & 0x40));
      PIN_JOY_P13_t0.pin_in(1, !(sys_buttons & 0x80));
    }
    else {
      PIN_JOY_P10_t0.pin_in(1, 1);
      PIN_JOY_P11_t0.pin_in(1, 1);
      PIN_JOY_P12_t0.pin_in(1, 1);
      PIN_JOY_P13_t0.pin_in(1, 1);
    }
    /* p02.KERY*/ wire _KERY_ANY_BUTTONp_t0 = or4(PIN_JOY_P13_t0.qn(), PIN_JOY_P12_t0.qn(), PIN_JOY_P11_t0.qn(), PIN_JOY_P10_t0.qn());

    {
      /* p02.AWOB*/ joypad.AWOB_WAKE_CPU.tp_latch(_BOGA_Axxxxxxx_t1, _KERY_ANY_BUTTONp_t0);
      wire _AWOB_WAKE_CPUp_t1 = joypad.AWOB_WAKE_CPU.qp08_new();

      Pin2 PIN_CPU_WAKE_t1; // top right wire by itself <- P02.AWOB
      PIN_CPU_WAKE_t1.pin_in(1, _AWOB_WAKE_CPUp_t1);

      /* p02.APUG*/ joypad.APUG_JP_GLITCH3.dff17(_BOGA_Axxxxxxx_t1, _ALUR_SYS_RSTn_t0, joypad.AGEM_JP_GLITCH2.qp17_old());
      /* p02.AGEM*/ joypad.AGEM_JP_GLITCH2.dff17(_BOGA_Axxxxxxx_t1, _ALUR_SYS_RSTn_t0, joypad.ACEF_JP_GLITCH1.qp17_old());
      /* p02.ACEF*/ joypad.ACEF_JP_GLITCH1.dff17(_BOGA_Axxxxxxx_t1, _ALUR_SYS_RSTn_t0, joypad.BATU_JP_GLITCH0.qp17_old());
      /* p02.BATU*/ joypad.BATU_JP_GLITCH0.dff17(_BOGA_Axxxxxxx_t1, _ALUR_SYS_RSTn_t0, _KERY_ANY_BUTTONp_t0);

      /* p05.KEVU*/ joypad.KEVU_JOYP_L0n.tp_latch(_BYZO_FF00_RDn_t0, PIN_JOY_P10_t0.qn());
      /* p05.KAPA*/ joypad.KAPA_JOYP_L1n.tp_latch(_BYZO_FF00_RDn_t0, PIN_JOY_P11_t0.qn());
      /* p05.KEJA*/ joypad.KEJA_JOYP_L2n.tp_latch(_BYZO_FF00_RDn_t0, PIN_JOY_P12_t0.qn());
      /* p05.KOLO*/ joypad.KOLO_JOYP_L3n.tp_latch(_BYZO_FF00_RDn_t0, PIN_JOY_P13_t0.qn());

      /* FF00 JOYP */
      /* p05.KEMA*/ BUS_CPU_Dp_out[0].tri6_nn(_BYZO_FF00_RDn_t0, joypad.KEVU_JOYP_L0n.qp08_new());
      /* p05.KURO*/ BUS_CPU_Dp_out[1].tri6_nn(_BYZO_FF00_RDn_t0, joypad.KAPA_JOYP_L1n.qp08_new());
      /* p05.KUVE*/ BUS_CPU_Dp_out[2].tri6_nn(_BYZO_FF00_RDn_t0, joypad.KEJA_JOYP_L2n.qp08_new());
      /* p05.JEKU*/ BUS_CPU_Dp_out[3].tri6_nn(_BYZO_FF00_RDn_t0, joypad.KOLO_JOYP_L3n.qp08_new());
      /* p05.KOCE*/ BUS_CPU_Dp_out[4].tri6_nn(_BYZO_FF00_RDn_t0, joypad.KELY_JOYP_UDLRp.qn16_new());
      /* p05.CUDY*/ BUS_CPU_Dp_out[5].tri6_nn(_BYZO_FF00_RDn_t0, joypad.COFY_JOYP_ABCSp.qn16_new());
      /* p??.????*/ BUS_CPU_Dp_out[6].tri6_nn(_BYZO_FF00_RDn_t0, joypad.KUKO_DBG_FF00_D6n.qp17_new());
      /* p??.????*/ BUS_CPU_Dp_out[7].tri6_nn(_BYZO_FF00_RDn_t0, joypad.KERU_DBG_FF00_D7n.qp17_new());
    }
  }
#pragma endregion



  {
    /*#p21.VOGA*/ wire _VOGA_HBLANKp_t0 = pix_pipe.VOGA_HBLANKp.qp17_old();
    /*#p21.WEGO*/ wire _WEGO_HBLANKp_t0 = or2(_TOFU_VID_RSTp_t0, _VOGA_HBLANKp_t0);
    /*#p21.XYMU*/ pix_pipe.XYMU_RENDERINGn.nor_latch(_WEGO_HBLANKp_t0, _AVAP_SCAN_DONE_TRIGp_t2);
  }

#pragma region SpriteStoreMatch
  bool _YDUG_STORE0_MATCHn_t0;
  bool _DYDU_STORE1_MATCHn_t0;
  bool _DEGO_STORE2_MATCHn_t0;
  bool _YLOZ_STORE3_MATCHn_t0;
  bool _XAGE_STORE4_MATCHn_t0;
  bool _EGOM_STORE5_MATCHn_t0;
  bool _YBEZ_STORE6_MATCHn_t0;
  bool _DYKA_STORE7_MATCHn_t0;
  bool _EFYL_STORE8_MATCHn_t0;
  bool _YGEM_STORE9_MATCHn_t0;
  bool _FEPO_STORE_MATCHp_t0;
  bool _WODU_HBLANKp_t0;
  {
    /*#p21.ACAM*/ wire _ACAM_PX0n_t0 = not1(pix_pipe.XEHO_PX0p.qp17_old());
    /* p21.AZUB*/ wire _AZUB_PX1n_t0 = not1(pix_pipe.SAVY_PX1p.qp17_old());
    /* p21.AMEL*/ wire _AMEL_PX2n_t0 = not1(pix_pipe.XODU_PX2p.qp17_old());
    /* p21.AHAL*/ wire _AHAL_PX3n_t0 = not1(pix_pipe.XYDO_PX3p.qp17_old());
    /* p21.APUX*/ wire _APUX_PX4n_t0 = not1(pix_pipe.TUHU_PX4p.qp17_old());
    /* p21.ABEF*/ wire _ABEF_PX5n_t0 = not1(pix_pipe.TUKY_PX5p.qp17_old());
    /* p21.ADAZ*/ wire _ADAZ_PX6n_t0 = not1(pix_pipe.TAKO_PX6p.qp17_old());
    /* p21.ASAH*/ wire _ASAH_PX7n_t0 = not1(pix_pipe.SYBE_PX7p.qp17_old());

    /*#p31.ZOGY*/ wire _ZOGY_STORE0_MATCH0n_t0 = xor2(sprite_store.XEPE_STORE0_X0p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.ZEBA*/ wire _ZEBA_STORE0_MATCH1n_t0 = xor2(sprite_store.YLAH_STORE0_X1p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.ZAHA*/ wire _ZAHA_STORE0_MATCH2n_t0 = xor2(sprite_store.ZOLA_STORE0_X2p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.ZOKY*/ wire _ZOKY_STORE0_MATCH3n_t0 = xor2(sprite_store.ZULU_STORE0_X3p.qn08_old(), _AHAL_PX3n_t0);
    /* p31.WOJU*/ wire _WOJU_STORE0_MATCH4n_t0 = xor2(sprite_store.WELO_STORE0_X4p.qn08_old(), _APUX_PX4n_t0);
    /* p31.YFUN*/ wire _YFUN_STORE0_MATCH5n_t0 = xor2(sprite_store.XUNY_STORE0_X5p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.WYZA*/ wire _WYZA_STORE0_MATCH6n_t0 = xor2(sprite_store.WOTE_STORE0_X6p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.YPUK*/ wire _YPUK_STORE0_MATCH7n_t0 = xor2(sprite_store.XAKO_STORE0_X7p.qn08_old(), _ASAH_PX7n_t0);

    /* p31.EDYM*/ wire _EDYM_STORE1_MATCH0n_t0 = xor2(sprite_store.DANY_STORE1_X0p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.EMYB*/ wire _EMYB_STORE1_MATCH1n_t0 = xor2(sprite_store.DUKO_STORE1_X1p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.EBEF*/ wire _EBEF_STORE1_MATCH2n_t0 = xor2(sprite_store.DESU_STORE1_X2p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.EWOK*/ wire _EWOK_STORE1_MATCH3n_t0 = xor2(sprite_store.DAZO_STORE1_X3p.qn08_old(), _AHAL_PX3n_t0);
    /* p31.COLA*/ wire _COLA_STORE1_MATCH4n_t0 = xor2(sprite_store.DAKE_STORE1_X4p.qn08_old(), _APUX_PX4n_t0);
    /* p31.BOBA*/ wire _BOBA_STORE1_MATCH5n_t0 = xor2(sprite_store.CESO_STORE1_X5p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.COLU*/ wire _COLU_STORE1_MATCH6n_t0 = xor2(sprite_store.DYFU_STORE1_X6p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.BAHU*/ wire _BAHU_STORE1_MATCH7n_t0 = xor2(sprite_store.CUSY_STORE1_X7p.qn08_old(), _ASAH_PX7n_t0);

    /* p31.FUZU*/ wire _FUZU_STORE2_MATCH0n_t0 = xor2(sprite_store.FOKA_STORE2_X0p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.FESO*/ wire _FESO_STORE2_MATCH1n_t0 = xor2(sprite_store.FYTY_STORE2_X1p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.FOKY*/ wire _FOKY_STORE2_MATCH2n_t0 = xor2(sprite_store.FUBY_STORE2_X2p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.FYVA*/ wire _FYVA_STORE2_MATCH3n_t0 = xor2(sprite_store.GOXU_STORE2_X3p.qn08_old(), _AHAL_PX3n_t0);
    /* p31.CEKO*/ wire _CEKO_STORE2_MATCH4n_t0 = xor2(sprite_store.DUHY_STORE2_X4p.qn08_old(), _APUX_PX4n_t0);
    /* p31.DETY*/ wire _DETY_STORE2_MATCH5n_t0 = xor2(sprite_store.EJUF_STORE2_X5p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.DOZO*/ wire _DOZO_STORE2_MATCH6n_t0 = xor2(sprite_store.ENOR_STORE2_X6p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.CONY*/ wire _CONY_STORE2_MATCH7n_t0 = xor2(sprite_store.DEPY_STORE2_X7p.qn08_old(), _ASAH_PX7n_t0);

    /* p31.YHOK*/ wire _YHOK_STORE3_MATCH0n_t0 = xor2(sprite_store.XOLY_STORE3_X0p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.YCAH*/ wire _YCAH_STORE3_MATCH1n_t0 = xor2(sprite_store.XYBA_STORE3_X1p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.YDAJ*/ wire _YDAJ_STORE3_MATCH2n_t0 = xor2(sprite_store.XABE_STORE3_X2p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.YVUZ*/ wire _YVUZ_STORE3_MATCH3n_t0 = xor2(sprite_store.XEKA_STORE3_X3p.qn08_old(), _AHAL_PX3n_t0);
    /* p31.YVAP*/ wire _YVAP_STORE3_MATCH4n_t0 = xor2(sprite_store.XOMY_STORE3_X4p.qn08_old(), _APUX_PX4n_t0);
    /* p31.XENY*/ wire _XENY_STORE3_MATCH5n_t0 = xor2(sprite_store.WUHA_STORE3_X5p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.XAVU*/ wire _XAVU_STORE3_MATCH6n_t0 = xor2(sprite_store.WYNA_STORE3_X6p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.XEVA*/ wire _XEVA_STORE3_MATCH7n_t0 = xor2(sprite_store.WECO_STORE3_X7p.qn08_old(), _ASAH_PX7n_t0);

    /* p31.XEJU*/ wire _XEJU_STORE4_MATCH0n_t0 = xor2(sprite_store.WEDU_STORE4_X0p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.ZATE*/ wire _ZATE_STORE4_MATCH1n_t0 = xor2(sprite_store.YGAJ_STORE4_X1p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.ZAKU*/ wire _ZAKU_STORE4_MATCH2n_t0 = xor2(sprite_store.ZYJO_STORE4_X2p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.YBOX*/ wire _YBOX_STORE4_MATCH3n_t0 = xor2(sprite_store.XURY_STORE4_X3p.qn08_old(), _AHAL_PX3n_t0);
    /* p31.ZYKU*/ wire _ZYKU_STORE4_MATCH4n_t0 = xor2(sprite_store.YBED_STORE4_X4p.qn08_old(), _APUX_PX4n_t0);
    /* p31.ZYPU*/ wire _ZYPU_STORE4_MATCH5n_t0 = xor2(sprite_store.ZALA_STORE4_X5p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.XAHA*/ wire _XAHA_STORE4_MATCH6n_t0 = xor2(sprite_store.WYDE_STORE4_X6p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.ZEFE*/ wire _ZEFE_STORE4_MATCH7n_t0 = xor2(sprite_store.XEPA_STORE4_X7p.qn08_old(), _ASAH_PX7n_t0);

    /* p31.GUZO*/ wire _GUZO_STORE5_MATCH0n_t0 = xor2(sprite_store.FUSA_STORE5_X0p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.GOLA*/ wire _GOLA_STORE5_MATCH1n_t0 = xor2(sprite_store.FAXA_STORE5_X1p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.GEVE*/ wire _GEVE_STORE5_MATCH2n_t0 = xor2(sprite_store.FOZY_STORE5_X2p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.GUDE*/ wire _GUDE_STORE5_MATCH3n_t0 = xor2(sprite_store.FESY_STORE5_X3p.qn08_old(), _AHAL_PX3n_t0);
    /* p31.BAZY*/ wire _BAZY_STORE5_MATCH4n_t0 = xor2(sprite_store.CYWE_STORE5_X4p.qn08_old(), _APUX_PX4n_t0);
    /* p31.CYLE*/ wire _CYLE_STORE5_MATCH5n_t0 = xor2(sprite_store.DYBY_STORE5_X5p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.CEVA*/ wire _CEVA_STORE5_MATCH6n_t0 = xor2(sprite_store.DURY_STORE5_X6p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.BUMY*/ wire _BUMY_STORE5_MATCH7n_t0 = xor2(sprite_store.CUVY_STORE5_X7p.qn08_old(), _ASAH_PX7n_t0);

    /* p31.XOSU*/ wire _XOSU_STORE6_MATCH0n_t0 = xor2(sprite_store.YCOL_STORE6_X0p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.ZUVU*/ wire _ZUVU_STORE6_MATCH1n_t0 = xor2(sprite_store.YRAC_STORE6_X1p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.XUCO*/ wire _XUCO_STORE6_MATCH2n_t0 = xor2(sprite_store.YMEM_STORE6_X2p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.ZULO*/ wire _ZULO_STORE6_MATCH3n_t0 = xor2(sprite_store.YVAG_STORE6_X3p.qn08_old(), _AHAL_PX3n_t0);
    /* p31.ZARE*/ wire _ZARE_STORE6_MATCH4n_t0 = xor2(sprite_store.ZOLY_STORE6_X4p.qn08_old(), _APUX_PX4n_t0);
    /* p31.ZEMU*/ wire _ZEMU_STORE6_MATCH5n_t0 = xor2(sprite_store.ZOGO_STORE6_X5p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.ZYGO*/ wire _ZYGO_STORE6_MATCH6n_t0 = xor2(sprite_store.ZECU_STORE6_X6p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.ZUZY*/ wire _ZUZY_STORE6_MATCH7n_t0 = xor2(sprite_store.ZESA_STORE6_X7p.qn08_old(), _ASAH_PX7n_t0);

    /* p31.DUSE*/ wire _DUSE_STORE7_MATCH0n_t0 = xor2(sprite_store.ERAZ_STORE7_X0p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.DAGU*/ wire _DAGU_STORE7_MATCH1n_t0 = xor2(sprite_store.EPUM_STORE7_X1p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.DYZE*/ wire _DYZE_STORE7_MATCH2n_t0 = xor2(sprite_store.EROL_STORE7_X2p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.DESO*/ wire _DESO_STORE7_MATCH3n_t0 = xor2(sprite_store.EHYN_STORE7_X3p.qn08_old(), _AHAL_PX3n_t0);
    /* p31.EJOT*/ wire _EJOT_STORE7_MATCH4n_t0 = xor2(sprite_store.FAZU_STORE7_X4p.qn08_old(), _APUX_PX4n_t0);
    /* p31.ESAJ*/ wire _ESAJ_STORE7_MATCH5n_t0 = xor2(sprite_store.FAXE_STORE7_X5p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.DUCU*/ wire _DUCU_STORE7_MATCH6n_t0 = xor2(sprite_store.EXUK_STORE7_X6p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.EWUD*/ wire _EWUD_STORE7_MATCH7n_t0 = xor2(sprite_store.FEDE_STORE7_X7p.qn08_old(), _ASAH_PX7n_t0);

    /* p31.DUZE*/ wire _DUZE_STORE8_MATCH0n_t0 = xor2(sprite_store.EZUF_STORE8_X0p.qn08_old(), _APUX_PX4n_t0);
    /* p31.DAGA*/ wire _DAGA_STORE8_MATCH1n_t0 = xor2(sprite_store.ENAD_STORE8_X1p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.DAWU*/ wire _DAWU_STORE8_MATCH2n_t0 = xor2(sprite_store.EBOW_STORE8_X2p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.EJAW*/ wire _EJAW_STORE8_MATCH3n_t0 = xor2(sprite_store.FYCA_STORE8_X3p.qn08_old(), _ASAH_PX7n_t0);
    /* p31.GOHO*/ wire _GOHO_STORE8_MATCH4n_t0 = xor2(sprite_store.GAVY_STORE8_X4p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.GASU*/ wire _GASU_STORE8_MATCH5n_t0 = xor2(sprite_store.GYPU_STORE8_X5p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.GABU*/ wire _GABU_STORE8_MATCH6n_t0 = xor2(sprite_store.GADY_STORE8_X6p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.GAFE*/ wire _GAFE_STORE8_MATCH7n_t0 = xor2(sprite_store.GAZA_STORE8_X7p.qn08_old(), _AHAL_PX3n_t0);

    /* p31.YMAM*/ wire _YMAM_STORE9_MATCH0n_t0 = xor2(sprite_store.XUVY_STORE9_X0p.qn08_old(), _ACAM_PX0n_t0);
    /* p31.YTYP*/ wire _YTYP_STORE9_MATCH1n_t0 = xor2(sprite_store.XERE_STORE9_X1p.qn08_old(), _AZUB_PX1n_t0);
    /* p31.YFOP*/ wire _YFOP_STORE9_MATCH2n_t0 = xor2(sprite_store.XUZO_STORE9_X2p.qn08_old(), _AMEL_PX2n_t0);
    /* p31.YVAC*/ wire _YVAC_STORE9_MATCH3n_t0 = xor2(sprite_store.XEXA_STORE9_X3p.qn08_old(), _AHAL_PX3n_t0);
    /* p31.ZYWU*/ wire _ZYWU_STORE9_MATCH4n_t0 = xor2(sprite_store.YPOD_STORE9_X4p.qn08_old(), _APUX_PX4n_t0);
    /* p31.ZUZA*/ wire _ZUZA_STORE9_MATCH5n_t0 = xor2(sprite_store.YROP_STORE9_X5p.qn08_old(), _ABEF_PX5n_t0);
    /* p31.ZEJO*/ wire _ZEJO_STORE9_MATCH6n_t0 = xor2(sprite_store.YNEP_STORE9_X6p.qn08_old(), _ADAZ_PX6n_t0);
    /* p31.ZEDA*/ wire _ZEDA_STORE9_MATCH7n_t0 = xor2(sprite_store.YZOF_STORE9_X7p.qn08_old(), _ASAH_PX7n_t0);

    /* p31.ZAKO*/ wire _ZAKO_STORE0_MATCHAp_t0 = nor4(_ZOGY_STORE0_MATCH0n_t0, _ZEBA_STORE0_MATCH1n_t0, _ZAHA_STORE0_MATCH2n_t0, _ZOKY_STORE0_MATCH3n_t0);
    /* p31.XEBA*/ wire _XEBA_STORE0_MATCHBp_t0 = nor4(_WOJU_STORE0_MATCH4n_t0, _YFUN_STORE0_MATCH5n_t0, _WYZA_STORE0_MATCH6n_t0, _YPUK_STORE0_MATCH7n_t0);
    /* p31.CYVY*/ wire _CYVY_STORE1_MATCHBp_t0 = nor4(_COLA_STORE1_MATCH4n_t0, _BOBA_STORE1_MATCH5n_t0, _COLU_STORE1_MATCH6n_t0, _BAHU_STORE1_MATCH7n_t0);
    /* p31.EWAM*/ wire _EWAM_STORE1_MATCHAp_t0 = nor4(_EDYM_STORE1_MATCH0n_t0, _EMYB_STORE1_MATCH1n_t0, _EBEF_STORE1_MATCH2n_t0, _EWOK_STORE1_MATCH3n_t0);
    /* p31.CEHU*/ wire _CEHU_STORE2_MATCHAp_t0 = nor4(_CEKO_STORE2_MATCH4n_t0, _DETY_STORE2_MATCH5n_t0, _DOZO_STORE2_MATCH6n_t0, _CONY_STORE2_MATCH7n_t0);
    /* p31.EKES*/ wire _EKES_STORE2_MATCHBp_t0 = nor4(_FUZU_STORE2_MATCH0n_t0, _FESO_STORE2_MATCH1n_t0, _FOKY_STORE2_MATCH2n_t0, _FYVA_STORE2_MATCH3n_t0);
    /* p31.ZURE*/ wire _ZURE_STORE3_MATCHAp_t0 = nor4(_YHOK_STORE3_MATCH0n_t0, _YCAH_STORE3_MATCH1n_t0, _YDAJ_STORE3_MATCH2n_t0, _YVUZ_STORE3_MATCH3n_t0);
    /* p31.YWOS*/ wire _YWOS_STORE3_MATCHBp_t0 = nor4(_YVAP_STORE3_MATCH4n_t0, _XENY_STORE3_MATCH5n_t0, _XAVU_STORE3_MATCH6n_t0, _XEVA_STORE3_MATCH7n_t0);
    /* p31.YKOK*/ wire _YKOK_STORE4_MATCHAp_t0 = nor4(_XEJU_STORE4_MATCH0n_t0, _ZATE_STORE4_MATCH1n_t0, _ZAKU_STORE4_MATCH2n_t0, _YBOX_STORE4_MATCH3n_t0);
    /* p31.YNAZ*/ wire _YNAZ_STORE4_MATCHBp_t0 = nor4(_ZYKU_STORE4_MATCH4n_t0, _ZYPU_STORE4_MATCH5n_t0, _XAHA_STORE4_MATCH6n_t0, _ZEFE_STORE4_MATCH7n_t0);
    /* p31.COGY*/ wire _COGY_STORE5_MATCHAp_t0 = nor4(_BAZY_STORE5_MATCH4n_t0, _CYLE_STORE5_MATCH5n_t0, _CEVA_STORE5_MATCH6n_t0, _BUMY_STORE5_MATCH7n_t0);
    /* p31.FYMA*/ wire _FYMA_STORE5_MATCHBp_t0 = nor4(_GUZO_STORE5_MATCH0n_t0, _GOLA_STORE5_MATCH1n_t0, _GEVE_STORE5_MATCH2n_t0, _GUDE_STORE5_MATCH3n_t0);
    /* p31.YWAP*/ wire _YWAP_STORE6_MATCHAp_t0 = nor4(_ZARE_STORE6_MATCH4n_t0, _ZEMU_STORE6_MATCH5n_t0, _ZYGO_STORE6_MATCH6n_t0, _ZUZY_STORE6_MATCH7n_t0);
    /* p31.YDOT*/ wire _YDOT_STORE6_MATCHBp_t0 = nor4(_XOSU_STORE6_MATCH0n_t0, _ZUVU_STORE6_MATCH1n_t0, _XUCO_STORE6_MATCH2n_t0, _ZULO_STORE6_MATCH3n_t0);
    /* p31.CYCO*/ wire _CYCO_STORE7_MATCHAp_t0 = nor4(_DUSE_STORE7_MATCH0n_t0, _DAGU_STORE7_MATCH1n_t0, _DYZE_STORE7_MATCH2n_t0, _DESO_STORE7_MATCH3n_t0);
    /* p31.DAJE*/ wire _DAJE_STORE7_MATCHBp_t0 = nor4(_EJOT_STORE7_MATCH4n_t0, _ESAJ_STORE7_MATCH5n_t0, _DUCU_STORE7_MATCH6n_t0, _EWUD_STORE7_MATCH7n_t0);
    /* p31.DAMA*/ wire _DAMA_STORE8_MATCHAp_t0 = nor4(_DUZE_STORE8_MATCH0n_t0, _DAGA_STORE8_MATCH1n_t0, _DAWU_STORE8_MATCH2n_t0, _EJAW_STORE8_MATCH3n_t0);
    /* p31.FEHA*/ wire _FEHA_STORE8_MATCHBp_t0 = nor4(_GOHO_STORE8_MATCH4n_t0, _GASU_STORE8_MATCH5n_t0, _GABU_STORE8_MATCH6n_t0, _GAFE_STORE8_MATCH7n_t0);
    /* p31.YLEV*/ wire _YLEV_STORE9_MATCHAp_t0 = nor4(_YMAM_STORE9_MATCH0n_t0, _YTYP_STORE9_MATCH1n_t0, _YFOP_STORE9_MATCH2n_t0, _YVAC_STORE9_MATCH3n_t0);
    /* p31.YTUB*/ wire _YTUB_STORE9_MATCHBp_t0 = nor4(_ZYWU_STORE9_MATCH4n_t0, _ZUZA_STORE9_MATCH5n_t0, _ZEJO_STORE9_MATCH6n_t0, _ZEDA_STORE9_MATCH7n_t0);

    /* p29.CEHA*/ wire _CEHA_SCANNINGp_t0 = not1(sprite_scanner.CENO_SCANNINGp.qn16_old());
    /*#p29.BYJO*/ wire _BYJO_SCANNINGn_t0 = not1(_CEHA_SCANNINGp_t0);
    /*#p29.AZEM*/ wire _AZEM_RENDERINGp_t2 = and2(pix_pipe.XYMU_RENDERINGn.qn03_new(), _BYJO_SCANNINGn_t0);
    /*#p29.AROR*/ wire _AROR_MATCH_ENp_t2 = and2(_AZEM_RENDERINGp_t2, pix_pipe.XYLO_LCDC_SPENn.qn08_new());
    /* p29.YDUG*/ _YDUG_STORE0_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _ZAKO_STORE0_MATCHAp_t0, _XEBA_STORE0_MATCHBp_t0);
    /* p29.DYDU*/ _DYDU_STORE1_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _EWAM_STORE1_MATCHAp_t0, _CYVY_STORE1_MATCHBp_t0);
    /* p29.DEGO*/ _DEGO_STORE2_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _CEHU_STORE2_MATCHAp_t0, _EKES_STORE2_MATCHBp_t0);
    /* p29.YLOZ*/ _YLOZ_STORE3_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _ZURE_STORE3_MATCHAp_t0, _YWOS_STORE3_MATCHBp_t0);
    /* p29.XAGE*/ _XAGE_STORE4_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _YKOK_STORE4_MATCHAp_t0, _YNAZ_STORE4_MATCHBp_t0);
    /* p29.EGOM*/ _EGOM_STORE5_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _COGY_STORE5_MATCHAp_t0, _FYMA_STORE5_MATCHBp_t0);
    /* p29.YBEZ*/ _YBEZ_STORE6_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _YWAP_STORE6_MATCHAp_t0, _YDOT_STORE6_MATCHBp_t0);
    /* p29.DYKA*/ _DYKA_STORE7_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _CYCO_STORE7_MATCHAp_t0, _DAJE_STORE7_MATCHBp_t0);
    /* p29.EFYL*/ _EFYL_STORE8_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _DAMA_STORE8_MATCHAp_t0, _FEHA_STORE8_MATCHBp_t0);
    /* p29.YGEM*/ _YGEM_STORE9_MATCHn_t0 = nand3(_AROR_MATCH_ENp_t2, _YLEV_STORE9_MATCHAp_t0, _YTUB_STORE9_MATCHBp_t0);

    /* p29.FEFY*/ wire _FEFY_STORE_MATCHp_t0 = nand5(_XAGE_STORE4_MATCHn_t0, _YLOZ_STORE3_MATCHn_t0, _DEGO_STORE2_MATCHn_t0, _DYDU_STORE1_MATCHn_t0, _YDUG_STORE0_MATCHn_t0);
    /* p29.FOVE*/ wire _FOVE_STORE_MATCHp_t0 = nand5(_YGEM_STORE9_MATCHn_t0, _EFYL_STORE8_MATCHn_t0, _DYKA_STORE7_MATCHn_t0, _YBEZ_STORE6_MATCHn_t0, _EGOM_STORE5_MATCHn_t0);
    /* p29.FEPO*/ _FEPO_STORE_MATCHp_t0 = or2(_FEFY_STORE_MATCHp_t0, _FOVE_STORE_MATCHp_t0);

    /*#p21.XENA*/ wire _XENA_STORE_MATCHn_t0 = not1(_FEPO_STORE_MATCHp_t0);
    /*#p21.XUGU*/ wire _XUGU_PX167n_t0 = nand5(pix_pipe.XEHO_PX0p.qp17_old(),
                                               pix_pipe.SAVY_PX1p.qp17_old(),
                                               pix_pipe.XODU_PX2p.qp17_old(),
                                               pix_pipe.TUKY_PX5p.qp17_old(),
                                               pix_pipe.SYBE_PX7p.qp17_old()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire _XANO_PX167p_t0 = not1(_XUGU_PX167n_t0);
    /*#p21.WODU*/ _WODU_HBLANKp_t0 = and2(_XENA_STORE_MATCHn_t0, _XANO_PX167p_t0);
  }
#pragma endregion

  {
    /*#p21.VOGA*/ pix_pipe.VOGA_HBLANKp.dff17_ff(_ALET_xBxDxFxH_t0, _WODU_HBLANKp_t0);
  }

  /* p24.LOBY*/ wire _LOBY_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
  /* p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONEp.qp17_old(), _LOBY_RENDERINGn_t2);

  /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn_t2 = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp04_new());
  /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn_t2 = nand4(pix_pipe.XYMU_RENDERINGn.qn03_new(), _ROMO_PRELOAD_DONEn_t2, tile_fetcher.NYKA_FETCH_DONEp.qp17_old(), tile_fetcher.PORY_FETCH_DONEp.qp17_old());
  /*#p27.NUNY*/ wire _NUNY_WX_MATCH_TRIGp_t0 = and2(pix_pipe.PYNU_WIN_MODE_Ap.qp04_new(), pix_pipe.NOPA_WIN_MODE_Bp.qn16_old());
  /* p27.NYFO*/ wire _NYFO_WIN_FETCH_TRIGn_t0 = not1(_NUNY_WX_MATCH_TRIGp_t0);
  /* p27.MOSU*/ wire _MOSU_WIN_FETCH_TRIGp_t0 = not1(_NYFO_WIN_FETCH_TRIGn_t0);
  /* p27.TAVE*/ wire _TAVE_PRELOAD_DONE_TRIGp_t2 = not1(_SUVU_PRELOAD_DONE_TRIGn_t2);

  {
    /* p27.SEKO*/ wire _SEKO_FETCH_TRIGp_t0 = nor2(pix_pipe.RYFA_FETCHn_A.qn16_old(), pix_pipe.RENE_FETCHn_B.qp17_old());
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_t0 = not1(pix_pipe.RYDY_WIN_HITp.to_wire_new());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_t0 = nand2(_SYLO_WIN_HITn_t0, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17_old());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_t0 = not1(_TUXY_WIN_FIRST_TILEne_t0);
    /* p27.TEVO*/ wire _TEVO_FETCH_TRIGp_t2 = or3(_SEKO_FETCH_TRIGp_t0, _SUZU_WIN_FIRST_TILEne_t0, _TAVE_PRELOAD_DONE_TRIGp_t2); // Schematic wrong, this is OR
    /* p27.NYXU*/ wire _NYXU_FETCH_TRIGn_t2 = nor3(_AVAP_SCAN_DONE_TRIGp_t2, _MOSU_WIN_FETCH_TRIGp_t0, _TEVO_FETCH_TRIGp_t2);
    /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn_t2 = and2(tile_fetcher.LOVY_FETCH_DONEp.qn16_old(), pix_pipe.XYMU_RENDERINGn.qn03_new());
    /* p27.LONY*/ tile_fetcher.LONY_FETCHINGp.nand_latch(_NYXU_FETCH_TRIGn_t2, _LURY_BG_FETCH_DONEn_t2);
  }

  {
    /*#p27.PAHA*/ wire _PAHA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
    /*#p27.POVA*/ wire _POVA_FINE_MATCHpe_t0 = and2(pix_pipe.PUXA_SCX_FINE_MATCH_A.qp17_old(), pix_pipe.NYZE_SCX_FINE_MATCH_B.qn16_old());
    /*#p27.ROXY*/ pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.nor_latch(_PAHA_RENDERINGn_t2, _POVA_FINE_MATCHpe_t0);
  }

  /*#p24.SEGU*/ bool _SEGU_CLKPIPEn_t2;
  /*#p24.SACU*/ bool _SACU_CLKPIPEp_t2;
  {
    /*#p24.VYBO*/ wire _VYBO_CLKPIPEp_t2 = nor3(_FEPO_STORE_MATCHp_t0, _WODU_HBLANKp_t0, _MYVO_AxCxExGx_t0);
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_t0 = not1(pix_pipe.RYDY_WIN_HITp.to_wire_new());
    /*#p24.TOMU*/ wire _TOMU_WIN_HITp_t0 = not1(_SYLO_WIN_HITn_t0);
    /*#p24.SOCY*/ wire _SOCY_WIN_HITn_t0 = not1(_TOMU_WIN_HITp_t0);
    /*#p24.TYFA*/ wire _TYFA_CLKPIPEp_t2 = and3(_SOCY_WIN_HITn_t0, tile_fetcher.POKY_PRELOAD_LATCHp.qp04_new(), _VYBO_CLKPIPEp_t2);
    /*#p24.SEGU*/ _SEGU_CLKPIPEn_t2 = not1(_TYFA_CLKPIPEp_t2);
    /*#p24.SACU*/ _SACU_CLKPIPEp_t2 = or2(_SEGU_CLKPIPEn_t2, pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.qp04_new()); // Schematic wrong, this is OR
  }

  //----------------------------------------
  // Pixel counter, has carry lookahead because this can increment every tcycle

  {
    /* p21.RYBO*/ wire _RYBO = xor2(pix_pipe.XEHO_PX0p.qp17_old(), pix_pipe.SAVY_PX1p.qp17_old()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /* p21.XUKE*/ wire _XUKE = and2(pix_pipe.XEHO_PX0p.qp17_old(), pix_pipe.SAVY_PX1p.qp17_old());
    /* p21.XYLE*/ wire _XYLE = and2(pix_pipe.XODU_PX2p.qp17_old(), _XUKE);
    /* p21.XEGY*/ wire _XEGY = xor2(pix_pipe.XODU_PX2p.qp17_old(), _XUKE); // feet facing gnd
    /* p21.XORA*/ wire _XORA = xor2(pix_pipe.XYDO_PX3p.qp17_old(), _XYLE); // feet facing gnd

    /* p21.XEHO*/ pix_pipe.XEHO_PX0p.dff17_ff(_SACU_CLKPIPEp_t2, pix_pipe.XEHO_PX0p.qn16_old());
    /* p21.SAVY*/ pix_pipe.SAVY_PX1p.dff17_ff(_SACU_CLKPIPEp_t2, _RYBO);
    /* p21.XODU*/ pix_pipe.XODU_PX2p.dff17_ff(_SACU_CLKPIPEp_t2, _XEGY);
    /* p21.XYDO*/ pix_pipe.XYDO_PX3p.dff17_ff(_SACU_CLKPIPEp_t2, _XORA);

    /* p24.TOCA*/ wire _TOCA = not1(pix_pipe.XYDO_PX3p.qp17_new());

    /* p21.SAKE*/ wire _SAKE = xor2(pix_pipe.TUHU_PX4p.qp17_old(), pix_pipe.TUKY_PX5p.qp17_old());
    /* p21.TYBA*/ wire _TYBA = and2(pix_pipe.TUHU_PX4p.qp17_old(), pix_pipe.TUKY_PX5p.qp17_old());
    /* p21.SURY*/ wire _SURY = and2(pix_pipe.TAKO_PX6p.qp17_old(), _TYBA);
    /* p21.TYGE*/ wire _TYGE = xor2(pix_pipe.TAKO_PX6p.qp17_old(), _TYBA);
    /* p21.ROKU*/ wire _ROKU = xor2(pix_pipe.SYBE_PX7p.qp17_old(), _SURY);

    /* p21.TUHU*/ pix_pipe.TUHU_PX4p.dff17_ff(_TOCA, pix_pipe.TUHU_PX4p.qn16_old());
    /* p21.TUKY*/ pix_pipe.TUKY_PX5p.dff17_ff(_TOCA, _SAKE);
    /* p21.TAKO*/ pix_pipe.TAKO_PX6p.dff17_ff(_TOCA, _TYGE);
    /* p21.SYBE*/ pix_pipe.SYBE_PX7p.dff17_ff(_TOCA, _ROKU);

    /* p21.XEHO*/ pix_pipe.XEHO_PX0p.dff17_rst(_TADY_LINE_RSTn_t2);
    /* p21.SAVY*/ pix_pipe.SAVY_PX1p.dff17_rst(_TADY_LINE_RSTn_t2);
    /* p21.XODU*/ pix_pipe.XODU_PX2p.dff17_rst(_TADY_LINE_RSTn_t2);
    /* p21.XYDO*/ pix_pipe.XYDO_PX3p.dff17_rst(_TADY_LINE_RSTn_t2);
    /* p21.TUHU*/ pix_pipe.TUHU_PX4p.dff17_rst(_TADY_LINE_RSTn_t2);
    /* p21.TUKY*/ pix_pipe.TUKY_PX5p.dff17_rst(_TADY_LINE_RSTn_t2);
    /* p21.TAKO*/ pix_pipe.TAKO_PX6p.dff17_rst(_TADY_LINE_RSTn_t2);
    /* p21.SYBE*/ pix_pipe.SYBE_PX7p.dff17_rst(_TADY_LINE_RSTn_t2);
  }
















































































































#pragma region SpriteFetcher
  {
    // WARNING reg-to-clock-to-reg loops

    /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1.dff17(_LAPE_AxCxExGx_t0,                           _VYPO_VCC, sprite_fetcher.TOXE_SFETCH_S0p.qp17_old());
    /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5.dff17(_LAPE_AxCxExGx_t0, pix_pipe.XYMU_RENDERINGn.qn03_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp17_old());
    /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4.dff17(_TAVA_xBxDxFxH_t0, pix_pipe.XYMU_RENDERINGn.qn03_new(), sprite_fetcher.TOBU_SFETCH_S1p_D2.qp17_old());
    /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2.dff17(_TAVA_xBxDxFxH_t0, pix_pipe.XYMU_RENDERINGn.qn03_new(), sprite_fetcher.TULY_SFETCH_S1p.qp17_old());

    /* p29.TYNO*/ wire _TYNO_t0 = nand3(sprite_fetcher.TOXE_SFETCH_S0p.qp17_old(), sprite_fetcher.SEBA_SFETCH_S1p_D5.qp17_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp17_new());
    /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn_t0  = or2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qn16_new(), _TYNO_t0);
    /* p29.WUTY*/ wire _WUTY_SPRITE_DONEp_t0 = not1(_VUSA_SPRITE_DONEn_t0);
    /* p27.SEKO*/ wire _SEKO_FETCH_TRIGp_t0 = nor2(pix_pipe.RYFA_FETCHn_A.qn16_old(), pix_pipe.RENE_FETCHn_B.qp17_old());
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_t0 = not1(pix_pipe.RYDY_WIN_HITp.to_wire_new());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_t0 = nand2(_SYLO_WIN_HITn_t0, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17_old());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_t0 = not1(_TUXY_WIN_FIRST_TILEne_t0);
    /* p27.TEVO*/ wire _TEVO_FETCH_TRIGp_t2 = or3(_SEKO_FETCH_TRIGp_t0, _SUZU_WIN_FIRST_TILEne_t0, _TAVE_PRELOAD_DONE_TRIGp_t2); // Schematic wrong, this is OR
    /* p27.NYXU*/ wire _NYXU_FETCH_TRIGn_t2 = nor3(_AVAP_SCAN_DONE_TRIGp_t2, _MOSU_WIN_FETCH_TRIGp_t0, _TEVO_FETCH_TRIGp_t2);
    /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn_t2 = nand3(tile_fetcher.LAXU_BFETCH_S0p.qp17_old(), tile_fetcher.NYVA_BFETCH_S2p.qp17_old(), _NYXU_FETCH_TRIGn_t2);
    /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp_t2 = not1(_MOCE_BFETCH_DONEn_t2);
    /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn_t2 = nor2(_WUTY_SPRITE_DONEp_t0, _TAVE_PRELOAD_DONE_TRIGp_t2); // def nor

    /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE_t0 = nand2(sprite_fetcher.TESE_SFETCH_S2p.qp17_old(), sprite_fetcher.TOXE_SFETCH_S0p.qp17_old());
    /*#p29.TOMA*/ wire _TOMA_SFETCH_CLK_xBxDxFxH_t0 = nand2(_LAPE_AxCxExGx_t0, _TAME_SFETCH_CLK_GATE_t0);

    /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp_t0 = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp17_old(), sprite_fetcher.SUDA_SFETCH_REQp.qn16_old());
    /*#p27.SECA*/ wire _SECA_SFETCH_RUNNING_SETn_t2 = nor3(_RYCE_SFETCH_TRIGp_t0, _ROSY_VID_RSTp_t0, _ATEJ_LINE_TRIGp_t2);

    /* p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RUNNING_SETn_t2, _VEKU_SFETCH_RUNNING_RSTn_t2);
    /* p27.SOWO*/ wire _SOWO_SFETCH_RUNNINGn_t2 = not1(sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp03_new());

    /*#p24.TOMU*/ wire _TOMU_WIN_HITp_t0 = not1(_SYLO_WIN_HITn_t0);
    /* p27.TUKU*/ wire _TUKU_WIN_HITn_t0 = not1(_TOMU_WIN_HITp_t0);
    /* p27.TEKY*/ wire _TEKY_SFETCH_REQp_t2 = and4(_FEPO_STORE_MATCHp_t0, _TUKU_WIN_HITn_t0, _LYRY_BFETCH_DONEp_t2, _SOWO_SFETCH_RUNNINGn_t2);

    /* p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp.dff17(_LAPE_AxCxExGx_t0, _VYPO_VCC, sprite_fetcher.SOBU_SFETCH_REQp.qp17_old());
    /* p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp.dff17(_TAVA_xBxDxFxH_t0, _VYPO_VCC, _TEKY_SFETCH_REQp_t2);

    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.dff17(_TOMA_SFETCH_CLK_xBxDxFxH_t0,              _SECA_SFETCH_RUNNING_SETn_t2, sprite_fetcher.TOXE_SFETCH_S0p.qn16_old());
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p.dff17(sprite_fetcher.TOXE_SFETCH_S0p.qn16_new(), _SECA_SFETCH_RUNNING_SETn_t2, sprite_fetcher.TULY_SFETCH_S1p.qn16_old());
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p.dff17(sprite_fetcher.TULY_SFETCH_S1p.qn16_new(), _SECA_SFETCH_RUNNING_SETn_t2, sprite_fetcher.TESE_SFETCH_S2p.qn16_old());
  }
#pragma endregion


  //----------------------------------------

  /* SPR_I */
  Bus2 SPR_TRI_I0p_t2;
  Bus2 SPR_TRI_I1p_t2;
  Bus2 SPR_TRI_I2p_t2;
  Bus2 SPR_TRI_I3p_t2;
  Bus2 SPR_TRI_I4p_t2;
  Bus2 SPR_TRI_I5p_t2;

  /* SPR_L */
  Bus2 SPR_TRI_L0p_t2;
  Bus2 SPR_TRI_L1p_t2;
  Bus2 SPR_TRI_L2p_t2;
  Bus2 SPR_TRI_L3p_t2;

  {
    // this is using an adder as a subtracter by inverting the first input.

    /*#p29.EBOS*/ wire _EBOS_LY0n_t0 = not1(lcd_reg.MUWY_LY0p.qp17_old());
    /* p29.DASA*/ wire _DASA_LY1n_t0 = not1(lcd_reg.MYRO_LY1p.qp17_old());
    /* p29.FUKY*/ wire _FUKY_LY2n_t0 = not1(lcd_reg.LEXA_LY2p.qp17_old());
    /* p29.FUVE*/ wire _FUVE_LY3n_t0 = not1(lcd_reg.LYDO_LY3p.qp17_old());
    /* p29.FEPU*/ wire _FEPU_LY4n_t0 = not1(lcd_reg.LOVU_LY4p.qp17_old());
    /* p29.FOFA*/ wire _FOFA_LY5n_t0 = not1(lcd_reg.LEMA_LY5p.qp17_old());
    /* p29.FEMO*/ wire _FEMO_LY6n_t0 = not1(lcd_reg.MATO_LY6p.qp17_old());
    /* p29.GUSU*/ wire _GUSU_LY7n_t0 = not1(lcd_reg.LAFO_LY7p.qp17_old());

    /* p29.ERUC*/ wire _ERUC_YDIFF_S0_t0 = add_s(_EBOS_LY0n_t0, oam_bus.XUSO_OAM_DA0p.qp08_old(), GND);
    /* p29.ERUC*/ wire _ERUC_YDIFF_C0_t0 = add_c(_EBOS_LY0n_t0, oam_bus.XUSO_OAM_DA0p.qp08_old(), GND);
    /* p29.ENEF*/ wire _ENEF_YDIFF_S1_t0 = add_s(_DASA_LY1n_t0, oam_bus.XEGU_OAM_DA1p.qp08_old(), _ERUC_YDIFF_C0_t0);
    /* p29.ENEF*/ wire _ENEF_YDIFF_C1_t0 = add_c(_DASA_LY1n_t0, oam_bus.XEGU_OAM_DA1p.qp08_old(), _ERUC_YDIFF_C0_t0);
    /* p29.FECO*/ wire _FECO_YDIFF_S2_t0 = add_s(_FUKY_LY2n_t0, oam_bus.YJEX_OAM_DA2p.qp08_old(), _ENEF_YDIFF_C1_t0);
    /* p29.FECO*/ wire _FECO_YDIFF_C2_t0 = add_c(_FUKY_LY2n_t0, oam_bus.YJEX_OAM_DA2p.qp08_old(), _ENEF_YDIFF_C1_t0);
    /* p29.GYKY*/ wire _GYKY_YDIFF_S3_t0 = add_s(_FUVE_LY3n_t0, oam_bus.XYJU_OAM_DA3p.qp08_old(), _FECO_YDIFF_C2_t0);
    /* p29.GYKY*/ wire _GYKY_YDIFF_C3_t0 = add_c(_FUVE_LY3n_t0, oam_bus.XYJU_OAM_DA3p.qp08_old(), _FECO_YDIFF_C2_t0);
    /* p29.GOPU*/ wire _GOPU_YDIFF_S4_t0 = add_s(_FEPU_LY4n_t0, oam_bus.YBOG_OAM_DA4p.qp08_old(), _GYKY_YDIFF_C3_t0);
    /* p29.GOPU*/ wire _GOPU_YDIFF_C4_t0 = add_c(_FEPU_LY4n_t0, oam_bus.YBOG_OAM_DA4p.qp08_old(), _GYKY_YDIFF_C3_t0);
    /* p29.FUWA*/ wire _FUWA_YDIFF_S5_t0 = add_s(_FOFA_LY5n_t0, oam_bus.WYSO_OAM_DA5p.qp08_old(), _GOPU_YDIFF_C4_t0);
    /* p29.FUWA*/ wire _FUWA_YDIFF_C5_t0 = add_c(_FOFA_LY5n_t0, oam_bus.WYSO_OAM_DA5p.qp08_old(), _GOPU_YDIFF_C4_t0);
    /* p29.GOJU*/ wire _GOJU_YDIFF_S6_t0 = add_s(_FEMO_LY6n_t0, oam_bus.XOTE_OAM_DA6p.qp08_old(), _FUWA_YDIFF_C5_t0);
    /* p29.GOJU*/ wire _GOJU_YDIFF_C6_t0 = add_c(_FEMO_LY6n_t0, oam_bus.XOTE_OAM_DA6p.qp08_old(), _FUWA_YDIFF_C5_t0);
    /* p29.WUHU*/ wire _WUHU_YDIFF_S7_t0 = add_s(_GUSU_LY7n_t0, oam_bus.YZAB_OAM_DA7p.qp08_old(), _GOJU_YDIFF_C6_t0);
    /* p29.WUHU*/ wire _WUHU_YDIFF_C7_t0 = add_c(_GUSU_LY7n_t0, oam_bus.YZAB_OAM_DA7p.qp08_old(), _GOJU_YDIFF_C6_t0);

    /* p29.GACE*/ wire _GACE_SPRITE_DELTA4_t0 = not1(_GOPU_YDIFF_S4_t0);
    /* p29.GUVU*/ wire _GUVU_SPRITE_DELTA5_t0 = not1(_FUWA_YDIFF_S5_t0);
    /* p29.GYDA*/ wire _GYDA_SPRITE_DELTA6_t0 = not1(_GOJU_YDIFF_S6_t0);
    /* p29.GEWY*/ wire _GEWY_SPRITE_DELTA7_t0 = not1(_WUHU_YDIFF_S7_t0);
    /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH_t0  = or2(pix_pipe.XYMO_LCDC_SPSIZEn.qn08_new(), _GYKY_YDIFF_S3_t0);

    /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn_t0 = nand6(_GACE_SPRITE_DELTA4_t0, _GUVU_SPRITE_DELTA5_t0, _GYDA_SPRITE_DELTA6_t0, _GEWY_SPRITE_DELTA7_t0, _WUHU_YDIFF_C7_t0, _GOVU_SPSIZE_MATCH_t0);
    /* p29.GESE*/ wire _GESE_SCAN_MATCH_Yp_t0 = not1(_WOTA_SCAN_MATCH_Yn_t0);

    {
      /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn_t2 = and2(sprite_scanner.CENO_SCANNINGp.qn16_old(), pix_pipe.XYMU_RENDERINGn.qn03_new());
      /*#p30.WUZY*/ SPR_TRI_I0p_t2.tri6_nn(_BUZA_STORE_SPRITE_INDXn_t2, sprite_scanner.XADU_SPRITE_IDX0p.qn12_old());
      /* p30.WYSE*/ SPR_TRI_I1p_t2.tri6_nn(_BUZA_STORE_SPRITE_INDXn_t2, sprite_scanner.XEDY_SPRITE_IDX1p.qn12_old());
      /* p30.ZYSU*/ SPR_TRI_I2p_t2.tri6_nn(_BUZA_STORE_SPRITE_INDXn_t2, sprite_scanner.ZUZE_SPRITE_IDX2p.qn12_old());
      /* p30.WYDA*/ SPR_TRI_I3p_t2.tri6_nn(_BUZA_STORE_SPRITE_INDXn_t2, sprite_scanner.XOBE_SPRITE_IDX3p.qn12_old());
      /* p30.WUCO*/ SPR_TRI_I4p_t2.tri6_nn(_BUZA_STORE_SPRITE_INDXn_t2, sprite_scanner.YDUF_SPRITE_IDX4p.qn12_old());
      /* p30.WEZA*/ SPR_TRI_I5p_t2.tri6_nn(_BUZA_STORE_SPRITE_INDXn_t2, sprite_scanner.XECU_SPRITE_IDX5p.qn12_old());
    }

    {
      /* p29.DEGE*/ wire _DEGE_SPRITE_DELTA0_t0 = not1(_ERUC_YDIFF_S0_t0);
      /* p29.DABY*/ wire _DABY_SPRITE_DELTA1_t0 = not1(_ENEF_YDIFF_S1_t0);
      /* p29.DABU*/ wire _DABU_SPRITE_DELTA2_t0 = not1(_FECO_YDIFF_S2_t0);
      /* p29.GYSA*/ wire _GYSA_SPRITE_DELTA3_t0 = not1(_GYKY_YDIFF_S3_t0);

      /*#p30.CUCU*/ SPR_TRI_L0p_t2.tri6_nn(_FEPO_STORE_MATCHp_t0, _DEGE_SPRITE_DELTA0_t0);
      /*#p30.CUCA*/ SPR_TRI_L1p_t2.tri6_nn(_FEPO_STORE_MATCHp_t0, _DABY_SPRITE_DELTA1_t0);
      /*#p30.CEGA*/ SPR_TRI_L2p_t2.tri6_nn(_FEPO_STORE_MATCHp_t0, _DABU_SPRITE_DELTA2_t0);
      /*#p30.WENU*/ SPR_TRI_L3p_t2.tri6_nn(_FEPO_STORE_MATCHp_t0, _GYSA_SPRITE_DELTA3_t0);
    }



    /* p29.CEHA*/ wire _CEHA_SCANNINGp_t0 = not1(sprite_scanner.CENO_SCANNINGp.qn16_old());
    /* p29.CARE*/ wire _CARE_STORE_ENp_ABxxEFxx_t1 = and3(_XOCE_xBCxxFGx_t1, _CEHA_SCANNINGp_t0, _GESE_SCAN_MATCH_Yp_t0); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /* p29.DYTY*/ wire _DYTY_STORE_ENn_xxCDxxGH_t1 = not1(_CARE_STORE_ENp_ABxxEFxx_t1);

    {
      // Sprite store counter. The sprite count clock stops ticking once we have 10 sprites.
      /*#p29.BAKY*/ wire _BAKY_SPRITES_FULL_t0 = and2(sprite_store.CUXY_SPRITE_COUNT1.qp17_old(), sprite_store.DYBE_SPRITE_COUNT3.qp17_old());
      /*#p29.CAKE*/ wire _CAKE_CLKp_t0 = or2(_BAKY_SPRITES_FULL_t0, sprite_store.DEZY_STORE_ENn.qp17_old());
      /* p29.BESE*/ sprite_store.BESE_SPRITE_COUNT0.dff17(_CAKE_CLKp_t0,                              _AZYB_LINE_TRIGn_t2, sprite_store.BESE_SPRITE_COUNT0.qn16_old());
      /* p29.CUXY*/ sprite_store.CUXY_SPRITE_COUNT1.dff17(sprite_store.BESE_SPRITE_COUNT0.qn16_new(), _AZYB_LINE_TRIGn_t2, sprite_store.CUXY_SPRITE_COUNT1.qn16_old());
      /* p29.BEGO*/ sprite_store.BEGO_SPRITE_COUNT2.dff17(sprite_store.CUXY_SPRITE_COUNT1.qn16_new(), _AZYB_LINE_TRIGn_t2, sprite_store.BEGO_SPRITE_COUNT2.qn16_old());
      /* p29.DYBE*/ sprite_store.DYBE_SPRITE_COUNT3.dff17(sprite_store.BEGO_SPRITE_COUNT2.qn16_new(), _AZYB_LINE_TRIGn_t2, sprite_store.DYBE_SPRITE_COUNT3.qn16_old());

      /* p29.DEZY*/ sprite_store.DEZY_STORE_ENn.dff17(_ZEME_AxCxExGx_t0, _XAPO_VID_RSTn_t0, _DYTY_STORE_ENn_xxCDxxGH_t1);
    }


    //----------------------------------------
    // 10 sprite stores
    // Resetting the store X coords to 0 doesn't make sense, as they'd fire during a line even if we never stored any sprites.
    // I guess it must store inverted X, so that when reset X = 0xFF?

    /* p29.EDEN*/ wire _EDEN_SPRITE_COUNT0n_t0 = not1(sprite_store.BESE_SPRITE_COUNT0.qp17_new());
    /* p29.CYPY*/ wire _CYPY_SPRITE_COUNT1n_t0 = not1(sprite_store.CUXY_SPRITE_COUNT1.qp17_new());
    /* p29.CAPE*/ wire _CAPE_SPRITE_COUNT2n_t0 = not1(sprite_store.BEGO_SPRITE_COUNT2.qp17_new());
    /* p29.CAXU*/ wire _CAXU_SPRITE_COUNT3n_t0 = not1(sprite_store.DYBE_SPRITE_COUNT3.qp17_new());

    /* p29.FYCU*/ wire _FYCU_SPRITE_COUNT0p_t0 = not1(_EDEN_SPRITE_COUNT0n_t0);
    /* p29.FONE*/ wire _FONE_SPRITE_COUNT1p_t0 = not1(_CYPY_SPRITE_COUNT1n_t0);
    /* p29.EKUD*/ wire _EKUD_SPRITE_COUNT2p_t0 = not1(_CAPE_SPRITE_COUNT2n_t0);
    /* p29.ELYG*/ wire _ELYG_SPRITE_COUNT3p_t0 = not1(_CAXU_SPRITE_COUNT3n_t0);

    /*#p31.ZAGO*/ wire _ZAGO_SPX0n_t0 = not1(oam_bus.YLOR_OAM_DB0p.qp08_old());
    /* p31.ZOCY*/ wire _ZOCY_SPX1n_t0 = not1(oam_bus.ZYTY_OAM_DB1p.qp08_old());
    /* p31.YPUR*/ wire _YPUR_SPX2n_t0 = not1(oam_bus.ZYVE_OAM_DB2p.qp08_old());
    /* p31.YVOK*/ wire _YVOK_SPX3n_t0 = not1(oam_bus.ZEZY_OAM_DB3p.qp08_old());
    /* p31.COSE*/ wire _COSE_SPX4n_t0 = not1(oam_bus.GOMO_OAM_DB4p.qp08_old());
    /* p31.AROP*/ wire _AROP_SPX5n_t0 = not1(oam_bus.BAXO_OAM_DB5p.qp08_old());
    /* p31.XATU*/ wire _XATU_SPX6n_t0 = not1(oam_bus.YZOS_OAM_DB6p.qp08_old());
    /* p31.BADY*/ wire _BADY_SPX7n_t0 = not1(oam_bus.DEPO_OAM_DB7p.qp08_old());

    /* p29.TYNO*/ wire _TYNO_t0 = nand3(sprite_fetcher.TOXE_SFETCH_S0p.qp17_new(), sprite_fetcher.SEBA_SFETCH_S1p_D5.qp17_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp17_new());
    /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn_t0  = or2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qn16_new(), _TYNO_t0);
    /* p29.WUTY*/ wire _WUTY_SPRITE_DONEp_t0 = not1(_VUSA_SPRITE_DONEn_t0);

    {
      /* p29.GUVA*/ wire GUVA_SPRITE0_GETp_t2 = nor2(_YDUG_STORE0_MATCHn_t0, GND);
      /* p29.FURO*/ wire FURO_SPRITE0_GETn_t2 = not1(GUVA_SPRITE0_GETp_t2);

      /* p29.DEZO*/ wire _DEZO_STORE0_SELn_t0 = nand4(_EDEN_SPRITE_COUNT0n_t0, _CYPY_SPRITE_COUNT1n_t0, _CAPE_SPRITE_COUNT2n_t0, _CAXU_SPRITE_COUNT3n_t0);
      /* p29.CEMY*/ wire CEMY_STORE0_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _DEZO_STORE0_SELn_t0);
      /* p29.DYHU*/ wire DYHU_STORE0_CLKn_t1 = not1(CEMY_STORE0_CLKp_t1);      /* p29.GENY*/ wire GENY_STORE0_CLKp_t1 = not1(DYHU_STORE0_CLKn_t1);
      /* p29.ENOB*/ wire ENOB_STORE0_CLKp_t1 = not1(DYHU_STORE0_CLKn_t1);
      /* p29.FUXU*/ wire FUXU_STORE0_CLKp_t1 = not1(DYHU_STORE0_CLKn_t1);

      /* p30.YGUS*/ sprite_store.YGUS_STORE0_I0n.dff8n(GENY_STORE0_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.YSOK*/ sprite_store.YSOK_STORE0_I1n.dff8n(GENY_STORE0_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.YZEP*/ sprite_store.YZEP_STORE0_I2n.dff8n(GENY_STORE0_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.WYTE*/ sprite_store.WYTE_STORE0_I3n.dff8n(GENY_STORE0_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.ZONY*/ sprite_store.ZONY_STORE0_I4n.dff8n(GENY_STORE0_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.YWAK*/ sprite_store.YWAK_STORE0_I5n.dff8n(GENY_STORE0_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());
      /* p30.GYHO*/ sprite_store.GYHO_STORE0_L0n.dff8n(ENOB_STORE0_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.CUFO*/ sprite_store.CUFO_STORE0_L1n.dff8n(ENOB_STORE0_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.BOZU*/ sprite_store.BOZU_STORE0_L2n.dff8n(ENOB_STORE0_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.FYHY*/ sprite_store.FYHY_STORE0_L3n.dff8n(ENOB_STORE0_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.EBOJ*/ sprite_store.EBOJ_STORE0_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, GUVA_SPRITE0_GETp_t2); // FIXME bad clock/signal
      /* p29.DYWE*/ wire _DYWE_STORE0_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.EBOJ_STORE0_RSTp.qp17_new());
      /* p29.DYNA*/ wire _DYNA_STORE0_RSTn_t2 = not1(_DYWE_STORE0_RSTp_t2);

      /*#p31.XEPE*/ sprite_store.XEPE_STORE0_X0p.dff9(FUXU_STORE0_CLKp_t1, _DYNA_STORE0_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.YLAH*/ sprite_store.YLAH_STORE0_X1p.dff9(FUXU_STORE0_CLKp_t1, _DYNA_STORE0_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.ZOLA*/ sprite_store.ZOLA_STORE0_X2p.dff9(FUXU_STORE0_CLKp_t1, _DYNA_STORE0_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.ZULU*/ sprite_store.ZULU_STORE0_X3p.dff9(FUXU_STORE0_CLKp_t1, _DYNA_STORE0_RSTn_t2, _YVOK_SPX3n_t0);
      /* p31.WELO*/ sprite_store.WELO_STORE0_X4p.dff9(FUXU_STORE0_CLKp_t1, _DYNA_STORE0_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.XUNY*/ sprite_store.XUNY_STORE0_X5p.dff9(FUXU_STORE0_CLKp_t1, _DYNA_STORE0_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.WOTE*/ sprite_store.WOTE_STORE0_X6p.dff9(FUXU_STORE0_CLKp_t1, _DYNA_STORE0_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.XAKO*/ sprite_store.XAKO_STORE0_X7p.dff9(FUXU_STORE0_CLKp_t1, _DYNA_STORE0_RSTn_t2, _BADY_SPX7n_t0);

      /* p30.ZETU*/ SPR_TRI_I0p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.YGUS_STORE0_I0n.qp08_new());
      /* p30.ZECE*/ SPR_TRI_I1p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.YSOK_STORE0_I1n.qp08_new());
      /* p30.ZAVE*/ SPR_TRI_I2p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.YZEP_STORE0_I2n.qp08_new());
      /* p30.WOKO*/ SPR_TRI_I3p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.WYTE_STORE0_I3n.qp08_new());
      /* p30.ZUMU*/ SPR_TRI_I4p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.ZONY_STORE0_I4n.qp08_new());
      /*#p30.ZEDY*/ SPR_TRI_I5p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.YWAK_STORE0_I5n.qp08_new());

      /* p30.WEHE*/ SPR_TRI_L0p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.GYHO_STORE0_L0n.qp08_new());
      /* p30.BUKY*/ SPR_TRI_L1p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.CUFO_STORE0_L1n.qp08_new());
      /* p30.AJAL*/ SPR_TRI_L2p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.BOZU_STORE0_L2n.qp08_new());
      /* p30.GOFO*/ SPR_TRI_L3p_t2.tri6_nn(FURO_SPRITE0_GETn_t2, sprite_store.FYHY_STORE0_L3n.qp08_new());
    }

    /* p29.WEFU*/ wire WEFU_STORE0_MATCH_t2 = not1(_YDUG_STORE0_MATCHn_t0);
    /* p29.GEZE*/ wire GEZE_STORE0_MATCH_OUT_t2 = or2(WEFU_STORE0_MATCH_t2, GND);
    {
      /* p29.ENUT*/ wire ENUT_SPRITE1_GETp_t2 = nor2(_DYDU_STORE1_MATCHn_t0, GEZE_STORE0_MATCH_OUT_t2);
      /* p29.DYDO*/ wire DYDO_SPRITE1_GETn_t2 = not1(ENUT_SPRITE1_GETp_t2);

      /* p29.CUVA*/ wire _CUVA_STORE1_SELn_t0 = nand4(_FYCU_SPRITE_COUNT0p_t0, _CYPY_SPRITE_COUNT1n_t0, _CAPE_SPRITE_COUNT2n_t0, _CAXU_SPRITE_COUNT3n_t0);
      /* p29.BYBY*/ wire BYBY_STORE1_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _CUVA_STORE1_SELn_t0);
      /* p29.BUCO*/ wire BUCO_STORE1_CLKn_t1 = not1(BYBY_STORE1_CLKp_t1);      /* p29.BYVY*/ wire BYVY_STORE1_CLKp_t1 = not1(BUCO_STORE1_CLKn_t1);
      /* p29.AHOF*/ wire AHOF_STORE1_CLKp_t1 = not1(BUCO_STORE1_CLKn_t1);
      /* p29.ASYS*/ wire ASYS_STORE1_CLKp_t1 = not1(BUCO_STORE1_CLKn_t1);

      /* p30.CADU*/ sprite_store.CADU_STORE1_I0n.dff8n(BYVY_STORE1_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.CEBO*/ sprite_store.CEBO_STORE1_I1n.dff8n(BYVY_STORE1_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.CUFA*/ sprite_store.CUFA_STORE1_I2n.dff8n(BYVY_STORE1_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.COMA*/ sprite_store.COMA_STORE1_I3n.dff8n(BYVY_STORE1_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.CUZA*/ sprite_store.CUZA_STORE1_I4n.dff8n(BYVY_STORE1_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.CAJY*/ sprite_store.CAJY_STORE1_I5n.dff8n(BYVY_STORE1_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());
      /* p30.AMES*/ sprite_store.AMES_STORE1_L0n.dff8n(AHOF_STORE1_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.AROF*/ sprite_store.AROF_STORE1_L1n.dff8n(AHOF_STORE1_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.ABOP*/ sprite_store.ABOP_STORE1_L2n.dff8n(AHOF_STORE1_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.ABUG*/ sprite_store.ABUG_STORE1_L3n.dff8n(AHOF_STORE1_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.CEDY*/ sprite_store.CEDY_STORE1_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, ENUT_SPRITE1_GETp_t2);
      /* p29.EFEV*/ wire _EFEV_STORE1_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.CEDY_STORE1_RSTp.qp17_new());
      /* p29.DOKU*/ wire _DOKU_STORE1_RSTn_t2 = not1(_EFEV_STORE1_RSTp_t2);

      /* p31.DANY*/ sprite_store.DANY_STORE1_X0p.dff9(ASYS_STORE1_CLKp_t1, _DOKU_STORE1_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.DUKO*/ sprite_store.DUKO_STORE1_X1p.dff9(ASYS_STORE1_CLKp_t1, _DOKU_STORE1_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.DESU*/ sprite_store.DESU_STORE1_X2p.dff9(ASYS_STORE1_CLKp_t1, _DOKU_STORE1_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.DAZO*/ sprite_store.DAZO_STORE1_X3p.dff9(ASYS_STORE1_CLKp_t1, _DOKU_STORE1_RSTn_t2, _YVOK_SPX3n_t0);
      /* p31.DAKE*/ sprite_store.DAKE_STORE1_X4p.dff9(ASYS_STORE1_CLKp_t1, _DOKU_STORE1_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.CESO*/ sprite_store.CESO_STORE1_X5p.dff9(ASYS_STORE1_CLKp_t1, _DOKU_STORE1_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.DYFU*/ sprite_store.DYFU_STORE1_X6p.dff9(ASYS_STORE1_CLKp_t1, _DOKU_STORE1_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.CUSY*/ sprite_store.CUSY_STORE1_X7p.dff9(ASYS_STORE1_CLKp_t1, _DOKU_STORE1_RSTn_t2, _BADY_SPX7n_t0);

      /*#p30.CUBO*/ SPR_TRI_I0p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.CADU_STORE1_I0n.qp08_new());
      /* p30.CELU*/ SPR_TRI_I1p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.CEBO_STORE1_I1n.qp08_new());
      /* p30.CEGY*/ SPR_TRI_I2p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.CUFA_STORE1_I2n.qp08_new());
      /* p30.BETY*/ SPR_TRI_I3p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.COMA_STORE1_I3n.qp08_new());
      /* p30.CYBY*/ SPR_TRI_I4p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.CUZA_STORE1_I4n.qp08_new());
      /* p30.BEMO*/ SPR_TRI_I5p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.CAJY_STORE1_I5n.qp08_new());

      /* p30.BYRO*/ SPR_TRI_L0p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.AMES_STORE1_L0n.qp08_new());
      /* p30.AHUM*/ SPR_TRI_L1p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.AROF_STORE1_L1n.qp08_new());
      /* p30.BACO*/ SPR_TRI_L2p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.ABOP_STORE1_L2n.qp08_new());
      /* p30.BEFE*/ SPR_TRI_L3p_t2.tri6_nn(DYDO_SPRITE1_GETn_t2, sprite_store.ABUG_STORE1_L3n.qp08_new());
    }

    /* p29.GAJA*/ wire GAJA_STORE1_MATCH_t2 = not1(_DYDU_STORE1_MATCHn_t0);
    /* p29.FUMA*/ wire FUMA_STORE1_MATCH_OUT_t2 = or2(GAJA_STORE1_MATCH_t2, GEZE_STORE0_MATCH_OUT_t2);
    {
      /* p29.EMOL*/ wire EMOL_SPRITE2_GETp_t2 = nor2(_DEGO_STORE2_MATCHn_t0, FUMA_STORE1_MATCH_OUT_t2);
      /* p29.FAME*/ wire FAME_SPRITE2_GETn_t2 = not1(EMOL_SPRITE2_GETp_t2);

      /* p29.GEBU*/ wire _GEBU_STORE2_SELn_t0 = nand4(_EDEN_SPRITE_COUNT0n_t0, _FONE_SPRITE_COUNT1p_t0, _CAPE_SPRITE_COUNT2n_t0, _CAXU_SPRITE_COUNT3n_t0);
      /* p29.WYXO*/ wire WYXO_STORE2_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _GEBU_STORE2_SELn_t0);
      /* p29.GYFO*/ wire GYFO_STORE2_CLKn_t1 = not1(WYXO_STORE2_CLKp_t1);
      /* p29.BUZY*/ wire BUZY_STORE2_CLKp_t1 = not1(GYFO_STORE2_CLKn_t1);
      /* p29.FUKE*/ wire FUKE_STORE2_CLKp_t1 = not1(GYFO_STORE2_CLKn_t1);
      /* p29.CACU*/ wire CACU_STORE2_CLKp_t1 = not1(GYFO_STORE2_CLKn_t1);

      /* p30.BUHE*/ sprite_store.BUHE_STORE2_I0n.dff8n(BUZY_STORE2_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.BYHU*/ sprite_store.BYHU_STORE2_I1n.dff8n(BUZY_STORE2_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.BECA*/ sprite_store.BECA_STORE2_I2n.dff8n(BUZY_STORE2_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.BULU*/ sprite_store.BULU_STORE2_I3n.dff8n(BUZY_STORE2_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.BUNA*/ sprite_store.BUNA_STORE2_I4n.dff8n(BUZY_STORE2_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.BOXA*/ sprite_store.BOXA_STORE2_I5n.dff8n(BUZY_STORE2_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());
      /* p30.YLOV*/ sprite_store.YLOV_STORE2_L0n.dff8n(FUKE_STORE2_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.XOSY*/ sprite_store.XOSY_STORE2_L1n.dff8n(FUKE_STORE2_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.XAZY*/ sprite_store.XAZY_STORE2_L2n.dff8n(FUKE_STORE2_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.YKUK*/ sprite_store.YKUK_STORE2_L3n.dff8n(FUKE_STORE2_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.EGAV*/ sprite_store.EGAV_STORE2_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, EMOL_SPRITE2_GETp_t2);
      /* p29.FOKO*/ wire _FOKO_STORE2_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.EGAV_STORE2_RSTp.qp17_new());
      /* p29.GAMY*/ wire _GAMY_STORE2_RSTn_t2 = not1(_FOKO_STORE2_RSTp_t2);

      /* p31.FOKA*/ sprite_store.FOKA_STORE2_X0p.dff9(CACU_STORE2_CLKp_t1, _GAMY_STORE2_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.FYTY*/ sprite_store.FYTY_STORE2_X1p.dff9(CACU_STORE2_CLKp_t1, _GAMY_STORE2_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.FUBY*/ sprite_store.FUBY_STORE2_X2p.dff9(CACU_STORE2_CLKp_t1, _GAMY_STORE2_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.GOXU*/ sprite_store.GOXU_STORE2_X3p.dff9(CACU_STORE2_CLKp_t1, _GAMY_STORE2_RSTn_t2, _YVOK_SPX3n_t0);
      /* p31.DUHY*/ sprite_store.DUHY_STORE2_X4p.dff9(CACU_STORE2_CLKp_t1, _GAMY_STORE2_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.EJUF*/ sprite_store.EJUF_STORE2_X5p.dff9(CACU_STORE2_CLKp_t1, _GAMY_STORE2_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.ENOR*/ sprite_store.ENOR_STORE2_X6p.dff9(CACU_STORE2_CLKp_t1, _GAMY_STORE2_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.DEPY*/ sprite_store.DEPY_STORE2_X7p.dff9(CACU_STORE2_CLKp_t1, _GAMY_STORE2_RSTn_t2, _BADY_SPX7n_t0);

      /* p30.CUBE*/ SPR_TRI_I0p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.BUHE_STORE2_I0n.qp08_new());
      /* p30.AFOZ*/ SPR_TRI_I1p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.BYHU_STORE2_I1n.qp08_new());
      /* p30.APON*/ SPR_TRI_I2p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.BECA_STORE2_I2n.qp08_new());
      /* p30.CUVU*/ SPR_TRI_I3p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.BULU_STORE2_I3n.qp08_new());
      /* p30.CYRO*/ SPR_TRI_I4p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.BUNA_STORE2_I4n.qp08_new());
      /* p30.AXEC*/ SPR_TRI_I5p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.BOXA_STORE2_I5n.qp08_new());

      /* p30.ZUKE*/ SPR_TRI_L0p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.YLOV_STORE2_L0n.qp08_new());
      /* p30.WERE*/ SPR_TRI_L1p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.XOSY_STORE2_L1n.qp08_new());
      /* p30.WUXE*/ SPR_TRI_L2p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.XAZY_STORE2_L2n.qp08_new());
      /* p30.ZABY*/ SPR_TRI_L3p_t2.tri6_nn(FAME_SPRITE2_GETn_t2, sprite_store.YKUK_STORE2_L3n.qp08_new());
    }

    /* p29.GUPO*/ wire GUPO_STORE2_MATCH_t2 = not1(_DEGO_STORE2_MATCHn_t0);
    /* p29.GEDE*/ wire GEDE_STORE2_MATCH_OUT_t2 = or2(GUPO_STORE2_MATCH_t2, FUMA_STORE1_MATCH_OUT_t2);
    {
      /* p29.GYFY*/ wire GYFY_SPRITE3_GETp_t2 = nor2(_YLOZ_STORE3_MATCHn_t0, GEDE_STORE2_MATCH_OUT_t2);
      /* p29.GYMA*/ wire GYMA_SPRITE3_GETn_t2 = not1(GYFY_SPRITE3_GETp_t2);

      /* p29.FOCO*/ wire _FOCO_STORE3_SELn_t0 = nand4(_FYCU_SPRITE_COUNT0p_t0, _FONE_SPRITE_COUNT1p_t0, _CAPE_SPRITE_COUNT2n_t0, _CAXU_SPRITE_COUNT3n_t0);
      /* p29.GUVE*/ wire GUVE_STORE3_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _FOCO_STORE3_SELn_t0);
      /* p29.GUSA*/ wire GUSA_STORE3_CLKn_t1 = not1(GUVE_STORE3_CLKp_t1);
      /* p29.FEKA*/ wire FEKA_STORE3_CLKp_t1 = not1(GUSA_STORE3_CLKn_t1);
      /* p29.XYHA*/ wire XYHA_STORE3_CLKp_t1 = not1(GUSA_STORE3_CLKn_t1);
      /* p29.YFAG*/ wire YFAG_STORE3_CLKp_t1 = not1(GUSA_STORE3_CLKn_t1);

      /* p30.DEVY*/ sprite_store.DEVY_STORE3_I0n.dff8n(FEKA_STORE3_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.DESE*/ sprite_store.DESE_STORE3_I1n.dff8n(FEKA_STORE3_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.DUNY*/ sprite_store.DUNY_STORE3_I2n.dff8n(FEKA_STORE3_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.DUHA*/ sprite_store.DUHA_STORE3_I3n.dff8n(FEKA_STORE3_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.DEBA*/ sprite_store.DEBA_STORE3_I4n.dff8n(FEKA_STORE3_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.DAFU*/ sprite_store.DAFU_STORE3_I5n.dff8n(FEKA_STORE3_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());
      /* p30.ZURO*/ sprite_store.ZURO_STORE3_L0n.dff8n(XYHA_STORE3_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.ZYLU*/ sprite_store.ZYLU_STORE3_L1n.dff8n(XYHA_STORE3_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.ZENE*/ sprite_store.ZENE_STORE3_L2n.dff8n(XYHA_STORE3_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.ZURY*/ sprite_store.ZURY_STORE3_L3n.dff8n(XYHA_STORE3_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.GOTA*/ sprite_store.GOTA_STORE3_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, GYFY_SPRITE3_GETp_t2);
      /* p29.GAKE*/ wire _GAKE_STORE3_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.GOTA_STORE3_RSTp.qp17_new());
      /* p29.WUPA*/ wire _WUPA_STORE3_RSTn_t2 = not1(_GAKE_STORE3_RSTp_t2);

      /* p31.XOLY*/ sprite_store.XOLY_STORE3_X0p.dff9(YFAG_STORE3_CLKp_t1, _WUPA_STORE3_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.XYBA*/ sprite_store.XYBA_STORE3_X1p.dff9(YFAG_STORE3_CLKp_t1, _WUPA_STORE3_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.XABE*/ sprite_store.XABE_STORE3_X2p.dff9(YFAG_STORE3_CLKp_t1, _WUPA_STORE3_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.XEKA*/ sprite_store.XEKA_STORE3_X3p.dff9(YFAG_STORE3_CLKp_t1, _WUPA_STORE3_RSTn_t2, _YVOK_SPX3n_t0);
      /* p31.XOMY*/ sprite_store.XOMY_STORE3_X4p.dff9(YFAG_STORE3_CLKp_t1, _WUPA_STORE3_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.WUHA*/ sprite_store.WUHA_STORE3_X5p.dff9(YFAG_STORE3_CLKp_t1, _WUPA_STORE3_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.WYNA*/ sprite_store.WYNA_STORE3_X6p.dff9(YFAG_STORE3_CLKp_t1, _WUPA_STORE3_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.WECO*/ sprite_store.WECO_STORE3_X7p.dff9(YFAG_STORE3_CLKp_t1, _WUPA_STORE3_RSTn_t2, _BADY_SPX7n_t0);

      /* p30.ENAP*/ SPR_TRI_I0p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.DEVY_STORE3_I0n.qp08_new());
      /* p30.DYGO*/ SPR_TRI_I1p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.DESE_STORE3_I1n.qp08_new());
      /* p30.DOWA*/ SPR_TRI_I2p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.DUNY_STORE3_I2n.qp08_new());
      /* p30.DONY*/ SPR_TRI_I3p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.DUHA_STORE3_I3n.qp08_new());
      /* p30.EFUD*/ SPR_TRI_I4p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.DEBA_STORE3_I4n.qp08_new());
      /* p30.DEZU*/ SPR_TRI_I5p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.DAFU_STORE3_I5n.qp08_new());

      /* p30.ZEXE*/ SPR_TRI_L0p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.ZURO_STORE3_L0n.qp08_new());
      /* p30.YWAV*/ SPR_TRI_L1p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.ZYLU_STORE3_L1n.qp08_new());
      /* p30.YJEM*/ SPR_TRI_L2p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.ZENE_STORE3_L2n.qp08_new());
      /* p30.ZYPO*/ SPR_TRI_L3p_t2.tri6_nn(GYMA_SPRITE3_GETn_t2, sprite_store.ZURY_STORE3_L3n.qp08_new());
    }

    /* p29.WEBO*/ wire WEBO_STORE3_MATCH_t2 = not1(_YLOZ_STORE3_MATCHn_t0);
    /* p29.WUTO*/ wire WUTO_STORE3_MATCH_OUT_t2 = or2(WEBO_STORE3_MATCH_t2, GEDE_STORE2_MATCH_OUT_t2);
    {
      /* p29.GONO*/ wire GONO_SPRITE4_GETp_t2 = nor2(_XAGE_STORE4_MATCHn_t0, WUTO_STORE3_MATCH_OUT_t2);
      /* p29.GOWO*/ wire GOWO_SPRITE4_GETn_t2 = not1(GONO_SPRITE4_GETp_t2);

      /* p29.CUPE*/ wire _CUPE_STORE4_SELn_t0 = nand4(_EDEN_SPRITE_COUNT0n_t0, _CYPY_SPRITE_COUNT1n_t0, _EKUD_SPRITE_COUNT2p_t0, _CAXU_SPRITE_COUNT3n_t0);
      /* p29.CECU*/ wire CECU_STORE4_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _CUPE_STORE4_SELn_t0);
      /* p29.DUKE*/ wire DUKE_STORE4_CLKn_t1 = not1(CECU_STORE4_CLKp_t1);
      /* p29.WOFO*/ wire WOFO_STORE4_CLKp_t1 = not1(DUKE_STORE4_CLKn_t1);
      /* p29.WYLU*/ wire WYLU_STORE4_CLKp_t1 = not1(DUKE_STORE4_CLKn_t1);
      /* p29.EWOT*/ wire EWOT_STORE4_CLKp_t1 = not1(DUKE_STORE4_CLKn_t1);

      /* p30.XAVE*/ sprite_store.XAVE_STORE4_I0n.dff8n(WYLU_STORE4_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.XEFE*/ sprite_store.XEFE_STORE4_I1n.dff8n(WYLU_STORE4_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.WANU*/ sprite_store.WANU_STORE4_I2n.dff8n(WYLU_STORE4_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.XABO*/ sprite_store.XABO_STORE4_I3n.dff8n(WYLU_STORE4_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.XEGE*/ sprite_store.XEGE_STORE4_I4n.dff8n(WYLU_STORE4_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.XYNU*/ sprite_store.XYNU_STORE4_I5n.dff8n(WYLU_STORE4_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());
      /* p30.CAPO*/ sprite_store.CAPO_STORE4_L0n.dff8n(EWOT_STORE4_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.CAJU*/ sprite_store.CAJU_STORE4_L1n.dff8n(EWOT_STORE4_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.CONO*/ sprite_store.CONO_STORE4_L2n.dff8n(EWOT_STORE4_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.CUMU*/ sprite_store.CUMU_STORE4_L3n.dff8n(EWOT_STORE4_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.XUDY*/ sprite_store.XUDY_STORE4_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, GONO_SPRITE4_GETp_t2);
      /* p29.WOHU*/ wire _WOHU_STORE4_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.XUDY_STORE4_RSTp.qp17_new());
      /* p29.WUNU*/ wire _WUNU_STORE4_RSTn_t2 = not1(_WOHU_STORE4_RSTp_t2);

      /* p31.WEDU*/ sprite_store.WEDU_STORE4_X0p.dff9(WOFO_STORE4_CLKp_t1, _WUNU_STORE4_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.YGAJ*/ sprite_store.YGAJ_STORE4_X1p.dff9(WOFO_STORE4_CLKp_t1, _WUNU_STORE4_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.ZYJO*/ sprite_store.ZYJO_STORE4_X2p.dff9(WOFO_STORE4_CLKp_t1, _WUNU_STORE4_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.XURY*/ sprite_store.XURY_STORE4_X3p.dff9(WOFO_STORE4_CLKp_t1, _WUNU_STORE4_RSTn_t2, _YVOK_SPX3n_t0);
      /* p31.YBED*/ sprite_store.YBED_STORE4_X4p.dff9(WOFO_STORE4_CLKp_t1, _WUNU_STORE4_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.ZALA*/ sprite_store.ZALA_STORE4_X5p.dff9(WOFO_STORE4_CLKp_t1, _WUNU_STORE4_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.WYDE*/ sprite_store.WYDE_STORE4_X6p.dff9(WOFO_STORE4_CLKp_t1, _WUNU_STORE4_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.XEPA*/ sprite_store.XEPA_STORE4_X7p.dff9(WOFO_STORE4_CLKp_t1, _WUNU_STORE4_RSTn_t2, _BADY_SPX7n_t0);

      /* p30.WUXU*/ SPR_TRI_I0p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.XAVE_STORE4_I0n.qp08_new());
      /* p30.WEPY*/ SPR_TRI_I1p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.XEFE_STORE4_I1n.qp08_new());
      /* p30.WERU*/ SPR_TRI_I2p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.WANU_STORE4_I2n.qp08_new());
      /* p30.XYRE*/ SPR_TRI_I3p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.XABO_STORE4_I3n.qp08_new());
      /* p30.WOXY*/ SPR_TRI_I4p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.XEGE_STORE4_I4n.qp08_new());
      /* p30.WAJA*/ SPR_TRI_I5p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.XYNU_STORE4_I5n.qp08_new());

      /* p30.BUCE*/ SPR_TRI_L0p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.CAPO_STORE4_L0n.qp08_new());
      /* p30.BEVY*/ SPR_TRI_L1p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.CAJU_STORE4_L1n.qp08_new());
      /* p30.BOVE*/ SPR_TRI_L2p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.CONO_STORE4_L2n.qp08_new());
      /* p30.BYDO*/ SPR_TRI_L3p_t2.tri6_nn(GOWO_SPRITE4_GETn_t2, sprite_store.CUMU_STORE4_L3n.qp08_new());
    }

    /* p29.WUNA*/ wire WUNA_STORE4_MATCH_t2 = not1(_XAGE_STORE4_MATCHn_t0);
    /* p29.XYLA*/ wire XYLA_STORE4_MATCH_OUT_t2 = or2(WUNA_STORE4_MATCH_t2, WUTO_STORE3_MATCH_OUT_t2);
    {
      /* p29.GEGA*/ wire GEGA_SPRITE5_GETp_t2 = nor2(_EGOM_STORE5_MATCHn_t0, XYLA_STORE4_MATCH_OUT_t2);
      /* p29.GYGY*/ wire GYGY_SPRITE5_GETn_t2 = not1(GEGA_SPRITE5_GETp_t2);

      /* p29.CUGU*/ wire _CUGU_STORE5_SELn_t0 = nand4(_FYCU_SPRITE_COUNT0p_t0, _CYPY_SPRITE_COUNT1n_t0, _EKUD_SPRITE_COUNT2p_t0, _CAXU_SPRITE_COUNT3n_t0);
      /* p29.CADO*/ wire CADO_STORE5_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _CUGU_STORE5_SELn_t0);
      /* p29.BEDE*/ wire BEDE_STORE5_CLKn_t1 = not1(CADO_STORE5_CLKp_t1);
      /* p29.CYLA*/ wire CYLA_STORE5_CLKp_t1 = not1(BEDE_STORE5_CLKn_t1);
      /* p29.DYMO*/ wire DYMO_STORE5_CLKp_t1 = not1(BEDE_STORE5_CLKn_t1);
      /* p29.BUCY*/ wire BUCY_STORE5_CLKp_t1 = not1(BEDE_STORE5_CLKn_t1);

      /* p30.EKOP*/ sprite_store.EKOP_STORE5_I0n.dff8n(DYMO_STORE5_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.ETYM*/ sprite_store.ETYM_STORE5_I1n.dff8n(DYMO_STORE5_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.GORU*/ sprite_store.GORU_STORE5_I2n.dff8n(DYMO_STORE5_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.EBEX*/ sprite_store.EBEX_STORE5_I3n.dff8n(DYMO_STORE5_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.ETAV*/ sprite_store.ETAV_STORE5_I4n.dff8n(DYMO_STORE5_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.EKAP*/ sprite_store.EKAP_STORE5_I5n.dff8n(DYMO_STORE5_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());
      /* p30.ACEP*/ sprite_store.ACEP_STORE5_L0n.dff8n(BUCY_STORE5_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.ABEG*/ sprite_store.ABEG_STORE5_L1n.dff8n(BUCY_STORE5_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.ABUX*/ sprite_store.ABUX_STORE5_L2n.dff8n(BUCY_STORE5_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.ANED*/ sprite_store.ANED_STORE5_L3n.dff8n(BUCY_STORE5_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.WAFY*/ sprite_store.WAFY_STORE5_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, GEGA_SPRITE5_GETp_t2);
      /* p29.FEVE*/ wire _FEVE_STORE5_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.WAFY_STORE5_RSTp.qp17_new());
      /* p29.EJAD*/ wire _EJAD_STORE5_RSTn_t2 = not1(_FEVE_STORE5_RSTp_t2);

      /* p31.FUSA*/ sprite_store.FUSA_STORE5_X0p.dff9(CYLA_STORE5_CLKp_t1, _EJAD_STORE5_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.FAXA*/ sprite_store.FAXA_STORE5_X1p.dff9(CYLA_STORE5_CLKp_t1, _EJAD_STORE5_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.FOZY*/ sprite_store.FOZY_STORE5_X2p.dff9(CYLA_STORE5_CLKp_t1, _EJAD_STORE5_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.FESY*/ sprite_store.FESY_STORE5_X3p.dff9(CYLA_STORE5_CLKp_t1, _EJAD_STORE5_RSTn_t2, _YVOK_SPX3n_t0);
      /* p31.CYWE*/ sprite_store.CYWE_STORE5_X4p.dff9(CYLA_STORE5_CLKp_t1, _EJAD_STORE5_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.DYBY*/ sprite_store.DYBY_STORE5_X5p.dff9(CYLA_STORE5_CLKp_t1, _EJAD_STORE5_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.DURY*/ sprite_store.DURY_STORE5_X6p.dff9(CYLA_STORE5_CLKp_t1, _EJAD_STORE5_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.CUVY*/ sprite_store.CUVY_STORE5_X7p.dff9(CYLA_STORE5_CLKp_t1, _EJAD_STORE5_RSTn_t2, _BADY_SPX7n_t0);

      /* p30.DOBO*/ SPR_TRI_I0p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.EKOP_STORE5_I0n.qp08_new());
      /* p30.DYNY*/ SPR_TRI_I1p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.ETYM_STORE5_I1n.qp08_new());
      /* p30.WAGA*/ SPR_TRI_I2p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.GORU_STORE5_I2n.qp08_new());
      /* p30.DUZA*/ SPR_TRI_I3p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.EBEX_STORE5_I3n.qp08_new());
      /* p30.DALY*/ SPR_TRI_I4p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.ETAV_STORE5_I4n.qp08_new());
      /* p30.DALO*/ SPR_TRI_I5p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.EKAP_STORE5_I5n.qp08_new());

      /* p30.BACE*/ SPR_TRI_L0p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.ACEP_STORE5_L0n.qp08_new());
      /* p30.BUJA*/ SPR_TRI_L1p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.ABEG_STORE5_L1n.qp08_new());
      /* p30.BODU*/ SPR_TRI_L2p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.ABUX_STORE5_L2n.qp08_new());
      /* p30.AWAT*/ SPR_TRI_L3p_t2.tri6_nn(GYGY_SPRITE5_GETn_t2, sprite_store.ANED_STORE5_L3n.qp08_new());
    }

    /* p29.GABA*/ wire GABA_STORE5_MATCH_t2 = not1(_EGOM_STORE5_MATCHn_t0);
    /* p29.WEJA*/ wire WEJA_STORE5_MATCH_OUT_t2 = or2(GABA_STORE5_MATCH_t2, XYLA_STORE4_MATCH_OUT_t2);
    {
      /* p29.XOJA*/ wire XOJA_SPRITE6_GETp_t2 = nor2(_YBEZ_STORE6_MATCHn_t0, WEJA_STORE5_MATCH_OUT_t2);
      /* p29.XYME*/ wire XYME_SPRITE6_GETn_t2 = not1(XOJA_SPRITE6_GETp_t2);

      /* p29.WOMU*/ wire _WOMU_STORE6_SELn_t0 = nand4(_EDEN_SPRITE_COUNT0n_t0, _FONE_SPRITE_COUNT1p_t0, _EKUD_SPRITE_COUNT2p_t0, _CAXU_SPRITE_COUNT3n_t0);
      /* p29.XUJO*/ wire XUJO_STORE6_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _WOMU_STORE6_SELn_t0);
      /* p29.WEKA*/ wire WEKA_STORE6_CLKn_t1 = not1(XUJO_STORE6_CLKp_t1);
      /* p29.ZAPE*/ wire ZAPE_STORE6_CLKp_t1 = not1(WEKA_STORE6_CLKn_t1);
      /* p29.WUSE*/ wire WUSE_STORE6_CLKp_t1 = not1(WEKA_STORE6_CLKn_t1);
      /* p29.ZURU*/ wire ZURU_STORE6_CLKp_t1 = not1(WEKA_STORE6_CLKn_t1);

      /* p30.GABO*/ sprite_store.GABO_STORE6_I0n.dff8n(WUSE_STORE6_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.GACY*/ sprite_store.GACY_STORE6_I1n.dff8n(WUSE_STORE6_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.FOGO*/ sprite_store.FOGO_STORE6_I2n.dff8n(WUSE_STORE6_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.GOHU*/ sprite_store.GOHU_STORE6_I3n.dff8n(WUSE_STORE6_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.FOXY*/ sprite_store.FOXY_STORE6_I4n.dff8n(WUSE_STORE6_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.GECU*/ sprite_store.GECU_STORE6_I5n.dff8n(WUSE_STORE6_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());
      /* p30.ZUMY*/ sprite_store.ZUMY_STORE6_L0n.dff8n(ZURU_STORE6_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.ZAFU*/ sprite_store.ZAFU_STORE6_L1n.dff8n(ZURU_STORE6_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.ZEXO*/ sprite_store.ZEXO_STORE6_L2n.dff8n(ZURU_STORE6_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.ZUBE*/ sprite_store.ZUBE_STORE6_L3n.dff8n(ZURU_STORE6_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.WOMY*/ sprite_store.WOMY_STORE6_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, XOJA_SPRITE6_GETp_t2);
      /* p29.WACY*/ wire _WACY_STORE6_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.WOMY_STORE6_RSTp.qp17_new());
      /* p29.XAHO*/ wire _XAHO_STORE6_RSTn_t2 = not1(_WACY_STORE6_RSTp_t2);

      /* p31.YCOL*/ sprite_store.YCOL_STORE6_X0p.dff9(ZAPE_STORE6_CLKp_t1, _XAHO_STORE6_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.YRAC*/ sprite_store.YRAC_STORE6_X1p.dff9(ZAPE_STORE6_CLKp_t1, _XAHO_STORE6_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.YMEM*/ sprite_store.YMEM_STORE6_X2p.dff9(ZAPE_STORE6_CLKp_t1, _XAHO_STORE6_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.YVAG*/ sprite_store.YVAG_STORE6_X3p.dff9(ZAPE_STORE6_CLKp_t1, _XAHO_STORE6_RSTn_t2, _YVOK_SPX3n_t0);
      /* p31.ZOLY*/ sprite_store.ZOLY_STORE6_X4p.dff9(ZAPE_STORE6_CLKp_t1, _XAHO_STORE6_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.ZOGO*/ sprite_store.ZOGO_STORE6_X5p.dff9(ZAPE_STORE6_CLKp_t1, _XAHO_STORE6_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.ZECU*/ sprite_store.ZECU_STORE6_X6p.dff9(ZAPE_STORE6_CLKp_t1, _XAHO_STORE6_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.ZESA*/ sprite_store.ZESA_STORE6_X7p.dff9(ZAPE_STORE6_CLKp_t1, _XAHO_STORE6_RSTn_t2, _BADY_SPX7n_t0);

      /* p30.WATO*/ SPR_TRI_I0p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.GABO_STORE6_I0n.qp08_new());
      /* p30.WYWY*/ SPR_TRI_I1p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.GACY_STORE6_I1n.qp08_new());
      /* p30.EZOC*/ SPR_TRI_I2p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.FOGO_STORE6_I2n.qp08_new());
      /* p30.WABO*/ SPR_TRI_I3p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.GOHU_STORE6_I3n.qp08_new());
      /* p30.ELYC*/ SPR_TRI_I4p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.FOXY_STORE6_I4n.qp08_new());
      /* p30.WOCY*/ SPR_TRI_I5p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.GECU_STORE6_I5n.qp08_new());

      /* p30.YBUK*/ SPR_TRI_L0p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.ZUMY_STORE6_L0n.qp08_new());
      /* p30.YKOZ*/ SPR_TRI_L1p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.ZAFU_STORE6_L1n.qp08_new());
      /* p30.ZYTO*/ SPR_TRI_L2p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.ZEXO_STORE6_L2n.qp08_new());
      /* p30.ZUDO*/ SPR_TRI_L3p_t2.tri6_nn(XYME_SPRITE6_GETn_t2, sprite_store.ZUBE_STORE6_L3n.qp08_new());
    }

    /* p29.WASE*/ wire WASE_STORE6_MATCH_t2 = not1(_YBEZ_STORE6_MATCHn_t0);
    /* p29.WYLA*/ wire WYLA_STORE6_MATCH_OUT_t2 = or2(WASE_STORE6_MATCH_t2, WEJA_STORE5_MATCH_OUT_t2);
    {
      /* p29.GUTU*/ wire GUTU_SPRITE7_GETp_t2 = nor2(_DYKA_STORE7_MATCHn_t0, WYLA_STORE6_MATCH_OUT_t2);
      /* p29.GUGY*/ wire GUGY_SPRITE7_GETn_t2 = not1(GUTU_SPRITE7_GETp_t2);

      /* p29.GUNA*/ wire _GUNA_STORE7_SELn_t0 = nand4(_FYCU_SPRITE_COUNT0p_t0, _FONE_SPRITE_COUNT1p_t0, _EKUD_SPRITE_COUNT2p_t0, _CAXU_SPRITE_COUNT3n_t0);
      /* p29.GAPE*/ wire GAPE_STORE7_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _GUNA_STORE7_SELn_t0);
      /* p29.GYVO*/ wire GYVO_STORE7_CLKn_t1 = not1(GAPE_STORE7_CLKp_t1);
      /* p29.GECY*/ wire GECY_STORE7_CLKp_t1 = not1(GYVO_STORE7_CLKn_t1);
      /* p29.FEFO*/ wire FEFO_STORE7_CLKp_t1 = not1(GYVO_STORE7_CLKn_t1);
      /* p29.WABE*/ wire WABE_STORE7_CLKp_t1 = not1(GYVO_STORE7_CLKn_t1);

      /* p30.GULE*/ sprite_store.GULE_STORE7_I0n.dff8n(FEFO_STORE7_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.GYNO*/ sprite_store.GYNO_STORE7_I1n.dff8n(FEFO_STORE7_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.FEFA*/ sprite_store.FEFA_STORE7_I2n.dff8n(FEFO_STORE7_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.FYSU*/ sprite_store.FYSU_STORE7_I3n.dff8n(FEFO_STORE7_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.GESY*/ sprite_store.GESY_STORE7_I4n.dff8n(FEFO_STORE7_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.FUZO*/ sprite_store.FUZO_STORE7_I5n.dff8n(FEFO_STORE7_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());
      /* p30.XYNA*/ sprite_store.XYNA_STORE7_L0n.dff8n(WABE_STORE7_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.YGUM*/ sprite_store.YGUM_STORE7_L1n.dff8n(WABE_STORE7_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.XAKU*/ sprite_store.XAKU_STORE7_L2n.dff8n(WABE_STORE7_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.XYGO*/ sprite_store.XYGO_STORE7_L3n.dff8n(WABE_STORE7_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.WAPO*/ sprite_store.WAPO_STORE7_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, GUTU_SPRITE7_GETp_t2);
      /* p29.GUKY*/ wire _GUKY_STORE7_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.WAPO_STORE7_RSTp.qp17_new());
      /* p29.GAFY*/ wire _GAFY_STORE7_RSTn_t2 = not1(_GUKY_STORE7_RSTp_t2);

      /* p31.ERAZ*/ sprite_store.ERAZ_STORE7_X0p.dff9(GECY_STORE7_CLKp_t1, _GAFY_STORE7_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.EPUM*/ sprite_store.EPUM_STORE7_X1p.dff9(GECY_STORE7_CLKp_t1, _GAFY_STORE7_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.EROL*/ sprite_store.EROL_STORE7_X2p.dff9(GECY_STORE7_CLKp_t1, _GAFY_STORE7_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.EHYN*/ sprite_store.EHYN_STORE7_X3p.dff9(GECY_STORE7_CLKp_t1, _GAFY_STORE7_RSTn_t2, _YVOK_SPX3n_t0);
      /* p31.FAZU*/ sprite_store.FAZU_STORE7_X4p.dff9(GECY_STORE7_CLKp_t1, _GAFY_STORE7_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.FAXE*/ sprite_store.FAXE_STORE7_X5p.dff9(GECY_STORE7_CLKp_t1, _GAFY_STORE7_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.EXUK*/ sprite_store.EXUK_STORE7_X6p.dff9(GECY_STORE7_CLKp_t1, _GAFY_STORE7_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.FEDE*/ sprite_store.FEDE_STORE7_X7p.dff9(GECY_STORE7_CLKp_t1, _GAFY_STORE7_RSTn_t2, _BADY_SPX7n_t0);

      /* p30.WAKO*/ SPR_TRI_I0p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.GULE_STORE7_I0n.qp08_new());
      /* p30.WYGO*/ SPR_TRI_I1p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.GYNO_STORE7_I1n.qp08_new());
      /* p30.ELEP*/ SPR_TRI_I2p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.FEFA_STORE7_I2n.qp08_new());
      /* p30.ETAD*/ SPR_TRI_I3p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.FYSU_STORE7_I3n.qp08_new());
      /* p30.WABA*/ SPR_TRI_I4p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.GESY_STORE7_I4n.qp08_new());
      /* p30.EVYT*/ SPR_TRI_I5p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.FUZO_STORE7_I5n.qp08_new());

      /* p30.WAXE*/ SPR_TRI_L0p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.XYNA_STORE7_L0n.qp08_new());
      /* p30.YPOZ*/ SPR_TRI_L1p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.YGUM_STORE7_L1n.qp08_new());
      /* p30.WABU*/ SPR_TRI_L2p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.XAKU_STORE7_L2n.qp08_new());
      /* p30.WANA*/ SPR_TRI_L3p_t2.tri6_nn(GUGY_SPRITE7_GETn_t2, sprite_store.XYGO_STORE7_L3n.qp08_new());
    }

    /* p29.GYTE*/ wire GYTE_STORE7_MATCH_t2 = not1(_DYKA_STORE7_MATCHn_t0);
    /* p29.FAVO*/ wire FAVO_STORE7_MATCH_OUT_t2 = or2(GYTE_STORE7_MATCH_t2, WYLA_STORE6_MATCH_OUT_t2);
    {
      /* p29.FOXA*/ wire FOXA_SPRITE8_GETp_t2 = nor2(_EFYL_STORE8_MATCHn_t0, FAVO_STORE7_MATCH_OUT_t2);
      /* p29.DENY*/ wire DENY_SPRITE8_GETn_t2 = not1(FOXA_SPRITE8_GETp_t2);

      /* p29.DEWY*/ wire _DEWY_STORE8_SELn_t0 = nand4(_EDEN_SPRITE_COUNT0n_t0, _CYPY_SPRITE_COUNT1n_t0, _CAPE_SPRITE_COUNT2n_t0, _ELYG_SPRITE_COUNT3p_t0);
      /* p29.CAHO*/ wire _CAHO_STORE8_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _DEWY_STORE8_SELn_t0);
      /* p29.BUKA*/ wire _BUKA_STORE8_CLKn_t1 = not1(_CAHO_STORE8_CLKp_t1);
      /* p29.CEXU*/ wire _CEXU_STORE8_CLKp_t1 = not1(_BUKA_STORE8_CLKn_t1);
      /* p29.AKOL*/ wire _AKOL_STORE8_CLKp_t1 = not1(_BUKA_STORE8_CLKn_t1);
      /* p29.BYMY*/ wire _BYMY_STORE8_CLKp_t1 = not1(_BUKA_STORE8_CLKn_t1);

      /* p30.AXUV*/ sprite_store.AXUV_STORE8_I0n.dff8n(_AKOL_STORE8_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.BADA*/ sprite_store.BADA_STORE8_I1n.dff8n(_AKOL_STORE8_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.APEV*/ sprite_store.APEV_STORE8_I2n.dff8n(_AKOL_STORE8_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.BADO*/ sprite_store.BADO_STORE8_I3n.dff8n(_AKOL_STORE8_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.BEXY*/ sprite_store.BEXY_STORE8_I4n.dff8n(_AKOL_STORE8_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.BYHE*/ sprite_store.BYHE_STORE8_I5n.dff8n(_AKOL_STORE8_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());

      /* p30.AZAP*/ sprite_store.AZAP_STORE8_L0n.dff8n(_BYMY_STORE8_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.AFYX*/ sprite_store.AFYX_STORE8_L1n.dff8n(_BYMY_STORE8_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.AFUT*/ sprite_store.AFUT_STORE8_L2n.dff8n(_BYMY_STORE8_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.AFYM*/ sprite_store.AFYM_STORE8_L3n.dff8n(_BYMY_STORE8_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.EXUQ*/ sprite_store.EXUQ_STORE8_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, FOXA_SPRITE8_GETp_t2);
      /* p29.GORO*/ wire _GORO_STORE8_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.EXUQ_STORE8_RSTp.qp17_new());
      /* p29.WUZO*/ wire _WUZO_STORE8_RSTn_t2 = not1(_GORO_STORE8_RSTp_t2);

      /* p31.EZUF*/ sprite_store.EZUF_STORE8_X0p.dff9(_CEXU_STORE8_CLKp_t1, _WUZO_STORE8_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.ENAD*/ sprite_store.ENAD_STORE8_X1p.dff9(_CEXU_STORE8_CLKp_t1, _WUZO_STORE8_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.EBOW*/ sprite_store.EBOW_STORE8_X2p.dff9(_CEXU_STORE8_CLKp_t1, _WUZO_STORE8_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.FYCA*/ sprite_store.FYCA_STORE8_X3p.dff9(_CEXU_STORE8_CLKp_t1, _WUZO_STORE8_RSTn_t2, _BADY_SPX7n_t0);
      /* p31.GAVY*/ sprite_store.GAVY_STORE8_X4p.dff9(_CEXU_STORE8_CLKp_t1, _WUZO_STORE8_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.GYPU*/ sprite_store.GYPU_STORE8_X5p.dff9(_CEXU_STORE8_CLKp_t1, _WUZO_STORE8_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.GADY*/ sprite_store.GADY_STORE8_X6p.dff9(_CEXU_STORE8_CLKp_t1, _WUZO_STORE8_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.GAZA*/ sprite_store.GAZA_STORE8_X7p.dff9(_CEXU_STORE8_CLKp_t1, _WUZO_STORE8_RSTn_t2, _YVOK_SPX3n_t0);

      /* p30.APOC*/ SPR_TRI_I0p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.AXUV_STORE8_I0n.qp08_new());
      /* p30.AKYH*/ SPR_TRI_I1p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.BADA_STORE8_I1n.qp08_new());
      /* p30.AFEN*/ SPR_TRI_I2p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.APEV_STORE8_I2n.qp08_new());
      /* p30.APYV*/ SPR_TRI_I3p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.BADO_STORE8_I3n.qp08_new());
      /* p30.APOB*/ SPR_TRI_I4p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.BEXY_STORE8_I4n.qp08_new());
      /* p30.ADYB*/ SPR_TRI_I5p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.BYHE_STORE8_I5n.qp08_new());

      /* p30.BOSO*/ SPR_TRI_L0p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.AZAP_STORE8_L0n.qp08_new());
      /* p30.BAZU*/ SPR_TRI_L1p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.AFYX_STORE8_L1n.qp08_new());
      /* p30.AHAC*/ SPR_TRI_L2p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.AFUT_STORE8_L2n.qp08_new());
      /* p30.BUJY*/ SPR_TRI_L3p_t2.tri6_nn(DENY_SPRITE8_GETn_t2, sprite_store.AFYM_STORE8_L3n.qp08_new());
    }

    /* p29.GEKE*/ wire GEKE_STORE8_MATCH_t2 = not1(_EFYL_STORE8_MATCHn_t0);
    /* p29.GYGA*/ wire GYGA_STORE8_MATCH_OUT_t2 = or2(GEKE_STORE8_MATCH_t2, FAVO_STORE7_MATCH_OUT_t2);
    {
      /* p29.GUZE*/ wire GUZE_SPRITE9_GETp_t2 = nor2(_YGEM_STORE9_MATCHn_t0, GYGA_STORE8_MATCH_OUT_t2);
      /* p29.FADO*/ wire FADO_SPRITE9_GETn_t2 = not1(GUZE_SPRITE9_GETp_t2);

      /* p29.DOGU*/ wire _DOGU_STORE9_SELn_t0 = nand4(_FYCU_SPRITE_COUNT0p_t0, _CYPY_SPRITE_COUNT1n_t0, _CAPE_SPRITE_COUNT2n_t0, _ELYG_SPRITE_COUNT3p_t0);
      /* p29.CATO*/ wire _CATO_STORE9_CLKp_t1 = or2(_DYTY_STORE_ENn_xxCDxxGH_t1, _DOGU_STORE9_SELn_t0);
      /* p29.DECU*/ wire _DECU_STORE9_CLKn_t1 = not1(_CATO_STORE9_CLKp_t1);
      /* p29.WEME*/ wire _WEME_STORE9_CLKp_t1 = not1(_DECU_STORE9_CLKn_t1);
      /* p29.WUFA*/ wire _WUFA_STORE9_CLKp_t1 = not1(_DECU_STORE9_CLKn_t1);
      /* p29.FAKA*/ wire _FAKA_STORE9_CLKp_t1 = not1(_DECU_STORE9_CLKn_t1);

      /* p30.YBER*/ sprite_store.YBER_STORE9_I0n.dff8n(_WUFA_STORE9_CLKp_t1, SPR_TRI_I0p_t2.to_wire_new());
      /* p30.YZOR*/ sprite_store.YZOR_STORE9_I1n.dff8n(_WUFA_STORE9_CLKp_t1, SPR_TRI_I1p_t2.to_wire_new());
      /* p30.XYFE*/ sprite_store.XYFE_STORE9_I2n.dff8n(_WUFA_STORE9_CLKp_t1, SPR_TRI_I2p_t2.to_wire_new());
      /* p30.XOTU*/ sprite_store.XOTU_STORE9_I3n.dff8n(_WUFA_STORE9_CLKp_t1, SPR_TRI_I3p_t2.to_wire_new());
      /* p30.XUTE*/ sprite_store.XUTE_STORE9_I4n.dff8n(_WUFA_STORE9_CLKp_t1, SPR_TRI_I4p_t2.to_wire_new());
      /* p30.XUFO*/ sprite_store.XUFO_STORE9_I5n.dff8n(_WUFA_STORE9_CLKp_t1, SPR_TRI_I5p_t2.to_wire_new());

      /* p30.CANA*/ sprite_store.CANA_STORE9_L0n.dff8n(_FAKA_STORE9_CLKp_t1, SPR_TRI_L0p_t2.to_wire_new());
      /* p30.FOFO*/ sprite_store.FOFO_STORE9_L1n.dff8n(_FAKA_STORE9_CLKp_t1, SPR_TRI_L1p_t2.to_wire_new());
      /* p30.DYSY*/ sprite_store.DYSY_STORE9_L2n.dff8n(_FAKA_STORE9_CLKp_t1, SPR_TRI_L2p_t2.to_wire_new());
      /* p30.DEWU*/ sprite_store.DEWU_STORE9_L3n.dff8n(_FAKA_STORE9_CLKp_t1, SPR_TRI_L3p_t2.to_wire_new());

      /* p29.FONO*/ sprite_store.FONO_STORE9_RSTp.dff17(_WUTY_SPRITE_DONEp_t0, _BYVA_LINE_RSTn_t2, GUZE_SPRITE9_GETp_t2);
      /* p29.DUBU*/ wire _DUBU_STORE9_RSTp_t2 = or2(_DYBA_LINE_RSTp_t2, sprite_store.FONO_STORE9_RSTp.qp17_new());
      /* p29.DOSY*/ wire _DOSY_STORE9_RSTn_t2 = not1(_DUBU_STORE9_RSTp_t2);

      /* p31.XUVY*/ sprite_store.XUVY_STORE9_X0p.dff9(_WEME_STORE9_CLKp_t1, _DOSY_STORE9_RSTn_t2, _ZAGO_SPX0n_t0);
      /* p31.XERE*/ sprite_store.XERE_STORE9_X1p.dff9(_WEME_STORE9_CLKp_t1, _DOSY_STORE9_RSTn_t2, _ZOCY_SPX1n_t0);
      /* p31.XUZO*/ sprite_store.XUZO_STORE9_X2p.dff9(_WEME_STORE9_CLKp_t1, _DOSY_STORE9_RSTn_t2, _YPUR_SPX2n_t0);
      /* p31.XEXA*/ sprite_store.XEXA_STORE9_X3p.dff9(_WEME_STORE9_CLKp_t1, _DOSY_STORE9_RSTn_t2, _YVOK_SPX3n_t0);
      /* p31.YPOD*/ sprite_store.YPOD_STORE9_X4p.dff9(_WEME_STORE9_CLKp_t1, _DOSY_STORE9_RSTn_t2, _COSE_SPX4n_t0);
      /* p31.YROP*/ sprite_store.YROP_STORE9_X5p.dff9(_WEME_STORE9_CLKp_t1, _DOSY_STORE9_RSTn_t2, _AROP_SPX5n_t0);
      /* p31.YNEP*/ sprite_store.YNEP_STORE9_X6p.dff9(_WEME_STORE9_CLKp_t1, _DOSY_STORE9_RSTn_t2, _XATU_SPX6n_t0);
      /* p31.YZOF*/ sprite_store.YZOF_STORE9_X7p.dff9(_WEME_STORE9_CLKp_t1, _DOSY_STORE9_RSTn_t2, _BADY_SPX7n_t0);

      /* p30.ZARO*/ SPR_TRI_I0p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.YBER_STORE9_I0n.qp08_new());
      /* p30.ZOJY*/ SPR_TRI_I1p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.YZOR_STORE9_I1n.qp08_new());
      /* p30.YNEV*/ SPR_TRI_I2p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.XYFE_STORE9_I2n.qp08_new());
      /* p30.XYRA*/ SPR_TRI_I3p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.XOTU_STORE9_I3n.qp08_new());
      /* p30.YRAD*/ SPR_TRI_I4p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.XUTE_STORE9_I4n.qp08_new());
      /* p30.YHAL*/ SPR_TRI_I5p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.XUFO_STORE9_I5n.qp08_new());

      /* p30.BYME*/ SPR_TRI_L0p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.CANA_STORE9_L0n.qp08_new());
      /* p30.GATE*/ SPR_TRI_L1p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.FOFO_STORE9_L1n.qp08_new());
      /* p30.COHO*/ SPR_TRI_L2p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.DYSY_STORE9_L2n.qp08_new());
      /* p30.CAWO*/ SPR_TRI_L3p_t2.tri6_nn(FADO_SPRITE9_GETn_t2, sprite_store.DEWU_STORE9_L3n.qp08_new());
    }
  }










  //----------------------------------------
  // VRAM interface

  Bus2 BUS_VRAM_Dp_in[8];
  Bus2 BUS_VRAM_Dp_out[8];

  {
    /*#p25.SYCY*/ wire _SYCY_DBG_CLOCKn_t0 = not1(_UNOR_MODE_DBG2p_t0);
    /*#p25.SOTO*/ SOTO_DBG_VRAMp.dff17(_SYCY_DBG_CLOCKn_t0, _CUNU_SYS_RSTn_t0, SOTO_DBG_VRAMp.qn16_old());
  }

  {

    Bus2 BUS_VRAM_An_t2[13];

    //----------------------------------------
    // Map scroll adder

    /*#p26.FAFO*/ wire _FAFO_TILE_Y0S_t0 = add_s(lcd_reg.MUWY_LY0p.qp17_old(), pix_pipe.GAVE_SCY0n.qn08_new(), 0);
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0C_t0 = add_c(lcd_reg.MUWY_LY0p.qp17_old(), pix_pipe.GAVE_SCY0n.qn08_new(), 0);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1S_t0 = add_s(lcd_reg.MYRO_LY1p.qp17_old(), pix_pipe.FYMO_SCY1n.qn08_new(), _FAFO_TILE_Y0C_t0);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1C_t0 = add_c(lcd_reg.MYRO_LY1p.qp17_old(), pix_pipe.FYMO_SCY1n.qn08_new(), _FAFO_TILE_Y0C_t0);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2S_t0 = add_s(lcd_reg.LEXA_LY2p.qp17_old(), pix_pipe.FEZU_SCY2n.qn08_new(), _EMUX_TILE_Y1C_t0);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2C_t0 = add_c(lcd_reg.LEXA_LY2p.qp17_old(), pix_pipe.FEZU_SCY2n.qn08_new(), _EMUX_TILE_Y1C_t0);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0S_t0  = add_s(lcd_reg.LYDO_LY3p.qp17_old(), pix_pipe.FUJO_SCY3n.qn08_new(), _ECAB_TILE_Y2C_t0);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0C_t0  = add_c(lcd_reg.LYDO_LY3p.qp17_old(), pix_pipe.FUJO_SCY3n.qn08_new(), _ECAB_TILE_Y2C_t0);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1S_t0  = add_s(lcd_reg.LOVU_LY4p.qp17_old(), pix_pipe.DEDE_SCY4n.qn08_new(), _ETAM_MAP_Y0C_t0);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1C_t0  = add_c(lcd_reg.LOVU_LY4p.qp17_old(), pix_pipe.DEDE_SCY4n.qn08_new(), _ETAM_MAP_Y0C_t0);
    /* p26.DABA*/ wire _DABA_MAP_Y2S_t0  = add_s(lcd_reg.LEMA_LY5p.qp17_old(), pix_pipe.FOTY_SCY5n.qn08_new(), _DOTO_MAP_Y1C_t0);
    /* p26.DABA*/ wire _DABA_MAP_Y2C_t0  = add_c(lcd_reg.LEMA_LY5p.qp17_old(), pix_pipe.FOTY_SCY5n.qn08_new(), _DOTO_MAP_Y1C_t0);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3S_t0  = add_s(lcd_reg.MATO_LY6p.qp17_old(), pix_pipe.FOHA_SCY6n.qn08_new(), _DABA_MAP_Y2C_t0);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3C_t0  = add_c(lcd_reg.MATO_LY6p.qp17_old(), pix_pipe.FOHA_SCY6n.qn08_new(), _DABA_MAP_Y2C_t0);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4S_t0  = add_s(lcd_reg.LAFO_LY7p.qp17_old(), pix_pipe.FUNY_SCY7n.qn08_new(), _EFYK_MAP_Y3C_t0);
    ///* p26.EJOK*/ wire _EJOK_MAP_Y4C_t0  = add_c(lcd_reg.LAFO_LY7p.qp17_old(), pix_pipe.FUNY_SCY7n.qn08_new(), _EFYK_MAP_Y3C_t0);

    ///*#p26.ATAD*/ wire _ATAD_TILE_X0S_t0 = add_s(pix_pipe.XEHO_PX0p.qp17_new(), pix_pipe.DATY_SCX0n.qn08_new(), 0);
    /*#p26.ATAD*/ wire _ATAD_TILE_X0C_t0 = add_c(pix_pipe.XEHO_PX0p.qp17_new(), pix_pipe.DATY_SCX0n.qn08_new(), 0);
    ///* p26.BEHU*/ wire _BEHU_TILE_X1S_t0 = add_s(pix_pipe.SAVY_PX1p.qp17_new(), pix_pipe.DUZU_SCX1n.qn08_new(), _ATAD_TILE_X0C_t0);
    /* p26.BEHU*/ wire _BEHU_TILE_X1C_t0 = add_c(pix_pipe.SAVY_PX1p.qp17_new(), pix_pipe.DUZU_SCX1n.qn08_new(), _ATAD_TILE_X0C_t0);
    ///* p26.APYH*/ wire _APYH_TILE_X2S_t0 = add_s(pix_pipe.XODU_PX2p.qp17_new(), pix_pipe.CYXU_SCX2n.qn08_new(), _BEHU_TILE_X1C_t0);
    /* p26.APYH*/ wire _APYH_TILE_X2C_t0 = add_c(pix_pipe.XODU_PX2p.qp17_new(), pix_pipe.CYXU_SCX2n.qn08_new(), _BEHU_TILE_X1C_t0);
    /* p26.BABE*/ wire _BABE_MAP_X0S_t0  = add_s(pix_pipe.XYDO_PX3p.qp17_new(), pix_pipe.GUBO_SCX3n.qn08_new(), _APYH_TILE_X2C_t0);
    /* p26.BABE*/ wire _BABE_MAP_X0C_t0  = add_c(pix_pipe.XYDO_PX3p.qp17_new(), pix_pipe.GUBO_SCX3n.qn08_new(), _APYH_TILE_X2C_t0);
    /* p26.ABOD*/ wire _ABOD_MAP_X1S_t0  = add_s(pix_pipe.TUHU_PX4p.qp17_new(), pix_pipe.BEMY_SCX4n.qn08_new(), _BABE_MAP_X0C_t0);
    /* p26.ABOD*/ wire _ABOD_MAP_X1C_t0  = add_c(pix_pipe.TUHU_PX4p.qp17_new(), pix_pipe.BEMY_SCX4n.qn08_new(), _BABE_MAP_X0C_t0);
    /* p26.BEWY*/ wire _BEWY_MAP_X2S_t0  = add_s(pix_pipe.TUKY_PX5p.qp17_new(), pix_pipe.CUZY_SCX5n.qn08_new(), _ABOD_MAP_X1C_t0);
    /* p26.BEWY*/ wire _BEWY_MAP_X2C_t0  = add_c(pix_pipe.TUKY_PX5p.qp17_new(), pix_pipe.CUZY_SCX5n.qn08_new(), _ABOD_MAP_X1C_t0);
    /* p26.BYCA*/ wire _BYCA_MAP_X3S_t0  = add_s(pix_pipe.TAKO_PX6p.qp17_new(), pix_pipe.CABU_SCX6n.qn08_new(), _BEWY_MAP_X2C_t0);
    /* p26.BYCA*/ wire _BYCA_MAP_X3C_t0  = add_c(pix_pipe.TAKO_PX6p.qp17_new(), pix_pipe.CABU_SCX6n.qn08_new(), _BEWY_MAP_X2C_t0);
    /* p26.ACUL*/ wire _ACUL_MAP_X4S_t0  = add_s(pix_pipe.SYBE_PX7p.qp17_new(), pix_pipe.BAKE_SCX7n.qn08_new(), _BYCA_MAP_X3C_t0);
    ///* p26.ACUL*/ wire _ACUL_MAP_X4C_t0  = add_c(pix_pipe.SYBE_PX7p.qp17_new(), pix_pipe.BAKE_SCX7n.qn08_new(), _BYCA_MAP_X3C_t0);

    /* CBA -> VBA */
    {
      /* p04.MARU*/ wire _MARU_DMA_A15p_t0 = dma_reg.MARU_DMA_A15n.qn07_new();
      /* p04.PULA*/ wire _PULA_DMA_A13p_t0 = dma_reg.PULA_DMA_A13n.qn07_new();
      /* p04.POKU*/ wire _POKU_DMA_A14p_t0 = dma_reg.POKU_DMA_A14n.qn07_new();
      /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(_MARU_DMA_A15p_t0);
      /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0  = nor3(_PULA_DMA_A13p_t0, _POKU_DMA_A14p_t0, _LEBU_DMA_A15n_t0);
      /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_t0  = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _MUDA_DMA_VRAMp_t0);
      /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_t0  = not1(_MUHO_DMA_VRAMp_t0);
      /*#p25.XANE*/ wire _XANE_VRAM_LOCKn_t2 = nor2(_LUFA_DMA_VRAMp_t0, pix_pipe.XYMU_RENDERINGn.qn03_new());
      /* p25.XEDU*/ wire _XEDU_VRAM_LOCKp_t2 = not1(_XANE_VRAM_LOCKn_t2);
      /* p25.XAKY*/ BUS_VRAM_An_t2[ 0].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 0]);
      /* p25.XUXU*/ BUS_VRAM_An_t2[ 1].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 1]);
      /* p25.XYNE*/ BUS_VRAM_An_t2[ 2].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 2]);
      /* p25.XODY*/ BUS_VRAM_An_t2[ 3].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 3]);
      /* p25.XECA*/ BUS_VRAM_An_t2[ 4].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 4]);
      /* p25.XOBA*/ BUS_VRAM_An_t2[ 5].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 5]);
      /* p25.XOPO*/ BUS_VRAM_An_t2[ 6].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 6]);
      /* p25.XYBO*/ BUS_VRAM_An_t2[ 7].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 7]);
      /* p25.RYSU*/ BUS_VRAM_An_t2[ 8].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 8]);
      /* p25.RESE*/ BUS_VRAM_An_t2[ 9].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[ 9]);
      /* p25.RUSE*/ BUS_VRAM_An_t2[10].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[10]);
      /* p25.RYNA*/ BUS_VRAM_An_t2[11].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[11]);
      /* p25.RUMO*/ BUS_VRAM_An_t2[12].tri6_nn(_XEDU_VRAM_LOCKp_t2, BUS_CPU_A_t0[12]);
    }

    /* DBA -> VBA */
    {
      /* p04.MARU*/ wire _MARU_DMA_A15p_t0 = dma_reg.MARU_DMA_A15n.qn07_new();
      /* p04.PULA*/ wire _PULA_DMA_A13p_t0 = dma_reg.PULA_DMA_A13n.qn07_new();
      /* p04.POKU*/ wire _POKU_DMA_A14p_t0 = dma_reg.POKU_DMA_A14n.qn07_new();
      /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(_MARU_DMA_A15p_t0);
      /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0  = nor3(_PULA_DMA_A13p_t0, _POKU_DMA_A14p_t0, _LEBU_DMA_A15n_t0);
      /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_t0 = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _MUDA_DMA_VRAMp_t0);
      /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_t0 = not1(_MUHO_DMA_VRAMp_t0);
      /* p04.AHOC*/ wire _AHOC_DMA_VRAMn_t0 = not1(_LUFA_DMA_VRAMp_t0);
      /* p04.NAKY*/ wire _NAKY_DMA_A00p_t0 = dma_reg.NAKY_DMA_A00p.qp17_new();
      /* p04.PYRO*/ wire _PYRO_DMA_A01p_t0 = dma_reg.PYRO_DMA_A01p.qp17_new();
      /* p04.NEFY*/ wire _NEFY_DMA_A02p_t0 = dma_reg.NEFY_DMA_A02p.qp17_new();
      /* p04.MUTY*/ wire _MUTY_DMA_A03p_t0 = dma_reg.MUTY_DMA_A03p.qp17_new();
      /* p04.NYKO*/ wire _NYKO_DMA_A04p_t0 = dma_reg.NYKO_DMA_A04p.qp17_new();
      /* p04.PYLO*/ wire _PYLO_DMA_A05p_t0 = dma_reg.PYLO_DMA_A05p.qp17_new();
      /* p04.NUTO*/ wire _NUTO_DMA_A06p_t0 = dma_reg.NUTO_DMA_A06p.qp17_new();
      /* p04.MUGU*/ wire _MUGU_DMA_A07p_t0 = dma_reg.MUGU_DMA_A07p.qp17_new();
      /* p04.NAFA*/ wire _NAFA_DMA_A08p_t0 = dma_reg.NAFA_DMA_A08n.qn07_new();
      /* p04.PYNE*/ wire _PYNE_DMA_A09p_t0 = dma_reg.PYNE_DMA_A09n.qn07_new();
      /* p04.PARA*/ wire _PARA_DMA_A10p_t0 = dma_reg.PARA_DMA_A10n.qn07_new();
      /* p04.NYDO*/ wire _NYDO_DMA_A11p_t0 = dma_reg.NYDO_DMA_A11n.qn07_new();
      /* p04.NYGY*/ wire _NYGY_DMA_A12p_t0 = dma_reg.NYGY_DMA_A12n.qn07_new();

      /* p04.ECAL*/ BUS_VRAM_An_t2[ 0].tri6_nn(_AHOC_DMA_VRAMn_t0, _NAKY_DMA_A00p_t0);
      /* p04.EGEZ*/ BUS_VRAM_An_t2[ 1].tri6_nn(_AHOC_DMA_VRAMn_t0, _PYRO_DMA_A01p_t0);
      /* p04.FUHE*/ BUS_VRAM_An_t2[ 2].tri6_nn(_AHOC_DMA_VRAMn_t0, _NEFY_DMA_A02p_t0);
      /* p04.FYZY*/ BUS_VRAM_An_t2[ 3].tri6_nn(_AHOC_DMA_VRAMn_t0, _MUTY_DMA_A03p_t0);
      /* p04.DAMU*/ BUS_VRAM_An_t2[ 4].tri6_nn(_AHOC_DMA_VRAMn_t0, _NYKO_DMA_A04p_t0);
      /* p04.DAVA*/ BUS_VRAM_An_t2[ 5].tri6_nn(_AHOC_DMA_VRAMn_t0, _PYLO_DMA_A05p_t0);
      /* p04.ETEG*/ BUS_VRAM_An_t2[ 6].tri6_nn(_AHOC_DMA_VRAMn_t0, _NUTO_DMA_A06p_t0);
      /*#p04.EREW*/ BUS_VRAM_An_t2[ 7].tri6_nn(_AHOC_DMA_VRAMn_t0, _MUGU_DMA_A07p_t0);
      /*#p04.EVAX*/ BUS_VRAM_An_t2[ 8].tri6_nn(_AHOC_DMA_VRAMn_t0, _NAFA_DMA_A08p_t0);
      /* p04.DUVE*/ BUS_VRAM_An_t2[ 9].tri6_nn(_AHOC_DMA_VRAMn_t0, _PYNE_DMA_A09p_t0);
      /* p04.ERAF*/ BUS_VRAM_An_t2[10].tri6_nn(_AHOC_DMA_VRAMn_t0, _PARA_DMA_A10p_t0);
      /* p04.FUSY*/ BUS_VRAM_An_t2[11].tri6_nn(_AHOC_DMA_VRAMn_t0, _NYDO_DMA_A11p_t0);
      /* p04.EXYF*/ BUS_VRAM_An_t2[12].tri6_nn(_AHOC_DMA_VRAMn_t0, _NYGY_DMA_A12p_t0);
    }

    /* Sprite fetcher read */
    {
      /*#p29.XUQU*/ wire _XUQU_SPRITE_AB_t0 = not1(sprite_fetcher.VONU_SFETCH_S1p_D4.qn16_new());
      /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY_t0 = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp17_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp17_new());
      /* p29.TEPA*/ wire _TEPA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /* p29.TYSO*/ wire   _TYSO_SPRITE_READn_t2 = or2(_SAKY_SFETCH_MYSTERY_t0, _TEPA_RENDERINGn_t2); // def or
      /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp_t2 = not1(_TYSO_SPRITE_READn_t2);
      /* p29.ABON*/ wire _ABON_SPR_VRM_RDn_t2 = not1(_TEXY_SPR_READ_VRAMp_t2);
      /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn_t0 = not1(pix_pipe.XYMO_LCDC_SPSIZEn.qn08_new());
      /*#p29.WUKY*/ wire _WUKY_FLIP_Yp_t0 = not1(oam_bus.YZOS_OAM_DB6p.qp08_old());

      /*#p29.CYVU*/ wire _CYVU_L0_t2 = xor2(_WUKY_FLIP_Yp_t0, SPR_TRI_L0p_t2.to_wire_new());
      /*#p29.BORE*/ wire _BORE_L1_t2 = xor2(_WUKY_FLIP_Yp_t0, SPR_TRI_L1p_t2.to_wire_new());
      /*#p29.BUVY*/ wire _BUVY_L2_t2 = xor2(_WUKY_FLIP_Yp_t0, SPR_TRI_L2p_t2.to_wire_new());
      /*#p29.WAGO*/ wire _WAGO_L3_t2 = xor2(_WUKY_FLIP_Yp_t0, SPR_TRI_L3p_t2.to_wire_new());
      /*#p29.GEJY*/ wire _GEJY_L3_t2 = amux2(oam_bus.XUSO_OAM_DA0p.qp08_old(), _FUFO_LCDC_SPSIZEn_t0, pix_pipe.XYMO_LCDC_SPSIZEn.qn08_new(), _WAGO_L3_t2);

      /* p29.ABEM*/ BUS_VRAM_An_t2[ 0].tri6_nn(_ABON_SPR_VRM_RDn_t2, _XUQU_SPRITE_AB_t0);
      /* p29.BAXE*/ BUS_VRAM_An_t2[ 1].tri6_nn(_ABON_SPR_VRM_RDn_t2, _CYVU_L0_t2);
      /* p29.ARAS*/ BUS_VRAM_An_t2[ 2].tri6_nn(_ABON_SPR_VRM_RDn_t2, _BORE_L1_t2);
      /* p29.AGAG*/ BUS_VRAM_An_t2[ 3].tri6_nn(_ABON_SPR_VRM_RDn_t2, _BUVY_L2_t2);
      /* p29.FAMU*/ BUS_VRAM_An_t2[ 4].tri6_nn(_ABON_SPR_VRM_RDn_t2, _GEJY_L3_t2);
      /*#p29.FUGY*/ BUS_VRAM_An_t2[ 5].tri6_nn(_ABON_SPR_VRM_RDn_t2, oam_bus.XEGU_OAM_DA1p.qp08_old());
      /* p29.GAVO*/ BUS_VRAM_An_t2[ 6].tri6_nn(_ABON_SPR_VRM_RDn_t2, oam_bus.YJEX_OAM_DA2p.qp08_old());
      /* p29.WYGA*/ BUS_VRAM_An_t2[ 7].tri6_nn(_ABON_SPR_VRM_RDn_t2, oam_bus.XYJU_OAM_DA3p.qp08_old());
      /* p29.WUNE*/ BUS_VRAM_An_t2[ 8].tri6_nn(_ABON_SPR_VRM_RDn_t2, oam_bus.YBOG_OAM_DA4p.qp08_old());
      /* p29.GOTU*/ BUS_VRAM_An_t2[ 9].tri6_nn(_ABON_SPR_VRM_RDn_t2, oam_bus.WYSO_OAM_DA5p.qp08_old());
      /* p29.GEGU*/ BUS_VRAM_An_t2[10].tri6_nn(_ABON_SPR_VRM_RDn_t2, oam_bus.XOTE_OAM_DA6p.qp08_old());
      /* p29.XEHE*/ BUS_VRAM_An_t2[11].tri6_nn(_ABON_SPR_VRM_RDn_t2, oam_bus.YZAB_OAM_DA7p.qp08_old());
      /* p29.DYSO*/ BUS_VRAM_An_t2[12].tri6_nn(_ABON_SPR_VRM_RDn_t2, 0);   // sprites always in low half of tile store
    }

    /* Background & window map/tile read */
    {
      /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n_t0 = not1(tile_fetcher.MESU_BFETCH_S1p.qp17_old());
      /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n_t0 = not1(tile_fetcher.NYVA_BFETCH_S2p.qp17_old());
      /* p27.NOGU*/ wire _NOGU_BFETCH_01p_t0 = nand2(_NAKO_BFETCH_S1n_t0, _NOFU_BFETCH_S2n_t0);
      /* p27.NENY*/ wire _NENY_BFETCH_01n_t0 = not1(_NOGU_BFETCH_01p_t0);
      /* p27.LUSU*/ wire   _LUSU_BGW_VRAM_RDn_t2 = not1(tile_fetcher.LONY_FETCHINGp.qp03_new());
      /* p27.LENA*/ wire _LENA_BGW_VRAM_RDp_t2 = not1(_LUSU_BGW_VRAM_RDn_t2);
      /* p27.POTU*/ wire _POTU_MAP_READp_t2 = and2(_LENA_BGW_VRAM_RDp_t2, _NENY_BFETCH_01n_t0);

      /*#p27.NOCU*/ wire _NOCU_WIN_MODEn_t0 = not1(pix_pipe.PYNU_WIN_MODE_Ap.qp04_new());
      /* p27.PORE*/ wire _PORE_WIN_MODEp_t0 = not1(_NOCU_WIN_MODEn_t0);
      /* p26.AXAD*/ wire _AXAD_WIN_MODEn_t0 = not1(_PORE_WIN_MODEp_t0);
      /* p27.NETA*/ wire _NETA_TILE_READp = and2(_LENA_BGW_VRAM_RDp_t2, _NOGU_BFETCH_01p_t0);
      {
        /* p26.ACEN*/ wire _ACEN_BG_MAP_READp_t2 = and2(_POTU_MAP_READp_t2, _AXAD_WIN_MODEn_t0);
        /* p26.BAFY*/ wire _BAFY_BG_MAP_READn_t2 = not1(_ACEN_BG_MAP_READp_t2);
        /* p26.AXEP*/ BUS_VRAM_An_t2[ 0].tri6_nn(_BAFY_BG_MAP_READn_t2, _BABE_MAP_X0S_t0);
        /* p26.AFEB*/ BUS_VRAM_An_t2[ 1].tri6_nn(_BAFY_BG_MAP_READn_t2, _ABOD_MAP_X1S_t0);
        /* p26.ALEL*/ BUS_VRAM_An_t2[ 2].tri6_nn(_BAFY_BG_MAP_READn_t2, _BEWY_MAP_X2S_t0);
        /* p26.COLY*/ BUS_VRAM_An_t2[ 3].tri6_nn(_BAFY_BG_MAP_READn_t2, _BYCA_MAP_X3S_t0);
        /* p26.AJAN*/ BUS_VRAM_An_t2[ 4].tri6_nn(_BAFY_BG_MAP_READn_t2, _ACUL_MAP_X4S_t0);
        /* p26.DUHO*/ BUS_VRAM_An_t2[ 5].tri6_nn(_BAFY_BG_MAP_READn_t2, _ETAM_MAP_Y0S_t0);
        /* p26.CASE*/ BUS_VRAM_An_t2[ 6].tri6_nn(_BAFY_BG_MAP_READn_t2, _DOTO_MAP_Y1S_t0);
        /* p26.CYPO*/ BUS_VRAM_An_t2[ 7].tri6_nn(_BAFY_BG_MAP_READn_t2, _DABA_MAP_Y2S_t0);
        /* p26.CETA*/ BUS_VRAM_An_t2[ 8].tri6_nn(_BAFY_BG_MAP_READn_t2, _EFYK_MAP_Y3S_t0);
        /* p26.DAFE*/ BUS_VRAM_An_t2[ 9].tri6_nn(_BAFY_BG_MAP_READn_t2, _EJOK_MAP_Y4S_t0);
        /*#p26.AMUV*/ BUS_VRAM_An_t2[10].tri6_nn(_BAFY_BG_MAP_READn_t2, pix_pipe.XAFO_LCDC_BGMAPn.qn08_new());
        /* p26.COVE*/ BUS_VRAM_An_t2[11].tri6_nn(_BAFY_BG_MAP_READn_t2, 1);
        /* p26.COXO*/ BUS_VRAM_An_t2[12].tri6_nn(_BAFY_BG_MAP_READn_t2, 1);
      }

      {
        /*#p25.XEZE*/ wire _XEZE_WIN_MAP_READp_t2 = and2(_POTU_MAP_READp_t2, _PORE_WIN_MODEp_t0);
        /*#p25.WUKO*/ wire _WUKO_WIN_MAP_READn_t2 = not1(_XEZE_WIN_MAP_READp_t2);
        /*#p27.XEJA*/ BUS_VRAM_An_t2[ 0].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.WYKA_WIN_X3.qp17_old());
        /* p27.XAMO*/ BUS_VRAM_An_t2[ 1].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.WODY_WIN_X4.qp17_old());
        /* p27.XAHE*/ BUS_VRAM_An_t2[ 2].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.WOBO_WIN_X5.qp17_old());
        /* p27.XULO*/ BUS_VRAM_An_t2[ 3].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.WYKO_WIN_X6.qp17_old());
        /* p27.WUJU*/ BUS_VRAM_An_t2[ 4].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.XOLO_WIN_X7.qp17_old());
        /*#p27.VYTO*/ BUS_VRAM_An_t2[ 5].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.TUFU_WIN_Y3.qp17_old());
        /* p27.VEHA*/ BUS_VRAM_An_t2[ 6].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.TAXA_WIN_Y4.qp17_old());
        /* p27.VACE*/ BUS_VRAM_An_t2[ 7].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.TOZO_WIN_Y5.qp17_old());
        /* p27.VOVO*/ BUS_VRAM_An_t2[ 8].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.TATE_WIN_Y6.qp17_old());
        /* p27.VULO*/ BUS_VRAM_An_t2[ 9].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.TEKE_WIN_Y7.qp17_old());
        /*#p27.VEVY*/ BUS_VRAM_An_t2[10].tri6_nn(_WUKO_WIN_MAP_READn_t2, pix_pipe.WOKY_LCDC_WINMAPn.qn08_new());
        /* p27.VEZA*/ BUS_VRAM_An_t2[11].tri6_nn(_WUKO_WIN_MAP_READn_t2, 1);
        /* p27.VOGU*/ BUS_VRAM_An_t2[12].tri6_nn(_WUKO_WIN_MAP_READn_t2, 1);
      }

      {
        /* p26.ASUL*/ wire _ASUL_BG_TILE_READp = and2(_NETA_TILE_READp, _AXAD_WIN_MODEn_t0);
        /* p26.BEJE*/ wire _BEJE_BG_TILE_READn = not1(_ASUL_BG_TILE_READp);
        /*#p27.XUHA*/ wire _XUHA_FETCH_S2p = not1(_NOFU_BFETCH_S2n_t0);
        /*#p26.ASUM*/ BUS_VRAM_An_t2[0].tri6_nn(_BEJE_BG_TILE_READn, _XUHA_FETCH_S2p);
        /* p26.EVAD*/ BUS_VRAM_An_t2[1].tri6_nn(_BEJE_BG_TILE_READn, _FAFO_TILE_Y0S_t0);
        /* p26.DAHU*/ BUS_VRAM_An_t2[2].tri6_nn(_BEJE_BG_TILE_READn, _EMUX_TILE_Y1S_t0);
        /* p26.DODE*/ BUS_VRAM_An_t2[3].tri6_nn(_BEJE_BG_TILE_READn, _ECAB_TILE_Y2S_t0); // check outputs of ECAB
      }

      {
        /* p25.XUCY*/ wire _XUCY_WIN_TILE_READn = nand2(_NETA_TILE_READp, _PORE_WIN_MODEp_t0);
        /*#p27.XUHA*/ wire _XUHA_FETCH_S2p = not1(_NOFU_BFETCH_S2n_t0);
        /*#p25.XONU*/ BUS_VRAM_An_t2[0].tri6_nn(_XUCY_WIN_TILE_READn, _XUHA_FETCH_S2p);
        /*#p25.WUDO*/ BUS_VRAM_An_t2[1].tri6_nn(_XUCY_WIN_TILE_READn, pix_pipe.VYNO_WIN_Y0.qp17_old());
        /*#p25.WAWE*/ BUS_VRAM_An_t2[2].tri6_nn(_XUCY_WIN_TILE_READn, pix_pipe.VUJO_WIN_Y1.qp17_old());
        /*#p25.WOLU*/ BUS_VRAM_An_t2[3].tri6_nn(_XUCY_WIN_TILE_READn, pix_pipe.VYMU_WIN_Y2.qp17_old());
      }

      {
        /*#p25.VAPY*/ BUS_VRAM_An_t2[ 4].tri6_pn(_NETA_TILE_READp, vram_bus.RAWU_TILE_DB0p.q11p_old());
        /*#p25.SEZU*/ BUS_VRAM_An_t2[ 5].tri6_pn(_NETA_TILE_READp, vram_bus.POZO_TILE_DB1p.q11p_old());
        /*#p25.VEJY*/ BUS_VRAM_An_t2[ 6].tri6_pn(_NETA_TILE_READp, vram_bus.PYZO_TILE_DB2p.q11p_old());
        /*#p25.RUSA*/ BUS_VRAM_An_t2[ 7].tri6_pn(_NETA_TILE_READp, vram_bus.POXA_TILE_DB3p.q11p_old());
        /*#p25.ROHA*/ BUS_VRAM_An_t2[ 8].tri6_pn(_NETA_TILE_READp, vram_bus.PULO_TILE_DB4p.q11p_old());
        /*#p25.RESO*/ BUS_VRAM_An_t2[ 9].tri6_pn(_NETA_TILE_READp, vram_bus.POJU_TILE_DB5p.q11p_old());
        /*#p25.SUVO*/ BUS_VRAM_An_t2[10].tri6_pn(_NETA_TILE_READp, vram_bus.POWY_TILE_DB6p.q11p_old());
        /*#p25.TOBO*/ BUS_VRAM_An_t2[11].tri6_pn(_NETA_TILE_READp, vram_bus.PYJU_TILE_DB7p.q11p_old());
      }

      {
        /*#p25.VUZA*/ wire _VUZA_TILE_BANKp = nor2(vram_bus.PYJU_TILE_DB7p.q11p_old(), pix_pipe.WEXU_LCDC_BGTILEn.qn08_new());
        /*#p25.VURY*/ BUS_VRAM_An_t2[12].tri6_pn(_NETA_TILE_READp, _VUZA_TILE_BANKp);
      }
    }

    {
      /* CBD -> VBD */
      // Ignoring debug for now
      /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp_t1 = and2(_SOSE_8000_9FFFp_t0, _ABUZ_xxCDEFGH_t1);
      ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_VRAM_CSn.qn());
      /*#p25.TOLE*/ wire _TOLE_VRAM_RDp_t1     = /*mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, */ _TUCA_CPU_VRAM_RDp_t1 /*)*/;
      /*#p25.ROPY*/ wire _ROPY_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp_t2 = and2(_TOLE_VRAM_RDp_t1, _ROPY_RENDERINGn_t2);
      // Ignoring debug for now
      /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn_t0 = nand2(_SOSE_8000_9FFFp_t0, PIN_CPU_WRp_t0.qp());  // Schematic wrong, second input is PIN_CPU_WRp
      ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_VRAM_OEn.qn());
      /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn_t0 = /*mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp,*/ _TEGU_CPU_VRAM_WRn_t0 /*)*/;
      /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp_t0 = not1(_SALE_CPU_VRAM_WRn_t0);
      /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp_t2 = and2(_SERE_CPU_VRM_RDp_t2, _RUVY_CPU_VRAM_WRp_t0);
      /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn_t2 = not1(_SAZO_CBD_TO_VPDp_t2);
      /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp_t2 = not1(_RYJE_CBD_TO_VPDn_t2);
      /* p25.ROCY*/ wire _ROCY_CBD_TO_VPDp_t2 = and2(_REVO_CBD_TO_VPDp_t2, _SAZO_CBD_TO_VPDp_t2);
      /* p25.RAHU*/ wire _RAHU_CBD_TO_VPDn_t2 = not1(_ROCY_CBD_TO_VPDp_t2);
      /* p25.TEME*/ BUS_VRAM_Dp_out[0].tri10_np(_RAHU_CBD_TO_VPDn_t2, BUS_CPU_Dp_in[0].to_wire_new());
      /* p25.TEWU*/ BUS_VRAM_Dp_out[1].tri10_np(_RAHU_CBD_TO_VPDn_t2, BUS_CPU_Dp_in[1].to_wire_new());
      /*#p25.TYGO*/ BUS_VRAM_Dp_out[2].tri10_np(_RAHU_CBD_TO_VPDn_t2, BUS_CPU_Dp_in[2].to_wire_new());
      /* p25.SOTE*/ BUS_VRAM_Dp_out[3].tri10_np(_RAHU_CBD_TO_VPDn_t2, BUS_CPU_Dp_in[3].to_wire_new());
      /* p25.SEKE*/ BUS_VRAM_Dp_out[4].tri10_np(_RAHU_CBD_TO_VPDn_t2, BUS_CPU_Dp_in[4].to_wire_new());
      /* p25.RUJO*/ BUS_VRAM_Dp_out[5].tri10_np(_RAHU_CBD_TO_VPDn_t2, BUS_CPU_Dp_in[5].to_wire_new());
      /* p25.TOFA*/ BUS_VRAM_Dp_out[6].tri10_np(_RAHU_CBD_TO_VPDn_t2, BUS_CPU_Dp_in[6].to_wire_new());
      /* p25.SUZA*/ BUS_VRAM_Dp_out[7].tri10_np(_RAHU_CBD_TO_VPDn_t2, BUS_CPU_Dp_in[7].to_wire_new());
    }

    uint16_t vram_addr_t2;

    {
      // VRAM addr bus -> VRAM addr pin
      /* p25.MYFU*/ wire _MYFUp_t2 = not1(BUS_VRAM_An_t2[ 0].to_wire_new());
      /* p25.MASA*/ wire _MASAp_t2 = not1(BUS_VRAM_An_t2[ 1].to_wire_new());
      /* p25.MYRE*/ wire _MYREp_t2 = not1(BUS_VRAM_An_t2[ 2].to_wire_new());
      /* p25.MAVU*/ wire _MAVUp_t2 = not1(BUS_VRAM_An_t2[ 3].to_wire_new());
      /* p25.MEPA*/ wire _MEPAp_t2 = not1(BUS_VRAM_An_t2[ 4].to_wire_new());
      /* p25.MYSA*/ wire _MYSAp_t2 = not1(BUS_VRAM_An_t2[ 5].to_wire_new());
      /* p25.MEWY*/ wire _MEWYp_t2 = not1(BUS_VRAM_An_t2[ 6].to_wire_new());
      /* p25.MUME*/ wire _MUMEp_t2 = not1(BUS_VRAM_An_t2[ 7].to_wire_new());
      /* p25.VOVA*/ wire _VOVAp_t2 = not1(BUS_VRAM_An_t2[ 8].to_wire_new());
      /* p25.VODE*/ wire _VODEp_t2 = not1(BUS_VRAM_An_t2[ 9].to_wire_new());
      /* p25.RUKY*/ wire _RUKYp_t2 = not1(BUS_VRAM_An_t2[10].to_wire_new());
      /* p25.RUMA*/ wire _RUMAp_t2 = not1(BUS_VRAM_An_t2[11].to_wire_new());
      /* p25.REHO*/ wire _REHOp_t2 = not1(BUS_VRAM_An_t2[12].to_wire_new());

      /* p25.LEXE*/ wire _LEXEn_t2 = not1(_MYFUp_t2);
      /* p25.LOZU*/ wire _LOZUn_t2 = not1(_MASAp_t2);
      /* p25.LACA*/ wire _LACAn_t2 = not1(_MYREp_t2);
      /* p25.LUVO*/ wire _LUVOn_t2 = not1(_MAVUp_t2);
      /* p25.LOLY*/ wire _LOLYn_t2 = not1(_MEPAp_t2);
      /* p25.LALO*/ wire _LALOn_t2 = not1(_MYSAp_t2);
      /* p25.LEFA*/ wire _LEFAn_t2 = not1(_MEWYp_t2);
      /* p25.LUBY*/ wire _LUBYn_t2 = not1(_MUMEp_t2);
      /* p25.TUJY*/ wire _TUJYn_t2 = not1(_VOVAp_t2);
      /* p25.TAGO*/ wire _TAGOn_t2 = not1(_VODEp_t2);
      /* p25.NUVA*/ wire _NUVAn_t2 = not1(_RUKYp_t2);
      /* p25.PEDU*/ wire _PEDUn_t2 = not1(_RUMAp_t2);
      /* p25.PONY*/ wire _PONYn_t2 = not1(_REHOp_t2);

      Pin2 PIN_VRAM_Ap_t2[13];
      static_assert(sizeof(PIN_VRAM_Ap_t2) == 13, "bad bitbase");

      PIN_VRAM_Ap_t2[ 0].pin_out(1, _LEXEn_t2, _LEXEn_t2);
      PIN_VRAM_Ap_t2[ 1].pin_out(1, _LOZUn_t2, _LOZUn_t2);
      PIN_VRAM_Ap_t2[ 2].pin_out(1, _LACAn_t2, _LACAn_t2);
      PIN_VRAM_Ap_t2[ 3].pin_out(1, _LUVOn_t2, _LUVOn_t2);
      PIN_VRAM_Ap_t2[ 4].pin_out(1, _LOLYn_t2, _LOLYn_t2);
      PIN_VRAM_Ap_t2[ 5].pin_out(1, _LALOn_t2, _LALOn_t2);
      PIN_VRAM_Ap_t2[ 6].pin_out(1, _LEFAn_t2, _LEFAn_t2);
      PIN_VRAM_Ap_t2[ 7].pin_out(1, _LUBYn_t2, _LUBYn_t2);
      PIN_VRAM_Ap_t2[ 8].pin_out(1, _TUJYn_t2, _TUJYn_t2);
      PIN_VRAM_Ap_t2[ 9].pin_out(1, _TAGOn_t2, _TAGOn_t2);
      PIN_VRAM_Ap_t2[10].pin_out(1, _NUVAn_t2, _NUVAn_t2);
      PIN_VRAM_Ap_t2[11].pin_out(1, _PEDUn_t2, _PEDUn_t2);
      PIN_VRAM_Ap_t2[12].pin_out(1, _PONYn_t2, _PONYn_t2);

      vram_addr_t2 = pack_u16p_new(13, PIN_VRAM_Ap_t2);
    }

    {
      /* p29.TYTU*/ wire _TYTU_SFETCH_S0n_t0 = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp17_new());
      /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG_t0 = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp17_new(), _TYTU_SFETCH_S0n_t0);

      /* p04.MARU*/ wire _MARU_DMA_A15p_t0 = dma_reg.MARU_DMA_A15n.qn07_new();
      /* p04.PULA*/ wire _PULA_DMA_A13p_t0 = dma_reg.PULA_DMA_A13n.qn07_new();
      /* p04.POKU*/ wire _POKU_DMA_A14p_t0 = dma_reg.POKU_DMA_A14n.qn07_new();
      /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(_MARU_DMA_A15p_t0);
      /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0  = nor3(_PULA_DMA_A13p_t0, _POKU_DMA_A14p_t0, _LEBU_DMA_A15n_t0);
      /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_t0  = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _MUDA_DMA_VRAMp_t0);
      /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_t0  = not1(_MUHO_DMA_VRAMp_t0);
      /*#p25.XANE*/ wire _XANE_VRAM_LOCKn_t2 = nor2(_LUFA_DMA_VRAMp_t0, pix_pipe.XYMU_RENDERINGn.qn03_new());
      // Ignoring debug for now
      /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn_t0 = nand2(_SOSE_8000_9FFFp_t0, PIN_CPU_WRp_t0.qp());  // Schematic wrong, second input is PIN_CPU_WRp
      ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_VRAM_OEn.qn());
      /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn_t0 = /*mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp,*/ _TEGU_CPU_VRAM_WRn_t0 /*)*/;
      /* p25.RYLU*/ wire _RYLU_CPU_VRAM_RDn_t2 = nand2(_SALE_CPU_VRAM_WRn_t0, _XANE_VRAM_LOCKn_t2);
      /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY_t0 = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp17_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp17_new());
      /* p29.TEPA*/ wire _TEPA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /* p29.TYSO*/ wire   _TYSO_SPRITE_READn_t2 = or2(_SAKY_SFETCH_MYSTERY_t0, _TEPA_RENDERINGn_t2); // def or
      /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp_t2 = not1(_TYSO_SPRITE_READn_t2);
      /* p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp_t2 = and2(_TACU_SPR_SEQ_5_TRIG_t0, _TEXY_SPR_READ_VRAMp_t2);
      /* p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn_t2 = not1(_SOHO_SPR_VRAM_RDp_t2);
      /* p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn_t2 = not1(tile_fetcher.LONY_FETCHINGp.qp03_new());

      /* p25.APAM*/ wire _APAM_DMA_VRAMn_t0    = not1(_LUFA_DMA_VRAMp_t0);
      /* p25.RACU*/ wire _RACU_MOEn_t2 = and4(_RYLU_CPU_VRAM_RDn_t2, _RAWA_SPR_VRAM_RDn_t2, _MYMA_BGW_VRAM_RDn_t2, _APAM_DMA_VRAMn_t0); // def and

      /* p25.TUTO*/ wire _TUTO_DBG_VRAMp_t1 = and2(_UNOR_MODE_DBG2p_t0, SOTO_DBG_VRAMp.qn16_new());
      /*#p25.RACO*/ wire _RACO_DBG_VRAMn_t1 = not1(_TUTO_DBG_VRAMp_t1);
      /* p25.SEMA*/ wire _SEMA_MOEn_A_t2 = and2(_RACU_MOEn_t2, _RACO_DBG_VRAMn_t1);
      /* p25.RUTE*/ wire _RUTE_MOEn_D_t2 = or2(_RACU_MOEn_t2, _TUTO_DBG_VRAMp_t1); // schematic wrong, second input is RACU
      /* p25.REFO*/ wire _REFO_MOEn_A_t2 = not1(_SEMA_MOEn_A_t2);
      /* p25.SAHA*/ wire _SAHA_MOEn_D_t2 = not1(_RUTE_MOEn_D_t2);

      Pin2 PIN_VRAM_OEn_t2; // PIN_45
      PIN_VRAM_OEn_t2.pin_out(1, _REFO_MOEn_A_t2, _SAHA_MOEn_D_t2);

      Pin2 PIN_VRAM_Dp_in_t2[8];  // This pin isn't driven between tile fetches while rendering; where's the pullup?
      static_assert(sizeof(PIN_VRAM_Dp_in_t2) == 8, "bad bitbase");
      uint8_t  vram_data = vid_ram[vram_addr_t2];

      PIN_VRAM_Dp_in_t2[0].pin_in(!PIN_VRAM_OEn_t2.qp(), (vram_data & 0x01));
      PIN_VRAM_Dp_in_t2[1].pin_in(!PIN_VRAM_OEn_t2.qp(), (vram_data & 0x02));
      PIN_VRAM_Dp_in_t2[2].pin_in(!PIN_VRAM_OEn_t2.qp(), (vram_data & 0x04));
      PIN_VRAM_Dp_in_t2[3].pin_in(!PIN_VRAM_OEn_t2.qp(), (vram_data & 0x08));
      PIN_VRAM_Dp_in_t2[4].pin_in(!PIN_VRAM_OEn_t2.qp(), (vram_data & 0x10));
      PIN_VRAM_Dp_in_t2[5].pin_in(!PIN_VRAM_OEn_t2.qp(), (vram_data & 0x20));
      PIN_VRAM_Dp_in_t2[6].pin_in(!PIN_VRAM_OEn_t2.qp(), (vram_data & 0x40));
      PIN_VRAM_Dp_in_t2[7].pin_in(!PIN_VRAM_OEn_t2.qp(), (vram_data & 0x80));

      // Ignoring debug for now
      /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp_t1 = and2(_SOSE_8000_9FFFp_t0, _ABUZ_xxCDEFGH_t1);
      ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_VRAM_CSn.qn());
      /*#p25.TOLE*/ wire _TOLE_VRAM_RDp_t1     = /*mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, */ _TUCA_CPU_VRAM_RDp_t1 /*)*/;
      /*#p25.ROPY*/ wire _ROPY_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp_t2 = and2(_TOLE_VRAM_RDp_t1, _ROPY_RENDERINGn_t2);
      /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp_t0 = not1(_SALE_CPU_VRAM_WRn_t0);
      /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp_t2 = and2(_SERE_CPU_VRM_RDp_t2, _RUVY_CPU_VRAM_WRp_t0);
      /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn_t2 = not1(_SAZO_CBD_TO_VPDp_t2);
      /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp_t2 = not1(_RYJE_CBD_TO_VPDn_t2);
      /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp_t2 = or2(_REVO_CBD_TO_VPDp_t2, _SAZO_CBD_TO_VPDp_t2);
      /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn_t2 = not1(_RELA_CBD_TO_VPDp_t2);
      /* p25.RODY*/ BUS_VRAM_Dp_in[0].tri6_pn(_RENA_CBD_TO_VPDn_t2, PIN_VRAM_Dp_in_t2[0].qn());
      /* p25.REBA*/ BUS_VRAM_Dp_in[1].tri6_pn(_RENA_CBD_TO_VPDn_t2, PIN_VRAM_Dp_in_t2[1].qn());
      /* p25.RYDO*/ BUS_VRAM_Dp_in[2].tri6_pn(_RENA_CBD_TO_VPDn_t2, PIN_VRAM_Dp_in_t2[2].qn());
      /* p25.REMO*/ BUS_VRAM_Dp_in[3].tri6_pn(_RENA_CBD_TO_VPDn_t2, PIN_VRAM_Dp_in_t2[3].qn());
      /* p25.ROCE*/ BUS_VRAM_Dp_in[4].tri6_pn(_RENA_CBD_TO_VPDn_t2, PIN_VRAM_Dp_in_t2[4].qn());
      /* p25.ROPU*/ BUS_VRAM_Dp_in[5].tri6_pn(_RENA_CBD_TO_VPDn_t2, PIN_VRAM_Dp_in_t2[5].qn());
      /* p25.RETA*/ BUS_VRAM_Dp_in[6].tri6_pn(_RENA_CBD_TO_VPDn_t2, PIN_VRAM_Dp_in_t2[6].qn());
      /* p25.RAKU*/ BUS_VRAM_Dp_in[7].tri6_pn(_RENA_CBD_TO_VPDn_t2, PIN_VRAM_Dp_in_t2[7].qn());
    }

    {
      // Ignoring debug for now
      /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp_t1 = and2(_SOSE_8000_9FFFp_t0, _ABUZ_xxCDEFGH_t1);
      ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_VRAM_CSn.qn());
      /*#p25.TOLE*/ wire _TOLE_VRAM_RDp_t1     = /*mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, */ _TUCA_CPU_VRAM_RDp_t1 /*)*/;
      /* p04.MARU*/ wire _MARU_DMA_A15p_t0 = dma_reg.MARU_DMA_A15n.qn07_new();
      /* p04.PULA*/ wire _PULA_DMA_A13p_t0 = dma_reg.PULA_DMA_A13n.qn07_new();
      /* p04.POKU*/ wire _POKU_DMA_A14p_t0 = dma_reg.POKU_DMA_A14n.qn07_new();
      /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(_MARU_DMA_A15p_t0);
      /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0  = nor3(_PULA_DMA_A13p_t0, _POKU_DMA_A14p_t0, _LEBU_DMA_A15n_t0);
      /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_t0  = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _MUDA_DMA_VRAMp_t0);
      /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_t0  = not1(_MUHO_DMA_VRAMp_t0);
      /*#p25.ROPY*/ wire _ROPY_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp_t2 = and2(_TOLE_VRAM_RDp_t1, _ROPY_RENDERINGn_t2);
      /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY_t0 = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp17_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp17_new());
      /* p29.TEPA*/ wire _TEPA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /* p29.TYSO*/ wire   _TYSO_SPRITE_READn_t2 = or2(_SAKY_SFETCH_MYSTERY_t0, _TEPA_RENDERINGn_t2); // def or
      /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp_t2 = not1(_TYSO_SPRITE_READn_t2);
      /* p27.LUSU*/ wire   _LUSU_BGW_VRAM_RDn_t2 = not1(tile_fetcher.LONY_FETCHINGp.qp03_new());
      /* p27.LENA*/ wire _LENA_BGW_VRAM_RDp_t2 = not1(_LUSU_BGW_VRAM_RDn_t2);
      /*#p25.SUTU*/ wire _SUTU_MCSn_t2 = nor4(_LENA_BGW_VRAM_RDp_t2, _LUFA_DMA_VRAMp_t0, _TEXY_SPR_READ_VRAMp_t2, _SERE_CPU_VRM_RDp_t2);
      /* p25.TUTO*/ wire _TUTO_DBG_VRAMp_t1 = and2(_UNOR_MODE_DBG2p_t0, SOTO_DBG_VRAMp.qn16_new());
      /*#p25.RACO*/ wire _RACO_DBG_VRAMn_t1 = not1(_TUTO_DBG_VRAMp_t1);
      /* p25.TODE*/ wire _TODE_MCSn_A_t2 = and2(_SUTU_MCSn_t2, _RACO_DBG_VRAMn_t1);
      /* p25.SEWO*/ wire _SEWO_MCSn_D_t2 = or2(_SUTU_MCSn_t2, _TUTO_DBG_VRAMp_t1);
      /* p25.SOKY*/ wire _SOKY_MCSp_A_t2 = not1(_TODE_MCSn_A_t2);
      /* p25.SETY*/ wire _SETY_MCSp_D_t2 = not1(_SEWO_MCSn_D_t2);

      // FIXME not actually using this pin, but we should
      Pin2 PIN_VRAM_CSn_t2; // PIN_43
      PIN_VRAM_CSn_t2.pin_out(1, _SOKY_MCSp_A_t2, _SETY_MCSp_D_t2);

      /* p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp_t1 = and2(_SOSE_8000_9FFFp_t0, _APOV_CPU_WRp_xxxxEFGx_t1);


      /* p25.SUDO*/ wire _SUDO_MWRp = not1(/*vram_bus.PIN_VRAM_WRn.qn()*/ 1); // Ignoring debug stuff for now
      /* p25.TYJY*/ wire _TYJY_VRAM_WRp_t1 = mux2p(_TUTO_DBG_VRAMp_t1, _SUDO_MWRp, _TUJA_CPU_VRAM_WRp_t1);

      /* p25.SOHY*/ wire _SOHY_MWRn_t2 = nand2(_TYJY_VRAM_WRp_t1, _SERE_CPU_VRM_RDp_t2);

      /* p25.TAXY*/ wire _TAXY_MWRn_A_t2 = and2(_SOHY_MWRn_t2, _RACO_DBG_VRAMn_t1);
      /* p25.SOFY*/ wire _SOFY_MWRn_D_t2 = or2(_SOHY_MWRn_t2, _TUTO_DBG_VRAMp_t1);
      /* p25.SYSY*/ wire _SYSY_MWRp_A_t2 = not1(_TAXY_MWRn_A_t2);
      /* p25.RAGU*/ wire _RAGU_MWRp_D_t2 = not1(_SOFY_MWRn_D_t2);

      Pin2 PIN_VRAM_WRn_t2; // PIN_49
      PIN_VRAM_WRn_t2.pin_out(1, _SYSY_MWRp_A_t2, _RAGU_MWRp_D_t2);

      Pin2 PIN_VRAM_Dp_out[8]; // This pin isn't driven between tile fetches while rendering; where's the pullup?
      static_assert(sizeof(PIN_VRAM_Dp_out) == 8, "bad bitbase");

      // Ignoring debug for now
      /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn_t0 = nand2(_SOSE_8000_9FFFp_t0, PIN_CPU_WRp_t0.qp());  // Schematic wrong, second input is PIN_CPU_WRp
      ///*#p25.TAVY*/ wire _TAVY_MOEp         = not1(vram_bus.PIN_VRAM_OEn.qn());
      /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn_t0 = /*mux2p(_TUTO_DBG_VRAMp, _TAVY_MOEp,*/ _TEGU_CPU_VRAM_WRn_t0 /*)*/;
      /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp_t0 = not1(_SALE_CPU_VRAM_WRn_t0);
      /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp_t2 = and2(_SERE_CPU_VRM_RDp_t2, _RUVY_CPU_VRAM_WRp_t0);
      /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn_t2 = not1(_SAZO_CBD_TO_VPDp_t2);
      /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp_t2 = not1(_RYJE_CBD_TO_VPDn_t2);
      /* p25.ROCY*/ wire _ROCY_CBD_TO_VPDp_t2 = and2(_REVO_CBD_TO_VPDp_t2, _SAZO_CBD_TO_VPDp_t2);
      /* p25.RAHU*/ wire _RAHU_CBD_TO_VPDn_t2 = not1(_ROCY_CBD_TO_VPDp_t2);
      /*#p25.ROVE*/ wire _ROVE_CBD_TO_VPDp_t2 = not1(_RAHU_CBD_TO_VPDn_t2);
      /*#p25.SEFA*/ wire _SEFA_D0p = and2(BUS_VRAM_Dp_out[0].to_wire_new(), _ROVE_CBD_TO_VPDp_t2);
      /* p25.SOGO*/ wire _SOGO_D1p = and2(BUS_VRAM_Dp_out[1].to_wire_new(), _ROVE_CBD_TO_VPDp_t2);
      /* p25.SEFU*/ wire _SEFU_D2p = and2(BUS_VRAM_Dp_out[2].to_wire_new(), _ROVE_CBD_TO_VPDp_t2);
      /* p25.SUNA*/ wire _SUNA_D3p = and2(BUS_VRAM_Dp_out[3].to_wire_new(), _ROVE_CBD_TO_VPDp_t2);
      /* p25.SUMO*/ wire _SUMO_D4p = and2(BUS_VRAM_Dp_out[4].to_wire_new(), _ROVE_CBD_TO_VPDp_t2);
      /* p25.SAZU*/ wire _SAZU_D5p = and2(BUS_VRAM_Dp_out[5].to_wire_new(), _ROVE_CBD_TO_VPDp_t2);
      /* p25.SAMO*/ wire _SAMO_D6p = and2(BUS_VRAM_Dp_out[6].to_wire_new(), _ROVE_CBD_TO_VPDp_t2);
      /* p25.SUKE*/ wire _SUKE_D7p = and2(BUS_VRAM_Dp_out[7].to_wire_new(), _ROVE_CBD_TO_VPDp_t2);

      /*#p25.SYNU*/ wire _SYNU_D0p = or2(_RAHU_CBD_TO_VPDn_t2, BUS_VRAM_Dp_out[0].to_wire_new());
      /* p25.SYMA*/ wire _SYMA_D1p = or2(_RAHU_CBD_TO_VPDn_t2, BUS_VRAM_Dp_out[1].to_wire_new());
      /* p25.ROKO*/ wire _ROKO_D2p = or2(_RAHU_CBD_TO_VPDn_t2, BUS_VRAM_Dp_out[2].to_wire_new());
      /* p25.SYBU*/ wire _SYBU_D3p = or2(_RAHU_CBD_TO_VPDn_t2, BUS_VRAM_Dp_out[3].to_wire_new());
      /* p25.SAKO*/ wire _SAKO_D4p = or2(_RAHU_CBD_TO_VPDn_t2, BUS_VRAM_Dp_out[4].to_wire_new());
      /* p25.SEJY*/ wire _SEJY_D5p = or2(_RAHU_CBD_TO_VPDn_t2, BUS_VRAM_Dp_out[5].to_wire_new());
      /* p25.SEDO*/ wire _SEDO_D6p = or2(_RAHU_CBD_TO_VPDn_t2, BUS_VRAM_Dp_out[6].to_wire_new());
      /* p25.SAWU*/ wire _SAWU_D7p = or2(_RAHU_CBD_TO_VPDn_t2, BUS_VRAM_Dp_out[7].to_wire_new());

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

      /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp_t2 = or2(_REVO_CBD_TO_VPDp_t2, _SAZO_CBD_TO_VPDp_t2);
      /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn_t2 = not1(_RELA_CBD_TO_VPDp_t2);
      /*#p25.ROFA*/ wire _ROFA_CBD_TO_VPDp_t2 = not1(_RENA_CBD_TO_VPDn_t2);
      PIN_VRAM_Dp_out[0].pin_out(_ROFA_CBD_TO_VPDp_t2, _REGE_D0n, _RURA_D0n);
      PIN_VRAM_Dp_out[1].pin_out(_ROFA_CBD_TO_VPDp_t2, _RYKY_D1n, _RULY_D1n);
      PIN_VRAM_Dp_out[2].pin_out(_ROFA_CBD_TO_VPDp_t2, _RAZO_D2n, _RARE_D2n);
      PIN_VRAM_Dp_out[3].pin_out(_ROFA_CBD_TO_VPDp_t2, _RADA_D3n, _RODU_D3n);
      PIN_VRAM_Dp_out[4].pin_out(_ROFA_CBD_TO_VPDp_t2, _RYRO_D4n, _RUBE_D4n);
      PIN_VRAM_Dp_out[5].pin_out(_ROFA_CBD_TO_VPDp_t2, _REVU_D5n, _RUMU_D5n);
      PIN_VRAM_Dp_out[6].pin_out(_ROFA_CBD_TO_VPDp_t2, _REKU_D6n, _RYTY_D6n);
      PIN_VRAM_Dp_out[7].pin_out(_ROFA_CBD_TO_VPDp_t2, _RYZE_D7n, _RADY_D7n);

      // We're getting a fake write on the first phase because PIN_VRAM_WRn resets to 0...
      // ignore it if we're in reset

      if (!sys_rst && !PIN_VRAM_WRn_t2.qp()) {
        vid_ram[vram_addr_t2] = pack_u8p_new(8, PIN_VRAM_Dp_out);
      }
    }
  }






































































  //----------------------------------------
  // Ext pins

  Pin2 PIN_EXT_D_in[8];

  Pin2 PIN_EXT_A[16];
  {
    /* p08.LOXO*/ wire _LOXO_HOLDn_t0 = and_or3(_MULE_MODE_DBG1n_t0, _TEXO_8000_9FFFn_t0, _UMUT_MODE_DBG1p_t0);
    /* p08.LASY*/ wire _LASY_HOLDp_t0 = not1(_LOXO_HOLDn_t0);
    /* p08.MATE*/ wire _MATE_HOLDn_t0 = not1(_LASY_HOLDp_t0);
    /* p08.ALOR*/ ext_bus.ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 0]);
    /* p08.APUR*/ ext_bus.APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 1]);
    /* p08.ALYR*/ ext_bus.ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 2]);
    /* p08.ARET*/ ext_bus.ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 3]);
    /* p08.AVYS*/ ext_bus.AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 4]);
    /* p08.ATEV*/ ext_bus.ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 5]);
    /* p08.AROS*/ ext_bus.AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 6]);
    /* p08.ARYM*/ ext_bus.ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 7]);
    /* p08.LUNO*/ ext_bus.LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 8]);
    /* p08.LYSA*/ ext_bus.LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[ 9]);
    /* p08.PATE*/ ext_bus.PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[10]);
    /* p08.LUMY*/ ext_bus.LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[11]);
    /* p08.LOBU*/ ext_bus.LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[12]);
    /* p08.LONU*/ ext_bus.LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[13]);
    /* p08.NYRE*/ ext_bus.NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn_t0, BUS_CPU_A_t0[14]);

    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(dma_reg.MARU_DMA_A15n.qn07_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0 = nor3(dma_reg.PULA_DMA_A13n.qn07_new(), dma_reg.POKU_DMA_A14n.qn07_new(), _LEBU_DMA_A15n_t0);
    /* p04.LOGO*/ wire _LOGO_DMA_VRAMn_t0 = not1(_MUDA_DMA_VRAMp_t0);
    /* p04.MORY*/ wire _MORY_DMA_CARTn_t0 = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _LOGO_DMA_VRAMn_t0);
    /* p04.LUMA*/ wire _LUMA_DMA_CARTp_t0 = not1(_MORY_DMA_CARTn_t0);
    /* p08.AMET*/ wire _AMET_A00p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.NAKY_DMA_A00p.qp17_new(), ext_bus.ALOR_EXT_ADDR_LATCH_00p.qp08_new());
    /* p08.ATOL*/ wire _ATOL_A01p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.PYRO_DMA_A01p.qp17_new(), ext_bus.APUR_EXT_ADDR_LATCH_01p.qp08_new());
    /* p08.APOK*/ wire _APOK_A02p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.NEFY_DMA_A02p.qp17_new(), ext_bus.ALYR_EXT_ADDR_LATCH_02p.qp08_new());
    /* p08.AMER*/ wire _AMER_A03p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.MUTY_DMA_A03p.qp17_new(), ext_bus.ARET_EXT_ADDR_LATCH_03p.qp08_new());
    /* p08.ATEM*/ wire _ATEM_A04p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.NYKO_DMA_A04p.qp17_new(), ext_bus.AVYS_EXT_ADDR_LATCH_04p.qp08_new());
    /* p08.ATOV*/ wire _ATOV_A05p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.PYLO_DMA_A05p.qp17_new(), ext_bus.ATEV_EXT_ADDR_LATCH_05p.qp08_new());
    /* p08.ATYR*/ wire _ATYR_A06p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.NUTO_DMA_A06p.qp17_new(), ext_bus.AROS_EXT_ADDR_LATCH_06p.qp08_new());
    /*#p08.ASUR*/ wire _ASUR_A07p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.MUGU_DMA_A07p.qp17_new(), ext_bus.ARYM_EXT_ADDR_LATCH_07p.qp08_new());
    /*#p08.MANO*/ wire _MANO_A08p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.NAFA_DMA_A08n.qn07_new(), ext_bus.LUNO_EXT_ADDR_LATCH_08p.qp08_new());
    /* p08.MASU*/ wire _MASU_A09p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.PYNE_DMA_A09n.qn07_new(), ext_bus.LYSA_EXT_ADDR_LATCH_09p.qp08_new());
    /* p08.PAMY*/ wire _PAMY_A10p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.PARA_DMA_A10n.qn07_new(), ext_bus.PATE_EXT_ADDR_LATCH_10p.qp08_new());
    /* p08.MALE*/ wire _MALE_A11p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.NYDO_DMA_A11n.qn07_new(), ext_bus.LUMY_EXT_ADDR_LATCH_11p.qp08_new());
    /* p08.MOJY*/ wire _MOJY_A12p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.NYGY_DMA_A12n.qn07_new(), ext_bus.LOBU_EXT_ADDR_LATCH_12p.qp08_new());
    /* p08.MUCE*/ wire _MUCE_A13p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.PULA_DMA_A13n.qn07_new(), ext_bus.LONU_EXT_ADDR_LATCH_13p.qp08_new());
    /* p08.PEGE*/ wire _PEGE_A14p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.POKU_DMA_A14n.qn07_new(), ext_bus.NYRE_EXT_ADDR_LATCH_14p.qp08_new());

    /* p08.KUPO*/ wire _KUPO = nand2(_AMET_A00p, _TOVA_MODE_DBG2n_t0);
    /* p08.CABA*/ wire _CABA = nand2(_ATOL_A01p, _TOVA_MODE_DBG2n_t0);
    /* p08.BOKU*/ wire _BOKU = nand2(_APOK_A02p, _TOVA_MODE_DBG2n_t0);
    /* p08.BOTY*/ wire _BOTY = nand2(_AMER_A03p, _TOVA_MODE_DBG2n_t0);
    /* p08.BYLA*/ wire _BYLA = nand2(_ATEM_A04p, _TOVA_MODE_DBG2n_t0);
    /* p08.BADU*/ wire _BADU = nand2(_ATOV_A05p, _TOVA_MODE_DBG2n_t0);
    /* p08.CEPU*/ wire _CEPU = nand2(_ATYR_A06p, _TOVA_MODE_DBG2n_t0);
    /* p08.DEFY*/ wire _DEFY = nand2(_ASUR_A07p, _TOVA_MODE_DBG2n_t0);
    /* p08.MYNY*/ wire _MYNY = nand2(_MANO_A08p, _TOVA_MODE_DBG2n_t0);
    /* p08.MUNE*/ wire _MUNE = nand2(_MASU_A09p, _TOVA_MODE_DBG2n_t0);
    /* p08.ROXU*/ wire _ROXU = nand2(_PAMY_A10p, _TOVA_MODE_DBG2n_t0);
    /* p08.LEPY*/ wire _LEPY = nand2(_MALE_A11p, _TOVA_MODE_DBG2n_t0);
    /* p08.LUCE*/ wire _LUCE = nand2(_MOJY_A12p, _TOVA_MODE_DBG2n_t0);
    /* p08.LABE*/ wire _LABE = nand2(_MUCE_A13p, _TOVA_MODE_DBG2n_t0);
    /* p08.PUHE*/ wire _PUHE = nand2(_PEGE_A14p, _TOVA_MODE_DBG2n_t0);

    /* p08.KOTY*/ wire _KOTY = nor2 (_AMET_A00p, _UNOR_MODE_DBG2p_t0);
    /* p08.COTU*/ wire _COTU = nor2 (_ATOL_A01p, _UNOR_MODE_DBG2p_t0);
    /* p08.BAJO*/ wire _BAJO = nor2 (_APOK_A02p, _UNOR_MODE_DBG2p_t0);
    /* p08.BOLA*/ wire _BOLA = nor2 (_AMER_A03p, _UNOR_MODE_DBG2p_t0);
    /* p08.BEVO*/ wire _BEVO = nor2 (_ATEM_A04p, _UNOR_MODE_DBG2p_t0);
    /* p08.AJAV*/ wire _AJAV = nor2 (_ATOV_A05p, _UNOR_MODE_DBG2p_t0);
    /* p08.CYKA*/ wire _CYKA = nor2 (_ATYR_A06p, _UNOR_MODE_DBG2p_t0);
    /* p08.COLO*/ wire _COLO = nor2 (_ASUR_A07p, _UNOR_MODE_DBG2p_t0);
    /* p08.MEGO*/ wire _MEGO = nor2 (_MANO_A08p, _UNOR_MODE_DBG2p_t0);
    /* p08.MENY*/ wire _MENY = nor2 (_MASU_A09p, _UNOR_MODE_DBG2p_t0);
    /* p08.RORE*/ wire _RORE = nor2 (_PAMY_A10p, _UNOR_MODE_DBG2p_t0);
    /* p08.LYNY*/ wire _LYNY = nor2 (_MALE_A11p, _UNOR_MODE_DBG2p_t0);
    /* p08.LOSO*/ wire _LOSO = nor2 (_MOJY_A12p, _UNOR_MODE_DBG2p_t0);
    /* p08.LEVA*/ wire _LEVA = nor2 (_MUCE_A13p, _UNOR_MODE_DBG2p_t0);
    /* p08.PAHY*/ wire _PAHY = nor2 (_PEGE_A14p, _UNOR_MODE_DBG2p_t0);

    // A15 is "special"
    /* p08.SOBY*/ wire _SOBY_A15n = nor2(BUS_CPU_A_t0[15], _TUTU_ADDR_BOOTp_t0);
    /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx = nand2(_ABUZ_xxCDEFGH_t1, _SOBY_A15n);
    /* p08.TAZY*/ wire _TAZY_A15p = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.MARU_DMA_A15n.qn07_new(), _SEPY_A15p_ABxxxxxx);
    /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n = nand2(_TAZY_A15p, _RYCA_MODE_DBG2n_t0);
    /* p08.RULO*/ wire _RULO_PIN_EXT_A15n = nor2 (_TAZY_A15p, _UNOR_MODE_DBG2p_t0);

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
    PIN_EXT_A[15].pin_out(1, _SUZE_PIN_EXT_A15n, _RULO_PIN_EXT_A15n);
  }

  Pin2 PIN_EXT_CLK;    // PIN_75
  {
    PIN_EXT_CLK.pin_out(1, _BUDE_xxxxEFGH_t1, _BUDE_xxxxEFGH_t1);
  }

  Pin2 PIN_EXT_RDn;    // PIN_79 // RDn idles low, goes high on phase B for an external write
  {
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(dma_reg.MARU_DMA_A15n.qn07_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0 = nor3(dma_reg.PULA_DMA_A13n.qn07_new(), dma_reg.POKU_DMA_A14n.qn07_new(), _LEBU_DMA_A15n_t0);
    /* p04.LOGO*/ wire _LOGO_DMA_VRAMn_t0 = not1(_MUDA_DMA_VRAMp_t0);
    /* p04.MORY*/ wire _MORY_DMA_CARTn_t0 = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _LOGO_DMA_VRAMn_t0);
    /* p04.LUMA*/ wire _LUMA_DMA_CARTp_t0 = not1(_MORY_DMA_CARTn_t0);
    /* p08.TYMU*/ wire _TYMU_EXT_RDn = nor2(_LUMA_DMA_CARTp_t0, _MOTY_CPU_EXT_RD_t0);
    /* p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMU_EXT_RDn, _TOVA_MODE_DBG2n_t0);
    /* p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMU_EXT_RDn, _UNOR_MODE_DBG2p_t0);
    PIN_EXT_RDn.pin_out(1, _UGAC_RD_A, _URUN_RD_D);
  }

  Pin2 PIN_EXT_WRn;    // PIN_78 // WRn idles high, goes low during EFG if there's a write
  {
    /* p08.MEXO*/ wire _MEXO_CPU_WRn_ABCDxxxH = not1(_APOV_CPU_WRp_xxxxEFGx_t1);
    /* p08.NEVY*/ wire _NEVY = or2(_MEXO_CPU_WRn_ABCDxxxH, _MOCA_DBG_EXT_RD_t0);
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(dma_reg.MARU_DMA_A15n.qn07_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0 = nor3(dma_reg.PULA_DMA_A13n.qn07_new(), dma_reg.POKU_DMA_A14n.qn07_new(), _LEBU_DMA_A15n_t0);
    /* p04.LOGO*/ wire _LOGO_DMA_VRAMn_t0 = not1(_MUDA_DMA_VRAMp_t0);
    /* p04.MORY*/ wire _MORY_DMA_CARTn_t0 = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _LOGO_DMA_VRAMn_t0);
    /* p04.LUMA*/ wire _LUMA_DMA_CARTp_t0 = not1(_MORY_DMA_CARTn_t0);
    /* p08.PUVA*/ wire _PUVA_EXT_WRn = or2(_NEVY, _LUMA_DMA_CARTp_t0);
    /* p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_WRn, _TOVA_MODE_DBG2n_t0);
    /* p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_WRn, _UNOR_MODE_DBG2p_t0);
    PIN_EXT_WRn.pin_out(1, _UVER_WR_A, _USUF_WR_D);
  }

  Pin2 PIN_EXT_CSn;    // PIN_80 // CS changes on phase C if addr in [A000,FDFF]
  {
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(dma_reg.MARU_DMA_A15n.qn07_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0 = nor3(dma_reg.PULA_DMA_A13n.qn07_new(), dma_reg.POKU_DMA_A14n.qn07_new(), _LEBU_DMA_A15n_t0);
    /* p04.LOGO*/ wire _LOGO_DMA_VRAMn_t0 = not1(_MUDA_DMA_VRAMp_t0);
    /* p04.MORY*/ wire _MORY_DMA_CARTn_t0 = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _LOGO_DMA_VRAMn_t0);
    /* p04.LUMA*/ wire _LUMA_DMA_CARTp_t0 = not1(_MORY_DMA_CARTn_t0);

    /* p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH = and3(_ABUZ_xxCDEFGH_t1, _TYNU_ADDR_RAM, _TUNA_0000_FDFFp_t0);
    /* p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH = mux2p(_LUMA_DMA_CARTp_t0, dma_reg.MARU_DMA_A15n.qn07_new(), _TOZA_PIN_EXT_CS_A_xxCDEFGH);
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

    /* p08.RORU*/ wire _RORU_CBD_TO_EPDn = mux2p(_UNOR_MODE_DBG2p_t0, _REDU_CPU_RDn_t0, _MOTY_CPU_EXT_RD_t0);
    /* p08.LULA*/ wire _LULA_CBD_TO_EPDp = not1(_RORU_CBD_TO_EPDn);

    /* p25.RUXA*/ wire _RUXA = nand2(BUS_CPU_Dp_in[0].to_wire_new(), _LULA_CBD_TO_EPDp);
    /* p25.RUJA*/ wire _RUJA = nand2(BUS_CPU_Dp_in[1].to_wire_new(), _LULA_CBD_TO_EPDp);
    /* p25.RABY*/ wire _RABY = nand2(BUS_CPU_Dp_in[2].to_wire_new(), _LULA_CBD_TO_EPDp);
    /* p25.RERA*/ wire _RERA = nand2(BUS_CPU_Dp_in[3].to_wire_new(), _LULA_CBD_TO_EPDp);
    /* p25.RORY*/ wire _RORY = nand2(BUS_CPU_Dp_in[4].to_wire_new(), _LULA_CBD_TO_EPDp);
    /* p25.RYVO*/ wire _RYVO = nand2(BUS_CPU_Dp_in[5].to_wire_new(), _LULA_CBD_TO_EPDp);
    /* p25.RAFY*/ wire _RAFY = nand2(BUS_CPU_Dp_in[6].to_wire_new(), _LULA_CBD_TO_EPDp);
    /* p25.RAVU*/ wire _RAVU = nand2(BUS_CPU_Dp_in[7].to_wire_new(), _LULA_CBD_TO_EPDp);

    /* p08.RUNE*/ wire _RUNE = nor2 (BUS_CPU_Dp_in[0].to_wire_new(), _RORU_CBD_TO_EPDn);
    /* p08.RYPU*/ wire _RYPU = nor2 (BUS_CPU_Dp_in[1].to_wire_new(), _RORU_CBD_TO_EPDn);
    /* p08.SULY*/ wire _SULY = nor2 (BUS_CPU_Dp_in[2].to_wire_new(), _RORU_CBD_TO_EPDn);
    /* p08.SEZE*/ wire _SEZE = nor2 (BUS_CPU_Dp_in[3].to_wire_new(), _RORU_CBD_TO_EPDn);
    /* p08.RESY*/ wire _RESY = nor2 (BUS_CPU_Dp_in[4].to_wire_new(), _RORU_CBD_TO_EPDn);
    /* p08.TAMU*/ wire _TAMU = nor2 (BUS_CPU_Dp_in[5].to_wire_new(), _RORU_CBD_TO_EPDn);
    /* p08.ROGY*/ wire _ROGY = nor2 (BUS_CPU_Dp_in[6].to_wire_new(), _RORU_CBD_TO_EPDn);
    /* p08.RYDA*/ wire _RYDA = nor2 (BUS_CPU_Dp_in[7].to_wire_new(), _RORU_CBD_TO_EPDn);

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

    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(dma_reg.MARU_DMA_A15n.qn07_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0 = nor3(dma_reg.PULA_DMA_A13n.qn07_new(), dma_reg.POKU_DMA_A14n.qn07_new(), _LEBU_DMA_A15n_t0);
    /* p04.LOGO*/ wire _LOGO_DMA_VRAMn_t0 = not1(_MUDA_DMA_VRAMp_t0);
    /* p04.MORY*/ wire _MORY_DMA_CARTn_t0 = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _LOGO_DMA_VRAMn_t0);
    /* p04.LUMA*/ wire _LUMA_DMA_CARTp_t0 = not1(_MORY_DMA_CARTn_t0);
    /* p25.CEDE*/ wire _CEDE_EBD_TO_OBDn = not1(_LUMA_DMA_CARTp_t0);
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

  {
    /* DMA VBD -> OBD */
    /*#p04.LEBU*/ wire _LEBU_DMA_A15n_t0 = not1(dma_reg.MARU_DMA_A15n.qn07_new());
    /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp_t0 = nor3(dma_reg.PULA_DMA_A13n.qn07_new(), dma_reg.POKU_DMA_A14n.qn07_new(), _LEBU_DMA_A15n_t0);
    /* p04.MUHO*/ wire _MUHO_DMA_VRAMp_t0 = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _MUDA_DMA_VRAMp_t0);
    /* p04.LUFA*/ wire _LUFA_DMA_VRAMp_t0 = not1(_MUHO_DMA_VRAMp_t0);
    /* p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not1(_LUFA_DMA_VRAMp_t0);
    /* p28.WUZU*/ BUS_OAM_DAn_out[0].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[0].to_wire_new());
    /* p28.AXER*/ BUS_OAM_DAn_out[1].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[1].to_wire_new());
    /* p28.ASOX*/ BUS_OAM_DAn_out[2].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[2].to_wire_new());
    /* p28.CETU*/ BUS_OAM_DAn_out[3].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[3].to_wire_new());
    /* p28.ARYN*/ BUS_OAM_DAn_out[4].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[4].to_wire_new());
    /* p28.ACOT*/ BUS_OAM_DAn_out[5].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[5].to_wire_new());
    /* p28.CUJE*/ BUS_OAM_DAn_out[6].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[6].to_wire_new());
    /* p28.ATER*/ BUS_OAM_DAn_out[7].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[7].to_wire_new());

    /* p28.WOWA*/ BUS_OAM_DBn_out[0].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[0].to_wire_new());
    /* p28.AVEB*/ BUS_OAM_DBn_out[1].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[1].to_wire_new());
    /* p28.AMUH*/ BUS_OAM_DBn_out[2].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[2].to_wire_new());
    /* p28.COFO*/ BUS_OAM_DBn_out[3].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[3].to_wire_new());
    /* p28.AZOZ*/ BUS_OAM_DBn_out[4].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[4].to_wire_new());
    /* p28.AGYK*/ BUS_OAM_DBn_out[5].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[5].to_wire_new());
    /* p28.BUSE*/ BUS_OAM_DBn_out[6].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[6].to_wire_new());
    /* p28.ANUM*/ BUS_OAM_DBn_out[7].tri6_nn(_AZAR_VBD_TO_OBDn, BUS_VRAM_Dp_in[7].to_wire_new());
  }





  {
    /* p04.DECY*/ wire _DECY_LATCH_EXTn_t0 = not1(PIN_CPU_LATCH_EXT_t0.qp());
    /* p04.CATY*/ wire _CATY_LATCH_EXTp_t0 = not1(_DECY_LATCH_EXTn_t0);
    /* p04.MAKA*/ oam_bus.MAKA_HOLD_MEMp.dff17(_ZEME_AxCxExGx_t0, _CUNU_SYS_RSTn_t0, _CATY_LATCH_EXTp_t0);
  }








  //----------------------------------------
  // OAM data out

#pragma region OAM

  Bus2 BUS_OAM_An[8];
  uint8_t oam_addr;
  {
    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_t0 = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
    /* p28.AJON*/ wire _AJON_PPU_OAM_ENp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, pix_pipe.XYMU_RENDERINGn.qn03_new()); // def AND. ppu can read oam when there's rendering but no dma
    /*#p28.ACYL*/ wire _ACYL_SCANNINGp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, sprite_scanner.BESU_SCANNINGp.qp04_new());

    /* SCAN  -> OBA */
    /* p28.APAR*/ wire _APAR_SCAN_OAM_RDn = not1(_ACYL_SCANNINGp_t2);
    /* p28.GEFY*/ BUS_OAM_An[0].tri6_nn(_APAR_SCAN_OAM_RDn, GND);
    /* p28.WUWE*/ BUS_OAM_An[1].tri6_nn(_APAR_SCAN_OAM_RDn, GND);
    /* p28.GUSE*/ BUS_OAM_An[2].tri6_nn(_APAR_SCAN_OAM_RDn, sprite_scanner.YFEL_SCAN0.qp17_old());
    /* p28.GEMA*/ BUS_OAM_An[3].tri6_nn(_APAR_SCAN_OAM_RDn, sprite_scanner.WEWY_SCAN1.qp17_old());
    /* p28.FUTO*/ BUS_OAM_An[4].tri6_nn(_APAR_SCAN_OAM_RDn, sprite_scanner.GOSO_SCAN2.qp17_old());
    /* p28.FAKU*/ BUS_OAM_An[5].tri6_nn(_APAR_SCAN_OAM_RDn, sprite_scanner.ELYN_SCAN3.qp17_old());
    /* p28.GAMA*/ BUS_OAM_An[6].tri6_nn(_APAR_SCAN_OAM_RDn, sprite_scanner.FAHA_SCAN4.qp17_old());
    /* p28.GOBY*/ BUS_OAM_An[7].tri6_nn(_APAR_SCAN_OAM_RDn, sprite_scanner.FONY_SCAN5.qp17_old());

    /* DBA   -> OBA */
    /* p04.DUGA*/ wire _DUGA_DMA_OAM_RDn  = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
    /* p28.FODO*/ BUS_OAM_An[0].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.NAKY_DMA_A00p.qp17_new());
    /* p28.FESA*/ BUS_OAM_An[1].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.PYRO_DMA_A01p.qp17_new());
    /* p28.FAGO*/ BUS_OAM_An[2].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.NEFY_DMA_A02p.qp17_new());
    /* p28.FYKY*/ BUS_OAM_An[3].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.MUTY_DMA_A03p.qp17_new());
    /* p28.ELUG*/ BUS_OAM_An[4].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.NYKO_DMA_A04p.qp17_new());
    /* p28.EDOL*/ BUS_OAM_An[5].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.PYLO_DMA_A05p.qp17_new());
    /* p28.FYDU*/ BUS_OAM_An[6].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.NUTO_DMA_A06p.qp17_new());
    /* p28.FETU*/ BUS_OAM_An[7].tri6_nn(_DUGA_DMA_OAM_RDn, dma_reg.MUGU_DMA_A07p.qp17_new());

    /* CBA   -> OBA */
    /* p28.ASAM*/ wire _ASAM_CPU_OAM_RDn  = or3(_ACYL_SCANNINGp_t2, pix_pipe.XYMU_RENDERINGn.qn03_new(), dma_reg.MATU_DMA_RUNNINGp.qp17_new());
    /* p28.GARO*/ BUS_OAM_An[0].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A_t0[ 0]);
    /* p28.WACU*/ BUS_OAM_An[1].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A_t0[ 1]);
    /* p28.GOSE*/ BUS_OAM_An[2].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A_t0[ 2]);
    /* p28.WAPE*/ BUS_OAM_An[3].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A_t0[ 3]);
    /* p28.FEVU*/ BUS_OAM_An[4].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A_t0[ 4]);
    /* p28.GERA*/ BUS_OAM_An[5].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A_t0[ 5]);
    /* p28.WAXA*/ BUS_OAM_An[6].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A_t0[ 6]);
    /* p28.FOBY*/ BUS_OAM_An[7].tri6_nn(_ASAM_CPU_OAM_RDn, BUS_CPU_A_t0[ 7]);

    /* SPR_I -> OBA */
    /* p28.BETE*/ wire _BETE_PPU_OAM_RDn  = not1(_AJON_PPU_OAM_ENp_t2);
    /* p28.GECA*/ BUS_OAM_An[0].tri6_nn(_BETE_PPU_OAM_RDn, _WEFE_VCC);
    /* p28.WYDU*/ BUS_OAM_An[1].tri6_nn(_BETE_PPU_OAM_RDn, _WEFE_VCC);
    /* p28.GYBU*/ BUS_OAM_An[2].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I0p_t2.to_wire_new());
    /* p28.GYKA*/ BUS_OAM_An[3].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I1p_t2.to_wire_new());
    /* p28.FABY*/ BUS_OAM_An[4].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I2p_t2.to_wire_new());
    /* p28.FACO*/ BUS_OAM_An[5].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I3p_t2.to_wire_new());
    /* p28.FUGU*/ BUS_OAM_An[6].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I4p_t2.to_wire_new());
    /* p28.FYKE*/ BUS_OAM_An[7].tri6_nn(_BETE_PPU_OAM_RDn, SPR_TRI_I5p_t2.to_wire_new());

    oam_addr = pack_u8n_new(7, &BUS_OAM_An[1]);
  }

  {
    {
      /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_t0 = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
      /*#p29.WOJO*/ wire _WOJO_AxxxExxxp_t1 = nor2(clk_reg.WOSU_AxxDExxHp.qn16_new(), clk_reg.WUVU_ABxxEFxxp.qn16_new());
      /* p29.XYSO*/ wire _XYSO_AxxxExxxn_t1 = not1(_WOJO_AxxxExxxp_t1);
      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, sprite_scanner.BESU_SCANNINGp.qp04_new());
      /* p25.AVER*/ wire _AVER_AxxxExxx_t2    = nand2(_ACYL_SCANNINGp_t2, _XYSO_AxxxExxxn_t1);
      /* p29.TEPA*/ wire _TEPA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp_t2 = nor3(_TEPA_RENDERINGn_t2, sprite_fetcher.TULY_SFETCH_S1p.qp17_new(), sprite_fetcher.TESE_SFETCH_S2p.qp17_new());
      /* p29.TYTU*/ wire _TYTU_SFETCH_S0n_t0 = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp17_new());
      /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG_t0 = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp17_new(), _TYTU_SFETCH_S0n_t0);
      /* p25.VAPE*/ wire _VAPE_OAM_CLKENn_t2  = and2(_TUVO_PPU_OAM_RDp_t2, _TACU_SPR_SEQ_5_TRIG_t0);
      /* p25.XUJY*/ wire _XUJY_OAM_CLKENp_t2  = not1(_VAPE_OAM_CLKENn_t2);
      /* p25.CUFE*/ wire _CUFE_OAM_CLKENp_t1  = not1(or_and3(_SARO_FE00_FEFFp_t0, dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _MOPA_xxxxEFGH_t1)); // CUFE looks like BYHA minus an inverter
      /* p25.BYCU*/ wire _BYCU_xBCDxFGH_t2    = nand3(_AVER_AxxxExxx_t2, _XUJY_OAM_CLKENp_t2, _CUFE_OAM_CLKENp_t1);
      /* p25.COTA*/ wire _COTA_AxxxxExxx_t2   = not1(_BYCU_xBCDxFGH_t2);
      Pin2 PIN_OAM_CLK_t2;
      PIN_OAM_CLK_t2.pin_in(1, _COTA_AxxxxExxx_t2);
    }

    {
      /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_t0 = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
      /* p28.AJON*/ wire _AJON_PPU_OAM_ENp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, pix_pipe.XYMU_RENDERINGn.qn03_new()); // def AND. ppu can read oam when there's rendering but no dma
      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, sprite_scanner.BESU_SCANNINGp.qp04_new());
      /* p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and2(_SARO_FE00_FEFFp_t0, _CUPA_CPU_WRp_xxxxEFGx_t1);
      /* p28.XYNY*/ wire _XYNY_ABCDxxxx_t1 = not1(_MOPA_xxxxEFGH_t1);
      /* p28.WUJE*/ oam_bus.WUJE_CPU_OAM_WRn.nor_latch(_XYNY_ABCDxxxx_t1, _XUTO_CPU_OAM_WRp);
      /* p28.XUPA*/ wire _XUPA_CPU_OAM_WRp = not1(oam_bus.WUJE_CPU_OAM_WRn.qp04_new());
      /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn      = nor3(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _ACYL_SCANNINGp_t2, _AJON_PPU_OAM_ENp_t2); // def nor
      /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(_SARO_FE00_FEFFp_t0, _AJUJ_OAM_BUSYn); // def and
      /* p28.APAG*/ wire _APAG_CBD_TO_OBDp = amux2(_XUPA_CPU_OAM_WRp, _AMAB_CPU_OAM_ENp, _AJUJ_OAM_BUSYn, _ADAH_FE00_FEFFn_t0);

      /* p28.AZUL*/ wire _AZUL_CBD_TO_OBDn = not1(_APAG_CBD_TO_OBDp);
      /* p28.ZAXA*/ BUS_OAM_DAn_out[0].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[0].to_wire_new());
      /* p28.ZAKY*/ BUS_OAM_DAn_out[1].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[1].to_wire_new());
      /* p28.WULE*/ BUS_OAM_DAn_out[2].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[2].to_wire_new());
      /* p28.ZOZO*/ BUS_OAM_DAn_out[3].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[3].to_wire_new());
      /* p28.ZUFO*/ BUS_OAM_DAn_out[4].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[4].to_wire_new());
      /* p28.ZATO*/ BUS_OAM_DAn_out[5].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[5].to_wire_new());
      /* p28.YVUC*/ BUS_OAM_DAn_out[6].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[6].to_wire_new());
      /* p28.ZUFE*/ BUS_OAM_DAn_out[7].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[7].to_wire_new());

      /* p28.ZAMY*/ BUS_OAM_DBn_out[0].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[0].to_wire_new());
      /* p28.ZOPU*/ BUS_OAM_DBn_out[1].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[1].to_wire_new());
      /* p28.WYKY*/ BUS_OAM_DBn_out[2].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[2].to_wire_new());
      /* p28.ZAJA*/ BUS_OAM_DBn_out[3].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[3].to_wire_new());
      /* p28.ZUGA*/ BUS_OAM_DBn_out[4].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[4].to_wire_new());
      /* p28.ZUMO*/ BUS_OAM_DBn_out[5].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[5].to_wire_new());
      /* p28.XYTO*/ BUS_OAM_DBn_out[6].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[6].to_wire_new());
      /* p28.ZYFA*/ BUS_OAM_DBn_out[7].tri6_nn(_AZUL_CBD_TO_OBDn, BUS_CPU_Dp_in[7].to_wire_new());
    }

    {
      /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_t0 = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
      /* p28.AJON*/ wire _AJON_PPU_OAM_ENp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, pix_pipe.XYMU_RENDERINGn.qn03_new()); // def AND. ppu can read oam when there's rendering but no dma
      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, sprite_scanner.BESU_SCANNINGp.qp04_new());
      /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(BUS_OAM_An[0].to_wire_new());
      /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn      = nor3(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _ACYL_SCANNINGp_t2, _AJON_PPU_OAM_ENp_t2); // def nor
      /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(_SARO_FE00_FEFFp_t0, _AJUJ_OAM_BUSYn); // def and
      /* p04.NAXY*/ wire _NAXY_DMA_OAM_WRp = nor2(_UVYT_ABCDxxxx_t1, oam_bus.MAKA_HOLD_MEMp.qp17_new()); // def nor2
      /* p04.POWU*/ wire _POWU_DMA_OAM_WRp  = and2(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _NAXY_DMA_OAM_WRp); // def and
      /* p04.WYJA*/ wire _WYJA_OAM_WRp   = and_or3(_AMAB_CPU_OAM_ENp, _CUPA_CPU_WRp_xxxxEFGx_t1, _POWU_DMA_OAM_WRp);
      /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);
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

    //----------------------------------------
    // PIN_CPU_LATCH_EXT _blocks_ DMA from writing to OAM? wat?

    Bus2 BUS_OAM_DAn_in[8];
    Bus2 BUS_OAM_DBn_in[8];

    {
      /* oam_ram -> OBD */
      // FIXME This should be using PIN_OAM_CLK (which might actually be PIN_OAM_CSn?)

      /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_t0 = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, sprite_scanner.BESU_SCANNINGp.qp04_new());
      /* p29.TEPA*/ wire _TEPA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp_t2 = nor3(_TEPA_RENDERINGn_t2, sprite_fetcher.TULY_SFETCH_S1p.qp17_new(), sprite_fetcher.TESE_SFETCH_S2p.qp17_new());
      /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(_ACYL_SCANNINGp_t2, _XOCE_xBCxxFGx_t1); // schematic wrong, is def nand2
      /* p28.WEFY*/ wire _WEFY_SPR_READp = and2(_TUVO_PPU_OAM_RDp_t2, sprite_fetcher.TYFO_SFETCH_S0p_D1.qp17_new());
      /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(_WEFY_SPR_READp);
      /* p04.DECY*/ wire _DECY_LATCH_EXTn_t0 = not1(PIN_CPU_LATCH_EXT_t0.qp());
      /* p04.CATY*/ wire _CATY_LATCH_EXTp_t0 = not1(_DECY_LATCH_EXTn_t0);
      /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn_t0 = not1(_CATY_LATCH_EXTp_t0);
      /*#p28.BOTA*/ wire _BOTA_OAM_OEn  = nand3(_BOFE_LATCH_EXTn_t0, _SARO_FE00_FEFFp_t0, _ASOT_CPU_RDp_t0); // Schematic wrong, this is NAND
      /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn); // def and
      /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);
      /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
      /*#p28.YRYV*/ wire _YRYU_OAM_OEp = not1(_YVAL_OAM_OEn);
      /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYU_OAM_OEp);

      Pin2 PIN_OAM_OEn;
      PIN_OAM_OEn.pin_in(1, _ZODO_OAM_OEn);

      uint8_t oam_data_a = oam_ram[(oam_addr << 1) + 0];
      uint8_t oam_data_b = oam_ram[(oam_addr << 1) + 1];

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
    }

    {
      /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_t0 = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, sprite_scanner.BESU_SCANNINGp.qp04_new());
      /* p29.TEPA*/ wire _TEPA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp_t2 = nor3(_TEPA_RENDERINGn_t2, sprite_fetcher.TULY_SFETCH_S1p.qp17_new(), sprite_fetcher.TESE_SFETCH_S2p.qp17_new());
      /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(_ACYL_SCANNINGp_t2, _XOCE_xBCxxFGx_t1); // schematic wrong, is def nand2
      /* p28.WEFY*/ wire _WEFY_SPR_READp = and2(_TUVO_PPU_OAM_RDp_t2, sprite_fetcher.TYFO_SFETCH_S0p_D1.qp17_new());
      /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(_WEFY_SPR_READp);
      /* p04.DECY*/ wire _DECY_LATCH_EXTn_t0 = not1(PIN_CPU_LATCH_EXT_t0.qp());
      /* p04.CATY*/ wire _CATY_LATCH_EXTp_t0 = not1(_DECY_LATCH_EXTn_t0);
      /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn_t0 = not1(_CATY_LATCH_EXTp_t0);
      /*#p28.BOTA*/ wire _BOTA_OAM_OEn  = nand3(_BOFE_LATCH_EXTn_t0, _SARO_FE00_FEFFp_t0, _ASOT_CPU_RDp_t0); // Schematic wrong, this is NAND
      /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_OAM_OEn); // def and
      /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);

      // OPD -> OBL
      /* p29.YDYV*/ oam_bus.YDYV_OAM_LATCH_DA0n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[0].to_wire_new());
      /* p29.YCEB*/ oam_bus.YCEB_OAM_LATCH_DA1n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[1].to_wire_new());
      /* p29.ZUCA*/ oam_bus.ZUCA_OAM_LATCH_DA2n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[2].to_wire_new());
      /* p29.WONE*/ oam_bus.WONE_OAM_LATCH_DA3n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[3].to_wire_new());
      /* p29.ZAXE*/ oam_bus.ZAXE_OAM_LATCH_DA4n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[4].to_wire_new());
      /* p29.XAFU*/ oam_bus.XAFU_OAM_LATCH_DA5n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[5].to_wire_new());
      /* p29.YSES*/ oam_bus.YSES_OAM_LATCH_DA6n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[6].to_wire_new());
      /* p29.ZECA*/ oam_bus.ZECA_OAM_LATCH_DA7n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DAn_in[7].to_wire_new());

      /*#p31.XYKY*/ oam_bus.XYKY_OAM_LATCH_DB0n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[0].to_wire_new());
      /* p31.YRUM*/ oam_bus.YRUM_OAM_LATCH_DB1n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[1].to_wire_new());
      /* p31.YSEX*/ oam_bus.YSEX_OAM_LATCH_DB2n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[2].to_wire_new());
      /* p31.YVEL*/ oam_bus.YVEL_OAM_LATCH_DB3n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[3].to_wire_new());
      /* p31.WYNO*/ oam_bus.WYNO_OAM_LATCH_DB4n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[4].to_wire_new());
      /* p31.CYRA*/ oam_bus.CYRA_OAM_LATCH_DB5n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[5].to_wire_new());
      /* p31.ZUVE*/ oam_bus.ZUVE_OAM_LATCH_DB6n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[6].to_wire_new());
      /* p31.ECED*/ oam_bus.ECED_OAM_LATCH_DB7n.tp_latch(_BODE_OAM_OEp, BUS_OAM_DBn_in[7].to_wire_new());
    }

    // OBL -> temp
    {
      /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_t0 = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
      /*#p29.WOJO*/ wire _WOJO_AxxxExxxp_t1 = nor2(clk_reg.WOSU_AxxDExxHp.qn16_new(), clk_reg.WUVU_ABxxEFxxp.qn16_new());
      /* p29.XYSO*/ wire _XYSO_AxxxExxxn_t1 = not1(_WOJO_AxxxExxxp_t1);
      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, sprite_scanner.BESU_SCANNINGp.qp04_new());
      /* p25.AVER*/ wire _AVER_AxxxExxx_t2    = nand2(_ACYL_SCANNINGp_t2, _XYSO_AxxxExxxn_t1);
      /* p29.TEPA*/ wire _TEPA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
      /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp_t2 = nor3(_TEPA_RENDERINGn_t2, sprite_fetcher.TULY_SFETCH_S1p.qp17_new(), sprite_fetcher.TESE_SFETCH_S2p.qp17_new());
      /* p29.TYTU*/ wire _TYTU_SFETCH_S0n_t0 = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp17_new());
      /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG_t0 = nand2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qp17_new(), _TYTU_SFETCH_S0n_t0);
      /* p25.VAPE*/ wire _VAPE_OAM_CLKENn_t2  = and2(_TUVO_PPU_OAM_RDp_t2, _TACU_SPR_SEQ_5_TRIG_t0);
      /* p25.XUJY*/ wire _XUJY_OAM_CLKENp_t2  = not1(_VAPE_OAM_CLKENn_t2);
      /* p25.CUFE*/ wire _CUFE_OAM_CLKENp_t1  = not1(or_and3(_SARO_FE00_FEFFp_t0, dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _MOPA_xxxxEFGH_t1)); // CUFE looks like BYHA minus an inverter
      /* p25.BYCU*/ wire _BYCU_xBCDxFGH_t2    = nand3(_AVER_AxxxExxx_t2, _XUJY_OAM_CLKENp_t2, _CUFE_OAM_CLKENp_t1);
      /* p25.COTA*/ wire _COTA_AxxxxExxx_t2   = not1(_BYCU_xBCDxFGH_t2);

      /* p29.YWOK*/ wire _YWOK_xBCDxFGH_t2    = not1(_COTA_AxxxxExxx_t2);
      /*#p29.XUSO*/ oam_bus.XUSO_OAM_DA0p.dff8n(_YWOK_xBCDxFGH_t2, oam_bus.YDYV_OAM_LATCH_DA0n.qp08_new());
      /* p29.XEGU*/ oam_bus.XEGU_OAM_DA1p.dff8n(_YWOK_xBCDxFGH_t2, oam_bus.YCEB_OAM_LATCH_DA1n.qp08_new());
      /* p29.YJEX*/ oam_bus.YJEX_OAM_DA2p.dff8n(_YWOK_xBCDxFGH_t2, oam_bus.ZUCA_OAM_LATCH_DA2n.qp08_new());
      /* p29.XYJU*/ oam_bus.XYJU_OAM_DA3p.dff8n(_YWOK_xBCDxFGH_t2, oam_bus.WONE_OAM_LATCH_DA3n.qp08_new());
      /* p29.YBOG*/ oam_bus.YBOG_OAM_DA4p.dff8n(_YWOK_xBCDxFGH_t2, oam_bus.ZAXE_OAM_LATCH_DA4n.qp08_new());
      /* p29.WYSO*/ oam_bus.WYSO_OAM_DA5p.dff8n(_YWOK_xBCDxFGH_t2, oam_bus.XAFU_OAM_LATCH_DA5n.qp08_new());
      /* p29.XOTE*/ oam_bus.XOTE_OAM_DA6p.dff8n(_YWOK_xBCDxFGH_t2, oam_bus.YSES_OAM_LATCH_DA6n.qp08_new());
      /* p29.YZAB*/ oam_bus.YZAB_OAM_DA7p.dff8n(_YWOK_xBCDxFGH_t2, oam_bus.ZECA_OAM_LATCH_DA7n.qp08_new());

      /* p31.XEGA*/ wire _XEGA_xBCDxFGH_t2    = not1(_COTA_AxxxxExxx_t2);
      /* p31.YLOR*/ oam_bus.YLOR_OAM_DB0p.dff8n(_XEGA_xBCDxFGH_t2, oam_bus.XYKY_OAM_LATCH_DB0n.qp08_new());
      /* p31.ZYTY*/ oam_bus.ZYTY_OAM_DB1p.dff8n(_XEGA_xBCDxFGH_t2, oam_bus.YRUM_OAM_LATCH_DB1n.qp08_new());
      /* p31.ZYVE*/ oam_bus.ZYVE_OAM_DB2p.dff8n(_XEGA_xBCDxFGH_t2, oam_bus.YSEX_OAM_LATCH_DB2n.qp08_new());
      /* p31.ZEZY*/ oam_bus.ZEZY_OAM_DB3p.dff8n(_XEGA_xBCDxFGH_t2, oam_bus.YVEL_OAM_LATCH_DB3n.qp08_new());
      /* p31.GOMO*/ oam_bus.GOMO_OAM_DB4p.dff8n(_XEGA_xBCDxFGH_t2, oam_bus.WYNO_OAM_LATCH_DB4n.qp08_new());
      /* p31.BAXO*/ oam_bus.BAXO_OAM_DB5p.dff8n(_XEGA_xBCDxFGH_t2, oam_bus.CYRA_OAM_LATCH_DB5n.qp08_new());
      /* p31.YZOS*/ oam_bus.YZOS_OAM_DB6p.dff8n(_XEGA_xBCDxFGH_t2, oam_bus.ZUVE_OAM_LATCH_DB6n.qp08_new());
      /* p31.DEPO*/ oam_bus.DEPO_OAM_DB7p.dff8n(_XEGA_xBCDxFGH_t2, oam_bus.ECED_OAM_LATCH_DB7n.qp08_new());
    }

    {
      /* OBL  -> CBD */
      /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_t0 = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
      /* p28.AJON*/ wire _AJON_PPU_OAM_ENp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, pix_pipe.XYMU_RENDERINGn.qn03_new()); // def AND. ppu can read oam when there's rendering but no dma
      /*#p28.ACYL*/ wire _ACYL_SCANNINGp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, sprite_scanner.BESU_SCANNINGp.qp04_new());
      /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(BUS_OAM_An[0].to_wire_new());
      /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn      = nor3(dma_reg.MATU_DMA_RUNNINGp.qp17_new(), _ACYL_SCANNINGp_t2, _AJON_PPU_OAM_ENp_t2); // def nor
      /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(_SARO_FE00_FEFFp_t0, _AJUJ_OAM_BUSYn); // def and
      /* p04.DECY*/ wire _DECY_LATCH_EXTn_t0 = not1(PIN_CPU_LATCH_EXT_t0.qp());
      /* p04.CATY*/ wire _CATY_LATCH_EXTp_t0 = not1(_DECY_LATCH_EXTn_t0);
      /* p28.MYNU*/ wire _MYNU_CPU_RDn_t0    = nand2(_ASOT_CPU_RDp_t0, _CATY_LATCH_EXTp_t0);
      /* p28.LEKO*/ wire _LEKO_CPU_RDp_t0    = not1(_MYNU_CPU_RDn_t0);
      /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and3(_LEKO_CPU_RDp_t0, _AMAB_CPU_OAM_ENp, _GEKA_OAM_A0p);
      /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not1(_WUKU_OBL_TO_CBDp);
      /* p31.XACA*/ BUS_CPU_Dp_out[0].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.XYKY_OAM_LATCH_DB0n.qn10_new());
      /* p31.XAGU*/ BUS_CPU_Dp_out[1].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.YRUM_OAM_LATCH_DB1n.qn10_new());
      /* p31.XEPU*/ BUS_CPU_Dp_out[2].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.YSEX_OAM_LATCH_DB2n.qn10_new());
      /* p31.XYGU*/ BUS_CPU_Dp_out[3].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.YVEL_OAM_LATCH_DB3n.qn10_new());
      /* p31.XUNA*/ BUS_CPU_Dp_out[4].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.WYNO_OAM_LATCH_DB4n.qn10_new());
      /* p31.DEVE*/ BUS_CPU_Dp_out[5].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.CYRA_OAM_LATCH_DB5n.qn10_new());
      /* p31.ZEHA*/ BUS_CPU_Dp_out[6].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.ZUVE_OAM_LATCH_DB6n.qn10_new());
      /* p31.FYRA*/ BUS_CPU_Dp_out[7].tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.ECED_OAM_LATCH_DB7n.qn10_new());

      /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);
      /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and3(_LEKO_CPU_RDp_t0, _AMAB_CPU_OAM_ENp, _WAFO_OAM_A0n);
      /* p28.WUME*/ wire _WUME_OBL_TO_CBDn = not1(_GUKO_OBL_TO_CBDp);
      /* p29.YFAP*/ BUS_CPU_Dp_out[0].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.YDYV_OAM_LATCH_DA0n.qn10_new());
      /* p29.XELE*/ BUS_CPU_Dp_out[1].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.YCEB_OAM_LATCH_DA1n.qn10_new());
      /* p29.YPON*/ BUS_CPU_Dp_out[2].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.ZUCA_OAM_LATCH_DA2n.qn10_new());
      /* p29.XUVO*/ BUS_CPU_Dp_out[3].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.WONE_OAM_LATCH_DA3n.qn10_new());
      /* p29.ZYSA*/ BUS_CPU_Dp_out[4].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.ZAXE_OAM_LATCH_DA4n.qn10_new());
      /* p29.YWEG*/ BUS_CPU_Dp_out[5].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.XAFU_OAM_LATCH_DA5n.qn10_new());
      /* p29.XABU*/ BUS_CPU_Dp_out[6].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.YSES_OAM_LATCH_DA6n.qn10_new());
      /* p29.YTUX*/ BUS_CPU_Dp_out[7].tri10_np(_WUME_OBL_TO_CBDn, oam_bus.ZECA_OAM_LATCH_DA7n.qn10_new());
    }
  }
#pragma endregion

#pragma region Sprite_Scanner
  {
    /* p28.YFOT*/ wire _YFOT_OAM_A2p = not1(BUS_OAM_An[2].to_wire_new());
    /* p28.YFOC*/ wire _YFOC_OAM_A3p = not1(BUS_OAM_An[3].to_wire_new());
    /* p28.YVOM*/ wire _YVOM_OAM_A4p = not1(BUS_OAM_An[4].to_wire_new());
    /* p28.YMEV*/ wire _YMEV_OAM_A5p = not1(BUS_OAM_An[5].to_wire_new());
    /* p28.XEMU*/ wire _XEMU_OAM_A6p = not1(BUS_OAM_An[6].to_wire_new());
    /* p28.YZET*/ wire _YZET_OAM_A7p = not1(BUS_OAM_An[7].to_wire_new());

    /*#p30.CYKE*/ wire _CYKE_ABxxEFxx_t1 = not1(_XUPY_ABxxEFxx_t1);
    /*#p30.WUDA*/ wire _WUDA_xxCDxxGH_t1 = not1(_CYKE_ABxxEFxx_t1);
    /* p30.XADU*/ sprite_scanner.XADU_SPRITE_IDX0p.dff13(_WUDA_xxCDxxGH_t1, _WEFE_VCC, _YFOT_OAM_A2p);
    /* p30.XEDY*/ sprite_scanner.XEDY_SPRITE_IDX1p.dff13(_WUDA_xxCDxxGH_t1, _WEFE_VCC, _YFOC_OAM_A3p);
    /* p30.ZUZE*/ sprite_scanner.ZUZE_SPRITE_IDX2p.dff13(_WUDA_xxCDxxGH_t1, _WEFE_VCC, _YVOM_OAM_A4p);
    /* p30.XOBE*/ sprite_scanner.XOBE_SPRITE_IDX3p.dff13(_WUDA_xxCDxxGH_t1, _WEFE_VCC, _YMEV_OAM_A5p);
    /* p30.YDUF*/ sprite_scanner.YDUF_SPRITE_IDX4p.dff13(_WUDA_xxCDxxGH_t1, _WEFE_VCC, _XEMU_OAM_A6p);
    /* p30.XECU*/ sprite_scanner.XECU_SPRITE_IDX5p.dff13(_WUDA_xxCDxxGH_t1, _WEFE_VCC, _YZET_OAM_A7p);

    // 32 + 4 + 2 + 1 = 39
    /*#p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and4(sprite_scanner.YFEL_SCAN0.qp17_old(), sprite_scanner.WEWY_SCAN1.qp17_old(), sprite_scanner.GOSO_SCAN2.qp17_old(), sprite_scanner.FONY_SCAN5.qp17_old());
    /* p28.GAVA*/ wire _GAVA_SCAN_CLK = or2(_FETO_SCAN_DONE_d0, _XUPY_ABxxEFxx_t1);

    /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_Bp.dff17(_ALET_xBxDxFxH_t0, _BAGY_LINE_RSTn_t2, sprite_scanner.BYBA_SCAN_DONE_Ap.qp17_old());
    /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_Ap.dff17(_XUPY_ABxxEFxx_t1, _BAGY_LINE_RSTn_t2, _FETO_SCAN_DONE_d0);
    /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGp   .dff17(_XUPY_ABxxEFxx_t1, _ABEZ_VID_RSTn_t0,  sprite_scanner.BESU_SCANNINGp.qp04_new());

    /* p28.YFEL*/ sprite_scanner.YFEL_SCAN0.dff17(_GAVA_SCAN_CLK,                       _ANOM_LINE_RSTn_t2, sprite_scanner.YFEL_SCAN0.qn16_old());
    /* p28.WEWY*/ sprite_scanner.WEWY_SCAN1.dff17(sprite_scanner.YFEL_SCAN0.qn16_new(), _ANOM_LINE_RSTn_t2, sprite_scanner.WEWY_SCAN1.qn16_old());
    /* p28.GOSO*/ sprite_scanner.GOSO_SCAN2.dff17(sprite_scanner.WEWY_SCAN1.qn16_new(), _ANOM_LINE_RSTn_t2, sprite_scanner.GOSO_SCAN2.qn16_old());
    /* p28.ELYN*/ sprite_scanner.ELYN_SCAN3.dff17(sprite_scanner.GOSO_SCAN2.qn16_new(), _ANOM_LINE_RSTn_t2, sprite_scanner.ELYN_SCAN3.qn16_old());
    /* p28.FAHA*/ sprite_scanner.FAHA_SCAN4.dff17(sprite_scanner.ELYN_SCAN3.qn16_new(), _ANOM_LINE_RSTn_t2, sprite_scanner.FAHA_SCAN4.qn16_old());
    /* p28.FONY*/ sprite_scanner.FONY_SCAN5.dff17(sprite_scanner.FAHA_SCAN4.qn16_new(), _ANOM_LINE_RSTn_t2, sprite_scanner.FONY_SCAN5.qn16_old());
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

    /* p06.CULY*/ ser_reg.CULY_XFER_DIR.dff17_ff(_UWAM_FF02_WRn, BUS_CPU_Dp_in[0].to_wire_new());
    /* p06.CULY*/ ser_reg.CULY_XFER_DIR.dff17_rst(_ALUR_SYS_RSTn_t0);

    /* p01.UVYN*/ wire _UVYN_DIV_05n_next = not1(tim_reg.TAMA_DIV05p.qp17_new());
    /* p06.COTY*/ ser_reg.COTY_SER_CLK.dff17_ff(_UVYN_DIV_05n_next, ser_reg.COTY_SER_CLK.qn16_old());
    /* p06.COTY*/ ser_reg.COTY_SER_CLK.dff17_rst(_UWAM_FF02_WRn);

    {
      /* p06.KEXU*/ wire _KEXU = nand2(ser_reg.COTY_SER_CLK.qp17_new(), ser_reg.CULY_XFER_DIR.qp17_new());
      /* p06.JAGO*/ wire _JAGO = not1(ser_reg.CULY_XFER_DIR.qp17_new());
      /* p06.KUJO*/ wire _KUJO = nor2(ser_reg.COTY_SER_CLK.qp17_new(), _JAGO);

      // FIXME this doesn't look quite right
      /* p06.KEXU*/ PIN_SCK.pin_out(_KEXU, _KUJO, ser_reg.CULY_XFER_DIR.qp17_new());
    }

    /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn = nand2(BUS_CPU_Dp_in[0].to_wire_new(), _DAKU_FF01_WRp);
    /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn = nand2(BUS_CPU_Dp_in[1].to_wire_new(), _DAKU_FF01_WRp);
    /* p06.DELA*/ wire _DELA_SER_DATA2_SETn = nand2(BUS_CPU_Dp_in[2].to_wire_new(), _DAKU_FF01_WRp);
    /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn = nand2(BUS_CPU_Dp_in[3].to_wire_new(), _DAKU_FF01_WRp);
    /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn = nand2(BUS_CPU_Dp_in[4].to_wire_new(), _DAKU_FF01_WRp);
    /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn = nand2(BUS_CPU_Dp_in[5].to_wire_new(), _DAKU_FF01_WRp);
    /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn = nand2(BUS_CPU_Dp_in[6].to_wire_new(), _DAKU_FF01_WRp);
    /* p06.EFEF*/ wire _EFEL_SER_DATA7_SETn = nand2(BUS_CPU_Dp_in[7].to_wire_new(), _DAKU_FF01_WRp);

    /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn = or_and3(_URYS_FF01_WRn, BUS_CPU_Dp_in[0].to_wire_new(), _ALUR_SYS_RSTn_t0);
    /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn = or_and3(_URYS_FF01_WRn, BUS_CPU_Dp_in[1].to_wire_new(), _ALUR_SYS_RSTn_t0);
    /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn = or_and3(_URYS_FF01_WRn, BUS_CPU_Dp_in[2].to_wire_new(), _ALUR_SYS_RSTn_t0);
    /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn = or_and3(_URYS_FF01_WRn, BUS_CPU_Dp_in[3].to_wire_new(), _ALUR_SYS_RSTn_t0);
    /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn = or_and3(_URYS_FF01_WRn, BUS_CPU_Dp_in[4].to_wire_new(), _ALUR_SYS_RSTn_t0);
    /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn = or_and3(_URYS_FF01_WRn, BUS_CPU_Dp_in[5].to_wire_new(), _ALUR_SYS_RSTn_t0);
    /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn = or_and3(_URYS_FF01_WRn, BUS_CPU_Dp_in[6].to_wire_new(), _ALUR_SYS_RSTn_t0);
    /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn = or_and3(_URYS_FF01_WRn, BUS_CPU_Dp_in[7].to_wire_new(), _ALUR_SYS_RSTn_t0);

    /* p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(ser_reg.CULY_XFER_DIR.qp17_new(), ser_reg.COTY_SER_CLK.qp17_new(), PIN_SCK.qn());
    /* p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, ser_reg.ETAF_SER_RUNNING.qn16_old()); // this must stop the clock or something when the transmit's done
    /* p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
    /* p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
    /* p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
    /* p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

    /* p06.CARO*/ wire _CARO_SER_RST = and2(_UWAM_FF02_WRn, _ALUR_SYS_RSTn_t0);
    /* p06.CAFA*/ ser_reg.CAFA_SER_CNT0.dff17_ff(_DAWA_SER_CLK,                    ser_reg.CAFA_SER_CNT0.qn16_old());
    /* p06.CYLO*/ ser_reg.CYLO_SER_CNT1.dff17_ff(ser_reg.CAFA_SER_CNT0.qn16_new(), ser_reg.CYLO_SER_CNT1.qn16_old());
    /* p06.CYDE*/ ser_reg.CYDE_SER_CNT2.dff17_ff(ser_reg.CYLO_SER_CNT1.qn16_new(), ser_reg.CYDE_SER_CNT2.qn16_old());
    /* p06.CALY*/ ser_reg.CALY_SER_CNT3.dff17_ff(ser_reg.CYDE_SER_CNT2.qn16_new(), ser_reg.CALY_SER_CNT3.qn16_old());

    /* p06.CAFA*/ ser_reg.CAFA_SER_CNT0.dff17_rst(_CARO_SER_RST);
    /* p06.CYLO*/ ser_reg.CYLO_SER_CNT1.dff17_rst(_CARO_SER_RST);
    /* p06.CYDE*/ ser_reg.CYDE_SER_CNT2.dff17_rst(_CARO_SER_RST);
    /* p06.CALY*/ ser_reg.CALY_SER_CNT3.dff17_rst(_CARO_SER_RST);

    /* p06.CAGE*/ wire _CAGE_SER_IN  = not1(PIN_SIN.qn());

    /* p06.ELYS*/ ser_reg.ELYS_SER_OUT.dff17_ff(_EDYL_SER_CLK, ser_reg.EDER_SER_DATA7.qp16_old());
    /* p06.ELYS*/ ser_reg.ELYS_SER_OUT.dff17_rst(_ALUR_SYS_RSTn_t0);

    /* p06.EDER*/ ser_reg.EDER_SER_DATA7.dff22_ff(_EPYT_SER_CLK, ser_reg.EROD_SER_DATA6.qp16_old());
    /* p06.EROD*/ ser_reg.EROD_SER_DATA6.dff22_ff(_EPYT_SER_CLK, ser_reg.EJAB_SER_DATA5.qp16_old());
    /* p06.EJAB*/ ser_reg.EJAB_SER_DATA5.dff22_ff(_EPYT_SER_CLK, ser_reg.DOVU_SER_DATA4.qp16_old());
    /* p06.DOVU*/ ser_reg.DOVU_SER_DATA4.dff22_ff(_EPYT_SER_CLK, ser_reg.DOJO_SER_DATA3.qp16_old());
    /* p06.DOJO*/ ser_reg.DOJO_SER_DATA3.dff22_ff(_DAWE_SER_CLK, ser_reg.DYRA_SER_DATA2.qp16_old());
    /* p06.DYRA*/ ser_reg.DYRA_SER_DATA2.dff22_ff(_DAWE_SER_CLK, ser_reg.DEGU_SER_DATA1.qp16_old());
    /* p06.DEGU*/ ser_reg.DEGU_SER_DATA1.dff22_ff(_DAWE_SER_CLK, ser_reg.CUBA_SER_DATA0.qp16_old());
    /* p06.CUBA*/ ser_reg.CUBA_SER_DATA0.dff22_ff(_DAWE_SER_CLK, _CAGE_SER_IN);

    /* p06.EDER*/ ser_reg.EDER_SER_DATA7.dff22_set_rst(_EFEL_SER_DATA7_SETn, _EGUV_SER_DATA7_RSTn);
    /* p06.EROD*/ ser_reg.EROD_SER_DATA6.dff22_set_rst(_EDEL_SER_DATA6_SETn, _EFAK_SER_DATA6_RSTn);
    /* p06.EJAB*/ ser_reg.EJAB_SER_DATA5.dff22_set_rst(_ELOK_SER_DATA5_SETn, _EHUJ_SER_DATA5_RSTn);
    /* p06.DOVU*/ ser_reg.DOVU_SER_DATA4.dff22_set_rst(_DOLA_SER_DATA4_SETn, _DYLY_SER_DATA4_RSTn);
    /* p06.DOJO*/ ser_reg.DOJO_SER_DATA3.dff22_set_rst(_DYGE_SER_DATA3_SETn, _DAJU_SER_DATA3_RSTn);
    /* p06.DYRA*/ ser_reg.DYRA_SER_DATA2.dff22_set_rst(_DELA_SER_DATA2_SETn, _DYBO_SER_DATA2_RSTn);
    /* p06.DEGU*/ ser_reg.DEGU_SER_DATA1.dff22_set_rst(_DOCU_SER_DATA1_SETn, _DUMO_SER_DATA1_RSTn);
    /* p06.CUBA*/ ser_reg.CUBA_SER_DATA0.dff22_set_rst(_CUFU_SER_DATA0_SETn, _COHY_SER_DATA0_RSTn);

    // FIXME hacking out debug stuff
    ///* p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);
    /* p05.KENA*/ PIN_SOUT.pin_out(1, ser_reg.ELYS_SER_OUT.qp17_new(), ser_reg.ELYS_SER_OUT.qp17_new());

    /* p06.COBA*/ wire _COBA_SER_CNT3n  = not1(ser_reg.CALY_SER_CNT3.qp17_new());
    /* p06.CABY*/ wire _CABY_XFER_RESET = and2(_COBA_SER_CNT3n, _ALUR_SYS_RSTn_t0);
    /* p06.ETAF*/ ser_reg.ETAF_SER_RUNNING.dff17_ff(_UWAM_FF02_WRn, BUS_CPU_Dp_in[7].to_wire_new());
    /* p06.ETAF*/ ser_reg.ETAF_SER_RUNNING.dff17_rst(_CABY_XFER_RESET);
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

    /* p02.LETY*/ wire _LETY_INT_VBL_ACKn_t0  = not1(PIN_CPU_ACK_VBLANK_t0.qp());
    /* p02.LEJA*/ wire _LEJA_INT_STAT_ACKn_t0 = not1(PIN_CPU_ACK_STAT_t0.qp());
    /* p02.LESA*/ wire _LESA_INT_TIM_ACKn_t0  = not1(PIN_CPU_ACK_TIMER_t0.qp());
    /* p02.LUFE*/ wire _LUFE_INT_SER_ACKn_t0  = not1(PIN_CPU_ACK_SERIAL_t0.qp());
    /* p02.LAMO*/ wire _LAMO_INT_JOY_ACKn_t0  = not1(PIN_CPU_ACK_JOYPAD_t0.qp());

    /* p02.MYZU*/ wire _MYZU_FF0F_SET0n_t1 = nand3(_ROTU_FF0F_WRp_t1, _LETY_INT_VBL_ACKn_t0,  BUS_CPU_Dp_in[0].to_wire_new());
    /* p02.MODY*/ wire _MODY_FF0F_SET1n_t1 = nand3(_ROTU_FF0F_WRp_t1, _LEJA_INT_STAT_ACKn_t0, BUS_CPU_Dp_in[1].to_wire_new());
    /* p02.PYHU*/ wire _PYHU_FF0F_SET2n_t1 = nand3(_ROTU_FF0F_WRp_t1, _LESA_INT_TIM_ACKn_t0,  BUS_CPU_Dp_in[2].to_wire_new());
    /* p02.TOME*/ wire _TOME_FF0F_SET3n_t1 = nand3(_ROTU_FF0F_WRp_t1, _LUFE_INT_SER_ACKn_t0,  BUS_CPU_Dp_in[3].to_wire_new());
    /* p02.TOGA*/ wire _TOGA_FF0F_SET4n_t1 = nand3(_ROTU_FF0F_WRp_t1, _LAMO_INT_JOY_ACKn_t0,  BUS_CPU_Dp_in[4].to_wire_new());

    /*#p02.MUXE*/ wire _MUXE_INT0_WRn_t1 = or2(BUS_CPU_Dp_in[0].to_wire_new(), _REFA_FF0F_WRn_t1);
    /* p02.NABE*/ wire _NABE_INT1_WRn_t1 = or2(BUS_CPU_Dp_in[1].to_wire_new(), _REFA_FF0F_WRn_t1);
    /* p02.RAKE*/ wire _RAKE_INT2_WRn_t1 = or2(BUS_CPU_Dp_in[2].to_wire_new(), _REFA_FF0F_WRn_t1);
    /* p02.SULO*/ wire _SULO_INT3_WRn_t1 = or2(BUS_CPU_Dp_in[3].to_wire_new(), _REFA_FF0F_WRn_t1);
    /* p02.SEME*/ wire _SEME_INT4_WRn_t1 = or2(BUS_CPU_Dp_in[4].to_wire_new(), _REFA_FF0F_WRn_t1);

    /*#p02.LYTA*/ wire _LYTA_FF0F_RST0n_t1 = and3(_MUXE_INT0_WRn_t1, _LETY_INT_VBL_ACKn_t0,  _ALUR_SYS_RSTn_t0);
    /* p02.MOVU*/ wire _MOVU_FF0F_RST1n_t1 = and3(_NABE_INT1_WRn_t1, _LEJA_INT_STAT_ACKn_t0, _ALUR_SYS_RSTn_t0);
    /* p02.PYGA*/ wire _PYGA_FF0F_RST2n_t1 = and3(_RAKE_INT2_WRn_t1, _LESA_INT_TIM_ACKn_t0,  _ALUR_SYS_RSTn_t0);
    /* p02.TUNY*/ wire _TUNY_FF0F_RST3n_t1 = and3(_SULO_INT3_WRn_t1, _LUFE_INT_SER_ACKn_t0,  _ALUR_SYS_RSTn_t0);
    /* p02.TYME*/ wire _TYME_FF0F_RST4n_t1 = and3(_SEME_INT4_WRn_t1, _LAMO_INT_JOY_ACKn_t0,  _ALUR_SYS_RSTn_t0);

    /*#p21.TAPA*/ wire _TAPA_INT_OAM_t3   = and2(_TOLU_VBLANKn_t3, _SELA_LINE_P908p_t2);
    /*#p21.TARU*/ wire _TARU_INT_HBL_t3   = and2(_WODU_HBLANKp_t0, _TOLU_VBLANKn_t3);
    /*#p21.SUKO*/ wire _SUKO_INT_STATp_t3 = amux4(pix_pipe.RUGU_STAT_LYI_ENn.qn08_new(), lcd_reg.ROPO_LY_MATCH_SYNCp.qp17_old(),
                                                  pix_pipe.REFE_STAT_OAI_ENn.qn08_new(), _TAPA_INT_OAM_t3,
                                                  pix_pipe.RUFO_STAT_VBI_ENn.qn08_new(), _PARU_VBLANKp_t3, // polarity?
                                                  pix_pipe.ROXE_STAT_HBI_ENn.qn08_new(), _TARU_INT_HBL_t3);

    /*#p21.VYPU*/ wire _VYPU_INT_VBLANKp_t3 = not1(_TOLU_VBLANKn_t3);
    /* p02.ASOK*/ wire _ASOK_INT_JOYp       = and2(joypad.APUG_JP_GLITCH3.qp17_new(), joypad.BATU_JP_GLITCH0.qp17_new());
    /*#p21.TUVA*/ wire _TUVA_INT_STATn_t3   = not1(_SUKO_INT_STATp_t3);
    /*#p21.VOTY*/ wire _VOTY_INT_STATp_t3   = not1(_TUVA_INT_STATn_t3);

    /* p02.LOPE*/ int_reg.LOPE_FF0F_D0p.dff22_ff(_VYPU_INT_VBLANKp_t3, _PESU_VCC);
    /* p02.LALU*/ int_reg.LALU_FF0F_D1p.dff22_ff(_VOTY_INT_STATp_t3, _PESU_VCC);
    /* p02.NYBO*/ int_reg.NYBO_FF0F_D2p.dff22_ff(tim_reg.MOBA_TIMER_OVERFLOWp.qp17_new(), _PESU_VCC);
    /* p02.UBUL*/ int_reg.UBUL_FF0F_D3p.dff22_ff(ser_reg.CALY_SER_CNT3.qp17_new(), _PESU_VCC); // FIXME should use new
    /* p02.ULAK*/ int_reg.ULAK_FF0F_D4p.dff22_ff(_ASOK_INT_JOYp, _PESU_VCC);

    /* p02.LOPE*/ int_reg.LOPE_FF0F_D0p.dff22_set_rst(_MYZU_FF0F_SET0n_t1, _LYTA_FF0F_RST0n_t1);
    /* p02.LALU*/ int_reg.LALU_FF0F_D1p.dff22_set_rst(_MODY_FF0F_SET1n_t1, _MOVU_FF0F_RST1n_t1);
    /* p02.NYBO*/ int_reg.NYBO_FF0F_D2p.dff22_set_rst(_PYHU_FF0F_SET2n_t1, _PYGA_FF0F_RST2n_t1);
    /* p02.UBUL*/ int_reg.UBUL_FF0F_D3p.dff22_set_rst(_TOME_FF0F_SET3n_t1, _TUNY_FF0F_RST3n_t1);
    /* p02.ULAK*/ int_reg.ULAK_FF0F_D4p.dff22_set_rst(_TOGA_FF0F_SET4n_t1, _TYME_FF0F_RST4n_t1);

    /* p02.LOPE*/ wire _LOPE_FF0F_D0p_t4 = int_reg.LOPE_FF0F_D0p.qp16_new();
    /* p02.LALU*/ wire _LALU_FF0F_D1p_t4 = int_reg.LALU_FF0F_D1p.qp16_new();
    /* p02.NYBO*/ wire _NYBO_FF0F_D2p_t4 = int_reg.NYBO_FF0F_D2p.qp16_new();
    /* p02.UBUL*/ wire _UBUL_FF0F_D3p_t4 = int_reg.UBUL_FF0F_D3p.qp16_new();
    /* p02.ULAK*/ wire _ULAK_FF0F_D4p_t4 = int_reg.ULAK_FF0F_D4p.qp16_new();

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
      wire FFFF_WRn = nand2(_TAPU_CPU_WRp_xxxxEFGx_t1, FFFF_HIT);

      IE_D0.dff(FFFF_WRn, BUS_CPU_Dp_in[0].to_wire_new());
      IE_D1.dff(FFFF_WRn, BUS_CPU_Dp_in[1].to_wire_new());
      IE_D2.dff(FFFF_WRn, BUS_CPU_Dp_in[2].to_wire_new());
      IE_D3.dff(FFFF_WRn, BUS_CPU_Dp_in[3].to_wire_new());
      IE_D4.dff(FFFF_WRn, BUS_CPU_Dp_in[4].to_wire_new());

      IE_D0.dff_RSTn(!sys_rst);
      IE_D1.dff_RSTn(!sys_rst);
      IE_D2.dff_RSTn(!sys_rst);
      IE_D3.dff_RSTn(!sys_rst);
      IE_D4.dff_RSTn(!sys_rst);

      if (DELTA_HA) {
        imask_latch = pack_u8p_new(5, &IE_D0);
      }
    }
  }
#pragma endregion

  //------------------------------------------------------------------------------

  {
    /* p27.SEKO*/ wire _SEKO_FETCH_TRIGp_t0 = nor2(pix_pipe.RYFA_FETCHn_A.qn16_old(), pix_pipe.RENE_FETCHn_B.qp17_old());
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_t0 = not1(pix_pipe.RYDY_WIN_HITp.to_wire_new());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_t0 = nand2(_SYLO_WIN_HITn_t0, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17_old());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_t0 = not1(_TUXY_WIN_FIRST_TILEne_t0);
    /* p27.TEVO*/ wire _TEVO_FETCH_TRIGp_t2 = or3(_SEKO_FETCH_TRIGp_t0, _SUZU_WIN_FIRST_TILEne_t0, _TAVE_PRELOAD_DONE_TRIGp_t2); // Schematic wrong, this is OR
    /* p27.NYXU*/ wire _NYXU_FETCH_TRIGn_t2 = nor3(_AVAP_SCAN_DONE_TRIGp_t2, _MOSU_WIN_FETCH_TRIGp_t0, _TEVO_FETCH_TRIGp_t2);
    /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn_t2 = nand3(tile_fetcher.LAXU_BFETCH_S0p.qp17_old(), tile_fetcher.NYVA_BFETCH_S2p.qp17_old(), _NYXU_FETCH_TRIGn_t2);
    /* p27.LEBO*/ wire _LEBO_AxCxExGx_t2 = nand2(_ALET_xBxDxFxH_t0, _MOCE_BFETCH_DONEn_t2);

    /* p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0p_D1.dff17_ff(_ALET_xBxDxFxH_t0, tile_fetcher.LAXU_BFETCH_S0p.qp17_old());
    /* p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0p_D1.dff17_rst(pix_pipe.XYMU_RENDERINGn.qn03_new());

    /* p27.LAXU*/ tile_fetcher.LAXU_BFETCH_S0p.dff17_ff(_LEBO_AxCxExGx_t2,                       tile_fetcher.LAXU_BFETCH_S0p.qn16_old());
    /* p27.MESU*/ tile_fetcher.MESU_BFETCH_S1p.dff17_ff(tile_fetcher.LAXU_BFETCH_S0p.qn16_new(), tile_fetcher.MESU_BFETCH_S1p.qn16_old());
    /* p27.NYVA*/ tile_fetcher.NYVA_BFETCH_S2p.dff17_ff(tile_fetcher.MESU_BFETCH_S1p.qn16_new(), tile_fetcher.NYVA_BFETCH_S2p.qn16_old());

    /* p27.LAXU*/ tile_fetcher.LAXU_BFETCH_S0p.dff17_rst(_NYXU_FETCH_TRIGn_t2);
    /* p27.MESU*/ tile_fetcher.MESU_BFETCH_S1p.dff17_rst(_NYXU_FETCH_TRIGn_t2);
    /* p27.NYVA*/ tile_fetcher.NYVA_BFETCH_S2p.dff17_rst(_NYXU_FETCH_TRIGn_t2);

    /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp_t2 = not1(_MOCE_BFETCH_DONEn_t2);

    /* p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp.dff17_ff(_ALET_xBxDxFxH_t0, tile_fetcher.PORY_FETCH_DONEp.qp17_old());
    /* p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONEp.dff17_rst(pix_pipe.XYMU_RENDERINGn.qn03_new());

    /* p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG_t2 = nor2(_MOSU_WIN_FETCH_TRIGp_t0, _LOBY_RENDERINGn_t2);
    /* p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp.dff17_ff(_MYVO_AxCxExGx_t0, tile_fetcher.NYKA_FETCH_DONEp.qp17_old());
    /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17_ff(_ALET_xBxDxFxH_t0, _LYRY_BFETCH_DONEp_t2);
    /* p24.PORY*/ tile_fetcher.PORY_FETCH_DONEp.dff17_rst(_NAFY_RENDERING_AND_NOT_WIN_TRIG_t2);
    /* p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONEp.dff17_rst(_NAFY_RENDERING_AND_NOT_WIN_TRIG_t2);

    /* p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp.dff17_ff(_MYVO_AxCxExGx_t0, _LYRY_BFETCH_DONEp_t2);
    /* p27.LOVY*/ tile_fetcher.LOVY_FETCH_DONEp.dff17_rst(_NYXU_FETCH_TRIGn_t2);
  }

  //------------------------------------------------------------------------------
  // VRAM data in


  {
    // VBD -> tile pix temp
    /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n     = not1(tile_fetcher.LAXU_BFETCH_S0p.qp17_new());
    /*#p27.MYSO*/ wire _MYSO_BG_TRIGp       = nor3(_LOBY_RENDERINGn_t2, _LAXE_BFETCH_S0n, tile_fetcher.LYZU_BFETCH_S0p_D1.qp17_new()); // MYSO fires on fetch phase 2, 6, 10

    /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n_t0 = not1(tile_fetcher.NYVA_BFETCH_S2p.qp17_new());
    /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn = nand3(_MYSO_BG_TRIGp, tile_fetcher.MESU_BFETCH_S1p.qp17_new(), _NOFU_BFETCH_S2n_t0); // NYDY on fetch phase 6
    /*#p32.METE*/ wire _METE_LATCH_TILE_DAp = not1(_NYDY_LATCH_TILE_DAn);
    /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn = not1(_METE_LATCH_TILE_DAp);

    /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n_t0 = not1(tile_fetcher.MESU_BFETCH_S1p.qp17_new());
    /* p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp = and2(_MYSO_BG_TRIGp, _NAKO_BFETCH_S1n_t0); // MOFU fires on fetch phase 2 and 10
    /* p32.LESO*/ wire _LESO_LATCH_TILE_DBn = not1(_MOFU_LATCH_TILE_DBp);
    /* p??.LUVE*/ wire _LUVE_LATCH_TILE_DBp = not1(_LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /* p32.LABU*/ wire _LABU_LATCH_TILE_DBn = not1(_LUVE_LATCH_TILE_DBp);

    /* p32.LEGU*/ vram_bus.LEGU_TILE_DA0n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[0].to_wire_new());
    /* p32.NUDU*/ vram_bus.NUDU_TILE_DA1n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[1].to_wire_new());
    /* p32.MUKU*/ vram_bus.MUKU_TILE_DA2n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[2].to_wire_new());
    /* p32.LUZO*/ vram_bus.LUZO_TILE_DA3n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[3].to_wire_new());
    /* p32.MEGU*/ vram_bus.MEGU_TILE_DA4n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[4].to_wire_new());
    /* p32.MYJY*/ vram_bus.MYJY_TILE_DA5n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[5].to_wire_new());
    /* p32.NASA*/ vram_bus.NASA_TILE_DA6n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[6].to_wire_new());
    /* p32.NEFO*/ vram_bus.NEFO_TILE_DA7n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_Dp_in[7].to_wire_new());

    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /* p32.RAWU*/ vram_bus.RAWU_TILE_DB0p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[0].to_wire_new());
    /* p32.POZO*/ vram_bus.POZO_TILE_DB1p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[1].to_wire_new());
    /* p32.PYZO*/ vram_bus.PYZO_TILE_DB2p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[2].to_wire_new());
    /* p32.POXA*/ vram_bus.POXA_TILE_DB3p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[3].to_wire_new());
    /* p32.PULO*/ vram_bus.PULO_TILE_DB4p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[4].to_wire_new());
    /* p32.POJU*/ vram_bus.POJU_TILE_DB5p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[5].to_wire_new());
    /* p32.POWY*/ vram_bus.POWY_TILE_DB6p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[6].to_wire_new());
    /* p32.PYJU*/ vram_bus.PYJU_TILE_DB7p.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, BUS_VRAM_Dp_in[7].to_wire_new());
  }

  {
    // VRAM tri -> sprite pix temp + x flip

    /* p29.TYTU*/ wire _TYTU_SFETCH_S0n_t0 = not1(sprite_fetcher.TOXE_SFETCH_S0p.qp17_new());
    /* p29.SYCU*/ wire _SYCU_SFETCH_S0pe = nor3(_TYTU_SFETCH_S0n_t0, _LOBY_RENDERINGn_t2, sprite_fetcher.TYFO_SFETCH_S0p_D1.qp17_new());

    /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY_t0 = nor2(sprite_fetcher.TULY_SFETCH_S1p.qp17_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp17_new());
    /* p29.TEPA*/ wire _TEPA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
    /* p29.TYSO*/ wire   _TYSO_SPRITE_READn_t2 = or2(_SAKY_SFETCH_MYSTERY_t0, _TEPA_RENDERINGn_t2); // def or
    /* p29.TEXY*/ wire _TEXY_SPR_READ_VRAMp_t2 = not1(_TYSO_SPRITE_READn_t2);
    /*#p29.XONO*/ wire _XONO_FLIP_X_t2 = and2(oam_bus.BAXO_OAM_DB5p.qp08_new(), _TEXY_SPR_READ_VRAMp_t2);
    /* p33.PUTE*/ wire _PUTE_FLIP0p_t2 = mux2p(_XONO_FLIP_X_t2, BUS_VRAM_Dp_in[7].to_wire_new(), BUS_VRAM_Dp_in[0].to_wire_new());
    /* p33.PELO*/ wire _PELO_FLIP1p_t2 = mux2p(_XONO_FLIP_X_t2, BUS_VRAM_Dp_in[6].to_wire_new(), BUS_VRAM_Dp_in[1].to_wire_new());
    /* p33.PONO*/ wire _PONO_FLIP2p_t2 = mux2p(_XONO_FLIP_X_t2, BUS_VRAM_Dp_in[5].to_wire_new(), BUS_VRAM_Dp_in[2].to_wire_new());
    /* p33.POBE*/ wire _POBE_FLIP3p_t2 = mux2p(_XONO_FLIP_X_t2, BUS_VRAM_Dp_in[4].to_wire_new(), BUS_VRAM_Dp_in[3].to_wire_new());
    /* p33.PACY*/ wire _PACY_FLIP4p_t2 = mux2p(_XONO_FLIP_X_t2, BUS_VRAM_Dp_in[3].to_wire_new(), BUS_VRAM_Dp_in[4].to_wire_new());
    /* p33.PUGU*/ wire _PUGU_FLIP5p_t2 = mux2p(_XONO_FLIP_X_t2, BUS_VRAM_Dp_in[2].to_wire_new(), BUS_VRAM_Dp_in[5].to_wire_new());
    /* p33.PAWE*/ wire _PAWE_FLIP6p_t2 = mux2p(_XONO_FLIP_X_t2, BUS_VRAM_Dp_in[1].to_wire_new(), BUS_VRAM_Dp_in[6].to_wire_new());
    /* p33.PULY*/ wire _PULY_FLIP7p_t2 = mux2p(_XONO_FLIP_X_t2, BUS_VRAM_Dp_in[0].to_wire_new(), BUS_VRAM_Dp_in[7].to_wire_new());

    /*#p29.RACA*/ wire _RACA_CLKp = and2(sprite_fetcher.VONU_SFETCH_S1p_D4.qp17_new(), _SYCU_SFETCH_S0pe);
    /*#p29.PEBY*/ wire _PEBY_CLKn = not1(_RACA_CLKp);
    /*#p29.NYBE*/ wire _NYBE_CLKp = not1(_PEBY_CLKn);
    /*#p29.PUCO*/ wire _PUCO_CLKn = not1(_NYBE_CLKp);

    /*#p29.TOPU*/ wire _TOPU_CLKp = and2(sprite_fetcher.TULY_SFETCH_S1p.qp17_new(), _SYCU_SFETCH_S0pe);
    /*#p29.VYWA*/ wire _VYWA_CLKn = not1(_TOPU_CLKp);
    /*#p29.WENY*/ wire _WENY_CLKp = not1(_VYWA_CLKn);
    /*#p29.XADO*/ wire _XADO_CLKn = not1(_WENY_CLKp);


    /* p33.PEFO*/ vram_bus.PEFO_SPRITE_DB0n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PUTE_FLIP0p_t2);
    /* p33.ROKA*/ vram_bus.ROKA_SPRITE_DB1n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PELO_FLIP1p_t2);
    /* p33.MYTU*/ vram_bus.MYTU_SPRITE_DB2n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PONO_FLIP2p_t2);
    /* p33.RAMU*/ vram_bus.RAMU_SPRITE_DB3n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _POBE_FLIP3p_t2);
    /* p33.SELE*/ vram_bus.SELE_SPRITE_DB4n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PACY_FLIP4p_t2);
    /* p33.SUTO*/ vram_bus.SUTO_SPRITE_DB5n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PUGU_FLIP5p_t2);
    /* p33.RAMA*/ vram_bus.RAMA_SPRITE_DB6n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PAWE_FLIP6p_t2);
    /* p33.RYDU*/ vram_bus.RYDU_SPRITE_DB7n.dff8n(/*vram_bus.latch_sprite_a_delay.q6()*/ _PUCO_CLKn, _PULY_FLIP7p_t2);

    /* p33.REWO*/ vram_bus.REWO_SPRITE_DA0n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PUTE_FLIP0p_t2);
    /* p33.PEBA*/ vram_bus.PEBA_SPRITE_DA1n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PELO_FLIP1p_t2);
    /* p33.MOFO*/ vram_bus.MOFO_SPRITE_DA2n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PONO_FLIP2p_t2);
    /* p33.PUDU*/ vram_bus.PUDU_SPRITE_DA3n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _POBE_FLIP3p_t2);
    /* p33.SAJA*/ vram_bus.SAJA_SPRITE_DA4n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PACY_FLIP4p_t2);
    /* p33.SUNY*/ vram_bus.SUNY_SPRITE_DA5n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PUGU_FLIP5p_t2);
    /* p33.SEMO*/ vram_bus.SEMO_SPRITE_DA6n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PAWE_FLIP6p_t2);
    /* p33.SEGA*/ vram_bus.SEGA_SPRITE_DA7n.dff8n(/*vram_bus.latch_sprite_b_delay.q6()*/ _XADO_CLKn, _PULY_FLIP7p_t2);
  }

  //------------------------------------------------------------------------------


#pragma region LCD_Regs
  {
    /*#p21.MUDE*/ wire _MUDE_X_RSTn_t2 = nor2(lcd_reg.RUTU_LINE_P910p.qp17_new(), _LYHA_VID_RSTp_t0);
    /*#p21.SAXO*/ lcd_reg.SAXO_LX0p.dff17(_TALU_xxCDEFxx_t1,            _MUDE_X_RSTn_t2, lcd_reg.SAXO_LX0p.qn16_old());
    /*#p21.TYPO*/ lcd_reg.TYPO_LX1p.dff17(lcd_reg.SAXO_LX0p.qn16_new(), _MUDE_X_RSTn_t2, lcd_reg.TYPO_LX1p.qn16_old());
    /*#p21.VYZO*/ lcd_reg.VYZO_LX2p.dff17(lcd_reg.TYPO_LX1p.qn16_new(), _MUDE_X_RSTn_t2, lcd_reg.VYZO_LX2p.qn16_old());
    /*#p21.TELU*/ lcd_reg.TELU_LX3p.dff17(lcd_reg.VYZO_LX2p.qn16_new(), _MUDE_X_RSTn_t2, lcd_reg.TELU_LX3p.qn16_old());
    /*#p21.SUDE*/ lcd_reg.SUDE_LX4p.dff17(lcd_reg.TELU_LX3p.qn16_new(), _MUDE_X_RSTn_t2, lcd_reg.SUDE_LX4p.qn16_old());
    /*#p21.TAHA*/ lcd_reg.TAHA_LX5p.dff17(lcd_reg.SUDE_LX4p.qn16_new(), _MUDE_X_RSTn_t2, lcd_reg.TAHA_LX5p.qn16_old());
    /*#p21.TYRY*/ lcd_reg.TYRY_LX6p.dff17(lcd_reg.TAHA_LX5p.qn16_new(), _MUDE_X_RSTn_t2, lcd_reg.TYRY_LX6p.qn16_old());
  }

  {
    /*#p21.LAMA*/ wire _LAMA_FRAME_RSTn_t3 = nor2(lcd_reg.MYTA_y153p.qp17_new(), _LYHA_VID_RSTp_t0);
    /*#p21.MUWY*/ lcd_reg.MUWY_LY0p.dff17(lcd_reg.RUTU_LINE_P910p.qp17_new(), _LAMA_FRAME_RSTn_t3, lcd_reg.MUWY_LY0p.qn16_old());
    /*#p21.MYRO*/ lcd_reg.MYRO_LY1p.dff17(lcd_reg.MUWY_LY0p.qn16_new(),       _LAMA_FRAME_RSTn_t3, lcd_reg.MYRO_LY1p.qn16_old());
    /*#p21.LEXA*/ lcd_reg.LEXA_LY2p.dff17(lcd_reg.MYRO_LY1p.qn16_new(),       _LAMA_FRAME_RSTn_t3, lcd_reg.LEXA_LY2p.qn16_old());
    /*#p21.LYDO*/ lcd_reg.LYDO_LY3p.dff17(lcd_reg.LEXA_LY2p.qn16_new(),       _LAMA_FRAME_RSTn_t3, lcd_reg.LYDO_LY3p.qn16_old());
    /*#p21.LOVU*/ lcd_reg.LOVU_LY4p.dff17(lcd_reg.LYDO_LY3p.qn16_new(),       _LAMA_FRAME_RSTn_t3, lcd_reg.LOVU_LY4p.qn16_old());
    /*#p21.LEMA*/ lcd_reg.LEMA_LY5p.dff17(lcd_reg.LOVU_LY4p.qn16_new(),       _LAMA_FRAME_RSTn_t3, lcd_reg.LEMA_LY5p.qn16_old());
    /*#p21.MATO*/ lcd_reg.MATO_LY6p.dff17(lcd_reg.LEMA_LY5p.qn16_new(),       _LAMA_FRAME_RSTn_t3, lcd_reg.MATO_LY6p.qn16_old());
    /*#p21.LAFO*/ lcd_reg.LAFO_LY7p.dff17(lcd_reg.MATO_LY6p.qn16_new(),       _LAMA_FRAME_RSTn_t3, lcd_reg.LAFO_LY7p.qn16_old());
  }

  {
    /*#p24.LOFU*/ wire _LOFU_LINE_ENDn_t2 = not1(lcd_reg.RUTU_LINE_P910p.qp17_new());
    /*#p24.LUCA*/ lcd_reg.LUCA_LINE_EVENp .dff17(_LOFU_LINE_ENDn_t2, _LYFE_LCD_RSTn_t0, lcd_reg.LUCA_LINE_EVENp.qn16_old());
    /*#p21.NAPO*/ lcd_reg.NAPO_FRAME_EVENp.dff17(_POPU_VBLANKp_t3,   _LYFE_LCD_RSTn_t0, lcd_reg.NAPO_FRAME_EVENp.qn16_old());
  }

  {
    /*#p21.SAXO*/ wire _SAXO_LX0p_t0 = lcd_reg.SAXO_LX0p.qp17_new();
    /*#p21.TYPO*/ wire _TYPO_LX1p_t0 = lcd_reg.TYPO_LX1p.qp17_new();
    /*#p21.VYZO*/ wire _VYZO_LX2p_t0 = lcd_reg.VYZO_LX2p.qp17_new();
    /*#p21.TELU*/ wire _TELU_LX3p_t0 = lcd_reg.TELU_LX3p.qp17_new();
    /*#p21.SUDE*/ wire _SUDE_LX4p_t0 = lcd_reg.SUDE_LX4p.qp17_new();
    /*#p21.TAHA*/ wire _TAHA_LX5p_t0 = lcd_reg.TAHA_LX5p.qp17_new();
    /*#p21.TYRY*/ wire _TYRY_LX6p_t0 = lcd_reg.TYRY_LX6p.qp17_new();

    /*#p21.TOCU*/ wire _TOCU_LX0n_t0 = not1(_SAXO_LX0p_t0);
    /*#p21.VEPE*/ wire _VEPE_LX1n_t0 = not1(_TYPO_LX1p_t0);
    /* p21.VUTY*/ wire _VUTY_LX2n_t0 = not1(_VYZO_LX2p_t0);
    /* p21.VATE*/ wire _VATE_LX3n_t0 = not1(_TELU_LX3p_t0);
    /* p21.TUDA*/ wire _TUDA_LX4n_t0 = not1(_SUDE_LX4p_t0);
    /* p21.TAFY*/ wire _TAFY_LX5n_t0 = not1(_TAHA_LX5p_t0);
    /* p21.TUJU*/ wire _TUJU_LX6n_t0 = not1(_TYRY_LX6p_t0);

    /* p21.VOKU*/ wire _VOKU_LX000n_t0 = nand7(_TUJU_LX6n_t0, _TAFY_LX5n_t0, _TUDA_LX4n_t0, _VATE_LX3n_t0, _VUTY_LX2n_t0, _VEPE_LX1n_t0, _TOCU_LX0n_t0); // 0000000 == 0
    /* p21.TOZU*/ wire _TOZU_LX007n_t0 = nand7(_TUJU_LX6n_t0, _TAFY_LX5n_t0, _TUDA_LX4n_t0, _VATE_LX3n_t0, _VYZO_LX2p_t0, _TYPO_LX1p_t0, _SAXO_LX0p_t0); // 0000111 == 7
    /* p21.TECE*/ wire _TECE_LX045n_t0 = nand7(_TUJU_LX6n_t0, _TAHA_LX5p_t0, _TUDA_LX4n_t0, _TELU_LX3p_t0, _VYZO_LX2p_t0, _VEPE_LX1n_t0, _SAXO_LX0p_t0); // 0101101 == 45
    /*#p21.TEBO*/ wire _TEBO_LX083n_t0 = nand7(_TYRY_LX6p_t0, _TAFY_LX5n_t0, _SUDE_LX4p_t0, _VATE_LX3n_t0, _VUTY_LX2n_t0, _TYPO_LX1p_t0, _SAXO_LX0p_t0); // 1010011 == 83
    /*#p21.TEGY*/ wire _TEGY_STROBE_t0 = nand4(_VOKU_LX000n_t0, _TOZU_LX007n_t0, _TECE_LX045n_t0, _TEBO_LX083n_t0);
    /*#p21.SYGU*/ lcd_reg.SYGU_LINE_STROBE.dff17(_SONO_ABxxxxGH_t1,  _LYFE_LCD_RSTn_t0, _TEGY_STROBE_t0);
  }

  {
    // LYC matcher
    /* p21.RYME*/ wire _RYME_LY_MATCH0n_t0 = xor2(lcd_reg.MUWY_LY0p.qp17_new(), lcd_reg.SYRY_LYC0n.qn08_new());
    /* p21.TYDE*/ wire _TYDE_LY_MATCH1n_t0 = xor2(lcd_reg.MYRO_LY1p.qp17_new(), lcd_reg.VUCE_LYC1n.qn08_new());
    /* p21.REDA*/ wire _REDA_LY_MATCH2n_t0 = xor2(lcd_reg.LEXA_LY2p.qp17_new(), lcd_reg.SEDY_LYC2n.qn08_new());
    /* p21.RASY*/ wire _RASY_LY_MATCH3n_t0 = xor2(lcd_reg.LYDO_LY3p.qp17_new(), lcd_reg.SALO_LYC3n.qn08_new());
    /* p21.TYKU*/ wire _TYKU_LY_MATCH4n_t0 = xor2(lcd_reg.LOVU_LY4p.qp17_new(), lcd_reg.SOTA_LYC4n.qn08_new());
    /* p21.TUCY*/ wire _TUCY_LY_MATCH5n_t0 = xor2(lcd_reg.LEMA_LY5p.qp17_new(), lcd_reg.VAFA_LYC5n.qn08_new());
    /* p21.TERY*/ wire _TERY_LY_MATCH6n_t0 = xor2(lcd_reg.MATO_LY6p.qp17_new(), lcd_reg.VEVO_LYC6n.qn08_new());
    /* p21.SYFU*/ wire _SYFU_LY_MATCH7n_t0 = xor2(lcd_reg.LAFO_LY7p.qp17_new(), lcd_reg.RAHA_LYC7n.qn08_new());

    /* p21.SOVU*/ wire _SOVU_LY_MATCHA_t0 = nor4 (_SYFU_LY_MATCH7n_t0, _TERY_LY_MATCH6n_t0, _TUCY_LY_MATCH5n_t0, _TYKU_LY_MATCH4n_t0); // def nor4
    /* p21.SUBO*/ wire _SUBO_LY_MATCHB_t0 = nor4 (_RASY_LY_MATCH3n_t0, _REDA_LY_MATCH2n_t0, _TYDE_LY_MATCH1n_t0, _RYME_LY_MATCH0n_t0); // def nor4
    /* p21.RAPE*/ wire _RAPE_LY_MATCHn_t0 = nand2(_SOVU_LY_MATCHA_t0, _SUBO_LY_MATCHB_t0); // def nand2
    /* p21.PALY*/ wire _PALY_LY_MATCHa_t0 = not1(_RAPE_LY_MATCHn_t0); // def not

    /*#p21.ROPO*/ lcd_reg.ROPO_LY_MATCH_SYNCp.dff17(_TALU_xxCDEFxx_t1,  _WESY_SYS_RSTn_t0, _PALY_LY_MATCHa_t0);
  }
#pragma endregion


#pragma region Window

  {
    /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp = xnor2(pix_pipe.DATY_SCX0n.qn08_new(), pix_pipe.RYKU_FINE_CNT0.qp17_old());
    /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp = xnor2(pix_pipe.DUZU_SCX1n.qn08_new(), pix_pipe.ROGA_FINE_CNT1.qp17_old());
    /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp = xnor2(pix_pipe.CYXU_SCX2n.qn08_new(), pix_pipe.RUBU_FINE_CNT2.qp17_old());

    /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn = nand4(pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.qp04_new(), _SUHA_SCX_FINE_MATCHp, _SYBY_SCX_FINE_MATCHp, _SOZU_SCX_FINE_MATCHp);
    /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp = not1(_RONE_SCX_FINE_MATCHn);

    /*#p24.ROXO*/ wire _ROXO_CLKPIPEp_t2 = not1(_SEGU_CLKPIPEn_t2);

    /*#p27.NYZE*/ pix_pipe.NYZE_SCX_FINE_MATCH_B.dff17_ff(_MOXE_AxCxExGx_t0, pix_pipe.PUXA_SCX_FINE_MATCH_A.qp17_old());
    /*#p27.PUXA*/ pix_pipe.PUXA_SCX_FINE_MATCH_A.dff17_ff(_ROXO_CLKPIPEp_t2, _POHU_SCX_FINE_MATCHp);

    /*#p27.PUXA*/ pix_pipe.PUXA_SCX_FINE_MATCH_A.dff17_rst(pix_pipe.XYMU_RENDERINGn.qn03_new());
    /*#p27.NYZE*/ pix_pipe.NYZE_SCX_FINE_MATCH_B.dff17_rst(pix_pipe.XYMU_RENDERINGn.qn03_new());

    /* p24.PAHO*/ pix_pipe.PAHO_X_8_SYNC.dff17_ff(_ROXO_CLKPIPEp_t2, pix_pipe.XYDO_PX3p.qp17_new());
    /* p24.PAHO*/ pix_pipe.PAHO_X_8_SYNC.dff17_rst(pix_pipe.XYMU_RENDERINGn.qn03_new());
  }

  {
    /*#p27.NAZE*/ wire _NAZE_WY_MATCH0p_t0 = xnor2(pix_pipe.NESO_WY0n.qn08_new(), lcd_reg.MUWY_LY0p.qp17_new());
    /* p27.PEBO*/ wire _PEBO_WY_MATCH1p_t0 = xnor2(pix_pipe.NYRO_WY1n.qn08_new(), lcd_reg.MYRO_LY1p.qp17_new());
    /* p27.POMO*/ wire _POMO_WY_MATCH2p_t0 = xnor2(pix_pipe.NAGA_WY2n.qn08_new(), lcd_reg.LEXA_LY2p.qp17_new());
    /* p27.NEVU*/ wire _NEVU_WY_MATCH3p_t0 = xnor2(pix_pipe.MELA_WY3n.qn08_new(), lcd_reg.LYDO_LY3p.qp17_new());
    /* p27.NOJO*/ wire _NOJO_WY_MATCH4p_t0 = xnor2(pix_pipe.NULO_WY4n.qn08_new(), lcd_reg.LOVU_LY4p.qp17_new());
    /* p27.PAGA*/ wire _PAGA_WY_MATCH5p_t0 = xnor2(pix_pipe.NENE_WY5n.qn08_new(), lcd_reg.LEMA_LY5p.qp17_new());
    /* p27.PEZO*/ wire _PEZO_WY_MATCH6p_t0 = xnor2(pix_pipe.NUKA_WY6n.qn08_new(), lcd_reg.MATO_LY6p.qp17_new());
    /* p27.NUPA*/ wire _NUPA_WY_MATCH7p_t0 = xnor2(pix_pipe.NAFU_WY7n.qn08_new(), lcd_reg.LAFO_LY7p.qp17_new());

    /*#p27.MYLO*/ wire _MYLO_WX_MATCH0p_t0 = xnor2(pix_pipe.XEHO_PX0p.qp17_new(), pix_pipe.MYPA_WX0n.qn08_new());
    /* p27.PUWU*/ wire _PUWU_WX_MATCH1p_t0 = xnor2(pix_pipe.SAVY_PX1p.qp17_new(), pix_pipe.NOFE_WX1n.qn08_new());
    /* p27.PUHO*/ wire _PUHO_WX_MATCH2p_t0 = xnor2(pix_pipe.XODU_PX2p.qp17_new(), pix_pipe.NOKE_WX2n.qn08_new());
    /* p27.NYTU*/ wire _NYTU_WX_MATCH3p_t0 = xnor2(pix_pipe.XYDO_PX3p.qp17_new(), pix_pipe.MEBY_WX3n.qn08_new());
    /* p27.NEZO*/ wire _NEZO_WX_MATCH4p_t0 = xnor2(pix_pipe.TUHU_PX4p.qp17_new(), pix_pipe.MYPU_WX4n.qn08_new());
    /* p27.NORY*/ wire _NORY_WX_MATCH5p_t0 = xnor2(pix_pipe.TUKY_PX5p.qp17_new(), pix_pipe.MYCE_WX5n.qn08_new());
    /* p27.NONO*/ wire _NONO_WX_MATCH6p_t0 = xnor2(pix_pipe.TAKO_PX6p.qp17_new(), pix_pipe.MUVO_WX6n.qn08_new());
    /* p27.PASE*/ wire _PASE_WX_MATCH7p_t0 = xnor2(pix_pipe.SYBE_PX7p.qp17_new(), pix_pipe.NUKU_WX7n.qn08_new());

    /*#p27.PALO*/ wire _PALO_WY_MATCH_HIn_t0  = nand5(pix_pipe.WYMO_LCDC_WINENn.qn08_new(), _NOJO_WY_MATCH4p_t0, _PAGA_WY_MATCH5p_t0, _PEZO_WY_MATCH6p_t0, _NUPA_WY_MATCH7p_t0);
    /* p27.NELE*/ wire _NELE_WY_MATCH_HIp_t0  = not1(_PALO_WY_MATCH_HIn_t0);
    /* p27.PAFU*/ wire _PAFU_WY_MATCHn_t0     = nand5(_NELE_WY_MATCH_HIp_t0, _NAZE_WY_MATCH0p_t0, _PEBO_WY_MATCH1p_t0, _POMO_WY_MATCH2p_t0, _NEVU_WY_MATCH3p_t0);
    /* p27.ROGE*/ wire _ROGE_WY_MATCHp_t0     = not1(_PAFU_WY_MATCHn_t0);

    /* p27.REJO*/ wire _REJO_WY_MATCHp_t0     = pix_pipe.REJO_WY_MATCHp.qp04_old();
    /* p27.PUKY*/ wire _PUKY_WX_MATCH_HIn_t0  = nand5(_REJO_WY_MATCHp_t0, _NEZO_WX_MATCH4p_t0, _NORY_WX_MATCH5p_t0, _NONO_WX_MATCH6p_t0, _PASE_WX_MATCH7p_t0);
    /* p27.NUFA*/ wire _NUFA_WX_MATCH_HIp_t0  = not1(_PUKY_WX_MATCH_HIn_t0);
    /* p27.NOGY*/ wire _NOGY_WX_MATCHn_t0     = nand5(_NUFA_WX_MATCH_HIp_t0, _MYLO_WX_MATCH0p_t0, _PUWU_WX_MATCH1p_t0, _PUHO_WX_MATCH2p_t0, _NYTU_WX_MATCH3p_t0);
    /* p27.NUKO*/ wire _NUKO_WX_MATCHp_t0     = not1(_NOGY_WX_MATCHn_t0);

    /* p27.REPU*/ wire _REPU_VBLANK_RSTp_t3 = or2(_PARU_VBLANKp_t3, _PYRY_VID_RSTp_t0);
    /* p27.REJO*/ pix_pipe.REJO_WY_MATCHp.nor_latch(pix_pipe.SARY_WY_MATCHp.qp17_old(), _REPU_VBLANK_RSTp_t3);
    /* p27.SARY*/ pix_pipe.SARY_WY_MATCHp.dff17(_TALU_xxCDEFxx_t1, _XAPO_VID_RSTn_t0, _ROGE_WY_MATCHp_t0);

    /* p27.ROCO*/ wire _ROCO_CLKPIPEp_t2 = not1(_SEGU_CLKPIPEn_t2);
    /* p27.NUNU*/ pix_pipe.NUNU_WX_MATCH_Bp.dff17(_MEHE_AxCxExGx_t0, _XAPO_VID_RSTn_t0, pix_pipe.PYCO_WX_MATCH_Ap.qp17_old());
    /* p27.PYCO*/ pix_pipe.PYCO_WX_MATCH_Ap.dff17(_ROCO_CLKPIPEp_t2, _XAPO_VID_RSTn_t0, _NUKO_WX_MATCHp_t0);

    /* p27.RENE*/ pix_pipe.RENE_FETCHn_B.dff17(_ALET_xBxDxFxH_t0, pix_pipe.XYMU_RENDERINGn.qn03_new(), pix_pipe.RYFA_FETCHn_A.qp17_old());

    // If we use the "new" values of FINE_CNT here, things break because the signal is then "newer" than the clock to RYFA
    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n_t0 = nand3(pix_pipe.RUBU_FINE_CNT2.qp17_old(), pix_pipe.ROGA_FINE_CNT1.qp17_old(), pix_pipe.RYKU_FINE_CNT0.qp17_old());
    /* p27.PANY*/ wire _PANY_FETCHn_t0 = nor2(_NUKO_WX_MATCHp_t0, _ROZE_FINE_COUNT_7n_t0);
    /* p27.RYFA*/ pix_pipe.RYFA_FETCHn_A.dff17(_SEGU_CLKPIPEn_t2, pix_pipe.XYMU_RENDERINGn.qn03_new(), _PANY_FETCHn_t0);

    /* p27.NOPA*/ pix_pipe.NOPA_WIN_MODE_Bp.dff17     (_ALET_xBxDxFxH_t0, _XAPO_VID_RSTn_t0, pix_pipe.PYNU_WIN_MODE_Ap.qp04_new());
    /* p27.SOVY*/ pix_pipe.SOVY_WIN_FIRST_TILE_B.dff17(_ALET_xBxDxFxH_t0, _XAPO_VID_RSTn_t0, pix_pipe.RYDY_WIN_HITp.to_wire_new());
  }

  {
    /* p27.SEKO*/ wire _SEKO_FETCH_TRIGp_t0 = nor2(pix_pipe.RYFA_FETCHn_A.qn16_new(), pix_pipe.RENE_FETCHn_B.qp17_new());
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_t0 = not1(pix_pipe.RYDY_WIN_HITp.to_wire_new());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_t0 = nand2(_SYLO_WIN_HITn_t0, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17_new());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_t0 = not1(_TUXY_WIN_FIRST_TILEne_t0);
    /* p27.TEVO*/ wire _TEVO_FETCH_TRIGp_t2 = or3(_SEKO_FETCH_TRIGp_t0, _SUZU_WIN_FIRST_TILEne_t0, _TAVE_PRELOAD_DONE_TRIGp_t2); // Schematic wrong, this is OR
    /*#p27.NOCU*/ wire _NOCU_WIN_MODEn_t0 = not1(pix_pipe.PYNU_WIN_MODE_Ap.qp04_new());
    /* p27.PORE*/ wire _PORE_WIN_MODEp_t0 = not1(_NOCU_WIN_MODEn_t0);
    /* p27.VETU*/ wire _VETU_WIN_MAP_CLK = and2(_TEVO_FETCH_TRIGp_t2, _PORE_WIN_MODEp_t0);
    /*#p27.XOFO*/ wire _XOFO_WIN_RSTp_t2 = nand3(pix_pipe.WYMO_LCDC_WINENn.qn08_new(), _XAHY_LINE_TRIGn_t2, _XAPO_VID_RSTn_t0);
    /* p27.XACO*/ wire _XACO_WIN_RSTn_t2 = not1(_XOFO_WIN_RSTp_t2);
    /* p27.WYKA*/ pix_pipe.WYKA_WIN_X3.dff17(_VETU_WIN_MAP_CLK,               _XACO_WIN_RSTn_t2, pix_pipe.WYKA_WIN_X3.qn16_old());
    /* p27.WODY*/ pix_pipe.WODY_WIN_X4.dff17(pix_pipe.WYKA_WIN_X3.qn16_new(), _XACO_WIN_RSTn_t2, pix_pipe.WODY_WIN_X4.qn16_old());
    /* p27.WOBO*/ pix_pipe.WOBO_WIN_X5.dff17(pix_pipe.WODY_WIN_X4.qn16_new(), _XACO_WIN_RSTn_t2, pix_pipe.WOBO_WIN_X5.qn16_old());
    /* p27.WYKO*/ pix_pipe.WYKO_WIN_X6.dff17(pix_pipe.WOBO_WIN_X5.qn16_new(), _XACO_WIN_RSTn_t2, pix_pipe.WYKO_WIN_X6.qn16_old());
    /* p27.XOLO*/ pix_pipe.XOLO_WIN_X7.dff17(pix_pipe.WYKO_WIN_X6.qn16_new(), _XACO_WIN_RSTn_t2, pix_pipe.XOLO_WIN_X7.qn16_old());
  }

  {
    // Every time we leave win mode we increment win_y

    /*#p27.NOCU*/ wire _NOCU_WIN_MODEn_t0 = not1(pix_pipe.PYNU_WIN_MODE_Ap.qp04_new());
    /* p27.PORE*/ wire _PORE_WIN_MODEp_t0 = not1(_NOCU_WIN_MODEn_t0);
    /* p27.REPU*/ wire _REPU_VBLANK_RSTp_t3 = or2(_PARU_VBLANKp_t3, _PYRY_VID_RSTp_t0);
    /* p27.WAZY*/ wire _WAZY_WIN_MODEn_t0 = not1(_PORE_WIN_MODEp_t0);
    /* p27.SYNY*/ wire _SYNY_VBLANK_RSTn_t3 = not1(_REPU_VBLANK_RSTp_t3);
    /* p27.VYNO*/ pix_pipe.VYNO_WIN_Y0.dff17(_WAZY_WIN_MODEn_t0,              _SYNY_VBLANK_RSTn_t3, pix_pipe.VYNO_WIN_Y0.qn16_old());
    /* p27.VUJO*/ pix_pipe.VUJO_WIN_Y1.dff17(pix_pipe.VYNO_WIN_Y0.qn16_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.VUJO_WIN_Y1.qn16_old());
    /* p27.VYMU*/ pix_pipe.VYMU_WIN_Y2.dff17(pix_pipe.VUJO_WIN_Y1.qn16_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.VYMU_WIN_Y2.qn16_old());
    /* p27.TUFU*/ pix_pipe.TUFU_WIN_Y3.dff17(pix_pipe.VYMU_WIN_Y2.qn16_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TUFU_WIN_Y3.qn16_old());
    /* p27.TAXA*/ pix_pipe.TAXA_WIN_Y4.dff17(pix_pipe.TUFU_WIN_Y3.qn16_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TAXA_WIN_Y4.qn16_old());
    /* p27.TOZO*/ pix_pipe.TOZO_WIN_Y5.dff17(pix_pipe.TAXA_WIN_Y4.qn16_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TOZO_WIN_Y5.qn16_old());
    /* p27.TATE*/ pix_pipe.TATE_WIN_Y6.dff17(pix_pipe.TOZO_WIN_Y5.qn16_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TATE_WIN_Y6.qn16_old());
    /* p27.TEKE*/ pix_pipe.TEKE_WIN_Y7.dff17(pix_pipe.TATE_WIN_Y6.qn16_new(), _SYNY_VBLANK_RSTn_t3, pix_pipe.TEKE_WIN_Y7.qn16_old());
  }

  {
    /*#p24.ROXO*/ wire _ROXO_CLKPIPEp_t2 = not1(_SEGU_CLKPIPEn_t2);
    /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n_t0 = nand3(pix_pipe.RUBU_FINE_CNT2.qp17_old(), pix_pipe.ROGA_FINE_CNT1.qp17_old(), pix_pipe.RYKU_FINE_CNT0.qp17_old());
    /*#p27.PECU*/ wire _PECU_FINE_CLK_t2 = nand2(_ROXO_CLKPIPEp_t2, _ROZE_FINE_COUNT_7n_t0);
    /*#p27.PAHA*/ wire _PAHA_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
    /* p27.SEKO*/ wire _SEKO_FETCH_TRIGp_t0 = nor2(pix_pipe.RYFA_FETCHn_A.qn16_new(), pix_pipe.RENE_FETCHn_B.qp17_new());
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_t0 = not1(pix_pipe.RYDY_WIN_HITp.to_wire_new());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_t0 = nand2(_SYLO_WIN_HITn_t0, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17_new());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_t0 = not1(_TUXY_WIN_FIRST_TILEne_t0);
    /* p27.TEVO*/ wire _TEVO_FETCH_TRIGp_t2 = or3(_SEKO_FETCH_TRIGp_t0, _SUZU_WIN_FIRST_TILEne_t0, _TAVE_PRELOAD_DONE_TRIGp_t2); // Schematic wrong, this is OR
    /*#p27.PASO*/ wire _PASO_FINE_RST_t2 = nor2(_PAHA_RENDERINGn_t2, _TEVO_FETCH_TRIGp_t2);
    /*#p27.RYKU*/ pix_pipe.RYKU_FINE_CNT0.dff17(_PECU_FINE_CLK_t2,                  _PASO_FINE_RST_t2, pix_pipe.RYKU_FINE_CNT0.qn16_old());
    /*#p27.ROGA*/ pix_pipe.ROGA_FINE_CNT1.dff17(pix_pipe.RYKU_FINE_CNT0.qn16_new(), _PASO_FINE_RST_t2, pix_pipe.ROGA_FINE_CNT1.qn16_old());
    /*#p27.RUBU*/ pix_pipe.RUBU_FINE_CNT2.dff17(pix_pipe.ROGA_FINE_CNT1.qn16_new(), _PASO_FINE_RST_t2, pix_pipe.RUBU_FINE_CNT2.qn16_old());

  }
#pragma endregion




#pragma region Pixel_Pipes
  // Shift all the pipes.
  {
    /* p32.PYBO*/ pix_pipe.PYBO_BG_PIPE_A7.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.NEDA_BG_PIPE_A6.qp16_old());
    /* p32.NEDA*/ pix_pipe.NEDA_BG_PIPE_A6.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.MODU_BG_PIPE_A5.qp16_old());
    /* p32.MODU*/ pix_pipe.MODU_BG_PIPE_A5.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.NEPO_BG_PIPE_A4.qp16_old());
    /* p32.NEPO*/ pix_pipe.NEPO_BG_PIPE_A4.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.MACU_BG_PIPE_A3.qp16_old());
    /* p32.MACU*/ pix_pipe.MACU_BG_PIPE_A3.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.MOJU_BG_PIPE_A2.qp16_old());
    /* p32.MOJU*/ pix_pipe.MOJU_BG_PIPE_A2.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.NOZO_BG_PIPE_A1.qp16_old());
    /* p32.NOZO*/ pix_pipe.NOZO_BG_PIPE_A1.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.MYDE_BG_PIPE_A0.qp16_old());
    /* p32.MYDE*/ pix_pipe.MYDE_BG_PIPE_A0.dff22_ff(_SACU_CLKPIPEp_t2, GND);

    /* p32.SOHU*/ pix_pipe.SOHU_BG_PIPE_B7.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.RALU_BG_PIPE_B6.qp16_old());
    /* p32.RALU*/ pix_pipe.RALU_BG_PIPE_B6.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.SETU_BG_PIPE_B5.qp16_old());
    /* p32.SETU*/ pix_pipe.SETU_BG_PIPE_B5.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.SOBO_BG_PIPE_B4.qp16_old());
    /* p32.SOBO*/ pix_pipe.SOBO_BG_PIPE_B4.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.RYSA_BG_PIPE_B3.qp16_old());
    /* p32.RYSA*/ pix_pipe.RYSA_BG_PIPE_B3.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.SADY_BG_PIPE_B2.qp16_old());
    /* p32.SADY*/ pix_pipe.SADY_BG_PIPE_B2.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.TACA_BG_PIPE_B1.qp16_old());
    /* p32.TACA*/ pix_pipe.TACA_BG_PIPE_B1.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.TOMY_BG_PIPE_B0.qp16_old());
    /* p32.TOMY*/ pix_pipe.TOMY_BG_PIPE_B0.dff22_ff(_SACU_CLKPIPEp_t2, GND);

    /* p33.WUFY*/ pix_pipe.WUFY_SPR_PIPE_A7.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.VAFO_SPR_PIPE_A6.qp16_old());
    /* p33.VAFO*/ pix_pipe.VAFO_SPR_PIPE_A6.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.WORA_SPR_PIPE_A5.qp16_old());
    /* p33.WORA*/ pix_pipe.WORA_SPR_PIPE_A5.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.WYHO_SPR_PIPE_A4.qp16_old());
    /* p33.WYHO*/ pix_pipe.WYHO_SPR_PIPE_A4.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.LESU_SPR_PIPE_A3.qp16_old());
    /* p33.LESU*/ pix_pipe.LESU_SPR_PIPE_A3.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.LEFE_SPR_PIPE_A2.qp16_old());
    /* p33.LEFE*/ pix_pipe.LEFE_SPR_PIPE_A2.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.MASO_SPR_PIPE_A1.qp16_old());
    /* p33.MASO*/ pix_pipe.MASO_SPR_PIPE_A1.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.NURO_SPR_PIPE_A0.qp16_old());
    /* p33.NURO*/ pix_pipe.NURO_SPR_PIPE_A0.dff22_ff(_SACU_CLKPIPEp_t2, GND);

    /* p33.VUPY*/ pix_pipe.VUPY_SPR_PIPE_B7.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.VANU_SPR_PIPE_B6.qp16_old());
    /* p33.VANU*/ pix_pipe.VANU_SPR_PIPE_B6.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.WEBA_SPR_PIPE_B5.qp16_old());
    /* p33.WEBA*/ pix_pipe.WEBA_SPR_PIPE_B5.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.VARE_SPR_PIPE_B4.qp16_old());
    /* p33.VARE*/ pix_pipe.VARE_SPR_PIPE_B4.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.PYJO_SPR_PIPE_B3.qp16_old());
    /* p33.PYJO*/ pix_pipe.PYJO_SPR_PIPE_B3.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.NATY_SPR_PIPE_B2.qp16_old());
    /* p33.NATY*/ pix_pipe.NATY_SPR_PIPE_B2.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.PEFU_SPR_PIPE_B1.qp16_old());
    /* p33.PEFU*/ pix_pipe.PEFU_SPR_PIPE_B1.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.NYLU_SPR_PIPE_B0.qp16_old());
    /* p33.NYLU*/ pix_pipe.NYLU_SPR_PIPE_B0.dff22_ff(_SACU_CLKPIPEp_t2, GND);

    /* p34.LYME*/ pix_pipe.LYME_PAL_PIPE_7.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.MODA_PAL_PIPE_6.qp16_old());
    /* p34.MODA*/ pix_pipe.MODA_PAL_PIPE_6.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.NUKE_PAL_PIPE_5.qp16_old());
    /* p34.NUKE*/ pix_pipe.NUKE_PAL_PIPE_5.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.PALU_PAL_PIPE_4.qp16_old());
    /* p34.PALU*/ pix_pipe.PALU_PAL_PIPE_4.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.SOMY_PAL_PIPE_3.qp16_old());
    /* p34.SOMY*/ pix_pipe.SOMY_PAL_PIPE_3.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.ROSA_PAL_PIPE_2.qp16_old());
    /* p34.ROSA*/ pix_pipe.ROSA_PAL_PIPE_2.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.SATA_PAL_PIPE_1.qp16_old());
    /* p34.SATA*/ pix_pipe.SATA_PAL_PIPE_1.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.RUGO_PAL_PIPE_0.qp16_old());
    /* p34.RUGO*/ pix_pipe.RUGO_PAL_PIPE_0.dff22_ff(_SACU_CLKPIPEp_t2, GND);

    /* p26.VAVA*/ pix_pipe.VAVA_MASK_PIPE_7.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.VUMO_MASK_PIPE_6.qp16_old());
    /* p26.VUMO*/ pix_pipe.VUMO_MASK_PIPE_6.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.WODA_MASK_PIPE_5.qp16_old());
    /* p26.WODA*/ pix_pipe.WODA_MASK_PIPE_5.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.XETE_MASK_PIPE_4.qp16_old());
    /* p26.XETE*/ pix_pipe.XETE_MASK_PIPE_4.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.WYFU_MASK_PIPE_3.qp16_old());
    /* p26.WYFU*/ pix_pipe.WYFU_MASK_PIPE_3.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.VOSA_MASK_PIPE_2.qp16_old());
    /* p26.VOSA*/ pix_pipe.VOSA_MASK_PIPE_2.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.WURU_MASK_PIPE_1.qp16_old());
    /* p26.WURU*/ pix_pipe.WURU_MASK_PIPE_1.dff22_ff(_SACU_CLKPIPEp_t2, pix_pipe.VEZO_MASK_PIPE_0.qp16_old());
    /* p26.VEZO*/ pix_pipe.VEZO_MASK_PIPE_0.dff22_ff(_SACU_CLKPIPEp_t2, _VYPO_VCC);
  }

  // Reload all the pipes using async set/rst
  {
    /* p32.LUHE*/ wire BG_PIX_A0n = not1(vram_bus.LEGU_TILE_DA0n.qn07_new());
    /* p32.NOLY*/ wire BG_PIX_A1n = not1(vram_bus.NUDU_TILE_DA1n.qn07_new());
    /* p32.LEKE*/ wire BG_PIX_A2n = not1(vram_bus.MUKU_TILE_DA2n.qn07_new());
    /* p32.LOMY*/ wire BG_PIX_A3n = not1(vram_bus.LUZO_TILE_DA3n.qn07_new());
    /* p32.LALA*/ wire BG_PIX_A4n = not1(vram_bus.MEGU_TILE_DA4n.qn07_new());
    /* p32.LOXA*/ wire BG_PIX_A5n = not1(vram_bus.MYJY_TILE_DA5n.qn07_new());
    /* p32.NEZE*/ wire BG_PIX_A6n = not1(vram_bus.NASA_TILE_DA6n.qn07_new());
    /* p32.NOBO*/ wire BG_PIX_A7n = not1(vram_bus.NEFO_TILE_DA7n.qn07_new());

    /* p27.SEKO*/ wire _SEKO_FETCH_TRIGp_t0 = nor2(pix_pipe.RYFA_FETCHn_A.qn16_new(), pix_pipe.RENE_FETCHn_B.qp17_new());
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_t0 = not1(pix_pipe.RYDY_WIN_HITp.to_wire_new());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_t0 = nand2(_SYLO_WIN_HITn_t0, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17_new());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_t0 = not1(_TUXY_WIN_FIRST_TILEne_t0);
    /* p27.TEVO*/ wire _TEVO_FETCH_TRIGp_t2 = or3(_SEKO_FETCH_TRIGp_t0, _SUZU_WIN_FIRST_TILEne_t0, _TAVE_PRELOAD_DONE_TRIGp_t2); // Schematic wrong, this is OR
    /* p27.NYXU*/ wire _NYXU_FETCH_TRIGn_t2 = nor3(_AVAP_SCAN_DONE_TRIGp_t2, _MOSU_WIN_FETCH_TRIGp_t0, _TEVO_FETCH_TRIGp_t2);
    /* p32.LOZE*/ wire _LOZE_PIPE_A_LOADp = not1(_NYXU_FETCH_TRIGn_t2);
    /* p32.LAKY*/ wire BG_PIPE_A_SET0 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.LEGU_TILE_DA0n.qn07_new());
    /* p32.NYXO*/ wire BG_PIPE_A_SET1 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.NUDU_TILE_DA1n.qn07_new());
    /* p32.LOTO*/ wire BG_PIPE_A_SET2 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.MUKU_TILE_DA2n.qn07_new());
    /* p32.LYDU*/ wire BG_PIPE_A_SET3 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.LUZO_TILE_DA3n.qn07_new());
    /* p32.MYVY*/ wire BG_PIPE_A_SET4 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.MEGU_TILE_DA4n.qn07_new());
    /* p32.LODO*/ wire BG_PIPE_A_SET5 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.MYJY_TILE_DA5n.qn07_new());
    /* p32.NUTE*/ wire BG_PIPE_A_SET6 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.NASA_TILE_DA6n.qn07_new());
    /* p32.NAJA*/ wire BG_PIPE_A_SET7 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.NEFO_TILE_DA7n.qn07_new());

    /* p32.LOTY*/ wire BG_PIPE_A_RST0 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A0n);
    /* p32.NEXA*/ wire BG_PIPE_A_RST1 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A1n);
    /* p32.LUTU*/ wire BG_PIPE_A_RST2 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A2n);
    /* p32.LUJA*/ wire BG_PIPE_A_RST3 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A3n);
    /* p32.MOSY*/ wire BG_PIPE_A_RST4 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A4n);
    /* p32.LERU*/ wire BG_PIPE_A_RST5 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A5n);
    /* p32.NYHA*/ wire BG_PIPE_A_RST6 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A6n);
    /* p32.NADY*/ wire BG_PIPE_A_RST7 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A7n);

    /* p32.PYBO*/ pix_pipe.PYBO_BG_PIPE_A7.dff22_set_rst(BG_PIPE_A_SET7, BG_PIPE_A_RST7);
    /* p32.NEDA*/ pix_pipe.NEDA_BG_PIPE_A6.dff22_set_rst(BG_PIPE_A_SET6, BG_PIPE_A_RST6);
    /* p32.MODU*/ pix_pipe.MODU_BG_PIPE_A5.dff22_set_rst(BG_PIPE_A_SET5, BG_PIPE_A_RST5);
    /* p32.NEPO*/ pix_pipe.NEPO_BG_PIPE_A4.dff22_set_rst(BG_PIPE_A_SET4, BG_PIPE_A_RST4);
    /* p32.MACU*/ pix_pipe.MACU_BG_PIPE_A3.dff22_set_rst(BG_PIPE_A_SET3, BG_PIPE_A_RST3);
    /* p32.MOJU*/ pix_pipe.MOJU_BG_PIPE_A2.dff22_set_rst(BG_PIPE_A_SET2, BG_PIPE_A_RST2);
    /* p32.NOZO*/ pix_pipe.NOZO_BG_PIPE_A1.dff22_set_rst(BG_PIPE_A_SET1, BG_PIPE_A_RST1);
    /* p32.MYDE*/ pix_pipe.MYDE_BG_PIPE_A0.dff22_set_rst(BG_PIPE_A_SET0, BG_PIPE_A_RST0);
  }

  {
    /* p32.TOSA*/ wire BG_PIX_B0n = not1(vram_bus.RAWU_TILE_DB0p.q11p_new());
    /* p32.RUCO*/ wire BG_PIX_B1n = not1(vram_bus.POZO_TILE_DB1p.q11p_new());
    /* p32.TYCE*/ wire BG_PIX_B2n = not1(vram_bus.PYZO_TILE_DB2p.q11p_new());
    /* p32.REVY*/ wire BG_PIX_B3n = not1(vram_bus.POXA_TILE_DB3p.q11p_new());
    /* p32.RYGA*/ wire BG_PIX_B4n = not1(vram_bus.PULO_TILE_DB4p.q11p_new());
    /* p32.RYLE*/ wire BG_PIX_B5n = not1(vram_bus.POJU_TILE_DB5p.q11p_new());
    /* p32.RAPU*/ wire BG_PIX_B6n = not1(vram_bus.POWY_TILE_DB6p.q11p_new());
    /* p32.SOJA*/ wire BG_PIX_B7n = not1(vram_bus.PYJU_TILE_DB7p.q11p_new());

    /* p27.SEKO*/ wire _SEKO_FETCH_TRIGp_t0 = nor2(pix_pipe.RYFA_FETCHn_A.qn16_new(), pix_pipe.RENE_FETCHn_B.qp17_new());
    /*#p27.SYLO*/ wire _SYLO_WIN_HITn_t0 = not1(pix_pipe.RYDY_WIN_HITp.to_wire_new());
    /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILEne_t0 = nand2(_SYLO_WIN_HITn_t0, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17_new());
    /* p27.SUZU*/ wire _SUZU_WIN_FIRST_TILEne_t0 = not1(_TUXY_WIN_FIRST_TILEne_t0);
    /* p27.TEVO*/ wire _TEVO_FETCH_TRIGp_t2 = or3(_SEKO_FETCH_TRIGp_t0, _SUZU_WIN_FIRST_TILEne_t0, _TAVE_PRELOAD_DONE_TRIGp_t2); // Schematic wrong, this is OR
    /* p27.NYXU*/ wire _NYXU_FETCH_TRIGn_t2 = nor3(_AVAP_SCAN_DONE_TRIGp_t2, _MOSU_WIN_FETCH_TRIGp_t0, _TEVO_FETCH_TRIGp_t2);
    /* p32.LUXA*/ wire LUXA_PIPE_B_LOADp = not1(_NYXU_FETCH_TRIGn_t2);
    /* p32.TUXE*/ wire BG_PIPE_B_SET0 = nand2(LUXA_PIPE_B_LOADp, vram_bus.RAWU_TILE_DB0p.q11p_new());
    /* p32.SOLY*/ wire BG_PIPE_B_SET1 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POZO_TILE_DB1p.q11p_new());
    /* p32.RUCE*/ wire BG_PIPE_B_SET2 = nand2(LUXA_PIPE_B_LOADp, vram_bus.PYZO_TILE_DB2p.q11p_new());
    /* p32.RYJA*/ wire BG_PIPE_B_SET3 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POXA_TILE_DB3p.q11p_new());
    /* p32.RUTO*/ wire BG_PIPE_B_SET4 = nand2(LUXA_PIPE_B_LOADp, vram_bus.PULO_TILE_DB4p.q11p_new());
    /* p32.RAJA*/ wire BG_PIPE_B_SET5 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POJU_TILE_DB5p.q11p_new());
    /* p32.RAJO*/ wire BG_PIPE_B_SET6 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POWY_TILE_DB6p.q11p_new());
    /* p32.RAGA*/ wire BG_PIPE_B_SET7 = nand2(LUXA_PIPE_B_LOADp, vram_bus.PYJU_TILE_DB7p.q11p_new());

    /* p32.SEJA*/ wire BG_PIPE_B_RST0 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B0n);
    /* p32.SENO*/ wire BG_PIPE_B_RST1 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B1n);
    /* p32.SURE*/ wire BG_PIPE_B_RST2 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B2n);
    /* p32.SEBO*/ wire BG_PIPE_B_RST3 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B3n);
    /* p32.SUCA*/ wire BG_PIPE_B_RST4 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B4n);
    /* p32.SYWE*/ wire BG_PIPE_B_RST5 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B5n);
    /* p32.SUPU*/ wire BG_PIPE_B_RST6 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B6n);
    /* p32.RYJY*/ wire BG_PIPE_B_RST7 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B7n);

    /* p32.SOHU*/ pix_pipe.SOHU_BG_PIPE_B7.dff22_set_rst(BG_PIPE_B_SET7, BG_PIPE_B_RST7);
    /* p32.RALU*/ pix_pipe.RALU_BG_PIPE_B6.dff22_set_rst(BG_PIPE_B_SET6, BG_PIPE_B_RST6);
    /* p32.SETU*/ pix_pipe.SETU_BG_PIPE_B5.dff22_set_rst(BG_PIPE_B_SET5, BG_PIPE_B_RST5);
    /* p32.SOBO*/ pix_pipe.SOBO_BG_PIPE_B4.dff22_set_rst(BG_PIPE_B_SET4, BG_PIPE_B_RST4);
    /* p32.RYSA*/ pix_pipe.RYSA_BG_PIPE_B3.dff22_set_rst(BG_PIPE_B_SET3, BG_PIPE_B_RST3);
    /* p32.SADY*/ pix_pipe.SADY_BG_PIPE_B2.dff22_set_rst(BG_PIPE_B_SET2, BG_PIPE_B_RST2);
    /* p32.TACA*/ pix_pipe.TACA_BG_PIPE_B1.dff22_set_rst(BG_PIPE_B_SET1, BG_PIPE_B_RST1);
    /* p32.TOMY*/ pix_pipe.TOMY_BG_PIPE_B0.dff22_set_rst(BG_PIPE_B_SET0, BG_PIPE_B_RST0);
  }

  {
    /* p29.TYNO*/ wire _TYNO_t0 = nand3(sprite_fetcher.TOXE_SFETCH_S0p.qp17_new(), sprite_fetcher.SEBA_SFETCH_S1p_D5.qp17_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp17_new());
    /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn_t0  = or2(sprite_fetcher.TYFO_SFETCH_S0p_D1.qn16_new(), _TYNO_t0);
    /* p29.WUTY*/ wire _WUTY_SPRITE_DONEp_t0 = not1(_VUSA_SPRITE_DONEn_t0);
    /* p29.XEFY*/ wire XEPY_SPRITE_DONEn  = not1(_WUTY_SPRITE_DONEp_t0);
    /* p34.MEFU*/ wire MEFU_SPRITE_MASK0n = or3(XEPY_SPRITE_DONEn, pix_pipe.NYLU_SPR_PIPE_B0.qp16_new(), pix_pipe.NURO_SPR_PIPE_A0.qp16_new()); // def or
    /* p34.MEVE*/ wire MEVE_SPRITE_MASK1n = or3(XEPY_SPRITE_DONEn, pix_pipe.PEFU_SPR_PIPE_B1.qp16_new(), pix_pipe.MASO_SPR_PIPE_A1.qp16_new());
    /* p34.MYZO*/ wire MYZO_SPRITE_MASK2n = or3(XEPY_SPRITE_DONEn, pix_pipe.NATY_SPR_PIPE_B2.qp16_new(), pix_pipe.LEFE_SPR_PIPE_A2.qp16_new());
    /* p34.RUDA*/ wire RUDA_SPRITE_MASK3n = or3(XEPY_SPRITE_DONEn, pix_pipe.PYJO_SPR_PIPE_B3.qp16_new(), pix_pipe.LESU_SPR_PIPE_A3.qp16_new());
    /* p34.VOTO*/ wire VOTO_SPRITE_MASK4n = or3(XEPY_SPRITE_DONEn, pix_pipe.VARE_SPR_PIPE_B4.qp16_new(), pix_pipe.WYHO_SPR_PIPE_A4.qp16_new());
    /* p34.VYSA*/ wire VYSA_SPRITE_MASK5n = or3(XEPY_SPRITE_DONEn, pix_pipe.WEBA_SPR_PIPE_B5.qp16_new(), pix_pipe.WORA_SPR_PIPE_A5.qp16_new());
    /* p34.TORY*/ wire TORY_SPRITE_MASK6n = or3(XEPY_SPRITE_DONEn, pix_pipe.VANU_SPR_PIPE_B6.qp16_new(), pix_pipe.VAFO_SPR_PIPE_A6.qp16_new());
    /* p34.WOPE*/ wire WOPE_SPRITE_MASK7n = or3(XEPY_SPRITE_DONEn, pix_pipe.VUPY_SPR_PIPE_B7.qp16_new(), pix_pipe.WUFY_SPR_PIPE_A7.qp16_new());

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
      /* p33.PABE*/ wire PABE_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, vram_bus.REWO_SPRITE_DA0n.qn07_new());
      /* p33.MYTO*/ wire MYTO_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, vram_bus.PEBA_SPRITE_DA1n.qn07_new());
      /* p33.LELA*/ wire LELA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, vram_bus.MOFO_SPRITE_DA2n.qn07_new());
      /* p33.MAME*/ wire MAME_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, vram_bus.PUDU_SPRITE_DA3n.qn07_new());
      /* p33.VEXU*/ wire VEXU_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, vram_bus.SAJA_SPRITE_DA4n.qn07_new());
      /* p33.VABY*/ wire VABY_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, vram_bus.SUNY_SPRITE_DA5n.qn07_new());
      /* p33.TUXA*/ wire TUXA_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, vram_bus.SEMO_SPRITE_DA6n.qn07_new());
      /* p33.VUNE*/ wire VUNE_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, vram_bus.SEGA_SPRITE_DA7n.qn07_new());

      /* p33.RATA*/ wire RATA_SPR_PIX_DA0n = not1(vram_bus.REWO_SPRITE_DA0n.qn07_new());
      /* p33.NUCA*/ wire NUCA_SPR_PIX_DA1n = not1(vram_bus.PEBA_SPRITE_DA1n.qn07_new());
      /* p33.LASE*/ wire LASE_SPR_PIX_DA2n = not1(vram_bus.MOFO_SPRITE_DA2n.qn07_new());
      /* p33.LUBO*/ wire LUBO_SPR_PIX_DA3n = not1(vram_bus.PUDU_SPRITE_DA3n.qn07_new());
      /* p33.WERY*/ wire WERY_SPR_PIX_DA4n = not1(vram_bus.SAJA_SPRITE_DA4n.qn07_new());
      /* p33.WURA*/ wire WURA_SPR_PIX_DA5n = not1(vram_bus.SUNY_SPRITE_DA5n.qn07_new());
      /* p33.SULU*/ wire SULU_SPR_PIX_DA6n = not1(vram_bus.SEMO_SPRITE_DA6n.qn07_new());
      /* p33.WAMY*/ wire WAMY_SPR_PIX_DA7n = not1(vram_bus.SEGA_SPRITE_DA7n.qn07_new());

      /* p33.PYZU*/ wire PYZU_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, RATA_SPR_PIX_DA0n);
      /* p33.MADA*/ wire MADA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, NUCA_SPR_PIX_DA1n);
      /* p33.LYDE*/ wire LYDE_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LASE_SPR_PIX_DA2n);
      /* p33.LUFY*/ wire LUFY_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, LUBO_SPR_PIX_DA3n);
      /* p33.XATO*/ wire XATO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, WERY_SPR_PIX_DA4n);
      /* p33.XEXU*/ wire XEXU_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WURA_SPR_PIX_DA5n);
      /* p33.TUPE*/ wire TUPE_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SULU_SPR_PIX_DA6n);
      /* p33.XYVE*/ wire XYVE_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, WAMY_SPR_PIX_DA7n);

      /* p33.WUFY*/ pix_pipe.WUFY_SPR_PIPE_A7.dff22_set_rst(VUNE_SPR_PIX_SET7, XYVE_SPR_PIX_RST7);
      /* p33.VAFO*/ pix_pipe.VAFO_SPR_PIPE_A6.dff22_set_rst(TUXA_SPR_PIX_SET6, TUPE_SPR_PIX_RST6);
      /* p33.WORA*/ pix_pipe.WORA_SPR_PIPE_A5.dff22_set_rst(VABY_SPR_PIX_SET5, XEXU_SPR_PIX_RST5);
      /* p33.WYHO*/ pix_pipe.WYHO_SPR_PIPE_A4.dff22_set_rst(VEXU_SPR_PIX_SET4, XATO_SPR_PIX_RST4);
      /* p33.LESU*/ pix_pipe.LESU_SPR_PIPE_A3.dff22_set_rst(MAME_SPR_PIX_SET3, LUFY_SPR_PIX_RST3);
      /* p33.LEFE*/ pix_pipe.LEFE_SPR_PIPE_A2.dff22_set_rst(LELA_SPR_PIX_SET2, LYDE_SPR_PIX_RST2);
      /* p33.MASO*/ pix_pipe.MASO_SPR_PIPE_A1.dff22_set_rst(MYTO_SPR_PIX_SET1, MADA_SPR_PIX_RST1);
      /* p33.NURO*/ pix_pipe.NURO_SPR_PIPE_A0.dff22_set_rst(PABE_SPR_PIX_SET0, PYZU_SPR_PIX_RST0);
    }

    // Sprite pipe B
    {
      /* p33.MEZU*/ wire MEZU_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, vram_bus.PEFO_SPRITE_DB0n.qn07_new());
      /* p33.RUSY*/ wire RUSY_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, vram_bus.ROKA_SPRITE_DB1n.qn07_new());
      /* p33.MYXA*/ wire MYXA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, vram_bus.MYTU_SPRITE_DB2n.qn07_new());
      /* p33.RANO*/ wire RANO_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, vram_bus.RAMU_SPRITE_DB3n.qn07_new());
      /* p33.TYGA*/ wire TYGA_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, vram_bus.SELE_SPRITE_DB4n.qn07_new());
      /* p33.VUME*/ wire VUME_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, vram_bus.SUTO_SPRITE_DB5n.qn07_new());
      /* p33.TAPO*/ wire TAPO_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, vram_bus.RAMA_SPRITE_DB6n.qn07_new());
      /* p33.TESO*/ wire TESO_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, vram_bus.RYDU_SPRITE_DB7n.qn07_new());

      /* p33.LOZA*/ wire LOZA_SPR_PIX_DB0n = not1(vram_bus.PEFO_SPRITE_DB0n.qn07_new());
      /* p33.SYBO*/ wire SYBO_SPR_PIX_DB1n = not1(vram_bus.ROKA_SPRITE_DB1n.qn07_new());
      /* p33.LUMO*/ wire LUMO_SPR_PIX_DB2n = not1(vram_bus.MYTU_SPRITE_DB2n.qn07_new());
      /* p33.SOLO*/ wire SOLO_SPR_PIX_DB3n = not1(vram_bus.RAMU_SPRITE_DB3n.qn07_new());
      /* p33.VOBY*/ wire VOBY_SPR_PIX_DB4n = not1(vram_bus.SELE_SPRITE_DB4n.qn07_new());
      /* p33.WYCO*/ wire WYCO_SPR_PIX_DB5n = not1(vram_bus.SUTO_SPRITE_DB5n.qn07_new());
      /* p33.SERY*/ wire SERY_SPR_PIX_DB6n = not1(vram_bus.RAMA_SPRITE_DB6n.qn07_new());
      /* p33.SELU*/ wire SELU_SPR_PIX_DB7n = not1(vram_bus.RYDU_SPRITE_DB7n.qn07_new());

      /* p33.MOFY*/ wire MOFY_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, LOZA_SPR_PIX_DB0n);
      /* p33.RUCA*/ wire RUCA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, SYBO_SPR_PIX_DB1n);
      /* p33.MAJO*/ wire MAJO_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LUMO_SPR_PIX_DB2n);
      /* p33.REHU*/ wire REHU_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, SOLO_SPR_PIX_DB3n);
      /* p33.WAXO*/ wire WAXO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, VOBY_SPR_PIX_DB4n);
      /* p33.XOLE*/ wire XOLE_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WYCO_SPR_PIX_DB5n);
      /* p33.TABY*/ wire TABY_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SERY_SPR_PIX_DB6n);
      /* p33.TULA*/ wire TULA_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, SELU_SPR_PIX_DB7n);

      /* p33.VUPY*/ pix_pipe.VUPY_SPR_PIPE_B7.dff22_set_rst(TESO_SPR_PIX_SET7, TULA_SPR_PIX_RST7);
      /* p33.VANU*/ pix_pipe.VANU_SPR_PIPE_B6.dff22_set_rst(TAPO_SPR_PIX_SET6, TABY_SPR_PIX_RST6);
      /* p33.WEBA*/ pix_pipe.WEBA_SPR_PIPE_B5.dff22_set_rst(VUME_SPR_PIX_SET5, XOLE_SPR_PIX_RST5);
      /* p33.VARE*/ pix_pipe.VARE_SPR_PIPE_B4.dff22_set_rst(TYGA_SPR_PIX_SET4, WAXO_SPR_PIX_RST4);
      /* p33.PYJO*/ pix_pipe.PYJO_SPR_PIPE_B3.dff22_set_rst(RANO_SPR_PIX_SET3, REHU_SPR_PIX_RST3);
      /* p33.NATY*/ pix_pipe.NATY_SPR_PIPE_B2.dff22_set_rst(MYXA_SPR_PIX_SET2, MAJO_SPR_PIX_RST2);
      /* p33.PEFU*/ pix_pipe.PEFU_SPR_PIPE_B1.dff22_set_rst(RUSY_SPR_PIX_SET1, RUCA_SPR_PIX_RST1);
      /* p33.NYLU*/ pix_pipe.NYLU_SPR_PIPE_B0.dff22_set_rst(MEZU_SPR_PIX_SET0, MOFY_SPR_PIX_RST0);
    }

    // Palette pipe
    {
      /* p34.PUME*/ wire PUME_PAL_PIPE_SET0n = nand2(LESY_SPRITE_MASK0p, oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.SORO*/ wire SORO_PAL_PIPE_SET1n = nand2(LOTA_SPRITE_MASK1p, oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.PAMO*/ wire PAMO_PAL_PIPE_SET2n = nand2(LYKU_SPRITE_MASK2p, oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.SUKY*/ wire SUKY_PAL_PIPE_SET3n = nand2(ROBY_SPRITE_MASK3p, oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.RORA*/ wire RORA_PAL_PIPE_SET4n = nand2(TYTA_SPRITE_MASK4p, oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.MENE*/ wire MENE_PAL_PIPE_SET5n = nand2(TYCO_SPRITE_MASK5p, oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.LUKE*/ wire LUKE_PAL_PIPE_SET6n = nand2(SOKA_SPRITE_MASK6p, oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.LAMY*/ wire LAMY_PAL_PIPE_SET7n = nand2(XOVU_SPRITE_MASK7p, oam_bus.GOMO_OAM_DB4p.qp08_new());

      /* p34.SYPY*/ wire SYPY = not1(oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.TOTU*/ wire TOTU = not1(oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.NARO*/ wire NARO = not1(oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.WEXY*/ wire WEXY = not1(oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.RYZY*/ wire RYZY = not1(oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.RYFE*/ wire RYFE = not1(oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.LADY*/ wire LADY = not1(oam_bus.GOMO_OAM_DB4p.qp08_new());
      /* p34.LAFY*/ wire LAFY = not1(oam_bus.GOMO_OAM_DB4p.qp08_new());

      /* p34.SUCO*/ wire SUCO_PAL_PIPE_RST0n = nand2(LESY_SPRITE_MASK0p, SYPY);
      /* p34.TAFA*/ wire TAFA_PAL_PIPE_RST1n = nand2(LOTA_SPRITE_MASK1p, TOTU);
      /* p34.PYZY*/ wire PYZY_PAL_PIPE_RST2n = nand2(LYKU_SPRITE_MASK2p, NARO);
      /* p34.TOWA*/ wire TOWA_PAL_PIPE_RST3n = nand2(ROBY_SPRITE_MASK3p, WEXY);
      /* p34.RUDU*/ wire RUDU_PAL_PIPE_RST4n = nand2(TYTA_SPRITE_MASK4p, RYZY);
      /* p34.PAZO*/ wire PAZO_PAL_PIPE_RST5n = nand2(TYCO_SPRITE_MASK5p, RYFE);
      /* p34.LOWA*/ wire LOWA_PAL_PIPE_RST6n = nand2(SOKA_SPRITE_MASK6p, LADY);
      /* p34.LUNU*/ wire LUNU_PAL_PIPE_RST7n = nand2(XOVU_SPRITE_MASK7p, LAFY);

      /* p34.LYME*/ pix_pipe.LYME_PAL_PIPE_7.dff22_set_rst(LAMY_PAL_PIPE_SET7n, LUNU_PAL_PIPE_RST7n);
      /* p34.MODA*/ pix_pipe.MODA_PAL_PIPE_6.dff22_set_rst(LUKE_PAL_PIPE_SET6n, LOWA_PAL_PIPE_RST6n);
      /* p34.NUKE*/ pix_pipe.NUKE_PAL_PIPE_5.dff22_set_rst(MENE_PAL_PIPE_SET5n, PAZO_PAL_PIPE_RST5n);
      /* p34.PALU*/ pix_pipe.PALU_PAL_PIPE_4.dff22_set_rst(RORA_PAL_PIPE_SET4n, RUDU_PAL_PIPE_RST4n);
      /* p34.SOMY*/ pix_pipe.SOMY_PAL_PIPE_3.dff22_set_rst(SUKY_PAL_PIPE_SET3n, TOWA_PAL_PIPE_RST3n);
      /* p34.ROSA*/ pix_pipe.ROSA_PAL_PIPE_2.dff22_set_rst(PAMO_PAL_PIPE_SET2n, PYZY_PAL_PIPE_RST2n);
      /* p34.SATA*/ pix_pipe.SATA_PAL_PIPE_1.dff22_set_rst(SORO_PAL_PIPE_SET1n, TAFA_PAL_PIPE_RST1n);
      /* p34.RUGO*/ pix_pipe.RUGO_PAL_PIPE_0.dff22_set_rst(PUME_PAL_PIPE_SET0n, SUCO_PAL_PIPE_RST0n);
    }

    // Background mask pipe
    {
      /* p26.TEDE*/ wire TEDE_MASK_PIPE_SET0 = nand2(LESY_SPRITE_MASK0p, oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.XALA*/ wire XALA_MASK_PIPE_SET1 = nand2(LOTA_SPRITE_MASK1p, oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.TYRA*/ wire TYRA_MASK_PIPE_SET2 = nand2(LYKU_SPRITE_MASK2p, oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.XYRU*/ wire XYRU_MASK_PIPE_SET3 = nand2(ROBY_SPRITE_MASK3p, oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.XUKU*/ wire XUKU_MASK_PIPE_SET4 = nand2(TYTA_SPRITE_MASK4p, oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.XELY*/ wire XELY_MASK_PIPE_SET5 = nand2(TYCO_SPRITE_MASK5p, oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.TYKO*/ wire TYKO_MASK_PIPE_SET6 = nand2(SOKA_SPRITE_MASK6p, oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.TUWU*/ wire TUWU_MASK_PIPE_SET7 = nand2(XOVU_SPRITE_MASK7p, oam_bus.DEPO_OAM_DB7p.qp08_new());

      /* p26.XOGA*/ wire XOGA = not1(oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.XURA*/ wire XURA = not1(oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.TAJO*/ wire TAJO = not1(oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.XENU*/ wire XENU = not1(oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.XYKE*/ wire XYKE = not1(oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.XABA*/ wire XABA = not1(oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.TAFU*/ wire TAFU = not1(oam_bus.DEPO_OAM_DB7p.qp08_new());
      /* p26.XUHO*/ wire XUHO = not1(oam_bus.DEPO_OAM_DB7p.qp08_new());

      /* p26.WOKA*/ wire WOKA_MASK_PIPE_RST0 = nand2(LESY_SPRITE_MASK0p, XOGA);
      /* p26.WEDE*/ wire WEDE_MASK_PIPE_RST1 = nand2(LOTA_SPRITE_MASK1p, XURA);
      /* p26.TUFO*/ wire TUFO_MASK_PIPE_RST2 = nand2(LYKU_SPRITE_MASK2p, TAJO);
      /* p26.WEVO*/ wire WEVO_MASK_PIPE_RST3 = nand2(ROBY_SPRITE_MASK3p, XENU);
      /* p26.WEDY*/ wire WEDY_MASK_PIPE_RST4 = nand2(TYTA_SPRITE_MASK4p, XYKE);
      /* p26.WUJA*/ wire WUJA_MASK_PIPE_RST5 = nand2(TYCO_SPRITE_MASK5p, XABA);
      /* p26.TENA*/ wire TENA_MASK_PIPE_RST6 = nand2(SOKA_SPRITE_MASK6p, TAFU);
      /* p26.WUBU*/ wire WUBU_MASK_PIPE_RST7 = nand2(XOVU_SPRITE_MASK7p, XUHO);

      /* p26.VAVA*/ pix_pipe.VAVA_MASK_PIPE_7.dff22_set_rst(TUWU_MASK_PIPE_SET7, WUBU_MASK_PIPE_RST7);
      /* p26.VUMO*/ pix_pipe.VUMO_MASK_PIPE_6.dff22_set_rst(TYKO_MASK_PIPE_SET6, TENA_MASK_PIPE_RST6);
      /* p26.WODA*/ pix_pipe.WODA_MASK_PIPE_5.dff22_set_rst(XELY_MASK_PIPE_SET5, WUJA_MASK_PIPE_RST5);
      /* p26.XETE*/ pix_pipe.XETE_MASK_PIPE_4.dff22_set_rst(XUKU_MASK_PIPE_SET4, WEDY_MASK_PIPE_RST4);
      /* p26.WYFU*/ pix_pipe.WYFU_MASK_PIPE_3.dff22_set_rst(XYRU_MASK_PIPE_SET3, WEVO_MASK_PIPE_RST3);
      /* p26.VOSA*/ pix_pipe.VOSA_MASK_PIPE_2.dff22_set_rst(TYRA_MASK_PIPE_SET2, TUFO_MASK_PIPE_RST2);
      /* p26.WURU*/ pix_pipe.WURU_MASK_PIPE_1.dff22_set_rst(XALA_MASK_PIPE_SET1, WEDE_MASK_PIPE_RST1);
      /* p26.VEZO*/ pix_pipe.VEZO_MASK_PIPE_0.dff22_set_rst(TEDE_MASK_PIPE_SET0, WOKA_MASK_PIPE_RST0);
    }
  }
#pragma endregion































  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //============================================================================== COMBI OUTPUT ONLY BELOW THIS LINE
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================





























  //------------------------------------------------------------------------------

  /* LCD Pixel Pipe */ {
    /*PIN_50*/ Pin2 PIN_LCD_DATA1;
    /*PIN_51*/ Pin2 PIN_LCD_DATA0;
    {
      // Pixel merge + emit
      /*#p35.RAJY*/ wire RAJY_PIX_BG_LOp  = and2(pix_pipe.PYBO_BG_PIPE_A7.qp16_new(), pix_pipe.VYXE_LCDC_BGENn.qn08_new());
      /*#p35.TADE*/ wire TADE_PIX_BG_HIp  = and2(pix_pipe.SOHU_BG_PIPE_B7.qp16_new(), pix_pipe.VYXE_LCDC_BGENn.qn08_new());
      /*#p35.XULA*/ wire XULA_PIX_SP_LOp  = and2(pix_pipe.XYLO_LCDC_SPENn.qn08_new(), pix_pipe.WUFY_SPR_PIPE_A7.qp16_new());
      /*#p35.WOXA*/ wire WOXA_PIX_SP_HIp  = and2(pix_pipe.XYLO_LCDC_SPENn.qn08_new(), pix_pipe.VUPY_SPR_PIPE_B7.qp16_new());

      /*#p35.NULY*/ wire NULY_PIX_SP_MASKn = nor2(WOXA_PIX_SP_HIp, XULA_PIX_SP_LOp);

      /*#p35.RYFU*/ wire RYFU_MASK_BG0 = and2(RAJY_PIX_BG_LOp, pix_pipe.VAVA_MASK_PIPE_7.qp16_new());
      /*#p35.RUTA*/ wire RUTA_MASK_BG1 = and2(TADE_PIX_BG_HIp, pix_pipe.VAVA_MASK_PIPE_7.qp16_new());
      /*#p35.POKA*/ wire POKA_BGPIXELn = nor3(NULY_PIX_SP_MASKn, RUTA_MASK_BG1, RYFU_MASK_BG0);

      /*#p34.LOME*/ wire LOME_PAL_PIPE_7n = not1(pix_pipe.LYME_PAL_PIPE_7.qp16_new());
      /*#p34.LAFU*/ wire LAFU_OBP0PIXELn = nand2(LOME_PAL_PIPE_7n, POKA_BGPIXELn);
      /*#p34.LEKA*/ wire LEKA_OBP1PIXELn = nand2(pix_pipe.LYME_PAL_PIPE_7.qp16_new(), POKA_BGPIXELn);

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

      /*#p35.WUFU*/ wire WUFU_COL_OBP0_HI = amux4(pix_pipe.XANA_OBP0_D7n.qn07_new(), VYRO_PAL_OBP0D,
                                                  pix_pipe.XYZE_OBP0_D5n.qn07_new(), VATA_PAL_OBP0C,
                                                  pix_pipe.XALO_OBP0_D3n.qn07_new(), VOLO_PAL_OBP0B,
                                                  pix_pipe.XUKY_OBP0_D1n.qn07_new(), VUGO_PAL_OBP0A);

      /*#p35.WALY*/ wire WALY_COL_OBP0_LO = amux4(pix_pipe.XUPO_OBP0_D6n.qn07_new(), VYRO_PAL_OBP0D,
                                                  pix_pipe.XERU_OBP0_D4n.qn07_new(), VATA_PAL_OBP0C,
                                                  pix_pipe.XOVA_OBP0_D2n.qn07_new(), VOLO_PAL_OBP0B,
                                                  pix_pipe.XUFU_OBP0_D0n.qn07_new(), VUGO_PAL_OBP0A);

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

      /*#p35.MOKA*/ wire MOKA_COL_OBP1_HI = amux4(pix_pipe.LUXO_OBP1_D7n.qn07_new(), LEDO_PAL_OBP1D,
                                                  pix_pipe.LUGU_OBP1_D5n.qn07_new(), LARU_PAL_OBP1C,
                                                  pix_pipe.LOSE_OBP1_D3n.qn07_new(), LYKY_PAL_OBP1B,
                                                  pix_pipe.LAWO_OBP1_D1n.qn07_new(), LOPU_PAL_OBP1A);

      /*#p35.MUFA*/ wire MUFA_COL_OBP1_LO = amux4(LEDO_PAL_OBP1D, pix_pipe.LEPU_OBP1_D6n.qn07_new(),
                                                  LARU_PAL_OBP1C, pix_pipe.LUNE_OBP1_D4n.qn07_new(),
                                                  LYKY_PAL_OBP1B, pix_pipe.MOSA_OBP1_D2n.qn07_new(),
                                                  LOPU_PAL_OBP1A, pix_pipe.MOXY_OBP1_D0n.qn07_new());

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

      /*#p35.NELO*/ wire NELO_COL_BG_LO = amux4(NYPO_PAL_BGPD, pix_pipe.MOGY_BGP_D6n.qn07_new(),
                                                NUMA_PAL_BGPC, pix_pipe.MUKE_BGP_D4n.qn07_new(),
                                                NUXO_PAL_BGPB, pix_pipe.PYLU_BGP_D2n.qn07_new(),
                                                POBU_PAL_BGPA, pix_pipe.PAVO_BGP_D0n.qn07_new());

      /*#p35.NURA*/ wire NURA_COL_BG_HI = amux4(pix_pipe.MENA_BGP_D7n.qn07_new(), NYPO_PAL_BGPD,
                                                pix_pipe.MORU_BGP_D5n.qn07_new(), NUMA_PAL_BGPC,
                                                pix_pipe.MAXY_BGP_D3n.qn07_new(), NUXO_PAL_BGPB,
                                                pix_pipe.NUSY_BGP_D1n.qn07_new(), POBU_PAL_BGPA);

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
      wire _POME = pix_pipe.POME.to_wire_old();
      wire _RUJU = pix_pipe.RUJU.to_wire_old();
      wire _POFY = pix_pipe.POFY.to_wire_old();
      /*#p24.POME*/ pix_pipe.POME.set(nor2(_AVAP_SCAN_DONE_TRIGp_t2, _POFY));
      /*#p24.RUJU*/ pix_pipe.RUJU.set(or3(pix_pipe.PAHO_X_8_SYNC.qp17_new(), _TOFU_VID_RSTp_t0, _POME));
      /*#p24.POFY*/ pix_pipe.POFY.set(not1(_RUJU));
      /*#p24.RUZE*/ wire _RUZE_HSYNCn = not1(_POFY);
      PIN_LCD_HSYNC.pin_out(1, _RUZE_HSYNCn, _RUZE_HSYNCn);
    }

    /*PIN_56*/ Pin2 PIN_LCD_FLIPS;
    {
      // if LCDC_ENn, LCD_PIN_ALTSG = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)
      /*#p24.MAGU*/ wire _MAGU = xor2(lcd_reg.NAPO_FRAME_EVENp.qp17_new(), lcd_reg.LUCA_LINE_EVENp.qn16_new());
      /*#p24.MECO*/ wire _MECO = not1(_MAGU);
      /*#p24.KEBO*/ wire _KEBO = not1(_MECO);
      /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(pix_pipe.XONA_LCDC_LCDENn.qn08_new());
      /* p01.UREK*/ wire _UREK_DIV07n_t0 = not1(tim_reg.TULU_DIV07p.qp17_new());
      /*#p24.USEC*/ wire _USEC_DIV_07p = not1(_UREK_DIV07n_t0);
      /*#p24.KUPA*/ wire _KUPA = amux2(pix_pipe.XONA_LCDC_LCDENn.qn08_new(), _KEBO, _KEDY_LCDC_ENn, _USEC_DIV_07p);
      /*#p24.KOFO*/ wire _KOFO = not1(_KUPA);

      PIN_LCD_FLIPS.pin_out(1, _KOFO, _KOFO);
    }

    /*PIN_52*/ Pin2 PIN_LCD_CNTRL;
    {
      /*#p21.RYNO*/ wire _RYNO = or2(lcd_reg.SYGU_LINE_STROBE.qp17_new(), lcd_reg.RUTU_LINE_P910p.qp17_new());
      /*#p21.POGU*/ wire _POGU = not1(_RYNO);
      PIN_LCD_CNTRL.pin_out(1, _POGU, _POGU);
    }

    /*PIN_55*/ Pin2 PIN_LCD_LATCH;
    {
      /*#p24.KASA*/ wire _KASA_LINE_ENDp = not1(_PURE_LINE_P908n_t2);
      /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(pix_pipe.XONA_LCDC_LCDENn.qn08_new());
      /* p01.UMEK*/ wire _UMEK_DIV06n_t0 = not1(tim_reg.UGOT_DIV06p.qp17_new());
      /*#p24.UMOB*/ wire UMOB_DIV_06p = not1(_UMEK_DIV06n_t0);
      /*#p24.KAHE*/ wire _KAHE_LINE_ENDp = amux2(pix_pipe.XONA_LCDC_LCDENn.qn08_new(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, UMOB_DIV_06p);
      /*#p24.KYMO*/ wire _KYMO_LINE_ENDn = not1(_KAHE_LINE_ENDp);
      PIN_LCD_LATCH.pin_out(1, _KYMO_LINE_ENDn, _KYMO_LINE_ENDn);
    }

    /*PIN_53*/ Pin2 PIN_LCD_CLOCK;
    {
      /*#p21.XAJO*/ wire _XAJO_X_009p = and2(pix_pipe.XEHO_PX0p.qp17_new(), pix_pipe.XYDO_PX3p.qp17_new());
      /*#p21.VOGA*/ wire _VOGA_HBLANKp_t0 = pix_pipe.VOGA_HBLANKp.qp17_new();
      /*#p21.WEGO*/ wire _WEGO_HBLANKp_t0 = or2(_TOFU_VID_RSTp_t0, _VOGA_HBLANKp_t0);
      /*#p21.WUSA*/ pix_pipe.WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p, _WEGO_HBLANKp_t0);
      /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK = and2(pix_pipe.WUSA_LCD_CLOCK_GATE.qp04_new(), _SACU_CLKPIPEp_t2);
      /*#p27.POVA*/ wire _POVA_FINE_MATCHpe_t0 = and2(pix_pipe.PUXA_SCX_FINE_MATCH_A.qp17_new(), pix_pipe.NYZE_SCX_FINE_MATCH_B.qn16_new());
      /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, _POVA_FINE_MATCHpe_t0);
      /*#p21.RYPO*/ wire _RYPO_LCD_CLOCK = not1(_SEMU_LCD_CLOCK);
      PIN_LCD_CLOCK.pin_out(1, _RYPO_LCD_CLOCK, _RYPO_LCD_CLOCK);
    }

    /*PIN_57*/ Pin2 PIN_LCD_VSYNC;
    {
      /*#p24.MURE*/ wire _MURE_VSYNC = not1(lcd_reg.MEDA_VSYNC_OUTn.qp17_new());
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
          uint8_t p0 = lcd_pipe_lo[x + 1].qp_old();
          uint8_t p1 = lcd_pipe_hi[x + 1].qp_old();
          framebuffer[x + gb_screen_y * 160] = p0 + p1 * 2;
        }
        {
          uint8_t p0 = lcd_pix_lo.qp04_new();
          uint8_t p1 = lcd_pix_hi.qp04_new();
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
        lcd_pipe_lo[i].dff(!PIN_LCD_CLOCK.qp(), lcd_pipe_lo[i + 1].qp_old());
        lcd_pipe_hi[i].dff(!PIN_LCD_CLOCK.qp(), lcd_pipe_hi[i + 1].qp_old());
      }

      lcd_pipe_lo[159].dff(!PIN_LCD_CLOCK.qp(), lcd_pix_lo.qp04_new());
      lcd_pipe_hi[159].dff(!PIN_LCD_CLOCK.qp(), lcd_pix_hi.qp04_new());

    }

    old_lcd_clock = PIN_LCD_CLOCK.qp();
    old_lcd_latch = PIN_LCD_LATCH.qp();
  }

  //------------------------------------------------------------------------------
  // Latch stuff for CPU

#pragma region BUS_CPU_Dp_out_Drivers
  {
    /* FF01 SER */
    /*#p06.CUGY*/ BUS_CPU_Dp_out[0].tri6_pn(_UFEG_FF01_RDp_t0, ser_reg.CUBA_SER_DATA0.qn15_new());
    /* p06.DUDE*/ BUS_CPU_Dp_out[1].tri6_pn(_UFEG_FF01_RDp_t0, ser_reg.DEGU_SER_DATA1.qn15_new());
    /* p06.DETU*/ BUS_CPU_Dp_out[2].tri6_pn(_UFEG_FF01_RDp_t0, ser_reg.DYRA_SER_DATA2.qn15_new());
    /* p06.DASO*/ BUS_CPU_Dp_out[3].tri6_pn(_UFEG_FF01_RDp_t0, ser_reg.DOJO_SER_DATA3.qn15_new());
    /* p06.DAME*/ BUS_CPU_Dp_out[4].tri6_pn(_UFEG_FF01_RDp_t0, ser_reg.DOVU_SER_DATA4.qn15_new());
    /* p06.EVOK*/ BUS_CPU_Dp_out[5].tri6_pn(_UFEG_FF01_RDp_t0, ser_reg.EJAB_SER_DATA5.qn15_new());
    /* p06.EFAB*/ BUS_CPU_Dp_out[6].tri6_pn(_UFEG_FF01_RDp_t0, ser_reg.EROD_SER_DATA6.qn15_new());
    /*#p06.ETAK*/ BUS_CPU_Dp_out[7].tri6_pn(_UFEG_FF01_RDp_t0, ser_reg.EDER_SER_DATA7.qn15_new());

    /* FF02 SER */
    /* p06.CORE*/ BUS_CPU_Dp_out[0].tri6_pn(_UCOM_FF02_RDp_t0, ser_reg.CULY_XFER_DIR.qn16_new());
    /* p06.ELUV*/ BUS_CPU_Dp_out[7].tri6_pn(_UCOM_FF02_RDp_t0, ser_reg.ETAF_SER_RUNNING.qn16_new());
  }

  {
    /* FF04 DIV */
    /* p01.UMEK*/ wire _UMEK_DIV06n_t0 = not1(tim_reg.UGOT_DIV06p.qp17_new());
    /* p01.UREK*/ wire _UREK_DIV07n_t0 = not1(tim_reg.TULU_DIV07p.qp17_new());
    /* p01.UTOK*/ wire _UTOK_DIV08n_t0 = not1(tim_reg.TUGO_DIV08p.qp17_new());
    /* p01.SAPY*/ wire _SAPY_DIV09n_t0 = not1(tim_reg.TOFE_DIV09p.qp17_new());
    /* p01.UMER*/ wire _UMER_DIV10n_t0 = not1(tim_reg.TERU_DIV10p.qp17_new());
    /* p01.RAVE*/ wire _RAVE_DIV11n_t0 = not1(tim_reg.SOLA_DIV11p.qp17_new());
    /* p01.RYSO*/ wire _RYSO_DIV12n_t0 = not1(tim_reg.SUBU_DIV12p.qp17_new());
    /* p01.UDOR*/ wire _UDOR_DIV13n_t0 = not1(tim_reg.TEKA_DIV13p.qp17_new());

    /* p01.TAWU*/ BUS_CPU_Dp_out[0].tri6_pn(_TAGY_FF04_RDp_t0, _UMEK_DIV06n_t0);
    /* p01.TAKU*/ BUS_CPU_Dp_out[1].tri6_pn(_TAGY_FF04_RDp_t0, _UREK_DIV07n_t0);
    /* p01.TEMU*/ BUS_CPU_Dp_out[2].tri6_pn(_TAGY_FF04_RDp_t0, _UTOK_DIV08n_t0);
    /* p01.TUSE*/ BUS_CPU_Dp_out[3].tri6_pn(_TAGY_FF04_RDp_t0, _SAPY_DIV09n_t0);
    /* p01.UPUG*/ BUS_CPU_Dp_out[4].tri6_pn(_TAGY_FF04_RDp_t0, _UMER_DIV10n_t0); // Schematic wrong, UPUG/SEPU driving D5/D4
    /* p01.SEPU*/ BUS_CPU_Dp_out[5].tri6_pn(_TAGY_FF04_RDp_t0, _RAVE_DIV11n_t0);
    /* p01.SAWA*/ BUS_CPU_Dp_out[6].tri6_pn(_TAGY_FF04_RDp_t0, _RYSO_DIV12n_t0);
    /* p01.TATU*/ BUS_CPU_Dp_out[7].tri6_pn(_TAGY_FF04_RDp_t0, _UDOR_DIV13n_t0);
  }

  {
    /* FF05 TIMA */
    /*#p03.SOKU*/ BUS_CPU_Dp_out[0].tri6_pn(_TEDA_FF05_RDp_t0, tim_reg.REGA_TIMA0p.qn17_new());
    /*#p03.RACY*/ BUS_CPU_Dp_out[1].tri6_pn(_TEDA_FF05_RDp_t0, tim_reg.POVY_TIMA1p.qn17_new());
    /*#p03.RAVY*/ BUS_CPU_Dp_out[2].tri6_pn(_TEDA_FF05_RDp_t0, tim_reg.PERU_TIMA2p.qn17_new());
    /*#p03.SOSY*/ BUS_CPU_Dp_out[3].tri6_pn(_TEDA_FF05_RDp_t0, tim_reg.RATE_TIMA3p.qn17_new());
    /*#p03.SOMU*/ BUS_CPU_Dp_out[4].tri6_pn(_TEDA_FF05_RDp_t0, tim_reg.RUBY_TIMA4p.qn17_new());
    /*#p03.SURO*/ BUS_CPU_Dp_out[5].tri6_pn(_TEDA_FF05_RDp_t0, tim_reg.RAGE_TIMA5p.qn17_new());
    /*#p03.ROWU*/ BUS_CPU_Dp_out[6].tri6_pn(_TEDA_FF05_RDp_t0, tim_reg.PEDA_TIMA6p.qn17_new());
    /*#p03.PUSO*/ BUS_CPU_Dp_out[7].tri6_pn(_TEDA_FF05_RDp_t0, tim_reg.NUGA_TIMA7p.qn17_new());
  }

  {
    /* FF06 TMA */
    /*#p03.SETE*/ BUS_CPU_Dp_out[0].tri6_pn(_TUBY_FF06_RDp_t0, tim_reg.SABU_TMA0p.qn16_new());
    /*#p03.PYRE*/ BUS_CPU_Dp_out[1].tri6_pn(_TUBY_FF06_RDp_t0, tim_reg.NYKE_TMA1p.qn16_new());
    /*#p03.NOLA*/ BUS_CPU_Dp_out[2].tri6_pn(_TUBY_FF06_RDp_t0, tim_reg.MURU_TMA2p.qn16_new());
    /*#p03.SALU*/ BUS_CPU_Dp_out[3].tri6_pn(_TUBY_FF06_RDp_t0, tim_reg.TYVA_TMA3p.qn16_new());
    /*#p03.SUPO*/ BUS_CPU_Dp_out[4].tri6_pn(_TUBY_FF06_RDp_t0, tim_reg.TYRU_TMA4p.qn16_new());
    /*#p03.SOTU*/ BUS_CPU_Dp_out[5].tri6_pn(_TUBY_FF06_RDp_t0, tim_reg.SUFY_TMA5p.qn16_new());
    /*#p03.REVA*/ BUS_CPU_Dp_out[6].tri6_pn(_TUBY_FF06_RDp_t0, tim_reg.PETO_TMA6p.qn16_new());
    /*#p03.SAPU*/ BUS_CPU_Dp_out[7].tri6_pn(_TUBY_FF06_RDp_t0, tim_reg.SETA_TMA7p.qn16_new());
  }

  {
    /* FF07 TAC */
    /*#p03.RYLA*/ BUS_CPU_Dp_out[0].tri6_pn(_SORA_FF07_RDp_t0, tim_reg.SOPU_TAC0p.qn16_new());
    /*#p03.ROTE*/ BUS_CPU_Dp_out[1].tri6_pn(_SORA_FF07_RDp_t0, tim_reg.SAMY_TAC1p.qn16_new());
    /*#p03.SUPE*/ BUS_CPU_Dp_out[2].tri6_pn(_SORA_FF07_RDp_t0, tim_reg.SABO_TAC2p.qn16_new());
  }

  {
    /* FF0F INTF */
    // FIXME this inversion fixes a bunch of tests...
    // MATY is one of those big yellow latchy things
    /* p02.MATY*/ int_reg.MATY_FF0F_L0p.tp_latch(!_ROLO_FF0F_RDn_t0, int_reg.LOPE_FF0F_D0p.qp16_new()); // OUTPUT ON RUNG 10
    /* p02.MOPO*/ int_reg.MOPO_FF0F_L1p.tp_latch(!_ROLO_FF0F_RDn_t0, int_reg.LALU_FF0F_D1p.qp16_new()); // OUTPUT ON RUNG 10
    /* p02.PAVY*/ int_reg.PAVY_FF0F_L2p.tp_latch(!_ROLO_FF0F_RDn_t0, int_reg.NYBO_FF0F_D2p.qp16_new()); // OUTPUT ON RUNG 10
    /* p02.NEJY*/ int_reg.NEJY_FF0F_L3p.tp_latch(!_ROLO_FF0F_RDn_t0, int_reg.UBUL_FF0F_D3p.qp16_new()); // OUTPUT ON RUNG 10
    /* p02.NUTY*/ int_reg.NUTY_FF0F_L4p.tp_latch(!_ROLO_FF0F_RDn_t0, int_reg.ULAK_FF0F_D4p.qp16_new()); // OUTPUT ON RUNG 10

    /*#p02.NELA*/ BUS_CPU_Dp_out[0].tri6_pn(_POLA_FF0F_RDp_t0, int_reg.MATY_FF0F_L0p.qn10_new());
    /*#p02.NABO*/ BUS_CPU_Dp_out[1].tri6_pn(_POLA_FF0F_RDp_t0, int_reg.MOPO_FF0F_L1p.qn10_new());
    /*#p02.ROVA*/ BUS_CPU_Dp_out[2].tri6_pn(_POLA_FF0F_RDp_t0, int_reg.PAVY_FF0F_L2p.qn10_new());
    /*#p02.PADO*/ BUS_CPU_Dp_out[3].tri6_pn(_POLA_FF0F_RDp_t0, int_reg.NEJY_FF0F_L3p.qn10_new());
    /*#p02.PEGY*/ BUS_CPU_Dp_out[4].tri6_pn(_POLA_FF0F_RDp_t0, int_reg.NUTY_FF0F_L4p.qn10_new());
  }

  {
    /* FF40 LCDC */
    /*#p23.WYPO*/ BUS_CPU_Dp_out[0].tri6_nn(_WYCE_FF40_RDn_t0, pix_pipe.VYXE_LCDC_BGENn  .qp09_new());
    /*#p23.XERO*/ BUS_CPU_Dp_out[1].tri6_nn(_WYCE_FF40_RDn_t0, pix_pipe.XYLO_LCDC_SPENn  .qp09_new());
    /* p23.WYJU*/ BUS_CPU_Dp_out[2].tri6_nn(_WYCE_FF40_RDn_t0, pix_pipe.XYMO_LCDC_SPSIZEn.qp09_new());
    /* p23.WUKA*/ BUS_CPU_Dp_out[3].tri6_nn(_WYCE_FF40_RDn_t0, pix_pipe.XAFO_LCDC_BGMAPn .qp09_new());
    /* p23.VOKE*/ BUS_CPU_Dp_out[4].tri6_nn(_WYCE_FF40_RDn_t0, pix_pipe.WEXU_LCDC_BGTILEn.qp09_new());
    /* p23.VATO*/ BUS_CPU_Dp_out[5].tri6_nn(_WYCE_FF40_RDn_t0, pix_pipe.WYMO_LCDC_WINENn .qp09_new());
    /*#p23.VAHA*/ BUS_CPU_Dp_out[6].tri6_nn(_WYCE_FF40_RDn_t0, pix_pipe.WOKY_LCDC_WINMAPn.qp09_new());
    /*#p23.XEBU*/ BUS_CPU_Dp_out[7].tri6_nn(_WYCE_FF40_RDn_t0, pix_pipe.XONA_LCDC_LCDENn .qp09_new());
  }

  {
    /* FF41 STAT */
    // This latch isn't actually used as a latch, it's effectively an inverter.
    /* p21.PAGO*/ wire _PAGO_LYC_MATCH_RST_t1 = or2(_WESY_SYS_RSTn_t0, _RYJU_FF41_WRn_t1);
    /* p21.RUPO*/ pix_pipe.RUPO_LYC_MATCH_LATCHn.nor_latch(_PAGO_LYC_MATCH_RST_t1, lcd_reg.ROPO_LY_MATCH_SYNCp.qp17_new());

    /* p21.SADU*/ wire _SADU_STAT_MODE0n = nor2(pix_pipe.XYMU_RENDERINGn.qn03_new(), _PARU_VBLANKp_t3); // die NOR
    /*#p28.BOGE*/ wire _BOGE_DMA_RUNNINGn_t0 = not1(dma_reg.MATU_DMA_RUNNINGp.qp17_new());
    /*#p28.ACYL*/ wire _ACYL_SCANNINGp_t2 = and2(_BOGE_DMA_RUNNINGn_t0, sprite_scanner.BESU_SCANNINGp.qp04_new());
    /* p21.XATY*/ wire _XATY_STAT_MODE1n = nor2(_ACYL_SCANNINGp_t2, pix_pipe.XYMU_RENDERINGn.qn03_new()); // die NOR
    /*#p21.TEBY*/ BUS_CPU_Dp_out[0].tri6_pn(_TOBE_FF41_RDp_t0, _SADU_STAT_MODE0n);
    /*#p21.WUGA*/ BUS_CPU_Dp_out[1].tri6_pn(_TOBE_FF41_RDp_t0, _XATY_STAT_MODE1n);
    /*#p21.SEGO*/ BUS_CPU_Dp_out[2].tri6_pn(_TOBE_FF41_RDp_t0, pix_pipe.RUPO_LYC_MATCH_LATCHn.qp04_new());
    /* p21.PUZO*/ BUS_CPU_Dp_out[3].tri6_nn(_VAVE_FF41_RDn_t0, pix_pipe.ROXE_STAT_HBI_ENn.qp09_new());
    /* p21.POFO*/ BUS_CPU_Dp_out[4].tri6_nn(_VAVE_FF41_RDn_t0, pix_pipe.RUFO_STAT_VBI_ENn.qp09_new());
    /* p21.SASY*/ BUS_CPU_Dp_out[5].tri6_nn(_VAVE_FF41_RDn_t0, pix_pipe.REFE_STAT_OAI_ENn.qp09_new());
    /* p21.POTE*/ BUS_CPU_Dp_out[6].tri6_nn(_VAVE_FF41_RDn_t0, pix_pipe.RUGU_STAT_LYI_ENn.qp09_new());
  }

  {
    /* FF42 SCY */
    /*#p23.WARE*/ BUS_CPU_Dp_out[0].tri6_nn(_BUWY_FF42_RDn_t0, pix_pipe.GAVE_SCY0n.qp09_new());
    /* p23.GOBA*/ BUS_CPU_Dp_out[1].tri6_nn(_BUWY_FF42_RDn_t0, pix_pipe.FYMO_SCY1n.qp09_new());
    /* p23.GONU*/ BUS_CPU_Dp_out[2].tri6_nn(_BUWY_FF42_RDn_t0, pix_pipe.FEZU_SCY2n.qp09_new());
    /* p23.GODO*/ BUS_CPU_Dp_out[3].tri6_nn(_BUWY_FF42_RDn_t0, pix_pipe.FUJO_SCY3n.qp09_new());
    /* p23.CUSA*/ BUS_CPU_Dp_out[4].tri6_nn(_BUWY_FF42_RDn_t0, pix_pipe.DEDE_SCY4n.qp09_new());
    /* p23.GYZO*/ BUS_CPU_Dp_out[5].tri6_nn(_BUWY_FF42_RDn_t0, pix_pipe.FOTY_SCY5n.qp09_new());
    /* p23.GUNE*/ BUS_CPU_Dp_out[6].tri6_nn(_BUWY_FF42_RDn_t0, pix_pipe.FOHA_SCY6n.qp09_new());
    /* p23.GYZA*/ BUS_CPU_Dp_out[7].tri6_nn(_BUWY_FF42_RDn_t0, pix_pipe.FUNY_SCY7n.qp09_new());
  }

  {
    /* FF43 SCX */
    /*#p23.EDOS*/ BUS_CPU_Dp_out[0].tri6_nn(_BEBA_FF43_RDn_t0, pix_pipe.DATY_SCX0n.qp09_new());
    /* p23.EKOB*/ BUS_CPU_Dp_out[1].tri6_nn(_BEBA_FF43_RDn_t0, pix_pipe.DUZU_SCX1n.qp09_new());
    /* p23.CUGA*/ BUS_CPU_Dp_out[2].tri6_nn(_BEBA_FF43_RDn_t0, pix_pipe.CYXU_SCX2n.qp09_new());
    /* p23.WONY*/ BUS_CPU_Dp_out[3].tri6_nn(_BEBA_FF43_RDn_t0, pix_pipe.GUBO_SCX3n.qp09_new());
    /* p23.CEDU*/ BUS_CPU_Dp_out[4].tri6_nn(_BEBA_FF43_RDn_t0, pix_pipe.BEMY_SCX4n.qp09_new());
    /* p23.CATA*/ BUS_CPU_Dp_out[5].tri6_nn(_BEBA_FF43_RDn_t0, pix_pipe.CUZY_SCX5n.qp09_new());
    /* p23.DOXE*/ BUS_CPU_Dp_out[6].tri6_nn(_BEBA_FF43_RDn_t0, pix_pipe.CABU_SCX6n.qp09_new());
    /* p23.CASY*/ BUS_CPU_Dp_out[7].tri6_nn(_BEBA_FF43_RDn_t0, pix_pipe.BAKE_SCX7n.qp09_new());
  }

  {
    /* FF44 LY */
    /*#p23.WURY*/ wire _WURY_LY0n_t0 = not1(lcd_reg.MUWY_LY0p.qp17_new());
    /* p23.XEPO*/ wire _XEPO_LY1n_t0 = not1(lcd_reg.MYRO_LY1p.qp17_new());
    /* p23.MYFA*/ wire _MYFA_LY2n_t0 = not1(lcd_reg.LEXA_LY2p.qp17_new());
    /* p23.XUHY*/ wire _XUHY_LY3n_t0 = not1(lcd_reg.LYDO_LY3p.qp17_new());
    /* p23.WATA*/ wire _WATA_LY4n_t0 = not1(lcd_reg.LOVU_LY4p.qp17_new());
    /* p23.XAGA*/ wire _XAGA_LY5n_t0 = not1(lcd_reg.LEMA_LY5p.qp17_new());
    /* p23.XUCE*/ wire _XUCE_LY6n_t0 = not1(lcd_reg.MATO_LY6p.qp17_new());
    /* p23.XOWO*/ wire _XOWO_LY7n_t0 = not1(lcd_reg.LAFO_LY7p.qp17_new());

    /* p23.VEGA*/ BUS_CPU_Dp_out[0].tri6_nn(_VARO_FF44_RDn_t0, _WURY_LY0n_t0);
    /* p23.WUVA*/ BUS_CPU_Dp_out[1].tri6_nn(_VARO_FF44_RDn_t0, _XEPO_LY1n_t0);
    /* p23.LYCO*/ BUS_CPU_Dp_out[2].tri6_nn(_VARO_FF44_RDn_t0, _MYFA_LY2n_t0);
    /* p23.WOJY*/ BUS_CPU_Dp_out[3].tri6_nn(_VARO_FF44_RDn_t0, _XUHY_LY3n_t0);
    /* p23.VYNE*/ BUS_CPU_Dp_out[4].tri6_nn(_VARO_FF44_RDn_t0, _WATA_LY4n_t0);
    /* p23.WAMA*/ BUS_CPU_Dp_out[5].tri6_nn(_VARO_FF44_RDn_t0, _XAGA_LY5n_t0);
    /* p23.WAVO*/ BUS_CPU_Dp_out[6].tri6_nn(_VARO_FF44_RDn_t0, _XUCE_LY6n_t0);
    /* p23.WEZE*/ BUS_CPU_Dp_out[7].tri6_nn(_VARO_FF44_RDn_t0, _XOWO_LY7n_t0);
  }

  {
    /* FF45 LYC */
    /*#p23.RETU*/ BUS_CPU_Dp_out[0].tri6_nn(_WEKU_FF45_RDn_t0, lcd_reg.SYRY_LYC0n.qp09_new());
    /* p23.VOJO*/ BUS_CPU_Dp_out[1].tri6_nn(_WEKU_FF45_RDn_t0, lcd_reg.VUCE_LYC1n.qp09_new());
    /* p23.RAZU*/ BUS_CPU_Dp_out[2].tri6_nn(_WEKU_FF45_RDn_t0, lcd_reg.SEDY_LYC2n.qp09_new());
    /* p23.REDY*/ BUS_CPU_Dp_out[3].tri6_nn(_WEKU_FF45_RDn_t0, lcd_reg.SALO_LYC3n.qp09_new());
    /* p23.RACE*/ BUS_CPU_Dp_out[4].tri6_nn(_WEKU_FF45_RDn_t0, lcd_reg.SOTA_LYC4n.qp09_new());
    /*#p23.VAZU*/ BUS_CPU_Dp_out[5].tri6_nn(_WEKU_FF45_RDn_t0, lcd_reg.VAFA_LYC5n.qp09_new());
    /* p23.VAFE*/ BUS_CPU_Dp_out[6].tri6_nn(_WEKU_FF45_RDn_t0, lcd_reg.VEVO_LYC6n.qp09_new());
    /* p23.PUFY*/ BUS_CPU_Dp_out[7].tri6_nn(_WEKU_FF45_RDn_t0, lcd_reg.RAHA_LYC7n.qp09_new());
  }

  {
    /* FF46 DMA */
    /*#p04.POLY*/ BUS_CPU_Dp_out[0].tri6_pn(_PUSY_FF46_RDp_t0, dma_reg.NAFA_DMA_A08n.qp08_new());
    /* p04.ROFO*/ BUS_CPU_Dp_out[1].tri6_pn(_PUSY_FF46_RDp_t0, dma_reg.PYNE_DMA_A09n.qp08_new());
    /* p04.REMA*/ BUS_CPU_Dp_out[2].tri6_pn(_PUSY_FF46_RDp_t0, dma_reg.PARA_DMA_A10n.qp08_new());
    /* p04.PANE*/ BUS_CPU_Dp_out[3].tri6_pn(_PUSY_FF46_RDp_t0, dma_reg.NYDO_DMA_A11n.qp08_new());
    /* p04.PARE*/ BUS_CPU_Dp_out[4].tri6_pn(_PUSY_FF46_RDp_t0, dma_reg.NYGY_DMA_A12n.qp08_new());
    /* p04.RALY*/ BUS_CPU_Dp_out[5].tri6_pn(_PUSY_FF46_RDp_t0, dma_reg.PULA_DMA_A13n.qp08_new());
    /* p04.RESU*/ BUS_CPU_Dp_out[6].tri6_pn(_PUSY_FF46_RDp_t0, dma_reg.POKU_DMA_A14n.qp08_new());
    /* p04.NUVY*/ BUS_CPU_Dp_out[7].tri6_pn(_PUSY_FF46_RDp_t0, dma_reg.MARU_DMA_A15n.qp08_new());
  }

  {
    /* FF47 BGP */
    /*#p36.RARO*/ BUS_CPU_Dp_out[0].tri6_nn(_TEPY_FF47_RDn_t0, pix_pipe.PAVO_BGP_D0n.qp08_new());
    /* p36.PABA*/ BUS_CPU_Dp_out[1].tri6_nn(_TEPY_FF47_RDn_t0, pix_pipe.NUSY_BGP_D1n.qp08_new());
    /* p36.REDO*/ BUS_CPU_Dp_out[2].tri6_nn(_TEPY_FF47_RDn_t0, pix_pipe.PYLU_BGP_D2n.qp08_new());
    /* p36.LOBE*/ BUS_CPU_Dp_out[3].tri6_nn(_TEPY_FF47_RDn_t0, pix_pipe.MAXY_BGP_D3n.qp08_new());
    /* p36.LACE*/ BUS_CPU_Dp_out[4].tri6_nn(_TEPY_FF47_RDn_t0, pix_pipe.MUKE_BGP_D4n.qp08_new());
    /* p36.LYKA*/ BUS_CPU_Dp_out[5].tri6_nn(_TEPY_FF47_RDn_t0, pix_pipe.MORU_BGP_D5n.qp08_new());
    /* p36.LODY*/ BUS_CPU_Dp_out[6].tri6_nn(_TEPY_FF47_RDn_t0, pix_pipe.MOGY_BGP_D6n.qp08_new());
    /* p36.LARY*/ BUS_CPU_Dp_out[7].tri6_nn(_TEPY_FF47_RDn_t0, pix_pipe.MENA_BGP_D7n.qp08_new());
  }

  {
    /* FF48 OBP0 */
    /*#p36.XARY*/ BUS_CPU_Dp_out[0].tri6_nn(_XOZY_FF48_RDn_t0, pix_pipe.XUFU_OBP0_D0n.qp08_new());
    /* p36.XOKE*/ BUS_CPU_Dp_out[1].tri6_nn(_XOZY_FF48_RDn_t0, pix_pipe.XUKY_OBP0_D1n.qp08_new());
    /* p36.XUNO*/ BUS_CPU_Dp_out[2].tri6_nn(_XOZY_FF48_RDn_t0, pix_pipe.XOVA_OBP0_D2n.qp08_new());
    /* p36.XUBY*/ BUS_CPU_Dp_out[3].tri6_nn(_XOZY_FF48_RDn_t0, pix_pipe.XALO_OBP0_D3n.qp08_new());
    /* p36.XAJU*/ BUS_CPU_Dp_out[4].tri6_nn(_XOZY_FF48_RDn_t0, pix_pipe.XERU_OBP0_D4n.qp08_new());
    /* p36.XOBO*/ BUS_CPU_Dp_out[5].tri6_nn(_XOZY_FF48_RDn_t0, pix_pipe.XYZE_OBP0_D5n.qp08_new());
    /* p36.XAXA*/ BUS_CPU_Dp_out[6].tri6_nn(_XOZY_FF48_RDn_t0, pix_pipe.XUPO_OBP0_D6n.qp08_new());
    /* p36.XAWO*/ BUS_CPU_Dp_out[7].tri6_nn(_XOZY_FF48_RDn_t0, pix_pipe.XANA_OBP0_D7n.qp08_new());
  }

  {
    /* FF49 OBP1 */
    /*#p36.LAJU*/ BUS_CPU_Dp_out[0].tri6_nn(_LOTE_FF49_RDn_t0, pix_pipe.MOXY_OBP1_D0n.qp08_new());
    /* p36.LEPA*/ BUS_CPU_Dp_out[1].tri6_nn(_LOTE_FF49_RDn_t0, pix_pipe.LAWO_OBP1_D1n.qp08_new());
    /* p36.LODE*/ BUS_CPU_Dp_out[2].tri6_nn(_LOTE_FF49_RDn_t0, pix_pipe.MOSA_OBP1_D2n.qp08_new());
    /* p36.LYZA*/ BUS_CPU_Dp_out[3].tri6_nn(_LOTE_FF49_RDn_t0, pix_pipe.LOSE_OBP1_D3n.qp08_new());
    /* p36.LUKY*/ BUS_CPU_Dp_out[4].tri6_nn(_LOTE_FF49_RDn_t0, pix_pipe.LUNE_OBP1_D4n.qp08_new());
    /* p36.LUGA*/ BUS_CPU_Dp_out[5].tri6_nn(_LOTE_FF49_RDn_t0, pix_pipe.LUGU_OBP1_D5n.qp08_new());
    /* p36.LEBA*/ BUS_CPU_Dp_out[6].tri6_nn(_LOTE_FF49_RDn_t0, pix_pipe.LEPU_OBP1_D6n.qp08_new());
    /* p36.LELU*/ BUS_CPU_Dp_out[7].tri6_nn(_LOTE_FF49_RDn_t0, pix_pipe.LUXO_OBP1_D7n.qp08_new());
  }

  {
    /* FF4A WY */
    /*#p23.PUNU*/ BUS_CPU_Dp_out[0].tri6_nn(_VOMY_FF4A_RDn_t0, pix_pipe.NESO_WY0n.qp09_new());
    /* p23.PODA*/ BUS_CPU_Dp_out[1].tri6_nn(_VOMY_FF4A_RDn_t0, pix_pipe.NYRO_WY1n.qp09_new());
    /* p23.PYGU*/ BUS_CPU_Dp_out[2].tri6_nn(_VOMY_FF4A_RDn_t0, pix_pipe.NAGA_WY2n.qp09_new());
    /* p23.LOKA*/ BUS_CPU_Dp_out[3].tri6_nn(_VOMY_FF4A_RDn_t0, pix_pipe.MELA_WY3n.qp09_new());
    /* p23.MEGA*/ BUS_CPU_Dp_out[4].tri6_nn(_VOMY_FF4A_RDn_t0, pix_pipe.NULO_WY4n.qp09_new());
    /* p23.PELA*/ BUS_CPU_Dp_out[5].tri6_nn(_VOMY_FF4A_RDn_t0, pix_pipe.NENE_WY5n.qp09_new());
    /* p23.POLO*/ BUS_CPU_Dp_out[6].tri6_nn(_VOMY_FF4A_RDn_t0, pix_pipe.NUKA_WY6n.qp09_new());
    /* p23.MERA*/ BUS_CPU_Dp_out[7].tri6_nn(_VOMY_FF4A_RDn_t0, pix_pipe.NAFU_WY7n.qp09_new());
  }

  {
    /* FF4B WX */
    /*#p23.LOVA*/ BUS_CPU_Dp_out[0].tri6_nn(_VYCU_FF4B_RDn_t0, pix_pipe.MYPA_WX0n.qp09_new());
    /* p23.MUKA*/ BUS_CPU_Dp_out[1].tri6_nn(_VYCU_FF4B_RDn_t0, pix_pipe.NOFE_WX1n.qp09_new());
    /* p23.MOKO*/ BUS_CPU_Dp_out[2].tri6_nn(_VYCU_FF4B_RDn_t0, pix_pipe.NOKE_WX2n.qp09_new());
    /* p23.LOLE*/ BUS_CPU_Dp_out[3].tri6_nn(_VYCU_FF4B_RDn_t0, pix_pipe.MEBY_WX3n.qp09_new());
    /* p23.MELE*/ BUS_CPU_Dp_out[4].tri6_nn(_VYCU_FF4B_RDn_t0, pix_pipe.MYPU_WX4n.qp09_new());
    /* p23.MUFE*/ BUS_CPU_Dp_out[5].tri6_nn(_VYCU_FF4B_RDn_t0, pix_pipe.MYCE_WX5n.qp09_new());
    /* p23.MULY*/ BUS_CPU_Dp_out[6].tri6_nn(_VYCU_FF4B_RDn_t0, pix_pipe.MUVO_WX6n.qp09_new());
    /* p23.MARA*/ BUS_CPU_Dp_out[7].tri6_nn(_VYCU_FF4B_RDn_t0, pix_pipe.NUKU_WX7n.qp09_new());
  }

  {
    /* FF50 BOOT */
    /* p07.SYPU*/ BUS_CPU_Dp_out[0].tri6_pn(_TEXE_FF50_RDp_t0, BOOT_BITn.qp17_new());
  }

  {
    /* FFFF IE */ // This is technically in the CPU, but we're going to implement it here for now.
    BUS_CPU_Dp_out[0].tri6_nn(FFFF_RDn, IE_D0.qn_new());
    BUS_CPU_Dp_out[1].tri6_nn(FFFF_RDn, IE_D1.qn_new());
    BUS_CPU_Dp_out[2].tri6_nn(FFFF_RDn, IE_D2.qn_new());
    BUS_CPU_Dp_out[3].tri6_nn(FFFF_RDn, IE_D3.qn_new());
    BUS_CPU_Dp_out[4].tri6_nn(FFFF_RDn, IE_D4.qn_new());
  }

  {
    /* EBL -> CBD */
    /* p08.LAVO*/ wire _LAVO_HOLDn = nand3(PIN_CPU_RDp_t0.qp(), _TEXO_8000_9FFFn_t0, PIN_CPU_LATCH_EXT_t0.qp());
    /*#p08.SOMA*/ ext_bus.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[0].qn());
    /* p08.RONY*/ ext_bus.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[1].qn());
    /* p08.RAXY*/ ext_bus.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[2].qn());
    /* p08.SELO*/ ext_bus.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[3].qn());
    /* p08.SODY*/ ext_bus.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[4].qn());
    /* p08.SAGO*/ ext_bus.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[5].qn());
    /* p08.RUPA*/ ext_bus.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[6].qn());
    /* p08.SAZY*/ ext_bus.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn, PIN_EXT_D_in[7].qn());

    /*#p08.RYMA*/ BUS_CPU_Dp_out[0].tri6_nn(_LAVO_HOLDn, ext_bus.SOMA_EXT_DATA_LATCH_D0n.qp08_new());
    /* p08.RUVO*/ BUS_CPU_Dp_out[1].tri6_nn(_LAVO_HOLDn, ext_bus.RONY_EXT_DATA_LATCH_D1n.qp08_new());
    /* p08.RYKO*/ BUS_CPU_Dp_out[2].tri6_nn(_LAVO_HOLDn, ext_bus.RAXY_EXT_DATA_LATCH_D2n.qp08_new());
    /* p08.TAVO*/ BUS_CPU_Dp_out[3].tri6_nn(_LAVO_HOLDn, ext_bus.SELO_EXT_DATA_LATCH_D3n.qp08_new());
    /* p08.TEPE*/ BUS_CPU_Dp_out[4].tri6_nn(_LAVO_HOLDn, ext_bus.SODY_EXT_DATA_LATCH_D4n.qp08_new());
    /* p08.SAFO*/ BUS_CPU_Dp_out[5].tri6_nn(_LAVO_HOLDn, ext_bus.SAGO_EXT_DATA_LATCH_D5n.qp08_new());
    /* p08.SEVU*/ BUS_CPU_Dp_out[6].tri6_nn(_LAVO_HOLDn, ext_bus.RUPA_EXT_DATA_LATCH_D6n.qp08_new());
    /* p08.TAJU*/ BUS_CPU_Dp_out[7].tri6_nn(_LAVO_HOLDn, ext_bus.SAZY_EXT_DATA_LATCH_D7n.qp08_new());
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
    BUS_CPU_Dp_out[0].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x01));
    BUS_CPU_Dp_out[1].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x02));
    BUS_CPU_Dp_out[2].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x04));
    BUS_CPU_Dp_out[3].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x08));
    BUS_CPU_Dp_out[4].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x10));
    BUS_CPU_Dp_out[5].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x20));
    BUS_CPU_Dp_out[6].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x40));
    BUS_CPU_Dp_out[7].tri6_pn(_ZERY_BOOT_CSp, !bool(bootrom_data & 0x80));
  }

  /* VBD  -> CBD */
  {
    /* p25.RERY*/ wire _RERY_VBUS_D0n = not1(BUS_VRAM_Dp_in[0].to_wire_new());
    /* p25.RUNA*/ wire _RUNA_VBUS_D1n = not1(BUS_VRAM_Dp_in[1].to_wire_new());
    /* p25.RONA*/ wire _RONA_VBUS_D2n = not1(BUS_VRAM_Dp_in[2].to_wire_new());
    /* p25.RUNO*/ wire _RUNO_VBUS_D3n = not1(BUS_VRAM_Dp_in[3].to_wire_new());
    /* p25.SANA*/ wire _SANA_VBUS_D4n = not1(BUS_VRAM_Dp_in[4].to_wire_new());
    /* p25.RORO*/ wire _RORO_VBUS_D5n = not1(BUS_VRAM_Dp_in[5].to_wire_new());
    /* p25.RABO*/ wire _RABO_VBUS_D6n = not1(BUS_VRAM_Dp_in[6].to_wire_new());
    /* p25.SAME*/ wire _SAME_VBUS_D7n = not1(BUS_VRAM_Dp_in[7].to_wire_new());

    // Ignoring debug for now
    /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp_t1 = and2(_SOSE_8000_9FFFp_t0, _ABUZ_xxCDEFGH_t1);
    ///*#p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not1(vram_bus.PIN_VRAM_CSn.qn());
    /*#p25.TOLE*/ wire _TOLE_VRAM_RDp_t1     = /*mux2p(_TEFY_VRAM_MCSp, _TUTO_DBG_VRAMp, */ _TUCA_CPU_VRAM_RDp_t1 /*)*/;
    /*#p25.ROPY*/ wire _ROPY_RENDERINGn_t2 = not1(pix_pipe.XYMU_RENDERINGn.qn03_new());
    /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp_t2 = and2(_TOLE_VRAM_RDp_t1, _ROPY_RENDERINGn_t2);
    /* p04.DECY*/ wire _DECY_LATCH_EXTn_t0 = not1(PIN_CPU_LATCH_EXT_t0.qp());
    /* p04.CATY*/ wire _CATY_LATCH_EXTp_t0 = not1(_DECY_LATCH_EXTn_t0);
    /* p28.MYNU*/ wire _MYNU_CPU_RDn_t0    = nand2(_ASOT_CPU_RDp_t0, _CATY_LATCH_EXTp_t0);
    /* p28.LEKO*/ wire _LEKO_CPU_RDp_t0    = not1(_MYNU_CPU_RDn_t0);
    /* p25.TYVY*/ wire _TYVY_VBD_TO_CBDn = nand2(_SERE_CPU_VRM_RDp_t2, _LEKO_CPU_RDp_t0);
    /* p25.SEBY*/ wire _SEBY_VBD_TO_CBDp = not1(_TYVY_VBD_TO_CBDn);
    /*#p25.RUGA*/ BUS_CPU_Dp_out[0].tri6_pn(_SEBY_VBD_TO_CBDp, _RERY_VBUS_D0n);
    /* p25.ROTA*/ BUS_CPU_Dp_out[1].tri6_pn(_SEBY_VBD_TO_CBDp, _RUNA_VBUS_D1n);
    /* p25.RYBU*/ BUS_CPU_Dp_out[2].tri6_pn(_SEBY_VBD_TO_CBDp, _RONA_VBUS_D2n);
    /* p25.RAJU*/ BUS_CPU_Dp_out[3].tri6_pn(_SEBY_VBD_TO_CBDp, _RUNO_VBUS_D3n);
    /* p25.TYJA*/ BUS_CPU_Dp_out[4].tri6_pn(_SEBY_VBD_TO_CBDp, _SANA_VBUS_D4n);
    /* p25.REXU*/ BUS_CPU_Dp_out[5].tri6_pn(_SEBY_VBD_TO_CBDp, _RORO_VBUS_D5n);
    /* p25.RUPY*/ BUS_CPU_Dp_out[6].tri6_pn(_SEBY_VBD_TO_CBDp, _RABO_VBUS_D6n);
    /* p25.TOKU*/ BUS_CPU_Dp_out[7].tri6_pn(_SEBY_VBD_TO_CBDp, _SAME_VBUS_D7n);
  }

  {
    // ZRAM control signals are

    // clk_reg.PIN_CPU_BUKE_AxxxxxGH
    // TEDO_CPU_RDp();
    // TAPU_CPU_WRp_xxxxEFGx()
    // _SYKE_FF00_FFFFp
    // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

    if ((cpu_bus_addr >= 0xFF80) && (cpu_bus_addr <= 0xFFFE)) {
      if (_TAPU_CPU_WRp_xxxxEFGx_t1) {
        zero_ram[cpu_bus_addr & 0x007F] = pack_u8p_new(8, &BUS_CPU_Dp_in[0]);
      }

      BUS_CPU_Dp_out[0].tri(_TEDO_CPU_RDp_t0, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x01));
      BUS_CPU_Dp_out[1].tri(_TEDO_CPU_RDp_t0, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x02));
      BUS_CPU_Dp_out[2].tri(_TEDO_CPU_RDp_t0, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x04));
      BUS_CPU_Dp_out[3].tri(_TEDO_CPU_RDp_t0, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x08));
      BUS_CPU_Dp_out[4].tri(_TEDO_CPU_RDp_t0, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x10));
      BUS_CPU_Dp_out[5].tri(_TEDO_CPU_RDp_t0, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x20));
      BUS_CPU_Dp_out[6].tri(_TEDO_CPU_RDp_t0, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x40));
      BUS_CPU_Dp_out[7].tri(_TEDO_CPU_RDp_t0, wire(zero_ram[cpu_bus_addr & 0x007F] & 0x80));
    }
  }

  if (DELTA_GH) {
    cpu_data_latch = pack_u8p_new(8, &BUS_CPU_Dp_out[0]);
  }
#pragma endregion

  // Sanity check - can't drive the cpu bus in both directions at once.
  CHECK_N(BUS_CPU_Dp_in[0].bit_driven && BUS_CPU_Dp_out[0].bit_driven);
  CHECK_N(BUS_CPU_Dp_in[1].bit_driven && BUS_CPU_Dp_out[1].bit_driven);
  CHECK_N(BUS_CPU_Dp_in[2].bit_driven && BUS_CPU_Dp_out[2].bit_driven);
  CHECK_N(BUS_CPU_Dp_in[3].bit_driven && BUS_CPU_Dp_out[3].bit_driven);
  CHECK_N(BUS_CPU_Dp_in[4].bit_driven && BUS_CPU_Dp_out[4].bit_driven);
  CHECK_N(BUS_CPU_Dp_in[5].bit_driven && BUS_CPU_Dp_out[5].bit_driven);
  CHECK_N(BUS_CPU_Dp_in[6].bit_driven && BUS_CPU_Dp_out[6].bit_driven);
  CHECK_N(BUS_CPU_Dp_in[7].bit_driven && BUS_CPU_Dp_out[7].bit_driven);

  CHECK_N(BUS_VRAM_Dp_in[0].bit_driven && BUS_VRAM_Dp_out[0].bit_driven);
  CHECK_N(BUS_VRAM_Dp_in[1].bit_driven && BUS_VRAM_Dp_out[1].bit_driven);
  CHECK_N(BUS_VRAM_Dp_in[2].bit_driven && BUS_VRAM_Dp_out[2].bit_driven);
  CHECK_N(BUS_VRAM_Dp_in[3].bit_driven && BUS_VRAM_Dp_out[3].bit_driven);
  CHECK_N(BUS_VRAM_Dp_in[4].bit_driven && BUS_VRAM_Dp_out[4].bit_driven);
  CHECK_N(BUS_VRAM_Dp_in[5].bit_driven && BUS_VRAM_Dp_out[5].bit_driven);
  CHECK_N(BUS_VRAM_Dp_in[6].bit_driven && BUS_VRAM_Dp_out[6].bit_driven);
  CHECK_N(BUS_VRAM_Dp_in[7].bit_driven && BUS_VRAM_Dp_out[7].bit_driven);
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
