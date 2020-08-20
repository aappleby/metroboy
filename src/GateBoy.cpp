#include "GateBoy.h"
#include <memory.h>

#include "Constants.h"
#include "Debug.h"
#include "File.h"

//-----------------------------------------------------------------------------

GateBoy::GateBoy() {
  memset(fb, 4, 160*144);
  cpu.reset(0x100);
}

//-----------------------------------------------------------------------------

void GateBoy::reset(bool verbose) {
  if (verbose) log("GateBoy::run_reset_sequence() begin\n");

  // No bus activity during reset
  dbg_req = {.addr = 0x0000, .data = 0, .read = 0, .write = 0 };

  if (verbose) log("In reset\n");
  run(8);

  if (verbose) log("Out of reset\n");
  sys_rst = 0;
  run(8);

  sys_clken = 1;
  while(1) {
    if (verbose) log("Sync with phase A\n");
    run(1);
    if (top.clk_reg.AFUR_xxxxEFGH.qn() &&
        top.clk_reg.ALEF_AxxxxFGH.qp() &&
        top.clk_reg.APUK_ABxxxxGH.qp() &&
        top.clk_reg.ADYK_ABCxxxxH.qp()) break;
  }
  CHECK_P(top.clk_reg.CPU_PIN_BOMA_Axxxxxxx.tp());

  if (verbose) log("Sync done, reset phase counter to 0\n");
  phase_total = 0;
  pass_count = 0;
  pass_total = 0;

  if (verbose) log("Set CLKGOOD\n");
  sys_clkgood = 1;
  run(8);

  while(!top.clk_reg.CPU_PIN_STARTp.tp()) {
    if (verbose) log("Wait for CPU_PIN_START\n");
    run(8);
  }

  if (verbose) log("CPU_PIN_START high, delay 8 phases\n");
  run(8);

  if (verbose) log("Set CPU_READY\n");
  sys_cpuready = 1;
  run(8);
  if (verbose) log("GateBoy::run_reset_sequence() done\n");
  if (verbose) log("\n");

  cpu.get_bus_req(cpu_req);
}

//------------------------------------------------------------------------------

void GateBoy::set_boot_bit() {
  dbg_write(0xFF50, 0xFF);
}

//------------------------------------------------------------------------------

void GateBoy::log(const char* format, ...) {
  printf("@%06d : ", phase_total);
  va_list args;
  va_start (args, format);
  vprintf (format, args);
  va_end (args);
}

//------------------------------------------------------------------------------

void GateBoy::load(const char* filename) {
  log("Loading %s\n", filename);
  memset(mem, 0, 65536);
  size_t size = load_blob(filename, mem);

  // OAM is actually stored inverted, so invert it here.
  for (int addr = 0xFE00; addr <= 0xFEFF; addr++) {
    mem[addr] = ~mem[addr];
  }

  dbg_write(ADDR_BGP,  mem[ADDR_BGP]);
  dbg_write(ADDR_OBP0, mem[ADDR_OBP0]);
  dbg_write(ADDR_OBP1, mem[ADDR_OBP1]);
  dbg_write(ADDR_SCY,  mem[ADDR_SCY]);
  dbg_write(ADDR_SCX,  mem[ADDR_SCX]);
  dbg_write(ADDR_WY,   mem[ADDR_WY]);
  dbg_write(ADDR_WX,   mem[ADDR_WX]);

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

  dbg_write(ADDR_LCDC, mem[ADDR_LCDC]);

  log("Loaded %zd bytes from %s\n", size, filename);
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

  //----------
  // Update CPU

  // ab BC CD DE ef fg gh ha

  cpu_req = cpu_en ? cpu.bus_req : dbg_req;


  if (cpu_en) {
    uint8_t imask = 0;
    uint8_t intf = 0;

    //req.addr = 0x1234;

    Ack ack = {0};

    ack.addr    = top.cpu_bus.get_bus_addr();
    ack.data_lo = top.cpu_bus.get_bus_data();

    switch(old_phase) {
    case 0: cpu.tock_a(imask, intf, ack); break;
    case 1: cpu.tock_b(imask, intf, ack); break;
    case 2: cpu.tock_c(imask, intf, ack); break;
    case 3: cpu.tock_d(imask, intf, ack); break;
    case 4: cpu.tock_e(imask, intf, ack); break;
    case 5: cpu.tock_f(imask, intf, ack); break;
    case 6: cpu.tock_g(imask, intf, ack); break;
    case 7: cpu.tock_h(imask, intf, ack); break;
    }
  }

  bus_data = top.cpu_bus.get_bus_data();

  // AB bc cd de ef fg gh ha

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
    int p0 = !top.LCD_PIN_DATA0n.tp();
    int p1 = !top.LCD_PIN_DATA1n.tp();
    fb[fb_x + fb_y * 160] = uint8_t(p0 + p1 * 2);
  }

  //----------
  // Done

  phase_total++;
  phase_hash = hash_regs_old;
  combine_hash(total_hash, phase_hash);
}

