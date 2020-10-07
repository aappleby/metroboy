#include "GateBoyLib/GateBoy.h"
#include <memory.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h"
#include "CoreLib/File.h"

//-----------------------------------------------------------------------------

GateBoy::GateBoy() {
  //memset(cart_rom, 0, 32768);
  memset(vid_ram,  0, 8192);
  memset(cart_ram, 0, 8192);
  memset(ext_ram,  0, 8192);
  memset(oam_ram,  0, 256);
  memset(zero_ram, 0, 128);

  memset(framebuffer, 4, 160*144);

  top.cpu_bus.BUS_CPU_A00.lock(0);
  top.cpu_bus.BUS_CPU_A01.lock(0);
  top.cpu_bus.BUS_CPU_A02.lock(0);
  top.cpu_bus.BUS_CPU_A03.lock(0);
  top.cpu_bus.BUS_CPU_A04.lock(0);
  top.cpu_bus.BUS_CPU_A05.lock(0);
  top.cpu_bus.BUS_CPU_A06.lock(0);
  top.cpu_bus.BUS_CPU_A07.lock(0);
  top.cpu_bus.BUS_CPU_A08.lock(0);
  top.cpu_bus.BUS_CPU_A09.lock(0);
  top.cpu_bus.BUS_CPU_A10.lock(0);
  top.cpu_bus.BUS_CPU_A11.lock(0);
  top.cpu_bus.BUS_CPU_A12.lock(0);
  top.cpu_bus.BUS_CPU_A13.lock(0);
  top.cpu_bus.BUS_CPU_A14.lock(0);
  top.cpu_bus.BUS_CPU_A15.lock(0);

  top.cpu_bus.PIN_CPU_RDp.lock(!bus_req.write);
  top.cpu_bus.PIN_CPU_WRp.lock(bus_req.write);
  top.cpu_bus.PIN_CPU_6.lock(0);
  top.cpu_bus.PIN_CPU_ADDR_EXTp.lock(1);
  top.cpu_bus.PIN_CPU_LATCH_EXT.lock(0);

  top.int_reg.PIN_CPU_ACK_VBLANK.lock(0);
  top.int_reg.PIN_CPU_ACK_STAT  .lock(0);
  top.int_reg.PIN_CPU_ACK_TIMER .lock(0);
  top.int_reg.PIN_CPU_ACK_SERIAL.lock(0);
  top.int_reg.PIN_CPU_ACK_JOYPAD.lock(0);

  cpu.reset(0x0000);
}

//-----------------------------------------------------------------------------

void GateBoy::reset_to_bootrom(uint8_t* _rom_buf, size_t _rom_size) {
  rom_buf = _rom_buf;
  rom_size = _rom_size;

  // In reset
  sys_rst = 1;
  run(5);

  // Out of reset
  // Start clock and sync with phase
  sys_rst = 0;
  sys_clken = 1;
  sys_clkgood = 1;
  run(3);

  CHECK_P(top.clk_reg.AFUR_xxxxEFGH.q08n());
  CHECK_P(top.clk_reg.ALEF_AxxxxFGH.q09p());
  CHECK_P(top.clk_reg.APUK_ABxxxxGH.q09p());
  CHECK_P(top.clk_reg.ADYK_ABCxxxxH.q09p());

  // Wait for PIN_CPU_START
  while(!top.cpu_bus.PIN_CPU_STARTp.qp()) {
    run(8);
  }

  // Delay to sync w/ expected div value after bootrom
  run(8);
  run(8);

  // Done, initialize bus with whatever the CPU wants.
  cpu.reset(0x0000);
  sys_cpuready = 1;
  sys_cpu_en = true;
}

//------------------------------------------------------------------------------

void GateBoy::reset_post_bootrom(uint8_t* _rom_buf, size_t _rom_size) {
  load_obj("gateboy_post_bootrom.raw.dump", *this);
  check_sentinel();
  check_div();

  rom_buf = _rom_buf;
  rom_size = _rom_size;
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

  //----------------------------------------

  if (pass_count == 0) {
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

    if (DELTA_HA) {
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

    if (DELTA_AB) {
      cpu_req = cpu.bus_req;
      bus_req = {0};

      if (sys_cpu_en) bus_req = cpu_req;
      if (dbg_req.read || dbg_req.write) bus_req = dbg_req;

      bool addr_rom = bus_req.addr <= 0x7FFF;
      bool addr_ram = bus_req.addr >= 0xA000 && bus_req.addr <= 0xFDFF;
      bool addr_ext = (bus_req.read || bus_req.write) && (addr_rom || addr_ram);
      if (bus_req.addr <= 0x00FF && top.cpu_bus.PIN_CPU_BOOTp.qp()) addr_ext = false;

      top.cpu_bus.PIN_CPU_RDp.lock(bus_req.read);
      top.cpu_bus.PIN_CPU_WRp.lock(bus_req.write);
      top.cpu_bus.PIN_CPU_ADDR_EXTp.lock(addr_ext);
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

    top.cpu_bus.PIN_CPU_LATCH_EXT.lock(0);
    top.cpu_bus.unlock_data();

    if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
      if (bus_req.read && (bus_req.addr < 0xFF00)) top.cpu_bus.PIN_CPU_LATCH_EXT.lock(1);
    }

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
    // Send pixels to the display if necessary.
    // FIXME should probably use the lcd sync pins for this...

    // clock phase is ~119.21 nsec (4.19304 mhz crystal)
    // hsync seems consistently 3.495 - 3.500 us (29 phases?)
    // hsync to bogus clock pulse 1.465 us
    // data changes on rising edge of lcd clock
    // hsync should go low the same phase that lcd clock goes high
    // vsync 108.720 usec - right on 912 phases

    int fb_x = top.pix_pipe.get_pix_count() - 8;
    int fb_y = top.lcd_reg.get_ly();

    if (fb_x >= 0 && fb_x < 160 && fb_y >= 0 && fb_y < 144) {
      int p0 = top.PIN_LCD_DATA0.qp();
      int p1 = top.PIN_LCD_DATA1.qp();
      framebuffer[fb_x + fb_y * 160] = uint8_t(p0 + p1 * 2);
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
