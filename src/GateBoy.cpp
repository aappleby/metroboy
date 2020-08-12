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

void GateBoy::run_reset_sequence() {
  printf("GateBoy::run_reset_sequence()\n");

  log("Starting GateBoy sim\n");

  // No bus activity during reset
  dbg_req = {.addr = 0x0100, .data = 0, .read = 0, .write = 0 };

  log("In reset\n");
  run(8);

  log("Out of reset\n");
  sys_rst = 0;
  run(8);

  log("Sync with phase A\n");
  sys_clken = 1;
  run(11);

  CHECK_P(top.clk_reg.AFUR_ABCDxxxx.qp());
  CHECK_P(top.clk_reg.ALEF_AxxxxFGH.qp());
  CHECK_N(top.clk_reg.APUK_xxCDEFxx.qp());
  CHECK_P(top.clk_reg.ADYK_ABCxxxxH.qp());
  CHECK_P((phase_total & 7) == 0);

  log("Clock good\n");
  sys_clkgood = 1;
  run(8);

  log("Wait for CPU_PIN_START");
  while(!top.clk_reg.CPU_PIN_STARTp.qp()) {
    run(8);
  }

  log("Delay\n");
  run(8);

  log("Set CPU_READY = 1\n");
  sys_cpuready = 1;
  run(8);

  log("BOOT @ 0xFF50 => %d\n", dbg_read(0xFF50));
  log("Set BOOT = 1\n");
  dbg_write(0xFF50, 0xFF);
  log("BOOT @ 0xFF50 => %d\n", dbg_read(0xFF50));

  log("LCDC = 0x%02x\n", dbg_read(ADDR_LCDC));

  dbg_write(ADDR_LCDC, 0);

#if !_DEBUG
  test_all_regs();
  test_all_mem();
#endif

  const char* filename = "roms/LinksAwakening_dog.dump";
  load(filename);

  dbg_write(ADDR_BGP,  mem[ADDR_BGP]);
  dbg_write(ADDR_OBP0, mem[ADDR_OBP0]);
  dbg_write(ADDR_OBP1, mem[ADDR_OBP1]);
  dbg_write(ADDR_SCY, mem[ADDR_SCY]);
  dbg_write(ADDR_SCX, mem[ADDR_SCX]);
  dbg_write(ADDR_WY, mem[ADDR_WY]);
  dbg_write(ADDR_WX, mem[ADDR_WX]);

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

  //cpu_en = true;
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

  log("Loaded %zd bytes from %s\n", size, filename);
}

//------------------------------------------------------------------------------

void GateBoy::test_all_mem() {
  printf("//----------------------------------------\n");
  printf("// Testing Cart ROM read: ");
  test_mem(0x0000, 0x7FFF, 256, false);

  printf("// Testing VRAM read/write: ");
  test_mem(0x8000, 0x9FFF, 256, true);

  printf("// Testing Cart RAM read/write: ");
  test_mem(0xA000, 0xBFFF, 256, true);

  printf("// Testing Main RAM read/write: ");
  test_mem(0xC000, 0xDFFF, 256, true);

  printf("// Testing Echo RAM read/write: ");
  test_mem(0xE000, 0xFDFF, 256, true);

  printf("// Testing OAM RAM read/write: ");
  test_mem(0xFE00, 0xFEFF, 1, true);

  printf("// Testing ZRAM read/write: ");
  test_mem(0xFF80, 0xFFFE, 1, true);
}

//------------------------------------------------------------------------------

