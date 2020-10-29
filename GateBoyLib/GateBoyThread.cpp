#include "GateBoyThread.h"

#include "CoreLib/Constants.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

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
}

//-----------------------------------------------------------------------------

void GateBoyThread::reset() {
  printf("Resetting sim\n");
  gb.reset_states();
  gb->reset_cart();
  gb->set_rom(rom_buf.data(), rom_buf.size());
}

//------------------------------------------------------------------------------

void GateBoyThread::thread_main() {
  std::unique_lock<std::mutex> lock(mut, std::defer_lock);

  while(!sig_exit) {
    // Lock and wait until we're unpaused and we have a job in the queue.
    lock.lock();
    while (sig_pause || (command.count == 0 && (cursor_r == cursor_w))) {
      sig_waiting = true;
      cv_thread_pause.notify_one();
      cv_thread_resume.wait(lock);
    }
    sig_waiting = false;

    // Grab the next job off the queue.
    if (command.count == 0 && (cursor_r != cursor_w)) {
      command = ring[cursor_r++];
      if (command.op == CMD_StepPhase) gb.push();
    }

    // Unlock and do the job if we have one.
    lock.unlock();
    switch(command.op) {
    case CMD_Exit:      sig_exit = true;  break;
    case CMD_StepPhase: run_step_phase(); break;
    case CMD_StepPass:  run_step_pass();  break;
    case CMD_StepBack:  run_step_back();  break;
    default:
      printf("BAD COMMAND\n");
      command.count = 0;
      break;
    }
  }
}

//----------------------------------------

void GateBoyThread::pause() {
  if (!pause_count++) {
    std::unique_lock<std::mutex> lock(mut);
    sig_pause = true;
    while (!sig_waiting) cv_thread_pause.wait(lock);
  }
}

//----------------------------------------

void GateBoyThread::resume() {
  if (pause_count && !--pause_count) {
    std::unique_lock<std::mutex> lock(mut);
    sig_pause = false;
    cv_thread_resume.notify_one();
  }
}

//----------------------------------------

void GateBoyThread::post_work(Command c) {
  std::unique_lock<std::mutex> lock(mut);
  if (uint8_t(cursor_w + 1) != cursor_r) {
    ring[cursor_w++] = c;
  }
  cv_thread_resume.notify_one();
}

//----------------------------------------

void GateBoyThread::clear_work() {
  pause();
  cursor_r = 0;
  cursor_w = 0;
  command.count = 0;
  resume();
}

//------------------------------------------------------------------------------

void GateBoyThread::run_step_phase() {
  double time_begin = timestamp();

  for(;command.count && !sig_pause; command.count--) {
    gb->next_phase();
  }

  double time_end = timestamp();
  gb->sim_time += (time_end - time_begin);
}

//----------------------------------------

void GateBoyThread::run_step_pass() {
  for(;command.count && !sig_pause; command.count--) {
    gb->next_pass();
  }
}

//----------------------------------------

void GateBoyThread::run_step_back() {
  for(;command.count && !sig_pause; command.count--) {
    printf("pop\n");
    gb.pop();
  }
}

//------------------------------------------------------------------------------

void GateBoyThread::start() {
  if (main) return;
  main = new std::thread([this] { thread_main(); });
}

//----------------------------------------

void GateBoyThread::stop() {
  if (!main) return;
  clear_work();
  post_work({CMD_Exit, 1});
  while (pause_count) resume();
  main->join();
  delete main;
}

//----------------------------------------

void GateBoyThread::step_phase(int steps) {
  post_work({CMD_StepPhase, steps});
}

void GateBoyThread::step_pass(int steps) {
  post_work({CMD_StepPass, steps});
}

void GateBoyThread::step_back(int steps) {
  post_work({CMD_StepBack, steps});
}

//------------------------------------------------------------------------------

void GateBoyThread::step_sim() {
  //printf("%d %d\n", sem_run.count.load(), sem_stop.count.load());
}

//------------------------------------------------------------------------------

void GateBoyThread::load_raw_dump() {
  printf("Loading raw dump from %s\n", "gateboy.raw.dump");
  gb.reset_states();
  gb->load_dump("gateboy.raw.dump");
  gb->set_rom(rom_buf.data(), rom_buf.size());
}

void GateBoyThread::save_raw_dump() {
  printf("Saving raw dump to %s\n", "gateboy.raw.dump");
  gb->save_dump("gateboy.raw.dump");
}

//------------------------------------------------------------------------------
// Load a standard GB rom

void GateBoyThread::load_rom(const char* filename) {
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

  d("Commands left %d\n",    uint8_t(cursor_w - cursor_r));
  d("Steps left    %d\n",    command.count);
  d("Sim time      %f\n",    gb->sim_time);
  d("Waiting       %d\n",    sig_waiting.load());

  double phase_rate = (gb->phase_total - old_phase_total) / (gb->sim_time - old_sim_time);

  if (gb->sim_time == old_sim_time) {
    phase_rate = 0;
  }

  phase_rate_smooth = (phase_rate_smooth * 0.99) + (phase_rate * 0.01);

  d("Phase rate    %f\n",      phase_rate_smooth);
  old_phase_total = gb->phase_total;
  old_sim_time = gb->sim_time;

  /*
  d("Pass count  %lld\n",    pass_count);
  d("Pass rate   %f\n",      double(pass_count) / sim_time);
  d("Phase count %lld\n",    phase_count);
  d("Phase rate  %f\n",      double(phase_count) / sim_time);
  */

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
  gb->top.clk_reg.dump(d);
  gb->top.joypad.dump(d);
  gb->top.ser_reg.dump(d);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump3(Dumper& d) {
  gb->top.cpu_bus.dump(d);
  gb->top.ext_bus.dump(d);
  gb->top.vram_bus.dump(d, gb->top);
  gb->top.oam_bus.dump(d);
  gb->top.dma_reg.dump(d);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump4(Dumper& d) {
  gb->top.lcd_reg.dump(d, gb->top);
  gb->top.pix_pipe.dump(d, gb->top);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump5(Dumper& d) {
  gb->top.sprite_fetcher.dump(d);
  gb->top.sprite_scanner.dump(d, gb->top);
  gb->top.sprite_store.dump(d);
  gb->top.tile_fetcher.dump(d, gb->top);
}

//------------------------------------------------------------------------------

void GateBoyThread::dump6(Dumper& d) {
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