//-----------------------------------------------------------------------------

uint64_t GateBoy::next_pass(int old_phase, int new_phase) {
  uint64_t hash = HASH_INIT;

  wire CLK = (phase_total & 1) & sys_clken;

  top.clk_reg.preset_rst(sys_rst);
  top.clk_reg.preset_t1t2(sys_t1, sys_t2);
  top.clk_reg.preset_cpu_ready(sys_cpuready);
  top.clk_reg.preset_clk_a(sys_clkgood);
  top.clk_reg.preset_clk_b(CLK);
  top.joypad.preset_buttons(0);

  RegBase::sim_running = true;
  RegBase::bus_collision = false;
  RegBase::bus_floating = false;

  top.tick_slow();

  tock_ext_bus();
  tock_cpu_bus(old_phase, new_phase, cpu_req);
  tock_vram_bus();
  tock_zram_bus();
  tock_oam_bus();

  RegBase::sim_running = false;

  commit_and_hash(top, hash);

  return hash;
}

//-----------------------------------------------------------------------------

void GateBoy::tock_cpu_bus(int old_phase, int new_phase, Req req) {
  auto& bus = top.cpu_bus;

  bus.CPU_PIN6 = 0;

  // CPU_PIN_ADDR_EXT latches the cpu bus address into the ext address

  //bool     read  = req.read;
  bool     write = req.write;
  uint16_t addr  = req.addr;
  uint8_t  data  = req.data_lo;

  bool io = req.read || req.write;
  bool addr_ext = io && ((req.addr <= 0x7FFF) || (req.addr >= 0xA000 && req.addr < 0xFDFF));
  bool hold_mem = io && (req.addr < 0xFF00);

  if (top.cpu_bus.CPU_PIN_BOOTp.tp()) {
    addr_ext = false;
  }

  // if we set read on phase A, we fail the mem test when we switch from testing OAM to testing ZRAM.
  // these signals are trimmed down as much as they can be without breaking anything.
  // probably they should be fixed to align w cpu clock edges and match cart traces

  // CPU clocks for reference -
  //Tri CPU_PIN_BOWA_xBCDEFGH = TRI_HZNP; // top left port PORTD_01: // Blue clock - decoders, alu, some reset stuff
  //Tri CPU_PIN_BEDO_Axxxxxxx = TRI_HZNP; // top left port PORTD_02:
  //Tri CPU_PIN_BEKO_ABCDxxxx = TRI_HZNP; // top left port PORTD_03:
  //Tri CPU_PIN_BUDE_xxxxEFGH = TRI_HZNP; // top left port PORTD_04: 
  //Tri CPU_PIN_BOLO_ABCDEFxx = TRI_HZNP; // top left port PORTD_05: // CPU OEn? Would make sense with AFAS_xxxxEFGx as "WRen" I guess
  //Tri CPU_PIN_BUKE_AxxxxxGH = TRI_HZNP; // top left port PORTD_07: // this is probably the "latch bus data" clock
  //Tri CPU_PIN_BOMA_Axxxxxxx = TRI_HZNP; // top left port PORTD_08: (RESET_CLK)
  //Tri CPU_PIN_BOGA_xBCDEFGH = TRI_HZNP; // top left port PORTD_09: - test pad 3
  //Tri EXT_PIN_CLK_xxxxEFGH  = TRI_HZNP; // PIN_75 <- P01.BUDE/BEVA

#if 0
  if (DELTA_AB) { bus.set_addr(addr); bus.set_data_z();          bus.CPU_PIN_RDp = 0;    bus.CPU_PIN_WRp = 0;     bus.CPU_PIN_HOLD_MEM = 0;        bus.CPU_PIN_ADDR_EXTp = 0; }
  if (DELTA_BC) { bus.set_addr(addr); bus.set_data_z();          bus.CPU_PIN_RDp = 0;    bus.CPU_PIN_WRp = 0;     bus.CPU_PIN_HOLD_MEM = 0;        bus.CPU_PIN_ADDR_EXTp = 0; }
  if (DELTA_CD) { bus.set_addr(addr); bus.set_data_z();          bus.CPU_PIN_RDp = 0;    bus.CPU_PIN_WRp = 0;     bus.CPU_PIN_HOLD_MEM = 0;        bus.CPU_PIN_ADDR_EXTp = 0; }
  if (DELTA_DE) { bus.set_addr(addr); bus.set_data_z();          bus.CPU_PIN_RDp = 0;    bus.CPU_PIN_WRp = 0;     bus.CPU_PIN_HOLD_MEM = 0;        bus.CPU_PIN_ADDR_EXTp = 0; }
  if (DELTA_EF) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = read; bus.CPU_PIN_WRp = write; bus.CPU_PIN_HOLD_MEM = 0;        bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_FG) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = read; bus.CPU_PIN_WRp = 0;     bus.CPU_PIN_HOLD_MEM = hold_mem; bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_GH) { bus.set_addr(addr); bus.set_data_z();          bus.CPU_PIN_RDp = read; bus.CPU_PIN_WRp = 0;     bus.CPU_PIN_HOLD_MEM = hold_mem; bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_HA) { bus.set_addr(addr); bus.set_data_z();          bus.CPU_PIN_RDp = 0;    bus.CPU_PIN_WRp = 0;     bus.CPU_PIN_HOLD_MEM = 0;        bus.CPU_PIN_ADDR_EXTp = 0; }