void GateBoy::test_all_regs() {
  printf("//----------------------------------------\n");
  printf("// Testing reg read/write\n");

  test_reg("JOYP", 0xFF00, 0b00110000);
  
  //test_reg("SB",   0xFF01, 0b11111111); // something wrong with these ones
  //test_reg("SC",   0xFF02, 0b10000011);

  //test_reg("DIV",  0xFF04, 177); // not standard reg
  //test_reg("TIMA", 0xFF05, 177); // not standard reg

  test_reg("TMA",  0xFF06, 0b11111111);
  test_reg("TAC",  0xFF07, 0b00000111);

  //test_reg("LCDC", 0xFF40, 177); // so if we enable the ppu, we crash after a while because we get stuck in rendering state because we're not emitting pixels
  //test_reg("STAT", 0xFF41, 0b11110000); // broken
  test_reg("SCY",  0xFF42, 0b11111111);
  test_reg("SCX",  0xFF43, 0b11111111);
  //test_reg("LY",   0xFF44, 177); // not standard reg
  test_reg("LYC",  0xFF45, 0b11111111);
  //test_reg("DMA",  0xFF46, 0b11111111); works, but let's not trigger random dmas...
  test_reg("BGP",  0xFF47, 0b11111111);
  test_reg("OBP0", 0xFF48, 0b11111111);
  test_reg("OBP1", 0xFF49, 0b11111111);
  test_reg("WY",   0xFF4A, 0b11111111);
  test_reg("WX",   0xFF4B, 0b11111111);

  //test_reg("IF",   0xFF0F, 0b00011111); // broken
  //test_reg("IE",   0xFFFF, 0b00011111);
  printf("\n");
}

//------------------------------------------------------------------------------

void GateBoy::test_mem(uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write) {
  for (uint16_t addr = addr_start; addr <= addr_end; addr += step) {
    uint8_t data_wr = 0x55;
    if (test_write) {
      mem[addr] = 0;
      dbg_write(addr, data_wr);
    }
    else {
      mem[addr] = data_wr;
    }

    uint8_t data_rd = dbg_read(addr);
    if (data_rd != data_wr) {
      printf("XXXXXXXXXXXXXXXXX FAIL XXXXXXXXXXXXXXXXX @ 0x%04x : expected 0x%02x, was 0x%02x\n", addr, data_wr, data_rd);
      return;
    }
  }

  for (uint16_t addr = addr_start; addr <= addr_end; addr += step) {
    uint8_t data_wr = 0xAA;
    if (test_write) {
      mem[addr] = 0;
      dbg_write(addr, data_wr);
    }
    else {
      mem[addr] = data_wr;
    }

    uint8_t data_rd = dbg_read(addr);
    if (data_rd != data_wr) {
      printf("XXXXXXXXXXXXXXXXX FAIL XXXXXXXXXXXXXXXXX @ 0x%04x : expected 0x%02x, was 0x%02x\n", addr, data_wr, data_rd);
      return;
    }
  }

  printf("PASS\n");
}

//------------------------------------------------------------------------------

void GateBoy::test_reg(const char* regname, uint16_t addr, uint8_t mask) {

  printf("Testing reg %5s: ", regname);
  for (int data_in = 0; data_in < 256; data_in++) {
    if (!(data_in & mask)) continue;
    dbg_write(addr, uint8_t(data_in));
    uint8_t data_out = dbg_read(addr);
    //printf("%5s @ %04x: wrote %02x, read %02x\n", regname, addr, data_in & mask, data_out & mask);

    if ((data_in & mask) != (data_out & mask)) {
      printf("XXXXXXXXXXXXXXXXX FAIL XXXXXXXXXXXXXXXXX - wrote 0x%02x, read 0x%02x\n", uint8_t(data_in & mask), uint8_t(data_out & mask));
      return;
    }
  }
  printf("PASS\n");
}

//------------------------------------------------------------------------------

#pragma warning(disable:4189)

uint8_t GateBoy::dbg_read(int addr) {
  CHECK_P((phase_total & 7) == 0);
  dbg_req = {.addr = uint16_t(addr), .data = 0, .read = 1, .write = 0 };

  /* A->B */ phase();
  /* B->C */ phase();
  /* C->D */ phase();
  /* D->E */ phase();
  /* E->F */ phase();

  // CPU_PIN_BUKE_xxxxxFxx asserted on F, assuming this is "latch bus data".
  uint8_t sample = top.cpu_bus.get_bus_data();

  /* F->G */ phase();
  /* G->H */ phase();
  /* H->A */ phase();

  return sample;
}

