#include "GateBoyLib/GateBoy.h"
#include <memory.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h"
#include "CoreLib/File.h"
#include "GateBoyLib/Probe.h"

//------------------------------------------------------------------------------

void GateBoy::dump(Dumper& d) const {
  d("\002===== GateBoy =====\001\n");
  d("screen_x       %3d\n", screen_x);
  d("screen_y       %3d\n", screen_y);
  d("lcd_data_latch %3d\n", lcd_data_latch);
  d("lcd_pix_lo     %c\n",  top.lcd_pix_lo.c());
  d("lcd_pix_hi     %c\n",  top.lcd_pix_hi.c());

  d("lcd_pipe_lo    %c%c%c%c%c%c%c%c\n",
    top.lcd_pipe_lo[0].c(), top.lcd_pipe_lo[1].c(), top.lcd_pipe_lo[2].c(), top.lcd_pipe_lo[3].c(),
    top.lcd_pipe_lo[4].c(), top.lcd_pipe_lo[5].c(), top.lcd_pipe_lo[6].c(), top.lcd_pipe_lo[7].c());
  d("lcd_pipe_hi    %c%c%c%c%c%c%c%c\n",
    top.lcd_pipe_hi[0].c(), top.lcd_pipe_hi[1].c(), top.lcd_pipe_hi[2].c(), top.lcd_pipe_hi[3].c(),
    top.lcd_pipe_hi[4].c(), top.lcd_pipe_hi[5].c(), top.lcd_pipe_hi[6].c(), top.lcd_pipe_hi[7].c());
}

//------------------------------------------------------------------------------

void GateBoy::load_post_bootrom_state() {
  load_obj("gateboy_post_bootrom.raw.dump", *this);
  check_sentinel();
  check_div();
  rom_buf = nullptr;
  rom_size = 0;
}

void GateBoy::set_rom(uint8_t* _rom_buf, size_t _rom_size) {
  rom_buf = _rom_buf;
  rom_size = _rom_size;
}

//------------------------------------------------------------------------------

void GateBoy::reset_cart() {
  sentinel1 = 0xdeadbeefbaadf00d;

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

  top.reset_cart();

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

  screen_x = 0;
  screen_y = 152;
  lcd_data_latch = 0;

  rom_buf = nullptr;
  rom_size = 0;
  sentinel2 = 0xf00dcafebaadc0de;
}

//-----------------------------------------------------------------------------

void GateBoy::reset_boot() {
  sentinel1 = 0xDEADBEEFBAADF00D;

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

  top.reset_boot();

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

  memset(vid_ram,  0, 8192);
  memset(cart_ram, 0, 8192);
  memset(ext_ram,  0, 8192);
  memset(oam_ram,  0, 256);
  memset(zero_ram, 0, 128);
  memset(framebuffer, 4, 160*144);

  rom_buf = nullptr;
  rom_size = 0;
  sentinel2 = 0xF00DCAFEBAADC0DE;
}

//------------------------------------------------------------------------------