#endif

  if (DELTA_AB) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = !write; bus.CPU_PIN_WRp = write;  bus.CPU_PIN_HOLD_MEM = hold_mem; bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_BC) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = !write; bus.CPU_PIN_WRp = write;  bus.CPU_PIN_HOLD_MEM = hold_mem; bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_CD) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = !write; bus.CPU_PIN_WRp = write;  bus.CPU_PIN_HOLD_MEM = hold_mem; bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_DE) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = !write; bus.CPU_PIN_WRp = write;  bus.CPU_PIN_HOLD_MEM = hold_mem; bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_EF) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = !write; bus.CPU_PIN_WRp = write;  bus.CPU_PIN_HOLD_MEM = 0;        bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_FG) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = !write; bus.CPU_PIN_WRp = 0;      bus.CPU_PIN_HOLD_MEM = hold_mem; bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_GH) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = !write; bus.CPU_PIN_WRp = 0;      bus.CPU_PIN_HOLD_MEM = hold_mem; bus.CPU_PIN_ADDR_EXTp = addr_ext; }
  if (DELTA_HA) { bus.set_addr(addr); bus.set_data(write, data); bus.CPU_PIN_RDp = !write; bus.CPU_PIN_WRp = write;  bus.CPU_PIN_HOLD_MEM = hold_mem; bus.CPU_PIN_ADDR_EXTp = addr_ext; }

  top.int_reg.CPU_PIN_ACK_VBLANK = 0;
  top.int_reg.CPU_PIN_ACK_STAT = 0;
  top.int_reg.CPU_PIN_ACK_TIMER = 0;
  top.int_reg.CPU_PIN_ACK_SERIAL = 0;
  top.int_reg.CPU_PIN_ACK_JOYPAD = 0;

  top.ser_reg.SCK_C = 0;
  top.ser_reg.SIN_Cn = 0;
}

//-----------------------------------------------------------------------------

