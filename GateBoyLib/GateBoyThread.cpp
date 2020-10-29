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
  const auto& top = gb->top;

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

  d("\002===== Timer =====\001\n");
  d("DIV    : 0x%04x %d\n", top.tim_reg.get_div(),    top.tim_reg.get_div());
  d("TIMA A : 0x%02x %d\n", top.tim_reg.get_tima_a(), top.tim_reg.get_tima_a());
  d("TIMA B : 0x%02x %d\n", top.tim_reg.get_tima_b(), top.tim_reg.get_tima_b());
  d("TMA    : 0x%02x %d\n", top.tim_reg.get_tma(),    top.tim_reg.get_tma());
  d("TAC    : 0x%02x %d\n", top.tim_reg.get_tac(),    top.tim_reg.get_tac());
  d("NYDU_TIMA_D7_DELAY   %c\n", top.tim_reg.NYDU_TIMA_D7_DELAY.c());
  d("MOBA_TIMER_OVERFLOWp %c\n", top.tim_reg.MOBA_TIMER_OVERFLOWp.c());
  d("\n");

  d("\002===== Ints =====\001\n");
  d("IE_D0              %c\n", top.IE_D0.c());
  d("IE_D1              %c\n", top.IE_D1.c());
  d("IE_D2              %c\n", top.IE_D2.c());
  d("IE_D3              %c\n", top.IE_D3.c());
  d("IE_D4              %c\n", top.IE_D4.c());
  d("\n");

  d("LOPE_FF0F_0        %c\n", top.int_reg.LOPE_FF0F_D0p.c());
  d("LALU_FF0F_1        %c\n", top.int_reg.LALU_FF0F_D1p.c());
  d("NYBO_FF0F_2        %c\n", top.int_reg.NYBO_FF0F_D2p.c());
  d("UBUL_FF0F_3        %c\n", top.int_reg.UBUL_FF0F_D3p.c());
  d("ULAK_FF0F_4        %c\n", top.int_reg.ULAK_FF0F_D4p.c());
  d("\n");
  d("MATY_FF0F_L0p      %c\n", top.int_reg.MATY_FF0F_L0p.c());
  d("MOPO_FF0F_L1p      %c\n", top.int_reg.MOPO_FF0F_L1p.c());
  d("PAVY_FF0F_L2p      %c\n", top.int_reg.PAVY_FF0F_L2p.c());
  d("NEJY_FF0F_L3p      %c\n", top.int_reg.NEJY_FF0F_L3p.c());
  d("NUTY_FF0F_L4p      %c\n", top.int_reg.NUTY_FF0F_L4p.c());
  d("\n");
  d("PIN_CPU_INT_VBLANK %c\n", top.int_reg.PIN_CPU_INT_VBLANK.c());
  d("PIN_CPU_INT_STAT   %c\n", top.int_reg.PIN_CPU_INT_STAT.c());
  d("PIN_CPU_INT_TIMER  %c\n", top.int_reg.PIN_CPU_INT_TIMER.c());
  d("PIN_CPU_INT_SERIAL %c\n", top.int_reg.PIN_CPU_INT_SERIAL.c());
  d("PIN_CPU_INT_JOYPAD %c\n", top.int_reg.PIN_CPU_INT_JOYPAD.c());
  d("\n");
  d("PIN_CPU_ACK_VBLANK %c\n", top.int_reg.PIN_CPU_ACK_VBLANK.c());
  d("PIN_CPU_ACK_STAT   %c\n", top.int_reg.PIN_CPU_ACK_STAT.c());
  d("PIN_CPU_ACK_TIMER  %c\n", top.int_reg.PIN_CPU_ACK_TIMER.c());
  d("PIN_CPU_ACK_SERIAL %c\n", top.int_reg.PIN_CPU_ACK_SERIAL.c());
  d("PIN_CPU_ACK_JOYPAD %c\n", top.int_reg.PIN_CPU_ACK_JOYPAD.c());
  d("\n");
}

//------------------------------------------------------------------------------

