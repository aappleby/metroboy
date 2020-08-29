#include "GateBoyLib/GateBoy.h"
#include <memory.h>

#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h"
#include "CoreLib/File.h"

//-----------------------------------------------------------------------------

GateBoy::GateBoy() {
  memset(fb, 4, 160*144);
  memset(ext_ram, 0, 8192);
  memset(cart_rom, 0, 32768);
  memset(cart_ram, 0, 8192);
  cpu.reset(0x100);
}

//-----------------------------------------------------------------------------

void GateBoy::reset(bool verbose) {
  if (verbose) printf("GateBoy::run_reset_sequence() begin\n");

  // No bus activity during reset
  dbg_req = {.addr = 0x0000, .data = 0, .read = 0, .write = 0 };

  if (verbose) printf("In reset\n");
  run(8);

  if (verbose) printf("Out of reset\n");
  sys_rst = 0;
  run(8);

  sys_clken = 1;
  while(1) {
    if (verbose) printf("Sync with phase A\n");
    run(1);
    if (top.clk_reg.AFUR_xxxxEFGH.qn() &&
        top.clk_reg.ALEF_AxxxxFGH.qp() &&
        top.clk_reg.APUK_ABxxxxGH.qp() &&
        top.clk_reg.ADYK_ABCxxxxH.qp()) break;
  }
  CHECK_P(top.cpu_bus.CPU_PIN_BOMA_Axxxxxxx.tp());

  if (verbose) printf("Sync done, reset phase counter to 0\n");
  phase_total = 0;
  pass_count = 0;
  pass_total = 0;

  if (verbose) printf("Set CLKGOOD\n");
  sys_clkgood = 1;
  run(8);

  while(!top.cpu_bus.CPU_PIN_STARTp.tp()) {
    if (verbose) printf("Wait for CPU_PIN_START\n");
    run(8);
  }

  if (verbose) printf("CPU_PIN_START high, delay 8 phases\n");
  run(8);

  if (verbose) printf("Set CPU_READY\n");
  sys_cpuready = 1;
  run(8);
  if (verbose) printf("GateBoy::run_reset_sequence() done\n");
  if (verbose) printf("\n");

  cpu.get_bus_req(cpu_req);
}

//------------------------------------------------------------------------------

void GateBoy::set_boot_bit() {
  dbg_write(0xFF50, 0xFF);
}

//------------------------------------------------------------------------------

void GateBoy::load_dump(const char* filename) {
  printf("Loading %s\n", filename);

  uint8_t* dump = new uint8_t[65536];
  memset(dump, 0, 65536);

  size_t size = load_blob(filename, dump, 65536);

  memcpy(cart_rom, dump + 0x0000, 32768);
  memcpy(vid_ram,  dump + 0x8000, 8192);
  memcpy(cart_ram, dump + 0xA000, 8192);
  memcpy(ext_ram,  dump + 0xC000, 8192);

  // OAM is actually stored inverted, so invert it here.
  for (int i = 0; i < 256; i++) {
    oam_ram[i] = ~dump[0xFE00 + i];
  }

  memcpy(zero_ram, dump + 0xFF80, 128);

  dbg_write(ADDR_BGP,  dump[ADDR_BGP]);
  dbg_write(ADDR_OBP0, dump[ADDR_OBP0]);
  dbg_write(ADDR_OBP1, dump[ADDR_OBP1]);
  dbg_write(ADDR_SCY,  dump[ADDR_SCY]);
  dbg_write(ADDR_SCX,  dump[ADDR_SCX]);
  dbg_write(ADDR_WY,   dump[ADDR_WY]);
  dbg_write(ADDR_WX,   dump[ADDR_WX]);

  // Bit 7 - LCD Display Enable             (0=Off, 1=On)
  // Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
  // Bit 5 - Window Display Enable          (0=Off, 1=On)
  // Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
  // Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
  // Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
  // Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
  // Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)

  // #define FLAG_BG_ON        0x01
  // #define FLAG_OBJ_ON       0x02
  // #define FLAG_TALL_SPRITES 0x04
  // #define FLAG_BG_MAP_1     0x08
  // #define FLAG_TILE_0       0x10
  // #define FLAG_WIN_ON       0x20
  // #define FLAG_WIN_MAP_1    0x40
  // #define FLAG_LCD_ON       0x80

  dbg_write(ADDR_LCDC, dump[ADDR_LCDC]);

  printf("Loaded %zd bytes from dump %s\n", size, filename);
}

