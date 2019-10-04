#include "MetroBoy.h"

#include "Audio.h"

#include <assert.h>

extern uint8_t rom_buf[];
extern const int op_sizes[];

#pragma warning(disable : 4996)

//-----------------------------------------------------------------------------

MetroBoy::MetroBoy()
: current_gameboy(new Gameboy()),
  history_frame(),
  history_line(),
  history_cycle(),
  cycles(0),
  trace(true)
{
  memset(current_gameboy->framebuffer, 0, sizeof(current_gameboy->framebuffer));
  memset(tracebuffer, 0, sizeof(tracebuffer));
  gb_out = {0};
}

//-----------------------------------------------------------------------------

void MetroBoy::load_rom(int model, const char* filename, bool run_bootrom) {
  FILE* rom_file = fopen(filename, "rb");
  fseek(rom_file, 0, SEEK_END);
  size_t rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  rom_size = fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);

  current_gameboy->reset(model, rom_size, run_bootrom ? 0x0000 : 0x0100);
}

void MetroBoy::load_dump() {
  FILE* dump_file = fopen("dump.MetroBoy", "rb");
  size_t size = fread(current_gameboy, 1, sizeof(Gameboy), dump_file);
  assert(size == sizeof(Gameboy));
  size = fread(rom_buf, 1, 1024 * 1024, dump_file);
  assert(size == 1024 * 1024);
  fclose(dump_file);
}

void MetroBoy::save_dump() {
  FILE* dump_file = fopen("dump.MetroBoy", "wb");
  fwrite(current_gameboy, 1, sizeof(Gameboy), dump_file);
  fwrite(rom_buf, 1, 1024 * 1024, dump_file);
  fclose(dump_file);
}

//-----------------------------------------------------------------------------

void MetroBoy::reset(uint16_t new_pc) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  memset(tracebuffer, 0, sizeof(tracebuffer));

  current_gameboy->reset(new_pc);
}

//-----------------------------------------------------------------------------

void MetroBoy::run_fast(uint8_t buttons, int fast_cycles) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  current_gameboy->set_buttons(~buttons);

  for (int cycle = 0; cycle < fast_cycles; cycle++) mcycle();
}

//-------------------------------------

void MetroBoy::run_vsync(uint8_t buttons) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  current_gameboy->set_buttons(~buttons);

  while(gb_out.y != 144) {
    cycle();
  }

  audio_begin();

  for (int i = 0; i < 154 * 114; i++) {
    mcycle();
    audio_post(gb_out.out_l, gb_out.out_r);
  }

  audio_end();
}

//-------------------------------------

void MetroBoy::run_to(uint16_t breakpoint) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  while (1) {
    uint16_t pc = current_gameboy->get_cpu().get_pc();
    if (pc == breakpoint) break;
    cycle();
  }
}

//-------------------------------------

void MetroBoy::step_frame() {
  clear_line_history();
  clear_cycle_history();

  history_frame.push_back(current_gameboy);

  current_gameboy = new Gameboy();
  memcpy(current_gameboy, history_frame.back(), sizeof(Gameboy));

  do {
    mcycle();
  } while (gb_out.y == 144);

  do {
    mcycle();
  } while (gb_out.y != 144);
}

void MetroBoy::step_line() {
  clear_cycle_history();

  history_line.push_back(current_gameboy);

  current_gameboy = new Gameboy();
  memcpy(current_gameboy, history_line.back(), sizeof(Gameboy));

  int line = gb_out.y;
  do {
    mcycle();
  } while (gb_out.y == line);
}

void MetroBoy::step_cycle() {
  history_cycle.push_back(current_gameboy);

  current_gameboy = new Gameboy();
  memcpy(current_gameboy, history_cycle.back(), sizeof(Gameboy));
  cycle();
}

void MetroBoy::step_over() {
  history_cycle.push_back(current_gameboy);

  current_gameboy = new Gameboy();
  memcpy(current_gameboy, history_cycle.back(), sizeof(Gameboy));

  //int op = current_gameboy->get_op();
  int pc = current_gameboy->get_cpu().get_pc();
  int op = rom_buf[pc];
  int op_size = op_sizes[op];
  if (op == 0xcb) op_size = 2;

  int next_pc = pc + op_size;

  int i = 0;
  for (; i < 1000000; i++) {
    if (current_gameboy->get_cpu().get_pc() == next_pc) {
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

void MetroBoy::unstep_frame() {
  if (history_frame.empty()) return;

  delete current_gameboy;

  current_gameboy = history_frame.back();
  history_frame.pop_back();
}

void MetroBoy::unstep_line() {
  if (history_line.empty()) return;

  delete current_gameboy;

  current_gameboy = history_line.back();
  history_line.pop_back();
}

void MetroBoy::unstep_cycle() {
  if (history_cycle.empty()) return;

  delete current_gameboy;

  current_gameboy = history_cycle.back();
  history_cycle.pop_back();
}

//-----------------------------------------------------------------------------

void MetroBoy::clear_frame_history() {
  for (auto g : history_frame) delete g;
  history_frame.clear();
}

void MetroBoy::clear_line_history() {
  for (auto g : history_line) delete g;
  history_line.clear();
}

void MetroBoy::clear_cycle_history() {
  for (auto g : history_cycle) delete g;
  history_cycle.clear();
}

//-----------------------------------------------------------------------------

void MetroBoy::cycle() {
  current_gameboy->tock();
  gb_out = current_gameboy->tick();

  if (trace) {
    tracebuffer[gb_out.y * 456 + gb_out.counter] = gb_out.trace;
  }

  if (gb_out.pix_oe) {
    int x = gb_out.x - 1;
    int y = gb_out.y;

    if (x >= 0 && x < 160 && y >= 0 && y < 144) {
      current_gameboy->framebuffer[x + y * 160] = gb_out.pix;
    }
  }

  current_gameboy->check_sentinel();
  cycles++;
}

void MetroBoy::mcycle() {
  cycle();
  if ((current_gameboy->get_tcycle() & 3) == 0) return;
  cycle();
  if ((current_gameboy->get_tcycle() & 3) == 0) return;
  cycle();
  if ((current_gameboy->get_tcycle() & 3) == 0) return;
  cycle();
}

//-----------------------------------------------------------------------------
