#include "GateBoyLib/GateBoy.h"
#include <memory.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h"
#include "CoreLib/File.h"

//-----------------------------------------------------------------------------

GateBoy::GateBoy() {
  memset(cart_rom, 0, 32768);
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

  cpu.reset(0x0000);
}

//-----------------------------------------------------------------------------

void GateBoy::reset_to_bootrom() {
  // No bus activity during reset
  dbg_req.addr = 0x0000;
  dbg_req.data = 0;
  dbg_req.read = 0;
  dbg_req.write = 0;

  // In reset
  run(8);

  // Out of reset
  sys_rst = 0;
  run(8);

  // Start clock and sync with phase A
  sys_clken = 1;
  while(1) {
    run(1);
    if (top.clk_reg.AFUR_xxxxEFGH.qn() &&
        top.clk_reg.ALEF_AxxxxFGH.qp() &&
        top.clk_reg.APUK_ABxxxxGH.qp() &&
        top.clk_reg.ADYK_ABCxxxxH.qp()) break;
  }
  CHECK_P(top.cpu_bus.PIN_CPU_BOMA_Axxxxxxx.qp());

  // Sync done, reset phase counter to 0
  phase_total = 0;
  pass_count = 0;
  pass_total = 0;

  // Set CLKGOOD
  sys_clkgood = 1;
  run(8);

  // Wait for PIN_CPU_START
  //printf("Waiting for PIN_CPU_START\n");
  while(!top.cpu_bus.PIN_CPU_STARTp.qp()) {
    run(8);
  }
  //printf("Got PIN_CPU_START\n");

  // PIN_CPU_START high, delay 8 phases
  run(8);

  // Set CPU_READY, delay 8 phases
  sys_cpuready = 1;
  run(8);

  // Done, initialize bus with whatever the CPU wants.
  cpu.reset(0x0000);
  cpu.get_bus_req(bus_req);

  // and skip AB so we can latch the first opcode before the cpu starts running
  next_phase();
  sys_cpu_en = true;
  run(7);

  //top.tim_reg.

}

//------------------------------------------------------------------------------

void GateBoy::reset_post_bootrom() {
  load_obj("gateboy_post_bootrom.raw.dump", *this);
  check_sentinel();

  // FIXME
  // LCD has to be at (98,0) before we start the tests, but for some reason
  // right now our bootrom is putting us at (26,0) - run for an additional
  // 72*8 = 576 phases with the CPU off to get back in sync.

  sys_cpu_en = 0;
  run(576);
  sys_cpu_en = 1;

  // Similarly, the bootrom isn't putting us at the right point relative to
  // DIV. If we run bootrom w/o FAST_BOOT, at app start div is 0xDEC2 so
  // we're early by 0xC30 (3120) mcycles

  top.tim_reg.force_set_div(0xEAF2); // this passes poweron_000/4/5_div
}

//------------------------------------------------------------------------------
// I'm guessing we proooobably latch bus data on DE, since that's also
// when we put data on the bus for a write?

uint8_t GateBoy::dbg_read(int addr) {
  CHECK_P((phase_total & 7) == 0);
  
  dbg_req.addr = uint16_t(addr);
  dbg_req.data = 0;
  dbg_req.read = 1;
  dbg_req.write = 0;

  /* AB */ next_phase(); /* xx */
  /* BC */ next_phase(); /* xx */
  /* CD */ next_phase(); /* ok */ 
  /* DE */ next_phase(); /* ok */
  /* EF */ next_phase(); /* ok */ 
  /* FG */ next_phase(); /* ok */ uint8_t bus_data = top.cpu_bus.get_bus_data(); 
  /* GH */ next_phase(); /* ok */ 
  /* HA */ next_phase(); /* xx */
  dbg_req = {0};
  return bus_data;
}

//------------------------------------------------------------------------------

void GateBoy::dbg_write(int addr, uint8_t data) {
  CHECK_P((phase_total & 7) == 0);

  dbg_req.addr = uint16_t(addr);
  dbg_req.data = data;
  dbg_req.read = 0;
  dbg_req.write = 1;

  /* AB */ next_phase();
  /* BC */ next_phase();
  /* CD */ next_phase();
  /* DE */ next_phase();
  /* EF */ next_phase();
  /* FG */ next_phase();
  /* GH */ next_phase();
  /* HA */ next_phase();
  dbg_req = {0};
}