void GateBoy::dbg_write(int addr, uint8_t data) {
  CHECK_P((phase_total & 7) == 0);
  dbg_req = {.addr = uint16_t(addr), .data = data, .read = 0, .write = 1 };

  /* A->B */ phase();
  /* B->C */ phase();
  /* C->D */ phase();
  /* D->E */ phase();
  /* E->F */ phase();
  /* F->G */ phase();
  /* G->H */ phase();
  /* H->A */ phase();
}

//------------------------------------------------------------------------------

#define CHECK_CLK_PHASE(A, B) CHECK_P(wire(A) == wire((B) & (1 << (7 - phase))))

void GateBoy::phase() {
  phase_total++;
  const int phase = phase_total & 7;

  //----------
  // Update CPU

  cpu_req = cpu_en ? cpu.bus_req : dbg_req;

  if (cpu_en) {
    uint8_t imask = 0;
    uint8_t intf = 0;

    //req.addr = 0x1234;

    Ack ack = {0};

    ack.addr    = top.cpu_bus.get_bus_addr();
    ack.data_lo = top.cpu_bus.get_bus_data();

    switch(phase) {
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


  //----------
  // Update clock and buses

  uint64_t hash_regs_old  = HASH_INIT;
  uint64_t hash_regs_new  = HASH_INIT;

  StringDumper d;

  for (pass_count = 0; pass_count < 100; pass_count++) {
    hash_regs_old = hash_regs_new;
    hash_regs_new  = pass();
    if (hash_regs_new == hash_regs_old) break;
    if (pass_count == 90) {
      printf("!!!STUCK!!!\n");
    }
  }

  if (bus_collision) {
    printf("Bus collision!\n");
    bus_collision = false;
  }

  CHECK_P(pass_count < 100);

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
    int p0 = top.LCD_PIN_DATA0.qp();
    int p1 = top.LCD_PIN_DATA1.qp();
    fb[fb_x + fb_y * 160] = uint8_t(p0 + p1 * 2);
  }

  //----------

#if 1

  if (sys_clkgood) {
    CHECK_CLK_PHASE(top.clk_reg.ATAL_xBxDxFxH(), 0b01010101);
    CHECK_CLK_PHASE(top.clk_reg.ZAXY_xBxDxFxH(), 0b01010101);
    CHECK_CLK_PHASE(top.clk_reg.AZOF_AxCxExGx(), 0b10101010); 
    CHECK_CLK_PHASE(top.clk_reg.ZAXY_xBxDxFxH(), 0b01010101); 
    CHECK_CLK_PHASE(top.clk_reg.ZEME_AxCxExGx(), 0b10101010); 
    CHECK_CLK_PHASE(top.clk_reg.ALET_xBxDxFxH(), 0b01010101); 
    CHECK_CLK_PHASE(top.clk_reg.MEHE_AxCxExGx(), 0b10101010); 
    CHECK_CLK_PHASE(top.clk_reg.MYVO_AxCxExGx(), 0b10101010); 


    if (sys_clken) {
      CHECK_CLK_PHASE(top.clk_reg.AFUR_ABCDxxxx.qp(), 0b11110000);
      CHECK_CLK_PHASE(top.clk_reg.ALEF_AxxxxFGH.qp(), 0b10000111);
      CHECK_CLK_PHASE(top.clk_reg.APUK_xxCDEFxx.qp(), 0b00111100);
      CHECK_CLK_PHASE(top.clk_reg.ADYK_ABCxxxxH.qp(), 0b11100001);
      CHECK_CLK_PHASE(top.clk_reg.AROV_ABxxxxGH,     0b11000011);
      CHECK_CLK_PHASE(top.clk_reg.AFEP_AxxxxFGH,     0b10000111);
      CHECK_CLK_PHASE(top.clk_reg.ATYP_xxxxEFGH,     0b00001111);
      CHECK_CLK_PHASE(top.clk_reg.AJAX_ABCDxxxx,     0b11110000);

      CHECK_CLK_PHASE(top.clk_reg.ADAR_ABCxxxxH(),   0b11100001);
      CHECK_CLK_PHASE(top.clk_reg.AFAS_xxxDxxxx(),   0b00010000);

      if (sys_cpuready) {
        CHECK_CLK_PHASE(top.clk_reg.BELU_ABCDxxxx(),       0b11110000);
        CHECK_CLK_PHASE(top.clk_reg.BYRY_xxxxEFGH(),       0b00001111);
        CHECK_CLK_PHASE(top.clk_reg.BUDE_ABCDxxxx(),       0b11110000);
        CHECK_CLK_PHASE(top.clk_reg.UVYT_xxxxEFGH(),       0b00001111);
        CHECK_CLK_PHASE(top.clk_reg.BEKO_xxxxEFGH(),       0b00001111);
        CHECK_CLK_PHASE(top.clk_reg.MOPA_ABCDxxxx(),       0b11110000);
        CHECK_CLK_PHASE(top.clk_reg.XYNY_xxxxEFGH(),       0b00001111);

        CHECK_CLK_PHASE(top.clk_reg.BAPY_xxCDxxxx(),       0b00110000);
        CHECK_CLK_PHASE(top.clk_reg.BERU_ABxxEFGH(),       0b11001111);
        CHECK_CLK_PHASE(top.clk_reg.BUFA_xxCDxxxx(),       0b00110000);
        CHECK_CLK_PHASE(top.clk_reg.BOLO_ABxxEFGH(),       0b11001111);

        CHECK_CLK_PHASE(top.clk_reg.BEJA_ABCDxxxx(),       0b11110000);
        CHECK_CLK_PHASE(top.clk_reg.BANE_xxxxEFGH(),       0b00001111);
        CHECK_CLK_PHASE(top.clk_reg.BELO_ABCDxxxx(),       0b11110000);
        CHECK_CLK_PHASE(top.clk_reg.BAZE_xxxxEFGH(),       0b00001111);

        CHECK_CLK_PHASE(top.clk_reg.BUTO_ABCDExxx(),       0b11111000);
        CHECK_CLK_PHASE(top.clk_reg.BELE_xxxxxFGH(),       0b00000111);
        CHECK_CLK_PHASE(top.clk_reg.BYJU_ABCDExxx(),       0b11111000);
        CHECK_CLK_PHASE(top.clk_reg.BALY_xxxxxFGH(),       0b00000111);
        CHECK_CLK_PHASE(top.clk_reg.BOGA_ABCDExxx(),       0b11111000);
        CHECK_CLK_PHASE(top.clk_reg.BOMA_xxxxxFGH(),       0b00000111);

        CHECK_CLK_PHASE(top.clk_reg.CPU_PIN_BOWA_ABCDExxx.qp(), 0b11111000);
        CHECK_CLK_PHASE(top.clk_reg.CPU_PIN_BEDO_xxxxxFGH.qp(), 0b00000111);
        CHECK_CLK_PHASE(top.clk_reg.CPU_PIN_BEKO_xxxxEFGH.qp(), 0b00001111);
        CHECK_CLK_PHASE(top.clk_reg.CPU_PIN_BUDE_ABCDxxxx.qp(), 0b11110000);
        CHECK_CLK_PHASE(top.clk_reg.CPU_PIN_BOLO_ABxxEFGH.qp(), 0b11001111);
        CHECK_CLK_PHASE(top.clk_reg.CPU_PIN_BUKE_xxxxxFxx.qp(), 0b00000100);
        CHECK_CLK_PHASE(top.clk_reg.CPU_PIN_BOMA_xxxxxFGH.qp(), 0b00000111);
        CHECK_CLK_PHASE(top.clk_reg.CPU_PIN_BOGA_ABCDExxx.qp(), 0b11111000);
      }
    }
  }
#endif

  //----------
  // Done

  phase_hash = hash_regs_old;
  combine_hash(total_hash, phase_hash);
}

//-----------------------------------------------------------------------------

uint64_t GateBoy::pass() {
  const int phase = phase_total & 7;
  uint64_t hash = HASH_INIT;

  wire CLK = (phase_total & 1) & sys_clken;
  top.clk_reg.preset_rst(sys_rst);
  top.clk_reg.preset_t1t2(sys_t1, sys_t2);
  top.clk_reg.preset_cpu_ready(sys_cpuready);
  top.clk_reg.preset_clk_a(sys_clkgood);
  top.clk_reg.preset_clk_b(CLK);
  top.joypad.preset_buttons(0);
  update_cpu_bus(phase, cpu_req);
  update_ext_bus(phase);
  update_vrm_bus(phase);
  update_oam_bus(phase);
  update_zram_bus(phase);

  Sig::sim_running = true;
  bus_collision = false;
  top.tick_slow(phase);
  Sig::sim_running = false;

  commit_and_hash(top, hash);

  return hash;
}

//-----------------------------------------------------------------------------

void GateBoy::update_cpu_bus(int phase, Req req) {
  auto& bus = top.cpu_bus;

  bus.CPU_PIN6.preset(0);

  // CPU_PIN_ADDR_EXT latches the cpu bus address into the ext address

  bool     read  = req.read;
  bool     write = req.write;
  uint16_t addr  = req.addr;
  uint8_t  data  = req.data_lo;

  bool io = req.read || req.write;
  bool addr_ext = io && ((req.addr <= 0x7FFF) || (req.addr >= 0xA000 && req.addr < 0xFDFF));
  bool hold_mem = io && (req.addr < 0xFF00);

  // if we set read on phase A, we fail the mem test when we switch from testing OAM to testing ZRAM.
  // these signals are trimmed down as much as they can be without breaking anything.
  // probably they should be fixed to align w cpu clock edges and match cart traces

  // CPU clocks for reference -
  //Tri CPU_PIN_BOWA_ABCDExxx = TRI_HZNP; // top left port PORTD_01: // Blue clock - decoders, alu, some reset stuff
  //Tri CPU_PIN_BEDO_xxxxxFGH = TRI_HZNP; // top left port PORTD_02:
  //Tri CPU_PIN_BEKO_xxxxEFGH = TRI_HZNP; // top left port PORTD_03:
  //Tri CPU_PIN_BUDE_ABCDxxxx = TRI_HZNP; // top left port PORTD_04: 
  //Tri CPU_PIN_BOLO_ABxxEFGH = TRI_HZNP; // top left port PORTD_05: // CPU OEn? Would make sense with AFAS_xxxDxxxx as "WRen" I guess
  //Tri CPU_PIN_BUKE_xxxxxFxx = TRI_HZNP; // top left port PORTD_07: // this is probably the "latch bus data" clock
  //Tri CPU_PIN_BOMA_xxxxxFGH = TRI_HZNP; // top left port PORTD_08: (RESET_CLK)
  //Tri CPU_PIN_BOGA_ABCDExxx = TRI_HZNP; // top left port PORTD_09: - test pad 3
  //Tri EXT_PIN_CLK_ABCDxxxx  = TRI_HZNP; // PIN_75 <- P01.BUDE/BEVA

  if (PHASE_A) {
    bus.preset_addr(addr); bus.preset_data_z();          bus.CPU_PIN_RDp.preset(0);    bus.CPU_PIN_WRp.preset(0);     bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(0);
  }
  if (PHASE_B) {
    bus.preset_addr(addr); bus.preset_data_z();          bus.CPU_PIN_RDp.preset(0);    bus.CPU_PIN_WRp.preset(0);     bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(0);
  }
  if (PHASE_C) {
    bus.preset_addr(addr); bus.preset_data_z();          bus.CPU_PIN_RDp.preset(0);    bus.CPU_PIN_WRp.preset(0);     bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(0);
  }
  if (PHASE_D) {
    bus.preset_addr(addr); bus.preset_data(write, data); bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(write); bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
  }
  if (PHASE_E) {
    bus.preset_addr(addr); bus.preset_data(write, data); bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(0);     bus.CPU_PIN_HOLD_MEM.preset(hold_mem); bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
  }
  if (PHASE_F) {
    bus.preset_addr(addr); bus.preset_data_z();          bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(0);     bus.CPU_PIN_HOLD_MEM.preset(hold_mem); bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
  }
  if (PHASE_G) {
    bus.preset_addr(addr); bus.preset_data_z();          bus.CPU_PIN_RDp.preset(0);    bus.CPU_PIN_WRp.preset(0);     bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(0);
  }
  if (PHASE_H) {
    bus.preset_addr(addr); bus.preset_data_z();          bus.CPU_PIN_RDp.preset(0);    bus.CPU_PIN_WRp.preset(0);     bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(0);
  }

  top.int_reg.CPU_PIN_ACK_VBLANK.preset(0);
  top.int_reg.CPU_PIN_ACK_STAT.preset(0);
  top.int_reg.CPU_PIN_ACK_TIMER.preset(0);
  top.int_reg.CPU_PIN_ACK_SERIAL.preset(0);
  top.int_reg.CPU_PIN_ACK_JOYPAD.preset(0);

  top.ser_reg.SCK_C.preset(0);
  top.ser_reg.SIN_C.preset(0);
}

//-----------------------------------------------------------------------------

void GateBoy::update_ext_bus(int phase) {
  (void)phase;

  top.ext_bus.EXT_PIN_WR_C.preset(top.ext_bus.EXT_PIN_WR_A.qp());
  top.ext_bus.EXT_PIN_RD_C.preset(top.ext_bus.EXT_PIN_RD_A.qp());

  top.ext_bus.EXT_PIN_A00_C.preset(top.ext_bus.EXT_PIN_A00_A.qp());
  top.ext_bus.EXT_PIN_A01_C.preset(top.ext_bus.EXT_PIN_A01_A.qp());
  top.ext_bus.EXT_PIN_A02_C.preset(top.ext_bus.EXT_PIN_A02_A.qp());
  top.ext_bus.EXT_PIN_A03_C.preset(top.ext_bus.EXT_PIN_A03_A.qp());
  top.ext_bus.EXT_PIN_A04_C.preset(top.ext_bus.EXT_PIN_A04_A.qp());
  top.ext_bus.EXT_PIN_A05_C.preset(top.ext_bus.EXT_PIN_A05_A.qp());
  top.ext_bus.EXT_PIN_A06_C.preset(top.ext_bus.EXT_PIN_A06_A.qp());
  top.ext_bus.EXT_PIN_A07_C.preset(top.ext_bus.EXT_PIN_A07_A.qp());
  top.ext_bus.EXT_PIN_A08_C.preset(top.ext_bus.EXT_PIN_A08_A.qp());
  top.ext_bus.EXT_PIN_A09_C.preset(top.ext_bus.EXT_PIN_A09_A.qp());
  top.ext_bus.EXT_PIN_A10_C.preset(top.ext_bus.EXT_PIN_A10_A.qp());
  top.ext_bus.EXT_PIN_A11_C.preset(top.ext_bus.EXT_PIN_A11_A.qp());
  top.ext_bus.EXT_PIN_A12_C.preset(top.ext_bus.EXT_PIN_A12_A.qp());
  top.ext_bus.EXT_PIN_A13_C.preset(top.ext_bus.EXT_PIN_A13_A.qp());
  top.ext_bus.EXT_PIN_A14_C.preset(top.ext_bus.EXT_PIN_A14_A.qp());
  top.ext_bus.EXT_PIN_A15_C.preset(top.ext_bus.EXT_PIN_A15_A.qp());

  if (top.ext_bus.EXT_PIN_WR_A.qp()) {
    uint16_t ext_addr = ~top.ext_bus.get_pin_addr();
    uint8_t ext_data = ~top.ext_bus.get_pin_data_out();
    
    if (ext_addr >= 0 && ext_addr <= 0x7FFF) {
      // FIXME yeah we don't actually want to allow writing to ROM...
      //mem[ext_addr] = ext_data;
      //printf("PHASE %C: EXT WRITE %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xA000 && ext_addr <= 0xBFFF) {
      mem[ext_addr] = ext_data;
      //printf("PHASE %C: EXT WRITE %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xC000 && ext_addr <= 0xDFFF) {
      mem[ext_addr] = ext_data;
      //printf("PHASE %C: EXT WRITE %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xE000 && ext_addr <= 0xFFFF) {
      mem[ext_addr] = ext_data;
      //printf("PHASE %C: EXT WRITE %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
  }

  if (top.ext_bus.EXT_PIN_RD_A.qp()) {
    uint16_t ext_addr = ~top.ext_bus.get_pin_addr();

    if (ext_addr >= 0 && ext_addr <= 0x7FFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.preset_pin_data_in(ext_data);
      //printf("PHASE %C: EXT_READ  %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xA000 && ext_addr <= 0xBFFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.preset_pin_data_in(ext_data);
      //printf("PHASE %C: EXT_READ  %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xC000 && ext_addr <= 0xDFFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.preset_pin_data_in(ext_data);
      //printf("PHASE %C: EXT_READ  %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xE000 && ext_addr <= 0xFFFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.preset_pin_data_in(ext_data);
      //printf("PHASE %C: EXT_READ  %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else {
      //printf("PHASE %C: EXT_READ  %04x (bad addr)\n", 'A' + phase, ext_addr);
      top.ext_bus.preset_pin_data_z();
    }
  }
  else {
    top.ext_bus.preset_pin_data_z();
  }
}

//-----------------------------------------------------------------------------

void GateBoy::update_vrm_bus(int phase) {
  (void)phase;

  top.vram_bus._VRAM_PIN_CS_C.preset(0);
  top.vram_bus._VRAM_PIN_OE_C.preset(0);
  top.vram_bus._VRAM_PIN_WR_C.preset(0);

  uint16_t vram_pin_addr = top.vram_bus.get_pin_addr();
  vram_pin_addr ^= 0b0001111111111111;

  if (top.vram_bus._VRAM_PIN_WR_A.qp()) {
    uint8_t vram_pin_data_out = ~top.vram_bus.get_pin_data_out();
    //printf("vram[%04x] = %d\n", vram_pin_addr, data);
    mem[vram_pin_addr + 0x8000] = vram_pin_data_out;
    //printf("PHASE %C: VRAM WRITE %04x %d\n", 'A' + phase, vram_pin_addr, vram_pin_data_out);
  }

  if (top.vram_bus._VRAM_PIN_OE_A.qp()) {
    uint8_t vram_pin_data_in = mem[vram_pin_addr + 0x8000];
    top.vram_bus.preset_pin_data_in(vram_pin_data_in);

    // io reads are showing up on the vram bus and they probably shouldn't be
    //printf("PHASE %C: VRAM READ %04x %d\n", 'A' + phase, vram_pin_addr, vram_pin_data_in);
  }
  else {
    top.vram_bus.preset_pin_data_z();
  }
}

//-----------------------------------------------------------------------------

void GateBoy::update_oam_bus(int phase) {
  (void)phase;

  uint16_t  oam_addr = top.oam_bus.get_oam_pin_addr();

  uint8_t& oam_data_a = mem[0xFE00 + (oam_addr << 1) + 0];
  uint8_t& oam_data_b = mem[0xFE00 + (oam_addr << 1) + 1];

  if (!top.oam_bus.OAM_PIN_OE.qp()) {
    top.oam_bus.preset_bus_data_a(oam_data_a);
    top.oam_bus.preset_bus_data_b(oam_data_b);
  }

  if (!top.oam_bus.OAM_PIN_WR_A.qp()) oam_data_a = top.oam_bus.get_oam_pin_data_a();
  if (!top.oam_bus.OAM_PIN_WR_B.qp()) oam_data_b = top.oam_bus.get_oam_pin_data_b();
}
//-----------------------------------------------------------------------------

void GateBoy::update_zram_bus(int phase) {
  (void)phase;

  // ZRAM control signals are

  // top.clk_reg.CPU_PIN_BUKE_xxxxxFxx
  // top.TEDO_CPU_RDp();
  // top.TAPU_CPU_WRp_xxxDxxxx()
  // top.cpu_bus.SYKE_FF00_FFFFp()

  // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

  int addr = top.cpu_bus.get_bus_addr();
  bool hit_zram = (addr >= 0xFF80) && (addr <= 0xFFFE);

  if (hit_zram) {
    if (top.TAPU_CPU_WRp_xxxDxxxx()) {
      mem[addr] = top.cpu_bus.get_bus_data();
    }

    if (top.TEDO_CPU_RDp()) {
      top.cpu_bus.preset_data(true, mem[addr]);
    }
  }
}


//-----------------------------------------------------------------------------