void GateBoy::run_reset_sequence() {

  //----------------------------------------

  CHECK_P(rom_buf != nullptr);
  CHECK_P(rom_size);

  // In reset
  sys_rst = 1;
  run(5);

  // Out of reset
  // Start clock and sync with phase
  sys_rst = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  run(3);

  CHECK_P(top.clk_reg.AFUR_xxxxEFGH.qn08());
  CHECK_P(top.clk_reg.ALEF_AxxxxFGH.qp09());
  CHECK_P(top.clk_reg.APUK_ABxxxxGH.qp09());
  CHECK_P(top.clk_reg.ADYK_ABCxxxxH.qp09());

  // Wait for PIN_CPU_START
  while(!top.cpu_bus.PIN_CPU_STARTp.qp()) {
    run(8);
  }

  // Delay to sync w/ expected div value after bootrom
  run(8);
  run(8);

  // Done, initialize bus with whatever the CPU wants.
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

void GateBoy::next_pass() {

  probes.begin_pass(phase_total);

  //----------------------------------------

  if (pass_count == 0) {
    probes.begin_phase();

    cpu_data_latch = top.cpu_bus.get_bus_data();
    imask_latch = (uint8_t)pack_p(top.IE_D0.qp(), top.IE_D1.qp(), top.IE_D2.qp(), top.IE_D3.qp(), top.IE_D4.qp(), 0, 0, 0);

    if (DELTA_DE) int_vblank_halt = top.int_reg.PIN_CPU_INT_VBLANK.qp();
    if (DELTA_DE) int_stat_halt   = top.int_reg.PIN_CPU_INT_STAT.qp();
    if (DELTA_DE) int_serial_halt = top.int_reg.PIN_CPU_INT_SERIAL.qp();
    if (DELTA_DE) int_joypad_halt = top.int_reg.PIN_CPU_INT_JOYPAD.qp();

    if (DELTA_HA) int_timer_halt  = top.int_reg.PIN_CPU_INT_TIMER.qp(); // this one latches funny on HA, some hardware bug

    if (DELTA_HA) int_vblank = top.int_reg.PIN_CPU_INT_VBLANK.qp();
    if (DELTA_HA) int_stat   = top.int_reg.PIN_CPU_INT_STAT.qp();
    if (DELTA_HA) int_timer  = top.int_reg.PIN_CPU_INT_TIMER.qp();
    if (DELTA_HA) int_serial = top.int_reg.PIN_CPU_INT_SERIAL.qp();
    if (DELTA_HA) int_joypad = top.int_reg.PIN_CPU_INT_JOYPAD.qp();

    if (DELTA_AB) {
      cpu_req = cpu.bus_req;
      bus_req = {0};

      if (sys_cpu_en) bus_req = cpu_req;
      if (dbg_req.read || dbg_req.write) bus_req = dbg_req;

      top.cpu_bus.PIN_CPU_RDp.lock(bus_req.read);
      top.cpu_bus.PIN_CPU_WRp.lock(bus_req.write);

      bool addr_ext = (bus_req.read || bus_req.write) && (bus_req.addr < 0xFE00);
      if (bus_req.addr <= 0x00FF && top.cpu_bus.PIN_CPU_BOOTp.qp()) addr_ext = false;
      top.cpu_bus.PIN_CPU_EXT_BUSp.lock(addr_ext);

      top.int_reg.PIN_CPU_ACK_VBLANK.lock(wire(cpu.int_ack & INT_VBLANK_MASK));
      top.int_reg.PIN_CPU_ACK_STAT  .lock(wire(cpu.int_ack & INT_STAT_MASK));
      top.int_reg.PIN_CPU_ACK_TIMER .lock(wire(cpu.int_ack & INT_TIMER_MASK));
      top.int_reg.PIN_CPU_ACK_SERIAL.lock(wire(cpu.int_ack & INT_SERIAL_MASK));
      top.int_reg.PIN_CPU_ACK_JOYPAD.lock(wire(cpu.int_ack & INT_JOYPAD_MASK));

      top.cpu_bus.BUS_CPU_A00.lock(wire(bus_req.addr & 0x0001));
      top.cpu_bus.BUS_CPU_A01.lock(wire(bus_req.addr & 0x0002));
      top.cpu_bus.BUS_CPU_A02.lock(wire(bus_req.addr & 0x0004));
      top.cpu_bus.BUS_CPU_A03.lock(wire(bus_req.addr & 0x0008));
      top.cpu_bus.BUS_CPU_A04.lock(wire(bus_req.addr & 0x0010));
      top.cpu_bus.BUS_CPU_A05.lock(wire(bus_req.addr & 0x0020));
      top.cpu_bus.BUS_CPU_A06.lock(wire(bus_req.addr & 0x0040));
      top.cpu_bus.BUS_CPU_A07.lock(wire(bus_req.addr & 0x0080));
      top.cpu_bus.BUS_CPU_A08.lock(wire(bus_req.addr & 0x0100));
      top.cpu_bus.BUS_CPU_A09.lock(wire(bus_req.addr & 0x0200));
      top.cpu_bus.BUS_CPU_A10.lock(wire(bus_req.addr & 0x0400));
      top.cpu_bus.BUS_CPU_A11.lock(wire(bus_req.addr & 0x0800));
      top.cpu_bus.BUS_CPU_A12.lock(wire(bus_req.addr & 0x1000));
      top.cpu_bus.BUS_CPU_A13.lock(wire(bus_req.addr & 0x2000));
      top.cpu_bus.BUS_CPU_A14.lock(wire(bus_req.addr & 0x4000));
      top.cpu_bus.BUS_CPU_A15.lock(wire(bus_req.addr & 0x8000));
    }

    if (DELTA_HA) {
      // This seems wrong, but it passes tests. *shrug*
      if (bus_req.addr >= 0x8000 && bus_req.addr <= 0x9FFF) {
        top.cpu_bus.PIN_CPU_EXT_BUSp.lock(0);
      }

      top.cpu_bus.PIN_CPU_RDp.lock(0);
      top.cpu_bus.PIN_CPU_WRp.lock(0);
      top.cpu_bus.BUS_CPU_A08.lock(0);
      top.cpu_bus.BUS_CPU_A09.lock(0);
      top.cpu_bus.BUS_CPU_A10.lock(0);
      top.cpu_bus.BUS_CPU_A11.lock(0);
      top.cpu_bus.BUS_CPU_A12.lock(0);
      top.cpu_bus.BUS_CPU_A13.lock(0);
      top.cpu_bus.BUS_CPU_A14.lock(0);
      top.cpu_bus.BUS_CPU_A15.lock(0);
    }

    top.cpu_bus.PIN_CPU_LATCH_EXT.lock(0);
    top.cpu_bus.unlock_data();

    // not at all certain about this. seems to break some oam read glitches.
    if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      if (bus_req.read && (bus_req.addr < 0xFF00)) top.cpu_bus.PIN_CPU_LATCH_EXT.lock(1);
    }

    // Data has to be driven on EFGH or we fail the wave tests
    if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      if (bus_req.write) top.cpu_bus.lock_data(bus_req.data_lo);
    }

    top.joypad.set_buttons(sys_buttons);
  }

  //----------------------------------------
  // Run one pass of our simulation.

  uint64_t pass_hash_old = pass_hash;

  RegBase::bus_collision = false;
  RegBase::bus_floating = false;

  wire CLK = !(phase_total & 1) & sys_clken;

  RegBase::sim_running = true;
  RegBase::tick_running = true;
  top.tick_slow(sys_rst, CLK, sys_clkgood, sys_t1, sys_t2, sys_cpuready);
  RegBase::tick_running = false;

  RegBase::tock_running = true;
  top.tock_slow(sys_rst, CLK, sys_clkgood, sys_t1, sys_t2, sys_cpuready);
  top.tock_ext_bus (sys_rst, rom_buf, cart_ram, ext_ram);
  top.tock_vram_bus(sys_rst, vid_ram);
  top.tock_zram_bus(sys_rst, zero_ram);
  top.tock_oam_bus (sys_rst, oam_ram);
  top.ser_reg.set_pins(DELTA_TRIZ, DELTA_TRIZ);
  RegBase::tock_running = false;
  RegBase::sim_running = false;

  uint64_t pass_hash_new = commit_and_hash(top);

  sim_stable = pass_hash_old == pass_hash_new;
  pass_hash = pass_hash_new;
  pass_count++;

  if (pass_count > 90) printf("!!!STUCK!!!\n");

  //----------

  probe(1, "PIN_CPU_RDp",       top.cpu_bus.PIN_CPU_RDp.qp());
  probe(2, "PIN_CPU_WRp",       top.cpu_bus.PIN_CPU_WRp.qp());
  probe(3, "SADU_STAT_MODE0",      !top.pix_pipe.SADU_STAT_MODE0n);
  probe(4, "XATY_STAT_MODE1",      !top.pix_pipe.XATY_STAT_MODE1n);
  //probe(5, "ROPO_LY_MATCH_SYNCp",  top.lcd_reg.ROPO_LY_MATCH_SYNCp.qp17());
  //probe(3, "RUPO_LYC_MATCH_LATCH", top.pix_pipe.RUPO_LYC_MATCH_LATCHn.qn03());

  //probe( 6, "addr_rom",          bus_req.addr <= 0x7FFF);
  //probe( 7, "addr_ram",          bus_req.addr >= 0xA000 && bus_req.addr <= 0xFDFF);
  //probe(10, "PIN_CPU_ADDR_EXTp", top.cpu_bus.PIN_CPU_EXT_BUSp.qp());
  //probe(11, "PIN_CPU_LATCH_EXT", top.cpu_bus.PIN_CPU_LATCH_EXT.qp());
  //probe(13, "ABUZ_xxCDEFGH",     top.ABUZ_xxCDEFGH);
  //probe(14, "TEXO_8000_9FFFn",   top.TEXO_ADDR_EXT_AND_NOT_VRAM);

  //probe(16, "PIN_VRAM_CSp",      !top.vram_bus.PIN_VRAM_CSn.qp());
  //probe(17, "PIN_VRAM_OEp",      !top.vram_bus.PIN_VRAM_OEn.qp());
  //probe(18, "PIN_VRAM_WRp",      !top.vram_bus.PIN_VRAM_WRn.qp());


  //probe( 1, "PIN_CPU_RDp", top.cpu_bus.PIN_CPU_RDp.qp());
  //probe( 2, "PIN_CPU_WRp", top.cpu_bus.PIN_CPU_WRp.qp());

  //probe( 3, "XEHO_X0p",    top.pix_pipe.XEHO_X0p.qn16());
  //probe( 4, "SAVY_X1p",    top.pix_pipe.SAVY_X1p.qn16());
  //probe( 5, "XODU_X2p",    top.pix_pipe.XODU_X2p.qn16());

  //probe(10, "PIN_LCD_CLOCK", top.PIN_LCD_CLOCK.qp());
  //probe(11, "PIN_LCD_HSYNC", top.PIN_LCD_HSYNC.qp());
  //probe(12, "PIN_LCD_VSYNC", top.PIN_LCD_VSYNC.qp());
  //probe(13, "PIN_LCD_DATA0", top.PIN_LCD_DATA0.qp());
  //probe(14, "PIN_LCD_DATA1", top.PIN_LCD_DATA1.qp());
  //probe(15, "PIN_LCD_LATCH", top.PIN_LCD_LATCH.qp());

  //probe(8, "PIN_EXT_RDp", !top.ext_bus.PIN_EXT_RDn.qp());
  //probe(9, "PIN_EXT_WRp", !top.ext_bus.PIN_EXT_WRn.qp());

  //probe(16, "BUS_CPU_D0p", top.cpu_bus.BUS_CPU_D0p.c());
  //probe(17, "BUS_CPU_D0p", top.cpu_bus.BUS_CPU_D1p.c());
  //probe(18, "BUS_CPU_D0p", top.cpu_bus.BUS_CPU_D2p.c());
  //probe(19, "BUS_CPU_D0p", top.cpu_bus.BUS_CPU_D3p.c());
  //probe(20, "BUS_CPU_D0p", top.cpu_bus.BUS_CPU_D4p.c());
  //probe(21, "BUS_CPU_D0p", top.cpu_bus.BUS_CPU_D5p.c());
  //probe(22, "BUS_CPU_D0p", top.cpu_bus.BUS_CPU_D6p.c());
  //probe(23, "BUS_CPU_D0p", top.cpu_bus.BUS_CPU_D7p.c());

  probe(10, "PIN_OAM_CLK",  top.oam_bus.PIN_OAM_CLK.qp());
  probe(11, "PIN_OAM_OE",   top.oam_bus.PIN_OAM_OE.qp());
  probe(12, "PIN_OAM_WR_A", top.oam_bus.PIN_OAM_WR_A.qp());
  probe(13, "PIN_OAM_WR_B", top.oam_bus.PIN_OAM_WR_B.qp());

  //----------------------------------------

  //probe( 2, "CUPA_CPU_WRp",          CUPA_CPU_WRp_xxxxEFGx);
  //probe( 5, "BESU_SCANNINGp",        top.sprite_scanner.BESU_SCANNINGp.qp());
  //probe( 6, "XYMU_RENDERINGp",       top.pix_pipe.XYMU_RENDERINGp.qp());
  //probe( 7, "MATU_DMA_RUNNINGp",     top.dma_reg.MATU_DMA_RUNNINGp.qp());
  //probe(10, "TEDO_CPU_RDp",          TEDO_CPU_RDp);
  //probe(11, "CUPA_CPU_WRp_xxxxEFGx", CUPA_CPU_WRp_xxxxEFGx);
  //probe(12, "SARO_FE00_FEFFp",       SARO_FE00_FEFFp);
  //probe(13, "AJON_PPU_OAM_ENp",      AJON_PPU_OAM_ENp);
  //probe(14, "AMAB_CPU_OAM_ENp",      AMAB_CPU_OAM_ENp);
  //probe(20, "PIN_OAM_CLKp",          !COTA_OAM_CLKn);
  //probe(21, "PIN_OAM_OEp",           !ZODO_OAM_OEn);
  //probe(22, "PIN_OAM_WR_Ap",         !ZOFE);
  //probe(23, "PIN_OAM_WR_Bp",         !ZONE);
  //probe(11, "GUKO_OBL_TO_CBDp",      GUKO_OBL_TO_CBDp);
  //probe(12, "WUKU_OBL_TO_CBDp",      WUKU_OBL_TO_CBDp);

  //probe(16, "RUTU_LINE_P910", top.lcd_reg.RUTU_LINE_P910.qp());
  //probe(17, "CATU_LINE_P000", top.lcd_reg.CATU_LINE_P000.qp());
  //probe(18, "NYPE_LINE_P002", top.lcd_reg.NYPE_LINE_P002.qp());
  //probe(19, "ANEL_LINE_P002", top.lcd_reg.ANEL_LINE_P002.qp());

  //probe(6, "XYMU_RENDERINGp",      XYMU_RENDERINGn.qn03());
  //probe(7, "PARU_VBLANKp_d4",      top.lcd_reg.PARU_VBLANKp_d4);
  //probe(8, "MATU_DMA_RUNNINGp",    top.dma_reg.MATU_DMA_RUNNINGp.qp());
  //probe(9, "BESU_SCANNINGp",       top.sprite_scanner.BESU_SCANNINGp.qp());



  if (top.PIN_LCD_CLOCK.posedge()) {
    screen_x++;
  }
  if (top.PIN_LCD_HSYNC.qp() || top.PIN_LCD_LATCH.qp()) {
    screen_x = 0;
  }

  if (top.PIN_LCD_LATCH.posedge()) {
    if (screen_y < 144) {
      for (int x = 0; x < 159; x++) {
        uint8_t p0 = top.lcd_pipe_lo[x + 1].qp();
        uint8_t p1 = top.lcd_pipe_hi[x + 1].qp();
        framebuffer[x + screen_y * 160] = p0 + p1 * 2;
      }
      {
        uint8_t p0 = top.lcd_pix_lo.qp04();
        uint8_t p1 = top.lcd_pix_hi.qp04();
        framebuffer[159 + screen_y * 160] = p0 + p1 * 2;
      }
    }

    screen_y++;
    if (top.PIN_LCD_VSYNC.qp()) {
      screen_y = 0;
    }
  }

  //----------------------------------------
  // Once the simulation converges, latch the data that needs to go back to the
  // CPU or test function and update the CPU if necessary.

  if (sim_stable) {

    //----------

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
    // If the sim is stable and there's still a bus collision or float, we have a problem.

    if (RegBase::bus_collision) printf("Bus collision!\n");
    if (RegBase::bus_floating)  printf("Bus floating!\n");

    //----------
    // Done, move to the next phase.

    pass_total += pass_count;
    pass_count = 0;
    phase_total++;
    combine_hash(total_hash, pass_hash);

    probes.end_phase();
  }

  probe(0, "phase", "ABCDEFGH"[phase_total & 7]);
  probes.end_pass(sim_stable);
}

//-----------------------------------------------------------------------------