//------------------------------------------------------------------------------

void GateBoy::next_phase() {
  int old_phase = (phase_total + 0) & 7;
  int new_phase = (phase_total + 1) & 7;

  //----------
  // Update CPU

  if (sys_cpu_en) {
    uint8_t imask = 0;
    uint8_t intf = 0;

    if (top.int_reg.PIN_CPU_INT_VBLANK.qp()) intf |= INT_VBLANK_MASK;
    if (top.int_reg.PIN_CPU_INT_STAT.qp())   intf |= INT_STAT_MASK;
    if (top.int_reg.PIN_CPU_INT_TIMER.qp())  intf |= INT_TIMER_MASK;
    if (top.int_reg.PIN_CPU_INT_SERIAL.qp()) intf |= INT_SERIAL_MASK;
    if (top.int_reg.PIN_CPU_INT_JOYPAD.qp()) intf |= INT_JOYPAD_MASK;

    uint8_t bus_data = top.cpu_bus.get_bus_data();

    // tock_ack can't be on EF or FG, see lcdon_to_ly1_b - ly changes to 1 on fg
    // and there's no clock edge going to the cpu on GH, so data must be latched on HA

    switch(old_phase) {
    /* AB */ case 0: cpu.tock_req(imask, intf, bus_data); break; // bus request _must_ change on AB, see trace
    /* BC */ case 1: break;
    /* CD */ case 2: break;
    /* DE */ case 3: break;
    /* EF */ case 4: break;
    /* FG */ case 5: break;
    /* GH */ case 6: break;
    /* HA */ case 7: cpu.tock_ack(imask, intf, bus_data); break;
    }
  }

  //----------
  // Run logic passes

  uint64_t hash_regs_old  = HASH_INIT;
  uint64_t hash_regs_new  = HASH_INIT;

  for (pass_count = 1; pass_count < 100; pass_count++) {
    hash_regs_old = hash_regs_new;
    hash_regs_new  = next_pass(old_phase, new_phase);
    if (hash_regs_new == hash_regs_old) break;
    if (pass_count > 90) {
      printf("!!!STUCK!!!\n");
    }
  }

  if (RegBase::bus_collision) {
    printf("Bus collision!\n");
    RegBase::bus_collision = false;
  }

  if (RegBase::bus_floating) {
    printf("Bus floating!\n");
    RegBase::bus_floating = false;
    //next_pass(old_phase, new_phase);
    //RegBase::bus_floating = false;
  }

  CHECK_P(pass_count < 100);
  pass_total += pass_count;

  //----------
  // Pixels _def_ latched on positive clock edge (neg edge inverted)

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
  // Done

  ack_vblank = 0;
  ack_stat = 0;
  ack_timer = 0;
  ack_serial = 0;
  ack_joypad = 0;

  phase_total++;
  phase_hash = hash_regs_old;
  combine_hash(total_hash, phase_hash);
}

//-----------------------------------------------------------------------------

