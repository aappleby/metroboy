#include "GateBoyThread.h"

#include "CoreLib/Constants.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

void gateboy_main()
{
  std::cout << "countdown:\n";
  while(1) {
    static int i = 0;
    std::cout << i << std::endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));
    i++;
  }
}

//-----------------------------------------------------------------------------

void GateBoyThread::init() {
  printf("GateBoyThread::init()\n");

  load_rom("roms/tetris.gb");

  // regenerate post-bootrom dump
#if 0
  gb->reset_boot();
  rom_buf = load_blob("roms/tetris.gb");
  gb->set_rom(rom_buf.data(), rom_buf.size());
  gb->run_reset_sequence();

  for (int i = 0; i < 8192; i++) {
    gb->vid_ram[i] = (uint8_t)rand();
  }
#endif

#if 0
  // run tiny app
  if (1) {
    std::string app;
    app += "0150:\n";

    app += "ld a, $00\n";
    app += "ldh ($40), a\n";
    app += "ld a, $73\n";
    app += "ld hl, $8000\n";
    app += "ld (hl), a\n";
    app += "ld hl, $809F\n";
    app += "ld (hl), a\n";

    app += "ld hl, $FF80\n";
    app += "ld a, $E0\n"; app += "ld (hl+), a\n";
    app += "ld a, $46\n"; app += "ld (hl+), a\n";
    app += "ld a, $3E\n"; app += "ld (hl+), a\n";
    app += "ld a, $28\n"; app += "ld (hl+), a\n";
    app += "ld a, $3D\n"; app += "ld (hl+), a\n";
    app += "ld a, $20\n"; app += "ld (hl+), a\n";
    app += "ld a, $FD\n"; app += "ld (hl+), a\n";
    app += "ld a, $C9\n"; app += "ld (hl+), a\n";

    app += "ld a, $80\n";
    app += "call $ff80\n";

    app += "ld a, $00\n";
    app += "ld hl, $8000\n";
    app += "add (hl)\n";
    app += "jr -2\n";

    Assembler as;
    as.assemble(app.c_str());
    blob rom = as.link();

    gb->reset_post_bootrom(rom.data(), rom.size());
  }
#endif

  load_flat_dump("roms/LinksAwakening_dog.dump");
  gb->sys_cpu_en = false;

  /*

  {
    memset(gb->oam_ram, 0, 160);
    memset(gb->vid_ram, 0, 8192);
    uint8_t* cursor = gb->vid_ram;
    for (int i = 0; i < 384; i++) {
      *cursor++ = 0b11111111;
      *cursor++ = 0b11111111;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;f
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b11111111;
      *cursor++ = 0b11111111;
    }
  }
  */

  /*
  for (int i = 0; i < 2048; i += 2) {
    gb->vid_ram[i + 0] = 0xFF;
    gb->vid_ram[i + 1] = 0x00;
  }
  memset(&gb->vid_ram[1024 * 2], 0x00, 1024 * 4);

  for (int i = 0; i < 160; i+= 4) {
    gb->oam_ram[i + 0] = 0xFF;
    gb->oam_ram[i + 1] = 0xFF;
  }

  gb->oam_ram[0] = 17;
  gb->oam_ram[1] = 8;
  */


  //gb->top.pix_pipe.set_wx(7);
  //gb->top.pix_pipe.set_wy(16);

  // run rom

  //load_rom   ("roms/mealybug/m3_lcdc_win_en_change_multiple_wx.gb");
  //load_golden("roms/mealybug/m3_lcdc_win_en_change_multiple_wx.bmp");

  //load_rom("microtests/build/dmg/oam_read_l0_d.gb");

  GateBoy::current = gb.state();
}

//-----------------------------------------------------------------------------

void GateBoyThread::reset() {
  sim_guard g(sim_lock);

  printf("Resetting sim\n");
  gb.reset_states();
  gb->reset_cart();
  gb->set_rom(rom_buf.data(), rom_buf.size());
}

//-----------------------------------------------------------------------------