void GateBoy::tock_ext_bus() {
  top.ext_bus.EXT_PIN_WR_C  = (top.ext_bus.EXT_PIN_WR_A.tp());
  top.ext_bus.EXT_PIN_RD_C  = (top.ext_bus.EXT_PIN_RD_A.tp());
  top.ext_bus.EXT_PIN_A00_C = (top.ext_bus.EXT_PIN_A00n_A.tp());
  top.ext_bus.EXT_PIN_A01_C = (top.ext_bus.EXT_PIN_A01n_A.tp());
  top.ext_bus.EXT_PIN_A02_C = (top.ext_bus.EXT_PIN_A02n_A.tp());
  top.ext_bus.EXT_PIN_A03_C = (top.ext_bus.EXT_PIN_A03n_A.tp());
  top.ext_bus.EXT_PIN_A04_C = (top.ext_bus.EXT_PIN_A04n_A.tp());
  top.ext_bus.EXT_PIN_A05_C = (top.ext_bus.EXT_PIN_A05n_A.tp());
  top.ext_bus.EXT_PIN_A06_C = (top.ext_bus.EXT_PIN_A06n_A.tp());
  top.ext_bus.EXT_PIN_A07_C = (top.ext_bus.EXT_PIN_A07n_A.tp());
  top.ext_bus.EXT_PIN_A08_C = (top.ext_bus.EXT_PIN_A08n_A.tp());
  top.ext_bus.EXT_PIN_A09_C = (top.ext_bus.EXT_PIN_A09n_A.tp());
  top.ext_bus.EXT_PIN_A10_C = (top.ext_bus.EXT_PIN_A10n_A.tp());
  top.ext_bus.EXT_PIN_A11_C = (top.ext_bus.EXT_PIN_A11n_A.tp());
  top.ext_bus.EXT_PIN_A12_C = (top.ext_bus.EXT_PIN_A12n_A.tp());
  top.ext_bus.EXT_PIN_A13_C = (top.ext_bus.EXT_PIN_A13n_A.tp());
  top.ext_bus.EXT_PIN_A14_C = (top.ext_bus.EXT_PIN_A14n_A.tp());
  top.ext_bus.EXT_PIN_A15_C = (top.ext_bus.EXT_PIN_A15n_A.tp());

  uint16_t ext_addr = top.ext_bus.get_pin_addr();

  if (top.ext_bus.EXT_PIN_WR_A.tp()) {
    uint8_t ext_data = top.ext_bus.get_pin_data_out();
    
    if (ext_addr >= 0 && ext_addr <= 0x7FFF) {
      //mem[ext_addr] = ext_data; // yeah we don't actually want to allow writing to ROM...
    }
    else if (ext_addr >= 0xA000 && ext_addr <= 0xBFFF) {
      mem[ext_addr] = ext_data;
    }
    else if (ext_addr >= 0xC000 && ext_addr <= 0xDFFF) {
      mem[ext_addr] = ext_data;
    }
    else if (ext_addr >= 0xE000 && ext_addr <= 0xFFFF) {
      mem[ext_addr] = ext_data;
    }
  }
  
  if (top.ext_bus.EXT_PIN_RD_A.tp()) {
    if (ext_addr >= 0 && ext_addr <= 0x7FFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.set_pin_data_in(ext_data);
    }
    else if (ext_addr >= 0xA000 && ext_addr <= 0xBFFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.set_pin_data_in(ext_data);
    }
    else if (ext_addr >= 0xC000 && ext_addr <= 0xDFFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.set_pin_data_in(ext_data);
    }
    else if (ext_addr >= 0xE000 && ext_addr <= 0xFFFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.set_pin_data_in(ext_data);
    }
    else {
      top.ext_bus.set_pin_data_z();
    }
  }
  else {
    top.ext_bus.EXT_PIN_D0n_C = DELTA_TRIZ;
    top.ext_bus.EXT_PIN_D1n_C = DELTA_TRIZ;
    top.ext_bus.EXT_PIN_D2n_C = DELTA_TRIZ;
    top.ext_bus.EXT_PIN_D3n_C = DELTA_TRIZ;
    top.ext_bus.EXT_PIN_D4n_C = DELTA_TRIZ;
    top.ext_bus.EXT_PIN_D5n_C = DELTA_TRIZ;
    top.ext_bus.EXT_PIN_D6n_C = DELTA_TRIZ;
    top.ext_bus.EXT_PIN_D7n_C = DELTA_TRIZ;
  }
}

//-----------------------------------------------------------------------------

void GateBoy::tock_vram_bus() {
  top.vram_bus._VRAM_PIN_CS_C = 0;
  top.vram_bus._VRAM_PIN_OE_C = 0;
  top.vram_bus._VRAM_PIN_WR_C = 0;

  uint16_t vram_addr = top.vram_bus.get_pin_addr();
  uint8_t& vram_data = mem[0x8000 + vram_addr];

  if (top.vram_bus._VRAM_PIN_WR_A.tp()) vram_data = top.vram_bus.get_pin_data_out();

  if (top.vram_bus._VRAM_PIN_OE_A.tp()) {
    top.vram_bus.set_pin_data_in(vram_data);
  }
  else {
    top.vram_bus.set_pin_data_z();
  }
}

//-----------------------------------------------------------------------------

void GateBoy::tock_oam_bus() {
  uint16_t oam_addr = top.oam_bus.get_oam_pin_addr();
  uint8_t& oam_data_a = mem[0xFE00 + (oam_addr << 1) + 0];
  uint8_t& oam_data_b = mem[0xFE00 + (oam_addr << 1) + 1];

  if (!top.oam_bus.OAM_PIN_WR_B.tp()) oam_data_b = top.oam_bus.get_oam_pin_data_b();
  if (!top.oam_bus.OAM_PIN_WR_A.tp()) oam_data_a = top.oam_bus.get_oam_pin_data_a();

  if (!top.oam_bus.OAM_PIN_OE.tp()) top.oam_bus.set_bus_data_a(oam_data_a);
  if (!top.oam_bus.OAM_PIN_OE.tp()) top.oam_bus.set_bus_data_b(oam_data_b);
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
    uint8_t& data = mem[addr];
    if (top.TAPU_CPU_WRp_xxxxEFGx()) data = top.cpu_bus.get_bus_data();
    if (top.TEDO_CPU_RDp())          top.cpu_bus.set_data(true, data);
  }
}

//-----------------------------------------------------------------------------