void GateBoyThread::dump2(Dumper& d) {
  const auto& top = gb->top;

  gb->top.clk_reg.dump(d);

  d("\002===== Joypad =====\001\n");
  d("PIN_JOY_P10      %c\n", top.joypad.PIN_JOY_P10.c());
  d("PIN_JOY_P11      %c\n", top.joypad.PIN_JOY_P11.c());
  d("PIN_JOY_P12      %c\n", top.joypad.PIN_JOY_P12.c());
  d("PIN_JOY_P13      %c\n", top.joypad.PIN_JOY_P13.c());
  d("PIN_JOY_P14      %c\n", top.joypad.PIN_JOY_P14.c());
  d("PIN_JOY_P15      %c\n", top.joypad.PIN_JOY_P15.c());
  d("PIN_CPU_WAKE     %c\n", top.joypad.PIN_CPU_WAKE .c());
  d("AWOB_WAKE_CPU    %c\n", top.joypad.AWOB_WAKE_CPU.c());
  d("BATU_JP_GLITCH0  %c\n", top.joypad.BATU_JP_GLITCH0.c());
  d("ACEF_JP_GLITCH1  %c\n", top.joypad.ACEF_JP_GLITCH1.c());
  d("AGEM_JP_GLITCH2  %c\n", top.joypad.AGEM_JP_GLITCH2.c());
  d("APUG_JP_GLITCH3  %c\n", top.joypad.APUG_JP_GLITCH3.c());
  d("JUTE_JOYP_RA     %c\n", top.joypad.JUTE_JOYP_RA.c());
  d("KECY_JOYP_LB     %c\n", top.joypad.KECY_JOYP_LB.c());
  d("JALE_JOYP_UC     %c\n", top.joypad.JALE_JOYP_UC.c());
  d("KYME_JOYP_DS     %c\n", top.joypad.KYME_JOYP_DS.c());
  d("KELY_JOYP_UDLR   %c\n", top.joypad.KELY_JOYP_UDLR.c());
  d("COFY_JOYP_ABCS   %c\n", top.joypad.COFY_JOYP_ABCS.c());
  d("KUKO_DBG_FF00_D6 %c\n", top.joypad.KUKO_DBG_FF00_D6.c());
  d("KERU_DBG_FF00_D7 %c\n", top.joypad.KERU_DBG_FF00_D7.c());
  d("KEVU_JOYP_L0     %c\n", top.joypad.KEVU_JOYP_L0.c());
  d("KAPA_JOYP_L1     %c\n", top.joypad.KAPA_JOYP_L1.c());
  d("KEJA_JOYP_L2     %c\n", top.joypad.KEJA_JOYP_L2.c());
  d("KOLO_JOYP_L3     %c\n", top.joypad.KOLO_JOYP_L3.c());
  d("\n");

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
  const auto& top = gb->top;

  d("\002===== LCD =====\001\n");
  d("PIX COUNT : %03d\n", top.pix_pipe.get_pix_count());
  d("LCD X     : %03d\n", top.lcd_reg.get_lx());
  d("LCD Y     : %03d\n", top.lcd_reg.get_ly());
  d("LYC       : %03d\n", top.lcd_reg.get_lyc());
  d("\n");

  d("PIN_LCD_CLOCK   : "); top.PIN_LCD_CLOCK.dump(d); d("\n");
  d("PIN_LCD_HSYNC   : "); top.PIN_LCD_HSYNC.dump(d); d("\n");
  d("PIN_LCD_VSYNC   : "); top.PIN_LCD_VSYNC.dump(d); d("\n");
  d("PIN_LCD_DATA1   : "); top.PIN_LCD_DATA1.dump(d); d("\n");
  d("PIN_LCD_DATA0   : "); top.PIN_LCD_DATA0.dump(d); d("\n");
  d("PIN_LCD_CNTRL   : "); top.PIN_LCD_CNTRL.dump(d); d("\n");
  d("PIN_LCD_DATALCH : "); top.PIN_LCD_LATCH.dump(d); d("\n");
  d("PIN_LCD_ALTSIGL   : "); top.PIN_LCD_FLIPS.dump(d); d("\n");
  d("\n");

  d("CATU_LINE_P000      %c\n", top.lcd_reg.CATU_LINE_P000.c());
  d("NYPE_LINE_P002      %c\n", top.lcd_reg.NYPE_LINE_P002.c());
  d("ANEL_LINE_P002      %c\n", top.lcd_reg.ANEL_LINE_P002.c());
  d("RUTU_LINE_P910      %c\n", top.lcd_reg.RUTU_LINE_P910.c());
  d("MYTA_LINE_153p      %c\n", top.lcd_reg.MYTA_LINE_153p     .c());
  d("POPU_IN_VBLANKp     %c\n", top.lcd_reg.POPU_IN_VBLANKp    .c());
  d("ROPO_LY_MATCH_SYNCp %c\n", top.lcd_reg.ROPO_LY_MATCH_SYNCp.c());
  d("\n");

  d("\002===== Pix Pipe =====\001\n");

  d.dump_reg("PIX COUNT",
    top.pix_pipe.XEHO_X0p.qp17(), top.pix_pipe.SAVY_X1p.qp17(), top.pix_pipe.XODU_X2p.qp17(), top.pix_pipe.XYDO_X3p.qp17(),
    top.pix_pipe.TUHU_X4p.qp17(), top.pix_pipe.TUKY_X5p.qp17(), top.pix_pipe.TAKO_X6p.qp17(), top.pix_pipe.SYBE_X7p.qp17());


  d.dump_reg("FF40 LCDC",
    top.pix_pipe.VYXE_LCDC_BGENn.qn08(),
    top.pix_pipe.XYLO_LCDC_SPENn.qn08(),
    top.pix_pipe.XYMO_LCDC_SPSIZEn.qn08(),
    top.pix_pipe.XAFO_LCDC_BGMAPn.qn08(),
    top.pix_pipe.WEXU_LCDC_BGTILEn.qn08(),
    top.pix_pipe.WYMO_LCDC_WINENn.qn08(),
    top.pix_pipe.WOKY_LCDC_WINMAPn.qn08(),
    top.pix_pipe.XONA_LCDC_LCDENn.qn08());

  d.dump_reg("FF41 STAT",
    !top.pix_pipe.SADU_STAT_MODE0n,
    !top.pix_pipe.XATY_STAT_MODE1n,
    top.pix_pipe.RUPO_LYC_MATCH_LATCHn.qn03(),
    top.pix_pipe.ROXE_STAT_HBI_ENn.qn08(),
    top.pix_pipe.RUFO_STAT_VBI_ENn.qn08(),
    top.pix_pipe.REFE_STAT_OAI_ENn.qn08(),
    top.pix_pipe.RUGU_STAT_LYI_ENn.qn08(),
    1
  );

  d.dump_reg("FF42 SCY",
    top.pix_pipe.GAVE_SCY0n.qn08(),
    top.pix_pipe.FYMO_SCY1n.qn08(),
    top.pix_pipe.FEZU_SCY2n.qn08(),
    top.pix_pipe.FUJO_SCY3n.qn08(),
    top.pix_pipe.DEDE_SCY4n.qn08(),
    top.pix_pipe.FOTY_SCY5n.qn08(),
    top.pix_pipe.FOHA_SCY6n.qn08(),
    top.pix_pipe.FUNY_SCY7n.qn08()
  );

  d.dump_reg("FF43 SCX",
    top.pix_pipe.DATY_SCX0n.qn08(),
    top.pix_pipe.DUZU_SCX1n.qn08(),
    top.pix_pipe.CYXU_SCX2n.qn08(),
    top.pix_pipe.GUBO_SCX3n.qn08(),
    top.pix_pipe.BEMY_SCX4n.qn08(),
    top.pix_pipe.CUZY_SCX5n.qn08(),
    top.pix_pipe.CABU_SCX6n.qn08(),
    top.pix_pipe.BAKE_SCX7n.qn08()
  );

  d.dump_reg("FF47 BGP",
    top.pix_pipe.PAVO_BGP_D0n.qn07(),
    top.pix_pipe.NUSY_BGP_D1n.qn07(),
    top.pix_pipe.PYLU_BGP_D2n.qn07(),
    top.pix_pipe.MAXY_BGP_D3n.qn07(),
    top.pix_pipe.MUKE_BGP_D4n.qn07(),
    top.pix_pipe.MORU_BGP_D5n.qn07(),
    top.pix_pipe.MOGY_BGP_D6n.qn07(),
    top.pix_pipe.MENA_BGP_D7n.qn07()
  );

  d.dump_reg("FF48 OBP0",
    top.pix_pipe.XUFU_OBP0_D0n.qn07(),
    top.pix_pipe.XUKY_OBP0_D1n.qn07(),
    top.pix_pipe.XOVA_OBP0_D2n.qn07(),
    top.pix_pipe.XALO_OBP0_D3n.qn07(),
    top.pix_pipe.XERU_OBP0_D4n.qn07(),
    top.pix_pipe.XYZE_OBP0_D5n.qn07(),
    top.pix_pipe.XUPO_OBP0_D6n.qn07(),
    top.pix_pipe.XANA_OBP0_D7n.qn07()
  );

  d.dump_reg("FF49 OBP1",
    top.pix_pipe.MOXY_OBP1_D0n.qn07(),
    top.pix_pipe.LAWO_OBP1_D1n.qn07(),
    top.pix_pipe.MOSA_OBP1_D2n.qn07(),
    top.pix_pipe.LOSE_OBP1_D3n.qn07(),
    top.pix_pipe.LUNE_OBP1_D4n.qn07(),
    top.pix_pipe.LUGU_OBP1_D5n.qn07(),
    top.pix_pipe.LEPU_OBP1_D6n.qn07(),
    top.pix_pipe.LUXO_OBP1_D7n.qn07()
  );

  d.dump_reg("FF4A WY",
    top.pix_pipe.NESO_WY0n.qn08(),
    top.pix_pipe.NYRO_WY1n.qn08(),
    top.pix_pipe.NAGA_WY2n.qn08(),
    top.pix_pipe.MELA_WY3n.qn08(),
    top.pix_pipe.NULO_WY4n.qn08(),
    top.pix_pipe.NENE_WY5n.qn08(),
    top.pix_pipe.NUKA_WY6n.qn08(),
    top.pix_pipe.NAFU_WY7n.qn08()
  );

  d.dump_reg("FF4B WX",
    top.pix_pipe.MYPA_WX0n.qn08(),
    top.pix_pipe.NOFE_WX1n.qn08(),
    top.pix_pipe.NOKE_WX2n.qn08(),
    top.pix_pipe.MEBY_WX3n.qn08(),
    top.pix_pipe.MYPU_WX4n.qn08(),
    top.pix_pipe.MYCE_WX5n.qn08(),
    top.pix_pipe.MUVO_WX6n.qn08(),
    top.pix_pipe.NUKU_WX7n.qn08()
  );

  d.dump_reg("BG_PIPE_A",
    top.pix_pipe.MYDE_BG_PIPE_A0.qp16(), top.pix_pipe.NOZO_BG_PIPE_A1.qp16(), top.pix_pipe.MOJU_BG_PIPE_A2.qp16(), top.pix_pipe.MACU_BG_PIPE_A3.qp16(),
    top.pix_pipe.NEPO_BG_PIPE_A4.qp16(), top.pix_pipe.MODU_BG_PIPE_A5.qp16(), top.pix_pipe.NEDA_BG_PIPE_A6.qp16(), top.pix_pipe.PYBO_BG_PIPE_A7.qp16());

  d.dump_reg("BG_PIPE_B",
    top.pix_pipe.TOMY_BG_PIPE_B0.qp16(), top.pix_pipe.TACA_BG_PIPE_B1.qp16(), top.pix_pipe.SADY_BG_PIPE_B2.qp16(), top.pix_pipe.RYSA_BG_PIPE_B3.qp16(),
    top.pix_pipe.SOBO_BG_PIPE_B4.qp16(), top.pix_pipe.SETU_BG_PIPE_B5.qp16(), top.pix_pipe.RALU_BG_PIPE_B6.qp16(), top.pix_pipe.SOHU_BG_PIPE_B7.qp16());

  d.dump_reg("SPR_PIPE_A",
    top.pix_pipe.NYLU_SPR_PIPE_B0.qp16(), top.pix_pipe.PEFU_SPR_PIPE_B1.qp16(), top.pix_pipe.NATY_SPR_PIPE_B2.qp16(), top.pix_pipe.PYJO_SPR_PIPE_B3.qp16(),
    top.pix_pipe.VARE_SPR_PIPE_B4.qp16(), top.pix_pipe.WEBA_SPR_PIPE_B5.qp16(), top.pix_pipe.VANU_SPR_PIPE_B6.qp16(), top.pix_pipe.VUPY_SPR_PIPE_B7.qp16());

  d.dump_reg("SPR_PIPE_B",
    top.pix_pipe.NURO_SPR_PIPE_A0.qp16(), top.pix_pipe.MASO_SPR_PIPE_A1.qp16(), top.pix_pipe.LEFE_SPR_PIPE_A2.qp16(), top.pix_pipe.LESU_SPR_PIPE_A3.qp16(),
    top.pix_pipe.WYHO_SPR_PIPE_A4.qp16(), top.pix_pipe.WORA_SPR_PIPE_A5.qp16(), top.pix_pipe.VAFO_SPR_PIPE_A6.qp16(), top.pix_pipe.WUFY_SPR_PIPE_A7.qp16());

  d.dump_reg("PAL_PIPE",
    top.pix_pipe.RUGO_PAL_PIPE_0.qp16(), top.pix_pipe.SATA_PAL_PIPE_1.qp16(), top.pix_pipe.ROSA_PAL_PIPE_2.qp16(), top.pix_pipe.SOMY_PAL_PIPE_3.qp16(),
    top.pix_pipe.PALU_PAL_PIPE_4.qp16(), top.pix_pipe.NUKE_PAL_PIPE_5.qp16(), top.pix_pipe.MODA_PAL_PIPE_6.qp16(), top.pix_pipe.LYME_PAL_PIPE_7.qp16()
  );

  d.dump_reg("MASK_PIPE",
    top.pix_pipe.VEZO_MASK_PIPE_0.qp16(), top.pix_pipe.WURU_MASK_PIPE_1.qp16(), top.pix_pipe.VOSA_MASK_PIPE_2.qp16(), top.pix_pipe.WYFU_MASK_PIPE_3.qp16(),
    top.pix_pipe.XETE_MASK_PIPE_4.qp16(), top.pix_pipe.WODA_MASK_PIPE_5.qp16(), top.pix_pipe.VUMO_MASK_PIPE_6.qp16(), top.pix_pipe.VAVA_MASK_PIPE_7.qp16()
  );

  d.dump_reg("WIN X",
    0,
    0,
    0,
    top.pix_pipe.WYKA_WIN_X3.qp17(),
    top.pix_pipe.WODY_WIN_X4.qp17(),
    top.pix_pipe.WOBO_WIN_X5.qp17(),
    top.pix_pipe.WYKO_WIN_X6.qp17(),
    top.pix_pipe.XOLO_WIN_X7.qp17()
  );

  d.dump_reg("WIN Y",
    top.pix_pipe.VYNO_WIN_Y0.qp17(),
    top.pix_pipe.VUJO_WIN_Y1.qp17(),
    top.pix_pipe.VYMU_WIN_Y2.qp17(),
    top.pix_pipe.TUFU_WIN_Y3.qp17(),
    top.pix_pipe.TAXA_WIN_Y4.qp17(),
    top.pix_pipe.TOZO_WIN_Y5.qp17(),
    top.pix_pipe.TATE_WIN_Y6.qp17(),
    top.pix_pipe.TEKE_WIN_Y7.qp17()
  );

  d("\n");

  d("ROXY_FINE_SCROLL_DONEn %c\n", top.pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.c());
  d("RYKU_FINE_CNT0         %c\n", top.pix_pipe.RYKU_FINE_CNT0.c());
  d("ROGA_FINE_CNT1         %c\n", top.pix_pipe.ROGA_FINE_CNT1.c());
  d("RUBU_FINE_CNT2         %c\n", top.pix_pipe.RUBU_FINE_CNT2.c());
  d("XYMU_RENDERINGp        %c\n", top.pix_pipe.XYMU_RENDERINGn.c());
  d("RUPO_LYC_MATCH_LATCHn  %c\n", top.pix_pipe.RUPO_LYC_MATCH_LATCHn.c());
  d("WUSA_LCD_CLOCK_GATE    %c\n", top.pix_pipe.WUSA_LCD_CLOCK_GATE.c());
  d("VOGA_RENDER_DONE_SYNC  %c\n", top.pix_pipe.VOGA_HBLANKp.c());
  d("PUXA_FINE_MATCH_A      %c\n", top.pix_pipe.PUXA_SCX_FINE_MATCH_A.c());
  d("NYZE_FINE_MATCH_B      %c\n", top.pix_pipe.NYZE_SCX_FINE_MATCH_B.c());
  d("PAHO_X_8_SYNC          %c\n", top.pix_pipe.PAHO_X_8_SYNC.c());
  d("POFY_HSYNCp            %c\n", top.pix_pipe.POFY.c());
  d("VOTY_INT_STATp         %c\n", top.pix_pipe.VOTY_INT_STATp.c());
  d("\n");


  /*
  d("\002===== Window =====\001\n");

  d("PYNU_WIN_MODE_A       : %c\n", PYNU_WIN_MODE_A.c());
  d("RYDY_WIN_FIRST_TILE_A : %c\n", RYDY_WIN_FIRST_TILE_A.c());
  d("NOPA_WIN_MODE_B       : %c\n", NOPA_WIN_MODE_B.c());
  d("SOVY_WIN_FIRST_TILE_B : %c\n", SOVY_WIN_FIRST_TILE_B.c());
  d("REJO_WY_MATCH_LATCH   : %c\n", REJO_WY_MATCH_LATCH.c());
  d("SARY_WY_MATCH         : %c\n", SARY_WY_MATCH.c());
  d("RYFA_FETCHn_A         : %c\n", RYFA_FETCHn_A.c());
  d("RENE_FETCHn_B         : %c\n", RENE_FETCHn_B.c());
  d("PYCO_WX_MATCH_A       : %c\n", PYCO_WX_MATCH_A.c());
  d("NUNU_WX_MATCH_B       : %c\n", NUNU_WX_MATCH_B.c());
  d("\n");
  */
}