void GateBoyThread::sim_update(int runmode, int stepmode, int step_forward, int step_backward) {
  if (gb->rom_buf != rom_buf.data()) __debugbreak();

  double sim_begin = timestamp();
  int64_t phase_begin = gb->phase_total;

  if (runmode == RUN_FAST) {
    sim_push();
    sim_run(114 * 8 * 8);
  }
  else if (runmode == RUN_STEP && step_forward) {
    sim_push();
    for (int i = 0; i < step_forward; i++) {
      switch(stepmode) {
      case STEP_PASS:  { sim_guard g(sim_lock); gb->next_pass(); break; }
      case STEP_PHASE: { sim_run(1); break; }
      case STEP_CYCLE: { sim_run(8); break; }
      case STEP_LINE:  { sim_run(114 * 8); break; }
      }
    }
  }
  else if (runmode == RUN_STEP && step_backward) {
    for (int i  = 0; i < step_backward; i++) {
      sim_pop();
    }
  }

  double sim_end = timestamp();
  int64_t phase_end = gb->phase_total;
  {
    sim_guard g(sim_lock);
    sim_time = sim_end - sim_begin;
    sim_time_smooth = sim_time_smooth * 0.9 + sim_time * 0.1;
    sim_rate = (phase_end - phase_begin) / sim_time_smooth;
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::sim_push() {
  sim_guard g(sim_lock);
  gb.push();
}

void GateBoyThread::sim_pop() {
  sim_guard g(sim_lock);
  gb.pop();
  GateBoy::current = gb.state();
}

//------------------------------------------------------------------------------

void GateBoyThread::sim_run(int phases) {
  for (int i = 0; i < phases; i++) {
    sim_guard g(sim_lock);
    gb->next_phase();
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::toggle_cpu() {
  sim_guard g(sim_lock);

  if (gb->sys_cpu_en) {
    printf("Disabling CPU\n");
    gb->sys_cpu_en = 0;
  }
  else {
    printf("Enabling CPU\n");
    gb->sys_cpu_en = 1;
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::load_raw_dump() {
  sim_guard g(sim_lock);

  printf("Loading raw dump from %s\n", "gateboy.raw.dump");
  gb.reset_states();
  gb->load_dump("gateboy.raw.dump");
  gb->set_rom(rom_buf.data(), rom_buf.size());
}

void GateBoyThread::save_raw_dump() {
  sim_guard g(sim_lock);

  printf("Saving raw dump to %s\n", "gateboy.raw.dump");
  gb->save_dump("gateboy.raw.dump");
}

//------------------------------------------------------------------------------
// Load a standard GB rom

void GateBoyThread::load_rom(const char* filename) {
  sim_guard g(sim_lock);

  printf("Loading %s\n", filename);
  rom_buf = load_blob(filename);

  gb.reset_states();
  gb->reset_cart();
  gb->set_rom(rom_buf.data(), rom_buf.size());
  gb->phase_total = 0;
  gb->pass_count = 0;
  gb->pass_total = 0;

  printf("Loaded %zd bytes from rom %s\n", rom_buf.size(), filename);
}


//-----------------------------------------------------------------------------
// Load a flat (just raw contents of memory addresses, no individual regs) dump
// and copy it into the various regs and memory chunks.

void GateBoyThread::load_flat_dump(const char* filename) {
  sim_guard g(sim_lock);

  rom_buf = load_blob(filename);

  gb.reset_states();
  gb->reset_cart();
  gb->set_rom(rom_buf.data(), rom_buf.size());

  memcpy(gb->vid_ram,  rom_buf.data() + 0x8000, 8192);
  memcpy(gb->cart_ram, rom_buf.data() + 0xA000, 8192);
  memcpy(gb->ext_ram,  rom_buf.data() + 0xC000, 8192);
  memcpy(gb->oam_ram,  rom_buf.data() + 0xFE00, 256);
  memcpy(gb->zero_ram, rom_buf.data() + 0xFF80, 128);

  gb->dbg_write(ADDR_BGP,  rom_buf[ADDR_BGP]);
  gb->dbg_write(ADDR_OBP0, rom_buf[ADDR_OBP0]);
  gb->dbg_write(ADDR_OBP1, rom_buf[ADDR_OBP1]);
  gb->dbg_write(ADDR_SCY,  rom_buf[ADDR_SCY]);
  gb->dbg_write(ADDR_SCX,  rom_buf[ADDR_SCX]);
  gb->dbg_write(ADDR_WY,   rom_buf[ADDR_WY]);
  gb->dbg_write(ADDR_WX,   rom_buf[ADDR_WX]);

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

  gb->dbg_write(ADDR_LCDC, rom_buf[ADDR_LCDC]);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump1(Dumper& d) {
  sim_guard g(sim_lock);

  d("\002===== Top =====\001\n");

  const char* phases[] = {
    "\002A_______\001",
    "\003_B______\001",
    "\002__C_____\001",
    "\003___D____\001",
    "\002____E___\001",
    "\003_____F__\001",
    "\002______G_\001",
    "\003_______H\001",
  };

  d("phase %s\n", phases[gb->phase_total & 7]);

  d("State count %d\n", gb.state_count());
  size_t state_size = gb.state_size_bytes();
  if (state_size < 1024 * 1024) {
    d("State size  %d K\n", state_size / 1024);
  }
  else {
    d("State size  %d M\n", state_size / (1024 * 1024));
  }
  d("Phase count %d\n",      gb->phase_total);
  d("Pass count  %d\n",      gb->pass_count);
  d("Pass total  %d\n",      gb->pass_total);
  d("Pass avg    %4.2f\n",   float(gb->pass_total) / float(gb->phase_total));
  d("Pass hash   %016llx\n", gb->pass_hash);
  d("Total hash  %016llx\n", gb->total_hash);
  d("BGB cycle   0x%08x\n",  (gb->phase_total / 4) - 0x10000);
  d("Sim clock   %f\n",      double(gb->phase_total) / (4194304.0 * 2));

  d("\n");
  d("dbg_req ");
  dump_req(d, gb->dbg_req);
  d("cpu_req ");
  dump_req(d, gb->cpu_req);
  d("bus_req ");
  dump_req(d, gb->bus_req);
  d("cpu_data_latch %d 0x%02x\n", gb->cpu_data_latch, gb->cpu_data_latch);
  d("\n");

  gb->dump(d);
  d("\n");

  gb->cpu.dump(d);
  gb->top.tim_reg.dump(d);
  gb->top.int_reg.dump(d, gb->top);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump2(Dumper& d) {
  sim_guard g(sim_lock);

  wire CLK = gb->phase_total & 1;
  gb->top.clk_reg.dump(d, CLK);
  gb->top.joypad.dump(d);
  gb->top.ser_reg.dump(d);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump3(Dumper& d) {
  sim_guard g(sim_lock);

  gb->top.cpu_bus.dump(d);
  gb->top.ext_bus.dump(d);
  gb->top.vram_bus.dump(d, gb->top);
  gb->top.oam_bus.dump(d);
  gb->top.dma_reg.dump(d);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump4(Dumper& d) {
  sim_guard g(sim_lock);

  gb->top.lcd_reg.dump(d, gb->top);
  gb->top.pix_pipe.dump(d, gb->top);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump5(Dumper& d) {
  sim_guard g(sim_lock);

  gb->top.sprite_fetcher.dump(d);
  gb->top.sprite_scanner.dump(d, gb->top);
  gb->top.sprite_store.dump(d);
  gb->top.tile_fetcher.dump(d, gb->top);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump6(Dumper& d) {
  sim_guard g(sim_lock);

  d("\002===== Disasm =====\001\n");
  {
    uint16_t pc = gb->cpu.op_addr;
    const uint8_t* code = nullptr;
    uint16_t code_size = 0;
    uint16_t code_base = 0;

    if (!gb->top.bootrom.BOOT_BITn.qp17()) {
      code = DMG_ROM_bin;
      code_size = 256;
      code_base = ADDR_BOOT_ROM_BEGIN;
    }
    else if (pc >= 0x0000 && pc <= 0x7FFF) {
      // FIXME needs to account for mbc1 mem mapping
      code = gb->rom_buf;
      code_size = 32768;
      code_base = ADDR_CART_ROM_BEGIN;
    }
    else if (pc >= 0xFF80 && pc <= 0xFFFE) {
      code = gb->zero_ram;
      code_size = 127;
      code_base = ADDR_ZEROPAGE_BEGIN;
    }

    assembler.disassemble(code, code_size, code_base, pc, 34, d, /*collapse_nops*/ false);
  }
  d("\n");

  d("\002===== OAM =====\001\n");
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", gb->oam_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n");
  d("\002===== ZRAM =====\001\n");
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", gb->zero_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n");
}

//------------------------------------------------------------------------------
