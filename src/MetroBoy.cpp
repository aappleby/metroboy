#include "MetroBoy.h"

#include "Audio.h"

#include <assert.h>

extern uint8_t rom_buf[];
extern const int op_sizes[];

#pragma warning(disable : 4996)

//-----------------------------------------------------------------------------

StateManager::StateManager()
{
  gb_out = {};
  current_gb = new Gameboy();
  current_fb = new Framebuffer();
  cycles = 0;
  trace = true;
  memset(tracebuffer, 0, sizeof(tracebuffer));
}

//-----------------------------------------------------------------------------

void StateManager::load_rom(const char* filename, bool run_bootrom) {
  FILE* rom_file = fopen(filename, "rb");
  fseek(rom_file, 0, SEEK_END);
  size_t rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  rom_size = fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);

  current_gb->reset(rom_size, run_bootrom ? 0x0000 : 0x0100);
}

void StateManager::load_dump() {
  FILE* dump_file = fopen("dump.MetroBoy", "rb");
  size_t size = fread(current_gb, 1, sizeof(Gameboy), dump_file);
  assert(size == sizeof(Gameboy));
  size = fread(rom_buf, 1, 1024 * 1024, dump_file);
  assert(size == 1024 * 1024);
  fclose(dump_file);
}

void StateManager::save_dump() {
  FILE* dump_file = fopen("dump.MetroBoy", "wb");
  fwrite(current_gb, 1, sizeof(Gameboy), dump_file);
  fwrite(rom_buf, 1, 1024 * 1024, dump_file);
  fclose(dump_file);
}

//-----------------------------------------------------------------------------

void StateManager::reset(uint16_t new_pc) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  memset(tracebuffer, 0, sizeof(tracebuffer));

  current_gb->reset(new_pc);
}

//-----------------------------------------------------------------------------

void StateManager::run_fast(uint8_t buttons, int fast_cycles) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  current_gb->set_buttons(~buttons);

  for (int cycle = 0; cycle < fast_cycles; cycle++) mcycle();
}

//-------------------------------------

void StateManager::run_vsync(uint8_t buttons) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  current_gb->set_buttons(~buttons);

  while(gb_out.y != 144) {
    cycle();
  }

  audio_begin();

  for (int i = 0; i < 154 * 114; i++) {
    mcycle();
    audio_post(current_gb->get_host_data().out_l, current_gb->get_host_data().out_r);
  }

  audio_end();
}

//-------------------------------------

void StateManager::run_to(uint16_t breakpoint) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  while (1) {
    uint16_t pc = current_gb->get_cpu().get_pc();
    if (pc == breakpoint) break;
    cycle();
  }
}

//-------------------------------------

void StateManager::step_frame() {
  clear_line_history();
  clear_cycle_history();

  gb_frame.push_back(current_gb);
  fb_frame.push_back(current_fb);

  current_gb = new Gameboy();
  current_fb = new Framebuffer();
  memcpy(current_gb, gb_frame.back(), sizeof(Gameboy));
  memcpy(current_fb, fb_frame.back(), sizeof(Framebuffer));

  do {
    mcycle();
  } while (gb_out.y == 144);

  do {
    mcycle();
  } while (gb_out.y != 144);
}

void StateManager::step_line() {
  clear_cycle_history();

  gb_line.push_back(current_gb);
  fb_line.push_back(current_fb);

  current_gb = new Gameboy();
  current_fb = new Framebuffer();
  memcpy(current_gb, gb_line.back(), sizeof(Gameboy));
  memcpy(current_fb, fb_line.back(), sizeof(Framebuffer));

  int line = gb_out.y;
  do {
    mcycle();
  } while (gb_out.y == line);
}

void StateManager::step_cycle() {
  gb_cycle.push_back(current_gb);
  fb_cycle.push_back(current_fb);

  current_gb = new Gameboy();
  current_fb = new Framebuffer();
  memcpy(current_gb, gb_cycle.back(), sizeof(Gameboy));
  memcpy(current_fb, fb_cycle.back(), sizeof(Framebuffer));

  cycle();
}

void StateManager::step_over() {
  gb_cycle.push_back(current_gb);
  fb_cycle.push_back(current_fb);

  current_gb = new Gameboy();
  current_fb = new Framebuffer();
  memcpy(current_gb, gb_cycle.back(), sizeof(Gameboy));
  memcpy(current_fb, fb_cycle.back(), sizeof(Framebuffer));

  //int op = current_gb->get_op();
  int pc = current_gb->get_cpu().get_pc();
  int op = rom_buf[pc];
  int op_size = op_sizes[op];
  if (op == 0xcb) op_size = 2;

  int next_pc = pc + op_size;

  int i = 0;
  for (; i < 1000000; i++) {
    if (current_gb->get_cpu().get_pc() == next_pc) {
      // step succeeded
      return;
    }
    cycle();
  }

  // step failed
  unstep_cycle();
  return;
}

//-----------------------------------------------------------------------------

void StateManager::unstep_frame() {
  if (gb_frame.empty()) return;

  delete current_gb;
  delete current_fb;

  current_gb = gb_frame.back();
  current_fb = fb_frame.back();

  gb_frame.pop_back();
  fb_frame.pop_back();
}

void StateManager::unstep_line() {
  if (gb_line.empty()) return;

  delete current_gb;
  delete current_fb;

  current_gb = gb_line.back();
  current_fb = fb_line.back();

  gb_line.pop_back();
  fb_line.pop_back();
}

void StateManager::unstep_cycle() {
  if (gb_cycle.empty()) return;

  delete current_gb;
  delete current_fb;

  current_gb = gb_cycle.back();
  current_fb = fb_cycle.back();

  gb_cycle.pop_back();
  fb_cycle.pop_back();
}

//-----------------------------------------------------------------------------

void StateManager::clear_frame_history() {
  for (auto g : gb_frame) delete g;
  for (auto f : fb_frame) delete f;
  gb_frame.clear();
  fb_frame.clear();
}

void StateManager::clear_line_history() {
  for (auto g : gb_line) delete g;
  for (auto f : fb_line) delete f;
  gb_line.clear();
  fb_line.clear();
}

void StateManager::clear_cycle_history() {
  for (auto g : gb_cycle) delete g;
  for (auto f : fb_cycle) delete f;
  gb_cycle.clear();
  fb_cycle.clear();
}

//-----------------------------------------------------------------------------

void StateManager::cycle() {
  current_gb->tick();
  current_gb->tock();

  gb_out = current_gb->get_host_data();

  if (trace) {
    tracebuffer[gb_out.y * 456 + gb_out.counter] = gb_out.trace;
  }

  if (gb_out.pix_oe) {
    int x = gb_out.x - 1;
    int y = gb_out.y;

    if (x >= 0 && x < 160 && y >= 0 && y < 144) {
      current_fb->buf[x + y * 160] = gb_out.pix;
    }
  }

  current_gb->check_sentinel();
  cycles++;
}

void StateManager::mcycle() {
  cycle();
  if ((current_gb->get_tcycle() & 3) == 0) return;
  cycle();
  if ((current_gb->get_tcycle() & 3) == 0) return;
  cycle();
  if ((current_gb->get_tcycle() & 3) == 0) return;
  cycle();
}

//-----------------------------------------------------------------------------