//------------------------------------------------------------------------------

void GateBoyThread::dump5(Dumper& d) {
  const auto& top = gb->top;

  d("\002===== SpriteFetch =====\001\n");
  d("SOBU_SFETCH_REQp     %c\n", top.sprite_fetcher.SOBU_SFETCH_REQp    .c());
  d("SUDA_SFETCH_REQp     %c\n", top.sprite_fetcher.SUDA_SFETCH_REQp    .c());
  d("\n");
  d("TAKA_SFETCH_RUNNINGp %c\n", top.sprite_fetcher.TAKA_SFETCH_RUNNINGp.c());
  d("\n");
  d("TOXE_SFETCH_S0       %c\n", top.sprite_fetcher.TOXE_SFETCH_S0      .c());
  d("TYFO_SFETCH_S0_D1    %c\n", top.sprite_fetcher.TYFO_SFETCH_S0_D1   .c());
  d("\n");
  d("TULY_SFETCH_S1       %c\n", top.sprite_fetcher.TULY_SFETCH_S1      .c());
  d("TOBU_SFETCH_S1_D2    %c\n", top.sprite_fetcher.TOBU_SFETCH_S1_D2   .c());
  d("VONU_SFETCH_S1_D4    %c\n", top.sprite_fetcher.VONU_SFETCH_S1_D4   .c());
  d("SEBA_SFETCH_S1_D5    %c\n", top.sprite_fetcher.SEBA_SFETCH_S1_D5   .c());
  d("\n");
  d("TESE_SFETCH_S2       %c\n", top.sprite_fetcher.TESE_SFETCH_S2      .c());
  d("\n");

  d("\002===== SpriteScan =====\001\n");

  d("SCAN INDEX       %02d\n", pack_p(top.sprite_scanner.YFEL_SCAN0.qp17(), top.sprite_scanner.WEWY_SCAN1.qp17(), top.sprite_scanner.GOSO_SCAN2.qp17(), top.sprite_scanner.ELYN_SCAN3.qp17(),
                                      top.sprite_scanner.FAHA_SCAN4.qp17(), top.sprite_scanner.FONY_SCAN5.qp17(), 0, 0));
  d("SPRITE INDEX     %02d\n", pack_p(top.sprite_scanner.XADU_SPRITE_IDX0p.qn12(), top.sprite_scanner.XEDY_SPRITE_IDX1p.qn12(), top.sprite_scanner.ZUZE_SPRITE_IDX2p.qn12(), top.sprite_scanner.XOBE_SPRITE_IDX3p.qn12(),
                                      top.sprite_scanner.YDUF_SPRITE_IDX4p.qn12(), top.sprite_scanner.XECU_SPRITE_IDX5p.qn12(), 0, 0));

  d("BESU_SCANNINGp   %c\n", top.sprite_scanner.BESU_SCANNINGp  .c());
  d("CENO_SCANNINGp   %c\n", top.sprite_scanner.CENO_SCANNINGp  .c());
  d("BYBA_SCAN_DONE_A %c\n", top.sprite_scanner.BYBA_SCAN_DONE_A.c());
  d("DOBA_SCAN_DONE_B %c\n", top.sprite_scanner.DOBA_SCAN_DONE_B.c());
  d("\n");
  d("LCD Y      %03d\n", top.lcd_reg.get_ly());
  d("Y DIFF     %03d\n", pack_p(top.sprite_scanner.DEGE_SPRITE_DELTA0, top.sprite_scanner.DABY_SPRITE_DELTA1, top.sprite_scanner.DABU_SPRITE_DELTA2, top.sprite_scanner.GYSA_SPRITE_DELTA3,
                                top.sprite_scanner.GACE_SPRITE_DELTA4, top.sprite_scanner.GUVU_SPRITE_DELTA5, top.sprite_scanner.GYDA_SPRITE_DELTA6, top.sprite_scanner.GEWY_SPRITE_DELTA7));
  d("CARE_STORE_ENp_ABxxEFxx %d\n", (wire)top.sprite_scanner.CARE_STORE_ENp_ABxxEFxx);

  d("\n");


  d("\002===== SpriteStore =====\001\n");
  d("DEZY_STORE_ENn %c\n", top.sprite_store.DEZY_STORE_ENn.c());
  d("FEPO_STORE_MATCHp %d\n", wire(top.sprite_store.FEPO_STORE_MATCHp));

  d("STORE MATCH %c%c%c%c%c%c%c%c%c%c\n",
    top.sprite_store.YDUG_STORE0_MATCHn.c(), top.sprite_store.DYDU_STORE1_MATCHn.c(), top.sprite_store.DEGO_STORE2_MATCHn.c(), top.sprite_store.YLOZ_STORE3_MATCHn.c(), top.sprite_store.XAGE_STORE4_MATCHn.c(),
    top.sprite_store.EGOM_STORE5_MATCHn.c(), top.sprite_store.YBEZ_STORE6_MATCHn.c(), top.sprite_store.DYKA_STORE7_MATCHn.c(), top.sprite_store.EFYL_STORE8_MATCHn.c(), top.sprite_store.YGEM_STORE9_MATCHn.c());

  d("SPRITE COUNT %02d\n", pack_p(top.sprite_store.BESE_SPRITE_COUNT0.qp17(), top.sprite_store.CUXY_SPRITE_COUNT1.qp17(), top.sprite_store.BEGO_SPRITE_COUNT2.qp17(), top.sprite_store.DYBE_SPRITE_COUNT3.qp17()));

  int spr_tri_idx = pack_p(top.sprite_store.SPR_TRI_I0p.qp(), top.sprite_store.SPR_TRI_I1p.qp(), top.sprite_store.SPR_TRI_I2p.qp(), top.sprite_store.SPR_TRI_I3p.qp(), top.sprite_store.SPR_TRI_I4p.qp(), top.sprite_store.SPR_TRI_I5p.qp(), 0, 0);
  d("SPR_TRI_IDX  = %2d %c%c%c%c%c%c\n",
    spr_tri_idx,
    top.sprite_store.SPR_TRI_I5p.c(), top.sprite_store.SPR_TRI_I4p.c(), top.sprite_store.SPR_TRI_I3p.c(), top.sprite_store.SPR_TRI_I2p.c(),
    top.sprite_store.SPR_TRI_I1p.c(), top.sprite_store.SPR_TRI_I0p.c());

  int spr_tri_line = pack_p(top.sprite_store.SPR_TRI_L0.qp(), top.sprite_store.SPR_TRI_L1.qp(), top.sprite_store.SPR_TRI_L2.qp(), top.sprite_store.SPR_TRI_L3.qp());
  d("SPR_TRI_LINE = %2d %c%c%c%c\n",
    spr_tri_line,
    top.sprite_store.SPR_TRI_L0.c(), top.sprite_store.SPR_TRI_L1.c(), top.sprite_store.SPR_TRI_L2.c(), top.sprite_store.SPR_TRI_L3.c());

  d("STORE0 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.YDUG_STORE0_MATCHn,
    top.sprite_store.EBOJ_STORE0_RSTp.qp17(),
    pack_n(top.sprite_store.YGUS_STORE0_I0n.qp08(), top.sprite_store.YSOK_STORE0_I1n.qp08(), top.sprite_store.YZEP_STORE0_I2n.qp08(), top.sprite_store.WYTE_STORE0_I3n.qp08(), top.sprite_store.ZONY_STORE0_I4n.qp08(), top.sprite_store.YWAK_STORE0_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.GYHO_STORE0_L0n.qp08(), top.sprite_store.CUFO_STORE0_L1n.qp08(), top.sprite_store.BOZU_STORE0_L2n.qp08(), top.sprite_store.FYHY_STORE0_L3n.qp08()),
    pack_n(top.sprite_store.XEPE_STORE0_X0p.qn08(), top.sprite_store.YLAH_STORE0_X1p.qn08(), top.sprite_store.ZOLA_STORE0_X2p.qn08(), top.sprite_store.ZULU_STORE0_X3p.qn08(), top.sprite_store.WELO_STORE0_X4p.qn08(), top.sprite_store.XUNY_STORE0_X5p.qn08(), top.sprite_store.WOTE_STORE0_X6p.qn08(), top.sprite_store.XAKO_STORE0_X7p.qn08())
  );

  d("STORE1 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.DYDU_STORE1_MATCHn,
    top.sprite_store.CEDY_STORE1_RSTp.qp17(),
    pack_n(top.sprite_store.CADU_STORE1_I0n.qp08(), top.sprite_store.CEBO_STORE1_I1n.qp08(), top.sprite_store.CUFA_STORE1_I2n.qp08(), top.sprite_store.COMA_STORE1_I3n.qp08(), top.sprite_store.CUZA_STORE1_I4n.qp08(), top.sprite_store.CAJY_STORE1_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.AMES_STORE1_L0n.qp08(), top.sprite_store.AROF_STORE1_L1n.qp08(), top.sprite_store.ABOP_STORE1_L2n.qp08(), top.sprite_store.ABUG_STORE1_L3n.qp08()),
    pack_n(top.sprite_store.DANY_STORE1_X0p.qn08(), top.sprite_store.DUKO_STORE1_X1p.qn08(), top.sprite_store.DESU_STORE1_X2p.qn08(), top.sprite_store.DAZO_STORE1_X3p.qn08(), top.sprite_store.DAKE_STORE1_X4p.qn08(), top.sprite_store.CESO_STORE1_X5p.qn08(), top.sprite_store.DYFU_STORE1_X6p.qn08(), top.sprite_store.CUSY_STORE1_X7p.qn08())
  );

  d("STORE2 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.DEGO_STORE2_MATCHn,
    top.sprite_store.EGAV_STORE2_RSTp.qp17(),
    pack_n(top.sprite_store.BUHE_STORE2_I0n.qp08(), top.sprite_store.BYHU_STORE2_I1n.qp08(), top.sprite_store.BECA_STORE2_I2n.qp08(), top.sprite_store.BULU_STORE2_I3n.qp08(), top.sprite_store.BUNA_STORE2_I4n.qp08(), top.sprite_store.BOXA_STORE2_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.YLOV_STORE2_L0n.qp08(), top.sprite_store.XOSY_STORE2_L1n.qp08(), top.sprite_store.XAZY_STORE2_L2n.qp08(), top.sprite_store.YKUK_STORE2_L3n.qp08()),
    pack_n(top.sprite_store.FOKA_STORE2_X0p.qn08(), top.sprite_store.FYTY_STORE2_X1p.qn08(), top.sprite_store.FUBY_STORE2_X2p.qn08(), top.sprite_store.GOXU_STORE2_X3p.qn08(), top.sprite_store.DUHY_STORE2_X4p.qn08(), top.sprite_store.EJUF_STORE2_X5p.qn08(), top.sprite_store.ENOR_STORE2_X6p.qn08(), top.sprite_store.DEPY_STORE2_X7p.qn08())
  );

  d("STORE3 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.YLOZ_STORE3_MATCHn,
    top.sprite_store.GOTA_STORE3_RSTp.qp17(),
    pack_n(top.sprite_store.DEVY_STORE3_I0n.qp08(), top.sprite_store.DESE_STORE3_I1n.qp08(), top.sprite_store.DUNY_STORE3_I2n.qp08(), top.sprite_store.DUHA_STORE3_I3n.qp08(), top.sprite_store.DEBA_STORE3_I4n.qp08(), top.sprite_store.DAFU_STORE3_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.ZURO_STORE3_L0n.qp08(), top.sprite_store.ZYLU_STORE3_L1n.qp08(), top.sprite_store.ZENE_STORE3_L2n.qp08(), top.sprite_store.ZURY_STORE3_L3n.qp08()),
    pack_n(top.sprite_store.XOLY_STORE3_X0p.qn08(), top.sprite_store.XYBA_STORE3_X1p.qn08(), top.sprite_store.XABE_STORE3_X2p.qn08(), top.sprite_store.XEKA_STORE3_X3p.qn08(), top.sprite_store.XOMY_STORE3_X4p.qn08(), top.sprite_store.WUHA_STORE3_X5p.qn08(), top.sprite_store.WYNA_STORE3_X6p.qn08(), top.sprite_store.WECO_STORE3_X7p.qn08())
  );

  d("STORE4 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.XAGE_STORE4_MATCHn,
    top.sprite_store.XUDY_STORE4_RSTp.qp17(),
    pack_n(top.sprite_store.XAVE_STORE4_I0n.qp08(), top.sprite_store.XEFE_STORE4_I1n.qp08(), top.sprite_store.WANU_STORE4_I2n.qp08(), top.sprite_store.XABO_STORE4_I3n.qp08(), top.sprite_store.XEGE_STORE4_I4n.qp08(), top.sprite_store.XYNU_STORE4_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.CAPO_STORE4_L0n.qp08(), top.sprite_store.CAJU_STORE4_L1n.qp08(), top.sprite_store.CONO_STORE4_L2n.qp08(), top.sprite_store.CUMU_STORE4_L3n.qp08()),
    pack_n(top.sprite_store.WEDU_STORE4_X0p.qn08(), top.sprite_store.YGAJ_STORE4_X1p.qn08(), top.sprite_store.ZYJO_STORE4_X2p.qn08(), top.sprite_store.XURY_STORE4_X3p.qn08(), top.sprite_store.YBED_STORE4_X4p.qn08(), top.sprite_store.ZALA_STORE4_X5p.qn08(), top.sprite_store.WYDE_STORE4_X6p.qn08(), top.sprite_store.XEPA_STORE4_X7p.qn08())
  );

  d("STORE5 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.EGOM_STORE5_MATCHn,
    top.sprite_store.WAFY_STORE5_RSTp.qp17(),
    pack_n(top.sprite_store.EKOP_STORE5_I0n.qp08(), top.sprite_store.ETYM_STORE5_I1n.qp08(), top.sprite_store.GORU_STORE5_I2n.qp08(), top.sprite_store.EBEX_STORE5_I3n.qp08(), top.sprite_store.ETAV_STORE5_I4n.qp08(), top.sprite_store.EKAP_STORE5_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.ACEP_STORE5_L0n.qp08(), top.sprite_store.ABEG_STORE5_L1n.qp08(), top.sprite_store.ABUX_STORE5_L2n.qp08(), top.sprite_store.ANED_STORE5_L3n.qp08()),
    pack_n(top.sprite_store.FUSA_STORE5_X0p.qn08(), top.sprite_store.FAXA_STORE5_X1p.qn08(), top.sprite_store.FOZY_STORE5_X2p.qn08(), top.sprite_store.FESY_STORE5_X3p.qn08(), top.sprite_store.CYWE_STORE5_X4p.qn08(), top.sprite_store.DYBY_STORE5_X5p.qn08(), top.sprite_store.DURY_STORE5_X6p.qn08(), top.sprite_store.CUVY_STORE5_X7p.qn08())
  );

  d("STORE6 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.YBEZ_STORE6_MATCHn,
    top.sprite_store.WOMY_STORE6_RSTp.qp17(),
    pack_n(top.sprite_store.GABO_STORE6_I0n.qp08(), top.sprite_store.GACY_STORE6_I1n.qp08(), top.sprite_store.FOGO_STORE6_I2n.qp08(), top.sprite_store.GOHU_STORE6_I3n.qp08(), top.sprite_store.FOXY_STORE6_I4n.qp08(), top.sprite_store.GECU_STORE6_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.ZUMY_STORE6_L0n.qp08(), top.sprite_store.ZAFU_STORE6_L1n.qp08(), top.sprite_store.ZEXO_STORE6_L2n.qp08(), top.sprite_store.ZUBE_STORE6_L3n.qp08()),
    pack_n(top.sprite_store.YCOL_STORE6_X0p.qn08(), top.sprite_store.YRAC_STORE6_X1p.qn08(), top.sprite_store.YMEM_STORE6_X2p.qn08(), top.sprite_store.YVAG_STORE6_X3p.qn08(), top.sprite_store.ZOLY_STORE6_X4p.qn08(), top.sprite_store.ZOGO_STORE6_X5p.qn08(), top.sprite_store.ZECU_STORE6_X6p.qn08(), top.sprite_store.ZESA_STORE6_X7p.qn08())
  );

  d("STORE7 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.DYKA_STORE7_MATCHn,
    top.sprite_store.WAPO_STORE7_RSTp.qp17(),
    pack_n(top.sprite_store.GULE_STORE7_I0n.qp08(), top.sprite_store.GYNO_STORE7_I1n.qp08(), top.sprite_store.FEFA_STORE7_I2n.qp08(), top.sprite_store.FYSU_STORE7_I3n.qp08(), top.sprite_store.GESY_STORE7_I4n.qp08(), top.sprite_store.FUZO_STORE7_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.XYNA_STORE7_L0n.qp08(), top.sprite_store.YGUM_STORE7_L1n.qp08(), top.sprite_store.XAKU_STORE7_L2n.qp08(), top.sprite_store.XYGO_STORE7_L3n.qp08()),
    pack_n(top.sprite_store.ERAZ_STORE7_X0p.qn08(), top.sprite_store.EPUM_STORE7_X1p.qn08(), top.sprite_store.EROL_STORE7_X2p.qn08(), top.sprite_store.EHYN_STORE7_X3p.qn08(), top.sprite_store.FAZU_STORE7_X4p.qn08(), top.sprite_store.FAXE_STORE7_X5p.qn08(), top.sprite_store.EXUK_STORE7_X6p.qn08(), top.sprite_store.FEDE_STORE7_X7p.qn08())
  );

  d("STORE8 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.EFYL_STORE8_MATCHn,
    top.sprite_store.EXUQ_STORE8_RSTp.qp17(),
    pack_n(top.sprite_store.AXUV_STORE8_I0n.qp08(), top.sprite_store.BADA_STORE8_I1n.qp08(), top.sprite_store.APEV_STORE8_I2n.qp08(), top.sprite_store.BADO_STORE8_I3n.qp08(), top.sprite_store.BEXY_STORE8_I4n.qp08(), top.sprite_store.BYHE_STORE8_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.AZAP_STORE8_L0n.qp08(), top.sprite_store.AFYX_STORE8_L1n.qp08(), top.sprite_store.AFUT_STORE8_L2n.qp08(), top.sprite_store.AFYM_STORE8_L3n.qp08()),
    pack_n(top.sprite_store.EZUF_STORE8_X0p.qn08(), top.sprite_store.ENAD_STORE8_X1p.qn08(), top.sprite_store.EBOW_STORE8_X2p.qn08(), top.sprite_store.FYCA_STORE8_X3p.qn08(), top.sprite_store.GAVY_STORE8_X4p.qn08(), top.sprite_store.GYPU_STORE8_X5p.qn08(), top.sprite_store.GADY_STORE8_X6p.qn08(), top.sprite_store.GAZA_STORE8_X7p.qn08())
  );

  d("STORE9 M%d R%d I%02d L%02d X%03d\n",
    !top.sprite_store.YGEM_STORE9_MATCHn,
    top.sprite_store.FONO_STORE9_RSTp.qp17(),
    pack_n(top.sprite_store.YBER_STORE9_I0n.qp08(), top.sprite_store.YZOR_STORE9_I1n.qp08(), top.sprite_store.XYFE_STORE9_I2n.qp08(), top.sprite_store.XOTU_STORE9_I3n.qp08(), top.sprite_store.XUTE_STORE9_I4n.qp08(), top.sprite_store.XUFO_STORE9_I5n.qp08(), 1, 1),
    pack_n(top.sprite_store.CANA_STORE9_L0n.qp08(), top.sprite_store.FOFO_STORE9_L1n.qp08(), top.sprite_store.DYSY_STORE9_L2n.qp08(), top.sprite_store.DEWU_STORE9_L3n.qp08()),
    pack_n(top.sprite_store.XUVY_STORE9_X0p.qn08(), top.sprite_store.XERE_STORE9_X1p.qn08(), top.sprite_store.XUZO_STORE9_X2p.qn08(), top.sprite_store.XEXA_STORE9_X3p.qn08(), top.sprite_store.YPOD_STORE9_X4p.qn08(), top.sprite_store.YROP_STORE9_X5p.qn08(), top.sprite_store.YNEP_STORE9_X6p.qn08(), top.sprite_store.YZOF_STORE9_X7p.qn08())
  );

  d("\n");


  d("\002=====TileFetcher=====\001\n");
  d("LAXU_BFETCH_S0           %c\n", top.tile_fetcher.LAXU_BFETCH_S0.c());
  d("MESU_BFETCH_S1           %c\n", top.tile_fetcher.MESU_BFETCH_S1.c());
  d("NYVA_BFETCH_S2           %c\n", top.tile_fetcher.NYVA_BFETCH_S2.c());
  d("LYZU_BFETCH_S0_D1        %c\n", top.tile_fetcher.LYZU_BFETCH_S0_D1.c());
  d("\n");
  d("NYKA_FETCH_DONE_P11      %c\n", top.tile_fetcher.NYKA_FETCH_DONE_P11.c());
  d("PORY_FETCH_DONE_P12      %c\n", top.tile_fetcher.PORY_FETCH_DONE_P12.c());
  d("PYGO_FETCH_DONE_P13      %c\n", top.tile_fetcher.PYGO_FETCH_DONE_P13.c());
  d("POKY_PRELOAD_DONEp       %c\n", top.tile_fetcher.POKY_PRELOAD_LATCHp.c());
  d("\n");
  d("LONY_FETCH_RUNNINGp      %c\n", top.tile_fetcher.LONY_BG_FETCH_RUNNINGp.c()); // 1 for phases 0-11, 0 for 12-15
  d("LOVY_FETCH_DONEp         %c\n", top.tile_fetcher.LOVY_BG_FETCH_DONEp.c());    // 0 for phases 0-11, 1 for 12-15
  d("\n");
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