uint64_t GateBoy::next_pass(int old_phase, int new_phase) {
  RegBase::sim_running = true;
  RegBase::bus_collision = false;
  RegBase::bus_floating = false;

  wire CLK = (new_phase & 1) & sys_clken;
  top.joypad.set_buttons(sys_buttons);

  RegBase::tick_running = true;
  top.tick_slow(sys_rst, CLK, sys_clkgood, sys_t1, sys_t2, sys_cpuready);
  RegBase::tick_running = false;

  //----------
  // CPU bus stuff

  if (DELTA_AB) {
    if (dbg_req.read || dbg_req.write) {
      bus_req.addr  = dbg_req.addr;
      bus_req.data  = dbg_req.data;
      bus_req.read  = dbg_req.read;
      bus_req.write = dbg_req.write;
    }
    else {
      bus_req.addr  = cpu.bus_req.addr;
      bus_req.data  = cpu.bus_req.data;
      bus_req.read  = cpu.bus_req.read;
      bus_req.write = cpu.bus_req.write;
    }

    if (bus_req.read || bus_req.write) {
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

    top.cpu_bus.PIN_CPU_RDp.lock(bus_req.read);
    top.cpu_bus.PIN_CPU_WRp.lock(bus_req.write);

    bool addr_rom = bus_req.addr <= 0x7FFF;
    bool addr_ram = bus_req.addr >= 0xA000 && bus_req.addr <= 0xFDFF;
    bool addr_hi  = bus_req.addr >= 0xFF00 && bus_req.addr <= 0xFFFF;
    addr_hi = false;
    bool addr_ext = (bus_req.read || bus_req.write) && (addr_rom || addr_ram || addr_hi) && !top.cpu_bus.PIN_CPU_BOOTp.qp();
    top.cpu_bus.PIN_CPU_ADDR_EXTp.lock(addr_ext);
  }
  else if (DELTA_BC) {
  }
  else if (DELTA_CD) {
    bool hold_mem = bus_req.read && (bus_req.addr < 0xFF00);
    top.cpu_bus.PIN_CPU_LATCH_EXT.lock(hold_mem);
  }
  else if (DELTA_DE) {
    if (bus_req.write) top.cpu_bus.set_data(bus_req.data_lo);
  }
  else if (DELTA_EF) {
    if (bus_req.write) top.cpu_bus.set_data(bus_req.data_lo);
  }
  else if (DELTA_FG) {
    if (bus_req.write) top.cpu_bus.set_data(bus_req.data_lo);
  }
  else if (DELTA_GH) {
    if (bus_req.write) top.cpu_bus.set_data(bus_req.data_lo);
  }
  else if (DELTA_HA) {
    if ((bus_req.addr & 0xFF00) != 0xFF00) {
      top.cpu_bus.BUS_CPU_A08.lock(0);
      top.cpu_bus.BUS_CPU_A09.lock(0);
      top.cpu_bus.BUS_CPU_A10.lock(0);
      top.cpu_bus.BUS_CPU_A11.lock(0);
      top.cpu_bus.BUS_CPU_A12.lock(0);
      top.cpu_bus.BUS_CPU_A13.lock(0);
      top.cpu_bus.BUS_CPU_A14.lock(0);
      top.cpu_bus.BUS_CPU_A15.lock(0);
    }

    top.cpu_bus.PIN_CPU_RDp.lock(1);
    top.cpu_bus.PIN_CPU_WRp.lock(0);

    top.cpu_bus.PIN_CPU_LATCH_EXT.lock(0);
  }

  //----------

  top.int_reg.PIN_CPU_ACK_VBLANK.set(wire(cpu.int_ack & INT_VBLANK_MASK));
  top.int_reg.PIN_CPU_ACK_STAT  .set(wire(cpu.int_ack & INT_STAT_MASK));
  top.int_reg.PIN_CPU_ACK_TIMER .set(wire(cpu.int_ack & INT_TIMER_MASK));
  top.int_reg.PIN_CPU_ACK_SERIAL.set(wire(cpu.int_ack & INT_SERIAL_MASK));
  top.int_reg.PIN_CPU_ACK_JOYPAD.set(wire(cpu.int_ack & INT_JOYPAD_MASK));

  //----------

  RegBase::tock_running = true;
  top.tock_slow(sys_rst, CLK, sys_clkgood, sys_t1, sys_t2, sys_cpuready);
  RegBase::tock_running = false;

  //----------

  tock_ext_bus();
  top.ser_reg.set_pins(DELTA_TRIZ, DELTA_TRIZ);
  tock_vram_bus();
  tock_zram_bus();
  tock_oam_bus();

  RegBase::sim_running = false;

  uint64_t hash = HASH_INIT;
  commit_and_hash(top, hash);
  return hash;
}

//-----------------------------------------------------------------------------

void GateBoy::tock_ext_bus() {
  uint16_t ext_addr = top.ext_bus.get_pin_addr();

  // ROM read
  if (sys_cart_loaded) {
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire OEn = top.ext_bus.PIN_EXT_RDn.qp();
    wire CEn = top.ext_bus.PIN_EXT_A15p.qp();

    if (!CEn && !OEn) {
      top.ext_bus.set_pin_data(cart_rom[rom_addr]);
    }
  }

  // Ext RAM read/write (also echo RAM)
  {
    uint16_t ram_addr = (ext_addr & 0x1FFF);

    wire WRn  = top.ext_bus.PIN_EXT_WRn.qp();
    wire CE1n = top.ext_bus.PIN_EXT_CSn.qp();
    wire CE2  = top.ext_bus.PIN_EXT_A14p.qp();
    wire OEn  = top.ext_bus.PIN_EXT_RDn.qp();

    // Write
    if (!CE1n && CE2 && !WRn) {
      ext_ram[ram_addr] = top.ext_bus.get_pin_data();
    }

    // Read
    if (!CE1n && CE2 && WRn && !OEn) {
      top.ext_bus.set_pin_data(ext_ram[ram_addr]);
    }
  }

  // Cart RAM read/write
  {
    // A000-BFFF
    // 0b101xxxxxxxxxxxxx

    uint16_t ram_addr = (ext_addr & 0x1FFF);

    wire WRn  = top.ext_bus.PIN_EXT_WRn.qp();
    wire CS1n = top.ext_bus.PIN_EXT_CSn.qp();
    wire CS2  = top.ext_bus.PIN_EXT_A13p.qp() && !top.ext_bus.PIN_EXT_A14p.qp() && top.ext_bus.PIN_EXT_A15p.qp();
    wire OEn = top.ext_bus.PIN_EXT_RDn.qp();

    // Write
    if (!CS1n && CS2 && !WRn) {
      cart_ram[ram_addr] = top.ext_bus.get_pin_data();
    }

    // Read
    if (!CS1n && CS2 && !OEn) {
      top.ext_bus.set_pin_data(cart_ram[ram_addr]);
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
      wire OEn = top.ext_bus.PIN_EXT_RDn.qp();
      wire CEn = top.ext_bus.PIN_EXT_A15p.qp();

      if (!CEn && !OEn) {
        top.ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
    }
  }
  */
}

//-----------------------------------------------------------------------------

void GateBoy::tock_vram_bus() {
  int vram_addr = top.vram_bus.get_pin_addr();
  uint8_t& vram_data = vid_ram[vram_addr];

  // We're getting a fake write on the first phase because PIN_VRAM_WRn resets to 0...
  // ignore it if we're in reset

  if (!sys_rst) {
    if (!top.vram_bus.PIN_VRAM_WRn.qp()) {
      vram_data = (uint8_t)top.vram_bus.get_pin_data();
    }

    if (!top.vram_bus.PIN_VRAM_OEn.qp()) {
      top.vram_bus.set_pin_data_in(vram_data);
    }
    else {
      top.vram_bus.set_pin_data_z();
    }
  }
}

//-----------------------------------------------------------------------------

void GateBoy::tock_oam_bus() {
  uint16_t oam_addr = top.oam_bus.get_oam_pin_addr();
  uint8_t& oam_data_a = oam_ram[(oam_addr << 1) + 0];
  uint8_t& oam_data_b = oam_ram[(oam_addr << 1) + 1];

  if (!top.oam_bus.PIN_OAM_WR_A.qp()) oam_data_a = top.oam_bus.get_oam_pin_data_a();
  if (!top.oam_bus.PIN_OAM_WR_B.qp()) oam_data_b = top.oam_bus.get_oam_pin_data_b();

  if (!top.oam_bus.PIN_OAM_OE.qp()) top.oam_bus.set_pin_data_a(oam_data_a);
  if (!top.oam_bus.PIN_OAM_OE.qp()) top.oam_bus.set_pin_data_b(oam_data_b);
}

//-----------------------------------------------------------------------------

void GateBoy::tock_zram_bus() {
  // ZRAM control signals are

  // top.clk_reg.PIN_CPU_BUKE_AxxxxxGH
  // top.TEDO_CPU_RDp();
  // top.TAPU_CPU_WRp_xxxxEFGx()
  // top.cpu_bus.SYKE_FF00_FFFFp()

  // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

  int addr = top.cpu_bus.get_bus_addr();
  bool hit_zram = (addr >= 0xFF80) && (addr <= 0xFFFE);

  if (hit_zram) {
    uint8_t& data = zero_ram[addr & 0x007F];
    if (top.TAPU_CPU_WRp_xxxxEFGx) data = top.cpu_bus.get_bus_data();
    if (top.TEDO_CPU_RDp) top.cpu_bus.set_data(data);
  }
}

//-----------------------------------------------------------------------------