//------------------------------------------------------------------------------

void GateBoy::load_rom(const char* filename) {
  printf("Loading %s\n", filename);

  uint8_t* dump = new uint8_t[65536];
  size_t size = load_blob(filename, dump, 65536);

  memcpy(cart_rom, dump, 32768);
  memset(vid_ram,  0,    8192);
  memset(cart_ram, 0,    8192);
  memset(ext_ram,  0,    8192);
  memset(oam_ram,  0,    256);
  memset(zero_ram, 0,    128);

  printf("Loaded %zd bytes from rom %s\n", size, filename);
}

//------------------------------------------------------------------------------

uint8_t GateBoy::dbg_read(int addr) {
  CHECK_P((phase_total & 7) == 0);
  dbg_req = {.addr = uint16_t(addr), .data = 0, .read = 1, .write = 0 };
  /* AB */ next_phase();
  /* BC */ next_phase();
  /* CD */ next_phase();
  /* DE */ next_phase();
  /* EF */ next_phase();
  /* FG */ next_phase();
  uint8_t bus_data = top.cpu_bus.get_bus_data();
  /* GH */ next_phase();
  /* HA */ next_phase();
  dbg_req = {0};

  return bus_data;
}

//------------------------------------------------------------------------------

void GateBoy::dbg_write(int addr, uint8_t data) {
  CHECK_P((phase_total & 7) == 0);
  dbg_req = {.addr = uint16_t(addr), .data = data, .read = 0, .write = 1 };
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

  if (phase_total == 164) {
    ack_vblank = 1;
  }

  if (script) {
    dbg_req = script[(phase_total / 8) % script_len];
  }

  //----------
  // Update CPU

  if (sys_cpu_en) {
    uint8_t imask = 0;
    uint8_t intf = 0;

    uint8_t bus_data = top.cpu_bus.get_bus_data();

    switch(old_phase) {
    /* AB */ case 0: cpu.tock_req(imask, intf, bus_data); break; // bus request _must_ change on AB, see trace
    /* BC */ case 1: break;
    /* CD */ case 2: break;
    /* DE */ case 3: break;
    /* EF */ case 4: cpu.tock_ack(imask, intf, bus_data); break; // bus data latches
    /* FG */ case 5: break;
    /* GH */ case 6: break;
    /* HA */ case 7: break;
    }
  }

  if (DELTA_AB) {
    cpu_req.addr &= 0x00FF;
  }

  if (DELTA_BC) {
    if (!sys_cpu_en || dbg_req.read || dbg_req.write) {
      cpu_req = dbg_req;
    }
    else {
      cpu_req = cpu.bus_req;
    }
  }

  //----------
  // Run logic passes

  uint64_t hash_regs_old  = HASH_INIT;
  uint64_t hash_regs_new  = HASH_INIT;

  StringDumper d;

  for (pass_count = 1; pass_count < 100; pass_count++) {
    hash_regs_old = hash_regs_new;
    hash_regs_new  = next_pass(old_phase, new_phase);
    if (hash_regs_new == hash_regs_old) break;
    if (pass_count == 90) {
      printf("!!!STUCK!!!\n");
    }
  }

  if (RegBase::bus_collision) {
    printf("Bus collision!\n");
    RegBase::bus_collision = false;
  }

  if (RegBase::bus_floating) {
    // FIXME hitting this a lot
    //printf("Bus floating!\n");
    RegBase::bus_floating = false;
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
  int fb_y = top.lcd_reg.get_y();

  if (fb_x >= 0 && fb_x < 160 && fb_y >= 0 && fb_y < 144) {
    int p0 = top.PIN_LCD_DATA0n.qp();
    int p1 = top.PIN_LCD_DATA1n.qp();
    fb[fb_x + fb_y * 160] = uint8_t(p0 + p1 * 2);
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

  top.joypad.preset_buttons(sys_buttons);

  RegBase::tick_running = true;
  top.tick_slow(sys_rst, CLK, sys_clkgood, sys_t1, sys_t2, sys_cpuready);
  RegBase::tick_running = false;

  RegBase::tock_running = true;
  top.tock_slow(sys_rst, CLK, sys_clkgood, sys_t1, sys_t2, sys_cpuready);
  RegBase::tock_running = false;

  tock_ext_bus();
  
  bool addr_rom = cpu_req.addr <= 0x7FFF;
  bool addr_ram = cpu_req.addr >= 0xA000 && cpu_req.addr < 0xFDFF;
  bool addr_ext = (cpu_req.read || cpu_req.write) && (addr_rom || addr_ram) && !top.cpu_bus.CPU_PIN_BOOTp.tp();
  bool hold_mem = cpu_req.read && (cpu_req.addr < 0xFF00);

  top.cpu_bus.CPU_PIN6 = 0;
  top.cpu_bus.set_addr(cpu_req.addr);
  top.cpu_bus.set_data(cpu_req.write, cpu_req.data_lo);
  top.cpu_bus.CPU_PIN_RDp = !cpu_req.write;
  top.cpu_bus.CPU_PIN_WRp = cpu_req.write;  
  top.cpu_bus.CPU_PIN_ADDR_EXTp = addr_ext;

  if (DELTA_AB) { top.cpu_bus.CPU_PIN_LATCH_EXT = 0; }
  if (DELTA_BC) { top.cpu_bus.CPU_PIN_LATCH_EXT = 0; }
  if (DELTA_CD) { top.cpu_bus.CPU_PIN_LATCH_EXT = hold_mem; }
  if (DELTA_DE) { top.cpu_bus.CPU_PIN_LATCH_EXT = hold_mem; }
  if (DELTA_EF) { top.cpu_bus.CPU_PIN_LATCH_EXT = hold_mem; }
  if (DELTA_FG) { top.cpu_bus.CPU_PIN_LATCH_EXT = hold_mem; }
  if (DELTA_GH) { top.cpu_bus.CPU_PIN_LATCH_EXT = hold_mem; }
  if (DELTA_HA) { top.cpu_bus.CPU_PIN_LATCH_EXT = hold_mem; }

  top.int_reg.CPU_PIN_ACK_VBLANK = ack_vblank;
  top.int_reg.CPU_PIN_ACK_STAT   = ack_stat;
  top.int_reg.CPU_PIN_ACK_TIMER  = ack_timer;
  top.int_reg.CPU_PIN_ACK_SERIAL = ack_serial;
  top.int_reg.CPU_PIN_ACK_JOYPAD = ack_joypad;

  top.ser_reg.SCK = DELTA_TRIZ;
  top.ser_reg.SIN = DELTA_TRIZ;

  tock_vram_bus();
  tock_zram_bus();
  tock_oam_bus();

  RegBase::sim_running = false;

  uint64_t hash = HASH_INIT;
  commit_and_hash(top, hash);
  return hash;
}

//-----------------------------------------------------------------------------

#pragma warning(disable : 4189)

void GateBoy::tock_ext_bus() {
  uint16_t ext_addr = top.ext_bus.get_pin_addr();

  // ROM read
  {
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire OEn = top.ext_bus.EXT_PIN_RDn.qp();
    wire CEn = top.ext_bus.EXT_PIN_A15p.qp();

    if (!CEn && !OEn) {
      top.ext_bus.set_pin_data(cart_rom[rom_addr]);
    }
  }

  // Ext RAM read/write (also echo RAM)
  {
    uint16_t ram_addr = (ext_addr & 0x1FFF);

    wire WRn  = top.ext_bus.EXT_PIN_WRn.qp();
    wire CE1n = top.ext_bus.EXT_PIN_CSn.qp();
    wire CE2  = top.ext_bus.EXT_PIN_A14p.qp();
    wire OEn  = top.ext_bus.EXT_PIN_RDn.qp();

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

    wire WRn  = top.ext_bus.EXT_PIN_WRn.qp();
    wire CS1n = top.ext_bus.EXT_PIN_CSn.qp();
    wire CS2  = top.ext_bus.EXT_PIN_A13p.qp() && !top.ext_bus.EXT_PIN_A14p.qp() && top.ext_bus.EXT_PIN_A15p.qp();
    wire OEn = top.ext_bus.EXT_PIN_RDn.qp();

    // Write
    if (!CS1n && CS2 && !WRn) {
      cart_ram[ram_addr] = top.ext_bus.get_pin_data();
    }

    // Read
    if (!CS1n && CS2 && !OEn) {
      top.ext_bus.set_pin_data(cart_ram[ram_addr]);
    }
  }

  // MBC1

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
  }
  */

#if 0

  // CS seems to actually serve as a mux between rom/ram.
  // Based on the traces, the gb-live32 cart ignores the high bit of the
  // address and puts data on the bus on phase B if CSn is high.

  if (!top.ext_bus.EXT_PIN_CSn.qp()) {
    // FIXME need to split this up into ext ram / cart ram / echo ram
    if (!top.ext_bus.EXT_PIN_WRn.qp()) {
      mem[ext_addr] = top.ext_bus.get_pin_data();
    }

    if (!top.ext_bus.EXT_PIN_RDn.qp()) {
      top.ext_bus.set_pin_data(mem[ext_addr]);
    }
    else {
      top.ext_bus.set_pin_data_z();
    }
  }
  else {
    if (!(ext_addr & 0x8000) && !top.ext_bus.EXT_PIN_RDn.qp()) {
      top.ext_bus.set_pin_data(mem[ext_addr]);
    }
    else {
      top.ext_bus.set_pin_data_z();
    }
  }

#endif
}

//-----------------------------------------------------------------------------

void GateBoy::tock_vram_bus() {
  int vram_addr = top.vram_bus.get_pin_addr();
  uint8_t& vram_data = vid_ram[vram_addr];

  if (!top.vram_bus.VRAM_PIN_WRn.qp()) vram_data = (uint8_t)top.vram_bus.get_pin_data();

  if (!top.vram_bus.VRAM_PIN_OEn.qp()) {
    top.vram_bus.set_pin_data_in(vram_data);
  }
  else {
    top.vram_bus.set_pin_data_z();
  }
}

//-----------------------------------------------------------------------------

void GateBoy::tock_oam_bus() {
  uint16_t oam_addr = top.oam_bus.get_oam_pin_addr();
  uint8_t& oam_data_a = oam_ram[(oam_addr << 1) + 0];
  uint8_t& oam_data_b = oam_ram[(oam_addr << 1) + 1];

  if (!top.oam_bus.OAM_PIN_WR_A.tp()) oam_data_a = top.oam_bus.get_oam_pin_data_a();
  if (!top.oam_bus.OAM_PIN_WR_B.tp()) oam_data_b = top.oam_bus.get_oam_pin_data_b();

  if (!top.oam_bus.OAM_PIN_OE.tp()) top.oam_bus.set_pin_data_a(oam_data_a);
  if (!top.oam_bus.OAM_PIN_OE.tp()) top.oam_bus.set_pin_data_b(oam_data_b);
}

//-----------------------------------------------------------------------------

void GateBoy::tock_zram_bus() {
  // ZRAM control signals are

  // top.clk_reg.CPU_PIN_BUKE_AxxxxxGH
  // top.TEDO_CPU_RDp();
  // top.TAPU_CPU_WRp_xxxxEFGx()
  // top.cpu_bus.SYKE_FF00_FFFFp()

  // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

  int addr = top.cpu_bus.get_bus_addr();
  bool hit_zram = (addr >= 0xFF80) && (addr <= 0xFFFE);

  if (hit_zram) {
    uint8_t& data = zero_ram[addr & 0x007F];
    if (top.TAPU_CPU_WRp_xxxxEFGx) data = top.cpu_bus.get_bus_data();
    if (top.TEDO_CPU_RDp) top.cpu_bus.set_data(true, data);
  }
}

//-----------------------------------------------------------------------------
